/**
 * @file oprofile_stats.h
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#ifndef OPROFILE_STATS_H
#define OPROFILE_STATS_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master

struct oprofile_stat_struct {
	atomic_t sample_lost_no_mm;
	atomic_t sample_lost_no_mapping;
	atomic_t bt_lost_no_mapping;
	atomic_t event_lost_overflow;
	atomic_t multiplex_counter;
};

extern struct oprofile_stat_struct oprofile_stats;

/* reset all stats to zero */
void oprofile_reset_stats(void);

<<<<<<< HEAD
struct super_block;
struct dentry;

/* create the stats/ dir */
void oprofile_create_stats_files(struct super_block *sb, struct dentry *root);
=======
struct dentry;

/* create the stats/ dir */
void oprofile_create_stats_files(struct dentry *root);
>>>>>>> refs/remotes/origin/master

#endif /* OPROFILE_STATS_H */
