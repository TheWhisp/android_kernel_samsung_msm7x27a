<<<<<<< HEAD
#include "../../../include/linux/hw_breakpoint.h"
=======
#include <linux/hw_breakpoint.h>
>>>>>>> refs/remotes/origin/master
#include "util.h"
#include "../perf.h"
#include "evlist.h"
#include "evsel.h"
#include "parse-options.h"
#include "parse-events.h"
#include "exec_cmd.h"
<<<<<<< HEAD
#include "string.h"
#include "symbol.h"
#include "cache.h"
#include "header.h"
#include "debugfs.h"
<<<<<<< HEAD
=======
#include "parse-events-flex.h"
#include "pmu.h"

#define MAX_NAME_LEN 100
>>>>>>> refs/remotes/origin/cm-10.0

struct event_symbol {
	u8		type;
	u64		config;
=======
#include "linux/string.h"
#include "symbol.h"
#include "cache.h"
#include "header.h"
#include <api/fs/debugfs.h>
#include "parse-events-bison.h"
#define YY_EXTRA_TYPE int
#include "parse-events-flex.h"
#include "pmu.h"
#include "thread_map.h"

#define MAX_NAME_LEN 100

struct event_symbol {
>>>>>>> refs/remotes/origin/master
	const char	*symbol;
	const char	*alias;
};

<<<<<<< HEAD
<<<<<<< HEAD
enum event_result {
	EVT_FAILED,
	EVT_HANDLED,
	EVT_HANDLED_ALL
};

char debugfs_path[MAXPATHLEN];
=======
int parse_events_parse(struct list_head *list, struct list_head *list_tmp,
		       int *idx);
>>>>>>> refs/remotes/origin/cm-10.0

#define CHW(x) .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_##x
#define CSW(x) .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_##x

static struct event_symbol event_symbols[] = {
  { CHW(CPU_CYCLES),			"cpu-cycles",			"cycles"		},
  { CHW(STALLED_CYCLES_FRONTEND),	"stalled-cycles-frontend",	"idle-cycles-frontend"	},
  { CHW(STALLED_CYCLES_BACKEND),	"stalled-cycles-backend",	"idle-cycles-backend"	},
  { CHW(INSTRUCTIONS),			"instructions",			""			},
  { CHW(CACHE_REFERENCES),		"cache-references",		""			},
  { CHW(CACHE_MISSES),			"cache-misses",			""			},
  { CHW(BRANCH_INSTRUCTIONS),		"branch-instructions",		"branches"		},
  { CHW(BRANCH_MISSES),			"branch-misses",		""			},
  { CHW(BUS_CYCLES),			"bus-cycles",			""			},
<<<<<<< HEAD
=======
  { CHW(REF_CPU_CYCLES),		"ref-cycles",			""			},
>>>>>>> refs/remotes/origin/cm-10.0

