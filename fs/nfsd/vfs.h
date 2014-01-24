/*
 * Copyright (C) 1995-1997 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef LINUX_NFSD_VFS_H
#define LINUX_NFSD_VFS_H

#include "nfsfh.h"
<<<<<<< HEAD
=======
#include "nfsd.h"
>>>>>>> refs/remotes/origin/master

/*
 * Flags for nfsd_permission
 */
<<<<<<< HEAD
<<<<<<< HEAD
#define NFSD_MAY_NOP		0
#define NFSD_MAY_EXEC		1 /* == MAY_EXEC */
#define NFSD_MAY_WRITE		2 /* == MAY_WRITE */
#define NFSD_MAY_READ		4 /* == MAY_READ */
#define NFSD_MAY_SATTR		8
#define NFSD_MAY_TRUNC		16
#define NFSD_MAY_LOCK		32
#define NFSD_MAY_MASK		63

/* extra hints to permission and open routines: */
#define NFSD_MAY_OWNER_OVERRIDE	64
#define NFSD_MAY_LOCAL_ACCESS	128 /* IRIX doing local access check on device special file*/
#define NFSD_MAY_BYPASS_GSS_ON_ROOT 256
#define NFSD_MAY_NOT_BREAK_LEASE 512
#define NFSD_MAY_BYPASS_GSS	1024
#define NFSD_MAY_READ_IF_EXEC	2048
=======
=======
>>>>>>> refs/remotes/origin/master
#define NFSD_MAY_NOP			0
#define NFSD_MAY_EXEC			0x001 /* == MAY_EXEC */
#define NFSD_MAY_WRITE			0x002 /* == MAY_WRITE */
#define NFSD_MAY_READ			0x004 /* == MAY_READ */
#define NFSD_MAY_SATTR			0x008
#define NFSD_MAY_TRUNC			0x010
#define NFSD_MAY_LOCK			0x020
#define NFSD_MAY_MASK			0x03f

/* extra hints to permission and open routines: */
#define NFSD_MAY_OWNER_OVERRIDE		0x040
#define NFSD_MAY_LOCAL_ACCESS		0x080 /* for device special files */
#define NFSD_MAY_BYPASS_GSS_ON_ROOT	0x100
#define NFSD_MAY_NOT_BREAK_LEASE	0x200
#define NFSD_MAY_BYPASS_GSS		0x400
#define NFSD_MAY_READ_IF_EXEC		0x800

#define NFSD_MAY_64BIT_COOKIE		0x1000 /* 64 bit readdir cookies for >= NFSv3 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#define NFSD_MAY_CREATE		(NFSD_MAY_EXEC|NFSD_MAY_WRITE)
#define NFSD_MAY_REMOVE		(NFSD_MAY_EXEC|NFSD_MAY_WRITE|NFSD_MAY_TRUNC)

/*
 * Callback function for readdir
 */
typedef int (*nfsd_dirop_t)(struct inode *, struct dentry *, int, int);

/* nfsd/vfs.c */
<<<<<<< HEAD
int		fh_lock_parent(struct svc_fh *, struct dentry *);
=======
>>>>>>> refs/remotes/origin/master
int		nfsd_racache_init(int);
void		nfsd_racache_shutdown(void);
int		nfsd_cross_mnt(struct svc_rqst *rqstp, struct dentry **dpp,
		                struct svc_export **expp);
__be32		nfsd_lookup(struct svc_rqst *, struct svc_fh *,
				const char *, unsigned int, struct svc_fh *);
__be32		 nfsd_lookup_dentry(struct svc_rqst *, struct svc_fh *,
				const char *, unsigned int,
				struct svc_export **, struct dentry **);
__be32		nfsd_setattr(struct svc_rqst *, struct svc_fh *,
				struct iattr *, int, time_t);
int nfsd_mountpoint(struct dentry *, struct svc_export *);
#ifdef CONFIG_NFSD_V4
__be32          nfsd4_set_nfs4_acl(struct svc_rqst *, struct svc_fh *,
                    struct nfs4_acl *);
int             nfsd4_get_nfs4_acl(struct svc_rqst *, struct dentry *, struct nfs4_acl **);
<<<<<<< HEAD
=======
__be32          nfsd4_set_nfs4_label(struct svc_rqst *, struct svc_fh *,
		    struct xdr_netobj *);
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_NFSD_V4 */
__be32		nfsd_create(struct svc_rqst *, struct svc_fh *,
				char *name, int len, struct iattr *attrs,
				int type, dev_t rdev, struct svc_fh *res);
