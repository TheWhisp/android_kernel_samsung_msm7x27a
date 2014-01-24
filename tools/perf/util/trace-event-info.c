/*
 * Copyright (C) 2008,2009, Steven Rostedt <srostedt@redhat.com>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License (not later!)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
<<<<<<< HEAD
<<<<<<< HEAD
#define _GNU_SOURCE
=======
#include "util.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "util.h"
>>>>>>> refs/remotes/origin/master
#include <dirent.h>
#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <ctype.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <errno.h>
#include <stdbool.h>
#include <linux/list.h>
#include <linux/kernel.h>

#include "../perf.h"
#include "trace-event.h"
<<<<<<< HEAD
#include "debugfs.h"
=======
#include <api/fs/debugfs.h>
>>>>>>> refs/remotes/origin/master
#include "evsel.h"

#define VERSION "0.5"

<<<<<<< HEAD
<<<<<<< HEAD
#define _STR(x) #x
#define STR(x) _STR(x)
#define MAX_PATH 256

=======
>>>>>>> refs/remotes/origin/cm-10.0
#define TRACE_CTRL	"tracing_on"
#define TRACE		"trace"
#define AVAILABLE	"available_tracers"
#define CURRENT		"current_tracer"
#define ITER_CTRL	"trace_options"
#define MAX_LATENCY	"tracing_max_latency"

unsigned int page_size;

static const char *output_file = "trace.info";
static int output_fd;

struct event_list {
	struct event_list *next;
	const char *event;
};

struct events {
	struct events *sibling;
	struct events *children;
	struct events *next;
	char *name;
};


<<<<<<< HEAD

static void die(const char *fmt, ...)
{
	va_list ap;
	int ret = errno;

	if (errno)
		perror("trace-cmd");
	else
		ret = -1;

	va_start(ap, fmt);
	fprintf(stderr, "  ");
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fprintf(stderr, "\n");
	exit(ret);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
void *malloc_or_die(unsigned int size)
{
	void *data;

	data = malloc(size);
	if (!data)
		die("malloc");
	return data;
}

static const char *find_debugfs(void)
{
	const char *path = debugfs_mount(NULL);

	if (!path)
		die("Your kernel not support debugfs filesystem");

	return path;
}

/*
 * Finds the path to the debugfs/tracing
 * Allocates the string and stores it.
 */
static const char *find_tracing_dir(void)
{
	static char *tracing;
	static int tracing_found;
	const char *debugfs;

	if (tracing_found)
		return tracing;

	debugfs = find_debugfs();

	tracing = malloc_or_die(strlen(debugfs) + 9);

	sprintf(tracing, "%s/tracing", debugfs);

	tracing_found = 1;
	return tracing;
}

static char *get_tracing_file(const char *name)
{
	const char *tracing;
	char *file;

	tracing = find_tracing_dir();
	if (!tracing)
		return NULL;

	file = malloc_or_die(strlen(tracing) + strlen(name) + 2);

	sprintf(file, "%s/%s", tracing, name);
	return file;
}

static void put_tracing_file(char *file)
{
	free(file);
}

static ssize_t calc_data_size;

static ssize_t write_or_die(const void *buf, size_t len)
{
	int ret;

	if (calc_data_size) {
		calc_data_size += len;
		return len;
	}

	ret = write(output_fd, buf, len);
	if (ret < 0)
		die("writing to '%s'", output_file);

	return ret;
}
=======
static int output_fd;

>>>>>>> refs/remotes/origin/master

int bigendian(void)
{
	unsigned char str[] = { 0x1, 0x2, 0x3, 0x4, 0x0, 0x0, 0x0, 0x0};
	unsigned int *ptr;

	ptr = (unsigned int *)(void *)str;
	return *ptr == 0x01020304;
}

