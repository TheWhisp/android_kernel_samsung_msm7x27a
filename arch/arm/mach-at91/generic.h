/*
 * linux/arch/arm/mach-at91/generic.h
 *
 *  Copyright (C) 2005 David Brownell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/clkdev.h>
<<<<<<< HEAD
<<<<<<< HEAD

 /* Map io */
extern void __init at91rm9200_map_io(void);
extern void __init at91sam9260_map_io(void);
extern void __init at91sam9261_map_io(void);
extern void __init at91sam9263_map_io(void);
extern void __init at91sam9rl_map_io(void);
extern void __init at91sam9g45_map_io(void);
extern void __init at91x40_map_io(void);
extern void __init at91cap9_map_io(void);

 /* Processors */
extern void __init at91rm9200_set_type(int type);
extern void __init at91rm9200_initialize(unsigned long main_clock);
extern void __init at91sam9260_initialize(unsigned long main_clock);
extern void __init at91sam9261_initialize(unsigned long main_clock);
extern void __init at91sam9263_initialize(unsigned long main_clock);
extern void __init at91sam9rl_initialize(unsigned long main_clock);
extern void __init at91sam9g45_initialize(unsigned long main_clock);
extern void __init at91x40_initialize(unsigned long main_clock);
extern void __init at91cap9_initialize(unsigned long main_clock);

 /* Interrupts */
extern void __init at91rm9200_init_interrupts(unsigned int priority[]);
extern void __init at91sam9260_init_interrupts(unsigned int priority[]);
extern void __init at91sam9261_init_interrupts(unsigned int priority[]);
extern void __init at91sam9263_init_interrupts(unsigned int priority[]);
extern void __init at91sam9rl_init_interrupts(unsigned int priority[]);
extern void __init at91sam9g45_init_interrupts(unsigned int priority[]);
extern void __init at91x40_init_interrupts(unsigned int priority[]);
extern void __init at91cap9_init_interrupts(unsigned int priority[]);
extern void __init at91_aic_init(unsigned int priority[]);

 /* Timer */
struct sys_timer;
extern struct sys_timer at91rm9200_timer;
=======
#include <linux/of.h>
=======
#include <linux/of.h>
#include <linux/reboot.h>
>>>>>>> refs/remotes/origin/master

 /* Map io */
extern void __init at91_map_io(void);
extern void __init at91_init_sram(int bank, unsigned long base,
				  unsigned int length);

 /* Processors */
extern void __init at91rm9200_set_type(int type);
extern void __init at91_initialize(unsigned long main_clock);
extern void __init at91x40_initialize(unsigned long main_clock);
<<<<<<< HEAD
=======
extern void __init at91rm9200_dt_initialize(void);
>>>>>>> refs/remotes/origin/master
extern void __init at91_dt_initialize(void);

 /* Interrupts */
extern void __init at91_init_irq_default(void);
extern void __init at91_init_interrupts(unsigned int priority[]);
extern void __init at91x40_init_interrupts(unsigned int priority[]);
<<<<<<< HEAD
extern void __init at91_aic_init(unsigned int priority[]);
extern int  __init at91_aic_of_init(struct device_node *node,
				    struct device_node *parent);


 /* Timer */
struct sys_timer;
extern void at91rm9200_ioremap_st(u32 addr);
extern struct sys_timer at91rm9200_timer;
extern void at91sam926x_ioremap_pit(u32 addr);
>>>>>>> refs/remotes/origin/cm-10.0
extern struct sys_timer at91sam926x_timer;
extern struct sys_timer at91x40_timer;

 /* Clocks */
<<<<<<< HEAD
extern int __init at91_clock_init(unsigned long main_clock);
=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * function to specify the clock of the default console. As we do not
 * use the device/driver bus, the dev_name is not intialize. So we need
 * to link the clock to a specific con_id only "usart"
 */
extern void __init at91rm9200_set_console_clock(int id);
extern void __init at91sam9260_set_console_clock(int id);
extern void __init at91sam9261_set_console_clock(int id);
extern void __init at91sam9263_set_console_clock(int id);
extern void __init at91sam9rl_set_console_clock(int id);
extern void __init at91sam9g45_set_console_clock(int id);
<<<<<<< HEAD
extern void __init at91cap9_set_console_clock(int id);
=======
=======
extern void __init at91_aic_init(unsigned int priority[],
				 unsigned int ext_irq_mask);
extern int  __init at91_aic_of_init(struct device_node *node,
				    struct device_node *parent);
extern int  __init at91_aic5_of_init(struct device_node *node,
				    struct device_node *parent);
extern void __init at91_sysirq_mask_rtc(u32 rtc_base);
extern void __init at91_sysirq_mask_rtt(u32 rtt_base);


 /* Timer */
extern void at91rm9200_ioremap_st(u32 addr);
extern void at91rm9200_timer_init(void);
extern void at91sam926x_ioremap_pit(u32 addr);
extern void at91sam926x_pit_init(void);
extern void at91x40_timer_init(void);

 /* Clocks */
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_AT91_PMC_UNIT
extern int __init at91_clock_init(unsigned long main_clock);
extern int __init at91_dt_clock_init(void);
#else
static int inline at91_clock_init(unsigned long main_clock) { return 0; }
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct device;

 /* Power Management */
extern void at91_irq_suspend(void);
extern void at91_irq_resume(void);

<<<<<<< HEAD
<<<<<<< HEAD
/* reset */
extern void at91sam9_alt_reset(void);
=======
=======
>>>>>>> refs/remotes/origin/master
/* idle */
extern void at91sam9_idle(void);

/* reset */
extern void at91_ioremap_rstc(u32 base_addr);
<<<<<<< HEAD
extern void at91sam9_alt_restart(char, const char *);
extern void at91sam9g45_restart(char, const char *);
=======
extern void at91sam9_alt_restart(enum reboot_mode, const char *);
extern void at91sam9g45_restart(enum reboot_mode, const char *);
>>>>>>> refs/remotes/origin/master

/* shutdown */
extern void at91_ioremap_shdwc(u32 base_addr);

/* Matrix */
extern void at91_ioremap_matrix(u32 base_addr);

/* Ram Controler */
extern void at91_ioremap_ramc(int id, u32 addr, u32 size);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

 /* GPIO */
#define AT91RM9200_PQFP		3	/* AT91RM9200 PQFP package has 3 banks */
#define AT91RM9200_BGA		4	/* AT91RM9200 BGA package has 4 banks */

struct at91_gpio_bank {
	unsigned short id;		/* peripheral ID */
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long offset;		/* offset from system peripheral base */
	struct clk *clock;		/* associated clock */
};
extern void __init at91_gpio_init(struct at91_gpio_bank *, int nr_banks);
extern void __init at91_gpio_irq_setup(void);

extern void (*at91_arch_reset)(void);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long regbase;		/* offset from system peripheral base */
};
extern void __init at91_gpio_init(struct at91_gpio_bank *, int nr_banks);
extern void __init at91_gpio_irq_setup(void);
extern int  __init at91_gpio_of_irq_setup(struct device_node *node,
					  struct device_node *parent);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
extern int at91_extern_irq;
=======
extern u32 at91_get_extern_irq(void);
>>>>>>> refs/remotes/origin/master
