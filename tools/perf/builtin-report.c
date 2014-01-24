/*
 * builtin-report.c
 *
 * Builtin report command: Analyze the perf.data input file,
 * look up and read DSOs and symbol information and display
 * a histogram of results, along various sorting keys.
 */
#include "builtin.h"

#include "util/util.h"

#include "util/annotate.h"
#include "util/color.h"
#include <linux/list.h>
#include "util/cache.h"
#include <linux/rbtree.h>
#include "util/symbol.h"
#include "util/callchain.h"
#include "util/strlist.h"
#include "util/values.h"

#include "perf.h"
#include "util/debug.h"
#include "util/evlist.h"
#include "util/evsel.h"
#include "util/header.h"
#include "util/session.h"
<<<<<<< HEAD
=======
#include "util/tool.h"
>>>>>>> refs/remotes/origin/cm-10.0

#include "util/parse-options.h"
#include "util/parse-events.h"

#include "util/thread.h"
#include "util/sort.h"
#include "util/hist.h"

<<<<<<< HEAD
static char		const *input_name = "perf.data";

static bool		force, use_tui, use_stdio;
static bool		hide_unresolved;
static bool		dont_use_callchains;

static bool		show_threads;
static struct perf_read_values	show_threads_values;

static const char	default_pretty_printing_style[] = "normal";
static const char	*pretty_printing_style = default_pretty_printing_style;

static char		callchain_default_opt[] = "fractal,0.5";
static symbol_filter_t	annotate_init;

static int perf_session__add_hist_entry(struct perf_session *session,
					struct addr_location *al,
					struct perf_sample *sample,
					struct perf_evsel *evsel)
=======
#include <linux/bitmap.h>

struct perf_report {
	struct perf_tool	tool;
	struct perf_session	*session;
	char const		*input_name;
	bool			force, use_tui, use_gtk, use_stdio;
	bool			hide_unresolved;
	bool			dont_use_callchains;
	bool			show_full_info;
	bool			show_threads;
	bool			inverted_callchain;
	struct perf_read_values	show_threads_values;
	const char		*pretty_printing_style;
	symbol_filter_t		annotate_init;
	const char		*cpu_list;
	const char		*symbol_filter_str;
	DECLARE_BITMAP(cpu_bitmap, MAX_NR_CPUS);
};

static int perf_report__add_branch_hist_entry(struct perf_tool *tool,
					struct addr_location *al,
					struct perf_sample *sample,
					struct perf_evsel *evsel,
				      struct machine *machine)
{
	struct perf_report *rep = container_of(tool, struct perf_report, tool);
	struct symbol *parent = NULL;
	int err = 0;
	unsigned i;
	struct hist_entry *he;
	struct branch_info *bi, *bx;

	if ((sort__has_parent || symbol_conf.use_callchain)
	    && sample->callchain) {
		err = machine__resolve_callchain(machine, evsel, al->thread,
						 sample->callchain, &parent);
		if (err)
			return err;
	}

	bi = machine__resolve_bstack(machine, al->thread,
				     sample->branch_stack);
	if (!bi)
		return -ENOMEM;

	for (i = 0; i < sample->branch_stack->nr; i++) {
		if (rep->hide_unresolved && !(bi[i].from.sym && bi[i].to.sym))
			continue;
		/*
		 * The report shows the percentage of total branches captured
		 * and not events sampled. Thus we use a pseudo period of 1.
		 */
		he = __hists__add_branch_entry(&evsel->hists, al, parent,
				&bi[i], 1);
		if (he) {
			struct annotation *notes;
			err = -ENOMEM;
			bx = he->branch_info;
			if (bx->from.sym && use_browser > 0) {
				notes = symbol__annotation(bx->from.sym);
				if (!notes->src
				    && symbol__alloc_hist(bx->from.sym) < 0)
					goto out;

				err = symbol__inc_addr_samples(bx->from.sym,
							       bx->from.map,
							       evsel->idx,
							       bx->from.al_addr);
				if (err)
					goto out;
			}

			if (bx->to.sym && use_browser > 0) {
				notes = symbol__annotation(bx->to.sym);
				if (!notes->src
				    && symbol__alloc_hist(bx->to.sym) < 0)
					goto out;

				err = symbol__inc_addr_samples(bx->to.sym,
							       bx->to.map,
							       evsel->idx,
							       bx->to.al_addr);
				if (err)
					goto out;
			}
			evsel->hists.stats.total_period += 1;
			hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
			err = 0;
		} else
			return -ENOMEM;
	}
out:
	return err;
}

