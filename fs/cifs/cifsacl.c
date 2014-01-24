/*
 *   fs/cifs/cifsacl.c
 *
 *   Copyright (C) International Business Machines  Corp., 2007,2008
 *   Author(s): Steve French (sfrench@us.ibm.com)
 *
 *   Contains the routines for mapping CIFS/NTFS ACLs
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/keyctl.h>
#include <linux/key-type.h>
#include <keys/user-type.h>
#include "cifspdu.h"
#include "cifsglob.h"
#include "cifsacl.h"
#include "cifsproto.h"
#include "cifs_debug.h"

/* security id for everyone/world system group */
static const struct cifs_sid sid_everyone = {
	1, 1, {0, 0, 0, 0, 0, 1}, {0} };
/* security id for Authenticated Users system group */
static const struct cifs_sid sid_authusers = {
	1, 1, {0, 0, 0, 0, 0, 5}, {__constant_cpu_to_le32(11)} };
/* group users */
static const struct cifs_sid sid_user = {1, 2 , {0, 0, 0, 0, 0, 5}, {} };

<<<<<<< HEAD
const struct cred *root_cred;

static void
shrink_idmap_tree(struct rb_root *root, int nr_to_scan, int *nr_rem,
			int *nr_del)
{
	struct rb_node *node;
	struct rb_node *tmp;
	struct cifs_sid_id *psidid;

	node = rb_first(root);
	while (node) {
		tmp = node;
		node = rb_next(tmp);
		psidid = rb_entry(tmp, struct cifs_sid_id, rbnode);
		if (nr_to_scan == 0 || *nr_del == nr_to_scan)
			++(*nr_rem);
		else {
			if (time_after(jiffies, psidid->time + SID_MAP_EXPIRE)
						&& psidid->refcount == 0) {
				rb_erase(tmp, root);
				++(*nr_del);
			} else
				++(*nr_rem);
		}
	}
}

/*
 * Run idmap cache shrinker.
 */
static int
cifs_idmap_shrinker(struct shrinker *shrink, struct shrink_control *sc)
{
	int nr_to_scan = sc->nr_to_scan;
	int nr_del = 0;
	int nr_rem = 0;
	struct rb_root *root;

	root = &uidtree;
	spin_lock(&siduidlock);
	shrink_idmap_tree(root, nr_to_scan, &nr_rem, &nr_del);
	spin_unlock(&siduidlock);

	root = &gidtree;
	spin_lock(&sidgidlock);
	shrink_idmap_tree(root, nr_to_scan, &nr_rem, &nr_del);
	spin_unlock(&sidgidlock);

<<<<<<< HEAD
	return nr_rem;
}

=======
	root = &siduidtree;
	spin_lock(&uidsidlock);
	shrink_idmap_tree(root, nr_to_scan, &nr_rem, &nr_del);
	spin_unlock(&uidsidlock);

	root = &sidgidtree;
	spin_lock(&gidsidlock);
	shrink_idmap_tree(root, nr_to_scan, &nr_rem, &nr_del);
	spin_unlock(&gidsidlock);

	return nr_rem;
}

static void
sid_rb_insert(struct rb_root *root, unsigned long cid,
		struct cifs_sid_id **psidid, char *typestr)
{
	char *strptr;
	struct rb_node *node = root->rb_node;
	struct rb_node *parent = NULL;
	struct rb_node **linkto = &(root->rb_node);
	struct cifs_sid_id *lsidid;

	while (node) {
		lsidid = rb_entry(node, struct cifs_sid_id, rbnode);
		parent = node;
		if (cid > lsidid->id) {
			linkto = &(node->rb_left);
			node = node->rb_left;
		}
		if (cid < lsidid->id) {
			linkto = &(node->rb_right);
			node = node->rb_right;
		}
	}

	(*psidid)->id = cid;
	(*psidid)->time = jiffies - (SID_MAP_RETRY + 1);
	(*psidid)->refcount = 0;

	sprintf((*psidid)->sidstr, "%s", typestr);
	strptr = (*psidid)->sidstr + strlen((*psidid)->sidstr);
	sprintf(strptr, "%ld", cid);

	clear_bit(SID_ID_PENDING, &(*psidid)->state);
	clear_bit(SID_ID_MAPPED, &(*psidid)->state);

	rb_link_node(&(*psidid)->rbnode, parent, linkto);
	rb_insert_color(&(*psidid)->rbnode, root);
}

static struct cifs_sid_id *
sid_rb_search(struct rb_root *root, unsigned long cid)
{
	struct rb_node *node = root->rb_node;
	struct cifs_sid_id *lsidid;

	while (node) {
		lsidid = rb_entry(node, struct cifs_sid_id, rbnode);
		if (cid > lsidid->id)
			node = node->rb_left;
		else if (cid < lsidid->id)
			node = node->rb_right;
		else /* node found */
			return lsidid;
	}

	return NULL;
}

>>>>>>> refs/remotes/origin/cm-10.0
static struct shrinker cifs_shrinker = {
	.shrink = cifs_idmap_shrinker,
	.seeks = DEFAULT_SEEKS,
};

