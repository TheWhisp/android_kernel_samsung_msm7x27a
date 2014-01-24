/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
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

#include <linux/blkdev.h>
#include <linux/module.h>
#include <linux/buffer_head.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/highmem.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/backing-dev.h>
#include <linux/mount.h>
#include <linux/mpage.h>
#include <linux/swap.h>
#include <linux/writeback.h>
#include <linux/statfs.h>
#include <linux/compat.h>
#include <linux/parser.h>
#include <linux/ctype.h>
#include <linux/namei.h>
#include <linux/miscdevice.h>
#include <linux/magic.h>
#include <linux/slab.h>
#include <linux/cleancache.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/ratelimit.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "compat.h"
=======
#include <linux/ratelimit.h>
#include <linux/btrfs.h>
>>>>>>> refs/remotes/origin/master
#include "delayed-inode.h"
#include "ctree.h"
#include "disk-io.h"
#include "transaction.h"
#include "btrfs_inode.h"
<<<<<<< HEAD
#include "ioctl.h"
#include "print-tree.h"
#include "xattr.h"
#include "volumes.h"
#include "version.h"
#include "export.h"
#include "compression.h"
=======
#include "print-tree.h"
#include "xattr.h"
#include "volumes.h"
#include "export.h"
#include "compression.h"
#include "rcu-string.h"
#include "dev-replace.h"
#include "free-space-cache.h"
#include "backref.h"
#include "tests/btrfs-tests.h"
>>>>>>> refs/remotes/origin/master

#define CREATE_TRACE_POINTS
#include <trace/events/btrfs.h>

static const struct super_operations btrfs_super_ops;
<<<<<<< HEAD
<<<<<<< HEAD
=======
static struct file_system_type btrfs_fs_type;
>>>>>>> refs/remotes/origin/cm-10.0

static const char *btrfs_decode_error(struct btrfs_fs_info *fs_info, int errno,
				      char nbuf[16])
{
	char *errstr = NULL;
=======
static struct file_system_type btrfs_fs_type;

static const char *btrfs_decode_error(int errno)
{
	char *errstr = "unknown";
>>>>>>> refs/remotes/origin/master

	switch (errno) {
	case -EIO:
		errstr = "IO failure";
		break;
	case -ENOMEM:
		errstr = "Out of memory";
		break;
	case -EROFS:
		errstr = "Readonly filesystem";
		break;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case -EEXIST:
		errstr = "Object already exists";
		break;
>>>>>>> refs/remotes/origin/cm-10.0
	default:
		if (nbuf) {
			if (snprintf(nbuf, 16, "error %d", -errno) >= 0)
				errstr = nbuf;
		}
=======
	case -EEXIST:
		errstr = "Object already exists";
		break;
	case -ENOSPC:
		errstr = "No space left";
		break;
	case -ENOENT:
		errstr = "No such entry";
>>>>>>> refs/remotes/origin/master
		break;
	}

	return errstr;
}

<<<<<<< HEAD
static void __save_error_info(struct btrfs_fs_info *fs_info)
=======
static void save_error_info(struct btrfs_fs_info *fs_info)
>>>>>>> refs/remotes/origin/master
{
	/*
	 * today we only save the error info into ram.  Long term we'll
	 * also send it down to the disk
	 */
<<<<<<< HEAD
	fs_info->fs_state = BTRFS_SUPER_FLAG_ERROR;
}

/* NOTE:
 *	We move write_super stuff at umount in order to avoid deadlock
 *	for umount hold all lock.
 */
static void save_error_info(struct btrfs_fs_info *fs_info)
{
	__save_error_info(fs_info);
=======
	set_bit(BTRFS_FS_STATE_ERROR, &fs_info->fs_state);
>>>>>>> refs/remotes/origin/master
}

/* btrfs handle error by forcing the filesystem readonly */
static void btrfs_handle_error(struct btrfs_fs_info *fs_info)
{
	struct super_block *sb = fs_info->sb;

	if (sb->s_flags & MS_RDONLY)
		return;

<<<<<<< HEAD
	if (fs_info->fs_state & BTRFS_SUPER_FLAG_ERROR) {
		sb->s_flags |= MS_RDONLY;
		printk(KERN_INFO "btrfs is forced readonly\n");
<<<<<<< HEAD
=======
		__btrfs_scrub_cancel(fs_info);
//		WARN_ON(1);
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

=======
	if (test_bit(BTRFS_FS_STATE_ERROR, &fs_info->fs_state)) {
		sb->s_flags |= MS_RDONLY;
		btrfs_info(fs_info, "forced readonly");
		/*
		 * Note that a running device replace operation is not
		 * canceled here although there is no way to update
		 * the progress. It would add the risk of a deadlock,
		 * therefore the canceling is ommited. The only penalty
		 * is that some I/O remains active until the procedure
		 * completes. The next time when the filesystem is
		 * mounted writeable again, the device replace
		 * operation continues.
		 */
	}
}

#ifdef CONFIG_PRINTK
>>>>>>> refs/remotes/origin/master
/*
 * __btrfs_std_error decodes expected errors from the caller and
 * invokes the approciate error response.
 */
void __btrfs_std_error(struct btrfs_fs_info *fs_info, const char *function,
<<<<<<< HEAD
<<<<<<< HEAD
		     unsigned int line, int errno)
=======
		       unsigned int line, int errno, const char *fmt, ...)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct super_block *sb = fs_info->sb;
	char nbuf[16];
	const char *errstr;
<<<<<<< HEAD
=======
	va_list args;
	va_start(args, fmt);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       unsigned int line, int errno, const char *fmt, ...)
{
	struct super_block *sb = fs_info->sb;
	const char *errstr;
>>>>>>> refs/remotes/origin/master

	/*
	 * Special case: if the error is EROFS, and we're already
	 * under MS_RDONLY, then it is safe here.
	 */
	if (errno == -EROFS && (sb->s_flags & MS_RDONLY))
<<<<<<< HEAD
<<<<<<< HEAD
		return;

	errstr = btrfs_decode_error(fs_info, errno, nbuf);
	printk(KERN_CRIT "BTRFS error (device %s) in %s:%d: %s\n",
		sb->s_id, function, line, errstr);
	save_error_info(fs_info);

	btrfs_handle_error(fs_info);
}

static void btrfs_put_super(struct super_block *sb)
{
	struct btrfs_root *root = btrfs_sb(sb);
	int ret;

	ret = close_ctree(root);
	sb->s_fs_info = NULL;

	(void)ret; /* FIXME: need to fix VFS to return error? */
=======
  		return;

  	errstr = btrfs_decode_error(fs_info, errno, nbuf);
	if (fmt) {
		struct va_format vaf = {
			.fmt = fmt,
			.va = &args,
		};

		printk(KERN_CRIT "BTRFS error (device %s) in %s:%d: %s (%pV)\n",
			sb->s_id, function, line, errstr, &vaf);
	} else {
		printk(KERN_CRIT "BTRFS error (device %s) in %s:%d: %s\n",
			sb->s_id, function, line, errstr);
	}

	/* Don't go through full error handling during mount */
	if (sb->s_flags & MS_BORN) {
		save_error_info(fs_info);
		btrfs_handle_error(fs_info);
	}
	va_end(args);
}

const char *logtypes[] = {
=======
  		return;

	errstr = btrfs_decode_error(errno);
	if (fmt) {
		struct va_format vaf;
		va_list args;

		va_start(args, fmt);
		vaf.fmt = fmt;
		vaf.va = &args;

		printk(KERN_CRIT "BTRFS error (device %s) in %s:%d: errno=%d %s (%pV)\n",
			sb->s_id, function, line, errno, errstr, &vaf);
		va_end(args);
	} else {
		printk(KERN_CRIT "BTRFS error (device %s) in %s:%d: errno=%d %s\n",
			sb->s_id, function, line, errno, errstr);
	}

	/* Don't go through full error handling during mount */
	save_error_info(fs_info);
	if (sb->s_flags & MS_BORN)
		btrfs_handle_error(fs_info);
}

static const char * const logtypes[] = {
>>>>>>> refs/remotes/origin/master
	"emergency",
	"alert",
	"critical",
	"error",
	"warning",
	"notice",
	"info",
	"debug",
};

<<<<<<< HEAD
void btrfs_printk(struct btrfs_fs_info *fs_info, const char *fmt, ...)
=======
void btrfs_printk(const struct btrfs_fs_info *fs_info, const char *fmt, ...)
>>>>>>> refs/remotes/origin/master
{
	struct super_block *sb = fs_info->sb;
	char lvl[4];
	struct va_format vaf;
	va_list args;
	const char *type = logtypes[4];
<<<<<<< HEAD

	va_start(args, fmt);

	if (fmt[0] == '<' && isdigit(fmt[1]) && fmt[2] == '>') {
		strncpy(lvl, fmt, 3);
		fmt += 3;
		type = logtypes[fmt[1] - '0'];
=======
	int kern_level;

	va_start(args, fmt);

	kern_level = printk_get_level(fmt);
	if (kern_level) {
		size_t size = printk_skip_level(fmt) - fmt;
		memcpy(lvl, fmt,  size);
		lvl[size] = '\0';
		fmt += size;
		type = logtypes[kern_level - '0'];
>>>>>>> refs/remotes/origin/master
	} else
		*lvl = '\0';

	vaf.fmt = fmt;
	vaf.va = &args;
<<<<<<< HEAD
	printk("%sBTRFS %s (device %s): %pV", lvl, type, sb->s_id, &vaf);
}
=======

	printk("%sBTRFS %s (device %s): %pV\n", lvl, type, sb->s_id, &vaf);

	va_end(args);
}

#else

void __btrfs_std_error(struct btrfs_fs_info *fs_info, const char *function,
		       unsigned int line, int errno, const char *fmt, ...)
{
	struct super_block *sb = fs_info->sb;

	/*
	 * Special case: if the error is EROFS, and we're already
	 * under MS_RDONLY, then it is safe here.
	 */
	if (errno == -EROFS && (sb->s_flags & MS_RDONLY))
		return;

	/* Don't go through full error handling during mount */
	if (sb->s_flags & MS_BORN) {
		save_error_info(fs_info);
		btrfs_handle_error(fs_info);
	}
}
#endif
>>>>>>> refs/remotes/origin/master

/*
 * We only mark the transaction aborted and then set the file system read-only.
 * This will prevent new transactions from starting or trying to join this
 * one.
 *
 * This means that error recovery at the call site is limited to freeing
 * any local memory allocations and passing the error code up without
 * further cleanup. The transaction should complete as it normally would
 * in the call path but will return -EIO.
 *
 * We'll complete the cleanup in btrfs_end_transaction and
 * btrfs_commit_transaction.
 */
void __btrfs_abort_transaction(struct btrfs_trans_handle *trans,
			       struct btrfs_root *root, const char *function,
			       unsigned int line, int errno)
{
<<<<<<< HEAD
	WARN_ONCE(1, KERN_DEBUG "btrfs: Transaction aborted");
=======
	/*
	 * Report first abort since mount
	 */
	if (!test_and_set_bit(BTRFS_FS_STATE_TRANS_ABORTED,
				&root->fs_info->fs_state)) {
		WARN(1, KERN_DEBUG "btrfs: Transaction aborted (error %d)\n",
				errno);
	}
>>>>>>> refs/remotes/origin/master
	trans->aborted = errno;
	/* Nothing used. The other threads that have joined this
	 * transaction may be able to continue. */
	if (!trans->blocks_used) {
<<<<<<< HEAD
		btrfs_printk(root->fs_info, "Aborting unused transaction.\n");
		return;
	}
	trans->transaction->aborted = errno;
=======
		const char *errstr;

		errstr = btrfs_decode_error(errno);
		btrfs_warn(root->fs_info,
		           "%s:%d: Aborting unused transaction(%s).",
		           function, line, errstr);
		return;
	}
	ACCESS_ONCE(trans->transaction->aborted) = errno;
	/* Wake up anybody who may be waiting on this transaction */
	wake_up(&root->fs_info->transaction_wait);
	wake_up(&root->fs_info->transaction_blocked_wait);
