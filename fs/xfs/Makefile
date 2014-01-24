#
# Copyright (c) 2000-2005 Silicon Graphics, Inc.
# All Rights Reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it would be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write the Free Software Foundation,
# Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

<<<<<<< HEAD
<<<<<<< HEAD
ccflags-y := -I$(src) -I$(src)/linux-2.6
ccflags-$(CONFIG_XFS_DEBUG) += -g

XFS_LINUX := linux-2.6

obj-$(CONFIG_XFS_FS)		+= xfs.o

xfs-y				+= linux-2.6/xfs_trace.o

xfs-$(CONFIG_XFS_QUOTA)		+= $(addprefix quota/, \
				   xfs_dquot.o \
				   xfs_dquot_item.o \
				   xfs_trans_dquot.o \
				   xfs_qm_syscalls.o \
				   xfs_qm_bhv.o \
				   xfs_qm.o)
xfs-$(CONFIG_XFS_QUOTA)		+= linux-2.6/xfs_quotaops.o

ifeq ($(CONFIG_XFS_QUOTA),y)
xfs-$(CONFIG_PROC_FS)		+= quota/xfs_qm_stats.o
endif

xfs-$(CONFIG_XFS_RT)		+= xfs_rtalloc.o
xfs-$(CONFIG_XFS_POSIX_ACL)	+= $(XFS_LINUX)/xfs_acl.o
xfs-$(CONFIG_PROC_FS)		+= $(XFS_LINUX)/xfs_stats.o
xfs-$(CONFIG_SYSCTL)		+= $(XFS_LINUX)/xfs_sysctl.o
xfs-$(CONFIG_COMPAT)		+= $(XFS_LINUX)/xfs_ioctl32.o


=======
=======
>>>>>>> refs/remotes/origin/master
ccflags-y += -I$(src)			# needed for trace events

ccflags-$(CONFIG_XFS_DEBUG) += -g

obj-$(CONFIG_XFS_FS)		+= xfs.o

# this one should be compiled first, as the tracing macros can easily blow up
xfs-y				+= xfs_trace.o

# highlevel code
xfs-y				+= xfs_aops.o \
<<<<<<< HEAD
				   xfs_bit.o \
				   xfs_buf.o \
				   xfs_dfrag.o \
				   xfs_discard.o \
				   xfs_error.o \
				   xfs_export.o \
				   xfs_file.o \
				   xfs_filestream.o \
				   xfs_fsops.o \
				   xfs_fs_subr.o \
				   xfs_globals.o \
				   xfs_iget.o \
=======
				   xfs_attr_inactive.o \
				   xfs_attr_list.o \
				   xfs_bit.o \
				   xfs_bmap_util.o \
				   xfs_buf.o \
				   xfs_dir2_readdir.o \
				   xfs_discard.o \
				   xfs_error.o \
				   xfs_export.o \
				   xfs_extent_busy.o \
				   xfs_file.o \
				   xfs_filestream.o \
				   xfs_fsops.o \
				   xfs_globals.o \
				   xfs_icache.o \
>>>>>>> refs/remotes/origin/master
				   xfs_ioctl.o \
				   xfs_iomap.o \
				   xfs_iops.o \
				   xfs_itable.o \
				   xfs_message.o \
<<<<<<< HEAD
				   xfs_mru_cache.o \
				   xfs_super.o \
				   xfs_sync.o \
				   xfs_xattr.o \
				   xfs_rename.o \
				   xfs_rw.o \
				   xfs_utils.o \
				   xfs_vnodeops.o \
=======
				   xfs_mount.o \
				   xfs_mru_cache.o \
				   xfs_super.o \
				   xfs_symlink.o \
				   xfs_trans.o \
				   xfs_xattr.o \
>>>>>>> refs/remotes/origin/master
				   kmem.o \
				   uuid.o

# code shared with libxfs
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
xfs-y				+= xfs_alloc.o \
				   xfs_alloc_btree.o \
				   xfs_attr.o \
				   xfs_attr_leaf.o \
<<<<<<< HEAD
<<<<<<< HEAD
				   xfs_bit.o \
				   xfs_bmap.o \
				   xfs_bmap_btree.o \
				   xfs_btree.o \
				   xfs_buf_item.o \
=======
				   xfs_bmap.o \
				   xfs_bmap_btree.o \
				   xfs_btree.o \
