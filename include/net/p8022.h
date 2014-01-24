#ifndef _NET_P8022_H
#define _NET_P8022_H
<<<<<<< HEAD
extern struct datalink_proto *
	register_8022_client(unsigned char type,
			     int (*func)(struct sk_buff *skb,
					 struct net_device *dev,
					 struct packet_type *pt,
					 struct net_device *orig_dev));
extern void unregister_8022_client(struct datalink_proto *proto);

extern struct datalink_proto *make_8023_client(void);
extern void destroy_8023_client(struct datalink_proto *dl);
=======
struct datalink_proto *
register_8022_client(unsigned char type,
		     int (*func)(struct sk_buff *skb,
				 struct net_device *dev,
				 struct packet_type *pt,
				 struct net_device *orig_dev));
void unregister_8022_client(struct datalink_proto *proto);

struct datalink_proto *make_8023_client(void);
void destroy_8023_client(struct datalink_proto *dl);
>>>>>>> refs/remotes/origin/master
#endif
