#pragma once

template <auto val>
struct UVal_to_unsigned
{    
  static_assert(std::is_unsigned_v<decltype(val)>, "val is unsigned");

  using type = std::conditional_t< ((uint64_t)val>0xFFFF'FFFFU), uint64_t, 
               std::conditional_t< ((uint64_t)val>0xFFFFU), uint32_t, 
               std::conditional_t< ((uint64_t)val>0xFFU), uint16_t, uint8_t >>>;
};

template <auto val>
struct UVal_to_unsigned_fast
{    
  static_assert(std::is_unsigned_v<decltype(val)>, "val is unsigned");

  using type = std::conditional_t< ((uint64_t)val>0xFFFF'FFFFU), uint_fast64_t, 
               std::conditional_t< ((uint64_t)val>0xFFFFU), uint_fast32_t, 
               std::conditional_t< ((uint64_t)val>0xFFU), uint_fast16_t, uint_fast8_t >>>;
};

template <auto val>
struct Size_to_unsigned
{    
  static_assert(std::is_integral_v<decltype(val)> && ((val>0)&&(val<=64)), "val is 1...64");

  using type = std::conditional_t< (val>32), uint64_t, 
               std::conditional_t< (val>16), uint32_t, 
               std::conditional_t< (val>8), uint16_t, uint8_t >>>;
};
                                                                          
template <auto val>
struct Size_to_unsigned_fast
{    
  static_assert(std::is_integral_v<decltype(val)> && ((val>0)&&(val<=64)), "val is 1...64");

  using type = std::conditional_t< (val>32), uint_fast64_t, 
               std::conditional_t< (val>16), uint_fast32_t, 
               std::conditional_t< (val>8), uint_fast16_t, uint_fast8_t >>>;
};


template <auto val>
struct Size_to_signed_fast
{    
  static_assert(std::is_integral_v<decltype(val)> && ((val>0)&&(val<=64)), "val is 1...64");

  using type = std::conditional_t< (val>32), int_fast64_t, 
               std::conditional_t< (val>16), int_fast32_t, 
               std::conditional_t< (val>8), int_fast16_t, int_fast8_t >>>;
};


template <auto val>
using UVal_to_unsigned_t = typename UVal_to_unsigned<val>::type;

template <auto val>
using UVal_to_unsigned_fast_t = typename UVal_to_unsigned_fast<val>::type;

template <auto val>
using Size_to_unsigned_t = typename Size_to_unsigned<val>::type;

template <auto val>
using Size_to_unsigned_fast_t = typename Size_to_unsigned_fast<val>::type;

template <auto val>
using Size_to_signed_fast_t = typename Size_to_signed_fast<val>::type;
