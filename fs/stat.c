/*
 *  linux/fs/stat.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/file.h>
#include <linux/highuid.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/security.h>
#include <linux/syscalls.h>
#include <linux/pagemap.h>

#include <asm/uaccess.h>
#include <asm/unistd.h>

void generic_fillattr(struct inode *inode, struct kstat *stat)
{
	stat->dev = inode->i_sb->s_dev;
	stat->ino = inode->i_ino;
	stat->mode = inode->i_mode;
	stat->nlink = inode->i_nlink;
	stat->uid = inode->i_uid;
	stat->gid = inode->i_gid;
	stat->rdev = inode->i_rdev;
<<<<<<< HEAD
<<<<<<< HEAD
	stat->atime = inode->i_atime;
	stat->mtime = inode->i_mtime;
	stat->ctime = inode->i_ctime;
	stat->size = i_size_read(inode);
	stat->blocks = inode->i_blocks;
	stat->blksize = (1 << inode->i_blkbits);
=======
=======
>>>>>>> refs/remotes/origin/master
	stat->size = i_size_read(inode);
	stat->atime = inode->i_atime;
	stat->mtime = inode->i_mtime;
	stat->ctime = inode->i_ctime;
	stat->blksize = (1 << inode->i_blkbits);
	stat->blocks = inode->i_blocks;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

EXPORT_SYMBOL(generic_fillattr);

<<<<<<< HEAD
int vfs_getattr(struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat)
{
	struct inode *inode = dentry->d_inode;
	int retval;

	retval = security_inode_getattr(mnt, dentry);
	if (retval)
		return retval;

	if (inode->i_op->getattr)
		return inode->i_op->getattr(mnt, dentry, stat);
=======
/**
 * vfs_getattr_nosec - getattr without security checks
 * @path: file to get attributes from
 * @stat: structure to return attributes in
 *
 * Get attributes without calling security_inode_getattr.
 *
 * Currently the only caller other than vfs_getattr is internal to the
 * filehandle lookup code, which uses only the inode number and returns
 * no attributes to any user.  Any other code probably wants
 * vfs_getattr.
 */
int vfs_getattr_nosec(struct path *path, struct kstat *stat)
{
	struct inode *inode = path->dentry->d_inode;

	if (inode->i_op->getattr)
		return inode->i_op->getattr(path->mnt, path->dentry, stat);
>>>>>>> refs/remotes/origin/master

	generic_fillattr(inode, stat);
	return 0;
}

<<<<<<< HEAD
=======
EXPORT_SYMBOL(vfs_getattr_nosec);

int vfs_getattr(struct path *path, struct kstat *stat)
{
	int retval;

	retval = security_inode_getattr(path->mnt, path->dentry);
	if (retval)
		return retval;
	return vfs_getattr_nosec(path, stat);
}

>>>>>>> refs/remotes/origin/master
EXPORT_SYMBOL(vfs_getattr);

int vfs_fstat(unsigned int fd, struct kstat *stat)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct file *f = fget_raw(fd);
	int error = -EBADF;

	if (f) {
		error = vfs_getattr(f->f_path.mnt, f->f_path.dentry, stat);
		fput(f);
=======
	struct fd f = fdget_raw(fd);
	int error = -EBADF;

	if (f.file) {
		error = vfs_getattr(&f.file->f_path, stat);
		fdput(f);
>>>>>>> refs/remotes/origin/master
	}
	return error;
}
EXPORT_SYMBOL(vfs_fstat);

