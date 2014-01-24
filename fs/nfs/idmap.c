/*
 * fs/nfs/idmap.c
 *
 *  UID and GID to name mapping for clients.
 *
 *  Copyright (c) 2002 The Regents of the University of Michigan.
 *  All rights reserved.
 *
 *  Marius Aamodt Eriksen <marius@umich.edu>
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the University nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <linux/types.h>
<<<<<<< HEAD
#include <linux/string.h>
#include <linux/kernel.h>
=======
#include <linux/parser.h>
#include <linux/fs.h>
#include <linux/nfs_idmap.h>
#include <net/net_namespace.h>
#include <linux/sunrpc/rpc_pipe_fs.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_fs_sb.h>
#include <linux/key.h>
#include <linux/keyctl.h>
#include <linux/key-type.h>
#include <keys/user-type.h>
#include <linux/module.h>

#include "internal.h"
#include "netns.h"

#define NFS_UINT_MAXLEN 11

/* Default cache timeout is 10 minutes */
unsigned int nfs_idmap_cache_timeout = 600;
static const struct cred *id_resolver_cache;
static struct key_type key_type_id_resolver_legacy;

struct idmap {
	struct rpc_pipe		*idmap_pipe;
	struct key_construction	*idmap_key_cons;
	struct mutex		idmap_mutex;
};

struct idmap_legacy_upcalldata {
	struct rpc_pipe_msg pipe_msg;
	struct idmap_msg idmap_msg;
	struct idmap *idmap;
};

/**
 * nfs_fattr_init_names - initialise the nfs_fattr owner_name/group_name fields
 * @fattr: fully initialised struct nfs_fattr
 * @owner_name: owner name string cache
 * @group_name: group name string cache
 */
void nfs_fattr_init_names(struct nfs_fattr *fattr,
		struct nfs4_string *owner_name,
		struct nfs4_string *group_name)
{
	fattr->owner_name = owner_name;
	fattr->group_name = group_name;
}

static void nfs_fattr_free_owner_name(struct nfs_fattr *fattr)
{
	fattr->valid &= ~NFS_ATTR_FATTR_OWNER_NAME;
	kfree(fattr->owner_name->data);
}

static void nfs_fattr_free_group_name(struct nfs_fattr *fattr)
{
	fattr->valid &= ~NFS_ATTR_FATTR_GROUP_NAME;
	kfree(fattr->group_name->data);
}

static bool nfs_fattr_map_owner_name(struct nfs_server *server, struct nfs_fattr *fattr)
{
	struct nfs4_string *owner = fattr->owner_name;
	__u32 uid;

	if (!(fattr->valid & NFS_ATTR_FATTR_OWNER_NAME))
		return false;
	if (nfs_map_name_to_uid(server, owner->data, owner->len, &uid) == 0) {
		fattr->uid = uid;
		fattr->valid |= NFS_ATTR_FATTR_OWNER;
	}
	return true;
}

static bool nfs_fattr_map_group_name(struct nfs_server *server, struct nfs_fattr *fattr)
{
	struct nfs4_string *group = fattr->group_name;
	__u32 gid;

	if (!(fattr->valid & NFS_ATTR_FATTR_GROUP_NAME))
		return false;
	if (nfs_map_group_to_gid(server, group->data, group->len, &gid) == 0) {
		fattr->gid = gid;
		fattr->valid |= NFS_ATTR_FATTR_GROUP;
	}
	return true;
}

/**
 * nfs_fattr_free_names - free up the NFSv4 owner and group strings
 * @fattr: a fully initialised nfs_fattr structure
 */
void nfs_fattr_free_names(struct nfs_fattr *fattr)
{
	if (fattr->valid & NFS_ATTR_FATTR_OWNER_NAME)
		nfs_fattr_free_owner_name(fattr);
	if (fattr->valid & NFS_ATTR_FATTR_GROUP_NAME)
		nfs_fattr_free_group_name(fattr);
}

/**
 * nfs_fattr_map_and_free_names - map owner/group strings into uid/gid and free
 * @server: pointer to the filesystem nfs_server structure
 * @fattr: a fully initialised nfs_fattr structure
 *
 * This helper maps the cached NFSv4 owner/group strings in fattr into
 * their numeric uid/gid equivalents, and then frees the cached strings.
 */
void nfs_fattr_map_and_free_names(struct nfs_server *server, struct nfs_fattr *fattr)
{
	if (nfs_fattr_map_owner_name(server, fattr))
		nfs_fattr_free_owner_name(fattr);
	if (nfs_fattr_map_group_name(server, fattr))
		nfs_fattr_free_group_name(fattr);
}
>>>>>>> refs/remotes/origin/cm-10.0

static int nfs_map_string_to_numeric(const char *name, size_t namelen, __u32 *res)
{
	unsigned long val;
	char buf[16];

	if (memchr(name, '@', namelen) != NULL || namelen >= sizeof(buf))
		return 0;
	memcpy(buf, name, namelen);
	buf[namelen] = '\0';
	if (strict_strtoul(buf, 0, &val) != 0)
		return 0;
	*res = val;
	return 1;
}

static int nfs_map_numeric_to_string(__u32 id, char *buf, size_t buflen)
{
	return snprintf(buf, buflen, "%u", id);
}

<<<<<<< HEAD
#ifdef CONFIG_NFS_USE_NEW_IDMAPPER

#include <linux/slab.h>
#include <linux/cred.h>
#include <linux/sunrpc/sched.h>
#include <linux/nfs4.h>
#include <linux/nfs_fs_sb.h>
#include <linux/nfs_idmap.h>
#include <linux/keyctl.h>
#include <linux/key-type.h>
#include <linux/rcupdate.h>
#include <linux/err.h>

