/*
 * Copyright (C) 2009, Steven Rostedt <srostedt@redhat.com>
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
#define _FILE_OFFSET_BITS 64

=======
>>>>>>> refs/remotes/origin/master
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
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

#include "../perf.h"
#include "util.h"
#include "trace-event.h"

static int input_fd;

<<<<<<< HEAD
static int read_page;

int file_bigendian;
int host_bigendian;
static int long_size;

static unsigned long	page_size;

static ssize_t calc_data_size;
static bool repipe;

static int do_read(int fd, void *buf, int size)
=======
static ssize_t trace_data_size;
static bool repipe;

static int __do_read(int fd, void *buf, int size)
>>>>>>> refs/remotes/origin/master
{
	int rsize = size;

	while (size) {
		int ret = read(fd, buf, size);

		if (ret <= 0)
			return -1;

		if (repipe) {
			int retw = write(STDOUT_FILENO, buf, ret);

<<<<<<< HEAD
			if (retw <= 0 || retw != ret)
				die("repiping input file");
=======
			if (retw <= 0 || retw != ret) {
				pr_debug("repiping input file");
				return -1;
			}
>>>>>>> refs/remotes/origin/master
		}

		size -= ret;
		buf += ret;
	}

	return rsize;
}

<<<<<<< HEAD
static int read_or_die(void *data, int size)
{
	int r;

	r = do_read(input_fd, data, size);
	if (r <= 0)
		die("reading input file (size expected=%d received=%d)",
		    size, r);

	if (calc_data_size)
		calc_data_size += r;
=======
static int do_read(void *data, int size)
{
	int r;

	r = __do_read(input_fd, data, size);
	if (r <= 0) {
		pr_debug("reading input file (size expected=%d received=%d)",
			 size, r);
		return -1;
	}

	trace_data_size += r;
>>>>>>> refs/remotes/origin/master

	return r;
}

/* If it fails, the next read will report it */
static void skip(int size)
{
	char buf[BUFSIZ];
	int r;

	while (size) {
		r = size > BUFSIZ ? BUFSIZ : size;
<<<<<<< HEAD
		read_or_die(buf, r);
=======
		do_read(buf, r);
>>>>>>> refs/remotes/origin/master
		size -= r;
	};
}

<<<<<<< HEAD
static unsigned int read4(void)
{
	unsigned int data;

	read_or_die(&data, 4);
	return __data2host4(data);
}

static unsigned long long read8(void)
{
	unsigned long long data;

	read_or_die(&data, 8);
	return __data2host8(data);
=======
static unsigned int read4(struct pevent *pevent)
{
	unsigned int data;

	if (do_read(&data, 4) < 0)
		return 0;
	return __data2host4(pevent, data);
}

static unsigned long long read8(struct pevent *pevent)
{
	unsigned long long data;

	if (do_read(&data, 8) < 0)
		return 0;
	return __data2host8(pevent, data);
>>>>>>> refs/remotes/origin/master
}

static char *read_string(void)
{
	char buf[BUFSIZ];
	char *str = NULL;
	int size = 0;
	off_t r;
	char c;

	for (;;) {
		r = read(input_fd, &c, 1);
<<<<<<< HEAD
		if (r < 0)
			die("reading input file");

		if (!r)
			die("no data");
=======
		if (r < 0) {
			pr_debug("reading input file");
			goto out;
		}

		if (!r) {
			pr_debug("no data");
			goto out;
		}
>>>>>>> refs/remotes/origin/master

		if (repipe) {
			int retw = write(STDOUT_FILENO, &c, 1);

<<<<<<< HEAD
			if (retw <= 0 || retw != r)
				die("repiping input file string");
=======
			if (retw <= 0 || retw != r) {
				pr_debug("repiping input file string");
				goto out;
			}
>>>>>>> refs/remotes/origin/master
		}

		buf[size++] = c;

		if (!c)
			break;
	}

<<<<<<< HEAD
	if (calc_data_size)
		calc_data_size += size;

	str = malloc_or_die(size);
	memcpy(str, buf, size);

	return str;
}

static void read_proc_kallsyms(void)
=======
	trace_data_size += size;

	str = malloc(size);
	if (str)
		memcpy(str, buf, size);
out:
	return str;
}

static int read_proc_kallsyms(struct pevent *pevent)
>>>>>>> refs/remotes/origin/master
{
	unsigned int size;
	char *buf;

<<<<<<< HEAD
	size = read4();
	if (!size)
		return;

	buf = malloc_or_die(size + 1);
	read_or_die(buf, size);
	buf[size] = '\0';

	parse_proc_kallsyms(buf, size);

	free(buf);
}

