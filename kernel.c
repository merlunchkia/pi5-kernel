#include <stdint.h>

#define GIO_AON_BASE   0x7D517C00UL

#define GIO0_ODEN   ((volatile uint32_t *)(GIO_AON_BASE + 0x00))
#define GIO0_DATA   ((volatile uint32_t *)(GIO_AON_BASE + 0x04))
#define GIO0_IODIR  ((volatile uint32_t *)(GIO_AON_BASE + 0x08))

#define ACT_LED_BIT 9
#define ACT_LED_MASK (1u << ACT_LED_BIT)

static void delay(volatile uint32_t count) {
    while (count--) {
        __asm__ volatile("nop");
    }
}

static void act_led_init(void) {
    /*
     * Make sure it is NOT open-drain.
     * Clear bit 9 in ODEN.
     */
    *GIO0_ODEN &= ~ACT_LED_MASK;

    /*
     * Set GPIO 9 as output.
     * For this controller: 0 = output, 1 = input
     * So we CLEAR the direction bit.
     */
    *GIO0_IODIR &= ~ACT_LED_MASK;

    /*
     * Start with LED off.
     * LED is active-low, so writing 1 turns it off.
     */
    *GIO0_DATA |= ACT_LED_MASK;
}

static void act_led_on(void) {
    /*
     * Active-low:
     * write 0 to turn LED on
     */
    *GIO0_DATA &= ~ACT_LED_MASK;
}

static void act_led_off(void) {
    /*
     * Active-low:
     * write 1 to turn LED off
     */
    *GIO0_DATA |= ACT_LED_MASK;
}

void kernel_main(void) {
    act_led_init();

    while (1) {
        act_led_on();
        delay(3000000);

        act_led_off();
        delay(3000000);
    }
}