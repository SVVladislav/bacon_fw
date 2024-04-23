#include "Main.hpp"

void __attribute__((fast_interrupt)) Temer1Interrupt()
{  
  uint8_t x = LMK::ReadReg(0x0003);
  DEBUG_Print("%d ",x);
}

int main()
{
  DEBUG_Print("Bacon firmware started. __cplusplus=%d\r\n",__cplusplus);

  microblaze_disable_interrupts();

  UART_USB::Init<921'600, PARITY::None>();
  UART_RS485_PC::Init<921'600, PARITY::None>();
  UART_RS485_AFAR::Init<921'600, PARITY::Odd>();

  UART_USB::EnableInterrupt(UIER::RBF);
  UART_RS485_PC::EnableInterrupt(UIER::RBF);
  UART_RS485_AFAR::EnableInterrupt(UIER::RBF);
  
  INTC::Init();
  INTC::InstallIRQ(Temer1Interrupt_NUM, Temer1Interrupt);
  INTC::InstallIRQ(UartRS485PC_Interrupt_NUM, UartRS485PC_InterruptHandler);
  INTC::InstallIRQ(UartRS485AFAR_Interrupt_NUM, UartRS485AFAR_InterruptHandler);
  INTC::InstallIRQ(UartUSB_Interrupt_NUM, UartUSB_InterruptHandler);
  
  LMK::Init();

  for(auto i=0u; i<32; bacon.WriteTestProfile(i++));
  
  for(auto i=0u; i<32; bacon.WriteTestProfile<BCO_PROFILE_BASE>(i++));

  microblaze_enable_interrupts();

  for(;;) 
  {
    if(control_if_usb.isCommandReceived())   control_if_usb.CommandProcessing();
    if(control_if_rs485.isCommandReceived()) control_if_rs485.CommandProcessing();
  }
}