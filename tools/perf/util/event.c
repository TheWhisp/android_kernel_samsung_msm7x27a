#include <linux/types.h>
#include "event.h"
#include "debug.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "session.h"
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "machine.h"
>>>>>>> refs/remotes/origin/master
#include "sort.h"
#include "string.h"
#include "strlist.h"
#include "thread.h"
#include "thread_map.h"
<<<<<<< HEAD
=======
#include "symbol/kallsyms.h"
>>>>>>> refs/remotes/origin/master

static const char *perf_event__names[] = {
	[0]					= "TOTAL",
	[PERF_RECORD_MMAP]			= "MMAP",
<<<<<<< HEAD
=======
	[PERF_RECORD_MMAP2]			= "MMAP2",
>>>>>>> refs/remotes/origin/master
	[PERF_RECORD_LOST]			= "LOST",
	[PERF_RECORD_COMM]			= "COMM",
	[PERF_RECORD_EXIT]			= "EXIT",
	[PERF_RECORD_THROTTLE]			= "THROTTLE",
	[PERF_RECORD_UNTHROTTLE]		= "UNTHROTTLE",
	[PERF_RECORD_FORK]			= "FORK",
	[PERF_RECORD_READ]			= "READ",
	[PERF_RECORD_SAMPLE]			= "SAMPLE",
	[PERF_RECORD_HEADER_ATTR]		= "ATTR",
	[PERF_RECORD_HEADER_EVENT_TYPE]		= "EVENT_TYPE",
	[PERF_RECORD_HEADER_TRACING_DATA]	= "TRACING_DATA",
	[PERF_RECORD_HEADER_BUILD_ID]		= "BUILD_ID",
	[PERF_RECORD_FINISHED_ROUND]		= "FINISHED_ROUND",
};

const char *perf_event__name(unsigned int id)
{
	if (id >= ARRAY_SIZE(perf_event__names))
		return "INVALID";
	if (!perf_event__names[id])
		return "UNKNOWN";
	return perf_event__names[id];
}

static struct perf_sample synth_sample = {
	.pid	   = -1,
	.tid	   = -1,
	.time	   = -1,
	.stream_id = -1,
	.cpu	   = -1,
	.period	   = 1,
};

<<<<<<< HEAD
<<<<<<< HEAD
static pid_t perf_event__synthesize_comm(union perf_event *event, pid_t pid,
					 int full, perf_event__handler_t process,
					 struct perf_session *session)
