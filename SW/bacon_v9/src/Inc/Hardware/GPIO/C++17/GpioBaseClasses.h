#pragma once

class GpioBase {};
class PinBase {};
class PinModeBase {};
class PinModeListBase {};
class PinListBase {};
class ConfigListBase {};
class ConfigListCMDBase {};

template<typename T>
constexpr bool IsGpio() { return std::is_base_of_v<GpioBase, T>; }

template<typename T>
constexpr bool IsPin() { return std::is_base_of_v<PinBase, T>; }

template<typename T>
constexpr bool IsPinList() { return std::is_base_of_v<PinListBase, T>; }

template<typename T>
constexpr bool IsPinListElement() { return IsPinList<T>() || IsPin<T>() || IsGpio<T>(); }

template<typename T>
constexpr bool IsPinMode() { return std::is_base_of_v<PinModeBase, T>; }

template<typename T>
constexpr bool IsPinModeList() { return std::is_base_of_v<PinModeListBase, T>; }

template<typename T>
constexpr bool IsPinModeListElement() { return IsPinModeList<T>() || IsPinMode<T>(); }

template<typename T>
constexpr bool IsConfigList() { return std::is_base_of_v<ConfigListBase, T>; }

template<typename T>
constexpr bool IsConfigListCMD() { return std::is_base_of_v<ConfigListCMDBase, T>; }

template <typename T>
constexpr bool IsConfigListElement() { return IsPinListElement<T>() || IsPinMode<T>() || IsConfigList<T>() || IsConfigListCMD<T>(); }

