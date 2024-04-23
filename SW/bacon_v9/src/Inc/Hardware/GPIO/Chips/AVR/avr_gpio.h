#pragma once

//==============================================================================
// AVR GPIO Port
//==============================================================================

struct AVR_GPIO_TypeDef
{
  volatile uint8_t PIN;
  volatile uint8_t DDR;
  volatile uint8_t PORT;
};

namespace AVR_GPIO
{

using namespace AVR_GPIO_MODE;

template<uint8_t GpioID, uint8_t PinsMask, typename TPM = AVR_GPIO_MODE::NotDefined, bool PinToggle = false>
class TAVR_GPIO : public GpioTemplate<GpioID, 8, PinsMask, TPM>
{
  static_assert(PinsMask <= makeMask(8), "MAX port width is 8 bits");
  static constexpr auto base() { return (AVR_GPIO_TypeDef*)GpioID; }
  using tGpioTemplate = GpioTemplate<GpioID, 8, PinsMask, TPM>;
  
public:
  using tgpio = TAVR_GPIO<GpioID, PinsMask, TPM>;

  constexpr TAVR_GPIO() = default;

  template<uint8_t NewPinsMask, uint8_t NewGpioID=GpioID, typename NewTPM = TPM, bool NewPinToggle = PinToggle>
  static constexpr auto clone() { return std::type_identity<TAVR_GPIO<NewGpioID,NewPinsMask,NewTPM,NewPinToggle>>{}; }
  
#if (__cplusplus > 201703L) 
  static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<TypeUnBox<clone<PinsMask>()>>();
#else
  static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<type_unbox<decltype(clone<PinsMask>())>>();
#endif

  inline TAVR_GPIO& operator=(const TAVR_GPIO& gpio)
  {
    write(gpio.read());
    return *this;
  }

  inline TAVR_GPIO& operator=(uint32_t value)
  {
    write(value);
    return *this;
  }

  inline operator uint8_t() const { return read(); }

  template<uint8_t PM = PinsMask>
  static inline void lock() { }

  template<uint8_t PM = PinsMask>
  static inline void unlock() {}
  
  template<uint8_t PM=PinsMask>
  static inline void write(uint8_t data)
  {    
    if constexpr(PM==0) return;
	else if constexpr (PM == 0xFF)
	  base()->PORT = data;
	else if constexpr (std::popcount(PM)==1)
	{
	  if(data) set<PM>(); else clear<PM>();	  
	}
	else
      base()->PORT = (base()->PORT & ~PM ) | (data & PM);
  }

  template<uint8_t PM=PinsMask, typename... ModedPins> // ModedPins надо для Dummy
  static inline uint8_t read(TypeList<ModedPins...> pins = {})
  {
    if constexpr (PM == 0xFF)
      return base()->PIN;
    else
	  return base()->PIN & PM;
  }

  template<uint8_t PM=PinsMask>
  static inline uint8_t readOutput()
  {
    if constexpr (PM == 0xFF)
      return base()->PORT;
    else
	  return base()->PORT & PM;      
  }

  template<uint8_t PM=PinsMask>
  static inline void set()
  {
    if constexpr (PM == 0xFF)
      base()->PORT = 0xFF;
    else if constexpr (std::popcount(PM) == 1)
      base()->PORT |= PM;
    else
      base()->PORT = (base()->PORT & ~PM) | PM;
  }

  template<uint8_t PM=PinsMask>
  static inline void clear()
  {    
    if constexpr (PM == 0xFF)
      base()->PORT = 0;
    else
      base()->PORT &= ~PM;
  }

  template<uint8_t PM=PinsMask>
  static inline void toggle()
  {    
    if constexpr (PinToggle)
    {
      if constexpr (std::popcount(PM) == 1)
        base()->PIN |= PM;
      else
        base()->PIN = PM;
    }
    else
    {
      if constexpr (std::popcount(PM) == 1)
      {
        if (base()->PORT & PM) clear(); else set();
      }
      else
        base()->PORT ^= PM;
    }
  }

  // Same mode for all pins set
  static inline void mode()
  {
    pins_mode<false>(pins_);
  }

#if (__cplusplus > 201703L)
  // Same mode for all pins set
  template<IsPinMode T>
  static inline void mode()
  {
    static constexpr auto modes = TypeList<>::generate<std::popcount(PinsMask),T>();
    pins_mode<false>(tGpioTemplate::MakeModedPins(pins_,modes));
  }
   
  template<IsPinModeListElement... ML>
  static inline void modes()
  {
    using PML = PinModeList<ML...>;
    static_assert(std::popcount(PinsMask) == PML::Size(), "ModeList and Port size is not Equal!");
    pins_mode<false>(tGpioTemplate::MakeModedPins(pins_, PML::GetModes()));
  }
                  
#else                  
  // Same mode for all pins set
  template<typename T>
  static inline void mode()
  {
    static_assert(IsPinMode<T>(), "T is not PinMode");
    static constexpr auto modes = TypeList<>::generate<std::popcount(PinsMask),T>();
    pins_mode<false>(tGpioTemplate::MakeModedPins(pins_,modes));
  }

