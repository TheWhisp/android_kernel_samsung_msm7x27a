<<<<<<< HEAD
/* Copyright (C) 2008-2011 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
=======
/* Copyright (C) 2008-2013 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* Kernel module implementing an IP set type: the list:set type */

#include <linux/module.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/errno.h>

#include <linux/netfilter/ipset/ip_set.h>
<<<<<<< HEAD
#include <linux/netfilter/ipset/ip_set_timeout.h>
#include <linux/netfilter/ipset/ip_set_list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("list:set type of IP sets");
MODULE_ALIAS("ip_set_list:set");

/* Member elements without and with timeout */
=======
#include <linux/netfilter/ipset/ip_set_list.h>

#define IPSET_TYPE_REV_MIN	0
/*				1    Counters support added */
#define IPSET_TYPE_REV_MAX	2 /* Comments support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
IP_SET_MODULE_DESC("list:set", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
MODULE_ALIAS("ip_set_list:set");

/* Member elements  */
>>>>>>> refs/remotes/origin/master
struct set_elem {
	ip_set_id_t id;
};

<<<<<<< HEAD
struct set_telem {
	ip_set_id_t id;
	unsigned long timeout;
=======
struct set_adt_elem {
	ip_set_id_t id;
	ip_set_id_t refid;
	int before;
>>>>>>> refs/remotes/origin/master
};

/* Type structure */
struct list_set {
<<<<<<< HEAD
	size_t dsize;		/* element size */
	u32 size;		/* size of set list array */
	u32 timeout;		/* timeout value */
	struct timer_list gc;	/* garbage collection */
	struct set_elem members[0]; /* the set members */
};

static inline struct set_elem *
list_set_elem(const struct list_set *map, u32 id)
{
	return (struct set_elem *)((void *)map->members + id * map->dsize);
}

static inline struct set_telem *
list_set_telem(const struct list_set *map, u32 id)
{
	return (struct set_telem *)((void *)map->members + id * map->dsize);
}

static inline bool
list_set_timeout(const struct list_set *map, u32 id)
{
	const struct set_telem *elem = list_set_telem(map, id);

	return ip_set_timeout_test(elem->timeout);
}

static inline bool
list_set_expired(const struct list_set *map, u32 id)
{
	const struct set_telem *elem = list_set_telem(map, id);

	return ip_set_timeout_expired(elem->timeout);
}

/* Set list without and with timeout */

static int
list_set_kadt(struct ip_set *set, const struct sk_buff *skb,
<<<<<<< HEAD
	      enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
	      const struct xt_action_param *par,
	      enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct list_set *map = set->data;
	struct set_elem *elem;
=======
	u32 size;		/* size of set list array */
	struct timer_list gc;	/* garbage collection */
	struct net *net;	/* namespace */
	struct set_elem members[0]; /* the set members */
};

#define list_set_elem(set, map, id)	\
	(struct set_elem *)((void *)(map)->members + (id) * (set)->dsize)

static int
list_set_ktest(struct ip_set *set, const struct sk_buff *skb,
	       const struct xt_action_param *par,
	       struct ip_set_adt_opt *opt, const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e;
	u32 i, cmdflags = opt->cmdflags;
	int ret;

	/* Don't lookup sub-counters at all */
	opt->cmdflags &= ~IPSET_FLAG_MATCH_COUNTERS;
	if (opt->cmdflags & IPSET_FLAG_SKIP_SUBCOUNTER_UPDATE)
		opt->cmdflags &= ~IPSET_FLAG_SKIP_COUNTER_UPDATE;
	for (i = 0; i < map->size; i++) {
		e = list_set_elem(set, map, i);
		if (e->id == IPSET_INVALID_ID)
			return 0;
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		ret = ip_set_test(e->id, skb, par, opt);
		if (ret > 0) {
			if (SET_WITH_COUNTER(set))
				ip_set_update_counter(ext_counter(e, set),
						      ext, &opt->ext,
						      cmdflags);
			return ret;
		}
	}
	return 0;
}

static int
list_set_kadd(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      struct ip_set_adt_opt *opt, const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e;
	u32 i;
	int ret;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(set, map, i);
		if (e->id == IPSET_INVALID_ID)
			return 0;
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		ret = ip_set_add(e->id, skb, par, opt);
		if (ret == 0)
			return ret;
	}
	return 0;
}

static int
list_set_kdel(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      struct ip_set_adt_opt *opt, const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e;
>>>>>>> refs/remotes/origin/master
	u32 i;
	int ret;

	for (i = 0; i < map->size; i++) {
<<<<<<< HEAD
		elem = list_set_elem(map, i);
		if (elem->id == IPSET_INVALID_ID)
			return 0;
		if (with_timeout(map->timeout) && list_set_expired(map, i))
			continue;
		switch (adt) {
		case IPSET_TEST:
<<<<<<< HEAD
			ret = ip_set_test(elem->id, skb, pf, dim, flags);
=======
			ret = ip_set_test(elem->id, skb, par, opt);
>>>>>>> refs/remotes/origin/cm-10.0
			if (ret > 0)
				return ret;
			break;
		case IPSET_ADD:
<<<<<<< HEAD
			ret = ip_set_add(elem->id, skb, pf, dim, flags);
=======
			ret = ip_set_add(elem->id, skb, par, opt);
>>>>>>> refs/remotes/origin/cm-10.0
			if (ret == 0)
				return ret;
			break;
		case IPSET_DEL:
<<<<<<< HEAD
			ret = ip_set_del(elem->id, skb, pf, dim, flags);
=======
			ret = ip_set_del(elem->id, skb, par, opt);
>>>>>>> refs/remotes/origin/cm-10.0
			if (ret == 0)
				return ret;
			break;
		default:
			break;
		}
=======
		e = list_set_elem(set, map, i);
		if (e->id == IPSET_INVALID_ID)
			return 0;
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		ret = ip_set_del(e->id, skb, par, opt);
		if (ret == 0)
			return ret;
	}
	return 0;
}

static int
list_set_kadt(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);

	switch (adt) {
	case IPSET_TEST:
		return list_set_ktest(set, skb, par, opt, &ext);
	case IPSET_ADD:
		return list_set_kadd(set, skb, par, opt, &ext);
	case IPSET_DEL:
		return list_set_kdel(set, skb, par, opt, &ext);
	default:
		break;
>>>>>>> refs/remotes/origin/master
	}
	return -EINVAL;
}