static int perf_evsel__add_hist_entry(struct perf_evsel *evsel,
				      struct addr_location *al,
				      struct perf_sample *sample,
				      struct machine *machine)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct symbol *parent = NULL;
	int err = 0;
	struct hist_entry *he;

	if ((sort__has_parent || symbol_conf.use_callchain) && sample->callchain) {
<<<<<<< HEAD
		err = perf_session__resolve_callchain(session, al->thread,
						      sample->callchain, &parent);
=======
		err = machine__resolve_callchain(machine, evsel, al->thread,
						 sample->callchain, &parent);
>>>>>>> refs/remotes/origin/cm-10.0
		if (err)
			return err;
	}

	he = __hists__add_entry(&evsel->hists, al, parent, sample->period);
	if (he == NULL)
		return -ENOMEM;

	if (symbol_conf.use_callchain) {
<<<<<<< HEAD
		err = callchain_append(he->callchain, &session->callchain_cursor,
=======
		err = callchain_append(he->callchain,
				       &evsel->hists.callchain_cursor,
>>>>>>> refs/remotes/origin/cm-10.0
				       sample->period);
		if (err)
			return err;
	}
	/*
	 * Only in the newt browser we are doing integrated annotation,
	 * so we don't allocated the extra space needed because the stdio
	 * code will not use it.
	 */
	if (al->sym != NULL && use_browser > 0) {
		struct annotation *notes = symbol__annotation(he->ms.sym);

		assert(evsel != NULL);

		err = -ENOMEM;
<<<<<<< HEAD
		if (notes->src == NULL &&
		    symbol__alloc_hist(he->ms.sym, session->evlist->nr_entries) < 0)
=======
		if (notes->src == NULL && symbol__alloc_hist(he->ms.sym) < 0)
>>>>>>> refs/remotes/origin/cm-10.0
			goto out;

		err = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
	}

	evsel->hists.stats.total_period += sample->period;
	hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
out:
	return err;
}