  template<typename... ML>
  static inline void modes()
  {
    using PML = PinModeList<ML...>;
    static_assert(std::popcount(PinsMask) == PML::Size(), "ModeList and Port size is not Equal!");
    pins_mode<false>(tGpioTemplate::MakeModedPins(pins_, PML::GetModes()));
  }  
#endif
                  
private:
  enum class GpioRegs : uint8_t { DDR = 1, PORT = 2, State = 3 };

  template<uint8_t mask>
  static inline void WriteReg8(pVU8 reg, uint8_t value)
  {
    if constexpr (mask == 0) return;
    if constexpr (mask == 0xFF)
        *reg = value;
    else
      *reg = (*reg & ~mask) | value;
  }

  struct ModeValues
  {
    GpioRegs UsedRegs     = 0;
    uint8_t DDR           = 0;
    uint8_t DDRMask       = 0;
    uint8_t PORT          = 0;
    uint8_t PORTMask      = 0;	
    consteval ModeValues operator|(const ModeValues& rv)
    {      
        return { UsedRegs | rv.UsedRegs,
                 uint8_t(DDR | rv.DDR), uint8_t(DDRMask | rv.DDRMask),
                 uint8_t(PORT | rv.PORT), uint8_t(PORTMask | rv.PORTMask) };
    }
  };                 
                    
public:
  template<bool UsePwrCfg=false, typename... Ps>
  static inline void pins_mode(TypeList<Ps...> pins)
  {
    static_assert(std::popcount(PinsMask) == sizeof...(Ps), "PinList and Port size is not Equal!");

    static constexpr auto mode_values = getModeValues(pins);

    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::DDR))
    {
      if constexpr (UsePwrCfg)
      {                  
        if constexpr (mode_values.DDR)
        {          
          if constexpr ((mode_values.DDRMask==0xFF) && (std::popcount(mode_values.DDR)>1))
            WriteReg8<mode_values.DDRMask>(& base()->DDR, mode_values.DDR);
          else
            WriteReg8<mode_values.DDR& mode_values.DDRMask>(&base()->DDR, mode_values.DDR);
        }
      }
      else
      {
        WriteReg8<mode_values.DDRMask>(&base()->DDR, mode_values.DDR);
      }
    }
            
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::PORT))
    {   
      if constexpr (UsePwrCfg)
      {
        if constexpr (mode_values.PORT)
        {
          if constexpr ((mode_values.PORTMask == 0xFF) && (std::popcount(mode_values.PORT) > 1))
            WriteReg8<mode_values.PORTMask>(&base()->PORT, mode_values.PORT);
          else
            WriteReg8<mode_values.PORT& mode_values.PORTMask>(&base()->PORT, mode_values.PORT);
        }
      }
      else
      {
        WriteReg8<mode_values.PORTMask>(&base()->PORT, mode_values.PORT);
      }
    }    
  }

private:
  template<typename... ModedPins>
  static constexpr ModeValues getModeValues(TypeList<ModedPins...> moded_pins)
  {
    return ( getModeValues<ModedPins>() | ...);
  }
  
  template<typename ModedPin>
  static constexpr ModeValues getModeValues()
  {
    constexpr uint8_t pin_num = ModedPin::PinNum();
    constexpr uint8_t pin_mask = ModedPin::PinMask();
    constexpr auto mode = ModedPin::tpm::pin_mode;
    static_assert(mode!=AVR_GPIO_PinMode::NotDefined,"Pin mode undefined");    
    constexpr uint8_t st = ModedPin::tpm::state;
    
    constexpr auto UsedRegs = GpioRegs::DDR |
                              ((isOutputStateDefined<mode, st>() || isInputMode<mode>()) ? GpioRegs::PORT : GpioRegs(0));

    constexpr uint8_t DDR         = isInputMode<mode>() ? 0 : pin_mask;
    constexpr uint8_t DDRMask     = 1 << pin_num;
    constexpr uint8_t PORT        = (isOutputStateDefined<mode,st>() ? st*pin_mask : 0) | ((mode == AVR_GPIO_PinMode::Input_PullUp) ? pin_mask:0);
    constexpr uint8_t PORTMask    = hasFlag(UsedRegs, GpioRegs::PORT) ? pin_mask : 0;
    
    return { UsedRegs, DDR, DDRMask, PORT, PORTMask };
  }

};

#define MAKE_PORT(PORT_NAME) \
template<uint8_t PinsMask = 0xFF, typename TPM = AVR_GPIO_MODE::NotDefined>\
using Gpio##PORT_NAME = TAVR_GPIO<GPIO##PORT_NAME##_BASE, PinsMask, TPM, PIN_TOGGLE>;\
  
#ifdef GPIOA_BASE
  MAKE_PORT(A)
  MAKE_8PINS(GpioA,a)
#endif
#ifdef GPIOB_BASE
  MAKE_PORT(B)
  MAKE_8PINS(GpioB,b)
#endif
#ifdef GPIOC_BASE
  MAKE_PORT(C)
  MAKE_8PINS(GpioC,c)
#endif
#ifdef GPIOD_BASE
  MAKE_PORT(D)
  MAKE_8PINS(GpioD,d)
#endif
#ifdef GPIOE_BASE
  MAKE_PORT(E)
  MAKE_8PINS(GpioE,e)
#endif
#ifdef GPIOF_BASE
  MAKE_PORT(F)
  MAKE_8PINS(GpioF,f)
#endif

}  //namespace AVR_GPIO
