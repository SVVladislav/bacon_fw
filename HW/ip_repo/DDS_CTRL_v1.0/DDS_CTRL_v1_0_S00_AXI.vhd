library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity DDS_CTRL_v1_0_S00_AXI is
	generic (
		-- Users to add parameters here

		-- User parameters ends
		-- Do not modify the parameters beyond this line

		-- Width of S_AXI data bus
		C_S_AXI_DATA_WIDTH	: integer	:= 32;
		-- Width of S_AXI address bus
		C_S_AXI_ADDR_WIDTH	: integer	:= 6
	);
	port (
		-- Users to add ports here
		NXT_PROF        : in std_logic_vector(1023 downto 0);
		CUR_PROF        : in std_logic_vector(1023 downto 0);
        en_zond         : in std_logic;
        str_pn          : in std_logic;
        str_prd         : in std_logic;
        clk_dds         : in std_logic;
--        dds_io_upd_sinh : in std_logic;
        
        D               : out std_logic_vector(31 downto 0);
        F               : out std_logic_vector(3 downto 0);
        PS              : out std_logic_vector(2 downto 0);
        pwdwn           : out std_logic := '1';
        rst             : out std_logic := '1';
        osk_in          : in std_logic;
        osk_out         : out std_logic;
        drctrl_in       : in std_logic;
        drctrl_out      : out std_logic;
        drhold_in       : in std_logic;
        drhold_out      : out std_logic;
        drover_in       : in std_logic;
        drover_out      : out std_logic;
        io_upd_in       : in std_logic;
        io_upd_out      : out std_logic;
        sinc_out        : out std_logic;
        sinc_in         : in std_logic;
        dir             : out std_logic;
                
		-- User ports ends
		-- Do not modify the ports beyond this line

		-- Global Clock Signal
		S_AXI_ACLK	: in std_logic;
		-- Global Reset Signal. This Signal is Active LOW
		S_AXI_ARESETN	: in std_logic;
		-- Write address (issued by master, acceped by Slave)
		S_AXI_AWADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		-- Write channel Protection type. This signal indicates the
    		-- privilege and security level of the transaction, and whether
    		-- the transaction is a data access or an instruction access.
		S_AXI_AWPROT	: in std_logic_vector(2 downto 0);
		-- Write address valid. This signal indicates that the master signaling
    		-- valid write address and control information.
		S_AXI_AWVALID	: in std_logic;
		-- Write address ready. This signal indicates that the slave is ready
    		-- to accept an address and associated control signals.
		S_AXI_AWREADY	: out std_logic;
		-- Write data (issued by master, acceped by Slave) 
		S_AXI_WDATA	: in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		-- Write strobes. This signal indicates which byte lanes hold
    		-- valid data. There is one write strobe bit for each eight
    		-- bits of the write data bus.    
		S_AXI_WSTRB	: in std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
		-- Write valid. This signal indicates that valid write
    		-- data and strobes are available.
		S_AXI_WVALID	: in std_logic;
		-- Write ready. This signal indicates that the slave
    		-- can accept the write data.
		S_AXI_WREADY	: out std_logic;
		-- Write response. This signal indicates the status
    		-- of the write transaction.
		S_AXI_BRESP	: out std_logic_vector(1 downto 0);
		-- Write response valid. This signal indicates that the channel
    		-- is signaling a valid write response.
		S_AXI_BVALID	: out std_logic;
		-- Response ready. This signal indicates that the master
    		-- can accept a write response.
		S_AXI_BREADY	: in std_logic;
		-- Read address (issued by master, acceped by Slave)
		S_AXI_ARADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		-- Protection type. This signal indicates the privilege
    		-- and security level of the transaction, and whether the
    		-- transaction is a data access or an instruction access.
		S_AXI_ARPROT	: in std_logic_vector(2 downto 0);
		-- Read address valid. This signal indicates that the channel
    		-- is signaling valid read address and control information.
		S_AXI_ARVALID	: in std_logic;
		-- Read address ready. This signal indicates that the slave is
    		-- ready to accept an address and associated control signals.
		S_AXI_ARREADY	: out std_logic;
		-- Read data (issued by slave)
		S_AXI_RDATA	: out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		-- Read response. This signal indicates the status of the
    		-- read transfer.
		S_AXI_RRESP	: out std_logic_vector(1 downto 0);
		-- Read valid. This signal indicates that the channel is
    		-- signaling the required read data.
		S_AXI_RVALID	: out std_logic;
		-- Read ready. This signal indicates that the master can
    		-- accept the read data and response information.
		S_AXI_RREADY	: in std_logic

        
		
	);
end DDS_CTRL_v1_0_S00_AXI;

