#pragma once

template <typename TPin, bool invert=false>
struct TLed final
{  
  using tpin = TPin;
  static inline void On()  { invert ? TPin::clear() : TPin::set(); }
  static inline void Off() { invert ? TPin::set() : TPin::clear(); }
  static inline void Toggle() { TPin::toggle(); }
  static inline bool Read() { return invert ? !TPin::read() : TPin::read(); }
  inline TLed& operator=(bool state)
  {
    if(state) On(); else Off();
    return *this;
  }
  inline operator bool() const { return Read(); }
};

template <typename TLed, uint8_t divider>
struct TBlinkingLed final
{
  static_assert(divider > 0);
  void On() { counter = 1; TLed::On(); }
  void Off() { counter = 0; TLed::Off(); }
  void Toggle() { if (counter) Off(); else On(); }

  void tic()
  {
    uint8_t tmp = counter;
    if (tmp)
    {
      if (tmp++ == divider)
      {
        tmp = 1;
        TLed::Toggle();
      }
      counter = tmp;
    }
  }
private:
  volatile uint8_t counter = 0;
};
