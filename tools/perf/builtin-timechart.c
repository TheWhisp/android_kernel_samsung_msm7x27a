/*
 * builtin-timechart.c - make an svg timechart of system activity
 *
 * (C) Copyright 2009 Intel Corporation
 *
 * Authors:
 *     Arjan van de Ven <arjan@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

<<<<<<< HEAD
=======
#include <traceevent/event-parse.h>

>>>>>>> refs/remotes/origin/master
#include "builtin.h"

#include "util/util.h"

#include "util/color.h"
#include <linux/list.h>
#include "util/cache.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "util/evsel.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "util/evlist.h"
#include "util/evsel.h"
>>>>>>> refs/remotes/origin/master
#include <linux/rbtree.h>
#include "util/symbol.h"
#include "util/callchain.h"
#include "util/strlist.h"

#include "perf.h"
#include "util/header.h"
#include "util/parse-options.h"
#include "util/parse-events.h"
#include "util/event.h"
#include "util/session.h"
#include "util/svghelper.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "util/tool.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "util/tool.h"
#include "util/data.h"
>>>>>>> refs/remotes/origin/master

#define SUPPORT_OLD_POWER_EVENTS 1
#define PWR_EVENT_EXIT -1

<<<<<<< HEAD

<<<<<<< HEAD
static char		const *input_name = "perf.data";
static char		const *output_name = "output.svg";
=======
static const char	*input_name;
static const char	*output_name = "output.svg";
>>>>>>> refs/remotes/origin/cm-10.0

static unsigned int	numcpus;
static u64		min_freq;	/* Lowest CPU frequency seen */
static u64		max_freq;	/* Highest CPU frequency seen */
static u64		turbo_frequency;

static u64		first_time, last_time;

static bool		power_only;


struct per_pid;
struct per_pidcomm;

struct cpu_sample;
struct power_event;
struct wake_event;

struct sample_wrapper;
=======
struct per_pid;
struct power_event;
struct wake_event;

struct timechart {
	struct perf_tool	tool;
	struct per_pid		*all_data;
	struct power_event	*power_events;
	struct wake_event	*wake_events;
	int			proc_num;
	unsigned int		numcpus;
	u64			min_freq,	/* Lowest CPU frequency seen */
				max_freq,	/* Highest CPU frequency seen */
				turbo_frequency,
				first_time, last_time;
	bool			power_only,
				tasks_only,
				with_backtrace,
				topology;
};

struct per_pidcomm;
struct cpu_sample;
>>>>>>> refs/remotes/origin/master

/*
 * Datastructure layout:
 * We keep an list of "pid"s, matching the kernels notion of a task struct.
 * Each "pid" entry, has a list of "comm"s.
 *	this is because we want to track different programs different, while
 *	exec will reuse the original pid (by design).
 * Each comm has a list of samples that will be used to draw
 * final graph.
 */

struct per_pid {
	struct per_pid *next;

	int		pid;
	int		ppid;

	u64		start_time;
	u64		end_time;
	u64		total_time;
	int		display;

	struct per_pidcomm *all;
	struct per_pidcomm *current;
};


struct per_pidcomm {
	struct per_pidcomm *next;

	u64		start_time;
	u64		end_time;
	u64		total_time;

	int		Y;
	int		display;

	long		state;
	u64		state_since;

	char		*comm;

	struct cpu_sample *samples;
};

struct sample_wrapper {
	struct sample_wrapper *next;

	u64		timestamp;
	unsigned char	data[0];
};

#define TYPE_NONE	0
#define TYPE_RUNNING	1
#define TYPE_WAITING	2
#define TYPE_BLOCKED	3

struct cpu_sample {
	struct cpu_sample *next;

	u64 start_time;
	u64 end_time;
	int type;
	int cpu;
<<<<<<< HEAD
};

static struct per_pid *all_data;

=======
	const char *backtrace;
};

>>>>>>> refs/remotes/origin/master
#define CSTATE 1
#define PSTATE 2

struct power_event {
	struct power_event *next;
	int type;
	int state;
	u64 start_time;
	u64 end_time;
	int cpu;
};

struct wake_event {
	struct wake_event *next;
	int waker;
	int wakee;
	u64 time;
<<<<<<< HEAD
};

static struct power_event    *power_events;
static struct wake_event     *wake_events;

struct process_filter;
=======
	const char *backtrace;
};

>>>>>>> refs/remotes/origin/master
struct process_filter {
	char			*name;
	int			pid;
	struct process_filter	*next;
};

static struct process_filter *process_filter;


<<<<<<< HEAD
static struct per_pid *find_create_pid(int pid)
{
	struct per_pid *cursor = all_data;
=======
static struct per_pid *find_create_pid(struct timechart *tchart, int pid)
{
	struct per_pid *cursor = tchart->all_data;
>>>>>>> refs/remotes/origin/master

