#pragma once

#include "..\XX32\xx32_gpio.h"

#define STM32F103

namespace REMAP
{
  // SWD and JTAG configuration
  enum class SWJCFG : uint32_t { SWD_Enable_JTAG_Enable_nJTRST_Enable = 0, 
                                 SWD_Enable_JTAG_Enable_nJTRST_Disable = (1U << 24),
                                 SWD_Enable_JTAG_Disable_nJTRST_Disable = (2U << 24),
                                 SWD_Disable_JTAG_Disable_nJTRST_Disable = (4U << 24) };

  // TIM5_CH4 (MPAPR bits)
  enum class TIM5_CH4_RM : uint32_t { TIM5_CH4 = 0, LSI = (1U << 16) };

  // PD0,PD1 remap to OSC_IN,OSC_OUT
  enum class PD0_PD1_RM : uint32_t { PD0_PD1 = 0, OSC_IN_OUT = (1U << 15) };

  // TIM4 (MPAPR bits)
  enum class TIM4_RM : uint32_t { CH1_PB6_CH2_PB7_CH3_PB8_CH4_PB9 = 0,
                                  CH1_PD12_CH2_PD13_CH3_PD14_CH4_PD15 = (1U << 12) };

  // TIM3 (MPAPR bits)
  enum class TIM3_RM : uint32_t { CH1_PA6_CH2_PA7_CH3_PB0_CH4_PB1 = 0,
                                  CH1_PB4_CH2_PB5_CH3_PB0_CH4_PB1 = (2U << 10),
                                  CH1_PC6_CH2_PC7_CH3_PC8_CH4_PC9 = (3U << 10) };

  // TIM2 (MPAPR bits)
  enum class TIM2_RM : uint32_t { CH1_ETR_PA0_CH2_PA1_CH3_PA2_CH4_PA3 = 0,
                                  CH1_ETR_PA15_CH2_PB3_CH3_PA2_CH4_PA3 = (1U << 8),
                                  CH1_ETR_PA0_CH2_PA1_CH3_PB10_CH4_PB11 = (2U << 8),
                                  CH1_ETR_PA15_CH2_PB3_CH3_PB10_CH4_PB11 = (3U << 8), };
  // TIM1 (MPAPR bits)
  enum class TIM1_RM : uint32_t { ETR_PA12_CH1_PA8_CH2_PA9_CH3_PA10_CH4_PA11_BKIN_PB12_CH1N_PB13_CH2N_PB14_CH3N_PB15 = 0,
                                  ETR_PA12_CH1_PA8_CH2_PA9_CH3_PA10_CH4_PA11_BKIN_PA6_CH1N_PA7_CH2N_PB0_CH3N_PB1 = (1U << 6),
                                  ETR_PE7_CH1_PE9_CH2_PE11_CH3_PE13_CH4_PE14_BKIN_PE15_CH1N_PE8_CH2N_PE10_CH3N_PE12 = (3U << 6) };

  // USART3 (MPAPR)
  enum class USART3_RM : uint32_t { CK_PB12_TX_PB10_RX_PB11_CTS_PB13_RTS_PB14 = 0,
                                    CK_PC12_TX_PC10_RX_PC11_CTS_PB13_RTS_PB14 = (1U<<4),
                                    CK_PD10_TX_PD8_RX_PD9_CTS_PD11_RTS_PD12 = (3U << 4) };
  // USART2 (MPAPR bits)
  enum class USART2_RM : uint32_t { CK_PA4_TX_PA2_RX_PA3_CTS_PA0_RTS_PA1 = 0,
                                    CK_PD7_TX_PD5_RX_PD6_CTS_PD3_RTS_PD4 = (1U << 3) };
  // USART1 (MPAPR bits)
  enum class USART1_RM : uint32_t { TX_PA9_RX_PA10 = 0,
                                    TX_PB6_RX_PB7 = (1U << 2) };
  // I2C1 (MPAPR bits)
  enum class I2C1_RM : uint32_t { SCL_PB6_SDA_PB7 = 0,
                                  SCL_PB8_SDA_PB9 = (1U<<1) };

  // SPI1 (MPAPR bits)
  enum class SPI1_RM : uint32_t { NSS_PA4_SCK_PA5_MISO_PA6_MOSI_PA7 = 0,
                                  NSS_PA15_SCK_PB3_MISO_PB4_MOSI_PB5 = 1U };

