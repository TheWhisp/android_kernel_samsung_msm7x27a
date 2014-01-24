/*
 * DECnet       An implementation of the DECnet protocol suite for the LINUX
 *              operating system.  DECnet is implemented using the  BSD Socket
 *              interface as the means of communication with the user level.
 *
 *              DECnet Socket Timer Functions
 *
 * Author:      Steve Whitehouse <SteveW@ACM.org>
 *
 *
 * Changes:
 *       Steve Whitehouse      : Made keepalive timer part of the same
 *                               timer idea.
 *       Steve Whitehouse      : Added checks for sk->sock_readers
 *       David S. Miller       : New socket locking
 *       Steve Whitehouse      : Timer grabs socket ref.
 */
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/timer.h>
#include <linux/spinlock.h>
#include <net/sock.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master
#include <net/flow.h>
#include <net/dn.h>

/*
 * Slow timer is for everything else (n * 500mS)
 */

#define SLOW_INTERVAL (HZ/2)

static void dn_slow_timer(unsigned long arg);

void dn_start_slow_timer(struct sock *sk)
{
<<<<<<< HEAD
<<<<<<< HEAD
	sk->sk_timer.expires	= jiffies + SLOW_INTERVAL;
	sk->sk_timer.function	= dn_slow_timer;
	sk->sk_timer.data	= (unsigned long)sk;

	add_timer(&sk->sk_timer);
=======
	setup_timer(&sk->sk_timer, dn_slow_timer, (unsigned long)sk);
	sk_reset_timer(sk, &sk->sk_timer, jiffies + SLOW_INTERVAL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	setup_timer(&sk->sk_timer, dn_slow_timer, (unsigned long)sk);
	sk_reset_timer(sk, &sk->sk_timer, jiffies + SLOW_INTERVAL);
>>>>>>> refs/remotes/origin/master
}

void dn_stop_slow_timer(struct sock *sk)
{
<<<<<<< HEAD
<<<<<<< HEAD
	del_timer(&sk->sk_timer);
=======
	sk_stop_timer(sk, &sk->sk_timer);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sk_stop_timer(sk, &sk->sk_timer);
>>>>>>> refs/remotes/origin/master
}

static void dn_slow_timer(unsigned long arg)
{
	struct sock *sk = (struct sock *)arg;
	struct dn_scp *scp = DN_SK(sk);

<<<<<<< HEAD
<<<<<<< HEAD
	sock_hold(sk);
	bh_lock_sock(sk);

	if (sock_owned_by_user(sk)) {
		sk->sk_timer.expires = jiffies + HZ / 10;
		add_timer(&sk->sk_timer);
=======
=======
>>>>>>> refs/remotes/origin/master
	bh_lock_sock(sk);

	if (sock_owned_by_user(sk)) {
		sk_reset_timer(sk, &sk->sk_timer, jiffies + HZ / 10);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	/*
	 * The persist timer is the standard slow timer used for retransmits
	 * in both connection establishment and disconnection as well as
	 * in the RUN state. The different states are catered for by changing
	 * the function pointer in the socket. Setting the timer to a value
	 * of zero turns it off. We allow the persist_fxn to turn the
	 * timer off in a permant way by returning non-zero, so that
	 * timer based routines may remove sockets. This is why we have a
	 * sock_hold()/sock_put() around the timer to prevent the socket
	 * going away in the middle.
	 */
	if (scp->persist && scp->persist_fxn) {
		if (scp->persist <= SLOW_INTERVAL) {
			scp->persist = 0;

			if (scp->persist_fxn(sk))
				goto out;
		} else {
			scp->persist -= SLOW_INTERVAL;
		}
	}

	/*
	 * Check for keepalive timeout. After the other timer 'cos if
	 * the previous timer caused a retransmit, we don't need to
	 * do this. scp->stamp is the last time that we sent a packet.
	 * The keepalive function sends a link service packet to the
	 * other end. If it remains unacknowledged, the standard
	 * socket timers will eventually shut the socket down. Each
	 * time we do this, scp->stamp will be updated, thus
	 * we won't try and send another until scp->keepalive has passed
	 * since the last successful transmission.
	 */
	if (scp->keepalive && scp->keepalive_fxn && (scp->state == DN_RUN)) {
		if ((jiffies - scp->stamp) >= scp->keepalive)
			scp->keepalive_fxn(sk);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	sk->sk_timer.expires = jiffies + SLOW_INTERVAL;

	add_timer(&sk->sk_timer);
=======
	sk_reset_timer(sk, &sk->sk_timer, jiffies + SLOW_INTERVAL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sk_reset_timer(sk, &sk->sk_timer, jiffies + SLOW_INTERVAL);
>>>>>>> refs/remotes/origin/master
out:
	bh_unlock_sock(sk);
	sock_put(sk);
}
