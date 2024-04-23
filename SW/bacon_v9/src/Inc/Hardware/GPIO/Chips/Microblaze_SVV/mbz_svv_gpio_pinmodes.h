#pragma once


struct SVV_GPIO_TypeDef
{
  volatile uint32_t ODR;
  volatile uint32_t BSR;
  volatile uint32_t BCR;
  volatile uint32_t DIR;
  volatile uint32_t IDR;   // Read from port, write -> toggle
  volatile uint32_t BCSRL; // 0xFFFF'0000 clear, 0x0000'FFFF set for LOW16(ODR)
  volatile uint32_t BCSRH; // 0xFFFF'0000 clear, 0x0000'FFFF set for HIGH16(ODR)
};

#define GPIOA               ((SVV_GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((SVV_GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((SVV_GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((SVV_GPIO_TypeDef *) GPIOD_BASE)

//==============================================================================
// Microblaze SVV_GPIO pin Modes
//==============================================================================

enum class MBZ_SVV_GPIO_PinMode : uint32_t
{
  NotDefined = 0x12345678,
  // Input mode
  Input           = 1,
  // Output mode
  Output          = 0
};

template<MBZ_SVV_GPIO_PinMode Mode, uint32_t ST = 2>
struct TPinMode : PinModeBase
{
  static constexpr auto pin_mode = Mode;  
  static constexpr uint32_t state = ST;
private:
};

namespace MBZ_SVV_GPIO_MODE
{
using NotDefined = TPinMode<MBZ_SVV_GPIO_PinMode::NotDefined>;

// Input mode
using Input = TPinMode<MBZ_SVV_GPIO_PinMode::Input>;

// Output mode
template<uint32_t ST=2>
using Output = TPinMode<MBZ_SVV_GPIO_PinMode::Output,ST>;

template<MBZ_SVV_GPIO_PinMode Mode>
static constexpr bool isInputMode()
{
  if constexpr ( Mode == MBZ_SVV_GPIO_PinMode::Input )
    return true;
  else
    return false;
}

template<MBZ_SVV_GPIO_PinMode Mode, uint32_t ST>
static constexpr bool isOutputStateDefined()
{
  return isInputMode<Mode>() ? false : ((ST<2) ? true : false);
}

} //MBZ_SVV_GPIO_MODE