static void read_ftrace_printk(void)
=======
	size = read4(pevent);
	if (!size)
		return 0;

	buf = malloc(size + 1);
	if (buf == NULL)
		return -1;

	if (do_read(buf, size) < 0) {
		free(buf);
		return -1;
	}
	buf[size] = '\0';

	parse_proc_kallsyms(pevent, buf, size);

	free(buf);
	return 0;
}

static int read_ftrace_printk(struct pevent *pevent)
>>>>>>> refs/remotes/origin/master
{
	unsigned int size;
	char *buf;

<<<<<<< HEAD
	size = read4();
	if (!size)
		return;

	buf = malloc_or_die(size);
	read_or_die(buf, size);

	parse_ftrace_printk(buf, size);

	free(buf);
}

static void read_header_files(void)
{
	unsigned long long size;
	char *header_event;
	char buf[BUFSIZ];

	read_or_die(buf, 12);

	if (memcmp(buf, "header_page", 12) != 0)
		die("did not read header page");

	size = read8();
	skip(size);

	/*
	 * The size field in the page is of type long,
	 * use that instead, since it represents the kernel.
	 */
	long_size = header_page_size_size;

	read_or_die(buf, 13);
	if (memcmp(buf, "header_event", 13) != 0)
		die("did not read header event");

	size = read8();
	header_event = malloc_or_die(size);
	read_or_die(header_event, size);
	free(header_event);
}

static void read_ftrace_file(unsigned long long size)
{
	char *buf;

	buf = malloc_or_die(size);
	read_or_die(buf, size);
	parse_ftrace_file(buf, size);
	free(buf);
}

static void read_event_file(char *sys, unsigned long long size)
{
	char *buf;

	buf = malloc_or_die(size);
	read_or_die(buf, size);
	parse_event_file(buf, size, sys);
	free(buf);
}

static void read_ftrace_files(void)
=======
	/* it can have 0 size */
	size = read4(pevent);
	if (!size)
		return 0;

	buf = malloc(size);
	if (buf == NULL)
		return -1;

	if (do_read(buf, size) < 0) {
		free(buf);
		return -1;
	}

	parse_ftrace_printk(pevent, buf, size);

	free(buf);
	return 0;
}

static int read_header_files(struct pevent *pevent)
{
	unsigned long long size;
	char *header_page;
	char buf[BUFSIZ];
	int ret = 0;

	if (do_read(buf, 12) < 0)
		return -1;

	if (memcmp(buf, "header_page", 12) != 0) {
		pr_debug("did not read header page");
		return -1;
	}

	size = read8(pevent);

	header_page = malloc(size);
	if (header_page == NULL)
		return -1;

	if (do_read(header_page, size) < 0) {
		pr_debug("did not read header page");
		free(header_page);
		return -1;
	}

	if (!pevent_parse_header_page(pevent, header_page, size,
				      pevent_get_long_size(pevent))) {
		/*
		 * The commit field in the page is of type long,
		 * use that instead, since it represents the kernel.
		 */
		pevent_set_long_size(pevent, pevent->header_page_size_size);
	}
	free(header_page);

	if (do_read(buf, 13) < 0)
		return -1;

	if (memcmp(buf, "header_event", 13) != 0) {
		pr_debug("did not read header event");
		return -1;
	}

	size = read8(pevent);
	skip(size);

	return ret;
}

static int read_ftrace_file(struct pevent *pevent, unsigned long long size)
{
	char *buf;

	buf = malloc(size);
	if (buf == NULL)
		return -1;

	if (do_read(buf, size) < 0) {
		free(buf);
		return -1;
	}

	parse_ftrace_file(pevent, buf, size);
	free(buf);
	return 0;
}

static int read_event_file(struct pevent *pevent, char *sys,
			    unsigned long long size)
{
	char *buf;

	buf = malloc(size);
	if (buf == NULL)
		return -1;

	if (do_read(buf, size) < 0) {
		free(buf);
		return -1;
	}

	parse_event_file(pevent, buf, size, sys);
	free(buf);
	return 0;
}

static int read_ftrace_files(struct pevent *pevent)
>>>>>>> refs/remotes/origin/master
{
	unsigned long long size;
	int count;
	int i;
<<<<<<< HEAD

	count = read4();

	for (i = 0; i < count; i++) {
		size = read8();
		read_ftrace_file(size);
	}
}

static void read_event_files(void)
=======
	int ret;

	count = read4(pevent);

	for (i = 0; i < count; i++) {
		size = read8(pevent);
		ret = read_ftrace_file(pevent, size);
		if (ret)
			return ret;
	}
	return 0;
}

