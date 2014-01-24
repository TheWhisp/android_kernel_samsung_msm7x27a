/*
 * SMP support for R-Mobile / SH-Mobile - sh73a0 portion
 *
 * Copyright (C) 2010  Magnus Damm
 * Copyright (C) 2010  Takashi Yoshii
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/smp.h>
<<<<<<< HEAD
#include <linux/spinlock.h>
#include <linux/io.h>
#include <mach/common.h>
<<<<<<< HEAD
=======
#include <asm/smp_plat.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/smp_scu.h>
#include <asm/smp_twd.h>
#include <asm/hardware/gic.h>

<<<<<<< HEAD
#define WUPCR		0xe6151010
#define SRESCR		0xe6151018
#define PSTR		0xe6151040
#define SBAR            0xe6180020
#define APARMBAREA      0xe6f10020
=======
=======
#include <linux/io.h>
#include <linux/delay.h>
#include <mach/common.h>
#include <mach/sh73a0.h>
#include <asm/smp_plat.h>
#include <asm/smp_twd.h>

>>>>>>> refs/remotes/origin/master
#define WUPCR		IOMEM(0xe6151010)
#define SRESCR		IOMEM(0xe6151018)
#define PSTR		IOMEM(0xe6151040)
#define SBAR		IOMEM(0xe6180020)
#define APARMBAREA	IOMEM(0xe6f10020)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

static void __iomem *scu_base_addr(void)
{
	return (void __iomem *)0xf0000000;
}

static DEFINE_SPINLOCK(scu_lock);
static unsigned long tmp;

<<<<<<< HEAD
=======
#ifdef CONFIG_HAVE_ARM_TWD
static DEFINE_TWD_LOCAL_TIMER(twd_local_timer, 0xf0000600, 29);
=======

#define SH73A0_SCU_BASE 0xf0000000

#ifdef CONFIG_HAVE_ARM_TWD
static DEFINE_TWD_LOCAL_TIMER(twd_local_timer, SH73A0_SCU_BASE + 0x600, 29);
>>>>>>> refs/remotes/origin/master
void __init sh73a0_register_twd(void)
{
	twd_local_timer_register(&twd_local_timer);
}
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static void modify_scu_cpu_psr(unsigned long set, unsigned long clr)
{
	void __iomem *scu_base = scu_base_addr();

	spin_lock(&scu_lock);
	tmp = __raw_readl(scu_base + 8);
	tmp &= ~clr;
	tmp |= set;
	spin_unlock(&scu_lock);

	/* disable cache coherency after releasing the lock */
	__raw_writel(tmp, scu_base + 8);
}

unsigned int __init sh73a0_get_core_count(void)
{
	void __iomem *scu_base = scu_base_addr();

<<<<<<< HEAD
#ifdef CONFIG_HAVE_ARM_TWD
	/* twd_base needs to be initialized before percpu_timer_setup() */
	twd_base = (void __iomem *)0xf0000600;
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
	return scu_get_core_count(scu_base);
}

void __cpuinit sh73a0_secondary_init(unsigned int cpu)
{
	gic_secondary_init(0);
}

int __cpuinit sh73a0_boot_secondary(unsigned int cpu)
{
<<<<<<< HEAD
	/* enable cache coherency */
	modify_scu_cpu_psr(0, 3 << (cpu * 8));

	if (((__raw_readw(__io(PSTR)) >> (4 * cpu)) & 3) == 3)
		__raw_writel(1 << cpu, __io(WUPCR));	/* wake up */
	else
		__raw_writel(1 << cpu, __io(SRESCR));	/* reset */
=======
	cpu = cpu_logical_map(cpu);

	/* enable cache coherency */
	modify_scu_cpu_psr(0, 3 << (cpu * 8));

	if (((__raw_readl(PSTR) >> (4 * cpu)) & 3) == 3)
		__raw_writel(1 << cpu, WUPCR);	/* wake up */
	else
		__raw_writel(1 << cpu, SRESCR);	/* reset */
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int sh73a0_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned int lcpu = cpu_logical_map(cpu);

	if (((__raw_readl(PSTR) >> (4 * lcpu)) & 3) == 3)
		__raw_writel(1 << lcpu, WUPCR);	/* wake up */
	else
		__raw_writel(1 << lcpu, SRESCR);	/* reset */
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
void __init sh73a0_smp_prepare_cpus(void)
{
<<<<<<< HEAD
	scu_enable(scu_base_addr());

	/* Map the reset vector (in headsmp.S) */
	__raw_writel(0, __io(APARMBAREA));      /* 4k */
	__raw_writel(__pa(shmobile_secondary_vector), __io(SBAR));

	/* enable cache coherency on CPU0 */
	modify_scu_cpu_psr(0, 3 << (0 * 8));
=======
	int cpu = cpu_logical_map(0);

	scu_enable(scu_base_addr());

	/* Map the reset vector (in headsmp.S) */
	__raw_writel(0, APARMBAREA);      /* 4k */
	__raw_writel(__pa(shmobile_secondary_vector), SBAR);

	/* enable cache coherency on CPU0 */
	modify_scu_cpu_psr(0, 3 << (cpu * 8));
>>>>>>> refs/remotes/origin/cm-10.0
}
=======
static void __init sh73a0_smp_prepare_cpus(unsigned int max_cpus)
{
	/* Map the reset vector (in headsmp.S) */
	__raw_writel(0, APARMBAREA);      /* 4k */
	__raw_writel(__pa(shmobile_boot_vector), SBAR);

	/* setup sh73a0 specific SCU bits */
	shmobile_scu_base = IOMEM(SH73A0_SCU_BASE);
	shmobile_smp_scu_prepare_cpus(max_cpus);
}

struct smp_operations sh73a0_smp_ops __initdata = {
	.smp_prepare_cpus	= sh73a0_smp_prepare_cpus,
	.smp_boot_secondary	= sh73a0_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_disable		= shmobile_smp_cpu_disable,
	.cpu_die		= shmobile_smp_scu_cpu_die,
	.cpu_kill		= shmobile_smp_scu_cpu_kill,
#endif
};
>>>>>>> refs/remotes/origin/master
