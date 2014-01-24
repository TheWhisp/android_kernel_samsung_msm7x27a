/*
 * trace_events_filter - generic event filtering
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) 2009 Tom Zanussi <tzanussi@gmail.com>
 */

#include <linux/module.h>
#include <linux/ctype.h>
#include <linux/mutex.h>
#include <linux/perf_event.h>
#include <linux/slab.h>

#include "trace.h"
#include "trace_output.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#define DEFAULT_SYS_FILTER_MESSAGE					\
	"### global filter ###\n"					\
	"# Use this to set filters for multiple events.\n"		\
	"# Only events with the given fields will be affected.\n"	\
	"# If no events are modified, an error message will be displayed here"

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
enum filter_op_ids
{
	OP_OR,
	OP_AND,
	OP_GLOB,
	OP_NE,
	OP_EQ,
	OP_LT,
	OP_LE,
	OP_GT,
	OP_GE,
<<<<<<< HEAD
=======
	OP_BAND,
>>>>>>> refs/remotes/origin/master
	OP_NONE,
	OP_OPEN_PAREN,
};

struct filter_op {
	int id;
	char *string;
	int precedence;
};

<<<<<<< HEAD
=======
/* Order must be the same as enum filter_op_ids above */
>>>>>>> refs/remotes/origin/master
static struct filter_op filter_ops[] = {
	{ OP_OR,	"||",		1 },
	{ OP_AND,	"&&",		2 },
	{ OP_GLOB,	"~",		4 },
	{ OP_NE,	"!=",		4 },
	{ OP_EQ,	"==",		4 },
	{ OP_LT,	"<",		5 },
	{ OP_LE,	"<=",		5 },
	{ OP_GT,	">",		5 },
	{ OP_GE,	">=",		5 },
<<<<<<< HEAD
=======
	{ OP_BAND,	"&",		6 },
>>>>>>> refs/remotes/origin/master
	{ OP_NONE,	"OP_NONE",	0 },
	{ OP_OPEN_PAREN, "(",		0 },
};

enum {
	FILT_ERR_NONE,
	FILT_ERR_INVALID_OP,
	FILT_ERR_UNBALANCED_PAREN,
	FILT_ERR_TOO_MANY_OPERANDS,
	FILT_ERR_OPERAND_TOO_LONG,
	FILT_ERR_FIELD_NOT_FOUND,
	FILT_ERR_ILLEGAL_FIELD_OP,
	FILT_ERR_ILLEGAL_INTVAL,
	FILT_ERR_BAD_SUBSYS_FILTER,
	FILT_ERR_TOO_MANY_PREDS,
	FILT_ERR_MISSING_FIELD,
	FILT_ERR_INVALID_FILTER,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	FILT_ERR_IP_FIELD_ONLY,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	FILT_ERR_IP_FIELD_ONLY,
>>>>>>> refs/remotes/origin/master
};

static char *err_text[] = {
	"No error",
	"Invalid operator",
	"Unbalanced parens",
	"Too many operands",
	"Operand too long",
	"Field not found",
	"Illegal operation for field type",
	"Illegal integer value",
	"Couldn't find or set field in one of a subsystem's events",
	"Too many terms in predicate expression",
	"Missing field name and/or value",
	"Meaningless filter expression",
<<<<<<< HEAD
<<<<<<< HEAD
=======
	"Only 'ip' field is supported for function trace",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	"Only 'ip' field is supported for function trace",
>>>>>>> refs/remotes/origin/master
};

struct opstack_op {
	int op;
	struct list_head list;
};

struct postfix_elt {
	int op;
	char *operand;
	struct list_head list;
};

struct filter_parse_state {
	struct filter_op *ops;
	struct list_head opstack;
	struct list_head postfix;
	int lasterr;
	int lasterr_pos;

	struct {
		char *string;
		unsigned int cnt;
		unsigned int tail;
	} infix;

	struct {
		char string[MAX_FILTER_STR_VAL];
		int pos;
		unsigned int tail;
	} operand;
};

struct pred_stack {
	struct filter_pred	**preds;
	int			index;
};

#define DEFINE_COMPARISON_PRED(type)					\
static int filter_pred_##type(struct filter_pred *pred, void *event)	\
{									\
	type *addr = (type *)(event + pred->offset);			\
	type val = (type)pred->val;					\
	int match = 0;							\
									\
	switch (pred->op) {						\
	case OP_LT:							\
		match = (*addr < val);					\
		break;							\
	case OP_LE:							\
		match = (*addr <= val);					\
		break;							\
	case OP_GT:							\
		match = (*addr > val);					\
		break;							\
	case OP_GE:							\
		match = (*addr >= val);					\
		break;							\
<<<<<<< HEAD
=======
	case OP_BAND:							\
		match = (*addr & val);					\
		break;							\
>>>>>>> refs/remotes/origin/master
	default:							\
		break;							\
	}								\
									\
	return match;							\
}

#define DEFINE_EQUALITY_PRED(size)					\
static int filter_pred_##size(struct filter_pred *pred, void *event)	\
{									\
	u##size *addr = (u##size *)(event + pred->offset);		\
	u##size val = (u##size)pred->val;				\
	int match;							\
									\
	match = (val == *addr) ^ pred->not;				\
									\
	return match;							\
}

DEFINE_COMPARISON_PRED(s64);
DEFINE_COMPARISON_PRED(u64);
DEFINE_COMPARISON_PRED(s32);
DEFINE_COMPARISON_PRED(u32);
DEFINE_COMPARISON_PRED(s16);
DEFINE_COMPARISON_PRED(u16);
DEFINE_COMPARISON_PRED(s8);
DEFINE_COMPARISON_PRED(u8);

DEFINE_EQUALITY_PRED(64);
DEFINE_EQUALITY_PRED(32);
DEFINE_EQUALITY_PRED(16);
DEFINE_EQUALITY_PRED(8);

/* Filter predicate for fixed sized arrays of characters */
static int filter_pred_string(struct filter_pred *pred, void *event)
{
	char *addr = (char *)(event + pred->offset);
	int cmp, match;

	cmp = pred->regex.match(addr, &pred->regex, pred->regex.field_len);

	match = cmp ^ pred->not;

	return match;
}

/* Filter predicate for char * pointers */
static int filter_pred_pchar(struct filter_pred *pred, void *event)
{
	char **addr = (char **)(event + pred->offset);
	int cmp, match;
	int len = strlen(*addr) + 1;	/* including tailing '\0' */

	cmp = pred->regex.match(*addr, &pred->regex, len);

	match = cmp ^ pred->not;

	return match;
}

/*
 * Filter predicate for dynamic sized arrays of characters.
 * These are implemented through a list of strings at the end
 * of the entry.
 * Also each of these strings have a field in the entry which
 * contains its offset from the beginning of the entry.
 * We have then first to get this field, dereference it
 * and add it to the address of the entry, and at last we have
 * the address of the string.
 */
static int filter_pred_strloc(struct filter_pred *pred, void *event)
{
	u32 str_item = *(u32 *)(event + pred->offset);
	int str_loc = str_item & 0xffff;
	int str_len = str_item >> 16;
	char *addr = (char *)(event + str_loc);
	int cmp, match;

	cmp = pred->regex.match(addr, &pred->regex, str_len);

	match = cmp ^ pred->not;

	return match;
}

static int filter_pred_none(struct filter_pred *pred, void *event)
{
	return 0;
}

/*
 * regex_match_foo - Basic regex callbacks
 *
 * @str: the string to be searched
 * @r:   the regex structure containing the pattern string
 * @len: the length of the string to be searched (including '\0')
 *
 * Note:
 * - @str might not be NULL-terminated if it's of type DYN_STRING
 *   or STATIC_STRING
 */

static int regex_match_full(char *str, struct regex *r, int len)
{
	if (strncmp(str, r->pattern, len) == 0)
		return 1;
	return 0;
}

static int regex_match_front(char *str, struct regex *r, int len)
{
	if (strncmp(str, r->pattern, r->len) == 0)
		return 1;
	return 0;
}

static int regex_match_middle(char *str, struct regex *r, int len)
{
	if (strnstr(str, r->pattern, len))
		return 1;
	return 0;
}

static int regex_match_end(char *str, struct regex *r, int len)
{
	int strlen = len - 1;

	if (strlen >= r->len &&
	    memcmp(str + strlen - r->len, r->pattern, r->len) == 0)
		return 1;
	return 0;
}

/**
 * filter_parse_regex - parse a basic regex
 * @buff:   the raw regex
 * @len:    length of the regex
 * @search: will point to the beginning of the string to compare
 * @not:    tell whether the match will have to be inverted
 *
 * This passes in a buffer containing a regex and this function will
 * set search to point to the search part of the buffer and
 * return the type of search it is (see enum above).
 * This does modify buff.
 *
 * Returns enum type.
 *  search returns the pointer to use for comparison.
 *  not returns 1 if buff started with a '!'
 *     0 otherwise.
 */
enum regex_type filter_parse_regex(char *buff, int len, char **search, int *not)
{
	int type = MATCH_FULL;
	int i;

	if (buff[0] == '!') {
		*not = 1;
		buff++;
		len--;
	} else
		*not = 0;

	*search = buff;

	for (i = 0; i < len; i++) {
		if (buff[i] == '*') {
			if (!i) {
				*search = buff + 1;
				type = MATCH_END_ONLY;
			} else {
				if (type == MATCH_END_ONLY)
					type = MATCH_MIDDLE_ONLY;
				else
					type = MATCH_FRONT_ONLY;
				buff[i] = 0;
				break;
			}
		}
	}

	return type;
}

static void filter_build_regex(struct filter_pred *pred)
{
	struct regex *r = &pred->regex;
	char *search;
	enum regex_type type = MATCH_FULL;
	int not = 0;

	if (pred->op == OP_GLOB) {
		type = filter_parse_regex(r->pattern, r->len, &search, &not);
		r->len = strlen(search);
		memmove(r->pattern, search, r->len+1);
	}

	switch (type) {
	case MATCH_FULL:
		r->match = regex_match_full;
		break;
	case MATCH_FRONT_ONLY:
		r->match = regex_match_front;
		break;
	case MATCH_MIDDLE_ONLY:
		r->match = regex_match_middle;
		break;
	case MATCH_END_ONLY:
		r->match = regex_match_end;
		break;
	}

	pred->not ^= not;
}

enum move_type {
	MOVE_DOWN,
	MOVE_UP_FROM_LEFT,
	MOVE_UP_FROM_RIGHT
};

