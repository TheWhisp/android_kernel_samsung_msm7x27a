/*
 * Packet network namespace
 */
#ifndef __NETNS_PACKET_H__
#define __NETNS_PACKET_H__

#include <linux/rculist.h>
<<<<<<< HEAD
#include <linux/spinlock.h>

struct netns_packet {
	spinlock_t		sklist_lock;
=======
#include <linux/mutex.h>

struct netns_packet {
	struct mutex		sklist_lock;
>>>>>>> refs/remotes/origin/master
	struct hlist_head	sklist;
};

#endif /* __NETNS_PACKET_H__ */
