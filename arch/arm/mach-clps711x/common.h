/*
 * linux/arch/arm/mach-clps711x/common.h
 *
 * Common bits.
 */

<<<<<<< HEAD
struct sys_timer;

extern void clps711x_map_io(void);
extern void clps711x_init_irq(void);
extern struct sys_timer clps711x_timer;
<<<<<<< HEAD
=======
extern void clps711x_restart(char mode, const char *cmd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/reboot.h>

#define CLPS711X_NR_IRQS	(33)
#define CLPS711X_NR_GPIO	(4 * 8 + 3)
#define CLPS711X_GPIO(prt, bit)	((prt) * 8 + (bit))

extern void clps711x_map_io(void);
extern void clps711x_init_irq(void);
extern void clps711x_timer_init(void);
extern void clps711x_handle_irq(struct pt_regs *regs);
extern void clps711x_restart(enum reboot_mode mode, const char *cmd);
extern void clps711x_init_early(void);
>>>>>>> refs/remotes/origin/master
