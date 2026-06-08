#include "types.h"
#include "mmap.h"
#include "uart.h"

extern uint64_t _end;
extern uint64_t _stack_top;

void kernel_main(uint32_t el, uint64_t dtb)
{
    uint64_t freq;
    __asm__ volatile("mrs %0, CNTFRQ_EL0" : "=r"(freq));

    uart_init();

    uart_puts("\r\n[BOOT] pi5-kernel starting\r\n");
    uart_puts("[BOOT] Exception Level: ");
    uart_hex(el);
    uart_puts("\r\n");
    uart_puts("[BOOT] DTB address:   0x");
    uart_hex(dtb);
    uart_puts("\r\n");
    uart_puts("[BOOT] Kernel end:    0x");
    uart_hex((uint64_t)&_end);
    uart_puts("\r\n");
    uart_puts("[BOOT] Stack top:     0x");
    uart_hex((uint64_t)&_stack_top);
    uart_puts("\r\n");
    uart_puts("[BOOT] Timer freq:    ");
    uart_hex(freq);
    uart_puts(" Hz\r\n");
    uart_puts("[BOOT] --- init complete, entering main loop ---\r\n");

    while (1) {
        __asm__ volatile("wfe");
    }
}
