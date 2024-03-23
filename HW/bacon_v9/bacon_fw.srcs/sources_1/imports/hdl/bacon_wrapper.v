//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2023.2.2 (win64) Build 4126759 Thu Feb  8 23:53:51 MST 2024
//Date        : Sat Mar 23 22:23:37 2024
//Host        : Zen running 64-bit major release  (build 9200)
//Command     : generate_target bacon_wrapper.bd
//Design      : bacon_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module bacon_wrapper
   (clk_in1,
    clk_out_156M25);
  input clk_in1;
  output clk_out_156M25;

  wire clk_in1;
  wire clk_out_156M25;

  bacon bacon_i
       (.clk_in1(clk_in1),
        .clk_out_156M25(clk_out_156M25));
endmodule
