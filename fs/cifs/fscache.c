/*
 *   fs/cifs/fscache.c - CIFS filesystem cache interface
 *
 *   Copyright (c) 2010 Novell, Inc.
 *   Author(s): Suresh Jayaraman <sjayaraman@suse.de>
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
#include "fscache.h"
#include "cifsglob.h"
#include "cifs_debug.h"
#include "cifs_fs_sb.h"

void cifs_fscache_get_client_cookie(struct TCP_Server_Info *server)
{
	server->fscache =
		fscache_acquire_cookie(cifs_fscache_netfs.primary_index,
<<<<<<< HEAD
				&cifs_fscache_server_index_def, server);
	cFYI(1, "%s: (0x%p/0x%p)", __func__, server,
			server->fscache);
=======
				&cifs_fscache_server_index_def, server, true);
	cifs_dbg(FYI, "%s: (0x%p/0x%p)\n",
		 __func__, server, server->fscache);
>>>>>>> refs/remotes/origin/master
}

void cifs_fscache_release_client_cookie(struct TCP_Server_Info *server)
{
<<<<<<< HEAD
	cFYI(1, "%s: (0x%p/0x%p)", __func__, server,
			server->fscache);
=======
	cifs_dbg(FYI, "%s: (0x%p/0x%p)\n",
		 __func__, server, server->fscache);
>>>>>>> refs/remotes/origin/master
	fscache_relinquish_cookie(server->fscache, 0);
	server->fscache = NULL;
}

void cifs_fscache_get_super_cookie(struct cifs_tcon *tcon)
{
	struct TCP_Server_Info *server = tcon->ses->server;

	tcon->fscache =
		fscache_acquire_cookie(server->fscache,
<<<<<<< HEAD
				&cifs_fscache_super_index_def, tcon);
	cFYI(1, "%s: (0x%p/0x%p)", __func__, server->fscache,
			tcon->fscache);
=======
				&cifs_fscache_super_index_def, tcon, true);
	cifs_dbg(FYI, "%s: (0x%p/0x%p)\n",
		 __func__, server->fscache, tcon->fscache);
>>>>>>> refs/remotes/origin/master
}

void cifs_fscache_release_super_cookie(struct cifs_tcon *tcon)
{
<<<<<<< HEAD
	cFYI(1, "%s: (0x%p)", __func__, tcon->fscache);
=======
	cifs_dbg(FYI, "%s: (0x%p)\n", __func__, tcon->fscache);
>>>>>>> refs/remotes/origin/master
	fscache_relinquish_cookie(tcon->fscache, 0);
	tcon->fscache = NULL;
}

static void cifs_fscache_enable_inode_cookie(struct inode *inode)
{
	struct cifsInodeInfo *cifsi = CIFS_I(inode);
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
	struct cifs_tcon *tcon = cifs_sb_master_tcon(cifs_sb);

	if (cifsi->fscache)
		return;

	if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_FSCACHE) {
		cifsi->fscache = fscache_acquire_cookie(tcon->fscache,
<<<<<<< HEAD
				&cifs_fscache_inode_object_def, cifsi);
		cFYI(1, "%s: got FH cookie (0x%p/0x%p)", __func__,
				tcon->fscache, cifsi->fscache);
=======
				&cifs_fscache_inode_object_def, cifsi, true);
		cifs_dbg(FYI, "%s: got FH cookie (0x%p/0x%p)\n",
			 __func__, tcon->fscache, cifsi->fscache);
>>>>>>> refs/remotes/origin/master
	}
}

void cifs_fscache_release_inode_cookie(struct inode *inode)
{
	struct cifsInodeInfo *cifsi = CIFS_I(inode);

	if (cifsi->fscache) {
<<<<<<< HEAD
		cFYI(1, "%s: (0x%p)", __func__, cifsi->fscache);
=======
		cifs_dbg(FYI, "%s: (0x%p)\n", __func__, cifsi->fscache);
>>>>>>> refs/remotes/origin/master
		fscache_relinquish_cookie(cifsi->fscache, 0);
		cifsi->fscache = NULL;
	}
}

static void cifs_fscache_disable_inode_cookie(struct inode *inode)
{
	struct cifsInodeInfo *cifsi = CIFS_I(inode);

	if (cifsi->fscache) {
<<<<<<< HEAD
		cFYI(1, "%s: (0x%p)", __func__, cifsi->fscache);
=======
		cifs_dbg(FYI, "%s: (0x%p)\n", __func__, cifsi->fscache);
>>>>>>> refs/remotes/origin/master
		fscache_uncache_all_inode_pages(cifsi->fscache, inode);
		fscache_relinquish_cookie(cifsi->fscache, 1);
		cifsi->fscache = NULL;
	}
}

void cifs_fscache_set_inode_cookie(struct inode *inode, struct file *filp)
{
	if ((filp->f_flags & O_ACCMODE) != O_RDONLY)
		cifs_fscache_disable_inode_cookie(inode);
	else
		cifs_fscache_enable_inode_cookie(inode);
}

void cifs_fscache_reset_inode_cookie(struct inode *inode)
{
	struct cifsInodeInfo *cifsi = CIFS_I(inode);
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
	struct fscache_cookie *old = cifsi->fscache;

	if (cifsi->fscache) {
		/* retire the current fscache cache and get a new one */
		fscache_relinquish_cookie(cifsi->fscache, 1);

		cifsi->fscache = fscache_acquire_cookie(
					cifs_sb_master_tcon(cifs_sb)->fscache,
					&cifs_fscache_inode_object_def,
<<<<<<< HEAD
					cifsi);
		cFYI(1, "%s: new cookie 0x%p oldcookie 0x%p",
				__func__, cifsi->fscache, old);
