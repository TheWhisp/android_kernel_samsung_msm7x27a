/* 
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

<<<<<<< HEAD
#include "linux/sched.h"
#include "linux/slab.h"
#include "kern_util.h"
#include "os.h"
#include "skas.h"
=======
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/oom.h>
#include <kern_util.h>
#include <os.h>
#include <skas.h>
>>>>>>> refs/remotes/origin/master

void (*pm_power_off)(void);

static void kill_off_processes(void)
{
	if (proc_mm)
		/*
		 * FIXME: need to loop over userspace_pids
		 */
		os_kill_ptraced_process(userspace_pid[0], 1);
	else {
		struct task_struct *p;
<<<<<<< HEAD
<<<<<<< HEAD
		int pid, me;

		me = os_getpid();
=======
		int pid;

>>>>>>> refs/remotes/origin/cm-10.0
		for_each_process(p) {
			if (p->mm == NULL)
				continue;

			pid = p->mm->context.id.u.pid;
			os_kill_ptraced_process(pid, 1);
		}
=======
		int pid;

		read_lock(&tasklist_lock);
		for_each_process(p) {
			struct task_struct *t;

			t = find_lock_task_mm(p);
			if (!t)
				continue;
			pid = t->mm->context.id.u.pid;
			task_unlock(t);
			os_kill_ptraced_process(pid, 1);
		}
		read_unlock(&tasklist_lock);
>>>>>>> refs/remotes/origin/master
	}
}

void uml_cleanup(void)
{
	kmalloc_ok = 0;
	do_uml_exitcalls();
	kill_off_processes();
}

void machine_restart(char * __unused)
{
	uml_cleanup();
	reboot_skas();
}

void machine_power_off(void)
{
	uml_cleanup();
	halt_skas();
}

void machine_halt(void)
{
	machine_power_off();
}
