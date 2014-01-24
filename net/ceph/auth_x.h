#ifndef _FS_CEPH_AUTH_X_H
#define _FS_CEPH_AUTH_X_H

#include <linux/rbtree.h>

#include <linux/ceph/auth.h>

#include "crypto.h"
#include "auth_x_protocol.h"

/*
 * Handle ticket for a single service.
 */
struct ceph_x_ticket_handler {
	struct rb_node node;
<<<<<<< HEAD
	unsigned service;
=======
	unsigned int service;
>>>>>>> refs/remotes/origin/master

	struct ceph_crypto_key session_key;
	struct ceph_timespec validity;

	u64 secret_id;
	struct ceph_buffer *ticket_blob;

	unsigned long renew_after, expires;
};


struct ceph_x_authorizer {
	struct ceph_buffer *buf;
<<<<<<< HEAD
	unsigned service;
	u64 nonce;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	u64 secret_id;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int service;
	u64 nonce;
	u64 secret_id;
>>>>>>> refs/remotes/origin/master
=======
	u64 secret_id;
>>>>>>> refs/remotes/origin/cm-11.0
	char reply_buf[128];  /* big enough for encrypted blob */
};

struct ceph_x_info {
	struct ceph_crypto_key secret;

	bool starting;
	u64 server_challenge;

<<<<<<< HEAD
	unsigned have_keys;
=======
	unsigned int have_keys;
>>>>>>> refs/remotes/origin/master
	struct rb_root ticket_handlers;

	struct ceph_x_authorizer auth_authorizer;
};

<<<<<<< HEAD
extern int ceph_x_init(struct ceph_auth_client *ac);
=======
int ceph_x_init(struct ceph_auth_client *ac);
>>>>>>> refs/remotes/origin/master

#endif

