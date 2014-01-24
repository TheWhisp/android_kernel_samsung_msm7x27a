/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#include <linux/highmem.h>
<<<<<<< HEAD

/* this is some deeply nasty code.  ctree.h has a different
 * definition for this BTRFS_SETGET_FUNCS macro, behind a #ifndef
 *
 * The end result is that anyone who #includes ctree.h gets a
 * declaration for the btrfs_set_foo functions and btrfs_foo functions
 *
 * This file declares the macros and then #includes ctree.h, which results
 * in cpp creating the function here based on the template below.
=======
#include <asm/unaligned.h>

#include "ctree.h"

static inline u8 get_unaligned_le8(const void *p)
{
       return *(u8 *)p;
}

static inline void put_unaligned_le8(u8 val, void *p)
{
       *(u8 *)p = val;
}

/*
 * this is some deeply nasty code.
 *
 * The end result is that anyone who #includes ctree.h gets a
 * declaration for the btrfs_set_foo functions and btrfs_foo functions,
 * which are wappers of btrfs_set_token_#bits functions and
 * btrfs_get_token_#bits functions, which are defined in this file.
>>>>>>> refs/remotes/origin/master
 *
 * These setget functions do all the extent_buffer related mapping
 * required to efficiently read and write specific fields in the extent
 * buffers.  Every pointer to metadata items in btrfs is really just
 * an unsigned long offset into the extent buffer which has been
 * cast to a specific type.  This gives us all the gcc type checking.
 *
<<<<<<< HEAD
 * The extent buffer api is used to do all the kmapping and page
 * spanning work required to get extent buffers in highmem and have
 * a metadata blocksize different from the page size.
 *
 * The macro starts with a simple function prototype declaration so that
 * sparse won't complain about it being static.
 */

#define BTRFS_SETGET_FUNCS(name, type, member, bits)			\
u##bits btrfs_##name(struct extent_buffer *eb, type *s);		\
void btrfs_set_##name(struct extent_buffer *eb, type *s, u##bits val);	\
<<<<<<< HEAD
u##bits btrfs_##name(struct extent_buffer *eb,				\
				   type *s)				\
=======
void btrfs_set_token_##name(struct extent_buffer *eb, type *s, u##bits val, struct btrfs_map_token *token);	\
u##bits btrfs_token_##name(struct extent_buffer *eb,				\
			   type *s, struct btrfs_map_token *token)	\
>>>>>>> refs/remotes/origin/cm-10.0
{									\
	unsigned long part_offset = (unsigned long)s;			\
	unsigned long offset = part_offset + offsetof(type, member);	\
	type *p;							\
<<<<<<< HEAD
	/* ugly, but we want the fast path here */			\
	if (eb->map_token && offset >= eb->map_start &&			\
	    offset + sizeof(((type *)0)->member) <= eb->map_start +	\
	    eb->map_len) {						\
		p = (type *)(eb->kaddr + part_offset - eb->map_start);	\
		return le##bits##_to_cpu(p->member);			\
	}								\
	{								\
		int err;						\
		char *map_token;					\
		char *kaddr;						\
		int unmap_on_exit = (eb->map_token == NULL);		\
		unsigned long map_start;				\
		unsigned long map_len;					\
		u##bits res;						\
		err = map_extent_buffer(eb, offset,			\
				sizeof(((type *)0)->member),		\
				&map_token, &kaddr,			\
				&map_start, &map_len, KM_USER1);	\
		if (err) {						\
			__le##bits leres;				\
			read_eb_member(eb, s, type, member, &leres);	\
			return le##bits##_to_cpu(leres);		\
		}							\
		p = (type *)(kaddr + part_offset - map_start);		\
		res = le##bits##_to_cpu(p->member);			\
		if (unmap_on_exit)					\
			unmap_extent_buffer(eb, map_token, KM_USER1);	\
		return res;						\
	}								\
}									\
void btrfs_set_##name(struct extent_buffer *eb,				\
				    type *s, u##bits val)		\
