#pragma once

#include <cstdint>
#include <array>

template<typename TADI_SPI>
struct TLMK04828
{    
  TLMK04828() = delete;
  
  // Полная загрузка
  static inline void Init() { WriteInit(regs); }
  
private:
  // Загрузить массив инициализации синтезатора
  template <typename T>
  static inline void WriteInit(const T &regs)
  {
    for(auto &x : regs) 
	{ 
      TADI_SPI::Write(x);
	}
  }

  static constexpr std::array regs = {
    // Reset
    0x000090, //0x000010,
	// Power down off
    0x000200,
	
	// Readonly !!!
    // 0x000306,
    // 0x0004D0,
    // 0x00055B,
    // 0x000600,
    // 0x000C51,
    // 0x000D04,	
	
	// 0x100 - 0x138 : Device Clock and SYSREF Clock Output Controls    
		0x010001, 0x010155, 0x010255, 0x010300,
        0x010402, 0x010500, 0x0106F9, 0x010755,
		//DCLKout2   CLK_DDS=3000 Мгц  Bypass
        0x010801, 0x010955, 0x010A55, 0x010B02, 
        0x010C02, 0x010D00, 0x010EF1, 0x010F01,
		//DCLKout4   CLK_ADC2=1000 Мгц
        0x011003, 0x011155, 0x011255, 0x011300,
        0x011460, 0x01151C, 0x0116B0, 0x011711,
		//DCLKout6   CLK_ADC1=1000 Мгц
        0x011803, 0x011955, 0x011A55, 0x011B00,
        0x011C20, 0x011D1C, 0x011EF0, 0x011F11,
		//DCLKout8   CLK0_JESD=500 Мгц
        0x012006, 0x012155, 0x012255, 0x012300,
        0x012420, 0x012500, 0x0126F0, 0x012711,
		//DCLKout10   CLK1_JESD=500 Мгц
        0x012806, 0x012955, 0x012A55, 0x012B00,
        0x012C02, 0x012D00, 0x012EF1, 0x012F01,
		//DCLKout12 CLK0_FPGA=187,5МГц
        0x013018, 0x013155, 0x013255, 0x013300,
        0x013402, 0x013500, 0x0136F1, 0x013701,
		
        0x013821,
        0x013903,//0x013900,
        0x013A00,
        0x013B60,
        0x013C00,
        0x013D08,
        0x013E03,
        0x013F00,
        0x01408B,
        0x014100,
        0x014200,
		0x014359,
        0x01447F,
        0x01457F,		
		// (0x146 - 0x149): CLKin Control
        0x014604, 0x01470E, 0x014802, 0x014942,		
        0x014A02,
        0x014B16,
        0x014C00,
        0x014D00,
        0x014EC0,
        0x014F7F,
        0x015003,
        0x015102,
        0x015200,
        // (0x153 - 0x15F): PLL1 Configuration		
        0x015300, 0x015430, 0x015500, 0x015678,
        0x015700, 0x015896, 0x015900, 0x015A3B,
        0x015BD4, 0x015C20, 0x015D00, 0x015E00, 0x015F6B,
        // (0x160 - 0x16E): PLL2 Configuration		
        0x016000, 0x016101, 0x016244, 0x016300,
        0x016400, 0x016502, 		
		// Fixed reisters
		0x0171AA, 0x017202,
        0x017C15,
        0x017D33,
        0x016600,
        0x016700,
        0x01680F,
        0x016959,
        0x016A20,
        0x016B00,
        0x016C00,
        0x016D00,
        0x016E13,
        0x017300,
		// SPI UnLock Не надо?
        //0x1FFD00, 0x1FFE00, 0x1FFF53
  };
  
}; 
