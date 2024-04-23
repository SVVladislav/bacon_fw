#pragma once

#if (__cplusplus <= 201703L) 
namespace std
{
  #define consteval constexpr
   
  template <typename T>
  constexpr int popcount(T value)
  {
    int bits = 0;

    while (value)
    {
      value &= value - 1;
      bits++;
    }
    return bits;
  }
  
  template <typename T>
  constexpr bool has_single_bit(T x)
  { 
    return popcount(x) == 1; 
  }
  
  template<class T>
  struct type_identity 
  {
    using type = T;
  };

  
}
#endif