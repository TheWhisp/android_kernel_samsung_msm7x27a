#ifndef _NF_CONNTRACK_FTP_H
#define _NF_CONNTRACK_FTP_H
<<<<<<< HEAD
/* FTP tracking. */

/* This enum is exposed to userspace */
enum nf_ct_ftp_type {
	/* PORT command from client */
	NF_CT_FTP_PORT,
	/* PASV response from server */
	NF_CT_FTP_PASV,
	/* EPRT command from client */
	NF_CT_FTP_EPRT,
	/* EPSV response from server */
	NF_CT_FTP_EPSV,
};

#ifdef __KERNEL__

#define FTP_PORT	21

=======

#include <uapi/linux/netfilter/nf_conntrack_ftp.h>


#define FTP_PORT	21

#define NF_CT_FTP_SEQ_PICKUP	(1 << 0)

>>>>>>> refs/remotes/origin/master
#define NUM_SEQ_TO_REMEMBER 2
/* This structure exists only once per master */
struct nf_ct_ftp_master {
	/* Valid seq positions for cmd matching after newline */
	u_int32_t seq_aft_nl[IP_CT_DIR_MAX][NUM_SEQ_TO_REMEMBER];
	/* 0 means seq_match_aft_nl not set */
<<<<<<< HEAD
	int seq_aft_nl_num[IP_CT_DIR_MAX];
=======
	u_int16_t seq_aft_nl_num[IP_CT_DIR_MAX];
	/* pickup sequence tracking, useful for conntrackd */
	u_int16_t flags[IP_CT_DIR_MAX];
>>>>>>> refs/remotes/origin/master
};

struct nf_conntrack_expect;

/* For NAT to hook in when we find a packet which describes what other
 * connection we should expect. */
extern unsigned int (*nf_nat_ftp_hook)(struct sk_buff *skb,
				       enum ip_conntrack_info ctinfo,
				       enum nf_ct_ftp_type type,
<<<<<<< HEAD
				       unsigned int matchoff,
				       unsigned int matchlen,
				       struct nf_conntrack_expect *exp);
#endif /* __KERNEL__ */

=======
				       unsigned int protoff,
				       unsigned int matchoff,
				       unsigned int matchlen,
				       struct nf_conntrack_expect *exp);
>>>>>>> refs/remotes/origin/master
#endif /* _NF_CONNTRACK_FTP_H */