<<<<<<< HEAD
<<<<<<< HEAD
static unsigned long long copy_file_fd(int fd)
{
	unsigned long long size = 0;
	char buf[BUFSIZ];
	int r;

	do {
		r = read(fd, buf, BUFSIZ);
		if (r > 0) {
			size += r;
			write_or_die(buf, r);
		}
	} while (r > 0);

	return size;
}

static unsigned long long copy_file(const char *file)
{
	unsigned long long size = 0;
	int fd;
=======
/* unfortunately, you can not stat debugfs or proc files for size */
static void record_file(const char *file, size_t hdr_sz)
=======
/* unfortunately, you can not stat debugfs or proc files for size */
static int record_file(const char *file, ssize_t hdr_sz)
>>>>>>> refs/remotes/origin/master
{
	unsigned long long size = 0;
	char buf[BUFSIZ], *sizep;
	off_t hdr_pos = lseek(output_fd, 0, SEEK_CUR);
	int r, fd;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	fd = open(file, O_RDONLY);
	if (fd < 0)
		die("Can't read '%s'", file);
<<<<<<< HEAD
	size = copy_file_fd(fd);
	close(fd);

	return size;
}

static unsigned long get_size_fd(int fd)
{
	unsigned long long size = 0;
	char buf[BUFSIZ];
	int r;

	do {
		r = read(fd, buf, BUFSIZ);
		if (r > 0)
			size += r;
	} while (r > 0);

	lseek(fd, 0, SEEK_SET);

	return size;
}

static unsigned long get_size(const char *file)
{
	unsigned long long size = 0;
	int fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		die("Can't read '%s'", file);
	size = get_size_fd(fd);
	close(fd);

	return size;
=======

	/* put in zeros for file size, then fill true size later */
	if (hdr_sz)
		write_or_die(&size, hdr_sz);
=======
	int err = -EIO;

	fd = open(file, O_RDONLY);
	if (fd < 0) {
		pr_debug("Can't read '%s'", file);
		return -errno;
	}

	/* put in zeros for file size, then fill true size later */
	if (hdr_sz) {
		if (write(output_fd, &size, hdr_sz) != hdr_sz)
			goto out;
	}
>>>>>>> refs/remotes/origin/master

	do {
		r = read(fd, buf, BUFSIZ);
		if (r > 0) {
			size += r;
<<<<<<< HEAD
			write_or_die(buf, r);
		}
	} while (r > 0);
	close(fd);
=======
			if (write(output_fd, buf, r) != r)
				goto out;
		}
	} while (r > 0);
>>>>>>> refs/remotes/origin/master

	/* ugh, handle big-endian hdr_size == 4 */
	sizep = (char*)&size;
	if (bigendian())
		sizep += sizeof(u64) - hdr_sz;

<<<<<<< HEAD
	if (hdr_sz && pwrite(output_fd, sizep, hdr_sz, hdr_pos) < 0)
		die("writing to %s", output_file);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void read_header_files(void)
{
<<<<<<< HEAD
	unsigned long long size, check_size;
	char *path;
	int fd;

	path = get_tracing_file("events/header_page");
	fd = open(path, O_RDONLY);
	if (fd < 0)
		die("can't read '%s'", path);

	/* unfortunately, you can not stat debugfs files for size */
	size = get_size_fd(fd);

	write_or_die("header_page", 12);
	write_or_die(&size, 8);
	check_size = copy_file_fd(fd);
	close(fd);

	if (size != check_size)
		die("wrong size for '%s' size=%lld read=%lld",
		    path, size, check_size);
	put_tracing_file(path);

	path = get_tracing_file("events/header_event");
	fd = open(path, O_RDONLY);
	if (fd < 0)
		die("can't read '%s'", path);

	size = get_size_fd(fd);

	write_or_die("header_event", 13);
	write_or_die(&size, 8);
	check_size = copy_file_fd(fd);
	if (size != check_size)
		die("wrong size for '%s'", path);
	put_tracing_file(path);
	close(fd);
=======
	char *path;
	struct stat st;

	path = get_tracing_file("events/header_page");
	if (stat(path, &st) < 0)
		die("can't read '%s'", path);

	write_or_die("header_page", 12);
	record_file(path, 8);
	put_tracing_file(path);

	path = get_tracing_file("events/header_event");
	if (stat(path, &st) < 0)
		die("can't read '%s'", path);

	write_or_die("header_event", 13);
	record_file(path, 8);
	put_tracing_file(path);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (hdr_sz && pwrite(output_fd, sizep, hdr_sz, hdr_pos) < 0) {
		pr_debug("writing file size failed\n");
		goto out;
	}

	err = 0;
out:
	close(fd);
	return err;
}

