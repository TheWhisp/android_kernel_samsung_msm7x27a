/*
 * builtin-probe.c
 *
 * Builtin probe command: Set up probe events by C expression
 *
 * Written by Masami Hiramatsu <mhiramat@redhat.com>
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
 */
<<<<<<< HEAD
<<<<<<< HEAD
#define _GNU_SOURCE
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
<<<<<<< HEAD
#undef _GNU_SOURCE
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include "perf.h"
#include "builtin.h"
#include "util/util.h"
#include "util/strlist.h"
#include "util/strfilter.h"
#include "util/symbol.h"
#include "util/debug.h"
<<<<<<< HEAD
#include "util/debugfs.h"
=======
#include <api/fs/debugfs.h>
>>>>>>> refs/remotes/origin/master
#include "util/parse-options.h"
#include "util/probe-finder.h"
#include "util/probe-event.h"

#define DEFAULT_VAR_FILTER "!__k???tab_* & !__crc_*"
#define DEFAULT_FUNC_FILTER "!_*"
<<<<<<< HEAD
<<<<<<< HEAD
#define MAX_PATH_LEN 256
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/* Session management structure */
static struct {
	bool list_events;
	bool force_add;
	bool show_lines;
	bool show_vars;
	bool show_ext_vars;
	bool show_funcs;
	bool mod_events;
<<<<<<< HEAD
=======
	bool uprobes;
>>>>>>> refs/remotes/origin/master
	int nevents;
	struct perf_probe_event events[MAX_PROBES];
	struct strlist *dellist;
	struct line_range line_range;
<<<<<<< HEAD
<<<<<<< HEAD
	const char *target_module;
=======
	const char *target;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	char *target;
>>>>>>> refs/remotes/origin/master
	int max_probe_points;
	struct strfilter *filter;
} params;

/* Parse an event definition. Note that any error must die. */
static int parse_probe_event(const char *str)
{
	struct perf_probe_event *pev = &params.events[params.nevents];
	int ret;

	pr_debug("probe-definition(%d): %s\n", params.nevents, str);
	if (++params.nevents == MAX_PROBES) {
		pr_err("Too many probes (> %d) were specified.", MAX_PROBES);
		return -1;
	}

<<<<<<< HEAD
=======
	pev->uprobes = params.uprobes;

>>>>>>> refs/remotes/origin/master
	/* Parse a perf-probe command into event */
	ret = parse_perf_probe_command(str, pev);
	pr_debug("%d arguments\n", pev->nargs);

	return ret;
}

