/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1999,2001-2004, 2006 Silicon Graphics, Inc.  All Rights Reserved.
 *
 * Module to export the system's Firmware Interface Tables, including
 * PROM revision numbers and banners, in /proc
 */
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
<<<<<<< HEAD
#include <linux/nodemask.h>
<<<<<<< HEAD
#include <asm/system.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/seq_file.h>
#include <linux/nodemask.h>
>>>>>>> refs/remotes/origin/master
#include <asm/io.h>
#include <asm/sn/sn_sal.h>
#include <asm/sn/sn_cpuid.h>
#include <asm/sn/addrs.h>

MODULE_DESCRIPTION("PROM version reporting for /proc");
MODULE_AUTHOR("Chad Talbott");
MODULE_LICENSE("GPL");

/* Standard Intel FIT entry types */
#define FIT_ENTRY_FIT_HEADER	0x00	/* FIT header entry */
#define FIT_ENTRY_PAL_B		0x01	/* PAL_B entry */
/* Entries 0x02 through 0x0D reserved by Intel */
#define FIT_ENTRY_PAL_A_PROC	0x0E	/* Processor-specific PAL_A entry */
#define FIT_ENTRY_PAL_A		0x0F	/* PAL_A entry, same as... */
#define FIT_ENTRY_PAL_A_GEN	0x0F	/* ...Generic PAL_A entry */
#define FIT_ENTRY_UNUSED	0x7F	/* Unused (reserved by Intel?) */
/* OEM-defined entries range from 0x10 to 0x7E. */
#define FIT_ENTRY_SAL_A		0x10	/* SAL_A entry */
#define FIT_ENTRY_SAL_B		0x11	/* SAL_B entry */
#define FIT_ENTRY_SALRUNTIME	0x12	/* SAL runtime entry */
#define FIT_ENTRY_EFI		0x1F	/* EFI entry */
#define FIT_ENTRY_FPSWA		0x20	/* embedded fpswa entry */
#define FIT_ENTRY_VMLINUX	0x21	/* embedded vmlinux entry */

#define FIT_MAJOR_SHIFT	(32 + 8)
#define FIT_MAJOR_MASK	((1 << 8) - 1)
#define FIT_MINOR_SHIFT	32
#define FIT_MINOR_MASK	((1 << 8) - 1)

#define FIT_MAJOR(q)	\
	((unsigned) ((q) >> FIT_MAJOR_SHIFT) & FIT_MAJOR_MASK)
#define FIT_MINOR(q)	\
	((unsigned) ((q) >> FIT_MINOR_SHIFT) & FIT_MINOR_MASK)

#define FIT_TYPE_SHIFT	(32 + 16)
#define FIT_TYPE_MASK	((1 << 7) - 1)

#define FIT_TYPE(q)	\
	((unsigned) ((q) >> FIT_TYPE_SHIFT) & FIT_TYPE_MASK)

struct fit_type_map_t {
	unsigned char type;
	const char *name;
};

static const struct fit_type_map_t fit_entry_types[] = {
	{FIT_ENTRY_FIT_HEADER, "FIT Header"},
	{FIT_ENTRY_PAL_A_GEN, "Generic PAL_A"},
	{FIT_ENTRY_PAL_A_PROC, "Processor-specific PAL_A"},
	{FIT_ENTRY_PAL_A, "PAL_A"},
	{FIT_ENTRY_PAL_B, "PAL_B"},
	{FIT_ENTRY_SAL_A, "SAL_A"},
	{FIT_ENTRY_SAL_B, "SAL_B"},
	{FIT_ENTRY_SALRUNTIME, "SAL runtime"},
	{FIT_ENTRY_EFI, "EFI"},
	{FIT_ENTRY_VMLINUX, "Embedded Linux"},
	{FIT_ENTRY_FPSWA, "Embedded FPSWA"},
	{FIT_ENTRY_UNUSED, "Unused"},
	{0xff, "Error"},
};

