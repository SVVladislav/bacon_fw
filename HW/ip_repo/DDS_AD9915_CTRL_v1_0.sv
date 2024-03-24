// (c) Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// (c) Copyright 2022-2024 Advanced Micro Devices, Inc. All rights reserved.
// 
// This file contains confidential and proprietary information
// of AMD and is protected under U.S. and international copyright
// and other intellectual property laws.
// 
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// AMD, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND AMD HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) AMD shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or AMD had been advised of the
// possibility of the same.
// 
// CRITICAL APPLICATIONS
// AMD products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of AMD products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
// 
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// 
// DO NOT MODIFY THIS FILE.


`ifndef DDS_AD9915_CTRL_v1_0
`define DDS_AD9915_CTRL_v1_0

interface DDS_AD9915_CTRL_v1_0();
  logic [31:0] D = 0;                                    // Parallel Port Pins
  logic [3:0] F = 0;                                    // Function Pins
  logic [2:0] PS = 0;                                   // Profile Select Pins
  logic pwr_dwn = 0;                                    // External Power-Down
  logic rst = 0;                                        // Master Reset
  logic osk = 0;                                        // Output Shift Keying
  logic drctl = 0;                                      // Ramp Control
  logic drhold = 0;                                     // Ramp Hold
  logic drover;                                          // Ramp Over
  logic io_update = 0;                                  // Input/Output Update
  logic sync = 0;                                       // Digital Synchronization Output
  logic sync_clk;                                        // Many of the digital inputs on the chip, such as I/O_UPDATE, PS[2:0], and the parallel data port (D0 to D31), must be set up on the rising edge of this signal
  logic dir;                                            // Data Bus Direction

  modport MASTER (
    input drover, sync_clk, 
    output D, F, PS, pwr_dwn, rst, osk, drctl, drhold, io_update, sync
    );

  modport SLAVE (
    input D, F, PS, pwr_dwn, rst, osk, drctl, drhold, io_update, sync, 
    output drover, sync_clk, dir
    );

  modport MONITOR (
    input D, F, PS, pwr_dwn, rst, osk, drctl, drhold, drover, io_update, sync, sync_clk, dir
    );

endinterface // DDS_AD9915_CTRL_v1_0

`endif