#include "builtin.h"
#include "perf.h"

#include "util/util.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "util/cache.h"
=======
#include "util/evlist.h"
#include "util/cache.h"
#include "util/evsel.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "util/evlist.h"
#include "util/cache.h"
#include "util/evsel.h"
>>>>>>> refs/remotes/origin/master
#include "util/symbol.h"
#include "util/thread.h"
#include "util/header.h"
#include "util/session.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "util/tool.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "util/tool.h"
>>>>>>> refs/remotes/origin/master

#include "util/parse-options.h"
#include "util/trace-event.h"

#include "util/debug.h"

#include <sys/prctl.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <sys/resource.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <sys/resource.h>
>>>>>>> refs/remotes/origin/master

#include <semaphore.h>
#include <pthread.h>
#include <math.h>

<<<<<<< HEAD
<<<<<<< HEAD
static char			const *input_name = "perf.data";
=======
static const char		*input_name;
>>>>>>> refs/remotes/origin/cm-10.0

static char			default_sort_order[] = "avg, max, switch, runtime";
static const char		*sort_order = default_sort_order;

static int			profile_cpu = -1;

#define PR_SET_NAME		15               /* Set process name */
#define MAX_CPUS		4096

static u64			run_measurement_overhead;
static u64			sleep_measurement_overhead;

#define COMM_LEN		20
#define SYM_LEN			129

#define MAX_PID			65536

static unsigned long		nr_tasks;

=======
#define PR_SET_NAME		15               /* Set process name */
#define MAX_CPUS		4096
#define COMM_LEN		20
#define SYM_LEN			129
#define MAX_PID			65536

>>>>>>> refs/remotes/origin/master
struct sched_atom;

struct task_desc {
	unsigned long		nr;
	unsigned long		pid;
	char			comm[COMM_LEN];

	unsigned long		nr_events;
	unsigned long		curr_event;
	struct sched_atom	**atoms;

	pthread_t		thread;
	sem_t			sleep_sem;

	sem_t			ready_for_work;
	sem_t			work_done_sem;

	u64			cpu_usage;
};

enum sched_event_type {
	SCHED_EVENT_RUN,
	SCHED_EVENT_SLEEP,
	SCHED_EVENT_WAKEUP,
	SCHED_EVENT_MIGRATION,
};

struct sched_atom {
	enum sched_event_type	type;
	int			specific_wait;
	u64			timestamp;
	u64			duration;
	unsigned long		nr;
	sem_t			*wait_sem;
	struct task_desc	*wakee;
};

<<<<<<< HEAD
static struct task_desc		*pid_to_task[MAX_PID];

static struct task_desc		**tasks;

static pthread_mutex_t		start_work_mutex = PTHREAD_MUTEX_INITIALIZER;
static u64			start_time;

static pthread_mutex_t		work_done_wait_mutex = PTHREAD_MUTEX_INITIALIZER;

static unsigned long		nr_run_events;
static unsigned long		nr_sleep_events;
static unsigned long		nr_wakeup_events;

static unsigned long		nr_sleep_corrections;
static unsigned long		nr_run_events_optimized;

static unsigned long		targetless_wakeups;
static unsigned long		multitarget_wakeups;

static u64			cpu_usage;
static u64			runavg_cpu_usage;
static u64			parent_cpu_usage;
static u64			runavg_parent_cpu_usage;

static unsigned long		nr_runs;
static u64			sum_runtime;
static u64			sum_fluct;
static u64			run_avg;

static unsigned int		replay_repeat = 10;
static unsigned long		nr_timestamps;
static unsigned long		nr_unordered_timestamps;
static unsigned long		nr_state_machine_bugs;
static unsigned long		nr_context_switch_bugs;
static unsigned long		nr_events;
static unsigned long		nr_lost_chunks;
static unsigned long		nr_lost_events;

=======
>>>>>>> refs/remotes/origin/master
#define TASK_STATE_TO_CHAR_STR "RSDTtZX"

enum thread_state {
	THREAD_SLEEPING = 0,
	THREAD_WAIT_CPU,
	THREAD_SCHED_IN,
	THREAD_IGNORE
};

struct work_atom {
	struct list_head	list;
	enum thread_state	state;
	u64			sched_out_time;
	u64			wake_up_time;
	u64			sched_in_time;
	u64			runtime;
};

struct work_atoms {
	struct list_head	work_list;
	struct thread		*thread;
	struct rb_node		node;
	u64			max_lat;
	u64			max_lat_at;
	u64			total_lat;
	u64			nb_atoms;
	u64			total_runtime;
};

typedef int (*sort_fn_t)(struct work_atoms *, struct work_atoms *);

<<<<<<< HEAD
static struct rb_root		atom_root, sorted_atom_root;

static u64			all_runtime;
static u64			all_count;

=======
struct perf_sched;

struct trace_sched_handler {
	int (*switch_event)(struct perf_sched *sched, struct perf_evsel *evsel,
			    struct perf_sample *sample, struct machine *machine);

	int (*runtime_event)(struct perf_sched *sched, struct perf_evsel *evsel,
			     struct perf_sample *sample, struct machine *machine);

	int (*wakeup_event)(struct perf_sched *sched, struct perf_evsel *evsel,
			    struct perf_sample *sample, struct machine *machine);

	/* PERF_RECORD_FORK event, not sched_process_fork tracepoint */
	int (*fork_event)(struct perf_sched *sched, union perf_event *event,
			  struct machine *machine);

	int (*migrate_task_event)(struct perf_sched *sched,
				  struct perf_evsel *evsel,
				  struct perf_sample *sample,
				  struct machine *machine);
};

struct perf_sched {
	struct perf_tool tool;
	const char	 *sort_order;
	unsigned long	 nr_tasks;
	struct task_desc *pid_to_task[MAX_PID];
	struct task_desc **tasks;
	const struct trace_sched_handler *tp_handler;
	pthread_mutex_t	 start_work_mutex;
	pthread_mutex_t	 work_done_wait_mutex;
	int		 profile_cpu;
/*
 * Track the current task - that way we can know whether there's any
 * weird events, such as a task being switched away that is not current.
 */
	int		 max_cpu;
	u32		 curr_pid[MAX_CPUS];
	struct thread	 *curr_thread[MAX_CPUS];
	char		 next_shortname1;
	char		 next_shortname2;
	unsigned int	 replay_repeat;
	unsigned long	 nr_run_events;
	unsigned long	 nr_sleep_events;
	unsigned long	 nr_wakeup_events;
	unsigned long	 nr_sleep_corrections;
	unsigned long	 nr_run_events_optimized;
	unsigned long	 targetless_wakeups;
	unsigned long	 multitarget_wakeups;
	unsigned long	 nr_runs;
	unsigned long	 nr_timestamps;
	unsigned long	 nr_unordered_timestamps;
	unsigned long	 nr_state_machine_bugs;
	unsigned long	 nr_context_switch_bugs;
	unsigned long	 nr_events;
	unsigned long	 nr_lost_chunks;
	unsigned long	 nr_lost_events;
	u64		 run_measurement_overhead;
	u64		 sleep_measurement_overhead;
	u64		 start_time;
	u64		 cpu_usage;
	u64		 runavg_cpu_usage;
	u64		 parent_cpu_usage;
	u64		 runavg_parent_cpu_usage;
	u64		 sum_runtime;
	u64		 sum_fluct;
	u64		 run_avg;
	u64		 all_runtime;
	u64		 all_count;
	u64		 cpu_last_switched[MAX_CPUS];
	struct rb_root	 atom_root, sorted_atom_root;
	struct list_head sort_list, cmp_pid;
};
>>>>>>> refs/remotes/origin/master

static u64 get_nsecs(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

<<<<<<< HEAD
static void burn_nsecs(u64 nsecs)
=======
static void burn_nsecs(struct perf_sched *sched, u64 nsecs)
>>>>>>> refs/remotes/origin/master
{
	u64 T0 = get_nsecs(), T1;

	do {
		T1 = get_nsecs();
<<<<<<< HEAD
	} while (T1 + run_measurement_overhead < T0 + nsecs);
=======
	} while (T1 + sched->run_measurement_overhead < T0 + nsecs);
>>>>>>> refs/remotes/origin/master
}

static void sleep_nsecs(u64 nsecs)
{
	struct timespec ts;

	ts.tv_nsec = nsecs % 999999999;
	ts.tv_sec = nsecs / 999999999;

	nanosleep(&ts, NULL);
}

<<<<<<< HEAD
static void calibrate_run_measurement_overhead(void)
=======
static void calibrate_run_measurement_overhead(struct perf_sched *sched)
>>>>>>> refs/remotes/origin/master
{
	u64 T0, T1, delta, min_delta = 1000000000ULL;
	int i;

	for (i = 0; i < 10; i++) {
		T0 = get_nsecs();
<<<<<<< HEAD
		burn_nsecs(0);
=======
		burn_nsecs(sched, 0);
>>>>>>> refs/remotes/origin/master
		T1 = get_nsecs();
		delta = T1-T0;
		min_delta = min(min_delta, delta);
	}
<<<<<<< HEAD
	run_measurement_overhead = min_delta;
=======
	sched->run_measurement_overhead = min_delta;
>>>>>>> refs/remotes/origin/master

	printf("run measurement overhead: %" PRIu64 " nsecs\n", min_delta);
}

<<<<<<< HEAD
static void calibrate_sleep_measurement_overhead(void)
=======
static void calibrate_sleep_measurement_overhead(struct perf_sched *sched)
>>>>>>> refs/remotes/origin/master
{
	u64 T0, T1, delta, min_delta = 1000000000ULL;
	int i;

	for (i = 0; i < 10; i++) {
		T0 = get_nsecs();
		sleep_nsecs(10000);
		T1 = get_nsecs();
		delta = T1-T0;
		min_delta = min(min_delta, delta);
	}
	min_delta -= 10000;
<<<<<<< HEAD
	sleep_measurement_overhead = min_delta;
=======
	sched->sleep_measurement_overhead = min_delta;
>>>>>>> refs/remotes/origin/master

	printf("sleep measurement overhead: %" PRIu64 " nsecs\n", min_delta);
}

static struct sched_atom *
get_new_event(struct task_desc *task, u64 timestamp)
{
	struct sched_atom *event = zalloc(sizeof(*event));
	unsigned long idx = task->nr_events;
	size_t size;

	event->timestamp = timestamp;
	event->nr = idx;

	task->nr_events++;
	size = sizeof(struct sched_atom *) * task->nr_events;
	task->atoms = realloc(task->atoms, size);
	BUG_ON(!task->atoms);

	task->atoms[idx] = event;

	return event;
}

static struct sched_atom *last_event(struct task_desc *task)
{
	if (!task->nr_events)
		return NULL;

	return task->atoms[task->nr_events - 1];
}

<<<<<<< HEAD
static void
add_sched_event_run(struct task_desc *task, u64 timestamp, u64 duration)
=======
static void add_sched_event_run(struct perf_sched *sched, struct task_desc *task,
				u64 timestamp, u64 duration)
>>>>>>> refs/remotes/origin/master
{
	struct sched_atom *event, *curr_event = last_event(task);

	/*
	 * optimize an existing RUN event by merging this one
	 * to it:
	 */
	if (curr_event && curr_event->type == SCHED_EVENT_RUN) {
<<<<<<< HEAD
		nr_run_events_optimized++;
=======
		sched->nr_run_events_optimized++;
>>>>>>> refs/remotes/origin/master
		curr_event->duration += duration;
		return;
	}

	event = get_new_event(task, timestamp);

	event->type = SCHED_EVENT_RUN;
	event->duration = duration;

<<<<<<< HEAD
	nr_run_events++;
}

static void
add_sched_event_wakeup(struct task_desc *task, u64 timestamp,
		       struct task_desc *wakee)
=======
	sched->nr_run_events++;
}

static void add_sched_event_wakeup(struct perf_sched *sched, struct task_desc *task,
				   u64 timestamp, struct task_desc *wakee)
>>>>>>> refs/remotes/origin/master
{
	struct sched_atom *event, *wakee_event;

	event = get_new_event(task, timestamp);
	event->type = SCHED_EVENT_WAKEUP;
	event->wakee = wakee;

	wakee_event = last_event(wakee);
	if (!wakee_event || wakee_event->type != SCHED_EVENT_SLEEP) {
<<<<<<< HEAD
		targetless_wakeups++;
		return;
	}
	if (wakee_event->wait_sem) {
		multitarget_wakeups++;
=======
		sched->targetless_wakeups++;
		return;
	}
	if (wakee_event->wait_sem) {
		sched->multitarget_wakeups++;
>>>>>>> refs/remotes/origin/master
		return;
	}

	wakee_event->wait_sem = zalloc(sizeof(*wakee_event->wait_sem));
	sem_init(wakee_event->wait_sem, 0, 0);
	wakee_event->specific_wait = 1;
	event->wait_sem = wakee_event->wait_sem;

<<<<<<< HEAD
	nr_wakeup_events++;
}

