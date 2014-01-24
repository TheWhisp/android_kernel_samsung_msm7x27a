/*
 *   fs/cifs/xattr.c
 *
 *   Copyright (c) International Business Machines  Corp., 2003, 2007
 *   Author(s): Steve French (sfrench@us.ibm.com)
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
#include <linux/posix_acl_xattr.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/xattr.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/xattr.h>
>>>>>>> refs/remotes/origin/master
#include "cifsfs.h"
#include "cifspdu.h"
#include "cifsglob.h"
#include "cifsproto.h"
#include "cifs_debug.h"

#define MAX_EA_VALUE_SIZE 65535
#define CIFS_XATTR_DOS_ATTRIB "user.DosAttrib"
#define CIFS_XATTR_CIFS_ACL "system.cifs_acl"
<<<<<<< HEAD
<<<<<<< HEAD
#define CIFS_XATTR_USER_PREFIX "user."
#define CIFS_XATTR_SYSTEM_PREFIX "system."
#define CIFS_XATTR_OS2_PREFIX "os2."
#define CIFS_XATTR_SECURITY_PREFIX "security."
#define CIFS_XATTR_TRUSTED_PREFIX "trusted."
#define XATTR_TRUSTED_PREFIX_LEN  8
#define XATTR_SECURITY_PREFIX_LEN 9
/* BB need to add server (Samba e.g) support for security and trusted prefix */


=======

/* BB need to add server (Samba e.g) support for security and trusted prefix */
>>>>>>> refs/remotes/origin/cm-10.0
=======

/* BB need to add server (Samba e.g) support for security and trusted prefix */
>>>>>>> refs/remotes/origin/master

int cifs_removexattr(struct dentry *direntry, const char *ea_name)
{
	int rc = -EOPNOTSUPP;
#ifdef CONFIG_CIFS_XATTR
<<<<<<< HEAD
	int xid;
=======
	unsigned int xid;
>>>>>>> refs/remotes/origin/master
	struct cifs_sb_info *cifs_sb;
	struct tcon_link *tlink;
	struct cifs_tcon *pTcon;
	struct super_block *sb;
	char *full_path = NULL;

	if (direntry == NULL)
		return -EIO;
	if (direntry->d_inode == NULL)
		return -EIO;
	sb = direntry->d_inode->i_sb;
	if (sb == NULL)
		return -EIO;

	cifs_sb = CIFS_SB(sb);
	tlink = cifs_sb_tlink(cifs_sb);
	if (IS_ERR(tlink))
		return PTR_ERR(tlink);
	pTcon = tlink_tcon(tlink);

<<<<<<< HEAD
	xid = GetXid();
=======
	xid = get_xid();
>>>>>>> refs/remotes/origin/master

	full_path = build_path_from_dentry(direntry);
	if (full_path == NULL) {
		rc = -ENOMEM;
		goto remove_ea_exit;
	}
	if (ea_name == NULL) {
<<<<<<< HEAD
		cFYI(1, "Null xattr names not supported");
<<<<<<< HEAD
	} else if (strncmp(ea_name, CIFS_XATTR_USER_PREFIX, 5)
		&& (strncmp(ea_name, CIFS_XATTR_OS2_PREFIX, 4))) {
=======
	} else if (strncmp(ea_name, XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN)
		&& (strncmp(ea_name, XATTR_OS2_PREFIX, XATTR_OS2_PREFIX_LEN))) {
>>>>>>> refs/remotes/origin/cm-10.0
		cFYI(1,
		     "illegal xattr request %s (only user namespace supported)",
		     ea_name);
=======
		cifs_dbg(FYI, "Null xattr names not supported\n");
	} else if (strncmp(ea_name, XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN)
		&& (strncmp(ea_name, XATTR_OS2_PREFIX, XATTR_OS2_PREFIX_LEN))) {
		cifs_dbg(FYI,
			 "illegal xattr request %s (only user namespace supported)\n",
			 ea_name);
>>>>>>> refs/remotes/origin/master
		/* BB what if no namespace prefix? */
		/* Should we just pass them to server, except for
		system and perhaps security prefixes? */
	} else {
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_XATTR)
			goto remove_ea_exit;

<<<<<<< HEAD
<<<<<<< HEAD
		ea_name += 5; /* skip past user. prefix */
=======
		ea_name += XATTR_USER_PREFIX_LEN; /* skip past user. prefix */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ea_name += XATTR_USER_PREFIX_LEN; /* skip past user. prefix */
>>>>>>> refs/remotes/origin/master
		rc = CIFSSMBSetEA(xid, pTcon, full_path, ea_name, NULL,
			(__u16)0, cifs_sb->local_nls,
			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
	}
remove_ea_exit:
	kfree(full_path);
<<<<<<< HEAD
	FreeXid(xid);
=======
	free_xid(xid);
>>>>>>> refs/remotes/origin/master
	cifs_put_tlink(tlink);
#endif
	return rc;
}

