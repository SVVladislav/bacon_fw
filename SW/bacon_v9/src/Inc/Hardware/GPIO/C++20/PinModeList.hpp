#pragma once

//------------------------------------------------------------------------------
// class PinModeList
//------------------------------------------------------------------------------
template<IsPinModeListElement... PME>
struct PinModeList : PinModeListBase
{
 static consteval auto GetModes() { return modes_; }  
 static consteval auto Size() { return modes_.size(); }

  // Сформировать спосок режимов из списка пинов
  template<IsPin... Ps>
  static consteval auto makeModeList(TypeList<Ps...>)
  {
    return TypeList<typename Ps::tpm...>{};
  }
  
private:  
  static constexpr auto ExpandML = [](auto x) 
                                   { using T = TypeUnBox<x>;
                                     static_assert(IsPinModeListElement<T>, "Only Modes and PinModeList allowed");
                                     if constexpr (IsPinModeList<T>) return T::GetModes();
                                     else return TypeList<T>{}; //IsPinMode
                                   };

  static constexpr auto modes_ = TypeList<PME...>::accumulate(ExpandML);
};
