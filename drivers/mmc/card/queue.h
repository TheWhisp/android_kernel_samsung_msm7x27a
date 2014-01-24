#ifndef MMC_QUEUE_H
#define MMC_QUEUE_H

<<<<<<< HEAD
struct request;
struct task_struct;

<<<<<<< HEAD
=======
=======
#define MMC_REQ_SPECIAL_MASK	(REQ_DISCARD | REQ_FLUSH)

struct request;
struct task_struct;

>>>>>>> refs/remotes/origin/master
struct mmc_blk_request {
	struct mmc_request	mrq;
	struct mmc_command	sbc;
	struct mmc_command	cmd;
	struct mmc_command	stop;
	struct mmc_data		data;
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
enum mmc_blk_status {
	MMC_BLK_SUCCESS = 0,
	MMC_BLK_PARTIAL,
	MMC_BLK_CMD_ERR,
	MMC_BLK_RETRY,
	MMC_BLK_ABORT,
	MMC_BLK_DATA_ERR,
	MMC_BLK_ECC_ERR,
	MMC_BLK_NOMEDIUM,
};

enum mmc_packed_cmd {
<<<<<<< HEAD
=======
enum mmc_packed_type {
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	MMC_PACKED_NONE = 0,
	MMC_PACKED_WRITE,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define mmc_packed_cmd(type)	((type) != MMC_PACKED_NONE)
#define mmc_packed_wr(type)	((type) == MMC_PACKED_WRITE)

struct mmc_packed {
	struct list_head	list;
	u32			cmd_hdr[1024];
	unsigned int		blocks;
	u8			nr_entries;
	u8			retries;
	s16			idx_failure;
};

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct mmc_queue_req {
	struct request		*req;
	struct mmc_blk_request	brq;
	struct scatterlist	*sg;
	char			*bounce_buf;
	struct scatterlist	*bounce_sg;
	unsigned int		bounce_sg_len;
	struct mmc_async_req	mmc_active;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct list_head	packed_list;
	u32			packed_cmd_hdr[128];
	unsigned int		packed_blocks;
	enum mmc_packed_cmd	packed_cmd;
	int		packed_fail_idx;
	u8		packed_num;
};

>>>>>>> refs/remotes/origin/cm-10.0
=======
	enum mmc_packed_type	cmd_type;
	struct mmc_packed	*packed;
};

>>>>>>> refs/remotes/origin/master
struct mmc_queue {
	struct mmc_card		*card;
	struct task_struct	*thread;
	struct semaphore	thread_sem;
	unsigned int		flags;
<<<<<<< HEAD
<<<<<<< HEAD
	struct request		*req;
	int			(*issue_fn)(struct mmc_queue *, struct request *);
	void			*data;
	struct request_queue	*queue;
	struct scatterlist	*sg;
	char			*bounce_buf;
	struct scatterlist	*bounce_sg;
	unsigned int		bounce_sg_len;
=======
=======
#define MMC_QUEUE_SUSPENDED	(1 << 0)
#define MMC_QUEUE_NEW_REQUEST	(1 << 1)

>>>>>>> refs/remotes/origin/master
	int			(*issue_fn)(struct mmc_queue *, struct request *);
	void			*data;
	struct request_queue	*queue;
	struct mmc_queue_req	mqrq[2];
	struct mmc_queue_req	*mqrq_cur;
	struct mmc_queue_req	*mqrq_prev;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	bool			wr_packing_enabled;
	int			num_of_potential_packed_wr_reqs;
	int			num_wr_reqs_to_start_packing;
	int (*err_check_fn) (struct mmc_card *, struct mmc_async_req *);
	void (*packed_test_fn) (struct request_queue *, struct mmc_queue_req *);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

extern int mmc_init_queue(struct mmc_queue *, struct mmc_card *, spinlock_t *,
			  const char *);
extern void mmc_cleanup_queue(struct mmc_queue *);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
extern void mmc_queue_suspend(struct mmc_queue *);
=======
extern int mmc_queue_suspend(struct mmc_queue *);
>>>>>>> refs/remotes/origin/cm-11.0
extern void mmc_queue_resume(struct mmc_queue *);

extern unsigned int mmc_queue_map_sg(struct mmc_queue *);
extern void mmc_queue_bounce_pre(struct mmc_queue *);
extern void mmc_queue_bounce_post(struct mmc_queue *);
=======
extern int mmc_queue_suspend(struct mmc_queue *);
extern void mmc_queue_resume(struct mmc_queue *);

=======
extern void mmc_queue_suspend(struct mmc_queue *);
extern void mmc_queue_resume(struct mmc_queue *);

>>>>>>> refs/remotes/origin/master
extern unsigned int mmc_queue_map_sg(struct mmc_queue *,
				     struct mmc_queue_req *);
extern void mmc_queue_bounce_pre(struct mmc_queue_req *);
extern void mmc_queue_bounce_post(struct mmc_queue_req *);

<<<<<<< HEAD
<<<<<<< HEAD
extern void print_mmc_packing_stats(struct mmc_card *card);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int mmc_packed_init(struct mmc_queue *, struct mmc_card *);
extern void mmc_packed_clean(struct mmc_queue *);
>>>>>>> refs/remotes/origin/master
=======
extern void print_mmc_packing_stats(struct mmc_card *card);
>>>>>>> refs/remotes/origin/cm-11.0

#endif
