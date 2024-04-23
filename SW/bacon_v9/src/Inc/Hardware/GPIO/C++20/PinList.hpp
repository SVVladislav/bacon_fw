#pragma once

//------------------------------------------------------------------------------
// class PinList
//------------------------------------------------------------------------------
template<IsPinListElement... PLEs>
class PinList final : PinListBase
{

  static constexpr auto ExpandPL = [](auto x)
                                   { using T = TypeUnBox<x>;
                                     static_assert(IsPinListElement<T>, "Only Pin, Gpio and PinList allowed in PinList");
                                     if constexpr (IsPinList<T>) return T::GetPins();
                                     else if constexpr (IsGpio<T>) return T::template AddPinsFromRange<T>();
                                     else return TypeList<T>{}; // IsPin<T>
                                   };

  static constexpr auto pins_ = TypeList<PLEs...>::accumulate(ExpandPL);
  //static_assert(pins_.size(), "Empty PinList");

  using TWidth = Size_to_unsigned_fast_t<pins_.size()>;  // Bus width type

public:
  constexpr PinList() = default;
  static consteval auto PinCount() { return pins_.size(); }
  static consteval auto PortCount() { return ports_.size(); }
  static consteval auto GetPins() { return pins_; }
  static consteval auto GetPorts() { return ports_; }

  template <IsPin... Ps> // Излечь список портов из внешнего списка пинов Ps
  static constexpr auto GetPorts(TypeList<Ps...> pins) { return getPortList_(pins, pins); }

  static inline void set()    { ports_.foreach([](auto port){ TypeUnBox<port>::set();    }); }
  static inline void clear()  { ports_.foreach([](auto port){ TypeUnBox<port>::clear();  }); }
  static inline void toggle() { ports_.foreach([](auto port){ TypeUnBox<port>::toggle(); }); }
  static inline void lock()   { ports_.foreach([](auto port){ TypeUnBox<port>::lock();   }); }

  static inline void write(TWidth value)
  {
    static_assert(PinCount() <= 64, "Max PinList size for write is 64!");
    ports_.foreach([value](auto port){ TypeUnBox<port>::write(readWrite<PinShift::Write>(value, getIndexedPins<TypeUnBox<port>>(pins_))); });
  }

  template <TWidth Value>
  static inline void write()
  {
    static_assert(PinCount() <= 64, "Max PinList size for write is 64!");
    ports_.foreach([](auto port){ TypeUnBox<port>::write(calcConst<Value>(getIndexedPins<TypeUnBox<port>>(pins_))); });
  }

  static inline TWidth read()
  {
    static_assert(PinCount() <= 64, "Max PinList size for read is 64!");
    return []<typename... Ports>(TypeList<Ports...>)
    {
      return (readWrite<PinShift::Read>(Ports::read(GetPins<Ports>(pins_)), getIndexedPins<Ports>(pins_)) | ...);
    } (ports_);
  }

  // Задать всем пинам одинаковый режим
  template<IsPinMode Mode>
  static inline void mode()                 // PinList::mode<PinMode>()
  {
    ports_.foreach([](auto port)
                   { 
                     constexpr auto ps_  = TypeUnBox<port>::GetPins();
                     constexpr auto mds_ = TypeList<>::generate<TypeUnBox<port>::PinCount(), Mode>();
                     using tgpio_ = TypeUnBox<port>::tgpio;
                     TypeUnBox<port>::pins_mode(tgpio_::MakeModedPins(ps_,mds_));
                   });
  }
  
  // Установить режимы всем пинам согласно того как они были определены
  // при объявлении типа PinList. Режимы всех пинов должны быть заданы.
  template<bool UsePwrCfg = false>
  static inline void modes() { ports_.foreach([](auto port){ TypeUnBox<port>::template pins_mode<UsePwrCfg>(GetPins<TypeUnBox<port>>(pins_)); }); }

  static inline void modes() { modes<false>(); }

  // Установить всем пинам режимы в соответствии со списком режимов PinModeList
  template<IsPinModeListElement... ML>
  static inline void modes()           // PinList::modes<PinModeList>()
  {
    ports_.foreach([](auto port)
                   { 
                     using tgpio_ = TypeUnBox<port>::tgpio;
                     TypeUnBox<port>::pins_mode(GetPins<TypeUnBox<port>>(tgpio_::MakeModedPins(pins_, PinModeList<ML...>::GetModes())));
                   });
  }

private:
  enum class PinShift { Write = 1, Read = 2, Reverse = 4, WriteRev = Write | Reverse, ReadRev = Read | Reverse };

  template <uint8_t PinIndex, uint8_t DataIndex>
  struct IndexedPin
  {
    static constexpr uint8_t pinIndex = PinIndex;
    static constexpr uint8_t dataIndex = DataIndex;
  };

  // Собрать маску из всех пинов, принадлежащих этому порту
  template <IsGpio Port, IsPin... Ps>
  static consteval uint32_t getPinsMask(TypeList<Ps...>)
  {
    return (((Port::gpio_id == Ps::tgpio::gpio_id)?Ps::PinMask():0) | ...);
  }
  
  template <PinShift ShiftType, typename Pin>
  static consteval int8_t calcShift()
  {
    constexpr int8_t msbit = sizeof(TWidth) * 8 - 1;
    if (ShiftType == PinShift::Write)    return int8_t(Pin::pinIndex) - int8_t(Pin::dataIndex);
    if (ShiftType == PinShift::WriteRev) return int8_t(Pin::pinIndex) - (msbit - int8_t(Pin::dataIndex));
    if (ShiftType == PinShift::Read)     return int8_t(Pin::dataIndex) - int8_t(Pin::pinIndex);
    return int8_t(Pin::dataIndex) - (msbit - int8_t(Pin::pinIndex));
  }

