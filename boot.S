.section ".text.boot"

.global _start
_start:
	ldr x0, =_stack_top
	mov sp, x0

	bl kernel_main

1:
	wfe
	b 1b

.section ".bss.stack"
.align 12
_stack_bottom:
	.skip 4096
_stack_top: