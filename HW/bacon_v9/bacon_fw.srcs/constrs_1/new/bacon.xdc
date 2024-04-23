set_property CONFIG_MODE SPIx4 [current_design]
set_property BITSTREAM.CONFIG.UNUSEDPIN PULLNONE [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 66 [current_design]
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 2.5 [current_design]


#set_property PACKAGE_PIN N25 [get_ports clk_out_156M25]
set_property PACKAGE_PIN T26 [get_ports clk_in1]
#set_property IOSTANDARD LVCMOS33 [get_ports clk_out_156M25]
set_property IOSTANDARD LVCMOS33 [get_ports clk_in1]

// Test points
#set_property -dict {PACKAGE_PIN N24 IOSTANDARD LVCMOS33} [get_ports P74]
#set_property -dict {PACKAGE_PIN N25 IOSTANDARD LVCMOS33} [get_ports P76]
#set_property -dict {PACKAGE_PIN N26 IOSTANDARD LVCMOS33} [get_ports P77]
#set_property -dict {PACKAGE_PIN N27 IOSTANDARD LVCMOS33} [get_ports P78]
#set_property -dict {PACKAGE_PIN N29 IOSTANDARD LVCMOS33} [get_ports P79]
#set_property -dict {PACKAGE_PIN N30 IOSTANDARD LVCMOS33} [get_ports P80]
#set_property -dict {PACKAGE_PIN P19 IOSTANDARD LVCMOS33} [get_ports P81]
#set_property -dict {PACKAGE_PIN P21 IOSTANDARD LVCMOS33} [get_ports P82]