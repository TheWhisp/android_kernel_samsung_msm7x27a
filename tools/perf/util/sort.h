#ifndef __PERF_SORT_H
#define __PERF_SORT_H
#include "../builtin.h"

#include "util.h"

#include "color.h"
#include <linux/list.h>
#include "cache.h"
#include <linux/rbtree.h>
#include "symbol.h"
#include "string.h"
#include "callchain.h"
#include "strlist.h"
#include "values.h"

#include "../perf.h"
#include "debug.h"
#include "header.h"

#include "parse-options.h"
#include "parse-events.h"

#include "thread.h"
<<<<<<< HEAD
#include "sort.h"
=======
>>>>>>> refs/remotes/origin/master

extern regex_t parent_regex;
extern const char *sort_order;
extern const char default_parent_pattern[];
extern const char *parent_pattern;
extern const char default_sort_order[];
<<<<<<< HEAD
extern int sort__need_collapse;
extern int sort__has_parent;
<<<<<<< HEAD
=======
extern int sort__branch_mode;
>>>>>>> refs/remotes/origin/cm-10.0
extern char *field_sep;
=======
extern regex_t ignore_callees_regex;
extern int have_ignore_callees;
extern int sort__need_collapse;
extern int sort__has_parent;
extern int sort__has_sym;
extern enum sort_mode sort__mode;
>>>>>>> refs/remotes/origin/master
extern struct sort_entry sort_comm;
extern struct sort_entry sort_dso;
extern struct sort_entry sort_sym;
extern struct sort_entry sort_parent;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
extern struct sort_entry sort_dso_from;
extern struct sort_entry sort_dso_to;
extern struct sort_entry sort_sym_from;
extern struct sort_entry sort_sym_to;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
extern enum sort_type sort__first_dimension;

=======
extern enum sort_type sort__first_dimension;

struct he_stat {
	u64			period;
	u64			period_sys;
	u64			period_us;
	u64			period_guest_sys;
	u64			period_guest_us;
	u64			weight;
	u32			nr_events;
};

struct hist_entry_diff {
	bool	computed;

	/* PERF_HPP__DELTA */
	double	period_ratio_delta;

	/* PERF_HPP__RATIO */
	double	period_ratio;

	/* HISTC_WEIGHTED_DIFF */
	s64	wdiff;
};

>>>>>>> refs/remotes/origin/master
/**
 * struct hist_entry - histogram entry
 *
 * @row_offset - offset from the first callchain expanded to appear on screen
 * @nr_rows - rows expanded in callchain, recalculated on folding/unfolding
 */
struct hist_entry {
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct rb_node		rb_node_in;
>>>>>>> refs/remotes/origin/cm-10.0
	struct rb_node		rb_node;
	u64			period;
	u64			period_sys;
	u64			period_us;
	u64			period_guest_sys;
	u64			period_guest_us;
	struct map_symbol	ms;
	struct thread		*thread;
	u64			ip;
	s32			cpu;
	u32			nr_events;
=======
	struct rb_node		rb_node_in;
	struct rb_node		rb_node;
	union {
		struct list_head node;
		struct list_head head;
	} pairs;
	struct he_stat		stat;
	struct map_symbol	ms;
	struct thread		*thread;
	struct comm		*comm;
	u64			ip;
	u64			transaction;
	s32			cpu;

	struct hist_entry_diff	diff;

	/* We are added by hists__add_dummy_entry. */
	bool			dummy;
>>>>>>> refs/remotes/origin/master

	/* XXX These two should move to some tree widget lib */
	u16			row_offset;
	u16			nr_rows;

	bool			init_have_children;
	char			level;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool			used;
>>>>>>> refs/remotes/origin/cm-10.0
	u8			filtered;
	struct symbol		*parent;
	union {
		unsigned long	  position;
		struct hist_entry *pair;
		struct rb_root	  sorted_chain;
	};
<<<<<<< HEAD
=======
	struct branch_info	*branch_info;
>>>>>>> refs/remotes/origin/cm-10.0
	struct callchain_root	callchain[0];
};

enum sort_type {
=======
	bool			used;
	u8			filtered;
	char			*srcline;
	struct symbol		*parent;
	unsigned long		position;
	struct rb_root		sorted_chain;
	struct branch_info	*branch_info;
	struct hists		*hists;
	struct mem_info		*mem_info;
	struct callchain_root	callchain[0]; /* must be last member */
};

