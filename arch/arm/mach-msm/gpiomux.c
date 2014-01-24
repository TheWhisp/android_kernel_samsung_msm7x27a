<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2010, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
/* Copyright (c) 2010, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
<<<<<<< HEAD
<<<<<<< HEAD
 */
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <mach/gpiomux.h>

struct msm_gpiomux_rec {
	struct gpiomux_setting *sets[GPIOMUX_NSETTINGS];
	int ref;
};
static DEFINE_SPINLOCK(gpiomux_lock);
static struct msm_gpiomux_rec *msm_gpiomux_recs;
static struct gpiomux_setting *msm_gpiomux_sets;
static unsigned msm_gpiomux_ngpio;

int msm_gpiomux_write(unsigned gpio, enum msm_gpiomux_setting which,
	struct gpiomux_setting *setting, struct gpiomux_setting *old_setting)
{
	struct msm_gpiomux_rec *rec = msm_gpiomux_recs + gpio;
	unsigned set_slot = gpio * GPIOMUX_NSETTINGS + which;
	unsigned long irq_flags;
	struct gpiomux_setting *new_set;
	int status = 0;

	if (!msm_gpiomux_recs)
		return -EFAULT;

	if (gpio >= msm_gpiomux_ngpio)
=======
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
=======
>>>>>>> refs/remotes/origin/cm-11.0
 */
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
<<<<<<< HEAD
#include "gpiomux.h"
#include "proc_comm.h"
=======
#include <mach/gpiomux.h>
>>>>>>> refs/remotes/origin/cm-11.0

struct msm_gpiomux_rec {
	struct gpiomux_setting *sets[GPIOMUX_NSETTINGS];
	int ref;
};
static DEFINE_SPINLOCK(gpiomux_lock);
static struct msm_gpiomux_rec *msm_gpiomux_recs;
static struct gpiomux_setting *msm_gpiomux_sets;
static unsigned msm_gpiomux_ngpio;

<<<<<<< HEAD
static void __msm_gpiomux_write(unsigned gpio, gpiomux_config_t val)
{
	unsigned tlmm_config  = (val & ~GPIOMUX_CTL_MASK) |
				((gpio & 0x3ff) << 4);
	unsigned tlmm_disable = 0;
	int rc;

	rc = msm_proc_comm(PCOM_RPC_GPIO_TLMM_CONFIG_EX,
			   &tlmm_config, &tlmm_disable);
	if (rc)
		pr_err("%s: unexpected proc_comm failure %d: %08x %08x\n",
		       __func__, rc, tlmm_config, tlmm_disable);
}

int msm_gpiomux_write(unsigned gpio,
		      gpiomux_config_t active,
		      gpiomux_config_t suspended)
=======
int msm_gpiomux_write(unsigned gpio, enum msm_gpiomux_setting which,
	struct gpiomux_setting *setting, struct gpiomux_setting *old_setting)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct msm_gpiomux_rec *rec = msm_gpiomux_recs + gpio;
	unsigned set_slot = gpio * GPIOMUX_NSETTINGS + which;
	unsigned long irq_flags;
	struct gpiomux_setting *new_set;
	int status = 0;

	if (!msm_gpiomux_recs)
		return -EFAULT;

<<<<<<< HEAD
	if (gpio >= GPIOMUX_NGPIOS)
>>>>>>> refs/remotes/origin/master
=======
	if (gpio >= msm_gpiomux_ngpio)
>>>>>>> refs/remotes/origin/cm-11.0
		return -EINVAL;

	spin_lock_irqsave(&gpiomux_lock, irq_flags);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (old_setting) {
		if (rec->sets[which] == NULL)
			status = 1;
		else
			*old_setting =  *(rec->sets[which]);
	}
<<<<<<< HEAD

	if (setting) {
		msm_gpiomux_sets[set_slot] = *setting;
		rec->sets[which] = &msm_gpiomux_sets[set_slot];
	} else {
		rec->sets[which] = NULL;
	}

	new_set = rec->ref ? rec->sets[GPIOMUX_ACTIVE] :
		rec->sets[GPIOMUX_SUSPENDED];
	if (new_set)
		__msm_gpiomux_write(gpio, *new_set);

