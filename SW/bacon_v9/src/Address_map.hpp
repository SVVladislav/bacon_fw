#pragma once

#define AXI_INTC_BASE   (XPAR_XINTC_0_BASEADDR)     // Interrupt Contrioller
#define UART_MDM_BASE   (XPAR_MDM_0_BASEADDR)       // Debug Interface

#define PC_UART_BASE (XPAR_UARTS_AND_STROBES_AXI_UART16550_0_BASEADDR)    // RS485_PC
#define AFAR_UART_BASE (XPAR_UARTS_AND_STROBES_AXI_UART16550_1_BASEADDR)  // RS485_AFAR
#define USB_UART_BASE (XPAR_UARTS_AND_STROBES_AXI_UART16550_3_BASEADDR)   // USB-UART

#define CYCLE_COUNTER_BASE    (XPAR_SVV_AXI_CYCCNT_0_BASEADDR)             // Cycle and mks Counter
#define GPIOA_BASE            (XPAR_SVV_LMB_GPIO_0_BASEADDR)               // SVV GPIOA
#define PROFILE_BRAM_BASE     (XPAR_AXI_BRAM_0_BASEADDRESS)                // Profile memory
#define SVV_LMB_LMK04828_SPI_BASE  (XPAR_SVV_LMB_LMK04828_SPI_0_BASEADDR)  // SVV LMK04828 SPI Interface

//#define AXI_IO_RX_BASE (XPAR_AXI_FIFO_RX_BASEADDR)
//#define AXI_IO_TX_BASE (XPAR_AXI_FIFO_TX_BASEADDR)

#define BCO_PROFILE_BASE      (XPAR_BCO_PROFILE_0_BASEADDR)

//#define AXIS_FIFO_RX_InterruptHandler_NUM 
//#define AXIS_FIFO_TX_InterruptHandler_NUM 
#define MDM_Interrupt_NUM                 12
#define UartRS485PC_Interrupt_NUM         8
#define UartRS485AFAR_Interrupt_NUM       3
#define UartUSB_Interrupt_NUM             2
#define Temer1Interrupt_NUM               14