/*
 * gptimers.c - Blackfin General Purpose Timer core API
 *
 * Copyright (c) 2005-2008 Analog Devices Inc.
 * Copyright (C) 2005 John DeHority
 * Copyright (C) 2006 Hella Aglaia GmbH (awe@aglaia-gmbh.de)
 *
 * Licensed under the GPLv2.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>

#include <asm/blackfin.h>
#include <asm/gptimers.h>

#ifdef DEBUG
# define tassert(expr)
#else
# define tassert(expr) \
	if (!(expr)) \
		printk(KERN_DEBUG "%s:%s:%i: Assertion failed: " #expr "\n", __FILE__, __func__, __LINE__);
#endif

<<<<<<< HEAD
#define BFIN_TIMER_NUM_GROUP  (BFIN_TIMER_OCTET(MAX_BLACKFIN_GPTIMERS - 1) + 1)

<<<<<<< HEAD
typedef struct {
	uint16_t config;
	uint16_t __pad;
	uint32_t counter;
	uint32_t period;
	uint32_t width;
} GPTIMER_timer_regs;

typedef struct {
	uint16_t enable;
	uint16_t __pad0;
	uint16_t disable;
	uint16_t __pad1;
	uint32_t status;
} GPTIMER_group_regs;

static volatile GPTIMER_timer_regs *const timer_regs[MAX_BLACKFIN_GPTIMERS] =
{
	(GPTIMER_timer_regs *)TIMER0_CONFIG,
	(GPTIMER_timer_regs *)TIMER1_CONFIG,
	(GPTIMER_timer_regs *)TIMER2_CONFIG,
#if (MAX_BLACKFIN_GPTIMERS > 3)
	(GPTIMER_timer_regs *)TIMER3_CONFIG,
	(GPTIMER_timer_regs *)TIMER4_CONFIG,
	(GPTIMER_timer_regs *)TIMER5_CONFIG,
	(GPTIMER_timer_regs *)TIMER6_CONFIG,
	(GPTIMER_timer_regs *)TIMER7_CONFIG,
# if (MAX_BLACKFIN_GPTIMERS > 8)
	(GPTIMER_timer_regs *)TIMER8_CONFIG,
	(GPTIMER_timer_regs *)TIMER9_CONFIG,
	(GPTIMER_timer_regs *)TIMER10_CONFIG,
#  if (MAX_BLACKFIN_GPTIMERS > 11)
	(GPTIMER_timer_regs *)TIMER11_CONFIG,
=======
=======
#ifndef CONFIG_BF60x
# define BFIN_TIMER_NUM_GROUP  (BFIN_TIMER_OCTET(MAX_BLACKFIN_GPTIMERS - 1) + 1)
#else
# define BFIN_TIMER_NUM_GROUP  1
#endif

>>>>>>> refs/remotes/origin/master
static struct bfin_gptimer_regs * const timer_regs[MAX_BLACKFIN_GPTIMERS] =
{
	(void *)TIMER0_CONFIG,
	(void *)TIMER1_CONFIG,
	(void *)TIMER2_CONFIG,
#if (MAX_BLACKFIN_GPTIMERS > 3)
	(void *)TIMER3_CONFIG,
	(void *)TIMER4_CONFIG,
	(void *)TIMER5_CONFIG,
	(void *)TIMER6_CONFIG,
	(void *)TIMER7_CONFIG,
# if (MAX_BLACKFIN_GPTIMERS > 8)
	(void *)TIMER8_CONFIG,
	(void *)TIMER9_CONFIG,
	(void *)TIMER10_CONFIG,
#  if (MAX_BLACKFIN_GPTIMERS > 11)
	(void *)TIMER11_CONFIG,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#  endif
# endif
#endif
};

<<<<<<< HEAD
<<<<<<< HEAD
static volatile GPTIMER_group_regs *const group_regs[BFIN_TIMER_NUM_GROUP] =
{
	(GPTIMER_group_regs *)TIMER0_GROUP_REG,
#if (MAX_BLACKFIN_GPTIMERS > 8)
	(GPTIMER_group_regs *)TIMER8_GROUP_REG,
=======
=======
>>>>>>> refs/remotes/origin/master
static struct bfin_gptimer_group_regs * const group_regs[BFIN_TIMER_NUM_GROUP] =
{
	(void *)TIMER0_GROUP_REG,
#if (MAX_BLACKFIN_GPTIMERS > 8)
	(void *)TIMER8_GROUP_REG,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif
};

static uint32_t const trun_mask[MAX_BLACKFIN_GPTIMERS] =
{
	TIMER_STATUS_TRUN0,
	TIMER_STATUS_TRUN1,
	TIMER_STATUS_TRUN2,
#if (MAX_BLACKFIN_GPTIMERS > 3)
	TIMER_STATUS_TRUN3,
	TIMER_STATUS_TRUN4,
	TIMER_STATUS_TRUN5,
	TIMER_STATUS_TRUN6,
	TIMER_STATUS_TRUN7,
# if (MAX_BLACKFIN_GPTIMERS > 8)
	TIMER_STATUS_TRUN8,
	TIMER_STATUS_TRUN9,
	TIMER_STATUS_TRUN10,
#  if (MAX_BLACKFIN_GPTIMERS > 11)
	TIMER_STATUS_TRUN11,
#  endif
# endif
#endif
};

static uint32_t const tovf_mask[MAX_BLACKFIN_GPTIMERS] =
{
	TIMER_STATUS_TOVF0,
	TIMER_STATUS_TOVF1,
	TIMER_STATUS_TOVF2,
#if (MAX_BLACKFIN_GPTIMERS > 3)
	TIMER_STATUS_TOVF3,
	TIMER_STATUS_TOVF4,
	TIMER_STATUS_TOVF5,
	TIMER_STATUS_TOVF6,
	TIMER_STATUS_TOVF7,
# if (MAX_BLACKFIN_GPTIMERS > 8)
	TIMER_STATUS_TOVF8,
	TIMER_STATUS_TOVF9,
	TIMER_STATUS_TOVF10,
#  if (MAX_BLACKFIN_GPTIMERS > 11)
	TIMER_STATUS_TOVF11,
#  endif
# endif
#endif
};

static uint32_t const timil_mask[MAX_BLACKFIN_GPTIMERS] =
{
	TIMER_STATUS_TIMIL0,
	TIMER_STATUS_TIMIL1,
	TIMER_STATUS_TIMIL2,
#if (MAX_BLACKFIN_GPTIMERS > 3)
	TIMER_STATUS_TIMIL3,
	TIMER_STATUS_TIMIL4,
	TIMER_STATUS_TIMIL5,
	TIMER_STATUS_TIMIL6,
	TIMER_STATUS_TIMIL7,
# if (MAX_BLACKFIN_GPTIMERS > 8)
	TIMER_STATUS_TIMIL8,
	TIMER_STATUS_TIMIL9,
	TIMER_STATUS_TIMIL10,
#  if (MAX_BLACKFIN_GPTIMERS > 11)
	TIMER_STATUS_TIMIL11,
#  endif
# endif
#endif
};

void set_gptimer_pwidth(unsigned int timer_id, uint32_t value)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	timer_regs[timer_id]->width = value;
=======
	bfin_write(&timer_regs[timer_id]->width, value);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bfin_write(&timer_regs[timer_id]->width, value);
>>>>>>> refs/remotes/origin/master
	SSYNC();
}
EXPORT_SYMBOL(set_gptimer_pwidth);

uint32_t get_gptimer_pwidth(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	return timer_regs[timer_id]->width;
=======
	return bfin_read(&timer_regs[timer_id]->width);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return bfin_read(&timer_regs[timer_id]->width);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(get_gptimer_pwidth);

void set_gptimer_period(unsigned int timer_id, uint32_t period)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	timer_regs[timer_id]->period = period;
=======
	bfin_write(&timer_regs[timer_id]->period, period);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bfin_write(&timer_regs[timer_id]->period, period);
>>>>>>> refs/remotes/origin/master
	SSYNC();
}
EXPORT_SYMBOL(set_gptimer_period);

uint32_t get_gptimer_period(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	return timer_regs[timer_id]->period;
=======
	return bfin_read(&timer_regs[timer_id]->period);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return bfin_read(&timer_regs[timer_id]->period);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(get_gptimer_period);

uint32_t get_gptimer_count(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	return timer_regs[timer_id]->counter;
=======
	return bfin_read(&timer_regs[timer_id]->counter);
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(get_gptimer_count);

uint32_t get_gptimer_status(unsigned int group)
{
	tassert(group < BFIN_TIMER_NUM_GROUP);
<<<<<<< HEAD
	return group_regs[group]->status;
=======
	return bfin_read(&group_regs[group]->status);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return bfin_read(&timer_regs[timer_id]->counter);
}
EXPORT_SYMBOL(get_gptimer_count);

#ifdef CONFIG_BF60x
void set_gptimer_delay(unsigned int timer_id, uint32_t delay)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
	bfin_write(&timer_regs[timer_id]->delay, delay);
	SSYNC();
}
EXPORT_SYMBOL(set_gptimer_delay);

uint32_t get_gptimer_delay(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
	return bfin_read(&timer_regs[timer_id]->delay);
}
EXPORT_SYMBOL(get_gptimer_delay);
#endif

#ifdef CONFIG_BF60x
int get_gptimer_intr(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
	return !!(bfin_read(&group_regs[BFIN_TIMER_OCTET(timer_id)]->data_ilat) & timil_mask[timer_id]);
}
EXPORT_SYMBOL(get_gptimer_intr);

void clear_gptimer_intr(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
	bfin_write(&group_regs[BFIN_TIMER_OCTET(timer_id)]->data_ilat, timil_mask[timer_id]);
}
EXPORT_SYMBOL(clear_gptimer_intr);

int get_gptimer_over(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
	return !!(bfin_read(&group_regs[BFIN_TIMER_OCTET(timer_id)]->stat_ilat) & tovf_mask[timer_id]);
}
EXPORT_SYMBOL(get_gptimer_over);

void clear_gptimer_over(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
	bfin_write(&group_regs[BFIN_TIMER_OCTET(timer_id)]->stat_ilat, tovf_mask[timer_id]);
}
EXPORT_SYMBOL(clear_gptimer_over);

int get_gptimer_run(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
	return !!(bfin_read(&group_regs[BFIN_TIMER_OCTET(timer_id)]->run) & trun_mask[timer_id]);
}
EXPORT_SYMBOL(get_gptimer_run);

uint32_t get_gptimer_status(unsigned int group)
{
	tassert(group < BFIN_TIMER_NUM_GROUP);
	return bfin_read(&group_regs[group]->data_ilat);
}
EXPORT_SYMBOL(get_gptimer_status);

void set_gptimer_status(unsigned int group, uint32_t value)
{
	tassert(group < BFIN_TIMER_NUM_GROUP);
	bfin_write(&group_regs[group]->data_ilat, value);
	SSYNC();
}
EXPORT_SYMBOL(set_gptimer_status);
#else
uint32_t get_gptimer_status(unsigned int group)
{
	tassert(group < BFIN_TIMER_NUM_GROUP);
	return bfin_read(&group_regs[group]->status);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(get_gptimer_status);

void set_gptimer_status(unsigned int group, uint32_t value)
{
	tassert(group < BFIN_TIMER_NUM_GROUP);
<<<<<<< HEAD
<<<<<<< HEAD
	group_regs[group]->status = value;
=======
	bfin_write(&group_regs[group]->status, value);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bfin_write(&group_regs[group]->status, value);
>>>>>>> refs/remotes/origin/master
	SSYNC();
}
EXPORT_SYMBOL(set_gptimer_status);

<<<<<<< HEAD
<<<<<<< HEAD
int get_gptimer_intr(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
	return !!(group_regs[BFIN_TIMER_OCTET(timer_id)]->status & timil_mask[timer_id]);
=======
=======
>>>>>>> refs/remotes/origin/master
static uint32_t read_gptimer_status(unsigned int timer_id)
{
	return bfin_read(&group_regs[BFIN_TIMER_OCTET(timer_id)]->status);
}

int get_gptimer_intr(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
	return !!(read_gptimer_status(timer_id) & timil_mask[timer_id]);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(get_gptimer_intr);

void clear_gptimer_intr(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	group_regs[BFIN_TIMER_OCTET(timer_id)]->status = timil_mask[timer_id];
=======
	bfin_write(&group_regs[BFIN_TIMER_OCTET(timer_id)]->status, timil_mask[timer_id]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bfin_write(&group_regs[BFIN_TIMER_OCTET(timer_id)]->status, timil_mask[timer_id]);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(clear_gptimer_intr);

int get_gptimer_over(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	return !!(group_regs[BFIN_TIMER_OCTET(timer_id)]->status & tovf_mask[timer_id]);
=======
	return !!(read_gptimer_status(timer_id) & tovf_mask[timer_id]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return !!(read_gptimer_status(timer_id) & tovf_mask[timer_id]);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(get_gptimer_over);

void clear_gptimer_over(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	group_regs[BFIN_TIMER_OCTET(timer_id)]->status = tovf_mask[timer_id];
=======
	bfin_write(&group_regs[BFIN_TIMER_OCTET(timer_id)]->status, tovf_mask[timer_id]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bfin_write(&group_regs[BFIN_TIMER_OCTET(timer_id)]->status, tovf_mask[timer_id]);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(clear_gptimer_over);

int get_gptimer_run(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	return !!(group_regs[BFIN_TIMER_OCTET(timer_id)]->status & trun_mask[timer_id]);
=======
	return !!(read_gptimer_status(timer_id) & trun_mask[timer_id]);
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(get_gptimer_run);
=======
	return !!(read_gptimer_status(timer_id) & trun_mask[timer_id]);
}
EXPORT_SYMBOL(get_gptimer_run);
#endif
>>>>>>> refs/remotes/origin/master

void set_gptimer_config(unsigned int timer_id, uint16_t config)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	timer_regs[timer_id]->config = config;
=======
	bfin_write(&timer_regs[timer_id]->config, config);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bfin_write(&timer_regs[timer_id]->config, config);
>>>>>>> refs/remotes/origin/master
	SSYNC();
}
EXPORT_SYMBOL(set_gptimer_config);

uint16_t get_gptimer_config(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	return timer_regs[timer_id]->config;
=======
	return bfin_read(&timer_regs[timer_id]->config);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return bfin_read(&timer_regs[timer_id]->config);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(get_gptimer_config);

void enable_gptimers(uint16_t mask)
{
	int i;
<<<<<<< HEAD
	tassert((mask & ~BLACKFIN_GPTIMER_IDMASK) == 0);
	for (i = 0; i < BFIN_TIMER_NUM_GROUP; ++i) {
<<<<<<< HEAD
		group_regs[i]->enable = mask & 0xFF;
=======
		bfin_write(&group_regs[i]->enable, mask & 0xFF);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_BF60x
	uint16_t imask;
	imask = bfin_read16(TIMER_DATA_IMSK);
	imask &= ~mask;
	bfin_write16(TIMER_DATA_IMSK, imask);
#endif
	tassert((mask & ~BLACKFIN_GPTIMER_IDMASK) == 0);
	for (i = 0; i < BFIN_TIMER_NUM_GROUP; ++i) {
		bfin_write(&group_regs[i]->enable, mask & 0xFF);
>>>>>>> refs/remotes/origin/master
		mask >>= 8;
	}
	SSYNC();
}
EXPORT_SYMBOL(enable_gptimers);

static void _disable_gptimers(uint16_t mask)
{
	int i;
	uint16_t m = mask;
	tassert((mask & ~BLACKFIN_GPTIMER_IDMASK) == 0);
	for (i = 0; i < BFIN_TIMER_NUM_GROUP; ++i) {
<<<<<<< HEAD
<<<<<<< HEAD
		group_regs[i]->disable = m & 0xFF;
=======
		bfin_write(&group_regs[i]->disable, m & 0xFF);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		bfin_write(&group_regs[i]->disable, m & 0xFF);
>>>>>>> refs/remotes/origin/master
		m >>= 8;
	}
}

void disable_gptimers(uint16_t mask)
{
<<<<<<< HEAD
=======
#ifndef CONFIG_BF60x
>>>>>>> refs/remotes/origin/master
	int i;
	_disable_gptimers(mask);
	for (i = 0; i < MAX_BLACKFIN_GPTIMERS; ++i)
		if (mask & (1 << i))
<<<<<<< HEAD
<<<<<<< HEAD
			group_regs[BFIN_TIMER_OCTET(i)]->status = trun_mask[i];
=======
			bfin_write(&group_regs[BFIN_TIMER_OCTET(i)]->status, trun_mask[i]);
>>>>>>> refs/remotes/origin/cm-10.0
	SSYNC();
=======
			bfin_write(&group_regs[BFIN_TIMER_OCTET(i)]->status, trun_mask[i]);
	SSYNC();
#else
	_disable_gptimers(mask);
#endif
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(disable_gptimers);

void disable_gptimers_sync(uint16_t mask)
{
	_disable_gptimers(mask);
	SSYNC();
}
EXPORT_SYMBOL(disable_gptimers_sync);

void set_gptimer_pulse_hi(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	timer_regs[timer_id]->config |= TIMER_PULSE_HI;
=======
	bfin_write_or(&timer_regs[timer_id]->config, TIMER_PULSE_HI);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bfin_write_or(&timer_regs[timer_id]->config, TIMER_PULSE_HI);
>>>>>>> refs/remotes/origin/master
	SSYNC();
}
EXPORT_SYMBOL(set_gptimer_pulse_hi);

void clear_gptimer_pulse_hi(unsigned int timer_id)
{
	tassert(timer_id < MAX_BLACKFIN_GPTIMERS);
<<<<<<< HEAD
<<<<<<< HEAD
	timer_regs[timer_id]->config &= ~TIMER_PULSE_HI;
=======
	bfin_write_and(&timer_regs[timer_id]->config, ~TIMER_PULSE_HI);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bfin_write_and(&timer_regs[timer_id]->config, ~TIMER_PULSE_HI);
>>>>>>> refs/remotes/origin/master
	SSYNC();
}
EXPORT_SYMBOL(clear_gptimer_pulse_hi);

uint16_t get_enabled_gptimers(void)
{
	int i;
	uint16_t result = 0;
	for (i = 0; i < BFIN_TIMER_NUM_GROUP; ++i)
<<<<<<< HEAD
<<<<<<< HEAD
		result |= (group_regs[i]->enable << (i << 3));
=======
		result |= (bfin_read(&group_regs[i]->enable) << (i << 3));
>>>>>>> refs/remotes/origin/cm-10.0
=======
		result |= (bfin_read(&group_regs[i]->enable) << (i << 3));
>>>>>>> refs/remotes/origin/master
	return result;
}
EXPORT_SYMBOL(get_enabled_gptimers);

MODULE_AUTHOR("Axel Weiss (awe@aglaia-gmbh.de)");
MODULE_DESCRIPTION("Blackfin General Purpose Timers API");
MODULE_LICENSE("GPL");
