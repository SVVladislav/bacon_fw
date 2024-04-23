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

using UART_USB        = TAURT16550<USB_UART_BASE, 14'745'600, MODE::FULL_DUPLEX>;
using UART_RS485_AFAR = TAURT16550<AFAR_UART_BASE, 14'745'600, MODE::HALF_DUPLEX>;
using UART_RS485_PC   = TAURT16550<PC_UART_BASE, 14'745'600, MODE::HALF_DUPLEX>;

#define __NOP()   asm volatile ("nop")

#define MICROBLAZE_SVV
#include "Inc\SVVTL\template_lib.hpp"
#include "Inc\Hardware\GPIO\Gpio.hpp"

#include "Inc\cycle_counter.hpp"
using MKS = TCYCLE_COUNTER<CYCLE_COUNTER_BASE>;

namespace CONTROL_IF
{
  enum class ERROR_STATUS : uint8_t { Ok=0, Fault=1, WrongCMD=2, WrongData=4 };
}

#include "Bacon.hpp"

#include "Control_Interface.hpp"

using CTRL_IF_USB = CONTROL_IF::TCONTROL_IF<UART_USB, MKS>;
using CTRL_IF_RS485 = CONTROL_IF::TCONTROL_IF<UART_RS485_PC, MKS>;

extern CTRL_IF_USB control_if_usb;
extern CTRL_IF_USB control_if_rs485;

#include "SVV_LMK04828.hpp"
using LMK = TLMK04828<SVV_LMB_LMK04828_SPI_BASE>;

