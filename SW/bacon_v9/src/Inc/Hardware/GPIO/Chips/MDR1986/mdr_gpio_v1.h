#pragma once

//==============================================================================
// MDR1986  GPIO Port
//==============================================================================

namespace MDR1896_GPIO
{

using namespace MDR1986_MODE;

template<uint32_t GpioID, uint32_t PinsMask, typename TPM = MDR1986_MODE::NotDefined>
class TMDR1986_GPIO : public GpioTemplate<GpioID, 32, PinsMask, TPM>
{
  static_assert(PinsMask<=makeMask(32), "MAX port width is 32 bits");
  static constexpr auto base() { return (PortControl*)GpioID; }
  using tGpioTemplate = GpioTemplate<GpioID, 32, PinsMask, TPM>;
  
public:
  using tgpio = TMDR1986_GPIO<GpioID, PinsMask, TPM>;

  constexpr TMDR1986_GPIO() = default;

  template<uint32_t NewPinsMask, uint32_t NewGpioID=GpioID, typename NewTPM = TPM>
  static constexpr auto clone() { return std::type_identity<TMDR1986_GPIO<NewGpioID,NewPinsMask,NewTPM>>{}; }
  
#if (__cplusplus > 201703L)   
  static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<TypeUnBox<clone<PinsMask>()>>();
#else
  static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<type_unbox<decltype(clone<PinsMask>())>>();
#endif
  
  inline TMDR1986_GPIO& operator=(const TMDR1986_GPIO& gpio)
  {
    write(gpio.read());
    return *this;
  }

  inline TMDR1986_GPIO& operator=(uint32_t value)
  {
    write(value);
    return *this;
  }

  inline operator uint32_t() const { return read(); }

  static inline void unlock() { base()->KEY = 0x8555AAA1; }
  