>>>>>>> refs/remotes/origin/master
	__btrfs_std_error(root->fs_info, function, line, errno, NULL);
}
/*
 * __btrfs_panic decodes unexpected, fatal errors from the caller,
 * issues an alert, and either panics or BUGs, depending on mount options.
 */
void __btrfs_panic(struct btrfs_fs_info *fs_info, const char *function,
		   unsigned int line, int errno, const char *fmt, ...)
{
<<<<<<< HEAD
	char nbuf[16];
=======
>>>>>>> refs/remotes/origin/master
	char *s_id = "<unknown>";
	const char *errstr;
	struct va_format vaf = { .fmt = fmt };
	va_list args;

	if (fs_info)
		s_id = fs_info->sb->s_id;

	va_start(args, fmt);
	vaf.va = &args;

<<<<<<< HEAD
	errstr = btrfs_decode_error(fs_info, errno, nbuf);
	if (fs_info->mount_opt & BTRFS_MOUNT_PANIC_ON_FATAL_ERROR)
		panic(KERN_CRIT "BTRFS panic (device %s) in %s:%d: %pV (%s)\n",
			s_id, function, line, &vaf, errstr);

	printk(KERN_CRIT "BTRFS panic (device %s) in %s:%d: %pV (%s)\n",
	       s_id, function, line, &vaf, errstr);
=======
	errstr = btrfs_decode_error(errno);
	if (fs_info && (fs_info->mount_opt & BTRFS_MOUNT_PANIC_ON_FATAL_ERROR))
		panic(KERN_CRIT "BTRFS panic (device %s) in %s:%d: %pV (errno=%d %s)\n",
			s_id, function, line, &vaf, errno, errstr);

	printk(KERN_CRIT "BTRFS panic (device %s) in %s:%d: %pV (errno=%d %s)\n",
	       s_id, function, line, &vaf, errno, errstr);
>>>>>>> refs/remotes/origin/master
	va_end(args);
	/* Caller calls BUG() */
}