int cifs_setxattr(struct dentry *direntry, const char *ea_name,
		  const void *ea_value, size_t value_size, int flags)
{
	int rc = -EOPNOTSUPP;
#ifdef CONFIG_CIFS_XATTR
<<<<<<< HEAD
	int xid;
=======
	unsigned int xid;
>>>>>>> refs/remotes/origin/master
	struct cifs_sb_info *cifs_sb;
	struct tcon_link *tlink;
	struct cifs_tcon *pTcon;
	struct super_block *sb;
	char *full_path;
<<<<<<< HEAD
<<<<<<< HEAD
	struct cifs_ntsd *pacl;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (direntry == NULL)
		return -EIO;
	if (direntry->d_inode == NULL)
		return -EIO;
	sb = direntry->d_inode->i_sb;
	if (sb == NULL)
		return -EIO;

	cifs_sb = CIFS_SB(sb);
	tlink = cifs_sb_tlink(cifs_sb);
	if (IS_ERR(tlink))
		return PTR_ERR(tlink);
	pTcon = tlink_tcon(tlink);

<<<<<<< HEAD
	xid = GetXid();
=======
	xid = get_xid();
>>>>>>> refs/remotes/origin/master

	full_path = build_path_from_dentry(direntry);
	if (full_path == NULL) {
		rc = -ENOMEM;
		goto set_ea_exit;
	}
	/* return dos attributes as pseudo xattr */
	/* return alt name if available as pseudo attr */

	/* if proc/fs/cifs/streamstoxattr is set then
		search server for EAs or streams to
		returns as xattrs */
	if (value_size > MAX_EA_VALUE_SIZE) {
<<<<<<< HEAD
		cFYI(1, "size of EA value too large");
=======
		cifs_dbg(FYI, "size of EA value too large\n");
>>>>>>> refs/remotes/origin/master
		rc = -EOPNOTSUPP;
		goto set_ea_exit;
	}

	if (ea_name == NULL) {
<<<<<<< HEAD
		cFYI(1, "Null xattr names not supported");
<<<<<<< HEAD
	} else if (strncmp(ea_name, CIFS_XATTR_USER_PREFIX, 5) == 0) {
=======
	} else if (strncmp(ea_name, XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN)
		   == 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_XATTR)
			goto set_ea_exit;
		if (strncmp(ea_name, CIFS_XATTR_DOS_ATTRIB, 14) == 0)
			cFYI(1, "attempt to set cifs inode metadata");

<<<<<<< HEAD
		ea_name += 5; /* skip past user. prefix */
		rc = CIFSSMBSetEA(xid, pTcon, full_path, ea_name, ea_value,
			(__u16)value_size, cifs_sb->local_nls,
			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
	} else if (strncmp(ea_name, CIFS_XATTR_OS2_PREFIX, 4) == 0) {
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_XATTR)
			goto set_ea_exit;

		ea_name += 4; /* skip past os2. prefix */
=======
=======
		cifs_dbg(FYI, "Null xattr names not supported\n");
	} else if (strncmp(ea_name, XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN)
		   == 0) {
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_XATTR)
			goto set_ea_exit;
		if (strncmp(ea_name, CIFS_XATTR_DOS_ATTRIB, 14) == 0)
			cifs_dbg(FYI, "attempt to set cifs inode metadata\n");

