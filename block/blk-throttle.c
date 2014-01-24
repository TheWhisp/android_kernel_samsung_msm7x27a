/*
 * Interface for controlling IO bandwidth on a request queue
 *
 * Copyright (C) 2010 Vivek Goyal <vgoyal@redhat.com>
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/blktrace_api.h>
#include "blk-cgroup.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "blk.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "blk.h"
>>>>>>> refs/remotes/origin/master

/* Max dispatch from a group in 1 round */
static int throtl_grp_quantum = 8;

/* Total max dispatch from all groups in one round */
static int throtl_quantum = 32;

/* Throttling is performed over 100ms slice and after that slice is renewed */
static unsigned long throtl_slice = HZ/10;	/* 100 ms */

<<<<<<< HEAD
/* A workqueue to queue throttle related work */
static struct workqueue_struct *kthrotld_workqueue;
static void throtl_schedule_delayed_work(struct throtl_data *td,
				unsigned long delay);

struct throtl_rb_root {
	struct rb_root rb;
	struct rb_node *left;
	unsigned int count;
	unsigned long min_disptime;
};

#define THROTL_RB_ROOT	(struct throtl_rb_root) { .rb = RB_ROOT, .left = NULL, \
			.count = 0, .min_disptime = 0}

#define rb_entry_tg(node)	rb_entry((node), struct throtl_grp, rb_node)

struct throtl_grp {
	/* List of throtl groups on the request queue*/
	struct hlist_node tg_node;

	/* active throtl group service_tree member */
	struct rb_node rb_node;

=======
static struct blkcg_policy blkcg_policy_throtl;

/* A workqueue to queue throttle related work */
static struct workqueue_struct *kthrotld_workqueue;

/*
 * To implement hierarchical throttling, throtl_grps form a tree and bios
 * are dispatched upwards level by level until they reach the top and get
 * issued.  When dispatching bios from the children and local group at each
 * level, if the bios are dispatched into a single bio_list, there's a risk
 * of a local or child group which can queue many bios at once filling up
 * the list starving others.
 *
 * To avoid such starvation, dispatched bios are queued separately
 * according to where they came from.  When they are again dispatched to
 * the parent, they're popped in round-robin order so that no single source
 * hogs the dispatch window.
 *
 * throtl_qnode is used to keep the queued bios separated by their sources.
 * Bios are queued to throtl_qnode which in turn is queued to
 * throtl_service_queue and then dispatched in round-robin order.
 *
 * It's also used to track the reference counts on blkg's.  A qnode always
 * belongs to a throtl_grp and gets queued on itself or the parent, so
 * incrementing the reference of the associated throtl_grp when a qnode is
 * queued and decrementing when dequeued is enough to keep the whole blkg
 * tree pinned while bios are in flight.
 */
struct throtl_qnode {
	struct list_head	node;		/* service_queue->queued[] */
	struct bio_list		bios;		/* queued bios */
	struct throtl_grp	*tg;		/* tg this qnode belongs to */
};

struct throtl_service_queue {
	struct throtl_service_queue *parent_sq;	/* the parent service_queue */

	/*
	 * Bios queued directly to this service_queue or dispatched from
	 * children throtl_grp's.
	 */
	struct list_head	queued[2];	/* throtl_qnode [READ/WRITE] */
	unsigned int		nr_queued[2];	/* number of queued bios */

	/*
	 * RB tree of active children throtl_grp's, which are sorted by
	 * their ->disptime.
	 */
	struct rb_root		pending_tree;	/* RB tree of active tgs */
	struct rb_node		*first_pending;	/* first node in the tree */
	unsigned int		nr_pending;	/* # queued in the tree */
	unsigned long		first_pending_disptime;	/* disptime of the first tg */
	struct timer_list	pending_timer;	/* fires on first_pending_disptime */
};

enum tg_state_flags {
	THROTL_TG_PENDING	= 1 << 0,	/* on parent's pending tree */
	THROTL_TG_WAS_EMPTY	= 1 << 1,	/* bio_lists[] became non-empty */
};

#define rb_entry_tg(node)	rb_entry((node), struct throtl_grp, rb_node)

/* Per-cpu group stats */
struct tg_stats_cpu {
	/* total bytes transferred */
	struct blkg_rwstat		service_bytes;
	/* total IOs serviced, post merge */
	struct blkg_rwstat		serviced;
};

struct throtl_grp {
	/* must be the first member */
	struct blkg_policy_data pd;

	/* active throtl group service_queue member */
	struct rb_node rb_node;

	/* throtl_data this group belongs to */
	struct throtl_data *td;

	/* this group's service queue */
	struct throtl_service_queue service_queue;

	/*
	 * qnode_on_self is used when bios are directly queued to this
	 * throtl_grp so that local bios compete fairly with bios
	 * dispatched from children.  qnode_on_parent is used when bios are
	 * dispatched from this throtl_grp into its parent and will compete
	 * with the sibling qnode_on_parents and the parent's
	 * qnode_on_self.
	 */
	struct throtl_qnode qnode_on_self[2];
	struct throtl_qnode qnode_on_parent[2];

>>>>>>> refs/remotes/origin/master
	/*
	 * Dispatch time in jiffies. This is the estimated time when group
	 * will unthrottle and is ready to dispatch more bio. It is used as
	 * key to sort active groups in service tree.
	 */
	unsigned long disptime;

<<<<<<< HEAD
	struct blkio_group blkg;
	atomic_t ref;
	unsigned int flags;

	/* Two lists for READ and WRITE */
	struct bio_list bio_lists[2];

	/* Number of queued bios on READ and WRITE lists */
	unsigned int nr_queued[2];
=======
	unsigned int flags;

	/* are there any throtl rules between this group and td? */
	bool has_rules[2];
>>>>>>> refs/remotes/origin/master

	/* bytes per second rate limits */
	uint64_t bps[2];

	/* IOPS limits */
	unsigned int iops[2];

	/* Number of bytes disptached in current slice */
	uint64_t bytes_disp[2];
	/* Number of bio's dispatched in current slice */
	unsigned int io_disp[2];

	/* When did we start a new slice */
	unsigned long slice_start[2];
	unsigned long slice_end[2];

<<<<<<< HEAD
	/* Some throttle limits got updated for the group */
	int limits_changed;

	struct rcu_head rcu_head;
=======
	/* Per cpu stats pointer */
	struct tg_stats_cpu __percpu *stats_cpu;

	/* List of tgs waiting for per cpu stats memory to be allocated */
	struct list_head stats_alloc_node;
>>>>>>> refs/remotes/origin/master
};

struct throtl_data
{
<<<<<<< HEAD
	/* List of throtl groups */
	struct hlist_head tg_list;

	/* service tree for active throtl groups */
	struct throtl_rb_root tg_service_tree;

	struct throtl_grp *root_tg;
=======
	/* service tree for active throtl groups */
	struct throtl_service_queue service_queue;

>>>>>>> refs/remotes/origin/master
	struct request_queue *queue;

	/* Total Number of queued bios on READ and WRITE lists */
	unsigned int nr_queued[2];

	/*
	 * number of total undestroyed groups
	 */
	unsigned int nr_undestroyed_grps;

	/* Work for dispatching throttled bios */
<<<<<<< HEAD
	struct delayed_work throtl_work;

