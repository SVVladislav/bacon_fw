#pragma once

//------------------------------------------------------------------------------
// class PinList
//------------------------------------------------------------------------------
template<typename... PLEs>
class PinList : PinListBase
{
  static_assert((IsPinListElement<PLEs>()&&...),"Pins is not Pin or GPIO or PinList");  
  
  static constexpr auto ExpandPL = [](auto x)
                                   { using T = type_unbox<decltype(x)>;
                                     static_assert(IsPinListElement<T>(), "Only Pin, Gpio and PinList allowed in PinList");
                                     if constexpr (IsPinList<T>()) return T::GetPins();
                                     else if constexpr (IsGpio<T>()) return T::template AddPinsFromRange<T>();
                                     else return TypeList<T>{}; // IsPin<T>
                                   };
  
  static constexpr auto pins_ = TypeList<PLEs...>::accumulate(ExpandPL);
  //static_assert(pins_.size(), "Empty PinList");

  using TWidth = Size_to_signed_fast_t<std::max(pins_.size(), (unsigned)std::popcount(std::numeric_limits<unsigned int>::max())) >;  // Bus width type
  //using TWidth = Size_to_signed_fast_t<pins_.size()>;  // Bus width type
  
public:
  constexpr PinList() = default;
  static constexpr auto PinCount() { return pins_.size(); }
  static constexpr auto PortCount() { return ports_.size(); }
  static constexpr auto GetPins() { return pins_; }
  static constexpr auto GetPorts() { return ports_; }
  template<typename... Ps> // Излечь список портов из внешнего списка пинов Ps
  static constexpr auto GetPorts(TypeList<Ps...> pins) { return getPortList_(pins,pins); }
    
  static inline void set()   { ports_.foreach([](auto port){ type_unbox<decltype(port)>::set();    }); }
  static inline void clear() { ports_.foreach([](auto port){ type_unbox<decltype(port)>::clear();  }); }
  static inline void toggle(){ ports_.foreach([](auto port){ type_unbox<decltype(port)>::toggle(); }); }
  static inline void lock()  { ports_.foreach([](auto port){ type_unbox<decltype(port)>::lock();   }); }
 
  static inline void write(TWidth value)
  {
    static_assert(PinCount() <= 64, "Max PinList size for write is 64!");
    ports_.foreach([value](auto port){ type_unbox<decltype(port)>::write(readWrite<PinShift::Write>(value, getIndexedPins<type_unbox<decltype(port)>>(pins_))); });
  }

  template<TWidth Value>
  static inline void write()
  {
    static_assert(PinCount() <= 64, "Max PinList size for write is 64!");
    ports_.foreach ([](auto port) { type_unbox<decltype(port)>::write(calcConst<Value>(getIndexedPins<type_unbox<decltype(port)>>(pins_))); });
  }
  
  static inline TWidth read()
  {
    static_assert(PinCount() <= 64, "Max PinList size for read is 64!");
    return read_(ports_);    
  }

  // Задать всем пинам одинаковый режим
  template<typename Mode, typename = std::enable_if_t<IsPinMode<Mode>()>>
  static inline void mode()                 // PinList::mode<PinMode>()
  {    
    ports_.foreach([](auto port)
                   {
                     constexpr auto ps_  = type_unbox<decltype(port)>::GetPins();
                     constexpr auto mds_ = TypeList<>::generate<type_unbox<decltype(port)>::PinCount(),Mode>();
                     using tgpio_ = typename type_unbox<decltype(port)>::tgpio;                     
                     type_unbox<decltype(port)>::pins_mode(tgpio_::MakeModedPins(ps_, mds_));
                   });
  }
  
  // Установить режимы всем пинам согласно того как они были определены
  // при объявлении типа PinList. Режимы всех пинов должны быть заданы.
  template<bool UsePwrCfg = false>
  static inline void modes() { ports_.foreach([](auto port){ type_unbox<decltype(port)>::template pins_mode<UsePwrCfg>(GetPins<type_unbox<decltype(port)>>(pins_)); }); }

  static inline void modes() { modes<false>(); }
  
  // Установить всем пинам режимы в соответствии со списком режимов PinModeList
  template<typename... ML, typename = std::enable_if_t<(IsPinModeListElement<ML>()&&...)>>
  static inline void modes()                 // PinList::modes<PinModeList>()
  {
    ports_.foreach([](auto port)
                   { 
                     using tgpio_ = typename type_unbox<decltype(port)>::tgpio;
                     type_unbox<decltype(port)>::pins_mode(GetPins<type_unbox<decltype(port)>>(tgpio_::MakeModedPins(pins_, PinModeList<ML...>::GetModes())));
                   });
  }
  
private:
  enum class PinShift { Write = 1, Read = 2, Reverse = 4, WriteRev = Write | Reverse, ReadRev = Read | Reverse };

  template<uint8_t PinIndex, uint8_t DataIndex>
  struct IndexedPin
  {
    static constexpr uint8_t pinIndex = PinIndex;
    static constexpr uint8_t dataIndex = DataIndex;
  };  
  
  // Собрать маску из всех пинов, принадлежащих этому порту
  template<typename TPort, typename... Ps>
  static constexpr uint32_t getPinsMask(TypeList<Ps...> pins)  //Тут подумать над разрядностью
  {
    static_assert( IsGpio<TPort>(), "Ps is not Port");
    static_assert( (IsPin<Ps>() && ...), "Ps is not pin");
    return (((TPort::gpio_id == Ps::tgpio::gpio_id)?Ps::PinMask():0) | ...);
  }
  
