library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity BCO_PROFILE_v2_0 is
	generic (
		-- Users to add parameters here

		-- User parameters ends
		-- Do not modify the parameters beyond this line


		-- Parameters of Axi Slave Bus Interface S00_AXI
		C_S00_AXI_DATA_WIDTH	: integer	:= 32;
		C_S00_AXI_ADDR_WIDTH	: integer	:= 8
	);
	port (
		-- Users to add ports here
        en_zond         : in std_logic;
        mac_prof        : out std_logic_vector(1023 downto 0);
		cur_prof        : out std_logic_vector(1023 downto 0);
        nxt_prof        : out std_logic_vector(1023 downto 0);
        en_zond_snc     : out std_logic;
        str_pn          : out std_logic;
        str_pn_out      : out std_logic;
		str_dds         : out std_logic;
        str_prd         : out std_logic;
		str_prm         : out std_logic;
		str_adc         : out std_logic;
--        clk             : in std_logic;
        clk_prof        : in std_logic;
        clk_adc_div_16  : out std_logic;
--        clk_dds_sinh    : out std_logic;
                -- User ports ends
		-- Do not modify the ports beyond this line


		-- Ports of Axi Slave Bus Interface S00_AXI
		s00_axi_aclk	: in std_logic;
		s00_axi_aresetn	: in std_logic;
		s00_axi_awaddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_awprot	: in std_logic_vector(2 downto 0);
		s00_axi_awvalid	: in std_logic;
		s00_axi_awready	: out std_logic;
		s00_axi_wdata	: in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_wstrb	: in std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0);
		s00_axi_wvalid	: in std_logic;
		s00_axi_wready	: out std_logic;
		s00_axi_bresp	: out std_logic_vector(1 downto 0);
		s00_axi_bvalid	: out std_logic;
		s00_axi_bready	: in std_logic;
		s00_axi_araddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_arprot	: in std_logic_vector(2 downto 0);
		s00_axi_arvalid	: in std_logic;
		s00_axi_arready	: out std_logic;
		s00_axi_rdata	: out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_rresp	: out std_logic_vector(1 downto 0);
		s00_axi_rvalid	: out std_logic;
		s00_axi_rready	: in std_logic
	);
end BCO_PROFILE_v2_0;

