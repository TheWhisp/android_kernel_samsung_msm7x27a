#ifndef __LINUX_NET_SCM_H
#define __LINUX_NET_SCM_H

#include <linux/limits.h>
#include <linux/net.h>
#include <linux/security.h>
#include <linux/pid.h>
#include <linux/nsproxy.h>

/* Well, we should have at least one descriptor open
 * to accept passed FDs 8)
 */
#define SCM_MAX_FD	253

<<<<<<< HEAD
struct scm_fp_list {
	struct list_head	list;
=======
struct scm_creds {
	u32	pid;
	kuid_t	uid;
	kgid_t	gid;
};

struct scm_fp_list {
>>>>>>> refs/remotes/origin/master
	short			count;
	short			max;
	struct file		*fp[SCM_MAX_FD];
};

struct scm_cookie {
	struct pid		*pid;		/* Skb credentials */
<<<<<<< HEAD
	const struct cred	*cred;
	struct scm_fp_list	*fp;		/* Passed files		*/
	struct ucred		creds;		/* Skb credentials	*/
=======
	struct scm_fp_list	*fp;		/* Passed files		*/
	struct scm_creds	creds;		/* Skb credentials	*/
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_SECURITY_NETWORK
	u32			secid;		/* Passed security ID 	*/
#endif
};

<<<<<<< HEAD
extern void scm_detach_fds(struct msghdr *msg, struct scm_cookie *scm);
extern void scm_detach_fds_compat(struct msghdr *msg, struct scm_cookie *scm);
extern int __scm_send(struct socket *sock, struct msghdr *msg, struct scm_cookie *scm);
extern void __scm_destroy(struct scm_cookie *scm);
extern struct scm_fp_list * scm_fp_dup(struct scm_fp_list *fpl);
=======
void scm_detach_fds(struct msghdr *msg, struct scm_cookie *scm);
void scm_detach_fds_compat(struct msghdr *msg, struct scm_cookie *scm);
int __scm_send(struct socket *sock, struct msghdr *msg, struct scm_cookie *scm);
void __scm_destroy(struct scm_cookie *scm);
struct scm_fp_list *scm_fp_dup(struct scm_fp_list *fpl);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_SECURITY_NETWORK
static __inline__ void unix_get_peersec_dgram(struct socket *sock, struct scm_cookie *scm)
{
	security_socket_getpeersec_dgram(sock, NULL, &scm->secid);
}
#else
static __inline__ void unix_get_peersec_dgram(struct socket *sock, struct scm_cookie *scm)
{ }
#endif /* CONFIG_SECURITY_NETWORK */

static __inline__ void scm_set_cred(struct scm_cookie *scm,
<<<<<<< HEAD
				    struct pid *pid, const struct cred *cred)
{
	scm->pid  = get_pid(pid);
<<<<<<< HEAD
	scm->cred = get_cred(cred);
=======
	scm->cred = cred ? get_cred(cred) : NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	cred_to_ucred(pid, cred, &scm->creds, false);
=======
				    struct pid *pid, kuid_t uid, kgid_t gid)
{
	scm->pid  = get_pid(pid);
	scm->creds.pid = pid_vnr(pid);
	scm->creds.uid = uid;
	scm->creds.gid = gid;
>>>>>>> refs/remotes/origin/master
=======
	cred_to_ucred(pid, cred, &scm->creds, false);
>>>>>>> refs/remotes/origin/cm-11.0
}

static __inline__ void scm_destroy_cred(struct scm_cookie *scm)
{
	put_pid(scm->pid);
	scm->pid  = NULL;
<<<<<<< HEAD

	if (scm->cred)
		put_cred(scm->cred);
	scm->cred = NULL;
=======
>>>>>>> refs/remotes/origin/master
}

static __inline__ void scm_destroy(struct scm_cookie *scm)
{
	scm_destroy_cred(scm);
<<<<<<< HEAD
	if (scm && scm->fp)
=======
	if (scm->fp)
>>>>>>> refs/remotes/origin/master
		__scm_destroy(scm);
}

static __inline__ int scm_send(struct socket *sock, struct msghdr *msg,
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
			       struct scm_cookie *scm)
=======
			       struct scm_cookie *scm, bool forcecreds)
>>>>>>> refs/remotes/origin/cm-11.0
{
	scm_set_cred(scm, task_tgid(current), current_cred());
	scm->fp = NULL;
=======
			       struct scm_cookie *scm, bool forcecreds)
{
	memset(scm, 0, sizeof(*scm));
	if (forcecreds)
		scm_set_cred(scm, task_tgid(current), current_cred());
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
			       struct scm_cookie *scm, bool forcecreds)
{
	memset(scm, 0, sizeof(*scm));
	scm->creds.uid = INVALID_UID;
	scm->creds.gid = INVALID_GID;
	if (forcecreds)
		scm_set_cred(scm, task_tgid(current), current_uid(), current_gid());
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	unix_get_peersec_dgram(sock, scm);
	if (msg->msg_controllen <= 0)
		return 0;
	return __scm_send(sock, msg, scm);
}

#ifdef CONFIG_SECURITY_NETWORK
static inline void scm_passec(struct socket *sock, struct msghdr *msg, struct scm_cookie *scm)
{
	char *secdata;
	u32 seclen;
	int err;

	if (test_bit(SOCK_PASSSEC, &sock->flags)) {
		err = security_secid_to_secctx(scm->secid, &secdata, &seclen);

		if (!err) {
			put_cmsg(msg, SOL_SOCKET, SCM_SECURITY, seclen, secdata);
			security_release_secctx(secdata, seclen);
		}
	}
}
#else
static inline void scm_passec(struct socket *sock, struct msghdr *msg, struct scm_cookie *scm)
{ }
#endif /* CONFIG_SECURITY_NETWORK */

static __inline__ void scm_recv(struct socket *sock, struct msghdr *msg,
				struct scm_cookie *scm, int flags)
{
	if (!msg->msg_control) {
		if (test_bit(SOCK_PASSCRED, &sock->flags) || scm->fp)
			msg->msg_flags |= MSG_CTRUNC;
		scm_destroy(scm);
		return;
	}

<<<<<<< HEAD
	if (test_bit(SOCK_PASSCRED, &sock->flags))
		put_cmsg(msg, SOL_SOCKET, SCM_CREDENTIALS, sizeof(scm->creds), &scm->creds);
=======
	if (test_bit(SOCK_PASSCRED, &sock->flags)) {
		struct user_namespace *current_ns = current_user_ns();
		struct ucred ucreds = {
			.pid = scm->creds.pid,
			.uid = from_kuid_munged(current_ns, scm->creds.uid),
			.gid = from_kgid_munged(current_ns, scm->creds.gid),
		};
		put_cmsg(msg, SOL_SOCKET, SCM_CREDENTIALS, sizeof(ucreds), &ucreds);
	}
>>>>>>> refs/remotes/origin/master

	scm_destroy_cred(scm);

	scm_passec(sock, msg, scm);

	if (!scm->fp)
		return;
	
	scm_detach_fds(msg, scm);
}


#endif /* __LINUX_NET_SCM_H */

