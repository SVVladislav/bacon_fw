#pragma once

#include "..\XX32\xx32_gpio.h"

namespace REMAP
{

#if defined(CH32V307)

  // Ethernet PTP PPS (PCFR1 bits)
  enum class PTP_PPSP_RM : uint32_t{ Disable = 0, PB5 = (1U<<30) };

  // TIM2 Internal trigger1 (PCFR1 bits)
  enum class TIM2_ITR1_RM : uint32_t { Ethernet_PTP = 0, USB_OTG_SOF = (1U<<29) };

  // SPI3 (PCFR1 bits)
  enum class SPI3_RM : uint32_t { NSSP_PA15_SCK_PB3_MISO_PB4_MOSI_PB5 = 0U,
                                  NSSP_PA4_SCK_PC10_MISO_PC11_MOSI_PC12 = (1U<<28) };

  // SWD Interface (PCFR1 bits)
  enum class SWCFG : uint32_t { Enable = 0, Disable = (4U<<24) };

  // MII or RMII select (PCFR1 bits)
  enum class MII_RMII_SEL : uint32_t { MII_PHY = 0, RMII_PHY = (1U<<23) };
  
  // CAN2 (PCFR1 bits)
  enum class CAN2_RM : uint32_t { RX_PB12_TX_PB13=0, RX_PB5_TX_PB6 = (1U<<22) };

  // ETH (PCFR1 bits)
  enum class ETH_RM : uint32_t { RX_DV_PA7_RXD0_PC4_RXD1_PC5_RXD2_PB0_RXD3_PB1=0,
                                 RX_DV_PD8_RXD0_PD9_RXD1_PD10_RXD2_PD11_RXD3_PD12 = (1U<<21) };  

  // ADC2 External trigger regular conversion (PCFR1 bits)
  enum class ADC2_ETRG_REG : uint32_t { EXTI11 = 0, TIM8_TRGO = (1U << 20) };

  // ADC2 External trigger injection conversion (PCFR1 bits)
  enum class ADC2_ETRG_INJ : uint32_t { EXTI15 = 0, TIM8_CH4 = (1U << 19) };

  // ADC1 External trigger regular conversion (PCFR1 bits)
  enum class ADC1_ETRG_REG : uint32_t { EXTI11 = 0, TIM8_TRGO = (1U << 18) };

  // ADC1 External trigger injection conversion (PCFR1 bits)
  enum class ADC1_ETRG_INJ : uint32_t { EXTI15 = 0, TIM8_CH4 = (1U << 17) };

  // TIM5_CH4 (PCFR1 bits)
  enum class TIM5_CH4_RM : uint32_t { TIM5_CH4 = 0, LSI = (1U << 16) };

  // PD0,PD1 remap to OSC_IN,OSC_OUT
  enum class PD0_PD1_RM : uint32_t { PD0_PD1 = 0, OSC_IN_OUT = (1U << 15) };

  // CAN1 (PCFR1 bits)
  enum class CAN1_RM : uint32_t { RX_PA11_TX_PA12 = 0,
                                  RX_PB8_TX_PB9 = (2U << 13),
                                  RX_PD0_TX_PD1 = (3U << 13) };
  // TIM4 (PCFR1 bits)
  enum class TIM4_RM : uint32_t { CH1_PB6_CH2_PB7_CH3_PB8_CH4_PB9 = 0,
                                  CH1_PD12_CH2_PD13_CH3_PD14_CH4_PD15 = (1U << 12) };

  // TIM3 (PCFR1 bits)
  enum class TIM3_RM : uint32_t { CH1_PA6_CH2_PA7_CH3_PB0_CH4_PB1 = 0,
                                  CH1_PB4_CH2_PB5_CH3_PB0_CH4_PB1 = (2U << 10),
                                  CH1_PC6_CH2_PC7_CH3_PC8_CH4_PC9 = (3U << 10) };

