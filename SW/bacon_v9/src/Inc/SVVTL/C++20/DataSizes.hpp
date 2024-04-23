#pragma once

template <std::unsigned_integral auto val>
struct UVal_to_unsigned
{
    using type = std::conditional_t< ((uint64_t)val > 0xFFFF'FFFFU), uint64_t,
        std::conditional_t< ((uint64_t)val > 0xFFFFU), uint32_t,
        std::conditional_t< ((uint64_t)val > 0xFFU), uint16_t, uint8_t >>>;
};

template <std::unsigned_integral auto val>
struct UVal_to_unsigned_fast
{
    using type = std::conditional_t< ((uint64_t)val > 0xFFFF'FFFFU), uint_fast64_t,
        std::conditional_t< ((uint64_t)val > 0xFFFFU), uint_fast32_t,
        std::conditional_t< ((uint64_t)val > 0xFFU), uint_fast16_t, uint_fast8_t >>>;
};

template <std::integral auto val> requires ((val >= 0) && (val <= 64))
struct Size_to_unsigned
{
    using type = std::conditional_t< (val > 32), uint64_t,
        std::conditional_t< (val > 16), uint32_t,
        std::conditional_t< (val > 8), uint16_t, uint8_t >>>;
};

template <std::integral auto val> requires ((val >= 0) && (val <= 64))
struct Size_to_unsigned_fast
{
    using type = std::conditional_t< (val > 32), uint_fast64_t,
        std::conditional_t< (val > 16), uint_fast32_t,
        std::conditional_t< (val > 8), uint_fast16_t, uint_fast8_t >>>;
};

template <std::integral auto val> requires ((val >= 0) && (val <= 64))
struct Size_to_signed_fast
{
    using type = std::conditional_t< (val > 32), int_fast64_t,
        std::conditional_t< (val > 16), int_fast32_t,
        std::conditional_t< (val > 8), int_fast16_t, int_fast8_t >>>;
};

template <auto val>
using UVal_to_unsigned_t = UVal_to_unsigned<val>::type;

template <auto val>
using UVal_to_unsigned_fast_t = UVal_to_unsigned_fast<val>::type;

template <auto val>
using Size_to_unsigned_t = Size_to_unsigned<val>::type;

template <auto val>
using Size_to_unsigned_fast_t = Size_to_unsigned_fast<val>::type;

template <auto val>
using Size_to_signed_fast_t = typename Size_to_signed_fast<val>::type;
