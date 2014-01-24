/*
 * Squashfs - a compressed read only filesystem for Linux
 *
 * Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009
 * Phillip Lougher <phillip@squashfs.org.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * zlib_wrapper.c
 */


#include <linux/mutex.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include <linux/zlib.h>
#include <linux/vmalloc.h>

#include "squashfs_fs.h"
#include "squashfs_fs_sb.h"
#include "squashfs.h"
#include "decompressor.h"
<<<<<<< HEAD

static void *zlib_init(struct squashfs_sb_info *dummy, void *buff, int len)
=======
#include "page_actor.h"

static void *zlib_init(struct squashfs_sb_info *dummy, void *buff)
>>>>>>> refs/remotes/origin/master
{
	z_stream *stream = kmalloc(sizeof(z_stream), GFP_KERNEL);
	if (stream == NULL)
		goto failed;
	stream->workspace = vmalloc(zlib_inflate_workspacesize());
	if (stream->workspace == NULL)
		goto failed;

	return stream;

failed:
	ERROR("Failed to allocate zlib workspace\n");
	kfree(stream);
	return ERR_PTR(-ENOMEM);
}


static void zlib_free(void *strm)
{
	z_stream *stream = strm;

	if (stream)
		vfree(stream->workspace);
	kfree(stream);
}


<<<<<<< HEAD
static int zlib_uncompress(struct squashfs_sb_info *msblk, void **buffer,
	struct buffer_head **bh, int b, int offset, int length, int srclength,
	int pages)
{
	int zlib_err, zlib_init = 0;
	int k = 0, page = 0;
	z_stream *stream = msblk->stream;

	mutex_lock(&msblk->read_data_mutex);

	stream->avail_out = 0;
=======
static int zlib_uncompress(struct squashfs_sb_info *msblk, void *strm,
	struct buffer_head **bh, int b, int offset, int length,
	struct squashfs_page_actor *output)
{
	int zlib_err, zlib_init = 0, k = 0;
	z_stream *stream = strm;

	stream->avail_out = PAGE_CACHE_SIZE;
	stream->next_out = squashfs_first_page(output);
>>>>>>> refs/remotes/origin/master
	stream->avail_in = 0;

	do {
		if (stream->avail_in == 0 && k < b) {
			int avail = min(length, msblk->devblksize - offset);
			length -= avail;
<<<<<<< HEAD
			wait_on_buffer(bh[k]);
			if (!buffer_uptodate(bh[k]))
				goto release_mutex;

=======
>>>>>>> refs/remotes/origin/master
			stream->next_in = bh[k]->b_data + offset;
			stream->avail_in = avail;
			offset = 0;
		}

<<<<<<< HEAD
		if (stream->avail_out == 0 && page < pages) {
			stream->next_out = buffer[page++];
			stream->avail_out = PAGE_CACHE_SIZE;
=======
		if (stream->avail_out == 0) {
			stream->next_out = squashfs_next_page(output);
			if (stream->next_out != NULL)
				stream->avail_out = PAGE_CACHE_SIZE;
>>>>>>> refs/remotes/origin/master
		}

		if (!zlib_init) {
			zlib_err = zlib_inflateInit(stream);
			if (zlib_err != Z_OK) {
<<<<<<< HEAD
				ERROR("zlib_inflateInit returned unexpected "
					"result 0x%x, srclength %d\n",
					zlib_err, srclength);
				goto release_mutex;
=======
				squashfs_finish_page(output);
				goto out;
>>>>>>> refs/remotes/origin/master
			}
			zlib_init = 1;
		}

		zlib_err = zlib_inflate(stream, Z_SYNC_FLUSH);

		if (stream->avail_in == 0 && k < b)
			put_bh(bh[k++]);
	} while (zlib_err == Z_OK);

<<<<<<< HEAD
	if (zlib_err != Z_STREAM_END) {
		ERROR("zlib_inflate error, data probably corrupt\n");
		goto release_mutex;
	}

	zlib_err = zlib_inflateEnd(stream);
	if (zlib_err != Z_OK) {
		ERROR("zlib_inflate error, data probably corrupt\n");
		goto release_mutex;
	}

	if (k < b) {
		ERROR("zlib_uncompress error, data remaining\n");
		goto release_mutex;
	}

	length = stream->total_out;
	mutex_unlock(&msblk->read_data_mutex);
	return length;

release_mutex:
	mutex_unlock(&msblk->read_data_mutex);

=======
	squashfs_finish_page(output);

	if (zlib_err != Z_STREAM_END)
		goto out;

	zlib_err = zlib_inflateEnd(stream);
	if (zlib_err != Z_OK)
		goto out;

	if (k < b)
		goto out;

	return stream->total_out;

out:
>>>>>>> refs/remotes/origin/master
	for (; k < b; k++)
		put_bh(bh[k]);

	return -EIO;
}

const struct squashfs_decompressor squashfs_zlib_comp_ops = {
	.init = zlib_init,
	.free = zlib_free,
	.decompress = zlib_uncompress,
	.id = ZLIB_COMPRESSION,
	.name = "zlib",
	.supported = 1
};

