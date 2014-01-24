/*
 *  linux/fs/readdir.c
 *
 *  Copyright (C) 1995  Linus Torvalds
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/time.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/stat.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/dirent.h>
#include <linux/security.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>

#include <asm/uaccess.h>

<<<<<<< HEAD
int vfs_readdir(struct file *file, filldir_t filler, void *buf)
{
	struct inode *inode = file->f_path.dentry->d_inode;
	int res = -ENOTDIR;
	if (!file->f_op || !file->f_op->readdir)
=======
int iterate_dir(struct file *file, struct dir_context *ctx)
{
	struct inode *inode = file_inode(file);
	int res = -ENOTDIR;
	if (!file->f_op->iterate)
>>>>>>> refs/remotes/origin/master
		goto out;

	res = security_file_permission(file, MAY_READ);
	if (res)
		goto out;

	res = mutex_lock_killable(&inode->i_mutex);
	if (res)
		goto out;

	res = -ENOENT;
	if (!IS_DEADDIR(inode)) {
<<<<<<< HEAD
		res = file->f_op->readdir(file, buf, filler);
=======
		ctx->pos = file->f_pos;
		res = file->f_op->iterate(file, ctx);
		file->f_pos = ctx->pos;
>>>>>>> refs/remotes/origin/master
		file_accessed(file);
	}
	mutex_unlock(&inode->i_mutex);
out:
	return res;
}
<<<<<<< HEAD

EXPORT_SYMBOL(vfs_readdir);
=======
EXPORT_SYMBOL(iterate_dir);
>>>>>>> refs/remotes/origin/master

/*
 * Traditional linux readdir() handling..
 *
 * "count=1" is a special case, meaning that the buffer is one
 * dirent-structure in size and that the code can't handle more
 * anyway. Thus the special "fillonedir()" function for that
 * case (the low-level handlers don't need to care about this).
 */

#ifdef __ARCH_WANT_OLD_READDIR

struct old_linux_dirent {
	unsigned long	d_ino;
	unsigned long	d_offset;
	unsigned short	d_namlen;
	char		d_name[1];
};

struct readdir_callback {
<<<<<<< HEAD
=======
	struct dir_context ctx;
>>>>>>> refs/remotes/origin/master
	struct old_linux_dirent __user * dirent;
	int result;
};

static int fillonedir(void * __buf, const char * name, int namlen, loff_t offset,
		      u64 ino, unsigned int d_type)
{
<<<<<<< HEAD
	struct readdir_callback * buf = (struct readdir_callback *) __buf;
=======
	struct readdir_callback *buf = (struct readdir_callback *) __buf;
>>>>>>> refs/remotes/origin/master
	struct old_linux_dirent __user * dirent;
	unsigned long d_ino;

	if (buf->result)
		return -EINVAL;
	d_ino = ino;
	if (sizeof(d_ino) < sizeof(ino) && d_ino != ino) {
		buf->result = -EOVERFLOW;
		return -EOVERFLOW;
	}
	buf->result++;
	dirent = buf->dirent;
	if (!access_ok(VERIFY_WRITE, dirent,
			(unsigned long)(dirent->d_name + namlen + 1) -
				(unsigned long)dirent))
		goto efault;
	if (	__put_user(d_ino, &dirent->d_ino) ||
		__put_user(offset, &dirent->d_offset) ||
		__put_user(namlen, &dirent->d_namlen) ||
		__copy_to_user(dirent->d_name, name, namlen) ||
		__put_user(0, dirent->d_name + namlen))
		goto efault;
	return 0;
efault:
	buf->result = -EFAULT;
	return -EFAULT;
}

SYSCALL_DEFINE3(old_readdir, unsigned int, fd,
		struct old_linux_dirent __user *, dirent, unsigned int, count)
{
	int error;
<<<<<<< HEAD
	struct file * file;
	struct readdir_callback buf;

	error = -EBADF;
	file = fget(fd);
	if (!file)
		goto out;

