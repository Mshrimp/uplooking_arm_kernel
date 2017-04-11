#include <config.h>
#include <board.h>
#include <string.h>
#include <gic.h>

extern void vector_start();
extern void vector_end();

u32 *ttb1 = (void*)0x60000000;	//16K对齐的地址;


int main(void)
{
	printf("main start\n");
	memset(ttb1, 0, 16 * 1024); 
	mmu_l1_init_uboot(ttb1);
	mmu_l1_mmap(ttb1, 0xffff0000, 0x7fff0000);
	memcpy((void*)0x7fff0000, vector_start, vector_end - vector_start);

	asm __volatile__(
		"mcr p15, 0, r0, c8, c7, 0\n\t"		//clear_tlb
		"mcr p15, 0, %0, c2, c0, 0\n\t"		//set_ttb
		"mcr p15, 0, %1, c3, c0, 0\n\t"		//set_domain
		"mrc p15, 0, r0, c1, c0, 0\n\t"		//
		"orr r0, r0, #(1<<13)\n\t"			//V
		"orr r0, r0, #1\n\t"				//M
		"mcr p15, 0, r0, c1, c0, 0\n\t"		//SCTLR 
		: 
		: "r" (ttb1), "r" (0xffffffff) 
		: "memory", "r0"
	);

	asm __volatile(
		"mrs r0, cpsr\n\t"
		"bic r0, #(1<<7)\n\t"
		"msr cpsr, r0\n\t"
		::: "r0"
	);
	
	/*
	asm __volatile__(
		".word	0xffffffff"
	);
	*/

	//SGI: CPU0 2
	//cpu:
	set_one(ICCICR_CPU0, 0);
	set_nbit(ICCPMR_CPU0, 0, 8, 255);

	//ICD:
	set_one(ICDDCR, 0);
	set_one(ICDISER0_CPU0, 13);
	set_nbit(ICDIPR0_CPU0, 16, 8, 254);
	set_nbit(ICDIPTR0_CPU0, 16, 8, 1);

	//SGI: send IRQID:2 to cpu0
	set_val(ICDSGIR, (1<<16)|13);

	printf("main end\n");
	return 0;
}	

asm (
	".align	2\n\t"
	".global	vector_start\n"
"vector_start:\n\t"
	"nop\n\t"
	"ldr pc, _undef\n\t"
	"ldr pc, _swi\n\t"
	"ldr pc, _pabort\n\t"
	"ldr pc, _dabort\n\t"
	"nop\n\t"			
	"ldr pc, _irq\n\t"
	"ldr pc, _firq\n"
"_undef:\n\t"
	".word	undef\n"
"_swi:\n\t"
	".word	swi\n"
"_pabort:\n\t"
	".word	pabort\n"
"_dabort:\n\t"
	".word	dabort\n"
"_irq:\n\t"
	".word	irq\n"
"_firq:\n\t"
	".word	firq\n"

	".global vector_end\n"
"vector_end:\n\t"
	"nop\n\t"
);

void do_undef(unsigned int s)
{
	printf("s = %p\n", s);
}

void do_swi(unsigned int n)
{
	printf("system call num %d\n", n);
}

void do_irq(void)
{
	u32 tmp = get_val(ICCIAR_CPU0);
	printf("CPUID: %d IRQID: %d\n", (tmp >> 10) & 0x7, tmp & 0x3ff);
}

void do_firq(void)
{

}





