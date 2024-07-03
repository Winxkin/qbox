// #include "nvic.h"

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

void main(void)
{
    // nvic_enable_irq(0);

    uart_puts("Hello from risc-v 64 core!\r\n");

    // *(volatile unsigned int *)0xE002E000 = 0x00;

    
}
