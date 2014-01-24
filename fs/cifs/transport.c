/*
 *   fs/cifs/transport.c
 *
 *   Copyright (C) International Business Machines  Corp., 2002,2008
 *   Author(s): Steve French (sfrench@us.ibm.com)
 *   Jeremy Allison (jra@samba.org) 2006.
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
#include <linux/list.h>
#include <linux/gfp.h>
#include <linux/wait.h>
#include <linux/net.h>
#include <linux/delay.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/freezer.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/freezer.h>
#include <linux/tcp.h>
#include <linux/highmem.h>
>>>>>>> refs/remotes/origin/master
#include <asm/uaccess.h>
#include <asm/processor.h>
#include <linux/mempool.h>
#include "cifspdu.h"
#include "cifsglob.h"
#include "cifsproto.h"
#include "cifs_debug.h"

<<<<<<< HEAD
extern mempool_t *cifs_mid_poolp;

static void
wake_up_task(struct mid_q_entry *mid)
=======
void
cifs_wake_up_task(struct mid_q_entry *mid)
>>>>>>> refs/remotes/origin/master
{
	wake_up_process(mid->callback_data);
}

struct mid_q_entry *
AllocMidQEntry(const struct smb_hdr *smb_buffer, struct TCP_Server_Info *server)
{
	struct mid_q_entry *temp;

	if (server == NULL) {
<<<<<<< HEAD
		cERROR(1, "Null TCP session in AllocMidQEntry");
=======
		cifs_dbg(VFS, "Null TCP session in AllocMidQEntry\n");
>>>>>>> refs/remotes/origin/master
		return NULL;
	}

	temp = mempool_alloc(cifs_mid_poolp, GFP_NOFS);
	if (temp == NULL)
		return temp;
	else {
		memset(temp, 0, sizeof(struct mid_q_entry));
<<<<<<< HEAD
		temp->mid = smb_buffer->Mid;	/* always LE */
		temp->pid = current->pid;
<<<<<<< HEAD
		temp->command = smb_buffer->Command;
		cFYI(1, "For smb_command %d", temp->command);
=======
		temp->command = cpu_to_le16(smb_buffer->Command);
		cFYI(1, "For smb_command %d", smb_buffer->Command);
>>>>>>> refs/remotes/origin/cm-10.0
	/*	do_gettimeofday(&temp->when_sent);*/ /* easier to use jiffies */
		/* when mid allocated can be before when sent */
		temp->when_alloc = jiffies;
=======
		temp->mid = get_mid(smb_buffer);
		temp->pid = current->pid;
		temp->command = cpu_to_le16(smb_buffer->Command);
		cifs_dbg(FYI, "For smb_command %d\n", smb_buffer->Command);
	/*	do_gettimeofday(&temp->when_sent);*/ /* easier to use jiffies */
		/* when mid allocated can be before when sent */
		temp->when_alloc = jiffies;
		temp->server = server;
>>>>>>> refs/remotes/origin/master

		/*
		 * The default is for the mid to be synchronous, so the
		 * default callback just wakes up the current task.
		 */
<<<<<<< HEAD
		temp->callback = wake_up_task;
=======
		temp->callback = cifs_wake_up_task;
>>>>>>> refs/remotes/origin/master
		temp->callback_data = current;
	}

	atomic_inc(&midCount);
<<<<<<< HEAD
<<<<<<< HEAD
	temp->midState = MID_REQUEST_ALLOCATED;
=======
	temp->mid_state = MID_REQUEST_ALLOCATED;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	temp->mid_state = MID_REQUEST_ALLOCATED;
>>>>>>> refs/remotes/origin/master
	return temp;
}

