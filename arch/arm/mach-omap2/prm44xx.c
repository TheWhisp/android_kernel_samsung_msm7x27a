/*
 * OMAP4 PRM module functions
 *
<<<<<<< HEAD
 * Copyright (C) 2010 Texas Instruments, Inc.
=======
 * Copyright (C) 2011 Texas Instruments, Inc.
>>>>>>> refs/remotes/origin/cm-10.0
 * Copyright (C) 2010 Nokia Corporation
 * Benoît Cousson
 * Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/io.h>

<<<<<<< HEAD
#include <plat/common.h>
#include <plat/cpu.h>
#include <plat/prcm.h>

#include "prm44xx.h"
#include "prm-regbits-44xx.h"

/*
 * Address offset (in bytes) between the reset control and the reset
 * status registers: 4 bytes on OMAP4
 */
#define OMAP4_RST_CTRL_ST_OFFSET		4
=======
#include <plat/cpu.h>
#include <plat/irqs.h>
#include <plat/prcm.h>

#include "iomap.h"
#include "common.h"
#include "vp.h"
#include "prm44xx.h"
#include "prm-regbits-44xx.h"
#include "prcm44xx.h"
#include "prminst44xx.h"

static const struct omap_prcm_irq omap4_prcm_irqs[] = {
	OMAP_PRCM_IRQ("wkup",   0,      0),
	OMAP_PRCM_IRQ("io",     9,      1),
};

static struct omap_prcm_irq_setup omap4_prcm_irq_setup = {
	.ack			= OMAP4_PRM_IRQSTATUS_MPU_OFFSET,
	.mask			= OMAP4_PRM_IRQENABLE_MPU_OFFSET,
	.nr_regs		= 2,
	.irqs			= omap4_prcm_irqs,
	.nr_irqs		= ARRAY_SIZE(omap4_prcm_irqs),
	.irq			= OMAP44XX_IRQ_PRCM,
	.read_pending_irqs	= &omap44xx_prm_read_pending_irqs,
	.ocp_barrier		= &omap44xx_prm_ocp_barrier,
	.save_and_clear_irqen	= &omap44xx_prm_save_and_clear_irqen,
	.restore_irqen		= &omap44xx_prm_restore_irqen,
};
>>>>>>> refs/remotes/origin/cm-10.0

/* PRM low-level functions */

/* Read a register in a CM/PRM instance in the PRM module */
u32 omap4_prm_read_inst_reg(s16 inst, u16 reg)
{
	return __raw_readl(OMAP44XX_PRM_REGADDR(inst, reg));
}

/* Write into a register in a CM/PRM instance in the PRM module */
void omap4_prm_write_inst_reg(u32 val, s16 inst, u16 reg)
{
	__raw_writel(val, OMAP44XX_PRM_REGADDR(inst, reg));
}

/* Read-modify-write a register in a PRM module. Caller must lock */
u32 omap4_prm_rmw_inst_reg_bits(u32 mask, u32 bits, s16 inst, s16 reg)
{
	u32 v;

	v = omap4_prm_read_inst_reg(inst, reg);
	v &= ~mask;
	v |= bits;
	omap4_prm_write_inst_reg(v, inst, reg);

	return v;
}

<<<<<<< HEAD
/* Read a PRM register, AND it, and shift the result down to bit 0 */
/* XXX deprecated */
u32 omap4_prm_read_bits_shift(void __iomem *reg, u32 mask)
{
	u32 v;

	v = __raw_readl(reg);
	v &= mask;
	v >>= __ffs(mask);

	return v;
}

/* Read-modify-write a register in a PRM module. Caller must lock */
/* XXX deprecated */
u32 omap4_prm_rmw_reg_bits(u32 mask, u32 bits, void __iomem *reg)
{
	u32 v;

	v = __raw_readl(reg);
	v &= ~mask;
	v |= bits;
	__raw_writel(v, reg);

	return v;
}

u32 omap4_prm_set_inst_reg_bits(u32 bits, s16 inst, s16 reg)
{
	return omap4_prm_rmw_inst_reg_bits(bits, bits, inst, reg);
}