static const char *fit_type_name(unsigned char type)
{
	struct fit_type_map_t const *mapp;

	for (mapp = fit_entry_types; mapp->type != 0xff; mapp++)
		if (type == mapp->type)
			return mapp->name;

	if ((type > FIT_ENTRY_PAL_A) && (type < FIT_ENTRY_UNUSED))
		return "OEM type";
	if ((type > FIT_ENTRY_PAL_B) && (type < FIT_ENTRY_PAL_A))
		return "Reserved";

	return "Unknown type";
}

static int
get_fit_entry(unsigned long nasid, int index, unsigned long *fentry,
	      char *banner, int banlen)
{
	return ia64_sn_get_fit_compt(nasid, index, fentry, banner, banlen);
}


/*
 * These two routines display the FIT table for each node.
 */
<<<<<<< HEAD
static int dump_fit_entry(char *page, unsigned long *fentry)
=======
static void dump_fit_entry(struct seq_file *m, unsigned long *fentry)
>>>>>>> refs/remotes/origin/master
{
	unsigned type;

	type = FIT_TYPE(fentry[1]);
<<<<<<< HEAD
	return sprintf(page, "%02x %-25s %x.%02x %016lx %u\n",
		       type,
		       fit_type_name(type),
		       FIT_MAJOR(fentry[1]), FIT_MINOR(fentry[1]),
		       fentry[0],
		       /* mult by sixteen to get size in bytes */
		       (unsigned)(fentry[1] & 0xffffff) * 16);
=======
	seq_printf(m, "%02x %-25s %x.%02x %016lx %u\n",
		   type,
		   fit_type_name(type),
		   FIT_MAJOR(fentry[1]), FIT_MINOR(fentry[1]),
		   fentry[0],
		   /* mult by sixteen to get size in bytes */
		   (unsigned)(fentry[1] & 0xffffff) * 16);
>>>>>>> refs/remotes/origin/master
}


/*
 * We assume that the fit table will be small enough that we can print
 * the whole thing into one page.  (This is true for our default 16kB
 * pages -- each entry is about 60 chars wide when printed.)  I read
 * somewhere that the maximum size of the FIT is 128 entries, so we're
 * OK except for 4kB pages (and no one is going to do that on SN
 * anyway).
 */
<<<<<<< HEAD
static int
dump_fit(char *page, unsigned long nasid)
{
	unsigned long fentry[2];
	int index;
	char *p;

	p = page;
=======
static int proc_fit_show(struct seq_file *m, void *v)
{
	unsigned long nasid = (unsigned long)m->private;
	unsigned long fentry[2];
	int index;

>>>>>>> refs/remotes/origin/master
	for (index=0;;index++) {
		BUG_ON(index * 60 > PAGE_SIZE);
		if (get_fit_entry(nasid, index, fentry, NULL, 0))
			break;
<<<<<<< HEAD
		p += dump_fit_entry(p, fentry);
	}

	return p - page;
}

static int
dump_version(char *page, unsigned long nasid)
{
	unsigned long fentry[2];
	char banner[128];
	int index;
	int len;
=======
		dump_fit_entry(m, fentry);
	}
	return 0;
}

static int proc_fit_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_fit_show, PDE_DATA(inode));
}

static const struct file_operations proc_fit_fops = {
	.open		= proc_fit_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int proc_version_show(struct seq_file *m, void *v)
{
	unsigned long nasid = (unsigned long)m->private;
	unsigned long fentry[2];
	char banner[128];
	int index;
>>>>>>> refs/remotes/origin/master

	for (index = 0; ; index++) {
		if (get_fit_entry(nasid, index, fentry, banner,
				  sizeof(banner)))
			return 0;
		if (FIT_TYPE(fentry[1]) == FIT_ENTRY_SAL_A)
			break;
	}

<<<<<<< HEAD
	len = sprintf(page, "%x.%02x\n", FIT_MAJOR(fentry[1]),
		      FIT_MINOR(fentry[1]));
	page += len;

	if (banner[0])
		len += snprintf(page, PAGE_SIZE-len, "%s\n", banner);

	return len;
}

/* same as in proc_misc.c */
static int
proc_calc_metrics(char *page, char **start, off_t off, int count, int *eof,
		  int len)
{
	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}

static int
read_version_entry(char *page, char **start, off_t off, int count, int *eof,
		   void *data)
{
	int len;

	/* data holds the NASID of the node */
	len = dump_version(page, (unsigned long)data);
	len = proc_calc_metrics(page, start, off, count, eof, len);
	return len;
}

static int
read_fit_entry(char *page, char **start, off_t off, int count, int *eof,
	       void *data)
{
	int len;

	/* data holds the NASID of the node */
	len = dump_fit(page, (unsigned long)data);
	len = proc_calc_metrics(page, start, off, count, eof, len);

	return len;
}
=======
	seq_printf(m, "%x.%02x\n", FIT_MAJOR(fentry[1]), FIT_MINOR(fentry[1]));

	if (banner[0])
		seq_printf(m, "%s\n", banner);
	return 0;
}

