#pragma once

//==============================================================================
// MDR1986 pin Modes
//==============================================================================

enum class MDR1986_PinMode : uint32_t
{
  NotDefined = 0x12345678,  
  
  // OE(Direction)[7] : AF[6] : ANALOG[5] : PULLUP[4] : PULLDOWN[3] : PD(OpenDrain)[2] : PWR(Speed)[1:0]
  
  // Analog mode
  Analog                            = 0b0'0'0'0'0'0'00,  //0x00
  
  // Input modes
  Input_Floating                    = 0b0'0'1'0'0'0'00,  //0x20
  Input_PullUp                      = 0b0'0'1'1'0'0'00,  //0x30
  Input_PullDown                    = 0b0'0'1'0'1'0'00,  //0x28
  
  // General purpose output PushPull modes
  PushPull_Off                      = 0b1'0'1'0'0'0'00,  //0xA0
  PushPull_LowSpeed                 = 0b1'0'1'0'0'0'01,  //0xA1
  PushPull_MediumSpeed              = 0b1'0'1'0'0'0'10,  //0xA2
  PushPull_HighSpeed                = 0b1'0'1'0'0'0'11,  //0xA3

  PushPull_Off_PullUp               = 0b1'0'1'1'0'0'00,  //0xB0
  PushPull_LowSpeed_PullUp          = 0b1'0'1'1'0'0'01,  //0xB1
  PushPull_MediumSpeed_PullUp       = 0b1'0'1'1'0'0'10,  //0xB2
  PushPull_HighSpeed_PullUp         = 0b1'0'1'1'0'0'11,  //0xB3
  
  PushPull_Off_PullDown             = 0b1'0'1'0'1'0'00,  //0xA8
  PushPull_LowSpeed_PullDown        = 0b1'0'1'0'1'0'01,  //0xA9
  PushPull_MediumSpeed_PullDown     = 0b1'0'1'0'1'0'10,  //0xAA
  PushPull_HighSpeed_PullDown       = 0b1'0'1'0'1'0'11,  //0xAB
  
  // General purpose output Open Drain modes
  OpenDrain_Off                     = 0b1'0'1'0'0'1'00,  //0xA4
  OpenDrain_LowSpeed                = 0b1'0'1'0'0'1'01,  //0xA5
  OpenDrain_MediumSpeed             = 0b1'0'1'0'0'1'10,  //0xA6
  OpenDrain_HighSpeed               = 0b1'0'1'0'0'1'11,  //0xA7

  OpenDrain_Off_PullUp              = 0b1'0'1'1'0'1'00,  //0xB4    
  OpenDrain_LowSpeed_PullUp         = 0b1'0'1'1'0'1'01,  //0xB5
  OpenDrain_MediumSpeed_PullUp      = 0b1'0'1'1'0'1'10,  //0xB6
  OpenDrain_HighSpeed_PullUp        = 0b1'0'1'1'0'1'11,  //0xB7

  OpenDrain_Off_PullDown            = 0b1'0'1'0'1'1'00,  //0xAC
  OpenDrain_LowSpeed_PullDown       = 0b1'0'1'0'1'1'01,  //0xAD
  OpenDrain_MediumSpeed_PullDown    = 0b1'0'1'0'1'1'10,  //0xAE
  OpenDrain_HighSpeed_PullDown      = 0b1'0'1'0'1'1'11,  //0xAF

  // Alternate function Open Drain modes
  AF_OpenDrain_Off                  = 0b0'1'1'0'0'1'00,  //0x64
  AF_OpenDrain_LowSpeed             = 0b0'1'1'0'0'1'01,  //0x65
  AF_OpenDrain_MediumSpeed          = 0b0'1'1'0'0'1'10,  //0x66
  AF_OpenDrain_HighSpeed            = 0b0'1'1'0'0'1'11,  //0x67
  
