/*
 * Copyright (C) 2011, Red Hat Inc, Arnaldo Carvalho de Melo <acme@redhat.com>
 *
 * Parts came from builtin-{top,stat,record}.c, see those files for further
 * copyright notes.
 *
 * Released under the GPL v2. (and only v2, not any later version)
 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "util.h"
#include "debugfs.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include <poll.h>
#include "cpumap.h"
#include "thread_map.h"
#include "evlist.h"
#include "evsel.h"
<<<<<<< HEAD
#include "util.h"
=======
#include <unistd.h>

#include "parse-events.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "util.h"
#include <api/fs/debugfs.h>
#include <poll.h>
#include "cpumap.h"
#include "thread_map.h"
#include "target.h"
#include "evlist.h"
#include "evsel.h"
#include "debug.h"
#include <unistd.h>

#include "parse-events.h"
#include "parse-options.h"
>>>>>>> refs/remotes/origin/master

#include <sys/mman.h>

#include <linux/bitops.h>
#include <linux/hash.h>

#define FD(e, x, y) (*(int *)xyarray__entry(e->fd, x, y))
#define SID(e, x, y) xyarray__entry(e->sample_id, x, y)

void perf_evlist__init(struct perf_evlist *evlist, struct cpu_map *cpus,
		       struct thread_map *threads)
{
	int i;

	for (i = 0; i < PERF_EVLIST__HLIST_SIZE; ++i)
		INIT_HLIST_HEAD(&evlist->heads[i]);
	INIT_LIST_HEAD(&evlist->entries);
	perf_evlist__set_maps(evlist, cpus, threads);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	evlist->workload.pid = -1;
>>>>>>> refs/remotes/origin/cm-10.0
}

struct perf_evlist *perf_evlist__new(struct cpu_map *cpus,
				     struct thread_map *threads)
=======
	evlist->workload.pid = -1;
}

struct perf_evlist *perf_evlist__new(void)
>>>>>>> refs/remotes/origin/master
{
	struct perf_evlist *evlist = zalloc(sizeof(*evlist));

	if (evlist != NULL)
<<<<<<< HEAD
		perf_evlist__init(evlist, cpus, threads);
=======
		perf_evlist__init(evlist, NULL, NULL);
>>>>>>> refs/remotes/origin/master

	return evlist;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
void perf_evlist__config_attrs(struct perf_evlist *evlist,
			       struct perf_record_opts *opts)
{
	struct perf_evsel *evsel, *first;

	if (evlist->cpus->map[0] < 0)
		opts->no_inherit = true;

	first = list_entry(evlist->entries.next, struct perf_evsel, node);

	list_for_each_entry(evsel, &evlist->entries, node) {
		perf_evsel__config(evsel, opts, first);

		if (evlist->nr_entries > 1)
			evsel->attr.sample_type |= PERF_SAMPLE_ID;
	}
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
struct perf_evlist *perf_evlist__new_default(void)
{
	struct perf_evlist *evlist = perf_evlist__new();

	if (evlist && perf_evlist__add_default(evlist)) {
		perf_evlist__delete(evlist);
		evlist = NULL;
	}

	return evlist;
}

/**
 * perf_evlist__set_id_pos - set the positions of event ids.
 * @evlist: selected event list
 *
 * Events with compatible sample types all have the same id_pos
 * and is_pos.  For convenience, put a copy on evlist.
 */
void perf_evlist__set_id_pos(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist);

	evlist->id_pos = first->id_pos;
	evlist->is_pos = first->is_pos;
}

static void perf_evlist__update_id_pos(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;

	evlist__for_each(evlist, evsel)
		perf_evsel__calc_id_pos(evsel);

	perf_evlist__set_id_pos(evlist);
}

>>>>>>> refs/remotes/origin/master
static void perf_evlist__purge(struct perf_evlist *evlist)
{
	struct perf_evsel *pos, *n;

<<<<<<< HEAD
	list_for_each_entry_safe(pos, n, &evlist->entries, node) {
=======
	evlist__for_each_safe(evlist, n, pos) {
>>>>>>> refs/remotes/origin/master
		list_del_init(&pos->node);
		perf_evsel__delete(pos);
	}

	evlist->nr_entries = 0;
}

void perf_evlist__exit(struct perf_evlist *evlist)
{
<<<<<<< HEAD
	free(evlist->mmap);
	free(evlist->pollfd);
	evlist->mmap = NULL;
	evlist->pollfd = NULL;
=======
	zfree(&evlist->mmap);
	zfree(&evlist->pollfd);
>>>>>>> refs/remotes/origin/master
}

void perf_evlist__delete(struct perf_evlist *evlist)
{
<<<<<<< HEAD
=======
	perf_evlist__munmap(evlist);
	perf_evlist__close(evlist);
	cpu_map__delete(evlist->cpus);
	thread_map__delete(evlist->threads);
	evlist->cpus = NULL;
	evlist->threads = NULL;
>>>>>>> refs/remotes/origin/master
	perf_evlist__purge(evlist);
	perf_evlist__exit(evlist);
	free(evlist);
}

void perf_evlist__add(struct perf_evlist *evlist, struct perf_evsel *entry)
{
	list_add_tail(&entry->node, &evlist->entries);
<<<<<<< HEAD
	++evlist->nr_entries;
}

<<<<<<< HEAD
=======
=======
	entry->idx = evlist->nr_entries;

	if (!evlist->nr_entries++)
		perf_evlist__set_id_pos(evlist);
}

>>>>>>> refs/remotes/origin/master
void perf_evlist__splice_list_tail(struct perf_evlist *evlist,
				   struct list_head *list,
				   int nr_entries)
{
<<<<<<< HEAD
	list_splice_tail(list, &evlist->entries);
	evlist->nr_entries += nr_entries;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool set_id_pos = !evlist->nr_entries;

	list_splice_tail(list, &evlist->entries);
	evlist->nr_entries += nr_entries;
	if (set_id_pos)
		perf_evlist__set_id_pos(evlist);
}

void __perf_evlist__set_leader(struct list_head *list)
{
	struct perf_evsel *evsel, *leader;

	leader = list_entry(list->next, struct perf_evsel, node);
	evsel = list_entry(list->prev, struct perf_evsel, node);

	leader->nr_members = evsel->idx - leader->idx + 1;

	__evlist__for_each(list, evsel) {
		evsel->leader = leader;
	}
}

void perf_evlist__set_leader(struct perf_evlist *evlist)
{
	if (evlist->nr_entries) {
		evlist->nr_groups = evlist->nr_entries > 1 ? 1 : 0;
		__perf_evlist__set_leader(&evlist->entries);
	}
}

>>>>>>> refs/remotes/origin/master
int perf_evlist__add_default(struct perf_evlist *evlist)
{
	struct perf_event_attr attr = {
		.type = PERF_TYPE_HARDWARE,
		.config = PERF_COUNT_HW_CPU_CYCLES,
	};
<<<<<<< HEAD
<<<<<<< HEAD
	struct perf_evsel *evsel = perf_evsel__new(&attr, 0);

	if (evsel == NULL)
		return -ENOMEM;

	perf_evlist__add(evlist, evsel);
	return 0;
}

