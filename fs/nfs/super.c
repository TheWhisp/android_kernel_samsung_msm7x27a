/*
 *  linux/fs/nfs/super.c
 *
 *  Copyright (C) 1992  Rick Sladkey
 *
 *  nfs superblock handling functions
 *
 *  Modularised by Alan Cox <alan@lxorguk.ukuu.org.uk>, while hacking some
 *  experimental NFS changes. Modularisation taken straight from SYS5 fs.
 *
 *  Change to nfs_read_super() to permit NFS mounts to multi-homed hosts.
 *  J.S.Peatfield@damtp.cam.ac.uk
 *
 *  Split from inode.c by David Howells <dhowells@redhat.com>
 *
 * - superblocks are indexed on server only - all inodes, dentries, etc. associated with a
 *   particular server are held in the same superblock
 * - NFS superblocks can have several effective roots to the dentry tree
 * - directory type roots are spliced into the tree when a path from one root reaches the root
 *   of another (see nfs_lookup())
 */

#include <linux/module.h>
#include <linux/init.h>

#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/sunrpc/clnt.h>
<<<<<<< HEAD
=======
#include <linux/sunrpc/addr.h>
>>>>>>> refs/remotes/origin/master
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/metrics.h>
#include <linux/sunrpc/xprtsock.h>
#include <linux/sunrpc/xprtrdma.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_mount.h>
#include <linux/nfs4_mount.h>
#include <linux/lockd/bind.h>
#include <linux/seq_file.h>
#include <linux/mount.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/mnt_namespace.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/namei.h>
#include <linux/nfs_idmap.h>
#include <linux/vfs.h>
#include <linux/inet.h>
#include <linux/in6.h>
#include <linux/slab.h>
#include <net/ipv6.h>
#include <linux/netdevice.h>
#include <linux/nfs_xdr.h>
#include <linux/magic.h>
#include <linux/parser.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <asm/system.h>
=======
#include <linux/nsproxy.h>
#include <linux/rcupdate.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/nsproxy.h>
#include <linux/rcupdate.h>

>>>>>>> refs/remotes/origin/master
#include <asm/uaccess.h>

#include "nfs4_fs.h"
#include "callback.h"
#include "delegation.h"
#include "iostat.h"
#include "internal.h"
#include "fscache.h"
<<<<<<< HEAD
#include "pnfs.h"

#define NFSDBG_FACILITY		NFSDBG_VFS

#ifdef CONFIG_NFS_V3
=======
#include "nfs4session.h"
#include "pnfs.h"
#include "nfs.h"

#define NFSDBG_FACILITY		NFSDBG_VFS
#define NFS_TEXT_DATA		1

#if IS_ENABLED(CONFIG_NFS_V3)
>>>>>>> refs/remotes/origin/master
#define NFS_DEFAULT_VERSION 3
#else
#define NFS_DEFAULT_VERSION 2
#endif

enum {
	/* Mount options that take no arguments */
	Opt_soft, Opt_hard,
	Opt_posix, Opt_noposix,
	Opt_cto, Opt_nocto,
	Opt_ac, Opt_noac,
	Opt_lock, Opt_nolock,
<<<<<<< HEAD
<<<<<<< HEAD
	Opt_v2, Opt_v3, Opt_v4,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	Opt_udp, Opt_tcp, Opt_rdma,
	Opt_acl, Opt_noacl,
	Opt_rdirplus, Opt_nordirplus,
	Opt_sharecache, Opt_nosharecache,
	Opt_resvport, Opt_noresvport,
	Opt_fscache, Opt_nofscache,
<<<<<<< HEAD
=======
	Opt_migration, Opt_nomigration,
>>>>>>> refs/remotes/origin/master

	/* Mount options that take integer arguments */
	Opt_port,
	Opt_rsize, Opt_wsize, Opt_bsize,
	Opt_timeo, Opt_retrans,
	Opt_acregmin, Opt_acregmax,
	Opt_acdirmin, Opt_acdirmax,
	Opt_actimeo,
	Opt_namelen,
	Opt_mountport,
	Opt_mountvers,
<<<<<<< HEAD
<<<<<<< HEAD
	Opt_nfsvers,
	Opt_minorversion,

	/* Mount options that take string arguments */
=======
=======
>>>>>>> refs/remotes/origin/master
	Opt_minorversion,

	/* Mount options that take string arguments */
	Opt_nfsvers,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	Opt_sec, Opt_proto, Opt_mountproto, Opt_mounthost,
	Opt_addr, Opt_mountaddr, Opt_clientaddr,
	Opt_lookupcache,
	Opt_fscache_uniq,
	Opt_local_lock,

	/* Special mount options */
	Opt_userspace, Opt_deprecated, Opt_sloppy,

	Opt_err
};

static const match_table_t nfs_mount_option_tokens = {
	{ Opt_userspace, "bg" },
	{ Opt_userspace, "fg" },
	{ Opt_userspace, "retry=%s" },

	{ Opt_sloppy, "sloppy" },

	{ Opt_soft, "soft" },
	{ Opt_hard, "hard" },
	{ Opt_deprecated, "intr" },
	{ Opt_deprecated, "nointr" },
	{ Opt_posix, "posix" },
	{ Opt_noposix, "noposix" },
	{ Opt_cto, "cto" },
	{ Opt_nocto, "nocto" },
	{ Opt_ac, "ac" },
	{ Opt_noac, "noac" },
	{ Opt_lock, "lock" },
	{ Opt_nolock, "nolock" },
<<<<<<< HEAD
<<<<<<< HEAD
	{ Opt_v2, "v2" },
	{ Opt_v3, "v3" },
	{ Opt_v4, "v4" },
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	{ Opt_udp, "udp" },
	{ Opt_tcp, "tcp" },
	{ Opt_rdma, "rdma" },
	{ Opt_acl, "acl" },
	{ Opt_noacl, "noacl" },
	{ Opt_rdirplus, "rdirplus" },
	{ Opt_nordirplus, "nordirplus" },
	{ Opt_sharecache, "sharecache" },
	{ Opt_nosharecache, "nosharecache" },
	{ Opt_resvport, "resvport" },
	{ Opt_noresvport, "noresvport" },
	{ Opt_fscache, "fsc" },
	{ Opt_nofscache, "nofsc" },
<<<<<<< HEAD
=======
	{ Opt_migration, "migration" },
	{ Opt_nomigration, "nomigration" },
>>>>>>> refs/remotes/origin/master

	{ Opt_port, "port=%s" },
	{ Opt_rsize, "rsize=%s" },
	{ Opt_wsize, "wsize=%s" },
	{ Opt_bsize, "bsize=%s" },
	{ Opt_timeo, "timeo=%s" },
	{ Opt_retrans, "retrans=%s" },
	{ Opt_acregmin, "acregmin=%s" },
	{ Opt_acregmax, "acregmax=%s" },
	{ Opt_acdirmin, "acdirmin=%s" },
	{ Opt_acdirmax, "acdirmax=%s" },
	{ Opt_actimeo, "actimeo=%s" },
	{ Opt_namelen, "namlen=%s" },
	{ Opt_mountport, "mountport=%s" },
	{ Opt_mountvers, "mountvers=%s" },
<<<<<<< HEAD
<<<<<<< HEAD
	{ Opt_nfsvers, "nfsvers=%s" },
	{ Opt_nfsvers, "vers=%s" },
	{ Opt_minorversion, "minorversion=%s" },
=======
=======
>>>>>>> refs/remotes/origin/master
	{ Opt_minorversion, "minorversion=%s" },

	{ Opt_nfsvers, "nfsvers=%s" },
	{ Opt_nfsvers, "vers=%s" },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	{ Opt_sec, "sec=%s" },
	{ Opt_proto, "proto=%s" },
	{ Opt_mountproto, "mountproto=%s" },
	{ Opt_addr, "addr=%s" },
	{ Opt_clientaddr, "clientaddr=%s" },
	{ Opt_mounthost, "mounthost=%s" },
	{ Opt_mountaddr, "mountaddr=%s" },

	{ Opt_lookupcache, "lookupcache=%s" },
	{ Opt_fscache_uniq, "fsc=%s" },
	{ Opt_local_lock, "local_lock=%s" },

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* The following needs to be listed after all other options */
	{ Opt_nfsvers, "v%s" },

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* The following needs to be listed after all other options */
	{ Opt_nfsvers, "v%s" },

>>>>>>> refs/remotes/origin/master
	{ Opt_err, NULL }
};

enum {
	Opt_xprt_udp, Opt_xprt_udp6, Opt_xprt_tcp, Opt_xprt_tcp6, Opt_xprt_rdma,

	Opt_xprt_err
};

static const match_table_t nfs_xprt_protocol_tokens = {
	{ Opt_xprt_udp, "udp" },
	{ Opt_xprt_udp6, "udp6" },
	{ Opt_xprt_tcp, "tcp" },
	{ Opt_xprt_tcp6, "tcp6" },
	{ Opt_xprt_rdma, "rdma" },

	{ Opt_xprt_err, NULL }
};

enum {
	Opt_sec_none, Opt_sec_sys,
	Opt_sec_krb5, Opt_sec_krb5i, Opt_sec_krb5p,
	Opt_sec_lkey, Opt_sec_lkeyi, Opt_sec_lkeyp,
	Opt_sec_spkm, Opt_sec_spkmi, Opt_sec_spkmp,

	Opt_sec_err
};

static const match_table_t nfs_secflavor_tokens = {
	{ Opt_sec_none, "none" },
	{ Opt_sec_none, "null" },
	{ Opt_sec_sys, "sys" },

	{ Opt_sec_krb5, "krb5" },
	{ Opt_sec_krb5i, "krb5i" },
	{ Opt_sec_krb5p, "krb5p" },

	{ Opt_sec_lkey, "lkey" },
	{ Opt_sec_lkeyi, "lkeyi" },
	{ Opt_sec_lkeyp, "lkeyp" },

	{ Opt_sec_spkm, "spkm3" },
	{ Opt_sec_spkmi, "spkm3i" },
	{ Opt_sec_spkmp, "spkm3p" },

	{ Opt_sec_err, NULL }
};

enum {
	Opt_lookupcache_all, Opt_lookupcache_positive,
	Opt_lookupcache_none,

	Opt_lookupcache_err
};

static match_table_t nfs_lookupcache_tokens = {
	{ Opt_lookupcache_all, "all" },
	{ Opt_lookupcache_positive, "pos" },
	{ Opt_lookupcache_positive, "positive" },
	{ Opt_lookupcache_none, "none" },

	{ Opt_lookupcache_err, NULL }
};

enum {
	Opt_local_lock_all, Opt_local_lock_flock, Opt_local_lock_posix,
	Opt_local_lock_none,

	Opt_local_lock_err
};

static match_table_t nfs_local_lock_tokens = {
	{ Opt_local_lock_all, "all" },
	{ Opt_local_lock_flock, "flock" },
	{ Opt_local_lock_posix, "posix" },
	{ Opt_local_lock_none, "none" },

	{ Opt_local_lock_err, NULL }
};

<<<<<<< HEAD
<<<<<<< HEAD

static void nfs_umount_begin(struct super_block *);
static int  nfs_statfs(struct dentry *, struct kstatfs *);
static int  nfs_show_options(struct seq_file *, struct vfsmount *);
static int  nfs_show_devname(struct seq_file *, struct vfsmount *);
static int  nfs_show_path(struct seq_file *, struct vfsmount *);
static int  nfs_show_stats(struct seq_file *, struct vfsmount *);
=======
enum {
	Opt_vers_2, Opt_vers_3, Opt_vers_4, Opt_vers_4_0,
	Opt_vers_4_1,
=======
enum {
	Opt_vers_2, Opt_vers_3, Opt_vers_4, Opt_vers_4_0,
	Opt_vers_4_1, Opt_vers_4_2,
>>>>>>> refs/remotes/origin/master

	Opt_vers_err
};

static match_table_t nfs_vers_tokens = {
	{ Opt_vers_2, "2" },
	{ Opt_vers_3, "3" },
	{ Opt_vers_4, "4" },
	{ Opt_vers_4_0, "4.0" },
	{ Opt_vers_4_1, "4.1" },
<<<<<<< HEAD
=======
	{ Opt_vers_4_2, "4.2" },
>>>>>>> refs/remotes/origin/master

	{ Opt_vers_err, NULL }
};

<<<<<<< HEAD
static void nfs_umount_begin(struct super_block *);
static int  nfs_statfs(struct dentry *, struct kstatfs *);
static int  nfs_show_options(struct seq_file *, struct dentry *);
static int  nfs_show_devname(struct seq_file *, struct dentry *);
static int  nfs_show_path(struct seq_file *, struct dentry *);
static int  nfs_show_stats(struct seq_file *, struct dentry *);
>>>>>>> refs/remotes/origin/cm-10.0
static struct dentry *nfs_fs_mount(struct file_system_type *,
		int, const char *, void *);
static struct dentry *nfs_xdev_mount(struct file_system_type *fs_type,
		int flags, const char *dev_name, void *raw_data);
static void nfs_put_super(struct super_block *);
static void nfs_kill_super(struct super_block *);
static int nfs_remount(struct super_block *sb, int *flags, char *raw_data);

static struct file_system_type nfs_fs_type = {
=======
static struct dentry *nfs_xdev_mount(struct file_system_type *fs_type,
		int flags, const char *dev_name, void *raw_data);

struct file_system_type nfs_fs_type = {
>>>>>>> refs/remotes/origin/master
	.owner		= THIS_MODULE,
	.name		= "nfs",
	.mount		= nfs_fs_mount,
	.kill_sb	= nfs_kill_super,
<<<<<<< HEAD
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_REVAL_DOT|FS_BINARY_MOUNTDATA,
};
=======
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_BINARY_MOUNTDATA,
};
MODULE_ALIAS_FS("nfs");
EXPORT_SYMBOL_GPL(nfs_fs_type);
>>>>>>> refs/remotes/origin/master

struct file_system_type nfs_xdev_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "nfs",
	.mount		= nfs_xdev_mount,
	.kill_sb	= nfs_kill_super,
<<<<<<< HEAD
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_REVAL_DOT|FS_BINARY_MOUNTDATA,
};

static const struct super_operations nfs_sops = {
	.alloc_inode	= nfs_alloc_inode,
	.destroy_inode	= nfs_destroy_inode,
	.write_inode	= nfs_write_inode,
=======
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_BINARY_MOUNTDATA,
};

const struct super_operations nfs_sops = {
	.alloc_inode	= nfs_alloc_inode,
	.destroy_inode	= nfs_destroy_inode,
	.write_inode	= nfs_write_inode,
	.drop_inode	= nfs_drop_inode,
>>>>>>> refs/remotes/origin/master
	.put_super	= nfs_put_super,
	.statfs		= nfs_statfs,
	.evict_inode	= nfs_evict_inode,
	.umount_begin	= nfs_umount_begin,
	.show_options	= nfs_show_options,
	.show_devname	= nfs_show_devname,
	.show_path	= nfs_show_path,
	.show_stats	= nfs_show_stats,
	.remount_fs	= nfs_remount,
};
<<<<<<< HEAD

#ifdef CONFIG_NFS_V4
static int nfs4_validate_text_mount_data(void *options,
	struct nfs_parsed_mount_data *args, const char *dev_name);
static struct dentry *nfs4_try_mount(int flags, const char *dev_name,
	struct nfs_parsed_mount_data *data);
static struct dentry *nfs4_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *raw_data);
static struct dentry *nfs4_remote_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *raw_data);
static struct dentry *nfs4_xdev_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *raw_data);
static struct dentry *nfs4_referral_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *raw_data);
static struct dentry *nfs4_remote_referral_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *raw_data);
static void nfs4_kill_super(struct super_block *sb);

static struct file_system_type nfs4_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "nfs4",
	.mount		= nfs4_mount,
	.kill_sb	= nfs4_kill_super,
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_REVAL_DOT|FS_BINARY_MOUNTDATA,
};

static struct file_system_type nfs4_remote_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "nfs4",
	.mount		= nfs4_remote_mount,
	.kill_sb	= nfs4_kill_super,
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_REVAL_DOT|FS_BINARY_MOUNTDATA,
};

struct file_system_type nfs4_xdev_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "nfs4",
	.mount		= nfs4_xdev_mount,
	.kill_sb	= nfs4_kill_super,
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_REVAL_DOT|FS_BINARY_MOUNTDATA,
};

static struct file_system_type nfs4_remote_referral_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "nfs4",
	.mount		= nfs4_remote_referral_mount,
	.kill_sb	= nfs4_kill_super,
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_REVAL_DOT|FS_BINARY_MOUNTDATA,
};

struct file_system_type nfs4_referral_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "nfs4",
	.mount		= nfs4_referral_mount,
	.kill_sb	= nfs4_kill_super,
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_REVAL_DOT|FS_BINARY_MOUNTDATA,
};

static const struct super_operations nfs4_sops = {
	.alloc_inode	= nfs_alloc_inode,
	.destroy_inode	= nfs_destroy_inode,
	.write_inode	= nfs_write_inode,
	.put_super	= nfs_put_super,
	.statfs		= nfs_statfs,
	.evict_inode	= nfs4_evict_inode,
	.umount_begin	= nfs_umount_begin,
	.show_options	= nfs_show_options,
	.show_devname	= nfs_show_devname,
	.show_path	= nfs_show_path,
	.show_stats	= nfs_show_stats,
	.remount_fs	= nfs_remount,
};
#endif

static struct shrinker acl_shrinker = {
	.shrink		= nfs_access_cache_shrinker,
=======
EXPORT_SYMBOL_GPL(nfs_sops);

#if IS_ENABLED(CONFIG_NFS_V4)
static void nfs4_validate_mount_flags(struct nfs_parsed_mount_data *);
static int nfs4_validate_mount_data(void *options,
	struct nfs_parsed_mount_data *args, const char *dev_name);

struct file_system_type nfs4_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "nfs4",
	.mount		= nfs_fs_mount,
	.kill_sb	= nfs_kill_super,
	.fs_flags	= FS_RENAME_DOES_D_MOVE|FS_BINARY_MOUNTDATA,
};
MODULE_ALIAS_FS("nfs4");
MODULE_ALIAS("nfs4");
EXPORT_SYMBOL_GPL(nfs4_fs_type);

static int __init register_nfs4_fs(void)
{
	return register_filesystem(&nfs4_fs_type);
}

static void unregister_nfs4_fs(void)
{
	unregister_filesystem(&nfs4_fs_type);
}
#else
static int __init register_nfs4_fs(void)
{
	return 0;
}

