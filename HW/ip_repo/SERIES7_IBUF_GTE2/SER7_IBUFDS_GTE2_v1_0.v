`timescale 1 ns / 1 ps

module SER7_IBUFDS_GTE2_v1_0 #
(
  parameter MODE = "Always On" 
)
(
  // Users to add ports here
  input I, 
  input IB,
  output O,
  output ODIV2,
  input CEB

);

generate
  case (MODE)
    "Always On" : begin
                    IBUFDS_GTE2 #(
                      .CLKCM_CFG("TRUE"),   // Refer to Transceiver User Guide
                      .CLKRCV_TRST("TRUE"), // Refer to Transceiver User Guide
                      .CLKSWING_CFG(2'b11)  // Refer to Transceiver User Guide
                    )
                    IBUFDS_GTE2_inst (
                      .O(O),         // 1-bit output: Refer to Transceiver User Guide
                      .ODIV2(ODIV2), // 1-bit output: Refer to Transceiver User Guide
                      .CEB(1'b0),     // 1-bit input: Refer to Transceiver User Guide
                      .I(I),         // 1-bit input: Refer to Transceiver User Guide
                      .IB(IB)        // 1-bit input: Refer to Transceiver User Guide
                    );
                  end
    "Controled" : begin
                   IBUFDS_GTE2 #(
                     .CLKCM_CFG("TRUE"),   // Refer to Transceiver User Guide
                     .CLKRCV_TRST("TRUE"), // Refer to Transceiver User Guide
                     .CLKSWING_CFG(2'b11)  // Refer to Transceiver User Guide
                   )
                   IBUFDS_GTE2_inst (
                     .O(O),         // 1-bit output: Refer to Transceiver User Guide
                     .ODIV2(ODIV2), // 1-bit output: Refer to Transceiver User Guide
                     .CEB(CEB),     // 1-bit input: Refer to Transceiver User Guide
                     .I(I),         // 1-bit input: Refer to Transceiver User Guide
                     .IB(IB)        // 1-bit input: Refer to Transceiver User Guide
                   );
                 end
  endcase
endgenerate

endmodule
