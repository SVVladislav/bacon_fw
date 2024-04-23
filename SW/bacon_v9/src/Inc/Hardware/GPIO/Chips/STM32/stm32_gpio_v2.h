#pragma once

//==============================================================================
// STM32F0, STM32F3, STM32F4, STM32L0  GPIO Port
//==============================================================================

namespace STM32Fx_GPIO
{
  
template<STM32Fx_PinMode Mode>
static constexpr bool isAnalog() { return Mode == STM32Fx_PinMode::Analog; }

template<STM32Fx_PinMode Mode>
static constexpr bool isAfMode() { return (uint32_t(Mode) & 3) == 2; }

template<STM32Fx_PinMode Mode>
static constexpr bool isOpenDrain() { return uint32_t(Mode) & 0x10; }

template<STM32Fx_PinMode Mode>
static constexpr bool isInputMode()
{
  if constexpr ( Mode == STM32Fx_PinMode::Input_Floating || Mode == STM32Fx_PinMode::Input_PullUp ||
                 Mode == STM32Fx_PinMode::Input_PullDown || Mode == STM32Fx_PinMode::Analog )
    return true;
  else
    return false;
}

template<STM32Fx_PinMode Mode, uint32_t AF>
static constexpr bool isOutputStateDefined()
{
  return (isAfMode<Mode>() || isInputMode<Mode>()) ? false : (AF<2) ? true : false;
}

template<uint32_t GpioID, uint32_t PinsMask, typename TPM = STM32Fx_MODE::NotDefined,
         uint32_t pwr_moder=0, uint32_t pwr_ospeedr=0, uint32_t pwr_pupdr=0>
class TSTM32Fx_GPIO : public GpioTemplate<GpioID, 16, PinsMask, TPM>
{  
  static_assert(PinsMask<=makeMask(16), "MAX STM32 port width is 16 bits");
  static constexpr auto base() { return (GPIO_TypeDef*)GpioID; }  
  using tGpioTemplate = GpioTemplate<GpioID, 16, PinsMask, TPM>;
  
public:  
  using tgpio = TSTM32Fx_GPIO<GpioID, PinsMask, TPM>;

  constexpr TSTM32Fx_GPIO() = default;
  static constexpr auto GetPins() { return pins_; }  

  template<uint32_t NewPinsMask, uint32_t NewGpioID=GpioID, typename NewTPM = TPM>
  static constexpr auto clone() { return TypeBox<TSTM32Fx_GPIO<NewGpioID,NewPinsMask,NewTPM,pwr_moder,pwr_ospeedr,pwr_pupdr>>{}; }

  template<uint32_t PM=PinsMask>
  static inline void write(uint16_t data)
  {
    if constexpr (PM==0) return;
    if constexpr (PM == 0xFFFF)
      *pVU16(&base()->ODR) = data;
    else if constexpr (PM == 0x00FF)
      *pVU8(&base()->ODR) = data;
    else if constexpr (PM == 0xFF00)
      *(pVU8(&base()->ODR) + 1) = data >> 8;
    else
    {
      base()->BSRR = (PM << 16) | (data & PM);
    }
  }
  
  template<uint32_t PM=PinsMask, typename... ModedPins> // ModedPins надо для Dummy
  static inline uint16_t read(TypeList<ModedPins...> pins = {})
  {
    return readReg16<PM>(&base()->IDR);
  }

  template<uint32_t PM=PinsMask>
  static inline uint16_t readOutput()
  {
    return readReg16<PM>(&base()->ODR);
  }

