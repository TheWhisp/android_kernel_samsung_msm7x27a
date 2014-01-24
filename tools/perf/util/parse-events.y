<<<<<<< HEAD

%parse-param {struct list_head *list_all}
%parse-param {struct list_head *list_event}
%parse-param {int *idx}
=======
%pure-parser
%parse-param {void *_data}
%parse-param {void *scanner}
%lex-param {void* scanner}
>>>>>>> refs/remotes/origin/master

%{

#define YYDEBUG 1

#include <linux/compiler.h>
#include <linux/list.h>
#include "types.h"
#include "util.h"
#include "parse-events.h"
<<<<<<< HEAD

extern int parse_events_lex (void);
=======
#include "parse-events-bison.h"

extern int parse_events_lex (YYSTYPE* lvalp, void* scanner);
>>>>>>> refs/remotes/origin/master

#define ABORT_ON(val) \
do { \
	if (val) \
		YYABORT; \
} while (0)

<<<<<<< HEAD
%}

%token PE_VALUE PE_VALUE_SYM PE_RAW PE_SH_RAW PE_FAB_RAW PE_TERM
%token PE_NAME
%token PE_MODIFIER_EVENT PE_MODIFIER_BP
%token PE_NAME_CACHE_TYPE PE_NAME_CACHE_OP_RESULT
%token PE_PREFIX_MEM PE_PREFIX_RAW
%token PE_ERROR
%type <num> PE_VALUE
%type <num> PE_VALUE_SYM
%type <num> PE_RAW
%type <num> PE_SH_RAW
%type <num> PE_FAB_RAW
<<<<<<< HEAD
=======
#define ALLOC_LIST(list) \
do { \
	list = malloc(sizeof(*list)); \
	ABORT_ON(!list);              \
	INIT_LIST_HEAD(list);         \
} while (0)

static inc_group_count(struct list_head *list,
		       struct parse_events_evlist *data)
{
	/* Count groups only have more than 1 members */
	if (!list_is_last(list->next, list))
		data->nr_groups++;
}

%}

%token PE_START_EVENTS PE_START_TERMS
%token PE_VALUE PE_VALUE_SYM_HW PE_VALUE_SYM_SW PE_RAW PE_TERM
%token PE_EVENT_NAME
%token PE_NAME
%token PE_MODIFIER_EVENT PE_MODIFIER_BP
%token PE_NAME_CACHE_TYPE PE_NAME_CACHE_OP_RESULT
%token PE_PREFIX_MEM PE_PREFIX_RAW PE_PREFIX_GROUP
%token PE_ERROR
%type <num> PE_VALUE
%type <num> PE_VALUE_SYM_HW
%type <num> PE_VALUE_SYM_SW
%type <num> PE_RAW
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
%type <num> PE_TERM
%type <str> PE_NAME
%type <str> PE_NAME_CACHE_TYPE
%type <str> PE_NAME_CACHE_OP_RESULT
%type <str> PE_MODIFIER_EVENT
%type <str> PE_MODIFIER_BP
<<<<<<< HEAD
%type <head> event_config
%type <term> event_term
=======
%type <str> PE_EVENT_NAME
%type <num> value_sym
%type <head> event_config
%type <term> event_term
%type <head> event_pmu
%type <head> event_legacy_symbol
%type <head> event_legacy_cache
%type <head> event_legacy_mem
%type <head> event_legacy_tracepoint
%type <head> event_legacy_numeric
%type <head> event_legacy_raw
%type <head> event_def
%type <head> event_mod
%type <head> event_name
%type <head> event
%type <head> events
%type <head> group_def
%type <head> group
%type <head> groups
>>>>>>> refs/remotes/origin/master

%union
{
	char *str;
<<<<<<< HEAD
	unsigned long num;
	struct list_head *head;
	struct parse_events__term *term;
}
%%

events:
events ',' event | event

event:
event_def PE_MODIFIER_EVENT
{
=======
	u64 num;
	struct list_head *head;
	struct parse_events_term *term;
}
%%

start:
PE_START_EVENTS start_events
|
PE_START_TERMS  start_terms