u32 omap4_prm_clear_inst_reg_bits(u32 bits, s16 inst, s16 reg)
{
	return omap4_prm_rmw_inst_reg_bits(bits, 0x0, inst, reg);
}

/**
 * omap4_prm_is_hardreset_asserted - read the HW reset line state of
 * submodules contained in the hwmod module
 * @rstctrl_reg: RM_RSTCTRL register address for this module
 * @shift: register bit shift corresponding to the reset line to check
 *
 * Returns 1 if the (sub)module hardreset line is currently asserted,
 * 0 if the (sub)module hardreset line is not currently asserted, or
 * -EINVAL upon parameter error.
 */
int omap4_prm_is_hardreset_asserted(void __iomem *rstctrl_reg, u8 shift)
{
	if (!cpu_is_omap44xx() || !rstctrl_reg)
		return -EINVAL;

	return omap4_prm_read_bits_shift(rstctrl_reg, (1 << shift));
}

/**
 * omap4_prm_assert_hardreset - assert the HW reset line of a submodule
 * @rstctrl_reg: RM_RSTCTRL register address for this module
 * @shift: register bit shift corresponding to the reset line to assert
 *
 * Some IPs like dsp, ipu or iva contain processors that require an HW
 * reset line to be asserted / deasserted in order to fully enable the
 * IP.  These modules may have multiple hard-reset lines that reset
 * different 'submodules' inside the IP block.  This function will
 * place the submodule into reset.  Returns 0 upon success or -EINVAL
 * upon an argument error.
 */
int omap4_prm_assert_hardreset(void __iomem *rstctrl_reg, u8 shift)
{
	u32 mask;

	if (!cpu_is_omap44xx() || !rstctrl_reg)
		return -EINVAL;

	mask = 1 << shift;
	omap4_prm_rmw_reg_bits(mask, mask, rstctrl_reg);

	return 0;
}

/**
 * omap4_prm_deassert_hardreset - deassert a submodule hardreset line and wait
 * @rstctrl_reg: RM_RSTCTRL register address for this module
 * @shift: register bit shift corresponding to the reset line to deassert
 *
 * Some IPs like dsp, ipu or iva contain processors that require an HW
 * reset line to be asserted / deasserted in order to fully enable the
 * IP.  These modules may have multiple hard-reset lines that reset
 * different 'submodules' inside the IP block.  This function will
 * take the submodule out of reset and wait until the PRCM indicates
 * that the reset has completed before returning.  Returns 0 upon success or
 * -EINVAL upon an argument error, -EEXIST if the submodule was already out
 * of reset, or -EBUSY if the submodule did not exit reset promptly.
 */
int omap4_prm_deassert_hardreset(void __iomem *rstctrl_reg, u8 shift)
{
	u32 mask;
	void __iomem *rstst_reg;
	int c;

	if (!cpu_is_omap44xx() || !rstctrl_reg)
		return -EINVAL;

	rstst_reg = rstctrl_reg + OMAP4_RST_CTRL_ST_OFFSET;

	mask = 1 << shift;

	/* Check the current status to avoid de-asserting the line twice */
	if (omap4_prm_read_bits_shift(rstctrl_reg, mask) == 0)
		return -EEXIST;

	/* Clear the reset status by writing 1 to the status bit */
	omap4_prm_rmw_reg_bits(0xffffffff, mask, rstst_reg);
	/* de-assert the reset control line */
	omap4_prm_rmw_reg_bits(mask, 0, rstctrl_reg);
	/* wait the status to be set */
	omap_test_timeout(omap4_prm_read_bits_shift(rstst_reg, mask),
			  MAX_MODULE_HARDRESET_WAIT, c);

	return (c == MAX_MODULE_HARDRESET_WAIT) ? -EBUSY : 0;
}

