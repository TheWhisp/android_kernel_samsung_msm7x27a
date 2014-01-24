/*
 *	Wrapper functions for 16bit uid back compatibility. All nicely tied
 *	together in the faint hope we can take the out in five years time.
 */

#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/prctl.h>
#include <linux/capability.h>
#include <linux/init.h>
#include <linux/highuid.h>
#include <linux/security.h>
#include <linux/syscalls.h>

#include <asm/uaccess.h>

SYSCALL_DEFINE3(chown16, const char __user *, filename, old_uid_t, user, old_gid_t, group)
{
<<<<<<< HEAD
	long ret = sys_chown(filename, low2highuid(user), low2highgid(group));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(3, ret, filename, user, group);
	return ret;
=======
	return sys_chown(filename, low2highuid(user), low2highgid(group));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE3(lchown16, const char __user *, filename, old_uid_t, user, old_gid_t, group)
{
<<<<<<< HEAD
	long ret = sys_lchown(filename, low2highuid(user), low2highgid(group));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(3, ret, filename, user, group);
	return ret;
=======
	return sys_lchown(filename, low2highuid(user), low2highgid(group));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE3(fchown16, unsigned int, fd, old_uid_t, user, old_gid_t, group)
{
<<<<<<< HEAD
	long ret = sys_fchown(fd, low2highuid(user), low2highgid(group));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(3, ret, fd, user, group);
	return ret;
=======
	return sys_fchown(fd, low2highuid(user), low2highgid(group));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE2(setregid16, old_gid_t, rgid, old_gid_t, egid)
{
<<<<<<< HEAD
	long ret = sys_setregid(low2highgid(rgid), low2highgid(egid));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(2, ret, rgid, egid);
	return ret;
=======
	return sys_setregid(low2highgid(rgid), low2highgid(egid));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE1(setgid16, old_gid_t, gid)
{
<<<<<<< HEAD
	long ret = sys_setgid(low2highgid(gid));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(1, ret, gid);
	return ret;
=======
	return sys_setgid(low2highgid(gid));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE2(setreuid16, old_uid_t, ruid, old_uid_t, euid)
{
<<<<<<< HEAD
	long ret = sys_setreuid(low2highuid(ruid), low2highuid(euid));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(2, ret, ruid, euid);
	return ret;
=======
	return sys_setreuid(low2highuid(ruid), low2highuid(euid));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE1(setuid16, old_uid_t, uid)
{
<<<<<<< HEAD
	long ret = sys_setuid(low2highuid(uid));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(1, ret, uid);
	return ret;
=======
	return sys_setuid(low2highuid(uid));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE3(setresuid16, old_uid_t, ruid, old_uid_t, euid, old_uid_t, suid)
{
<<<<<<< HEAD
	long ret = sys_setresuid(low2highuid(ruid), low2highuid(euid),
				 low2highuid(suid));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(3, ret, ruid, euid, suid);
	return ret;
}

SYSCALL_DEFINE3(getresuid16, old_uid_t __user *, ruid, old_uid_t __user *, euid, old_uid_t __user *, suid)
{
	const struct cred *cred = current_cred();
	int retval;

	if (!(retval   = put_user(high2lowuid(cred->uid),  ruid)) &&
	    !(retval   = put_user(high2lowuid(cred->euid), euid)))
		retval = put_user(high2lowuid(cred->suid), suid);
=======
	return sys_setresuid(low2highuid(ruid), low2highuid(euid),
				 low2highuid(suid));
}

SYSCALL_DEFINE3(getresuid16, old_uid_t __user *, ruidp, old_uid_t __user *, euidp, old_uid_t __user *, suidp)
{
	const struct cred *cred = current_cred();
	int retval;
	old_uid_t ruid, euid, suid;

	ruid = high2lowuid(from_kuid_munged(cred->user_ns, cred->uid));
	euid = high2lowuid(from_kuid_munged(cred->user_ns, cred->euid));
	suid = high2lowuid(from_kuid_munged(cred->user_ns, cred->suid));

	if (!(retval   = put_user(ruid, ruidp)) &&
	    !(retval   = put_user(euid, euidp)))
		retval = put_user(suid, suidp);
>>>>>>> refs/remotes/origin/master

	return retval;
}

SYSCALL_DEFINE3(setresgid16, old_gid_t, rgid, old_gid_t, egid, old_gid_t, sgid)
{
<<<<<<< HEAD
	long ret = sys_setresgid(low2highgid(rgid), low2highgid(egid),
				 low2highgid(sgid));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(3, ret, rgid, egid, sgid);
	return ret;
}


SYSCALL_DEFINE3(getresgid16, old_gid_t __user *, rgid, old_gid_t __user *, egid, old_gid_t __user *, sgid)
{
	const struct cred *cred = current_cred();
	int retval;

	if (!(retval   = put_user(high2lowgid(cred->gid),  rgid)) &&
	    !(retval   = put_user(high2lowgid(cred->egid), egid)))
		retval = put_user(high2lowgid(cred->sgid), sgid);
=======
	return sys_setresgid(low2highgid(rgid), low2highgid(egid),
				 low2highgid(sgid));
}


SYSCALL_DEFINE3(getresgid16, old_gid_t __user *, rgidp, old_gid_t __user *, egidp, old_gid_t __user *, sgidp)
{
	const struct cred *cred = current_cred();
	int retval;
	old_gid_t rgid, egid, sgid;

	rgid = high2lowgid(from_kgid_munged(cred->user_ns, cred->gid));
	egid = high2lowgid(from_kgid_munged(cred->user_ns, cred->egid));
	sgid = high2lowgid(from_kgid_munged(cred->user_ns, cred->sgid));

	if (!(retval   = put_user(rgid, rgidp)) &&
	    !(retval   = put_user(egid, egidp)))
		retval = put_user(sgid, sgidp);
>>>>>>> refs/remotes/origin/master

	return retval;
}

SYSCALL_DEFINE1(setfsuid16, old_uid_t, uid)
{
<<<<<<< HEAD
	long ret = sys_setfsuid(low2highuid(uid));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(1, ret, uid);
	return ret;
=======
	return sys_setfsuid(low2highuid(uid));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE1(setfsgid16, old_gid_t, gid)
{
<<<<<<< HEAD
	long ret = sys_setfsgid(low2highgid(gid));
	/* avoid REGPARM breakage on x86: */
	asmlinkage_protect(1, ret, gid);
	return ret;
=======
	return sys_setfsgid(low2highgid(gid));
>>>>>>> refs/remotes/origin/master
}

static int groups16_to_user(old_gid_t __user *grouplist,
    struct group_info *group_info)
{
<<<<<<< HEAD
	int i;
	old_gid_t group;

	for (i = 0; i < group_info->ngroups; i++) {
		group = high2lowgid(GROUP_AT(group_info, i));
=======
	struct user_namespace *user_ns = current_user_ns();
	int i;
	old_gid_t group;
	kgid_t kgid;

	for (i = 0; i < group_info->ngroups; i++) {
		kgid = GROUP_AT(group_info, i);
		group = high2lowgid(from_kgid_munged(user_ns, kgid));
>>>>>>> refs/remotes/origin/master
		if (put_user(group, grouplist+i))
			return -EFAULT;
	}

	return 0;
}

static int groups16_from_user(struct group_info *group_info,
    old_gid_t __user *grouplist)
{
<<<<<<< HEAD
	int i;
	old_gid_t group;
=======
	struct user_namespace *user_ns = current_user_ns();
	int i;
	old_gid_t group;
	kgid_t kgid;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < group_info->ngroups; i++) {
		if (get_user(group, grouplist+i))
			return  -EFAULT;
<<<<<<< HEAD
		GROUP_AT(group_info, i) = low2highgid(group);
=======

		kgid = make_kgid(user_ns, low2highgid(group));
		if (!gid_valid(kgid))
			return -EINVAL;

		GROUP_AT(group_info, i) = kgid;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

SYSCALL_DEFINE2(getgroups16, int, gidsetsize, old_gid_t __user *, grouplist)
{
	const struct cred *cred = current_cred();
	int i;

	if (gidsetsize < 0)
		return -EINVAL;

	i = cred->group_info->ngroups;
	if (gidsetsize) {
		if (i > gidsetsize) {
			i = -EINVAL;
			goto out;
		}
		if (groups16_to_user(grouplist, cred->group_info)) {
			i = -EFAULT;
			goto out;
		}
	}
out:
	return i;
}

SYSCALL_DEFINE2(setgroups16, int, gidsetsize, old_gid_t __user *, grouplist)
{
	struct group_info *group_info;
	int retval;

<<<<<<< HEAD
	if (!nsown_capable(CAP_SETGID))
=======
	if (!ns_capable(current_user_ns(), CAP_SETGID))
>>>>>>> refs/remotes/origin/master
		return -EPERM;
	if ((unsigned)gidsetsize > NGROUPS_MAX)
		return -EINVAL;

	group_info = groups_alloc(gidsetsize);
	if (!group_info)
		return -ENOMEM;
	retval = groups16_from_user(group_info, grouplist);
	if (retval) {
		put_group_info(group_info);
		return retval;
	}

	retval = set_current_groups(group_info);
	put_group_info(group_info);

	return retval;
}

SYSCALL_DEFINE0(getuid16)
{
<<<<<<< HEAD
	return high2lowuid(current_uid());
=======
	return high2lowuid(from_kuid_munged(current_user_ns(), current_uid()));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE0(geteuid16)
{
<<<<<<< HEAD
	return high2lowuid(current_euid());
=======
	return high2lowuid(from_kuid_munged(current_user_ns(), current_euid()));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE0(getgid16)
{
<<<<<<< HEAD
	return high2lowgid(current_gid());
=======
	return high2lowgid(from_kgid_munged(current_user_ns(), current_gid()));
>>>>>>> refs/remotes/origin/master
}

SYSCALL_DEFINE0(getegid16)
{
<<<<<<< HEAD
	return high2lowgid(current_egid());
=======
	return high2lowgid(from_kgid_munged(current_user_ns(), current_egid()));
>>>>>>> refs/remotes/origin/master
}
