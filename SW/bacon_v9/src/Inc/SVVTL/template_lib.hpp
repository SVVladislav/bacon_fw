#pragma once

#include <stddef.h>
#include <utility>
#include <functional>

#if (__cplusplus > 201703L)

#include <concepts>
#include <bit>
#include "C++20/EnumClasses.hpp"
#include "C++20/TypeList.hpp"
#include "C++20/DataSizes.hpp"

#else

#include "C++17/IAR_defs.h"
#include "C++17/C++20_defs.h"
#include "C++17/EnumClasses.h"
#include "C++17/TypeList.h"
#include "C++17/DataSizes.h"

#endif

#if defined(__RTT_ENABLE__)
#define DEBUG_Print(...) SEGGER_RTT_printf(0, __VA_ARGS__)
#elif defined(__DEBUG__) && defined(__MICROBLAZE__)
#define DEBUG_Print(...) xil_printf(__VA_ARGS__)
#else
#define DEBUG_Print(...) ((void)0)
#endif

template<unsigned N, typename F , auto... args>
__attribute__((always_inline)) inline void Repeat(F f)
{
  if constexpr (N > 0)
  {
    std::invoke(f, args...);
    //f(args...);
    Repeat<N-1, args...>(f);
  }
}

template<unsigned N, typename F, typename... Args>
__attribute__((always_inline)) inline void repeat(F f, Args&&... args)
{
  if constexpr (N > 0)
  {
    std::invoke(f, args...);
    repeat<N - 1>(f,args...);
  }
}


template< unsigned N >
__attribute__((always_inline)) void inline Nops()
{
    if constexpr (N)
    {
#ifdef __ICCAVR__      
      __no_operation();
#elif defined(__AVR_ARCH__)
      asm volatile ("nop");
#else
      __NOP();
#endif
        Nops<N - 1>();
    }
}

#if 1

template <std::size_t N, std::size_t... Is>
constexpr auto make_reverse_index_sequence_impl()
{
    if constexpr (N == 0) return std::index_sequence<Is...>();
    else return make_reverse_index_sequence_impl<N - 1, Is..., N - 1>();
}

template <std::size_t N>
using make_reverse_index_sequence = std::decay_t<decltype(make_reverse_index_sequence_impl<N>())>;

#else

template <size_t Size, size_t... Indexes>
struct make_reverse_index_sequence : make_reverse_index_sequence<Size - 1, Indexes..., Size - 1> {};

template <size_t... Indexes>
struct make_reverse_index_sequence<0, Indexes...> : std::index_sequence<Indexes...> {};

#endif

template <typename T, size_t SIZE>
class ENCODE_TABLE
{
  T table[SIZE]{};
public:   
  struct Iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = value_type*;
    using reference = value_type&;

    constexpr Iterator(const T* ptr) : ptr(ptr) {}

    constexpr void operator++() { ++ptr; }
    constexpr void operator--() { --ptr; }
    constexpr T const& operator* () const { return *ptr; }
    constexpr bool operator==(const Iterator& rhs) const { return ptr == rhs.ptr; }
    constexpr bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }

  private:
    pointer ptr;
  };
  constexpr Iterator begin() const { return Iterator(table); }
  constexpr Iterator end()   const { return Iterator(table + SIZE); }

  template <typename F>
  constexpr ENCODE_TABLE(F f) { size_t i{}; for (auto& x : table) x = f(i++); }
  constexpr auto operator()(const size_t n) const { return table[n]; }
  constexpr auto& operator[](const size_t n) const { return table[n]; }
};

namespace SVVTL
{
  using pVU8 = volatile uint8_t*;
  using pVU16 = volatile uint16_t*;
  using pVU32 = volatile uint32_t*;

