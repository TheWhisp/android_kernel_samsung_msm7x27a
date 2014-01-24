/*
   drbd_nl.c

   This file is part of DRBD by Philipp Reisner and Lars Ellenberg.

   Copyright (C) 2001-2008, LINBIT Information Technologies GmbH.
   Copyright (C) 1999-2008, Philipp Reisner <philipp.reisner@linbit.com>.
   Copyright (C) 2002-2008, Lars Ellenberg <lars.ellenberg@linbit.com>.

   drbd is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   drbd is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with drbd; see the file COPYING.  If not, write to
   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 */

#include <linux/module.h>
#include <linux/drbd.h>
#include <linux/in.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/connector.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/blkpg.h>
#include <linux/cpumask.h>
#include "drbd_int.h"
#include "drbd_req.h"
#include "drbd_wrappers.h"
#include <asm/unaligned.h>
<<<<<<< HEAD
#include <linux/drbd_tag_magic.h>
#include <linux/drbd_limits.h>
#include <linux/compiler.h>
#include <linux/kthread.h>

static unsigned short *tl_add_blob(unsigned short *, enum drbd_tags, const void *, int);
static unsigned short *tl_add_str(unsigned short *, enum drbd_tags, const char *);
static unsigned short *tl_add_int(unsigned short *, enum drbd_tags, const void *);

/* see get_sb_bdev and bd_claim */
static char *drbd_m_holder = "Hands off! this is DRBD's meta data device.";

/* Generate the tag_list to struct functions */
#define NL_PACKET(name, number, fields) \
static int name ## _from_tags(struct drbd_conf *mdev, \
	unsigned short *tags, struct name *arg) __attribute__ ((unused)); \
static int name ## _from_tags(struct drbd_conf *mdev, \
	unsigned short *tags, struct name *arg) \
{ \
	int tag; \
	int dlen; \
	\
	while ((tag = get_unaligned(tags++)) != TT_END) {	\
		dlen = get_unaligned(tags++);			\
		switch (tag_number(tag)) { \
		fields \
		default: \
			if (tag & T_MANDATORY) { \
				dev_err(DEV, "Unknown tag: %d\n", tag_number(tag)); \
				return 0; \
			} \
		} \
		tags = (unsigned short *)((char *)tags + dlen); \
	} \
	return 1; \
}
#define NL_INTEGER(pn, pr, member) \
	case pn: /* D_ASSERT( tag_type(tag) == TT_INTEGER ); */ \
		arg->member = get_unaligned((int *)(tags));	\
		break;
#define NL_INT64(pn, pr, member) \
	case pn: /* D_ASSERT( tag_type(tag) == TT_INT64 ); */ \
		arg->member = get_unaligned((u64 *)(tags));	\
		break;
#define NL_BIT(pn, pr, member) \
	case pn: /* D_ASSERT( tag_type(tag) == TT_BIT ); */ \
		arg->member = *(char *)(tags) ? 1 : 0; \
		break;
#define NL_STRING(pn, pr, member, len) \
	case pn: /* D_ASSERT( tag_type(tag) == TT_STRING ); */ \
		if (dlen > len) { \
			dev_err(DEV, "arg too long: %s (%u wanted, max len: %u bytes)\n", \
				#member, dlen, (unsigned int)len); \
			return 0; \
		} \
		 arg->member ## _len = dlen; \
		 memcpy(arg->member, tags, min_t(size_t, dlen, len)); \
		 break;
<<<<<<< HEAD
#include "linux/drbd_nl.h"
=======
#include <linux/drbd_nl.h>
>>>>>>> refs/remotes/origin/cm-10.0

/* Generate the struct to tag_list functions */
#define NL_PACKET(name, number, fields) \
static unsigned short* \
name ## _to_tags(struct drbd_conf *mdev, \
	struct name *arg, unsigned short *tags) __attribute__ ((unused)); \
static unsigned short* \
name ## _to_tags(struct drbd_conf *mdev, \
	struct name *arg, unsigned short *tags) \
{ \
	fields \
	return tags; \
}

#define NL_INTEGER(pn, pr, member) \
	put_unaligned(pn | pr | TT_INTEGER, tags++);	\
	put_unaligned(sizeof(int), tags++);		\
	put_unaligned(arg->member, (int *)tags);	\
	tags = (unsigned short *)((char *)tags+sizeof(int));
#define NL_INT64(pn, pr, member) \
	put_unaligned(pn | pr | TT_INT64, tags++);	\
	put_unaligned(sizeof(u64), tags++);		\
	put_unaligned(arg->member, (u64 *)tags);	\
	tags = (unsigned short *)((char *)tags+sizeof(u64));
#define NL_BIT(pn, pr, member) \
	put_unaligned(pn | pr | TT_BIT, tags++);	\
	put_unaligned(sizeof(char), tags++);		\
	*(char *)tags = arg->member; \
	tags = (unsigned short *)((char *)tags+sizeof(char));