	buf.result = 0;
	buf.dirent = dirent;

	error = vfs_readdir(file, fillonedir, &buf);
	if (buf.result)
		error = buf.result;

	fput(file);
out:
=======
	struct fd f = fdget(fd);
	struct readdir_callback buf = {
		.ctx.actor = fillonedir,
		.dirent = dirent
	};

	if (!f.file)
		return -EBADF;

	error = iterate_dir(f.file, &buf.ctx);
	if (buf.result)
		error = buf.result;

	fdput(f);
>>>>>>> refs/remotes/origin/master
	return error;
}

#endif /* __ARCH_WANT_OLD_READDIR */

/*
 * New, all-improved, singing, dancing, iBCS2-compliant getdents()
 * interface. 
 */
struct linux_dirent {
	unsigned long	d_ino;
	unsigned long	d_off;
	unsigned short	d_reclen;
	char		d_name[1];
};

struct getdents_callback {
<<<<<<< HEAD
=======
	struct dir_context ctx;
>>>>>>> refs/remotes/origin/master
	struct linux_dirent __user * current_dir;
	struct linux_dirent __user * previous;
	int count;
	int error;
};

static int filldir(void * __buf, const char * name, int namlen, loff_t offset,
		   u64 ino, unsigned int d_type)
{
	struct linux_dirent __user * dirent;
	struct getdents_callback * buf = (struct getdents_callback *) __buf;
	unsigned long d_ino;
	int reclen = ALIGN(offsetof(struct linux_dirent, d_name) + namlen + 2,
		sizeof(long));

	buf->error = -EINVAL;	/* only used if we fail.. */
	if (reclen > buf->count)
		return -EINVAL;
	d_ino = ino;
	if (sizeof(d_ino) < sizeof(ino) && d_ino != ino) {
		buf->error = -EOVERFLOW;
		return -EOVERFLOW;
	}
	dirent = buf->previous;
	if (dirent) {
		if (__put_user(offset, &dirent->d_off))
			goto efault;
	}
	dirent = buf->current_dir;
	if (__put_user(d_ino, &dirent->d_ino))
		goto efault;
	if (__put_user(reclen, &dirent->d_reclen))
		goto efault;
	if (copy_to_user(dirent->d_name, name, namlen))
		goto efault;
	if (__put_user(0, dirent->d_name + namlen))
		goto efault;
	if (__put_user(d_type, (char __user *) dirent + reclen - 1))
		goto efault;
	buf->previous = dirent;
	dirent = (void __user *)dirent + reclen;
	buf->current_dir = dirent;
	buf->count -= reclen;
	return 0;
efault:
	buf->error = -EFAULT;
	return -EFAULT;
}

SYSCALL_DEFINE3(getdents, unsigned int, fd,
		struct linux_dirent __user *, dirent, unsigned int, count)
{
<<<<<<< HEAD
	struct file * file;
	struct linux_dirent __user * lastdirent;
	struct getdents_callback buf;
	int error;

	error = -EFAULT;
	if (!access_ok(VERIFY_WRITE, dirent, count))
		goto out;

	error = -EBADF;
	file = fget(fd);
	if (!file)
		goto out;

	buf.current_dir = dirent;
	buf.previous = NULL;
	buf.count = count;
	buf.error = 0;

	error = vfs_readdir(file, filldir, &buf);
=======
	struct fd f;
	struct linux_dirent __user * lastdirent;
	struct getdents_callback buf = {
		.ctx.actor = filldir,
		.count = count,
		.current_dir = dirent
	};
	int error;

	if (!access_ok(VERIFY_WRITE, dirent, count))
		return -EFAULT;

	f = fdget(fd);
	if (!f.file)
		return -EBADF;