static int record_header_files(void)
{
	char *path;
	struct stat st;
	int err = -EIO;

	path = get_tracing_file("events/header_page");
	if (!path) {
		pr_debug("can't get tracing/events/header_page");
		return -ENOMEM;
	}

	if (stat(path, &st) < 0) {
		pr_debug("can't read '%s'", path);
		goto out;
	}

	if (write(output_fd, "header_page", 12) != 12) {
		pr_debug("can't write header_page\n");
		goto out;
	}

	if (record_file(path, 8) < 0) {
		pr_debug("can't record header_page file\n");
		goto out;
	}

	put_tracing_file(path);

	path = get_tracing_file("events/header_event");
	if (!path) {
		pr_debug("can't get tracing/events/header_event");
		err = -ENOMEM;
		goto out;
	}

	if (stat(path, &st) < 0) {
		pr_debug("can't read '%s'", path);
		goto out;
	}

	if (write(output_fd, "header_event", 13) != 13) {
		pr_debug("can't write header_event\n");
		goto out;
	}

	if (record_file(path, 8) < 0) {
		pr_debug("can't record header_event file\n");
		goto out;
	}

	err = 0;
out:
	put_tracing_file(path);
	return err;
>>>>>>> refs/remotes/origin/master
}

static bool name_in_tp_list(char *sys, struct tracepoint_path *tps)
{
	while (tps) {
		if (!strcmp(sys, tps->name))
			return true;
		tps = tps->next;
	}

	return false;
}

<<<<<<< HEAD
static void copy_event_system(const char *sys, struct tracepoint_path *tps)
{
<<<<<<< HEAD
	unsigned long long size, check_size;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int copy_event_system(const char *sys, struct tracepoint_path *tps)
{
>>>>>>> refs/remotes/origin/master
	struct dirent *dent;
	struct stat st;
	char *format;
	DIR *dir;
	int count = 0;
	int ret;
<<<<<<< HEAD

	dir = opendir(sys);
	if (!dir)
		die("can't read directory '%s'", sys);
=======
	int err;

	dir = opendir(sys);
	if (!dir) {
		pr_debug("can't read directory '%s'", sys);
		return -errno;
	}
>>>>>>> refs/remotes/origin/master

	while ((dent = readdir(dir))) {
		if (dent->d_type != DT_DIR ||
		    strcmp(dent->d_name, ".") == 0 ||
		    strcmp(dent->d_name, "..") == 0 ||
		    !name_in_tp_list(dent->d_name, tps))
			continue;
<<<<<<< HEAD
		format = malloc_or_die(strlen(sys) + strlen(dent->d_name) + 10);
=======
		format = malloc(strlen(sys) + strlen(dent->d_name) + 10);
		if (!format) {
			err = -ENOMEM;
			goto out;
		}
>>>>>>> refs/remotes/origin/master
		sprintf(format, "%s/%s/format", sys, dent->d_name);
		ret = stat(format, &st);
		free(format);
		if (ret < 0)
			continue;
		count++;
	}

<<<<<<< HEAD
	write_or_die(&count, 4);
=======
	if (write(output_fd, &count, 4) != 4) {
		err = -EIO;
		pr_debug("can't write count\n");
		goto out;
	}
>>>>>>> refs/remotes/origin/master

	rewinddir(dir);
	while ((dent = readdir(dir))) {
		if (dent->d_type != DT_DIR ||
		    strcmp(dent->d_name, ".") == 0 ||
		    strcmp(dent->d_name, "..") == 0 ||
		    !name_in_tp_list(dent->d_name, tps))
			continue;
<<<<<<< HEAD
		format = malloc_or_die(strlen(sys) + strlen(dent->d_name) + 10);
		sprintf(format, "%s/%s/format", sys, dent->d_name);
		ret = stat(format, &st);

<<<<<<< HEAD
		if (ret >= 0) {
			/* unfortunately, you can not stat debugfs files for size */
			size = get_size(format);
			write_or_die(&size, 8);
			check_size = copy_file(format);
			if (size != check_size)
				die("error in size of file '%s'", format);
		}
=======
		if (ret >= 0)
			record_file(format, 8);
>>>>>>> refs/remotes/origin/cm-10.0

		free(format);
	}
	closedir(dir);
}

static void read_ftrace_files(struct tracepoint_path *tps)
{
	char *path;

	path = get_tracing_file("events/ftrace");

	copy_event_system(path, tps);

	put_tracing_file(path);
=======
		format = malloc(strlen(sys) + strlen(dent->d_name) + 10);
		if (!format) {
			err = -ENOMEM;
			goto out;
		}
		sprintf(format, "%s/%s/format", sys, dent->d_name);
		ret = stat(format, &st);

		if (ret >= 0) {
			err = record_file(format, 8);
			if (err) {
				free(format);
				goto out;
			}
		}
		free(format);
	}
	err = 0;
out:
	closedir(dir);
	return err;
}

