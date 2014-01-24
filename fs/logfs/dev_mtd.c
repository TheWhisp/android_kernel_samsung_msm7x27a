/*
 * fs/logfs/dev_mtd.c	- Device access methods for MTD
 *
 * As should be obvious for Linux kernel code, license is GPLv2
 *
 * Copyright (c) 2005-2008 Joern Engel <joern@logfs.org>
 */
#include "logfs.h"
#include <linux/completion.h>
#include <linux/mount.h>
#include <linux/sched.h>
#include <linux/slab.h>

#define PAGE_OFS(ofs) ((ofs) & (PAGE_SIZE-1))

<<<<<<< HEAD
static int mtd_read(struct super_block *sb, loff_t ofs, size_t len, void *buf)
=======
static int logfs_mtd_read(struct super_block *sb, loff_t ofs, size_t len,
			void *buf)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct mtd_info *mtd = logfs_super(sb)->s_mtd;
	size_t retlen;
	int ret;

<<<<<<< HEAD
	ret = mtd->read(mtd, ofs, len, &retlen, buf);
=======
	ret = mtd_read(mtd, ofs, len, &retlen, buf);
>>>>>>> refs/remotes/origin/cm-10.0
	BUG_ON(ret == -EINVAL);
	if (ret)
		return ret;

	/* Not sure if we should loop instead. */
	if (retlen != len)
		return -EIO;

	return 0;
}

<<<<<<< HEAD
static int mtd_write(struct super_block *sb, loff_t ofs, size_t len, void *buf)
=======
static int loffs_mtd_write(struct super_block *sb, loff_t ofs, size_t len,
			void *buf)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct logfs_super *super = logfs_super(sb);
	struct mtd_info *mtd = super->s_mtd;
	size_t retlen;
	loff_t page_start, page_end;
	int ret;

	if (super->s_flags & LOGFS_SB_FLAG_RO)
		return -EROFS;

	BUG_ON((ofs >= mtd->size) || (len > mtd->size - ofs));
	BUG_ON(ofs != (ofs >> super->s_writeshift) << super->s_writeshift);
	BUG_ON(len > PAGE_CACHE_SIZE);
	page_start = ofs & PAGE_CACHE_MASK;
	page_end = PAGE_CACHE_ALIGN(ofs + len) - 1;
<<<<<<< HEAD
	ret = mtd->write(mtd, ofs, len, &retlen, buf);
=======
	ret = mtd_write(mtd, ofs, len, &retlen, buf);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret || (retlen != len))
		return -EIO;

	return 0;
}

/*
 * For as long as I can remember (since about 2001) mtd->erase has been an
 * asynchronous interface lacking the first driver to actually use the
 * asynchronous properties.  So just to prevent the first implementor of such
 * a thing from breaking logfs in 2350, we do the usual pointless dance to
 * declare a completion variable and wait for completion before returning
<<<<<<< HEAD
 * from mtd_erase().  What an exercise in futility!
=======
 * from logfs_mtd_erase().  What an exercise in futility!
>>>>>>> refs/remotes/origin/cm-10.0
 */
static void logfs_erase_callback(struct erase_info *ei)
{
	complete((struct completion *)ei->priv);
}

<<<<<<< HEAD
static int mtd_erase_mapping(struct super_block *sb, loff_t ofs, size_t len)
=======
static int logfs_mtd_erase_mapping(struct super_block *sb, loff_t ofs,
				size_t len)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct logfs_super *super = logfs_super(sb);
	struct address_space *mapping = super->s_mapping_inode->i_mapping;
	struct page *page;
	pgoff_t index = ofs >> PAGE_SHIFT;

	for (index = ofs >> PAGE_SHIFT; index < (ofs + len) >> PAGE_SHIFT; index++) {
		page = find_get_page(mapping, index);
		if (!page)
			continue;
		memset(page_address(page), 0xFF, PAGE_SIZE);
		page_cache_release(page);
	}
	return 0;
}

