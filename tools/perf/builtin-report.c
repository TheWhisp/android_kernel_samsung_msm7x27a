/*
 * builtin-report.c
 *
 * Builtin report command: Analyze the perf.data input file,
 * look up and read DSOs and symbol information and display
 * a histogram of results, along various sorting keys.
 */
#include "builtin.h"

#include "util/util.h"
<<<<<<< HEAD
=======
#include "util/cache.h"
>>>>>>> refs/remotes/origin/master

#include "util/annotate.h"
#include "util/color.h"
#include <linux/list.h>
<<<<<<< HEAD
#include "util/cache.h"
=======
>>>>>>> refs/remotes/origin/master
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
<<<<<<< HEAD
=======
#include "util/tool.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "util/tool.h"
>>>>>>> refs/remotes/origin/master

#include "util/parse-options.h"
#include "util/parse-events.h"

#include "util/thread.h"
#include "util/sort.h"
#include "util/hist.h"
<<<<<<< HEAD

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
=======
#include "util/data.h"
#include "arch/common.h"

#include <dlfcn.h>
#include <linux/bitmap.h>

struct report {
	struct perf_tool	tool;
	struct perf_session	*session;
>>>>>>> refs/remotes/origin/master
	bool			force, use_tui, use_gtk, use_stdio;
	bool			hide_unresolved;
	bool			dont_use_callchains;
	bool			show_full_info;
	bool			show_threads;
	bool			inverted_callchain;
<<<<<<< HEAD
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
=======
	bool			mem_mode;
	bool			header;
	bool			header_only;
	int			max_stack;
	struct perf_read_values	show_threads_values;
	const char		*pretty_printing_style;
	const char		*cpu_list;
	const char		*symbol_filter_str;
	float			min_percent;
	DECLARE_BITMAP(cpu_bitmap, MAX_NR_CPUS);
};

static int report__config(const char *var, const char *value, void *cb)
{
	if (!strcmp(var, "report.group")) {
		symbol_conf.event_group = perf_config_bool(var, value);
		return 0;
	}
	if (!strcmp(var, "report.percent-limit")) {
		struct report *rep = cb;
		rep->min_percent = strtof(value, NULL);
		return 0;
	}

	return perf_default_config(var, value, cb);
}

static int report__add_mem_hist_entry(struct perf_tool *tool, struct addr_location *al,
				      struct perf_sample *sample, struct perf_evsel *evsel,
				      union perf_event *event)
{
	struct report *rep = container_of(tool, struct report, tool);
	struct symbol *parent = NULL;
	u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;
	struct hist_entry *he;
	struct mem_info *mi, *mx;
	uint64_t cost;
	int err = sample__resolve_callchain(sample, &parent, evsel, al, rep->max_stack);

	if (err)
		return err;

	mi = machine__resolve_mem(al->machine, al->thread, sample, cpumode);
	if (!mi)
		return -ENOMEM;

	if (rep->hide_unresolved && !al->sym)
		return 0;

	cost = sample->weight;
	if (!cost)
		cost = 1;

	/*
	 * must pass period=weight in order to get the correct
	 * sorting from hists__collapse_resort() which is solely
	 * based on periods. We want sorting be done on nr_events * weight
	 * and this is indirectly achieved by passing period=weight here
	 * and the he_stat__add_period() function.
	 */
	he = __hists__add_entry(&evsel->hists, al, parent, NULL, mi,
				cost, cost, 0);
	if (!he)
		return -ENOMEM;

	err = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
	if (err)
		goto out;

	mx = he->mem_info;
	err = addr_map_symbol__inc_samples(&mx->daddr, evsel->idx);
	if (err)
		goto out;

	evsel->hists.stats.total_period += cost;
	hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
	err = hist_entry__append_callchain(he, sample);
out:
	return err;
}

static int report__add_branch_hist_entry(struct perf_tool *tool, struct addr_location *al,
					 struct perf_sample *sample, struct perf_evsel *evsel)
{
	struct report *rep = container_of(tool, struct report, tool);
	struct symbol *parent = NULL;
	unsigned i;
	struct hist_entry *he;
	struct branch_info *bi, *bx;
	int err = sample__resolve_callchain(sample, &parent, evsel, al, rep->max_stack);

	if (err)
		return err;