>>>>>>> refs/remotes/origin/cm-10.0
				   xfs_da_btree.o \
=======
				   xfs_attr_remote.o \
				   xfs_bmap.o \
				   xfs_bmap_btree.o \
				   xfs_btree.o \
				   xfs_da_btree.o \
				   xfs_da_format.o \
>>>>>>> refs/remotes/origin/master
				   xfs_dir2.o \
				   xfs_dir2_block.o \
				   xfs_dir2_data.o \
				   xfs_dir2_leaf.o \
				   xfs_dir2_node.o \
				   xfs_dir2_sf.o \
<<<<<<< HEAD
<<<<<<< HEAD
				   xfs_error.o \
				   xfs_extfree_item.o \
				   xfs_filestream.o \
				   xfs_fsops.o \
				   xfs_ialloc.o \
				   xfs_ialloc_btree.o \
				   xfs_iget.o \
				   xfs_inode.o \
				   xfs_inode_item.o \
				   xfs_iomap.o \
				   xfs_itable.o \
				   xfs_dfrag.o \
				   xfs_log.o \
				   xfs_log_cil.o \
				   xfs_log_recover.o \
				   xfs_mount.o \
				   xfs_mru_cache.o \
				   xfs_rename.o \
				   xfs_trans.o \
=======
				   xfs_ialloc.o \
				   xfs_ialloc_btree.o \
				   xfs_inode.o \
				   xfs_log_recover.o \
				   xfs_mount.o \
				   xfs_trans.o
=======
				   xfs_dquot_buf.o \
				   xfs_ialloc.o \
				   xfs_ialloc_btree.o \
				   xfs_icreate_item.o \
				   xfs_inode.o \
				   xfs_inode_fork.o \
				   xfs_inode_buf.o \
				   xfs_log_recover.o \
				   xfs_log_rlimit.o \
				   xfs_sb.o \
				   xfs_symlink_remote.o \
				   xfs_trans_resv.o
>>>>>>> refs/remotes/origin/master

# low-level transaction/log code
xfs-y				+= xfs_log.o \
				   xfs_log_cil.o \
				   xfs_buf_item.o \
				   xfs_extfree_item.o \
				   xfs_inode_item.o \
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				   xfs_trans_ail.o \
				   xfs_trans_buf.o \
				   xfs_trans_extfree.o \
				   xfs_trans_inode.o \
<<<<<<< HEAD
<<<<<<< HEAD
				   xfs_utils.o \
				   xfs_vnodeops.o \
				   xfs_rw.o

xfs-$(CONFIG_XFS_TRACE)		+= xfs_btree_trace.o

# Objects in linux/
xfs-y				+= $(addprefix $(XFS_LINUX)/, \
				   kmem.o \
				   xfs_aops.o \
				   xfs_buf.o \
				   xfs_discard.o \
				   xfs_export.o \
				   xfs_file.o \
				   xfs_fs_subr.o \
				   xfs_globals.o \
				   xfs_ioctl.o \
				   xfs_iops.o \
				   xfs_message.o \
				   xfs_super.o \
				   xfs_sync.o \
				   xfs_xattr.o)

# Objects in support/
xfs-y				+= support/uuid.o
=======
=======
>>>>>>> refs/remotes/origin/master

# optional features
xfs-$(CONFIG_XFS_QUOTA)		+= xfs_dquot.o \
				   xfs_dquot_item.o \
				   xfs_trans_dquot.o \
				   xfs_qm_syscalls.o \
				   xfs_qm_bhv.o \
				   xfs_qm.o \
				   xfs_quotaops.o
<<<<<<< HEAD
xfs-$(CONFIG_XFS_RT)		+= xfs_rtalloc.o
=======

# xfs_rtbitmap is shared with libxfs
xfs-$(CONFIG_XFS_RT)		+= xfs_rtalloc.o \
				   xfs_rtbitmap.o

>>>>>>> refs/remotes/origin/master
xfs-$(CONFIG_XFS_POSIX_ACL)	+= xfs_acl.o
xfs-$(CONFIG_PROC_FS)		+= xfs_stats.o
xfs-$(CONFIG_SYSCTL)		+= xfs_sysctl.o
xfs-$(CONFIG_COMPAT)		+= xfs_ioctl32.o
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