<<<<<<< HEAD
static int mtd_erase(struct super_block *sb, loff_t ofs, size_t len,
=======
static int logfs_mtd_erase(struct super_block *sb, loff_t ofs, size_t len,
>>>>>>> refs/remotes/origin/cm-10.0
		int ensure_write)
{
	struct mtd_info *mtd = logfs_super(sb)->s_mtd;
	struct erase_info ei;
	DECLARE_COMPLETION_ONSTACK(complete);
	int ret;

	BUG_ON(len % mtd->erasesize);
	if (logfs_super(sb)->s_flags & LOGFS_SB_FLAG_RO)
		return -EROFS;

	memset(&ei, 0, sizeof(ei));
	ei.mtd = mtd;
	ei.addr = ofs;
	ei.len = len;
	ei.callback = logfs_erase_callback;
	ei.priv = (long)&complete;
<<<<<<< HEAD
	ret = mtd->erase(mtd, &ei);
=======
	ret = mtd_erase(mtd, &ei);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		return -EIO;

	wait_for_completion(&complete);
	if (ei.state != MTD_ERASE_DONE)
		return -EIO;
<<<<<<< HEAD
	return mtd_erase_mapping(sb, ofs, len);
}

static void mtd_sync(struct super_block *sb)
{
	struct mtd_info *mtd = logfs_super(sb)->s_mtd;

	if (mtd->sync)
		mtd->sync(mtd);
}

static int mtd_readpage(void *_sb, struct page *page)
=======
	return logfs_mtd_erase_mapping(sb, ofs, len);
}

static void logfs_mtd_sync(struct super_block *sb)
{
	struct mtd_info *mtd = logfs_super(sb)->s_mtd;

	mtd_sync(mtd);
}

static int logfs_mtd_readpage(void *_sb, struct page *page)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct super_block *sb = _sb;
	int err;

<<<<<<< HEAD
	err = mtd_read(sb, page->index << PAGE_SHIFT, PAGE_SIZE,
=======
	err = logfs_mtd_read(sb, page->index << PAGE_SHIFT, PAGE_SIZE,
>>>>>>> refs/remotes/origin/cm-10.0
			page_address(page));
	if (err == -EUCLEAN || err == -EBADMSG) {
		/* -EBADMSG happens regularly on power failures */
		err = 0;
		/* FIXME: force GC this segment */
	}
	if (err) {
		ClearPageUptodate(page);
		SetPageError(page);
	} else {
		SetPageUptodate(page);
		ClearPageError(page);
	}
	unlock_page(page);
	return err;
}

<<<<<<< HEAD
static struct page *mtd_find_first_sb(struct super_block *sb, u64 *ofs)
{
	struct logfs_super *super = logfs_super(sb);
	struct address_space *mapping = super->s_mapping_inode->i_mapping;
	filler_t *filler = mtd_readpage;
	struct mtd_info *mtd = super->s_mtd;

	if (!mtd->block_isbad)
		return NULL;

