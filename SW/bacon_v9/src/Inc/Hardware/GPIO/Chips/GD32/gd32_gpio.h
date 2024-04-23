#pragma once

#define GPIOA_BASE (GPIO_BASE + 0x00000000U)
#define GPIOB_BASE (GPIO_BASE + 0x00000400U)
#define GPIOC_BASE (GPIO_BASE + 0x00000800U)
#define GPIOD_BASE (GPIO_BASE + 0x00000C00U)
#define GPIOE_BASE (GPIO_BASE + 0x00001000U)

#include "..\XX32\xx32_gpio.h"

namespace REMAP
{
#ifdef GD32VF103

  // TIM1 Internal trigger1 (PCF0 bits)
  enum class TIM1_ITR1_RM : uint32_t { Connect_0 = 0, USB_OTG_SOF = (1U << 29) };

  // SPI2 (PCF0 bits)
  enum class SPI2_RM : uint32_t { NSSP_PA15_SCK_PB3_MISO_PB4_MOSI_PB5 = 0U,
                                  NSSP_PA4_SCK_PC10_MISO_PC11_MOSI_PC12 = (1U<<28) };

  // JTAG Interface (PCF0 bits)
  enum class SWJ_CFG : uint32_t { JTAG_Enable_NJTRST_Enable = 0, 
                                  JTAG_Enable_NJTRST_Disable = (1U << 24),
                                  Disable = (2U << 24) };
  // CAN1 (PCFR1 bits)
  enum class CAN1_RM : uint32_t { RX_PB12_TX_PB13 = 0, RX_PB5_TX_PB6 = (1U << 22) };

  // TIM4_CH3 (PCF0 bits)
  enum class TIM4_CH4_RM : uint32_t { TIM4_CH3 = 0, IRC40K = (1U << 16) };

  // PD0,PD1 remap to OSC_IN,OSC_OUT
  enum class PD0_PD1_RM : uint32_t { PD0_PD1 = 0, OSC_IN_OUT = (1U << 15) };

  // CAN0 (PCF0 bits)
  enum class CAN0_RM : uint32_t { RX_PA11_TX_PA12 = 0,
                                  RX_PB8_TX_PB9 = (2U << 13),
                                  RX_PD0_TX_PD1 = (3U << 13) };

  // TIM3 (PCF0 bits)
  enum class TIM3_RM : uint32_t { CH0_PB6_CH1_PB7_CH2_PB8_CH3_PB9 = 0,
                                  CH0_PD12_CH1_PD13_CH2_PD14_CH3_PD15 = (1U << 12) };

  // TIM2 (PCF0 bits)
  enum class TIM2_RM : uint32_t { CH0_PA6_CH1_PA7_CH2_PB0_CH3_PB1 = 0,
                                  CH0_PB4_CH1_PB5_CH2_PB0_CH3_PB1 = (2U << 10),
                                  CH0_PC6_CH1_PC7_CH2_PC8_CH3_PC9 = (3U << 10) };

  // TIM1 (PCF0 bits)
  enum class TIM1_RM : uint32_t { CH0_ETI_PA0_CH1_PA1_CH2_PA2_CH3_PA3 = 0,
                                  CH0_ETI_PA15_CH1_PB3_CH2_PA2_CH3_PA3 = (1U << 8),
                                  CH0_ETI_PA0_CH1_PA1_CH2_PB10_CH3_PB11 = (2U << 8),
                                  CH0_ETI_PA15_CH1_PB3_CH2_PB10_CH3_PB11 = (3U << 8), };
  // TIM0 (PCF0 bits)
  enum class TIM0_RM : uint32_t { ETI_PA12_CH0_PA8_CH1_PA9_CH2_PA10_CH3_PA11_BKIN_PB12_CH0N_PB13_CH1N_PB14_CH2N_PB15 = 0,
                                  ETI_PA12_CH0_PA8_CH1_PA9_CH2_PA10_CH3_PA11_BKIN_PA6_CH0N_PA7_CH1N_PB0_CH2N_PB1 = (1U << 6),
                                  ETI_PE7_CH0_PE9_CH1_PE11_CH2_PE13_CH3_PE14_BKIN_PE15_CH0N_PE8_CH1N_PE10_CH2N_PE12 = (3U << 6) };

  // USART2 (PCF0)
  enum class USART2_RM : uint32_t { CK_PB12_TX_PB10_RX_PB11_CTS_PB13_RTS_PB14 = 0,
                                    CK_PC12_TX_PC10_RX_PC11_CTS_PB13_RTS_PB14 = (1U<<4),
                                    CK_PD10_TX_PD8_RX_PD9_CTS_PD11_RTS_PD12 = (3U << 4) };
  // USART1 (PCF0 bits)
  enum class USART1_RM : uint32_t { CK_PA4_TX_PA2_RX_PA3_CTS_PA0_RTS_PA1 = 0,
                                    CK_PD7_TX_PD5_RX_PD6_CTS_PD3_RTS_PD4 = (1U << 3) };

  // USART0 (PCF0 & PCFR2 bits)
  enum class USART0_RM : uint32_t { TX_PA9_RX_PA10 = 0U,
                                    TX_PB6_RX_PB7 = (1U<<2) };

  // I2C0 (PCF0 bits)
  enum class I2C0_RM : uint32_t { SCL_PB6_SDA_PB7 = 0,
                                  SCL_PB8_SDA_PB9 = (1U<<1) };

  // SPI0 (PCF0 bits)
  enum class SPI0_RM : uint32_t { NSS_PA4_SCK_PA5_MISO_PA6_MOSI_PA7 = 0,
                                  NSS_PA15_SCK_PB3_MISO_PB4_MOSI_PB5 = 1U };

  // EXMC_NADV (PCF1 bits)
  enum class EXMC_NADV : uint32_t { PB7 = 0, Disable = (1U << 31) + (1U << 10) };

  template <auto... rm>
  void Remap()
  {
    // Filter PCF0
    constexpr auto pcf0 = (((uint32_t(rm) & (1U << 31)) == 0 ? uint32_t(rm) : 0) | ... | 0);
    // Filter PCF1
    constexpr auto pcf1 = (((uint32_t(rm) & (1U << 31)) == (1 << 31) ? (uint32_t(rm) & 0x7FFF'FFFF) : 0) | ... | 0);

    if constexpr (pcf0 != 0) AFIO->PCF0 = pcf0;
    if constexpr (pcf0 != 0) AFIO->PCF1 = pcf1;
  }


#endif
} // namespace REMAP