static void btrfs_put_super(struct super_block *sb)
{
	(void)close_ctree(btrfs_sb(sb)->tree_root);
	/* FIXME: need to fix VFS to return error? */
	/* AV: return it _where_?  ->put_super() can be triggered by any number
	 * of async events, up to and including delivery of SIGKILL to the
	 * last process that kept it busy.  Or segfault in the aforementioned
	 * process...  Whom would you report that to?
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

enum {
	Opt_degraded, Opt_subvol, Opt_subvolid, Opt_device, Opt_nodatasum,
	Opt_nodatacow, Opt_max_inline, Opt_alloc_start, Opt_nobarrier, Opt_ssd,
	Opt_nossd, Opt_ssd_spread, Opt_thread_pool, Opt_noacl, Opt_compress,
	Opt_compress_type, Opt_compress_force, Opt_compress_force_type,
	Opt_notreelog, Opt_ratio, Opt_flushoncommit, Opt_discard,
	Opt_space_cache, Opt_clear_cache, Opt_user_subvol_rm_allowed,
<<<<<<< HEAD
<<<<<<< HEAD
	Opt_enospc_debug, Opt_subvolrootid, Opt_defrag,
	Opt_inode_cache, Opt_err,
=======
	Opt_enospc_debug, Opt_subvolrootid, Opt_defrag, Opt_inode_cache,
	Opt_no_space_cache, Opt_recovery, Opt_skip_balance,
	Opt_check_integrity, Opt_check_integrity_including_extent_data,
	Opt_check_integrity_print_mask, Opt_fatal_errors,
	Opt_err,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	Opt_enospc_debug, Opt_subvolrootid, Opt_defrag, Opt_inode_cache,
	Opt_no_space_cache, Opt_recovery, Opt_skip_balance,
	Opt_check_integrity, Opt_check_integrity_including_extent_data,
	Opt_check_integrity_print_mask, Opt_fatal_errors, Opt_rescan_uuid_tree,
	Opt_commit_interval,
	Opt_err,
>>>>>>> refs/remotes/origin/master
};

static match_table_t tokens = {
	{Opt_degraded, "degraded"},
	{Opt_subvol, "subvol=%s"},
<<<<<<< HEAD
	{Opt_subvolid, "subvolid=%d"},
=======
	{Opt_subvolid, "subvolid=%s"},
>>>>>>> refs/remotes/origin/master
	{Opt_device, "device=%s"},
	{Opt_nodatasum, "nodatasum"},
	{Opt_nodatacow, "nodatacow"},
	{Opt_nobarrier, "nobarrier"},
	{Opt_max_inline, "max_inline=%s"},
	{Opt_alloc_start, "alloc_start=%s"},
	{Opt_thread_pool, "thread_pool=%d"},
	{Opt_compress, "compress"},
	{Opt_compress_type, "compress=%s"},
	{Opt_compress_force, "compress-force"},
	{Opt_compress_force_type, "compress-force=%s"},
	{Opt_ssd, "ssd"},
	{Opt_ssd_spread, "ssd_spread"},
	{Opt_nossd, "nossd"},
	{Opt_noacl, "noacl"},
	{Opt_notreelog, "notreelog"},
	{Opt_flushoncommit, "flushoncommit"},
	{Opt_ratio, "metadata_ratio=%d"},
	{Opt_discard, "discard"},
	{Opt_space_cache, "space_cache"},
	{Opt_clear_cache, "clear_cache"},
	{Opt_user_subvol_rm_allowed, "user_subvol_rm_allowed"},
	{Opt_enospc_debug, "enospc_debug"},
	{Opt_subvolrootid, "subvolrootid=%d"},
	{Opt_defrag, "autodefrag"},
	{Opt_inode_cache, "inode_cache"},
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	{Opt_no_space_cache, "nospace_cache"},
	{Opt_recovery, "recovery"},
	{Opt_skip_balance, "skip_balance"},
	{Opt_check_integrity, "check_int"},
	{Opt_check_integrity_including_extent_data, "check_int_data"},
	{Opt_check_integrity_print_mask, "check_int_print_mask=%d"},
<<<<<<< HEAD
	{Opt_fatal_errors, "fatal_errors=%s"},
>>>>>>> refs/remotes/origin/cm-10.0
=======
	{Opt_rescan_uuid_tree, "rescan_uuid_tree"},
	{Opt_fatal_errors, "fatal_errors=%s"},
	{Opt_commit_interval, "commit=%d"},
>>>>>>> refs/remotes/origin/master
	{Opt_err, NULL},
};

/*
 * Regular mount options parser.  Everything that is needed only when
 * reading in a new superblock is parsed here.
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * XXX JDM: This needs to be cleaned up for remount.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * XXX JDM: This needs to be cleaned up for remount.
>>>>>>> refs/remotes/origin/master
 */
int btrfs_parse_options(struct btrfs_root *root, char *options)
{
	struct btrfs_fs_info *info = root->fs_info;
	substring_t args[MAX_OPT_ARGS];
<<<<<<< HEAD
<<<<<<< HEAD
	char *p, *num, *orig;
=======
	char *p, *num, *orig = NULL;
	u64 cache_gen;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	char *p, *num, *orig = NULL;
	u64 cache_gen;
>>>>>>> refs/remotes/origin/master
	int intarg;
	int ret = 0;
	char *compress_type;
	bool compress_force = false;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!options)
		return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	cache_gen = btrfs_super_cache_generation(root->fs_info->super_copy);
	if (cache_gen)
		btrfs_set_opt(info->mount_opt, SPACE_CACHE);

	if (!options)
		goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * strsep changes the string, duplicate it because parse_options
	 * gets called twice
	 */
	options = kstrdup(options, GFP_NOFS);
	if (!options)
		return -ENOMEM;

	orig = options;

	while ((p = strsep(&options, ",")) != NULL) {
		int token;
		if (!*p)
			continue;

		token = match_token(p, tokens, args);
		switch (token) {
		case Opt_degraded:
			printk(KERN_INFO "btrfs: allowing degraded mounts\n");
			btrfs_set_opt(info->mount_opt, DEGRADED);
			break;
		case Opt_subvol:
		case Opt_subvolid:
		case Opt_subvolrootid:
		case Opt_device:
			/*
			 * These are parsed by btrfs_parse_early_options
			 * and can be happily ignored here.
			 */
			break;
		case Opt_nodatasum:
			printk(KERN_INFO "btrfs: setting nodatasum\n");
			btrfs_set_opt(info->mount_opt, NODATASUM);
			break;
		case Opt_nodatacow:
<<<<<<< HEAD
			printk(KERN_INFO "btrfs: setting nodatacow\n");
=======
			if (!btrfs_test_opt(root, COMPRESS) ||
				!btrfs_test_opt(root, FORCE_COMPRESS)) {
					printk(KERN_INFO "btrfs: setting nodatacow, compression disabled\n");
			} else {
				printk(KERN_INFO "btrfs: setting nodatacow\n");
			}
			btrfs_clear_opt(info->mount_opt, COMPRESS);
			btrfs_clear_opt(info->mount_opt, FORCE_COMPRESS);
>>>>>>> refs/remotes/origin/master
			btrfs_set_opt(info->mount_opt, NODATACOW);
			btrfs_set_opt(info->mount_opt, NODATASUM);
			break;
		case Opt_compress_force:
		case Opt_compress_force_type:
			compress_force = true;
<<<<<<< HEAD
=======
			/* Fallthrough */
>>>>>>> refs/remotes/origin/master
		case Opt_compress:
		case Opt_compress_type:
			if (token == Opt_compress ||
			    token == Opt_compress_force ||
			    strcmp(args[0].from, "zlib") == 0) {
				compress_type = "zlib";
				info->compress_type = BTRFS_COMPRESS_ZLIB;
<<<<<<< HEAD
			} else if (strcmp(args[0].from, "lzo") == 0) {
				compress_type = "lzo";
				info->compress_type = BTRFS_COMPRESS_LZO;
=======
				btrfs_set_opt(info->mount_opt, COMPRESS);
				btrfs_clear_opt(info->mount_opt, NODATACOW);
				btrfs_clear_opt(info->mount_opt, NODATASUM);
			} else if (strcmp(args[0].from, "lzo") == 0) {
				compress_type = "lzo";
				info->compress_type = BTRFS_COMPRESS_LZO;
				btrfs_set_opt(info->mount_opt, COMPRESS);
				btrfs_clear_opt(info->mount_opt, NODATACOW);
				btrfs_clear_opt(info->mount_opt, NODATASUM);
				btrfs_set_fs_incompat(info, COMPRESS_LZO);
			} else if (strncmp(args[0].from, "no", 2) == 0) {
				compress_type = "no";
				btrfs_clear_opt(info->mount_opt, COMPRESS);
				btrfs_clear_opt(info->mount_opt, FORCE_COMPRESS);
				compress_force = false;
>>>>>>> refs/remotes/origin/master
			} else {
				ret = -EINVAL;
				goto out;
			}

<<<<<<< HEAD
			btrfs_set_opt(info->mount_opt, COMPRESS);
=======
>>>>>>> refs/remotes/origin/master
			if (compress_force) {
				btrfs_set_opt(info->mount_opt, FORCE_COMPRESS);
				pr_info("btrfs: force %s compression\n",
					compress_type);
<<<<<<< HEAD
			} else
				pr_info("btrfs: use %s compression\n",
					compress_type);
=======
			} else if (btrfs_test_opt(root, COMPRESS)) {
				pr_info("btrfs: use %s compression\n",
					compress_type);
			}
>>>>>>> refs/remotes/origin/master
			break;
		case Opt_ssd:
			printk(KERN_INFO "btrfs: use ssd allocation scheme\n");
			btrfs_set_opt(info->mount_opt, SSD);
			break;
		case Opt_ssd_spread:
			printk(KERN_INFO "btrfs: use spread ssd "
			       "allocation scheme\n");
			btrfs_set_opt(info->mount_opt, SSD);
			btrfs_set_opt(info->mount_opt, SSD_SPREAD);
			break;
		case Opt_nossd:
			printk(KERN_INFO "btrfs: not using ssd allocation "
			       "scheme\n");
			btrfs_set_opt(info->mount_opt, NOSSD);
			btrfs_clear_opt(info->mount_opt, SSD);
			btrfs_clear_opt(info->mount_opt, SSD_SPREAD);
			break;
		case Opt_nobarrier:
			printk(KERN_INFO "btrfs: turning off barriers\n");
			btrfs_set_opt(info->mount_opt, NOBARRIER);
			break;
		case Opt_thread_pool:
<<<<<<< HEAD
			intarg = 0;
			match_int(&args[0], &intarg);
			if (intarg) {
				info->thread_pool_size = intarg;
				printk(KERN_INFO "btrfs: thread pool %d\n",
				       info->thread_pool_size);
=======
			ret = match_int(&args[0], &intarg);
			if (ret) {
				goto out;
			} else if (intarg > 0) {
				info->thread_pool_size = intarg;
			} else {
				ret = -EINVAL;
				goto out;
>>>>>>> refs/remotes/origin/master
			}
			break;
		case Opt_max_inline:
			num = match_strdup(&args[0]);
			if (num) {
				info->max_inline = memparse(num, NULL);
				kfree(num);

				if (info->max_inline) {
					info->max_inline = max_t(u64,
						info->max_inline,
						root->sectorsize);
				}
				printk(KERN_INFO "btrfs: max_inline at %llu\n",
<<<<<<< HEAD
					(unsigned long long)info->max_inline);
=======
					info->max_inline);
			} else {
				ret = -ENOMEM;
				goto out;
>>>>>>> refs/remotes/origin/master
			}
			break;
		case Opt_alloc_start:
			num = match_strdup(&args[0]);
			if (num) {
<<<<<<< HEAD
				info->alloc_start = memparse(num, NULL);
				kfree(num);
				printk(KERN_INFO
					"btrfs: allocations start at %llu\n",
					(unsigned long long)info->alloc_start);
=======
				mutex_lock(&info->chunk_mutex);
				info->alloc_start = memparse(num, NULL);
				mutex_unlock(&info->chunk_mutex);
				kfree(num);
				printk(KERN_INFO
					"btrfs: allocations start at %llu\n",
					info->alloc_start);
			} else {
				ret = -ENOMEM;
				goto out;
>>>>>>> refs/remotes/origin/master
			}
			break;
		case Opt_noacl:
			root->fs_info->sb->s_flags &= ~MS_POSIXACL;
			break;
		case Opt_notreelog:
			printk(KERN_INFO "btrfs: disabling tree log\n");
			btrfs_set_opt(info->mount_opt, NOTREELOG);
			break;
		case Opt_flushoncommit:
			printk(KERN_INFO "btrfs: turning on flush-on-commit\n");
			btrfs_set_opt(info->mount_opt, FLUSHONCOMMIT);
			break;
		case Opt_ratio:
<<<<<<< HEAD
			intarg = 0;
			match_int(&args[0], &intarg);
			if (intarg) {
				info->metadata_ratio = intarg;
				printk(KERN_INFO "btrfs: metadata ratio %d\n",
				       info->metadata_ratio);
=======
			ret = match_int(&args[0], &intarg);
			if (ret) {
				goto out;
			} else if (intarg >= 0) {
				info->metadata_ratio = intarg;
				printk(KERN_INFO "btrfs: metadata ratio %d\n",
				       info->metadata_ratio);
			} else {
				ret = -EINVAL;
				goto out;
>>>>>>> refs/remotes/origin/master
			}
			break;
		case Opt_discard:
			btrfs_set_opt(info->mount_opt, DISCARD);
			break;
		case Opt_space_cache:
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_INFO "btrfs: enabling disk space caching\n");
			btrfs_set_opt(info->mount_opt, SPACE_CACHE);
			break;
=======
			btrfs_set_opt(info->mount_opt, SPACE_CACHE);
=======
			btrfs_set_opt(info->mount_opt, SPACE_CACHE);
			break;
		case Opt_rescan_uuid_tree:
			btrfs_set_opt(info->mount_opt, RESCAN_UUID_TREE);
>>>>>>> refs/remotes/origin/master
			break;
		case Opt_no_space_cache:
			printk(KERN_INFO "btrfs: disabling disk space caching\n");
			btrfs_clear_opt(info->mount_opt, SPACE_CACHE);
			break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case Opt_inode_cache:
			printk(KERN_INFO "btrfs: enabling inode map caching\n");
			btrfs_set_opt(info->mount_opt, INODE_MAP_CACHE);
			break;
		case Opt_clear_cache:
			printk(KERN_INFO "btrfs: force clearing of disk cache\n");
			btrfs_set_opt(info->mount_opt, CLEAR_CACHE);
			break;
		case Opt_user_subvol_rm_allowed:
			btrfs_set_opt(info->mount_opt, USER_SUBVOL_RM_ALLOWED);
			break;
		case Opt_enospc_debug:
			btrfs_set_opt(info->mount_opt, ENOSPC_DEBUG);
			break;
		case Opt_defrag:
<<<<<<< HEAD
			printk(KERN_INFO "btrfs: enabling auto defrag");
			btrfs_set_opt(info->mount_opt, AUTO_DEFRAG);
			break;
<<<<<<< HEAD
=======
		case Opt_recovery:
			printk(KERN_INFO "btrfs: enabling auto recovery");
=======
			printk(KERN_INFO "btrfs: enabling auto defrag\n");
			btrfs_set_opt(info->mount_opt, AUTO_DEFRAG);
			break;
		case Opt_recovery:
			printk(KERN_INFO "btrfs: enabling auto recovery\n");
>>>>>>> refs/remotes/origin/master
			btrfs_set_opt(info->mount_opt, RECOVERY);
			break;
		case Opt_skip_balance:
			btrfs_set_opt(info->mount_opt, SKIP_BALANCE);
			break;
#ifdef CONFIG_BTRFS_FS_CHECK_INTEGRITY
		case Opt_check_integrity_including_extent_data:
			printk(KERN_INFO "btrfs: enabling check integrity"
			       " including extent data\n");
			btrfs_set_opt(info->mount_opt,
				      CHECK_INTEGRITY_INCLUDING_EXTENT_DATA);
			btrfs_set_opt(info->mount_opt, CHECK_INTEGRITY);
			break;
		case Opt_check_integrity:
			printk(KERN_INFO "btrfs: enabling check integrity\n");
			btrfs_set_opt(info->mount_opt, CHECK_INTEGRITY);
			break;
		case Opt_check_integrity_print_mask:
<<<<<<< HEAD
			intarg = 0;
			match_int(&args[0], &intarg);
			if (intarg) {
=======
			ret = match_int(&args[0], &intarg);
			if (ret) {
				goto out;
			} else if (intarg >= 0) {
>>>>>>> refs/remotes/origin/master
				info->check_integrity_print_mask = intarg;
				printk(KERN_INFO "btrfs:"
				       " check_integrity_print_mask 0x%x\n",
				       info->check_integrity_print_mask);
<<<<<<< HEAD
=======
			} else {
				ret = -EINVAL;
				goto out;
>>>>>>> refs/remotes/origin/master
			}
			break;
#else
		case Opt_check_integrity_including_extent_data:
		case Opt_check_integrity:
		case Opt_check_integrity_print_mask:
			printk(KERN_ERR "btrfs: support for check_integrity*"
			       " not compiled in!\n");
			ret = -EINVAL;
			goto out;
#endif
		case Opt_fatal_errors:
			if (strcmp(args[0].from, "panic") == 0)
				btrfs_set_opt(info->mount_opt,
					      PANIC_ON_FATAL_ERROR);
			else if (strcmp(args[0].from, "bug") == 0)
				btrfs_clear_opt(info->mount_opt,
					      PANIC_ON_FATAL_ERROR);
			else {
				ret = -EINVAL;
				goto out;
			}
			break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
		case Opt_commit_interval:
			intarg = 0;
			ret = match_int(&args[0], &intarg);
			if (ret < 0) {
				printk(KERN_ERR
					"btrfs: invalid commit interval\n");
				ret = -EINVAL;
				goto out;
			}
			if (intarg > 0) {
				if (intarg > 300) {
					printk(KERN_WARNING
					    "btrfs: excessive commit interval %d\n",
							intarg);
				}
				info->commit_interval = intarg;
			} else {
				printk(KERN_INFO
				    "btrfs: using default commit interval %ds\n",
				    BTRFS_DEFAULT_COMMIT_INTERVAL);
				info->commit_interval = BTRFS_DEFAULT_COMMIT_INTERVAL;
			}
			break;
>>>>>>> refs/remotes/origin/master
		case Opt_err:
			printk(KERN_INFO "btrfs: unrecognized mount option "
			       "'%s'\n", p);
			ret = -EINVAL;
			goto out;
		default:
			break;
		}
	}
out:
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (!ret && btrfs_test_opt(root, SPACE_CACHE))
		printk(KERN_INFO "btrfs: disk space caching is enabled\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!ret && btrfs_test_opt(root, SPACE_CACHE))
		printk(KERN_INFO "btrfs: disk space caching is enabled\n");
>>>>>>> refs/remotes/origin/master
	kfree(orig);
	return ret;
}

/*
 * Parse mount options that are required early in the mount process.
 *
 * All other options will be parsed on much later in the mount process and
 * only when we need to allocate a new super block.
 */
static int btrfs_parse_early_options(const char *options, fmode_t flags,
		void *holder, char **subvol_name, u64 *subvol_objectid,
<<<<<<< HEAD
		u64 *subvol_rootid, struct btrfs_fs_devices **fs_devices)
{
	substring_t args[MAX_OPT_ARGS];
<<<<<<< HEAD
	char *opts, *orig, *p;
=======
	char *device_name, *opts, *orig, *p;
>>>>>>> refs/remotes/origin/cm-10.0
	int error = 0;
	int intarg;

	if (!options)
<<<<<<< HEAD
		goto out;
=======
		return 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		struct btrfs_fs_devices **fs_devices)
{
	substring_t args[MAX_OPT_ARGS];
	char *device_name, *opts, *orig, *p;
	char *num = NULL;
	int error = 0;

	if (!options)
		return 0;
>>>>>>> refs/remotes/origin/master

	/*
	 * strsep changes the string, duplicate it because parse_options
	 * gets called twice
	 */
	opts = kstrdup(options, GFP_KERNEL);
	if (!opts)
		return -ENOMEM;
	orig = opts;

	while ((p = strsep(&opts, ",")) != NULL) {
		int token;
		if (!*p)
			continue;

		token = match_token(p, tokens, args);
		switch (token) {
		case Opt_subvol:
<<<<<<< HEAD
<<<<<<< HEAD
=======
			kfree(*subvol_name);
>>>>>>> refs/remotes/origin/cm-10.0
			*subvol_name = match_strdup(&args[0]);
			break;
		case Opt_subvolid:
			intarg = 0;
			error = match_int(&args[0], &intarg);
			if (!error) {
				/* we want the original fs_tree */
				if (!intarg)
					*subvol_objectid =
						BTRFS_FS_TREE_OBJECTID;
				else
					*subvol_objectid = intarg;
			}
			break;
		case Opt_subvolrootid:
			intarg = 0;
			error = match_int(&args[0], &intarg);
			if (!error) {
				/* we want the original fs_tree */
				if (!intarg)
					*subvol_rootid =
						BTRFS_FS_TREE_OBJECTID;
				else
					*subvol_rootid = intarg;
			}
			break;
		case Opt_device:
<<<<<<< HEAD
			error = btrfs_scan_one_device(match_strdup(&args[0]),
					flags, holder, fs_devices);
			if (error)
				goto out_free_opts;
=======
=======
			kfree(*subvol_name);
			*subvol_name = match_strdup(&args[0]);
			if (!*subvol_name) {
				error = -ENOMEM;
				goto out;
			}
			break;
		case Opt_subvolid:
			num = match_strdup(&args[0]);
			if (num) {
				*subvol_objectid = memparse(num, NULL);
				kfree(num);
				/* we want the original fs_tree */
				if (!*subvol_objectid)
					*subvol_objectid =
						BTRFS_FS_TREE_OBJECTID;
			} else {
				error = -EINVAL;
				goto out;
			}
			break;
		case Opt_subvolrootid:
			printk(KERN_WARNING
				"btrfs: 'subvolrootid' mount option is deprecated and has no effect\n");
			break;
		case Opt_device:
>>>>>>> refs/remotes/origin/master
			device_name = match_strdup(&args[0]);
			if (!device_name) {
				error = -ENOMEM;
				goto out;
			}
			error = btrfs_scan_one_device(device_name,
					flags, holder, fs_devices);
			kfree(device_name);
			if (error)
				goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			break;
		default:
			break;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
 out_free_opts:
	kfree(orig);
 out:
	/*
	 * If no subvolume name is specified we use the default one.  Allocate
	 * a copy of the string "." here so that code later in the
	 * mount path doesn't care if it's the default volume or another one.
	 */
	if (!*subvol_name) {
		*subvol_name = kstrdup(".", GFP_KERNEL);
		if (!*subvol_name)
			return -ENOMEM;
	}
=======
out:
	kfree(orig);
>>>>>>> refs/remotes/origin/cm-10.0
=======
out:
	kfree(orig);
>>>>>>> refs/remotes/origin/master
	return error;
}

static struct dentry *get_default_root(struct super_block *sb,
				       u64 subvol_objectid)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct btrfs_root *root = sb->s_fs_info;
=======
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
	struct btrfs_root *root = fs_info->tree_root;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
	struct btrfs_root *root = fs_info->tree_root;
>>>>>>> refs/remotes/origin/master
	struct btrfs_root *new_root;
	struct btrfs_dir_item *di;
	struct btrfs_path *path;
	struct btrfs_key location;
	struct inode *inode;
<<<<<<< HEAD
<<<<<<< HEAD
	struct dentry *dentry;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u64 dir_id;
	int new = 0;

	/*
	 * We have a specific subvol we want to mount, just setup location and
	 * go look up the root.
	 */
	if (subvol_objectid) {
		location.objectid = subvol_objectid;
		location.type = BTRFS_ROOT_ITEM_KEY;
		location.offset = (u64)-1;
		goto find_root;
	}

	path = btrfs_alloc_path();
	if (!path)
		return ERR_PTR(-ENOMEM);
	path->leave_spinning = 1;

	/*
	 * Find the "default" dir item which points to the root item that we
	 * will mount by default if we haven't been given a specific subvolume
	 * to mount.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	dir_id = btrfs_super_root_dir(&root->fs_info->super_copy);
=======
	dir_id = btrfs_super_root_dir(fs_info->super_copy);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dir_id = btrfs_super_root_dir(fs_info->super_copy);
>>>>>>> refs/remotes/origin/master
	di = btrfs_lookup_dir_item(NULL, root, path, dir_id, "default", 7, 0);
	if (IS_ERR(di)) {
		btrfs_free_path(path);
		return ERR_CAST(di);
	}
	if (!di) {
		/*
		 * Ok the default dir item isn't there.  This is weird since
		 * it's always been there, but don't freak out, just try and
		 * mount to root most subvolume.
		 */
		btrfs_free_path(path);
		dir_id = BTRFS_FIRST_FREE_OBJECTID;
<<<<<<< HEAD
<<<<<<< HEAD
		new_root = root->fs_info->fs_root;
=======
		new_root = fs_info->fs_root;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		new_root = fs_info->fs_root;
>>>>>>> refs/remotes/origin/master
		goto setup_root;
	}

	btrfs_dir_item_key_to_cpu(path->nodes[0], di, &location);
	btrfs_free_path(path);

find_root:
<<<<<<< HEAD
<<<<<<< HEAD
	new_root = btrfs_read_fs_root_no_name(root->fs_info, &location);
=======
	new_root = btrfs_read_fs_root_no_name(fs_info, &location);
>>>>>>> refs/remotes/origin/cm-10.0
	if (IS_ERR(new_root))
		return ERR_CAST(new_root);

	if (btrfs_root_refs(&new_root->root_item) == 0)
		return ERR_PTR(-ENOENT);

=======
	new_root = btrfs_read_fs_root_no_name(fs_info, &location);
	if (IS_ERR(new_root))
		return ERR_CAST(new_root);

>>>>>>> refs/remotes/origin/master
	dir_id = btrfs_root_dirid(&new_root->root_item);
setup_root:
	location.objectid = dir_id;
	location.type = BTRFS_INODE_ITEM_KEY;
	location.offset = 0;

	inode = btrfs_iget(sb, &location, new_root, &new);
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	/*
	 * If we're just mounting the root most subvol put the inode and return
	 * a reference to the dentry.  We will have already gotten a reference
	 * to the inode in btrfs_fill_super so we're good to go.
	 */
	if (!new && sb->s_root->d_inode == inode) {
		iput(inode);
		return dget(sb->s_root);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (new) {
		const struct qstr name = { .name = "/", .len = 1 };

		/*
		 * New inode, we need to make the dentry a sibling of s_root so
		 * everything gets cleaned up properly on unmount.
		 */
		dentry = d_alloc(sb->s_root, &name);
		if (!dentry) {
			iput(inode);
			return ERR_PTR(-ENOMEM);
		}
		d_splice_alias(inode, dentry);
	} else {
		/*
		 * We found the inode in cache, just find a dentry for it and
		 * put the reference to the inode we just got.
		 */
		dentry = d_find_alias(inode);
		iput(inode);
	}

	return dentry;
=======
	return d_obtain_alias(inode);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return d_obtain_alias(inode);
>>>>>>> refs/remotes/origin/master
}

static int btrfs_fill_super(struct super_block *sb,
			    struct btrfs_fs_devices *fs_devices,
			    void *data, int silent)
{
	struct inode *inode;
<<<<<<< HEAD
<<<<<<< HEAD
	struct dentry *root_dentry;
	struct btrfs_root *tree_root;
=======
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
>>>>>>> refs/remotes/origin/master
	struct btrfs_key key;
	int err;

	sb->s_maxbytes = MAX_LFS_FILESIZE;
	sb->s_magic = BTRFS_SUPER_MAGIC;
	sb->s_op = &btrfs_super_ops;
	sb->s_d_op = &btrfs_dentry_operations;
	sb->s_export_op = &btrfs_export_ops;
	sb->s_xattr = btrfs_xattr_handlers;
	sb->s_time_gran = 1;
#ifdef CONFIG_BTRFS_FS_POSIX_ACL
	sb->s_flags |= MS_POSIXACL;
#endif
<<<<<<< HEAD

<<<<<<< HEAD
	tree_root = open_ctree(sb, fs_devices, (char *)data);

	if (IS_ERR(tree_root)) {
		printk("btrfs: open_ctree failed\n");
		return PTR_ERR(tree_root);
	}
	sb->s_fs_info = tree_root;
=======
=======
	sb->s_flags |= MS_I_VERSION;
>>>>>>> refs/remotes/origin/master
	err = open_ctree(sb, fs_devices, (char *)data);
	if (err) {
		printk("btrfs: open_ctree failed\n");
		return err;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	key.objectid = BTRFS_FIRST_FREE_OBJECTID;
	key.type = BTRFS_INODE_ITEM_KEY;
	key.offset = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	inode = btrfs_iget(sb, &key, tree_root->fs_info->fs_root, NULL);
=======
	inode = btrfs_iget(sb, &key, fs_info->fs_root, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	inode = btrfs_iget(sb, &key, fs_info->fs_root, NULL);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto fail_close;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	root_dentry = d_alloc_root(inode);
	if (!root_dentry) {
		iput(inode);
=======
	sb->s_root = d_make_root(inode);
	if (!sb->s_root) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sb->s_root = d_make_root(inode);
	if (!sb->s_root) {
>>>>>>> refs/remotes/origin/master
		err = -ENOMEM;
		goto fail_close;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	sb->s_root = root_dentry;

	save_mount_options(sb, data);
	cleancache_init_fs(sb);
	return 0;

fail_close:
	close_ctree(tree_root);
=======
=======
>>>>>>> refs/remotes/origin/master
	save_mount_options(sb, data);
	cleancache_init_fs(sb);
	sb->s_flags |= MS_ACTIVE;
	return 0;

fail_close:
	close_ctree(fs_info->tree_root);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return err;
}

int btrfs_sync_fs(struct super_block *sb, int wait)
{
	struct btrfs_trans_handle *trans;
<<<<<<< HEAD
<<<<<<< HEAD
	struct btrfs_root *root = btrfs_sb(sb);
=======
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
	struct btrfs_root *root = fs_info->tree_root;
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;
=======
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
	struct btrfs_root *root = fs_info->tree_root;
>>>>>>> refs/remotes/origin/master

	trace_btrfs_sync_fs(wait);

	if (!wait) {
<<<<<<< HEAD
<<<<<<< HEAD
		filemap_flush(root->fs_info->btree_inode->i_mapping);
		return 0;
	}

	btrfs_start_delalloc_inodes(root, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
		filemap_flush(fs_info->btree_inode->i_mapping);
		return 0;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	btrfs_wait_ordered_extents(root, 0, 0);

	trans = btrfs_start_transaction(root, 0);
	if (IS_ERR(trans))
		return PTR_ERR(trans);
	ret = btrfs_commit_transaction(trans, root);
	return ret;
}

<<<<<<< HEAD
static int btrfs_show_options(struct seq_file *seq, struct vfsmount *vfs)
{
	struct btrfs_root *root = btrfs_sb(vfs->mnt_sb);
	struct btrfs_fs_info *info = root->fs_info;
=======
=======
	btrfs_wait_ordered_roots(fs_info, -1);

	trans = btrfs_attach_transaction_barrier(root);
	if (IS_ERR(trans)) {
		/* no transaction, don't bother */
		if (PTR_ERR(trans) == -ENOENT)
			return 0;
		return PTR_ERR(trans);
	}
	return btrfs_commit_transaction(trans, root);
}

>>>>>>> refs/remotes/origin/master
static int btrfs_show_options(struct seq_file *seq, struct dentry *dentry)
{
	struct btrfs_fs_info *info = btrfs_sb(dentry->d_sb);
	struct btrfs_root *root = info->tree_root;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	char *compress_type;

	if (btrfs_test_opt(root, DEGRADED))
		seq_puts(seq, ",degraded");
	if (btrfs_test_opt(root, NODATASUM))
		seq_puts(seq, ",nodatasum");
	if (btrfs_test_opt(root, NODATACOW))
		seq_puts(seq, ",nodatacow");
	if (btrfs_test_opt(root, NOBARRIER))
		seq_puts(seq, ",nobarrier");
	if (info->max_inline != 8192 * 1024)
<<<<<<< HEAD
		seq_printf(seq, ",max_inline=%llu",
			   (unsigned long long)info->max_inline);
	if (info->alloc_start != 0)
		seq_printf(seq, ",alloc_start=%llu",
			   (unsigned long long)info->alloc_start);
=======
		seq_printf(seq, ",max_inline=%llu", info->max_inline);
	if (info->alloc_start != 0)
		seq_printf(seq, ",alloc_start=%llu", info->alloc_start);
>>>>>>> refs/remotes/origin/master
	if (info->thread_pool_size !=  min_t(unsigned long,
					     num_online_cpus() + 2, 8))
		seq_printf(seq, ",thread_pool=%d", info->thread_pool_size);
	if (btrfs_test_opt(root, COMPRESS)) {
		if (info->compress_type == BTRFS_COMPRESS_ZLIB)
			compress_type = "zlib";
		else
			compress_type = "lzo";
		if (btrfs_test_opt(root, FORCE_COMPRESS))
			seq_printf(seq, ",compress-force=%s", compress_type);
		else
			seq_printf(seq, ",compress=%s", compress_type);
	}
	if (btrfs_test_opt(root, NOSSD))
		seq_puts(seq, ",nossd");
	if (btrfs_test_opt(root, SSD_SPREAD))
		seq_puts(seq, ",ssd_spread");
	else if (btrfs_test_opt(root, SSD))
		seq_puts(seq, ",ssd");
	if (btrfs_test_opt(root, NOTREELOG))
		seq_puts(seq, ",notreelog");
	if (btrfs_test_opt(root, FLUSHONCOMMIT))
		seq_puts(seq, ",flushoncommit");
	if (btrfs_test_opt(root, DISCARD))
		seq_puts(seq, ",discard");
	if (!(root->fs_info->sb->s_flags & MS_POSIXACL))
		seq_puts(seq, ",noacl");
	if (btrfs_test_opt(root, SPACE_CACHE))
		seq_puts(seq, ",space_cache");
<<<<<<< HEAD
<<<<<<< HEAD
=======
	else
		seq_puts(seq, ",nospace_cache");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	else
		seq_puts(seq, ",nospace_cache");
	if (btrfs_test_opt(root, RESCAN_UUID_TREE))
		seq_puts(seq, ",rescan_uuid_tree");
>>>>>>> refs/remotes/origin/master
	if (btrfs_test_opt(root, CLEAR_CACHE))
		seq_puts(seq, ",clear_cache");
	if (btrfs_test_opt(root, USER_SUBVOL_RM_ALLOWED))
		seq_puts(seq, ",user_subvol_rm_allowed");
	if (btrfs_test_opt(root, ENOSPC_DEBUG))
		seq_puts(seq, ",enospc_debug");
	if (btrfs_test_opt(root, AUTO_DEFRAG))
		seq_puts(seq, ",autodefrag");
	if (btrfs_test_opt(root, INODE_MAP_CACHE))
		seq_puts(seq, ",inode_cache");
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (btrfs_test_opt(root, SKIP_BALANCE))
		seq_puts(seq, ",skip_balance");
	if (btrfs_test_opt(root, PANIC_ON_FATAL_ERROR))
		seq_puts(seq, ",fatal_errors=panic");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (btrfs_test_opt(root, SKIP_BALANCE))
		seq_puts(seq, ",skip_balance");
	if (btrfs_test_opt(root, RECOVERY))
		seq_puts(seq, ",recovery");
#ifdef CONFIG_BTRFS_FS_CHECK_INTEGRITY
	if (btrfs_test_opt(root, CHECK_INTEGRITY_INCLUDING_EXTENT_DATA))
		seq_puts(seq, ",check_int_data");
	else if (btrfs_test_opt(root, CHECK_INTEGRITY))
		seq_puts(seq, ",check_int");
	if (info->check_integrity_print_mask)
		seq_printf(seq, ",check_int_print_mask=%d",
				info->check_integrity_print_mask);
#endif
	if (info->metadata_ratio)
		seq_printf(seq, ",metadata_ratio=%d",
				info->metadata_ratio);
	if (btrfs_test_opt(root, PANIC_ON_FATAL_ERROR))
		seq_puts(seq, ",fatal_errors=panic");
	if (info->commit_interval != BTRFS_DEFAULT_COMMIT_INTERVAL)
		seq_printf(seq, ",commit=%d", info->commit_interval);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int btrfs_test_super(struct super_block *s, void *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct btrfs_root *test_root = data;
	struct btrfs_root *root = btrfs_sb(s);

	/*
	 * If this super block is going away, return false as it
	 * can't match as an existing super block.
	 */
	if (!atomic_read(&s->s_active))
		return 0;
	return root->fs_info->fs_devices == test_root->fs_info->fs_devices;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct btrfs_fs_info *p = data;
	struct btrfs_fs_info *fs_info = btrfs_sb(s);

	return fs_info->fs_devices == p->fs_devices;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int btrfs_set_super(struct super_block *s, void *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	s->s_fs_info = data;

	return set_anon_super(s, data);
}

=======
=======
>>>>>>> refs/remotes/origin/master
	int err = set_anon_super(s, data);
	if (!err)
		s->s_fs_info = data;
	return err;
}

/*
 * subvolumes are identified by ino 256
 */
static inline int is_subvolume_inode(struct inode *inode)
{
	if (inode && inode->i_ino == BTRFS_FIRST_FREE_OBJECTID)
		return 1;
	return 0;
}

/*
 * This will strip out the subvol=%s argument for an argument string and add
 * subvolid=0 to make sure we get the actual tree root for path walking to the
 * subvol we want.
 */
static char *setup_root_args(char *args)
{
<<<<<<< HEAD
	unsigned copied = 0;
	unsigned len = strlen(args) + 2;
	char *pos;
	char *ret;

	/*
	 * We need the same args as before, but minus
	 *
	 * subvol=a
	 *
	 * and add
	 *
	 * subvolid=0
	 *
	 * which is a difference of 2 characters, so we allocate strlen(args) +
	 * 2 characters.
	 */
	ret = kzalloc(len * sizeof(char), GFP_NOFS);
	if (!ret)
		return NULL;
	pos = strstr(args, "subvol=");

	/* This shouldn't happen, but just in case.. */
	if (!pos) {
		kfree(ret);
		return NULL;
	}

	/*
	 * The subvol=<> arg is not at the front of the string, copy everybody
	 * up to that into ret.
	 */
	if (pos != args) {
		*pos = '\0';
		strcpy(ret, args);
		copied += strlen(args);
		pos++;
	}

	strncpy(ret + copied, "subvolid=0", len - copied);

	/* Length of subvolid=0 */
	copied += 10;

	/*
	 * If there is no , after the subvol= option then we know there's no
	 * other options and we can just return.
	 */
	pos = strchr(pos, ',');
	if (!pos)
		return ret;

	/* Copy the rest of the arguments into our buffer */
	strncpy(ret + copied, pos, len - copied);
	copied += strlen(pos);

	return ret;
=======
	unsigned len = strlen(args) + 2 + 1;
	char *src, *dst, *buf;

	/*
	 * We need the same args as before, but with this substitution:
	 * s!subvol=[^,]+!subvolid=0!
	 *
	 * Since the replacement string is up to 2 bytes longer than the
	 * original, allocate strlen(args) + 2 + 1 bytes.
	 */

	src = strstr(args, "subvol=");
	/* This shouldn't happen, but just in case.. */
	if (!src)
		return NULL;

	buf = dst = kmalloc(len, GFP_NOFS);
	if (!buf)
		return NULL;

	/*
	 * If the subvol= arg is not at the start of the string,
	 * copy whatever precedes it into buf.
	 */
	if (src != args) {
		*src++ = '\0';
		strcpy(buf, args);
		dst += strlen(args);
	}

	strcpy(dst, "subvolid=0");
	dst += strlen("subvolid=0");

	/*
	 * If there is a "," after the original subvol=... string,
	 * copy that suffix into our buffer.  Otherwise, we're done.
	 */
	src = strchr(src, ',');
	if (src)
		strcpy(dst, src);

	return buf;
>>>>>>> refs/remotes/origin/master
}

static struct dentry *mount_subvol(const char *subvol_name, int flags,
				   const char *device_name, char *data)
{
	struct dentry *root;
	struct vfsmount *mnt;
	char *newargs;

	newargs = setup_root_args(data);
	if (!newargs)
		return ERR_PTR(-ENOMEM);
	mnt = vfs_kern_mount(&btrfs_fs_type, flags, device_name,
			     newargs);
	kfree(newargs);
	if (IS_ERR(mnt))
		return ERR_CAST(mnt);

	root = mount_subtree(mnt, subvol_name);

	if (!IS_ERR(root) && !is_subvolume_inode(root->d_inode)) {
		struct super_block *s = root->d_sb;
		dput(root);
		root = ERR_PTR(-EINVAL);
		deactivate_locked_super(s);
		printk(KERN_ERR "btrfs: '%s' is not a valid subvolume\n",
				subvol_name);
	}

	return root;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * Find a superblock for the given device / mount point.
 *
 * Note:  This is based on get_sb_bdev from fs/super.c with a few additions
 *	  for multiple device setup.  Make sure to keep it in sync.
 */
static struct dentry *btrfs_mount(struct file_system_type *fs_type, int flags,
		const char *device_name, void *data)
{
	struct block_device *bdev = NULL;
	struct super_block *s;
	struct dentry *root;
	struct btrfs_fs_devices *fs_devices = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	struct btrfs_root *tree_root = NULL;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct btrfs_fs_info *fs_info = NULL;
	fmode_t mode = FMODE_READ;
	char *subvol_name = NULL;
	u64 subvol_objectid = 0;
<<<<<<< HEAD
	u64 subvol_rootid = 0;
=======
>>>>>>> refs/remotes/origin/master
	int error = 0;

	if (!(flags & MS_RDONLY))
		mode |= FMODE_WRITE;

	error = btrfs_parse_early_options(data, mode, fs_type,
					  &subvol_name, &subvol_objectid,
<<<<<<< HEAD
					  &subvol_rootid, &fs_devices);
<<<<<<< HEAD
	if (error)
		return ERR_PTR(error);

	error = btrfs_scan_one_device(device_name, mode, fs_type, &fs_devices);
	if (error)
		goto error_free_subvol_name;

	error = btrfs_open_devices(fs_devices, mode, fs_type);
	if (error)
		goto error_free_subvol_name;

	if (!(flags & MS_RDONLY) && fs_devices->rw_devices == 0) {
		error = -EACCES;
		goto error_close_devices;
	}
=======
=======
					  &fs_devices);
>>>>>>> refs/remotes/origin/master
	if (error) {
		kfree(subvol_name);
		return ERR_PTR(error);
	}

	if (subvol_name) {
		root = mount_subvol(subvol_name, flags, device_name, data);
		kfree(subvol_name);
		return root;
	}

	error = btrfs_scan_one_device(device_name, mode, fs_type, &fs_devices);
	if (error)
		return ERR_PTR(error);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Setup a dummy root and fs_info for test/set super.  This is because
	 * we don't actually fill this stuff out until open_ctree, but we need
	 * it for searching for existing supers, so this lets us do that and
	 * then open_ctree will properly initialize everything later.
	 */
	fs_info = kzalloc(sizeof(struct btrfs_fs_info), GFP_NOFS);
<<<<<<< HEAD
<<<<<<< HEAD
	tree_root = kzalloc(sizeof(struct btrfs_root), GFP_NOFS);
	if (!fs_info || !tree_root) {
		error = -ENOMEM;
		goto error_close_devices;
	}
	fs_info->tree_root = tree_root;
	fs_info->fs_devices = fs_devices;
	tree_root->fs_info = fs_info;

	bdev = fs_devices->latest_bdev;
	s = sget(fs_type, btrfs_test_super, btrfs_set_super, tree_root);
	if (IS_ERR(s))
		goto error_s;

	if (s->s_root) {
		if ((flags ^ s->s_flags) & MS_RDONLY) {
			deactivate_locked_super(s);
			error = -EBUSY;
			goto error_close_devices;
		}

		btrfs_close_devices(fs_devices);
		kfree(fs_info);
		kfree(tree_root);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!fs_info)
		return ERR_PTR(-ENOMEM);

	fs_info->fs_devices = fs_devices;

	fs_info->super_copy = kzalloc(BTRFS_SUPER_INFO_SIZE, GFP_NOFS);
	fs_info->super_for_commit = kzalloc(BTRFS_SUPER_INFO_SIZE, GFP_NOFS);
	if (!fs_info->super_copy || !fs_info->super_for_commit) {
		error = -ENOMEM;
		goto error_fs_info;
	}

	error = btrfs_open_devices(fs_devices, mode, fs_type);
	if (error)
		goto error_fs_info;

	if (!(flags & MS_RDONLY) && fs_devices->rw_devices == 0) {
		error = -EACCES;
		goto error_close_devices;
	}

	bdev = fs_devices->latest_bdev;
<<<<<<< HEAD
	s = sget(fs_type, btrfs_test_super, btrfs_set_super, fs_info);
=======
	s = sget(fs_type, btrfs_test_super, btrfs_set_super, flags | MS_NOSEC,
		 fs_info);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(s)) {
		error = PTR_ERR(s);
		goto error_close_devices;
	}

	if (s->s_root) {
		btrfs_close_devices(fs_devices);
		free_fs_info(fs_info);
		if ((flags ^ s->s_flags) & MS_RDONLY)
			error = -EBUSY;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		char b[BDEVNAME_SIZE];

		s->s_flags = flags | MS_NOSEC;
		strlcpy(s->s_id, bdevname(bdev, b), sizeof(s->s_id));
<<<<<<< HEAD
		error = btrfs_fill_super(s, fs_devices, data,
					 flags & MS_SILENT ? 1 : 0);
		if (error) {
			deactivate_locked_super(s);
			goto error_free_subvol_name;
		}

		btrfs_sb(s)->fs_info->bdev_holder = fs_type;
		s->s_flags |= MS_ACTIVE;
	}

	/* if they gave us a subvolume name bind mount into that */
	if (strcmp(subvol_name, ".")) {
		struct dentry *new_root;

		root = get_default_root(s, subvol_rootid);
		if (IS_ERR(root)) {
			error = PTR_ERR(root);
			deactivate_locked_super(s);
			goto error_free_subvol_name;
		}

		mutex_lock(&root->d_inode->i_mutex);
		new_root = lookup_one_len(subvol_name, root,
				      strlen(subvol_name));
		mutex_unlock(&root->d_inode->i_mutex);

		if (IS_ERR(new_root)) {
			dput(root);
			deactivate_locked_super(s);
			error = PTR_ERR(new_root);
			goto error_free_subvol_name;
		}
		if (!new_root->d_inode) {
			dput(root);
			dput(new_root);
			deactivate_locked_super(s);
			error = -ENXIO;
			goto error_free_subvol_name;
		}
		dput(root);
		root = new_root;
	} else {
		root = get_default_root(s, subvol_objectid);
		if (IS_ERR(root)) {
			error = PTR_ERR(root);
			deactivate_locked_super(s);
			goto error_free_subvol_name;
		}
	}

	kfree(subvol_name);
	return root;

error_s:
	error = PTR_ERR(s);
error_close_devices:
	btrfs_close_devices(fs_devices);
	kfree(fs_info);
	kfree(tree_root);
error_free_subvol_name:
	kfree(subvol_name);
=======
=======
	} else {
		char b[BDEVNAME_SIZE];

		strlcpy(s->s_id, bdevname(bdev, b), sizeof(s->s_id));
>>>>>>> refs/remotes/origin/master
		btrfs_sb(s)->bdev_holder = fs_type;
		error = btrfs_fill_super(s, fs_devices, data,
					 flags & MS_SILENT ? 1 : 0);
	}

	root = !error ? get_default_root(s, subvol_objectid) : ERR_PTR(error);
	if (IS_ERR(root))
		deactivate_locked_super(s);

	return root;

error_close_devices:
	btrfs_close_devices(fs_devices);
error_fs_info:
	free_fs_info(fs_info);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return ERR_PTR(error);
}

static int btrfs_remount(struct super_block *sb, int *flags, char *data)
{
<<<<<<< HEAD
	struct btrfs_root *root = btrfs_sb(sb);
	int ret;

	ret = btrfs_parse_options(root, data);
	if (ret)
		return -EINVAL;
=======
=======
	return ERR_PTR(error);
}

static void btrfs_set_max_workers(struct btrfs_workers *workers, int new_limit)
{
	spin_lock_irq(&workers->lock);
	workers->max_workers = new_limit;
	spin_unlock_irq(&workers->lock);
}

static void btrfs_resize_thread_pool(struct btrfs_fs_info *fs_info,
				     int new_pool_size, int old_pool_size)
{
	if (new_pool_size == old_pool_size)
		return;

	fs_info->thread_pool_size = new_pool_size;

	printk(KERN_INFO "btrfs: resize thread pool %d -> %d\n",
	       old_pool_size, new_pool_size);

	btrfs_set_max_workers(&fs_info->generic_worker, new_pool_size);
	btrfs_set_max_workers(&fs_info->workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->delalloc_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->submit_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->caching_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->fixup_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->endio_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->endio_meta_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->endio_meta_write_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->endio_write_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->endio_freespace_worker, new_pool_size);
	btrfs_set_max_workers(&fs_info->delayed_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->readahead_workers, new_pool_size);
	btrfs_set_max_workers(&fs_info->scrub_wr_completion_workers,
			      new_pool_size);
}

static inline void btrfs_remount_prepare(struct btrfs_fs_info *fs_info)
{
	set_bit(BTRFS_FS_STATE_REMOUNTING, &fs_info->fs_state);
}

static inline void btrfs_remount_begin(struct btrfs_fs_info *fs_info,
				       unsigned long old_opts, int flags)
{
	if (btrfs_raw_test_opt(old_opts, AUTO_DEFRAG) &&
	    (!btrfs_raw_test_opt(fs_info->mount_opt, AUTO_DEFRAG) ||
	     (flags & MS_RDONLY))) {
		/* wait for any defraggers to finish */
		wait_event(fs_info->transaction_wait,
			   (atomic_read(&fs_info->defrag_running) == 0));
		if (flags & MS_RDONLY)
			sync_filesystem(fs_info->sb);
	}
}

static inline void btrfs_remount_cleanup(struct btrfs_fs_info *fs_info,
					 unsigned long old_opts)
{
	/*
	 * We need cleanup all defragable inodes if the autodefragment is
	 * close or the fs is R/O.
	 */
	if (btrfs_raw_test_opt(old_opts, AUTO_DEFRAG) &&
	    (!btrfs_raw_test_opt(fs_info->mount_opt, AUTO_DEFRAG) ||
	     (fs_info->sb->s_flags & MS_RDONLY))) {
		btrfs_cleanup_defrag_inodes(fs_info);
	}

	clear_bit(BTRFS_FS_STATE_REMOUNTING, &fs_info->fs_state);
}

static int btrfs_remount(struct super_block *sb, int *flags, char *data)
{
>>>>>>> refs/remotes/origin/master
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
	struct btrfs_root *root = fs_info->tree_root;
	unsigned old_flags = sb->s_flags;
	unsigned long old_opts = fs_info->mount_opt;
	unsigned long old_compress_type = fs_info->compress_type;
	u64 old_max_inline = fs_info->max_inline;
	u64 old_alloc_start = fs_info->alloc_start;
	int old_thread_pool_size = fs_info->thread_pool_size;
	unsigned int old_metadata_ratio = fs_info->metadata_ratio;
	int ret;

<<<<<<< HEAD
=======
	btrfs_remount_prepare(fs_info);

>>>>>>> refs/remotes/origin/master
	ret = btrfs_parse_options(root, data);
	if (ret) {
		ret = -EINVAL;
		goto restore;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	if ((*flags & MS_RDONLY) == (sb->s_flags & MS_RDONLY))
		return 0;

	if (*flags & MS_RDONLY) {
		sb->s_flags |= MS_RDONLY;

<<<<<<< HEAD
		ret =  btrfs_commit_super(root);
		WARN_ON(ret);
	} else {
		if (root->fs_info->fs_devices->rw_devices == 0)
			return -EACCES;

		if (btrfs_super_log_root(&root->fs_info->super_copy) != 0)
			return -EINVAL;

		ret = btrfs_cleanup_fs_roots(root->fs_info);
		WARN_ON(ret);

		/* recover relocation */
		ret = btrfs_recover_relocation(root);
		WARN_ON(ret);
=======
=======

	btrfs_remount_begin(fs_info, old_opts, *flags);
	btrfs_resize_thread_pool(fs_info,
		fs_info->thread_pool_size, old_thread_pool_size);

	if ((*flags & MS_RDONLY) == (sb->s_flags & MS_RDONLY))
		goto out;

	if (*flags & MS_RDONLY) {
		/*
		 * this also happens on 'umount -rf' or on shutdown, when
		 * the filesystem is busy.
		 */

		/* wait for the uuid_scan task to finish */
		down(&fs_info->uuid_tree_rescan_sem);
		/* avoid complains from lockdep et al. */
		up(&fs_info->uuid_tree_rescan_sem);

		sb->s_flags |= MS_RDONLY;

		btrfs_dev_replace_suspend_for_unmount(fs_info);
		btrfs_scrub_cancel(fs_info);
		btrfs_pause_balance(fs_info);

>>>>>>> refs/remotes/origin/master
		ret = btrfs_commit_super(root);
		if (ret)
			goto restore;
	} else {
<<<<<<< HEAD
=======
		if (test_bit(BTRFS_FS_STATE_ERROR, &root->fs_info->fs_state)) {
			btrfs_err(fs_info,
				"Remounting read-write after error is not allowed\n");
			ret = -EINVAL;
			goto restore;
		}
>>>>>>> refs/remotes/origin/master
		if (fs_info->fs_devices->rw_devices == 0) {
			ret = -EACCES;
			goto restore;
		}

<<<<<<< HEAD
=======
		if (fs_info->fs_devices->missing_devices >
		     fs_info->num_tolerated_disk_barrier_failures &&
		    !(*flags & MS_RDONLY)) {
			printk(KERN_WARNING
			       "Btrfs: too many missing devices, writeable remount is not allowed\n");
			ret = -EACCES;
			goto restore;
		}

>>>>>>> refs/remotes/origin/master
		if (btrfs_super_log_root(fs_info->super_copy) != 0) {
			ret = -EINVAL;
			goto restore;
		}

		ret = btrfs_cleanup_fs_roots(fs_info);
		if (ret)
			goto restore;

		/* recover relocation */
		ret = btrfs_recover_relocation(root);
		if (ret)
			goto restore;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

		sb->s_flags &= ~MS_RDONLY;
	}

	return 0;
<<<<<<< HEAD
=======
=======

		ret = btrfs_resume_balance_async(fs_info);
		if (ret)
			goto restore;

		ret = btrfs_resume_dev_replace_async(fs_info);
		if (ret) {
			pr_warn("btrfs: failed to resume dev_replace\n");
			goto restore;
		}

		if (!fs_info->uuid_root) {
			pr_info("btrfs: creating UUID tree\n");
			ret = btrfs_create_uuid_tree(fs_info);
			if (ret) {
				pr_warn("btrfs: failed to create the uuid tree"
					"%d\n", ret);
				goto restore;
			}
		}
		sb->s_flags &= ~MS_RDONLY;
	}
out:
	btrfs_remount_cleanup(fs_info, old_opts);
	return 0;
>>>>>>> refs/remotes/origin/master

restore:
	/* We've hit an error - don't reset MS_RDONLY */
	if (sb->s_flags & MS_RDONLY)
		old_flags |= MS_RDONLY;
	sb->s_flags = old_flags;
	fs_info->mount_opt = old_opts;
	fs_info->compress_type = old_compress_type;
	fs_info->max_inline = old_max_inline;
<<<<<<< HEAD
	fs_info->alloc_start = old_alloc_start;
	fs_info->thread_pool_size = old_thread_pool_size;
	fs_info->metadata_ratio = old_metadata_ratio;
	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mutex_lock(&fs_info->chunk_mutex);
	fs_info->alloc_start = old_alloc_start;
	mutex_unlock(&fs_info->chunk_mutex);
	btrfs_resize_thread_pool(fs_info,
		old_thread_pool_size, fs_info->thread_pool_size);
	fs_info->metadata_ratio = old_metadata_ratio;
	btrfs_remount_cleanup(fs_info, old_opts);
	return ret;
>>>>>>> refs/remotes/origin/master
}

/* Used to sort the devices by max_avail(descending sort) */
static int btrfs_cmp_device_free_bytes(const void *dev_info1,
				       const void *dev_info2)
{
	if (((struct btrfs_device_info *)dev_info1)->max_avail >
	    ((struct btrfs_device_info *)dev_info2)->max_avail)
		return -1;
	else if (((struct btrfs_device_info *)dev_info1)->max_avail <
		 ((struct btrfs_device_info *)dev_info2)->max_avail)
		return 1;
	else
	return 0;
}

/*
 * sort the devices by max_avail, in which max free extent size of each device
 * is stored.(Descending Sort)
 */
static inline void btrfs_descending_sort_devices(
					struct btrfs_device_info *devices,
					size_t nr_devices)
{
	sort(devices, nr_devices, sizeof(struct btrfs_device_info),
	     btrfs_cmp_device_free_bytes, NULL);
}

/*
 * The helper to calc the free space on the devices that can be used to store
 * file data.
 */
static int btrfs_calc_avail_data_space(struct btrfs_root *root, u64 *free_bytes)
{
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct btrfs_device_info *devices_info;
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	struct btrfs_device *device;
	u64 skip_space;
	u64 type;
	u64 avail_space;
	u64 used_space;
	u64 min_stripe_size;
<<<<<<< HEAD
<<<<<<< HEAD
	int min_stripes = 1;
	int i = 0, nr_devices;
	int ret;

	nr_devices = fs_info->fs_devices->rw_devices;
=======
=======
>>>>>>> refs/remotes/origin/master
	int min_stripes = 1, num_stripes = 1;
	int i = 0, nr_devices;
	int ret;

	nr_devices = fs_info->fs_devices->open_devices;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	BUG_ON(!nr_devices);

	devices_info = kmalloc(sizeof(*devices_info) * nr_devices,
=======
	BUG_ON(!nr_devices);

	devices_info = kmalloc_array(nr_devices, sizeof(*devices_info),
>>>>>>> refs/remotes/origin/master
			       GFP_NOFS);
	if (!devices_info)
		return -ENOMEM;

	/* calc min stripe number for data space alloction */
	type = btrfs_get_alloc_profile(root, 1);
<<<<<<< HEAD
<<<<<<< HEAD
	if (type & BTRFS_BLOCK_GROUP_RAID0)
		min_stripes = 2;
	else if (type & BTRFS_BLOCK_GROUP_RAID1)
		min_stripes = 2;
	else if (type & BTRFS_BLOCK_GROUP_RAID10)
		min_stripes = 4;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (type & BTRFS_BLOCK_GROUP_RAID0) {
		min_stripes = 2;
		num_stripes = nr_devices;
	} else if (type & BTRFS_BLOCK_GROUP_RAID1) {
		min_stripes = 2;
		num_stripes = 2;
	} else if (type & BTRFS_BLOCK_GROUP_RAID10) {
		min_stripes = 4;
		num_stripes = 4;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (type & BTRFS_BLOCK_GROUP_DUP)
		min_stripe_size = 2 * BTRFS_STRIPE_LEN;
	else
		min_stripe_size = BTRFS_STRIPE_LEN;

<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(device, &fs_devices->alloc_list, dev_alloc_list) {
		if (!device->in_fs_metadata)
=======
	list_for_each_entry(device, &fs_devices->devices, dev_list) {
		if (!device->in_fs_metadata || !device->bdev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	list_for_each_entry(device, &fs_devices->devices, dev_list) {
		if (!device->in_fs_metadata || !device->bdev ||
		    device->is_tgtdev_for_dev_replace)
>>>>>>> refs/remotes/origin/master
			continue;

		avail_space = device->total_bytes - device->bytes_used;

		/* align with stripe_len */
		do_div(avail_space, BTRFS_STRIPE_LEN);
		avail_space *= BTRFS_STRIPE_LEN;

		/*
		 * In order to avoid overwritting the superblock on the drive,
		 * btrfs starts at an offset of at least 1MB when doing chunk
		 * allocation.
		 */
		skip_space = 1024 * 1024;

		/* user can set the offset in fs_info->alloc_start. */
		if (fs_info->alloc_start + BTRFS_STRIPE_LEN <=
		    device->total_bytes)
			skip_space = max(fs_info->alloc_start, skip_space);

		/*
		 * btrfs can not use the free space in [0, skip_space - 1],
		 * we must subtract it from the total. In order to implement
		 * it, we account the used space in this range first.
		 */
		ret = btrfs_account_dev_extents_size(device, 0, skip_space - 1,
						     &used_space);
		if (ret) {
			kfree(devices_info);
			return ret;
		}

		/* calc the free space in [0, skip_space - 1] */
		skip_space -= used_space;

		/*
		 * we can use the free space in [0, skip_space - 1], subtract
		 * it from the total.
		 */
		if (avail_space && avail_space >= skip_space)
			avail_space -= skip_space;
		else
			avail_space = 0;

		if (avail_space < min_stripe_size)
			continue;

		devices_info[i].dev = device;
		devices_info[i].max_avail = avail_space;

		i++;
	}

	nr_devices = i;

	btrfs_descending_sort_devices(devices_info, nr_devices);

	i = nr_devices - 1;
	avail_space = 0;
	while (nr_devices >= min_stripes) {
<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (num_stripes > nr_devices)
			num_stripes = nr_devices;

>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (num_stripes > nr_devices)
			num_stripes = nr_devices;

>>>>>>> refs/remotes/origin/master
		if (devices_info[i].max_avail >= min_stripe_size) {
			int j;
			u64 alloc_size;

<<<<<<< HEAD
<<<<<<< HEAD
			avail_space += devices_info[i].max_avail * min_stripes;
			alloc_size = devices_info[i].max_avail;
			for (j = i + 1 - min_stripes; j <= i; j++)
=======
			avail_space += devices_info[i].max_avail * num_stripes;
			alloc_size = devices_info[i].max_avail;
			for (j = i + 1 - num_stripes; j <= i; j++)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			avail_space += devices_info[i].max_avail * num_stripes;
			alloc_size = devices_info[i].max_avail;
			for (j = i + 1 - num_stripes; j <= i; j++)
>>>>>>> refs/remotes/origin/master
				devices_info[j].max_avail -= alloc_size;
		}
		i--;
		nr_devices--;
	}

	kfree(devices_info);
	*free_bytes = avail_space;
	return 0;
}

static int btrfs_statfs(struct dentry *dentry, struct kstatfs *buf)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct btrfs_root *root = btrfs_sb(dentry->d_sb);
	struct btrfs_super_block *disk_super = &root->fs_info->super_copy;
	struct list_head *head = &root->fs_info->space_info;
=======
	struct btrfs_fs_info *fs_info = btrfs_sb(dentry->d_sb);
	struct btrfs_super_block *disk_super = fs_info->super_copy;
	struct list_head *head = &fs_info->space_info;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct btrfs_fs_info *fs_info = btrfs_sb(dentry->d_sb);
	struct btrfs_super_block *disk_super = fs_info->super_copy;
	struct list_head *head = &fs_info->space_info;
>>>>>>> refs/remotes/origin/master
	struct btrfs_space_info *found;
	u64 total_used = 0;
	u64 total_free_data = 0;
	int bits = dentry->d_sb->s_blocksize_bits;
<<<<<<< HEAD
<<<<<<< HEAD
	__be32 *fsid = (__be32 *)root->fs_info->fsid;
	int ret;

	/* holding chunk_muext to avoid allocating new chunks */
	mutex_lock(&root->fs_info->chunk_mutex);
=======
=======
>>>>>>> refs/remotes/origin/master
	__be32 *fsid = (__be32 *)fs_info->fsid;
	int ret;

	/* holding chunk_muext to avoid allocating new chunks */
	mutex_lock(&fs_info->chunk_mutex);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	rcu_read_lock();
	list_for_each_entry_rcu(found, head, list) {
		if (found->flags & BTRFS_BLOCK_GROUP_DATA) {
			total_free_data += found->disk_total - found->disk_used;
			total_free_data -=
				btrfs_account_ro_block_groups_free_space(found);
		}

		total_used += found->disk_used;
	}
	rcu_read_unlock();

	buf->f_namelen = BTRFS_NAME_LEN;
	buf->f_blocks = btrfs_super_total_bytes(disk_super) >> bits;
	buf->f_bfree = buf->f_blocks - (total_used >> bits);
	buf->f_bsize = dentry->d_sb->s_blocksize;
	buf->f_type = BTRFS_SUPER_MAGIC;
	buf->f_bavail = total_free_data;
<<<<<<< HEAD
<<<<<<< HEAD
	ret = btrfs_calc_avail_data_space(root, &total_free_data);
	if (ret) {
		mutex_unlock(&root->fs_info->chunk_mutex);
=======
	ret = btrfs_calc_avail_data_space(fs_info->tree_root, &total_free_data);
	if (ret) {
		mutex_unlock(&fs_info->chunk_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = btrfs_calc_avail_data_space(fs_info->tree_root, &total_free_data);
	if (ret) {
		mutex_unlock(&fs_info->chunk_mutex);
>>>>>>> refs/remotes/origin/master
		return ret;
	}
	buf->f_bavail += total_free_data;
	buf->f_bavail = buf->f_bavail >> bits;
<<<<<<< HEAD
<<<<<<< HEAD
	mutex_unlock(&root->fs_info->chunk_mutex);
=======
	mutex_unlock(&fs_info->chunk_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mutex_unlock(&fs_info->chunk_mutex);
>>>>>>> refs/remotes/origin/master

	/* We treat it as constant endianness (it doesn't matter _which_)
	   because we want the fsid to come out the same whether mounted
	   on a big-endian or little-endian host */
	buf->f_fsid.val[0] = be32_to_cpu(fsid[0]) ^ be32_to_cpu(fsid[2]);
	buf->f_fsid.val[1] = be32_to_cpu(fsid[1]) ^ be32_to_cpu(fsid[3]);
	/* Mask in the root object ID too, to disambiguate subvols */
	buf->f_fsid.val[0] ^= BTRFS_I(dentry->d_inode)->root->objectid >> 32;
	buf->f_fsid.val[1] ^= BTRFS_I(dentry->d_inode)->root->objectid;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void btrfs_kill_super(struct super_block *sb)
{
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
	kill_anon_super(sb);
	free_fs_info(fs_info);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct file_system_type btrfs_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "btrfs",
	.mount		= btrfs_mount,
<<<<<<< HEAD
<<<<<<< HEAD
	.kill_sb	= kill_anon_super,
=======
	.kill_sb	= btrfs_kill_super,
>>>>>>> refs/remotes/origin/cm-10.0
	.fs_flags	= FS_REQUIRES_DEV,
};
=======
	.kill_sb	= btrfs_kill_super,
	.fs_flags	= FS_REQUIRES_DEV,
};
MODULE_ALIAS_FS("btrfs");
>>>>>>> refs/remotes/origin/master

/*
 * used by btrfsctl to scan devices when no FS is mounted
 */
static long btrfs_control_ioctl(struct file *file, unsigned int cmd,
				unsigned long arg)
{
	struct btrfs_ioctl_vol_args *vol;
	struct btrfs_fs_devices *fs_devices;
	int ret = -ENOTTY;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	vol = memdup_user((void __user *)arg, sizeof(*vol));
	if (IS_ERR(vol))
		return PTR_ERR(vol);

	switch (cmd) {
	case BTRFS_IOC_SCAN_DEV:
		ret = btrfs_scan_one_device(vol->name, FMODE_READ,
					    &btrfs_fs_type, &fs_devices);
		break;
<<<<<<< HEAD
=======
	case BTRFS_IOC_DEVICES_READY:
		ret = btrfs_scan_one_device(vol->name, FMODE_READ,
					    &btrfs_fs_type, &fs_devices);
		if (ret)
			break;
		ret = !(fs_devices->num_devices == fs_devices->total_devices);
		break;
>>>>>>> refs/remotes/origin/master
	}

	kfree(vol);
	return ret;
}

static int btrfs_freeze(struct super_block *sb)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct btrfs_root *root = btrfs_sb(sb);
	mutex_lock(&root->fs_info->transaction_kthread_mutex);
	mutex_lock(&root->fs_info->cleaner_mutex);
=======
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
	mutex_lock(&fs_info->transaction_kthread_mutex);
	mutex_lock(&fs_info->cleaner_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
=======
	struct btrfs_trans_handle *trans;
	struct btrfs_root *root = btrfs_sb(sb)->tree_root;

	trans = btrfs_attach_transaction_barrier(root);
	if (IS_ERR(trans)) {
		/* no transaction, don't bother */
		if (PTR_ERR(trans) == -ENOENT)
			return 0;
		return PTR_ERR(trans);
	}
	return btrfs_commit_transaction(trans, root);
>>>>>>> refs/remotes/origin/master
}

static int btrfs_unfreeze(struct super_block *sb)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct btrfs_root *root = btrfs_sb(sb);
	mutex_unlock(&root->fs_info->cleaner_mutex);
	mutex_unlock(&root->fs_info->transaction_kthread_mutex);
	return 0;
}

=======
	struct btrfs_fs_info *fs_info = btrfs_sb(sb);
	mutex_unlock(&fs_info->cleaner_mutex);
	mutex_unlock(&fs_info->transaction_kthread_mutex);
	return 0;
}

static void btrfs_fs_dirty_inode(struct inode *inode, int flags)
{
	int ret;

	ret = btrfs_dirty_inode(inode);
	if (ret)
		printk_ratelimited(KERN_ERR "btrfs: fail to dirty inode %Lu "
				   "error %d\n", btrfs_ino(inode), ret);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	return 0;
}

static int btrfs_show_devname(struct seq_file *m, struct dentry *root)
{
	struct btrfs_fs_info *fs_info = btrfs_sb(root->d_sb);
	struct btrfs_fs_devices *cur_devices;
	struct btrfs_device *dev, *first_dev = NULL;
	struct list_head *head;
	struct rcu_string *name;

	mutex_lock(&fs_info->fs_devices->device_list_mutex);
	cur_devices = fs_info->fs_devices;
	while (cur_devices) {
		head = &cur_devices->devices;
		list_for_each_entry(dev, head, dev_list) {
			if (dev->missing)
				continue;
			if (!first_dev || dev->devid < first_dev->devid)
				first_dev = dev;
		}
		cur_devices = cur_devices->seed;
	}

	if (first_dev) {
		rcu_read_lock();
		name = rcu_dereference(first_dev->name);
		seq_escape(m, name->str, " \t\n\\");
		rcu_read_unlock();
	} else {
		WARN_ON(1);
	}
	mutex_unlock(&fs_info->fs_devices->device_list_mutex);
	return 0;
}

>>>>>>> refs/remotes/origin/master
static const struct super_operations btrfs_super_ops = {
	.drop_inode	= btrfs_drop_inode,
	.evict_inode	= btrfs_evict_inode,
	.put_super	= btrfs_put_super,
	.sync_fs	= btrfs_sync_fs,
	.show_options	= btrfs_show_options,
<<<<<<< HEAD
	.write_inode	= btrfs_write_inode,
<<<<<<< HEAD
	.dirty_inode	= btrfs_dirty_inode,
=======
	.dirty_inode	= btrfs_fs_dirty_inode,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.show_devname	= btrfs_show_devname,
	.write_inode	= btrfs_write_inode,
>>>>>>> refs/remotes/origin/master
	.alloc_inode	= btrfs_alloc_inode,
	.destroy_inode	= btrfs_destroy_inode,
	.statfs		= btrfs_statfs,
	.remount_fs	= btrfs_remount,
	.freeze_fs	= btrfs_freeze,
	.unfreeze_fs	= btrfs_unfreeze,
};

static const struct file_operations btrfs_ctl_fops = {
	.unlocked_ioctl	 = btrfs_control_ioctl,
	.compat_ioctl = btrfs_control_ioctl,
	.owner	 = THIS_MODULE,
	.llseek = noop_llseek,
};

static struct miscdevice btrfs_misc = {
	.minor		= BTRFS_MINOR,
	.name		= "btrfs-control",
	.fops		= &btrfs_ctl_fops
};

MODULE_ALIAS_MISCDEV(BTRFS_MINOR);
MODULE_ALIAS("devname:btrfs-control");

static int btrfs_interface_init(void)
{
	return misc_register(&btrfs_misc);
}

static void btrfs_interface_exit(void)
{
	if (misc_deregister(&btrfs_misc) < 0)
<<<<<<< HEAD
		printk(KERN_INFO "misc_deregister failed for control device");
=======
		printk(KERN_INFO "btrfs: misc_deregister failed for control device\n");
}

static void btrfs_print_info(void)
{
	printk(KERN_INFO "Btrfs loaded"
#ifdef CONFIG_BTRFS_DEBUG
			", debug=on"
#endif
#ifdef CONFIG_BTRFS_ASSERT
			", assert=on"
#endif
#ifdef CONFIG_BTRFS_FS_CHECK_INTEGRITY
			", integrity-checker=on"
#endif
			"\n");
}

static int btrfs_run_sanity_tests(void)
{
	int ret;

	ret = btrfs_init_test_fs();
	if (ret)
		return ret;

	ret = btrfs_test_free_space_cache();
	if (ret)
		goto out;
	ret = btrfs_test_extent_buffer_operations();
	if (ret)
		goto out;
	ret = btrfs_test_extent_io();
	if (ret)
		goto out;
	ret = btrfs_test_inodes();
out:
	btrfs_destroy_test_fs();
	return ret;
>>>>>>> refs/remotes/origin/master
}

static int __init init_btrfs_fs(void)
{
	int err;

	err = btrfs_init_sysfs();
	if (err)
		return err;

<<<<<<< HEAD
<<<<<<< HEAD
	err = btrfs_init_compress();
	if (err)
		goto free_sysfs;
=======
	btrfs_init_compress();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	btrfs_init_compress();
>>>>>>> refs/remotes/origin/master

	err = btrfs_init_cachep();
	if (err)
		goto free_compress;

	err = extent_io_init();
	if (err)
		goto free_cachep;

	err = extent_map_init();
	if (err)
		goto free_extent_io;

<<<<<<< HEAD
	err = btrfs_delayed_inode_init();
	if (err)
		goto free_extent_map;

	err = btrfs_interface_init();
	if (err)
		goto free_delayed_inode;

	err = register_filesystem(&btrfs_fs_type);
	if (err)
		goto unregister_ioctl;

<<<<<<< HEAD
=======
	btrfs_init_lockdep();

>>>>>>> refs/remotes/origin/cm-10.0
	printk(KERN_INFO "%s loaded\n", BTRFS_BUILD_VERSION);
=======
	err = ordered_data_init();
	if (err)
		goto free_extent_map;

	err = btrfs_delayed_inode_init();
	if (err)
		goto free_ordered_data;

	err = btrfs_auto_defrag_init();
	if (err)
		goto free_delayed_inode;

	err = btrfs_delayed_ref_init();
	if (err)
		goto free_auto_defrag;

	err = btrfs_prelim_ref_init();
	if (err)
		goto free_prelim_ref;

	err = btrfs_interface_init();
	if (err)
		goto free_delayed_ref;

	btrfs_init_lockdep();

	btrfs_print_info();

	err = btrfs_run_sanity_tests();
	if (err)
		goto unregister_ioctl;

	err = register_filesystem(&btrfs_fs_type);
	if (err)
		goto unregister_ioctl;

>>>>>>> refs/remotes/origin/master
	return 0;

unregister_ioctl:
	btrfs_interface_exit();
<<<<<<< HEAD
free_delayed_inode:
	btrfs_delayed_inode_exit();
=======
free_prelim_ref:
	btrfs_prelim_ref_exit();
free_delayed_ref:
	btrfs_delayed_ref_exit();
free_auto_defrag:
	btrfs_auto_defrag_exit();
free_delayed_inode:
	btrfs_delayed_inode_exit();
free_ordered_data:
	ordered_data_exit();
>>>>>>> refs/remotes/origin/master
free_extent_map:
	extent_map_exit();
free_extent_io:
	extent_io_exit();
free_cachep:
	btrfs_destroy_cachep();
free_compress:
	btrfs_exit_compress();
<<<<<<< HEAD
<<<<<<< HEAD
free_sysfs:
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	btrfs_exit_sysfs();
	return err;
}

static void __exit exit_btrfs_fs(void)
{
	btrfs_destroy_cachep();
<<<<<<< HEAD
	btrfs_delayed_inode_exit();
=======
	btrfs_delayed_ref_exit();
	btrfs_auto_defrag_exit();
	btrfs_delayed_inode_exit();
	btrfs_prelim_ref_exit();
	ordered_data_exit();
>>>>>>> refs/remotes/origin/master
	extent_map_exit();
	extent_io_exit();
	btrfs_interface_exit();
	unregister_filesystem(&btrfs_fs_type);
	btrfs_exit_sysfs();
	btrfs_cleanup_fs_uuids();
	btrfs_exit_compress();
}

module_init(init_btrfs_fs)
module_exit(exit_btrfs_fs)

MODULE_LICENSE("GPL");