=======
	int err;						\
	char *kaddr;						\
	unsigned long map_start;				\
	unsigned long map_len;					\
	unsigned long mem_len = sizeof(((type *)0)->member);	\
	u##bits res;						\
	if (token && token->kaddr && token->offset <= offset &&	\
	    token->eb == eb &&					\
	   (token->offset + PAGE_CACHE_SIZE >= offset + mem_len)) { \
		kaddr = token->kaddr;				\
		p = (type *)(kaddr + part_offset - token->offset);  \
		res = le##bits##_to_cpu(p->member);		\
		return res;					\
	}							\
	err = map_private_extent_buffer(eb, offset,		\
			mem_len,				\
			&kaddr, &map_start, &map_len);		\
	if (err) {						\
		__le##bits leres;				\
		read_eb_member(eb, s, type, member, &leres);	\
		return le##bits##_to_cpu(leres);		\
	}							\
	p = (type *)(kaddr + part_offset - map_start);		\
	res = le##bits##_to_cpu(p->member);			\
	if (token) {						\
		token->kaddr = kaddr;				\
		token->offset = map_start;			\
		token->eb = eb;					\
	}							\
	return res;						\
}									\
void btrfs_set_token_##name(struct extent_buffer *eb,				\
			    type *s, u##bits val, struct btrfs_map_token *token)		\
>>>>>>> refs/remotes/origin/cm-10.0
{									\
	unsigned long part_offset = (unsigned long)s;			\
	unsigned long offset = part_offset + offsetof(type, member);	\
	type *p;							\
<<<<<<< HEAD
	/* ugly, but we want the fast path here */			\
	if (eb->map_token && offset >= eb->map_start &&			\
	    offset + sizeof(((type *)0)->member) <= eb->map_start +	\
	    eb->map_len) {						\
		p = (type *)(eb->kaddr + part_offset - eb->map_start);	\
		p->member = cpu_to_le##bits(val);			\
		return;							\
	}								\
	{								\
		int err;						\
		char *map_token;					\
		char *kaddr;						\
		int unmap_on_exit = (eb->map_token == NULL);		\
		unsigned long map_start;				\
		unsigned long map_len;					\
		err = map_extent_buffer(eb, offset,			\
				sizeof(((type *)0)->member),		\
				&map_token, &kaddr,			\
				&map_start, &map_len, KM_USER1);	\
		if (err) {						\
			__le##bits val2;				\
			val2 = cpu_to_le##bits(val);			\
			write_eb_member(eb, s, type, member, &val2);	\
			return;						\
		}							\
		p = (type *)(kaddr + part_offset - map_start);		\
		p->member = cpu_to_le##bits(val);			\
		if (unmap_on_exit)					\
			unmap_extent_buffer(eb, map_token, KM_USER1);	\
	}								\
}
=======
	int err;						\
	char *kaddr;						\
	unsigned long map_start;				\
	unsigned long map_len;					\
	unsigned long mem_len = sizeof(((type *)0)->member);	\
	if (token && token->kaddr && token->offset <= offset &&	\
	    token->eb == eb &&					\
	   (token->offset + PAGE_CACHE_SIZE >= offset + mem_len)) { \
		kaddr = token->kaddr;				\
		p = (type *)(kaddr + part_offset - token->offset);  \
		p->member = cpu_to_le##bits(val);		\
		return;						\
	}							\
	err = map_private_extent_buffer(eb, offset,		\
			mem_len,				\
			&kaddr, &map_start, &map_len);		\
	if (err) {						\
		__le##bits val2;				\
		val2 = cpu_to_le##bits(val);			\
		write_eb_member(eb, s, type, member, &val2);	\
		return;						\
	}							\
	p = (type *)(kaddr + part_offset - map_start);		\
	p->member = cpu_to_le##bits(val);			\
	if (token) {						\
		token->kaddr = kaddr;				\
		token->offset = map_start;			\
		token->eb = eb;					\
	}							\
}								\
void btrfs_set_##name(struct extent_buffer *eb,			\
		      type *s, u##bits val)			\
{								\
	btrfs_set_token_##name(eb, s, val, NULL);		\
}								\
u##bits btrfs_##name(struct extent_buffer *eb,			\
		      type *s)					\
{								\
	return btrfs_token_##name(eb, s, NULL);			\
}								\
>>>>>>> refs/remotes/origin/cm-10.0

