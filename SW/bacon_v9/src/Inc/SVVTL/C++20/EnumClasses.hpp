#pragma once

//----------------------------------------------
// Операторы для работы с enum class
//----------------------------------------------
template <typename E> requires std::is_enum_v<E>
consteval bool hasFlag(E e1, E e2)
{
  using T = std::underlying_type_t<E>;
  return static_cast<bool>(static_cast<T>(e1) & static_cast<T>(e2));
}

template <typename E> requires std::is_enum_v<E>
consteval E operator|(E lhs, E rhs)
{
  using T = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

template <typename E> requires std::is_enum_v<E>
  consteval E operator&(E lhs, E rhs)
{
  using T = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

template <typename E> requires std::is_enum_v<E>
consteval E operator^(E lhs, E rhs)
{
  using T = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<T>(lhs) ^ static_cast<T>(rhs));
}

template <typename E> requires std::is_enum_v<E>
consteval E operator~(E lhs)
{
  using T = std::underlying_type_t<E>;
  return static_cast<E>(~static_cast<T>(lhs));
}

template <typename E> requires std::is_enum_v<E>
consteval E &operator|=(E &lhs, E rhs)
{
  using T = std::underlying_type_t<E>;
  lhs = static_cast<E>(static_cast<T>(lhs) | static_cast<T>(rhs));
  return lhs;
}

template <typename E> requires std::is_enum_v<E>
consteval E &operator&=(E &lhs, E rhs)
{
  using T = std::underlying_type_t<E>;
  lhs = static_cast<E>(static_cast<T>(lhs) & static_cast<T>(rhs));
  return lhs;
}

template <typename E> requires std::is_enum_v<E>
consteval E &operator^=(E &lhs, E rhs)
{
  using T = std::underlying_type_t<E>;
  lhs = static_cast<E>(static_cast<T>(lhs) ^ static_cast<T>(rhs));
  return lhs;
}