#pragma once

// -------------- STM32F4 pin Modes --------------------------------------------

enum class STM32F4_PinMode : uint32_t
{
  NotDefined = 0x12345678,
  // PUPDR[6:5] : OTYPER[4] : OSPEEDR[3:2] : MODER[1:0]

  // Input modes
  Input_Floating                      = 0b00'0'00'00,  //0x00
  Input_PullUp                        = 0b01'0'00'00,  //0x20
  Input_PullDown                      = 0b10'0'00'00,  //0x40

  // Analog mode
  Analog                              = 0b00'0'00'11,  //0x03

  // General purpose output PushPull modes
  PushPull_LowSpeed                   = 0b00'0'00'01,  //0x01
  PushPull_MediumSpeed                = 0b00'0'01'01,  //0x05
  PushPull_HighSpeed                  = 0b00'0'10'01,  //0x09
  PushPull_VeryHighSpeed              = 0b00'0'11'01,  //0x0D

  // General purpose output Open Drain modes
  OpenDrain_LowSpeed                  = 0b00'1'00'01,  //0x11
  OpenDrain_MediumSpeed               = 0b00'1'01'01,  //0x15
  OpenDrain_HighSpeed                 = 0b00'1'10'01,  //0x19
  OpenDrain_VeryHighSpeed             = 0b00'1'11'01,  //0x1D

  OpenDrain_LowSpeed_PullUp           = 0b01'1'00'01,  //0x31
  OpenDrain_MediumSpeed_PullUp        = 0b01'1'01'01,  //0x35
  OpenDrain_HighSpeed_PullUp          = 0b01'1'10'01,  //0x39
  OpenDrain_VeryHighSpeed_PullUp      = 0b01'1'11'01,  //0x3D

  // Alternate function Open Drain modes
  AF_OpenDrain_LowSpeed               = 0b00'1'00'10,  //0x12
  AF_OpenDrain_MediumSpeed            = 0b00'1'01'10,  //0x16
  AF_OpenDrain_HighSpeed              = 0b00'1'10'10,  //0x1A
  AF_OpenDrain_VeryHighSpeed          = 0b00'1'11'10,  //0x1E

  AF_OpenDrain_LowSpeed_PullUp        = 0b01'1'00'10,  //0x32
  AF_OpenDrain_MediumSpeed_PullUp     = 0b01'1'01'10,  //0x36
  AF_OpenDrain_HighSpeed_PullUp       = 0b01'1'10'10,  //0x3A
  AF_OpenDrain_VeryHighSpeed_PullUp   = 0b01'1'11'10,  //0x3E

  AF_OpenDrain_LowSpeed_PullDown      = 0b10'1'00'10,  //0x52
  AF_OpenDrain_MediumSpeed_PullDown   = 0b10'1'01'10,  //0x56
  AF_OpenDrain_HighSpeed_PullDown     = 0b10'1'10'10,  //0x5A
  AF_OpenDrain_VeryHighSpeed_PullDown = 0b10'1'11'10,  //0x5E

  // Alternate function General Purpose output modes
  AF_PushPull_LowSpeed                = 0b00'0'00'10,  //0x02
  AF_PushPull_MediumSpeed             = 0b00'0'01'10,  //0x06
  AF_PushPull_HighSpeed               = 0b00'0'10'10,  //0x0A
  AF_PushPull_VeryHighSpeed           = 0b00'0'11'10,  //0x0E

  AF_PushPull_LowSpeed_PullUp         = 0b01'0'00'10,  //0x22
  AF_PushPull_MediumSpeed_PullUp      = 0b01'0'01'10,  //0x26
  AF_PushPull_HighSpeed_PullUp        = 0b01'0'10'10,  //0x2A
  AF_PushPull_VeryHighSpeed_PullUp    = 0b01'0'11'10,  //0x2E

  AF_PushPull_LowSpeed_PullDown       = 0b10'0'00'10,  //0x42
  AF_PushPull_MediumSpeed_PullDown    = 0b10'0'01'10,  //0x46
  AF_PushPull_HighSpeed_PullDown      = 0b10'0'10'10,  //0x4A
  AF_PushPull_VeryHighSpeed_PullDown  = 0b10'0'11'10   //0x4E
};

template<STM32F4_PinMode Mode, uint32_t AF = 16>
struct TPinMode : PinModeBase
{
  static constexpr auto pin_mode = Mode;
  static constexpr auto af = AF;
};

