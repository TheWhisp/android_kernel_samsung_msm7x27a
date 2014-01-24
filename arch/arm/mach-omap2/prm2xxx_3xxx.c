/*
 * OMAP2/3 PRM module functions
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 2010 Texas Instruments, Inc.
=======
 * Copyright (C) 2010-2011 Texas Instruments, Inc.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 2010-2011 Texas Instruments, Inc.
>>>>>>> refs/remotes/origin/master
 * Copyright (C) 2010 Nokia Corporation
 * Benoît Cousson
 * Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/io.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <plat/common.h>
#include <plat/cpu.h>
#include <plat/prcm.h>
=======
#include "common.h"
#include <plat/cpu.h>
#include <plat/prcm.h>
#include <plat/irqs.h>

#include "vp.h"
>>>>>>> refs/remotes/origin/cm-10.0

#include "prm2xxx_3xxx.h"
#include "cm2xxx_3xxx.h"
#include "prm-regbits-24xx.h"
#include "prm-regbits-34xx.h"

<<<<<<< HEAD
=======
static const struct omap_prcm_irq omap3_prcm_irqs[] = {
	OMAP_PRCM_IRQ("wkup",	0,	0),
	OMAP_PRCM_IRQ("io",	9,	1),
};

static struct omap_prcm_irq_setup omap3_prcm_irq_setup = {
	.ack			= OMAP3_PRM_IRQSTATUS_MPU_OFFSET,
	.mask			= OMAP3_PRM_IRQENABLE_MPU_OFFSET,
	.nr_regs		= 1,
	.irqs			= omap3_prcm_irqs,
	.nr_irqs		= ARRAY_SIZE(omap3_prcm_irqs),
	.irq			= INT_34XX_PRCM_MPU_IRQ,
	.read_pending_irqs	= &omap3xxx_prm_read_pending_irqs,
	.ocp_barrier		= &omap3xxx_prm_ocp_barrier,
	.save_and_clear_irqen	= &omap3xxx_prm_save_and_clear_irqen,
	.restore_irqen		= &omap3xxx_prm_restore_irqen,
};

>>>>>>> refs/remotes/origin/cm-10.0
u32 omap2_prm_read_mod_reg(s16 module, u16 idx)
{
	return __raw_readl(prm_base + module + idx);
}

void omap2_prm_write_mod_reg(u32 val, s16 module, u16 idx)
{
	__raw_writel(val, prm_base + module + idx);
}

/* Read-modify-write a register in a PRM module. Caller must lock */
u32 omap2_prm_rmw_mod_reg_bits(u32 mask, u32 bits, s16 module, s16 idx)
{
	u32 v;

	v = omap2_prm_read_mod_reg(module, idx);
	v &= ~mask;
	v |= bits;
	omap2_prm_write_mod_reg(v, module, idx);

	return v;
}

/* Read a PRM register, AND it, and shift the result down to bit 0 */
u32 omap2_prm_read_mod_bits_shift(s16 domain, s16 idx, u32 mask)
{
	u32 v;

	v = omap2_prm_read_mod_reg(domain, idx);
	v &= mask;
	v >>= __ffs(mask);

	return v;
}

u32 omap2_prm_set_mod_reg_bits(u32 bits, s16 module, s16 idx)
{
	return omap2_prm_rmw_mod_reg_bits(bits, bits, module, idx);
}

u32 omap2_prm_clear_mod_reg_bits(u32 bits, s16 module, s16 idx)
{
	return omap2_prm_rmw_mod_reg_bits(bits, 0x0, module, idx);
}

=======
#include "common.h"
#include "powerdomain.h"
#include "prm2xxx_3xxx.h"
#include "prm-regbits-24xx.h"
#include "clockdomain.h"
>>>>>>> refs/remotes/origin/master

/**
 * omap2_prm_is_hardreset_asserted - read the HW reset line state of
 * submodules contained in the hwmod module
 * @prm_mod: PRM submodule base (e.g. CORE_MOD)
 * @shift: register bit shift corresponding to the reset line to check
 *
 * Returns 1 if the (sub)module hardreset line is currently asserted,
 * 0 if the (sub)module hardreset line is not currently asserted, or
 * -EINVAL if called while running on a non-OMAP2/3 chip.
 */
int omap2_prm_is_hardreset_asserted(s16 prm_mod, u8 shift)
{
<<<<<<< HEAD
	if (!(cpu_is_omap24xx() || cpu_is_omap34xx()))
		return -EINVAL;

=======
>>>>>>> refs/remotes/origin/master
	return omap2_prm_read_mod_bits_shift(prm_mod, OMAP2_RM_RSTCTRL,
				       (1 << shift));
}