#include <keys/user-type.h>

#define NFS_UINT_MAXLEN 11

const struct cred *id_resolver_cache;

struct key_type key_type_id_resolver = {
=======
static struct key_type key_type_id_resolver = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name		= "id_resolver",
	.instantiate	= user_instantiate,
	.match		= user_match,
	.revoke		= user_revoke,
	.destroy	= user_destroy,
	.describe	= user_describe,
	.read		= user_read,
};

<<<<<<< HEAD
int nfs_idmap_init(void)
=======
static int nfs_idmap_init_keyring(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct cred *cred;
	struct key *keyring;
	int ret = 0;

<<<<<<< HEAD
	printk(KERN_NOTICE "Registering the %s key type\n", key_type_id_resolver.name);
=======
	printk(KERN_NOTICE "NFS: Registering the %s key type\n",
		key_type_id_resolver.name);
>>>>>>> refs/remotes/origin/cm-10.0

	cred = prepare_kernel_cred(NULL);
	if (!cred)
		return -ENOMEM;

	keyring = key_alloc(&key_type_keyring, ".id_resolver", 0, 0, cred,
			     (KEY_POS_ALL & ~KEY_POS_SETATTR) |
			     KEY_USR_VIEW | KEY_USR_READ,
			     KEY_ALLOC_NOT_IN_QUOTA);
	if (IS_ERR(keyring)) {
		ret = PTR_ERR(keyring);
		goto failed_put_cred;
	}

	ret = key_instantiate_and_link(keyring, NULL, 0, NULL, NULL);
	if (ret < 0)
		goto failed_put_key;

	ret = register_key_type(&key_type_id_resolver);
	if (ret < 0)
		goto failed_put_key;

<<<<<<< HEAD
=======
	ret = register_key_type(&key_type_id_resolver_legacy);
	if (ret < 0)
		goto failed_reg_legacy;

	set_bit(KEY_FLAG_ROOT_CAN_CLEAR, &keyring->flags);
>>>>>>> refs/remotes/origin/cm-10.0
	cred->thread_keyring = keyring;
	cred->jit_keyring = KEY_REQKEY_DEFL_THREAD_KEYRING;
	id_resolver_cache = cred;
	return 0;

<<<<<<< HEAD
=======
failed_reg_legacy:
	unregister_key_type(&key_type_id_resolver);
>>>>>>> refs/remotes/origin/cm-10.0
failed_put_key:
	key_put(keyring);
failed_put_cred:
	put_cred(cred);
	return ret;
}

<<<<<<< HEAD
void nfs_idmap_quit(void)
{
	key_revoke(id_resolver_cache->thread_keyring);
	unregister_key_type(&key_type_id_resolver);
=======
static void nfs_idmap_quit_keyring(void)
{
	key_revoke(id_resolver_cache->thread_keyring);
	unregister_key_type(&key_type_id_resolver);
	unregister_key_type(&key_type_id_resolver_legacy);
>>>>>>> refs/remotes/origin/cm-10.0
	put_cred(id_resolver_cache);
}

/*
 * Assemble the description to pass to request_key()
 * This function will allocate a new string and update dest to point
 * at it.  The caller is responsible for freeing dest.
 *
 * On error 0 is returned.  Otherwise, the length of dest is returned.
 */
static ssize_t nfs_idmap_get_desc(const char *name, size_t namelen,
				const char *type, size_t typelen, char **desc)
{
	char *cp;
	size_t desclen = typelen + namelen + 2;

	*desc = kmalloc(desclen, GFP_KERNEL);
	if (!*desc)
		return -ENOMEM;

	cp = *desc;
	memcpy(cp, type, typelen);
	cp += typelen;
	*cp++ = ':';

	memcpy(cp, name, namelen);
	cp += namelen;
	*cp = '\0';
	return desclen;
}

<<<<<<< HEAD
static ssize_t nfs_idmap_request_key(const char *name, size_t namelen,
		const char *type, void *data, size_t data_size)
=======
static ssize_t nfs_idmap_request_key(struct key_type *key_type,
				     const char *name, size_t namelen,
				     const char *type, void *data,
				     size_t data_size, struct idmap *idmap)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct cred *saved_cred;
	struct key *rkey;
	char *desc;
	struct user_key_payload *payload;
	ssize_t ret;

	ret = nfs_idmap_get_desc(name, namelen, type, strlen(type), &desc);
	if (ret <= 0)
		goto out;

	saved_cred = override_creds(id_resolver_cache);
<<<<<<< HEAD
	rkey = request_key(&key_type_id_resolver, desc, "");
	revert_creds(saved_cred);
=======
	if (idmap)
		rkey = request_key_with_auxdata(key_type, desc, "", 0, idmap);
	else
		rkey = request_key(&key_type_id_resolver, desc, "");
	revert_creds(saved_cred);

>>>>>>> refs/remotes/origin/cm-10.0
	kfree(desc);
	if (IS_ERR(rkey)) {
		ret = PTR_ERR(rkey);
		goto out;
	}

	rcu_read_lock();
	rkey->perm |= KEY_USR_VIEW;

	ret = key_validate(rkey);
	if (ret < 0)
		goto out_up;

	payload = rcu_dereference(rkey->payload.data);
	if (IS_ERR_OR_NULL(payload)) {
		ret = PTR_ERR(payload);
		goto out_up;
	}

	ret = payload->datalen;
	if (ret > 0 && ret <= data_size)
		memcpy(data, payload->data, ret);
	else
		ret = -EINVAL;

out_up:
	rcu_read_unlock();
	key_put(rkey);
out:
	return ret;
}

