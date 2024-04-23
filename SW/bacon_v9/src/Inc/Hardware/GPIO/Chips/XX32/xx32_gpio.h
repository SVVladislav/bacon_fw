#pragma once

//==============================================================================
// STM32F1, CH32  GPIO Port
//==============================================================================

namespace XX32_GPIO
{

struct GPIO_Type
{
  __IO uint32_t CRL;
  __IO uint32_t CRH;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;
};

template<XX32_PinMode Mode, uint32_t ST>
static constexpr bool isOutputStateDefined()
{
  if constexpr ( (Mode==XX32_PinMode::Input_PullUp)||(Mode==XX32_PinMode::Input_PullDown) ||
                 (((Mode==XX32_PinMode::OpenDrain_LowSpeed)||(Mode==XX32_PinMode::OpenDrain_MediumSpeed)||(Mode==XX32_PinMode::OpenDrain_HighSpeed) ||
                   (Mode==XX32_PinMode::PushPull_LowSpeed)||(Mode==XX32_PinMode::PushPull_MediumSpeed)||(Mode==XX32_PinMode::PushPull_HighSpeed))&&(ST<2)) )
    return true;
  else
    return false;
}

template<uint32_t GpioID, uint32_t PinsMask, typename TPM = XX32_MODE::NotDefined,
         uint32_t pwr_crh = 0x4444'4444, uint32_t pwr_crl = 0x4444'4444>
class TXX32_GPIO : public GpioTemplate<GpioID, 16, PinsMask, TPM>
{
  static_assert(PinsMask<=makeMask(16), "MAX port width is 16 bits");
  static constexpr auto base() { return (GPIO_Type*)GpioID; }
  using tGpioTemplate = GpioTemplate<GpioID, 16, PinsMask, TPM>;
public:
  using tgpio = TXX32_GPIO<GpioID, PinsMask, TPM>;

  constexpr TXX32_GPIO() = default;
  static constexpr auto GetPins() { return pins_; }

  template<uint32_t NewPinsMask, uint32_t NewGpioID=GpioID, typename NewTPM = TPM>
  static constexpr auto clone() { return TypeBox<TXX32_GPIO<NewGpioID, NewPinsMask, NewTPM, pwr_crh, pwr_crl>>{}; }
  
  template<uint32_t PM=PinsMask>
  static inline void write(uint32_t data)
  {
    if constexpr (PM == 0xFFFF)
      base()->ODR = data;
    else
      base()->BSRR = (PM << 16) | (data & PM);
  }

  template<uint32_t PM=PinsMask, typename... SortedPins>
  static inline uint16_t read(TypeList<SortedPins...> pins = {})
  {
    if constexpr (PM == 0xFFFF)
      return base()->IDR;
    else
      return base()->IDR & PM;
  }

