#include "xuartlite.h"

XUartLite_Config XUartLite_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,mdm-3.2", /* compatible */
		0x41400000, /* reg */
		0x0, /* xlnx,baudrate */
		0x0, /* xlnx,use-parity */
		0x0, /* xlnx,odd-parity */
		0x0, /* xlnx,data-bits */
		0xc, /* interrupts */
		0x41200001 /* interrupt-parent */
	},
	 {
		 NULL
	}
};