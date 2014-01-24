/*
 * builtin-inject.c
 *
 * Builtin inject command: Examine the live mode (stdin) event stream
 * and repipe it to stdout while optionally injecting additional
 * events into it.
 */
#include "builtin.h"

#include "perf.h"
<<<<<<< HEAD
#include "util/session.h"
<<<<<<< HEAD
=======
#include "util/tool.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "util/debug.h"

#include "util/parse-options.h"

static char		const *input_name = "-";
static bool		inject_build_ids;

<<<<<<< HEAD
static int perf_event__repipe_synth(union perf_event *event,
				    struct perf_session *session __used)
=======
static int perf_event__repipe_synth(struct perf_tool *tool __used,
				    union perf_event *event,
				    struct machine *machine __used)
>>>>>>> refs/remotes/origin/cm-10.0
{
	uint32_t size;
	void *buf = event;

	size = event->header.size;

	while (size) {
		int ret = write(STDOUT_FILENO, buf, size);
		if (ret < 0)
			return -errno;

		size -= ret;
		buf += ret;
	}

	return 0;
}

<<<<<<< HEAD
static int perf_event__repipe(union perf_event *event,
			      struct perf_sample *sample __used,
			      struct perf_session *session)
{
	return perf_event__repipe_synth(event, session);
}

static int perf_event__repipe_sample(union perf_event *event,
			      struct perf_sample *sample __used,
			      struct perf_evsel *evsel __used,
			      struct perf_session *session)
{
	return perf_event__repipe_synth(event, session);
}

static int perf_event__repipe_mmap(union perf_event *event,
				   struct perf_sample *sample,
				   struct perf_session *session)
{
	int err;

	err = perf_event__process_mmap(event, sample, session);
	perf_event__repipe(event, sample, session);
=======
static int perf_event__repipe_op2_synth(struct perf_tool *tool,
					union perf_event *event,
					struct perf_session *session __used)
{
	return perf_event__repipe_synth(tool, event, NULL);
}

static int perf_event__repipe_event_type_synth(struct perf_tool *tool,
					       union perf_event *event)
{
	return perf_event__repipe_synth(tool, event, NULL);
}

static int perf_event__repipe_tracing_data_synth(union perf_event *event,
						 struct perf_session *session __used)
{
	return perf_event__repipe_synth(NULL, event, NULL);
}

static int perf_event__repipe_attr(union perf_event *event,
				   struct perf_evlist **pevlist __used)
{
	return perf_event__repipe_synth(NULL, event, NULL);
=======
#include "util/color.h"
#include "util/evlist.h"
#include "util/evsel.h"
#include "util/session.h"
#include "util/tool.h"
#include "util/debug.h"
#include "util/build-id.h"
#include "util/data.h"

#include "util/parse-options.h"

#include <linux/list.h>

struct perf_inject {
	struct perf_tool	tool;
	bool			build_ids;
	bool			sched_stat;
	const char		*input_name;
	struct perf_data_file	output;
	u64			bytes_written;
	struct list_head	samples;
};

struct event_entry {
	struct list_head node;
	u32		 tid;
	union perf_event event[0];
};

static int perf_event__repipe_synth(struct perf_tool *tool,
				    union perf_event *event)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);
	ssize_t size;

	size = perf_data_file__write(&inject->output, event,
				     event->header.size);
	if (size < 0)
		return -errno;

	inject->bytes_written += size;
	return 0;
}

static int perf_event__repipe_op2_synth(struct perf_tool *tool,
					union perf_event *event,
					struct perf_session *session
					__maybe_unused)
{
	return perf_event__repipe_synth(tool, event);
}

static int perf_event__repipe_attr(struct perf_tool *tool,
				   union perf_event *event,
				   struct perf_evlist **pevlist)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject,
						  tool);
	int ret;

	ret = perf_event__process_attr(tool, event, pevlist);
	if (ret)
		return ret;

	if (&inject->output.is_pipe)
		return 0;

	return perf_event__repipe_synth(tool, event);
>>>>>>> refs/remotes/origin/master
}

static int perf_event__repipe(struct perf_tool *tool,
			      union perf_event *event,
<<<<<<< HEAD
			      struct perf_sample *sample __used,
			      struct machine *machine)
{
	return perf_event__repipe_synth(tool, event, machine);
}