static struct filter_pred *
get_pred_parent(struct filter_pred *pred, struct filter_pred *preds,
		int index, enum move_type *move)
{
	if (pred->parent & FILTER_PRED_IS_RIGHT)
		*move = MOVE_UP_FROM_RIGHT;
	else
		*move = MOVE_UP_FROM_LEFT;
	pred = &preds[pred->parent & ~FILTER_PRED_IS_RIGHT];

	return pred;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
enum walk_return {
	WALK_PRED_ABORT,
	WALK_PRED_PARENT,
	WALK_PRED_DEFAULT,
};

typedef int (*filter_pred_walkcb_t) (enum move_type move,
				     struct filter_pred *pred,
				     int *err, void *data);

static int walk_pred_tree(struct filter_pred *preds,
			  struct filter_pred *root,
			  filter_pred_walkcb_t cb, void *data)
{
	struct filter_pred *pred = root;
	enum move_type move = MOVE_DOWN;
	int done = 0;

	if  (!preds)
		return -EINVAL;

	do {
		int err = 0, ret;

		ret = cb(move, pred, &err, data);
		if (ret == WALK_PRED_ABORT)
			return err;
		if (ret == WALK_PRED_PARENT)
			goto get_parent;

		switch (move) {
		case MOVE_DOWN:
			if (pred->left != FILTER_PRED_INVALID) {
				pred = &preds[pred->left];
				continue;
			}
			goto get_parent;
		case MOVE_UP_FROM_LEFT:
			pred = &preds[pred->right];
			move = MOVE_DOWN;
			continue;
		case MOVE_UP_FROM_RIGHT:
 get_parent:
			if (pred == root)
				break;
			pred = get_pred_parent(pred, preds,
					       pred->parent,
					       &move);
			continue;
		}
		done = 1;
	} while (!done);

	/* We are fine. */
	return 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * A series of AND or ORs where found together. Instead of
 * climbing up and down the tree branches, an array of the
 * ops were made in order of checks. We can just move across
 * the array and short circuit if needed.
 */
static int process_ops(struct filter_pred *preds,
		       struct filter_pred *op, void *rec)
{
	struct filter_pred *pred;
	int match = 0;
	int type;
	int i;

	/*
	 * Micro-optimization: We set type to true if op
	 * is an OR and false otherwise (AND). Then we
	 * just need to test if the match is equal to
	 * the type, and if it is, we can short circuit the
	 * rest of the checks:
	 *
	 * if ((match && op->op == OP_OR) ||
	 *     (!match && op->op == OP_AND))
	 *	  return match;
	 */
	type = op->op == OP_OR;

	for (i = 0; i < op->val; i++) {
		pred = &preds[op->ops[i]];
<<<<<<< HEAD
<<<<<<< HEAD
		match = pred->fn(pred, rec);
=======
		if (!WARN_ON_ONCE(!pred->fn))
			match = pred->fn(pred, rec);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!WARN_ON_ONCE(!pred->fn))
			match = pred->fn(pred, rec);
>>>>>>> refs/remotes/origin/master
		if (!!match == type)
			return match;
	}
	return match;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* return 1 if event matches, 0 otherwise (discard) */
int filter_match_preds(struct event_filter *filter, void *rec)
{
	int match = -1;
	enum move_type move = MOVE_DOWN;
	struct filter_pred *preds;
	struct filter_pred *pred;
	struct filter_pred *root;
	int n_preds;
	int done = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
struct filter_match_preds_data {
	struct filter_pred *preds;
	int match;
	void *rec;
};

static int filter_match_preds_cb(enum move_type move, struct filter_pred *pred,
				 int *err, void *data)
{
	struct filter_match_preds_data *d = data;

	*err = 0;
	switch (move) {
	case MOVE_DOWN:
		/* only AND and OR have children */
		if (pred->left != FILTER_PRED_INVALID) {
			/* If ops is set, then it was folded. */
			if (!pred->ops)
				return WALK_PRED_DEFAULT;
			/* We can treat folded ops as a leaf node */
			d->match = process_ops(d->preds, pred, d->rec);
		} else {
			if (!WARN_ON_ONCE(!pred->fn))
				d->match = pred->fn(pred, d->rec);
		}

		return WALK_PRED_PARENT;
	case MOVE_UP_FROM_LEFT:
		/*
		 * Check for short circuits.
		 *
		 * Optimization: !!match == (pred->op == OP_OR)
		 *   is the same as:
		 * if ((match && pred->op == OP_OR) ||
		 *     (!match && pred->op == OP_AND))
		 */
		if (!!d->match == (pred->op == OP_OR))
			return WALK_PRED_PARENT;
		break;
	case MOVE_UP_FROM_RIGHT:
		break;
	}

	return WALK_PRED_DEFAULT;
}

/* return 1 if event matches, 0 otherwise (discard) */
int filter_match_preds(struct event_filter *filter, void *rec)
{
	struct filter_pred *preds;
	struct filter_pred *root;
	struct filter_match_preds_data data = {
		/* match is currently meaningless */
		.match = -1,
		.rec   = rec,
	};
	int n_preds, ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* no filter is considered a match */
	if (!filter)
		return 1;

	n_preds = filter->n_preds;
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!n_preds)
		return 1;

	/*
	 * n_preds, root and filter->preds are protect with preemption disabled.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	preds = rcu_dereference_sched(filter->preds);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	root = rcu_dereference_sched(filter->root);
	if (!root)
		return 1;

<<<<<<< HEAD
<<<<<<< HEAD
	pred = root;

	/* match is currently meaningless */
	match = -1;

	do {
		switch (move) {
		case MOVE_DOWN:
			/* only AND and OR have children */
			if (pred->left != FILTER_PRED_INVALID) {
				/* If ops is set, then it was folded. */
				if (!pred->ops) {
					/* keep going to down the left side */
					pred = &preds[pred->left];
					continue;
				}
				/* We can treat folded ops as a leaf node */
				match = process_ops(preds, pred, rec);
			} else
				match = pred->fn(pred, rec);
			/* If this pred is the only pred */
			if (pred == root)
				break;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		case MOVE_UP_FROM_LEFT:
			/*
			 * Check for short circuits.
			 *
			 * Optimization: !!match == (pred->op == OP_OR)
			 *   is the same as:
			 * if ((match && pred->op == OP_OR) ||
			 *     (!match && pred->op == OP_AND))
			 */
			if (!!match == (pred->op == OP_OR)) {
				if (pred == root)
					break;
				pred = get_pred_parent(pred, preds,
						       pred->parent, &move);
				continue;
			}
			/* now go down the right side of the tree. */
			pred = &preds[pred->right];
			move = MOVE_DOWN;
			continue;
		case MOVE_UP_FROM_RIGHT:
			/* We finished this equation. */
			if (pred == root)
				break;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		}
		done = 1;
	} while (!done);

	return match;
=======
=======
>>>>>>> refs/remotes/origin/master
	data.preds = preds = rcu_dereference_sched(filter->preds);
	ret = walk_pred_tree(preds, root, filter_match_preds_cb, &data);
	WARN_ON(ret);
	return data.match;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(filter_match_preds);

static void parse_error(struct filter_parse_state *ps, int err, int pos)
{
	ps->lasterr = err;
	ps->lasterr_pos = pos;
}

static void remove_filter_string(struct event_filter *filter)
{
	if (!filter)
		return;

	kfree(filter->filter_string);
	filter->filter_string = NULL;
}

static int replace_filter_string(struct event_filter *filter,
				 char *filter_string)
{
	kfree(filter->filter_string);
	filter->filter_string = kstrdup(filter_string, GFP_KERNEL);
	if (!filter->filter_string)
		return -ENOMEM;

	return 0;
}

static int append_filter_string(struct event_filter *filter,
				char *string)
{
	int newlen;
	char *new_filter_string;

	BUG_ON(!filter->filter_string);
	newlen = strlen(filter->filter_string) + strlen(string) + 1;
	new_filter_string = kmalloc(newlen, GFP_KERNEL);
	if (!new_filter_string)
		return -ENOMEM;

	strcpy(new_filter_string, filter->filter_string);
	strcat(new_filter_string, string);
	kfree(filter->filter_string);
	filter->filter_string = new_filter_string;

	return 0;
}

static void append_filter_err(struct filter_parse_state *ps,
			      struct event_filter *filter)
{
	int pos = ps->lasterr_pos;
	char *buf, *pbuf;

	buf = (char *)__get_free_page(GFP_TEMPORARY);
	if (!buf)
		return;

	append_filter_string(filter, "\n");
	memset(buf, ' ', PAGE_SIZE);
	if (pos > PAGE_SIZE - 128)
		pos = 0;
	buf[pos] = '^';
	pbuf = &buf[pos] + 1;

	sprintf(pbuf, "\nparse_error: %s\n", err_text[ps->lasterr]);
	append_filter_string(filter, buf);
	free_page((unsigned long) buf);
}

<<<<<<< HEAD
void print_event_filter(struct ftrace_event_call *call, struct trace_seq *s)
{
	struct event_filter *filter;

	mutex_lock(&event_mutex);
	filter = call->filter;
	if (filter && filter->filter_string)
		trace_seq_printf(s, "%s\n", filter->filter_string);
	else
		trace_seq_printf(s, "none\n");
	mutex_unlock(&event_mutex);
=======
static inline struct event_filter *event_filter(struct ftrace_event_file *file)
{
	if (file->event_call->flags & TRACE_EVENT_FL_USE_CALL_FILTER)
		return file->event_call->filter;
	else
		return file->filter;
}

/* caller must hold event_mutex */
void print_event_filter(struct ftrace_event_file *file, struct trace_seq *s)
{
	struct event_filter *filter = event_filter(file);

	if (filter && filter->filter_string)
		trace_seq_printf(s, "%s\n", filter->filter_string);
	else
		trace_seq_puts(s, "none\n");
>>>>>>> refs/remotes/origin/master
}

void print_subsystem_event_filter(struct event_subsystem *system,
				  struct trace_seq *s)
{
	struct event_filter *filter;

	mutex_lock(&event_mutex);
	filter = system->filter;
	if (filter && filter->filter_string)
		trace_seq_printf(s, "%s\n", filter->filter_string);
	else
<<<<<<< HEAD
<<<<<<< HEAD
		trace_seq_printf(s, "none\n");
=======
		trace_seq_printf(s, DEFAULT_SYS_FILTER_MESSAGE "\n");
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&event_mutex);
}

static struct ftrace_event_field *
__find_event_field(struct list_head *head, char *name)
{
	struct ftrace_event_field *field;

	list_for_each_entry(field, head, link) {
		if (!strcmp(field->name, name))
			return field;
	}

	return NULL;
}

static struct ftrace_event_field *
find_event_field(struct ftrace_event_call *call, char *name)
{
	struct ftrace_event_field *field;
	struct list_head *head;

	field = __find_event_field(&ftrace_common_fields, name);
	if (field)
		return field;

	head = trace_get_fields(call);
	return __find_event_field(head, name);
}

<<<<<<< HEAD
static void filter_free_pred(struct filter_pred *pred)
{
	if (!pred)
		return;

	kfree(pred->field_name);
	kfree(pred);
}

static void filter_clear_pred(struct filter_pred *pred)
{
	kfree(pred->field_name);
	pred->field_name = NULL;
	pred->regex.len = 0;
}

static int __alloc_pred_stack(struct pred_stack *stack, int n_preds)
{
	stack->preds = kzalloc(sizeof(*stack->preds)*(n_preds + 1), GFP_KERNEL);
=======
static int __alloc_pred_stack(struct pred_stack *stack, int n_preds)
{
	stack->preds = kcalloc(n_preds + 1, sizeof(*stack->preds), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		trace_seq_puts(s, DEFAULT_SYS_FILTER_MESSAGE "\n");
	mutex_unlock(&event_mutex);
}

static int __alloc_pred_stack(struct pred_stack *stack, int n_preds)
{
	stack->preds = kcalloc(n_preds + 1, sizeof(*stack->preds), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!stack->preds)
		return -ENOMEM;
	stack->index = n_preds;
	return 0;
}

static void __free_pred_stack(struct pred_stack *stack)
{
	kfree(stack->preds);
	stack->index = 0;
}

static int __push_pred_stack(struct pred_stack *stack,
			     struct filter_pred *pred)
{
	int index = stack->index;

	if (WARN_ON(index == 0))
		return -ENOSPC;

	stack->preds[--index] = pred;
	stack->index = index;
	return 0;
}

static struct filter_pred *
__pop_pred_stack(struct pred_stack *stack)
{
	struct filter_pred *pred;
	int index = stack->index;

	pred = stack->preds[index++];
	if (!pred)
		return NULL;

	stack->index = index;
	return pred;
}

static int filter_set_pred(struct event_filter *filter,
			   int idx,
			   struct pred_stack *stack,
<<<<<<< HEAD
<<<<<<< HEAD
			   struct filter_pred *src,
			   filter_pred_fn_t fn)
=======
			   struct filter_pred *src)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			   struct filter_pred *src)
>>>>>>> refs/remotes/origin/master
{
	struct filter_pred *dest = &filter->preds[idx];
	struct filter_pred *left;
	struct filter_pred *right;

	*dest = *src;
<<<<<<< HEAD
<<<<<<< HEAD
	if (src->field_name) {
		dest->field_name = kstrdup(src->field_name, GFP_KERNEL);
		if (!dest->field_name)
			return -ENOMEM;
	}
	dest->fn = fn;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	dest->index = idx;

	if (dest->op == OP_OR || dest->op == OP_AND) {
		right = __pop_pred_stack(stack);
		left = __pop_pred_stack(stack);
		if (!left || !right)
			return -EINVAL;
		/*
		 * If both children can be folded
		 * and they are the same op as this op or a leaf,
		 * then this op can be folded.
		 */
		if (left->index & FILTER_PRED_FOLD &&
		    (left->op == dest->op ||
		     left->left == FILTER_PRED_INVALID) &&
		    right->index & FILTER_PRED_FOLD &&
		    (right->op == dest->op ||
		     right->left == FILTER_PRED_INVALID))
			dest->index |= FILTER_PRED_FOLD;

		dest->left = left->index & ~FILTER_PRED_FOLD;
		dest->right = right->index & ~FILTER_PRED_FOLD;
		left->parent = dest->index & ~FILTER_PRED_FOLD;
		right->parent = dest->index | FILTER_PRED_IS_RIGHT;
	} else {
		/*
		 * Make dest->left invalid to be used as a quick
		 * way to know this is a leaf node.
		 */
		dest->left = FILTER_PRED_INVALID;

		/* All leafs allow folding the parent ops. */
		dest->index |= FILTER_PRED_FOLD;
	}

	return __push_pred_stack(stack, dest);
}

static void __free_preds(struct event_filter *filter)
{
	int i;

	if (filter->preds) {
<<<<<<< HEAD
<<<<<<< HEAD
		for (i = 0; i < filter->a_preds; i++)
			kfree(filter->preds[i].field_name);
=======
		for (i = 0; i < filter->n_preds; i++)
			kfree(filter->preds[i].ops);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		for (i = 0; i < filter->n_preds; i++)
			kfree(filter->preds[i].ops);
>>>>>>> refs/remotes/origin/master
		kfree(filter->preds);
		filter->preds = NULL;
	}
	filter->a_preds = 0;
	filter->n_preds = 0;
}

<<<<<<< HEAD
static void filter_disable(struct ftrace_event_call *call)
=======
static void call_filter_disable(struct ftrace_event_call *call)
>>>>>>> refs/remotes/origin/master
{
	call->flags &= ~TRACE_EVENT_FL_FILTERED;
}

<<<<<<< HEAD
=======
static void filter_disable(struct ftrace_event_file *file)
{
	struct ftrace_event_call *call = file->event_call;

	if (call->flags & TRACE_EVENT_FL_USE_CALL_FILTER)
		call_filter_disable(call);
	else
		file->flags &= ~FTRACE_EVENT_FL_FILTERED;
}

>>>>>>> refs/remotes/origin/master
static void __free_filter(struct event_filter *filter)
{
	if (!filter)
		return;

	__free_preds(filter);
	kfree(filter->filter_string);
	kfree(filter);
}

<<<<<<< HEAD
/*
 * Called when destroying the ftrace_event_call.
 * The call is being freed, so we do not need to worry about
 * the call being currently used. This is for module code removing
 * the tracepoints from within it.
 */
void destroy_preds(struct ftrace_event_call *call)
{
	__free_filter(call->filter);
	call->filter = NULL;
=======
void destroy_call_preds(struct ftrace_event_call *call)
{
	__free_filter(call->filter);
	call->filter = NULL;
}

static void destroy_file_preds(struct ftrace_event_file *file)
{
	__free_filter(file->filter);
	file->filter = NULL;
}

/*
 * Called when destroying the ftrace_event_file.
 * The file is being freed, so we do not need to worry about
 * the file being currently used. This is for module code removing
 * the tracepoints from within it.
 */
void destroy_preds(struct ftrace_event_file *file)
{
	if (file->event_call->flags & TRACE_EVENT_FL_USE_CALL_FILTER)
		destroy_call_preds(file->event_call);
	else
		destroy_file_preds(file);
>>>>>>> refs/remotes/origin/master
}

static struct event_filter *__alloc_filter(void)
{
	struct event_filter *filter;

	filter = kzalloc(sizeof(*filter), GFP_KERNEL);
	return filter;
}

static int __alloc_preds(struct event_filter *filter, int n_preds)
{
	struct filter_pred *pred;
	int i;

	if (filter->preds)
		__free_preds(filter);

<<<<<<< HEAD
<<<<<<< HEAD
	filter->preds =
		kzalloc(sizeof(*filter->preds) * n_preds, GFP_KERNEL);
=======
	filter->preds = kcalloc(n_preds, sizeof(*filter->preds), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	filter->preds = kcalloc(n_preds, sizeof(*filter->preds), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master

	if (!filter->preds)
		return -ENOMEM;

	filter->a_preds = n_preds;
	filter->n_preds = 0;

	for (i = 0; i < n_preds; i++) {
		pred = &filter->preds[i];
		pred->fn = filter_pred_none;
	}

	return 0;
}

<<<<<<< HEAD
static void filter_free_subsystem_preds(struct event_subsystem *system)
{
	struct ftrace_event_call *call;

	list_for_each_entry(call, &ftrace_events, list) {
		if (strcmp(call->class->system, system->name) != 0)
			continue;

		filter_disable(call);
		remove_filter_string(call->filter);
	}
}

static void filter_free_subsystem_filters(struct event_subsystem *system)
{
	struct ftrace_event_call *call;

	list_for_each_entry(call, &ftrace_events, list) {
		if (strcmp(call->class->system, system->name) != 0)
			continue;
		__free_filter(call->filter);
		call->filter = NULL;
	}
}

<<<<<<< HEAD
static int filter_add_pred_fn(struct filter_parse_state *ps,
			      struct ftrace_event_call *call,
			      struct event_filter *filter,
			      struct filter_pred *pred,
			      struct pred_stack *stack,
			      filter_pred_fn_t fn)
{
	int idx, err;
=======
=======
static inline void __remove_filter(struct ftrace_event_file *file)
{
	struct ftrace_event_call *call = file->event_call;

	filter_disable(file);
	if (call->flags & TRACE_EVENT_FL_USE_CALL_FILTER)
		remove_filter_string(call->filter);
	else
		remove_filter_string(file->filter);
}

static void filter_free_subsystem_preds(struct event_subsystem *system,
					struct trace_array *tr)
{
	struct ftrace_event_file *file;
	struct ftrace_event_call *call;

	list_for_each_entry(file, &tr->events, list) {
		call = file->event_call;
		if (strcmp(call->class->system, system->name) != 0)
			continue;

		__remove_filter(file);
	}
}

static inline void __free_subsystem_filter(struct ftrace_event_file *file)
{
	struct ftrace_event_call *call = file->event_call;

	if (call->flags & TRACE_EVENT_FL_USE_CALL_FILTER) {
		__free_filter(call->filter);
		call->filter = NULL;
	} else {
		__free_filter(file->filter);
		file->filter = NULL;
	}
}

static void filter_free_subsystem_filters(struct event_subsystem *system,
					  struct trace_array *tr)
{
	struct ftrace_event_file *file;
	struct ftrace_event_call *call;

	list_for_each_entry(file, &tr->events, list) {
		call = file->event_call;
		if (strcmp(call->class->system, system->name) != 0)
			continue;
		__free_subsystem_filter(file);
	}
}

>>>>>>> refs/remotes/origin/master
static int filter_add_pred(struct filter_parse_state *ps,
			   struct event_filter *filter,
			   struct filter_pred *pred,
			   struct pred_stack *stack)
{
	int err;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (WARN_ON(filter->n_preds == filter->a_preds)) {
		parse_error(ps, FILT_ERR_TOO_MANY_PREDS, 0);
		return -ENOSPC;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	idx = filter->n_preds;
	filter_clear_pred(&filter->preds[idx]);
	err = filter_set_pred(filter, idx, stack, pred, fn);
=======
	err = filter_set_pred(filter, filter->n_preds, stack, pred);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = filter_set_pred(filter, filter->n_preds, stack, pred);
>>>>>>> refs/remotes/origin/master
	if (err)
		return err;

	filter->n_preds++;

	return 0;
}

int filter_assign_type(const char *type)
{
	if (strstr(type, "__data_loc") && strstr(type, "char"))
		return FILTER_DYN_STRING;

	if (strchr(type, '[') && strstr(type, "char"))
		return FILTER_STATIC_STRING;

	return FILTER_OTHER;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static bool is_function_field(struct ftrace_event_field *field)
{
	return field->filter_type == FILTER_TRACE_FN;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static bool is_string_field(struct ftrace_event_field *field)
{
	return field->filter_type == FILTER_DYN_STRING ||
	       field->filter_type == FILTER_STATIC_STRING ||
	       field->filter_type == FILTER_PTR_STRING;
}

static int is_legal_op(struct ftrace_event_field *field, int op)
{
	if (is_string_field(field) &&
	    (op != OP_EQ && op != OP_NE && op != OP_GLOB))
		return 0;
	if (!is_string_field(field) && op == OP_GLOB)
		return 0;

	return 1;
}

static filter_pred_fn_t select_comparison_fn(int op, int field_size,
					     int field_is_signed)
{
	filter_pred_fn_t fn = NULL;

	switch (field_size) {
	case 8:
		if (op == OP_EQ || op == OP_NE)
			fn = filter_pred_64;
		else if (field_is_signed)
			fn = filter_pred_s64;
		else
			fn = filter_pred_u64;
		break;
	case 4:
		if (op == OP_EQ || op == OP_NE)
			fn = filter_pred_32;
		else if (field_is_signed)
			fn = filter_pred_s32;
		else
			fn = filter_pred_u32;
		break;
	case 2:
		if (op == OP_EQ || op == OP_NE)
			fn = filter_pred_16;
		else if (field_is_signed)
			fn = filter_pred_s16;
		else
			fn = filter_pred_u16;
		break;
	case 1:
		if (op == OP_EQ || op == OP_NE)
			fn = filter_pred_8;
		else if (field_is_signed)
			fn = filter_pred_s8;
		else
			fn = filter_pred_u8;
		break;
	}

	return fn;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int filter_add_pred(struct filter_parse_state *ps,
			   struct ftrace_event_call *call,
			   struct event_filter *filter,
			   struct filter_pred *pred,
			   struct pred_stack *stack,
			   bool dry_run)
{
	struct ftrace_event_field *field;
	filter_pred_fn_t fn;
	unsigned long long val;
	int ret;

	fn = pred->fn = filter_pred_none;

	if (pred->op == OP_AND)
		goto add_pred_fn;
	else if (pred->op == OP_OR)
		goto add_pred_fn;

	field = find_event_field(call, pred->field_name);
	if (!field) {
		parse_error(ps, FILT_ERR_FIELD_NOT_FOUND, 0);
		return -EINVAL;
	}

=======
=======
>>>>>>> refs/remotes/origin/master
static int init_pred(struct filter_parse_state *ps,
		     struct ftrace_event_field *field,
		     struct filter_pred *pred)

{
	filter_pred_fn_t fn = filter_pred_none;
	unsigned long long val;
	int ret;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	pred->offset = field->offset;

	if (!is_legal_op(field, pred->op)) {
		parse_error(ps, FILT_ERR_ILLEGAL_FIELD_OP, 0);
		return -EINVAL;
	}

	if (is_string_field(field)) {
		filter_build_regex(pred);

		if (field->filter_type == FILTER_STATIC_STRING) {
			fn = filter_pred_string;
			pred->regex.field_len = field->size;
		} else if (field->filter_type == FILTER_DYN_STRING)
			fn = filter_pred_strloc;
		else
			fn = filter_pred_pchar;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	} else if (is_function_field(field)) {
		if (strcmp(field->name, "ip")) {
			parse_error(ps, FILT_ERR_IP_FIELD_ONLY, 0);
			return -EINVAL;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		if (field->is_signed)
			ret = strict_strtoll(pred->regex.pattern, 0, &val);
		else
			ret = strict_strtoull(pred->regex.pattern, 0, &val);
=======
	} else {
		if (field->is_signed)
			ret = kstrtoll(pred->regex.pattern, 0, &val);
		else
			ret = kstrtoull(pred->regex.pattern, 0, &val);
>>>>>>> refs/remotes/origin/master
		if (ret) {
			parse_error(ps, FILT_ERR_ILLEGAL_INTVAL, 0);
			return -EINVAL;
		}
		pred->val = val;

		fn = select_comparison_fn(pred->op, field->size,
					  field->is_signed);
		if (!fn) {
			parse_error(ps, FILT_ERR_INVALID_OP, 0);
			return -EINVAL;
		}
	}

	if (pred->op == OP_NE)
		pred->not = 1;

<<<<<<< HEAD
<<<<<<< HEAD
add_pred_fn:
	if (!dry_run)
		return filter_add_pred_fn(ps, call, filter, pred, stack, fn);
=======
	pred->fn = fn;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pred->fn = fn;
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void parse_init(struct filter_parse_state *ps,
		       struct filter_op *ops,
		       char *infix_string)
{
	memset(ps, '\0', sizeof(*ps));

	ps->infix.string = infix_string;
	ps->infix.cnt = strlen(infix_string);
	ps->ops = ops;

	INIT_LIST_HEAD(&ps->opstack);
	INIT_LIST_HEAD(&ps->postfix);
}

static char infix_next(struct filter_parse_state *ps)
{
	ps->infix.cnt--;

	return ps->infix.string[ps->infix.tail++];
}

static char infix_peek(struct filter_parse_state *ps)
{
	if (ps->infix.tail == strlen(ps->infix.string))
		return 0;

	return ps->infix.string[ps->infix.tail];
}

static void infix_advance(struct filter_parse_state *ps)
{
	ps->infix.cnt--;
	ps->infix.tail++;
}

static inline int is_precedence_lower(struct filter_parse_state *ps,
				      int a, int b)
{
	return ps->ops[a].precedence < ps->ops[b].precedence;
}

static inline int is_op_char(struct filter_parse_state *ps, char c)
{
	int i;

	for (i = 0; strcmp(ps->ops[i].string, "OP_NONE"); i++) {
		if (ps->ops[i].string[0] == c)
			return 1;
	}

	return 0;
}

static int infix_get_op(struct filter_parse_state *ps, char firstc)
{
	char nextc = infix_peek(ps);
	char opstr[3];
	int i;

	opstr[0] = firstc;
	opstr[1] = nextc;
	opstr[2] = '\0';

	for (i = 0; strcmp(ps->ops[i].string, "OP_NONE"); i++) {
		if (!strcmp(opstr, ps->ops[i].string)) {
			infix_advance(ps);
			return ps->ops[i].id;
		}
	}

	opstr[1] = '\0';

	for (i = 0; strcmp(ps->ops[i].string, "OP_NONE"); i++) {
		if (!strcmp(opstr, ps->ops[i].string))
			return ps->ops[i].id;
	}

	return OP_NONE;
}

static inline void clear_operand_string(struct filter_parse_state *ps)
{
	memset(ps->operand.string, '\0', MAX_FILTER_STR_VAL);
	ps->operand.tail = 0;
}

static inline int append_operand_char(struct filter_parse_state *ps, char c)
{
	if (ps->operand.tail == MAX_FILTER_STR_VAL - 1)
		return -EINVAL;

	ps->operand.string[ps->operand.tail++] = c;

	return 0;
}

static int filter_opstack_push(struct filter_parse_state *ps, int op)
{
	struct opstack_op *opstack_op;

	opstack_op = kmalloc(sizeof(*opstack_op), GFP_KERNEL);
	if (!opstack_op)
		return -ENOMEM;

	opstack_op->op = op;
	list_add(&opstack_op->list, &ps->opstack);

	return 0;
}

static int filter_opstack_empty(struct filter_parse_state *ps)
{
	return list_empty(&ps->opstack);
}

static int filter_opstack_top(struct filter_parse_state *ps)
{
	struct opstack_op *opstack_op;

	if (filter_opstack_empty(ps))
		return OP_NONE;

	opstack_op = list_first_entry(&ps->opstack, struct opstack_op, list);

	return opstack_op->op;
}

static int filter_opstack_pop(struct filter_parse_state *ps)
{
	struct opstack_op *opstack_op;
	int op;

	if (filter_opstack_empty(ps))
		return OP_NONE;

	opstack_op = list_first_entry(&ps->opstack, struct opstack_op, list);
	op = opstack_op->op;
	list_del(&opstack_op->list);

	kfree(opstack_op);

	return op;
}

static void filter_opstack_clear(struct filter_parse_state *ps)
{
	while (!filter_opstack_empty(ps))
		filter_opstack_pop(ps);
}

static char *curr_operand(struct filter_parse_state *ps)
{
	return ps->operand.string;
}

static int postfix_append_operand(struct filter_parse_state *ps, char *operand)
{
	struct postfix_elt *elt;

	elt = kmalloc(sizeof(*elt), GFP_KERNEL);
	if (!elt)
		return -ENOMEM;

	elt->op = OP_NONE;
	elt->operand = kstrdup(operand, GFP_KERNEL);
	if (!elt->operand) {
		kfree(elt);
		return -ENOMEM;
	}

	list_add_tail(&elt->list, &ps->postfix);

	return 0;
}

static int postfix_append_op(struct filter_parse_state *ps, int op)
{
	struct postfix_elt *elt;

	elt = kmalloc(sizeof(*elt), GFP_KERNEL);
	if (!elt)
		return -ENOMEM;

	elt->op = op;
	elt->operand = NULL;

	list_add_tail(&elt->list, &ps->postfix);

	return 0;
}

static void postfix_clear(struct filter_parse_state *ps)
{
	struct postfix_elt *elt;

	while (!list_empty(&ps->postfix)) {
		elt = list_first_entry(&ps->postfix, struct postfix_elt, list);
		list_del(&elt->list);
		kfree(elt->operand);
		kfree(elt);
	}
}

static int filter_parse(struct filter_parse_state *ps)
{
	int in_string = 0;
	int op, top_op;
	char ch;

	while ((ch = infix_next(ps))) {
		if (ch == '"') {
			in_string ^= 1;
			continue;
		}

		if (in_string)
			goto parse_operand;

		if (isspace(ch))
			continue;

		if (is_op_char(ps, ch)) {
			op = infix_get_op(ps, ch);
			if (op == OP_NONE) {
				parse_error(ps, FILT_ERR_INVALID_OP, 0);
				return -EINVAL;
			}

			if (strlen(curr_operand(ps))) {
				postfix_append_operand(ps, curr_operand(ps));
				clear_operand_string(ps);
			}

			while (!filter_opstack_empty(ps)) {
				top_op = filter_opstack_top(ps);
				if (!is_precedence_lower(ps, top_op, op)) {
					top_op = filter_opstack_pop(ps);
					postfix_append_op(ps, top_op);
					continue;
				}
				break;
			}

			filter_opstack_push(ps, op);
			continue;
		}

		if (ch == '(') {
			filter_opstack_push(ps, OP_OPEN_PAREN);
			continue;
		}

		if (ch == ')') {
			if (strlen(curr_operand(ps))) {
				postfix_append_operand(ps, curr_operand(ps));
				clear_operand_string(ps);
			}

			top_op = filter_opstack_pop(ps);
			while (top_op != OP_NONE) {
				if (top_op == OP_OPEN_PAREN)
					break;
				postfix_append_op(ps, top_op);
				top_op = filter_opstack_pop(ps);
			}
			if (top_op == OP_NONE) {
				parse_error(ps, FILT_ERR_UNBALANCED_PAREN, 0);
				return -EINVAL;
			}
			continue;
		}
parse_operand:
		if (append_operand_char(ps, ch)) {
			parse_error(ps, FILT_ERR_OPERAND_TOO_LONG, 0);
			return -EINVAL;
		}
	}

	if (strlen(curr_operand(ps)))
		postfix_append_operand(ps, curr_operand(ps));

	while (!filter_opstack_empty(ps)) {
		top_op = filter_opstack_pop(ps);
		if (top_op == OP_NONE)
			break;
		if (top_op == OP_OPEN_PAREN) {
			parse_error(ps, FILT_ERR_UNBALANCED_PAREN, 0);
			return -EINVAL;
		}
		postfix_append_op(ps, top_op);
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct filter_pred *create_pred(int op, char *operand1, char *operand2)
{
	struct filter_pred *pred;

	pred = kzalloc(sizeof(*pred), GFP_KERNEL);
	if (!pred)
		return NULL;

	pred->field_name = kstrdup(operand1, GFP_KERNEL);
	if (!pred->field_name) {
		kfree(pred);
		return NULL;
	}

	strcpy(pred->regex.pattern, operand2);
	pred->regex.len = strlen(pred->regex.pattern);

	pred->op = op;

	return pred;
}

static struct filter_pred *create_logical_pred(int op)
{
	struct filter_pred *pred;

	pred = kzalloc(sizeof(*pred), GFP_KERNEL);
	if (!pred)
		return NULL;

	pred->op = op;

	return pred;
=======
=======
>>>>>>> refs/remotes/origin/master
static struct filter_pred *create_pred(struct filter_parse_state *ps,
				       struct ftrace_event_call *call,
				       int op, char *operand1, char *operand2)
{
	struct ftrace_event_field *field;
	static struct filter_pred pred;

	memset(&pred, 0, sizeof(pred));
	pred.op = op;

	if (op == OP_AND || op == OP_OR)
		return &pred;

	if (!operand1 || !operand2) {
		parse_error(ps, FILT_ERR_MISSING_FIELD, 0);
		return NULL;
	}

<<<<<<< HEAD
	field = find_event_field(call, operand1);
=======
	field = trace_find_event_field(call, operand1);
>>>>>>> refs/remotes/origin/master
	if (!field) {
		parse_error(ps, FILT_ERR_FIELD_NOT_FOUND, 0);
		return NULL;
	}

	strcpy(pred.regex.pattern, operand2);
	pred.regex.len = strlen(pred.regex.pattern);
	pred.field = field;
	return init_pred(ps, field, &pred) ? NULL : &pred;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int check_preds(struct filter_parse_state *ps)
{
	int n_normal_preds = 0, n_logical_preds = 0;
	struct postfix_elt *elt;

	list_for_each_entry(elt, &ps->postfix, list) {
		if (elt->op == OP_NONE)
			continue;

		if (elt->op == OP_AND || elt->op == OP_OR) {
			n_logical_preds++;
			continue;
		}
		n_normal_preds++;
	}

	if (!n_normal_preds || n_logical_preds >= n_normal_preds) {
		parse_error(ps, FILT_ERR_INVALID_FILTER, 0);
		return -EINVAL;
	}

	return 0;
}

static int count_preds(struct filter_parse_state *ps)
{
	struct postfix_elt *elt;
	int n_preds = 0;

	list_for_each_entry(elt, &ps->postfix, list) {
		if (elt->op == OP_NONE)
			continue;
		n_preds++;
	}

	return n_preds;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
struct check_pred_data {
	int count;
	int max;
};

static int check_pred_tree_cb(enum move_type move, struct filter_pred *pred,
			      int *err, void *data)
{
	struct check_pred_data *d = data;

	if (WARN_ON(d->count++ > d->max)) {
		*err = -EINVAL;
		return WALK_PRED_ABORT;
	}
	return WALK_PRED_DEFAULT;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * The tree is walked at filtering of an event. If the tree is not correctly
 * built, it may cause an infinite loop. Check here that the tree does
 * indeed terminate.
 */
static int check_pred_tree(struct event_filter *filter,
			   struct filter_pred *root)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct filter_pred *preds;
	struct filter_pred *pred;
	enum move_type move = MOVE_DOWN;
	int count = 0;
	int done = 0;
	int max;

	/*
	 * The max that we can hit a node is three times.
	 * Once going down, once coming up from left, and
	 * once coming up from right. This is more than enough
	 * since leafs are only hit a single time.
	 */
	max = 3 * filter->n_preds;

	preds = filter->preds;
	if  (!preds)
		return -EINVAL;
	pred = root;

	do {
		if (WARN_ON(count++ > max))
			return -EINVAL;

		switch (move) {
		case MOVE_DOWN:
			if (pred->left != FILTER_PRED_INVALID) {
				pred = &preds[pred->left];
				continue;
			}
			/* A leaf at the root is just a leaf in the tree */
			if (pred == root)
				break;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		case MOVE_UP_FROM_LEFT:
			pred = &preds[pred->right];
			move = MOVE_DOWN;
			continue;
		case MOVE_UP_FROM_RIGHT:
			if (pred == root)
				break;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		}
		done = 1;
	} while (!done);

	/* We are fine. */
	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct check_pred_data data = {
		/*
		 * The max that we can hit a node is three times.
		 * Once going down, once coming up from left, and
		 * once coming up from right. This is more than enough
		 * since leafs are only hit a single time.
		 */
		.max   = 3 * filter->n_preds,
		.count = 0,
	};

	return walk_pred_tree(filter->preds, root,
			      check_pred_tree_cb, &data);
}

static int count_leafs_cb(enum move_type move, struct filter_pred *pred,
			  int *err, void *data)
{
	int *count = data;

	if ((move == MOVE_DOWN) &&
	    (pred->left == FILTER_PRED_INVALID))
		(*count)++;

	return WALK_PRED_DEFAULT;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int count_leafs(struct filter_pred *preds, struct filter_pred *root)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct filter_pred *pred;
	enum move_type move = MOVE_DOWN;
	int count = 0;
	int done = 0;

	pred = root;

	do {
		switch (move) {
		case MOVE_DOWN:
			if (pred->left != FILTER_PRED_INVALID) {
				pred = &preds[pred->left];
				continue;
			}
			/* A leaf at the root is just a leaf in the tree */
			if (pred == root)
				return 1;
			count++;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		case MOVE_UP_FROM_LEFT:
			pred = &preds[pred->right];
			move = MOVE_DOWN;
			continue;
		case MOVE_UP_FROM_RIGHT:
			if (pred == root)
				break;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		}
		done = 1;
	} while (!done);

	return count;
=======
=======
>>>>>>> refs/remotes/origin/master
	int count = 0, ret;

	ret = walk_pred_tree(preds, root, count_leafs_cb, &count);
	WARN_ON(ret);
	return count;
}

struct fold_pred_data {
	struct filter_pred *root;
	int count;
	int children;
};

static int fold_pred_cb(enum move_type move, struct filter_pred *pred,
			int *err, void *data)
{
	struct fold_pred_data *d = data;
	struct filter_pred *root = d->root;

	if (move != MOVE_DOWN)
		return WALK_PRED_DEFAULT;
	if (pred->left != FILTER_PRED_INVALID)
		return WALK_PRED_DEFAULT;

	if (WARN_ON(d->count == d->children)) {
		*err = -EINVAL;
		return WALK_PRED_ABORT;
	}

	pred->index &= ~FILTER_PRED_FOLD;
	root->ops[d->count++] = pred->index;
	return WALK_PRED_DEFAULT;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int fold_pred(struct filter_pred *preds, struct filter_pred *root)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct filter_pred *pred;
	enum move_type move = MOVE_DOWN;
	int count = 0;
	int children;
	int done = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct fold_pred_data data = {
		.root  = root,
		.count = 0,
	};
	int children;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* No need to keep the fold flag */
	root->index &= ~FILTER_PRED_FOLD;

	/* If the root is a leaf then do nothing */
	if (root->left == FILTER_PRED_INVALID)
		return 0;

	/* count the children */
	children = count_leafs(preds, &preds[root->left]);
	children += count_leafs(preds, &preds[root->right]);

<<<<<<< HEAD
<<<<<<< HEAD
	root->ops = kzalloc(sizeof(*root->ops) * children, GFP_KERNEL);
=======
	root->ops = kcalloc(children, sizeof(*root->ops), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	root->ops = kcalloc(children, sizeof(*root->ops), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!root->ops)
		return -ENOMEM;

	root->val = children;
<<<<<<< HEAD
<<<<<<< HEAD

	pred = root;
	do {
		switch (move) {
		case MOVE_DOWN:
			if (pred->left != FILTER_PRED_INVALID) {
				pred = &preds[pred->left];
				continue;
			}
			if (WARN_ON(count == children))
				return -EINVAL;
			pred->index &= ~FILTER_PRED_FOLD;
			root->ops[count++] = pred->index;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		case MOVE_UP_FROM_LEFT:
			pred = &preds[pred->right];
			move = MOVE_DOWN;
			continue;
		case MOVE_UP_FROM_RIGHT:
			if (pred == root)
				break;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		}
		done = 1;
	} while (!done);

	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	data.children = children;
	return walk_pred_tree(preds, root, fold_pred_cb, &data);
}

static int fold_pred_tree_cb(enum move_type move, struct filter_pred *pred,
			     int *err, void *data)
{
	struct filter_pred *preds = data;

	if (move != MOVE_DOWN)
		return WALK_PRED_DEFAULT;
	if (!(pred->index & FILTER_PRED_FOLD))
		return WALK_PRED_DEFAULT;

	*err = fold_pred(preds, pred);
	if (*err)
		return WALK_PRED_ABORT;

	/* eveyrhing below is folded, continue with parent */
	return WALK_PRED_PARENT;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * To optimize the processing of the ops, if we have several "ors" or
 * "ands" together, we can put them in an array and process them all
 * together speeding up the filter logic.
 */
static int fold_pred_tree(struct event_filter *filter,
			   struct filter_pred *root)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct filter_pred *preds;
	struct filter_pred *pred;
	enum move_type move = MOVE_DOWN;
	int done = 0;
	int err;

	preds = filter->preds;
	if  (!preds)
		return -EINVAL;
	pred = root;

	do {
		switch (move) {
		case MOVE_DOWN:
			if (pred->index & FILTER_PRED_FOLD) {
				err = fold_pred(preds, pred);
				if (err)
					return err;
				/* Folded nodes are like leafs */
			} else if (pred->left != FILTER_PRED_INVALID) {
				pred = &preds[pred->left];
				continue;
			}

			/* A leaf at the root is just a leaf in the tree */
			if (pred == root)
				break;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		case MOVE_UP_FROM_LEFT:
			pred = &preds[pred->right];
			move = MOVE_DOWN;
			continue;
		case MOVE_UP_FROM_RIGHT:
			if (pred == root)
				break;
			pred = get_pred_parent(pred, preds,
					       pred->parent, &move);
			continue;
		}
		done = 1;
	} while (!done);

	return 0;
=======
	return walk_pred_tree(filter->preds, root, fold_pred_tree_cb,
			      filter->preds);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return walk_pred_tree(filter->preds, root, fold_pred_tree_cb,
			      filter->preds);
>>>>>>> refs/remotes/origin/master
}

static int replace_preds(struct ftrace_event_call *call,
			 struct event_filter *filter,
			 struct filter_parse_state *ps,
			 char *filter_string,
			 bool dry_run)
{
	char *operand1 = NULL, *operand2 = NULL;
	struct filter_pred *pred;
	struct filter_pred *root;
	struct postfix_elt *elt;
	struct pred_stack stack = { }; /* init to NULL */
	int err;
	int n_preds = 0;

	n_preds = count_preds(ps);
	if (n_preds >= MAX_FILTER_PRED) {
		parse_error(ps, FILT_ERR_TOO_MANY_PREDS, 0);
		return -ENOSPC;
	}

	err = check_preds(ps);
	if (err)
		return err;

	if (!dry_run) {
		err = __alloc_pred_stack(&stack, n_preds);
		if (err)
			return err;
		err = __alloc_preds(filter, n_preds);
		if (err)
			goto fail;
	}

	n_preds = 0;
	list_for_each_entry(elt, &ps->postfix, list) {
		if (elt->op == OP_NONE) {
			if (!operand1)
				operand1 = elt->operand;
			else if (!operand2)
				operand2 = elt->operand;
			else {
				parse_error(ps, FILT_ERR_TOO_MANY_OPERANDS, 0);
				err = -EINVAL;
				goto fail;
			}
			continue;
		}

		if (WARN_ON(n_preds++ == MAX_FILTER_PRED)) {
			parse_error(ps, FILT_ERR_TOO_MANY_PREDS, 0);
			err = -ENOSPC;
			goto fail;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		if (elt->op == OP_AND || elt->op == OP_OR) {
			pred = create_logical_pred(elt->op);
			goto add_pred;
		}

		if (!operand1 || !operand2) {
			parse_error(ps, FILT_ERR_MISSING_FIELD, 0);
=======
		pred = create_pred(ps, call, elt->op, operand1, operand2);
		if (!pred) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pred = create_pred(ps, call, elt->op, operand1, operand2);
		if (!pred) {
>>>>>>> refs/remotes/origin/master
			err = -EINVAL;
			goto fail;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		pred = create_pred(elt->op, operand1, operand2);
add_pred:
		if (!pred) {
			err = -ENOMEM;
			goto fail;
		}
		err = filter_add_pred(ps, call, filter, pred, &stack, dry_run);
		filter_free_pred(pred);
		if (err)
			goto fail;
=======
=======
>>>>>>> refs/remotes/origin/master
		if (!dry_run) {
			err = filter_add_pred(ps, filter, pred, &stack);
			if (err)
				goto fail;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		operand1 = operand2 = NULL;
	}

	if (!dry_run) {
		/* We should have one item left on the stack */
		pred = __pop_pred_stack(&stack);
		if (!pred)
			return -EINVAL;
		/* This item is where we start from in matching */
		root = pred;
		/* Make sure the stack is empty */
		pred = __pop_pred_stack(&stack);
		if (WARN_ON(pred)) {
			err = -EINVAL;
			filter->root = NULL;
			goto fail;
		}
		err = check_pred_tree(filter, root);
		if (err)
			goto fail;

		/* Optimize the tree */
		err = fold_pred_tree(filter, root);
		if (err)
			goto fail;

		/* We don't set root until we know it works */
		barrier();
		filter->root = root;
	}

	err = 0;
fail:
	__free_pred_stack(&stack);
	return err;
}

<<<<<<< HEAD
=======
static inline void event_set_filtered_flag(struct ftrace_event_file *file)
{
	struct ftrace_event_call *call = file->event_call;

	if (call->flags & TRACE_EVENT_FL_USE_CALL_FILTER)
		call->flags |= TRACE_EVENT_FL_FILTERED;
	else
		file->flags |= FTRACE_EVENT_FL_FILTERED;
}

static inline void event_set_filter(struct ftrace_event_file *file,
				    struct event_filter *filter)
{
	struct ftrace_event_call *call = file->event_call;

	if (call->flags & TRACE_EVENT_FL_USE_CALL_FILTER)
		rcu_assign_pointer(call->filter, filter);
	else
		rcu_assign_pointer(file->filter, filter);
}

static inline void event_clear_filter(struct ftrace_event_file *file)
{
	struct ftrace_event_call *call = file->event_call;

	if (call->flags & TRACE_EVENT_FL_USE_CALL_FILTER)
		RCU_INIT_POINTER(call->filter, NULL);
	else
		RCU_INIT_POINTER(file->filter, NULL);
}

static inline void
event_set_no_set_filter_flag(struct ftrace_event_file *file)
{
	struct ftrace_event_call *call = file->event_call;

	if (call->flags & TRACE_EVENT_FL_USE_CALL_FILTER)
		call->flags |= TRACE_EVENT_FL_NO_SET_FILTER;
	else
		file->flags |= FTRACE_EVENT_FL_NO_SET_FILTER;
}

static inline void
event_clear_no_set_filter_flag(struct ftrace_event_file *file)
{
	struct ftrace_event_call *call = file->event_call;

	if (call->flags & TRACE_EVENT_FL_USE_CALL_FILTER)
		call->flags &= ~TRACE_EVENT_FL_NO_SET_FILTER;
	else
		file->flags &= ~FTRACE_EVENT_FL_NO_SET_FILTER;
}

static inline bool
event_no_set_filter_flag(struct ftrace_event_file *file)
{
	struct ftrace_event_call *call = file->event_call;

	if (file->flags & FTRACE_EVENT_FL_NO_SET_FILTER)
		return true;

	if ((call->flags & TRACE_EVENT_FL_USE_CALL_FILTER) &&
	    (call->flags & TRACE_EVENT_FL_NO_SET_FILTER))
		return true;

	return false;
}

>>>>>>> refs/remotes/origin/master
struct filter_list {
	struct list_head	list;
	struct event_filter	*filter;
};

static int replace_system_preds(struct event_subsystem *system,
<<<<<<< HEAD
				struct filter_parse_state *ps,
				char *filter_string)
{
=======
				struct trace_array *tr,
				struct filter_parse_state *ps,
				char *filter_string)
{
	struct ftrace_event_file *file;
>>>>>>> refs/remotes/origin/master
	struct ftrace_event_call *call;
	struct filter_list *filter_item;
	struct filter_list *tmp;
	LIST_HEAD(filter_list);
	bool fail = true;
	int err;

<<<<<<< HEAD
	list_for_each_entry(call, &ftrace_events, list) {

=======
	list_for_each_entry(file, &tr->events, list) {
		call = file->event_call;
>>>>>>> refs/remotes/origin/master
		if (strcmp(call->class->system, system->name) != 0)
			continue;

		/*
		 * Try to see if the filter can be applied
		 *  (filter arg is ignored on dry_run)
		 */
		err = replace_preds(call, NULL, ps, filter_string, true);
		if (err)
<<<<<<< HEAD
<<<<<<< HEAD
			goto fail;
=======
			call->flags |= TRACE_EVENT_FL_NO_SET_FILTER;
		else
			call->flags &= ~TRACE_EVENT_FL_NO_SET_FILTER;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	list_for_each_entry(call, &ftrace_events, list) {
		struct event_filter *filter;

		if (strcmp(call->class->system, system->name) != 0)
			continue;

<<<<<<< HEAD
=======
		if (call->flags & TRACE_EVENT_FL_NO_SET_FILTER)
			continue;

>>>>>>> refs/remotes/origin/cm-10.0
=======
			event_set_no_set_filter_flag(file);
		else
			event_clear_no_set_filter_flag(file);
	}

	list_for_each_entry(file, &tr->events, list) {
		struct event_filter *filter;

		call = file->event_call;

		if (strcmp(call->class->system, system->name) != 0)
			continue;

		if (event_no_set_filter_flag(file))
			continue;

>>>>>>> refs/remotes/origin/master
		filter_item = kzalloc(sizeof(*filter_item), GFP_KERNEL);
		if (!filter_item)
			goto fail_mem;

		list_add_tail(&filter_item->list, &filter_list);

		filter_item->filter = __alloc_filter();
		if (!filter_item->filter)
			goto fail_mem;
		filter = filter_item->filter;

		/* Can only fail on no memory */
		err = replace_filter_string(filter, filter_string);
		if (err)
			goto fail_mem;

		err = replace_preds(call, filter, ps, filter_string, false);
		if (err) {
<<<<<<< HEAD
			filter_disable(call);
			parse_error(ps, FILT_ERR_BAD_SUBSYS_FILTER, 0);
			append_filter_err(ps, filter);
		} else
			call->flags |= TRACE_EVENT_FL_FILTERED;
=======
			filter_disable(file);
			parse_error(ps, FILT_ERR_BAD_SUBSYS_FILTER, 0);
			append_filter_err(ps, filter);
		} else
			event_set_filtered_flag(file);
>>>>>>> refs/remotes/origin/master
		/*
		 * Regardless of if this returned an error, we still
		 * replace the filter for the call.
		 */
<<<<<<< HEAD
		filter = call->filter;
		rcu_assign_pointer(call->filter, filter_item->filter);
=======
		filter = event_filter(file);
		event_set_filter(file, filter_item->filter);
>>>>>>> refs/remotes/origin/master
		filter_item->filter = filter;

		fail = false;
	}

	if (fail)
		goto fail;

	/*
	 * The calls can still be using the old filters.
	 * Do a synchronize_sched() to ensure all calls are
	 * done with them before we free them.
	 */
	synchronize_sched();
	list_for_each_entry_safe(filter_item, tmp, &filter_list, list) {
		__free_filter(filter_item->filter);
		list_del(&filter_item->list);
		kfree(filter_item);
	}
	return 0;
 fail:
	/* No call succeeded */
	list_for_each_entry_safe(filter_item, tmp, &filter_list, list) {
		list_del(&filter_item->list);
		kfree(filter_item);
	}
	parse_error(ps, FILT_ERR_BAD_SUBSYS_FILTER, 0);
	return -EINVAL;
 fail_mem:
	/* If any call succeeded, we still need to sync */
	if (!fail)
		synchronize_sched();
	list_for_each_entry_safe(filter_item, tmp, &filter_list, list) {
		__free_filter(filter_item->filter);
		list_del(&filter_item->list);
		kfree(filter_item);
	}
	return -ENOMEM;
}

<<<<<<< HEAD
<<<<<<< HEAD
int apply_event_filter(struct ftrace_event_call *call, char *filter_string)
{
	struct filter_parse_state *ps;
	struct event_filter *filter;
	struct event_filter *tmp;
=======
=======
>>>>>>> refs/remotes/origin/master
static int create_filter_start(char *filter_str, bool set_str,
			       struct filter_parse_state **psp,
			       struct event_filter **filterp)
{
	struct event_filter *filter;
	struct filter_parse_state *ps = NULL;
	int err = 0;

	WARN_ON_ONCE(*psp || *filterp);

	/* allocate everything, and if any fails, free all and fail */
	filter = __alloc_filter();
	if (filter && set_str)
		err = replace_filter_string(filter, filter_str);

	ps = kzalloc(sizeof(*ps), GFP_KERNEL);

	if (!filter || !ps || err) {
		kfree(ps);
		__free_filter(filter);
		return -ENOMEM;
	}

	/* we're committed to creating a new filter */
	*filterp = filter;
	*psp = ps;

	parse_init(ps, filter_ops, filter_str);
	err = filter_parse(ps);
	if (err && set_str)
		append_filter_err(ps, filter);
	return err;
}

static void create_filter_finish(struct filter_parse_state *ps)
{
	if (ps) {
		filter_opstack_clear(ps);
		postfix_clear(ps);
		kfree(ps);
	}
}

/**
 * create_filter - create a filter for a ftrace_event_call
 * @call: ftrace_event_call to create a filter for
 * @filter_str: filter string
 * @set_str: remember @filter_str and enable detailed error in filter
 * @filterp: out param for created filter (always updated on return)
 *
 * Creates a filter for @call with @filter_str.  If @set_str is %true,
 * @filter_str is copied and recorded in the new filter.
 *
 * On success, returns 0 and *@filterp points to the new filter.  On
 * failure, returns -errno and *@filterp may point to %NULL or to a new
 * filter.  In the latter case, the returned filter contains error
 * information if @set_str is %true and the caller is responsible for
 * freeing it.
 */
static int create_filter(struct ftrace_event_call *call,
			 char *filter_str, bool set_str,
			 struct event_filter **filterp)
{
	struct event_filter *filter = NULL;
	struct filter_parse_state *ps = NULL;
	int err;

	err = create_filter_start(filter_str, set_str, &ps, &filter);
	if (!err) {
		err = replace_preds(call, filter, ps, filter_str, false);
		if (err && set_str)
			append_filter_err(ps, filter);
	}
	create_filter_finish(ps);

	*filterp = filter;
	return err;
}

/**
 * create_system_filter - create a filter for an event_subsystem
 * @system: event_subsystem to create a filter for
 * @filter_str: filter string
 * @filterp: out param for created filter (always updated on return)
 *
 * Identical to create_filter() except that it creates a subsystem filter
 * and always remembers @filter_str.
 */
static int create_system_filter(struct event_subsystem *system,
<<<<<<< HEAD
=======
				struct trace_array *tr,
>>>>>>> refs/remotes/origin/master
				char *filter_str, struct event_filter **filterp)
{
	struct event_filter *filter = NULL;
	struct filter_parse_state *ps = NULL;
	int err;

	err = create_filter_start(filter_str, true, &ps, &filter);
	if (!err) {
<<<<<<< HEAD
		err = replace_system_preds(system, ps, filter_str);
=======
		err = replace_system_preds(system, tr, ps, filter_str);
>>>>>>> refs/remotes/origin/master
		if (!err) {
			/* System filters just show a default message */
			kfree(filter->filter_string);
			filter->filter_string = NULL;
		} else {
			append_filter_err(ps, filter);
		}
	}
	create_filter_finish(ps);

	*filterp = filter;
	return err;
}

<<<<<<< HEAD
int apply_event_filter(struct ftrace_event_call *call, char *filter_string)
{
	struct event_filter *filter;
>>>>>>> refs/remotes/origin/cm-10.0
	int err = 0;

	mutex_lock(&event_mutex);

	if (!strcmp(strstrip(filter_string), "0")) {
		filter_disable(call);
		filter = call->filter;
		if (!filter)
			goto out_unlock;
		RCU_INIT_POINTER(call->filter, NULL);
		/* Make sure the filter is not being used */
		synchronize_sched();
		__free_filter(filter);
		goto out_unlock;
	}

<<<<<<< HEAD
	err = -ENOMEM;
	ps = kzalloc(sizeof(*ps), GFP_KERNEL);
	if (!ps)
		goto out_unlock;

	filter = __alloc_filter();
	if (!filter) {
		kfree(ps);
		goto out_unlock;
	}

	replace_filter_string(filter, filter_string);

	parse_init(ps, filter_ops, filter_string);
	err = filter_parse(ps);
	if (err) {
		append_filter_err(ps, filter);
		goto out;
	}

	err = replace_preds(call, filter, ps, filter_string, false);
	if (err) {
		filter_disable(call);
		append_filter_err(ps, filter);
	} else
		call->flags |= TRACE_EVENT_FL_FILTERED;
out:
=======
	err = create_filter(call, filter_string, true, &filter);

>>>>>>> refs/remotes/origin/cm-10.0
=======
/* caller must hold event_mutex */
int apply_event_filter(struct ftrace_event_file *file, char *filter_string)
{
	struct ftrace_event_call *call = file->event_call;
	struct event_filter *filter;
	int err;

	if (!strcmp(strstrip(filter_string), "0")) {
		filter_disable(file);
		filter = event_filter(file);

		if (!filter)
			return 0;

		event_clear_filter(file);

		/* Make sure the filter is not being used */
		synchronize_sched();
		__free_filter(filter);

		return 0;
	}

	err = create_filter(call, filter_string, true, &filter);

>>>>>>> refs/remotes/origin/master
	/*
	 * Always swap the call filter with the new filter
	 * even if there was an error. If there was an error
	 * in the filter, we disable the filter and show the error
	 * string
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	tmp = call->filter;
	rcu_assign_pointer(call->filter, filter);
	if (tmp) {
		/* Make sure the call is done with the filter */
		synchronize_sched();
		__free_filter(tmp);
	}
	filter_opstack_clear(ps);
	postfix_clear(ps);
	kfree(ps);
=======
	if (filter) {
		struct event_filter *tmp = call->filter;

		if (!err)
			call->flags |= TRACE_EVENT_FL_FILTERED;
		else
			filter_disable(call);

		rcu_assign_pointer(call->filter, filter);
=======
	if (filter) {
		struct event_filter *tmp;

		tmp = event_filter(file);
		if (!err)
			event_set_filtered_flag(file);
		else
			filter_disable(file);

		event_set_filter(file, filter);
>>>>>>> refs/remotes/origin/master

		if (tmp) {
			/* Make sure the call is done with the filter */
			synchronize_sched();
			__free_filter(tmp);
		}
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
out_unlock:
	mutex_unlock(&event_mutex);
=======
>>>>>>> refs/remotes/origin/master

	return err;
}

<<<<<<< HEAD
int apply_subsystem_event_filter(struct event_subsystem *system,
				 char *filter_string)
{
<<<<<<< HEAD
	struct filter_parse_state *ps;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
int apply_subsystem_event_filter(struct ftrace_subsystem_dir *dir,
				 char *filter_string)
{
	struct event_subsystem *system = dir->subsystem;
	struct trace_array *tr = dir->tr;
>>>>>>> refs/remotes/origin/master
	struct event_filter *filter;
	int err = 0;

	mutex_lock(&event_mutex);

	/* Make sure the system still has events */
<<<<<<< HEAD
	if (!system->nr_events) {
=======
	if (!dir->nr_events) {
>>>>>>> refs/remotes/origin/master
		err = -ENODEV;
		goto out_unlock;
	}

	if (!strcmp(strstrip(filter_string), "0")) {
<<<<<<< HEAD
		filter_free_subsystem_preds(system);
=======
		filter_free_subsystem_preds(system, tr);
>>>>>>> refs/remotes/origin/master
		remove_filter_string(system->filter);
		filter = system->filter;
		system->filter = NULL;
		/* Ensure all filters are no longer used */
		synchronize_sched();
<<<<<<< HEAD
		filter_free_subsystem_filters(system);
=======
		filter_free_subsystem_filters(system, tr);
>>>>>>> refs/remotes/origin/master
		__free_filter(filter);
		goto out_unlock;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	err = -ENOMEM;
	ps = kzalloc(sizeof(*ps), GFP_KERNEL);
	if (!ps)
		goto out_unlock;

	filter = __alloc_filter();
	if (!filter)
		goto out;

	replace_filter_string(filter, filter_string);
	/*
	 * No event actually uses the system filter
	 * we can free it without synchronize_sched().
	 */
	__free_filter(system->filter);
	system->filter = filter;

	parse_init(ps, filter_ops, filter_string);
	err = filter_parse(ps);
	if (err) {
		append_filter_err(ps, system->filter);
		goto out;
	}

	err = replace_system_preds(system, ps, filter_string);
	if (err)
		append_filter_err(ps, system->filter);

out:
	filter_opstack_clear(ps);
	postfix_clear(ps);
	kfree(ps);
=======
	err = create_system_filter(system, filter_string, &filter);
=======
	err = create_system_filter(system, tr, filter_string, &filter);
>>>>>>> refs/remotes/origin/master
	if (filter) {
		/*
		 * No event actually uses the system filter
		 * we can free it without synchronize_sched().
		 */
		__free_filter(system->filter);
		system->filter = filter;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
out_unlock:
	mutex_unlock(&event_mutex);

	return err;
}

#ifdef CONFIG_PERF_EVENTS

void ftrace_profile_free_filter(struct perf_event *event)
{
	struct event_filter *filter = event->filter;

	event->filter = NULL;
	__free_filter(filter);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
struct function_filter_data {
	struct ftrace_ops *ops;
	int first_filter;
	int first_notrace;
};

#ifdef CONFIG_FUNCTION_TRACER
static char **
ftrace_function_filter_re(char *buf, int len, int *count)
{
	char *str, *sep, **re;

	str = kstrndup(buf, len, GFP_KERNEL);
	if (!str)
		return NULL;

	/*
	 * The argv_split function takes white space
	 * as a separator, so convert ',' into spaces.
	 */
	while ((sep = strchr(str, ',')))
		*sep = ' ';

	re = argv_split(GFP_KERNEL, str, count);
	kfree(str);
	return re;
}

static int ftrace_function_set_regexp(struct ftrace_ops *ops, int filter,
				      int reset, char *re, int len)
{
	int ret;

	if (filter)
		ret = ftrace_set_filter(ops, re, len, reset);
	else
		ret = ftrace_set_notrace(ops, re, len, reset);

	return ret;
}

static int __ftrace_function_set_filter(int filter, char *buf, int len,
					struct function_filter_data *data)
{
<<<<<<< HEAD
	int i, re_cnt, ret;
=======
	int i, re_cnt, ret = -EINVAL;
>>>>>>> refs/remotes/origin/master
	int *reset;
	char **re;

	reset = filter ? &data->first_filter : &data->first_notrace;

	/*
	 * The 'ip' field could have multiple filters set, separated
	 * either by space or comma. We first cut the filter and apply
	 * all pieces separatelly.
	 */
	re = ftrace_function_filter_re(buf, len, &re_cnt);
	if (!re)
		return -EINVAL;

	for (i = 0; i < re_cnt; i++) {
		ret = ftrace_function_set_regexp(data->ops, filter, *reset,
						 re[i], strlen(re[i]));
		if (ret)
			break;

		if (*reset)
			*reset = 0;
	}

	argv_free(re);
	return ret;
}

static int ftrace_function_check_pred(struct filter_pred *pred, int leaf)
{
	struct ftrace_event_field *field = pred->field;

	if (leaf) {
		/*
		 * Check the leaf predicate for function trace, verify:
		 *  - only '==' and '!=' is used
		 *  - the 'ip' field is used
		 */
		if ((pred->op != OP_EQ) && (pred->op != OP_NE))
			return -EINVAL;

		if (strcmp(field->name, "ip"))
			return -EINVAL;
	} else {
		/*
		 * Check the non leaf predicate for function trace, verify:
		 *  - only '||' is used
		*/
		if (pred->op != OP_OR)
			return -EINVAL;
	}

	return 0;
}

static int ftrace_function_set_filter_cb(enum move_type move,
					 struct filter_pred *pred,
					 int *err, void *data)
{
	/* Checking the node is valid for function trace. */
	if ((move != MOVE_DOWN) ||
	    (pred->left != FILTER_PRED_INVALID)) {
		*err = ftrace_function_check_pred(pred, 0);
	} else {
		*err = ftrace_function_check_pred(pred, 1);
		if (*err)
			return WALK_PRED_ABORT;

		*err = __ftrace_function_set_filter(pred->op == OP_EQ,
						    pred->regex.pattern,
						    pred->regex.len,
						    data);
	}

	return (*err) ? WALK_PRED_ABORT : WALK_PRED_DEFAULT;
}

static int ftrace_function_set_filter(struct perf_event *event,
				      struct event_filter *filter)
{
	struct function_filter_data data = {
		.first_filter  = 1,
		.first_notrace = 1,
		.ops           = &event->ftrace_ops,
	};

	return walk_pred_tree(filter->preds, filter->root,
			      ftrace_function_set_filter_cb, &data);
}
#else
static int ftrace_function_set_filter(struct perf_event *event,
				      struct event_filter *filter)
{
	return -ENODEV;
}
#endif /* CONFIG_FUNCTION_TRACER */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int ftrace_profile_set_filter(struct perf_event *event, int event_id,
			      char *filter_str)
{
	int err;
	struct event_filter *filter;
<<<<<<< HEAD
<<<<<<< HEAD
	struct filter_parse_state *ps;
	struct ftrace_event_call *call = NULL;

	mutex_lock(&event_mutex);

	list_for_each_entry(call, &ftrace_events, list) {
		if (call->event.type == event_id)
			break;
	}

	err = -EINVAL;
	if (&call->list == &ftrace_events)
=======
=======
>>>>>>> refs/remotes/origin/master
	struct ftrace_event_call *call;

	mutex_lock(&event_mutex);

	call = event->tp_event;

	err = -EINVAL;
	if (!call)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		goto out_unlock;

	err = -EEXIST;
	if (event->filter)
		goto out_unlock;

<<<<<<< HEAD
<<<<<<< HEAD
	filter = __alloc_filter();
	if (!filter) {
		err = PTR_ERR(filter);
		goto out_unlock;
	}

	err = -ENOMEM;
	ps = kzalloc(sizeof(*ps), GFP_KERNEL);
	if (!ps)
		goto free_filter;

	parse_init(ps, filter_ops, filter_str);
	err = filter_parse(ps);
	if (err)
		goto free_ps;

	err = replace_preds(call, filter, ps, filter_str, false);
	if (!err)
		event->filter = filter;

free_ps:
	filter_opstack_clear(ps);
	postfix_clear(ps);
	kfree(ps);

free_filter:
	if (err)
=======
=======
>>>>>>> refs/remotes/origin/master
	err = create_filter(call, filter_str, false, &filter);
	if (err)
		goto free_filter;

	if (ftrace_event_is_function(call))
		err = ftrace_function_set_filter(event, filter);
	else
		event->filter = filter;

free_filter:
	if (err || ftrace_event_is_function(call))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		__free_filter(filter);

out_unlock:
	mutex_unlock(&event_mutex);

	return err;
}

#endif /* CONFIG_PERF_EVENTS */

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_FTRACE_STARTUP_TEST

#include <linux/types.h>
#include <linux/tracepoint.h>

#define CREATE_TRACE_POINTS
#include "trace_events_filter_test.h"

#define DATA_REC(m, va, vb, vc, vd, ve, vf, vg, vh, nvisit) \
{ \
	.filter = FILTER, \
	.rec    = { .a = va, .b = vb, .c = vc, .d = vd, \
		    .e = ve, .f = vf, .g = vg, .h = vh }, \
	.match  = m, \
	.not_visited = nvisit, \
}
#define YES 1
#define NO  0

static struct test_filter_data_t {
	char *filter;
	struct ftrace_raw_ftrace_test_filter rec;
	int match;
	char *not_visited;
} test_filter_data[] = {
#define FILTER "a == 1 && b == 1 && c == 1 && d == 1 && " \
	       "e == 1 && f == 1 && g == 1 && h == 1"
	DATA_REC(YES, 1, 1, 1, 1, 1, 1, 1, 1, ""),
	DATA_REC(NO,  0, 1, 1, 1, 1, 1, 1, 1, "bcdefgh"),
	DATA_REC(NO,  1, 1, 1, 1, 1, 1, 1, 0, ""),
#undef FILTER
#define FILTER "a == 1 || b == 1 || c == 1 || d == 1 || " \
	       "e == 1 || f == 1 || g == 1 || h == 1"
	DATA_REC(NO,  0, 0, 0, 0, 0, 0, 0, 0, ""),
	DATA_REC(YES, 0, 0, 0, 0, 0, 0, 0, 1, ""),
	DATA_REC(YES, 1, 0, 0, 0, 0, 0, 0, 0, "bcdefgh"),
#undef FILTER
#define FILTER "(a == 1 || b == 1) && (c == 1 || d == 1) && " \
	       "(e == 1 || f == 1) && (g == 1 || h == 1)"
	DATA_REC(NO,  0, 0, 1, 1, 1, 1, 1, 1, "dfh"),
	DATA_REC(YES, 0, 1, 0, 1, 0, 1, 0, 1, ""),
	DATA_REC(YES, 1, 0, 1, 0, 0, 1, 0, 1, "bd"),
	DATA_REC(NO,  1, 0, 1, 0, 0, 1, 0, 0, "bd"),
#undef FILTER
#define FILTER "(a == 1 && b == 1) || (c == 1 && d == 1) || " \
	       "(e == 1 && f == 1) || (g == 1 && h == 1)"
	DATA_REC(YES, 1, 0, 1, 1, 1, 1, 1, 1, "efgh"),
	DATA_REC(YES, 0, 0, 0, 0, 0, 0, 1, 1, ""),
	DATA_REC(NO,  0, 0, 0, 0, 0, 0, 0, 1, ""),
#undef FILTER
#define FILTER "(a == 1 && b == 1) && (c == 1 && d == 1) && " \
	       "(e == 1 && f == 1) || (g == 1 && h == 1)"
	DATA_REC(YES, 1, 1, 1, 1, 1, 1, 0, 0, "gh"),
	DATA_REC(NO,  0, 0, 0, 0, 0, 0, 0, 1, ""),
	DATA_REC(YES, 1, 1, 1, 1, 1, 0, 1, 1, ""),
#undef FILTER
#define FILTER "((a == 1 || b == 1) || (c == 1 || d == 1) || " \
	       "(e == 1 || f == 1)) && (g == 1 || h == 1)"
	DATA_REC(YES, 1, 1, 1, 1, 1, 1, 0, 1, "bcdef"),
	DATA_REC(NO,  0, 0, 0, 0, 0, 0, 0, 0, ""),
	DATA_REC(YES, 1, 1, 1, 1, 1, 0, 1, 1, "h"),
#undef FILTER
#define FILTER "((((((((a == 1) && (b == 1)) || (c == 1)) && (d == 1)) || " \
	       "(e == 1)) && (f == 1)) || (g == 1)) && (h == 1))"
	DATA_REC(YES, 1, 1, 1, 1, 1, 1, 1, 1, "ceg"),
	DATA_REC(NO,  0, 1, 0, 1, 0, 1, 0, 1, ""),
	DATA_REC(NO,  1, 0, 1, 0, 1, 0, 1, 0, ""),
#undef FILTER
#define FILTER "((((((((a == 1) || (b == 1)) && (c == 1)) || (d == 1)) && " \
	       "(e == 1)) || (f == 1)) && (g == 1)) || (h == 1))"
	DATA_REC(YES, 1, 1, 1, 1, 1, 1, 1, 1, "bdfh"),
	DATA_REC(YES, 0, 1, 0, 1, 0, 1, 0, 1, ""),
	DATA_REC(YES, 1, 0, 1, 0, 1, 0, 1, 0, "bdfh"),
};

#undef DATA_REC
#undef FILTER
#undef YES
#undef NO

#define DATA_CNT (sizeof(test_filter_data)/sizeof(struct test_filter_data_t))

static int test_pred_visited;

static int test_pred_visited_fn(struct filter_pred *pred, void *event)
{
	struct ftrace_event_field *field = pred->field;

	test_pred_visited = 1;
	printk(KERN_INFO "\npred visited %s\n", field->name);
	return 1;
}

static int test_walk_pred_cb(enum move_type move, struct filter_pred *pred,
			     int *err, void *data)
{
	char *fields = data;

	if ((move == MOVE_DOWN) &&
	    (pred->left == FILTER_PRED_INVALID)) {
		struct ftrace_event_field *field = pred->field;

		if (!field) {
			WARN(1, "all leafs should have field defined");
			return WALK_PRED_DEFAULT;
		}
		if (!strchr(fields, *field->name))
			return WALK_PRED_DEFAULT;

		WARN_ON(!pred->fn);
		pred->fn = test_pred_visited_fn;
	}
	return WALK_PRED_DEFAULT;
}

static __init int ftrace_test_event_filter(void)
{
	int i;

	printk(KERN_INFO "Testing ftrace filter: ");

	for (i = 0; i < DATA_CNT; i++) {
		struct event_filter *filter = NULL;
		struct test_filter_data_t *d = &test_filter_data[i];
		int err;

		err = create_filter(&event_ftrace_test_filter, d->filter,
				    false, &filter);
		if (err) {
			printk(KERN_INFO
			       "Failed to get filter for '%s', err %d\n",
			       d->filter, err);
			__free_filter(filter);
			break;
		}

		/*
		 * The preemption disabling is not really needed for self
		 * tests, but the rcu dereference will complain without it.
		 */
		preempt_disable();
		if (*d->not_visited)
			walk_pred_tree(filter->preds, filter->root,
				       test_walk_pred_cb,
				       d->not_visited);

		test_pred_visited = 0;
		err = filter_match_preds(filter, &d->rec);
		preempt_enable();

		__free_filter(filter);

		if (test_pred_visited) {
			printk(KERN_INFO
			       "Failed, unwanted pred visited for filter %s\n",
			       d->filter);
			break;
		}

		if (err != d->match) {
			printk(KERN_INFO
			       "Failed to match filter '%s', expected %d\n",
			       d->filter, d->match);
			break;
		}
	}

	if (i == DATA_CNT)
		printk(KERN_CONT "OK\n");

	return 0;
}

late_initcall(ftrace_test_event_filter);

#endif /* CONFIG_FTRACE_STARTUP_TEST */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
