#pragma once

#include <stdint.h>

#include "xparameters.h"
#include "xil_printf.h"
#include "mb_interface.h"

#define __DEBUG__
//#define __IF_TEST__      // Модуль тестирования протокола управления

#ifndef __DEBUG__
#undef __IF_TEST__
#endif

#ifdef  __IF_TEST__
#define IF_DEBUG_Print(...) DEBUG_Print(__VA_ARGS__)
#else
#define IF_DEBUG_Print(...) ((void)0)
#endif

#include "Address_map.hpp"

#include "InterruptController.hpp"

#include "XUART.hpp"

// Определение классов последовательных портов
using UART_USB        = TAURT16550<USB_UART_BASE, 14'745'600, MODE::FULL_DUPLEX>;
using UART_RS485_AFAR = TAURT16550<AFAR_UART_BASE, 14'745'600, MODE::HALF_DUPLEX>;
using UART_RS485_PC   = TAURT16550<PC_UART_BASE, 14'745'600, MODE::HALF_DUPLEX>;

#define __NOP()   asm volatile ("nop")

// Определение GPIO портов и подключение пинов
#define MICROBLAZE_SVV
#include "Inc\SVVTL\template_lib.hpp"
#include "Inc\Hardware\GPIO\Gpio.hpp"
using EN_ZOND = GPIO::PA_0;
using PDN_ADC1 = GPIO::PA_16;
using PDN_ADC2 = GPIO::PA_17;

// Счётчик тактов процессора и мкс
#include "Inc\cycle_counter.hpp"
using MKS = TCYCLE_COUNTER<CYCLE_COUNTER_BASE>;

// Порт упрвления через Ethernet
//#include "eth_port.hpp"
//using ETH_PORT = TETH_PORT<AXI_IO_RX_BASE, AXI_IO_TX_BASE, PAYLOAD<XPAR_AXI_GPIO_PAYLOAD_BASEADDR> >;

// Стату ошибок в протоколе управления
namespace CONTROL_IF
{
  enum class ERROR_STATUS : uint8_t { Ok=0, Fault=1, WrongCMD=2, WrongData=4 };
}

// Объект 
#include "Bacon.hpp"
extern TBACON<EN_ZOND> bacon;

#include "Control_Interface.hpp"
using CTRL_IF_USB = CONTROL_IF::TCONTROL_IF<UART_USB, MKS>;         // Интерфейс управления через USB
using CTRL_IF_RS485 = CONTROL_IF::TCONTROL_IF<UART_RS485_PC, MKS>;  // Интерфейс управления через RS485
//using CTRL_IF_ETH = CONTROL_IF::TCONTROL_IF<ETH_PORT, MKS>;         // Интерфейс управления через Ethernet

extern CTRL_IF_USB control_if_usb;
extern CTRL_IF_USB control_if_rs485;
//extern CTRL_IF_ETH control_if_eth;

// SPI-интерфейсы
#include "SVV_ADI_SPI.hpp"
using ADI_SPI_LMK = TADI_SPI<SPI_LMK_BASE>;
using ADI_SPI_ADC = TADI_SPI<SPI_ADC_BASE>;

// Синтезатор частот LMK04828 на СВЧ-плате
#include "SVV_LMK04828.hpp"
using LMK = TLMK04828<ADI_SPI_LMK>;
