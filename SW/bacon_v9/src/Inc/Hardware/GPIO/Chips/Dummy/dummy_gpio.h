#pragma once

namespace DUMMY_GPIO
{
  
// -------------- Dummy pin Modes --------------------------------------------
enum class DUMMY_PinMode : uint32_t
{
  NotDefined = 0x12345678,
  DummyLow = 0,
  DummyHigh = 1
};

template<DUMMY_PinMode Mode>
class TDummyPinMode : PinModeBase
{
public:
  static constexpr auto pin_mode = Mode;
  constexpr inline TDummyPinMode() = default;
private:
};

namespace DUMMY_MODE
{
using DummyNotDefined = TDummyPinMode<DUMMY_PinMode ::NotDefined>;
using DummyLow = TDummyPinMode<DUMMY_PinMode::DummyLow>;
using DummyHigh = TDummyPinMode<DUMMY_PinMode::DummyHigh>;
}

//==============================================================================
// DUMMY GPIO Port
//==============================================================================

template<uint32_t GpioID, uint32_t PinsMask, typename TPM = DUMMY_MODE::DummyNotDefined>
class TDUMMY_GPIO : public GpioTemplate<GpioID, 32, PinsMask, TPM>
{
  static_assert(PinsMask<= (uint32_t)makeMask(32), "MAX Dummy port width is 32 bits");
  using tGpioTemplate = GpioTemplate<GpioID, 32, PinsMask, TPM>;
public:
  using tgpio = TDUMMY_GPIO<GpioID, PinsMask, TPM>;

  constexpr TDUMMY_GPIO() = default;

  template<uint32_t NewPinsMask, uint32_t NewGpioID=GpioID, typename NewTPM = TPM>
  static constexpr auto clone() { return TypeBox<TDUMMY_GPIO<NewGpioID, NewPinsMask, NewTPM>>{}; }
  
  template<uint32_t PM=PinsMask>
  static inline void write(uint32_t data) {}

  template<uint32_t PM=PinsMask, typename TPinMode = TPM>
  static inline uint32_t read()
  {
#if (__cplusplus > 201703L) 
    static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<TypeUnBox<clone<PM>()>>();
#else
    static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<type_unbox<decltype(clone<PinsMask>())>>();    
#endif
    static constexpr auto modes_ = TypeList<>::generate<std::popcount(PM), TPinMode>();
    return getValueFromModes(tGpioTemplate::MakeModedPins(pins_, modes_));
  }

  template<uint32_t PM=PinsMask, typename... ModedPins>
  static constexpr uint32_t read(TypeList<ModedPins...> moded_pins)
  {
    return getValueFromModes(moded_pins) & PM;
  }

  template<uint32_t PM=PinsMask>
  static inline uint32_t readOutput() { return read(); }

  template<uint32_t PM=PinsMask>
  static inline void set() {}

  template<uint32_t PM=PinsMask>
  static inline void clear() {}

  template<uint32_t PM=PinsMask>
  static inline void toggle() {}

  template<uint32_t PM=PinsMask>
  static inline void lock() {}

  // Same mode for all pins set mode
  static inline void mode() {}

  // Same mode for all pins set mode
  template<typename T>
  static inline void mode() {}

  template<typename... ML>
  static inline void modes()
  {
    static_assert(std::popcount(PinsMask) == PinModeList<ML...>::Size(), "ModeList and Port size is not Equal!");    
  }
  
/*  
  template<bool UsePwrCfg=false, typename... Modes>
    static inline void modes(TypeList<Modes...> modes)
  {
    static_assert(std::popcount(PinsMask) == PinModeList<Modes...>::Size, "ModeList and Port size is not Equal!");
  }
*/  
  
  //Для задания режимов из PinList
  template<bool UsePwrCfg=false, typename... Ps>
  static inline void pins_mode(TypeList<Ps...> modedpins)
  {
    static_assert(std::popcount(PinsMask) == sizeof...(Ps), "PinList and Port size is not Equal!");    
  }
  
  inline TDUMMY_GPIO& operator=(const TDUMMY_GPIO& gpio) { return *this; }

  inline TDUMMY_GPIO& operator=(uint32_t value) { return *this; }

  inline operator uint32_t() const { return read(); }

private:  
  template<typename... ModedPins>
  static inline constexpr uint32_t getValueFromModes(TypeList<ModedPins...> moded_pins)
  {
    return ( getValueFromMode<ModedPins>() | ...);
  }
  
  template<typename ModedPin>
  static inline constexpr uint32_t getValueFromMode()
  {
    constexpr auto mode = ModedPin::tpm::pin_mode;
    static_assert(mode != DUMMY_PinMode::NotDefined, "Dummy state undefined");
    return (mode==DUMMY_PinMode::DummyHigh)? ModedPin::PinMask() : 0;    
  }
  
};

#define MAKE_DUMMY_PORT(PORT_NAME,PORT_ID) \
template<uint32_t PinsMask = 0xFFFF'FFFF, typename TPM = DUMMY_MODE::DummyNotDefined>\
using Dummy##PORT_NAME = TDUMMY_GPIO<PORT_ID,PinsMask, TPM>\
    
MAKE_DUMMY_PORT(A,1001);
MAKE_32PINS(DummyA, DA);
  
}  //namespace DUMMY_GPIO