=======
static pid_t perf_event__get_comm_tgid(pid_t pid, char *comm, size_t len)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static pid_t perf_event__get_comm_tgid(pid_t pid, char *comm, size_t len)
>>>>>>> refs/remotes/origin/master
{
	char filename[PATH_MAX];
	char bf[BUFSIZ];
	FILE *fp;
	size_t size = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	DIR *tasks;
	struct dirent dirent, *next;
	pid_t tgid = 0;
=======
	pid_t tgid = -1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pid_t tgid = -1;
>>>>>>> refs/remotes/origin/master

	snprintf(filename, sizeof(filename), "/proc/%d/status", pid);

	fp = fopen(filename, "r");
	if (fp == NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
out_race:
		/*
		 * We raced with a task exiting - just return:
		 */
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		pr_debug("couldn't open %s\n", filename);
		return 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	memset(&event->comm, 0, sizeof(event->comm));

	while (!event->comm.comm[0] || !event->comm.pid) {
		if (fgets(bf, sizeof(bf), fp) == NULL) {
			pr_warning("couldn't get COMM and pgid, malformed %s\n", filename);
			goto out;
=======
=======
>>>>>>> refs/remotes/origin/master
	while (!comm[0] || (tgid < 0)) {
		if (fgets(bf, sizeof(bf), fp) == NULL) {
			pr_warning("couldn't get COMM and pgid, malformed %s\n",
				   filename);
			break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}

		if (memcmp(bf, "Name:", 5) == 0) {
			char *name = bf + 5;
			while (*name && isspace(*name))
				++name;
			size = strlen(name) - 1;
<<<<<<< HEAD
<<<<<<< HEAD
			memcpy(event->comm.comm, name, size++);
=======
=======
>>>>>>> refs/remotes/origin/master
			if (size >= len)
				size = len - 1;
			memcpy(comm, name, size);
			comm[size] = '\0';

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		} else if (memcmp(bf, "Tgid:", 5) == 0) {
			char *tgids = bf + 5;
			while (*tgids && isspace(*tgids))
				++tgids;
<<<<<<< HEAD
<<<<<<< HEAD
			tgid = event->comm.pid = atoi(tgids);
		}
	}

	event->comm.header.type = PERF_RECORD_COMM;
	size = ALIGN(size, sizeof(u64));
	memset(event->comm.comm + size, 0, session->id_hdr_size);
	event->comm.header.size = (sizeof(event->comm) -
				(sizeof(event->comm.comm) - size) +
				session->id_hdr_size);
	if (!full) {
		event->comm.tid = pid;

		process(event, &synth_sample, session);
=======
=======
>>>>>>> refs/remotes/origin/master
			tgid = atoi(tgids);
		}
	}

	fclose(fp);

	return tgid;
}

static pid_t perf_event__synthesize_comm(struct perf_tool *tool,
					 union perf_event *event, pid_t pid,
					 int full,
					 perf_event__handler_t process,
					 struct machine *machine)
{
	char filename[PATH_MAX];
	size_t size;
	DIR *tasks;
	struct dirent dirent, *next;
	pid_t tgid;

	memset(&event->comm, 0, sizeof(event->comm));

<<<<<<< HEAD
	tgid = perf_event__get_comm_tgid(pid, event->comm.comm,
					 sizeof(event->comm.comm));
=======
	if (machine__is_host(machine))
		tgid = perf_event__get_comm_tgid(pid, event->comm.comm,
						 sizeof(event->comm.comm));
	else
		tgid = machine->pid;

>>>>>>> refs/remotes/origin/master
	if (tgid < 0)
		goto out;

	event->comm.pid = tgid;
	event->comm.header.type = PERF_RECORD_COMM;

	size = strlen(event->comm.comm) + 1;
<<<<<<< HEAD
	size = ALIGN(size, sizeof(u64));
=======
	size = PERF_ALIGN(size, sizeof(u64));
>>>>>>> refs/remotes/origin/master
	memset(event->comm.comm + size, 0, machine->id_hdr_size);
	event->comm.header.size = (sizeof(event->comm) -
				(sizeof(event->comm.comm) - size) +
				machine->id_hdr_size);
	if (!full) {
		event->comm.tid = pid;

<<<<<<< HEAD
		process(tool, event, &synth_sample, machine);
>>>>>>> refs/remotes/origin/cm-10.0
		goto out;
	}

	snprintf(filename, sizeof(filename), "/proc/%d/task", pid);

	tasks = opendir(filename);
<<<<<<< HEAD
	if (tasks == NULL)
		goto out_race;
=======
=======
		if (process(tool, event, &synth_sample, machine) != 0)
			return -1;

		goto out;
	}

	if (machine__is_default_guest(machine))
		return 0;

	snprintf(filename, sizeof(filename), "%s/proc/%d/task",
		 machine->root_dir, pid);

	tasks = opendir(filename);
>>>>>>> refs/remotes/origin/master
	if (tasks == NULL) {
		pr_debug("couldn't open %s\n", filename);
		return 0;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	while (!readdir_r(tasks, &dirent, &next) && next) {
		char *end;
		pid = strtol(dirent.d_name, &end, 10);
		if (*end)
			continue;

<<<<<<< HEAD
<<<<<<< HEAD
		event->comm.tid = pid;

		process(event, &synth_sample, session);
=======
=======
>>>>>>> refs/remotes/origin/master
		/* already have tgid; jut want to update the comm */
		(void) perf_event__get_comm_tgid(pid, event->comm.comm,
					 sizeof(event->comm.comm));

		size = strlen(event->comm.comm) + 1;
<<<<<<< HEAD
		size = ALIGN(size, sizeof(u64));
=======
		size = PERF_ALIGN(size, sizeof(u64));
>>>>>>> refs/remotes/origin/master
		memset(event->comm.comm + size, 0, machine->id_hdr_size);
		event->comm.header.size = (sizeof(event->comm) -
					  (sizeof(event->comm.comm) - size) +
					  machine->id_hdr_size);

		event->comm.tid = pid;

<<<<<<< HEAD
		process(tool, event, &synth_sample, machine);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (process(tool, event, &synth_sample, machine) != 0) {
			tgid = -1;
			break;
		}
>>>>>>> refs/remotes/origin/master
	}

	closedir(tasks);
out:
<<<<<<< HEAD
<<<<<<< HEAD
	fclose(fp);

	return tgid;
}

static int perf_event__synthesize_mmap_events(union perf_event *event,
					      pid_t pid, pid_t tgid,
					      perf_event__handler_t process,
					      struct perf_session *session)
=======
	return tgid;
}

static int perf_event__synthesize_mmap_events(struct perf_tool *tool,
					      union perf_event *event,
					      pid_t pid, pid_t tgid,
					      perf_event__handler_t process,
					      struct machine *machine)
>>>>>>> refs/remotes/origin/cm-10.0
{
	char filename[PATH_MAX];
	FILE *fp;

	snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
=======
	return tgid;
}

int perf_event__synthesize_mmap_events(struct perf_tool *tool,
				       union perf_event *event,
				       pid_t pid, pid_t tgid,
				       perf_event__handler_t process,
				       struct machine *machine,
				       bool mmap_data)
{
	char filename[PATH_MAX];
	FILE *fp;
	int rc = 0;

	if (machine__is_default_guest(machine))
		return 0;

	snprintf(filename, sizeof(filename), "%s/proc/%d/maps",
		 machine->root_dir, pid);
>>>>>>> refs/remotes/origin/master

	fp = fopen(filename, "r");
	if (fp == NULL) {
		/*
		 * We raced with a task exiting - just return:
		 */
		pr_debug("couldn't open %s\n", filename);
		return -1;
	}

	event->header.type = PERF_RECORD_MMAP;
<<<<<<< HEAD
	/*
	 * Just like the kernel, see __perf_event_mmap in kernel/perf_event.c
	 */
	event->header.misc = PERF_RECORD_MISC_USER;

	while (1) {
		char bf[BUFSIZ], *pbf = bf;
		int n;
		size_t size;
		if (fgets(bf, sizeof(bf), fp) == NULL)
			break;

		/* 00400000-0040c000 r-xp 00000000 fd:01 41038  /bin/cat */
		n = hex2u64(pbf, &event->mmap.start);
		if (n < 0)
			continue;
		pbf += n + 1;
		n = hex2u64(pbf, &event->mmap.len);
		if (n < 0)
			continue;
		pbf += n + 3;
		if (*pbf == 'x') { /* vm_exec */
<<<<<<< HEAD
=======
			char anonstr[] = "//anon\n";
>>>>>>> refs/remotes/origin/cm-10.0
			char *execname = strchr(bf, '/');

			/* Catch VDSO */
			if (execname == NULL)
				execname = strstr(bf, "[vdso]");

<<<<<<< HEAD
=======
			/* Catch anonymous mmaps */
			if ((execname == NULL) && !strstr(bf, "["))
				execname = anonstr;

>>>>>>> refs/remotes/origin/cm-10.0
			if (execname == NULL)
				continue;

			pbf += 3;
			n = hex2u64(pbf, &event->mmap.pgoff);

			size = strlen(execname);
			execname[size - 1] = '\0'; /* Remove \n */
			memcpy(event->mmap.filename, execname, size);
			size = ALIGN(size, sizeof(u64));
			event->mmap.len -= event->mmap.start;
			event->mmap.header.size = (sizeof(event->mmap) -
					        (sizeof(event->mmap.filename) - size));
<<<<<<< HEAD
			memset(event->mmap.filename + size, 0, session->id_hdr_size);
			event->mmap.header.size += session->id_hdr_size;
			event->mmap.pid = tgid;
			event->mmap.tid = pid;

			process(event, &synth_sample, session);
=======
			memset(event->mmap.filename + size, 0, machine->id_hdr_size);
			event->mmap.header.size += machine->id_hdr_size;
			event->mmap.pid = tgid;
			event->mmap.tid = pid;

			process(tool, event, &synth_sample, machine);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	while (1) {
		char bf[BUFSIZ];
		char prot[5];
		char execname[PATH_MAX];
		char anonstr[] = "//anon";
		size_t size;
		ssize_t n;

		if (fgets(bf, sizeof(bf), fp) == NULL)
			break;

		/* ensure null termination since stack will be reused. */
		strcpy(execname, "");

		/* 00400000-0040c000 r-xp 00000000 fd:01 41038  /bin/cat */
		n = sscanf(bf, "%"PRIx64"-%"PRIx64" %s %"PRIx64" %*x:%*x %*u %s\n",
		       &event->mmap.start, &event->mmap.len, prot,
		       &event->mmap.pgoff,
		       execname);
		/*
 		 * Anon maps don't have the execname.
 		 */
		if (n < 4)
			continue;
		/*
		 * Just like the kernel, see __perf_event_mmap in kernel/perf_event.c
		 */
		if (machine__is_host(machine))
			event->header.misc = PERF_RECORD_MISC_USER;
		else
			event->header.misc = PERF_RECORD_MISC_GUEST_USER;

		if (prot[2] != 'x') {
			if (!mmap_data || prot[0] != 'r')
				continue;

			event->header.misc |= PERF_RECORD_MISC_MMAP_DATA;
		}

		if (!strcmp(execname, ""))
			strcpy(execname, anonstr);

		size = strlen(execname) + 1;
		memcpy(event->mmap.filename, execname, size);
		size = PERF_ALIGN(size, sizeof(u64));
		event->mmap.len -= event->mmap.start;
		event->mmap.header.size = (sizeof(event->mmap) -
					(sizeof(event->mmap.filename) - size));
		memset(event->mmap.filename + size, 0, machine->id_hdr_size);
		event->mmap.header.size += machine->id_hdr_size;
		event->mmap.pid = tgid;
		event->mmap.tid = pid;

		if (process(tool, event, &synth_sample, machine) != 0) {
			rc = -1;
			break;
>>>>>>> refs/remotes/origin/master
		}
	}

	fclose(fp);
<<<<<<< HEAD
	return 0;
}

<<<<<<< HEAD
int perf_event__synthesize_modules(perf_event__handler_t process,
				   struct perf_session *session,
=======
int perf_event__synthesize_modules(struct perf_tool *tool,
				   perf_event__handler_t process,
>>>>>>> refs/remotes/origin/cm-10.0
				   struct machine *machine)
{
	struct rb_node *nd;
	struct map_groups *kmaps = &machine->kmaps;
	union perf_event *event = zalloc((sizeof(event->mmap) +
<<<<<<< HEAD
					  session->id_hdr_size));
=======
					  machine->id_hdr_size));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return rc;
}

int perf_event__synthesize_modules(struct perf_tool *tool,
				   perf_event__handler_t process,
				   struct machine *machine)
{
	int rc = 0;
	struct rb_node *nd;
	struct map_groups *kmaps = &machine->kmaps;
	union perf_event *event = zalloc((sizeof(event->mmap) +
					  machine->id_hdr_size));
>>>>>>> refs/remotes/origin/master
	if (event == NULL) {
		pr_debug("Not enough memory synthesizing mmap event "
			 "for kernel modules\n");
		return -1;
	}

	event->header.type = PERF_RECORD_MMAP;

	/*
	 * kernel uses 0 for user space maps, see kernel/perf_event.c
	 * __perf_event_mmap
	 */
	if (machine__is_host(machine))
		event->header.misc = PERF_RECORD_MISC_KERNEL;
	else
		event->header.misc = PERF_RECORD_MISC_GUEST_KERNEL;

	for (nd = rb_first(&kmaps->maps[MAP__FUNCTION]);
	     nd; nd = rb_next(nd)) {
		size_t size;
		struct map *pos = rb_entry(nd, struct map, rb_node);

		if (pos->dso->kernel)
			continue;

<<<<<<< HEAD
		size = ALIGN(pos->dso->long_name_len + 1, sizeof(u64));
		event->mmap.header.type = PERF_RECORD_MMAP;
		event->mmap.header.size = (sizeof(event->mmap) -
				        (sizeof(event->mmap.filename) - size));
<<<<<<< HEAD
		memset(event->mmap.filename + size, 0, session->id_hdr_size);
		event->mmap.header.size += session->id_hdr_size;
=======
		memset(event->mmap.filename + size, 0, machine->id_hdr_size);
		event->mmap.header.size += machine->id_hdr_size;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		size = PERF_ALIGN(pos->dso->long_name_len + 1, sizeof(u64));
		event->mmap.header.type = PERF_RECORD_MMAP;
		event->mmap.header.size = (sizeof(event->mmap) -
				        (sizeof(event->mmap.filename) - size));
		memset(event->mmap.filename + size, 0, machine->id_hdr_size);
		event->mmap.header.size += machine->id_hdr_size;
>>>>>>> refs/remotes/origin/master
		event->mmap.start = pos->start;
		event->mmap.len   = pos->end - pos->start;
		event->mmap.pid   = machine->pid;

		memcpy(event->mmap.filename, pos->dso->long_name,
		       pos->dso->long_name_len + 1);
<<<<<<< HEAD
<<<<<<< HEAD
		process(event, &synth_sample, session);
=======
		process(tool, event, &synth_sample, machine);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	free(event);
	return 0;
=======
		if (process(tool, event, &synth_sample, machine) != 0) {
			rc = -1;
			break;
		}
	}

	free(event);
	return rc;
>>>>>>> refs/remotes/origin/master
}

static int __event__synthesize_thread(union perf_event *comm_event,
				      union perf_event *mmap_event,
<<<<<<< HEAD
<<<<<<< HEAD
				      pid_t pid, perf_event__handler_t process,
				      struct perf_session *session)
{
	pid_t tgid = perf_event__synthesize_comm(comm_event, pid, 1, process,
					    session);
	if (tgid == -1)
		return -1;
	return perf_event__synthesize_mmap_events(mmap_event, pid, tgid,
					     process, session);
}

int perf_event__synthesize_thread_map(struct thread_map *threads,
				      perf_event__handler_t process,
				      struct perf_session *session)
{
	union perf_event *comm_event, *mmap_event;
	int err = -1, thread;

	comm_event = malloc(sizeof(comm_event->comm) + session->id_hdr_size);
	if (comm_event == NULL)
		goto out;

	mmap_event = malloc(sizeof(mmap_event->mmap) + session->id_hdr_size);
=======
				      pid_t pid, int full,
					  perf_event__handler_t process,
				      struct perf_tool *tool,
				      struct machine *machine)
=======
				      pid_t pid, int full,
					  perf_event__handler_t process,
				      struct perf_tool *tool,
				      struct machine *machine, bool mmap_data)
>>>>>>> refs/remotes/origin/master
{
	pid_t tgid = perf_event__synthesize_comm(tool, comm_event, pid, full,
						 process, machine);
	if (tgid == -1)
		return -1;
	return perf_event__synthesize_mmap_events(tool, mmap_event, pid, tgid,
<<<<<<< HEAD
						  process, machine);
=======
						  process, machine, mmap_data);
>>>>>>> refs/remotes/origin/master
}