/**
 * omap2_prm_assert_hardreset - assert the HW reset line of a submodule
 * @prm_mod: PRM submodule base (e.g. CORE_MOD)
 * @shift: register bit shift corresponding to the reset line to assert
 *
 * Some IPs like dsp or iva contain processors that require an HW
 * reset line to be asserted / deasserted in order to fully enable the
 * IP.  These modules may have multiple hard-reset lines that reset
 * different 'submodules' inside the IP block.  This function will
 * place the submodule into reset.  Returns 0 upon success or -EINVAL
 * upon an argument error.
 */
int omap2_prm_assert_hardreset(s16 prm_mod, u8 shift)
{
	u32 mask;

<<<<<<< HEAD
	if (!(cpu_is_omap24xx() || cpu_is_omap34xx()))
		return -EINVAL;

=======
>>>>>>> refs/remotes/origin/master
	mask = 1 << shift;
	omap2_prm_rmw_mod_reg_bits(mask, mask, prm_mod, OMAP2_RM_RSTCTRL);

	return 0;
}

/**
 * omap2_prm_deassert_hardreset - deassert a submodule hardreset line and wait
 * @prm_mod: PRM submodule base (e.g. CORE_MOD)
 * @rst_shift: register bit shift corresponding to the reset line to deassert
 * @st_shift: register bit shift for the status of the deasserted submodule
 *
 * Some IPs like dsp or iva contain processors that require an HW
 * reset line to be asserted / deasserted in order to fully enable the
 * IP.  These modules may have multiple hard-reset lines that reset
 * different 'submodules' inside the IP block.  This function will
 * take the submodule out of reset and wait until the PRCM indicates
 * that the reset has completed before returning.  Returns 0 upon success or
 * -EINVAL upon an argument error, -EEXIST if the submodule was already out
 * of reset, or -EBUSY if the submodule did not exit reset promptly.
 */