<<<<<<< HEAD

/* ID -> Name */
static ssize_t nfs_idmap_lookup_name(__u32 id, const char *type, char *buf, size_t buflen)
=======
static ssize_t nfs_idmap_get_key(const char *name, size_t namelen,
				 const char *type, void *data,
				 size_t data_size, struct idmap *idmap)
{
	ssize_t ret = nfs_idmap_request_key(&key_type_id_resolver,
					    name, namelen, type, data,
					    data_size, NULL);
	if (ret < 0) {
		mutex_lock(&idmap->idmap_mutex);
		ret = nfs_idmap_request_key(&key_type_id_resolver_legacy,
					    name, namelen, type, data,
					    data_size, idmap);
		idmap->idmap_key_cons = NULL;
		mutex_unlock(&idmap->idmap_mutex);
	}
	return ret;
}

/* ID -> Name */
static ssize_t nfs_idmap_lookup_name(__u32 id, const char *type, char *buf,
				     size_t buflen, struct idmap *idmap)
>>>>>>> refs/remotes/origin/cm-10.0
{
	char id_str[NFS_UINT_MAXLEN];
	int id_len;
	ssize_t ret;

	id_len = snprintf(id_str, sizeof(id_str), "%u", id);
<<<<<<< HEAD
	ret = nfs_idmap_request_key(id_str, id_len, type, buf, buflen);
=======
	ret = nfs_idmap_get_key(id_str, id_len, type, buf, buflen, idmap);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		return -EINVAL;
	return ret;
}

/* Name -> ID */
<<<<<<< HEAD
static int nfs_idmap_lookup_id(const char *name, size_t namelen,
				const char *type, __u32 *id)
=======
static int nfs_idmap_lookup_id(const char *name, size_t namelen, const char *type,
			       __u32 *id, struct idmap *idmap)
>>>>>>> refs/remotes/origin/cm-10.0
{
	char id_str[NFS_UINT_MAXLEN];
	long id_long;
	ssize_t data_size;
	int ret = 0;

<<<<<<< HEAD
	data_size = nfs_idmap_request_key(name, namelen, type, id_str, NFS_UINT_MAXLEN);
=======
	data_size = nfs_idmap_get_key(name, namelen, type, id_str, NFS_UINT_MAXLEN, idmap);
>>>>>>> refs/remotes/origin/cm-10.0
	if (data_size <= 0) {
		ret = -EINVAL;
	} else {
		ret = strict_strtol(id_str, 10, &id_long);
		*id = (__u32)id_long;
	}
	return ret;
}

<<<<<<< HEAD
int nfs_map_name_to_uid(const struct nfs_server *server, const char *name, size_t namelen, __u32 *uid)
{
	if (nfs_map_string_to_numeric(name, namelen, uid))
		return 0;
	return nfs_idmap_lookup_id(name, namelen, "uid", uid);
}

int nfs_map_group_to_gid(const struct nfs_server *server, const char *name, size_t namelen, __u32 *gid)
{
	if (nfs_map_string_to_numeric(name, namelen, gid))
		return 0;
	return nfs_idmap_lookup_id(name, namelen, "gid", gid);
}

int nfs_map_uid_to_name(const struct nfs_server *server, __u32 uid, char *buf, size_t buflen)
{
	int ret = -EINVAL;

	if (!(server->caps & NFS_CAP_UIDGID_NOMAP))
		ret = nfs_idmap_lookup_name(uid, "user", buf, buflen);
	if (ret < 0)
		ret = nfs_map_numeric_to_string(uid, buf, buflen);
	return ret;
}
int nfs_map_gid_to_group(const struct nfs_server *server, __u32 gid, char *buf, size_t buflen)
{
	int ret = -EINVAL;

	if (!(server->caps & NFS_CAP_UIDGID_NOMAP))
		ret = nfs_idmap_lookup_name(gid, "group", buf, buflen);
	if (ret < 0)
		ret = nfs_map_numeric_to_string(gid, buf, buflen);
	return ret;
}

#else  /* CONFIG_NFS_USE_NEW_IDMAPPER not defined */

#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/sched.h>

#include <linux/sunrpc/clnt.h>
#include <linux/workqueue.h>
#include <linux/sunrpc/rpc_pipe_fs.h>

#include <linux/nfs_fs.h>

#include <linux/nfs_idmap.h>
#include "nfs4_fs.h"

#define IDMAP_HASH_SZ          128

/* Default cache timeout is 10 minutes */
unsigned int nfs_idmap_cache_timeout = 600 * HZ;

static int param_set_idmap_timeout(const char *val, struct kernel_param *kp)
{
	char *endp;
	int num = simple_strtol(val, &endp, 0);
	int jif = num * HZ;
	if (endp == val || *endp || num < 0 || jif < num)
		return -EINVAL;
	*((int *)kp->arg) = jif;
	return 0;
}

module_param_call(idmap_cache_timeout, param_set_idmap_timeout, param_get_int,
		 &nfs_idmap_cache_timeout, 0644);

struct idmap_hashent {
	unsigned long		ih_expires;
	__u32			ih_id;
	size_t			ih_namelen;
	char			ih_name[IDMAP_NAMESZ];
};

struct idmap_hashtable {
	__u8			h_type;
	struct idmap_hashent	h_entries[IDMAP_HASH_SZ];
};

struct idmap {
	struct dentry		*idmap_dentry;
	wait_queue_head_t	idmap_wq;
	struct idmap_msg	idmap_im;
	struct mutex		idmap_lock;	/* Serializes upcalls */
	struct mutex		idmap_im_lock;	/* Protects the hashtable */
	struct idmap_hashtable	idmap_user_hash;
	struct idmap_hashtable	idmap_group_hash;
};