static void unregister_nfs4_fs(void)
{
}
#endif

static struct shrinker acl_shrinker = {
	.count_objects	= nfs_access_cache_count,
	.scan_objects	= nfs_access_cache_scan,
>>>>>>> refs/remotes/origin/master
	.seeks		= DEFAULT_SEEKS,
};

/*
 * Register the NFS filesystems
 */
int __init register_nfs_fs(void)
{
	int ret;

        ret = register_filesystem(&nfs_fs_type);
	if (ret < 0)
		goto error_0;

<<<<<<< HEAD
	ret = nfs_register_sysctl();
	if (ret < 0)
		goto error_1;
#ifdef CONFIG_NFS_V4
	ret = register_filesystem(&nfs4_fs_type);
	if (ret < 0)
		goto error_2;
#endif
	register_shrinker(&acl_shrinker);
	return 0;

#ifdef CONFIG_NFS_V4
error_2:
	nfs_unregister_sysctl();
#endif
=======
	ret = register_nfs4_fs();
	if (ret < 0)
		goto error_1;

	ret = nfs_register_sysctl();
	if (ret < 0)
		goto error_2;
	register_shrinker(&acl_shrinker);
	return 0;

error_2:
	unregister_nfs4_fs();
>>>>>>> refs/remotes/origin/master
error_1:
	unregister_filesystem(&nfs_fs_type);
error_0:
	return ret;
}

/*
 * Unregister the NFS filesystems
 */
void __exit unregister_nfs_fs(void)
{
	unregister_shrinker(&acl_shrinker);
<<<<<<< HEAD
#ifdef CONFIG_NFS_V4
	unregister_filesystem(&nfs4_fs_type);
#endif
	nfs_unregister_sysctl();
=======
	nfs_unregister_sysctl();
	unregister_nfs4_fs();
>>>>>>> refs/remotes/origin/master
	unregister_filesystem(&nfs_fs_type);
}

void nfs_sb_active(struct super_block *sb)
{
	struct nfs_server *server = NFS_SB(sb);

	if (atomic_inc_return(&server->active) == 1)
		atomic_inc(&sb->s_active);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_sb_active);
>>>>>>> refs/remotes/origin/master

void nfs_sb_deactive(struct super_block *sb)
{
	struct nfs_server *server = NFS_SB(sb);

	if (atomic_dec_and_test(&server->active))
		deactivate_super(sb);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_sb_deactive);
>>>>>>> refs/remotes/origin/master

/*
 * Deliver file system statistics to userspace
 */
<<<<<<< HEAD
static int nfs_statfs(struct dentry *dentry, struct kstatfs *buf)
=======
int nfs_statfs(struct dentry *dentry, struct kstatfs *buf)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_server *server = NFS_SB(dentry->d_sb);
	unsigned char blockbits;
	unsigned long blockres;
	struct nfs_fh *fh = NFS_FH(dentry->d_inode);
	struct nfs_fsstat res;
	int error = -ENOMEM;

	res.fattr = nfs_alloc_fattr();
	if (res.fattr == NULL)
		goto out_err;

	error = server->nfs_client->rpc_ops->statfs(server, fh, &res);
	if (unlikely(error == -ESTALE)) {
		struct dentry *pd_dentry;

		pd_dentry = dget_parent(dentry);
		if (pd_dentry != NULL) {
			nfs_zap_caches(pd_dentry->d_inode);
			dput(pd_dentry);
		}
	}
	nfs_free_fattr(res.fattr);
	if (error < 0)
		goto out_err;

	buf->f_type = NFS_SUPER_MAGIC;

	/*
	 * Current versions of glibc do not correctly handle the
	 * case where f_frsize != f_bsize.  Eventually we want to
	 * report the value of wtmult in this field.
	 */
	buf->f_frsize = dentry->d_sb->s_blocksize;

	/*
	 * On most *nix systems, f_blocks, f_bfree, and f_bavail
	 * are reported in units of f_frsize.  Linux hasn't had
	 * an f_frsize field in its statfs struct until recently,
	 * thus historically Linux's sys_statfs reports these
	 * fields in units of f_bsize.
	 */
	buf->f_bsize = dentry->d_sb->s_blocksize;
	blockbits = dentry->d_sb->s_blocksize_bits;
	blockres = (1 << blockbits) - 1;
	buf->f_blocks = (res.tbytes + blockres) >> blockbits;
	buf->f_bfree = (res.fbytes + blockres) >> blockbits;
	buf->f_bavail = (res.abytes + blockres) >> blockbits;

	buf->f_files = res.tfiles;
	buf->f_ffree = res.afiles;

	buf->f_namelen = server->namelen;

	return 0;

 out_err:
	dprintk("%s: statfs error = %d\n", __func__, -error);
	return error;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_statfs);
>>>>>>> refs/remotes/origin/master

/*
 * Map the security flavour number to a name
 */
static const char *nfs_pseudoflavour_to_name(rpc_authflavor_t flavour)
{
	static const struct {
		rpc_authflavor_t flavour;
		const char *str;
<<<<<<< HEAD
	} sec_flavours[] = {
=======
	} sec_flavours[NFS_AUTH_INFO_MAX_FLAVORS] = {
		/* update NFS_AUTH_INFO_MAX_FLAVORS when this list changes! */
>>>>>>> refs/remotes/origin/master
		{ RPC_AUTH_NULL, "null" },
		{ RPC_AUTH_UNIX, "sys" },
		{ RPC_AUTH_GSS_KRB5, "krb5" },
		{ RPC_AUTH_GSS_KRB5I, "krb5i" },
		{ RPC_AUTH_GSS_KRB5P, "krb5p" },
		{ RPC_AUTH_GSS_LKEY, "lkey" },
		{ RPC_AUTH_GSS_LKEYI, "lkeyi" },
		{ RPC_AUTH_GSS_LKEYP, "lkeyp" },
		{ RPC_AUTH_GSS_SPKM, "spkm" },
		{ RPC_AUTH_GSS_SPKMI, "spkmi" },
		{ RPC_AUTH_GSS_SPKMP, "spkmp" },
		{ UINT_MAX, "unknown" }
	};
	int i;

	for (i = 0; sec_flavours[i].flavour != UINT_MAX; i++) {
		if (sec_flavours[i].flavour == flavour)
			break;
	}
	return sec_flavours[i].str;
}

static void nfs_show_mountd_netid(struct seq_file *m, struct nfs_server *nfss,
				  int showdefaults)
{
	struct sockaddr *sap = (struct sockaddr *) &nfss->mountd_address;

	seq_printf(m, ",mountproto=");
	switch (sap->sa_family) {
	case AF_INET:
		switch (nfss->mountd_protocol) {
		case IPPROTO_UDP:
			seq_printf(m, RPCBIND_NETID_UDP);
			break;
		case IPPROTO_TCP:
			seq_printf(m, RPCBIND_NETID_TCP);
			break;
		default:
			if (showdefaults)
				seq_printf(m, "auto");
		}
		break;
	case AF_INET6:
		switch (nfss->mountd_protocol) {
		case IPPROTO_UDP:
			seq_printf(m, RPCBIND_NETID_UDP6);
			break;
		case IPPROTO_TCP:
			seq_printf(m, RPCBIND_NETID_TCP6);
			break;
		default:
			if (showdefaults)
				seq_printf(m, "auto");
		}
		break;
	default:
		if (showdefaults)
			seq_printf(m, "auto");
	}
}

static void nfs_show_mountd_options(struct seq_file *m, struct nfs_server *nfss,
				    int showdefaults)
{
	struct sockaddr *sap = (struct sockaddr *)&nfss->mountd_address;

	if (nfss->flags & NFS_MOUNT_LEGACY_INTERFACE)
		return;

	switch (sap->sa_family) {
	case AF_INET: {
		struct sockaddr_in *sin = (struct sockaddr_in *)sap;
		seq_printf(m, ",mountaddr=%pI4", &sin->sin_addr.s_addr);
		break;
	}
	case AF_INET6: {
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sap;
		seq_printf(m, ",mountaddr=%pI6c", &sin6->sin6_addr);
		break;
	}
	default:
		if (showdefaults)
			seq_printf(m, ",mountaddr=unspecified");
	}

	if (nfss->mountd_version || showdefaults)
		seq_printf(m, ",mountvers=%u", nfss->mountd_version);
	if ((nfss->mountd_port &&
		nfss->mountd_port != (unsigned short)NFS_UNSPEC_PORT) ||
		showdefaults)
		seq_printf(m, ",mountport=%u", nfss->mountd_port);

	nfs_show_mountd_netid(m, nfss, showdefaults);
}

