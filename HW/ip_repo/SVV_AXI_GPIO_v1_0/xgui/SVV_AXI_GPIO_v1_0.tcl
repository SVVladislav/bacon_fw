# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "GPIO_WIDTH" -parent ${Page_0}
  set ODR_INIT [ipgui::add_param $IPINST -name "ODR_INIT" -parent ${Page_0}]
  set_property tooltip {Power on ODR state} ${ODR_INIT}
  set DIR_INIT [ipgui::add_param $IPINST -name "DIR_INIT" -parent ${Page_0}]
  set_property tooltip {Power on DIR state} ${DIR_INIT}


}

proc update_PARAM_VALUE.DIR_INIT { PARAM_VALUE.DIR_INIT } {
	# Procedure called to update DIR_INIT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.DIR_INIT { PARAM_VALUE.DIR_INIT } {
	# Procedure called to validate DIR_INIT
	return true
}

proc update_PARAM_VALUE.GPIO_WIDTH { PARAM_VALUE.GPIO_WIDTH } {
	# Procedure called to update GPIO_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.GPIO_WIDTH { PARAM_VALUE.GPIO_WIDTH } {
	# Procedure called to validate GPIO_WIDTH
	return true
}

proc update_PARAM_VALUE.ODR_INIT { PARAM_VALUE.ODR_INIT } {
	# Procedure called to update ODR_INIT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.ODR_INIT { PARAM_VALUE.ODR_INIT } {
	# Procedure called to validate ODR_INIT
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to update C_S00_AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to validate C_S00_AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to update C_S00_AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to validate C_S00_AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to update C_S00_AXI_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to validate C_S00_AXI_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to update C_S00_AXI_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to validate C_S00_AXI_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH { MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.GPIO_WIDTH { MODELPARAM_VALUE.GPIO_WIDTH PARAM_VALUE.GPIO_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.GPIO_WIDTH}] ${MODELPARAM_VALUE.GPIO_WIDTH}
}

proc update_MODELPARAM_VALUE.ODR_INIT { MODELPARAM_VALUE.ODR_INIT PARAM_VALUE.ODR_INIT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.ODR_INIT}] ${MODELPARAM_VALUE.ODR_INIT}
}

proc update_MODELPARAM_VALUE.DIR_INIT { MODELPARAM_VALUE.DIR_INIT PARAM_VALUE.DIR_INIT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DIR_INIT}] ${MODELPARAM_VALUE.DIR_INIT}
}