	int limits_changed;
};

enum tg_state_flags {
	THROTL_TG_FLAG_on_rr = 0,	/* on round-robin busy list */
};

#define THROTL_TG_FNS(name)						\
static inline void throtl_mark_tg_##name(struct throtl_grp *tg)		\
{									\
	(tg)->flags |= (1 << THROTL_TG_FLAG_##name);			\
}									\
static inline void throtl_clear_tg_##name(struct throtl_grp *tg)	\
{									\
	(tg)->flags &= ~(1 << THROTL_TG_FLAG_##name);			\
}									\
static inline int throtl_tg_##name(const struct throtl_grp *tg)		\
{									\
	return ((tg)->flags & (1 << THROTL_TG_FLAG_##name)) != 0;	\
}

THROTL_TG_FNS(on_rr);

#define throtl_log_tg(td, tg, fmt, args...)				\
	blk_add_trace_msg((td)->queue, "throtl %s " fmt,		\
				blkg_path(&(tg)->blkg), ##args);      	\

#define throtl_log(td, fmt, args...)	\
	blk_add_trace_msg((td)->queue, "throtl " fmt, ##args)

static inline struct throtl_grp *tg_of_blkg(struct blkio_group *blkg)
{
	if (blkg)
		return container_of(blkg, struct throtl_grp, blkg);

	return NULL;
}

<<<<<<< HEAD
static inline int total_nr_queued(struct throtl_data *td)
{
	return (td->nr_queued[0] + td->nr_queued[1]);
=======
static inline unsigned int total_nr_queued(struct throtl_data *td)
{
	return td->nr_queued[0] + td->nr_queued[1];
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline struct throtl_grp *throtl_ref_get_tg(struct throtl_grp *tg)
{
	atomic_inc(&tg->ref);
	return tg;
}

static void throtl_free_tg(struct rcu_head *head)
{
	struct throtl_grp *tg;

	tg = container_of(head, struct throtl_grp, rcu_head);
	free_percpu(tg->blkg.stats_cpu);
	kfree(tg);
}

static void throtl_put_tg(struct throtl_grp *tg)
{
	BUG_ON(atomic_read(&tg->ref) <= 0);
	if (!atomic_dec_and_test(&tg->ref))
		return;

	/*
	 * A group is freed in rcu manner. But having an rcu lock does not
	 * mean that one can access all the fields of blkg and assume these
	 * are valid. For example, don't try to follow throtl_data and
	 * request queue links.
	 *
	 * Having a reference to blkg under an rcu allows acess to only
	 * values local to groups like group stats and group rate limits
	 */
	call_rcu(&tg->rcu_head, throtl_free_tg);
}

static void throtl_init_group(struct throtl_grp *tg)
{
	INIT_HLIST_NODE(&tg->tg_node);
	RB_CLEAR_NODE(&tg->rb_node);
	bio_list_init(&tg->bio_lists[0]);
	bio_list_init(&tg->bio_lists[1]);
	tg->limits_changed = false;

	/* Practically unlimited BW */
	tg->bps[0] = tg->bps[1] = -1;
	tg->iops[0] = tg->iops[1] = -1;

	/*
	 * Take the initial reference that will be released on destroy
	 * This can be thought of a joint reference by cgroup and
	 * request queue which will be dropped by either request queue
	 * exit or cgroup deletion path depending on who is exiting first.
	 */
	atomic_set(&tg->ref, 1);
}

/* Should be called with rcu read lock held (needed for blkcg) */
static void
throtl_add_group_to_td_list(struct throtl_data *td, struct throtl_grp *tg)
{
	hlist_add_head(&tg->tg_node, &td->tg_list);
	td->nr_undestroyed_grps++;
}

static void
__throtl_tg_fill_dev_details(struct throtl_data *td, struct throtl_grp *tg)
{
	struct backing_dev_info *bdi = &td->queue->backing_dev_info;
	unsigned int major, minor;

	if (!tg || tg->blkg.dev)
		return;

	/*
	 * Fill in device details for a group which might not have been
	 * filled at group creation time as queue was being instantiated
	 * and driver had not attached a device yet
	 */
	if (bdi->dev && dev_name(bdi->dev)) {
		sscanf(dev_name(bdi->dev), "%u:%u", &major, &minor);
		tg->blkg.dev = MKDEV(major, minor);
	}
}

/*
 * Should be called with without queue lock held. Here queue lock will be
 * taken rarely. It will be taken only once during life time of a group
 * if need be
 */
static void
throtl_tg_fill_dev_details(struct throtl_data *td, struct throtl_grp *tg)
{
	if (!tg || tg->blkg.dev)
		return;

	spin_lock_irq(td->queue->queue_lock);
	__throtl_tg_fill_dev_details(td, tg);
	spin_unlock_irq(td->queue->queue_lock);
}

static void throtl_init_add_tg_lists(struct throtl_data *td,
			struct throtl_grp *tg, struct blkio_cgroup *blkcg)
{
	__throtl_tg_fill_dev_details(td, tg);

	/* Add group onto cgroup list */
	blkiocg_add_blkio_group(blkcg, &tg->blkg, (void *)td,
				tg->blkg.dev, BLKIO_POLICY_THROTL);

	tg->bps[READ] = blkcg_get_read_bps(blkcg, tg->blkg.dev);
	tg->bps[WRITE] = blkcg_get_write_bps(blkcg, tg->blkg.dev);
	tg->iops[READ] = blkcg_get_read_iops(blkcg, tg->blkg.dev);
	tg->iops[WRITE] = blkcg_get_write_iops(blkcg, tg->blkg.dev);

	throtl_add_group_to_td_list(td, tg);
}

/* Should be called without queue lock and outside of rcu period */
static struct throtl_grp *throtl_alloc_tg(struct throtl_data *td)
{
	struct throtl_grp *tg = NULL;
	int ret;

	tg = kzalloc_node(sizeof(*tg), GFP_ATOMIC, td->queue->node);
	if (!tg)
		return NULL;

	ret = blkio_alloc_blkg_stats(&tg->blkg);

	if (ret) {
		kfree(tg);
		return NULL;
	}

	throtl_init_group(tg);
	return tg;
}

static struct
throtl_grp *throtl_find_tg(struct throtl_data *td, struct blkio_cgroup *blkcg)
{
	struct throtl_grp *tg = NULL;
	void *key = td;

	/*
	 * This is the common case when there are no blkio cgroups.
 	 * Avoid lookup in this case
 	 */
	if (blkcg == &blkio_root_cgroup)
		tg = td->root_tg;
	else
		tg = tg_of_blkg(blkiocg_lookup_group(blkcg, key));

	__throtl_tg_fill_dev_details(td, tg);
	return tg;
}

<<<<<<< HEAD
/*
 * This function returns with queue lock unlocked in case of error, like
 * request queue is no more
 */
=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct throtl_grp * throtl_get_tg(struct throtl_data *td)
{
	struct throtl_grp *tg = NULL, *__tg = NULL;
	struct blkio_cgroup *blkcg;
	struct request_queue *q = td->queue;

<<<<<<< HEAD
=======
	/* no throttling for dead queue */
	if (unlikely(blk_queue_dead(q)))
		return NULL;

>>>>>>> refs/remotes/origin/cm-10.0
	rcu_read_lock();
	blkcg = task_blkio_cgroup(current);
	tg = throtl_find_tg(td, blkcg);
	if (tg) {
		rcu_read_unlock();
		return tg;
	}

	/*
	 * Need to allocate a group. Allocation of group also needs allocation
	 * of per cpu stats which in-turn takes a mutex() and can block. Hence
<<<<<<< HEAD
	 * we need to drop rcu lock and queue_lock before we call alloc
	 *
	 * Take the request queue reference to make sure queue does not
	 * go away once we return from allocation.
	 */
	blk_get_queue(q);
=======
	 * we need to drop rcu lock and queue_lock before we call alloc.
	 */
>>>>>>> refs/remotes/origin/cm-10.0
	rcu_read_unlock();
	spin_unlock_irq(q->queue_lock);

	tg = throtl_alloc_tg(td);
<<<<<<< HEAD
	/*
	 * We might have slept in group allocation. Make sure queue is not
	 * dead
	 */
	if (unlikely(test_bit(QUEUE_FLAG_DEAD, &q->queue_flags))) {
		blk_put_queue(q);
		if (tg)
			kfree(tg);

		return ERR_PTR(-ENODEV);
	}
	blk_put_queue(q);
=======
>>>>>>> refs/remotes/origin/cm-10.0

	/* Group allocated and queue is still alive. take the lock */
	spin_lock_irq(q->queue_lock);

<<<<<<< HEAD
=======
	/* Make sure @q is still alive */
	if (unlikely(blk_queue_dead(q))) {
		kfree(tg);
		return NULL;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Initialize the new group. After sleeping, read the blkcg again.
	 */
	rcu_read_lock();
	blkcg = task_blkio_cgroup(current);

	/*
	 * If some other thread already allocated the group while we were
	 * not holding queue lock, free up the group
	 */
	__tg = throtl_find_tg(td, blkcg);

	if (__tg) {
		kfree(tg);
		rcu_read_unlock();
		return __tg;
	}

	/* Group allocation failed. Account the IO to root group */
	if (!tg) {
		tg = td->root_tg;
		return tg;
	}

	throtl_init_add_tg_lists(td, tg, blkcg);
	rcu_read_unlock();
	return tg;
}

static struct throtl_grp *throtl_rb_first(struct throtl_rb_root *root)
{
	/* Service tree is empty */
	if (!root->count)
		return NULL;

	if (!root->left)
		root->left = rb_first(&root->rb);

	if (root->left)
		return rb_entry_tg(root->left);
=======
	struct work_struct dispatch_work;
};

/* list and work item to allocate percpu group stats */
static DEFINE_SPINLOCK(tg_stats_alloc_lock);
static LIST_HEAD(tg_stats_alloc_list);

static void tg_stats_alloc_fn(struct work_struct *);
static DECLARE_DELAYED_WORK(tg_stats_alloc_work, tg_stats_alloc_fn);

static void throtl_pending_timer_fn(unsigned long arg);

static inline struct throtl_grp *pd_to_tg(struct blkg_policy_data *pd)
{
	return pd ? container_of(pd, struct throtl_grp, pd) : NULL;
}

static inline struct throtl_grp *blkg_to_tg(struct blkcg_gq *blkg)
{
	return pd_to_tg(blkg_to_pd(blkg, &blkcg_policy_throtl));
}

static inline struct blkcg_gq *tg_to_blkg(struct throtl_grp *tg)
{
	return pd_to_blkg(&tg->pd);
}

static inline struct throtl_grp *td_root_tg(struct throtl_data *td)
{
	return blkg_to_tg(td->queue->root_blkg);
}

/**
 * sq_to_tg - return the throl_grp the specified service queue belongs to
 * @sq: the throtl_service_queue of interest
 *
 * Return the throtl_grp @sq belongs to.  If @sq is the top-level one
 * embedded in throtl_data, %NULL is returned.
 */
static struct throtl_grp *sq_to_tg(struct throtl_service_queue *sq)
{
	if (sq && sq->parent_sq)
		return container_of(sq, struct throtl_grp, service_queue);
	else
		return NULL;
}

/**
 * sq_to_td - return throtl_data the specified service queue belongs to
 * @sq: the throtl_service_queue of interest
 *
 * A service_queue can be embeded in either a throtl_grp or throtl_data.
 * Determine the associated throtl_data accordingly and return it.
 */
static struct throtl_data *sq_to_td(struct throtl_service_queue *sq)
{
	struct throtl_grp *tg = sq_to_tg(sq);

	if (tg)
		return tg->td;
	else
		return container_of(sq, struct throtl_data, service_queue);
}

/**
 * throtl_log - log debug message via blktrace
 * @sq: the service_queue being reported
 * @fmt: printf format string
 * @args: printf args
 *
 * The messages are prefixed with "throtl BLKG_NAME" if @sq belongs to a
 * throtl_grp; otherwise, just "throtl".
 *
 * TODO: this should be made a function and name formatting should happen
 * after testing whether blktrace is enabled.
 */
#define throtl_log(sq, fmt, args...)	do {				\
	struct throtl_grp *__tg = sq_to_tg((sq));			\
	struct throtl_data *__td = sq_to_td((sq));			\
									\
	(void)__td;							\
	if ((__tg)) {							\
		char __pbuf[128];					\
									\
		blkg_path(tg_to_blkg(__tg), __pbuf, sizeof(__pbuf));	\
		blk_add_trace_msg(__td->queue, "throtl %s " fmt, __pbuf, ##args); \
	} else {							\
		blk_add_trace_msg(__td->queue, "throtl " fmt, ##args);	\
	}								\
} while (0)

static void tg_stats_init(struct tg_stats_cpu *tg_stats)
{
	blkg_rwstat_init(&tg_stats->service_bytes);
	blkg_rwstat_init(&tg_stats->serviced);
}

/*
 * Worker for allocating per cpu stat for tgs. This is scheduled on the
 * system_wq once there are some groups on the alloc_list waiting for
 * allocation.
 */
static void tg_stats_alloc_fn(struct work_struct *work)
{
	static struct tg_stats_cpu *stats_cpu;	/* this fn is non-reentrant */
	struct delayed_work *dwork = to_delayed_work(work);
	bool empty = false;

alloc_stats:
	if (!stats_cpu) {
		int cpu;

		stats_cpu = alloc_percpu(struct tg_stats_cpu);
		if (!stats_cpu) {
			/* allocation failed, try again after some time */
			schedule_delayed_work(dwork, msecs_to_jiffies(10));
			return;
		}
		for_each_possible_cpu(cpu)
			tg_stats_init(per_cpu_ptr(stats_cpu, cpu));
	}

	spin_lock_irq(&tg_stats_alloc_lock);

	if (!list_empty(&tg_stats_alloc_list)) {
		struct throtl_grp *tg = list_first_entry(&tg_stats_alloc_list,
							 struct throtl_grp,
							 stats_alloc_node);
		swap(tg->stats_cpu, stats_cpu);
		list_del_init(&tg->stats_alloc_node);
	}

	empty = list_empty(&tg_stats_alloc_list);
	spin_unlock_irq(&tg_stats_alloc_lock);
	if (!empty)
		goto alloc_stats;
}

static void throtl_qnode_init(struct throtl_qnode *qn, struct throtl_grp *tg)
{
	INIT_LIST_HEAD(&qn->node);
	bio_list_init(&qn->bios);
	qn->tg = tg;
}

/**
 * throtl_qnode_add_bio - add a bio to a throtl_qnode and activate it
 * @bio: bio being added
 * @qn: qnode to add bio to
 * @queued: the service_queue->queued[] list @qn belongs to
 *
 * Add @bio to @qn and put @qn on @queued if it's not already on.
 * @qn->tg's reference count is bumped when @qn is activated.  See the
 * comment on top of throtl_qnode definition for details.
 */
static void throtl_qnode_add_bio(struct bio *bio, struct throtl_qnode *qn,
				 struct list_head *queued)
{
	bio_list_add(&qn->bios, bio);
	if (list_empty(&qn->node)) {
		list_add_tail(&qn->node, queued);
		blkg_get(tg_to_blkg(qn->tg));
	}
}

/**
 * throtl_peek_queued - peek the first bio on a qnode list
 * @queued: the qnode list to peek
 */
static struct bio *throtl_peek_queued(struct list_head *queued)
{
	struct throtl_qnode *qn = list_first_entry(queued, struct throtl_qnode, node);
	struct bio *bio;

	if (list_empty(queued))
		return NULL;

	bio = bio_list_peek(&qn->bios);
	WARN_ON_ONCE(!bio);
	return bio;
}

/**
 * throtl_pop_queued - pop the first bio form a qnode list
 * @queued: the qnode list to pop a bio from
 * @tg_to_put: optional out argument for throtl_grp to put
 *
 * Pop the first bio from the qnode list @queued.  After popping, the first
 * qnode is removed from @queued if empty or moved to the end of @queued so
 * that the popping order is round-robin.
 *
 * When the first qnode is removed, its associated throtl_grp should be put
 * too.  If @tg_to_put is NULL, this function automatically puts it;
 * otherwise, *@tg_to_put is set to the throtl_grp to put and the caller is
 * responsible for putting it.
 */
static struct bio *throtl_pop_queued(struct list_head *queued,
				     struct throtl_grp **tg_to_put)
{
	struct throtl_qnode *qn = list_first_entry(queued, struct throtl_qnode, node);
	struct bio *bio;

	if (list_empty(queued))
		return NULL;

	bio = bio_list_pop(&qn->bios);
	WARN_ON_ONCE(!bio);

	if (bio_list_empty(&qn->bios)) {
		list_del_init(&qn->node);
		if (tg_to_put)
			*tg_to_put = qn->tg;
		else
			blkg_put(tg_to_blkg(qn->tg));
	} else {
		list_move_tail(&qn->node, queued);
	}

	return bio;
}

/* init a service_queue, assumes the caller zeroed it */
static void throtl_service_queue_init(struct throtl_service_queue *sq,
				      struct throtl_service_queue *parent_sq)
{
	INIT_LIST_HEAD(&sq->queued[0]);
	INIT_LIST_HEAD(&sq->queued[1]);
	sq->pending_tree = RB_ROOT;
	sq->parent_sq = parent_sq;
	setup_timer(&sq->pending_timer, throtl_pending_timer_fn,
		    (unsigned long)sq);
}

static void throtl_service_queue_exit(struct throtl_service_queue *sq)
{
	del_timer_sync(&sq->pending_timer);
}

static void throtl_pd_init(struct blkcg_gq *blkg)
{
	struct throtl_grp *tg = blkg_to_tg(blkg);
	struct throtl_data *td = blkg->q->td;
	struct throtl_service_queue *parent_sq;
	unsigned long flags;
	int rw;

	/*
	 * If sane_hierarchy is enabled, we switch to properly hierarchical
	 * behavior where limits on a given throtl_grp are applied to the
	 * whole subtree rather than just the group itself.  e.g. If 16M
	 * read_bps limit is set on the root group, the whole system can't
	 * exceed 16M for the device.
	 *
	 * If sane_hierarchy is not enabled, the broken flat hierarchy
	 * behavior is retained where all throtl_grps are treated as if
	 * they're all separate root groups right below throtl_data.
	 * Limits of a group don't interact with limits of other groups
	 * regardless of the position of the group in the hierarchy.
	 */
	parent_sq = &td->service_queue;

	if (cgroup_sane_behavior(blkg->blkcg->css.cgroup) && blkg->parent)
		parent_sq = &blkg_to_tg(blkg->parent)->service_queue;

	throtl_service_queue_init(&tg->service_queue, parent_sq);

	for (rw = READ; rw <= WRITE; rw++) {
		throtl_qnode_init(&tg->qnode_on_self[rw], tg);
		throtl_qnode_init(&tg->qnode_on_parent[rw], tg);
	}

	RB_CLEAR_NODE(&tg->rb_node);
	tg->td = td;

	tg->bps[READ] = -1;
	tg->bps[WRITE] = -1;
	tg->iops[READ] = -1;
	tg->iops[WRITE] = -1;

	/*
	 * Ugh... We need to perform per-cpu allocation for tg->stats_cpu
	 * but percpu allocator can't be called from IO path.  Queue tg on
	 * tg_stats_alloc_list and allocate from work item.
	 */
	spin_lock_irqsave(&tg_stats_alloc_lock, flags);
	list_add(&tg->stats_alloc_node, &tg_stats_alloc_list);
	schedule_delayed_work(&tg_stats_alloc_work, 0);
	spin_unlock_irqrestore(&tg_stats_alloc_lock, flags);
}

/*
 * Set has_rules[] if @tg or any of its parents have limits configured.
 * This doesn't require walking up to the top of the hierarchy as the
 * parent's has_rules[] is guaranteed to be correct.
 */
static void tg_update_has_rules(struct throtl_grp *tg)
{
	struct throtl_grp *parent_tg = sq_to_tg(tg->service_queue.parent_sq);
	int rw;

	for (rw = READ; rw <= WRITE; rw++)
		tg->has_rules[rw] = (parent_tg && parent_tg->has_rules[rw]) ||
				    (tg->bps[rw] != -1 || tg->iops[rw] != -1);
}

static void throtl_pd_online(struct blkcg_gq *blkg)
{
	/*
	 * We don't want new groups to escape the limits of its ancestors.
	 * Update has_rules[] after a new group is brought online.
	 */
	tg_update_has_rules(blkg_to_tg(blkg));
}

static void throtl_pd_exit(struct blkcg_gq *blkg)
{
	struct throtl_grp *tg = blkg_to_tg(blkg);
	unsigned long flags;

	spin_lock_irqsave(&tg_stats_alloc_lock, flags);
	list_del_init(&tg->stats_alloc_node);
	spin_unlock_irqrestore(&tg_stats_alloc_lock, flags);

	free_percpu(tg->stats_cpu);

	throtl_service_queue_exit(&tg->service_queue);
}

static void throtl_pd_reset_stats(struct blkcg_gq *blkg)
{
	struct throtl_grp *tg = blkg_to_tg(blkg);
	int cpu;

	if (tg->stats_cpu == NULL)
		return;

	for_each_possible_cpu(cpu) {
		struct tg_stats_cpu *sc = per_cpu_ptr(tg->stats_cpu, cpu);

		blkg_rwstat_reset(&sc->service_bytes);
		blkg_rwstat_reset(&sc->serviced);
	}
}

static struct throtl_grp *throtl_lookup_tg(struct throtl_data *td,
					   struct blkcg *blkcg)
{
	/*
	 * This is the common case when there are no blkcgs.  Avoid lookup
	 * in this case
	 */
	if (blkcg == &blkcg_root)
		return td_root_tg(td);

	return blkg_to_tg(blkg_lookup(blkcg, td->queue));
}

static struct throtl_grp *throtl_lookup_create_tg(struct throtl_data *td,
						  struct blkcg *blkcg)
{
	struct request_queue *q = td->queue;
	struct throtl_grp *tg = NULL;

	/*
	 * This is the common case when there are no blkcgs.  Avoid lookup
	 * in this case
	 */
	if (blkcg == &blkcg_root) {
		tg = td_root_tg(td);
	} else {
		struct blkcg_gq *blkg;

		blkg = blkg_lookup_create(blkcg, q);

		/* if %NULL and @q is alive, fall back to root_tg */
		if (!IS_ERR(blkg))
			tg = blkg_to_tg(blkg);
		else if (!blk_queue_dying(q))
			tg = td_root_tg(td);
	}

	return tg;
}

static struct throtl_grp *
throtl_rb_first(struct throtl_service_queue *parent_sq)
{
	/* Service tree is empty */
	if (!parent_sq->nr_pending)
		return NULL;

	if (!parent_sq->first_pending)
		parent_sq->first_pending = rb_first(&parent_sq->pending_tree);

	if (parent_sq->first_pending)
		return rb_entry_tg(parent_sq->first_pending);
>>>>>>> refs/remotes/origin/master

	return NULL;
}

static void rb_erase_init(struct rb_node *n, struct rb_root *root)
{
	rb_erase(n, root);
	RB_CLEAR_NODE(n);
}

<<<<<<< HEAD
static void throtl_rb_erase(struct rb_node *n, struct throtl_rb_root *root)
{
	if (root->left == n)
		root->left = NULL;
	rb_erase_init(n, &root->rb);
	--root->count;
}

static void update_min_dispatch_time(struct throtl_rb_root *st)
{
	struct throtl_grp *tg;

	tg = throtl_rb_first(st);
	if (!tg)
		return;

	st->min_disptime = tg->disptime;
}

static void
tg_service_tree_add(struct throtl_rb_root *st, struct throtl_grp *tg)
{
	struct rb_node **node = &st->rb.rb_node;
=======
static void throtl_rb_erase(struct rb_node *n,
			    struct throtl_service_queue *parent_sq)
{
	if (parent_sq->first_pending == n)
		parent_sq->first_pending = NULL;
	rb_erase_init(n, &parent_sq->pending_tree);
	--parent_sq->nr_pending;
}

static void update_min_dispatch_time(struct throtl_service_queue *parent_sq)
{
	struct throtl_grp *tg;

	tg = throtl_rb_first(parent_sq);
	if (!tg)
		return;

	parent_sq->first_pending_disptime = tg->disptime;
}

static void tg_service_queue_add(struct throtl_grp *tg)
{
	struct throtl_service_queue *parent_sq = tg->service_queue.parent_sq;
	struct rb_node **node = &parent_sq->pending_tree.rb_node;
>>>>>>> refs/remotes/origin/master
	struct rb_node *parent = NULL;
	struct throtl_grp *__tg;
	unsigned long key = tg->disptime;
	int left = 1;

	while (*node != NULL) {
		parent = *node;
		__tg = rb_entry_tg(parent);

		if (time_before(key, __tg->disptime))
			node = &parent->rb_left;
		else {
			node = &parent->rb_right;
			left = 0;
		}
	}

	if (left)
<<<<<<< HEAD
		st->left = &tg->rb_node;

	rb_link_node(&tg->rb_node, parent, node);
	rb_insert_color(&tg->rb_node, &st->rb);
}

static void __throtl_enqueue_tg(struct throtl_data *td, struct throtl_grp *tg)
{
	struct throtl_rb_root *st = &td->tg_service_tree;

	tg_service_tree_add(st, tg);
	throtl_mark_tg_on_rr(tg);
	st->count++;
}

static void throtl_enqueue_tg(struct throtl_data *td, struct throtl_grp *tg)
{
	if (!throtl_tg_on_rr(tg))
		__throtl_enqueue_tg(td, tg);
}

static void __throtl_dequeue_tg(struct throtl_data *td, struct throtl_grp *tg)
{
	throtl_rb_erase(&tg->rb_node, &td->tg_service_tree);
	throtl_clear_tg_on_rr(tg);
}

static void throtl_dequeue_tg(struct throtl_data *td, struct throtl_grp *tg)
{
	if (throtl_tg_on_rr(tg))
		__throtl_dequeue_tg(td, tg);
}

static void throtl_schedule_next_dispatch(struct throtl_data *td)
{
	struct throtl_rb_root *st = &td->tg_service_tree;

	/*
	 * If there are more bios pending, schedule more work.
	 */
	if (!total_nr_queued(td))
		return;

	BUG_ON(!st->count);

	update_min_dispatch_time(st);

	if (time_before_eq(st->min_disptime, jiffies))
		throtl_schedule_delayed_work(td, 0);
	else
		throtl_schedule_delayed_work(td, (st->min_disptime - jiffies));
}

static inline void
throtl_start_new_slice(struct throtl_data *td, struct throtl_grp *tg, bool rw)
=======
		parent_sq->first_pending = &tg->rb_node;

	rb_link_node(&tg->rb_node, parent, node);
	rb_insert_color(&tg->rb_node, &parent_sq->pending_tree);
}

static void __throtl_enqueue_tg(struct throtl_grp *tg)
{
	tg_service_queue_add(tg);
	tg->flags |= THROTL_TG_PENDING;
	tg->service_queue.parent_sq->nr_pending++;
}

static void throtl_enqueue_tg(struct throtl_grp *tg)
{
	if (!(tg->flags & THROTL_TG_PENDING))
		__throtl_enqueue_tg(tg);
}

static void __throtl_dequeue_tg(struct throtl_grp *tg)
{
	throtl_rb_erase(&tg->rb_node, tg->service_queue.parent_sq);
	tg->flags &= ~THROTL_TG_PENDING;
}

static void throtl_dequeue_tg(struct throtl_grp *tg)
{
	if (tg->flags & THROTL_TG_PENDING)
		__throtl_dequeue_tg(tg);
}

/* Call with queue lock held */
static void throtl_schedule_pending_timer(struct throtl_service_queue *sq,
					  unsigned long expires)
{
	mod_timer(&sq->pending_timer, expires);
	throtl_log(sq, "schedule timer. delay=%lu jiffies=%lu",
		   expires - jiffies, jiffies);
}

/**
 * throtl_schedule_next_dispatch - schedule the next dispatch cycle
 * @sq: the service_queue to schedule dispatch for
 * @force: force scheduling
 *
 * Arm @sq->pending_timer so that the next dispatch cycle starts on the
 * dispatch time of the first pending child.  Returns %true if either timer
 * is armed or there's no pending child left.  %false if the current
 * dispatch window is still open and the caller should continue
 * dispatching.
 *
 * If @force is %true, the dispatch timer is always scheduled and this
 * function is guaranteed to return %true.  This is to be used when the
 * caller can't dispatch itself and needs to invoke pending_timer
 * unconditionally.  Note that forced scheduling is likely to induce short
 * delay before dispatch starts even if @sq->first_pending_disptime is not
 * in the future and thus shouldn't be used in hot paths.
 */
static bool throtl_schedule_next_dispatch(struct throtl_service_queue *sq,
					  bool force)
{
	/* any pending children left? */
	if (!sq->nr_pending)
		return true;

	update_min_dispatch_time(sq);

	/* is the next dispatch time in the future? */
	if (force || time_after(sq->first_pending_disptime, jiffies)) {
		throtl_schedule_pending_timer(sq, sq->first_pending_disptime);
		return true;
	}

	/* tell the caller to continue dispatching */
	return false;
}

static inline void throtl_start_new_slice_with_credit(struct throtl_grp *tg,
		bool rw, unsigned long start)
{
	tg->bytes_disp[rw] = 0;
	tg->io_disp[rw] = 0;

	/*
	 * Previous slice has expired. We must have trimmed it after last
	 * bio dispatch. That means since start of last slice, we never used
	 * that bandwidth. Do try to make use of that bandwidth while giving
	 * credit.
	 */
	if (time_after_eq(start, tg->slice_start[rw]))
		tg->slice_start[rw] = start;

	tg->slice_end[rw] = jiffies + throtl_slice;
	throtl_log(&tg->service_queue,
		   "[%c] new slice with credit start=%lu end=%lu jiffies=%lu",
		   rw == READ ? 'R' : 'W', tg->slice_start[rw],
		   tg->slice_end[rw], jiffies);
}

static inline void throtl_start_new_slice(struct throtl_grp *tg, bool rw)
>>>>>>> refs/remotes/origin/master
{
	tg->bytes_disp[rw] = 0;
	tg->io_disp[rw] = 0;
	tg->slice_start[rw] = jiffies;
	tg->slice_end[rw] = jiffies + throtl_slice;
<<<<<<< HEAD
	throtl_log_tg(td, tg, "[%c] new slice start=%lu end=%lu jiffies=%lu",
			rw == READ ? 'R' : 'W', tg->slice_start[rw],
			tg->slice_end[rw], jiffies);
}

static inline void throtl_set_slice_end(struct throtl_data *td,
		struct throtl_grp *tg, bool rw, unsigned long jiffy_end)
=======
	throtl_log(&tg->service_queue,
		   "[%c] new slice start=%lu end=%lu jiffies=%lu",
		   rw == READ ? 'R' : 'W', tg->slice_start[rw],
		   tg->slice_end[rw], jiffies);
}

static inline void throtl_set_slice_end(struct throtl_grp *tg, bool rw,
					unsigned long jiffy_end)
>>>>>>> refs/remotes/origin/master
{
	tg->slice_end[rw] = roundup(jiffy_end, throtl_slice);
}

<<<<<<< HEAD
static inline void throtl_extend_slice(struct throtl_data *td,
		struct throtl_grp *tg, bool rw, unsigned long jiffy_end)
{
	tg->slice_end[rw] = roundup(jiffy_end, throtl_slice);
	throtl_log_tg(td, tg, "[%c] extend slice start=%lu end=%lu jiffies=%lu",
			rw == READ ? 'R' : 'W', tg->slice_start[rw],
			tg->slice_end[rw], jiffies);
}

/* Determine if previously allocated or extended slice is complete or not */
static bool
throtl_slice_used(struct throtl_data *td, struct throtl_grp *tg, bool rw)
=======
static inline void throtl_extend_slice(struct throtl_grp *tg, bool rw,
				       unsigned long jiffy_end)
{
	tg->slice_end[rw] = roundup(jiffy_end, throtl_slice);
	throtl_log(&tg->service_queue,
		   "[%c] extend slice start=%lu end=%lu jiffies=%lu",
		   rw == READ ? 'R' : 'W', tg->slice_start[rw],
		   tg->slice_end[rw], jiffies);
}

/* Determine if previously allocated or extended slice is complete or not */
static bool throtl_slice_used(struct throtl_grp *tg, bool rw)
>>>>>>> refs/remotes/origin/master
{
	if (time_in_range(jiffies, tg->slice_start[rw], tg->slice_end[rw]))
		return 0;

	return 1;
}

/* Trim the used slices and adjust slice start accordingly */
<<<<<<< HEAD
static inline void
throtl_trim_slice(struct throtl_data *td, struct throtl_grp *tg, bool rw)
=======
static inline void throtl_trim_slice(struct throtl_grp *tg, bool rw)
>>>>>>> refs/remotes/origin/master
{
	unsigned long nr_slices, time_elapsed, io_trim;
	u64 bytes_trim, tmp;

	BUG_ON(time_before(tg->slice_end[rw], tg->slice_start[rw]));

	/*
	 * If bps are unlimited (-1), then time slice don't get
	 * renewed. Don't try to trim the slice if slice is used. A new
	 * slice will start when appropriate.
	 */
<<<<<<< HEAD
	if (throtl_slice_used(td, tg, rw))
=======
	if (throtl_slice_used(tg, rw))
>>>>>>> refs/remotes/origin/master
		return;

	/*
	 * A bio has been dispatched. Also adjust slice_end. It might happen
	 * that initially cgroup limit was very low resulting in high
	 * slice_end, but later limit was bumped up and bio was dispached
	 * sooner, then we need to reduce slice_end. A high bogus slice_end
	 * is bad because it does not allow new slice to start.
	 */

<<<<<<< HEAD
	throtl_set_slice_end(td, tg, rw, jiffies + throtl_slice);
=======
	throtl_set_slice_end(tg, rw, jiffies + throtl_slice);
>>>>>>> refs/remotes/origin/master

	time_elapsed = jiffies - tg->slice_start[rw];

	nr_slices = time_elapsed / throtl_slice;

	if (!nr_slices)
		return;
	tmp = tg->bps[rw] * throtl_slice * nr_slices;
	do_div(tmp, HZ);
	bytes_trim = tmp;

	io_trim = (tg->iops[rw] * throtl_slice * nr_slices)/HZ;

	if (!bytes_trim && !io_trim)
		return;

	if (tg->bytes_disp[rw] >= bytes_trim)
		tg->bytes_disp[rw] -= bytes_trim;
	else
		tg->bytes_disp[rw] = 0;

	if (tg->io_disp[rw] >= io_trim)
		tg->io_disp[rw] -= io_trim;
	else
		tg->io_disp[rw] = 0;

	tg->slice_start[rw] += nr_slices * throtl_slice;

<<<<<<< HEAD
	throtl_log_tg(td, tg, "[%c] trim slice nr=%lu bytes=%llu io=%lu"
			" start=%lu end=%lu jiffies=%lu",
			rw == READ ? 'R' : 'W', nr_slices, bytes_trim, io_trim,
			tg->slice_start[rw], tg->slice_end[rw], jiffies);
}

static bool tg_with_in_iops_limit(struct throtl_data *td, struct throtl_grp *tg,
		struct bio *bio, unsigned long *wait)
=======
	throtl_log(&tg->service_queue,
		   "[%c] trim slice nr=%lu bytes=%llu io=%lu start=%lu end=%lu jiffies=%lu",
		   rw == READ ? 'R' : 'W', nr_slices, bytes_trim, io_trim,
		   tg->slice_start[rw], tg->slice_end[rw], jiffies);
}

static bool tg_with_in_iops_limit(struct throtl_grp *tg, struct bio *bio,
				  unsigned long *wait)
>>>>>>> refs/remotes/origin/master
{
	bool rw = bio_data_dir(bio);
	unsigned int io_allowed;
	unsigned long jiffy_elapsed, jiffy_wait, jiffy_elapsed_rnd;
	u64 tmp;

	jiffy_elapsed = jiffy_elapsed_rnd = jiffies - tg->slice_start[rw];

	/* Slice has just started. Consider one slice interval */
	if (!jiffy_elapsed)
		jiffy_elapsed_rnd = throtl_slice;

	jiffy_elapsed_rnd = roundup(jiffy_elapsed_rnd, throtl_slice);

	/*
	 * jiffy_elapsed_rnd should not be a big value as minimum iops can be
	 * 1 then at max jiffy elapsed should be equivalent of 1 second as we
	 * will allow dispatch after 1 second and after that slice should
	 * have been trimmed.
	 */

	tmp = (u64)tg->iops[rw] * jiffy_elapsed_rnd;
	do_div(tmp, HZ);

	if (tmp > UINT_MAX)
		io_allowed = UINT_MAX;
	else
		io_allowed = tmp;

	if (tg->io_disp[rw] + 1 <= io_allowed) {
		if (wait)
			*wait = 0;
		return 1;
	}

	/* Calc approx time to dispatch */
	jiffy_wait = ((tg->io_disp[rw] + 1) * HZ)/tg->iops[rw] + 1;

	if (jiffy_wait > jiffy_elapsed)
		jiffy_wait = jiffy_wait - jiffy_elapsed;
	else
		jiffy_wait = 1;

	if (wait)
		*wait = jiffy_wait;
	return 0;
}

<<<<<<< HEAD
static bool tg_with_in_bps_limit(struct throtl_data *td, struct throtl_grp *tg,
		struct bio *bio, unsigned long *wait)
=======
static bool tg_with_in_bps_limit(struct throtl_grp *tg, struct bio *bio,
				 unsigned long *wait)
>>>>>>> refs/remotes/origin/master
{
	bool rw = bio_data_dir(bio);
	u64 bytes_allowed, extra_bytes, tmp;
	unsigned long jiffy_elapsed, jiffy_wait, jiffy_elapsed_rnd;

	jiffy_elapsed = jiffy_elapsed_rnd = jiffies - tg->slice_start[rw];

	/* Slice has just started. Consider one slice interval */
	if (!jiffy_elapsed)
		jiffy_elapsed_rnd = throtl_slice;

	jiffy_elapsed_rnd = roundup(jiffy_elapsed_rnd, throtl_slice);

	tmp = tg->bps[rw] * jiffy_elapsed_rnd;
	do_div(tmp, HZ);
	bytes_allowed = tmp;

	if (tg->bytes_disp[rw] + bio->bi_size <= bytes_allowed) {
		if (wait)
			*wait = 0;
		return 1;
	}

	/* Calc approx time to dispatch */
	extra_bytes = tg->bytes_disp[rw] + bio->bi_size - bytes_allowed;
	jiffy_wait = div64_u64(extra_bytes * HZ, tg->bps[rw]);

	if (!jiffy_wait)
		jiffy_wait = 1;

	/*
	 * This wait time is without taking into consideration the rounding
	 * up we did. Add that time also.
	 */
	jiffy_wait = jiffy_wait + (jiffy_elapsed_rnd - jiffy_elapsed);
	if (wait)
		*wait = jiffy_wait;
	return 0;
}

<<<<<<< HEAD
static bool tg_no_rule_group(struct throtl_grp *tg, bool rw) {
	if (tg->bps[rw] == -1 && tg->iops[rw] == -1)
		return 1;
	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * Returns whether one can dispatch a bio or not. Also returns approx number
 * of jiffies to wait before this bio is with-in IO rate and can be dispatched
 */
<<<<<<< HEAD
static bool tg_may_dispatch(struct throtl_data *td, struct throtl_grp *tg,
				struct bio *bio, unsigned long *wait)
=======
static bool tg_may_dispatch(struct throtl_grp *tg, struct bio *bio,
			    unsigned long *wait)
>>>>>>> refs/remotes/origin/master
{
	bool rw = bio_data_dir(bio);
	unsigned long bps_wait = 0, iops_wait = 0, max_wait = 0;

	/*
 	 * Currently whole state machine of group depends on first bio
	 * queued in the group bio list. So one should not be calling
	 * this function with a different bio if there are other bios
	 * queued.
	 */
<<<<<<< HEAD
	BUG_ON(tg->nr_queued[rw] && bio != bio_list_peek(&tg->bio_lists[rw]));
=======
	BUG_ON(tg->service_queue.nr_queued[rw] &&
	       bio != throtl_peek_queued(&tg->service_queue.queued[rw]));
>>>>>>> refs/remotes/origin/master

	/* If tg->bps = -1, then BW is unlimited */
	if (tg->bps[rw] == -1 && tg->iops[rw] == -1) {
		if (wait)
			*wait = 0;
		return 1;
	}

	/*
	 * If previous slice expired, start a new one otherwise renew/extend
	 * existing slice to make sure it is at least throtl_slice interval
	 * long since now.
	 */
<<<<<<< HEAD
	if (throtl_slice_used(td, tg, rw))
		throtl_start_new_slice(td, tg, rw);
	else {
		if (time_before(tg->slice_end[rw], jiffies + throtl_slice))
			throtl_extend_slice(td, tg, rw, jiffies + throtl_slice);
	}

	if (tg_with_in_bps_limit(td, tg, bio, &bps_wait)
	    && tg_with_in_iops_limit(td, tg, bio, &iops_wait)) {
=======
	if (throtl_slice_used(tg, rw))
		throtl_start_new_slice(tg, rw);
	else {
		if (time_before(tg->slice_end[rw], jiffies + throtl_slice))
			throtl_extend_slice(tg, rw, jiffies + throtl_slice);
	}

	if (tg_with_in_bps_limit(tg, bio, &bps_wait) &&
	    tg_with_in_iops_limit(tg, bio, &iops_wait)) {
>>>>>>> refs/remotes/origin/master
		if (wait)
			*wait = 0;
		return 1;
	}

	max_wait = max(bps_wait, iops_wait);

	if (wait)
		*wait = max_wait;

	if (time_before(tg->slice_end[rw], jiffies + max_wait))
<<<<<<< HEAD
		throtl_extend_slice(td, tg, rw, jiffies + max_wait);
=======
		throtl_extend_slice(tg, rw, jiffies + max_wait);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static void throtl_charge_bio(struct throtl_grp *tg, struct bio *bio)
{
	bool rw = bio_data_dir(bio);
<<<<<<< HEAD
	bool sync = bio->bi_rw & REQ_SYNC;
=======
	bool sync = rw_is_sync(bio->bi_rw);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void throtl_update_dispatch_stats(struct blkcg_gq *blkg, u64 bytes,
					 int rw)
{
	struct throtl_grp *tg = blkg_to_tg(blkg);
	struct tg_stats_cpu *stats_cpu;
	unsigned long flags;

	/* If per cpu stats are not allocated yet, don't do any accounting. */
	if (tg->stats_cpu == NULL)
		return;

	/*
	 * Disabling interrupts to provide mutual exclusion between two
	 * writes on same cpu. It probably is not needed for 64bit. Not
	 * optimizing that case yet.
	 */
	local_irq_save(flags);

	stats_cpu = this_cpu_ptr(tg->stats_cpu);

	blkg_rwstat_add(&stats_cpu->serviced, rw, 1);
	blkg_rwstat_add(&stats_cpu->service_bytes, rw, bytes);

	local_irq_restore(flags);
}

static void throtl_charge_bio(struct throtl_grp *tg, struct bio *bio)
{
	bool rw = bio_data_dir(bio);
>>>>>>> refs/remotes/origin/master

	/* Charge the bio to the group */
	tg->bytes_disp[rw] += bio->bi_size;
	tg->io_disp[rw]++;

<<<<<<< HEAD
	blkiocg_update_dispatch_stats(&tg->blkg, bio->bi_size, rw, sync);
}

static void throtl_add_bio_tg(struct throtl_data *td, struct throtl_grp *tg,
			struct bio *bio)
{
	bool rw = bio_data_dir(bio);

	bio_list_add(&tg->bio_lists[rw], bio);
	/* Take a bio reference on tg */
	throtl_ref_get_tg(tg);
	tg->nr_queued[rw]++;
	td->nr_queued[rw]++;
	throtl_enqueue_tg(td, tg);
}

static void tg_update_disptime(struct throtl_data *td, struct throtl_grp *tg)
{
	unsigned long read_wait = -1, write_wait = -1, min_wait = -1, disptime;
	struct bio *bio;

	if ((bio = bio_list_peek(&tg->bio_lists[READ])))
		tg_may_dispatch(td, tg, bio, &read_wait);

	if ((bio = bio_list_peek(&tg->bio_lists[WRITE])))
		tg_may_dispatch(td, tg, bio, &write_wait);
=======
	/*
	 * REQ_THROTTLED is used to prevent the same bio to be throttled
	 * more than once as a throttled bio will go through blk-throtl the
	 * second time when it eventually gets issued.  Set it when a bio
	 * is being charged to a tg.
	 *
	 * Dispatch stats aren't recursive and each @bio should only be
	 * accounted by the @tg it was originally associated with.  Let's
	 * update the stats when setting REQ_THROTTLED for the first time
	 * which is guaranteed to be for the @bio's original tg.
	 */
	if (!(bio->bi_rw & REQ_THROTTLED)) {
		bio->bi_rw |= REQ_THROTTLED;
		throtl_update_dispatch_stats(tg_to_blkg(tg), bio->bi_size,
					     bio->bi_rw);
	}
}

/**
 * throtl_add_bio_tg - add a bio to the specified throtl_grp
 * @bio: bio to add
 * @qn: qnode to use
 * @tg: the target throtl_grp
 *
 * Add @bio to @tg's service_queue using @qn.  If @qn is not specified,
 * tg->qnode_on_self[] is used.
 */
static void throtl_add_bio_tg(struct bio *bio, struct throtl_qnode *qn,
			      struct throtl_grp *tg)
{
	struct throtl_service_queue *sq = &tg->service_queue;
	bool rw = bio_data_dir(bio);

	if (!qn)
		qn = &tg->qnode_on_self[rw];

	/*
	 * If @tg doesn't currently have any bios queued in the same
	 * direction, queueing @bio can change when @tg should be
	 * dispatched.  Mark that @tg was empty.  This is automatically
	 * cleaered on the next tg_update_disptime().
	 */
	if (!sq->nr_queued[rw])
		tg->flags |= THROTL_TG_WAS_EMPTY;

	throtl_qnode_add_bio(bio, qn, &sq->queued[rw]);

	sq->nr_queued[rw]++;
	throtl_enqueue_tg(tg);
}

static void tg_update_disptime(struct throtl_grp *tg)
{
	struct throtl_service_queue *sq = &tg->service_queue;
	unsigned long read_wait = -1, write_wait = -1, min_wait = -1, disptime;
	struct bio *bio;

	if ((bio = throtl_peek_queued(&sq->queued[READ])))
		tg_may_dispatch(tg, bio, &read_wait);

	if ((bio = throtl_peek_queued(&sq->queued[WRITE])))
		tg_may_dispatch(tg, bio, &write_wait);
>>>>>>> refs/remotes/origin/master

	min_wait = min(read_wait, write_wait);
	disptime = jiffies + min_wait;

	/* Update dispatch time */
<<<<<<< HEAD
	throtl_dequeue_tg(td, tg);
	tg->disptime = disptime;
	throtl_enqueue_tg(td, tg);
}

static void tg_dispatch_one_bio(struct throtl_data *td, struct throtl_grp *tg,
				bool rw, struct bio_list *bl)
{
	struct bio *bio;

	bio = bio_list_pop(&tg->bio_lists[rw]);
	tg->nr_queued[rw]--;
	/* Drop bio reference on tg */
	throtl_put_tg(tg);

	BUG_ON(td->nr_queued[rw] <= 0);
	td->nr_queued[rw]--;

	throtl_charge_bio(tg, bio);
	bio_list_add(bl, bio);
	bio->bi_rw |= REQ_THROTTLED;

	throtl_trim_slice(td, tg, rw);
}

static int throtl_dispatch_tg(struct throtl_data *td, struct throtl_grp *tg,
				struct bio_list *bl)
{
=======
	throtl_dequeue_tg(tg);
	tg->disptime = disptime;
	throtl_enqueue_tg(tg);

	/* see throtl_add_bio_tg() */
	tg->flags &= ~THROTL_TG_WAS_EMPTY;
}

static void start_parent_slice_with_credit(struct throtl_grp *child_tg,
					struct throtl_grp *parent_tg, bool rw)
{
	if (throtl_slice_used(parent_tg, rw)) {
		throtl_start_new_slice_with_credit(parent_tg, rw,
				child_tg->slice_start[rw]);
	}

}

static void tg_dispatch_one_bio(struct throtl_grp *tg, bool rw)
{
	struct throtl_service_queue *sq = &tg->service_queue;
	struct throtl_service_queue *parent_sq = sq->parent_sq;
	struct throtl_grp *parent_tg = sq_to_tg(parent_sq);
	struct throtl_grp *tg_to_put = NULL;
	struct bio *bio;

	/*
	 * @bio is being transferred from @tg to @parent_sq.  Popping a bio
	 * from @tg may put its reference and @parent_sq might end up
	 * getting released prematurely.  Remember the tg to put and put it
	 * after @bio is transferred to @parent_sq.
	 */
	bio = throtl_pop_queued(&sq->queued[rw], &tg_to_put);
	sq->nr_queued[rw]--;

	throtl_charge_bio(tg, bio);

	/*
	 * If our parent is another tg, we just need to transfer @bio to
	 * the parent using throtl_add_bio_tg().  If our parent is
	 * @td->service_queue, @bio is ready to be issued.  Put it on its
	 * bio_lists[] and decrease total number queued.  The caller is
	 * responsible for issuing these bios.
	 */
	if (parent_tg) {
		throtl_add_bio_tg(bio, &tg->qnode_on_parent[rw], parent_tg);
		start_parent_slice_with_credit(tg, parent_tg, rw);
	} else {
		throtl_qnode_add_bio(bio, &tg->qnode_on_parent[rw],
				     &parent_sq->queued[rw]);
		BUG_ON(tg->td->nr_queued[rw] <= 0);
		tg->td->nr_queued[rw]--;
	}

	throtl_trim_slice(tg, rw);

	if (tg_to_put)
		blkg_put(tg_to_blkg(tg_to_put));
}

static int throtl_dispatch_tg(struct throtl_grp *tg)
{
	struct throtl_service_queue *sq = &tg->service_queue;
>>>>>>> refs/remotes/origin/master
	unsigned int nr_reads = 0, nr_writes = 0;
	unsigned int max_nr_reads = throtl_grp_quantum*3/4;
	unsigned int max_nr_writes = throtl_grp_quantum - max_nr_reads;
	struct bio *bio;

	/* Try to dispatch 75% READS and 25% WRITES */

<<<<<<< HEAD
	while ((bio = bio_list_peek(&tg->bio_lists[READ]))
		&& tg_may_dispatch(td, tg, bio, NULL)) {

		tg_dispatch_one_bio(td, tg, bio_data_dir(bio), bl);
=======
	while ((bio = throtl_peek_queued(&sq->queued[READ])) &&
	       tg_may_dispatch(tg, bio, NULL)) {

		tg_dispatch_one_bio(tg, bio_data_dir(bio));
>>>>>>> refs/remotes/origin/master
		nr_reads++;

		if (nr_reads >= max_nr_reads)
			break;
	}

<<<<<<< HEAD
	while ((bio = bio_list_peek(&tg->bio_lists[WRITE]))
		&& tg_may_dispatch(td, tg, bio, NULL)) {

		tg_dispatch_one_bio(td, tg, bio_data_dir(bio), bl);
=======
	while ((bio = throtl_peek_queued(&sq->queued[WRITE])) &&
	       tg_may_dispatch(tg, bio, NULL)) {

		tg_dispatch_one_bio(tg, bio_data_dir(bio));
>>>>>>> refs/remotes/origin/master
		nr_writes++;

		if (nr_writes >= max_nr_writes)
			break;
	}

	return nr_reads + nr_writes;
}

<<<<<<< HEAD
static int throtl_select_dispatch(struct throtl_data *td, struct bio_list *bl)
{
	unsigned int nr_disp = 0;
	struct throtl_grp *tg;
	struct throtl_rb_root *st = &td->tg_service_tree;

	while (1) {
		tg = throtl_rb_first(st);
=======
static int throtl_select_dispatch(struct throtl_service_queue *parent_sq)
{
	unsigned int nr_disp = 0;

	while (1) {
		struct throtl_grp *tg = throtl_rb_first(parent_sq);
		struct throtl_service_queue *sq = &tg->service_queue;
>>>>>>> refs/remotes/origin/master

		if (!tg)
			break;

		if (time_before(jiffies, tg->disptime))
			break;

<<<<<<< HEAD
		throtl_dequeue_tg(td, tg);

		nr_disp += throtl_dispatch_tg(td, tg, bl);

		if (tg->nr_queued[0] || tg->nr_queued[1]) {
			tg_update_disptime(td, tg);
			throtl_enqueue_tg(td, tg);
		}
=======
		throtl_dequeue_tg(tg);

		nr_disp += throtl_dispatch_tg(tg);

		if (sq->nr_queued[0] || sq->nr_queued[1])
			tg_update_disptime(tg);
>>>>>>> refs/remotes/origin/master

		if (nr_disp >= throtl_quantum)
			break;
	}

	return nr_disp;
}

<<<<<<< HEAD
static void throtl_process_limit_change(struct throtl_data *td)
{
	struct throtl_grp *tg;
	struct hlist_node *pos, *n;

	if (!td->limits_changed)
		return;

	xchg(&td->limits_changed, false);

	throtl_log(td, "limits changed");

	hlist_for_each_entry_safe(tg, pos, n, &td->tg_list, tg_node) {
		if (!tg->limits_changed)
			continue;

		if (!xchg(&tg->limits_changed, false))
			continue;

		throtl_log_tg(td, tg, "limit change rbps=%llu wbps=%llu"
			" riops=%u wiops=%u", tg->bps[READ], tg->bps[WRITE],
			tg->iops[READ], tg->iops[WRITE]);

		/*
		 * Restart the slices for both READ and WRITES. It
		 * might happen that a group's limit are dropped
		 * suddenly and we don't want to account recently
		 * dispatched IO with new low rate
		 */
		throtl_start_new_slice(td, tg, 0);
		throtl_start_new_slice(td, tg, 1);

		if (throtl_tg_on_rr(tg))
			tg_update_disptime(td, tg);
	}
}

/* Dispatch throttled bios. Should be called without queue lock held. */
static int throtl_dispatch(struct request_queue *q)
{
	struct throtl_data *td = q->td;
	unsigned int nr_disp = 0;
	struct bio_list bio_list_on_stack;
	struct bio *bio;
	struct blk_plug plug;

	spin_lock_irq(q->queue_lock);

	throtl_process_limit_change(td);

	if (!total_nr_queued(td))
		goto out;

	bio_list_init(&bio_list_on_stack);

<<<<<<< HEAD
	throtl_log(td, "dispatch nr_queued=%d read=%u write=%u",
=======
	throtl_log(td, "dispatch nr_queued=%u read=%u write=%u",
>>>>>>> refs/remotes/origin/cm-10.0
			total_nr_queued(td), td->nr_queued[READ],
			td->nr_queued[WRITE]);

	nr_disp = throtl_select_dispatch(td, &bio_list_on_stack);

	if (nr_disp)
		throtl_log(td, "bios disp=%u", nr_disp);

	throtl_schedule_next_dispatch(td);
out:
	spin_unlock_irq(q->queue_lock);

	/*
	 * If we dispatched some requests, unplug the queue to make sure
	 * immediate dispatch
	 */
	if (nr_disp) {
=======
/**
 * throtl_pending_timer_fn - timer function for service_queue->pending_timer
 * @arg: the throtl_service_queue being serviced
 *
 * This timer is armed when a child throtl_grp with active bio's become
 * pending and queued on the service_queue's pending_tree and expires when
 * the first child throtl_grp should be dispatched.  This function
 * dispatches bio's from the children throtl_grps to the parent
 * service_queue.
 *
 * If the parent's parent is another throtl_grp, dispatching is propagated
 * by either arming its pending_timer or repeating dispatch directly.  If
 * the top-level service_tree is reached, throtl_data->dispatch_work is
 * kicked so that the ready bio's are issued.
 */
static void throtl_pending_timer_fn(unsigned long arg)
{
	struct throtl_service_queue *sq = (void *)arg;
	struct throtl_grp *tg = sq_to_tg(sq);
	struct throtl_data *td = sq_to_td(sq);
	struct request_queue *q = td->queue;
	struct throtl_service_queue *parent_sq;
	bool dispatched;
	int ret;

	spin_lock_irq(q->queue_lock);
again:
	parent_sq = sq->parent_sq;
	dispatched = false;

	while (true) {
		throtl_log(sq, "dispatch nr_queued=%u read=%u write=%u",
			   sq->nr_queued[READ] + sq->nr_queued[WRITE],
			   sq->nr_queued[READ], sq->nr_queued[WRITE]);

		ret = throtl_select_dispatch(sq);
		if (ret) {
			throtl_log(sq, "bios disp=%u", ret);
			dispatched = true;
		}

		if (throtl_schedule_next_dispatch(sq, false))
			break;

		/* this dispatch windows is still open, relax and repeat */
		spin_unlock_irq(q->queue_lock);
		cpu_relax();
		spin_lock_irq(q->queue_lock);
	}

	if (!dispatched)
		goto out_unlock;

	if (parent_sq) {
		/* @parent_sq is another throl_grp, propagate dispatch */
		if (tg->flags & THROTL_TG_WAS_EMPTY) {
			tg_update_disptime(tg);
			if (!throtl_schedule_next_dispatch(parent_sq, false)) {
				/* window is already open, repeat dispatching */
				sq = parent_sq;
				tg = sq_to_tg(sq);
				goto again;
			}
		}
	} else {
		/* reached the top-level, queue issueing */
		queue_work(kthrotld_workqueue, &td->dispatch_work);
	}
out_unlock:
	spin_unlock_irq(q->queue_lock);
}

/**
 * blk_throtl_dispatch_work_fn - work function for throtl_data->dispatch_work
 * @work: work item being executed
 *
 * This function is queued for execution when bio's reach the bio_lists[]
 * of throtl_data->service_queue.  Those bio's are ready and issued by this
 * function.
 */
void blk_throtl_dispatch_work_fn(struct work_struct *work)
{
	struct throtl_data *td = container_of(work, struct throtl_data,
					      dispatch_work);
	struct throtl_service_queue *td_sq = &td->service_queue;
	struct request_queue *q = td->queue;
	struct bio_list bio_list_on_stack;
	struct bio *bio;
	struct blk_plug plug;
	int rw;

	bio_list_init(&bio_list_on_stack);

	spin_lock_irq(q->queue_lock);
	for (rw = READ; rw <= WRITE; rw++)
		while ((bio = throtl_pop_queued(&td_sq->queued[rw], NULL)))
			bio_list_add(&bio_list_on_stack, bio);
	spin_unlock_irq(q->queue_lock);

	if (!bio_list_empty(&bio_list_on_stack)) {
>>>>>>> refs/remotes/origin/master
		blk_start_plug(&plug);
		while((bio = bio_list_pop(&bio_list_on_stack)))
			generic_make_request(bio);
		blk_finish_plug(&plug);
	}
<<<<<<< HEAD
	return nr_disp;
}

void blk_throtl_work(struct work_struct *work)
{
	struct throtl_data *td = container_of(work, struct throtl_data,
					throtl_work.work);
	struct request_queue *q = td->queue;

	throtl_dispatch(q);
}

/* Call with queue lock held */
static void
throtl_schedule_delayed_work(struct throtl_data *td, unsigned long delay)
{

	struct delayed_work *dwork = &td->throtl_work;

	/* schedule work if limits changed even if no bio is queued */
<<<<<<< HEAD
	if (total_nr_queued(td) > 0 || td->limits_changed) {
=======
	if (total_nr_queued(td) || td->limits_changed) {
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * We might have a work scheduled to be executed in future.
		 * Cancel that and schedule a new one.
		 */
		__cancel_delayed_work(dwork);
		queue_delayed_work(kthrotld_workqueue, dwork, delay);
		throtl_log(td, "schedule work. delay=%lu jiffies=%lu",
				delay, jiffies);
	}
}

static void
throtl_destroy_tg(struct throtl_data *td, struct throtl_grp *tg)
{
	/* Something wrong if we are trying to remove same group twice */
	BUG_ON(hlist_unhashed(&tg->tg_node));

	hlist_del_init(&tg->tg_node);

	/*
	 * Put the reference taken at the time of creation so that when all
	 * queues are gone, group can be destroyed.
	 */
	throtl_put_tg(tg);
	td->nr_undestroyed_grps--;
}

static void throtl_release_tgs(struct throtl_data *td)
{
	struct hlist_node *pos, *n;
	struct throtl_grp *tg;

	hlist_for_each_entry_safe(tg, pos, n, &td->tg_list, tg_node) {
		/*
		 * If cgroup removal path got to blk_group first and removed
		 * it from cgroup list, then it will take care of destroying
		 * cfqg also.
		 */
		if (!blkiocg_del_blkio_group(&tg->blkg))
			throtl_destroy_tg(td, tg);
	}
}

<<<<<<< HEAD
static void throtl_td_free(struct throtl_data *td)
{
	kfree(td);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Blk cgroup controller notification saying that blkio_group object is being
 * delinked as associated cgroup object is going away. That also means that
 * no new IO will come in this group. So get rid of this group as soon as
 * any pending IO in the group is finished.
 *
 * This function is called under rcu_read_lock(). key is the rcu protected
 * pointer. That means "key" is a valid throtl_data pointer as long as we are
 * rcu read lock.
 *
 * "key" was fetched from blkio_group under blkio_cgroup->lock. That means
 * it should not be NULL as even if queue was going away, cgroup deltion
 * path got to it first.
 */
void throtl_unlink_blkio_group(void *key, struct blkio_group *blkg)
{
	unsigned long flags;
	struct throtl_data *td = key;

	spin_lock_irqsave(td->queue->queue_lock, flags);
	throtl_destroy_tg(td, tg_of_blkg(blkg));
	spin_unlock_irqrestore(td->queue->queue_lock, flags);
}

static void throtl_update_blkio_group_common(struct throtl_data *td,
				struct throtl_grp *tg)
{
	xchg(&tg->limits_changed, true);
	xchg(&td->limits_changed, true);
	/* Schedule a work now to process the limit change */
	throtl_schedule_delayed_work(td, 0);
}

/*
 * For all update functions, key should be a valid pointer because these
 * update functions are called under blkcg_lock, that means, blkg is
 * valid and in turn key is valid. queue exit path can not race because
 * of blkcg_lock
 *
 * Can not take queue lock in update functions as queue lock under blkcg_lock
 * is not allowed. Under other paths we take blkcg_lock under queue_lock.
 */
static void throtl_update_blkio_group_read_bps(void *key,
				struct blkio_group *blkg, u64 read_bps)
{
	struct throtl_data *td = key;
	struct throtl_grp *tg = tg_of_blkg(blkg);

	tg->bps[READ] = read_bps;
	throtl_update_blkio_group_common(td, tg);
}

static void throtl_update_blkio_group_write_bps(void *key,
				struct blkio_group *blkg, u64 write_bps)
{
	struct throtl_data *td = key;
	struct throtl_grp *tg = tg_of_blkg(blkg);

	tg->bps[WRITE] = write_bps;
	throtl_update_blkio_group_common(td, tg);
}

static void throtl_update_blkio_group_read_iops(void *key,
			struct blkio_group *blkg, unsigned int read_iops)
{
	struct throtl_data *td = key;
	struct throtl_grp *tg = tg_of_blkg(blkg);

	tg->iops[READ] = read_iops;
	throtl_update_blkio_group_common(td, tg);
}

static void throtl_update_blkio_group_write_iops(void *key,
			struct blkio_group *blkg, unsigned int write_iops)
{
	struct throtl_data *td = key;
	struct throtl_grp *tg = tg_of_blkg(blkg);

	tg->iops[WRITE] = write_iops;
	throtl_update_blkio_group_common(td, tg);
}

static void throtl_shutdown_wq(struct request_queue *q)
{
	struct throtl_data *td = q->td;

	cancel_delayed_work_sync(&td->throtl_work);
}

static struct blkio_policy_type blkio_policy_throtl = {
	.ops = {
		.blkio_unlink_group_fn = throtl_unlink_blkio_group,
		.blkio_update_group_read_bps_fn =
					throtl_update_blkio_group_read_bps,
		.blkio_update_group_write_bps_fn =
					throtl_update_blkio_group_write_bps,
		.blkio_update_group_read_iops_fn =
					throtl_update_blkio_group_read_iops,
		.blkio_update_group_write_iops_fn =
					throtl_update_blkio_group_write_iops,
	},
	.plid = BLKIO_POLICY_THROTL,
};

<<<<<<< HEAD
int blk_throtl_bio(struct request_queue *q, struct bio **biop)
{
	struct throtl_data *td = q->td;
	struct throtl_grp *tg;
	struct bio *bio = *biop;
	bool rw = bio_data_dir(bio), update_disptime = true;
	struct blkio_cgroup *blkcg;

	if (bio->bi_rw & REQ_THROTTLED) {
		bio->bi_rw &= ~REQ_THROTTLED;
		return 0;
=======
bool blk_throtl_bio(struct request_queue *q, struct bio *bio)
{
	struct throtl_data *td = q->td;
	struct throtl_grp *tg;
	bool rw = bio_data_dir(bio), update_disptime = true;
	struct blkio_cgroup *blkcg;
	bool throttled = false;

	if (bio->bi_rw & REQ_THROTTLED) {
		bio->bi_rw &= ~REQ_THROTTLED;
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
}

static u64 tg_prfill_cpu_rwstat(struct seq_file *sf,
				struct blkg_policy_data *pd, int off)
{
	struct throtl_grp *tg = pd_to_tg(pd);
	struct blkg_rwstat rwstat = { }, tmp;
	int i, cpu;

	for_each_possible_cpu(cpu) {
		struct tg_stats_cpu *sc = per_cpu_ptr(tg->stats_cpu, cpu);

		tmp = blkg_rwstat_read((void *)sc + off);
		for (i = 0; i < BLKG_RWSTAT_NR; i++)
			rwstat.cnt[i] += tmp.cnt[i];
	}

	return __blkg_prfill_rwstat(sf, pd, &rwstat);
}

static int tg_print_cpu_rwstat(struct seq_file *sf, void *v)
{
	blkcg_print_blkgs(sf, css_to_blkcg(seq_css(sf)), tg_prfill_cpu_rwstat,
			  &blkcg_policy_throtl, seq_cft(sf)->private, true);
	return 0;
}

static u64 tg_prfill_conf_u64(struct seq_file *sf, struct blkg_policy_data *pd,
			      int off)
{
	struct throtl_grp *tg = pd_to_tg(pd);
	u64 v = *(u64 *)((void *)tg + off);

	if (v == -1)
		return 0;
	return __blkg_prfill_u64(sf, pd, v);
}

static u64 tg_prfill_conf_uint(struct seq_file *sf, struct blkg_policy_data *pd,
			       int off)
{
	struct throtl_grp *tg = pd_to_tg(pd);
	unsigned int v = *(unsigned int *)((void *)tg + off);

	if (v == -1)
		return 0;
	return __blkg_prfill_u64(sf, pd, v);
}

static int tg_print_conf_u64(struct seq_file *sf, void *v)
{
	blkcg_print_blkgs(sf, css_to_blkcg(seq_css(sf)), tg_prfill_conf_u64,
			  &blkcg_policy_throtl, seq_cft(sf)->private, false);
	return 0;
}

static int tg_print_conf_uint(struct seq_file *sf, void *v)
{
	blkcg_print_blkgs(sf, css_to_blkcg(seq_css(sf)), tg_prfill_conf_uint,
			  &blkcg_policy_throtl, seq_cft(sf)->private, false);
	return 0;
}

static int tg_set_conf(struct cgroup_subsys_state *css, struct cftype *cft,
		       const char *buf, bool is_u64)
{
	struct blkcg *blkcg = css_to_blkcg(css);
	struct blkg_conf_ctx ctx;
	struct throtl_grp *tg;
	struct throtl_service_queue *sq;
	struct blkcg_gq *blkg;
	struct cgroup_subsys_state *pos_css;
	int ret;

	ret = blkg_conf_prep(blkcg, &blkcg_policy_throtl, buf, &ctx);
	if (ret)
		return ret;

	tg = blkg_to_tg(ctx.blkg);
	sq = &tg->service_queue;

	if (!ctx.v)
		ctx.v = -1;

	if (is_u64)
		*(u64 *)((void *)tg + cft->private) = ctx.v;
	else
		*(unsigned int *)((void *)tg + cft->private) = ctx.v;

	throtl_log(&tg->service_queue,
		   "limit change rbps=%llu wbps=%llu riops=%u wiops=%u",
		   tg->bps[READ], tg->bps[WRITE],
		   tg->iops[READ], tg->iops[WRITE]);

	/*
	 * Update has_rules[] flags for the updated tg's subtree.  A tg is
	 * considered to have rules if either the tg itself or any of its
	 * ancestors has rules.  This identifies groups without any
	 * restrictions in the whole hierarchy and allows them to bypass
	 * blk-throttle.
	 */
	blkg_for_each_descendant_pre(blkg, pos_css, ctx.blkg)
		tg_update_has_rules(blkg_to_tg(blkg));

	/*
	 * We're already holding queue_lock and know @tg is valid.  Let's
	 * apply the new config directly.
	 *
	 * Restart the slices for both READ and WRITES. It might happen
	 * that a group's limit are dropped suddenly and we don't want to
	 * account recently dispatched IO with new low rate.
	 */
	throtl_start_new_slice(tg, 0);
	throtl_start_new_slice(tg, 1);

	if (tg->flags & THROTL_TG_PENDING) {
		tg_update_disptime(tg);
		throtl_schedule_next_dispatch(sq->parent_sq, true);
	}

	blkg_conf_finish(&ctx);
	return 0;
}

static int tg_set_conf_u64(struct cgroup_subsys_state *css, struct cftype *cft,
			   const char *buf)
{
	return tg_set_conf(css, cft, buf, true);
}

static int tg_set_conf_uint(struct cgroup_subsys_state *css, struct cftype *cft,
			    const char *buf)
{
	return tg_set_conf(css, cft, buf, false);
}

static struct cftype throtl_files[] = {
	{
		.name = "throttle.read_bps_device",
		.private = offsetof(struct throtl_grp, bps[READ]),
		.seq_show = tg_print_conf_u64,
		.write_string = tg_set_conf_u64,
		.max_write_len = 256,
	},
	{
		.name = "throttle.write_bps_device",
		.private = offsetof(struct throtl_grp, bps[WRITE]),
		.seq_show = tg_print_conf_u64,
		.write_string = tg_set_conf_u64,
		.max_write_len = 256,
	},
	{
		.name = "throttle.read_iops_device",
		.private = offsetof(struct throtl_grp, iops[READ]),
		.seq_show = tg_print_conf_uint,
		.write_string = tg_set_conf_uint,
		.max_write_len = 256,
	},
	{
		.name = "throttle.write_iops_device",
		.private = offsetof(struct throtl_grp, iops[WRITE]),
		.seq_show = tg_print_conf_uint,
		.write_string = tg_set_conf_uint,
		.max_write_len = 256,
	},
	{
		.name = "throttle.io_service_bytes",
		.private = offsetof(struct tg_stats_cpu, service_bytes),
		.seq_show = tg_print_cpu_rwstat,
	},
	{
		.name = "throttle.io_serviced",
		.private = offsetof(struct tg_stats_cpu, serviced),
		.seq_show = tg_print_cpu_rwstat,
	},
	{ }	/* terminate */
};

static void throtl_shutdown_wq(struct request_queue *q)
{
	struct throtl_data *td = q->td;

	cancel_work_sync(&td->dispatch_work);
}

static struct blkcg_policy blkcg_policy_throtl = {
	.pd_size		= sizeof(struct throtl_grp),
	.cftypes		= throtl_files,

	.pd_init_fn		= throtl_pd_init,
	.pd_online_fn		= throtl_pd_online,
	.pd_exit_fn		= throtl_pd_exit,
	.pd_reset_stats_fn	= throtl_pd_reset_stats,
};

bool blk_throtl_bio(struct request_queue *q, struct bio *bio)
{
	struct throtl_data *td = q->td;
	struct throtl_qnode *qn = NULL;
	struct throtl_grp *tg;
	struct throtl_service_queue *sq;
	bool rw = bio_data_dir(bio);
	struct blkcg *blkcg;
	bool throttled = false;

	/* see throtl_charge_bio() */
	if (bio->bi_rw & REQ_THROTTLED)
		goto out;
>>>>>>> refs/remotes/origin/master

	/*
	 * A throtl_grp pointer retrieved under rcu can be used to access
	 * basic fields like stats and io rates. If a group has no rules,
	 * just update the dispatch stats in lockless manner and return.
	 */
<<<<<<< HEAD

	rcu_read_lock();
	blkcg = task_blkio_cgroup(current);
	tg = throtl_find_tg(td, blkcg);
	if (tg) {
		throtl_tg_fill_dev_details(td, tg);

		if (tg_no_rule_group(tg, rw)) {
			blkiocg_update_dispatch_stats(&tg->blkg, bio->bi_size,
<<<<<<< HEAD
					rw, bio->bi_rw & REQ_SYNC);
			rcu_read_unlock();
			return 0;
=======
					rw, rw_is_sync(bio->bi_rw));
			rcu_read_unlock();
			goto out;
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}
	rcu_read_unlock();
=======
	rcu_read_lock();
	blkcg = bio_blkcg(bio);
	tg = throtl_lookup_tg(td, blkcg);
	if (tg) {
		if (!tg->has_rules[rw]) {
			throtl_update_dispatch_stats(tg_to_blkg(tg),
						     bio->bi_size, bio->bi_rw);
			goto out_unlock_rcu;
		}
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * Either group has not been allocated yet or it is not an unlimited
	 * IO group
	 */
<<<<<<< HEAD
<<<<<<< HEAD

	spin_lock_irq(q->queue_lock);
	tg = throtl_get_tg(td);

	if (IS_ERR(tg)) {
		if (PTR_ERR(tg)	== -ENODEV) {
			/*
			 * Queue is gone. No queue lock held here.
			 */
			return -ENODEV;
		}
	}
=======
	spin_lock_irq(q->queue_lock);
	tg = throtl_get_tg(td);
	if (unlikely(!tg))
		goto out_unlock;
>>>>>>> refs/remotes/origin/cm-10.0

	if (tg->nr_queued[rw]) {
		/*
		 * There is already another bio queued in same dir. No
		 * need to update dispatch time.
		 */
		update_disptime = false;
		goto queue_bio;

	}

	/* Bio is with-in rate limit of group */
	if (tg_may_dispatch(td, tg, bio, NULL)) {
=======
	spin_lock_irq(q->queue_lock);
	tg = throtl_lookup_create_tg(td, blkcg);
	if (unlikely(!tg))
		goto out_unlock;

	sq = &tg->service_queue;

	while (true) {
		/* throtl is FIFO - if bios are already queued, should queue */
		if (sq->nr_queued[rw])
			break;

		/* if above limits, break to queue */
		if (!tg_may_dispatch(tg, bio, NULL))
			break;

		/* within limits, let's charge and dispatch directly */
>>>>>>> refs/remotes/origin/master
		throtl_charge_bio(tg, bio);

		/*
		 * We need to trim slice even when bios are not being queued
		 * otherwise it might happen that a bio is not queued for
		 * a long time and slice keeps on extending and trim is not
		 * called for a long time. Now if limits are reduced suddenly
		 * we take into account all the IO dispatched so far at new
		 * low rate and * newly queued IO gets a really long dispatch
		 * time.
		 *
		 * So keep on trimming slice even if bio is not queued.
		 */
<<<<<<< HEAD
		throtl_trim_slice(td, tg, rw);
<<<<<<< HEAD
		goto out;
=======
		goto out_unlock;
>>>>>>> refs/remotes/origin/cm-10.0
	}

queue_bio:
	throtl_log_tg(td, tg, "[%c] bio. bdisp=%llu sz=%u bps=%llu"
			" iodisp=%u iops=%u queued=%d/%d",
			rw == READ ? 'R' : 'W',
			tg->bytes_disp[rw], bio->bi_size, tg->bps[rw],
			tg->io_disp[rw], tg->iops[rw],
			tg->nr_queued[READ], tg->nr_queued[WRITE]);

	throtl_add_bio_tg(q->td, tg, bio);
<<<<<<< HEAD
	*biop = NULL;
=======
	throttled = true;
>>>>>>> refs/remotes/origin/cm-10.0

	if (update_disptime) {
		tg_update_disptime(td, tg);
		throtl_schedule_next_dispatch(td);
	}

<<<<<<< HEAD
out:
	spin_unlock_irq(q->queue_lock);
	return 0;
=======
out_unlock:
	spin_unlock_irq(q->queue_lock);
out:
	return throttled;
}

=======
		throtl_trim_slice(tg, rw);

		/*
		 * @bio passed through this layer without being throttled.
		 * Climb up the ladder.  If we''re already at the top, it
		 * can be executed directly.
		 */
		qn = &tg->qnode_on_parent[rw];
		sq = sq->parent_sq;
		tg = sq_to_tg(sq);
		if (!tg)
			goto out_unlock;
	}

	/* out-of-limit, queue to @tg */
	throtl_log(sq, "[%c] bio. bdisp=%llu sz=%u bps=%llu iodisp=%u iops=%u queued=%d/%d",
		   rw == READ ? 'R' : 'W',
		   tg->bytes_disp[rw], bio->bi_size, tg->bps[rw],
		   tg->io_disp[rw], tg->iops[rw],
		   sq->nr_queued[READ], sq->nr_queued[WRITE]);

	bio_associate_current(bio);
	tg->td->nr_queued[rw]++;
	throtl_add_bio_tg(bio, qn, tg);
	throttled = true;

	/*
	 * Update @tg's dispatch time and force schedule dispatch if @tg
	 * was empty before @bio.  The forced scheduling isn't likely to
	 * cause undue delay as @bio is likely to be dispatched directly if
	 * its @tg's disptime is not in the future.
	 */
	if (tg->flags & THROTL_TG_WAS_EMPTY) {
		tg_update_disptime(tg);
		throtl_schedule_next_dispatch(tg->service_queue.parent_sq, true);
	}

out_unlock:
	spin_unlock_irq(q->queue_lock);
out_unlock_rcu:
	rcu_read_unlock();
out:
	/*
	 * As multiple blk-throtls may stack in the same issue path, we
	 * don't want bios to leave with the flag set.  Clear the flag if
	 * being issued.
	 */
	if (!throttled)
		bio->bi_rw &= ~REQ_THROTTLED;
	return throttled;
}

/*
 * Dispatch all bios from all children tg's queued on @parent_sq.  On
 * return, @parent_sq is guaranteed to not have any active children tg's
 * and all bios from previously active tg's are on @parent_sq->bio_lists[].
 */
static void tg_drain_bios(struct throtl_service_queue *parent_sq)
{
	struct throtl_grp *tg;

	while ((tg = throtl_rb_first(parent_sq))) {
		struct throtl_service_queue *sq = &tg->service_queue;
		struct bio *bio;

		throtl_dequeue_tg(tg);

		while ((bio = throtl_peek_queued(&sq->queued[READ])))
			tg_dispatch_one_bio(tg, bio_data_dir(bio));
		while ((bio = throtl_peek_queued(&sq->queued[WRITE])))
			tg_dispatch_one_bio(tg, bio_data_dir(bio));
	}
}

>>>>>>> refs/remotes/origin/master
/**
 * blk_throtl_drain - drain throttled bios
 * @q: request_queue to drain throttled bios for
 *
 * Dispatch all currently throttled bios on @q through ->make_request_fn().
 */
void blk_throtl_drain(struct request_queue *q)
	__releases(q->queue_lock) __acquires(q->queue_lock)
{
	struct throtl_data *td = q->td;
<<<<<<< HEAD
	struct throtl_rb_root *st = &td->tg_service_tree;
	struct throtl_grp *tg;
	struct bio_list bl;
	struct bio *bio;

	queue_lockdep_assert_held(q);

	bio_list_init(&bl);

	while ((tg = throtl_rb_first(st))) {
		throtl_dequeue_tg(td, tg);

		while ((bio = bio_list_peek(&tg->bio_lists[READ])))
			tg_dispatch_one_bio(td, tg, bio_data_dir(bio), &bl);
		while ((bio = bio_list_peek(&tg->bio_lists[WRITE])))
			tg_dispatch_one_bio(td, tg, bio_data_dir(bio), &bl);
	}
	spin_unlock_irq(q->queue_lock);

	while ((bio = bio_list_pop(&bl)))
		generic_make_request(bio);

	spin_lock_irq(q->queue_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct blkcg_gq *blkg;
	struct cgroup_subsys_state *pos_css;
	struct bio *bio;
	int rw;

	queue_lockdep_assert_held(q);
	rcu_read_lock();

	/*
	 * Drain each tg while doing post-order walk on the blkg tree, so
	 * that all bios are propagated to td->service_queue.  It'd be
	 * better to walk service_queue tree directly but blkg walk is
	 * easier.
	 */
	blkg_for_each_descendant_post(blkg, pos_css, td->queue->root_blkg)
		tg_drain_bios(&blkg_to_tg(blkg)->service_queue);

	/* finally, transfer bios from top-level tg's into the td */
	tg_drain_bios(&td->service_queue);

	rcu_read_unlock();
	spin_unlock_irq(q->queue_lock);

	/* all bios now should be in td->service_queue, issue them */
	for (rw = READ; rw <= WRITE; rw++)
		while ((bio = throtl_pop_queued(&td->service_queue.queued[rw],
						NULL)))
			generic_make_request(bio);

	spin_lock_irq(q->queue_lock);
>>>>>>> refs/remotes/origin/master
}

int blk_throtl_init(struct request_queue *q)
{
	struct throtl_data *td;
<<<<<<< HEAD
	struct throtl_grp *tg;
=======
	int ret;
>>>>>>> refs/remotes/origin/master

	td = kzalloc_node(sizeof(*td), GFP_KERNEL, q->node);
	if (!td)
		return -ENOMEM;

<<<<<<< HEAD
	INIT_HLIST_HEAD(&td->tg_list);
	td->tg_service_tree = THROTL_RB_ROOT;
	td->limits_changed = false;
	INIT_DELAYED_WORK(&td->throtl_work, blk_throtl_work);

	/* alloc and Init root group. */
	td->queue = q;
	tg = throtl_alloc_tg(td);

	if (!tg) {
		kfree(td);
		return -ENOMEM;
	}

	td->root_tg = tg;

	rcu_read_lock();
	throtl_init_add_tg_lists(td, tg, &blkio_root_cgroup);
	rcu_read_unlock();

	/* Attach throtl data to request queue */
	q->td = td;
	return 0;
=======
	INIT_WORK(&td->dispatch_work, blk_throtl_dispatch_work_fn);
	throtl_service_queue_init(&td->service_queue, NULL);

	q->td = td;
	td->queue = q;

	/* activate policy */
	ret = blkcg_activate_policy(q, &blkcg_policy_throtl);
	if (ret)
		kfree(td);
	return ret;
>>>>>>> refs/remotes/origin/master
}

void blk_throtl_exit(struct request_queue *q)
{
<<<<<<< HEAD
	struct throtl_data *td = q->td;
	bool wait = false;

	BUG_ON(!td);

	throtl_shutdown_wq(q);

	spin_lock_irq(q->queue_lock);
	throtl_release_tgs(td);

	/* If there are other groups */
	if (td->nr_undestroyed_grps > 0)
		wait = true;

	spin_unlock_irq(q->queue_lock);

	/*
	 * Wait for tg->blkg->key accessors to exit their grace periods.
	 * Do this wait only if there are other undestroyed groups out
	 * there (other than root group). This can happen if cgroup deletion
	 * path claimed the responsibility of cleaning up a group before
	 * queue cleanup code get to the group.
	 *
	 * Do not call synchronize_rcu() unconditionally as there are drivers
	 * which create/delete request queue hundreds of times during scan/boot
	 * and synchronize_rcu() can take significant time and slow down boot.
	 */
	if (wait)
		synchronize_rcu();

	/*
	 * Just being safe to make sure after previous flush if some body did
	 * update limits through cgroup and another work got queued, cancel
	 * it.
	 */
	throtl_shutdown_wq(q);
<<<<<<< HEAD
	throtl_td_free(td);
=======
}

void blk_throtl_release(struct request_queue *q)
{
	kfree(q->td);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	BUG_ON(!q->td);
	throtl_shutdown_wq(q);
	blkcg_deactivate_policy(q, &blkcg_policy_throtl);
	kfree(q->td);
>>>>>>> refs/remotes/origin/master
}

static int __init throtl_init(void)
{
	kthrotld_workqueue = alloc_workqueue("kthrotld", WQ_MEM_RECLAIM, 0);
	if (!kthrotld_workqueue)
		panic("Failed to create kthrotld\n");

<<<<<<< HEAD
	blkio_policy_register(&blkio_policy_throtl);
	return 0;
=======
	return blkcg_policy_register(&blkcg_policy_throtl);
>>>>>>> refs/remotes/origin/master
}

module_init(throtl_init);
