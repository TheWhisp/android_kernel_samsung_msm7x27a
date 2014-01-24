<<<<<<< HEAD
#define _FILE_OFFSET_BITS 64

#include <linux/kernel.h>
=======
#include <linux/kernel.h>
#include <traceevent/event-parse.h>
>>>>>>> refs/remotes/origin/master

#include <byteswap.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "evlist.h"
#include "evsel.h"
#include "session.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "sort.h"
#include "util.h"
=======
=======
>>>>>>> refs/remotes/origin/master
#include "tool.h"
#include "sort.h"
#include "util.h"
#include "cpumap.h"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

static int perf_session__open(struct perf_session *self, bool force)
{
	struct stat input_stat;

	if (!strcmp(self->filename, "-")) {
		self->fd_pipe = true;
		self->fd = STDIN_FILENO;

		if (perf_session__read_header(self, self->fd) < 0)
<<<<<<< HEAD
			pr_err("incompatible file format");
=======
			pr_err("incompatible file format (rerun with -v to learn more)");
>>>>>>> refs/remotes/origin/cm-10.0

		return 0;
	}

	self->fd = open(self->filename, O_RDONLY);
	if (self->fd < 0) {
		int err = errno;

		pr_err("failed to open %s: %s", self->filename, strerror(err));
		if (err == ENOENT && !strcmp(self->filename, "perf.data"))
			pr_err("  (try 'perf record' first)");
		pr_err("\n");
		return -errno;
	}

	if (fstat(self->fd, &input_stat) < 0)
		goto out_close;

	if (!force && input_stat.st_uid && (input_stat.st_uid != geteuid())) {
		pr_err("file %s not owned by current user or root\n",
		       self->filename);
		goto out_close;
	}

	if (!input_stat.st_size) {
		pr_info("zero-sized file (%s), nothing to do!\n",
			self->filename);
		goto out_close;
	}

	if (perf_session__read_header(self, self->fd) < 0) {
<<<<<<< HEAD
		pr_err("incompatible file format");
=======
		pr_err("incompatible file format (rerun with -v to learn more)");
>>>>>>> refs/remotes/origin/cm-10.0
		goto out_close;
	}

	if (!perf_evlist__valid_sample_type(self->evlist)) {
		pr_err("non matching sample_type");
		goto out_close;
	}

	if (!perf_evlist__valid_sample_id_all(self->evlist)) {
		pr_err("non matching sample_id_all");
		goto out_close;
	}

	self->size = input_stat.st_size;
	return 0;

out_close:
	close(self->fd);
	self->fd = -1;
	return -1;
}