>>>>>>> refs/remotes/origin/master
		ea_name += XATTR_USER_PREFIX_LEN; /* skip past user. prefix */
		rc = CIFSSMBSetEA(xid, pTcon, full_path, ea_name, ea_value,
			(__u16)value_size, cifs_sb->local_nls,
			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
	} else if (strncmp(ea_name, XATTR_OS2_PREFIX, XATTR_OS2_PREFIX_LEN)
		   == 0) {
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_XATTR)
			goto set_ea_exit;

		ea_name += XATTR_OS2_PREFIX_LEN; /* skip past os2. prefix */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		rc = CIFSSMBSetEA(xid, pTcon, full_path, ea_name, ea_value,
			(__u16)value_size, cifs_sb->local_nls,
			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
	} else if (strncmp(ea_name, CIFS_XATTR_CIFS_ACL,
			strlen(CIFS_XATTR_CIFS_ACL)) == 0) {
<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifdef CONFIG_CIFS_ACL
		struct cifs_ntsd *pacl;
>>>>>>> refs/remotes/origin/cm-10.0
		pacl = kmalloc(value_size, GFP_KERNEL);
		if (!pacl) {
			cFYI(1, "%s: Can't allocate memory for ACL",
					__func__);
			rc = -ENOMEM;
		} else {
<<<<<<< HEAD
#ifdef CONFIG_CIFS_ACL
			memcpy(pacl, ea_value, value_size);
			rc = set_cifs_acl(pacl, value_size,
				direntry->d_inode, full_path);
			if (rc == 0) /* force revalidate of the inode */
				CIFS_I(direntry->d_inode)->time = 0;
			kfree(pacl);
#else
			cFYI(1, "Set CIFS ACL not supported yet");
#endif /* CONFIG_CIFS_ACL */
		}
=======
=======
#ifdef CONFIG_CIFS_ACL
		struct cifs_ntsd *pacl;
		pacl = kmalloc(value_size, GFP_KERNEL);
		if (!pacl) {
			rc = -ENOMEM;
		} else {
>>>>>>> refs/remotes/origin/master
			memcpy(pacl, ea_value, value_size);
			rc = set_cifs_acl(pacl, value_size,
				direntry->d_inode, full_path, CIFS_ACL_DACL);
			if (rc == 0) /* force revalidate of the inode */
				CIFS_I(direntry->d_inode)->time = 0;
			kfree(pacl);
		}
#else
<<<<<<< HEAD
			cFYI(1, "Set CIFS ACL not supported yet");
#endif /* CONFIG_CIFS_ACL */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cifs_dbg(FYI, "Set CIFS ACL not supported yet\n");
#endif /* CONFIG_CIFS_ACL */
>>>>>>> refs/remotes/origin/master
	} else {
		int temp;
		temp = strncmp(ea_name, POSIX_ACL_XATTR_ACCESS,
			strlen(POSIX_ACL_XATTR_ACCESS));
		if (temp == 0) {
#ifdef CONFIG_CIFS_POSIX
			if (sb->s_flags & MS_POSIXACL)
				rc = CIFSSMBSetPosixACL(xid, pTcon, full_path,
					ea_value, (const int)value_size,
					ACL_TYPE_ACCESS, cifs_sb->local_nls,
					cifs_sb->mnt_cifs_flags &
						CIFS_MOUNT_MAP_SPECIAL_CHR);
<<<<<<< HEAD
			cFYI(1, "set POSIX ACL rc %d", rc);
#else
			cFYI(1, "set POSIX ACL not supported");
=======
			cifs_dbg(FYI, "set POSIX ACL rc %d\n", rc);
#else
			cifs_dbg(FYI, "set POSIX ACL not supported\n");
>>>>>>> refs/remotes/origin/master
#endif
		} else if (strncmp(ea_name, POSIX_ACL_XATTR_DEFAULT,
				   strlen(POSIX_ACL_XATTR_DEFAULT)) == 0) {
#ifdef CONFIG_CIFS_POSIX
			if (sb->s_flags & MS_POSIXACL)
				rc = CIFSSMBSetPosixACL(xid, pTcon, full_path,
					ea_value, (const int)value_size,
					ACL_TYPE_DEFAULT, cifs_sb->local_nls,
					cifs_sb->mnt_cifs_flags &
						CIFS_MOUNT_MAP_SPECIAL_CHR);
<<<<<<< HEAD
			cFYI(1, "set POSIX default ACL rc %d", rc);
#else
			cFYI(1, "set default POSIX ACL not supported");
#endif
		} else {
			cFYI(1, "illegal xattr request %s (only user namespace"
				" supported)", ea_name);
=======
			cifs_dbg(FYI, "set POSIX default ACL rc %d\n", rc);
#else
			cifs_dbg(FYI, "set default POSIX ACL not supported\n");
#endif
		} else {
			cifs_dbg(FYI, "illegal xattr request %s (only user namespace supported)\n",
				 ea_name);
>>>>>>> refs/remotes/origin/master
		  /* BB what if no namespace prefix? */
		  /* Should we just pass them to server, except for
		  system and perhaps security prefixes? */
		}
	}

