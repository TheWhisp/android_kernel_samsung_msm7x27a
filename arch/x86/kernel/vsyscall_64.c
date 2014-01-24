/*
 *  Copyright (C) 2001 Andrea Arcangeli <andrea@suse.de> SuSE
 *  Copyright 2003 Andi Kleen, SuSE Labs.
 *
<<<<<<< HEAD
<<<<<<< HEAD
=======
 *  [ NOTE: this mechanism is now deprecated in favor of the vDSO. ]
 *
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *  [ NOTE: this mechanism is now deprecated in favor of the vDSO. ]
 *
>>>>>>> refs/remotes/origin/master
 *  Thanks to hpa@transmeta.com for some useful hint.
 *  Special thanks to Ingo Molnar for his early experience with
 *  a different vsyscall implementation for Linux/IA32 and for the name.
 *
 *  vsyscall 1 is located at -10Mbyte, vsyscall 2 is located
 *  at virtual address -10Mbyte+1024bytes etc... There are at max 4
 *  vsyscalls. One vsyscall can reserve more than 1 slot to avoid
 *  jumping out of line if necessary. We cannot add more with this
 *  mechanism because older kernels won't return -ENOSYS.
<<<<<<< HEAD
<<<<<<< HEAD
 *  If we want more than four we need a vDSO.
 *
 *  Note: the concept clashes with user mode linux. If you use UML and
 *  want per guest time just set the kernel.vsyscall64 sysctl to 0.
 */

/* Disable profiling for userspace code: */
#define DISABLE_BRANCH_PROFILING

=======
=======
>>>>>>> refs/remotes/origin/master
 *
 *  Note: the concept clashes with user mode linux.  UML users should
 *  use the vDSO.
 */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/time.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/seqlock.h>
#include <linux/jiffies.h>
#include <linux/sysctl.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/topology.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/clocksource.h>
=======
#include <linux/topology.h>
#include <linux/timekeeper_internal.h>
>>>>>>> refs/remotes/origin/master
#include <linux/getcpu.h>
#include <linux/cpu.h>
#include <linux/smp.h>
#include <linux/notifier.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <asm/vsyscall.h>
#include <asm/pgtable.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/syscalls.h>
#include <linux/ratelimit.h>

#include <asm/vsyscall.h>
#include <asm/pgtable.h>
#include <asm/compat.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <asm/page.h>
#include <asm/unistd.h>
#include <asm/fixmap.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/segment.h>
#include <asm/desc.h>
#include <asm/topology.h>
#include <asm/vgtod.h>
<<<<<<< HEAD
<<<<<<< HEAD