static int record_ftrace_files(struct tracepoint_path *tps)
{
	char *path;
	int ret;

	path = get_tracing_file("events/ftrace");
	if (!path) {
		pr_debug("can't get tracing/events/ftrace");
		return -ENOMEM;
	}

	ret = copy_event_system(path, tps);

	put_tracing_file(path);

	return ret;
>>>>>>> refs/remotes/origin/master
}

static bool system_in_tp_list(char *sys, struct tracepoint_path *tps)
{
	while (tps) {
		if (!strcmp(sys, tps->system))
			return true;
		tps = tps->next;
	}

	return false;
}

<<<<<<< HEAD
static void read_event_files(struct tracepoint_path *tps)
=======
static int record_event_files(struct tracepoint_path *tps)
>>>>>>> refs/remotes/origin/master
{
	struct dirent *dent;
	struct stat st;
	char *path;
	char *sys;
	DIR *dir;
	int count = 0;
	int ret;
<<<<<<< HEAD

	path = get_tracing_file("events");

	dir = opendir(path);
	if (!dir)
		die("can't read directory '%s'", path);
=======
	int err;

	path = get_tracing_file("events");
	if (!path) {
		pr_debug("can't get tracing/events");
		return -ENOMEM;
	}

	dir = opendir(path);
	if (!dir) {
		err = -errno;
		pr_debug("can't read directory '%s'", path);
		goto out;
	}
>>>>>>> refs/remotes/origin/master

	while ((dent = readdir(dir))) {
		if (dent->d_type != DT_DIR ||
		    strcmp(dent->d_name, ".") == 0 ||
		    strcmp(dent->d_name, "..") == 0 ||
		    strcmp(dent->d_name, "ftrace") == 0 ||
		    !system_in_tp_list(dent->d_name, tps))
			continue;
		count++;
	}

<<<<<<< HEAD
	write_or_die(&count, 4);
=======
	if (write(output_fd, &count, 4) != 4) {
		err = -EIO;
		pr_debug("can't write count\n");
		goto out;
	}
>>>>>>> refs/remotes/origin/master

	rewinddir(dir);
	while ((dent = readdir(dir))) {
		if (dent->d_type != DT_DIR ||
		    strcmp(dent->d_name, ".") == 0 ||
		    strcmp(dent->d_name, "..") == 0 ||
		    strcmp(dent->d_name, "ftrace") == 0 ||
		    !system_in_tp_list(dent->d_name, tps))
			continue;
<<<<<<< HEAD
		sys = malloc_or_die(strlen(path) + strlen(dent->d_name) + 2);
		sprintf(sys, "%s/%s", path, dent->d_name);
		ret = stat(sys, &st);
		if (ret >= 0) {
			write_or_die(dent->d_name, strlen(dent->d_name) + 1);
			copy_event_system(sys, tps);
		}
		free(sys);
	}

	closedir(dir);
	put_tracing_file(path);
}

