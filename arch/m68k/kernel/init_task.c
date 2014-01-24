/*
 *  linux/arch/m68knommu/kernel/init_task.c
 */
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/fs.h>
#include <linux/mqueue.h>

#include <asm/uaccess.h>
#include <asm/pgtable.h>

static struct signal_struct init_signals = INIT_SIGNALS(init_signals);
static struct sighand_struct init_sighand = INIT_SIGHAND(init_sighand);
/*
 * Initial task structure.
 *
 * All other task structs will be allocated on slabs in fork.c
 */
<<<<<<< HEAD
__asm__(".align 4");
=======
>>>>>>> refs/remotes/origin/cm-10.0
struct task_struct init_task = INIT_TASK(init_task);

EXPORT_SYMBOL(init_task);

/*
 * Initial thread structure.
 *
<<<<<<< HEAD
 * We need to make sure that this is 8192-byte aligned due to the
=======
 * We need to make sure that this is THREAD size aligned due to the
>>>>>>> refs/remotes/origin/cm-10.0
 * way process stacks are handled. This is done by having a special
 * "init_task" linker map entry..
 */
union thread_union init_thread_union __init_task_data =
	{ INIT_THREAD_INFO(init_task) };

