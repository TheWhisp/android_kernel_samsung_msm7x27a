#ifndef BCM63XX_GPIO_H
#define BCM63XX_GPIO_H

#include <linux/init.h>
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <bcm63xx_cpu.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <bcm63xx_cpu.h>
>>>>>>> refs/remotes/origin/master
=======
#include <bcm63xx_cpu.h>
>>>>>>> refs/remotes/origin/cm-11.0

int __init bcm63xx_gpio_init(void);

static inline unsigned long bcm63xx_gpio_count(void)
{
	switch (bcm63xx_get_cpu_id()) {
<<<<<<< HEAD
=======
	case BCM6328_CPU_ID:
		return 32;
	case BCM3368_CPU_ID:
>>>>>>> refs/remotes/origin/master
	case BCM6358_CPU_ID:
		return 40;
	case BCM6338_CPU_ID:
		return 8;
	case BCM6345_CPU_ID:
		return 16;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case BCM6368_CPU_ID:
		return 38;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case BCM6362_CPU_ID:
		return 48;
	case BCM6368_CPU_ID:
		return 38;
>>>>>>> refs/remotes/origin/master
	case BCM6348_CPU_ID:
	default:
		return 37;
	}
}

#define BCM63XX_GPIO_DIR_OUT	0x0
#define BCM63XX_GPIO_DIR_IN	0x1

#endif /* !BCM63XX_GPIO_H */