static void read_proc_kallsyms(void)
{
<<<<<<< HEAD
	unsigned int size, check_size;
=======
	unsigned int size;
>>>>>>> refs/remotes/origin/cm-10.0
	const char *path = "/proc/kallsyms";
	struct stat st;
	int ret;
=======
		sys = malloc(strlen(path) + strlen(dent->d_name) + 2);
		if (!sys) {
			err = -ENOMEM;
			goto out;
		}
		sprintf(sys, "%s/%s", path, dent->d_name);
		ret = stat(sys, &st);
		if (ret >= 0) {
			ssize_t size = strlen(dent->d_name) + 1;

			if (write(output_fd, dent->d_name, size) != size ||
			    copy_event_system(sys, tps) < 0) {
				err = -EIO;
				free(sys);
				goto out;
			}
		}
		free(sys);
	}
	err = 0;
out:
	closedir(dir);
	put_tracing_file(path);

	return err;
}

static int record_proc_kallsyms(void)
{
	unsigned int size;
	const char *path = "/proc/kallsyms";
	struct stat st;
	int ret, err = 0;
>>>>>>> refs/remotes/origin/master

	ret = stat(path, &st);
	if (ret < 0) {
		/* not found */
		size = 0;
<<<<<<< HEAD
		write_or_die(&size, 4);
		return;
	}
<<<<<<< HEAD
	size = get_size(path);
	write_or_die(&size, 4);
	check_size = copy_file(path);
	if (size != check_size)
		die("error in size of file '%s'", path);

=======
	record_file(path, 4);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void read_ftrace_printk(void)
{
<<<<<<< HEAD
	unsigned int size, check_size;
=======
	unsigned int size;
>>>>>>> refs/remotes/origin/cm-10.0
	char *path;
	struct stat st;
	int ret;

	path = get_tracing_file("printk_formats");
=======
		if (write(output_fd, &size, 4) != 4)
			err = -EIO;
		return err;
	}
	return record_file(path, 4);
}

static int record_ftrace_printk(void)
{
	unsigned int size;
	char *path;
	struct stat st;
	int ret, err = 0;

	path = get_tracing_file("printk_formats");
	if (!path) {
		pr_debug("can't get tracing/printk_formats");
		return -ENOMEM;
	}

>>>>>>> refs/remotes/origin/master
	ret = stat(path, &st);
	if (ret < 0) {
		/* not found */
		size = 0;
<<<<<<< HEAD
		write_or_die(&size, 4);
		goto out;
	}
<<<<<<< HEAD
	size = get_size(path);
	write_or_die(&size, 4);
	check_size = copy_file(path);
	if (size != check_size)
		die("error in size of file '%s'", path);
=======
	record_file(path, 4);

>>>>>>> refs/remotes/origin/cm-10.0
out:
	put_tracing_file(path);
=======
		if (write(output_fd, &size, 4) != 4)
			err = -EIO;
		goto out;
	}
	err = record_file(path, 4);

out:
	put_tracing_file(path);
	return err;
}

static void
put_tracepoints_path(struct tracepoint_path *tps)
{
	while (tps) {
		struct tracepoint_path *t = tps;

		tps = tps->next;
		zfree(&t->name);
		zfree(&t->system);
		free(t);
	}
>>>>>>> refs/remotes/origin/master
}

