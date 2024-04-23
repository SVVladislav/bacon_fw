#pragma once

#define ADVANCED_GPIO

#include "SVVTL/template_lib.hpp"

#include <algorithm>
#include <limits>

namespace GPIO {

#if (__cplusplus > 201703L)

#include "C++20/GpioBaseClasses.hpp"
#include "C++20/Pins.hpp"
#include "C++20/PinModeList.hpp"
#include "GpioPortTemplate.hpp"
#include "Chips/chips_defs.h"
#include "C++20/PinList.hpp"
#include "C++20/ConfigList.hpp"

#else

#include "C++17/GpioBaseClasses.h"
#include "C++17/Pins.h"
#include "C++17/PinModeList.h"
#include "GpioPortTemplate.hpp"
#include "Chips/chips_defs.h"
#include "C++17/PinList.h"
#include "C++17/ConfigList.h"

#endif

}