architecture arch_imp of BCO_PROFILE_v2_0 is

	-- component declaration
	component BCO_PROFILE_v2_0_S00_AXI is
		generic (
		C_S_AXI_DATA_WIDTH	: integer	:= 32;
		C_S_AXI_ADDR_WIDTH	: integer	:= 8
		);
		port (
		
        --index_mac       : in std_logic_vector(7 downto 0);
        --prof_mac        : out std_logic_vector(1023 downto 0);
        index_cur       : in std_logic_vector(7 downto 0);
        prof_cur        : out std_logic_vector(1023 downto 0);
        index_nxt       : in std_logic_vector(7 downto 0);
        prof_nxt        : out std_logic_vector(1023 downto 0);
        
        S_AXI_ACLK	    : in std_logic;
		S_AXI_ARESETN	: in std_logic;
		S_AXI_AWADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_AWPROT	: in std_logic_vector(2 downto 0);
		S_AXI_AWVALID	: in std_logic;
		S_AXI_AWREADY	: out std_logic;
		S_AXI_WDATA	: in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_WSTRB	: in std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
		S_AXI_WVALID	: in std_logic;
		S_AXI_WREADY	: out std_logic;
		S_AXI_BRESP	: out std_logic_vector(1 downto 0);
		S_AXI_BVALID	: out std_logic;
		S_AXI_BREADY	: in std_logic;
		S_AXI_ARADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_ARPROT	: in std_logic_vector(2 downto 0);
		S_AXI_ARVALID	: in std_logic;
		S_AXI_ARREADY	: out std_logic;
		S_AXI_RDATA	: out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_RRESP	: out std_logic_vector(1 downto 0);
		S_AXI_RVALID	: out std_logic;
		S_AXI_RREADY	: in std_logic
		);
	end component BCO_PROFILE_v2_0_S00_AXI;
    
    signal cntr_prof        : integer range 0 to 63 := 0;
    signal cntr_exact       : integer range 0 to 255 := 0;
    signal index_cur        : integer range 0 to 63 := 0;
    signal index_nxt        : integer range 0 to 63 := 1;
    signal index_start      : integer range 0 to 63 := 0;
    signal index_mac        : integer range 0 to 63 := 0;
    signal cntr_iter        : integer range 0 to 65535 := 0;
    signal cntr_start      : integer range 0 to 63 := 1;
    signal num_iter         : integer range 0 to 65535 := 0;
    signal nxt_num_iter     : integer range 0 to 65535 := 0;
    signal start_num_iter   : integer range 0 to 65535 := 0;
    signal cntr_clk10       : integer range 0 to 65535 := 0;
    signal cntr_clk0        : integer range 0 to 65535 := 0;
    signal cntr_clk1        : integer range 0 to 9 := 0;
    signal period           : integer range 0 to 65535 := 0;
        
    
    signal str_adc_buf      : std_logic := '0';
    signal str_adc_buf_z1   : std_logic := '0';
    signal str_adc_buf_r    : std_logic := '0';
    signal str_adc_exact    : std_logic := '0';
    signal str_pn_buf       : std_logic := '0';
    signal str_pn_buf_d1    : std_logic := '0';
    signal index_mac_slv    : std_logic_vector(7 downto 0) := x"00";
    signal index_cur_slv    : std_logic_vector(7 downto 0) := x"00";
    signal index_nxt_slv    : std_logic_vector(7 downto 0) := x"00";
    signal prof_index       : std_logic_vector(7 downto 0) := x"00";
    signal cur_prof_buf     : std_logic_vector(1023 downto 0) := (others => '0');
    signal cur_prof_buf_d1  : std_logic_vector(1023 downto 0) := (others => '0');
    signal nxt_prof_buf     : std_logic_vector(1023 downto 0) := (others => '0');
    signal clk1             : std_logic := '0';
    signal clk1_z1          : std_logic := '0';
    signal clk1_rse         : std_logic := '0';
    signal rse_pn           : std_logic := '0';
    signal rse_pn_z1        : std_logic := '0';
    signal rse_pn_z2        : std_logic := '0';
    signal clk_d2           : std_logic := '0';
    signal znd_r_d          : std_logic := '0';
    signal znd_r_d1         : std_logic := '0';
    signal znd_r_d2         : std_logic := '0';
    signal str_cntr_clk10   : std_logic := '0';
    signal str_cntr_clk10_r_d : std_logic := '0';
    signal str_cntr_clk10_rise : std_logic := '0';
    signal str_prd_buf         : std_logic := '0';
    signal str_prm_buf         : std_logic := '0';
    
    
begin

