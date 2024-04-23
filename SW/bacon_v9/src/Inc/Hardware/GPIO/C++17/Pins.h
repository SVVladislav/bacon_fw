#pragma once

template<typename TGPIO, uint32_t Pin, typename TPM = typename TGPIO::tpin_mode>
struct TPin final : PinBase
{
  static_assert(IsPinMode<TPM>(), "TPM must be TPinMode");  
  constexpr TPin() = default;
  constexpr TPin(bool val) { write(val); } 
  static constexpr auto PinNum() { return Pin; }
  static constexpr auto PinMask() { return 1U << Pin; }
  using tgpio = type_unbox<decltype(TGPIO::template clone<PinMask()>())>;
  using tpm = TPM;

  static inline void set() { tgpio::set(); }
  static inline void clear() { tgpio::clear(); }
  static inline void toggle() { tgpio::toggle(); }
  static inline void write(bool data) { tgpio::write(data<<Pin); }
  template<bool data> 
  static inline void write() { if constexpr (data) tgpio::set(); else tgpio::clear(); }
  static inline bool read() { return tgpio::template read<PinMask(),TPM>(); }
  static inline void lock() { tgpio::lock(); }
  static inline void mode() { tgpio::template mode<TPM>(); }
  template<typename T>
  static inline void mode() { tgpio::template mode<T>(); }
  
  inline TPin& operator=(const TPin& pin)
  {
    write(pin.read());
    return *this;
  }

  inline TPin& operator=(bool value)
  {
    write(value);
    return *this;
  }

  inline operator bool() const { return read(); }
};

#define MAKE_16PINS(PORT_TYPE, PORT_NAME) \
template<uint32_t pin, typename TPM = PORT_TYPE<0>::tpins_mode>\
using Pin##PORT_NAME = TPin<PORT_TYPE<1U<<pin>, pin, TPM >;\
\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##0 = Pin##PORT_NAME<0,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##1 = Pin##PORT_NAME<1,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##2 = Pin##PORT_NAME<2,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##3 = Pin##PORT_NAME<3,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##4 = Pin##PORT_NAME<4,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##5 = Pin##PORT_NAME<5,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##6 = Pin##PORT_NAME<6,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##7 = Pin##PORT_NAME<7,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##8 = Pin##PORT_NAME<8,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##9 = Pin##PORT_NAME<9,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##10 = Pin##PORT_NAME<10,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##11 = Pin##PORT_NAME<11,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##12 = Pin##PORT_NAME<12,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##13 = Pin##PORT_NAME<13,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##14 = Pin##PORT_NAME<14,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##15 = Pin##PORT_NAME<15,TPM>;\
using P##PORT_NAME##_0 = P##PORT_NAME##0<>;\
using P##PORT_NAME##_1 = P##PORT_NAME##1<>;\
using P##PORT_NAME##_2 = P##PORT_NAME##2<>;\
using P##PORT_NAME##_3 = P##PORT_NAME##3<>;\
using P##PORT_NAME##_4 = P##PORT_NAME##4<>;\
using P##PORT_NAME##_5 = P##PORT_NAME##5<>;\
using P##PORT_NAME##_6 = P##PORT_NAME##6<>;\
using P##PORT_NAME##_7 = P##PORT_NAME##7<>;\
using P##PORT_NAME##_8 = P##PORT_NAME##8<>;\
using P##PORT_NAME##_9 = P##PORT_NAME##9<>;\
using P##PORT_NAME##_10 = P##PORT_NAME##10<>;\
using P##PORT_NAME##_11 = P##PORT_NAME##11<>;\
using P##PORT_NAME##_12 = P##PORT_NAME##12<>;\
using P##PORT_NAME##_13 = P##PORT_NAME##13<>;\
using P##PORT_NAME##_14 = P##PORT_NAME##14<>;\
using P##PORT_NAME##_15 = P##PORT_NAME##15<>;\

