#pragma once

constexpr uint8_t hw_version[4] = { 79, 0, 0, 1 };                         // Версия железа  
constexpr uint8_t revision[4]   = { 'S', 'V', 0x00, 0x01 };                // Исполнение
constexpr uint32_t ser_num      = 1234;                                    // Серийный номер
constexpr uint8_t gw_ip[4]      = { 10, 0, 0, 1 };                         // IP-адрес шлюза
constexpr uint8_t net_mask[4]   = { 255, 0, 0, 0 };                        // маска подсети
constexpr uint8_t mac_adr[6]    = { 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, }; // MAC-адрес
constexpr uint8_t baconc_ip[4]  = { 10, 0, 0, 2 };                         // IP-адрес bacon
constexpr uint8_t dst_ip[4]     = { 10, 255, 255, 255 };                   // IP-адрес получателя
constexpr uint16_t data_udp     = 8888;                                    // UDP-порт данные
constexpr uint16_t cntrl_udp    = 1028;                                    // UDP-порт управление

// Тестовый профиль
constexpr uint8_t test_profile[64] = 
{
    1,0,0,0,             // 0x00 taskID
    1,                   // 0x04 profile ID
    1,                   // 0x05 blocksize
    5, 0,                // 0x06 itarations (младшим вперёд)
    0xbb,0xbb,0xbb,0x3b, // 0x08 FTW
    0x9d,0xa6,0x0b,0,    // 0x0C DFTW
    1,0,0,0,             // 0x10 DFRRW
    0x90,0x01,           // 0x14 period      (младшим вперёд, шаг 160 нс)  (64 мкс)
    0,0,                 // 0x16 резерв
    200,0,               // 0x18 dds_start   (младшим вперёд, шаг 16 нс )  (3,2 мкс)
    0xC2,0x01,           // 0x1A dds_stop                                  (7,2 мкс)
    187,0,               // 0x1C prd_start   (младшим вперёд, шаг 16 нс )  (3 мкс)
    0xCE,0x01,           // 0x1E prd_stop                                  (7,4 мкс)
    50,0,                // 0x20 adc_start
    0x76,1,              // 0x22 adc_stop
    49,0,                // 0x24 prm_start                                 (8 мкс)
    0x77,1,              // 0x26 prm_stop                                  (60 мкс)
    3,                   // 0x28 signal_type
    0,                   // 0x29 presume
    0,                   // 0x2A decimation
    0x0F,                // 0x2B chanels
    0,                   // 0x2C RCPattern
    0,                   // 0x2D TRPattern
    0,                   // 0x2E Polarization
    0,                   // 0x2F IFF
    0x00,0x00,0x00,0x00, // 0x30 
    0x00,                // 0x34 mode_str  <-должен быть тут, но нет
    0x00,0x00,0x00,      // 0x35    
    0x0E,                // 0x38 !!! mode_str !!!
    0x00,                // 0x39 
    0x00,                // 0x3A 
    0x00,                // 0x3B 
    0x00,0x00,0x00,0x00  // 0x3C  
};

//========================================================================
// Класс управления блоком BACON
//========================================================================
struct TBACON
{
  TBACON() =default; //{ ClearProfiles(); } 

  // Функция разбора команды записи регистров(памяти) через интерфес управления
  auto Process_Write_Control_CMD(uint32_t adr, uint32_t len, uint8_t *rx_buf)
  {
    DEBUG_Print("W: A=%x L=%d\r\n",adr,len);

    while( (adr<1024) && (len>0))
    { // Диапазон регистров управления bacon
      bacon_regs[adr++] = *rx_buf++;
      len--;
    }

    while( (adr<3072) && (len>0))
    { // Диапазон регистров профилей
      uint32_t prof_num =  (adr-1024)>>6;    // Номер записываемого профиля
      uint32_t prof_elem = (adr-1024)&0x3F;  // Номер первого записываемого в профиль элемента      
      volatile uint8_t *pbuf = (uint8_t *)(PROFILE_BRAM_BASE + 64*prof_num + prof_elem);
      DEBUG_Print("W: Pr:%d Start:%d\r\n", prof_num+1, prof_elem);
      do 
      {
        *pbuf++ = *rx_buf++;         
        len--; prof_elem++; adr++;
      }
      while( (len>0) && (adr<3072));     
    }

    while(len>0)
    {     
      len--;
    } 

    return CONTROL_IF::ERROR_STATUS::Ok;
  }

