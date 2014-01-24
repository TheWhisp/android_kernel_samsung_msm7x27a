#ifndef __PERF_TOP_H
#define __PERF_TOP_H 1

<<<<<<< HEAD
#include "types.h"
#include "../perf.h"
#include <stddef.h>
#include <pthread.h>
#include <linux/list.h>
#include <linux/rbtree.h>

struct perf_evlist;
struct perf_evsel;

struct sym_entry {
	struct rb_node		rb_node;
	struct list_head	node;
	unsigned long		snap_count;
	double			weight;
	struct map		*map;
	unsigned long		count[0];
};

static inline struct symbol *sym_entry__symbol(struct sym_entry *self)
{
       return ((void *)self) + symbol_conf.priv_size;
}

struct perf_top {
=======
#include "tool.h"
#include "types.h"
#include <stddef.h>
#include <stdbool.h>

struct perf_evlist;
struct perf_evsel;
struct perf_session;

struct perf_top {
	struct perf_tool   tool;
>>>>>>> refs/remotes/origin/cm-10.0
	struct perf_evlist *evlist;
	/*
	 * Symbols will be added here in perf_event__process_sample and will
	 * get out after decayed.
	 */
<<<<<<< HEAD
	struct list_head   active_symbols;
	pthread_mutex_t	   active_symbols_lock;
	pthread_cond_t	   active_symbols_cond;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	u64		   samples;
	u64		   kernel_samples, us_samples;
	u64		   exact_samples;
	u64		   guest_us_samples, guest_kernel_samples;
	int		   print_entries, count_filter, delay_secs;
<<<<<<< HEAD
	int		   display_weighted, freq, rb_entries;
	pid_t		   target_pid, target_tid;
	bool		   hide_kernel_symbols, hide_user_symbols, zero;
	const char	   *cpu_list;
	struct sym_entry   *sym_filter_entry;
	struct perf_evsel  *sym_evsel;
=======
	int		   freq;
	const char	   *target_pid, *target_tid;
	uid_t		   uid;
	bool		   hide_kernel_symbols, hide_user_symbols, zero;
	bool		   system_wide;
	bool		   use_tui, use_stdio;
	bool		   sort_has_symbols;
	bool		   dont_use_callchains;
	bool		   kptr_restrict_warned;
	bool		   vmlinux_warned;
	bool		   inherit;
	bool		   group;
	bool		   sample_id_all_missing;
	bool		   exclude_guest_missing;
	bool		   dump_symtab;
	const char	   *cpu_list;
	struct hist_entry  *sym_filter_entry;
	struct perf_evsel  *sym_evsel;
	struct perf_session *session;
	struct winsize	   winsize;
	unsigned int	   mmap_pages;
	int		   default_interval;
	int		   realtime_prio;
	int		   sym_pcnt_filter;
	const char	   *sym_filter;
	const char	   *uid_str;
>>>>>>> refs/remotes/origin/cm-10.0
};

size_t perf_top__header_snprintf(struct perf_top *top, char *bf, size_t size);
void perf_top__reset_sample_counters(struct perf_top *top);
<<<<<<< HEAD
float perf_top__decay_samples(struct perf_top *top, struct rb_root *root);
void perf_top__find_widths(struct perf_top *top, struct rb_root *root,
			   int *dso_width, int *dso_short_width, int *sym_width);

#ifdef NO_NEWT_SUPPORT
static inline int perf_top__tui_browser(struct perf_top *top __used)
{
	return 0;
}
#else
int perf_top__tui_browser(struct perf_top *top);
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
#endif /* __PERF_TOP_H */