	while (cursor) {
		if (cursor->pid == pid)
			return cursor;
		cursor = cursor->next;
	}
<<<<<<< HEAD
	cursor = malloc(sizeof(struct per_pid));
	assert(cursor != NULL);
	memset(cursor, 0, sizeof(struct per_pid));
	cursor->pid = pid;
	cursor->next = all_data;
	all_data = cursor;
	return cursor;
}

static void pid_set_comm(int pid, char *comm)
{
	struct per_pid *p;
	struct per_pidcomm *c;
	p = find_create_pid(pid);
=======
	cursor = zalloc(sizeof(*cursor));
	assert(cursor != NULL);
	cursor->pid = pid;
	cursor->next = tchart->all_data;
	tchart->all_data = cursor;
	return cursor;
}

static void pid_set_comm(struct timechart *tchart, int pid, char *comm)
{
	struct per_pid *p;
	struct per_pidcomm *c;
	p = find_create_pid(tchart, pid);
>>>>>>> refs/remotes/origin/master
	c = p->all;
	while (c) {
		if (c->comm && strcmp(c->comm, comm) == 0) {
			p->current = c;
			return;
		}
		if (!c->comm) {
			c->comm = strdup(comm);
			p->current = c;
			return;
		}
		c = c->next;
	}
<<<<<<< HEAD
	c = malloc(sizeof(struct per_pidcomm));
	assert(c != NULL);
	memset(c, 0, sizeof(struct per_pidcomm));
=======
	c = zalloc(sizeof(*c));
	assert(c != NULL);
>>>>>>> refs/remotes/origin/master
	c->comm = strdup(comm);
	p->current = c;
	c->next = p->all;
	p->all = c;
}

<<<<<<< HEAD
static void pid_fork(int pid, int ppid, u64 timestamp)
{
	struct per_pid *p, *pp;
	p = find_create_pid(pid);
	pp = find_create_pid(ppid);
	p->ppid = ppid;
	if (pp->current && pp->current->comm && !p->current)
		pid_set_comm(pid, pp->current->comm);
=======
static void pid_fork(struct timechart *tchart, int pid, int ppid, u64 timestamp)
{
	struct per_pid *p, *pp;
	p = find_create_pid(tchart, pid);
	pp = find_create_pid(tchart, ppid);
	p->ppid = ppid;
	if (pp->current && pp->current->comm && !p->current)
		pid_set_comm(tchart, pid, pp->current->comm);
>>>>>>> refs/remotes/origin/master

	p->start_time = timestamp;
	if (p->current) {
		p->current->start_time = timestamp;
		p->current->state_since = timestamp;
	}
}

<<<<<<< HEAD
static void pid_exit(int pid, u64 timestamp)
{
	struct per_pid *p;
	p = find_create_pid(pid);
=======
static void pid_exit(struct timechart *tchart, int pid, u64 timestamp)
{
	struct per_pid *p;
	p = find_create_pid(tchart, pid);
>>>>>>> refs/remotes/origin/master
	p->end_time = timestamp;
	if (p->current)
		p->current->end_time = timestamp;
}

<<<<<<< HEAD
static void
pid_put_sample(int pid, int type, unsigned int cpu, u64 start, u64 end)
=======
static void pid_put_sample(struct timechart *tchart, int pid, int type,
			   unsigned int cpu, u64 start, u64 end,
			   const char *backtrace)
>>>>>>> refs/remotes/origin/master
{
	struct per_pid *p;
	struct per_pidcomm *c;
	struct cpu_sample *sample;

<<<<<<< HEAD
	p = find_create_pid(pid);
	c = p->current;
	if (!c) {
		c = malloc(sizeof(struct per_pidcomm));
		assert(c != NULL);
		memset(c, 0, sizeof(struct per_pidcomm));
=======
	p = find_create_pid(tchart, pid);
	c = p->current;
	if (!c) {
		c = zalloc(sizeof(*c));
		assert(c != NULL);
>>>>>>> refs/remotes/origin/master
		p->current = c;
		c->next = p->all;
		p->all = c;
	}

<<<<<<< HEAD
	sample = malloc(sizeof(struct cpu_sample));
	assert(sample != NULL);
	memset(sample, 0, sizeof(struct cpu_sample));
=======
	sample = zalloc(sizeof(*sample));
	assert(sample != NULL);
>>>>>>> refs/remotes/origin/master
	sample->start_time = start;
	sample->end_time = end;
	sample->type = type;
	sample->next = c->samples;
	sample->cpu = cpu;
<<<<<<< HEAD
=======
	sample->backtrace = backtrace;
>>>>>>> refs/remotes/origin/master
	c->samples = sample;

	if (sample->type == TYPE_RUNNING && end > start && start > 0) {
		c->total_time += (end-start);
		p->total_time += (end-start);
	}

	if (c->start_time == 0 || c->start_time > start)
		c->start_time = start;
	if (p->start_time == 0 || p->start_time > start)
		p->start_time = start;
}

#define MAX_CPUS 4096

static u64 cpus_cstate_start_times[MAX_CPUS];
static int cpus_cstate_state[MAX_CPUS];
static u64 cpus_pstate_start_times[MAX_CPUS];
static u64 cpus_pstate_state[MAX_CPUS];

<<<<<<< HEAD
<<<<<<< HEAD
static int process_comm_event(union perf_event *event,
			      struct perf_sample *sample __used,
			      struct perf_session *session __used)
=======
static int process_comm_event(struct perf_tool *tool __used,
			      union perf_event *event,
			      struct perf_sample *sample __used,
			      struct machine *machine __used)
>>>>>>> refs/remotes/origin/cm-10.0
{
	pid_set_comm(event->comm.tid, event->comm.comm);
	return 0;
}

<<<<<<< HEAD
static int process_fork_event(union perf_event *event,
			      struct perf_sample *sample __used,
			      struct perf_session *session __used)
=======
static int process_fork_event(struct perf_tool *tool __used,
			      union perf_event *event,
			      struct perf_sample *sample __used,
			      struct machine *machine __used)
>>>>>>> refs/remotes/origin/cm-10.0
{
	pid_fork(event->fork.pid, event->fork.ppid, event->fork.time);
	return 0;
}

<<<<<<< HEAD
static int process_exit_event(union perf_event *event,
			      struct perf_sample *sample __used,
			      struct perf_session *session __used)
=======
static int process_exit_event(struct perf_tool *tool __used,
			      union perf_event *event,
			      struct perf_sample *sample __used,
			      struct machine *machine __used)
>>>>>>> refs/remotes/origin/cm-10.0
{
	pid_exit(event->fork.pid, event->fork.time);
	return 0;
}

struct trace_entry {
	unsigned short		type;
	unsigned char		flags;
	unsigned char		preempt_count;
	int			pid;
	int			lock_depth;
};

#ifdef SUPPORT_OLD_POWER_EVENTS
static int use_old_power_events;
struct power_entry_old {
	struct trace_entry te;
	u64	type;
	u64	value;
	u64	cpu_id;
};
#endif

struct power_processor_entry {
	struct trace_entry te;
	u32	state;
	u32	cpu_id;
};

#define TASK_COMM_LEN 16
struct wakeup_entry {
	struct trace_entry te;
	char comm[TASK_COMM_LEN];
	int   pid;
	int   prio;
	int   success;
};

/*
 * trace_flag_type is an enumeration that holds different
 * states when a trace occurs. These are:
 *  IRQS_OFF            - interrupts were disabled
 *  IRQS_NOSUPPORT      - arch does not support irqs_disabled_flags
 *  NEED_RESCED         - reschedule is requested
 *  HARDIRQ             - inside an interrupt handler
 *  SOFTIRQ             - inside a softirq handler
 */
enum trace_flag_type {
	TRACE_FLAG_IRQS_OFF		= 0x01,
	TRACE_FLAG_IRQS_NOSUPPORT	= 0x02,
	TRACE_FLAG_NEED_RESCHED		= 0x04,
	TRACE_FLAG_HARDIRQ		= 0x08,
	TRACE_FLAG_SOFTIRQ		= 0x10,
};



struct sched_switch {
	struct trace_entry te;
	char prev_comm[TASK_COMM_LEN];
	int  prev_pid;
	int  prev_prio;
	long prev_state; /* Arjan weeps. */
	char next_comm[TASK_COMM_LEN];
	int  next_pid;
	int  next_prio;
};

=======
static int process_comm_event(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
{
	struct timechart *tchart = container_of(tool, struct timechart, tool);
	pid_set_comm(tchart, event->comm.tid, event->comm.comm);
	return 0;
}

static int process_fork_event(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
{
	struct timechart *tchart = container_of(tool, struct timechart, tool);
	pid_fork(tchart, event->fork.pid, event->fork.ppid, event->fork.time);
	return 0;
}

static int process_exit_event(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
{
	struct timechart *tchart = container_of(tool, struct timechart, tool);
	pid_exit(tchart, event->fork.pid, event->fork.time);
	return 0;
}

#ifdef SUPPORT_OLD_POWER_EVENTS
static int use_old_power_events;
#endif

>>>>>>> refs/remotes/origin/master
static void c_state_start(int cpu, u64 timestamp, int state)
{
	cpus_cstate_start_times[cpu] = timestamp;
	cpus_cstate_state[cpu] = state;
}

<<<<<<< HEAD
static void c_state_end(int cpu, u64 timestamp)
{
	struct power_event *pwr;
	pwr = malloc(sizeof(struct power_event));
	if (!pwr)
		return;
	memset(pwr, 0, sizeof(struct power_event));
=======
static void c_state_end(struct timechart *tchart, int cpu, u64 timestamp)
{
	struct power_event *pwr = zalloc(sizeof(*pwr));

	if (!pwr)
		return;
>>>>>>> refs/remotes/origin/master

	pwr->state = cpus_cstate_state[cpu];
	pwr->start_time = cpus_cstate_start_times[cpu];
	pwr->end_time = timestamp;
	pwr->cpu = cpu;
	pwr->type = CSTATE;
<<<<<<< HEAD
	pwr->next = power_events;

	power_events = pwr;
}

static void p_state_change(int cpu, u64 timestamp, u64 new_freq)
{
	struct power_event *pwr;
	pwr = malloc(sizeof(struct power_event));
=======
	pwr->next = tchart->power_events;

	tchart->power_events = pwr;
}

static void p_state_change(struct timechart *tchart, int cpu, u64 timestamp, u64 new_freq)
{
	struct power_event *pwr;
>>>>>>> refs/remotes/origin/master

	if (new_freq > 8000000) /* detect invalid data */
		return;

<<<<<<< HEAD
	if (!pwr)
		return;
	memset(pwr, 0, sizeof(struct power_event));
=======
	pwr = zalloc(sizeof(*pwr));
	if (!pwr)
		return;
>>>>>>> refs/remotes/origin/master

	pwr->state = cpus_pstate_state[cpu];
	pwr->start_time = cpus_pstate_start_times[cpu];
	pwr->end_time = timestamp;
	pwr->cpu = cpu;
	pwr->type = PSTATE;
<<<<<<< HEAD
	pwr->next = power_events;

	if (!pwr->start_time)
		pwr->start_time = first_time;

	power_events = pwr;
=======
	pwr->next = tchart->power_events;

	if (!pwr->start_time)
		pwr->start_time = tchart->first_time;

	tchart->power_events = pwr;
>>>>>>> refs/remotes/origin/master

	cpus_pstate_state[cpu] = new_freq;
	cpus_pstate_start_times[cpu] = timestamp;

<<<<<<< HEAD
	if ((u64)new_freq > max_freq)
		max_freq = new_freq;

	if (new_freq < min_freq || min_freq == 0)
		min_freq = new_freq;

	if (new_freq == max_freq - 1000)
			turbo_frequency = max_freq;
}

static void
sched_wakeup(int cpu, u64 timestamp, int pid, struct trace_entry *te)
{
	struct wake_event *we;
	struct per_pid *p;
	struct wakeup_entry *wake = (void *)te;

	we = malloc(sizeof(struct wake_event));
	if (!we)
		return;

	memset(we, 0, sizeof(struct wake_event));
	we->time = timestamp;
	we->waker = pid;

	if ((te->flags & TRACE_FLAG_HARDIRQ) || (te->flags & TRACE_FLAG_SOFTIRQ))
		we->waker = -1;

	we->wakee = wake->pid;
	we->next = wake_events;
	wake_events = we;
	p = find_create_pid(we->wakee);
=======
	if ((u64)new_freq > tchart->max_freq)
		tchart->max_freq = new_freq;

	if (new_freq < tchart->min_freq || tchart->min_freq == 0)
		tchart->min_freq = new_freq;

	if (new_freq == tchart->max_freq - 1000)
		tchart->turbo_frequency = tchart->max_freq;
}

static void sched_wakeup(struct timechart *tchart, int cpu, u64 timestamp,
			 int waker, int wakee, u8 flags, const char *backtrace)
{
	struct per_pid *p;
	struct wake_event *we = zalloc(sizeof(*we));

	if (!we)
		return;

	we->time = timestamp;
	we->waker = waker;
	we->backtrace = backtrace;

	if ((flags & TRACE_FLAG_HARDIRQ) || (flags & TRACE_FLAG_SOFTIRQ))
		we->waker = -1;

	we->wakee = wakee;
	we->next = tchart->wake_events;
	tchart->wake_events = we;
	p = find_create_pid(tchart, we->wakee);
>>>>>>> refs/remotes/origin/master

	if (p && p->current && p->current->state == TYPE_NONE) {
		p->current->state_since = timestamp;
		p->current->state = TYPE_WAITING;
	}
	if (p && p->current && p->current->state == TYPE_BLOCKED) {
<<<<<<< HEAD
		pid_put_sample(p->pid, p->current->state, cpu, p->current->state_since, timestamp);
=======
		pid_put_sample(tchart, p->pid, p->current->state, cpu,
			       p->current->state_since, timestamp, NULL);
>>>>>>> refs/remotes/origin/master
		p->current->state_since = timestamp;
		p->current->state = TYPE_WAITING;
	}
}

<<<<<<< HEAD
static void sched_switch(int cpu, u64 timestamp, struct trace_entry *te)
{
	struct per_pid *p = NULL, *prev_p;
	struct sched_switch *sw = (void *)te;


	prev_p = find_create_pid(sw->prev_pid);

	p = find_create_pid(sw->next_pid);

	if (prev_p->current && prev_p->current->state != TYPE_NONE)
		pid_put_sample(sw->prev_pid, TYPE_RUNNING, cpu, prev_p->current->state_since, timestamp);
	if (p && p->current) {
		if (p->current->state != TYPE_NONE)
			pid_put_sample(sw->next_pid, p->current->state, cpu, p->current->state_since, timestamp);
=======
static void sched_switch(struct timechart *tchart, int cpu, u64 timestamp,
			 int prev_pid, int next_pid, u64 prev_state,
			 const char *backtrace)
{
	struct per_pid *p = NULL, *prev_p;

	prev_p = find_create_pid(tchart, prev_pid);

	p = find_create_pid(tchart, next_pid);

	if (prev_p->current && prev_p->current->state != TYPE_NONE)
		pid_put_sample(tchart, prev_pid, TYPE_RUNNING, cpu,
			       prev_p->current->state_since, timestamp,
			       backtrace);
	if (p && p->current) {
		if (p->current->state != TYPE_NONE)
			pid_put_sample(tchart, next_pid, p->current->state, cpu,
				       p->current->state_since, timestamp,
				       backtrace);
>>>>>>> refs/remotes/origin/master

		p->current->state_since = timestamp;
		p->current->state = TYPE_RUNNING;
	}

	if (prev_p->current) {
		prev_p->current->state = TYPE_NONE;
		prev_p->current->state_since = timestamp;
<<<<<<< HEAD
		if (sw->prev_state & 2)
			prev_p->current->state = TYPE_BLOCKED;
		if (sw->prev_state == 0)
=======
		if (prev_state & 2)
			prev_p->current->state = TYPE_BLOCKED;
		if (prev_state == 0)
>>>>>>> refs/remotes/origin/master
			prev_p->current->state = TYPE_WAITING;
	}
}

<<<<<<< HEAD

<<<<<<< HEAD
static int process_sample_event(union perf_event *event __used,
				struct perf_sample *sample,
				struct perf_evsel *evsel __used,
				struct perf_session *session)
{
	struct trace_entry *te;

	if (session->sample_type & PERF_SAMPLE_TIME) {
=======
static int process_sample_event(struct perf_tool *tool __used,
				union perf_event *event __used,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct machine *machine __used)
{
	struct trace_entry *te;

	if (evsel->attr.sample_type & PERF_SAMPLE_TIME) {
>>>>>>> refs/remotes/origin/cm-10.0
		if (!first_time || first_time > sample->time)
			first_time = sample->time;
		if (last_time < sample->time)
			last_time = sample->time;
	}

	te = (void *)sample->raw_data;
<<<<<<< HEAD
	if (session->sample_type & PERF_SAMPLE_RAW && sample->raw_size > 0) {
=======
	if ((evsel->attr.sample_type & PERF_SAMPLE_RAW) && sample->raw_size > 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		char *event_str;
#ifdef SUPPORT_OLD_POWER_EVENTS
		struct power_entry_old *peo;
		peo = (void *)te;
#endif
		/*
		 * FIXME: use evsel, its already mapped from id to perf_evsel,
		 * remove perf_header__find_event infrastructure bits.
		 * Mapping all these "power:cpu_idle" strings to the tracepoint
		 * ID and then just comparing against evsel->attr.config.
		 *
		 * e.g.:
		 *
		 * if (evsel->attr.config == power_cpu_idle_id)
		 */
		event_str = perf_header__find_event(te->type);

		if (!event_str)
			return 0;

		if (sample->cpu > numcpus)
			numcpus = sample->cpu;

		if (strcmp(event_str, "power:cpu_idle") == 0) {
			struct power_processor_entry *ppe = (void *)te;
			if (ppe->state == (u32)PWR_EVENT_EXIT)
				c_state_end(ppe->cpu_id, sample->time);
			else
				c_state_start(ppe->cpu_id, sample->time,
					      ppe->state);
		}
		else if (strcmp(event_str, "power:cpu_frequency") == 0) {
			struct power_processor_entry *ppe = (void *)te;
			p_state_change(ppe->cpu_id, sample->time, ppe->state);
		}

		else if (strcmp(event_str, "sched:sched_wakeup") == 0)
			sched_wakeup(sample->cpu, sample->time, sample->pid, te);

		else if (strcmp(event_str, "sched:sched_switch") == 0)
			sched_switch(sample->cpu, sample->time, te);

#ifdef SUPPORT_OLD_POWER_EVENTS
		if (use_old_power_events) {
			if (strcmp(event_str, "power:power_start") == 0)
				c_state_start(peo->cpu_id, sample->time,
					      peo->value);

			else if (strcmp(event_str, "power:power_end") == 0)
				c_state_end(sample->cpu, sample->time);

			else if (strcmp(event_str,
					"power:power_frequency") == 0)
				p_state_change(peo->cpu_id, sample->time,
					       peo->value);
		}
#endif
	}
	return 0;
}
=======
static const char *cat_backtrace(union perf_event *event,
				 struct perf_sample *sample,
				 struct machine *machine)
{
	struct addr_location al;
	unsigned int i;
	char *p = NULL;
	size_t p_len;
	u8 cpumode = PERF_RECORD_MISC_USER;
	struct addr_location tal;
	struct ip_callchain *chain = sample->callchain;
	FILE *f = open_memstream(&p, &p_len);

	if (!f) {
		perror("open_memstream error");
		return NULL;
	}

	if (!chain)
		goto exit;

	if (perf_event__preprocess_sample(event, machine, &al, sample) < 0) {
		fprintf(stderr, "problem processing %d event, skipping it.\n",
			event->header.type);
		goto exit;
	}

	for (i = 0; i < chain->nr; i++) {
		u64 ip;

		if (callchain_param.order == ORDER_CALLEE)
			ip = chain->ips[i];
		else
			ip = chain->ips[chain->nr - i - 1];

		if (ip >= PERF_CONTEXT_MAX) {
			switch (ip) {
			case PERF_CONTEXT_HV:
				cpumode = PERF_RECORD_MISC_HYPERVISOR;
				break;
			case PERF_CONTEXT_KERNEL:
				cpumode = PERF_RECORD_MISC_KERNEL;
				break;
			case PERF_CONTEXT_USER:
				cpumode = PERF_RECORD_MISC_USER;
				break;
			default:
				pr_debug("invalid callchain context: "
					 "%"PRId64"\n", (s64) ip);

				/*
				 * It seems the callchain is corrupted.
				 * Discard all.
				 */
				zfree(&p);
				goto exit;
			}
			continue;
		}

		tal.filtered = false;
		thread__find_addr_location(al.thread, machine, cpumode,
					   MAP__FUNCTION, ip, &tal);

		if (tal.sym)
			fprintf(f, "..... %016" PRIx64 " %s\n", ip,
				tal.sym->name);
		else
			fprintf(f, "..... %016" PRIx64 "\n", ip);
	}

exit:
	fclose(f);

	return p;
}

typedef int (*tracepoint_handler)(struct timechart *tchart,
				  struct perf_evsel *evsel,
				  struct perf_sample *sample,
				  const char *backtrace);

static int process_sample_event(struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct machine *machine)
{
	struct timechart *tchart = container_of(tool, struct timechart, tool);

	if (evsel->attr.sample_type & PERF_SAMPLE_TIME) {
		if (!tchart->first_time || tchart->first_time > sample->time)
			tchart->first_time = sample->time;
		if (tchart->last_time < sample->time)
			tchart->last_time = sample->time;
	}

	if (evsel->handler != NULL) {
		tracepoint_handler f = evsel->handler;
		return f(tchart, evsel, sample,
			 cat_backtrace(event, sample, machine));
	}

	return 0;
}

static int
process_sample_cpu_idle(struct timechart *tchart __maybe_unused,
			struct perf_evsel *evsel,
			struct perf_sample *sample,
			const char *backtrace __maybe_unused)
{
	u32 state = perf_evsel__intval(evsel, sample, "state");
	u32 cpu_id = perf_evsel__intval(evsel, sample, "cpu_id");

	if (state == (u32)PWR_EVENT_EXIT)
		c_state_end(tchart, cpu_id, sample->time);
	else
		c_state_start(cpu_id, sample->time, state);
	return 0;
}

static int
process_sample_cpu_frequency(struct timechart *tchart,
			     struct perf_evsel *evsel,
			     struct perf_sample *sample,
			     const char *backtrace __maybe_unused)
{
	u32 state = perf_evsel__intval(evsel, sample, "state");
	u32 cpu_id = perf_evsel__intval(evsel, sample, "cpu_id");

	p_state_change(tchart, cpu_id, sample->time, state);
	return 0;
}

static int
process_sample_sched_wakeup(struct timechart *tchart,
			    struct perf_evsel *evsel,
			    struct perf_sample *sample,
			    const char *backtrace)
{
	u8 flags = perf_evsel__intval(evsel, sample, "common_flags");
	int waker = perf_evsel__intval(evsel, sample, "common_pid");
	int wakee = perf_evsel__intval(evsel, sample, "pid");

	sched_wakeup(tchart, sample->cpu, sample->time, waker, wakee, flags, backtrace);
	return 0;
}

static int
process_sample_sched_switch(struct timechart *tchart,
			    struct perf_evsel *evsel,
			    struct perf_sample *sample,
			    const char *backtrace)
{
	int prev_pid = perf_evsel__intval(evsel, sample, "prev_pid");
	int next_pid = perf_evsel__intval(evsel, sample, "next_pid");
	u64 prev_state = perf_evsel__intval(evsel, sample, "prev_state");

	sched_switch(tchart, sample->cpu, sample->time, prev_pid, next_pid,
		     prev_state, backtrace);
	return 0;
}

#ifdef SUPPORT_OLD_POWER_EVENTS
static int
process_sample_power_start(struct timechart *tchart __maybe_unused,
			   struct perf_evsel *evsel,
			   struct perf_sample *sample,
			   const char *backtrace __maybe_unused)
{
	u64 cpu_id = perf_evsel__intval(evsel, sample, "cpu_id");
	u64 value = perf_evsel__intval(evsel, sample, "value");

	c_state_start(cpu_id, sample->time, value);
	return 0;
}

static int
process_sample_power_end(struct timechart *tchart,
			 struct perf_evsel *evsel __maybe_unused,
			 struct perf_sample *sample,
			 const char *backtrace __maybe_unused)
{
	c_state_end(tchart, sample->cpu, sample->time);
	return 0;
}

static int
process_sample_power_frequency(struct timechart *tchart,
			       struct perf_evsel *evsel,
			       struct perf_sample *sample,
			       const char *backtrace __maybe_unused)
{
	u64 cpu_id = perf_evsel__intval(evsel, sample, "cpu_id");
	u64 value = perf_evsel__intval(evsel, sample, "value");

	p_state_change(tchart, cpu_id, sample->time, value);
	return 0;
}
#endif /* SUPPORT_OLD_POWER_EVENTS */
>>>>>>> refs/remotes/origin/master

/*
 * After the last sample we need to wrap up the current C/P state
 * and close out each CPU for these.
 */
<<<<<<< HEAD
static void end_sample_processing(void)
=======
static void end_sample_processing(struct timechart *tchart)
>>>>>>> refs/remotes/origin/master
{
	u64 cpu;
	struct power_event *pwr;

<<<<<<< HEAD
	for (cpu = 0; cpu <= numcpus; cpu++) {
		pwr = malloc(sizeof(struct power_event));
		if (!pwr)
			return;
		memset(pwr, 0, sizeof(struct power_event));

		/* C state */
#if 0
		pwr->state = cpus_cstate_state[cpu];
		pwr->start_time = cpus_cstate_start_times[cpu];
		pwr->end_time = last_time;
		pwr->cpu = cpu;
		pwr->type = CSTATE;
		pwr->next = power_events;

		power_events = pwr;
#endif
		/* P state */

		pwr = malloc(sizeof(struct power_event));
		if (!pwr)
			return;
		memset(pwr, 0, sizeof(struct power_event));

		pwr->state = cpus_pstate_state[cpu];
		pwr->start_time = cpus_pstate_start_times[cpu];
		pwr->end_time = last_time;
		pwr->cpu = cpu;
		pwr->type = PSTATE;
		pwr->next = power_events;

		if (!pwr->start_time)
			pwr->start_time = first_time;
		if (!pwr->state)
			pwr->state = min_freq;
		power_events = pwr;
=======
	for (cpu = 0; cpu <= tchart->numcpus; cpu++) {
		/* C state */
#if 0
		pwr = zalloc(sizeof(*pwr));
		if (!pwr)
			return;

		pwr->state = cpus_cstate_state[cpu];
		pwr->start_time = cpus_cstate_start_times[cpu];
		pwr->end_time = tchart->last_time;
		pwr->cpu = cpu;
		pwr->type = CSTATE;
		pwr->next = tchart->power_events;

		tchart->power_events = pwr;
#endif
		/* P state */

		pwr = zalloc(sizeof(*pwr));
		if (!pwr)
			return;

		pwr->state = cpus_pstate_state[cpu];
		pwr->start_time = cpus_pstate_start_times[cpu];
		pwr->end_time = tchart->last_time;
		pwr->cpu = cpu;
		pwr->type = PSTATE;
		pwr->next = tchart->power_events;

		if (!pwr->start_time)
			pwr->start_time = tchart->first_time;
		if (!pwr->state)
			pwr->state = tchart->min_freq;
		tchart->power_events = pwr;
>>>>>>> refs/remotes/origin/master
	}
}

/*
 * Sort the pid datastructure
 */
<<<<<<< HEAD
static void sort_pids(void)
=======
static void sort_pids(struct timechart *tchart)
>>>>>>> refs/remotes/origin/master
{
	struct per_pid *new_list, *p, *cursor, *prev;
	/* sort by ppid first, then by pid, lowest to highest */

	new_list = NULL;

<<<<<<< HEAD
	while (all_data) {
		p = all_data;
		all_data = p->next;
=======
	while (tchart->all_data) {
		p = tchart->all_data;
		tchart->all_data = p->next;
>>>>>>> refs/remotes/origin/master
		p->next = NULL;

		if (new_list == NULL) {
			new_list = p;
			p->next = NULL;
			continue;
		}
		prev = NULL;
		cursor = new_list;
		while (cursor) {
			if (cursor->ppid > p->ppid ||
				(cursor->ppid == p->ppid && cursor->pid > p->pid)) {
				/* must insert before */
				if (prev) {
					p->next = prev->next;
					prev->next = p;
					cursor = NULL;
					continue;
				} else {
					p->next = new_list;
					new_list = p;
					cursor = NULL;
					continue;
				}
			}

			prev = cursor;
			cursor = cursor->next;
			if (!cursor)
				prev->next = p;
		}
	}
<<<<<<< HEAD
	all_data = new_list;
}


static void draw_c_p_states(void)
{
	struct power_event *pwr;
	pwr = power_events;
=======
	tchart->all_data = new_list;
}


static void draw_c_p_states(struct timechart *tchart)
{
	struct power_event *pwr;
	pwr = tchart->power_events;
>>>>>>> refs/remotes/origin/master

	/*
	 * two pass drawing so that the P state bars are on top of the C state blocks
	 */
	while (pwr) {
		if (pwr->type == CSTATE)
			svg_cstate(pwr->cpu, pwr->start_time, pwr->end_time, pwr->state);
		pwr = pwr->next;
	}

<<<<<<< HEAD
	pwr = power_events;
	while (pwr) {
		if (pwr->type == PSTATE) {
			if (!pwr->state)
				pwr->state = min_freq;
=======
	pwr = tchart->power_events;
	while (pwr) {
		if (pwr->type == PSTATE) {
			if (!pwr->state)
				pwr->state = tchart->min_freq;
>>>>>>> refs/remotes/origin/master
			svg_pstate(pwr->cpu, pwr->start_time, pwr->end_time, pwr->state);
		}
		pwr = pwr->next;
	}
}

<<<<<<< HEAD
static void draw_wakeups(void)
=======
static void draw_wakeups(struct timechart *tchart)
>>>>>>> refs/remotes/origin/master
{
	struct wake_event *we;
	struct per_pid *p;
	struct per_pidcomm *c;

<<<<<<< HEAD
	we = wake_events;
=======
	we = tchart->wake_events;
>>>>>>> refs/remotes/origin/master
	while (we) {
		int from = 0, to = 0;
		char *task_from = NULL, *task_to = NULL;

		/* locate the column of the waker and wakee */
<<<<<<< HEAD
		p = all_data;
=======
		p = tchart->all_data;
>>>>>>> refs/remotes/origin/master
		while (p) {
			if (p->pid == we->waker || p->pid == we->wakee) {
				c = p->all;
				while (c) {
					if (c->Y && c->start_time <= we->time && c->end_time >= we->time) {
						if (p->pid == we->waker && !from) {
							from = c->Y;
							task_from = strdup(c->comm);
						}
						if (p->pid == we->wakee && !to) {
							to = c->Y;
							task_to = strdup(c->comm);
						}
					}
					c = c->next;
				}
				c = p->all;
				while (c) {
					if (p->pid == we->waker && !from) {
						from = c->Y;
						task_from = strdup(c->comm);
					}
					if (p->pid == we->wakee && !to) {
						to = c->Y;
						task_to = strdup(c->comm);
					}
					c = c->next;
				}
			}
			p = p->next;
		}

		if (!task_from) {
			task_from = malloc(40);
			sprintf(task_from, "[%i]", we->waker);
		}
		if (!task_to) {
			task_to = malloc(40);
			sprintf(task_to, "[%i]", we->wakee);
		}

		if (we->waker == -1)
<<<<<<< HEAD
			svg_interrupt(we->time, to);
		else if (from && to && abs(from - to) == 1)
			svg_wakeline(we->time, from, to);
		else
			svg_partial_wakeline(we->time, from, task_from, to, task_to);
=======
			svg_interrupt(we->time, to, we->backtrace);
		else if (from && to && abs(from - to) == 1)
			svg_wakeline(we->time, from, to, we->backtrace);
		else
			svg_partial_wakeline(we->time, from, task_from, to,
					     task_to, we->backtrace);
>>>>>>> refs/remotes/origin/master
		we = we->next;

		free(task_from);
		free(task_to);
	}
}

<<<<<<< HEAD
static void draw_cpu_usage(void)
=======
static void draw_cpu_usage(struct timechart *tchart)
>>>>>>> refs/remotes/origin/master
{
	struct per_pid *p;
	struct per_pidcomm *c;
	struct cpu_sample *sample;
<<<<<<< HEAD
	p = all_data;
=======
	p = tchart->all_data;
>>>>>>> refs/remotes/origin/master
	while (p) {
		c = p->all;
		while (c) {
			sample = c->samples;
			while (sample) {
<<<<<<< HEAD
				if (sample->type == TYPE_RUNNING)
					svg_process(sample->cpu, sample->start_time, sample->end_time, "sample", c->comm);
=======
				if (sample->type == TYPE_RUNNING) {
					svg_process(sample->cpu,
						    sample->start_time,
						    sample->end_time,
						    p->pid,
						    c->comm,
						    sample->backtrace);
				}
>>>>>>> refs/remotes/origin/master

				sample = sample->next;
			}
			c = c->next;
		}
		p = p->next;
	}
}

<<<<<<< HEAD
static void draw_process_bars(void)
=======
static void draw_process_bars(struct timechart *tchart)
>>>>>>> refs/remotes/origin/master
{
	struct per_pid *p;
	struct per_pidcomm *c;
	struct cpu_sample *sample;
	int Y = 0;

<<<<<<< HEAD
	Y = 2 * numcpus + 2;

	p = all_data;
=======
	Y = 2 * tchart->numcpus + 2;

	p = tchart->all_data;
>>>>>>> refs/remotes/origin/master
	while (p) {
		c = p->all;
		while (c) {
			if (!c->display) {
				c->Y = 0;
				c = c->next;
				continue;
			}

			svg_box(Y, c->start_time, c->end_time, "process");
			sample = c->samples;
			while (sample) {
				if (sample->type == TYPE_RUNNING)
<<<<<<< HEAD
					svg_sample(Y, sample->cpu, sample->start_time, sample->end_time);
				if (sample->type == TYPE_BLOCKED)
					svg_box(Y, sample->start_time, sample->end_time, "blocked");
				if (sample->type == TYPE_WAITING)
					svg_waiting(Y, sample->start_time, sample->end_time);
=======
					svg_running(Y, sample->cpu,
						    sample->start_time,
						    sample->end_time,
						    sample->backtrace);
				if (sample->type == TYPE_BLOCKED)
					svg_blocked(Y, sample->cpu,
						    sample->start_time,
						    sample->end_time,
						    sample->backtrace);
				if (sample->type == TYPE_WAITING)
					svg_waiting(Y, sample->cpu,
						    sample->start_time,
						    sample->end_time,
						    sample->backtrace);
>>>>>>> refs/remotes/origin/master
				sample = sample->next;
			}

			if (c->comm) {
				char comm[256];
				if (c->total_time > 5000000000) /* 5 seconds */
					sprintf(comm, "%s:%i (%2.2fs)", c->comm, p->pid, c->total_time / 1000000000.0);
				else
					sprintf(comm, "%s:%i (%3.1fms)", c->comm, p->pid, c->total_time / 1000000.0);

				svg_text(Y, c->start_time, comm);
			}
			c->Y = Y;
			Y++;
			c = c->next;
		}
		p = p->next;
	}
}

static void add_process_filter(const char *string)
{
<<<<<<< HEAD
	struct process_filter *filt;
	int pid;

	pid = strtoull(string, NULL, 10);
	filt = malloc(sizeof(struct process_filter));
=======
	int pid = strtoull(string, NULL, 10);
	struct process_filter *filt = malloc(sizeof(*filt));

>>>>>>> refs/remotes/origin/master
	if (!filt)
		return;

	filt->name = strdup(string);
	filt->pid  = pid;
	filt->next = process_filter;

	process_filter = filt;
}

static int passes_filter(struct per_pid *p, struct per_pidcomm *c)
{
	struct process_filter *filt;
	if (!process_filter)
		return 1;

	filt = process_filter;
	while (filt) {
		if (filt->pid && p->pid == filt->pid)
			return 1;
		if (strcmp(filt->name, c->comm) == 0)
			return 1;
		filt = filt->next;
	}
	return 0;
}

<<<<<<< HEAD
static int determine_display_tasks_filtered(void)
=======
static int determine_display_tasks_filtered(struct timechart *tchart)
>>>>>>> refs/remotes/origin/master
{
	struct per_pid *p;
	struct per_pidcomm *c;
	int count = 0;

<<<<<<< HEAD
	p = all_data;
	while (p) {
		p->display = 0;
		if (p->start_time == 1)
			p->start_time = first_time;

		/* no exit marker, task kept running to the end */
		if (p->end_time == 0)
			p->end_time = last_time;
=======
	p = tchart->all_data;
	while (p) {
		p->display = 0;
		if (p->start_time == 1)
			p->start_time = tchart->first_time;

		/* no exit marker, task kept running to the end */
		if (p->end_time == 0)
			p->end_time = tchart->last_time;
>>>>>>> refs/remotes/origin/master

		c = p->all;

		while (c) {
			c->display = 0;

			if (c->start_time == 1)
<<<<<<< HEAD
				c->start_time = first_time;
=======
				c->start_time = tchart->first_time;
>>>>>>> refs/remotes/origin/master

			if (passes_filter(p, c)) {
				c->display = 1;
				p->display = 1;
				count++;
			}

			if (c->end_time == 0)
<<<<<<< HEAD
				c->end_time = last_time;
=======
				c->end_time = tchart->last_time;
>>>>>>> refs/remotes/origin/master

			c = c->next;
		}
		p = p->next;
	}
	return count;
}

<<<<<<< HEAD
static int determine_display_tasks(u64 threshold)
=======
static int determine_display_tasks(struct timechart *tchart, u64 threshold)
>>>>>>> refs/remotes/origin/master
{
	struct per_pid *p;
	struct per_pidcomm *c;
	int count = 0;

	if (process_filter)
<<<<<<< HEAD
		return determine_display_tasks_filtered();

	p = all_data;
	while (p) {
		p->display = 0;
		if (p->start_time == 1)
			p->start_time = first_time;

		/* no exit marker, task kept running to the end */
		if (p->end_time == 0)
			p->end_time = last_time;
		if (p->total_time >= threshold && !power_only)
=======
		return determine_display_tasks_filtered(tchart);

	p = tchart->all_data;
	while (p) {
		p->display = 0;
		if (p->start_time == 1)
			p->start_time = tchart->first_time;

		/* no exit marker, task kept running to the end */
		if (p->end_time == 0)
			p->end_time = tchart->last_time;
		if (p->total_time >= threshold)
>>>>>>> refs/remotes/origin/master
			p->display = 1;

		c = p->all;

		while (c) {
			c->display = 0;

			if (c->start_time == 1)
<<<<<<< HEAD
				c->start_time = first_time;

			if (c->total_time >= threshold && !power_only) {
=======
				c->start_time = tchart->first_time;

			if (c->total_time >= threshold) {
>>>>>>> refs/remotes/origin/master
				c->display = 1;
				count++;
			}

			if (c->end_time == 0)
<<<<<<< HEAD
				c->end_time = last_time;
=======
				c->end_time = tchart->last_time;
>>>>>>> refs/remotes/origin/master

			c = c->next;
		}
		p = p->next;
	}
	return count;
}



#define TIME_THRESH 10000000

<<<<<<< HEAD
static void write_svg_file(const char *filename)
{
	u64 i;
	int count;

	numcpus++;


	count = determine_display_tasks(TIME_THRESH);

	/* We'd like to show at least 15 tasks; be less picky if we have fewer */
	if (count < 15)
		count = determine_display_tasks(TIME_THRESH / 10);

	open_svg(filename, numcpus, count, first_time, last_time);
=======
static void write_svg_file(struct timechart *tchart, const char *filename)
{
	u64 i;
	int count;
	int thresh = TIME_THRESH;

	if (tchart->power_only)
		tchart->proc_num = 0;

	/* We'd like to show at least proc_num tasks;
	 * be less picky if we have fewer */
	do {
		count = determine_display_tasks(tchart, thresh);
		thresh /= 10;
	} while (!process_filter && thresh && count < tchart->proc_num);

	open_svg(filename, tchart->numcpus, count, tchart->first_time, tchart->last_time);
>>>>>>> refs/remotes/origin/master

	svg_time_grid();
	svg_legenda();

<<<<<<< HEAD
	for (i = 0; i < numcpus; i++)
		svg_cpu_box(i, max_freq, turbo_frequency);

	draw_cpu_usage();
	draw_process_bars();
	draw_c_p_states();
	draw_wakeups();
=======
	for (i = 0; i < tchart->numcpus; i++)
		svg_cpu_box(i, tchart->max_freq, tchart->turbo_frequency);

	draw_cpu_usage(tchart);
	if (tchart->proc_num)
		draw_process_bars(tchart);
	if (!tchart->tasks_only)
		draw_c_p_states(tchart);
	if (tchart->proc_num)
		draw_wakeups(tchart);
>>>>>>> refs/remotes/origin/master

	svg_close();
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct perf_event_ops event_ops = {
=======
static struct perf_tool perf_timechart = {
>>>>>>> refs/remotes/origin/cm-10.0
	.comm			= process_comm_event,
	.fork			= process_fork_event,
	.exit			= process_exit_event,
	.sample			= process_sample_event,
	.ordered_samples	= true,
};

static int __cmd_timechart(void)
{
	struct perf_session *session = perf_session__new(input_name, O_RDONLY,
<<<<<<< HEAD
							 0, false, &event_ops);
=======
							 0, false, &perf_timechart);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int process_header(struct perf_file_section *section __maybe_unused,
			  struct perf_header *ph,
			  int feat,
			  int fd __maybe_unused,
			  void *data)
{
	struct timechart *tchart = data;

	switch (feat) {
	case HEADER_NRCPUS:
		tchart->numcpus = ph->env.nr_cpus_avail;
		break;

	case HEADER_CPU_TOPOLOGY:
		if (!tchart->topology)
			break;

		if (svg_build_topology_map(ph->env.sibling_cores,
					   ph->env.nr_sibling_cores,
					   ph->env.sibling_threads,
					   ph->env.nr_sibling_threads))
			fprintf(stderr, "problem building topology\n");
		break;

	default:
		break;
	}

	return 0;
}

static int __cmd_timechart(struct timechart *tchart, const char *output_name)
{
	const struct perf_evsel_str_handler power_tracepoints[] = {
		{ "power:cpu_idle",		process_sample_cpu_idle },
		{ "power:cpu_frequency",	process_sample_cpu_frequency },
		{ "sched:sched_wakeup",		process_sample_sched_wakeup },
		{ "sched:sched_switch",		process_sample_sched_switch },
#ifdef SUPPORT_OLD_POWER_EVENTS
		{ "power:power_start",		process_sample_power_start },
		{ "power:power_end",		process_sample_power_end },
		{ "power:power_frequency",	process_sample_power_frequency },
#endif
	};
	struct perf_data_file file = {
		.path = input_name,
		.mode = PERF_DATA_MODE_READ,
	};

	struct perf_session *session = perf_session__new(&file, false,
							 &tchart->tool);
>>>>>>> refs/remotes/origin/master
	int ret = -EINVAL;

	if (session == NULL)
		return -ENOMEM;

<<<<<<< HEAD
	if (!perf_session__has_traces(session, "timechart record"))
		goto out_delete;

<<<<<<< HEAD
	ret = perf_session__process_events(session, &event_ops);
=======
	ret = perf_session__process_events(session, &perf_timechart);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		goto out_delete;

	end_sample_processing();

	sort_pids();

	write_svg_file(output_name);

	pr_info("Written %2.1f seconds of trace to %s.\n",
		(last_time - first_time) / 1000000000.0, output_name);
=======
	(void)perf_header__process_sections(&session->header,
					    perf_data_file__fd(session->file),
					    tchart,
					    process_header);

	if (!perf_session__has_traces(session, "timechart record"))
		goto out_delete;

	if (perf_session__set_tracepoints_handlers(session,
						   power_tracepoints)) {
		pr_err("Initializing session tracepoint handlers failed\n");
		goto out_delete;
	}

	ret = perf_session__process_events(session, &tchart->tool);
	if (ret)
		goto out_delete;

	end_sample_processing(tchart);

	sort_pids(tchart);

	write_svg_file(tchart, output_name);

	pr_info("Written %2.1f seconds of trace to %s.\n",
		(tchart->last_time - tchart->first_time) / 1000000000.0, output_name);
>>>>>>> refs/remotes/origin/master
out_delete:
	perf_session__delete(session);
	return ret;
}

<<<<<<< HEAD
static const char * const timechart_usage[] = {
	"perf timechart [<options>] {record}",
	NULL
};

#ifdef SUPPORT_OLD_POWER_EVENTS
static const char * const record_old_args[] = {
	"record",
	"-a",
	"-R",
	"-f",
	"-c", "1",
	"-e", "power:power_start",
	"-e", "power:power_end",
	"-e", "power:power_frequency",
	"-e", "sched:sched_wakeup",
	"-e", "sched:sched_switch",
};
#endif

static const char * const record_new_args[] = {
	"record",
	"-a",
	"-R",
	"-f",
	"-c", "1",
	"-e", "power:cpu_frequency",
	"-e", "power:cpu_idle",
	"-e", "sched:sched_wakeup",
	"-e", "sched:sched_switch",
};

static int __cmd_record(int argc, const char **argv)
{
	unsigned int rec_argc, i, j;
	const char **rec_argv;
	const char * const *record_args = record_new_args;
	unsigned int record_elems = ARRAY_SIZE(record_new_args);
=======
static int timechart__record(struct timechart *tchart, int argc, const char **argv)
{
	unsigned int rec_argc, i, j;
	const char **rec_argv;
	const char **p;
	unsigned int record_elems;

	const char * const common_args[] = {
		"record", "-a", "-R", "-c", "1",
	};
	unsigned int common_args_nr = ARRAY_SIZE(common_args);

	const char * const backtrace_args[] = {
		"-g",
	};
	unsigned int backtrace_args_no = ARRAY_SIZE(backtrace_args);

	const char * const power_args[] = {
		"-e", "power:cpu_frequency",
		"-e", "power:cpu_idle",
	};
	unsigned int power_args_nr = ARRAY_SIZE(power_args);

	const char * const old_power_args[] = {
#ifdef SUPPORT_OLD_POWER_EVENTS
		"-e", "power:power_start",
		"-e", "power:power_end",
		"-e", "power:power_frequency",
#endif
	};
	unsigned int old_power_args_nr = ARRAY_SIZE(old_power_args);

	const char * const tasks_args[] = {
		"-e", "sched:sched_wakeup",
		"-e", "sched:sched_switch",
	};
	unsigned int tasks_args_nr = ARRAY_SIZE(tasks_args);
>>>>>>> refs/remotes/origin/master

#ifdef SUPPORT_OLD_POWER_EVENTS
	if (!is_valid_tracepoint("power:cpu_idle") &&
	    is_valid_tracepoint("power:power_start")) {
		use_old_power_events = 1;
<<<<<<< HEAD
		record_args = record_old_args;
		record_elems = ARRAY_SIZE(record_old_args);
	}
#endif

	rec_argc = record_elems + argc - 1;
=======
		power_args_nr = 0;
	} else {
		old_power_args_nr = 0;
	}
#endif

	if (tchart->power_only)
		tasks_args_nr = 0;

	if (tchart->tasks_only) {
		power_args_nr = 0;
		old_power_args_nr = 0;
	}

	if (!tchart->with_backtrace)
		backtrace_args_no = 0;

	record_elems = common_args_nr + tasks_args_nr +
		power_args_nr + old_power_args_nr + backtrace_args_no;

	rec_argc = record_elems + argc;
>>>>>>> refs/remotes/origin/master
	rec_argv = calloc(rec_argc + 1, sizeof(char *));

	if (rec_argv == NULL)
		return -ENOMEM;

<<<<<<< HEAD
	for (i = 0; i < record_elems; i++)
		rec_argv[i] = strdup(record_args[i]);

	for (j = 1; j < (unsigned int)argc; j++, i++)
		rec_argv[i] = argv[j];

	return cmd_record(i, rec_argv, NULL);
}

static int
parse_process(const struct option *opt __used, const char *arg, int __used unset)
=======
	p = rec_argv;
	for (i = 0; i < common_args_nr; i++)
		*p++ = strdup(common_args[i]);

	for (i = 0; i < backtrace_args_no; i++)
		*p++ = strdup(backtrace_args[i]);

	for (i = 0; i < tasks_args_nr; i++)
		*p++ = strdup(tasks_args[i]);

	for (i = 0; i < power_args_nr; i++)
		*p++ = strdup(power_args[i]);

	for (i = 0; i < old_power_args_nr; i++)
		*p++ = strdup(old_power_args[i]);

	for (j = 1; j < (unsigned int)argc; j++)
		*p++ = argv[j];

	return cmd_record(rec_argc, rec_argv, NULL);
}

static int
parse_process(const struct option *opt __maybe_unused, const char *arg,
	      int __maybe_unused unset)
>>>>>>> refs/remotes/origin/master
{
	if (arg)
		add_process_filter(arg);
	return 0;
}

<<<<<<< HEAD
static const struct option options[] = {
	OPT_STRING('i', "input", &input_name, "file",
		    "input file name"),
	OPT_STRING('o', "output", &output_name, "file",
		    "output file name"),
	OPT_INTEGER('w', "width", &svg_page_width,
		    "page width"),
	OPT_BOOLEAN('P', "power-only", &power_only,
		    "output power data only"),
=======
static int
parse_highlight(const struct option *opt __maybe_unused, const char *arg,
		int __maybe_unused unset)
{
	unsigned long duration = strtoul(arg, NULL, 0);

	if (svg_highlight || svg_highlight_name)
		return -1;

	if (duration)
		svg_highlight = duration;
	else
		svg_highlight_name = strdup(arg);

	return 0;
}

int cmd_timechart(int argc, const char **argv,
		  const char *prefix __maybe_unused)
{
	struct timechart tchart = {
		.tool = {
			.comm		 = process_comm_event,
			.fork		 = process_fork_event,
			.exit		 = process_exit_event,
			.sample		 = process_sample_event,
			.ordered_samples = true,
		},
		.proc_num = 15,
	};
	const char *output_name = "output.svg";
	const struct option timechart_options[] = {
	OPT_STRING('i', "input", &input_name, "file", "input file name"),
	OPT_STRING('o', "output", &output_name, "file", "output file name"),
	OPT_INTEGER('w', "width", &svg_page_width, "page width"),
	OPT_CALLBACK(0, "highlight", NULL, "duration or task name",
		      "highlight tasks. Pass duration in ns or process name.",
		       parse_highlight),
	OPT_BOOLEAN('P', "power-only", &tchart.power_only, "output power data only"),
	OPT_BOOLEAN('T', "tasks-only", &tchart.tasks_only,
		    "output processes data only"),
>>>>>>> refs/remotes/origin/master
	OPT_CALLBACK('p', "process", NULL, "process",
		      "process selector. Pass a pid or process name.",
		       parse_process),
	OPT_STRING(0, "symfs", &symbol_conf.symfs, "directory",
		    "Look for files with symbols relative to this directory"),
<<<<<<< HEAD
	OPT_END()
};


int cmd_timechart(int argc, const char **argv, const char *prefix __used)
{
	argc = parse_options(argc, argv, options, timechart_usage,
			PARSE_OPT_STOP_AT_NON_OPTION);

	symbol__init();

	if (argc && !strncmp(argv[0], "rec", 3))
		return __cmd_record(argc, argv);
	else if (argc)
		usage_with_options(timechart_usage, options);

	setup_pager();

	return __cmd_timechart();
=======
	OPT_INTEGER('n', "proc-num", &tchart.proc_num,
		    "min. number of tasks to print"),
	OPT_BOOLEAN('t', "topology", &tchart.topology,
		    "sort CPUs according to topology"),
	OPT_END()
	};
	const char * const timechart_usage[] = {
		"perf timechart [<options>] {record}",
		NULL
	};

	const struct option record_options[] = {
	OPT_BOOLEAN('P', "power-only", &tchart.power_only, "output power data only"),
	OPT_BOOLEAN('T', "tasks-only", &tchart.tasks_only,
		    "output processes data only"),
	OPT_BOOLEAN('g', "callchain", &tchart.with_backtrace, "record callchain"),
	OPT_END()
	};
	const char * const record_usage[] = {
		"perf timechart record [<options>]",
		NULL
	};
	argc = parse_options(argc, argv, timechart_options, timechart_usage,
			PARSE_OPT_STOP_AT_NON_OPTION);

	if (tchart.power_only && tchart.tasks_only) {
		pr_err("-P and -T options cannot be used at the same time.\n");
		return -1;
	}

	symbol__init();

	if (argc && !strncmp(argv[0], "rec", 3)) {
		argc = parse_options(argc, argv, record_options, record_usage,
				     PARSE_OPT_STOP_AT_NON_OPTION);

		if (tchart.power_only && tchart.tasks_only) {
			pr_err("-P and -T options cannot be used at the same time.\n");
			return -1;
		}

		return timechart__record(&tchart, argc, argv);
	} else if (argc)
		usage_with_options(timechart_usage, timechart_options);

	setup_pager();

	return __cmd_timechart(&tchart, output_name);
>>>>>>> refs/remotes/origin/master
}
