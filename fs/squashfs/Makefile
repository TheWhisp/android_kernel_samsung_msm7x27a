#
# Makefile for the linux squashfs routines.
#

obj-$(CONFIG_SQUASHFS) += squashfs.o
squashfs-y += block.o cache.o dir.o export.o file.o fragment.o id.o inode.o
<<<<<<< HEAD
<<<<<<< HEAD
squashfs-y += namei.o super.o symlink.o zlib_wrapper.o decompressor.o
squashfs-$(CONFIG_SQUASHFS_XATTR) += xattr.o xattr_id.o
squashfs-$(CONFIG_SQUASHFS_LZO) += lzo_wrapper.o
squashfs-$(CONFIG_SQUASHFS_XZ) += xz_wrapper.o
=======
squashfs-y += namei.o super.o symlink.o decompressor.o
=======
squashfs-y += namei.o super.o symlink.o decompressor.o
squashfs-$(CONFIG_SQUASHFS_FILE_CACHE) += file_cache.o
squashfs-$(CONFIG_SQUASHFS_FILE_DIRECT) += file_direct.o page_actor.o
squashfs-$(CONFIG_SQUASHFS_DECOMP_SINGLE) += decompressor_single.o
squashfs-$(CONFIG_SQUASHFS_DECOMP_MULTI) += decompressor_multi.o
squashfs-$(CONFIG_SQUASHFS_DECOMP_MULTI_PERCPU) += decompressor_multi_percpu.o
>>>>>>> refs/remotes/origin/master
squashfs-$(CONFIG_SQUASHFS_XATTR) += xattr.o xattr_id.o
squashfs-$(CONFIG_SQUASHFS_LZO) += lzo_wrapper.o
squashfs-$(CONFIG_SQUASHFS_XZ) += xz_wrapper.o
squashfs-$(CONFIG_SQUASHFS_ZLIB) += zlib_wrapper.o
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
