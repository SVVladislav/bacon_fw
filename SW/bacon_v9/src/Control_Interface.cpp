#include "Main.hpp"

uint8_t uart_control_rx_buf[uint32_t(CONTROL_IF::MSG_LEN::MAX)];
uint8_t uart_control_tx_buf[uint32_t(CONTROL_IF::MSG_LEN::MAX)];

//uint8_t eth_control_rx_buf[uint32_t(CONTROL_IF::MSG_LEN::MAX)];
//uint8_t eth_control_tx_buf[uint32_t(CONTROL_IF::MSG_LEN::MAX)];

CTRL_IF_USB control_if_usb(uart_control_rx_buf, uart_control_tx_buf);
CTRL_IF_USB control_if_rs485(uart_control_rx_buf, uart_control_tx_buf);
//CTRL_IF_ETH control_if_eth(eth_control_rx_buf, eth_control_tx_buf);

void __attribute__((fast_interrupt)) UartRS485PC_InterruptHandler()
{
  while(UART_RS485_PC::isDR()) control_if_rs485.ProcessByte(UART_RS485_PC::ReadByte());
}

void __attribute__((fast_interrupt)) UartUSB_InterruptHandler()
{
  while(UART_USB::isDR()) control_if_usb.ProcessByte(UART_USB::ReadByte());
}

void __attribute__((fast_interrupt)) UartRS485AFAR_InterruptHandler()
{
  while(UART_RS485_AFAR::isDR()) UART_RS485_AFAR::ReadByte();
}

//void __attribute__((fast_interrupt)) AXIS_FIFO_RX_InterruptHandler()
//{
//  ETH_PORT::ClearRXCInterrupt();
//  while(ETH_PORT::ReadRXFIFIOccupancy())
//  {
//    uint32_t rx_len = ETH_PORT::ReadReceiveLength();
//    for(auto i=0u; i<rx_len/4; i++) control_if_eth.ProcessWord(ETH_PORT::ReadWord());
//  }  
//}