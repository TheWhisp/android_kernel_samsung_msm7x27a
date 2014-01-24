#ifndef _LINUX_ERRNO_H
#define _LINUX_ERRNO_H

<<<<<<< HEAD
#include <asm/errno.h>

#ifdef __KERNEL__
=======
#include <uapi/linux/errno.h>

>>>>>>> refs/remotes/origin/master

/*
 * These should never be seen by user programs.  To return one of ERESTART*
 * codes, signal_pending() MUST be set.  Note that ptrace can observe these
 * at syscall exit tracing, but they will never be left for the debugged user
 * process to see.
 */
#define ERESTARTSYS	512
#define ERESTARTNOINTR	513
#define ERESTARTNOHAND	514	/* restart if no handler.. */
#define ENOIOCTLCMD	515	/* No ioctl command */
#define ERESTART_RESTARTBLOCK 516 /* restart by calling sys_restart_syscall */
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define EPROBE_DEFER	517	/* Driver requests probe retry */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define EPROBE_DEFER	517	/* Driver requests probe retry */
#define EOPENSTALE	518	/* open found a stale dentry */
>>>>>>> refs/remotes/origin/master

/* Defined for the NFSv3 protocol */
#define EBADHANDLE	521	/* Illegal NFS file handle */
#define ENOTSYNC	522	/* Update synchronization mismatch */
#define EBADCOOKIE	523	/* Cookie is stale */
#define ENOTSUPP	524	/* Operation is not supported */
#define ETOOSMALL	525	/* Buffer or request is too small */
#define ESERVERFAULT	526	/* An untranslatable error occurred */
#define EBADTYPE	527	/* Type not supported by server */
#define EJUKEBOX	528	/* Request initiated, but will not complete before timeout */
#define EIOCBQUEUED	529	/* iocb queued, will get completion event */
<<<<<<< HEAD
#define EIOCBRETRY	530	/* iocb queued, will trigger a retry */

#endif
=======
>>>>>>> refs/remotes/origin/master

#endif