  //����������� ������ �� ����� 32-������� �������� � ������������ 8 � 16-������� �������
  template<uint32_t mask>
  static inline void WriteReg32(pVU32 reg, uint32_t value)
  {
    if constexpr (mask == 0) return;
    if constexpr (mask == 0xFFFF'FFFF)
      *reg = value;
    else if constexpr (mask == 0x0000'FFFF)
      *pVU16(reg) = value;
    else if constexpr (mask == 0xFFFF'0000)
      *(pVU16(reg) + 1) = value >> 16;
    else if constexpr (mask == 0x0000'00FF)
      *pVU8(reg) = value;
    else if constexpr (mask == 0x0000'FF00)
      *(pVU8(reg) + 1) = value >> 8;
    else if constexpr (mask == 0x00FF'0000)
      *(pVU8(reg) + 2) = value >> 16;
    else if constexpr (mask == 0xFF00'0000)
      *(pVU8(reg) + 3) = value >> 24;
    else if constexpr ((mask & 0xFFFF'FF00) == 0)
      *pVU8(reg) = (*pVU8(reg) & ~mask) | value;
    else if constexpr (!(mask & 0xFFFF'00FF))
      *(pVU8(reg) + 1) = (*(pVU8(reg) + 1) & ~(mask >> 8)) | (value >> 8);
    else if constexpr (!(mask & 0xFF00'FFFF))
      *(pVU8(reg) + 2) = (*(pVU8(reg) + 2) & ~(mask >> 16)) | (value >> 16);
    else if constexpr (!(mask & 0x00FF'FFFF))
      *(pVU8(reg) + 3) = (*(pVU8(reg) + 3) & ~(mask >> 24)) | (value >> 24);
    else if constexpr (!(mask & 0xFFFF'0000))
      *pVU16(reg) = (*pVU16(reg) & ~mask) | value;
    else if constexpr (!(mask & 0x0000'FFFF))
      *(pVU16(reg) + 1) = (*(pVU16(reg) + 1) & ~(mask >> 16)) | (value >> 16);
    else
      *reg = (*reg & ~mask) | value;
  }

/*
  //����������� ������ �� ����� 16-������� �������� � ������������ 8-������� �������
  template<uint16_t mask>
  static inline void WriteReg16(pVU32 reg, uint16_t value)
  {
    if constexpr (mask == 0) return;
    if constexpr (mask == 0xFFFF)
      *pVU16(reg) = value;
    else if constexpr (mask == 0x00FF)
      *pVU8(reg) = value;
    else if constexpr (mask == 0xFF00)
      *(pVU8(reg) + 1) = value >> 8;
    else if constexpr (!(mask & 0xFF00))
      *pVU8(reg) = (*pVU8(reg) & ~mask) | value;
    else if constexpr ((mask & 0x00FF) == 0)
      *(pVU8(reg) + 1) = (*(pVU8(reg) + 1) & ~(mask >> 8)) | (value >> 8);
    else
      *reg = (*reg & ~mask) | value;
  }
*/

  //����������� ������ �� ����� 32-������� �������� � ������������ 8 � 16-������� �������
  template<uint32_t mask, bool noshift = false>
  static inline uint32_t ReadReg32(pVU32 reg)
  {
    static_assert(mask != 0);
    if constexpr (mask == 0xFFFF'FFFF)
      return *pVU32(reg);
    else if constexpr (mask == 0x0000'FFFF)
      return *pVU16(reg);
    else if constexpr (mask == 0xFFFF'0000)
      return noshift ? (uint32_t)*(pVU16(reg) + 1) << 16 : *(pVU16(reg) + 1);
    else if constexpr (mask == 0x0000'00FF)
      return *pVU8(reg);
    else if constexpr (mask == 0x0000'FF00)
      return noshift ? *(pVU8(reg) + 1) << 8 : *(pVU8(reg) + 1);
    else if constexpr (mask == 0x00FF'0000)
      return noshift ? (uint32_t)*(pVU8(reg) + 2) << 16 : *(pVU8(reg) + 2);
    else if constexpr (mask == 0xFF00'0000)
      return noshift ? (uint32_t)*(pVU8(reg) + 3) << 24 : *(pVU8(reg) + 3);
    else if constexpr ((mask & 0xFFFF'FF00) == 0)
      return *pVU8(reg) & ~mask;
    else if constexpr (!(mask & 0xFFFF'00FF))
      return noshift ? (*(pVU8(reg) + 1) & ~(mask >> 8)) << 8 : *(pVU8(reg) + 1) & ~(mask >> 8);
    else if constexpr (!(mask & 0xFF00'FFFF))
      return noshift ? (*(pVU8(reg) + 2) & ~(mask >> 16)) << 16 : *(pVU8(reg) + 2) & ~(mask >> 16);
    else if constexpr (!(mask & 0x00FF'FFFF))
      return noshift ? (*(pVU8(reg) + 3) & ~(mask >> 24)) << 24 : *(pVU8(reg) + 3) & ~(mask >> 24);
    else if constexpr (!(mask & 0xFFFF'0000))
      return *pVU16(reg) = *pVU16(reg) & ~mask;
    else if constexpr (!(mask & 0x0000'FFFF))
      return noshift ? (*(pVU16(reg) + 1) & ~(mask >> 16)) << 16 : *(pVU16(reg) + 1) & ~(mask >> 16);
    else
      return *reg & ~mask;
  }


  //����������� ������ �� ����� 16-������� �������� � ������������ 8-������� �������
  template<uint32_t pmask>
  static inline uint16_t ReadReg16(pVU32 reg)
  {
    if constexpr (pmask == 0xFFFF)
      return *pVU16(reg);
    else if constexpr (pmask == 0x00FF)
      return *pVU8(reg);
    else if constexpr (pmask == 0xFF00)
      return *(pVU8(reg) + 1) << 8;
    else
      return *reg & pmask;
  }

}