static int perf_event__repipe_sample(struct perf_tool *tool,
				     union perf_event *event,
			      struct perf_sample *sample __used,
			      struct perf_evsel *evsel __used,
			      struct machine *machine)
{
	return perf_event__repipe_synth(tool, event, machine);
=======
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
{
	return perf_event__repipe_synth(tool, event);
}

typedef int (*inject_handler)(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample,
			      struct perf_evsel *evsel,
			      struct machine *machine);

static int perf_event__repipe_sample(struct perf_tool *tool,
				     union perf_event *event,
				     struct perf_sample *sample,
				     struct perf_evsel *evsel,
				     struct machine *machine)
{
	if (evsel->handler) {
		inject_handler f = evsel->handler;
		return f(tool, event, sample, evsel, machine);
	}

	build_id__mark_dso_hit(tool, event, sample, evsel, machine);

	return perf_event__repipe_synth(tool, event);
>>>>>>> refs/remotes/origin/master
}

static int perf_event__repipe_mmap(struct perf_tool *tool,
				   union perf_event *event,
				   struct perf_sample *sample,
				   struct machine *machine)
{
	int err;

	err = perf_event__process_mmap(tool, event, sample, machine);
	perf_event__repipe(tool, event, sample, machine);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int perf_event__repipe_task(union perf_event *event,
				   struct perf_sample *sample,
				   struct perf_session *session)
{
	int err;