void
DeleteMidQEntry(struct mid_q_entry *midEntry)
{
#ifdef CONFIG_CIFS_STATS2
<<<<<<< HEAD
	unsigned long now;
#endif
<<<<<<< HEAD
	midEntry->midState = MID_FREE;
	atomic_dec(&midCount);
	if (midEntry->largeBuf)
=======
	midEntry->mid_state = MID_FREE;
	atomic_dec(&midCount);
	if (midEntry->large_buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	__le16 command = midEntry->server->vals->lock_cmd;
	unsigned long now;
#endif
	midEntry->mid_state = MID_FREE;
	atomic_dec(&midCount);
	if (midEntry->large_buf)
>>>>>>> refs/remotes/origin/master
		cifs_buf_release(midEntry->resp_buf);
	else
		cifs_small_buf_release(midEntry->resp_buf);
#ifdef CONFIG_CIFS_STATS2
	now = jiffies;
	/* commands taking longer than one second are indications that
	   something is wrong, unless it is quite a slow link or server */
	if ((now - midEntry->when_alloc) > HZ) {
<<<<<<< HEAD
		if ((cifsFYI & CIFS_TIMER) &&
<<<<<<< HEAD
		   (midEntry->command != SMB_COM_LOCKING_ANDX)) {
			printk(KERN_DEBUG " CIFS slow rsp: cmd %d mid %d",
=======
		    (midEntry->command != cpu_to_le16(SMB_COM_LOCKING_ANDX))) {
			printk(KERN_DEBUG " CIFS slow rsp: cmd %d mid %llu",
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if ((cifsFYI & CIFS_TIMER) && (midEntry->command != command)) {
			printk(KERN_DEBUG " CIFS slow rsp: cmd %d mid %llu",
>>>>>>> refs/remotes/origin/master
			       midEntry->command, midEntry->mid);
			printk(" A: 0x%lx S: 0x%lx R: 0x%lx\n",
			       now - midEntry->when_alloc,
			       now - midEntry->when_sent,
			       now - midEntry->when_received);
		}
	}
#endif
	mempool_free(midEntry, cifs_mid_poolp);
}

<<<<<<< HEAD
static void
delete_mid(struct mid_q_entry *mid)
=======
void
cifs_delete_mid(struct mid_q_entry *mid)
>>>>>>> refs/remotes/origin/master
{
	spin_lock(&GlobalMid_Lock);
	list_del(&mid->qhead);
	spin_unlock(&GlobalMid_Lock);

	DeleteMidQEntry(mid);
}

<<<<<<< HEAD
static int
smb_sendv(struct TCP_Server_Info *server, struct kvec *iov, int n_vec)
=======
/*
 * smb_send_kvec - send an array of kvecs to the server
 * @server:	Server to send the data to
 * @iov:	Pointer to array of kvecs
 * @n_vec:	length of kvec array
 * @sent:	amount of data sent on socket is stored here
 *
 * Our basic "send data to server" function. Should be called with srv_mutex
 * held. The caller is responsible for handling the results.
 */
static int
smb_send_kvec(struct TCP_Server_Info *server, struct kvec *iov, size_t n_vec,
		size_t *sent)
>>>>>>> refs/remotes/origin/master
{
	int rc = 0;
	int i = 0;
	struct msghdr smb_msg;
<<<<<<< HEAD
<<<<<<< HEAD
	struct smb_hdr *smb_buffer = iov[0].iov_base;
	unsigned int len = iov[0].iov_len;
	unsigned int total_len;
	int first_vec = 0;
	unsigned int smb_buf_length = be32_to_cpu(smb_buffer->smb_buf_length);
=======
	__be32 *buf_len = (__be32 *)(iov[0].iov_base);
	unsigned int len = iov[0].iov_len;
	unsigned int total_len;
	int first_vec = 0;
	unsigned int smb_buf_length = get_rfc1002_length(iov[0].iov_base);
>>>>>>> refs/remotes/origin/cm-10.0
	struct socket *ssocket = server->ssocket;

	if (ssocket == NULL)
		return -ENOTSOCK; /* BB eventually add reconnect code here */
=======
	unsigned int remaining;
	size_t first_vec = 0;
	struct socket *ssocket = server->ssocket;

	*sent = 0;
>>>>>>> refs/remotes/origin/master

	smb_msg.msg_name = (struct sockaddr *) &server->dstaddr;
	smb_msg.msg_namelen = sizeof(struct sockaddr);
	smb_msg.msg_control = NULL;
	smb_msg.msg_controllen = 0;
	if (server->noblocksnd)
		smb_msg.msg_flags = MSG_DONTWAIT + MSG_NOSIGNAL;
	else
		smb_msg.msg_flags = MSG_NOSIGNAL;

<<<<<<< HEAD
	total_len = 0;
	for (i = 0; i < n_vec; i++)
		total_len += iov[i].iov_len;

	cFYI(1, "Sending smb:  total_len %d", total_len);
<<<<<<< HEAD
	dump_smb(smb_buffer, len);
=======
	dump_smb(iov[0].iov_base, len);
>>>>>>> refs/remotes/origin/cm-10.0

	i = 0;
	while (total_len) {
		rc = kernel_sendmsg(ssocket, &smb_msg, &iov[first_vec],
				    n_vec - first_vec, total_len);
		if ((rc == -ENOSPC) || (rc == -EAGAIN)) {
			i++;
<<<<<<< HEAD
			/* if blocking send we try 3 times, since each can block
			   for 5 seconds. For nonblocking  we have to try more
			   but wait increasing amounts of time allowing time for
			   socket to clear.  The overall time we wait in either
			   case to send on the socket is about 15 seconds.
			   Similarly we wait for 15 seconds for
			   a response from the server in SendReceive[2]
			   for the server to send a response back for
			   most types of requests (except SMB Write
			   past end of file which can be slow, and
			   blocking lock operations). NFS waits slightly longer
			   than CIFS, but this can make it take longer for
			   nonresponsive servers to be detected and 15 seconds
			   is more than enough time for modern networks to
			   send a packet.  In most cases if we fail to send
			   after the retries we will kill the socket and
			   reconnect which may clear the network problem.
			*/
=======
			/*
			 * If blocking send we try 3 times, since each can block
			 * for 5 seconds. For nonblocking  we have to try more
			 * but wait increasing amounts of time allowing time for
			 * socket to clear.  The overall time we wait in either
			 * case to send on the socket is about 15 seconds.
			 * Similarly we wait for 15 seconds for a response from
			 * the server in SendReceive[2] for the server to send
			 * a response back for most types of requests (except
			 * SMB Write past end of file which can be slow, and
			 * blocking lock operations). NFS waits slightly longer
			 * than CIFS, but this can make it take longer for
			 * nonresponsive servers to be detected and 15 seconds
			 * is more than enough time for modern networks to
			 * send a packet.  In most cases if we fail to send
			 * after the retries we will kill the socket and
			 * reconnect which may clear the network problem.
			 */
>>>>>>> refs/remotes/origin/cm-10.0
			if ((i >= 14) || (!server->noblocksnd && (i > 2))) {
				cERROR(1, "sends on sock %p stuck for 15 seconds",
				    ssocket);
=======
	remaining = 0;
	for (i = 0; i < n_vec; i++)
		remaining += iov[i].iov_len;

	i = 0;
	while (remaining) {
		/*
		 * If blocking send, we try 3 times, since each can block
		 * for 5 seconds. For nonblocking  we have to try more
		 * but wait increasing amounts of time allowing time for
		 * socket to clear.  The overall time we wait in either
		 * case to send on the socket is about 15 seconds.
		 * Similarly we wait for 15 seconds for a response from
		 * the server in SendReceive[2] for the server to send
		 * a response back for most types of requests (except
		 * SMB Write past end of file which can be slow, and
		 * blocking lock operations). NFS waits slightly longer
		 * than CIFS, but this can make it take longer for
		 * nonresponsive servers to be detected and 15 seconds
		 * is more than enough time for modern networks to
		 * send a packet.  In most cases if we fail to send
		 * after the retries we will kill the socket and
		 * reconnect which may clear the network problem.
		 */
		rc = kernel_sendmsg(ssocket, &smb_msg, &iov[first_vec],
				    n_vec - first_vec, remaining);
		if (rc == -EAGAIN) {
			i++;
			if (i >= 14 || (!server->noblocksnd && (i > 2))) {
				cifs_dbg(VFS, "sends on sock %p stuck for 15 seconds\n",
					 ssocket);
>>>>>>> refs/remotes/origin/master
				rc = -EAGAIN;
				break;
			}
			msleep(1 << i);
			continue;
		}
<<<<<<< HEAD
		if (rc < 0)
			break;

		if (rc == total_len) {
			total_len = 0;
			break;
		} else if (rc > total_len) {
			cERROR(1, "sent %d requested %d", rc, total_len);
			break;
		}
		if (rc == 0) {
			/* should never happen, letting socket clear before
			   retrying is our only obvious option here */
			cERROR(1, "tcp sent no data");
			msleep(500);
			continue;
		}
		total_len -= rc;
=======

		if (rc < 0)
			break;

		/* send was at least partially successful */
		*sent += rc;

		if (rc == remaining) {
			remaining = 0;
			break;
		}

		if (rc > remaining) {
			cifs_dbg(VFS, "sent %d requested %d\n", rc, remaining);
			break;
		}

		if (rc == 0) {
			/* should never happen, letting socket clear before
			   retrying is our only obvious option here */
			cifs_dbg(VFS, "tcp sent no data\n");
			msleep(500);
			continue;
		}

		remaining -= rc;

>>>>>>> refs/remotes/origin/master
		/* the line below resets i */
		for (i = first_vec; i < n_vec; i++) {
			if (iov[i].iov_len) {
				if (rc > iov[i].iov_len) {
					rc -= iov[i].iov_len;
					iov[i].iov_len = 0;
				} else {
					iov[i].iov_base += rc;
					iov[i].iov_len -= rc;
					first_vec = i;
					break;
				}
			}
		}
<<<<<<< HEAD
		i = 0; /* in case we get ENOSPC on the next send */
	}

	if ((total_len > 0) && (total_len != smb_buf_length + 4)) {
		cFYI(1, "partial send (%d remaining), terminating session",
			total_len);
		/* If we have only sent part of an SMB then the next SMB
		   could be taken as the remainder of this one.  We need
		   to kill the socket so the server throws away the partial
		   SMB */
=======

		i = 0; /* in case we get ENOSPC on the next send */
		rc = 0;
	}
	return rc;
}

/**
 * rqst_page_to_kvec - Turn a slot in the smb_rqst page array into a kvec
 * @rqst: pointer to smb_rqst
 * @idx: index into the array of the page
 * @iov: pointer to struct kvec that will hold the result
 *
 * Helper function to convert a slot in the rqst->rq_pages array into a kvec.
 * The page will be kmapped and the address placed into iov_base. The length
 * will then be adjusted according to the ptailoff.
 */
void
cifs_rqst_page_to_kvec(struct smb_rqst *rqst, unsigned int idx,
			struct kvec *iov)
{
	/*
	 * FIXME: We could avoid this kmap altogether if we used
	 * kernel_sendpage instead of kernel_sendmsg. That will only
	 * work if signing is disabled though as sendpage inlines the
	 * page directly into the fraglist. If userspace modifies the
	 * page after we calculate the signature, then the server will
	 * reject it and may break the connection. kernel_sendmsg does
	 * an extra copy of the data and avoids that issue.
	 */
	iov->iov_base = kmap(rqst->rq_pages[idx]);

	/* if last page, don't send beyond this offset into page */
	if (idx == (rqst->rq_npages - 1))
		iov->iov_len = rqst->rq_tailsz;
	else
		iov->iov_len = rqst->rq_pagesz;
}

static int
smb_send_rqst(struct TCP_Server_Info *server, struct smb_rqst *rqst)
{
	int rc;
	struct kvec *iov = rqst->rq_iov;
	int n_vec = rqst->rq_nvec;
	unsigned int smb_buf_length = get_rfc1002_length(iov[0].iov_base);
	unsigned int i;
	size_t total_len = 0, sent;
	struct socket *ssocket = server->ssocket;
	int val = 1;

	if (ssocket == NULL)
		return -ENOTSOCK;

	cifs_dbg(FYI, "Sending smb: smb_len=%u\n", smb_buf_length);
	dump_smb(iov[0].iov_base, iov[0].iov_len);

	/* cork the socket */
	kernel_setsockopt(ssocket, SOL_TCP, TCP_CORK,
				(char *)&val, sizeof(val));

	rc = smb_send_kvec(server, iov, n_vec, &sent);
	if (rc < 0)
		goto uncork;

	total_len += sent;

	/* now walk the page array and send each page in it */
	for (i = 0; i < rqst->rq_npages; i++) {
		struct kvec p_iov;

		cifs_rqst_page_to_kvec(rqst, i, &p_iov);
		rc = smb_send_kvec(server, &p_iov, 1, &sent);
		kunmap(rqst->rq_pages[i]);
		if (rc < 0)
			break;

		total_len += sent;
	}

uncork:
	/* uncork it */
	val = 0;
	kernel_setsockopt(ssocket, SOL_TCP, TCP_CORK,
				(char *)&val, sizeof(val));

	if ((total_len > 0) && (total_len != smb_buf_length + 4)) {
		cifs_dbg(FYI, "partial send (wanted=%u sent=%zu): terminating session\n",
			 smb_buf_length + 4, total_len);
		/*
		 * If we have only sent part of an SMB then the next SMB could
		 * be taken as the remainder of this one. We need to kill the
		 * socket so the server throws away the partial SMB
		 */
>>>>>>> refs/remotes/origin/master
		server->tcpStatus = CifsNeedReconnect;
	}

	if (rc < 0 && rc != -EINTR)
<<<<<<< HEAD
		cERROR(1, "Error %d sending data on socket to server", rc);
	else
		rc = 0;

<<<<<<< HEAD
	/* Don't want to modify the buffer as a
	   side effect of this call. */
	smb_buffer->smb_buf_length = cpu_to_be32(smb_buf_length);
=======
	/* Don't want to modify the buffer as a side effect of this call. */
	*buf_len = cpu_to_be32(smb_buf_length);
>>>>>>> refs/remotes/origin/cm-10.0

	return rc;
}

=======
		cifs_dbg(VFS, "Error %d sending data on socket to server\n",
			 rc);
	else
		rc = 0;

	return rc;
}

static int
smb_sendv(struct TCP_Server_Info *server, struct kvec *iov, int n_vec)
{
	struct smb_rqst rqst = { .rq_iov = iov,
				 .rq_nvec = n_vec };

	return smb_send_rqst(server, &rqst);
}

>>>>>>> refs/remotes/origin/master
int
smb_send(struct TCP_Server_Info *server, struct smb_hdr *smb_buffer,
	 unsigned int smb_buf_length)
{
	struct kvec iov;

	iov.iov_base = smb_buffer;
	iov.iov_len = smb_buf_length + 4;

	return smb_sendv(server, &iov, 1);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int wait_for_free_request(struct TCP_Server_Info *server,
				 const int long_op)
{
	if (long_op == CIFS_ASYNC_OP) {
		/* oplock breaks must not be held up */
		atomic_inc(&server->inFlight);
		return 0;
	}

	spin_lock(&GlobalMid_Lock);
	while (1) {
		if (atomic_read(&server->inFlight) >= cifs_max_pending) {
			spin_unlock(&GlobalMid_Lock);
#ifdef CONFIG_CIFS_STATS2
			atomic_inc(&server->num_waiters);
#endif
			wait_event(server->request_q,
				   atomic_read(&server->inFlight)
				     < cifs_max_pending);
#ifdef CONFIG_CIFS_STATS2
			atomic_dec(&server->num_waiters);
#endif
			spin_lock(&GlobalMid_Lock);
		} else {
			if (server->tcpStatus == CifsExiting) {
				spin_unlock(&GlobalMid_Lock);
				return -ENOENT;
			}

			/* can not count locking commands against total
			   as they are allowed to block on server */

			/* update # of requests on the wire to server */
			if (long_op != CIFS_BLOCKING_OP)
				atomic_inc(&server->inFlight);
			spin_unlock(&GlobalMid_Lock);
=======
static int
wait_for_free_credits(struct TCP_Server_Info *server, const int optype,
=======
static int
wait_for_free_credits(struct TCP_Server_Info *server, const int timeout,
>>>>>>> refs/remotes/origin/master
		      int *credits)
{
	int rc;

	spin_lock(&server->req_lock);
<<<<<<< HEAD
	if (optype == CIFS_ASYNC_OP) {
=======
	if (timeout == CIFS_ASYNC_OP) {
>>>>>>> refs/remotes/origin/master
		/* oplock breaks must not be held up */
		server->in_flight++;
		*credits -= 1;
		spin_unlock(&server->req_lock);
		return 0;
	}

	while (1) {
		if (*credits <= 0) {
			spin_unlock(&server->req_lock);
			cifs_num_waiters_inc(server);
			rc = wait_event_killable(server->request_q,
						 has_credits(server, credits));
			cifs_num_waiters_dec(server);
			if (rc)
				return rc;
			spin_lock(&server->req_lock);
		} else {
			if (server->tcpStatus == CifsExiting) {
				spin_unlock(&server->req_lock);
				return -ENOENT;
			}

			/*
			 * Can not count locking commands against total
			 * as they are allowed to block on server.
			 */

			/* update # of requests on the wire to server */
<<<<<<< HEAD
			if (optype != CIFS_BLOCKING_OP) {
=======
			if (timeout != CIFS_BLOCKING_OP) {
>>>>>>> refs/remotes/origin/master
				*credits -= 1;
				server->in_flight++;
			}
			spin_unlock(&server->req_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			break;
		}
	}
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int
wait_for_free_request(struct TCP_Server_Info *server, const int optype)
{
	return wait_for_free_credits(server, optype, get_credits_field(server));
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static int
wait_for_free_request(struct TCP_Server_Info *server, const int timeout,
		      const int optype)
{
	int *val;

	val = server->ops->get_credits_field(server, optype);
	/* Since an echo is already inflight, no need to wait to send another */
	if (*val <= 0 && optype == CIFS_ECHO_OP)
		return -EAGAIN;
	return wait_for_free_credits(server, timeout, val);
}

>>>>>>> refs/remotes/origin/master
static int allocate_mid(struct cifs_ses *ses, struct smb_hdr *in_buf,
			struct mid_q_entry **ppmidQ)
{
	if (ses->server->tcpStatus == CifsExiting) {
		return -ENOENT;
	}

	if (ses->server->tcpStatus == CifsNeedReconnect) {
<<<<<<< HEAD
		cFYI(1, "tcp session dead - return to caller to retry");
		return -EAGAIN;
	}

	if (ses->status != CifsGood) {
		/* check if SMB session is bad because we are setting it up */
=======
		cifs_dbg(FYI, "tcp session dead - return to caller to retry\n");
		return -EAGAIN;
	}

	if (ses->status == CifsNew) {
>>>>>>> refs/remotes/origin/master
		if ((in_buf->Command != SMB_COM_SESSION_SETUP_ANDX) &&
			(in_buf->Command != SMB_COM_NEGOTIATE))
			return -EAGAIN;
		/* else ok - we are setting up session */
	}
<<<<<<< HEAD
=======

	if (ses->status == CifsExiting) {
		/* check if SMB session is bad because we are setting it up */
		if (in_buf->Command != SMB_COM_LOGOFF_ANDX)
			return -EAGAIN;
		/* else ok - we are shutting down session */
	}

>>>>>>> refs/remotes/origin/master
	*ppmidQ = AllocMidQEntry(in_buf, ses->server);
	if (*ppmidQ == NULL)
		return -ENOMEM;
	spin_lock(&GlobalMid_Lock);
	list_add_tail(&(*ppmidQ)->qhead, &ses->server->pending_mid_q);
	spin_unlock(&GlobalMid_Lock);
	return 0;
}

static int
wait_for_response(struct TCP_Server_Info *server, struct mid_q_entry *midQ)
{
	int error;

<<<<<<< HEAD
<<<<<<< HEAD
	error = wait_event_killable(server->response_q,
				    midQ->midState != MID_REQUEST_SUBMITTED);
=======
	error = wait_event_freezekillable(server->response_q,
				    midQ->mid_state != MID_REQUEST_SUBMITTED);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = wait_event_freezekillable_unsafe(server->response_q,
				    midQ->mid_state != MID_REQUEST_SUBMITTED);
>>>>>>> refs/remotes/origin/master
	if (error < 0)
		return -ERESTARTSYS;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD

/*
 * Send a SMB request and set the callback function in the mid to handle
 * the result. Caller is responsible for dealing with timeouts.
 */
int
cifs_call_async(struct TCP_Server_Info *server, struct kvec *iov,
		unsigned int nvec, mid_callback_t *callback, void *cbdata,
		bool ignore_pend)
{
	int rc;
	struct mid_q_entry *mid;
	struct smb_hdr *hdr = (struct smb_hdr *)iov[0].iov_base;

	rc = wait_for_free_request(server, ignore_pend ? CIFS_ASYNC_OP : 0);
	if (rc)
		return rc;
=======
static int
cifs_setup_async_request(struct TCP_Server_Info *server, struct kvec *iov,
			 unsigned int nvec, struct mid_q_entry **ret_mid)
{
	int rc;
	struct smb_hdr *hdr = (struct smb_hdr *)iov[0].iov_base;
	struct mid_q_entry *mid;
>>>>>>> refs/remotes/origin/cm-10.0

	/* enable signing if server requires it */
	if (server->sec_mode & (SECMODE_SIGN_REQUIRED | SECMODE_SIGN_ENABLED))
		hdr->Flags2 |= SMBFLG2_SECURITY_SIGNATURE;

<<<<<<< HEAD
	mutex_lock(&server->srv_mutex);
	mid = AllocMidQEntry(hdr, server);
	if (mid == NULL) {
		mutex_unlock(&server->srv_mutex);
		atomic_dec(&server->inFlight);
		wake_up(&server->request_q);
		return -ENOMEM;
	}
=======
	mid = AllocMidQEntry(hdr, server);
	if (mid == NULL)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0

	/* put it on the pending_mid_q */
	spin_lock(&GlobalMid_Lock);
	list_add_tail(&mid->qhead, &server->pending_mid_q);
	spin_unlock(&GlobalMid_Lock);

	rc = cifs_sign_smb2(iov, nvec, server, &mid->sequence_number);
<<<<<<< HEAD
	if (rc) {
		mutex_unlock(&server->srv_mutex);
		goto out_err;
	}

	mid->callback = callback;
	mid->callback_data = cbdata;
	mid->midState = MID_REQUEST_SUBMITTED;
#ifdef CONFIG_CIFS_STATS2
	atomic_inc(&server->inSend);
#endif
	rc = smb_sendv(server, iov, nvec);
#ifdef CONFIG_CIFS_STATS2
	atomic_dec(&server->inSend);
	mid->when_sent = jiffies;
#endif
	mutex_unlock(&server->srv_mutex);
=======
	if (rc)
		delete_mid(mid);
	*ret_mid = mid;
	return rc;
=======
struct mid_q_entry *
cifs_setup_async_request(struct TCP_Server_Info *server, struct smb_rqst *rqst)
{
	int rc;
	struct smb_hdr *hdr = (struct smb_hdr *)rqst->rq_iov[0].iov_base;
	struct mid_q_entry *mid;

	/* enable signing if server requires it */
	if (server->sign)
		hdr->Flags2 |= SMBFLG2_SECURITY_SIGNATURE;

	mid = AllocMidQEntry(hdr, server);
	if (mid == NULL)
		return ERR_PTR(-ENOMEM);

	rc = cifs_sign_rqst(rqst, server, &mid->sequence_number);
	if (rc) {
		DeleteMidQEntry(mid);
		return ERR_PTR(rc);
	}

	return mid;
>>>>>>> refs/remotes/origin/master
}

/*
 * Send a SMB request and set the callback function in the mid to handle
 * the result. Caller is responsible for dealing with timeouts.
 */
int
<<<<<<< HEAD
cifs_call_async(struct TCP_Server_Info *server, struct kvec *iov,
		unsigned int nvec, mid_receive_t *receive,
		mid_callback_t *callback, void *cbdata, bool ignore_pend)
{
	int rc;
	struct mid_q_entry *mid;

	rc = wait_for_free_request(server, ignore_pend ? CIFS_ASYNC_OP : 0);
=======
cifs_call_async(struct TCP_Server_Info *server, struct smb_rqst *rqst,
		mid_receive_t *receive, mid_callback_t *callback,
		void *cbdata, const int flags)
{
	int rc, timeout, optype;
	struct mid_q_entry *mid;

	timeout = flags & CIFS_TIMEOUT_MASK;
	optype = flags & CIFS_OP_MASK;

	rc = wait_for_free_request(server, timeout, optype);
>>>>>>> refs/remotes/origin/master
	if (rc)
		return rc;

	mutex_lock(&server->srv_mutex);
<<<<<<< HEAD
	rc = cifs_setup_async_request(server, iov, nvec, &mid);
	if (rc) {
		mutex_unlock(&server->srv_mutex);
		cifs_add_credits(server, 1);
		wake_up(&server->request_q);
		return rc;
=======
	mid = server->ops->setup_async_request(server, rqst);
	if (IS_ERR(mid)) {
		mutex_unlock(&server->srv_mutex);
		add_credits(server, 1, optype);
		wake_up(&server->request_q);
		return PTR_ERR(mid);
>>>>>>> refs/remotes/origin/master
	}

	mid->receive = receive;
	mid->callback = callback;
	mid->callback_data = cbdata;
	mid->mid_state = MID_REQUEST_SUBMITTED;

<<<<<<< HEAD
	cifs_in_send_inc(server);
	rc = smb_sendv(server, iov, nvec);
	cifs_in_send_dec(server);
	cifs_save_when_sent(mid);
	mutex_unlock(&server->srv_mutex);

>>>>>>> refs/remotes/origin/cm-10.0
	if (rc)
		goto out_err;

	return rc;
out_err:
	delete_mid(mid);
<<<<<<< HEAD
	atomic_dec(&server->inFlight);
=======
	cifs_add_credits(server, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* put it on the pending_mid_q */
	spin_lock(&GlobalMid_Lock);
	list_add_tail(&mid->qhead, &server->pending_mid_q);
	spin_unlock(&GlobalMid_Lock);


	cifs_in_send_inc(server);
	rc = smb_send_rqst(server, rqst);
	cifs_in_send_dec(server);
	cifs_save_when_sent(mid);

	if (rc < 0)
		server->sequence_number -= 2;
	mutex_unlock(&server->srv_mutex);

	if (rc == 0)
		return 0;

	cifs_delete_mid(mid);
	add_credits(server, 1, optype);
>>>>>>> refs/remotes/origin/master
	wake_up(&server->request_q);
	return rc;
}

/*
 *
 * Send an SMB Request.  No response info (other than return code)
 * needs to be parsed.
 *
 * flags indicate the type of request buffer and how long to wait
 * and whether to log NT STATUS code (error) before mapping it to POSIX error
 *
 */
int
SendReceiveNoRsp(const unsigned int xid, struct cifs_ses *ses,
<<<<<<< HEAD
<<<<<<< HEAD
		struct smb_hdr *in_buf, int flags)
=======
		 char *in_buf, int flags)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 char *in_buf, int flags)
>>>>>>> refs/remotes/origin/master
{
	int rc;
	struct kvec iov[1];
	int resp_buf_type;

<<<<<<< HEAD
<<<<<<< HEAD
	iov[0].iov_base = (char *)in_buf;
	iov[0].iov_len = be32_to_cpu(in_buf->smb_buf_length) + 4;
=======
	iov[0].iov_base = in_buf;
	iov[0].iov_len = get_rfc1002_length(in_buf) + 4;
>>>>>>> refs/remotes/origin/cm-10.0
	flags |= CIFS_NO_RESP;
	rc = SendReceive2(xid, ses, iov, 1, &resp_buf_type, flags);
	cFYI(DBG2, "SendRcvNoRsp flags %d rc %d", flags, rc);
=======
	iov[0].iov_base = in_buf;
	iov[0].iov_len = get_rfc1002_length(in_buf) + 4;
	flags |= CIFS_NO_RESP;
	rc = SendReceive2(xid, ses, iov, 1, &resp_buf_type, flags);
	cifs_dbg(NOISY, "SendRcvNoRsp flags %d rc %d\n", flags, rc);
>>>>>>> refs/remotes/origin/master

	return rc;
}

static int
cifs_sync_mid_result(struct mid_q_entry *mid, struct TCP_Server_Info *server)
{
	int rc = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	cFYI(1, "%s: cmd=%d mid=%d state=%d", __func__, mid->command,
		mid->mid, mid->midState);

	spin_lock(&GlobalMid_Lock);
	switch (mid->midState) {
=======
	cFYI(1, "%s: cmd=%d mid=%llu state=%d", __func__,
	     le16_to_cpu(mid->command), mid->mid, mid->mid_state);

	spin_lock(&GlobalMid_Lock);
	switch (mid->mid_state) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cifs_dbg(FYI, "%s: cmd=%d mid=%llu state=%d\n",
		 __func__, le16_to_cpu(mid->command), mid->mid, mid->mid_state);

	spin_lock(&GlobalMid_Lock);
	switch (mid->mid_state) {
>>>>>>> refs/remotes/origin/master
	case MID_RESPONSE_RECEIVED:
		spin_unlock(&GlobalMid_Lock);
		return rc;
	case MID_RETRY_NEEDED:
		rc = -EAGAIN;
		break;
	case MID_RESPONSE_MALFORMED:
		rc = -EIO;
		break;
	case MID_SHUTDOWN:
		rc = -EHOSTDOWN;
		break;
	default:
		list_del_init(&mid->qhead);
<<<<<<< HEAD
<<<<<<< HEAD
		cERROR(1, "%s: invalid mid state mid=%d state=%d", __func__,
			mid->mid, mid->midState);
=======
		cERROR(1, "%s: invalid mid state mid=%llu state=%d", __func__,
		       mid->mid, mid->mid_state);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cifs_dbg(VFS, "%s: invalid mid state mid=%llu state=%d\n",
			 __func__, mid->mid, mid->mid_state);
>>>>>>> refs/remotes/origin/master
		rc = -EIO;
	}
	spin_unlock(&GlobalMid_Lock);

	DeleteMidQEntry(mid);
	return rc;
}

<<<<<<< HEAD
/*
 * An NT cancel request header looks just like the original request except:
 *
 * The Command is SMB_COM_NT_CANCEL
 * The WordCount is zeroed out
 * The ByteCount is zeroed out
 *
 * This function mangles an existing request buffer into a
 * SMB_COM_NT_CANCEL request and then sends it.
 */
static int
send_nt_cancel(struct TCP_Server_Info *server, struct smb_hdr *in_buf,
		struct mid_q_entry *mid)
{
	int rc = 0;

	/* -4 for RFC1001 length and +2 for BCC field */
	in_buf->smb_buf_length = cpu_to_be32(sizeof(struct smb_hdr) - 4  + 2);
	in_buf->Command = SMB_COM_NT_CANCEL;
	in_buf->WordCount = 0;
	put_bcc(0, in_buf);

	mutex_lock(&server->srv_mutex);
	rc = cifs_sign_smb(in_buf, server, &mid->sequence_number);
	if (rc) {
		mutex_unlock(&server->srv_mutex);
		return rc;
	}
	rc = smb_send(server, in_buf, be32_to_cpu(in_buf->smb_buf_length));
	mutex_unlock(&server->srv_mutex);

	cFYI(1, "issued NT_CANCEL for mid %u, rc = %d",
		in_buf->Mid, rc);

	return rc;
=======
static inline int
send_cancel(struct TCP_Server_Info *server, void *buf, struct mid_q_entry *mid)
{
	return server->ops->send_cancel ?
				server->ops->send_cancel(server, buf, mid) : 0;
>>>>>>> refs/remotes/origin/master
}

int
cifs_check_receive(struct mid_q_entry *mid, struct TCP_Server_Info *server,
		   bool log_error)
{
<<<<<<< HEAD
<<<<<<< HEAD
	dump_smb(mid->resp_buf,
		 min_t(u32, 92, be32_to_cpu(mid->resp_buf->smb_buf_length)));

	/* convert the length into a more usable form */
	if (server->sec_mode & (SECMODE_SIGN_REQUIRED | SECMODE_SIGN_ENABLED)) {
		/* FIXME: add code to kill session */
		if (cifs_verify_signature(mid->resp_buf, server,
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned int len = get_rfc1002_length(mid->resp_buf) + 4;

	dump_smb(mid->resp_buf, min_t(u32, 92, len));

	/* convert the length into a more usable form */
<<<<<<< HEAD
	if (server->sec_mode & (SECMODE_SIGN_REQUIRED | SECMODE_SIGN_ENABLED)) {
		struct kvec iov;
=======
	if (server->sign) {
		struct kvec iov;
		int rc = 0;
		struct smb_rqst rqst = { .rq_iov = &iov,
					 .rq_nvec = 1 };
>>>>>>> refs/remotes/origin/master

		iov.iov_base = mid->resp_buf;
		iov.iov_len = len;
		/* FIXME: add code to kill session */
<<<<<<< HEAD
		if (cifs_verify_signature(&iov, 1, server,
>>>>>>> refs/remotes/origin/cm-10.0
					  mid->sequence_number + 1) != 0)
			cERROR(1, "Unexpected SMB signature");
=======
		rc = cifs_verify_signature(&rqst, server,
					   mid->sequence_number);
		if (rc)
			cifs_dbg(VFS, "SMB signature verification returned error = %d\n",
				 rc);
>>>>>>> refs/remotes/origin/master
	}

	/* BB special case reconnect tid and uid here? */
	return map_smb_to_linux_error(mid->resp_buf, log_error);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int
cifs_setup_request(struct cifs_ses *ses, struct kvec *iov,
		   unsigned int nvec, struct mid_q_entry **ret_mid)
{
	int rc;
	struct smb_hdr *hdr = (struct smb_hdr *)iov[0].iov_base;
=======
struct mid_q_entry *
cifs_setup_request(struct cifs_ses *ses, struct smb_rqst *rqst)
{
	int rc;
	struct smb_hdr *hdr = (struct smb_hdr *)rqst->rq_iov[0].iov_base;
>>>>>>> refs/remotes/origin/master
	struct mid_q_entry *mid;

	rc = allocate_mid(ses, hdr, &mid);
	if (rc)
<<<<<<< HEAD
		return rc;
	rc = cifs_sign_smb2(iov, nvec, ses->server, &mid->sequence_number);
	if (rc)
		delete_mid(mid);
	*ret_mid = mid;
	return rc;
}

>>>>>>> refs/remotes/origin/cm-10.0
int
SendReceive2(const unsigned int xid, struct cifs_ses *ses,
	     struct kvec *iov, int n_vec, int *pRespBufType /* ret */,
	     const int flags)
{
	int rc = 0;
	int long_op;
	struct mid_q_entry *midQ;
<<<<<<< HEAD
	struct smb_hdr *in_buf = iov[0].iov_base;
=======
	char *buf = iov[0].iov_base;
>>>>>>> refs/remotes/origin/cm-10.0

	long_op = flags & CIFS_TIMEOUT_MASK;

	*pRespBufType = CIFS_NO_BUFFER;  /* no response buf yet */

	if ((ses == NULL) || (ses->server == NULL)) {
<<<<<<< HEAD
		cifs_small_buf_release(in_buf);
=======
		cifs_small_buf_release(buf);
>>>>>>> refs/remotes/origin/cm-10.0
		cERROR(1, "Null session");
=======
		return ERR_PTR(rc);
	rc = cifs_sign_rqst(rqst, ses->server, &mid->sequence_number);
	if (rc) {
		cifs_delete_mid(mid);
		return ERR_PTR(rc);
	}
	return mid;
}

int
SendReceive2(const unsigned int xid, struct cifs_ses *ses,
	     struct kvec *iov, int n_vec, int *resp_buf_type /* ret */,
	     const int flags)
{
	int rc = 0;
	int timeout, optype;
	struct mid_q_entry *midQ;
	char *buf = iov[0].iov_base;
	unsigned int credits = 1;
	struct smb_rqst rqst = { .rq_iov = iov,
				 .rq_nvec = n_vec };

	timeout = flags & CIFS_TIMEOUT_MASK;
	optype = flags & CIFS_OP_MASK;

	*resp_buf_type = CIFS_NO_BUFFER;  /* no response buf yet */

	if ((ses == NULL) || (ses->server == NULL)) {
		cifs_small_buf_release(buf);
		cifs_dbg(VFS, "Null session\n");
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}

	if (ses->server->tcpStatus == CifsExiting) {
<<<<<<< HEAD
<<<<<<< HEAD
		cifs_small_buf_release(in_buf);
		return -ENOENT;
	}

	/* Ensure that we do not send more than 50 overlapping requests
	   to the same server. We may make this configurable later or
	   use ses->maxReq */

	rc = wait_for_free_request(ses->server, long_op);
	if (rc) {
		cifs_small_buf_release(in_buf);
		return rc;
	}

	/* make sure that we sign in the same order that we send on this socket
	   and avoid races inside tcp sendmsg code that could cause corruption
	   of smb data */

	mutex_lock(&ses->server->srv_mutex);

	rc = allocate_mid(ses, in_buf, &midQ);
	if (rc) {
		mutex_unlock(&ses->server->srv_mutex);
		cifs_small_buf_release(in_buf);
		/* Update # of requests on wire to server */
		atomic_dec(&ses->server->inFlight);
		wake_up(&ses->server->request_q);
		return rc;
	}
	rc = cifs_sign_smb2(iov, n_vec, ses->server, &midQ->sequence_number);
	if (rc) {
		mutex_unlock(&ses->server->srv_mutex);
		cifs_small_buf_release(in_buf);
		goto out;
	}

	midQ->midState = MID_REQUEST_SUBMITTED;
#ifdef CONFIG_CIFS_STATS2
	atomic_inc(&ses->server->inSend);
#endif
	rc = smb_sendv(ses->server, iov, n_vec);
#ifdef CONFIG_CIFS_STATS2
	atomic_dec(&ses->server->inSend);
	midQ->when_sent = jiffies;
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
		cifs_small_buf_release(buf);
		return -ENOENT;
	}

	/*
	 * Ensure that we do not send more than 50 overlapping requests
	 * to the same server. We may make this configurable later or
	 * use ses->maxReq.
	 */

<<<<<<< HEAD
	rc = wait_for_free_request(ses->server, long_op);
=======
	rc = wait_for_free_request(ses->server, timeout, optype);
>>>>>>> refs/remotes/origin/master
	if (rc) {
		cifs_small_buf_release(buf);
		return rc;
	}

	/*
	 * Make sure that we sign in the same order that we send on this socket
	 * and avoid races inside tcp sendmsg code that could cause corruption
	 * of smb data.
	 */

	mutex_lock(&ses->server->srv_mutex);

<<<<<<< HEAD
	rc = cifs_setup_request(ses, iov, n_vec, &midQ);
	if (rc) {
		mutex_unlock(&ses->server->srv_mutex);
		cifs_small_buf_release(buf);
		/* Update # of requests on wire to server */
		cifs_add_credits(ses->server, 1);
		return rc;
=======
	midQ = ses->server->ops->setup_request(ses, &rqst);
	if (IS_ERR(midQ)) {
		mutex_unlock(&ses->server->srv_mutex);
		cifs_small_buf_release(buf);
		/* Update # of requests on wire to server */
		add_credits(ses->server, 1, optype);
		return PTR_ERR(midQ);
>>>>>>> refs/remotes/origin/master
	}

	midQ->mid_state = MID_REQUEST_SUBMITTED;
	cifs_in_send_inc(ses->server);
	rc = smb_sendv(ses->server, iov, n_vec);
	cifs_in_send_dec(ses->server);
	cifs_save_when_sent(midQ);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_unlock(&ses->server->srv_mutex);

	if (rc < 0) {
<<<<<<< HEAD
		cifs_small_buf_release(in_buf);
=======
		cifs_small_buf_release(buf);
>>>>>>> refs/remotes/origin/cm-10.0
		goto out;
	}

	if (long_op == CIFS_ASYNC_OP) {
<<<<<<< HEAD
		cifs_small_buf_release(in_buf);
=======
		cifs_small_buf_release(buf);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (rc < 0)
		ses->server->sequence_number -= 2;
	mutex_unlock(&ses->server->srv_mutex);

	if (rc < 0) {
		cifs_small_buf_release(buf);
		goto out;
	}

	if (timeout == CIFS_ASYNC_OP) {
		cifs_small_buf_release(buf);
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	rc = wait_for_response(ses->server, midQ);
	if (rc != 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		send_nt_cancel(ses->server, in_buf, midQ);
		spin_lock(&GlobalMid_Lock);
		if (midQ->midState == MID_REQUEST_SUBMITTED) {
			midQ->callback = DeleteMidQEntry;
			spin_unlock(&GlobalMid_Lock);
			cifs_small_buf_release(in_buf);
			atomic_dec(&ses->server->inFlight);
			wake_up(&ses->server->request_q);
=======
		send_nt_cancel(ses->server, (struct smb_hdr *)buf, midQ);
=======
		send_cancel(ses->server, buf, midQ);
>>>>>>> refs/remotes/origin/master
		spin_lock(&GlobalMid_Lock);
		if (midQ->mid_state == MID_REQUEST_SUBMITTED) {
			midQ->callback = DeleteMidQEntry;
			spin_unlock(&GlobalMid_Lock);
			cifs_small_buf_release(buf);
<<<<<<< HEAD
			cifs_add_credits(ses->server, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			add_credits(ses->server, 1, optype);
>>>>>>> refs/remotes/origin/master
			return rc;
		}
		spin_unlock(&GlobalMid_Lock);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	cifs_small_buf_release(in_buf);

	rc = cifs_sync_mid_result(midQ, ses->server);
	if (rc != 0) {
		atomic_dec(&ses->server->inFlight);
		wake_up(&ses->server->request_q);
		return rc;
	}

	if (!midQ->resp_buf || midQ->midState != MID_RESPONSE_RECEIVED) {
=======
=======
>>>>>>> refs/remotes/origin/master
	cifs_small_buf_release(buf);

	rc = cifs_sync_mid_result(midQ, ses->server);
	if (rc != 0) {
<<<<<<< HEAD
		cifs_add_credits(ses->server, 1);
=======
		add_credits(ses->server, 1, optype);
>>>>>>> refs/remotes/origin/master
		return rc;
	}

	if (!midQ->resp_buf || midQ->mid_state != MID_RESPONSE_RECEIVED) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		rc = -EIO;
		cFYI(1, "Bad MID state?");
		goto out;
	}

<<<<<<< HEAD
	iov[0].iov_base = (char *)midQ->resp_buf;
	iov[0].iov_len = be32_to_cpu(midQ->resp_buf->smb_buf_length) + 4;
	if (midQ->largeBuf)
=======
=======
		rc = -EIO;
		cifs_dbg(FYI, "Bad MID state?\n");
		goto out;
	}

>>>>>>> refs/remotes/origin/master
	buf = (char *)midQ->resp_buf;
	iov[0].iov_base = buf;
	iov[0].iov_len = get_rfc1002_length(buf) + 4;
	if (midQ->large_buf)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		*pRespBufType = CIFS_LARGE_BUFFER;
	else
		*pRespBufType = CIFS_SMALL_BUFFER;

	rc = cifs_check_receive(midQ, ses->server, flags & CIFS_LOG_ERROR);

	/* mark it so buf will not be freed by delete_mid */
	if ((flags & CIFS_NO_RESP) == 0)
		midQ->resp_buf = NULL;
out:
	delete_mid(midQ);
<<<<<<< HEAD
	atomic_dec(&ses->server->inFlight);
	wake_up(&ses->server->request_q);
=======
	cifs_add_credits(ses->server, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		*resp_buf_type = CIFS_LARGE_BUFFER;
	else
		*resp_buf_type = CIFS_SMALL_BUFFER;

	credits = ses->server->ops->get_credits(midQ);

	rc = ses->server->ops->check_receive(midQ, ses->server,
					     flags & CIFS_LOG_ERROR);

	/* mark it so buf will not be freed by cifs_delete_mid */
	if ((flags & CIFS_NO_RESP) == 0)
		midQ->resp_buf = NULL;
out:
	cifs_delete_mid(midQ);
	add_credits(ses->server, credits, optype);
>>>>>>> refs/remotes/origin/master

	return rc;
}

int
SendReceive(const unsigned int xid, struct cifs_ses *ses,
	    struct smb_hdr *in_buf, struct smb_hdr *out_buf,
<<<<<<< HEAD
	    int *pbytes_returned, const int long_op)
=======
	    int *pbytes_returned, const int timeout)
>>>>>>> refs/remotes/origin/master
{
	int rc = 0;
	struct mid_q_entry *midQ;

	if (ses == NULL) {
<<<<<<< HEAD
		cERROR(1, "Null smb session");
		return -EIO;
	}
	if (ses->server == NULL) {
		cERROR(1, "Null tcp session");
=======
		cifs_dbg(VFS, "Null smb session\n");
		return -EIO;
	}
	if (ses->server == NULL) {
		cifs_dbg(VFS, "Null tcp session\n");
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}

	if (ses->server->tcpStatus == CifsExiting)
		return -ENOENT;

	/* Ensure that we do not send more than 50 overlapping requests
	   to the same server. We may make this configurable later or
	   use ses->maxReq */

	if (be32_to_cpu(in_buf->smb_buf_length) > CIFSMaxBufSize +
			MAX_CIFS_HDR_SIZE - 4) {
<<<<<<< HEAD
		cERROR(1, "Illegal length, greater than maximum frame, %d",
			   be32_to_cpu(in_buf->smb_buf_length));
		return -EIO;
	}

	rc = wait_for_free_request(ses->server, long_op);
=======
		cifs_dbg(VFS, "Illegal length, greater than maximum frame, %d\n",
			 be32_to_cpu(in_buf->smb_buf_length));
		return -EIO;
	}

	rc = wait_for_free_request(ses->server, timeout, 0);
>>>>>>> refs/remotes/origin/master
	if (rc)
		return rc;

	/* make sure that we sign in the same order that we send on this socket
	   and avoid races inside tcp sendmsg code that could cause corruption
	   of smb data */

	mutex_lock(&ses->server->srv_mutex);

	rc = allocate_mid(ses, in_buf, &midQ);
	if (rc) {
		mutex_unlock(&ses->server->srv_mutex);
		/* Update # of requests on wire to server */
<<<<<<< HEAD
<<<<<<< HEAD
		atomic_dec(&ses->server->inFlight);
		wake_up(&ses->server->request_q);
=======
		cifs_add_credits(ses->server, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		add_credits(ses->server, 1, 0);
>>>>>>> refs/remotes/origin/master
		return rc;
	}

	rc = cifs_sign_smb(in_buf, ses->server, &midQ->sequence_number);
	if (rc) {
		mutex_unlock(&ses->server->srv_mutex);
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	midQ->midState = MID_REQUEST_SUBMITTED;
#ifdef CONFIG_CIFS_STATS2
	atomic_inc(&ses->server->inSend);
#endif
	rc = smb_send(ses->server, in_buf, be32_to_cpu(in_buf->smb_buf_length));
#ifdef CONFIG_CIFS_STATS2
	atomic_dec(&ses->server->inSend);
	midQ->when_sent = jiffies;
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
	midQ->mid_state = MID_REQUEST_SUBMITTED;

	cifs_in_send_inc(ses->server);
	rc = smb_send(ses->server, in_buf, be32_to_cpu(in_buf->smb_buf_length));
	cifs_in_send_dec(ses->server);
	cifs_save_when_sent(midQ);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (rc < 0)
		ses->server->sequence_number -= 2;

>>>>>>> refs/remotes/origin/master
	mutex_unlock(&ses->server->srv_mutex);

	if (rc < 0)
		goto out;

<<<<<<< HEAD
	if (long_op == CIFS_ASYNC_OP)
=======
	if (timeout == CIFS_ASYNC_OP)
>>>>>>> refs/remotes/origin/master
		goto out;

	rc = wait_for_response(ses->server, midQ);
	if (rc != 0) {
<<<<<<< HEAD
		send_nt_cancel(ses->server, in_buf, midQ);
		spin_lock(&GlobalMid_Lock);
<<<<<<< HEAD
		if (midQ->midState == MID_REQUEST_SUBMITTED) {
			/* no longer considered to be "in-flight" */
			midQ->callback = DeleteMidQEntry;
			spin_unlock(&GlobalMid_Lock);
			atomic_dec(&ses->server->inFlight);
			wake_up(&ses->server->request_q);
=======
=======
		send_cancel(ses->server, in_buf, midQ);
		spin_lock(&GlobalMid_Lock);
>>>>>>> refs/remotes/origin/master
		if (midQ->mid_state == MID_REQUEST_SUBMITTED) {
			/* no longer considered to be "in-flight" */
			midQ->callback = DeleteMidQEntry;
			spin_unlock(&GlobalMid_Lock);
<<<<<<< HEAD
			cifs_add_credits(ses->server, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			add_credits(ses->server, 1, 0);
>>>>>>> refs/remotes/origin/master
			return rc;
		}
		spin_unlock(&GlobalMid_Lock);
	}

	rc = cifs_sync_mid_result(midQ, ses->server);
	if (rc != 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		atomic_dec(&ses->server->inFlight);
		wake_up(&ses->server->request_q);
=======
		cifs_add_credits(ses->server, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		add_credits(ses->server, 1, 0);
>>>>>>> refs/remotes/origin/master
		return rc;
	}

	if (!midQ->resp_buf || !out_buf ||
<<<<<<< HEAD
<<<<<<< HEAD
	    midQ->midState != MID_RESPONSE_RECEIVED) {
=======
	    midQ->mid_state != MID_RESPONSE_RECEIVED) {
>>>>>>> refs/remotes/origin/cm-10.0
		rc = -EIO;
		cERROR(1, "Bad MID state?");
		goto out;
	}

<<<<<<< HEAD
	*pbytes_returned = be32_to_cpu(midQ->resp_buf->smb_buf_length);
=======
	*pbytes_returned = get_rfc1002_length(midQ->resp_buf);
>>>>>>> refs/remotes/origin/cm-10.0
	memcpy(out_buf, midQ->resp_buf, *pbytes_returned + 4);
	rc = cifs_check_receive(midQ, ses->server, 0);
out:
	delete_mid(midQ);
<<<<<<< HEAD
	atomic_dec(&ses->server->inFlight);
	wake_up(&ses->server->request_q);
=======
	cifs_add_credits(ses->server, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    midQ->mid_state != MID_RESPONSE_RECEIVED) {
		rc = -EIO;
		cifs_dbg(VFS, "Bad MID state?\n");
		goto out;
	}

	*pbytes_returned = get_rfc1002_length(midQ->resp_buf);
	memcpy(out_buf, midQ->resp_buf, *pbytes_returned + 4);
	rc = cifs_check_receive(midQ, ses->server, 0);
out:
	cifs_delete_mid(midQ);
	add_credits(ses->server, 1, 0);
>>>>>>> refs/remotes/origin/master

	return rc;
}

/* We send a LOCKINGX_CANCEL_LOCK to cause the Windows
   blocking lock to return. */

static int
send_lock_cancel(const unsigned int xid, struct cifs_tcon *tcon,
			struct smb_hdr *in_buf,
			struct smb_hdr *out_buf)
{
	int bytes_returned;
	struct cifs_ses *ses = tcon->ses;
	LOCK_REQ *pSMB = (LOCK_REQ *)in_buf;

	/* We just modify the current in_buf to change
	   the type of lock from LOCKING_ANDX_SHARED_LOCK
	   or LOCKING_ANDX_EXCLUSIVE_LOCK to
	   LOCKING_ANDX_CANCEL_LOCK. */

	pSMB->LockType = LOCKING_ANDX_CANCEL_LOCK|LOCKING_ANDX_LARGE_FILES;
	pSMB->Timeout = 0;
<<<<<<< HEAD
	pSMB->hdr.Mid = GetNextMid(ses->server);
=======
	pSMB->hdr.Mid = get_next_mid(ses->server);
>>>>>>> refs/remotes/origin/master

	return SendReceive(xid, ses, in_buf, out_buf,
			&bytes_returned, 0);
}

int
SendReceiveBlockingLock(const unsigned int xid, struct cifs_tcon *tcon,
	    struct smb_hdr *in_buf, struct smb_hdr *out_buf,
	    int *pbytes_returned)
{
	int rc = 0;
	int rstart = 0;
	struct mid_q_entry *midQ;
	struct cifs_ses *ses;

	if (tcon == NULL || tcon->ses == NULL) {
<<<<<<< HEAD
		cERROR(1, "Null smb session");
=======
		cifs_dbg(VFS, "Null smb session\n");
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}
	ses = tcon->ses;

	if (ses->server == NULL) {
<<<<<<< HEAD
		cERROR(1, "Null tcp session");
=======
		cifs_dbg(VFS, "Null tcp session\n");
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}

	if (ses->server->tcpStatus == CifsExiting)
		return -ENOENT;

	/* Ensure that we do not send more than 50 overlapping requests
	   to the same server. We may make this configurable later or
	   use ses->maxReq */

	if (be32_to_cpu(in_buf->smb_buf_length) > CIFSMaxBufSize +
			MAX_CIFS_HDR_SIZE - 4) {
<<<<<<< HEAD
		cERROR(1, "Illegal length, greater than maximum frame, %d",
			   be32_to_cpu(in_buf->smb_buf_length));
		return -EIO;
	}

	rc = wait_for_free_request(ses->server, CIFS_BLOCKING_OP);
=======
		cifs_dbg(VFS, "Illegal length, greater than maximum frame, %d\n",
			 be32_to_cpu(in_buf->smb_buf_length));
		return -EIO;
	}

	rc = wait_for_free_request(ses->server, CIFS_BLOCKING_OP, 0);
>>>>>>> refs/remotes/origin/master
	if (rc)
		return rc;

	/* make sure that we sign in the same order that we send on this socket
	   and avoid races inside tcp sendmsg code that could cause corruption
	   of smb data */

	mutex_lock(&ses->server->srv_mutex);

	rc = allocate_mid(ses, in_buf, &midQ);
	if (rc) {
		mutex_unlock(&ses->server->srv_mutex);
		return rc;
	}

	rc = cifs_sign_smb(in_buf, ses->server, &midQ->sequence_number);
	if (rc) {
<<<<<<< HEAD
		delete_mid(midQ);
=======
		cifs_delete_mid(midQ);
>>>>>>> refs/remotes/origin/master
		mutex_unlock(&ses->server->srv_mutex);
		return rc;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	midQ->midState = MID_REQUEST_SUBMITTED;
#ifdef CONFIG_CIFS_STATS2
	atomic_inc(&ses->server->inSend);
#endif
	rc = smb_send(ses->server, in_buf, be32_to_cpu(in_buf->smb_buf_length));
#ifdef CONFIG_CIFS_STATS2
	atomic_dec(&ses->server->inSend);
	midQ->when_sent = jiffies;
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
	midQ->mid_state = MID_REQUEST_SUBMITTED;
	cifs_in_send_inc(ses->server);
	rc = smb_send(ses->server, in_buf, be32_to_cpu(in_buf->smb_buf_length));
	cifs_in_send_dec(ses->server);
	cifs_save_when_sent(midQ);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&ses->server->srv_mutex);

	if (rc < 0) {
		delete_mid(midQ);
=======

	if (rc < 0)
		ses->server->sequence_number -= 2;

	mutex_unlock(&ses->server->srv_mutex);

	if (rc < 0) {
		cifs_delete_mid(midQ);
>>>>>>> refs/remotes/origin/master
		return rc;
	}

	/* Wait for a reply - allow signals to interrupt. */
	rc = wait_event_interruptible(ses->server->response_q,
<<<<<<< HEAD
<<<<<<< HEAD
		(!(midQ->midState == MID_REQUEST_SUBMITTED)) ||
=======
		(!(midQ->mid_state == MID_REQUEST_SUBMITTED)) ||
>>>>>>> refs/remotes/origin/cm-10.0
=======
		(!(midQ->mid_state == MID_REQUEST_SUBMITTED)) ||
>>>>>>> refs/remotes/origin/master
		((ses->server->tcpStatus != CifsGood) &&
		 (ses->server->tcpStatus != CifsNew)));

	/* Were we interrupted by a signal ? */
	if ((rc == -ERESTARTSYS) &&
<<<<<<< HEAD
<<<<<<< HEAD
		(midQ->midState == MID_REQUEST_SUBMITTED) &&
=======
		(midQ->mid_state == MID_REQUEST_SUBMITTED) &&
>>>>>>> refs/remotes/origin/cm-10.0
=======
		(midQ->mid_state == MID_REQUEST_SUBMITTED) &&
>>>>>>> refs/remotes/origin/master
		((ses->server->tcpStatus == CifsGood) ||
		 (ses->server->tcpStatus == CifsNew))) {

		if (in_buf->Command == SMB_COM_TRANSACTION2) {
			/* POSIX lock. We send a NT_CANCEL SMB to cause the
			   blocking lock to return. */
<<<<<<< HEAD
			rc = send_nt_cancel(ses->server, in_buf, midQ);
			if (rc) {
				delete_mid(midQ);
=======
			rc = send_cancel(ses->server, in_buf, midQ);
			if (rc) {
				cifs_delete_mid(midQ);
>>>>>>> refs/remotes/origin/master
				return rc;
			}
		} else {
			/* Windows lock. We send a LOCKINGX_CANCEL_LOCK
			   to cause the blocking lock to return. */

			rc = send_lock_cancel(xid, tcon, in_buf, out_buf);

			/* If we get -ENOLCK back the lock may have
			   already been removed. Don't exit in this case. */
			if (rc && rc != -ENOLCK) {
<<<<<<< HEAD
				delete_mid(midQ);
=======
				cifs_delete_mid(midQ);
>>>>>>> refs/remotes/origin/master
				return rc;
			}
		}

		rc = wait_for_response(ses->server, midQ);
		if (rc) {
<<<<<<< HEAD
			send_nt_cancel(ses->server, in_buf, midQ);
			spin_lock(&GlobalMid_Lock);
<<<<<<< HEAD
			if (midQ->midState == MID_REQUEST_SUBMITTED) {
=======
			if (midQ->mid_state == MID_REQUEST_SUBMITTED) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			send_cancel(ses->server, in_buf, midQ);
			spin_lock(&GlobalMid_Lock);
			if (midQ->mid_state == MID_REQUEST_SUBMITTED) {
>>>>>>> refs/remotes/origin/master
				/* no longer considered to be "in-flight" */
				midQ->callback = DeleteMidQEntry;
				spin_unlock(&GlobalMid_Lock);
				return rc;
			}
			spin_unlock(&GlobalMid_Lock);
		}

		/* We got the response - restart system call. */
		rstart = 1;
	}

	rc = cifs_sync_mid_result(midQ, ses->server);
	if (rc != 0)
		return rc;

	/* rcvd frame is ok */
<<<<<<< HEAD
<<<<<<< HEAD
	if (out_buf == NULL || midQ->midState != MID_RESPONSE_RECEIVED) {
=======
	if (out_buf == NULL || midQ->mid_state != MID_RESPONSE_RECEIVED) {
>>>>>>> refs/remotes/origin/cm-10.0
		rc = -EIO;
		cERROR(1, "Bad MID state?");
		goto out;
	}

<<<<<<< HEAD
	*pbytes_returned = be32_to_cpu(midQ->resp_buf->smb_buf_length);
=======
	*pbytes_returned = get_rfc1002_length(midQ->resp_buf);
>>>>>>> refs/remotes/origin/cm-10.0
	memcpy(out_buf, midQ->resp_buf, *pbytes_returned + 4);
	rc = cifs_check_receive(midQ, ses->server, 0);
out:
	delete_mid(midQ);
=======
	if (out_buf == NULL || midQ->mid_state != MID_RESPONSE_RECEIVED) {
		rc = -EIO;
		cifs_dbg(VFS, "Bad MID state?\n");
		goto out;
	}

	*pbytes_returned = get_rfc1002_length(midQ->resp_buf);
	memcpy(out_buf, midQ->resp_buf, *pbytes_returned + 4);
	rc = cifs_check_receive(midQ, ses->server, 0);
out:
	cifs_delete_mid(midQ);
>>>>>>> refs/remotes/origin/master
	if (rstart && rc == -EACCES)
		return -ERESTARTSYS;
	return rc;
}
