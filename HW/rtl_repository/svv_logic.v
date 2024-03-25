module INVERTER #(parameter WIDTH = 1)
(
  input wire [WIDTH-1:0] din,
  output wire [WIDTH-1:0] dout
);
  assign dout = ~din;
endmodule

module AND #(parameter WIDTH = 1)
(
  input wire [WIDTH-1:0] din1,
  input wire [WIDTH-1:0] din2,
  output wire [WIDTH-1:0] dout
);
  assign dout = din1&din2;
endmodule

module STROBE_LOCK #(parameter WIDTH = 1)
(
  input wire blk,
  input wire str_in,
  input wire [WIDTH-1:0] str_en,  
  output wire [WIDTH-1:0] str_out
);
  
  genvar i;
  
  for (i=0; i<WIDTH; i=i+1)
    assign str_out[i] = str_en[i]&str_in&(~blk);
    
endmodule

module tri_io_buf #(parameter WIDTH = 1)
                   (
                     input   wire  [WIDTH-1:0]       din,
                     input   wire                    oen,
                     inout   wire  [WIDTH-1:0]       io_pad,
                     output  wire  [WIDTH-1:0]       dout
                   );
  genvar i;

  generate
    for (i=0; i<WIDTH; i=i+1)
    begin
      IOBUF #( .DRIVE (12),            // Specify the output drive strength
               .IBUF_LOW_PWR ("TRUE"), // Low Power - "TRUE", High Performance = "FALSE"
               .IOSTANDARD ("DEFAULT") // Specify the I/O standard
             )
             IOBUF_inst (.O (dout[i]),    // Buffer output     (data to core)
                         .IO (io_pad[i]), // Buffer inout port (To I/O pad)
                         .I (din[i]),     // Buffer input      (core to I/O pad)
                         .T (~oen)       // 3-state enable input, low=tristate hence input, high=output
                        );
    end
  endgenerate
endmodule
