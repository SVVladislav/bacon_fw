//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2023.2.2 (win64) Build 4126759 Thu Feb  8 23:53:51 MST 2024
//Date        : Tue Apr 30 17:12:16 2024
//Host        : DESKTOP-BA273GU running 64-bit major release  (build 9200)
//Command     : generate_target bacon_wrapper.bd
//Design      : bacon_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module bacon_wrapper
   (ADC_nCS,
    ADC_sclk,
    ADC_sdio,
    CLK0_JESD_N,
    CLK0_JESD_P,
    CLK1_JESD_N,
    CLK1_JESD_P,
    DDS_AD9915_d,
    DDS_AD9915_dir,
    DDS_AD9915_drctl,
    DDS_AD9915_drhold,
    DDS_AD9915_drover,
    DDS_AD9915_f,
    DDS_AD9915_io_update,
    DDS_AD9915_osk,
    DDS_AD9915_ps,
    DDS_AD9915_pwr_dwn,
    DDS_AD9915_rst,
    DDS_AD9915_sync,
    LMK04828_ncs,
    LMK04828_reset,
    LMK04828_sclk,
    LMK04828_sdio,
    LMK04828_sync,
    P74,
    P76,
    P77,
    P78,
    PDN_ADC1,
    PDN_ADC2,
    RESET_USB1,
    RS485_1_AFAR_de,
    RS485_1_AFAR_di,
    RS485_1_AFAR_re,
    RS485_1_AFAR_ro,
    RS485_2_PC_de,
    RS485_2_PC_di,
    RS485_2_PC_re,
    RS485_2_PC_ro,
    RS485_3_de,
    RS485_3_di,
    RS485_3_re,
    RS485_3_ro,
    RS485_4_de,
    RS485_4_di,
    RS485_4_re,
    RS485_4_ro,
    SIN_USB1,
    SOUT_USB1,
    STR1_PN_de,
    STR1_PN_di,
    STR1_PN_re,
    STR1_PN_ro,
    STR2_PRD_de,
    STR2_PRD_di,
    STR2_PRD_re,
    STR2_PRD_ro,
    STR3_PRM_de,
    STR3_PRM_di,
    STR3_PRM_re,
    STR3_PRM_ro,
    STR4_PN_de,
    STR4_PN_di,
    STR4_PN_re,
    STR4_PN_ro,
    STR5_de,
    STR5_di,
    STR5_re,
    STR5_ro,
    STR6_de,
    STR6_di,
    STR6_re,
    STR6_ro,
    STR7_de,
    STR7_di,
    STR7_re,
    STR7_ro,
    STR8_de,
    STR8_di,
    STR8_re,
    STR8_ro,
    clk_in1,
    rx_1_0_n,
    rx_1_0_p,
    rx_1_1_n,
    rx_1_1_p,
    rx_1_2_n,
    rx_1_2_p,
    rx_1_3_n,
    rx_1_3_p,
    rx_2_0_n,
    rx_2_0_p,
    rx_2_1_n,
    rx_2_1_p,
    rx_2_2_n,
    rx_2_2_p,
    rx_2_3_n,
    rx_2_3_p,
    rx_sync_1,
    rx_sync_2);
  output [1:0]ADC_nCS;
  output ADC_sclk;
  inout ADC_sdio;
  input CLK0_JESD_N;
  input CLK0_JESD_P;
  input CLK1_JESD_N;
  input CLK1_JESD_P;
  output [31:0]DDS_AD9915_d;
  output DDS_AD9915_dir;
  output DDS_AD9915_drctl;
  output DDS_AD9915_drhold;
  input DDS_AD9915_drover;
  output [3:0]DDS_AD9915_f;
  output DDS_AD9915_io_update;
  output DDS_AD9915_osk;
  output [2:0]DDS_AD9915_ps;
  output DDS_AD9915_pwr_dwn;
  output DDS_AD9915_rst;
  output DDS_AD9915_sync;
  output [0:0]LMK04828_ncs;
  output [0:0]LMK04828_reset;
  output LMK04828_sclk;
  inout LMK04828_sdio;
  output [0:0]LMK04828_sync;
  output [0:0]P74;
  output [0:0]P76;
  output P77;
  output P78;
  output [0:0]PDN_ADC1;
  output [0:0]PDN_ADC2;
  output RESET_USB1;
  output RS485_1_AFAR_de;
  output RS485_1_AFAR_di;
  output RS485_1_AFAR_re;
  input RS485_1_AFAR_ro;
  output RS485_2_PC_de;
  output RS485_2_PC_di;
  output RS485_2_PC_re;
  input RS485_2_PC_ro;
  output RS485_3_de;
  output RS485_3_di;
  output RS485_3_re;
  input RS485_3_ro;
  output RS485_4_de;
  output RS485_4_di;
  output RS485_4_re;
  input RS485_4_ro;
  input SIN_USB1;
  output SOUT_USB1;
  output STR1_PN_de;
  output STR1_PN_di;
  output STR1_PN_re;
  input STR1_PN_ro;
  output STR2_PRD_de;
  output STR2_PRD_di;
  output STR2_PRD_re;
  input STR2_PRD_ro;
  output STR3_PRM_de;
  output STR3_PRM_di;
  output STR3_PRM_re;
  input STR3_PRM_ro;
  output STR4_PN_de;
  output STR4_PN_di;
  output STR4_PN_re;
  input STR4_PN_ro;
  output STR5_de;
  output STR5_di;
  output STR5_re;
  input STR5_ro;
  output STR6_de;
  output STR6_di;
  output STR6_re;
  input STR6_ro;
  output STR7_de;
  output STR7_di;
  output STR7_re;
  input STR7_ro;
  output STR8_de;
  output STR8_di;
  output STR8_re;
  input STR8_ro;
  input clk_in1;
  input rx_1_0_n;
  input rx_1_0_p;
  input rx_1_1_n;
  input rx_1_1_p;
  input rx_1_2_n;
  input rx_1_2_p;
  input rx_1_3_n;
  input rx_1_3_p;
  input rx_2_0_n;
  input rx_2_0_p;
  input rx_2_1_n;
  input rx_2_1_p;
  input rx_2_2_n;
  input rx_2_2_p;
  input rx_2_3_n;
  input rx_2_3_p;
  output [0:0]rx_sync_1;
  output [0:0]rx_sync_2;

  wire [1:0]ADC_nCS;
  wire ADC_sclk;
  wire ADC_sdio;
  wire CLK0_JESD_N;
  wire CLK0_JESD_P;
  wire CLK1_JESD_N;
  wire CLK1_JESD_P;
  wire [31:0]DDS_AD9915_d;
  wire DDS_AD9915_dir;
  wire DDS_AD9915_drctl;
  wire DDS_AD9915_drhold;
  wire DDS_AD9915_drover;
  wire [3:0]DDS_AD9915_f;
  wire DDS_AD9915_io_update;
  wire DDS_AD9915_osk;
  wire [2:0]DDS_AD9915_ps;
  wire DDS_AD9915_pwr_dwn;
  wire DDS_AD9915_rst;
  wire DDS_AD9915_sync;
  wire [0:0]LMK04828_ncs;
  wire [0:0]LMK04828_reset;
  wire LMK04828_sclk;
  wire LMK04828_sdio;
  wire [0:0]LMK04828_sync;
  wire [0:0]P74;
  wire [0:0]P76;
  wire P77;
  wire P78;
  wire [0:0]PDN_ADC1;
  wire [0:0]PDN_ADC2;
  wire RESET_USB1;
  wire RS485_1_AFAR_de;
  wire RS485_1_AFAR_di;
  wire RS485_1_AFAR_re;
  wire RS485_1_AFAR_ro;
  wire RS485_2_PC_de;
  wire RS485_2_PC_di;
  wire RS485_2_PC_re;
  wire RS485_2_PC_ro;
  wire RS485_3_de;
  wire RS485_3_di;
  wire RS485_3_re;
  wire RS485_3_ro;
  wire RS485_4_de;
  wire RS485_4_di;
  wire RS485_4_re;
  wire RS485_4_ro;
  wire SIN_USB1;
  wire SOUT_USB1;
  wire STR1_PN_de;
  wire STR1_PN_di;
  wire STR1_PN_re;
  wire STR1_PN_ro;
  wire STR2_PRD_de;
  wire STR2_PRD_di;
  wire STR2_PRD_re;
  wire STR2_PRD_ro;
  wire STR3_PRM_de;
  wire STR3_PRM_di;
  wire STR3_PRM_re;
  wire STR3_PRM_ro;
  wire STR4_PN_de;
  wire STR4_PN_di;
  wire STR4_PN_re;
  wire STR4_PN_ro;
  wire STR5_de;
  wire STR5_di;
  wire STR5_re;
  wire STR5_ro;
  wire STR6_de;
  wire STR6_di;
  wire STR6_re;
  wire STR6_ro;
  wire STR7_de;
  wire STR7_di;
  wire STR7_re;
  wire STR7_ro;
  wire STR8_de;
  wire STR8_di;
  wire STR8_re;
  wire STR8_ro;
  wire clk_in1;
  wire rx_1_0_n;
  wire rx_1_0_p;
  wire rx_1_1_n;
  wire rx_1_1_p;
  wire rx_1_2_n;
  wire rx_1_2_p;
  wire rx_1_3_n;
  wire rx_1_3_p;
  wire rx_2_0_n;
  wire rx_2_0_p;
  wire rx_2_1_n;
  wire rx_2_1_p;
  wire rx_2_2_n;
  wire rx_2_2_p;
  wire rx_2_3_n;
  wire rx_2_3_p;
  wire [0:0]rx_sync_1;
  wire [0:0]rx_sync_2;

  bacon bacon_i
       (.ADC_nCS(ADC_nCS),
        .ADC_sclk(ADC_sclk),
        .ADC_sdio(ADC_sdio),
        .CLK0_JESD_N(CLK0_JESD_N),
        .CLK0_JESD_P(CLK0_JESD_P),
        .CLK1_JESD_N(CLK1_JESD_N),
        .CLK1_JESD_P(CLK1_JESD_P),
        .DDS_AD9915_d(DDS_AD9915_d),
        .DDS_AD9915_dir(DDS_AD9915_dir),
        .DDS_AD9915_drctl(DDS_AD9915_drctl),
        .DDS_AD9915_drhold(DDS_AD9915_drhold),
        .DDS_AD9915_drover(DDS_AD9915_drover),
        .DDS_AD9915_f(DDS_AD9915_f),
        .DDS_AD9915_io_update(DDS_AD9915_io_update),
        .DDS_AD9915_osk(DDS_AD9915_osk),
        .DDS_AD9915_ps(DDS_AD9915_ps),
        .DDS_AD9915_pwr_dwn(DDS_AD9915_pwr_dwn),
        .DDS_AD9915_rst(DDS_AD9915_rst),
        .DDS_AD9915_sync(DDS_AD9915_sync),
        .LMK04828_ncs(LMK04828_ncs),
        .LMK04828_reset(LMK04828_reset),
        .LMK04828_sclk(LMK04828_sclk),
        .LMK04828_sdio(LMK04828_sdio),
        .LMK04828_sync(LMK04828_sync),
        .P74(P74),
        .P76(P76),
        .P77(P77),
        .P78(P78),
        .PDN_ADC1(PDN_ADC1),
        .PDN_ADC2(PDN_ADC2),
        .RESET_USB1(RESET_USB1),
        .RS485_1_AFAR_de(RS485_1_AFAR_de),
        .RS485_1_AFAR_di(RS485_1_AFAR_di),
        .RS485_1_AFAR_re(RS485_1_AFAR_re),
        .RS485_1_AFAR_ro(RS485_1_AFAR_ro),
        .RS485_2_PC_de(RS485_2_PC_de),
        .RS485_2_PC_di(RS485_2_PC_di),
        .RS485_2_PC_re(RS485_2_PC_re),
        .RS485_2_PC_ro(RS485_2_PC_ro),
        .RS485_3_de(RS485_3_de),
        .RS485_3_di(RS485_3_di),
        .RS485_3_re(RS485_3_re),
        .RS485_3_ro(RS485_3_ro),
        .RS485_4_de(RS485_4_de),
        .RS485_4_di(RS485_4_di),
        .RS485_4_re(RS485_4_re),
        .RS485_4_ro(RS485_4_ro),
        .SIN_USB1(SIN_USB1),
        .SOUT_USB1(SOUT_USB1),
        .STR1_PN_de(STR1_PN_de),
        .STR1_PN_di(STR1_PN_di),
        .STR1_PN_re(STR1_PN_re),
        .STR1_PN_ro(STR1_PN_ro),
        .STR2_PRD_de(STR2_PRD_de),
        .STR2_PRD_di(STR2_PRD_di),
        .STR2_PRD_re(STR2_PRD_re),
        .STR2_PRD_ro(STR2_PRD_ro),
        .STR3_PRM_de(STR3_PRM_de),
        .STR3_PRM_di(STR3_PRM_di),
        .STR3_PRM_re(STR3_PRM_re),
        .STR3_PRM_ro(STR3_PRM_ro),
        .STR4_PN_de(STR4_PN_de),
        .STR4_PN_di(STR4_PN_di),
        .STR4_PN_re(STR4_PN_re),
        .STR4_PN_ro(STR4_PN_ro),
        .STR5_de(STR5_de),
        .STR5_di(STR5_di),
        .STR5_re(STR5_re),
        .STR5_ro(STR5_ro),
        .STR6_de(STR6_de),
        .STR6_di(STR6_di),
        .STR6_re(STR6_re),
        .STR6_ro(STR6_ro),
        .STR7_de(STR7_de),
        .STR7_di(STR7_di),
        .STR7_re(STR7_re),
        .STR7_ro(STR7_ro),
        .STR8_de(STR8_de),
        .STR8_di(STR8_di),
        .STR8_re(STR8_re),
        .STR8_ro(STR8_ro),
        .clk_in1(clk_in1),
        .rx_1_0_n(rx_1_0_n),
        .rx_1_0_p(rx_1_0_p),
        .rx_1_1_n(rx_1_1_n),
        .rx_1_1_p(rx_1_1_p),
        .rx_1_2_n(rx_1_2_n),
        .rx_1_2_p(rx_1_2_p),
        .rx_1_3_n(rx_1_3_n),
        .rx_1_3_p(rx_1_3_p),
        .rx_2_0_n(rx_2_0_n),
        .rx_2_0_p(rx_2_0_p),
        .rx_2_1_n(rx_2_1_n),
        .rx_2_1_p(rx_2_1_p),
        .rx_2_2_n(rx_2_2_n),
        .rx_2_2_p(rx_2_2_p),
        .rx_2_3_n(rx_2_3_n),
        .rx_2_3_p(rx_2_3_p),
        .rx_sync_1(rx_sync_1),
        .rx_sync_2(rx_sync_2));
endmodule