static int
cifs_idmap_key_instantiate(struct key *key, const void *data, size_t datalen)
{
	char *payload;

	payload = kmalloc(datalen, GFP_KERNEL);
	if (!payload)
		return -ENOMEM;

	memcpy(payload, data, datalen);
	key->payload.data = payload;
<<<<<<< HEAD
=======
	key->datalen = datalen;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct cred *root_cred;

static int
cifs_idmap_key_instantiate(struct key *key, struct key_preparsed_payload *prep)
{
	char *payload;

	/*
	 * If the payload is less than or equal to the size of a pointer, then
	 * an allocation here is wasteful. Just copy the data directly to the
	 * payload.value union member instead.
	 *
	 * With this however, you must check the datalen before trying to
	 * dereference payload.data!
	 */
	if (prep->datalen <= sizeof(key->payload)) {
		key->payload.value = 0;
		memcpy(&key->payload.value, prep->data, prep->datalen);
		key->datalen = prep->datalen;
		return 0;
	}
	payload = kmemdup(prep->data, prep->datalen, GFP_KERNEL);
	if (!payload)
		return -ENOMEM;

	key->payload.data = payload;
	key->datalen = prep->datalen;
>>>>>>> refs/remotes/origin/master
	return 0;
}

static inline void
cifs_idmap_key_destroy(struct key *key)
{
<<<<<<< HEAD
	kfree(key->payload.data);
}

struct key_type cifs_idmap_key_type = {
=======
	if (key->datalen > sizeof(key->payload))
		kfree(key->payload.data);
}

static struct key_type cifs_idmap_key_type = {
>>>>>>> refs/remotes/origin/master
	.name        = "cifs.idmap",
	.instantiate = cifs_idmap_key_instantiate,
	.destroy     = cifs_idmap_key_destroy,
	.describe    = user_describe,
	.match       = user_match,
};

<<<<<<< HEAD
static void
sid_to_str(struct cifs_sid *sidptr, char *sidstr)
{
	int i;
	unsigned long saval;
	char *strptr;

	strptr = sidstr;

	sprintf(strptr, "%s", "S");
	strptr = sidstr + strlen(sidstr);

	sprintf(strptr, "-%d", sidptr->revision);
	strptr = sidstr + strlen(sidstr);

	for (i = 0; i < 6; ++i) {
		if (sidptr->authority[i]) {
			sprintf(strptr, "-%d", sidptr->authority[i]);
			strptr = sidstr + strlen(sidstr);
		}
	}

	for (i = 0; i < sidptr->num_subauth; ++i) {
		saval = le32_to_cpu(sidptr->sub_auth[i]);
		sprintf(strptr, "-%ld", saval);
		strptr = sidstr + strlen(sidstr);
	}
}

static void
<<<<<<< HEAD
=======
cifs_copy_sid(struct cifs_sid *dst, const struct cifs_sid *src)
{
	memcpy(dst, src, sizeof(*dst));
	dst->num_subauth = min_t(u8, src->num_subauth, NUM_SUBAUTHS);
}

static void
>>>>>>> refs/remotes/origin/cm-10.0
id_rb_insert(struct rb_root *root, struct cifs_sid *sidptr,
		struct cifs_sid_id **psidid, char *typestr)
{
	int rc;
	char *strptr;
	struct rb_node *node = root->rb_node;
	struct rb_node *parent = NULL;
	struct rb_node **linkto = &(root->rb_node);
	struct cifs_sid_id *lsidid;

	while (node) {
		lsidid = rb_entry(node, struct cifs_sid_id, rbnode);
		parent = node;
		rc = compare_sids(sidptr, &((lsidid)->sid));
		if (rc > 0) {
			linkto = &(node->rb_left);
			node = node->rb_left;
		} else if (rc < 0) {
			linkto = &(node->rb_right);
			node = node->rb_right;
		}
	}

<<<<<<< HEAD
	memcpy(&(*psidid)->sid, sidptr, sizeof(struct cifs_sid));
=======
	cifs_copy_sid(&(*psidid)->sid, sidptr);
>>>>>>> refs/remotes/origin/cm-10.0
	(*psidid)->time = jiffies - (SID_MAP_RETRY + 1);
	(*psidid)->refcount = 0;

	sprintf((*psidid)->sidstr, "%s", typestr);
	strptr = (*psidid)->sidstr + strlen((*psidid)->sidstr);
	sid_to_str(&(*psidid)->sid, strptr);

	clear_bit(SID_ID_PENDING, &(*psidid)->state);
	clear_bit(SID_ID_MAPPED, &(*psidid)->state);

	rb_link_node(&(*psidid)->rbnode, parent, linkto);
	rb_insert_color(&(*psidid)->rbnode, root);
}

static struct cifs_sid_id *
id_rb_search(struct rb_root *root, struct cifs_sid *sidptr)
{
	int rc;
	struct rb_node *node = root->rb_node;
	struct cifs_sid_id *lsidid;

	while (node) {
		lsidid = rb_entry(node, struct cifs_sid_id, rbnode);
		rc = compare_sids(sidptr, &((lsidid)->sid));
		if (rc > 0) {
			node = node->rb_left;
		} else if (rc < 0) {
			node = node->rb_right;
		} else /* node found */
			return lsidid;
	}

	return NULL;
}

static int
sidid_pending_wait(void *unused)
{
	schedule();
	return signal_pending(current) ? -ERESTARTSYS : 0;
}

static int
<<<<<<< HEAD
=======
id_to_sid(unsigned long cid, uint sidtype, struct cifs_sid *ssid)
{
	int rc = 0;
	struct key *sidkey;
	const struct cred *saved_cred;
	struct cifs_sid *lsid;
	struct cifs_sid_id *psidid, *npsidid;
	struct rb_root *cidtree;
	spinlock_t *cidlock;

	if (sidtype == SIDOWNER) {
		cidlock = &siduidlock;
		cidtree = &uidtree;
	} else if (sidtype == SIDGROUP) {
		cidlock = &sidgidlock;
		cidtree = &gidtree;
	} else
		return -EINVAL;

	spin_lock(cidlock);
	psidid = sid_rb_search(cidtree, cid);

	if (!psidid) { /* node does not exist, allocate one & attempt adding */
		spin_unlock(cidlock);
		npsidid = kzalloc(sizeof(struct cifs_sid_id), GFP_KERNEL);
		if (!npsidid)
			return -ENOMEM;

		npsidid->sidstr = kmalloc(SIDLEN, GFP_KERNEL);
		if (!npsidid->sidstr) {
			kfree(npsidid);
			return -ENOMEM;
		}

		spin_lock(cidlock);
		psidid = sid_rb_search(cidtree, cid);
		if (psidid) { /* node happened to get inserted meanwhile */
			++psidid->refcount;
			spin_unlock(cidlock);
			kfree(npsidid->sidstr);
			kfree(npsidid);
		} else {
			psidid = npsidid;
			sid_rb_insert(cidtree, cid, &psidid,
					sidtype == SIDOWNER ? "oi:" : "gi:");
			++psidid->refcount;
			spin_unlock(cidlock);
		}
	} else {
		++psidid->refcount;
		spin_unlock(cidlock);
	}

	/*
	 * If we are here, it is safe to access psidid and its fields
	 * since a reference was taken earlier while holding the spinlock.
	 * A reference on the node is put without holding the spinlock
	 * and it is OK to do so in this case, shrinker will not erase
	 * this node until all references are put and we do not access
	 * any fields of the node after a reference is put .
	 */
	if (test_bit(SID_ID_MAPPED, &psidid->state)) {
		cifs_copy_sid(ssid, &psidid->sid);
		psidid->time = jiffies; /* update ts for accessing */
		goto id_sid_out;
	}

	if (time_after(psidid->time + SID_MAP_RETRY, jiffies)) {
		rc = -EINVAL;
		goto id_sid_out;
	}

	if (!test_and_set_bit(SID_ID_PENDING, &psidid->state)) {
		saved_cred = override_creds(root_cred);
		sidkey = request_key(&cifs_idmap_key_type, psidid->sidstr, "");
		if (IS_ERR(sidkey)) {
			rc = -EINVAL;
			cFYI(1, "%s: Can't map and id to a SID", __func__);
		} else if (sidkey->datalen < sizeof(struct cifs_sid)) {
			rc = -EIO;
			cFYI(1, "%s: Downcall contained malformed key "
				"(datalen=%hu)", __func__, sidkey->datalen);
		} else {
			lsid = (struct cifs_sid *)sidkey->payload.data;
			cifs_copy_sid(&psidid->sid, lsid);
			cifs_copy_sid(ssid, &psidid->sid);
			set_bit(SID_ID_MAPPED, &psidid->state);
			key_put(sidkey);
			kfree(psidid->sidstr);
		}
		psidid->time = jiffies; /* update ts for accessing */
		revert_creds(saved_cred);
		clear_bit(SID_ID_PENDING, &psidid->state);
		wake_up_bit(&psidid->state, SID_ID_PENDING);
	} else {
		rc = wait_on_bit(&psidid->state, SID_ID_PENDING,
				sidid_pending_wait, TASK_INTERRUPTIBLE);
		if (rc) {
			cFYI(1, "%s: sidid_pending_wait interrupted %d",
					__func__, rc);
			--psidid->refcount;
			return rc;
		}
		if (test_bit(SID_ID_MAPPED, &psidid->state))
			cifs_copy_sid(ssid, &psidid->sid);
		else
			rc = -EINVAL;
	}
id_sid_out:
	--psidid->refcount;
	return rc;
}

static int
>>>>>>> refs/remotes/origin/cm-10.0
=======
static char *
sid_to_key_str(struct cifs_sid *sidptr, unsigned int type)
{
	int i, len;
	unsigned int saval;
	char *sidstr, *strptr;
	unsigned long long id_auth_val;

	/* 3 bytes for prefix */
	sidstr = kmalloc(3 + SID_STRING_BASE_SIZE +
			 (SID_STRING_SUBAUTH_SIZE * sidptr->num_subauth),
			 GFP_KERNEL);
	if (!sidstr)
		return sidstr;

	strptr = sidstr;
	len = sprintf(strptr, "%cs:S-%hhu", type == SIDOWNER ? 'o' : 'g',
			sidptr->revision);
	strptr += len;

	/* The authority field is a single 48-bit number */
	id_auth_val = (unsigned long long)sidptr->authority[5];
	id_auth_val |= (unsigned long long)sidptr->authority[4] << 8;
	id_auth_val |= (unsigned long long)sidptr->authority[3] << 16;
	id_auth_val |= (unsigned long long)sidptr->authority[2] << 24;
	id_auth_val |= (unsigned long long)sidptr->authority[1] << 32;
	id_auth_val |= (unsigned long long)sidptr->authority[0] << 48;

	/*
	 * MS-DTYP states that if the authority is >= 2^32, then it should be
	 * expressed as a hex value.
	 */
	if (id_auth_val <= UINT_MAX)
		len = sprintf(strptr, "-%llu", id_auth_val);
	else
		len = sprintf(strptr, "-0x%llx", id_auth_val);

	strptr += len;

	for (i = 0; i < sidptr->num_subauth; ++i) {
		saval = le32_to_cpu(sidptr->sub_auth[i]);
		len = sprintf(strptr, "-%u", saval);
		strptr += len;
	}

	return sidstr;
}

/*
 * if the two SIDs (roughly equivalent to a UUID for a user or group) are
 * the same returns zero, if they do not match returns non-zero.
 */
static int
compare_sids(const struct cifs_sid *ctsid, const struct cifs_sid *cwsid)
{
	int i;
	int num_subauth, num_sat, num_saw;

	if ((!ctsid) || (!cwsid))
		return 1;

	/* compare the revision */
	if (ctsid->revision != cwsid->revision) {
		if (ctsid->revision > cwsid->revision)
			return 1;
		else
			return -1;
	}

	/* compare all of the six auth values */
	for (i = 0; i < NUM_AUTHS; ++i) {
		if (ctsid->authority[i] != cwsid->authority[i]) {
			if (ctsid->authority[i] > cwsid->authority[i])
				return 1;
			else
				return -1;
		}
	}

	/* compare all of the subauth values if any */
	num_sat = ctsid->num_subauth;
	num_saw = cwsid->num_subauth;
	num_subauth = num_sat < num_saw ? num_sat : num_saw;
	if (num_subauth) {
		for (i = 0; i < num_subauth; ++i) {
			if (ctsid->sub_auth[i] != cwsid->sub_auth[i]) {
				if (le32_to_cpu(ctsid->sub_auth[i]) >
					le32_to_cpu(cwsid->sub_auth[i]))
					return 1;
				else
					return -1;
			}
		}
	}

	return 0; /* sids compare/match */
}

static void
cifs_copy_sid(struct cifs_sid *dst, const struct cifs_sid *src)
{
	int i;

	dst->revision = src->revision;
	dst->num_subauth = min_t(u8, src->num_subauth, SID_MAX_SUB_AUTHORITIES);
	for (i = 0; i < NUM_AUTHS; ++i)
		dst->authority[i] = src->authority[i];
	for (i = 0; i < dst->num_subauth; ++i)
		dst->sub_auth[i] = src->sub_auth[i];
}

static int
id_to_sid(unsigned int cid, uint sidtype, struct cifs_sid *ssid)
{
	int rc;
	struct key *sidkey;
	struct cifs_sid *ksid;
	unsigned int ksid_size;
	char desc[3 + 10 + 1]; /* 3 byte prefix + 10 bytes for value + NULL */
	const struct cred *saved_cred;

	rc = snprintf(desc, sizeof(desc), "%ci:%u",
			sidtype == SIDOWNER ? 'o' : 'g', cid);
	if (rc >= sizeof(desc))
		return -EINVAL;

	rc = 0;
	saved_cred = override_creds(root_cred);
	sidkey = request_key(&cifs_idmap_key_type, desc, "");
	if (IS_ERR(sidkey)) {
		rc = -EINVAL;
		cifs_dbg(FYI, "%s: Can't map %cid %u to a SID\n",
			 __func__, sidtype == SIDOWNER ? 'u' : 'g', cid);
		goto out_revert_creds;
	} else if (sidkey->datalen < CIFS_SID_BASE_SIZE) {
		rc = -EIO;
		cifs_dbg(FYI, "%s: Downcall contained malformed key (datalen=%hu)\n",
			 __func__, sidkey->datalen);
		goto invalidate_key;
	}

	/*
	 * A sid is usually too large to be embedded in payload.value, but if
	 * there are no subauthorities and the host has 8-byte pointers, then
	 * it could be.
	 */
	ksid = sidkey->datalen <= sizeof(sidkey->payload) ?
		(struct cifs_sid *)&sidkey->payload.value :
		(struct cifs_sid *)sidkey->payload.data;

	ksid_size = CIFS_SID_BASE_SIZE + (ksid->num_subauth * sizeof(__le32));
	if (ksid_size > sidkey->datalen) {
		rc = -EIO;
		cifs_dbg(FYI, "%s: Downcall contained malformed key (datalen=%hu, ksid_size=%u)\n",
			 __func__, sidkey->datalen, ksid_size);
		goto invalidate_key;
	}

	cifs_copy_sid(ssid, ksid);
out_key_put:
	key_put(sidkey);
out_revert_creds:
	revert_creds(saved_cred);
	return rc;

invalidate_key:
	key_invalidate(sidkey);
	goto out_key_put;
}

static int
>>>>>>> refs/remotes/origin/master
sid_to_id(struct cifs_sb_info *cifs_sb, struct cifs_sid *psid,
		struct cifs_fattr *fattr, uint sidtype)
{
	int rc;
<<<<<<< HEAD
	unsigned long cid;
	struct key *idkey;
	const struct cred *saved_cred;
	struct cifs_sid_id *psidid, *npsidid;
	struct rb_root *cidtree;
	spinlock_t *cidlock;

	if (sidtype == SIDOWNER) {
		cid = cifs_sb->mnt_uid; /* default uid, in case upcall fails */
		cidlock = &siduidlock;
		cidtree = &uidtree;
	} else if (sidtype == SIDGROUP) {
		cid = cifs_sb->mnt_gid; /* default gid, in case upcall fails */
		cidlock = &sidgidlock;
		cidtree = &gidtree;
	} else
		return -ENOENT;

	spin_lock(cidlock);
	psidid = id_rb_search(cidtree, psid);

	if (!psidid) { /* node does not exist, allocate one & attempt adding */
		spin_unlock(cidlock);
		npsidid = kzalloc(sizeof(struct cifs_sid_id), GFP_KERNEL);
		if (!npsidid)
			return -ENOMEM;

		npsidid->sidstr = kmalloc(SIDLEN, GFP_KERNEL);
		if (!npsidid->sidstr) {
			kfree(npsidid);
			return -ENOMEM;
		}

		spin_lock(cidlock);
		psidid = id_rb_search(cidtree, psid);
		if (psidid) { /* node happened to get inserted meanwhile */
			++psidid->refcount;
			spin_unlock(cidlock);
			kfree(npsidid->sidstr);
			kfree(npsidid);
		} else {
			psidid = npsidid;
			id_rb_insert(cidtree, psid, &psidid,
					sidtype == SIDOWNER ? "os:" : "gs:");
			++psidid->refcount;
			spin_unlock(cidlock);
		}
	} else {
		++psidid->refcount;
		spin_unlock(cidlock);
	}

	/*
	 * If we are here, it is safe to access psidid and its fields
	 * since a reference was taken earlier while holding the spinlock.
	 * A reference on the node is put without holding the spinlock
	 * and it is OK to do so in this case, shrinker will not erase
	 * this node until all references are put and we do not access
	 * any fields of the node after a reference is put .
	 */
	if (test_bit(SID_ID_MAPPED, &psidid->state)) {
		cid = psidid->id;
		psidid->time = jiffies; /* update ts for accessing */
		goto sid_to_id_out;
	}

	if (time_after(psidid->time + SID_MAP_RETRY, jiffies))
		goto sid_to_id_out;

	if (!test_and_set_bit(SID_ID_PENDING, &psidid->state)) {
		saved_cred = override_creds(root_cred);
		idkey = request_key(&cifs_idmap_key_type, psidid->sidstr, "");
		if (IS_ERR(idkey))
			cFYI(1, "%s: Can't map SID to an id", __func__);
		else {
			cid = *(unsigned long *)idkey->payload.value;
			psidid->id = cid;
			set_bit(SID_ID_MAPPED, &psidid->state);
			key_put(idkey);
			kfree(psidid->sidstr);
		}
		revert_creds(saved_cred);
		psidid->time = jiffies; /* update ts for accessing */
		clear_bit(SID_ID_PENDING, &psidid->state);
		wake_up_bit(&psidid->state, SID_ID_PENDING);
	} else {
		rc = wait_on_bit(&psidid->state, SID_ID_PENDING,
				sidid_pending_wait, TASK_INTERRUPTIBLE);
		if (rc) {
			cFYI(1, "%s: sidid_pending_wait interrupted %d",
					__func__, rc);
			--psidid->refcount; /* decremented without spinlock */
			return rc;
		}
		if (test_bit(SID_ID_MAPPED, &psidid->state))
			cid = psidid->id;
	}

sid_to_id_out:
	--psidid->refcount; /* decremented without spinlock */
	if (sidtype == SIDOWNER)
		fattr->cf_uid = cid;
	else
		fattr->cf_gid = cid;

=======
	struct key *sidkey;
	char *sidstr;
	const struct cred *saved_cred;
	kuid_t fuid = cifs_sb->mnt_uid;
	kgid_t fgid = cifs_sb->mnt_gid;

	/*
	 * If we have too many subauthorities, then something is really wrong.
	 * Just return an error.
	 */
	if (unlikely(psid->num_subauth > SID_MAX_SUB_AUTHORITIES)) {
		cifs_dbg(FYI, "%s: %u subauthorities is too many!\n",
			 __func__, psid->num_subauth);
		return -EIO;
	}

	sidstr = sid_to_key_str(psid, sidtype);
	if (!sidstr)
		return -ENOMEM;

	saved_cred = override_creds(root_cred);
	sidkey = request_key(&cifs_idmap_key_type, sidstr, "");
	if (IS_ERR(sidkey)) {
		rc = -EINVAL;
		cifs_dbg(FYI, "%s: Can't map SID %s to a %cid\n",
			 __func__, sidstr, sidtype == SIDOWNER ? 'u' : 'g');
		goto out_revert_creds;
	}

	/*
	 * FIXME: Here we assume that uid_t and gid_t are same size. It's
	 * probably a safe assumption but might be better to check based on
	 * sidtype.
	 */
	BUILD_BUG_ON(sizeof(uid_t) != sizeof(gid_t));
	if (sidkey->datalen != sizeof(uid_t)) {
		rc = -EIO;
		cifs_dbg(FYI, "%s: Downcall contained malformed key (datalen=%hu)\n",
			 __func__, sidkey->datalen);
		key_invalidate(sidkey);
		goto out_key_put;
	}

	if (sidtype == SIDOWNER) {
		kuid_t uid;
		uid_t id;
		memcpy(&id, &sidkey->payload.value, sizeof(uid_t));
		uid = make_kuid(&init_user_ns, id);
		if (uid_valid(uid))
			fuid = uid;
	} else {
		kgid_t gid;
		gid_t id;
		memcpy(&id, &sidkey->payload.value, sizeof(gid_t));
		gid = make_kgid(&init_user_ns, id);
		if (gid_valid(gid))
			fgid = gid;
	}

out_key_put:
	key_put(sidkey);
out_revert_creds:
	revert_creds(saved_cred);
	kfree(sidstr);

	/*
	 * Note that we return 0 here unconditionally. If the mapping
	 * fails then we just fall back to using the mnt_uid/mnt_gid.
	 */
	if (sidtype == SIDOWNER)
		fattr->cf_uid = fuid;
	else
		fattr->cf_gid = fgid;
>>>>>>> refs/remotes/origin/master
	return 0;
}

int
init_cifs_idmap(void)
{
	struct cred *cred;
	struct key *keyring;
	int ret;

<<<<<<< HEAD
	cFYI(1, "Registering the %s key type\n", cifs_idmap_key_type.name);
=======
	cifs_dbg(FYI, "Registering the %s key type\n",
		 cifs_idmap_key_type.name);
>>>>>>> refs/remotes/origin/master

	/* create an override credential set with a special thread keyring in
	 * which requests are cached
	 *
	 * this is used to prevent malicious redirections from being installed
	 * with add_key().
	 */
	cred = prepare_kernel_cred(NULL);
	if (!cred)
		return -ENOMEM;

<<<<<<< HEAD
	keyring = key_alloc(&key_type_keyring, ".cifs_idmap", 0, 0, cred,
			    (KEY_POS_ALL & ~KEY_POS_SETATTR) |
			    KEY_USR_VIEW | KEY_USR_READ,
			    KEY_ALLOC_NOT_IN_QUOTA);
=======
	keyring = keyring_alloc(".cifs_idmap",
				GLOBAL_ROOT_UID, GLOBAL_ROOT_GID, cred,
				(KEY_POS_ALL & ~KEY_POS_SETATTR) |
				KEY_USR_VIEW | KEY_USR_READ,
				KEY_ALLOC_NOT_IN_QUOTA, NULL);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(keyring)) {
		ret = PTR_ERR(keyring);
		goto failed_put_cred;
	}

<<<<<<< HEAD
	ret = key_instantiate_and_link(keyring, NULL, 0, NULL, NULL);
	if (ret < 0)
		goto failed_put_key;

=======
>>>>>>> refs/remotes/origin/master
	ret = register_key_type(&cifs_idmap_key_type);
	if (ret < 0)
		goto failed_put_key;

	/* instruct request_key() to use this special keyring as a cache for
	 * the results it looks up */
<<<<<<< HEAD
<<<<<<< HEAD
=======
	set_bit(KEY_FLAG_ROOT_CAN_CLEAR, &keyring->flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	set_bit(KEY_FLAG_ROOT_CAN_CLEAR, &keyring->flags);
>>>>>>> refs/remotes/origin/master
	cred->thread_keyring = keyring;
	cred->jit_keyring = KEY_REQKEY_DEFL_THREAD_KEYRING;
	root_cred = cred;

<<<<<<< HEAD
	spin_lock_init(&siduidlock);
	uidtree = RB_ROOT;
	spin_lock_init(&sidgidlock);
	gidtree = RB_ROOT;

<<<<<<< HEAD
=======
	spin_lock_init(&uidsidlock);
	siduidtree = RB_ROOT;
	spin_lock_init(&gidsidlock);
	sidgidtree = RB_ROOT;
>>>>>>> refs/remotes/origin/cm-10.0
	register_shrinker(&cifs_shrinker);

	cFYI(1, "cifs idmap keyring: %d\n", key_serial(keyring));
=======
	cifs_dbg(FYI, "cifs idmap keyring: %d\n", key_serial(keyring));
>>>>>>> refs/remotes/origin/master
	return 0;

failed_put_key:
	key_put(keyring);
failed_put_cred:
	put_cred(cred);
	return ret;
}

void
exit_cifs_idmap(void)
{
	key_revoke(root_cred->thread_keyring);
	unregister_key_type(&cifs_idmap_key_type);
	put_cred(root_cred);
<<<<<<< HEAD
	unregister_shrinker(&cifs_shrinker);
	cFYI(1, "Unregistered %s key type\n", cifs_idmap_key_type.name);
}

void
cifs_destroy_idmaptrees(void)
{
	struct rb_root *root;
	struct rb_node *node;

	root = &uidtree;
	spin_lock(&siduidlock);
	while ((node = rb_first(root)))
		rb_erase(node, root);
	spin_unlock(&siduidlock);

	root = &gidtree;
	spin_lock(&sidgidlock);
	while ((node = rb_first(root)))
		rb_erase(node, root);
	spin_unlock(&sidgidlock);
<<<<<<< HEAD
=======

	root = &siduidtree;
	spin_lock(&uidsidlock);
	while ((node = rb_first(root)))
		rb_erase(node, root);
	spin_unlock(&uidsidlock);

	root = &sidgidtree;
	spin_lock(&gidsidlock);
	while ((node = rb_first(root)))
		rb_erase(node, root);
	spin_unlock(&gidsidlock);
>>>>>>> refs/remotes/origin/cm-10.0
}

/* if the two SIDs (roughly equivalent to a UUID for a user or group) are
   the same returns 1, if they do not match returns 0 */
int compare_sids(const struct cifs_sid *ctsid, const struct cifs_sid *cwsid)
{
	int i;
	int num_subauth, num_sat, num_saw;

	if ((!ctsid) || (!cwsid))
		return 1;

	/* compare the revision */
	if (ctsid->revision != cwsid->revision) {
		if (ctsid->revision > cwsid->revision)
			return 1;
		else
			return -1;
	}

	/* compare all of the six auth values */
	for (i = 0; i < 6; ++i) {
		if (ctsid->authority[i] != cwsid->authority[i]) {
			if (ctsid->authority[i] > cwsid->authority[i])
				return 1;
			else
				return -1;
		}
	}

	/* compare all of the subauth values if any */
	num_sat = ctsid->num_subauth;
	num_saw = cwsid->num_subauth;
	num_subauth = num_sat < num_saw ? num_sat : num_saw;
	if (num_subauth) {
		for (i = 0; i < num_subauth; ++i) {
			if (ctsid->sub_auth[i] != cwsid->sub_auth[i]) {
				if (le32_to_cpu(ctsid->sub_auth[i]) >
					le32_to_cpu(cwsid->sub_auth[i]))
					return 1;
				else
					return -1;
			}
		}
	}

	return 0; /* sids compare/match */
}


=======
	cifs_dbg(FYI, "Unregistered %s key type\n", cifs_idmap_key_type.name);
}