set_ea_exit:
	kfree(full_path);
<<<<<<< HEAD
	FreeXid(xid);
=======
	free_xid(xid);
>>>>>>> refs/remotes/origin/master
	cifs_put_tlink(tlink);
#endif
	return rc;
}

ssize_t cifs_getxattr(struct dentry *direntry, const char *ea_name,
	void *ea_value, size_t buf_size)
{
	ssize_t rc = -EOPNOTSUPP;
#ifdef CONFIG_CIFS_XATTR
<<<<<<< HEAD
	int xid;
=======
	unsigned int xid;
>>>>>>> refs/remotes/origin/master
	struct cifs_sb_info *cifs_sb;
	struct tcon_link *tlink;
	struct cifs_tcon *pTcon;
	struct super_block *sb;
	char *full_path;

	if (direntry == NULL)
		return -EIO;
	if (direntry->d_inode == NULL)
		return -EIO;
	sb = direntry->d_inode->i_sb;
	if (sb == NULL)
		return -EIO;

	cifs_sb = CIFS_SB(sb);
	tlink = cifs_sb_tlink(cifs_sb);
	if (IS_ERR(tlink))
		return PTR_ERR(tlink);
	pTcon = tlink_tcon(tlink);

<<<<<<< HEAD
	xid = GetXid();
=======
	xid = get_xid();
>>>>>>> refs/remotes/origin/master

	full_path = build_path_from_dentry(direntry);
	if (full_path == NULL) {
		rc = -ENOMEM;
		goto get_ea_exit;
	}
	/* return dos attributes as pseudo xattr */
	/* return alt name if available as pseudo attr */
	if (ea_name == NULL) {
<<<<<<< HEAD
		cFYI(1, "Null xattr names not supported");
<<<<<<< HEAD
	} else if (strncmp(ea_name, CIFS_XATTR_USER_PREFIX, 5) == 0) {
=======
	} else if (strncmp(ea_name, XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN)
		   == 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cifs_dbg(FYI, "Null xattr names not supported\n");
	} else if (strncmp(ea_name, XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN)
		   == 0) {
>>>>>>> refs/remotes/origin/master
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_XATTR)
			goto get_ea_exit;

		if (strncmp(ea_name, CIFS_XATTR_DOS_ATTRIB, 14) == 0) {
<<<<<<< HEAD
			cFYI(1, "attempt to query cifs inode metadata");
			/* revalidate/getattr then populate from inode */
		} /* BB add else when above is implemented */
<<<<<<< HEAD
		ea_name += 5; /* skip past user. prefix */
		rc = CIFSSMBQAllEAs(xid, pTcon, full_path, ea_name, ea_value,
			buf_size, cifs_sb->local_nls,
			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
	} else if (strncmp(ea_name, CIFS_XATTR_OS2_PREFIX, 4) == 0) {
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_XATTR)
			goto get_ea_exit;

		ea_name += 4; /* skip past os2. prefix */
=======
=======
			cifs_dbg(FYI, "attempt to query cifs inode metadata\n");
			/* revalidate/getattr then populate from inode */
		} /* BB add else when above is implemented */
