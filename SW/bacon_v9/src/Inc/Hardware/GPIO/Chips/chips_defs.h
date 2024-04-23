#pragma once

#if defined(STM32F030) || defined(STM32F070)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0x0C00'0000, 0x0C00'0000, 0x2400'0000
#define PWR_CFG_B 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_C 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_F 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_G 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F0_pinmodes.h"
#define STM32_GPIO_V2

#elif defined(STM32F031) || defined(STM32F051) || defined(STM32F071) || defined(STM32F091) || \
      defined(STM32F042) || defined(STM32F072) || \
      defined(STM32F038) || defined(STM32F048) || defined(STM32F058) || defined(STM32F078) || defined(STM32F098)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0x2800'0000, 0x0C00'0000, 0x2400'0000
#define PWR_CFG_B 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_C 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_F 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_G 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F0_pinmodes.h"
#define STM32_GPIO_V2

#elif defined(STM32F303) || defined(STM32F328) || defined(STM32F358) || defined(STM32F398)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0xA800'0000, 0x0C00'0000, 0x6400'0000
#define PWR_CFG_B 0x0000'0280, 0x0000'00C0, 0x0000'0100
#define PWR_CFG_C 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_F 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_G 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_I 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_J 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_K 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F0_pinmodes.h"
#define STM32_GPIO_V2
#define __RBIT_ENABLE

#elif defined(STM32F401)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0x0C00'0000, 0x0C00'0000, 0x6400'0000
#define PWR_CFG_B 0x0000'0280, 0x0000'00C0, 0x0000'0100
#define PWR_CFG_C 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_F 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_G 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F4_pinmodes.h"
#define STM32_GPIO_V2
#define __RBIT_ENABLE

#elif defined(STM32F411) || defined(STM32F412) || \
      defined(STM32F405) || defined(STM32F415) || defined(STM32F429) || defined(STM32F439) || \
      defined(STM32F407) || defined(STM32F417) || defined(STM32F427) || defined(STM32F437)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0xA800'0000, 0x0C00'0000, 0x6400'0000
#define PWR_CFG_B 0x0000'0280, 0x0000'00C0, 0x0000'0100
#define PWR_CFG_C 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_F 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_G 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_I 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_J 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_K 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F4_pinmodes.h"
#define STM32_GPIO_V2
#define __RBIT_ENABLE

#elif defined(STM32L011) || defined(STM32L021) || defined(STM32L031) || defined(STM32L041) || defined(STM32L051) || defined(STM32L071) || defined(STM32L081) || \
      defined(STM32L053) || defined(STM32L063) || defined(STM32L073) || defined(STM32L083) || \
      defined(STM32G031) || defined(STM32G041) || defined(STM32G071) || defined(STM32G081)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0xEBFF'FCFF, 0x0C00'0000, 0x2400'0000
#define PWR_CFG_B 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_C 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_F 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_G 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F4_pinmodes.h"
#define STM32_GPIO_V2


#elif defined(STM32G431) || defined(STM32G441) || defined(STM32G473) || defined(STM32G474) || defined(STM32G483) || defined(STM32G484)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0xABFF'FFFF, 0x0C00'0000, 0x6400'0000
#define PWR_CFG_B 0xFFFF'FEBF, 0x0000'0000, 0x0000'0100
#define PWR_CFG_C 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_F 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_G 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F4_pinmodes.h"
#define STM32_GPIO_V2
#define __RBIT_ENABLE

#elif defined(STM32H745) || defined(STM32H747) || defined(STM32H755) || defined(STM32H757) || defined(STM32H750)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0xABFF'FFFF, 0x0C00'0000, 0x6400'0000
#define PWR_CFG_B 0xFFFF'FEBF, 0x0000'00C0, 0x0000'0100
#define PWR_CFG_C 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_F 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_G 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_I 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_J 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_K 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F4_pinmodes.h"
#define STM32_GPIO_V2
#define __RBIT_ENABLE
 
#elif defined(STM32F777) || defined(STM32F779) || defined(STM32F765) || defined(STM32F767) || defined(STM32F769)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0xAB00'0000, 0x0C00'0000, 0x6400'0000
#define PWR_CFG_B 0x0000'0280, 0x0000'00C0, 0x0000'0100
#define PWR_CFG_C 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_F 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_G 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_I 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_J 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PWR_CFG_K 0x0000'0000, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F4_pinmodes.h"
#define STM32_GPIO_V2
#define __RBIT_ENABLE        
        
#elif defined(STM32WB55) || defined(STM32WB35)
//                    MODER      OSPEEDR        PUPDR
#define PWR_CFG_A 0xABFF'FFFF, 0x0C00'0000, 0x6400'0000
#define PWR_CFG_B 0xFFFF'FEBF, 0x0000'00C0, 0x0000'0100
#define PWR_CFG_C 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_D 0xFFFF'FFFF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_E 0x0000'03FF, 0x0000'0000, 0x0000'0000
#define PWR_CFG_H 0x0000'00CF, 0x0000'0000, 0x0000'0000
#define PIN_MODES_FILE "STM32/stm32F4_pinmodes.h"
#define STM32_GPIO_V2
#define __RBIT_ENABLE

