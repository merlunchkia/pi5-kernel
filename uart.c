#include "types.h"
#include "mmap.h"
#include "uart.h"

#define UART_BASE   DEBUG_UART_BASE

#define REG_DR    ((reg32 *)(UART_BASE + 0x000))
#define REG_FR    ((reg32 *)(UART_BASE + 0x018))
#define REG_IBRD  ((reg32 *)(UART_BASE + 0x024))
#define REG_FBRD  ((reg32 *)(UART_BASE + 0x028))
#define REG_LCR_H ((reg32 *)(UART_BASE + 0x02C))
#define REG_CR    ((reg32 *)(UART_BASE + 0x030))

#define FR_TXFF   (1 << 5)

static void uart_flush(void)
{
    __asm__ volatile("dmb sy");
}

void uart_init(void)
{
    *REG_CR = 0;
    uart_flush();

    *REG_IBRD = 5;
    *REG_FBRD = 0;

    *REG_LCR_H = (0b11 << 5) | (1 << 4);
    uart_flush();

    *REG_CR = (1 << 0) | (1 << 8) | (1 << 9);
    uart_flush();
}

void uart_putc(char c)
{
    while (*REG_FR & FR_TXFF) {
        __asm__ volatile("nop");
    }
    *REG_DR = (uint32_t)(unsigned char)c;
}

void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}

void uart_hex(uint64_t x)
{
    for (int i = 60; i >= 0; i -= 4) {
        uint8_t nibble = (uint8_t)((x >> i) & 0xF);
        if (nibble < 10)
            uart_putc('0' + nibble);
        else
            uart_putc('A' + (nibble - 10));
    }
}