#define NL_STRING(pn, pr, member, len) \
	put_unaligned(pn | pr | TT_STRING, tags++);	\
	put_unaligned(arg->member ## _len, tags++);	\
	memcpy(tags, arg->member, arg->member ## _len); \
	tags = (unsigned short *)((char *)tags + arg->member ## _len);
<<<<<<< HEAD
#include "linux/drbd_nl.h"
=======
#include <linux/drbd_nl.h>
>>>>>>> refs/remotes/origin/cm-10.0

void drbd_bcast_ev_helper(struct drbd_conf *mdev, char *helper_name);
void drbd_nl_send_reply(struct cn_msg *, int);
=======
#include <linux/drbd_limits.h>
#include <linux/kthread.h>

#include <net/genetlink.h>

/* .doit */
// int drbd_adm_create_resource(struct sk_buff *skb, struct genl_info *info);
// int drbd_adm_delete_resource(struct sk_buff *skb, struct genl_info *info);

int drbd_adm_add_minor(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_delete_minor(struct sk_buff *skb, struct genl_info *info);

int drbd_adm_new_resource(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_del_resource(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_down(struct sk_buff *skb, struct genl_info *info);

int drbd_adm_set_role(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_attach(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_disk_opts(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_detach(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_connect(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_net_opts(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_resize(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_start_ov(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_new_c_uuid(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_disconnect(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_invalidate(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_invalidate_peer(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_pause_sync(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_resume_sync(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_suspend_io(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_resume_io(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_outdate(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_resource_opts(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_get_status(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_get_timeout_type(struct sk_buff *skb, struct genl_info *info);
/* .dumpit */
int drbd_adm_get_status_all(struct sk_buff *skb, struct netlink_callback *cb);

#include <linux/drbd_genl_api.h>
#include "drbd_nla.h"
#include <linux/genl_magic_func.h>

/* used blkdev_get_by_path, to claim our meta data device(s) */
static char *drbd_m_holder = "Hands off! this is DRBD's meta data device.";

/* Configuration is strictly serialized, because generic netlink message
 * processing is strictly serialized by the genl_lock().
 * Which means we can use one static global drbd_config_context struct.
 */
static struct drbd_config_context {
	/* assigned from drbd_genlmsghdr */
	unsigned int minor;
	/* assigned from request attributes, if present */
	unsigned int volume;
#define VOLUME_UNSPECIFIED		(-1U)
	/* pointer into the request skb,
	 * limited lifetime! */
	char *resource_name;
	struct nlattr *my_addr;
	struct nlattr *peer_addr;

	/* reply buffer */
	struct sk_buff *reply_skb;
	/* pointer into reply buffer */
	struct drbd_genlmsghdr *reply_dh;
	/* resolved from attributes, if possible */
	struct drbd_conf *mdev;
	struct drbd_tconn *tconn;
} adm_ctx;

static void drbd_adm_send_reply(struct sk_buff *skb, struct genl_info *info)
{
	genlmsg_end(skb, genlmsg_data(nlmsg_data(nlmsg_hdr(skb))));
	if (genlmsg_reply(skb, info))
		printk(KERN_ERR "drbd: error sending genl reply\n");
}

/* Used on a fresh "drbd_adm_prepare"d reply_skb, this cannot fail: The only
 * reason it could fail was no space in skb, and there are 4k available. */
int drbd_msg_put_info(const char *info)
{
	struct sk_buff *skb = adm_ctx.reply_skb;
	struct nlattr *nla;
	int err = -EMSGSIZE;

	if (!info || !info[0])
		return 0;

	nla = nla_nest_start(skb, DRBD_NLA_CFG_REPLY);
	if (!nla)
		return err;

	err = nla_put_string(skb, T_info_text, info);
	if (err) {
		nla_nest_cancel(skb, nla);
		return err;
	} else
		nla_nest_end(skb, nla);
	return 0;
}

/* This would be a good candidate for a "pre_doit" hook,
 * and per-family private info->pointers.
 * But we need to stay compatible with older kernels.
 * If it returns successfully, adm_ctx members are valid.
 */
#define DRBD_ADM_NEED_MINOR	1
#define DRBD_ADM_NEED_RESOURCE	2
#define DRBD_ADM_NEED_CONNECTION 4
static int drbd_adm_prepare(struct sk_buff *skb, struct genl_info *info,
		unsigned flags)
{
	struct drbd_genlmsghdr *d_in = info->userhdr;
	const u8 cmd = info->genlhdr->cmd;
	int err;

	memset(&adm_ctx, 0, sizeof(adm_ctx));

	/* genl_rcv_msg only checks for CAP_NET_ADMIN on "GENL_ADMIN_PERM" :( */
	if (cmd != DRBD_ADM_GET_STATUS && !capable(CAP_NET_ADMIN))
	       return -EPERM;

	adm_ctx.reply_skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!adm_ctx.reply_skb) {
		err = -ENOMEM;
		goto fail;
	}

	adm_ctx.reply_dh = genlmsg_put_reply(adm_ctx.reply_skb,
					info, &drbd_genl_family, 0, cmd);
	/* put of a few bytes into a fresh skb of >= 4k will always succeed.
	 * but anyways */
	if (!adm_ctx.reply_dh) {
		err = -ENOMEM;
		goto fail;
	}

	adm_ctx.reply_dh->minor = d_in->minor;
	adm_ctx.reply_dh->ret_code = NO_ERROR;

	adm_ctx.volume = VOLUME_UNSPECIFIED;
	if (info->attrs[DRBD_NLA_CFG_CONTEXT]) {
		struct nlattr *nla;
		/* parse and validate only */
		err = drbd_cfg_context_from_attrs(NULL, info);
		if (err)
			goto fail;

		/* It was present, and valid,
		 * copy it over to the reply skb. */
		err = nla_put_nohdr(adm_ctx.reply_skb,
				info->attrs[DRBD_NLA_CFG_CONTEXT]->nla_len,
				info->attrs[DRBD_NLA_CFG_CONTEXT]);
		if (err)
			goto fail;

		/* and assign stuff to the global adm_ctx */
		nla = nested_attr_tb[__nla_type(T_ctx_volume)];
		if (nla)
			adm_ctx.volume = nla_get_u32(nla);
		nla = nested_attr_tb[__nla_type(T_ctx_resource_name)];
		if (nla)
			adm_ctx.resource_name = nla_data(nla);
		adm_ctx.my_addr = nested_attr_tb[__nla_type(T_ctx_my_addr)];
		adm_ctx.peer_addr = nested_attr_tb[__nla_type(T_ctx_peer_addr)];
		if ((adm_ctx.my_addr &&
		     nla_len(adm_ctx.my_addr) > sizeof(adm_ctx.tconn->my_addr)) ||
		    (adm_ctx.peer_addr &&
		     nla_len(adm_ctx.peer_addr) > sizeof(adm_ctx.tconn->peer_addr))) {
			err = -EINVAL;
			goto fail;
		}
	}

	adm_ctx.minor = d_in->minor;
	adm_ctx.mdev = minor_to_mdev(d_in->minor);
	adm_ctx.tconn = conn_get_by_name(adm_ctx.resource_name);

	if (!adm_ctx.mdev && (flags & DRBD_ADM_NEED_MINOR)) {
		drbd_msg_put_info("unknown minor");
		return ERR_MINOR_INVALID;
	}
	if (!adm_ctx.tconn && (flags & DRBD_ADM_NEED_RESOURCE)) {
		drbd_msg_put_info("unknown resource");
		return ERR_INVALID_REQUEST;
	}

	if (flags & DRBD_ADM_NEED_CONNECTION) {
		if (adm_ctx.tconn && !(flags & DRBD_ADM_NEED_RESOURCE)) {
			drbd_msg_put_info("no resource name expected");
			return ERR_INVALID_REQUEST;
		}
		if (adm_ctx.mdev) {
			drbd_msg_put_info("no minor number expected");
			return ERR_INVALID_REQUEST;
		}
		if (adm_ctx.my_addr && adm_ctx.peer_addr)
			adm_ctx.tconn = conn_get_by_addrs(nla_data(adm_ctx.my_addr),
							  nla_len(adm_ctx.my_addr),
							  nla_data(adm_ctx.peer_addr),
							  nla_len(adm_ctx.peer_addr));
		if (!adm_ctx.tconn) {
			drbd_msg_put_info("unknown connection");
			return ERR_INVALID_REQUEST;
		}
	}

	/* some more paranoia, if the request was over-determined */
	if (adm_ctx.mdev && adm_ctx.tconn &&
	    adm_ctx.mdev->tconn != adm_ctx.tconn) {
		pr_warning("request: minor=%u, resource=%s; but that minor belongs to connection %s\n",
				adm_ctx.minor, adm_ctx.resource_name,
				adm_ctx.mdev->tconn->name);
		drbd_msg_put_info("minor exists in different resource");
		return ERR_INVALID_REQUEST;
	}
	if (adm_ctx.mdev &&
	    adm_ctx.volume != VOLUME_UNSPECIFIED &&
	    adm_ctx.volume != adm_ctx.mdev->vnr) {
		pr_warning("request: minor=%u, volume=%u; but that minor is volume %u in %s\n",
				adm_ctx.minor, adm_ctx.volume,
				adm_ctx.mdev->vnr, adm_ctx.mdev->tconn->name);
		drbd_msg_put_info("minor exists as different volume");
		return ERR_INVALID_REQUEST;
	}

	return NO_ERROR;

fail:
	nlmsg_free(adm_ctx.reply_skb);
	adm_ctx.reply_skb = NULL;
	return err;
}

static int drbd_adm_finish(struct genl_info *info, int retcode)
{
	if (adm_ctx.tconn) {
		kref_put(&adm_ctx.tconn->kref, &conn_destroy);
		adm_ctx.tconn = NULL;
	}

	if (!adm_ctx.reply_skb)
		return -ENOMEM;

	adm_ctx.reply_dh->ret_code = retcode;
	drbd_adm_send_reply(adm_ctx.reply_skb, info);
	return 0;
}

static void setup_khelper_env(struct drbd_tconn *tconn, char **envp)
{
	char *afs;

	/* FIXME: A future version will not allow this case. */
	if (tconn->my_addr_len == 0 || tconn->peer_addr_len == 0)
		return;

	switch (((struct sockaddr *)&tconn->peer_addr)->sa_family) {
	case AF_INET6:
		afs = "ipv6";
		snprintf(envp[4], 60, "DRBD_PEER_ADDRESS=%pI6",
			 &((struct sockaddr_in6 *)&tconn->peer_addr)->sin6_addr);
		break;
	case AF_INET:
		afs = "ipv4";
		snprintf(envp[4], 60, "DRBD_PEER_ADDRESS=%pI4",
			 &((struct sockaddr_in *)&tconn->peer_addr)->sin_addr);
		break;
	default:
		afs = "ssocks";
		snprintf(envp[4], 60, "DRBD_PEER_ADDRESS=%pI4",
			 &((struct sockaddr_in *)&tconn->peer_addr)->sin_addr);
	}
	snprintf(envp[3], 20, "DRBD_PEER_AF=%s", afs);
}
>>>>>>> refs/remotes/origin/master

int drbd_khelper(struct drbd_conf *mdev, char *cmd)
{
	char *envp[] = { "HOME=/",
			"TERM=linux",
			"PATH=/sbin:/usr/sbin:/bin:/usr/bin",
<<<<<<< HEAD
			NULL, /* Will be set to address family */
			NULL, /* Will be set to address */
			NULL };

	char mb[12], af[20], ad[60], *afs;
	char *argv[] = {usermode_helper, cmd, mb, NULL };
	int ret;

	snprintf(mb, 12, "minor-%d", mdev_to_minor(mdev));

	if (get_net_conf(mdev)) {
		switch (((struct sockaddr *)mdev->net_conf->peer_addr)->sa_family) {
		case AF_INET6:
			afs = "ipv6";
			snprintf(ad, 60, "DRBD_PEER_ADDRESS=%pI6",
				 &((struct sockaddr_in6 *)mdev->net_conf->peer_addr)->sin6_addr);
			break;
		case AF_INET:
			afs = "ipv4";
			snprintf(ad, 60, "DRBD_PEER_ADDRESS=%pI4",
				 &((struct sockaddr_in *)mdev->net_conf->peer_addr)->sin_addr);
			break;
		default:
			afs = "ssocks";
			snprintf(ad, 60, "DRBD_PEER_ADDRESS=%pI4",
				 &((struct sockaddr_in *)mdev->net_conf->peer_addr)->sin_addr);
		}
		snprintf(af, 20, "DRBD_PEER_AF=%s", afs);
		envp[3]=af;
		envp[4]=ad;
		put_net_conf(mdev);
	}
=======
			 (char[20]) { }, /* address family */
			 (char[60]) { }, /* address */
			NULL };
	char mb[12];
	char *argv[] = {usermode_helper, cmd, mb, NULL };
	struct drbd_tconn *tconn = mdev->tconn;
	struct sib_info sib;
	int ret;

	if (current == tconn->worker.task)
		set_bit(CALLBACK_PENDING, &tconn->flags);

	snprintf(mb, 12, "minor-%d", mdev_to_minor(mdev));
	setup_khelper_env(tconn, envp);
>>>>>>> refs/remotes/origin/master

	/* The helper may take some time.
	 * write out any unsynced meta data changes now */
	drbd_md_sync(mdev);

	dev_info(DEV, "helper command: %s %s %s\n", usermode_helper, cmd, mb);
<<<<<<< HEAD

	drbd_bcast_ev_helper(mdev, cmd);
<<<<<<< HEAD
	ret = call_usermodehelper(usermode_helper, argv, envp, 1);
=======
	ret = call_usermodehelper(usermode_helper, argv, envp, UMH_WAIT_PROC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sib.sib_reason = SIB_HELPER_PRE;
	sib.helper_name = cmd;
	drbd_bcast_event(mdev, &sib);
	ret = call_usermodehelper(usermode_helper, argv, envp, UMH_WAIT_PROC);
>>>>>>> refs/remotes/origin/master
	if (ret)
		dev_warn(DEV, "helper command: %s %s %s exit code %u (0x%x)\n",
				usermode_helper, cmd, mb,
				(ret >> 8) & 0xff, ret);
	else
		dev_info(DEV, "helper command: %s %s %s exit code %u (0x%x)\n",
				usermode_helper, cmd, mb,
				(ret >> 8) & 0xff, ret);
<<<<<<< HEAD
=======
	sib.sib_reason = SIB_HELPER_POST;
	sib.helper_exit_code = ret;
	drbd_bcast_event(mdev, &sib);

	if (current == tconn->worker.task)
		clear_bit(CALLBACK_PENDING, &tconn->flags);

	if (ret < 0) /* Ignore any ERRNOs we got. */
		ret = 0;

	return ret;
}

int conn_khelper(struct drbd_tconn *tconn, char *cmd)
{
	char *envp[] = { "HOME=/",
			"TERM=linux",
			"PATH=/sbin:/usr/sbin:/bin:/usr/bin",
			 (char[20]) { }, /* address family */
			 (char[60]) { }, /* address */
			NULL };
	char *argv[] = {usermode_helper, cmd, tconn->name, NULL };
	int ret;

	setup_khelper_env(tconn, envp);
	conn_md_sync(tconn);

	conn_info(tconn, "helper command: %s %s %s\n", usermode_helper, cmd, tconn->name);
	/* TODO: conn_bcast_event() ?? */

	ret = call_usermodehelper(usermode_helper, argv, envp, UMH_WAIT_PROC);
	if (ret)
		conn_warn(tconn, "helper command: %s %s %s exit code %u (0x%x)\n",
			  usermode_helper, cmd, tconn->name,
			  (ret >> 8) & 0xff, ret);
	else
		conn_info(tconn, "helper command: %s %s %s exit code %u (0x%x)\n",
			  usermode_helper, cmd, tconn->name,
			  (ret >> 8) & 0xff, ret);
	/* TODO: conn_bcast_event() ?? */
>>>>>>> refs/remotes/origin/master

	if (ret < 0) /* Ignore any ERRNOs we got. */
		ret = 0;

	return ret;
}

<<<<<<< HEAD
enum drbd_disk_state drbd_try_outdate_peer(struct drbd_conf *mdev)
{
	char *ex_to_string;
	int r;
	enum drbd_disk_state nps;
	enum drbd_fencing_p fp;

	D_ASSERT(mdev->state.pdsk == D_UNKNOWN);

	if (get_ldev_if_state(mdev, D_CONSISTENT)) {
		fp = mdev->ldev->dc.fencing;
		put_ldev(mdev);
	} else {
		dev_warn(DEV, "Not fencing peer, I'm not even Consistent myself.\n");
		nps = mdev->state.pdsk;
		goto out;
	}

	r = drbd_khelper(mdev, "fence-peer");
=======
static enum drbd_fencing_p highest_fencing_policy(struct drbd_tconn *tconn)
{
	enum drbd_fencing_p fp = FP_NOT_AVAIL;
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		if (get_ldev_if_state(mdev, D_CONSISTENT)) {
			fp = max_t(enum drbd_fencing_p, fp,
				   rcu_dereference(mdev->ldev->disk_conf)->fencing);
			put_ldev(mdev);
		}
	}
	rcu_read_unlock();

	return fp;
}

bool conn_try_outdate_peer(struct drbd_tconn *tconn)
{
	unsigned int connect_cnt;
	union drbd_state mask = { };
	union drbd_state val = { };
	enum drbd_fencing_p fp;
	char *ex_to_string;
	int r;

	if (tconn->cstate >= C_WF_REPORT_PARAMS) {
		conn_err(tconn, "Expected cstate < C_WF_REPORT_PARAMS\n");
		return false;
	}

	spin_lock_irq(&tconn->req_lock);
	connect_cnt = tconn->connect_cnt;
	spin_unlock_irq(&tconn->req_lock);

	fp = highest_fencing_policy(tconn);
	switch (fp) {
	case FP_NOT_AVAIL:
		conn_warn(tconn, "Not fencing peer, I'm not even Consistent myself.\n");
		goto out;
	case FP_DONT_CARE:
		return true;
	default: ;
	}

	r = conn_khelper(tconn, "fence-peer");
>>>>>>> refs/remotes/origin/master

	switch ((r>>8) & 0xff) {
	case 3: /* peer is inconsistent */
		ex_to_string = "peer is inconsistent or worse";
<<<<<<< HEAD
		nps = D_INCONSISTENT;
		break;
	case 4: /* peer got outdated, or was already outdated */
		ex_to_string = "peer was fenced";
		nps = D_OUTDATED;
		break;
	case 5: /* peer was down */
		if (mdev->state.disk == D_UP_TO_DATE) {
			/* we will(have) create(d) a new UUID anyways... */
			ex_to_string = "peer is unreachable, assumed to be dead";
			nps = D_OUTDATED;
		} else {
			ex_to_string = "peer unreachable, doing nothing since disk != UpToDate";
			nps = mdev->state.pdsk;
=======
		mask.pdsk = D_MASK;
		val.pdsk = D_INCONSISTENT;
		break;
	case 4: /* peer got outdated, or was already outdated */
		ex_to_string = "peer was fenced";
		mask.pdsk = D_MASK;
		val.pdsk = D_OUTDATED;
		break;
	case 5: /* peer was down */
		if (conn_highest_disk(tconn) == D_UP_TO_DATE) {
			/* we will(have) create(d) a new UUID anyways... */
			ex_to_string = "peer is unreachable, assumed to be dead";
			mask.pdsk = D_MASK;
			val.pdsk = D_OUTDATED;
		} else {
			ex_to_string = "peer unreachable, doing nothing since disk != UpToDate";
>>>>>>> refs/remotes/origin/master
		}
		break;
	case 6: /* Peer is primary, voluntarily outdate myself.
		 * This is useful when an unconnected R_SECONDARY is asked to
		 * become R_PRIMARY, but finds the other peer being active. */
		ex_to_string = "peer is active";
<<<<<<< HEAD
		dev_warn(DEV, "Peer is primary, outdating myself.\n");
		nps = D_UNKNOWN;
		_drbd_request_state(mdev, NS(disk, D_OUTDATED), CS_WAIT_COMPLETE);
		break;
	case 7:
		if (fp != FP_STONITH)
			dev_err(DEV, "fence-peer() = 7 && fencing != Stonith !!!\n");
		ex_to_string = "peer was stonithed";
		nps = D_OUTDATED;
		break;
	default:
		/* The script is broken ... */
		nps = D_UNKNOWN;
		dev_err(DEV, "fence-peer helper broken, returned %d\n", (r>>8)&0xff);
		return nps;
	}

	dev_info(DEV, "fence-peer helper returned %d (%s)\n",
			(r>>8) & 0xff, ex_to_string);

out:
	if (mdev->state.susp_fen && nps >= D_UNKNOWN) {
		/* The handler was not successful... unfreeze here, the
		   state engine can not unfreeze... */
		_drbd_request_state(mdev, NS(susp_fen, 0), CS_VERBOSE);
	}

	return nps;
=======
		conn_warn(tconn, "Peer is primary, outdating myself.\n");
		mask.disk = D_MASK;
		val.disk = D_OUTDATED;
		break;
	case 7:
		if (fp != FP_STONITH)
			conn_err(tconn, "fence-peer() = 7 && fencing != Stonith !!!\n");
		ex_to_string = "peer was stonithed";
		mask.pdsk = D_MASK;
		val.pdsk = D_OUTDATED;
		break;
	default:
		/* The script is broken ... */
		conn_err(tconn, "fence-peer helper broken, returned %d\n", (r>>8)&0xff);
		return false; /* Eventually leave IO frozen */
	}

	conn_info(tconn, "fence-peer helper returned %d (%s)\n",
		  (r>>8) & 0xff, ex_to_string);

 out:

	/* Not using
	   conn_request_state(tconn, mask, val, CS_VERBOSE);
	   here, because we might were able to re-establish the connection in the
	   meantime. */
	spin_lock_irq(&tconn->req_lock);
	if (tconn->cstate < C_WF_REPORT_PARAMS && !test_bit(STATE_SENT, &tconn->flags)) {
		if (tconn->connect_cnt != connect_cnt)
			/* In case the connection was established and droped
			   while the fence-peer handler was running, ignore it */
			conn_info(tconn, "Ignoring fence-peer exit code\n");
		else
			_conn_request_state(tconn, mask, val, CS_VERBOSE);
	}
	spin_unlock_irq(&tconn->req_lock);

	return conn_highest_pdsk(tconn) <= D_OUTDATED;
>>>>>>> refs/remotes/origin/master
}

static int _try_outdate_peer_async(void *data)
{
<<<<<<< HEAD
	struct drbd_conf *mdev = (struct drbd_conf *)data;
	enum drbd_disk_state nps;
	union drbd_state ns;

	nps = drbd_try_outdate_peer(mdev);

	/* Not using
	   drbd_request_state(mdev, NS(pdsk, nps));
	   here, because we might were able to re-establish the connection
	   in the meantime. This can only partially be solved in the state's
	   engine is_valid_state() and is_valid_state_transition()
	   functions.

	   nps can be D_INCONSISTENT, D_OUTDATED or D_UNKNOWN.
	   pdsk == D_INCONSISTENT while conn >= C_CONNECTED is valid,
	   therefore we have to have the pre state change check here.
	*/
	spin_lock_irq(&mdev->req_lock);
	ns = mdev->state;
	if (ns.conn < C_WF_REPORT_PARAMS) {
		ns.pdsk = nps;
		_drbd_set_state(mdev, ns, CS_VERBOSE, NULL);
	}
	spin_unlock_irq(&mdev->req_lock);

	return 0;
}

void drbd_try_outdate_peer_async(struct drbd_conf *mdev)
{
	struct task_struct *opa;

	opa = kthread_run(_try_outdate_peer_async, mdev, "drbd%d_a_helper", mdev_to_minor(mdev));
	if (IS_ERR(opa))
		dev_err(DEV, "out of mem, failed to invoke fence-peer helper\n");
=======
	struct drbd_tconn *tconn = (struct drbd_tconn *)data;

	conn_try_outdate_peer(tconn);

	kref_put(&tconn->kref, &conn_destroy);
	return 0;
}

void conn_try_outdate_peer_async(struct drbd_tconn *tconn)
{
	struct task_struct *opa;

	kref_get(&tconn->kref);
	opa = kthread_run(_try_outdate_peer_async, tconn, "drbd_async_h");
	if (IS_ERR(opa)) {
		conn_err(tconn, "out of mem, failed to invoke fence-peer helper\n");
		kref_put(&tconn->kref, &conn_destroy);
	}
>>>>>>> refs/remotes/origin/master
}

enum drbd_state_rv
drbd_set_role(struct drbd_conf *mdev, enum drbd_role new_role, int force)
{
	const int max_tries = 4;
	enum drbd_state_rv rv = SS_UNKNOWN_ERROR;
<<<<<<< HEAD
	int try = 0;
	int forced = 0;
	union drbd_state mask, val;
	enum drbd_disk_state nps;

	if (new_role == R_PRIMARY)
		request_ping(mdev); /* Detect a dead peer ASAP */

	mutex_lock(&mdev->state_mutex);
=======
	struct net_conf *nc;
	int try = 0;
	int forced = 0;
	union drbd_state mask, val;

	if (new_role == R_PRIMARY)
		request_ping(mdev->tconn); /* Detect a dead peer ASAP */

	mutex_lock(mdev->state_mutex);
>>>>>>> refs/remotes/origin/master

	mask.i = 0; mask.role = R_MASK;
	val.i  = 0; val.role  = new_role;

	while (try++ < max_tries) {
		rv = _drbd_request_state(mdev, mask, val, CS_WAIT_COMPLETE);

		/* in case we first succeeded to outdate,
		 * but now suddenly could establish a connection */
		if (rv == SS_CW_FAILED_BY_PEER && mask.pdsk != 0) {
			val.pdsk = 0;
			mask.pdsk = 0;
			continue;
		}

		if (rv == SS_NO_UP_TO_DATE_DISK && force &&
		    (mdev->state.disk < D_UP_TO_DATE &&
		     mdev->state.disk >= D_INCONSISTENT)) {
			mask.disk = D_MASK;
			val.disk  = D_UP_TO_DATE;
			forced = 1;
			continue;
		}

		if (rv == SS_NO_UP_TO_DATE_DISK &&
		    mdev->state.disk == D_CONSISTENT && mask.pdsk == 0) {
			D_ASSERT(mdev->state.pdsk == D_UNKNOWN);
<<<<<<< HEAD
			nps = drbd_try_outdate_peer(mdev);

			if (nps == D_OUTDATED || nps == D_INCONSISTENT) {
				val.disk = D_UP_TO_DATE;
				mask.disk = D_MASK;
			}

			val.pdsk = nps;
			mask.pdsk = D_MASK;

=======

			if (conn_try_outdate_peer(mdev->tconn)) {
				val.disk = D_UP_TO_DATE;
				mask.disk = D_MASK;
			}
>>>>>>> refs/remotes/origin/master
			continue;
		}

		if (rv == SS_NOTHING_TO_DO)
<<<<<<< HEAD
			goto fail;
		if (rv == SS_PRIMARY_NOP && mask.pdsk == 0) {
			nps = drbd_try_outdate_peer(mdev);

			if (force && nps > D_OUTDATED) {
				dev_warn(DEV, "Forced into split brain situation!\n");
				nps = D_OUTDATED;
			}

			mask.pdsk = D_MASK;
			val.pdsk  = nps;

=======
			goto out;
		if (rv == SS_PRIMARY_NOP && mask.pdsk == 0) {
			if (!conn_try_outdate_peer(mdev->tconn) && force) {
				dev_warn(DEV, "Forced into split brain situation!\n");
				mask.pdsk = D_MASK;
				val.pdsk  = D_OUTDATED;

			}
>>>>>>> refs/remotes/origin/master
			continue;
		}
		if (rv == SS_TWO_PRIMARIES) {
			/* Maybe the peer is detected as dead very soon...
			   retry at most once more in this case. */
<<<<<<< HEAD
			schedule_timeout_interruptible((mdev->net_conf->ping_timeo+1)*HZ/10);
=======
			int timeo;
			rcu_read_lock();
			nc = rcu_dereference(mdev->tconn->net_conf);
			timeo = nc ? (nc->ping_timeo + 1) * HZ / 10 : 1;
			rcu_read_unlock();
			schedule_timeout_interruptible(timeo);
>>>>>>> refs/remotes/origin/master
			if (try < max_tries)
				try = max_tries - 1;
			continue;
		}
		if (rv < SS_SUCCESS) {
			rv = _drbd_request_state(mdev, mask, val,
						CS_VERBOSE + CS_WAIT_COMPLETE);
			if (rv < SS_SUCCESS)
<<<<<<< HEAD
				goto fail;
=======
				goto out;
>>>>>>> refs/remotes/origin/master
		}
		break;
	}

	if (rv < SS_SUCCESS)
<<<<<<< HEAD
		goto fail;
=======
		goto out;
>>>>>>> refs/remotes/origin/master

	if (forced)
		dev_warn(DEV, "Forced to consider local data as UpToDate!\n");

	/* Wait until nothing is on the fly :) */
	wait_event(mdev->misc_wait, atomic_read(&mdev->ap_pending_cnt) == 0);

<<<<<<< HEAD
=======
	/* FIXME also wait for all pending P_BARRIER_ACK? */

>>>>>>> refs/remotes/origin/master
	if (new_role == R_SECONDARY) {
		set_disk_ro(mdev->vdisk, true);
		if (get_ldev(mdev)) {
			mdev->ldev->md.uuid[UI_CURRENT] &= ~(u64)1;
			put_ldev(mdev);
		}
	} else {
<<<<<<< HEAD
		if (get_net_conf(mdev)) {
			mdev->net_conf->want_lose = 0;
			put_net_conf(mdev);
		}
=======
		mutex_lock(&mdev->tconn->conf_update);
		nc = mdev->tconn->net_conf;
		if (nc)
			nc->discard_my_data = 0; /* without copy; single bit op is atomic */
		mutex_unlock(&mdev->tconn->conf_update);

>>>>>>> refs/remotes/origin/master
		set_disk_ro(mdev->vdisk, false);
		if (get_ldev(mdev)) {
			if (((mdev->state.conn < C_CONNECTED ||
			       mdev->state.pdsk <= D_FAILED)
			      && mdev->ldev->md.uuid[UI_BITMAP] == 0) || forced)
				drbd_uuid_new_current(mdev);

			mdev->ldev->md.uuid[UI_CURRENT] |=  (u64)1;
			put_ldev(mdev);
		}
	}

	/* writeout of activity log covered areas of the bitmap
	 * to stable storage done in after state change already */

	if (mdev->state.conn >= C_WF_REPORT_PARAMS) {
		/* if this was forced, we should consider sync */
		if (forced)
			drbd_send_uuids(mdev);
<<<<<<< HEAD
		drbd_send_state(mdev);
=======
		drbd_send_current_state(mdev);
>>>>>>> refs/remotes/origin/master
	}

	drbd_md_sync(mdev);

	kobject_uevent(&disk_to_dev(mdev->vdisk)->kobj, KOBJ_CHANGE);
<<<<<<< HEAD
 fail:
	mutex_unlock(&mdev->state_mutex);
	return rv;
}

static struct drbd_conf *ensure_mdev(int minor, int create)
{
	struct drbd_conf *mdev;

	if (minor >= minor_count)
		return NULL;

	mdev = minor_to_mdev(minor);

	if (!mdev && create) {
		struct gendisk *disk = NULL;
		mdev = drbd_new_device(minor);

		spin_lock_irq(&drbd_pp_lock);
		if (minor_table[minor] == NULL) {
			minor_table[minor] = mdev;
			disk = mdev->vdisk;
			mdev = NULL;
		} /* else: we lost the race */
		spin_unlock_irq(&drbd_pp_lock);

		if (disk) /* we won the race above */
			/* in case we ever add a drbd_delete_device(),
			 * don't forget the del_gendisk! */
			add_disk(disk);
		else /* we lost the race above */
			drbd_free_mdev(mdev);

		mdev = minor_to_mdev(minor);
	}

	return mdev;
}

static int drbd_nl_primary(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			   struct drbd_nl_cfg_reply *reply)
{
	struct primary primary_args;

	memset(&primary_args, 0, sizeof(struct primary));
	if (!primary_from_tags(mdev, nlp->tag_list, &primary_args)) {
		reply->ret_code = ERR_MANDATORY_TAG;
		return 0;
	}

	reply->ret_code =
		drbd_set_role(mdev, R_PRIMARY, primary_args.primary_force);

	return 0;
}

static int drbd_nl_secondary(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			     struct drbd_nl_cfg_reply *reply)
{
	reply->ret_code = drbd_set_role(mdev, R_SECONDARY, 0);

	return 0;
}

/* initializes the md.*_offset members, so we are able to find
 * the on disk meta data */
=======
out:
	mutex_unlock(mdev->state_mutex);
	return rv;
}

static const char *from_attrs_err_to_txt(int err)
{
	return	err == -ENOMSG ? "required attribute missing" :
		err == -EOPNOTSUPP ? "unknown mandatory attribute" :
		err == -EEXIST ? "can not change invariant setting" :
		"invalid attribute value";
}

int drbd_adm_set_role(struct sk_buff *skb, struct genl_info *info)
{
	struct set_role_parms parms;
	int err;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	memset(&parms, 0, sizeof(parms));
	if (info->attrs[DRBD_NLA_SET_ROLE_PARMS]) {
		err = set_role_parms_from_attrs(&parms, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
			drbd_msg_put_info(from_attrs_err_to_txt(err));
			goto out;
		}
	}

	if (info->genlhdr->cmd == DRBD_ADM_PRIMARY)
		retcode = drbd_set_role(adm_ctx.mdev, R_PRIMARY, parms.assume_uptodate);
	else
		retcode = drbd_set_role(adm_ctx.mdev, R_SECONDARY, 0);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

/* Initializes the md.*_offset members, so we are able to find
 * the on disk meta data.
 *
 * We currently have two possible layouts:
 * external:
 *   |----------- md_size_sect ------------------|
 *   [ 4k superblock ][ activity log ][  Bitmap  ]
 *   | al_offset == 8 |
 *   | bm_offset = al_offset + X      |
 *  ==> bitmap sectors = md_size_sect - bm_offset
 *
 * internal:
 *            |----------- md_size_sect ------------------|
 * [data.....][  Bitmap  ][ activity log ][ 4k superblock ]
 *                        | al_offset < 0 |
 *            | bm_offset = al_offset - Y |
 *  ==> bitmap sectors = Y = al_offset - bm_offset
 *
 *  Activity log size used to be fixed 32kB,
 *  but is about to become configurable.
 */
>>>>>>> refs/remotes/origin/master
static void drbd_md_set_sector_offsets(struct drbd_conf *mdev,
				       struct drbd_backing_dev *bdev)
{
	sector_t md_size_sect = 0;
<<<<<<< HEAD
	switch (bdev->dc.meta_dev_idx) {
	default:
		/* v07 style fixed size indexed meta data */
		bdev->md.md_size_sect = MD_RESERVED_SECT;
		bdev->md.md_offset = drbd_md_ss__(mdev, bdev);
		bdev->md.al_offset = MD_AL_OFFSET;
		bdev->md.bm_offset = MD_BM_OFFSET;
=======
	unsigned int al_size_sect = bdev->md.al_size_4k * 8;

	bdev->md.md_offset = drbd_md_ss(bdev);

	switch (bdev->md.meta_dev_idx) {
	default:
		/* v07 style fixed size indexed meta data */
		bdev->md.md_size_sect = MD_128MB_SECT;
		bdev->md.al_offset = MD_4kB_SECT;
		bdev->md.bm_offset = MD_4kB_SECT + al_size_sect;
>>>>>>> refs/remotes/origin/master
		break;
	case DRBD_MD_INDEX_FLEX_EXT:
		/* just occupy the full device; unit: sectors */
		bdev->md.md_size_sect = drbd_get_capacity(bdev->md_bdev);
<<<<<<< HEAD
		bdev->md.md_offset = 0;
		bdev->md.al_offset = MD_AL_OFFSET;
		bdev->md.bm_offset = MD_BM_OFFSET;
		break;
	case DRBD_MD_INDEX_INTERNAL:
	case DRBD_MD_INDEX_FLEX_INT:
		bdev->md.md_offset = drbd_md_ss__(mdev, bdev);
		/* al size is still fixed */
		bdev->md.al_offset = -MD_AL_MAX_SIZE;
=======
		bdev->md.al_offset = MD_4kB_SECT;
		bdev->md.bm_offset = MD_4kB_SECT + al_size_sect;
		break;
	case DRBD_MD_INDEX_INTERNAL:
	case DRBD_MD_INDEX_FLEX_INT:
		/* al size is still fixed */
		bdev->md.al_offset = -al_size_sect;
>>>>>>> refs/remotes/origin/master
		/* we need (slightly less than) ~ this much bitmap sectors: */
		md_size_sect = drbd_get_capacity(bdev->backing_bdev);
		md_size_sect = ALIGN(md_size_sect, BM_SECT_PER_EXT);
		md_size_sect = BM_SECT_TO_EXT(md_size_sect);
		md_size_sect = ALIGN(md_size_sect, 8);

		/* plus the "drbd meta data super block",
		 * and the activity log; */
<<<<<<< HEAD
		md_size_sect += MD_BM_OFFSET;

		bdev->md.md_size_sect = md_size_sect;
		/* bitmap offset is adjusted by 'super' block size */
		bdev->md.bm_offset   = -md_size_sect + MD_AL_OFFSET;
=======
		md_size_sect += MD_4kB_SECT + al_size_sect;

		bdev->md.md_size_sect = md_size_sect;
		/* bitmap offset is adjusted by 'super' block size */
		bdev->md.bm_offset   = -md_size_sect + MD_4kB_SECT;
>>>>>>> refs/remotes/origin/master
		break;
	}
}

/* input size is expected to be in KB */
char *ppsize(char *buf, unsigned long long size)
{
	/* Needs 9 bytes at max including trailing NUL:
	 * -1ULL ==> "16384 EB" */
	static char units[] = { 'K', 'M', 'G', 'T', 'P', 'E' };
	int base = 0;
	while (size >= 10000 && base < sizeof(units)-1) {
		/* shift + round */
		size = (size >> 10) + !!(size & (1<<9));
		base++;
	}
	sprintf(buf, "%u %cB", (unsigned)size, units[base]);

	return buf;
}

/* there is still a theoretical deadlock when called from receiver
 * on an D_INCONSISTENT R_PRIMARY:
 *  remote READ does inc_ap_bio, receiver would need to receive answer
 *  packet from remote to dec_ap_bio again.
 *  receiver receive_sizes(), comes here,
 *  waits for ap_bio_cnt == 0. -> deadlock.
 * but this cannot happen, actually, because:
 *  R_PRIMARY D_INCONSISTENT, and peer's disk is unreachable
 *  (not connected, or bad/no disk on peer):
 *  see drbd_fail_request_early, ap_bio_cnt is zero.
 *  R_PRIMARY D_INCONSISTENT, and C_SYNC_TARGET:
 *  peer may not initiate a resize.
 */
<<<<<<< HEAD
void drbd_suspend_io(struct drbd_conf *mdev)
{
	set_bit(SUSPEND_IO, &mdev->flags);
	if (is_susp(mdev->state))
=======
/* Note these are not to be confused with
 * drbd_adm_suspend_io/drbd_adm_resume_io,
 * which are (sub) state changes triggered by admin (drbdsetup),
 * and can be long lived.
 * This changes an mdev->flag, is triggered by drbd internals,
 * and should be short-lived. */
void drbd_suspend_io(struct drbd_conf *mdev)
{
	set_bit(SUSPEND_IO, &mdev->flags);
	if (drbd_suspended(mdev))
>>>>>>> refs/remotes/origin/master
		return;
	wait_event(mdev->misc_wait, !atomic_read(&mdev->ap_bio_cnt));
}

void drbd_resume_io(struct drbd_conf *mdev)
{
	clear_bit(SUSPEND_IO, &mdev->flags);
	wake_up(&mdev->misc_wait);
}

/**
 * drbd_determine_dev_size() -  Sets the right device size obeying all constraints
 * @mdev:	DRBD device.
 *
 * Returns 0 on success, negative return values indicate errors.
 * You should call drbd_md_sync() after calling this function.
 */
<<<<<<< HEAD
enum determine_dev_size drbd_determine_dev_size(struct drbd_conf *mdev, enum dds_flags flags) __must_hold(local)
{
	sector_t prev_first_sect, prev_size; /* previous meta location */
	sector_t la_size;
	sector_t size;
	char ppb[10];

	int md_moved, la_size_changed;
	enum determine_dev_size rv = unchanged;
=======
enum determine_dev_size
drbd_determine_dev_size(struct drbd_conf *mdev, enum dds_flags flags, struct resize_parms *rs) __must_hold(local)
{
	sector_t prev_first_sect, prev_size; /* previous meta location */
	sector_t la_size_sect, u_size;
	struct drbd_md *md = &mdev->ldev->md;
	u32 prev_al_stripe_size_4k;
	u32 prev_al_stripes;
	sector_t size;
	char ppb[10];
	void *buffer;

	int md_moved, la_size_changed;
	enum determine_dev_size rv = DS_UNCHANGED;
>>>>>>> refs/remotes/origin/master

	/* race:
	 * application request passes inc_ap_bio,
	 * but then cannot get an AL-reference.
	 * this function later may wait on ap_bio_cnt == 0. -> deadlock.
	 *
	 * to avoid that:
	 * Suspend IO right here.
	 * still lock the act_log to not trigger ASSERTs there.
	 */
	drbd_suspend_io(mdev);
<<<<<<< HEAD
=======
	buffer = drbd_md_get_buffer(mdev); /* Lock meta-data IO */
	if (!buffer) {
		drbd_resume_io(mdev);
		return DS_ERROR;
	}
>>>>>>> refs/remotes/origin/master

	/* no wait necessary anymore, actually we could assert that */
	wait_event(mdev->al_wait, lc_try_lock(mdev->act_log));

	prev_first_sect = drbd_md_first_sector(mdev->ldev);
	prev_size = mdev->ldev->md.md_size_sect;
<<<<<<< HEAD
	la_size = mdev->ldev->md.la_size_sect;

	/* TODO: should only be some assert here, not (re)init... */
	drbd_md_set_sector_offsets(mdev, mdev->ldev);

	size = drbd_new_dev_size(mdev, mdev->ldev, flags & DDSF_FORCED);
=======
	la_size_sect = mdev->ldev->md.la_size_sect;

	if (rs) {
		/* rs is non NULL if we should change the AL layout only */

		prev_al_stripes = md->al_stripes;
		prev_al_stripe_size_4k = md->al_stripe_size_4k;

		md->al_stripes = rs->al_stripes;
		md->al_stripe_size_4k = rs->al_stripe_size / 4;
		md->al_size_4k = (u64)rs->al_stripes * rs->al_stripe_size / 4;
	}

	drbd_md_set_sector_offsets(mdev, mdev->ldev);

	rcu_read_lock();
	u_size = rcu_dereference(mdev->ldev->disk_conf)->disk_size;
	rcu_read_unlock();
	size = drbd_new_dev_size(mdev, mdev->ldev, u_size, flags & DDSF_FORCED);

	if (size < la_size_sect) {
		if (rs && u_size == 0) {
			/* Remove "rs &&" later. This check should always be active, but
			   right now the receiver expects the permissive behavior */
			dev_warn(DEV, "Implicit shrink not allowed. "
				 "Use --size=%llus for explicit shrink.\n",
				 (unsigned long long)size);
			rv = DS_ERROR_SHRINK;
		}
		if (u_size > size)
			rv = DS_ERROR_SPACE_MD;
		if (rv != DS_UNCHANGED)
			goto err_out;
	}
>>>>>>> refs/remotes/origin/master

	if (drbd_get_capacity(mdev->this_bdev) != size ||
	    drbd_bm_capacity(mdev) != size) {
		int err;
		err = drbd_bm_resize(mdev, size, !(flags & DDSF_NO_RESYNC));
		if (unlikely(err)) {
			/* currently there is only one error: ENOMEM! */
			size = drbd_bm_capacity(mdev)>>1;
			if (size == 0) {
				dev_err(DEV, "OUT OF MEMORY! "
				    "Could not allocate bitmap!\n");
			} else {
				dev_err(DEV, "BM resizing failed. "
				    "Leaving size unchanged at size = %lu KB\n",
				    (unsigned long)size);
			}
<<<<<<< HEAD
			rv = dev_size_error;
=======
			rv = DS_ERROR;
>>>>>>> refs/remotes/origin/master
		}
		/* racy, see comments above. */
		drbd_set_my_capacity(mdev, size);
		mdev->ldev->md.la_size_sect = size;
		dev_info(DEV, "size = %s (%llu KB)\n", ppsize(ppb, size>>1),
		     (unsigned long long)size>>1);
	}
<<<<<<< HEAD
	if (rv == dev_size_error)
		goto out;

	la_size_changed = (la_size != mdev->ldev->md.la_size_sect);
=======
	if (rv <= DS_ERROR)
		goto err_out;

	la_size_changed = (la_size_sect != mdev->ldev->md.la_size_sect);
>>>>>>> refs/remotes/origin/master

	md_moved = prev_first_sect != drbd_md_first_sector(mdev->ldev)
		|| prev_size	   != mdev->ldev->md.md_size_sect;

<<<<<<< HEAD
	if (la_size_changed || md_moved) {
		int err;

		drbd_al_shrink(mdev); /* All extents inactive. */
=======
	if (la_size_changed || md_moved || rs) {
		u32 prev_flags;

		drbd_al_shrink(mdev); /* All extents inactive. */

		prev_flags = md->flags;
		md->flags &= ~MDF_PRIMARY_IND;
		drbd_md_write(mdev, buffer);

>>>>>>> refs/remotes/origin/master
		dev_info(DEV, "Writing the whole bitmap, %s\n",
			 la_size_changed && md_moved ? "size changed and md moved" :
			 la_size_changed ? "size changed" : "md moved");
		/* next line implicitly does drbd_suspend_io()+drbd_resume_io() */
<<<<<<< HEAD
		err = drbd_bitmap_io(mdev, &drbd_bm_write,
				"size changed", BM_LOCKED_MASK);
		if (err) {
			rv = dev_size_error;
			goto out;
		}
		drbd_md_mark_dirty(mdev);
	}

	if (size > la_size)
		rv = grew;
	if (size < la_size)
		rv = shrunk;
out:
	lc_unlock(mdev->act_log);
	wake_up(&mdev->al_wait);
=======
		drbd_bitmap_io(mdev, md_moved ? &drbd_bm_write_all : &drbd_bm_write,
			       "size changed", BM_LOCKED_MASK);
		drbd_initialize_al(mdev, buffer);

		md->flags = prev_flags;
		drbd_md_write(mdev, buffer);

		if (rs)
			dev_info(DEV, "Changed AL layout to al-stripes = %d, al-stripe-size-kB = %d\n",
				 md->al_stripes, md->al_stripe_size_4k * 4);
	}

	if (size > la_size_sect)
		rv = la_size_sect ? DS_GREW : DS_GREW_FROM_ZERO;
	if (size < la_size_sect)
		rv = DS_SHRUNK;

	if (0) {
	err_out:
		if (rs) {
			md->al_stripes = prev_al_stripes;
			md->al_stripe_size_4k = prev_al_stripe_size_4k;
			md->al_size_4k = (u64)prev_al_stripes * prev_al_stripe_size_4k;

			drbd_md_set_sector_offsets(mdev, mdev->ldev);
		}
	}
	lc_unlock(mdev->act_log);
	wake_up(&mdev->al_wait);
	drbd_md_put_buffer(mdev);
>>>>>>> refs/remotes/origin/master
	drbd_resume_io(mdev);

	return rv;
}

sector_t
<<<<<<< HEAD
drbd_new_dev_size(struct drbd_conf *mdev, struct drbd_backing_dev *bdev, int assume_peer_has_space)
{
	sector_t p_size = mdev->p_size;   /* partner's disk size. */
	sector_t la_size = bdev->md.la_size_sect; /* last agreed size. */
	sector_t m_size; /* my size */
	sector_t u_size = bdev->dc.disk_size; /* size requested by user. */
=======
drbd_new_dev_size(struct drbd_conf *mdev, struct drbd_backing_dev *bdev,
		  sector_t u_size, int assume_peer_has_space)
{
	sector_t p_size = mdev->p_size;   /* partner's disk size. */
	sector_t la_size_sect = bdev->md.la_size_sect; /* last agreed size. */
	sector_t m_size; /* my size */
>>>>>>> refs/remotes/origin/master
	sector_t size = 0;

	m_size = drbd_get_max_capacity(bdev);

	if (mdev->state.conn < C_CONNECTED && assume_peer_has_space) {
		dev_warn(DEV, "Resize while not connected was forced by the user!\n");
		p_size = m_size;
	}

	if (p_size && m_size) {
		size = min_t(sector_t, p_size, m_size);
	} else {
<<<<<<< HEAD
		if (la_size) {
			size = la_size;
=======
		if (la_size_sect) {
			size = la_size_sect;
>>>>>>> refs/remotes/origin/master
			if (m_size && m_size < size)
				size = m_size;
			if (p_size && p_size < size)
				size = p_size;
		} else {
			if (m_size)
				size = m_size;
			if (p_size)
				size = p_size;
		}
	}

	if (size == 0)
		dev_err(DEV, "Both nodes diskless!\n");

	if (u_size) {
		if (u_size > size)
			dev_err(DEV, "Requested disk size is too big (%lu > %lu)\n",
			    (unsigned long)u_size>>1, (unsigned long)size>>1);
		else
			size = u_size;
	}

	return size;
}

/**
 * drbd_check_al_size() - Ensures that the AL is of the right size
 * @mdev:	DRBD device.
 *
 * Returns -EBUSY if current al lru is still used, -ENOMEM when allocation
 * failed, and 0 on success. You should call drbd_md_sync() after you called
 * this function.
 */
<<<<<<< HEAD
static int drbd_check_al_size(struct drbd_conf *mdev)
=======
static int drbd_check_al_size(struct drbd_conf *mdev, struct disk_conf *dc)
>>>>>>> refs/remotes/origin/master
{
	struct lru_cache *n, *t;
	struct lc_element *e;
	unsigned int in_use;
	int i;

<<<<<<< HEAD
	ERR_IF(mdev->sync_conf.al_extents < 7)
		mdev->sync_conf.al_extents = 127;

	if (mdev->act_log &&
	    mdev->act_log->nr_elements == mdev->sync_conf.al_extents)
=======
	if (mdev->act_log &&
	    mdev->act_log->nr_elements == dc->al_extents)
>>>>>>> refs/remotes/origin/master
		return 0;

	in_use = 0;
	t = mdev->act_log;
<<<<<<< HEAD
	n = lc_create("act_log", drbd_al_ext_cache,
		mdev->sync_conf.al_extents, sizeof(struct lc_element), 0);
=======
	n = lc_create("act_log", drbd_al_ext_cache, AL_UPDATES_PER_TRANSACTION,
		dc->al_extents, sizeof(struct lc_element), 0);
>>>>>>> refs/remotes/origin/master

	if (n == NULL) {
		dev_err(DEV, "Cannot allocate act_log lru!\n");
		return -ENOMEM;
	}
	spin_lock_irq(&mdev->al_lock);
	if (t) {
		for (i = 0; i < t->nr_elements; i++) {
			e = lc_element_by_index(t, i);
			if (e->refcnt)
				dev_err(DEV, "refcnt(%d)==%d\n",
				    e->lc_number, e->refcnt);
			in_use += e->refcnt;
		}
	}
	if (!in_use)
		mdev->act_log = n;
	spin_unlock_irq(&mdev->al_lock);
	if (in_use) {
		dev_err(DEV, "Activity log still in use!\n");
		lc_destroy(n);
		return -EBUSY;
	} else {
		if (t)
			lc_destroy(t);
	}
	drbd_md_mark_dirty(mdev); /* we changed mdev->act_log->nr_elemens */
	return 0;
}

static void drbd_setup_queue_param(struct drbd_conf *mdev, unsigned int max_bio_size)
{
	struct request_queue * const q = mdev->rq_queue;
<<<<<<< HEAD
	int max_hw_sectors = max_bio_size >> 9;
	int max_segments = 0;
=======
	unsigned int max_hw_sectors = max_bio_size >> 9;
	unsigned int max_segments = 0;
>>>>>>> refs/remotes/origin/master

	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		struct request_queue * const b = mdev->ldev->backing_bdev->bd_disk->queue;

		max_hw_sectors = min(queue_max_hw_sectors(b), max_bio_size >> 9);
<<<<<<< HEAD
		max_segments = mdev->ldev->dc.max_bio_bvecs;
=======
		rcu_read_lock();
		max_segments = rcu_dereference(mdev->ldev->disk_conf)->max_bio_bvecs;
		rcu_read_unlock();
>>>>>>> refs/remotes/origin/master
		put_ldev(mdev);
	}

	blk_queue_logical_block_size(q, 512);
	blk_queue_max_hw_sectors(q, max_hw_sectors);
	/* This is the workaround for "bio would need to, but cannot, be split" */
	blk_queue_max_segments(q, max_segments ? max_segments : BLK_MAX_SEGMENTS);
	blk_queue_segment_boundary(q, PAGE_CACHE_SIZE-1);

	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		struct request_queue * const b = mdev->ldev->backing_bdev->bd_disk->queue;

		blk_queue_stack_limits(q, b);

		if (q->backing_dev_info.ra_pages != b->backing_dev_info.ra_pages) {
			dev_info(DEV, "Adjusting my ra_pages to backing device's (%lu -> %lu)\n",
				 q->backing_dev_info.ra_pages,
				 b->backing_dev_info.ra_pages);
			q->backing_dev_info.ra_pages = b->backing_dev_info.ra_pages;
		}
		put_ldev(mdev);
	}
}

void drbd_reconsider_max_bio_size(struct drbd_conf *mdev)
{
<<<<<<< HEAD
	int now, new, local, peer;
=======
	unsigned int now, new, local, peer;
>>>>>>> refs/remotes/origin/master

	now = queue_max_hw_sectors(mdev->rq_queue) << 9;
	local = mdev->local_max_bio_size; /* Eventually last known value, from volatile memory */
	peer = mdev->peer_max_bio_size; /* Eventually last known value, from meta data */

	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		local = queue_max_hw_sectors(mdev->ldev->backing_bdev->bd_disk->queue) << 9;
		mdev->local_max_bio_size = local;
		put_ldev(mdev);
	}
<<<<<<< HEAD
=======
	local = min(local, DRBD_MAX_BIO_SIZE);
>>>>>>> refs/remotes/origin/master

	/* We may ignore peer limits if the peer is modern enough.
	   Because new from 8.3.8 onwards the peer can use multiple
	   BIOs for a single peer_request */
<<<<<<< HEAD
	if (mdev->state.conn >= C_CONNECTED) {
		if (mdev->agreed_pro_version < 94)
			peer = mdev->peer_max_bio_size;
		else if (mdev->agreed_pro_version == 94)
			peer = DRBD_MAX_SIZE_H80_PACKET;
		else /* drbd 8.3.8 onwards */
			peer = DRBD_MAX_BIO_SIZE;
	}

	new = min_t(int, local, peer);

	if (mdev->state.role == R_PRIMARY && new < now)
		dev_err(DEV, "ASSERT FAILED new < now; (%d < %d)\n", new, now);
=======
	if (mdev->state.conn >= C_WF_REPORT_PARAMS) {
		if (mdev->tconn->agreed_pro_version < 94)
			peer = min(mdev->peer_max_bio_size, DRBD_MAX_SIZE_H80_PACKET);
			/* Correct old drbd (up to 8.3.7) if it believes it can do more than 32KiB */
		else if (mdev->tconn->agreed_pro_version == 94)
			peer = DRBD_MAX_SIZE_H80_PACKET;
		else if (mdev->tconn->agreed_pro_version < 100)
			peer = DRBD_MAX_BIO_SIZE_P95;  /* drbd 8.3.8 onwards, before 8.4.0 */
		else
			peer = DRBD_MAX_BIO_SIZE;
	}

	new = min(local, peer);

	if (mdev->state.role == R_PRIMARY && new < now)
		dev_err(DEV, "ASSERT FAILED new < now; (%u < %u)\n", new, now);
>>>>>>> refs/remotes/origin/master

	if (new != now)
		dev_info(DEV, "max BIO size = %u\n", new);

	drbd_setup_queue_param(mdev, new);
}

<<<<<<< HEAD
/* serialize deconfig (worker exiting, doing cleanup)
 * and reconfig (drbdsetup disk, drbdsetup net)
 *
 * Wait for a potentially exiting worker, then restart it,
 * or start a new one.  Flush any pending work, there may still be an
 * after_state_change queued.
 */
static void drbd_reconfig_start(struct drbd_conf *mdev)
{
	wait_event(mdev->state_wait, !test_and_set_bit(CONFIG_PENDING, &mdev->flags));
	wait_event(mdev->state_wait, !test_bit(DEVICE_DYING, &mdev->flags));
	drbd_thread_start(&mdev->worker);
	drbd_flush_workqueue(mdev);
}

/* if still unconfigured, stops worker again.
 * if configured now, clears CONFIG_PENDING.
 * wakes potential waiters */
static void drbd_reconfig_done(struct drbd_conf *mdev)
{
	spin_lock_irq(&mdev->req_lock);
	if (mdev->state.disk == D_DISKLESS &&
	    mdev->state.conn == C_STANDALONE &&
	    mdev->state.role == R_SECONDARY) {
		set_bit(DEVICE_DYING, &mdev->flags);
		drbd_thread_stop_nowait(&mdev->worker);
	} else
		clear_bit(CONFIG_PENDING, &mdev->flags);
	spin_unlock_irq(&mdev->req_lock);
	wake_up(&mdev->state_wait);
=======
/* Starts the worker thread */
static void conn_reconfig_start(struct drbd_tconn *tconn)
{
	drbd_thread_start(&tconn->worker);
	conn_flush_workqueue(tconn);
}

/* if still unconfigured, stops worker again. */
static void conn_reconfig_done(struct drbd_tconn *tconn)
{
	bool stop_threads;
	spin_lock_irq(&tconn->req_lock);
	stop_threads = conn_all_vols_unconf(tconn) &&
		tconn->cstate == C_STANDALONE;
	spin_unlock_irq(&tconn->req_lock);
	if (stop_threads) {
		/* asender is implicitly stopped by receiver
		 * in conn_disconnect() */
		drbd_thread_stop(&tconn->receiver);
		drbd_thread_stop(&tconn->worker);
	}
>>>>>>> refs/remotes/origin/master
}

/* Make sure IO is suspended before calling this function(). */
static void drbd_suspend_al(struct drbd_conf *mdev)
{
	int s = 0;

<<<<<<< HEAD
	if (lc_try_lock(mdev->act_log)) {
		drbd_al_shrink(mdev);
		lc_unlock(mdev->act_log);
	} else {
=======
	if (!lc_try_lock(mdev->act_log)) {
>>>>>>> refs/remotes/origin/master
		dev_warn(DEV, "Failed to lock al in drbd_suspend_al()\n");
		return;
	}

<<<<<<< HEAD
	spin_lock_irq(&mdev->req_lock);
	if (mdev->state.conn < C_CONNECTED)
		s = !test_and_set_bit(AL_SUSPENDED, &mdev->flags);

	spin_unlock_irq(&mdev->req_lock);
=======
	drbd_al_shrink(mdev);
	spin_lock_irq(&mdev->tconn->req_lock);
	if (mdev->state.conn < C_CONNECTED)
		s = !test_and_set_bit(AL_SUSPENDED, &mdev->flags);
	spin_unlock_irq(&mdev->tconn->req_lock);
	lc_unlock(mdev->act_log);
>>>>>>> refs/remotes/origin/master

	if (s)
		dev_info(DEV, "Suspended AL updates\n");
}

<<<<<<< HEAD
/* does always return 0;
 * interesting return code is in reply->ret_code */
static int drbd_nl_disk_conf(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			     struct drbd_nl_cfg_reply *reply)
{
	enum drbd_ret_code retcode;
	enum determine_dev_size dd;
	sector_t max_possible_sectors;
	sector_t min_md_device_sectors;
	struct drbd_backing_dev *nbc = NULL; /* new_backing_conf */
	struct block_device *bdev;
	struct lru_cache *resync_lru = NULL;
	union drbd_state ns, os;
	enum drbd_state_rv rv;
	int cp_discovered = 0;
	int logical_block_size;

	drbd_reconfig_start(mdev);

	/* if you want to reconfigure, please tear down first */
	if (mdev->state.disk > D_DISKLESS) {
		retcode = ERR_DISK_CONFIGURED;
		goto fail;
	}
	/* It may just now have detached because of IO error.  Make sure
	 * drbd_ldev_destroy is done already, we may end up here very fast,
	 * e.g. if someone calls attach from the on-io-error handler,
	 * to realize a "hot spare" feature (not that I'd recommend that) */
	wait_event(mdev->misc_wait, !atomic_read(&mdev->local_cnt));

	/* allocation not in the IO path, cqueue thread context */
	nbc = kzalloc(sizeof(struct drbd_backing_dev), GFP_KERNEL);
	if (!nbc) {
=======

static bool should_set_defaults(struct genl_info *info)
{
	unsigned flags = ((struct drbd_genlmsghdr*)info->userhdr)->flags;
	return 0 != (flags & DRBD_GENL_F_SET_DEFAULTS);
}

static unsigned int drbd_al_extents_max(struct drbd_backing_dev *bdev)
{
	/* This is limited by 16 bit "slot" numbers,
	 * and by available on-disk context storage.
	 *
	 * Also (u16)~0 is special (denotes a "free" extent).
	 *
	 * One transaction occupies one 4kB on-disk block,
	 * we have n such blocks in the on disk ring buffer,
	 * the "current" transaction may fail (n-1),
	 * and there is 919 slot numbers context information per transaction.
	 *
	 * 72 transaction blocks amounts to more than 2**16 context slots,
	 * so cap there first.
	 */
	const unsigned int max_al_nr = DRBD_AL_EXTENTS_MAX;
	const unsigned int sufficient_on_disk =
		(max_al_nr + AL_CONTEXT_PER_TRANSACTION -1)
		/AL_CONTEXT_PER_TRANSACTION;

	unsigned int al_size_4k = bdev->md.al_size_4k;

	if (al_size_4k > sufficient_on_disk)
		return max_al_nr;

	return (al_size_4k - 1) * AL_CONTEXT_PER_TRANSACTION;
}

int drbd_adm_disk_opts(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;
	struct drbd_conf *mdev;
	struct disk_conf *new_disk_conf, *old_disk_conf;
	struct fifo_buffer *old_plan = NULL, *new_plan = NULL;
	int err, fifo_size;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	mdev = adm_ctx.mdev;

	/* we also need a disk
	 * to change the options on */
	if (!get_ldev(mdev)) {
		retcode = ERR_NO_DISK;
		goto out;
	}

	new_disk_conf = kmalloc(sizeof(struct disk_conf), GFP_KERNEL);
	if (!new_disk_conf) {
>>>>>>> refs/remotes/origin/master
		retcode = ERR_NOMEM;
		goto fail;
	}

<<<<<<< HEAD
	nbc->dc.disk_size     = DRBD_DISK_SIZE_SECT_DEF;
	nbc->dc.on_io_error   = DRBD_ON_IO_ERROR_DEF;
	nbc->dc.fencing       = DRBD_FENCING_DEF;
	nbc->dc.max_bio_bvecs = DRBD_MAX_BIO_BVECS_DEF;

	if (!disk_conf_from_tags(mdev, nlp->tag_list, &nbc->dc)) {
		retcode = ERR_MANDATORY_TAG;
		goto fail;
	}

	if (nbc->dc.meta_dev_idx < DRBD_MD_INDEX_FLEX_INT) {
		retcode = ERR_MD_IDX_INVALID;
		goto fail;
	}

	if (get_net_conf(mdev)) {
		int prot = mdev->net_conf->wire_protocol;
		put_net_conf(mdev);
		if (nbc->dc.fencing == FP_STONITH && prot == DRBD_PROT_A) {
			retcode = ERR_STONITH_AND_PROT_A;
			goto fail;
		}
	}

	bdev = blkdev_get_by_path(nbc->dc.backing_dev,
				  FMODE_READ | FMODE_WRITE | FMODE_EXCL, mdev);
	if (IS_ERR(bdev)) {
		dev_err(DEV, "open(\"%s\") failed with %ld\n", nbc->dc.backing_dev,
=======
	mutex_lock(&mdev->tconn->conf_update);
	old_disk_conf = mdev->ldev->disk_conf;
	*new_disk_conf = *old_disk_conf;
	if (should_set_defaults(info))
		set_disk_conf_defaults(new_disk_conf);

	err = disk_conf_from_attrs_for_change(new_disk_conf, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(from_attrs_err_to_txt(err));
	}

	if (!expect(new_disk_conf->resync_rate >= 1))
		new_disk_conf->resync_rate = 1;

	if (new_disk_conf->al_extents < DRBD_AL_EXTENTS_MIN)
		new_disk_conf->al_extents = DRBD_AL_EXTENTS_MIN;
	if (new_disk_conf->al_extents > drbd_al_extents_max(mdev->ldev))
		new_disk_conf->al_extents = drbd_al_extents_max(mdev->ldev);

	if (new_disk_conf->c_plan_ahead > DRBD_C_PLAN_AHEAD_MAX)
		new_disk_conf->c_plan_ahead = DRBD_C_PLAN_AHEAD_MAX;

	fifo_size = (new_disk_conf->c_plan_ahead * 10 * SLEEP_TIME) / HZ;
	if (fifo_size != mdev->rs_plan_s->size) {
		new_plan = fifo_alloc(fifo_size);
		if (!new_plan) {
			dev_err(DEV, "kmalloc of fifo_buffer failed");
			retcode = ERR_NOMEM;
			goto fail_unlock;
		}
	}

	drbd_suspend_io(mdev);
	wait_event(mdev->al_wait, lc_try_lock(mdev->act_log));
	drbd_al_shrink(mdev);
	err = drbd_check_al_size(mdev, new_disk_conf);
	lc_unlock(mdev->act_log);
	wake_up(&mdev->al_wait);
	drbd_resume_io(mdev);

	if (err) {
		retcode = ERR_NOMEM;
		goto fail_unlock;
	}

	write_lock_irq(&global_state_lock);
	retcode = drbd_resync_after_valid(mdev, new_disk_conf->resync_after);
	if (retcode == NO_ERROR) {
		rcu_assign_pointer(mdev->ldev->disk_conf, new_disk_conf);
		drbd_resync_after_changed(mdev);
	}
	write_unlock_irq(&global_state_lock);

	if (retcode != NO_ERROR)
		goto fail_unlock;

	if (new_plan) {
		old_plan = mdev->rs_plan_s;
		rcu_assign_pointer(mdev->rs_plan_s, new_plan);
	}

	mutex_unlock(&mdev->tconn->conf_update);

	if (new_disk_conf->al_updates)
		mdev->ldev->md.flags &= ~MDF_AL_DISABLED;
	else
		mdev->ldev->md.flags |= MDF_AL_DISABLED;

	if (new_disk_conf->md_flushes)
		clear_bit(MD_NO_FUA, &mdev->flags);
	else
		set_bit(MD_NO_FUA, &mdev->flags);

	drbd_bump_write_ordering(mdev->tconn, WO_bdev_flush);

	drbd_md_sync(mdev);

	if (mdev->state.conn >= C_CONNECTED)
		drbd_send_sync_param(mdev);

	synchronize_rcu();
	kfree(old_disk_conf);
	kfree(old_plan);
	mod_timer(&mdev->request_timer, jiffies + HZ);
	goto success;

fail_unlock:
	mutex_unlock(&mdev->tconn->conf_update);
 fail:
	kfree(new_disk_conf);
	kfree(new_plan);
success:
	put_ldev(mdev);
 out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_attach(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_conf *mdev;
	int err;
	enum drbd_ret_code retcode;
	enum determine_dev_size dd;
	sector_t max_possible_sectors;
	sector_t min_md_device_sectors;
	struct drbd_backing_dev *nbc = NULL; /* new_backing_conf */
	struct disk_conf *new_disk_conf = NULL;
	struct block_device *bdev;
	struct lru_cache *resync_lru = NULL;
	struct fifo_buffer *new_plan = NULL;
	union drbd_state ns, os;
	enum drbd_state_rv rv;
	struct net_conf *nc;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto finish;

	mdev = adm_ctx.mdev;
	conn_reconfig_start(mdev->tconn);

	/* if you want to reconfigure, please tear down first */
	if (mdev->state.disk > D_DISKLESS) {
		retcode = ERR_DISK_CONFIGURED;
		goto fail;
	}
	/* It may just now have detached because of IO error.  Make sure
	 * drbd_ldev_destroy is done already, we may end up here very fast,
	 * e.g. if someone calls attach from the on-io-error handler,
	 * to realize a "hot spare" feature (not that I'd recommend that) */
	wait_event(mdev->misc_wait, !atomic_read(&mdev->local_cnt));

	/* make sure there is no leftover from previous force-detach attempts */
	clear_bit(FORCE_DETACH, &mdev->flags);
	clear_bit(WAS_IO_ERROR, &mdev->flags);
	clear_bit(WAS_READ_ERROR, &mdev->flags);

	/* and no leftover from previously aborted resync or verify, either */
	mdev->rs_total = 0;
	mdev->rs_failed = 0;
	atomic_set(&mdev->rs_pending_cnt, 0);

	/* allocation not in the IO path, drbdsetup context */
	nbc = kzalloc(sizeof(struct drbd_backing_dev), GFP_KERNEL);
	if (!nbc) {
		retcode = ERR_NOMEM;
		goto fail;
	}
	spin_lock_init(&nbc->md.uuid_lock);

	new_disk_conf = kzalloc(sizeof(struct disk_conf), GFP_KERNEL);
	if (!new_disk_conf) {
		retcode = ERR_NOMEM;
		goto fail;
	}
	nbc->disk_conf = new_disk_conf;

	set_disk_conf_defaults(new_disk_conf);
	err = disk_conf_from_attrs(new_disk_conf, info);
	if (err) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(from_attrs_err_to_txt(err));
		goto fail;
	}

	if (new_disk_conf->c_plan_ahead > DRBD_C_PLAN_AHEAD_MAX)
		new_disk_conf->c_plan_ahead = DRBD_C_PLAN_AHEAD_MAX;

	new_plan = fifo_alloc((new_disk_conf->c_plan_ahead * 10 * SLEEP_TIME) / HZ);
	if (!new_plan) {
		retcode = ERR_NOMEM;
		goto fail;
	}

	if (new_disk_conf->meta_dev_idx < DRBD_MD_INDEX_FLEX_INT) {
		retcode = ERR_MD_IDX_INVALID;
		goto fail;
	}

	write_lock_irq(&global_state_lock);
	retcode = drbd_resync_after_valid(mdev, new_disk_conf->resync_after);
	write_unlock_irq(&global_state_lock);
	if (retcode != NO_ERROR)
		goto fail;

	rcu_read_lock();
	nc = rcu_dereference(mdev->tconn->net_conf);
	if (nc) {
		if (new_disk_conf->fencing == FP_STONITH && nc->wire_protocol == DRBD_PROT_A) {
			rcu_read_unlock();
			retcode = ERR_STONITH_AND_PROT_A;
			goto fail;
		}
	}
	rcu_read_unlock();

	bdev = blkdev_get_by_path(new_disk_conf->backing_dev,
				  FMODE_READ | FMODE_WRITE | FMODE_EXCL, mdev);
	if (IS_ERR(bdev)) {
		dev_err(DEV, "open(\"%s\") failed with %ld\n", new_disk_conf->backing_dev,
>>>>>>> refs/remotes/origin/master
			PTR_ERR(bdev));
		retcode = ERR_OPEN_DISK;
		goto fail;
	}
	nbc->backing_bdev = bdev;

	/*
	 * meta_dev_idx >= 0: external fixed size, possibly multiple
	 * drbd sharing one meta device.  TODO in that case, paranoia
	 * check that [md_bdev, meta_dev_idx] is not yet used by some
	 * other drbd minor!  (if you use drbd.conf + drbdadm, that
	 * should check it for you already; but if you don't, or
	 * someone fooled it, we need to double check here)
	 */
<<<<<<< HEAD
	bdev = blkdev_get_by_path(nbc->dc.meta_dev,
				  FMODE_READ | FMODE_WRITE | FMODE_EXCL,
				  (nbc->dc.meta_dev_idx < 0) ?
				  (void *)mdev : (void *)drbd_m_holder);
	if (IS_ERR(bdev)) {
		dev_err(DEV, "open(\"%s\") failed with %ld\n", nbc->dc.meta_dev,
=======
	bdev = blkdev_get_by_path(new_disk_conf->meta_dev,
				  FMODE_READ | FMODE_WRITE | FMODE_EXCL,
				  (new_disk_conf->meta_dev_idx < 0) ?
				  (void *)mdev : (void *)drbd_m_holder);
	if (IS_ERR(bdev)) {
		dev_err(DEV, "open(\"%s\") failed with %ld\n", new_disk_conf->meta_dev,
>>>>>>> refs/remotes/origin/master
			PTR_ERR(bdev));
		retcode = ERR_OPEN_MD_DISK;
		goto fail;
	}
	nbc->md_bdev = bdev;

	if ((nbc->backing_bdev == nbc->md_bdev) !=
<<<<<<< HEAD
	    (nbc->dc.meta_dev_idx == DRBD_MD_INDEX_INTERNAL ||
	     nbc->dc.meta_dev_idx == DRBD_MD_INDEX_FLEX_INT)) {
=======
	    (new_disk_conf->meta_dev_idx == DRBD_MD_INDEX_INTERNAL ||
	     new_disk_conf->meta_dev_idx == DRBD_MD_INDEX_FLEX_INT)) {
>>>>>>> refs/remotes/origin/master
		retcode = ERR_MD_IDX_INVALID;
		goto fail;
	}

	resync_lru = lc_create("resync", drbd_bm_ext_cache,
<<<<<<< HEAD
			61, sizeof(struct bm_extent),
=======
			1, 61, sizeof(struct bm_extent),
>>>>>>> refs/remotes/origin/master
			offsetof(struct bm_extent, lce));
	if (!resync_lru) {
		retcode = ERR_NOMEM;
		goto fail;
	}

<<<<<<< HEAD
	/* RT - for drbd_get_max_capacity() DRBD_MD_INDEX_FLEX_INT */
	drbd_md_set_sector_offsets(mdev, nbc);

	if (drbd_get_max_capacity(nbc) < nbc->dc.disk_size) {
		dev_err(DEV, "max capacity %llu smaller than disk size %llu\n",
			(unsigned long long) drbd_get_max_capacity(nbc),
			(unsigned long long) nbc->dc.disk_size);
		retcode = ERR_DISK_TO_SMALL;
		goto fail;
	}

	if (nbc->dc.meta_dev_idx < 0) {
=======
	/* Read our meta data super block early.
	 * This also sets other on-disk offsets. */
	retcode = drbd_md_read(mdev, nbc);
	if (retcode != NO_ERROR)
		goto fail;

	if (new_disk_conf->al_extents < DRBD_AL_EXTENTS_MIN)
		new_disk_conf->al_extents = DRBD_AL_EXTENTS_MIN;
	if (new_disk_conf->al_extents > drbd_al_extents_max(nbc))
		new_disk_conf->al_extents = drbd_al_extents_max(nbc);

	if (drbd_get_max_capacity(nbc) < new_disk_conf->disk_size) {
		dev_err(DEV, "max capacity %llu smaller than disk size %llu\n",
			(unsigned long long) drbd_get_max_capacity(nbc),
			(unsigned long long) new_disk_conf->disk_size);
		retcode = ERR_DISK_TOO_SMALL;
		goto fail;
	}

	if (new_disk_conf->meta_dev_idx < 0) {
>>>>>>> refs/remotes/origin/master
		max_possible_sectors = DRBD_MAX_SECTORS_FLEX;
		/* at least one MB, otherwise it does not make sense */
		min_md_device_sectors = (2<<10);
	} else {
		max_possible_sectors = DRBD_MAX_SECTORS;
<<<<<<< HEAD
		min_md_device_sectors = MD_RESERVED_SECT * (nbc->dc.meta_dev_idx + 1);
	}

	if (drbd_get_capacity(nbc->md_bdev) < min_md_device_sectors) {
		retcode = ERR_MD_DISK_TO_SMALL;
=======
		min_md_device_sectors = MD_128MB_SECT * (new_disk_conf->meta_dev_idx + 1);
	}

	if (drbd_get_capacity(nbc->md_bdev) < min_md_device_sectors) {
		retcode = ERR_MD_DISK_TOO_SMALL;
>>>>>>> refs/remotes/origin/master
		dev_warn(DEV, "refusing attach: md-device too small, "
		     "at least %llu sectors needed for this meta-disk type\n",
		     (unsigned long long) min_md_device_sectors);
		goto fail;
	}

	/* Make sure the new disk is big enough
	 * (we may currently be R_PRIMARY with no local disk...) */
	if (drbd_get_max_capacity(nbc) <
	    drbd_get_capacity(mdev->this_bdev)) {
<<<<<<< HEAD
		retcode = ERR_DISK_TO_SMALL;
=======
		retcode = ERR_DISK_TOO_SMALL;
>>>>>>> refs/remotes/origin/master
		goto fail;
	}

	nbc->known_size = drbd_get_capacity(nbc->backing_bdev);

	if (nbc->known_size > max_possible_sectors) {
		dev_warn(DEV, "==> truncating very big lower level device "
			"to currently maximum possible %llu sectors <==\n",
			(unsigned long long) max_possible_sectors);
<<<<<<< HEAD
		if (nbc->dc.meta_dev_idx >= 0)
=======
		if (new_disk_conf->meta_dev_idx >= 0)
>>>>>>> refs/remotes/origin/master
			dev_warn(DEV, "==>> using internal or flexible "
				      "meta data may help <<==\n");
	}

	drbd_suspend_io(mdev);
	/* also wait for the last barrier ack. */
<<<<<<< HEAD
	wait_event(mdev->misc_wait, !atomic_read(&mdev->ap_pending_cnt) || is_susp(mdev->state));
=======
	/* FIXME see also https://daiquiri.linbit/cgi-bin/bugzilla/show_bug.cgi?id=171
	 * We need a way to either ignore barrier acks for barriers sent before a device
	 * was attached, or a way to wait for all pending barrier acks to come in.
	 * As barriers are counted per resource,
	 * we'd need to suspend io on all devices of a resource.
	 */
	wait_event(mdev->misc_wait, !atomic_read(&mdev->ap_pending_cnt) || drbd_suspended(mdev));
>>>>>>> refs/remotes/origin/master
	/* and for any other previously queued work */
	drbd_flush_workqueue(mdev);

	rv = _drbd_request_state(mdev, NS(disk, D_ATTACHING), CS_VERBOSE);
	retcode = rv;  /* FIXME: Type mismatch. */
	drbd_resume_io(mdev);
	if (rv < SS_SUCCESS)
		goto fail;

	if (!get_ldev_if_state(mdev, D_ATTACHING))
		goto force_diskless;

<<<<<<< HEAD
	drbd_md_set_sector_offsets(mdev, nbc);

	/* allocate a second IO page if logical_block_size != 512 */
	logical_block_size = bdev_logical_block_size(nbc->md_bdev);
	if (logical_block_size == 0)
		logical_block_size = MD_SECTOR_SIZE;

	if (logical_block_size != MD_SECTOR_SIZE) {
		if (!mdev->md_io_tmpp) {
			struct page *page = alloc_page(GFP_NOIO);
			if (!page)
				goto force_diskless_dec;

			dev_warn(DEV, "Meta data's bdev logical_block_size = %d != %d\n",
			     logical_block_size, MD_SECTOR_SIZE);
			dev_warn(DEV, "Workaround engaged (has performance impact).\n");

			mdev->md_io_tmpp = page;
		}
	}

=======
>>>>>>> refs/remotes/origin/master
	if (!mdev->bitmap) {
		if (drbd_bm_init(mdev)) {
			retcode = ERR_NOMEM;
			goto force_diskless_dec;
		}
	}

<<<<<<< HEAD
	retcode = drbd_md_read(mdev, nbc);
	if (retcode != NO_ERROR)
		goto force_diskless_dec;

=======
>>>>>>> refs/remotes/origin/master
	if (mdev->state.conn < C_CONNECTED &&
	    mdev->state.role == R_PRIMARY &&
	    (mdev->ed_uuid & ~((u64)1)) != (nbc->md.uuid[UI_CURRENT] & ~((u64)1))) {
		dev_err(DEV, "Can only attach to data with current UUID=%016llX\n",
		    (unsigned long long)mdev->ed_uuid);
		retcode = ERR_DATA_NOT_CURRENT;
		goto force_diskless_dec;
	}

	/* Since we are diskless, fix the activity log first... */
<<<<<<< HEAD
	if (drbd_check_al_size(mdev)) {
=======
	if (drbd_check_al_size(mdev, new_disk_conf)) {
>>>>>>> refs/remotes/origin/master
		retcode = ERR_NOMEM;
		goto force_diskless_dec;
	}

	/* Prevent shrinking of consistent devices ! */
	if (drbd_md_test_flag(nbc, MDF_CONSISTENT) &&
<<<<<<< HEAD
	    drbd_new_dev_size(mdev, nbc, 0) < nbc->md.la_size_sect) {
		dev_warn(DEV, "refusing to truncate a consistent device\n");
		retcode = ERR_DISK_TO_SMALL;
		goto force_diskless_dec;
	}

	if (!drbd_al_read_log(mdev, nbc)) {
		retcode = ERR_IO_MD_DISK;
=======
	    drbd_new_dev_size(mdev, nbc, nbc->disk_conf->disk_size, 0) < nbc->md.la_size_sect) {
		dev_warn(DEV, "refusing to truncate a consistent device\n");
		retcode = ERR_DISK_TOO_SMALL;
>>>>>>> refs/remotes/origin/master
		goto force_diskless_dec;
	}

	/* Reset the "barriers don't work" bits here, then force meta data to
	 * be written, to ensure we determine if barriers are supported. */
<<<<<<< HEAD
	if (nbc->dc.no_md_flush)
		set_bit(MD_NO_FUA, &mdev->flags);
	else
		clear_bit(MD_NO_FUA, &mdev->flags);
=======
	if (new_disk_conf->md_flushes)
		clear_bit(MD_NO_FUA, &mdev->flags);
	else
		set_bit(MD_NO_FUA, &mdev->flags);
>>>>>>> refs/remotes/origin/master

	/* Point of no return reached.
	 * Devices and memory are no longer released by error cleanup below.
	 * now mdev takes over responsibility, and the state engine should
	 * clean it up somewhere.  */
	D_ASSERT(mdev->ldev == NULL);
	mdev->ldev = nbc;
	mdev->resync = resync_lru;
<<<<<<< HEAD
	nbc = NULL;
	resync_lru = NULL;

	mdev->write_ordering = WO_bdev_flush;
	drbd_bump_write_ordering(mdev, WO_bdev_flush);
=======
	mdev->rs_plan_s = new_plan;
	nbc = NULL;
	resync_lru = NULL;
	new_disk_conf = NULL;
	new_plan = NULL;

	drbd_bump_write_ordering(mdev->tconn, WO_bdev_flush);
>>>>>>> refs/remotes/origin/master

	if (drbd_md_test_flag(mdev->ldev, MDF_CRASHED_PRIMARY))
		set_bit(CRASHED_PRIMARY, &mdev->flags);
	else
		clear_bit(CRASHED_PRIMARY, &mdev->flags);

	if (drbd_md_test_flag(mdev->ldev, MDF_PRIMARY_IND) &&
<<<<<<< HEAD
	    !(mdev->state.role == R_PRIMARY && mdev->state.susp_nod)) {
		set_bit(CRASHED_PRIMARY, &mdev->flags);
		cp_discovered = 1;
	}
=======
	    !(mdev->state.role == R_PRIMARY && mdev->tconn->susp_nod))
		set_bit(CRASHED_PRIMARY, &mdev->flags);
>>>>>>> refs/remotes/origin/master

	mdev->send_cnt = 0;
	mdev->recv_cnt = 0;
	mdev->read_cnt = 0;
	mdev->writ_cnt = 0;

	drbd_reconsider_max_bio_size(mdev);

	/* If I am currently not R_PRIMARY,
	 * but meta data primary indicator is set,
	 * I just now recover from a hard crash,
	 * and have been R_PRIMARY before that crash.
	 *
	 * Now, if I had no connection before that crash
	 * (have been degraded R_PRIMARY), chances are that
	 * I won't find my peer now either.
	 *
	 * In that case, and _only_ in that case,
	 * we use the degr-wfc-timeout instead of the default,
	 * so we can automatically recover from a crash of a
	 * degraded but active "cluster" after a certain timeout.
	 */
	clear_bit(USE_DEGR_WFC_T, &mdev->flags);
	if (mdev->state.role != R_PRIMARY &&
	     drbd_md_test_flag(mdev->ldev, MDF_PRIMARY_IND) &&
	    !drbd_md_test_flag(mdev->ldev, MDF_CONNECTED_IND))
		set_bit(USE_DEGR_WFC_T, &mdev->flags);

<<<<<<< HEAD
	dd = drbd_determine_dev_size(mdev, 0);
	if (dd == dev_size_error) {
		retcode = ERR_NOMEM_BITMAP;
		goto force_diskless_dec;
	} else if (dd == grew)
		set_bit(RESYNC_AFTER_NEG, &mdev->flags);

	if (drbd_md_test_flag(mdev->ldev, MDF_FULL_SYNC)) {
=======
	dd = drbd_determine_dev_size(mdev, 0, NULL);
	if (dd <= DS_ERROR) {
		retcode = ERR_NOMEM_BITMAP;
		goto force_diskless_dec;
	} else if (dd == DS_GREW)
		set_bit(RESYNC_AFTER_NEG, &mdev->flags);

	if (drbd_md_test_flag(mdev->ldev, MDF_FULL_SYNC) ||
	    (test_bit(CRASHED_PRIMARY, &mdev->flags) &&
	     drbd_md_test_flag(mdev->ldev, MDF_AL_DISABLED))) {
>>>>>>> refs/remotes/origin/master
		dev_info(DEV, "Assuming that all blocks are out of sync "
		     "(aka FullSync)\n");
		if (drbd_bitmap_io(mdev, &drbd_bmio_set_n_write,
			"set_n_write from attaching", BM_LOCKED_MASK)) {
			retcode = ERR_IO_MD_DISK;
			goto force_diskless_dec;
		}
	} else {
		if (drbd_bitmap_io(mdev, &drbd_bm_read,
<<<<<<< HEAD
			"read from attaching", BM_LOCKED_MASK) < 0) {
			retcode = ERR_IO_MD_DISK;
			goto force_diskless_dec;
		}
	}

	if (cp_discovered) {
		drbd_al_apply_to_bm(mdev);
		if (drbd_bitmap_io(mdev, &drbd_bm_write,
			"crashed primary apply AL", BM_LOCKED_MASK)) {
=======
			"read from attaching", BM_LOCKED_MASK)) {
>>>>>>> refs/remotes/origin/master
			retcode = ERR_IO_MD_DISK;
			goto force_diskless_dec;
		}
	}

	if (_drbd_bm_total_weight(mdev) == drbd_bm_bits(mdev))
		drbd_suspend_al(mdev); /* IO is still suspended here... */

<<<<<<< HEAD
	spin_lock_irq(&mdev->req_lock);
	os = mdev->state;
	ns.i = os.i;
=======
	spin_lock_irq(&mdev->tconn->req_lock);
	os = drbd_read_state(mdev);
	ns = os;
>>>>>>> refs/remotes/origin/master
	/* If MDF_CONSISTENT is not set go into inconsistent state,
	   otherwise investigate MDF_WasUpToDate...
	   If MDF_WAS_UP_TO_DATE is not set go into D_OUTDATED disk state,
	   otherwise into D_CONSISTENT state.
	*/
	if (drbd_md_test_flag(mdev->ldev, MDF_CONSISTENT)) {
		if (drbd_md_test_flag(mdev->ldev, MDF_WAS_UP_TO_DATE))
			ns.disk = D_CONSISTENT;
		else
			ns.disk = D_OUTDATED;
	} else {
		ns.disk = D_INCONSISTENT;
	}

	if (drbd_md_test_flag(mdev->ldev, MDF_PEER_OUT_DATED))
		ns.pdsk = D_OUTDATED;

<<<<<<< HEAD
	if ( ns.disk == D_CONSISTENT &&
	    (ns.pdsk == D_OUTDATED || mdev->ldev->dc.fencing == FP_DONT_CARE))
=======
	rcu_read_lock();
	if (ns.disk == D_CONSISTENT &&
	    (ns.pdsk == D_OUTDATED || rcu_dereference(mdev->ldev->disk_conf)->fencing == FP_DONT_CARE))
>>>>>>> refs/remotes/origin/master
		ns.disk = D_UP_TO_DATE;

	/* All tests on MDF_PRIMARY_IND, MDF_CONNECTED_IND,
	   MDF_CONSISTENT and MDF_WAS_UP_TO_DATE must happen before
	   this point, because drbd_request_state() modifies these
	   flags. */

<<<<<<< HEAD
=======
	if (rcu_dereference(mdev->ldev->disk_conf)->al_updates)
		mdev->ldev->md.flags &= ~MDF_AL_DISABLED;
	else
		mdev->ldev->md.flags |= MDF_AL_DISABLED;

	rcu_read_unlock();

>>>>>>> refs/remotes/origin/master
	/* In case we are C_CONNECTED postpone any decision on the new disk
	   state after the negotiation phase. */
	if (mdev->state.conn == C_CONNECTED) {
		mdev->new_state_tmp.i = ns.i;
		ns.i = os.i;
		ns.disk = D_NEGOTIATING;

		/* We expect to receive up-to-date UUIDs soon.
		   To avoid a race in receive_state, free p_uuid while
		   holding req_lock. I.e. atomic with the state change */
		kfree(mdev->p_uuid);
		mdev->p_uuid = NULL;
	}

	rv = _drbd_set_state(mdev, ns, CS_VERBOSE, NULL);
<<<<<<< HEAD
	ns = mdev->state;
	spin_unlock_irq(&mdev->req_lock);
=======
	spin_unlock_irq(&mdev->tconn->req_lock);
>>>>>>> refs/remotes/origin/master

	if (rv < SS_SUCCESS)
		goto force_diskless_dec;

<<<<<<< HEAD
=======
	mod_timer(&mdev->request_timer, jiffies + HZ);

>>>>>>> refs/remotes/origin/master
	if (mdev->state.role == R_PRIMARY)
		mdev->ldev->md.uuid[UI_CURRENT] |=  (u64)1;
	else
		mdev->ldev->md.uuid[UI_CURRENT] &= ~(u64)1;

	drbd_md_mark_dirty(mdev);
	drbd_md_sync(mdev);

	kobject_uevent(&disk_to_dev(mdev->vdisk)->kobj, KOBJ_CHANGE);
	put_ldev(mdev);
<<<<<<< HEAD
	reply->ret_code = retcode;
	drbd_reconfig_done(mdev);
=======
	conn_reconfig_done(mdev->tconn);
	drbd_adm_finish(info, retcode);
>>>>>>> refs/remotes/origin/master
	return 0;

 force_diskless_dec:
	put_ldev(mdev);
 force_diskless:
<<<<<<< HEAD
	drbd_force_state(mdev, NS(disk, D_FAILED));
	drbd_md_sync(mdev);
 fail:
=======
	drbd_force_state(mdev, NS(disk, D_DISKLESS));
	drbd_md_sync(mdev);
 fail:
	conn_reconfig_done(mdev->tconn);
>>>>>>> refs/remotes/origin/master
	if (nbc) {
		if (nbc->backing_bdev)
			blkdev_put(nbc->backing_bdev,
				   FMODE_READ | FMODE_WRITE | FMODE_EXCL);
		if (nbc->md_bdev)
			blkdev_put(nbc->md_bdev,
				   FMODE_READ | FMODE_WRITE | FMODE_EXCL);
		kfree(nbc);
	}
<<<<<<< HEAD
	lc_destroy(resync_lru);

	reply->ret_code = retcode;
	drbd_reconfig_done(mdev);
	return 0;
}

/* Detaching the disk is a process in multiple stages.  First we need to lock
 * out application IO, in-flight IO, IO stuck in drbd_al_begin_io.
 * Then we transition to D_DISKLESS, and wait for put_ldev() to return all
 * internal references as well.
 * Only then we have finally detached. */
static int drbd_nl_detach(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			  struct drbd_nl_cfg_reply *reply)
{
	enum drbd_ret_code retcode;
	int ret;
	drbd_suspend_io(mdev); /* so no-one is stuck in drbd_al_begin_io */
	retcode = drbd_request_state(mdev, NS(disk, D_FAILED));
=======
	kfree(new_disk_conf);
	lc_destroy(resync_lru);
	kfree(new_plan);

 finish:
	drbd_adm_finish(info, retcode);
	return 0;
}

static int adm_detach(struct drbd_conf *mdev, int force)
{
	enum drbd_state_rv retcode;
	int ret;

	if (force) {
		set_bit(FORCE_DETACH, &mdev->flags);
		drbd_force_state(mdev, NS(disk, D_FAILED));
		retcode = SS_SUCCESS;
		goto out;
	}

	drbd_suspend_io(mdev); /* so no-one is stuck in drbd_al_begin_io */
	drbd_md_get_buffer(mdev); /* make sure there is no in-flight meta-data IO */
	retcode = drbd_request_state(mdev, NS(disk, D_FAILED));
	drbd_md_put_buffer(mdev);
>>>>>>> refs/remotes/origin/master
	/* D_FAILED will transition to DISKLESS. */
	ret = wait_event_interruptible(mdev->misc_wait,
			mdev->state.disk != D_FAILED);
	drbd_resume_io(mdev);
	if ((int)retcode == (int)SS_IS_DISKLESS)
		retcode = SS_NOTHING_TO_DO;
	if (ret)
		retcode = ERR_INTR;
<<<<<<< HEAD
	reply->ret_code = retcode;
	return 0;
}

static int drbd_nl_net_conf(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			    struct drbd_nl_cfg_reply *reply)
{
	int i, ns;
	enum drbd_ret_code retcode;
	struct net_conf *new_conf = NULL;
	struct crypto_hash *tfm = NULL;
	struct crypto_hash *integrity_w_tfm = NULL;
	struct crypto_hash *integrity_r_tfm = NULL;
	struct hlist_head *new_tl_hash = NULL;
	struct hlist_head *new_ee_hash = NULL;
	struct drbd_conf *odev;
	char hmac_name[CRYPTO_MAX_ALG_NAME];
	void *int_dig_out = NULL;
	void *int_dig_in = NULL;
	void *int_dig_vv = NULL;
	struct sockaddr *new_my_addr, *new_peer_addr, *taken_addr;

	drbd_reconfig_start(mdev);

	if (mdev->state.conn > C_STANDALONE) {
		retcode = ERR_NET_CONFIGURED;
		goto fail;
	}

	/* allocation not in the IO path, cqueue thread context */
	new_conf = kzalloc(sizeof(struct net_conf), GFP_KERNEL);
	if (!new_conf) {
		retcode = ERR_NOMEM;
		goto fail;
	}

	new_conf->timeout	   = DRBD_TIMEOUT_DEF;
	new_conf->try_connect_int  = DRBD_CONNECT_INT_DEF;
	new_conf->ping_int	   = DRBD_PING_INT_DEF;
	new_conf->max_epoch_size   = DRBD_MAX_EPOCH_SIZE_DEF;
	new_conf->max_buffers	   = DRBD_MAX_BUFFERS_DEF;
	new_conf->unplug_watermark = DRBD_UNPLUG_WATERMARK_DEF;
	new_conf->sndbuf_size	   = DRBD_SNDBUF_SIZE_DEF;
	new_conf->rcvbuf_size	   = DRBD_RCVBUF_SIZE_DEF;
	new_conf->ko_count	   = DRBD_KO_COUNT_DEF;
	new_conf->after_sb_0p	   = DRBD_AFTER_SB_0P_DEF;
	new_conf->after_sb_1p	   = DRBD_AFTER_SB_1P_DEF;
	new_conf->after_sb_2p	   = DRBD_AFTER_SB_2P_DEF;
	new_conf->want_lose	   = 0;
	new_conf->two_primaries    = 0;
	new_conf->wire_protocol    = DRBD_PROT_C;
	new_conf->ping_timeo	   = DRBD_PING_TIMEO_DEF;
	new_conf->rr_conflict	   = DRBD_RR_CONFLICT_DEF;
	new_conf->on_congestion    = DRBD_ON_CONGESTION_DEF;
	new_conf->cong_extents     = DRBD_CONG_EXTENTS_DEF;

	if (!net_conf_from_tags(mdev, nlp->tag_list, new_conf)) {
		retcode = ERR_MANDATORY_TAG;
		goto fail;
	}

	if (new_conf->two_primaries
	    && (new_conf->wire_protocol != DRBD_PROT_C)) {
		retcode = ERR_NOT_PROTO_C;
		goto fail;
	}

	if (get_ldev(mdev)) {
		enum drbd_fencing_p fp = mdev->ldev->dc.fencing;
		put_ldev(mdev);
		if (new_conf->wire_protocol == DRBD_PROT_A && fp == FP_STONITH) {
			retcode = ERR_STONITH_AND_PROT_A;
			goto fail;
		}
	}

	if (new_conf->on_congestion != OC_BLOCK && new_conf->wire_protocol != DRBD_PROT_A) {
		retcode = ERR_CONG_NOT_PROTO_A;
		goto fail;
	}

	if (mdev->state.role == R_PRIMARY && new_conf->want_lose) {
		retcode = ERR_DISCARD;
		goto fail;
	}

	retcode = NO_ERROR;

	new_my_addr = (struct sockaddr *)&new_conf->my_addr;
	new_peer_addr = (struct sockaddr *)&new_conf->peer_addr;
	for (i = 0; i < minor_count; i++) {
		odev = minor_to_mdev(i);
		if (!odev || odev == mdev)
			continue;
		if (get_net_conf(odev)) {
			taken_addr = (struct sockaddr *)&odev->net_conf->my_addr;
			if (new_conf->my_addr_len == odev->net_conf->my_addr_len &&
			    !memcmp(new_my_addr, taken_addr, new_conf->my_addr_len))
				retcode = ERR_LOCAL_ADDR;

			taken_addr = (struct sockaddr *)&odev->net_conf->peer_addr;
			if (new_conf->peer_addr_len == odev->net_conf->peer_addr_len &&
			    !memcmp(new_peer_addr, taken_addr, new_conf->peer_addr_len))
				retcode = ERR_PEER_ADDR;

			put_net_conf(odev);
			if (retcode != NO_ERROR)
				goto fail;
		}
	}

	if (new_conf->cram_hmac_alg[0] != 0) {
		snprintf(hmac_name, CRYPTO_MAX_ALG_NAME, "hmac(%s)",
			new_conf->cram_hmac_alg);
		tfm = crypto_alloc_hash(hmac_name, 0, CRYPTO_ALG_ASYNC);
		if (IS_ERR(tfm)) {
			tfm = NULL;
			retcode = ERR_AUTH_ALG;
			goto fail;
		}

		if (!drbd_crypto_is_hash(crypto_hash_tfm(tfm))) {
			retcode = ERR_AUTH_ALG_ND;
			goto fail;
		}
	}

	if (new_conf->integrity_alg[0]) {
		integrity_w_tfm = crypto_alloc_hash(new_conf->integrity_alg, 0, CRYPTO_ALG_ASYNC);
		if (IS_ERR(integrity_w_tfm)) {
			integrity_w_tfm = NULL;
			retcode=ERR_INTEGRITY_ALG;
			goto fail;
		}

		if (!drbd_crypto_is_hash(crypto_hash_tfm(integrity_w_tfm))) {
			retcode=ERR_INTEGRITY_ALG_ND;
			goto fail;
		}

		integrity_r_tfm = crypto_alloc_hash(new_conf->integrity_alg, 0, CRYPTO_ALG_ASYNC);
		if (IS_ERR(integrity_r_tfm)) {
			integrity_r_tfm = NULL;
			retcode=ERR_INTEGRITY_ALG;
			goto fail;
		}
	}

	ns = new_conf->max_epoch_size/8;
	if (mdev->tl_hash_s != ns) {
		new_tl_hash = kzalloc(ns*sizeof(void *), GFP_KERNEL);
		if (!new_tl_hash) {
			retcode = ERR_NOMEM;
			goto fail;
		}
	}

	ns = new_conf->max_buffers/8;
	if (new_conf->two_primaries && (mdev->ee_hash_s != ns)) {
		new_ee_hash = kzalloc(ns*sizeof(void *), GFP_KERNEL);
		if (!new_ee_hash) {
			retcode = ERR_NOMEM;
			goto fail;
		}
	}

	((char *)new_conf->shared_secret)[SHARED_SECRET_MAX-1] = 0;

	if (integrity_w_tfm) {
		i = crypto_hash_digestsize(integrity_w_tfm);
		int_dig_out = kmalloc(i, GFP_KERNEL);
		if (!int_dig_out) {
			retcode = ERR_NOMEM;
			goto fail;
		}
		int_dig_in = kmalloc(i, GFP_KERNEL);
		if (!int_dig_in) {
			retcode = ERR_NOMEM;
			goto fail;
		}
		int_dig_vv = kmalloc(i, GFP_KERNEL);
		if (!int_dig_vv) {
			retcode = ERR_NOMEM;
			goto fail;
		}
	}

	if (!mdev->bitmap) {
		if(drbd_bm_init(mdev)) {
			retcode = ERR_NOMEM;
			goto fail;
		}
	}

	drbd_flush_workqueue(mdev);
	spin_lock_irq(&mdev->req_lock);
	if (mdev->net_conf != NULL) {
		retcode = ERR_NET_CONFIGURED;
		spin_unlock_irq(&mdev->req_lock);
		goto fail;
	}
	mdev->net_conf = new_conf;

	mdev->send_cnt = 0;
	mdev->recv_cnt = 0;

	if (new_tl_hash) {
		kfree(mdev->tl_hash);
		mdev->tl_hash_s = mdev->net_conf->max_epoch_size/8;
		mdev->tl_hash = new_tl_hash;
	}

	if (new_ee_hash) {
		kfree(mdev->ee_hash);
		mdev->ee_hash_s = mdev->net_conf->max_buffers/8;
		mdev->ee_hash = new_ee_hash;
	}

	crypto_free_hash(mdev->cram_hmac_tfm);
	mdev->cram_hmac_tfm = tfm;

	crypto_free_hash(mdev->integrity_w_tfm);
	mdev->integrity_w_tfm = integrity_w_tfm;

	crypto_free_hash(mdev->integrity_r_tfm);
	mdev->integrity_r_tfm = integrity_r_tfm;

	kfree(mdev->int_dig_out);
	kfree(mdev->int_dig_in);
	kfree(mdev->int_dig_vv);
	mdev->int_dig_out=int_dig_out;
	mdev->int_dig_in=int_dig_in;
	mdev->int_dig_vv=int_dig_vv;
	retcode = _drbd_set_state(_NS(mdev, conn, C_UNCONNECTED), CS_VERBOSE, NULL);
	spin_unlock_irq(&mdev->req_lock);

	kobject_uevent(&disk_to_dev(mdev->vdisk)->kobj, KOBJ_CHANGE);
	reply->ret_code = retcode;
	drbd_reconfig_done(mdev);
	return 0;

fail:
	kfree(int_dig_out);
	kfree(int_dig_in);
	kfree(int_dig_vv);
	crypto_free_hash(tfm);
	crypto_free_hash(integrity_w_tfm);
	crypto_free_hash(integrity_r_tfm);
	kfree(new_tl_hash);
	kfree(new_ee_hash);
	kfree(new_conf);

	reply->ret_code = retcode;
	drbd_reconfig_done(mdev);
	return 0;
}

static int drbd_nl_disconnect(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			      struct drbd_nl_cfg_reply *reply)
{
	int retcode;
	struct disconnect dc;

	memset(&dc, 0, sizeof(struct disconnect));
	if (!disconnect_from_tags(mdev, nlp->tag_list, &dc)) {
		retcode = ERR_MANDATORY_TAG;
		goto fail;
	}

	if (dc.force) {
		spin_lock_irq(&mdev->req_lock);
		if (mdev->state.conn >= C_WF_CONNECTION)
			_drbd_set_state(_NS(mdev, conn, C_DISCONNECTING), CS_HARD, NULL);
		spin_unlock_irq(&mdev->req_lock);
		goto done;
	}

	retcode = _drbd_request_state(mdev, NS(conn, C_DISCONNECTING), CS_ORDERED);

	if (retcode == SS_NOTHING_TO_DO)
		goto done;
	else if (retcode == SS_ALREADY_STANDALONE)
		goto done;
	else if (retcode == SS_PRIMARY_NOP) {
		/* Our statche checking code wants to see the peer outdated. */
		retcode = drbd_request_state(mdev, NS2(conn, C_DISCONNECTING,
						      pdsk, D_OUTDATED));
	} else if (retcode == SS_CW_FAILED_BY_PEER) {
		/* The peer probably wants to see us outdated. */
		retcode = _drbd_request_state(mdev, NS2(conn, C_DISCONNECTING,
							disk, D_OUTDATED),
					      CS_ORDERED);
		if (retcode == SS_IS_DISKLESS || retcode == SS_LOWER_THAN_OUTDATED) {
			drbd_force_state(mdev, NS(conn, C_DISCONNECTING));
			retcode = SS_SUCCESS;
		}
	}

	if (retcode < SS_SUCCESS)
		goto fail;

	if (wait_event_interruptible(mdev->state_wait,
				     mdev->state.conn != C_DISCONNECTING)) {
		/* Do not test for mdev->state.conn == C_STANDALONE, since
		   someone else might connect us in the mean time! */
		retcode = ERR_INTR;
		goto fail;
	}

 done:
	retcode = NO_ERROR;
 fail:
	drbd_md_sync(mdev);
	reply->ret_code = retcode;
=======
out:
	return retcode;
}

/* Detaching the disk is a process in multiple stages.  First we need to lock
 * out application IO, in-flight IO, IO stuck in drbd_al_begin_io.
 * Then we transition to D_DISKLESS, and wait for put_ldev() to return all
 * internal references as well.
 * Only then we have finally detached. */
int drbd_adm_detach(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;
	struct detach_parms parms = { };
	int err;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (info->attrs[DRBD_NLA_DETACH_PARMS]) {
		err = detach_parms_from_attrs(&parms, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
			drbd_msg_put_info(from_attrs_err_to_txt(err));
			goto out;
		}
	}

	retcode = adm_detach(adm_ctx.mdev, parms.force_detach);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

static bool conn_resync_running(struct drbd_tconn *tconn)
{
	struct drbd_conf *mdev;
	bool rv = false;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		if (mdev->state.conn == C_SYNC_SOURCE ||
		    mdev->state.conn == C_SYNC_TARGET ||
		    mdev->state.conn == C_PAUSED_SYNC_S ||
		    mdev->state.conn == C_PAUSED_SYNC_T) {
			rv = true;
			break;
		}
	}
	rcu_read_unlock();

	return rv;
}

static bool conn_ov_running(struct drbd_tconn *tconn)
{
	struct drbd_conf *mdev;
	bool rv = false;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		if (mdev->state.conn == C_VERIFY_S ||
		    mdev->state.conn == C_VERIFY_T) {
			rv = true;
			break;
		}
	}
	rcu_read_unlock();

	return rv;
}

static enum drbd_ret_code
_check_net_options(struct drbd_tconn *tconn, struct net_conf *old_conf, struct net_conf *new_conf)
{
	struct drbd_conf *mdev;
	int i;

	if (old_conf && tconn->cstate == C_WF_REPORT_PARAMS && tconn->agreed_pro_version < 100) {
		if (new_conf->wire_protocol != old_conf->wire_protocol)
			return ERR_NEED_APV_100;

		if (new_conf->two_primaries != old_conf->two_primaries)
			return ERR_NEED_APV_100;

		if (strcmp(new_conf->integrity_alg, old_conf->integrity_alg))
			return ERR_NEED_APV_100;
	}

	if (!new_conf->two_primaries &&
	    conn_highest_role(tconn) == R_PRIMARY &&
	    conn_highest_peer(tconn) == R_PRIMARY)
		return ERR_NEED_ALLOW_TWO_PRI;

	if (new_conf->two_primaries &&
	    (new_conf->wire_protocol != DRBD_PROT_C))
		return ERR_NOT_PROTO_C;

	idr_for_each_entry(&tconn->volumes, mdev, i) {
		if (get_ldev(mdev)) {
			enum drbd_fencing_p fp = rcu_dereference(mdev->ldev->disk_conf)->fencing;
			put_ldev(mdev);
			if (new_conf->wire_protocol == DRBD_PROT_A && fp == FP_STONITH)
				return ERR_STONITH_AND_PROT_A;
		}
		if (mdev->state.role == R_PRIMARY && new_conf->discard_my_data)
			return ERR_DISCARD_IMPOSSIBLE;
	}

	if (new_conf->on_congestion != OC_BLOCK && new_conf->wire_protocol != DRBD_PROT_A)
		return ERR_CONG_NOT_PROTO_A;

	return NO_ERROR;
}

static enum drbd_ret_code
check_net_options(struct drbd_tconn *tconn, struct net_conf *new_conf)
{
	static enum drbd_ret_code rv;
	struct drbd_conf *mdev;
	int i;

	rcu_read_lock();
	rv = _check_net_options(tconn, rcu_dereference(tconn->net_conf), new_conf);
	rcu_read_unlock();

	/* tconn->volumes protected by genl_lock() here */
	idr_for_each_entry(&tconn->volumes, mdev, i) {
		if (!mdev->bitmap) {
			if(drbd_bm_init(mdev))
				return ERR_NOMEM;
		}
	}

	return rv;
}

struct crypto {
	struct crypto_hash *verify_tfm;
	struct crypto_hash *csums_tfm;
	struct crypto_hash *cram_hmac_tfm;
	struct crypto_hash *integrity_tfm;
};

static int
alloc_hash(struct crypto_hash **tfm, char *tfm_name, int err_alg)
{
	if (!tfm_name[0])
		return NO_ERROR;

	*tfm = crypto_alloc_hash(tfm_name, 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(*tfm)) {
		*tfm = NULL;
		return err_alg;
	}

	return NO_ERROR;
}

static enum drbd_ret_code
alloc_crypto(struct crypto *crypto, struct net_conf *new_conf)
{
	char hmac_name[CRYPTO_MAX_ALG_NAME];
	enum drbd_ret_code rv;

	rv = alloc_hash(&crypto->csums_tfm, new_conf->csums_alg,
		       ERR_CSUMS_ALG);
	if (rv != NO_ERROR)
		return rv;
	rv = alloc_hash(&crypto->verify_tfm, new_conf->verify_alg,
		       ERR_VERIFY_ALG);
	if (rv != NO_ERROR)
		return rv;
	rv = alloc_hash(&crypto->integrity_tfm, new_conf->integrity_alg,
		       ERR_INTEGRITY_ALG);
	if (rv != NO_ERROR)
		return rv;
	if (new_conf->cram_hmac_alg[0] != 0) {
		snprintf(hmac_name, CRYPTO_MAX_ALG_NAME, "hmac(%s)",
			 new_conf->cram_hmac_alg);

		rv = alloc_hash(&crypto->cram_hmac_tfm, hmac_name,
			       ERR_AUTH_ALG);
	}

	return rv;
}

static void free_crypto(struct crypto *crypto)
{
	crypto_free_hash(crypto->cram_hmac_tfm);
	crypto_free_hash(crypto->integrity_tfm);
	crypto_free_hash(crypto->csums_tfm);
	crypto_free_hash(crypto->verify_tfm);
}

int drbd_adm_net_opts(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;
	struct drbd_tconn *tconn;
	struct net_conf *old_conf, *new_conf = NULL;
	int err;
	int ovr; /* online verify running */
	int rsr; /* re-sync running */
	struct crypto crypto = { };

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_CONNECTION);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	tconn = adm_ctx.tconn;

	new_conf = kzalloc(sizeof(struct net_conf), GFP_KERNEL);
	if (!new_conf) {
		retcode = ERR_NOMEM;
		goto out;
	}

	conn_reconfig_start(tconn);

	mutex_lock(&tconn->data.mutex);
	mutex_lock(&tconn->conf_update);
	old_conf = tconn->net_conf;

	if (!old_conf) {
		drbd_msg_put_info("net conf missing, try connect");
		retcode = ERR_INVALID_REQUEST;
		goto fail;
	}

	*new_conf = *old_conf;
	if (should_set_defaults(info))
		set_net_conf_defaults(new_conf);

	err = net_conf_from_attrs_for_change(new_conf, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(from_attrs_err_to_txt(err));
		goto fail;
	}

	retcode = check_net_options(tconn, new_conf);
	if (retcode != NO_ERROR)
		goto fail;

	/* re-sync running */
	rsr = conn_resync_running(tconn);
	if (rsr && strcmp(new_conf->csums_alg, old_conf->csums_alg)) {
		retcode = ERR_CSUMS_RESYNC_RUNNING;
		goto fail;
	}

	/* online verify running */
	ovr = conn_ov_running(tconn);
	if (ovr && strcmp(new_conf->verify_alg, old_conf->verify_alg)) {
		retcode = ERR_VERIFY_RUNNING;
		goto fail;
	}

	retcode = alloc_crypto(&crypto, new_conf);
	if (retcode != NO_ERROR)
		goto fail;

	rcu_assign_pointer(tconn->net_conf, new_conf);

	if (!rsr) {
		crypto_free_hash(tconn->csums_tfm);
		tconn->csums_tfm = crypto.csums_tfm;
		crypto.csums_tfm = NULL;
	}
	if (!ovr) {
		crypto_free_hash(tconn->verify_tfm);
		tconn->verify_tfm = crypto.verify_tfm;
		crypto.verify_tfm = NULL;
	}

	crypto_free_hash(tconn->integrity_tfm);
	tconn->integrity_tfm = crypto.integrity_tfm;
	if (tconn->cstate >= C_WF_REPORT_PARAMS && tconn->agreed_pro_version >= 100)
		/* Do this without trying to take tconn->data.mutex again.  */
		__drbd_send_protocol(tconn, P_PROTOCOL_UPDATE);

	crypto_free_hash(tconn->cram_hmac_tfm);
	tconn->cram_hmac_tfm = crypto.cram_hmac_tfm;

	mutex_unlock(&tconn->conf_update);
	mutex_unlock(&tconn->data.mutex);
	synchronize_rcu();
	kfree(old_conf);

	if (tconn->cstate >= C_WF_REPORT_PARAMS)
		drbd_send_sync_param(minor_to_mdev(conn_lowest_minor(tconn)));

	goto done;

 fail:
	mutex_unlock(&tconn->conf_update);
	mutex_unlock(&tconn->data.mutex);
	free_crypto(&crypto);
	kfree(new_conf);
 done:
	conn_reconfig_done(tconn);
 out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_connect(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_conf *mdev;
	struct net_conf *old_conf, *new_conf = NULL;
	struct crypto crypto = { };
	struct drbd_tconn *tconn;
	enum drbd_ret_code retcode;
	int i;
	int err;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_RESOURCE);

	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;
	if (!(adm_ctx.my_addr && adm_ctx.peer_addr)) {
		drbd_msg_put_info("connection endpoint(s) missing");
		retcode = ERR_INVALID_REQUEST;
		goto out;
	}

	/* No need for _rcu here. All reconfiguration is
	 * strictly serialized on genl_lock(). We are protected against
	 * concurrent reconfiguration/addition/deletion */
	list_for_each_entry(tconn, &drbd_tconns, all_tconn) {
		if (nla_len(adm_ctx.my_addr) == tconn->my_addr_len &&
		    !memcmp(nla_data(adm_ctx.my_addr), &tconn->my_addr, tconn->my_addr_len)) {
			retcode = ERR_LOCAL_ADDR;
			goto out;
		}

		if (nla_len(adm_ctx.peer_addr) == tconn->peer_addr_len &&
		    !memcmp(nla_data(adm_ctx.peer_addr), &tconn->peer_addr, tconn->peer_addr_len)) {
			retcode = ERR_PEER_ADDR;
			goto out;
		}
	}

	tconn = adm_ctx.tconn;
	conn_reconfig_start(tconn);

	if (tconn->cstate > C_STANDALONE) {
		retcode = ERR_NET_CONFIGURED;
		goto fail;
	}

	/* allocation not in the IO path, drbdsetup / netlink process context */
	new_conf = kzalloc(sizeof(*new_conf), GFP_KERNEL);
	if (!new_conf) {
		retcode = ERR_NOMEM;
		goto fail;
	}

	set_net_conf_defaults(new_conf);

	err = net_conf_from_attrs(new_conf, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(from_attrs_err_to_txt(err));
		goto fail;
	}

	retcode = check_net_options(tconn, new_conf);
	if (retcode != NO_ERROR)
		goto fail;

	retcode = alloc_crypto(&crypto, new_conf);
	if (retcode != NO_ERROR)
		goto fail;

	((char *)new_conf->shared_secret)[SHARED_SECRET_MAX-1] = 0;

	conn_flush_workqueue(tconn);

	mutex_lock(&tconn->conf_update);
	old_conf = tconn->net_conf;
	if (old_conf) {
		retcode = ERR_NET_CONFIGURED;
		mutex_unlock(&tconn->conf_update);
		goto fail;
	}
	rcu_assign_pointer(tconn->net_conf, new_conf);

	conn_free_crypto(tconn);
	tconn->cram_hmac_tfm = crypto.cram_hmac_tfm;
	tconn->integrity_tfm = crypto.integrity_tfm;
	tconn->csums_tfm = crypto.csums_tfm;
	tconn->verify_tfm = crypto.verify_tfm;

	tconn->my_addr_len = nla_len(adm_ctx.my_addr);
	memcpy(&tconn->my_addr, nla_data(adm_ctx.my_addr), tconn->my_addr_len);
	tconn->peer_addr_len = nla_len(adm_ctx.peer_addr);
	memcpy(&tconn->peer_addr, nla_data(adm_ctx.peer_addr), tconn->peer_addr_len);

	mutex_unlock(&tconn->conf_update);

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, i) {
		mdev->send_cnt = 0;
		mdev->recv_cnt = 0;
	}
	rcu_read_unlock();

	retcode = conn_request_state(tconn, NS(conn, C_UNCONNECTED), CS_VERBOSE);

	conn_reconfig_done(tconn);
	drbd_adm_finish(info, retcode);
	return 0;

fail:
	free_crypto(&crypto);
	kfree(new_conf);

	conn_reconfig_done(tconn);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

static enum drbd_state_rv conn_try_disconnect(struct drbd_tconn *tconn, bool force)
{
	enum drbd_state_rv rv;

	rv = conn_request_state(tconn, NS(conn, C_DISCONNECTING),
			force ? CS_HARD : 0);

	switch (rv) {
	case SS_NOTHING_TO_DO:
		break;
	case SS_ALREADY_STANDALONE:
		return SS_SUCCESS;
	case SS_PRIMARY_NOP:
		/* Our state checking code wants to see the peer outdated. */
		rv = conn_request_state(tconn, NS2(conn, C_DISCONNECTING, pdsk, D_OUTDATED), 0);

		if (rv == SS_OUTDATE_WO_CONN) /* lost connection before graceful disconnect succeeded */
			rv = conn_request_state(tconn, NS(conn, C_DISCONNECTING), CS_VERBOSE);

		break;
	case SS_CW_FAILED_BY_PEER:
		/* The peer probably wants to see us outdated. */
		rv = conn_request_state(tconn, NS2(conn, C_DISCONNECTING,
							disk, D_OUTDATED), 0);
		if (rv == SS_IS_DISKLESS || rv == SS_LOWER_THAN_OUTDATED) {
			rv = conn_request_state(tconn, NS(conn, C_DISCONNECTING),
					CS_HARD);
		}
		break;
	default:;
		/* no special handling necessary */
	}

	if (rv >= SS_SUCCESS) {
		enum drbd_state_rv rv2;
		/* No one else can reconfigure the network while I am here.
		 * The state handling only uses drbd_thread_stop_nowait(),
		 * we want to really wait here until the receiver is no more.
		 */
		drbd_thread_stop(&adm_ctx.tconn->receiver);

		/* Race breaker.  This additional state change request may be
		 * necessary, if this was a forced disconnect during a receiver
		 * restart.  We may have "killed" the receiver thread just
		 * after drbdd_init() returned.  Typically, we should be
		 * C_STANDALONE already, now, and this becomes a no-op.
		 */
		rv2 = conn_request_state(tconn, NS(conn, C_STANDALONE),
				CS_VERBOSE | CS_HARD);
		if (rv2 < SS_SUCCESS)
			conn_err(tconn,
				"unexpected rv2=%d in conn_try_disconnect()\n",
				rv2);
	}
	return rv;
}

int drbd_adm_disconnect(struct sk_buff *skb, struct genl_info *info)
{
	struct disconnect_parms parms;
	struct drbd_tconn *tconn;
	enum drbd_state_rv rv;
	enum drbd_ret_code retcode;
	int err;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_CONNECTION);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto fail;

	tconn = adm_ctx.tconn;
	memset(&parms, 0, sizeof(parms));
	if (info->attrs[DRBD_NLA_DISCONNECT_PARMS]) {
		err = disconnect_parms_from_attrs(&parms, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
			drbd_msg_put_info(from_attrs_err_to_txt(err));
			goto fail;
		}
	}

	rv = conn_try_disconnect(tconn, parms.force_disconnect);
	if (rv < SS_SUCCESS)
		retcode = rv;  /* FIXME: Type mismatch. */
	else
		retcode = NO_ERROR;
 fail:
	drbd_adm_finish(info, retcode);
>>>>>>> refs/remotes/origin/master
	return 0;
}

void resync_after_online_grow(struct drbd_conf *mdev)
{
	int iass; /* I am sync source */

	dev_info(DEV, "Resync of new storage after online grow\n");
	if (mdev->state.role != mdev->state.peer)
		iass = (mdev->state.role == R_PRIMARY);
	else
<<<<<<< HEAD
		iass = test_bit(DISCARD_CONCURRENT, &mdev->flags);
=======
		iass = test_bit(RESOLVE_CONFLICTS, &mdev->tconn->flags);
>>>>>>> refs/remotes/origin/master

	if (iass)
		drbd_start_resync(mdev, C_SYNC_SOURCE);
	else
		_drbd_request_state(mdev, NS(conn, C_WF_SYNC_UUID), CS_VERBOSE + CS_SERIALIZE);
}

<<<<<<< HEAD
static int drbd_nl_resize(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			  struct drbd_nl_cfg_reply *reply)
{
	struct resize rs;
	int retcode = NO_ERROR;
	enum determine_dev_size dd;
	enum dds_flags ddsf;

	memset(&rs, 0, sizeof(struct resize));
	if (!resize_from_tags(mdev, nlp->tag_list, &rs)) {
		retcode = ERR_MANDATORY_TAG;
		goto fail;
=======
int drbd_adm_resize(struct sk_buff *skb, struct genl_info *info)
{
	struct disk_conf *old_disk_conf, *new_disk_conf = NULL;
	struct resize_parms rs;
	struct drbd_conf *mdev;
	enum drbd_ret_code retcode;
	enum determine_dev_size dd;
	bool change_al_layout = false;
	enum dds_flags ddsf;
	sector_t u_size;
	int err;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto fail;

	mdev = adm_ctx.mdev;
	if (!get_ldev(mdev)) {
		retcode = ERR_NO_DISK;
		goto fail;
	}

	memset(&rs, 0, sizeof(struct resize_parms));
	rs.al_stripes = mdev->ldev->md.al_stripes;
	rs.al_stripe_size = mdev->ldev->md.al_stripe_size_4k * 4;
	if (info->attrs[DRBD_NLA_RESIZE_PARMS]) {
		err = resize_parms_from_attrs(&rs, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
			drbd_msg_put_info(from_attrs_err_to_txt(err));
			goto fail_ldev;
		}
>>>>>>> refs/remotes/origin/master
	}

	if (mdev->state.conn > C_CONNECTED) {
		retcode = ERR_RESIZE_RESYNC;
<<<<<<< HEAD
		goto fail;
=======
		goto fail_ldev;
>>>>>>> refs/remotes/origin/master
	}

	if (mdev->state.role == R_SECONDARY &&
	    mdev->state.peer == R_SECONDARY) {
		retcode = ERR_NO_PRIMARY;
<<<<<<< HEAD
		goto fail;
	}

	if (!get_ldev(mdev)) {
		retcode = ERR_NO_DISK;
		goto fail;
	}

	if (rs.no_resync && mdev->agreed_pro_version < 93) {
		retcode = ERR_NEED_APV_93;
		goto fail;
=======
		goto fail_ldev;
	}

	if (rs.no_resync && mdev->tconn->agreed_pro_version < 93) {
		retcode = ERR_NEED_APV_93;
		goto fail_ldev;
	}

	rcu_read_lock();
	u_size = rcu_dereference(mdev->ldev->disk_conf)->disk_size;
	rcu_read_unlock();
	if (u_size != (sector_t)rs.resize_size) {
		new_disk_conf = kmalloc(sizeof(struct disk_conf), GFP_KERNEL);
		if (!new_disk_conf) {
			retcode = ERR_NOMEM;
			goto fail_ldev;
		}
	}

	if (mdev->ldev->md.al_stripes != rs.al_stripes ||
	    mdev->ldev->md.al_stripe_size_4k != rs.al_stripe_size / 4) {
		u32 al_size_k = rs.al_stripes * rs.al_stripe_size;

		if (al_size_k > (16 * 1024 * 1024)) {
			retcode = ERR_MD_LAYOUT_TOO_BIG;
			goto fail_ldev;
		}

		if (al_size_k < MD_32kB_SECT/2) {
			retcode = ERR_MD_LAYOUT_TOO_SMALL;
			goto fail_ldev;
		}

		if (mdev->state.conn != C_CONNECTED) {
			retcode = ERR_MD_LAYOUT_CONNECTED;
			goto fail_ldev;
		}

		change_al_layout = true;
>>>>>>> refs/remotes/origin/master
	}

	if (mdev->ldev->known_size != drbd_get_capacity(mdev->ldev->backing_bdev))
		mdev->ldev->known_size = drbd_get_capacity(mdev->ldev->backing_bdev);

<<<<<<< HEAD
	mdev->ldev->dc.disk_size = (sector_t)rs.resize_size;
	ddsf = (rs.resize_force ? DDSF_FORCED : 0) | (rs.no_resync ? DDSF_NO_RESYNC : 0);
	dd = drbd_determine_dev_size(mdev, ddsf);
	drbd_md_sync(mdev);
	put_ldev(mdev);
	if (dd == dev_size_error) {
		retcode = ERR_NOMEM_BITMAP;
		goto fail;
	}

	if (mdev->state.conn == C_CONNECTED) {
		if (dd == grew)
=======
	if (new_disk_conf) {
		mutex_lock(&mdev->tconn->conf_update);
		old_disk_conf = mdev->ldev->disk_conf;
		*new_disk_conf = *old_disk_conf;
		new_disk_conf->disk_size = (sector_t)rs.resize_size;
		rcu_assign_pointer(mdev->ldev->disk_conf, new_disk_conf);
		mutex_unlock(&mdev->tconn->conf_update);
		synchronize_rcu();
		kfree(old_disk_conf);
	}

	ddsf = (rs.resize_force ? DDSF_FORCED : 0) | (rs.no_resync ? DDSF_NO_RESYNC : 0);
	dd = drbd_determine_dev_size(mdev, ddsf, change_al_layout ? &rs : NULL);
	drbd_md_sync(mdev);
	put_ldev(mdev);
	if (dd == DS_ERROR) {
		retcode = ERR_NOMEM_BITMAP;
		goto fail;
	} else if (dd == DS_ERROR_SPACE_MD) {
		retcode = ERR_MD_LAYOUT_NO_FIT;
		goto fail;
	} else if (dd == DS_ERROR_SHRINK) {
		retcode = ERR_IMPLICIT_SHRINK;
		goto fail;
	}

	if (mdev->state.conn == C_CONNECTED) {
		if (dd == DS_GREW)
>>>>>>> refs/remotes/origin/master
			set_bit(RESIZE_PENDING, &mdev->flags);

		drbd_send_uuids(mdev);
		drbd_send_sizes(mdev, 1, ddsf);
	}

 fail:
<<<<<<< HEAD
	reply->ret_code = retcode;
	return 0;
}

static int drbd_nl_syncer_conf(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			       struct drbd_nl_cfg_reply *reply)
{
	int retcode = NO_ERROR;
	int err;
	int ovr; /* online verify running */
	int rsr; /* re-sync running */
	struct crypto_hash *verify_tfm = NULL;
	struct crypto_hash *csums_tfm = NULL;
	struct syncer_conf sc;
	cpumask_var_t new_cpu_mask;
	int *rs_plan_s = NULL;
	int fifo_size;

	if (!zalloc_cpumask_var(&new_cpu_mask, GFP_KERNEL)) {
		retcode = ERR_NOMEM;
		goto fail;
	}

	if (nlp->flags & DRBD_NL_SET_DEFAULTS) {
		memset(&sc, 0, sizeof(struct syncer_conf));
		sc.rate       = DRBD_RATE_DEF;
		sc.after      = DRBD_AFTER_DEF;
		sc.al_extents = DRBD_AL_EXTENTS_DEF;
		sc.on_no_data  = DRBD_ON_NO_DATA_DEF;
		sc.c_plan_ahead = DRBD_C_PLAN_AHEAD_DEF;
		sc.c_delay_target = DRBD_C_DELAY_TARGET_DEF;
		sc.c_fill_target = DRBD_C_FILL_TARGET_DEF;
		sc.c_max_rate = DRBD_C_MAX_RATE_DEF;
		sc.c_min_rate = DRBD_C_MIN_RATE_DEF;
	} else
		memcpy(&sc, &mdev->sync_conf, sizeof(struct syncer_conf));

	if (!syncer_conf_from_tags(mdev, nlp->tag_list, &sc)) {
		retcode = ERR_MANDATORY_TAG;
		goto fail;
	}

	/* re-sync running */
	rsr = (	mdev->state.conn == C_SYNC_SOURCE ||
		mdev->state.conn == C_SYNC_TARGET ||
		mdev->state.conn == C_PAUSED_SYNC_S ||
		mdev->state.conn == C_PAUSED_SYNC_T );

	if (rsr && strcmp(sc.csums_alg, mdev->sync_conf.csums_alg)) {
		retcode = ERR_CSUMS_RESYNC_RUNNING;
		goto fail;
	}

	if (!rsr && sc.csums_alg[0]) {
		csums_tfm = crypto_alloc_hash(sc.csums_alg, 0, CRYPTO_ALG_ASYNC);
		if (IS_ERR(csums_tfm)) {
			csums_tfm = NULL;
			retcode = ERR_CSUMS_ALG;
			goto fail;
		}

		if (!drbd_crypto_is_hash(crypto_hash_tfm(csums_tfm))) {
			retcode = ERR_CSUMS_ALG_ND;
			goto fail;
		}
	}

	/* online verify running */
	ovr = (mdev->state.conn == C_VERIFY_S || mdev->state.conn == C_VERIFY_T);

	if (ovr) {
		if (strcmp(sc.verify_alg, mdev->sync_conf.verify_alg)) {
			retcode = ERR_VERIFY_RUNNING;
			goto fail;
		}
	}

	if (!ovr && sc.verify_alg[0]) {
		verify_tfm = crypto_alloc_hash(sc.verify_alg, 0, CRYPTO_ALG_ASYNC);
		if (IS_ERR(verify_tfm)) {
			verify_tfm = NULL;
			retcode = ERR_VERIFY_ALG;
			goto fail;
		}

		if (!drbd_crypto_is_hash(crypto_hash_tfm(verify_tfm))) {
			retcode = ERR_VERIFY_ALG_ND;
			goto fail;
		}
	}

	/* silently ignore cpu mask on UP kernel */
	if (nr_cpu_ids > 1 && sc.cpu_mask[0] != 0) {
<<<<<<< HEAD
		err = __bitmap_parse(sc.cpu_mask, 32, 0,
				cpumask_bits(new_cpu_mask), nr_cpu_ids);
		if (err) {
			dev_warn(DEV, "__bitmap_parse() failed with %d\n", err);
=======
		err = bitmap_parse(sc.cpu_mask, 32,
				cpumask_bits(new_cpu_mask), nr_cpu_ids);
		if (err) {
			dev_warn(DEV, "bitmap_parse() failed with %d\n", err);
>>>>>>> refs/remotes/origin/cm-10.0
			retcode = ERR_CPU_MASK_PARSE;
			goto fail;
		}
	}

	ERR_IF (sc.rate < 1) sc.rate = 1;
	ERR_IF (sc.al_extents < 7) sc.al_extents = 127; /* arbitrary minimum */
#define AL_MAX ((MD_AL_MAX_SIZE-1) * AL_EXTENTS_PT)
	if (sc.al_extents > AL_MAX) {
		dev_err(DEV, "sc.al_extents > %d\n", AL_MAX);
		sc.al_extents = AL_MAX;
	}
#undef AL_MAX

	/* to avoid spurious errors when configuring minors before configuring
	 * the minors they depend on: if necessary, first create the minor we
	 * depend on */
	if (sc.after >= 0)
		ensure_mdev(sc.after, 1);

	/* most sanity checks done, try to assign the new sync-after
	 * dependency.  need to hold the global lock in there,
	 * to avoid a race in the dependency loop check. */
	retcode = drbd_alter_sa(mdev, sc.after);
	if (retcode != NO_ERROR)
		goto fail;

	fifo_size = (sc.c_plan_ahead * 10 * SLEEP_TIME) / HZ;
	if (fifo_size != mdev->rs_plan_s.size && fifo_size > 0) {
		rs_plan_s   = kzalloc(sizeof(int) * fifo_size, GFP_KERNEL);
		if (!rs_plan_s) {
			dev_err(DEV, "kmalloc of fifo_buffer failed");
			retcode = ERR_NOMEM;
			goto fail;
		}
	}

	/* ok, assign the rest of it as well.
	 * lock against receive_SyncParam() */
	spin_lock(&mdev->peer_seq_lock);
	mdev->sync_conf = sc;

	if (!rsr) {
		crypto_free_hash(mdev->csums_tfm);
		mdev->csums_tfm = csums_tfm;
		csums_tfm = NULL;
	}

	if (!ovr) {
		crypto_free_hash(mdev->verify_tfm);
		mdev->verify_tfm = verify_tfm;
		verify_tfm = NULL;
	}

	if (fifo_size != mdev->rs_plan_s.size) {
		kfree(mdev->rs_plan_s.values);
		mdev->rs_plan_s.values = rs_plan_s;
		mdev->rs_plan_s.size   = fifo_size;
		mdev->rs_planed = 0;
		rs_plan_s = NULL;
	}

	spin_unlock(&mdev->peer_seq_lock);

	if (get_ldev(mdev)) {
		wait_event(mdev->al_wait, lc_try_lock(mdev->act_log));
		drbd_al_shrink(mdev);
		err = drbd_check_al_size(mdev);
		lc_unlock(mdev->act_log);
		wake_up(&mdev->al_wait);

		put_ldev(mdev);
		drbd_md_sync(mdev);

		if (err) {
			retcode = ERR_NOMEM;
			goto fail;
		}
	}

	if (mdev->state.conn >= C_CONNECTED)
		drbd_send_sync_param(mdev, &sc);

	if (!cpumask_equal(mdev->cpu_mask, new_cpu_mask)) {
		cpumask_copy(mdev->cpu_mask, new_cpu_mask);
		drbd_calc_cpu_mask(mdev);
		mdev->receiver.reset_cpu_mask = 1;
		mdev->asender.reset_cpu_mask = 1;
		mdev->worker.reset_cpu_mask = 1;
	}

	kobject_uevent(&disk_to_dev(mdev->vdisk)->kobj, KOBJ_CHANGE);
fail:
	kfree(rs_plan_s);
	free_cpumask_var(new_cpu_mask);
	crypto_free_hash(csums_tfm);
	crypto_free_hash(verify_tfm);
	reply->ret_code = retcode;
	return 0;
}

static int drbd_nl_invalidate(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			      struct drbd_nl_cfg_reply *reply)
{
	int retcode;

	/* If there is still bitmap IO pending, probably because of a previous
	 * resync just being finished, wait for it before requesting a new resync. */
	wait_event(mdev->misc_wait, !test_bit(BITMAP_IO, &mdev->flags));

	retcode = _drbd_request_state(mdev, NS(conn, C_STARTING_SYNC_T), CS_ORDERED);

	if (retcode < SS_SUCCESS && retcode != SS_NEED_CONNECTION)
		retcode = drbd_request_state(mdev, NS(conn, C_STARTING_SYNC_T));

	while (retcode == SS_NEED_CONNECTION) {
		spin_lock_irq(&mdev->req_lock);
		if (mdev->state.conn < C_CONNECTED)
			retcode = _drbd_set_state(_NS(mdev, disk, D_INCONSISTENT), CS_VERBOSE, NULL);
		spin_unlock_irq(&mdev->req_lock);

		if (retcode != SS_NEED_CONNECTION)
			break;

		retcode = drbd_request_state(mdev, NS(conn, C_STARTING_SYNC_T));
	}

	reply->ret_code = retcode;
=======
	drbd_adm_finish(info, retcode);
	return 0;

 fail_ldev:
	put_ldev(mdev);
	goto fail;
}

int drbd_adm_resource_opts(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;
	struct drbd_tconn *tconn;
	struct res_opts res_opts;
	int err;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_RESOURCE);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto fail;
	tconn = adm_ctx.tconn;

	res_opts = tconn->res_opts;
	if (should_set_defaults(info))
		set_res_opts_defaults(&res_opts);

	err = res_opts_from_attrs(&res_opts, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(from_attrs_err_to_txt(err));
		goto fail;
	}

	err = set_resource_options(tconn, &res_opts);
	if (err) {
		retcode = ERR_INVALID_REQUEST;
		if (err == -ENOMEM)
			retcode = ERR_NOMEM;
	}

fail:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_invalidate(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_conf *mdev;
	int retcode; /* enum drbd_ret_code rsp. enum drbd_state_rv */

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	mdev = adm_ctx.mdev;

	/* If there is still bitmap IO pending, probably because of a previous
	 * resync just being finished, wait for it before requesting a new resync.
	 * Also wait for it's after_state_ch(). */
	drbd_suspend_io(mdev);
	wait_event(mdev->misc_wait, !test_bit(BITMAP_IO, &mdev->flags));
	drbd_flush_workqueue(mdev);

	/* If we happen to be C_STANDALONE R_SECONDARY, just change to
	 * D_INCONSISTENT, and set all bits in the bitmap.  Otherwise,
	 * try to start a resync handshake as sync target for full sync.
	 */
	if (mdev->state.conn == C_STANDALONE && mdev->state.role == R_SECONDARY) {
		retcode = drbd_request_state(mdev, NS(disk, D_INCONSISTENT));
		if (retcode >= SS_SUCCESS) {
			if (drbd_bitmap_io(mdev, &drbd_bmio_set_n_write,
				"set_n_write from invalidate", BM_LOCKED_MASK))
				retcode = ERR_IO_MD_DISK;
		}
	} else
		retcode = drbd_request_state(mdev, NS(conn, C_STARTING_SYNC_T));
	drbd_resume_io(mdev);

out:
	drbd_adm_finish(info, retcode);
	return 0;
}

static int drbd_adm_simple_request_state(struct sk_buff *skb, struct genl_info *info,
		union drbd_state mask, union drbd_state val)
{
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	retcode = drbd_request_state(adm_ctx.mdev, mask, val);
out:
	drbd_adm_finish(info, retcode);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int drbd_bmio_set_susp_al(struct drbd_conf *mdev)
{
	int rv;

	rv = drbd_bmio_set_n_write(mdev);
	drbd_suspend_al(mdev);
	return rv;
}

<<<<<<< HEAD
static int drbd_nl_invalidate_peer(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
				   struct drbd_nl_cfg_reply *reply)
{
	int retcode;

	/* If there is still bitmap IO pending, probably because of a previous
	 * resync just being finished, wait for it before requesting a new resync. */
	wait_event(mdev->misc_wait, !test_bit(BITMAP_IO, &mdev->flags));

	retcode = _drbd_request_state(mdev, NS(conn, C_STARTING_SYNC_S), CS_ORDERED);

	if (retcode < SS_SUCCESS) {
		if (retcode == SS_NEED_CONNECTION && mdev->state.role == R_PRIMARY) {
			/* The peer will get a resync upon connect anyways. Just make that
			   into a full resync. */
			retcode = drbd_request_state(mdev, NS(pdsk, D_INCONSISTENT));
			if (retcode >= SS_SUCCESS) {
				if (drbd_bitmap_io(mdev, &drbd_bmio_set_susp_al,
					"set_n_write from invalidate_peer",
					BM_LOCKED_SET_ALLOWED))
					retcode = ERR_IO_MD_DISK;
			}
		} else
			retcode = drbd_request_state(mdev, NS(conn, C_STARTING_SYNC_S));
	}

	reply->ret_code = retcode;
	return 0;
}

static int drbd_nl_pause_sync(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			      struct drbd_nl_cfg_reply *reply)
{
	int retcode = NO_ERROR;

	if (drbd_request_state(mdev, NS(user_isp, 1)) == SS_NOTHING_TO_DO)
		retcode = ERR_PAUSE_IS_SET;

	reply->ret_code = retcode;
	return 0;
}

static int drbd_nl_resume_sync(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			       struct drbd_nl_cfg_reply *reply)
{
	int retcode = NO_ERROR;
	union drbd_state s;

	if (drbd_request_state(mdev, NS(user_isp, 0)) == SS_NOTHING_TO_DO) {
		s = mdev->state;
=======
int drbd_adm_invalidate_peer(struct sk_buff *skb, struct genl_info *info)
{
	int retcode; /* drbd_ret_code, drbd_state_rv */
	struct drbd_conf *mdev;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	mdev = adm_ctx.mdev;

	/* If there is still bitmap IO pending, probably because of a previous
	 * resync just being finished, wait for it before requesting a new resync.
	 * Also wait for it's after_state_ch(). */
	drbd_suspend_io(mdev);
	wait_event(mdev->misc_wait, !test_bit(BITMAP_IO, &mdev->flags));
	drbd_flush_workqueue(mdev);

	/* If we happen to be C_STANDALONE R_PRIMARY, just set all bits
	 * in the bitmap.  Otherwise, try to start a resync handshake
	 * as sync source for full sync.
	 */
	if (mdev->state.conn == C_STANDALONE && mdev->state.role == R_PRIMARY) {
		/* The peer will get a resync upon connect anyways. Just make that
		   into a full resync. */
		retcode = drbd_request_state(mdev, NS(pdsk, D_INCONSISTENT));
		if (retcode >= SS_SUCCESS) {
			if (drbd_bitmap_io(mdev, &drbd_bmio_set_susp_al,
				"set_n_write from invalidate_peer",
				BM_LOCKED_SET_ALLOWED))
				retcode = ERR_IO_MD_DISK;
		}
	} else
		retcode = drbd_request_state(mdev, NS(conn, C_STARTING_SYNC_S));
	drbd_resume_io(mdev);

out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_pause_sync(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (drbd_request_state(adm_ctx.mdev, NS(user_isp, 1)) == SS_NOTHING_TO_DO)
		retcode = ERR_PAUSE_IS_SET;
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_resume_sync(struct sk_buff *skb, struct genl_info *info)
{
	union drbd_dev_state s;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (drbd_request_state(adm_ctx.mdev, NS(user_isp, 0)) == SS_NOTHING_TO_DO) {
		s = adm_ctx.mdev->state;
>>>>>>> refs/remotes/origin/master
		if (s.conn == C_PAUSED_SYNC_S || s.conn == C_PAUSED_SYNC_T) {
			retcode = s.aftr_isp ? ERR_PIC_AFTER_DEP :
				  s.peer_isp ? ERR_PIC_PEER_DEP : ERR_PAUSE_IS_CLEAR;
		} else {
			retcode = ERR_PAUSE_IS_CLEAR;
		}
	}

<<<<<<< HEAD
	reply->ret_code = retcode;
	return 0;
}

static int drbd_nl_suspend_io(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			      struct drbd_nl_cfg_reply *reply)
{
	reply->ret_code = drbd_request_state(mdev, NS(susp, 1));

	return 0;
}

static int drbd_nl_resume_io(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			     struct drbd_nl_cfg_reply *reply)
{
=======
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_suspend_io(struct sk_buff *skb, struct genl_info *info)
{
	return drbd_adm_simple_request_state(skb, info, NS(susp, 1));
}

int drbd_adm_resume_io(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_conf *mdev;
	int retcode; /* enum drbd_ret_code rsp. enum drbd_state_rv */

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	mdev = adm_ctx.mdev;
>>>>>>> refs/remotes/origin/master
	if (test_bit(NEW_CUR_UUID, &mdev->flags)) {
		drbd_uuid_new_current(mdev);
		clear_bit(NEW_CUR_UUID, &mdev->flags);
	}
	drbd_suspend_io(mdev);
<<<<<<< HEAD
	reply->ret_code = drbd_request_state(mdev, NS3(susp, 0, susp_nod, 0, susp_fen, 0));
	if (reply->ret_code == SS_SUCCESS) {
		if (mdev->state.conn < C_CONNECTED)
			tl_clear(mdev);
		if (mdev->state.disk == D_DISKLESS || mdev->state.disk == D_FAILED)
			tl_restart(mdev, fail_frozen_disk_io);
	}
	drbd_resume_io(mdev);

	return 0;
}

static int drbd_nl_outdate(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			   struct drbd_nl_cfg_reply *reply)
{
	reply->ret_code = drbd_request_state(mdev, NS(disk, D_OUTDATED));
	return 0;
}

static int drbd_nl_get_config(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			   struct drbd_nl_cfg_reply *reply)
{
	unsigned short *tl;

	tl = reply->tag_list;

	if (get_ldev(mdev)) {
		tl = disk_conf_to_tags(mdev, &mdev->ldev->dc, tl);
		put_ldev(mdev);
	}

	if (get_net_conf(mdev)) {
		tl = net_conf_to_tags(mdev, mdev->net_conf, tl);
		put_net_conf(mdev);
	}
	tl = syncer_conf_to_tags(mdev, &mdev->sync_conf, tl);

	put_unaligned(TT_END, tl++); /* Close the tag list */

	return (int)((char *)tl - (char *)reply->tag_list);
}

static int drbd_nl_get_state(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			     struct drbd_nl_cfg_reply *reply)
{
	unsigned short *tl = reply->tag_list;
	union drbd_state s = mdev->state;
	unsigned long rs_left;
	unsigned int res;

	tl = get_state_to_tags(mdev, (struct get_state *)&s, tl);

	/* no local ref, no bitmap, no syncer progress. */
	if (s.conn >= C_SYNC_SOURCE && s.conn <= C_PAUSED_SYNC_T) {
		if (get_ldev(mdev)) {
			drbd_get_syncer_progress(mdev, &rs_left, &res);
			tl = tl_add_int(tl, T_sync_progress, &res);
			put_ldev(mdev);
		}
	}
	put_unaligned(TT_END, tl++); /* Close the tag list */

	return (int)((char *)tl - (char *)reply->tag_list);
}

static int drbd_nl_get_uuids(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			     struct drbd_nl_cfg_reply *reply)
{
	unsigned short *tl;

	tl = reply->tag_list;

	if (get_ldev(mdev)) {
		tl = tl_add_blob(tl, T_uuids, mdev->ldev->md.uuid, UI_SIZE*sizeof(u64));
		tl = tl_add_int(tl, T_uuids_flags, &mdev->ldev->md.flags);
		put_ldev(mdev);
	}
	put_unaligned(TT_END, tl++); /* Close the tag list */

	return (int)((char *)tl - (char *)reply->tag_list);
}

/**
 * drbd_nl_get_timeout_flag() - Used by drbdsetup to find out which timeout value to use
 * @mdev:	DRBD device.
 * @nlp:	Netlink/connector packet from drbdsetup
 * @reply:	Reply packet for drbdsetup
 */
static int drbd_nl_get_timeout_flag(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
				    struct drbd_nl_cfg_reply *reply)
{
	unsigned short *tl;
	char rv;

	tl = reply->tag_list;

	rv = mdev->state.pdsk == D_OUTDATED        ? UT_PEER_OUTDATED :
	  test_bit(USE_DEGR_WFC_T, &mdev->flags) ? UT_DEGRADED : UT_DEFAULT;

	tl = tl_add_blob(tl, T_use_degraded, &rv, sizeof(rv));
	put_unaligned(TT_END, tl++); /* Close the tag list */

	return (int)((char *)tl - (char *)reply->tag_list);
}

static int drbd_nl_start_ov(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
				    struct drbd_nl_cfg_reply *reply)
{
	/* default to resume from last known position, if possible */
	struct start_ov args =
		{ .start_sector = mdev->ov_start_sector };

	if (!start_ov_from_tags(mdev, nlp->tag_list, &args)) {
		reply->ret_code = ERR_MANDATORY_TAG;
		return 0;
	}

	/* If there is still bitmap IO pending, e.g. previous resync or verify
	 * just being finished, wait for it before requesting a new resync. */
	wait_event(mdev->misc_wait, !test_bit(BITMAP_IO, &mdev->flags));

	/* w_make_ov_request expects position to be aligned */
	mdev->ov_start_sector = args.start_sector & ~BM_SECT_PER_BIT;
	reply->ret_code = drbd_request_state(mdev,NS(conn,C_VERIFY_S));
=======
	retcode = drbd_request_state(mdev, NS3(susp, 0, susp_nod, 0, susp_fen, 0));
	if (retcode == SS_SUCCESS) {
		if (mdev->state.conn < C_CONNECTED)
			tl_clear(mdev->tconn);
		if (mdev->state.disk == D_DISKLESS || mdev->state.disk == D_FAILED)
			tl_restart(mdev->tconn, FAIL_FROZEN_DISK_IO);
	}
	drbd_resume_io(mdev);

out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_outdate(struct sk_buff *skb, struct genl_info *info)
{
	return drbd_adm_simple_request_state(skb, info, NS(disk, D_OUTDATED));
}

int nla_put_drbd_cfg_context(struct sk_buff *skb, struct drbd_tconn *tconn, unsigned vnr)
{
	struct nlattr *nla;
	nla = nla_nest_start(skb, DRBD_NLA_CFG_CONTEXT);
	if (!nla)
		goto nla_put_failure;
	if (vnr != VOLUME_UNSPECIFIED &&
	    nla_put_u32(skb, T_ctx_volume, vnr))
		goto nla_put_failure;
	if (nla_put_string(skb, T_ctx_resource_name, tconn->name))
		goto nla_put_failure;
	if (tconn->my_addr_len &&
	    nla_put(skb, T_ctx_my_addr, tconn->my_addr_len, &tconn->my_addr))
		goto nla_put_failure;
	if (tconn->peer_addr_len &&
	    nla_put(skb, T_ctx_peer_addr, tconn->peer_addr_len, &tconn->peer_addr))
		goto nla_put_failure;
	nla_nest_end(skb, nla);
	return 0;

nla_put_failure:
	if (nla)
		nla_nest_cancel(skb, nla);
	return -EMSGSIZE;
}

int nla_put_status_info(struct sk_buff *skb, struct drbd_conf *mdev,
		const struct sib_info *sib)
{
	struct state_info *si = NULL; /* for sizeof(si->member); */
	struct nlattr *nla;
	int got_ldev;
	int err = 0;
	int exclude_sensitive;

	/* If sib != NULL, this is drbd_bcast_event, which anyone can listen
	 * to.  So we better exclude_sensitive information.
	 *
	 * If sib == NULL, this is drbd_adm_get_status, executed synchronously
	 * in the context of the requesting user process. Exclude sensitive
	 * information, unless current has superuser.
	 *
	 * NOTE: for drbd_adm_get_status_all(), this is a netlink dump, and
	 * relies on the current implementation of netlink_dump(), which
	 * executes the dump callback successively from netlink_recvmsg(),
	 * always in the context of the receiving process */
	exclude_sensitive = sib || !capable(CAP_SYS_ADMIN);

	got_ldev = get_ldev(mdev);

	/* We need to add connection name and volume number information still.
	 * Minor number is in drbd_genlmsghdr. */
	if (nla_put_drbd_cfg_context(skb, mdev->tconn, mdev->vnr))
		goto nla_put_failure;

	if (res_opts_to_skb(skb, &mdev->tconn->res_opts, exclude_sensitive))
		goto nla_put_failure;

	rcu_read_lock();
	if (got_ldev) {
		struct disk_conf *disk_conf;

		disk_conf = rcu_dereference(mdev->ldev->disk_conf);
		err = disk_conf_to_skb(skb, disk_conf, exclude_sensitive);
	}
	if (!err) {
		struct net_conf *nc;

		nc = rcu_dereference(mdev->tconn->net_conf);
		if (nc)
			err = net_conf_to_skb(skb, nc, exclude_sensitive);
	}
	rcu_read_unlock();
	if (err)
		goto nla_put_failure;

	nla = nla_nest_start(skb, DRBD_NLA_STATE_INFO);
	if (!nla)
		goto nla_put_failure;
	if (nla_put_u32(skb, T_sib_reason, sib ? sib->sib_reason : SIB_GET_STATUS_REPLY) ||
	    nla_put_u32(skb, T_current_state, mdev->state.i) ||
	    nla_put_u64(skb, T_ed_uuid, mdev->ed_uuid) ||
	    nla_put_u64(skb, T_capacity, drbd_get_capacity(mdev->this_bdev)) ||
	    nla_put_u64(skb, T_send_cnt, mdev->send_cnt) ||
	    nla_put_u64(skb, T_recv_cnt, mdev->recv_cnt) ||
	    nla_put_u64(skb, T_read_cnt, mdev->read_cnt) ||
	    nla_put_u64(skb, T_writ_cnt, mdev->writ_cnt) ||
	    nla_put_u64(skb, T_al_writ_cnt, mdev->al_writ_cnt) ||
	    nla_put_u64(skb, T_bm_writ_cnt, mdev->bm_writ_cnt) ||
	    nla_put_u32(skb, T_ap_bio_cnt, atomic_read(&mdev->ap_bio_cnt)) ||
	    nla_put_u32(skb, T_ap_pending_cnt, atomic_read(&mdev->ap_pending_cnt)) ||
	    nla_put_u32(skb, T_rs_pending_cnt, atomic_read(&mdev->rs_pending_cnt)))
		goto nla_put_failure;

	if (got_ldev) {
		int err;

		spin_lock_irq(&mdev->ldev->md.uuid_lock);
		err = nla_put(skb, T_uuids, sizeof(si->uuids), mdev->ldev->md.uuid);
		spin_unlock_irq(&mdev->ldev->md.uuid_lock);

		if (err)
			goto nla_put_failure;

		if (nla_put_u32(skb, T_disk_flags, mdev->ldev->md.flags) ||
		    nla_put_u64(skb, T_bits_total, drbd_bm_bits(mdev)) ||
		    nla_put_u64(skb, T_bits_oos, drbd_bm_total_weight(mdev)))
			goto nla_put_failure;
		if (C_SYNC_SOURCE <= mdev->state.conn &&
		    C_PAUSED_SYNC_T >= mdev->state.conn) {
			if (nla_put_u64(skb, T_bits_rs_total, mdev->rs_total) ||
			    nla_put_u64(skb, T_bits_rs_failed, mdev->rs_failed))
				goto nla_put_failure;
		}
	}

	if (sib) {
		switch(sib->sib_reason) {
		case SIB_SYNC_PROGRESS:
		case SIB_GET_STATUS_REPLY:
			break;
		case SIB_STATE_CHANGE:
			if (nla_put_u32(skb, T_prev_state, sib->os.i) ||
			    nla_put_u32(skb, T_new_state, sib->ns.i))
				goto nla_put_failure;
			break;
		case SIB_HELPER_POST:
			if (nla_put_u32(skb, T_helper_exit_code,
					sib->helper_exit_code))
				goto nla_put_failure;
			/* fall through */
		case SIB_HELPER_PRE:
			if (nla_put_string(skb, T_helper, sib->helper_name))
				goto nla_put_failure;
			break;
		}
	}
	nla_nest_end(skb, nla);

	if (0)
nla_put_failure:
		err = -EMSGSIZE;
	if (got_ldev)
		put_ldev(mdev);
	return err;
}

int drbd_adm_get_status(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;
	int err;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	err = nla_put_status_info(adm_ctx.reply_skb, adm_ctx.mdev, NULL);
	if (err) {
		nlmsg_free(adm_ctx.reply_skb);
		return err;
	}
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int get_one_status(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct drbd_conf *mdev;
	struct drbd_genlmsghdr *dh;
	struct drbd_tconn *pos = (struct drbd_tconn*)cb->args[0];
	struct drbd_tconn *tconn = NULL;
	struct drbd_tconn *tmp;
	unsigned volume = cb->args[1];

	/* Open coded, deferred, iteration:
	 * list_for_each_entry_safe(tconn, tmp, &drbd_tconns, all_tconn) {
	 *	idr_for_each_entry(&tconn->volumes, mdev, i) {
	 *	  ...
	 *	}
	 * }
	 * where tconn is cb->args[0];
	 * and i is cb->args[1];
	 *
	 * cb->args[2] indicates if we shall loop over all resources,
	 * or just dump all volumes of a single resource.
	 *
	 * This may miss entries inserted after this dump started,
	 * or entries deleted before they are reached.
	 *
	 * We need to make sure the mdev won't disappear while
	 * we are looking at it, and revalidate our iterators
	 * on each iteration.
	 */

	/* synchronize with conn_create()/conn_destroy() */
	rcu_read_lock();
	/* revalidate iterator position */
	list_for_each_entry_rcu(tmp, &drbd_tconns, all_tconn) {
		if (pos == NULL) {
			/* first iteration */
			pos = tmp;
			tconn = pos;
			break;
		}
		if (tmp == pos) {
			tconn = pos;
			break;
		}
	}
	if (tconn) {
next_tconn:
		mdev = idr_get_next(&tconn->volumes, &volume);
		if (!mdev) {
			/* No more volumes to dump on this tconn.
			 * Advance tconn iterator. */
			pos = list_entry_rcu(tconn->all_tconn.next,
					     struct drbd_tconn, all_tconn);
			/* Did we dump any volume on this tconn yet? */
			if (volume != 0) {
				/* If we reached the end of the list,
				 * or only a single resource dump was requested,
				 * we are done. */
				if (&pos->all_tconn == &drbd_tconns || cb->args[2])
					goto out;
				volume = 0;
				tconn = pos;
				goto next_tconn;
			}
		}

		dh = genlmsg_put(skb, NETLINK_CB(cb->skb).portid,
				cb->nlh->nlmsg_seq, &drbd_genl_family,
				NLM_F_MULTI, DRBD_ADM_GET_STATUS);
		if (!dh)
			goto out;

		if (!mdev) {
			/* This is a tconn without a single volume.
			 * Suprisingly enough, it may have a network
			 * configuration. */
			struct net_conf *nc;
			dh->minor = -1U;
			dh->ret_code = NO_ERROR;
			if (nla_put_drbd_cfg_context(skb, tconn, VOLUME_UNSPECIFIED))
				goto cancel;
			nc = rcu_dereference(tconn->net_conf);
			if (nc && net_conf_to_skb(skb, nc, 1) != 0)
				goto cancel;
			goto done;
		}

		D_ASSERT(mdev->vnr == volume);
		D_ASSERT(mdev->tconn == tconn);

		dh->minor = mdev_to_minor(mdev);
		dh->ret_code = NO_ERROR;

		if (nla_put_status_info(skb, mdev, NULL)) {
cancel:
			genlmsg_cancel(skb, dh);
			goto out;
		}
done:
		genlmsg_end(skb, dh);
        }

out:
	rcu_read_unlock();
	/* where to start the next iteration */
        cb->args[0] = (long)pos;
        cb->args[1] = (pos == tconn) ? volume + 1 : 0;

	/* No more tconns/volumes/minors found results in an empty skb.
	 * Which will terminate the dump. */
        return skb->len;
}

/*
 * Request status of all resources, or of all volumes within a single resource.
 *
 * This is a dump, as the answer may not fit in a single reply skb otherwise.
 * Which means we cannot use the family->attrbuf or other such members, because
 * dump is NOT protected by the genl_lock().  During dump, we only have access
 * to the incoming skb, and need to opencode "parsing" of the nlattr payload.
 *
 * Once things are setup properly, we call into get_one_status().
 */
int drbd_adm_get_status_all(struct sk_buff *skb, struct netlink_callback *cb)
{
	const unsigned hdrlen = GENL_HDRLEN + GENL_MAGIC_FAMILY_HDRSZ;
	struct nlattr *nla;
	const char *resource_name;
	struct drbd_tconn *tconn;
	int maxtype;

	/* Is this a followup call? */
	if (cb->args[0]) {
		/* ... of a single resource dump,
		 * and the resource iterator has been advanced already? */
		if (cb->args[2] && cb->args[2] != cb->args[0])
			return 0; /* DONE. */
		goto dump;
	}

	/* First call (from netlink_dump_start).  We need to figure out
	 * which resource(s) the user wants us to dump. */
	nla = nla_find(nlmsg_attrdata(cb->nlh, hdrlen),
			nlmsg_attrlen(cb->nlh, hdrlen),
			DRBD_NLA_CFG_CONTEXT);

	/* No explicit context given.  Dump all. */
	if (!nla)
		goto dump;
	maxtype = ARRAY_SIZE(drbd_cfg_context_nl_policy) - 1;
	nla = drbd_nla_find_nested(maxtype, nla, __nla_type(T_ctx_resource_name));
	if (IS_ERR(nla))
		return PTR_ERR(nla);
	/* context given, but no name present? */
	if (!nla)
		return -EINVAL;
	resource_name = nla_data(nla);
	tconn = conn_get_by_name(resource_name);

	if (!tconn)
		return -ENODEV;

	kref_put(&tconn->kref, &conn_destroy); /* get_one_status() (re)validates tconn by itself */

	/* prime iterators, and set "filter" mode mark:
	 * only dump this tconn. */
	cb->args[0] = (long)tconn;
	/* cb->args[1] = 0; passed in this way. */
	cb->args[2] = (long)tconn;

dump:
	return get_one_status(skb, cb);
}

int drbd_adm_get_timeout_type(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;
	struct timeout_parms tp;
	int err;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	tp.timeout_type =
		adm_ctx.mdev->state.pdsk == D_OUTDATED ? UT_PEER_OUTDATED :
		test_bit(USE_DEGR_WFC_T, &adm_ctx.mdev->flags) ? UT_DEGRADED :
		UT_DEFAULT;

	err = timeout_parms_to_priv_skb(adm_ctx.reply_skb, &tp);
	if (err) {
		nlmsg_free(adm_ctx.reply_skb);
		return err;
	}
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_start_ov(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_conf *mdev;
	enum drbd_ret_code retcode;
	struct start_ov_parms parms;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	mdev = adm_ctx.mdev;

	/* resume from last known position, if possible */
	parms.ov_start_sector = mdev->ov_start_sector;
	parms.ov_stop_sector = ULLONG_MAX;
	if (info->attrs[DRBD_NLA_START_OV_PARMS]) {
		int err = start_ov_parms_from_attrs(&parms, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
			drbd_msg_put_info(from_attrs_err_to_txt(err));
			goto out;
		}
	}
	/* w_make_ov_request expects position to be aligned */
	mdev->ov_start_sector = parms.ov_start_sector & ~(BM_SECT_PER_BIT-1);
	mdev->ov_stop_sector = parms.ov_stop_sector;

	/* If there is still bitmap IO pending, e.g. previous resync or verify
	 * just being finished, wait for it before requesting a new resync. */
	drbd_suspend_io(mdev);
	wait_event(mdev->misc_wait, !test_bit(BITMAP_IO, &mdev->flags));
	retcode = drbd_request_state(mdev,NS(conn,C_VERIFY_S));
	drbd_resume_io(mdev);
out:
	drbd_adm_finish(info, retcode);
>>>>>>> refs/remotes/origin/master
	return 0;
}


<<<<<<< HEAD
static int drbd_nl_new_c_uuid(struct drbd_conf *mdev, struct drbd_nl_cfg_req *nlp,
			      struct drbd_nl_cfg_reply *reply)
{
	int retcode = NO_ERROR;
	int skip_initial_sync = 0;
	int err;

	struct new_c_uuid args;

	memset(&args, 0, sizeof(struct new_c_uuid));
	if (!new_c_uuid_from_tags(mdev, nlp->tag_list, &args)) {
		reply->ret_code = ERR_MANDATORY_TAG;
		return 0;
	}

	mutex_lock(&mdev->state_mutex); /* Protects us against serialized state changes. */
=======
int drbd_adm_new_c_uuid(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_conf *mdev;
	enum drbd_ret_code retcode;
	int skip_initial_sync = 0;
	int err;
	struct new_c_uuid_parms args;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out_nolock;

	mdev = adm_ctx.mdev;
	memset(&args, 0, sizeof(args));
	if (info->attrs[DRBD_NLA_NEW_C_UUID_PARMS]) {
		err = new_c_uuid_parms_from_attrs(&args, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
			drbd_msg_put_info(from_attrs_err_to_txt(err));
			goto out_nolock;
		}
	}

	mutex_lock(mdev->state_mutex); /* Protects us against serialized state changes. */
>>>>>>> refs/remotes/origin/master

	if (!get_ldev(mdev)) {
		retcode = ERR_NO_DISK;
		goto out;
	}

	/* this is "skip initial sync", assume to be clean */
<<<<<<< HEAD
	if (mdev->state.conn == C_CONNECTED && mdev->agreed_pro_version >= 90 &&
=======
	if (mdev->state.conn == C_CONNECTED && mdev->tconn->agreed_pro_version >= 90 &&
>>>>>>> refs/remotes/origin/master
	    mdev->ldev->md.uuid[UI_CURRENT] == UUID_JUST_CREATED && args.clear_bm) {
		dev_info(DEV, "Preparing to skip initial sync\n");
		skip_initial_sync = 1;
	} else if (mdev->state.conn != C_STANDALONE) {
		retcode = ERR_CONNECTED;
		goto out_dec;
	}

	drbd_uuid_set(mdev, UI_BITMAP, 0); /* Rotate UI_BITMAP to History 1, etc... */
	drbd_uuid_new_current(mdev); /* New current, previous to UI_BITMAP */

	if (args.clear_bm) {
		err = drbd_bitmap_io(mdev, &drbd_bmio_clear_n_write,
			"clear_n_write from new_c_uuid", BM_LOCKED_MASK);
		if (err) {
			dev_err(DEV, "Writing bitmap failed with %d\n",err);
			retcode = ERR_IO_MD_DISK;
		}
		if (skip_initial_sync) {
			drbd_send_uuids_skip_initial_sync(mdev);
			_drbd_uuid_set(mdev, UI_BITMAP, 0);
			drbd_print_uuids(mdev, "cleared bitmap UUID");
<<<<<<< HEAD
			spin_lock_irq(&mdev->req_lock);
			_drbd_set_state(_NS2(mdev, disk, D_UP_TO_DATE, pdsk, D_UP_TO_DATE),
					CS_VERBOSE, NULL);
			spin_unlock_irq(&mdev->req_lock);
=======
			spin_lock_irq(&mdev->tconn->req_lock);
			_drbd_set_state(_NS2(mdev, disk, D_UP_TO_DATE, pdsk, D_UP_TO_DATE),
					CS_VERBOSE, NULL);
			spin_unlock_irq(&mdev->tconn->req_lock);
>>>>>>> refs/remotes/origin/master
		}
	}

	drbd_md_sync(mdev);
out_dec:
	put_ldev(mdev);
out:
<<<<<<< HEAD
	mutex_unlock(&mdev->state_mutex);

	reply->ret_code = retcode;
	return 0;
}

struct cn_handler_struct {
	int (*function)(struct drbd_conf *,
			 struct drbd_nl_cfg_req *,
			 struct drbd_nl_cfg_reply *);
	int reply_body_size;
};

static struct cn_handler_struct cnd_table[] = {
	[ P_primary ]		= { &drbd_nl_primary,		0 },
	[ P_secondary ]		= { &drbd_nl_secondary,		0 },
	[ P_disk_conf ]		= { &drbd_nl_disk_conf,		0 },
	[ P_detach ]		= { &drbd_nl_detach,		0 },
	[ P_net_conf ]		= { &drbd_nl_net_conf,		0 },
	[ P_disconnect ]	= { &drbd_nl_disconnect,	0 },
	[ P_resize ]		= { &drbd_nl_resize,		0 },
	[ P_syncer_conf ]	= { &drbd_nl_syncer_conf,	0 },
	[ P_invalidate ]	= { &drbd_nl_invalidate,	0 },
	[ P_invalidate_peer ]	= { &drbd_nl_invalidate_peer,	0 },
	[ P_pause_sync ]	= { &drbd_nl_pause_sync,	0 },
	[ P_resume_sync ]	= { &drbd_nl_resume_sync,	0 },
	[ P_suspend_io ]	= { &drbd_nl_suspend_io,	0 },
	[ P_resume_io ]		= { &drbd_nl_resume_io,		0 },
	[ P_outdate ]		= { &drbd_nl_outdate,		0 },
	[ P_get_config ]	= { &drbd_nl_get_config,
				    sizeof(struct syncer_conf_tag_len_struct) +
				    sizeof(struct disk_conf_tag_len_struct) +
				    sizeof(struct net_conf_tag_len_struct) },
	[ P_get_state ]		= { &drbd_nl_get_state,
				    sizeof(struct get_state_tag_len_struct) +
				    sizeof(struct sync_progress_tag_len_struct)	},
	[ P_get_uuids ]		= { &drbd_nl_get_uuids,
				    sizeof(struct get_uuids_tag_len_struct) },
	[ P_get_timeout_flag ]	= { &drbd_nl_get_timeout_flag,
				    sizeof(struct get_timeout_flag_tag_len_struct)},
	[ P_start_ov ]		= { &drbd_nl_start_ov,		0 },
	[ P_new_c_uuid ]	= { &drbd_nl_new_c_uuid,	0 },
};

static void drbd_connector_callback(struct cn_msg *req, struct netlink_skb_parms *nsp)
{
	struct drbd_nl_cfg_req *nlp = (struct drbd_nl_cfg_req *)req->data;
	struct cn_handler_struct *cm;
	struct cn_msg *cn_reply;
	struct drbd_nl_cfg_reply *reply;
	struct drbd_conf *mdev;
	int retcode, rr;
	int reply_size = sizeof(struct cn_msg)
		+ sizeof(struct drbd_nl_cfg_reply)
		+ sizeof(short int);

	if (!try_module_get(THIS_MODULE)) {
		printk(KERN_ERR "drbd: try_module_get() failed!\n");
		return;
	}

<<<<<<< HEAD
	if (!cap_raised(current_cap(), CAP_SYS_ADMIN)) {
=======
	if (!capable(CAP_SYS_ADMIN)) {
>>>>>>> refs/remotes/origin/cm-10.0
		retcode = ERR_PERM;
		goto fail;
	}

	mdev = ensure_mdev(nlp->drbd_minor,
			(nlp->flags & DRBD_NL_CREATE_DEVICE));
	if (!mdev) {
		retcode = ERR_MINOR_INVALID;
		goto fail;
	}

	if (nlp->packet_type >= P_nl_after_last_packet ||
	    nlp->packet_type == P_return_code_only) {
		retcode = ERR_PACKET_NR;
		goto fail;
	}

	cm = cnd_table + nlp->packet_type;

	/* This may happen if packet number is 0: */
	if (cm->function == NULL) {
		retcode = ERR_PACKET_NR;
		goto fail;
	}

	reply_size += cm->reply_body_size;

	/* allocation not in the IO path, cqueue thread context */
	cn_reply = kzalloc(reply_size, GFP_KERNEL);
	if (!cn_reply) {
		retcode = ERR_NOMEM;
		goto fail;
	}
	reply = (struct drbd_nl_cfg_reply *) cn_reply->data;

	reply->packet_type =
		cm->reply_body_size ? nlp->packet_type : P_return_code_only;
	reply->minor = nlp->drbd_minor;
	reply->ret_code = NO_ERROR; /* Might by modified by cm->function. */
	/* reply->tag_list; might be modified by cm->function. */

	rr = cm->function(mdev, nlp, reply);

	cn_reply->id = req->id;
	cn_reply->seq = req->seq;
	cn_reply->ack = req->ack  + 1;
	cn_reply->len = sizeof(struct drbd_nl_cfg_reply) + rr;
	cn_reply->flags = 0;

	rr = cn_netlink_send(cn_reply, CN_IDX_DRBD, GFP_KERNEL);
	if (rr && rr != -ESRCH)
		printk(KERN_INFO "drbd: cn_netlink_send()=%d\n", rr);

	kfree(cn_reply);
	module_put(THIS_MODULE);
	return;
 fail:
	drbd_nl_send_reply(req, retcode);
	module_put(THIS_MODULE);
}

static atomic_t drbd_nl_seq = ATOMIC_INIT(2); /* two. */

static unsigned short *
__tl_add_blob(unsigned short *tl, enum drbd_tags tag, const void *data,
	unsigned short len, int nul_terminated)
{
	unsigned short l = tag_descriptions[tag_number(tag)].max_len;
	len = (len < l) ? len :  l;
	put_unaligned(tag, tl++);
	put_unaligned(len, tl++);
	memcpy(tl, data, len);
	tl = (unsigned short*)((char*)tl + len);
	if (nul_terminated)
		*((char*)tl - 1) = 0;
	return tl;
}

static unsigned short *
tl_add_blob(unsigned short *tl, enum drbd_tags tag, const void *data, int len)
{
	return __tl_add_blob(tl, tag, data, len, 0);
}

static unsigned short *
tl_add_str(unsigned short *tl, enum drbd_tags tag, const char *str)
{
	return __tl_add_blob(tl, tag, str, strlen(str)+1, 0);
}

static unsigned short *
tl_add_int(unsigned short *tl, enum drbd_tags tag, const void *val)
{
	put_unaligned(tag, tl++);
	switch(tag_type(tag)) {
	case TT_INTEGER:
		put_unaligned(sizeof(int), tl++);
		put_unaligned(*(int *)val, (int *)tl);
		tl = (unsigned short*)((char*)tl+sizeof(int));
		break;
	case TT_INT64:
		put_unaligned(sizeof(u64), tl++);
		put_unaligned(*(u64 *)val, (u64 *)tl);
		tl = (unsigned short*)((char*)tl+sizeof(u64));
		break;
	default:
		/* someone did something stupid. */
		;
	}
	return tl;
}

void drbd_bcast_state(struct drbd_conf *mdev, union drbd_state state)
{
	char buffer[sizeof(struct cn_msg)+
		    sizeof(struct drbd_nl_cfg_reply)+
		    sizeof(struct get_state_tag_len_struct)+
		    sizeof(short int)];
	struct cn_msg *cn_reply = (struct cn_msg *) buffer;
	struct drbd_nl_cfg_reply *reply =
		(struct drbd_nl_cfg_reply *)cn_reply->data;
	unsigned short *tl = reply->tag_list;

	/* dev_warn(DEV, "drbd_bcast_state() got called\n"); */

	tl = get_state_to_tags(mdev, (struct get_state *)&state, tl);

	put_unaligned(TT_END, tl++); /* Close the tag list */

	cn_reply->id.idx = CN_IDX_DRBD;
	cn_reply->id.val = CN_VAL_DRBD;

	cn_reply->seq = atomic_add_return(1, &drbd_nl_seq);
	cn_reply->ack = 0; /* not used here. */
	cn_reply->len = sizeof(struct drbd_nl_cfg_reply) +
		(int)((char *)tl - (char *)reply->tag_list);
	cn_reply->flags = 0;

	reply->packet_type = P_get_state;
	reply->minor = mdev_to_minor(mdev);
	reply->ret_code = NO_ERROR;

	cn_netlink_send(cn_reply, CN_IDX_DRBD, GFP_NOIO);
}

void drbd_bcast_ev_helper(struct drbd_conf *mdev, char *helper_name)
{
	char buffer[sizeof(struct cn_msg)+
		    sizeof(struct drbd_nl_cfg_reply)+
		    sizeof(struct call_helper_tag_len_struct)+
		    sizeof(short int)];
	struct cn_msg *cn_reply = (struct cn_msg *) buffer;
	struct drbd_nl_cfg_reply *reply =
		(struct drbd_nl_cfg_reply *)cn_reply->data;
	unsigned short *tl = reply->tag_list;

	/* dev_warn(DEV, "drbd_bcast_state() got called\n"); */

	tl = tl_add_str(tl, T_helper, helper_name);
	put_unaligned(TT_END, tl++); /* Close the tag list */

	cn_reply->id.idx = CN_IDX_DRBD;
	cn_reply->id.val = CN_VAL_DRBD;

	cn_reply->seq = atomic_add_return(1, &drbd_nl_seq);
	cn_reply->ack = 0; /* not used here. */
	cn_reply->len = sizeof(struct drbd_nl_cfg_reply) +
		(int)((char *)tl - (char *)reply->tag_list);
	cn_reply->flags = 0;

	reply->packet_type = P_call_helper;
	reply->minor = mdev_to_minor(mdev);
	reply->ret_code = NO_ERROR;

	cn_netlink_send(cn_reply, CN_IDX_DRBD, GFP_NOIO);
}

void drbd_bcast_ee(struct drbd_conf *mdev,
		const char *reason, const int dgs,
		const char* seen_hash, const char* calc_hash,
		const struct drbd_epoch_entry* e)
{
	struct cn_msg *cn_reply;
	struct drbd_nl_cfg_reply *reply;
	unsigned short *tl;
	struct page *page;
	unsigned len;

	if (!e)
		return;
	if (!reason || !reason[0])
		return;

	/* apparently we have to memcpy twice, first to prepare the data for the
	 * struct cn_msg, then within cn_netlink_send from the cn_msg to the
	 * netlink skb. */
	/* receiver thread context, which is not in the writeout path (of this node),
	 * but may be in the writeout path of the _other_ node.
	 * GFP_NOIO to avoid potential "distributed deadlock". */
	cn_reply = kzalloc(
		sizeof(struct cn_msg)+
		sizeof(struct drbd_nl_cfg_reply)+
		sizeof(struct dump_ee_tag_len_struct)+
		sizeof(short int),
		GFP_NOIO);

	if (!cn_reply) {
		dev_err(DEV, "could not kmalloc buffer for drbd_bcast_ee, sector %llu, size %u\n",
				(unsigned long long)e->sector, e->size);
		return;
	}

	reply = (struct drbd_nl_cfg_reply*)cn_reply->data;
	tl = reply->tag_list;

	tl = tl_add_str(tl, T_dump_ee_reason, reason);
	tl = tl_add_blob(tl, T_seen_digest, seen_hash, dgs);
	tl = tl_add_blob(tl, T_calc_digest, calc_hash, dgs);
	tl = tl_add_int(tl, T_ee_sector, &e->sector);
	tl = tl_add_int(tl, T_ee_block_id, &e->block_id);

	/* dump the first 32k */
	len = min_t(unsigned, e->size, 32 << 10);
	put_unaligned(T_ee_data, tl++);
	put_unaligned(len, tl++);

	page = e->pages;
	page_chain_for_each(page) {
<<<<<<< HEAD
		void *d = kmap_atomic(page, KM_USER0);
		unsigned l = min_t(unsigned, len, PAGE_SIZE);
		memcpy(tl, d, l);
		kunmap_atomic(d, KM_USER0);
=======
		void *d = kmap_atomic(page);
		unsigned l = min_t(unsigned, len, PAGE_SIZE);
		memcpy(tl, d, l);
		kunmap_atomic(d);
>>>>>>> refs/remotes/origin/cm-10.0
		tl = (unsigned short*)((char*)tl + l);
		len -= l;
		if (len == 0)
			break;
	}
	put_unaligned(TT_END, tl++); /* Close the tag list */

	cn_reply->id.idx = CN_IDX_DRBD;
	cn_reply->id.val = CN_VAL_DRBD;

	cn_reply->seq = atomic_add_return(1,&drbd_nl_seq);
	cn_reply->ack = 0; // not used here.
	cn_reply->len = sizeof(struct drbd_nl_cfg_reply) +
		(int)((char*)tl - (char*)reply->tag_list);
	cn_reply->flags = 0;

	reply->packet_type = P_dump_ee;
	reply->minor = mdev_to_minor(mdev);
	reply->ret_code = NO_ERROR;

	cn_netlink_send(cn_reply, CN_IDX_DRBD, GFP_NOIO);
	kfree(cn_reply);
}

void drbd_bcast_sync_progress(struct drbd_conf *mdev)
{
	char buffer[sizeof(struct cn_msg)+
		    sizeof(struct drbd_nl_cfg_reply)+
		    sizeof(struct sync_progress_tag_len_struct)+
		    sizeof(short int)];
	struct cn_msg *cn_reply = (struct cn_msg *) buffer;
	struct drbd_nl_cfg_reply *reply =
		(struct drbd_nl_cfg_reply *)cn_reply->data;
	unsigned short *tl = reply->tag_list;
	unsigned long rs_left;
	unsigned int res;

	/* no local ref, no bitmap, no syncer progress, no broadcast. */
	if (!get_ldev(mdev))
		return;
	drbd_get_syncer_progress(mdev, &rs_left, &res);
	put_ldev(mdev);

	tl = tl_add_int(tl, T_sync_progress, &res);
	put_unaligned(TT_END, tl++); /* Close the tag list */

	cn_reply->id.idx = CN_IDX_DRBD;
	cn_reply->id.val = CN_VAL_DRBD;

	cn_reply->seq = atomic_add_return(1, &drbd_nl_seq);
	cn_reply->ack = 0; /* not used here. */
	cn_reply->len = sizeof(struct drbd_nl_cfg_reply) +
		(int)((char *)tl - (char *)reply->tag_list);
	cn_reply->flags = 0;

	reply->packet_type = P_sync_progress;
	reply->minor = mdev_to_minor(mdev);
	reply->ret_code = NO_ERROR;

	cn_netlink_send(cn_reply, CN_IDX_DRBD, GFP_NOIO);
}

int __init drbd_nl_init(void)
{
	static struct cb_id cn_id_drbd;
	int err, try=10;

	cn_id_drbd.val = CN_VAL_DRBD;
	do {
		cn_id_drbd.idx = cn_idx;
		err = cn_add_callback(&cn_id_drbd, "cn_drbd", &drbd_connector_callback);
		if (!err)
			break;
		cn_idx = (cn_idx + CN_IDX_STEP);
	} while (try--);

	if (err) {
		printk(KERN_ERR "drbd: cn_drbd failed to register\n");
		return err;
	}

	return 0;
}

void drbd_nl_cleanup(void)
{
	static struct cb_id cn_id_drbd;

	cn_id_drbd.idx = cn_idx;
	cn_id_drbd.val = CN_VAL_DRBD;

	cn_del_callback(&cn_id_drbd);
}

void drbd_nl_send_reply(struct cn_msg *req, int ret_code)
{
	char buffer[sizeof(struct cn_msg)+sizeof(struct drbd_nl_cfg_reply)];
	struct cn_msg *cn_reply = (struct cn_msg *) buffer;
	struct drbd_nl_cfg_reply *reply =
		(struct drbd_nl_cfg_reply *)cn_reply->data;
	int rr;

	memset(buffer, 0, sizeof(buffer));
	cn_reply->id = req->id;

	cn_reply->seq = req->seq;
	cn_reply->ack = req->ack  + 1;
	cn_reply->len = sizeof(struct drbd_nl_cfg_reply);
	cn_reply->flags = 0;

	reply->packet_type = P_return_code_only;
	reply->minor = ((struct drbd_nl_cfg_req *)req->data)->drbd_minor;
	reply->ret_code = ret_code;

	rr = cn_netlink_send(cn_reply, CN_IDX_DRBD, GFP_NOIO);
	if (rr && rr != -ESRCH)
		printk(KERN_INFO "drbd: cn_netlink_send()=%d\n", rr);
}

=======
	mutex_unlock(mdev->state_mutex);
out_nolock:
	drbd_adm_finish(info, retcode);
	return 0;
}

static enum drbd_ret_code
drbd_check_resource_name(const char *name)
{
	if (!name || !name[0]) {
		drbd_msg_put_info("resource name missing");
		return ERR_MANDATORY_TAG;
	}
	/* if we want to use these in sysfs/configfs/debugfs some day,
	 * we must not allow slashes */
	if (strchr(name, '/')) {
		drbd_msg_put_info("invalid resource name");
		return ERR_INVALID_REQUEST;
	}
	return NO_ERROR;
}

int drbd_adm_new_resource(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;
	struct res_opts res_opts;
	int err;

	retcode = drbd_adm_prepare(skb, info, 0);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	set_res_opts_defaults(&res_opts);
	err = res_opts_from_attrs(&res_opts, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(from_attrs_err_to_txt(err));
		goto out;
	}

	retcode = drbd_check_resource_name(adm_ctx.resource_name);
	if (retcode != NO_ERROR)
		goto out;

	if (adm_ctx.tconn) {
		if (info->nlhdr->nlmsg_flags & NLM_F_EXCL) {
			retcode = ERR_INVALID_REQUEST;
			drbd_msg_put_info("resource exists");
		}
		/* else: still NO_ERROR */
		goto out;
	}

	if (!conn_create(adm_ctx.resource_name, &res_opts))
		retcode = ERR_NOMEM;
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_add_minor(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_genlmsghdr *dh = info->userhdr;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_RESOURCE);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (dh->minor > MINORMASK) {
		drbd_msg_put_info("requested minor out of range");
		retcode = ERR_INVALID_REQUEST;
		goto out;
	}
	if (adm_ctx.volume > DRBD_VOLUME_MAX) {
		drbd_msg_put_info("requested volume id out of range");
		retcode = ERR_INVALID_REQUEST;
		goto out;
	}

	/* drbd_adm_prepare made sure already
	 * that mdev->tconn and mdev->vnr match the request. */
	if (adm_ctx.mdev) {
		if (info->nlhdr->nlmsg_flags & NLM_F_EXCL)
			retcode = ERR_MINOR_EXISTS;
		/* else: still NO_ERROR */
		goto out;
	}

	retcode = conn_new_minor(adm_ctx.tconn, dh->minor, adm_ctx.volume);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

static enum drbd_ret_code adm_delete_minor(struct drbd_conf *mdev)
{
	if (mdev->state.disk == D_DISKLESS &&
	    /* no need to be mdev->state.conn == C_STANDALONE &&
	     * we may want to delete a minor from a live replication group.
	     */
	    mdev->state.role == R_SECONDARY) {
		_drbd_request_state(mdev, NS(conn, C_WF_REPORT_PARAMS),
				    CS_VERBOSE + CS_WAIT_COMPLETE);
		idr_remove(&mdev->tconn->volumes, mdev->vnr);
		idr_remove(&minors, mdev_to_minor(mdev));
		destroy_workqueue(mdev->submit.wq);
		del_gendisk(mdev->vdisk);
		synchronize_rcu();
		kref_put(&mdev->kref, &drbd_minor_destroy);
		return NO_ERROR;
	} else
		return ERR_MINOR_CONFIGURED;
}

int drbd_adm_delete_minor(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	retcode = adm_delete_minor(adm_ctx.mdev);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_down(struct sk_buff *skb, struct genl_info *info)
{
	int retcode; /* enum drbd_ret_code rsp. enum drbd_state_rv */
	struct drbd_conf *mdev;
	unsigned i;

	retcode = drbd_adm_prepare(skb, info, 0);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (!adm_ctx.tconn) {
		retcode = ERR_RES_NOT_KNOWN;
		goto out;
	}

	/* demote */
	idr_for_each_entry(&adm_ctx.tconn->volumes, mdev, i) {
		retcode = drbd_set_role(mdev, R_SECONDARY, 0);
		if (retcode < SS_SUCCESS) {
			drbd_msg_put_info("failed to demote");
			goto out;
		}
	}

	retcode = conn_try_disconnect(adm_ctx.tconn, 0);
	if (retcode < SS_SUCCESS) {
		drbd_msg_put_info("failed to disconnect");
		goto out;
	}

	/* detach */
	idr_for_each_entry(&adm_ctx.tconn->volumes, mdev, i) {
		retcode = adm_detach(mdev, 0);
		if (retcode < SS_SUCCESS || retcode > NO_ERROR) {
			drbd_msg_put_info("failed to detach");
			goto out;
		}
	}

	/* If we reach this, all volumes (of this tconn) are Secondary,
	 * Disconnected, Diskless, aka Unconfigured. Make sure all threads have
	 * actually stopped, state handling only does drbd_thread_stop_nowait(). */
	drbd_thread_stop(&adm_ctx.tconn->worker);

	/* Now, nothing can fail anymore */

	/* delete volumes */
	idr_for_each_entry(&adm_ctx.tconn->volumes, mdev, i) {
		retcode = adm_delete_minor(mdev);
		if (retcode != NO_ERROR) {
			/* "can not happen" */
			drbd_msg_put_info("failed to delete volume");
			goto out;
		}
	}

	/* delete connection */
	if (conn_lowest_minor(adm_ctx.tconn) < 0) {
		list_del_rcu(&adm_ctx.tconn->all_tconn);
		synchronize_rcu();
		kref_put(&adm_ctx.tconn->kref, &conn_destroy);

		retcode = NO_ERROR;
	} else {
		/* "can not happen" */
		retcode = ERR_RES_IN_USE;
		drbd_msg_put_info("failed to delete connection");
	}
	goto out;
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_del_resource(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_RESOURCE);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (conn_lowest_minor(adm_ctx.tconn) < 0) {
		list_del_rcu(&adm_ctx.tconn->all_tconn);
		synchronize_rcu();
		kref_put(&adm_ctx.tconn->kref, &conn_destroy);

		retcode = NO_ERROR;
	} else {
		retcode = ERR_RES_IN_USE;
	}

	if (retcode == NO_ERROR)
		drbd_thread_stop(&adm_ctx.tconn->worker);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

void drbd_bcast_event(struct drbd_conf *mdev, const struct sib_info *sib)
{
	static atomic_t drbd_genl_seq = ATOMIC_INIT(2); /* two. */
	struct sk_buff *msg;
	struct drbd_genlmsghdr *d_out;
	unsigned seq;
	int err = -ENOMEM;

	if (sib->sib_reason == SIB_SYNC_PROGRESS) {
		if (time_after(jiffies, mdev->rs_last_bcast + HZ))
			mdev->rs_last_bcast = jiffies;
		else
			return;
	}

	seq = atomic_inc_return(&drbd_genl_seq);
	msg = genlmsg_new(NLMSG_GOODSIZE, GFP_NOIO);
	if (!msg)
		goto failed;

	err = -EMSGSIZE;
	d_out = genlmsg_put(msg, 0, seq, &drbd_genl_family, 0, DRBD_EVENT);
	if (!d_out) /* cannot happen, but anyways. */
		goto nla_put_failure;
	d_out->minor = mdev_to_minor(mdev);
	d_out->ret_code = NO_ERROR;

	if (nla_put_status_info(msg, mdev, sib))
		goto nla_put_failure;
	genlmsg_end(msg, d_out);
	err = drbd_genl_multicast_events(msg, 0);
	/* msg has been consumed or freed in netlink_broadcast() */
	if (err && err != -ESRCH)
		goto failed;

	return;

nla_put_failure:
	nlmsg_free(msg);
failed:
	dev_err(DEV, "Error %d while broadcasting event. "
			"Event seq:%u sib_reason:%u\n",
			err, seq, sib->sib_reason);
}
>>>>>>> refs/remotes/origin/master