>>>>>>> refs/remotes/origin/master
		ea_name += XATTR_USER_PREFIX_LEN; /* skip past user. prefix */
		rc = CIFSSMBQAllEAs(xid, pTcon, full_path, ea_name, ea_value,
			buf_size, cifs_sb->local_nls,
			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
	} else if (strncmp(ea_name, XATTR_OS2_PREFIX, XATTR_OS2_PREFIX_LEN) == 0) {
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_XATTR)
			goto get_ea_exit;

		ea_name += XATTR_OS2_PREFIX_LEN; /* skip past os2. prefix */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		rc = CIFSSMBQAllEAs(xid, pTcon, full_path, ea_name, ea_value,
			buf_size, cifs_sb->local_nls,
			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
	} else if (strncmp(ea_name, POSIX_ACL_XATTR_ACCESS,
			  strlen(POSIX_ACL_XATTR_ACCESS)) == 0) {
#ifdef CONFIG_CIFS_POSIX
		if (sb->s_flags & MS_POSIXACL)
			rc = CIFSSMBGetPosixACL(xid, pTcon, full_path,
				ea_value, buf_size, ACL_TYPE_ACCESS,
				cifs_sb->local_nls,
				cifs_sb->mnt_cifs_flags &
					CIFS_MOUNT_MAP_SPECIAL_CHR);
#else
<<<<<<< HEAD
		cFYI(1, "Query POSIX ACL not supported yet");
=======
		cifs_dbg(FYI, "Query POSIX ACL not supported yet\n");
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_CIFS_POSIX */
	} else if (strncmp(ea_name, POSIX_ACL_XATTR_DEFAULT,
			  strlen(POSIX_ACL_XATTR_DEFAULT)) == 0) {
#ifdef CONFIG_CIFS_POSIX
		if (sb->s_flags & MS_POSIXACL)
			rc = CIFSSMBGetPosixACL(xid, pTcon, full_path,
				ea_value, buf_size, ACL_TYPE_DEFAULT,
				cifs_sb->local_nls,
				cifs_sb->mnt_cifs_flags &
					CIFS_MOUNT_MAP_SPECIAL_CHR);
#else
<<<<<<< HEAD
		cFYI(1, "Query POSIX default ACL not supported yet");
=======
		cifs_dbg(FYI, "Query POSIX default ACL not supported yet\n");
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_CIFS_POSIX */
	} else if (strncmp(ea_name, CIFS_XATTR_CIFS_ACL,
				strlen(CIFS_XATTR_CIFS_ACL)) == 0) {
#ifdef CONFIG_CIFS_ACL
			u32 acllen;
			struct cifs_ntsd *pacl;

			pacl = get_cifs_acl(cifs_sb, direntry->d_inode,
						full_path, &acllen);
			if (IS_ERR(pacl)) {
				rc = PTR_ERR(pacl);
<<<<<<< HEAD
				cERROR(1, "%s: error %zd getting sec desc",
						__func__, rc);
=======
				cifs_dbg(VFS, "%s: error %zd getting sec desc\n",
					 __func__, rc);
>>>>>>> refs/remotes/origin/master
			} else {
				if (ea_value) {
					if (acllen > buf_size)
						acllen = -ERANGE;
					else
						memcpy(ea_value, pacl, acllen);
				}
				rc = acllen;
				kfree(pacl);
			}
#else
<<<<<<< HEAD
		cFYI(1, "Query CIFS ACL not supported yet");
#endif /* CONFIG_CIFS_ACL */
	} else if (strncmp(ea_name,
<<<<<<< HEAD
		  CIFS_XATTR_TRUSTED_PREFIX, XATTR_TRUSTED_PREFIX_LEN) == 0) {
		cFYI(1, "Trusted xattr namespace not supported yet");
	} else if (strncmp(ea_name,
		  CIFS_XATTR_SECURITY_PREFIX, XATTR_SECURITY_PREFIX_LEN) == 0) {
=======
		  XATTR_TRUSTED_PREFIX, XATTR_TRUSTED_PREFIX_LEN) == 0) {
		cFYI(1, "Trusted xattr namespace not supported yet");
	} else if (strncmp(ea_name,
		  XATTR_SECURITY_PREFIX, XATTR_SECURITY_PREFIX_LEN) == 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		cFYI(1, "Security xattr namespace not supported yet");
	} else
		cFYI(1,
		    "illegal xattr request %s (only user namespace supported)",
		     ea_name);
