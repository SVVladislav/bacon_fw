#pragma once

//==============================================================================
// Microblaze SVV GPIO Port
//==============================================================================

namespace MBZ_SVV_GPIO
{

using namespace MBZ_SVV_GPIO_MODE;

template<uint32_t GpioID, uint32_t PinsMask, typename TPM = MBZ_SVV_GPIO_MODE::NotDefined>
class TMBZ_SVV_GPIO : public GpioTemplate<GpioID, 32, PinsMask, TPM>
{
  static_assert(PinsMask<=makeMask(32), "MAX port width is 32 bits");
  static constexpr auto base() { return (SVV_GPIO_TypeDef*)GpioID; }
  using tGpioTemplate = GpioTemplate<GpioID, 32, PinsMask, TPM>;
  
public:
  using tgpio = TMBZ_SVV_GPIO<GpioID, PinsMask, TPM>;

  constexpr TMBZ_SVV_GPIO() = default;

  template<uint32_t NewPinsMask, uint32_t NewGpioID=GpioID, typename NewTPM = TPM>
  static constexpr auto clone() { return std::type_identity<TMBZ_SVV_GPIO<NewGpioID,NewPinsMask,NewTPM>>{}; }
  
#if (__cplusplus > 201703L) 
  static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<TypeUnBox<clone<PinsMask>()>>();
#else
  static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<type_unbox<decltype(clone<PinsMask>())>>();
#endif

  inline TMBZ_SVV_GPIO& operator=(const TMBZ_SVV_GPIO& gpio)
  {
    write(gpio.read());
    return *this;
  }

  inline TMBZ_SVV_GPIO& operator=(uint32_t value)
  {
    write(value);
    return *this;
  }

  inline operator uint32_t() const { return read(); }

  static inline void unlock() {}
  
  template<uint32_t PM=PinsMask>
  static inline void write(uint32_t data)
  {    
	if constexpr(PM==0) return;
	else if constexpr (PM == 0xFFFF'FFFF)
	  base()->ODR = data;
    else if constexpr ( (PM & 0xFFFF'0000)==0)
    { // ������������ ������ ������� 16 ���
      if constexpr(PM==0x0000'00FF)
        *pVU8(&base()->ODR) = data;
      else if constexpr(PM==0x0000'FF00)
    	*(pVU8(&base()->ODR) + 1) = data >> 8;
      else if constexpr(PM==0x0000'FFFF)
        *pVU16(&base()->ODR) = data;
      else
    	base()->BCSRL = (PM << 16) | (data & PM);
    }
    else if constexpr ((PM & 0x0000'FFFF)==0 )
    { // ������������ ������ ������� 16 ���
      if constexpr(PM==0x00FF'0000)
        *(pVU8(&base()->ODR) + 2) = data >> 16;
      else if constexpr(PM==0xFF00'0000)
        *(pVU8(&base()->ODR) + 3) = data >> 24;
      else if constexpr(PM==0xFFFF'0000)
        *(pVU16(&base()->ODR) + 1) = data >> 16;
      else
      	base()->BCSRH = PM | ((data & PM)>>16);
    }
    else
    {
      base()->BSR = data & PM;
      base()->BCR = ~data & PM;
    }
  }

  template<uint32_t PM=PinsMask, typename... ModedPins> // ModedPins надо для Dummy
  static inline uint32_t read(TypeList<ModedPins...> pins = {})
  {   
    if constexpr (PM == 0xFFFF'FFFF)
      return base()->IDR;
    else
      return base()->IDR & PM;
  }

  template<uint32_t PM=PinsMask>
  static inline uint32_t readOutput()
  {    
    if constexpr (PM == 0xFFFF'FFFF)
      return base()->ODR;
    else
      return base()->ODR & PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void set()
  {
    base()->BSR = PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void clear()
  {    
    if constexpr (PM == 0xFFFF'FFFF)
      base()->ODR = 0;
    else
      base()->BCR = PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void toggle()
  {    
    base()->IDR = PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void lock() { }

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
  enum class GpioRegs { DIR = 1, State = 2 };
  
  template<uint32_t mask, uint32_t value>
  static inline void WriteSCReg32(volatile uint32_t *reg)
  {
    if constexpr ((mask==0) || ((value & mask)==0)) return;
    *reg = value & mask;
  }

  struct ModeValues
  {
    GpioRegs UsedRegs     = 0;
    uint32_t DIR          = 0;
    uint32_t DIRMask      = 0;
    uint32_t State        = 0;
    uint32_t StateMask    = 0;
    consteval ModeValues operator|(const ModeValues& rv)
    {      
      return { UsedRegs | rv.UsedRegs, 
               DIR | rv.DIR, DIRMask | rv.DIRMask,
               State | rv.State, StateMask | rv.StateMask }; 
    }
  };                 
                    
public:
  template<bool UsePwrCfg=false, typename... Ps>
  static inline void pins_mode(TypeList<Ps...> pins)
  {
    static_assert(std::popcount(PinsMask) == sizeof...(Ps), "PinList and Port size is not Equal!");
    
    static constexpr auto mode_values = getModeValues(pins);
    
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::DIR))
    {
      base()->DIR = mode_values.DIR;
    }
            
    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::State))
    {   
      base()->ODR = mode_values.State;      
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
    static_assert(mode!=MBZ_SVV_GPIO_PinMode::NotDefined,"Pin mode undefined");    
    constexpr uint32_t st = ModedPin::tpm::state;
    
    constexpr auto UsedRegs = isInputMode<mode>() ? GpioRegs::DIR : GpioRegs(0);

    constexpr uint32_t DIR          = isInputMode<mode>() ? 1 << pin_num : 0;
    constexpr uint32_t DIRMask      = 1 << pin_num;
    constexpr uint32_t State        = isOutputStateDefined<mode,st>() ? st*pin_mask : 0;
    constexpr uint32_t StateMask    = isOutputStateDefined<mode,st>() ? pin_mask : 0;
    
    return { UsedRegs, DIR, DIRMask, State, StateMask };
  }

};

#define MAKE_PORT(PORT_NAME) \
template<uint32_t PinsMask = 0xFFFFFFFF, typename TPM = MBZ_SVV_GPIO_MODE::NotDefined>\
using Gpio##PORT_NAME = TMBZ_SVV_GPIO<GPIO##PORT_NAME##_BASE, PinsMask, TPM>;\
  
#ifdef GPIOA_BASE
  MAKE_PORT(A)
  MAKE_32PINS(GpioA,A)
#endif
#ifdef GPIOB_BASE
  MAKE_PORT(B)
  MAKE_32PINS(GpioB,B)
#endif
#ifdef GPIOC_BASE
  MAKE_PORT(C)
  MAKE_32PINS(GpioC,C)
#endif
#ifdef GPIOD_BASE
  MAKE_PORT(D)
  MAKE_32PINS(GpioD,D)
#endif
#ifdef GPIOE_BASE
  MAKE_PORT(E)
  MAKE_32PINS(GpioE,E)
#endif
#ifdef GPIOF_BASE
  MAKE_PORT(F)
  MAKE_32PINS(GpioF,F)
#endif

}  //namespace MBZ_SVV_GPIO