int perf_evlist__alloc_pollfd(struct perf_evlist *evlist)
=======
=======
>>>>>>> refs/remotes/origin/master
	struct perf_evsel *evsel;

	event_attr_init(&attr);

<<<<<<< HEAD
	evsel = perf_evsel__new(&attr, 0);
=======
	evsel = perf_evsel__new(&attr);
>>>>>>> refs/remotes/origin/master
	if (evsel == NULL)
		goto error;

	/* use strdup() because free(evsel) assumes name is allocated */
	evsel->name = strdup("cycles");
	if (!evsel->name)
		goto error_free;

	perf_evlist__add(evlist, evsel);
	return 0;
error_free:
	perf_evsel__delete(evsel);
error:
	return -ENOMEM;
}

<<<<<<< HEAD
int perf_evlist__add_attrs(struct perf_evlist *evlist,
			   struct perf_event_attr *attrs, size_t nr_attrs)
=======
static int perf_evlist__add_attrs(struct perf_evlist *evlist,
				  struct perf_event_attr *attrs, size_t nr_attrs)
>>>>>>> refs/remotes/origin/master
{
	struct perf_evsel *evsel, *n;
	LIST_HEAD(head);
	size_t i;

	for (i = 0; i < nr_attrs; i++) {
<<<<<<< HEAD
		evsel = perf_evsel__new(attrs + i, evlist->nr_entries + i);
=======
		evsel = perf_evsel__new_idx(attrs + i, evlist->nr_entries + i);
>>>>>>> refs/remotes/origin/master
		if (evsel == NULL)
			goto out_delete_partial_list;
		list_add_tail(&evsel->node, &head);
	}

	perf_evlist__splice_list_tail(evlist, &head, nr_attrs);

	return 0;

out_delete_partial_list:
<<<<<<< HEAD
	list_for_each_entry_safe(evsel, n, &head, node)
=======
	__evlist__for_each_safe(&head, n, evsel)
>>>>>>> refs/remotes/origin/master
		perf_evsel__delete(evsel);
	return -1;
}

<<<<<<< HEAD
static int trace_event__id(const char *evname)
{
	char *filename, *colon;
	int err = -1, fd;

	if (asprintf(&filename, "%s/%s/id", tracing_events_path, evname) < 0)
		return -1;

	colon = strrchr(filename, ':');
	if (colon != NULL)
		*colon = '/';

	fd = open(filename, O_RDONLY);
	if (fd >= 0) {
		char id[16];
		if (read(fd, id, sizeof(id)) > 0)
			err = atoi(id);
		close(fd);
	}

	free(filename);
	return err;
}

int perf_evlist__add_tracepoints(struct perf_evlist *evlist,
				 const char *tracepoints[],
				 size_t nr_tracepoints)
{
	int err;
	size_t i;
	struct perf_event_attr *attrs = zalloc(nr_tracepoints * sizeof(*attrs));

	if (attrs == NULL)
		return -1;

	for (i = 0; i < nr_tracepoints; i++) {
		err = trace_event__id(tracepoints[i]);

		if (err < 0)
			goto out_free_attrs;

		attrs[i].type	       = PERF_TYPE_TRACEPOINT;
		attrs[i].config	       = err;
	        attrs[i].sample_type   = (PERF_SAMPLE_RAW | PERF_SAMPLE_TIME |
					  PERF_SAMPLE_CPU);
		attrs[i].sample_period = 1;
	}

	err = perf_evlist__add_attrs(evlist, attrs, nr_tracepoints);
out_free_attrs:
	free(attrs);
	return err;
}

