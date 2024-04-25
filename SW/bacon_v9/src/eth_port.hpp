#pragma once

struct AXI_GPIO
{
uint32_t DATA_CH1;
uint32_t TRI_CH1;
uint32_t DATA_CH2;
uint32_t TRI_CH2;
};

template <uint32_t pAXI_GPIO>
struct PAYLOAD
{
  PAYLOAD() = delete;
  static inline void Set(uint32_t payload) { base()->DATA_CH1 = payload; }
private:
  static inline auto base() { return (AXI_GPIO *) (pAXI_GPIO); }
};

struct TAXI_IO_REGS
{
  uint32_t ISR;
  uint32_t IER;
  uint32_t TDFR;
  uint32_t TDFV;
  uint32_t TDFD;
  uint32_t TLR;
  uint32_t RDFR;
  uint32_t RDFO;
  uint32_t RDFD;
  uint32_t RLR;
  uint32_t SRR;
  uint32_t TDR;
  uint32_t RDR;
};

template <uint32_t pRX_FIFO, uint32_t pTX_FIFO, typename TPAYLOAD>
struct TETH_PORT
{
  TETH_PORT() = default;

  static void Init()
  {
    // RX FIFO
    rx_base()->ISR = 0xFFFF'FFFF;  // reset fifo_rx interrupt register
    rx_base()->IER = 0x0400'0000;  // disable all fifo_rx interrupts except of rx complete
    // TX FIFO
    //tx_base()->IER = 0xC000000;   // disable all except rx complete and tx complete
    tx_base()->ISR = 0xFFFF'FFFF; // reset interrupt register
    TXFIFOReset();
    RXFIFOReset();
  }

  static void Write(uint8_t *buf, uint32_t size) 
  {    
    TPAYLOAD::Set(size);
    uint32_t *pbuf = (uint32_t *)buf;   
    for(auto i=0u; i<size/4; i++) tx_base()->TDFD = *pbuf++;
    tx_base()->TDFV;
    tx_base()->TLR = size;
    
  }

  static inline uint32_t ReadReceiveLength() { return rx_base()->RLR & 0x003F'FFFF; }
  static inline uint32_t  ReadRXFIFIOccupancy() { return rx_base()->RDFO; }
  static inline uint32_t ReadWord() { return rx_base()->RDFD; }  
  static void ClearRXCInterrupt() { rx_base()->ISR = 0x0400'0000; }
  static inline void TXFIFOReset() { rx_base()->TDFR = 0xA5; }
  static inline void RXFIFOReset() { rx_base()->RDFR = 0xA5; }

private:
  static inline auto rx_base() { return (TAXI_IO_REGS *) (pRX_FIFO); }
  static inline auto tx_base() { return (TAXI_IO_REGS *) (pTX_FIFO); }
};