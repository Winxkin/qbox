// __attribute__((noreturn)) void hang(void)
// {
//     while (1)
//     {
//         __asm__("wfi");
//     }
// }

// __attribute__((section (".exception"), interrupt, aligned (4))) void exception_handler(void)
// {
//     hang();
// }

void uart_driver_init(void) {};

static void uart_puts(const char *str)
{
    while (*str)
    {
        *(volatile unsigned int *)0xc0000000 = *str++;
    }
}

int main(void)
{
    /* Init UART */
    uart_driver_init();
    /* Print */
    uart_puts("Hello from RISV core");
    return 0;
}
