
	.section	.text
	.align	2
	.global	MIDR
MIDR:
	mrc p15, 0, r0, c0, c0, 0
	mov pc, lr

	.align	2
	.global	SCTLR
SCTLR:
	mrc p15, 0, r0, c1, c0, 0
	mov pc, lr

	.align	2
	.global	mmu_enable
mmu_enable:
	mrc p15, 0, r0, c1, c0, 0
	orr r0, r0, #1
	mcr p15, 0, r0, c1, c0, 0
	mov pc, lr
	
	.align	2
	.global	TTBCR
TTBCR:
	mrc p15, 0, r0, c2, c0, 2
	mov pc, lr

	.align	2
	.global	set_ttb
set_ttb:
	mcr p15, 0, r0, c2, c0, 0
	mov pc, lr

	.align	2
	.global	set_domain
set_domain:
	mcr p15, 0, r0, c3, c0, 0
	mov pc, lr

	.align	2
	.global	clear_tlb
clear_tlb:
	mcr p15, 0, r0, c8, c7, 0
	mov pc, lr

	.align	2
	.global	enable_cache
enable_cache:
	mrc p15, 0, r0, c1, c0, 0
	orr r0, r0, #(1<<2)
	orr r0, r0, #(1<<12)
	mcr p15, 0, r0, c1, c0, 0
	mov pc, lr
	
	.align	2
	.global	set_h_vector
set_h_vector:
	mrc p15, 0, r0, c1, c0, 0
	orr r0, r0, #(1<<13)
	mcr p15, 0, r0, c1, c0, 0
	mov pc, lr	


