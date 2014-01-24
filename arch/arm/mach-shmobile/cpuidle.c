/*
 * CPUIdle support code for SH-Mobile ARM
 *
 *  Copyright (C) 2011 Magnus Damm
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/pm.h>
#include <linux/cpuidle.h>
#include <linux/suspend.h>
#include <linux/module.h>
#include <linux/err.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
=======
#include <asm/cpuidle.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/io.h>

static void shmobile_enter_wfi(void)
{
	cpu_do_idle();
}

void (*shmobile_cpuidle_modes[CPUIDLE_STATE_MAX])(void) = {
	shmobile_enter_wfi, /* regular sleep mode */
};

static int shmobile_cpuidle_enter(struct cpuidle_device *dev,
<<<<<<< HEAD
				  struct cpuidle_state *state)
{
	ktime_t before, after;
	int requested_state = state - &dev->states[0];

	dev->last_state = &dev->states[requested_state];
	before = ktime_get();

	local_irq_disable();
	local_fiq_disable();

	shmobile_cpuidle_modes[requested_state]();

	local_irq_enable();
	local_fiq_enable();

	after = ktime_get();
	return ktime_to_ns(ktime_sub(after, before)) >> 10;
=======
				  struct cpuidle_driver *drv,
				  int index)
{
	shmobile_cpuidle_modes[index]();

	return index;
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct cpuidle_device shmobile_cpuidle_dev;
static struct cpuidle_driver shmobile_cpuidle_driver = {
<<<<<<< HEAD
	.name =		"shmobile_cpuidle",
	.owner =	THIS_MODULE,
};

void (*shmobile_cpuidle_setup)(struct cpuidle_device *dev);
=======
	.name			= "shmobile_cpuidle",
	.owner			= THIS_MODULE,
	.en_core_tk_irqen	= 1,
=======
#include <asm/cpuidle.h>
#include <asm/io.h>

static struct cpuidle_driver shmobile_cpuidle_default_driver = {
	.name			= "shmobile_cpuidle",
	.owner			= THIS_MODULE,
>>>>>>> refs/remotes/origin/master
	.states[0]		= ARM_CPUIDLE_WFI_STATE,
	.safe_state_index	= 0, /* C1 */
	.state_count		= 1,
};

<<<<<<< HEAD
void (*shmobile_cpuidle_setup)(struct cpuidle_driver *drv);
>>>>>>> refs/remotes/origin/cm-10.0

static int shmobile_cpuidle_init(void)
{
	struct cpuidle_device *dev = &shmobile_cpuidle_dev;
<<<<<<< HEAD
	struct cpuidle_state *state;
	int i;

	cpuidle_register_driver(&shmobile_cpuidle_driver);

	for (i = 0; i < CPUIDLE_STATE_MAX; i++) {
		dev->states[i].name[0] = '\0';
		dev->states[i].desc[0] = '\0';
		dev->states[i].enter = shmobile_cpuidle_enter;
	}

	i = CPUIDLE_DRIVER_STATE_START;

	state = &dev->states[i++];
	snprintf(state->name, CPUIDLE_NAME_LEN, "C1");
	strncpy(state->desc, "WFI", CPUIDLE_DESC_LEN);
	state->exit_latency = 1;
	state->target_residency = 1 * 2;
	state->power_usage = 3;
	state->flags = 0;
	state->flags |= CPUIDLE_FLAG_TIME_VALID;

	dev->safe_state = state;
	dev->state_count = i;

	if (shmobile_cpuidle_setup)
		shmobile_cpuidle_setup(dev);

=======
	struct cpuidle_driver *drv = &shmobile_cpuidle_driver;
	int i;

	for (i = 0; i < CPUIDLE_STATE_MAX; i++)
		drv->states[i].enter = shmobile_cpuidle_enter;

	if (shmobile_cpuidle_setup)
		shmobile_cpuidle_setup(drv);

	cpuidle_register_driver(drv);

	dev->state_count = drv->state_count;
>>>>>>> refs/remotes/origin/cm-10.0
	cpuidle_register_device(dev);

	return 0;
}
late_initcall(shmobile_cpuidle_init);
=======
static struct cpuidle_driver *cpuidle_drv = &shmobile_cpuidle_default_driver;

void __init shmobile_cpuidle_set_driver(struct cpuidle_driver *drv)
{
	cpuidle_drv = drv;
}

int __init shmobile_cpuidle_init(void)
{
	return cpuidle_register(cpuidle_drv, NULL);
}
>>>>>>> refs/remotes/origin/master