  template<uint32_t PM=PinsMask>
  static inline void write(uint32_t data)
  {    
    if constexpr (PM == 0xFFFF'FFFF)
      base()->RXTX = data;
    else
    {
      base()->SRXTX = data & PM;
      base()->CRXTX = ~data & PM;        
    }
  }

  template<uint32_t PM=PinsMask, typename... ModedPins> // ModedPins надо для Dummy
  static inline uint32_t read(TypeList<ModedPins...> pins = {})
  {   
    if constexpr (PM == 0xFFFF'FFFF)
      return base()->RXTX;
    else
      return base()->RXTX & PM;
  }

  template<uint32_t PM=PinsMask>
  static inline uint32_t readOutput()
  {    
    if constexpr (PM == 0xFFFF'FFFF)
      return base()->SRXTX;
    else
      return base()->SRXTX & PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void set()
  {
    base()->SRXTX = PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void clear()
  {    
    if constexpr (PM == 0xFFFF'FFFF)
      base()->RXTX =0;
    else
      base()->CRXTX = PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void toggle()
  {    
    write(~base()->SRXTX);
  }

  template<uint32_t PM=PinsMask>
  static inline void lock()
  {
    base()->KEY = 0;
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

  enum class GpioRegs { OE = 1, Analog = 2, PWR0 = 4, PWR1 = 8, PULLUP = 16, PULLDOWN = 32, PD = 64, 
                        FUNC0 = 128, FUNC1 = 256, FUNC2 = 512, FUNC3 = 1024, State = 2048 };
  
  template<uint32_t mask, uint32_t value>
  static inline void WriteSCReg32(volatile uint32_t *reg)
  {
    if constexpr ((mask==0) || ((value & mask)==0)) return;
    *reg = value & mask;
  }

  struct ModeValues
  {
    GpioRegs UsedRegs     = 0;
    uint32_t OE           = 0;
    uint32_t OEMask       = 0;
    uint32_t Analog       = 0;
    uint32_t PWR0         = 0;
    uint32_t PWR0Mask     = 0;
    uint32_t PWR1         = 0;
    uint32_t PWR1Mask     = 0;
    uint32_t PULLUP       = 0;
    uint32_t PULLDOWN     = 0;
    uint32_t PD           = 0;
    uint32_t PDMask       = 0;
    uint32_t FUNC0        = 0;
    uint32_t FUNC0Mask    = 0;
    uint32_t FUNC1        = 0;
    uint32_t FUNC1Mask    = 0;
    uint32_t FUNC2        = 0;
    uint32_t FUNC2Mask    = 0;
    uint32_t FUNC3        = 0;
    uint32_t FUNC3Mask    = 0;
    uint32_t State        = 0;
    uint32_t StateMask    = 0;
    consteval ModeValues operator|(const ModeValues& rv)
    {      
      return { UsedRegs | rv.UsedRegs, 
               OE | rv.OE, OEMask | rv.OEMask,
               Analog | rv.Analog, 
               PWR0 | rv.PWR0, PWR0Mask | rv.PWR0Mask,
               PWR1 | rv.PWR1, PWR1Mask | rv.PWR1Mask,
               PULLUP | rv.PULLUP,
               PULLDOWN | rv.PULLDOWN,
               PD | rv.PD, PDMask | rv.PDMask, 
               FUNC0 | rv.FUNC0, FUNC0Mask | rv.FUNC0Mask, 
               FUNC1 | rv.FUNC1, FUNC1Mask | rv.FUNC1Mask,
               FUNC2 | rv.FUNC2, FUNC2Mask | rv.FUNC2Mask, 
               FUNC3 | rv.FUNC3, FUNC3Mask | rv.FUNC3Mask,
               State | rv.State, StateMask | rv.StateMask }; 
    }
  };                 
                    
public:  
  template<bool UsePwrCfg=false, typename... Ps>
  static inline void pins_mode(TypeList<Ps...> pins)
  {
    static_assert(std::popcount(PinsMask) == sizeof...(Ps), "PinList and Port size is not Equal!");
    
    if constexpr (UsePwrCfg) unlock();
    
    static constexpr auto mode_values = getModeValues(pins);
    
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::OE))
    {
      WriteSCReg32<mode_values.OEMask, mode_values.OE>(&base()->SOE);
      if constexpr (!UsePwrCfg)
        WriteSCReg32<mode_values.OEMask, ~mode_values.OE>(&base()->COE);
    }
        
    WriteSCReg32<PinsMask, mode_values.Analog>(&base()->SANALOG);
    if constexpr (!UsePwrCfg)
      WriteSCReg32<PinsMask, ~mode_values.Analog>(&base()->CANALOG);
    
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::PD))
    {
      WriteSCReg32<mode_values.PDMask, mode_values.PD>(&base()->SPD);
      if constexpr (!UsePwrCfg)
        WriteSCReg32<mode_values.PDMask, ~mode_values.PD>(&base()->CPD);       
    }
    
    WriteSCReg32<PinsMask, mode_values.PULLUP>(&base()->SPULLUP);
    if constexpr (!UsePwrCfg)
      WriteSCReg32<PinsMask, ~mode_values.PULLUP>(&base()->CPULLUP);

    WriteSCReg32<PinsMask, mode_values.PULLDOWN>(&base()->SPULLDOWN);
    if constexpr (!UsePwrCfg)
      WriteSCReg32<PinsMask, ~mode_values.PULLDOWN>(&base()->CPULLDOWN);
    
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::PWR0))
    {
      WriteSCReg32<mode_values.PWR0Mask, mode_values.PWR0>(&base()->SPWR[0]);
      if constexpr (!UsePwrCfg)
        WriteSCReg32<mode_values.PWR0Mask, ~mode_values.PWR0>(&base()->CPWR[0]);       
    }
        
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::PWR1))
    {    
      WriteSCReg32<mode_values.PWR1Mask, mode_values.PWR1>(&base()->SPWR[1]);
      if constexpr (!UsePwrCfg)
        WriteSCReg32<mode_values.PWR1Mask, ~mode_values.PWR1>(&base()->CPWR[1]);        
    }
        
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::FUNC0))
    {    
      WriteSCReg32<mode_values.FUNC0Mask, mode_values.FUNC0>(&base()->SFUNC[0]);
      if constexpr (!UsePwrCfg)
        WriteSCReg32<mode_values.FUNC0Mask, ~mode_values.FUNC0>(&base()->CFUNC[0]);        
    }
    
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::FUNC1))
    {    
      WriteSCReg32<mode_values.FUNC1Mask, mode_values.FUNC1>(&base()->SFUNC[1]);
      if constexpr (!UsePwrCfg)
        WriteSCReg32<mode_values.FUNC1Mask, ~mode_values.FUNC1>(&base()->CFUNC[1]);        
    }
    
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::FUNC2))
    {    
      WriteSCReg32<mode_values.FUNC2Mask, mode_values.FUNC2>(&base()->SFUNC[2]);
      if constexpr (!UsePwrCfg)
        WriteSCReg32<mode_values.FUNC2Mask, ~mode_values.FUNC2>(&base()->CFUNC[2]);        
    }
    
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::FUNC3))
    {    
      WriteSCReg32<mode_values.FUNC3Mask, mode_values.FUNC3>(&base()->SFUNC[3]);
      if constexpr (!UsePwrCfg)
        WriteSCReg32<mode_values.FUNC3Mask, ~mode_values.FUNC3>(&base()->CFUNC[3]);        
    }
    
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::State))
    {    
      WriteSCReg32<mode_values.StateMask, mode_values.State>(&base()->SRXTX);
      if constexpr (!UsePwrCfg)
        WriteSCReg32<mode_values.StateMask, ~mode_values.State>(&base()->CRXTX);        
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
    constexpr uint32_t pin_num = ModedPin::PinNum();
    constexpr uint32_t pin_mask = ModedPin::PinMask();
    constexpr auto mode = ModedPin::tpm::pin_mode;
    static_assert(mode!=MDR1986_PinMode::NotDefined,"Pin mode undefined");    
    constexpr uint32_t af = ModedPin::tpm::af;    
    static_assert( (isAfMode<mode>() && (af<16)) || !isAfMode<mode>(), "Alternate Function is not defined");
    
    constexpr auto UsedRegs = ( !isAfMode<mode>() ? GpioRegs::OE : GpioRegs(0) )                                  |
                              ( !isInputMode<mode>() ? GpioRegs::PD : GpioRegs(0) )                               |
                              ( (!isInputMode<mode>() && (pin_mask&0x0000FFFF)) ? GpioRegs::PWR0 : GpioRegs(0) )  |
                              ( (!isInputMode<mode>() && (pin_mask&0xFFFF0000)) ? GpioRegs::PWR1 : GpioRegs(0) )  |                                
                              ( isOutputStateDefined<mode,af>() ? GpioRegs::State : GpioRegs(0) )                 |
                              ( (!isAnalog<mode>() && (pin_mask&0x000000FF)) ? GpioRegs::FUNC0 : GpioRegs(0) )     |
                              ( (!isAnalog<mode>() && (pin_mask&0x0000FF00)) ? GpioRegs::FUNC1 : GpioRegs(0) )     |
                              ( (!isAnalog<mode>() && (pin_mask&0x00FF0000)) ? GpioRegs::FUNC2 : GpioRegs(0) )     |
                              ( (!isAnalog<mode>() && (pin_mask&0xFF000000)) ? GpioRegs::FUNC3 : GpioRegs(0) );

    constexpr uint32_t OE           = isInputMode<mode>() ? 0 : pin_mask;
    constexpr uint32_t OEMask       = isAfMode<mode>() ? 0 : pin_mask;
    constexpr uint32_t Analog       = isAnalog<mode>() ? 0: pin_mask;
    constexpr uint32_t PWR0         = (pin_mask&0x0000FFFF) ? ((uint32_t(mode) & 3) << pin_num * 2) : 0;
    constexpr uint32_t PWR0Mask     = ((pin_mask&0x0000FFFF) && !isInputMode<mode>()) ? (3 << pin_num * 2) : 0;
    constexpr uint32_t PWR1         = (pin_mask&0xFFFF0000) ? ((uint32_t(mode) & 3) << (pin_num-16) * 2) : 0;
    constexpr uint32_t PWR1Mask     = ((pin_mask&0xFFFF0000) && !isInputMode<mode>()) ? (3 << (pin_num-16) * 2) : 0;
    constexpr uint32_t PULLUP       = isPullUp<mode>() ? pin_mask : 0;
    constexpr uint32_t PULLDOWN     = isPullDown<mode>() ? pin_mask : 0;;    
    constexpr uint32_t PD           = isOpenDrain<mode>() ? pin_mask : 0;
    constexpr uint32_t PDMask       = !isInputMode<mode>() ? pin_mask : 0;  
    
    constexpr uint32_t FUNC0Mask    = (!isAnalog<mode>() && (pin_mask&0x000000FF)) ? 0xFU << pin_num * 4 : 0;        
    constexpr uint32_t FUNC0        = isAfMode<mode>() ? (((pin_mask&0x000000FF)? (af&0xF) << pin_num * 4 : 0) & FUNC0Mask) : 0;
    
    constexpr uint32_t FUNC1Mask    = (!isAnalog<mode>() && (pin_mask&0x0000FF00)) ? 0xFU << (pin_num-8) * 4 : 0;    
    constexpr uint32_t FUNC1        = isAfMode<mode>() ? (((pin_mask&0x0000FF00)? (af&0xF) << (pin_num-8) * 4 : 0) & FUNC1Mask) : 0;
    constexpr uint32_t FUNC2Mask    = (!isAnalog<mode>() && (pin_mask&0x00FF0000)) ? 0xFU << (pin_num-16) * 4 : 0;    
    constexpr uint32_t FUNC2        = isAfMode<mode>() ? (((pin_mask&0x00FF0000)? (af&0xF) << (pin_num-16) * 4 : 0) & FUNC2Mask) : 0;
    constexpr uint32_t FUNC3Mask    = (!isAnalog<mode>() && (pin_mask&0xFF000000)) ? 0xFU << (pin_num-24) * 4 : 0;    
    constexpr uint32_t FUNC3        = isAfMode<mode>() ? (((pin_mask&0xFF000000)? (af&0xF) << (pin_num-24) * 4 : 0) & FUNC3Mask) : 0;   
    constexpr uint32_t State        = isOutputStateDefined<mode,af>() ? af*pin_mask : 0;
    constexpr uint32_t StateMask    = isOutputStateDefined<mode,af>() ? pin_mask : 0;
    
    return { UsedRegs, OE, OEMask, Analog, PWR0, PWR0Mask, PWR1, PWR1Mask, PULLUP, PULLDOWN,
             PD, PDMask, FUNC0, FUNC0Mask, FUNC1, FUNC1Mask, FUNC2, FUNC2Mask, FUNC3, FUNC3Mask, State, StateMask };
  }

};

#define MAKE_PORT(PORT_NAME) \
template<uint32_t PinsMask = 0xFFFFFFFF, typename TPM = MDR1986_MODE::NotDefined>\
using Gpio##PORT_NAME = TMDR1986_GPIO<PORT##PORT_NAME##_BASE, PinsMask, TPM>;\
  
#ifdef PORTA
  MAKE_PORT(A)
  MAKE_32PINS(GpioA,A)
#endif
#ifdef PORTB
  MAKE_PORT(B)
  MAKE_32PINS(GpioB,B)
#endif
#ifdef PORTC
  MAKE_PORT(C)
  MAKE_32PINS(GpioC,C)
#endif
#ifdef PORTD
  MAKE_PORT(D)
  MAKE_32PINS(GpioD,D)
#endif
#ifdef PORTE
  MAKE_PORT(E)
  MAKE_32PINS(GpioE,E)
#endif
#ifdef PORTF
  MAKE_PORT(F)
  MAKE_32PINS(GpioF,F)
#endif

}  //namespace MDR1896_GPIO
