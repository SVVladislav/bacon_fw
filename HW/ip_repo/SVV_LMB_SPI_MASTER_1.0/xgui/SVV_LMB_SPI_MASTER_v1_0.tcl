# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"

}

proc update_PARAM_VALUE.ADDRES { PARAM_VALUE.ADDRES } {
	# Procedure called to update ADDRES when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.ADDRES { PARAM_VALUE.ADDRES } {
	# Procedure called to validate ADDRES
	return true
}

proc update_PARAM_VALUE.SLAVES { PARAM_VALUE.SLAVES } {
	# Procedure called to update SLAVES when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.SLAVES { PARAM_VALUE.SLAVES } {
	# Procedure called to validate SLAVES
	return true
}


proc update_MODELPARAM_VALUE.SLAVES { MODELPARAM_VALUE.SLAVES PARAM_VALUE.SLAVES } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.SLAVES}] ${MODELPARAM_VALUE.SLAVES}
}

proc update_MODELPARAM_VALUE.ADDRES { MODELPARAM_VALUE.ADDRES PARAM_VALUE.ADDRES } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.ADDRES}] ${MODELPARAM_VALUE.ADDRES}
}