<<<<<<< HEAD
#ifdef CONFIG_NFS_V4
=======
#if IS_ENABLED(CONFIG_NFS_V4)
>>>>>>> refs/remotes/origin/master
static void nfs_show_nfsv4_options(struct seq_file *m, struct nfs_server *nfss,
				    int showdefaults)
{
	struct nfs_client *clp = nfss->nfs_client;

	seq_printf(m, ",clientaddr=%s", clp->cl_ipaddr);
<<<<<<< HEAD
<<<<<<< HEAD
	seq_printf(m, ",minorversion=%u", clp->cl_minorversion);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
#else
static void nfs_show_nfsv4_options(struct seq_file *m, struct nfs_server *nfss,
				    int showdefaults)
{
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void nfs_show_nfs_version(struct seq_file *m,
		unsigned int version,
		unsigned int minorversion)
{
	seq_printf(m, ",vers=%u", version);
	if (version == 4)
		seq_printf(m, ".%u", minorversion);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Describe the mount options in force on this server representation
 */
static void nfs_show_mount_options(struct seq_file *m, struct nfs_server *nfss,
				   int showdefaults)
{
	static const struct proc_nfs_info {
		int flag;
		const char *str;
		const char *nostr;
	} nfs_info[] = {
		{ NFS_MOUNT_SOFT, ",soft", ",hard" },
		{ NFS_MOUNT_POSIX, ",posix", "" },
		{ NFS_MOUNT_NOCTO, ",nocto", "" },
		{ NFS_MOUNT_NOAC, ",noac", "" },
		{ NFS_MOUNT_NONLM, ",nolock", "" },
		{ NFS_MOUNT_NOACL, ",noacl", "" },
		{ NFS_MOUNT_NORDIRPLUS, ",nordirplus", "" },
		{ NFS_MOUNT_UNSHARED, ",nosharecache", "" },
		{ NFS_MOUNT_NORESVPORT, ",noresvport", "" },
		{ 0, NULL, NULL }
	};
	const struct proc_nfs_info *nfs_infop;
	struct nfs_client *clp = nfss->nfs_client;
	u32 version = clp->rpc_ops->version;
	int local_flock, local_fcntl;

<<<<<<< HEAD
<<<<<<< HEAD
	seq_printf(m, ",vers=%u", version);
=======
	nfs_show_nfs_version(m, version, clp->cl_minorversion);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	nfs_show_nfs_version(m, version, clp->cl_minorversion);
>>>>>>> refs/remotes/origin/master
	seq_printf(m, ",rsize=%u", nfss->rsize);
	seq_printf(m, ",wsize=%u", nfss->wsize);
	if (nfss->bsize != 0)
		seq_printf(m, ",bsize=%u", nfss->bsize);
	seq_printf(m, ",namlen=%u", nfss->namelen);
	if (nfss->acregmin != NFS_DEF_ACREGMIN*HZ || showdefaults)
		seq_printf(m, ",acregmin=%u", nfss->acregmin/HZ);
	if (nfss->acregmax != NFS_DEF_ACREGMAX*HZ || showdefaults)
		seq_printf(m, ",acregmax=%u", nfss->acregmax/HZ);
	if (nfss->acdirmin != NFS_DEF_ACDIRMIN*HZ || showdefaults)
		seq_printf(m, ",acdirmin=%u", nfss->acdirmin/HZ);
	if (nfss->acdirmax != NFS_DEF_ACDIRMAX*HZ || showdefaults)
		seq_printf(m, ",acdirmax=%u", nfss->acdirmax/HZ);
	for (nfs_infop = nfs_info; nfs_infop->flag; nfs_infop++) {
		if (nfss->flags & nfs_infop->flag)
			seq_puts(m, nfs_infop->str);
		else
			seq_puts(m, nfs_infop->nostr);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	seq_printf(m, ",proto=%s",
		   rpc_peeraddr2str(nfss->client, RPC_DISPLAY_NETID));
=======
=======
>>>>>>> refs/remotes/origin/master
	rcu_read_lock();
	seq_printf(m, ",proto=%s",
		   rpc_peeraddr2str(nfss->client, RPC_DISPLAY_NETID));
	rcu_read_unlock();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (version == 4) {
		if (nfss->port != NFS_PORT)
			seq_printf(m, ",port=%u", nfss->port);
	} else
		if (nfss->port)
			seq_printf(m, ",port=%u", nfss->port);

	seq_printf(m, ",timeo=%lu", 10U * nfss->client->cl_timeout->to_initval / HZ);
	seq_printf(m, ",retrans=%u", nfss->client->cl_timeout->to_retries);
	seq_printf(m, ",sec=%s", nfs_pseudoflavour_to_name(nfss->client->cl_auth->au_flavor));

	if (version != 4)
		nfs_show_mountd_options(m, nfss, showdefaults);
	else
		nfs_show_nfsv4_options(m, nfss, showdefaults);

	if (nfss->options & NFS_OPTION_FSCACHE)
		seq_printf(m, ",fsc");

<<<<<<< HEAD
=======
	if (nfss->options & NFS_OPTION_MIGRATION)
		seq_printf(m, ",migration");

>>>>>>> refs/remotes/origin/master
	if (nfss->flags & NFS_MOUNT_LOOKUP_CACHE_NONEG) {
		if (nfss->flags & NFS_MOUNT_LOOKUP_CACHE_NONE)
			seq_printf(m, ",lookupcache=none");
		else
			seq_printf(m, ",lookupcache=pos");
	}

	local_flock = nfss->flags & NFS_MOUNT_LOCAL_FLOCK;
	local_fcntl = nfss->flags & NFS_MOUNT_LOCAL_FCNTL;

	if (!local_flock && !local_fcntl)
		seq_printf(m, ",local_lock=none");
	else if (local_flock && local_fcntl)
		seq_printf(m, ",local_lock=all");
	else if (local_flock)
		seq_printf(m, ",local_lock=flock");
	else
		seq_printf(m, ",local_lock=posix");
}

/*
 * Describe the mount options on this VFS mountpoint
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int nfs_show_options(struct seq_file *m, struct vfsmount *mnt)
{
	struct nfs_server *nfss = NFS_SB(mnt->mnt_sb);

	nfs_show_mount_options(m, nfss, 0);

	seq_printf(m, ",addr=%s",
			rpc_peeraddr2str(nfss->nfs_client->cl_rpcclient,
							RPC_DISPLAY_ADDR));

	return 0;
}
#ifdef CONFIG_NFS_V4_1
void show_sessions(struct seq_file *m, struct nfs_server *server)
=======
static int nfs_show_options(struct seq_file *m, struct dentry *root)
=======
int nfs_show_options(struct seq_file *m, struct dentry *root)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_server *nfss = NFS_SB(root->d_sb);

	nfs_show_mount_options(m, nfss, 0);

	rcu_read_lock();
	seq_printf(m, ",addr=%s",
			rpc_peeraddr2str(nfss->nfs_client->cl_rpcclient,
							RPC_DISPLAY_ADDR));
	rcu_read_unlock();

	return 0;
}
<<<<<<< HEAD

#ifdef CONFIG_NFS_V4
#ifdef CONFIG_NFS_V4_1
static void show_sessions(struct seq_file *m, struct nfs_server *server)
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL_GPL(nfs_show_options);

#if IS_ENABLED(CONFIG_NFS_V4)
#ifdef CONFIG_NFS_V4_1
static void show_sessions(struct seq_file *m, struct nfs_server *server)
>>>>>>> refs/remotes/origin/master
{
	if (nfs4_has_session(server->nfs_client))
		seq_printf(m, ",sessions");
}
#else
<<<<<<< HEAD
<<<<<<< HEAD
void show_sessions(struct seq_file *m, struct nfs_server *server) {}
#endif

#ifdef CONFIG_NFS_V4_1
void show_pnfs(struct seq_file *m, struct nfs_server *server)
=======
=======
>>>>>>> refs/remotes/origin/master
static void show_sessions(struct seq_file *m, struct nfs_server *server) {}
#endif
#endif

#ifdef CONFIG_NFS_V4_1
static void show_pnfs(struct seq_file *m, struct nfs_server *server)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	seq_printf(m, ",pnfs=");
	if (server->pnfs_curr_ld)
		seq_printf(m, "%s", server->pnfs_curr_ld->name);
	else
		seq_printf(m, "not configured");
}
<<<<<<< HEAD
<<<<<<< HEAD
#else  /* CONFIG_NFS_V4_1 */
void show_pnfs(struct seq_file *m, struct nfs_server *server) {}
#endif /* CONFIG_NFS_V4_1 */

static int nfs_show_devname(struct seq_file *m, struct vfsmount *mnt)
=======

static void show_implementation_id(struct seq_file *m, struct nfs_server *nfss)
{
	if (nfss->nfs_client && nfss->nfs_client->impl_id) {
		struct nfs41_impl_id *impl_id = nfss->nfs_client->impl_id;
=======

static void show_implementation_id(struct seq_file *m, struct nfs_server *nfss)
{
	if (nfss->nfs_client && nfss->nfs_client->cl_implid) {
		struct nfs41_impl_id *impl_id = nfss->nfs_client->cl_implid;
>>>>>>> refs/remotes/origin/master
		seq_printf(m, "\n\timpl_id:\tname='%s',domain='%s',"
			   "date='%llu,%u'",
			   impl_id->name, impl_id->domain,
			   impl_id->date.seconds, impl_id->date.nseconds);
	}
}
#else
<<<<<<< HEAD
#ifdef CONFIG_NFS_V4
=======
#if IS_ENABLED(CONFIG_NFS_V4)
>>>>>>> refs/remotes/origin/master
static void show_pnfs(struct seq_file *m, struct nfs_server *server)
{
}
#endif
static void show_implementation_id(struct seq_file *m, struct nfs_server *nfss)
{
}
#endif

<<<<<<< HEAD
static int nfs_show_devname(struct seq_file *m, struct dentry *root)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int nfs_show_devname(struct seq_file *m, struct dentry *root)
>>>>>>> refs/remotes/origin/master
{
	char *page = (char *) __get_free_page(GFP_KERNEL);
	char *devname, *dummy;
	int err = 0;
	if (!page)
		return -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
	devname = nfs_path(&dummy, mnt->mnt_root, page, PAGE_SIZE, 0);
=======
	devname = nfs_path(&dummy, root, page, PAGE_SIZE, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	devname = nfs_path(&dummy, root, page, PAGE_SIZE, 0);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(devname))
		err = PTR_ERR(devname);
	else
		seq_escape(m, devname, " \t\n\\");
	free_page((unsigned long)page);
	return err;
}
<<<<<<< HEAD

<<<<<<< HEAD
static int nfs_show_path(struct seq_file *m, struct vfsmount *mnt)
=======
static int nfs_show_path(struct seq_file *m, struct dentry *dentry)
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL_GPL(nfs_show_devname);

int nfs_show_path(struct seq_file *m, struct dentry *dentry)
>>>>>>> refs/remotes/origin/master
{
	seq_puts(m, "/");
	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_show_path);
>>>>>>> refs/remotes/origin/master

/*
 * Present statistical information for this VFS mountpoint
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int nfs_show_stats(struct seq_file *m, struct vfsmount *mnt)
{
	int i, cpu;
	struct nfs_server *nfss = NFS_SB(mnt->mnt_sb);
=======
static int nfs_show_stats(struct seq_file *m, struct dentry *root)
{
	int i, cpu;
	struct nfs_server *nfss = NFS_SB(root->d_sb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int nfs_show_stats(struct seq_file *m, struct dentry *root)
{
	int i, cpu;
	struct nfs_server *nfss = NFS_SB(root->d_sb);
>>>>>>> refs/remotes/origin/master
	struct rpc_auth *auth = nfss->client->cl_auth;
	struct nfs_iostats totals = { };

	seq_printf(m, "statvers=%s", NFS_IOSTAT_VERS);

	/*
	 * Display all mount option settings
	 */
	seq_printf(m, "\n\topts:\t");
<<<<<<< HEAD
<<<<<<< HEAD
	seq_puts(m, mnt->mnt_sb->s_flags & MS_RDONLY ? "ro" : "rw");
	seq_puts(m, mnt->mnt_sb->s_flags & MS_SYNCHRONOUS ? ",sync" : "");
	seq_puts(m, mnt->mnt_sb->s_flags & MS_NOATIME ? ",noatime" : "");
	seq_puts(m, mnt->mnt_sb->s_flags & MS_NODIRATIME ? ",nodiratime" : "");
=======
=======
>>>>>>> refs/remotes/origin/master
	seq_puts(m, root->d_sb->s_flags & MS_RDONLY ? "ro" : "rw");
	seq_puts(m, root->d_sb->s_flags & MS_SYNCHRONOUS ? ",sync" : "");
	seq_puts(m, root->d_sb->s_flags & MS_NOATIME ? ",noatime" : "");
	seq_puts(m, root->d_sb->s_flags & MS_NODIRATIME ? ",nodiratime" : "");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	nfs_show_mount_options(m, nfss, 1);

	seq_printf(m, "\n\tage:\t%lu", (jiffies - nfss->mount_time) / HZ);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	show_implementation_id(m, nfss);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	show_implementation_id(m, nfss);

>>>>>>> refs/remotes/origin/master
	seq_printf(m, "\n\tcaps:\t");
	seq_printf(m, "caps=0x%x", nfss->caps);
	seq_printf(m, ",wtmult=%u", nfss->wtmult);
	seq_printf(m, ",dtsize=%u", nfss->dtsize);
	seq_printf(m, ",bsize=%u", nfss->bsize);
	seq_printf(m, ",namlen=%u", nfss->namelen);

<<<<<<< HEAD
#ifdef CONFIG_NFS_V4
=======
#if IS_ENABLED(CONFIG_NFS_V4)
>>>>>>> refs/remotes/origin/master
	if (nfss->nfs_client->rpc_ops->version == 4) {
		seq_printf(m, "\n\tnfsv4:\t");
		seq_printf(m, "bm0=0x%x", nfss->attr_bitmask[0]);
		seq_printf(m, ",bm1=0x%x", nfss->attr_bitmask[1]);
<<<<<<< HEAD
=======
		seq_printf(m, ",bm2=0x%x", nfss->attr_bitmask[2]);
>>>>>>> refs/remotes/origin/master
		seq_printf(m, ",acl=0x%x", nfss->acl_bitmask);
		show_sessions(m, nfss);
		show_pnfs(m, nfss);
	}
#endif

	/*
	 * Display security flavor in effect for this mount
	 */
	seq_printf(m, "\n\tsec:\tflavor=%u", auth->au_ops->au_flavor);
	if (auth->au_flavor)
		seq_printf(m, ",pseudoflavor=%u", auth->au_flavor);

	/*
	 * Display superblock I/O counters
	 */
	for_each_possible_cpu(cpu) {
		struct nfs_iostats *stats;

		preempt_disable();
		stats = per_cpu_ptr(nfss->io_stats, cpu);

		for (i = 0; i < __NFSIOS_COUNTSMAX; i++)
			totals.events[i] += stats->events[i];
		for (i = 0; i < __NFSIOS_BYTESMAX; i++)
			totals.bytes[i] += stats->bytes[i];
#ifdef CONFIG_NFS_FSCACHE
		for (i = 0; i < __NFSIOS_FSCACHEMAX; i++)
			totals.fscache[i] += stats->fscache[i];
#endif

		preempt_enable();
	}

	seq_printf(m, "\n\tevents:\t");
	for (i = 0; i < __NFSIOS_COUNTSMAX; i++)
		seq_printf(m, "%lu ", totals.events[i]);
	seq_printf(m, "\n\tbytes:\t");
	for (i = 0; i < __NFSIOS_BYTESMAX; i++)
		seq_printf(m, "%Lu ", totals.bytes[i]);
#ifdef CONFIG_NFS_FSCACHE
	if (nfss->options & NFS_OPTION_FSCACHE) {
		seq_printf(m, "\n\tfsc:\t");
		for (i = 0; i < __NFSIOS_FSCACHEMAX; i++)
			seq_printf(m, "%Lu ", totals.bytes[i]);
	}
#endif
	seq_printf(m, "\n");

	rpc_print_iostats(m, nfss->client);

	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_show_stats);
>>>>>>> refs/remotes/origin/master

/*
 * Begin unmount by attempting to remove all automounted mountpoints we added
 * in response to xdev traversals and referrals
 */
<<<<<<< HEAD
static void nfs_umount_begin(struct super_block *sb)
=======
void nfs_umount_begin(struct super_block *sb)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_server *server;
	struct rpc_clnt *rpc;

	server = NFS_SB(sb);
	/* -EIO all pending I/O */
	rpc = server->client_acl;
	if (!IS_ERR(rpc))
		rpc_killall_tasks(rpc);
	rpc = server->client;
	if (!IS_ERR(rpc))
		rpc_killall_tasks(rpc);
}
<<<<<<< HEAD

static struct nfs_parsed_mount_data *nfs_alloc_parsed_mount_data(unsigned int version)
=======
EXPORT_SYMBOL_GPL(nfs_umount_begin);

static struct nfs_parsed_mount_data *nfs_alloc_parsed_mount_data(void)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_parsed_mount_data *data;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (data) {
		data->acregmin		= NFS_DEF_ACREGMIN;
		data->acregmax		= NFS_DEF_ACREGMAX;
		data->acdirmin		= NFS_DEF_ACDIRMIN;
		data->acdirmax		= NFS_DEF_ACDIRMAX;
		data->mount_server.port	= NFS_UNSPEC_PORT;
		data->nfs_server.port	= NFS_UNSPEC_PORT;
		data->nfs_server.protocol = XPRT_TRANSPORT_TCP;
<<<<<<< HEAD
		data->auth_flavors[0]	= RPC_AUTH_UNIX;
		data->auth_flavor_len	= 1;
		data->version		= version;
		data->minorversion	= 0;
<<<<<<< HEAD
=======
		data->net		= current->nsproxy->net_ns;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		data->selected_flavor	= RPC_AUTH_MAXFLAVOR;
		data->minorversion	= 0;
		data->need_mount	= true;
		data->net		= current->nsproxy->net_ns;
>>>>>>> refs/remotes/origin/master
		security_init_mnt_opts(&data->lsm_opts);
	}
	return data;
}

static void nfs_free_parsed_mount_data(struct nfs_parsed_mount_data *data)
{
	if (data) {
		kfree(data->client_address);
		kfree(data->mount_server.hostname);
		kfree(data->nfs_server.export_path);
		kfree(data->nfs_server.hostname);
		kfree(data->fscache_uniq);
		security_free_mnt_opts(&data->lsm_opts);
		kfree(data);
	}
}

/*
 * Sanity-check a server address provided by the mount command.
 *
 * Address family must be initialized, and address must not be
 * the ANY address for that family.
 */
static int nfs_verify_server_address(struct sockaddr *addr)
{
	switch (addr->sa_family) {
	case AF_INET: {
		struct sockaddr_in *sa = (struct sockaddr_in *)addr;
		return sa->sin_addr.s_addr != htonl(INADDR_ANY);
	}
	case AF_INET6: {
		struct in6_addr *sa = &((struct sockaddr_in6 *)addr)->sin6_addr;
		return !ipv6_addr_any(sa);
	}
	}

	dfprintk(MOUNT, "NFS: Invalid IP address specified\n");
	return 0;
}

/*
 * Select between a default port value and a user-specified port value.
 * If a zero value is set, then autobind will be used.
 */
static void nfs_set_port(struct sockaddr *sap, int *port,
				 const unsigned short default_port)
{
	if (*port == NFS_UNSPEC_PORT)
		*port = default_port;

	rpc_set_port(sap, *port);
}

/*
 * Sanity check the NFS transport protocol.
 *
 */
static void nfs_validate_transport_protocol(struct nfs_parsed_mount_data *mnt)
{
	switch (mnt->nfs_server.protocol) {
	case XPRT_TRANSPORT_UDP:
	case XPRT_TRANSPORT_TCP:
	case XPRT_TRANSPORT_RDMA:
		break;
	default:
		mnt->nfs_server.protocol = XPRT_TRANSPORT_TCP;
	}
}

/*
 * For text based NFSv2/v3 mounts, the mount protocol transport default
 * settings should depend upon the specified NFS transport.
 */
static void nfs_set_mount_transport_protocol(struct nfs_parsed_mount_data *mnt)
{
	nfs_validate_transport_protocol(mnt);

	if (mnt->mount_server.protocol == XPRT_TRANSPORT_UDP ||
	    mnt->mount_server.protocol == XPRT_TRANSPORT_TCP)
			return;
	switch (mnt->nfs_server.protocol) {
	case XPRT_TRANSPORT_UDP:
		mnt->mount_server.protocol = XPRT_TRANSPORT_UDP;
		break;
	case XPRT_TRANSPORT_TCP:
	case XPRT_TRANSPORT_RDMA:
		mnt->mount_server.protocol = XPRT_TRANSPORT_TCP;
	}
}

/*
<<<<<<< HEAD
=======
 * Add 'flavor' to 'auth_info' if not already present.
 * Returns true if 'flavor' ends up in the list, false otherwise
 */
static bool nfs_auth_info_add(struct nfs_auth_info *auth_info,
			      rpc_authflavor_t flavor)
{
	unsigned int i;
	unsigned int max_flavor_len = (sizeof(auth_info->flavors) /
				       sizeof(auth_info->flavors[0]));

	/* make sure this flavor isn't already in the list */
	for (i = 0; i < auth_info->flavor_len; i++) {
		if (flavor == auth_info->flavors[i])
			return true;
	}

	if (auth_info->flavor_len + 1 >= max_flavor_len) {
		dfprintk(MOUNT, "NFS: too many sec= flavors\n");
		return false;
	}

	auth_info->flavors[auth_info->flavor_len++] = flavor;
	return true;
}

/*
 * Return true if 'match' is in auth_info or auth_info is empty.
 * Return false otherwise.
 */
bool nfs_auth_info_match(const struct nfs_auth_info *auth_info,
			 rpc_authflavor_t match)
{
	int i;

	if (!auth_info->flavor_len)
		return true;

	for (i = 0; i < auth_info->flavor_len; i++) {
		if (auth_info->flavors[i] == match)
			return true;
	}
	return false;
}
EXPORT_SYMBOL_GPL(nfs_auth_info_match);

/*
>>>>>>> refs/remotes/origin/master
 * Parse the value of the 'sec=' option.
 */
static int nfs_parse_security_flavors(char *value,
				      struct nfs_parsed_mount_data *mnt)
{
	substring_t args[MAX_OPT_ARGS];
<<<<<<< HEAD

	dfprintk(MOUNT, "NFS: parsing sec=%s option\n", value);

	switch (match_token(value, nfs_secflavor_tokens, args)) {
	case Opt_sec_none:
		mnt->auth_flavors[0] = RPC_AUTH_NULL;
		break;
	case Opt_sec_sys:
		mnt->auth_flavors[0] = RPC_AUTH_UNIX;
		break;
	case Opt_sec_krb5:
		mnt->auth_flavors[0] = RPC_AUTH_GSS_KRB5;
		break;
	case Opt_sec_krb5i:
		mnt->auth_flavors[0] = RPC_AUTH_GSS_KRB5I;
		break;
	case Opt_sec_krb5p:
		mnt->auth_flavors[0] = RPC_AUTH_GSS_KRB5P;
		break;
	case Opt_sec_lkey:
		mnt->auth_flavors[0] = RPC_AUTH_GSS_LKEY;
		break;
	case Opt_sec_lkeyi:
		mnt->auth_flavors[0] = RPC_AUTH_GSS_LKEYI;
		break;
	case Opt_sec_lkeyp:
		mnt->auth_flavors[0] = RPC_AUTH_GSS_LKEYP;
		break;
	case Opt_sec_spkm:
		mnt->auth_flavors[0] = RPC_AUTH_GSS_SPKM;
		break;
	case Opt_sec_spkmi:
		mnt->auth_flavors[0] = RPC_AUTH_GSS_SPKMI;
		break;
	case Opt_sec_spkmp:
		mnt->auth_flavors[0] = RPC_AUTH_GSS_SPKMP;
		break;
	default:
		return 0;
	}

	mnt->flags |= NFS_MOUNT_SECFLAVOUR;
	mnt->auth_flavor_len = 1;
	return 1;
}

<<<<<<< HEAD
=======
=======
	rpc_authflavor_t pseudoflavor;
	char *p;

	dfprintk(MOUNT, "NFS: parsing sec=%s option\n", value);

	while ((p = strsep(&value, ":")) != NULL) {
		switch (match_token(p, nfs_secflavor_tokens, args)) {
		case Opt_sec_none:
			pseudoflavor = RPC_AUTH_NULL;
			break;
		case Opt_sec_sys:
			pseudoflavor = RPC_AUTH_UNIX;
			break;
		case Opt_sec_krb5:
			pseudoflavor = RPC_AUTH_GSS_KRB5;
			break;
		case Opt_sec_krb5i:
			pseudoflavor = RPC_AUTH_GSS_KRB5I;
			break;
		case Opt_sec_krb5p:
			pseudoflavor = RPC_AUTH_GSS_KRB5P;
			break;
		case Opt_sec_lkey:
			pseudoflavor = RPC_AUTH_GSS_LKEY;
			break;
		case Opt_sec_lkeyi:
			pseudoflavor = RPC_AUTH_GSS_LKEYI;
			break;
		case Opt_sec_lkeyp:
			pseudoflavor = RPC_AUTH_GSS_LKEYP;
			break;
		case Opt_sec_spkm:
			pseudoflavor = RPC_AUTH_GSS_SPKM;
			break;
		case Opt_sec_spkmi:
			pseudoflavor = RPC_AUTH_GSS_SPKMI;
			break;
		case Opt_sec_spkmp:
			pseudoflavor = RPC_AUTH_GSS_SPKMP;
			break;
		default:
			dfprintk(MOUNT,
				 "NFS: sec= option '%s' not recognized\n", p);
			return 0;
		}

		if (!nfs_auth_info_add(&mnt->auth_info, pseudoflavor))
			return 0;
	}

	return 1;
}

>>>>>>> refs/remotes/origin/master
static int nfs_parse_version_string(char *string,
		struct nfs_parsed_mount_data *mnt,
		substring_t *args)
{
	mnt->flags &= ~NFS_MOUNT_VER3;
	switch (match_token(string, nfs_vers_tokens, args)) {
	case Opt_vers_2:
		mnt->version = 2;
		break;
	case Opt_vers_3:
		mnt->flags |= NFS_MOUNT_VER3;
		mnt->version = 3;
		break;
	case Opt_vers_4:
		/* Backward compatibility option. In future,
		 * the mount program should always supply
		 * a NFSv4 minor version number.
		 */
		mnt->version = 4;
		break;
	case Opt_vers_4_0:
		mnt->version = 4;
		mnt->minorversion = 0;
		break;
	case Opt_vers_4_1:
		mnt->version = 4;
		mnt->minorversion = 1;
		break;
<<<<<<< HEAD
=======
	case Opt_vers_4_2:
		mnt->version = 4;
		mnt->minorversion = 2;
		break;
>>>>>>> refs/remotes/origin/master
	default:
		return 0;
	}
	return 1;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int nfs_get_option_str(substring_t args[], char **option)
{
	kfree(*option);
	*option = match_strdup(args);
	return !*option;
}

static int nfs_get_option_ul(substring_t args[], unsigned long *option)
{
	int rc;
	char *string;

	string = match_strdup(args);
	if (string == NULL)
		return -ENOMEM;
<<<<<<< HEAD
	rc = strict_strtoul(string, 10, option);
=======
	rc = kstrtoul(string, 10, option);
>>>>>>> refs/remotes/origin/master
	kfree(string);

	return rc;
}

/*
 * Error-check and convert a string of mount options from user space into
 * a data structure.  The whole mount string is processed; bad options are
 * skipped as they are encountered.  If there were no errors, return 1;
 * otherwise return 0 (zero).
 */
static int nfs_parse_mount_options(char *raw,
				   struct nfs_parsed_mount_data *mnt)
{
	char *p, *string, *secdata;
	int rc, sloppy = 0, invalid_option = 0;
	unsigned short protofamily = AF_UNSPEC;
	unsigned short mountfamily = AF_UNSPEC;

	if (!raw) {
		dfprintk(MOUNT, "NFS: mount options string was NULL.\n");
		return 1;
	}
	dfprintk(MOUNT, "NFS: nfs mount opts='%s'\n", raw);

	secdata = alloc_secdata();
	if (!secdata)
		goto out_nomem;

	rc = security_sb_copy_data(raw, secdata);
	if (rc)
		goto out_security_failure;

	rc = security_sb_parse_opts_str(secdata, &mnt->lsm_opts);
	if (rc)
		goto out_security_failure;

	free_secdata(secdata);

	while ((p = strsep(&raw, ",")) != NULL) {
		substring_t args[MAX_OPT_ARGS];
		unsigned long option;
		int token;

		if (!*p)
			continue;

		dfprintk(MOUNT, "NFS:   parsing nfs mount option '%s'\n", p);

		token = match_token(p, nfs_mount_option_tokens, args);
		switch (token) {

		/*
		 * boolean options:  foo/nofoo
		 */
		case Opt_soft:
			mnt->flags |= NFS_MOUNT_SOFT;
			break;
		case Opt_hard:
			mnt->flags &= ~NFS_MOUNT_SOFT;
			break;
		case Opt_posix:
			mnt->flags |= NFS_MOUNT_POSIX;
			break;
		case Opt_noposix:
			mnt->flags &= ~NFS_MOUNT_POSIX;
			break;
		case Opt_cto:
			mnt->flags &= ~NFS_MOUNT_NOCTO;
			break;
		case Opt_nocto:
			mnt->flags |= NFS_MOUNT_NOCTO;
			break;
		case Opt_ac:
			mnt->flags &= ~NFS_MOUNT_NOAC;
			break;
		case Opt_noac:
			mnt->flags |= NFS_MOUNT_NOAC;
			break;
		case Opt_lock:
			mnt->flags &= ~NFS_MOUNT_NONLM;
			mnt->flags &= ~(NFS_MOUNT_LOCAL_FLOCK |
					NFS_MOUNT_LOCAL_FCNTL);
			break;
		case Opt_nolock:
			mnt->flags |= NFS_MOUNT_NONLM;
			mnt->flags |= (NFS_MOUNT_LOCAL_FLOCK |
				       NFS_MOUNT_LOCAL_FCNTL);
			break;
<<<<<<< HEAD
<<<<<<< HEAD
		case Opt_v2:
			mnt->flags &= ~NFS_MOUNT_VER3;
			mnt->version = 2;
			break;
		case Opt_v3:
			mnt->flags |= NFS_MOUNT_VER3;
			mnt->version = 3;
			break;
		case Opt_v4:
			mnt->flags &= ~NFS_MOUNT_VER3;
			mnt->version = 4;
			break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case Opt_udp:
			mnt->flags &= ~NFS_MOUNT_TCP;
			mnt->nfs_server.protocol = XPRT_TRANSPORT_UDP;
			break;
		case Opt_tcp:
			mnt->flags |= NFS_MOUNT_TCP;
			mnt->nfs_server.protocol = XPRT_TRANSPORT_TCP;
			break;
		case Opt_rdma:
			mnt->flags |= NFS_MOUNT_TCP; /* for side protocols */
			mnt->nfs_server.protocol = XPRT_TRANSPORT_RDMA;
			xprt_load_transport(p);
			break;
		case Opt_acl:
			mnt->flags &= ~NFS_MOUNT_NOACL;
			break;
		case Opt_noacl:
			mnt->flags |= NFS_MOUNT_NOACL;
			break;
		case Opt_rdirplus:
			mnt->flags &= ~NFS_MOUNT_NORDIRPLUS;
			break;
		case Opt_nordirplus:
			mnt->flags |= NFS_MOUNT_NORDIRPLUS;
			break;
		case Opt_sharecache:
			mnt->flags &= ~NFS_MOUNT_UNSHARED;
			break;
		case Opt_nosharecache:
			mnt->flags |= NFS_MOUNT_UNSHARED;
			break;
		case Opt_resvport:
			mnt->flags &= ~NFS_MOUNT_NORESVPORT;
			break;
		case Opt_noresvport:
			mnt->flags |= NFS_MOUNT_NORESVPORT;
			break;
		case Opt_fscache:
			mnt->options |= NFS_OPTION_FSCACHE;
			kfree(mnt->fscache_uniq);
			mnt->fscache_uniq = NULL;
			break;
		case Opt_nofscache:
			mnt->options &= ~NFS_OPTION_FSCACHE;
			kfree(mnt->fscache_uniq);
			mnt->fscache_uniq = NULL;
			break;
<<<<<<< HEAD
=======
		case Opt_migration:
			mnt->options |= NFS_OPTION_MIGRATION;
			break;
		case Opt_nomigration:
			mnt->options &= NFS_OPTION_MIGRATION;
			break;
>>>>>>> refs/remotes/origin/master

		/*
		 * options that take numeric values
		 */
		case Opt_port:
			if (nfs_get_option_ul(args, &option) ||
			    option > USHRT_MAX)
				goto out_invalid_value;
			mnt->nfs_server.port = option;
			break;
		case Opt_rsize:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			mnt->rsize = option;
			break;
		case Opt_wsize:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			mnt->wsize = option;
			break;
		case Opt_bsize:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			mnt->bsize = option;
			break;
		case Opt_timeo:
			if (nfs_get_option_ul(args, &option) || option == 0)
				goto out_invalid_value;
			mnt->timeo = option;
			break;
		case Opt_retrans:
			if (nfs_get_option_ul(args, &option) || option == 0)
				goto out_invalid_value;
			mnt->retrans = option;
			break;
		case Opt_acregmin:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			mnt->acregmin = option;
			break;
		case Opt_acregmax:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			mnt->acregmax = option;
			break;
		case Opt_acdirmin:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			mnt->acdirmin = option;
			break;
		case Opt_acdirmax:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			mnt->acdirmax = option;
			break;
		case Opt_actimeo:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			mnt->acregmin = mnt->acregmax =
			mnt->acdirmin = mnt->acdirmax = option;
			break;
		case Opt_namelen:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			mnt->namlen = option;
			break;
		case Opt_mountport:
			if (nfs_get_option_ul(args, &option) ||
			    option > USHRT_MAX)
				goto out_invalid_value;
			mnt->mount_server.port = option;
			break;
		case Opt_mountvers:
			if (nfs_get_option_ul(args, &option) ||
			    option < NFS_MNT_VERSION ||
			    option > NFS_MNT3_VERSION)
				goto out_invalid_value;
			mnt->mount_server.version = option;
			break;
<<<<<<< HEAD
<<<<<<< HEAD
		case Opt_nfsvers:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			switch (option) {
			case NFS2_VERSION:
				mnt->flags &= ~NFS_MOUNT_VER3;
				mnt->version = 2;
				break;
			case NFS3_VERSION:
				mnt->flags |= NFS_MOUNT_VER3;
				mnt->version = 3;
				break;
			case NFS4_VERSION:
				mnt->flags &= ~NFS_MOUNT_VER3;
				mnt->version = 4;
				break;
			default:
				goto out_invalid_value;
			}
			break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case Opt_minorversion:
			if (nfs_get_option_ul(args, &option))
				goto out_invalid_value;
			if (option > NFS4_MAX_MINOR_VERSION)
				goto out_invalid_value;
			mnt->minorversion = option;
			break;

		/*
		 * options that take text values
		 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
		case Opt_nfsvers:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;
			rc = nfs_parse_version_string(string, mnt, args);
			kfree(string);
			if (!rc)
				goto out_invalid_value;
			break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case Opt_sec:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;
			rc = nfs_parse_security_flavors(string, mnt);
			kfree(string);
			if (!rc) {
				dfprintk(MOUNT, "NFS:   unrecognized "
						"security flavor\n");
				return 0;
			}
			break;
		case Opt_proto:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;
			token = match_token(string,
					    nfs_xprt_protocol_tokens, args);

			protofamily = AF_INET;
			switch (token) {
			case Opt_xprt_udp6:
				protofamily = AF_INET6;
			case Opt_xprt_udp:
				mnt->flags &= ~NFS_MOUNT_TCP;
				mnt->nfs_server.protocol = XPRT_TRANSPORT_UDP;
				break;
			case Opt_xprt_tcp6:
				protofamily = AF_INET6;
			case Opt_xprt_tcp:
				mnt->flags |= NFS_MOUNT_TCP;
				mnt->nfs_server.protocol = XPRT_TRANSPORT_TCP;
				break;
			case Opt_xprt_rdma:
				/* vector side protocols to TCP */
				mnt->flags |= NFS_MOUNT_TCP;
				mnt->nfs_server.protocol = XPRT_TRANSPORT_RDMA;
				xprt_load_transport(string);
				break;
			default:
				dfprintk(MOUNT, "NFS:   unrecognized "
						"transport protocol\n");
				kfree(string);
				return 0;
			}
			kfree(string);
			break;
		case Opt_mountproto:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;
			token = match_token(string,
					    nfs_xprt_protocol_tokens, args);
			kfree(string);

			mountfamily = AF_INET;
			switch (token) {
			case Opt_xprt_udp6:
				mountfamily = AF_INET6;
			case Opt_xprt_udp:
				mnt->mount_server.protocol = XPRT_TRANSPORT_UDP;
				break;
			case Opt_xprt_tcp6:
				mountfamily = AF_INET6;
			case Opt_xprt_tcp:
				mnt->mount_server.protocol = XPRT_TRANSPORT_TCP;
				break;
			case Opt_xprt_rdma: /* not used for side protocols */
			default:
				dfprintk(MOUNT, "NFS:   unrecognized "
						"transport protocol\n");
				return 0;
			}
			break;
		case Opt_addr:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;
			mnt->nfs_server.addrlen =
<<<<<<< HEAD
<<<<<<< HEAD
				rpc_pton(string, strlen(string),
=======
				rpc_pton(mnt->net, string, strlen(string),
>>>>>>> refs/remotes/origin/cm-10.0
=======
				rpc_pton(mnt->net, string, strlen(string),
>>>>>>> refs/remotes/origin/master
					(struct sockaddr *)
					&mnt->nfs_server.address,
					sizeof(mnt->nfs_server.address));
			kfree(string);
			if (mnt->nfs_server.addrlen == 0)
				goto out_invalid_address;
			break;
		case Opt_clientaddr:
			if (nfs_get_option_str(args, &mnt->client_address))
				goto out_nomem;
			break;
		case Opt_mounthost:
			if (nfs_get_option_str(args,
					       &mnt->mount_server.hostname))
				goto out_nomem;
			break;
		case Opt_mountaddr:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;
			mnt->mount_server.addrlen =
<<<<<<< HEAD
<<<<<<< HEAD
				rpc_pton(string, strlen(string),
=======
				rpc_pton(mnt->net, string, strlen(string),
>>>>>>> refs/remotes/origin/cm-10.0
=======
				rpc_pton(mnt->net, string, strlen(string),
>>>>>>> refs/remotes/origin/master
					(struct sockaddr *)
					&mnt->mount_server.address,
					sizeof(mnt->mount_server.address));
			kfree(string);
			if (mnt->mount_server.addrlen == 0)
				goto out_invalid_address;
			break;
		case Opt_lookupcache:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;
			token = match_token(string,
					nfs_lookupcache_tokens, args);
			kfree(string);
			switch (token) {
				case Opt_lookupcache_all:
					mnt->flags &= ~(NFS_MOUNT_LOOKUP_CACHE_NONEG|NFS_MOUNT_LOOKUP_CACHE_NONE);
					break;
				case Opt_lookupcache_positive:
					mnt->flags &= ~NFS_MOUNT_LOOKUP_CACHE_NONE;
					mnt->flags |= NFS_MOUNT_LOOKUP_CACHE_NONEG;
					break;
				case Opt_lookupcache_none:
					mnt->flags |= NFS_MOUNT_LOOKUP_CACHE_NONEG|NFS_MOUNT_LOOKUP_CACHE_NONE;
					break;
				default:
					dfprintk(MOUNT, "NFS:   invalid "
							"lookupcache argument\n");
					return 0;
			};
			break;
		case Opt_fscache_uniq:
			if (nfs_get_option_str(args, &mnt->fscache_uniq))
				goto out_nomem;
			mnt->options |= NFS_OPTION_FSCACHE;
			break;
		case Opt_local_lock:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;
			token = match_token(string, nfs_local_lock_tokens,
					args);
			kfree(string);
			switch (token) {
			case Opt_local_lock_all:
				mnt->flags |= (NFS_MOUNT_LOCAL_FLOCK |
					       NFS_MOUNT_LOCAL_FCNTL);
				break;
			case Opt_local_lock_flock:
				mnt->flags |= NFS_MOUNT_LOCAL_FLOCK;
				break;
			case Opt_local_lock_posix:
				mnt->flags |= NFS_MOUNT_LOCAL_FCNTL;
				break;
			case Opt_local_lock_none:
				mnt->flags &= ~(NFS_MOUNT_LOCAL_FLOCK |
						NFS_MOUNT_LOCAL_FCNTL);
				break;
			default:
				dfprintk(MOUNT, "NFS:	invalid	"
						"local_lock argument\n");
				return 0;
			};
			break;

		/*
		 * Special options
		 */
		case Opt_sloppy:
			sloppy = 1;
			dfprintk(MOUNT, "NFS:   relaxing parsing rules\n");
			break;
		case Opt_userspace:
		case Opt_deprecated:
			dfprintk(MOUNT, "NFS:   ignoring mount option "
					"'%s'\n", p);
			break;

		default:
			invalid_option = 1;
			dfprintk(MOUNT, "NFS:   unrecognized mount option "
					"'%s'\n", p);
		}
	}

	if (!sloppy && invalid_option)
		return 0;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (mnt->minorversion && mnt->version != 4)
		goto out_minorversion_mismatch;

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * verify that any proto=/mountproto= options match the address
	 * familiies in the addr=/mountaddr= options.
=======
	if (mnt->minorversion && mnt->version != 4)
		goto out_minorversion_mismatch;

	if (mnt->options & NFS_OPTION_MIGRATION &&
	    (mnt->version != 4 || mnt->minorversion != 0))
		goto out_migration_misuse;

	/*
	 * verify that any proto=/mountproto= options match the address
	 * families in the addr=/mountaddr= options.
>>>>>>> refs/remotes/origin/master
	 */
	if (protofamily != AF_UNSPEC &&
	    protofamily != mnt->nfs_server.address.ss_family)
		goto out_proto_mismatch;

	if (mountfamily != AF_UNSPEC) {
		if (mnt->mount_server.addrlen) {
			if (mountfamily != mnt->mount_server.address.ss_family)
				goto out_mountproto_mismatch;
		} else {
			if (mountfamily != mnt->nfs_server.address.ss_family)
				goto out_mountproto_mismatch;
		}
	}

	return 1;

out_mountproto_mismatch:
	printk(KERN_INFO "NFS: mount server address does not match mountproto= "
			 "option\n");
	return 0;
out_proto_mismatch:
	printk(KERN_INFO "NFS: server address does not match proto= option\n");
	return 0;
out_invalid_address:
	printk(KERN_INFO "NFS: bad IP address specified: %s\n", p);
	return 0;
out_invalid_value:
	printk(KERN_INFO "NFS: bad mount option value specified: %s\n", p);
	return 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
out_minorversion_mismatch:
	printk(KERN_INFO "NFS: mount option vers=%u does not support "
			 "minorversion=%u\n", mnt->version, mnt->minorversion);
	return 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
out_migration_misuse:
	printk(KERN_INFO
		"NFS: 'migration' not supported for this NFS version\n");
	return 0;
>>>>>>> refs/remotes/origin/master
out_nomem:
	printk(KERN_INFO "NFS: not enough memory to parse option\n");
	return 0;
out_security_failure:
	free_secdata(secdata);
	printk(KERN_INFO "NFS: security options invalid: %d\n", rc);
	return 0;
}

/*
<<<<<<< HEAD
 * Match the requested auth flavors with the list returned by
 * the server.  Returns zero and sets the mount's authentication
 * flavor on success; returns -EACCES if server does not support
 * the requested flavor.
 */
static int nfs_walk_authlist(struct nfs_parsed_mount_data *args,
			     struct nfs_mount_request *request)
{
	unsigned int i, j, server_authlist_len = *(request->auth_flav_len);

	/*
	 * Certain releases of Linux's mountd return an empty
	 * flavor list.  To prevent behavioral regression with
	 * these servers (ie. rejecting mounts that used to
	 * succeed), revert to pre-2.6.32 behavior (no checking)
	 * if the returned flavor list is empty.
	 */
	if (server_authlist_len == 0)
		return 0;

	/*
	 * We avoid sophisticated negotiating here, as there are
	 * plenty of cases where we can get it wrong, providing
	 * either too little or too much security.
	 *
	 * RFC 2623, section 2.7 suggests we SHOULD prefer the
	 * flavor listed first.  However, some servers list
	 * AUTH_NULL first.  Our caller plants AUTH_SYS, the
	 * preferred default, in args->auth_flavors[0] if user
	 * didn't specify sec= mount option.
	 */
	for (i = 0; i < args->auth_flavor_len; i++)
		for (j = 0; j < server_authlist_len; j++)
			if (args->auth_flavors[i] == request->auth_flavs[j]) {
				dfprintk(MOUNT, "NFS: using auth flavor %d\n",
					request->auth_flavs[j]);
				args->auth_flavors[0] = request->auth_flavs[j];
				return 0;
			}

	dfprintk(MOUNT, "NFS: server does not support requested auth flavor\n");
	nfs_umount(request);
	return -EACCES;
=======
 * Ensure that a specified authtype in args->auth_info is supported by
 * the server. Returns 0 and sets args->selected_flavor if it's ok, and
 * -EACCES if not.
 */
static int nfs_verify_authflavors(struct nfs_parsed_mount_data *args,
			rpc_authflavor_t *server_authlist, unsigned int count)
{
	rpc_authflavor_t flavor = RPC_AUTH_MAXFLAVOR;
	unsigned int i;

	/*
	 * If the sec= mount option is used, the specified flavor or AUTH_NULL
	 * must be in the list returned by the server.
	 *
	 * AUTH_NULL has a special meaning when it's in the server list - it
	 * means that the server will ignore the rpc creds, so any flavor
	 * can be used.
	 */
	for (i = 0; i < count; i++) {
		flavor = server_authlist[i];

		if (nfs_auth_info_match(&args->auth_info, flavor) ||
		    flavor == RPC_AUTH_NULL)
			goto out;
	}

	dfprintk(MOUNT,
		 "NFS: specified auth flavors not supported by server\n");
	return -EACCES;

out:
	args->selected_flavor = flavor;
	dfprintk(MOUNT, "NFS: using auth flavor %u\n", args->selected_flavor);
	return 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * Use the remote server's MOUNT service to request the NFS file handle
 * corresponding to the provided path.
 */
<<<<<<< HEAD
static int nfs_try_mount(struct nfs_parsed_mount_data *args,
			 struct nfs_fh *root_fh)
{
	rpc_authflavor_t server_authlist[NFS_MAX_SECFLAVORS];
	unsigned int server_authlist_len = ARRAY_SIZE(server_authlist);
=======
static int nfs_request_mount(struct nfs_parsed_mount_data *args,
			     struct nfs_fh *root_fh,
			     rpc_authflavor_t *server_authlist,
			     unsigned int *server_authlist_len)
{
>>>>>>> refs/remotes/origin/master
	struct nfs_mount_request request = {
		.sap		= (struct sockaddr *)
						&args->mount_server.address,
		.dirpath	= args->nfs_server.export_path,
		.protocol	= args->mount_server.protocol,
		.fh		= root_fh,
		.noresvport	= args->flags & NFS_MOUNT_NORESVPORT,
<<<<<<< HEAD
		.auth_flav_len	= &server_authlist_len,
		.auth_flavs	= server_authlist,
<<<<<<< HEAD
=======
		.net		= args->net,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.auth_flav_len	= server_authlist_len,
		.auth_flavs	= server_authlist,
		.net		= args->net,
>>>>>>> refs/remotes/origin/master
	};
	int status;

	if (args->mount_server.version == 0) {
		switch (args->version) {
			default:
				args->mount_server.version = NFS_MNT3_VERSION;
				break;
			case 2:
				args->mount_server.version = NFS_MNT_VERSION;
		}
	}
	request.version = args->mount_server.version;

	if (args->mount_server.hostname)
		request.hostname = args->mount_server.hostname;
	else
		request.hostname = args->nfs_server.hostname;

	/*
	 * Construct the mount server's address.
	 */
	if (args->mount_server.address.ss_family == AF_UNSPEC) {
		memcpy(request.sap, &args->nfs_server.address,
		       args->nfs_server.addrlen);
		args->mount_server.addrlen = args->nfs_server.addrlen;
	}
	request.salen = args->mount_server.addrlen;
	nfs_set_port(request.sap, &args->mount_server.port, 0);

	/*
	 * Now ask the mount server to map our export path
	 * to a file handle.
	 */
	status = nfs_mount(&request);
	if (status != 0) {
		dfprintk(MOUNT, "NFS: unable to mount server %s, error %d\n",
				request.hostname, status);
		return status;
	}

<<<<<<< HEAD
	/*
	 * MNTv1 (NFSv2) does not support auth flavor negotiation.
	 */
	if (args->mount_server.version != NFS_MNT3_VERSION)
		return 0;
	return nfs_walk_authlist(args, &request);
}
=======
	return 0;
}

static struct nfs_server *nfs_try_mount_request(struct nfs_mount_info *mount_info,
					struct nfs_subversion *nfs_mod)
{
	int status;
	unsigned int i;
	bool tried_auth_unix = false;
	bool auth_null_in_list = false;
	struct nfs_server *server = ERR_PTR(-EACCES);
	struct nfs_parsed_mount_data *args = mount_info->parsed;
	rpc_authflavor_t authlist[NFS_MAX_SECFLAVORS];
	unsigned int authlist_len = ARRAY_SIZE(authlist);

	status = nfs_request_mount(args, mount_info->mntfh, authlist,
					&authlist_len);
	if (status)
		return ERR_PTR(status);

	/*
	 * Was a sec= authflavor specified in the options? First, verify
	 * whether the server supports it, and then just try to use it if so.
	 */
	if (args->auth_info.flavor_len > 0) {
		status = nfs_verify_authflavors(args, authlist, authlist_len);
		dfprintk(MOUNT, "NFS: using auth flavor %u\n",
			 args->selected_flavor);
		if (status)
			return ERR_PTR(status);
		return nfs_mod->rpc_ops->create_server(mount_info, nfs_mod);
	}

	/*
	 * No sec= option was provided. RFC 2623, section 2.7 suggests we
	 * SHOULD prefer the flavor listed first. However, some servers list
	 * AUTH_NULL first. Avoid ever choosing AUTH_NULL.
	 */
	for (i = 0; i < authlist_len; ++i) {
		rpc_authflavor_t flavor;
		struct rpcsec_gss_info info;

		flavor = authlist[i];
		switch (flavor) {
		case RPC_AUTH_UNIX:
			tried_auth_unix = true;
			break;
		case RPC_AUTH_NULL:
			auth_null_in_list = true;
			continue;
		default:
			if (rpcauth_get_gssinfo(flavor, &info) != 0)
				continue;
			/* Fallthrough */
		}
		dfprintk(MOUNT, "NFS: attempting to use auth flavor %u\n", flavor);
		args->selected_flavor = flavor;
		server = nfs_mod->rpc_ops->create_server(mount_info, nfs_mod);
		if (!IS_ERR(server))
			return server;
	}

	/*
	 * Nothing we tried so far worked. At this point, give up if we've
	 * already tried AUTH_UNIX or if the server's list doesn't contain
	 * AUTH_NULL
	 */
	if (tried_auth_unix || !auth_null_in_list)
		return server;

	/* Last chance! Try AUTH_UNIX */
	dfprintk(MOUNT, "NFS: attempting to use auth flavor %u\n", RPC_AUTH_UNIX);
	args->selected_flavor = RPC_AUTH_UNIX;
	return nfs_mod->rpc_ops->create_server(mount_info, nfs_mod);
}

struct dentry *nfs_try_mount(int flags, const char *dev_name,
			     struct nfs_mount_info *mount_info,
			     struct nfs_subversion *nfs_mod)
{
	struct nfs_server *server;

	if (mount_info->parsed->need_mount)
		server = nfs_try_mount_request(mount_info, nfs_mod);
	else
		server = nfs_mod->rpc_ops->create_server(mount_info, nfs_mod);

	if (IS_ERR(server))
		return ERR_CAST(server);

	return nfs_fs_mount_common(server, flags, dev_name, mount_info, nfs_mod);
}
EXPORT_SYMBOL_GPL(nfs_try_mount);
>>>>>>> refs/remotes/origin/master

/*
 * Split "dev_name" into "hostname:export_path".
 *
 * The leftmost colon demarks the split between the server's hostname
 * and the export path.  If the hostname starts with a left square
 * bracket, then it may contain colons.
 *
 * Note: caller frees hostname and export path, even on error.
 */
static int nfs_parse_devname(const char *dev_name,
			     char **hostname, size_t maxnamlen,
			     char **export_path, size_t maxpathlen)
{
	size_t len;
	char *end;

	/* Is the host name protected with square brakcets? */
	if (*dev_name == '[') {
		end = strchr(++dev_name, ']');
		if (end == NULL || end[1] != ':')
			goto out_bad_devname;

		len = end - dev_name;
		end++;
	} else {
		char *comma;

		end = strchr(dev_name, ':');
		if (end == NULL)
			goto out_bad_devname;
		len = end - dev_name;

		/* kill possible hostname list: not supported */
		comma = strchr(dev_name, ',');
		if (comma != NULL && comma < end)
			*comma = 0;
	}

	if (len > maxnamlen)
		goto out_hostname;

	/* N.B. caller will free nfs_server.hostname in all cases */
	*hostname = kstrndup(dev_name, len, GFP_KERNEL);
	if (*hostname == NULL)
		goto out_nomem;
	len = strlen(++end);
	if (len > maxpathlen)
		goto out_path;
	*export_path = kstrndup(end, len, GFP_KERNEL);
	if (!*export_path)
		goto out_nomem;

	dfprintk(MOUNT, "NFS: MNTPATH: '%s'\n", *export_path);
	return 0;

out_bad_devname:
	dfprintk(MOUNT, "NFS: device name not in host:path format\n");
	return -EINVAL;

out_nomem:
	dfprintk(MOUNT, "NFS: not enough memory to parse device name\n");
	return -ENOMEM;

out_hostname:
	dfprintk(MOUNT, "NFS: server hostname too long\n");
	return -ENAMETOOLONG;

out_path:
	dfprintk(MOUNT, "NFS: export pathname too long\n");
	return -ENAMETOOLONG;
}

/*
 * Validate the NFS2/NFS3 mount data
 * - fills in the mount root filehandle
 *
 * For option strings, user space handles the following behaviors:
 *
 * + DNS: mapping server host name to IP address ("addr=" option)
 *
 * + failure mode: how to behave if a mount request can't be handled
 *   immediately ("fg/bg" option)
 *
 * + retry: how often to retry a mount request ("retry=" option)
 *
 * + breaking back: trying proto=udp after proto=tcp, v2 after v3,
 *   mountproto=tcp after mountproto=udp, and so on
 */
<<<<<<< HEAD
static int nfs_validate_mount_data(void *options,
				   struct nfs_parsed_mount_data *args,
				   struct nfs_fh *mntfh,
				   const char *dev_name)
{
	struct nfs_mount_data *data = (struct nfs_mount_data *)options;
	struct sockaddr *sap = (struct sockaddr *)&args->nfs_server.address;
=======
static int nfs23_validate_mount_data(void *options,
				     struct nfs_parsed_mount_data *args,
				     struct nfs_fh *mntfh,
				     const char *dev_name)
{
	struct nfs_mount_data *data = (struct nfs_mount_data *)options;
	struct sockaddr *sap = (struct sockaddr *)&args->nfs_server.address;
	int extra_flags = NFS_MOUNT_LEGACY_INTERFACE;
>>>>>>> refs/remotes/origin/master

	if (data == NULL)
		goto out_no_data;

<<<<<<< HEAD
=======
	args->version = NFS_DEFAULT_VERSION;
>>>>>>> refs/remotes/origin/master
	switch (data->version) {
	case 1:
		data->namlen = 0;
	case 2:
		data->bsize = 0;
	case 3:
		if (data->flags & NFS_MOUNT_VER3)
			goto out_no_v3;
		data->root.size = NFS2_FHSIZE;
		memcpy(data->root.data, data->old_root.data, NFS2_FHSIZE);
<<<<<<< HEAD
=======
		/* Turn off security negotiation */
		extra_flags |= NFS_MOUNT_SECFLAVOUR;
>>>>>>> refs/remotes/origin/master
	case 4:
		if (data->flags & NFS_MOUNT_SECFLAVOUR)
			goto out_no_sec;
	case 5:
		memset(data->context, 0, sizeof(data->context));
	case 6:
		if (data->flags & NFS_MOUNT_VER3) {
			if (data->root.size > NFS3_FHSIZE || data->root.size == 0)
				goto out_invalid_fh;
			mntfh->size = data->root.size;
			args->version = 3;
		} else {
			mntfh->size = NFS2_FHSIZE;
			args->version = 2;
		}


		memcpy(mntfh->data, data->root.data, mntfh->size);
		if (mntfh->size < sizeof(mntfh->data))
			memset(mntfh->data + mntfh->size, 0,
			       sizeof(mntfh->data) - mntfh->size);

		/*
		 * Translate to nfs_parsed_mount_data, which nfs_fill_super
		 * can deal with.
		 */
		args->flags		= data->flags & NFS_MOUNT_FLAGMASK;
<<<<<<< HEAD
		args->flags		|= NFS_MOUNT_LEGACY_INTERFACE;
=======
		args->flags		|= extra_flags;
>>>>>>> refs/remotes/origin/master
		args->rsize		= data->rsize;
		args->wsize		= data->wsize;
		args->timeo		= data->timeo;
		args->retrans		= data->retrans;
		args->acregmin		= data->acregmin;
		args->acregmax		= data->acregmax;
		args->acdirmin		= data->acdirmin;
		args->acdirmax		= data->acdirmax;
<<<<<<< HEAD
=======
		args->need_mount	= false;
>>>>>>> refs/remotes/origin/master

		memcpy(sap, &data->addr, sizeof(data->addr));
		args->nfs_server.addrlen = sizeof(data->addr);
		args->nfs_server.port = ntohs(data->addr.sin_port);
		if (!nfs_verify_server_address(sap))
			goto out_no_address;

		if (!(data->flags & NFS_MOUNT_TCP))
			args->nfs_server.protocol = XPRT_TRANSPORT_UDP;
		/* N.B. caller will free nfs_server.hostname in all cases */
		args->nfs_server.hostname = kstrdup(data->hostname, GFP_KERNEL);
		args->namlen		= data->namlen;
		args->bsize		= data->bsize;

		if (data->flags & NFS_MOUNT_SECFLAVOUR)
<<<<<<< HEAD
			args->auth_flavors[0] = data->pseudoflavor;
=======
			args->selected_flavor = data->pseudoflavor;
		else
			args->selected_flavor = RPC_AUTH_UNIX;
>>>>>>> refs/remotes/origin/master
		if (!args->nfs_server.hostname)
			goto out_nomem;

		if (!(data->flags & NFS_MOUNT_NONLM))
			args->flags &= ~(NFS_MOUNT_LOCAL_FLOCK|
					 NFS_MOUNT_LOCAL_FCNTL);
		else
			args->flags |= (NFS_MOUNT_LOCAL_FLOCK|
					NFS_MOUNT_LOCAL_FCNTL);
		/*
		 * The legacy version 6 binary mount data from userspace has a
		 * field used only to transport selinux information into the
		 * the kernel.  To continue to support that functionality we
		 * have a touch of selinux knowledge here in the NFS code. The
		 * userspace code converted context=blah to just blah so we are
		 * converting back to the full string selinux understands.
		 */
		if (data->context[0]){
#ifdef CONFIG_SECURITY_SELINUX
			int rc;
			char *opts_str = kmalloc(sizeof(data->context) + 8, GFP_KERNEL);
			if (!opts_str)
				return -ENOMEM;
			strcpy(opts_str, "context=");
			data->context[NFS_MAX_CONTEXT_LEN] = '\0';
			strcat(opts_str, &data->context[0]);
			rc = security_sb_parse_opts_str(opts_str, &args->lsm_opts);
			kfree(opts_str);
			if (rc)
				return rc;
#else
			return -EINVAL;
#endif
		}

		break;
<<<<<<< HEAD
	default: {
		int status;

		if (nfs_parse_mount_options((char *)options, args) == 0)
			return -EINVAL;

		if (!nfs_verify_server_address(sap))
			goto out_no_address;

		if (args->version == 4)
#ifdef CONFIG_NFS_V4
			return nfs4_validate_text_mount_data(options,
							     args, dev_name);
#else
			goto out_v4_not_compiled;
#endif

		nfs_set_port(sap, &args->nfs_server.port, 0);

		nfs_set_mount_transport_protocol(args);

		status = nfs_parse_devname(dev_name,
					   &args->nfs_server.hostname,
					   PAGE_SIZE,
					   &args->nfs_server.export_path,
					   NFS_MAXPATHLEN);
		if (!status)
			status = nfs_try_mount(args, mntfh);

		kfree(args->nfs_server.export_path);
		args->nfs_server.export_path = NULL;

		if (status)
			return status;

		break;
		}
	}

#ifndef CONFIG_NFS_V3
=======
	default:
		return NFS_TEXT_DATA;
	}

#if !IS_ENABLED(CONFIG_NFS_V3)
>>>>>>> refs/remotes/origin/master
	if (args->version == 3)
		goto out_v3_not_compiled;
#endif /* !CONFIG_NFS_V3 */

	return 0;

out_no_data:
	dfprintk(MOUNT, "NFS: mount program didn't pass any mount data\n");
	return -EINVAL;

out_no_v3:
	dfprintk(MOUNT, "NFS: nfs_mount_data version %d does not support v3\n",
		 data->version);
	return -EINVAL;

out_no_sec:
	dfprintk(MOUNT, "NFS: nfs_mount_data version supports only AUTH_SYS\n");
	return -EINVAL;

<<<<<<< HEAD
#ifndef CONFIG_NFS_V3
=======
#if !IS_ENABLED(CONFIG_NFS_V3)
>>>>>>> refs/remotes/origin/master
out_v3_not_compiled:
	dfprintk(MOUNT, "NFS: NFSv3 is not compiled into kernel\n");
	return -EPROTONOSUPPORT;
#endif /* !CONFIG_NFS_V3 */

<<<<<<< HEAD
#ifndef CONFIG_NFS_V4
out_v4_not_compiled:
	dfprintk(MOUNT, "NFS: NFSv4 is not compiled into kernel\n");
	return -EPROTONOSUPPORT;
#endif /* !CONFIG_NFS_V4 */

=======
>>>>>>> refs/remotes/origin/master
out_nomem:
	dfprintk(MOUNT, "NFS: not enough memory to handle mount options\n");
	return -ENOMEM;

out_no_address:
	dfprintk(MOUNT, "NFS: mount program didn't pass remote address\n");
	return -EINVAL;

out_invalid_fh:
	dfprintk(MOUNT, "NFS: invalid root filehandle\n");
	return -EINVAL;
}

<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_NFS_V4)
static int nfs_validate_mount_data(struct file_system_type *fs_type,
				   void *options,
				   struct nfs_parsed_mount_data *args,
				   struct nfs_fh *mntfh,
				   const char *dev_name)
{
	if (fs_type == &nfs_fs_type)
		return nfs23_validate_mount_data(options, args, mntfh, dev_name);
	return nfs4_validate_mount_data(options, args, dev_name);
}
#else
static int nfs_validate_mount_data(struct file_system_type *fs_type,
				   void *options,
				   struct nfs_parsed_mount_data *args,
				   struct nfs_fh *mntfh,
				   const char *dev_name)
{
	return nfs23_validate_mount_data(options, args, mntfh, dev_name);
}
#endif

static int nfs_validate_text_mount_data(void *options,
					struct nfs_parsed_mount_data *args,
					const char *dev_name)
{
	int port = 0;
	int max_namelen = PAGE_SIZE;
	int max_pathlen = NFS_MAXPATHLEN;
	struct sockaddr *sap = (struct sockaddr *)&args->nfs_server.address;

	if (nfs_parse_mount_options((char *)options, args) == 0)
		return -EINVAL;

	if (!nfs_verify_server_address(sap))
		goto out_no_address;

	if (args->version == 4) {
#if IS_ENABLED(CONFIG_NFS_V4)
		port = NFS_PORT;
		max_namelen = NFS4_MAXNAMLEN;
		max_pathlen = NFS4_MAXPATHLEN;
		nfs_validate_transport_protocol(args);
		if (args->nfs_server.protocol == XPRT_TRANSPORT_UDP)
			goto out_invalid_transport_udp;
		nfs4_validate_mount_flags(args);
#else
		goto out_v4_not_compiled;
#endif /* CONFIG_NFS_V4 */
	} else
		nfs_set_mount_transport_protocol(args);

	nfs_set_port(sap, &args->nfs_server.port, port);

	return nfs_parse_devname(dev_name,
				   &args->nfs_server.hostname,
				   max_namelen,
				   &args->nfs_server.export_path,
				   max_pathlen);

#if !IS_ENABLED(CONFIG_NFS_V4)
out_v4_not_compiled:
	dfprintk(MOUNT, "NFS: NFSv4 is not compiled into kernel\n");
	return -EPROTONOSUPPORT;
#else
out_invalid_transport_udp:
	dfprintk(MOUNT, "NFSv4: Unsupported transport protocol udp\n");
	return -EINVAL;
#endif /* !CONFIG_NFS_V4 */

out_no_address:
	dfprintk(MOUNT, "NFS: mount program didn't pass remote address\n");
	return -EINVAL;
}

>>>>>>> refs/remotes/origin/master
static int
nfs_compare_remount_data(struct nfs_server *nfss,
			 struct nfs_parsed_mount_data *data)
{
	if (data->flags != nfss->flags ||
	    data->rsize != nfss->rsize ||
	    data->wsize != nfss->wsize ||
<<<<<<< HEAD
	    data->retrans != nfss->client->cl_timeout->to_retries ||
	    data->auth_flavors[0] != nfss->client->cl_auth->au_flavor ||
=======
	    data->version != nfss->nfs_client->rpc_ops->version ||
	    data->minorversion != nfss->nfs_client->cl_minorversion ||
	    data->retrans != nfss->client->cl_timeout->to_retries ||
	    data->selected_flavor != nfss->client->cl_auth->au_flavor ||
>>>>>>> refs/remotes/origin/master
	    data->acregmin != nfss->acregmin / HZ ||
	    data->acregmax != nfss->acregmax / HZ ||
	    data->acdirmin != nfss->acdirmin / HZ ||
	    data->acdirmax != nfss->acdirmax / HZ ||
	    data->timeo != (10U * nfss->client->cl_timeout->to_initval / HZ) ||
	    data->nfs_server.port != nfss->port ||
	    data->nfs_server.addrlen != nfss->nfs_client->cl_addrlen ||
	    !rpc_cmp_addr((struct sockaddr *)&data->nfs_server.address,
			  (struct sockaddr *)&nfss->nfs_client->cl_addr))
		return -EINVAL;

	return 0;
}

<<<<<<< HEAD
static int
=======
int
>>>>>>> refs/remotes/origin/master
nfs_remount(struct super_block *sb, int *flags, char *raw_data)
{
	int error;
	struct nfs_server *nfss = sb->s_fs_info;
	struct nfs_parsed_mount_data *data;
	struct nfs_mount_data *options = (struct nfs_mount_data *)raw_data;
	struct nfs4_mount_data *options4 = (struct nfs4_mount_data *)raw_data;
	u32 nfsvers = nfss->nfs_client->rpc_ops->version;

	/*
	 * Userspace mount programs that send binary options generally send
	 * them populated with default values. We have no way to know which
	 * ones were explicitly specified. Fall back to legacy behavior and
	 * just return success.
	 */
	if ((nfsvers == 4 && (!options4 || options4->version == 1)) ||
	    (nfsvers <= 3 && (!options || (options->version >= 1 &&
					   options->version <= 6))))
		return 0;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;

	/* fill out struct with values from existing mount */
	data->flags = nfss->flags;
	data->rsize = nfss->rsize;
	data->wsize = nfss->wsize;
	data->retrans = nfss->client->cl_timeout->to_retries;
<<<<<<< HEAD
	data->auth_flavors[0] = nfss->client->cl_auth->au_flavor;
=======
	data->selected_flavor = nfss->client->cl_auth->au_flavor;
	data->auth_info = nfss->auth_info;
>>>>>>> refs/remotes/origin/master
	data->acregmin = nfss->acregmin / HZ;
	data->acregmax = nfss->acregmax / HZ;
	data->acdirmin = nfss->acdirmin / HZ;
	data->acdirmax = nfss->acdirmax / HZ;
	data->timeo = 10U * nfss->client->cl_timeout->to_initval / HZ;
	data->nfs_server.port = nfss->port;
	data->nfs_server.addrlen = nfss->nfs_client->cl_addrlen;
<<<<<<< HEAD
=======
	data->version = nfsvers;
	data->minorversion = nfss->nfs_client->cl_minorversion;
>>>>>>> refs/remotes/origin/master
	memcpy(&data->nfs_server.address, &nfss->nfs_client->cl_addr,
		data->nfs_server.addrlen);

	/* overwrite those values with any that were specified */
<<<<<<< HEAD
	error = nfs_parse_mount_options((char *)options, data);
	if (error < 0)
=======
	error = -EINVAL;
	if (!nfs_parse_mount_options((char *)options, data))
>>>>>>> refs/remotes/origin/master
		goto out;

	/*
	 * noac is a special case. It implies -o sync, but that's not
	 * necessarily reflected in the mtab options. do_remount_sb
	 * will clear MS_SYNCHRONOUS if -o sync wasn't specified in the
	 * remount options, so we have to explicitly reset it.
	 */
	if (data->flags & NFS_MOUNT_NOAC)
		*flags |= MS_SYNCHRONOUS;

	/* compare new mount options with old ones */
	error = nfs_compare_remount_data(nfss, data);
out:
	kfree(data);
	return error;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_remount);
>>>>>>> refs/remotes/origin/master

/*
 * Initialise the common bits of the superblock
 */
<<<<<<< HEAD
static inline void nfs_initialise_sb(struct super_block *sb)
=======
inline void nfs_initialise_sb(struct super_block *sb)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_server *server = NFS_SB(sb);

	sb->s_magic = NFS_SUPER_MAGIC;

	/* We probably want something more informative here */
	snprintf(sb->s_id, sizeof(sb->s_id),
<<<<<<< HEAD
<<<<<<< HEAD
		 "%x:%x", MAJOR(sb->s_dev), MINOR(sb->s_dev));
=======
		 "%u:%u", MAJOR(sb->s_dev), MINOR(sb->s_dev));
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 "%u:%u", MAJOR(sb->s_dev), MINOR(sb->s_dev));
>>>>>>> refs/remotes/origin/master

	if (sb->s_blocksize == 0)
		sb->s_blocksize = nfs_block_bits(server->wsize,
						 &sb->s_blocksize_bits);

<<<<<<< HEAD
<<<<<<< HEAD
	if (server->flags & NFS_MOUNT_NOAC)
		sb->s_flags |= MS_SYNCHRONOUS;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	sb->s_bdi = &server->backing_dev_info;

	nfs_super_set_maxbytes(sb, server->maxfilesize);
}

/*
 * Finish setting up an NFS2/3 superblock
 */
<<<<<<< HEAD
static void nfs_fill_super(struct super_block *sb,
			   struct nfs_parsed_mount_data *data)
{
=======
void nfs_fill_super(struct super_block *sb, struct nfs_mount_info *mount_info)
{
	struct nfs_parsed_mount_data *data = mount_info->parsed;
>>>>>>> refs/remotes/origin/master
	struct nfs_server *server = NFS_SB(sb);

	sb->s_blocksize_bits = 0;
	sb->s_blocksize = 0;
<<<<<<< HEAD
	if (data->bsize)
		sb->s_blocksize = nfs_block_size(data->bsize, &sb->s_blocksize_bits);

	if (server->nfs_client->rpc_ops->version == 3) {
=======
	sb->s_xattr = server->nfs_client->cl_nfs_mod->xattr;
	sb->s_op = server->nfs_client->cl_nfs_mod->sops;
	if (data && data->bsize)
		sb->s_blocksize = nfs_block_size(data->bsize, &sb->s_blocksize_bits);

	if (server->nfs_client->rpc_ops->version != 2) {
>>>>>>> refs/remotes/origin/master
		/* The VFS shouldn't apply the umask to mode bits. We will do
		 * so ourselves when necessary.
		 */
		sb->s_flags |= MS_POSIXACL;
		sb->s_time_gran = 1;
	}

<<<<<<< HEAD
	sb->s_op = &nfs_sops;
 	nfs_initialise_sb(sb);
}

/*
 * Finish setting up a cloned NFS2/3 superblock
 */
static void nfs_clone_super(struct super_block *sb,
			    const struct super_block *old_sb)
{
=======
 	nfs_initialise_sb(sb);
}
EXPORT_SYMBOL_GPL(nfs_fill_super);

/*
 * Finish setting up a cloned NFS2/3/4 superblock
 */
void nfs_clone_super(struct super_block *sb, struct nfs_mount_info *mount_info)
{
	const struct super_block *old_sb = mount_info->cloned->sb;
>>>>>>> refs/remotes/origin/master
	struct nfs_server *server = NFS_SB(sb);

	sb->s_blocksize_bits = old_sb->s_blocksize_bits;
	sb->s_blocksize = old_sb->s_blocksize;
	sb->s_maxbytes = old_sb->s_maxbytes;
<<<<<<< HEAD

	if (server->nfs_client->rpc_ops->version == 3) {
=======
	sb->s_xattr = old_sb->s_xattr;
	sb->s_op = old_sb->s_op;
	sb->s_time_gran = 1;

	if (server->nfs_client->rpc_ops->version != 2) {
>>>>>>> refs/remotes/origin/master
		/* The VFS shouldn't apply the umask to mode bits. We will do
		 * so ourselves when necessary.
		 */
		sb->s_flags |= MS_POSIXACL;
<<<<<<< HEAD
		sb->s_time_gran = 1;
	}

	sb->s_op = old_sb->s_op;
 	nfs_initialise_sb(sb);
}

=======
	}

 	nfs_initialise_sb(sb);
}

#define NFS_MOUNT_CMP_FLAGMASK ~(NFS_MOUNT_INTR \
		| NFS_MOUNT_SECURE \
		| NFS_MOUNT_TCP \
		| NFS_MOUNT_VER3 \
		| NFS_MOUNT_KERBEROS \
		| NFS_MOUNT_NONLM \
		| NFS_MOUNT_BROKEN_SUID \
		| NFS_MOUNT_STRICTLOCK \
		| NFS_MOUNT_UNSHARED \
		| NFS_MOUNT_NORESVPORT \
		| NFS_MOUNT_LEGACY_INTERFACE)

>>>>>>> refs/remotes/origin/master
static int nfs_compare_mount_options(const struct super_block *s, const struct nfs_server *b, int flags)
{
	const struct nfs_server *a = s->s_fs_info;
	const struct rpc_clnt *clnt_a = a->client;
	const struct rpc_clnt *clnt_b = b->client;

	if ((s->s_flags & NFS_MS_MASK) != (flags & NFS_MS_MASK))
		goto Ebusy;
	if (a->nfs_client != b->nfs_client)
		goto Ebusy;
<<<<<<< HEAD
	if (a->flags != b->flags)
=======
	if ((a->flags ^ b->flags) & NFS_MOUNT_CMP_FLAGMASK)
>>>>>>> refs/remotes/origin/master
		goto Ebusy;
	if (a->wsize != b->wsize)
		goto Ebusy;
	if (a->rsize != b->rsize)
		goto Ebusy;
	if (a->acregmin != b->acregmin)
		goto Ebusy;
	if (a->acregmax != b->acregmax)
		goto Ebusy;
	if (a->acdirmin != b->acdirmin)
		goto Ebusy;
	if (a->acdirmax != b->acdirmax)
		goto Ebusy;
<<<<<<< HEAD
	if (clnt_a->cl_auth->au_flavor != clnt_b->cl_auth->au_flavor)
=======
	if (b->auth_info.flavor_len > 0 &&
	   clnt_a->cl_auth->au_flavor != clnt_b->cl_auth->au_flavor)
>>>>>>> refs/remotes/origin/master
		goto Ebusy;
	return 1;
Ebusy:
	return 0;
}

struct nfs_sb_mountdata {
	struct nfs_server *server;
	int mntflags;
};

static int nfs_set_super(struct super_block *s, void *data)
{
	struct nfs_sb_mountdata *sb_mntdata = data;
	struct nfs_server *server = sb_mntdata->server;
	int ret;

	s->s_flags = sb_mntdata->mntflags;
	s->s_fs_info = server;
	s->s_d_op = server->nfs_client->rpc_ops->dentry_ops;
	ret = set_anon_super(s, server);
	if (ret == 0)
		server->s_dev = s->s_dev;
	return ret;
}

static int nfs_compare_super_address(struct nfs_server *server1,
				     struct nfs_server *server2)
{
	struct sockaddr *sap1, *sap2;

	sap1 = (struct sockaddr *)&server1->nfs_client->cl_addr;
	sap2 = (struct sockaddr *)&server2->nfs_client->cl_addr;

	if (sap1->sa_family != sap2->sa_family)
		return 0;

	switch (sap1->sa_family) {
	case AF_INET: {
		struct sockaddr_in *sin1 = (struct sockaddr_in *)sap1;
		struct sockaddr_in *sin2 = (struct sockaddr_in *)sap2;
		if (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr)
			return 0;
		if (sin1->sin_port != sin2->sin_port)
			return 0;
		break;
	}
	case AF_INET6: {
		struct sockaddr_in6 *sin1 = (struct sockaddr_in6 *)sap1;
		struct sockaddr_in6 *sin2 = (struct sockaddr_in6 *)sap2;
		if (!ipv6_addr_equal(&sin1->sin6_addr, &sin2->sin6_addr))
			return 0;
		if (sin1->sin6_port != sin2->sin6_port)
			return 0;
		break;
	}
	default:
		return 0;
	}

	return 1;
}

static int nfs_compare_super(struct super_block *sb, void *data)
{
	struct nfs_sb_mountdata *sb_mntdata = data;
	struct nfs_server *server = sb_mntdata->server, *old = NFS_SB(sb);
	int mntflags = sb_mntdata->mntflags;

	if (!nfs_compare_super_address(old, server))
		return 0;
	/* Note: NFS_MOUNT_UNSHARED == NFS4_MOUNT_UNSHARED */
	if (old->flags & NFS_MOUNT_UNSHARED)
		return 0;
	if (memcmp(&old->fsid, &server->fsid, sizeof(old->fsid)) != 0)
		return 0;
	return nfs_compare_mount_options(sb, server, mntflags);
}

<<<<<<< HEAD
=======
#ifdef CONFIG_NFS_FSCACHE
static void nfs_get_cache_cookie(struct super_block *sb,
				 struct nfs_parsed_mount_data *parsed,
				 struct nfs_clone_mount *cloned)
{
	struct nfs_server *nfss = NFS_SB(sb);
	char *uniq = NULL;
	int ulen = 0;

	nfss->fscache_key = NULL;
	nfss->fscache = NULL;

	if (parsed) {
		if (!(parsed->options & NFS_OPTION_FSCACHE))
			return;
		if (parsed->fscache_uniq) {
			uniq = parsed->fscache_uniq;
			ulen = strlen(parsed->fscache_uniq);
		}
	} else if (cloned) {
		struct nfs_server *mnt_s = NFS_SB(cloned->sb);
		if (!(mnt_s->options & NFS_OPTION_FSCACHE))
			return;
		if (mnt_s->fscache_key) {
			uniq = mnt_s->fscache_key->key.uniquifier;
			ulen = mnt_s->fscache_key->key.uniq_len;
		};
	} else
		return;

	nfs_fscache_get_super_cookie(sb, uniq, ulen);
}
#else
static void nfs_get_cache_cookie(struct super_block *sb,
				 struct nfs_parsed_mount_data *parsed,
				 struct nfs_clone_mount *cloned)
{
}
#endif

>>>>>>> refs/remotes/origin/master
static int nfs_bdi_register(struct nfs_server *server)
{
	return bdi_register_dev(&server->backing_dev_info, server->s_dev);
}

<<<<<<< HEAD
static struct dentry *nfs_fs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *raw_data)
{
	struct nfs_server *server = NULL;
	struct super_block *s;
	struct nfs_parsed_mount_data *data;
	struct nfs_fh *mntfh;
=======
int nfs_set_sb_security(struct super_block *s, struct dentry *mntroot,
			struct nfs_mount_info *mount_info)
{
	int error;
	unsigned long kflags = 0, kflags_out = 0;
	if (NFS_SB(s)->caps & NFS_CAP_SECURITY_LABEL)
		kflags |= SECURITY_LSM_NATIVE_LABELS;

	error = security_sb_set_mnt_opts(s, &mount_info->parsed->lsm_opts,
						kflags, &kflags_out);
	if (error)
		goto err;

	if (NFS_SB(s)->caps & NFS_CAP_SECURITY_LABEL &&
		!(kflags_out & SECURITY_LSM_NATIVE_LABELS))
		NFS_SB(s)->caps &= ~NFS_CAP_SECURITY_LABEL;
err:
	return error;
}
EXPORT_SYMBOL_GPL(nfs_set_sb_security);

int nfs_clone_sb_security(struct super_block *s, struct dentry *mntroot,
			  struct nfs_mount_info *mount_info)
{
	/* clone any lsm security options from the parent to the new sb */
	if (mntroot->d_inode->i_op != NFS_SB(s)->nfs_client->rpc_ops->dir_inode_ops)
		return -ESTALE;
	return security_sb_clone_mnt_opts(mount_info->cloned->sb, s);
}
EXPORT_SYMBOL_GPL(nfs_clone_sb_security);

struct dentry *nfs_fs_mount_common(struct nfs_server *server,
				   int flags, const char *dev_name,
				   struct nfs_mount_info *mount_info,
				   struct nfs_subversion *nfs_mod)
{
	struct super_block *s;
>>>>>>> refs/remotes/origin/master
	struct dentry *mntroot = ERR_PTR(-ENOMEM);
	int (*compare_super)(struct super_block *, void *) = nfs_compare_super;
	struct nfs_sb_mountdata sb_mntdata = {
		.mntflags = flags,
<<<<<<< HEAD
	};
	int error;

	data = nfs_alloc_parsed_mount_data(NFS_DEFAULT_VERSION);
	mntfh = nfs_alloc_fhandle();
	if (data == NULL || mntfh == NULL)
		goto out;

	/* Validate the mount data */
	error = nfs_validate_mount_data(raw_data, data, mntfh, dev_name);
	if (error < 0) {
		mntroot = ERR_PTR(error);
		goto out;
	}

#ifdef CONFIG_NFS_V4
	if (data->version == 4) {
		mntroot = nfs4_try_mount(flags, dev_name, data);
		goto out;
	}
#endif	/* CONFIG_NFS_V4 */

	/* Get a volume representation */
	server = nfs_create_server(data, mntfh);
	if (IS_ERR(server)) {
		mntroot = ERR_CAST(server);
		goto out;
	}
	sb_mntdata.server = server;

	if (server->flags & NFS_MOUNT_UNSHARED)
		compare_super = NULL;

<<<<<<< HEAD
=======
=======
		.server = server,
	};
	int error;

	if (server->flags & NFS_MOUNT_UNSHARED)
		compare_super = NULL;

>>>>>>> refs/remotes/origin/master
	/* -o noac implies -o sync */
	if (server->flags & NFS_MOUNT_NOAC)
		sb_mntdata.mntflags |= MS_SYNCHRONOUS;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	/* Get a superblock - note that we may end up sharing one that already exists */
	s = sget(fs_type, compare_super, nfs_set_super, &sb_mntdata);
=======
	if (mount_info->cloned != NULL && mount_info->cloned->sb != NULL)
		if (mount_info->cloned->sb->s_flags & MS_SYNCHRONOUS)
			sb_mntdata.mntflags |= MS_SYNCHRONOUS;

	/* Get a superblock - note that we may end up sharing one that already exists */
	s = sget(nfs_mod->nfs_fs, compare_super, nfs_set_super, flags, &sb_mntdata);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(s)) {
		mntroot = ERR_CAST(s);
		goto out_err_nosb;
	}

	if (s->s_fs_info != server) {
		nfs_free_server(server);
		server = NULL;
	} else {
		error = nfs_bdi_register(server);
		if (error) {
			mntroot = ERR_PTR(error);
			goto error_splat_bdi;
		}
<<<<<<< HEAD
=======
		server->super = s;
>>>>>>> refs/remotes/origin/master
	}

	if (!s->s_root) {
		/* initial superblock/root creation */
<<<<<<< HEAD
		nfs_fill_super(s, data);
		nfs_fscache_get_super_cookie(s, data->fscache_uniq, NULL);
	}

	mntroot = nfs_get_root(s, mntfh, dev_name);
	if (IS_ERR(mntroot))
		goto error_splat_super;

	error = security_sb_set_mnt_opts(s, &data->lsm_opts);
=======
		mount_info->fill_super(s, mount_info);
		nfs_get_cache_cookie(s, mount_info->parsed, mount_info->cloned);
	}

	mntroot = nfs_get_root(s, mount_info->mntfh, dev_name);
	if (IS_ERR(mntroot))
		goto error_splat_super;

	error = mount_info->set_security(s, mntroot, mount_info);
>>>>>>> refs/remotes/origin/master
	if (error)
		goto error_splat_root;

	s->s_flags |= MS_ACTIVE;

out:
<<<<<<< HEAD
	nfs_free_parsed_mount_data(data);
	nfs_free_fhandle(mntfh);
=======
>>>>>>> refs/remotes/origin/master
	return mntroot;

out_err_nosb:
	nfs_free_server(server);
	goto out;

error_splat_root:
	dput(mntroot);
	mntroot = ERR_PTR(error);
error_splat_super:
	if (server && !s->s_root)
		bdi_unregister(&server->backing_dev_info);
error_splat_bdi:
	deactivate_locked_super(s);
	goto out;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_fs_mount_common);

struct dentry *nfs_fs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *raw_data)
{
	struct nfs_mount_info mount_info = {
		.fill_super = nfs_fill_super,
		.set_security = nfs_set_sb_security,
	};
	struct dentry *mntroot = ERR_PTR(-ENOMEM);
	struct nfs_subversion *nfs_mod;
	int error;

