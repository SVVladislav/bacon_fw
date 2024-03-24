//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2023.2.2 (win64) Build 4126759 Thu Feb  8 23:53:51 MST 2024
//Date        : Sun Mar 24 18:34:41 2024
//Host        : Zen running 64-bit major release  (build 9200)
//Command     : generate_target bacon_wrapper.bd
//Design      : bacon_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module bacon_wrapper
   (STR1_PN_de,
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
    clk_in1,
    clk_out_156M25,
    dds_ad9915_d,
    dds_ad9915_dir,
    dds_ad9915_drctl,
    dds_ad9915_drhold,
    dds_ad9915_drover,
    dds_ad9915_f,
    dds_ad9915_io_update,
    dds_ad9915_osk,
    dds_ad9915_ps,
    dds_ad9915_pwr_dwn,
    dds_ad9915_rst,
    dds_ad9915_sync);
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
  input clk_in1;
  output clk_out_156M25;
  output [31:0]dds_ad9915_d;
  output dds_ad9915_dir;
  output dds_ad9915_drctl;
  output dds_ad9915_drhold;
  input dds_ad9915_drover;
  output [3:0]dds_ad9915_f;
  output dds_ad9915_io_update;
  output dds_ad9915_osk;
  output [2:0]dds_ad9915_ps;
  output dds_ad9915_pwr_dwn;
  output dds_ad9915_rst;
  output dds_ad9915_sync;

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
  wire clk_in1;
  wire clk_out_156M25;
  wire [31:0]dds_ad9915_d;
  wire dds_ad9915_dir;
  wire dds_ad9915_drctl;
  wire dds_ad9915_drhold;
  wire dds_ad9915_drover;
  wire [3:0]dds_ad9915_f;
  wire dds_ad9915_io_update;
  wire dds_ad9915_osk;
  wire [2:0]dds_ad9915_ps;
  wire dds_ad9915_pwr_dwn;
  wire dds_ad9915_rst;
  wire dds_ad9915_sync;

  bacon bacon_i
       (.STR1_PN_de(STR1_PN_de),
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
        .clk_in1(clk_in1),
        .clk_out_156M25(clk_out_156M25),
        .dds_ad9915_d(dds_ad9915_d),
        .dds_ad9915_dir(dds_ad9915_dir),
        .dds_ad9915_drctl(dds_ad9915_drctl),
        .dds_ad9915_drhold(dds_ad9915_drhold),
        .dds_ad9915_drover(dds_ad9915_drover),
        .dds_ad9915_f(dds_ad9915_f),
        .dds_ad9915_io_update(dds_ad9915_io_update),
        .dds_ad9915_osk(dds_ad9915_osk),
        .dds_ad9915_ps(dds_ad9915_ps),
        .dds_ad9915_pwr_dwn(dds_ad9915_pwr_dwn),
        .dds_ad9915_rst(dds_ad9915_rst),
        .dds_ad9915_sync(dds_ad9915_sync));
endmodule
