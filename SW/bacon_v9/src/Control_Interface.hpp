#pragma once

#include <type_traits>

// Функции обработки прерываний от управляющих интерфейсов
void __attribute__((fast_interrupt)) UartRS485PC_InterruptHandler();
void __attribute__((fast_interrupt)) UartUSB_InterruptHandler();
void __attribute__((fast_interrupt)) UartRS485AFAR_InterruptHandler();
//void __attribute__((fast_interrupt)) AXIS_FIFO_RX_InterruptHandler();

namespace CONTROL_IF
{
// Константы заголовка сообщения управляющего интерфейса
constexpr auto PROTOCOL_VERSION = 2;                        // Версия протокола
constexpr auto PREFIX = 0x2222'2233;                        // Префикс заголовка
enum class MSG_TYPE : uint8_t { CMD=34, REPLY=35 };         // Идентификатор сообщения
enum class DEV_TYPE : uint8_t { RLS=1, UPR=2 };             // Идентификаторы устройств
enum class MSG_LEN { MIN=44, MAX=2048 };                    // Допустимая длина сообщения
enum class IDENT : uint32_t { IN = uint32_t(MSG_TYPE::CMD)
                                 + uint32_t(PROTOCOL_VERSION<<8)
                                 + (uint32_t(DEV_TYPE::UPR)<<16)
                                 + (uint32_t(DEV_TYPE::RLS)<<24),
                              OUT = uint32_t(MSG_TYPE::REPLY)
                                  + uint32_t(PROTOCOL_VERSION<<8)
                                  + (uint32_t(DEV_TYPE::RLS)<<16)
                                  + (uint32_t(DEV_TYPE::UPR)<<24) };
// Константы информационной части команды
enum class OPERATION_TYPE : uint32_t { Write=0xFFFF'0012, Read=0xFFFF'0013 };
enum class DATA_LEN { MIN=0, MAX=2004 };

#define p32TX ((uint32_t *)p8TX)
#define p32RX ((uint32_t *)p8RX)

template <typename TPORT, typename TMKS=void>
struct TCONTROL_IF
{
  constexpr TCONTROL_IF(uint8_t *rx, uint8_t *tx)
  {
    p8RX = rx;
    p8TX = tx;
    p32TX[0]=PREFIX;
    p32RX[0]=PREFIX;
    p32TX[2]=(uint32_t)IDENT::OUT;
    msg_num = 1;
  }

  // Приём из пословных интерфейсов
  void ProcessWord(uint32_t rw)
  {
    if(cmd_received) return;
    switch(rcv_counter)
    {
      case 0:
        if (rw==PREFIX) rcv_counter++;
        break;
      case 1:
        p32RX[rcv_counter++] = rw;
        {
          uint32_t msg_len = rw;
          if((msg_len>uint32_t(MSG_LEN::MAX)) || (msg_len<uint32_t(MSG_LEN::MIN)) || ((msg_len&0x3)!=0))
          {
            IF_DEBUG_Print("IF: Wrong length\r\n");
            rcv_counter=0;
          }
        }
        break;
      default:
        p32RX[rcv_counter++] = rw;
        if(rcv_counter == (p32RX[1]>>2))
        {
          if(p32RX[2] == uint32_t(IDENT::IN)) cmd_received=true;
          else
          { // Неправильный протокол или команда не мне
            IF_DEBUG_Print("IF: Wrong protocol\r\n");
          }
          rcv_counter=0;   
        }
        break;             
    }
  }

  // Приём из побайтовых интерфейсов
  void ProcessByte(uint8_t rb)
  {
    switch(rcv_counter)
    {
      case 0:
        if(rb == (PREFIX&0xFF)) rcv_counter++;
        break;
      case 1:
        if(rb == ((PREFIX>>8)&0xFF)) rcv_counter++;
        else rcv_counter = rb==(PREFIX&0xFF) ? 1 : 0;
        break;
      case 2:
        if(rb == ((PREFIX>>16)&0xFF)) rcv_counter++;
        else rcv_counter = rb==(PREFIX&0xFF) ? 1 : 0;
        break;
      case 3:
        if(rb == ((PREFIX>>24)&0xFF)) rcv_counter++;
        else rcv_counter = rb==(PREFIX&0xFF) ? 1 : 0;
        break;
      case 4:
      case 5:
      case 6:
        p8RX[rcv_counter++] = rb;
        break;
      case 7:
        p8RX[rcv_counter++] = rb;
        {
          uint32_t msg_len = p32RX[1];
          if((msg_len>uint32_t(MSG_LEN::MAX)) || (msg_len<uint32_t(MSG_LEN::MIN)) || ((msg_len&0x3)!=0))
          {
            IF_DEBUG_Print("IF: Wrong length\r\n");
            rcv_counter=0;
          }
        }
        break;
      default:
        p8RX[rcv_counter++] = rb;
        if(rcv_counter == p32RX[1])
        {
          if(p32RX[2] == uint32_t(IDENT::IN)) cmd_received=true;
          else
          { // Неправильный протокол или команда не мне
            IF_DEBUG_Print("IF: Wrong protocol\r\n");
          }
          rcv_counter=0;
        }
        break;
    }
  }

