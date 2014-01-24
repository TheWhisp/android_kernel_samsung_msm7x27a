/*
 * linux/drivers/cpufreq/freq_table.c
 *
 * Copyright (C) 2002 - 2003 Dominik Brodowski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/cpufreq.h>
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master

/*********************************************************************
 *                     FREQUENCY TABLE HELPERS                       *
 *********************************************************************/

int cpufreq_frequency_table_cpuinfo(struct cpufreq_policy *policy,
				    struct cpufreq_frequency_table *table)
{
	unsigned int min_freq = ~0;
	unsigned int max_freq = 0;
	unsigned int i;

	for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
		unsigned int freq = table[i].frequency;
		if (freq == CPUFREQ_ENTRY_INVALID) {
			pr_debug("table entry %u is invalid, skipping\n", i);

			continue;
		}
<<<<<<< HEAD
		pr_debug("table entry %u: %u kHz, %u index\n",
					i, freq, table[i].index);
=======
		pr_debug("table entry %u: %u kHz, %u driver_data\n",
					i, freq, table[i].driver_data);
>>>>>>> refs/remotes/origin/master
		if (freq < min_freq)
			min_freq = freq;
		if (freq > max_freq)
			max_freq = freq;
	}

	policy->min = policy->cpuinfo.min_freq = min_freq;
	policy->max = policy->cpuinfo.max_freq = max_freq;

	if (policy->min == ~0)
		return -EINVAL;
	else
		return 0;
}
EXPORT_SYMBOL_GPL(cpufreq_frequency_table_cpuinfo);


int cpufreq_frequency_table_verify(struct cpufreq_policy *policy,
				   struct cpufreq_frequency_table *table)
{
<<<<<<< HEAD
	unsigned int next_larger = ~0;
	unsigned int i;
	unsigned int count = 0;
=======
	unsigned int next_larger = ~0, freq, i = 0;
	bool found = false;
>>>>>>> refs/remotes/origin/master

	pr_debug("request for verification of policy (%u - %u kHz) for cpu %u\n",
					policy->min, policy->max, policy->cpu);

<<<<<<< HEAD
	if (!cpu_online(policy->cpu))
		return -EINVAL;

	cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq,
				     policy->cpuinfo.max_freq);

	for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
		unsigned int freq = table[i].frequency;
		if (freq == CPUFREQ_ENTRY_INVALID)
			continue;
		if ((freq >= policy->min) && (freq <= policy->max))
			count++;
		else if ((next_larger > freq) && (freq > policy->max))
			next_larger = freq;
	}

	if (!count)
		policy->max = next_larger;

	cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq,
				     policy->cpuinfo.max_freq);
=======
	cpufreq_verify_within_cpu_limits(policy);

	for (; freq = table[i].frequency, freq != CPUFREQ_TABLE_END; i++) {
		if (freq == CPUFREQ_ENTRY_INVALID)
			continue;
		if ((freq >= policy->min) && (freq <= policy->max)) {
			found = true;
			break;
		}

		if ((next_larger > freq) && (freq > policy->max))
			next_larger = freq;
	}

	if (!found) {
		policy->max = next_larger;
		cpufreq_verify_within_cpu_limits(policy);
	}
>>>>>>> refs/remotes/origin/master

	pr_debug("verification lead to (%u - %u kHz) for cpu %u\n",
				policy->min, policy->max, policy->cpu);

	return 0;
}
EXPORT_SYMBOL_GPL(cpufreq_frequency_table_verify);

<<<<<<< HEAD
=======
/*
 * Generic routine to verify policy & frequency table, requires driver to call
 * cpufreq_frequency_table_get_attr() prior to it.
 */
int cpufreq_generic_frequency_table_verify(struct cpufreq_policy *policy)
{
	struct cpufreq_frequency_table *table =
		cpufreq_frequency_get_table(policy->cpu);
	if (!table)
		return -ENODEV;

	return cpufreq_frequency_table_verify(policy, table);
}
EXPORT_SYMBOL_GPL(cpufreq_generic_frequency_table_verify);
>>>>>>> refs/remotes/origin/master

int cpufreq_frequency_table_target(struct cpufreq_policy *policy,
				   struct cpufreq_frequency_table *table,
				   unsigned int target_freq,
				   unsigned int relation,
				   unsigned int *index)
{
	struct cpufreq_frequency_table optimal = {
<<<<<<< HEAD
		.index = ~0,
		.frequency = 0,
	};
	struct cpufreq_frequency_table suboptimal = {
		.index = ~0,
=======
		.driver_data = ~0,
		.frequency = 0,
	};
	struct cpufreq_frequency_table suboptimal = {
		.driver_data = ~0,
>>>>>>> refs/remotes/origin/master
		.frequency = 0,
	};
	unsigned int i;

	pr_debug("request for target %u kHz (relation: %u) for cpu %u\n",
					target_freq, relation, policy->cpu);

	switch (relation) {
	case CPUFREQ_RELATION_H:
		suboptimal.frequency = ~0;
		break;
	case CPUFREQ_RELATION_L:
		optimal.frequency = ~0;
		break;
	}

<<<<<<< HEAD
	if (!cpu_online(policy->cpu))
		return -EINVAL;

=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
		unsigned int freq = table[i].frequency;
		if (freq == CPUFREQ_ENTRY_INVALID)
			continue;
		if ((freq < policy->min) || (freq > policy->max))
			continue;
		switch (relation) {
		case CPUFREQ_RELATION_H:
			if (freq <= target_freq) {
				if (freq >= optimal.frequency) {
					optimal.frequency = freq;
<<<<<<< HEAD
					optimal.index = i;
=======
					optimal.driver_data = i;
>>>>>>> refs/remotes/origin/master
				}
			} else {
				if (freq <= suboptimal.frequency) {
					suboptimal.frequency = freq;
<<<<<<< HEAD
					suboptimal.index = i;
=======
					suboptimal.driver_data = i;
>>>>>>> refs/remotes/origin/master
				}
			}
			break;
		case CPUFREQ_RELATION_L:
			if (freq >= target_freq) {
				if (freq <= optimal.frequency) {
					optimal.frequency = freq;
<<<<<<< HEAD
					optimal.index = i;
=======
					optimal.driver_data = i;
>>>>>>> refs/remotes/origin/master
				}
			} else {
				if (freq >= suboptimal.frequency) {
					suboptimal.frequency = freq;
<<<<<<< HEAD
					suboptimal.index = i;
=======
					suboptimal.driver_data = i;
>>>>>>> refs/remotes/origin/master
				}
			}
			break;
		}
	}