int perf_event__synthesize_thread_map(struct perf_tool *tool,
				      struct thread_map *threads,
				      perf_event__handler_t process,
<<<<<<< HEAD
				      struct machine *machine)
=======
				      struct machine *machine,
				      bool mmap_data)
>>>>>>> refs/remotes/origin/master
{
	union perf_event *comm_event, *mmap_event;
	int err = -1, thread, j;

	comm_event = malloc(sizeof(comm_event->comm) + machine->id_hdr_size);
	if (comm_event == NULL)
		goto out;

	mmap_event = malloc(sizeof(mmap_event->mmap) + machine->id_hdr_size);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (mmap_event == NULL)
		goto out_free_comm;

	err = 0;
	for (thread = 0; thread < threads->nr; ++thread) {
		if (__event__synthesize_thread(comm_event, mmap_event,
<<<<<<< HEAD
<<<<<<< HEAD
					       threads->map[thread],
					       process, session)) {
			err = -1;
			break;
		}
=======
					       threads->map[thread], 0,
					       process, tool, machine)) {
=======
					       threads->map[thread], 0,
					       process, tool, machine,
					       mmap_data)) {
>>>>>>> refs/remotes/origin/master
			err = -1;
			break;
		}

		/*
		 * comm.pid is set to thread group id by
		 * perf_event__synthesize_comm
		 */
		if ((int) comm_event->comm.pid != threads->map[thread]) {
			bool need_leader = true;

			/* is thread group leader in thread_map? */
			for (j = 0; j < threads->nr; ++j) {
				if ((int) comm_event->comm.pid == threads->map[j]) {
					need_leader = false;
					break;
				}
			}

			/* if not, generate events for it */
			if (need_leader &&
<<<<<<< HEAD
			    __event__synthesize_thread(comm_event,
						      mmap_event,
						      comm_event->comm.pid, 0,
						      process, tool, machine)) {
=======
			    __event__synthesize_thread(comm_event, mmap_event,
						       comm_event->comm.pid, 0,
						       process, tool, machine,
						       mmap_data)) {
>>>>>>> refs/remotes/origin/master
				err = -1;
				break;
			}
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	free(mmap_event);
out_free_comm:
	free(comm_event);
out:
	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
int perf_event__synthesize_threads(perf_event__handler_t process,
				   struct perf_session *session)
=======
int perf_event__synthesize_threads(struct perf_tool *tool,
				   perf_event__handler_t process,
				   struct machine *machine)
>>>>>>> refs/remotes/origin/cm-10.0
{
	DIR *proc;
=======
int perf_event__synthesize_threads(struct perf_tool *tool,
				   perf_event__handler_t process,
				   struct machine *machine, bool mmap_data)
{
	DIR *proc;
	char proc_path[PATH_MAX];
>>>>>>> refs/remotes/origin/master
	struct dirent dirent, *next;
	union perf_event *comm_event, *mmap_event;
	int err = -1;

<<<<<<< HEAD
<<<<<<< HEAD
	comm_event = malloc(sizeof(comm_event->comm) + session->id_hdr_size);
	if (comm_event == NULL)
		goto out;

	mmap_event = malloc(sizeof(mmap_event->mmap) + session->id_hdr_size);
=======
=======
>>>>>>> refs/remotes/origin/master
	comm_event = malloc(sizeof(comm_event->comm) + machine->id_hdr_size);
	if (comm_event == NULL)
		goto out;

	mmap_event = malloc(sizeof(mmap_event->mmap) + machine->id_hdr_size);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (mmap_event == NULL)
		goto out_free_comm;

	proc = opendir("/proc");
=======
	if (mmap_event == NULL)
		goto out_free_comm;

	if (machine__is_default_guest(machine))
		return 0;

	snprintf(proc_path, sizeof(proc_path), "%s/proc", machine->root_dir);
	proc = opendir(proc_path);

>>>>>>> refs/remotes/origin/master
	if (proc == NULL)
		goto out_free_mmap;

	while (!readdir_r(proc, &dirent, &next) && next) {
		char *end;
		pid_t pid = strtol(dirent.d_name, &end, 10);

		if (*end) /* only interested in proper numerical dirents */
			continue;
<<<<<<< HEAD

<<<<<<< HEAD
		__event__synthesize_thread(comm_event, mmap_event, pid,
					   process, session);
=======
		__event__synthesize_thread(comm_event, mmap_event, pid, 1,
					   process, tool, machine);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	closedir(proc);
	err = 0;
=======
		/*
 		 * We may race with exiting thread, so don't stop just because
 		 * one thread couldn't be synthesized.
 		 */
		__event__synthesize_thread(comm_event, mmap_event, pid, 1,
					   process, tool, machine, mmap_data);
	}

	err = 0;
	closedir(proc);
>>>>>>> refs/remotes/origin/master
out_free_mmap:
	free(mmap_event);
out_free_comm:
	free(comm_event);
out:
	return err;
}

struct process_symbol_args {
	const char *name;
	u64	   start;
};

static int find_symbol_cb(void *arg, const char *name, char type,
<<<<<<< HEAD
			  u64 start, u64 end __used)
=======
			  u64 start)
>>>>>>> refs/remotes/origin/master
{
	struct process_symbol_args *args = arg;

	/*
	 * Must be a function or at least an alias, as in PARISC64, where "_text" is
	 * an 'A' to the same address as "_stext".
	 */
	if (!(symbol_type__is_a(type, MAP__FUNCTION) ||
	      type == 'A') || strcmp(name, args->name))
		return 0;

	args->start = start;
	return 1;
}

<<<<<<< HEAD
<<<<<<< HEAD
int perf_event__synthesize_kernel_mmap(perf_event__handler_t process,
				       struct perf_session *session,
=======
int perf_event__synthesize_kernel_mmap(struct perf_tool *tool,
				       perf_event__handler_t process,
>>>>>>> refs/remotes/origin/cm-10.0
=======
int perf_event__synthesize_kernel_mmap(struct perf_tool *tool,
				       perf_event__handler_t process,
>>>>>>> refs/remotes/origin/master
				       struct machine *machine,
				       const char *symbol_name)
{
	size_t size;
	const char *filename, *mmap_name;
	char path[PATH_MAX];
	char name_buff[PATH_MAX];
	struct map *map;
	int err;
	/*
	 * We should get this from /sys/kernel/sections/.text, but till that is
	 * available use this, and after it is use this as a fallback for older
	 * kernels.
	 */
	struct process_symbol_args args = { .name = symbol_name, };
	union perf_event *event = zalloc((sizeof(event->mmap) +
<<<<<<< HEAD
<<<<<<< HEAD
					  session->id_hdr_size));
=======
					  machine->id_hdr_size));
>>>>>>> refs/remotes/origin/cm-10.0
=======
					  machine->id_hdr_size));
>>>>>>> refs/remotes/origin/master
	if (event == NULL) {
		pr_debug("Not enough memory synthesizing mmap event "
			 "for kernel modules\n");
		return -1;
	}

	mmap_name = machine__mmap_name(machine, name_buff, sizeof(name_buff));
	if (machine__is_host(machine)) {
		/*
		 * kernel uses PERF_RECORD_MISC_USER for user space maps,
		 * see kernel/perf_event.c __perf_event_mmap
		 */
		event->header.misc = PERF_RECORD_MISC_KERNEL;
		filename = "/proc/kallsyms";
	} else {
		event->header.misc = PERF_RECORD_MISC_GUEST_KERNEL;
		if (machine__is_default_guest(machine))
			filename = (char *) symbol_conf.default_guest_kallsyms;
		else {
			sprintf(path, "%s/proc/kallsyms", machine->root_dir);
			filename = path;
		}
	}

<<<<<<< HEAD
	if (kallsyms__parse(filename, &args, find_symbol_cb) <= 0)
		return -ENOENT;
=======
	if (kallsyms__parse(filename, &args, find_symbol_cb) <= 0) {
		free(event);
		return -ENOENT;
	}
>>>>>>> refs/remotes/origin/master

	map = machine->vmlinux_maps[MAP__FUNCTION];
	size = snprintf(event->mmap.filename, sizeof(event->mmap.filename),
			"%s%s", mmap_name, symbol_name) + 1;
<<<<<<< HEAD
	size = ALIGN(size, sizeof(u64));
	event->mmap.header.type = PERF_RECORD_MMAP;
	event->mmap.header.size = (sizeof(event->mmap) -
<<<<<<< HEAD
			(sizeof(event->mmap.filename) - size) + session->id_hdr_size);
=======
			(sizeof(event->mmap.filename) - size) + machine->id_hdr_size);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	size = PERF_ALIGN(size, sizeof(u64));
	event->mmap.header.type = PERF_RECORD_MMAP;
	event->mmap.header.size = (sizeof(event->mmap) -
			(sizeof(event->mmap.filename) - size) + machine->id_hdr_size);
>>>>>>> refs/remotes/origin/master
	event->mmap.pgoff = args.start;
	event->mmap.start = map->start;
	event->mmap.len   = map->end - event->mmap.start;
	event->mmap.pid   = machine->pid;

<<<<<<< HEAD
<<<<<<< HEAD
	err = process(event, &synth_sample, session);
=======
	err = process(tool, event, &synth_sample, machine);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = process(tool, event, &synth_sample, machine);
>>>>>>> refs/remotes/origin/master
	free(event);

	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
int perf_event__process_comm(union perf_event *event,
			     struct perf_sample *sample __used,
			     struct perf_session *session)
{
	struct thread *thread = perf_session__findnew(session, event->comm.tid);

	dump_printf(": %s:%d\n", event->comm.comm, event->comm.tid);
=======
=======
>>>>>>> refs/remotes/origin/master
size_t perf_event__fprintf_comm(union perf_event *event, FILE *fp)
{
	return fprintf(fp, ": %s:%d\n", event->comm.comm, event->comm.tid);
}

<<<<<<< HEAD
int perf_event__process_comm(struct perf_tool *tool __used,
			     union perf_event *event,
			     struct perf_sample *sample __used,
			     struct machine *machine)
{
	struct thread *thread = machine__findnew_thread(machine, event->comm.tid);

	if (dump_trace)
		perf_event__fprintf_comm(event, stdout);
>>>>>>> refs/remotes/origin/cm-10.0

