/*
 * builtin-annotate.c
 *
 * Builtin annotate command: Analyze the perf.data input file,
 * look up and read DSOs and symbol information and display
 * a histogram of results, along various sorting keys.
 */
#include "builtin.h"

#include "util/util.h"
#include "util/color.h"
#include <linux/list.h>
#include "util/cache.h"
#include <linux/rbtree.h>
#include "util/symbol.h"

#include "perf.h"
#include "util/debug.h"

#include "util/evlist.h"
#include "util/evsel.h"
#include "util/annotate.h"
#include "util/event.h"
#include "util/parse-options.h"
#include "util/parse-events.h"
#include "util/thread.h"
#include "util/sort.h"
#include "util/hist.h"
#include "util/session.h"
<<<<<<< HEAD
<<<<<<< HEAD

static char		const *input_name = "perf.data";

static bool		force, use_tui, use_stdio;

static bool		full_paths;

static bool		print_line;

static const char *sym_hist_filter;

static int perf_evlist__add_sample(struct perf_evlist *evlist,
				   struct perf_sample *sample,
				   struct perf_evsel *evsel,
				   struct addr_location *al)
=======
#include "util/tool.h"

=======
#include "util/tool.h"
#include "util/data.h"
#include "arch/common.h"

#include <dlfcn.h>
>>>>>>> refs/remotes/origin/master
#include <linux/bitmap.h>

struct perf_annotate {
	struct perf_tool tool;
<<<<<<< HEAD
	char const *input_name;
	bool	   force, use_tui, use_stdio;
	bool	   full_paths;
	bool	   print_line;
=======
	bool	   force, use_tui, use_stdio, use_gtk;
	bool	   full_paths;
	bool	   print_line;
	bool	   skip_missing;
>>>>>>> refs/remotes/origin/master
	const char *sym_hist_filter;
	const char *cpu_list;
	DECLARE_BITMAP(cpu_bitmap, MAX_NR_CPUS);
};