  // Функция разбора команды чтения регистров(памяти) через интерфес управления
  auto Process_Read_Control_CMD(uint32_t adr, uint32_t len, uint8_t *tx_buf)
  {
    while( (adr<1024) && (len>0))
    { // Диапазон регистров управления bacon
      *tx_buf++ = bacon_regs[adr++];
      len--;
    }

    while( (adr<3072) && (len>0))
    { // Диапазон регистров профилей
            
      uint32_t prof_num =  (adr-1024)>>6;    // Номер записываемого профиля
      uint32_t prof_elem = (adr-1024)&0x3F;  // Номер первого записываемого в профиль элемента
      volatile uint8_t *pbuf = (uint8_t *)(PROFILE_BRAM_BASE + 64*prof_num + prof_elem);
      DEBUG_Print("R: Pr:%d Start:%d\r\n", prof_num+1, prof_elem);
      do 
      {
        *tx_buf++ = *pbuf++;
        len--; prof_elem++; adr++;
      }
      while( (len>0) && (adr<3072));
      
    }

    while(len>0)
    {
      *tx_buf++ = 0;
      len--;
    }      
    
    return CONTROL_IF::ERROR_STATUS::Ok;
  }

  // Функция записи тестового профиля
  template <uint32_t BASE_ADR = PROFILE_BRAM_BASE>
  void WriteTestProfile(uint32_t num)
  {
    if constexpr(BASE_ADR == PROFILE_BRAM_BASE)
    { // Для железа bacon
      volatile uint8_t *pbuf = (uint8_t *)(BASE_ADR + num*64);
      for(auto i=0u; i<64; i++)
      {  
        switch(i)
        {
          case 4: 
            *pbuf++ = num+1;
            break;
          case 6:
              if((num<1) || (num>2)) *pbuf++ = 0; else *pbuf++ = test_profile[i];
            break;
          default:
            *pbuf++ = test_profile[i];
            break;
        }
      }      
    }
    else
    { // Для железа БАЦО
      volatile uint8_t *pbuf = (uint8_t *)(BCO_PROFILE_BASE);  
      volatile uint32_t *preg = (uint32_t *)(BCO_PROFILE_BASE);
      *(preg+63) = num;          // Установить номер профиля
      for(auto i=0u; i<120; i++)
      {  
        if(i<64) *pbuf++ = test_profile[i];
        else *pbuf++ = i;
      }
      *(preg+62) = 0x8000'0000;   // Включить перекодировку профиля   
    }
  }

  template <uint32_t BASE_ADR = PROFILE_BRAM_BASE>
  void PrintProfile(uint32_t num)
  {
    if constexpr(BASE_ADR == PROFILE_BRAM_BASE)
    { // Для железа bacon
/*    
      volatile uint8_t *pbuf = (uint8_t *)(BACON_PROFILE_BASE);
      volatile uint32_t *preg = (uint32_t *)(BACON_PROFILE_BASE);
      *(preg+16)=num;
      DEBUG_Print("\r\nBACON Profile(%d): ", *(preg+16));
      for(auto i=0u; i<64; i++)
      {          
        DEBUG_Print("%02x ", *pbuf++);
      }
*/      
    }
    else
    { // Для железа БАЦО
      volatile uint8_t *pbuf = (uint8_t *)(BCO_PROFILE_BASE);
      volatile uint32_t *preg = (uint32_t *)(BCO_PROFILE_BASE);
      *(preg+63) = num;
      DEBUG_Print("\r\nBCO Profile(%d): ", *(preg+63));
      for(auto i=0u; i<128; i++)
      {  
        DEBUG_Print("%02x ", *pbuf++);
      }
    }
  }

private:

  // Очистить все профили
  void ClearProfiles()
  {
    //volatile uint32_t *preg = (uint32_t *)(BACON_PROFILE_BASE);
    //for(auto i=0; i<32; i++)
    //{
    //  *(preg+16) = i;
    //  for (auto j=0u; j<16; j++) *(preg+j)=0;
    //}
  }

  uint8_t bacon_regs[1024] = 
  { 0,                                                          // 0x00 резерв
    gw_ip[0], gw_ip[1], gw_ip[2], gw_ip[3],                     // 0x01 
    net_mask[0], net_mask[1], net_mask[2], net_mask[3],         // 0x05
    mac_adr[0], mac_adr[1], mac_adr[2],                         // 0x09
    mac_adr[3], mac_adr[4], mac_adr[5], 
    baconc_ip[0], baconc_ip[1], baconc_ip[2], baconc_ip[3],     // 0x0F
    0,0,0,0,0,0,0,                                              // 0x13 резерв
    dst_ip[0], dst_ip[1], dst_ip[2], dst_ip[3],                 // 0x1A
    data_udp&0xFF, data_udp>>8,                                 // 0x1E
    cntrl_udp&0xFF, cntrl_udp>>8,                               // 0x20
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                // 0x22
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                            // 0x30
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                            // 0x40
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                            // 0x50
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                            // 0x60
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                            // 0x70
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                            // 0x80
    ser_num&0xFF, (ser_num>>8)&0xFF,                            // 0x90
    (ser_num>>16)&0xFF, (ser_num>>24)&0xFF,                     // 0x92
    revision[3], revision[2], revision[1], revision[0],         // 0x94 
    0,0,0,0,                                                    // 0x98
    hw_version[3], hw_version[2], hw_version[1], hw_version[0]  // 0x9C
  };

};

extern TBACON bacon;

