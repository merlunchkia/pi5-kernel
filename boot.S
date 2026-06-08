.section ".text.boot"

.global _start
_start:
    mov x19, x0

    mrs x20, CurrentEL
    lsr x20, x20, #2
    and x20, x20, #3

    ldr x1, =__bss_start
    ldr x2, =__bss_end
    sub x3, x2, x1
    cbz x3, 2f
    mov x0, xzr
1:  str x0, [x1], #8
    subs x3, x3, #8
    b.ne 1b
2:

    ldr x1, =_stack_top
    mov sp, x1

    mov w0, w20
    mov x1, x19
    bl kernel_main

3:  wfe
    b 3b

.section ".bss.stack", "aw", @nobits
.align 12
_stack_bottom:
    .skip 16384
.global _stack_top
_stack_top:
