#include "util.h"
#include "../perf.h"
#include "parse-options.h"
#include "evsel.h"
#include "cgroup.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "debugfs.h" /* MAX_PATH, STR() */
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include "evlist.h"

int nr_cgroups;

static int
cgroupfs_find_mountpoint(char *buf, size_t maxlen)
{
	FILE *fp;
<<<<<<< HEAD
<<<<<<< HEAD
	char mountpoint[MAX_PATH+1], tokens[MAX_PATH+1], type[MAX_PATH+1];
=======
	char mountpoint[PATH_MAX + 1], tokens[PATH_MAX + 1], type[PATH_MAX + 1];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	char mountpoint[PATH_MAX + 1], tokens[PATH_MAX + 1], type[PATH_MAX + 1];
>>>>>>> refs/remotes/origin/master
	char *token, *saved_ptr = NULL;
	int found = 0;

	fp = fopen("/proc/mounts", "r");
	if (!fp)
		return -1;

	/*
	 * in order to handle split hierarchy, we need to scan /proc/mounts
	 * and inspect every cgroupfs mount point to find one that has
	 * perf_event subsystem
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	while (fscanf(fp, "%*s %"STR(MAX_PATH)"s %"STR(MAX_PATH)"s %"
				STR(MAX_PATH)"s %*d %*d\n",
=======
	while (fscanf(fp, "%*s %"STR(PATH_MAX)"s %"STR(PATH_MAX)"s %"
				STR(PATH_MAX)"s %*d %*d\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	while (fscanf(fp, "%*s %"STR(PATH_MAX)"s %"STR(PATH_MAX)"s %"
				STR(PATH_MAX)"s %*d %*d\n",
>>>>>>> refs/remotes/origin/master
				mountpoint, type, tokens) == 3) {

		if (!strcmp(type, "cgroup")) {

			token = strtok_r(tokens, ",", &saved_ptr);

			while (token != NULL) {
				if (!strcmp(token, "perf_event")) {
					found = 1;
					break;
				}
				token = strtok_r(NULL, ",", &saved_ptr);
			}
		}
		if (found)
			break;
	}
	fclose(fp);
	if (!found)
		return -1;

	if (strlen(mountpoint) < maxlen) {
		strcpy(buf, mountpoint);
		return 0;
	}
	return -1;
}

static int open_cgroup(char *name)
{
<<<<<<< HEAD
<<<<<<< HEAD
	char path[MAX_PATH+1];
	char mnt[MAX_PATH+1];
	int fd;


	if (cgroupfs_find_mountpoint(mnt, MAX_PATH+1))
		return -1;

	snprintf(path, MAX_PATH, "%s/%s", mnt, name);
=======
=======
>>>>>>> refs/remotes/origin/master
	char path[PATH_MAX + 1];
	char mnt[PATH_MAX + 1];
	int fd;


	if (cgroupfs_find_mountpoint(mnt, PATH_MAX + 1))
		return -1;

	snprintf(path, PATH_MAX, "%s/%s", mnt, name);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	fd = open(path, O_RDONLY);
	if (fd == -1)
		fprintf(stderr, "no access to cgroup %s\n", path);

	return fd;
}

static int add_cgroup(struct perf_evlist *evlist, char *str)
{
	struct perf_evsel *counter;
	struct cgroup_sel *cgrp = NULL;
	int n;
	/*
	 * check if cgrp is already defined, if so we reuse it
	 */
<<<<<<< HEAD
	list_for_each_entry(counter, &evlist->entries, node) {
=======
	evlist__for_each(evlist, counter) {
>>>>>>> refs/remotes/origin/master
		cgrp = counter->cgrp;
		if (!cgrp)
			continue;
		if (!strcmp(cgrp->name, str))
			break;

		cgrp = NULL;
	}

	if (!cgrp) {
		cgrp = zalloc(sizeof(*cgrp));
		if (!cgrp)
			return -1;

		cgrp->name = str;

		cgrp->fd = open_cgroup(str);
		if (cgrp->fd == -1) {
			free(cgrp);
			return -1;
		}
	}

	/*
	 * find corresponding event
	 * if add cgroup N, then need to find event N
	 */
	n = 0;
<<<<<<< HEAD
	list_for_each_entry(counter, &evlist->entries, node) {
=======
	evlist__for_each(evlist, counter) {
>>>>>>> refs/remotes/origin/master
		if (n == nr_cgroups)
			goto found;
		n++;
	}
	if (cgrp->refcnt == 0)
		free(cgrp);

	return -1;
found:
	cgrp->refcnt++;
	counter->cgrp = cgrp;
	return 0;
}

void close_cgroup(struct cgroup_sel *cgrp)
{
	if (!cgrp)
		return;

	/* XXX: not reentrant */
	if (--cgrp->refcnt == 0) {
		close(cgrp->fd);
<<<<<<< HEAD
		free(cgrp->name);
=======
		zfree(&cgrp->name);
>>>>>>> refs/remotes/origin/master
		free(cgrp);
	}
}

<<<<<<< HEAD
int parse_cgroups(const struct option *opt __used, const char *str,
		  int unset __used)
=======
int parse_cgroups(const struct option *opt __maybe_unused, const char *str,
		  int unset __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	struct perf_evlist *evlist = *(struct perf_evlist **)opt->value;
	const char *p, *e, *eos = str + strlen(str);
	char *s;
	int ret;

	if (list_empty(&evlist->entries)) {
		fprintf(stderr, "must define events before cgroups\n");
		return -1;
	}

	for (;;) {
		p = strchr(str, ',');
		e = p ? p : eos;

		/* allow empty cgroups, i.e., skip */
		if (e - str) {
			/* termination added */
			s = strndup(str, e - str);
			if (!s)
				return -1;
			ret = add_cgroup(evlist, s);
			if (ret) {
				free(s);
				return -1;
			}
		}
		/* nr_cgroups is increased een for empty cgroups */
		nr_cgroups++;
		if (!p)
			break;
		str = p+1;
	}
	return 0;
}
