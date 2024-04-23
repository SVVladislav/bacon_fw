set(DDR lmb_bram_0)
set(lmb_bram_0 "0x50;0xffb0")
set(axi_bram_0 "0xc2000000;0x1000")
set(TOTAL_MEM_CONTROLLERS "lmb_bram_0;axi_bram_0")
set(MEMORY_SECTION "MEMORY
{
	lmb_bram_0 : ORIGIN = 0x50, LENGTH = 0xffb0
	axi_bram_0 : ORIGIN = 0xc2000000, LENGTH = 0x1000
}")
set(STACK_SIZE 0x400)
set(HEAP_SIZE 0x400)
