/*
 * builtin-top.c
 *
 * Builtin top command: Display a continuously updated profile of
 * any workload, CPU or specific PID.
 *
 * Copyright (C) 2008, Red Hat Inc, Ingo Molnar <mingo@redhat.com>
<<<<<<< HEAD
<<<<<<< HEAD
=======
 *		 2011, Red Hat Inc, Arnaldo Carvalho de Melo <acme@redhat.com>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *		 2011, Red Hat Inc, Arnaldo Carvalho de Melo <acme@redhat.com>
>>>>>>> refs/remotes/origin/master
 *
 * Improvements and fixes by:
 *
 *   Arjan van de Ven <arjan@linux.intel.com>
 *   Yanmin Zhang <yanmin.zhang@intel.com>
 *   Wu Fengguang <fengguang.wu@intel.com>
 *   Mike Galbraith <efault@gmx.de>
 *   Paul Mackerras <paulus@samba.org>
 *
 * Released under the GPL v2. (and only v2, not any later version)
 */
#include "builtin.h"

#include "perf.h"

#include "util/annotate.h"
#include "util/cache.h"
#include "util/color.h"
#include "util/evlist.h"
#include "util/evsel.h"
<<<<<<< HEAD
=======
#include "util/machine.h"
>>>>>>> refs/remotes/origin/master
#include "util/session.h"
#include "util/symbol.h"
#include "util/thread.h"
#include "util/thread_map.h"
#include "util/top.h"
#include "util/util.h"
#include <linux/rbtree.h>
#include "util/parse-options.h"
#include "util/parse-events.h"
#include "util/cpumap.h"
#include "util/xyarray.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "util/sort.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "util/sort.h"
#include "util/intlist.h"
#include "arch/common.h"
>>>>>>> refs/remotes/origin/master

#include "util/debug.h"

#include <assert.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <elf.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <elf.h>
>>>>>>> refs/remotes/origin/master
#include <fcntl.h>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <inttypes.h>

#include <errno.h>
#include <time.h>
#include <sched.h>

#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <sys/uio.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <sys/utsname.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <sys/utsname.h>
>>>>>>> refs/remotes/origin/master
#include <sys/mman.h>

#include <linux/unistd.h>
#include <linux/types.h>

<<<<<<< HEAD
<<<<<<< HEAD
static struct perf_top top = {
	.count_filter		= 5,
	.delay_secs		= 2,
	.display_weighted	= -1,
	.target_pid		= -1,
	.target_tid		= -1,
	.active_symbols		= LIST_HEAD_INIT(top.active_symbols),
	.active_symbols_lock	= PTHREAD_MUTEX_INITIALIZER,
	.active_symbols_cond	= PTHREAD_COND_INITIALIZER,
	.freq			= 1000, /* 1 KHz */
};

static bool			system_wide			=  false;

static bool			use_tui, use_stdio;

static int			default_interval		=      0;

static bool			kptr_restrict_warned;
static bool			vmlinux_warned;
static bool			inherit				=  false;
static int			realtime_prio			=      0;
static bool			group				=  false;
static unsigned int		page_size;
static unsigned int		mmap_pages			=    128;

static bool			dump_symtab                     =  false;

static struct winsize		winsize;

static const char		*sym_filter			=   NULL;
struct sym_entry		*sym_filter_entry_sched		=   NULL;
static int			sym_pcnt_filter			=      5;

/*
 * Source functions
 */

=======
>>>>>>> refs/remotes/origin/cm-10.0
void get_term_dimensions(struct winsize *ws)
{
	char *s = getenv("LINES");

	if (s != NULL) {
		ws->ws_row = atoi(s);
		s = getenv("COLUMNS");
		if (s != NULL) {
			ws->ws_col = atoi(s);
			if (ws->ws_row && ws->ws_col)
				return;
		}
	}
#ifdef TIOCGWINSZ
	if (ioctl(1, TIOCGWINSZ, ws) == 0 &&
	    ws->ws_row && ws->ws_col)
		return;
#endif
	ws->ws_row = 25;
	ws->ws_col = 80;
}

<<<<<<< HEAD
static void update_print_entries(struct winsize *ws)
{
	top.print_entries = ws->ws_row;

	if (top.print_entries > 9)
		top.print_entries -= 9;
}

static void sig_winch_handler(int sig __used)
{
	get_term_dimensions(&winsize);
	update_print_entries(&winsize);
}

static int parse_source(struct sym_entry *syme)
=======
static void perf_top__update_print_entries(struct perf_top *top)
{
	if (top->print_entries > 9)
		top->print_entries -= 9;
}

static void perf_top__sig_winch(int sig __used, siginfo_t *info __used, void *arg)
=======
static volatile int done;

#define HEADER_LINE_NR  5

static void perf_top__update_print_entries(struct perf_top *top)
{
	top->print_entries = top->winsize.ws_row - HEADER_LINE_NR;
}

static void perf_top__sig_winch(int sig __maybe_unused,
				siginfo_t *info __maybe_unused, void *arg)
>>>>>>> refs/remotes/origin/master
{
	struct perf_top *top = arg;

	get_term_dimensions(&top->winsize);
<<<<<<< HEAD
	if (!top->print_entries
	    || (top->print_entries+4) > top->winsize.ws_row) {
		top->print_entries = top->winsize.ws_row;
	} else {
		top->print_entries += 4;
		top->winsize.ws_row = top->print_entries;
	}
=======
>>>>>>> refs/remotes/origin/master
	perf_top__update_print_entries(top);
}

static int perf_top__parse_source(struct perf_top *top, struct hist_entry *he)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct symbol *sym;
	struct annotation *notes;
	struct map *map;
	int err = -1;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!syme)
		return -1;

	sym = sym_entry__symbol(syme);
	map = syme->map;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!he || !he->ms.sym)
		return -1;

	sym = he->ms.sym;
	map = he->ms.map;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * We can't annotate with just /proc/kallsyms
	 */