static void
add_sched_event_sleep(struct task_desc *task, u64 timestamp,
		      u64 task_state __used)
=======
	sched->nr_wakeup_events++;
}

static void add_sched_event_sleep(struct perf_sched *sched, struct task_desc *task,
				  u64 timestamp, u64 task_state __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	struct sched_atom *event = get_new_event(task, timestamp);

	event->type = SCHED_EVENT_SLEEP;

<<<<<<< HEAD
	nr_sleep_events++;
}

static struct task_desc *register_pid(unsigned long pid, const char *comm)
=======
	sched->nr_sleep_events++;
}

static struct task_desc *register_pid(struct perf_sched *sched,
				      unsigned long pid, const char *comm)
>>>>>>> refs/remotes/origin/master
{
	struct task_desc *task;

	BUG_ON(pid >= MAX_PID);

<<<<<<< HEAD
	task = pid_to_task[pid];
=======
	task = sched->pid_to_task[pid];
>>>>>>> refs/remotes/origin/master

	if (task)
		return task;

	task = zalloc(sizeof(*task));
	task->pid = pid;
<<<<<<< HEAD
	task->nr = nr_tasks;
=======
	task->nr = sched->nr_tasks;
>>>>>>> refs/remotes/origin/master
	strcpy(task->comm, comm);
	/*
	 * every task starts in sleeping state - this gets ignored
	 * if there's no wakeup pointing to this sleep state:
	 */
<<<<<<< HEAD
	add_sched_event_sleep(task, 0, 0);

	pid_to_task[pid] = task;
	nr_tasks++;
	tasks = realloc(tasks, nr_tasks*sizeof(struct task_task *));
	BUG_ON(!tasks);
	tasks[task->nr] = task;

	if (verbose)
		printf("registered task #%ld, PID %ld (%s)\n", nr_tasks, pid, comm);
=======
	add_sched_event_sleep(sched, task, 0, 0);

	sched->pid_to_task[pid] = task;
	sched->nr_tasks++;
	sched->tasks = realloc(sched->tasks, sched->nr_tasks * sizeof(struct task_task *));
	BUG_ON(!sched->tasks);
	sched->tasks[task->nr] = task;

	if (verbose)
		printf("registered task #%ld, PID %ld (%s)\n", sched->nr_tasks, pid, comm);
>>>>>>> refs/remotes/origin/master

	return task;
}


<<<<<<< HEAD
static void print_task_traces(void)
=======
static void print_task_traces(struct perf_sched *sched)
>>>>>>> refs/remotes/origin/master
{
	struct task_desc *task;
	unsigned long i;

<<<<<<< HEAD
	for (i = 0; i < nr_tasks; i++) {
		task = tasks[i];
=======
	for (i = 0; i < sched->nr_tasks; i++) {
		task = sched->tasks[i];
>>>>>>> refs/remotes/origin/master
		printf("task %6ld (%20s:%10ld), nr_events: %ld\n",
			task->nr, task->comm, task->pid, task->nr_events);
	}
}

<<<<<<< HEAD
static void add_cross_task_wakeups(void)
=======
static void add_cross_task_wakeups(struct perf_sched *sched)
>>>>>>> refs/remotes/origin/master
{
	struct task_desc *task1, *task2;
	unsigned long i, j;

<<<<<<< HEAD
	for (i = 0; i < nr_tasks; i++) {
		task1 = tasks[i];
		j = i + 1;
		if (j == nr_tasks)
			j = 0;
		task2 = tasks[j];
		add_sched_event_wakeup(task1, 0, task2);
	}
}

static void
process_sched_event(struct task_desc *this_task __used, struct sched_atom *atom)
=======
	for (i = 0; i < sched->nr_tasks; i++) {
		task1 = sched->tasks[i];
		j = i + 1;
		if (j == sched->nr_tasks)
			j = 0;
		task2 = sched->tasks[j];
		add_sched_event_wakeup(sched, task1, 0, task2);
	}
}

static void perf_sched__process_event(struct perf_sched *sched,
				      struct sched_atom *atom)
>>>>>>> refs/remotes/origin/master
{
	int ret = 0;

	switch (atom->type) {
		case SCHED_EVENT_RUN:
<<<<<<< HEAD
			burn_nsecs(atom->duration);
=======
			burn_nsecs(sched, atom->duration);
>>>>>>> refs/remotes/origin/master
			break;
		case SCHED_EVENT_SLEEP:
			if (atom->wait_sem)
				ret = sem_wait(atom->wait_sem);
			BUG_ON(ret);
			break;
		case SCHED_EVENT_WAKEUP:
			if (atom->wait_sem)
				ret = sem_post(atom->wait_sem);
			BUG_ON(ret);
			break;
		case SCHED_EVENT_MIGRATION:
			break;
		default:
			BUG_ON(1);
	}
}

static u64 get_cpu_usage_nsec_parent(void)
{
	struct rusage ru;
	u64 sum;
	int err;

	err = getrusage(RUSAGE_SELF, &ru);
	BUG_ON(err);

	sum =  ru.ru_utime.tv_sec*1e9 + ru.ru_utime.tv_usec*1e3;
	sum += ru.ru_stime.tv_sec*1e9 + ru.ru_stime.tv_usec*1e3;

	return sum;
}

static int self_open_counters(void)
{
	struct perf_event_attr attr;
	int fd;

	memset(&attr, 0, sizeof(attr));

	attr.type = PERF_TYPE_SOFTWARE;
	attr.config = PERF_COUNT_SW_TASK_CLOCK;

	fd = sys_perf_event_open(&attr, 0, -1, -1, 0);

	if (fd < 0)
<<<<<<< HEAD
		die("Error: sys_perf_event_open() syscall returned"
		    "with %d (%s)\n", fd, strerror(errno));
=======
		pr_err("Error: sys_perf_event_open() syscall returned "
		       "with %d (%s)\n", fd, strerror(errno));
>>>>>>> refs/remotes/origin/master
	return fd;
}

static u64 get_cpu_usage_nsec_self(int fd)
{
	u64 runtime;
	int ret;

	ret = read(fd, &runtime, sizeof(runtime));
	BUG_ON(ret != sizeof(runtime));

	return runtime;
}

