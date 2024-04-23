#pragma once

//==============================================================================
// STM32F1, XX32 pin Modes
//==============================================================================

enum class XX32_PinMode : uint32_t
{
  NotDefined = 0x12345678,
  // ODR[4] : CNF[3:2] : MODE[1:0]
  Input                       = 0b0'01'00, // 0x04
  Input_PullUp                = 0b1'10'00, // 0x18
  Input_PullDown              = 0b0'10'00, // 0x08
  // Analog mode
  Analog                      = 0b0'00'00, // 0x00
  // Output Open Drain modes
  OpenDrain_LowSpeed          = 0b0'01'10, // 0x06
  OpenDrain_MediumSpeed       = 0b0'01'01, // 0x05
  OpenDrain_HighSpeed         = 0b0'01'11, // 0x07
  // Output Push-Pull modes
  PushPull_LowSpeed           = 0b0'00'10, // 0x02
  PushPull_MediumSpeed        = 0b0'00'01, // 0x01
  PushPull_HighSpeed          = 0b0'00'11, // 0x03
  // Alternate Function Open Drain modes
  AF_OpenDrain_LowSpeed       = 0b0'11'10, // 0x0E
  AF_OpenDrain_MediumSpeed    = 0b0'11'01, // 0x0D
  AF_OpenDrain_HighSpeed      = 0b0'11'11, // 0x0F
  // Alternate Function Push-Pull modes
  AF_PushPull_LowSpeed        = 0b0'10'10, // 0x0A
  AF_PushPull_MediumSpeed     = 0b0'10'01, // 0x09
  AF_PushPull_HighSpeed       = 0b0'10'11  // 0x0B
};

template<XX32_PinMode  Mode, uint32_t ST=2>
class TPinMode : PinModeBase
{
public:
  static constexpr auto pin_mode = Mode;
  static constexpr uint32_t state = ST;
private:
};

namespace XX32_GPIO
{
namespace XX32_MODE
{
using NotDefined = TPinMode<XX32_PinMode::NotDefined>;
using Analog = TPinMode<XX32_PinMode::Analog>;
using Input = TPinMode<XX32_PinMode::Input>;
using Input_PullUp = TPinMode<XX32_PinMode::Input_PullUp>;
using Input_PullDown = TPinMode<XX32_PinMode::Input_PullDown>;
template<uint32_t ST=2>
using OpenDrain_LowSpeed = TPinMode<XX32_PinMode::OpenDrain_LowSpeed,ST>;
template<uint32_t ST=2>
using OpenDrain_MediumSpeed = TPinMode<XX32_PinMode::OpenDrain_MediumSpeed,ST>;
template<uint32_t ST=2>
using OpenDrain_HighSpeed = TPinMode<XX32_PinMode::OpenDrain_HighSpeed,ST>;
template<uint32_t ST=2>
using PushPull_LowSpeed = TPinMode<XX32_PinMode::PushPull_LowSpeed,ST>;
template<uint32_t ST=2>
using PushPull_MediumSpeed = TPinMode<XX32_PinMode::PushPull_MediumSpeed,ST>;
template<uint32_t ST=2>
using PushPull_HighSpeed = TPinMode<XX32_PinMode::PushPull_HighSpeed,ST>;
using AF_OpenDrain_LowSpeed = TPinMode<XX32_PinMode::AF_OpenDrain_LowSpeed>;
using AF_OpenDrain_MediumSpeed = TPinMode<XX32_PinMode::AF_OpenDrain_MediumSpeed>;
using AF_OpenDrain_HighSpeed = TPinMode<XX32_PinMode::AF_OpenDrain_HighSpeed>;
using AF_PushPull_LowSpeed = TPinMode<XX32_PinMode::AF_PushPull_LowSpeed>;
using AF_PushPull_MediumSpeed = TPinMode<XX32_PinMode::AF_PushPull_MediumSpeed>;
using AF_PushPull_HighSpeed = TPinMode<XX32_PinMode::AF_PushPull_HighSpeed>;
} // namespace XX32_MODE
} // namespace XX32_GPIO