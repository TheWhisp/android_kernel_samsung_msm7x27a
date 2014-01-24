/*
 *	Variables and functions used by the code in sleep.c
 */

<<<<<<< HEAD
#include <asm/trampoline.h>
<<<<<<< HEAD
=======
#include <linux/linkage.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/realmode.h>
>>>>>>> refs/remotes/origin/master

extern unsigned long saved_video_mode;
extern long saved_magic;

extern int wakeup_pmode_return;

<<<<<<< HEAD
<<<<<<< HEAD
=======
extern u8 wake_sleep_flags;
extern asmlinkage void acpi_enter_s3(void);

>>>>>>> refs/remotes/origin/cm-10.0
=======
extern u8 wake_sleep_flags;

>>>>>>> refs/remotes/origin/master
extern unsigned long acpi_copy_wakeup_routine(unsigned long);
extern void wakeup_long64(void);

extern void do_suspend_lowlevel(void);
<<<<<<< HEAD
=======

extern int x86_acpi_suspend_lowlevel(void);

acpi_status asmlinkage x86_acpi_enter_sleep_state(u8 state);
>>>>>>> refs/remotes/origin/master