namespace STM32F4_MODE
{
using NotDefined = TPinMode<STM32F4_PinMode::NotDefined>;

// Input modes
using Input_Floating = TPinMode<STM32F4_PinMode::Input_Floating>;
using Input_PullUp = TPinMode<STM32F4_PinMode::Input_PullUp>;
using Input_PullDown = TPinMode<STM32F4_PinMode::Input_PullDown>;

// Analog mode
using Analog = TPinMode<STM32F4_PinMode::Analog>;

// General purpose output PushPull modes
template<uint32_t AF=2>
using PushPull_LowSpeed = TPinMode<STM32F4_PinMode::PushPull_LowSpeed,AF>;
template<uint32_t AF=2>
using PushPull_MediumSpeed = TPinMode<STM32F4_PinMode::PushPull_MediumSpeed,AF>;
template<uint32_t AF=2>
using PushPull_HighSpeed = TPinMode<STM32F4_PinMode::PushPull_HighSpeed,AF>;
template<uint32_t AF=2>
using PushPull_VeryHighSpeed = TPinMode<STM32F4_PinMode::PushPull_VeryHighSpeed,AF>;

// General purpose output Open Drain modes
template<uint32_t AF=2>
using OpenDrain_LowSpeed = TPinMode<STM32F4_PinMode::OpenDrain_LowSpeed,AF>;
template<uint32_t AF=2>
using OpenDrain_MediumSpeed = TPinMode<STM32F4_PinMode::OpenDrain_MediumSpeed,AF>;
template<uint32_t AF=2>
using OpenDrain_HighSpeed = TPinMode<STM32F4_PinMode::OpenDrain_HighSpeed,AF>;
template<uint32_t AF=2>
using OpenDrain_VeryHighSpeed = TPinMode<STM32F4_PinMode::OpenDrain_VeryHighSpeed,AF>;

template<uint32_t AF=2>
using OpenDrain_LowSpeed_PullUp = TPinMode<STM32F4_PinMode::OpenDrain_LowSpeed_PullUp,AF>;
template<uint32_t AF=2>
using OpenDrain_MediumSpeed_PullUp = TPinMode<STM32F4_PinMode::OpenDrain_MediumSpeed_PullUp,AF>;
template<uint32_t AF=2>
using OpenDrain_HighSpeed_PullUp = TPinMode<STM32F4_PinMode::OpenDrain_HighSpeed_PullUp,AF>;
template<uint32_t AF=2>
using OpenDrain_VeryHighSpeed_PullUp = TPinMode<STM32F4_PinMode::OpenDrain_VeryHighSpeed_PullUp,AF>;

// Alternate function Open Drain modes
template<uint32_t AF=16>
using AF_OpenDrain_LowSpeed = TPinMode<STM32F4_PinMode::AF_OpenDrain_LowSpeed,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_MediumSpeed = TPinMode<STM32F4_PinMode::AF_OpenDrain_MediumSpeed,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_HighSpeed = TPinMode<STM32F4_PinMode::AF_OpenDrain_HighSpeed,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_VeryHighSpeed = TPinMode<STM32F4_PinMode::AF_OpenDrain_VeryHighSpeed,AF>;

template<uint32_t AF=16>
using AF_OpenDrain_LowSpeed_PullUp = TPinMode<STM32F4_PinMode::AF_OpenDrain_LowSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_MediumSpeed_PullUp = TPinMode<STM32F4_PinMode::AF_OpenDrain_MediumSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_HighSpeed_PullUp = TPinMode<STM32F4_PinMode::AF_OpenDrain_HighSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_VeryHighSpeed_PullUp = TPinMode<STM32F4_PinMode::AF_OpenDrain_VeryHighSpeed_PullUp,AF>;

template<uint32_t AF=16>
using AF_OpenDrain_LowSpeed_PullDown = TPinMode<STM32F4_PinMode::AF_OpenDrain_LowSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_MediumSpeed_PullDown = TPinMode<STM32F4_PinMode::AF_OpenDrain_MediumSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_HighSpeed_PullDown = TPinMode<STM32F4_PinMode::AF_OpenDrain_HighSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_VeryHighSpeed_PullDown = TPinMode<STM32F4_PinMode::AF_OpenDrain_VeryHighSpeed_PullDown,AF>;

// Alternate function General Purpose output modes
template<uint32_t AF=16>
using AF_PushPull_LowSpeed = TPinMode<STM32F4_PinMode::AF_PushPull_LowSpeed,AF>;
template<uint32_t AF=16>
using AF_PushPull_MediumSpeed = TPinMode<STM32F4_PinMode::AF_PushPull_MediumSpeed,AF>;
template<uint32_t AF=16>
using AF_PushPull_HighSpeed = TPinMode<STM32F4_PinMode::AF_PushPull_HighSpeed,AF>;
template<uint32_t AF=16>
using AF_PushPull_VeryHighSpeed = TPinMode<STM32F4_PinMode::AF_PushPull_VeryHighSpeed,AF>;

template<uint32_t AF=16>
using AF_PushPull_LowSpeed_PullUp = TPinMode<STM32F4_PinMode::AF_PushPull_LowSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_PushPull_MediumSpeed_PullUp = TPinMode<STM32F4_PinMode::AF_PushPull_MediumSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_PushPull_HighSpeed_PullUp = TPinMode<STM32F4_PinMode::AF_PushPull_HighSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_PushPull_VeryHighSpeed_PullUp = TPinMode<STM32F4_PinMode::AF_PushPull_VeryHighSpeed_PullUp,AF>;

template<uint32_t AF=16>
using AF_PushPull_LowSpeed_PullDown = TPinMode<STM32F4_PinMode::AF_PushPull_LowSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_PushPull_MediumSpeed_PullDown = TPinMode<STM32F4_PinMode::AF_PushPull_MediumSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_PushPull_HighSpeed_PullDown = TPinMode<STM32F4_PinMode::AF_PushPull_HighSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_PushPull_VeryHighSpeed_PullDown = TPinMode<STM32F4_PinMode::AF_PushPull_VeryHighSpeed_PullDown,AF>;

} // namespace STM32F4_MODE

namespace STM32Fx_GPIO
{
  using STM32Fx_PinMode = STM32F4_PinMode;
  namespace STM32Fx_MODE = STM32F4_MODE;
}