architecture arch_imp of DDS_CTRL_v1_0_S00_AXI is

	-- AXI4LITE signals
	signal axi_awaddr	: std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
	signal axi_awready	: std_logic;
	signal axi_wready	: std_logic;
	signal axi_bresp	: std_logic_vector(1 downto 0);
	signal axi_bvalid	: std_logic;
	signal axi_araddr	: std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
	signal axi_arready	: std_logic;
	signal axi_rdata	: std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal axi_rresp	: std_logic_vector(1 downto 0);
	signal axi_rvalid	: std_logic;

	-- Example-specific design signals
	-- local parameter for addressing 32 bit / 64 bit C_S_AXI_DATA_WIDTH
	-- ADDR_LSB is used for addressing 32/64 bit registers/memories
	-- ADDR_LSB = 2 for 32 bits (n downto 2)
	-- ADDR_LSB = 3 for 64 bits (n downto 3)
	constant ADDR_LSB  : integer := (C_S_AXI_DATA_WIDTH/32)+ 1;
	constant OPT_MEM_ADDR_BITS : integer := 3;
	------------------------------------------------
	---- Signals for user logic register space example
	--------------------------------------------------
	---- Number of Slave Registers 16
	signal slv_reg0	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg1	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg2	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg3	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg4	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg5	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg6	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg7	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg8	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg9	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg10	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg11	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg12	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg13	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg14	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg15	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg_rden	: std_logic;
	signal slv_reg_wren	: std_logic;
	signal reg_data_out	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
	signal byte_index	: integer;
	signal aw_en	: std_logic;
	
	constant clk_divider  : integer := 0;
	
	signal   siganal_type : std_logic_vector(7 downto 0) := (others => '0');
	signal   siganal_type_nxt : std_logic_vector(7 downto 0) := (others => '0');
	signal   ITER      	  : std_logic_vector (15 downto 0) := (others => '0');
	signal   FTW    	  : std_logic_vector(31 downto 0) := (others => '0');
	signal   DFTW    	  : std_logic_vector(31 downto 0) := (others => '0');
	signal   DFRRW        : std_logic_vector(31 downto 0) := (others => '0');
	signal   NUM_PULSE    : std_logic_vector(15 downto 0) := (others => '0');
	signal   PERIOD       : std_logic_vector(15 downto 0) := (others => '0');
	signal   N_step       : std_logic_vector(15 downto 0) := (others => '0');
	signal   dds_start    : std_logic_vector(15 downto 0) := (others => '0');
	signal   dds_stop     : std_logic_vector(15 downto 0) := (others => '0');
	signal   ctrl_reg	  : std_logic_vector(31 downto 0) := (others => '0');
	signal   data_reg	  : std_logic_vector(31 downto 0) := (others => '0');
	signal   PS_buf	      : std_logic_vector(2 downto 0) := (others => '0');
	signal   addr_buf     : std_logic_vector(3 downto 0) := (others => '0');
	signal   UL	          : std_logic_vector(31 downto 0) := (others => '0');
	signal   UL_2         : std_logic_vector(31 downto 0) := (others => '0');
	signal   buf1         : std_logic_vector(7 downto 0) := x"03";
		
	signal   clk_div	   : std_logic := '0';
	signal   new_data	   : std_logic := '0';
	signal   clr_main_cntr : std_logic := '0';
	signal   clr_bvalid    : std_logic := '0';
	signal   last_prd      : std_logic := '0';
	signal   last_prd_pack : std_logic := '0';
	signal   io_upd        : std_logic := '0';
	signal   io_upd_mnl    : std_logic := '0';
	signal   osk           : std_logic := '0';
	signal   drctrl        : std_logic := '1';
	signal   drhold        : std_logic := '0';
	signal   pn_rise       : std_logic := '0';
    signal   pn_r_d        : std_logic := '0';
	signal   prd_rise      : std_logic := '0';
	signal   prd_rise_z1   : std_logic := '0';
	signal   prd_rise_z2   : std_logic := '0';
	signal   prd_rise_z3   : std_logic := '0';
	signal   prd_rise_z4   : std_logic := '0';
	signal   prd_rise_clk_dds : std_logic := '0';
    signal   prd_rise_clk_dds_z1 : std_logic := '0';
    signal   prd_rise_clk_dds_z2 : std_logic := '0';
    signal   prd_rise_clk_dds_z3 : std_logic := '0';
    signal   prd_rise_clk_dds_z4 : std_logic := '0';
    signal   prd_fall_clk_dds : std_logic := '0';
    signal   prd_fall_clk_dds_z1 : std_logic := '0';
    signal   prd_fall_clk_dds_z2 : std_logic := '0';
    signal   prd_fall_clk_dds_z3 : std_logic := '0';
    signal   prd_fall_clk_dds_z4 : std_logic := '0';
    signal   prd_fall_clk_dds_z5 : std_logic := '0';
    signal   prd_fall_clk_dds_z6 : std_logic := '0';
    signal   prd_fall_clk_dds_z7 : std_logic := '0';
    signal   prd_fall_clk_dds_z8 : std_logic := '0';
    signal   prd_rise_clk_dds_start : std_logic := '0';
    signal   prd_fall_clk_dds_stop  : std_logic := '0';
    signal   prd_fall      : std_logic := '0';
    signal   prd_fall_z1   : std_logic := '0';
    signal   prd_fall_z2   : std_logic := '0';
    signal   prd_fall_z3   : std_logic := '0';
	signal   znd_rise      : std_logic := '0';
    signal   znd_r_d       : std_logic := '0';
    signal   znd_fall      : std_logic := '0';
	signal   znd_f_d       : std_logic := '0';
	signal   prd_r_d       : std_logic := '0';
    signal   prd_r_d_clk_dds : std_logic := '0';
    signal   prd_f_d_clk_dds : std_logic := '0';
    signal   prd_clk_div   : std_logic := '0';
    signal   prd_f_d       : std_logic := '0';
    signal   str_prd_sync  : std_logic := '0';
	
	
	signal   clk_div_cntr : integer range 0 to 7 := 0;
	signal   main_cntr    : integer range 0 to 127 := 0;
	signal   cntr_prd     : integer range 0 to 65535 := 0;
	signal   cntr_prd_pack: integer range 0 to 65535 := 0;
	signal   UL_int       : unsigned (31 downto 0) := (others => '0');
	signal   N_step_int   : unsigned (15 downto 0) := (others => '0');
	signal   FTW_int      : unsigned (31 downto 0) := (others => '0');
	signal   DFTW_int     : unsigned (31 downto 0) := (others => '0');
	signal   DFRRW_int    : unsigned (31 downto 0) := (others => '0');
	signal   UL_usg       : unsigned (95 downto 0) := (others => '0');
	signal   UL_usg2      : unsigned (127 downto 0) := (others => '0');
	signal   PERIOD_int   : unsigned (31 downto 0) := (others => '0');
	signal   ITER_int	  : unsigned (15 downto 0) := (others => '0');

	
	
	type state_dds is 
	(manual,
	 auto_pwd,
	 auto_cnt_str,
	 auto_cnt_prp,
	 auto_pls_prd,
	 auto_pls_prm,
	 auto_pls_prp,
	 auto_chirp1_prd,
	 auto_chirp1_prm,
	 auto_chirp1_prp,
	 auto_chirp2_prd,
	 auto_chirp2_prm,
     auto_chirp2_prp,
	 auto_chirp_pack_prd,
     auto_chirp_pack_prm,
     auto_chirp_pack_prp);
    
    signal stt_dds : state_dds := manual;
