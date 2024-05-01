#pragma once

#include <cstdint>
#include <array>

struct SVV_ADI_SPI_REGS
{
  volatile uint32_t SSR;          // Slave Select Registe
    
  volatile uint32_t DR;           // Transmit/Receive Data Register
                                  // Write register operation: 
						          // TDR[23:21] = 0b000
								  // TDR[20:8]  = Rergister Address
								  // TDR[7:0]   = Data
								  // Read register operation:
								  // TDR[7:0]   = XXX
  volatile uint32_t SR;          // Status register
                                 // SR[0] - Operaton in progress
};

enum class WAIT_BUSY { YES, NO };

template<uint32_t PSPI>
struct TADI_SPI
{    
  TADI_SPI() = delete;
  
  static inline void SlaveSelect(uint32_t ss) {  base()->SSR = ss; }

  template <WAIT_BUSY wb=WAIT_BUSY::YES>
  static inline void Write(uint32_t x)
  {
    if constexpr (wb==WAIT_BUSY::YES) { while(isBusy()); }
    base()->DR = x;  
  }

  template <WAIT_BUSY wb=WAIT_BUSY::YES>
  static inline void WriteReg(uint32_t reg, uint8_t value)
  {
    if constexpr (wb==WAIT_BUSY::YES) { while(isBusy()); }
    base()->DR = (reg << 8) | value;
  }

  template <WAIT_BUSY wb=WAIT_BUSY::YES>
  static inline uint32_t ReadReg(uint32_t reg)
  {
    if constexpr (wb==WAIT_BUSY::YES) { while(isBusy()); }
    base()->DR = (reg << 8) | 0x80'0000;
    while(isBusy());
    return base()->DR;
  }

  static inline bool isBusy() { return base()->SR; }

private:
  static inline auto base() { return (SVV_ADI_SPI_REGS *)PSPI; }  
  
};
