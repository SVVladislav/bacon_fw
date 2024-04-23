#pragma once

class GpioBase {};
class PinBase {};
class PinModeBase {};
class PinModeListBase {};
class PinListBase {};
class ConfigListBase {};
class ConfigListCMDBase {};

//--------------------------------------------------
// Концепты для ограничения типов аргументов списков
//--------------------------------------------------
template<typename T>
concept IsGpio = std::is_base_of_v<GpioBase, T>;

template<typename T>
concept IsPin = std::is_base_of_v<PinBase, T>;

template<typename T>
concept IsPinList = std::is_base_of_v<PinListBase, T>;

template<typename T>
concept IsPinListElement = IsPinList<T> || IsPin<T> || IsGpio<T>;

template<typename T>
concept IsPinMode = std::is_base_of_v<PinModeBase, T>;

template<typename T>
concept IsPinModeList = std::is_base_of_v<PinModeListBase, T>;

template<typename T>
concept IsPinModeListElement = IsPinModeList<T> || IsPinMode<T>;

template<typename T>
concept IsConfigList = std::is_base_of_v<ConfigListBase, T>;

template<typename T>
concept IsConfigListCMD = std::is_base_of_v<ConfigListCMDBase, T>;

template <typename T>
concept IsConfigListElement = IsPinListElement<T> || IsPinMode<T> || IsConfigList<T> || IsConfigListCMD<T>;

template <typename T>
concept IsSimpleListElement = IsPinMode<T> || IsPin<T> || IsConfigListCMD<T>;

