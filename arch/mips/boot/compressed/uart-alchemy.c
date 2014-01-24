#include <asm/mach-au1x00/au1000.h>

void putc(char c)
{
<<<<<<< HEAD
	/* all current (Jan. 2010) in-kernel boards */
	alchemy_uart_putchar(AU1000_UART0_PHYS_ADDR, c);
=======
#ifdef CONFIG_MIPS_DB1300
	alchemy_uart_putchar(AU1300_UART2_PHYS_ADDR, c);
#else
	alchemy_uart_putchar(AU1000_UART0_PHYS_ADDR, c);
#endif
>>>>>>> refs/remotes/origin/cm-10.0
}