>>>>>>> refs/remotes/origin/master
/* copy ntsd, owner sid, and group sid from a security descriptor to another */
static void copy_sec_desc(const struct cifs_ntsd *pntsd,
				struct cifs_ntsd *pnntsd, __u32 sidsoffset)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct cifs_sid *owner_sid_ptr, *group_sid_ptr;
	struct cifs_sid *nowner_sid_ptr, *ngroup_sid_ptr;

	/* copy security descriptor control portion */
	pnntsd->revision = pntsd->revision;
	pnntsd->type = pntsd->type;
	pnntsd->dacloffset = cpu_to_le32(sizeof(struct cifs_ntsd));
	pnntsd->sacloffset = 0;
	pnntsd->osidoffset = cpu_to_le32(sidsoffset);
	pnntsd->gsidoffset = cpu_to_le32(sidsoffset + sizeof(struct cifs_sid));

	/* copy owner sid */
	owner_sid_ptr = (struct cifs_sid *)((char *)pntsd +
				le32_to_cpu(pntsd->osidoffset));
	nowner_sid_ptr = (struct cifs_sid *)((char *)pnntsd + sidsoffset);
<<<<<<< HEAD
<<<<<<< HEAD

	nowner_sid_ptr->revision = owner_sid_ptr->revision;
	nowner_sid_ptr->num_subauth = owner_sid_ptr->num_subauth;
	for (i = 0; i < 6; i++)
		nowner_sid_ptr->authority[i] = owner_sid_ptr->authority[i];
	for (i = 0; i < 5; i++)
		nowner_sid_ptr->sub_auth[i] = owner_sid_ptr->sub_auth[i];
