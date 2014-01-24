/*
 * builtin-buildid-list.c
 *
<<<<<<< HEAD
 * Builtin buildid-list command: list buildids in perf.data
=======
 * Builtin buildid-list command: list buildids in perf.data, in the running
 * kernel and in ELF files.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Copyright (C) 2009, Red Hat Inc.
 * Copyright (C) 2009, Arnaldo Carvalho de Melo <acme@redhat.com>
 */
#include "builtin.h"
#include "perf.h"
#include "util/build-id.h"
#include "util/cache.h"
#include "util/debug.h"
#include "util/parse-options.h"
#include "util/session.h"
#include "util/symbol.h"

<<<<<<< HEAD
static char const *input_name = "perf.data";
static bool force;
=======
#include <libelf.h>

static const char *input_name;
static bool force;
static bool show_kernel;
>>>>>>> refs/remotes/origin/cm-10.0
static bool with_hits;

static const char * const buildid_list_usage[] = {
	"perf buildid-list [<options>]",
	NULL
};

static const struct option options[] = {
	OPT_BOOLEAN('H', "with-hits", &with_hits, "Show only DSOs with hits"),
	OPT_STRING('i', "input", &input_name, "file",
		    "input file name"),
	OPT_BOOLEAN('f', "force", &force, "don't complain, do it"),
<<<<<<< HEAD
=======
	OPT_BOOLEAN('k', "kernel", &show_kernel, "Show current kernel build id"),
>>>>>>> refs/remotes/origin/cm-10.0
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose"),
	OPT_END()
};

<<<<<<< HEAD
static int __cmd_buildid_list(void)
{
	struct perf_session *session;

=======
static int sysfs__fprintf_build_id(FILE *fp)
{
	u8 kallsyms_build_id[BUILD_ID_SIZE];
	char sbuild_id[BUILD_ID_SIZE * 2 + 1];

	if (sysfs__read_build_id("/sys/kernel/notes", kallsyms_build_id,
				 sizeof(kallsyms_build_id)) != 0)
		return -1;

	build_id__sprintf(kallsyms_build_id, sizeof(kallsyms_build_id),
			  sbuild_id);
	fprintf(fp, "%s\n", sbuild_id);
	return 0;
}

static int filename__fprintf_build_id(const char *name, FILE *fp)
{
	u8 build_id[BUILD_ID_SIZE];
	char sbuild_id[BUILD_ID_SIZE * 2 + 1];

	if (filename__read_build_id(name, build_id,
				    sizeof(build_id)) != sizeof(build_id))
		return 0;

	build_id__sprintf(build_id, sizeof(build_id), sbuild_id);
	return fprintf(fp, "%s\n", sbuild_id);
}

static int perf_session__list_build_ids(void)
{
	struct perf_session *session;

	elf_version(EV_CURRENT);

>>>>>>> refs/remotes/origin/cm-10.0
	session = perf_session__new(input_name, O_RDONLY, force, false,
				    &build_id__mark_dso_hit_ops);
	if (session == NULL)
		return -1;

<<<<<<< HEAD
=======
	/*
	 * See if this is an ELF file first:
	 */
	if (filename__fprintf_build_id(session->filename, stdout))
		goto out;

>>>>>>> refs/remotes/origin/cm-10.0
	if (with_hits)
		perf_session__process_events(session, &build_id__mark_dso_hit_ops);

	perf_session__fprintf_dsos_buildid(session, stdout, with_hits);
<<<<<<< HEAD

=======
out:
>>>>>>> refs/remotes/origin/cm-10.0
	perf_session__delete(session);
	return 0;
}

<<<<<<< HEAD
=======
static int __cmd_buildid_list(void)
{
	if (show_kernel)
		return sysfs__fprintf_build_id(stdout);

	return perf_session__list_build_ids();
}

>>>>>>> refs/remotes/origin/cm-10.0
int cmd_buildid_list(int argc, const char **argv, const char *prefix __used)
{
	argc = parse_options(argc, argv, options, buildid_list_usage, 0);
	setup_pager();
	return __cmd_buildid_list();
}