<<<<<<< HEAD
	if (optimal.index > i) {
		if (suboptimal.index > i)
			return -EINVAL;
		*index = suboptimal.index;
	} else
		*index = optimal.index;

	pr_debug("target is %u (%u kHz, %u)\n", *index, table[*index].frequency,
		table[*index].index);
=======
	if (optimal.driver_data > i) {
		if (suboptimal.driver_data > i)
			return -EINVAL;
		*index = suboptimal.driver_data;
	} else
		*index = optimal.driver_data;

	pr_debug("target is %u (%u kHz, %u)\n", *index, table[*index].frequency,
		table[*index].driver_data);
>>>>>>> refs/remotes/origin/master

	return 0;
}
EXPORT_SYMBOL_GPL(cpufreq_frequency_table_target);

static DEFINE_PER_CPU(struct cpufreq_frequency_table *, cpufreq_show_table);
/**
 * show_available_freqs - show available frequencies for the specified CPU
 */
static ssize_t show_available_freqs(struct cpufreq_policy *policy, char *buf)
{
	unsigned int i = 0;
	unsigned int cpu = policy->cpu;
	ssize_t count = 0;
	struct cpufreq_frequency_table *table;

	if (!per_cpu(cpufreq_show_table, cpu))
		return -ENODEV;

	table = per_cpu(cpufreq_show_table, cpu);

	for (i = 0; (table[i].frequency != CPUFREQ_TABLE_END); i++) {
		if (table[i].frequency == CPUFREQ_ENTRY_INVALID)
			continue;
		count += sprintf(&buf[count], "%d ", table[i].frequency);
	}
	count += sprintf(&buf[count], "\n");

	return count;

}

struct freq_attr cpufreq_freq_attr_scaling_available_freqs = {
	.attr = { .name = "scaling_available_frequencies",
		  .mode = 0444,
		},
	.show = show_available_freqs,
};
EXPORT_SYMBOL_GPL(cpufreq_freq_attr_scaling_available_freqs);

<<<<<<< HEAD
=======
struct freq_attr *cpufreq_generic_attr[] = {
	&cpufreq_freq_attr_scaling_available_freqs,
	NULL,
};
EXPORT_SYMBOL_GPL(cpufreq_generic_attr);

>>>>>>> refs/remotes/origin/master
/*
 * if you use these, you must assure that the frequency table is valid
 * all the time between get_attr and put_attr!
 */
void cpufreq_frequency_table_get_attr(struct cpufreq_frequency_table *table,
				      unsigned int cpu)
{
	pr_debug("setting show_table for cpu %u to %p\n", cpu, table);
	per_cpu(cpufreq_show_table, cpu) = table;
}
EXPORT_SYMBOL_GPL(cpufreq_frequency_table_get_attr);

void cpufreq_frequency_table_put_attr(unsigned int cpu)
{
	pr_debug("clearing show_table for cpu %u\n", cpu);
	per_cpu(cpufreq_show_table, cpu) = NULL;
}
EXPORT_SYMBOL_GPL(cpufreq_frequency_table_put_attr);

<<<<<<< HEAD
=======
int cpufreq_table_validate_and_show(struct cpufreq_policy *policy,
				      struct cpufreq_frequency_table *table)
{
	int ret = cpufreq_frequency_table_cpuinfo(policy, table);

	if (!ret)
		cpufreq_frequency_table_get_attr(table, policy->cpu);

	return ret;
}
EXPORT_SYMBOL_GPL(cpufreq_table_validate_and_show);

void cpufreq_frequency_table_update_policy_cpu(struct cpufreq_policy *policy)
{
	pr_debug("Updating show_table for new_cpu %u from last_cpu %u\n",
			policy->cpu, policy->last_cpu);
	per_cpu(cpufreq_show_table, policy->cpu) = per_cpu(cpufreq_show_table,
			policy->last_cpu);
	per_cpu(cpufreq_show_table, policy->last_cpu) = NULL;
}

>>>>>>> refs/remotes/origin/master
struct cpufreq_frequency_table *cpufreq_frequency_get_table(unsigned int cpu)
{
	return per_cpu(cpufreq_show_table, cpu);
}
EXPORT_SYMBOL_GPL(cpufreq_frequency_get_table);

MODULE_AUTHOR("Dominik Brodowski <linux@brodo.de>");
MODULE_DESCRIPTION("CPUfreq frequency table helpers");
MODULE_LICENSE("GPL");