  AF_OpenDrain_Off_PullUp           = 0b0'1'1'1'0'1'00,  //0x74    
  AF_OpenDrain_LowSpeed_PullUp      = 0b0'1'1'1'0'1'01,  //0x75
  AF_OpenDrain_MediumSpeed_PullUp   = 0b0'1'1'1'0'1'10,  //0x76
  AF_OpenDrain_HighSpeed_PullUp     = 0b0'1'1'1'0'1'11,  //0x77
  
  AF_OpenDrain_Off_PullDown         = 0b0'1'1'0'1'1'00,  //0x6C
  AF_OpenDrain_LowSpeed_PullDown    = 0b0'1'1'0'1'1'01,  //0x6D
  AF_OpenDrain_MediumSpeed_PullDown = 0b0'1'1'0'1'1'10,  //0x6E
  AF_OpenDrain_HighSpeed_PullDown   = 0b0'1'1'0'1'1'11,  //0x6F
  
  // Alternate function General Purpose output modes
  AF_PushPull_Off                      = 0b0'1'1'0'0'0'00,  //0x60
  AF_PushPull_LowSpeed                 = 0b0'1'1'0'0'0'01,  //0x61
  AF_PushPull_MediumSpeed              = 0b0'1'1'0'0'0'10,  //0x62
  AF_PushPull_HighSpeed                = 0b0'1'1'0'0'0'11,  //0x63

  AF_PushPull_Off_PullUp               = 0b0'1'1'1'0'0'00,  //0x70
  AF_PushPull_LowSpeed_PullUp          = 0b0'1'1'1'0'0'01,  //0x71
  AF_PushPull_MediumSpeed_PullUp       = 0b0'1'1'1'0'0'10,  //0x72
  AF_PushPull_HighSpeed_PullUp         = 0b0'1'1'1'0'0'11,  //0x73
  
  AF_PushPull_Off_PullDown             = 0b0'1'1'0'1'0'00,  //0x68
  AF_PushPull_LowSpeed_PullDown        = 0b0'1'1'0'1'0'01,  //0x69
  AF_PushPull_MediumSpeed_PullDown     = 0b0'1'1'0'1'0'10,  //0x6A
  AF_PushPull_HighSpeed_PullDown       = 0b0'1'1'0'1'0'11,  //0x6B

};

template<MDR1986_PinMode Mode, uint32_t AF = 16>
struct TPinMode : PinModeBase
{
  static constexpr auto pin_mode = Mode; 
  static constexpr uint32_t af = AF;
};