	bi = machine__resolve_bstack(al->machine, al->thread,
>>>>>>> refs/remotes/origin/master
				     sample->branch_stack);
	if (!bi)
		return -ENOMEM;

	for (i = 0; i < sample->branch_stack->nr; i++) {
		if (rep->hide_unresolved && !(bi[i].from.sym && bi[i].to.sym))
			continue;
<<<<<<< HEAD
=======

		err = -ENOMEM;

		/* overwrite the 'al' to branch-to info */
		al->map = bi[i].to.map;
		al->sym = bi[i].to.sym;
		al->addr = bi[i].to.addr;
>>>>>>> refs/remotes/origin/master
		/*
		 * The report shows the percentage of total branches captured
		 * and not events sampled. Thus we use a pseudo period of 1.
		 */
<<<<<<< HEAD
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

=======
		he = __hists__add_entry(&evsel->hists, al, parent, &bi[i], NULL,
					1, 1, 0);
		if (he) {
			bx = he->branch_info;
			err = addr_map_symbol__inc_samples(&bx->from, evsel->idx);
			if (err)
				goto out;

			err = addr_map_symbol__inc_samples(&bx->to, evsel->idx);
			if (err)
				goto out;

			evsel->hists.stats.total_period += 1;
			hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
		} else
			goto out;
	}
	err = 0;
out:
	free(bi);
	return err;
}

static int report__add_hist_entry(struct perf_tool *tool, struct perf_evsel *evsel,
				  struct addr_location *al, struct perf_sample *sample)
{
	struct report *rep = container_of(tool, struct report, tool);
	struct symbol *parent = NULL;
	struct hist_entry *he;
	int err = sample__resolve_callchain(sample, &parent, evsel, al, rep->max_stack);

	if (err)
		return err;

	he = __hists__add_entry(&evsel->hists, al, parent, NULL, NULL,
				sample->period, sample->weight,
				sample->transaction);
	if (he == NULL)
		return -ENOMEM;

	err = hist_entry__append_callchain(he, sample);
	if (err)
		goto out;