static bool
<<<<<<< HEAD
<<<<<<< HEAD
next_id_eq(const struct list_set *map, u32 i, ip_set_id_t id)
{
	const struct set_elem *elem;

	if (i + 1 < map->size) {
		elem = list_set_elem(map, i + 1);
		return !!(elem->id == id &&
			  !(with_timeout(map->timeout) &&
			    list_set_expired(map, i + 1)));
=======
id_eq(const struct list_set *map, u32 i, ip_set_id_t id)
{
	const struct set_elem *elem;

	if (i < map->size) {
		elem = list_set_elem(map, i);
		return elem->id == id;
	}

	return 0;
}

static bool
id_eq_timeout(const struct list_set *map, u32 i, ip_set_id_t id)
{
	const struct set_elem *elem;

	if (i < map->size) {
		elem = list_set_elem(map, i);
		return !!(elem->id == id &&
			  !(with_timeout(map->timeout) &&
			    list_set_expired(map, i)));
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return 0;
}

static void
list_elem_add(struct list_set *map, u32 i, ip_set_id_t id)
{
	struct set_elem *e;

	for (; i < map->size; i++) {
		e = list_set_elem(map, i);
		swap(e->id, id);
		if (e->id == IPSET_INVALID_ID)
			break;
	}
}

static void
list_elem_tadd(struct list_set *map, u32 i, ip_set_id_t id,
	       unsigned long timeout)
{
	struct set_telem *e;

	for (; i < map->size; i++) {
		e = list_set_telem(map, i);
		swap(e->id, id);
		swap(e->timeout, timeout);
		if (e->id == IPSET_INVALID_ID)
			break;
=======
id_eq(const struct ip_set *set, u32 i, ip_set_id_t id)
{
	const struct list_set *map = set->data;
	const struct set_elem *e;

	if (i >= map->size)
		return 0;

	e = list_set_elem(set, map, i);
	return !!(e->id == id &&
		 !(SET_WITH_TIMEOUT(set) &&
		   ip_set_timeout_expired(ext_timeout(e, set))));
}

static int
list_set_add(struct ip_set *set, u32 i, struct set_adt_elem *d,
	     const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e = list_set_elem(set, map, i);

	if (e->id != IPSET_INVALID_ID) {
		if (i == map->size - 1) {
			/* Last element replaced: e.g. add new,before,last */
			ip_set_put_byindex(map->net, e->id);
			ip_set_ext_destroy(set, e);
		} else {
			struct set_elem *x = list_set_elem(set, map,
							   map->size - 1);

			/* Last element pushed off */
			if (x->id != IPSET_INVALID_ID) {
				ip_set_put_byindex(map->net, x->id);
				ip_set_ext_destroy(set, x);
			}
			memmove(list_set_elem(set, map, i + 1), e,
				set->dsize * (map->size - (i + 1)));
			/* Extensions must be initialized to zero */
			memset(e, 0, set->dsize);
		}
	}

	e->id = d->id;
	if (SET_WITH_TIMEOUT(set))
		ip_set_timeout_set(ext_timeout(e, set), ext->timeout);
	if (SET_WITH_COUNTER(set))
		ip_set_init_counter(ext_counter(e, set), ext);
	if (SET_WITH_COMMENT(set))
		ip_set_init_comment(ext_comment(e, set), ext);
	return 0;
}

static int
list_set_del(struct ip_set *set, u32 i)
{
	struct list_set *map = set->data;
	struct set_elem *e = list_set_elem(set, map, i);

	ip_set_put_byindex(map->net, e->id);
	ip_set_ext_destroy(set, e);

	if (i < map->size - 1)
		memmove(e, list_set_elem(set, map, i + 1),
			set->dsize * (map->size - (i + 1)));

	/* Last element */
	e = list_set_elem(set, map, map->size - 1);
	e->id = IPSET_INVALID_ID;
	return 0;
}

static void
set_cleanup_entries(struct ip_set *set)
{
	struct list_set *map = set->data;
	struct set_elem *e;
	u32 i = 0;

	while (i < map->size) {
		e = list_set_elem(set, map, i);
		if (e->id != IPSET_INVALID_ID &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			list_set_del(set, i);
			/* Check element moved to position i in next loop */
		else
			i++;
>>>>>>> refs/remotes/origin/master
	}
}

static int
<<<<<<< HEAD
list_set_add(struct list_set *map, u32 i, ip_set_id_t id,
	     unsigned long timeout)
{
	const struct set_elem *e = list_set_elem(map, i);

	if (i == map->size - 1 && e->id != IPSET_INVALID_ID)
		/* Last element replaced: e.g. add new,before,last */
		ip_set_put_byindex(e->id);
	if (with_timeout(map->timeout))
		list_elem_tadd(map, i, id, ip_set_timeout_set(timeout));
	else
		list_elem_add(map, i, id);

	return 0;
}

static int
list_set_del(struct list_set *map, u32 i)
{
	struct set_elem *a = list_set_elem(map, i), *b;

	ip_set_put_byindex(a->id);

	for (; i < map->size - 1; i++) {
		b = list_set_elem(map, i + 1);
		a->id = b->id;
		if (with_timeout(map->timeout))
			((struct set_telem *)a)->timeout =
				((struct set_telem *)b)->timeout;
		a = b;
		if (a->id == IPSET_INVALID_ID)
			break;
	}
	/* Last element */
	a->id = IPSET_INVALID_ID;
	return 0;
}

<<<<<<< HEAD
static int
list_set_uadt(struct ip_set *set, struct nlattr *tb[],
	      enum ipset_adt adt, u32 *lineno, u32 flags)
{
	struct list_set *map = set->data;
	bool with_timeout = with_timeout(map->timeout);
=======
static void
cleanup_entries(struct list_set *map)
{
	struct set_telem *e;
	u32 i;

	for (i = 0; i < map->size; i++) {
		e = list_set_telem(map, i);
		if (e->id != IPSET_INVALID_ID && list_set_expired(map, i))
			list_set_del(map, i);
	}
=======
list_set_utest(struct ip_set *set, void *value, const struct ip_set_ext *ext,
	       struct ip_set_ext *mext, u32 flags)
{
	struct list_set *map = set->data;
	struct set_adt_elem *d = value;
	struct set_elem *e;
	u32 i;
	int ret;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(set, map, i);
		if (e->id == IPSET_INVALID_ID)
			return 0;
		else if (SET_WITH_TIMEOUT(set) &&
			 ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		else if (e->id != d->id)
			continue;

		if (d->before == 0)
			return 1;
		else if (d->before > 0)
			ret = id_eq(set, i + 1, d->refid);
		else
			ret = i > 0 && id_eq(set, i - 1, d->refid);
		return ret;
	}
	return 0;
}


static int
list_set_uadd(struct ip_set *set, void *value, const struct ip_set_ext *ext,
	      struct ip_set_ext *mext, u32 flags)
{
	struct list_set *map = set->data;
	struct set_adt_elem *d = value;
	struct set_elem *e;
	bool flag_exist = flags & IPSET_FLAG_EXIST;
	u32 i, ret = 0;

	if (SET_WITH_TIMEOUT(set))
		set_cleanup_entries(set);

	/* Check already added element */
	for (i = 0; i < map->size; i++) {
		e = list_set_elem(set, map, i);
		if (e->id == IPSET_INVALID_ID)
			goto insert;
		else if (e->id != d->id)
			continue;

		if ((d->before > 1 && !id_eq(set, i + 1, d->refid)) ||
		    (d->before < 0 &&
		     (i == 0 || !id_eq(set, i - 1, d->refid))))
			/* Before/after doesn't match */
			return -IPSET_ERR_REF_EXIST;
		if (!flag_exist)
			/* Can't re-add */
			return -IPSET_ERR_EXIST;
		/* Update extensions */
		ip_set_ext_destroy(set, e);

		if (SET_WITH_TIMEOUT(set))
			ip_set_timeout_set(ext_timeout(e, set), ext->timeout);
		if (SET_WITH_COUNTER(set))
			ip_set_init_counter(ext_counter(e, set), ext);
		if (SET_WITH_COMMENT(set))
			ip_set_init_comment(ext_comment(e, set), ext);
		/* Set is already added to the list */
		ip_set_put_byindex(map->net, d->id);
		return 0;
	}
insert:
	ret = -IPSET_ERR_LIST_FULL;
	for (i = 0; i < map->size && ret == -IPSET_ERR_LIST_FULL; i++) {
		e = list_set_elem(set, map, i);
		if (e->id == IPSET_INVALID_ID)
			ret = d->before != 0 ? -IPSET_ERR_REF_EXIST
				: list_set_add(set, i, d, ext);
		else if (e->id != d->refid)
			continue;
		else if (d->before > 0)
			ret = list_set_add(set, i, d, ext);
		else if (i + 1 < map->size)
			ret = list_set_add(set, i + 1, d, ext);
	}

	return ret;
}

static int
list_set_udel(struct ip_set *set, void *value, const struct ip_set_ext *ext,
	      struct ip_set_ext *mext, u32 flags)
{
	struct list_set *map = set->data;
	struct set_adt_elem *d = value;
	struct set_elem *e;
	u32 i;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(set, map, i);
		if (e->id == IPSET_INVALID_ID)
			return d->before != 0 ? -IPSET_ERR_REF_EXIST
					      : -IPSET_ERR_EXIST;
		else if (SET_WITH_TIMEOUT(set) &&
			 ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		else if (e->id != d->id)
			continue;

		if (d->before == 0)
			return list_set_del(set, i);
		else if (d->before > 0) {
			if (!id_eq(set, i + 1, d->refid))
				return -IPSET_ERR_REF_EXIST;
			return list_set_del(set, i);
		} else if (i == 0 || !id_eq(set, i - 1, d->refid))
			return -IPSET_ERR_REF_EXIST;
		else
			return list_set_del(set, i);
	}
	return -IPSET_ERR_EXIST;
>>>>>>> refs/remotes/origin/master
}

static int
list_set_uadt(struct ip_set *set, struct nlattr *tb[],
	      enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	struct list_set *map = set->data;
<<<<<<< HEAD
	bool with_timeout = with_timeout(map->timeout);
	bool flag_exist = flags & IPSET_FLAG_EXIST;
>>>>>>> refs/remotes/origin/cm-10.0
	int before = 0;
	u32 timeout = map->timeout;
	ip_set_id_t id, refid = IPSET_INVALID_ID;
	const struct set_elem *elem;
	struct ip_set *s;
	u32 i;
=======
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct set_adt_elem e = { .refid = IPSET_INVALID_ID };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	struct ip_set *s;
>>>>>>> refs/remotes/origin/master
	int ret = 0;

	if (unlikely(!tb[IPSET_ATTR_NAME] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
<<<<<<< HEAD
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PACKETS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_BYTES)))
>>>>>>> refs/remotes/origin/master
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

<<<<<<< HEAD
	id = ip_set_get_byname(nla_data(tb[IPSET_ATTR_NAME]), &s);
	if (id == IPSET_INVALID_ID)
=======
	ret = ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;
	e.id = ip_set_get_byname(map->net, nla_data(tb[IPSET_ATTR_NAME]), &s);
	if (e.id == IPSET_INVALID_ID)
>>>>>>> refs/remotes/origin/master
		return -IPSET_ERR_NAME;
	/* "Loop detection" */
	if (s->type->features & IPSET_TYPE_NAME) {
		ret = -IPSET_ERR_LOOP;
		goto finish;
	}

	if (tb[IPSET_ATTR_CADT_FLAGS]) {
		u32 f = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
<<<<<<< HEAD
		before = f & IPSET_FLAG_BEFORE;
	}

	if (before && !tb[IPSET_ATTR_NAMEREF]) {
=======
		e.before = f & IPSET_FLAG_BEFORE;
	}

	if (e.before && !tb[IPSET_ATTR_NAMEREF]) {
>>>>>>> refs/remotes/origin/master
		ret = -IPSET_ERR_BEFORE;
		goto finish;
	}

	if (tb[IPSET_ATTR_NAMEREF]) {
<<<<<<< HEAD
		refid = ip_set_get_byname(nla_data(tb[IPSET_ATTR_NAMEREF]),
					  &s);
		if (refid == IPSET_INVALID_ID) {
			ret = -IPSET_ERR_NAMEREF;
			goto finish;
		}
		if (!before)
			before = -1;
	}
	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout) {
			ret = -IPSET_ERR_TIMEOUT;
			goto finish;
		}
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}
<<<<<<< HEAD
=======
	if (with_timeout && adt != IPSET_TEST)
		cleanup_entries(map);
>>>>>>> refs/remotes/origin/cm-10.0

	switch (adt) {
	case IPSET_TEST:
		for (i = 0; i < map->size && !ret; i++) {
			elem = list_set_elem(map, i);
			if (elem->id == IPSET_INVALID_ID ||
			    (before != 0 && i + 1 >= map->size))
				break;
			else if (with_timeout && list_set_expired(map, i))
				continue;
			else if (before > 0 && elem->id == id)
<<<<<<< HEAD
				ret = next_id_eq(map, i, refid);
			else if (before < 0 && elem->id == refid)
				ret = next_id_eq(map, i, id);
=======
				ret = id_eq_timeout(map, i + 1, refid);
			else if (before < 0 && elem->id == refid)
				ret = id_eq_timeout(map, i + 1, id);
>>>>>>> refs/remotes/origin/cm-10.0
			else if (before == 0 && elem->id == id)
				ret = 1;
		}
		break;
	case IPSET_ADD:
<<<<<<< HEAD
		for (i = 0; i < map->size && !ret; i++) {
			elem = list_set_elem(map, i);
			if (elem->id == id &&
			    !(with_timeout && list_set_expired(map, i)))
				ret = -IPSET_ERR_EXIST;
		}
		if (ret == -IPSET_ERR_EXIST)
			break;
=======
		for (i = 0; i < map->size; i++) {
			elem = list_set_elem(map, i);
			if (elem->id != id)
				continue;
			if (!(with_timeout && flag_exist)) {
				ret = -IPSET_ERR_EXIST;
				goto finish;
			} else {
				struct set_telem *e = list_set_telem(map, i);

				if ((before > 1 &&
				     !id_eq(map, i + 1, refid)) ||
				    (before < 0 &&
				     (i == 0 || !id_eq(map, i - 1, refid)))) {
					ret = -IPSET_ERR_EXIST;
					goto finish;
				}
				e->timeout = ip_set_timeout_set(timeout);
				ip_set_put_byindex(id);
				ret = 0;
				goto finish;
			}
		}
>>>>>>> refs/remotes/origin/cm-10.0
		ret = -IPSET_ERR_LIST_FULL;
		for (i = 0; i < map->size && ret == -IPSET_ERR_LIST_FULL; i++) {
			elem = list_set_elem(map, i);
			if (elem->id == IPSET_INVALID_ID)
				ret = before != 0 ? -IPSET_ERR_REF_EXIST
					: list_set_add(map, i, id, timeout);
			else if (elem->id != refid)
				continue;
<<<<<<< HEAD
			else if (with_timeout && list_set_expired(map, i))
				ret = -IPSET_ERR_REF_EXIST;
			else if (before)
=======
			else if (before > 0)
>>>>>>> refs/remotes/origin/cm-10.0
				ret = list_set_add(map, i, id, timeout);
			else if (i + 1 < map->size)
				ret = list_set_add(map, i + 1, id, timeout);
		}
		break;
	case IPSET_DEL:
		ret = -IPSET_ERR_EXIST;
		for (i = 0; i < map->size && ret == -IPSET_ERR_EXIST; i++) {
			elem = list_set_elem(map, i);
			if (elem->id == IPSET_INVALID_ID) {
				ret = before != 0 ? -IPSET_ERR_REF_EXIST
						  : -IPSET_ERR_EXIST;
				break;
<<<<<<< HEAD
			} else if (with_timeout && list_set_expired(map, i))
				continue;
			else if (elem->id == id &&
				 (before == 0 ||
				  (before > 0 &&
				   next_id_eq(map, i, refid))))
				ret = list_set_del(map, i);
			else if (before < 0 &&
				 elem->id == refid &&
				 next_id_eq(map, i, id))
=======
			} else if (elem->id == id &&
				   (before == 0 ||
				    (before > 0 && id_eq(map, i + 1, refid))))
				ret = list_set_del(map, i);
			else if (elem->id == refid &&
				 before < 0 && id_eq(map, i + 1, id))
>>>>>>> refs/remotes/origin/cm-10.0
				ret = list_set_del(map, i + 1);
		}
		break;
	default:
		break;
	}

finish:
	if (refid != IPSET_INVALID_ID)
		ip_set_put_byindex(refid);
	if (adt != IPSET_ADD || ret)
		ip_set_put_byindex(id);
=======
		e.refid = ip_set_get_byname(map->net,
					    nla_data(tb[IPSET_ATTR_NAMEREF]),
					    &s);
		if (e.refid == IPSET_INVALID_ID) {
			ret = -IPSET_ERR_NAMEREF;
			goto finish;
		}
		if (!e.before)
			e.before = -1;
	}
	if (adt != IPSET_TEST && SET_WITH_TIMEOUT(set))
		set_cleanup_entries(set);

	ret = adtfn(set, &e, &ext, &ext, flags);

finish:
	if (e.refid != IPSET_INVALID_ID)
		ip_set_put_byindex(map->net, e.refid);
	if (adt != IPSET_ADD || ret)
		ip_set_put_byindex(map->net, e.id);
>>>>>>> refs/remotes/origin/master

	return ip_set_eexist(ret, flags) ? 0 : ret;
}

static void
list_set_flush(struct ip_set *set)
{
	struct list_set *map = set->data;
<<<<<<< HEAD
	struct set_elem *elem;
	u32 i;

	for (i = 0; i < map->size; i++) {
		elem = list_set_elem(map, i);
		if (elem->id != IPSET_INVALID_ID) {
			ip_set_put_byindex(elem->id);
			elem->id = IPSET_INVALID_ID;
=======
	struct set_elem *e;
	u32 i;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(set, map, i);
		if (e->id != IPSET_INVALID_ID) {
			ip_set_put_byindex(map->net, e->id);
			ip_set_ext_destroy(set, e);
			e->id = IPSET_INVALID_ID;
>>>>>>> refs/remotes/origin/master
		}
	}
}

static void
list_set_destroy(struct ip_set *set)
{
	struct list_set *map = set->data;

<<<<<<< HEAD
	if (with_timeout(map->timeout))
=======
	if (SET_WITH_TIMEOUT(set))
>>>>>>> refs/remotes/origin/master
		del_timer_sync(&map->gc);
	list_set_flush(set);
	kfree(map);

	set->data = NULL;
}

static int
list_set_head(struct ip_set *set, struct sk_buff *skb)
{
	const struct list_set *map = set->data;
	struct nlattr *nested;

	nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
	if (!nested)
		goto nla_put_failure;
<<<<<<< HEAD
	NLA_PUT_NET32(skb, IPSET_ATTR_SIZE, htonl(map->size));
	if (with_timeout(map->timeout))
		NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT, htonl(map->timeout));
	NLA_PUT_NET32(skb, IPSET_ATTR_REFERENCES, htonl(set->ref - 1));
	NLA_PUT_NET32(skb, IPSET_ATTR_MEMSIZE,
		      htonl(sizeof(*map) + map->size * map->dsize));
=======
	if (nla_put_net32(skb, IPSET_ATTR_SIZE, htonl(map->size)) ||
	    nla_put_net32(skb, IPSET_ATTR_REFERENCES, htonl(set->ref - 1)) ||
	    nla_put_net32(skb, IPSET_ATTR_MEMSIZE,
			  htonl(sizeof(*map) + map->size * set->dsize)))
		goto nla_put_failure;
	if (unlikely(ip_set_put_flags(skb, set)))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	ipset_nest_end(skb, nested);

	return 0;
nla_put_failure:
	return -EMSGSIZE;
}

static int
list_set_list(const struct ip_set *set,
	      struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct list_set *map = set->data;
	struct nlattr *atd, *nested;
<<<<<<< HEAD
	u32 i, first = cb->args[2];
=======
	u32 i, first = cb->args[IPSET_CB_ARG0];
>>>>>>> refs/remotes/origin/master
	const struct set_elem *e;

	atd = ipset_nest_start(skb, IPSET_ATTR_ADT);
	if (!atd)
		return -EMSGSIZE;
<<<<<<< HEAD
	for (; cb->args[2] < map->size; cb->args[2]++) {
		i = cb->args[2];
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			goto finish;
		if (with_timeout(map->timeout) && list_set_expired(map, i))
=======
	for (; cb->args[IPSET_CB_ARG0] < map->size;
	     cb->args[IPSET_CB_ARG0]++) {
		i = cb->args[IPSET_CB_ARG0];
		e = list_set_elem(set, map, i);
		if (e->id == IPSET_INVALID_ID)
			goto finish;
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
>>>>>>> refs/remotes/origin/master
			continue;
		nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
		if (!nested) {
			if (i == first) {
				nla_nest_cancel(skb, atd);
				return -EMSGSIZE;
			} else
				goto nla_put_failure;
		}
<<<<<<< HEAD
		NLA_PUT_STRING(skb, IPSET_ATTR_NAME,
			       ip_set_name_byindex(e->id));
		if (with_timeout(map->timeout)) {
			const struct set_telem *te =
				(const struct set_telem *) e;
			NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
				      htonl(ip_set_timeout_get(te->timeout)));
		}
=======
		if (nla_put_string(skb, IPSET_ATTR_NAME,
				   ip_set_name_byindex(map->net, e->id)))
			goto nla_put_failure;
		if (ip_set_put_extensions(skb, set, e, true))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
		ipset_nest_end(skb, nested);
	}
finish:
	ipset_nest_end(skb, atd);
	/* Set listing finished */
<<<<<<< HEAD
	cb->args[2] = 0;
=======
	cb->args[IPSET_CB_ARG0] = 0;
>>>>>>> refs/remotes/origin/master
	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nested);
<<<<<<< HEAD
	ipset_nest_end(skb, atd);
	if (unlikely(i == first)) {
		cb->args[2] = 0;
		return -EMSGSIZE;
	}
=======
	if (unlikely(i == first)) {
		cb->args[IPSET_CB_ARG0] = 0;
		return -EMSGSIZE;
	}
	ipset_nest_end(skb, atd);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static bool
list_set_same_set(const struct ip_set *a, const struct ip_set *b)
{
	const struct list_set *x = a->data;
	const struct list_set *y = b->data;

	return x->size == y->size &&
<<<<<<< HEAD
	       x->timeout == y->timeout;
}

static const struct ip_set_type_variant list_set = {
	.kadt	= list_set_kadt,
	.uadt	= list_set_uadt,
=======
	       a->timeout == b->timeout &&
	       a->extensions == b->extensions;
}

static const struct ip_set_type_variant set_variant = {
	.kadt	= list_set_kadt,
	.uadt	= list_set_uadt,
	.adt	= {
		[IPSET_ADD] = list_set_uadd,
		[IPSET_DEL] = list_set_udel,
		[IPSET_TEST] = list_set_utest,
	},
>>>>>>> refs/remotes/origin/master
	.destroy = list_set_destroy,
	.flush	= list_set_flush,
	.head	= list_set_head,
	.list	= list_set_list,
	.same_set = list_set_same_set,
};

static void
list_set_gc(unsigned long ul_set)
{
	struct ip_set *set = (struct ip_set *) ul_set;
	struct list_set *map = set->data;
<<<<<<< HEAD
<<<<<<< HEAD
	struct set_telem *e;
	u32 i;

	write_lock_bh(&set->lock);
	for (i = 0; i < map->size; i++) {
		e = list_set_telem(map, i);
		if (e->id != IPSET_INVALID_ID && list_set_expired(map, i))
			list_set_del(map, i);
	}
=======

	write_lock_bh(&set->lock);
	cleanup_entries(map);
>>>>>>> refs/remotes/origin/cm-10.0
	write_unlock_bh(&set->lock);

	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
=======

	write_lock_bh(&set->lock);
	set_cleanup_entries(set);
	write_unlock_bh(&set->lock);

	map->gc.expires = jiffies + IPSET_GC_PERIOD(set->timeout) * HZ;
>>>>>>> refs/remotes/origin/master
	add_timer(&map->gc);
}

static void
<<<<<<< HEAD
list_set_gc_init(struct ip_set *set)
=======
list_set_gc_init(struct ip_set *set, void (*gc)(unsigned long ul_set))
>>>>>>> refs/remotes/origin/master
{
	struct list_set *map = set->data;

	init_timer(&map->gc);
	map->gc.data = (unsigned long) set;
<<<<<<< HEAD
	map->gc.function = list_set_gc;
	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
=======
	map->gc.function = gc;
	map->gc.expires = jiffies + IPSET_GC_PERIOD(set->timeout) * HZ;
>>>>>>> refs/remotes/origin/master
	add_timer(&map->gc);
}

/* Create list:set type of sets */

static bool
<<<<<<< HEAD
init_list_set(struct ip_set *set, u32 size, size_t dsize,
	      unsigned long timeout)
=======
init_list_set(struct net *net, struct ip_set *set, u32 size)
>>>>>>> refs/remotes/origin/master
{
	struct list_set *map;
	struct set_elem *e;
	u32 i;

<<<<<<< HEAD
	map = kzalloc(sizeof(*map) + size * dsize, GFP_KERNEL);
=======
	map = kzalloc(sizeof(*map) + size * set->dsize, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!map)
		return false;

	map->size = size;
<<<<<<< HEAD
	map->dsize = dsize;
	map->timeout = timeout;
	set->data = map;

	for (i = 0; i < size; i++) {
		e = list_set_elem(map, i);
=======
	map->net = net;
	set->data = map;

	for (i = 0; i < size; i++) {
		e = list_set_elem(set, map, i);
>>>>>>> refs/remotes/origin/master
		e->id = IPSET_INVALID_ID;
	}

	return true;
}

static int
<<<<<<< HEAD
list_set_create(struct ip_set *set, struct nlattr *tb[], u32 flags)
=======
list_set_create(struct net *net, struct ip_set *set, struct nlattr *tb[],
		u32 flags)
>>>>>>> refs/remotes/origin/master
{
	u32 size = IP_SET_LIST_DEFAULT_SIZE;

	if (unlikely(!ip_set_optattr_netorder(tb, IPSET_ATTR_SIZE) ||
<<<<<<< HEAD
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
>>>>>>> refs/remotes/origin/master
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_SIZE])
		size = ip_set_get_h32(tb[IPSET_ATTR_SIZE]);
	if (size < IP_SET_LIST_MIN_SIZE)
		size = IP_SET_LIST_MIN_SIZE;

<<<<<<< HEAD
	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!init_list_set(set, size, sizeof(struct set_telem),
				   ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT])))
			return -ENOMEM;

		list_set_gc_init(set);
	} else {
		if (!init_list_set(set, size, sizeof(struct set_elem),
				   IPSET_NO_TIMEOUT))
			return -ENOMEM;
	}
	set->variant = &list_set;
=======
	set->variant = &set_variant;
	set->dsize = ip_set_elem_len(set, tb, sizeof(struct set_elem));
	if (!init_list_set(net, set, size))
		return -ENOMEM;
	if (tb[IPSET_ATTR_TIMEOUT]) {
		set->timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
		list_set_gc_init(set, list_set_gc);
	}
>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct ip_set_type list_set_type __read_mostly = {
	.name		= "list:set",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_NAME | IPSET_DUMP_LAST,
	.dimension	= IPSET_DIM_ONE,
<<<<<<< HEAD
<<<<<<< HEAD
	.family		= AF_UNSPEC,
	.revision	= 0,
=======
	.family		= NFPROTO_UNSPEC,
	.revision_min	= 0,
	.revision_max	= 0,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.family		= NFPROTO_UNSPEC,
	.revision_min	= IPSET_TYPE_REV_MIN,
	.revision_max	= IPSET_TYPE_REV_MAX,
>>>>>>> refs/remotes/origin/master
	.create		= list_set_create,
	.create_policy	= {
		[IPSET_ATTR_SIZE]	= { .type = NLA_U32 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
<<<<<<< HEAD
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
>>>>>>> refs/remotes/origin/master
	},
	.adt_policy	= {
		[IPSET_ATTR_NAME]	= { .type = NLA_STRING,
					    .len = IPSET_MAXNAMELEN },
		[IPSET_ATTR_NAMEREF]	= { .type = NLA_STRING,
					    .len = IPSET_MAXNAMELEN },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_LINENO]	= { .type = NLA_U32 },
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
<<<<<<< HEAD
=======
		[IPSET_ATTR_BYTES]	= { .type = NLA_U64 },
		[IPSET_ATTR_PACKETS]	= { .type = NLA_U64 },
		[IPSET_ATTR_COMMENT]	= { .type = NLA_NUL_STRING },
>>>>>>> refs/remotes/origin/master
	},
	.me		= THIS_MODULE,
};

static int __init
list_set_init(void)
{
	return ip_set_type_register(&list_set_type);
}

static void __exit
list_set_fini(void)
{
	ip_set_type_unregister(&list_set_type);
}

module_init(list_set_init);
module_exit(list_set_fini);