static ssize_t idmap_pipe_upcall(struct file *, struct rpc_pipe_msg *,
				 char __user *, size_t);
static ssize_t idmap_pipe_downcall(struct file *, const char __user *,
				   size_t);
static void idmap_pipe_destroy_msg(struct rpc_pipe_msg *);

static unsigned int fnvhash32(const void *, size_t);

static const struct rpc_pipe_ops idmap_upcall_ops = {
	.upcall		= idmap_pipe_upcall,
	.downcall	= idmap_pipe_downcall,
	.destroy_msg	= idmap_pipe_destroy_msg,
};
=======
/* idmap classic begins here */
module_param(nfs_idmap_cache_timeout, int, 0644);

enum {
	Opt_find_uid, Opt_find_gid, Opt_find_user, Opt_find_group, Opt_find_err
};

static const match_table_t nfs_idmap_tokens = {
	{ Opt_find_uid, "uid:%s" },
	{ Opt_find_gid, "gid:%s" },
	{ Opt_find_user, "user:%s" },
	{ Opt_find_group, "group:%s" },
	{ Opt_find_err, NULL }
};

static int nfs_idmap_legacy_upcall(struct key_construction *, const char *, void *);
static ssize_t idmap_pipe_downcall(struct file *, const char __user *,
				   size_t);
static void idmap_release_pipe(struct inode *);
static void idmap_pipe_destroy_msg(struct rpc_pipe_msg *);

static const struct rpc_pipe_ops idmap_upcall_ops = {
	.upcall		= rpc_pipe_generic_upcall,
	.downcall	= idmap_pipe_downcall,
	.release_pipe	= idmap_release_pipe,
	.destroy_msg	= idmap_pipe_destroy_msg,
};

static struct key_type key_type_id_resolver_legacy = {
	.name		= "id_legacy",
	.instantiate	= user_instantiate,
	.match		= user_match,
	.revoke		= user_revoke,
	.destroy	= user_destroy,
	.describe	= user_describe,
	.read		= user_read,
	.request_key	= nfs_idmap_legacy_upcall,
};

static void __nfs_idmap_unregister(struct rpc_pipe *pipe)
{
	if (pipe->dentry)
		rpc_unlink(pipe->dentry);
}

static int __nfs_idmap_register(struct dentry *dir,
				     struct idmap *idmap,
				     struct rpc_pipe *pipe)
{
	struct dentry *dentry;

	dentry = rpc_mkpipe_dentry(dir, "idmap", idmap, pipe);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);
	pipe->dentry = dentry;
	return 0;
}

static void nfs_idmap_unregister(struct nfs_client *clp,
				      struct rpc_pipe *pipe)
{
	struct net *net = clp->net;
	struct super_block *pipefs_sb;

	pipefs_sb = rpc_get_sb_net(net);
	if (pipefs_sb) {
		__nfs_idmap_unregister(pipe);
		rpc_put_sb_net(net);
	}
}

static int nfs_idmap_register(struct nfs_client *clp,
				   struct idmap *idmap,
				   struct rpc_pipe *pipe)
{
	struct net *net = clp->net;
	struct super_block *pipefs_sb;
	int err = 0;

	pipefs_sb = rpc_get_sb_net(net);
	if (pipefs_sb) {
		if (clp->cl_rpcclient->cl_dentry)
			err = __nfs_idmap_register(clp->cl_rpcclient->cl_dentry,
						   idmap, pipe);
		rpc_put_sb_net(net);
	}
	return err;
}
>>>>>>> refs/remotes/origin/cm-10.0

int
nfs_idmap_new(struct nfs_client *clp)
{
	struct idmap *idmap;
<<<<<<< HEAD
=======
	struct rpc_pipe *pipe;
>>>>>>> refs/remotes/origin/cm-10.0
	int error;

	BUG_ON(clp->cl_idmap != NULL);

	idmap = kzalloc(sizeof(*idmap), GFP_KERNEL);
	if (idmap == NULL)
		return -ENOMEM;

<<<<<<< HEAD
	idmap->idmap_dentry = rpc_mkpipe(clp->cl_rpcclient->cl_path.dentry,
			"idmap", idmap, &idmap_upcall_ops, 0);
	if (IS_ERR(idmap->idmap_dentry)) {
		error = PTR_ERR(idmap->idmap_dentry);
		kfree(idmap);
		return error;
	}

	mutex_init(&idmap->idmap_lock);
	mutex_init(&idmap->idmap_im_lock);
	init_waitqueue_head(&idmap->idmap_wq);
	idmap->idmap_user_hash.h_type = IDMAP_TYPE_USER;
	idmap->idmap_group_hash.h_type = IDMAP_TYPE_GROUP;
=======
	pipe = rpc_mkpipe_data(&idmap_upcall_ops, 0);
	if (IS_ERR(pipe)) {
		error = PTR_ERR(pipe);
		kfree(idmap);
		return error;
	}
	error = nfs_idmap_register(clp, idmap, pipe);
	if (error) {
		rpc_destroy_pipe_data(pipe);
		kfree(idmap);
		return error;
	}
	idmap->idmap_pipe = pipe;
	mutex_init(&idmap->idmap_mutex);
>>>>>>> refs/remotes/origin/cm-10.0

	clp->cl_idmap = idmap;
	return 0;
}

void
nfs_idmap_delete(struct nfs_client *clp)
{
	struct idmap *idmap = clp->cl_idmap;

	if (!idmap)
		return;
<<<<<<< HEAD
	rpc_unlink(idmap->idmap_dentry);
=======
	nfs_idmap_unregister(clp, idmap->idmap_pipe);
	rpc_destroy_pipe_data(idmap->idmap_pipe);
>>>>>>> refs/remotes/origin/cm-10.0
	clp->cl_idmap = NULL;
	kfree(idmap);
}

