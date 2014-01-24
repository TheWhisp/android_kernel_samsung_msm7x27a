#ifndef _ASM_X86_TIME_H
#define _ASM_X86_TIME_H

<<<<<<< HEAD
extern void hpet_time_init(void);

#include <asm/mc146818rtc.h>

extern void time_init(void);

=======
#include <linux/clocksource.h>
#include <asm/mc146818rtc.h>

extern void hpet_time_init(void);
extern void time_init(void);

extern struct clock_event_device *global_clock_event;

>>>>>>> refs/remotes/origin/cm-10.0
#endif /* _ASM_X86_TIME_H */
