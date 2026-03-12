#include <stdint.h>

#define RP1_UART0_BASE  0x1F00030000UL

#define UART_DR    ((volatile uint32_t *)(RP1_UART0_BASE + 0x00))
#define UART_FR    ((volatile uint32_t *)(RP1_UART0_BASE + 0x18))
#define UART_IBRD  ((volatile uint32_t *)(RP1_UART0_BASE + 0x24))
#define UART_FBRD  ((volatile uint32_t *)(RP1_UART0_BASE + 0x28))
#define UART_LCRH  ((volatile uint32_t *)(RP1_UART0_BASE + 0x2C))
#define UART_CR    ((volatile uint32_t *)(RP1_UART0_BASE + 0x30))
#define UART_IMSC  ((volatile uint32_t *)(RP1_UART0_BASE + 0x38))
#define UART_ICR   ((volatile uint32_t *)(RP1_UART0_BASE + 0x44))

static void uart_init(void) {
    *UART_CR = 0x00000000;
    *UART_ICR = 0x7FF;

    *UART_IBRD = 26;
    *UART_FBRD = 3;

    *UART_LCRH = (1 << 4) | (3 << 5);
    *UART_IMSC = 0x00000000;
    *UART_CR = (1 << 0) | (1 << 8) | (1 << 9);
}

static void uart_putc(char c) {
    while (*UART_FR & (1 << 5)) {
    }
    *UART_DR = (uint32_t)c;
}

static void uart_puts(const char *s) {
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r');
        }
        uart_putc(*s++);
    }
}

static void uart_hex(uint64_t x) {
    for (int i = 60; i >= 0; i -= 4) {
        uint8_t nibble = (x >> i) & 0xF;
        if (nibble < 10) {
            uart_putc('0' + nibble);
        } else {
            uart_putc('A' + (nibble - 10));
        }
    }
}

void kernel_main(void) {
    uart_init();

    uart_puts("Hello from bare-metal Raspberry Pi 5!\n");
    uart_puts("RP1 UART0 is alive.\n");
    uart_puts("UART base = 0x");
    uart_hex(RP1_UART0_BASE);
    uart_puts("\n");

    while (1) {
        __asm__ volatile("wfe");
    }
}