	error = iterate_dir(f.file, &buf.ctx);
>>>>>>> refs/remotes/origin/master
	if (error >= 0)
		error = buf.error;
	lastdirent = buf.previous;
	if (lastdirent) {
<<<<<<< HEAD
		if (put_user(file->f_pos, &lastdirent->d_off))
=======
		if (put_user(buf.ctx.pos, &lastdirent->d_off))
>>>>>>> refs/remotes/origin/master
			error = -EFAULT;
		else
			error = count - buf.count;
	}
<<<<<<< HEAD
	fput(file);
out:
=======
	fdput(f);
>>>>>>> refs/remotes/origin/master
	return error;
}

struct getdents_callback64 {
<<<<<<< HEAD
=======
	struct dir_context ctx;
>>>>>>> refs/remotes/origin/master
	struct linux_dirent64 __user * current_dir;
	struct linux_dirent64 __user * previous;
	int count;
	int error;
};

static int filldir64(void * __buf, const char * name, int namlen, loff_t offset,
		     u64 ino, unsigned int d_type)
{
	struct linux_dirent64 __user *dirent;
	struct getdents_callback64 * buf = (struct getdents_callback64 *) __buf;
	int reclen = ALIGN(offsetof(struct linux_dirent64, d_name) + namlen + 1,
		sizeof(u64));

	buf->error = -EINVAL;	/* only used if we fail.. */
	if (reclen > buf->count)
		return -EINVAL;
	dirent = buf->previous;
	if (dirent) {
		if (__put_user(offset, &dirent->d_off))
			goto efault;
	}
	dirent = buf->current_dir;
	if (__put_user(ino, &dirent->d_ino))
		goto efault;
	if (__put_user(0, &dirent->d_off))
		goto efault;
	if (__put_user(reclen, &dirent->d_reclen))
		goto efault;
	if (__put_user(d_type, &dirent->d_type))
		goto efault;
	if (copy_to_user(dirent->d_name, name, namlen))
		goto efault;
	if (__put_user(0, dirent->d_name + namlen))
		goto efault;
	buf->previous = dirent;
	dirent = (void __user *)dirent + reclen;
	buf->current_dir = dirent;
	buf->count -= reclen;
	return 0;
efault:
	buf->error = -EFAULT;
	return -EFAULT;
}

SYSCALL_DEFINE3(getdents64, unsigned int, fd,
		struct linux_dirent64 __user *, dirent, unsigned int, count)
{
<<<<<<< HEAD
	struct file * file;
	struct linux_dirent64 __user * lastdirent;
	struct getdents_callback64 buf;
	int error;

	error = -EFAULT;
	if (!access_ok(VERIFY_WRITE, dirent, count))
		goto out;

	error = -EBADF;
	file = fget(fd);
	if (!file)
		goto out;

	buf.current_dir = dirent;
	buf.previous = NULL;
	buf.count = count;
	buf.error = 0;

	error = vfs_readdir(file, filldir64, &buf);
=======
	struct fd f;
	struct linux_dirent64 __user * lastdirent;
	struct getdents_callback64 buf = {
		.ctx.actor = filldir64,
		.count = count,
		.current_dir = dirent
	};
	int error;

	if (!access_ok(VERIFY_WRITE, dirent, count))
		return -EFAULT;

	f = fdget(fd);
	if (!f.file)
		return -EBADF;

	error = iterate_dir(f.file, &buf.ctx);
>>>>>>> refs/remotes/origin/master
	if (error >= 0)
		error = buf.error;
	lastdirent = buf.previous;
	if (lastdirent) {
<<<<<<< HEAD
		typeof(lastdirent->d_off) d_off = file->f_pos;
=======
		typeof(lastdirent->d_off) d_off = buf.ctx.pos;
>>>>>>> refs/remotes/origin/master
		if (__put_user(d_off, &lastdirent->d_off))
			error = -EFAULT;
		else
			error = count - buf.count;
	}
<<<<<<< HEAD
	fput(file);
out:
=======
	fdput(f);
>>>>>>> refs/remotes/origin/master
	return error;
}
