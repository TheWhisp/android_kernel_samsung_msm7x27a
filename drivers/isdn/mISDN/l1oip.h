/*
 * see notice in l1oip.c
 */

/* debugging */
#define DEBUG_L1OIP_INIT	0x00010000
#define DEBUG_L1OIP_SOCKET	0x00020000
#define DEBUG_L1OIP_MGR		0x00040000
#define DEBUG_L1OIP_MSG		0x00080000

/* enable to disorder received bchannels by sequence 2143658798... */
/*
<<<<<<< HEAD
<<<<<<< HEAD
#define REORDER_DEBUG
=======
  #define REORDER_DEBUG
>>>>>>> refs/remotes/origin/cm-10.0
=======
  #define REORDER_DEBUG
>>>>>>> refs/remotes/origin/master
*/

/* frames */
#define L1OIP_MAX_LEN		2048		/* max packet size form l2 */
#define L1OIP_MAX_PERFRAME	1400		/* max data size in one frame */


/* timers */
#define L1OIP_KEEPALIVE		15
#define L1OIP_TIMEOUT		65


/* socket */
#define L1OIP_DEFAULTPORT	931


/* channel structure */
struct l1oip_chan {
<<<<<<< HEAD
<<<<<<< HEAD
	struct dchannel       	*dch;
	struct bchannel       	*bch;
=======
	struct dchannel		*dch;
	struct bchannel		*bch;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct dchannel		*dch;
	struct bchannel		*bch;
>>>>>>> refs/remotes/origin/master
	u32			tx_counter;	/* counts xmit bytes/packets */
	u32			rx_counter;	/* counts recv bytes/packets */
	u32			codecstate;	/* used by codec to save data */
#ifdef REORDER_DEBUG
	int			disorder_flag;
	struct sk_buff		*disorder_skb;
	u32			disorder_cnt;
#endif
};


/* card structure */
struct l1oip {
	struct list_head        list;

	/* card */
	int			registered;	/* if registered with mISDN */
	char			name[MISDN_MAX_IDLEN];
	int			idx;		/* card index */
	int			pri;		/* 1=pri, 0=bri */
	int			d_idx;		/* current dchannel number */
	int			b_num;		/* number of bchannels */
	u32			id;		/* id of connection */
	int			ondemand;	/* if transmis. is on demand */
	int			bundle;		/* bundle channels in one frm */
	int			codec;		/* codec to use for transmis. */
	int			limit;		/* limit number of bchannels */

	/* timer */
<<<<<<< HEAD
<<<<<<< HEAD
	struct timer_list 	keep_tl;
	struct timer_list 	timeout_tl;
=======
	struct timer_list	keep_tl;
	struct timer_list	timeout_tl;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct timer_list	keep_tl;
	struct timer_list	timeout_tl;
>>>>>>> refs/remotes/origin/master
	int			timeout_on;
	struct work_struct	workq;

	/* socket */
<<<<<<< HEAD
<<<<<<< HEAD
	struct socket 		*socket;	/* if set, socket is created */
	struct completion 	socket_complete;/* completion of sock thread */
	struct task_struct	*socket_thread;
	spinlock_t 		socket_lock;	/* access sock outside thread */
	u32			remoteip;	/* if all set, ip is assigned */
	u16	 		localport;	/* must always be set */
	u16	 		remoteport;	/* must always be set */
=======
=======
>>>>>>> refs/remotes/origin/master
	struct socket		*socket;	/* if set, socket is created */
	struct completion	socket_complete;/* completion of sock thread */
	struct task_struct	*socket_thread;
	spinlock_t		socket_lock;	/* access sock outside thread */
	u32			remoteip;	/* if all set, ip is assigned */
	u16			localport;	/* must always be set */
	u16			remoteport;	/* must always be set */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct sockaddr_in	sin_local;	/* local socket name */
	struct sockaddr_in	sin_remote;	/* remote socket name */
	struct msghdr		sendmsg;	/* ip message to send */
	struct kvec		sendiov;	/* iov for message */

	/* frame */
	struct l1oip_chan	chan[128];	/* channel instances */
};

extern int l1oip_law_to_4bit(u8 *data, int len, u8 *result, u32 *state);
extern int l1oip_4bit_to_law(u8 *data, int len, u8 *result);
extern int l1oip_alaw_to_ulaw(u8 *data, int len, u8 *result);
extern int l1oip_ulaw_to_alaw(u8 *data, int len, u8 *result);
extern void l1oip_4bit_free(void);
extern int l1oip_4bit_alloc(int ulaw);
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