static struct tracepoint_path *
get_tracepoints_path(struct list_head *pattrs)
{
	struct tracepoint_path path, *ppath = &path;
	struct perf_evsel *pos;
	int nr_tracepoints = 0;

	list_for_each_entry(pos, pattrs, node) {
		if (pos->attr.type != PERF_TYPE_TRACEPOINT)
			continue;
		++nr_tracepoints;
<<<<<<< HEAD
		ppath->next = tracepoint_id_to_path(pos->attr.config);
		if (!ppath->next)
			die("%s\n", "No memory to alloc tracepoints list");
=======

		if (pos->name) {
			ppath->next = tracepoint_name_to_path(pos->name);
			if (ppath->next)
				goto next;

			if (strchr(pos->name, ':') == NULL)
				goto try_id;

			goto error;
		}

try_id:
		ppath->next = tracepoint_id_to_path(pos->attr.config);
		if (!ppath->next) {
error:
			pr_debug("No memory to alloc tracepoints list\n");
			put_tracepoints_path(&path);
			return NULL;
		}
next:
>>>>>>> refs/remotes/origin/master
		ppath = ppath->next;
	}

	return nr_tracepoints > 0 ? path.next : NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static void
put_tracepoints_path(struct tracepoint_path *tps)
{
	while (tps) {
		struct tracepoint_path *t = tps;

		tps = tps->next;
		free(t->name);
		free(t->system);
		free(t);
	}
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
bool have_tracepoints(struct list_head *pattrs)
{
	struct perf_evsel *pos;

	list_for_each_entry(pos, pattrs, node)
		if (pos->attr.type == PERF_TYPE_TRACEPOINT)
			return true;

	return false;
}

<<<<<<< HEAD
<<<<<<< HEAD
int read_tracing_data(int fd, struct list_head *pattrs)
{
	char buf[BUFSIZ];
	struct tracepoint_path *tps = get_tracepoints_path(pattrs);

	/*
	 * What? No tracepoints? No sense writing anything here, bail out.
	 */
	if (tps == NULL)
		return -1;

	output_fd = fd;

=======
static void tracing_data_header(void)
{
	char buf[20];

	/* just guessing this is someone's birthday.. ;) */
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int tracing_data_header(void)
{
	char buf[20];
	ssize_t size;

	/* just guessing this is someone's birthday.. ;) */
>>>>>>> refs/remotes/origin/master
	buf[0] = 23;
	buf[1] = 8;
	buf[2] = 68;
	memcpy(buf + 3, "tracing", 7);

<<<<<<< HEAD
	write_or_die(buf, 10);

	write_or_die(VERSION, strlen(VERSION) + 1);
=======
	if (write(output_fd, buf, 10) != 10)
		return -1;

	size = strlen(VERSION) + 1;
	if (write(output_fd, VERSION, size) != size)
		return -1;
>>>>>>> refs/remotes/origin/master

	/* save endian */
	if (bigendian())
		buf[0] = 1;
	else
		buf[0] = 0;

<<<<<<< HEAD
	write_or_die(buf, 1);

	/* save size of long */
	buf[0] = sizeof(long);
	write_or_die(buf, 1);

	/* save page_size */
	page_size = sysconf(_SC_PAGESIZE);
	write_or_die(&page_size, 4);
<<<<<<< HEAD

=======
=======
	if (write(output_fd, buf, 1) != 1)
		return -1;

	/* save size of long */
	buf[0] = sizeof(long);
	if (write(output_fd, buf, 1) != 1)
		return -1;

	/* save page_size */
	if (write(output_fd, &page_size, 4) != 4)
		return -1;

	return 0;
>>>>>>> refs/remotes/origin/master
}

struct tracing_data *tracing_data_get(struct list_head *pattrs,
				      int fd, bool temp)
{
	struct tracepoint_path *tps;
	struct tracing_data *tdata;
<<<<<<< HEAD
=======
	int err;
>>>>>>> refs/remotes/origin/master

	output_fd = fd;

	tps = get_tracepoints_path(pattrs);
	if (!tps)
		return NULL;

<<<<<<< HEAD
	tdata = malloc_or_die(sizeof(*tdata));
=======
	tdata = malloc(sizeof(*tdata));
	if (!tdata)
		return NULL;

>>>>>>> refs/remotes/origin/master
	tdata->temp = temp;
	tdata->size = 0;

	if (temp) {
		int temp_fd;

		snprintf(tdata->temp_file, sizeof(tdata->temp_file),
			 "/tmp/perf-XXXXXX");
<<<<<<< HEAD
		if (!mkstemp(tdata->temp_file))
			die("Can't make temp file");

		temp_fd = open(tdata->temp_file, O_RDWR);
		if (temp_fd < 0)
			die("Can't read '%s'", tdata->temp_file);
=======
		if (!mkstemp(tdata->temp_file)) {
			pr_debug("Can't make temp file");
			return NULL;
		}

		temp_fd = open(tdata->temp_file, O_RDWR);
		if (temp_fd < 0) {
			pr_debug("Can't read '%s'", tdata->temp_file);
			return NULL;
		}
>>>>>>> refs/remotes/origin/master

		/*
		 * Set the temp file the default output, so all the
		 * tracing data are stored into it.
		 */
		output_fd = temp_fd;
	}

<<<<<<< HEAD
	tracing_data_header();
>>>>>>> refs/remotes/origin/cm-10.0
	read_header_files();
	read_ftrace_files(tps);
	read_event_files(tps);
	read_proc_kallsyms();
	read_ftrace_printk();

<<<<<<< HEAD
	return 0;
}

ssize_t read_tracing_data_size(int fd, struct list_head *pattrs)
{
	ssize_t size;
	int err = 0;

	calc_data_size = 1;
	err = read_tracing_data(fd, pattrs);
	size = calc_data_size - 1;
	calc_data_size = 0;

	if (err < 0)
		return err;

	return size;
=======
=======
	err = tracing_data_header();
	if (err)
		goto out;
	err = record_header_files();
	if (err)
		goto out;
	err = record_ftrace_files(tps);
	if (err)
		goto out;
	err = record_event_files(tps);
	if (err)
		goto out;
	err = record_proc_kallsyms();
	if (err)
		goto out;
	err = record_ftrace_printk();

out:
>>>>>>> refs/remotes/origin/master
	/*
	 * All tracing data are stored by now, we can restore
	 * the default output file in case we used temp file.
	 */
	if (temp) {
		tdata->size = lseek(output_fd, 0, SEEK_CUR);
		close(output_fd);
		output_fd = fd;
	}

<<<<<<< HEAD
=======
	if (err)
		zfree(&tdata);

>>>>>>> refs/remotes/origin/master
	put_tracepoints_path(tps);
	return tdata;
}

<<<<<<< HEAD
void tracing_data_put(struct tracing_data *tdata)
{
	if (tdata->temp) {
		record_file(tdata->temp_file, 0);
=======
int tracing_data_put(struct tracing_data *tdata)
{
	int err = 0;

	if (tdata->temp) {
		err = record_file(tdata->temp_file, 0);
>>>>>>> refs/remotes/origin/master
		unlink(tdata->temp_file);
	}

	free(tdata);
<<<<<<< HEAD
=======
	return err;
>>>>>>> refs/remotes/origin/master
}

int read_tracing_data(int fd, struct list_head *pattrs)
{
<<<<<<< HEAD
=======
	int err;
>>>>>>> refs/remotes/origin/master
	struct tracing_data *tdata;

	/*
	 * We work over the real file, so we can write data
	 * directly, no temp file is needed.
	 */
	tdata = tracing_data_get(pattrs, fd, false);
	if (!tdata)
		return -ENOMEM;

<<<<<<< HEAD
	tracing_data_put(tdata);
	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = tracing_data_put(tdata);
	return err;
>>>>>>> refs/remotes/origin/master
}