  // FSMC_NADV (MPAPR2 bits)
  enum class FSMC_NADV : uint32_t { PB7 = 0, Disable = (1U << 31) + (1U << 10) };

#ifdef TIM14
  // TIM14_CH1 (MPAPR2 bits)
  enum class TIM14_CH1_RM : uint32_t { PA7 = 0, PF9 = (1U << 31) + (1U << 9) };
#endif

#ifdef TIM13
  // TIM13_CH1 (MPAPR2 bits)
  enum class TIM13_CH1_RM : uint32_t { PA6 = 0, PF8 = (1U << 31) + (1U << 8) };
#endif

#ifdef TIM11
  // TIM11_CH1  (MPAPR2 bits)
  enum class TIM13_CH1_RM : uint32_t { PB9 = 0, PF7 = (1U << 31) + (1U << 7) };
#endif

#ifdef TIM10
  // TIM10_CH1 (MPAPR2 bits)
  enum class TIM10_CH1_RM : uint32_t  { PB8 = 0, PF6 = (1U << 31) + (1U << 6) };
#endif

#ifdef TIM9
  // TIM9 (MPAPR2 bits)
  enum class TIM9_RM : uint32_t  { CH1_PA2_CH2_PA3 = (1U << 31) + 0U,
                                   CH1_PE5_CH2_PE6 = (1U << 31) + (1U << 5) };
#endif

#if defined(STM32F107) || defined(STM32F105)

  // Ethernet PTP PPS (MPAPR bits)
  enum class PTP_PPSP_RM : uint32_t { Disable = 0, PB5 = (1U << 30) };

  // TIM2 Internal trigger1 (MPAPR bits)
  enum class TIM2_ITR1_RM : uint32_t { Ethernet_PTP = 0, USB_OTG_SOF = (1U << 29) };

  // SPI3 (MPAPR bits)
  enum class SPI3_RM : uint32_t { NSSP_PA15_SCK_PB3_MISO_PB4_MOSI_PB5 = 0U,
                                  NSSP_PA4_SCK_PC10_MISO_PC11_MOSI_PC12 = (1U<<28) };

  // MII or RMII select (MPAPR bits)
  enum class MII_RMII_SEL : uint32_t { MII_PHY = 0, RMII_PHY = (1U << 23) };

  // CAN2 (MPAPR bits)
  enum class CAN2_RM : uint32_t { RX_PB12_TX_PB13 = 0, RX_PB5_TX_PB6 = (1U << 22) };

  // ETH (MPAPR bits)
  enum class ETH_RM : uint32_t { RX_DV_PA7_RXD0_PC4_RXD1_PC5_RXD2_PB0_RXD3_PB1 = 0,
                                 RX_DV_PD8_RXD0_PD9_RXD1_PD10_RXD2_PD11_RXD3_PD12 = (1U << 21) };

  // CAN1 (MPAPR bits)
  enum class CAN1_RM : uint32_t { RX_PA11_TX_PA12 = 0,
                                  RX_PB8_TX_PB9 = (2U << 13),
                                  RX_PD0_TX_PD1 = (3U << 13) };

#endif

#if defined(STM32F101) || defined(STM32F102) || defined(STM32F103)  

  // ADC2 External trigger regular conversion (MPAPR bits)
  enum class ADC2_ETRG_REG : uint32_t { EXTI11 = 0, TIM8_TRGO = (1U << 20) };

  // ADC2 External trigger injection conversion (MPAPR bits)
  enum class ADC2_ETRG_INJ : uint32_t { EXTI15 = 0, TIM8_CH4 = (1U << 19) };

  // ADC1 External trigger regular conversion (MPAPR bits)
  enum class ADC1_ETRG_REG : uint32_t { EXTI11 = 0, TIM8_TRGO = (1U << 18) };

  // ADC1 External trigger injection conversion (MPAPR bits)
  enum class ADC1_ETRG_INJ : uint32_t { EXTI15 = 0, TIM8_CH4 = (1U << 17) };

  // CAN (MPAPR bits)
  enum class CAN_RM : uint32_t { RX_PA11_TX_PA12 = 0,
                                 RX_PB8_TX_PB9 = (2U << 13),
                                 RX_PD0_TX_PD1 = (3U << 13) };


#endif

  template <auto... rm>
  void Remap()
  {
    // Filter MAPR
    constexpr auto mapr = (((uint32_t(rm) & (1U << 31)) == 0 ? uint32_t(rm) : 0) | ... | 0);
    // Filter MAPR2
    constexpr auto mapr2 = (((uint32_t(rm) & (1U << 31)) == (1U << 31) ? (uint32_t(rm) & 0x7FFF'FFFF) : 0) | ... | 0);

    if constexpr (mapr != 0) AFIO->MAPR = mapr;
    if constexpr (mapr2 != 0) AFIO->MAPR2 = mapr2;
  }



} // namespace REMAP

#undef STM32F103