  template <int8_t Shift, PinShift ShiftType, typename... Ps>
  static consteval TWidth getMaskForShift(TypeList<Ps...> pins)
  {
    return (((calcShift<ShiftType, Ps>() == Shift) ? (hasFlag(ShiftType, PinShift::Write) ? 1UL << Ps::dataIndex : 1UL << Ps::pinIndex) : 0) | ...);
  }

  template <int8_t Shift, PinShift ShiftType, typename... Ps>
  static consteval auto getListExceptShift(TypeList<Ps...> pins)
  {
    return (std::conditional_t<calcShift<ShiftType, Ps>() != Shift, TypeList<Ps>, TypeList<>>{} + ...);
  }

  template <PinShift ShiftType, typename... Ps>
  static inline TWidth readWrite(TWidth value, TypeList<Ps...> pins)
  {
    if constexpr (!pins.is_empty())
    {      
      using Pin = TypeUnBox<pins.head()>;
      static constexpr auto shift = calcShift<ShiftType, Pin>();
      static constexpr TWidth mask = getMaskForShift<shift, ShiftType>(pins);
#ifdef __RBIT_ENABLE
      static constexpr auto ShiftTypeRev = ShiftType | PinShift::Reverse;
      static constexpr auto shiftRev = calcShift<ShiftTypeRev, Pin>();
      static constexpr auto maskRev = getMaskForShift<shiftRev, ShiftTypeRev>(pins);
      if constexpr ((std::popcount(maskRev) > 2) && (std::popcount(maskRev) > std::popcount((TWidth)mask)) && (sizeof(TWidth) == 32))
      {
        if constexpr (shiftRev >= 0)
          return __RBIT(value & maskRev) << shiftRev | readWrite<ShiftType>(value, getListExceptShift<shiftRev, ShiftTypeRev>(pins));
        else
          return __RBIT(value & maskRev) >> -shiftRev | readWrite<ShiftType>(value, getListExceptShift<shiftRev, ShiftTypeRev>(pins));
      }
      else
#endif
      {
        if constexpr (shift >= 0)
          return (value & mask) << shift | readWrite<ShiftType>(value, getListExceptShift<shift, ShiftType>(pins));
        else
          return (value & mask) >> -shift | readWrite<ShiftType>(value, getListExceptShift<shift, ShiftType>(pins));
      }
    }
    else return 0;
  }

public:
  template <IsGpio Port, IsPin... Ps>
  static consteval auto GetPins(TypeList<Ps...> pins)
  {    
    return pins.template filter<Port>([](auto port, auto pin){ return (TypeUnBox<port>::gpio_id == TypeUnBox<pin>::tgpio::gpio_id); }); 
  }
  
private:
  // Проиндексировать пины в пределах одного порта
  template <IsGpio Port, IsPin... Ps>
  static consteval auto getIndexedPins(TypeList<Ps...>)
  {
    return []<auto... Is>(std::index_sequence<Is...>)
    {
      return (std::conditional_t<Port::gpio_id==Ps::tgpio::gpio_id, TypeList<IndexedPin<Ps::PinNum(), sizeof...(Ps) - Is - 1>>, TypeList<>>{} + ...);
    }
    (std::make_index_sequence<sizeof...(Ps)>());
  }

  template <TWidth Value, typename... Ps>
  static consteval TWidth calcConst(TypeList<Ps...> pins)
  {
    return ((bool(Value & (1U << Ps::dataIndex)) << Ps::pinIndex) | ...);
  }
 
public:
  template <IsPin... Ps, IsPin... AllPs, typename... PortID>
  static consteval auto getPortList_(TypeList<Ps...> pins, TypeList<AllPs...> allpins, TypeList<PortID...> portids = {})
  {
    if constexpr (!pins.is_empty())
    {
      using Port = TypeUnBox<pins.head()>::tgpio;
      if constexpr (!portids.template contains<ValueBox<Port::gpio_id>>())
      {
        using TportClone = TypeUnBox<Port::template clone<getPinsMask<Port>(allpins)>()>;
        return TypeList<TportClone>{} + getPortList_(pins.tail(), allpins, portids.template push_back<ValueBox<Port::gpio_id>>());
      }
      else
        return getPortList_(pins.tail(), allpins, portids);
    }
    else return TypeList<>{};
  }
  
private:  
  static constexpr auto ports_ = GetPorts(pins_);

  static_assert(pins_.is_unique([](auto pin1, auto pin2)
                                {
                                  return (TypeUnBox<pin1>::tgpio::gpio_id == TypeUnBox<pin2>::tgpio::gpio_id) &&
                                         (TypeUnBox<pin1>::PinMask() == TypeUnBox<pin2>::PinMask());
                                }), "PinList has Duplicates!");
public:

  inline operator TWidth() { return read(); }
  
  template<typename... Ps> requires (PinList<Ps...>::PinCount()==PinCount())
  inline PinList& operator=(const PinList<Ps...>& pins)
  {
    write(pins.read());
    return *this;
  }
  
  inline PinList& operator=(const PinList &pins)
  {
    ports_.foreach([](auto port){ TypeUnBox<port>::write(TypeUnBox<port>::read()); });
    return *this;
  }

  inline PinList& operator=(TWidth value)
  {
    write(value);
    return *this;
  }  
  
};

template <typename... T>
constexpr auto ToPinList(TypeList<T...>) { return PinList<T...>{}; }