<<<<<<< HEAD
static int parse_probe_event_argv(int argc, const char **argv)
{
	int i, len, ret;
	char *buf;

	/* Bind up rest arguments */
	len = 0;
	for (i = 0; i < argc; i++)
		len += strlen(argv[i]) + 1;
=======
static int set_target(const char *ptr)
{
	int found = 0;
	const char *buf;

	/*
	 * The first argument after options can be an absolute path
	 * to an executable / library or kernel module.
	 *
	 * TODO: Support relative path, and $PATH, $LD_LIBRARY_PATH,
	 * short module name.
	 */
	if (!params.target && ptr && *ptr == '/') {
		params.target = strdup(ptr);
		if (!params.target)
			return -ENOMEM;

		found = 1;
		buf = ptr + (strlen(ptr) - 3);

		if (strcmp(buf, ".ko"))
			params.uprobes = true;

	}

	return found;
}

static int parse_probe_event_argv(int argc, const char **argv)
{
	int i, len, ret, found_target;
	char *buf;

	found_target = set_target(argv[0]);
	if (found_target < 0)
		return found_target;

	if (found_target && argc == 1)
		return 0;

	/* Bind up rest arguments */
	len = 0;
	for (i = 0; i < argc; i++) {
		if (i == 0 && found_target)
			continue;

		len += strlen(argv[i]) + 1;
	}
>>>>>>> refs/remotes/origin/master
	buf = zalloc(len + 1);
	if (buf == NULL)
		return -ENOMEM;
	len = 0;
<<<<<<< HEAD
	for (i = 0; i < argc; i++)
		len += sprintf(&buf[len], "%s ", argv[i]);
=======
	for (i = 0; i < argc; i++) {
		if (i == 0 && found_target)
			continue;

		len += sprintf(&buf[len], "%s ", argv[i]);
	}
>>>>>>> refs/remotes/origin/master
	params.mod_events = true;
	ret = parse_probe_event(buf);
	free(buf);
	return ret;
}

<<<<<<< HEAD
static int opt_add_probe_event(const struct option *opt __used,
			      const char *str, int unset __used)
=======
static int opt_add_probe_event(const struct option *opt __maybe_unused,
			      const char *str, int unset __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	if (str) {
		params.mod_events = true;
		return parse_probe_event(str);
	} else
		return 0;
}

<<<<<<< HEAD
static int opt_del_probe_event(const struct option *opt __used,
			       const char *str, int unset __used)
=======
static int opt_del_probe_event(const struct option *opt __maybe_unused,
			       const char *str, int unset __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	if (str) {
		params.mod_events = true;
		if (!params.dellist)
			params.dellist = strlist__new(true, NULL);
		strlist__add(params.dellist, str);
	}
	return 0;
}

<<<<<<< HEAD
#ifdef DWARF_SUPPORT
static int opt_show_lines(const struct option *opt __used,
			  const char *str, int unset __used)
{
	int ret = 0;

<<<<<<< HEAD
	if (str)
		ret = parse_line_range_desc(str, &params.line_range);
	INIT_LIST_HEAD(&params.line_range.line_list);
	params.show_lines = true;
=======
=======
static int opt_set_target(const struct option *opt, const char *str,
			int unset __maybe_unused)
{
	int ret = -ENOENT;
	char *tmp;

	if  (str && !params.target) {
		if (!strcmp(opt->long_name, "exec"))
			params.uprobes = true;
#ifdef HAVE_DWARF_SUPPORT
		else if (!strcmp(opt->long_name, "module"))
			params.uprobes = false;
#endif
		else
			return ret;

		/* Expand given path to absolute path, except for modulename */
		if (params.uprobes || strchr(str, '/')) {
			tmp = realpath(str, NULL);
			if (!tmp) {
				pr_warning("Failed to get the absolute path of %s: %m\n", str);
				return ret;
			}
		} else {
			tmp = strdup(str);
			if (!tmp)
				return -ENOMEM;
		}
		params.target = tmp;
		ret = 0;
	}

	return ret;
}

#ifdef HAVE_DWARF_SUPPORT
static int opt_show_lines(const struct option *opt __maybe_unused,
			  const char *str, int unset __maybe_unused)
{
	int ret = 0;

>>>>>>> refs/remotes/origin/master
	if (!str)
		return 0;

	if (params.show_lines) {
		pr_warning("Warning: more than one --line options are"
			   " detected. Only the first one is valid.\n");
		return 0;
	}

	params.show_lines = true;
	ret = parse_line_range_desc(str, &params.line_range);
<<<<<<< HEAD
	INIT_LIST_HEAD(&params.line_range.line_list);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return ret;
}

<<<<<<< HEAD
static int opt_show_vars(const struct option *opt __used,
			 const char *str, int unset __used)
=======
static int opt_show_vars(const struct option *opt __maybe_unused,
			 const char *str, int unset __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	struct perf_probe_event *pev = &params.events[params.nevents];
	int ret;

	if (!str)
		return 0;

	ret = parse_probe_event(str);
	if (!ret && pev->nargs != 0) {
		pr_err("  Error: '--vars' doesn't accept arguments.\n");
		return -EINVAL;
	}
	params.show_vars = true;

	return ret;
}
#endif

<<<<<<< HEAD
static int opt_set_filter(const struct option *opt __used,
			  const char *str, int unset __used)
=======
static int opt_set_filter(const struct option *opt __maybe_unused,
			  const char *str, int unset __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	const char *err;

	if (str) {
		pr_debug2("Set filter: %s\n", str);
		if (params.filter)
			strfilter__delete(params.filter);
		params.filter = strfilter__new(str, &err);
		if (!params.filter) {
			pr_err("Filter parse error at %td.\n", err - str + 1);
			pr_err("Source: \"%s\"\n", str);
			pr_err("         %*c\n", (int)(err - str + 1), '^');
			return -EINVAL;
		}
	}

	return 0;
}

<<<<<<< HEAD
static const char * const probe_usage[] = {
	"perf probe [<options>] 'PROBEDEF' ['PROBEDEF' ...]",
	"perf probe [<options>] --add 'PROBEDEF' [--add 'PROBEDEF' ...]",
	"perf probe [<options>] --del '[GROUP:]EVENT' ...",
	"perf probe --list",
#ifdef DWARF_SUPPORT
	"perf probe [<options>] --line 'LINEDESC'",
	"perf probe [<options>] --vars 'PROBEPOINT'",
#endif
	NULL
};

static const struct option options[] = {
=======
static void init_params(void)
{
	line_range__init(&params.line_range);
}

static void cleanup_params(void)
{
	int i;

	for (i = 0; i < params.nevents; i++)
		clear_perf_probe_event(params.events + i);
	if (params.dellist)
		strlist__delete(params.dellist);
	line_range__clear(&params.line_range);
	free(params.target);
	if (params.filter)
		strfilter__delete(params.filter);
	memset(&params, 0, sizeof(params));
}

static int
__cmd_probe(int argc, const char **argv, const char *prefix __maybe_unused)
{
	const char * const probe_usage[] = {
		"perf probe [<options>] 'PROBEDEF' ['PROBEDEF' ...]",
		"perf probe [<options>] --add 'PROBEDEF' [--add 'PROBEDEF' ...]",
		"perf probe [<options>] --del '[GROUP:]EVENT' ...",
		"perf probe --list",
#ifdef HAVE_DWARF_SUPPORT
		"perf probe [<options>] --line 'LINEDESC'",
		"perf probe [<options>] --vars 'PROBEPOINT'",
#endif
		NULL
};
	const struct option options[] = {
>>>>>>> refs/remotes/origin/master
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show parsed arguments, etc)"),
	OPT_BOOLEAN('l', "list", &params.list_events,
		    "list up current probe events"),
	OPT_CALLBACK('d', "del", NULL, "[GROUP:]EVENT", "delete a probe event.",
		opt_del_probe_event),
	OPT_CALLBACK('a', "add", NULL,
<<<<<<< HEAD
#ifdef DWARF_SUPPORT
=======
#ifdef HAVE_DWARF_SUPPORT
>>>>>>> refs/remotes/origin/master
		"[EVENT=]FUNC[@SRC][+OFF|%return|:RL|;PT]|SRC:AL|SRC;PT"
		" [[NAME=]ARG ...]",
#else
		"[EVENT=]FUNC[+OFF|%return] [[NAME=]ARG ...]",
#endif
		"probe point definition, where\n"
		"\t\tGROUP:\tGroup name (optional)\n"
		"\t\tEVENT:\tEvent name\n"
		"\t\tFUNC:\tFunction name\n"
		"\t\tOFF:\tOffset from function entry (in byte)\n"
		"\t\t%return:\tPut the probe at function return\n"
<<<<<<< HEAD
#ifdef DWARF_SUPPORT
=======
#ifdef HAVE_DWARF_SUPPORT
>>>>>>> refs/remotes/origin/master
		"\t\tSRC:\tSource code path\n"
		"\t\tRL:\tRelative line number from function entry.\n"
		"\t\tAL:\tAbsolute line number in file.\n"
		"\t\tPT:\tLazy expression of line code.\n"
		"\t\tARG:\tProbe argument (local variable name or\n"
		"\t\t\tkprobe-tracer argument format.)\n",
#else
		"\t\tARG:\tProbe argument (kprobe-tracer argument format.)\n",
#endif
		opt_add_probe_event),
	OPT_BOOLEAN('f', "force", &params.force_add, "forcibly add events"
		    " with existing name"),
<<<<<<< HEAD
#ifdef DWARF_SUPPORT
=======
#ifdef HAVE_DWARF_SUPPORT
>>>>>>> refs/remotes/origin/master
	OPT_CALLBACK('L', "line", NULL,
		     "FUNC[:RLN[+NUM|-RLN2]]|SRC:ALN[+NUM|-ALN2]",
		     "Show source code lines.", opt_show_lines),
	OPT_CALLBACK('V', "vars", NULL,
		     "FUNC[@SRC][+OFF|%return|:RL|;PT]|SRC:AL|SRC;PT",
		     "Show accessible variables on PROBEDEF", opt_show_vars),
	OPT_BOOLEAN('\0', "externs", &params.show_ext_vars,
		    "Show external variables too (with --vars only)"),
	OPT_STRING('k', "vmlinux", &symbol_conf.vmlinux_name,
		   "file", "vmlinux pathname"),
	OPT_STRING('s', "source", &symbol_conf.source_prefix,
		   "directory", "path to kernel source"),
<<<<<<< HEAD
<<<<<<< HEAD
	OPT_STRING('m', "module", &params.target_module,
		   "modname", "target module name"),
=======
	OPT_STRING('m', "module", &params.target,
		   "modname|path",
		   "target module name (for online) or path (for offline)"),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	OPT_CALLBACK('m', "module", NULL, "modname|path",
		"target module name (for online) or path (for offline)",
		opt_set_target),
>>>>>>> refs/remotes/origin/master
#endif
	OPT__DRY_RUN(&probe_event_dry_run),
	OPT_INTEGER('\0', "max-probes", &params.max_probe_points,
		 "Set how many probe points can be found for a probe."),
	OPT_BOOLEAN('F', "funcs", &params.show_funcs,
		    "Show potential probe-able functions."),
	OPT_CALLBACK('\0', "filter", NULL,
		     "[!]FILTER", "Set a filter (with --vars/funcs only)\n"
		     "\t\t\t(default: \"" DEFAULT_VAR_FILTER "\" for --vars,\n"
		     "\t\t\t \"" DEFAULT_FUNC_FILTER "\" for --funcs)",
		     opt_set_filter),
<<<<<<< HEAD
	OPT_END()
};

int cmd_probe(int argc, const char **argv, const char *prefix __used)
{
=======
	OPT_CALLBACK('x', "exec", NULL, "executable|path",
			"target executable name or path", opt_set_target),
	OPT_BOOLEAN(0, "demangle", &symbol_conf.demangle,
		    "Disable symbol demangling"),
	OPT_END()
	};
>>>>>>> refs/remotes/origin/master
	int ret;

	argc = parse_options(argc, argv, options, probe_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	if (argc > 0) {
		if (strcmp(argv[0], "-") == 0) {
			pr_warning("  Error: '-' is not supported.\n");
			usage_with_options(probe_usage, options);
		}
		ret = parse_probe_event_argv(argc, argv);
		if (ret < 0) {
			pr_err("  Error: Parse Error.  (%d)\n", ret);
			return ret;
		}
	}

	if (params.max_probe_points == 0)
		params.max_probe_points = MAX_PROBES;

	if ((!params.nevents && !params.dellist && !params.list_events &&
	     !params.show_lines && !params.show_funcs))
		usage_with_options(probe_usage, options);

	/*
	 * Only consider the user's kernel image path if given.
	 */
	symbol_conf.try_vmlinux_path = (symbol_conf.vmlinux_name == NULL);

	if (params.list_events) {
		if (params.mod_events) {
			pr_err("  Error: Don't use --list with --add/--del.\n");
			usage_with_options(probe_usage, options);
		}
		if (params.show_lines) {
			pr_err("  Error: Don't use --list with --line.\n");
			usage_with_options(probe_usage, options);
		}
		if (params.show_vars) {
			pr_err(" Error: Don't use --list with --vars.\n");
			usage_with_options(probe_usage, options);
		}
		if (params.show_funcs) {
			pr_err("  Error: Don't use --list with --funcs.\n");
			usage_with_options(probe_usage, options);
		}
<<<<<<< HEAD
=======
		if (params.uprobes) {
			pr_warning("  Error: Don't use --list with --exec.\n");
			usage_with_options(probe_usage, options);
		}
>>>>>>> refs/remotes/origin/master
		ret = show_perf_probe_events();
		if (ret < 0)
			pr_err("  Error: Failed to show event list. (%d)\n",
			       ret);
		return ret;
	}
	if (params.show_funcs) {
		if (params.nevents != 0 || params.dellist) {
			pr_err("  Error: Don't use --funcs with"
			       " --add/--del.\n");
			usage_with_options(probe_usage, options);
		}
		if (params.show_lines) {
			pr_err("  Error: Don't use --funcs with --line.\n");
			usage_with_options(probe_usage, options);
		}
		if (params.show_vars) {
			pr_err("  Error: Don't use --funcs with --vars.\n");
			usage_with_options(probe_usage, options);
		}
		if (!params.filter)
			params.filter = strfilter__new(DEFAULT_FUNC_FILTER,
						       NULL);
<<<<<<< HEAD
<<<<<<< HEAD
		ret = show_available_funcs(params.target_module,
=======
		ret = show_available_funcs(params.target,
>>>>>>> refs/remotes/origin/cm-10.0
					   params.filter);
		strfilter__delete(params.filter);
=======
		ret = show_available_funcs(params.target, params.filter,
					params.uprobes);
		strfilter__delete(params.filter);
		params.filter = NULL;
>>>>>>> refs/remotes/origin/master
		if (ret < 0)
			pr_err("  Error: Failed to show functions."
			       " (%d)\n", ret);
		return ret;
	}

<<<<<<< HEAD
#ifdef DWARF_SUPPORT
=======
#ifdef HAVE_DWARF_SUPPORT
>>>>>>> refs/remotes/origin/master
	if (params.show_lines) {
		if (params.mod_events) {
			pr_err("  Error: Don't use --line with"
			       " --add/--del.\n");
			usage_with_options(probe_usage, options);
		}
		if (params.show_vars) {
			pr_err(" Error: Don't use --line with --vars.\n");
			usage_with_options(probe_usage, options);
		}

<<<<<<< HEAD
<<<<<<< HEAD
		ret = show_line_range(&params.line_range, params.target_module);
=======
		ret = show_line_range(&params.line_range, params.target);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = show_line_range(&params.line_range, params.target);
>>>>>>> refs/remotes/origin/master
		if (ret < 0)
			pr_err("  Error: Failed to show lines. (%d)\n", ret);
		return ret;
	}
	if (params.show_vars) {
		if (params.mod_events) {
			pr_err("  Error: Don't use --vars with"
			       " --add/--del.\n");
			usage_with_options(probe_usage, options);
		}
		if (!params.filter)
			params.filter = strfilter__new(DEFAULT_VAR_FILTER,
						       NULL);

		ret = show_available_vars(params.events, params.nevents,
					  params.max_probe_points,
<<<<<<< HEAD
<<<<<<< HEAD
					  params.target_module,
=======
					  params.target,
>>>>>>> refs/remotes/origin/cm-10.0
					  params.filter,
					  params.show_ext_vars);
		strfilter__delete(params.filter);
=======
					  params.target,
					  params.filter,
					  params.show_ext_vars);
		strfilter__delete(params.filter);
		params.filter = NULL;
>>>>>>> refs/remotes/origin/master
		if (ret < 0)
			pr_err("  Error: Failed to show vars. (%d)\n", ret);
		return ret;
	}
#endif

	if (params.dellist) {
		ret = del_perf_probe_events(params.dellist);
<<<<<<< HEAD
		strlist__delete(params.dellist);
=======
>>>>>>> refs/remotes/origin/master
		if (ret < 0) {
			pr_err("  Error: Failed to delete events. (%d)\n", ret);
			return ret;
		}
	}

	if (params.nevents) {
		ret = add_perf_probe_events(params.events, params.nevents,
					    params.max_probe_points,
<<<<<<< HEAD
<<<<<<< HEAD
					    params.target_module,
=======
					    params.target,
>>>>>>> refs/remotes/origin/cm-10.0
=======
					    params.target,
>>>>>>> refs/remotes/origin/master
					    params.force_add);
		if (ret < 0) {
			pr_err("  Error: Failed to add events. (%d)\n", ret);
			return ret;
		}
	}
	return 0;
}
<<<<<<< HEAD
=======

int cmd_probe(int argc, const char **argv, const char *prefix)
{
	int ret;

	init_params();
	ret = __cmd_probe(argc, argv, prefix);
	cleanup_params();

	return ret;
}
>>>>>>> refs/remotes/origin/master
