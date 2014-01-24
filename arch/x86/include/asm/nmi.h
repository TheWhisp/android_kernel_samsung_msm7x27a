#ifndef _ASM_X86_NMI_H
#define _ASM_X86_NMI_H

#include <linux/pm.h>
#include <asm/irq.h>
#include <asm/io.h>

#ifdef CONFIG_X86_LOCAL_APIC

extern int avail_to_resrv_perfctr_nmi_bit(unsigned int);
extern int reserve_perfctr_nmi(unsigned int);
extern void release_perfctr_nmi(unsigned int);
extern int reserve_evntsel_nmi(unsigned int);
extern void release_evntsel_nmi(unsigned int);

struct ctl_table;
extern int proc_nmi_enabled(struct ctl_table *, int ,
			void __user *, size_t *, loff_t *);
extern int unknown_nmi_panic;

<<<<<<< HEAD
void arch_trigger_all_cpu_backtrace(void);
#define arch_trigger_all_cpu_backtrace arch_trigger_all_cpu_backtrace
#endif

<<<<<<< HEAD
/*
 * Define some priorities for the nmi notifier call chain.
 *
 * Create a local nmi bit that has a higher priority than
 * external nmis, because the local ones are more frequent.
 *
 * Also setup some default high/normal/low settings for
 * subsystems to registers with.  Using 4 bits to separate
 * the priorities.  This can go a lot higher if needed be.
 */

#define NMI_LOCAL_SHIFT		16	/* randomly picked */
#define NMI_LOCAL_BIT		(1ULL << NMI_LOCAL_SHIFT)
#define NMI_HIGH_PRIOR		(1ULL << 8)
#define NMI_NORMAL_PRIOR	(1ULL << 4)
#define NMI_LOW_PRIOR		(1ULL << 0)
#define NMI_LOCAL_HIGH_PRIOR	(NMI_LOCAL_BIT | NMI_HIGH_PRIOR)
#define NMI_LOCAL_NORMAL_PRIOR	(NMI_LOCAL_BIT | NMI_NORMAL_PRIOR)
#define NMI_LOCAL_LOW_PRIOR	(NMI_LOCAL_BIT | NMI_LOW_PRIOR)

void stop_nmi(void);
void restart_nmi(void);
=======
=======
#endif /* CONFIG_X86_LOCAL_APIC */

>>>>>>> refs/remotes/origin/master
#define NMI_FLAG_FIRST	1

enum {
	NMI_LOCAL=0,
	NMI_UNKNOWN,
<<<<<<< HEAD
=======
	NMI_SERR,
	NMI_IO_CHECK,
>>>>>>> refs/remotes/origin/master
	NMI_MAX
};

#define NMI_DONE	0
#define NMI_HANDLED	1

typedef int (*nmi_handler_t)(unsigned int, struct pt_regs *);

<<<<<<< HEAD
int register_nmi_handler(unsigned int, nmi_handler_t, unsigned long,
			 const char *);
=======
struct nmiaction {
	struct list_head	list;
	nmi_handler_t		handler;
	unsigned long		flags;
	const char		*name;
};

#define register_nmi_handler(t, fn, fg, n, init...)	\
({							\
	static struct nmiaction init fn##_na = {	\
		.handler = (fn),			\
		.name = (n),				\
		.flags = (fg),				\
	};						\
	__register_nmi_handler((t), &fn##_na);		\
})

int __register_nmi_handler(unsigned int, struct nmiaction *);
>>>>>>> refs/remotes/origin/master

void unregister_nmi_handler(unsigned int, const char *);

void stop_nmi(void);
void restart_nmi(void);
void local_touch_nmi(void);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif /* _ASM_X86_NMI_H */