	mount_info.parsed = nfs_alloc_parsed_mount_data();
	mount_info.mntfh = nfs_alloc_fhandle();
	if (mount_info.parsed == NULL || mount_info.mntfh == NULL)
		goto out;

	/* Validate the mount data */
	error = nfs_validate_mount_data(fs_type, raw_data, mount_info.parsed, mount_info.mntfh, dev_name);
	if (error == NFS_TEXT_DATA)
		error = nfs_validate_text_mount_data(raw_data, mount_info.parsed, dev_name);
	if (error < 0) {
		mntroot = ERR_PTR(error);
		goto out;
	}

	nfs_mod = get_nfs_version(mount_info.parsed->version);
	if (IS_ERR(nfs_mod)) {
		mntroot = ERR_CAST(nfs_mod);
		goto out;
	}

	mntroot = nfs_mod->rpc_ops->try_mount(flags, dev_name, &mount_info, nfs_mod);

	put_nfs_version(nfs_mod);
out:
	nfs_free_parsed_mount_data(mount_info.parsed);
	nfs_free_fhandle(mount_info.mntfh);
	return mntroot;
}
EXPORT_SYMBOL_GPL(nfs_fs_mount);
>>>>>>> refs/remotes/origin/master

/*
 * Ensure that we unregister the bdi before kill_anon_super
 * releases the device name
 */