start_events: groups
{
	struct parse_events_evlist *data = _data;

	parse_events_update_lists($1, &data->list);
}

groups:
groups ',' group
{
	struct list_head *list  = $1;
	struct list_head *group = $3;

	parse_events_update_lists(group, list);
	$$ = list;
}
|
groups ',' event
{
	struct list_head *list  = $1;
	struct list_head *event = $3;

	parse_events_update_lists(event, list);
	$$ = list;
}
|
group
|
event

group:
group_def ':' PE_MODIFIER_EVENT
{
	struct list_head *list = $1;

	ABORT_ON(parse_events__modifier_group(list, $3));
	$$ = list;
}
|
group_def

group_def:
PE_NAME '{' events '}'
{
	struct list_head *list = $3;

	inc_group_count(list, _data);
	parse_events__set_leader($1, list);
	$$ = list;
}
|
'{' events '}'
{
	struct list_head *list = $2;

	inc_group_count(list, _data);
	parse_events__set_leader(NULL, list);
	$$ = list;
}

events:
events ',' event
{
	struct list_head *event = $3;
	struct list_head *list  = $1;

	parse_events_update_lists(event, list);
	$$ = list;
}
|
event

event: event_mod

event_mod:
event_name PE_MODIFIER_EVENT
{
	struct list_head *list = $1;

>>>>>>> refs/remotes/origin/master
	/*
	 * Apply modifier on all events added by single event definition
	 * (there could be more events added for multiple tracepoint
	 * definitions via '*?'.
	 */
<<<<<<< HEAD
	ABORT_ON(parse_events_modifier(list_event, $2));
	parse_events_update_lists(list_event, list_all);
}
|
event_def
{
	parse_events_update_lists(list_event, list_all);
}
=======
	ABORT_ON(parse_events__modifier_event(list, $2, false));
	$$ = list;
}
|
event_name

event_name:
PE_EVENT_NAME event_def
{
	ABORT_ON(parse_events_name($2, $1));
	free($1);
	$$ = $2;
}
|
event_def
>>>>>>> refs/remotes/origin/master

event_def: event_pmu |
	   event_legacy_symbol |
	   event_legacy_cache sep_dc |
	   event_legacy_mem |
	   event_legacy_tracepoint sep_dc |
	   event_legacy_numeric sep_dc |
<<<<<<< HEAD
<<<<<<< HEAD
	   event_legacy_raw sep_dc |
	   event_legacy_shared_raw sep_dc |
	   event_legacy_fabric_raw sep_dc
=======
	   event_legacy_raw sep_dc
>>>>>>> refs/remotes/origin/master
=======
	   event_legacy_raw sep_dc |
	   event_legacy_shared_raw sep_dc |
	   event_legacy_fabric_raw sep_dc
>>>>>>> refs/remotes/origin/cm-11.0

event_pmu:
PE_NAME '/' event_config '/'
{
<<<<<<< HEAD
	ABORT_ON(parse_events_add_pmu(list_event, idx, $1, $3));
	parse_events__free_terms($3);
}

event_legacy_symbol:
PE_VALUE_SYM '/' event_config '/'
{
	int type = $1 >> 16;
	int config = $1 & 255;

	ABORT_ON(parse_events_add_numeric(list_event, idx, type, config, $3));
	parse_events__free_terms($3);
}
|
PE_VALUE_SYM sep_slash_dc
{
	int type = $1 >> 16;
	int config = $1 & 255;

	ABORT_ON(parse_events_add_numeric(list_event, idx, type, config, NULL));
=======
	struct parse_events_evlist *data = _data;
	struct list_head *list;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_pmu(list, &data->idx, $1, $3));
	parse_events__free_terms($3);
	$$ = list;
}

value_sym:
PE_VALUE_SYM_HW
|
PE_VALUE_SYM_SW

event_legacy_symbol:
value_sym '/' event_config '/'
{
	struct parse_events_evlist *data = _data;
	struct list_head *list;
	int type = $1 >> 16;
	int config = $1 & 255;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_numeric(list, &data->idx,
					  type, config, $3));
	parse_events__free_terms($3);
	$$ = list;
}
|
value_sym sep_slash_dc
{
	struct parse_events_evlist *data = _data;
	struct list_head *list;
	int type = $1 >> 16;
	int config = $1 & 255;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_numeric(list, &data->idx,
					  type, config, NULL));
	$$ = list;
