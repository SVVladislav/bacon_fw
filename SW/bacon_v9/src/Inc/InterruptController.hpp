#pragma once

#include <stdint.h>

struct AXI_INTC_REGS
{
  volatile uint32_t ISR;           // Interrupt Status Register
  volatile uint32_t IPR;           // Interrupt Pending Register
  volatile uint32_t IER;           // Interrupt Enable Register
  volatile uint32_t IAR;           // IAR Interrupt Acknowledge Register
  volatile uint32_t SIE;           // Set Interrupt Enables
  volatile uint32_t CIE;           // Clear Interrupt Enables
  volatile uint32_t IVR;           // Interrupt Vector Register
  volatile uint32_t MER;           // Master Enable Register
  volatile uint32_t IMR;           // Interrupt Mode Register
  volatile uint32_t ILR;           // Interrupt Level Register
  volatile uint32_t reserved1[54];
  volatile uint32_t IVAR[32];      // Interrupt Vector Address Register
  volatile uint32_t reserved2[32];
  volatile uint32_t IVEAR[64];     // Interrupt Vector Extended Address Register
};

#define INTC_MER_HIE (1<<1)
#define INTC_MER_ME  (1<<0)

template <uint32_t pINTC>
class TINTC
{
public:
	constexpr TINTC() = default;

	static inline void Init()
	{
	  base()->IMR = 0xFFFF'FFFF; // Set Fast Interrupt Modes
	  base()->MER = INTC_MER_ME | INTC_MER_HIE;
	  base()->CIE = 0xFFFFFFFF;
	  base()->IAR = 0xFFFFFFFF;
	}

	static inline void EnableInterrupts(uint32_t mask) { base()->SIE = mask; }

	static inline void DisableInterrupts(uint32_t mask) { base()->CIE = mask; }

    static inline void InstallIRQ(uint32_t irq, void F() )
    {
      base()->IVAR[irq] = (uint32_t)F;
      EnableInterrupts(1<<irq);
    }

    static inline auto Regs() { return base(); }

private:
  static inline auto base() { return (AXI_INTC_REGS *) pINTC; }
};

#ifdef AXI_INTC_BASE
  using INTC =  TINTC<AXI_INTC_BASE>;
#else
  #error "AXI_INTC_BASE not defined"
#endif
