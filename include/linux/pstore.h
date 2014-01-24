/*
 * Persistent Storage - pstore.h
 *
 * Copyright (C) 2010 Intel Corporation <tony.luck@intel.com>
 *
 * This code is the generic layer to export data records from platform
 * level persistent storage via a file system.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _LINUX_PSTORE_H
#define _LINUX_PSTORE_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/time.h>
#include <linux/kmsg_dump.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/time.h>
#include <linux/kmsg_dump.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/errno.h>

>>>>>>> refs/remotes/origin/master
/* types */
enum pstore_type_id {
	PSTORE_TYPE_DMESG	= 0,
	PSTORE_TYPE_MCE		= 1,
<<<<<<< HEAD
	PSTORE_TYPE_UNKNOWN	= 255
};

struct pstore_info {
	struct module	*owner;
	char		*name;
<<<<<<< HEAD
	struct mutex	buf_mutex;	/* serialize access to 'buf' */
	char		*buf;
	size_t		bufsize;
	int		(*open)(struct pstore_info *psi);
	int		(*close)(struct pstore_info *psi);
	ssize_t		(*read)(u64 *id, enum pstore_type_id *type,
			struct timespec *time);
	u64		(*write)(enum pstore_type_id type, size_t size);
	int		(*erase)(u64 id);
=======
=======
	PSTORE_TYPE_CONSOLE	= 2,
	PSTORE_TYPE_FTRACE	= 3,
	/* PPC64 partition types */
	PSTORE_TYPE_PPC_RTAS	= 4,
	PSTORE_TYPE_PPC_OF	= 5,
	PSTORE_TYPE_PPC_COMMON	= 6,
	PSTORE_TYPE_UNKNOWN	= 255
};

struct module;

struct pstore_info {
	struct module	*owner;
	char		*name;
>>>>>>> refs/remotes/origin/master
	spinlock_t	buf_lock;	/* serialize access to 'buf' */
	char		*buf;
	size_t		bufsize;
	struct mutex	read_mutex;	/* serialize open/read/close */
<<<<<<< HEAD
	int		(*open)(struct pstore_info *psi);
	int		(*close)(struct pstore_info *psi);
	ssize_t		(*read)(u64 *id, enum pstore_type_id *type,
			struct timespec *time, char **buf,
			struct pstore_info *psi);
	int		(*write)(enum pstore_type_id type,
			enum kmsg_dump_reason reason, u64 *id,
			unsigned int part, size_t size, struct pstore_info *psi);
	int		(*erase)(enum pstore_type_id type, u64 id,
			struct pstore_info *psi);
	void		*data;
>>>>>>> refs/remotes/origin/cm-10.0
};

#ifdef CONFIG_PSTORE
extern int pstore_register(struct pstore_info *);
<<<<<<< HEAD
extern int pstore_write(enum pstore_type_id type, char *buf, size_t size);
=======
extern bool pstore_cannot_block_path(enum kmsg_dump_reason reason);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int		flags;
	int		(*open)(struct pstore_info *psi);
	int		(*close)(struct pstore_info *psi);
	ssize_t		(*read)(u64 *id, enum pstore_type_id *type,
			int *count, struct timespec *time, char **buf,
			bool *compressed, struct pstore_info *psi);
	int		(*write)(enum pstore_type_id type,
			enum kmsg_dump_reason reason, u64 *id,
			unsigned int part, int count, bool compressed,
			size_t size, struct pstore_info *psi);
	int		(*write_buf)(enum pstore_type_id type,
			enum kmsg_dump_reason reason, u64 *id,
			unsigned int part, const char *buf, bool compressed,
			size_t size, struct pstore_info *psi);
	int		(*erase)(enum pstore_type_id type, u64 id,
			int count, struct timespec time,
			struct pstore_info *psi);
	void		*data;
};

#define	PSTORE_FLAGS_FRAGILE	1

#ifdef CONFIG_PSTORE
extern int pstore_register(struct pstore_info *);
extern bool pstore_cannot_block_path(enum kmsg_dump_reason reason);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
#else
static inline int
pstore_register(struct pstore_info *psi)
{
	return -ENODEV;
}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static inline int
pstore_write(enum pstore_type_id type, char *buf, size_t size)
{
	return -ENODEV;
=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static inline bool
pstore_cannot_block_path(enum kmsg_dump_reason reason)
{
	return false;
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}
#endif

#endif /*_LINUX_PSTORE_H*/
