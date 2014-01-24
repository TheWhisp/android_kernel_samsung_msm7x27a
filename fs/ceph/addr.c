#include <linux/ceph/ceph_debug.h>

#include <linux/backing-dev.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/writeback.h>	/* generic_writepages */
#include <linux/slab.h>
#include <linux/pagevec.h>
#include <linux/task_io_accounting_ops.h>

#include "super.h"
#include "mds_client.h"
<<<<<<< HEAD
=======
#include "cache.h"
>>>>>>> refs/remotes/origin/master
#include <linux/ceph/osd_client.h>

/*
 * Ceph address space ops.
 *
 * There are a few funny things going on here.
 *
 * The page->private field is used to reference a struct
 * ceph_snap_context for _every_ dirty page.  This indicates which
 * snapshot the page was logically dirtied in, and thus which snap
 * context needs to be associated with the osd write during writeback.
 *
 * Similarly, struct ceph_inode_info maintains a set of counters to
 * count dirty pages on the inode.  In the absence of snapshots,
 * i_wrbuffer_ref == i_wrbuffer_ref_head == the dirty page count.
 *
 * When a snapshot is taken (that is, when the client receives
 * notification that a snapshot was taken), each inode with caps and
 * with dirty pages (dirty pages implies there is a cap) gets a new
 * ceph_cap_snap in the i_cap_snaps list (which is sorted in ascending
 * order, new snaps go to the tail).  The i_wrbuffer_ref_head count is
 * moved to capsnap->dirty. (Unless a sync write is currently in
 * progress.  In that case, the capsnap is said to be "pending", new
 * writes cannot start, and the capsnap isn't "finalized" until the
 * write completes (or fails) and a final size/mtime for the inode for
 * that snap can be settled upon.)  i_wrbuffer_ref_head is reset to 0.
 *
 * On writeback, we must submit writes to the osd IN SNAP ORDER.  So,
 * we look for the first capsnap in i_cap_snaps and write out pages in
 * that snap context _only_.  Then we move on to the next capsnap,
 * eventually reaching the "live" or "head" context (i.e., pages that
 * are not yet snapped) and are writing the most recently dirtied
 * pages.
 *
 * Invalidate and so forth must take care to ensure the dirty page
 * accounting is preserved.
 */

#define CONGESTION_ON_THRESH(congestion_kb) (congestion_kb >> (PAGE_SHIFT-10))
#define CONGESTION_OFF_THRESH(congestion_kb)				\
	(CONGESTION_ON_THRESH(congestion_kb) -				\
	 (CONGESTION_ON_THRESH(congestion_kb) >> 2))

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static inline struct ceph_snap_context *page_snap_context(struct page *page)
{
	if (PagePrivate(page))
		return (void *)page->private;
	return NULL;
}
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

/*
 * Dirty a page.  Optimistically adjust accounting, on the assumption
 * that we won't race with invalidate.  If we do, readjust.
 */
