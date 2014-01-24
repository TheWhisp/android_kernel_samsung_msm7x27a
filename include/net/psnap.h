#ifndef _NET_PSNAP_H
#define _NET_PSNAP_H

<<<<<<< HEAD
extern struct datalink_proto *
=======
struct datalink_proto *
>>>>>>> refs/remotes/origin/master
register_snap_client(const unsigned char *desc,
		     int (*rcvfunc)(struct sk_buff *, struct net_device *,
				    struct packet_type *,
				    struct net_device *orig_dev));
<<<<<<< HEAD
extern void unregister_snap_client(struct datalink_proto *proto);
=======
void unregister_snap_client(struct datalink_proto *proto);
>>>>>>> refs/remotes/origin/master

#endif