<<<<<<< HEAD
static int process_sample_event(union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct perf_session *session)
{
	struct addr_location al;

	if (perf_event__preprocess_sample(event, session, &al, sample,
					  annotate_init) < 0) {
=======
static int process_sample_event(struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct machine *machine)
{
	struct perf_report *rep = container_of(tool, struct perf_report, tool);
	struct addr_location al;

	if (perf_event__preprocess_sample(event, machine, &al, sample,
					  rep->annotate_init) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		fprintf(stderr, "problem processing %d event, skipping it.\n",
			event->header.type);
		return -1;
	}

<<<<<<< HEAD
	if (al.filtered || (hide_unresolved && al.sym == NULL))
		return 0;

	if (al.map != NULL)
		al.map->dso->hit = 1;

	if (perf_session__add_hist_entry(session, &al, sample, evsel)) {
		pr_debug("problem incrementing symbol period, skipping event\n");
		return -1;
	}

	return 0;
}

static int process_read_event(union perf_event *event,
			      struct perf_sample *sample __used,
			      struct perf_session *session)
{
	struct perf_evsel *evsel = perf_evlist__id2evsel(session->evlist,
							 event->read.id);
	if (show_threads) {
		const char *name = evsel ? event_name(evsel) : "unknown";
		perf_read_values_add_value(&show_threads_values,
=======
	if (al.filtered || (rep->hide_unresolved && al.sym == NULL))
		return 0;

	if (rep->cpu_list && !test_bit(sample->cpu, rep->cpu_bitmap))
		return 0;

	if (sort__branch_mode == 1) {
		if (perf_report__add_branch_hist_entry(tool, &al, sample,
						       evsel, machine)) {
			pr_debug("problem adding lbr entry, skipping event\n");
			return -1;
		}
	} else {
		if (al.map != NULL)
			al.map->dso->hit = 1;

		if (perf_evsel__add_hist_entry(evsel, &al, sample, machine)) {
			pr_debug("problem incrementing symbol period, skipping event\n");
			return -1;
		}
	}
	return 0;
}

static int process_read_event(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample __used,
			      struct perf_evsel *evsel,
			      struct machine *machine __used)
{
	struct perf_report *rep = container_of(tool, struct perf_report, tool);

	if (rep->show_threads) {
		const char *name = evsel ? event_name(evsel) : "unknown";
		perf_read_values_add_value(&rep->show_threads_values,
>>>>>>> refs/remotes/origin/cm-10.0
					   event->read.pid, event->read.tid,
					   event->read.id,
					   name,
					   event->read.value);
	}

	dump_printf(": %d %d %s %" PRIu64 "\n", event->read.pid, event->read.tid,
		    evsel ? event_name(evsel) : "FAIL",
		    event->read.value);

	return 0;
}

<<<<<<< HEAD
static int perf_session__setup_sample_type(struct perf_session *self)
{
	if (!(self->sample_type & PERF_SAMPLE_CALLCHAIN)) {
		if (sort__has_parent) {
			fprintf(stderr, "selected --sort parent, but no"
					" callchain data. Did you call"
					" perf record without -g?\n");
			return -EINVAL;
		}
		if (symbol_conf.use_callchain) {
			fprintf(stderr, "selected -g but no callchain data."
					" Did you call perf record without"
					" -g?\n");
			return -1;
		}
	} else if (!dont_use_callchains && callchain_param.mode != CHAIN_NONE &&
		   !symbol_conf.use_callchain) {
			symbol_conf.use_callchain = true;
			if (callchain_register_param(&callchain_param) < 0) {
				fprintf(stderr, "Can't register callchain"
						" params\n");
=======
static int perf_report__setup_sample_type(struct perf_report *rep)
{
	struct perf_session *self = rep->session;

	if (!(self->sample_type & PERF_SAMPLE_CALLCHAIN)) {
		if (sort__has_parent) {
			ui__warning("Selected --sort parent, but no "
				    "callchain data. Did you call "
				    "'perf record' without -g?\n");
			return -EINVAL;
		}
		if (symbol_conf.use_callchain) {
			ui__warning("Selected -g but no callchain data. Did "
				    "you call 'perf record' without -g?\n");
			return -1;
		}
	} else if (!rep->dont_use_callchains &&
		   callchain_param.mode != CHAIN_NONE &&
		   !symbol_conf.use_callchain) {
			symbol_conf.use_callchain = true;
			if (callchain_register_param(&callchain_param) < 0) {
				ui__warning("Can't register callchain "
					    "params.\n");
>>>>>>> refs/remotes/origin/cm-10.0
				return -EINVAL;
			}
	}

<<<<<<< HEAD
	return 0;
}

static struct perf_event_ops event_ops = {
	.sample		 = process_sample_event,
	.mmap		 = perf_event__process_mmap,
	.comm		 = perf_event__process_comm,
	.exit		 = perf_event__process_task,
	.fork		 = perf_event__process_task,
	.lost		 = perf_event__process_lost,
	.read		 = process_read_event,
	.attr		 = perf_event__process_attr,
	.event_type	 = perf_event__process_event_type,
	.tracing_data	 = perf_event__process_tracing_data,
	.build_id	 = perf_event__process_build_id,
	.ordered_samples = true,
	.ordering_requires_timestamps = true,
};

=======
	if (sort__branch_mode == 1) {
		if (!(self->sample_type & PERF_SAMPLE_BRANCH_STACK)) {
			fprintf(stderr, "selected -b but no branch data."
					" Did you call perf record without"
					" -b?\n");
			return -1;
		}
	}

	return 0;
}

>>>>>>> refs/remotes/origin/cm-10.0
extern volatile int session_done;

static void sig_handler(int sig __used)
{
	session_done = 1;
}

static size_t hists__fprintf_nr_sample_events(struct hists *self,
					      const char *evname, FILE *fp)
{
	size_t ret;
	char unit;
	unsigned long nr_events = self->stats.nr_events[PERF_RECORD_SAMPLE];

	nr_events = convert_unit(nr_events, &unit);
	ret = fprintf(fp, "# Events: %lu%c", nr_events, unit);
	if (evname != NULL)
		ret += fprintf(fp, " %s", evname);
	return ret + fprintf(fp, "\n#\n");
}

static int perf_evlist__tty_browse_hists(struct perf_evlist *evlist,
<<<<<<< HEAD
=======
					 struct perf_report *rep,
>>>>>>> refs/remotes/origin/cm-10.0
					 const char *help)
{
	struct perf_evsel *pos;

	list_for_each_entry(pos, &evlist->entries, node) {
		struct hists *hists = &pos->hists;
<<<<<<< HEAD
		const char *evname = NULL;

		if (rb_first(&hists->entries) != rb_last(&hists->entries))
			evname = event_name(pos);

		hists__fprintf_nr_sample_events(hists, evname, stdout);
		hists__fprintf(hists, NULL, false, stdout);
=======
		const char *evname = event_name(pos);

		hists__fprintf_nr_sample_events(hists, evname, stdout);
		hists__fprintf(hists, NULL, false, true, 0, 0, stdout);
>>>>>>> refs/remotes/origin/cm-10.0
		fprintf(stdout, "\n\n");
	}

	if (sort_order == default_sort_order &&
	    parent_pattern == default_parent_pattern) {
		fprintf(stdout, "#\n# (%s)\n#\n", help);

<<<<<<< HEAD
		if (show_threads) {
			bool style = !strcmp(pretty_printing_style, "raw");
			perf_read_values_display(stdout, &show_threads_values,
						 style);
			perf_read_values_destroy(&show_threads_values);
=======
		if (rep->show_threads) {
			bool style = !strcmp(rep->pretty_printing_style, "raw");
			perf_read_values_display(stdout, &rep->show_threads_values,
						 style);
			perf_read_values_destroy(&rep->show_threads_values);
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}

	return 0;
}

<<<<<<< HEAD
static int __cmd_report(void)
{
	int ret = -EINVAL;
	u64 nr_samples;
	struct perf_session *session;
=======
static int __cmd_report(struct perf_report *rep)
{
	int ret = -EINVAL;
	u64 nr_samples;
	struct perf_session *session = rep->session;
>>>>>>> refs/remotes/origin/cm-10.0
	struct perf_evsel *pos;
	struct map *kernel_map;
	struct kmap *kernel_kmap;
	const char *help = "For a higher level overview, try: perf report --sort comm,dso";

	signal(SIGINT, sig_handler);

<<<<<<< HEAD
	session = perf_session__new(input_name, O_RDONLY, force, false, &event_ops);
	if (session == NULL)
		return -ENOMEM;

	if (show_threads)
		perf_read_values_init(&show_threads_values);

	ret = perf_session__setup_sample_type(session);
	if (ret)
		goto out_delete;

	ret = perf_session__process_events(session, &event_ops);
=======
	if (rep->cpu_list) {
		ret = perf_session__cpu_bitmap(session, rep->cpu_list,
					       rep->cpu_bitmap);
		if (ret)
			goto out_delete;
	}

	if (use_browser <= 0)
		perf_session__fprintf_info(session, stdout, rep->show_full_info);

	if (rep->show_threads)
		perf_read_values_init(&rep->show_threads_values);

	ret = perf_report__setup_sample_type(rep);
	if (ret)
		goto out_delete;

	ret = perf_session__process_events(session, &rep->tool);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		goto out_delete;

	kernel_map = session->host_machine.vmlinux_maps[MAP__FUNCTION];
	kernel_kmap = map__kmap(kernel_map);
	if (kernel_map == NULL ||
	    (kernel_map->dso->hit &&
	     (kernel_kmap->ref_reloc_sym == NULL ||
	      kernel_kmap->ref_reloc_sym->addr == 0))) {
<<<<<<< HEAD
		const struct dso *kdso = kernel_map->dso;
=======
		const char *desc =
		    "As no suitable kallsyms nor vmlinux was found, kernel samples\n"
		    "can't be resolved.";

		if (kernel_map) {
			const struct dso *kdso = kernel_map->dso;
			if (!RB_EMPTY_ROOT(&kdso->symbols[MAP__FUNCTION])) {
				desc = "If some relocation was applied (e.g. "
				       "kexec) symbols may be misresolved.";
			}
		}
>>>>>>> refs/remotes/origin/cm-10.0

		ui__warning(
"Kernel address maps (/proc/{kallsyms,modules}) were restricted.\n\n"
"Check /proc/sys/kernel/kptr_restrict before running 'perf record'.\n\n%s\n\n"
"Samples in kernel modules can't be resolved as well.\n\n",
<<<<<<< HEAD
			    RB_EMPTY_ROOT(&kdso->symbols[MAP__FUNCTION]) ?
"As no suitable kallsyms nor vmlinux was found, kernel samples\n"
"can't be resolved." :
"If some relocation was applied (e.g. kexec) symbols may be misresolved.");
=======
		desc);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	if (dump_trace) {
		perf_session__fprintf_nr_events(session, stdout);
		goto out_delete;
	}

	if (verbose > 3)
		perf_session__fprintf(session, stdout);

	if (verbose > 2)
		perf_session__fprintf_dsos(session, stdout);

	nr_samples = 0;
	list_for_each_entry(pos, &session->evlist->entries, node) {
		struct hists *hists = &pos->hists;

<<<<<<< HEAD
=======
		if (pos->idx == 0)
			hists->symbol_filter_str = rep->symbol_filter_str;

>>>>>>> refs/remotes/origin/cm-10.0
		hists__collapse_resort(hists);
		hists__output_resort(hists);
		nr_samples += hists->stats.nr_events[PERF_RECORD_SAMPLE];
	}

	if (nr_samples == 0) {
<<<<<<< HEAD
		ui__warning("The %s file has no samples!\n", input_name);
		goto out_delete;
	}

	if (use_browser > 0)
		perf_evlist__tui_browse_hists(session->evlist, help);
	else
		perf_evlist__tty_browse_hists(session->evlist, help);
=======
		ui__warning("The %s file has no samples!\n", session->filename);
		goto out_delete;
	}

	if (use_browser > 0) {
		if (use_browser == 1) {
			perf_evlist__tui_browse_hists(session->evlist, help,
						      NULL, NULL, 0);
		} else if (use_browser == 2) {
			perf_evlist__gtk_browse_hists(session->evlist, help,
						      NULL, NULL, 0);
		}
	} else
		perf_evlist__tty_browse_hists(session->evlist, rep, help);
>>>>>>> refs/remotes/origin/cm-10.0

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

static int
<<<<<<< HEAD
parse_callchain_opt(const struct option *opt __used, const char *arg,
		    int unset)
{
=======
parse_callchain_opt(const struct option *opt, const char *arg, int unset)
{
	struct perf_report *rep = (struct perf_report *)opt->value;
>>>>>>> refs/remotes/origin/cm-10.0
	char *tok, *tok2;
	char *endptr;

	/*
	 * --no-call-graph
	 */
	if (unset) {
<<<<<<< HEAD
		dont_use_callchains = true;
=======
		rep->dont_use_callchains = true;
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;
	}

	symbol_conf.use_callchain = true;

	if (!arg)
		return 0;

	tok = strtok((char *)arg, ",");
	if (!tok)
		return -1;

	/* get the output mode */
	if (!strncmp(tok, "graph", strlen(arg)))
		callchain_param.mode = CHAIN_GRAPH_ABS;

	else if (!strncmp(tok, "flat", strlen(arg)))
		callchain_param.mode = CHAIN_FLAT;

	else if (!strncmp(tok, "fractal", strlen(arg)))
		callchain_param.mode = CHAIN_GRAPH_REL;

	else if (!strncmp(tok, "none", strlen(arg))) {
		callchain_param.mode = CHAIN_NONE;
		symbol_conf.use_callchain = false;

		return 0;
	}

	else
		return -1;

	/* get the min percentage */
	tok = strtok(NULL, ",");
	if (!tok)
		goto setup;

<<<<<<< HEAD
	tok2 = strtok(NULL, ",");
=======
>>>>>>> refs/remotes/origin/cm-10.0
	callchain_param.min_percent = strtod(tok, &endptr);
	if (tok == endptr)
		return -1;

<<<<<<< HEAD
	if (tok2)
		callchain_param.print_limit = strtod(tok2, &endptr);
=======
	/* get the print limit */
	tok2 = strtok(NULL, ",");
	if (!tok2)
		goto setup;

	if (tok2[0] != 'c') {
		callchain_param.print_limit = strtoul(tok2, &endptr, 0);
		tok2 = strtok(NULL, ",");
		if (!tok2)
			goto setup;
	}

	/* get the call chain order */
	if (!strcmp(tok2, "caller"))
		callchain_param.order = ORDER_CALLER;
	else if (!strcmp(tok2, "callee"))
		callchain_param.order = ORDER_CALLEE;
	else
		return -1;
>>>>>>> refs/remotes/origin/cm-10.0
setup:
	if (callchain_register_param(&callchain_param) < 0) {
		fprintf(stderr, "Can't register callchain params\n");
		return -1;
	}
	return 0;
}

<<<<<<< HEAD
static const char * const report_usage[] = {
	"perf report [<options>] <command>",
	NULL
};

static const struct option options[] = {
	OPT_STRING('i', "input", &input_name, "file",
=======
static int
parse_branch_mode(const struct option *opt __used, const char *str __used, int unset)
{
	sort__branch_mode = !unset;
	return 0;
}

int cmd_report(int argc, const char **argv, const char *prefix __used)
{
	struct perf_session *session;
	struct stat st;
	bool has_br_stack = false;
	int ret = -1;
	char callchain_default_opt[] = "fractal,0.5,callee";
	const char * const report_usage[] = {
		"perf report [<options>]",
		NULL
	};
	struct perf_report report = {
		.tool = {
			.sample		 = process_sample_event,
			.mmap		 = perf_event__process_mmap,
			.comm		 = perf_event__process_comm,
			.exit		 = perf_event__process_task,
			.fork		 = perf_event__process_task,
			.lost		 = perf_event__process_lost,
			.read		 = process_read_event,
			.attr		 = perf_event__process_attr,
			.event_type	 = perf_event__process_event_type,
			.tracing_data	 = perf_event__process_tracing_data,
			.build_id	 = perf_event__process_build_id,
			.ordered_samples = true,
			.ordering_requires_timestamps = true,
		},
		.pretty_printing_style	 = "normal",
	};
	const struct option options[] = {
	OPT_STRING('i', "input", &report.input_name, "file",
>>>>>>> refs/remotes/origin/cm-10.0
		    "input file name"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_STRING('k', "vmlinux", &symbol_conf.vmlinux_name,
		   "file", "vmlinux pathname"),
	OPT_STRING(0, "kallsyms", &symbol_conf.kallsyms_name,
		   "file", "kallsyms pathname"),
<<<<<<< HEAD
	OPT_BOOLEAN('f', "force", &force, "don't complain, do it"),
=======
	OPT_BOOLEAN('f', "force", &report.force, "don't complain, do it"),
>>>>>>> refs/remotes/origin/cm-10.0
	OPT_BOOLEAN('m', "modules", &symbol_conf.use_modules,
		    "load module symbols - WARNING: use only with -k and LIVE kernel"),
	OPT_BOOLEAN('n', "show-nr-samples", &symbol_conf.show_nr_samples,
		    "Show a column with the number of samples"),
<<<<<<< HEAD
	OPT_BOOLEAN('T', "threads", &show_threads,
		    "Show per-thread event counters"),
	OPT_STRING(0, "pretty", &pretty_printing_style, "key",
		   "pretty printing style key: normal raw"),
	OPT_BOOLEAN(0, "tui", &use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "stdio", &use_stdio, "Use the stdio interface"),
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
		   "sort by key(s): pid, comm, dso, symbol, parent"),
=======
	OPT_BOOLEAN('T', "threads", &report.show_threads,
		    "Show per-thread event counters"),
	OPT_STRING(0, "pretty", &report.pretty_printing_style, "key",
		   "pretty printing style key: normal raw"),
	OPT_BOOLEAN(0, "tui", &report.use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "gtk", &report.use_gtk, "Use the GTK2 interface"),
	OPT_BOOLEAN(0, "stdio", &report.use_stdio,
		    "Use the stdio interface"),
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
		   "sort by key(s): pid, comm, dso, symbol, parent, dso_to,"
		   " dso_from, symbol_to, symbol_from, mispredict"),
>>>>>>> refs/remotes/origin/cm-10.0
	OPT_BOOLEAN(0, "showcpuutilization", &symbol_conf.show_cpu_utilization,
		    "Show sample percentage for different cpu modes"),
	OPT_STRING('p', "parent", &parent_pattern, "regex",
		   "regex filter to identify parent, see: '--sort parent'"),
	OPT_BOOLEAN('x', "exclude-other", &symbol_conf.exclude_other,
		    "Only display entries with parent-match"),
<<<<<<< HEAD
	OPT_CALLBACK_DEFAULT('g', "call-graph", NULL, "output_type,min_percent",
		     "Display callchains using output_type (graph, flat, fractal, or none) and min percent threshold. "
		     "Default: fractal,0.5", &parse_callchain_opt, callchain_default_opt),
	OPT_STRING('d', "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING('C', "comms", &symbol_conf.comm_list_str, "comm[,comm...]",
		   "only consider symbols in these comms"),
	OPT_STRING('S', "symbols", &symbol_conf.sym_list_str, "symbol[,symbol...]",
		   "only consider these symbols"),
=======
	OPT_CALLBACK_DEFAULT('g', "call-graph", &report, "output_type,min_percent[,print_limit],call_order",
		     "Display callchains using output_type (graph, flat, fractal, or none) , min percent threshold, optional print limit and callchain order. "
		     "Default: fractal,0.5,callee", &parse_callchain_opt, callchain_default_opt),
	OPT_BOOLEAN('G', "inverted", &report.inverted_callchain,
		    "alias for inverted call graph"),
	OPT_STRING('d', "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING('c', "comms", &symbol_conf.comm_list_str, "comm[,comm...]",
		   "only consider symbols in these comms"),
	OPT_STRING('S', "symbols", &symbol_conf.sym_list_str, "symbol[,symbol...]",
		   "only consider these symbols"),
	OPT_STRING(0, "symbol-filter", &report.symbol_filter_str, "filter",
		   "only show symbols that (partially) match with this filter"),
>>>>>>> refs/remotes/origin/cm-10.0
	OPT_STRING('w', "column-widths", &symbol_conf.col_width_list_str,
		   "width[,width...]",
		   "don't try to adjust column width, use these fixed values"),
	OPT_STRING('t', "field-separator", &symbol_conf.field_sep, "separator",
		   "separator for columns, no spaces will be added between "
		   "columns '.' is reserved."),
<<<<<<< HEAD
	OPT_BOOLEAN('U', "hide-unresolved", &hide_unresolved,
		    "Only display entries resolved to a symbol"),
	OPT_STRING(0, "symfs", &symbol_conf.symfs, "directory",
		    "Look for files with symbols relative to this directory"),
	OPT_END()
};

int cmd_report(int argc, const char **argv, const char *prefix __used)
{
	argc = parse_options(argc, argv, options, report_usage, 0);

	if (use_stdio)
		use_browser = 0;
	else if (use_tui)
		use_browser = 1;

	if (strcmp(input_name, "-") != 0)
		setup_browser(true);
	else
		use_browser = 0;
=======
	OPT_BOOLEAN('U', "hide-unresolved", &report.hide_unresolved,
		    "Only display entries resolved to a symbol"),
	OPT_STRING(0, "symfs", &symbol_conf.symfs, "directory",
		    "Look for files with symbols relative to this directory"),
	OPT_STRING('C', "cpu", &report.cpu_list, "cpu",
		   "list of cpus to profile"),
	OPT_BOOLEAN('I', "show-info", &report.show_full_info,
		    "Display extended information about perf.data file"),
	OPT_BOOLEAN(0, "source", &symbol_conf.annotate_src,
		    "Interleave source code with assembly code (default)"),
	OPT_BOOLEAN(0, "asm-raw", &symbol_conf.annotate_asm_raw,
		    "Display raw encoding of assembly instructions (default)"),
	OPT_STRING('M', "disassembler-style", &disassembler_style, "disassembler style",
		   "Specify disassembler style (e.g. -M intel for intel syntax)"),
	OPT_BOOLEAN(0, "show-total-period", &symbol_conf.show_total_period,
		    "Show a column with the sum of periods"),
	OPT_CALLBACK_NOOPT('b', "branch-stack", &sort__branch_mode, "",
		    "use branch records for histogram filling", parse_branch_mode),
	OPT_END()
	};

	argc = parse_options(argc, argv, options, report_usage, 0);

	if (report.use_stdio)
		use_browser = 0;
	else if (report.use_tui)
		use_browser = 1;
	else if (report.use_gtk)
		use_browser = 2;

	if (report.inverted_callchain)
		callchain_param.order = ORDER_CALLER;

	if (!report.input_name || !strlen(report.input_name)) {
		if (!fstat(STDIN_FILENO, &st) && S_ISFIFO(st.st_mode))
			report.input_name = "-";
		else
			report.input_name = "perf.data";
	}
	session = perf_session__new(report.input_name, O_RDONLY,
				    report.force, false, &report.tool);
	if (session == NULL)
		return -ENOMEM;

	report.session = session;

	has_br_stack = perf_header__has_feat(&session->header,
					     HEADER_BRANCH_STACK);

	if (sort__branch_mode == -1 && has_br_stack)
		sort__branch_mode = 1;

	/* sort__branch_mode could be 0 if --no-branch-stack */
	if (sort__branch_mode == 1) {
		/*
		 * if no sort_order is provided, then specify
		 * branch-mode specific order
		 */
		if (sort_order == default_sort_order)
			sort_order = "comm,dso_from,symbol_from,"
				     "dso_to,symbol_to";

	}

	if (strcmp(report.input_name, "-") != 0) {
		if (report.use_gtk)
			perf_gtk_setup_browser(argc, argv, true);
		else
			setup_browser(true);
	} else {
		use_browser = 0;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Only in the newt browser we are doing integrated annotation,
	 * so don't allocate extra space that won't be used in the stdio
	 * implementation.
	 */
	if (use_browser > 0) {
		symbol_conf.priv_size = sizeof(struct annotation);
<<<<<<< HEAD
		annotate_init	      = symbol__annotate_init;
=======
		report.annotate_init  = symbol__annotate_init;
>>>>>>> refs/remotes/origin/cm-10.0
		/*
 		 * For searching by name on the "Browse map details".
 		 * providing it only in verbose mode not to bloat too
 		 * much struct symbol.
 		 */
		if (verbose) {
			/*
			 * XXX: Need to provide a less kludgy way to ask for
			 * more space per symbol, the u32 is for the index on
			 * the ui browser.
			 * See symbol__browser_index.
			 */
			symbol_conf.priv_size += sizeof(u32);
			symbol_conf.sort_by_name = true;
		}
	}

	if (symbol__init() < 0)
<<<<<<< HEAD
		return -1;
=======
		goto error;
>>>>>>> refs/remotes/origin/cm-10.0

	setup_sorting(report_usage, options);

	if (parent_pattern != default_parent_pattern) {
		if (sort_dimension__add("parent") < 0)
<<<<<<< HEAD
			return -1;
		sort_parent.elide = 1;
	} else
		symbol_conf.exclude_other = false;

	/*
	 * Any (unrecognized) arguments left?
	 */
	if (argc)
		usage_with_options(report_usage, options);

	sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "dso", stdout);
	sort_entry__setup_elide(&sort_comm, symbol_conf.comm_list, "comm", stdout);
	sort_entry__setup_elide(&sort_sym, symbol_conf.sym_list, "symbol", stdout);

	return __cmd_report();
=======
			goto error;

		/*
		 * Only show the parent fields if we explicitly
		 * sort that way. If we only use parent machinery
		 * for filtering, we don't want it.
		 */
		if (!strstr(sort_order, "parent"))
			sort_parent.elide = 1;
	} else
		symbol_conf.exclude_other = false;

	if (argc) {
		/*
		 * Special case: if there's an argument left then assume that
		 * it's a symbol filter:
		 */
		if (argc > 1)
			usage_with_options(report_usage, options);

		report.symbol_filter_str = argv[0];
	}

	sort_entry__setup_elide(&sort_comm, symbol_conf.comm_list, "comm", stdout);

	if (sort__branch_mode == 1) {
		sort_entry__setup_elide(&sort_dso_from, symbol_conf.dso_from_list, "dso_from", stdout);
		sort_entry__setup_elide(&sort_dso_to, symbol_conf.dso_to_list, "dso_to", stdout);
		sort_entry__setup_elide(&sort_sym_from, symbol_conf.sym_from_list, "sym_from", stdout);
		sort_entry__setup_elide(&sort_sym_to, symbol_conf.sym_to_list, "sym_to", stdout);
	} else {
		sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "dso", stdout);
		sort_entry__setup_elide(&sort_sym, symbol_conf.sym_list, "symbol", stdout);
	}

	ret = __cmd_report(&report);
error:
	perf_session__delete(session);
	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}