<<<<<<< HEAD
static void nfs_put_super(struct super_block *s)
=======
void nfs_put_super(struct super_block *s)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_server *server = NFS_SB(s);

	bdi_unregister(&server->backing_dev_info);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_put_super);
>>>>>>> refs/remotes/origin/master

/*
 * Destroy an NFS2/3 superblock
 */
<<<<<<< HEAD
static void nfs_kill_super(struct super_block *s)
=======
void nfs_kill_super(struct super_block *s)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_server *server = NFS_SB(s);

	kill_anon_super(s);
	nfs_fscache_release_super_cookie(s);
	nfs_free_server(server);
}
<<<<<<< HEAD

/*
 * Clone an NFS2/3 server record on xdev traversal (FSID-change)
=======
EXPORT_SYMBOL_GPL(nfs_kill_super);

/*
 * Clone an NFS2/3/4 server record on xdev traversal (FSID-change)
>>>>>>> refs/remotes/origin/master
 */
static struct dentry *
nfs_xdev_mount(struct file_system_type *fs_type, int flags,
		const char *dev_name, void *raw_data)
{
	struct nfs_clone_mount *data = raw_data;
<<<<<<< HEAD
	struct super_block *s;
	struct nfs_server *server;
	struct dentry *mntroot;
	int (*compare_super)(struct super_block *, void *) = nfs_compare_super;
	struct nfs_sb_mountdata sb_mntdata = {
		.mntflags = flags,
	};
	int error;

	dprintk("--> nfs_xdev_mount()\n");

	/* create a new volume representation */
<<<<<<< HEAD
	server = nfs_clone_server(NFS_SB(data->sb), data->fh, data->fattr);
=======
	server = nfs_clone_server(NFS_SB(data->sb), data->fh, data->fattr, data->authflavor);
>>>>>>> refs/remotes/origin/cm-10.0
	if (IS_ERR(server)) {
		error = PTR_ERR(server);
		goto out_err_noserver;
	}
	sb_mntdata.server = server;

	if (server->flags & NFS_MOUNT_UNSHARED)
		compare_super = NULL;

<<<<<<< HEAD
=======
	/* -o noac implies -o sync */
	if (server->flags & NFS_MOUNT_NOAC)
		sb_mntdata.mntflags |= MS_SYNCHRONOUS;

>>>>>>> refs/remotes/origin/cm-10.0
	/* Get a superblock - note that we may end up sharing one that already exists */
	s = sget(&nfs_fs_type, compare_super, nfs_set_super, &sb_mntdata);
	if (IS_ERR(s)) {
		error = PTR_ERR(s);
		goto out_err_nosb;
	}

	if (s->s_fs_info != server) {
		nfs_free_server(server);
		server = NULL;
	} else {
		error = nfs_bdi_register(server);
		if (error)
			goto error_splat_bdi;
	}

	if (!s->s_root) {
		/* initial superblock/root creation */
		nfs_clone_super(s, data->sb);
		nfs_fscache_get_super_cookie(s, NULL, data);
	}

	mntroot = nfs_get_root(s, data->fh, dev_name);
	if (IS_ERR(mntroot)) {
		error = PTR_ERR(mntroot);
		goto error_splat_super;
	}
	if (mntroot->d_inode->i_op != NFS_SB(s)->nfs_client->rpc_ops->dir_inode_ops) {
		dput(mntroot);
		error = -ESTALE;
		goto error_splat_super;
	}

	s->s_flags |= MS_ACTIVE;

	/* clone any lsm security options from the parent to the new sb */
	security_sb_clone_mnt_opts(data->sb, s);

	dprintk("<-- nfs_xdev_mount() = 0\n");
	return mntroot;

out_err_nosb:
	nfs_free_server(server);
out_err_noserver:
	dprintk("<-- nfs_xdev_mount() = %d [error]\n", error);
	return ERR_PTR(error);

error_splat_super:
	if (server && !s->s_root)
		bdi_unregister(&server->backing_dev_info);
error_splat_bdi:
	deactivate_locked_super(s);
	dprintk("<-- nfs_xdev_mount() = %d [splat]\n", error);
	return ERR_PTR(error);
}

