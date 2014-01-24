<<<<<<< HEAD

extern struct sys_timer footbridge_timer;
extern struct sys_timer isa_timer;
=======
#include <linux/reboot.h>

extern void footbridge_timer_init(void);
extern void isa_timer_init(void);
>>>>>>> refs/remotes/origin/master

extern void isa_rtc_init(void);

extern void footbridge_map_io(void);
extern void footbridge_init_irq(void);

extern void isa_init_irq(unsigned int irq);
<<<<<<< HEAD
<<<<<<< HEAD
=======
extern void footbridge_restart(char, const char *);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void footbridge_restart(enum reboot_mode, const char *);
>>>>>>> refs/remotes/origin/master