static int proc_version_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_version_show, PDE_DATA(inode));
}

static const struct file_operations proc_version_fops = {
	.open		= proc_version_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
>>>>>>> refs/remotes/origin/master

/* module entry points */
int __init prominfo_init(void);
void __exit prominfo_exit(void);

module_init(prominfo_init);
module_exit(prominfo_exit);

<<<<<<< HEAD
static struct proc_dir_entry **proc_entries;
static struct proc_dir_entry *sgi_prominfo_entry;

=======
>>>>>>> refs/remotes/origin/master
#define NODE_NAME_LEN 11

int __init prominfo_init(void)
{
<<<<<<< HEAD
	struct proc_dir_entry **entp;
	cnodeid_t cnodeid;
	unsigned long nasid;
	int size;
	char name[NODE_NAME_LEN];
=======
	struct proc_dir_entry *sgi_prominfo_entry;
	cnodeid_t cnodeid;
>>>>>>> refs/remotes/origin/master

	if (!ia64_platform_is("sn2"))
		return 0;

<<<<<<< HEAD
	size = num_online_nodes() * sizeof(struct proc_dir_entry *);
	proc_entries = kzalloc(size, GFP_KERNEL);
	if (!proc_entries)
		return -ENOMEM;

	sgi_prominfo_entry = proc_mkdir("sgi_prominfo", NULL);

	entp = proc_entries;
	for_each_online_node(cnodeid) {
		sprintf(name, "node%d", cnodeid);
		*entp = proc_mkdir(name, sgi_prominfo_entry);
		nasid = cnodeid_to_nasid(cnodeid);
		create_proc_read_entry("fit", 0, *entp, read_fit_entry,
					   (void *)nasid);
		create_proc_read_entry("version", 0, *entp,
					   read_version_entry, (void *)nasid);
		entp++;
	}

=======
	sgi_prominfo_entry = proc_mkdir("sgi_prominfo", NULL);
	if (!sgi_prominfo_entry)
		return -ENOMEM;

	for_each_online_node(cnodeid) {
		struct proc_dir_entry *dir;
		unsigned long nasid;
		char name[NODE_NAME_LEN];

		sprintf(name, "node%d", cnodeid);
		dir = proc_mkdir(name, sgi_prominfo_entry);
		if (!dir)
			continue;
		nasid = cnodeid_to_nasid(cnodeid);
		proc_create_data("fit", 0, dir,
				 &proc_fit_fops, (void *)nasid);
		proc_create_data("version", 0, dir,
				 &proc_version_fops, (void *)nasid);
	}
>>>>>>> refs/remotes/origin/master
	return 0;
}

void __exit prominfo_exit(void)
{
<<<<<<< HEAD
	struct proc_dir_entry **entp;
	unsigned int cnodeid;
	char name[NODE_NAME_LEN];

	entp = proc_entries;
	for_each_online_node(cnodeid) {
		remove_proc_entry("fit", *entp);
		remove_proc_entry("version", *entp);
		sprintf(name, "node%d", cnodeid);
		remove_proc_entry(name, sgi_prominfo_entry);
		entp++;
	}
	remove_proc_entry("sgi_prominfo", NULL);
	kfree(proc_entries);
=======
	remove_proc_subtree("sgi_prominfo", NULL);
>>>>>>> refs/remotes/origin/master
}