#ifdef CONFIG_NFS_V4

/*
 * Finish setting up a cloned NFS4 superblock
 */
static void nfs4_clone_super(struct super_block *sb,
			    const struct super_block *old_sb)
{
	sb->s_blocksize_bits = old_sb->s_blocksize_bits;
	sb->s_blocksize = old_sb->s_blocksize;
	sb->s_maxbytes = old_sb->s_maxbytes;
	sb->s_time_gran = 1;
	sb->s_op = old_sb->s_op;
	/*
	 * The VFS shouldn't apply the umask to mode bits. We will do
	 * so ourselves when necessary.
	 */
	sb->s_flags  |= MS_POSIXACL;
	sb->s_xattr  = old_sb->s_xattr;
	nfs_initialise_sb(sb);
}

/*
 * Set up an NFS4 superblock
 */
static void nfs4_fill_super(struct super_block *sb)
{
	sb->s_time_gran = 1;
	sb->s_op = &nfs4_sops;
	/*
	 * The VFS shouldn't apply the umask to mode bits. We will do
	 * so ourselves when necessary.
	 */
	sb->s_flags  |= MS_POSIXACL;
	sb->s_xattr = nfs4_xattr_handlers;
	nfs_initialise_sb(sb);
}
=======
	struct nfs_mount_info mount_info = {
		.fill_super = nfs_clone_super,
		.set_security = nfs_clone_sb_security,
		.cloned = data,
	};
	struct nfs_server *server;
	struct dentry *mntroot = ERR_PTR(-ENOMEM);
	struct nfs_subversion *nfs_mod = NFS_SB(data->sb)->nfs_client->cl_nfs_mod;

	dprintk("--> nfs_xdev_mount()\n");

	mount_info.mntfh = mount_info.cloned->fh;

	/* create a new volume representation */
	server = nfs_mod->rpc_ops->clone_server(NFS_SB(data->sb), data->fh, data->fattr, data->authflavor);

	if (IS_ERR(server))
		mntroot = ERR_CAST(server);
	else
		mntroot = nfs_fs_mount_common(server, flags,
				dev_name, &mount_info, nfs_mod);

	dprintk("<-- nfs_xdev_mount() = %ld\n",
			IS_ERR(mntroot) ? PTR_ERR(mntroot) : 0L);
	return mntroot;
}