#ifdef CONFIG_NFSD_V3
__be32		nfsd_access(struct svc_rqst *, struct svc_fh *, u32 *, u32 *);
__be32		do_nfsd_create(struct svc_rqst *, struct svc_fh *,
				char *name, int len, struct iattr *attrs,
				struct svc_fh *res, int createmode,
<<<<<<< HEAD
<<<<<<< HEAD
				u32 *verifier, int *truncp, int *created);
__be32		nfsd_commit(struct svc_rqst *, struct svc_fh *,
				loff_t, unsigned long);
#endif /* CONFIG_NFSD_V3 */
__be32		nfsd_open(struct svc_rqst *, struct svc_fh *, int,
=======
=======
>>>>>>> refs/remotes/origin/master
				u32 *verifier, bool *truncp, bool *created);
__be32		nfsd_commit(struct svc_rqst *, struct svc_fh *,
				loff_t, unsigned long);
#endif /* CONFIG_NFSD_V3 */
__be32		nfsd_open(struct svc_rqst *, struct svc_fh *, umode_t,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				int, struct file **);
void		nfsd_close(struct file *);
__be32 		nfsd_read(struct svc_rqst *, struct svc_fh *,
				loff_t, struct kvec *, int, unsigned long *);
__be32 		nfsd_read_file(struct svc_rqst *, struct svc_fh *, struct file *,
				loff_t, struct kvec *, int, unsigned long *);
__be32 		nfsd_write(struct svc_rqst *, struct svc_fh *,struct file *,
				loff_t, struct kvec *,int, unsigned long *, int *);
__be32		nfsd_readlink(struct svc_rqst *, struct svc_fh *,
				char *, int *);
__be32		nfsd_symlink(struct svc_rqst *, struct svc_fh *,
				char *name, int len, char *path, int plen,
				struct svc_fh *res, struct iattr *);
__be32		nfsd_link(struct svc_rqst *, struct svc_fh *,
				char *, int, struct svc_fh *);
__be32		nfsd_rename(struct svc_rqst *,
				struct svc_fh *, char *, int,
				struct svc_fh *, char *, int);
__be32		nfsd_remove(struct svc_rqst *,
				struct svc_fh *, char *, int);
__be32		nfsd_unlink(struct svc_rqst *, struct svc_fh *, int type,
				char *name, int len);
<<<<<<< HEAD
int		nfsd_truncate(struct svc_rqst *, struct svc_fh *,
				unsigned long size);
=======
>>>>>>> refs/remotes/origin/master
__be32		nfsd_readdir(struct svc_rqst *, struct svc_fh *,
			     loff_t *, struct readdir_cd *, filldir_t);
__be32		nfsd_statfs(struct svc_rqst *, struct svc_fh *,
				struct kstatfs *, int access);

<<<<<<< HEAD
int		nfsd_notify_change(struct inode *, struct iattr *);
__be32		nfsd_permission(struct svc_rqst *, struct svc_export *,
				struct dentry *, int);
int		nfsd_sync_dir(struct dentry *dp);
=======
__be32		nfsd_permission(struct svc_rqst *, struct svc_export *,
				struct dentry *, int);
>>>>>>> refs/remotes/origin/master

#if defined(CONFIG_NFSD_V2_ACL) || defined(CONFIG_NFSD_V3_ACL)
struct posix_acl *nfsd_get_posix_acl(struct svc_fh *, int);
int nfsd_set_posix_acl(struct svc_fh *, int, struct posix_acl *);
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
static inline int fh_want_write(struct svc_fh *fh)
{
	return mnt_want_write(fh->fh_export->ex_path.mnt);
=======
static inline int fh_want_write(struct svc_fh *fh)
{
	int ret = mnt_want_write(fh->fh_export->ex_path.mnt);

	if (!ret)
		fh->fh_want_write = 1;
	return ret;
>>>>>>> refs/remotes/origin/master
}

static inline void fh_drop_write(struct svc_fh *fh)
{
<<<<<<< HEAD
	mnt_drop_write(fh->fh_export->ex_path.mnt);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (fh->fh_want_write) {
		fh->fh_want_write = 0;
		mnt_drop_write(fh->fh_export->ex_path.mnt);
	}
}

static inline __be32 fh_getattr(struct svc_fh *fh, struct kstat *stat)
{
	struct path p = {.mnt = fh->fh_export->ex_path.mnt,
			 .dentry = fh->fh_dentry};
	return nfserrno(vfs_getattr(&p, stat));
}

>>>>>>> refs/remotes/origin/master
#endif /* LINUX_NFSD_VFS_H */
