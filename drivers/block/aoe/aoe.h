<<<<<<< HEAD
/* Copyright (c) 2007 Coraid, Inc.  See COPYING for GPL terms. */
#define VERSION "47q"
<<<<<<< HEAD
=======
/* Copyright (c) 2013 Coraid, Inc.  See COPYING for GPL terms. */
#define VERSION "85"
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define AOE_MAJOR 152
#define DEVICE_NAME "aoe"

/* set AOE_PARTITIONS to 1 to use whole-disks only
 * default is 16, which is 15 partitions plus the whole disk
 */
#ifndef AOE_PARTITIONS
#define AOE_PARTITIONS (16)
#endif

<<<<<<< HEAD
#define SYSMINOR(aoemajor, aoeminor) ((aoemajor) * NPERSHELF + (aoeminor))
#define AOEMAJOR(sysminor) ((sysminor) / NPERSHELF)
#define AOEMINOR(sysminor) ((sysminor) % NPERSHELF)
#define WHITESPACE " \t\v\f\n"
=======
#define WHITESPACE " \t\v\f\n,"
>>>>>>> refs/remotes/origin/master

enum {
	AOECMD_ATA,
	AOECMD_CFG,
	AOECMD_VEND_MIN = 0xf0,

	AOEFL_RSP = (1<<3),
	AOEFL_ERR = (1<<2),

	AOEAFL_EXT = (1<<6),
	AOEAFL_DEV = (1<<4),
	AOEAFL_ASYNC = (1<<1),
	AOEAFL_WRITE = (1<<0),

	AOECCMD_READ = 0,
	AOECCMD_TEST,
	AOECCMD_PTEST,
	AOECCMD_SET,
	AOECCMD_FSET,

	AOE_HVER = 0x10,
};

struct aoe_hdr {
	unsigned char dst[6];
	unsigned char src[6];
	__be16 type;
	unsigned char verfl;
	unsigned char err;
	__be16 major;
	unsigned char minor;
	unsigned char cmd;
	__be32 tag;
};

struct aoe_atahdr {
	unsigned char aflags;
	unsigned char errfeat;
	unsigned char scnt;
	unsigned char cmdstat;
	unsigned char lba0;
	unsigned char lba1;
	unsigned char lba2;
	unsigned char lba3;
	unsigned char lba4;
	unsigned char lba5;
	unsigned char res[2];
};

struct aoe_cfghdr {
	__be16 bufcnt;
	__be16 fwver;
	unsigned char scnt;
	unsigned char aoeccmd;
	unsigned char cslen[2];
};

enum {
	DEVFL_UP = 1,	/* device is installed in system and ready for AoE->ATA commands */
	DEVFL_TKILL = (1<<1),	/* flag for timer to know when to kill self */
	DEVFL_EXT = (1<<2),	/* device accepts lba48 commands */
<<<<<<< HEAD
	DEVFL_CLOSEWAIT = (1<<3), /* device is waiting for all closes to revalidate */
	DEVFL_GDALLOC = (1<<4),	/* need to alloc gendisk */
	DEVFL_KICKME = (1<<5),	/* slow polling network card catch */
	DEVFL_NEWSIZE = (1<<6),	/* need to update dev size in block layer */

	BUFFL_FAIL = 1,
=======
	DEVFL_GDALLOC = (1<<3),	/* need to alloc gendisk */
	DEVFL_GD_NOW = (1<<4),	/* allocating gendisk */
	DEVFL_KICKME = (1<<5),	/* slow polling network card catch */
	DEVFL_NEWSIZE = (1<<6),	/* need to update dev size in block layer */
	DEVFL_FREEING = (1<<7),	/* set when device is being cleaned up */
	DEVFL_FREED = (1<<8),	/* device has been cleaned up */
>>>>>>> refs/remotes/origin/master
};

enum {
	DEFAULTBCNT = 2 * 512,	/* 2 sectors */
<<<<<<< HEAD
	NPERSHELF = 16,		/* number of slots per shelf address */
	FREETAG = -1,
	MIN_BUFS = 16,
	NTARGETS = 8,
	NAOEIFS = 8,
	NSKBPOOLMAX = 128,

	TIMERTICK = HZ / 10,
	MINTIMER = HZ >> 2,
	MAXTIMER = HZ << 1,
	HELPWAIT = 20,
};