#if IS_ENABLED(CONFIG_NFS_V4)
>>>>>>> refs/remotes/origin/master

static void nfs4_validate_mount_flags(struct nfs_parsed_mount_data *args)
{
	args->flags &= ~(NFS_MOUNT_NONLM|NFS_MOUNT_NOACL|NFS_MOUNT_VER3|
			 NFS_MOUNT_LOCAL_FLOCK|NFS_MOUNT_LOCAL_FCNTL);
}

<<<<<<< HEAD
static int nfs4_validate_text_mount_data(void *options,
					 struct nfs_parsed_mount_data *args,
					 const char *dev_name)
{
	struct sockaddr *sap = (struct sockaddr *)&args->nfs_server.address;

	nfs_set_port(sap, &args->nfs_server.port, NFS_PORT);

	nfs_validate_transport_protocol(args);

	nfs4_validate_mount_flags(args);

	if (args->version != 4) {
		dfprintk(MOUNT,
			 "NFS4: Illegal mount version\n");
		return -EINVAL;
	}

	if (args->auth_flavor_len > 1) {
		dfprintk(MOUNT,
			 "NFS4: Too many RPC auth flavours specified\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (args->client_address == NULL) {
		dfprintk(MOUNT,
			 "NFS4: mount program didn't pass callback address\n");
		return -EINVAL;
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
	return nfs_parse_devname(dev_name,
				   &args->nfs_server.hostname,
				   NFS4_MAXNAMLEN,
				   &args->nfs_server.export_path,
				   NFS4_MAXPATHLEN);
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * Validate NFSv4 mount options
 */
static int nfs4_validate_mount_data(void *options,
				    struct nfs_parsed_mount_data *args,
				    const char *dev_name)
{
	struct sockaddr *sap = (struct sockaddr *)&args->nfs_server.address;
	struct nfs4_mount_data *data = (struct nfs4_mount_data *)options;
	char *c;

	if (data == NULL)
		goto out_no_data;

<<<<<<< HEAD
=======
	args->version = 4;

>>>>>>> refs/remotes/origin/master
	switch (data->version) {
	case 1:
		if (data->host_addrlen > sizeof(args->nfs_server.address))
			goto out_no_address;
		if (data->host_addrlen == 0)
			goto out_no_address;
		args->nfs_server.addrlen = data->host_addrlen;
		if (copy_from_user(sap, data->host_addr, data->host_addrlen))
			return -EFAULT;
		if (!nfs_verify_server_address(sap))
			goto out_no_address;
		args->nfs_server.port = ntohs(((struct sockaddr_in *)sap)->sin_port);

		if (data->auth_flavourlen) {
<<<<<<< HEAD
			if (data->auth_flavourlen > 1)
				goto out_inval_auth;
			if (copy_from_user(&args->auth_flavors[0],
					   data->auth_flavours,
					   sizeof(args->auth_flavors[0])))
				return -EFAULT;
		}
=======
			rpc_authflavor_t pseudoflavor;
			if (data->auth_flavourlen > 1)
				goto out_inval_auth;
			if (copy_from_user(&pseudoflavor,
					   data->auth_flavours,
					   sizeof(pseudoflavor)))
				return -EFAULT;
			args->selected_flavor = pseudoflavor;
		} else
			args->selected_flavor = RPC_AUTH_UNIX;
>>>>>>> refs/remotes/origin/master

		c = strndup_user(data->hostname.data, NFS4_MAXNAMLEN);
		if (IS_ERR(c))
			return PTR_ERR(c);
		args->nfs_server.hostname = c;

		c = strndup_user(data->mnt_path.data, NFS4_MAXPATHLEN);
		if (IS_ERR(c))
			return PTR_ERR(c);
		args->nfs_server.export_path = c;
		dfprintk(MOUNT, "NFS: MNTPATH: '%s'\n", c);

		c = strndup_user(data->client_addr.data, 16);
		if (IS_ERR(c))
			return PTR_ERR(c);
		args->client_address = c;

		/*
		 * Translate to nfs_parsed_mount_data, which nfs4_fill_super
		 * can deal with.
		 */

		args->flags	= data->flags & NFS4_MOUNT_FLAGMASK;
		args->rsize	= data->rsize;
		args->wsize	= data->wsize;
		args->timeo	= data->timeo;
		args->retrans	= data->retrans;
		args->acregmin	= data->acregmin;
		args->acregmax	= data->acregmax;
		args->acdirmin	= data->acdirmin;
		args->acdirmax	= data->acdirmax;
		args->nfs_server.protocol = data->proto;
		nfs_validate_transport_protocol(args);
<<<<<<< HEAD

		break;
	default:
		if (nfs_parse_mount_options((char *)options, args) == 0)
			return -EINVAL;

		if (!nfs_verify_server_address(sap))
			return -EINVAL;

		return nfs4_validate_text_mount_data(options, args, dev_name);
=======
		if (args->nfs_server.protocol == XPRT_TRANSPORT_UDP)
			goto out_invalid_transport_udp;

		break;
	default:
		return NFS_TEXT_DATA;
>>>>>>> refs/remotes/origin/master
	}

	return 0;

out_no_data:
	dfprintk(MOUNT, "NFS4: mount program didn't pass any mount data\n");
	return -EINVAL;

out_inval_auth:
	dfprintk(MOUNT, "NFS4: Invalid number of RPC auth flavours %d\n",
		 data->auth_flavourlen);
	return -EINVAL;

out_no_address:
	dfprintk(MOUNT, "NFS4: mount program didn't pass remote address\n");
	return -EINVAL;
<<<<<<< HEAD
}

/*
 * Get the superblock for the NFS4 root partition
 */
static struct dentry *
nfs4_remote_mount(struct file_system_type *fs_type, int flags,
		  const char *dev_name, void *raw_data)
{
	struct nfs_parsed_mount_data *data = raw_data;
	struct super_block *s;
	struct nfs_server *server;
	struct nfs_fh *mntfh;
	struct dentry *mntroot;
	int (*compare_super)(struct super_block *, void *) = nfs_compare_super;
	struct nfs_sb_mountdata sb_mntdata = {
		.mntflags = flags,
	};
	int error = -ENOMEM;

	mntfh = nfs_alloc_fhandle();
	if (data == NULL || mntfh == NULL)
		goto out;

	/* Get a volume representation */
	server = nfs4_create_server(data, mntfh);
	if (IS_ERR(server)) {
		error = PTR_ERR(server);
		goto out;
	}
	sb_mntdata.server = server;

	if (server->flags & NFS4_MOUNT_UNSHARED)
		compare_super = NULL;

<<<<<<< HEAD
=======
	/* -o noac implies -o sync */
	if (server->flags & NFS_MOUNT_NOAC)
		sb_mntdata.mntflags |= MS_SYNCHRONOUS;

>>>>>>> refs/remotes/origin/cm-10.0
	/* Get a superblock - note that we may end up sharing one that already exists */
	s = sget(&nfs4_fs_type, compare_super, nfs_set_super, &sb_mntdata);
	if (IS_ERR(s)) {
		error = PTR_ERR(s);
		goto out_free;
	}

	if (s->s_fs_info != server) {
		nfs_free_server(server);
		server = NULL;
	} else {
		error = nfs_bdi_register(server);
		if (error)
			goto error_splat_bdi;
	}

	if (!s->s_root) {
		/* initial superblock/root creation */
		nfs4_fill_super(s);
<<<<<<< HEAD
		nfs_fscache_get_super_cookie(
			s, data ? data->fscache_uniq : NULL, NULL);
=======
		nfs_fscache_get_super_cookie(s, data->fscache_uniq, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	mntroot = nfs4_get_root(s, mntfh, dev_name);
	if (IS_ERR(mntroot)) {
		error = PTR_ERR(mntroot);
		goto error_splat_super;
	}

	error = security_sb_set_mnt_opts(s, &data->lsm_opts);
	if (error)
		goto error_splat_root;

	s->s_flags |= MS_ACTIVE;

	nfs_free_fhandle(mntfh);
	return mntroot;

out:
	nfs_free_fhandle(mntfh);
	return ERR_PTR(error);

out_free:
	nfs_free_server(server);
	goto out;

error_splat_root:
	dput(mntroot);
error_splat_super:
	if (server && !s->s_root)
		bdi_unregister(&server->backing_dev_info);
error_splat_bdi:
	deactivate_locked_super(s);
	goto out;
}

static struct vfsmount *nfs_do_root_mount(struct file_system_type *fs_type,
		int flags, void *data, const char *hostname)
{
	struct vfsmount *root_mnt;
	char *root_devname;
	size_t len;

	len = strlen(hostname) + 5;
	root_devname = kmalloc(len, GFP_KERNEL);
	if (root_devname == NULL)
		return ERR_PTR(-ENOMEM);
	/* Does hostname needs to be enclosed in brackets? */
	if (strchr(hostname, ':'))
		snprintf(root_devname, len, "[%s]:/", hostname);
	else
		snprintf(root_devname, len, "%s:/", hostname);
	root_mnt = vfs_kern_mount(fs_type, flags, root_devname, data);
	kfree(root_devname);
	return root_mnt;
}

struct nfs_referral_count {
	struct list_head list;
	const struct task_struct *task;
	unsigned int referral_count;
};

static LIST_HEAD(nfs_referral_count_list);
static DEFINE_SPINLOCK(nfs_referral_count_list_lock);

static struct nfs_referral_count *nfs_find_referral_count(void)
{
	struct nfs_referral_count *p;

	list_for_each_entry(p, &nfs_referral_count_list, list) {
		if (p->task == current)
			return p;
	}
	return NULL;
}

#define NFS_MAX_NESTED_REFERRALS 2

static int nfs_referral_loop_protect(void)
{
	struct nfs_referral_count *p, *new;
	int ret = -ENOMEM;

	new = kmalloc(sizeof(*new), GFP_KERNEL);
	if (!new)
		goto out;
	new->task = current;
	new->referral_count = 1;

	ret = 0;
	spin_lock(&nfs_referral_count_list_lock);
	p = nfs_find_referral_count();
	if (p != NULL) {
		if (p->referral_count >= NFS_MAX_NESTED_REFERRALS)
			ret = -ELOOP;
		else
			p->referral_count++;
	} else {
		list_add(&new->list, &nfs_referral_count_list);
		new = NULL;
	}
	spin_unlock(&nfs_referral_count_list_lock);
	kfree(new);
out:
	return ret;
}

static void nfs_referral_loop_unprotect(void)
{
	struct nfs_referral_count *p;

	spin_lock(&nfs_referral_count_list_lock);
	p = nfs_find_referral_count();
	p->referral_count--;
	if (p->referral_count == 0)
		list_del(&p->list);
	else
		p = NULL;
	spin_unlock(&nfs_referral_count_list_lock);
	kfree(p);
}

static struct dentry *nfs_follow_remote_path(struct vfsmount *root_mnt,
		const char *export_path)
{
<<<<<<< HEAD
	struct nameidata *nd = NULL;
	struct mnt_namespace *ns_private;
	struct super_block *s;
	struct dentry *dentry;
	int ret;

	nd = kmalloc(sizeof(*nd), GFP_KERNEL);
	if (nd == NULL)
		return ERR_PTR(-ENOMEM);

	ns_private = create_mnt_ns(root_mnt);
	ret = PTR_ERR(ns_private);
	if (IS_ERR(ns_private))
		goto out_mntput;

	ret = nfs_referral_loop_protect();
	if (ret != 0)
		goto out_put_mnt_ns;

	ret = vfs_path_lookup(root_mnt->mnt_root, root_mnt,
			export_path, LOOKUP_FOLLOW|LOOKUP_AUTOMOUNT, nd);

	nfs_referral_loop_unprotect();
	put_mnt_ns(ns_private);

	if (ret != 0)
		goto out_err;

	s = nd->path.mnt->mnt_sb;
	atomic_inc(&s->s_active);
	dentry = dget(nd->path.dentry);

	path_put(&nd->path);
	kfree(nd);
	down_write(&s->s_umount);
	return dentry;
out_put_mnt_ns:
	put_mnt_ns(ns_private);
out_mntput:
	mntput(root_mnt);
out_err:
	kfree(nd);
	return ERR_PTR(ret);
=======
	struct dentry *dentry;
	int err;

	if (IS_ERR(root_mnt))
		return ERR_CAST(root_mnt);

	err = nfs_referral_loop_protect();
	if (err) {
		mntput(root_mnt);
		return ERR_PTR(err);
	}

	dentry = mount_subtree(root_mnt, export_path);
	nfs_referral_loop_unprotect();

	return dentry;
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct dentry *nfs4_try_mount(int flags, const char *dev_name,
			 struct nfs_parsed_mount_data *data)
{
	char *export_path;
	struct vfsmount *root_mnt;
	struct dentry *res;

	dfprintk(MOUNT, "--> nfs4_try_mount()\n");

	export_path = data->nfs_server.export_path;
	data->nfs_server.export_path = "/";
	root_mnt = nfs_do_root_mount(&nfs4_remote_fs_type, flags, data,
			data->nfs_server.hostname);
	data->nfs_server.export_path = export_path;

<<<<<<< HEAD
	res = ERR_CAST(root_mnt);
	if (!IS_ERR(root_mnt))
		res = nfs_follow_remote_path(root_mnt, export_path);
=======
	res = nfs_follow_remote_path(root_mnt, export_path);
>>>>>>> refs/remotes/origin/cm-10.0

	dfprintk(MOUNT, "<-- nfs4_try_mount() = %ld%s\n",
			IS_ERR(res) ? PTR_ERR(res) : 0,
			IS_ERR(res) ? " [error]" : "");
	return res;
}

/*
 * Get the superblock for an NFS4 mountpoint
 */
static struct dentry *nfs4_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *raw_data)
{
	struct nfs_parsed_mount_data *data;
	int error = -ENOMEM;
	struct dentry *res = ERR_PTR(-ENOMEM);

	data = nfs_alloc_parsed_mount_data(4);
	if (data == NULL)
		goto out;

	/* Validate the mount data */
	error = nfs4_validate_mount_data(raw_data, data, dev_name);
	if (error < 0) {
		res = ERR_PTR(error);
		goto out;
	}

	res = nfs4_try_mount(flags, dev_name, data);
	if (IS_ERR(res))
		error = PTR_ERR(res);

out:
	nfs_free_parsed_mount_data(data);
	dprintk("<-- nfs4_mount() = %d%s\n", error,
			error != 0 ? " [error]" : "");
	return res;
}

static void nfs4_kill_super(struct super_block *sb)
{
	struct nfs_server *server = NFS_SB(sb);

	dprintk("--> %s\n", __func__);
	nfs_super_return_all_delegations(sb);
	kill_anon_super(sb);
	nfs_fscache_release_super_cookie(sb);
	nfs_free_server(server);
	dprintk("<-- %s\n", __func__);
}

/*
 * Clone an NFS4 server record on xdev traversal (FSID-change)
 */
static struct dentry *
nfs4_xdev_mount(struct file_system_type *fs_type, int flags,
		 const char *dev_name, void *raw_data)
{
	struct nfs_clone_mount *data = raw_data;
	struct super_block *s;
	struct nfs_server *server;
	struct dentry *mntroot;
	int (*compare_super)(struct super_block *, void *) = nfs_compare_super;
	struct nfs_sb_mountdata sb_mntdata = {
		.mntflags = flags,
	};
	int error;

	dprintk("--> nfs4_xdev_mount()\n");

	/* create a new volume representation */
<<<<<<< HEAD
	server = nfs_clone_server(NFS_SB(data->sb), data->fh, data->fattr);
=======
	server = nfs_clone_server(NFS_SB(data->sb), data->fh, data->fattr, data->authflavor);
>>>>>>> refs/remotes/origin/cm-10.0
	if (IS_ERR(server)) {
		error = PTR_ERR(server);
		goto out_err_noserver;
	}
	sb_mntdata.server = server;

	if (server->flags & NFS4_MOUNT_UNSHARED)
		compare_super = NULL;

<<<<<<< HEAD
=======
	/* -o noac implies -o sync */
	if (server->flags & NFS_MOUNT_NOAC)
		sb_mntdata.mntflags |= MS_SYNCHRONOUS;

>>>>>>> refs/remotes/origin/cm-10.0
	/* Get a superblock - note that we may end up sharing one that already exists */
	s = sget(&nfs4_fs_type, compare_super, nfs_set_super, &sb_mntdata);
	if (IS_ERR(s)) {
		error = PTR_ERR(s);
		goto out_err_nosb;
	}

	if (s->s_fs_info != server) {
		nfs_free_server(server);
		server = NULL;
	} else {
		error = nfs_bdi_register(server);
		if (error)
			goto error_splat_bdi;
	}

	if (!s->s_root) {
		/* initial superblock/root creation */
		nfs4_clone_super(s, data->sb);
		nfs_fscache_get_super_cookie(s, NULL, data);
	}

	mntroot = nfs4_get_root(s, data->fh, dev_name);
	if (IS_ERR(mntroot)) {
		error = PTR_ERR(mntroot);
		goto error_splat_super;
	}
	if (mntroot->d_inode->i_op != NFS_SB(s)->nfs_client->rpc_ops->dir_inode_ops) {
		dput(mntroot);
		error = -ESTALE;
		goto error_splat_super;
	}

	s->s_flags |= MS_ACTIVE;

	security_sb_clone_mnt_opts(data->sb, s);

	dprintk("<-- nfs4_xdev_mount() = 0\n");
	return mntroot;

out_err_nosb:
	nfs_free_server(server);
out_err_noserver:
	dprintk("<-- nfs4_xdev_mount() = %d [error]\n", error);
	return ERR_PTR(error);

error_splat_super:
	if (server && !s->s_root)
		bdi_unregister(&server->backing_dev_info);
error_splat_bdi:
	deactivate_locked_super(s);
	dprintk("<-- nfs4_xdev_mount() = %d [splat]\n", error);
	return ERR_PTR(error);
}

static struct dentry *
nfs4_remote_referral_mount(struct file_system_type *fs_type, int flags,
			   const char *dev_name, void *raw_data)
{
	struct nfs_clone_mount *data = raw_data;
	struct super_block *s;
	struct nfs_server *server;
	struct dentry *mntroot;
	struct nfs_fh *mntfh;
	int (*compare_super)(struct super_block *, void *) = nfs_compare_super;
	struct nfs_sb_mountdata sb_mntdata = {
		.mntflags = flags,
	};
	int error = -ENOMEM;

	dprintk("--> nfs4_referral_get_sb()\n");

	mntfh = nfs_alloc_fhandle();
	if (mntfh == NULL)
		goto out_err_nofh;

	/* create a new volume representation */
	server = nfs4_create_referral_server(data, mntfh);
	if (IS_ERR(server)) {
		error = PTR_ERR(server);
		goto out_err_noserver;
	}
	sb_mntdata.server = server;

	if (server->flags & NFS4_MOUNT_UNSHARED)
		compare_super = NULL;

<<<<<<< HEAD
=======
	/* -o noac implies -o sync */
	if (server->flags & NFS_MOUNT_NOAC)
		sb_mntdata.mntflags |= MS_SYNCHRONOUS;

>>>>>>> refs/remotes/origin/cm-10.0
	/* Get a superblock - note that we may end up sharing one that already exists */
	s = sget(&nfs4_fs_type, compare_super, nfs_set_super, &sb_mntdata);
	if (IS_ERR(s)) {
		error = PTR_ERR(s);
		goto out_err_nosb;
	}

	if (s->s_fs_info != server) {
		nfs_free_server(server);
		server = NULL;
	} else {
		error = nfs_bdi_register(server);
		if (error)
			goto error_splat_bdi;
	}

	if (!s->s_root) {
		/* initial superblock/root creation */
		nfs4_fill_super(s);
		nfs_fscache_get_super_cookie(s, NULL, data);
	}

	mntroot = nfs4_get_root(s, mntfh, dev_name);
	if (IS_ERR(mntroot)) {
		error = PTR_ERR(mntroot);
		goto error_splat_super;
	}
	if (mntroot->d_inode->i_op != NFS_SB(s)->nfs_client->rpc_ops->dir_inode_ops) {
		dput(mntroot);
		error = -ESTALE;
		goto error_splat_super;
	}

	s->s_flags |= MS_ACTIVE;

	security_sb_clone_mnt_opts(data->sb, s);

	nfs_free_fhandle(mntfh);
	dprintk("<-- nfs4_referral_get_sb() = 0\n");
	return mntroot;

out_err_nosb:
	nfs_free_server(server);
out_err_noserver:
	nfs_free_fhandle(mntfh);
out_err_nofh:
	dprintk("<-- nfs4_referral_get_sb() = %d [error]\n", error);
	return ERR_PTR(error);

error_splat_super:
	if (server && !s->s_root)
		bdi_unregister(&server->backing_dev_info);
error_splat_bdi:
	deactivate_locked_super(s);
	nfs_free_fhandle(mntfh);
	dprintk("<-- nfs4_referral_get_sb() = %d [splat]\n", error);
	return ERR_PTR(error);
}

/*
 * Create an NFS4 server record on referral traversal
 */
static struct dentry *nfs4_referral_mount(struct file_system_type *fs_type,
		int flags, const char *dev_name, void *raw_data)
{
	struct nfs_clone_mount *data = raw_data;
	char *export_path;
	struct vfsmount *root_mnt;
	struct dentry *res;

	dprintk("--> nfs4_referral_mount()\n");

	export_path = data->mnt_path;
	data->mnt_path = "/";

	root_mnt = nfs_do_root_mount(&nfs4_remote_referral_fs_type,
			flags, data, data->hostname);
	data->mnt_path = export_path;

<<<<<<< HEAD
	res = ERR_CAST(root_mnt);
	if (!IS_ERR(root_mnt))
		res = nfs_follow_remote_path(root_mnt, export_path);
=======
	res = nfs_follow_remote_path(root_mnt, export_path);
>>>>>>> refs/remotes/origin/cm-10.0
	dprintk("<-- nfs4_referral_mount() = %ld%s\n",
			IS_ERR(res) ? PTR_ERR(res) : 0,
			IS_ERR(res) ? " [error]" : "");
	return res;
}

MODULE_ALIAS("nfs4");
=======

out_invalid_transport_udp:
	dfprintk(MOUNT, "NFSv4: Unsupported transport protocol udp\n");
	return -EINVAL;
}

/*
 * NFS v4 module parameters need to stay in the
 * NFS client for backwards compatibility
 */
unsigned int nfs_callback_set_tcpport;
unsigned short nfs_callback_tcpport;
/* Default cache timeout is 10 minutes */
unsigned int nfs_idmap_cache_timeout = 600;
/* Turn off NFSv4 uid/gid mapping when using AUTH_SYS */
bool nfs4_disable_idmapping = true;
unsigned short max_session_slots = NFS4_DEF_SLOT_TABLE_SIZE;
unsigned short send_implementation_id = 1;
char nfs4_client_id_uniquifier[NFS4_CLIENT_ID_UNIQ_LEN] = "";
bool recover_lost_locks = false;

EXPORT_SYMBOL_GPL(nfs_callback_set_tcpport);
EXPORT_SYMBOL_GPL(nfs_callback_tcpport);
EXPORT_SYMBOL_GPL(nfs_idmap_cache_timeout);
EXPORT_SYMBOL_GPL(nfs4_disable_idmapping);
EXPORT_SYMBOL_GPL(max_session_slots);
EXPORT_SYMBOL_GPL(send_implementation_id);
EXPORT_SYMBOL_GPL(nfs4_client_id_uniquifier);
EXPORT_SYMBOL_GPL(recover_lost_locks);

#define NFS_CALLBACK_MAXPORTNR (65535U)

static int param_set_portnr(const char *val, const struct kernel_param *kp)
{
	unsigned long num;
	int ret;

	if (!val)
		return -EINVAL;
	ret = kstrtoul(val, 0, &num);
	if (ret == -EINVAL || num > NFS_CALLBACK_MAXPORTNR)
		return -EINVAL;
	*((unsigned int *)kp->arg) = num;
	return 0;
}
static struct kernel_param_ops param_ops_portnr = {
	.set = param_set_portnr,
	.get = param_get_uint,
};
#define param_check_portnr(name, p) __param_check(name, p, unsigned int);

module_param_named(callback_tcpport, nfs_callback_set_tcpport, portnr, 0644);
module_param(nfs_idmap_cache_timeout, int, 0644);
module_param(nfs4_disable_idmapping, bool, 0644);
module_param_string(nfs4_unique_id, nfs4_client_id_uniquifier,
			NFS4_CLIENT_ID_UNIQ_LEN, 0600);
MODULE_PARM_DESC(nfs4_disable_idmapping,
		"Turn off NFSv4 idmapping when using 'sec=sys'");
module_param(max_session_slots, ushort, 0644);
MODULE_PARM_DESC(max_session_slots, "Maximum number of outstanding NFSv4.1 "
		"requests the client will negotiate");
module_param(send_implementation_id, ushort, 0644);
MODULE_PARM_DESC(send_implementation_id,
		"Send implementation ID with NFSv4.1 exchange_id");
MODULE_PARM_DESC(nfs4_unique_id, "nfs_client_id4 uniquifier string");

module_param(recover_lost_locks, bool, 0644);
MODULE_PARM_DESC(recover_lost_locks,
		 "If the server reports that a lock might be lost, "
		 "try to recover it risking data corruption.");

>>>>>>> refs/remotes/origin/master

#endif /* CONFIG_NFS_V4 */
