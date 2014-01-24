#ifndef BLK_INTERNAL_H
#define BLK_INTERNAL_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/idr.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/idr.h>

>>>>>>> refs/remotes/origin/master
/* Amount of time in which a process may batch requests */
#define BLK_BATCH_TIME	(HZ/50UL)

/* Number of requests a "batching" process may submit */
#define BLK_BATCH_REQ	32

extern struct kmem_cache *blk_requestq_cachep;
<<<<<<< HEAD
extern struct kobj_type blk_queue_ktype;
<<<<<<< HEAD
=======
=======
extern struct kmem_cache *request_cachep;
extern struct kobj_type blk_queue_ktype;
>>>>>>> refs/remotes/origin/master
extern struct ida blk_queue_ida;

static inline void __blk_get_queue(struct request_queue *q)
{
	kobject_get(&q->kobj);
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

=======

int blk_init_rl(struct request_list *rl, struct request_queue *q,
		gfp_t gfp_mask);
void blk_exit_rl(struct request_list *rl);
>>>>>>> refs/remotes/origin/master
void init_request_from_bio(struct request *req, struct bio *bio);
void blk_rq_bio_prep(struct request_queue *q, struct request *rq,
			struct bio *bio);
int blk_rq_append_bio(struct request_queue *q, struct request *rq,
		      struct bio *bio);
<<<<<<< HEAD
<<<<<<< HEAD
void blk_dequeue_request(struct request *rq);
void __blk_queue_free_tags(struct request_queue *q);
=======
void blk_drain_queue(struct request_queue *q, bool drain_all);
=======
void blk_queue_bypass_start(struct request_queue *q);
void blk_queue_bypass_end(struct request_queue *q);
>>>>>>> refs/remotes/origin/master
void blk_dequeue_request(struct request *rq);
void __blk_queue_free_tags(struct request_queue *q);
bool __blk_end_bidi_request(struct request *rq, int error,
			    unsigned int nr_bytes, unsigned int bidi_bytes);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

void blk_rq_timed_out_timer(unsigned long data);
void blk_delete_timer(struct request *);
void blk_add_timer(struct request *);
void __generic_unplug_device(struct request_queue *);
=======

void blk_rq_timed_out_timer(unsigned long data);
void blk_rq_check_expired(struct request *rq, unsigned long *next_timeout,
			  unsigned int *next_set);
void __blk_add_timer(struct request *req, struct list_head *timeout_list);
void blk_delete_timer(struct request *);
void blk_add_timer(struct request *);


bool bio_attempt_front_merge(struct request_queue *q, struct request *req,
			     struct bio *bio);
bool bio_attempt_back_merge(struct request_queue *q, struct request *req,
			    struct bio *bio);
bool blk_attempt_plug_merge(struct request_queue *q, struct bio *bio,
			    unsigned int *request_count);

void blk_account_io_start(struct request *req, bool new_io);
void blk_account_io_completion(struct request *req, unsigned int bytes);
void blk_account_io_done(struct request *req);
>>>>>>> refs/remotes/origin/master

/*
 * Internal atomic flags for request handling
 */
enum rq_atomic_flags {
	REQ_ATOM_COMPLETE = 0,
<<<<<<< HEAD
<<<<<<< HEAD
	REQ_ATOM_URGENT = 1,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	REQ_ATOM_STARTED,
>>>>>>> refs/remotes/origin/master
};

/*
 * EH timer and IO completion will both attempt to 'grab' the request, make
 * sure that only one of them succeeds
 */
static inline int blk_mark_rq_complete(struct request *rq)
{
	return test_and_set_bit(REQ_ATOM_COMPLETE, &rq->atomic_flags);
}

static inline void blk_clear_rq_complete(struct request *rq)
{
	clear_bit(REQ_ATOM_COMPLETE, &rq->atomic_flags);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int blk_mark_rq_urgent(struct request *rq)
{
	return test_and_set_bit(REQ_ATOM_URGENT, &rq->atomic_flags);
}

static inline void blk_clear_rq_urgent(struct request *rq)
{
	clear_bit(REQ_ATOM_URGENT, &rq->atomic_flags);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Internal elevator interface
 */
#define ELV_ON_HASH(rq)		(!hlist_unhashed(&(rq)->hash))
=======
/*
 * Internal elevator interface
 */
#define ELV_ON_HASH(rq) hash_hashed(&(rq)->hash)
>>>>>>> refs/remotes/origin/master

void blk_insert_flush(struct request *rq);
void blk_abort_flushes(struct request_queue *q);

static inline struct request *__elv_next_request(struct request_queue *q)
{
	struct request *rq;

	while (1) {
		if (!list_empty(&q->queue_head)) {
			rq = list_entry_rq(q->queue_head.next);
			return rq;
		}

		/*
		 * Flush request is running and flush request isn't queueable
		 * in the drive, we can hold the queue till flush request is
		 * finished. Even we don't do this, driver can't dispatch next
		 * requests and will requeue them. And this can improve
		 * throughput too. For example, we have request flush1, write1,
		 * flush 2. flush1 is dispatched, then queue is hold, write1
		 * isn't inserted to queue. After flush1 is finished, flush2
		 * will be dispatched. Since disk cache is already clean,
		 * flush2 will be finished very soon, so looks like flush2 is
		 * folded to flush1.
		 * Since the queue is hold, a flag is set to indicate the queue
		 * should be restarted later. Please see flush_end_io() for
		 * details.
		 */
		if (q->flush_pending_idx != q->flush_running_idx &&
				!queue_flush_queueable(q)) {
			q->flush_queue_delayed = 1;
			return NULL;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		if (test_bit(QUEUE_FLAG_DEAD, &q->queue_flags) ||
		    !q->elevator->ops->elevator_dispatch_fn(q, 0))
=======
		if (unlikely(blk_queue_dead(q)) ||
		    !q->elevator->type->ops.elevator_dispatch_fn(q, 0))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (unlikely(blk_queue_dying(q)) ||
		    !q->elevator->type->ops.elevator_dispatch_fn(q, 0))
>>>>>>> refs/remotes/origin/master
			return NULL;
	}
}

static inline void elv_activate_rq(struct request_queue *q, struct request *rq)
{
	struct elevator_queue *e = q->elevator;

<<<<<<< HEAD
<<<<<<< HEAD
	if (e->ops->elevator_activate_req_fn)
		e->ops->elevator_activate_req_fn(q, rq);
=======
	if (e->type->ops.elevator_activate_req_fn)
		e->type->ops.elevator_activate_req_fn(q, rq);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (e->type->ops.elevator_activate_req_fn)
		e->type->ops.elevator_activate_req_fn(q, rq);
>>>>>>> refs/remotes/origin/master
}

static inline void elv_deactivate_rq(struct request_queue *q, struct request *rq)
{
	struct elevator_queue *e = q->elevator;

<<<<<<< HEAD
<<<<<<< HEAD
	if (e->ops->elevator_deactivate_req_fn)
		e->ops->elevator_deactivate_req_fn(q, rq);
=======
	if (e->type->ops.elevator_deactivate_req_fn)
		e->type->ops.elevator_deactivate_req_fn(q, rq);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (e->type->ops.elevator_deactivate_req_fn)
		e->type->ops.elevator_deactivate_req_fn(q, rq);
>>>>>>> refs/remotes/origin/master
}

#ifdef CONFIG_FAIL_IO_TIMEOUT
int blk_should_fake_timeout(struct request_queue *);
ssize_t part_timeout_show(struct device *, struct device_attribute *, char *);
ssize_t part_timeout_store(struct device *, struct device_attribute *,
				const char *, size_t);
#else
static inline int blk_should_fake_timeout(struct request_queue *q)
{
	return 0;
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
struct io_context *current_io_context(gfp_t gfp_flags, int node);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int ll_back_merge_fn(struct request_queue *q, struct request *req,
		     struct bio *bio);
int ll_front_merge_fn(struct request_queue *q, struct request *req, 
		      struct bio *bio);
int attempt_back_merge(struct request_queue *q, struct request *rq);
int attempt_front_merge(struct request_queue *q, struct request *rq);
int blk_attempt_req_merge(struct request_queue *q, struct request *rq,
				struct request *next);
void blk_recalc_rq_segments(struct request *rq);
void blk_rq_set_mixed_merge(struct request *rq);
<<<<<<< HEAD
<<<<<<< HEAD
=======
bool blk_rq_merge_ok(struct request *rq, struct bio *bio);
int blk_try_merge(struct request *rq, struct bio *bio);
>>>>>>> refs/remotes/origin/cm-10.0

void blk_queue_congestion_threshold(struct request_queue *q);

int blk_dev_init(void);

void elv_quiesce_start(struct request_queue *q);
void elv_quiesce_end(struct request_queue *q);
=======
bool blk_rq_merge_ok(struct request *rq, struct bio *bio);
int blk_try_merge(struct request *rq, struct bio *bio);

void blk_queue_congestion_threshold(struct request_queue *q);

void __blk_run_queue_uncond(struct request_queue *q);

int blk_dev_init(void);
>>>>>>> refs/remotes/origin/master


/*
 * Return the threshold (number of used requests) at which the queue is
 * considered to be congested.  It include a little hysteresis to keep the
 * context switch rate down.
 */
static inline int queue_congestion_on_threshold(struct request_queue *q)
{
	return q->nr_congestion_on;
}

/*
 * The threshold at which a queue is considered to be uncongested
 */
static inline int queue_congestion_off_threshold(struct request_queue *q)
{
	return q->nr_congestion_off;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int blk_cpu_to_group(int cpu)
{
	int group = NR_CPUS;
#ifdef CONFIG_SCHED_MC
	const struct cpumask *mask = cpu_coregroup_mask(cpu);
	group = cpumask_first(mask);
#elif defined(CONFIG_SCHED_SMT)
	group = cpumask_first(topology_thread_cpumask(cpu));
#else
	return cpu;
#endif
	if (likely(group < NR_CPUS))
		return group;
	return cpu;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Contribute to IO statistics IFF:
 *
 *	a) it's attached to a gendisk, and
 *	b) the queue had IO stats enabled when this request was started, and
<<<<<<< HEAD
 *	c) it's a file system request or a discard request
=======
 *	c) it's a file system request
>>>>>>> refs/remotes/origin/master
 */
static inline int blk_do_io_stat(struct request *rq)
{
	return rq->rq_disk &&
	       (rq->cmd_flags & REQ_IO_STAT) &&
<<<<<<< HEAD
	       (rq->cmd_type == REQ_TYPE_FS ||
	        (rq->cmd_flags & REQ_DISCARD));
}

<<<<<<< HEAD
#endif
=======
=======
		(rq->cmd_type == REQ_TYPE_FS);
}

>>>>>>> refs/remotes/origin/master
/*
 * Internal io_context interface
 */
void get_io_context(struct io_context *ioc);
struct io_cq *ioc_lookup_icq(struct io_context *ioc, struct request_queue *q);
<<<<<<< HEAD
struct io_cq *ioc_create_icq(struct request_queue *q, gfp_t gfp_mask);
void ioc_clear_queue(struct request_queue *q);

void create_io_context_slowpath(struct task_struct *task, gfp_t gfp_mask,
				int node);

/**
 * create_io_context - try to create task->io_context
 * @task: target task
 * @gfp_mask: allocation mask
 * @node: allocation node
 *
 * If @task->io_context is %NULL, allocate a new io_context and install it.
 * Returns the current @task->io_context which may be %NULL if allocation
 * failed.
 *
 * Note that this function can't be called with IRQ disabled because
 * task_lock which protects @task->io_context is IRQ-unsafe.
 */
static inline struct io_context *create_io_context(struct task_struct *task,
						   gfp_t gfp_mask, int node)
{
	WARN_ON_ONCE(irqs_disabled());
	if (unlikely(!task->io_context))
		create_io_context_slowpath(task, gfp_mask, node);
	return task->io_context;
=======
struct io_cq *ioc_create_icq(struct io_context *ioc, struct request_queue *q,
			     gfp_t gfp_mask);
void ioc_clear_queue(struct request_queue *q);

int create_task_io_context(struct task_struct *task, gfp_t gfp_mask, int node);

/**
 * create_io_context - try to create task->io_context
 * @gfp_mask: allocation mask
 * @node: allocation node
 *
 * If %current->io_context is %NULL, allocate a new io_context and install
 * it.  Returns the current %current->io_context which may be %NULL if
 * allocation failed.
 *
 * Note that this function can't be called with IRQ disabled because
 * task_lock which protects %current->io_context is IRQ-unsafe.
 */
static inline struct io_context *create_io_context(gfp_t gfp_mask, int node)
{
	WARN_ON_ONCE(irqs_disabled());
	if (unlikely(!current->io_context))
		create_task_io_context(current, gfp_mask, node);
	return current->io_context;
>>>>>>> refs/remotes/origin/master
}

/*
 * Internal throttling interface
 */
#ifdef CONFIG_BLK_DEV_THROTTLING
extern bool blk_throtl_bio(struct request_queue *q, struct bio *bio);
extern void blk_throtl_drain(struct request_queue *q);
extern int blk_throtl_init(struct request_queue *q);
extern void blk_throtl_exit(struct request_queue *q);
<<<<<<< HEAD
extern void blk_throtl_release(struct request_queue *q);
=======
>>>>>>> refs/remotes/origin/master
#else /* CONFIG_BLK_DEV_THROTTLING */
static inline bool blk_throtl_bio(struct request_queue *q, struct bio *bio)
{
	return false;
}
static inline void blk_throtl_drain(struct request_queue *q) { }
static inline int blk_throtl_init(struct request_queue *q) { return 0; }
static inline void blk_throtl_exit(struct request_queue *q) { }
<<<<<<< HEAD
static inline void blk_throtl_release(struct request_queue *q) { }
#endif /* CONFIG_BLK_DEV_THROTTLING */

#endif /* BLK_INTERNAL_H */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif /* CONFIG_BLK_DEV_THROTTLING */

#endif /* BLK_INTERNAL_H */
>>>>>>> refs/remotes/origin/master