<<<<<<< HEAD
	if (map->dso->symtab_type == SYMTAB__KALLSYMS) {
=======
	if (map->dso->symtab_type == DSO_BINARY_TYPE__KALLSYMS &&
	    !dso__is_kcore(map->dso)) {
>>>>>>> refs/remotes/origin/master
		pr_err("Can't annotate %s: No vmlinux file was found in the "
		       "path\n", sym->name);
		sleep(1);
		return -1;
	}

	notes = symbol__annotation(sym);
	if (notes->src != NULL) {
		pthread_mutex_lock(&notes->lock);
		goto out_assign;
	}

	pthread_mutex_lock(&notes->lock);

<<<<<<< HEAD
<<<<<<< HEAD
	if (symbol__alloc_hist(sym, top.evlist->nr_entries) < 0) {
=======
	if (symbol__alloc_hist(sym) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (symbol__alloc_hist(sym) < 0) {
>>>>>>> refs/remotes/origin/master
		pthread_mutex_unlock(&notes->lock);
		pr_err("Not enough memory for annotating '%s' symbol!\n",
		       sym->name);
		sleep(1);
		return err;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	err = symbol__annotate(sym, syme->map, 0);
	if (err == 0) {
out_assign:
		top.sym_filter_entry = syme;
=======
=======
>>>>>>> refs/remotes/origin/master
	err = symbol__annotate(sym, map, 0);
	if (err == 0) {
out_assign:
		top->sym_filter_entry = he;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	pthread_mutex_unlock(&notes->lock);
	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void __zero_source_counters(struct sym_entry *syme)
{
	struct symbol *sym = sym_entry__symbol(syme);
	symbol__annotate_zero_histograms(sym);
}

static void record_precise_ip(struct sym_entry *syme, int counter, u64 ip)
{
	struct annotation *notes;
	struct symbol *sym;

	if (syme != top.sym_filter_entry)
		return;

	sym = sym_entry__symbol(syme);
=======
=======
>>>>>>> refs/remotes/origin/master
static void __zero_source_counters(struct hist_entry *he)
{
	struct symbol *sym = he->ms.sym;
	symbol__annotate_zero_histograms(sym);
}

static void ui__warn_map_erange(struct map *map, struct symbol *sym, u64 ip)
{
	struct utsname uts;
	int err = uname(&uts);

	ui__warning("Out of bounds address found:\n\n"
		    "Addr:   %" PRIx64 "\n"
		    "DSO:    %s %c\n"
		    "Map:    %" PRIx64 "-%" PRIx64 "\n"
		    "Symbol: %" PRIx64 "-%" PRIx64 " %c %s\n"
		    "Arch:   %s\n"
		    "Kernel: %s\n"
		    "Tools:  %s\n\n"
		    "Not all samples will be on the annotation output.\n\n"
		    "Please report to linux-kernel@vger.kernel.org\n",
		    ip, map->dso->long_name, dso__symtab_origin(map->dso),
		    map->start, map->end, sym->start, sym->end,
		    sym->binding == STB_GLOBAL ? 'g' :
		    sym->binding == STB_LOCAL  ? 'l' : 'w', sym->name,
		    err ? "[unknown]" : uts.machine,
		    err ? "[unknown]" : uts.release, perf_version_string);
	if (use_browser <= 0)
		sleep(5);
	
	map->erange_warned = true;
}

static void perf_top__record_precise_ip(struct perf_top *top,
					struct hist_entry *he,
					int counter, u64 ip)
{
	struct annotation *notes;
	struct symbol *sym;
	int err;

	if (he == NULL || he->ms.sym == NULL ||
	    ((top->sym_filter_entry == NULL ||
	      top->sym_filter_entry->ms.sym != he->ms.sym) && use_browser != 1))
		return;

	sym = he->ms.sym;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	notes = symbol__annotation(sym);

	if (pthread_mutex_trylock(&notes->lock))
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	ip = syme->map->map_ip(syme->map, ip);
	symbol__inc_addr_samples(sym, syme->map, counter, ip);

	pthread_mutex_unlock(&notes->lock);
}

static void show_details(struct sym_entry *syme)
{
=======
	if (notes->src == NULL && symbol__alloc_hist(sym) < 0) {
		pthread_mutex_unlock(&notes->lock);
		pr_err("Not enough memory for annotating '%s' symbol!\n",
		       sym->name);
		sleep(1);
		return;
	}

	ip = he->ms.map->map_ip(he->ms.map, ip);
	err = symbol__inc_addr_samples(sym, he->ms.map, counter, ip);

	pthread_mutex_unlock(&notes->lock);

	if (err == -ERANGE && !he->ms.map->erange_warned)
		ui__warn_map_erange(he->ms.map, sym, ip);
=======
	ip = he->ms.map->map_ip(he->ms.map, ip);
	err = hist_entry__inc_addr_samples(he, counter, ip);

	pthread_mutex_unlock(&notes->lock);

	if (err == -ERANGE && !he->ms.map->erange_warned)
		ui__warn_map_erange(he->ms.map, sym, ip);
	else if (err == -ENOMEM) {
		pr_err("Not enough memory for annotating '%s' symbol!\n",
		       sym->name);
		sleep(1);
	}
>>>>>>> refs/remotes/origin/master
}

static void perf_top__show_details(struct perf_top *top)
{
	struct hist_entry *he = top->sym_filter_entry;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct annotation *notes;
	struct symbol *symbol;
	int more;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!syme)
		return;

	symbol = sym_entry__symbol(syme);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!he)
		return;

	symbol = he->ms.sym;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	notes = symbol__annotation(symbol);

	pthread_mutex_lock(&notes->lock);

	if (notes->src == NULL)
		goto out_unlock;

<<<<<<< HEAD
<<<<<<< HEAD
	printf("Showing %s for %s\n", event_name(top.sym_evsel), symbol->name);
	printf("  Events  Pcnt (>=%d%%)\n", sym_pcnt_filter);

	more = symbol__annotate_printf(symbol, syme->map, top.sym_evsel->idx,
				       0, sym_pcnt_filter, top.print_entries, 4);
	if (top.zero)
		symbol__annotate_zero_histogram(symbol, top.sym_evsel->idx);
	else
		symbol__annotate_decay_histogram(symbol, top.sym_evsel->idx);
=======
	printf("Showing %s for %s\n", event_name(top->sym_evsel), symbol->name);
	printf("  Events  Pcnt (>=%d%%)\n", top->sym_pcnt_filter);

	more = symbol__annotate_printf(symbol, he->ms.map, top->sym_evsel->idx,
=======
	printf("Showing %s for %s\n", perf_evsel__name(top->sym_evsel), symbol->name);
	printf("  Events  Pcnt (>=%d%%)\n", top->sym_pcnt_filter);

	more = symbol__annotate_printf(symbol, he->ms.map, top->sym_evsel,
>>>>>>> refs/remotes/origin/master
				       0, top->sym_pcnt_filter, top->print_entries, 4);
	if (top->zero)
		symbol__annotate_zero_histogram(symbol, top->sym_evsel->idx);
	else
		symbol__annotate_decay_histogram(symbol, top->sym_evsel->idx);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (more != 0)
		printf("%d lines not displayed, maybe increase display entries [e]\n", more);
out_unlock:
	pthread_mutex_unlock(&notes->lock);
}

<<<<<<< HEAD
static const char		CONSOLE_CLEAR[] = "[H[2J";

<<<<<<< HEAD
static void __list_insert_active_sym(struct sym_entry *syme)
{
	list_add(&syme->node, &top.active_symbols);
}

static void print_sym_table(struct perf_session *session)
{
	char bf[160];
	int printed = 0;
	struct rb_node *nd;
	struct sym_entry *syme;
	struct rb_root tmp = RB_ROOT;
	const int win_width = winsize.ws_col - 1;
	int sym_width, dso_width, dso_short_width;
	float sum_ksamples = perf_top__decay_samples(&top, &tmp);

	puts(CONSOLE_CLEAR);

	perf_top__header_snprintf(&top, bf, sizeof(bf));
	printf("%s\n", bf);

	perf_top__reset_sample_counters(&top);

	printf("%-*.*s\n", win_width, win_width, graph_dotted_line);

	if (session->hists.stats.total_lost != 0) {
		color_fprintf(stdout, PERF_COLOR_RED, "WARNING:");
		printf(" LOST %" PRIu64 " events, Check IO/CPU overload\n",
		       session->hists.stats.total_lost);
	}

	if (top.sym_filter_entry) {
		show_details(top.sym_filter_entry);
		return;
	}

	perf_top__find_widths(&top, &tmp, &dso_width, &dso_short_width,
			      &sym_width);

	if (sym_width + dso_width > winsize.ws_col - 29) {
		dso_width = dso_short_width;
		if (sym_width + dso_width > winsize.ws_col - 29)
			sym_width = winsize.ws_col - dso_width - 29;
	}
	putchar('\n');
	if (top.evlist->nr_entries == 1)
		printf("             samples  pcnt");
	else
		printf("   weight    samples  pcnt");

	if (verbose)
		printf("         RIP       ");
	printf(" %-*.*s DSO\n", sym_width, sym_width, "function");
	printf("   %s    _______ _____",
	       top.evlist->nr_entries == 1 ? "      " : "______");
	if (verbose)
		printf(" ________________");
	printf(" %-*.*s", sym_width, sym_width, graph_line);
	printf(" %-*.*s", dso_width, dso_width, graph_line);
	puts("\n");

	for (nd = rb_first(&tmp); nd; nd = rb_next(nd)) {
		struct symbol *sym;
		double pcnt;

		syme = rb_entry(nd, struct sym_entry, rb_node);
		sym = sym_entry__symbol(syme);
		if (++printed > top.print_entries ||
		    (int)syme->snap_count < top.count_filter)
			continue;

		pcnt = 100.0 - (100.0 * ((sum_ksamples - syme->snap_count) /
					 sum_ksamples));

		if (top.evlist->nr_entries == 1 || !top.display_weighted)
			printf("%20.2f ", syme->weight);
		else
			printf("%9.1f %10ld ", syme->weight, syme->snap_count);

		percent_color_fprintf(stdout, "%4.1f%%", pcnt);
		if (verbose)
			printf(" %016" PRIx64, sym->start);
		printf(" %-*.*s", sym_width, sym_width, sym->name);
		printf(" %-*.*s\n", dso_width, dso_width,
		       dso_width >= syme->map->dso->long_name_len ?
					syme->map->dso->long_name :
					syme->map->dso->short_name);
	}
=======
=======
>>>>>>> refs/remotes/origin/master
static struct hist_entry *perf_evsel__add_hist_entry(struct perf_evsel *evsel,
						     struct addr_location *al,
						     struct perf_sample *sample)
{
	struct hist_entry *he;

<<<<<<< HEAD
	he = __hists__add_entry(&evsel->hists, al, NULL, sample->period);
=======
	pthread_mutex_lock(&evsel->hists.lock);
	he = __hists__add_entry(&evsel->hists, al, NULL, NULL, NULL,
				sample->period, sample->weight,
				sample->transaction);
	pthread_mutex_unlock(&evsel->hists.lock);
>>>>>>> refs/remotes/origin/master
	if (he == NULL)
		return NULL;

	hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
	return he;
}

static void perf_top__print_sym_table(struct perf_top *top)
{
	char bf[160];
	int printed = 0;
	const int win_width = top->winsize.ws_col - 1;

	puts(CONSOLE_CLEAR);

	perf_top__header_snprintf(top, bf, sizeof(bf));
	printf("%s\n", bf);

	perf_top__reset_sample_counters(top);

	printf("%-*.*s\n", win_width, win_width, graph_dotted_line);

	if (top->sym_evsel->hists.stats.nr_lost_warned !=
	    top->sym_evsel->hists.stats.nr_events[PERF_RECORD_LOST]) {
		top->sym_evsel->hists.stats.nr_lost_warned =
			top->sym_evsel->hists.stats.nr_events[PERF_RECORD_LOST];
		color_fprintf(stdout, PERF_COLOR_RED,
			      "WARNING: LOST %d chunks, Check IO/CPU overload",
			      top->sym_evsel->hists.stats.nr_lost_warned);
		++printed;
	}

	if (top->sym_filter_entry) {
		perf_top__show_details(top);
		return;
	}

<<<<<<< HEAD
	hists__collapse_resort_threaded(&top->sym_evsel->hists);
	hists__output_resort_threaded(&top->sym_evsel->hists);
	hists__decay_entries_threaded(&top->sym_evsel->hists,
				      top->hide_user_symbols,
				      top->hide_kernel_symbols);
	hists__output_recalc_col_len(&top->sym_evsel->hists,
				     top->winsize.ws_row - 3);
	putchar('\n');
	hists__fprintf(&top->sym_evsel->hists, NULL, false, false,
		       top->winsize.ws_row - 4 - printed, win_width, stdout);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	hists__collapse_resort(&top->sym_evsel->hists, NULL);
	hists__output_resort(&top->sym_evsel->hists);
	hists__decay_entries(&top->sym_evsel->hists,
			     top->hide_user_symbols,
			     top->hide_kernel_symbols);
	hists__output_recalc_col_len(&top->sym_evsel->hists,
				     top->print_entries - printed);
	putchar('\n');
	hists__fprintf(&top->sym_evsel->hists, false,
		       top->print_entries - printed, win_width,
		       top->min_percent, stdout);
>>>>>>> refs/remotes/origin/master
}

static void prompt_integer(int *target, const char *msg)
{
	char *buf = malloc(0), *p;
	size_t dummy = 0;
	int tmp;

	fprintf(stdout, "\n%s: ", msg);
	if (getline(&buf, &dummy, stdin) < 0)
		return;

	p = strchr(buf, '\n');
	if (p)
		*p = 0;

	p = buf;
	while(*p) {
		if (!isdigit(*p))
			goto out_free;
		p++;
	}
	tmp = strtoul(buf, NULL, 10);
	*target = tmp;
out_free:
	free(buf);
}

static void prompt_percent(int *target, const char *msg)
{
	int tmp = 0;

	prompt_integer(&tmp, msg);
	if (tmp >= 0 && tmp <= 100)
		*target = tmp;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void prompt_symbol(struct sym_entry **target, const char *msg)
{
	char *buf = malloc(0), *p;
	struct sym_entry *syme = *target, *n, *found = NULL;
=======
=======
>>>>>>> refs/remotes/origin/master
static void perf_top__prompt_symbol(struct perf_top *top, const char *msg)
{
	char *buf = malloc(0), *p;
	struct hist_entry *syme = top->sym_filter_entry, *n, *found = NULL;
	struct rb_node *next;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	size_t dummy = 0;

	/* zero counters of active symbol */
	if (syme) {
		__zero_source_counters(syme);
<<<<<<< HEAD
<<<<<<< HEAD
		*target = NULL;
=======
		top->sym_filter_entry = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		top->sym_filter_entry = NULL;
>>>>>>> refs/remotes/origin/master
	}

	fprintf(stdout, "\n%s: ", msg);
	if (getline(&buf, &dummy, stdin) < 0)
		goto out_free;

	p = strchr(buf, '\n');
	if (p)
		*p = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	pthread_mutex_lock(&top.active_symbols_lock);
	syme = list_entry(top.active_symbols.next, struct sym_entry, node);
	pthread_mutex_unlock(&top.active_symbols_lock);

	list_for_each_entry_safe_from(syme, n, &top.active_symbols, node) {
		struct symbol *sym = sym_entry__symbol(syme);

		if (!strcmp(buf, sym->name)) {
			found = syme;
			break;
		}
=======
=======
>>>>>>> refs/remotes/origin/master
	next = rb_first(&top->sym_evsel->hists.entries);
	while (next) {
		n = rb_entry(next, struct hist_entry, rb_node);
		if (n->ms.sym && !strcmp(buf, n->ms.sym->name)) {
			found = n;
			break;
		}
		next = rb_next(&n->rb_node);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	if (!found) {
		fprintf(stderr, "Sorry, %s is not active.\n", buf);
		sleep(1);
<<<<<<< HEAD
<<<<<<< HEAD
		return;
	} else
		parse_source(found);
=======
	} else
		perf_top__parse_source(top, found);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	} else
		perf_top__parse_source(top, found);
>>>>>>> refs/remotes/origin/master

out_free:
	free(buf);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void print_mapped_keys(void)
{
	char *name = NULL;

	if (top.sym_filter_entry) {
		struct symbol *sym = sym_entry__symbol(top.sym_filter_entry);
=======
=======
>>>>>>> refs/remotes/origin/master
static void perf_top__print_mapped_keys(struct perf_top *top)
{
	char *name = NULL;

	if (top->sym_filter_entry) {
		struct symbol *sym = top->sym_filter_entry->ms.sym;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		name = sym->name;
	}

	fprintf(stdout, "\nMapped keys:\n");
<<<<<<< HEAD
<<<<<<< HEAD
	fprintf(stdout, "\t[d]     display refresh delay.             \t(%d)\n", top.delay_secs);
	fprintf(stdout, "\t[e]     display entries (lines).           \t(%d)\n", top.print_entries);

	if (top.evlist->nr_entries > 1)
		fprintf(stdout, "\t[E]     active event counter.              \t(%s)\n", event_name(top.sym_evsel));

	fprintf(stdout, "\t[f]     profile display filter (count).    \t(%d)\n", top.count_filter);

	fprintf(stdout, "\t[F]     annotate display filter (percent). \t(%d%%)\n", sym_pcnt_filter);
	fprintf(stdout, "\t[s]     annotate symbol.                   \t(%s)\n", name?: "NULL");
	fprintf(stdout, "\t[S]     stop annotation.\n");

	if (top.evlist->nr_entries > 1)
		fprintf(stdout, "\t[w]     toggle display weighted/count[E]r. \t(%d)\n", top.display_weighted ? 1 : 0);

	fprintf(stdout,
		"\t[K]     hide kernel_symbols symbols.     \t(%s)\n",
		top.hide_kernel_symbols ? "yes" : "no");
	fprintf(stdout,
		"\t[U]     hide user symbols.               \t(%s)\n",
		top.hide_user_symbols ? "yes" : "no");
	fprintf(stdout, "\t[z]     toggle sample zeroing.             \t(%d)\n", top.zero ? 1 : 0);
	fprintf(stdout, "\t[qQ]    quit.\n");
}

static int key_mapped(int c)
=======
=======
>>>>>>> refs/remotes/origin/master
	fprintf(stdout, "\t[d]     display refresh delay.             \t(%d)\n", top->delay_secs);
	fprintf(stdout, "\t[e]     display entries (lines).           \t(%d)\n", top->print_entries);

	if (top->evlist->nr_entries > 1)
<<<<<<< HEAD
		fprintf(stdout, "\t[E]     active event counter.              \t(%s)\n", event_name(top->sym_evsel));
=======
		fprintf(stdout, "\t[E]     active event counter.              \t(%s)\n", perf_evsel__name(top->sym_evsel));
>>>>>>> refs/remotes/origin/master

	fprintf(stdout, "\t[f]     profile display filter (count).    \t(%d)\n", top->count_filter);

	fprintf(stdout, "\t[F]     annotate display filter (percent). \t(%d%%)\n", top->sym_pcnt_filter);
	fprintf(stdout, "\t[s]     annotate symbol.                   \t(%s)\n", name?: "NULL");
	fprintf(stdout, "\t[S]     stop annotation.\n");

	fprintf(stdout,
		"\t[K]     hide kernel_symbols symbols.     \t(%s)\n",
		top->hide_kernel_symbols ? "yes" : "no");
	fprintf(stdout,
		"\t[U]     hide user symbols.               \t(%s)\n",
		top->hide_user_symbols ? "yes" : "no");
	fprintf(stdout, "\t[z]     toggle sample zeroing.             \t(%d)\n", top->zero ? 1 : 0);
	fprintf(stdout, "\t[qQ]    quit.\n");
}

static int perf_top__key_mapped(struct perf_top *top, int c)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	switch (c) {
		case 'd':
		case 'e':
		case 'f':
		case 'z':
		case 'q':
		case 'Q':
		case 'K':
		case 'U':
		case 'F':
		case 's':
		case 'S':
			return 1;
		case 'E':
<<<<<<< HEAD
<<<<<<< HEAD
		case 'w':
			return top.evlist->nr_entries > 1 ? 1 : 0;
=======
			return top->evlist->nr_entries > 1 ? 1 : 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			return top->evlist->nr_entries > 1 ? 1 : 0;
>>>>>>> refs/remotes/origin/master
		default:
			break;
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void handle_keypress(struct perf_session *session, int c)
{
	if (!key_mapped(c)) {
		struct pollfd stdin_poll = { .fd = 0, .events = POLLIN };
		struct termios tc, save;

		print_mapped_keys();
=======
static void perf_top__handle_keypress(struct perf_top *top, int c)
{
=======
static bool perf_top__handle_keypress(struct perf_top *top, int c)
{
	bool ret = true;

>>>>>>> refs/remotes/origin/master
	if (!perf_top__key_mapped(top, c)) {
		struct pollfd stdin_poll = { .fd = 0, .events = POLLIN };
		struct termios tc, save;

		perf_top__print_mapped_keys(top);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		fprintf(stdout, "\nEnter selection, or unmapped key to continue: ");
		fflush(stdout);

		tcgetattr(0, &save);
		tc = save;
		tc.c_lflag &= ~(ICANON | ECHO);
		tc.c_cc[VMIN] = 0;
		tc.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &tc);

		poll(&stdin_poll, 1, -1);
		c = getc(stdin);

		tcsetattr(0, TCSAFLUSH, &save);
<<<<<<< HEAD
<<<<<<< HEAD
		if (!key_mapped(c))
=======
		if (!perf_top__key_mapped(top, c))
>>>>>>> refs/remotes/origin/cm-10.0
			return;
=======
		if (!perf_top__key_mapped(top, c))
			return ret;
>>>>>>> refs/remotes/origin/master
	}

	switch (c) {
		case 'd':
<<<<<<< HEAD
<<<<<<< HEAD
			prompt_integer(&top.delay_secs, "Enter display delay");
			if (top.delay_secs < 1)
				top.delay_secs = 1;
			break;
		case 'e':
			prompt_integer(&top.print_entries, "Enter display entries (lines)");
			if (top.print_entries == 0) {
				sig_winch_handler(SIGWINCH);
				signal(SIGWINCH, sig_winch_handler);
			} else
				signal(SIGWINCH, SIG_DFL);
			break;
		case 'E':
			if (top.evlist->nr_entries > 1) {
=======
=======
>>>>>>> refs/remotes/origin/master
			prompt_integer(&top->delay_secs, "Enter display delay");
			if (top->delay_secs < 1)
				top->delay_secs = 1;
			break;
		case 'e':
			prompt_integer(&top->print_entries, "Enter display entries (lines)");
			if (top->print_entries == 0) {
				struct sigaction act = {
					.sa_sigaction = perf_top__sig_winch,
					.sa_flags     = SA_SIGINFO,
				};
				perf_top__sig_winch(SIGWINCH, NULL, top);
				sigaction(SIGWINCH, &act, NULL);
			} else {
<<<<<<< HEAD
				perf_top__sig_winch(SIGWINCH, NULL, top);
=======
>>>>>>> refs/remotes/origin/master
				signal(SIGWINCH, SIG_DFL);
			}
			break;
		case 'E':
			if (top->evlist->nr_entries > 1) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				/* Select 0 as the default event: */
				int counter = 0;

				fprintf(stderr, "\nAvailable events:");

<<<<<<< HEAD
<<<<<<< HEAD
				list_for_each_entry(top.sym_evsel, &top.evlist->entries, node)
					fprintf(stderr, "\n\t%d %s", top.sym_evsel->idx, event_name(top.sym_evsel));

				prompt_integer(&counter, "Enter details event counter");

				if (counter >= top.evlist->nr_entries) {
					top.sym_evsel = list_entry(top.evlist->entries.next, struct perf_evsel, node);
					fprintf(stderr, "Sorry, no such event, using %s.\n", event_name(top.sym_evsel));
					sleep(1);
					break;
				}
				list_for_each_entry(top.sym_evsel, &top.evlist->entries, node)
					if (top.sym_evsel->idx == counter)
						break;
			} else
				top.sym_evsel = list_entry(top.evlist->entries.next, struct perf_evsel, node);
			break;
		case 'f':
			prompt_integer(&top.count_filter, "Enter display event count filter");
			break;
		case 'F':
			prompt_percent(&sym_pcnt_filter, "Enter details display event filter (percent)");
			break;
		case 'K':
			top.hide_kernel_symbols = !top.hide_kernel_symbols;
=======
				list_for_each_entry(top->sym_evsel, &top->evlist->entries, node)
					fprintf(stderr, "\n\t%d %s", top->sym_evsel->idx, event_name(top->sym_evsel));
=======
				evlist__for_each(top->evlist, top->sym_evsel)
					fprintf(stderr, "\n\t%d %s", top->sym_evsel->idx, perf_evsel__name(top->sym_evsel));
>>>>>>> refs/remotes/origin/master

				prompt_integer(&counter, "Enter details event counter");

				if (counter >= top->evlist->nr_entries) {
<<<<<<< HEAD
					top->sym_evsel = list_entry(top->evlist->entries.next, struct perf_evsel, node);
					fprintf(stderr, "Sorry, no such event, using %s.\n", event_name(top->sym_evsel));
					sleep(1);
					break;
				}
				list_for_each_entry(top->sym_evsel, &top->evlist->entries, node)
					if (top->sym_evsel->idx == counter)
						break;
			} else
				top->sym_evsel = list_entry(top->evlist->entries.next, struct perf_evsel, node);
=======
					top->sym_evsel = perf_evlist__first(top->evlist);
					fprintf(stderr, "Sorry, no such event, using %s.\n", perf_evsel__name(top->sym_evsel));
					sleep(1);
					break;
				}
				evlist__for_each(top->evlist, top->sym_evsel)
					if (top->sym_evsel->idx == counter)
						break;
			} else
				top->sym_evsel = perf_evlist__first(top->evlist);
>>>>>>> refs/remotes/origin/master
			break;
		case 'f':
			prompt_integer(&top->count_filter, "Enter display event count filter");
			break;
		case 'F':
			prompt_percent(&top->sym_pcnt_filter,
				       "Enter details display event filter (percent)");
			break;
		case 'K':
			top->hide_kernel_symbols = !top->hide_kernel_symbols;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			break;
		case 'q':
		case 'Q':
			printf("exiting.\n");
<<<<<<< HEAD
<<<<<<< HEAD
			if (dump_symtab)
				perf_session__fprintf_dsos(session, stderr);
			exit(0);
		case 's':
			prompt_symbol(&top.sym_filter_entry, "Enter details symbol");
			break;
		case 'S':
			if (!top.sym_filter_entry)
				break;
			else {
				struct sym_entry *syme = top.sym_filter_entry;

				top.sym_filter_entry = NULL;
=======
			if (top->dump_symtab)
				perf_session__fprintf_dsos(top->session, stderr);
			exit(0);
=======
			if (top->dump_symtab)
				perf_session__fprintf_dsos(top->session, stderr);
			ret = false;
			break;
>>>>>>> refs/remotes/origin/master
		case 's':
			perf_top__prompt_symbol(top, "Enter details symbol");
			break;
		case 'S':
			if (!top->sym_filter_entry)
				break;
			else {
				struct hist_entry *syme = top->sym_filter_entry;

				top->sym_filter_entry = NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				__zero_source_counters(syme);
			}
			break;
		case 'U':
<<<<<<< HEAD
<<<<<<< HEAD
			top.hide_user_symbols = !top.hide_user_symbols;
			break;
		case 'w':
			top.display_weighted = ~top.display_weighted;
			break;
		case 'z':
			top.zero = !top.zero;
=======
=======
>>>>>>> refs/remotes/origin/master
			top->hide_user_symbols = !top->hide_user_symbols;
			break;
		case 'z':
			top->zero = !top->zero;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			break;
		default:
			break;
	}
<<<<<<< HEAD
}

<<<<<<< HEAD
static void *display_thread_tui(void *arg __used)
{
	int err = 0;
	pthread_mutex_lock(&top.active_symbols_lock);
	while (list_empty(&top.active_symbols)) {
		err = pthread_cond_wait(&top.active_symbols_cond,
					&top.active_symbols_lock);
		if (err)
			break;
	}
	pthread_mutex_unlock(&top.active_symbols_lock);
	if (!err)
		perf_top__tui_browser(&top);
=======
=======

	return ret;
}

>>>>>>> refs/remotes/origin/master
static void perf_top__sort_new_samples(void *arg)
{
	struct perf_top *t = arg;
	perf_top__reset_sample_counters(t);

	if (t->evlist->selected != NULL)
		t->sym_evsel = t->evlist->selected;

<<<<<<< HEAD
	hists__collapse_resort_threaded(&t->sym_evsel->hists);
	hists__output_resort_threaded(&t->sym_evsel->hists);
	hists__decay_entries_threaded(&t->sym_evsel->hists,
				      t->hide_user_symbols,
				      t->hide_kernel_symbols);
=======
	hists__collapse_resort(&t->sym_evsel->hists, NULL);
	hists__output_resort(&t->sym_evsel->hists);
	hists__decay_entries(&t->sym_evsel->hists,
			     t->hide_user_symbols,
			     t->hide_kernel_symbols);
>>>>>>> refs/remotes/origin/master
}

static void *display_thread_tui(void *arg)
{
	struct perf_evsel *pos;
	struct perf_top *top = arg;
	const char *help = "For a higher level overview, try: perf top --sort comm,dso";
<<<<<<< HEAD
=======
	struct hist_browser_timer hbt = {
		.timer		= perf_top__sort_new_samples,
		.arg		= top,
		.refresh	= top->delay_secs,
	};
>>>>>>> refs/remotes/origin/master

	perf_top__sort_new_samples(top);

	/*
	 * Initialize the uid_filter_str, in the future the TUI will allow
	 * Zooming in/out UIDs. For now juse use whatever the user passed
	 * via --uid.
	 */
<<<<<<< HEAD
	list_for_each_entry(pos, &top->evlist->entries, node)
		pos->hists.uid_filter_str = top->uid_str;

	perf_evlist__tui_browse_hists(top->evlist, help,
				      perf_top__sort_new_samples,
				      top, top->delay_secs);

>>>>>>> refs/remotes/origin/cm-10.0
	exit_browser(0);
	exit(0);
	return NULL;
}

<<<<<<< HEAD
static void *display_thread(void *arg __used)
{
	struct pollfd stdin_poll = { .fd = 0, .events = POLLIN };
	struct termios tc, save;
	int delay_msecs, c;
	struct perf_session *session = (struct perf_session *) arg;
=======
=======
	evlist__for_each(top->evlist, pos)
		pos->hists.uid_filter_str = top->record_opts.target.uid_str;

	perf_evlist__tui_browse_hists(top->evlist, help, &hbt, top->min_percent,
				      &top->session->header.env);

	done = 1;
	return NULL;
}

>>>>>>> refs/remotes/origin/master
static void *display_thread(void *arg)
{
	struct pollfd stdin_poll = { .fd = 0, .events = POLLIN };
	struct termios tc, save;
	struct perf_top *top = arg;
	int delay_msecs, c;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	tcgetattr(0, &save);
	tc = save;
	tc.c_lflag &= ~(ICANON | ECHO);
	tc.c_cc[VMIN] = 0;
	tc.c_cc[VTIME] = 0;

<<<<<<< HEAD
<<<<<<< HEAD
repeat:
	delay_msecs = top.delay_secs * 1000;
=======
	pthread__unblock_sigwinch();
repeat:
	delay_msecs = top->delay_secs * 1000;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pthread__unblock_sigwinch();
repeat:
	delay_msecs = top->delay_secs * 1000;
>>>>>>> refs/remotes/origin/master
	tcsetattr(0, TCSANOW, &tc);
	/* trash return*/
	getc(stdin);

<<<<<<< HEAD
<<<<<<< HEAD
	do {
		print_sym_table(session);
	} while (!poll(&stdin_poll, 1, delay_msecs) == 1);

	c = getc(stdin);
	tcsetattr(0, TCSAFLUSH, &save);

	handle_keypress(session, c);
=======
	while (1) {
=======
	while (!done) {
>>>>>>> refs/remotes/origin/master
		perf_top__print_sym_table(top);
		/*
		 * Either timeout expired or we got an EINTR due to SIGWINCH,
		 * refresh screen in both cases.
		 */
		switch (poll(&stdin_poll, 1, delay_msecs)) {
		case 0:
			continue;
		case -1:
			if (errno == EINTR)
				continue;
			/* Fall trhu */
		default:
<<<<<<< HEAD
			goto process_hotkey;
		}
	}
process_hotkey:
	c = getc(stdin);
	tcsetattr(0, TCSAFLUSH, &save);

	perf_top__handle_keypress(top, c);
>>>>>>> refs/remotes/origin/cm-10.0
	goto repeat;
=======
			c = getc(stdin);
			tcsetattr(0, TCSAFLUSH, &save);

			if (perf_top__handle_keypress(top, c))
				goto repeat;
			done = 1;
		}
	}
>>>>>>> refs/remotes/origin/master

	return NULL;
}

<<<<<<< HEAD
/* Tag samples to be skipped. */
static const char *skip_symbols[] = {
<<<<<<< HEAD
=======
	"intel_idle",
>>>>>>> refs/remotes/origin/cm-10.0
	"default_idle",
	"native_safe_halt",
	"cpu_idle",
	"enter_idle",
	"exit_idle",
	"mwait_idle",
	"mwait_idle_with_hints",
	"poll_idle",
	"ppc64_runlatch_off",
	"pseries_dedicated_idle_sleep",
	NULL
};

<<<<<<< HEAD
static int symbol_filter(struct map *map, struct symbol *sym)
{
	struct sym_entry *syme;
=======
static int symbol_filter(struct map *map __used, struct symbol *sym)
{
>>>>>>> refs/remotes/origin/cm-10.0
	const char *name = sym->name;
	int i;
=======
static int symbol_filter(struct map *map __maybe_unused, struct symbol *sym)
{
	const char *name = sym->name;
>>>>>>> refs/remotes/origin/master

	/*
	 * ppc64 uses function descriptors and appends a '.' to the
	 * start of every instruction address. Remove it.
	 */
	if (name[0] == '.')
		name++;

	if (!strcmp(name, "_text") ||
	    !strcmp(name, "_etext") ||
	    !strcmp(name, "_sinittext") ||
	    !strncmp("init_module", name, 11) ||
	    !strncmp("cleanup_module", name, 14) ||
	    strstr(name, "_text_start") ||
	    strstr(name, "_text_end"))
		return 1;

<<<<<<< HEAD
<<<<<<< HEAD
	syme = symbol__priv(sym);
	syme->map = map;
	symbol__annotate_init(map, sym);

	if (!top.sym_filter_entry && sym_filter && !strcmp(name, sym_filter)) {
		/* schedule initial sym_filter_entry setup */
		sym_filter_entry_sched = syme;
		sym_filter = NULL;
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
	for (i = 0; skip_symbols[i]; i++) {
		if (!strcmp(skip_symbols[i], name)) {
			sym->ignore = true;
			break;
		}
	}
=======
	if (symbol__is_idle(sym))
		sym->ignore = true;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void perf_event__process_sample(const union perf_event *event,
				       struct perf_sample *sample,
				       struct perf_session *session)
{
	u64 ip = event->ip.ip;
	struct sym_entry *syme;
	struct addr_location al;
	struct machine *machine;
	u8 origin = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;

	++top.samples;

	switch (origin) {
	case PERF_RECORD_MISC_USER:
		++top.us_samples;
		if (top.hide_user_symbols)
			return;
		machine = perf_session__find_host_machine(session);
		break;
	case PERF_RECORD_MISC_KERNEL:
		++top.kernel_samples;
		if (top.hide_kernel_symbols)
			return;
		machine = perf_session__find_host_machine(session);
		break;
	case PERF_RECORD_MISC_GUEST_KERNEL:
		++top.guest_kernel_samples;
		machine = perf_session__find_machine(session, event->ip.pid);
		break;
	case PERF_RECORD_MISC_GUEST_USER:
		++top.guest_us_samples;
		/*
		 * TODO: we don't process guest user from host side
		 * except simple counting.
		 */
		return;
	default:
		return;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
static void perf_event__process_sample(struct perf_tool *tool,
				       const union perf_event *event,
				       struct perf_evsel *evsel,
				       struct perf_sample *sample,
				       struct machine *machine)
{
	struct perf_top *top = container_of(tool, struct perf_top, tool);
	struct symbol *parent = NULL;
<<<<<<< HEAD
	u64 ip = event->ip.ip;
	struct addr_location al;
	int err;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!machine && perf_guest) {
		pr_err("Can't find guest [%d]'s kernel information\n",
			event->ip.pid);
		return;
	}

<<<<<<< HEAD
	if (event->header.misc & PERF_RECORD_MISC_EXACT_IP)
		top.exact_samples++;

	if (perf_event__preprocess_sample(event, session, &al, sample,
=======
	if (!machine) {
		pr_err("%u unprocessable samples recorded.",
		       top->session->hists.stats.nr_unprocessable_samples++);
=======
	u64 ip = sample->ip;
	struct addr_location al;
	int err;

	if (!machine && perf_guest) {
		static struct intlist *seen;

		if (!seen)
			seen = intlist__new(NULL);

		if (!intlist__has_entry(seen, sample->pid)) {
			pr_err("Can't find guest [%d]'s kernel information\n",
				sample->pid);
			intlist__add(seen, sample->pid);
		}
		return;
	}

	if (!machine) {
		pr_err("%u unprocessable samples recorded.\r",
		       top->session->stats.nr_unprocessable_samples++);
>>>>>>> refs/remotes/origin/master
		return;
	}

	if (event->header.misc & PERF_RECORD_MISC_EXACT_IP)
		top->exact_samples++;

<<<<<<< HEAD
	if (perf_event__preprocess_sample(event, machine, &al, sample,
>>>>>>> refs/remotes/origin/cm-10.0
					  symbol_filter) < 0 ||
	    al.filtered)
		return;

<<<<<<< HEAD
	if (!kptr_restrict_warned &&
=======
	if (!top->kptr_restrict_warned &&
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (perf_event__preprocess_sample(event, machine, &al, sample) < 0 ||
	    al.filtered)
		return;

	if (!top->kptr_restrict_warned &&
>>>>>>> refs/remotes/origin/master
	    symbol_conf.kptr_restrict &&
	    al.cpumode == PERF_RECORD_MISC_KERNEL) {
		ui__warning(
"Kernel address maps (/proc/{kallsyms,modules}) are restricted.\n\n"
"Check /proc/sys/kernel/kptr_restrict.\n\n"
"Kernel%s samples will not be resolved.\n",
			  !RB_EMPTY_ROOT(&al.map->dso->symbols[MAP__FUNCTION]) ?
			  " modules" : "");
		if (use_browser <= 0)
			sleep(5);
<<<<<<< HEAD
<<<<<<< HEAD
		kptr_restrict_warned = true;
=======
		top->kptr_restrict_warned = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		top->kptr_restrict_warned = true;
>>>>>>> refs/remotes/origin/master
	}

	if (al.sym == NULL) {
		const char *msg = "Kernel samples will not be resolved.\n";
		/*
		 * As we do lazy loading of symtabs we only will know if the
		 * specified vmlinux file is invalid when we actually have a
		 * hit in kernel space and then try to load it. So if we get
		 * here and there are _no_ symbols in the DSO backing the
		 * kernel map, bail out.
		 *
		 * We may never get here, for instance, if we use -K/
		 * --hide-kernel-symbols, even if the user specifies an
		 * invalid --vmlinux ;-)
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (!kptr_restrict_warned && !vmlinux_warned &&
=======
		if (!top->kptr_restrict_warned && !top->vmlinux_warned &&
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!top->kptr_restrict_warned && !top->vmlinux_warned &&
>>>>>>> refs/remotes/origin/master
		    al.map == machine->vmlinux_maps[MAP__FUNCTION] &&
		    RB_EMPTY_ROOT(&al.map->dso->symbols[MAP__FUNCTION])) {
			if (symbol_conf.vmlinux_name) {
				ui__warning("The %s file can't be used.\n%s",
					    symbol_conf.vmlinux_name, msg);
			} else {
				ui__warning("A vmlinux file was not found.\n%s",
					    msg);
			}

			if (use_browser <= 0)
				sleep(5);
<<<<<<< HEAD
<<<<<<< HEAD
			vmlinux_warned = true;
		}

		return;
	}

	/* let's see, whether we need to install initial sym_filter_entry */
	if (sym_filter_entry_sched) {
		top.sym_filter_entry = sym_filter_entry_sched;
		sym_filter_entry_sched = NULL;
		if (parse_source(top.sym_filter_entry) < 0) {
			struct symbol *sym = sym_entry__symbol(top.sym_filter_entry);

			pr_err("Can't annotate %s", sym->name);
			if (top.sym_filter_entry->map->dso->symtab_type == SYMTAB__KALLSYMS) {
				pr_err(": No vmlinux file was found in the path:\n");
				machine__fprintf_vmlinux_path(machine, stderr);
			} else
				pr_err(".\n");
			exit(1);
		}
	}

	syme = symbol__priv(al.sym);
	if (!al.sym->ignore) {
		struct perf_evsel *evsel;

		evsel = perf_evlist__id2evsel(top.evlist, sample->id);
		assert(evsel != NULL);
		syme->count[evsel->idx]++;
		record_precise_ip(syme, evsel->idx, ip);
		pthread_mutex_lock(&top.active_symbols_lock);
		if (list_empty(&syme->node) || !syme->node.next) {
			static bool first = true;
			__list_insert_active_sym(syme);
			if (first) {
				pthread_cond_broadcast(&top.active_symbols_cond);
				first = false;
			}
		}
		pthread_mutex_unlock(&top.active_symbols_lock);
	}
}

static void perf_session__mmap_read_idx(struct perf_session *self, int idx)
{
	struct perf_sample sample;
	union perf_event *event;
	int ret;

	while ((event = perf_evlist__mmap_read(top.evlist, idx)) != NULL) {
		ret = perf_session__parse_sample(self, event, &sample);
=======
=======
>>>>>>> refs/remotes/origin/master
			top->vmlinux_warned = true;
		}
	}

	if (al.sym == NULL || !al.sym->ignore) {
		struct hist_entry *he;

<<<<<<< HEAD
		if ((sort__has_parent || symbol_conf.use_callchain) &&
		    sample->callchain) {
			err = machine__resolve_callchain(machine, evsel, al.thread,
							 sample->callchain, &parent);
			if (err)
				return;
		}
=======
		err = sample__resolve_callchain(sample, &parent, evsel, &al,
						top->max_stack);
		if (err)
			return;
>>>>>>> refs/remotes/origin/master

		he = perf_evsel__add_hist_entry(evsel, &al, sample);
		if (he == NULL) {
			pr_err("Problem incrementing symbol period, skipping event\n");
			return;
		}

<<<<<<< HEAD
		if (symbol_conf.use_callchain) {
			err = callchain_append(he->callchain, &evsel->hists.callchain_cursor,
					       sample->period);
			if (err)
				return;
		}

		if (top->sort_has_symbols)
=======
		err = hist_entry__append_callchain(he, sample);
		if (err)
			return;

		if (sort__has_sym)
>>>>>>> refs/remotes/origin/master
			perf_top__record_precise_ip(top, he, evsel->idx, ip);
	}

	return;
}

static void perf_top__mmap_read_idx(struct perf_top *top, int idx)
{
	struct perf_sample sample;
	struct perf_evsel *evsel;
	struct perf_session *session = top->session;
	union perf_event *event;
	struct machine *machine;
	u8 origin;
	int ret;

	while ((event = perf_evlist__mmap_read(top->evlist, idx)) != NULL) {
<<<<<<< HEAD
		ret = perf_session__parse_sample(session, event, &sample);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret) {
			pr_err("Can't parse sample, err = %d\n", ret);
			continue;
		}

<<<<<<< HEAD
		if (event->header.type == PERF_RECORD_SAMPLE)
			perf_event__process_sample(event, &sample, self);
		else
			perf_event__process(event, &sample, self);
	}
}

static void perf_session__mmap_read(struct perf_session *self)
{
	int i;

	for (i = 0; i < top.evlist->nr_mmaps; i++)
		perf_session__mmap_read_idx(self, i);
}

static void start_counters(struct perf_evlist *evlist)
{
	struct perf_evsel *counter;

	list_for_each_entry(counter, &evlist->entries, node) {
		struct perf_event_attr *attr = &counter->attr;

		attr->sample_type = PERF_SAMPLE_IP | PERF_SAMPLE_TID;

		if (top.freq) {
			attr->sample_type |= PERF_SAMPLE_PERIOD;
			attr->freq	  = 1;
			attr->sample_freq = top.freq;
=======
=======
		ret = perf_evlist__parse_sample(top->evlist, event, &sample);
		if (ret) {
			pr_err("Can't parse sample, err = %d\n", ret);
			goto next_event;
		}

>>>>>>> refs/remotes/origin/master
		evsel = perf_evlist__id2evsel(session->evlist, sample.id);
		assert(evsel != NULL);

		origin = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;

		if (event->header.type == PERF_RECORD_SAMPLE)
			++top->samples;

		switch (origin) {
		case PERF_RECORD_MISC_USER:
			++top->us_samples;
			if (top->hide_user_symbols)
<<<<<<< HEAD
				continue;
			machine = perf_session__find_host_machine(session);
=======
				goto next_event;
			machine = &session->machines.host;
>>>>>>> refs/remotes/origin/master
			break;
		case PERF_RECORD_MISC_KERNEL:
			++top->kernel_samples;
			if (top->hide_kernel_symbols)
<<<<<<< HEAD
				continue;
			machine = perf_session__find_host_machine(session);
			break;
		case PERF_RECORD_MISC_GUEST_KERNEL:
			++top->guest_kernel_samples;
			machine = perf_session__find_machine(session, event->ip.pid);
=======
				goto next_event;
			machine = &session->machines.host;
			break;
		case PERF_RECORD_MISC_GUEST_KERNEL:
			++top->guest_kernel_samples;
			machine = perf_session__find_machine(session,
							     sample.pid);
>>>>>>> refs/remotes/origin/master
			break;
		case PERF_RECORD_MISC_GUEST_USER:
			++top->guest_us_samples;
			/*
			 * TODO: we don't process guest user from host side
			 * except simple counting.
			 */
			/* Fall thru */
		default:
<<<<<<< HEAD
			continue;
=======
			goto next_event;
>>>>>>> refs/remotes/origin/master
		}


		if (event->header.type == PERF_RECORD_SAMPLE) {
			perf_event__process_sample(&top->tool, event, evsel,
						   &sample, machine);
		} else if (event->header.type < PERF_RECORD_MAX) {
			hists__inc_nr_events(&evsel->hists, event->header.type);
<<<<<<< HEAD
			perf_event__process(&top->tool, event, &sample, machine);
		} else
			++session->hists.stats.nr_unknown_events;
=======
			machine__process_event(machine, event, &sample);
		} else
			++session->stats.nr_unknown_events;
next_event:
		perf_evlist__mmap_consume(top->evlist, idx);
>>>>>>> refs/remotes/origin/master
	}
}

static void perf_top__mmap_read(struct perf_top *top)
{
	int i;

	for (i = 0; i < top->evlist->nr_mmaps; i++)
		perf_top__mmap_read_idx(top, i);
}

<<<<<<< HEAD
static void perf_top__start_counters(struct perf_top *top)
{
	struct perf_evsel *counter, *first;
	struct perf_evlist *evlist = top->evlist;

	first = list_entry(evlist->entries.next, struct perf_evsel, node);

	list_for_each_entry(counter, &evlist->entries, node) {
		struct perf_event_attr *attr = &counter->attr;
		struct xyarray *group_fd = NULL;

		if (top->group && counter != first)
			group_fd = first->fd;

		attr->sample_type = PERF_SAMPLE_IP | PERF_SAMPLE_TID;

		if (top->freq) {
			attr->sample_type |= PERF_SAMPLE_PERIOD;
			attr->freq	  = 1;
			attr->sample_freq = top->freq;
>>>>>>> refs/remotes/origin/cm-10.0
		}

		if (evlist->nr_entries > 1) {
			attr->sample_type |= PERF_SAMPLE_ID;
			attr->read_format |= PERF_FORMAT_ID;
		}

<<<<<<< HEAD
		attr->mmap = 1;
		attr->inherit = inherit;
try_again:
		if (perf_evsel__open(counter, top.evlist->cpus,
				     top.evlist->threads, group) < 0) {
			int err = errno;

			if (err == EPERM || err == EACCES) {
				ui__warning_paranoid();
				goto out_err;
=======
		if (symbol_conf.use_callchain)
			attr->sample_type |= PERF_SAMPLE_CALLCHAIN;

		attr->mmap = 1;
		attr->comm = 1;
		attr->inherit = top->inherit;
fallback_missing_features:
		if (top->exclude_guest_missing)
			attr->exclude_guest = attr->exclude_host = 0;
retry_sample_id:
		attr->sample_id_all = top->sample_id_all_missing ? 0 : 1;
try_again:
		if (perf_evsel__open(counter, top->evlist->cpus,
				     top->evlist->threads, top->group,
				     group_fd) < 0) {
			int err = errno;

			if (err == EPERM || err == EACCES) {
				ui__error_paranoid();
				goto out_err;
			} else if (err == EINVAL) {
				if (!top->exclude_guest_missing &&
				    (attr->exclude_guest || attr->exclude_host)) {
					pr_debug("Old kernel, cannot exclude "
						 "guest or host samples.\n");
					top->exclude_guest_missing = true;
					goto fallback_missing_features;
				} else if (!top->sample_id_all_missing) {
					/*
					 * Old kernel, no attr->sample_id_type_all field
					 */
					top->sample_id_all_missing = true;
					goto retry_sample_id;
				}
>>>>>>> refs/remotes/origin/cm-10.0
			}
			/*
			 * If it's cycles then fall back to hrtimer
			 * based cpu-clock-tick sw counter, which
			 * is always available even if no PMU support:
			 */
			if (attr->type == PERF_TYPE_HARDWARE &&
			    attr->config == PERF_COUNT_HW_CPU_CYCLES) {
				if (verbose)
					ui__warning("Cycles event not supported,\n"
						    "trying to fall back to cpu-clock-ticks\n");

				attr->type = PERF_TYPE_SOFTWARE;
				attr->config = PERF_COUNT_SW_CPU_CLOCK;
				goto try_again;
			}

			if (err == ENOENT) {
				ui__warning("The %s event is not supported.\n",
					    event_name(counter));
				goto out_err;
<<<<<<< HEAD
=======
			} else if (err == EMFILE) {
				ui__warning("Too many events are opened.\n"
					    "Try again after reducing the number of events\n");
				goto out_err;
>>>>>>> refs/remotes/origin/cm-10.0
			}

			ui__warning("The sys_perf_event_open() syscall "
				    "returned with %d (%s).  /bin/dmesg "
				    "may provide additional information.\n"
				    "No CONFIG_PERF_EVENTS=y kernel support "
				    "configured?\n", err, strerror(err));
=======
static int perf_top__start_counters(struct perf_top *top)
{
	char msg[512];
	struct perf_evsel *counter;
	struct perf_evlist *evlist = top->evlist;
	struct record_opts *opts = &top->record_opts;

	perf_evlist__config(evlist, opts);

	evlist__for_each(evlist, counter) {
try_again:
		if (perf_evsel__open(counter, top->evlist->cpus,
				     top->evlist->threads) < 0) {
			if (perf_evsel__fallback(counter, errno, msg, sizeof(msg))) {
				if (verbose)
					ui__warning("%s\n", msg);
				goto try_again;
			}

			perf_evsel__open_strerror(counter, &opts->target,
						  errno, msg, sizeof(msg));
			ui__error("%s\n", msg);
>>>>>>> refs/remotes/origin/master
			goto out_err;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (perf_evlist__mmap(evlist, mmap_pages, false) < 0) {
=======
	if (perf_evlist__mmap(evlist, top->mmap_pages, false) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		ui__warning("Failed to mmap with %d (%s)\n",
=======
	if (perf_evlist__mmap(evlist, opts->mmap_pages, false) < 0) {
		ui__error("Failed to mmap with %d (%s)\n",
>>>>>>> refs/remotes/origin/master
			    errno, strerror(errno));
		goto out_err;
	}

<<<<<<< HEAD
	return;

out_err:
	exit_browser(0);
	exit(0);
}

<<<<<<< HEAD
static int __cmd_top(void)
{
	pthread_t thread;
	int ret __used;
=======
static int perf_top__setup_sample_type(struct perf_top *top)
{
	if (!top->sort_has_symbols) {
		if (symbol_conf.use_callchain) {
			ui__warning("Selected -g but \"sym\" not present in --sort/-s.");
			return -EINVAL;
		}
	} else if (!top->dont_use_callchains && callchain_param.mode != CHAIN_NONE) {
		if (callchain_register_param(&callchain_param) < 0) {
			ui__warning("Can't register callchain params.\n");
=======
	return 0;

out_err:
	return -1;
}

static int perf_top__setup_sample_type(struct perf_top *top __maybe_unused)
{
	if (!sort__has_sym) {
		if (symbol_conf.use_callchain) {
			ui__error("Selected -g but \"sym\" not present in --sort/-s.");
			return -EINVAL;
		}
	} else if (callchain_param.mode != CHAIN_NONE) {
		if (callchain_register_param(&callchain_param) < 0) {
			ui__error("Can't register callchain params.\n");
>>>>>>> refs/remotes/origin/master
			return -EINVAL;
		}
	}

	return 0;
}

static int __cmd_top(struct perf_top *top)
{
<<<<<<< HEAD
	pthread_t thread;
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * FIXME: perf_session__new should allow passing a O_MMAP, so that all this
	 * mmap reading, etc is encapsulated in it. Use O_WRONLY for now.
	 */
<<<<<<< HEAD
	struct perf_session *session = perf_session__new(NULL, O_WRONLY, false, false, NULL);
	if (session == NULL)
		return -ENOMEM;

	if (top.target_tid != -1)
		perf_event__synthesize_thread_map(top.evlist->threads,
						  perf_event__process, session);
	else
		perf_event__synthesize_threads(perf_event__process, session);

	start_counters(top.evlist);
	session->evlist = top.evlist;
	perf_session__update_sample_type(session);

	/* Wait for a minimal set of events before starting the snapshot */
	poll(top.evlist->pollfd, top.evlist->nr_fds, 100);

	perf_session__mmap_read(session);

	if (pthread_create(&thread, NULL, (use_browser > 0 ? display_thread_tui :
							     display_thread), session)) {
=======
	top->session = perf_session__new(NULL, O_WRONLY, false, false, NULL);
	if (top->session == NULL)
		return -ENOMEM;

=======
	struct record_opts *opts = &top->record_opts;
	pthread_t thread;
	int ret;

	top->session = perf_session__new(NULL, false, NULL);
	if (top->session == NULL)
		return -ENOMEM;

	machines__set_symbol_filter(&top->session->machines, symbol_filter);

	if (!objdump_path) {
		ret = perf_session_env__lookup_objdump(&top->session->header.env);
		if (ret)
			goto out_delete;
	}

>>>>>>> refs/remotes/origin/master
	ret = perf_top__setup_sample_type(top);
	if (ret)
		goto out_delete;

<<<<<<< HEAD
	if (top->target_tid || top->uid != UINT_MAX)
		perf_event__synthesize_thread_map(&top->tool, top->evlist->threads,
						  perf_event__process,
						  &top->session->host_machine);
	else
		perf_event__synthesize_threads(&top->tool, perf_event__process,
					       &top->session->host_machine);
	perf_top__start_counters(top);
	top->session->evlist = top->evlist;
	perf_session__update_sample_type(top->session);
=======
	machine__synthesize_threads(&top->session->machines.host, &opts->target,
				    top->evlist->threads, false);
	ret = perf_top__start_counters(top);
	if (ret)
		goto out_delete;

	top->session->evlist = top->evlist;
	perf_session__set_id_hdr_size(top->session);

	/*
	 * When perf is starting the traced process, all the events (apart from
	 * group members) have enable_on_exec=1 set, so don't spoil it by
	 * prematurely enabling them.
	 *
	 * XXX 'top' still doesn't start workloads like record, trace, but should,
	 * so leave the check here.
	 */
        if (!target__none(&opts->target))
                perf_evlist__enable(top->evlist);
>>>>>>> refs/remotes/origin/master

	/* Wait for a minimal set of events before starting the snapshot */
	poll(top->evlist->pollfd, top->evlist->nr_fds, 100);

	perf_top__mmap_read(top);

<<<<<<< HEAD
	if (pthread_create(&thread, NULL, (use_browser > 0 ? display_thread_tui :
							    display_thread), top)) {
>>>>>>> refs/remotes/origin/cm-10.0
		printf("Could not create display thread.\n");
		exit(-1);
	}

<<<<<<< HEAD
	if (realtime_prio) {
		struct sched_param param;

		param.sched_priority = realtime_prio;
=======
=======
	ret = -1;
	if (pthread_create(&thread, NULL, (use_browser > 0 ? display_thread_tui :
							    display_thread), top)) {
		ui__error("Could not create display thread.\n");
		goto out_delete;
	}

>>>>>>> refs/remotes/origin/master
	if (top->realtime_prio) {
		struct sched_param param;

		param.sched_priority = top->realtime_prio;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		if (sched_setscheduler(0, SCHED_FIFO, &param)) {
			printf("Could not set realtime priority.\n");
			exit(-1);
		}
	}

	while (1) {
<<<<<<< HEAD
		u64 hits = top.samples;

		perf_session__mmap_read(session);

		if (hits == top.samples)
			ret = poll(top.evlist->pollfd, top.evlist->nr_fds, 100);
	}

=======
=======
		if (sched_setscheduler(0, SCHED_FIFO, &param)) {
			ui__error("Could not set realtime priority.\n");
			goto out_delete;
		}
	}

	while (!done) {
>>>>>>> refs/remotes/origin/master
		u64 hits = top->samples;

		perf_top__mmap_read(top);

		if (hits == top->samples)
			ret = poll(top->evlist->pollfd, top->evlist->nr_fds, 100);
	}

<<<<<<< HEAD
=======
	ret = 0;
>>>>>>> refs/remotes/origin/master
out_delete:
	perf_session__delete(top->session);
	top->session = NULL;

<<<<<<< HEAD
	return 0;
}

static int
parse_callchain_opt(const struct option *opt, const char *arg, int unset)
{
	struct perf_top *top = (struct perf_top *)opt->value;
	char *tok, *tok2;
	char *endptr;

	/*
	 * --no-call-graph
	 */
	if (unset) {
		top->dont_use_callchains = true;
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
	} else
		return -1;

	/* get the min percentage */
	tok = strtok(NULL, ",");
	if (!tok)
		goto setup;

	callchain_param.min_percent = strtod(tok, &endptr);
	if (tok == endptr)
		return -1;

	/* get the print limit */
	tok2 = strtok(NULL, ",");
	if (!tok2)
		goto setup;

	if (tok2[0] != 'c') {
		callchain_param.print_limit = strtod(tok2, &endptr);
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
setup:
	if (callchain_register_param(&callchain_param) < 0) {
		fprintf(stderr, "Can't register callchain params\n");
		return -1;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static const char * const top_usage[] = {
	"perf top [<options>]",
	NULL
};

<<<<<<< HEAD
static const struct option options[] = {
	OPT_CALLBACK('e', "event", &top.evlist, "event",
		     "event selector. use 'perf list' to list available events",
		     parse_events),
	OPT_INTEGER('c', "count", &default_interval,
		    "event period to sample"),
	OPT_INTEGER('p', "pid", &top.target_pid,
		    "profile events on existing process id"),
	OPT_INTEGER('t', "tid", &top.target_tid,
		    "profile events on existing thread id"),
	OPT_BOOLEAN('a', "all-cpus", &system_wide,
=======
int cmd_top(int argc, const char **argv, const char *prefix __used)
{
	struct perf_evsel *pos;
	int status = -ENOMEM;
	struct perf_top top = {
		.count_filter	     = 5,
		.delay_secs	     = 2,
		.uid		     = UINT_MAX,
		.freq		     = 1000, /* 1 KHz */
		.mmap_pages	     = 128,
		.sym_pcnt_filter     = 5,
	};
	char callchain_default_opt[] = "fractal,0.5,callee";
=======
	return ret;
}

static int
callchain_opt(const struct option *opt, const char *arg, int unset)
{
	symbol_conf.use_callchain = true;
	return record_callchain_opt(opt, arg, unset);
}

static int
parse_callchain_opt(const struct option *opt, const char *arg, int unset)
{
	symbol_conf.use_callchain = true;
	return record_parse_callchain_opt(opt, arg, unset);
}

static int
parse_percent_limit(const struct option *opt, const char *arg,
		    int unset __maybe_unused)
{
	struct perf_top *top = opt->value;

	top->min_percent = strtof(arg, NULL);
	return 0;
}

int cmd_top(int argc, const char **argv, const char *prefix __maybe_unused)
{
	int status = -1;
	char errbuf[BUFSIZ];
	struct perf_top top = {
		.count_filter	     = 5,
		.delay_secs	     = 2,
		.record_opts = {
			.mmap_pages	= UINT_MAX,
			.user_freq	= UINT_MAX,
			.user_interval	= ULLONG_MAX,
			.freq		= 4000, /* 4 KHz */
			.target		= {
				.uses_mmap   = true,
			},
		},
		.max_stack	     = PERF_MAX_STACK_DEPTH,
		.sym_pcnt_filter     = 5,
	};
	struct record_opts *opts = &top.record_opts;
	struct target *target = &opts->target;
>>>>>>> refs/remotes/origin/master
	const struct option options[] = {
	OPT_CALLBACK('e', "event", &top.evlist, "event",
		     "event selector. use 'perf list' to list available events",
		     parse_events_option),
<<<<<<< HEAD
	OPT_INTEGER('c', "count", &top.default_interval,
		    "event period to sample"),
	OPT_STRING('p', "pid", &top.target_pid, "pid",
		    "profile events on existing process id"),
	OPT_STRING('t', "tid", &top.target_tid, "tid",
		    "profile events on existing thread id"),
	OPT_BOOLEAN('a', "all-cpus", &top.system_wide,
>>>>>>> refs/remotes/origin/cm-10.0
			    "system-wide collection from all CPUs"),
	OPT_STRING('C', "cpu", &top.cpu_list, "cpu",
		    "list of cpus to monitor"),
	OPT_STRING('k', "vmlinux", &symbol_conf.vmlinux_name,
		   "file", "vmlinux pathname"),
	OPT_BOOLEAN('K', "hide_kernel_symbols", &top.hide_kernel_symbols,
		    "hide kernel symbols"),
<<<<<<< HEAD
	OPT_UINTEGER('m', "mmap-pages", &mmap_pages, "number of mmap data pages"),
	OPT_INTEGER('r', "realtime", &realtime_prio,
		    "collect data with this RT SCHED_FIFO priority"),
	OPT_INTEGER('d', "delay", &top.delay_secs,
		    "number of seconds to delay between refreshes"),
	OPT_BOOLEAN('D', "dump-symtab", &dump_symtab,
			    "dump the symbol table used for profiling"),
	OPT_INTEGER('f', "count-filter", &top.count_filter,
		    "only display functions with more events than this"),
	OPT_BOOLEAN('g', "group", &group,
			    "put the counters into a counter group"),
	OPT_BOOLEAN('i', "inherit", &inherit,
		    "child tasks inherit counters"),
	OPT_STRING('s', "sym-annotate", &sym_filter, "symbol name",
=======
	OPT_UINTEGER('m', "mmap-pages", &top.mmap_pages, "number of mmap data pages"),
=======
	OPT_U64('c', "count", &opts->user_interval, "event period to sample"),
	OPT_STRING('p', "pid", &target->pid, "pid",
		    "profile events on existing process id"),
	OPT_STRING('t', "tid", &target->tid, "tid",
		    "profile events on existing thread id"),
	OPT_BOOLEAN('a', "all-cpus", &target->system_wide,
			    "system-wide collection from all CPUs"),
	OPT_STRING('C', "cpu", &target->cpu_list, "cpu",
		    "list of cpus to monitor"),
	OPT_STRING('k', "vmlinux", &symbol_conf.vmlinux_name,
		   "file", "vmlinux pathname"),
	OPT_BOOLEAN(0, "ignore-vmlinux", &symbol_conf.ignore_vmlinux,
		    "don't load vmlinux even if found"),
	OPT_BOOLEAN('K', "hide_kernel_symbols", &top.hide_kernel_symbols,
		    "hide kernel symbols"),
	OPT_CALLBACK('m', "mmap-pages", &opts->mmap_pages, "pages",
		     "number of mmap data pages",
		     perf_evlist__parse_mmap_pages),
>>>>>>> refs/remotes/origin/master
	OPT_INTEGER('r', "realtime", &top.realtime_prio,
		    "collect data with this RT SCHED_FIFO priority"),
	OPT_INTEGER('d', "delay", &top.delay_secs,
		    "number of seconds to delay between refreshes"),
	OPT_BOOLEAN('D', "dump-symtab", &top.dump_symtab,
			    "dump the symbol table used for profiling"),
	OPT_INTEGER('f', "count-filter", &top.count_filter,
		    "only display functions with more events than this"),
<<<<<<< HEAD
	OPT_BOOLEAN('g', "group", &top.group,
			    "put the counters into a counter group"),
	OPT_BOOLEAN('i', "inherit", &top.inherit,
		    "child tasks inherit counters"),
	OPT_STRING(0, "sym-annotate", &top.sym_filter, "symbol name",
>>>>>>> refs/remotes/origin/cm-10.0
		    "symbol to annotate"),
	OPT_BOOLEAN('z', "zero", &top.zero,
		    "zero history across updates"),
	OPT_INTEGER('F', "freq", &top.freq,
		    "profile at this frequency"),
=======
	OPT_BOOLEAN(0, "group", &opts->group,
			    "put the counters into a counter group"),
	OPT_BOOLEAN('i', "no-inherit", &opts->no_inherit,
		    "child tasks do not inherit counters"),
	OPT_STRING(0, "sym-annotate", &top.sym_filter, "symbol name",
		    "symbol to annotate"),
	OPT_BOOLEAN('z', "zero", &top.zero, "zero history across updates"),
	OPT_UINTEGER('F', "freq", &opts->user_freq, "profile at this frequency"),
>>>>>>> refs/remotes/origin/master
	OPT_INTEGER('E', "entries", &top.print_entries,
		    "display this many functions"),
	OPT_BOOLEAN('U', "hide_user_symbols", &top.hide_user_symbols,
		    "hide user symbols"),
<<<<<<< HEAD
<<<<<<< HEAD
	OPT_BOOLEAN(0, "tui", &use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "stdio", &use_stdio, "Use the stdio interface"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show counter open errors, etc)"),
	OPT_END()
};

int cmd_top(int argc, const char **argv, const char *prefix __used)
{
	struct perf_evsel *pos;
	int status = -ENOMEM;
=======
=======
>>>>>>> refs/remotes/origin/master
	OPT_BOOLEAN(0, "tui", &top.use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "stdio", &top.use_stdio, "Use the stdio interface"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show counter open errors, etc)"),
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
<<<<<<< HEAD
		   "sort by key(s): pid, comm, dso, symbol, parent"),
	OPT_BOOLEAN('n', "show-nr-samples", &symbol_conf.show_nr_samples,
		    "Show a column with the number of samples"),
	OPT_CALLBACK_DEFAULT('G', "call-graph", &top, "output_type,min_percent, call_order",
		     "Display callchains using output_type (graph, flat, fractal, or none), min percent threshold and callchain order. "
		     "Default: fractal,0.5,callee", &parse_callchain_opt,
		     callchain_default_opt),
=======
		   "sort by key(s): pid, comm, dso, symbol, parent, weight, local_weight,"
		   " abort, in_tx, transaction"),
	OPT_BOOLEAN('n', "show-nr-samples", &symbol_conf.show_nr_samples,
		    "Show a column with the number of samples"),
	OPT_CALLBACK_NOOPT('g', NULL, &top.record_opts,
			   NULL, "enables call-graph recording",
			   &callchain_opt),
	OPT_CALLBACK(0, "call-graph", &top.record_opts,
		     "mode[,dump_size]", record_callchain_help,
		     &parse_callchain_opt),
	OPT_INTEGER(0, "max-stack", &top.max_stack,
		    "Set the maximum stack depth when parsing the callchain. "
		    "Default: " __stringify(PERF_MAX_STACK_DEPTH)),
	OPT_CALLBACK(0, "ignore-callees", NULL, "regex",
		   "ignore callees of these functions in call graphs",
		   report_parse_ignore_callees_opt),
>>>>>>> refs/remotes/origin/master
	OPT_BOOLEAN(0, "show-total-period", &symbol_conf.show_total_period,
		    "Show a column with the sum of periods"),
	OPT_STRING(0, "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING(0, "comms", &symbol_conf.comm_list_str, "comm[,comm...]",
		   "only consider symbols in these comms"),
	OPT_STRING(0, "symbols", &symbol_conf.sym_list_str, "symbol[,symbol...]",
		   "only consider these symbols"),
	OPT_BOOLEAN(0, "source", &symbol_conf.annotate_src,
		    "Interleave source code with assembly code (default)"),
	OPT_BOOLEAN(0, "asm-raw", &symbol_conf.annotate_asm_raw,
		    "Display raw encoding of assembly instructions (default)"),
<<<<<<< HEAD
	OPT_STRING('M', "disassembler-style", &disassembler_style, "disassembler style",
		   "Specify disassembler style (e.g. -M intel for intel syntax)"),
	OPT_STRING('u', "uid", &top.uid_str, "user", "user to profile"),
	OPT_END()
	};
>>>>>>> refs/remotes/origin/cm-10.0

	top.evlist = perf_evlist__new(NULL, NULL);
	if (top.evlist == NULL)
		return -ENOMEM;

<<<<<<< HEAD
	page_size = sysconf(_SC_PAGE_SIZE);
=======
	symbol_conf.exclude_other = false;
>>>>>>> refs/remotes/origin/cm-10.0

=======
	OPT_STRING(0, "objdump", &objdump_path, "path",
		    "objdump binary to use for disassembly and annotations"),
	OPT_STRING('M', "disassembler-style", &disassembler_style, "disassembler style",
		   "Specify disassembler style (e.g. -M intel for intel syntax)"),
	OPT_STRING('u', "uid", &target->uid_str, "user", "user to profile"),
	OPT_CALLBACK(0, "percent-limit", &top, "percent",
		     "Don't show entries under that percent", parse_percent_limit),
	OPT_END()
	};
	const char * const top_usage[] = {
		"perf top [<options>]",
		NULL
	};

	top.evlist = perf_evlist__new();
	if (top.evlist == NULL)
		return -ENOMEM;

>>>>>>> refs/remotes/origin/master
	argc = parse_options(argc, argv, options, top_usage, 0);
	if (argc)
		usage_with_options(top_usage, options);

<<<<<<< HEAD
<<<<<<< HEAD
	/*
 	 * XXX For now start disabled, only using TUI if explicitely asked for.
 	 * Change that when handle_keys equivalent gets written, live annotation
 	 * done, etc.
 	 */
	use_browser = 0;

	if (use_stdio)
		use_browser = 0;
	else if (use_tui)
=======
	if (sort_order == default_sort_order)
		sort_order = "dso,symbol";

	setup_sorting(top_usage, options);
=======
	if (sort_order == default_sort_order)
		sort_order = "dso,symbol";

	if (setup_sorting() < 0) {
		parse_options_usage(top_usage, options, "s", 1);
		goto out_delete_evlist;
	}

	/* display thread wants entries to be collapsed in a different tree */
	sort__need_collapse = 1;
>>>>>>> refs/remotes/origin/master

	if (top.use_stdio)
		use_browser = 0;
	else if (top.use_tui)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		use_browser = 1;

	setup_browser(false);

<<<<<<< HEAD
<<<<<<< HEAD
	/* CPU and PID are mutually exclusive */
	if (top.target_tid > 0 && top.cpu_list) {
=======
	top.uid = parse_target_uid(top.uid_str, top.target_tid, top.target_pid);
	if (top.uid_str != NULL && top.uid == UINT_MAX - 1)
		goto out_delete_evlist;

	/* CPU and PID are mutually exclusive */
	if (top.target_tid && top.cpu_list) {
>>>>>>> refs/remotes/origin/cm-10.0
		printf("WARNING: PID switch overriding CPU\n");
		sleep(1);
		top.cpu_list = NULL;
	}

<<<<<<< HEAD
	if (top.target_pid != -1)
		top.target_tid = top.target_pid;

	if (perf_evlist__create_maps(top.evlist, top.target_pid,
				     top.target_tid, top.cpu_list) < 0)
=======
	if (top.target_pid)
		top.target_tid = top.target_pid;

	if (perf_evlist__create_maps(top.evlist, top.target_pid,
				     top.target_tid, top.uid, top.cpu_list) < 0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	status = target__validate(target);
	if (status) {
		target__strerror(target, status, errbuf, BUFSIZ);
		ui__warning("%s\n", errbuf);
	}

	status = target__parse_uid(target);
	if (status) {
		int saved_errno = errno;

		target__strerror(target, status, errbuf, BUFSIZ);
		ui__error("%s\n", errbuf);

		status = -saved_errno;
		goto out_delete_evlist;
	}

	if (target__none(target))
		target->system_wide = true;

	if (perf_evlist__create_maps(top.evlist, target) < 0)
>>>>>>> refs/remotes/origin/master
		usage_with_options(top_usage, options);

	if (!top.evlist->nr_entries &&
	    perf_evlist__add_default(top.evlist) < 0) {
<<<<<<< HEAD
		pr_err("Not enough memory for event selector list\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
=======
	symbol_conf.nr_events = top.evlist->nr_entries;

>>>>>>> refs/remotes/origin/cm-10.0
	if (top.delay_secs < 1)
		top.delay_secs = 1;

	/*
	 * User specified count overrides default frequency.
	 */
<<<<<<< HEAD
	if (default_interval)
		top.freq = 0;
	else if (top.freq) {
		default_interval = top.freq;
=======
	if (top.default_interval)
		top.freq = 0;
	else if (top.freq) {
		top.default_interval = top.freq;
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		fprintf(stderr, "frequency and count are zero, aborting\n");
		exit(EXIT_FAILURE);
	}

	list_for_each_entry(pos, &top.evlist->entries, node) {
<<<<<<< HEAD
		if (perf_evsel__alloc_fd(pos, top.evlist->cpus->nr,
					 top.evlist->threads->nr) < 0)
			goto out_free_fd;
		/*
		 * Fill in the ones not specifically initialized via -c:
		 */
		if (pos->attr.sample_period)
			continue;

		pos->attr.sample_period = default_interval;
	}

	if (perf_evlist__alloc_pollfd(top.evlist) < 0 ||
	    perf_evlist__alloc_mmap(top.evlist) < 0)
		goto out_free_fd;

	top.sym_evsel = list_entry(top.evlist->entries.next, struct perf_evsel, node);

	symbol_conf.priv_size = (sizeof(struct sym_entry) + sizeof(struct annotation) +
				 (top.evlist->nr_entries + 1) * sizeof(unsigned long));
=======
		/*
		 * Fill in the ones not specifically initialized via -c:
		 */
		if (!pos->attr.sample_period)
			pos->attr.sample_period = top.default_interval;
	}

	top.sym_evsel = list_entry(top.evlist->entries.next, struct perf_evsel, node);

	symbol_conf.priv_size = sizeof(struct annotation);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ui__error("Not enough memory for event selector list\n");
		goto out_delete_evlist;
	}

	symbol_conf.nr_events = top.evlist->nr_entries;

	if (top.delay_secs < 1)
		top.delay_secs = 1;

	if (record_opts__config(opts)) {
		status = -EINVAL;
		goto out_delete_evlist;
	}

	top.sym_evsel = perf_evlist__first(top.evlist);

	symbol_conf.priv_size = sizeof(struct annotation);
>>>>>>> refs/remotes/origin/master

	symbol_conf.try_vmlinux_path = (symbol_conf.vmlinux_name == NULL);
	if (symbol__init() < 0)
		return -1;

<<<<<<< HEAD
<<<<<<< HEAD
	get_term_dimensions(&winsize);
	if (top.print_entries == 0) {
		update_print_entries(&winsize);
		signal(SIGWINCH, sig_winch_handler);
	}

	status = __cmd_top();
out_free_fd:
=======
	sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "dso", stdout);
	sort_entry__setup_elide(&sort_comm, symbol_conf.comm_list, "comm", stdout);
	sort_entry__setup_elide(&sort_sym, symbol_conf.sym_list, "symbol", stdout);

	/*
	 * Avoid annotation data structures overhead when symbols aren't on the
	 * sort list.
	 */
	top.sort_has_symbols = sort_sym.list.next != NULL;
=======
	sort__setup_elide(stdout);
>>>>>>> refs/remotes/origin/master

	get_term_dimensions(&top.winsize);
	if (top.print_entries == 0) {
		struct sigaction act = {
			.sa_sigaction = perf_top__sig_winch,
			.sa_flags     = SA_SIGINFO,
		};
		perf_top__update_print_entries(&top);
		sigaction(SIGWINCH, &act, NULL);
	}

	status = __cmd_top(&top);

out_delete_evlist:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	perf_evlist__delete(top.evlist);

	return status;
}
