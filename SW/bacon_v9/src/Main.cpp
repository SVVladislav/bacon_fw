#include "Main.hpp"

void __attribute__((fast_interrupt)) Temer1Interrupt()
{  
  //uint32_t x = LMK::ReadReg(0x0003);
  //DEBUG_Print("%x ",x); 
  //LMK::WriteReg(0x0000,0x90);

}

int main()
{
  DEBUG_Print("Bacon firmware started. __cplusplus=%d\r\n",__cplusplus);

  microblaze_disable_interrupts();

  // Инициализация последовательных интерфейсов UART
  UART_USB::Init<921'600, PARITY::None>();
  UART_RS485_PC::Init<921'600, PARITY::None>();
  UART_RS485_AFAR::Init<921'600, PARITY::Odd>();
  UART_USB::EnableInterrupt(UIER::RBF);
  UART_RS485_PC::EnableInterrupt(UIER::RBF);
  UART_RS485_AFAR::EnableInterrupt(UIER::RBF);
  
  // Инициализация Ethernet
  //ETH_PORT::Init();

  // Иницализация контроллера прерываний
  INTC::Init();
  INTC::InstallIRQ(Temer1Interrupt_NUM, Temer1Interrupt);
  INTC::InstallIRQ(UartRS485PC_Interrupt_NUM, UartRS485PC_InterruptHandler);
  INTC::InstallIRQ(UartRS485AFAR_Interrupt_NUM, UartRS485AFAR_InterruptHandler);
  INTC::InstallIRQ(UartUSB_Interrupt_NUM, UartUSB_InterruptHandler);
  //INTC::InstallIRQ(AXIS_FIFO_RX_InterruptHandler_NUM, AXIS_FIFO_RX_InterruptHandler);
  //INTC::InstallIRQ(AXIS_FIFO_TX_InterruptHandler_NUM, AXIS_FIFO_TX_InterruptHandler);
  
  // Загрузить синтезатор частот LMK04828  
  LMK::Init();

  PDN_ADC1::clear();

  // Установить тестовые профили
  for(auto i=0u; i<32; bacon.WriteTestProfile(i++));
  
  for(auto i=0u; i<32; bacon.WriteTestProfile<BCO_PROFILE_BASE>(i++));

  microblaze_enable_interrupts();

  ADI_SPI_ADC::SlaveSelect(1);

  for(auto i=0u; i<8; i++)
  {  
    auto x = ADI_SPI_ADC::ReadReg(i);
    DEBUG_Print("ADC Reg %d = %X\r\n", i, x); 
  }

  for(;;) 
  {
    if(control_if_usb.isCommandReceived())   control_if_usb.CommandProcessing();
    if(control_if_rs485.isCommandReceived()) control_if_rs485.CommandProcessing();
    //if(control_if_eth.isCommandReceived())   control_if_eth.CommandProcessing();
  }
}