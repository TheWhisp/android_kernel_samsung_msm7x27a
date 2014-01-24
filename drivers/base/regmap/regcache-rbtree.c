/*
 * Register cache access API - rbtree caching support
 *
 * Copyright 2011 Wolfson Microelectronics plc
 *
 * Author: Dimitris Papastamos <dp@opensource.wolfsonmicro.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/slab.h>
#include <linux/device.h>
#include <linux/debugfs.h>
#include <linux/rbtree.h>
#include <linux/seq_file.h>

#include "internal.h"

static int regcache_rbtree_write(struct regmap *map, unsigned int reg,
				 unsigned int value);
static int regcache_rbtree_exit(struct regmap *map);

struct regcache_rbtree_node {
	/* the actual rbtree node holding this block */
	struct rb_node node;
	/* base register handled by this block */
	unsigned int base_reg;
	/* block of adjacent registers */
	void *block;
<<<<<<< HEAD
=======
	/* Which registers are present */
	long *cache_present;
>>>>>>> refs/remotes/origin/master
	/* number of registers available in the block */
	unsigned int blklen;
} __attribute__ ((packed));

struct regcache_rbtree_ctx {
	struct rb_root root;
	struct regcache_rbtree_node *cached_rbnode;
};

static inline void regcache_rbtree_get_base_top_reg(
<<<<<<< HEAD
=======
	struct regmap *map,
>>>>>>> refs/remotes/origin/master
	struct regcache_rbtree_node *rbnode,
	unsigned int *base, unsigned int *top)
{
	*base = rbnode->base_reg;
<<<<<<< HEAD
	*top = rbnode->base_reg + rbnode->blklen - 1;
}

static unsigned int regcache_rbtree_get_register(
	struct regcache_rbtree_node *rbnode, unsigned int idx,
	unsigned int word_size)
{
	return regcache_get_val(rbnode->block, idx, word_size);
}

static void regcache_rbtree_set_register(struct regcache_rbtree_node *rbnode,
					 unsigned int idx, unsigned int val,
					 unsigned int word_size)
{
	regcache_set_val(rbnode->block, idx, val, word_size);
}

static struct regcache_rbtree_node *regcache_rbtree_lookup(struct regmap *map,
	unsigned int reg)
=======
	*top = rbnode->base_reg + ((rbnode->blklen - 1) * map->reg_stride);
}

static unsigned int regcache_rbtree_get_register(struct regmap *map,
	struct regcache_rbtree_node *rbnode, unsigned int idx)
{
	return regcache_get_val(map, rbnode->block, idx);
}

static void regcache_rbtree_set_register(struct regmap *map,
					 struct regcache_rbtree_node *rbnode,
					 unsigned int idx, unsigned int val)
{
	set_bit(idx, rbnode->cache_present);
	regcache_set_val(map, rbnode->block, idx, val);
}

static struct regcache_rbtree_node *regcache_rbtree_lookup(struct regmap *map,
							   unsigned int reg)
>>>>>>> refs/remotes/origin/master
{
	struct regcache_rbtree_ctx *rbtree_ctx = map->cache;
	struct rb_node *node;
	struct regcache_rbtree_node *rbnode;
	unsigned int base_reg, top_reg;

	rbnode = rbtree_ctx->cached_rbnode;
	if (rbnode) {
<<<<<<< HEAD
		regcache_rbtree_get_base_top_reg(rbnode, &base_reg, &top_reg);
=======
		regcache_rbtree_get_base_top_reg(map, rbnode, &base_reg,
						 &top_reg);
>>>>>>> refs/remotes/origin/master
		if (reg >= base_reg && reg <= top_reg)
			return rbnode;
	}

	node = rbtree_ctx->root.rb_node;
	while (node) {
		rbnode = container_of(node, struct regcache_rbtree_node, node);
<<<<<<< HEAD
		regcache_rbtree_get_base_top_reg(rbnode, &base_reg, &top_reg);
=======
		regcache_rbtree_get_base_top_reg(map, rbnode, &base_reg,
						 &top_reg);
>>>>>>> refs/remotes/origin/master
		if (reg >= base_reg && reg <= top_reg) {
			rbtree_ctx->cached_rbnode = rbnode;
			return rbnode;
		} else if (reg > top_reg) {
			node = node->rb_right;
		} else if (reg < base_reg) {
			node = node->rb_left;
		}
	}

	return NULL;
}

