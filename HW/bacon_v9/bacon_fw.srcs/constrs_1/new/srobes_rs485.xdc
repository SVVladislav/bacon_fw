# LVDS Transeivers - DS91M040
set_property -dict {PACKAGE_PIN L27 IOSTANDARD LVCMOS33} [get_ports STR1_PN_de]
set_property -dict {PACKAGE_PIN L21 IOSTANDARD LVCMOS33} [get_ports STR1_PN_di]
set_property -dict {PACKAGE_PIN L26 IOSTANDARD LVCMOS33} [get_ports STR1_PN_re]
set_property -dict {PACKAGE_PIN L22 IOSTANDARD LVCMOS33} [get_ports STR1_PN_ro]

set_property -dict {PACKAGE_PIN L25 IOSTANDARD LVCMOS33} [get_ports STR2_PRD_de]
set_property -dict {PACKAGE_PIN K30 IOSTANDARD LVCMOS33} [get_ports STR2_PRD_di]
set_property -dict {PACKAGE_PIN L23 IOSTANDARD LVCMOS33} [get_ports STR2_PRD_re]
set_property -dict {PACKAGE_PIN L20 IOSTANDARD LVCMOS33} [get_ports STR2_PRD_ro]

set_property -dict {PACKAGE_PIN K23 IOSTANDARD LVCMOS33} [get_ports STR3_PRM_de]
set_property -dict {PACKAGE_PIN K28 IOSTANDARD LVCMOS33} [get_ports STR3_PRM_di]
set_property -dict {PACKAGE_PIN K24 IOSTANDARD LVCMOS33} [get_ports STR3_PRM_re]
set_property -dict {PACKAGE_PIN K29 IOSTANDARD LVCMOS33} [get_ports STR3_PRM_ro]

set_property -dict {PACKAGE_PIN J29 IOSTANDARD LVCMOS33} [get_ports STR4_PN_de]
set_property -dict {PACKAGE_PIN K25 IOSTANDARD LVCMOS33} [get_ports STR4_PN_di]
set_property -dict {PACKAGE_PIN K21 IOSTANDARD LVCMOS33} [get_ports STR4_PN_re]
set_property -dict {PACKAGE_PIN K26 IOSTANDARD LVCMOS33} [get_ports STR4_PN_ro]

set_property -dict {PACKAGE_PIN J26 IOSTANDARD LVCMOS33} [get_ports STR5_de]
set_property -dict {PACKAGE_PIN H29 IOSTANDARD LVCMOS33} [get_ports STR5_di]
set_property -dict {PACKAGE_PIN J24 IOSTANDARD LVCMOS33} [get_ports STR5_re]
set_property -dict {PACKAGE_PIN J21 IOSTANDARD LVCMOS33} [get_ports STR5_ro]

set_property -dict {PACKAGE_PIN J23 IOSTANDARD LVCMOS33} [get_ports STR6_de]
set_property -dict {PACKAGE_PIN G30 IOSTANDARD LVCMOS33} [get_ports STR6_di]
set_property -dict {PACKAGE_PIN J22 IOSTANDARD LVCMOS33} [get_ports STR6_re]
set_property -dict {PACKAGE_PIN H30 IOSTANDARD LVCMOS33} [get_ports STR6_ro]

set_property -dict {PACKAGE_PIN G29 IOSTANDARD LVCMOS33} [get_ports STR7_de]
set_property -dict {PACKAGE_PIN E30 IOSTANDARD LVCMOS33} [get_ports STR7_di]
set_property -dict {PACKAGE_PIN A30 IOSTANDARD LVCMOS33} [get_ports STR7_re]
set_property -dict {PACKAGE_PIN F30 IOSTANDARD LVCMOS33} [get_ports STR7_ro]

set_property -dict {PACKAGE_PIN D29 IOSTANDARD LVCMOS33} [get_ports STR8_de]
set_property -dict {PACKAGE_PIN B30 IOSTANDARD LVCMOS33} [get_ports STR8_di]
set_property -dict {PACKAGE_PIN E29 IOSTANDARD LVCMOS33} [get_ports STR8_re]
set_property -dict {PACKAGE_PIN C30 IOSTANDARD LVCMOS33} [get_ports STR8_ro]

# RS485 Transeivers - LTC2850
#RS1
set_property -dict {PACKAGE_PIN N20 IOSTANDARD LVCMOS33} [get_ports RS485_1_AFAR_de]
set_property -dict {PACKAGE_PIN N19 IOSTANDARD LVCMOS33} [get_ports RS485_1_AFAR_di]
set_property -dict {PACKAGE_PIN N21 IOSTANDARD LVCMOS33} [get_ports RS485_1_AFAR_re]
set_property -dict {PACKAGE_PIN N22 IOSTANDARD LVCMOS33} [get_ports RS485_1_AFAR_ro]
#RS2
set_property -dict {PACKAGE_PIN M28 IOSTANDARD LVCMOS33} [get_ports RS485_2_PC_de]
set_property -dict {PACKAGE_PIN M27 IOSTANDARD LVCMOS33} [get_ports RS485_2_PC_di]
set_property -dict {PACKAGE_PIN M29 IOSTANDARD LVCMOS33} [get_ports RS485_2_PC_re]
set_property -dict {PACKAGE_PIN M30 IOSTANDARD LVCMOS33} [get_ports RS485_2_PC_ro]
#RS3
set_property -dict {PACKAGE_PIN M23 IOSTANDARD LVCMOS33} [get_ports RS485_3_de]
set_property -dict {PACKAGE_PIN M22 IOSTANDARD LVCMOS33} [get_ports RS485_3_di]
set_property -dict {PACKAGE_PIN M24 IOSTANDARD LVCMOS33} [get_ports RS485_3_re]
set_property -dict {PACKAGE_PIN M25 IOSTANDARD LVCMOS33} [get_ports RS485_3_ro]
#RS4
set_property -dict {PACKAGE_PIN L30 IOSTANDARD LVCMOS33} [get_ports RS485_4_de]
set_property -dict {PACKAGE_PIN L28 IOSTANDARD LVCMOS33} [get_ports RS485_4_di]
set_property -dict {PACKAGE_PIN M19 IOSTANDARD LVCMOS33} [get_ports RS485_4_re]
set_property -dict {PACKAGE_PIN M20 IOSTANDARD LVCMOS33} [get_ports RS485_4_ro]

# UART <-> USB - FT232
set_property -dict {PACKAGE_PIN H25 IOSTANDARD LVCMOS33} [get_ports SIN_USB1]
set_property -dict {PACKAGE_PIN C26 IOSTANDARD LVCMOS33} [get_ports SOUT_USB1]
set_property -dict {PACKAGE_PIN E26 IOSTANDARD LVCMOS33} [get_ports RESET_USB1]