<<<<<<< HEAD
/*
 * Helper routines for manipulating the hashtable
 */
static inline struct idmap_hashent *
idmap_name_hash(struct idmap_hashtable* h, const char *name, size_t len)
{
	return &h->h_entries[fnvhash32(name, len) % IDMAP_HASH_SZ];
}

static struct idmap_hashent *
idmap_lookup_name(struct idmap_hashtable *h, const char *name, size_t len)
{
	struct idmap_hashent *he = idmap_name_hash(h, name, len);

	if (he->ih_namelen != len || memcmp(he->ih_name, name, len) != 0)
		return NULL;
	if (time_after(jiffies, he->ih_expires))
		return NULL;
	return he;
}

static inline struct idmap_hashent *
idmap_id_hash(struct idmap_hashtable* h, __u32 id)
{
	return &h->h_entries[fnvhash32(&id, sizeof(id)) % IDMAP_HASH_SZ];
}

static struct idmap_hashent *
idmap_lookup_id(struct idmap_hashtable *h, __u32 id)
{
	struct idmap_hashent *he = idmap_id_hash(h, id);
	if (he->ih_id != id || he->ih_namelen == 0)
		return NULL;
	if (time_after(jiffies, he->ih_expires))
		return NULL;
	return he;
}

/*
 * Routines for allocating new entries in the hashtable.
 * For now, we just have 1 entry per bucket, so it's all
 * pretty trivial.
 */
static inline struct idmap_hashent *
idmap_alloc_name(struct idmap_hashtable *h, char *name, size_t len)
{
	return idmap_name_hash(h, name, len);
}

static inline struct idmap_hashent *
idmap_alloc_id(struct idmap_hashtable *h, __u32 id)
{
	return idmap_id_hash(h, id);
}

static void
idmap_update_entry(struct idmap_hashent *he, const char *name,
		size_t namelen, __u32 id)
{
	he->ih_id = id;
	memcpy(he->ih_name, name, namelen);
	he->ih_name[namelen] = '\0';
	he->ih_namelen = namelen;
	he->ih_expires = jiffies + nfs_idmap_cache_timeout;
}

/*
 * Name -> ID
 */
static int
nfs_idmap_id(struct idmap *idmap, struct idmap_hashtable *h,
		const char *name, size_t namelen, __u32 *id)
{
	struct rpc_pipe_msg msg;
	struct idmap_msg *im;
	struct idmap_hashent *he;
	DECLARE_WAITQUEUE(wq, current);
	int ret = -EIO;

	im = &idmap->idmap_im;

	/*
	 * String sanity checks
	 * Note that the userland daemon expects NUL terminated strings
	 */
	for (;;) {
		if (namelen == 0)
			return -EINVAL;
		if (name[namelen-1] != '\0')
			break;
		namelen--;
	}
	if (namelen >= IDMAP_NAMESZ)
		return -EINVAL;

	mutex_lock(&idmap->idmap_lock);
	mutex_lock(&idmap->idmap_im_lock);

	he = idmap_lookup_name(h, name, namelen);
	if (he != NULL) {
		*id = he->ih_id;
		ret = 0;
		goto out;
	}

	memset(im, 0, sizeof(*im));
	memcpy(im->im_name, name, namelen);

	im->im_type = h->h_type;
	im->im_conv = IDMAP_CONV_NAMETOID;

	memset(&msg, 0, sizeof(msg));
	msg.data = im;
	msg.len = sizeof(*im);

	add_wait_queue(&idmap->idmap_wq, &wq);
	if (rpc_queue_upcall(idmap->idmap_dentry->d_inode, &msg) < 0) {
		remove_wait_queue(&idmap->idmap_wq, &wq);
		goto out;
	}

	set_current_state(TASK_UNINTERRUPTIBLE);
	mutex_unlock(&idmap->idmap_im_lock);
	schedule();
	__set_current_state(TASK_RUNNING);
	remove_wait_queue(&idmap->idmap_wq, &wq);
	mutex_lock(&idmap->idmap_im_lock);

	if (im->im_status & IDMAP_STATUS_SUCCESS) {
		*id = im->im_id;
		ret = 0;
	}

 out:
	memset(im, 0, sizeof(*im));
	mutex_unlock(&idmap->idmap_im_lock);
	mutex_unlock(&idmap->idmap_lock);
	return ret;
}

/*
 * ID -> Name
 */
static int
nfs_idmap_name(struct idmap *idmap, struct idmap_hashtable *h,
		__u32 id, char *name)
{
	struct rpc_pipe_msg msg;
	struct idmap_msg *im;
	struct idmap_hashent *he;
	DECLARE_WAITQUEUE(wq, current);
	int ret = -EIO;
	unsigned int len;

	im = &idmap->idmap_im;

	mutex_lock(&idmap->idmap_lock);
	mutex_lock(&idmap->idmap_im_lock);

	he = idmap_lookup_id(h, id);
	if (he) {
		memcpy(name, he->ih_name, he->ih_namelen);
		ret = he->ih_namelen;
		goto out;
	}

	memset(im, 0, sizeof(*im));
	im->im_type = h->h_type;
	im->im_conv = IDMAP_CONV_IDTONAME;
	im->im_id = id;

	memset(&msg, 0, sizeof(msg));
	msg.data = im;
	msg.len = sizeof(*im);

	add_wait_queue(&idmap->idmap_wq, &wq);

	if (rpc_queue_upcall(idmap->idmap_dentry->d_inode, &msg) < 0) {
		remove_wait_queue(&idmap->idmap_wq, &wq);
		goto out;
	}

	set_current_state(TASK_UNINTERRUPTIBLE);
	mutex_unlock(&idmap->idmap_im_lock);
	schedule();
	__set_current_state(TASK_RUNNING);
	remove_wait_queue(&idmap->idmap_wq, &wq);
	mutex_lock(&idmap->idmap_im_lock);

	if (im->im_status & IDMAP_STATUS_SUCCESS) {
		if ((len = strnlen(im->im_name, IDMAP_NAMESZ)) == 0)
			goto out;
		memcpy(name, im->im_name, len);
		ret = len;
	}

 out:
	memset(im, 0, sizeof(*im));
	mutex_unlock(&idmap->idmap_im_lock);
	mutex_unlock(&idmap->idmap_lock);
	return ret;
}