#elif defined(STM32F101) || defined(STM32F102) || defined(STM32F103) || defined(STM32F105) || defined(STM32F107)
//                    CRL        CRH
#define PWR_CFG_A 0x8884'4444, 0x4444'4444
#define PWR_CFG_B 0x4444'4444, 0x4444'4444
#define PWR_CFG_C 0x4444'4444, 0x4444'4444
#define PWR_CFG_D 0x4444'4444, 0x4444'4444
#define PWR_CFG_E 0x4444'4444, 0x4444'4444
#define PWR_CFG_F 0x4444'4444, 0x4444'4444
#define PWR_CFG_G 0x4444'4444, 0x4444'4444
#define PWR_CFG_H 0x4444'4444, 0x4444'4444
#define PIN_MODES_FILE "XX32/XX32_pinmodes.h"
#define STM32_GPIO_V1
#define __RBIT_ENABLE

#elif defined(MDR1986VE8)
#define PIN_MODES_FILE "MDR1986/mdr1986_pinmodes.h"
#define MDR_GPIO_V1
#define __RBIT_ENABLE
   
#elif defined(MICROBLAZE_SVV)
#define PIN_MODES_FILE "Microblaze_SVV/mbz_svv_gpio_pinmodes.h"
#define MBZ_SVV_GPIO_AXI

#elif defined(__AVR_ARCH__) && ((__AVR_DEVICE_NAME__==atmega8) || (__AVR_DEVICE_NAME__==atmega8a))
#define GPIOB_BASE (0x16 + 0x20)
#define GPIOC_BASE (0x13 + 0x20)
#define GPIOD_BASE (0x10 + 0x20)
#define PIN_TOGGLE false
#define PIN_MODES_FILE "AVR/avr_gpio_pinmodes.h"
#define AVR_GPIO_V1

#elif defined(CH32V307)
//                    CFGHR        CFGLR
#define PWR_CFG_A 0x4884'4444, 0x4444'4444
#define PWR_CFG_B 0x4444'4444, 0x4444'4444
#define PWR_CFG_C 0x4444'4444, 0x4444'4444
#define PWR_CFG_D 0x4444'4444, 0x4444'4444
#define PWR_CFG_E 0x4444'4444, 0x4444'4444
#define PWR_CFG_F 0x4444'4444, 0x4444'4444
#define PWR_CFG_G 0x4444'4444, 0x4444'4444
#define PWR_CFG_H 0x4444'4444, 0x4444'4444
#define PIN_MODES_FILE "XX32/xx32_pinmodes.h"
#define CH32_GPIO_V1

#elif defined(CH32V003)
//                                 
#define PWR_CFG_A 0x4444'4444, 0x4444'4444
#define PWR_CFG_C 0x4444'4444, 0x4444'4444
#define PWR_CFG_D 0x4444'4444, 0x4444'4444
#define PIN_MODES_FILE "XX32/xx32_pinmodes.h"
#define CH32_GPIO_V1

#elif defined(GD32VF103)
//                                 
#define PWR_CFG_A 0x4884'4444, 0x4444'4444
#define PWR_CFG_B 0x4444'4444, 0x4444'4444
#define PWR_CFG_C 0x4444'4444, 0x4444'4444
#define PWR_CFG_D 0x4444'4444, 0x4444'4444
#define PWR_CFG_E 0x4444'4444, 0x4444'4444
#define PWR_CFG_F 0x4444'4444, 0x4444'4444
#define PWR_CFG_G 0x4444'4444, 0x4444'4444
#define PWR_CFG_H 0x4444'4444, 0x4444'4444
#define PIN_MODES_FILE "XX32/xx32_pinmodes.h"
#define GD32_GPIO_V1

#else
  static_assert(false,"MCU and PinModes not defined");
#endif


//------------------------------------------------------------------------------
// Errata
//------------------------------------------------------------------------------
#if defined(STM32L053) || defined(STM32L051)
#define STM32L0_ERRATA
#endif


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#include PIN_MODES_FILE

#ifdef STM32_GPIO_V1
  #include "STM32F1/stm32_gpio_v1.h"
  namespace PinMode = XX32_GPIO::XX32_MODE;
  namespace TGPIO = XX32_GPIO;
  
#elif defined(STM32_GPIO_V2)
  #include "STM32/stm32_gpio_v2.h"
  namespace PinMode = STM32Fx_GPIO::STM32Fx_MODE;
  namespace TGPIO = STM32Fx_GPIO;

#elif defined(MDR_GPIO_V1)
  #include "MDR1986/mdr_gpio_v1.h"  
  namespace PinMode = MDR1986_MODE;
  namespace TGPIO = MDR1896_GPIO;
  
#elif defined(MBZ_SVV_GPIO_AXI)
  #include "Microblaze_SVV/mbz_svv_gpio.h"  
  namespace PinMode = MBZ_SVV_GPIO_MODE;
  namespace TGPIO = MBZ_SVV_GPIO;

#elif defined(AVR_GPIO_V1)
  #include "AVR/avr_gpio.h"  
  namespace PinMode = AVR_GPIO_MODE;
  namespace TGPIO = AVR_GPIO;

#elif defined(CH32_GPIO_V1)
  #include "CH32/ch32_gpio.h"
  namespace PinMode = XX32_GPIO::XX32_MODE;
  namespace TGPIO = XX32_GPIO;

#elif defined(GD32_GPIO_V1)
#include "GD32/gd32_gpio.h"
  namespace PinMode = XX32_GPIO::XX32_MODE;
  namespace TGPIO = XX32_GPIO;

#else
  static_assert(false,"Define Chip Type");
#endif

using namespace TGPIO;

#include "Dummy/dummy_gpio.h"
using namespace DUMMY_GPIO;
using namespace DUMMY_MODE;