	spin_unlock_irqrestore(&gpiomux_lock, irq_flags);
	return status;
=======
	if (active & GPIOMUX_VALID)
		cfg->active = active;
=======
>>>>>>> refs/remotes/origin/cm-11.0

	if (setting) {
		msm_gpiomux_sets[set_slot] = *setting;
		rec->sets[which] = &msm_gpiomux_sets[set_slot];
	} else {
		rec->sets[which] = NULL;
	}

	new_set = rec->ref ? rec->sets[GPIOMUX_ACTIVE] :
		rec->sets[GPIOMUX_SUSPENDED];
	if (new_set)
		__msm_gpiomux_write(gpio, *new_set);

	spin_unlock_irqrestore(&gpiomux_lock, irq_flags);
<<<<<<< HEAD
	return 0;
>>>>>>> refs/remotes/origin/master
=======
	return status;
>>>>>>> refs/remotes/origin/cm-11.0
}
EXPORT_SYMBOL(msm_gpiomux_write);

int msm_gpiomux_get(unsigned gpio)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_gpiomux_rec *rec = msm_gpiomux_recs + gpio;
	unsigned long irq_flags;

	if (!msm_gpiomux_recs)
		return -EFAULT;

	if (gpio >= msm_gpiomux_ngpio)
		return -EINVAL;

	spin_lock_irqsave(&gpiomux_lock, irq_flags);
	if (rec->ref++ == 0 && rec->sets[GPIOMUX_ACTIVE])
		__msm_gpiomux_write(gpio, *rec->sets[GPIOMUX_ACTIVE]);
=======
	struct msm_gpiomux_config *cfg = msm_gpiomux_configs + gpio;
=======
	struct msm_gpiomux_rec *rec = msm_gpiomux_recs + gpio;
>>>>>>> refs/remotes/origin/cm-11.0
	unsigned long irq_flags;

	if (!msm_gpiomux_recs)
		return -EFAULT;

	if (gpio >= msm_gpiomux_ngpio)
		return -EINVAL;

	spin_lock_irqsave(&gpiomux_lock, irq_flags);
<<<<<<< HEAD
	if (cfg->ref++ == 0 && cfg->active & GPIOMUX_VALID)
		__msm_gpiomux_write(gpio, cfg->active);
>>>>>>> refs/remotes/origin/master
=======
	if (rec->ref++ == 0 && rec->sets[GPIOMUX_ACTIVE])
		__msm_gpiomux_write(gpio, *rec->sets[GPIOMUX_ACTIVE]);
>>>>>>> refs/remotes/origin/cm-11.0
	spin_unlock_irqrestore(&gpiomux_lock, irq_flags);
	return 0;
}
EXPORT_SYMBOL(msm_gpiomux_get);

int msm_gpiomux_put(unsigned gpio)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct msm_gpiomux_rec *rec = msm_gpiomux_recs + gpio;
	unsigned long irq_flags;

	if (!msm_gpiomux_recs)
		return -EFAULT;

	if (gpio >= msm_gpiomux_ngpio)
		return -EINVAL;

	spin_lock_irqsave(&gpiomux_lock, irq_flags);
	BUG_ON(rec->ref == 0);
	if (--rec->ref == 0 && rec->sets[GPIOMUX_SUSPENDED])
		__msm_gpiomux_write(gpio, *rec->sets[GPIOMUX_SUSPENDED]);
=======
	struct msm_gpiomux_config *cfg = msm_gpiomux_configs + gpio;
=======
	struct msm_gpiomux_rec *rec = msm_gpiomux_recs + gpio;
>>>>>>> refs/remotes/origin/cm-11.0
	unsigned long irq_flags;

	if (!msm_gpiomux_recs)
		return -EFAULT;

	if (gpio >= msm_gpiomux_ngpio)
		return -EINVAL;

	spin_lock_irqsave(&gpiomux_lock, irq_flags);