  template<uint32_t PM=PinsMask>
  static inline void set()
  {
    if constexpr((PM & 0xFF)==0)
      *(pVU8(&base()->BSRR)+1) = PM>>8;
    else
      base()->BSRR = PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void clear()
  {
    if constexpr (PM == 0xFFFF)
      base()->ODR = 0;
    else if constexpr (PM == 0x00FF)
      *pVU8(&base()->ODR) = 0;
    else if constexpr (PM == 0xFF00)
      *(pVU8(&base()->ODR) + 1) = 0;
    else if constexpr((PM & 0xFF)==0)
      *(pVU8(&base()->BSRR)+3) = PM>>8;
    else
      *(pVU16(&base()->BSRR)+1) = PM;
  }
  
  template<uint32_t PM=PinsMask>
  static inline void toggle()
  {
    if constexpr (PM == 0xFFFF)
      base()->ODR = ~base()->ODR;
    else if constexpr (PM == 0x00FF)
      *pVU8(&base()->ODR) = ~*pVU8(&base()->ODR);
    else if constexpr (PM == 0xFF00)
      *(pVU8(&base()->ODR) + 1) = ~*(pVU8(&base()->ODR) + 1);
    else if constexpr (std::has_single_bit(PM))
    { if (base()->ODR & PM) clear(); else set(); }
    else  
      base()->BSRR = (PM << 16) | (~base()->ODR & PM);
  }

  template<uint32_t PM=PinsMask>
  static inline void lock()
  {
    base()->LCKR = PM | GPIO_LCKR_LCKK;
    base()->LCKR = PM; 
    base()->LCKR = PM | GPIO_LCKR_LCKK;
    base()->LCKR;
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

  inline operator uint32_t() const { return read(); }
  
  inline TSTM32Fx_GPIO& operator=(const TSTM32Fx_GPIO& gpio)
  {
    write(gpio.read());
    return *this;
  }

  inline TSTM32Fx_GPIO& operator=(uint32_t value)
  {
    write(value);
    return *this;
  }
  
private:
#if (__cplusplus > 201703L) 
  static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<TypeUnBox<clone<PinsMask>()>>();
#else
  static constexpr auto pins_ = tGpioTemplate::template AddPinsFromRange<type_unbox<decltype(clone<PinsMask>())>>();
#endif

  enum class GpioRegs { Ospeedr = 1, Otyper = 2, Pupdr = 4, AfrL = 8, AfrH = 16, State = 32 };
    
  template<uint16_t pmask>
  static inline uint16_t readReg16(pVU32 reg)
  {
    if constexpr (pmask == 0xFFFF)
      return *pVU16(reg);
    else if constexpr (pmask == 0x00FF)
      return *pVU8(reg);
    else if constexpr (pmask == 0xFF00)
      return *(pVU8(reg) + 1)<<8;
    else
      return *pVU16(reg) & pmask;
  }
                                                  
  template<uint32_t DMask, uint32_t Regmask>
  static constexpr uint32_t OptimizeDMask()
  {
    if constexpr      ((DMask==0) || (Regmask==0)) return 0;
    else if constexpr (((DMask & 0x0000'00FF)==0x0000'00FF) && !(Regmask & 0xFFFF'FF00)) return 0x0000'00FF;
    else if constexpr (((DMask & 0x0000'FF00)==0x0000'FF00) && !(Regmask & 0xFFFF'00FF)) return 0x0000'FF00;
    else if constexpr (((DMask & 0x00FF'0000)==0x00FF'0000) && !(Regmask & 0xFF00'FFFF)) return 0x00FF'0000;
    else if constexpr (((DMask & 0xFF00'0000)==0xFF00'0000) && !(Regmask & 0x00FF'FFFF)) return 0xFF00'0000;
    else if constexpr (((DMask & 0x0000'FFFF)==0x0000'FFFF) && !(Regmask & 0xFFFF'0000)) return 0x0000'FFFF;
    else if constexpr (((DMask & 0xFFFF'0000)==0xFFFF'0000) && !(Regmask & 0x0000'FFFF)) return 0xFFFF'0000;
    else if constexpr (DMask==0xFFFF'FFFF) return 0xFFFF'FFFF;
    else return DMask & Regmask;
  }
                       
  template<uint32_t Mask, uint32_t Regmask>
  static constexpr uint32_t OptimizeMask()
  {
    if constexpr      ((Mask==0)||(Regmask==0)) return 0;
    else if constexpr (((Mask & 0x00FF)==0x00FF) && !(Regmask & 0xFF00)) return 0x00FF;
    else if constexpr (((Mask & 0xFF00)==0xFF00) && !(Regmask & 0x00FF)) return 0xFF00;
    else if constexpr (Mask==0xFFFF) return 0xFFFF;
    else return Mask & Regmask;
  }
                       
  struct ModeValues
  {
    GpioRegs UsedRegs    = 0;
    uint32_t Moder       = 0;
    uint32_t Ospeedr     = 0;
    uint32_t OspeedrMask = 0;
    uint32_t Pupdr       = 0;
    uint32_t PupdrMask   = 0;
    uint32_t Otyper      = 0;
    uint32_t OtyperMask  = 0;
    uint32_t AfrL        = 0;
    uint32_t AfrLMask    = 0;
    uint32_t AfrH        = 0;
    uint32_t AfrHMask    = 0;
    uint32_t State       = 0;
    uint32_t StateMask   = 0;
    consteval ModeValues operator|(const ModeValues& rv) // constexpr for C++17
    {      
      return { UsedRegs | rv.UsedRegs, Moder | rv.Moder, Ospeedr | rv.Ospeedr, OspeedrMask | rv.OspeedrMask, 
               Pupdr | rv.Pupdr, PupdrMask | rv.PupdrMask, Otyper | rv.Otyper,  OtyperMask | rv.OtyperMask,
               AfrL | rv.AfrL, AfrLMask | rv.AfrLMask, AfrH | rv.AfrH, AfrHMask | rv.AfrHMask, State | rv.State, StateMask | rv.StateMask };
    }
  };
  
public:
  template<bool UsePwrCfg=false, typename... Ps>
  static inline void pins_mode(TypeList<Ps...> pins)
  {
    static_assert(std::popcount(PinsMask) == sizeof...(Ps), "PinList and Port size is not Equal!");
    
    static constexpr auto mode_values = getModeValues(pins);
        
    static constexpr uint32_t dm = dmask(PinsMask);
        
    SVVTL::WriteReg32<UsePwrCfg ? OptimizeDMask<dm,mode_values.Moder^(pwr_moder&dm)>() : dm>(&base()->MODER,mode_values.Moder);

    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::Pupdr))
      SVVTL::WriteReg32<OptimizeDMask<dm, UsePwrCfg ? OptimizeDMask<mode_values.PupdrMask, mode_values.Pupdr^(pwr_pupdr&dm)>() : mode_values.PupdrMask>()>(&base()->PUPDR, mode_values.Pupdr);

    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::Ospeedr))
      SVVTL::WriteReg32<OptimizeDMask<dm, UsePwrCfg ? OptimizeDMask<mode_values.OspeedrMask, mode_values.Ospeedr^(pwr_ospeedr&dm)>() : mode_values.OspeedrMask>()>(&base()->OSPEEDR, mode_values.Ospeedr);

    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::Otyper))
    {
      if constexpr ( !UsePwrCfg || (UsePwrCfg && (mode_values.Otyper!=0)) )
      {
#ifdef STM32L0_ERRATA  //Errata: 2.1.1 Writing in byte mode to the GPIOx_OTYPER register does not work
        if constexpr (PinsMask==0xFFFF)
          base()->OTYPER = mode_values.Otyper;
        else
          base()->OTYPER = (base()->OTYPER & ~PinsMask) | mode_values.Otyper;
#else
        SVVTL::WriteReg32<OptimizeDMask<PinsMask, UsePwrCfg ? OptimizeDMask<mode_values.OtyperMask, mode_values.Otyper>() : mode_values.OtyperMask>()>(&base()->OTYPER, mode_values.Otyper);
#endif
      }
    }

    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::AfrL))
      SVVTL::WriteReg32<OptimizeDMask<qmask(PinsMask), UsePwrCfg ? OptimizeDMask<mode_values.AfrLMask, mode_values.AfrL>() : mode_values.AfrLMask>()>(&base()->AFR[0], mode_values.AfrL);

    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::AfrH))
      SVVTL::WriteReg32<OptimizeDMask<qmask(PinsMask>>8), UsePwrCfg ? OptimizeDMask<mode_values.AfrHMask, mode_values.AfrH>() : mode_values.AfrHMask>()>(&base()->AFR[1], mode_values.AfrH);

    if constexpr (hasFlag(mode_values.UsedRegs, GpioRegs::State))
      write<mode_values.StateMask&(UsePwrCfg?mode_values.State:0xFFFFFFFF)>(mode_values.State);
  }
                                              
private:
  template <typename ModedPin>
  static constexpr ModeValues getModeValues()
  {
    constexpr uint32_t pin_num = ModedPin::PinNum();
    constexpr uint32_t pin_mask = ModedPin::PinMask();
    constexpr auto mode = ModedPin::tpm::pin_mode;
    static_assert(mode != STM32Fx_PinMode::NotDefined, "Pin mode undefined");
    constexpr uint32_t af = ModedPin::tpm::af;
    static_assert((isAfMode<mode>() && (af < 16)) || !isAfMode<mode>(), "Alternate Function is not defined");

    constexpr auto UsedRegs = (!isInputMode<mode>() ? GpioRegs::Otyper | GpioRegs::Ospeedr : GpioRegs(0)) |
                              (isOutputStateDefined<mode, af>() ? GpioRegs::State : GpioRegs(0)) |
                              ((isAfMode<mode>() && (pin_mask & 0x00FF)) ? GpioRegs::AfrL : GpioRegs(0)) |
                              ((isAfMode<mode>() && (pin_mask & 0xFF00)) ? GpioRegs::AfrH : GpioRegs(0)) |
                              (!isAnalog<mode>() ? GpioRegs::Pupdr : GpioRegs(0));
    constexpr uint32_t Moder = (uint32_t(mode) & 3) << pin_num * 2;
    constexpr uint32_t Ospeedr = ((uint32_t(mode) >> 2) & 3) << pin_num * 2;
    constexpr uint32_t OspeedrMask = ((isInputMode<mode>() ? 0 : 3U) << pin_num * 2);
    constexpr uint32_t Pupdr = ((uint32_t(mode) >> 5) & 3) << pin_num * 2;
    constexpr uint32_t PupdrMask = ((isAnalog<mode>() ? 0 : 3U) << pin_num * 2);
    constexpr uint32_t Otyper = ((uint32_t(mode) >> 4) & 1) << pin_num;
    constexpr uint32_t OtyperMask = ((isInputMode<mode>() ? 0 : 1U) << pin_num);
    constexpr uint32_t AfrLMask = ((isAfMode<mode>() && (pin_mask & 0x00FF)) ? 0xFU << pin_num * 4 : 0);
    constexpr uint32_t AfrL = ((pin_mask & 0x00FF) ? (af & 0xF) << pin_num * 4 : 0) & AfrLMask;
    constexpr uint32_t AfrHMask = ((isAfMode<mode>() && (pin_mask & 0xFF00)) ? 0xFU << (pin_num - 8) * 4 : 0);
    constexpr uint32_t AfrH = ((pin_mask & 0xFF00) ? (af & 0xF) << (pin_num - 8) * 4 : 0) & AfrHMask;
    constexpr uint32_t State = (isOutputStateDefined<mode, af>() ? af * pin_mask : 0);
    constexpr uint32_t StateMask = (isOutputStateDefined<mode, af>() ? pin_mask : 0);

    return {UsedRegs, Moder, Ospeedr, OspeedrMask, Pupdr, PupdrMask, Otyper, OtyperMask, AfrL, AfrLMask, AfrH, AfrHMask, State, StateMask};
  }

  template <typename... ModedPins>
  static constexpr ModeValues getModeValues(TypeList<ModedPins...> modedpins)
  {
    return (getModeValues<ModedPins>() | ...);
  }
};

#define MAKE_PORT(PORT_NAME, PWR_CFG) \
template<uint32_t PinsMask = 0xFFFF, typename TPM = STM32Fx_MODE::NotDefined> \
using Gpio##PORT_NAME = TSTM32Fx_GPIO<GPIO##PORT_NAME##_BASE, PinsMask, TPM, PWR_CFG>;\
  
#ifdef GPIOA
  MAKE_PORT(A,PWR_CFG_A)
  MAKE_16PINS(GpioA,A)
#endif
#ifdef GPIOB
  MAKE_PORT(B,PWR_CFG_B)
  MAKE_16PINS(GpioB,B)
#endif
#ifdef GPIOC
  MAKE_PORT(C,PWR_CFG_C)
  MAKE_16PINS(GpioC,C)
#endif
#ifdef GPIOD
  MAKE_PORT(D,PWR_CFG_D)
  MAKE_16PINS(GpioD,D)
#endif
#ifdef GPIOE
  MAKE_PORT(E,PWR_CFG_E)
  MAKE_16PINS(GpioE,E)
#endif
#ifdef GPIOF
  MAKE_PORT(F,PWR_CFG_F)
  MAKE_16PINS(GpioF,F)
#endif
#ifdef GPIOG
  MAKE_PORT(G,PWR_CFG_G)
  MAKE_16PINS(GpioG,G)
#endif
#ifdef GPIOH
  MAKE_PORT(H,PWR_CFG_H)
  MAKE_16PINS(GpioH,H)
#endif
#ifdef GPIOI
  MAKE_PORT(I,PWR_CFG_I)
  MAKE_16PINS(GpioI,I)
#endif
#ifdef GPIOJ
  MAKE_PORT(J,PWR_CFG_J)
  MAKE_16PINS(GpioJ,J)
#endif
#ifdef GPIOK
  MAKE_PORT(K,PWR_CFG_K)
  MAKE_16PINS(GpioK,K)
#endif

}  //namespace STM32Fx_GPIO
