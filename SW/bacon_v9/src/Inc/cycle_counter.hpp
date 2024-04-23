#pragma once

#include <stdint.h>

struct TCYCLE_REGS
{
  volatile uint32_t cycle_lo;
  volatile uint32_t cycle_hi;
  volatile uint32_t mks_lo;
  volatile uint32_t mks_hi;
};

template <uint32_t pMKS>
struct TCYCLE_COUNTER
{
  TCYCLE_COUNTER() = delete;
  
  static inline uint64_t Read_mks()
  {
    uint32_t res_l = base()->mks_lo;
    uint32_t res_h = base()->mks_hi;
    return ((uint64_t)res_l + ((uint64_t)res_h<<32));
  }  

  static inline uint64_t Read_cycles()
  {
    uint32_t res_l = base()->cycle_lo;
    uint32_t res_h = base()->cycle_hi;
    return ((uint64_t)res_l + ((uint64_t)res_h<<32));
  }  


private:
  static inline auto base() { return (TCYCLE_REGS *) pMKS; }
};