#define __vsyscall(nr) \
		__attribute__ ((unused, __section__(".vsyscall_" #nr))) notrace
#define __syscall_clobber "r11","cx","memory"

DEFINE_VVAR(int, vgetcpu_mode);
DEFINE_VVAR(struct vsyscall_gtod_data, vsyscall_gtod_data) =
{
	.lock = __SEQLOCK_UNLOCKED(__vsyscall_gtod_data.lock),
	.sysctl_enabled = 1,
};

void update_vsyscall_tz(void)
{
	unsigned long flags;

	write_seqlock_irqsave(&vsyscall_gtod_data.lock, flags);
	/* sys_tz has changed */
	vsyscall_gtod_data.sys_tz = sys_tz;
	write_sequnlock_irqrestore(&vsyscall_gtod_data.lock, flags);
=======
=======
>>>>>>> refs/remotes/origin/master
#include <asm/traps.h>

#define CREATE_TRACE_POINTS
#include "vsyscall_trace.h"

DEFINE_VVAR(int, vgetcpu_mode);
DEFINE_VVAR(struct vsyscall_gtod_data, vsyscall_gtod_data);

static enum { EMULATE, NATIVE, NONE } vsyscall_mode = EMULATE;

static int __init vsyscall_setup(char *str)
{
	if (str) {
		if (!strcmp("emulate", str))
			vsyscall_mode = EMULATE;
		else if (!strcmp("native", str))
			vsyscall_mode = NATIVE;
		else if (!strcmp("none", str))
			vsyscall_mode = NONE;
		else
			return -EINVAL;

		return 0;
	}

	return -EINVAL;
}
early_param("vsyscall", vsyscall_setup);

void update_vsyscall_tz(void)
{
	vsyscall_gtod_data.sys_tz = sys_tz;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

void update_vsyscall(struct timespec *wall_time, struct timespec *wtm,
			struct clocksource *clock, u32 mult)
{
<<<<<<< HEAD
	unsigned long flags;

	write_seqlock_irqsave(&vsyscall_gtod_data.lock, flags);
	/* copy vsyscall data */
	vsyscall_gtod_data.clock.vread = clock->vread;
	vsyscall_gtod_data.clock.cycle_last = clock->cycle_last;
	vsyscall_gtod_data.clock.mask = clock->mask;
	vsyscall_gtod_data.clock.mult = mult;
	vsyscall_gtod_data.clock.shift = clock->shift;
	vsyscall_gtod_data.wall_time_sec = wall_time->tv_sec;
	vsyscall_gtod_data.wall_time_nsec = wall_time->tv_nsec;
	vsyscall_gtod_data.wall_to_monotonic = *wtm;
	vsyscall_gtod_data.wall_time_coarse = __current_kernel_time();
	write_sequnlock_irqrestore(&vsyscall_gtod_data.lock, flags);
}

/* RED-PEN may want to readd seq locking, but then the variable should be
 * write-once.
 */
static __always_inline void do_get_tz(struct timezone * tz)
{
	*tz = VVAR(vsyscall_gtod_data).sys_tz;
}

static __always_inline int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	int ret;
	asm volatile("syscall"
		: "=a" (ret)
		: "0" (__NR_gettimeofday),"D" (tv),"S" (tz)
		: __syscall_clobber );
	return ret;
}

static __always_inline long time_syscall(long *t)
{
	long secs;
	asm volatile("syscall"
		: "=a" (secs)
		: "0" (__NR_time),"D" (t) : __syscall_clobber);
	return secs;
}

static __always_inline void do_vgettimeofday(struct timeval * tv)
{
	cycle_t now, base, mask, cycle_delta;
	unsigned seq;
	unsigned long mult, shift, nsec;
	cycle_t (*vread)(void);
	do {
		seq = read_seqbegin(&VVAR(vsyscall_gtod_data).lock);

		vread = VVAR(vsyscall_gtod_data).clock.vread;
		if (unlikely(!VVAR(vsyscall_gtod_data).sysctl_enabled ||
			     !vread)) {
			gettimeofday(tv,NULL);
			return;
		}

		now = vread();
		base = VVAR(vsyscall_gtod_data).clock.cycle_last;
		mask = VVAR(vsyscall_gtod_data).clock.mask;
		mult = VVAR(vsyscall_gtod_data).clock.mult;
		shift = VVAR(vsyscall_gtod_data).clock.shift;

		tv->tv_sec = VVAR(vsyscall_gtod_data).wall_time_sec;
		nsec = VVAR(vsyscall_gtod_data).wall_time_nsec;
	} while (read_seqretry(&VVAR(vsyscall_gtod_data).lock, seq));

	/* calculate interval: */
	cycle_delta = (now - base) & mask;
	/* convert to nsecs: */
	nsec += (cycle_delta * mult) >> shift;

	while (nsec >= NSEC_PER_SEC) {
		tv->tv_sec += 1;
		nsec -= NSEC_PER_SEC;
	}
	tv->tv_usec = nsec / NSEC_PER_USEC;
}

int __vsyscall(0) vgettimeofday(struct timeval * tv, struct timezone * tz)
{
	if (tv)
		do_vgettimeofday(tv);
	if (tz)
		do_get_tz(tz);
	return 0;
}

/* This will break when the xtime seconds get inaccurate, but that is
 * unlikely */
time_t __vsyscall(1) vtime(time_t *t)
{
	unsigned seq;
	time_t result;
	if (unlikely(!VVAR(vsyscall_gtod_data).sysctl_enabled))
		return time_syscall(t);

	do {
		seq = read_seqbegin(&VVAR(vsyscall_gtod_data).lock);

		result = VVAR(vsyscall_gtod_data).wall_time_sec;

	} while (read_seqretry(&VVAR(vsyscall_gtod_data).lock, seq));

	if (t)
		*t = result;
	return result;
}

/* Fast way to get current CPU and node.
   This helps to do per node and per CPU caches in user space.
   The result is not guaranteed without CPU affinity, but usually
   works out because the scheduler tries to keep a thread on the same
   CPU.

   tcache must point to a two element sized long array.
   All arguments can be NULL. */
long __vsyscall(2)
vgetcpu(unsigned *cpu, unsigned *node, struct getcpu_cache *tcache)
{
	unsigned int p;
	unsigned long j = 0;

	/* Fast cache - only recompute value once per jiffies and avoid
	   relatively costly rdtscp/cpuid otherwise.
	   This works because the scheduler usually keeps the process
	   on the same CPU and this syscall doesn't guarantee its
	   results anyways.
	   We do this here because otherwise user space would do it on
	   its own in a likely inferior way (no access to jiffies).
	   If you don't like it pass NULL. */
	if (tcache && tcache->blob[0] == (j = VVAR(jiffies))) {
		p = tcache->blob[1];
	} else if (VVAR(vgetcpu_mode) == VGETCPU_RDTSCP) {
		/* Load per CPU data from RDTSCP */
		native_read_tscp(&p);
	} else {
		/* Load per CPU data from GDT */
		asm("lsl %1,%0" : "=r" (p) : "r" (__PER_CPU_SEG));
	}
	if (tcache) {
		tcache->blob[0] = j;
		tcache->blob[1] = p;
	}
	if (cpu)
		*cpu = p & 0xfff;
	if (node)
		*node = p >> 12;
	return 0;
}

static long __vsyscall(3) venosys_1(void)
{
	return -ENOSYS;
}

#ifdef CONFIG_SYSCTL
static ctl_table kernel_table2[] = {
	{ .procname = "vsyscall64",
	  .data = &vsyscall_gtod_data.sysctl_enabled, .maxlen = sizeof(int),
	  .mode = 0644,
	  .proc_handler = proc_dointvec },
	{}
};

static ctl_table kernel_root_table2[] = {
	{ .procname = "kernel", .mode = 0555,
	  .child = kernel_table2 },
	{}
};
#endif

/* Assume __initcall executes before all user space. Hopefully kmod
   doesn't violate that. We'll find out if it does. */
=======
	struct timespec monotonic;

	write_seqcount_begin(&vsyscall_gtod_data.seq);

	/* copy vsyscall data */
	vsyscall_gtod_data.clock.vclock_mode	= clock->archdata.vclock_mode;
	vsyscall_gtod_data.clock.cycle_last	= clock->cycle_last;
	vsyscall_gtod_data.clock.mask		= clock->mask;
	vsyscall_gtod_data.clock.mult		= mult;
	vsyscall_gtod_data.clock.shift		= clock->shift;

	vsyscall_gtod_data.wall_time_sec	= wall_time->tv_sec;
	vsyscall_gtod_data.wall_time_nsec	= wall_time->tv_nsec;

	monotonic = timespec_add(*wall_time, *wtm);
	vsyscall_gtod_data.monotonic_time_sec	= monotonic.tv_sec;
	vsyscall_gtod_data.monotonic_time_nsec	= monotonic.tv_nsec;

	vsyscall_gtod_data.wall_time_coarse	= __current_kernel_time();
	vsyscall_gtod_data.monotonic_time_coarse =
		timespec_add(vsyscall_gtod_data.wall_time_coarse, *wtm);

	write_seqcount_end(&vsyscall_gtod_data.seq);
=======
}

void update_vsyscall(struct timekeeper *tk)
{
	struct vsyscall_gtod_data *vdata = &vsyscall_gtod_data;

	write_seqcount_begin(&vdata->seq);

	/* copy vsyscall data */
	vdata->clock.vclock_mode	= tk->clock->archdata.vclock_mode;
	vdata->clock.cycle_last		= tk->clock->cycle_last;
	vdata->clock.mask		= tk->clock->mask;
	vdata->clock.mult		= tk->mult;
	vdata->clock.shift		= tk->shift;

	vdata->wall_time_sec		= tk->xtime_sec;
	vdata->wall_time_snsec		= tk->xtime_nsec;

	vdata->monotonic_time_sec	= tk->xtime_sec
					+ tk->wall_to_monotonic.tv_sec;
	vdata->monotonic_time_snsec	= tk->xtime_nsec
					+ (tk->wall_to_monotonic.tv_nsec
						<< tk->shift);
	while (vdata->monotonic_time_snsec >=
					(((u64)NSEC_PER_SEC) << tk->shift)) {
		vdata->monotonic_time_snsec -=
					((u64)NSEC_PER_SEC) << tk->shift;
		vdata->monotonic_time_sec++;
	}

	vdata->wall_time_coarse.tv_sec	= tk->xtime_sec;
	vdata->wall_time_coarse.tv_nsec	= (long)(tk->xtime_nsec >> tk->shift);

	vdata->monotonic_time_coarse	= timespec_add(vdata->wall_time_coarse,
							tk->wall_to_monotonic);

	write_seqcount_end(&vdata->seq);
>>>>>>> refs/remotes/origin/master
}

static void warn_bad_vsyscall(const char *level, struct pt_regs *regs,
			      const char *message)
{
<<<<<<< HEAD
	static DEFINE_RATELIMIT_STATE(rs, DEFAULT_RATELIMIT_INTERVAL, DEFAULT_RATELIMIT_BURST);
	struct task_struct *tsk;

	if (!show_unhandled_signals || !__ratelimit(&rs))
		return;

	tsk = current;

	printk("%s%s[%d] %s ip:%lx cs:%lx sp:%lx ax:%lx si:%lx di:%lx\n",
	       level, tsk->comm, task_pid_nr(tsk),
	       message, regs->ip, regs->cs,
	       regs->sp, regs->ax, regs->si, regs->di);
=======
	if (!show_unhandled_signals)
		return;

	pr_notice_ratelimited("%s%s[%d] %s ip:%lx cs:%lx sp:%lx ax:%lx si:%lx di:%lx\n",
			      level, current->comm, task_pid_nr(current),
			      message, regs->ip, regs->cs,
			      regs->sp, regs->ax, regs->si, regs->di);
>>>>>>> refs/remotes/origin/master
}

static int addr_to_vsyscall_nr(unsigned long addr)
{
	int nr;

	if ((addr & ~0xC00UL) != VSYSCALL_START)
		return -EINVAL;

	nr = (addr & 0xC00UL) >> 10;
	if (nr >= 3)
		return -EINVAL;

	return nr;
}

static bool write_ok_or_segv(unsigned long ptr, size_t size)
{
	/*
	 * XXX: if access_ok, get_user, and put_user handled
	 * sig_on_uaccess_error, this could go away.
	 */

	if (!access_ok(VERIFY_WRITE, (void __user *)ptr, size)) {
		siginfo_t info;
		struct thread_struct *thread = &current->thread;

		thread->error_code	= 6;  /* user fault, no page, write */
		thread->cr2		= ptr;
		thread->trap_nr		= X86_TRAP_PF;

		memset(&info, 0, sizeof(info));
		info.si_signo		= SIGSEGV;
		info.si_errno		= 0;
		info.si_code		= SEGV_MAPERR;
		info.si_addr		= (void __user *)ptr;

		force_sig_info(SIGSEGV, &info, current);
		return false;
	} else {
		return true;
	}
}

bool emulate_vsyscall(struct pt_regs *regs, unsigned long address)
{
	struct task_struct *tsk;
	unsigned long caller;
<<<<<<< HEAD
	int vsyscall_nr;
=======
	int vsyscall_nr, syscall_nr, tmp;
>>>>>>> refs/remotes/origin/master
	int prev_sig_on_uaccess_error;
	long ret;

	/*
	 * No point in checking CS -- the only way to get here is a user mode
	 * trap to a high address, which means that we're in 64-bit user code.
	 */

	WARN_ON_ONCE(address != regs->ip);

	if (vsyscall_mode == NONE) {
		warn_bad_vsyscall(KERN_INFO, regs,
				  "vsyscall attempted with vsyscall=none");
		return false;
	}

	vsyscall_nr = addr_to_vsyscall_nr(address);

	trace_emulate_vsyscall(vsyscall_nr);

	if (vsyscall_nr < 0) {
		warn_bad_vsyscall(KERN_WARNING, regs,
				  "misaligned vsyscall (exploit attempt or buggy program) -- look up the vsyscall kernel parameter if you need a workaround");
		goto sigsegv;
	}

	if (get_user(caller, (unsigned long __user *)regs->sp) != 0) {
		warn_bad_vsyscall(KERN_WARNING, regs,
				  "vsyscall with bad stack (exploit attempt?)");
		goto sigsegv;
	}

	tsk = current;
<<<<<<< HEAD
	if (seccomp_mode(&tsk->seccomp))
		do_exit(SIGKILL);

	/*
	 * With a real vsyscall, page faults cause SIGSEGV.  We want to
	 * preserve that behavior to make writing exploits harder.
	 */
	prev_sig_on_uaccess_error = current_thread_info()->sig_on_uaccess_error;
	current_thread_info()->sig_on_uaccess_error = 1;

	/*
=======

	/*
	 * Check for access_ok violations and find the syscall nr.
	 *
>>>>>>> refs/remotes/origin/master
	 * NULL is a valid user pointer (in the access_ok sense) on 32-bit and
	 * 64-bit, so we don't need to special-case it here.  For all the
	 * vsyscalls, NULL means "don't write anything" not "write it at
	 * address 0".
	 */
<<<<<<< HEAD
	ret = -EFAULT;
	switch (vsyscall_nr) {
	case 0:
		if (!write_ok_or_segv(regs->di, sizeof(struct timeval)) ||
		    !write_ok_or_segv(regs->si, sizeof(struct timezone)))
			break;

=======
	switch (vsyscall_nr) {
	case 0:
		if (!write_ok_or_segv(regs->di, sizeof(struct timeval)) ||
		    !write_ok_or_segv(regs->si, sizeof(struct timezone))) {
			ret = -EFAULT;
			goto check_fault;
		}

		syscall_nr = __NR_gettimeofday;
		break;

	case 1:
		if (!write_ok_or_segv(regs->di, sizeof(time_t))) {
			ret = -EFAULT;
			goto check_fault;
		}

		syscall_nr = __NR_time;
		break;

	case 2:
		if (!write_ok_or_segv(regs->di, sizeof(unsigned)) ||
		    !write_ok_or_segv(regs->si, sizeof(unsigned))) {
			ret = -EFAULT;
			goto check_fault;
		}

		syscall_nr = __NR_getcpu;
		break;
	}

	/*
	 * Handle seccomp.  regs->ip must be the original value.
	 * See seccomp_send_sigsys and Documentation/prctl/seccomp_filter.txt.
	 *
	 * We could optimize the seccomp disabled case, but performance
	 * here doesn't matter.
	 */
	regs->orig_ax = syscall_nr;
	regs->ax = -ENOSYS;
	tmp = secure_computing(syscall_nr);
	if ((!tmp && regs->orig_ax != syscall_nr) || regs->ip != address) {
		warn_bad_vsyscall(KERN_DEBUG, regs,
				  "seccomp tried to change syscall nr or ip");
		do_exit(SIGSYS);
	}
	if (tmp)
		goto do_ret;  /* skip requested */

	/*
	 * With a real vsyscall, page faults cause SIGSEGV.  We want to
	 * preserve that behavior to make writing exploits harder.
	 */
	prev_sig_on_uaccess_error = current_thread_info()->sig_on_uaccess_error;
	current_thread_info()->sig_on_uaccess_error = 1;

	ret = -EFAULT;
	switch (vsyscall_nr) {
	case 0:
>>>>>>> refs/remotes/origin/master
		ret = sys_gettimeofday(
			(struct timeval __user *)regs->di,
			(struct timezone __user *)regs->si);
		break;

	case 1:
<<<<<<< HEAD
		if (!write_ok_or_segv(regs->di, sizeof(time_t)))
			break;

=======
>>>>>>> refs/remotes/origin/master
		ret = sys_time((time_t __user *)regs->di);
		break;

	case 2:
<<<<<<< HEAD
		if (!write_ok_or_segv(regs->di, sizeof(unsigned)) ||
		    !write_ok_or_segv(regs->si, sizeof(unsigned)))
			break;

=======
>>>>>>> refs/remotes/origin/master
		ret = sys_getcpu((unsigned __user *)regs->di,
				 (unsigned __user *)regs->si,
				 NULL);
		break;
	}

	current_thread_info()->sig_on_uaccess_error = prev_sig_on_uaccess_error;

<<<<<<< HEAD
=======
check_fault:
>>>>>>> refs/remotes/origin/master
	if (ret == -EFAULT) {
		/* Bad news -- userspace fed a bad pointer to a vsyscall. */
		warn_bad_vsyscall(KERN_INFO, regs,
				  "vsyscall fault (exploit attempt?)");

		/*
		 * If we failed to generate a signal for any reason,
		 * generate one here.  (This should be impossible.)
		 */
		if (WARN_ON_ONCE(!sigismember(&tsk->pending.signal, SIGBUS) &&
				 !sigismember(&tsk->pending.signal, SIGSEGV)))
			goto sigsegv;

		return true;  /* Don't emulate the ret. */
	}

	regs->ax = ret;

<<<<<<< HEAD
	/* Emulate a ret instruction. */
	regs->ip = caller;
	regs->sp += 8;

=======
do_ret:
	/* Emulate a ret instruction. */
	regs->ip = caller;
	regs->sp += 8;
>>>>>>> refs/remotes/origin/master
	return true;

sigsegv:
	force_sig(SIGSEGV, current);
	return true;
}

/*
 * Assume __initcall executes before all user space. Hopefully kmod
 * doesn't violate that. We'll find out if it does.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static void __cpuinit vsyscall_set_cpu(int cpu)
=======
static void vsyscall_set_cpu(int cpu)
>>>>>>> refs/remotes/origin/master
{
	unsigned long d;
	unsigned long node = 0;
#ifdef CONFIG_NUMA
	node = cpu_to_node(cpu);
#endif
	if (cpu_has(&cpu_data(cpu), X86_FEATURE_RDTSCP))
		write_rdtscp_aux((node << 12) | cpu);

<<<<<<< HEAD
<<<<<<< HEAD
	/* Store cpu number in limit so that it can be loaded quickly
	   in user space in vgetcpu.
	   12 bits for the CPU and 8 bits for the node. */
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Store cpu number in limit so that it can be loaded quickly
	 * in user space in vgetcpu. (12 bits for the CPU and 8 bits for the node)
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	d = 0x0f40000000000ULL;
	d |= cpu;
	d |= (node & 0xf) << 12;
	d |= (node >> 4) << 48;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/cm-10.0
	write_gdt_entry(get_cpu_gdt_table(cpu), GDT_ENTRY_PER_CPU, &d, DESCTYPE_S);
}

static void __cpuinit cpu_vsyscall_init(void *arg)
=======

	write_gdt_entry(get_cpu_gdt_table(cpu), GDT_ENTRY_PER_CPU, &d, DESCTYPE_S);
}

static void cpu_vsyscall_init(void *arg)
>>>>>>> refs/remotes/origin/master
{
	/* preemption should be already off */
	vsyscall_set_cpu(raw_smp_processor_id());
}

<<<<<<< HEAD
static int __cpuinit
cpu_vsyscall_notifier(struct notifier_block *n, unsigned long action, void *arg)
{
	long cpu = (long)arg;
<<<<<<< HEAD
	if (action == CPU_ONLINE || action == CPU_ONLINE_FROZEN)
		smp_call_function_single(cpu, cpu_vsyscall_init, NULL, 1);
=======
=======
static int
cpu_vsyscall_notifier(struct notifier_block *n, unsigned long action, void *arg)
{
	long cpu = (long)arg;
>>>>>>> refs/remotes/origin/master

	if (action == CPU_ONLINE || action == CPU_ONLINE_FROZEN)
		smp_call_function_single(cpu, cpu_vsyscall_init, NULL, 1);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return NOTIFY_DONE;
}

void __init map_vsyscall(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	extern char __vsyscall_0;
	unsigned long physaddr_page0 = __pa_symbol(&__vsyscall_0);

	/* Note that VSYSCALL_MAPPED_PAGES must agree with the code below. */
	__set_fixmap(VSYSCALL_FIRST_PAGE, physaddr_page0, PAGE_KERNEL_VSYSCALL);
=======
=======
>>>>>>> refs/remotes/origin/master
	extern char __vsyscall_page;
	unsigned long physaddr_vsyscall = __pa_symbol(&__vsyscall_page);
	extern char __vvar_page;
	unsigned long physaddr_vvar_page = __pa_symbol(&__vvar_page);

	__set_fixmap(VSYSCALL_FIRST_PAGE, physaddr_vsyscall,
		     vsyscall_mode == NATIVE
		     ? PAGE_KERNEL_VSYSCALL
		     : PAGE_KERNEL_VVAR);
	BUILD_BUG_ON((unsigned long)__fix_to_virt(VSYSCALL_FIRST_PAGE) !=
		     (unsigned long)VSYSCALL_START);

	__set_fixmap(VVAR_PAGE, physaddr_vvar_page, PAGE_KERNEL_VVAR);
	BUILD_BUG_ON((unsigned long)__fix_to_virt(VVAR_PAGE) !=
		     (unsigned long)VVAR_ADDRESS);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int __init vsyscall_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	BUG_ON(((unsigned long) &vgettimeofday !=
			VSYSCALL_ADDR(__NR_vgettimeofday)));
	BUG_ON((unsigned long) &vtime != VSYSCALL_ADDR(__NR_vtime));
	BUG_ON((VSYSCALL_ADDR(0) != __fix_to_virt(VSYSCALL_FIRST_PAGE)));
	BUG_ON((unsigned long) &vgetcpu != VSYSCALL_ADDR(__NR_vgetcpu));
#ifdef CONFIG_SYSCTL
	register_sysctl_table(kernel_root_table2);
#endif
	on_each_cpu(cpu_vsyscall_init, NULL, 1);
	/* notifier priority > KVM */
	hotcpu_notifier(cpu_vsyscall_notifier, 30);
	return 0;
}

=======
=======
>>>>>>> refs/remotes/origin/master
	BUG_ON(VSYSCALL_ADDR(0) != __fix_to_virt(VSYSCALL_FIRST_PAGE));

	on_each_cpu(cpu_vsyscall_init, NULL, 1);
	/* notifier priority > KVM */
	hotcpu_notifier(cpu_vsyscall_notifier, 30);

	return 0;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
__initcall(vsyscall_init);
