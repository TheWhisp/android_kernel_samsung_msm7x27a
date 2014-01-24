/*
 * Copyright (C) 2008 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */
#include <linux/sched.h>
#include <linux/pagemap.h>
#include <linux/spinlock.h>
#include <linux/page-flags.h>
#include <asm/bug.h>
#include "ctree.h"
#include "extent_io.h"
#include "locking.h"

<<<<<<< HEAD
<<<<<<< HEAD
static inline void spin_nested(struct extent_buffer *eb)
{
	spin_lock(&eb->lock);
}

/*
 * Setting a lock to blocking will drop the spinlock and set the
 * flag that forces other procs who want the lock to wait.  After
 * this you can safely schedule with the lock held.
 */
void btrfs_set_lock_blocking(struct extent_buffer *eb)
{
	if (!test_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags)) {
		set_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags);
		spin_unlock(&eb->lock);
	}
	/* exit with the spin lock released and the bit set */
}

/*
 * clearing the blocking flag will take the spinlock again.
 * After this you can't safely schedule
 */
void btrfs_clear_lock_blocking(struct extent_buffer *eb)
{
	if (test_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags)) {
		spin_nested(eb);
		clear_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags);
		smp_mb__after_clear_bit();
	}
	/* exit with the spin lock held */
}

/*
 * unfortunately, many of the places that currently set a lock to blocking
 * don't end up blocking for very long, and often they don't block
 * at all.  For a dbench 50 run, if we don't spin on the blocking bit
 * at all, the context switch rate can jump up to 400,000/sec or more.
 *
 * So, we're still stuck with this crummy spin on the blocking bit,
 * at least until the most common causes of the short blocks
 * can be dealt with.
 */
static int btrfs_spin_on_block(struct extent_buffer *eb)
{
	int i;

	for (i = 0; i < 512; i++) {
		if (!test_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags))
			return 1;
		if (need_resched())
			break;
		cpu_relax();
	}
	return 0;
}

/*
 * This is somewhat different from trylock.  It will take the
 * spinlock but if it finds the lock is set to blocking, it will
 * return without the lock held.
 *
 * returns 1 if it was able to take the lock and zero otherwise
 *
 * After this call, scheduling is not safe without first calling
 * btrfs_set_lock_blocking()
 */
int btrfs_try_spin_lock(struct extent_buffer *eb)
{
	int i;

	if (btrfs_spin_on_block(eb)) {
		spin_nested(eb);
		if (!test_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags))
			return 1;
		spin_unlock(&eb->lock);
	}
	/* spin for a bit on the BLOCKING flag */
	for (i = 0; i < 2; i++) {
		cpu_relax();
		if (!btrfs_spin_on_block(eb))
			break;

		spin_nested(eb);
		if (!test_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags))
			return 1;
		spin_unlock(&eb->lock);
	}
	return 0;
}

/*
 * the autoremove wake function will return 0 if it tried to wake up
 * a process that was already awake, which means that process won't
 * count as an exclusive wakeup.  The waitq code will continue waking
 * procs until it finds one that was actually sleeping.
 *
 * For btrfs, this isn't quite what we want.  We want a single proc
 * to be notified that the lock is ready for taking.  If that proc
 * already happen to be awake, great, it will loop around and try for
 * the lock.
 *
 * So, btrfs_wake_function always returns 1, even when the proc that we
 * tried to wake up was already awake.
 */
