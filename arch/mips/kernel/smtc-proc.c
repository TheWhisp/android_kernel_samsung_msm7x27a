/*
 * /proc hooks for SMTC kernel
 * Copyright (C) 2005 Mips Technologies, Inc
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/cpumask.h>
#include <linux/interrupt.h>

#include <asm/cpu.h>
#include <asm/processor.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
#include <asm/system.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master
#include <asm/hardirq.h>
#include <asm/mmu_context.h>
#include <asm/mipsregs.h>
#include <asm/cacheflush.h>
#include <linux/proc_fs.h>
<<<<<<< HEAD
=======
#include <linux/seq_file.h>
>>>>>>> refs/remotes/origin/master

#include <asm/smtc_proc.h>

/*
 * /proc diagnostic and statistics hooks
 */

/*
 * Statistics gathered
 */
unsigned long selfipis[NR_CPUS];

struct smtc_cpu_proc smtc_cpu_stats[NR_CPUS];

<<<<<<< HEAD
static struct proc_dir_entry *smtc_stats;

atomic_t smtc_fpu_recoveries;

static int proc_read_smtc(char *page, char **start, off_t off,
                          int count, int *eof, void *data)
{
	int totalen = 0;
	int len;
	int i;
	extern unsigned long ebase;

	len = sprintf(page, "SMTC Status Word: 0x%08x\n", smtc_status);
	totalen += len;
	page += len;
	len = sprintf(page, "Config7: 0x%08x\n", read_c0_config7());
	totalen += len;
	page += len;
	len = sprintf(page, "EBASE: 0x%08lx\n", ebase);
	totalen += len;
	page += len;
	len = sprintf(page, "Counter Interrupts taken per CPU (TC)\n");
	totalen += len;
	page += len;
	for (i=0; i < NR_CPUS; i++) {
		len = sprintf(page, "%d: %ld\n", i, smtc_cpu_stats[i].timerints);
		totalen += len;
		page += len;
	}
	len = sprintf(page, "Self-IPIs by CPU:\n");
	totalen += len;
	page += len;
	for(i = 0; i < NR_CPUS; i++) {
		len = sprintf(page, "%d: %ld\n", i, smtc_cpu_stats[i].selfipis);
		totalen += len;
		page += len;
	}
	len = sprintf(page, "%d Recoveries of \"stolen\" FPU\n",
	              atomic_read(&smtc_fpu_recoveries));
	totalen += len;
	page += len;

	return totalen;
}

=======
atomic_t smtc_fpu_recoveries;

static int smtc_proc_show(struct seq_file *m, void *v)
{
	int i;
	extern unsigned long ebase;

	seq_printf(m, "SMTC Status Word: 0x%08x\n", smtc_status);
	seq_printf(m, "Config7: 0x%08x\n", read_c0_config7());
	seq_printf(m, "EBASE: 0x%08lx\n", ebase);
	seq_printf(m, "Counter Interrupts taken per CPU (TC)\n");
	for (i=0; i < NR_CPUS; i++)
		seq_printf(m, "%d: %ld\n", i, smtc_cpu_stats[i].timerints);
	seq_printf(m, "Self-IPIs by CPU:\n");
	for(i = 0; i < NR_CPUS; i++)
		seq_printf(m, "%d: %ld\n", i, smtc_cpu_stats[i].selfipis);
	seq_printf(m, "%d Recoveries of \"stolen\" FPU\n",
		   atomic_read(&smtc_fpu_recoveries));
	return 0;
}

static int smtc_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, smtc_proc_show, NULL);
}

static const struct file_operations smtc_proc_fops = {
	.open		= smtc_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

>>>>>>> refs/remotes/origin/master
void init_smtc_stats(void)
{
	int i;

	for (i=0; i<NR_CPUS; i++) {
		smtc_cpu_stats[i].timerints = 0;
		smtc_cpu_stats[i].selfipis = 0;
	}

	atomic_set(&smtc_fpu_recoveries, 0);

<<<<<<< HEAD
	smtc_stats = create_proc_read_entry("smtc", 0444, NULL,
	                                    proc_read_smtc, NULL);
=======
	proc_create("smtc", 0444, NULL, &smtc_proc_fops);
>>>>>>> refs/remotes/origin/master
}
