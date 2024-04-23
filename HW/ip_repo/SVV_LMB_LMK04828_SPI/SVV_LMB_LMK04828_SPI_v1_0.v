`timescale 1 ns / 1 ps

module SVV_LMB_LMK04828_SPI_v1_0 #
(
  parameter integer ADDRES = 32'hC4000000
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

  // Подключение LMK04828
  inout wire SDIO,
  output wire SCLK,
  output wire nCS,
  output wire RESET,
  output wire SYNC
);

  localparam DATA_WIDTH  = 32;

  reg [DATA_WIDTH-1 : 0]  SL_DBus;
  assign Sl_DBus = SL_DBus;
  reg SL_Ready;
  assign Sl_Ready = SL_Ready;
  integer  byte_index;

  reg f_d2;
  always @(posedge slmb_aclk) begin
    f_d2 <= ~f_d2;
  end
  
  reg f_d4;
  always @(posedge f_d2) begin
    f_d4 <= ~f_d4;
  end


  // Регистры
  reg [31 : 0 ] TDR;
  reg [31 : 0 ] RDR;
  
  reg start_action;
  reg action;
  
  reg [8:0] counter = 0;
  reg sclk;
  reg sdio_dir;  // Уаправление направлением выходного буфера на SDIO
  reg [31:0] sh_reg;

  assign SCLK = sclk;
  assign RESET = 0;
  assign SYNC = 0;   
  assign nCS = ~action;  
  wire MISO;

  always @(posedge slmb_aclk) begin
    if (slmb_aresetn==1'b0) begin
      // Начальное состояние по ресет
      TDR <= 0;
    end
    else begin
      if((LMB_AddrStrobe==1'b1) && (LMB_ABus[31:5]==(ADDRES >> 5))) begin
        SL_Ready<=1'b1;
		
        // Запись регистров
        if (LMB_WriteStrobe==1'b1) begin
          case (LMB_ABus[4:2])
		    3'd0 : begin
                    for ( byte_index = 0; byte_index <= (DATA_WIDTH/8)-1; byte_index = byte_index+1 )
                       if ( LMB_BE[byte_index] == 1 )
                         TDR[(byte_index*8) +: 8] <= LMB_WriteDBus[(byte_index*8) +: 8];					
                    start_action <= 1;
					end
			default: begin 
			         end			
          endcase
        end // (IO_Write_Strobe==1'b1) - Запись регистров

        // Чтение регистров
        if (LMB_ReadStrobe==1'b1) begin
          case (LMB_ABus[4:2])
            3'd0 :  SL_DBus <= TDR;
			3'd1 :  SL_DBus <= { TDR[23:8], RDR[7:0] };
			3'd2 :  SL_DBus <= { (start_action|action) };   // Контрольный регистр
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

  always @(posedge f_d4) begin
    if(action) begin

      if(counter==0) sh_reg <= TDR;
      else 
        if((counter & 1'b1)==1'b0) sh_reg <= sh_reg<<1;
		
	  if(counter & 1'b1) sclk <= 1; else  sclk <= 0;
	  
	  if((counter & 1'b1)==0) RDR <= (RDR<<1) | MISO;
	  
	  if((counter < 32) || (counter==(24<<1)) || (TDR[23] == 0)) sdio_dir <= 0; else sdio_dir<=1;
	  
      if(counter==(24<<1)) begin // Конец работы   
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
        sclk <= 0; 
      end
  end // always F_CLK
  
  
  // Двунаправленный буфер SDIO
  IOBUF i_iobuf_sdio (
    .T (sdio_dir),
    .I (sh_reg[24-1]),  // Выход сдвигового регистра в качестве MOSI
    .O (MISO),
    .IO (SDIO));
  
endmodule