/* RPC pipefs upcall/downcall routines */
static ssize_t
idmap_pipe_upcall(struct file *filp, struct rpc_pipe_msg *msg,
		  char __user *dst, size_t buflen)
{
	char *data = (char *)msg->data + msg->copied;
	size_t mlen = min(msg->len, buflen);
	unsigned long left;

	left = copy_to_user(dst, data, mlen);
	if (left == mlen) {
		msg->errno = -EFAULT;
		return -EFAULT;
	}

	mlen -= left;
	msg->copied += mlen;
	msg->errno = 0;
	return mlen;
=======
static int __rpc_pipefs_event(struct nfs_client *clp, unsigned long event,
			      struct super_block *sb)
{
	int err = 0;

	switch (event) {
	case RPC_PIPEFS_MOUNT:
		BUG_ON(clp->cl_rpcclient->cl_dentry == NULL);
		err = __nfs_idmap_register(clp->cl_rpcclient->cl_dentry,
						clp->cl_idmap,
						clp->cl_idmap->idmap_pipe);
		break;
	case RPC_PIPEFS_UMOUNT:
		if (clp->cl_idmap->idmap_pipe) {
			struct dentry *parent;

			parent = clp->cl_idmap->idmap_pipe->dentry->d_parent;
			__nfs_idmap_unregister(clp->cl_idmap->idmap_pipe);
			/*
			 * Note: This is a dirty hack. SUNRPC hook has been
			 * called already but simple_rmdir() call for the
			 * directory returned with error because of idmap pipe
			 * inside. Thus now we have to remove this directory
			 * here.
			 */
			if (rpc_rmdir(parent))
				printk(KERN_ERR "NFS: %s: failed to remove "
					"clnt dir!\n", __func__);
		}
		break;
	default:
		printk(KERN_ERR "NFS: %s: unknown event: %ld\n", __func__,
			event);
		return -ENOTSUPP;
	}
	return err;
}

static struct nfs_client *nfs_get_client_for_event(struct net *net, int event)
{
	struct nfs_net *nn = net_generic(net, nfs_net_id);
	struct dentry *cl_dentry;
	struct nfs_client *clp;

	spin_lock(&nn->nfs_client_lock);
	list_for_each_entry(clp, &nn->nfs_client_list, cl_share_link) {
		if (clp->rpc_ops != &nfs_v4_clientops)
			continue;
		cl_dentry = clp->cl_idmap->idmap_pipe->dentry;
		if (((event == RPC_PIPEFS_MOUNT) && cl_dentry) ||
		    ((event == RPC_PIPEFS_UMOUNT) && !cl_dentry))
			continue;
		atomic_inc(&clp->cl_count);
		spin_unlock(&nn->nfs_client_lock);
		return clp;
	}
	spin_unlock(&nn->nfs_client_lock);
	return NULL;
}

static int rpc_pipefs_event(struct notifier_block *nb, unsigned long event,
			    void *ptr)
{
	struct super_block *sb = ptr;
	struct nfs_client *clp;
	int error = 0;

	if (!try_module_get(THIS_MODULE))
		return 0;

	while ((clp = nfs_get_client_for_event(sb->s_fs_info, event))) {
		error = __rpc_pipefs_event(clp, event, sb);
		nfs_put_client(clp);
		if (error)
			break;
	}
	module_put(THIS_MODULE);
	return error;
}

#define PIPEFS_NFS_PRIO		1

static struct notifier_block nfs_idmap_block = {
	.notifier_call	= rpc_pipefs_event,
	.priority	= SUNRPC_PIPEFS_NFS_PRIO,
};

int nfs_idmap_init(void)
{
	int ret;
	ret = nfs_idmap_init_keyring();
	if (ret != 0)
		goto out;
	ret = rpc_pipefs_notifier_register(&nfs_idmap_block);
	if (ret != 0)
		nfs_idmap_quit_keyring();
out:
	return ret;
}

void nfs_idmap_quit(void)
{
	rpc_pipefs_notifier_unregister(&nfs_idmap_block);
	nfs_idmap_quit_keyring();
}

static int nfs_idmap_prepare_message(char *desc, struct idmap *idmap,
				     struct idmap_msg *im,
				     struct rpc_pipe_msg *msg)
{
	substring_t substr;
	int token, ret;

	memset(im,  0, sizeof(*im));
	memset(msg, 0, sizeof(*msg));

	im->im_type = IDMAP_TYPE_GROUP;
	token = match_token(desc, nfs_idmap_tokens, &substr);

	switch (token) {
	case Opt_find_uid:
		im->im_type = IDMAP_TYPE_USER;
	case Opt_find_gid:
		im->im_conv = IDMAP_CONV_NAMETOID;
		ret = match_strlcpy(im->im_name, &substr, IDMAP_NAMESZ);
		break;

	case Opt_find_user:
		im->im_type = IDMAP_TYPE_USER;
	case Opt_find_group:
		im->im_conv = IDMAP_CONV_IDTONAME;
		ret = match_int(&substr, &im->im_id);
		break;

	default:
		ret = -EINVAL;
		goto out;
	}

	msg->data = im;
	msg->len  = sizeof(struct idmap_msg);

out:
	return ret;
}

static int nfs_idmap_legacy_upcall(struct key_construction *cons,
				   const char *op,
				   void *aux)
{
	struct idmap_legacy_upcalldata *data;
	struct rpc_pipe_msg *msg;
	struct idmap_msg *im;
	struct idmap *idmap = (struct idmap *)aux;
	struct key *key = cons->key;
	int ret = -ENOMEM;

	/* msg and im are freed in idmap_pipe_destroy_msg */
	data = kmalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		goto out1;

	msg = &data->pipe_msg;
	im = &data->idmap_msg;
	data->idmap = idmap;

	ret = nfs_idmap_prepare_message(key->description, idmap, im, msg);
	if (ret < 0)
		goto out2;

	BUG_ON(idmap->idmap_key_cons != NULL);
	idmap->idmap_key_cons = cons;

	ret = rpc_queue_upcall(idmap->idmap_pipe, msg);
	if (ret < 0)
		goto out3;

	return ret;

out3:
	idmap->idmap_key_cons = NULL;
out2:
	kfree(data);
out1:
	complete_request_key(cons, ret);
	return ret;
}

static int nfs_idmap_instantiate(struct key *key, struct key *authkey, char *data)
{
	return key_instantiate_and_link(key, data, strlen(data) + 1,
					id_resolver_cache->thread_keyring,
					authkey);
}

static int nfs_idmap_read_message(struct idmap_msg *im, struct key *key, struct key *authkey)
{
	char id_str[NFS_UINT_MAXLEN];
	int ret = -EINVAL;

	switch (im->im_conv) {
	case IDMAP_CONV_NAMETOID:
		sprintf(id_str, "%d", im->im_id);
		ret = nfs_idmap_instantiate(key, authkey, id_str);
		break;
	case IDMAP_CONV_IDTONAME:
		ret = nfs_idmap_instantiate(key, authkey, im->im_name);
		break;
	}

	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}

static ssize_t
idmap_pipe_downcall(struct file *filp, const char __user *src, size_t mlen)
{
	struct rpc_inode *rpci = RPC_I(filp->f_path.dentry->d_inode);
	struct idmap *idmap = (struct idmap *)rpci->private;
<<<<<<< HEAD
	struct idmap_msg im_in, *im = &idmap->idmap_im;
	struct idmap_hashtable *h;
	struct idmap_hashent *he = NULL;
	size_t namelen_in;
	int ret;

	if (mlen != sizeof(im_in))
		return -ENOSPC;

	if (copy_from_user(&im_in, src, mlen) != 0)
		return -EFAULT;

	mutex_lock(&idmap->idmap_im_lock);

	ret = mlen;
	im->im_status = im_in.im_status;
	/* If we got an error, terminate now, and wake up pending upcalls */
	if (!(im_in.im_status & IDMAP_STATUS_SUCCESS)) {
		wake_up(&idmap->idmap_wq);
		goto out;
	}

	/* Sanity checking of strings */
	ret = -EINVAL;
	namelen_in = strnlen(im_in.im_name, IDMAP_NAMESZ);
	if (namelen_in == 0 || namelen_in == IDMAP_NAMESZ)
		goto out;

	switch (im_in.im_type) {
		case IDMAP_TYPE_USER:
			h = &idmap->idmap_user_hash;
			break;
		case IDMAP_TYPE_GROUP:
			h = &idmap->idmap_group_hash;
			break;
		default:
			goto out;
	}

	switch (im_in.im_conv) {
	case IDMAP_CONV_IDTONAME:
		/* Did we match the current upcall? */
		if (im->im_conv == IDMAP_CONV_IDTONAME
				&& im->im_type == im_in.im_type
				&& im->im_id == im_in.im_id) {
			/* Yes: copy string, including the terminating '\0'  */
			memcpy(im->im_name, im_in.im_name, namelen_in);
			im->im_name[namelen_in] = '\0';
			wake_up(&idmap->idmap_wq);
		}
		he = idmap_alloc_id(h, im_in.im_id);
		break;
	case IDMAP_CONV_NAMETOID:
		/* Did we match the current upcall? */
		if (im->im_conv == IDMAP_CONV_NAMETOID
				&& im->im_type == im_in.im_type
				&& strnlen(im->im_name, IDMAP_NAMESZ) == namelen_in
				&& memcmp(im->im_name, im_in.im_name, namelen_in) == 0) {
			im->im_id = im_in.im_id;
			wake_up(&idmap->idmap_wq);
		}
		he = idmap_alloc_name(h, im_in.im_name, namelen_in);
		break;
	default:
		goto out;
	}

	/* If the entry is valid, also copy it to the cache */
	if (he != NULL)
		idmap_update_entry(he, im_in.im_name, namelen_in, im_in.im_id);
	ret = mlen;
out:
	mutex_unlock(&idmap->idmap_im_lock);
=======
	struct key_construction *cons;
	struct idmap_msg im;
	size_t namelen_in;
	int ret;

	/* If instantiation is successful, anyone waiting for key construction
	 * will have been woken up and someone else may now have used
	 * idmap_key_cons - so after this point we may no longer touch it.
	 */
	cons = ACCESS_ONCE(idmap->idmap_key_cons);
	idmap->idmap_key_cons = NULL;

	if (mlen != sizeof(im)) {
		ret = -ENOSPC;
		goto out;
	}

	if (copy_from_user(&im, src, mlen) != 0) {
		ret = -EFAULT;
		goto out;
	}

	if (!(im.im_status & IDMAP_STATUS_SUCCESS)) {
		ret = -ENOKEY;
		goto out;
	}

	namelen_in = strnlen(im.im_name, IDMAP_NAMESZ);
	if (namelen_in == 0 || namelen_in == IDMAP_NAMESZ) {
		ret = -EINVAL;
		goto out;
	}

	ret = nfs_idmap_read_message(&im, cons->key, cons->authkey);
	if (ret >= 0) {
		key_set_timeout(cons->key, nfs_idmap_cache_timeout);
		ret = mlen;
	}

out:
	complete_request_key(cons, ret);
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static void
idmap_pipe_destroy_msg(struct rpc_pipe_msg *msg)
{
<<<<<<< HEAD
	struct idmap_msg *im = msg->data;
	struct idmap *idmap = container_of(im, struct idmap, idmap_im); 

	if (msg->errno >= 0)
		return;
	mutex_lock(&idmap->idmap_im_lock);
	im->im_status = IDMAP_STATUS_LOOKUPFAIL;
	wake_up(&idmap->idmap_wq);
	mutex_unlock(&idmap->idmap_im_lock);
}

/* 
 * Fowler/Noll/Vo hash
 *    http://www.isthe.com/chongo/tech/comp/fnv/
 */

#define FNV_P_32 ((unsigned int)0x01000193) /* 16777619 */
#define FNV_1_32 ((unsigned int)0x811c9dc5) /* 2166136261 */

static unsigned int fnvhash32(const void *buf, size_t buflen)
{
	const unsigned char *p, *end = (const unsigned char *)buf + buflen;
	unsigned int hash = FNV_1_32;

	for (p = buf; p < end; p++) {
		hash *= FNV_P_32;
		hash ^= (unsigned int)*p;
	}

	return hash;
=======
	struct idmap_legacy_upcalldata *data = container_of(msg,
			struct idmap_legacy_upcalldata,
			pipe_msg);
	struct idmap *idmap = data->idmap;
	struct key_construction *cons;
	if (msg->errno) {
		cons = ACCESS_ONCE(idmap->idmap_key_cons);
		idmap->idmap_key_cons = NULL;
		complete_request_key(cons, msg->errno);
	}
	/* Free memory allocated in nfs_idmap_legacy_upcall() */
	kfree(data);
}

static void
idmap_release_pipe(struct inode *inode)
{
	struct rpc_inode *rpci = RPC_I(inode);
	struct idmap *idmap = (struct idmap *)rpci->private;
	idmap->idmap_key_cons = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
}

int nfs_map_name_to_uid(const struct nfs_server *server, const char *name, size_t namelen, __u32 *uid)
{
	struct idmap *idmap = server->nfs_client->cl_idmap;

	if (nfs_map_string_to_numeric(name, namelen, uid))
		return 0;
<<<<<<< HEAD
	return nfs_idmap_id(idmap, &idmap->idmap_user_hash, name, namelen, uid);
}

int nfs_map_group_to_gid(const struct nfs_server *server, const char *name, size_t namelen, __u32 *uid)
{
	struct idmap *idmap = server->nfs_client->cl_idmap;

	if (nfs_map_string_to_numeric(name, namelen, uid))
		return 0;
	return nfs_idmap_id(idmap, &idmap->idmap_group_hash, name, namelen, uid);
=======
	return nfs_idmap_lookup_id(name, namelen, "uid", uid, idmap);
}

int nfs_map_group_to_gid(const struct nfs_server *server, const char *name, size_t namelen, __u32 *gid)
{
	struct idmap *idmap = server->nfs_client->cl_idmap;

	if (nfs_map_string_to_numeric(name, namelen, gid))
		return 0;
	return nfs_idmap_lookup_id(name, namelen, "gid", gid, idmap);
>>>>>>> refs/remotes/origin/cm-10.0
}

int nfs_map_uid_to_name(const struct nfs_server *server, __u32 uid, char *buf, size_t buflen)
{
	struct idmap *idmap = server->nfs_client->cl_idmap;
	int ret = -EINVAL;

	if (!(server->caps & NFS_CAP_UIDGID_NOMAP))
<<<<<<< HEAD
		ret = nfs_idmap_name(idmap, &idmap->idmap_user_hash, uid, buf);
=======
		ret = nfs_idmap_lookup_name(uid, "user", buf, buflen, idmap);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		ret = nfs_map_numeric_to_string(uid, buf, buflen);
	return ret;
}
<<<<<<< HEAD
int nfs_map_gid_to_group(const struct nfs_server *server, __u32 uid, char *buf, size_t buflen)
=======
int nfs_map_gid_to_group(const struct nfs_server *server, __u32 gid, char *buf, size_t buflen)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct idmap *idmap = server->nfs_client->cl_idmap;
	int ret = -EINVAL;

	if (!(server->caps & NFS_CAP_UIDGID_NOMAP))
<<<<<<< HEAD
		ret = nfs_idmap_name(idmap, &idmap->idmap_group_hash, uid, buf);
	if (ret < 0)
		ret = nfs_map_numeric_to_string(uid, buf, buflen);
	return ret;
}

#endif /* CONFIG_NFS_USE_NEW_IDMAPPER */
=======
		ret = nfs_idmap_lookup_name(gid, "group", buf, buflen, idmap);
	if (ret < 0)
		ret = nfs_map_numeric_to_string(gid, buf, buflen);
	return ret;
}
>>>>>>> refs/remotes/origin/cm-10.0