<<<<<<< HEAD
static int regcache_rbtree_insert(struct rb_root *root,
=======
static int regcache_rbtree_insert(struct regmap *map, struct rb_root *root,
>>>>>>> refs/remotes/origin/master
				  struct regcache_rbtree_node *rbnode)
{
	struct rb_node **new, *parent;
	struct regcache_rbtree_node *rbnode_tmp;
	unsigned int base_reg_tmp, top_reg_tmp;
	unsigned int base_reg;

	parent = NULL;
	new = &root->rb_node;
	while (*new) {
		rbnode_tmp = container_of(*new, struct regcache_rbtree_node,
					  node);
		/* base and top registers of the current rbnode */
<<<<<<< HEAD
		regcache_rbtree_get_base_top_reg(rbnode_tmp, &base_reg_tmp,
=======
		regcache_rbtree_get_base_top_reg(map, rbnode_tmp, &base_reg_tmp,
>>>>>>> refs/remotes/origin/master
						 &top_reg_tmp);
		/* base register of the rbnode to be added */
		base_reg = rbnode->base_reg;
		parent = *new;
		/* if this register has already been inserted, just return */
		if (base_reg >= base_reg_tmp &&
		    base_reg <= top_reg_tmp)
			return 0;
		else if (base_reg > top_reg_tmp)
			new = &((*new)->rb_right);
		else if (base_reg < base_reg_tmp)
			new = &((*new)->rb_left);
	}

	/* insert the node into the rbtree */
	rb_link_node(&rbnode->node, parent, new);
	rb_insert_color(&rbnode->node, root);

	return 1;
}

#ifdef CONFIG_DEBUG_FS
static int rbtree_show(struct seq_file *s, void *ignored)
{
	struct regmap *map = s->private;
	struct regcache_rbtree_ctx *rbtree_ctx = map->cache;
	struct regcache_rbtree_node *n;
	struct rb_node *node;
	unsigned int base, top;
<<<<<<< HEAD
	int nodes = 0;
	int registers = 0;
	int average;

	mutex_lock(&map->lock);
=======
	size_t mem_size;
	int nodes = 0;
	int registers = 0;
	int this_registers, average;

	map->lock(map->lock_arg);

	mem_size = sizeof(*rbtree_ctx);
>>>>>>> refs/remotes/origin/master

	for (node = rb_first(&rbtree_ctx->root); node != NULL;
	     node = rb_next(node)) {
		n = container_of(node, struct regcache_rbtree_node, node);
<<<<<<< HEAD

		regcache_rbtree_get_base_top_reg(n, &base, &top);
		seq_printf(s, "%x-%x (%d)\n", base, top, top - base + 1);

		nodes++;
		registers += top - base + 1;
=======
		mem_size += sizeof(*n);
		mem_size += (n->blklen * map->cache_word_size);
		mem_size += BITS_TO_LONGS(n->blklen) * sizeof(long);

		regcache_rbtree_get_base_top_reg(map, n, &base, &top);
		this_registers = ((top - base) / map->reg_stride) + 1;
		seq_printf(s, "%x-%x (%d)\n", base, top, this_registers);

		nodes++;
		registers += this_registers;
>>>>>>> refs/remotes/origin/master
	}

	if (nodes)
		average = registers / nodes;
	else
		average = 0;

<<<<<<< HEAD
	seq_printf(s, "%d nodes, %d registers, average %d registers\n",
		   nodes, registers, average);

	mutex_unlock(&map->lock);
=======
	seq_printf(s, "%d nodes, %d registers, average %d registers, used %zu bytes\n",
		   nodes, registers, average, mem_size);

	map->unlock(map->lock_arg);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int rbtree_open(struct inode *inode, struct file *file)
{
	return single_open(file, rbtree_show, inode->i_private);
}

static const struct file_operations rbtree_fops = {
	.open		= rbtree_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static void rbtree_debugfs_init(struct regmap *map)
{
	debugfs_create_file("rbtree", 0400, map->debugfs, map, &rbtree_fops);
}
#else
static void rbtree_debugfs_init(struct regmap *map)
{
}
#endif

static int regcache_rbtree_init(struct regmap *map)
{
	struct regcache_rbtree_ctx *rbtree_ctx;
	int i;
	int ret;

	map->cache = kmalloc(sizeof *rbtree_ctx, GFP_KERNEL);
	if (!map->cache)
		return -ENOMEM;

	rbtree_ctx = map->cache;
	rbtree_ctx->root = RB_ROOT;
	rbtree_ctx->cached_rbnode = NULL;

	for (i = 0; i < map->num_reg_defaults; i++) {
		ret = regcache_rbtree_write(map,
					    map->reg_defaults[i].reg,
					    map->reg_defaults[i].def);
		if (ret)
			goto err;
	}

	rbtree_debugfs_init(map);

	return 0;

err:
	regcache_rbtree_exit(map);
	return ret;
}

static int regcache_rbtree_exit(struct regmap *map)
{
	struct rb_node *next;
	struct regcache_rbtree_ctx *rbtree_ctx;
	struct regcache_rbtree_node *rbtree_node;

	/* if we've already been called then just return */
	rbtree_ctx = map->cache;
	if (!rbtree_ctx)
		return 0;

	/* free up the rbtree */
	next = rb_first(&rbtree_ctx->root);
	while (next) {
		rbtree_node = rb_entry(next, struct regcache_rbtree_node, node);
		next = rb_next(&rbtree_node->node);
		rb_erase(&rbtree_node->node, &rbtree_ctx->root);
<<<<<<< HEAD
=======
		kfree(rbtree_node->cache_present);
>>>>>>> refs/remotes/origin/master
		kfree(rbtree_node->block);
		kfree(rbtree_node);
	}

	/* release the resources */
	kfree(map->cache);
	map->cache = NULL;

	return 0;
}

static int regcache_rbtree_read(struct regmap *map,
				unsigned int reg, unsigned int *value)
{
	struct regcache_rbtree_node *rbnode;
	unsigned int reg_tmp;

	rbnode = regcache_rbtree_lookup(map, reg);
	if (rbnode) {
<<<<<<< HEAD
		reg_tmp = reg - rbnode->base_reg;
		*value = regcache_rbtree_get_register(rbnode, reg_tmp,
						      map->cache_word_size);
=======
		reg_tmp = (reg - rbnode->base_reg) / map->reg_stride;
		if (!test_bit(reg_tmp, rbnode->cache_present))
			return -ENOENT;
		*value = regcache_rbtree_get_register(map, rbnode, reg_tmp);
>>>>>>> refs/remotes/origin/master
	} else {
		return -ENOENT;
	}

	return 0;
}


<<<<<<< HEAD
static int regcache_rbtree_insert_to_block(struct regcache_rbtree_node *rbnode,
					   unsigned int pos, unsigned int reg,
					   unsigned int value, unsigned int word_size)
{
	u8 *blk;

	blk = krealloc(rbnode->block,
		       (rbnode->blklen + 1) * word_size, GFP_KERNEL);
	if (!blk)
		return -ENOMEM;

	/* insert the register value in the correct place in the rbnode block */
	memmove(blk + (pos + 1) * word_size,
		blk + pos * word_size,
		(rbnode->blklen - pos) * word_size);

	/* update the rbnode block, its size and the base register */
	rbnode->block = blk;
	rbnode->blklen++;
	if (!pos)
		rbnode->base_reg = reg;

	regcache_rbtree_set_register(rbnode, pos, value, word_size);
	return 0;
}

=======
static int regcache_rbtree_insert_to_block(struct regmap *map,
					   struct regcache_rbtree_node *rbnode,
					   unsigned int base_reg,
					   unsigned int top_reg,
					   unsigned int reg,
					   unsigned int value)
{
	unsigned int blklen;
	unsigned int pos, offset;
	unsigned long *present;
	u8 *blk;

	blklen = (top_reg - base_reg) / map->reg_stride + 1;
	pos = (reg - base_reg) / map->reg_stride;
	offset = (rbnode->base_reg - base_reg) / map->reg_stride;

	blk = krealloc(rbnode->block,
		       blklen * map->cache_word_size,
		       GFP_KERNEL);
	if (!blk)
		return -ENOMEM;

	present = krealloc(rbnode->cache_present,
		    BITS_TO_LONGS(blklen) * sizeof(*present), GFP_KERNEL);
	if (!present) {
		kfree(blk);
		return -ENOMEM;
	}

	/* insert the register value in the correct place in the rbnode block */
	if (pos == 0) {
		memmove(blk + offset * map->cache_word_size,
			blk, rbnode->blklen * map->cache_word_size);
		bitmap_shift_right(present, present, offset, blklen);
	}

	/* update the rbnode block, its size and the base register */
	rbnode->block = blk;
	rbnode->blklen = blklen;
	rbnode->base_reg = base_reg;
	rbnode->cache_present = present;

	regcache_rbtree_set_register(map, rbnode, pos, value);
	return 0;
}

static struct regcache_rbtree_node *
regcache_rbtree_node_alloc(struct regmap *map, unsigned int reg)
{
	struct regcache_rbtree_node *rbnode;
	const struct regmap_range *range;
	int i;

	rbnode = kzalloc(sizeof(*rbnode), GFP_KERNEL);
	if (!rbnode)
		return NULL;

	/* If there is a read table then use it to guess at an allocation */
	if (map->rd_table) {
		for (i = 0; i < map->rd_table->n_yes_ranges; i++) {
			if (regmap_reg_in_range(reg,
						&map->rd_table->yes_ranges[i]))
				break;
		}

		if (i != map->rd_table->n_yes_ranges) {
			range = &map->rd_table->yes_ranges[i];
			rbnode->blklen = (range->range_max - range->range_min) /
				map->reg_stride	+ 1;
			rbnode->base_reg = range->range_min;
		}
	}

	if (!rbnode->blklen) {
		rbnode->blklen = 1;
		rbnode->base_reg = reg;
	}

	rbnode->block = kmalloc(rbnode->blklen * map->cache_word_size,
				GFP_KERNEL);
	if (!rbnode->block)
		goto err_free;

	rbnode->cache_present = kzalloc(BITS_TO_LONGS(rbnode->blklen) *
		sizeof(*rbnode->cache_present), GFP_KERNEL);
	if (!rbnode->cache_present)
		goto err_free_block;

	return rbnode;

err_free_block:
	kfree(rbnode->block);
err_free:
	kfree(rbnode);
	return NULL;
}

>>>>>>> refs/remotes/origin/master
static int regcache_rbtree_write(struct regmap *map, unsigned int reg,
				 unsigned int value)
{
	struct regcache_rbtree_ctx *rbtree_ctx;
	struct regcache_rbtree_node *rbnode, *rbnode_tmp;
	struct rb_node *node;
<<<<<<< HEAD
	unsigned int val;
	unsigned int reg_tmp;
	unsigned int pos;
	int i;
	int ret;

	rbtree_ctx = map->cache;
=======
	unsigned int reg_tmp;
	int ret;

	rbtree_ctx = map->cache;

>>>>>>> refs/remotes/origin/master
	/* if we can't locate it in the cached rbnode we'll have
	 * to traverse the rbtree looking for it.
	 */
	rbnode = regcache_rbtree_lookup(map, reg);
	if (rbnode) {
<<<<<<< HEAD
		reg_tmp = reg - rbnode->base_reg;
		val = regcache_rbtree_get_register(rbnode, reg_tmp,
						   map->cache_word_size);
		if (val == value)
			return 0;
		regcache_rbtree_set_register(rbnode, reg_tmp, value,
					     map->cache_word_size);
	} else {
		/* look for an adjacent register to the one we are about to add */
		for (node = rb_first(&rbtree_ctx->root); node;
		     node = rb_next(node)) {
			rbnode_tmp = rb_entry(node, struct regcache_rbtree_node, node);
			for (i = 0; i < rbnode_tmp->blklen; i++) {
				reg_tmp = rbnode_tmp->base_reg + i;
				if (abs(reg_tmp - reg) != 1)
					continue;
				/* decide where in the block to place our register */
				if (reg_tmp + 1 == reg)
					pos = i + 1;
				else
					pos = i;
				ret = regcache_rbtree_insert_to_block(rbnode_tmp, pos,
								      reg, value,
								      map->cache_word_size);
				if (ret)
					return ret;
				rbtree_ctx->cached_rbnode = rbnode_tmp;
				return 0;
			}
		}
		/* we did not manage to find a place to insert it in an existing
		 * block so create a new rbnode with a single register in its block.
		 * This block will get populated further if any other adjacent
		 * registers get modified in the future.
		 */
		rbnode = kzalloc(sizeof *rbnode, GFP_KERNEL);
		if (!rbnode)
			return -ENOMEM;
		rbnode->blklen = 1;
		rbnode->base_reg = reg;
		rbnode->block = kmalloc(rbnode->blklen * map->cache_word_size,
					GFP_KERNEL);
		if (!rbnode->block) {
			kfree(rbnode);
			return -ENOMEM;
		}
		regcache_rbtree_set_register(rbnode, 0, value, map->cache_word_size);
		regcache_rbtree_insert(&rbtree_ctx->root, rbnode);
=======
		reg_tmp = (reg - rbnode->base_reg) / map->reg_stride;
		regcache_rbtree_set_register(map, rbnode, reg_tmp, value);
	} else {
		unsigned int base_reg, top_reg;
		unsigned int new_base_reg, new_top_reg;
		unsigned int min, max;
		unsigned int max_dist;

		max_dist = map->reg_stride * sizeof(*rbnode_tmp) /
			map->cache_word_size;
		if (reg < max_dist)
			min = 0;
		else
			min = reg - max_dist;
		max = reg + max_dist;

		/* look for an adjacent register to the one we are about to add */
		for (node = rb_first(&rbtree_ctx->root); node;
		     node = rb_next(node)) {
			rbnode_tmp = rb_entry(node, struct regcache_rbtree_node,
					      node);

			regcache_rbtree_get_base_top_reg(map, rbnode_tmp,
				&base_reg, &top_reg);

			if (base_reg <= max && top_reg >= min) {
				new_base_reg = min(reg, base_reg);
				new_top_reg = max(reg, top_reg);
			} else {
				continue;
			}

			ret = regcache_rbtree_insert_to_block(map, rbnode_tmp,
							      new_base_reg,
							      new_top_reg, reg,
							      value);
			if (ret)
				return ret;
			rbtree_ctx->cached_rbnode = rbnode_tmp;
			return 0;
		}

		/* We did not manage to find a place to insert it in
		 * an existing block so create a new rbnode.
		 */
		rbnode = regcache_rbtree_node_alloc(map, reg);
		if (!rbnode)
			return -ENOMEM;
		regcache_rbtree_set_register(map, rbnode,
					     reg - rbnode->base_reg, value);
		regcache_rbtree_insert(map, &rbtree_ctx->root, rbnode);
>>>>>>> refs/remotes/origin/master
		rbtree_ctx->cached_rbnode = rbnode;
	}

	return 0;
}

static int regcache_rbtree_sync(struct regmap *map, unsigned int min,
				unsigned int max)
{
	struct regcache_rbtree_ctx *rbtree_ctx;
	struct rb_node *node;
	struct regcache_rbtree_node *rbnode;
<<<<<<< HEAD
	unsigned int regtmp;
	unsigned int val;
	int ret;
	int i, base, end;
=======
	unsigned int base_reg, top_reg;
	unsigned int start, end;
	int ret;
>>>>>>> refs/remotes/origin/master

	rbtree_ctx = map->cache;
	for (node = rb_first(&rbtree_ctx->root); node; node = rb_next(node)) {
		rbnode = rb_entry(node, struct regcache_rbtree_node, node);

<<<<<<< HEAD
		if (rbnode->base_reg < min)
			continue;
		if (rbnode->base_reg > max)
			break;
		if (rbnode->base_reg + rbnode->blklen < min)
			continue;

		if (min > rbnode->base_reg)
			base = min - rbnode->base_reg;
		else
			base = 0;

		if (max < rbnode->base_reg + rbnode->blklen)
			end = max - rbnode->base_reg + 1;
		else
			end = rbnode->blklen;

		for (i = base; i < end; i++) {
			regtmp = rbnode->base_reg + i;
			val = regcache_rbtree_get_register(rbnode, i,
							   map->cache_word_size);

			/* Is this the hardware default?  If so skip. */
			ret = regcache_lookup_reg(map, regtmp);
			if (ret >= 0 && val == map->reg_defaults[ret].def)
				continue;

			map->cache_bypass = 1;
			ret = _regmap_write(map, regtmp, val);
			map->cache_bypass = 0;
			if (ret)
				return ret;
			dev_dbg(map->dev, "Synced register %#x, value %#x\n",
				regtmp, val);
		}
=======
		regcache_rbtree_get_base_top_reg(map, rbnode, &base_reg,
			&top_reg);
		if (base_reg > max)
			break;
		if (top_reg < min)
			continue;

		if (min > base_reg)
			start = (min - base_reg) / map->reg_stride;
		else
			start = 0;

		if (max < top_reg)
			end = (max - base_reg) / map->reg_stride + 1;
		else
			end = rbnode->blklen;

		ret = regcache_sync_block(map, rbnode->block,
					  rbnode->cache_present,
					  rbnode->base_reg, start, end);
		if (ret != 0)
			return ret;
	}

	return regmap_async_complete(map);
}

static int regcache_rbtree_drop(struct regmap *map, unsigned int min,
				unsigned int max)
{
	struct regcache_rbtree_ctx *rbtree_ctx;
	struct regcache_rbtree_node *rbnode;
	struct rb_node *node;
	unsigned int base_reg, top_reg;
	unsigned int start, end;

	rbtree_ctx = map->cache;
	for (node = rb_first(&rbtree_ctx->root); node; node = rb_next(node)) {
		rbnode = rb_entry(node, struct regcache_rbtree_node, node);

		regcache_rbtree_get_base_top_reg(map, rbnode, &base_reg,
			&top_reg);
		if (base_reg > max)
			break;
		if (top_reg < min)
			continue;

		if (min > base_reg)
			start = (min - base_reg) / map->reg_stride;
		else
			start = 0;

		if (max < top_reg)
			end = (max - base_reg) / map->reg_stride + 1;
		else
			end = rbnode->blklen;

		bitmap_clear(rbnode->cache_present, start, end - start);
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

struct regcache_ops regcache_rbtree_ops = {
	.type = REGCACHE_RBTREE,
	.name = "rbtree",
	.init = regcache_rbtree_init,
	.exit = regcache_rbtree_exit,
	.read = regcache_rbtree_read,
	.write = regcache_rbtree_write,
<<<<<<< HEAD
	.sync = regcache_rbtree_sync
=======
	.sync = regcache_rbtree_sync,
	.drop = regcache_rbtree_drop,
>>>>>>> refs/remotes/origin/master
};