	if (thread == NULL || thread__set_comm(thread, event->comm.comm)) {
		dump_printf("problem processing PERF_RECORD_COMM, skipping event.\n");
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
int perf_event__process_lost(union perf_event *event,
			     struct perf_sample *sample __used,
			     struct perf_session *session)
{
	dump_printf(": id:%" PRIu64 ": lost:%" PRIu64 "\n",
		    event->lost.id, event->lost.lost);
	session->hists.stats.total_lost += event->lost.lost;
=======
int perf_event__process_lost(struct perf_tool *tool __used,
			     union perf_event *event,
			     struct perf_sample *sample __used,
			     struct machine *machine __used)
{
	dump_printf(": id:%" PRIu64 ": lost:%" PRIu64 "\n",
		    event->lost.id, event->lost.lost);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static void perf_event__set_kernel_mmap_len(union perf_event *event,
					    struct map **maps)
{
	maps[MAP__FUNCTION]->start = event->mmap.start;
	maps[MAP__FUNCTION]->end   = event->mmap.start + event->mmap.len;
	/*
	 * Be a bit paranoid here, some perf.data file came with
	 * a zero sized synthesized MMAP event for the kernel.
	 */
	if (maps[MAP__FUNCTION]->end == 0)
		maps[MAP__FUNCTION]->end = ~0ULL;
}

<<<<<<< HEAD
static int perf_event__process_kernel_mmap(union perf_event *event,
					   struct perf_session *session)
{
	struct map *map;
	char kmmap_prefix[PATH_MAX];
	struct machine *machine;
	enum dso_kernel_type kernel_type;
	bool is_kernel_mmap;

	machine = perf_session__findnew_machine(session, event->mmap.pid);
	if (!machine) {
		pr_err("Can't find id %d's machine\n", event->mmap.pid);
		goto out_problem;
	}

=======
static int perf_event__process_kernel_mmap(struct perf_tool *tool __used,
					   union perf_event *event,
					   struct machine *machine)
{
	struct map *map;
	char kmmap_prefix[PATH_MAX];
	enum dso_kernel_type kernel_type;
	bool is_kernel_mmap;

>>>>>>> refs/remotes/origin/cm-10.0
	machine__mmap_name(machine, kmmap_prefix, sizeof(kmmap_prefix));
	if (machine__is_host(machine))
		kernel_type = DSO_TYPE_KERNEL;
	else
		kernel_type = DSO_TYPE_GUEST_KERNEL;

	is_kernel_mmap = memcmp(event->mmap.filename,
				kmmap_prefix,
<<<<<<< HEAD
				strlen(kmmap_prefix)) == 0;
=======
				strlen(kmmap_prefix) - 1) == 0;
>>>>>>> refs/remotes/origin/cm-10.0
	if (event->mmap.filename[0] == '/' ||
	    (!is_kernel_mmap && event->mmap.filename[0] == '[')) {

		char short_module_name[1024];
		char *name, *dot;

		if (event->mmap.filename[0] == '/') {
			name = strrchr(event->mmap.filename, '/');
			if (name == NULL)
				goto out_problem;

			++name; /* skip / */
			dot = strrchr(name, '.');
			if (dot == NULL)
				goto out_problem;
			snprintf(short_module_name, sizeof(short_module_name),
					"[%.*s]", (int)(dot - name), name);
			strxfrchar(short_module_name, '-', '_');
		} else
			strcpy(short_module_name, event->mmap.filename);

		map = machine__new_module(machine, event->mmap.start,
					  event->mmap.filename);
		if (map == NULL)
			goto out_problem;

		name = strdup(short_module_name);
		if (name == NULL)
			goto out_problem;

		map->dso->short_name = name;
		map->dso->sname_alloc = 1;
		map->end = map->start + event->mmap.len;
	} else if (is_kernel_mmap) {
		const char *symbol_name = (event->mmap.filename +
				strlen(kmmap_prefix));
		/*
		 * Should be there already, from the build-id table in
		 * the header.
		 */
		struct dso *kernel = __dsos__findnew(&machine->kernel_dsos,
						     kmmap_prefix);
		if (kernel == NULL)
			goto out_problem;

		kernel->kernel = kernel_type;
		if (__machine__create_kernel_maps(machine, kernel) < 0)
			goto out_problem;

		perf_event__set_kernel_mmap_len(event, machine->vmlinux_maps);

		/*
		 * Avoid using a zero address (kptr_restrict) for the ref reloc
		 * symbol. Effectively having zero here means that at record
		 * time /proc/sys/kernel/kptr_restrict was non zero.
		 */
		if (event->mmap.pgoff != 0) {
<<<<<<< HEAD
			perf_session__set_kallsyms_ref_reloc_sym(machine->vmlinux_maps,
								 symbol_name,
								 event->mmap.pgoff);
=======
			maps__set_kallsyms_ref_reloc_sym(machine->vmlinux_maps,
							 symbol_name,
							 event->mmap.pgoff);
>>>>>>> refs/remotes/origin/cm-10.0
		}

		if (machine__is_default_guest(machine)) {
			/*
			 * preload dso of guest kernel and modules
			 */
			dso__load(kernel, machine->vmlinux_maps[MAP__FUNCTION],
				  NULL);
		}
	}
	return 0;
out_problem:
	return -1;
}

<<<<<<< HEAD
int perf_event__process_mmap(union perf_event *event,
			     struct perf_sample *sample __used,
			     struct perf_session *session)
{
	struct machine *machine;
=======
size_t perf_event__fprintf_mmap(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " %d/%d: [%#" PRIx64 "(%#" PRIx64 ") @ %#" PRIx64 "]: %s\n",
		       event->mmap.pid, event->mmap.tid, event->mmap.start,
		       event->mmap.len, event->mmap.pgoff, event->mmap.filename);
}

int perf_event__process_mmap(struct perf_tool *tool,
			     union perf_event *event,
			     struct perf_sample *sample __used,
			     struct machine *machine)
{
>>>>>>> refs/remotes/origin/cm-10.0
	struct thread *thread;
	struct map *map;
	u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;
	int ret = 0;

<<<<<<< HEAD
	dump_printf(" %d/%d: [%#" PRIx64 "(%#" PRIx64 ") @ %#" PRIx64 "]: %s\n",
			event->mmap.pid, event->mmap.tid, event->mmap.start,
			event->mmap.len, event->mmap.pgoff, event->mmap.filename);

	if (cpumode == PERF_RECORD_MISC_GUEST_KERNEL ||
	    cpumode == PERF_RECORD_MISC_KERNEL) {
		ret = perf_event__process_kernel_mmap(event, session);
=======
	if (dump_trace)
		perf_event__fprintf_mmap(event, stdout);

	if (cpumode == PERF_RECORD_MISC_GUEST_KERNEL ||
	    cpumode == PERF_RECORD_MISC_KERNEL) {
		ret = perf_event__process_kernel_mmap(tool, event, machine);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret < 0)
			goto out_problem;
		return 0;
	}

<<<<<<< HEAD
	machine = perf_session__find_host_machine(session);
	if (machine == NULL)
		goto out_problem;
	thread = perf_session__findnew(session, event->mmap.pid);
=======
	thread = machine__findnew_thread(machine, event->mmap.pid);
>>>>>>> refs/remotes/origin/cm-10.0
	if (thread == NULL)
		goto out_problem;
	map = map__new(&machine->user_dsos, event->mmap.start,
			event->mmap.len, event->mmap.pgoff,
			event->mmap.pid, event->mmap.filename,
			MAP__FUNCTION);
	if (map == NULL)
		goto out_problem;

	thread__insert_map(thread, map);
	return 0;

out_problem:
	dump_printf("problem processing PERF_RECORD_MMAP, skipping event.\n");
	return 0;
}

<<<<<<< HEAD
int perf_event__process_task(union perf_event *event,
			     struct perf_sample *sample __used,
			     struct perf_session *session)
{
	struct thread *thread = perf_session__findnew(session, event->fork.tid);
	struct thread *parent = perf_session__findnew(session, event->fork.ptid);

	dump_printf("(%d:%d):(%d:%d)\n", event->fork.pid, event->fork.tid,
		    event->fork.ppid, event->fork.ptid);

	if (event->header.type == PERF_RECORD_EXIT) {
		perf_session__remove_thread(session, thread);
=======
=======
int perf_event__process_comm(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_comm_event(machine, event, sample);
}

int perf_event__process_lost(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_lost_event(machine, event, sample);
}

size_t perf_event__fprintf_mmap(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " %d/%d: [%#" PRIx64 "(%#" PRIx64 ") @ %#" PRIx64 "]: %c %s\n",
		       event->mmap.pid, event->mmap.tid, event->mmap.start,
		       event->mmap.len, event->mmap.pgoff,
		       (event->header.misc & PERF_RECORD_MISC_MMAP_DATA) ? 'r' : 'x',
		       event->mmap.filename);
}

size_t perf_event__fprintf_mmap2(union perf_event *event, FILE *fp)
{
	return fprintf(fp, " %d/%d: [%#" PRIx64 "(%#" PRIx64 ") @ %#" PRIx64
			   " %02x:%02x %"PRIu64" %"PRIu64"]: %c %s\n",
		       event->mmap2.pid, event->mmap2.tid, event->mmap2.start,
		       event->mmap2.len, event->mmap2.pgoff, event->mmap2.maj,
		       event->mmap2.min, event->mmap2.ino,
		       event->mmap2.ino_generation,
		       (event->header.misc & PERF_RECORD_MISC_MMAP_DATA) ? 'r' : 'x',
		       event->mmap2.filename);
}

int perf_event__process_mmap(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_mmap_event(machine, event, sample);
}

int perf_event__process_mmap2(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_mmap2_event(machine, event, sample);
}

>>>>>>> refs/remotes/origin/master
size_t perf_event__fprintf_task(union perf_event *event, FILE *fp)
{
	return fprintf(fp, "(%d:%d):(%d:%d)\n",
		       event->fork.pid, event->fork.tid,
		       event->fork.ppid, event->fork.ptid);
}

<<<<<<< HEAD
int perf_event__process_task(struct perf_tool *tool __used,
			     union perf_event *event,
			     struct perf_sample *sample __used,
			      struct machine *machine)
{
	struct thread *thread = machine__findnew_thread(machine, event->fork.tid);
	struct thread *parent = machine__findnew_thread(machine, event->fork.ptid);

	if (dump_trace)
		perf_event__fprintf_task(event, stdout);

	if (event->header.type == PERF_RECORD_EXIT) {
		machine__remove_thread(machine, thread);
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;
	}

	if (thread == NULL || parent == NULL ||
	    thread__fork(thread, parent) < 0) {
		dump_printf("problem processing PERF_RECORD_FORK, skipping event.\n");
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
int perf_event__process(union perf_event *event, struct perf_sample *sample,
			struct perf_session *session)
{
	switch (event->header.type) {
	case PERF_RECORD_COMM:
		perf_event__process_comm(event, sample, session);
		break;
	case PERF_RECORD_MMAP:
		perf_event__process_mmap(event, sample, session);
		break;
	case PERF_RECORD_FORK:
	case PERF_RECORD_EXIT:
		perf_event__process_task(event, sample, session);
		break;
	case PERF_RECORD_LOST:
		perf_event__process_lost(event, sample, session);
=======
=======
int perf_event__process_fork(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_fork_event(machine, event, sample);
}

int perf_event__process_exit(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_sample *sample,
			     struct machine *machine)
{
	return machine__process_exit_event(machine, event, sample);
}

>>>>>>> refs/remotes/origin/master
size_t perf_event__fprintf(union perf_event *event, FILE *fp)
{
	size_t ret = fprintf(fp, "PERF_RECORD_%s",
			     perf_event__name(event->header.type));

	switch (event->header.type) {
	case PERF_RECORD_COMM:
		ret += perf_event__fprintf_comm(event, fp);
		break;
	case PERF_RECORD_FORK:
	case PERF_RECORD_EXIT:
		ret += perf_event__fprintf_task(event, fp);
		break;
	case PERF_RECORD_MMAP:
		ret += perf_event__fprintf_mmap(event, fp);
		break;
<<<<<<< HEAD
=======
	case PERF_RECORD_MMAP2:
		ret += perf_event__fprintf_mmap2(event, fp);
		break;
>>>>>>> refs/remotes/origin/master
	default:
		ret += fprintf(fp, "\n");
	}

	return ret;
}

<<<<<<< HEAD
int perf_event__process(struct perf_tool *tool, union perf_event *event,
			struct perf_sample *sample, struct machine *machine)
{
	switch (event->header.type) {
	case PERF_RECORD_COMM:
		perf_event__process_comm(tool, event, sample, machine);
		break;
	case PERF_RECORD_MMAP:
		perf_event__process_mmap(tool, event, sample, machine);
		break;
	case PERF_RECORD_FORK:
	case PERF_RECORD_EXIT:
		perf_event__process_task(tool, event, sample, machine);
		break;
	case PERF_RECORD_LOST:
		perf_event__process_lost(tool, event, sample, machine);
>>>>>>> refs/remotes/origin/cm-10.0
	default:
		break;
	}

	return 0;
}

void thread__find_addr_map(struct thread *self,
<<<<<<< HEAD
			   struct perf_session *session, u8 cpumode,
			   enum map_type type, pid_t pid, u64 addr,
			   struct addr_location *al)
{
	struct map_groups *mg = &self->mg;
	struct machine *machine = NULL;
=======
=======
int perf_event__process(struct perf_tool *tool __maybe_unused,
			union perf_event *event,
			struct perf_sample *sample,
			struct machine *machine)
{
	return machine__process_event(machine, event, sample);
}

void thread__find_addr_map(struct thread *thread,
>>>>>>> refs/remotes/origin/master
			   struct machine *machine, u8 cpumode,
			   enum map_type type, u64 addr,
			   struct addr_location *al)
{
<<<<<<< HEAD
	struct map_groups *mg = &self->mg;
>>>>>>> refs/remotes/origin/cm-10.0

	al->thread = self;
=======
	struct map_groups *mg = &thread->mg;
	bool load_map = false;

	al->machine = machine;
	al->thread = thread;
>>>>>>> refs/remotes/origin/master
	al->addr = addr;
	al->cpumode = cpumode;
	al->filtered = false;

<<<<<<< HEAD
<<<<<<< HEAD
	if (cpumode == PERF_RECORD_MISC_KERNEL && perf_host) {
		al->level = 'k';
		machine = perf_session__find_host_machine(session);
		if (machine == NULL) {
			al->map = NULL;
			return;
		}
		mg = &machine->kmaps;
	} else if (cpumode == PERF_RECORD_MISC_USER && perf_host) {
		al->level = '.';
		machine = perf_session__find_host_machine(session);
	} else if (cpumode == PERF_RECORD_MISC_GUEST_KERNEL && perf_guest) {
		al->level = 'g';
		machine = perf_session__find_machine(session, pid);
		if (machine == NULL) {
			al->map = NULL;
			return;
		}
=======
=======
>>>>>>> refs/remotes/origin/master
	if (machine == NULL) {
		al->map = NULL;
		return;
	}

	if (cpumode == PERF_RECORD_MISC_KERNEL && perf_host) {
		al->level = 'k';
		mg = &machine->kmaps;
<<<<<<< HEAD
=======
		load_map = true;
>>>>>>> refs/remotes/origin/master
	} else if (cpumode == PERF_RECORD_MISC_USER && perf_host) {
		al->level = '.';
	} else if (cpumode == PERF_RECORD_MISC_GUEST_KERNEL && perf_guest) {
		al->level = 'g';
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		mg = &machine->kmaps;
	} else {
		/*
		 * 'u' means guest os user space.
		 * TODO: We don't support guest user space. Might support late.
		 */
		if (cpumode == PERF_RECORD_MISC_GUEST_USER && perf_guest)
			al->level = 'u';
		else
			al->level = 'H';
=======
		mg = &machine->kmaps;
		load_map = true;
	} else if (cpumode == PERF_RECORD_MISC_GUEST_USER && perf_guest) {
		al->level = 'u';
	} else {
		al->level = 'H';
>>>>>>> refs/remotes/origin/master
		al->map = NULL;

		if ((cpumode == PERF_RECORD_MISC_GUEST_USER ||
			cpumode == PERF_RECORD_MISC_GUEST_KERNEL) &&
			!perf_guest)
			al->filtered = true;
		if ((cpumode == PERF_RECORD_MISC_USER ||
			cpumode == PERF_RECORD_MISC_KERNEL) &&
			!perf_host)
			al->filtered = true;

		return;
	}
try_again:
	al->map = map_groups__find(mg, type, al->addr);
	if (al->map == NULL) {
		/*
		 * If this is outside of all known maps, and is a negative
		 * address, try to look it up in the kernel dso, as it might be
		 * a vsyscall or vdso (which executes in user-mode).
		 *
		 * XXX This is nasty, we should have a symbol list in the
		 * "[vdso]" dso, but for now lets use the old trick of looking
		 * in the whole kernel symbol list.
		 */
		if ((long long)al->addr < 0 &&
		    cpumode == PERF_RECORD_MISC_USER &&
		    machine && mg != &machine->kmaps) {
			mg = &machine->kmaps;
			goto try_again;
		}
<<<<<<< HEAD
	} else
		al->addr = al->map->map_ip(al->map, al->addr);
}

<<<<<<< HEAD
void thread__find_addr_location(struct thread *self,
				struct perf_session *session, u8 cpumode,
				enum map_type type, pid_t pid, u64 addr,
				struct addr_location *al,
				symbol_filter_t filter)
{
	thread__find_addr_map(self, session, cpumode, type, pid, addr, al);
=======
void thread__find_addr_location(struct thread *thread, struct machine *machine,
				u8 cpumode, enum map_type type, u64 addr,
				struct addr_location *al,
				symbol_filter_t filter)
{
	thread__find_addr_map(thread, machine, cpumode, type, addr, al);
>>>>>>> refs/remotes/origin/cm-10.0
	if (al->map != NULL)
		al->sym = map__find_symbol(al->map, al->addr, filter);
=======
	} else {
		/*
		 * Kernel maps might be changed when loading symbols so loading
		 * must be done prior to using kernel maps.
		 */
		if (load_map)
			map__load(al->map, machine->symbol_filter);
		al->addr = al->map->map_ip(al->map, al->addr);
	}
}

void thread__find_addr_location(struct thread *thread, struct machine *machine,
				u8 cpumode, enum map_type type, u64 addr,
				struct addr_location *al)
{
	thread__find_addr_map(thread, machine, cpumode, type, addr, al);
	if (al->map != NULL)
		al->sym = map__find_symbol(al->map, al->addr,
					   machine->symbol_filter);
>>>>>>> refs/remotes/origin/master
	else
		al->sym = NULL;
}

int perf_event__preprocess_sample(const union perf_event *event,
<<<<<<< HEAD
<<<<<<< HEAD
				  struct perf_session *session,
=======
				  struct machine *machine,
>>>>>>> refs/remotes/origin/cm-10.0
				  struct addr_location *al,
				  struct perf_sample *sample,
				  symbol_filter_t filter)
{
	u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;
<<<<<<< HEAD
	struct thread *thread = perf_session__findnew(session, event->ip.pid);
=======
	struct thread *thread = machine__findnew_thread(machine, event->ip.pid);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				  struct machine *machine,
				  struct addr_location *al,
				  struct perf_sample *sample)
{
	u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;
	struct thread *thread = machine__findnew_thread(machine, sample->pid,
							sample->pid);
>>>>>>> refs/remotes/origin/master

	if (thread == NULL)
		return -1;

<<<<<<< HEAD
	if (symbol_conf.comm_list &&
	    !strlist__has_entry(symbol_conf.comm_list, thread->comm))
		goto out_filtered;

	dump_printf(" ... thread: %s:%d\n", thread->comm, thread->pid);
	/*
<<<<<<< HEAD
	 * Have we already created the kernel maps for the host machine?
=======
	 * Have we already created the kernel maps for this machine?
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (thread__is_filtered(thread))
		goto out_filtered;

	dump_printf(" ... thread: %s:%d\n", thread__comm_str(thread), thread->tid);
	/*
	 * Have we already created the kernel maps for this machine?
>>>>>>> refs/remotes/origin/master
	 *
	 * This should have happened earlier, when we processed the kernel MMAP
	 * events, but for older perf.data files there was no such thing, so do
	 * it now.
	 */
	if (cpumode == PERF_RECORD_MISC_KERNEL &&
<<<<<<< HEAD
<<<<<<< HEAD
	    session->host_machine.vmlinux_maps[MAP__FUNCTION] == NULL)
		machine__create_kernel_maps(&session->host_machine);

	thread__find_addr_map(thread, session, cpumode, MAP__FUNCTION,
			      event->ip.pid, event->ip.ip, al);
=======
=======
>>>>>>> refs/remotes/origin/master
	    machine->vmlinux_maps[MAP__FUNCTION] == NULL)
		machine__create_kernel_maps(machine);

	thread__find_addr_map(thread, machine, cpumode, MAP__FUNCTION,
<<<<<<< HEAD
			      event->ip.ip, al);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			      sample->ip, al);
>>>>>>> refs/remotes/origin/master
	dump_printf(" ...... dso: %s\n",
		    al->map ? al->map->dso->long_name :
			al->level == 'H' ? "[hypervisor]" : "<not found>");
	al->sym = NULL;
	al->cpu = sample->cpu;

	if (al->map) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (symbol_conf.dso_list &&
		    (!al->map || !al->map->dso ||
		     !(strlist__has_entry(symbol_conf.dso_list,
					  al->map->dso->short_name) ||
		       (al->map->dso->short_name != al->map->dso->long_name &&
			strlist__has_entry(symbol_conf.dso_list,
					   al->map->dso->long_name)))))
=======
=======
>>>>>>> refs/remotes/origin/master
		struct dso *dso = al->map->dso;

		if (symbol_conf.dso_list &&
		    (!dso || !(strlist__has_entry(symbol_conf.dso_list,
						  dso->short_name) ||
			       (dso->short_name != dso->long_name &&
				strlist__has_entry(symbol_conf.dso_list,
						   dso->long_name)))))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			goto out_filtered;

		al->sym = map__find_symbol(al->map, al->addr, filter);
	}

	if (symbol_conf.sym_list && al->sym &&
	    !strlist__has_entry(symbol_conf.sym_list, al->sym->name))
=======
			goto out_filtered;

		al->sym = map__find_symbol(al->map, al->addr,
					   machine->symbol_filter);
	}

	if (symbol_conf.sym_list &&
		(!al->sym || !strlist__has_entry(symbol_conf.sym_list,
						al->sym->name)))
>>>>>>> refs/remotes/origin/master
		goto out_filtered;

	return 0;

out_filtered:
	al->filtered = true;
	return 0;
}