<<<<<<< HEAD
	BUG_ON(cfg->ref == 0);
	if (--cfg->ref == 0 && cfg->suspended & GPIOMUX_VALID)
		__msm_gpiomux_write(gpio, cfg->suspended);
>>>>>>> refs/remotes/origin/master
=======
	BUG_ON(rec->ref == 0);
	if (--rec->ref == 0 && rec->sets[GPIOMUX_SUSPENDED])
		__msm_gpiomux_write(gpio, *rec->sets[GPIOMUX_SUSPENDED]);
>>>>>>> refs/remotes/origin/cm-11.0
	spin_unlock_irqrestore(&gpiomux_lock, irq_flags);
	return 0;
}
EXPORT_SYMBOL(msm_gpiomux_put);

<<<<<<< HEAD
<<<<<<< HEAD
int msm_gpiomux_init(size_t ngpio)
{
	if (!ngpio)
		return -EINVAL;

	if (msm_gpiomux_recs)
		return -EPERM;

	msm_gpiomux_recs = kzalloc(sizeof(struct msm_gpiomux_rec) * ngpio,
				   GFP_KERNEL);
	if (!msm_gpiomux_recs)
		return -ENOMEM;

	/* There is no need to zero this memory, as clients will be blindly
	 * installing settings on top of it.
	 */
	msm_gpiomux_sets = kmalloc(sizeof(struct gpiomux_setting) * ngpio *
		GPIOMUX_NSETTINGS, GFP_KERNEL);
	if (!msm_gpiomux_sets) {
		kfree(msm_gpiomux_recs);
		msm_gpiomux_recs = NULL;
		return -ENOMEM;
	}

	msm_gpiomux_ngpio = ngpio;

	return 0;
}
EXPORT_SYMBOL(msm_gpiomux_init);

void msm_gpiomux_install(struct msm_gpiomux_config *configs, unsigned nconfigs)
{
	unsigned c, s;
	int rc;

	for (c = 0; c < nconfigs; ++c) {
		for (s = 0; s < GPIOMUX_NSETTINGS; ++s) {
			rc = msm_gpiomux_write(configs[c].gpio, s,
				configs[c].settings[s], NULL);
			if (rc)
				pr_err("%s: write failure: %d\n", __func__, rc);
		}
	}
}
EXPORT_SYMBOL(msm_gpiomux_install);
=======
static int __init gpiomux_init(void)
=======
int msm_gpiomux_init(size_t ngpio)
>>>>>>> refs/remotes/origin/cm-11.0
{
	if (!ngpio)
		return -EINVAL;

	if (msm_gpiomux_recs)
		return -EPERM;

	msm_gpiomux_recs = kzalloc(sizeof(struct msm_gpiomux_rec) * ngpio,
				   GFP_KERNEL);
	if (!msm_gpiomux_recs)
		return -ENOMEM;

	/* There is no need to zero this memory, as clients will be blindly
	 * installing settings on top of it.
	 */
	msm_gpiomux_sets = kmalloc(sizeof(struct gpiomux_setting) * ngpio *
		GPIOMUX_NSETTINGS, GFP_KERNEL);
	if (!msm_gpiomux_sets) {
		kfree(msm_gpiomux_recs);
		msm_gpiomux_recs = NULL;
		return -ENOMEM;
	}

	msm_gpiomux_ngpio = ngpio;

	return 0;
}
<<<<<<< HEAD
postcore_initcall(gpiomux_init);
>>>>>>> refs/remotes/origin/master
=======
EXPORT_SYMBOL(msm_gpiomux_init);

void msm_gpiomux_install(struct msm_gpiomux_config *configs, unsigned nconfigs)
{
	unsigned c, s;
	int rc;

	for (c = 0; c < nconfigs; ++c) {
		for (s = 0; s < GPIOMUX_NSETTINGS; ++s) {
			rc = msm_gpiomux_write(configs[c].gpio, s,
				configs[c].settings[s], NULL);
			if (rc)
				pr_err("%s: write failure: %d\n", __func__, rc);
		}
	}
}
EXPORT_SYMBOL(msm_gpiomux_install);
>>>>>>> refs/remotes/origin/cm-11.0