	*ofs = 0;
	while (mtd->block_isbad(mtd, *ofs)) {
=======
static struct page *logfs_mtd_find_first_sb(struct super_block *sb, u64 *ofs)
{
	struct logfs_super *super = logfs_super(sb);
	struct address_space *mapping = super->s_mapping_inode->i_mapping;
	filler_t *filler = logfs_mtd_readpage;
	struct mtd_info *mtd = super->s_mtd;

	*ofs = 0;
	while (mtd_block_isbad(mtd, *ofs)) {
>>>>>>> refs/remotes/origin/cm-10.0
		*ofs += mtd->erasesize;
		if (*ofs >= mtd->size)
			return NULL;
	}
	BUG_ON(*ofs & ~PAGE_MASK);
	return read_cache_page(mapping, *ofs >> PAGE_SHIFT, filler, sb);
}

<<<<<<< HEAD
static struct page *mtd_find_last_sb(struct super_block *sb, u64 *ofs)
{
	struct logfs_super *super = logfs_super(sb);
	struct address_space *mapping = super->s_mapping_inode->i_mapping;
	filler_t *filler = mtd_readpage;
	struct mtd_info *mtd = super->s_mtd;

	if (!mtd->block_isbad)
		return NULL;

	*ofs = mtd->size - mtd->erasesize;
	while (mtd->block_isbad(mtd, *ofs)) {
=======
static struct page *logfs_mtd_find_last_sb(struct super_block *sb, u64 *ofs)
{
	struct logfs_super *super = logfs_super(sb);
	struct address_space *mapping = super->s_mapping_inode->i_mapping;
	filler_t *filler = logfs_mtd_readpage;
	struct mtd_info *mtd = super->s_mtd;

	*ofs = mtd->size - mtd->erasesize;
	while (mtd_block_isbad(mtd, *ofs)) {
>>>>>>> refs/remotes/origin/cm-10.0
		*ofs -= mtd->erasesize;
		if (*ofs <= 0)
			return NULL;
	}
	*ofs = *ofs + mtd->erasesize - 0x1000;
	BUG_ON(*ofs & ~PAGE_MASK);
	return read_cache_page(mapping, *ofs >> PAGE_SHIFT, filler, sb);
}

<<<<<<< HEAD
static int __mtd_writeseg(struct super_block *sb, u64 ofs, pgoff_t index,
=======
static int __logfs_mtd_writeseg(struct super_block *sb, u64 ofs, pgoff_t index,
>>>>>>> refs/remotes/origin/cm-10.0
		size_t nr_pages)
{
	struct logfs_super *super = logfs_super(sb);
	struct address_space *mapping = super->s_mapping_inode->i_mapping;
	struct page *page;
	int i, err;

	for (i = 0; i < nr_pages; i++) {
		page = find_lock_page(mapping, index + i);
		BUG_ON(!page);

<<<<<<< HEAD
		err = mtd_write(sb, page->index << PAGE_SHIFT, PAGE_SIZE,
				page_address(page));
=======
		err = loffs_mtd_write(sb, page->index << PAGE_SHIFT, PAGE_SIZE,
					page_address(page));
>>>>>>> refs/remotes/origin/cm-10.0
		unlock_page(page);
		page_cache_release(page);
		if (err)
			return err;
	}
	return 0;
}

<<<<<<< HEAD
static void mtd_writeseg(struct super_block *sb, u64 ofs, size_t len)
=======
static void logfs_mtd_writeseg(struct super_block *sb, u64 ofs, size_t len)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct logfs_super *super = logfs_super(sb);
	int head;

	if (super->s_flags & LOGFS_SB_FLAG_RO)
		return;

	if (len == 0) {
		/* This can happen when the object fit perfectly into a
		 * segment, the segment gets written per sync and subsequently
		 * closed.
		 */
		return;
	}
	head = ofs & (PAGE_SIZE - 1);
	if (head) {
		ofs -= head;
		len += head;
	}
	len = PAGE_ALIGN(len);
<<<<<<< HEAD
	__mtd_writeseg(sb, ofs, ofs >> PAGE_SHIFT, len >> PAGE_SHIFT);
}

static void mtd_put_device(struct logfs_super *s)
=======
	__logfs_mtd_writeseg(sb, ofs, ofs >> PAGE_SHIFT, len >> PAGE_SHIFT);
}

static void logfs_mtd_put_device(struct logfs_super *s)
>>>>>>> refs/remotes/origin/cm-10.0
{
	put_mtd_device(s->s_mtd);
}

<<<<<<< HEAD
static int mtd_can_write_buf(struct super_block *sb, u64 ofs)
=======
static int logfs_mtd_can_write_buf(struct super_block *sb, u64 ofs)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct logfs_super *super = logfs_super(sb);
	void *buf;
	int err;

	buf = kmalloc(super->s_writesize, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;
<<<<<<< HEAD
	err = mtd_read(sb, ofs, super->s_writesize, buf);
=======
	err = logfs_mtd_read(sb, ofs, super->s_writesize, buf);
>>>>>>> refs/remotes/origin/cm-10.0
	if (err)
		goto out;
	if (memchr_inv(buf, 0xff, super->s_writesize))
		err = -EIO;
	kfree(buf);
out:
	return err;
}

static const struct logfs_device_ops mtd_devops = {
<<<<<<< HEAD
	.find_first_sb	= mtd_find_first_sb,
	.find_last_sb	= mtd_find_last_sb,
	.readpage	= mtd_readpage,
	.writeseg	= mtd_writeseg,
	.erase		= mtd_erase,
	.can_write_buf	= mtd_can_write_buf,
	.sync		= mtd_sync,
	.put_device	= mtd_put_device,
=======
	.find_first_sb	= logfs_mtd_find_first_sb,
	.find_last_sb	= logfs_mtd_find_last_sb,
	.readpage	= logfs_mtd_readpage,
	.writeseg	= logfs_mtd_writeseg,
	.erase		= logfs_mtd_erase,
	.can_write_buf	= logfs_mtd_can_write_buf,
	.sync		= logfs_mtd_sync,
	.put_device	= logfs_mtd_put_device,
>>>>>>> refs/remotes/origin/cm-10.0
};

int logfs_get_sb_mtd(struct logfs_super *s, int mtdnr)
{
	struct mtd_info *mtd = get_mtd_device(NULL, mtdnr);
	if (IS_ERR(mtd))
		return PTR_ERR(mtd);

	s->s_bdev = NULL;
	s->s_mtd = mtd;
	s->s_devops = &mtd_devops;
	return 0;
}