static int btrfs_wake_function(wait_queue_t *wait, unsigned mode,
			       int sync, void *key)
{
	autoremove_wake_function(wait, mode, sync, key);
=======
void btrfs_assert_tree_read_locked(struct extent_buffer *eb);
=======
static void btrfs_assert_tree_read_locked(struct extent_buffer *eb);
>>>>>>> refs/remotes/origin/master

/*
 * if we currently have a spinning reader or writer lock
 * (indicated by the rw flag) this will bump the count
 * of blocking holders and drop the spinlock.
 */
void btrfs_set_lock_blocking_rw(struct extent_buffer *eb, int rw)
{
	if (eb->lock_nested) {
		read_lock(&eb->lock);
		if (eb->lock_nested && current->pid == eb->lock_owner) {
			read_unlock(&eb->lock);
			return;
		}
		read_unlock(&eb->lock);
	}
	if (rw == BTRFS_WRITE_LOCK) {
		if (atomic_read(&eb->blocking_writers) == 0) {
			WARN_ON(atomic_read(&eb->spinning_writers) != 1);
			atomic_dec(&eb->spinning_writers);
			btrfs_assert_tree_locked(eb);
			atomic_inc(&eb->blocking_writers);
			write_unlock(&eb->lock);
		}
	} else if (rw == BTRFS_READ_LOCK) {
		btrfs_assert_tree_read_locked(eb);
		atomic_inc(&eb->blocking_readers);
		WARN_ON(atomic_read(&eb->spinning_readers) == 0);
		atomic_dec(&eb->spinning_readers);
		read_unlock(&eb->lock);
	}
	return;
}

/*
 * if we currently have a blocking lock, take the spinlock
 * and drop our blocking count
 */
void btrfs_clear_lock_blocking_rw(struct extent_buffer *eb, int rw)
{
	if (eb->lock_nested) {
		read_lock(&eb->lock);
<<<<<<< HEAD
		if (&eb->lock_nested && current->pid == eb->lock_owner) {
=======
		if (eb->lock_nested && current->pid == eb->lock_owner) {
>>>>>>> refs/remotes/origin/master
			read_unlock(&eb->lock);
			return;
		}
		read_unlock(&eb->lock);
	}
	if (rw == BTRFS_WRITE_LOCK_BLOCKING) {
		BUG_ON(atomic_read(&eb->blocking_writers) != 1);
		write_lock(&eb->lock);
		WARN_ON(atomic_read(&eb->spinning_writers));
		atomic_inc(&eb->spinning_writers);
<<<<<<< HEAD
		if (atomic_dec_and_test(&eb->blocking_writers))
=======
		if (atomic_dec_and_test(&eb->blocking_writers) &&
		    waitqueue_active(&eb->write_lock_wq))
>>>>>>> refs/remotes/origin/master
			wake_up(&eb->write_lock_wq);
	} else if (rw == BTRFS_READ_LOCK_BLOCKING) {
		BUG_ON(atomic_read(&eb->blocking_readers) == 0);
		read_lock(&eb->lock);
		atomic_inc(&eb->spinning_readers);
<<<<<<< HEAD
		if (atomic_dec_and_test(&eb->blocking_readers))
=======
		if (atomic_dec_and_test(&eb->blocking_readers) &&
		    waitqueue_active(&eb->read_lock_wq))
>>>>>>> refs/remotes/origin/master
			wake_up(&eb->read_lock_wq);
	}
	return;
}

/*
 * take a spinning read lock.  This will wait for any blocking
 * writers
 */
void btrfs_tree_read_lock(struct extent_buffer *eb)
{
again:
	read_lock(&eb->lock);
	if (atomic_read(&eb->blocking_writers) &&
	    current->pid == eb->lock_owner) {
		/*
		 * This extent is already write-locked by our thread. We allow
		 * an additional read lock to be added because it's for the same
		 * thread. btrfs_find_all_roots() depends on this as it may be
		 * called on a partly (write-)locked tree.
		 */
		BUG_ON(eb->lock_nested);
		eb->lock_nested = 1;
		read_unlock(&eb->lock);
		return;
	}
<<<<<<< HEAD
	read_unlock(&eb->lock);
	wait_event(eb->write_lock_wq, atomic_read(&eb->blocking_writers) == 0);
	read_lock(&eb->lock);
	if (atomic_read(&eb->blocking_writers)) {
		read_unlock(&eb->lock);
=======
	if (atomic_read(&eb->blocking_writers)) {
		read_unlock(&eb->lock);
		wait_event(eb->write_lock_wq,
			   atomic_read(&eb->blocking_writers) == 0);
>>>>>>> refs/remotes/origin/master
		goto again;
	}
	atomic_inc(&eb->read_locks);
	atomic_inc(&eb->spinning_readers);
}

/*
 * returns 1 if we get the read lock and 0 if we don't
 * this won't wait for blocking writers
 */
int btrfs_try_tree_read_lock(struct extent_buffer *eb)
{
	if (atomic_read(&eb->blocking_writers))
		return 0;

	read_lock(&eb->lock);
	if (atomic_read(&eb->blocking_writers)) {
		read_unlock(&eb->lock);
		return 0;
	}
	atomic_inc(&eb->read_locks);
	atomic_inc(&eb->spinning_readers);
	return 1;
}

/*
 * returns 1 if we get the read lock and 0 if we don't
 * this won't wait for blocking writers or readers
 */
int btrfs_try_tree_write_lock(struct extent_buffer *eb)
{
	if (atomic_read(&eb->blocking_writers) ||
	    atomic_read(&eb->blocking_readers))
		return 0;
	write_lock(&eb->lock);
	if (atomic_read(&eb->blocking_writers) ||
	    atomic_read(&eb->blocking_readers)) {
		write_unlock(&eb->lock);
		return 0;
	}
	atomic_inc(&eb->write_locks);
	atomic_inc(&eb->spinning_writers);
	eb->lock_owner = current->pid;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 1;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * returns with the extent buffer spinlocked.
 *
 * This will spin and/or wait as required to take the lock, and then
 * return with the spinlock held.
 *
 * After this call, scheduling is not safe without first calling
 * btrfs_set_lock_blocking()
 */
int btrfs_tree_lock(struct extent_buffer *eb)
{
	DEFINE_WAIT(wait);
	wait.func = btrfs_wake_function;

	if (!btrfs_spin_on_block(eb))
		goto sleep;

	while(1) {
		spin_nested(eb);

		/* nobody is blocking, exit with the spinlock held */
		if (!test_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags))
			return 0;

		/*
		 * we have the spinlock, but the real owner is blocking.
		 * wait for them
		 */
		spin_unlock(&eb->lock);

		/*
		 * spin for a bit, and if the blocking flag goes away,
		 * loop around
		 */
		cpu_relax();
		if (btrfs_spin_on_block(eb))
			continue;
sleep:
		prepare_to_wait_exclusive(&eb->lock_wq, &wait,
					  TASK_UNINTERRUPTIBLE);

		if (test_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags))
			schedule();

		finish_wait(&eb->lock_wq, &wait);
	}
	return 0;
}

int btrfs_tree_unlock(struct extent_buffer *eb)
{
	/*
	 * if we were a blocking owner, we don't have the spinlock held
	 * just clear the bit and look for waiters
	 */
	if (test_and_clear_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags))
		smp_mb__after_clear_bit();
	else
		spin_unlock(&eb->lock);

	if (waitqueue_active(&eb->lock_wq))
		wake_up(&eb->lock_wq);
	return 0;
}

void btrfs_assert_tree_locked(struct extent_buffer *eb)
{
	if (!test_bit(EXTENT_BUFFER_BLOCKING, &eb->bflags))
		assert_spin_locked(&eb->lock);
=======
=======
>>>>>>> refs/remotes/origin/master
 * drop a spinning read lock
 */
void btrfs_tree_read_unlock(struct extent_buffer *eb)
{
	if (eb->lock_nested) {
		read_lock(&eb->lock);
		if (eb->lock_nested && current->pid == eb->lock_owner) {
			eb->lock_nested = 0;
			read_unlock(&eb->lock);
			return;
		}
		read_unlock(&eb->lock);
	}
	btrfs_assert_tree_read_locked(eb);
	WARN_ON(atomic_read(&eb->spinning_readers) == 0);
	atomic_dec(&eb->spinning_readers);
	atomic_dec(&eb->read_locks);
	read_unlock(&eb->lock);
}

/*
 * drop a blocking read lock
 */
void btrfs_tree_read_unlock_blocking(struct extent_buffer *eb)
{
	if (eb->lock_nested) {
		read_lock(&eb->lock);
		if (eb->lock_nested && current->pid == eb->lock_owner) {
			eb->lock_nested = 0;
			read_unlock(&eb->lock);
			return;
		}
		read_unlock(&eb->lock);
	}
	btrfs_assert_tree_read_locked(eb);
	WARN_ON(atomic_read(&eb->blocking_readers) == 0);
<<<<<<< HEAD
	if (atomic_dec_and_test(&eb->blocking_readers))
=======
	if (atomic_dec_and_test(&eb->blocking_readers) &&
	    waitqueue_active(&eb->read_lock_wq))
>>>>>>> refs/remotes/origin/master
		wake_up(&eb->read_lock_wq);
	atomic_dec(&eb->read_locks);
}

/*
 * take a spinning write lock.  This will wait for both
 * blocking readers or writers
 */
void btrfs_tree_lock(struct extent_buffer *eb)
{
again:
	wait_event(eb->read_lock_wq, atomic_read(&eb->blocking_readers) == 0);
	wait_event(eb->write_lock_wq, atomic_read(&eb->blocking_writers) == 0);
	write_lock(&eb->lock);
	if (atomic_read(&eb->blocking_readers)) {
		write_unlock(&eb->lock);
		wait_event(eb->read_lock_wq,
			   atomic_read(&eb->blocking_readers) == 0);
		goto again;
	}
	if (atomic_read(&eb->blocking_writers)) {
		write_unlock(&eb->lock);
		wait_event(eb->write_lock_wq,
			   atomic_read(&eb->blocking_writers) == 0);
		goto again;
	}
	WARN_ON(atomic_read(&eb->spinning_writers));
	atomic_inc(&eb->spinning_writers);
	atomic_inc(&eb->write_locks);
	eb->lock_owner = current->pid;
}

/*
 * drop a spinning or a blocking write lock.
 */
void btrfs_tree_unlock(struct extent_buffer *eb)
{
	int blockers = atomic_read(&eb->blocking_writers);

	BUG_ON(blockers > 1);

	btrfs_assert_tree_locked(eb);
	atomic_dec(&eb->write_locks);

	if (blockers) {
		WARN_ON(atomic_read(&eb->spinning_writers));
		atomic_dec(&eb->blocking_writers);
<<<<<<< HEAD
		smp_wmb();
		wake_up(&eb->write_lock_wq);
=======
		smp_mb();
		if (waitqueue_active(&eb->write_lock_wq))
			wake_up(&eb->write_lock_wq);
>>>>>>> refs/remotes/origin/master
	} else {
		WARN_ON(atomic_read(&eb->spinning_writers) != 1);
		atomic_dec(&eb->spinning_writers);
		write_unlock(&eb->lock);
	}
}

void btrfs_assert_tree_locked(struct extent_buffer *eb)
{
	BUG_ON(!atomic_read(&eb->write_locks));
}

<<<<<<< HEAD
void btrfs_assert_tree_read_locked(struct extent_buffer *eb)
{
	BUG_ON(!atomic_read(&eb->read_locks));
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void btrfs_assert_tree_read_locked(struct extent_buffer *eb)
{
	BUG_ON(!atomic_read(&eb->read_locks));
>>>>>>> refs/remotes/origin/master
}