  { CSW(CPU_CLOCK),			"cpu-clock",			""			},
  { CSW(TASK_CLOCK),			"task-clock",			""			},
  { CSW(PAGE_FAULTS),			"page-faults",			"faults"		},
  { CSW(PAGE_FAULTS_MIN),		"minor-faults",			""			},
  { CSW(PAGE_FAULTS_MAJ),		"major-faults",			""			},
  { CSW(CONTEXT_SWITCHES),		"context-switches",		"cs"			},
  { CSW(CPU_MIGRATIONS),		"cpu-migrations",		"migrations"		},
  { CSW(ALIGNMENT_FAULTS),		"alignment-faults",		""			},
  { CSW(EMULATION_FAULTS),		"emulation-faults",		""			},
=======
#ifdef PARSER_DEBUG
extern int parse_events_debug;
#endif
int parse_events_parse(void *data, void *scanner);

static struct event_symbol event_symbols_hw[PERF_COUNT_HW_MAX] = {
	[PERF_COUNT_HW_CPU_CYCLES] = {
		.symbol = "cpu-cycles",
		.alias  = "cycles",
	},
	[PERF_COUNT_HW_INSTRUCTIONS] = {
		.symbol = "instructions",
		.alias  = "",
	},
	[PERF_COUNT_HW_CACHE_REFERENCES] = {
		.symbol = "cache-references",
		.alias  = "",
	},
	[PERF_COUNT_HW_CACHE_MISSES] = {
		.symbol = "cache-misses",
		.alias  = "",
	},
	[PERF_COUNT_HW_BRANCH_INSTRUCTIONS] = {
		.symbol = "branch-instructions",
		.alias  = "branches",
	},
	[PERF_COUNT_HW_BRANCH_MISSES] = {
		.symbol = "branch-misses",
		.alias  = "",
	},
	[PERF_COUNT_HW_BUS_CYCLES] = {
		.symbol = "bus-cycles",
		.alias  = "",
	},
	[PERF_COUNT_HW_STALLED_CYCLES_FRONTEND] = {
		.symbol = "stalled-cycles-frontend",
		.alias  = "idle-cycles-frontend",
	},
	[PERF_COUNT_HW_STALLED_CYCLES_BACKEND] = {
		.symbol = "stalled-cycles-backend",
		.alias  = "idle-cycles-backend",
	},
	[PERF_COUNT_HW_REF_CPU_CYCLES] = {
		.symbol = "ref-cycles",
		.alias  = "",
	},
};

static struct event_symbol event_symbols_sw[PERF_COUNT_SW_MAX] = {
	[PERF_COUNT_SW_CPU_CLOCK] = {
		.symbol = "cpu-clock",
		.alias  = "",
	},
	[PERF_COUNT_SW_TASK_CLOCK] = {
		.symbol = "task-clock",
		.alias  = "",
	},
	[PERF_COUNT_SW_PAGE_FAULTS] = {
		.symbol = "page-faults",
		.alias  = "faults",
	},
	[PERF_COUNT_SW_CONTEXT_SWITCHES] = {
		.symbol = "context-switches",
		.alias  = "cs",
	},
	[PERF_COUNT_SW_CPU_MIGRATIONS] = {
		.symbol = "cpu-migrations",
		.alias  = "migrations",
	},
	[PERF_COUNT_SW_PAGE_FAULTS_MIN] = {
		.symbol = "minor-faults",
		.alias  = "",
	},
	[PERF_COUNT_SW_PAGE_FAULTS_MAJ] = {
		.symbol = "major-faults",
		.alias  = "",
	},
	[PERF_COUNT_SW_ALIGNMENT_FAULTS] = {
		.symbol = "alignment-faults",
		.alias  = "",
	},
	[PERF_COUNT_SW_EMULATION_FAULTS] = {
		.symbol = "emulation-faults",
		.alias  = "",
	},
	[PERF_COUNT_SW_DUMMY] = {
		.symbol = "dummy",
		.alias  = "",
	},
>>>>>>> refs/remotes/origin/master
};

#define __PERF_EVENT_FIELD(config, name) \
	((config & PERF_EVENT_##name##_MASK) >> PERF_EVENT_##name##_SHIFT)

#define PERF_EVENT_RAW(config)		__PERF_EVENT_FIELD(config, RAW)
#define PERF_EVENT_CONFIG(config)	__PERF_EVENT_FIELD(config, CONFIG)
#define PERF_EVENT_TYPE(config)		__PERF_EVENT_FIELD(config, TYPE)
#define PERF_EVENT_ID(config)		__PERF_EVENT_FIELD(config, EVENT)

<<<<<<< HEAD
static const char *hw_event_names[PERF_COUNT_HW_MAX] = {
	"cycles",
	"instructions",
	"cache-references",
	"cache-misses",
	"branches",
	"branch-misses",
	"bus-cycles",
	"stalled-cycles-frontend",
	"stalled-cycles-backend",
<<<<<<< HEAD
=======
	"ref-cycles",
>>>>>>> refs/remotes/origin/cm-10.0
};

static const char *sw_event_names[PERF_COUNT_SW_MAX] = {
	"cpu-clock",
	"task-clock",
	"page-faults",
	"context-switches",
	"CPU-migrations",
	"minor-faults",
	"major-faults",
	"alignment-faults",
	"emulation-faults",
};

#define MAX_ALIASES 8

<<<<<<< HEAD
static const char *hw_cache[][MAX_ALIASES] = {
 { "L1-dcache",	"l1-d",		"l1d",		"L1-data",		},
 { "L1-icache",	"l1-i",		"l1i",		"L1-instruction",	},
 { "LLC",	"L2"							},
 { "dTLB",	"d-tlb",	"Data-TLB",				},
 { "iTLB",	"i-tlb",	"Instruction-TLB",			},
 { "branch",	"branches",	"bpu",		"btb",		"bpc",	},
};

static const char *hw_cache_op[][MAX_ALIASES] = {
=======
static const char *hw_cache[PERF_COUNT_HW_CACHE_MAX][MAX_ALIASES] = {
 { "L1-dcache",	"l1-d",		"l1d",		"L1-data",		},
 { "L1-icache",	"l1-i",		"l1i",		"L1-instruction",	},
 { "LLC",	"L2",							},
 { "dTLB",	"d-tlb",	"Data-TLB",				},
 { "iTLB",	"i-tlb",	"Instruction-TLB",			},
 { "branch",	"branches",	"bpu",		"btb",		"bpc",	},
 { "node",								},
};

static const char *hw_cache_op[PERF_COUNT_HW_CACHE_OP_MAX][MAX_ALIASES] = {
>>>>>>> refs/remotes/origin/cm-10.0
 { "load",	"loads",	"read",					},
 { "store",	"stores",	"write",				},
 { "prefetch",	"prefetches",	"speculative-read", "speculative-load",	},
};

<<<<<<< HEAD
static const char *hw_cache_result[][MAX_ALIASES] = {
=======
static const char *hw_cache_result[PERF_COUNT_HW_CACHE_RESULT_MAX]
				  [MAX_ALIASES] = {
>>>>>>> refs/remotes/origin/cm-10.0
 { "refs",	"Reference",	"ops",		"access",		},
 { "misses",	"miss",							},
};

#define C(x)		PERF_COUNT_HW_CACHE_##x
#define CACHE_READ	(1 << C(OP_READ))
#define CACHE_WRITE	(1 << C(OP_WRITE))
#define CACHE_PREFETCH	(1 << C(OP_PREFETCH))
#define COP(x)		(1 << x)

/*
 * cache operartion stat
 * L1I : Read and prefetch only
 * ITLB and BPU : Read-only
 */
static unsigned long hw_cache_stat[C(MAX)] = {
 [C(L1D)]	= (CACHE_READ | CACHE_WRITE | CACHE_PREFETCH),
 [C(L1I)]	= (CACHE_READ | CACHE_PREFETCH),
 [C(LL)]	= (CACHE_READ | CACHE_WRITE | CACHE_PREFETCH),
 [C(DTLB)]	= (CACHE_READ | CACHE_WRITE | CACHE_PREFETCH),
 [C(ITLB)]	= (CACHE_READ),
 [C(BPU)]	= (CACHE_READ),
<<<<<<< HEAD
=======
 [C(NODE)]	= (CACHE_READ | CACHE_WRITE | CACHE_PREFETCH),
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
>>>>>>> refs/remotes/origin/master
#define for_each_subsystem(sys_dir, sys_dirent, sys_next)	       \
	while (!readdir_r(sys_dir, &sys_dirent, &sys_next) && sys_next)	       \
	if (sys_dirent.d_type == DT_DIR &&				       \
	   (strcmp(sys_dirent.d_name, ".")) &&				       \
	   (strcmp(sys_dirent.d_name, "..")))

static int tp_event_has_id(struct dirent *sys_dir, struct dirent *evt_dir)
{
	char evt_path[MAXPATHLEN];
	int fd;

<<<<<<< HEAD
<<<<<<< HEAD
	snprintf(evt_path, MAXPATHLEN, "%s/%s/%s/id", debugfs_path,
=======
	snprintf(evt_path, MAXPATHLEN, "%s/%s/%s/id", tracing_events_path,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	snprintf(evt_path, MAXPATHLEN, "%s/%s/%s/id", tracing_events_path,
>>>>>>> refs/remotes/origin/master
			sys_dir->d_name, evt_dir->d_name);
	fd = open(evt_path, O_RDONLY);
	if (fd < 0)
		return -EINVAL;
	close(fd);

	return 0;
}

#define for_each_event(sys_dirent, evt_dir, evt_dirent, evt_next)	       \
	while (!readdir_r(evt_dir, &evt_dirent, &evt_next) && evt_next)        \
	if (evt_dirent.d_type == DT_DIR &&				       \
	   (strcmp(evt_dirent.d_name, ".")) &&				       \
	   (strcmp(evt_dirent.d_name, "..")) &&				       \
	   (!tp_event_has_id(&sys_dirent, &evt_dirent)))

#define MAX_EVENT_LENGTH 512


struct tracepoint_path *tracepoint_id_to_path(u64 config)
{
	struct tracepoint_path *path = NULL;
	DIR *sys_dir, *evt_dir;
	struct dirent *sys_next, *evt_next, sys_dirent, evt_dirent;
<<<<<<< HEAD
<<<<<<< HEAD
	char id_buf[4];
=======
	char id_buf[24];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	char id_buf[24];
>>>>>>> refs/remotes/origin/master
	int fd;
	u64 id;
	char evt_path[MAXPATHLEN];
	char dir_path[MAXPATHLEN];

<<<<<<< HEAD
<<<<<<< HEAD
	if (debugfs_valid_mountpoint(debugfs_path))
		return NULL;

	sys_dir = opendir(debugfs_path);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (debugfs_valid_mountpoint(tracing_events_path))
		return NULL;

	sys_dir = opendir(tracing_events_path);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!sys_dir)
		return NULL;

	for_each_subsystem(sys_dir, sys_dirent, sys_next) {

<<<<<<< HEAD
<<<<<<< HEAD
		snprintf(dir_path, MAXPATHLEN, "%s/%s", debugfs_path,
=======
		snprintf(dir_path, MAXPATHLEN, "%s/%s", tracing_events_path,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		snprintf(dir_path, MAXPATHLEN, "%s/%s", tracing_events_path,
>>>>>>> refs/remotes/origin/master
			 sys_dirent.d_name);
		evt_dir = opendir(dir_path);
		if (!evt_dir)
			continue;

		for_each_event(sys_dirent, evt_dir, evt_dirent, evt_next) {

			snprintf(evt_path, MAXPATHLEN, "%s/%s/id", dir_path,
				 evt_dirent.d_name);
			fd = open(evt_path, O_RDONLY);
			if (fd < 0)
				continue;
			if (read(fd, id_buf, sizeof(id_buf)) < 0) {
				close(fd);
				continue;
			}
			close(fd);
			id = atoll(id_buf);
			if (id == config) {
				closedir(evt_dir);
				closedir(sys_dir);
				path = zalloc(sizeof(*path));
				path->system = malloc(MAX_EVENT_LENGTH);
				if (!path->system) {
					free(path);
					return NULL;
				}
				path->name = malloc(MAX_EVENT_LENGTH);
				if (!path->name) {
<<<<<<< HEAD
					free(path->system);
=======
					zfree(&path->system);
>>>>>>> refs/remotes/origin/master
					free(path);
					return NULL;
				}
				strncpy(path->system, sys_dirent.d_name,
					MAX_EVENT_LENGTH);
				strncpy(path->name, evt_dirent.d_name,
					MAX_EVENT_LENGTH);
				return path;
			}
		}
		closedir(evt_dir);
	}

	closedir(sys_dir);
	return NULL;
}

<<<<<<< HEAD
#define TP_PATH_LEN (MAX_EVENT_LENGTH * 2 + 1)
static const char *tracepoint_id_to_name(u64 config)
{
	static char buf[TP_PATH_LEN];
	struct tracepoint_path *path;

	path = tracepoint_id_to_path(config);
	if (path) {
		snprintf(buf, TP_PATH_LEN, "%s:%s", path->system, path->name);
		free(path->name);
		free(path->system);
		free(path);
	} else
		snprintf(buf, TP_PATH_LEN, "%s:%s", "unknown", "unknown");

	return buf;
}

static int is_cache_op_valid(u8 cache_type, u8 cache_op)
{
	if (hw_cache_stat[cache_type] & COP(cache_op))
		return 1;	/* valid */
	else
		return 0;	/* invalid */
}

static char *event_cache_name(u8 cache_type, u8 cache_op, u8 cache_result)
{
	static char name[50];

	if (cache_result) {
		sprintf(name, "%s-%s-%s", hw_cache[cache_type][0],
			hw_cache_op[cache_op][0],
			hw_cache_result[cache_result][0]);
	} else {
		sprintf(name, "%s-%s", hw_cache[cache_type][0],
			hw_cache_op[cache_op][1]);
	}

	return name;
=======
struct tracepoint_path *tracepoint_name_to_path(const char *name)
{
	struct tracepoint_path *path = zalloc(sizeof(*path));
	char *str = strchr(name, ':');

	if (path == NULL || str == NULL) {
		free(path);
		return NULL;
	}

	path->system = strndup(name, str - name);
	path->name = strdup(str+1);

	if (path->system == NULL || path->name == NULL) {
		zfree(&path->system);
		zfree(&path->name);
		free(path);
		path = NULL;
	}

	return path;
>>>>>>> refs/remotes/origin/master
}

const char *event_type(int type)
{
	switch (type) {
	case PERF_TYPE_HARDWARE:
		return "hardware";

	case PERF_TYPE_SOFTWARE:
		return "software";

	case PERF_TYPE_TRACEPOINT:
		return "tracepoint";

	case PERF_TYPE_HW_CACHE:
		return "hardware-cache";

	default:
		break;
	}

	return "unknown";
}

<<<<<<< HEAD
const char *event_name(struct perf_evsel *evsel)
{
	u64 config = evsel->attr.config;
	int type = evsel->attr.type;

	if (evsel->name)
		return evsel->name;

	return __event_name(type, config);
}

const char *__event_name(int type, u64 config)
{
	static char buf[32];

	if (type == PERF_TYPE_RAW) {
		sprintf(buf, "raw 0x%" PRIx64, config);
		return buf;
	}

	switch (type) {
	case PERF_TYPE_HARDWARE:
		if (config < PERF_COUNT_HW_MAX && hw_event_names[config])
			return hw_event_names[config];
		return "unknown-hardware";

	case PERF_TYPE_HW_CACHE: {
		u8 cache_type, cache_op, cache_result;

		cache_type   = (config >>  0) & 0xff;
		if (cache_type > PERF_COUNT_HW_CACHE_MAX)
			return "unknown-ext-hardware-cache-type";

		cache_op     = (config >>  8) & 0xff;
		if (cache_op > PERF_COUNT_HW_CACHE_OP_MAX)
			return "unknown-ext-hardware-cache-op";

		cache_result = (config >> 16) & 0xff;
		if (cache_result > PERF_COUNT_HW_CACHE_RESULT_MAX)
			return "unknown-ext-hardware-cache-result";

		if (!is_cache_op_valid(cache_type, cache_op))
			return "invalid-cache";

		return event_cache_name(cache_type, cache_op, cache_result);
	}

	case PERF_TYPE_SOFTWARE:
		if (config < PERF_COUNT_SW_MAX && sw_event_names[config])
			return sw_event_names[config];
		return "unknown-software";

	case PERF_TYPE_TRACEPOINT:
		return tracepoint_id_to_name(config);

	default:
		break;
	}

	return "unknown";
}

<<<<<<< HEAD
static int parse_aliases(const char **str, const char *names[][MAX_ALIASES], int size)
=======
static int add_event(struct list_head *list, int *idx,
		     struct perf_event_attr *attr, char *name)
=======


static struct perf_evsel *
__add_event(struct list_head *list, int *idx,
	    struct perf_event_attr *attr,
	    char *name, struct cpu_map *cpus)
>>>>>>> refs/remotes/origin/master
{
	struct perf_evsel *evsel;

	event_attr_init(attr);

<<<<<<< HEAD
	evsel = perf_evsel__new(attr, (*idx)++);
	if (!evsel)
		return -ENOMEM;

	list_add_tail(&evsel->node, list);

	evsel->name = strdup(name);
	return 0;
}

static int parse_aliases(char *str, const char *names[][MAX_ALIASES], int size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	evsel = perf_evsel__new_idx(attr, (*idx)++);
	if (!evsel)
		return NULL;

	evsel->cpus = cpus;
	if (name)
		evsel->name = strdup(name);
	list_add_tail(&evsel->node, list);
	return evsel;
}

static int add_event(struct list_head *list, int *idx,
		     struct perf_event_attr *attr, char *name)
{
	return __add_event(list, idx, attr, name, NULL) ? 0 : -ENOMEM;
}

static int parse_aliases(char *str, const char *names[][PERF_EVSEL__MAX_ALIASES], int size)
>>>>>>> refs/remotes/origin/master
{
	int i, j;
	int n, longest = -1;

	for (i = 0; i < size; i++) {
<<<<<<< HEAD
		for (j = 0; j < MAX_ALIASES && names[i][j]; j++) {
			n = strlen(names[i][j]);
<<<<<<< HEAD
			if (n > longest && !strncasecmp(*str, names[i][j], n))
				longest = n;
		}
		if (longest > 0) {
			*str += longest;
			return i;
		}
=======
=======
		for (j = 0; j < PERF_EVSEL__MAX_ALIASES && names[i][j]; j++) {
			n = strlen(names[i][j]);
>>>>>>> refs/remotes/origin/master
			if (n > longest && !strncasecmp(str, names[i][j], n))
				longest = n;
		}
		if (longest > 0)
			return i;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return -1;
}

<<<<<<< HEAD
<<<<<<< HEAD
static enum event_result
parse_generic_hw_event(const char **str, struct perf_event_attr *attr)
{
	const char *s = *str;
	int cache_type = -1, cache_op = -1, cache_result = -1;

	cache_type = parse_aliases(&s, hw_cache, PERF_COUNT_HW_CACHE_MAX);
=======
=======
>>>>>>> refs/remotes/origin/master
int parse_events_add_cache(struct list_head *list, int *idx,
			   char *type, char *op_result1, char *op_result2)
{
	struct perf_event_attr attr;
	char name[MAX_NAME_LEN];
	int cache_type = -1, cache_op = -1, cache_result = -1;
	char *op_result[2] = { op_result1, op_result2 };
	int i, n;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * No fallback - if we cannot get a clear cache type
	 * then bail out:
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (cache_type == -1)
		return EVT_FAILED;

	while ((cache_op == -1 || cache_result == -1) && *s == '-') {
		++s;

		if (cache_op == -1) {
			cache_op = parse_aliases(&s, hw_cache_op,
						PERF_COUNT_HW_CACHE_OP_MAX);
			if (cache_op >= 0) {
				if (!is_cache_op_valid(cache_type, cache_op))
					return 0;
=======
	cache_type = parse_aliases(type, hw_cache,
=======
	cache_type = parse_aliases(type, perf_evsel__hw_cache,
>>>>>>> refs/remotes/origin/master
				   PERF_COUNT_HW_CACHE_MAX);
	if (cache_type == -1)
		return -EINVAL;

	n = snprintf(name, MAX_NAME_LEN, "%s", type);

	for (i = 0; (i < 2) && (op_result[i]); i++) {
		char *str = op_result[i];

<<<<<<< HEAD
		snprintf(name + n, MAX_NAME_LEN - n, "-%s\n", str);

		if (cache_op == -1) {
			cache_op = parse_aliases(str, hw_cache_op,
						 PERF_COUNT_HW_CACHE_OP_MAX);
			if (cache_op >= 0) {
				if (!is_cache_op_valid(cache_type, cache_op))
					return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		n += snprintf(name + n, MAX_NAME_LEN - n, "-%s", str);

		if (cache_op == -1) {
			cache_op = parse_aliases(str, perf_evsel__hw_cache_op,
						 PERF_COUNT_HW_CACHE_OP_MAX);
			if (cache_op >= 0) {
				if (!perf_evsel__is_cache_op_valid(cache_type, cache_op))
					return -EINVAL;
>>>>>>> refs/remotes/origin/master
				continue;
			}
		}

		if (cache_result == -1) {
<<<<<<< HEAD
<<<<<<< HEAD
			cache_result = parse_aliases(&s, hw_cache_result,
=======
			cache_result = parse_aliases(str, hw_cache_result,
>>>>>>> refs/remotes/origin/cm-10.0
						PERF_COUNT_HW_CACHE_RESULT_MAX);
			if (cache_result >= 0)
				continue;
		}
<<<<<<< HEAD

		/*
		 * Can't parse this as a cache op or result, so back up
		 * to the '-'.
		 */
		--s;
		break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
			cache_result = parse_aliases(str, perf_evsel__hw_cache_result,
						     PERF_COUNT_HW_CACHE_RESULT_MAX);
			if (cache_result >= 0)
				continue;
		}
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Fall back to reads:
	 */
	if (cache_op == -1)
		cache_op = PERF_COUNT_HW_CACHE_OP_READ;

	/*
	 * Fall back to accesses:
	 */
	if (cache_result == -1)
		cache_result = PERF_COUNT_HW_CACHE_RESULT_ACCESS;

<<<<<<< HEAD
<<<<<<< HEAD
	attr->config = cache_type | (cache_op << 8) | (cache_result << 16);
	attr->type = PERF_TYPE_HW_CACHE;

	*str = s;
	return EVT_HANDLED;
}

static enum event_result
parse_single_tracepoint_event(char *sys_name,
			      const char *evt_name,
			      unsigned int evt_length,
			      struct perf_event_attr *attr,
			      const char **strp)
{
=======
=======
>>>>>>> refs/remotes/origin/master
	memset(&attr, 0, sizeof(attr));
	attr.config = cache_type | (cache_op << 8) | (cache_result << 16);
	attr.type = PERF_TYPE_HW_CACHE;
	return add_event(list, idx, &attr, name);
}

static int add_tracepoint(struct list_head *list, int *idx,
			  char *sys_name, char *evt_name)
{
<<<<<<< HEAD
	struct perf_event_attr attr;
	char name[MAX_NAME_LEN];
>>>>>>> refs/remotes/origin/cm-10.0
	char evt_path[MAXPATHLEN];
	char id_buf[4];
	u64 id;
	int fd;

<<<<<<< HEAD
	snprintf(evt_path, MAXPATHLEN, "%s/%s/%s/id", debugfs_path,
=======
	snprintf(evt_path, MAXPATHLEN, "%s/%s/%s/id", tracing_events_path,
>>>>>>> refs/remotes/origin/cm-10.0
		 sys_name, evt_name);

	fd = open(evt_path, O_RDONLY);
	if (fd < 0)
<<<<<<< HEAD
		return EVT_FAILED;

	if (read(fd, id_buf, sizeof(id_buf)) < 0) {
		close(fd);
		return EVT_FAILED;
=======
		return -1;

	if (read(fd, id_buf, sizeof(id_buf)) < 0) {
		close(fd);
		return -1;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	close(fd);
	id = atoll(id_buf);
<<<<<<< HEAD
	attr->config = id;
	attr->type = PERF_TYPE_TRACEPOINT;
	*strp += strlen(sys_name) + evt_length + 1; /* + 1 for the ':' */

	attr->sample_type |= PERF_SAMPLE_RAW;
	attr->sample_type |= PERF_SAMPLE_TIME;
	attr->sample_type |= PERF_SAMPLE_CPU;

	attr->sample_period = 1;


	return EVT_HANDLED;
}

/* sys + ':' + event + ':' + flags*/
#define MAX_EVOPT_LEN	(MAX_EVENT_LENGTH * 2 + 2 + 128)
static enum event_result
parse_multiple_tracepoint_event(const struct option *opt, char *sys_name,
				const char *evt_exp, char *flags)
=======

	memset(&attr, 0, sizeof(attr));
	attr.config = id;
	attr.type = PERF_TYPE_TRACEPOINT;
	attr.sample_type |= PERF_SAMPLE_RAW;
	attr.sample_type |= PERF_SAMPLE_TIME;
	attr.sample_type |= PERF_SAMPLE_CPU;
	attr.sample_period = 1;

	snprintf(name, MAX_NAME_LEN, "%s:%s", sys_name, evt_name);
	return add_event(list, idx, &attr, name);
}

static int add_tracepoint_multi(struct list_head *list, int *idx,
				char *sys_name, char *evt_name)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct perf_evsel *evsel;

	evsel = perf_evsel__newtp_idx(sys_name, evt_name, (*idx)++);
	if (!evsel)
		return -ENOMEM;

	list_add_tail(&evsel->node, list);

	return 0;
}

static int add_tracepoint_multi_event(struct list_head *list, int *idx,
				      char *sys_name, char *evt_name)
>>>>>>> refs/remotes/origin/master
{
	char evt_path[MAXPATHLEN];
	struct dirent *evt_ent;
	DIR *evt_dir;
<<<<<<< HEAD
<<<<<<< HEAD

	snprintf(evt_path, MAXPATHLEN, "%s/%s", debugfs_path, sys_name);
	evt_dir = opendir(evt_path);

	if (!evt_dir) {
		perror("Can't open event dir");
		return EVT_FAILED;
	}

	while ((evt_ent = readdir(evt_dir))) {
		char event_opt[MAX_EVOPT_LEN + 1];
		int len;

=======
=======
>>>>>>> refs/remotes/origin/master
	int ret = 0;

	snprintf(evt_path, MAXPATHLEN, "%s/%s", tracing_events_path, sys_name);
	evt_dir = opendir(evt_path);
	if (!evt_dir) {
		perror("Can't open event dir");
		return -1;
	}

	while (!ret && (evt_ent = readdir(evt_dir))) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (!strcmp(evt_ent->d_name, ".")
		    || !strcmp(evt_ent->d_name, "..")
		    || !strcmp(evt_ent->d_name, "enable")
		    || !strcmp(evt_ent->d_name, "filter"))
			continue;

<<<<<<< HEAD
<<<<<<< HEAD
		if (!strglobmatch(evt_ent->d_name, evt_exp))
			continue;

		len = snprintf(event_opt, MAX_EVOPT_LEN, "%s:%s%s%s", sys_name,
			       evt_ent->d_name, flags ? ":" : "",
			       flags ?: "");
		if (len < 0)
			return EVT_FAILED;

		if (parse_events(opt, event_opt, 0))
			return EVT_FAILED;
	}

	return EVT_HANDLED_ALL;
}

static enum event_result
parse_tracepoint_event(const struct option *opt, const char **strp,
		       struct perf_event_attr *attr)
{
	const char *evt_name;
	char *flags = NULL, *comma_loc;
	char sys_name[MAX_EVENT_LENGTH];
	unsigned int sys_length, evt_length;

	if (debugfs_valid_mountpoint(debugfs_path))
		return 0;

	evt_name = strchr(*strp, ':');
	if (!evt_name)
		return EVT_FAILED;

	sys_length = evt_name - *strp;
	if (sys_length >= MAX_EVENT_LENGTH)
		return 0;

	strncpy(sys_name, *strp, sys_length);
	sys_name[sys_length] = '\0';
	evt_name = evt_name + 1;

	comma_loc = strchr(evt_name, ',');
	if (comma_loc) {
		/* take the event name up to the comma */
		evt_name = strndup(evt_name, comma_loc - evt_name);
	}
	flags = strchr(evt_name, ':');
	if (flags) {
		/* split it out: */
		evt_name = strndup(evt_name, flags - evt_name);
		flags++;
	}

	evt_length = strlen(evt_name);
	if (evt_length >= MAX_EVENT_LENGTH)
		return EVT_FAILED;
	if (strpbrk(evt_name, "*?")) {
		*strp += strlen(sys_name) + evt_length + 1; /* 1 == the ':' */
		return parse_multiple_tracepoint_event(opt, sys_name, evt_name,
						       flags);
	} else {
		return parse_single_tracepoint_event(sys_name, evt_name,
						     evt_length, attr, strp);
	}
}

static enum event_result
parse_breakpoint_type(const char *type, const char **strp,
		      struct perf_event_attr *attr)
=======
		if (!strglobmatch(evt_ent->d_name, evt_name))
			continue;

		ret = add_tracepoint(list, idx, sys_name, evt_ent->d_name);
	}

=======
		if (!strglobmatch(evt_ent->d_name, evt_name))
			continue;

		ret = add_tracepoint(list, idx, sys_name, evt_ent->d_name);
	}

	closedir(evt_dir);
	return ret;
}

static int add_tracepoint_event(struct list_head *list, int *idx,
				char *sys_name, char *evt_name)
{
	return strpbrk(evt_name, "*?") ?
	       add_tracepoint_multi_event(list, idx, sys_name, evt_name) :
	       add_tracepoint(list, idx, sys_name, evt_name);
}

static int add_tracepoint_multi_sys(struct list_head *list, int *idx,
				    char *sys_name, char *evt_name)
{
	struct dirent *events_ent;
	DIR *events_dir;
	int ret = 0;

	events_dir = opendir(tracing_events_path);
	if (!events_dir) {
		perror("Can't open event dir");
		return -1;
	}

	while (!ret && (events_ent = readdir(events_dir))) {
		if (!strcmp(events_ent->d_name, ".")
		    || !strcmp(events_ent->d_name, "..")
		    || !strcmp(events_ent->d_name, "enable")
		    || !strcmp(events_ent->d_name, "header_event")
		    || !strcmp(events_ent->d_name, "header_page"))
			continue;

		if (!strglobmatch(events_ent->d_name, sys_name))
			continue;

		ret = add_tracepoint_event(list, idx, events_ent->d_name,
					   evt_name);
	}

	closedir(events_dir);
>>>>>>> refs/remotes/origin/master
	return ret;
}

int parse_events_add_tracepoint(struct list_head *list, int *idx,
				char *sys, char *event)
{
	int ret;

	ret = debugfs_valid_mountpoint(tracing_events_path);
	if (ret)
		return ret;

<<<<<<< HEAD
	return strpbrk(event, "*?") ?
	       add_tracepoint_multi(list, idx, sys, event) :
	       add_tracepoint(list, idx, sys, event);
=======
	if (strpbrk(sys, "*?"))
		return add_tracepoint_multi_sys(list, idx, sys, event);
	else
		return add_tracepoint_event(list, idx, sys, event);
>>>>>>> refs/remotes/origin/master
}

static int
parse_breakpoint_type(const char *type, struct perf_event_attr *attr)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	int i;

	for (i = 0; i < 3; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (!type[i])
=======
		if (!type || !type[i])
>>>>>>> refs/remotes/origin/cm-10.0
			break;

		switch (type[i]) {
		case 'r':
			attr->bp_type |= HW_BREAKPOINT_R;
			break;
		case 'w':
			attr->bp_type |= HW_BREAKPOINT_W;
			break;
		case 'x':
			attr->bp_type |= HW_BREAKPOINT_X;
			break;
		default:
<<<<<<< HEAD
			return EVT_FAILED;
		}
	}
	if (!attr->bp_type) /* Default */
		attr->bp_type = HW_BREAKPOINT_R | HW_BREAKPOINT_W;

	*strp = type + i;

	return EVT_HANDLED;
}

static enum event_result
parse_breakpoint_event(const char **strp, struct perf_event_attr *attr)
{
	const char *target;
	const char *type;
	char *endaddr;
	u64 addr;
	enum event_result err;

	target = strchr(*strp, ':');
	if (!target)
		return EVT_FAILED;

	if (strncmp(*strp, "mem", target - *strp) != 0)
		return EVT_FAILED;

	target++;

	addr = strtoull(target, &endaddr, 0);
	if (target == endaddr)
		return EVT_FAILED;

	attr->bp_addr = addr;
	*strp = endaddr;

	type = strchr(target, ':');

	/* If no type is defined, just rw as default */
	if (!type) {
		attr->bp_type = HW_BREAKPOINT_R | HW_BREAKPOINT_W;
	} else {
		err = parse_breakpoint_type(++type, strp, attr);
		if (err == EVT_FAILED)
			return EVT_FAILED;
	}
=======
=======
		if (!type || !type[i])
			break;

#define CHECK_SET_TYPE(bit)		\
do {					\
	if (attr->bp_type & bit)	\
		return -EINVAL;		\
	else				\
		attr->bp_type |= bit;	\
} while (0)

		switch (type[i]) {
		case 'r':
			CHECK_SET_TYPE(HW_BREAKPOINT_R);
			break;
		case 'w':
			CHECK_SET_TYPE(HW_BREAKPOINT_W);
			break;
		case 'x':
			CHECK_SET_TYPE(HW_BREAKPOINT_X);
			break;
		default:
>>>>>>> refs/remotes/origin/master
			return -EINVAL;
		}
	}

<<<<<<< HEAD
=======
#undef CHECK_SET_TYPE

>>>>>>> refs/remotes/origin/master
	if (!attr->bp_type) /* Default */
		attr->bp_type = HW_BREAKPOINT_R | HW_BREAKPOINT_W;

	return 0;
}

int parse_events_add_breakpoint(struct list_head *list, int *idx,
				void *ptr, char *type)
{
	struct perf_event_attr attr;
<<<<<<< HEAD
	char name[MAX_NAME_LEN];
=======
>>>>>>> refs/remotes/origin/master

	memset(&attr, 0, sizeof(attr));
	attr.bp_addr = (unsigned long) ptr;

	if (parse_breakpoint_type(type, &attr))
		return -EINVAL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * We should find a nice way to override the access length
	 * Provide some defaults for now
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (attr->bp_type == HW_BREAKPOINT_X)
		attr->bp_len = sizeof(long);
	else
		attr->bp_len = HW_BREAKPOINT_LEN_4;

	attr->type = PERF_TYPE_BREAKPOINT;

	return EVT_HANDLED;
}

static int check_events(const char *str, unsigned int i)
{
	int n;

	n = strlen(event_symbols[i].symbol);
	if (!strncasecmp(str, event_symbols[i].symbol, n))
		return n;

	n = strlen(event_symbols[i].alias);
	if (n) {
		if (!strncasecmp(str, event_symbols[i].alias, n))
			return n;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
	if (attr.bp_type == HW_BREAKPOINT_X)
		attr.bp_len = sizeof(long);
	else
		attr.bp_len = HW_BREAKPOINT_LEN_4;

	attr.type = PERF_TYPE_BREAKPOINT;
<<<<<<< HEAD

	snprintf(name, MAX_NAME_LEN, "mem:%p:%s", ptr, type ? type : "rw");
	return add_event(list, idx, &attr, name);
}

static int config_term(struct perf_event_attr *attr,
		       struct parse_events__term *term)
{
	switch (term->type) {
	case PARSE_EVENTS__TERM_TYPE_CONFIG:
		attr->config = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_CONFIG1:
		attr->config1 = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_CONFIG2:
		attr->config2 = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
=======
	attr.sample_period = 1;

	return add_event(list, idx, &attr, NULL);
}

static int config_term(struct perf_event_attr *attr,
		       struct parse_events_term *term)
{
#define CHECK_TYPE_VAL(type)					\
do {								\
	if (PARSE_EVENTS__TERM_TYPE_ ## type != term->type_val)	\
		return -EINVAL;					\
} while (0)

	switch (term->type_term) {
	case PARSE_EVENTS__TERM_TYPE_CONFIG:
		CHECK_TYPE_VAL(NUM);
		attr->config = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_CONFIG1:
		CHECK_TYPE_VAL(NUM);
		attr->config1 = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_CONFIG2:
		CHECK_TYPE_VAL(NUM);
		attr->config2 = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
		CHECK_TYPE_VAL(NUM);
>>>>>>> refs/remotes/origin/master
		attr->sample_period = term->val.num;
		break;
	case PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE:
		/*
		 * TODO uncomment when the field is available
		 * attr->branch_sample_type = term->val.num;
		 */
		break;
<<<<<<< HEAD
	default:
		return -EINVAL;
	}
	return 0;
=======
	case PARSE_EVENTS__TERM_TYPE_NAME:
		CHECK_TYPE_VAL(STR);
		break;
	default:
		return -EINVAL;
	}

	return 0;
#undef CHECK_TYPE_VAL
>>>>>>> refs/remotes/origin/master
}

static int config_attr(struct perf_event_attr *attr,
		       struct list_head *head, int fail)
{
<<<<<<< HEAD
	struct parse_events__term *term;
=======
	struct parse_events_term *term;
>>>>>>> refs/remotes/origin/master

	list_for_each_entry(term, head, list)
		if (config_term(attr, term) && fail)
			return -EINVAL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static enum event_result
parse_symbolic_event(const char **strp, struct perf_event_attr *attr)
{
	const char *str = *strp;
	unsigned int i;
	int n;

	for (i = 0; i < ARRAY_SIZE(event_symbols); i++) {
		n = check_events(str, i);
		if (n > 0) {
			attr->type = event_symbols[i].type;
			attr->config = event_symbols[i].config;
			*strp = str + n;
			return EVT_HANDLED;
		}
	}
	return EVT_FAILED;
}

static enum event_result
parse_raw_event(const char **strp, struct perf_event_attr *attr)
{
	const char *str = *strp;
	u64 config;
	int n;

	if (*str != 'r')
		return EVT_FAILED;
	n = hex2u64(str + 1, &config);
	if (n > 0) {
		*strp = str + n + 1;
		attr->type = PERF_TYPE_RAW;
		attr->config = config;
		return EVT_HANDLED;
	}
	return EVT_FAILED;
}

static enum event_result
parse_numeric_event(const char **strp, struct perf_event_attr *attr)
{
	const char *str = *strp;
	char *endp;
	unsigned long type;
	u64 config;

	type = strtoul(str, &endp, 0);
	if (endp > str && type < PERF_TYPE_MAX && *endp == ':') {
		str = endp + 1;
		config = strtoul(str, &endp, 0);
		if (endp > str) {
			attr->type = type;
			attr->config = config;
			*strp = endp;
			return EVT_HANDLED;
		}
	}
	return EVT_FAILED;
}

static int
parse_event_modifier(const char **strp, struct perf_event_attr *attr)
{
	const char *str = *strp;
	int exclude = 0;
	int eu = 0, ek = 0, eh = 0, precise = 0;

	if (!*str)
		return 0;

	if (*str == ',')
		return 0;

	if (*str++ != ':')
		return -1;

=======
int parse_events_add_numeric(struct list_head *list, int *idx,
			     unsigned long type, unsigned long config,
=======
int parse_events_add_numeric(struct list_head *list, int *idx,
			     u32 type, u64 config,
>>>>>>> refs/remotes/origin/master
			     struct list_head *head_config)
{
	struct perf_event_attr attr;

	memset(&attr, 0, sizeof(attr));
	attr.type = type;
	attr.config = config;

	if (head_config &&
	    config_attr(&attr, head_config, 1))
		return -EINVAL;

<<<<<<< HEAD
	return add_event(list, idx, &attr,
			 (char *) __event_name(type, config));
=======
	return add_event(list, idx, &attr, NULL);
}

static int parse_events__is_name_term(struct parse_events_term *term)
{
	return term->type_term == PARSE_EVENTS__TERM_TYPE_NAME;
}

static char *pmu_event_name(struct list_head *head_terms)
{
	struct parse_events_term *term;

	list_for_each_entry(term, head_terms, list)
		if (parse_events__is_name_term(term))
			return term->val.str;

	return NULL;
>>>>>>> refs/remotes/origin/master
}

int parse_events_add_pmu(struct list_head *list, int *idx,
			 char *name, struct list_head *head_config)
{
	struct perf_event_attr attr;
	struct perf_pmu *pmu;
<<<<<<< HEAD
=======
	struct perf_evsel *evsel;
	char *unit;
	double scale;
>>>>>>> refs/remotes/origin/master

	pmu = perf_pmu__find(name);
	if (!pmu)
		return -EINVAL;

	memset(&attr, 0, sizeof(attr));

<<<<<<< HEAD
=======
	if (perf_pmu__check_alias(pmu, head_config, &unit, &scale))
		return -EINVAL;

>>>>>>> refs/remotes/origin/master
	/*
	 * Configure hardcoded terms first, no need to check
	 * return value when called with fail == 0 ;)
	 */
	config_attr(&attr, head_config, 0);

	if (perf_pmu__config(pmu, &attr, head_config))
		return -EINVAL;

<<<<<<< HEAD
	return add_event(list, idx, &attr, (char *) "pmu");
}

=======
	evsel = __add_event(list, idx, &attr, pmu_event_name(head_config),
			    pmu->cpus);
	if (evsel) {
		evsel->unit = unit;
		evsel->scale = scale;
	}

	return evsel ? 0 : -ENOMEM;
}

int parse_events__modifier_group(struct list_head *list,
				 char *event_mod)
{
	return parse_events__modifier_event(list, event_mod, true);
}

void parse_events__set_leader(char *name, struct list_head *list)
{
	struct perf_evsel *leader;

	__perf_evlist__set_leader(list);
	leader = list_entry(list->next, struct perf_evsel, node);
	leader->group_name = name ? strdup(name) : NULL;
}

/* list_event is assumed to point to malloc'ed memory */
>>>>>>> refs/remotes/origin/master
void parse_events_update_lists(struct list_head *list_event,
			       struct list_head *list_all)
{
	/*
	 * Called for single event definition. Update the
<<<<<<< HEAD
	 * 'all event' list, and reinit the 'signle event'
	 * list, for next event definition.
	 */
	list_splice_tail(list_event, list_all);
	INIT_LIST_HEAD(list_event);
}

int parse_events_modifier(struct list_head *list, char *str)
{
	struct perf_evsel *evsel;
	int exclude = 0, exclude_GH = 0;
	int eu = 0, ek = 0, eh = 0, eH = 0, eG = 0, precise = 0;

	if (str == NULL)
		return 0;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * 'all event' list, and reinit the 'single event'
	 * list, for next event definition.
	 */
	list_splice_tail(list_event, list_all);
	free(list_event);
}

struct event_modifier {
	int eu;
	int ek;
	int eh;
	int eH;
	int eG;
	int precise;
	int exclude_GH;
	int sample_read;
	int pinned;
};

static int get_event_modifier(struct event_modifier *mod, char *str,
			       struct perf_evsel *evsel)
{
	int eu = evsel ? evsel->attr.exclude_user : 0;
	int ek = evsel ? evsel->attr.exclude_kernel : 0;
	int eh = evsel ? evsel->attr.exclude_hv : 0;
	int eH = evsel ? evsel->attr.exclude_host : 0;
	int eG = evsel ? evsel->attr.exclude_guest : 0;
	int precise = evsel ? evsel->attr.precise_ip : 0;
	int sample_read = 0;
	int pinned = evsel ? evsel->attr.pinned : 0;

	int exclude = eu | ek | eh;
	int exclude_GH = evsel ? evsel->exclude_GH : 0;

	memset(mod, 0, sizeof(*mod));

>>>>>>> refs/remotes/origin/master
	while (*str) {
		if (*str == 'u') {
			if (!exclude)
				exclude = eu = ek = eh = 1;
			eu = 0;
		} else if (*str == 'k') {
			if (!exclude)
				exclude = eu = ek = eh = 1;
			ek = 0;
		} else if (*str == 'h') {
			if (!exclude)
				exclude = eu = ek = eh = 1;
			eh = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
		} else if (*str == 'G') {
			if (!exclude_GH)
				exclude_GH = eG = eH = 1;
			eG = 0;
		} else if (*str == 'H') {
			if (!exclude_GH)
				exclude_GH = eG = eH = 1;
			eH = 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		} else if (*str == 'p') {
			precise++;
=======
		} else if (*str == 'p') {
			precise++;
			/* use of precise requires exclude_guest */
			if (!exclude_GH)
				eG = 1;
		} else if (*str == 'S') {
			sample_read = 1;
		} else if (*str == 'D') {
			pinned = 1;
>>>>>>> refs/remotes/origin/master
		} else
			break;

		++str;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	if (str < *strp + 2)
		return -1;

	*strp = str;

	attr->exclude_user   = eu;
	attr->exclude_kernel = ek;
	attr->exclude_hv     = eh;
	attr->precise_ip     = precise;
=======
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * precise ip:
	 *
	 *  0 - SAMPLE_IP can have arbitrary skid
	 *  1 - SAMPLE_IP must have constant skid
	 *  2 - SAMPLE_IP requested to have 0 skid
	 *  3 - SAMPLE_IP must have 0 skid
	 *
	 *  See also PERF_RECORD_MISC_EXACT_IP
	 */
	if (precise > 3)
		return -EINVAL;

<<<<<<< HEAD
	list_for_each_entry(evsel, list, node) {
		evsel->attr.exclude_user   = eu;
		evsel->attr.exclude_kernel = ek;
		evsel->attr.exclude_hv     = eh;
		evsel->attr.precise_ip     = precise;
		evsel->attr.exclude_host   = eH;
		evsel->attr.exclude_guest  = eG;
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mod->eu = eu;
	mod->ek = ek;
	mod->eh = eh;
	mod->eH = eH;
	mod->eG = eG;
	mod->precise = precise;
	mod->exclude_GH = exclude_GH;
	mod->sample_read = sample_read;
	mod->pinned = pinned;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Each event can have multiple symbolic names.
 * Symbolic names are (almost) exactly matched.
 */
static enum event_result
parse_event_symbols(const struct option *opt, const char **str,
		    struct perf_event_attr *attr)
{
	enum event_result ret;

	ret = parse_tracepoint_event(opt, str, attr);
	if (ret != EVT_FAILED)
		goto modifier;

	ret = parse_raw_event(str, attr);
	if (ret != EVT_FAILED)
		goto modifier;

	ret = parse_numeric_event(str, attr);
	if (ret != EVT_FAILED)
		goto modifier;

	ret = parse_symbolic_event(str, attr);
	if (ret != EVT_FAILED)
		goto modifier;

	ret = parse_generic_hw_event(str, attr);
	if (ret != EVT_FAILED)
		goto modifier;

	ret = parse_breakpoint_event(str, attr);
	if (ret != EVT_FAILED)
		goto modifier;

	fprintf(stderr, "invalid or unsupported event: '%s'\n", *str);
	fprintf(stderr, "Run 'perf list' for a list of valid events\n");
	return EVT_FAILED;

modifier:
	if (parse_event_modifier(str, attr) < 0) {
		fprintf(stderr, "invalid event modifier: '%s'\n", *str);
		fprintf(stderr, "Run 'perf list' for a list of valid events and modifiers\n");

		return EVT_FAILED;
	}

	return ret;
}

int parse_events(const struct option *opt, const char *str, int unset __used)
{
	struct perf_evlist *evlist = *(struct perf_evlist **)opt->value;
	struct perf_event_attr attr;
	enum event_result ret;
	const char *ostr;

	for (;;) {
		ostr = str;
		memset(&attr, 0, sizeof(attr));
		ret = parse_event_symbols(opt, &str, &attr);
		if (ret == EVT_FAILED)
			return -1;

		if (!(*str == 0 || *str == ',' || isspace(*str)))
			return -1;

		if (ret != EVT_HANDLED_ALL) {
			struct perf_evsel *evsel;
			evsel = perf_evsel__new(&attr, evlist->nr_entries);
			if (evsel == NULL)
				return -1;
			perf_evlist__add(evlist, evsel);

			evsel->name = calloc(str - ostr + 1, 1);
			if (!evsel->name)
				return -1;
			strncpy(evsel->name, ostr, str - ostr);
		}

		if (*str == 0)
			break;
		if (*str == ',')
			++str;
		while (isspace(*str))
			++str;
	}

	return 0;
=======
int parse_events(struct perf_evlist *evlist, const char *str, int unset __used)
{
	LIST_HEAD(list);
	LIST_HEAD(list_tmp);
	YY_BUFFER_STATE buffer;
	int ret, idx = evlist->nr_entries;

	buffer = parse_events__scan_string(str);

	ret = parse_events_parse(&list, &list_tmp, &idx);

	parse_events__flush_buffer(buffer);
	parse_events__delete_buffer(buffer);

	if (!ret) {
		int entries = idx - evlist->nr_entries;
		perf_evlist__splice_list_tail(evlist, &list, entries);
=======
/*
 * Basic modifier sanity check to validate it contains only one
 * instance of any modifier (apart from 'p') present.
 */
static int check_modifier(char *str)
{
	char *p = str;

	/* The sizeof includes 0 byte as well. */
	if (strlen(str) > (sizeof("ukhGHpppSD") - 1))
		return -1;

	while (*p) {
		if (*p != 'p' && strchr(p + 1, *p))
			return -1;
		p++;
	}

	return 0;
}

int parse_events__modifier_event(struct list_head *list, char *str, bool add)
{
	struct perf_evsel *evsel;
	struct event_modifier mod;

	if (str == NULL)
		return 0;

	if (check_modifier(str))
		return -EINVAL;

	if (!add && get_event_modifier(&mod, str, NULL))
		return -EINVAL;

	__evlist__for_each(list, evsel) {
		if (add && get_event_modifier(&mod, str, evsel))
			return -EINVAL;

		evsel->attr.exclude_user   = mod.eu;
		evsel->attr.exclude_kernel = mod.ek;
		evsel->attr.exclude_hv     = mod.eh;
		evsel->attr.precise_ip     = mod.precise;
		evsel->attr.exclude_host   = mod.eH;
		evsel->attr.exclude_guest  = mod.eG;
		evsel->exclude_GH          = mod.exclude_GH;
		evsel->sample_read         = mod.sample_read;

		if (perf_evsel__is_group_leader(evsel))
			evsel->attr.pinned = mod.pinned;
	}

	return 0;
}

int parse_events_name(struct list_head *list, char *name)
{
	struct perf_evsel *evsel;

	__evlist__for_each(list, evsel) {
		if (!evsel->name)
			evsel->name = strdup(name);
	}

	return 0;
}

static int parse_events__scanner(const char *str, void *data, int start_token);

static int parse_events_fixup(int ret, const char *str, void *data,
			      int start_token)
{
	char *o = strdup(str);
	char *s = NULL;
	char *t = o;
	char *p;
	int len = 0;

	if (!o)
		return ret;
	while ((p = strsep(&t, ",")) != NULL) {
		if (s)
			str_append(&s, &len, ",");
		str_append(&s, &len, "cpu/");
		str_append(&s, &len, p);
		str_append(&s, &len, "/");
	}
	free(o);
	if (!s)
		return -ENOMEM;
	return parse_events__scanner(s, data, start_token);
}

static int parse_events__scanner(const char *str, void *data, int start_token)
{
	YY_BUFFER_STATE buffer;
	void *scanner;
	int ret;

	ret = parse_events_lex_init_extra(start_token, &scanner);
	if (ret)
		return ret;

	buffer = parse_events__scan_string(str, scanner);

#ifdef PARSER_DEBUG
	parse_events_debug = 1;
#endif
	ret = parse_events_parse(data, scanner);

	parse_events__flush_buffer(buffer, scanner);
	parse_events__delete_buffer(buffer, scanner);
	parse_events_lex_destroy(scanner);
	if (ret && !strchr(str, '/'))
		ret = parse_events_fixup(ret, str, data, start_token);
	return ret;
}

/*
 * parse event config string, return a list of event terms.
 */
int parse_events_terms(struct list_head *terms, const char *str)
{
	struct parse_events_terms data = {
		.terms = NULL,
	};
	int ret;

	ret = parse_events__scanner(str, &data, PE_START_TERMS);
	if (!ret) {
		list_splice(data.terms, terms);
		zfree(&data.terms);
		return 0;
	}

	if (data.terms)
		parse_events__free_terms(data.terms);
	return ret;
}

int parse_events(struct perf_evlist *evlist, const char *str)
{
	struct parse_events_evlist data = {
		.list = LIST_HEAD_INIT(data.list),
		.idx  = evlist->nr_entries,
	};
	int ret;

	ret = parse_events__scanner(str, &data, PE_START_EVENTS);
	if (!ret) {
		int entries = data.idx - evlist->nr_entries;
		perf_evlist__splice_list_tail(evlist, &data.list, entries);
		evlist->nr_groups += data.nr_groups;
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	/*
	 * There are 2 users - builtin-record and builtin-test objects.
	 * Both call perf_evlist__delete in case of error, so we dont
	 * need to bother.
	 */
<<<<<<< HEAD
	fprintf(stderr, "invalid or unsupported event: '%s'\n", str);
	fprintf(stderr, "Run 'perf list' for a list of valid events\n");
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

int parse_events_option(const struct option *opt, const char *str,
<<<<<<< HEAD
			int unset __used)
{
	struct perf_evlist *evlist = *(struct perf_evlist **)opt->value;
	return parse_events(evlist, str, unset);
>>>>>>> refs/remotes/origin/cm-10.0
}

int parse_filter(const struct option *opt, const char *str,
		 int unset __used)
=======
			int unset __maybe_unused)
{
	struct perf_evlist *evlist = *(struct perf_evlist **)opt->value;
	int ret = parse_events(evlist, str);

	if (ret) {
		fprintf(stderr, "invalid or unsupported event: '%s'\n", str);
		fprintf(stderr, "Run 'perf list' for a list of valid events\n");
	}
	return ret;
}

int parse_filter(const struct option *opt, const char *str,
		 int unset __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	struct perf_evlist *evlist = *(struct perf_evlist **)opt->value;
	struct perf_evsel *last = NULL;

	if (evlist->nr_entries > 0)
<<<<<<< HEAD
		last = list_entry(evlist->entries.prev, struct perf_evsel, node);
=======
		last = perf_evlist__last(evlist);
>>>>>>> refs/remotes/origin/master

	if (last == NULL || last->attr.type != PERF_TYPE_TRACEPOINT) {
		fprintf(stderr,
			"-F option should follow a -e tracepoint option\n");
		return -1;
	}

	last->filter = strdup(str);
	if (last->filter == NULL) {
		fprintf(stderr, "not enough memory to hold filter string\n");
		return -1;
	}

	return 0;
}

static const char * const event_type_descriptors[] = {
	"Hardware event",
	"Software event",
	"Tracepoint event",
	"Hardware cache event",
	"Raw hardware event descriptor",
	"Hardware breakpoint",
};

/*
 * Print the events from <debugfs_mount_point>/tracing/events
 */

<<<<<<< HEAD
void print_tracepoint_events(const char *subsys_glob, const char *event_glob)
=======
void print_tracepoint_events(const char *subsys_glob, const char *event_glob,
			     bool name_only)
>>>>>>> refs/remotes/origin/master
{
	DIR *sys_dir, *evt_dir;
	struct dirent *sys_next, *evt_next, sys_dirent, evt_dirent;
	char evt_path[MAXPATHLEN];
	char dir_path[MAXPATHLEN];

<<<<<<< HEAD
<<<<<<< HEAD
	if (debugfs_valid_mountpoint(debugfs_path))
		return;

	sys_dir = opendir(debugfs_path);
=======
	if (debugfs_valid_mountpoint(tracing_events_path))
		return;

	sys_dir = opendir(tracing_events_path);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (debugfs_valid_mountpoint(tracing_events_path)) {
		printf("  [ Tracepoints not available: %s ]\n", strerror(errno));
		return;
	}

	sys_dir = opendir(tracing_events_path);
>>>>>>> refs/remotes/origin/master
	if (!sys_dir)
		return;

	for_each_subsystem(sys_dir, sys_dirent, sys_next) {
		if (subsys_glob != NULL && 
		    !strglobmatch(sys_dirent.d_name, subsys_glob))
			continue;

<<<<<<< HEAD
<<<<<<< HEAD
		snprintf(dir_path, MAXPATHLEN, "%s/%s", debugfs_path,
=======
		snprintf(dir_path, MAXPATHLEN, "%s/%s", tracing_events_path,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		snprintf(dir_path, MAXPATHLEN, "%s/%s", tracing_events_path,
>>>>>>> refs/remotes/origin/master
			 sys_dirent.d_name);
		evt_dir = opendir(dir_path);
		if (!evt_dir)
			continue;

		for_each_event(sys_dirent, evt_dir, evt_dirent, evt_next) {
			if (event_glob != NULL && 
			    !strglobmatch(evt_dirent.d_name, event_glob))
				continue;

<<<<<<< HEAD
=======
			if (name_only) {
				printf("%s:%s ", sys_dirent.d_name, evt_dirent.d_name);
				continue;
			}

>>>>>>> refs/remotes/origin/master
			snprintf(evt_path, MAXPATHLEN, "%s:%s",
				 sys_dirent.d_name, evt_dirent.d_name);
			printf("  %-50s [%s]\n", evt_path,
				event_type_descriptors[PERF_TYPE_TRACEPOINT]);
		}
		closedir(evt_dir);
	}
	closedir(sys_dir);
}

/*
 * Check whether event is in <debugfs_mount_point>/tracing/events
 */

int is_valid_tracepoint(const char *event_string)
{
	DIR *sys_dir, *evt_dir;
	struct dirent *sys_next, *evt_next, sys_dirent, evt_dirent;
	char evt_path[MAXPATHLEN];
	char dir_path[MAXPATHLEN];

<<<<<<< HEAD
<<<<<<< HEAD
	if (debugfs_valid_mountpoint(debugfs_path))
		return 0;

	sys_dir = opendir(debugfs_path);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (debugfs_valid_mountpoint(tracing_events_path))
		return 0;

	sys_dir = opendir(tracing_events_path);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!sys_dir)
		return 0;

	for_each_subsystem(sys_dir, sys_dirent, sys_next) {

<<<<<<< HEAD
<<<<<<< HEAD
		snprintf(dir_path, MAXPATHLEN, "%s/%s", debugfs_path,
=======
		snprintf(dir_path, MAXPATHLEN, "%s/%s", tracing_events_path,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		snprintf(dir_path, MAXPATHLEN, "%s/%s", tracing_events_path,
>>>>>>> refs/remotes/origin/master
			 sys_dirent.d_name);
		evt_dir = opendir(dir_path);
		if (!evt_dir)
			continue;

		for_each_event(sys_dirent, evt_dir, evt_dirent, evt_next) {
			snprintf(evt_path, MAXPATHLEN, "%s:%s",
				 sys_dirent.d_name, evt_dirent.d_name);
			if (!strcmp(evt_path, event_string)) {
				closedir(evt_dir);
				closedir(sys_dir);
				return 1;
			}
		}
		closedir(evt_dir);
	}
	closedir(sys_dir);
	return 0;
}

<<<<<<< HEAD
void print_events_type(u8 type)
{
	struct event_symbol *syms = event_symbols;
	unsigned int i;
	char name[64];

	for (i = 0; i < ARRAY_SIZE(event_symbols); i++, syms++) {
		if (type != syms->type)
=======
static bool is_event_supported(u8 type, unsigned config)
{
	bool ret = true;
	struct perf_evsel *evsel;
	struct perf_event_attr attr = {
		.type = type,
		.config = config,
		.disabled = 1,
		.exclude_kernel = 1,
	};
	struct {
		struct thread_map map;
		int threads[1];
	} tmap = {
		.map.nr	 = 1,
		.threads = { 0 },
	};

	evsel = perf_evsel__new(&attr);
	if (evsel) {
		ret = perf_evsel__open(evsel, NULL, &tmap.map) >= 0;
		perf_evsel__delete(evsel);
	}

	return ret;
}

static void __print_events_type(u8 type, struct event_symbol *syms,
				unsigned max)
{
	char name[64];
	unsigned i;

	for (i = 0; i < max ; i++, syms++) {
		if (!is_event_supported(type, i))
>>>>>>> refs/remotes/origin/master
			continue;

		if (strlen(syms->alias))
			snprintf(name, sizeof(name),  "%s OR %s",
				 syms->symbol, syms->alias);
		else
			snprintf(name, sizeof(name), "%s", syms->symbol);

<<<<<<< HEAD
		printf("  %-50s [%s]\n", name,
			event_type_descriptors[type]);
	}
}

int print_hwcache_events(const char *event_glob)
{
	unsigned int type, op, i, printed = 0;
=======
		printf("  %-50s [%s]\n", name, event_type_descriptors[type]);
	}
}

void print_events_type(u8 type)
{
	if (type == PERF_TYPE_SOFTWARE)
		__print_events_type(type, event_symbols_sw, PERF_COUNT_SW_MAX);
	else
		__print_events_type(type, event_symbols_hw, PERF_COUNT_HW_MAX);
}

int print_hwcache_events(const char *event_glob, bool name_only)
{
	unsigned int type, op, i, printed = 0;
	char name[64];
>>>>>>> refs/remotes/origin/master

	for (type = 0; type < PERF_COUNT_HW_CACHE_MAX; type++) {
		for (op = 0; op < PERF_COUNT_HW_CACHE_OP_MAX; op++) {
			/* skip invalid cache type */
<<<<<<< HEAD
			if (!is_cache_op_valid(type, op))
				continue;

			for (i = 0; i < PERF_COUNT_HW_CACHE_RESULT_MAX; i++) {
				char *name = event_cache_name(type, op, i);

				if (event_glob != NULL && !strglobmatch(name, event_glob))
					continue;

				printf("  %-50s [%s]\n", name,
					event_type_descriptors[PERF_TYPE_HW_CACHE]);
=======
			if (!perf_evsel__is_cache_op_valid(type, op))
				continue;

			for (i = 0; i < PERF_COUNT_HW_CACHE_RESULT_MAX; i++) {
				__perf_evsel__hw_cache_type_op_res_name(type, op, i,
									name, sizeof(name));
				if (event_glob != NULL && !strglobmatch(name, event_glob))
					continue;

				if (!is_event_supported(PERF_TYPE_HW_CACHE,
							type | (op << 8) | (i << 16)))
					continue;

				if (name_only)
					printf("%s ", name);
				else
					printf("  %-50s [%s]\n", name,
					       event_type_descriptors[PERF_TYPE_HW_CACHE]);
>>>>>>> refs/remotes/origin/master
				++printed;
			}
		}
	}

<<<<<<< HEAD
	return printed;
}

<<<<<<< HEAD
#define MAX_NAME_LEN 100

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Print the help text for the event symbols:
 */
void print_events(const char *event_glob)
{
	unsigned int i, type, prev_type = -1, printed = 0, ntypes_printed = 0;
	struct event_symbol *syms = event_symbols;
	char name[MAX_NAME_LEN];

	printf("\n");
	printf("List of pre-defined events (to be used in -e):\n");

	for (i = 0; i < ARRAY_SIZE(event_symbols); i++, syms++) {
		type = syms->type;

		if (type != prev_type && printed) {
			printf("\n");
			printed = 0;
			ntypes_printed++;
		}
=======
	if (printed)
		printf("\n");
	return printed;
}

static void print_symbol_events(const char *event_glob, unsigned type,
				struct event_symbol *syms, unsigned max,
				bool name_only)
{
	unsigned i, printed = 0;
	char name[MAX_NAME_LEN];

	for (i = 0; i < max; i++, syms++) {
>>>>>>> refs/remotes/origin/master

		if (event_glob != NULL && 
		    !(strglobmatch(syms->symbol, event_glob) ||
		      (syms->alias && strglobmatch(syms->alias, event_glob))))
			continue;

<<<<<<< HEAD
=======
		if (!is_event_supported(type, i))
			continue;

		if (name_only) {
			printf("%s ", syms->symbol);
			continue;
		}

>>>>>>> refs/remotes/origin/master
		if (strlen(syms->alias))
			snprintf(name, MAX_NAME_LEN, "%s OR %s", syms->symbol, syms->alias);
		else
			strncpy(name, syms->symbol, MAX_NAME_LEN);
<<<<<<< HEAD
		printf("  %-50s [%s]\n", name,
			event_type_descriptors[type]);

		prev_type = type;
		++printed;
	}

	if (ntypes_printed) {
		printed = 0;
		printf("\n");
	}
	print_hwcache_events(event_glob);
=======

		printf("  %-50s [%s]\n", name, event_type_descriptors[type]);

		printed++;
	}

	if (printed)
		printf("\n");
}

/*
 * Print the help text for the event symbols:
 */
void print_events(const char *event_glob, bool name_only)
{
	if (!name_only) {
		printf("\n");
		printf("List of pre-defined events (to be used in -e):\n");
	}

	print_symbol_events(event_glob, PERF_TYPE_HARDWARE,
			    event_symbols_hw, PERF_COUNT_HW_MAX, name_only);

	print_symbol_events(event_glob, PERF_TYPE_SOFTWARE,
			    event_symbols_sw, PERF_COUNT_SW_MAX, name_only);

	print_hwcache_events(event_glob, name_only);

	print_pmu_events(event_glob, name_only);
>>>>>>> refs/remotes/origin/master

	if (event_glob != NULL)
		return;

<<<<<<< HEAD
	printf("\n");
	printf("  %-50s [%s]\n",
<<<<<<< HEAD
		"rNNN (see 'perf list --help' on how to encode it)",
	       event_type_descriptors[PERF_TYPE_RAW]);
=======
	       "rNNN",
	       event_type_descriptors[PERF_TYPE_RAW]);
	printf("  %-50s [%s]\n",
	       "cpu/t1=v1[,t2=v2,t3 ...]/modifier",
	       event_type_descriptors[PERF_TYPE_RAW]);
	printf("   (see 'perf list --help' on how to encode it)\n");
>>>>>>> refs/remotes/origin/cm-10.0
	printf("\n");

	printf("  %-50s [%s]\n",
			"mem:<addr>[:access]",
			event_type_descriptors[PERF_TYPE_BREAKPOINT]);
	printf("\n");

	print_tracepoint_events(NULL, NULL);
<<<<<<< HEAD

	exit(129);
=======
}

int parse_events__is_hardcoded_term(struct parse_events__term *term)
{
	return term->type <= PARSE_EVENTS__TERM_TYPE_HARDCODED_MAX;
}

int parse_events__new_term(struct parse_events__term **_term, int type,
			   char *config, char *str, long num)
{
	struct parse_events__term *term;
=======
	if (!name_only) {
		printf("  %-50s [%s]\n",
		       "rNNN",
		       event_type_descriptors[PERF_TYPE_RAW]);
		printf("  %-50s [%s]\n",
		       "cpu/t1=v1[,t2=v2,t3 ...]/modifier",
		       event_type_descriptors[PERF_TYPE_RAW]);
		printf("   (see 'man perf-list' on how to encode it)\n");
		printf("\n");

		printf("  %-50s [%s]\n",
		       "mem:<addr>[:access]",
			event_type_descriptors[PERF_TYPE_BREAKPOINT]);
		printf("\n");
	}

	print_tracepoint_events(NULL, NULL, name_only);
}

int parse_events__is_hardcoded_term(struct parse_events_term *term)
{
	return term->type_term != PARSE_EVENTS__TERM_TYPE_USER;
}

static int new_term(struct parse_events_term **_term, int type_val,
		    int type_term, char *config,
		    char *str, u64 num)
{
	struct parse_events_term *term;
>>>>>>> refs/remotes/origin/master

	term = zalloc(sizeof(*term));
	if (!term)
		return -ENOMEM;

	INIT_LIST_HEAD(&term->list);
<<<<<<< HEAD
	term->type = type;
	term->config = config;

	switch (type) {
	case PARSE_EVENTS__TERM_TYPE_CONFIG:
	case PARSE_EVENTS__TERM_TYPE_CONFIG1:
	case PARSE_EVENTS__TERM_TYPE_CONFIG2:
	case PARSE_EVENTS__TERM_TYPE_SAMPLE_PERIOD:
	case PARSE_EVENTS__TERM_TYPE_BRANCH_SAMPLE_TYPE:
=======
	term->type_val  = type_val;
	term->type_term = type_term;
	term->config = config;

	switch (type_val) {
>>>>>>> refs/remotes/origin/master
	case PARSE_EVENTS__TERM_TYPE_NUM:
		term->val.num = num;
		break;
	case PARSE_EVENTS__TERM_TYPE_STR:
		term->val.str = str;
		break;
	default:
<<<<<<< HEAD
=======
		free(term);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	*_term = term;
	return 0;
}

<<<<<<< HEAD
void parse_events__free_terms(struct list_head *terms)
{
	struct parse_events__term *term, *h;

	list_for_each_entry_safe(term, h, terms, list)
		free(term);

	free(terms);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int parse_events_term__num(struct parse_events_term **term,
			   int type_term, char *config, u64 num)
{
	return new_term(term, PARSE_EVENTS__TERM_TYPE_NUM, type_term,
			config, NULL, num);
}

int parse_events_term__str(struct parse_events_term **term,
			   int type_term, char *config, char *str)
{
	return new_term(term, PARSE_EVENTS__TERM_TYPE_STR, type_term,
			config, str, 0);
}

int parse_events_term__sym_hw(struct parse_events_term **term,
			      char *config, unsigned idx)
{
	struct event_symbol *sym;

	BUG_ON(idx >= PERF_COUNT_HW_MAX);
	sym = &event_symbols_hw[idx];

	if (config)
		return new_term(term, PARSE_EVENTS__TERM_TYPE_STR,
				PARSE_EVENTS__TERM_TYPE_USER, config,
				(char *) sym->symbol, 0);
	else
		return new_term(term, PARSE_EVENTS__TERM_TYPE_STR,
				PARSE_EVENTS__TERM_TYPE_USER,
				(char *) "event", (char *) sym->symbol, 0);
}

int parse_events_term__clone(struct parse_events_term **new,
			     struct parse_events_term *term)
{
	return new_term(new, term->type_val, term->type_term, term->config,
			term->val.str, term->val.num);
}

void parse_events__free_terms(struct list_head *terms)
{
	struct parse_events_term *term, *h;

	list_for_each_entry_safe(term, h, terms, list)
		free(term);
>>>>>>> refs/remotes/origin/master
}