-- Instantiation of Axi Bus Interface S00_AXI
BCO_PROFILE_v2_0_S00_AXI_inst : BCO_PROFILE_v2_0_S00_AXI
	generic map (
		C_S_AXI_DATA_WIDTH	=> C_S00_AXI_DATA_WIDTH,
		C_S_AXI_ADDR_WIDTH	=> C_S00_AXI_ADDR_WIDTH
	)
	port map (
		--index_mac(7 downto 0)   => index_mac_slv(7 downto 0),
        --prof_mac(1023 downto 0) => mac_prof(1023 downto 0),
		index_cur(7 downto 0)   => index_cur_slv(7 downto 0),
        prof_cur(1023 downto 0) => cur_prof_buf(1023 downto 0),
		index_nxt(7 downto 0)   => index_nxt_slv(7 downto 0),
        prof_nxt(1023 downto 0) => nxt_prof_buf(1023 downto 0),
          
		S_AXI_ACLK	    => s00_axi_aclk,
		S_AXI_ARESETN	=> s00_axi_aresetn,
		S_AXI_AWADDR	=> s00_axi_awaddr,
		S_AXI_AWPROT	=> s00_axi_awprot,
		S_AXI_AWVALID	=> s00_axi_awvalid,
		S_AXI_AWREADY	=> s00_axi_awready,
		S_AXI_WDATA	    => s00_axi_wdata,
		S_AXI_WSTRB	    => s00_axi_wstrb,
		S_AXI_WVALID	=> s00_axi_wvalid,
		S_AXI_WREADY	=> s00_axi_wready,
		S_AXI_BRESP	    => s00_axi_bresp,
		S_AXI_BVALID	=> s00_axi_bvalid,
		S_AXI_BREADY	=> s00_axi_bready,
		S_AXI_ARADDR	=> s00_axi_araddr,
		S_AXI_ARPROT	=> s00_axi_arprot,
		S_AXI_ARVALID	=> s00_axi_arvalid,
		S_AXI_ARREADY	=> s00_axi_arready,
		S_AXI_RDATA	    => s00_axi_rdata,
		S_AXI_RRESP	    => s00_axi_rresp,
		S_AXI_RVALID	=> s00_axi_rvalid,
		S_AXI_RREADY	=> s00_axi_rready
	);

	-- Add user logic here
	index_mac_slv(7 downto 0) <= STD_LOGIC_VECTOR(TO_UNSIGNED(0,8));
	index_cur_slv(7 downto 0) <= STD_LOGIC_VECTOR(TO_UNSIGNED(index_cur,8));--index_cur
	index_nxt_slv(7 downto 0) <= STD_LOGIC_VECTOR(TO_UNSIGNED(index_nxt,8));--index_nxt
	cur_prof(1023 downto 0)   <= cur_prof_buf_d1(1023 downto 0);
	nxt_prof(1023 downto 0)   <= nxt_prof_buf(1023 downto 0);
	
	period       <= to_integer(unsigned(cur_prof_buf_d1((74*8-1) downto (74-2)*8)));
	num_iter     <= to_integer(unsigned(cur_prof_buf_d1((60*8-1) downto (60-2)*8)));
	start_num_iter <= to_integer(unsigned(cur_prof_buf((60*8-1) downto (60-2)*8)));
	nxt_num_iter <= to_integer(unsigned(nxt_prof_buf((60*8-1) downto (60-2)*8)));
	
	process(clk_prof) 
    begin
        if rising_edge(clk_prof) then
            clk_d2 <= not clk_d2;
           end if;
    end process; 
 
	process(clk_prof) 
    begin
        if rising_edge(clk_prof) then
            clk_adc_div_16 <= clk_d2;
           end if;
    end process; 
    
    process(clk_d2) 
    begin
        if rising_edge(clk_d2) then
            if en_zond = '1' then
                if cntr_start < 31 then 
                    if start_num_iter = 0 then
                        cntr_start <= cntr_start + 1; 
                    end if;
                end if;
            else  cntr_start <= 0;  end if;
        end if;
    end process;

    process(clk_d2) 
    begin
        if rising_edge(clk_d2) then
            if en_zond = '1' then
                if znd_r_d = '0' then
                    if start_num_iter > 0 then
                        znd_r_d <= '1'; 
                    end if;
                end if; 
            else znd_r_d <= '0'; end if;
        end if;
    end process;
    en_zond_snc <= znd_r_d;
    
	process( clk_d2 ) is--импульс по нарастающему фронту строба DDS
    begin
        if (rising_edge (clk_d2)) then
            if cntr_clk10 = 0 then
                str_cntr_clk10 <= '1'; 
            else str_cntr_clk10 <= '0'; end if;
        end if;    
    end process;    
	
	process( clk_d2 ) is--импульс по нарастающему фронту строба DDS
    begin
        if (rising_edge (clk_d2)) then
            str_cntr_clk10_r_d <= str_cntr_clk10;
            str_cntr_clk10_rise <= str_cntr_clk10 and not str_cntr_clk10_r_d;
        end if;    
    end process;    

    process(clk_d2) --счетчик профилей
    begin
        if rising_edge(clk_d2) then
            if znd_r_d = '1' then
                if rse_pn_z1 = '0' then
                    if nxt_num_iter = 0 then
                        if index_nxt = 31 then
                            index_nxt <= 0;
                        else index_nxt <= index_nxt + 1; end if;
                    end if;
                else
                    if index_nxt = 31 then
                        index_nxt <= 0;
                    else index_nxt <= index_nxt + 1; end if;                
                end if;
            else index_nxt <= cntr_start; end if;
        end if;
    end process;
	
	process( clk_d2 ) --установка текущего профиля по фронту строба ПН str_pn_buf, znd_r_d
    begin
        if rising_edge(clk_d2) then
            if en_zond = '1' then 
                if znd_r_d = '1' then
                    if rse_pn = '1' then
                        if nxt_num_iter > 0 then
                            index_cur <= index_nxt; -- index_nxt всегда 0
                        end if;
                    end if;
                else
                   if num_iter = 0 then
                        if index_cur < 31 then
                            index_cur <= index_cur + 1;
                        end if;
                    end if;
                end if;
            else index_cur <= cntr_start; end if;
        end if;
    end process;

	process(clk_d2)--clk_d2 --установка текущего профиля по фронту строба ПН
    begin
        if znd_r_d = '1' then
            if rising_edge(clk_d2) then
                if rse_pn = '1' then
                    if nxt_num_iter > 0 then
                        cur_prof_buf_d1(1023 downto 0) <= nxt_prof_buf(1023 downto 0); -- всегда 0
                    end if;
                end if;
            end if;
        else cur_prof_buf_d1(1023 downto 0) <= cur_prof_buf(1023 downto 0); end if;
    end process;	

	process( clk_d2 ) is --импульс по нарастающему фронту строба ADC
    begin
        if (rising_edge (clk_d2)) then
            str_adc_buf_z1 <= str_adc_buf;
            str_adc_buf_r <= str_adc_buf and not str_adc_buf_z1;
        end if;    
    end process;    
	
	process(clk_d2) --установка текущего профиля по фронту строба ПРМ
    begin
        if rising_edge(clk_d2) then
            if str_adc_buf_r = '1' then
                mac_prof(1023 downto 0) <= cur_prof_buf_d1(1023 downto 0);--index_mac <= index_cur;
            end if;
        end if;
    end process;
    
    process(clk_d2) --счетчик тактового сигнала высокой частоты
    begin
        if znd_r_d = '1' then
            if rising_edge(clk_d2) then
                if cntr_clk10 >= period then
                    cntr_clk0 <= 0;
                else
                    if cntr_iter >= num_iter then
                        cntr_clk0 <= 0;
                    else
                        if cntr_clk0 < 65535 then
                            cntr_clk0 <= cntr_clk0 + 1;
                        end if;
                    end if;
                end if;
            end if;
        else cntr_clk0 <= 0; end if;
    end process;
    
    
    process(clk_d2) --счетчик тактового сигнала для формирователя тактового сигнала сниженной в 10 раз частоты
    begin
        if znd_r_d = '1' then
            if rising_edge(clk_d2) then
                if cntr_clk1 = 4 then
                    cntr_clk1 <= 0;
                else cntr_clk1 <= cntr_clk1 + 1; end if;
            end if;
        else cntr_clk1 <= 0; end if;
    end process;
    
    process(clk_d2) --формирователь тактового сигнала сниженной в 10 раз частоты
    begin
        if znd_r_d = '1' then
            if rising_edge(clk_d2) then
                if cntr_clk1 = 4 then
                    clk1 <= not clk1;
                end if;
            end if;
        else clk1 <= '1'; end if;
    end process;
    
    process(clk1) --счетчик тактов пониженной частоты 
    begin
        if rising_edge(clk1) then
            if znd_r_d = '1' then
                if cntr_clk10 >= period then
                    cntr_clk10 <= 0;
                else
                    if cntr_iter >= num_iter then
                        cntr_clk10 <= 0;
                    else
                        if cntr_clk10 < 65535 then
                            cntr_clk10 <= cntr_clk10 + 1;
                        end if;
                    end if; 
                end if;
            else cntr_clk10 <= 0; end if;
        end if;
    end process;

    process(clk1) --счетчик зондирований
    begin
        if znd_r_d = '1' then
            if rising_edge(clk1) then
                if cntr_clk10 >= period then
                    if cntr_iter >= (num_iter - 1) then
                        cntr_iter <= 0;
                    else cntr_iter <= cntr_iter + 1; end if;
                end if;
            end if;
        else cntr_iter <= 0; end if;
    end process;

    process(clk_d2) --фронт строба перенацеливания
    begin
        if rising_edge(clk_d2) then
            str_pn_buf_d1 <= str_pn_buf;
            rse_pn <= str_pn_buf and (not str_pn_buf_d1);
            rse_pn_z1 <= rse_pn;
            rse_pn_z2 <= rse_pn_z1;
        end if;
    end process;
    
    process (clk_d2)
    begin
        if rising_edge(clk_d2) then
            if (znd_r_d = '1' and cntr_iter = 0 and num_iter > 0) then
                if  (cntr_clk0 > 0)  and (cntr_clk0 < 163) then
                    str_pn_buf <= '1'; 
                else str_pn_buf <= '0'; end if;
            else str_pn_buf <= '0'; end if;
        end if;
    end process;    
    str_pn <= str_pn_buf when rising_edge(clk_d2);
    str_pn_out <= str_pn_buf and cur_prof_buf_d1(865) when rising_edge(clk_d2);
    
    
    process (clk_d2)
    begin
        if rising_edge(clk_d2) then
            if znd_r_d = '1' then
                if num_iter > 0 then
                    if  cntr_clk0 > to_integer(unsigned(cur_prof_buf_d1((82*8-1) downto (82-2)*8)))and
                        cntr_clk0 <=  to_integer(unsigned(cur_prof_buf_d1((84*8-1) downto (84-2)*8)))then
                        str_prd_buf <= '1'; 
                    else str_prd_buf <= '0'; end if;
                else str_prd_buf <= '0'; end if;
            else str_prd_buf <= '0'; end if;
        end if;
    end process;    
    str_prd <= str_prd_buf and cur_prof_buf_d1(866) when rising_edge(clk_d2);
    
    process (clk_d2)
    begin
        if rising_edge(clk_d2) then
            if znd_r_d = '1' then
                if num_iter > 0 then
                    if  cntr_clk0 > to_integer(unsigned(cur_prof_buf_d1((78*8-1) downto (78-2)*8)))and
                        cntr_clk0 <=  to_integer(unsigned(cur_prof_buf_d1((80*8-1) downto (80-2)*8)))then
                        str_dds <= '1'; 
                    else str_dds <= '0'; end if;
                else str_dds <= '0'; end if;
            else str_dds <= '0'; end if;
        end if;
    end process;    
    
    process (clk1)
    begin
        if znd_r_d = '1' then
            if rising_edge(clk1) then
                if num_iter > 0 then
                    if  cntr_clk10 > to_integer(unsigned(cur_prof_buf_d1((86*8-1) downto (86-2)*8)))and
                        cntr_clk10 <=  to_integer(unsigned(cur_prof_buf_d1((88*8-1) downto (88-2)*8)))then
                        str_adc_buf <= '1'; 
                    else str_adc_buf <= '0'; end if;
                else str_adc_buf <= '0'; end if;
            end if;
        else str_adc_buf <= '0'; end if;
    end process;    

    process(clk_prof) --фронт тактирующего сигнала 
    begin
        if rising_edge(clk_prof) then
            clk1_z1 <= clk1;
            clk1_rse <= (clk1 and (not clk1_z1));
        end if;
    end process;

    process (clk_prof) --счетчик в тактирующем сигнале
    begin
        if rising_edge(clk_prof) then
            if clk1_rse = '1' then   
                cntr_exact <= 0;
            else    
                if cntr_exact < 255 then
                    cntr_exact <= cntr_exact + 1;
                end if;
            end if;
        end if;
    end process;    

    process (clk_prof)
    begin
        if rising_edge(clk_prof) then
            if str_adc_buf = '1' then
                if cntr_exact = to_integer(unsigned(cur_prof_buf(113*8-1 downto (113-1)*8))) then --to_integer(unsigned(cur_prof_buf(113*8-1 downto (113-1)*8)))
                    str_adc_exact <= '1'; 
                end if;
            else    
                if cntr_exact = to_integer(unsigned(cur_prof_buf(114*8-1 downto (114-1)*8))) then --to_integer(unsigned(cur_prof_buf(29*32-16-1 downto 28*32+8)))
                    str_adc_exact <= '0'; 
                end if;
            end if;
            str_adc <= str_adc_exact;
        end if;
    end process;    
    --str_adc <= str_adc_exact when clk_prof;

    process (clk1)
    begin
        if znd_r_d = '1' then
            if rising_edge(clk1) then
                if num_iter > 0 then
                    if  cntr_clk10 > to_integer(unsigned(cur_prof_buf_d1((90*8-1) downto (90-2)*8)))and
                        cntr_clk10 <=  to_integer(unsigned(cur_prof_buf_d1((92*8-1) downto (92-2)*8)))then
                        str_prm_buf <= '1'; 
                    else str_prm_buf <= '0'; end if;
                else str_prm_buf <= '0'; end if;
            end if;
        else str_prm_buf <= '0'; end if;
    end process;  
    str_prm <= str_prm_buf and cur_prof_buf_d1(867) when rising_edge(clk1); 
      
	-- User logic ends

end arch_imp;