<<<<<<< HEAD
static void *thread_func(void *ctx)
{
	struct task_desc *this_task = ctx;
=======
struct sched_thread_parms {
	struct task_desc  *task;
	struct perf_sched *sched;
};

static void *thread_func(void *ctx)
{
	struct sched_thread_parms *parms = ctx;
	struct task_desc *this_task = parms->task;
	struct perf_sched *sched = parms->sched;
>>>>>>> refs/remotes/origin/master
	u64 cpu_usage_0, cpu_usage_1;
	unsigned long i, ret;
	char comm2[22];
	int fd;

<<<<<<< HEAD
	sprintf(comm2, ":%s", this_task->comm);
	prctl(PR_SET_NAME, comm2);
	fd = self_open_counters();

again:
	ret = sem_post(&this_task->ready_for_work);
	BUG_ON(ret);
	ret = pthread_mutex_lock(&start_work_mutex);
	BUG_ON(ret);
	ret = pthread_mutex_unlock(&start_work_mutex);
=======
	zfree(&parms);

	sprintf(comm2, ":%s", this_task->comm);
	prctl(PR_SET_NAME, comm2);
	fd = self_open_counters();
	if (fd < 0)
		return NULL;
again:
	ret = sem_post(&this_task->ready_for_work);
	BUG_ON(ret);
	ret = pthread_mutex_lock(&sched->start_work_mutex);
	BUG_ON(ret);
	ret = pthread_mutex_unlock(&sched->start_work_mutex);
>>>>>>> refs/remotes/origin/master
	BUG_ON(ret);

	cpu_usage_0 = get_cpu_usage_nsec_self(fd);

	for (i = 0; i < this_task->nr_events; i++) {
		this_task->curr_event = i;
<<<<<<< HEAD
		process_sched_event(this_task, this_task->atoms[i]);
=======
		perf_sched__process_event(sched, this_task->atoms[i]);
>>>>>>> refs/remotes/origin/master
	}

	cpu_usage_1 = get_cpu_usage_nsec_self(fd);
	this_task->cpu_usage = cpu_usage_1 - cpu_usage_0;
	ret = sem_post(&this_task->work_done_sem);
	BUG_ON(ret);

<<<<<<< HEAD
	ret = pthread_mutex_lock(&work_done_wait_mutex);
	BUG_ON(ret);
	ret = pthread_mutex_unlock(&work_done_wait_mutex);
=======
	ret = pthread_mutex_lock(&sched->work_done_wait_mutex);
	BUG_ON(ret);
	ret = pthread_mutex_unlock(&sched->work_done_wait_mutex);
>>>>>>> refs/remotes/origin/master
	BUG_ON(ret);

	goto again;
}

<<<<<<< HEAD
static void create_tasks(void)
=======
static void create_tasks(struct perf_sched *sched)
>>>>>>> refs/remotes/origin/master
{
	struct task_desc *task;
	pthread_attr_t attr;
	unsigned long i;
	int err;

	err = pthread_attr_init(&attr);
	BUG_ON(err);
	err = pthread_attr_setstacksize(&attr,
			(size_t) max(16 * 1024, PTHREAD_STACK_MIN));
	BUG_ON(err);
<<<<<<< HEAD
	err = pthread_mutex_lock(&start_work_mutex);
	BUG_ON(err);
	err = pthread_mutex_lock(&work_done_wait_mutex);
	BUG_ON(err);
	for (i = 0; i < nr_tasks; i++) {
		task = tasks[i];
=======
	err = pthread_mutex_lock(&sched->start_work_mutex);
	BUG_ON(err);
	err = pthread_mutex_lock(&sched->work_done_wait_mutex);
	BUG_ON(err);
	for (i = 0; i < sched->nr_tasks; i++) {
		struct sched_thread_parms *parms = malloc(sizeof(*parms));
		BUG_ON(parms == NULL);
		parms->task = task = sched->tasks[i];
		parms->sched = sched;
>>>>>>> refs/remotes/origin/master
		sem_init(&task->sleep_sem, 0, 0);
		sem_init(&task->ready_for_work, 0, 0);
		sem_init(&task->work_done_sem, 0, 0);
		task->curr_event = 0;
<<<<<<< HEAD
		err = pthread_create(&task->thread, &attr, thread_func, task);
=======
		err = pthread_create(&task->thread, &attr, thread_func, parms);
>>>>>>> refs/remotes/origin/master
		BUG_ON(err);
	}
}

<<<<<<< HEAD
static void wait_for_tasks(void)
=======
static void wait_for_tasks(struct perf_sched *sched)
>>>>>>> refs/remotes/origin/master
{
	u64 cpu_usage_0, cpu_usage_1;
	struct task_desc *task;
	unsigned long i, ret;

<<<<<<< HEAD
	start_time = get_nsecs();
	cpu_usage = 0;
	pthread_mutex_unlock(&work_done_wait_mutex);

	for (i = 0; i < nr_tasks; i++) {
		task = tasks[i];
=======
	sched->start_time = get_nsecs();
	sched->cpu_usage = 0;
	pthread_mutex_unlock(&sched->work_done_wait_mutex);

	for (i = 0; i < sched->nr_tasks; i++) {
		task = sched->tasks[i];
>>>>>>> refs/remotes/origin/master
		ret = sem_wait(&task->ready_for_work);
		BUG_ON(ret);
		sem_init(&task->ready_for_work, 0, 0);
	}
<<<<<<< HEAD
	ret = pthread_mutex_lock(&work_done_wait_mutex);
=======
	ret = pthread_mutex_lock(&sched->work_done_wait_mutex);
>>>>>>> refs/remotes/origin/master
	BUG_ON(ret);

	cpu_usage_0 = get_cpu_usage_nsec_parent();

<<<<<<< HEAD
	pthread_mutex_unlock(&start_work_mutex);

	for (i = 0; i < nr_tasks; i++) {
		task = tasks[i];
		ret = sem_wait(&task->work_done_sem);
		BUG_ON(ret);
		sem_init(&task->work_done_sem, 0, 0);
		cpu_usage += task->cpu_usage;
=======
	pthread_mutex_unlock(&sched->start_work_mutex);

	for (i = 0; i < sched->nr_tasks; i++) {
		task = sched->tasks[i];
		ret = sem_wait(&task->work_done_sem);
		BUG_ON(ret);
		sem_init(&task->work_done_sem, 0, 0);
		sched->cpu_usage += task->cpu_usage;
>>>>>>> refs/remotes/origin/master
		task->cpu_usage = 0;
	}

	cpu_usage_1 = get_cpu_usage_nsec_parent();
<<<<<<< HEAD
	if (!runavg_cpu_usage)
		runavg_cpu_usage = cpu_usage;
	runavg_cpu_usage = (runavg_cpu_usage*9 + cpu_usage)/10;

	parent_cpu_usage = cpu_usage_1 - cpu_usage_0;
	if (!runavg_parent_cpu_usage)
		runavg_parent_cpu_usage = parent_cpu_usage;
	runavg_parent_cpu_usage = (runavg_parent_cpu_usage*9 +
				   parent_cpu_usage)/10;

	ret = pthread_mutex_lock(&start_work_mutex);
	BUG_ON(ret);

	for (i = 0; i < nr_tasks; i++) {
		task = tasks[i];
=======
	if (!sched->runavg_cpu_usage)
		sched->runavg_cpu_usage = sched->cpu_usage;
	sched->runavg_cpu_usage = (sched->runavg_cpu_usage * 9 + sched->cpu_usage) / 10;

	sched->parent_cpu_usage = cpu_usage_1 - cpu_usage_0;
	if (!sched->runavg_parent_cpu_usage)
		sched->runavg_parent_cpu_usage = sched->parent_cpu_usage;
	sched->runavg_parent_cpu_usage = (sched->runavg_parent_cpu_usage * 9 +
					 sched->parent_cpu_usage)/10;

	ret = pthread_mutex_lock(&sched->start_work_mutex);
	BUG_ON(ret);

	for (i = 0; i < sched->nr_tasks; i++) {
		task = sched->tasks[i];
>>>>>>> refs/remotes/origin/master
		sem_init(&task->sleep_sem, 0, 0);
		task->curr_event = 0;
	}
}

<<<<<<< HEAD
static void run_one_test(void)
=======
static void run_one_test(struct perf_sched *sched)
>>>>>>> refs/remotes/origin/master
{
	u64 T0, T1, delta, avg_delta, fluct;

	T0 = get_nsecs();
<<<<<<< HEAD
	wait_for_tasks();
	T1 = get_nsecs();

	delta = T1 - T0;
	sum_runtime += delta;
	nr_runs++;

	avg_delta = sum_runtime / nr_runs;
=======
	wait_for_tasks(sched);
	T1 = get_nsecs();

	delta = T1 - T0;
	sched->sum_runtime += delta;
	sched->nr_runs++;

	avg_delta = sched->sum_runtime / sched->nr_runs;
>>>>>>> refs/remotes/origin/master
	if (delta < avg_delta)
		fluct = avg_delta - delta;
	else
		fluct = delta - avg_delta;
<<<<<<< HEAD
	sum_fluct += fluct;
	if (!run_avg)
		run_avg = delta;
	run_avg = (run_avg*9 + delta)/10;

	printf("#%-3ld: %0.3f, ",
		nr_runs, (double)delta/1000000.0);

	printf("ravg: %0.2f, ",
		(double)run_avg/1e6);

	printf("cpu: %0.2f / %0.2f",
		(double)cpu_usage/1e6, (double)runavg_cpu_usage/1e6);
=======
	sched->sum_fluct += fluct;
	if (!sched->run_avg)
		sched->run_avg = delta;
	sched->run_avg = (sched->run_avg * 9 + delta) / 10;

	printf("#%-3ld: %0.3f, ", sched->nr_runs, (double)delta / 1000000.0);

	printf("ravg: %0.2f, ", (double)sched->run_avg / 1e6);

	printf("cpu: %0.2f / %0.2f",
		(double)sched->cpu_usage / 1e6, (double)sched->runavg_cpu_usage / 1e6);
>>>>>>> refs/remotes/origin/master

#if 0
	/*
	 * rusage statistics done by the parent, these are less
<<<<<<< HEAD
	 * accurate than the sum_exec_runtime based statistics:
	 */
	printf(" [%0.2f / %0.2f]",
		(double)parent_cpu_usage/1e6,
		(double)runavg_parent_cpu_usage/1e6);
=======
	 * accurate than the sched->sum_exec_runtime based statistics:
	 */
	printf(" [%0.2f / %0.2f]",
		(double)sched->parent_cpu_usage/1e6,
		(double)sched->runavg_parent_cpu_usage/1e6);
>>>>>>> refs/remotes/origin/master
#endif

	printf("\n");

<<<<<<< HEAD
	if (nr_sleep_corrections)
		printf(" (%ld sleep corrections)\n", nr_sleep_corrections);
	nr_sleep_corrections = 0;
}

static void test_calibrations(void)
=======
	if (sched->nr_sleep_corrections)
		printf(" (%ld sleep corrections)\n", sched->nr_sleep_corrections);
	sched->nr_sleep_corrections = 0;
}

static void test_calibrations(struct perf_sched *sched)
>>>>>>> refs/remotes/origin/master
{
	u64 T0, T1;

	T0 = get_nsecs();
<<<<<<< HEAD
	burn_nsecs(1e6);
=======
	burn_nsecs(sched, 1e6);
>>>>>>> refs/remotes/origin/master
	T1 = get_nsecs();

	printf("the run test took %" PRIu64 " nsecs\n", T1 - T0);

	T0 = get_nsecs();
	sleep_nsecs(1e6);
	T1 = get_nsecs();

	printf("the sleep test took %" PRIu64 " nsecs\n", T1 - T0);
}

<<<<<<< HEAD
#define FILL_FIELD(ptr, field, event, data)	\
	ptr.field = (typeof(ptr.field)) raw_field_value(event, #field, data)

#define FILL_ARRAY(ptr, array, event, data)			\
do {								\
	void *__array = raw_field_ptr(event, #array, data);	\
	memcpy(ptr.array, __array, sizeof(ptr.array));	\
} while(0)

#define FILL_COMMON_FIELDS(ptr, event, data)			\
do {								\
	FILL_FIELD(ptr, common_type, event, data);		\
	FILL_FIELD(ptr, common_flags, event, data);		\
	FILL_FIELD(ptr, common_preempt_count, event, data);	\
	FILL_FIELD(ptr, common_pid, event, data);		\
	FILL_FIELD(ptr, common_tgid, event, data);		\
} while (0)



struct trace_switch_event {
	u32 size;

	u16 common_type;
	u8 common_flags;
	u8 common_preempt_count;
	u32 common_pid;
	u32 common_tgid;

	char prev_comm[16];
	u32 prev_pid;
	u32 prev_prio;
	u64 prev_state;
	char next_comm[16];
	u32 next_pid;
	u32 next_prio;
};

struct trace_runtime_event {
	u32 size;

	u16 common_type;
	u8 common_flags;
	u8 common_preempt_count;
	u32 common_pid;
	u32 common_tgid;

	char comm[16];
	u32 pid;
	u64 runtime;
	u64 vruntime;
};

struct trace_wakeup_event {
	u32 size;

	u16 common_type;
	u8 common_flags;
	u8 common_preempt_count;
	u32 common_pid;
	u32 common_tgid;

	char comm[16];
	u32 pid;

	u32 prio;
	u32 success;
	u32 cpu;
};

struct trace_fork_event {
	u32 size;

	u16 common_type;
	u8 common_flags;
	u8 common_preempt_count;
	u32 common_pid;
	u32 common_tgid;

	char parent_comm[16];
	u32 parent_pid;
	char child_comm[16];
	u32 child_pid;
};

struct trace_migrate_task_event {
	u32 size;

	u16 common_type;
	u8 common_flags;
	u8 common_preempt_count;
	u32 common_pid;
	u32 common_tgid;

	char comm[16];
	u32 pid;

	u32 prio;
	u32 cpu;
};

struct trace_sched_handler {
	void (*switch_event)(struct trace_switch_event *,
<<<<<<< HEAD
			     struct perf_session *,
=======
			     struct machine *,
>>>>>>> refs/remotes/origin/cm-10.0
			     struct event *,
			     int cpu,
			     u64 timestamp,
			     struct thread *thread);

	void (*runtime_event)(struct trace_runtime_event *,
<<<<<<< HEAD
			      struct perf_session *,
=======
			      struct machine *,
>>>>>>> refs/remotes/origin/cm-10.0
			      struct event *,
			      int cpu,
			      u64 timestamp,
			      struct thread *thread);

	void (*wakeup_event)(struct trace_wakeup_event *,
<<<<<<< HEAD
			     struct perf_session *,
=======
			     struct machine *,
>>>>>>> refs/remotes/origin/cm-10.0
			     struct event *,
			     int cpu,
			     u64 timestamp,
			     struct thread *thread);

	void (*fork_event)(struct trace_fork_event *,
			   struct event *,
			   int cpu,
			   u64 timestamp,
			   struct thread *thread);

	void (*migrate_task_event)(struct trace_migrate_task_event *,
<<<<<<< HEAD
			   struct perf_session *session,
=======
			   struct machine *machine,
>>>>>>> refs/remotes/origin/cm-10.0
			   struct event *,
			   int cpu,
			   u64 timestamp,
			   struct thread *thread);
};


static void
replay_wakeup_event(struct trace_wakeup_event *wakeup_event,
<<<<<<< HEAD
		    struct perf_session *session __used,
=======
		    struct machine *machine __used,
>>>>>>> refs/remotes/origin/cm-10.0
		    struct event *event,
		    int cpu __used,
		    u64 timestamp __used,
		    struct thread *thread __used)
{
	struct task_desc *waker, *wakee;

	if (verbose) {
		printf("sched_wakeup event %p\n", event);

		printf(" ... pid %d woke up %s/%d\n",
			wakeup_event->common_pid,
			wakeup_event->comm,
			wakeup_event->pid);
	}

	waker = register_pid(wakeup_event->common_pid, "<unknown>");
	wakee = register_pid(wakeup_event->pid, wakeup_event->comm);

	add_sched_event_wakeup(waker, timestamp, wakee);
}

static u64 cpu_last_switched[MAX_CPUS];

static void
replay_switch_event(struct trace_switch_event *switch_event,
<<<<<<< HEAD
		    struct perf_session *session __used,
=======
		    struct machine *machine __used,
>>>>>>> refs/remotes/origin/cm-10.0
		    struct event *event,
		    int cpu,
		    u64 timestamp,
		    struct thread *thread __used)
{
	struct task_desc *prev, __used *next;
	u64 timestamp0;
	s64 delta;

	if (verbose)
		printf("sched_switch event %p\n", event);

	if (cpu >= MAX_CPUS || cpu < 0)
		return;

	timestamp0 = cpu_last_switched[cpu];
=======
static int
replay_wakeup_event(struct perf_sched *sched,
		    struct perf_evsel *evsel, struct perf_sample *sample,
		    struct machine *machine __maybe_unused)
{
	const char *comm = perf_evsel__strval(evsel, sample, "comm");
	const u32 pid	 = perf_evsel__intval(evsel, sample, "pid");
	struct task_desc *waker, *wakee;

	if (verbose) {
		printf("sched_wakeup event %p\n", evsel);

		printf(" ... pid %d woke up %s/%d\n", sample->tid, comm, pid);
	}

	waker = register_pid(sched, sample->tid, "<unknown>");
	wakee = register_pid(sched, pid, comm);

	add_sched_event_wakeup(sched, waker, sample->time, wakee);
	return 0;
}

static int replay_switch_event(struct perf_sched *sched,
			       struct perf_evsel *evsel,
			       struct perf_sample *sample,
			       struct machine *machine __maybe_unused)
{
	const char *prev_comm  = perf_evsel__strval(evsel, sample, "prev_comm"),
		   *next_comm  = perf_evsel__strval(evsel, sample, "next_comm");
	const u32 prev_pid = perf_evsel__intval(evsel, sample, "prev_pid"),
		  next_pid = perf_evsel__intval(evsel, sample, "next_pid");
	const u64 prev_state = perf_evsel__intval(evsel, sample, "prev_state");
	struct task_desc *prev, __maybe_unused *next;
	u64 timestamp0, timestamp = sample->time;
	int cpu = sample->cpu;
	s64 delta;

	if (verbose)
		printf("sched_switch event %p\n", evsel);

	if (cpu >= MAX_CPUS || cpu < 0)
		return 0;

	timestamp0 = sched->cpu_last_switched[cpu];
>>>>>>> refs/remotes/origin/master
	if (timestamp0)
		delta = timestamp - timestamp0;
	else
		delta = 0;

<<<<<<< HEAD
	if (delta < 0)
		die("hm, delta: %" PRIu64 " < 0 ?\n", delta);

	if (verbose) {
		printf(" ... switch from %s/%d to %s/%d [ran %" PRIu64 " nsecs]\n",
			switch_event->prev_comm, switch_event->prev_pid,
			switch_event->next_comm, switch_event->next_pid,
			delta);
	}

	prev = register_pid(switch_event->prev_pid, switch_event->prev_comm);
	next = register_pid(switch_event->next_pid, switch_event->next_comm);

	cpu_last_switched[cpu] = timestamp;

	add_sched_event_run(prev, timestamp, delta);
	add_sched_event_sleep(prev, timestamp, switch_event->prev_state);
}


static void
replay_fork_event(struct trace_fork_event *fork_event,
		  struct event *event,
		  int cpu __used,
		  u64 timestamp __used,
		  struct thread *thread __used)
{
	if (verbose) {
		printf("sched_fork event %p\n", event);
		printf("... parent: %s/%d\n", fork_event->parent_comm, fork_event->parent_pid);
		printf("...  child: %s/%d\n", fork_event->child_comm, fork_event->child_pid);
	}
	register_pid(fork_event->parent_pid, fork_event->parent_comm);
	register_pid(fork_event->child_pid, fork_event->child_comm);
}

static struct trace_sched_handler replay_ops  = {
	.wakeup_event		= replay_wakeup_event,
	.switch_event		= replay_switch_event,
	.fork_event		= replay_fork_event,
};
=======
	if (delta < 0) {
		pr_err("hm, delta: %" PRIu64 " < 0 ?\n", delta);
		return -1;
	}

	pr_debug(" ... switch from %s/%d to %s/%d [ran %" PRIu64 " nsecs]\n",
		 prev_comm, prev_pid, next_comm, next_pid, delta);

	prev = register_pid(sched, prev_pid, prev_comm);
	next = register_pid(sched, next_pid, next_comm);

	sched->cpu_last_switched[cpu] = timestamp;

	add_sched_event_run(sched, prev, timestamp, delta);
	add_sched_event_sleep(sched, prev, timestamp, prev_state);

	return 0;
}

static int replay_fork_event(struct perf_sched *sched,
			     union perf_event *event,
			     struct machine *machine)
{
	struct thread *child, *parent;

	child = machine__findnew_thread(machine, event->fork.pid,
					event->fork.tid);
	parent = machine__findnew_thread(machine, event->fork.ppid,
					 event->fork.ptid);

	if (child == NULL || parent == NULL) {
		pr_debug("thread does not exist on fork event: child %p, parent %p\n",
				 child, parent);
		return 0;
	}

	if (verbose) {
		printf("fork event\n");
		printf("... parent: %s/%d\n", thread__comm_str(parent), parent->tid);
		printf("...  child: %s/%d\n", thread__comm_str(child), child->tid);
	}

	register_pid(sched, parent->tid, thread__comm_str(parent));
	register_pid(sched, child->tid, thread__comm_str(child));
	return 0;
}
>>>>>>> refs/remotes/origin/master

struct sort_dimension {
	const char		*name;
	sort_fn_t		cmp;
	struct list_head	list;
};

<<<<<<< HEAD
static LIST_HEAD(cmp_pid);

=======
>>>>>>> refs/remotes/origin/master
static int
thread_lat_cmp(struct list_head *list, struct work_atoms *l, struct work_atoms *r)
{
	struct sort_dimension *sort;
	int ret = 0;

	BUG_ON(list_empty(list));

	list_for_each_entry(sort, list, list) {
		ret = sort->cmp(l, r);
		if (ret)
			return ret;
	}

	return ret;
}

static struct work_atoms *
thread_atoms_search(struct rb_root *root, struct thread *thread,
			 struct list_head *sort_list)
{
	struct rb_node *node = root->rb_node;
	struct work_atoms key = { .thread = thread };

	while (node) {
		struct work_atoms *atoms;
		int cmp;

		atoms = container_of(node, struct work_atoms, node);

		cmp = thread_lat_cmp(sort_list, &key, atoms);
		if (cmp > 0)
			node = node->rb_left;
		else if (cmp < 0)
			node = node->rb_right;
		else {
			BUG_ON(thread != atoms->thread);
			return atoms;
		}
	}
	return NULL;
}

static void
__thread_latency_insert(struct rb_root *root, struct work_atoms *data,
			 struct list_head *sort_list)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;

	while (*new) {
		struct work_atoms *this;
		int cmp;

		this = container_of(*new, struct work_atoms, node);
		parent = *new;

		cmp = thread_lat_cmp(sort_list, data, this);

		if (cmp > 0)
			new = &((*new)->rb_left);
		else
			new = &((*new)->rb_right);
	}

	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);
}

<<<<<<< HEAD
static void thread_atoms_insert(struct thread *thread)
{
	struct work_atoms *atoms = zalloc(sizeof(*atoms));
	if (!atoms)
		die("No memory");

	atoms->thread = thread;
	INIT_LIST_HEAD(&atoms->work_list);
	__thread_latency_insert(&atom_root, atoms, &cmp_pid);
}

static void
latency_fork_event(struct trace_fork_event *fork_event __used,
		   struct event *event __used,
		   int cpu __used,
		   u64 timestamp __used,
		   struct thread *thread __used)
{
	/* should insert the newcomer */
}

__used
static char sched_out_state(struct trace_switch_event *switch_event)
{
	const char *str = TASK_STATE_TO_CHAR_STR;

	return str[switch_event->prev_state];
}

static void
=======
static int thread_atoms_insert(struct perf_sched *sched, struct thread *thread)
{
	struct work_atoms *atoms = zalloc(sizeof(*atoms));
	if (!atoms) {
		pr_err("No memory at %s\n", __func__);
		return -1;
	}

	atoms->thread = thread;
	INIT_LIST_HEAD(&atoms->work_list);
	__thread_latency_insert(&sched->atom_root, atoms, &sched->cmp_pid);
	return 0;
}

static char sched_out_state(u64 prev_state)
{
	const char *str = TASK_STATE_TO_CHAR_STR;

	return str[prev_state];
}

static int
>>>>>>> refs/remotes/origin/master
add_sched_out_event(struct work_atoms *atoms,
		    char run_state,
		    u64 timestamp)
{
	struct work_atom *atom = zalloc(sizeof(*atom));
<<<<<<< HEAD
	if (!atom)
		die("Non memory");
=======
	if (!atom) {
		pr_err("Non memory at %s", __func__);
		return -1;
	}
>>>>>>> refs/remotes/origin/master

	atom->sched_out_time = timestamp;

	if (run_state == 'R') {
		atom->state = THREAD_WAIT_CPU;
		atom->wake_up_time = atom->sched_out_time;
	}

	list_add_tail(&atom->list, &atoms->work_list);
<<<<<<< HEAD
}

static void
add_runtime_event(struct work_atoms *atoms, u64 delta, u64 timestamp __used)
=======
	return 0;
}

static void
add_runtime_event(struct work_atoms *atoms, u64 delta,
		  u64 timestamp __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	struct work_atom *atom;

	BUG_ON(list_empty(&atoms->work_list));

	atom = list_entry(atoms->work_list.prev, struct work_atom, list);

	atom->runtime += delta;
	atoms->total_runtime += delta;
}

static void
add_sched_in_event(struct work_atoms *atoms, u64 timestamp)
{
	struct work_atom *atom;
	u64 delta;

	if (list_empty(&atoms->work_list))
		return;

	atom = list_entry(atoms->work_list.prev, struct work_atom, list);

	if (atom->state != THREAD_WAIT_CPU)
		return;

	if (timestamp < atom->wake_up_time) {
		atom->state = THREAD_IGNORE;
		return;
	}

	atom->state = THREAD_SCHED_IN;
	atom->sched_in_time = timestamp;

	delta = atom->sched_in_time - atom->wake_up_time;
	atoms->total_lat += delta;
	if (delta > atoms->max_lat) {
		atoms->max_lat = delta;
		atoms->max_lat_at = timestamp;
	}
	atoms->nb_atoms++;
}

<<<<<<< HEAD
static void
latency_switch_event(struct trace_switch_event *switch_event,
<<<<<<< HEAD
		     struct perf_session *session,
=======
		     struct machine *machine,
>>>>>>> refs/remotes/origin/cm-10.0
		     struct event *event __used,
		     int cpu,
		     u64 timestamp,
		     struct thread *thread __used)
{
	struct work_atoms *out_events, *in_events;
	struct thread *sched_out, *sched_in;
	u64 timestamp0;
=======
static int latency_switch_event(struct perf_sched *sched,
				struct perf_evsel *evsel,
				struct perf_sample *sample,
				struct machine *machine)
{
	const u32 prev_pid = perf_evsel__intval(evsel, sample, "prev_pid"),
		  next_pid = perf_evsel__intval(evsel, sample, "next_pid");
	const u64 prev_state = perf_evsel__intval(evsel, sample, "prev_state");
	struct work_atoms *out_events, *in_events;
	struct thread *sched_out, *sched_in;
	u64 timestamp0, timestamp = sample->time;
	int cpu = sample->cpu;
>>>>>>> refs/remotes/origin/master
	s64 delta;

	BUG_ON(cpu >= MAX_CPUS || cpu < 0);

<<<<<<< HEAD
	timestamp0 = cpu_last_switched[cpu];
	cpu_last_switched[cpu] = timestamp;
=======
	timestamp0 = sched->cpu_last_switched[cpu];
	sched->cpu_last_switched[cpu] = timestamp;
>>>>>>> refs/remotes/origin/master
	if (timestamp0)
		delta = timestamp - timestamp0;
	else
		delta = 0;

<<<<<<< HEAD
	if (delta < 0)
		die("hm, delta: %" PRIu64 " < 0 ?\n", delta);


<<<<<<< HEAD
	sched_out = perf_session__findnew(session, switch_event->prev_pid);
	sched_in = perf_session__findnew(session, switch_event->next_pid);
=======
	sched_out = machine__findnew_thread(machine, switch_event->prev_pid);
	sched_in = machine__findnew_thread(machine, switch_event->next_pid);
>>>>>>> refs/remotes/origin/cm-10.0

	out_events = thread_atoms_search(&atom_root, sched_out, &cmp_pid);
	if (!out_events) {
		thread_atoms_insert(sched_out);
		out_events = thread_atoms_search(&atom_root, sched_out, &cmp_pid);
		if (!out_events)
			die("out-event: Internal tree error");
	}
	add_sched_out_event(out_events, sched_out_state(switch_event), timestamp);

	in_events = thread_atoms_search(&atom_root, sched_in, &cmp_pid);
	if (!in_events) {
		thread_atoms_insert(sched_in);
		in_events = thread_atoms_search(&atom_root, sched_in, &cmp_pid);
		if (!in_events)
			die("in-event: Internal tree error");
=======
	if (delta < 0) {
		pr_err("hm, delta: %" PRIu64 " < 0 ?\n", delta);
		return -1;
	}

	sched_out = machine__findnew_thread(machine, 0, prev_pid);
	sched_in = machine__findnew_thread(machine, 0, next_pid);

	out_events = thread_atoms_search(&sched->atom_root, sched_out, &sched->cmp_pid);
	if (!out_events) {
		if (thread_atoms_insert(sched, sched_out))
			return -1;
		out_events = thread_atoms_search(&sched->atom_root, sched_out, &sched->cmp_pid);
		if (!out_events) {
			pr_err("out-event: Internal tree error");
			return -1;
		}
	}
	if (add_sched_out_event(out_events, sched_out_state(prev_state), timestamp))
		return -1;

	in_events = thread_atoms_search(&sched->atom_root, sched_in, &sched->cmp_pid);
	if (!in_events) {
		if (thread_atoms_insert(sched, sched_in))
			return -1;
		in_events = thread_atoms_search(&sched->atom_root, sched_in, &sched->cmp_pid);
		if (!in_events) {
			pr_err("in-event: Internal tree error");
			return -1;
		}
>>>>>>> refs/remotes/origin/master
		/*
		 * Take came in we have not heard about yet,
		 * add in an initial atom in runnable state:
		 */
<<<<<<< HEAD
		add_sched_out_event(in_events, 'R', timestamp);
	}
	add_sched_in_event(in_events, timestamp);
}

static void
latency_runtime_event(struct trace_runtime_event *runtime_event,
<<<<<<< HEAD
		     struct perf_session *session,
=======
		     struct machine *machine,
>>>>>>> refs/remotes/origin/cm-10.0
		     struct event *event __used,
		     int cpu,
		     u64 timestamp,
		     struct thread *this_thread __used)
{
<<<<<<< HEAD
	struct thread *thread = perf_session__findnew(session, runtime_event->pid);
=======
	struct thread *thread = machine__findnew_thread(machine, runtime_event->pid);
>>>>>>> refs/remotes/origin/cm-10.0
	struct work_atoms *atoms = thread_atoms_search(&atom_root, thread, &cmp_pid);

	BUG_ON(cpu >= MAX_CPUS || cpu < 0);
	if (!atoms) {
		thread_atoms_insert(thread);
		atoms = thread_atoms_search(&atom_root, thread, &cmp_pid);
		if (!atoms)
			die("in-event: Internal tree error");
		add_sched_out_event(atoms, 'R', timestamp);
	}

	add_runtime_event(atoms, runtime_event->runtime, timestamp);
}

static void
latency_wakeup_event(struct trace_wakeup_event *wakeup_event,
<<<<<<< HEAD
		     struct perf_session *session,
=======
		     struct machine *machine,
>>>>>>> refs/remotes/origin/cm-10.0
		     struct event *__event __used,
		     int cpu __used,
		     u64 timestamp,
		     struct thread *thread __used)
{
	struct work_atoms *atoms;
	struct work_atom *atom;
	struct thread *wakee;

	/* Note for later, it may be interesting to observe the failing cases */
	if (!wakeup_event->success)
		return;

<<<<<<< HEAD
	wakee = perf_session__findnew(session, wakeup_event->pid);
=======
	wakee = machine__findnew_thread(machine, wakeup_event->pid);
>>>>>>> refs/remotes/origin/cm-10.0
	atoms = thread_atoms_search(&atom_root, wakee, &cmp_pid);
	if (!atoms) {
		thread_atoms_insert(wakee);
		atoms = thread_atoms_search(&atom_root, wakee, &cmp_pid);
		if (!atoms)
			die("wakeup-event: Internal tree error");
		add_sched_out_event(atoms, 'S', timestamp);
=======
		if (add_sched_out_event(in_events, 'R', timestamp))
			return -1;
	}
	add_sched_in_event(in_events, timestamp);

	return 0;
}

static int latency_runtime_event(struct perf_sched *sched,
				 struct perf_evsel *evsel,
				 struct perf_sample *sample,
				 struct machine *machine)
{
	const u32 pid	   = perf_evsel__intval(evsel, sample, "pid");
	const u64 runtime  = perf_evsel__intval(evsel, sample, "runtime");
	struct thread *thread = machine__findnew_thread(machine, 0, pid);
	struct work_atoms *atoms = thread_atoms_search(&sched->atom_root, thread, &sched->cmp_pid);
	u64 timestamp = sample->time;
	int cpu = sample->cpu;

	BUG_ON(cpu >= MAX_CPUS || cpu < 0);
	if (!atoms) {
		if (thread_atoms_insert(sched, thread))
			return -1;
		atoms = thread_atoms_search(&sched->atom_root, thread, &sched->cmp_pid);
		if (!atoms) {
			pr_err("in-event: Internal tree error");
			return -1;
		}
		if (add_sched_out_event(atoms, 'R', timestamp))
			return -1;
	}

	add_runtime_event(atoms, runtime, timestamp);
	return 0;
}

static int latency_wakeup_event(struct perf_sched *sched,
				struct perf_evsel *evsel,
				struct perf_sample *sample,
				struct machine *machine)
{
	const u32 pid	  = perf_evsel__intval(evsel, sample, "pid"),
		  success = perf_evsel__intval(evsel, sample, "success");
	struct work_atoms *atoms;
	struct work_atom *atom;
	struct thread *wakee;
	u64 timestamp = sample->time;

	/* Note for later, it may be interesting to observe the failing cases */
	if (!success)
		return 0;

	wakee = machine__findnew_thread(machine, 0, pid);
	atoms = thread_atoms_search(&sched->atom_root, wakee, &sched->cmp_pid);
	if (!atoms) {
		if (thread_atoms_insert(sched, wakee))
			return -1;
		atoms = thread_atoms_search(&sched->atom_root, wakee, &sched->cmp_pid);
		if (!atoms) {
			pr_err("wakeup-event: Internal tree error");
			return -1;
		}
		if (add_sched_out_event(atoms, 'S', timestamp))
			return -1;
>>>>>>> refs/remotes/origin/master
	}

	BUG_ON(list_empty(&atoms->work_list));

	atom = list_entry(atoms->work_list.prev, struct work_atom, list);

	/*
	 * You WILL be missing events if you've recorded only
	 * one CPU, or are only looking at only one, so don't
	 * make useless noise.
	 */
<<<<<<< HEAD
	if (profile_cpu == -1 && atom->state != THREAD_SLEEPING)
		nr_state_machine_bugs++;

	nr_timestamps++;
	if (atom->sched_out_time > timestamp) {
		nr_unordered_timestamps++;
		return;
=======
	if (sched->profile_cpu == -1 && atom->state != THREAD_SLEEPING)
		sched->nr_state_machine_bugs++;

	sched->nr_timestamps++;
	if (atom->sched_out_time > timestamp) {
		sched->nr_unordered_timestamps++;
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	atom->state = THREAD_WAIT_CPU;
	atom->wake_up_time = timestamp;
<<<<<<< HEAD
}

static void
latency_migrate_task_event(struct trace_migrate_task_event *migrate_task_event,
<<<<<<< HEAD
		     struct perf_session *session,
=======
		     struct machine *machine,
>>>>>>> refs/remotes/origin/cm-10.0
		     struct event *__event __used,
		     int cpu __used,
		     u64 timestamp,
		     struct thread *thread __used)
{
=======
	return 0;
}

static int latency_migrate_task_event(struct perf_sched *sched,
				      struct perf_evsel *evsel,
				      struct perf_sample *sample,
				      struct machine *machine)
{
	const u32 pid = perf_evsel__intval(evsel, sample, "pid");
	u64 timestamp = sample->time;
>>>>>>> refs/remotes/origin/master
	struct work_atoms *atoms;
	struct work_atom *atom;
	struct thread *migrant;

	/*
	 * Only need to worry about migration when profiling one CPU.
	 */
<<<<<<< HEAD
	if (profile_cpu == -1)
		return;

<<<<<<< HEAD
	migrant = perf_session__findnew(session, migrate_task_event->pid);
=======
	migrant = machine__findnew_thread(machine, migrate_task_event->pid);
>>>>>>> refs/remotes/origin/cm-10.0
	atoms = thread_atoms_search(&atom_root, migrant, &cmp_pid);
	if (!atoms) {
		thread_atoms_insert(migrant);
		register_pid(migrant->pid, migrant->comm);
		atoms = thread_atoms_search(&atom_root, migrant, &cmp_pid);
		if (!atoms)
			die("migration-event: Internal tree error");
		add_sched_out_event(atoms, 'R', timestamp);
=======
	if (sched->profile_cpu == -1)
		return 0;

	migrant = machine__findnew_thread(machine, 0, pid);
	atoms = thread_atoms_search(&sched->atom_root, migrant, &sched->cmp_pid);
	if (!atoms) {
		if (thread_atoms_insert(sched, migrant))
			return -1;
		register_pid(sched, migrant->tid, thread__comm_str(migrant));
		atoms = thread_atoms_search(&sched->atom_root, migrant, &sched->cmp_pid);
		if (!atoms) {
			pr_err("migration-event: Internal tree error");
			return -1;
		}
		if (add_sched_out_event(atoms, 'R', timestamp))
			return -1;
>>>>>>> refs/remotes/origin/master
	}

	BUG_ON(list_empty(&atoms->work_list));

	atom = list_entry(atoms->work_list.prev, struct work_atom, list);
	atom->sched_in_time = atom->sched_out_time = atom->wake_up_time = timestamp;

<<<<<<< HEAD
	nr_timestamps++;

	if (atom->sched_out_time > timestamp)
		nr_unordered_timestamps++;
}

static struct trace_sched_handler lat_ops  = {
	.wakeup_event		= latency_wakeup_event,
	.switch_event		= latency_switch_event,
	.runtime_event		= latency_runtime_event,
	.fork_event		= latency_fork_event,
	.migrate_task_event	= latency_migrate_task_event,
};

static void output_lat_thread(struct work_atoms *work_list)
=======
	sched->nr_timestamps++;

	if (atom->sched_out_time > timestamp)
		sched->nr_unordered_timestamps++;

	return 0;
}

static void output_lat_thread(struct perf_sched *sched, struct work_atoms *work_list)
>>>>>>> refs/remotes/origin/master
{
	int i;
	int ret;
	u64 avg;

	if (!work_list->nb_atoms)
		return;
	/*
	 * Ignore idle threads:
	 */
<<<<<<< HEAD
	if (!strcmp(work_list->thread->comm, "swapper"))
		return;

	all_runtime += work_list->total_runtime;
	all_count += work_list->nb_atoms;

	ret = printf("  %s:%d ", work_list->thread->comm, work_list->thread->pid);
=======
	if (!strcmp(thread__comm_str(work_list->thread), "swapper"))
		return;

	sched->all_runtime += work_list->total_runtime;
	sched->all_count   += work_list->nb_atoms;

	ret = printf("  %s:%d ", thread__comm_str(work_list->thread), work_list->thread->tid);
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < 24 - ret; i++)
		printf(" ");

	avg = work_list->total_lat / work_list->nb_atoms;

	printf("|%11.3f ms |%9" PRIu64 " | avg:%9.3f ms | max:%9.3f ms | max at: %9.6f s\n",
	      (double)work_list->total_runtime / 1e6,
		 work_list->nb_atoms, (double)avg / 1e6,
		 (double)work_list->max_lat / 1e6,
		 (double)work_list->max_lat_at / 1e9);
}

static int pid_cmp(struct work_atoms *l, struct work_atoms *r)
{
<<<<<<< HEAD
	if (l->thread->pid < r->thread->pid)
		return -1;
	if (l->thread->pid > r->thread->pid)
=======
	if (l->thread->tid < r->thread->tid)
		return -1;
	if (l->thread->tid > r->thread->tid)
>>>>>>> refs/remotes/origin/master
		return 1;

	return 0;
}

<<<<<<< HEAD
static struct sort_dimension pid_sort_dimension = {
	.name			= "pid",
	.cmp			= pid_cmp,
};

=======
>>>>>>> refs/remotes/origin/master
static int avg_cmp(struct work_atoms *l, struct work_atoms *r)
{
	u64 avgl, avgr;

	if (!l->nb_atoms)
		return -1;

	if (!r->nb_atoms)
		return 1;

	avgl = l->total_lat / l->nb_atoms;
	avgr = r->total_lat / r->nb_atoms;

	if (avgl < avgr)
		return -1;
	if (avgl > avgr)
		return 1;

	return 0;
}

<<<<<<< HEAD
static struct sort_dimension avg_sort_dimension = {
	.name			= "avg",
	.cmp			= avg_cmp,
};

=======
>>>>>>> refs/remotes/origin/master
static int max_cmp(struct work_atoms *l, struct work_atoms *r)
{
	if (l->max_lat < r->max_lat)
		return -1;
	if (l->max_lat > r->max_lat)
		return 1;

	return 0;
}

<<<<<<< HEAD
static struct sort_dimension max_sort_dimension = {
	.name			= "max",
	.cmp			= max_cmp,
};

=======
>>>>>>> refs/remotes/origin/master
static int switch_cmp(struct work_atoms *l, struct work_atoms *r)
{
	if (l->nb_atoms < r->nb_atoms)
		return -1;
	if (l->nb_atoms > r->nb_atoms)
		return 1;

	return 0;
}

<<<<<<< HEAD
static struct sort_dimension switch_sort_dimension = {
	.name			= "switch",
	.cmp			= switch_cmp,
};

=======
>>>>>>> refs/remotes/origin/master
static int runtime_cmp(struct work_atoms *l, struct work_atoms *r)
{
	if (l->total_runtime < r->total_runtime)
		return -1;
	if (l->total_runtime > r->total_runtime)
		return 1;

	return 0;
}

<<<<<<< HEAD
static struct sort_dimension runtime_sort_dimension = {
	.name			= "runtime",
	.cmp			= runtime_cmp,
};

static struct sort_dimension *available_sorts[] = {
	&pid_sort_dimension,
	&avg_sort_dimension,
	&max_sort_dimension,
	&switch_sort_dimension,
	&runtime_sort_dimension,
};

#define NB_AVAILABLE_SORTS	(int)(sizeof(available_sorts) / sizeof(struct sort_dimension *))

static LIST_HEAD(sort_list);

static int sort_dimension__add(const char *tok, struct list_head *list)
{
	int i;

	for (i = 0; i < NB_AVAILABLE_SORTS; i++) {
=======
static int sort_dimension__add(const char *tok, struct list_head *list)
{
	size_t i;
	static struct sort_dimension avg_sort_dimension = {
		.name = "avg",
		.cmp  = avg_cmp,
	};
	static struct sort_dimension max_sort_dimension = {
		.name = "max",
		.cmp  = max_cmp,
	};
	static struct sort_dimension pid_sort_dimension = {
		.name = "pid",
		.cmp  = pid_cmp,
	};
	static struct sort_dimension runtime_sort_dimension = {
		.name = "runtime",
		.cmp  = runtime_cmp,
	};
	static struct sort_dimension switch_sort_dimension = {
		.name = "switch",
		.cmp  = switch_cmp,
	};
	struct sort_dimension *available_sorts[] = {
		&pid_sort_dimension,
		&avg_sort_dimension,
		&max_sort_dimension,
		&switch_sort_dimension,
		&runtime_sort_dimension,
	};

	for (i = 0; i < ARRAY_SIZE(available_sorts); i++) {
>>>>>>> refs/remotes/origin/master
		if (!strcmp(available_sorts[i]->name, tok)) {
			list_add_tail(&available_sorts[i]->list, list);

			return 0;
		}
	}

	return -1;
}

<<<<<<< HEAD
static void setup_sorting(void);

static void sort_lat(void)
=======
static void perf_sched__sort_lat(struct perf_sched *sched)
>>>>>>> refs/remotes/origin/master
{
	struct rb_node *node;

	for (;;) {
		struct work_atoms *data;
<<<<<<< HEAD
		node = rb_first(&atom_root);
		if (!node)
			break;

		rb_erase(node, &atom_root);
		data = rb_entry(node, struct work_atoms, node);
		__thread_latency_insert(&sorted_atom_root, data, &sort_list);
	}
}

static struct trace_sched_handler *trace_handler;

static void
<<<<<<< HEAD
process_sched_wakeup_event(void *data, struct perf_session *session,
			   struct event *event,
			   int cpu __used,
			   u64 timestamp __used,
			   struct thread *thread __used)
{
=======
process_sched_wakeup_event(struct perf_tool *tool __used,
			   struct event *event,
			   struct perf_sample *sample,
			   struct machine *machine,
			   struct thread *thread)
{
	void *data = sample->raw_data;
>>>>>>> refs/remotes/origin/cm-10.0
	struct trace_wakeup_event wakeup_event;

	FILL_COMMON_FIELDS(wakeup_event, event, data);

	FILL_ARRAY(wakeup_event, comm, event, data);
	FILL_FIELD(wakeup_event, pid, event, data);
	FILL_FIELD(wakeup_event, prio, event, data);
	FILL_FIELD(wakeup_event, success, event, data);
	FILL_FIELD(wakeup_event, cpu, event, data);

	if (trace_handler->wakeup_event)
<<<<<<< HEAD
		trace_handler->wakeup_event(&wakeup_event, session, event,
					    cpu, timestamp, thread);
=======
		trace_handler->wakeup_event(&wakeup_event, machine, event,
					    sample->cpu, sample->time, thread);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Track the current task - that way we can know whether there's any
 * weird events, such as a task being switched away that is not current.
 */
static int max_cpu;

static u32 curr_pid[MAX_CPUS] = { [0 ... MAX_CPUS-1] = -1 };

static struct thread *curr_thread[MAX_CPUS];

static char next_shortname1 = 'A';
static char next_shortname2 = '0';

static void
map_switch_event(struct trace_switch_event *switch_event,
<<<<<<< HEAD
		 struct perf_session *session,
=======
		 struct machine *machine,
>>>>>>> refs/remotes/origin/cm-10.0
		 struct event *event __used,
		 int this_cpu,
		 u64 timestamp,
		 struct thread *thread __used)
{
	struct thread *sched_out __used, *sched_in;
	int new_shortname;
	u64 timestamp0;
	s64 delta;
	int cpu;

	BUG_ON(this_cpu >= MAX_CPUS || this_cpu < 0);

	if (this_cpu > max_cpu)
		max_cpu = this_cpu;

	timestamp0 = cpu_last_switched[this_cpu];
	cpu_last_switched[this_cpu] = timestamp;
=======
		node = rb_first(&sched->atom_root);
		if (!node)
			break;

		rb_erase(node, &sched->atom_root);
		data = rb_entry(node, struct work_atoms, node);
		__thread_latency_insert(&sched->sorted_atom_root, data, &sched->sort_list);
	}
}

static int process_sched_wakeup_event(struct perf_tool *tool,
				      struct perf_evsel *evsel,
				      struct perf_sample *sample,
				      struct machine *machine)
{
	struct perf_sched *sched = container_of(tool, struct perf_sched, tool);

	if (sched->tp_handler->wakeup_event)
		return sched->tp_handler->wakeup_event(sched, evsel, sample, machine);

	return 0;
}

static int map_switch_event(struct perf_sched *sched, struct perf_evsel *evsel,
			    struct perf_sample *sample, struct machine *machine)
{
	const u32 prev_pid = perf_evsel__intval(evsel, sample, "prev_pid"),
		  next_pid = perf_evsel__intval(evsel, sample, "next_pid");
	struct thread *sched_out __maybe_unused, *sched_in;
	int new_shortname;
	u64 timestamp0, timestamp = sample->time;
	s64 delta;
	int cpu, this_cpu = sample->cpu;

	BUG_ON(this_cpu >= MAX_CPUS || this_cpu < 0);

	if (this_cpu > sched->max_cpu)
		sched->max_cpu = this_cpu;

	timestamp0 = sched->cpu_last_switched[this_cpu];
	sched->cpu_last_switched[this_cpu] = timestamp;
>>>>>>> refs/remotes/origin/master
	if (timestamp0)
		delta = timestamp - timestamp0;
	else
		delta = 0;

<<<<<<< HEAD
	if (delta < 0)
		die("hm, delta: %" PRIu64 " < 0 ?\n", delta);


<<<<<<< HEAD
	sched_out = perf_session__findnew(session, switch_event->prev_pid);
	sched_in = perf_session__findnew(session, switch_event->next_pid);
=======
	sched_out = machine__findnew_thread(machine, switch_event->prev_pid);
	sched_in = machine__findnew_thread(machine, switch_event->next_pid);
>>>>>>> refs/remotes/origin/cm-10.0

	curr_thread[this_cpu] = sched_in;
=======
	if (delta < 0) {
		pr_err("hm, delta: %" PRIu64 " < 0 ?\n", delta);
		return -1;
	}

	sched_out = machine__findnew_thread(machine, 0, prev_pid);
	sched_in = machine__findnew_thread(machine, 0, next_pid);

	sched->curr_thread[this_cpu] = sched_in;
>>>>>>> refs/remotes/origin/master

	printf("  ");

	new_shortname = 0;
	if (!sched_in->shortname[0]) {
<<<<<<< HEAD
		sched_in->shortname[0] = next_shortname1;
		sched_in->shortname[1] = next_shortname2;

		if (next_shortname1 < 'Z') {
			next_shortname1++;
		} else {
			next_shortname1='A';
			if (next_shortname2 < '9') {
				next_shortname2++;
			} else {
				next_shortname2='0';
=======
		sched_in->shortname[0] = sched->next_shortname1;
		sched_in->shortname[1] = sched->next_shortname2;

		if (sched->next_shortname1 < 'Z') {
			sched->next_shortname1++;
		} else {
			sched->next_shortname1='A';
			if (sched->next_shortname2 < '9') {
				sched->next_shortname2++;
			} else {
				sched->next_shortname2='0';
>>>>>>> refs/remotes/origin/master
			}
		}
		new_shortname = 1;
	}

<<<<<<< HEAD
	for (cpu = 0; cpu <= max_cpu; cpu++) {
=======
	for (cpu = 0; cpu <= sched->max_cpu; cpu++) {
>>>>>>> refs/remotes/origin/master
		if (cpu != this_cpu)
			printf(" ");
		else
			printf("*");

<<<<<<< HEAD
		if (curr_thread[cpu]) {
			if (curr_thread[cpu]->pid)
				printf("%2s ", curr_thread[cpu]->shortname);
=======
		if (sched->curr_thread[cpu]) {
			if (sched->curr_thread[cpu]->tid)
				printf("%2s ", sched->curr_thread[cpu]->shortname);
>>>>>>> refs/remotes/origin/master
			else
				printf(".  ");
		} else
			printf("   ");
	}

	printf("  %12.6f secs ", (double)timestamp/1e9);
	if (new_shortname) {
		printf("%s => %s:%d\n",
<<<<<<< HEAD
			sched_in->shortname, sched_in->comm, sched_in->pid);
	} else {
		printf("\n");
	}
}

<<<<<<< HEAD

static void
process_sched_switch_event(void *data, struct perf_session *session,
			   struct event *event,
			   int this_cpu,
			   u64 timestamp __used,
			   struct thread *thread __used)
{
=======
static void
process_sched_switch_event(struct perf_tool *tool __used,
			   struct event *event,
			   struct perf_sample *sample,
			   struct machine *machine,
			   struct thread *thread)
{
	int this_cpu = sample->cpu;
	void *data = sample->raw_data;
>>>>>>> refs/remotes/origin/cm-10.0
	struct trace_switch_event switch_event;

	FILL_COMMON_FIELDS(switch_event, event, data);

	FILL_ARRAY(switch_event, prev_comm, event, data);
	FILL_FIELD(switch_event, prev_pid, event, data);
	FILL_FIELD(switch_event, prev_prio, event, data);
	FILL_FIELD(switch_event, prev_state, event, data);
	FILL_ARRAY(switch_event, next_comm, event, data);
	FILL_FIELD(switch_event, next_pid, event, data);
	FILL_FIELD(switch_event, next_prio, event, data);

	if (curr_pid[this_cpu] != (u32)-1) {
=======
		       sched_in->shortname, thread__comm_str(sched_in), sched_in->tid);
	} else {
		printf("\n");
	}

	return 0;
}

static int process_sched_switch_event(struct perf_tool *tool,
				      struct perf_evsel *evsel,
				      struct perf_sample *sample,
				      struct machine *machine)
{
	struct perf_sched *sched = container_of(tool, struct perf_sched, tool);
	int this_cpu = sample->cpu, err = 0;
	u32 prev_pid = perf_evsel__intval(evsel, sample, "prev_pid"),
	    next_pid = perf_evsel__intval(evsel, sample, "next_pid");

	if (sched->curr_pid[this_cpu] != (u32)-1) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Are we trying to switch away a PID that is
		 * not current?
		 */
<<<<<<< HEAD
		if (curr_pid[this_cpu] != switch_event.prev_pid)
			nr_context_switch_bugs++;
	}
	if (trace_handler->switch_event)
<<<<<<< HEAD
		trace_handler->switch_event(&switch_event, session, event,
					    this_cpu, timestamp, thread);
=======
		trace_handler->switch_event(&switch_event, machine, event,
					    this_cpu, sample->time, thread);
>>>>>>> refs/remotes/origin/cm-10.0

	curr_pid[this_cpu] = switch_event.next_pid;
}

static void
<<<<<<< HEAD
process_sched_runtime_event(void *data, struct perf_session *session,
			   struct event *event,
			   int cpu __used,
			   u64 timestamp __used,
			   struct thread *thread __used)
{
=======
process_sched_runtime_event(struct perf_tool *tool __used,
			    struct event *event,
			    struct perf_sample *sample,
			    struct machine *machine,
			    struct thread *thread)
{
	void *data = sample->raw_data;
>>>>>>> refs/remotes/origin/cm-10.0
	struct trace_runtime_event runtime_event;

	FILL_ARRAY(runtime_event, comm, event, data);
	FILL_FIELD(runtime_event, pid, event, data);
	FILL_FIELD(runtime_event, runtime, event, data);
	FILL_FIELD(runtime_event, vruntime, event, data);

	if (trace_handler->runtime_event)
<<<<<<< HEAD
		trace_handler->runtime_event(&runtime_event, session, event, cpu, timestamp, thread);
}

static void
process_sched_fork_event(void *data,
			 struct event *event,
			 int cpu __used,
			 u64 timestamp __used,
			 struct thread *thread __used)
{
=======
		trace_handler->runtime_event(&runtime_event, machine, event,
					     sample->cpu, sample->time, thread);
}

static void
process_sched_fork_event(struct perf_tool *tool __used,
			 struct event *event,
			 struct perf_sample *sample,
			 struct machine *machine __used,
			 struct thread *thread)
{
	void *data = sample->raw_data;
>>>>>>> refs/remotes/origin/cm-10.0
	struct trace_fork_event fork_event;

	FILL_COMMON_FIELDS(fork_event, event, data);

	FILL_ARRAY(fork_event, parent_comm, event, data);
	FILL_FIELD(fork_event, parent_pid, event, data);
	FILL_ARRAY(fork_event, child_comm, event, data);
	FILL_FIELD(fork_event, child_pid, event, data);

	if (trace_handler->fork_event)
		trace_handler->fork_event(&fork_event, event,
<<<<<<< HEAD
					  cpu, timestamp, thread);
}

static void
process_sched_exit_event(struct event *event,
			 int cpu __used,
			 u64 timestamp __used,
=======
					  sample->cpu, sample->time, thread);
}

static void
process_sched_exit_event(struct perf_tool *tool __used,
			 struct event *event,
			 struct perf_sample *sample __used,
			 struct machine *machine __used,
>>>>>>> refs/remotes/origin/cm-10.0
			 struct thread *thread __used)
{
	if (verbose)
		printf("sched_exit event %p\n", event);
}

static void
<<<<<<< HEAD
process_sched_migrate_task_event(void *data, struct perf_session *session,
			   struct event *event,
			   int cpu __used,
			   u64 timestamp __used,
			   struct thread *thread __used)
{
=======
process_sched_migrate_task_event(struct perf_tool *tool __used,
				 struct event *event,
				 struct perf_sample *sample,
				 struct machine *machine,
				 struct thread *thread)
{
	void *data = sample->raw_data;
>>>>>>> refs/remotes/origin/cm-10.0
	struct trace_migrate_task_event migrate_task_event;

	FILL_COMMON_FIELDS(migrate_task_event, event, data);

	FILL_ARRAY(migrate_task_event, comm, event, data);
	FILL_FIELD(migrate_task_event, pid, event, data);
	FILL_FIELD(migrate_task_event, prio, event, data);
	FILL_FIELD(migrate_task_event, cpu, event, data);

	if (trace_handler->migrate_task_event)
<<<<<<< HEAD
		trace_handler->migrate_task_event(&migrate_task_event, session,
						 event, cpu, timestamp, thread);
}

static void process_raw_event(union perf_event *raw_event __used,
			      struct perf_session *session, void *data, int cpu,
			      u64 timestamp, struct thread *thread)
{
	struct event *event;
	int type;


	type = trace_parse_common_type(data);
	event = trace_find_event(type);

	if (!strcmp(event->name, "sched_switch"))
		process_sched_switch_event(data, session, event, cpu, timestamp, thread);
	if (!strcmp(event->name, "sched_stat_runtime"))
		process_sched_runtime_event(data, session, event, cpu, timestamp, thread);
	if (!strcmp(event->name, "sched_wakeup"))
		process_sched_wakeup_event(data, session, event, cpu, timestamp, thread);
	if (!strcmp(event->name, "sched_wakeup_new"))
		process_sched_wakeup_event(data, session, event, cpu, timestamp, thread);
	if (!strcmp(event->name, "sched_process_fork"))
		process_sched_fork_event(data, event, cpu, timestamp, thread);
	if (!strcmp(event->name, "sched_process_exit"))
		process_sched_exit_event(event, cpu, timestamp, thread);
	if (!strcmp(event->name, "sched_migrate_task"))
		process_sched_migrate_task_event(data, session, event, cpu, timestamp, thread);
}

static int process_sample_event(union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel __used,
				struct perf_session *session)
{
	struct thread *thread;

	if (!(session->sample_type & PERF_SAMPLE_RAW))
		return 0;

	thread = perf_session__findnew(session, sample->pid);
	if (thread == NULL) {
		pr_debug("problem processing %d event, skipping it.\n",
			 event->header.type);
		return -1;
	}

	dump_printf(" ... thread: %s:%d\n", thread->comm, thread->pid);

	if (profile_cpu != -1 && profile_cpu != (int)sample->cpu)
		return 0;

	process_raw_event(event, session, sample->raw_data, sample->cpu,
			  sample->time, thread);
=======
		trace_handler->migrate_task_event(&migrate_task_event, machine,
						  event, sample->cpu,
						  sample->time, thread);
}

typedef void (*tracepoint_handler)(struct perf_tool *tool, struct event *event,
				   struct perf_sample *sample,
				   struct machine *machine,
				   struct thread *thread);

static int perf_sched__process_tracepoint_sample(struct perf_tool *tool,
						 union perf_event *event __used,
=======
		if (sched->curr_pid[this_cpu] != prev_pid)
			sched->nr_context_switch_bugs++;
	}

	if (sched->tp_handler->switch_event)
		err = sched->tp_handler->switch_event(sched, evsel, sample, machine);

	sched->curr_pid[this_cpu] = next_pid;
	return err;
}

static int process_sched_runtime_event(struct perf_tool *tool,
				       struct perf_evsel *evsel,
				       struct perf_sample *sample,
				       struct machine *machine)
{
	struct perf_sched *sched = container_of(tool, struct perf_sched, tool);

	if (sched->tp_handler->runtime_event)
		return sched->tp_handler->runtime_event(sched, evsel, sample, machine);

	return 0;
}

static int perf_sched__process_fork_event(struct perf_tool *tool,
					  union perf_event *event,
					  struct perf_sample *sample,
					  struct machine *machine)
{
	struct perf_sched *sched = container_of(tool, struct perf_sched, tool);

	/* run the fork event through the perf machineruy */
	perf_event__process_fork(tool, event, sample, machine);

	/* and then run additional processing needed for this command */
	if (sched->tp_handler->fork_event)
		return sched->tp_handler->fork_event(sched, event, machine);

	return 0;
}

static int process_sched_migrate_task_event(struct perf_tool *tool,
					    struct perf_evsel *evsel,
					    struct perf_sample *sample,
					    struct machine *machine)
{
	struct perf_sched *sched = container_of(tool, struct perf_sched, tool);

	if (sched->tp_handler->migrate_task_event)
		return sched->tp_handler->migrate_task_event(sched, evsel, sample, machine);

	return 0;
}

typedef int (*tracepoint_handler)(struct perf_tool *tool,
				  struct perf_evsel *evsel,
				  struct perf_sample *sample,
				  struct machine *machine);

static int perf_sched__process_tracepoint_sample(struct perf_tool *tool __maybe_unused,
						 union perf_event *event __maybe_unused,
>>>>>>> refs/remotes/origin/master
						 struct perf_sample *sample,
						 struct perf_evsel *evsel,
						 struct machine *machine)
{
<<<<<<< HEAD
	struct thread *thread = machine__findnew_thread(machine, sample->pid);

	if (thread == NULL) {
		pr_debug("problem processing %s event, skipping it.\n",
			 evsel->name);
		return -1;
	}
=======
	int err = 0;
>>>>>>> refs/remotes/origin/master

	evsel->hists.stats.total_period += sample->period;
	hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);

<<<<<<< HEAD
	if (evsel->handler.func != NULL) {
		tracepoint_handler f = evsel->handler.func;

		if (evsel->handler.data == NULL)
			evsel->handler.data = trace_find_event(evsel->attr.config);

		f(tool, evsel->handler.data, sample, machine, thread);
	}
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

<<<<<<< HEAD
static struct perf_event_ops event_ops = {
	.sample			= process_sample_event,
=======
static struct perf_tool perf_sched = {
	.sample			= perf_sched__process_tracepoint_sample,
>>>>>>> refs/remotes/origin/cm-10.0
	.comm			= perf_event__process_comm,
	.lost			= perf_event__process_lost,
	.fork			= perf_event__process_task,
	.ordered_samples	= true,
};

<<<<<<< HEAD
static int read_events(void)
{
	int err = -EINVAL;
	struct perf_session *session = perf_session__new(input_name, O_RDONLY,
							 0, false, &event_ops);
	if (session == NULL)
		return -ENOMEM;

	if (perf_session__has_traces(session, "record -R")) {
		err = perf_session__process_events(session, &event_ops);
=======
static void read_events(bool destroy, struct perf_session **psession)
{
	int err = -EINVAL;
=======
	if (evsel->handler != NULL) {
		tracepoint_handler f = evsel->handler;
		err = f(tool, evsel, sample, machine);
	}

	return err;
}

static int perf_sched__read_events(struct perf_sched *sched,
				   struct perf_session **psession)
{
>>>>>>> refs/remotes/origin/master
	const struct perf_evsel_str_handler handlers[] = {
		{ "sched:sched_switch",	      process_sched_switch_event, },
		{ "sched:sched_stat_runtime", process_sched_runtime_event, },
		{ "sched:sched_wakeup",	      process_sched_wakeup_event, },
		{ "sched:sched_wakeup_new",   process_sched_wakeup_event, },
<<<<<<< HEAD
		{ "sched:sched_process_fork", process_sched_fork_event, },
		{ "sched:sched_process_exit", process_sched_exit_event, },
		{ "sched:sched_migrate_task", process_sched_migrate_task_event, },
	};
	struct perf_session *session = perf_session__new(input_name, O_RDONLY,
							 0, false, &perf_sched);
	if (session == NULL)
		die("No Memory");

	err = perf_evlist__set_tracepoints_handlers_array(session->evlist, handlers);
	assert(err == 0);

	if (perf_session__has_traces(session, "record -R")) {
		err = perf_session__process_events(session, &perf_sched);
		if (err)
			die("Failed to process events, error %d", err);

>>>>>>> refs/remotes/origin/cm-10.0
		nr_events      = session->hists.stats.nr_events[0];
		nr_lost_events = session->hists.stats.total_lost;
		nr_lost_chunks = session->hists.stats.nr_events[PERF_RECORD_LOST];
	}

<<<<<<< HEAD
	perf_session__delete(session);
	return err;
=======
	if (destroy)
		perf_session__delete(session);

	if (psession)
		*psession = session;
>>>>>>> refs/remotes/origin/cm-10.0
}

static void print_bad_events(void)
{
	if (nr_unordered_timestamps && nr_timestamps) {
		printf("  INFO: %.3f%% unordered timestamps (%ld out of %ld)\n",
			(double)nr_unordered_timestamps/(double)nr_timestamps*100.0,
			nr_unordered_timestamps, nr_timestamps);
	}
	if (nr_lost_events && nr_events) {
		printf("  INFO: %.3f%% lost events (%ld out of %ld, in %ld chunks)\n",
			(double)nr_lost_events/(double)nr_events*100.0,
			nr_lost_events, nr_events, nr_lost_chunks);
	}
	if (nr_state_machine_bugs && nr_timestamps) {
		printf("  INFO: %.3f%% state machine bugs (%ld out of %ld)",
			(double)nr_state_machine_bugs/(double)nr_timestamps*100.0,
			nr_state_machine_bugs, nr_timestamps);
		if (nr_lost_events)
			printf(" (due to lost events?)");
		printf("\n");
	}
	if (nr_context_switch_bugs && nr_timestamps) {
		printf("  INFO: %.3f%% context switch bugs (%ld out of %ld)",
			(double)nr_context_switch_bugs/(double)nr_timestamps*100.0,
			nr_context_switch_bugs, nr_timestamps);
		if (nr_lost_events)
=======
		{ "sched:sched_migrate_task", process_sched_migrate_task_event, },
	};
	struct perf_session *session;
	struct perf_data_file file = {
		.path = input_name,
		.mode = PERF_DATA_MODE_READ,
	};

	session = perf_session__new(&file, false, &sched->tool);
	if (session == NULL) {
		pr_debug("No Memory for session\n");
		return -1;
	}

	if (perf_session__set_tracepoints_handlers(session, handlers))
		goto out_delete;

	if (perf_session__has_traces(session, "record -R")) {
		int err = perf_session__process_events(session, &sched->tool);
		if (err) {
			pr_err("Failed to process events, error %d", err);
			goto out_delete;
		}

		sched->nr_events      = session->stats.nr_events[0];
		sched->nr_lost_events = session->stats.total_lost;
		sched->nr_lost_chunks = session->stats.nr_events[PERF_RECORD_LOST];
	}

	if (psession)
		*psession = session;
	else
		perf_session__delete(session);

	return 0;

out_delete:
	perf_session__delete(session);
	return -1;
}

static void print_bad_events(struct perf_sched *sched)
{
	if (sched->nr_unordered_timestamps && sched->nr_timestamps) {
		printf("  INFO: %.3f%% unordered timestamps (%ld out of %ld)\n",
			(double)sched->nr_unordered_timestamps/(double)sched->nr_timestamps*100.0,
			sched->nr_unordered_timestamps, sched->nr_timestamps);
	}
	if (sched->nr_lost_events && sched->nr_events) {
		printf("  INFO: %.3f%% lost events (%ld out of %ld, in %ld chunks)\n",
			(double)sched->nr_lost_events/(double)sched->nr_events * 100.0,
			sched->nr_lost_events, sched->nr_events, sched->nr_lost_chunks);
	}
	if (sched->nr_state_machine_bugs && sched->nr_timestamps) {
		printf("  INFO: %.3f%% state machine bugs (%ld out of %ld)",
			(double)sched->nr_state_machine_bugs/(double)sched->nr_timestamps*100.0,
			sched->nr_state_machine_bugs, sched->nr_timestamps);
		if (sched->nr_lost_events)
			printf(" (due to lost events?)");
		printf("\n");
	}
	if (sched->nr_context_switch_bugs && sched->nr_timestamps) {
		printf("  INFO: %.3f%% context switch bugs (%ld out of %ld)",
			(double)sched->nr_context_switch_bugs/(double)sched->nr_timestamps*100.0,
			sched->nr_context_switch_bugs, sched->nr_timestamps);
		if (sched->nr_lost_events)
>>>>>>> refs/remotes/origin/master
			printf(" (due to lost events?)");
		printf("\n");
	}
}

<<<<<<< HEAD
static void __cmd_lat(void)
{
	struct rb_node *next;
<<<<<<< HEAD

	setup_pager();
	read_events();
=======
	struct perf_session *session;

	setup_pager();
	read_events(false, &session);
>>>>>>> refs/remotes/origin/cm-10.0
	sort_lat();
=======
static int perf_sched__lat(struct perf_sched *sched)
{
	struct rb_node *next;
	struct perf_session *session;

	setup_pager();

	/* save session -- references to threads are held in work_list */
	if (perf_sched__read_events(sched, &session))
		return -1;

	perf_sched__sort_lat(sched);
>>>>>>> refs/remotes/origin/master

	printf("\n ---------------------------------------------------------------------------------------------------------------\n");
	printf("  Task                  |   Runtime ms  | Switches | Average delay ms | Maximum delay ms | Maximum delay at     |\n");
	printf(" ---------------------------------------------------------------------------------------------------------------\n");

<<<<<<< HEAD
	next = rb_first(&sorted_atom_root);
=======
	next = rb_first(&sched->sorted_atom_root);
>>>>>>> refs/remotes/origin/master

	while (next) {
		struct work_atoms *work_list;

		work_list = rb_entry(next, struct work_atoms, node);
<<<<<<< HEAD
		output_lat_thread(work_list);
=======
		output_lat_thread(sched, work_list);
>>>>>>> refs/remotes/origin/master
		next = rb_next(next);
	}

	printf(" -----------------------------------------------------------------------------------------\n");
	printf("  TOTAL:                |%11.3f ms |%9" PRIu64 " |\n",
<<<<<<< HEAD
		(double)all_runtime/1e6, all_count);

	printf(" ---------------------------------------------------\n");

	print_bad_events();
	printf("\n");

<<<<<<< HEAD
=======
	perf_session__delete(session);
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct trace_sched_handler map_ops  = {
	.wakeup_event		= NULL,
	.switch_event		= map_switch_event,
	.runtime_event		= NULL,
	.fork_event		= NULL,
};

static void __cmd_map(void)
{
	max_cpu = sysconf(_SC_NPROCESSORS_CONF);

	setup_pager();
<<<<<<< HEAD
	read_events();
=======
	read_events(true, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
	print_bad_events();
}

static void __cmd_replay(void)
{
	unsigned long i;

	calibrate_run_measurement_overhead();
	calibrate_sleep_measurement_overhead();

	test_calibrations();

<<<<<<< HEAD
	read_events();
=======
	read_events(true, NULL);
>>>>>>> refs/remotes/origin/cm-10.0

	printf("nr_run_events:        %ld\n", nr_run_events);
	printf("nr_sleep_events:      %ld\n", nr_sleep_events);
	printf("nr_wakeup_events:     %ld\n", nr_wakeup_events);

	if (targetless_wakeups)
		printf("target-less wakeups:  %ld\n", targetless_wakeups);
	if (multitarget_wakeups)
		printf("multi-target wakeups: %ld\n", multitarget_wakeups);
	if (nr_run_events_optimized)
		printf("run atoms optimized: %ld\n",
			nr_run_events_optimized);

	print_task_traces();
	add_cross_task_wakeups();

	create_tasks();
	printf("------------------------------------------------------------\n");
	for (i = 0; i < replay_repeat; i++)
		run_one_test();
}


static const char * const sched_usage[] = {
<<<<<<< HEAD
	"perf sched [<options>] {record|latency|map|replay|trace}",
=======
	"perf sched [<options>] {record|latency|map|replay|script}",
>>>>>>> refs/remotes/origin/cm-10.0
	NULL
};

static const struct option sched_options[] = {
	OPT_STRING('i', "input", &input_name, "file",
		    "input file name"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_END()
};

static const char * const latency_usage[] = {
	"perf sched latency [<options>]",
	NULL
};

static const struct option latency_options[] = {
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
		   "sort by key(s): runtime, switch, avg, max"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_INTEGER('C', "CPU", &profile_cpu,
		    "CPU to profile on"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_END()
};

static const char * const replay_usage[] = {
	"perf sched replay [<options>]",
	NULL
};

static const struct option replay_options[] = {
	OPT_UINTEGER('r', "repeat", &replay_repeat,
		     "repeat the workload replay N times (-1: infinite)"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_END()
};

static void setup_sorting(void)
{
	char *tmp, *tok, *str = strdup(sort_order);

	for (tok = strtok_r(str, ", ", &tmp);
			tok; tok = strtok_r(NULL, ", ", &tmp)) {
		if (sort_dimension__add(tok, &sort_list) < 0) {
			error("Unknown --sort key: `%s'", tok);
			usage_with_options(latency_usage, latency_options);
=======
		(double)sched->all_runtime / 1e6, sched->all_count);

	printf(" ---------------------------------------------------\n");

	print_bad_events(sched);
	printf("\n");

	perf_session__delete(session);
	return 0;
}

static int perf_sched__map(struct perf_sched *sched)
{
	sched->max_cpu = sysconf(_SC_NPROCESSORS_CONF);

	setup_pager();
	if (perf_sched__read_events(sched, NULL))
		return -1;
	print_bad_events(sched);
	return 0;
}

static int perf_sched__replay(struct perf_sched *sched)
{
	unsigned long i;

	calibrate_run_measurement_overhead(sched);
	calibrate_sleep_measurement_overhead(sched);

	test_calibrations(sched);

	if (perf_sched__read_events(sched, NULL))
		return -1;

	printf("nr_run_events:        %ld\n", sched->nr_run_events);
	printf("nr_sleep_events:      %ld\n", sched->nr_sleep_events);
	printf("nr_wakeup_events:     %ld\n", sched->nr_wakeup_events);

	if (sched->targetless_wakeups)
		printf("target-less wakeups:  %ld\n", sched->targetless_wakeups);
	if (sched->multitarget_wakeups)
		printf("multi-target wakeups: %ld\n", sched->multitarget_wakeups);
	if (sched->nr_run_events_optimized)
		printf("run atoms optimized: %ld\n",
			sched->nr_run_events_optimized);

	print_task_traces(sched);
	add_cross_task_wakeups(sched);

	create_tasks(sched);
	printf("------------------------------------------------------------\n");
	for (i = 0; i < sched->replay_repeat; i++)
		run_one_test(sched);

	return 0;
}

static void setup_sorting(struct perf_sched *sched, const struct option *options,
			  const char * const usage_msg[])
{
	char *tmp, *tok, *str = strdup(sched->sort_order);

	for (tok = strtok_r(str, ", ", &tmp);
			tok; tok = strtok_r(NULL, ", ", &tmp)) {
		if (sort_dimension__add(tok, &sched->sort_list) < 0) {
			error("Unknown --sort key: `%s'", tok);
			usage_with_options(usage_msg, options);
>>>>>>> refs/remotes/origin/master
		}
	}

	free(str);

<<<<<<< HEAD
	sort_dimension__add("pid", &cmp_pid);
}

static const char *record_args[] = {
	"record",
	"-a",
	"-R",
	"-f",
	"-m", "1024",
	"-c", "1",
	"-e", "sched:sched_switch",
	"-e", "sched:sched_stat_wait",
	"-e", "sched:sched_stat_sleep",
	"-e", "sched:sched_stat_iowait",
	"-e", "sched:sched_stat_runtime",
	"-e", "sched:sched_process_exit",
	"-e", "sched:sched_process_fork",
	"-e", "sched:sched_wakeup",
	"-e", "sched:sched_migrate_task",
};

=======
	sort_dimension__add("pid", &sched->cmp_pid);
}

>>>>>>> refs/remotes/origin/master
static int __cmd_record(int argc, const char **argv)
{
	unsigned int rec_argc, i, j;
	const char **rec_argv;
<<<<<<< HEAD
=======
	const char * const record_args[] = {
		"record",
		"-a",
		"-R",
		"-m", "1024",
		"-c", "1",
		"-e", "sched:sched_switch",
		"-e", "sched:sched_stat_wait",
		"-e", "sched:sched_stat_sleep",
		"-e", "sched:sched_stat_iowait",
		"-e", "sched:sched_stat_runtime",
		"-e", "sched:sched_process_fork",
		"-e", "sched:sched_wakeup",
		"-e", "sched:sched_migrate_task",
	};
>>>>>>> refs/remotes/origin/master

	rec_argc = ARRAY_SIZE(record_args) + argc - 1;
	rec_argv = calloc(rec_argc + 1, sizeof(char *));

	if (rec_argv == NULL)
		return -ENOMEM;

	for (i = 0; i < ARRAY_SIZE(record_args); i++)
		rec_argv[i] = strdup(record_args[i]);

	for (j = 1; j < (unsigned int)argc; j++, i++)
		rec_argv[i] = argv[j];

	BUG_ON(i != rec_argc);

	return cmd_record(i, rec_argv, NULL);
}

<<<<<<< HEAD
int cmd_sched(int argc, const char **argv, const char *prefix __used)
{
=======
int cmd_sched(int argc, const char **argv, const char *prefix __maybe_unused)
{
	const char default_sort_order[] = "avg, max, switch, runtime";
	struct perf_sched sched = {
		.tool = {
			.sample		 = perf_sched__process_tracepoint_sample,
			.comm		 = perf_event__process_comm,
			.lost		 = perf_event__process_lost,
			.fork		 = perf_sched__process_fork_event,
			.ordered_samples = true,
		},
		.cmp_pid	      = LIST_HEAD_INIT(sched.cmp_pid),
		.sort_list	      = LIST_HEAD_INIT(sched.sort_list),
		.start_work_mutex     = PTHREAD_MUTEX_INITIALIZER,
		.work_done_wait_mutex = PTHREAD_MUTEX_INITIALIZER,
		.sort_order	      = default_sort_order,
		.replay_repeat	      = 10,
		.profile_cpu	      = -1,
		.next_shortname1      = 'A',
		.next_shortname2      = '0',
	};
	const struct option latency_options[] = {
	OPT_STRING('s', "sort", &sched.sort_order, "key[,key2...]",
		   "sort by key(s): runtime, switch, avg, max"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_INTEGER('C', "CPU", &sched.profile_cpu,
		    "CPU to profile on"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_END()
	};
	const struct option replay_options[] = {
	OPT_UINTEGER('r', "repeat", &sched.replay_repeat,
		     "repeat the workload replay N times (-1: infinite)"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_END()
	};
	const struct option sched_options[] = {
	OPT_STRING('i', "input", &input_name, "file",
		    "input file name"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_END()
	};
	const char * const latency_usage[] = {
		"perf sched latency [<options>]",
		NULL
	};
	const char * const replay_usage[] = {
		"perf sched replay [<options>]",
		NULL
	};
	const char * const sched_usage[] = {
		"perf sched [<options>] {record|latency|map|replay|script}",
		NULL
	};
	struct trace_sched_handler lat_ops  = {
		.wakeup_event	    = latency_wakeup_event,
		.switch_event	    = latency_switch_event,
		.runtime_event	    = latency_runtime_event,
		.migrate_task_event = latency_migrate_task_event,
	};
	struct trace_sched_handler map_ops  = {
		.switch_event	    = map_switch_event,
	};
	struct trace_sched_handler replay_ops  = {
		.wakeup_event	    = replay_wakeup_event,
		.switch_event	    = replay_switch_event,
		.fork_event	    = replay_fork_event,
	};
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(sched.curr_pid); i++)
		sched.curr_pid[i] = -1;

>>>>>>> refs/remotes/origin/master
	argc = parse_options(argc, argv, sched_options, sched_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	if (!argc)
		usage_with_options(sched_usage, sched_options);

	/*
	 * Aliased to 'perf script' for now:
	 */
	if (!strcmp(argv[0], "script"))
		return cmd_script(argc, argv, prefix);

	symbol__init();
	if (!strncmp(argv[0], "rec", 3)) {
		return __cmd_record(argc, argv);
	} else if (!strncmp(argv[0], "lat", 3)) {
<<<<<<< HEAD
		trace_handler = &lat_ops;
=======
		sched.tp_handler = &lat_ops;
>>>>>>> refs/remotes/origin/master
		if (argc > 1) {
			argc = parse_options(argc, argv, latency_options, latency_usage, 0);
			if (argc)
				usage_with_options(latency_usage, latency_options);
		}
<<<<<<< HEAD
		setup_sorting();
		__cmd_lat();
	} else if (!strcmp(argv[0], "map")) {
		trace_handler = &map_ops;
		setup_sorting();
		__cmd_map();
	} else if (!strncmp(argv[0], "rep", 3)) {
		trace_handler = &replay_ops;
=======
		setup_sorting(&sched, latency_options, latency_usage);
		return perf_sched__lat(&sched);
	} else if (!strcmp(argv[0], "map")) {
		sched.tp_handler = &map_ops;
		setup_sorting(&sched, latency_options, latency_usage);
		return perf_sched__map(&sched);
	} else if (!strncmp(argv[0], "rep", 3)) {
		sched.tp_handler = &replay_ops;
>>>>>>> refs/remotes/origin/master
		if (argc) {
			argc = parse_options(argc, argv, replay_options, replay_usage, 0);
			if (argc)
				usage_with_options(replay_usage, replay_options);
		}
<<<<<<< HEAD
		__cmd_replay();
=======
		return perf_sched__replay(&sched);
>>>>>>> refs/remotes/origin/master
	} else {
		usage_with_options(sched_usage, sched_options);
	}

	return 0;
}
