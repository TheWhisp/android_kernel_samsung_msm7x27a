<<<<<<< HEAD
#define ARRAY_AND_SIZE(x)	(x), ARRAY_SIZE(x)

struct sys_timer;

extern void timer_init(int irq);

extern void __init icu_init_irq(void);
extern void __init mmp_map_io(void);
<<<<<<< HEAD
=======
extern void mmp_restart(char, const char *);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/reboot.h>
#define ARRAY_AND_SIZE(x)	(x), ARRAY_SIZE(x)

extern void timer_init(int irq);

extern void __init mmp_map_io(void);
extern void mmp_restart(enum reboot_mode, const char *);
extern void __init pxa168_clk_init(void);
extern void __init pxa910_clk_init(void);
extern void __init mmp2_clk_init(void);
>>>>>>> refs/remotes/origin/master
