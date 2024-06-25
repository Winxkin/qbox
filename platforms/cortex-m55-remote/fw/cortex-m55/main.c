#include "nvic.h"

#define PASS 0
#define FAIL 1

#define DUMMY_REG_ADD 0xF0000000

void uart_driver_init(void)
{
}

void invalid_excp(void)
{
}

static void uart_puts(const char *str)
{
    while (*str) {
        *(volatile unsigned int *)0xc0000000 = *str++;
    }
}

void c_entry(void)
{
    nvic_enable_irq(0);

    uart_puts("Hello from cortex-m55!\r\n");
    *(volatile unsigned int *)DUMMY_REG_ADD = FAIL;

    while (1) {
        asm volatile ("wfi");
    }
}