  // TIM2 (PCFR1 bits)
  enum class TIM2_RM : uint32_t { CH1_ETR_PA0_CH2_PA1_CH3_PA2_CH4_PA3 = 0,
                                  CH1_ETR_PA15_CH2_PB3_CH3_PA2_CH4_PA3 = (1U << 8),
                                  CH1_ETR_PA0_CH2_PA1_CH3_PB10_CH4_PB11 = (2U << 8),
                                  CH1_ETR_PA15_CH2_PB3_CH3_PB10_CH4_PB11 = (3U << 8), };
  // TIM1 (PCFR1 bits)
  enum class TIM1_RM : uint32_t { ETR_PA12_CH1_PA8_CH2_PA9_CH3_PA10_CH4_PA11_BKIN_PB12_CH1N_PB13_CH2N_PB14_CH3N_PB15 = 0,
                                  ETR_PA12_CH1_PA8_CH2_PA9_CH3_PA10_CH4_PA11_BKIN_PA6_CH1N_PA7_CH2N_PB0_CH3N_PB1 = (1U << 6),
                                  ETR_PE7_CH1_PE9_CH2_PE11_CH3_PE13_CH4_PE14_BKIN_PE15_CH1N_PE8_CH2N_PE10_CH3N_PE12 = (3U << 6) };
  // USART3 (PCFR1)
  enum class USART3_RM : uint32_t { CK_PB12_TX_PB10_RX_PB11_CTS_PB13_RTS_PB14 = 0,
                                    CK_PC12_TX_PC10_RX_PC11_CTS_PB13_RTS_PB14 = (1U<<4),
                                    CK_PD10_TX_PD8_RX_PD9_CTS_PD11_RTS_PD12 = (3U << 4) };
  // USART2 (PCFR1 bits)
  enum class USART2_RM : uint32_t { CK_PA4_TX_PA2_RX_PA3_CTS_PA0_RTS_PA1 = 0,
                                    CK_PD7_TX_PD5_RX_PD6_CTS_PD3_RTS_PD4 = (1U << 3) };

  // I2C1 (PCFR1 bits)
  enum class I2C1_RM : uint32_t { SCL_PB6_SDA_PB7 = 0,
                                  SCL_PB8_SDA_PB9 = (1U<<1) };

  // SPI1 (PCFR1 bits)
  enum class SPI1_RM : uint32_t { NSS_PA4_SCK_PA5_MISO_PA6_MOSI_PA7 = 0,
                                  NSS_PA15_SCK_PB3_MISO_PB4_MOSI_PB5 = 1U };

  // UART8 (PCFR2 bits)
  enum class UART8_RM : uint32_t  { TX_PC4_RX_PC5 = (1U << 31) + 0U,
                                    TX_PA14_RX_PA15 = (1U << 31) + (1U << 24),
                                    TX_PE14_RX_PE15 = (1U << 31) + (2U << 24) };
  // UART7 (PCFR2 bits)
  enum class UART7_RM : uint32_t  { TX_PC2_RX_PC3 = (1U << 31) + 0U,
                                    TX_PA6_RX_PA7 = (1U << 31) + (1U << 22),
                                    TX_PE12_RX_PE13 = (1U << 31) + (2U << 22) };
  // UART6 (PCFR2 bits)
  enum class UART6_RM : uint32_t  { TX_PC0_RX_PC1 = (1U << 31) + 0U,
                                    TX_PB8_RX_PB9 = (1U << 31) + (1U << 20),
                                    TX_PE10_RX_PE11 = (1U << 31) + (2U << 20) };
  // UART5 (PCFR2 bits)
  enum class UART5_RM : uint32_t  { TX_PC12_RX_PD2 = (1U << 31) + 0U,
                                    TX_PB4_RX_PB5 = (1U << 31) + (1U << 18),
                                    TX_PE8_RX_PE9 = (1U << 31) + (2U << 18) };
  // UART4 (PCFR2 bits)
  enum class UART4_RM : uint32_t  { TX_PC10_RX_PC11 = (1U << 31) + 0U,
                                    TX_PB0_RX_PB1 = (1U << 31) + (1U << 16),
                                    TX_PE0_RX_PE1 = (1U << 31) + (2U << 16) };
  // FSMC_NADV (PCFR2 bits)
  enum class FSMC_NADV : uint32_t { PB7 = 0, Disable = (1U << 31) + (1U << 10) };

