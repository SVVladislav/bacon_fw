`timescale 1 ns / 1 ps

module SVV_LMB_SPI_MASTER_v1_0 #
(
  parameter integer SLAVES = 1,
  parameter integer ADDRES = 32'hC3000000
)
(
  // Шина LMB
  input wire [31 : 0] LMB_ABus,
  input wire LMB_AddrStrobe,
  input wire [3 : 0] LMB_BE,
  output wire Sl_CE,
  output wire [31 : 0]  Sl_DBus,
  input wire LMB_ReadStrobe,
  output wire Sl_Ready,
  output wire Sl_UE,
  output wire Sl_Wait,
  input wire [31 : 0] LMB_WriteDBus,
  input wire LMB_WriteStrobe,
  input wire  slmb_aclk,
  input wire  slmb_aresetn,

  // Выходы SPI
  output wire MOSI,
  output wire SCLK,
  output wire [SLAVES-1 : 0] SS,

  input wire F_IN
);

  localparam DATA_WIDTH  = 32;

  reg [DATA_WIDTH-1 : 0]  SL_DBus;
  assign Sl_DBus = SL_DBus;
  reg SL_Ready;
  assign Sl_Ready = SL_Ready;

  integer  byte_index;
  integer  index;

  // Регистры
  reg [31 : 0 ] CR;
  reg [31 : 0 ] SSR;
  reg [31 : 0 ] DR;

  reg start_action;
  reg action;
  reg [6:0] bits;

  always @(posedge slmb_aclk) begin
    if (slmb_aresetn==1'b0) begin
      // Начальное состояние по ресет
      for(index=0; index<SLAVES; index=index+1) SSR[index] <= 1'b1;
      CR[11:8] <= 4'd7;
    end
    else begin
      if((LMB_AddrStrobe==1'b1) && (LMB_ABus[31:5]==(ADDRES >> 5))) begin
        SL_Ready<=1'b1;

        // Запись регистров
        if (LMB_WriteStrobe==1'b1) begin
          case (LMB_ABus[4:2])
            3'd0 :  // CR
              for ( byte_index = 0; byte_index <= (DATA_WIDTH/8)-1; byte_index = byte_index+1 )
                if ( LMB_BE[byte_index] == 1 )
                  CR[(byte_index*8) +: 8] <= LMB_WriteDBus[(byte_index*8) +: 8];
            3'd1 : begin // DR
                     case (LMB_BE[3:0])
                       4'b0001 : bits<=8;
                       4'b0011 : bits<=16;
                       4'b1111 : bits<=32;
                       default : bits<=8;
                     endcase
                     for ( byte_index = 0; byte_index <= (DATA_WIDTH/8)-1; byte_index = byte_index+1 )
                       if ( LMB_BE[byte_index] == 1 )
                         DR[(byte_index*8) +: 8] <= LMB_WriteDBus[(byte_index*8) +: 8];
                     start_action <= 1;
                   end
            3'd2 : begin // DR24
                     bits<=24;
                     for ( byte_index = 0; byte_index <= (DATA_WIDTH/8)-1; byte_index = byte_index+1 )
                       if ( LMB_BE[byte_index] == 1 )
                         DR[(byte_index*8) +: 8] <= LMB_WriteDBus[(byte_index*8) +: 8];
                     start_action <= 1;
                   end
            3'd3 : begin //DRFS
                     bits<=CR[15:8]+1;
                     for ( byte_index = 0; byte_index <= (DATA_WIDTH/8)-1; byte_index = byte_index+1 )
                       if ( LMB_BE[byte_index] == 1 )
                         DR[(byte_index*8) +: 8] <= LMB_WriteDBus[(byte_index*8) +: 8];
                     start_action <= 1;
                   end
            3'd4 :  // SSR
              for ( byte_index = 0; byte_index <= (DATA_WIDTH/8)-1; byte_index = byte_index+1 )
                if ( LMB_BE[byte_index] == 1 )
                  SSR[(byte_index*8) +: 8] <= LMB_WriteDBus[(byte_index*8) +: 8];
          endcase
        end // (IO_Write_Strobe==1'b1) - Запись регистров

        // Чтение регистров
        if (LMB_ReadStrobe==1'b1) begin
          case (LMB_ABus[4:2])
            3'd0 :  SL_DBus <= { (start_action|action), CR[30:0] };   // Контрольный регистр
            3'd1 :  SL_DBus <= DR;
            3'd4 :  SL_DBus <= SSR;  //
            default : SL_DBus <= 32'h00000000;
          endcase
        end // (IO_Read_Strobe==1'b1) - Чтение регистров

      end // if(IO_Addr_Strobe==1'b1)
      else begin
        if(Sl_Ready==1'b1) begin
          SL_Ready<=1'b0;
          SL_DBus <= 32'h00000000;
        end
        if(action) start_action <= 0;
      end
    end
  end // always slmb_aclk

  reg [8:0] counter = 0;
  reg sclk;
  reg mosi;
  reg [31:0] sh_reg;

  assign SCLK = sclk;
  assign MOSI = sh_reg[bits-1];

  genvar i;
  for(i=0; i<SLAVES; i=i+1)
    assign SS[i] = CR[2]? SSR[i] : ~(SSR[i] & action); // Manual or auto Slave Select

  always @(posedge F_IN) begin
    if(action) begin

      if(counter==0) sh_reg <= DR;		
      else 
        if((counter & 1'b1)==1'b0) sh_reg <= sh_reg<<1;
	 
	 if( CR[0] == 1) begin
	   // CPHA=1
	   if( ((counter & 1'b1)==0) && (counter<(bits<<1)) ) sclk <= ~CR[1]; else  sclk <= CR[1];
	 end
	 else begin
	   // CPHA=0
	   if(counter & 1'b1) sclk <= ~CR[1]; else  sclk <= CR[1];
	 end
	 
      if(counter==(bits<<1)) begin // Конец работы
        action<=0;
        counter<=0;
      end
      else begin
        counter <= counter+1;
      end

    end
    else 	
      if(start_action) begin 
        action<=1; 
        sclk <= CR[1]; 
      end
  end // always F_CLK

endmodule