>>>>>>> refs/remotes/origin/master
}

event_legacy_cache:
PE_NAME_CACHE_TYPE '-' PE_NAME_CACHE_OP_RESULT '-' PE_NAME_CACHE_OP_RESULT
{
<<<<<<< HEAD
	ABORT_ON(parse_events_add_cache(list_event, idx, $1, $3, $5));
=======
	struct parse_events_evlist *data = _data;
	struct list_head *list;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_cache(list, &data->idx, $1, $3, $5));
	$$ = list;
>>>>>>> refs/remotes/origin/master
}
|
PE_NAME_CACHE_TYPE '-' PE_NAME_CACHE_OP_RESULT
{
<<<<<<< HEAD
	ABORT_ON(parse_events_add_cache(list_event, idx, $1, $3, NULL));
=======
	struct parse_events_evlist *data = _data;
	struct list_head *list;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_cache(list, &data->idx, $1, $3, NULL));
	$$ = list;
>>>>>>> refs/remotes/origin/master
}
|
PE_NAME_CACHE_TYPE
{
<<<<<<< HEAD
	ABORT_ON(parse_events_add_cache(list_event, idx, $1, NULL, NULL));
=======
	struct parse_events_evlist *data = _data;
	struct list_head *list;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_cache(list, &data->idx, $1, NULL, NULL));
	$$ = list;
>>>>>>> refs/remotes/origin/master
}

event_legacy_mem:
PE_PREFIX_MEM PE_VALUE ':' PE_MODIFIER_BP sep_dc
{
<<<<<<< HEAD
	ABORT_ON(parse_events_add_breakpoint(list_event, idx, (void *) $2, $4));
=======
	struct parse_events_evlist *data = _data;
	struct list_head *list;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_breakpoint(list, &data->idx,
					     (void *) $2, $4));
	$$ = list;
>>>>>>> refs/remotes/origin/master
}
|
PE_PREFIX_MEM PE_VALUE sep_dc
{
<<<<<<< HEAD
	ABORT_ON(parse_events_add_breakpoint(list_event, idx, (void *) $2, NULL));
=======
	struct parse_events_evlist *data = _data;
	struct list_head *list;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_breakpoint(list, &data->idx,
					     (void *) $2, NULL));
	$$ = list;
>>>>>>> refs/remotes/origin/master
}

event_legacy_tracepoint:
PE_NAME ':' PE_NAME
{
<<<<<<< HEAD
	ABORT_ON(parse_events_add_tracepoint(list_event, idx, $1, $3));
=======
	struct parse_events_evlist *data = _data;
	struct list_head *list;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_tracepoint(list, &data->idx, $1, $3));
	$$ = list;
>>>>>>> refs/remotes/origin/master
}

event_legacy_numeric:
PE_VALUE ':' PE_VALUE
{
<<<<<<< HEAD
	ABORT_ON(parse_events_add_numeric(list_event, idx, $1, $3, NULL));
=======
	struct parse_events_evlist *data = _data;
	struct list_head *list;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_numeric(list, &data->idx, (u32)$1, $3, NULL));
	$$ = list;
>>>>>>> refs/remotes/origin/master
}

event_legacy_raw:
PE_RAW
{
<<<<<<< HEAD
	ABORT_ON(parse_events_add_numeric(list_event, idx, PERF_TYPE_RAW, $1, NULL));
}

event_legacy_shared_raw:
PE_SH_RAW
{
	ABORT_ON(parse_events_add_numeric(list_event, idx, 6, $1, NULL));
}

event_legacy_fabric_raw:
PE_FAB_RAW
{
	ABORT_ON(parse_events_add_numeric(list_event, idx, 7, $1, NULL));
<<<<<<< HEAD
=======
	struct parse_events_evlist *data = _data;
	struct list_head *list;

	ALLOC_LIST(list);
	ABORT_ON(parse_events_add_numeric(list, &data->idx,
					  PERF_TYPE_RAW, $1, NULL));
	$$ = list;
}