void omap4_prm_global_warm_sw_reset(void)
{
	u32 v;

	v = omap4_prm_read_inst_reg(OMAP4430_PRM_DEVICE_INST,
				    OMAP4_RM_RSTCTRL);
	v |= OMAP4430_RST_GLOBAL_WARM_SW_MASK;
	omap4_prm_write_inst_reg(v, OMAP4430_PRM_DEVICE_INST,
				 OMAP4_RM_RSTCTRL);

	/* OCP barrier */
	v = omap4_prm_read_inst_reg(OMAP4430_PRM_DEVICE_INST,
				    OMAP4_RM_RSTCTRL);
}
=======
/* PRM VP */

/*
 * struct omap4_vp - OMAP4 VP register access description.
 * @irqstatus_mpu: offset to IRQSTATUS_MPU register for VP
 * @tranxdone_status: VP_TRANXDONE_ST bitmask in PRM_IRQSTATUS_MPU reg
 */
struct omap4_vp {
	u32 irqstatus_mpu;
	u32 tranxdone_status;
};

static struct omap4_vp omap4_vp[] = {
	[OMAP4_VP_VDD_MPU_ID] = {
		.irqstatus_mpu = OMAP4_PRM_IRQSTATUS_MPU_2_OFFSET,
		.tranxdone_status = OMAP4430_VP_MPU_TRANXDONE_ST_MASK,
	},
	[OMAP4_VP_VDD_IVA_ID] = {
		.irqstatus_mpu = OMAP4_PRM_IRQSTATUS_MPU_OFFSET,
		.tranxdone_status = OMAP4430_VP_IVA_TRANXDONE_ST_MASK,
	},
	[OMAP4_VP_VDD_CORE_ID] = {
		.irqstatus_mpu = OMAP4_PRM_IRQSTATUS_MPU_OFFSET,
		.tranxdone_status = OMAP4430_VP_CORE_TRANXDONE_ST_MASK,
	},
};

u32 omap4_prm_vp_check_txdone(u8 vp_id)
{
	struct omap4_vp *vp = &omap4_vp[vp_id];
	u32 irqstatus;

	irqstatus = omap4_prminst_read_inst_reg(OMAP4430_PRM_PARTITION,
						OMAP4430_PRM_OCP_SOCKET_INST,
						vp->irqstatus_mpu);
	return irqstatus & vp->tranxdone_status;
}

void omap4_prm_vp_clear_txdone(u8 vp_id)
{
	struct omap4_vp *vp = &omap4_vp[vp_id];

	omap4_prminst_write_inst_reg(vp->tranxdone_status,
				     OMAP4430_PRM_PARTITION,
				     OMAP4430_PRM_OCP_SOCKET_INST,
				     vp->irqstatus_mpu);
};

u32 omap4_prm_vcvp_read(u8 offset)
{
	return omap4_prminst_read_inst_reg(OMAP4430_PRM_PARTITION,
					   OMAP4430_PRM_DEVICE_INST, offset);
}

void omap4_prm_vcvp_write(u32 val, u8 offset)
{
	omap4_prminst_write_inst_reg(val, OMAP4430_PRM_PARTITION,
				     OMAP4430_PRM_DEVICE_INST, offset);
}

u32 omap4_prm_vcvp_rmw(u32 mask, u32 bits, u8 offset)
{
	return omap4_prminst_rmw_inst_reg_bits(mask, bits,
					       OMAP4430_PRM_PARTITION,
					       OMAP4430_PRM_DEVICE_INST,
					       offset);
}

static inline u32 _read_pending_irq_reg(u16 irqen_offs, u16 irqst_offs)
{
	u32 mask, st;

	/* XXX read mask from RAM? */
	mask = omap4_prm_read_inst_reg(OMAP4430_PRM_OCP_SOCKET_INST,
				       irqen_offs);
	st = omap4_prm_read_inst_reg(OMAP4430_PRM_OCP_SOCKET_INST, irqst_offs);

	return mask & st;
}

/**
 * omap44xx_prm_read_pending_irqs - read pending PRM MPU IRQs into @events
 * @events: ptr to two consecutive u32s, preallocated by caller
 *
 * Read PRM_IRQSTATUS_MPU* bits, AND'ed with the currently-enabled PRM
 * MPU IRQs, and store the result into the two u32s pointed to by @events.
 * No return value.
 */
