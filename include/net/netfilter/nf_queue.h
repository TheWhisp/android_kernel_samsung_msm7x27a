#ifndef _NF_QUEUE_H
#define _NF_QUEUE_H

/* Each queued (to userspace) skbuff has one of these. */
struct nf_queue_entry {
	struct list_head	list;
	struct sk_buff		*skb;
	unsigned int		id;

	struct nf_hook_ops	*elem;
	u_int8_t		pf;
<<<<<<< HEAD
=======
	u16			size; /* sizeof(entry) + saved route keys */
>>>>>>> refs/remotes/origin/master
	unsigned int		hook;
	struct net_device	*indev;
	struct net_device	*outdev;
	int			(*okfn)(struct sk_buff *);
<<<<<<< HEAD
=======

	/* extra space to store route keys */
>>>>>>> refs/remotes/origin/master
};

#define nf_queue_entry_reroute(x) ((void *)x + sizeof(struct nf_queue_entry))

/* Packet queuing */
struct nf_queue_handler {
	int			(*outfn)(struct nf_queue_entry *entry,
					 unsigned int queuenum);
<<<<<<< HEAD
	char			*name;
};

extern int nf_register_queue_handler(u_int8_t pf,
				     const struct nf_queue_handler *qh);
extern int nf_unregister_queue_handler(u_int8_t pf,
				       const struct nf_queue_handler *qh);
extern void nf_unregister_queue_handlers(const struct nf_queue_handler *qh);
extern void nf_reinject(struct nf_queue_entry *entry, unsigned int verdict);
=======
};

void nf_register_queue_handler(const struct nf_queue_handler *qh);
void nf_unregister_queue_handler(void);
void nf_reinject(struct nf_queue_entry *entry, unsigned int verdict);

bool nf_queue_entry_get_refs(struct nf_queue_entry *entry);
void nf_queue_entry_release_refs(struct nf_queue_entry *entry);
>>>>>>> refs/remotes/origin/master

#endif /* _NF_QUEUE_H */