#define MAKE_32PINS(PORT_TYPE, PORT_NAME) \
template<uint32_t pin, typename TPM = PORT_TYPE<0>::tpins_mode>\
using Pin##PORT_NAME = TPin<PORT_TYPE<1UL<<pin>, pin, TPM >;\
\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##0 = Pin##PORT_NAME<0,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##1 = Pin##PORT_NAME<1,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##2 = Pin##PORT_NAME<2,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##3 = Pin##PORT_NAME<3,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##4 = Pin##PORT_NAME<4,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##5 = Pin##PORT_NAME<5,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##6 = Pin##PORT_NAME<6,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##7 = Pin##PORT_NAME<7,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##8 = Pin##PORT_NAME<8,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##9 = Pin##PORT_NAME<9,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##10 = Pin##PORT_NAME<10,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##11 = Pin##PORT_NAME<11,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##12 = Pin##PORT_NAME<12,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##13 = Pin##PORT_NAME<13,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##14 = Pin##PORT_NAME<14,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##15 = Pin##PORT_NAME<15,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##16 = Pin##PORT_NAME<16,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##17 = Pin##PORT_NAME<17,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##18 = Pin##PORT_NAME<18,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##19 = Pin##PORT_NAME<19,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##20 = Pin##PORT_NAME<20,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##21 = Pin##PORT_NAME<21,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##22 = Pin##PORT_NAME<22,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##23 = Pin##PORT_NAME<23,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##24 = Pin##PORT_NAME<24,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##25 = Pin##PORT_NAME<25,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##26 = Pin##PORT_NAME<26,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##27 = Pin##PORT_NAME<27,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##28 = Pin##PORT_NAME<28,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##29 = Pin##PORT_NAME<29,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##30 = Pin##PORT_NAME<30,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##31 = Pin##PORT_NAME<31,TPM>;\
using P##PORT_NAME##_0 = P##PORT_NAME##0<>;\
using P##PORT_NAME##_1 = P##PORT_NAME##1<>;\
using P##PORT_NAME##_2 = P##PORT_NAME##2<>;\
using P##PORT_NAME##_3 = P##PORT_NAME##3<>;\
using P##PORT_NAME##_4 = P##PORT_NAME##4<>;\
using P##PORT_NAME##_5 = P##PORT_NAME##5<>;\
using P##PORT_NAME##_6 = P##PORT_NAME##6<>;\
using P##PORT_NAME##_7 = P##PORT_NAME##7<>;\
using P##PORT_NAME##_8 = P##PORT_NAME##8<>;\
using P##PORT_NAME##_9 = P##PORT_NAME##9<>;\
using P##PORT_NAME##_10 = P##PORT_NAME##10<>;\
using P##PORT_NAME##_11 = P##PORT_NAME##11<>;\
using P##PORT_NAME##_12 = P##PORT_NAME##12<>;\
using P##PORT_NAME##_13 = P##PORT_NAME##13<>;\
using P##PORT_NAME##_14 = P##PORT_NAME##14<>;\
using P##PORT_NAME##_15 = P##PORT_NAME##15<>;\
using P##PORT_NAME##_16 = P##PORT_NAME##16<>;\
using P##PORT_NAME##_17 = P##PORT_NAME##17<>;\
using P##PORT_NAME##_18 = P##PORT_NAME##18<>;\
using P##PORT_NAME##_19 = P##PORT_NAME##19<>;\
using P##PORT_NAME##_20 = P##PORT_NAME##20<>;\
using P##PORT_NAME##_21 = P##PORT_NAME##21<>;\
using P##PORT_NAME##_22 = P##PORT_NAME##22<>;\
using P##PORT_NAME##_23 = P##PORT_NAME##23<>;\
using P##PORT_NAME##_24 = P##PORT_NAME##24<>;\
using P##PORT_NAME##_25 = P##PORT_NAME##25<>;\
using P##PORT_NAME##_26 = P##PORT_NAME##26<>;\
using P##PORT_NAME##_27 = P##PORT_NAME##27<>;\
using P##PORT_NAME##_28 = P##PORT_NAME##28<>;\
using P##PORT_NAME##_29 = P##PORT_NAME##29<>;\
using P##PORT_NAME##_30 = P##PORT_NAME##30<>;\
using P##PORT_NAME##_31 = P##PORT_NAME##31<>;\

#define MAKE_8PINS(PORT_TYPE, PORT_NAME) \
template<uint8_t pin, typename TPM = PORT_TYPE<0>::tpins_mode>\
using Pin##PORT_NAME = TPin<PORT_TYPE<1U<<pin>, pin, TPM >;\
\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##0 = Pin##PORT_NAME<0,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##1 = Pin##PORT_NAME<1,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##2 = Pin##PORT_NAME<2,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##3 = Pin##PORT_NAME<3,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##4 = Pin##PORT_NAME<4,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##5 = Pin##PORT_NAME<5,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##6 = Pin##PORT_NAME<6,TPM>;\
template<typename TPM = PORT_TYPE<0>::tpins_mode>\
using P##PORT_NAME##7 = Pin##PORT_NAME<7,TPM>;\
using P##PORT_NAME##_0 = P##PORT_NAME##0<>;\
using P##PORT_NAME##_1 = P##PORT_NAME##1<>;\
using P##PORT_NAME##_2 = P##PORT_NAME##2<>;\
using P##PORT_NAME##_3 = P##PORT_NAME##3<>;\
using P##PORT_NAME##_4 = P##PORT_NAME##4<>;\
using P##PORT_NAME##_5 = P##PORT_NAME##5<>;\
using P##PORT_NAME##_6 = P##PORT_NAME##6<>;\
using P##PORT_NAME##_7 = P##PORT_NAME##7<>;