void omap44xx_prm_read_pending_irqs(unsigned long *events)
{
	events[0] = _read_pending_irq_reg(OMAP4_PRM_IRQENABLE_MPU_OFFSET,
					  OMAP4_PRM_IRQSTATUS_MPU_OFFSET);

	events[1] = _read_pending_irq_reg(OMAP4_PRM_IRQENABLE_MPU_2_OFFSET,
					  OMAP4_PRM_IRQSTATUS_MPU_2_OFFSET);
}

/**
 * omap44xx_prm_ocp_barrier - force buffered MPU writes to the PRM to complete
 *
 * Force any buffered writes to the PRM IP block to complete.  Needed
 * by the PRM IRQ handler, which reads and writes directly to the IP
 * block, to avoid race conditions after acknowledging or clearing IRQ
 * bits.  No return value.
 */
void omap44xx_prm_ocp_barrier(void)
{
	omap4_prm_read_inst_reg(OMAP4430_PRM_OCP_SOCKET_INST,
				OMAP4_REVISION_PRM_OFFSET);
}

/**
 * omap44xx_prm_save_and_clear_irqen - save/clear PRM_IRQENABLE_MPU* regs
 * @saved_mask: ptr to a u32 array to save IRQENABLE bits
 *
 * Save the PRM_IRQENABLE_MPU and PRM_IRQENABLE_MPU_2 registers to
 * @saved_mask.  @saved_mask must be allocated by the caller.
 * Intended to be used in the PRM interrupt handler suspend callback.
 * The OCP barrier is needed to ensure the write to disable PRM
 * interrupts reaches the PRM before returning; otherwise, spurious
 * interrupts might occur.  No return value.
 */
void omap44xx_prm_save_and_clear_irqen(u32 *saved_mask)
{
	saved_mask[0] =
		omap4_prm_read_inst_reg(OMAP4430_PRM_OCP_SOCKET_INST,
					OMAP4_PRM_IRQSTATUS_MPU_OFFSET);
	saved_mask[1] =
		omap4_prm_read_inst_reg(OMAP4430_PRM_OCP_SOCKET_INST,
					OMAP4_PRM_IRQSTATUS_MPU_2_OFFSET);

	omap4_prm_write_inst_reg(0, OMAP4430_PRM_OCP_SOCKET_INST,
				 OMAP4_PRM_IRQENABLE_MPU_OFFSET);
	omap4_prm_write_inst_reg(0, OMAP4430_PRM_OCP_SOCKET_INST,
				 OMAP4_PRM_IRQENABLE_MPU_2_OFFSET);

	/* OCP barrier */
	omap4_prm_read_inst_reg(OMAP4430_PRM_OCP_SOCKET_INST,
				OMAP4_REVISION_PRM_OFFSET);
}

/**
 * omap44xx_prm_restore_irqen - set PRM_IRQENABLE_MPU* registers from args
 * @saved_mask: ptr to a u32 array of IRQENABLE bits saved previously
 *
 * Restore the PRM_IRQENABLE_MPU and PRM_IRQENABLE_MPU_2 registers from
 * @saved_mask.  Intended to be used in the PRM interrupt handler resume
 * callback to restore values saved by omap44xx_prm_save_and_clear_irqen().
 * No OCP barrier should be needed here; any pending PRM interrupts will fire
 * once the writes reach the PRM.  No return value.
 */
void omap44xx_prm_restore_irqen(u32 *saved_mask)
{
	omap4_prm_write_inst_reg(saved_mask[0], OMAP4430_PRM_OCP_SOCKET_INST,
				 OMAP4_PRM_IRQENABLE_MPU_OFFSET);
	omap4_prm_write_inst_reg(saved_mask[1], OMAP4430_PRM_OCP_SOCKET_INST,
				 OMAP4_PRM_IRQENABLE_MPU_2_OFFSET);
}

static int __init omap4xxx_prcm_init(void)
{
	if (cpu_is_omap44xx())
		return omap_prcm_register_chain_handler(&omap4_prcm_irq_setup);
	return 0;
}
subsys_initcall(omap4xxx_prcm_init);
>>>>>>> refs/remotes/origin/cm-10.0