=======
	cifs_copy_sid(nowner_sid_ptr, owner_sid_ptr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cifs_copy_sid(nowner_sid_ptr, owner_sid_ptr);
>>>>>>> refs/remotes/origin/master

	/* copy group sid */
	group_sid_ptr = (struct cifs_sid *)((char *)pntsd +
				le32_to_cpu(pntsd->gsidoffset));
	ngroup_sid_ptr = (struct cifs_sid *)((char *)pnntsd + sidsoffset +
					sizeof(struct cifs_sid));
<<<<<<< HEAD
<<<<<<< HEAD

	ngroup_sid_ptr->revision = group_sid_ptr->revision;
	ngroup_sid_ptr->num_subauth = group_sid_ptr->num_subauth;
	for (i = 0; i < 6; i++)
		ngroup_sid_ptr->authority[i] = group_sid_ptr->authority[i];
	for (i = 0; i < 5; i++)
		ngroup_sid_ptr->sub_auth[i] = group_sid_ptr->sub_auth[i];
=======
	cifs_copy_sid(ngroup_sid_ptr, group_sid_ptr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cifs_copy_sid(ngroup_sid_ptr, group_sid_ptr);
>>>>>>> refs/remotes/origin/master

	return;
}


/*
   change posix mode to reflect permissions
   pmode is the existing mode (we only want to overwrite part of this
   bits to set can be: S_IRWXU, S_IRWXG or S_IRWXO ie 00700 or 00070 or 00007
*/
static void access_flags_to_mode(__le32 ace_flags, int type, umode_t *pmode,
				 umode_t *pbits_to_set)
{
	__u32 flags = le32_to_cpu(ace_flags);
	/* the order of ACEs is important.  The canonical order is to begin with
	   DENY entries followed by ALLOW, otherwise an allow entry could be
	   encountered first, making the subsequent deny entry like "dead code"
	   which would be superflous since Windows stops when a match is made
	   for the operation you are trying to perform for your user */

	/* For deny ACEs we change the mask so that subsequent allow access
	   control entries do not turn on the bits we are denying */
	if (type == ACCESS_DENIED) {
		if (flags & GENERIC_ALL)
			*pbits_to_set &= ~S_IRWXUGO;

		if ((flags & GENERIC_WRITE) ||
			((flags & FILE_WRITE_RIGHTS) == FILE_WRITE_RIGHTS))
			*pbits_to_set &= ~S_IWUGO;
		if ((flags & GENERIC_READ) ||
			((flags & FILE_READ_RIGHTS) == FILE_READ_RIGHTS))
			*pbits_to_set &= ~S_IRUGO;
		if ((flags & GENERIC_EXECUTE) ||
			((flags & FILE_EXEC_RIGHTS) == FILE_EXEC_RIGHTS))
			*pbits_to_set &= ~S_IXUGO;
		return;
	} else if (type != ACCESS_ALLOWED) {
<<<<<<< HEAD
		cERROR(1, "unknown access control type %d", type);
=======
		cifs_dbg(VFS, "unknown access control type %d\n", type);
>>>>>>> refs/remotes/origin/master
		return;
	}
	/* else ACCESS_ALLOWED type */

	if (flags & GENERIC_ALL) {
		*pmode |= (S_IRWXUGO & (*pbits_to_set));
<<<<<<< HEAD
		cFYI(DBG2, "all perms");
=======
		cifs_dbg(NOISY, "all perms\n");
>>>>>>> refs/remotes/origin/master
		return;
	}
	if ((flags & GENERIC_WRITE) ||
			((flags & FILE_WRITE_RIGHTS) == FILE_WRITE_RIGHTS))
		*pmode |= (S_IWUGO & (*pbits_to_set));
	if ((flags & GENERIC_READ) ||
			((flags & FILE_READ_RIGHTS) == FILE_READ_RIGHTS))
		*pmode |= (S_IRUGO & (*pbits_to_set));
	if ((flags & GENERIC_EXECUTE) ||
			((flags & FILE_EXEC_RIGHTS) == FILE_EXEC_RIGHTS))
		*pmode |= (S_IXUGO & (*pbits_to_set));

<<<<<<< HEAD
	cFYI(DBG2, "access flags 0x%x mode now 0x%x", flags, *pmode);
=======
	cifs_dbg(NOISY, "access flags 0x%x mode now 0x%x\n", flags, *pmode);
>>>>>>> refs/remotes/origin/master
	return;
}

/*
   Generate access flags to reflect permissions mode is the existing mode.
   This function is called for every ACE in the DACL whose SID matches
   with either owner or group or everyone.
*/

static void mode_to_access_flags(umode_t mode, umode_t bits_to_use,
				__u32 *pace_flags)
{
	/* reset access mask */
	*pace_flags = 0x0;

	/* bits to use are either S_IRWXU or S_IRWXG or S_IRWXO */
	mode &= bits_to_use;

	/* check for R/W/X UGO since we do not know whose flags
	   is this but we have cleared all the bits sans RWX for
	   either user or group or other as per bits_to_use */
	if (mode & S_IRUGO)
		*pace_flags |= SET_FILE_READ_RIGHTS;
	if (mode & S_IWUGO)
		*pace_flags |= SET_FILE_WRITE_RIGHTS;
	if (mode & S_IXUGO)
		*pace_flags |= SET_FILE_EXEC_RIGHTS;

<<<<<<< HEAD
	cFYI(DBG2, "mode: 0x%x, access flags now 0x%x", mode, *pace_flags);
=======
	cifs_dbg(NOISY, "mode: 0x%x, access flags now 0x%x\n",
		 mode, *pace_flags);
>>>>>>> refs/remotes/origin/master
	return;
}

static __u16 fill_ace_for_sid(struct cifs_ace *pntace,
			const struct cifs_sid *psid, __u64 nmode, umode_t bits)
{
	int i;
	__u16 size = 0;
	__u32 access_req = 0;

	pntace->type = ACCESS_ALLOWED;
	pntace->flags = 0x0;
	mode_to_access_flags(nmode, bits, &access_req);
	if (!access_req)
		access_req = SET_MINIMUM_RIGHTS;
	pntace->access_req = cpu_to_le32(access_req);

	pntace->sid.revision = psid->revision;
	pntace->sid.num_subauth = psid->num_subauth;
<<<<<<< HEAD
	for (i = 0; i < 6; i++)
=======
	for (i = 0; i < NUM_AUTHS; i++)
>>>>>>> refs/remotes/origin/master
		pntace->sid.authority[i] = psid->authority[i];
	for (i = 0; i < psid->num_subauth; i++)
		pntace->sid.sub_auth[i] = psid->sub_auth[i];

	size = 1 + 1 + 2 + 4 + 1 + 1 + 6 + (psid->num_subauth * 4);
	pntace->size = cpu_to_le16(size);

	return size;
}


#ifdef CONFIG_CIFS_DEBUG2
static void dump_ace(struct cifs_ace *pace, char *end_of_acl)
{
	int num_subauth;

	/* validate that we do not go past end of acl */

	if (le16_to_cpu(pace->size) < 16) {
<<<<<<< HEAD
		cERROR(1, "ACE too small %d", le16_to_cpu(pace->size));
=======
		cifs_dbg(VFS, "ACE too small %d\n", le16_to_cpu(pace->size));
>>>>>>> refs/remotes/origin/master
		return;
	}

	if (end_of_acl < (char *)pace + le16_to_cpu(pace->size)) {
<<<<<<< HEAD
		cERROR(1, "ACL too small to parse ACE");
=======
		cifs_dbg(VFS, "ACL too small to parse ACE\n");
>>>>>>> refs/remotes/origin/master
		return;
	}

	num_subauth = pace->sid.num_subauth;
	if (num_subauth) {
		int i;
<<<<<<< HEAD
		cFYI(1, "ACE revision %d num_auth %d type %d flags %d size %d",
			pace->sid.revision, pace->sid.num_subauth, pace->type,
			pace->flags, le16_to_cpu(pace->size));
		for (i = 0; i < num_subauth; ++i) {
			cFYI(1, "ACE sub_auth[%d]: 0x%x", i,
				le32_to_cpu(pace->sid.sub_auth[i]));
=======
		cifs_dbg(FYI, "ACE revision %d num_auth %d type %d flags %d size %d\n",
			 pace->sid.revision, pace->sid.num_subauth, pace->type,
			 pace->flags, le16_to_cpu(pace->size));
		for (i = 0; i < num_subauth; ++i) {
			cifs_dbg(FYI, "ACE sub_auth[%d]: 0x%x\n",
				 i, le32_to_cpu(pace->sid.sub_auth[i]));
>>>>>>> refs/remotes/origin/master
		}

		/* BB add length check to make sure that we do not have huge
			num auths and therefore go off the end */
	}

	return;
}
#endif


static void parse_dacl(struct cifs_acl *pdacl, char *end_of_acl,
		       struct cifs_sid *pownersid, struct cifs_sid *pgrpsid,
		       struct cifs_fattr *fattr)
{
	int i;
	int num_aces = 0;
	int acl_size;
	char *acl_base;
	struct cifs_ace **ppace;

	/* BB need to add parm so we can store the SID BB */

	if (!pdacl) {
		/* no DACL in the security descriptor, set
		   all the permissions for user/group/other */
		fattr->cf_mode |= S_IRWXUGO;
		return;
	}

	/* validate that we do not go past end of acl */
	if (end_of_acl < (char *)pdacl + le16_to_cpu(pdacl->size)) {
<<<<<<< HEAD
		cERROR(1, "ACL too small to parse DACL");
		return;
	}

	cFYI(DBG2, "DACL revision %d size %d num aces %d",
		le16_to_cpu(pdacl->revision), le16_to_cpu(pdacl->size),
		le32_to_cpu(pdacl->num_aces));
=======
		cifs_dbg(VFS, "ACL too small to parse DACL\n");
		return;
	}

	cifs_dbg(NOISY, "DACL revision %d size %d num aces %d\n",
		 le16_to_cpu(pdacl->revision), le16_to_cpu(pdacl->size),
		 le32_to_cpu(pdacl->num_aces));
>>>>>>> refs/remotes/origin/master

	/* reset rwx permissions for user/group/other.
	   Also, if num_aces is 0 i.e. DACL has no ACEs,
	   user/group/other have no permissions */
	fattr->cf_mode &= ~(S_IRWXUGO);

	acl_base = (char *)pdacl;
	acl_size = sizeof(struct cifs_acl);

	num_aces = le32_to_cpu(pdacl->num_aces);
<<<<<<< HEAD
<<<<<<< HEAD
	if (num_aces  > 0) {
=======
	if (num_aces > 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (num_aces > 0) {
>>>>>>> refs/remotes/origin/master
		umode_t user_mask = S_IRWXU;
		umode_t group_mask = S_IRWXG;
		umode_t other_mask = S_IRWXU | S_IRWXG | S_IRWXO;

<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (num_aces > ULONG_MAX / sizeof(struct cifs_ace *))
			return;
>>>>>>> refs/remotes/origin/cm-10.0
		ppace = kmalloc(num_aces * sizeof(struct cifs_ace *),
				GFP_KERNEL);
		if (!ppace) {
			cERROR(1, "DACL memory allocation error");
			return;
		}
=======
		if (num_aces > ULONG_MAX / sizeof(struct cifs_ace *))
			return;
		ppace = kmalloc(num_aces * sizeof(struct cifs_ace *),
				GFP_KERNEL);
		if (!ppace)
			return;
>>>>>>> refs/remotes/origin/master

		for (i = 0; i < num_aces; ++i) {
			ppace[i] = (struct cifs_ace *) (acl_base + acl_size);
#ifdef CONFIG_CIFS_DEBUG2
			dump_ace(ppace[i], end_of_acl);
#endif
			if (compare_sids(&(ppace[i]->sid), pownersid) == 0)
				access_flags_to_mode(ppace[i]->access_req,
						     ppace[i]->type,
						     &fattr->cf_mode,
						     &user_mask);
			if (compare_sids(&(ppace[i]->sid), pgrpsid) == 0)
				access_flags_to_mode(ppace[i]->access_req,
						     ppace[i]->type,
						     &fattr->cf_mode,
						     &group_mask);
			if (compare_sids(&(ppace[i]->sid), &sid_everyone) == 0)
				access_flags_to_mode(ppace[i]->access_req,
						     ppace[i]->type,
						     &fattr->cf_mode,
						     &other_mask);
			if (compare_sids(&(ppace[i]->sid), &sid_authusers) == 0)
				access_flags_to_mode(ppace[i]->access_req,
						     ppace[i]->type,
						     &fattr->cf_mode,
						     &other_mask);


/*			memcpy((void *)(&(cifscred->aces[i])),
				(void *)ppace[i],
				sizeof(struct cifs_ace)); */

			acl_base = (char *)ppace[i];
			acl_size = le16_to_cpu(ppace[i]->size);
		}

		kfree(ppace);
	}

	return;
}


static int set_chmod_dacl(struct cifs_acl *pndacl, struct cifs_sid *pownersid,
			struct cifs_sid *pgrpsid, __u64 nmode)
{
	u16 size = 0;
	struct cifs_acl *pnndacl;

	pnndacl = (struct cifs_acl *)((char *)pndacl + sizeof(struct cifs_acl));

	size += fill_ace_for_sid((struct cifs_ace *) ((char *)pnndacl + size),
					pownersid, nmode, S_IRWXU);
	size += fill_ace_for_sid((struct cifs_ace *)((char *)pnndacl + size),
					pgrpsid, nmode, S_IRWXG);
	size += fill_ace_for_sid((struct cifs_ace *)((char *)pnndacl + size),
					 &sid_everyone, nmode, S_IRWXO);

	pndacl->size = cpu_to_le16(size + sizeof(struct cifs_acl));
	pndacl->num_aces = cpu_to_le32(3);

	return 0;
}


static int parse_sid(struct cifs_sid *psid, char *end_of_acl)
{
	/* BB need to add parm so we can store the SID BB */

	/* validate that we do not go past end of ACL - sid must be at least 8
	   bytes long (assuming no sub-auths - e.g. the null SID */
	if (end_of_acl < (char *)psid + 8) {
<<<<<<< HEAD
		cERROR(1, "ACL too small to parse SID %p", psid);
		return -EINVAL;
	}

	if (psid->num_subauth) {
#ifdef CONFIG_CIFS_DEBUG2
		int i;
		cFYI(1, "SID revision %d num_auth %d",
			psid->revision, psid->num_subauth);

		for (i = 0; i < psid->num_subauth; i++) {
			cFYI(1, "SID sub_auth[%d]: 0x%x ", i,
				le32_to_cpu(psid->sub_auth[i]));
=======
		cifs_dbg(VFS, "ACL too small to parse SID %p\n", psid);
		return -EINVAL;
	}

#ifdef CONFIG_CIFS_DEBUG2
	if (psid->num_subauth) {
		int i;
		cifs_dbg(FYI, "SID revision %d num_auth %d\n",
			 psid->revision, psid->num_subauth);

		for (i = 0; i < psid->num_subauth; i++) {
			cifs_dbg(FYI, "SID sub_auth[%d]: 0x%x\n",
				 i, le32_to_cpu(psid->sub_auth[i]));
>>>>>>> refs/remotes/origin/master
		}

		/* BB add length check to make sure that we do not have huge
			num auths and therefore go off the end */
<<<<<<< HEAD
		cFYI(1, "RID 0x%x",
			le32_to_cpu(psid->sub_auth[psid->num_subauth-1]));
#endif
	}
=======
		cifs_dbg(FYI, "RID 0x%x\n",
			 le32_to_cpu(psid->sub_auth[psid->num_subauth-1]));
	}
#endif
>>>>>>> refs/remotes/origin/master

	return 0;
}


/* Convert CIFS ACL to POSIX form */
static int parse_sec_desc(struct cifs_sb_info *cifs_sb,
		struct cifs_ntsd *pntsd, int acl_len, struct cifs_fattr *fattr)
{
	int rc = 0;
	struct cifs_sid *owner_sid_ptr, *group_sid_ptr;
	struct cifs_acl *dacl_ptr; /* no need for SACL ptr */
	char *end_of_acl = ((char *)pntsd) + acl_len;
	__u32 dacloffset;

	if (pntsd == NULL)
		return -EIO;

	owner_sid_ptr = (struct cifs_sid *)((char *)pntsd +
				le32_to_cpu(pntsd->osidoffset));
	group_sid_ptr = (struct cifs_sid *)((char *)pntsd +
				le32_to_cpu(pntsd->gsidoffset));
	dacloffset = le32_to_cpu(pntsd->dacloffset);
	dacl_ptr = (struct cifs_acl *)((char *)pntsd + dacloffset);
<<<<<<< HEAD
	cFYI(DBG2, "revision %d type 0x%x ooffset 0x%x goffset 0x%x "
		 "sacloffset 0x%x dacloffset 0x%x",
=======
	cifs_dbg(NOISY, "revision %d type 0x%x ooffset 0x%x goffset 0x%x sacloffset 0x%x dacloffset 0x%x\n",
>>>>>>> refs/remotes/origin/master
		 pntsd->revision, pntsd->type, le32_to_cpu(pntsd->osidoffset),
		 le32_to_cpu(pntsd->gsidoffset),
		 le32_to_cpu(pntsd->sacloffset), dacloffset);
/*	cifs_dump_mem("owner_sid: ", owner_sid_ptr, 64); */
	rc = parse_sid(owner_sid_ptr, end_of_acl);
	if (rc) {
<<<<<<< HEAD
		cFYI(1, "%s: Error %d parsing Owner SID", __func__, rc);
=======
		cifs_dbg(FYI, "%s: Error %d parsing Owner SID\n", __func__, rc);
>>>>>>> refs/remotes/origin/master
		return rc;
	}
	rc = sid_to_id(cifs_sb, owner_sid_ptr, fattr, SIDOWNER);
	if (rc) {
<<<<<<< HEAD
		cFYI(1, "%s: Error %d mapping Owner SID to uid", __func__, rc);
=======
		cifs_dbg(FYI, "%s: Error %d mapping Owner SID to uid\n",
			 __func__, rc);
>>>>>>> refs/remotes/origin/master
		return rc;
	}

	rc = parse_sid(group_sid_ptr, end_of_acl);
	if (rc) {
<<<<<<< HEAD
		cFYI(1, "%s: Error %d mapping Owner SID to gid", __func__, rc);
=======
		cifs_dbg(FYI, "%s: Error %d mapping Owner SID to gid\n",
			 __func__, rc);
>>>>>>> refs/remotes/origin/master
		return rc;
	}
	rc = sid_to_id(cifs_sb, group_sid_ptr, fattr, SIDGROUP);
	if (rc) {
<<<<<<< HEAD
		cFYI(1, "%s: Error %d mapping Group SID to gid", __func__, rc);
=======
		cifs_dbg(FYI, "%s: Error %d mapping Group SID to gid\n",
			 __func__, rc);
>>>>>>> refs/remotes/origin/master
		return rc;
	}

	if (dacloffset)
		parse_dacl(dacl_ptr, end_of_acl, owner_sid_ptr,
			   group_sid_ptr, fattr);
	else
<<<<<<< HEAD
		cFYI(1, "no ACL"); /* BB grant all or default perms? */

<<<<<<< HEAD
/*	cifscred->uid = owner_sid_ptr->rid;
	cifscred->gid = group_sid_ptr->rid;
	memcpy((void *)(&(cifscred->osid)), (void *)owner_sid_ptr,
			sizeof(struct cifs_sid));
	memcpy((void *)(&(cifscred->gsid)), (void *)group_sid_ptr,
			sizeof(struct cifs_sid)); */
=======
		cifs_dbg(FYI, "no ACL\n"); /* BB grant all or default perms? */
>>>>>>> refs/remotes/origin/master

	return rc;
}

<<<<<<< HEAD

/* Convert permission bits from mode to equivalent CIFS ACL */
static int build_sec_desc(struct cifs_ntsd *pntsd, struct cifs_ntsd *pnntsd,
				struct inode *inode, __u64 nmode)
=======
	return rc;
}

/* Convert permission bits from mode to equivalent CIFS ACL */
static int build_sec_desc(struct cifs_ntsd *pntsd, struct cifs_ntsd *pnntsd,
	__u32 secdesclen, __u64 nmode, uid_t uid, gid_t gid, int *aclflag)
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Convert permission bits from mode to equivalent CIFS ACL */
static int build_sec_desc(struct cifs_ntsd *pntsd, struct cifs_ntsd *pnntsd,
	__u32 secdesclen, __u64 nmode, kuid_t uid, kgid_t gid, int *aclflag)
>>>>>>> refs/remotes/origin/master
{
	int rc = 0;
	__u32 dacloffset;
	__u32 ndacloffset;
	__u32 sidsoffset;
	struct cifs_sid *owner_sid_ptr, *group_sid_ptr;
<<<<<<< HEAD
<<<<<<< HEAD
	struct cifs_acl *dacl_ptr = NULL;  /* no need for SACL ptr */
	struct cifs_acl *ndacl_ptr = NULL; /* no need for SACL ptr */

	if ((inode == NULL) || (pntsd == NULL) || (pnntsd == NULL))
		return -EIO;

	owner_sid_ptr = (struct cifs_sid *)((char *)pntsd +
				le32_to_cpu(pntsd->osidoffset));
	group_sid_ptr = (struct cifs_sid *)((char *)pntsd +
				le32_to_cpu(pntsd->gsidoffset));

	dacloffset = le32_to_cpu(pntsd->dacloffset);
	dacl_ptr = (struct cifs_acl *)((char *)pntsd + dacloffset);

	ndacloffset = sizeof(struct cifs_ntsd);
	ndacl_ptr = (struct cifs_acl *)((char *)pnntsd + ndacloffset);
	ndacl_ptr->revision = dacl_ptr->revision;
	ndacl_ptr->size = 0;
	ndacl_ptr->num_aces = 0;

	rc = set_chmod_dacl(ndacl_ptr, owner_sid_ptr, group_sid_ptr, nmode);

	sidsoffset = ndacloffset + le16_to_cpu(ndacl_ptr->size);

	/* copy security descriptor control portion and owner and group sid */
	copy_sec_desc(pntsd, pnntsd, sidsoffset);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct cifs_sid *nowner_sid_ptr, *ngroup_sid_ptr;
	struct cifs_acl *dacl_ptr = NULL;  /* no need for SACL ptr */
	struct cifs_acl *ndacl_ptr = NULL; /* no need for SACL ptr */

	if (nmode != NO_CHANGE_64) { /* chmod */
		owner_sid_ptr = (struct cifs_sid *)((char *)pntsd +
				le32_to_cpu(pntsd->osidoffset));
		group_sid_ptr = (struct cifs_sid *)((char *)pntsd +
				le32_to_cpu(pntsd->gsidoffset));
		dacloffset = le32_to_cpu(pntsd->dacloffset);
		dacl_ptr = (struct cifs_acl *)((char *)pntsd + dacloffset);
		ndacloffset = sizeof(struct cifs_ntsd);
		ndacl_ptr = (struct cifs_acl *)((char *)pnntsd + ndacloffset);
		ndacl_ptr->revision = dacl_ptr->revision;
		ndacl_ptr->size = 0;
		ndacl_ptr->num_aces = 0;

		rc = set_chmod_dacl(ndacl_ptr, owner_sid_ptr, group_sid_ptr,
					nmode);
		sidsoffset = ndacloffset + le16_to_cpu(ndacl_ptr->size);
		/* copy sec desc control portion & owner and group sids */
		copy_sec_desc(pntsd, pnntsd, sidsoffset);
		*aclflag = CIFS_ACL_DACL;
	} else {
		memcpy(pnntsd, pntsd, secdesclen);
<<<<<<< HEAD
		if (uid != NO_CHANGE_32) { /* chown */
=======
		if (uid_valid(uid)) { /* chown */
			uid_t id;
>>>>>>> refs/remotes/origin/master
			owner_sid_ptr = (struct cifs_sid *)((char *)pnntsd +
					le32_to_cpu(pnntsd->osidoffset));
			nowner_sid_ptr = kmalloc(sizeof(struct cifs_sid),
								GFP_KERNEL);
			if (!nowner_sid_ptr)
				return -ENOMEM;
<<<<<<< HEAD
			rc = id_to_sid(uid, SIDOWNER, nowner_sid_ptr);
			if (rc) {
				cFYI(1, "%s: Mapping error %d for owner id %d",
						__func__, rc, uid);
=======
			id = from_kuid(&init_user_ns, uid);
			rc = id_to_sid(id, SIDOWNER, nowner_sid_ptr);
			if (rc) {
				cifs_dbg(FYI, "%s: Mapping error %d for owner id %d\n",
					 __func__, rc, id);
>>>>>>> refs/remotes/origin/master
				kfree(nowner_sid_ptr);
				return rc;
			}
			cifs_copy_sid(owner_sid_ptr, nowner_sid_ptr);
			kfree(nowner_sid_ptr);
			*aclflag = CIFS_ACL_OWNER;
		}
<<<<<<< HEAD
		if (gid != NO_CHANGE_32) { /* chgrp */
=======
		if (gid_valid(gid)) { /* chgrp */
			gid_t id;
>>>>>>> refs/remotes/origin/master
			group_sid_ptr = (struct cifs_sid *)((char *)pnntsd +
					le32_to_cpu(pnntsd->gsidoffset));
			ngroup_sid_ptr = kmalloc(sizeof(struct cifs_sid),
								GFP_KERNEL);
			if (!ngroup_sid_ptr)
				return -ENOMEM;
<<<<<<< HEAD
			rc = id_to_sid(gid, SIDGROUP, ngroup_sid_ptr);
			if (rc) {
				cFYI(1, "%s: Mapping error %d for group id %d",
						__func__, rc, gid);
=======
			id = from_kgid(&init_user_ns, gid);
			rc = id_to_sid(id, SIDGROUP, ngroup_sid_ptr);
			if (rc) {
				cifs_dbg(FYI, "%s: Mapping error %d for group id %d\n",
					 __func__, rc, id);
>>>>>>> refs/remotes/origin/master
				kfree(ngroup_sid_ptr);
				return rc;
			}
			cifs_copy_sid(group_sid_ptr, ngroup_sid_ptr);
			kfree(ngroup_sid_ptr);
			*aclflag = CIFS_ACL_GROUP;
		}
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return rc;
}

static struct cifs_ntsd *get_cifs_acl_by_fid(struct cifs_sb_info *cifs_sb,
		__u16 fid, u32 *pacllen)
{
	struct cifs_ntsd *pntsd = NULL;
<<<<<<< HEAD
	int xid, rc;
=======
	unsigned int xid;
	int rc;
>>>>>>> refs/remotes/origin/master
	struct tcon_link *tlink = cifs_sb_tlink(cifs_sb);

	if (IS_ERR(tlink))
		return ERR_CAST(tlink);

<<<<<<< HEAD
	xid = GetXid();
	rc = CIFSSMBGetCIFSACL(xid, tlink_tcon(tlink), fid, &pntsd, pacllen);
	FreeXid(xid);

	cifs_put_tlink(tlink);

	cFYI(1, "%s: rc = %d ACL len %d", __func__, rc, *pacllen);
=======
	xid = get_xid();
	rc = CIFSSMBGetCIFSACL(xid, tlink_tcon(tlink), fid, &pntsd, pacllen);
	free_xid(xid);

	cifs_put_tlink(tlink);

	cifs_dbg(FYI, "%s: rc = %d ACL len %d\n", __func__, rc, *pacllen);
>>>>>>> refs/remotes/origin/master
	if (rc)
		return ERR_PTR(rc);
	return pntsd;
}

static struct cifs_ntsd *get_cifs_acl_by_path(struct cifs_sb_info *cifs_sb,
		const char *path, u32 *pacllen)
{
	struct cifs_ntsd *pntsd = NULL;
	int oplock = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	int xid, rc;
=======
	int xid, rc, create_options = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int xid;
	int rc, create_options = 0;
>>>>>>> refs/remotes/origin/master
	__u16 fid;
	struct cifs_tcon *tcon;
	struct tcon_link *tlink = cifs_sb_tlink(cifs_sb);

	if (IS_ERR(tlink))
		return ERR_CAST(tlink);

	tcon = tlink_tcon(tlink);
<<<<<<< HEAD
	xid = GetXid();

<<<<<<< HEAD
	rc = CIFSSMBOpen(xid, tcon, path, FILE_OPEN, READ_CONTROL, 0,
			 &fid, &oplock, NULL, cifs_sb->local_nls,
			 cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
=======
=======
	xid = get_xid();

>>>>>>> refs/remotes/origin/master
	if (backup_cred(cifs_sb))
		create_options |= CREATE_OPEN_BACKUP_INTENT;

	rc = CIFSSMBOpen(xid, tcon, path, FILE_OPEN, READ_CONTROL,
			create_options, &fid, &oplock, NULL, cifs_sb->local_nls,
			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!rc) {
		rc = CIFSSMBGetCIFSACL(xid, tcon, fid, &pntsd, pacllen);
		CIFSSMBClose(xid, tcon, fid);
	}

	cifs_put_tlink(tlink);
<<<<<<< HEAD
	FreeXid(xid);

	cFYI(1, "%s: rc = %d ACL len %d", __func__, rc, *pacllen);
=======
	free_xid(xid);

	cifs_dbg(FYI, "%s: rc = %d ACL len %d\n", __func__, rc, *pacllen);
>>>>>>> refs/remotes/origin/master
	if (rc)
		return ERR_PTR(rc);
	return pntsd;
}

/* Retrieve an ACL from the server */
struct cifs_ntsd *get_cifs_acl(struct cifs_sb_info *cifs_sb,
				      struct inode *inode, const char *path,
				      u32 *pacllen)
{
	struct cifs_ntsd *pntsd = NULL;
	struct cifsFileInfo *open_file = NULL;

	if (inode)
		open_file = find_readable_file(CIFS_I(inode), true);
	if (!open_file)
		return get_cifs_acl_by_path(cifs_sb, path, pacllen);

<<<<<<< HEAD
	pntsd = get_cifs_acl_by_fid(cifs_sb, open_file->netfid, pacllen);
=======
	pntsd = get_cifs_acl_by_fid(cifs_sb, open_file->fid.netfid, pacllen);
>>>>>>> refs/remotes/origin/master
	cifsFileInfo_put(open_file);
	return pntsd;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int set_cifs_acl_by_fid(struct cifs_sb_info *cifs_sb, __u16 fid,
		struct cifs_ntsd *pnntsd, u32 acllen)
{
	int xid, rc;
	struct tcon_link *tlink = cifs_sb_tlink(cifs_sb);

	if (IS_ERR(tlink))
		return PTR_ERR(tlink);

	xid = GetXid();
	rc = CIFSSMBSetCIFSACL(xid, tlink_tcon(tlink), fid, pnntsd, acllen);
	FreeXid(xid);
	cifs_put_tlink(tlink);

	cFYI(DBG2, "SetCIFSACL rc = %d", rc);
	return rc;
}

static int set_cifs_acl_by_path(struct cifs_sb_info *cifs_sb, const char *path,
		struct cifs_ntsd *pnntsd, u32 acllen)
{
	int oplock = 0;
	int xid, rc;
	__u16 fid;
	struct cifs_tcon *tcon;
=======
=======
>>>>>>> refs/remotes/origin/master
 /* Set an ACL on the server */
int set_cifs_acl(struct cifs_ntsd *pnntsd, __u32 acllen,
			struct inode *inode, const char *path, int aclflag)
{
	int oplock = 0;
<<<<<<< HEAD
	int xid, rc, access_flags, create_options = 0;
	__u16 fid;
	struct cifs_tcon *tcon;
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int xid;
	int rc, access_flags, create_options = 0;
	__u16 fid;
	struct cifs_tcon *tcon;
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
>>>>>>> refs/remotes/origin/master
	struct tcon_link *tlink = cifs_sb_tlink(cifs_sb);

	if (IS_ERR(tlink))
		return PTR_ERR(tlink);

	tcon = tlink_tcon(tlink);
<<<<<<< HEAD
	xid = GetXid();

<<<<<<< HEAD
	rc = CIFSSMBOpen(xid, tcon, path, FILE_OPEN, WRITE_DAC, 0,
			 &fid, &oplock, NULL, cifs_sb->local_nls,
			 cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
=======
=======
	xid = get_xid();

>>>>>>> refs/remotes/origin/master
	if (backup_cred(cifs_sb))
		create_options |= CREATE_OPEN_BACKUP_INTENT;

	if (aclflag == CIFS_ACL_OWNER || aclflag == CIFS_ACL_GROUP)
		access_flags = WRITE_OWNER;
	else
		access_flags = WRITE_DAC;

	rc = CIFSSMBOpen(xid, tcon, path, FILE_OPEN, access_flags,
			create_options, &fid, &oplock, NULL, cifs_sb->local_nls,
			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (rc) {
		cERROR(1, "Unable to open file to set ACL");
		goto out;
	}

<<<<<<< HEAD
	rc = CIFSSMBSetCIFSACL(xid, tcon, fid, pnntsd, acllen);
=======
	rc = CIFSSMBSetCIFSACL(xid, tcon, fid, pnntsd, acllen, aclflag);
>>>>>>> refs/remotes/origin/cm-10.0
	cFYI(DBG2, "SetCIFSACL rc = %d", rc);

	CIFSSMBClose(xid, tcon, fid);
out:
	FreeXid(xid);
=======
	if (rc) {
		cifs_dbg(VFS, "Unable to open file to set ACL\n");
		goto out;
	}

	rc = CIFSSMBSetCIFSACL(xid, tcon, fid, pnntsd, acllen, aclflag);
	cifs_dbg(NOISY, "SetCIFSACL rc = %d\n", rc);

	CIFSSMBClose(xid, tcon, fid);
out:
	free_xid(xid);
>>>>>>> refs/remotes/origin/master
	cifs_put_tlink(tlink);
	return rc;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Set an ACL on the server */
int set_cifs_acl(struct cifs_ntsd *pnntsd, __u32 acllen,
				struct inode *inode, const char *path)
{
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
	struct cifsFileInfo *open_file;
	int rc;

	cFYI(DBG2, "set ACL for %s from mode 0x%x", path, inode->i_mode);

	open_file = find_readable_file(CIFS_I(inode), true);
	if (!open_file)
		return set_cifs_acl_by_path(cifs_sb, path, pnntsd, acllen);

	rc = set_cifs_acl_by_fid(cifs_sb, open_file->netfid, pnntsd, acllen);
	cifsFileInfo_put(open_file);
	return rc;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* Translate the CIFS ACL (simlar to NTFS ACL) for a file into mode bits */
int
cifs_acl_to_fattr(struct cifs_sb_info *cifs_sb, struct cifs_fattr *fattr,
		  struct inode *inode, const char *path, const __u16 *pfid)
{
	struct cifs_ntsd *pntsd = NULL;
	u32 acllen = 0;
	int rc = 0;

<<<<<<< HEAD
	cFYI(DBG2, "converting ACL to mode for %s", path);
=======
	cifs_dbg(NOISY, "converting ACL to mode for %s\n", path);
>>>>>>> refs/remotes/origin/master

	if (pfid)
		pntsd = get_cifs_acl_by_fid(cifs_sb, *pfid, &acllen);
	else
		pntsd = get_cifs_acl(cifs_sb, inode, path, &acllen);

	/* if we can retrieve the ACL, now parse Access Control Entries, ACEs */
	if (IS_ERR(pntsd)) {
		rc = PTR_ERR(pntsd);
<<<<<<< HEAD
		cERROR(1, "%s: error %d getting sec desc", __func__, rc);
=======
		cifs_dbg(VFS, "%s: error %d getting sec desc\n", __func__, rc);
>>>>>>> refs/remotes/origin/master
	} else {
		rc = parse_sec_desc(cifs_sb, pntsd, acllen, fattr);
		kfree(pntsd);
		if (rc)
<<<<<<< HEAD
			cERROR(1, "parse sec desc failed rc = %d", rc);
=======
			cifs_dbg(VFS, "parse sec desc failed rc = %d\n", rc);
>>>>>>> refs/remotes/origin/master
	}

	return rc;
}

/* Convert mode bits to an ACL so we can update the ACL on the server */
<<<<<<< HEAD
<<<<<<< HEAD
int mode_to_cifs_acl(struct inode *inode, const char *path, __u64 nmode)
{
	int rc = 0;
=======
int
id_mode_to_cifs_acl(struct inode *inode, const char *path, __u64 nmode,
			uid_t uid, gid_t gid)
{
	int rc = 0;
	int aclflag = CIFS_ACL_DACL; /* default flag to set */
>>>>>>> refs/remotes/origin/cm-10.0
=======
int
id_mode_to_cifs_acl(struct inode *inode, const char *path, __u64 nmode,
			kuid_t uid, kgid_t gid)
{
	int rc = 0;
	int aclflag = CIFS_ACL_DACL; /* default flag to set */
>>>>>>> refs/remotes/origin/master
	__u32 secdesclen = 0;
	struct cifs_ntsd *pntsd = NULL; /* acl obtained from server */
	struct cifs_ntsd *pnntsd = NULL; /* modified acl to be sent to server */

<<<<<<< HEAD
	cFYI(DBG2, "set ACL from mode for %s", path);

	/* Get the security descriptor */
	pntsd = get_cifs_acl(CIFS_SB(inode->i_sb), inode, path, &secdesclen);

	/* Add three ACEs for owner, group, everyone getting rid of
	   other ACEs as chmod disables ACEs and set the security descriptor */

	if (IS_ERR(pntsd)) {
		rc = PTR_ERR(pntsd);
		cERROR(1, "%s: error %d getting sec desc", __func__, rc);
	} else {
		/* allocate memory for the smb header,
		   set security descriptor request security descriptor
		   parameters, and secuirty descriptor itself */

		secdesclen = secdesclen < DEFSECDESCLEN ?
					DEFSECDESCLEN : secdesclen;
		pnntsd = kmalloc(secdesclen, GFP_KERNEL);
		if (!pnntsd) {
			cERROR(1, "Unable to allocate security descriptor");
			kfree(pntsd);
			return -ENOMEM;
		}

<<<<<<< HEAD
		rc = build_sec_desc(pntsd, pnntsd, inode, nmode);
=======
		rc = build_sec_desc(pntsd, pnntsd, secdesclen, nmode, uid, gid,
					&aclflag);
>>>>>>> refs/remotes/origin/cm-10.0

		cFYI(DBG2, "build_sec_desc rc: %d", rc);

		if (!rc) {
			/* Set the security descriptor */
<<<<<<< HEAD
			rc = set_cifs_acl(pnntsd, secdesclen, inode, path);
=======
			rc = set_cifs_acl(pnntsd, secdesclen, inode,
						path, aclflag);
>>>>>>> refs/remotes/origin/cm-10.0
			cFYI(DBG2, "set_cifs_acl rc: %d", rc);
		}

		kfree(pnntsd);
		kfree(pntsd);
	}

=======
	cifs_dbg(NOISY, "set ACL from mode for %s\n", path);

	/* Get the security descriptor */
	pntsd = get_cifs_acl(CIFS_SB(inode->i_sb), inode, path, &secdesclen);
	if (IS_ERR(pntsd)) {
		rc = PTR_ERR(pntsd);
		cifs_dbg(VFS, "%s: error %d getting sec desc\n", __func__, rc);
		goto out;
	}

	/*
	 * Add three ACEs for owner, group, everyone getting rid of other ACEs
	 * as chmod disables ACEs and set the security descriptor. Allocate
	 * memory for the smb header, set security descriptor request security
	 * descriptor parameters, and secuirty descriptor itself
	 */
	secdesclen = max_t(u32, secdesclen, DEFAULT_SEC_DESC_LEN);
	pnntsd = kmalloc(secdesclen, GFP_KERNEL);
	if (!pnntsd) {
		kfree(pntsd);
		return -ENOMEM;
	}

	rc = build_sec_desc(pntsd, pnntsd, secdesclen, nmode, uid, gid,
				&aclflag);

	cifs_dbg(NOISY, "build_sec_desc rc: %d\n", rc);

	if (!rc) {
		/* Set the security descriptor */
		rc = set_cifs_acl(pnntsd, secdesclen, inode, path, aclflag);
		cifs_dbg(NOISY, "set_cifs_acl rc: %d\n", rc);
	}

	kfree(pnntsd);
	kfree(pntsd);
out:
>>>>>>> refs/remotes/origin/master
	return rc;
}