static struct perf_evsel *
	perf_evlist__find_tracepoint_by_id(struct perf_evlist *evlist, int id)
{
	struct perf_evsel *evsel;

	list_for_each_entry(evsel, &evlist->entries, node) {
=======
int __perf_evlist__add_default_attrs(struct perf_evlist *evlist,
				     struct perf_event_attr *attrs, size_t nr_attrs)
{
	size_t i;

	for (i = 0; i < nr_attrs; i++)
		event_attr_init(attrs + i);

	return perf_evlist__add_attrs(evlist, attrs, nr_attrs);
}

struct perf_evsel *
perf_evlist__find_tracepoint_by_id(struct perf_evlist *evlist, int id)
{
	struct perf_evsel *evsel;

	evlist__for_each(evlist, evsel) {
>>>>>>> refs/remotes/origin/master
		if (evsel->attr.type   == PERF_TYPE_TRACEPOINT &&
		    (int)evsel->attr.config == id)
			return evsel;
	}

	return NULL;
}

<<<<<<< HEAD
int perf_evlist__set_tracepoints_handlers(struct perf_evlist *evlist,
					  const struct perf_evsel_str_handler *assocs,
					  size_t nr_assocs)
{
	struct perf_evsel *evsel;
	int err;
	size_t i;

	for (i = 0; i < nr_assocs; i++) {
		err = trace_event__id(assocs[i].name);
		if (err < 0)
			goto out;

		evsel = perf_evlist__find_tracepoint_by_id(evlist, err);
		if (evsel == NULL)
			continue;

		err = -EEXIST;
		if (evsel->handler.func != NULL)
			goto out;
		evsel->handler.func = assocs[i].handler;
	}

	err = 0;
out:
	return err;
=======
struct perf_evsel *
perf_evlist__find_tracepoint_by_name(struct perf_evlist *evlist,
				     const char *name)
{
	struct perf_evsel *evsel;

	evlist__for_each(evlist, evsel) {
		if ((evsel->attr.type == PERF_TYPE_TRACEPOINT) &&
		    (strcmp(evsel->name, name) == 0))
			return evsel;
	}

	return NULL;
}

int perf_evlist__add_newtp(struct perf_evlist *evlist,
			   const char *sys, const char *name, void *handler)
{
	struct perf_evsel *evsel = perf_evsel__newtp(sys, name);

	if (evsel == NULL)
		return -1;

	evsel->handler = handler;
	perf_evlist__add(evlist, evsel);
	return 0;
>>>>>>> refs/remotes/origin/master
}

void perf_evlist__disable(struct perf_evlist *evlist)
{
	int cpu, thread;
	struct perf_evsel *pos;
<<<<<<< HEAD

	for (cpu = 0; cpu < evlist->cpus->nr; cpu++) {
		list_for_each_entry(pos, &evlist->entries, node) {
			for (thread = 0; thread < evlist->threads->nr; thread++)
				ioctl(FD(pos, cpu, thread), PERF_EVENT_IOC_DISABLE);
=======
	int nr_cpus = cpu_map__nr(evlist->cpus);
	int nr_threads = thread_map__nr(evlist->threads);

	for (cpu = 0; cpu < nr_cpus; cpu++) {
		evlist__for_each(evlist, pos) {
			if (!perf_evsel__is_group_leader(pos) || !pos->fd)
				continue;
			for (thread = 0; thread < nr_threads; thread++)
				ioctl(FD(pos, cpu, thread),
				      PERF_EVENT_IOC_DISABLE, 0);
>>>>>>> refs/remotes/origin/master
		}
	}
}

void perf_evlist__enable(struct perf_evlist *evlist)
{
	int cpu, thread;
	struct perf_evsel *pos;
<<<<<<< HEAD

	for (cpu = 0; cpu < evlist->cpus->nr; cpu++) {
		list_for_each_entry(pos, &evlist->entries, node) {
			for (thread = 0; thread < evlist->threads->nr; thread++)
				ioctl(FD(pos, cpu, thread), PERF_EVENT_IOC_ENABLE);
		}
	}
}

static int perf_evlist__alloc_pollfd(struct perf_evlist *evlist)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int nfds = evlist->cpus->nr * evlist->threads->nr * evlist->nr_entries;
=======
	int nr_cpus = cpu_map__nr(evlist->cpus);
	int nr_threads = thread_map__nr(evlist->threads);

	for (cpu = 0; cpu < nr_cpus; cpu++) {
		evlist__for_each(evlist, pos) {
			if (!perf_evsel__is_group_leader(pos) || !pos->fd)
				continue;
			for (thread = 0; thread < nr_threads; thread++)
				ioctl(FD(pos, cpu, thread),
				      PERF_EVENT_IOC_ENABLE, 0);
		}
	}
}

int perf_evlist__disable_event(struct perf_evlist *evlist,
			       struct perf_evsel *evsel)
{
	int cpu, thread, err;

	if (!evsel->fd)
		return 0;

	for (cpu = 0; cpu < evlist->cpus->nr; cpu++) {
		for (thread = 0; thread < evlist->threads->nr; thread++) {
			err = ioctl(FD(evsel, cpu, thread),
				    PERF_EVENT_IOC_DISABLE, 0);
			if (err)
				return err;
		}
	}
	return 0;
}

int perf_evlist__enable_event(struct perf_evlist *evlist,
			      struct perf_evsel *evsel)
{
	int cpu, thread, err;

	if (!evsel->fd)
		return -EINVAL;

	for (cpu = 0; cpu < evlist->cpus->nr; cpu++) {
		for (thread = 0; thread < evlist->threads->nr; thread++) {
			err = ioctl(FD(evsel, cpu, thread),
				    PERF_EVENT_IOC_ENABLE, 0);
			if (err)
				return err;
		}
	}
	return 0;
}

static int perf_evlist__alloc_pollfd(struct perf_evlist *evlist)
{
	int nr_cpus = cpu_map__nr(evlist->cpus);
	int nr_threads = thread_map__nr(evlist->threads);
	int nfds = nr_cpus * nr_threads * evlist->nr_entries;
>>>>>>> refs/remotes/origin/master
	evlist->pollfd = malloc(sizeof(struct pollfd) * nfds);
	return evlist->pollfd != NULL ? 0 : -ENOMEM;
}

void perf_evlist__add_pollfd(struct perf_evlist *evlist, int fd)
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
	evlist->pollfd[evlist->nr_fds].fd = fd;
	evlist->pollfd[evlist->nr_fds].events = POLLIN;
	evlist->nr_fds++;
}

static void perf_evlist__id_hash(struct perf_evlist *evlist,
				 struct perf_evsel *evsel,
				 int cpu, int thread, u64 id)
{
	int hash;
	struct perf_sample_id *sid = SID(evsel, cpu, thread);

	sid->id = id;
	sid->evsel = evsel;
	hash = hash_64(sid->id, PERF_EVLIST__HLIST_BITS);
	hlist_add_head(&sid->node, &evlist->heads[hash]);
}

void perf_evlist__id_add(struct perf_evlist *evlist, struct perf_evsel *evsel,
			 int cpu, int thread, u64 id)
{
	perf_evlist__id_hash(evlist, evsel, cpu, thread, id);
	evsel->id[evsel->ids++] = id;
}

static int perf_evlist__id_add_fd(struct perf_evlist *evlist,
				  struct perf_evsel *evsel,
				  int cpu, int thread, int fd)
{
	u64 read_data[4] = { 0, };
	int id_idx = 1; /* The first entry is the counter value */
<<<<<<< HEAD
=======
	u64 id;
	int ret;

	ret = ioctl(fd, PERF_EVENT_IOC_ID, &id);
	if (!ret)
		goto add;

	if (errno != ENOTTY)
		return -1;

	/* Legacy way to get event id.. All hail to old kernels! */

	/*
	 * This way does not work with group format read, so bail
	 * out in that case.
	 */
	if (perf_evlist__read_format(evlist) & PERF_FORMAT_GROUP)
		return -1;
>>>>>>> refs/remotes/origin/master

	if (!(evsel->attr.read_format & PERF_FORMAT_ID) ||
	    read(fd, &read_data, sizeof(read_data)) == -1)
		return -1;

	if (evsel->attr.read_format & PERF_FORMAT_TOTAL_TIME_ENABLED)
		++id_idx;
	if (evsel->attr.read_format & PERF_FORMAT_TOTAL_TIME_RUNNING)
		++id_idx;

<<<<<<< HEAD
	perf_evlist__id_add(evlist, evsel, cpu, thread, read_data[id_idx]);
	return 0;
}

struct perf_evsel *perf_evlist__id2evsel(struct perf_evlist *evlist, u64 id)
{
	struct hlist_head *head;
	struct hlist_node *pos;
	struct perf_sample_id *sid;
	int hash;

	if (evlist->nr_entries == 1)
		return list_entry(evlist->entries.next, struct perf_evsel, node);
=======
	id = read_data[id_idx];

 add:
	perf_evlist__id_add(evlist, evsel, cpu, thread, id);
	return 0;
}

struct perf_sample_id *perf_evlist__id2sid(struct perf_evlist *evlist, u64 id)
{
	struct hlist_head *head;
	struct perf_sample_id *sid;
	int hash;

	hash = hash_64(id, PERF_EVLIST__HLIST_BITS);
	head = &evlist->heads[hash];

	hlist_for_each_entry(sid, head, node)
		if (sid->id == id)
			return sid;

	return NULL;
}

struct perf_evsel *perf_evlist__id2evsel(struct perf_evlist *evlist, u64 id)
{
	struct perf_sample_id *sid;

	if (evlist->nr_entries == 1)
		return perf_evlist__first(evlist);

	sid = perf_evlist__id2sid(evlist, id);
	if (sid)
		return sid->evsel;

	if (!perf_evlist__sample_id_all(evlist))
		return perf_evlist__first(evlist);

	return NULL;
}

static int perf_evlist__event2id(struct perf_evlist *evlist,
				 union perf_event *event, u64 *id)
{
	const u64 *array = event->sample.array;
	ssize_t n;

	n = (event->header.size - sizeof(event->header)) >> 3;

	if (event->header.type == PERF_RECORD_SAMPLE) {
		if (evlist->id_pos >= n)
			return -1;
		*id = array[evlist->id_pos];
	} else {
		if (evlist->is_pos > n)
			return -1;
		n -= evlist->is_pos;
		*id = array[n];
	}
	return 0;
}

static struct perf_evsel *perf_evlist__event2evsel(struct perf_evlist *evlist,
						   union perf_event *event)
{
	struct perf_evsel *first = perf_evlist__first(evlist);
	struct hlist_head *head;
	struct perf_sample_id *sid;
	int hash;
	u64 id;

	if (evlist->nr_entries == 1)
		return first;

	if (!first->attr.sample_id_all &&
	    event->header.type != PERF_RECORD_SAMPLE)
		return first;

	if (perf_evlist__event2id(evlist, event, &id))
		return NULL;

	/* Synthesized events have an id of zero */
	if (!id)
		return first;
>>>>>>> refs/remotes/origin/master

	hash = hash_64(id, PERF_EVLIST__HLIST_BITS);
	head = &evlist->heads[hash];

<<<<<<< HEAD
	hlist_for_each_entry(sid, pos, head, node)
		if (sid->id == id)
			return sid->evsel;
<<<<<<< HEAD
=======

	if (!perf_evlist__sample_id_all(evlist))
		return list_entry(evlist->entries.next, struct perf_evsel, node);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	hlist_for_each_entry(sid, head, node) {
		if (sid->id == id)
			return sid->evsel;
	}
>>>>>>> refs/remotes/origin/master
	return NULL;
}

union perf_event *perf_evlist__mmap_read(struct perf_evlist *evlist, int idx)
{
<<<<<<< HEAD
	/* XXX Move this to perf.c, making it generally available */
	unsigned int page_size = sysconf(_SC_PAGE_SIZE);
=======
>>>>>>> refs/remotes/origin/master
	struct perf_mmap *md = &evlist->mmap[idx];
	unsigned int head = perf_mmap__read_head(md);
	unsigned int old = md->prev;
	unsigned char *data = md->base + page_size;
	union perf_event *event = NULL;

	if (evlist->overwrite) {
		/*
		 * If we're further behind than half the buffer, there's a chance
		 * the writer will bite our tail and mess up the samples under us.
		 *
		 * If we somehow ended up ahead of the head, we got messed up.
		 *
		 * In either case, truncate and restart at head.
		 */
		int diff = head - old;
		if (diff > md->mask / 2 || diff < 0) {
			fprintf(stderr, "WARNING: failed to keep up with mmap data.\n");

			/*
			 * head points to a known good entry, start there.
			 */
			old = head;
		}
	}

	if (old != head) {
		size_t size;

		event = (union perf_event *)&data[old & md->mask];
		size = event->header.size;

		/*
		 * Event straddles the mmap boundary -- header should always
		 * be inside due to u64 alignment of output.
		 */
		if ((old & md->mask) + size != ((old + size) & md->mask)) {
			unsigned int offset = old;
			unsigned int len = min(sizeof(*event), size), cpy;
<<<<<<< HEAD
			void *dst = &evlist->event_copy;
=======
			void *dst = md->event_copy;
>>>>>>> refs/remotes/origin/master

			do {
				cpy = min(md->mask + 1 - (offset & md->mask), len);
				memcpy(dst, &data[offset & md->mask], cpy);
				offset += cpy;
				dst += cpy;
				len -= cpy;
			} while (len);

<<<<<<< HEAD
			event = &evlist->event_copy;
=======
			event = (union perf_event *) md->event_copy;
>>>>>>> refs/remotes/origin/master
		}

		old += size;
	}

	md->prev = old;

<<<<<<< HEAD
	if (!evlist->overwrite)
		perf_mmap__write_tail(md, old);

	return event;
=======
	return event;
}

void perf_evlist__mmap_consume(struct perf_evlist *evlist, int idx)
{
	if (!evlist->overwrite) {
		struct perf_mmap *md = &evlist->mmap[idx];
		unsigned int old = md->prev;

		perf_mmap__write_tail(md, old);
	}
}

static void __perf_evlist__munmap(struct perf_evlist *evlist, int idx)
{
	if (evlist->mmap[idx].base != NULL) {
		munmap(evlist->mmap[idx].base, evlist->mmap_len);
		evlist->mmap[idx].base = NULL;
	}
>>>>>>> refs/remotes/origin/master
}

void perf_evlist__munmap(struct perf_evlist *evlist)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < evlist->nr_mmaps; i++) {
		if (evlist->mmap[i].base != NULL) {
			munmap(evlist->mmap[i].base, evlist->mmap_len);
			evlist->mmap[i].base = NULL;
		}
	}

	free(evlist->mmap);
	evlist->mmap = NULL;
}

<<<<<<< HEAD
int perf_evlist__alloc_mmap(struct perf_evlist *evlist)
=======
static int perf_evlist__alloc_mmap(struct perf_evlist *evlist)
>>>>>>> refs/remotes/origin/cm-10.0
{
	evlist->nr_mmaps = evlist->cpus->nr;
	if (evlist->cpus->map[0] == -1)
		evlist->nr_mmaps = evlist->threads->nr;
=======
	if (evlist->mmap == NULL)
		return;

	for (i = 0; i < evlist->nr_mmaps; i++)
		__perf_evlist__munmap(evlist, i);

	zfree(&evlist->mmap);
}

static int perf_evlist__alloc_mmap(struct perf_evlist *evlist)
{
	evlist->nr_mmaps = cpu_map__nr(evlist->cpus);
	if (cpu_map__empty(evlist->cpus))
		evlist->nr_mmaps = thread_map__nr(evlist->threads);
>>>>>>> refs/remotes/origin/master
	evlist->mmap = zalloc(evlist->nr_mmaps * sizeof(struct perf_mmap));
	return evlist->mmap != NULL ? 0 : -ENOMEM;
}

static int __perf_evlist__mmap(struct perf_evlist *evlist,
			       int idx, int prot, int mask, int fd)
{
	evlist->mmap[idx].prev = 0;
	evlist->mmap[idx].mask = mask;
	evlist->mmap[idx].base = mmap(NULL, evlist->mmap_len, prot,
				      MAP_SHARED, fd, 0);
<<<<<<< HEAD
<<<<<<< HEAD
	if (evlist->mmap[idx].base == MAP_FAILED)
		return -1;
=======
	if (evlist->mmap[idx].base == MAP_FAILED) {
		evlist->mmap[idx].base = NULL;
		return -1;
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (evlist->mmap[idx].base == MAP_FAILED) {
		pr_debug2("failed to mmap perf event ring buffer, error %d\n",
			  errno);
		evlist->mmap[idx].base = NULL;
		return -1;
	}
>>>>>>> refs/remotes/origin/master

	perf_evlist__add_pollfd(evlist, fd);
	return 0;
}

<<<<<<< HEAD
static int perf_evlist__mmap_per_cpu(struct perf_evlist *evlist, int prot, int mask)
{
	struct perf_evsel *evsel;
	int cpu, thread;

	for (cpu = 0; cpu < evlist->cpus->nr; cpu++) {
		int output = -1;

		for (thread = 0; thread < evlist->threads->nr; thread++) {
			list_for_each_entry(evsel, &evlist->entries, node) {
				int fd = FD(evsel, cpu, thread);

				if (output == -1) {
					output = fd;
					if (__perf_evlist__mmap(evlist, cpu,
								prot, mask, output) < 0)
						goto out_unmap;
				} else {
					if (ioctl(fd, PERF_EVENT_IOC_SET_OUTPUT, output) != 0)
						goto out_unmap;
				}

				if ((evsel->attr.read_format & PERF_FORMAT_ID) &&
				    perf_evlist__id_add_fd(evlist, evsel, cpu, thread, fd) < 0)
					goto out_unmap;
			}
=======
static int perf_evlist__mmap_per_evsel(struct perf_evlist *evlist, int idx,
				       int prot, int mask, int cpu, int thread,
				       int *output)
{
	struct perf_evsel *evsel;

	evlist__for_each(evlist, evsel) {
		int fd = FD(evsel, cpu, thread);

		if (*output == -1) {
			*output = fd;
			if (__perf_evlist__mmap(evlist, idx, prot, mask,
						*output) < 0)
				return -1;
		} else {
			if (ioctl(fd, PERF_EVENT_IOC_SET_OUTPUT, *output) != 0)
				return -1;
		}

		if ((evsel->attr.read_format & PERF_FORMAT_ID) &&
		    perf_evlist__id_add_fd(evlist, evsel, cpu, thread, fd) < 0)
			return -1;
	}

	return 0;
}

static int perf_evlist__mmap_per_cpu(struct perf_evlist *evlist, int prot,
				     int mask)
{
	int cpu, thread;
	int nr_cpus = cpu_map__nr(evlist->cpus);
	int nr_threads = thread_map__nr(evlist->threads);

	pr_debug2("perf event ring buffer mmapped per cpu\n");
	for (cpu = 0; cpu < nr_cpus; cpu++) {
		int output = -1;

		for (thread = 0; thread < nr_threads; thread++) {
			if (perf_evlist__mmap_per_evsel(evlist, cpu, prot, mask,
							cpu, thread, &output))
				goto out_unmap;
>>>>>>> refs/remotes/origin/master
		}
	}

	return 0;

out_unmap:
<<<<<<< HEAD
	for (cpu = 0; cpu < evlist->cpus->nr; cpu++) {
		if (evlist->mmap[cpu].base != NULL) {
			munmap(evlist->mmap[cpu].base, evlist->mmap_len);
			evlist->mmap[cpu].base = NULL;
		}
	}
	return -1;
}

static int perf_evlist__mmap_per_thread(struct perf_evlist *evlist, int prot, int mask)
{
	struct perf_evsel *evsel;
	int thread;

	for (thread = 0; thread < evlist->threads->nr; thread++) {
		int output = -1;

		list_for_each_entry(evsel, &evlist->entries, node) {
			int fd = FD(evsel, 0, thread);

			if (output == -1) {
				output = fd;
				if (__perf_evlist__mmap(evlist, thread,
							prot, mask, output) < 0)
					goto out_unmap;
			} else {
				if (ioctl(fd, PERF_EVENT_IOC_SET_OUTPUT, output) != 0)
					goto out_unmap;
			}

			if ((evsel->attr.read_format & PERF_FORMAT_ID) &&
			    perf_evlist__id_add_fd(evlist, evsel, 0, thread, fd) < 0)
				goto out_unmap;
		}
=======
	for (cpu = 0; cpu < nr_cpus; cpu++)
		__perf_evlist__munmap(evlist, cpu);
	return -1;
}

static int perf_evlist__mmap_per_thread(struct perf_evlist *evlist, int prot,
					int mask)
{
	int thread;
	int nr_threads = thread_map__nr(evlist->threads);

	pr_debug2("perf event ring buffer mmapped per thread\n");
	for (thread = 0; thread < nr_threads; thread++) {
		int output = -1;

		if (perf_evlist__mmap_per_evsel(evlist, thread, prot, mask, 0,
						thread, &output))
			goto out_unmap;
>>>>>>> refs/remotes/origin/master
	}

	return 0;

out_unmap:
<<<<<<< HEAD
	for (thread = 0; thread < evlist->threads->nr; thread++) {
		if (evlist->mmap[thread].base != NULL) {
			munmap(evlist->mmap[thread].base, evlist->mmap_len);
			evlist->mmap[thread].base = NULL;
		}
	}
	return -1;
}

/** perf_evlist__mmap - Create per cpu maps to receive events
 *
 * @evlist - list of events
 * @pages - map length in pages
 * @overwrite - overwrite older events?
 *
 * If overwrite is false the user needs to signal event consuption using:
 *
 *	struct perf_mmap *m = &evlist->mmap[cpu];
 *	unsigned int head = perf_mmap__read_head(m);
 *
 *	perf_mmap__write_tail(m, head)
 *
 * Using perf_evlist__read_on_cpu does this automatically.
 */
<<<<<<< HEAD
int perf_evlist__mmap(struct perf_evlist *evlist, int pages, bool overwrite)
{
	unsigned int page_size = sysconf(_SC_PAGE_SIZE);
	int mask = pages * page_size - 1;
	struct perf_evsel *evsel;
	const struct cpu_map *cpus = evlist->cpus;
	const struct thread_map *threads = evlist->threads;
	int prot = PROT_READ | (overwrite ? 0 : PROT_WRITE);
=======
int perf_evlist__mmap(struct perf_evlist *evlist, unsigned int pages,
		      bool overwrite)
{
	unsigned int page_size = sysconf(_SC_PAGE_SIZE);
=======
	for (thread = 0; thread < nr_threads; thread++)
		__perf_evlist__munmap(evlist, thread);
	return -1;
}

static size_t perf_evlist__mmap_size(unsigned long pages)
{
	/* 512 kiB: default amount of unprivileged mlocked memory */
	if (pages == UINT_MAX)
		pages = (512 * 1024) / page_size;
	else if (!is_power_of_2(pages))
		return 0;

	return (pages + 1) * page_size;
}

static long parse_pages_arg(const char *str, unsigned long min,
			    unsigned long max)
{
	unsigned long pages, val;
	static struct parse_tag tags[] = {
		{ .tag  = 'B', .mult = 1       },
		{ .tag  = 'K', .mult = 1 << 10 },
		{ .tag  = 'M', .mult = 1 << 20 },
		{ .tag  = 'G', .mult = 1 << 30 },
		{ .tag  = 0 },
	};

	if (str == NULL)
		return -EINVAL;

	val = parse_tag_value(str, tags);
	if (val != (unsigned long) -1) {
		/* we got file size value */
		pages = PERF_ALIGN(val, page_size) / page_size;
	} else {
		/* we got pages count value */
		char *eptr;
		pages = strtoul(str, &eptr, 10);
		if (*eptr != '\0')
			return -EINVAL;
	}

	if (pages == 0 && min == 0) {
		/* leave number of pages at 0 */
	} else if (!is_power_of_2(pages)) {
		/* round pages up to next power of 2 */
		pages = next_pow2_l(pages);
		if (!pages)
			return -EINVAL;
		pr_info("rounding mmap pages size to %lu bytes (%lu pages)\n",
			pages * page_size, pages);
	}

	if (pages > max)
		return -EINVAL;

	return pages;
}

int perf_evlist__parse_mmap_pages(const struct option *opt, const char *str,
				  int unset __maybe_unused)
{
	unsigned int *mmap_pages = opt->value;
	unsigned long max = UINT_MAX;
	long pages;

	if (max > SIZE_MAX / page_size)
		max = SIZE_MAX / page_size;

	pages = parse_pages_arg(str, 1, max);
	if (pages < 0) {
		pr_err("Invalid argument for --mmap_pages/-m\n");
		return -1;
	}

	*mmap_pages = pages;
	return 0;
}

/**
 * perf_evlist__mmap - Create mmaps to receive events.
 * @evlist: list of events
 * @pages: map length in pages
 * @overwrite: overwrite older events?
 *
 * If @overwrite is %false the user needs to signal event consumption using
 * perf_mmap__write_tail().  Using perf_evlist__mmap_read() does this
 * automatically.
 *
 * Return: %0 on success, negative error code otherwise.
 */
int perf_evlist__mmap(struct perf_evlist *evlist, unsigned int pages,
		      bool overwrite)
{
>>>>>>> refs/remotes/origin/master
	struct perf_evsel *evsel;
	const struct cpu_map *cpus = evlist->cpus;
	const struct thread_map *threads = evlist->threads;
	int prot = PROT_READ | (overwrite ? 0 : PROT_WRITE), mask;

<<<<<<< HEAD
        /* 512 kiB: default amount of unprivileged mlocked memory */
        if (pages == UINT_MAX)
                pages = (512 * 1024) / page_size;
	else if (!is_power_of_2(pages))
		return -EINVAL;

	mask = pages * page_size - 1;
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
	if (evlist->mmap == NULL && perf_evlist__alloc_mmap(evlist) < 0)
		return -ENOMEM;

	if (evlist->pollfd == NULL && perf_evlist__alloc_pollfd(evlist) < 0)
		return -ENOMEM;

	evlist->overwrite = overwrite;
<<<<<<< HEAD
	evlist->mmap_len = (pages + 1) * page_size;

	list_for_each_entry(evsel, &evlist->entries, node) {
		if ((evsel->attr.read_format & PERF_FORMAT_ID) &&
		    evsel->sample_id == NULL &&
		    perf_evsel__alloc_id(evsel, cpus->nr, threads->nr) < 0)
			return -ENOMEM;
	}

	if (evlist->cpus->map[0] == -1)
=======
	evlist->mmap_len = perf_evlist__mmap_size(pages);
	pr_debug("mmap size %zuB\n", evlist->mmap_len);
	mask = evlist->mmap_len - page_size - 1;

	evlist__for_each(evlist, evsel) {
		if ((evsel->attr.read_format & PERF_FORMAT_ID) &&
		    evsel->sample_id == NULL &&
		    perf_evsel__alloc_id(evsel, cpu_map__nr(cpus), threads->nr) < 0)
			return -ENOMEM;
	}

	if (cpu_map__empty(cpus))
>>>>>>> refs/remotes/origin/master
		return perf_evlist__mmap_per_thread(evlist, prot, mask);

	return perf_evlist__mmap_per_cpu(evlist, prot, mask);
}

<<<<<<< HEAD
<<<<<<< HEAD
int perf_evlist__create_maps(struct perf_evlist *evlist, pid_t target_pid,
			     pid_t target_tid, const char *cpu_list)
{
	evlist->threads = thread_map__new(target_pid, target_tid);
=======
int perf_evlist__create_maps(struct perf_evlist *evlist, const char *target_pid,
			     const char *target_tid, uid_t uid, const char *cpu_list)
{
	evlist->threads = thread_map__new_str(target_pid, target_tid, uid);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int perf_evlist__create_maps(struct perf_evlist *evlist, struct target *target)
{
	evlist->threads = thread_map__new_str(target->pid, target->tid,
					      target->uid);
>>>>>>> refs/remotes/origin/master

	if (evlist->threads == NULL)
		return -1;

<<<<<<< HEAD
<<<<<<< HEAD
	if (cpu_list == NULL && target_tid != -1)
=======
	if (uid != UINT_MAX || (cpu_list == NULL && target_tid))
>>>>>>> refs/remotes/origin/cm-10.0
		evlist->cpus = cpu_map__dummy_new();
	else
		evlist->cpus = cpu_map__new(cpu_list);
=======
	if (target__uses_dummy_map(target))
		evlist->cpus = cpu_map__dummy_new();
	else
		evlist->cpus = cpu_map__new(target->cpu_list);
>>>>>>> refs/remotes/origin/master

	if (evlist->cpus == NULL)
		goto out_delete_threads;

	return 0;

out_delete_threads:
	thread_map__delete(evlist->threads);
	return -1;
}

<<<<<<< HEAD
void perf_evlist__delete_maps(struct perf_evlist *evlist)
{
	cpu_map__delete(evlist->cpus);
	thread_map__delete(evlist->threads);
	evlist->cpus	= NULL;
	evlist->threads = NULL;
}

int perf_evlist__set_filters(struct perf_evlist *evlist)
{
	const struct thread_map *threads = evlist->threads;
	const struct cpu_map *cpus = evlist->cpus;
	struct perf_evsel *evsel;
	char *filter;
	int thread;
	int cpu;
	int err;
	int fd;

	list_for_each_entry(evsel, &evlist->entries, node) {
		filter = evsel->filter;
		if (!filter)
			continue;
		for (cpu = 0; cpu < cpus->nr; cpu++) {
			for (thread = 0; thread < threads->nr; thread++) {
				fd = FD(evsel, cpu, thread);
				err = ioctl(fd, PERF_EVENT_IOC_SET_FILTER, filter);
				if (err)
					return err;
			}
		}
	}

	return 0;
}

bool perf_evlist__valid_sample_type(const struct perf_evlist *evlist)
{
	struct perf_evsel *pos, *first;

	pos = first = list_entry(evlist->entries.next, struct perf_evsel, node);

	list_for_each_entry_continue(pos, &evlist->entries, node) {
		if (first->attr.sample_type != pos->attr.sample_type)
=======
int perf_evlist__apply_filters(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;
	int err = 0;
	const int ncpus = cpu_map__nr(evlist->cpus),
		  nthreads = thread_map__nr(evlist->threads);

	evlist__for_each(evlist, evsel) {
		if (evsel->filter == NULL)
			continue;

		err = perf_evsel__set_filter(evsel, ncpus, nthreads, evsel->filter);
		if (err)
			break;
	}

	return err;
}

int perf_evlist__set_filter(struct perf_evlist *evlist, const char *filter)
{
	struct perf_evsel *evsel;
	int err = 0;
	const int ncpus = cpu_map__nr(evlist->cpus),
		  nthreads = thread_map__nr(evlist->threads);

	evlist__for_each(evlist, evsel) {
		err = perf_evsel__set_filter(evsel, ncpus, nthreads, filter);
		if (err)
			break;
	}

	return err;
}

bool perf_evlist__valid_sample_type(struct perf_evlist *evlist)
{
	struct perf_evsel *pos;

	if (evlist->nr_entries == 1)
		return true;

	if (evlist->id_pos < 0 || evlist->is_pos < 0)
		return false;

	evlist__for_each(evlist, pos) {
		if (pos->id_pos != evlist->id_pos ||
		    pos->is_pos != evlist->is_pos)
>>>>>>> refs/remotes/origin/master
			return false;
	}

	return true;
}

<<<<<<< HEAD
u64 perf_evlist__sample_type(const struct perf_evlist *evlist)
{
	struct perf_evsel *first;

	first = list_entry(evlist->entries.next, struct perf_evsel, node);
	return first->attr.sample_type;
}

<<<<<<< HEAD
=======
u16 perf_evlist__id_hdr_size(const struct perf_evlist *evlist)
{
	struct perf_evsel *first;
=======
u64 __perf_evlist__combined_sample_type(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;

	if (evlist->combined_sample_type)
		return evlist->combined_sample_type;

	evlist__for_each(evlist, evsel)
		evlist->combined_sample_type |= evsel->attr.sample_type;

	return evlist->combined_sample_type;
}

u64 perf_evlist__combined_sample_type(struct perf_evlist *evlist)
{
	evlist->combined_sample_type = 0;
	return __perf_evlist__combined_sample_type(evlist);
}

bool perf_evlist__valid_read_format(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist), *pos = first;
	u64 read_format = first->attr.read_format;
	u64 sample_type = first->attr.sample_type;

	evlist__for_each(evlist, pos) {
		if (read_format != pos->attr.read_format)
			return false;
	}

	/* PERF_SAMPLE_READ imples PERF_FORMAT_ID. */
	if ((sample_type & PERF_SAMPLE_READ) &&
	    !(read_format & PERF_FORMAT_ID)) {
		return false;
	}

	return true;
}

u64 perf_evlist__read_format(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist);
	return first->attr.read_format;
}

u16 perf_evlist__id_hdr_size(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist);
>>>>>>> refs/remotes/origin/master
	struct perf_sample *data;
	u64 sample_type;
	u16 size = 0;

<<<<<<< HEAD
	first = list_entry(evlist->entries.next, struct perf_evsel, node);

=======
>>>>>>> refs/remotes/origin/master
	if (!first->attr.sample_id_all)
		goto out;

	sample_type = first->attr.sample_type;

	if (sample_type & PERF_SAMPLE_TID)
		size += sizeof(data->tid) * 2;

       if (sample_type & PERF_SAMPLE_TIME)
		size += sizeof(data->time);

	if (sample_type & PERF_SAMPLE_ID)
		size += sizeof(data->id);

	if (sample_type & PERF_SAMPLE_STREAM_ID)
		size += sizeof(data->stream_id);

	if (sample_type & PERF_SAMPLE_CPU)
		size += sizeof(data->cpu) * 2;
<<<<<<< HEAD
=======

	if (sample_type & PERF_SAMPLE_IDENTIFIER)
		size += sizeof(data->id);
>>>>>>> refs/remotes/origin/master
out:
	return size;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
bool perf_evlist__valid_sample_id_all(const struct perf_evlist *evlist)
{
	struct perf_evsel *pos, *first;

	pos = first = list_entry(evlist->entries.next, struct perf_evsel, node);

	list_for_each_entry_continue(pos, &evlist->entries, node) {
=======
bool perf_evlist__valid_sample_id_all(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist), *pos = first;

	evlist__for_each_continue(evlist, pos) {
>>>>>>> refs/remotes/origin/master
		if (first->attr.sample_id_all != pos->attr.sample_id_all)
			return false;
	}

	return true;
}

<<<<<<< HEAD
bool perf_evlist__sample_id_all(const struct perf_evlist *evlist)
{
	struct perf_evsel *first;

	first = list_entry(evlist->entries.next, struct perf_evsel, node);
	return first->attr.sample_id_all;
}
<<<<<<< HEAD
=======
=======
bool perf_evlist__sample_id_all(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist);
	return first->attr.sample_id_all;
}
>>>>>>> refs/remotes/origin/master

void perf_evlist__set_selected(struct perf_evlist *evlist,
			       struct perf_evsel *evsel)
{
	evlist->selected = evsel;
}

<<<<<<< HEAD
int perf_evlist__open(struct perf_evlist *evlist, bool group)
{
	struct perf_evsel *evsel, *first;
	int err, ncpus, nthreads;

	first = list_entry(evlist->entries.next, struct perf_evsel, node);

	list_for_each_entry(evsel, &evlist->entries, node) {
		struct xyarray *group_fd = NULL;

		if (group && evsel != first)
			group_fd = first->fd;

		err = perf_evsel__open(evsel, evlist->cpus, evlist->threads,
				       group, group_fd);
=======
void perf_evlist__close(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;
	int ncpus = cpu_map__nr(evlist->cpus);
	int nthreads = thread_map__nr(evlist->threads);

	evlist__for_each_reverse(evlist, evsel)
		perf_evsel__close(evsel, ncpus, nthreads);
}

int perf_evlist__open(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;
	int err;

	perf_evlist__update_id_pos(evlist);

	evlist__for_each(evlist, evsel) {
		err = perf_evsel__open(evsel, evlist->cpus, evlist->threads);
>>>>>>> refs/remotes/origin/master
		if (err < 0)
			goto out_err;
	}

	return 0;
out_err:
<<<<<<< HEAD
	ncpus = evlist->cpus ? evlist->cpus->nr : 1;
	nthreads = evlist->threads ? evlist->threads->nr : 1;

	list_for_each_entry_reverse(evsel, &evlist->entries, node)
		perf_evsel__close(evsel, ncpus, nthreads);

=======
	perf_evlist__close(evlist);
>>>>>>> refs/remotes/origin/master
	errno = -err;
	return err;
}

<<<<<<< HEAD
int perf_evlist__prepare_workload(struct perf_evlist *evlist,
				  struct perf_record_opts *opts,
				  const char *argv[])
=======
int perf_evlist__prepare_workload(struct perf_evlist *evlist, struct target *target,
				  const char *argv[], bool pipe_output,
				  void (*exec_error)(int signo, siginfo_t *info, void *ucontext))
>>>>>>> refs/remotes/origin/master
{
	int child_ready_pipe[2], go_pipe[2];
	char bf;

	if (pipe(child_ready_pipe) < 0) {
		perror("failed to create 'ready' pipe");
		return -1;
	}

	if (pipe(go_pipe) < 0) {
		perror("failed to create 'go' pipe");
		goto out_close_ready_pipe;
	}

	evlist->workload.pid = fork();
	if (evlist->workload.pid < 0) {
		perror("failed to fork");
		goto out_close_pipes;
	}

	if (!evlist->workload.pid) {
<<<<<<< HEAD
		if (opts->pipe_output)
			dup2(2, 1);

=======
		if (pipe_output)
			dup2(2, 1);

		signal(SIGTERM, SIG_DFL);

>>>>>>> refs/remotes/origin/master
		close(child_ready_pipe[0]);
		close(go_pipe[1]);
		fcntl(go_pipe[0], F_SETFD, FD_CLOEXEC);

		/*
<<<<<<< HEAD
		 * Do a dummy execvp to get the PLT entry resolved,
		 * so we avoid the resolver overhead on the real
		 * execvp call.
		 */
		execvp("", (char **)argv);

		/*
=======
>>>>>>> refs/remotes/origin/master
		 * Tell the parent we're ready to go
		 */
		close(child_ready_pipe[1]);

		/*
		 * Wait until the parent tells us to go.
		 */
		if (read(go_pipe[0], &bf, 1) == -1)
			perror("unable to read pipe");

		execvp(argv[0], (char **)argv);

<<<<<<< HEAD
		perror(argv[0]);
		kill(getppid(), SIGUSR1);
		exit(-1);
	}

	if (!opts->system_wide && !opts->target_tid && !opts->target_pid)
=======
		if (exec_error) {
			union sigval val;

			val.sival_int = errno;
			if (sigqueue(getppid(), SIGUSR1, val))
				perror(argv[0]);
		} else
			perror(argv[0]);
		exit(-1);
	}

	if (exec_error) {
		struct sigaction act = {
			.sa_flags     = SA_SIGINFO,
			.sa_sigaction = exec_error,
		};
		sigaction(SIGUSR1, &act, NULL);
	}

	if (target__none(target))
>>>>>>> refs/remotes/origin/master
		evlist->threads->map[0] = evlist->workload.pid;

	close(child_ready_pipe[1]);
	close(go_pipe[0]);
	/*
	 * wait for child to settle
	 */
	if (read(child_ready_pipe[0], &bf, 1) == -1) {
		perror("unable to read pipe");
		goto out_close_pipes;
	}

<<<<<<< HEAD
=======
	fcntl(go_pipe[1], F_SETFD, FD_CLOEXEC);
>>>>>>> refs/remotes/origin/master
	evlist->workload.cork_fd = go_pipe[1];
	close(child_ready_pipe[0]);
	return 0;

out_close_pipes:
	close(go_pipe[0]);
	close(go_pipe[1]);
out_close_ready_pipe:
	close(child_ready_pipe[0]);
	close(child_ready_pipe[1]);
	return -1;
}

int perf_evlist__start_workload(struct perf_evlist *evlist)
{
	if (evlist->workload.cork_fd > 0) {
<<<<<<< HEAD
		/*
		 * Remove the cork, let it rip!
		 */
		return close(evlist->workload.cork_fd);
=======
		char bf = 0;
		int ret;
		/*
		 * Remove the cork, let it rip!
		 */
		ret = write(evlist->workload.cork_fd, &bf, 1);
		if (ret < 0)
			perror("enable to write to pipe");

		close(evlist->workload.cork_fd);
		return ret;
	}

	return 0;
}

int perf_evlist__parse_sample(struct perf_evlist *evlist, union perf_event *event,
			      struct perf_sample *sample)
{
	struct perf_evsel *evsel = perf_evlist__event2evsel(evlist, event);

	if (!evsel)
		return -EFAULT;
	return perf_evsel__parse_sample(evsel, event, sample);
}

size_t perf_evlist__fprintf(struct perf_evlist *evlist, FILE *fp)
{
	struct perf_evsel *evsel;
	size_t printed = 0;

	evlist__for_each(evlist, evsel) {
		printed += fprintf(fp, "%s%s", evsel->idx ? ", " : "",
				   perf_evsel__name(evsel));
	}

	return printed + fprintf(fp, "\n");
}

int perf_evlist__strerror_tp(struct perf_evlist *evlist __maybe_unused,
			     int err, char *buf, size_t size)
{
	char sbuf[128];

	switch (err) {
	case ENOENT:
		scnprintf(buf, size, "%s",
			  "Error:\tUnable to find debugfs\n"
			  "Hint:\tWas your kernel was compiled with debugfs support?\n"
			  "Hint:\tIs the debugfs filesystem mounted?\n"
			  "Hint:\tTry 'sudo mount -t debugfs nodev /sys/kernel/debug'");
		break;
	case EACCES:
		scnprintf(buf, size,
			  "Error:\tNo permissions to read %s/tracing/events/raw_syscalls\n"
			  "Hint:\tTry 'sudo mount -o remount,mode=755 %s'\n",
			  debugfs_mountpoint, debugfs_mountpoint);
		break;
	default:
		scnprintf(buf, size, "%s", strerror_r(err, sbuf, sizeof(sbuf)));
		break;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

int perf_evlist__strerror_open(struct perf_evlist *evlist __maybe_unused,
			       int err, char *buf, size_t size)
{
	int printed, value;
	char sbuf[128], *emsg = strerror_r(err, sbuf, sizeof(sbuf));

	switch (err) {
	case EACCES:
	case EPERM:
		printed = scnprintf(buf, size,
				    "Error:\t%s.\n"
				    "Hint:\tCheck /proc/sys/kernel/perf_event_paranoid setting.", emsg);

		value = perf_event_paranoid();

		printed += scnprintf(buf + printed, size - printed, "\nHint:\t");

		if (value >= 2) {
			printed += scnprintf(buf + printed, size - printed,
					     "For your workloads it needs to be <= 1\nHint:\t");
		}
		printed += scnprintf(buf + printed, size - printed,
				     "For system wide tracing it needs to be set to -1");

		printed += scnprintf(buf + printed, size - printed,
				    ".\nHint:\tThe current value is %d.", value);
		break;
	default:
		scnprintf(buf, size, "%s", emsg);
		break;
	}

	return 0;
}

void perf_evlist__to_front(struct perf_evlist *evlist,
			   struct perf_evsel *move_evsel)
{
	struct perf_evsel *evsel, *n;
	LIST_HEAD(move);

	if (move_evsel == perf_evlist__first(evlist))
		return;

	evlist__for_each_safe(evlist, n, evsel) {
		if (evsel->leader == move_evsel->leader)
			list_move_tail(&evsel->node, &move);
	}

	list_splice(&move, &evlist->entries);
}
>>>>>>> refs/remotes/origin/master