  template<uint32_t PM=PinsMask>
  static inline uint32_t readOutput()
  {
    if constexpr (PM == 0xFFFF)
      return base()->ODR;
    else
      return base()->ODR & PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void set()
  {
    base()->BSRR = PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void clear()
  {
    if constexpr (PM == 0xFFFF)
      base()->ODR = 0;
    else
      base()->BRR = PM;
  }

  template<uint32_t PM=PinsMask>
  static inline void toggle()
  {
    if constexpr (PM == 0xFFFF)
      base()->ODR = ~base()->ODR;
    else
      if constexpr (std::has_single_bit(PM)==1)
        if(base()->ODR & PM) clear<PM>(); else set<PM>();
      else  
        base()->BSRR = (PM << 16) | (~base()->ODR & PM);
  }

  template<uint32_t PM=PinsMask>
  static inline void lock()
  {
    base()->LCKR = PM | 0x0001'0000;
    base()->LCKR = PM;
    base()->LCKR = PM | 0x0001'0000;
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
    pins_mode<false>(tGpioTemplate::MakeModedPins(pins_,PML::GetModes()));
  }
#endif

  // Перегрузка операторов    
  inline operator uint32_t() const { return read(); }  
  
  inline TXX32_GPIO& operator=(const TXX32_GPIO& gpio)
  {
    write(gpio.read());
    return *this;
  }
  
  inline TXX32_GPIO& operator=(uint32_t value)
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
  
  struct ModeValues
  {
    uint32_t CRL       = 0;
    uint32_t CRLMask   = 0;
    uint32_t CRH       = 0;
    uint32_t CRHMask   = 0;
    uint32_t State     = 0;
    uint32_t StateMask = 0;
    constexpr ModeValues operator|(const ModeValues& rv)
    {      
      return { CRL | rv.CRL, CRLMask | rv.CRLMask,
               CRH | rv.CRH, CRHMask | rv.CRHMask,
               State | rv.State, StateMask | rv.StateMask };
    }
  };                       

public:  
  template<bool UsePwrCfg=false, typename... Ps>
  static inline void pins_mode(TypeList<Ps...> pins)
  {
    static_assert(std::popcount(PinsMask) == sizeof...(Ps), "PinList and Port size is not Equal!");

    static constexpr auto mode_values = getModeValues<UsePwrCfg>(pins);

    if constexpr (((PinsMask & 0x00FF) != 0) && (mode_values.CRLMask != 0))
    {
      static constexpr uint32_t qm = qmask(PinsMask);
      if constexpr(qm==0xFFFFFFFF)
        base()->CRL = mode_values.CRL;
      else
        base()->CRL = base()->CRL & ~qm | mode_values.CRL & qm;
    }

    if constexpr (((PinsMask & 0xFF00) != 0) && (mode_values.CRHMask != 0))
    {
      static constexpr uint32_t qm = qmask(PinsMask>>8);
      if constexpr(qm==0xFFFFFFFF)
        base()->CRH = mode_values.CRH;
      else
        base()->CRH = base()->CRH & ~qm | mode_values.CRH & qm;
    }

    if constexpr (mode_values.StateMask != 0)
      write<mode_values.StateMask>(mode_values.State);
  }

private:  
  template<bool UsePwrCfg, typename... ModedPins>
  static constexpr ModeValues getModeValues(TypeList<ModedPins...> moded_pins)
  {
    return ( getModeValues<UsePwrCfg, ModedPins>() | ...);
  }
  
  template<bool UsePwrCfg, typename ModedPin>
  static constexpr ModeValues getModeValues()
  {
    constexpr uint32_t pin_num = ModedPin::PinNum();
    constexpr uint32_t pin_mask = ModedPin::PinMask();
    constexpr auto mode = ModedPin::tpm::pin_mode;    
    static_assert(mode!=XX32_PinMode::NotDefined,"Pin mode undefined");    
    constexpr uint32_t state     = ModedPin::tpm::state;
    
    constexpr uint32_t StateMask = (isOutputStateDefined<mode,state>() ? pin_mask : 0);
    constexpr uint32_t State     =  ( (mode==XX32_PinMode::Input_PullUp) ? pin_mask :
                                      (mode==XX32_PinMode::Input_PullDown) ? 0 :
                                      isOutputStateDefined<mode,state>() ? (state<<pin_num) : 0 );

    constexpr uint32_t CRL = ((pin_mask & 0x00FF) ? (uint32_t(mode) & 0xF) << pin_num * 4 : 0);    
    constexpr uint32_t CRLMask = (pin_mask & 0x00FF) ? ( (UsePwrCfg == false) ? pin_mask
                                                                              : ( (CRL != (pwr_crl&(0xFu<<pin_num*4))) ? pin_mask : 0 )
                                                       ) : 0;     
          
    constexpr uint32_t CRH     = ((pin_mask&0xFF00)? (uint32_t(mode)&0xF) << (pin_num-8) * 4 :  0);
    constexpr uint32_t CRHMask = (pin_mask & 0xFF00) ? ( (UsePwrCfg == false) ? pin_mask
                                                                              : ( (CRH != (pwr_crh&(0xFu<<(pin_num-8)*4))) ? pin_mask : 0)
                                                       ) : 0;

    return { CRL, CRLMask, CRH, CRHMask, State, StateMask };
  }
};

#define MAKE_PORT(PORT_NAME, PWR_CFG) \
template<uint32_t PinsMask = 0xFFFF, typename TPM = XX32_MODE::NotDefined>\
using Gpio##PORT_NAME = TXX32_GPIO<GPIO##PORT_NAME##_BASE, PinsMask, TPM, PWR_CFG>;\
  
#ifdef GPIOA
  MAKE_PORT(A, PWR_CFG_A)
  MAKE_16PINS(GpioA,A)
#endif
#ifdef GPIOB
  MAKE_PORT(B, PWR_CFG_B)
  MAKE_16PINS(GpioB,B)
#endif
#ifdef GPIOC
  MAKE_PORT(C, PWR_CFG_C)
  MAKE_16PINS(GpioC,C)
#endif
#ifdef GPIOD
  MAKE_PORT(D, PWR_CFG_D)
  MAKE_16PINS(GpioD,D)
#endif
#ifdef GPIOE
  MAKE_PORT(E, PWR_CFG_E)
  MAKE_16PINS(GpioE,E)
#endif
#ifdef GPIOF
  MAKE_PORT(F, PWR_CFG_F)
  MAKE_16PINS(GpioF,F)
#endif
#ifdef GPIOG
  MAKE_PORT(G, PWR_CFG_G)
  MAKE_16PINS(GpioG,G)
#endif
#ifdef GPIOH
  MAKE_PORT(H, PWR_CFG_H)
  MAKE_16PINS(GpioH,H)
#endif

}  //namespace XX32_GPIO