  // TIM10 (PCFR2 bits)
  enum class TIM10_RM : uint32_t  { ETR_PC10_CH1_PB8_CH2_PB9_CH3_PC3_CH4_PC11_BKIN_PC12_CH1N_PA12_CH2N_PA13_CH3N_PA14 = (1U << 31) + 0U,
                                    ETR_PB11_CH1_PB3_CH2_PB4_CH3_PB5_CH4_PC15_BKIN_PB10_CH1N_PA5_CH2N_PA6_CH3N_PA7 = (1U << 31) + (1U << 5),
                                    ETR_PD0_CH1_PD1_CH2_PD3_CH3_PD5_CH4_PD7_BKIN_PE2_CH1N_PE3_CH2N_PE4_CH3N_PE5 = (1U << 31) + (2U << 5) };

  // TIM9 (PCFR2 bits)
  enum class TIM9_RM : uint32_t  { ETR_PA2_CH1_PA2_CH2_PA3_CH3_PA4_CH4_PC4_BKIN_PC5_CH1N_PC0_CH2N_PC1_CH3N_PC2 = (1U << 31) + 0U,
                                   ETR_PA2_CH1_PA2_CH2_PA3_CH3_PA4_CH4_PC14_BKIN_PA1_CH1N_PB0_CH2N_PB1_CH3N_PB2 = (1U << 31) + (1U << 3),
                                   ETR_PD9_CH1_PD9_CH2_PD11_CH3_PD13_CH4_PD15_BKIN_PD14_CH1N_PD8_CH2N_PD10_CH3N_PD12 = (1U << 31) + (2U << 3) };

  // TIM8  (PCFR2 bits)
  enum class TIM8_RM : uint32_t  { ETR_PA0_CH1_PC6_CH2_PC7_CH3_PC8_CH4_PC9_BKIN_PA6_CH1N_PA7_CH2N_PB0_CH3N_PB1 = (1U << 31) + 0U,                                    
                                   ETR_PA0_CH1_PB6_CH2_PB7_CH3_PB8_CH4_PC13_BKIN_PB9_CH1N_PA13_CH2N_PA14_CH3N_PA15 = (1U << 31) + (1U << 2) };

  // USART1 (PCFR1 & PCFR2 bits)
  enum class USART1_RM : uint32_t { CK_PA8_TX_PA9_RX_PA10_CTS_PA11_RTS_PA12 = (3U << 30) + 0U,
                                    CK_PA8_TX_PB6_RX_PB7_CTS_PA11_RTS_PA12 = (3U << 30) + 1U,
                                    CK_PA10_TX_PB15_RX_PA8_CTS_PA5_RTS_PA9 = (3U << 30) + 2U,
                                    CK_PA5_TX_PA6_RX_PA7_CTS_PC4_RTS_PC5 = (3U << 30) + 3U };

