
	.section	.rodata
	.align	2
.LC0:
	.string	"val = %d\n"
	.align	2
.LC1:
	.string	"%d"

	.section	.text
	.align	2
	.global	main
main:
	mov r10, lr		@保存_start返回地址到r10 

	@int ar[10];
	sub sp, sp, #20	@开辟空间;
	mov r5, sp		@r5指向数组首地址;

	mov r4, #0
1:
	cmp	r4, #10
	bge	1f

	strh r4, [r5], #2

	add r4, #1
	b	1b
1:

	mov r5, sp
	mov r4, #0
1:
	cmp	r4, #10
	bge	1f
	
	ldrh r1, [r5], #2
	@ldr r1, [r5], #4
	@ldr r1, [r5, r4, lsl #1]
	@ldr r1, [r5, r4]
	ldr r0, =.LC0
	bl	printf

	@add r5, #4
	add r4, #1
	b	1b
1:

	mov r5, sp
	ldrh r1, [r5, #8]!
	ldr r0, =.LC0
	bl	printf

	mov r5, sp
	ldrh r1, [r5, #(10)]
	ldr r0, =.LC0
	bl	printf

	@ldr r1, [r5, #4095];
	@ldr r0, =.LC0
	@bl	printf

	@add sp, sp, #40		@释放空间;
	add sp, sp, #20		@释放空间;
	mov r0, #0
	mov lr, r10
	mov pc, lr