struct buf {
	struct list_head bufs;
	ulong stime;	/* for disk stats */
	ulong flags;
	ulong nframesout;
	ulong resid;
	ulong bv_resid;
	ulong bv_off;
	sector_t sector;
	struct bio *bio;
	struct bio_vec *bv;
};

struct frame {
	int tag;
	ulong waited;
	struct buf *buf;
	char *bufaddr;
	ulong bcnt;
	sector_t lba;
	struct sk_buff *skb;
=======
	MIN_BUFS = 16,
	NTARGETS = 4,
	NAOEIFS = 8,
	NSKBPOOLMAX = 256,
	NFACTIVE = 61,

	TIMERTICK = HZ / 10,
	RTTSCALE = 8,
	RTTDSCALE = 3,
	RTTAVG_INIT = USEC_PER_SEC / 4 << RTTSCALE,
	RTTDEV_INIT = RTTAVG_INIT / 4,

	HARD_SCORN_SECS = 10,	/* try another remote port after this */
	MAX_TAINT = 1000,	/* cap on aoetgt taint */
};

struct buf {
	ulong nframesout;
	ulong resid;
	ulong bv_resid;
	sector_t sector;
	struct bio *bio;
	struct bio_vec *bv;
	struct request *rq;
};

enum frame_flags {
	FFL_PROBE = 1,
};

struct frame {
	struct list_head head;
	u32 tag;
	struct timeval sent;	/* high-res time packet was sent */
	u32 sent_jiffs;		/* low-res jiffies-based sent time */
	ulong waited;
	ulong waited_total;
	struct aoetgt *t;		/* parent target I belong to */
	sector_t lba;
	struct sk_buff *skb;		/* command skb freed on module exit */
	struct sk_buff *r_skb;		/* response skb for async processing */
	struct buf *buf;
	struct bio_vec *bv;
	ulong bcnt;
	ulong bv_off;
	char flags;
>>>>>>> refs/remotes/origin/master
};

struct aoeif {
	struct net_device *nd;
<<<<<<< HEAD
	unsigned char lost;
	unsigned char lostjumbo;
	ushort maxbcnt;
=======
	ulong lost;
	int bcnt;
>>>>>>> refs/remotes/origin/master
};

struct aoetgt {
	unsigned char addr[6];
<<<<<<< HEAD
	ushort nframes;
	struct frame *frames;
	struct aoeif ifs[NAOEIFS];
	struct aoeif *ifp;	/* current aoeif in use */
	ushort nout;
	ushort maxout;
	u16 lasttag;		/* last tag sent */
	u16 useme;
	ulong lastwadj;		/* last window adjustment */
	int wpkts, rpkts;
	int dataref;
=======
	ushort nframes;		/* cap on frames to use */
	struct aoedev *d;			/* parent device I belong to */
	struct list_head ffree;			/* list of free frames */
	struct aoeif ifs[NAOEIFS];
	struct aoeif *ifp;	/* current aoeif in use */
	ushort nout;		/* number of AoE commands outstanding */
	ushort maxout;		/* current value for max outstanding */
	ushort next_cwnd;	/* incr maxout after decrementing to zero */
	ushort ssthresh;	/* slow start threshold */
	ulong falloc;		/* number of allocated frames */
	int taint;		/* how much we want to avoid this aoetgt */
	int minbcnt;
	int wpkts, rpkts;
	char nout_probes;
>>>>>>> refs/remotes/origin/master
};

struct aoedev {
	struct aoedev *next;
	ulong sysminor;
	ulong aoemajor;
<<<<<<< HEAD
	u16 aoeminor;
	u16 flags;
	u16 nopen;		/* (bd_openers isn't available without sleeping) */
	u16 rttavg;		/* round trip average of requests/responses */
	u16 mintimer;
	u16 fw_ver;		/* version of blade's firmware */
	struct work_struct work;/* disk create work struct */
	struct gendisk *gd;
	struct request_queue *blkq;
	struct hd_geometry geo; 
	sector_t ssize;
	struct timer_list timer;
	spinlock_t lock;
	struct sk_buff_head sendq;
	struct sk_buff_head skbpool;
	mempool_t *bufpool;	/* for deadlock-free Buf allocation */
	struct list_head bufq;	/* queue of bios to work on */
	struct buf *inprocess;	/* the one we're currently working on */
	struct aoetgt *targets[NTARGETS];
	struct aoetgt **tgt;	/* target in use when working */
	struct aoetgt **htgt;	/* target needing rexmit assistance */
};

