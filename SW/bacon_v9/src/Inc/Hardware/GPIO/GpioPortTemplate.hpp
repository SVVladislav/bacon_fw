#pragma once

template <uint32_t GpioID, uint32_t Width,  uint32_t PinsMask, typename TPM>
struct GpioTemplate : GpioBase
{  
  constexpr GpioTemplate() = default;
  static constexpr auto PinCount() { return std::popcount(PinsMask); }
  static constexpr uint32_t gpio_id = GpioID;
  static constexpr uint32_t width = Width;
  static constexpr uint32_t pins_mask = PinsMask;
  using tpins_mode = TPM;

  template<typename... Ps, typename... Ms>
  static constexpr auto MakeModedPins(TypeList<Ps...> pins, TypeList<Ms...> modes)
  {
    static_assert(sizeof...(Ps) == sizeof...(Ms), "PinList and ModeList sizes is not Equal!");
    return (TypeList<TPin<typename Ps::tgpio, Ps::PinNum(), Ms>>{} + ...);    
  }
  
#if (__cplusplus > 201703L)
  template<typename TGpio, uint32_t Mask = TGpio::pins_mask>
  static consteval auto AddPinsFromRange()
  {
    return []<auto... Is>(std::index_sequence<Is...>)
    {      
      return (std::conditional_t<((1U<<Is)&Mask)!=0, TypeList<TPin<TGpio,Is, typename TGpio::tpins_mode>>, TypeList<>>{} + ...);
    } (make_reverse_index_sequence<TGpio::width>());
  } 
#else
  template<typename TGpio, uint32_t Mask = TGpio::pins_mask>
  static constexpr auto AddPinsFromRange()
  {
    return AddPinsFromRange_<TGpio,Mask>(make_reverse_index_sequence<TGpio::width>());    
  }
  
  template<typename TGpio, uint32_t Mask, auto... Is>
  static constexpr auto AddPinsFromRange_(std::index_sequence<Is...>)
  {
    return (std::conditional_t<((1U<<Is)&Mask)!=0, TypeList<TPin<TGpio, Is, typename TGpio::tpins_mode>>, TypeList<>>{} + ...);    
  } 
#endif
  
};

using pVU8 = volatile uint8_t*;
using pVU16 = volatile uint16_t*;
using pVU32 = volatile uint32_t*;

constexpr uint32_t makeMask(uint32_t width)
{
  uint32_t mask = 0;
  for (uint32_t i = 0; i < width; i++) mask |= 1UL << i;
  return mask;
}

constexpr uint32_t qmul(uint32_t val)
{
  return val | val << 4 | val << 8 | val << 12 | val << 16 | val << 20 | val << 24 | val << 28;
}

constexpr uint32_t dmask(uint32_t val, uint32_t outVal = 0, int cnt = 16)
{
  return cnt ? dmask(val >> 1, (val & 1) ? (outVal >> 2) | 0xC0000000 : outVal >> 2, cnt - 1) : outVal;
}

constexpr uint32_t qmask(uint32_t val, uint32_t outVal = 0, int cnt = 8)
{
  return cnt ? qmask(val >> 1, (val & 1) ? (outVal >> 4) | 0xF0000000 : outVal >> 4, cnt - 1) : outVal;
}