  template<PinShift ShiftType, typename Pin>
  static constexpr int8_t calcShift()
  {
    constexpr int8_t msbit = sizeof(TWidth) * 8 - 1;
    if (ShiftType == PinShift::Write)   return int8_t(Pin::pinIndex) - int8_t(Pin::dataIndex);
    if (ShiftType == PinShift::WriteRev)  return int8_t(Pin::pinIndex) - (msbit - int8_t(Pin::dataIndex));
    if (ShiftType == PinShift::Read)    return int8_t(Pin::dataIndex) - int8_t(Pin::pinIndex);
    return int8_t(Pin::dataIndex) - (msbit - int8_t(Pin::pinIndex)); // PinShift::ReadRev
  }
  
  template<int8_t Shift, PinShift ShiftType, typename... Ps>
  static constexpr TWidth getMaskForShift(TypeList<Ps...> pins)
  {
    return (((calcShift<ShiftType, Ps>() == Shift) ? (hasFlag(ShiftType, PinShift::Write) ? TWidth(1) << Ps::dataIndex : TWidth(1) << Ps::pinIndex) : 0) | ...);
  }
  
  template<int8_t Shift, PinShift ShiftType, typename... Ps>
  static constexpr auto getListExceptShift(TypeList<Ps...> pins)
  {
    return (std::conditional_t<calcShift<ShiftType, Ps>() != Shift, TypeList<Ps>, TypeList<>>{} + ...);
  }
  
  template<PinShift ShiftType, typename... Ps>
  static inline TWidth readWrite(TWidth value, TypeList<Ps...> pins)
  {
    if constexpr (!pins.is_empty())
    {      
      using Pin = type_unbox<decltype(pins.head())>;
      static constexpr auto shift = calcShift<ShiftType, Pin>();
      static constexpr TWidth mask = getMaskForShift<shift, ShiftType>(pins);
#ifdef __RBIT_ENABLE
      constexpr auto ShiftTypeRev = ShiftType | PinShift::Reverse;
      constexpr auto shiftRev = calcShift<ShiftTypeRev, Pin>();
      constexpr auto maskRev = getMaskForShift<shiftRev, ShiftTypeRev>(pins);
      if constexpr ((std::popcount(maskRev)>2) && (std::popcount(maskRev)>std::popcount(mask)) && (sizeof(TWidth)==32))
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
  
  template<typename TPort, typename... Ps>
  static constexpr auto GetPins(TypeList<Ps...>)
  {
    return (std::conditional_t<TPort::gpio_id==Ps::tgpio::gpio_id, TypeList<Ps>, TypeList<>>{} + ...);
  }
  
private:
  
  // Проиндексировать пины в пределах одного порта
  template<typename Port, typename... Ps>
  static constexpr auto getIndexedPins(TypeList<Ps...> pins)
  {
    if constexpr (!pins.is_empty())
    {
      using Head = type_unbox<decltype(pins.head())>;
      if constexpr (Head::tgpio::gpio_id == Port::gpio_id)
        return getIndexedPins<Port>(pins.tail()) + TypeList<IndexedPin<Head::PinNum(), sizeof...(Ps) - 1>>{};
      else
        return getIndexedPins<Port>(pins.tail());
    }
    else return TypeList<>{};
  }
  
  template<TWidth Value, typename... Ps>
  static constexpr TWidth calcConst(TypeList<Ps...> pins)
  {
    return ((bool(Value & (1U << Ps::dataIndex)) << Ps::pinIndex) | ...);
  }
  
public:  
  // Создаёт список используемых портов посредством клонирования с просчитанной маской
  template<typename... Ps, typename... AllPs, typename... PortID>
  static constexpr auto getPortList_(TypeList<Ps...> pins, TypeList<AllPs...> allpins, TypeList<PortID...> portids = {})
  {
    if constexpr (!pins.is_empty())
    {
      using Port = typename type_unbox<decltype(pins.head())>::tgpio;
      if constexpr (!portids.template contains<ValueBox<Port::gpio_id>>())      
      {
        using TportClone = type_unbox<decltype(Port::template clone<getPinsMask<Port>(allpins)>())>;
        return TypeList<TportClone>{} + getPortList_(pins.tail(), allpins, portids.template push_back<ValueBox<Port::gpio_id>>());        
      }
      else
        return getPortList_(pins.tail(), allpins, portids);
    }
    else return TypeList<>{};
  }
  
private:
  
  template<typename... Ports>
  static inline TWidth read_(TypeList<Ports...> ports)
  {
    static_assert( (IsGpio<Ports>() && ...), "Ports is not Port");
    return (readWrite<PinShift::Read>(Ports::read(GetPins<Ports>(pins_)), getIndexedPins<Ports>(pins_)) | ...);
  }
      
  static constexpr auto ports_ = GetPorts(pins_);

  static_assert(pins_.is_unique([](auto pin1, auto pin2)
                                {
                                  return (type_unbox<decltype(pin1)>::tgpio::gpio_id == type_unbox<decltype(pin2)>::tgpio::gpio_id) &&
                                         (type_unbox<decltype(pin1)>::PinMask() == type_unbox<decltype(pin2)>::PinMask());
                                }), "PinList has Duplicates!"); 
 public: 
  
  inline operator TWidth() { return read();  }

  template<typename... Ps, typename = std::enable_if_t<(PinList<Ps...>::PinCount() == PinCount())>>
  inline PinList & operator=(const PinList<Ps...>& pins)
  {
    write(pins.read());
    return *this;
  }

  inline PinList& operator=(const PinList& pins)
  {
    ports_.foreach([](auto port){ type_unbox<decltype(port)>::write(type_unbox<decltype(port)>::read()); });
    return *this;
  }

  inline PinList& operator=(TWidth value)
  {
    write(value);
    return *this;
  }
  
};

template<typename... T>
constexpr auto ToPinList(TypeList<T...>) { return PinList<T...>{}; }