	err = perf_event__process_task(event, sample, session);
	perf_event__repipe(event, sample, session);
=======
static int perf_event__repipe_task(struct perf_tool *tool,
=======
static int perf_event__repipe_mmap2(struct perf_tool *tool,
				   union perf_event *event,
				   struct perf_sample *sample,
				   struct machine *machine)
{
	int err;

	err = perf_event__process_mmap2(tool, event, sample, machine);
	perf_event__repipe(tool, event, sample, machine);

	return err;
}

static int perf_event__repipe_fork(struct perf_tool *tool,
>>>>>>> refs/remotes/origin/master
				   union perf_event *event,
				   struct perf_sample *sample,
				   struct machine *machine)
{
	int err;

<<<<<<< HEAD
	err = perf_event__process_task(tool, event, sample, machine);
	perf_event__repipe(tool, event, sample, machine);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = perf_event__process_fork(tool, event, sample, machine);
	perf_event__repipe(tool, event, sample, machine);
>>>>>>> refs/remotes/origin/master

	return err;
}

<<<<<<< HEAD
static int perf_event__repipe_tracing_data(union perf_event *event,
=======
static int perf_event__repipe_tracing_data(struct perf_tool *tool,
					   union perf_event *event,
>>>>>>> refs/remotes/origin/master
					   struct perf_session *session)
{
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	perf_event__repipe_synth(event, session);
=======
	perf_event__repipe_synth(NULL, event, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
	err = perf_event__process_tracing_data(event, session);
=======
	perf_event__repipe_synth(tool, event);
	err = perf_event__process_tracing_data(tool, event, session);
>>>>>>> refs/remotes/origin/master

	return err;
}

<<<<<<< HEAD
static int dso__read_build_id(struct dso *self)
{
	if (self->has_build_id)
		return 0;

	if (filename__read_build_id(self->long_name, self->build_id,
				    sizeof(self->build_id)) > 0) {
		self->has_build_id = true;
=======
static int dso__read_build_id(struct dso *dso)
{
	if (dso->has_build_id)
		return 0;

	if (filename__read_build_id(dso->long_name, dso->build_id,
				    sizeof(dso->build_id)) > 0) {
		dso->has_build_id = true;
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	return -1;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int dso__inject_build_id(struct dso *self, struct perf_session *session)
{
	u16 misc = PERF_RECORD_MISC_USER;
	struct machine *machine;
=======
static int dso__inject_build_id(struct dso *self, struct perf_tool *tool,
				struct machine *machine)
{
	u16 misc = PERF_RECORD_MISC_USER;
>>>>>>> refs/remotes/origin/cm-10.0
	int err;

	if (dso__read_build_id(self) < 0) {
		pr_debug("no build_id found for %s\n", self->long_name);
		return -1;
	}

<<<<<<< HEAD
	machine = perf_session__find_host_machine(session);
	if (machine == NULL) {
		pr_err("Can't find machine for session\n");
		return -1;
	}

	if (self->kernel)
		misc = PERF_RECORD_MISC_KERNEL;

	err = perf_event__synthesize_build_id(self, misc, perf_event__repipe,
					      machine, session);
=======
	if (self->kernel)
		misc = PERF_RECORD_MISC_KERNEL;

	err = perf_event__synthesize_build_id(tool, self, misc, perf_event__repipe,
					      machine);
>>>>>>> refs/remotes/origin/cm-10.0
	if (err) {
		pr_err("Can't synthesize build_id event for %s\n", self->long_name);
=======
static int dso__inject_build_id(struct dso *dso, struct perf_tool *tool,
				struct machine *machine)
{
	u16 misc = PERF_RECORD_MISC_USER;
	int err;

	if (dso__read_build_id(dso) < 0) {
		pr_debug("no build_id found for %s\n", dso->long_name);
		return -1;
	}

	if (dso->kernel)
		misc = PERF_RECORD_MISC_KERNEL;

	err = perf_event__synthesize_build_id(tool, dso, misc, perf_event__repipe,
					      machine);
	if (err) {
		pr_err("Can't synthesize build_id event for %s\n", dso->long_name);
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int perf_event__inject_buildid(union perf_event *event,
				      struct perf_sample *sample,
				      struct perf_evsel *evsel __used,
				      struct perf_session *session)
=======
static int perf_event__inject_buildid(struct perf_tool *tool,
				      union perf_event *event,
				      struct perf_sample *sample,
				      struct perf_evsel *evsel __used,
				      struct machine *machine)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int perf_event__inject_buildid(struct perf_tool *tool,
				      union perf_event *event,
				      struct perf_sample *sample,
				      struct perf_evsel *evsel __maybe_unused,
				      struct machine *machine)
>>>>>>> refs/remotes/origin/master
{
	struct addr_location al;
	struct thread *thread;
	u8 cpumode;

	cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;

<<<<<<< HEAD
<<<<<<< HEAD
	thread = perf_session__findnew(session, event->ip.pid);
=======
	thread = machine__findnew_thread(machine, event->ip.pid);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	thread = machine__findnew_thread(machine, sample->pid, sample->pid);
>>>>>>> refs/remotes/origin/master
	if (thread == NULL) {
		pr_err("problem processing %d event, skipping it.\n",
		       event->header.type);
		goto repipe;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	thread__find_addr_map(thread, session, cpumode, MAP__FUNCTION,
			      event->ip.pid, event->ip.ip, &al);
=======
	thread__find_addr_map(thread, machine, cpumode, MAP__FUNCTION,
			      event->ip.ip, &al);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	thread__find_addr_map(thread, machine, cpumode, MAP__FUNCTION,
			      sample->ip, &al);
>>>>>>> refs/remotes/origin/master

	if (al.map != NULL) {
		if (!al.map->dso->hit) {
			al.map->dso->hit = 1;
			if (map__load(al.map, NULL) >= 0) {
<<<<<<< HEAD
<<<<<<< HEAD
				dso__inject_build_id(al.map->dso, session);
=======
				dso__inject_build_id(al.map->dso, tool, machine);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				dso__inject_build_id(al.map->dso, tool, machine);
>>>>>>> refs/remotes/origin/master
				/*
				 * If this fails, too bad, let the other side
				 * account this as unresolved.
				 */
<<<<<<< HEAD
			} else
				pr_warning("no symbols found in %s, maybe "
					   "install a debug package?\n",
					   al.map->dso->long_name);
=======
			} else {
#ifdef HAVE_LIBELF_SUPPORT
				pr_warning("no symbols found in %s, maybe "
					   "install a debug package?\n",
					   al.map->dso->long_name);
#endif
			}
>>>>>>> refs/remotes/origin/master
		}
	}

repipe:
<<<<<<< HEAD
<<<<<<< HEAD
	perf_event__repipe(event, sample, session);
	return 0;
}

struct perf_event_ops inject_ops = {
=======
	perf_event__repipe(tool, event, sample, machine);
	return 0;
}

struct perf_tool perf_inject = {
>>>>>>> refs/remotes/origin/cm-10.0
	.sample		= perf_event__repipe_sample,
	.mmap		= perf_event__repipe,
	.comm		= perf_event__repipe,
	.fork		= perf_event__repipe,
	.exit		= perf_event__repipe,
	.lost		= perf_event__repipe,
<<<<<<< HEAD
	.read		= perf_event__repipe,
	.throttle	= perf_event__repipe,
	.unthrottle	= perf_event__repipe,
	.attr		= perf_event__repipe_synth,
	.event_type 	= perf_event__repipe_synth,
	.tracing_data 	= perf_event__repipe_synth,
	.build_id 	= perf_event__repipe_synth,
=======
	.read		= perf_event__repipe_sample,
	.throttle	= perf_event__repipe,
	.unthrottle	= perf_event__repipe,
	.attr		= perf_event__repipe_attr,
	.event_type	= perf_event__repipe_event_type_synth,
	.tracing_data	= perf_event__repipe_tracing_data_synth,
	.build_id	= perf_event__repipe_op2_synth,
>>>>>>> refs/remotes/origin/cm-10.0
};

extern volatile int session_done;

static void sig_handler(int sig __attribute__((__unused__)))
=======
	perf_event__repipe(tool, event, sample, machine);
	return 0;
}

static int perf_inject__sched_process_exit(struct perf_tool *tool,
					   union perf_event *event __maybe_unused,
					   struct perf_sample *sample,
					   struct perf_evsel *evsel __maybe_unused,
					   struct machine *machine __maybe_unused)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);
	struct event_entry *ent;

	list_for_each_entry(ent, &inject->samples, node) {
		if (sample->tid == ent->tid) {
			list_del_init(&ent->node);
			free(ent);
			break;
		}
	}

	return 0;
}

static int perf_inject__sched_switch(struct perf_tool *tool,
				     union perf_event *event,
				     struct perf_sample *sample,
				     struct perf_evsel *evsel,
				     struct machine *machine)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);
	struct event_entry *ent;

	perf_inject__sched_process_exit(tool, event, sample, evsel, machine);

	ent = malloc(event->header.size + sizeof(struct event_entry));
	if (ent == NULL) {
		color_fprintf(stderr, PERF_COLOR_RED,
			     "Not enough memory to process sched switch event!");
		return -1;
	}

	ent->tid = sample->tid;
	memcpy(&ent->event, event, event->header.size);
	list_add(&ent->node, &inject->samples);
	return 0;
}

static int perf_inject__sched_stat(struct perf_tool *tool,
				   union perf_event *event __maybe_unused,
				   struct perf_sample *sample,
				   struct perf_evsel *evsel,
				   struct machine *machine)
{
	struct event_entry *ent;
	union perf_event *event_sw;
	struct perf_sample sample_sw;
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);
	u32 pid = perf_evsel__intval(evsel, sample, "pid");

	list_for_each_entry(ent, &inject->samples, node) {
		if (pid == ent->tid)
			goto found;
	}

	return 0;
found:
	event_sw = &ent->event[0];
	perf_evsel__parse_sample(evsel, event_sw, &sample_sw);

	sample_sw.period = sample->period;
	sample_sw.time	 = sample->time;
	perf_event__synthesize_sample(event_sw, evsel->attr.sample_type,
				      evsel->attr.sample_regs_user,
				      evsel->attr.read_format, &sample_sw,
				      false);
	build_id__mark_dso_hit(tool, event_sw, &sample_sw, evsel, machine);
	return perf_event__repipe(tool, event_sw, &sample_sw, machine);
}

static void sig_handler(int sig __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	session_done = 1;
}

<<<<<<< HEAD
static int __cmd_inject(void)
{
	struct perf_session *session;
	int ret = -EINVAL;

	signal(SIGINT, sig_handler);

	if (inject_build_ids) {
<<<<<<< HEAD
		inject_ops.sample	= perf_event__inject_buildid;
		inject_ops.mmap		= perf_event__repipe_mmap;
		inject_ops.fork		= perf_event__repipe_task;
		inject_ops.tracing_data	= perf_event__repipe_tracing_data;
	}

	session = perf_session__new(input_name, O_RDONLY, false, true, &inject_ops);
	if (session == NULL)
		return -ENOMEM;

	ret = perf_session__process_events(session, &inject_ops);
=======
		perf_inject.sample	 = perf_event__inject_buildid;
		perf_inject.mmap	 = perf_event__repipe_mmap;
		perf_inject.fork	 = perf_event__repipe_task;
		perf_inject.tracing_data = perf_event__repipe_tracing_data;
	}

	session = perf_session__new(input_name, O_RDONLY, false, true, &perf_inject);
	if (session == NULL)
		return -ENOMEM;

	ret = perf_session__process_events(session, &perf_inject);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int perf_evsel__check_stype(struct perf_evsel *evsel,
				   u64 sample_type, const char *sample_msg)
{
	struct perf_event_attr *attr = &evsel->attr;
	const char *name = perf_evsel__name(evsel);

	if (!(attr->sample_type & sample_type)) {
		pr_err("Samples for %s event do not have %s attribute set.",
			name, sample_msg);
		return -EINVAL;
	}

	return 0;
}

static int __cmd_inject(struct perf_inject *inject)
{
	struct perf_session *session;
	int ret = -EINVAL;
	struct perf_data_file file = {
		.path = inject->input_name,
		.mode = PERF_DATA_MODE_READ,
	};
	struct perf_data_file *file_out = &inject->output;

	signal(SIGINT, sig_handler);

	if (inject->build_ids || inject->sched_stat) {
		inject->tool.mmap	  = perf_event__repipe_mmap;
		inject->tool.mmap2	  = perf_event__repipe_mmap2;
		inject->tool.fork	  = perf_event__repipe_fork;
		inject->tool.tracing_data = perf_event__repipe_tracing_data;
	}

	session = perf_session__new(&file, true, &inject->tool);
	if (session == NULL)
		return -ENOMEM;

	if (inject->build_ids) {
		inject->tool.sample = perf_event__inject_buildid;
	} else if (inject->sched_stat) {
		struct perf_evsel *evsel;

		inject->tool.ordered_samples = true;

		evlist__for_each(session->evlist, evsel) {
			const char *name = perf_evsel__name(evsel);

			if (!strcmp(name, "sched:sched_switch")) {
				if (perf_evsel__check_stype(evsel, PERF_SAMPLE_TID, "TID"))
					return -EINVAL;

				evsel->handler = perf_inject__sched_switch;
			} else if (!strcmp(name, "sched:sched_process_exit"))
				evsel->handler = perf_inject__sched_process_exit;
			else if (!strncmp(name, "sched:sched_stat_", 17))
				evsel->handler = perf_inject__sched_stat;
		}
	}

	if (!file_out->is_pipe)
		lseek(file_out->fd, session->header.data_offset, SEEK_SET);

	ret = perf_session__process_events(session, &inject->tool);

	if (!file_out->is_pipe) {
		session->header.data_size = inject->bytes_written;
		perf_session__write_header(session, session->evlist, file_out->fd, true);
	}
>>>>>>> refs/remotes/origin/master

	perf_session__delete(session);

	return ret;
}

<<<<<<< HEAD
static const char * const report_usage[] = {
	"perf inject [<options>]",
	NULL
};

static const struct option options[] = {
	OPT_BOOLEAN('b', "build-ids", &inject_build_ids,
		    "Inject build-ids into the output stream"),
	OPT_INCR('v', "verbose", &verbose,
		 "be more verbose (show build ids, etc)"),
	OPT_END()
};

int cmd_inject(int argc, const char **argv, const char *prefix __used)
{
	argc = parse_options(argc, argv, options, report_usage, 0);
=======
int cmd_inject(int argc, const char **argv, const char *prefix __maybe_unused)
{
	struct perf_inject inject = {
		.tool = {
			.sample		= perf_event__repipe_sample,
			.mmap		= perf_event__repipe,
			.mmap2		= perf_event__repipe,
			.comm		= perf_event__repipe,
			.fork		= perf_event__repipe,
			.exit		= perf_event__repipe,
			.lost		= perf_event__repipe,
			.read		= perf_event__repipe_sample,
			.throttle	= perf_event__repipe,
			.unthrottle	= perf_event__repipe,
			.attr		= perf_event__repipe_attr,
			.tracing_data	= perf_event__repipe_op2_synth,
			.finished_round	= perf_event__repipe_op2_synth,
			.build_id	= perf_event__repipe_op2_synth,
		},
		.input_name  = "-",
		.samples = LIST_HEAD_INIT(inject.samples),
		.output = {
			.path = "-",
			.mode = PERF_DATA_MODE_WRITE,
		},
	};
	const struct option options[] = {
		OPT_BOOLEAN('b', "build-ids", &inject.build_ids,
			    "Inject build-ids into the output stream"),
		OPT_STRING('i', "input", &inject.input_name, "file",
			   "input file name"),
		OPT_STRING('o', "output", &inject.output.path, "file",
			   "output file name"),
		OPT_BOOLEAN('s', "sched-stat", &inject.sched_stat,
			    "Merge sched-stat and sched-switch for getting events "
			    "where and how long tasks slept"),
		OPT_INCR('v', "verbose", &verbose,
			 "be more verbose (show build ids, etc)"),
		OPT_END()
	};
	const char * const inject_usage[] = {
		"perf inject [<options>]",
		NULL
	};

	argc = parse_options(argc, argv, options, inject_usage, 0);
>>>>>>> refs/remotes/origin/master

	/*
	 * Any (unrecognized) arguments left?
	 */
	if (argc)
<<<<<<< HEAD
		usage_with_options(report_usage, options);
=======
		usage_with_options(inject_usage, options);

	if (perf_data_file__open(&inject.output)) {
		perror("failed to create output file");
		return -1;
	}
>>>>>>> refs/remotes/origin/master

	if (symbol__init() < 0)
		return -1;

<<<<<<< HEAD
	return __cmd_inject();
=======
	return __cmd_inject(&inject);
>>>>>>> refs/remotes/origin/master
}