static inline bool hist_entry__has_pairs(struct hist_entry *he)
{
	return !list_empty(&he->pairs.node);
}

static inline struct hist_entry *hist_entry__next_pair(struct hist_entry *he)
{
	if (hist_entry__has_pairs(he))
		return list_entry(he->pairs.node.next, struct hist_entry, pairs.node);
	return NULL;
}

static inline void hist_entry__add_pair(struct hist_entry *pair,
					struct hist_entry *he)
{
	list_add_tail(&pair->pairs.node, &he->pairs.head);
}

enum sort_mode {
	SORT_MODE__NORMAL,
	SORT_MODE__BRANCH,
	SORT_MODE__MEMORY,
};

enum sort_type {
	/* common sort keys */
>>>>>>> refs/remotes/origin/master
	SORT_PID,
	SORT_COMM,
	SORT_DSO,
	SORT_SYM,
	SORT_PARENT,
	SORT_CPU,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	SORT_DSO_FROM,
=======
	SORT_SRCLINE,
	SORT_LOCAL_WEIGHT,
	SORT_GLOBAL_WEIGHT,
	SORT_TRANSACTION,

	/* branch stack specific sort keys */
	__SORT_BRANCH_STACK,
	SORT_DSO_FROM = __SORT_BRANCH_STACK,
>>>>>>> refs/remotes/origin/master
	SORT_DSO_TO,
	SORT_SYM_FROM,
	SORT_SYM_TO,
	SORT_MISPREDICT,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	SORT_ABORT,
	SORT_IN_TX,

	/* memory mode specific sort keys */
	__SORT_MEMORY_MODE,
	SORT_MEM_DADDR_SYMBOL = __SORT_MEMORY_MODE,
	SORT_MEM_DADDR_DSO,
	SORT_MEM_LOCKED,
	SORT_MEM_TLB,
	SORT_MEM_LVL,
	SORT_MEM_SNOOP,
>>>>>>> refs/remotes/origin/master
};

/*
 * configurable sorting bits
 */

struct sort_entry {
	struct list_head list;

	const char *se_header;

	int64_t (*se_cmp)(struct hist_entry *, struct hist_entry *);
	int64_t (*se_collapse)(struct hist_entry *, struct hist_entry *);
<<<<<<< HEAD
	int	(*se_snprintf)(struct hist_entry *self, char *bf, size_t size,
=======
	int	(*se_snprintf)(struct hist_entry *he, char *bf, size_t size,
>>>>>>> refs/remotes/origin/master
			       unsigned int width);
	u8	se_width_idx;
	bool	elide;
};

extern struct sort_entry sort_thread;
extern struct list_head hist_entry__sort_list;

<<<<<<< HEAD
void setup_sorting(const char * const usagestr[], const struct option *opts);
<<<<<<< HEAD

extern size_t sort__thread_print(FILE *, struct hist_entry *, unsigned int);
extern size_t sort__comm_print(FILE *, struct hist_entry *, unsigned int);
extern size_t sort__dso_print(FILE *, struct hist_entry *, unsigned int);
extern size_t sort__sym_print(FILE *, struct hist_entry *, unsigned int __used);
extern int64_t cmp_null(void *, void *);
extern int64_t sort__thread_cmp(struct hist_entry *, struct hist_entry *);
extern int64_t sort__comm_cmp(struct hist_entry *, struct hist_entry *);
extern int64_t sort__comm_collapse(struct hist_entry *, struct hist_entry *);
extern int64_t sort__dso_cmp(struct hist_entry *, struct hist_entry *);
extern int64_t sort__sym_cmp(struct hist_entry *, struct hist_entry *);
extern int64_t sort__parent_cmp(struct hist_entry *, struct hist_entry *);
int64_t sort__cpu_cmp(struct hist_entry *left, struct hist_entry *right);
extern size_t sort__parent_print(FILE *, struct hist_entry *, unsigned int);
=======
>>>>>>> refs/remotes/origin/cm-10.0
extern int sort_dimension__add(const char *);
void sort_entry__setup_elide(struct sort_entry *self, struct strlist *list,
			     const char *list_name, FILE *fp);
=======
int setup_sorting(void);
extern int sort_dimension__add(const char *);
void sort__setup_elide(FILE *fp);

int report_parse_ignore_callees_opt(const struct option *opt, const char *arg, int unset);
>>>>>>> refs/remotes/origin/master

#endif	/* __PERF_SORT_H */
