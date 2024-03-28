`timescale 1 ns / 1 ps

module serial_buf_config_v1_0 #
(
  parameter MODE = "Half Duplex" 
)
(
  input wire in_DI,
  input wire in_DE,
  input wire in_RE,
  output wire out_RO,
  
  output wire DI,
  output wire DE,
  output wire RE,
  input wire RO
);

generate
  case (MODE)
    "Half Duplex" : begin 
                      assign DI = in_DI;
                      assign DE = in_DE;
                      assign RE = in_RE;
                      assign out_RO = RO;
                    end
     "Full Duplex" : begin 
                      assign DI = in_DI;
                      assign DE = 1'b1;
                      assign RE = 1'b0;
                      assign out_RO = RO;
                    end
     "Transmit Only" : begin 
                         assign DI = in_DI;
                         assign DE = 1'b1;
                         assign RE = 1'b1;
                         assign out_RO = RO;
                       end
     "Receive Only" : begin 
                        assign DI = in_DI;
                        assign DE = 1'b0;
                        assign RE = 1'b0;
                        assign out_RO = RO;
                      end
     "Unused" : begin 
                  assign DI = in_DI;
                  assign DE = 1'b0;
                  assign RE = 1'b1;
                  assign out_RO = RO;
                end                                               
  endcase
endgenerate

endmodule