=======
			cifs_dbg(FYI, "Query CIFS ACL not supported yet\n");
#endif /* CONFIG_CIFS_ACL */
	} else if (strncmp(ea_name,
		  XATTR_TRUSTED_PREFIX, XATTR_TRUSTED_PREFIX_LEN) == 0) {
		cifs_dbg(FYI, "Trusted xattr namespace not supported yet\n");
	} else if (strncmp(ea_name,
		  XATTR_SECURITY_PREFIX, XATTR_SECURITY_PREFIX_LEN) == 0) {
		cifs_dbg(FYI, "Security xattr namespace not supported yet\n");
	} else
		cifs_dbg(FYI,
			 "illegal xattr request %s (only user namespace supported)\n",
			 ea_name);
>>>>>>> refs/remotes/origin/master

	/* We could add an additional check for streams ie
	    if proc/fs/cifs/streamstoxattr is set then
		search server for EAs or streams to
		returns as xattrs */

	if (rc == -EINVAL)
		rc = -EOPNOTSUPP;

get_ea_exit:
	kfree(full_path);
<<<<<<< HEAD
	FreeXid(xid);
=======
	free_xid(xid);
>>>>>>> refs/remotes/origin/master
	cifs_put_tlink(tlink);
#endif
	return rc;
}

ssize_t cifs_listxattr(struct dentry *direntry, char *data, size_t buf_size)
{
	ssize_t rc = -EOPNOTSUPP;
#ifdef CONFIG_CIFS_XATTR
<<<<<<< HEAD
	int xid;
=======
	unsigned int xid;
>>>>>>> refs/remotes/origin/master
	struct cifs_sb_info *cifs_sb;
	struct tcon_link *tlink;
	struct cifs_tcon *pTcon;
	struct super_block *sb;
	char *full_path;

	if (direntry == NULL)
		return -EIO;
	if (direntry->d_inode == NULL)
		return -EIO;
	sb = direntry->d_inode->i_sb;
	if (sb == NULL)
		return -EIO;

	cifs_sb = CIFS_SB(sb);
	if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NO_XATTR)
		return -EOPNOTSUPP;

	tlink = cifs_sb_tlink(cifs_sb);
	if (IS_ERR(tlink))
		return PTR_ERR(tlink);
	pTcon = tlink_tcon(tlink);

<<<<<<< HEAD
	xid = GetXid();
=======
	xid = get_xid();
>>>>>>> refs/remotes/origin/master

	full_path = build_path_from_dentry(direntry);
	if (full_path == NULL) {
		rc = -ENOMEM;
		goto list_ea_exit;
	}
	/* return dos attributes as pseudo xattr */
	/* return alt name if available as pseudo attr */

	/* if proc/fs/cifs/streamstoxattr is set then
		search server for EAs or streams to
		returns as xattrs */
	rc = CIFSSMBQAllEAs(xid, pTcon, full_path, NULL, data,
				buf_size, cifs_sb->local_nls,
				cifs_sb->mnt_cifs_flags &
					CIFS_MOUNT_MAP_SPECIAL_CHR);

list_ea_exit:
	kfree(full_path);
<<<<<<< HEAD
	FreeXid(xid);
=======
	free_xid(xid);
>>>>>>> refs/remotes/origin/master
	cifs_put_tlink(tlink);
#endif
	return rc;
}
