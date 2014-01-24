#ifndef _LINUX_RATELIMIT_H
#define _LINUX_RATELIMIT_H

#include <linux/param.h>
#include <linux/spinlock.h>

#define DEFAULT_RATELIMIT_INTERVAL	(5 * HZ)
#define DEFAULT_RATELIMIT_BURST		10

struct ratelimit_state {
<<<<<<< HEAD
<<<<<<< HEAD
	spinlock_t	lock;		/* protect the state */
=======
	raw_spinlock_t	lock;		/* protect the state */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	raw_spinlock_t	lock;		/* protect the state */
>>>>>>> refs/remotes/origin/master

	int		interval;
	int		burst;
	int		printed;
	int		missed;
	unsigned long	begin;
};

#define DEFINE_RATELIMIT_STATE(name, interval_init, burst_init)		\
									\
	struct ratelimit_state name = {					\
<<<<<<< HEAD
<<<<<<< HEAD
		.lock		= __SPIN_LOCK_UNLOCKED(name.lock),	\
=======
		.lock		= __RAW_SPIN_LOCK_UNLOCKED(name.lock),	\
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.lock		= __RAW_SPIN_LOCK_UNLOCKED(name.lock),	\
>>>>>>> refs/remotes/origin/master
		.interval	= interval_init,			\
		.burst		= burst_init,				\
	}

static inline void ratelimit_state_init(struct ratelimit_state *rs,
					int interval, int burst)
{
<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_init(&rs->lock);
=======
	raw_spin_lock_init(&rs->lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	raw_spin_lock_init(&rs->lock);
>>>>>>> refs/remotes/origin/master
	rs->interval = interval;
	rs->burst = burst;
	rs->printed = 0;
	rs->missed = 0;
	rs->begin = 0;
}

extern struct ratelimit_state printk_ratelimit_state;

extern int ___ratelimit(struct ratelimit_state *rs, const char *func);
#define __ratelimit(state) ___ratelimit(state, __func__)

#ifdef CONFIG_PRINTK

#define WARN_ON_RATELIMIT(condition, state)			\
		WARN_ON((condition) && __ratelimit(state))

<<<<<<< HEAD
#define __WARN_RATELIMIT(condition, state, format...)		\
({								\
	int rtn = 0;						\
	if (unlikely(__ratelimit(state)))			\
		rtn = WARN(condition, format);			\
	rtn;							\
})

#define WARN_RATELIMIT(condition, format...)			\
=======
#define WARN_RATELIMIT(condition, format, ...)			\
>>>>>>> refs/remotes/origin/master
({								\
	static DEFINE_RATELIMIT_STATE(_rs,			\
				      DEFAULT_RATELIMIT_INTERVAL,	\
				      DEFAULT_RATELIMIT_BURST);	\
<<<<<<< HEAD
	__WARN_RATELIMIT(condition, &_rs, format);		\
=======
	int rtn = !!(condition);				\
								\
	if (unlikely(rtn && __ratelimit(&_rs)))			\
		WARN(rtn, format, ##__VA_ARGS__);		\
								\
	rtn;							\
>>>>>>> refs/remotes/origin/master
})

#else

#define WARN_ON_RATELIMIT(condition, state)			\
	WARN_ON(condition)

<<<<<<< HEAD
#define __WARN_RATELIMIT(condition, state, format...)		\
({								\
	int rtn = WARN(condition, format);			\
	rtn;							\
})

#define WARN_RATELIMIT(condition, format...)			\
({								\
	int rtn = WARN(condition, format);			\
=======
#define WARN_RATELIMIT(condition, format, ...)			\
({								\
	int rtn = WARN(condition, format, ##__VA_ARGS__);	\
>>>>>>> refs/remotes/origin/master
	rtn;							\
})

#endif

#endif /* _LINUX_RATELIMIT_H */