----==================================================================
----   -- ќтладка
--    attribute mark_debug : string;
--    attribute mark_debug of buf1 : signal is "true";
----===================================================================  
begin
	-- I/O Connections assignments

	S_AXI_AWREADY	<= axi_awready;
	S_AXI_WREADY	<= axi_wready;
	S_AXI_BRESP	<= axi_bresp;
	S_AXI_BVALID	<= axi_bvalid;
	S_AXI_ARREADY	<= axi_arready;
	S_AXI_RDATA	<= axi_rdata;
	S_AXI_RRESP	<= axi_rresp;
	S_AXI_RVALID	<= axi_rvalid;
	-- Implement axi_awready generation
	-- axi_awready is asserted for one S_AXI_ACLK clock cycle when both
	-- S_AXI_AWVALID and S_AXI_WVALID are asserted. axi_awready is
	-- de-asserted when reset is low.

	process (S_AXI_ACLK)
	begin
	  if rising_edge(S_AXI_ACLK) then 
	    if S_AXI_ARESETN = '0' then
	      axi_awready <= '0';
	      aw_en <= '1';
	    else
	      if (axi_awready = '0' and S_AXI_AWVALID = '1' and S_AXI_WVALID = '1' and aw_en = '1') then
	        -- slave is ready to accept write address when
	        -- there is a valid write address and write data
	        -- on the write address and data bus. This design 
	        -- expects no outstanding transactions. 
	        axi_awready <= '1';
	        elsif (S_AXI_BREADY = '1' and axi_bvalid = '1') then
	            aw_en <= '1';
	        	axi_awready <= '0';
	      else
	        axi_awready <= '0';
	      end if;
	    end if;
	  end if;
	end process;

	-- Implement axi_awaddr latching
	-- This process is used to latch the address when both 
	-- S_AXI_AWVALID and S_AXI_WVALID are valid. 

	process (S_AXI_ACLK)
	begin
	  if rising_edge(S_AXI_ACLK) then 
	    if S_AXI_ARESETN = '0' then
	      axi_awaddr <= (others => '0');
	    else
	      if (axi_awready = '0' and S_AXI_AWVALID = '1' and S_AXI_WVALID = '1' and aw_en = '1') then
	        -- Write Address latching
	        axi_awaddr <= S_AXI_AWADDR;
	      end if;
	    end if;
	  end if;                   
	end process; 

	-- Implement axi_wready generation
	-- axi_wready is asserted for one S_AXI_ACLK clock cycle when both
	-- S_AXI_AWVALID and S_AXI_WVALID are asserted. axi_wready is 
	-- de-asserted when reset is low. 

	process (S_AXI_ACLK)
	begin
	  if rising_edge(S_AXI_ACLK) then 
	    if S_AXI_ARESETN = '0' then
	      axi_wready <= '0';
	    else
	      if (axi_wready = '0' and S_AXI_WVALID = '1' and S_AXI_AWVALID = '1' and aw_en = '1') then
	          -- slave is ready to accept write data when 
	          -- there is a valid write address and write data
	          -- on the write address and data bus. This design 
	          -- expects no outstanding transactions.           
	          axi_wready <= '1';
	      else
	        axi_wready <= '0';
	      end if;
	    end if;
	  end if;
	end process; 

	-- Implement memory mapped register select and write logic generation
	-- The write data is accepted and written to memory mapped registers when
	-- axi_awready, S_AXI_WVALID, axi_wready and S_AXI_WVALID are asserted. Write strobes are used to
	-- select byte enables of slave registers while writing.
	-- These registers are cleared when reset (active low) is applied.
	-- Slave register write enable is asserted when valid address and data are available
	-- and the slave is ready to accept the write address and write data.
	slv_reg_wren <= axi_wready and S_AXI_WVALID and axi_awready and S_AXI_AWVALID ;

	process (S_AXI_ACLK)
	variable loc_addr :std_logic_vector(OPT_MEM_ADDR_BITS downto 0); 
	begin
	  if rising_edge(S_AXI_ACLK) then 
	    if S_AXI_ARESETN = '0' then
	      slv_reg0 <= (others => '0');
	      slv_reg1 <= (others => '0');
	      slv_reg2 <= (others => '0');
	      slv_reg3 <= (others => '0');
	      slv_reg4 <= (others => '0');
	      slv_reg5 <= (others => '0');
	      slv_reg6 <= (others => '0');
	      slv_reg7 <= (others => '0');
	      slv_reg8 <= (others => '0');
	      slv_reg9 <= (others => '0');
	      slv_reg10 <= (others => '0');
	      slv_reg11 <= (others => '0');
	      slv_reg12 <= (others => '0');
	      slv_reg13 <= (others => '0');
	      slv_reg14 <= (others => '0');
	      slv_reg15 <= (others => '0');
	      addr_buf <= "0000";
	    else
	      loc_addr := axi_awaddr(ADDR_LSB + OPT_MEM_ADDR_BITS downto ADDR_LSB);
	      if (slv_reg_wren = '1') then
	        case loc_addr is
	          when b"0000" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 0
	                addr_buf <= "0000";
	                slv_reg0(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0001" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 1
	                addr_buf <= "0001";
	                slv_reg1(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0010" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 2
	                addr_buf <= "0010";
	                slv_reg2(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0011" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 3
	                addr_buf <= "0011";
	                slv_reg3(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0100" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 4
	                addr_buf <= "0100";
	                slv_reg4(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0101" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 5
	                addr_buf <= "0101";
	                slv_reg5(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0110" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 6
	                addr_buf <= "0110";
	                slv_reg6(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0111" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 7
	                addr_buf <= "0111";
	                slv_reg7(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1000" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 8
	                addr_buf <= "1000";
	                slv_reg8(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1001" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 9
	                addr_buf <= "1001";
	                slv_reg9(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1010" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 10
	                addr_buf <= "1010";
	                slv_reg10(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1011" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 11
	                addr_buf <= "1011";
	                slv_reg11(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1100" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 12
	                addr_buf <= "1100";
	                slv_reg12(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1101" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 13
	                addr_buf <= "1101";
	                slv_reg13(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1110" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 14
	                addr_buf <= "1110";
	                slv_reg14(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1111" =>
	            for byte_index in 0 to (C_S_AXI_DATA_WIDTH/8-1) loop
	              if ( S_AXI_WSTRB(byte_index) = '1' ) then
	                -- Respective byte enables are asserted as per write strobes                   
	                -- slave registor 15
	                addr_buf <= "1111";
	                slv_reg15(byte_index*8+7 downto byte_index*8) <= S_AXI_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when others =>
	            slv_reg0 <= slv_reg0;
	            slv_reg1 <= slv_reg1;
	            slv_reg2 <= slv_reg2;
	            slv_reg3 <= slv_reg3;
	            slv_reg4 <= slv_reg4;
	            slv_reg5 <= slv_reg5;
	            slv_reg6 <= slv_reg6;
	            slv_reg7 <= slv_reg7;
	            slv_reg8 <= slv_reg8;
	            slv_reg9 <= slv_reg9;
	            slv_reg10 <= slv_reg10;
	            slv_reg11 <= slv_reg11;
	            slv_reg12 <= slv_reg12;
	            slv_reg13 <= slv_reg13;
	            slv_reg14 <= slv_reg14;
	            slv_reg15 <= slv_reg15;
	        end case;
	      else
	        if slv_reg0(31) = '1' then slv_reg0(31 downto 0) <= '0' & slv_reg0(30 downto 0); else slv_reg0 <= slv_reg0; end if;
	      end if;
	    end if;
	  end if;                   
	end process; 

	-- Implement write response logic generation
	-- The write response and response valid signals are asserted by the slave 
	-- when axi_wready, S_AXI_WVALID, axi_wready and S_AXI_WVALID are asserted.  
	-- This marks the acceptance of address and indicates the status of 
	-- write transaction.

	process (S_AXI_ACLK)
	begin
	  if rising_edge(S_AXI_ACLK) then 
	    if S_AXI_ARESETN = '0' then
	      axi_bvalid  <= '0';
	      axi_bresp   <= "00"; --need to work more on the responses
	    else
	      if (axi_awready = '1' and S_AXI_AWVALID = '1' and axi_wready = '1' and S_AXI_WVALID = '1' and axi_bvalid = '0'  ) then
	        axi_bvalid <= '1';
	        axi_bresp  <= "00"; 
	      elsif (S_AXI_BREADY = '1' and axi_bvalid = '1') then   --check if bready is asserted while bvalid is high)
	        axi_bvalid <= '0';                                 -- (there is a possibility that bready is always asserted high)
	      end if;
	    end if;
	  end if;                   
	end process; 

	-- Implement axi_arready generation
	-- axi_arready is asserted for one S_AXI_ACLK clock cycle when
	-- S_AXI_ARVALID is asserted. axi_awready is 
	-- de-asserted when reset (active low) is asserted. 
	-- The read address is also latched when S_AXI_ARVALID is 
	-- asserted. axi_araddr is reset to zero on reset assertion.

	process (S_AXI_ACLK)
	begin
	  if rising_edge(S_AXI_ACLK) then 
	    if S_AXI_ARESETN = '0' then
	      axi_arready <= '0';
	      axi_araddr  <= (others => '1');
	    else
	      if (axi_arready = '0' and S_AXI_ARVALID = '1') then
	        -- indicates that the slave has acceped the valid read address
	        axi_arready <= '1';
	        -- Read Address latching 
	        axi_araddr  <= S_AXI_ARADDR;           
	      else
	        axi_arready <= '0';
	      end if;
	    end if;
	  end if;                   
	end process; 

	-- Implement axi_arvalid generation
	-- axi_rvalid is asserted for one S_AXI_ACLK clock cycle when both 
	-- S_AXI_ARVALID and axi_arready are asserted. The slave registers 
	-- data are available on the axi_rdata bus at this instance. The 
	-- assertion of axi_rvalid marks the validity of read data on the 
	-- bus and axi_rresp indicates the status of read transaction.axi_rvalid 
	-- is deasserted on reset (active low). axi_rresp and axi_rdata are 
	-- cleared to zero on reset (active low).  
	process (S_AXI_ACLK)
	begin
	  if rising_edge(S_AXI_ACLK) then
	    if S_AXI_ARESETN = '0' then
	      axi_rvalid <= '0';
	      axi_rresp  <= "00";
	    else
	      if (axi_arready = '1' and S_AXI_ARVALID = '1' and axi_rvalid = '0') then
	        -- Valid read data is available at the read data bus
	        axi_rvalid <= '1';
	        axi_rresp  <= "00"; -- 'OKAY' response
	      elsif (axi_rvalid = '1' and S_AXI_RREADY = '1') then
	        -- Read data is accepted by the master
	        axi_rvalid <= '0';
	      end if;            
	    end if;
	  end if;
	end process;

	-- Implement memory mapped register select and read logic generation
	-- Slave register read enable is asserted when valid address is available
	-- and the slave is ready to accept the read address.
	slv_reg_rden <= axi_arready and S_AXI_ARVALID and (not axi_rvalid) ;

	process (slv_reg0, slv_reg1, slv_reg2, slv_reg3, slv_reg4, slv_reg5, slv_reg6, slv_reg7, slv_reg8, slv_reg9, slv_reg10, slv_reg11, slv_reg12, slv_reg13, slv_reg14, slv_reg15, axi_araddr, S_AXI_ARESETN, slv_reg_rden)
	variable loc_addr :std_logic_vector(OPT_MEM_ADDR_BITS downto 0);
	begin
	    -- Address decoding for reading registers
	    loc_addr := axi_araddr(ADDR_LSB + OPT_MEM_ADDR_BITS downto ADDR_LSB);
	    case loc_addr is
	      when b"0000" =>
	        reg_data_out <= slv_reg0;
	      when b"0001" =>
	        reg_data_out <= slv_reg1;
	      when b"0010" =>
	        reg_data_out <= slv_reg2;
	      when b"0011" =>
	        reg_data_out <= slv_reg3;
	      when b"0100" =>
	        reg_data_out <= slv_reg4;
	      when b"0101" =>
	        reg_data_out <= slv_reg5;
	      when b"0110" =>
	        reg_data_out <= slv_reg6;
	      when b"0111" =>
	        reg_data_out <= slv_reg7;
	      when b"1000" =>
	        reg_data_out <= slv_reg8;
	      when b"1001" =>
	        reg_data_out <= slv_reg9;
	      when b"1010" =>
	        reg_data_out <= slv_reg10;
	      when b"1011" =>
	        reg_data_out <= slv_reg11;
	      when b"1100" =>
	        reg_data_out <= slv_reg12;
	      when b"1101" =>
	        reg_data_out <= slv_reg13;
	      when b"1110" =>
	        reg_data_out <= slv_reg14;
	      when b"1111" =>
	        reg_data_out <= slv_reg15;
	      when others =>
	        reg_data_out  <= (others => '0');
	    end case;
	end process; 

	-- Output register or memory read data

	process( S_AXI_ACLK ) is
	begin
	  if (rising_edge (S_AXI_ACLK)) then
	    if ( S_AXI_ARESETN = '0' ) then
	      axi_rdata  <= (others => '0');
	    else
	      if (slv_reg_rden = '1') then
	        -- When there is a valid read address (S_AXI_ARVALID) with 
	        -- acceptance of read address by the slave (axi_arready), 
	        -- output the read dada 
	        -- Read address mux
	          axi_rdata <= reg_data_out;     -- register read data
	      end if;   
	    end if;
	  end if;
	end process;


	-- Add user logic here
	--=================================================================
	-- яковлев 
	buf1 <= CUR_PROF(93*8-1 downto (93-1)*8);
	--==================================================================
	drctrl_out <= drctrl;--drctrl_in or drctrl;
    drover_out <= drover_in;
	osk_out <= osk or osk_in;
	drhold_out <= drhold;--drhold_in or drhold;
	drover_out <= drover_in;
	io_upd_out <= io_upd or io_upd_in;

	process( clk_dds ) is--импульс по нарастающему фронту строба DDS
    begin
        if (rising_edge (clk_dds)) then
            prd_r_d_clk_dds <= str_prd;
            prd_rise_clk_dds <= str_prd and not prd_r_d_clk_dds;
            prd_rise_clk_dds_z1 <= prd_rise_clk_dds;
            prd_rise_clk_dds_z2 <= prd_rise_clk_dds_z1;
            prd_rise_clk_dds_z3 <= prd_rise_clk_dds_z2;
            prd_rise_clk_dds_z4 <= prd_rise_clk_dds_z3;
        end if;    
    end process;    

	process( clk_dds ) is--импульс по нарастающему фронту строба DDS
    begin
        if (rising_edge (clk_dds)) then
            prd_f_d_clk_dds <= str_prd;
            prd_fall_clk_dds <= not str_prd and prd_f_d_clk_dds;
            prd_fall_clk_dds_z1 <= prd_fall_clk_dds;
            prd_fall_clk_dds_z2 <= prd_fall_clk_dds_z1;
            prd_fall_clk_dds_z3 <= prd_fall_clk_dds_z2;
            prd_fall_clk_dds_z4 <= prd_fall_clk_dds_z3;
            prd_fall_clk_dds_z5 <= prd_fall_clk_dds_z4;
            prd_fall_clk_dds_z6 <= prd_fall_clk_dds_z5;
            prd_fall_clk_dds_z7 <= prd_fall_clk_dds_z6;
            prd_fall_clk_dds_z8 <= prd_fall_clk_dds_z7;
        end if;    
    end process;    
    
	process( clk_div ) is--импульс по нарастающему фронту строба DDS
    begin
        if (rising_edge (clk_div)) then
            prd_clk_div <= str_prd;
        end if;    
    end process;    

    process( clk_dds ) is --генератор пониженной частоты f_clk_div = f_dds/(2^(clk_divider + 1+1));
    begin
            if (rising_edge (clk_dds)) then
                clk_div <= not clk_div;  
            end if;
    end process;
    
	process( clk_div ) is --импульс по спадающему фронту en_zond
    begin
        if (rising_edge (clk_div)) then
            znd_f_d <= en_zond;
            znd_fall <= not en_zond and znd_f_d;
        end if;    
    end process;    

	process( clk_div ) is --импульс по нарастающему фронту строба DDS
    begin
        if (rising_edge (clk_div)) then
            prd_r_d <= prd_clk_div;
            prd_rise <= prd_clk_div and not prd_r_d;
            prd_rise_z1 <= prd_rise;
            prd_rise_z2 <= prd_rise_z1;
            prd_rise_z3 <= prd_rise_z2;
            prd_rise_z4 <= prd_rise_z3;
        end if;    
    end process;    


	process( clk_div ) is --импульс по спадающему фронту строба DDS
    begin
        if (rising_edge (clk_div)) then
            prd_f_d <= prd_clk_div;
            prd_fall <= not prd_clk_div and prd_f_d;
            prd_fall_z1 <= prd_fall;
            prd_fall_z2 <= prd_fall_z1;
            prd_fall_z3 <= prd_fall_z2;
        end if;    
    end process;    
    
	process( S_AXI_ACLK ) is --буферизаци€ регистров управлени€ по AXI
    begin
        if (rising_edge (S_AXI_ACLK)) then
                ctrl_reg <= slv_reg0;
                data_reg <= slv_reg1;
        end if;    
    end process;
       
    clr_main_cntr <= slv_reg0(31) or prd_rise or prd_fall or znd_fall;
	process( clk_div ) is --обнуление главного счетчика
    begin
            if clr_main_cntr = '1' then
                main_cntr <= 0;
            else
                if rising_edge (clk_div) then
                    if main_cntr < 127 then
                        main_cntr <= main_cntr + 1;
                    end if;
                end if;
            end if;
    end process;    

   
	process( clk_div ) is --счетчик стробов передачи
    begin
        if en_zond = '1' then    
            if str_pn = '1' then    
	            cntr_prd <= 0;
	        else
	            if rising_edge (clk_div) then
	                if prd_rise = '1' then
	                     cntr_prd <= cntr_prd + 1;
	                end if;
	            end if; 
	        end if;
	    else
	        cntr_prd <= 0;
	    end if;
	end process;
	
	process( clk_div ) is --определение последнего зондировани€ в такте
    begin
       if rising_edge (clk_div) then
           if prd_rise_z1 = '1' then
               if cntr_prd = to_integer(unsigned(ITER (15 downto 0))) then 
                   last_prd <= '1';
               else
                   last_prd <= '0';
               end if;
           end if;    
       end if;
    end process;

	process( clk_div ) is --счетчик стробов передачи
    begin
        if en_zond = '1' then    
            if str_pn = '1' then    
                cntr_prd_pack <= 0;
            elsif rising_edge (clk_div) then
                if prd_rise = '1' then
                    if cntr_prd_pack = to_integer(unsigned(NUM_PULSE (15 downto 0))) then
                        cntr_prd_pack <= 1;
                    else    
                        cntr_prd_pack <= cntr_prd_pack + 1;
                    end if;
                end if;
            end if;
        else
            cntr_prd_pack <= 0;
        end if;
    end process;

	process( clk_div ) is --определение последнего зондировани€ в такте
    begin
       if rising_edge (clk_div) then
           if prd_rise_z1 = '1' then
               if cntr_prd_pack = to_integer(unsigned(NUM_PULSE (15 downto 0))) then 
                   last_prd_pack <= '1';
               else
                   last_prd_pack <= '0';
               end if;
           end if;    
       end if;
    end process;

----автомат состо€ний--------------------------------------------
	process( clk_div ) is
    begin
    if rising_edge (clk_div) then    
        if ctrl_reg(0) = '1' then
            stt_dds <= manual;
        else
          if en_zond = '1' then   
             case siganal_type(7 downto 0) is --CUR_PROF(93*8-1 downto (93-1)*8)
                when x"00" => 
                    if last_prd = '0' then 
                        stt_dds <= auto_pwd;
                    else 
                        if prd_clk_div = '0' then
                            case siganal_type_nxt(7 downto 0) is
                                when x"00" => stt_dds <= auto_pwd; 
                                when x"01" => stt_dds <= auto_cnt_prp;
                                when x"02" => stt_dds <= auto_pls_prp;
                                when x"03" => stt_dds <= auto_chirp1_prp;
                                when x"04" => stt_dds <= auto_chirp2_prp;
                                when x"05" => stt_dds <= auto_chirp_pack_prp;
                                when x"06" => stt_dds <= auto_chirp_pack_prp;
                               when others =>
                            end case;
                        end if;
                    end if;
                when x"01" => 
                    if last_prd = '0' then stt_dds <= auto_cnt_str; 
                    else 
                        if prd_clk_div = '0' then
                            case siganal_type_nxt(7 downto 0) is
                                when x"00" => stt_dds <= auto_pwd; 
                                when x"01" => stt_dds <= auto_cnt_prp;
                                when x"02" => stt_dds <= auto_pls_prp;
                                when x"03" => stt_dds <= auto_chirp1_prp;
                                when x"04" => stt_dds <= auto_chirp2_prp;
                                when x"05" => stt_dds <= auto_chirp_pack_prp;
                                when x"06" => stt_dds <= auto_chirp_pack_prp;
                                when others =>
                            end case;
                        end if;
                    end if;
                when x"02" => 
                    if last_prd = '0' then 
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_pls_prd;
                        elsif prd_fall_z2 = '1' then 
                            stt_dds <= auto_pls_prm;
                        end if; 
                    else 
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_pls_prd;
                        elsif prd_fall_z2 = '1' then
                            case siganal_type_nxt(7 downto 0) is
                                when x"00" => stt_dds <= auto_pwd; 
                                when x"01" => stt_dds <= auto_cnt_prp;
                                when x"02" => stt_dds <= auto_pls_prp;
                                when x"03" => stt_dds <= auto_chirp1_prp;
                                when x"04" => stt_dds <= auto_chirp2_prp;
                                when x"05" => stt_dds <= auto_chirp_pack_prp;
                                when x"06" => stt_dds <= auto_chirp_pack_prp;
                                when others =>
                            end case;
                        end if;
                    end if;
                when x"03" => 
                    if last_prd = '0' then 
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_chirp1_prd;
                        elsif prd_fall_z2 = '1' then
                            stt_dds <= auto_chirp1_prm;
                        end if; 
                    else 
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_chirp1_prd;
                        elsif prd_fall_z2 = '1' then
                            case siganal_type_nxt(7 downto 0) is --NXT_PROF(93*8-1 downto (93-1)*8)
                                when x"00" => stt_dds <= auto_pwd; 
                                when x"01" => stt_dds <= auto_cnt_prp;
                                when x"02" => stt_dds <= auto_pls_prp;
                                when x"03" => stt_dds <= auto_chirp1_prp;
                                when x"04" => stt_dds <= auto_chirp2_prp;
                                when x"05" => stt_dds <= auto_chirp_pack_prp;
                                when x"06" => stt_dds <= auto_chirp_pack_prp;
                                when others =>
                            end case;
                        end if;
                    end if;
                when x"04" => 
                    if last_prd = '0' then 
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_chirp2_prd;
                        elsif prd_fall_z2 = '1' then
                            stt_dds <= auto_chirp2_prm;
                        end if; 
                    else
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_chirp2_prd;
                        elsif prd_fall_z2 = '1' then
                            case siganal_type_nxt(7 downto 0) is
                                when x"00" => stt_dds <= auto_pwd; 
                                when x"01" => stt_dds <= auto_cnt_prp;
                                when x"02" => stt_dds <= auto_pls_prp;
                                when x"03" => stt_dds <= auto_chirp1_prp;
                                when x"04" => stt_dds <= auto_chirp2_prp;
                                when x"05" => stt_dds <= auto_chirp_pack_prp;
                                when x"06" => stt_dds <= auto_chirp_pack_prp;
                                when others =>
                            end case;
                        end if;
                    end if;
                when x"05" => 
                    if last_prd = '0' then 
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_chirp_pack_prd;
                        elsif prd_fall_z2 = '1' then
                            stt_dds <= auto_chirp_pack_prm;
                        end if; 
                    else
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_chirp_pack_prd;
                        elsif prd_fall_z2 = '1' then
                            case siganal_type_nxt(7 downto 0) is
                                when x"00" => stt_dds <= auto_pwd; 
                                when x"01" => stt_dds <= auto_cnt_prp;
                                when x"02" => stt_dds <= auto_pls_prp;
                                when x"03" => stt_dds <= auto_chirp1_prp;
                                when x"04" => stt_dds <= auto_chirp2_prp;
                                when x"05" => stt_dds <= auto_chirp_pack_prp;
                                when x"06" => stt_dds <= auto_chirp_pack_prp;
                                when others =>
                            end case;
                        end if;
                    end if;
                when x"06" => 
                    if last_prd = '0' then 
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_chirp_pack_prd;
                        elsif prd_fall_z2 = '1' then
                            stt_dds <= auto_chirp_pack_prm;
                        end if; 
                    else
                        if prd_rise_z2 = '1' then
                            stt_dds <= auto_chirp_pack_prd;
                        elsif prd_fall_z2 = '1' then
                            case siganal_type_nxt(7 downto 0) is
                                when x"00" => stt_dds <= auto_pwd; 
                                when x"01" => stt_dds <= auto_cnt_prp;
                                when x"02" => stt_dds <= auto_pls_prp;
                                when x"03" => stt_dds <= auto_chirp1_prp;
                                when x"04" => stt_dds <= auto_chirp2_prp;
                                when x"05" => stt_dds <= auto_chirp_pack_prp;
                                when x"06" => stt_dds <= auto_chirp_pack_prp;
                                when others =>
                            end case;
                        end if;
                    end if;
                when others =>
            end case;
          else
              stt_dds <= auto_pwd;
          end if;
      end if;
    end if;    
    end process;    
---------------------------------------------------------------------------------
 	
 	siganal_type(7 downto 0)    <= CUR_PROF(93*8-1 downto (93-1)*8) when rising_edge (prd_rise);
    siganal_type_nxt(7 downto 0)<= NXT_PROF(93*8-1 downto (93-1)*8) when rising_edge (prd_rise);
    FTW (31 downto 0)           <= NXT_PROF(64*8-1 downto (61-1)*8) when rising_edge (prd_rise); --x"3bbbbbbb";
    DFTW (31 downto 0)          <= NXT_PROF(68*8-1 downto (65-1)*8) when rising_edge (prd_rise);
    DFRRW (31 downto 0)         <= NXT_PROF(72*8-1 downto (72-4)*8) when rising_edge (prd_rise);
    NUM_PULSE (15 downto 0)     <= CUR_PROF(70*8-1 downto (72-4)*8) when rising_edge (prd_rise);
    PERIOD (15 downto 0)        <= NXT_PROF(74*8-1 downto (74-2)*8) when rising_edge (prd_rise);
    ITER (15 downto 0)          <= CUR_PROF (60*8-1 downto (60-2)*8) when rising_edge (prd_rise);
    dds_start (15 downto 0)     <= NXT_PROF((80*8-1) downto (80-2)*8) when rising_edge (prd_rise);--x"012c";--
    dds_stop (15 downto 0)      <= NXT_PROF((82*8-1) downto (82-2)*8) when rising_edge (prd_rise);--x"01aa";--    

    
    N_step_int (15 downto 0) <= unsigned(dds_stop (15 downto 0)) - unsigned(dds_start (15 downto 0));
    FTW_int (31 downto 0) <= unsigned( FTW (31 downto 0) );
    DFTW_int (31 downto 0) <= unsigned( DFTW (31 downto 0) );
    DFRRW_int (31 downto 0) <= unsigned( DFRRW (31 downto 0) );
    PERIOD_int (15 downto 0) <= unsigned( PERIOD (15 downto 0) );
    ITER_int (15 downto 0) <= unsigned( ITER (15 downto 0) );
    UL_usg (95 downto 0) <= FTW_int(31 downto 0) + DFTW_int(31 downto 0) * N_step_int(15 downto 0) * 2; 
    UL (31 downto 0) <=  x"ffffffff";--STD_LOGIC_VECTOR(UL_usg(31 downto 0));
    UL_usg2 (127 downto 0) <= FTW_int(31 downto 0) + DFTW_int(31 downto 0) * PERIOD_int (15 downto 0) * ITER_int (15 downto 0) * 2; --DFTW_int(31 downto 0)
    UL_2 (31 downto 0) <=  x"ffffffff";--STD_LOGIC_VECTOR(UL_usg2(31 downto 0));
    
----------------------загрузка данных в DDS--------------------------------------
	process( clk_div ) is
    begin
      if rising_edge (clk_div) then
        case stt_dds is
            when manual => 
                case main_cntr is
                    when 3 => 
                        PS_buf(2 downto 0) <= ctrl_reg(3 downto 1);
                        F(3 downto 0) <= ctrl_reg(7 downto 4);
                        pwdwn <= ctrl_reg(8);
                        rst <= ctrl_reg(9);
                        osk <= ctrl_reg(10);
                        --drctrl <= ctrl_reg(11);
                        dir <= '1';
                        D(31 downto 0) <= data_reg(31 downto 3) & "000";
                    when 4 => 
                        if data_reg(2) = '1' then
                            D(31 downto 0) <= data_reg(31 downto 3) & "100";
                        end if;
                    when 5 => 
                        D(31 downto 0) <= data_reg(31 downto 3) & "000";
                        io_upd_mnl <= '1';
                    when 6 => io_upd_mnl <= '0';
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_pwd => 
                case main_cntr is
                    when 3 => 
                        PS_buf(2 downto 0) <= "000";
                        F(3 downto 0) <= "0000";
                        pwdwn <= '0';
                        dir <= '1';
                        rst <= '0';
                        osk <= '0';
                    when 4 => D(31 downto 0) <= x"00" & x"00" & x"06" & "00000" & "000";
                    when 5 => D(31 downto 0) <= x"00" & x"00" & x"06" & "00000" & "100";
                    when 6 => D(31 downto 0) <= x"00" & x"a8" & x"00" & "00000" & "000";
                    when 7 => D(31 downto 0) <= x"00" & x"a8" & x"00" & "00000" & "100";
                    when 8 => D(31 downto 0) <= x"00" & x"0b" & x"01" & "00000" & "000";
                    when 9 => D(31 downto 0) <= x"00" & x"0b" & x"01" & "00000" & "100";
                    when 10 => D(31 downto 0) <= x"00" & x"0b" & x"01" & "00000" & "000";
                    when 11 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "000";
                    when 12 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "100";
                    when 13 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "000";
                    when 14 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "100";
                    when 15 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "000";
                    when 16 => io_upd_mnl <= '1';
                    when 17 => io_upd_mnl <= '0';
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_cnt_prp => 
                case main_cntr is
                    when 3 =>
                        PS_buf(2 downto 0) <= "000";
                        F(3 downto 0) <= "0000";
                        pwdwn <= '0';
                        rst <= '0';
                        dir <= '1';
                        osk <= '0';
                        D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "100";
                    when 9 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"2c" & "00000" & "000";
                    when 10 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"2c" & "00000" & "100";
                    when 11 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"2d" & "00000" & "000";
                    when 12 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"2d" & "00000" & "100";
                    when 13 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"2e" & "00000" & "000";
                    when 14 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"2e" & "00000" & "100";
                    when 15 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"2f" & "00000" & "000";
                    when 16 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"2f" & "00000" & "100";
                    when 17 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "000";
                    when 18 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "100";
                    when 19 => D(31 downto 0) <= x"00" & x"80" & x"06" & "00000" & "000";
                    when 20 => D(31 downto 0) <= x"00" & x"80" & x"06" & "00000" & "100";
                    when 21 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "000";
                    when 22 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "100";
                    when 23 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when 24 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_pls_prd => 
                case main_cntr is
                    when 3 => D(31 downto 0) <= x"00" & x"09" & x"01" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"09" & x"01" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_pls_prm => 
                case main_cntr is
                    when 3 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_pls_prp => 
                case main_cntr is
                    when 3 =>
                        PS_buf(2 downto 0) <= "000";
                        F(3 downto 0) <= "0000";
                        pwdwn <= '0';
                        dir <= '1';
                        rst <= '0';
                        osk <= '0';
                        D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "100";
                    when 9 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "000";
                    when 10 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "100";
                    when 11 => D(31 downto 0) <= x"00" & x"80" & x"06" & "00000" & "000";
                    when 12 => D(31 downto 0) <= x"00" & x"80" & x"06" & "00000" & "100";
                    when 13 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"2c" & "00000" & "000";
                    when 14 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"2c" & "00000" & "100";
                    when 15 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"2d" & "00000" & "000";
                    when 16 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"2d" & "00000" & "100";
                    when 17 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"2e" & "00000" & "000";
                    when 18 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"2e" & "00000" & "100";
                    when 19 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"2f" & "00000" & "000";
                    when 20 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"2f" & "00000" & "100";
                    when 21 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "000";
                    when 22 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "100";
                    when 23 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when 24 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_chirp1_prd => 
                case main_cntr is
                    when 3 => D(31 downto 0) <= x"00" & x"19" & x"01" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"19" & x"01" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_chirp1_prm => 
                case main_cntr is
                    when 3 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_chirp1_prp => 
                case main_cntr is
                    when 3 =>
                        PS_buf(2 downto 0) <= "000";
                        F(3 downto 0) <= "0000";
                        pwdwn <= '0';
                        dir <= '1';
                        rst <= '0';
                        osk <= '0';
                        D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "100";
                    when 9 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "000";
                    when 10 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "100";
                    when 11 => D(31 downto 0) <= x"00" & x"08" & x"06" & "00000" & "000";
                    when 12 => D(31 downto 0) <= x"00" & x"08" & x"06" & "00000" & "100";
                    when 13 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"10" & "00000" & "000";--Lower Limit = FTW
                    when 14 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"10" & "00000" & "100";
                    when 15 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"11" & "00000" & "000";
                    when 16 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"11" & "00000" & "100";
                    when 17 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"12" & "00000" & "000";
                    when 18 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"12" & "00000" & "100";
                    when 19 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"13" & "00000" & "000";
                    when 20 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"13" & "00000" & "100";
                    when 21 => D(31 downto 0) <= x"00" & UL(7 downto 0) & x"14" & "00000" & "000";--Upper Limit = FTW + DFTW*Nimp
                    when 22 => D(31 downto 0) <= x"00" & UL(7 downto 0) & x"14" & "00000" & "100";
                    when 23 => D(31 downto 0) <= x"00" & UL(15 downto 8) & x"15" & "00000" & "000";
                    when 24 => D(31 downto 0) <= x"00" & UL(15 downto 8) & x"15" & "00000" & "100";
                    when 25 => D(31 downto 0) <= x"00" & UL(23 downto 16) & x"16" & "00000" & "000";
                    when 26 => D(31 downto 0) <= x"00" & UL(23 downto 16) & x"16" & "00000" & "100";
                    when 27 => D(31 downto 0) <= x"00" & UL(31 downto 24) & x"17" & "00000" & "000";
                    when 28 => D(31 downto 0) <= x"00" & UL(31 downto 24) & x"17" & "00000" & "100";
                    when 29 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"18" & "00000" & "000";--DFTW
                    when 30 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"18" & "00000" & "100";
                    when 31 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"19" & "00000" & "000";
                    when 32 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"19" & "00000" & "100";
                    when 33 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1a" & "00000" & "000";
                    when 34 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1a" & "00000" & "100";
                    when 35 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1b" & "00000" & "000";
                    when 36 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1b" & "00000" & "100";
                    when 37 => D(31 downto 0) <= x"00" & x"01" & x"20" & "00000" & "000";--Positiv Ramp Rate
                    when 38 => D(31 downto 0) <= x"00" & x"01" & x"20" & "00000" & "100";
                    when 39 => D(31 downto 0) <= x"00" & x"00" & x"21" & "00000" & "000";
                    when 40 => D(31 downto 0) <= x"00" & x"00" & x"21" & "00000" & "100";
                    when 41 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "000";--Amplitude Scale Factor
                    when 42 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "100";
                    when 43 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when 44 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "100";
                    when 45 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_chirp2_prd => 
                case main_cntr is
                    when 3 => if drctrl = '0' or last_prd = '1' then D(31 downto 0) <= x"00" & x"19" & x"01" & "00000" & "000";
                              else D(31 downto 0) <= x"00" & x"09" & x"01" & "00000" & "000"; end if;   
                    when 4 => if drctrl = '0' or last_prd = '1' then D(31 downto 0) <= x"00" & x"19" & x"01" & "00000" & "100";
                              else D(31 downto 0) <= x"00" & x"09" & x"01" & "00000" & "100"; end if;   
                    when 5 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                    end case;
            when auto_chirp2_prm => 
                case main_cntr is
                    when 3 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_chirp2_prp => 
                case main_cntr is
                    when 3 =>
                        PS_buf(2 downto 0) <= "000";
                        F(3 downto 0) <= "0000";
                        pwdwn <= '0';
                        dir <= '1';
                        rst <= '0';
                        osk <= '0';
                        D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "100";
                    when 9 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "000";
                    when 10 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "100";
                    when 11 => D(31 downto 0) <= x"00" & x"08" & x"06" & "00000" & "000";
                    when 12 => D(31 downto 0) <= x"00" & x"08" & x"06" & "00000" & "100";
                    when 13 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"10" & "00000" & "000";--Lower Limit = FTW
                    when 14 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"10" & "00000" & "100";
                    when 15 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"11" & "00000" & "000";
                    when 16 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"11" & "00000" & "100";
                    when 17 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"12" & "00000" & "000";
                    when 18 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"12" & "00000" & "100";
                    when 19 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"13" & "00000" & "000";
                    when 20 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"13" & "00000" & "100";
                    when 21 => D(31 downto 0) <= x"00" & UL(7 downto 0) & x"14" & "00000" & "000";--Upper Limit = FTW + DFTW*Nimp
                    when 22 => D(31 downto 0) <= x"00" & UL(7 downto 0) & x"14" & "00000" & "100";
                    when 23 => D(31 downto 0) <= x"00" & UL(15 downto 8) & x"15" & "00000" & "000";
                    when 24 => D(31 downto 0) <= x"00" & UL(15 downto 8) & x"15" & "00000" & "100";
                    when 25 => D(31 downto 0) <= x"00" & UL(23 downto 16) & x"16" & "00000" & "000";
                    when 26 => D(31 downto 0) <= x"00" & UL(23 downto 16) & x"16" & "00000" & "100";
                    when 27 => D(31 downto 0) <= x"00" & UL(31 downto 24) & x"17" & "00000" & "000";
                    when 28 => D(31 downto 0) <= x"00" & UL(31 downto 24) & x"17" & "00000" & "100";
                    when 29 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"18" & "00000" & "000";--DFTW
                    when 30 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"18" & "00000" & "100";
                    when 31 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"19" & "00000" & "000";
                    when 32 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"19" & "00000" & "100";
                    when 33 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1a" & "00000" & "000";
                    when 34 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1a" & "00000" & "100";
                    when 35 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1b" & "00000" & "000";
                    when 36 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1b" & "00000" & "100";
                    when 37 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"1c" & "00000" & "000";--DFTW
                    when 38 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"1c" & "00000" & "100";
                    when 39 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"1d" & "00000" & "000";
                    when 40 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"1d" & "00000" & "100";
                    when 41 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1e" & "00000" & "000";
                    when 42 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1e" & "00000" & "100";
                    when 43 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1f" & "00000" & "000";
                    when 44 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1f" & "00000" & "100";
                    when 45 => D(31 downto 0) <= x"00" & x"01" & x"20" & "00000" & "000";--Positiv Ramp Rate
                    when 46 => D(31 downto 0) <= x"00" & x"01" & x"20" & "00000" & "100";
                    when 47 => D(31 downto 0) <= x"00" & x"00" & x"21" & "00000" & "000";
                    when 48 => D(31 downto 0) <= x"00" & x"00" & x"21" & "00000" & "100";
                    when 49 => D(31 downto 0) <= x"00" & x"01" & x"22" & "00000" & "000";--Negative Ramp Rate
                    when 50 => D(31 downto 0) <= x"00" & x"01" & x"22" & "00000" & "100";
                    when 51 => D(31 downto 0) <= x"00" & x"00" & x"23" & "00000" & "000";
                    when 52 => D(31 downto 0) <= x"00" & x"00" & x"23" & "00000" & "100";
                    when 53 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "000";--Amplitude Scale Factor
                    when 54 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "100";
                    when 55 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when 56 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_chirp_pack_prd => 
                case main_cntr is
                    when 3 => 
                        if siganal_type(3 downto 0) = x"5" then
                            if (last_prd = '1' or last_prd_pack = '1' ) then D(31 downto 0) <= x"00" & x"19" & x"01" & "00000" & "000"; end if;
                        elsif siganal_type(3 downto 0) = x"6" then
                            if (last_prd = '1' or (last_prd_pack = '1' and drctrl = '0')) then D(31 downto 0) <= x"00" & x"19" & x"01" & "00000" & "000"; end if;
                        end if;
                    when 4 => 
                        if siganal_type(3 downto 0) = x"5" then
                            if (last_prd = '1' or last_prd_pack = '1' ) then D(31 downto 0) <= x"00" & x"19" & x"01" & "00000" & "100"; end if;
                        elsif siganal_type(3 downto 0) = x"6" then
                            if (last_prd = '1' or (last_prd_pack = '1' and drctrl = '0')) then D(31 downto 0) <= x"00" & x"19" & x"01" & "00000" & "100"; end if;
                        end if;
        	        when 5 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "000";
            	    when 6 => D(31 downto 0) <= x"00" & x"00" & x"32" & "00000" & "100";
                	when 7 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "000";
                	when 8 => D(31 downto 0) <= x"00" & x"00" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_chirp_pack_prm => 
                case main_cntr is
                    when 3 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;
            when auto_chirp_pack_prp => 
                case main_cntr is
                    when 3 =>
                         PS_buf(2 downto 0) <= "000";
                         F(3 downto 0) <= "0000";
                         pwdwn <= '0';
                         dir <= '1'; 
                         rst <= '0';
                         osk <= '0';
                         D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "000";
                    when 4 => D(31 downto 0) <= x"00" & x"08" & x"00" & "00000" & "100";
                    when 5 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "000";
                    when 6 => D(31 downto 0) <= x"00" & x"01" & x"01" & "00000" & "100";
                    when 7 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "000";
                    when 8 => D(31 downto 0) <= x"00" & x"01" & x"02" & "00000" & "100";
                    when 9 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "000";
                    when 10 => D(31 downto 0) <= x"00" & x"09" & x"05" & "00000" & "100";
                    when 11 => D(31 downto 0) <= x"00" & x"08" & x"06" & "00000" & "000";
                    when 12 => D(31 downto 0) <= x"00" & x"08" & x"06" & "00000" & "100";
                    when 13 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"10" & "00000" & "000";--Lower Limit = FTW
                    when 14 => D(31 downto 0) <= x"00" & FTW(7 downto 0) & x"10" & "00000" & "100";
                    when 15 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"11" & "00000" & "000";
                    when 16 => D(31 downto 0) <= x"00" & FTW(15 downto 8) & x"11" & "00000" & "100";
                    when 17 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"12" & "00000" & "000";
                    when 18 => D(31 downto 0) <= x"00" & FTW(23 downto 16) & x"12" & "00000" & "100";
                    when 19 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"13" & "00000" & "000";
                    when 20 => D(31 downto 0) <= x"00" & FTW(31 downto 24) & x"13" & "00000" & "100";
                    when 21 => D(31 downto 0) <= x"00" & UL_2(7 downto 0) & x"14" & "00000" & "000";--Upper Limit = FTW + DFTW*Nimp
                    when 22 => D(31 downto 0) <= x"00" & UL_2(7 downto 0) & x"14" & "00000" & "100";
                    when 23 => D(31 downto 0) <= x"00" & UL_2(15 downto 8) & x"15" & "00000" & "000";
                    when 24 => D(31 downto 0) <= x"00" & UL_2(15 downto 8) & x"15" & "00000" & "100";
                    when 25 => D(31 downto 0) <= x"00" & UL_2(23 downto 16) & x"16" & "00000" & "000";
                    when 26 => D(31 downto 0) <= x"00" & UL_2(23 downto 16) & x"16" & "00000" & "100";
                    when 27 => D(31 downto 0) <= x"00" & UL_2(31 downto 24) & x"17" & "00000" & "000";
                    when 28 => D(31 downto 0) <= x"00" & UL_2(31 downto 24) & x"17" & "00000" & "100";
                    when 29 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"18" & "00000" & "000";--DFTW
                    when 30 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"18" & "00000" & "100";
                    when 31 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"19" & "00000" & "000";
                    when 32 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"19" & "00000" & "100";
                    when 33 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1a" & "00000" & "000";
                    when 34 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1a" & "00000" & "100";
                    when 35 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1b" & "00000" & "000";
                    when 36 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1b" & "00000" & "100";
                    when 37 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"1c" & "00000" & "000";--DFTW
                    when 38 => D(31 downto 0) <= x"00" & DFTW(7 downto 0) & x"1c" & "00000" & "100";
                    when 39 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"1d" & "00000" & "000";
                    when 40 => D(31 downto 0) <= x"00" & DFTW(15 downto 8) & x"1d" & "00000" & "100";
                    when 41 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1e" & "00000" & "000";
                    when 42 => D(31 downto 0) <= x"00" & DFTW(23 downto 16) & x"1e" & "00000" & "100";
                    when 43 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1f" & "00000" & "000";
                    when 44 => D(31 downto 0) <= x"00" & DFTW(31 downto 24) & x"1f" & "00000" & "100";
                    when 45 => D(31 downto 0) <= x"00" & x"0a" & x"20" & "00000" & "000";--Positiv Ramp Rate
                    when 46 => D(31 downto 0) <= x"00" & x"0a" & x"20" & "00000" & "100";
                    when 47 => D(31 downto 0) <= x"00" & x"00" & x"21" & "00000" & "000";
                    when 48 => D(31 downto 0) <= x"00" & x"00" & x"21" & "00000" & "100";
                    when 49 => D(31 downto 0) <= x"00" & x"0a" & x"22" & "00000" & "000";--Negative Ramp Rate
                    when 50 => D(31 downto 0) <= x"00" & x"0a" & x"22" & "00000" & "100";
                    when 51 => D(31 downto 0) <= x"00" & x"00" & x"23" & "00000" & "000";
                    when 52 => D(31 downto 0) <= x"00" & x"00" & x"23" & "00000" & "100";
                    when 53 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "000";--Amplitude Scale Factor
                    when 54 => D(31 downto 0) <= x"00" & x"ff" & x"32" & "00000" & "100";
                    when 55 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "000";
                    when 56 => D(31 downto 0) <= x"00" & x"0f" & x"33" & "00000" & "100";
                    when others => D(31 downto 0) <= x"00" & x"00" & x"00" & "00000" & "000";
                end case;

            when others =>
        end case;
      end if;
    end process;    
	
    process( clk_dds ) is
    begin
      if rising_edge (clk_dds) then  
        if ctrl_reg(0) = '1' then
            drctrl <= ctrl_reg(11);
        else
            case siganal_type(3 downto 0) is
            when x"0" => 
                drctrl <= '0';
            when x"1" =>
                drctrl <= '0';
            when x"2" =>
                drctrl <= '0';
            when x"3" =>
                drctrl <= '1';
            when x"4" =>
                if str_pn = '1' then
                    drctrl <= '0';
                elsif prd_rise_clk_dds = '1' then 
                    drctrl <= not drctrl;
                end if;
            when x"5" =>
                drctrl <= '1';
            when x"6" =>
                if en_zond = '0' then
                    drctrl <= '1';
                elsif (last_prd_pack = '1' and prd_rise_clk_dds = '1') then 
                    drctrl <= not drctrl;
                end if;
            when others =>
            end case;
        end if; 
      end if;    
    end process;    

    prd_rise_clk_dds_start <= prd_rise_clk_dds_z1;
    prd_fall_clk_dds_stop <= prd_fall_clk_dds;
    process( clk_dds ) is
    begin
      if rising_edge (clk_dds) then
        if ctrl_reg(0) = '1' then
            drhold <= '0';
        else
            case siganal_type(3 downto 0) is
            when x"0" => 
                drhold <= '0';
            when x"1" =>
                drhold <= '0';
            when x"2" =>
                drhold <= '0';
            when x"3" =>
                drhold <= '0';
            when x"4" =>
                if prd_rise_clk_dds_start = '1' then
                    drhold <= '0';
                elsif prd_fall_clk_dds_stop = '1' then
                    drhold <= '1';
                end if;    
            when x"5" =>
                drhold <= '0';
            when x"6" =>
                if (last_prd_pack = '1' and prd_rise_clk_dds_start = '1') then 
                    drhold <= '0';
                elsif (last_prd_pack = '1' and prd_fall_clk_dds_stop = '1') then   
                    drhold <= '1';
                end if;    
            when others =>
            end case;
        end if;
      end if;     
    end process;    

    io_upd <= prd_rise_clk_dds_z3 or prd_rise_clk_dds_z4 or prd_fall_clk_dds_z7 or prd_fall_clk_dds_z8;
	process( io_upd ) is ----установка профилей сигнала
    begin
        if (rising_edge (io_upd)) then
            PS <= PS_buf;
        end if;    
    end process;    
end arch_imp;