<<<<<<< HEAD
static void perf_session__id_header_size(struct perf_session *session)
{
       struct perf_sample *data;
       u64 sample_type = session->sample_type;
       u16 size = 0;

	if (!session->sample_id_all)
		goto out;

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
out:
       session->id_hdr_size = size;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
void perf_session__update_sample_type(struct perf_session *self)
{
	self->sample_type = perf_evlist__sample_type(self->evlist);
	self->sample_size = __perf_evsel__sample_size(self->sample_type);
	self->sample_id_all = perf_evlist__sample_id_all(self->evlist);
<<<<<<< HEAD
	perf_session__id_header_size(self);
=======
	self->id_hdr_size = perf_evlist__id_hdr_size(self->evlist);
	self->host_machine.id_hdr_size = self->id_hdr_size;
>>>>>>> refs/remotes/origin/cm-10.0
}

int perf_session__create_kernel_maps(struct perf_session *self)
{
	int ret = machine__create_kernel_maps(&self->host_machine);

	if (ret >= 0)
		ret = machines__create_guest_kernel_maps(&self->machines);
	return ret;
}

static void perf_session__destroy_kernel_maps(struct perf_session *self)
{
	machine__destroy_kernel_maps(&self->host_machine);
	machines__destroy_guest_kernel_maps(&self->machines);
}

struct perf_session *perf_session__new(const char *filename, int mode,
				       bool force, bool repipe,
<<<<<<< HEAD
				       struct perf_event_ops *ops)
{
	size_t len = filename ? strlen(filename) + 1 : 0;
	struct perf_session *self = zalloc(sizeof(*self) + len);
=======
				       struct perf_tool *tool)
{
	struct perf_session *self;
	struct stat st;
	size_t len;

	if (!filename || !strlen(filename)) {
		if (!fstat(STDIN_FILENO, &st) && S_ISFIFO(st.st_mode))
			filename = "-";
		else
			filename = "perf.data";
	}

	len = strlen(filename);
	self = zalloc(sizeof(*self) + len);
>>>>>>> refs/remotes/origin/cm-10.0

	if (self == NULL)
		goto out;

	memcpy(self->filename, filename, len);
<<<<<<< HEAD
	self->threads = RB_ROOT;
	INIT_LIST_HEAD(&self->dead_threads);
	self->last_match = NULL;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * On 64bit we can mmap the data file in one go. No need for tiny mmap
	 * slices. On 32bit we use 32MB.
	 */
#if BITS_PER_LONG == 64
	self->mmap_window = ULLONG_MAX;
#else
	self->mmap_window = 32 * 1024 * 1024ULL;
#endif
	self->machines = RB_ROOT;
	self->repipe = repipe;
	INIT_LIST_HEAD(&self->ordered_samples.samples);
	INIT_LIST_HEAD(&self->ordered_samples.sample_cache);
	INIT_LIST_HEAD(&self->ordered_samples.to_free);
	machine__init(&self->host_machine, "", HOST_KERNEL_ID);
<<<<<<< HEAD
=======
	hists__init(&self->hists);
>>>>>>> refs/remotes/origin/cm-10.0

	if (mode == O_RDONLY) {
		if (perf_session__open(self, force) < 0)
			goto out_delete;
		perf_session__update_sample_type(self);
	} else if (mode == O_WRONLY) {
=======
#include "perf_regs.h"
#include "vdso.h"

static int perf_session__open(struct perf_session *session)
{
	struct perf_data_file *file = session->file;

	if (perf_session__read_header(session) < 0) {
		pr_err("incompatible file format (rerun with -v to learn more)");
		return -1;
	}

	if (perf_data_file__is_pipe(file))
		return 0;

	if (!perf_evlist__valid_sample_type(session->evlist)) {
		pr_err("non matching sample_type");
		return -1;
	}

	if (!perf_evlist__valid_sample_id_all(session->evlist)) {
		pr_err("non matching sample_id_all");
		return -1;
	}

	if (!perf_evlist__valid_read_format(session->evlist)) {
		pr_err("non matching read_format");
		return -1;
	}

	return 0;
}

void perf_session__set_id_hdr_size(struct perf_session *session)
{
	u16 id_hdr_size = perf_evlist__id_hdr_size(session->evlist);

	machines__set_id_hdr_size(&session->machines, id_hdr_size);
}

int perf_session__create_kernel_maps(struct perf_session *session)
{
	int ret = machine__create_kernel_maps(&session->machines.host);

	if (ret >= 0)
		ret = machines__create_guest_kernel_maps(&session->machines);
	return ret;
}

static void perf_session__destroy_kernel_maps(struct perf_session *session)
{
	machines__destroy_kernel_maps(&session->machines);
}

struct perf_session *perf_session__new(struct perf_data_file *file,
				       bool repipe, struct perf_tool *tool)
{
	struct perf_session *session = zalloc(sizeof(*session));

	if (!session)
		goto out;

	session->repipe = repipe;
	INIT_LIST_HEAD(&session->ordered_samples.samples);
	INIT_LIST_HEAD(&session->ordered_samples.sample_cache);
	INIT_LIST_HEAD(&session->ordered_samples.to_free);
	machines__init(&session->machines);

	if (file) {
		if (perf_data_file__open(file))
			goto out_delete;

		session->file = file;

		if (perf_data_file__is_read(file)) {
			if (perf_session__open(session) < 0)
				goto out_close;

			perf_session__set_id_hdr_size(session);
		}
	}

	if (!file || perf_data_file__is_write(file)) {
>>>>>>> refs/remotes/origin/master
		/*
		 * In O_RDONLY mode this will be performed when reading the
		 * kernel MMAP event, in perf_event__process_mmap().
		 */
<<<<<<< HEAD
		if (perf_session__create_kernel_maps(self) < 0)
			goto out_delete;
	}

<<<<<<< HEAD
	if (ops && ops->ordering_requires_timestamps &&
	    ops->ordered_samples && !self->sample_id_all) {
		dump_printf("WARNING: No sample_id_all support, falling back to unordered processing\n");
		ops->ordered_samples = false;
=======
	if (tool && tool->ordering_requires_timestamps &&
	    tool->ordered_samples && !self->sample_id_all) {
		dump_printf("WARNING: No sample_id_all support, falling back to unordered processing\n");
		tool->ordered_samples = false;
>>>>>>> refs/remotes/origin/cm-10.0
	}

out:
	return self;
out_delete:
	perf_session__delete(self);
	return NULL;
}

<<<<<<< HEAD
static void perf_session__delete_dead_threads(struct perf_session *self)
{
	struct thread *n, *t;

	list_for_each_entry_safe(t, n, &self->dead_threads, node) {
=======
static void machine__delete_dead_threads(struct machine *machine)
{
	struct thread *n, *t;

	list_for_each_entry_safe(t, n, &machine->dead_threads, node) {
>>>>>>> refs/remotes/origin/cm-10.0
		list_del(&t->node);
		thread__delete(t);
	}
}

<<<<<<< HEAD
static void perf_session__delete_threads(struct perf_session *self)
=======
static void perf_session__delete_dead_threads(struct perf_session *session)
{
	machine__delete_dead_threads(&session->host_machine);
}

static void machine__delete_threads(struct machine *self)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct rb_node *nd = rb_first(&self->threads);

	while (nd) {
		struct thread *t = rb_entry(nd, struct thread, rb_node);

		rb_erase(&t->rb_node, &self->threads);
		nd = rb_next(nd);
		thread__delete(t);
	}
}

<<<<<<< HEAD
=======
static void perf_session__delete_threads(struct perf_session *session)
{
	machine__delete_threads(&session->host_machine);
}

>>>>>>> refs/remotes/origin/cm-10.0
void perf_session__delete(struct perf_session *self)
{
	perf_session__destroy_kernel_maps(self);
	perf_session__delete_dead_threads(self);
	perf_session__delete_threads(self);
	machine__exit(&self->host_machine);
	close(self->fd);
	free(self);
}

<<<<<<< HEAD
void perf_session__remove_thread(struct perf_session *self, struct thread *th)
=======
void machine__remove_thread(struct machine *self, struct thread *th)
>>>>>>> refs/remotes/origin/cm-10.0
{
	self->last_match = NULL;
	rb_erase(&th->rb_node, &self->threads);
	/*
	 * We may have references to this thread, for instance in some hist_entry
	 * instances, so just move them to a separate list.
	 */
	list_add_tail(&th->node, &self->dead_threads);
}

static bool symbol__match_parent_regex(struct symbol *sym)
{
	if (sym->name && !regexec(&parent_regex, sym->name, 0, NULL, 0))
		return 1;

	return 0;
}

<<<<<<< HEAD
int perf_session__resolve_callchain(struct perf_session *self,
				    struct thread *thread,
				    struct ip_callchain *chain,
				    struct symbol **parent)
=======
static const u8 cpumodes[] = {
	PERF_RECORD_MISC_USER,
	PERF_RECORD_MISC_KERNEL,
	PERF_RECORD_MISC_GUEST_USER,
	PERF_RECORD_MISC_GUEST_KERNEL
};
#define NCPUMODES (sizeof(cpumodes)/sizeof(u8))

static void ip__resolve_ams(struct machine *self, struct thread *thread,
			    struct addr_map_symbol *ams,
			    u64 ip)
{
	struct addr_location al;
	size_t i;
	u8 m;

	memset(&al, 0, sizeof(al));

	for (i = 0; i < NCPUMODES; i++) {
		m = cpumodes[i];
		/*
		 * We cannot use the header.misc hint to determine whether a
		 * branch stack address is user, kernel, guest, hypervisor.
		 * Branches may straddle the kernel/user/hypervisor boundaries.
		 * Thus, we have to try consecutively until we find a match
		 * or else, the symbol is unknown
		 */
		thread__find_addr_location(thread, self, m, MAP__FUNCTION,
				ip, &al, NULL);
		if (al.sym)
			goto found;
	}
found:
	ams->addr = ip;
	ams->al_addr = al.addr;
	ams->sym = al.sym;
	ams->map = al.map;
}

struct branch_info *machine__resolve_bstack(struct machine *self,
					    struct thread *thr,
					    struct branch_stack *bs)
{
	struct branch_info *bi;
	unsigned int i;

	bi = calloc(bs->nr, sizeof(struct branch_info));
	if (!bi)
		return NULL;

	for (i = 0; i < bs->nr; i++) {
		ip__resolve_ams(self, thr, &bi[i].to, bs->entries[i].to);
		ip__resolve_ams(self, thr, &bi[i].from, bs->entries[i].from);
		bi[i].flags = bs->entries[i].flags;
	}
	return bi;
}

int machine__resolve_callchain(struct machine *self, struct perf_evsel *evsel,
			       struct thread *thread,
			       struct ip_callchain *chain,
			       struct symbol **parent)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u8 cpumode = PERF_RECORD_MISC_USER;
	unsigned int i;
	int err;

<<<<<<< HEAD
	callchain_cursor_reset(&self->callchain_cursor);

	for (i = 0; i < chain->nr; i++) {
		u64 ip = chain->ips[i];
		struct addr_location al;

=======
	callchain_cursor_reset(&evsel->hists.callchain_cursor);

	for (i = 0; i < chain->nr; i++) {
		u64 ip;
		struct addr_location al;

		if (callchain_param.order == ORDER_CALLEE)
			ip = chain->ips[i];
		else
			ip = chain->ips[chain->nr - i - 1];

>>>>>>> refs/remotes/origin/cm-10.0
		if (ip >= PERF_CONTEXT_MAX) {
			switch (ip) {
			case PERF_CONTEXT_HV:
				cpumode = PERF_RECORD_MISC_HYPERVISOR;	break;
			case PERF_CONTEXT_KERNEL:
				cpumode = PERF_RECORD_MISC_KERNEL;	break;
			case PERF_CONTEXT_USER:
				cpumode = PERF_RECORD_MISC_USER;	break;
			default:
				break;
			}
			continue;
		}

		al.filtered = false;
		thread__find_addr_location(thread, self, cpumode,
<<<<<<< HEAD
				MAP__FUNCTION, thread->pid, ip, &al, NULL);
=======
					   MAP__FUNCTION, ip, &al, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
		if (al.sym != NULL) {
			if (sort__has_parent && !*parent &&
			    symbol__match_parent_regex(al.sym))
				*parent = al.sym;
			if (!symbol_conf.use_callchain)
				break;
		}

<<<<<<< HEAD
		err = callchain_cursor_append(&self->callchain_cursor,
=======
		err = callchain_cursor_append(&evsel->hists.callchain_cursor,
>>>>>>> refs/remotes/origin/cm-10.0
					      ip, al.map, al.sym);
		if (err)
			return err;
	}

	return 0;
}

<<<<<<< HEAD
static int process_event_synth_stub(union perf_event *event __used,
				    struct perf_session *session __used)
=======
static int process_event_synth_tracing_data_stub(union perf_event *event __used,
						 struct perf_session *session __used)
>>>>>>> refs/remotes/origin/cm-10.0
{
	dump_printf(": unhandled!\n");
	return 0;
}

<<<<<<< HEAD
static int process_event_sample_stub(union perf_event *event __used,
				     struct perf_sample *sample __used,
				     struct perf_evsel *evsel __used,
				     struct perf_session *session __used)
=======
static int process_event_synth_attr_stub(union perf_event *event __used,
					 struct perf_evlist **pevlist __used)
=======
		if (perf_session__create_kernel_maps(session) < 0)
			goto out_delete;
	}

	if (tool && tool->ordering_requires_timestamps &&
	    tool->ordered_samples && !perf_evlist__sample_id_all(session->evlist)) {
		dump_printf("WARNING: No sample_id_all support, falling back to unordered processing\n");
		tool->ordered_samples = false;
	}

	return session;

 out_close:
	perf_data_file__close(file);
 out_delete:
	perf_session__delete(session);
 out:
	return NULL;
}

static void perf_session__delete_dead_threads(struct perf_session *session)
{
	machine__delete_dead_threads(&session->machines.host);
}

static void perf_session__delete_threads(struct perf_session *session)
{
	machine__delete_threads(&session->machines.host);
}

static void perf_session_env__delete(struct perf_session_env *env)
{
	zfree(&env->hostname);
	zfree(&env->os_release);
	zfree(&env->version);
	zfree(&env->arch);
	zfree(&env->cpu_desc);
	zfree(&env->cpuid);

	zfree(&env->cmdline);
	zfree(&env->sibling_cores);
	zfree(&env->sibling_threads);
	zfree(&env->numa_nodes);
	zfree(&env->pmu_mappings);
}

void perf_session__delete(struct perf_session *session)
{
	perf_session__destroy_kernel_maps(session);
	perf_session__delete_dead_threads(session);
	perf_session__delete_threads(session);
	perf_session_env__delete(&session->header.env);
	machines__exit(&session->machines);
	if (session->file)
		perf_data_file__close(session->file);
	free(session);
	vdso__exit();
}

static int process_event_synth_tracing_data_stub(struct perf_tool *tool
						 __maybe_unused,
						 union perf_event *event
						 __maybe_unused,
						 struct perf_session *session
						__maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	dump_printf(": unhandled!\n");
	return 0;
}

<<<<<<< HEAD
static int process_event_sample_stub(struct perf_tool *tool __used,
				     union perf_event *event __used,
				     struct perf_sample *sample __used,
				     struct perf_evsel *evsel __used,
				     struct machine *machine __used)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int process_event_synth_attr_stub(struct perf_tool *tool __maybe_unused,
					 union perf_event *event __maybe_unused,
					 struct perf_evlist **pevlist
					 __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	dump_printf(": unhandled!\n");
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int process_event_stub(union perf_event *event __used,
			      struct perf_sample *sample __used,
			      struct perf_session *session __used)
=======
static int process_event_stub(struct perf_tool *tool __used,
			      union perf_event *event __used,
			      struct perf_sample *sample __used,
			      struct machine *machine __used)
=======
static int process_event_sample_stub(struct perf_tool *tool __maybe_unused,
				     union perf_event *event __maybe_unused,
				     struct perf_sample *sample __maybe_unused,
				     struct perf_evsel *evsel __maybe_unused,
				     struct machine *machine __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	dump_printf(": unhandled!\n");
	return 0;
}

<<<<<<< HEAD
static int process_finished_round_stub(struct perf_tool *tool __used,
				       union perf_event *event __used,
				       struct perf_session *perf_session __used)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int process_event_stub(struct perf_tool *tool __maybe_unused,
			      union perf_event *event __maybe_unused,
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	dump_printf(": unhandled!\n");
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int process_finished_round_stub(union perf_event *event __used,
				       struct perf_session *session __used,
				       struct perf_event_ops *ops __used)
=======
static int process_event_type_stub(struct perf_tool *tool __used,
				   union perf_event *event __used)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int process_finished_round_stub(struct perf_tool *tool __maybe_unused,
				       union perf_event *event __maybe_unused,
				       struct perf_session *perf_session
				       __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	dump_printf(": unhandled!\n");
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int process_finished_round(union perf_event *event,
				  struct perf_session *session,
				  struct perf_event_ops *ops);

static void perf_event_ops__fill_defaults(struct perf_event_ops *handler)
{
	if (handler->sample == NULL)
		handler->sample = process_event_sample_stub;
	if (handler->mmap == NULL)
		handler->mmap = process_event_stub;
	if (handler->comm == NULL)
		handler->comm = process_event_stub;
	if (handler->fork == NULL)
		handler->fork = process_event_stub;
	if (handler->exit == NULL)
		handler->exit = process_event_stub;
	if (handler->lost == NULL)
		handler->lost = perf_event__process_lost;
	if (handler->read == NULL)
		handler->read = process_event_stub;
	if (handler->throttle == NULL)
		handler->throttle = process_event_stub;
	if (handler->unthrottle == NULL)
		handler->unthrottle = process_event_stub;
	if (handler->attr == NULL)
		handler->attr = process_event_synth_stub;
	if (handler->event_type == NULL)
		handler->event_type = process_event_synth_stub;
	if (handler->tracing_data == NULL)
		handler->tracing_data = process_event_synth_stub;
	if (handler->build_id == NULL)
		handler->build_id = process_event_synth_stub;
	if (handler->finished_round == NULL) {
		if (handler->ordered_samples)
			handler->finished_round = process_finished_round;
		else
			handler->finished_round = process_finished_round_stub;
=======
=======
>>>>>>> refs/remotes/origin/master
static int process_finished_round(struct perf_tool *tool,
				  union perf_event *event,
				  struct perf_session *session);

<<<<<<< HEAD
static void perf_tool__fill_defaults(struct perf_tool *tool)
=======
void perf_tool__fill_defaults(struct perf_tool *tool)
>>>>>>> refs/remotes/origin/master
{
	if (tool->sample == NULL)
		tool->sample = process_event_sample_stub;
	if (tool->mmap == NULL)
		tool->mmap = process_event_stub;
<<<<<<< HEAD
=======
	if (tool->mmap2 == NULL)
		tool->mmap2 = process_event_stub;
>>>>>>> refs/remotes/origin/master
	if (tool->comm == NULL)
		tool->comm = process_event_stub;
	if (tool->fork == NULL)
		tool->fork = process_event_stub;
	if (tool->exit == NULL)
		tool->exit = process_event_stub;
	if (tool->lost == NULL)
		tool->lost = perf_event__process_lost;
	if (tool->read == NULL)
		tool->read = process_event_sample_stub;
	if (tool->throttle == NULL)
		tool->throttle = process_event_stub;
	if (tool->unthrottle == NULL)
		tool->unthrottle = process_event_stub;
	if (tool->attr == NULL)
		tool->attr = process_event_synth_attr_stub;
<<<<<<< HEAD
	if (tool->event_type == NULL)
		tool->event_type = process_event_type_stub;
=======
>>>>>>> refs/remotes/origin/master
	if (tool->tracing_data == NULL)
		tool->tracing_data = process_event_synth_tracing_data_stub;
	if (tool->build_id == NULL)
		tool->build_id = process_finished_round_stub;
	if (tool->finished_round == NULL) {
		if (tool->ordered_samples)
			tool->finished_round = process_finished_round;
		else
			tool->finished_round = process_finished_round_stub;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

void mem_bswap_64(void *src, int byte_size)
{
	u64 *m = src;

	while (byte_size > 0) {
		*m = bswap_64(*m);
		byte_size -= sizeof(u64);
		++m;
	}
}

static void perf_event__all64_swap(union perf_event *event)
=======
	}
}
 
static void swap_sample_id_all(union perf_event *event, void *data)
{
	void *end = (void *) event + event->header.size;
	int size = end - data;

	BUG_ON(size % sizeof(u64));
	mem_bswap_64(data, size);
}

static void perf_event__all64_swap(union perf_event *event,
				   bool sample_id_all __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	struct perf_event_header *hdr = &event->header;
	mem_bswap_64(hdr + 1, event->header.size - sizeof(*hdr));
}

<<<<<<< HEAD
static void perf_event__comm_swap(union perf_event *event)
{
	event->comm.pid = bswap_32(event->comm.pid);
	event->comm.tid = bswap_32(event->comm.tid);
}

static void perf_event__mmap_swap(union perf_event *event)
=======
static void perf_event__comm_swap(union perf_event *event, bool sample_id_all)
{
	event->comm.pid = bswap_32(event->comm.pid);
	event->comm.tid = bswap_32(event->comm.tid);

	if (sample_id_all) {
		void *data = &event->comm.comm;

		data += PERF_ALIGN(strlen(data) + 1, sizeof(u64));
		swap_sample_id_all(event, data);
	}
}

static void perf_event__mmap_swap(union perf_event *event,
				  bool sample_id_all)
>>>>>>> refs/remotes/origin/master
{
	event->mmap.pid	  = bswap_32(event->mmap.pid);
	event->mmap.tid	  = bswap_32(event->mmap.tid);
	event->mmap.start = bswap_64(event->mmap.start);
	event->mmap.len	  = bswap_64(event->mmap.len);
	event->mmap.pgoff = bswap_64(event->mmap.pgoff);
<<<<<<< HEAD
}

static void perf_event__task_swap(union perf_event *event)
=======

	if (sample_id_all) {
		void *data = &event->mmap.filename;

		data += PERF_ALIGN(strlen(data) + 1, sizeof(u64));
		swap_sample_id_all(event, data);
	}
}

static void perf_event__mmap2_swap(union perf_event *event,
				  bool sample_id_all)
{
	event->mmap2.pid   = bswap_32(event->mmap2.pid);
	event->mmap2.tid   = bswap_32(event->mmap2.tid);
	event->mmap2.start = bswap_64(event->mmap2.start);
	event->mmap2.len   = bswap_64(event->mmap2.len);
	event->mmap2.pgoff = bswap_64(event->mmap2.pgoff);
	event->mmap2.maj   = bswap_32(event->mmap2.maj);
	event->mmap2.min   = bswap_32(event->mmap2.min);
	event->mmap2.ino   = bswap_64(event->mmap2.ino);

	if (sample_id_all) {
		void *data = &event->mmap2.filename;

		data += PERF_ALIGN(strlen(data) + 1, sizeof(u64));
		swap_sample_id_all(event, data);
	}
}
static void perf_event__task_swap(union perf_event *event, bool sample_id_all)
>>>>>>> refs/remotes/origin/master
{
	event->fork.pid	 = bswap_32(event->fork.pid);
	event->fork.tid	 = bswap_32(event->fork.tid);
	event->fork.ppid = bswap_32(event->fork.ppid);
	event->fork.ptid = bswap_32(event->fork.ptid);
	event->fork.time = bswap_64(event->fork.time);
<<<<<<< HEAD
}

static void perf_event__read_swap(union perf_event *event)
=======

	if (sample_id_all)
		swap_sample_id_all(event, &event->fork + 1);
}

static void perf_event__read_swap(union perf_event *event, bool sample_id_all)
>>>>>>> refs/remotes/origin/master
{
	event->read.pid		 = bswap_32(event->read.pid);
	event->read.tid		 = bswap_32(event->read.tid);
	event->read.value	 = bswap_64(event->read.value);
	event->read.time_enabled = bswap_64(event->read.time_enabled);
	event->read.time_running = bswap_64(event->read.time_running);
	event->read.id		 = bswap_64(event->read.id);
<<<<<<< HEAD
=======

	if (sample_id_all)
		swap_sample_id_all(event, &event->read + 1);
}

static void perf_event__throttle_swap(union perf_event *event,
				      bool sample_id_all)
{
	event->throttle.time	  = bswap_64(event->throttle.time);
	event->throttle.id	  = bswap_64(event->throttle.id);
	event->throttle.stream_id = bswap_64(event->throttle.stream_id);

	if (sample_id_all)
		swap_sample_id_all(event, &event->throttle + 1);
}

static u8 revbyte(u8 b)
{
	int rev = (b >> 4) | ((b & 0xf) << 4);
	rev = ((rev & 0xcc) >> 2) | ((rev & 0x33) << 2);
	rev = ((rev & 0xaa) >> 1) | ((rev & 0x55) << 1);
	return (u8) rev;
}

/*
 * XXX this is hack in attempt to carry flags bitfield
 * throught endian village. ABI says:
 *
 * Bit-fields are allocated from right to left (least to most significant)
 * on little-endian implementations and from left to right (most to least
 * significant) on big-endian implementations.
 *
 * The above seems to be byte specific, so we need to reverse each
 * byte of the bitfield. 'Internet' also says this might be implementation
 * specific and we probably need proper fix and carry perf_event_attr
 * bitfield flags in separate data file FEAT_ section. Thought this seems
 * to work for now.
 */
static void swap_bitfield(u8 *p, unsigned len)
{
	unsigned i;

	for (i = 0; i < len; i++) {
		*p = revbyte(*p);
		p++;
	}
>>>>>>> refs/remotes/origin/master
}

/* exported for swapping attributes in file header */
void perf_event__attr_swap(struct perf_event_attr *attr)
{
	attr->type		= bswap_32(attr->type);
	attr->size		= bswap_32(attr->size);
	attr->config		= bswap_64(attr->config);
	attr->sample_period	= bswap_64(attr->sample_period);
	attr->sample_type	= bswap_64(attr->sample_type);
	attr->read_format	= bswap_64(attr->read_format);
	attr->wakeup_events	= bswap_32(attr->wakeup_events);
	attr->bp_type		= bswap_32(attr->bp_type);
	attr->bp_addr		= bswap_64(attr->bp_addr);
	attr->bp_len		= bswap_64(attr->bp_len);
<<<<<<< HEAD
}

static void perf_event__hdr_attr_swap(union perf_event *event)
=======
	attr->branch_sample_type = bswap_64(attr->branch_sample_type);
	attr->sample_regs_user	 = bswap_64(attr->sample_regs_user);
	attr->sample_stack_user  = bswap_32(attr->sample_stack_user);

	swap_bitfield((u8 *) (&attr->read_format + 1), sizeof(u64));
}

static void perf_event__hdr_attr_swap(union perf_event *event,
				      bool sample_id_all __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	size_t size;

	perf_event__attr_swap(&event->attr.attr);

	size = event->header.size;
	size -= (void *)&event->attr.id - (void *)event;
	mem_bswap_64(event->attr.id, size);
}

<<<<<<< HEAD
static void perf_event__event_type_swap(union perf_event *event)
=======
static void perf_event__event_type_swap(union perf_event *event,
					bool sample_id_all __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	event->event_type.event_type.event_id =
		bswap_64(event->event_type.event_type.event_id);
}

<<<<<<< HEAD
static void perf_event__tracing_data_swap(union perf_event *event)
=======
static void perf_event__tracing_data_swap(union perf_event *event,
					  bool sample_id_all __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	event->tracing_data.size = bswap_32(event->tracing_data.size);
}

<<<<<<< HEAD
typedef void (*perf_event__swap_op)(union perf_event *event);

static perf_event__swap_op perf_event__swap_ops[] = {
	[PERF_RECORD_MMAP]		  = perf_event__mmap_swap,
=======
typedef void (*perf_event__swap_op)(union perf_event *event,
				    bool sample_id_all);

static perf_event__swap_op perf_event__swap_ops[] = {
	[PERF_RECORD_MMAP]		  = perf_event__mmap_swap,
	[PERF_RECORD_MMAP2]		  = perf_event__mmap2_swap,
>>>>>>> refs/remotes/origin/master
	[PERF_RECORD_COMM]		  = perf_event__comm_swap,
	[PERF_RECORD_FORK]		  = perf_event__task_swap,
	[PERF_RECORD_EXIT]		  = perf_event__task_swap,
	[PERF_RECORD_LOST]		  = perf_event__all64_swap,
	[PERF_RECORD_READ]		  = perf_event__read_swap,
<<<<<<< HEAD
=======
	[PERF_RECORD_THROTTLE]		  = perf_event__throttle_swap,
	[PERF_RECORD_UNTHROTTLE]	  = perf_event__throttle_swap,
>>>>>>> refs/remotes/origin/master
	[PERF_RECORD_SAMPLE]		  = perf_event__all64_swap,
	[PERF_RECORD_HEADER_ATTR]	  = perf_event__hdr_attr_swap,
	[PERF_RECORD_HEADER_EVENT_TYPE]	  = perf_event__event_type_swap,
	[PERF_RECORD_HEADER_TRACING_DATA] = perf_event__tracing_data_swap,
	[PERF_RECORD_HEADER_BUILD_ID]	  = NULL,
	[PERF_RECORD_HEADER_MAX]	  = NULL,
};

struct sample_queue {
	u64			timestamp;
	u64			file_offset;
	union perf_event	*event;
	struct list_head	list;
};

static void perf_session_free_sample_buffers(struct perf_session *session)
{
	struct ordered_samples *os = &session->ordered_samples;

	while (!list_empty(&os->to_free)) {
		struct sample_queue *sq;

		sq = list_entry(os->to_free.next, struct sample_queue, list);
		list_del(&sq->list);
		free(sq);
	}
}

static int perf_session_deliver_event(struct perf_session *session,
				      union perf_event *event,
				      struct perf_sample *sample,
<<<<<<< HEAD
<<<<<<< HEAD
				      struct perf_event_ops *ops,
				      u64 file_offset);

static void flush_sample_queue(struct perf_session *s,
			       struct perf_event_ops *ops)
=======
				      struct perf_tool *tool,
				      u64 file_offset);

static void flush_sample_queue(struct perf_session *s,
			       struct perf_tool *tool)
>>>>>>> refs/remotes/origin/cm-10.0
=======
				      struct perf_tool *tool,
				      u64 file_offset);

static int flush_sample_queue(struct perf_session *s,
		       struct perf_tool *tool)
>>>>>>> refs/remotes/origin/master
{
	struct ordered_samples *os = &s->ordered_samples;
	struct list_head *head = &os->samples;
	struct sample_queue *tmp, *iter;
	struct perf_sample sample;
	u64 limit = os->next_flush;
	u64 last_ts = os->last_sample ? os->last_sample->timestamp : 0ULL;
<<<<<<< HEAD
<<<<<<< HEAD
	int ret;

	if (!ops->ordered_samples || !limit)
=======
	unsigned idx = 0, progress_next = os->nr_samples / 16;
	int ret;

	if (!tool->ordered_samples || !limit)
>>>>>>> refs/remotes/origin/cm-10.0
		return;

	list_for_each_entry_safe(iter, tmp, head, list) {
		if (iter->timestamp > limit)
			break;

		ret = perf_session__parse_sample(s, iter->event, &sample);
		if (ret)
			pr_err("Can't parse sample, err = %d\n", ret);
		else
<<<<<<< HEAD
			perf_session_deliver_event(s, iter->event, &sample, ops,
=======
			perf_session_deliver_event(s, iter->event, &sample, tool,
>>>>>>> refs/remotes/origin/cm-10.0
						   iter->file_offset);
=======
	bool show_progress = limit == ULLONG_MAX;
	struct ui_progress prog;
	int ret;

	if (!tool->ordered_samples || !limit)
		return 0;

	if (show_progress)
		ui_progress__init(&prog, os->nr_samples, "Processing time ordered events...");

	list_for_each_entry_safe(iter, tmp, head, list) {
		if (session_done())
			return 0;

		if (iter->timestamp > limit)
			break;

		ret = perf_evlist__parse_sample(s->evlist, iter->event, &sample);
		if (ret)
			pr_err("Can't parse sample, err = %d\n", ret);
		else {
			ret = perf_session_deliver_event(s, iter->event, &sample, tool,
							 iter->file_offset);
			if (ret)
				return ret;
		}
>>>>>>> refs/remotes/origin/master

		os->last_flush = iter->timestamp;
		list_del(&iter->list);
		list_add(&iter->list, &os->sample_cache);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (++idx >= progress_next) {
			progress_next += os->nr_samples / 16;
			ui_progress__update(idx, os->nr_samples,
					    "Processing time ordered events...");
		}
>>>>>>> refs/remotes/origin/cm-10.0
=======

		if (show_progress)
			ui_progress__update(&prog, 1);
>>>>>>> refs/remotes/origin/master
	}

	if (list_empty(head)) {
		os->last_sample = NULL;
	} else if (last_ts <= limit) {
		os->last_sample =
			list_entry(head->prev, struct sample_queue, list);
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======

	os->nr_samples = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	os->nr_samples = 0;

	return 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * When perf record finishes a pass on every buffers, it records this pseudo
 * event.
 * We record the max timestamp t found in the pass n.
 * Assuming these timestamps are monotonic across cpus, we know that if
 * a buffer still has events with timestamps below t, they will be all
 * available and then read in the pass n + 1.
 * Hence when we start to read the pass n + 2, we can safely flush every
 * events with timestamps below t.
 *
 *    ============ PASS n =================
 *       CPU 0         |   CPU 1
 *                     |
 *    cnt1 timestamps  |   cnt2 timestamps
 *          1          |         2
 *          2          |         3
 *          -          |         4  <--- max recorded
 *
 *    ============ PASS n + 1 ==============
 *       CPU 0         |   CPU 1
 *                     |
 *    cnt1 timestamps  |   cnt2 timestamps
 *          3          |         5
 *          4          |         6
 *          5          |         7 <---- max recorded
 *
 *      Flush every events below timestamp 4
 *
 *    ============ PASS n + 2 ==============
 *       CPU 0         |   CPU 1
 *                     |
 *    cnt1 timestamps  |   cnt2 timestamps
 *          6          |         8
 *          7          |         9
 *          -          |         10
 *
 *      Flush every events below timestamp 7
 *      etc...
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int process_finished_round(union perf_event *event __used,
				  struct perf_session *session,
				  struct perf_event_ops *ops)
{
	flush_sample_queue(session, ops);
=======
static int process_finished_round(struct perf_tool *tool,
				  union perf_event *event __used,
				  struct perf_session *session)
{
	flush_sample_queue(session, tool);
>>>>>>> refs/remotes/origin/cm-10.0
	session->ordered_samples.next_flush = session->ordered_samples.max_timestamp;

	return 0;
=======
static int process_finished_round(struct perf_tool *tool,
				  union perf_event *event __maybe_unused,
				  struct perf_session *session)
{
	int ret = flush_sample_queue(session, tool);
	if (!ret)
		session->ordered_samples.next_flush = session->ordered_samples.max_timestamp;

	return ret;
>>>>>>> refs/remotes/origin/master
}

/* The queue is ordered by time */
static void __queue_event(struct sample_queue *new, struct perf_session *s)
{
	struct ordered_samples *os = &s->ordered_samples;
	struct sample_queue *sample = os->last_sample;
	u64 timestamp = new->timestamp;
	struct list_head *p;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	++os->nr_samples;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	++os->nr_samples;
>>>>>>> refs/remotes/origin/master
	os->last_sample = new;

	if (!sample) {
		list_add(&new->list, &os->samples);
		os->max_timestamp = timestamp;
		return;
	}

	/*
	 * last_sample might point to some random place in the list as it's
	 * the last queued event. We expect that the new event is close to
	 * this.
	 */
	if (sample->timestamp <= timestamp) {
		while (sample->timestamp <= timestamp) {
			p = sample->list.next;
			if (p == &os->samples) {
				list_add_tail(&new->list, &os->samples);
				os->max_timestamp = timestamp;
				return;
			}
			sample = list_entry(p, struct sample_queue, list);
		}
		list_add_tail(&new->list, &sample->list);
	} else {
		while (sample->timestamp > timestamp) {
			p = sample->list.prev;
			if (p == &os->samples) {
				list_add(&new->list, &os->samples);
				return;
			}
			sample = list_entry(p, struct sample_queue, list);
		}
		list_add(&new->list, &sample->list);
	}
}

#define MAX_SAMPLE_BUFFER	(64 * 1024 / sizeof(struct sample_queue))

<<<<<<< HEAD
static int perf_session_queue_event(struct perf_session *s, union perf_event *event,
=======
int perf_session_queue_event(struct perf_session *s, union perf_event *event,
>>>>>>> refs/remotes/origin/master
				    struct perf_sample *sample, u64 file_offset)
{
	struct ordered_samples *os = &s->ordered_samples;
	struct list_head *sc = &os->sample_cache;
	u64 timestamp = sample->time;
	struct sample_queue *new;

	if (!timestamp || timestamp == ~0ULL)
		return -ETIME;

	if (timestamp < s->ordered_samples.last_flush) {
		printf("Warning: Timestamp below last timeslice flush\n");
		return -EINVAL;
	}

	if (!list_empty(sc)) {
		new = list_entry(sc->next, struct sample_queue, list);
		list_del(&new->list);
	} else if (os->sample_buffer) {
		new = os->sample_buffer + os->sample_buffer_idx;
		if (++os->sample_buffer_idx == MAX_SAMPLE_BUFFER)
			os->sample_buffer = NULL;
	} else {
		os->sample_buffer = malloc(MAX_SAMPLE_BUFFER * sizeof(*new));
		if (!os->sample_buffer)
			return -ENOMEM;
		list_add(&os->sample_buffer->list, &os->to_free);
		os->sample_buffer_idx = 2;
		new = os->sample_buffer + 1;
	}

	new->timestamp = timestamp;
	new->file_offset = file_offset;
	new->event = event;

	__queue_event(new, s);

	return 0;
}

static void callchain__printf(struct perf_sample *sample)
{
	unsigned int i;

	printf("... chain: nr:%" PRIu64 "\n", sample->callchain->nr);

	for (i = 0; i < sample->callchain->nr; i++)
		printf("..... %2d: %016" PRIx64 "\n",
		       i, sample->callchain->ips[i]);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void branch_stack__printf(struct perf_sample *sample)
{
	uint64_t i;

	printf("... branch stack: nr:%" PRIu64 "\n", sample->branch_stack->nr);

	for (i = 0; i < sample->branch_stack->nr; i++)
		printf("..... %2"PRIu64": %016" PRIx64 " -> %016" PRIx64 "\n",
			i, sample->branch_stack->entries[i].from,
			sample->branch_stack->entries[i].to);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void regs_dump__printf(u64 mask, u64 *regs)
{
	unsigned rid, i = 0;

	for_each_set_bit(rid, (unsigned long *) &mask, sizeof(mask) * 8) {
		u64 val = regs[i++];

		printf(".... %-5s 0x%" PRIx64 "\n",
		       perf_reg_name(rid), val);
	}
}

static void regs_user__printf(struct perf_sample *sample, u64 mask)
{
	struct regs_dump *user_regs = &sample->user_regs;

	if (user_regs->regs) {
		printf("... user regs: mask 0x%" PRIx64 "\n", mask);
		regs_dump__printf(mask, user_regs->regs);
	}
}

static void stack_user__printf(struct stack_dump *dump)
{
	printf("... ustack: size %" PRIu64 ", offset 0x%x\n",
	       dump->size, dump->offset);
}

>>>>>>> refs/remotes/origin/master
static void perf_session__print_tstamp(struct perf_session *session,
				       union perf_event *event,
				       struct perf_sample *sample)
{
<<<<<<< HEAD
	if (event->header.type != PERF_RECORD_SAMPLE &&
	    !session->sample_id_all) {
=======
	u64 sample_type = __perf_evlist__combined_sample_type(session->evlist);

	if (event->header.type != PERF_RECORD_SAMPLE &&
	    !perf_evlist__sample_id_all(session->evlist)) {
>>>>>>> refs/remotes/origin/master
		fputs("-1 -1 ", stdout);
		return;
	}

<<<<<<< HEAD
	if ((session->sample_type & PERF_SAMPLE_CPU))
		printf("%u ", sample->cpu);

	if (session->sample_type & PERF_SAMPLE_TIME)
		printf("%" PRIu64 " ", sample->time);
}

=======
	if ((sample_type & PERF_SAMPLE_CPU))
		printf("%u ", sample->cpu);

	if (sample_type & PERF_SAMPLE_TIME)
		printf("%" PRIu64 " ", sample->time);
}

static void sample_read__printf(struct perf_sample *sample, u64 read_format)
{
	printf("... sample_read:\n");

	if (read_format & PERF_FORMAT_TOTAL_TIME_ENABLED)
		printf("...... time enabled %016" PRIx64 "\n",
		       sample->read.time_enabled);

	if (read_format & PERF_FORMAT_TOTAL_TIME_RUNNING)
		printf("...... time running %016" PRIx64 "\n",
		       sample->read.time_running);

	if (read_format & PERF_FORMAT_GROUP) {
		u64 i;

		printf(".... group nr %" PRIu64 "\n", sample->read.group.nr);

		for (i = 0; i < sample->read.group.nr; i++) {
			struct sample_read_value *value;

			value = &sample->read.group.values[i];
			printf("..... id %016" PRIx64
			       ", value %016" PRIx64 "\n",
			       value->id, value->value);
		}
	} else
		printf("..... id %016" PRIx64 ", value %016" PRIx64 "\n",
			sample->read.one.id, sample->read.one.value);
}

>>>>>>> refs/remotes/origin/master
static void dump_event(struct perf_session *session, union perf_event *event,
		       u64 file_offset, struct perf_sample *sample)
{
	if (!dump_trace)
		return;

	printf("\n%#" PRIx64 " [%#x]: event: %d\n",
	       file_offset, event->header.size, event->header.type);

	trace_event(event);

	if (sample)
		perf_session__print_tstamp(session, event, sample);

	printf("%#" PRIx64 " [%#x]: PERF_RECORD_%s", file_offset,
	       event->header.size, perf_event__name(event->header.type));
}

<<<<<<< HEAD
static void dump_sample(struct perf_session *session, union perf_event *event,
			struct perf_sample *sample)
{
	if (!dump_trace)
		return;

<<<<<<< HEAD
	printf("(IP, %d): %d/%d: %#" PRIx64 " period: %" PRIu64 "\n",
	       event->header.misc, sample->pid, sample->tid, sample->ip,
	       sample->period);

	if (session->sample_type & PERF_SAMPLE_CALLCHAIN)
		callchain__printf(sample);
=======
=======
static void dump_sample(struct perf_evsel *evsel, union perf_event *event,
			struct perf_sample *sample)
{
	u64 sample_type;

	if (!dump_trace)
		return;

>>>>>>> refs/remotes/origin/master
	printf("(IP, %d): %d/%d: %#" PRIx64 " period: %" PRIu64 " addr: %#" PRIx64 "\n",
	       event->header.misc, sample->pid, sample->tid, sample->ip,
	       sample->period, sample->addr);

<<<<<<< HEAD
	if (session->sample_type & PERF_SAMPLE_CALLCHAIN)
		callchain__printf(sample);

	if (session->sample_type & PERF_SAMPLE_BRANCH_STACK)
		branch_stack__printf(sample);
=======
	sample_type = evsel->attr.sample_type;

	if (sample_type & PERF_SAMPLE_CALLCHAIN)
		callchain__printf(sample);

	if (sample_type & PERF_SAMPLE_BRANCH_STACK)
		branch_stack__printf(sample);

	if (sample_type & PERF_SAMPLE_REGS_USER)
		regs_user__printf(sample, evsel->attr.sample_regs_user);

	if (sample_type & PERF_SAMPLE_STACK_USER)
		stack_user__printf(&sample->user_stack);

	if (sample_type & PERF_SAMPLE_WEIGHT)
		printf("... weight: %" PRIu64 "\n", sample->weight);

	if (sample_type & PERF_SAMPLE_DATA_SRC)
		printf(" . data_src: 0x%"PRIx64"\n", sample->data_src);

	if (sample_type & PERF_SAMPLE_TRANSACTION)
		printf("... transaction: %" PRIx64 "\n", sample->transaction);

	if (sample_type & PERF_SAMPLE_READ)
		sample_read__printf(sample, evsel->attr.read_format);
>>>>>>> refs/remotes/origin/master
}

static struct machine *
	perf_session__find_machine_for_cpumode(struct perf_session *session,
<<<<<<< HEAD
					       union perf_event *event)
{
	const u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;

	if (cpumode == PERF_RECORD_MISC_GUEST_KERNEL && perf_guest) {
		u32 pid;

		if (event->header.type == PERF_RECORD_MMAP)
			pid = event->mmap.pid;
		else
			pid = event->ip.pid;

		return perf_session__find_machine(session, pid);
	}

	return perf_session__find_host_machine(session);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int perf_session_deliver_event(struct perf_session *session,
				      union perf_event *event,
				      struct perf_sample *sample,
<<<<<<< HEAD
				      struct perf_event_ops *ops,
				      u64 file_offset)
{
	struct perf_evsel *evsel;

	dump_event(session, event, file_offset, sample);

	switch (event->header.type) {
	case PERF_RECORD_SAMPLE:
		dump_sample(session, event, sample);
		evsel = perf_evlist__id2evsel(session->evlist, sample->id);
		if (evsel == NULL) {
			++session->hists.stats.nr_unknown_id;
			return -1;
		}
		return ops->sample(event, sample, evsel, session);
	case PERF_RECORD_MMAP:
		return ops->mmap(event, sample, session);
	case PERF_RECORD_COMM:
		return ops->comm(event, sample, session);
	case PERF_RECORD_FORK:
		return ops->fork(event, sample, session);
	case PERF_RECORD_EXIT:
		return ops->exit(event, sample, session);
	case PERF_RECORD_LOST:
		return ops->lost(event, sample, session);
	case PERF_RECORD_READ:
		return ops->read(event, sample, session);
	case PERF_RECORD_THROTTLE:
		return ops->throttle(event, sample, session);
	case PERF_RECORD_UNTHROTTLE:
		return ops->unthrottle(event, sample, session);
=======
=======
					       union perf_event *event,
					       struct perf_sample *sample)
{
	const u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;
	struct machine *machine;

	if (perf_guest &&
	    ((cpumode == PERF_RECORD_MISC_GUEST_KERNEL) ||
	     (cpumode == PERF_RECORD_MISC_GUEST_USER))) {
		u32 pid;

		if (event->header.type == PERF_RECORD_MMAP
		    || event->header.type == PERF_RECORD_MMAP2)
			pid = event->mmap.pid;
		else
			pid = sample->pid;

		machine = perf_session__find_machine(session, pid);
		if (!machine)
			machine = perf_session__findnew_machine(session,
						DEFAULT_GUEST_KERNEL_ID);
		return machine;
	}

	return &session->machines.host;
}

static int deliver_sample_value(struct perf_session *session,
				struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
				struct sample_read_value *v,
				struct machine *machine)
{
	struct perf_sample_id *sid;

	sid = perf_evlist__id2sid(session->evlist, v->id);
	if (sid) {
		sample->id     = v->id;
		sample->period = v->value - sid->period;
		sid->period    = v->value;
	}

	if (!sid || sid->evsel == NULL) {
		++session->stats.nr_unknown_id;
		return 0;
	}

	return tool->sample(tool, event, sample, sid->evsel, machine);
}

static int deliver_sample_group(struct perf_session *session,
				struct perf_tool *tool,
				union  perf_event *event,
				struct perf_sample *sample,
				struct machine *machine)
{
	int ret = -EINVAL;
	u64 i;

	for (i = 0; i < sample->read.group.nr; i++) {
		ret = deliver_sample_value(session, tool, event, sample,
					   &sample->read.group.values[i],
					   machine);
		if (ret)
			break;
	}

	return ret;
}

static int
perf_session__deliver_sample(struct perf_session *session,
			     struct perf_tool *tool,
			     union  perf_event *event,
			     struct perf_sample *sample,
			     struct perf_evsel *evsel,
			     struct machine *machine)
{
	/* We know evsel != NULL. */
	u64 sample_type = evsel->attr.sample_type;
	u64 read_format = evsel->attr.read_format;

	/* Standard sample delievery. */
	if (!(sample_type & PERF_SAMPLE_READ))
		return tool->sample(tool, event, sample, evsel, machine);

	/* For PERF_SAMPLE_READ we have either single or group mode. */
	if (read_format & PERF_FORMAT_GROUP)
		return deliver_sample_group(session, tool, event, sample,
					    machine);
	else
		return deliver_sample_value(session, tool, event, sample,
					    &sample->read.one, machine);
}

static int perf_session_deliver_event(struct perf_session *session,
				      union perf_event *event,
				      struct perf_sample *sample,
>>>>>>> refs/remotes/origin/master
				      struct perf_tool *tool,
				      u64 file_offset)
{
	struct perf_evsel *evsel;
	struct machine *machine;

	dump_event(session, event, file_offset, sample);

	evsel = perf_evlist__id2evsel(session->evlist, sample->id);
	if (evsel != NULL && event->header.type != PERF_RECORD_SAMPLE) {
		/*
		 * XXX We're leaving PERF_RECORD_SAMPLE unnacounted here
		 * because the tools right now may apply filters, discarding
		 * some of the samples. For consistency, in the future we
		 * should have something like nr_filtered_samples and remove
		 * the sample->period from total_sample_period, etc, KISS for
		 * now tho.
		 *
		 * Also testing against NULL allows us to handle files without
		 * attr.sample_id_all and/or without PERF_SAMPLE_ID. In the
		 * future probably it'll be a good idea to restrict event
		 * processing via perf_session to files with both set.
		 */
		hists__inc_nr_events(&evsel->hists, event->header.type);
	}

<<<<<<< HEAD
	machine = perf_session__find_machine_for_cpumode(session, event);

	switch (event->header.type) {
	case PERF_RECORD_SAMPLE:
		dump_sample(session, event, sample);
		if (evsel == NULL) {
			++session->hists.stats.nr_unknown_id;
			return 0;
		}
		if (machine == NULL) {
			++session->hists.stats.nr_unprocessable_samples;
			return 0;
		}
		return tool->sample(tool, event, sample, evsel, machine);
	case PERF_RECORD_MMAP:
		return tool->mmap(tool, event, sample, machine);
=======
	machine = perf_session__find_machine_for_cpumode(session, event,
							 sample);

	switch (event->header.type) {
	case PERF_RECORD_SAMPLE:
		dump_sample(evsel, event, sample);
		if (evsel == NULL) {
			++session->stats.nr_unknown_id;
			return 0;
		}
		if (machine == NULL) {
			++session->stats.nr_unprocessable_samples;
			return 0;
		}
		return perf_session__deliver_sample(session, tool, event,
						    sample, evsel, machine);
	case PERF_RECORD_MMAP:
		return tool->mmap(tool, event, sample, machine);
	case PERF_RECORD_MMAP2:
		return tool->mmap2(tool, event, sample, machine);
>>>>>>> refs/remotes/origin/master
	case PERF_RECORD_COMM:
		return tool->comm(tool, event, sample, machine);
	case PERF_RECORD_FORK:
		return tool->fork(tool, event, sample, machine);
	case PERF_RECORD_EXIT:
		return tool->exit(tool, event, sample, machine);
	case PERF_RECORD_LOST:
		if (tool->lost == perf_event__process_lost)
<<<<<<< HEAD
			session->hists.stats.total_lost += event->lost.lost;
=======
			session->stats.total_lost += event->lost.lost;
>>>>>>> refs/remotes/origin/master
		return tool->lost(tool, event, sample, machine);
	case PERF_RECORD_READ:
		return tool->read(tool, event, sample, evsel, machine);
	case PERF_RECORD_THROTTLE:
		return tool->throttle(tool, event, sample, machine);
	case PERF_RECORD_UNTHROTTLE:
		return tool->unthrottle(tool, event, sample, machine);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	default:
		++session->hists.stats.nr_unknown_events;
=======
	default:
		++session->stats.nr_unknown_events;
>>>>>>> refs/remotes/origin/master
		return -1;
	}
}

<<<<<<< HEAD
static int perf_session__preprocess_sample(struct perf_session *session,
					   union perf_event *event, struct perf_sample *sample)
{
	if (event->header.type != PERF_RECORD_SAMPLE ||
	    !(session->sample_type & PERF_SAMPLE_CALLCHAIN))
		return 0;

	if (!ip_callchain__valid(sample->callchain, event)) {
		pr_debug("call-chain problem with event, skipping it.\n");
		++session->hists.stats.nr_invalid_chains;
		session->hists.stats.total_invalid_chains += sample->period;
		return -EINVAL;
	}
	return 0;
}

static int perf_session__process_user_event(struct perf_session *session, union perf_event *event,
<<<<<<< HEAD
					    struct perf_event_ops *ops, u64 file_offset)
{
=======
					    struct perf_tool *tool, u64 file_offset)
{
	int err;

>>>>>>> refs/remotes/origin/cm-10.0
=======
static int perf_session__process_user_event(struct perf_session *session, union perf_event *event,
					    struct perf_tool *tool, u64 file_offset)
{
	int fd = perf_data_file__fd(session->file);
	int err;

>>>>>>> refs/remotes/origin/master
	dump_event(session, event, file_offset, NULL);

	/* These events are processed right away */
	switch (event->header.type) {
	case PERF_RECORD_HEADER_ATTR:
<<<<<<< HEAD
<<<<<<< HEAD
		return ops->attr(event, session);
	case PERF_RECORD_HEADER_EVENT_TYPE:
		return ops->event_type(event, session);
	case PERF_RECORD_HEADER_TRACING_DATA:
		/* setup for reading amidst mmap */
		lseek(session->fd, file_offset, SEEK_SET);
		return ops->tracing_data(event, session);
	case PERF_RECORD_HEADER_BUILD_ID:
		return ops->build_id(event, session);
	case PERF_RECORD_FINISHED_ROUND:
		return ops->finished_round(event, session, ops);
=======
		err = tool->attr(event, &session->evlist);
		if (err == 0)
			perf_session__update_sample_type(session);
		return err;
	case PERF_RECORD_HEADER_EVENT_TYPE:
		return tool->event_type(tool, event);
	case PERF_RECORD_HEADER_TRACING_DATA:
		/* setup for reading amidst mmap */
		lseek(session->fd, file_offset, SEEK_SET);
		return tool->tracing_data(event, session);
=======
		err = tool->attr(tool, event, &session->evlist);
		if (err == 0)
			perf_session__set_id_hdr_size(session);
		return err;
	case PERF_RECORD_HEADER_TRACING_DATA:
		/* setup for reading amidst mmap */
		lseek(fd, file_offset, SEEK_SET);
		return tool->tracing_data(tool, event, session);
>>>>>>> refs/remotes/origin/master
	case PERF_RECORD_HEADER_BUILD_ID:
		return tool->build_id(tool, event, session);
	case PERF_RECORD_FINISHED_ROUND:
		return tool->finished_round(tool, event, session);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	default:
		return -EINVAL;
	}
}

<<<<<<< HEAD
static int perf_session__process_event(struct perf_session *session,
				       union perf_event *event,
<<<<<<< HEAD
				       struct perf_event_ops *ops,
=======
				       struct perf_tool *tool,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void event_swap(union perf_event *event, bool sample_id_all)
{
	perf_event__swap_op swap;

	swap = perf_event__swap_ops[event->header.type];
	if (swap)
		swap(event, sample_id_all);
}

static int perf_session__process_event(struct perf_session *session,
				       union perf_event *event,
				       struct perf_tool *tool,
>>>>>>> refs/remotes/origin/master
				       u64 file_offset)
{
	struct perf_sample sample;
	int ret;

<<<<<<< HEAD
	if (session->header.needs_swap &&
	    perf_event__swap_ops[event->header.type])
		perf_event__swap_ops[event->header.type](event);
=======
	if (session->header.needs_swap)
		event_swap(event, perf_evlist__sample_id_all(session->evlist));
>>>>>>> refs/remotes/origin/master

	if (event->header.type >= PERF_RECORD_HEADER_MAX)
		return -EINVAL;

<<<<<<< HEAD
	hists__inc_nr_events(&session->hists, event->header.type);

	if (event->header.type >= PERF_RECORD_USER_TYPE_START)
<<<<<<< HEAD
		return perf_session__process_user_event(session, event, ops, file_offset);
=======
		return perf_session__process_user_event(session, event, tool, file_offset);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	events_stats__inc(&session->stats, event->header.type);

	if (event->header.type >= PERF_RECORD_USER_TYPE_START)
		return perf_session__process_user_event(session, event, tool, file_offset);
>>>>>>> refs/remotes/origin/master

	/*
	 * For all kernel events we get the sample data
	 */
<<<<<<< HEAD
	ret = perf_session__parse_sample(session, event, &sample);
	if (ret)
		return ret;

	/* Preprocess sample records - precheck callchains */
	if (perf_session__preprocess_sample(session, event, &sample))
		return 0;

<<<<<<< HEAD
	if (ops->ordered_samples) {
=======
	if (tool->ordered_samples) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = perf_evlist__parse_sample(session->evlist, event, &sample);
	if (ret)
		return ret;

	if (tool->ordered_samples) {
>>>>>>> refs/remotes/origin/master
		ret = perf_session_queue_event(session, event, &sample,
					       file_offset);
		if (ret != -ETIME)
			return ret;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	return perf_session_deliver_event(session, event, &sample, ops,
=======
	return perf_session_deliver_event(session, event, &sample, tool,
>>>>>>> refs/remotes/origin/cm-10.0
					  file_offset);
}

void perf_event_header__bswap(struct perf_event_header *self)
{
	self->type = bswap_32(self->type);
	self->misc = bswap_16(self->misc);
	self->size = bswap_16(self->size);
}

<<<<<<< HEAD
=======
struct thread *perf_session__findnew(struct perf_session *session, pid_t pid)
{
	return machine__findnew_thread(&session->host_machine, pid);
}

>>>>>>> refs/remotes/origin/cm-10.0
static struct thread *perf_session__register_idle_thread(struct perf_session *self)
{
	struct thread *thread = perf_session__findnew(self, 0);

	if (thread == NULL || thread__set_comm(thread, "swapper")) {
=======
	return perf_session_deliver_event(session, event, &sample, tool,
					  file_offset);
}

void perf_event_header__bswap(struct perf_event_header *hdr)
{
	hdr->type = bswap_32(hdr->type);
	hdr->misc = bswap_16(hdr->misc);
	hdr->size = bswap_16(hdr->size);
}

struct thread *perf_session__findnew(struct perf_session *session, pid_t pid)
{
	return machine__findnew_thread(&session->machines.host, 0, pid);
}

static struct thread *perf_session__register_idle_thread(struct perf_session *session)
{
	struct thread *thread = perf_session__findnew(session, 0);

	if (thread == NULL || thread__set_comm(thread, "swapper", 0)) {
>>>>>>> refs/remotes/origin/master
		pr_err("problem inserting idle task.\n");
		thread = NULL;
	}

	return thread;
}

static void perf_session__warn_about_errors(const struct perf_session *session,
<<<<<<< HEAD
<<<<<<< HEAD
					    const struct perf_event_ops *ops)
{
	if (ops->lost == perf_event__process_lost &&
	    session->hists.stats.total_lost != 0) {
		ui__warning("Processed %" PRIu64 " events and LOST %" PRIu64
			    "!\n\nCheck IO/CPU overload!\n\n",
			    session->hists.stats.total_period,
			    session->hists.stats.total_lost);
=======
					    const struct perf_tool *tool)
{
	if (tool->lost == perf_event__process_lost &&
	    session->hists.stats.nr_events[PERF_RECORD_LOST] != 0) {
		ui__warning("Processed %d events and lost %d chunks!\n\n"
			    "Check IO/CPU overload!\n\n",
			    session->hists.stats.nr_events[0],
			    session->hists.stats.nr_events[PERF_RECORD_LOST]);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	if (session->hists.stats.nr_unknown_events != 0) {
=======
					    const struct perf_tool *tool)
{
	if (tool->lost == perf_event__process_lost &&
	    session->stats.nr_events[PERF_RECORD_LOST] != 0) {
		ui__warning("Processed %d events and lost %d chunks!\n\n"
			    "Check IO/CPU overload!\n\n",
			    session->stats.nr_events[0],
			    session->stats.nr_events[PERF_RECORD_LOST]);
	}

	if (session->stats.nr_unknown_events != 0) {
>>>>>>> refs/remotes/origin/master
		ui__warning("Found %u unknown events!\n\n"
			    "Is this an older tool processing a perf.data "
			    "file generated by a more recent tool?\n\n"
			    "If that is not the case, consider "
			    "reporting to linux-kernel@vger.kernel.org.\n\n",
<<<<<<< HEAD
			    session->hists.stats.nr_unknown_events);
	}

	if (session->hists.stats.nr_unknown_id != 0) {
		ui__warning("%u samples with id not present in the header\n",
			    session->hists.stats.nr_unknown_id);
	}

 	if (session->hists.stats.nr_invalid_chains != 0) {
 		ui__warning("Found invalid callchains!\n\n"
 			    "%u out of %u events were discarded for this reason.\n\n"
 			    "Consider reporting to linux-kernel@vger.kernel.org.\n\n",
 			    session->hists.stats.nr_invalid_chains,
 			    session->hists.stats.nr_events[PERF_RECORD_SAMPLE]);
 	}
<<<<<<< HEAD
=======

	if (session->hists.stats.nr_unprocessable_samples != 0) {
		ui__warning("%u unprocessable samples recorded.\n"
			    "Do you have a KVM guest running and not using 'perf kvm'?\n",
			    session->hists.stats.nr_unprocessable_samples);
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

#define session_done()	(*(volatile int *)(&session_done))
volatile int session_done;

static int __perf_session__process_pipe_events(struct perf_session *self,
<<<<<<< HEAD
					       struct perf_event_ops *ops)
=======
					       struct perf_tool *tool)
>>>>>>> refs/remotes/origin/cm-10.0
{
	union perf_event event;
	uint32_t size;
	int skip = 0;
	u64 head;
	int err;
	void *p;

<<<<<<< HEAD
	perf_event_ops__fill_defaults(ops);
=======
	perf_tool__fill_defaults(tool);
>>>>>>> refs/remotes/origin/cm-10.0

	head = 0;
more:
	err = readn(self->fd, &event, sizeof(struct perf_event_header));
=======
			    session->stats.nr_unknown_events);
	}

	if (session->stats.nr_unknown_id != 0) {
		ui__warning("%u samples with id not present in the header\n",
			    session->stats.nr_unknown_id);
	}

 	if (session->stats.nr_invalid_chains != 0) {
 		ui__warning("Found invalid callchains!\n\n"
 			    "%u out of %u events were discarded for this reason.\n\n"
 			    "Consider reporting to linux-kernel@vger.kernel.org.\n\n",
 			    session->stats.nr_invalid_chains,
 			    session->stats.nr_events[PERF_RECORD_SAMPLE]);
 	}

	if (session->stats.nr_unprocessable_samples != 0) {
		ui__warning("%u unprocessable samples recorded.\n"
			    "Do you have a KVM guest running and not using 'perf kvm'?\n",
			    session->stats.nr_unprocessable_samples);
	}
}

volatile int session_done;

static int __perf_session__process_pipe_events(struct perf_session *session,
					       struct perf_tool *tool)
{
	int fd = perf_data_file__fd(session->file);
	union perf_event *event;
	uint32_t size, cur_size = 0;
	void *buf = NULL;
	int skip = 0;
	u64 head;
	ssize_t err;
	void *p;

	perf_tool__fill_defaults(tool);

	head = 0;
	cur_size = sizeof(union perf_event);

	buf = malloc(cur_size);
	if (!buf)
		return -errno;
more:
	event = buf;
	err = readn(fd, event, sizeof(struct perf_event_header));
>>>>>>> refs/remotes/origin/master
	if (err <= 0) {
		if (err == 0)
			goto done;

		pr_err("failed to read event header\n");
		goto out_err;
	}

<<<<<<< HEAD
	if (self->header.needs_swap)
		perf_event_header__bswap(&event.header);

	size = event.header.size;
	if (size == 0)
		size = 8;

	p = &event;
	p += sizeof(struct perf_event_header);

	if (size - sizeof(struct perf_event_header)) {
		err = readn(self->fd, p, size - sizeof(struct perf_event_header));
=======
	if (session->header.needs_swap)
		perf_event_header__bswap(&event->header);

	size = event->header.size;
	if (size < sizeof(struct perf_event_header)) {
		pr_err("bad event header size\n");
		goto out_err;
	}

	if (size > cur_size) {
		void *new = realloc(buf, size);
		if (!new) {
			pr_err("failed to allocate memory to read event\n");
			goto out_err;
		}
		buf = new;
		cur_size = size;
		event = buf;
	}
	p = event;
	p += sizeof(struct perf_event_header);

	if (size - sizeof(struct perf_event_header)) {
		err = readn(fd, p, size - sizeof(struct perf_event_header));
>>>>>>> refs/remotes/origin/master
		if (err <= 0) {
			if (err == 0) {
				pr_err("unexpected end of event stream\n");
				goto done;
			}

			pr_err("failed to read event data\n");
			goto out_err;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (size == 0 ||
	    (skip = perf_session__process_event(self, &event, ops, head)) < 0) {
=======
	if ((skip = perf_session__process_event(self, &event, tool, head)) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		dump_printf("%#" PRIx64 " [%#x]: skipping unknown header type: %d\n",
			    head, event.header.size, event.header.type);
		/*
		 * assume we lost track of the stream, check alignment, and
		 * increment a single u64 in the hope to catch on again 'soon'.
		 */
		if (unlikely(head & 7))
			head &= ~7ULL;

		size = 8;
=======
	if ((skip = perf_session__process_event(session, event, tool, head)) < 0) {
		pr_err("%#" PRIx64 " [%#x]: failed to process type: %d\n",
		       head, event->header.size, event->header.type);
		err = -EINVAL;
		goto out_err;
>>>>>>> refs/remotes/origin/master
	}

	head += size;

	if (skip > 0)
		head += skip;

	if (!session_done())
		goto more;
done:
<<<<<<< HEAD
	err = 0;
out_err:
<<<<<<< HEAD
	perf_session__warn_about_errors(self, ops);
=======
	perf_session__warn_about_errors(self, tool);
>>>>>>> refs/remotes/origin/cm-10.0
	perf_session_free_sample_buffers(self);
=======
	/* do the final flush for ordered samples */
	session->ordered_samples.next_flush = ULLONG_MAX;
	err = flush_sample_queue(session, tool);
out_err:
	free(buf);
	perf_session__warn_about_errors(session, tool);
	perf_session_free_sample_buffers(session);
>>>>>>> refs/remotes/origin/master
	return err;
}

static union perf_event *
fetch_mmaped_event(struct perf_session *session,
		   u64 head, size_t mmap_size, char *buf)
{
	union perf_event *event;

	/*
	 * Ensure we have enough space remaining to read
	 * the size of the event in the headers.
	 */
	if (head + sizeof(event->header) > mmap_size)
		return NULL;

	event = (union perf_event *)(buf + head);

	if (session->header.needs_swap)
		perf_event_header__bswap(&event->header);

<<<<<<< HEAD
	if (head + event->header.size > mmap_size)
		return NULL;
=======
	if (head + event->header.size > mmap_size) {
		/* We're not fetching the event so swap back again */
		if (session->header.needs_swap)
			perf_event_header__bswap(&event->header);
		return NULL;
	}
>>>>>>> refs/remotes/origin/master

	return event;
}

<<<<<<< HEAD
int __perf_session__process_events(struct perf_session *session,
				   u64 data_offset, u64 data_size,
<<<<<<< HEAD
				   u64 file_size, struct perf_event_ops *ops)
{
	u64 head, page_offset, file_offset, file_pos, progress_next;
	int err, mmap_prot, mmap_flags, map_idx = 0;
	struct ui_progress *progress;
=======
				   u64 file_size, struct perf_tool *tool)
{
	u64 head, page_offset, file_offset, file_pos, progress_next;
	int err, mmap_prot, mmap_flags, map_idx = 0;
>>>>>>> refs/remotes/origin/cm-10.0
	size_t	page_size, mmap_size;
	char *buf, *mmaps[8];
	union perf_event *event;
	uint32_t size;

<<<<<<< HEAD
	perf_event_ops__fill_defaults(ops);
=======
	perf_tool__fill_defaults(tool);
>>>>>>> refs/remotes/origin/cm-10.0

	page_size = sysconf(_SC_PAGESIZE);
=======
/*
 * On 64bit we can mmap the data file in one go. No need for tiny mmap
 * slices. On 32bit we use 32MB.
 */
#if BITS_PER_LONG == 64
#define MMAP_SIZE ULLONG_MAX
#define NUM_MMAPS 1
#else
#define MMAP_SIZE (32 * 1024 * 1024ULL)
#define NUM_MMAPS 128
#endif

int __perf_session__process_events(struct perf_session *session,
				   u64 data_offset, u64 data_size,
				   u64 file_size, struct perf_tool *tool)
{
	int fd = perf_data_file__fd(session->file);
	u64 head, page_offset, file_offset, file_pos;
	int err, mmap_prot, mmap_flags, map_idx = 0;
	size_t	mmap_size;
	char *buf, *mmaps[NUM_MMAPS];
	union perf_event *event;
	uint32_t size;
	struct ui_progress prog;

	perf_tool__fill_defaults(tool);
>>>>>>> refs/remotes/origin/master

	page_offset = page_size * (data_offset / page_size);
	file_offset = page_offset;
	head = data_offset - page_offset;

<<<<<<< HEAD
	if (data_offset + data_size < file_size)
		file_size = data_offset + data_size;

	progress_next = file_size / 16;
<<<<<<< HEAD
	progress = ui_progress__new("Processing events...", file_size);
	if (progress == NULL)
		return -1;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	mmap_size = session->mmap_window;
=======
	if (data_size && (data_offset + data_size < file_size))
		file_size = data_offset + data_size;

	ui_progress__init(&prog, file_size, "Processing events...");

	mmap_size = MMAP_SIZE;
>>>>>>> refs/remotes/origin/master
	if (mmap_size > file_size)
		mmap_size = file_size;

	memset(mmaps, 0, sizeof(mmaps));

	mmap_prot  = PROT_READ;
	mmap_flags = MAP_SHARED;

	if (session->header.needs_swap) {
		mmap_prot  |= PROT_WRITE;
		mmap_flags = MAP_PRIVATE;
	}
remap:
<<<<<<< HEAD
	buf = mmap(NULL, mmap_size, mmap_prot, mmap_flags, session->fd,
=======
	buf = mmap(NULL, mmap_size, mmap_prot, mmap_flags, fd,
>>>>>>> refs/remotes/origin/master
		   file_offset);
	if (buf == MAP_FAILED) {
		pr_err("failed to mmap file\n");
		err = -errno;
		goto out_err;
	}
	mmaps[map_idx] = buf;
	map_idx = (map_idx + 1) & (ARRAY_SIZE(mmaps) - 1);
	file_pos = file_offset + head;

more:
	event = fetch_mmaped_event(session, head, mmap_size, buf);
	if (!event) {
		if (mmaps[map_idx]) {
			munmap(mmaps[map_idx], mmap_size);
			mmaps[map_idx] = NULL;
		}

		page_offset = page_size * (head / page_size);
		file_offset += page_offset;
		head -= page_offset;
		goto remap;
	}

	size = event->header.size;

<<<<<<< HEAD
	if (size == 0 ||
<<<<<<< HEAD
	    perf_session__process_event(session, event, ops, file_pos) < 0) {
=======
	    perf_session__process_event(session, event, tool, file_pos) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		dump_printf("%#" PRIx64 " [%#x]: skipping unknown header type: %d\n",
			    file_offset + head, event->header.size,
			    event->header.type);
		/*
		 * assume we lost track of the stream, check alignment, and
		 * increment a single u64 in the hope to catch on again 'soon'.
		 */
		if (unlikely(head & 7))
			head &= ~7ULL;

		size = 8;
=======
	if (size < sizeof(struct perf_event_header) ||
	    perf_session__process_event(session, event, tool, file_pos) < 0) {
		pr_err("%#" PRIx64 " [%#x]: failed to process type: %d\n",
		       file_offset + head, event->header.size,
		       event->header.type);
		err = -EINVAL;
		goto out_err;
>>>>>>> refs/remotes/origin/master
	}

	head += size;
	file_pos += size;

<<<<<<< HEAD
	if (file_pos >= progress_next) {
		progress_next += file_size / 16;
<<<<<<< HEAD
		ui_progress__update(progress, file_pos);
=======
		ui_progress__update(file_pos, file_size,
				    "Processing events...");
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
	ui_progress__update(&prog, size);

	if (session_done())
		goto out;
>>>>>>> refs/remotes/origin/master

	if (file_pos < file_size)
		goto more;

<<<<<<< HEAD
	err = 0;
	/* do the final flush for ordered samples */
	session->ordered_samples.next_flush = ULLONG_MAX;
<<<<<<< HEAD
	flush_sample_queue(session, ops);
out_err:
	ui_progress__delete(progress);
	perf_session__warn_about_errors(session, ops);
=======
	flush_sample_queue(session, tool);
out_err:
	perf_session__warn_about_errors(session, tool);
>>>>>>> refs/remotes/origin/cm-10.0
=======
out:
	/* do the final flush for ordered samples */
	session->ordered_samples.next_flush = ULLONG_MAX;
	err = flush_sample_queue(session, tool);
out_err:
	ui_progress__finish();
	perf_session__warn_about_errors(session, tool);
>>>>>>> refs/remotes/origin/master
	perf_session_free_sample_buffers(session);
	return err;
}

<<<<<<< HEAD
int perf_session__process_events(struct perf_session *self,
<<<<<<< HEAD
				 struct perf_event_ops *ops)
=======
				 struct perf_tool *tool)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int err;

	if (perf_session__register_idle_thread(self) == NULL)
		return -ENOMEM;

	if (!self->fd_pipe)
		err = __perf_session__process_events(self,
						     self->header.data_offset,
						     self->header.data_size,
<<<<<<< HEAD
						     self->size, ops);
	else
		err = __perf_session__process_pipe_events(self, ops);
=======
						     self->size, tool);
	else
		err = __perf_session__process_pipe_events(self, tool);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int perf_session__process_events(struct perf_session *session,
				 struct perf_tool *tool)
{
	u64 size = perf_data_file__size(session->file);
	int err;

	if (perf_session__register_idle_thread(session) == NULL)
		return -ENOMEM;

	if (!perf_data_file__is_pipe(session->file))
		err = __perf_session__process_events(session,
						     session->header.data_offset,
						     session->header.data_size,
						     size, tool);
	else
		err = __perf_session__process_pipe_events(session, tool);
>>>>>>> refs/remotes/origin/master

	return err;
}

<<<<<<< HEAD
bool perf_session__has_traces(struct perf_session *self, const char *msg)
{
	if (!(self->sample_type & PERF_SAMPLE_RAW)) {
		pr_err("No trace sample to read. Did you call 'perf %s'?\n", msg);
		return false;
	}

	return true;
}

<<<<<<< HEAD
int perf_session__set_kallsyms_ref_reloc_sym(struct map **maps,
					     const char *symbol_name,
					     u64 addr)
=======
int maps__set_kallsyms_ref_reloc_sym(struct map **maps,
				     const char *symbol_name, u64 addr)
>>>>>>> refs/remotes/origin/cm-10.0
=======
bool perf_session__has_traces(struct perf_session *session, const char *msg)
{
	struct perf_evsel *evsel;

	evlist__for_each(session->evlist, evsel) {
		if (evsel->attr.type == PERF_TYPE_TRACEPOINT)
			return true;
	}

	pr_err("No trace sample to read. Did you call 'perf %s'?\n", msg);
	return false;
}

int maps__set_kallsyms_ref_reloc_sym(struct map **maps,
				     const char *symbol_name, u64 addr)
>>>>>>> refs/remotes/origin/master
{
	char *bracket;
	enum map_type i;
	struct ref_reloc_sym *ref;

	ref = zalloc(sizeof(struct ref_reloc_sym));
	if (ref == NULL)
		return -ENOMEM;

	ref->name = strdup(symbol_name);
	if (ref->name == NULL) {
		free(ref);
		return -ENOMEM;
	}

	bracket = strchr(ref->name, ']');
	if (bracket)
		*bracket = '\0';

	ref->addr = addr;

	for (i = 0; i < MAP__NR_TYPES; ++i) {
		struct kmap *kmap = map__kmap(maps[i]);
		kmap->ref_reloc_sym = ref;
	}

	return 0;
}

<<<<<<< HEAD
size_t perf_session__fprintf_dsos(struct perf_session *self, FILE *fp)
{
	return __dsos__fprintf(&self->host_machine.kernel_dsos, fp) +
	       __dsos__fprintf(&self->host_machine.user_dsos, fp) +
	       machines__fprintf_dsos(&self->machines, fp);
}

size_t perf_session__fprintf_dsos_buildid(struct perf_session *self, FILE *fp,
					  bool with_hits)
{
	size_t ret = machine__fprintf_dsos_buildid(&self->host_machine, fp, with_hits);
	return ret + machines__fprintf_dsos_buildid(&self->machines, fp, with_hits);
=======
size_t perf_session__fprintf_dsos(struct perf_session *session, FILE *fp)
{
	return machines__fprintf_dsos(&session->machines, fp);
}

size_t perf_session__fprintf_dsos_buildid(struct perf_session *session, FILE *fp,
					  bool (skip)(struct dso *dso, int parm), int parm)
{
	return machines__fprintf_dsos_buildid(&session->machines, fp, skip, parm);
>>>>>>> refs/remotes/origin/master
}

size_t perf_session__fprintf_nr_events(struct perf_session *session, FILE *fp)
{
	struct perf_evsel *pos;
	size_t ret = fprintf(fp, "Aggregated stats:\n");

<<<<<<< HEAD
	ret += hists__fprintf_nr_events(&session->hists, fp);

	list_for_each_entry(pos, &session->evlist->entries, node) {
		ret += fprintf(fp, "%s stats:\n", event_name(pos));
		ret += hists__fprintf_nr_events(&pos->hists, fp);
=======
	ret += events_stats__fprintf(&session->stats, fp);

	evlist__for_each(session->evlist, pos) {
		ret += fprintf(fp, "%s stats:\n", perf_evsel__name(pos));
		ret += events_stats__fprintf(&pos->hists.stats, fp);
>>>>>>> refs/remotes/origin/master
	}

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
size_t perf_session__fprintf(struct perf_session *session, FILE *fp)
{
	/*
	 * FIXME: Here we have to actually print all the machines in this
	 * session, not just the host...
	 */
<<<<<<< HEAD
	return machine__fprintf(&session->host_machine, fp);
}

void perf_session__remove_thread(struct perf_session *session,
				 struct thread *th)
{
	/*
	 * FIXME: This one makes no sense, we need to remove the thread from
	 * the machine it belongs to, perf_session can have many machines, so
	 * doing it always on ->host_machine is wrong.  Fix when auditing all
	 * the 'perf kvm' code.
	 */
	machine__remove_thread(&session->host_machine, th);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	return machine__fprintf(&session->machines.host, fp);
}

>>>>>>> refs/remotes/origin/master
struct perf_evsel *perf_session__find_first_evtype(struct perf_session *session,
					      unsigned int type)
{
	struct perf_evsel *pos;

<<<<<<< HEAD
	list_for_each_entry(pos, &session->evlist->entries, node) {
=======
	evlist__for_each(session->evlist, pos) {
>>>>>>> refs/remotes/origin/master
		if (pos->attr.type == type)
			return pos;
	}
	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
void perf_session__print_symbols(union perf_event *event,
				struct perf_sample *sample,
				struct perf_session *session)
{
	struct addr_location al;
	const char *symname, *dsoname;
	struct callchain_cursor *cursor = &session->callchain_cursor;
	struct callchain_cursor_node *node;

	if (perf_event__preprocess_sample(event, session, &al, sample,
=======
void perf_event__print_ip(union perf_event *event, struct perf_sample *sample,
			  struct machine *machine, struct perf_evsel *evsel,
			  int print_sym, int print_dso, int print_symoffset)
{
	struct addr_location al;
	struct callchain_cursor *cursor = &evsel->hists.callchain_cursor;
	struct callchain_cursor_node *node;

	if (perf_event__preprocess_sample(event, machine, &al, sample,
>>>>>>> refs/remotes/origin/cm-10.0
					  NULL) < 0) {
		error("problem processing %d event, skipping it.\n",
			event->header.type);
		return;
	}

	if (symbol_conf.use_callchain && sample->callchain) {

<<<<<<< HEAD
		if (perf_session__resolve_callchain(session, al.thread,
=======
		if (machine__resolve_callchain(machine, evsel, al.thread,
>>>>>>> refs/remotes/origin/cm-10.0
						sample->callchain, NULL) != 0) {
=======
void perf_evsel__print_ip(struct perf_evsel *evsel, struct perf_sample *sample,
			  struct addr_location *al,
			  unsigned int print_opts, unsigned int stack_depth)
{
	struct callchain_cursor_node *node;
	int print_ip = print_opts & PRINT_IP_OPT_IP;
	int print_sym = print_opts & PRINT_IP_OPT_SYM;
	int print_dso = print_opts & PRINT_IP_OPT_DSO;
	int print_symoffset = print_opts & PRINT_IP_OPT_SYMOFFSET;
	int print_oneline = print_opts & PRINT_IP_OPT_ONELINE;
	int print_srcline = print_opts & PRINT_IP_OPT_SRCLINE;
	char s = print_oneline ? ' ' : '\t';

	if (symbol_conf.use_callchain && sample->callchain) {
		struct addr_location node_al;

		if (machine__resolve_callchain(al->machine, evsel, al->thread,
					       sample, NULL, NULL,
					       PERF_MAX_STACK_DEPTH) != 0) {
>>>>>>> refs/remotes/origin/master
			if (verbose)
				error("Failed to resolve callchain. Skipping\n");
			return;
		}
<<<<<<< HEAD
		callchain_cursor_commit(cursor);

		while (1) {
			node = callchain_cursor_current(cursor);
			if (!node)
				break;

<<<<<<< HEAD
			if (node->sym && node->sym->name)
				symname = node->sym->name;
			else
				symname = "";

			if (node->map && node->map->dso && node->map->dso->name)
				dsoname = node->map->dso->name;
			else
				dsoname = "";

			printf("\t%16" PRIx64 " %s (%s)\n", node->ip, symname, dsoname);

			callchain_cursor_advance(cursor);
		}

	} else {
		if (al.sym && al.sym->name)
			symname = al.sym->name;
		else
			symname = "";

		if (al.map && al.map->dso && al.map->dso->name)
			dsoname = al.map->dso->name;
		else
			dsoname = "";

		printf("%16" PRIx64 " %s (%s)", al.addr, symname, dsoname);
	}
=======
			printf("\t%16" PRIx64, node->ip);
			if (print_sym) {
				printf(" ");
				symbol__fprintf_symname(node->sym, stdout);
			}
			if (print_dso) {
				printf(" (");
				map__fprintf_dsoname(al.map, stdout);
				printf(")");
			}
			printf("\n");

			callchain_cursor_advance(cursor);
		}

	} else {
		printf("%16" PRIx64, sample->ip);
		if (print_sym) {
			printf(" ");
			if (print_symoffset)
				symbol__fprintf_symname_offs(al.sym, &al,
							     stdout);
			else
				symbol__fprintf_symname(al.sym, stdout);
=======
		callchain_cursor_commit(&callchain_cursor);

		if (print_symoffset)
			node_al = *al;

		while (stack_depth) {
			u64 addr = 0;

			node = callchain_cursor_current(&callchain_cursor);
			if (!node)
				break;

			if (node->sym && node->sym->ignore)
				goto next;

			if (print_ip)
				printf("%c%16" PRIx64, s, node->ip);

			if (node->map)
				addr = node->map->map_ip(node->map, node->ip);

			if (print_sym) {
				printf(" ");
				if (print_symoffset) {
					node_al.addr = addr;
					node_al.map  = node->map;
					symbol__fprintf_symname_offs(node->sym, &node_al, stdout);
				} else
					symbol__fprintf_symname(node->sym, stdout);
			}

			if (print_dso) {
				printf(" (");
				map__fprintf_dsoname(node->map, stdout);
				printf(")");
			}

			if (print_srcline)
				map__fprintf_srcline(node->map, addr, "\n  ",
						     stdout);

			if (!print_oneline)
				printf("\n");

			stack_depth--;
next:
			callchain_cursor_advance(&callchain_cursor);
		}

	} else {
		if (al->sym && al->sym->ignore)
			return;

		if (print_ip)
			printf("%16" PRIx64, sample->ip);

		if (print_sym) {
			printf(" ");
			if (print_symoffset)
				symbol__fprintf_symname_offs(al->sym, al,
							     stdout);
			else
				symbol__fprintf_symname(al->sym, stdout);
>>>>>>> refs/remotes/origin/master
		}

		if (print_dso) {
			printf(" (");
<<<<<<< HEAD
			map__fprintf_dsoname(al.map, stdout);
			printf(")");
		}
=======
			map__fprintf_dsoname(al->map, stdout);
			printf(")");
		}

		if (print_srcline)
			map__fprintf_srcline(al->map, al->addr, "\n  ", stdout);
>>>>>>> refs/remotes/origin/master
	}
}

int perf_session__cpu_bitmap(struct perf_session *session,
			     const char *cpu_list, unsigned long *cpu_bitmap)
{
	int i;
	struct cpu_map *map;

	for (i = 0; i < PERF_TYPE_MAX; ++i) {
		struct perf_evsel *evsel;

		evsel = perf_session__find_first_evtype(session, i);
		if (!evsel)
			continue;

		if (!(evsel->attr.sample_type & PERF_SAMPLE_CPU)) {
			pr_err("File does not contain CPU events. "
			       "Remove -c option to proceed.\n");
			return -1;
		}
	}

	map = cpu_map__new(cpu_list);
	if (map == NULL) {
		pr_err("Invalid cpu_list\n");
		return -1;
	}

	for (i = 0; i < map->nr; i++) {
		int cpu = map->map[i];

		if (cpu >= MAX_NR_CPUS) {
			pr_err("Requested CPU %d too large. "
			       "Consider raising MAX_NR_CPUS\n", cpu);
			return -1;
		}

		set_bit(cpu, cpu_bitmap);
	}

	return 0;
}

void perf_session__fprintf_info(struct perf_session *session, FILE *fp,
				bool full)
{
<<<<<<< HEAD
=======
	int fd = perf_data_file__fd(session->file);
>>>>>>> refs/remotes/origin/master
	struct stat st;
	int ret;

	if (session == NULL || fp == NULL)
		return;

<<<<<<< HEAD
	ret = fstat(session->fd, &st);
=======
	ret = fstat(fd, &st);
>>>>>>> refs/remotes/origin/master
	if (ret == -1)
		return;

	fprintf(fp, "# ========\n");
	fprintf(fp, "# captured on: %s", ctime(&st.st_ctime));
	perf_header__fprintf_info(session, fp, full);
	fprintf(fp, "# ========\n#\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
}


int __perf_session__set_tracepoints_handlers(struct perf_session *session,
					     const struct perf_evsel_str_handler *assocs,
					     size_t nr_assocs)
{
	struct perf_evsel *evsel;
	size_t i;
	int err;

	for (i = 0; i < nr_assocs; i++) {
		/*
		 * Adding a handler for an event not in the session,
		 * just ignore it.
		 */
		evsel = perf_evlist__find_tracepoint_by_name(session->evlist, assocs[i].name);
		if (evsel == NULL)
			continue;

		err = -EEXIST;
		if (evsel->handler != NULL)
			goto out;
		evsel->handler = assocs[i].handler;
	}

	err = 0;
out:
	return err;
>>>>>>> refs/remotes/origin/master
}