static int read_event_files(struct pevent *pevent)
>>>>>>> refs/remotes/origin/master
{
	unsigned long long size;
	char *sys;
	int systems;
	int count;
	int i,x;
<<<<<<< HEAD

	systems = read4();

	for (i = 0; i < systems; i++) {
		sys = read_string();

		count = read4();
		for (x=0; x < count; x++) {
			size = read8();
			read_event_file(sys, size);
		}
	}
}

struct cpu_data {
	unsigned long long	offset;
	unsigned long long	size;
	unsigned long long	timestamp;
	struct record		*next;
	char			*page;
	int			cpu;
	int			index;
	int			page_size;
};

static struct cpu_data *cpu_data;

static void update_cpu_data_index(int cpu)
{
	cpu_data[cpu].offset += page_size;
	cpu_data[cpu].size -= page_size;
	cpu_data[cpu].index = 0;
}

static void get_next_page(int cpu)
{
	off_t save_seek;
	off_t ret;

	if (!cpu_data[cpu].page)
		return;

	if (read_page) {
		if (cpu_data[cpu].size <= page_size) {
			free(cpu_data[cpu].page);
			cpu_data[cpu].page = NULL;
			return;
		}

		update_cpu_data_index(cpu);

		/* other parts of the code may expect the pointer to not move */
		save_seek = lseek(input_fd, 0, SEEK_CUR);

		ret = lseek(input_fd, cpu_data[cpu].offset, SEEK_SET);
		if (ret == (off_t)-1)
			die("failed to lseek");
		ret = read(input_fd, cpu_data[cpu].page, page_size);
		if (ret < 0)
			die("failed to read page");

		/* reset the file pointer back */
		lseek(input_fd, save_seek, SEEK_SET);

		return;
	}

	munmap(cpu_data[cpu].page, page_size);
	cpu_data[cpu].page = NULL;

	if (cpu_data[cpu].size <= page_size)
		return;

	update_cpu_data_index(cpu);

	cpu_data[cpu].page = mmap(NULL, page_size, PROT_READ, MAP_PRIVATE,
				  input_fd, cpu_data[cpu].offset);
	if (cpu_data[cpu].page == MAP_FAILED)
		die("failed to mmap cpu %d at offset 0x%llx",
		    cpu, cpu_data[cpu].offset);
}

static unsigned int type_len4host(unsigned int type_len_ts)
{
	if (file_bigendian)
		return (type_len_ts >> 27) & ((1 << 5) - 1);
	else
		return type_len_ts & ((1 << 5) - 1);
}

static unsigned int ts4host(unsigned int type_len_ts)
{
	if (file_bigendian)
		return type_len_ts & ((1 << 27) - 1);
	else
		return type_len_ts >> 5;
}

static int calc_index(void *ptr, int cpu)
{
	return (unsigned long)ptr - (unsigned long)cpu_data[cpu].page;
}

struct record *trace_peek_data(int cpu)
{
	struct record *data;
	void *page = cpu_data[cpu].page;
	int idx = cpu_data[cpu].index;
	void *ptr = page + idx;
	unsigned long long extend;
	unsigned int type_len_ts;
	unsigned int type_len;
	unsigned int delta;
	unsigned int length = 0;

	if (cpu_data[cpu].next)
		return cpu_data[cpu].next;

	if (!page)
		return NULL;

	if (!idx) {
		/* FIXME: handle header page */
		if (header_page_ts_size != 8)
			die("expected a long long type for timestamp");
		cpu_data[cpu].timestamp = data2host8(ptr);
		ptr += 8;
		switch (header_page_size_size) {
		case 4:
			cpu_data[cpu].page_size = data2host4(ptr);
			ptr += 4;
			break;
		case 8:
			cpu_data[cpu].page_size = data2host8(ptr);
			ptr += 8;
			break;
		default:
			die("bad long size");
		}
		ptr = cpu_data[cpu].page + header_page_data_offset;
	}

read_again:
	idx = calc_index(ptr, cpu);

	if (idx >= cpu_data[cpu].page_size) {
		get_next_page(cpu);
		return trace_peek_data(cpu);
	}

	type_len_ts = data2host4(ptr);
	ptr += 4;

	type_len = type_len4host(type_len_ts);
	delta = ts4host(type_len_ts);

	switch (type_len) {
	case RINGBUF_TYPE_PADDING:
		if (!delta)
			die("error, hit unexpected end of page");
		length = data2host4(ptr);
		ptr += 4;
		length *= 4;
		ptr += length;
		goto read_again;

	case RINGBUF_TYPE_TIME_EXTEND:
		extend = data2host4(ptr);
		ptr += 4;
		extend <<= TS_SHIFT;
		extend += delta;
		cpu_data[cpu].timestamp += extend;
		goto read_again;

	case RINGBUF_TYPE_TIME_STAMP:
		ptr += 12;
		break;
	case 0:
		length = data2host4(ptr);
		ptr += 4;
		die("here! length=%d", length);
		break;
	default:
		length = type_len * 4;
		break;
	}