static int perf_evsel__add_sample(struct perf_evsel *evsel,
				  struct perf_sample *sample,
				  struct addr_location *al,
				  struct perf_annotate *ann)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct hist_entry *he;
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	if (sym_hist_filter != NULL &&
	    (al->sym == NULL || strcmp(sym_hist_filter, al->sym->name) != 0)) {
=======
	if (ann->sym_hist_filter != NULL &&
	    (al->sym == NULL ||
	     strcmp(ann->sym_hist_filter, al->sym->name) != 0)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ann->sym_hist_filter != NULL &&
	    (al->sym == NULL ||
	     strcmp(ann->sym_hist_filter, al->sym->name) != 0)) {
>>>>>>> refs/remotes/origin/master
		/* We're only interested in a symbol named sym_hist_filter */
		if (al->sym != NULL) {
			rb_erase(&al->sym->rb_node,
				 &al->map->dso->symbols[al->map->type]);
			symbol__delete(al->sym);
		}
		return 0;
	}

<<<<<<< HEAD
	he = __hists__add_entry(&evsel->hists, al, NULL, 1);
	if (he == NULL)
		return -ENOMEM;

	ret = 0;
	if (he->ms.sym != NULL) {
		struct annotation *notes = symbol__annotation(he->ms.sym);
<<<<<<< HEAD
		if (notes->src == NULL &&
		    symbol__alloc_hist(he->ms.sym, evlist->nr_entries) < 0)
=======
		if (notes->src == NULL && symbol__alloc_hist(he->ms.sym) < 0)
>>>>>>> refs/remotes/origin/cm-10.0
			return -ENOMEM;

		ret = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
	}

=======
	he = __hists__add_entry(&evsel->hists, al, NULL, NULL, NULL, 1, 1, 0);
	if (he == NULL)
		return -ENOMEM;

	ret = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
>>>>>>> refs/remotes/origin/master
	evsel->hists.stats.total_period += sample->period;
	hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int process_sample_event(union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct perf_session *session)
{
	struct addr_location al;

	if (perf_event__preprocess_sample(event, session, &al, sample,
=======
=======
>>>>>>> refs/remotes/origin/master
static int process_sample_event(struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct machine *machine)
{
	struct perf_annotate *ann = container_of(tool, struct perf_annotate, tool);
	struct addr_location al;

<<<<<<< HEAD
	if (perf_event__preprocess_sample(event, machine, &al, sample,
>>>>>>> refs/remotes/origin/cm-10.0
					  symbol__annotate_init) < 0) {
=======
	if (perf_event__preprocess_sample(event, machine, &al, sample) < 0) {
>>>>>>> refs/remotes/origin/master
		pr_warning("problem processing %d event, skipping it.\n",
			   event->header.type);
		return -1;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (!al.filtered &&
	    perf_evlist__add_sample(session->evlist, sample, evsel, &al)) {
=======
=======
>>>>>>> refs/remotes/origin/master
	if (ann->cpu_list && !test_bit(sample->cpu, ann->cpu_bitmap))
		return 0;

	if (!al.filtered && perf_evsel__add_sample(evsel, sample, &al, ann)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		pr_warning("problem incrementing symbol count, "
			   "skipping event\n");
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int hist_entry__tty_annotate(struct hist_entry *he, int evidx)
{
	return symbol__tty_annotate(he->ms.sym, he->ms.map, evidx,
				    print_line, full_paths, 0, 0);
}

static void hists__find_annotations(struct hists *self, int evidx)
{
	struct rb_node *nd = rb_first(&self->entries), *next;
	int key = KEY_RIGHT;
=======
static int hist_entry__tty_annotate(struct hist_entry *he, int evidx,
				    struct perf_annotate *ann)
{
	return symbol__tty_annotate(he->ms.sym, he->ms.map, evidx,
				    ann->print_line, ann->full_paths, 0, 0);
}

static void hists__find_annotations(struct hists *self, int evidx,
				    struct perf_annotate *ann)
{
	struct rb_node *nd = rb_first(&self->entries), *next;
	int key = K_RIGHT;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int hist_entry__tty_annotate(struct hist_entry *he,
				    struct perf_evsel *evsel,
				    struct perf_annotate *ann)
{
	return symbol__tty_annotate(he->ms.sym, he->ms.map, evsel,
				    ann->print_line, ann->full_paths, 0, 0);
}

static void hists__find_annotations(struct hists *hists,
				    struct perf_evsel *evsel,
				    struct perf_annotate *ann)
{
	struct rb_node *nd = rb_first(&hists->entries), *next;
	int key = K_RIGHT;
>>>>>>> refs/remotes/origin/master

	while (nd) {
		struct hist_entry *he = rb_entry(nd, struct hist_entry, rb_node);
		struct annotation *notes;

		if (he->ms.sym == NULL || he->ms.map->dso->annotate_warned)
			goto find_next;

		notes = symbol__annotation(he->ms.sym);
		if (notes->src == NULL) {
find_next:
<<<<<<< HEAD
<<<<<<< HEAD
			if (key == KEY_LEFT)
=======
			if (key == K_LEFT)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (key == K_LEFT)
>>>>>>> refs/remotes/origin/master
				nd = rb_prev(nd);
			else
				nd = rb_next(nd);
			continue;
		}

<<<<<<< HEAD
		if (use_browser > 0) {
<<<<<<< HEAD
			key = hist_entry__tui_annotate(he, evidx);
			switch (key) {
			case KEY_RIGHT:
				next = rb_next(nd);
				break;
			case KEY_LEFT:
=======
			key = hist_entry__tui_annotate(he, evidx, NULL, NULL, 0);
			switch (key) {
=======
		if (use_browser == 2) {
			int ret;
			int (*annotate)(struct hist_entry *he,
					struct perf_evsel *evsel,
					struct hist_browser_timer *hbt);

			annotate = dlsym(perf_gtk_handle,
					 "hist_entry__gtk_annotate");
			if (annotate == NULL) {
				ui__error("GTK browser not found!\n");
				return;
			}

			ret = annotate(he, evsel, NULL);
			if (!ret || !ann->skip_missing)
				return;

			/* skip missing symbols */
			nd = rb_next(nd);
		} else if (use_browser == 1) {
			key = hist_entry__tui_annotate(he, evsel, NULL);
			switch (key) {
			case -1:
				if (!ann->skip_missing)
					return;
				/* fall through */
>>>>>>> refs/remotes/origin/master
			case K_RIGHT:
				next = rb_next(nd);
				break;
			case K_LEFT:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				next = rb_prev(nd);
				break;
			default:
				return;
			}

			if (next != NULL)
				nd = next;
		} else {
<<<<<<< HEAD
<<<<<<< HEAD
			hist_entry__tty_annotate(he, evidx);
=======
			hist_entry__tty_annotate(he, evidx, ann);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			hist_entry__tty_annotate(he, evsel, ann);
>>>>>>> refs/remotes/origin/master
			nd = rb_next(nd);
			/*
			 * Since we have a hist_entry per IP for the same
			 * symbol, free he->ms.sym->src to signal we already
			 * processed this symbol.
			 */
<<<<<<< HEAD
			free(notes->src);
			notes->src = NULL;
=======
			zfree(&notes->src);
>>>>>>> refs/remotes/origin/master
		}
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct perf_event_ops event_ops = {
	.sample	= process_sample_event,
	.mmap	= perf_event__process_mmap,
	.comm	= perf_event__process_comm,
	.fork	= perf_event__process_task,
	.ordered_samples = true,
	.ordering_requires_timestamps = true,
};

static int __cmd_annotate(void)
=======
static int __cmd_annotate(struct perf_annotate *ann)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int __cmd_annotate(struct perf_annotate *ann)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	struct perf_session *session;
	struct perf_evsel *pos;
	u64 total_nr_samples;
<<<<<<< HEAD

<<<<<<< HEAD
	session = perf_session__new(input_name, O_RDONLY, force, false, &event_ops);
	if (session == NULL)
		return -ENOMEM;

	ret = perf_session__process_events(session, &event_ops);
=======
	session = perf_session__new(ann->input_name, O_RDONLY,
				    ann->force, false, &ann->tool);
	if (session == NULL)
		return -ENOMEM;
=======
	struct perf_data_file file = {
		.path  = input_name,
		.mode  = PERF_DATA_MODE_READ,
		.force = ann->force,
	};

	session = perf_session__new(&file, false, &ann->tool);
	if (session == NULL)
		return -ENOMEM;

	machines__set_symbol_filter(&session->machines, symbol__annotate_init);
>>>>>>> refs/remotes/origin/master

	if (ann->cpu_list) {
		ret = perf_session__cpu_bitmap(session, ann->cpu_list,
					       ann->cpu_bitmap);
		if (ret)
			goto out_delete;
	}

<<<<<<< HEAD
	ret = perf_session__process_events(session, &ann->tool);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!objdump_path) {
		ret = perf_session_env__lookup_objdump(&session->header.env);
		if (ret)
			goto out_delete;
	}

	ret = perf_session__process_events(session, &ann->tool);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto out_delete;

	if (dump_trace) {
		perf_session__fprintf_nr_events(session, stdout);
		goto out_delete;
	}

	if (verbose > 3)
		perf_session__fprintf(session, stdout);

	if (verbose > 2)
		perf_session__fprintf_dsos(session, stdout);

	total_nr_samples = 0;
<<<<<<< HEAD
	list_for_each_entry(pos, &session->evlist->entries, node) {
=======
	evlist__for_each(session->evlist, pos) {
>>>>>>> refs/remotes/origin/master
		struct hists *hists = &pos->hists;
		u32 nr_samples = hists->stats.nr_events[PERF_RECORD_SAMPLE];

		if (nr_samples > 0) {
			total_nr_samples += nr_samples;
<<<<<<< HEAD
			hists__collapse_resort(hists);
			hists__output_resort(hists);
<<<<<<< HEAD
			hists__find_annotations(hists, pos->idx);
=======
			hists__find_annotations(hists, pos->idx, ann);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			hists__collapse_resort(hists, NULL);
			hists__output_resort(hists);

			if (symbol_conf.event_group &&
			    !perf_evsel__is_group_leader(pos))
				continue;

			hists__find_annotations(hists, pos, ann);
>>>>>>> refs/remotes/origin/master
		}
	}

	if (total_nr_samples == 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		ui__warning("The %s file has no samples!\n", input_name);
=======
		ui__warning("The %s file has no samples!\n", session->filename);
>>>>>>> refs/remotes/origin/cm-10.0
		goto out_delete;
	}
=======
		ui__error("The %s file has no samples!\n", file.path);
		goto out_delete;
	}

	if (use_browser == 2) {
		void (*show_annotations)(void);

		show_annotations = dlsym(perf_gtk_handle,
					 "perf_gtk__show_annotations");
		if (show_annotations == NULL) {
			ui__error("GTK browser not found!\n");
			goto out_delete;
		}
		show_annotations();
	}

>>>>>>> refs/remotes/origin/master
out_delete:
	/*
	 * Speed up the exit process, for large files this can
	 * take quite a while.
	 *
	 * XXX Enable this when using valgrind or if we ever
	 * librarize this command.
	 *
	 * Also experiment with obstacks to see how much speed
	 * up we'll get here.
	 *
	 * perf_session__delete(session);
	 */
	return ret;
}

static const char * const annotate_usage[] = {
<<<<<<< HEAD
<<<<<<< HEAD
	"perf annotate [<options>] <command>",
	NULL
};

static const struct option options[] = {
	OPT_STRING('i', "input", &input_name, "file",
		    "input file name"),
	OPT_STRING('d', "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING('s', "symbol", &sym_hist_filter, "symbol",
		    "symbol to annotate"),
	OPT_BOOLEAN('f', "force", &force, "don't complain, do it"),
=======
=======
>>>>>>> refs/remotes/origin/master
	"perf annotate [<options>]",
	NULL
};

<<<<<<< HEAD
int cmd_annotate(int argc, const char **argv, const char *prefix __used)
=======
int cmd_annotate(int argc, const char **argv, const char *prefix __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	struct perf_annotate annotate = {
		.tool = {
			.sample	= process_sample_event,
			.mmap	= perf_event__process_mmap,
<<<<<<< HEAD
			.comm	= perf_event__process_comm,
			.fork	= perf_event__process_task,
=======
			.mmap2	= perf_event__process_mmap2,
			.comm	= perf_event__process_comm,
			.exit	= perf_event__process_exit,
			.fork	= perf_event__process_fork,
>>>>>>> refs/remotes/origin/master
			.ordered_samples = true,
			.ordering_requires_timestamps = true,
		},
	};
	const struct option options[] = {
<<<<<<< HEAD
	OPT_STRING('i', "input", &annotate.input_name, "file",
=======
	OPT_STRING('i', "input", &input_name, "file",
>>>>>>> refs/remotes/origin/master
		    "input file name"),
	OPT_STRING('d', "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING('s', "symbol", &annotate.sym_hist_filter, "symbol",
		    "symbol to annotate"),
	OPT_BOOLEAN('f', "force", &annotate.force, "don't complain, do it"),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
<<<<<<< HEAD
<<<<<<< HEAD
	OPT_BOOLEAN(0, "tui", &use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "stdio", &use_stdio, "Use the stdio interface"),
=======
	OPT_BOOLEAN(0, "tui", &annotate.use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "stdio", &annotate.use_stdio, "Use the stdio interface"),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	OPT_BOOLEAN(0, "gtk", &annotate.use_gtk, "Use the GTK interface"),
	OPT_BOOLEAN(0, "tui", &annotate.use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "stdio", &annotate.use_stdio, "Use the stdio interface"),
>>>>>>> refs/remotes/origin/master
	OPT_STRING('k', "vmlinux", &symbol_conf.vmlinux_name,
		   "file", "vmlinux pathname"),
	OPT_BOOLEAN('m', "modules", &symbol_conf.use_modules,
		    "load module symbols - WARNING: use only with -k and LIVE kernel"),
<<<<<<< HEAD
<<<<<<< HEAD
	OPT_BOOLEAN('l', "print-line", &print_line,
		    "print matching source lines (may be slow)"),
	OPT_BOOLEAN('P', "full-paths", &full_paths,
		    "Don't shorten the displayed pathnames"),
	OPT_END()
};

int cmd_annotate(int argc, const char **argv, const char *prefix __used)
{
	argc = parse_options(argc, argv, options, annotate_usage, 0);

	if (use_stdio)
		use_browser = 0;
	else if (use_tui)
=======
=======
>>>>>>> refs/remotes/origin/master
	OPT_BOOLEAN('l', "print-line", &annotate.print_line,
		    "print matching source lines (may be slow)"),
	OPT_BOOLEAN('P', "full-paths", &annotate.full_paths,
		    "Don't shorten the displayed pathnames"),
<<<<<<< HEAD
=======
	OPT_BOOLEAN(0, "skip-missing", &annotate.skip_missing,
		    "Skip symbols that cannot be annotated"),
>>>>>>> refs/remotes/origin/master
	OPT_STRING('C', "cpu", &annotate.cpu_list, "cpu", "list of cpus to profile"),
	OPT_STRING(0, "symfs", &symbol_conf.symfs, "directory",
		   "Look for files with symbols relative to this directory"),
	OPT_BOOLEAN(0, "source", &symbol_conf.annotate_src,
		    "Interleave source code with assembly code (default)"),
	OPT_BOOLEAN(0, "asm-raw", &symbol_conf.annotate_asm_raw,
		    "Display raw encoding of assembly instructions (default)"),
	OPT_STRING('M', "disassembler-style", &disassembler_style, "disassembler style",
		   "Specify disassembler style (e.g. -M intel for intel syntax)"),
<<<<<<< HEAD
=======
	OPT_STRING(0, "objdump", &objdump_path, "path",
		   "objdump binary to use for disassembly and annotations"),
	OPT_BOOLEAN(0, "group", &symbol_conf.event_group,
		    "Show event group information together"),
>>>>>>> refs/remotes/origin/master
	OPT_END()
	};

	argc = parse_options(argc, argv, options, annotate_usage, 0);

	if (annotate.use_stdio)
		use_browser = 0;
	else if (annotate.use_tui)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		use_browser = 1;
=======
		use_browser = 1;
	else if (annotate.use_gtk)
		use_browser = 2;
>>>>>>> refs/remotes/origin/master

	setup_browser(true);

	symbol_conf.priv_size = sizeof(struct annotation);
	symbol_conf.try_vmlinux_path = true;

	if (symbol__init() < 0)
		return -1;

<<<<<<< HEAD
	setup_sorting(annotate_usage, options);

	if (argc) {
		/*
		 * Special case: if there's an argument left then assume tha
=======
	if (setup_sorting() < 0)
		usage_with_options(annotate_usage, options);

	if (argc) {
		/*
		 * Special case: if there's an argument left then assume that
>>>>>>> refs/remotes/origin/master
		 * it's a symbol filter:
		 */
		if (argc > 1)
			usage_with_options(annotate_usage, options);

<<<<<<< HEAD
<<<<<<< HEAD
		sym_hist_filter = argv[0];
	}

	if (field_sep && *field_sep == '.') {
		pr_err("'.' is the only non valid --field-separator argument\n");
		return -1;
	}

	return __cmd_annotate();
=======
=======
>>>>>>> refs/remotes/origin/master
		annotate.sym_hist_filter = argv[0];
	}

	return __cmd_annotate(&annotate);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
