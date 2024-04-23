#include "xuartns550.h"

XUartNs550_Config XUartNs550_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,axi-uart16550-2.0", /* compatible */
		0x44a20000, /* reg */
		0x0, /* xlnx,clock-freq */
		0x0, /* current-speed */
		0x2008, /* interrupts */
		0x41200001 /* interrupt-parent */
	},
	{
		"xlnx,axi-uart16550-2.0", /* compatible */
		0x44a30000, /* reg */
		0x0, /* xlnx,clock-freq */
		0x0, /* current-speed */
		0x2003, /* interrupts */
		0x41200001 /* interrupt-parent */
	},
	{
		"xlnx,axi-uart16550-2.0", /* compatible */
		0x44a50000, /* reg */
		0x0, /* xlnx,clock-freq */
		0x0, /* current-speed */
		0x2002, /* interrupts */
		0x41200001 /* interrupt-parent */
	},
	 {
		 NULL
	}
};