start_terms: event_config
{
	struct parse_events_terms *data = _data;
	data->terms = $1;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

event_config:
event_config ',' event_term
{
	struct list_head *head = $1;
<<<<<<< HEAD
	struct parse_events__term *term = $3;
=======
	struct parse_events_term *term = $3;
>>>>>>> refs/remotes/origin/master

	ABORT_ON(!head);
	list_add_tail(&term->list, head);
	$$ = $1;
}
|
event_term
{
	struct list_head *head = malloc(sizeof(*head));
<<<<<<< HEAD
	struct parse_events__term *term = $1;
=======
	struct parse_events_term *term = $1;
>>>>>>> refs/remotes/origin/master

	ABORT_ON(!head);
	INIT_LIST_HEAD(head);
	list_add_tail(&term->list, head);
	$$ = head;
}

event_term:
PE_NAME '=' PE_NAME
{
<<<<<<< HEAD
	struct parse_events__term *term;

	ABORT_ON(parse_events__new_term(&term, PARSE_EVENTS__TERM_TYPE_STR,
		 $1, $3, 0));
=======
	struct parse_events_term *term;

	ABORT_ON(parse_events_term__str(&term, PARSE_EVENTS__TERM_TYPE_USER,
					$1, $3));
>>>>>>> refs/remotes/origin/master
	$$ = term;
}
|
PE_NAME '=' PE_VALUE
{
<<<<<<< HEAD
	struct parse_events__term *term;

	ABORT_ON(parse_events__new_term(&term, PARSE_EVENTS__TERM_TYPE_NUM,
		 $1, NULL, $3));
=======
	struct parse_events_term *term;

	ABORT_ON(parse_events_term__num(&term, PARSE_EVENTS__TERM_TYPE_USER,
					$1, $3));
	$$ = term;
}
|
PE_NAME '=' PE_VALUE_SYM_HW
{
	struct parse_events_term *term;
	int config = $3 & 255;

	ABORT_ON(parse_events_term__sym_hw(&term, $1, config));
>>>>>>> refs/remotes/origin/master
	$$ = term;
}
|
PE_NAME
{
<<<<<<< HEAD
	struct parse_events__term *term;

	ABORT_ON(parse_events__new_term(&term, PARSE_EVENTS__TERM_TYPE_NUM,
		 $1, NULL, 1));
=======
	struct parse_events_term *term;

	ABORT_ON(parse_events_term__num(&term, PARSE_EVENTS__TERM_TYPE_USER,
					$1, 1));
	$$ = term;
}
|
PE_VALUE_SYM_HW
{
	struct parse_events_term *term;
	int config = $1 & 255;

	ABORT_ON(parse_events_term__sym_hw(&term, NULL, config));
	$$ = term;
}
|
PE_TERM '=' PE_NAME
{
	struct parse_events_term *term;

	ABORT_ON(parse_events_term__str(&term, (int)$1, NULL, $3));
>>>>>>> refs/remotes/origin/master
	$$ = term;
}
|
PE_TERM '=' PE_VALUE
{
<<<<<<< HEAD
	struct parse_events__term *term;

	ABORT_ON(parse_events__new_term(&term, $1, NULL, NULL, $3));
=======
	struct parse_events_term *term;

	ABORT_ON(parse_events_term__num(&term, (int)$1, NULL, $3));
>>>>>>> refs/remotes/origin/master
	$$ = term;
}
|
PE_TERM
{
<<<<<<< HEAD
	struct parse_events__term *term;

	ABORT_ON(parse_events__new_term(&term, $1, NULL, NULL, 1));
=======
	struct parse_events_term *term;

	ABORT_ON(parse_events_term__num(&term, (int)$1, NULL, 1));
>>>>>>> refs/remotes/origin/master
	$$ = term;
}

sep_dc: ':' |

sep_slash_dc: '/' | ':' |

%%

<<<<<<< HEAD
void parse_events_error(struct list_head *list_all __used,
			struct list_head *list_event __used,
			int *idx __used,
			char const *msg __used)
=======
void parse_events_error(void *data __maybe_unused, void *scanner __maybe_unused,
			char const *msg __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
}