namespace MDR1986_MODE
{
using NotDefined = TPinMode<MDR1986_PinMode::NotDefined>;

// Analog mode
using Analog = TPinMode<MDR1986_PinMode::Analog>;

// Input modes
using Input_Floating = TPinMode<MDR1986_PinMode::Input_Floating>;
using Input_PullUp = TPinMode<MDR1986_PinMode::Input_PullUp>;
using Input_PullDown = TPinMode<MDR1986_PinMode::Input_PullDown>;

// General purpose output PushPull modes
template<uint32_t AF=2>
using PushPull_Off = TPinMode<MDR1986_PinMode::PushPull_Off,AF>;
template<uint32_t AF=2>
using PushPull_LowSpeed = TPinMode<MDR1986_PinMode::PushPull_LowSpeed,AF>;
template<uint32_t AF=2>
using PushPull_MediumSpeed = TPinMode<MDR1986_PinMode::PushPull_MediumSpeed,AF>;
template<uint32_t AF=2>
using PushPull_HighSpeed = TPinMode<MDR1986_PinMode::PushPull_HighSpeed,AF>;

template<uint32_t AF=2>
using PushPull_Off_PullUp = TPinMode<MDR1986_PinMode::PushPull_Off_PullUp,AF>;
template<uint32_t AF=2>
using PushPull_LowSpeed_PullUp = TPinMode<MDR1986_PinMode::PushPull_LowSpeed_PullUp,AF>;
template<uint32_t AF=2>
using PushPull_MediumSpeed_PullUp = TPinMode<MDR1986_PinMode::PushPull_MediumSpeed_PullUp,AF>;
template<uint32_t AF=2>
using PushPull_HighSpeed_PullUp = TPinMode<MDR1986_PinMode::PushPull_HighSpeed_PullUp,AF>;

template<uint32_t AF=2>
using PushPull_Off_PullDown = TPinMode<MDR1986_PinMode::PushPull_Off_PullDown,AF>;
template<uint32_t AF=2>
using PushPull_LowSpeed_PullDown = TPinMode<MDR1986_PinMode::PushPull_LowSpeed_PullDown,AF>;
template<uint32_t AF=2>
using PushPull_MediumSpeed_PullDown = TPinMode<MDR1986_PinMode::PushPull_MediumSpeed_PullDown,AF>;
template<uint32_t AF=2>
using PushPull_HighSpeed_PullDown = TPinMode<MDR1986_PinMode::PushPull_HighSpeed_PullDown,AF>;

// General purpose output Open Drain modes
template<uint32_t AF=2>
using OpenDrain_Off = TPinMode<MDR1986_PinMode::OpenDrain_Off,AF>;
template<uint32_t AF=2>
using OpenDrain_LowSpeed = TPinMode<MDR1986_PinMode::OpenDrain_LowSpeed,AF>;
template<uint32_t AF=2>
using OpenDrain_MediumSpeed = TPinMode<MDR1986_PinMode::OpenDrain_MediumSpeed,AF>;
template<uint32_t AF=2>
using OpenDrain_HighSpeed = TPinMode<MDR1986_PinMode::OpenDrain_HighSpeed,AF>;

template<uint32_t AF=2>
using OpenDrain_Off_PullUp = TPinMode<MDR1986_PinMode::OpenDrain_Off_PullUp,AF>;
template<uint32_t AF=2>
using OpenDrain_LowSpeed_PullUp = TPinMode<MDR1986_PinMode::OpenDrain_LowSpeed_PullUp,AF>;
template<uint32_t AF=2>
using OpenDrain_MediumSpeed_PullUp = TPinMode<MDR1986_PinMode::OpenDrain_MediumSpeed_PullUp,AF>;
template<uint32_t AF=2>
using OpenDrain_HighSpeed_PullUp = TPinMode<MDR1986_PinMode::OpenDrain_HighSpeed_PullUp,AF>;

template<uint32_t AF=2>
using OpenDrain_Off_PullDown = TPinMode<MDR1986_PinMode::OpenDrain_Off_PullDown,AF>;
template<uint32_t AF=2>
using OpenDrain_LowSpeed_PullDown = TPinMode<MDR1986_PinMode::OpenDrain_LowSpeed_PullDown,AF>;
template<uint32_t AF=2>
using OpenDrain_MediumSpeed_PullDown = TPinMode<MDR1986_PinMode::OpenDrain_MediumSpeed_PullDown,AF>;
template<uint32_t AF=2>
using OpenDrain_HighSpeed_PullDown = TPinMode<MDR1986_PinMode::OpenDrain_HighSpeed_PullDown,AF>;

// Alternate function Open Drain modes
template<uint32_t AF=16>
using AF_OpenDrain_Off = TPinMode<MDR1986_PinMode::AF_OpenDrain_Off,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_LowSpeed = TPinMode<MDR1986_PinMode::AF_OpenDrain_LowSpeed,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_MediumSpeed = TPinMode<MDR1986_PinMode::AF_OpenDrain_MediumSpeed,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_HighSpeed = TPinMode<MDR1986_PinMode::AF_OpenDrain_HighSpeed,AF>;

template<uint32_t AF=16>
using AF_OpenDrain_Off_PullUp = TPinMode<MDR1986_PinMode::AF_OpenDrain_Off_PullUp,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_LowSpeed_PullUp = TPinMode<MDR1986_PinMode::AF_OpenDrain_LowSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_MediumSpeed_PullUp = TPinMode<MDR1986_PinMode::AF_OpenDrain_MediumSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_HighSpeed_PullUp = TPinMode<MDR1986_PinMode::AF_OpenDrain_HighSpeed_PullUp,AF>;

template<uint32_t AF=16>
using AF_OpenDrain_Off_PullDown = TPinMode<MDR1986_PinMode::AF_OpenDrain_Off_PullDown,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_LowSpeed_PullDown = TPinMode<MDR1986_PinMode::AF_OpenDrain_LowSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_MediumSpeed_PullDown = TPinMode<MDR1986_PinMode::AF_OpenDrain_MediumSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_OpenDrain_HighSpeed_PullDown = TPinMode<MDR1986_PinMode::AF_OpenDrain_HighSpeed_PullDown,AF>;

// Alternate function General Purpose output modes
template<uint32_t AF=16>
using AF_PushPull_Off = TPinMode<MDR1986_PinMode::AF_PushPull_Off,AF>;
template<uint32_t AF=16>
using AF_PushPull_LowSpeed = TPinMode<MDR1986_PinMode::AF_PushPull_LowSpeed,AF>;
template<uint32_t AF=16>
using AF_PushPull_MediumSpeed = TPinMode<MDR1986_PinMode::AF_PushPull_MediumSpeed,AF>;
template<uint32_t AF=16>
using AF_PushPull_HighSpeed = TPinMode<MDR1986_PinMode::AF_PushPull_HighSpeed,AF>;

template<uint32_t AF=16>
using AF_PushPull_Off_PullUp = TPinMode<MDR1986_PinMode::AF_PushPull_Off_PullUp,AF>;
template<uint32_t AF=16>
using AF_PushPull_LowSpeed_PullUp = TPinMode<MDR1986_PinMode::AF_PushPull_LowSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_PushPull_MediumSpeed_PullUp = TPinMode<MDR1986_PinMode::AF_PushPull_MediumSpeed_PullUp,AF>;
template<uint32_t AF=16>
using AF_PushPull_HighSpeed_PullUp = TPinMode<MDR1986_PinMode::AF_PushPull_HighSpeed_PullUp,AF>;

template<uint32_t AF=16>
using AF_PushPull_Off_PullDown = TPinMode<MDR1986_PinMode::AF_PushPull_Off_PullDown,AF>;
template<uint32_t AF=16>
using AF_PushPull_LowSpeed_PullDown = TPinMode<MDR1986_PinMode::AF_PushPull_LowSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_PushPull_MediumSpeed_PullDown = TPinMode<MDR1986_PinMode::AF_PushPull_MediumSpeed_PullDown,AF>;
template<uint32_t AF=16>
using AF_PushPull_HighSpeed_PullDown = TPinMode<MDR1986_PinMode::AF_PushPull_HighSpeed_PullDown,AF>;

template<MDR1986_PinMode Mode>
static constexpr bool isAnalog() { return Mode == MDR1986_PinMode::Analog; }

template<MDR1986_PinMode Mode>
static constexpr bool isAfMode() { return uint32_t(Mode) & 0x40; }

template<MDR1986_PinMode Mode>
static constexpr bool isOpenDrain() { return uint32_t(Mode) & 0x04; }

template<MDR1986_PinMode Mode>
static constexpr bool isPullUp() { return uint32_t(Mode) & 0x10; }

template<MDR1986_PinMode Mode>
static constexpr bool isPullDown() { return uint32_t(Mode) & 0x08; }

template<MDR1986_PinMode Mode>
static constexpr bool isInputMode()
{
  if constexpr ( Mode == MDR1986_PinMode::Input_Floating || Mode == MDR1986_PinMode::Input_PullUp ||
                 Mode == MDR1986_PinMode::Input_PullDown || Mode == MDR1986_PinMode::Analog )
    return true;
  else
    return false;
}

template<MDR1986_PinMode Mode, uint32_t AF>
static constexpr bool isOutputStateDefined()
{
  return (isAfMode<Mode>() || isInputMode<Mode>()) ? false : (AF<2) ? true : false;
}

} //MDR1986_MODE