	cpu_data[cpu].timestamp += delta;

	data = malloc_or_die(sizeof(*data));
	memset(data, 0, sizeof(*data));

	data->ts = cpu_data[cpu].timestamp;
	data->size = length;
	data->data = ptr;
	ptr += length;

	cpu_data[cpu].index = calc_index(ptr, cpu);
	cpu_data[cpu].next = data;

	return data;
}

struct record *trace_read_data(int cpu)
{
	struct record *data;

	data = trace_peek_data(cpu);
	cpu_data[cpu].next = NULL;

	return data;
}

ssize_t trace_report(int fd, bool __repipe)
=======
	int ret;

	systems = read4(pevent);

	for (i = 0; i < systems; i++) {
		sys = read_string();
		if (sys == NULL)
			return -1;

		count = read4(pevent);

		for (x=0; x < count; x++) {
			size = read8(pevent);
			ret = read_event_file(pevent, sys, size);
			if (ret)
				return ret;
		}
	}
	return 0;
}

ssize_t trace_report(int fd, struct trace_event *tevent, bool __repipe)
>>>>>>> refs/remotes/origin/master
{
	char buf[BUFSIZ];
	char test[] = { 23, 8, 68 };
	char *version;
	int show_version = 0;
	int show_funcs = 0;
	int show_printk = 0;
<<<<<<< HEAD
	ssize_t size;

	calc_data_size = 1;
	repipe = __repipe;

	input_fd = fd;

	read_or_die(buf, 3);
	if (memcmp(buf, test, 3) != 0)
		die("no trace data in the file");

	read_or_die(buf, 7);
	if (memcmp(buf, "tracing", 7) != 0)
		die("not a trace file (missing 'tracing' tag)");

	version = read_string();
=======
	ssize_t size = -1;
	int file_bigendian;
	int host_bigendian;
	int file_long_size;
	int file_page_size;
	struct pevent *pevent = NULL;
	int err;

	repipe = __repipe;
	input_fd = fd;

	if (do_read(buf, 3) < 0)
		return -1;
	if (memcmp(buf, test, 3) != 0) {
		pr_debug("no trace data in the file");
		return -1;
	}

	if (do_read(buf, 7) < 0)
		return -1;
	if (memcmp(buf, "tracing", 7) != 0) {
		pr_debug("not a trace file (missing 'tracing' tag)");
		return -1;
	}

	version = read_string();
	if (version == NULL)
		return -1;
>>>>>>> refs/remotes/origin/master
	if (show_version)
		printf("version = %s\n", version);
	free(version);

<<<<<<< HEAD
	read_or_die(buf, 1);
	file_bigendian = buf[0];
	host_bigendian = bigendian();

	read_or_die(buf, 1);
	long_size = buf[0];

	page_size = read4();

	read_header_files();

	read_ftrace_files();
	read_event_files();
	read_proc_kallsyms();
	read_ftrace_printk();

	size = calc_data_size - 1;
	calc_data_size = 0;
	repipe = false;

	if (show_funcs) {
		print_funcs();
		return size;
	}
	if (show_printk) {
		print_printk();
		return size;
	}

=======
	if (do_read(buf, 1) < 0)
		return -1;
	file_bigendian = buf[0];
	host_bigendian = bigendian();

	if (trace_event__init(tevent)) {
		pr_debug("trace_event__init failed");
		goto out;
	}

	pevent = tevent->pevent;

	pevent_set_flag(pevent, PEVENT_NSEC_OUTPUT);
	pevent_set_file_bigendian(pevent, file_bigendian);
	pevent_set_host_bigendian(pevent, host_bigendian);

	if (do_read(buf, 1) < 0)
		goto out;
	file_long_size = buf[0];

	file_page_size = read4(pevent);
	if (!file_page_size)
		goto out;

	pevent_set_long_size(pevent, file_long_size);
	pevent_set_page_size(pevent, file_page_size);

	err = read_header_files(pevent);
	if (err)
		goto out;
	err = read_ftrace_files(pevent);
	if (err)
		goto out;
	err = read_event_files(pevent);
	if (err)
		goto out;
	err = read_proc_kallsyms(pevent);
	if (err)
		goto out;
	err = read_ftrace_printk(pevent);
	if (err)
		goto out;

	size = trace_data_size;
	repipe = false;

	if (show_funcs) {
		pevent_print_funcs(pevent);
	} else if (show_printk) {
		pevent_print_printk(pevent);
	}

	pevent = NULL;

out:
	if (pevent)
		trace_event__cleanup(tevent);
>>>>>>> refs/remotes/origin/master
	return size;
}