=======
	u32 rttavg;		/* scaled AoE round trip time average */
	u32 rttdev;		/* scaled round trip time mean deviation */
	u16 aoeminor;
	u16 flags;
	u16 nopen;		/* (bd_openers isn't available without sleeping) */
	u16 fw_ver;		/* version of blade's firmware */
	u16 lasttag;		/* last tag sent */
	u16 useme;
	ulong ref;
	struct work_struct work;/* disk create work struct */
	struct gendisk *gd;
	struct dentry *debugfs;
	struct request_queue *blkq;
	struct hd_geometry geo;
	sector_t ssize;
	struct timer_list timer;
	spinlock_t lock;
	struct sk_buff_head skbpool;
	mempool_t *bufpool;	/* for deadlock-free Buf allocation */
	struct {		/* pointers to work in progress */
		struct buf *buf;
		struct bio *nxbio;
		struct request *rq;
	} ip;
	ulong maxbcnt;
	struct list_head factive[NFACTIVE];	/* hash of active frames */
	struct list_head rexmitq; /* deferred retransmissions */
	struct aoetgt **targets;
	ulong ntargets;		/* number of allocated aoetgt pointers */
	struct aoetgt **tgt;	/* target in use when working */
	ulong kicked;
	char ident[512];
};

/* kthread tracking */
struct ktstate {
	struct completion rendez;
	struct task_struct *task;
	wait_queue_head_t *waitq;
	int (*fn) (int);
	char name[12];
	spinlock_t *lock;
	int id;
	int active;
};
>>>>>>> refs/remotes/origin/master

int aoeblk_init(void);
void aoeblk_exit(void);
void aoeblk_gdalloc(void *);
<<<<<<< HEAD
=======
void aoedisk_rm_debugfs(struct aoedev *d);
>>>>>>> refs/remotes/origin/master
void aoedisk_rm_sysfs(struct aoedev *d);

int aoechr_init(void);
void aoechr_exit(void);
void aoechr_error(char *);

void aoecmd_work(struct aoedev *d);
void aoecmd_cfg(ushort aoemajor, unsigned char aoeminor);
<<<<<<< HEAD
void aoecmd_ata_rsp(struct sk_buff *);
void aoecmd_cfg_rsp(struct sk_buff *);
void aoecmd_sleepwork(struct work_struct *);
void aoecmd_cleanslate(struct aoedev *);
struct sk_buff *aoecmd_ata_id(struct aoedev *);

int aoedev_init(void);
void aoedev_exit(void);
struct aoedev *aoedev_by_aoeaddr(int maj, int min);
struct aoedev *aoedev_by_sysminor_m(ulong sysminor);
void aoedev_downdev(struct aoedev *d);
int aoedev_flush(const char __user *str, size_t size);
=======
struct sk_buff *aoecmd_ata_rsp(struct sk_buff *);
void aoecmd_cfg_rsp(struct sk_buff *);
void aoecmd_sleepwork(struct work_struct *);
void aoecmd_wreset(struct aoetgt *t);
void aoecmd_cleanslate(struct aoedev *);
void aoecmd_exit(void);
int aoecmd_init(void);
struct sk_buff *aoecmd_ata_id(struct aoedev *);
void aoe_freetframe(struct frame *);
void aoe_flush_iocq(void);
void aoe_flush_iocq_by_index(int);
void aoe_end_request(struct aoedev *, struct request *, int);
int aoe_ktstart(struct ktstate *k);
void aoe_ktstop(struct ktstate *k);

int aoedev_init(void);
void aoedev_exit(void);
struct aoedev *aoedev_by_aoeaddr(ulong maj, int min, int do_alloc);
void aoedev_downdev(struct aoedev *d);
int aoedev_flush(const char __user *str, size_t size);
void aoe_failbuf(struct aoedev *, struct buf *);
void aoedev_put(struct aoedev *);
>>>>>>> refs/remotes/origin/master

int aoenet_init(void);
void aoenet_exit(void);
void aoenet_xmit(struct sk_buff_head *);
int is_aoe_netif(struct net_device *ifp);
int set_aoe_iflist(const char __user *str, size_t size);
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