  // Выдать статус есть ли принятая команда
  bool isCommandReceived() { return cmd_received; }

  // Разбор информационной части команды управляющего интерфейса
  void CommandProcessing()
  {
    if(Check_RX_CRC())
    {
      ERROR_STATUS status = ERROR_STATUS::Ok;
      switch(OPERATION_TYPE(p32RX[8]))
      {
        case OPERATION_TYPE::Write:
          status = WriteCMD();
          break;
        case OPERATION_TYPE::Read:
          status = ReadCMD();
          break;
        default:
          p32TX[1] = 40;  // Reply LEN   ??? Спросить у Олега
          status = ERROR_STATUS::WrongCMD;
          break;
      }

      // Номер сообщения
      p32TX[3] = msg_num++; if(msg_num==0) msg_num++;
      // Номер сообщения на которое отвечаем
      p32TX[4] = p32RX[3];
      // Локальное время в мкс
      if constexpr (!std::is_same_v<void,TMKS>)
      {
        uint64_t time_mks = TMKS::Read_mks();  
        p32TX[5] = time_mks;
        p32TX[6] = time_mks>>32;        
      }
      // Первое слово ответа информационной части
      p32TX[8] = p8RX[32] + (uint32_t(MSG_TYPE::CMD)<<8) + (uint32_t(status)<<16);

      Calculate_TX_CRC();

      TPORT::Write(p8TX, p32TX[1]);
      
      switch(status)
      {
        case ERROR_STATUS::Ok :
          IF_DEBUG_Print(" Ok");
          break;
        case ERROR_STATUS::Fault :
          IF_DEBUG_Print("IF: Fault");
          break;
        case ERROR_STATUS::WrongCMD :
          IF_DEBUG_Print("IF: WrongCMD");
          break;
        case ERROR_STATUS::WrongData :
          IF_DEBUG_Print(" WrongDATA");
          break;
       }
       IF_DEBUG_Print(" -> Reply LEN=%d ", p32TX[1]);
    }
    else
    {
      IF_DEBUG_Print("IF: WrongCRC\r\n");
    }
    cmd_received=false;
  }

private:
  inline ERROR_STATUS ReadCMD()
  {
    uint32_t tmp = p32RX[9];
    const uint32_t reqvested_len = tmp>>16;
#ifdef __MICROBLAZE__
    const uint32_t reqvested_adr = mb_swaph(mb_swapb(tmp))&0xFFFF;
#else
    const uint32_t reqvested_adr = ((tmp>>8)&0xFF) + ((tmp&0xFF)<<8);
#endif
    IF_DEBUG_Print("IF: Read ADR=0x%x LEN=%d", reqvested_adr, reqvested_len);
    p32TX[9] = p32RX[9];  // Adr and Size
    if(reqvested_len<=uint16_t(DATA_LEN::MAX))
    {
      p32TX[1] = (44 + reqvested_len + 3)&0xFFFC; // Reply len ( to word rounded)
      // Начало кода обработки чтения
      // Данные записывать начиная с p8TX[40]
      return bacon.Process_Read_Control_CMD(reqvested_adr, reqvested_len, &p8TX[40]);
      // Конец кода обработки чтения
    }
    else
    {
      p32TX[1] = 44; // Reply len
      return ERROR_STATUS::WrongData;
    }
  }

  inline ERROR_STATUS WriteCMD()
  {
    auto tmp = p32RX[9];
    const auto reqvested_len = tmp>>16;
#ifdef __MICROBLAZE__
    const auto reqvested_adr = mb_swaph(mb_swapb(tmp))&0xFFFF;
#else
    const auto reqvested_adr = ((tmp>>8)&0xFF) + ((tmp&0xFF)<<8);
#endif
    IF_DEBUG_Print("IF: Write ADR=0x%x LEN=%d",reqvested_adr, reqvested_len);
    p32TX[1] = 44;         // Reply len
    p32TX[9] = p32RX[9];   // Adr and Size
    if(reqvested_len<=uint16_t(DATA_LEN::MAX))
    {
      // Начало кода обработки записа
      // Данные читать начиная с p8RX[40]
      return bacon.Process_Write_Control_CMD(reqvested_adr, reqvested_len, &p8RX[40]);
      // Конец кода обработки записа
    }
    else return ERROR_STATUS::WrongData;
  }

  inline bool Check_RX_CRC()
  {
    uint32_t crc = 0;
    auto cnt = p32RX[1]/4-1;
    for(auto i=0u; i<cnt; crc ^= p32RX[i++] );
    if(crc==p32RX[cnt]) return true;
    else
    {
      IF_DEBUG_Print("CRC=%x ",crc);
      return false;
    }
  }

  inline void Calculate_TX_CRC()
  {
    uint32_t crc = 0;
    auto cnt = p32TX[1]/4-1;
    for(auto i=0u; i<cnt; crc ^= p32TX[i++] );
    p32TX[cnt]=crc;
  }

  uint32_t rcv_counter;
  volatile bool cmd_received;
  uint8_t *p8RX; // rx_buf[];
  uint8_t *p8TX; // tx_buf[];
  uint32_t msg_num;
};

} // namespace CONTROL_IF

