#pragma once

//------------------------------------------------------------------------------
// class ConfigList
//------------------------------------------------------------------------------

enum class ConfigListCMD : uint32_t { AllUnusedPins };

template<ConfigListCMD CfgCMD>
struct TCfgCmd : ConfigListCMDBase
{
  static constexpr ConfigListCMD cfg_cmd = CfgCMD;
};

namespace CfgCmd
{
  using AllUnusedPins = TCfgCmd<ConfigListCMD::AllUnusedPins>;
}

template<typename... CLE>
class ConfigList : ConfigListBase
{
  static_assert((IsConfigListElement<CLE>()&&...),"CLE is not ConfigList Element");
  template <typename...>  friend class ConfigList;
  enum class PrevElem{ First, Pin, Gpio, PinList, PinMode, ConfigList, CMD };
public:
  constexpr ConfigList() = default;
  // Задать конфигурацию
  static inline void mode() { mode_<false>(); } 
  // Установить конфигурацию с учётом состояния регистров при подаче питания
  static inline void pwr_config() { mode_<true>(); }
  
  //static constexpr auto GetPins() { return moded_pin_list_; }
  
  static constexpr auto PinCount() { return moded_pins_.size(); }

private:
  template<typename... Ports>
  static constexpr auto getUnusedPins(TypeList<Ports...> ports)
  {
    return ((Ports::tgpio::template AddPinsFromRange<Ports, ~Ports::pins_mask& makeMask(Ports::width)>()) + ... + TypeList<>{});  // + TypeList<>{} вроде не нужно
  }

  template<typename TPM = void, typename... Sls, typename... OutsPins>
  static constexpr auto SimpleListToModedPins(TypeList<Sls...> sls, TypeList<OutsPins...> outpins = {})
  {
    if constexpr (!sls.is_empty())
    {
      using Head = type_unbox<decltype(sls.head())>;
      if constexpr(IsPinMode<Head>())
        return SimpleListToModedPins<Head>(sls.tail(), outpins);
      else if constexpr(IsPin<Head>())
      {   
        using tmoded_pin = type_unbox<decltype(Head::tgpio::MakeModedPins(TypeList<Head>{}, TypeList<TPM>{}).head()) > ;
        return SimpleListToModedPins<TPM>(sls.tail(), outpins.template push_back<tmoded_pin>());
      }
      else //ConfigListCMDBase
        return SimpleListToModedPins<TPM>(getUnusedPins(PinList<OutsPins...>::GetPorts()), outpins);
    }
    else return outpins;
  }

  // Transforn ConfigList to SimpleList : <Pins, Modes and CMD>
  template<PrevElem prev=PrevElem::First, typename... Cls>
  static constexpr auto transformToSimpleList(TypeList<Cls...> cls)
  {
    if constexpr (!cls.is_empty())
    {
      using Head = type_unbox<decltype(cls.head())>;
      
      static_assert( prev!=PrevElem::ConfigList || IsPinMode<Head>() || IsConfigList<Head>(), "ConfigList: After ConfigList should be PinMode or ConfigList");
      static_assert( prev!=PrevElem::First || IsPinMode<Head>() || IsConfigList<Head>(), "ConfigList: First Element of ConfigList should be PinMode or ConfigList");
      static_assert( !IsConfigListCMD<Head>() || (cls.size()==1), "ConfigList: AllUnusedPins must be at the end");
      static_assert( !(IsPinMode<Head>() && (cls.size()==1)), "ConfigList: PinMode at the end");
      static_assert( prev!=PrevElem::PinMode || !IsPinMode<Head>(), "ConfigList: Two PinMode in a row" );

      if constexpr (IsPinList<Head>())
        return Head::GetPins() + transformToSimpleList<PrevElem::PinList>(cls.tail());
      else if constexpr (IsGpio<Head>())
        return Head::tgpio::template AddPinsFromRange<Head>() + transformToSimpleList<PrevElem::PinList>(cls.tail());        
      else if constexpr (IsConfigList<Head>())
        return Head::simple_list_ + transformToSimpleList<PrevElem::ConfigList>(cls.tail());
      else if constexpr (IsPin<Head>())
        return TypeList<Head>{} + transformToSimpleList<PrevElem::Pin>(cls.tail());
      else if constexpr (IsConfigListCMD<Head>())
        return TypeList<Head>{};
      else // PinMode
        return TypeList<Head>{} + transformToSimpleList<PrevElem::PinMode>(cls.tail());
    }
    else return TypeList<>{};
  }
  
  template<bool UsePwrCfg = false>
  static inline void mode_()
  { 
    ports_.foreach([](auto port){ type_unbox<decltype(port)>::template pins_mode<UsePwrCfg>(PinList<>::GetPins<type_unbox<decltype(port)>>(moded_pins_)); });
  }  

  
  static constexpr auto simple_list_ = transformToSimpleList(TypeList<CLE...>{});
  static constexpr auto moded_pins_ = SimpleListToModedPins(simple_list_);
  static_assert(moded_pins_.is_unique(), "ConfigList has Duplicates!");
  static constexpr auto ports_ = PinList<>::getPortList_(moded_pins_,moded_pins_);
};
