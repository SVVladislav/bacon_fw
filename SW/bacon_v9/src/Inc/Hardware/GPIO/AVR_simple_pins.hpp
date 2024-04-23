#pragma once

#ifndef ADVANCED_GPIO

namespace SimpleAVRPin
{

  struct GPIO_TypeDef
  {
    volatile uint8_t PIN;
    volatile uint8_t DDR;
    volatile uint8_t PORT;
   };

template<uint8_t pGPIO, uint8_t Pin, bool PinToggle=false>
struct TPin final
{  
  TPin(bool value) { if (value) set(); else clear(); }
  static inline void set() 
  {  
    base()->PORT |= pin_mask;
  }
  
  static inline void clear()
  {
  	base()->PORT &= ~pin_mask;
  }

  static inline void toggle()
  {
    if constexpr (PinToggle) base()->PIN |= pin_mask;
    else
    {
      if (base()->PORT & pin_mask) clear(); else set();
    }
  }
  
  static inline void write(bool data) 
  { 
    if (data) set(); else clear();
  }  

  template<bool data> 
  static inline void write() 
  { 
   if constexpr (data) set(); else clear(); 
  }
  static inline bool read() 
  { 
   return base()->PIN & pin_mask;
  }

  inline TPin& operator=(const TPin& pin)
  {
    write(pin.read());
    return *this;
  }

  inline TPin& operator=(bool value)
  {
    write(value);
    return *this;
  }

  inline operator bool() const { return read(); }
  
private:
  static constexpr uint8_t pin_mask = 1 << Pin;
  static constexpr auto base() { return (GPIO_TypeDef*)pGPIO; }
};


} //namespace SimpleAVRPin

#define MAKE_8PINS(PORT_NAME) \
using P##PORT_NAME##_0  = SimpleAVRPin::TPin<(uint8_t)GPIO##PORT_NAME##_BASE,0>; \
using P##PORT_NAME##_1  = SimpleAVRPin::TPin<(uint8_t)GPIO##PORT_NAME##_BASE,1>; \
using P##PORT_NAME##_2  = SimpleAVRPin::TPin<(uint8_t)GPIO##PORT_NAME##_BASE,2>; \
using P##PORT_NAME##_3  = SimpleAVRPin::TPin<(uint8_t)GPIO##PORT_NAME##_BASE,3>; \
using P##PORT_NAME##_4  = SimpleAVRPin::TPin<(uint8_t)GPIO##PORT_NAME##_BASE,4>; \
using P##PORT_NAME##_5  = SimpleAVRPin::TPin<(uint8_t)GPIO##PORT_NAME##_BASE,5>; \
using P##PORT_NAME##_6  = SimpleAVRPin::TPin<(uint8_t)GPIO##PORT_NAME##_BASE,6>; \
using P##PORT_NAME##_7  = SimpleAVRPin::TPin<(uint8_t)GPIO##PORT_NAME##_BASE,7>; \

namespace GPIO
{

#ifdef PORTB
#define GPIOb_BASE (0x16 + 0x20)  
MAKE_8PINS(b)
#endif

#ifdef PORTC
#define GPIOc_BASE (0x13 + 0x20)
MAKE_8PINS(c)
#endif

#ifdef PORTD
#define GPIOd_BASE (0x10 + 0x20)
MAKE_8PINS(d)
#endif

} //namespace GPIO

#endif // ADVANCED_GPIO