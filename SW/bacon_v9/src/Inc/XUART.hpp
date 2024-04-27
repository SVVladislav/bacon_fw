#pragma once

#include <stdint.h>

struct XUART_16550_REGS
{
  union
  {
    volatile uint32_t RBR;
    volatile uint32_t THR;
    volatile uint32_t DLL;
  };
  union
  {
    volatile uint32_t IER;
    volatile uint32_t DLM;
  };
  union
  {
    volatile uint32_t FCR;
    volatile uint32_t IIR;
  };
  volatile uint32_t LCR;
  volatile uint32_t MCR;
  volatile uint32_t LSR;
  volatile uint32_t MSR;
  volatile uint32_t SCR;
};

struct XUART_LITE_REGS
{
  volatile uint32_t RBR;
  volatile uint32_t THR;
  volatile uint32_t STAT;
  volatile uint32_t CTRL;
};

#define UART_STAT_PE                 (1<<7)
#define UART_STAT_FE                 (1<<6)
#define UART_STAT_OE                 (1<<5)
#define UART_STAT_INT_ENABLED        (1<<4)
#define UART_STAT_TXFIFO_FULL        (1<<3)
#define UART_STAT_TXFIFO_EMPTY       (1<<2)
#define UART_STAT_RXFIFO_FULL        (1<<1)
#define UART_STAT_RXFIFO_VALID_DATA  (1<<0)

template <uint32_t pUART>
struct TAURTLITE
{
	constexpr TAURTLITE() = default;

	static inline void EnableInterrupt() { base()->CTRL = 1<<4; }

	static inline void DisableInterrupt() { base()->CTRL = 0; }

	static inline uint8_t ReadByte() { return *(uint8_t *)&base()->RBR; }

	static inline void WriteByte(uint8_t data)
	{
	  while(base()->STAT & UART_STAT_TXFIFO_FULL);
	  base()->THR = data;
	}

	static inline bool isDR() { return ((base()->STAT & UART_STAT_RXFIFO_VALID_DATA)!=0); }

private:
  static inline auto base() { return (XUART_LITE_REGS *) pUART; }
};

#define UART_LCR_DLAB   (1<<7)   // Divisor Latch Access Bit
#define UART_LCR_SB     (1<<6)   // Set Break
#define UART_LCR_SP     (1<<5)   // Stick Parity
#define UART_LCR_ODD    (0<<4)   // Odd parity
#define UART_LCR_EVEN   (1<<4)   // Even parity
#define UART_LCR_PEN    (1<<3)   // Parity Enable
#define UART_LCR_STB_1  (0<<2)   // 1 Stop bit
#define UART_LCR_STB_2  (1<<2)   // 2 Stop bits
#define UART_WLS_5      (0<<0)   // 5 bits/character
#define UART_WLS_6      (1<<0)   // 6 bits/character
#define UART_WLS_7      (2<<0)   // 7 bits/character
#define UART_WLS_8      (3<<0)   // 8 bits/character

#define UART_LSR_DR   (1<<0)
#define UART_LSR_OE   (1<<1)
#define UART_LSR_PE   (1<<2)
#define UART_LSR_FE   (1<<3)
#define UART_LSR_BI   (1<<4)
#define UART_LSR_THRE (1<<5)
#define UART_LSR_TEMT (1<<6)
#define UART_LSR_ERROR_RCVR_FIFO (1<<7)

#define UART_IER_ERBFI   (1<<0)  // Enable Received Data Available Interrupt
#define UART_IER_ETBEI   (1<<1)  // Enable Transmitter Holding Register Empty Interrupt
#define UART_IER_ELSI    (1<<2)  // Enable Receiver Line Status Interrupt
#define UART_IER_EDSSI   (1<<3)  // Enable Modem Status Interrupt

enum class UIER { RBF=UART_IER_ERBFI, TBE=UART_IER_ETBEI,
	              LS = UART_IER_ELSI, DSS=UART_IER_EDSSI };

enum class PARITY    { None = 0, Odd = UART_LCR_PEN|UART_LCR_ODD, Even=UART_LCR_PEN|UART_LCR_EVEN };
enum class STOP_BIST { _1=UART_LCR_STB_1, _2=UART_LCR_STB_2 };
enum class DATA_BIST { _5=UART_WLS_5, _6=UART_WLS_6, _7=UART_WLS_7, _8=UART_WLS_8 };
enum class MODE { FULL_DUPLEX, HALF_DUPLEX };

template <uint32_t pUART,                // Base Address
          uint32_t ref_clk=100000000,    // Reference clock [Hz]
          MODE mode = MODE::HALF_DUPLEX> // R/W Control
class TAURT16550
{
public:
	constexpr TAURT16550() = default;

    template <uint32_t br=9600,           // Baud rate
              PARITY par=PARITY::None, 
              DATA_BIST db=DATA_BIST::_8, 
              STOP_BIST sb=STOP_BIST::_1>
	static inline void Init()
	{
        constexpr uint16_t  dll = (ref_clk + (br*16ul)/2)/(br*16ul);
		base()->SCR = 0;
		base()->IER = 0;
		base()->LCR = UART_LCR_DLAB;
		base()->DLM = dll>>16;
		base()->DLL = dll&0xff;
		base()->FCR = 1 | (1<<3);
		base()->LCR = uint32_t(db) | uint32_t(sb) | uint32_t(par);
	}

	static inline void EnableInterrupt(UIER ei)	{ base()->IER = base()->IER | (uint32_t)ei; }

	static inline void DisableInterrupt(UIER ei) { base()->IER = base()->IER & ~(uint32_t)ei; }

	static inline uint8_t ReadByte() { return *(uint8_t *)&base()->RBR; }

	static inline void WriteByte(uint8_t data)
	{
	  while(!(base()->LSR & UART_LSR_THRE));
	  base()->THR = data;
	}

    static inline void Write(uint8_t *buf, uint32_t size)
    {
      if constexpr(mode==MODE::HALF_DUPLEX) SetTX();
      for(auto i=0u; i<size; WriteByte(buf[i++]));
      if constexpr(mode==MODE::HALF_DUPLEX) SetRX();
    }

	static inline void SetTX() { base()->MCR = 1; }

	static inline void SetRX()
	{
	  while (!(base()->LSR & UART_LSR_TEMT));
	  base()->MCR = 0;
	}

	static inline bool isDR() { return ((base()->LSR & UART_LSR_DR)!=0); }

	static inline auto Regs() { return base(); }

private:
  static inline auto base() { return (XUART_16550_REGS *) (pUART + 0x1000); }
};

//#ifdef UART_MDM_BASE
//  using UART_MDM = TAURTLITE<UART_MDM_BASE>;
//#else
//  #error "UART_MDM_BASE not defined"
//#endif
