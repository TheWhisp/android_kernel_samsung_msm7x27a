#ifndef _LINUX_SUNRPC_RPC_PIPE_FS_H
#define _LINUX_SUNRPC_RPC_PIPE_FS_H

#ifdef __KERNEL__

#include <linux/workqueue.h>

<<<<<<< HEAD
=======
struct rpc_pipe_dir_head {
	struct list_head pdh_entries;
	struct dentry *pdh_dentry;
};

struct rpc_pipe_dir_object_ops;
struct rpc_pipe_dir_object {
	struct list_head pdo_head;
	const struct rpc_pipe_dir_object_ops *pdo_ops;

	void *pdo_data;
};

struct rpc_pipe_dir_object_ops {
	int (*create)(struct dentry *dir,
			struct rpc_pipe_dir_object *pdo);
	void (*destroy)(struct dentry *dir,
			struct rpc_pipe_dir_object *pdo);
};

>>>>>>> refs/remotes/origin/master
struct rpc_pipe_msg {
	struct list_head list;
	void *data;
	size_t len;
	size_t copied;
	int errno;
};

struct rpc_pipe_ops {
	ssize_t (*upcall)(struct file *, struct rpc_pipe_msg *, char __user *, size_t);
	ssize_t (*downcall)(struct file *, const char __user *, size_t);
	void (*release_pipe)(struct inode *);
	int (*open_pipe)(struct inode *);
	void (*destroy_msg)(struct rpc_pipe_msg *);
};

<<<<<<< HEAD
<<<<<<< HEAD
struct rpc_inode {
	struct inode vfs_inode;
	void *private;
=======
struct rpc_pipe {
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct rpc_pipe {
>>>>>>> refs/remotes/origin/master
	struct list_head pipe;
	struct list_head in_upcall;
	struct list_head in_downcall;
	int pipelen;
	int nreaders;
	int nwriters;
<<<<<<< HEAD
<<<<<<< HEAD
	int nkern_readwriters;
	wait_queue_head_t waitq;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define RPC_PIPE_WAIT_FOR_OPEN	1
	int flags;
	struct delayed_work queue_timeout;
	const struct rpc_pipe_ops *ops;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	spinlock_t lock;
	struct dentry *dentry;
};

struct rpc_inode {
	struct inode vfs_inode;
	void *private;
	struct rpc_pipe *pipe;
	wait_queue_head_t waitq;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static inline struct rpc_inode *
RPC_I(struct inode *inode)
{
	return container_of(inode, struct rpc_inode, vfs_inode);
}

<<<<<<< HEAD
<<<<<<< HEAD
extern int rpc_queue_upcall(struct inode *, struct rpc_pipe_msg *);
=======
=======
>>>>>>> refs/remotes/origin/master
enum {
	SUNRPC_PIPEFS_NFS_PRIO,
	SUNRPC_PIPEFS_RPC_PRIO,
};

extern int rpc_pipefs_notifier_register(struct notifier_block *);
extern void rpc_pipefs_notifier_unregister(struct notifier_block *);

enum {
	RPC_PIPEFS_MOUNT,
	RPC_PIPEFS_UMOUNT,
};

extern struct dentry *rpc_d_lookup_sb(const struct super_block *sb,
				      const unsigned char *dir_name);
extern void rpc_pipefs_init_net(struct net *net);
extern struct super_block *rpc_get_sb_net(const struct net *net);
extern void rpc_put_sb_net(const struct net *net);

extern ssize_t rpc_pipe_generic_upcall(struct file *, struct rpc_pipe_msg *,
				       char __user *, size_t);
extern int rpc_queue_upcall(struct rpc_pipe *, struct rpc_pipe_msg *);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

struct rpc_clnt;
extern struct dentry *rpc_create_client_dir(struct dentry *, struct qstr *, struct rpc_clnt *);
extern int rpc_remove_client_dir(struct dentry *);

struct cache_detail;
extern struct dentry *rpc_create_cache_dir(struct dentry *,
					   struct qstr *,
<<<<<<< HEAD
					   mode_t umode,
					   struct cache_detail *);
extern void rpc_remove_cache_dir(struct dentry *);

extern struct dentry *rpc_mkpipe(struct dentry *, const char *, void *,
				 const struct rpc_pipe_ops *, int flags);
extern int rpc_unlink(struct dentry *);
extern struct vfsmount *rpc_get_mount(void);
extern void rpc_put_mount(void);
=======
=======

struct rpc_clnt;
extern struct dentry *rpc_create_client_dir(struct dentry *, const char *, struct rpc_clnt *);
extern int rpc_remove_client_dir(struct rpc_clnt *);

extern void rpc_init_pipe_dir_head(struct rpc_pipe_dir_head *pdh);
extern void rpc_init_pipe_dir_object(struct rpc_pipe_dir_object *pdo,
		const struct rpc_pipe_dir_object_ops *pdo_ops,
		void *pdo_data);
extern int rpc_add_pipe_dir_object(struct net *net,
		struct rpc_pipe_dir_head *pdh,
		struct rpc_pipe_dir_object *pdo);
extern void rpc_remove_pipe_dir_object(struct net *net,
		struct rpc_pipe_dir_head *pdh,
		struct rpc_pipe_dir_object *pdo);
extern struct rpc_pipe_dir_object *rpc_find_or_alloc_pipe_dir_object(
		struct net *net,
		struct rpc_pipe_dir_head *pdh,
		int (*match)(struct rpc_pipe_dir_object *, void *),
		struct rpc_pipe_dir_object *(*alloc)(void *),
		void *data);

struct cache_detail;
extern struct dentry *rpc_create_cache_dir(struct dentry *,
					   const char *,
>>>>>>> refs/remotes/origin/master
					   umode_t umode,
					   struct cache_detail *);
extern void rpc_remove_cache_dir(struct dentry *);

extern int rpc_rmdir(struct dentry *dentry);

struct rpc_pipe *rpc_mkpipe_data(const struct rpc_pipe_ops *ops, int flags);
void rpc_destroy_pipe_data(struct rpc_pipe *pipe);
extern struct dentry *rpc_mkpipe_dentry(struct dentry *, const char *, void *,
					struct rpc_pipe *);
extern int rpc_unlink(struct dentry *);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
extern int register_rpc_pipefs(void);
extern void unregister_rpc_pipefs(void);

#endif
#endif
