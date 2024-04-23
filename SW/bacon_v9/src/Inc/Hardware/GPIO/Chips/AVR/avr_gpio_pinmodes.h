#pragma once

//==============================================================================
// Microblaze SVV_GPIO pin Modes
//==============================================================================

enum class AVR_GPIO_PinMode : uint8_t
{
  NotDefined   = 0x12,  
  Input        = 1,     // Input mode  
  Input_PullUp = 3,     // Input Pull Up mode  
  Output       = 0      // Output mode
};

template<AVR_GPIO_PinMode  Mode, uint8_t ST = 2>
struct TPinMode : PinModeBase
{
  static constexpr auto pin_mode = Mode;  
  static constexpr uint8_t state = ST;
};

namespace AVR_GPIO_MODE
{
using NotDefined = TPinMode<AVR_GPIO_PinMode::NotDefined>;

// Input modes
using Input = TPinMode<AVR_GPIO_PinMode::Input>;
using Input_PullUp = TPinMode<AVR_GPIO_PinMode::Input_PullUp>;

// Output mode
template<uint8_t ST=2>
using Output = TPinMode<AVR_GPIO_PinMode::Output,ST>;

template<AVR_GPIO_PinMode Mode>
static constexpr bool isInputMode()
{
  return (uint8_t(Mode)&1) ? true : false;
}

template<AVR_GPIO_PinMode Mode, uint8_t ST>
static constexpr bool isOutputStateDefined()
{
  return isInputMode<Mode>() ? false : ((ST<2) ? true : false);
}

} //AVR_GPIO_MODE

