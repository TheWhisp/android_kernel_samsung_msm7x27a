#include <linux/tty.h>
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/semaphore.h>
#include <linux/sched.h>

<<<<<<< HEAD
/*
 * The 'big tty mutex'
 *
 * This mutex is taken and released by tty_lock() and tty_unlock(),
 * replacing the older big kernel lock.
 * It can no longer be taken recursively, and does not get
 * released implicitly while sleeping.
 *
 * Don't use in new code.
 */
static DEFINE_MUTEX(big_tty_mutex);
<<<<<<< HEAD
struct task_struct *__big_tty_mutex_owner;
EXPORT_SYMBOL_GPL(__big_tty_mutex_owner);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Legacy tty mutex glue */

enum {
	TTY_MUTEX_NORMAL,
	TTY_MUTEX_NESTED,
};
>>>>>>> refs/remotes/origin/master

/*
 * Getting the big tty mutex.
 */
<<<<<<< HEAD
void __lockfunc tty_lock(void)
{
<<<<<<< HEAD
	struct task_struct *task = current;

	WARN_ON(__big_tty_mutex_owner == task);

	mutex_lock(&big_tty_mutex);
	__big_tty_mutex_owner = task;
=======
	mutex_lock(&big_tty_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(tty_lock);

void __lockfunc tty_unlock(void)
{
<<<<<<< HEAD
	struct task_struct *task = current;

	WARN_ON(__big_tty_mutex_owner != task);
	__big_tty_mutex_owner = NULL;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&big_tty_mutex);
}
EXPORT_SYMBOL(tty_unlock);
=======

static void __lockfunc tty_lock_nested(struct tty_struct *tty,
				       unsigned int subclass)
{
	if (tty->magic != TTY_MAGIC) {
		pr_err("L Bad %p\n", tty);
		WARN_ON(1);
		return;
	}
	tty_kref_get(tty);
	mutex_lock_nested(&tty->legacy_mutex, subclass);
}

void __lockfunc tty_lock(struct tty_struct *tty)
{
	return tty_lock_nested(tty, TTY_MUTEX_NORMAL);
}
EXPORT_SYMBOL(tty_lock);

void __lockfunc tty_unlock(struct tty_struct *tty)
{
	if (tty->magic != TTY_MAGIC) {
		pr_err("U Bad %p\n", tty);
		WARN_ON(1);
		return;
	}
	mutex_unlock(&tty->legacy_mutex);
	tty_kref_put(tty);
}
EXPORT_SYMBOL(tty_unlock);

/*
 * Getting the big tty mutex for a pair of ttys with lock ordering
 * On a non pty/tty pair tty2 can be NULL which is just fine.
 */
void __lockfunc tty_lock_pair(struct tty_struct *tty,
					struct tty_struct *tty2)
{
	if (tty < tty2) {
		tty_lock(tty);
		tty_lock_nested(tty2, TTY_MUTEX_NESTED);
	} else {
		if (tty2 && tty2 != tty)
			tty_lock(tty2);
		tty_lock_nested(tty, TTY_MUTEX_NESTED);
	}
}
EXPORT_SYMBOL(tty_lock_pair);

void __lockfunc tty_unlock_pair(struct tty_struct *tty,
						struct tty_struct *tty2)
{
	tty_unlock(tty);
	if (tty2 && tty2 != tty)
		tty_unlock(tty2);
}
EXPORT_SYMBOL(tty_unlock_pair);
>>>>>>> refs/remotes/origin/master