int omap2_prm_deassert_hardreset(s16 prm_mod, u8 rst_shift, u8 st_shift)
{
	u32 rst, st;
	int c;

<<<<<<< HEAD
	if (!(cpu_is_omap24xx() || cpu_is_omap34xx()))
		return -EINVAL;

=======
>>>>>>> refs/remotes/origin/master
	rst = 1 << rst_shift;
	st = 1 << st_shift;

	/* Check the current status to avoid de-asserting the line twice */
	if (omap2_prm_read_mod_bits_shift(prm_mod, OMAP2_RM_RSTCTRL, rst) == 0)
		return -EEXIST;

	/* Clear the reset status by writing 1 to the status bit */
	omap2_prm_rmw_mod_reg_bits(0xffffffff, st, prm_mod, OMAP2_RM_RSTST);
	/* de-assert the reset control line */
	omap2_prm_rmw_mod_reg_bits(rst, 0, prm_mod, OMAP2_RM_RSTCTRL);
	/* wait the status to be set */
	omap_test_timeout(omap2_prm_read_mod_bits_shift(prm_mod, OMAP2_RM_RSTST,
						  st),
			  MAX_MODULE_HARDRESET_WAIT, c);

	return (c == MAX_MODULE_HARDRESET_WAIT) ? -EBUSY : 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======

/* PRM VP */

/*
 * struct omap3_vp - OMAP3 VP register access description.
 * @tranxdone_status: VP_TRANXDONE_ST bitmask in PRM_IRQSTATUS_MPU reg
 */
struct omap3_vp {
	u32 tranxdone_status;
};

static struct omap3_vp omap3_vp[] = {
	[OMAP3_VP_VDD_MPU_ID] = {
		.tranxdone_status = OMAP3430_VP1_TRANXDONE_ST_MASK,
	},
	[OMAP3_VP_VDD_CORE_ID] = {
		.tranxdone_status = OMAP3430_VP2_TRANXDONE_ST_MASK,
	},
};

#define MAX_VP_ID ARRAY_SIZE(omap3_vp);

u32 omap3_prm_vp_check_txdone(u8 vp_id)
{
	struct omap3_vp *vp = &omap3_vp[vp_id];
	u32 irqstatus;

	irqstatus = omap2_prm_read_mod_reg(OCP_MOD,
					   OMAP3_PRM_IRQSTATUS_MPU_OFFSET);
	return irqstatus & vp->tranxdone_status;
}

void omap3_prm_vp_clear_txdone(u8 vp_id)
{
	struct omap3_vp *vp = &omap3_vp[vp_id];

	omap2_prm_write_mod_reg(vp->tranxdone_status,
				OCP_MOD, OMAP3_PRM_IRQSTATUS_MPU_OFFSET);
}

u32 omap3_prm_vcvp_read(u8 offset)
{
	return omap2_prm_read_mod_reg(OMAP3430_GR_MOD, offset);
}

void omap3_prm_vcvp_write(u32 val, u8 offset)
{
	omap2_prm_write_mod_reg(val, OMAP3430_GR_MOD, offset);
}

u32 omap3_prm_vcvp_rmw(u32 mask, u32 bits, u8 offset)
{
	return omap2_prm_rmw_mod_reg_bits(mask, bits, OMAP3430_GR_MOD, offset);
}

/**
 * omap3xxx_prm_read_pending_irqs - read pending PRM MPU IRQs into @events
 * @events: ptr to a u32, preallocated by caller
 *
 * Read PRM_IRQSTATUS_MPU bits, AND'ed with the currently-enabled PRM
 * MPU IRQs, and store the result into the u32 pointed to by @events.
 * No return value.
 */
void omap3xxx_prm_read_pending_irqs(unsigned long *events)
{
	u32 mask, st;

	/* XXX Can the mask read be avoided (e.g., can it come from RAM?) */
	mask = omap2_prm_read_mod_reg(OCP_MOD, OMAP3_PRM_IRQENABLE_MPU_OFFSET);
	st = omap2_prm_read_mod_reg(OCP_MOD, OMAP3_PRM_IRQSTATUS_MPU_OFFSET);

	events[0] = mask & st;
}

/**
 * omap3xxx_prm_ocp_barrier - force buffered MPU writes to the PRM to complete
 *
 * Force any buffered writes to the PRM IP block to complete.  Needed
 * by the PRM IRQ handler, which reads and writes directly to the IP
 * block, to avoid race conditions after acknowledging or clearing IRQ
 * bits.  No return value.
 */
void omap3xxx_prm_ocp_barrier(void)
{
	omap2_prm_read_mod_reg(OCP_MOD, OMAP3_PRM_REVISION_OFFSET);
}

/**
 * omap3xxx_prm_save_and_clear_irqen - save/clear PRM_IRQENABLE_MPU reg
 * @saved_mask: ptr to a u32 array to save IRQENABLE bits
 *
 * Save the PRM_IRQENABLE_MPU register to @saved_mask.  @saved_mask
 * must be allocated by the caller.  Intended to be used in the PRM
 * interrupt handler suspend callback.  The OCP barrier is needed to
 * ensure the write to disable PRM interrupts reaches the PRM before
 * returning; otherwise, spurious interrupts might occur.  No return
 * value.
 */
void omap3xxx_prm_save_and_clear_irqen(u32 *saved_mask)
{
	saved_mask[0] = omap2_prm_read_mod_reg(OCP_MOD,
					       OMAP3_PRM_IRQENABLE_MPU_OFFSET);
	omap2_prm_write_mod_reg(0, OCP_MOD, OMAP3_PRM_IRQENABLE_MPU_OFFSET);

	/* OCP barrier */
	omap2_prm_read_mod_reg(OCP_MOD, OMAP3_PRM_REVISION_OFFSET);
}

/**
 * omap3xxx_prm_restore_irqen - set PRM_IRQENABLE_MPU register from args
 * @saved_mask: ptr to a u32 array of IRQENABLE bits saved previously
 *
 * Restore the PRM_IRQENABLE_MPU register from @saved_mask.  Intended
 * to be used in the PRM interrupt handler resume callback to restore
 * values saved by omap3xxx_prm_save_and_clear_irqen().  No OCP
 * barrier should be needed here; any pending PRM interrupts will fire
 * once the writes reach the PRM.  No return value.
 */
void omap3xxx_prm_restore_irqen(u32 *saved_mask)
{
	omap2_prm_write_mod_reg(saved_mask[0], OCP_MOD,
				OMAP3_PRM_IRQENABLE_MPU_OFFSET);
}

static int __init omap3xxx_prcm_init(void)
{
	if (cpu_is_omap34xx())
		return omap_prcm_register_chain_handler(&omap3_prcm_irq_setup);
	return 0;
}
subsys_initcall(omap3xxx_prcm_init);
>>>>>>> refs/remotes/origin/cm-10.0
=======


/* Powerdomain low-level functions */

/* Common functions across OMAP2 and OMAP3 */
int omap2_pwrdm_set_mem_onst(struct powerdomain *pwrdm, u8 bank,
								u8 pwrst)
{
	u32 m;

	m = omap2_pwrdm_get_mem_bank_onstate_mask(bank);

	omap2_prm_rmw_mod_reg_bits(m, (pwrst << __ffs(m)), pwrdm->prcm_offs,
				   OMAP2_PM_PWSTCTRL);

	return 0;
}

int omap2_pwrdm_set_mem_retst(struct powerdomain *pwrdm, u8 bank,
								u8 pwrst)
{
	u32 m;

	m = omap2_pwrdm_get_mem_bank_retst_mask(bank);

	omap2_prm_rmw_mod_reg_bits(m, (pwrst << __ffs(m)), pwrdm->prcm_offs,
				   OMAP2_PM_PWSTCTRL);

	return 0;
}

int omap2_pwrdm_read_mem_pwrst(struct powerdomain *pwrdm, u8 bank)
{
	u32 m;

	m = omap2_pwrdm_get_mem_bank_stst_mask(bank);

	return omap2_prm_read_mod_bits_shift(pwrdm->prcm_offs, OMAP2_PM_PWSTST,
					     m);
}

int omap2_pwrdm_read_mem_retst(struct powerdomain *pwrdm, u8 bank)
{
	u32 m;

	m = omap2_pwrdm_get_mem_bank_retst_mask(bank);

	return omap2_prm_read_mod_bits_shift(pwrdm->prcm_offs,
					     OMAP2_PM_PWSTCTRL, m);
}

int omap2_pwrdm_set_logic_retst(struct powerdomain *pwrdm, u8 pwrst)
{
	u32 v;

	v = pwrst << __ffs(OMAP_LOGICRETSTATE_MASK);
	omap2_prm_rmw_mod_reg_bits(OMAP_LOGICRETSTATE_MASK, v, pwrdm->prcm_offs,
				   OMAP2_PM_PWSTCTRL);

	return 0;
}

int omap2_pwrdm_wait_transition(struct powerdomain *pwrdm)
{
	u32 c = 0;

	/*
	 * REVISIT: pwrdm_wait_transition() may be better implemented
	 * via a callback and a periodic timer check -- how long do we expect
	 * powerdomain transitions to take?
	 */

	/* XXX Is this udelay() value meaningful? */
	while ((omap2_prm_read_mod_reg(pwrdm->prcm_offs, OMAP2_PM_PWSTST) &
		OMAP_INTRANSITION_MASK) &&
		(c++ < PWRDM_TRANSITION_BAILOUT))
			udelay(1);

	if (c > PWRDM_TRANSITION_BAILOUT) {
		pr_err("powerdomain: %s: waited too long to complete transition\n",
		       pwrdm->name);
		return -EAGAIN;
	}

	pr_debug("powerdomain: completed transition in %d loops\n", c);

	return 0;
}

int omap2_clkdm_add_wkdep(struct clockdomain *clkdm1,
			  struct clockdomain *clkdm2)
{
	omap2_prm_set_mod_reg_bits((1 << clkdm2->dep_bit),
				   clkdm1->pwrdm.ptr->prcm_offs, PM_WKDEP);
	return 0;
}

int omap2_clkdm_del_wkdep(struct clockdomain *clkdm1,
			  struct clockdomain *clkdm2)
{
	omap2_prm_clear_mod_reg_bits((1 << clkdm2->dep_bit),
				     clkdm1->pwrdm.ptr->prcm_offs, PM_WKDEP);
	return 0;
}

int omap2_clkdm_read_wkdep(struct clockdomain *clkdm1,
			   struct clockdomain *clkdm2)
{
	return omap2_prm_read_mod_bits_shift(clkdm1->pwrdm.ptr->prcm_offs,
					     PM_WKDEP, (1 << clkdm2->dep_bit));
}

/* XXX Caller must hold the clkdm's powerdomain lock */
int omap2_clkdm_clear_all_wkdeps(struct clockdomain *clkdm)
{
	struct clkdm_dep *cd;
	u32 mask = 0;

	for (cd = clkdm->wkdep_srcs; cd && cd->clkdm_name; cd++) {
		if (!cd->clkdm)
			continue; /* only happens if data is erroneous */

		/* PRM accesses are slow, so minimize them */
		mask |= 1 << cd->clkdm->dep_bit;
		cd->wkdep_usecount = 0;
	}

	omap2_prm_clear_mod_reg_bits(mask, clkdm->pwrdm.ptr->prcm_offs,
				     PM_WKDEP);
	return 0;
}

>>>>>>> refs/remotes/origin/master
