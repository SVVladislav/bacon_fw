`timescale 1 ns / 1 ps

module BACON_PROFILE_v2_0 #
(

)
(
    output reg [511:0] cur_prof,
    output reg [511:0] next_prof,
    input wire en_zond,
    input wire clk_prof,
    output wire en_zond_snc,
    output reg str_pn,
    output wire str_pn_out,
    output reg str_dds,
    output reg str_prd,
    output reg str_prm,
    output reg str_adc,
    output reg clk_adc_div_16,

    output reg [31:0] bram_addr,
    output wire bram_clk,
    input wire [511:0] bram_dout,
    output wire bram_en,
    output wire bram_rst,
    output wire [63:0] bram_web
);

// Подключение BRAM
assign bram_en = 1'b1;
assign bram_rst = 1'b0;
assign bram_web = 64'b0;
assign bram_clk = clk_prof;

reg clk_d2;   // Частота 62,5 МГц (16 нс)
reg clk_d20;  // Частота 6,25 МГц (160 нс)
reg znd_r_d;  // Разрешинеие зондирования

reg [15:0] counter_16ns;  // Счётчик тактов 16 нс
reg [15:0] counter_160ns; // Счётчик тактов 160 нс, считает от 1 до period
reg [15:0] cntr_iter;     // Счётчик итераций

reg profile_valid;
reg [4:0] index_cur_prof;
reg [4:0] index_next_prof;
reg [4:0] index_search_prof;
reg [15:0] search_num_iter;
reg [6:0] searching_counter;
reg prof_searching_mode;

// регистры для выходов стробов
reg str_pn_buf, str_prd_buf, str_dds_buf, str_prm_buf, str_adc_buf;

// Расшифровка профиля
wire [15:0] num_iter = cur_prof[6*8+15 : 6*8];       // itarations
wire [15:0] period = cur_prof[20*8+15 : 20*8];       // period
wire [15:0] dds_start = cur_prof[24*8+15 : 24*8];    // dds_start
wire [15:0] dds_stop = cur_prof[26*8+15 : 26*8];     // dds_stop
wire [15:0] prd_start = cur_prof[28*8+15 : 28*8];    // prd_start
wire [15:0] prd_stop = cur_prof[30*8+15 : 30*8];     // prd_stop
wire [15:0] adc_start = cur_prof[32*8+15 : 32*8];    // adc_start
wire [15:0] adc_stop = cur_prof[34*8+15 : 34*8];     // adc_stop
wire [15:0] prm_start = cur_prof[36*8+15 : 36*8];    // prm_start
wire [15:0] prm_stop = cur_prof[38*8+15 : 38*8];     // prm_stop
wire str_pn_en  = cur_prof[56*8+1];                 // enable str_pn
wire str_prd_en = cur_prof[56*8+2];                 // enable str_prd
wire str_prm_en = cur_prof[56*8+3];                 // enable str_prm

// Сформировать частоту 62,5 МГц (16 нс)
always @( posedge clk_prof )
begin
  clk_d2 <= ~clk_d2;
end

// Свормировать частоту 6,25 МГц (160 нс)
reg [2:0] cnt_d20;
always @( posedge clk_d2 )
begin
  if(znd_r_d == 1'b1) begin
    if(cnt_d20 == 3'd4) begin
      cnt_d20 <= 0;
      clk_d20 <= ~clk_d20;
    end
    else begin
      cnt_d20 <= cnt_d20 + 1;
    end
  end
  else begin
    cnt_d20 <= 0;
    clk_d20 <= 0;
  end
end

// Счётчик такотов по 16 нс
always @( posedge clk_d2 )
begin
  if((znd_r_d == 1'b1) && (counter_160ns < period) && (counter_16ns<=65535))
    counter_16ns <= counter_16ns + 1;
  else
    counter_16ns <= 0;
end

// Счётчик тактов по 160 нс, считает от 1 до period
// Счётчик итераций, считает от 1 до num_iter
always @( posedge clk_d20 )
begin
  if(znd_r_d == 1'b1) begin
    if(counter_160ns < period) counter_160ns <= counter_160ns + 1;
    else begin
      counter_160ns <= 1;
      if(cntr_iter < num_iter) cntr_iter <= cntr_iter + 1;
      else cntr_iter<=1;
    end
  end
  else begin
    cntr_iter <= 1;
    counter_160ns <= 1;
  end
end

// Выход разрешения зондирования
always @( posedge clk_d2 )
begin
  // Если после запроса начала зондирования найден правильный профиль, то разрешить зондирование
  if((en_zond == 1'b1) && (profile_valid==1'b1)) znd_r_d <= 1'b1;
  else znd_r_d <= 1'b0;
end
assign en_zond_snc = znd_r_d;


// Сформировать STR_PRD
always @( posedge clk_d2 )
begin
  if((znd_r_d == 1'b1) && (num_iter != 0) && (counter_16ns > prd_start) && (counter_16ns <= prd_stop))
    str_prd_buf <= 1'b1;
  else
    str_prd_buf <= 1'b0;
end

// Сформировать STR_DDS
always @( posedge clk_d2 )
begin
  if((znd_r_d == 1'b1) && (num_iter != 0) && (counter_16ns > dds_start) && (counter_16ns <= dds_stop))
    str_dds_buf <= 1'b1;
  else
    str_dds_buf <= 1'b0;
end

// Сформировать STR_PRM
always @( posedge clk_d20 )
begin
  if((znd_r_d == 1'b1) && (num_iter != 0) && (counter_160ns > prm_start) && (counter_160ns <= prm_stop))
    str_prm_buf <= 1'b1;
  else
    str_prm_buf <= 1'b0;
end

// Сформировать STR_ADC
always @( posedge clk_d20 )
begin
  if((znd_r_d == 1'b1) && (num_iter != 0) && (counter_160ns > adc_start) && (counter_160ns <= adc_stop))
    str_adc_buf <= 1'b1;
  else
    str_adc_buf <= 1'b0;
end

// Сформировать STR_PN
always @( posedge clk_d2 )
begin
  if((znd_r_d == 1'b1) && (num_iter != 0) && (cntr_iter==1) && (counter_16ns != 0) && (counter_16ns < 163) )
    str_pn_buf <= 1'b1;
  else
    str_pn_buf <= 1'b0;
end

// ======================================
// Автомат поиска и переключения профилей
// ======================================
always @( posedge clk_d2 )
begin
  if(en_zond==1) begin
    if (znd_r_d == 0) begin
      // Начало: Поиск корректного профиля перед разрешением зондирования
      if(prof_searching_mode == 1) begin
	    if(searching_counter<64) begin
          if(searching_counter[0]==0) begin
            // чётные такты
            search_num_iter <= bram_dout[6*8+15 : 6*8];
            bram_addr <= (searching_counter[5:1]+1) << 6;
          end
          else begin
            // нечётные такты
            if(search_num_iter!=0) begin
              // Нашли активный профиль
              profile_valid <= 1;
              prof_searching_mode <= 0;
              index_next_prof <= searching_counter[5:1];
              index_cur_prof <= searching_counter[5:1];
			  cur_prof <= bram_dout;
			  next_prof <= bram_dout;
            end
          end
          searching_counter <= searching_counter + 1;
        end
        else begin
          // ничего не нашли
          prof_searching_mode <= 0;
          searching_counter <= 0;
        end
      end
      // Конец: Поиск корректного профиля перед разрешением зондирования
    end
    else begin
      // Во время зондирования  
      if (prof_searching_mode == 1) begin
        // Тут поиск следующего профиля во вреимя зондирования (в стробе ПН)				
        case (searching_counter)
          0: begin
              cur_prof <= bram_dout;
              bram_addr <= index_search_prof << 6;
			  //index_search_prof <= index_search_prof + 1;
             end
          63: begin
                // ничего больше не нашли, остаёмся на месте
                prof_searching_mode <= 0;
                bram_addr <= index_next_prof <<6;
                searching_counter <= 0;
              end
          default: begin
                    if(searching_counter[0]==1) begin
                      // нечётные такты
                      search_num_iter <= bram_dout[6*8+15 : 6*8];
                    end
                    else begin
                      // чётные такты
                      if(search_num_iter!=0) begin
                        // Нашли активный профиль
                        prof_searching_mode <= 0;
                        index_next_prof <= index_search_prof;
                        next_prof <= bram_dout;
                        //bram_addr <= index_search_prof <<6;
                      end
					  else begin
					    bram_addr <= ((index_search_prof+1)&5'h1f) << 6;
					    index_search_prof <= index_search_prof + 1;				  
					  end 
                    end
                  end
        endcase
        searching_counter <= searching_counter + 1;
		
      end
      else begin
        if((znd_r_d == 1'b1) && (cntr_iter==1) && (counter_16ns == 1))
        begin
          // переходим в режим поиска
          prof_searching_mode <= 1;
          searching_counter <= 0;          
          index_cur_prof <= index_next_prof;
          bram_addr <= index_next_prof << 6;
		  index_search_prof <= index_next_prof + 1;
        end
		//else next_prof <= bram_dout;
      end
    end
  end
  else begin
    // Тут запрос на зондирование выключен
    bram_addr <= 0;
    index_cur_prof <= 0;
    index_next_prof <= 0;
    profile_valid <= 0;
    prof_searching_mode <= 1;
    searching_counter <= 0;	
  end
end

always @( posedge clk_prof )
begin
  str_pn  <= str_pn_buf;
  str_dds <= str_dds_buf;
  str_adc <= str_adc_buf;
  str_prd <= str_prd_buf & str_prd_en;
  str_prm <= str_prm_buf & str_prm_en;
  clk_adc_div_16 <= clk_d2;
end
assign str_pn_out = str_pn & str_pn_en;

endmodule


// Не сделана точная подстройка строба АЦП. Сделать при необходимости в реальности
