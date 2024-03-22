//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2023.2.2 (win64) Build 4126759 Thu Feb  8 23:53:51 MST 2024
//Date        : Fri Mar 22 22:47:56 2024
//Host        : Zen running 64-bit major release  (build 9200)
//Command     : generate_target bacon_wrapper.bd
//Design      : bacon_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module bacon_wrapper
   (clk_in1,
    sout);
  input clk_in1;
  output sout;

  wire clk_in1;
  wire sout;

  bacon bacon_i
       (.clk_in1(clk_in1),
        .sout(sout));
endmodule