  template <auto... rm>
  void Remap()
  {
    // Filter USART1_RM 
    constexpr auto usart1_rm = (((uint32_t(rm) & (3U << 30)) == (3U << 30) ? uint32_t(rm) & 0x3 : 0) | ... | 0);
    // Filter PCFR1
    constexpr auto pcfr1 = ( ( (uint32_t(rm) & (1U << 31)) == 0 ? uint32_t(rm) : 0 ) | ... | 0) | ((usart1_rm&1)<<2);
    // Filter PCFR2
    constexpr auto pcfr2 = (((uint32_t(rm) & (3U << 30)) == (2<<30) ? (uint32_t(rm)&0x3FFF'FFFF) : 0) | ... | 0) | ((usart1_rm & 2) << 25);

    if constexpr (pcfr1 != 0) AFIO->PCFR1 = pcfr1;
    if constexpr (pcfr2 != 0) AFIO->PCFR2 = pcfr2;
  }

#elif defined(CH32V003)

  // SWD Interface (PCFR1 bits)
  enum class SWCFG : uint32_t { Enable = 0, Disable = (4U<<24) };

  // TIM1 (PCFR1 bits)
  enum class TIM1_IREMAP : uint32_t { External = 0, LSI = (1U << 23) };

  // I2C1 (PCFR1 bits)
  enum class I2C1_RM : uint32_t { SCL_PC2_SDA_PC1 = 0, SCL_PD1_SDA_PD06 = (1U << 1),
                                  SCL_PC5_SDA_PC6 = (1U << 1) | (1U << 22) };

  // USART1 (PCFR1 bits)
  enum class USART1_RM : uint32_t { CK_PD4_TX_PD5_RX_PD6_CTS_PD3_RTS_PC2 = 0,
                                    CK_PD7_TX_PD0_RX_PD1_CTS_PC3_RTS_PC2_SWRX_PD0 = (1U << 2),
                                    CK_PD7_TX_PD6_RX_PD5_CTS_PC6_RTS_PC7_SWRX_PD6 = (1U << 21),
                                    CK_PC5_TX_PC0_RX_PC1_CTS_PC6_RTS_PC7_SWRX_PC0 = (1U << 2) | (1U << 21) };

  // ADC_ETRGREG (PCFR1 bits)
  enum class ADC_ETRGREG_RM : uint32_t { PD3 = 0, PC2 = (1U << 18) };

  // ADC_ETRGINJ (PCFR1 bits)
  enum class ADC_ETRGINJ_RM : uint32_t { PD3 = 0, PC2 = (1U << 17) };

  // PA1, PA2 pins function (PCFR1 bits)
  enum class PA12_RM : uint32_t { GPIO = 0, CRISTALL = (1U << 15) };

  // TIM2 (PCFR1 bits)
  enum class TIM2_RM : uint32_t { CH1_ETR_PD4_CH2_PD3_CH3_PC0_CH4_PD7 = 0,
                                  CH1_ETR_PC5_CH2_PC2_CH3_PD2_CH4_PC1 = (1U << 8),
                                  CH1_ETR_PC1_CH2_PD3_CH3_PC0_CH4_PD7 = (2U << 8),
                                  CH1_ETR_PC1_CH2_PC7_CH3_PD6_CH4_PD5 = (3U << 8) };

  // TIM1 (PCFR1 bits)
  enum class TIM1_RM : uint32_t { ETR_PC5_CH1_PD2_CH2_PA1_CH3_PC3_CH4_PC4_BKIN_PC2_CH1N_PD0_CH2N_PA2_CH3N_PD1 = 0,
                                  ETR_PC5_CH1_PC6_CH2_PC7_CH3_PC0_CH4_PD3_BKIN_PC1_CH1N_PC3_CH2N_PC4_CH3N_PD1 = (1U << 6),
                                  ETR_PD4_CH1_PD2_CH2_PA1_CH3_PC3_CH4_PC4_BKIN_PC2_CH1N_PD0_CH2N_PA2_CH3N_PD1 = (2U << 6),
                                  ETR_PC2_CH1_PC4_CH2_PC7_CH3_PC5_CH4_PD4_BKIN_PC1_CH1N_PC3_CH2N_PD2_CH3N_PC6 = (3U << 6) };

  // SPI1 (PCFR1 bits)
  enum class SPI1_RM : uint32_t { NSS_PC1_SCK_PC5_MISO_PC7_MOSI_PC6 = 0,
                                  NSS_PC0_SCK_PC5_MISO_PC7_MOSI_PC6 = 1U };

  template <auto... rm>
  void Remap()
  {
    constexpr auto pcfr1 = (uint32_t(rm) | ... | 0);
    if constexpr (pcfr1 != 0) AFIO->PCFR1 = pcfr1;
  }

#endif
  
}