static int ceph_set_page_dirty(struct page *page)
{
	struct address_space *mapping = page->mapping;
	struct inode *inode;
	struct ceph_inode_info *ci;
<<<<<<< HEAD
	int undo = 0;
	struct ceph_snap_context *snapc;
=======
	struct ceph_snap_context *snapc;
	int ret;
>>>>>>> refs/remotes/origin/master

	if (unlikely(!mapping))
		return !TestSetPageDirty(page);

<<<<<<< HEAD
	if (TestSetPageDirty(page)) {
		dout("%p set_page_dirty %p idx %lu -- already dirty\n",
		     mapping->host, page, page->index);
=======
	if (PageDirty(page)) {
		dout("%p set_page_dirty %p idx %lu -- already dirty\n",
		     mapping->host, page, page->index);
		BUG_ON(!PagePrivate(page));
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	inode = mapping->host;
	ci = ceph_inode(inode);

	/*
	 * Note that we're grabbing a snapc ref here without holding
	 * any locks!
	 */
	snapc = ceph_get_snap_context(ci->i_snap_realm->cached_context);

	/* dirty the head */
<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&inode->i_lock);
=======
	spin_lock(&ci->i_ceph_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_lock(&ci->i_ceph_lock);
>>>>>>> refs/remotes/origin/master
	if (ci->i_head_snapc == NULL)
		ci->i_head_snapc = ceph_get_snap_context(snapc);
	++ci->i_wrbuffer_ref_head;
	if (ci->i_wrbuffer_ref == 0)
		ihold(inode);
	++ci->i_wrbuffer_ref;
	dout("%p set_page_dirty %p idx %lu head %d/%d -> %d/%d "
	     "snapc %p seq %lld (%d snaps)\n",
	     mapping->host, page, page->index,
	     ci->i_wrbuffer_ref-1, ci->i_wrbuffer_ref_head-1,
	     ci->i_wrbuffer_ref, ci->i_wrbuffer_ref_head,
	     snapc, snapc->seq, snapc->num_snaps);
<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock(&inode->i_lock);
=======
	spin_unlock(&ci->i_ceph_lock);
>>>>>>> refs/remotes/origin/cm-10.0

	/* now adjust page */
	spin_lock_irq(&mapping->tree_lock);
	if (page->mapping) {	/* Race with truncate? */
		WARN_ON_ONCE(!PageUptodate(page));
		account_page_dirtied(page, page->mapping);
		radix_tree_tag_set(&mapping->page_tree,
				page_index(page), PAGECACHE_TAG_DIRTY);

		/*
		 * Reference snap context in page->private.  Also set
		 * PagePrivate so that we get invalidatepage callback.
		 */
		page->private = (unsigned long)snapc;
		SetPagePrivate(page);
	} else {
		dout("ANON set_page_dirty %p (raced truncate?)\n", page);
		undo = 1;
	}

	spin_unlock_irq(&mapping->tree_lock);

	if (undo)
		/* whoops, we failed to dirty the page */
		ceph_put_wrbuffer_cap_refs(ci, 1, snapc);

	__mark_inode_dirty(mapping->host, I_DIRTY_PAGES);

	BUG_ON(!PageDirty(page));
	return 1;
=======
	spin_unlock(&ci->i_ceph_lock);

	/*
	 * Reference snap context in page->private.  Also set
	 * PagePrivate so that we get invalidatepage callback.
	 */
	BUG_ON(PagePrivate(page));
	page->private = (unsigned long)snapc;
	SetPagePrivate(page);

	ret = __set_page_dirty_nobuffers(page);
	WARN_ON(!PageLocked(page));
	WARN_ON(!page->mapping);

	return ret;
>>>>>>> refs/remotes/origin/master
}

/*
 * If we are truncating the full page (i.e. offset == 0), adjust the
 * dirty page counters appropriately.  Only called if there is private
 * data on the page.
 */
<<<<<<< HEAD
static void ceph_invalidatepage(struct page *page, unsigned long offset)
{
	struct inode *inode;
	struct ceph_inode_info *ci;
<<<<<<< HEAD
<<<<<<< HEAD
	struct ceph_snap_context *snapc = (void *)page->private;

	BUG_ON(!PageLocked(page));
	BUG_ON(!page->private);
=======
	struct ceph_snap_context *snapc = page_snap_context(page);

	BUG_ON(!PageLocked(page));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ceph_snap_context *snapc = page_snap_context(page);

	BUG_ON(!PageLocked(page));
>>>>>>> refs/remotes/origin/cm-11.0
	BUG_ON(!PagePrivate(page));
	BUG_ON(!page->mapping);

	inode = page->mapping->host;
=======
static void ceph_invalidatepage(struct page *page, unsigned int offset,
				unsigned int length)
{
	struct inode *inode;
	struct ceph_inode_info *ci;
	struct ceph_snap_context *snapc = page_snap_context(page);

	inode = page->mapping->host;
	ci = ceph_inode(inode);

	if (offset != 0 || length != PAGE_CACHE_SIZE) {
		dout("%p invalidatepage %p idx %lu partial dirty page %u~%u\n",
		     inode, page, page->index, offset, length);
		return;
	}

	ceph_invalidate_fscache_page(inode, page);

	if (!PagePrivate(page))
		return;
>>>>>>> refs/remotes/origin/master

	/*
	 * We can get non-dirty pages here due to races between
	 * set_page_dirty and truncate_complete_page; just spit out a
	 * warning, in case we end up with accounting problems later.
	 */
	if (!PageDirty(page))
		pr_err("%p invalidatepage %p page not dirty\n", inode, page);

<<<<<<< HEAD
	if (offset == 0)
		ClearPageChecked(page);

	ci = ceph_inode(inode);
	if (offset == 0) {
		dout("%p invalidatepage %p idx %lu full dirty page %lu\n",
		     inode, page, page->index, offset);
		ceph_put_wrbuffer_cap_refs(ci, 1, snapc);
		ceph_put_snap_context(snapc);
		page->private = 0;
		ClearPagePrivate(page);
	} else {
		dout("%p invalidatepage %p idx %lu partial dirty page\n",
		     inode, page, page->index);
	}
}

/* just a sanity check */
=======
	ClearPageChecked(page);

	dout("%p invalidatepage %p idx %lu full dirty page\n",
	     inode, page, page->index);

	ceph_put_wrbuffer_cap_refs(ci, 1, snapc);
	ceph_put_snap_context(snapc);
	page->private = 0;
	ClearPagePrivate(page);
}

>>>>>>> refs/remotes/origin/master
static int ceph_releasepage(struct page *page, gfp_t g)
{
	struct inode *inode = page->mapping ? page->mapping->host : NULL;
	dout("%p releasepage %p idx %lu\n", inode, page, page->index);
	WARN_ON(PageDirty(page));
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	WARN_ON(page->private);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	WARN_ON(PagePrivate(page));
	return 0;
=======

	/* Can we release the page from the cache? */
	if (!ceph_release_fscache_page(page, g))
		return 0;

	return !PagePrivate(page);
>>>>>>> refs/remotes/origin/master
}

/*
 * read a single page, without unlocking it.
 */
static int readpage_nounlock(struct file *filp, struct page *page)
{
<<<<<<< HEAD
	struct inode *inode = filp->f_dentry->d_inode;
	struct ceph_inode_info *ci = ceph_inode(inode);
	struct ceph_osd_client *osdc = 
=======
	struct inode *inode = file_inode(filp);
	struct ceph_inode_info *ci = ceph_inode(inode);
	struct ceph_osd_client *osdc =
>>>>>>> refs/remotes/origin/master
		&ceph_inode_to_client(inode)->client->osdc;
	int err = 0;
	u64 len = PAGE_CACHE_SIZE;

<<<<<<< HEAD
	dout("readpage inode %p file %p page %p index %lu\n",
	     inode, filp, page, page->index);
	err = ceph_osdc_readpages(osdc, ceph_vino(inode), &ci->i_layout,
<<<<<<< HEAD
<<<<<<< HEAD
				  page->index << PAGE_CACHE_SHIFT, &len,
=======
				  (u64) page_offset(page), &len,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = ceph_readpage_from_fscache(inode, page);

	if (err == 0)
		goto out;

	dout("readpage inode %p file %p page %p index %lu\n",
	     inode, filp, page, page->index);
	err = ceph_osdc_readpages(osdc, ceph_vino(inode), &ci->i_layout,
				  (u64) page_offset(page), &len,
>>>>>>> refs/remotes/origin/master
=======
				  (u64) page_offset(page), &len,
>>>>>>> refs/remotes/origin/cm-11.0
				  ci->i_truncate_seq, ci->i_truncate_size,
				  &page, 1, 0);
	if (err == -ENOENT)
		err = 0;
	if (err < 0) {
		SetPageError(page);
		goto out;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
	} else {
		if (err < PAGE_CACHE_SIZE) {
		/* zero fill remainder of page */
			zero_user_segment(page, err, PAGE_CACHE_SIZE);
		} else {
			flush_dcache_page(page);
		}
<<<<<<< HEAD
=======
	} else if (err < PAGE_CACHE_SIZE) {
		/* zero fill remainder of page */
		zero_user_segment(page, err, PAGE_CACHE_SIZE);
>>>>>>> refs/remotes/origin/cm-10.0
	}
	SetPageUptodate(page);

=======
=======
	} else {
		if (err < PAGE_CACHE_SIZE) {
		/* zero fill remainder of page */
			zero_user_segment(page, err, PAGE_CACHE_SIZE);
		} else {
			flush_dcache_page(page);
		}
>>>>>>> refs/remotes/origin/cm-11.0
	}
	SetPageUptodate(page);

	if (err >= 0)
		ceph_readpage_to_fscache(inode, page);

>>>>>>> refs/remotes/origin/master
out:
	return err < 0 ? err : 0;
}

static int ceph_readpage(struct file *filp, struct page *page)
{
	int r = readpage_nounlock(filp, page);
	unlock_page(page);
	return r;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Build a vector of contiguous pages from the provided page list.
 */
static struct page **page_vector_from_list(struct list_head *page_list,
					   unsigned *nr_pages)
{
	struct page **pages;
	struct page *page;
	int next_index, contig_pages = 0;

	/* build page vector */
	pages = kmalloc(sizeof(*pages) * *nr_pages, GFP_NOFS);
	if (!pages)
		return ERR_PTR(-ENOMEM);

	BUG_ON(list_empty(page_list));
	next_index = list_entry(page_list->prev, struct page, lru)->index;
	list_for_each_entry_reverse(page, page_list, lru) {
		if (page->index == next_index) {
			dout("readpages page %d %p\n", contig_pages, page);
			pages[contig_pages] = page;
			contig_pages++;
			next_index++;
		} else {
			break;
		}
	}
	*nr_pages = contig_pages;
	return pages;
}

/*
 * Read multiple pages.  Leave pages we don't read + unlock in page_list;
 * the caller (VM) cleans them up.
 */
static int ceph_readpages(struct file *file, struct address_space *mapping,
			  struct list_head *page_list, unsigned nr_pages)
{
	struct inode *inode = file->f_dentry->d_inode;
	struct ceph_inode_info *ci = ceph_inode(inode);
	struct ceph_osd_client *osdc =
		&ceph_inode_to_client(inode)->client->osdc;
	int rc = 0;
	struct page **pages;
	loff_t offset;
	u64 len;

	dout("readpages %p file %p nr_pages %d\n",
	     inode, file, nr_pages);

	pages = page_vector_from_list(page_list, &nr_pages);
	if (IS_ERR(pages))
		return PTR_ERR(pages);

	/* guess read extent */
	offset = pages[0]->index << PAGE_CACHE_SHIFT;
	len = nr_pages << PAGE_CACHE_SHIFT;
	rc = ceph_osdc_readpages(osdc, ceph_vino(inode), &ci->i_layout,
				 offset, &len,
				 ci->i_truncate_seq, ci->i_truncate_size,
				 pages, nr_pages, 0);
	if (rc == -ENOENT)
		rc = 0;
	if (rc < 0)
		goto out;

	for (; !list_empty(page_list) && len > 0;
	     rc -= PAGE_CACHE_SIZE, len -= PAGE_CACHE_SIZE) {
		struct page *page =
			list_entry(page_list->prev, struct page, lru);

		list_del(&page->lru);

		if (rc < (int)PAGE_CACHE_SIZE) {
			/* zero (remainder of) page */
			int s = rc < 0 ? 0 : rc;
			zero_user_segment(page, s, PAGE_CACHE_SIZE);
		}

		if (add_to_page_cache_lru(page, mapping, page->index,
					  GFP_NOFS)) {
			page_cache_release(page);
			dout("readpages %p add_to_page_cache failed %p\n",
			     inode, page);
			continue;
		}
		dout("readpages %p adding %p idx %lu\n", inode, page,
		     page->index);
		flush_dcache_page(page);
		SetPageUptodate(page);
		unlock_page(page);
		page_cache_release(page);
	}
	rc = 0;

out:
	kfree(pages);
=======
=======
>>>>>>> refs/remotes/origin/master
 * Finish an async read(ahead) op.
 */
static void finish_read(struct ceph_osd_request *req, struct ceph_msg *msg)
{
	struct inode *inode = req->r_inode;
<<<<<<< HEAD
	struct ceph_osd_reply_head *replyhead;
	int rc, bytes;
	int i;

	/* parse reply */
	replyhead = msg->front.iov_base;
	WARN_ON(le32_to_cpu(replyhead->num_ops) == 0);
	rc = le32_to_cpu(replyhead->result);
	bytes = le32_to_cpu(msg->hdr.data_len);

	dout("finish_read %p req %p rc %d bytes %d\n", inode, req, rc, bytes);

	/* unlock all pages, zeroing any data we didn't read */
	for (i = 0; i < req->r_num_pages; i++, bytes -= PAGE_CACHE_SIZE) {
		struct page *page = req->r_pages[i];
=======
	struct ceph_osd_data *osd_data;
	int rc = req->r_result;
	int bytes = le32_to_cpu(msg->hdr.data_len);
	int num_pages;
	int i;

	dout("finish_read %p req %p rc %d bytes %d\n", inode, req, rc, bytes);

	/* unlock all pages, zeroing any data we didn't read */
	osd_data = osd_req_op_extent_osd_data(req, 0);
	BUG_ON(osd_data->type != CEPH_OSD_DATA_TYPE_PAGES);
	num_pages = calc_pages_for((u64)osd_data->alignment,
					(u64)osd_data->length);
	for (i = 0; i < num_pages; i++) {
		struct page *page = osd_data->pages[i];
>>>>>>> refs/remotes/origin/master

		if (bytes < (int)PAGE_CACHE_SIZE) {
			/* zero (remainder of) page */
			int s = bytes < 0 ? 0 : bytes;
			zero_user_segment(page, s, PAGE_CACHE_SIZE);
		}
 		dout("finish_read %p uptodate %p idx %lu\n", inode, page,
		     page->index);
		flush_dcache_page(page);
		SetPageUptodate(page);
<<<<<<< HEAD
		unlock_page(page);
		page_cache_release(page);
	}
	kfree(req->r_pages);
=======
		ceph_readpage_to_fscache(inode, page);
		unlock_page(page);
		page_cache_release(page);
		bytes -= PAGE_CACHE_SIZE;
	}
	kfree(osd_data->pages);
>>>>>>> refs/remotes/origin/master
}

static void ceph_unlock_page_vector(struct page **pages, int num_pages)
{
	int i;

	for (i = 0; i < num_pages; i++)
		unlock_page(pages[i]);
}

static void ceph_unlock_page_vector(struct page **pages, int num_pages)
{
	int i;

	for (i = 0; i < num_pages; i++)
		unlock_page(pages[i]);
}

/*
 * start an async read(ahead) operation.  return nr_pages we submitted
 * a read for on success, or negative error code.
 */
static int start_read(struct inode *inode, struct list_head *page_list, int max)
{
	struct ceph_osd_client *osdc =
		&ceph_inode_to_client(inode)->client->osdc;
	struct ceph_inode_info *ci = ceph_inode(inode);
	struct page *page = list_entry(page_list->prev, struct page, lru);
<<<<<<< HEAD
=======
	struct ceph_vino vino;
>>>>>>> refs/remotes/origin/master
	struct ceph_osd_request *req;
	u64 off;
	u64 len;
	int i;
	struct page **pages;
	pgoff_t next_index;
	int nr_pages = 0;
	int ret;

	off = (u64) page_offset(page);

	/* count pages */
	next_index = page->index;
	list_for_each_entry_reverse(page, page_list, lru) {
		if (page->index != next_index)
			break;
		nr_pages++;
		next_index++;
		if (max && nr_pages == max)
			break;
	}
	len = nr_pages << PAGE_CACHE_SHIFT;
	dout("start_read %p nr_pages %d is %lld~%lld\n", inode, nr_pages,
	     off, len);
<<<<<<< HEAD

	req = ceph_osdc_new_request(osdc, &ci->i_layout, ceph_vino(inode),
				    off, &len,
				    CEPH_OSD_OP_READ, CEPH_OSD_FLAG_READ,
				    NULL, 0,
				    ci->i_truncate_seq, ci->i_truncate_size,
				    NULL, false, 1, 0);
<<<<<<< HEAD
=======
	vino = ceph_vino(inode);
	req = ceph_osdc_new_request(osdc, &ci->i_layout, vino, off, &len,
				    1, CEPH_OSD_OP_READ,
				    CEPH_OSD_FLAG_READ, NULL,
				    ci->i_truncate_seq, ci->i_truncate_size,
				    false);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (IS_ERR(req))
		return PTR_ERR(req);

	/* build page vector */
<<<<<<< HEAD
	nr_pages = len >> PAGE_CACHE_SHIFT;
=======
	nr_pages = calc_pages_for(0, len);
>>>>>>> refs/remotes/origin/master
	pages = kmalloc(sizeof(*pages) * nr_pages, GFP_NOFS);
	ret = -ENOMEM;
	if (!pages)
		goto out;
	for (i = 0; i < nr_pages; ++i) {
		page = list_entry(page_list->prev, struct page, lru);
		BUG_ON(PageLocked(page));
		list_del(&page->lru);
<<<<<<< HEAD
		
=======

>>>>>>> refs/remotes/origin/master
 		dout("start_read %p adding %p idx %lu\n", inode, page,
		     page->index);
		if (add_to_page_cache_lru(page, &inode->i_data, page->index,
					  GFP_NOFS)) {
<<<<<<< HEAD
=======
			ceph_fscache_uncache_page(inode, page);
>>>>>>> refs/remotes/origin/master
			page_cache_release(page);
			dout("start_read %p add_to_page_cache failed %p\n",
			     inode, page);
			nr_pages = i;
			goto out_pages;
		}
		pages[i] = page;
	}
<<<<<<< HEAD
	req->r_pages = pages;
	req->r_num_pages = nr_pages;
	req->r_callback = finish_read;
	req->r_inode = inode;

=======
	osd_req_op_extent_osd_data_pages(req, 0, pages, len, 0, false, false);
	req->r_callback = finish_read;
	req->r_inode = inode;

	ceph_osdc_build_request(req, off, NULL, vino.snap, NULL);

>>>>>>> refs/remotes/origin/master
	dout("start_read %p starting %p %lld~%lld\n", inode, req, off, len);
	ret = ceph_osdc_start_request(osdc, req, false);
	if (ret < 0)
		goto out_pages;
	ceph_osdc_put_request(req);
	return nr_pages;

out_pages:
	ceph_unlock_page_vector(pages, nr_pages);
	ceph_release_page_vector(pages, nr_pages);
out:
	ceph_osdc_put_request(req);
	return ret;
}


/*
 * Read multiple pages.  Leave pages we don't read + unlock in page_list;
 * the caller (VM) cleans them up.
 */
static int ceph_readpages(struct file *file, struct address_space *mapping,
			  struct list_head *page_list, unsigned nr_pages)
{
<<<<<<< HEAD
	struct inode *inode = file->f_dentry->d_inode;
=======
	struct inode *inode = file_inode(file);
>>>>>>> refs/remotes/origin/master
	struct ceph_fs_client *fsc = ceph_inode_to_client(inode);
	int rc = 0;
	int max = 0;

<<<<<<< HEAD
=======
	rc = ceph_readpages_from_fscache(mapping->host, mapping, page_list,
					 &nr_pages);

	if (rc == 0)
		goto out;

>>>>>>> refs/remotes/origin/master
	if (fsc->mount_options->rsize >= PAGE_CACHE_SIZE)
		max = (fsc->mount_options->rsize + PAGE_CACHE_SIZE - 1)
			>> PAGE_SHIFT;

<<<<<<< HEAD
	dout("readpages %p file %p nr_pages %d max %d\n", inode, file, nr_pages,
=======
	dout("readpages %p file %p nr_pages %d max %d\n", inode,
		file, nr_pages,
>>>>>>> refs/remotes/origin/master
	     max);
	while (!list_empty(page_list)) {
		rc = start_read(inode, page_list, max);
		if (rc < 0)
			goto out;
		BUG_ON(rc == 0);
	}
out:
<<<<<<< HEAD
	dout("readpages %p file %p ret %d\n", inode, file, rc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ceph_fscache_readpages_cancel(inode, page_list);

	dout("readpages %p file %p ret %d\n", inode, file, rc);
>>>>>>> refs/remotes/origin/master
	return rc;
}

/*
 * Get ref for the oldest snapc for an inode with dirty data... that is, the
 * only snap context we are allowed to write back.
 */
static struct ceph_snap_context *get_oldest_context(struct inode *inode,
						    u64 *snap_size)
{
	struct ceph_inode_info *ci = ceph_inode(inode);
	struct ceph_snap_context *snapc = NULL;
	struct ceph_cap_snap *capsnap = NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&inode->i_lock);
=======
	spin_lock(&ci->i_ceph_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_lock(&ci->i_ceph_lock);
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(capsnap, &ci->i_cap_snaps, ci_item) {
		dout(" cap_snap %p snapc %p has %d dirty pages\n", capsnap,
		     capsnap->context, capsnap->dirty_pages);
		if (capsnap->dirty_pages) {
			snapc = ceph_get_snap_context(capsnap->context);
			if (snap_size)
				*snap_size = capsnap->size;
			break;
		}
	}
	if (!snapc && ci->i_wrbuffer_ref_head) {
		snapc = ceph_get_snap_context(ci->i_head_snapc);
		dout(" head snapc %p has %d dirty pages\n",
		     snapc, ci->i_wrbuffer_ref_head);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock(&inode->i_lock);
=======
	spin_unlock(&ci->i_ceph_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock(&ci->i_ceph_lock);
>>>>>>> refs/remotes/origin/master
	return snapc;
}

/*
 * Write a single page, but leave the page locked.
 *
 * If we get a write error, set the page error bit, but still adjust the
 * dirty page accounting (i.e., page is no longer dirty).
 */
static int writepage_nounlock(struct page *page, struct writeback_control *wbc)
{
	struct inode *inode;
	struct ceph_inode_info *ci;
	struct ceph_fs_client *fsc;
	struct ceph_osd_client *osdc;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	loff_t page_off = page->index << PAGE_CACHE_SHIFT;
=======
	loff_t page_off = page_offset(page);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	loff_t page_off = page_offset(page);
>>>>>>> refs/remotes/origin/cm-11.0
	int len = PAGE_CACHE_SIZE;
	loff_t i_size;
	int err = 0;
	struct ceph_snap_context *snapc, *oldest;
	u64 snap_size = 0;
	long writeback_stat;
=======
	struct ceph_snap_context *snapc, *oldest;
	loff_t page_off = page_offset(page);
	long writeback_stat;
	u64 truncate_size, snap_size = 0;
	u32 truncate_seq;
	int err = 0, len = PAGE_CACHE_SIZE;
>>>>>>> refs/remotes/origin/master

	dout("writepage %p idx %lu\n", page, page->index);

	if (!page->mapping || !page->mapping->host) {
		dout("writepage %p - no mapping\n", page);
		return -EFAULT;
	}
	inode = page->mapping->host;
	ci = ceph_inode(inode);
	fsc = ceph_inode_to_client(inode);
	osdc = &fsc->client->osdc;

	/* verify this is a writeable snap context */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	snapc = (void *)page->private;
=======
	snapc = page_snap_context(page);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	snapc = page_snap_context(page);
>>>>>>> refs/remotes/origin/master
=======
	snapc = page_snap_context(page);
>>>>>>> refs/remotes/origin/cm-11.0
	if (snapc == NULL) {
		dout("writepage %p page %p not dirty?\n", inode, page);
		goto out;
	}
	oldest = get_oldest_context(inode, &snap_size);
	if (snapc->seq > oldest->seq) {
		dout("writepage %p page %p snapc %p not writeable - noop\n",
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		     inode, page, (void *)page->private);
=======
		     inode, page, snapc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		     inode, page, snapc);
>>>>>>> refs/remotes/origin/master
=======
		     inode, page, snapc);
>>>>>>> refs/remotes/origin/cm-11.0
		/* we should only noop if called by kswapd */
		WARN_ON((current->flags & PF_MEMALLOC) == 0);
		ceph_put_snap_context(oldest);
		goto out;
	}
	ceph_put_snap_context(oldest);

<<<<<<< HEAD
	/* is this a partial page at end of file? */
	if (snap_size)
		i_size = snap_size;
	else
		i_size = i_size_read(inode);
	if (i_size < page_off + len)
		len = i_size - page_off;
=======
	spin_lock(&ci->i_ceph_lock);
	truncate_seq = ci->i_truncate_seq;
	truncate_size = ci->i_truncate_size;
	if (!snap_size)
		snap_size = i_size_read(inode);
	spin_unlock(&ci->i_ceph_lock);

	/* is this a partial page at end of file? */
	if (page_off >= snap_size) {
		dout("%p page eof %llu\n", page, snap_size);
		goto out;
	}
	if (snap_size < page_off + len)
		len = snap_size - page_off;
>>>>>>> refs/remotes/origin/master

	dout("writepage %p page %p index %lu on %llu~%u snapc %p\n",
	     inode, page, page->index, page_off, len, snapc);

	writeback_stat = atomic_long_inc_return(&fsc->writeback_count);
	if (writeback_stat >
	    CONGESTION_ON_THRESH(fsc->mount_options->congestion_kb))
		set_bdi_congested(&fsc->backing_dev_info, BLK_RW_ASYNC);

<<<<<<< HEAD
=======
	ceph_readpage_to_fscache(inode, page);

>>>>>>> refs/remotes/origin/master
	set_page_writeback(page);
	err = ceph_osdc_writepages(osdc, ceph_vino(inode),
				   &ci->i_layout, snapc,
				   page_off, len,
<<<<<<< HEAD
				   ci->i_truncate_seq, ci->i_truncate_size,
				   &inode->i_mtime,
				   &page, 1, 0, 0, true);
=======
				   truncate_seq, truncate_size,
				   &inode->i_mtime, &page, 1);
>>>>>>> refs/remotes/origin/master
	if (err < 0) {
		dout("writepage setting page/mapping error %d %p\n", err, page);
		SetPageError(page);
		mapping_set_error(&inode->i_data, err);
		if (wbc)
			wbc->pages_skipped++;
	} else {
		dout("writepage cleaned page %p\n", page);
		err = 0;  /* vfs expects us to return 0 */
	}
	page->private = 0;
	ClearPagePrivate(page);
	end_page_writeback(page);
	ceph_put_wrbuffer_cap_refs(ci, 1, snapc);
	ceph_put_snap_context(snapc);  /* page's reference */
out:
	return err;
}

static int ceph_writepage(struct page *page, struct writeback_control *wbc)
{
	int err;
	struct inode *inode = page->mapping->host;
	BUG_ON(!inode);
	ihold(inode);
	err = writepage_nounlock(page, wbc);
	unlock_page(page);
	iput(inode);
	return err;
}


/*
 * lame release_pages helper.  release_pages() isn't exported to
 * modules.
 */
static void ceph_release_pages(struct page **pages, int num)
{
	struct pagevec pvec;
	int i;

	pagevec_init(&pvec, 0);
	for (i = 0; i < num; i++) {
		if (pagevec_add(&pvec, pages[i]) == 0)
			pagevec_release(&pvec);
	}
	pagevec_release(&pvec);
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/*
 * async writeback completion handler.
 *
 * If we get an error, set the mapping error bit, but not the individual
 * page error bits.
 */
static void writepages_finish(struct ceph_osd_request *req,
			      struct ceph_msg *msg)
{
	struct inode *inode = req->r_inode;
<<<<<<< HEAD
	struct ceph_osd_reply_head *replyhead;
	struct ceph_osd_op *op;
	struct ceph_inode_info *ci = ceph_inode(inode);
	unsigned wrote;
	struct page *page;
	int i;
	struct ceph_snap_context *snapc = req->r_snapc;
	struct address_space *mapping = inode->i_mapping;
	__s32 rc = -EIO;
	u64 bytes = 0;
=======
	struct ceph_inode_info *ci = ceph_inode(inode);
	struct ceph_osd_data *osd_data;
	unsigned wrote;
	struct page *page;
	int num_pages;
	int i;
	struct ceph_snap_context *snapc = req->r_snapc;
	struct address_space *mapping = inode->i_mapping;
	int rc = req->r_result;
	u64 bytes = req->r_ops[0].extent.length;
>>>>>>> refs/remotes/origin/master
	struct ceph_fs_client *fsc = ceph_inode_to_client(inode);
	long writeback_stat;
	unsigned issued = ceph_caps_issued(ci);

<<<<<<< HEAD
	/* parse reply */
	replyhead = msg->front.iov_base;
	WARN_ON(le32_to_cpu(replyhead->num_ops) == 0);
	op = (void *)(replyhead + 1);
	rc = le32_to_cpu(replyhead->result);
	bytes = le64_to_cpu(op->extent.length);

=======
	osd_data = osd_req_op_extent_osd_data(req, 0);
	BUG_ON(osd_data->type != CEPH_OSD_DATA_TYPE_PAGES);
	num_pages = calc_pages_for((u64)osd_data->alignment,
					(u64)osd_data->length);
>>>>>>> refs/remotes/origin/master
	if (rc >= 0) {
		/*
		 * Assume we wrote the pages we originally sent.  The
		 * osd might reply with fewer pages if our writeback
		 * raced with a truncation and was adjusted at the osd,
		 * so don't believe the reply.
		 */
<<<<<<< HEAD
		wrote = req->r_num_pages;
=======
		wrote = num_pages;
>>>>>>> refs/remotes/origin/master
	} else {
		wrote = 0;
		mapping_set_error(mapping, rc);
	}
	dout("writepages_finish %p rc %d bytes %llu wrote %d (pages)\n",
	     inode, rc, bytes, wrote);

	/* clean all pages */
<<<<<<< HEAD
	for (i = 0; i < req->r_num_pages; i++) {
		page = req->r_pages[i];
=======
	for (i = 0; i < num_pages; i++) {
		page = osd_data->pages[i];
>>>>>>> refs/remotes/origin/master
		BUG_ON(!page);
		WARN_ON(!PageUptodate(page));

		writeback_stat =
			atomic_long_dec_return(&fsc->writeback_count);
		if (writeback_stat <
		    CONGESTION_OFF_THRESH(fsc->mount_options->congestion_kb))
			clear_bdi_congested(&fsc->backing_dev_info,
					    BLK_RW_ASYNC);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		ceph_put_snap_context((void *)page->private);
=======
		ceph_put_snap_context(page_snap_context(page));
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ceph_put_snap_context(page_snap_context(page));
>>>>>>> refs/remotes/origin/master
=======
		ceph_put_snap_context(page_snap_context(page));
>>>>>>> refs/remotes/origin/cm-11.0
		page->private = 0;
		ClearPagePrivate(page);
		dout("unlocking %d %p\n", i, page);
		end_page_writeback(page);

		/*
		 * We lost the cache cap, need to truncate the page before
		 * it is unlocked, otherwise we'd truncate it later in the
		 * page truncation thread, possibly losing some data that
		 * raced its way in
		 */
		if ((issued & (CEPH_CAP_FILE_CACHE|CEPH_CAP_FILE_LAZYIO)) == 0)
			generic_error_remove_page(inode->i_mapping, page);

		unlock_page(page);
	}
	dout("%p wrote+cleaned %d pages\n", inode, wrote);
<<<<<<< HEAD
	ceph_put_wrbuffer_cap_refs(ci, req->r_num_pages, snapc);

	ceph_release_pages(req->r_pages, req->r_num_pages);
	if (req->r_pages_from_pool)
		mempool_free(req->r_pages,
			     ceph_sb_to_client(inode->i_sb)->wb_pagevec_pool);
	else
		kfree(req->r_pages);
=======
	ceph_put_wrbuffer_cap_refs(ci, num_pages, snapc);

	ceph_release_pages(osd_data->pages, num_pages);
	if (osd_data->pages_from_pool)
		mempool_free(osd_data->pages,
			     ceph_sb_to_client(inode->i_sb)->wb_pagevec_pool);
	else
		kfree(osd_data->pages);
>>>>>>> refs/remotes/origin/master
	ceph_osdc_put_request(req);
}

/*
<<<<<<< HEAD
 * allocate a page vec, either directly, or if necessary, via a the
 * mempool.  we avoid the mempool if we can because req->r_num_pages
 * may be less than the maximum write size.
 */
static void alloc_page_vec(struct ceph_fs_client *fsc,
			   struct ceph_osd_request *req)
{
	req->r_pages = kmalloc(sizeof(struct page *) * req->r_num_pages,
			       GFP_NOFS);
	if (!req->r_pages) {
		req->r_pages = mempool_alloc(fsc->wb_pagevec_pool, GFP_NOFS);
		req->r_pages_from_pool = 1;
		WARN_ON(!req->r_pages);
	}
}

/*
=======
>>>>>>> refs/remotes/origin/master
 * initiate async writeback
 */
static int ceph_writepages_start(struct address_space *mapping,
				 struct writeback_control *wbc)
{
	struct inode *inode = mapping->host;
	struct ceph_inode_info *ci = ceph_inode(inode);
<<<<<<< HEAD
	struct ceph_fs_client *fsc;
=======
	struct ceph_fs_client *fsc = ceph_inode_to_client(inode);
	struct ceph_vino vino = ceph_vino(inode);
>>>>>>> refs/remotes/origin/master
	pgoff_t index, start, end;
	int range_whole = 0;
	int should_loop = 1;
	pgoff_t max_pages = 0, max_pages_ever = 0;
	struct ceph_snap_context *snapc = NULL, *last_snapc = NULL, *pgsnapc;
	struct pagevec pvec;
	int done = 0;
	int rc = 0;
	unsigned wsize = 1 << inode->i_blkbits;
	struct ceph_osd_request *req = NULL;
	int do_sync;
<<<<<<< HEAD
	u64 snap_size = 0;
=======
	u64 truncate_size, snap_size;
	u32 truncate_seq;
>>>>>>> refs/remotes/origin/master

	/*
	 * Include a 'sync' in the OSD request if this is a data
	 * integrity write (e.g., O_SYNC write or fsync()), or if our
	 * cap is being revoked.
	 */
<<<<<<< HEAD
	do_sync = wbc->sync_mode == WB_SYNC_ALL;
	if (ceph_caps_revoking(ci, CEPH_CAP_FILE_BUFFER))
=======
	if ((wbc->sync_mode == WB_SYNC_ALL) ||
		ceph_caps_revoking(ci, CEPH_CAP_FILE_BUFFER))
>>>>>>> refs/remotes/origin/master
		do_sync = 1;
	dout("writepages_start %p dosync=%d (mode=%s)\n",
	     inode, do_sync,
	     wbc->sync_mode == WB_SYNC_NONE ? "NONE" :
	     (wbc->sync_mode == WB_SYNC_ALL ? "ALL" : "HOLD"));

<<<<<<< HEAD
	fsc = ceph_inode_to_client(inode);
=======
>>>>>>> refs/remotes/origin/master
	if (fsc->mount_state == CEPH_MOUNT_SHUTDOWN) {
		pr_warning("writepage_start %p on forced umount\n", inode);
		return -EIO; /* we're in a forced umount, don't write! */
	}
	if (fsc->mount_options->wsize && fsc->mount_options->wsize < wsize)
		wsize = fsc->mount_options->wsize;
	if (wsize < PAGE_CACHE_SIZE)
		wsize = PAGE_CACHE_SIZE;
	max_pages_ever = wsize >> PAGE_CACHE_SHIFT;

	pagevec_init(&pvec, 0);

	/* where to start/end? */
	if (wbc->range_cyclic) {
		start = mapping->writeback_index; /* Start from prev offset */
		end = -1;
		dout(" cyclic, start at %lu\n", start);
	} else {
		start = wbc->range_start >> PAGE_CACHE_SHIFT;
		end = wbc->range_end >> PAGE_CACHE_SHIFT;
		if (wbc->range_start == 0 && wbc->range_end == LLONG_MAX)
			range_whole = 1;
		should_loop = 0;
		dout(" not cyclic, %lu to %lu\n", start, end);
	}
	index = start;

retry:
	/* find oldest snap context with dirty data */
	ceph_put_snap_context(snapc);
<<<<<<< HEAD
=======
	snap_size = 0;
>>>>>>> refs/remotes/origin/master
	snapc = get_oldest_context(inode, &snap_size);
	if (!snapc) {
		/* hmm, why does writepages get called when there
		   is no dirty data? */
		dout(" no snap context with dirty data?\n");
		goto out;
	}
<<<<<<< HEAD
	dout(" oldest snapc is %p seq %lld (%d snaps)\n",
	     snapc, snapc->seq, snapc->num_snaps);
=======
	if (snap_size == 0)
		snap_size = i_size_read(inode);
	dout(" oldest snapc is %p seq %lld (%d snaps)\n",
	     snapc, snapc->seq, snapc->num_snaps);

	spin_lock(&ci->i_ceph_lock);
	truncate_seq = ci->i_truncate_seq;
	truncate_size = ci->i_truncate_size;
	if (!snap_size)
		snap_size = i_size_read(inode);
	spin_unlock(&ci->i_ceph_lock);

>>>>>>> refs/remotes/origin/master
	if (last_snapc && snapc != last_snapc) {
		/* if we switched to a newer snapc, restart our scan at the
		 * start of the original file range. */
		dout("  snapc differs from last pass, restarting at %lu\n",
		     index);
		index = start;
	}
	last_snapc = snapc;

	while (!done && index <= end) {
<<<<<<< HEAD
=======
		int num_ops = do_sync ? 2 : 1;
>>>>>>> refs/remotes/origin/master
		unsigned i;
		int first;
		pgoff_t next;
		int pvec_pages, locked_pages;
<<<<<<< HEAD
		struct page *page;
		int want;
		u64 offset, len;
		struct ceph_osd_request_head *reqhead;
		struct ceph_osd_op *op;
=======
		struct page **pages = NULL;
		mempool_t *pool = NULL;	/* Becomes non-null if mempool used */
		struct page *page;
		int want;
		u64 offset, len;
>>>>>>> refs/remotes/origin/master
		long writeback_stat;

		next = 0;
		locked_pages = 0;
		max_pages = max_pages_ever;

get_more_pages:
		first = -1;
		want = min(end - index,
			   min((pgoff_t)PAGEVEC_SIZE,
			       max_pages - (pgoff_t)locked_pages) - 1)
			+ 1;
		pvec_pages = pagevec_lookup_tag(&pvec, mapping, &index,
						PAGECACHE_TAG_DIRTY,
						want);
		dout("pagevec_lookup_tag got %d\n", pvec_pages);
		if (!pvec_pages && !locked_pages)
			break;
		for (i = 0; i < pvec_pages && locked_pages < max_pages; i++) {
			page = pvec.pages[i];
			dout("? %p idx %lu\n", page, page->index);
			if (locked_pages == 0)
				lock_page(page);  /* first page */
			else if (!trylock_page(page))
				break;

			/* only dirty pages, or our accounting breaks */
			if (unlikely(!PageDirty(page)) ||
			    unlikely(page->mapping != mapping)) {
				dout("!dirty or !mapping %p\n", page);
				unlock_page(page);
				break;
			}
			if (!wbc->range_cyclic && page->index > end) {
				dout("end of range %p\n", page);
				done = 1;
				unlock_page(page);
				break;
			}
			if (next && (page->index != next)) {
				dout("not consecutive %p\n", page);
				unlock_page(page);
				break;
			}
			if (wbc->sync_mode != WB_SYNC_NONE) {
				dout("waiting on writeback %p\n", page);
				wait_on_page_writeback(page);
			}
<<<<<<< HEAD
			if ((snap_size && page_offset(page) > snap_size) ||
			    (!snap_size &&
			     page_offset(page) > i_size_read(inode))) {
				dout("%p page eof %llu\n", page, snap_size ?
				     snap_size : i_size_read(inode));
=======
			if (page_offset(page) >= snap_size) {
				dout("%p page eof %llu\n", page, snap_size);
>>>>>>> refs/remotes/origin/master
				done = 1;
				unlock_page(page);
				break;
			}
			if (PageWriteback(page)) {
				dout("%p under writeback\n", page);
				unlock_page(page);
				break;
			}

			/* only if matching snap context */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
			pgsnapc = (void *)page->private;
=======
			pgsnapc = page_snap_context(page);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pgsnapc = page_snap_context(page);
>>>>>>> refs/remotes/origin/master
=======
			pgsnapc = page_snap_context(page);
>>>>>>> refs/remotes/origin/cm-11.0
			if (pgsnapc->seq > snapc->seq) {
				dout("page snapc %p %lld > oldest %p %lld\n",
				     pgsnapc, pgsnapc->seq, snapc, snapc->seq);
				unlock_page(page);
				if (!locked_pages)
					continue; /* keep looking for snap */
				break;
			}

			if (!clear_page_dirty_for_io(page)) {
				dout("%p !clear_page_dirty_for_io\n", page);
				unlock_page(page);
				break;
			}

<<<<<<< HEAD
			/* ok */
			if (locked_pages == 0) {
				/* prepare async write request */
<<<<<<< HEAD
<<<<<<< HEAD
				offset = (unsigned long long)page->index
					<< PAGE_CACHE_SHIFT;
=======
				offset = (u64) page_offset(page);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				offset = (u64) page_offset(page);
>>>>>>> refs/remotes/origin/cm-11.0
				len = wsize;
				req = ceph_osdc_new_request(&fsc->client->osdc,
					    &ci->i_layout,
					    ceph_vino(inode),
					    offset, &len,
					    CEPH_OSD_OP_WRITE,
					    CEPH_OSD_FLAG_WRITE |
						    CEPH_OSD_FLAG_ONDISK,
					    snapc, do_sync,
					    ci->i_truncate_seq,
					    ci->i_truncate_size,
					    &inode->i_mtime, true, 1, 0);

<<<<<<< HEAD
<<<<<<< HEAD
				if (!req) {
					rc = -ENOMEM;
=======
				if (IS_ERR(req)) {
					rc = PTR_ERR(req);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			/*
			 * We have something to write.  If this is
			 * the first locked page this time through,
			 * allocate an osd request and a page array
			 * that it will use.
			 */
			if (locked_pages == 0) {
				BUG_ON(pages);
				/* prepare async write request */
				offset = (u64)page_offset(page);
				len = wsize;
				req = ceph_osdc_new_request(&fsc->client->osdc,
							&ci->i_layout, vino,
							offset, &len, num_ops,
							CEPH_OSD_OP_WRITE,
							CEPH_OSD_FLAG_WRITE |
							CEPH_OSD_FLAG_ONDISK,
							snapc, truncate_seq,
							truncate_size, true);
				if (IS_ERR(req)) {
					rc = PTR_ERR(req);
>>>>>>> refs/remotes/origin/master
=======
				if (IS_ERR(req)) {
					rc = PTR_ERR(req);
>>>>>>> refs/remotes/origin/cm-11.0
					unlock_page(page);
					break;
				}

<<<<<<< HEAD
				max_pages = req->r_num_pages;

				alloc_page_vec(fsc, req);
				req->r_callback = writepages_finish;
				req->r_inode = inode;
=======
				req->r_callback = writepages_finish;
				req->r_inode = inode;

				max_pages = calc_pages_for(0, (u64)len);
				pages = kmalloc(max_pages * sizeof (*pages),
						GFP_NOFS);
				if (!pages) {
					pool = fsc->wb_pagevec_pool;
					pages = mempool_alloc(pool, GFP_NOFS);
					BUG_ON(!pages);
				}
>>>>>>> refs/remotes/origin/master
			}

			/* note position of first page in pvec */
			if (first < 0)
				first = i;
			dout("%p will write page %p idx %lu\n",
			     inode, page, page->index);

			writeback_stat =
			       atomic_long_inc_return(&fsc->writeback_count);
			if (writeback_stat > CONGESTION_ON_THRESH(
				    fsc->mount_options->congestion_kb)) {
				set_bdi_congested(&fsc->backing_dev_info,
						  BLK_RW_ASYNC);
			}

			set_page_writeback(page);
<<<<<<< HEAD
			req->r_pages[locked_pages] = page;
=======
			pages[locked_pages] = page;
>>>>>>> refs/remotes/origin/master
			locked_pages++;
			next = page->index + 1;
		}

		/* did we get anything? */
		if (!locked_pages)
			goto release_pvec_pages;
		if (i) {
			int j;
			BUG_ON(!locked_pages || first < 0);

			if (pvec_pages && i == pvec_pages &&
			    locked_pages < max_pages) {
				dout("reached end pvec, trying for more\n");
				pagevec_reinit(&pvec);
				goto get_more_pages;
			}

			/* shift unused pages over in the pvec...  we
			 * will need to release them below. */
			for (j = i; j < pvec_pages; j++) {
				dout(" pvec leftover page %p\n",
				     pvec.pages[j]);
				pvec.pages[j-i+first] = pvec.pages[j];
			}
			pvec.nr -= i-first;
		}

<<<<<<< HEAD
		/* submit the write */
		offset = req->r_pages[0]->index << PAGE_CACHE_SHIFT;
		len = min((snap_size ? snap_size : i_size_read(inode)) - offset,
=======
		/* Format the osd request message and submit the write */

		offset = page_offset(pages[0]);
		len = min(snap_size - offset,
>>>>>>> refs/remotes/origin/master
			  (u64)locked_pages << PAGE_CACHE_SHIFT);
		dout("writepages got %d pages at %llu~%llu\n",
		     locked_pages, offset, len);

<<<<<<< HEAD
		/* revise final length, page count */
		req->r_num_pages = locked_pages;
		reqhead = req->r_request->front.iov_base;
		op = (void *)(reqhead + 1);
		op->extent.length = cpu_to_le64(len);
		op->payload_len = cpu_to_le32(len);
		req->r_request->hdr.data_len = cpu_to_le32(len);
=======
		osd_req_op_extent_osd_data_pages(req, 0, pages, len, 0,
							!!pool, false);

		pages = NULL;	/* request message now owns the pages array */
		pool = NULL;

		/* Update the write op length in case we changed it */

		osd_req_op_extent_update(req, 0, len);

		vino = ceph_vino(inode);
		ceph_osdc_build_request(req, offset, snapc, vino.snap,
					&inode->i_mtime);
>>>>>>> refs/remotes/origin/master

		rc = ceph_osdc_start_request(&fsc->client->osdc, req, true);
		BUG_ON(rc);
		req = NULL;

		/* continue? */
		index = next;
		wbc->nr_to_write -= locked_pages;
		if (wbc->nr_to_write <= 0)
			done = 1;

release_pvec_pages:
		dout("pagevec_release on %d pages (%p)\n", (int)pvec.nr,
		     pvec.nr ? pvec.pages[0] : NULL);
		pagevec_release(&pvec);

		if (locked_pages && !done)
			goto retry;
	}

	if (should_loop && !done) {
		/* more to do; loop back to beginning of file */
		dout("writepages looping back to beginning of file\n");
		should_loop = 0;
		index = 0;
		goto retry;
	}

	if (wbc->range_cyclic || (range_whole && wbc->nr_to_write > 0))
		mapping->writeback_index = index;

out:
	if (req)
		ceph_osdc_put_request(req);
	ceph_put_snap_context(snapc);
	dout("writepages done, rc = %d\n", rc);
	return rc;
}



/*
 * See if a given @snapc is either writeable, or already written.
 */
static int context_is_writeable_or_written(struct inode *inode,
					   struct ceph_snap_context *snapc)
{
	struct ceph_snap_context *oldest = get_oldest_context(inode, NULL);
	int ret = !oldest || snapc->seq <= oldest->seq;

	ceph_put_snap_context(oldest);
	return ret;
}

/*
 * We are only allowed to write into/dirty the page if the page is
 * clean, or already dirty within the same snap context.
 *
 * called with page locked.
 * return success with page locked,
 * or any failure (incl -EAGAIN) with page unlocked.
 */
static int ceph_update_writeable_page(struct file *file,
			    loff_t pos, unsigned len,
			    struct page *page)
{
<<<<<<< HEAD
	struct inode *inode = file->f_dentry->d_inode;
=======
	struct inode *inode = file_inode(file);
>>>>>>> refs/remotes/origin/master
	struct ceph_inode_info *ci = ceph_inode(inode);
	struct ceph_mds_client *mdsc = ceph_inode_to_client(inode)->mdsc;
	loff_t page_off = pos & PAGE_CACHE_MASK;
	int pos_in_page = pos & ~PAGE_CACHE_MASK;
	int end_in_page = pos_in_page + len;
	loff_t i_size;
	int r;
	struct ceph_snap_context *snapc, *oldest;

retry_locked:
	/* writepages currently holds page lock, but if we change that later, */
	wait_on_page_writeback(page);

	/* check snap context */
	BUG_ON(!ci->i_snap_realm);
	down_read(&mdsc->snap_rwsem);
	BUG_ON(!ci->i_snap_realm->cached_context);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	snapc = (void *)page->private;
=======
	snapc = page_snap_context(page);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	snapc = page_snap_context(page);
>>>>>>> refs/remotes/origin/master
=======
	snapc = page_snap_context(page);
>>>>>>> refs/remotes/origin/cm-11.0
	if (snapc && snapc != ci->i_head_snapc) {
		/*
		 * this page is already dirty in another (older) snap
		 * context!  is it writeable now?
		 */
		oldest = get_oldest_context(inode, NULL);
		up_read(&mdsc->snap_rwsem);

		if (snapc->seq > oldest->seq) {
			ceph_put_snap_context(oldest);
			dout(" page %p snapc %p not current or oldest\n",
			     page, snapc);
			/*
			 * queue for writeback, and wait for snapc to
			 * be writeable or written
			 */
			snapc = ceph_get_snap_context(snapc);
			unlock_page(page);
			ceph_queue_writeback(inode);
			r = wait_event_interruptible(ci->i_cap_wq,
			       context_is_writeable_or_written(inode, snapc));
			ceph_put_snap_context(snapc);
			if (r == -ERESTARTSYS)
				return r;
			return -EAGAIN;
		}
		ceph_put_snap_context(oldest);

		/* yay, writeable, do it now (without dropping page lock) */
		dout(" page %p snapc %p not current, but oldest\n",
		     page, snapc);
		if (!clear_page_dirty_for_io(page))
			goto retry_locked;
		r = writepage_nounlock(page, NULL);
		if (r < 0)
			goto fail_nosnap;
		goto retry_locked;
	}

	if (PageUptodate(page)) {
		dout(" page %p already uptodate\n", page);
		return 0;
	}

	/* full page? */
	if (pos_in_page == 0 && len == PAGE_CACHE_SIZE)
		return 0;

	/* past end of file? */
	i_size = inode->i_size;   /* caller holds i_mutex */

	if (i_size + len > inode->i_sb->s_maxbytes) {
		/* file is too big */
		r = -EINVAL;
		goto fail;
	}

	if (page_off >= i_size ||
	    (pos_in_page == 0 && (pos+len) >= i_size &&
	     end_in_page - pos_in_page != PAGE_CACHE_SIZE)) {
		dout(" zeroing %p 0 - %d and %d - %d\n",
		     page, pos_in_page, end_in_page, (int)PAGE_CACHE_SIZE);
		zero_user_segments(page,
				   0, pos_in_page,
				   end_in_page, PAGE_CACHE_SIZE);
		return 0;
	}

	/* we need to read it. */
	up_read(&mdsc->snap_rwsem);
	r = readpage_nounlock(file, page);
	if (r < 0)
		goto fail_nosnap;
	goto retry_locked;

fail:
	up_read(&mdsc->snap_rwsem);
fail_nosnap:
	unlock_page(page);
	return r;
}

/*
 * We are only allowed to write into/dirty the page if the page is
 * clean, or already dirty within the same snap context.
 */
static int ceph_write_begin(struct file *file, struct address_space *mapping,
			    loff_t pos, unsigned len, unsigned flags,
			    struct page **pagep, void **fsdata)
{
<<<<<<< HEAD
	struct inode *inode = file->f_dentry->d_inode;
=======
	struct inode *inode = file_inode(file);
>>>>>>> refs/remotes/origin/master
	struct page *page;
	pgoff_t index = pos >> PAGE_CACHE_SHIFT;
	int r;

	do {
		/* get a page */
		page = grab_cache_page_write_begin(mapping, index, 0);
		if (!page)
			return -ENOMEM;
		*pagep = page;

		dout("write_begin file %p inode %p page %p %d~%d\n", file,
		     inode, page, (int)pos, (int)len);

		r = ceph_update_writeable_page(file, pos, len, page);
	} while (r == -EAGAIN);

	return r;
}

/*
 * we don't do anything in here that simple_write_end doesn't do
 * except adjust dirty page accounting and drop read lock on
 * mdsc->snap_rwsem.
 */
static int ceph_write_end(struct file *file, struct address_space *mapping,
			  loff_t pos, unsigned len, unsigned copied,
			  struct page *page, void *fsdata)
{
<<<<<<< HEAD
	struct inode *inode = file->f_dentry->d_inode;
=======
	struct inode *inode = file_inode(file);
>>>>>>> refs/remotes/origin/master
	struct ceph_fs_client *fsc = ceph_inode_to_client(inode);
	struct ceph_mds_client *mdsc = fsc->mdsc;
	unsigned from = pos & (PAGE_CACHE_SIZE - 1);
	int check_cap = 0;

	dout("write_end file %p inode %p page %p %d~%d (%d)\n", file,
	     inode, page, (int)pos, (int)copied, (int)len);

	/* zero the stale part of the page if we did a short copy */
	if (copied < len)
		zero_user_segment(page, from+copied, len);

	/* did file size increase? */
	/* (no need for i_size_read(); we caller holds i_mutex */
	if (pos+copied > inode->i_size)
		check_cap = ceph_inode_set_size(inode, pos+copied);

	if (!PageUptodate(page))
		SetPageUptodate(page);

	set_page_dirty(page);

	unlock_page(page);
	up_read(&mdsc->snap_rwsem);
	page_cache_release(page);

	if (check_cap)
		ceph_check_caps(ceph_inode(inode), CHECK_CAPS_AUTHONLY, NULL);

	return copied;
}

/*
 * we set .direct_IO to indicate direct io is supported, but since we
 * intercept O_DIRECT reads and writes early, this function should
 * never get called.
 */
static ssize_t ceph_direct_io(int rw, struct kiocb *iocb,
			      const struct iovec *iov,
			      loff_t pos, unsigned long nr_segs)
{
	WARN_ON(1);
	return -EINVAL;
}

const struct address_space_operations ceph_aops = {
	.readpage = ceph_readpage,
	.readpages = ceph_readpages,
	.writepage = ceph_writepage,
	.writepages = ceph_writepages_start,
	.write_begin = ceph_write_begin,
	.write_end = ceph_write_end,
	.set_page_dirty = ceph_set_page_dirty,
	.invalidatepage = ceph_invalidatepage,
	.releasepage = ceph_releasepage,
	.direct_IO = ceph_direct_io,
};


/*
 * vm ops
 */

/*
 * Reuse write_begin here for simplicity.
 */
static int ceph_page_mkwrite(struct vm_area_struct *vma, struct vm_fault *vmf)
{
<<<<<<< HEAD
	struct inode *inode = vma->vm_file->f_dentry->d_inode;
	struct page *page = vmf->page;
	struct ceph_mds_client *mdsc = ceph_inode_to_client(inode)->mdsc;
<<<<<<< HEAD
<<<<<<< HEAD
	loff_t off = page->index << PAGE_CACHE_SHIFT;
=======
	loff_t off = page_offset(page);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	loff_t off = page_offset(page);
>>>>>>> refs/remotes/origin/cm-11.0
	loff_t size, len;
	int ret;

=======
	struct inode *inode = file_inode(vma->vm_file);
	struct page *page = vmf->page;
	struct ceph_mds_client *mdsc = ceph_inode_to_client(inode)->mdsc;
	loff_t off = page_offset(page);
	loff_t size, len;
	int ret;

	/* Update time before taking page lock */
	file_update_time(vma->vm_file);

>>>>>>> refs/remotes/origin/master
	size = i_size_read(inode);
	if (off + PAGE_CACHE_SIZE <= size)
		len = PAGE_CACHE_SIZE;
	else
		len = size & ~PAGE_CACHE_MASK;

	dout("page_mkwrite %p %llu~%llu page %p idx %lu\n", inode,
	     off, len, page, page->index);

	lock_page(page);

	ret = VM_FAULT_NOPAGE;
	if ((off > size) ||
	    (page->mapping != inode->i_mapping))
		goto out;

	ret = ceph_update_writeable_page(vma->vm_file, off, len, page);
	if (ret == 0) {
		/* success.  we'll keep the page locked. */
		set_page_dirty(page);
		up_read(&mdsc->snap_rwsem);
		ret = VM_FAULT_LOCKED;
	} else {
		if (ret == -ENOMEM)
			ret = VM_FAULT_OOM;
		else
			ret = VM_FAULT_SIGBUS;
	}
out:
	dout("page_mkwrite %p %llu~%llu = %d\n", inode, off, len, ret);
	if (ret != VM_FAULT_LOCKED)
		unlock_page(page);
	return ret;
}

static struct vm_operations_struct ceph_vmops = {
	.fault		= filemap_fault,
	.page_mkwrite	= ceph_page_mkwrite,
<<<<<<< HEAD
=======
	.remap_pages	= generic_file_remap_pages,
>>>>>>> refs/remotes/origin/master
};

int ceph_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct address_space *mapping = file->f_mapping;

	if (!mapping->a_ops->readpage)
		return -ENOEXEC;
	file_accessed(file);
	vma->vm_ops = &ceph_vmops;
<<<<<<< HEAD
	vma->vm_flags |= VM_CAN_NONLINEAR;
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}