=======
					cifsi, true);
		cifs_dbg(FYI, "%s: new cookie 0x%p oldcookie 0x%p\n",
			 __func__, cifsi->fscache, old);
>>>>>>> refs/remotes/origin/master
	}
}

int cifs_fscache_release_page(struct page *page, gfp_t gfp)
{
	if (PageFsCache(page)) {
		struct inode *inode = page->mapping->host;
		struct cifsInodeInfo *cifsi = CIFS_I(inode);

<<<<<<< HEAD
		cFYI(1, "%s: (0x%p/0x%p)", __func__, page,
				cifsi->fscache);
=======
		cifs_dbg(FYI, "%s: (0x%p/0x%p)\n",
			 __func__, page, cifsi->fscache);
>>>>>>> refs/remotes/origin/master
		if (!fscache_maybe_release_page(cifsi->fscache, page, gfp))
			return 0;
	}

	return 1;
}

static void cifs_readpage_from_fscache_complete(struct page *page, void *ctx,
						int error)
{
<<<<<<< HEAD
	cFYI(1, "%s: (0x%p/%d)", __func__, page, error);
=======
	cifs_dbg(FYI, "%s: (0x%p/%d)\n", __func__, page, error);
>>>>>>> refs/remotes/origin/master
	if (!error)
		SetPageUptodate(page);
	unlock_page(page);
}

/*
 * Retrieve a page from FS-Cache
 */
int __cifs_readpage_from_fscache(struct inode *inode, struct page *page)
{
	int ret;

<<<<<<< HEAD
	cFYI(1, "%s: (fsc:%p, p:%p, i:0x%p", __func__,
			CIFS_I(inode)->fscache, page, inode);
=======
	cifs_dbg(FYI, "%s: (fsc:%p, p:%p, i:0x%p\n",
		 __func__, CIFS_I(inode)->fscache, page, inode);
>>>>>>> refs/remotes/origin/master
	ret = fscache_read_or_alloc_page(CIFS_I(inode)->fscache, page,
					 cifs_readpage_from_fscache_complete,
					 NULL,
					 GFP_KERNEL);
	switch (ret) {

	case 0: /* page found in fscache, read submitted */
<<<<<<< HEAD
		cFYI(1, "%s: submitted", __func__);
		return ret;
	case -ENOBUFS:	/* page won't be cached */
	case -ENODATA:	/* page not in cache */
		cFYI(1, "%s: %d", __func__, ret);
		return 1;

	default:
		cERROR(1, "unknown error ret = %d", ret);
=======
		cifs_dbg(FYI, "%s: submitted\n", __func__);
		return ret;
	case -ENOBUFS:	/* page won't be cached */
	case -ENODATA:	/* page not in cache */
		cifs_dbg(FYI, "%s: %d\n", __func__, ret);
		return 1;

	default:
		cifs_dbg(VFS, "unknown error ret = %d\n", ret);
>>>>>>> refs/remotes/origin/master
	}
	return ret;
}