	err = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
>>>>>>> refs/remotes/origin/master
	evsel->hists.stats.total_period += sample->period;
	hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
out:
	return err;
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
					  annotate_init) < 0) {
=======
=======
>>>>>>> refs/remotes/origin/master
static int process_sample_event(struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct machine *machine)
{
<<<<<<< HEAD
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
=======
	struct report *rep = container_of(tool, struct report, tool);
	struct addr_location al;
	int ret;

	if (perf_event__preprocess_sample(event, machine, &al, sample) < 0) {
		pr_debug("problem processing %d event, skipping it.\n",
			 event->header.type);
		return -1;
	}

>>>>>>> refs/remotes/origin/master
	if (al.filtered || (rep->hide_unresolved && al.sym == NULL))
		return 0;

	if (rep->cpu_list && !test_bit(sample->cpu, rep->cpu_bitmap))
		return 0;

<<<<<<< HEAD
	if (sort__branch_mode == 1) {
		if (perf_report__add_branch_hist_entry(tool, &al, sample,
						       evsel, machine)) {
			pr_debug("problem adding lbr entry, skipping event\n");
			return -1;
		}
=======
	if (sort__mode == SORT_MODE__BRANCH) {
		ret = report__add_branch_hist_entry(tool, &al, sample, evsel);
		if (ret < 0)
			pr_debug("problem adding lbr entry, skipping event\n");
	} else if (rep->mem_mode == 1) {
		ret = report__add_mem_hist_entry(tool, &al, sample, evsel, event);
		if (ret < 0)
			pr_debug("problem adding mem entry, skipping event\n");
>>>>>>> refs/remotes/origin/master
	} else {
		if (al.map != NULL)
			al.map->dso->hit = 1;

<<<<<<< HEAD
		if (perf_evsel__add_hist_entry(evsel, &al, sample, machine)) {
			pr_debug("problem incrementing symbol period, skipping event\n");
			return -1;
		}
	}
	return 0;
=======
		ret = report__add_hist_entry(tool, evsel, &al, sample);
		if (ret < 0)
			pr_debug("problem incrementing symbol period, skipping event\n");
	}
	return ret;
>>>>>>> refs/remotes/origin/master
}

static int process_read_event(struct perf_tool *tool,
			      union perf_event *event,
<<<<<<< HEAD
			      struct perf_sample *sample __used,
			      struct perf_evsel *evsel,
			      struct machine *machine __used)
{
	struct perf_report *rep = container_of(tool, struct perf_report, tool);

	if (rep->show_threads) {
		const char *name = evsel ? event_name(evsel) : "unknown";
		perf_read_values_add_value(&rep->show_threads_values,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			      struct perf_sample *sample __maybe_unused,
			      struct perf_evsel *evsel,
			      struct machine *machine __maybe_unused)
{
	struct report *rep = container_of(tool, struct report, tool);

	if (rep->show_threads) {
		const char *name = evsel ? perf_evsel__name(evsel) : "unknown";
		perf_read_values_add_value(&rep->show_threads_values,
>>>>>>> refs/remotes/origin/master
					   event->read.pid, event->read.tid,
					   event->read.id,
					   name,
					   event->read.value);
	}

	dump_printf(": %d %d %s %" PRIu64 "\n", event->read.pid, event->read.tid,
<<<<<<< HEAD
		    evsel ? event_name(evsel) : "FAIL",
=======
		    evsel ? perf_evsel__name(evsel) : "FAIL",
>>>>>>> refs/remotes/origin/master
		    event->read.value);

	return 0;
}

<<<<<<< HEAD
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
=======
/* For pipe mode, sample_type is not currently set */
static int report__setup_sample_type(struct report *rep)
{
	struct perf_session *session = rep->session;
	u64 sample_type = perf_evlist__combined_sample_type(session->evlist);
	bool is_pipe = perf_data_file__is_pipe(session->file);

	if (!is_pipe && !(sample_type & PERF_SAMPLE_CALLCHAIN)) {
		if (sort__has_parent) {
			ui__error("Selected --sort parent, but no "
>>>>>>> refs/remotes/origin/master
				    "callchain data. Did you call "
				    "'perf record' without -g?\n");
			return -EINVAL;
		}
		if (symbol_conf.use_callchain) {
<<<<<<< HEAD
			ui__warning("Selected -g but no callchain data. Did "
=======
			ui__error("Selected -g but no callchain data. Did "
>>>>>>> refs/remotes/origin/master
				    "you call 'perf record' without -g?\n");
			return -1;
		}
	} else if (!rep->dont_use_callchains &&
		   callchain_param.mode != CHAIN_NONE &&
		   !symbol_conf.use_callchain) {
			symbol_conf.use_callchain = true;
			if (callchain_register_param(&callchain_param) < 0) {
<<<<<<< HEAD
				ui__warning("Can't register callchain "
					    "params.\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
				ui__error("Can't register callchain params.\n");
>>>>>>> refs/remotes/origin/master
				return -EINVAL;
			}
	}

<<<<<<< HEAD
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
=======
	if (sort__mode == SORT_MODE__BRANCH) {
		if (!is_pipe &&
		    !(sample_type & PERF_SAMPLE_BRANCH_STACK)) {
			ui__error("Selected -b but no branch data. "
				  "Did you call perf record without -b?\n");
>>>>>>> refs/remotes/origin/master
			return -1;
		}
	}

	return 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
extern volatile int session_done;

static void sig_handler(int sig __used)
=======
static void sig_handler(int sig __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	session_done = 1;
}

<<<<<<< HEAD
static size_t hists__fprintf_nr_sample_events(struct hists *self,
=======
static size_t hists__fprintf_nr_sample_events(struct hists *hists, struct report *rep,
>>>>>>> refs/remotes/origin/master
					      const char *evname, FILE *fp)
{
	size_t ret;
	char unit;
<<<<<<< HEAD
	unsigned long nr_events = self->stats.nr_events[PERF_RECORD_SAMPLE];

	nr_events = convert_unit(nr_events, &unit);
	ret = fprintf(fp, "# Events: %lu%c", nr_events, unit);
	if (evname != NULL)
		ret += fprintf(fp, " %s", evname);
=======
	unsigned long nr_samples = hists->stats.nr_events[PERF_RECORD_SAMPLE];
	u64 nr_events = hists->stats.total_period;
	struct perf_evsel *evsel = hists_to_evsel(hists);
	char buf[512];
	size_t size = sizeof(buf);

	if (perf_evsel__is_group_event(evsel)) {
		struct perf_evsel *pos;

		perf_evsel__group_desc(evsel, buf, size);
		evname = buf;

		for_each_group_member(pos, evsel) {
			nr_samples += pos->hists.stats.nr_events[PERF_RECORD_SAMPLE];
			nr_events += pos->hists.stats.total_period;
		}
	}

	nr_samples = convert_unit(nr_samples, &unit);
	ret = fprintf(fp, "# Samples: %lu%c", nr_samples, unit);
	if (evname != NULL)
		ret += fprintf(fp, " of event '%s'", evname);

	if (rep->mem_mode) {
		ret += fprintf(fp, "\n# Total weight : %" PRIu64, nr_events);
		ret += fprintf(fp, "\n# Sort order   : %s", sort_order);
	} else
		ret += fprintf(fp, "\n# Event count (approx.): %" PRIu64, nr_events);
>>>>>>> refs/remotes/origin/master
	return ret + fprintf(fp, "\n#\n");
}

static int perf_evlist__tty_browse_hists(struct perf_evlist *evlist,
<<<<<<< HEAD
<<<<<<< HEAD
=======
					 struct perf_report *rep,
>>>>>>> refs/remotes/origin/cm-10.0
=======
					 struct report *rep,
>>>>>>> refs/remotes/origin/master
					 const char *help)
{
	struct perf_evsel *pos;

<<<<<<< HEAD
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
=======
	evlist__for_each(evlist, pos) {
		struct hists *hists = &pos->hists;
		const char *evname = perf_evsel__name(pos);

		if (symbol_conf.event_group &&
		    !perf_evsel__is_group_leader(pos))
			continue;

		hists__fprintf_nr_sample_events(hists, rep, evname, stdout);
		hists__fprintf(hists, true, 0, 0, rep->min_percent, stdout);
>>>>>>> refs/remotes/origin/master
		fprintf(stdout, "\n\n");
	}

	if (sort_order == default_sort_order &&
	    parent_pattern == default_parent_pattern) {
		fprintf(stdout, "#\n# (%s)\n#\n", help);

<<<<<<< HEAD
<<<<<<< HEAD
		if (show_threads) {
			bool style = !strcmp(pretty_printing_style, "raw");
			perf_read_values_display(stdout, &show_threads_values,
						 style);
			perf_read_values_destroy(&show_threads_values);
=======
=======
>>>>>>> refs/remotes/origin/master
		if (rep->show_threads) {
			bool style = !strcmp(rep->pretty_printing_style, "raw");
			perf_read_values_display(stdout, &rep->show_threads_values,
						 style);
			perf_read_values_destroy(&rep->show_threads_values);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}

	return 0;
}

<<<<<<< HEAD
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
=======
static void report__warn_kptr_restrict(const struct report *rep)
{
	struct map *kernel_map = rep->session->machines.host.vmlinux_maps[MAP__FUNCTION];
	struct kmap *kernel_kmap = map__kmap(kernel_map);

>>>>>>> refs/remotes/origin/master
	if (kernel_map == NULL ||
	    (kernel_map->dso->hit &&
	     (kernel_kmap->ref_reloc_sym == NULL ||
	      kernel_kmap->ref_reloc_sym->addr == 0))) {
<<<<<<< HEAD
<<<<<<< HEAD
		const struct dso *kdso = kernel_map->dso;
=======
=======
>>>>>>> refs/remotes/origin/master
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
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		ui__warning(
"Kernel address maps (/proc/{kallsyms,modules}) were restricted.\n\n"
"Check /proc/sys/kernel/kptr_restrict before running 'perf record'.\n\n%s\n\n"
"Samples in kernel modules can't be resolved as well.\n\n",
<<<<<<< HEAD
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
=======
		desc);
	}
}

static int report__gtk_browse_hists(struct report *rep, const char *help)
{
	int (*hist_browser)(struct perf_evlist *evlist, const char *help,
			    struct hist_browser_timer *timer, float min_pcnt);

	hist_browser = dlsym(perf_gtk_handle, "perf_evlist__gtk_browse_hists");

	if (hist_browser == NULL) {
		ui__error("GTK browser not found!\n");
		return -1;
	}

	return hist_browser(rep->session->evlist, help, NULL, rep->min_percent);
}

static int report__browse_hists(struct report *rep)
{
	int ret;
	struct perf_session *session = rep->session;
	struct perf_evlist *evlist = session->evlist;
	const char *help = "For a higher level overview, try: perf report --sort comm,dso";

	switch (use_browser) {
	case 1:
		ret = perf_evlist__tui_browse_hists(evlist, help, NULL,
						    rep->min_percent,
						    &session->header.env);
		/*
		 * Usually "ret" is the last pressed key, and we only
		 * care if the key notifies us to switch data file.
		 */
		if (ret != K_SWITCH_INPUT_DATA)
			ret = 0;
		break;
	case 2:
		ret = report__gtk_browse_hists(rep, help);
		break;
	default:
		ret = perf_evlist__tty_browse_hists(evlist, rep, help);
		break;
	}

	return ret;
}

static u64 report__collapse_hists(struct report *rep)
{
	struct ui_progress prog;
	struct perf_evsel *pos;
	u64 nr_samples = 0;
	/*
 	 * Count number of histogram entries to use when showing progress,
 	 * reusing nr_samples variable.
 	 */
	evlist__for_each(rep->session->evlist, pos)
		nr_samples += pos->hists.nr_entries;

	ui_progress__init(&prog, nr_samples, "Merging related events...");
	/*
	 * Count total number of samples, will be used to check if this
 	 * session had any.
 	 */
	nr_samples = 0;

	evlist__for_each(rep->session->evlist, pos) {
		struct hists *hists = &pos->hists;

		if (pos->idx == 0)
			hists->symbol_filter_str = rep->symbol_filter_str;

		hists__collapse_resort(hists, &prog);
		nr_samples += hists->stats.nr_events[PERF_RECORD_SAMPLE];

		/* Non-group events are considered as leader */
		if (symbol_conf.event_group &&
		    !perf_evsel__is_group_leader(pos)) {
			struct hists *leader_hists = &pos->leader->hists;

			hists__match(leader_hists, hists);
			hists__link(leader_hists, hists);
		}
	}

	ui_progress__finish();

	return nr_samples;
}

static int __cmd_report(struct report *rep)
{
	int ret;
	u64 nr_samples;
	struct perf_session *session = rep->session;
	struct perf_evsel *pos;
	struct perf_data_file *file = session->file;

	signal(SIGINT, sig_handler);

	if (rep->cpu_list) {
		ret = perf_session__cpu_bitmap(session, rep->cpu_list,
					       rep->cpu_bitmap);
		if (ret)
			return ret;
	}

	if (rep->show_threads)
		perf_read_values_init(&rep->show_threads_values);

	ret = report__setup_sample_type(rep);
	if (ret)
		return ret;

	ret = perf_session__process_events(session, &rep->tool);
	if (ret)
		return ret;

	report__warn_kptr_restrict(rep);

	if (use_browser == 0) {
		if (verbose > 3)
			perf_session__fprintf(session, stdout);

		if (verbose > 2)
			perf_session__fprintf_dsos(session, stdout);

		if (dump_trace) {
			perf_session__fprintf_nr_events(session, stdout);
			return 0;
		}
	}

	nr_samples = report__collapse_hists(rep);

	if (session_done())
		return 0;

	if (nr_samples == 0) {
		ui__error("The %s file has no samples!\n", file->path);
		return 0;
	}

	evlist__for_each(session->evlist, pos)
		hists__output_resort(&pos->hists);

	return report__browse_hists(rep);
}

static int
parse_callchain_opt(const struct option *opt, const char *arg, int unset)
{
	struct report *rep = (struct report *)opt->value;
>>>>>>> refs/remotes/origin/master
	char *tok, *tok2;
	char *endptr;

	/*
	 * --no-call-graph
	 */
	if (unset) {
<<<<<<< HEAD
<<<<<<< HEAD
		dont_use_callchains = true;
=======
		rep->dont_use_callchains = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		rep->dont_use_callchains = true;
>>>>>>> refs/remotes/origin/master
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
<<<<<<< HEAD
	tok2 = strtok(NULL, ",");
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	callchain_param.min_percent = strtod(tok, &endptr);
	if (tok == endptr)
		return -1;

<<<<<<< HEAD
<<<<<<< HEAD
	if (tok2)
		callchain_param.print_limit = strtod(tok2, &endptr);
=======
=======
>>>>>>> refs/remotes/origin/master
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
<<<<<<< HEAD
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
=======
	if (!strncmp(tok2, "caller", strlen("caller")))
		callchain_param.order = ORDER_CALLER;
	else if (!strncmp(tok2, "callee", strlen("callee")))
		callchain_param.order = ORDER_CALLEE;
	else
		return -1;

	/* Get the sort key */
	tok2 = strtok(NULL, ",");
	if (!tok2)
		goto setup;
	if (!strncmp(tok2, "function", strlen("function")))
		callchain_param.key = CCKEY_FUNCTION;
	else if (!strncmp(tok2, "address", strlen("address")))
		callchain_param.key = CCKEY_ADDRESS;
	else
		return -1;
setup:
	if (callchain_register_param(&callchain_param) < 0) {
		pr_err("Can't register callchain params\n");
>>>>>>> refs/remotes/origin/master
		return -1;
	}
	return 0;
}

<<<<<<< HEAD
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
=======
int
report_parse_ignore_callees_opt(const struct option *opt __maybe_unused,
				const char *arg, int unset __maybe_unused)
{
	if (arg) {
		int err = regcomp(&ignore_callees_regex, arg, REG_EXTENDED);
		if (err) {
			char buf[BUFSIZ];
			regerror(err, &ignore_callees_regex, buf, sizeof(buf));
			pr_err("Invalid --ignore-callees regex: %s\n%s", arg, buf);
			return -1;
		}
		have_ignore_callees = 1;
	}

	return 0;
}

static int
parse_branch_mode(const struct option *opt __maybe_unused,
		  const char *str __maybe_unused, int unset)
{
	int *branch_mode = opt->value;

	*branch_mode = !unset;
	return 0;
}

static int
parse_percent_limit(const struct option *opt, const char *str,
		    int unset __maybe_unused)
{
	struct report *rep = opt->value;

	rep->min_percent = strtof(str, NULL);
	return 0;
}

int cmd_report(int argc, const char **argv, const char *prefix __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	struct perf_session *session;
	struct stat st;
	bool has_br_stack = false;
<<<<<<< HEAD
=======
	int branch_mode = -1;
>>>>>>> refs/remotes/origin/master
	int ret = -1;
	char callchain_default_opt[] = "fractal,0.5,callee";
	const char * const report_usage[] = {
		"perf report [<options>]",
		NULL
	};
<<<<<<< HEAD
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
=======
	struct report report = {
		.tool = {
			.sample		 = process_sample_event,
			.mmap		 = perf_event__process_mmap,
			.mmap2		 = perf_event__process_mmap2,
			.comm		 = perf_event__process_comm,
			.exit		 = perf_event__process_exit,
			.fork		 = perf_event__process_fork,
			.lost		 = perf_event__process_lost,
			.read		 = process_read_event,
			.attr		 = perf_event__process_attr,
>>>>>>> refs/remotes/origin/master
			.tracing_data	 = perf_event__process_tracing_data,
			.build_id	 = perf_event__process_build_id,
			.ordered_samples = true,
			.ordering_requires_timestamps = true,
		},
<<<<<<< HEAD
		.pretty_printing_style	 = "normal",
	};
	const struct option options[] = {
	OPT_STRING('i', "input", &report.input_name, "file",
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.max_stack		 = PERF_MAX_STACK_DEPTH,
		.pretty_printing_style	 = "normal",
	};
	const struct option options[] = {
	OPT_STRING('i', "input", &input_name, "file",
>>>>>>> refs/remotes/origin/master
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
<<<<<<< HEAD
	OPT_BOOLEAN('f', "force", &force, "don't complain, do it"),
=======
	OPT_BOOLEAN('f', "force", &report.force, "don't complain, do it"),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	OPT_BOOLEAN('f', "force", &report.force, "don't complain, do it"),
>>>>>>> refs/remotes/origin/master
	OPT_BOOLEAN('m', "modules", &symbol_conf.use_modules,
		    "load module symbols - WARNING: use only with -k and LIVE kernel"),
	OPT_BOOLEAN('n', "show-nr-samples", &symbol_conf.show_nr_samples,
		    "Show a column with the number of samples"),
<<<<<<< HEAD
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
=======
>>>>>>> refs/remotes/origin/master
	OPT_BOOLEAN('T', "threads", &report.show_threads,
		    "Show per-thread event counters"),
	OPT_STRING(0, "pretty", &report.pretty_printing_style, "key",
		   "pretty printing style key: normal raw"),
	OPT_BOOLEAN(0, "tui", &report.use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "gtk", &report.use_gtk, "Use the GTK2 interface"),
	OPT_BOOLEAN(0, "stdio", &report.use_stdio,
		    "Use the stdio interface"),
<<<<<<< HEAD
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
		   "sort by key(s): pid, comm, dso, symbol, parent, dso_to,"
		   " dso_from, symbol_to, symbol_from, mispredict"),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	OPT_BOOLEAN(0, "header", &report.header, "Show data header."),
	OPT_BOOLEAN(0, "header-only", &report.header_only,
		    "Show only data header."),
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
		   "sort by key(s): pid, comm, dso, symbol, parent, cpu, srcline,"
		   " dso_to, dso_from, symbol_to, symbol_from, mispredict,"
		   " weight, local_weight, mem, symbol_daddr, dso_daddr, tlb, "
		   "snoop, locked, abort, in_tx, transaction"),
>>>>>>> refs/remotes/origin/master
	OPT_BOOLEAN(0, "showcpuutilization", &symbol_conf.show_cpu_utilization,
		    "Show sample percentage for different cpu modes"),
	OPT_STRING('p', "parent", &parent_pattern, "regex",
		   "regex filter to identify parent, see: '--sort parent'"),
	OPT_BOOLEAN('x', "exclude-other", &symbol_conf.exclude_other,
		    "Only display entries with parent-match"),
<<<<<<< HEAD
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
=======
	OPT_CALLBACK_DEFAULT('g', "call-graph", &report, "output_type,min_percent[,print_limit],call_order",
		     "Display callchains using output_type (graph, flat, fractal, or none) , min percent threshold, optional print limit, callchain order, key (function or address). "
		     "Default: fractal,0.5,callee,function", &parse_callchain_opt, callchain_default_opt),
	OPT_INTEGER(0, "max-stack", &report.max_stack,
		    "Set the maximum stack depth when parsing the callchain, "
		    "anything beyond the specified depth will be ignored. "
		    "Default: " __stringify(PERF_MAX_STACK_DEPTH)),
	OPT_BOOLEAN('G', "inverted", &report.inverted_callchain,
		    "alias for inverted call graph"),
	OPT_CALLBACK(0, "ignore-callees", NULL, "regex",
		   "ignore callees of these functions in call graphs",
		   report_parse_ignore_callees_opt),
>>>>>>> refs/remotes/origin/master
	OPT_STRING('d', "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING('c', "comms", &symbol_conf.comm_list_str, "comm[,comm...]",
		   "only consider symbols in these comms"),
	OPT_STRING('S', "symbols", &symbol_conf.sym_list_str, "symbol[,symbol...]",
		   "only consider these symbols"),
	OPT_STRING(0, "symbol-filter", &report.symbol_filter_str, "filter",
		   "only show symbols that (partially) match with this filter"),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	OPT_STRING('w', "column-widths", &symbol_conf.col_width_list_str,
		   "width[,width...]",
		   "don't try to adjust column width, use these fixed values"),
	OPT_STRING('t', "field-separator", &symbol_conf.field_sep, "separator",
		   "separator for columns, no spaces will be added between "
		   "columns '.' is reserved."),
<<<<<<< HEAD
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
=======
>>>>>>> refs/remotes/origin/master
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
<<<<<<< HEAD
	OPT_CALLBACK_NOOPT('b', "branch-stack", &sort__branch_mode, "",
		    "use branch records for histogram filling", parse_branch_mode),
	OPT_END()
	};
=======
	OPT_BOOLEAN(0, "group", &symbol_conf.event_group,
		    "Show event group information together"),
	OPT_CALLBACK_NOOPT('b', "branch-stack", &branch_mode, "",
		    "use branch records for histogram filling", parse_branch_mode),
	OPT_STRING(0, "objdump", &objdump_path, "path",
		   "objdump binary to use for disassembly and annotations"),
	OPT_BOOLEAN(0, "demangle", &symbol_conf.demangle,
		    "Disable symbol demangling"),
	OPT_BOOLEAN(0, "mem-mode", &report.mem_mode, "mem access profile"),
	OPT_CALLBACK(0, "percent-limit", &report, "percent",
		     "Don't show entries under that percent", parse_percent_limit),
	OPT_END()
	};
	struct perf_data_file file = {
		.mode  = PERF_DATA_MODE_READ,
	};

	perf_config(report__config, &report);
>>>>>>> refs/remotes/origin/master

	argc = parse_options(argc, argv, options, report_usage, 0);

	if (report.use_stdio)
		use_browser = 0;
	else if (report.use_tui)
		use_browser = 1;
	else if (report.use_gtk)
		use_browser = 2;

	if (report.inverted_callchain)
		callchain_param.order = ORDER_CALLER;

<<<<<<< HEAD
	if (!report.input_name || !strlen(report.input_name)) {
		if (!fstat(STDIN_FILENO, &st) && S_ISFIFO(st.st_mode))
			report.input_name = "-";
		else
			report.input_name = "perf.data";
	}
	session = perf_session__new(report.input_name, O_RDONLY,
				    report.force, false, &report.tool);
=======
	if (!input_name || !strlen(input_name)) {
		if (!fstat(STDIN_FILENO, &st) && S_ISFIFO(st.st_mode))
			input_name = "-";
		else
			input_name = "perf.data";
	}

	file.path  = input_name;
	file.force = report.force;

repeat:
	session = perf_session__new(&file, false, &report.tool);
>>>>>>> refs/remotes/origin/master
	if (session == NULL)
		return -ENOMEM;

	report.session = session;

	has_br_stack = perf_header__has_feat(&session->header,
					     HEADER_BRANCH_STACK);

<<<<<<< HEAD
	if (sort__branch_mode == -1 && has_br_stack)
		sort__branch_mode = 1;

	/* sort__branch_mode could be 0 if --no-branch-stack */
	if (sort__branch_mode == 1) {
=======
	if (branch_mode == -1 && has_br_stack)
		sort__mode = SORT_MODE__BRANCH;

	/* sort__mode could be NORMAL if --no-branch-stack */
	if (sort__mode == SORT_MODE__BRANCH) {
>>>>>>> refs/remotes/origin/master
		/*
		 * if no sort_order is provided, then specify
		 * branch-mode specific order
		 */
		if (sort_order == default_sort_order)
			sort_order = "comm,dso_from,symbol_from,"
				     "dso_to,symbol_to";

	}
<<<<<<< HEAD

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
=======
	if (report.mem_mode) {
		if (sort__mode == SORT_MODE__BRANCH) {
			pr_err("branch and mem mode incompatible\n");
			goto error;
		}
		sort__mode = SORT_MODE__MEMORY;

		/*
		 * if no sort_order is provided, then specify
		 * branch-mode specific order
		 */
		if (sort_order == default_sort_order)
			sort_order = "local_weight,mem,sym,dso,symbol_daddr,dso_daddr,snoop,tlb,locked";
	}

	if (setup_sorting() < 0) {
		parse_options_usage(report_usage, options, "s", 1);
		goto error;
	}

	if (parent_pattern != default_parent_pattern) {
		if (sort_dimension__add("parent") < 0)
			goto error;
	}

	/* Force tty output for header output. */
	if (report.header || report.header_only)
		use_browser = 0;

	if (strcmp(input_name, "-") != 0)
		setup_browser(true);
	else {
		use_browser = 0;
		perf_hpp__init();
	}

	if (report.header || report.header_only) {
		perf_session__fprintf_info(session, stdout,
					   report.show_full_info);
		if (report.header_only)
			return 0;
	} else if (use_browser == 0) {
		fputs("# To display the perf.data header info, please use --header/--header-only options.\n#\n",
		      stdout);
	}

	/*
	 * Only in the TUI browser we are doing integrated annotation,
	 * so don't allocate extra space that won't be used in the stdio
	 * implementation.
	 */
	if (use_browser == 1 && sort__has_sym) {
		symbol_conf.priv_size = sizeof(struct annotation);
		machines__set_symbol_filter(&session->machines,
					    symbol__annotate_init);
>>>>>>> refs/remotes/origin/master
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
=======
		goto error;
>>>>>>> refs/remotes/origin/master

	if (argc) {
		/*
		 * Special case: if there's an argument left then assume that
		 * it's a symbol filter:
		 */
		if (argc > 1)
			usage_with_options(report_usage, options);

		report.symbol_filter_str = argv[0];
	}

<<<<<<< HEAD
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
=======
	sort__setup_elide(stdout);

	ret = __cmd_report(&report);
	if (ret == K_SWITCH_INPUT_DATA) {
		perf_session__delete(session);
		goto repeat;
	} else
		ret = 0;

error:
	perf_session__delete(session);
	return ret;
>>>>>>> refs/remotes/origin/master
}