#include "ctree.h"
=======
 * The extent buffer api is used to do the page spanning work required to
 * have a metadata blocksize different from the page size.
 */

#define DEFINE_BTRFS_SETGET_BITS(bits)					\
u##bits btrfs_get_token_##bits(struct extent_buffer *eb, void *ptr,	\
			       unsigned long off,			\
			       struct btrfs_map_token *token)		\
{									\
	unsigned long part_offset = (unsigned long)ptr;			\
	unsigned long offset = part_offset + off;			\
	void *p;							\
	int err;							\
	char *kaddr;							\
	unsigned long map_start;					\
	unsigned long map_len;						\
	int size = sizeof(u##bits);					\
	u##bits res;							\
									\
	if (token && token->kaddr && token->offset <= offset &&		\
	    token->eb == eb &&						\
	   (token->offset + PAGE_CACHE_SIZE >= offset + size)) {	\
		kaddr = token->kaddr;					\
		p = kaddr + part_offset - token->offset;		\
		res = get_unaligned_le##bits(p + off);			\
		return res;						\
	}								\
	err = map_private_extent_buffer(eb, offset, size,		\
					&kaddr, &map_start, &map_len);	\
	if (err) {							\
		__le##bits leres;					\
									\
		read_extent_buffer(eb, &leres, offset, size);		\
		return le##bits##_to_cpu(leres);			\
	}								\
	p = kaddr + part_offset - map_start;				\
	res = get_unaligned_le##bits(p + off);				\
	if (token) {							\
		token->kaddr = kaddr;					\
		token->offset = map_start;				\
		token->eb = eb;						\
	}								\
	return res;							\
}									\
void btrfs_set_token_##bits(struct extent_buffer *eb,			\
			    void *ptr, unsigned long off, u##bits val,	\
			    struct btrfs_map_token *token)		\
{									\
	unsigned long part_offset = (unsigned long)ptr;			\
	unsigned long offset = part_offset + off;			\
	void *p;							\
	int err;							\
	char *kaddr;							\
	unsigned long map_start;					\
	unsigned long map_len;						\
	int size = sizeof(u##bits);					\
									\
	if (token && token->kaddr && token->offset <= offset &&		\
	    token->eb == eb &&						\
	   (token->offset + PAGE_CACHE_SIZE >= offset + size)) {	\
		kaddr = token->kaddr;					\
		p = kaddr + part_offset - token->offset;		\
		put_unaligned_le##bits(val, p + off);			\
		return;							\
	}								\
	err = map_private_extent_buffer(eb, offset, size,		\
			&kaddr, &map_start, &map_len);			\
	if (err) {							\
		__le##bits val2;					\
									\
		val2 = cpu_to_le##bits(val);				\
		write_extent_buffer(eb, &val2, offset, size);		\
		return;							\
	}								\
	p = kaddr + part_offset - map_start;				\
	put_unaligned_le##bits(val, p + off);				\
	if (token) {							\
		token->kaddr = kaddr;					\
		token->offset = map_start;				\
		token->eb = eb;						\
	}								\
}

DEFINE_BTRFS_SETGET_BITS(8)
DEFINE_BTRFS_SETGET_BITS(16)
DEFINE_BTRFS_SETGET_BITS(32)
DEFINE_BTRFS_SETGET_BITS(64)
>>>>>>> refs/remotes/origin/master

void btrfs_node_key(struct extent_buffer *eb,
		    struct btrfs_disk_key *disk_key, int nr)
{
	unsigned long ptr = btrfs_node_key_ptr_offset(nr);
<<<<<<< HEAD
<<<<<<< HEAD
	if (eb->map_token && ptr >= eb->map_start &&
	    ptr + sizeof(*disk_key) <= eb->map_start + eb->map_len) {
		memcpy(disk_key, eb->kaddr + ptr - eb->map_start,
			sizeof(*disk_key));
		return;
	} else if (eb->map_token) {
		unmap_extent_buffer(eb, eb->map_token, KM_USER1);
		eb->map_token = NULL;
	}
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	read_eb_member(eb, (struct btrfs_key_ptr *)ptr,
		       struct btrfs_key_ptr, key, disk_key);
}
