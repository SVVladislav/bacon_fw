#pragma once

template<typename... PME>
struct PinModeList : PinModeListBase
{
  static constexpr auto GetModes() { return modes_; }  
  static constexpr auto Size() { return modes_.size(); }

  // Вытащить из списка пинов список их режимов
  template<typename... Ps>
  static constexpr auto makeModeList(TypeList<Ps...> sortedpins)
  {
    static_assert( (IsPin<Ps>()&&...), "Ps is not Pin");
    return TypeList<typename Ps::tpm...>{};
  }
  
private:
  static constexpr auto ExpandML = [](auto x)
                                   { using T = type_unbox<decltype(x)>;
                                     static_assert(IsPinModeListElement<T>(), "Only Modes and PinModeList allowed");
                                     if constexpr (IsPinModeList<T>()) return T::GetModes();
                                     else return TypeList<T>{}; //IsPinMode
                                   };
        
  static constexpr auto modes_ = TypeList<PME...>::accumulate(ExpandML);
};