int vfs_fstatat(int dfd, const char __user *filename, struct kstat *stat,
		int flag)
{
	struct path path;
	int error = -EINVAL;
<<<<<<< HEAD
	int lookup_flags = 0;
=======
	unsigned int lookup_flags = 0;
>>>>>>> refs/remotes/origin/master

	if ((flag & ~(AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT |
		      AT_EMPTY_PATH)) != 0)
		goto out;

	if (!(flag & AT_SYMLINK_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;
<<<<<<< HEAD
<<<<<<< HEAD
	if (flag & AT_NO_AUTOMOUNT)
		lookup_flags |= LOOKUP_NO_AUTOMOUNT;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (flag & AT_EMPTY_PATH)
		lookup_flags |= LOOKUP_EMPTY;

=======
	if (flag & AT_EMPTY_PATH)
		lookup_flags |= LOOKUP_EMPTY;
retry:
>>>>>>> refs/remotes/origin/master
	error = user_path_at(dfd, filename, lookup_flags, &path);
	if (error)
		goto out;

<<<<<<< HEAD
	error = vfs_getattr(path.mnt, path.dentry, stat);
	path_put(&path);
=======
	error = vfs_getattr(&path, stat);
	path_put(&path);
	if (retry_estale(error, lookup_flags)) {
		lookup_flags |= LOOKUP_REVAL;
		goto retry;
	}
>>>>>>> refs/remotes/origin/master
out:
	return error;
}
EXPORT_SYMBOL(vfs_fstatat);

int vfs_stat(const char __user *name, struct kstat *stat)
{
	return vfs_fstatat(AT_FDCWD, name, stat, 0);
}
EXPORT_SYMBOL(vfs_stat);

int vfs_lstat(const char __user *name, struct kstat *stat)
{
	return vfs_fstatat(AT_FDCWD, name, stat, AT_SYMLINK_NOFOLLOW);
}
EXPORT_SYMBOL(vfs_lstat);


#ifdef __ARCH_WANT_OLD_STAT

/*
 * For backward compatibility?  Maybe this should be moved
 * into arch/i386 instead?
 */
static int cp_old_stat(struct kstat *stat, struct __old_kernel_stat __user * statbuf)
{
	static int warncount = 5;
	struct __old_kernel_stat tmp;
	
	if (warncount > 0) {
		warncount--;
		printk(KERN_WARNING "VFS: Warning: %s using old stat() call. Recompile your binary.\n",
			current->comm);
	} else if (warncount < 0) {
		/* it's laughable, but... */
		warncount = 0;
	}

	memset(&tmp, 0, sizeof(struct __old_kernel_stat));
	tmp.st_dev = old_encode_dev(stat->dev);
	tmp.st_ino = stat->ino;
	if (sizeof(tmp.st_ino) < sizeof(stat->ino) && tmp.st_ino != stat->ino)
		return -EOVERFLOW;
	tmp.st_mode = stat->mode;
	tmp.st_nlink = stat->nlink;
	if (tmp.st_nlink != stat->nlink)
		return -EOVERFLOW;
<<<<<<< HEAD
	SET_UID(tmp.st_uid, stat->uid);
	SET_GID(tmp.st_gid, stat->gid);
=======
	SET_UID(tmp.st_uid, from_kuid_munged(current_user_ns(), stat->uid));
	SET_GID(tmp.st_gid, from_kgid_munged(current_user_ns(), stat->gid));
>>>>>>> refs/remotes/origin/master
	tmp.st_rdev = old_encode_dev(stat->rdev);
#if BITS_PER_LONG == 32
	if (stat->size > MAX_NON_LFS)
		return -EOVERFLOW;
#endif	
	tmp.st_size = stat->size;
	tmp.st_atime = stat->atime.tv_sec;
	tmp.st_mtime = stat->mtime.tv_sec;
	tmp.st_ctime = stat->ctime.tv_sec;
	return copy_to_user(statbuf,&tmp,sizeof(tmp)) ? -EFAULT : 0;
}

SYSCALL_DEFINE2(stat, const char __user *, filename,
		struct __old_kernel_stat __user *, statbuf)
{
	struct kstat stat;
	int error;

	error = vfs_stat(filename, &stat);
	if (error)
		return error;

	return cp_old_stat(&stat, statbuf);
}

SYSCALL_DEFINE2(lstat, const char __user *, filename,
		struct __old_kernel_stat __user *, statbuf)
{
	struct kstat stat;
	int error;

	error = vfs_lstat(filename, &stat);
	if (error)
		return error;

	return cp_old_stat(&stat, statbuf);
}

SYSCALL_DEFINE2(fstat, unsigned int, fd, struct __old_kernel_stat __user *, statbuf)
{
	struct kstat stat;
	int error = vfs_fstat(fd, &stat);

	if (!error)
		error = cp_old_stat(&stat, statbuf);

	return error;
}

#endif /* __ARCH_WANT_OLD_STAT */

<<<<<<< HEAD
=======
#if BITS_PER_LONG == 32
#  define choose_32_64(a,b) a
#else
#  define choose_32_64(a,b) b
#endif

#define valid_dev(x)  choose_32_64(old_valid_dev,new_valid_dev)(x)
#define encode_dev(x) choose_32_64(old_encode_dev,new_encode_dev)(x)

#ifndef INIT_STRUCT_STAT_PADDING
#  define INIT_STRUCT_STAT_PADDING(st) memset(&st, 0, sizeof(st))
#endif

>>>>>>> refs/remotes/origin/master
static int cp_new_stat(struct kstat *stat, struct stat __user *statbuf)
{
	struct stat tmp;

<<<<<<< HEAD
#if BITS_PER_LONG == 32
	if (!old_valid_dev(stat->dev) || !old_valid_dev(stat->rdev))
		return -EOVERFLOW;
#else
	if (!new_valid_dev(stat->dev) || !new_valid_dev(stat->rdev))
		return -EOVERFLOW;
#endif

	memset(&tmp, 0, sizeof(tmp));
#if BITS_PER_LONG == 32
	tmp.st_dev = old_encode_dev(stat->dev);
#else
	tmp.st_dev = new_encode_dev(stat->dev);
#endif
=======
	if (!valid_dev(stat->dev) || !valid_dev(stat->rdev))
		return -EOVERFLOW;
#if BITS_PER_LONG == 32
	if (stat->size > MAX_NON_LFS)
		return -EOVERFLOW;
#endif

	INIT_STRUCT_STAT_PADDING(tmp);
	tmp.st_dev = encode_dev(stat->dev);
>>>>>>> refs/remotes/origin/master
	tmp.st_ino = stat->ino;
	if (sizeof(tmp.st_ino) < sizeof(stat->ino) && tmp.st_ino != stat->ino)
		return -EOVERFLOW;
	tmp.st_mode = stat->mode;
	tmp.st_nlink = stat->nlink;
	if (tmp.st_nlink != stat->nlink)
		return -EOVERFLOW;
<<<<<<< HEAD
	SET_UID(tmp.st_uid, stat->uid);
	SET_GID(tmp.st_gid, stat->gid);
#if BITS_PER_LONG == 32
	tmp.st_rdev = old_encode_dev(stat->rdev);
#else
	tmp.st_rdev = new_encode_dev(stat->rdev);
#endif
#if BITS_PER_LONG == 32
	if (stat->size > MAX_NON_LFS)
		return -EOVERFLOW;
#endif	
=======
	SET_UID(tmp.st_uid, from_kuid_munged(current_user_ns(), stat->uid));
	SET_GID(tmp.st_gid, from_kgid_munged(current_user_ns(), stat->gid));
	tmp.st_rdev = encode_dev(stat->rdev);
>>>>>>> refs/remotes/origin/master
	tmp.st_size = stat->size;
	tmp.st_atime = stat->atime.tv_sec;
	tmp.st_mtime = stat->mtime.tv_sec;
	tmp.st_ctime = stat->ctime.tv_sec;
#ifdef STAT_HAVE_NSEC
	tmp.st_atime_nsec = stat->atime.tv_nsec;
	tmp.st_mtime_nsec = stat->mtime.tv_nsec;
	tmp.st_ctime_nsec = stat->ctime.tv_nsec;
#endif
	tmp.st_blocks = stat->blocks;
	tmp.st_blksize = stat->blksize;
	return copy_to_user(statbuf,&tmp,sizeof(tmp)) ? -EFAULT : 0;
}

SYSCALL_DEFINE2(newstat, const char __user *, filename,
		struct stat __user *, statbuf)
{
	struct kstat stat;
	int error = vfs_stat(filename, &stat);

	if (error)
		return error;
	return cp_new_stat(&stat, statbuf);
}

SYSCALL_DEFINE2(newlstat, const char __user *, filename,
		struct stat __user *, statbuf)
{
	struct kstat stat;
	int error;

	error = vfs_lstat(filename, &stat);
	if (error)
		return error;

	return cp_new_stat(&stat, statbuf);
}

#if !defined(__ARCH_WANT_STAT64) || defined(__ARCH_WANT_SYS_NEWFSTATAT)
SYSCALL_DEFINE4(newfstatat, int, dfd, const char __user *, filename,
		struct stat __user *, statbuf, int, flag)
{
	struct kstat stat;
	int error;

	error = vfs_fstatat(dfd, filename, &stat, flag);
	if (error)
		return error;
	return cp_new_stat(&stat, statbuf);
}
#endif

SYSCALL_DEFINE2(newfstat, unsigned int, fd, struct stat __user *, statbuf)
{
	struct kstat stat;
	int error = vfs_fstat(fd, &stat);

	if (!error)
		error = cp_new_stat(&stat, statbuf);

	return error;
}

SYSCALL_DEFINE4(readlinkat, int, dfd, const char __user *, pathname,
		char __user *, buf, int, bufsiz)
{
	struct path path;
	int error;
	int empty = 0;
<<<<<<< HEAD
=======
	unsigned int lookup_flags = LOOKUP_EMPTY;
>>>>>>> refs/remotes/origin/master

	if (bufsiz <= 0)
		return -EINVAL;

<<<<<<< HEAD
	error = user_path_at_empty(dfd, pathname, LOOKUP_EMPTY, &path, &empty);
=======
retry:
	error = user_path_at_empty(dfd, pathname, lookup_flags, &path, &empty);
>>>>>>> refs/remotes/origin/master
	if (!error) {
		struct inode *inode = path.dentry->d_inode;

		error = empty ? -ENOENT : -EINVAL;
		if (inode->i_op->readlink) {
			error = security_inode_readlink(path.dentry);
			if (!error) {
<<<<<<< HEAD
<<<<<<< HEAD
				touch_atime(path.mnt, path.dentry);
=======
				touch_atime(&path);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				touch_atime(&path);
>>>>>>> refs/remotes/origin/master
				error = inode->i_op->readlink(path.dentry,
							      buf, bufsiz);
			}
		}
		path_put(&path);
<<<<<<< HEAD
=======
		if (retry_estale(error, lookup_flags)) {
			lookup_flags |= LOOKUP_REVAL;
			goto retry;
		}
>>>>>>> refs/remotes/origin/master
	}
	return error;
}

SYSCALL_DEFINE3(readlink, const char __user *, path, char __user *, buf,
		int, bufsiz)
{
	return sys_readlinkat(AT_FDCWD, path, buf, bufsiz);
}


/* ---------- LFS-64 ----------- */
<<<<<<< HEAD
#ifdef __ARCH_WANT_STAT64
=======
#if defined(__ARCH_WANT_STAT64) || defined(__ARCH_WANT_COMPAT_STAT64)

#ifndef INIT_STRUCT_STAT64_PADDING
#  define INIT_STRUCT_STAT64_PADDING(st) memset(&st, 0, sizeof(st))
#endif
>>>>>>> refs/remotes/origin/master

static long cp_new_stat64(struct kstat *stat, struct stat64 __user *statbuf)
{
	struct stat64 tmp;

<<<<<<< HEAD
	memset(&tmp, 0, sizeof(struct stat64));
=======
	INIT_STRUCT_STAT64_PADDING(tmp);
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_MIPS
	/* mips has weird padding, so we don't get 64 bits there */
	if (!new_valid_dev(stat->dev) || !new_valid_dev(stat->rdev))
		return -EOVERFLOW;
	tmp.st_dev = new_encode_dev(stat->dev);
	tmp.st_rdev = new_encode_dev(stat->rdev);
#else
	tmp.st_dev = huge_encode_dev(stat->dev);
	tmp.st_rdev = huge_encode_dev(stat->rdev);
#endif
	tmp.st_ino = stat->ino;
	if (sizeof(tmp.st_ino) < sizeof(stat->ino) && tmp.st_ino != stat->ino)
		return -EOVERFLOW;
#ifdef STAT64_HAS_BROKEN_ST_INO
	tmp.__st_ino = stat->ino;
#endif
	tmp.st_mode = stat->mode;
	tmp.st_nlink = stat->nlink;
<<<<<<< HEAD
	tmp.st_uid = stat->uid;
	tmp.st_gid = stat->gid;
=======
	tmp.st_uid = from_kuid_munged(current_user_ns(), stat->uid);
	tmp.st_gid = from_kgid_munged(current_user_ns(), stat->gid);
>>>>>>> refs/remotes/origin/master
	tmp.st_atime = stat->atime.tv_sec;
	tmp.st_atime_nsec = stat->atime.tv_nsec;
	tmp.st_mtime = stat->mtime.tv_sec;
	tmp.st_mtime_nsec = stat->mtime.tv_nsec;
	tmp.st_ctime = stat->ctime.tv_sec;
	tmp.st_ctime_nsec = stat->ctime.tv_nsec;
	tmp.st_size = stat->size;
	tmp.st_blocks = stat->blocks;
	tmp.st_blksize = stat->blksize;
	return copy_to_user(statbuf,&tmp,sizeof(tmp)) ? -EFAULT : 0;
}

SYSCALL_DEFINE2(stat64, const char __user *, filename,
		struct stat64 __user *, statbuf)
{
	struct kstat stat;
	int error = vfs_stat(filename, &stat);

	if (!error)
		error = cp_new_stat64(&stat, statbuf);

	return error;
}

SYSCALL_DEFINE2(lstat64, const char __user *, filename,
		struct stat64 __user *, statbuf)
{
	struct kstat stat;
	int error = vfs_lstat(filename, &stat);

	if (!error)
		error = cp_new_stat64(&stat, statbuf);

	return error;
}

SYSCALL_DEFINE2(fstat64, unsigned long, fd, struct stat64 __user *, statbuf)
{
	struct kstat stat;
	int error = vfs_fstat(fd, &stat);

	if (!error)
		error = cp_new_stat64(&stat, statbuf);

	return error;
}

SYSCALL_DEFINE4(fstatat64, int, dfd, const char __user *, filename,
		struct stat64 __user *, statbuf, int, flag)
{
	struct kstat stat;
	int error;

	error = vfs_fstatat(dfd, filename, &stat, flag);
	if (error)
		return error;
	return cp_new_stat64(&stat, statbuf);
}
<<<<<<< HEAD
#endif /* __ARCH_WANT_STAT64 */
=======
#endif /* __ARCH_WANT_STAT64 || __ARCH_WANT_COMPAT_STAT64 */
>>>>>>> refs/remotes/origin/master

/* Caller is here responsible for sufficient locking (ie. inode->i_lock) */
void __inode_add_bytes(struct inode *inode, loff_t bytes)
{
	inode->i_blocks += bytes >> 9;
	bytes &= 511;
	inode->i_bytes += bytes;
	if (inode->i_bytes >= 512) {
		inode->i_blocks++;
		inode->i_bytes -= 512;
	}
}

void inode_add_bytes(struct inode *inode, loff_t bytes)
{
	spin_lock(&inode->i_lock);
	__inode_add_bytes(inode, bytes);
	spin_unlock(&inode->i_lock);
}

EXPORT_SYMBOL(inode_add_bytes);

<<<<<<< HEAD
void inode_sub_bytes(struct inode *inode, loff_t bytes)
{
	spin_lock(&inode->i_lock);
=======
void __inode_sub_bytes(struct inode *inode, loff_t bytes)
{
>>>>>>> refs/remotes/origin/master
	inode->i_blocks -= bytes >> 9;
	bytes &= 511;
	if (inode->i_bytes < bytes) {
		inode->i_blocks--;
		inode->i_bytes += 512;
	}
	inode->i_bytes -= bytes;
<<<<<<< HEAD
=======
}

EXPORT_SYMBOL(__inode_sub_bytes);

void inode_sub_bytes(struct inode *inode, loff_t bytes)
{
	spin_lock(&inode->i_lock);
	__inode_sub_bytes(inode, bytes);
>>>>>>> refs/remotes/origin/master
	spin_unlock(&inode->i_lock);
}

EXPORT_SYMBOL(inode_sub_bytes);

loff_t inode_get_bytes(struct inode *inode)
{
	loff_t ret;

	spin_lock(&inode->i_lock);
	ret = (((loff_t)inode->i_blocks) << 9) + inode->i_bytes;
	spin_unlock(&inode->i_lock);
	return ret;
}

EXPORT_SYMBOL(inode_get_bytes);

void inode_set_bytes(struct inode *inode, loff_t bytes)
{
	/* Caller is here responsible for sufficient locking
	 * (ie. inode->i_lock) */
	inode->i_blocks = bytes >> 9;
	inode->i_bytes = bytes & 511;
}

EXPORT_SYMBOL(inode_set_bytes);