/*
 * Retrieve a set of pages from FS-Cache
 */
int __cifs_readpages_from_fscache(struct inode *inode,
				struct address_space *mapping,
				struct list_head *pages,
				unsigned *nr_pages)
{
	int ret;

<<<<<<< HEAD
	cFYI(1, "%s: (0x%p/%u/0x%p)", __func__,
			CIFS_I(inode)->fscache, *nr_pages, inode);
=======
	cifs_dbg(FYI, "%s: (0x%p/%u/0x%p)\n",
		 __func__, CIFS_I(inode)->fscache, *nr_pages, inode);
>>>>>>> refs/remotes/origin/master
	ret = fscache_read_or_alloc_pages(CIFS_I(inode)->fscache, mapping,
					  pages, nr_pages,
					  cifs_readpage_from_fscache_complete,
					  NULL,
					  mapping_gfp_mask(mapping));
	switch (ret) {
	case 0:	/* read submitted to the cache for all pages */
<<<<<<< HEAD
		cFYI(1, "%s: submitted", __func__);
=======
		cifs_dbg(FYI, "%s: submitted\n", __func__);
>>>>>>> refs/remotes/origin/master
		return ret;

	case -ENOBUFS:	/* some pages are not cached and can't be */
	case -ENODATA:	/* some pages are not cached */
<<<<<<< HEAD
		cFYI(1, "%s: no page", __func__);
		return 1;

	default:
		cFYI(1, "unknown error ret = %d", ret);
=======
		cifs_dbg(FYI, "%s: no page\n", __func__);
		return 1;

	default:
		cifs_dbg(FYI, "unknown error ret = %d\n", ret);
>>>>>>> refs/remotes/origin/master
	}

	return ret;
}

void __cifs_readpage_to_fscache(struct inode *inode, struct page *page)
{
	int ret;

<<<<<<< HEAD
	cFYI(1, "%s: (fsc: %p, p: %p, i: %p)", __func__,
			CIFS_I(inode)->fscache, page, inode);
=======
	cifs_dbg(FYI, "%s: (fsc: %p, p: %p, i: %p)\n",
		 __func__, CIFS_I(inode)->fscache, page, inode);
>>>>>>> refs/remotes/origin/master
	ret = fscache_write_page(CIFS_I(inode)->fscache, page, GFP_KERNEL);
	if (ret != 0)
		fscache_uncache_page(CIFS_I(inode)->fscache, page);
}

<<<<<<< HEAD
=======
void __cifs_fscache_readpages_cancel(struct inode *inode, struct list_head *pages)
{
	cifs_dbg(FYI, "%s: (fsc: %p, i: %p)\n",
		 __func__, CIFS_I(inode)->fscache, inode);
	fscache_readpages_cancel(CIFS_I(inode)->fscache, pages);
}

>>>>>>> refs/remotes/origin/master
void __cifs_fscache_invalidate_page(struct page *page, struct inode *inode)
{
	struct cifsInodeInfo *cifsi = CIFS_I(inode);
	struct fscache_cookie *cookie = cifsi->fscache;

<<<<<<< HEAD
	cFYI(1, "%s: (0x%p/0x%p)", __func__, page, cookie);
=======
	cifs_dbg(FYI, "%s: (0x%p/0x%p)\n", __func__, page, cookie);
>>>>>>> refs/remotes/origin/master
	fscache_wait_on_page_write(cookie, page);
	fscache_uncache_page(cookie, page);
}

