/*
 * altera_uart.h -- Altera UART driver defines.
 */

#ifndef	__ALTUART_H
#define	__ALTUART_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/init.h>

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct altera_uart_platform_uart {
	unsigned long mapbase;	/* Physical address base */
	unsigned int irq;	/* Interrupt vector */
	unsigned int uartclk;	/* UART clock rate */
	unsigned int bus_shift;	/* Bus shift (address stride) */
};

<<<<<<< HEAD
<<<<<<< HEAD
int __init early_altera_uart_setup(struct altera_uart_platform_uart *platp);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* __ALTUART_H */
