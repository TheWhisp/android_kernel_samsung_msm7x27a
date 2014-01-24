/*
 * MSM MDDI Transport
 *
 * Copyright (C) 2007 Google Incorporated
<<<<<<< HEAD
 * Copyright (c) 2007-2012, The Linux Foundation. All rights reserved.
=======
 * Copyright (C) 2007 QUALCOMM Incorporated
>>>>>>> refs/remotes/origin/master
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
<<<<<<< HEAD
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
=======
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
>>>>>>> refs/remotes/origin/master
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <mach/hardware.h>
#include <asm/io.h>

#include <asm/system.h>
#include <asm/mach-types.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include "msm_fb.h"
#include "mddihosti.h"
#include "mddihost.h"
#include <mach/gpio.h>
#include <mach/clk.h>

static int mddi_probe(struct platform_device *pdev);
static int mddi_remove(struct platform_device *pdev);

static int mddi_off(struct platform_device *pdev);
static int mddi_on(struct platform_device *pdev);

#ifdef CONFIG_PM
static int mddi_suspend(struct platform_device *pdev, pm_message_t state);
static int mddi_resume(struct platform_device *pdev);
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
static void mddi_early_suspend(struct early_suspend *h);
static void mddi_early_resume(struct early_suspend *h);
#endif

static void pmdh_clk_disable(void);
static void pmdh_clk_enable(void);
static struct platform_device *pdev_list[MSM_FB_MAX_DEV_LIST];
static int pdev_list_cnt;
static struct clk *mddi_clk;
static struct clk *mddi_pclk;
static struct mddi_platform_data *mddi_pdata;

DEFINE_MUTEX(mddi_timer_lock);

static int mddi_runtime_suspend(struct device *dev)
{
	dev_dbg(dev, "pm_runtime: suspending...\n");
	return 0;
}

static int mddi_runtime_resume(struct device *dev)
{
	dev_dbg(dev, "pm_runtime: resuming...\n");
	return 0;
}

static int mddi_runtime_idle(struct device *dev)
{
	dev_dbg(dev, "pm_runtime: idling...\n");
	return 0;
}

static struct dev_pm_ops mddi_dev_pm_ops = {
	.runtime_suspend = mddi_runtime_suspend,
	.runtime_resume = mddi_runtime_resume,
	.runtime_idle = mddi_runtime_idle,
};

static int pmdh_clk_status;
int irq_enabled;
unsigned char mddi_timer_shutdown_flag;

static struct platform_driver mddi_driver = {
	.probe = mddi_probe,
	.remove = mddi_remove,
#ifndef CONFIG_HAS_EARLYSUSPEND
#ifdef CONFIG_PM
	.suspend = mddi_suspend,
	.resume = mddi_resume,
#endif
#endif
	.shutdown = NULL,
	.driver = {
		.name = "mddi",
		.pm = &mddi_dev_pm_ops,
		   },
};

extern int int_mddi_pri_flag;
DEFINE_MUTEX(pmdh_clk_lock);

int pmdh_clk_func(int value)
{
	int ret = 0;

	switch (value) {
	case 0:
		pmdh_clk_disable();
		break;
	case 1:
		pmdh_clk_enable();
		break;
	case 2:
	default:
		mutex_lock(&pmdh_clk_lock);
		ret = pmdh_clk_status;
		mutex_unlock(&pmdh_clk_lock);
		break;
	}
	return ret;
}

static void pmdh_clk_disable()
{
	mutex_lock(&pmdh_clk_lock);
	if (pmdh_clk_status == 0) {
		mutex_unlock(&pmdh_clk_lock);
		return;
	}

	if (mddi_host_timer.function) {
		mutex_lock(&mddi_timer_lock);
		mddi_timer_shutdown_flag = 1;
		mutex_unlock(&mddi_timer_lock);
		del_timer_sync(&mddi_host_timer);
		mutex_lock(&mddi_timer_lock);
		mddi_timer_shutdown_flag = 0;
		mutex_unlock(&mddi_timer_lock);
	}
	if (int_mddi_pri_flag && irq_enabled) {
		disable_irq(INT_MDDI_PRI);
		irq_enabled = 0;
	}

	if (mddi_clk) {
		clk_disable_unprepare(mddi_clk);
		pmdh_clk_status = 0;
	}
	if (mddi_pclk)
		clk_disable_unprepare(mddi_pclk);
	mutex_unlock(&pmdh_clk_lock);
}

static void pmdh_clk_enable()
{
	mutex_lock(&pmdh_clk_lock);
	if (pmdh_clk_status == 1) {
		mutex_unlock(&pmdh_clk_lock);
		return;
	}

	if (mddi_clk) {
		clk_prepare_enable(mddi_clk);
		pmdh_clk_status = 1;
	}
	if (mddi_pclk)
		clk_prepare_enable(mddi_pclk);

	if (int_mddi_pri_flag && !irq_enabled) {
		enable_irq(INT_MDDI_PRI);
		irq_enabled = 1;
	}

	if (mddi_host_timer.function)
		mddi_host_timer_service(0);

	mutex_unlock(&pmdh_clk_lock);
}

static int mddi_off(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;
	boolean dma_pending, dma_update_flag;
	int ret, i;

	mfd = platform_get_drvdata(pdev);

	for (i = 0; i < 6; i++) {
		dma_update_flag = mfd->dma_update_flag;
		dma_pending = mfd->dma->busy;
		if (dma_update_flag && !dma_pending)
			break;
		msleep(5);
	}

	pmdh_clk_enable();
	ret = panel_next_off(pdev);
	pmdh_clk_disable();

	if (mddi_pdata && mddi_pdata->mddi_power_save)
		mddi_pdata->mddi_power_save(0);
#ifdef CONFIG_MSM_BUS_SCALING
	mdp_bus_scale_update_request(0);
#else
	if (mfd->ebi1_clk)
		clk_disable_unprepare(mfd->ebi1_clk);
#endif
	pm_runtime_put(&pdev->dev);
	return ret;
}

static int mddi_on(struct platform_device *pdev)
{
	int ret = 0;
	u32 clk_rate;
	struct msm_fb_data_type *mfd;
#ifdef ENABLE_FWD_LINK_SKEW_CALIBRATION
	mddi_host_type host_idx = MDDI_HOST_PRIM;
	u32 stat_reg;
#endif

	mfd = platform_get_drvdata(pdev);
	pm_runtime_get(&pdev->dev);
	if (mddi_pdata && mddi_pdata->mddi_power_save)
		mddi_pdata->mddi_power_save(1);

	pmdh_clk_enable();
#ifdef ENABLE_FWD_LINK_SKEW_CALIBRATION
	if (mddi_client_type < 2) {
		/* For skew calibration, clock should be less than 50MHz */
		clk_rate = clk_round_rate(mddi_clk, 49000000);
		if (!clk_set_rate(mddi_clk, clk_rate)) {
			stat_reg = mddi_host_reg_in(STAT);
			printk(KERN_DEBUG "\n stat_reg = 0x%x", stat_reg);
			mddi_host_reg_out(CMD, MDDI_CMD_HIBERNATE);
			if (stat_reg & (0x1 << 4))
				mddi_host_reg_out(CMD, MDDI_CMD_LINK_ACTIVE);

			mddi_host_reg_out(CMD, MDDI_CMD_SEND_RTD);
			mddi_send_fw_link_skew_cal(host_idx);
			mddi_host_reg_out(CMD, MDDI_CMD_SEND_RTD);
			mddi_host_reg_out(CMD, MDDI_CMD_HIBERNATE | 1);
		} else {
			printk(KERN_ERR "%s: clk_set_rate failed\n",
				__func__);
		}
	}
#endif

	clk_rate = mfd->fbi->var.pixclock;
	clk_rate = min(clk_rate, mfd->panel_info.clk_max);

	if (mddi_pdata &&
	    mddi_pdata->mddi_sel_clk &&
	    mddi_pdata->mddi_sel_clk(&clk_rate))
			printk(KERN_ERR
			  "%s: can't select mddi io clk targate rate = %d\n",
			  __func__, clk_rate);

	clk_rate = clk_round_rate(mddi_clk, clk_rate);
	if (clk_set_rate(mddi_clk, clk_rate) < 0)
		printk(KERN_ERR "%s: clk_set_rate failed\n",
			__func__);

#ifdef CONFIG_MSM_BUS_SCALING
	mdp_bus_scale_update_request(2);
#else
	if (mfd->ebi1_clk)
		clk_prepare_enable(mfd->ebi1_clk);
#endif
	ret = panel_next_on(pdev);

	return ret;
}

static int mddi_resource_initialized;

static int mddi_probe(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;
	struct platform_device *mdp_dev = NULL;
	struct msm_fb_panel_data *pdata = NULL;
	int rc;
	resource_size_t size ;
	u32 clk_rate;
	unsigned long rate;
	int ret;
	struct clk *ebi1_clk = NULL;

	if ((pdev->id == 0) && (pdev->num_resources >= 0)) {
		mddi_pdata = pdev->dev.platform_data;
		pmdh_clk_status = 0;

		mddi_clk = clk_get(&pdev->dev, "core_clk");
		if (IS_ERR(mddi_clk)) {
			pr_err("can't find mddi_clk\n");
			return PTR_ERR(mddi_clk);
		}
		rate = clk_round_rate(mddi_clk, 49000000);
		ret = clk_set_rate(mddi_clk, rate);
		if (ret)
			pr_err("Can't set mddi_clk min rate to %lu\n",
									rate);

		pr_info("mddi_clk init rate is %lu\n",
			clk_get_rate(mddi_clk));
		mddi_pclk = clk_get(&pdev->dev, "iface_clk");
		if (IS_ERR(mddi_pclk))
			mddi_pclk = NULL;
		pmdh_clk_enable();

#ifndef CONFIG_MSM_BUS_SCALING
		ebi1_clk = clk_get(&pdev->dev, "mem_clk");
		if (IS_ERR(ebi1_clk))
			return PTR_ERR(ebi1_clk);
		clk_set_rate(ebi1_clk, 65000000);
#endif

		size =  resource_size(&pdev->resource[0]);
		msm_pmdh_base =  ioremap(pdev->resource[0].start, size);

		MSM_FB_INFO("primary mddi base phy_addr = 0x%x virt = 0x%x\n",
				pdev->resource[0].start, (int) msm_pmdh_base);

		if (unlikely(!msm_pmdh_base))
			return -ENOMEM;

		if (mddi_pdata && mddi_pdata->mddi_power_save)
			mddi_pdata->mddi_power_save(1);

		mddi_resource_initialized = 1;
		return 0;
	}

	if (!mddi_resource_initialized)
		return -EPERM;

	mfd = platform_get_drvdata(pdev);
	mfd->ebi1_clk = ebi1_clk;

	if (!mfd)
		return -ENODEV;

	if (mfd->key != MFD_KEY)
		return -EINVAL;

	if (pdev_list_cnt >= MSM_FB_MAX_DEV_LIST)
		return -ENOMEM;

	mdp_dev = platform_device_alloc("mdp", pdev->id);
	if (!mdp_dev)
		return -ENOMEM;

	/*
	 * link to the latest pdev
	 */
	mfd->pdev = mdp_dev;
	mfd->dest = DISPLAY_LCD;

	/*
	 * alloc panel device data
	 */
	if (platform_device_add_data
	    (mdp_dev, pdev->dev.platform_data,
	     sizeof(struct msm_fb_panel_data))) {
		printk(KERN_ERR "mddi_probe: platform_device_add_data failed!\n");
		platform_device_put(mdp_dev);
		return -ENOMEM;
	}
	/*
	 * data chain
	 */
	pdata = mdp_dev->dev.platform_data;
	pdata->on = mddi_on;
	pdata->off = mddi_off;
	pdata->next = pdev;
	pdata->clk_func = pmdh_clk_func;
	/*
	 * get/set panel specific fb info
	 */
	mfd->panel_info = pdata->panel_info;

	if (mfd->index == 0)
		mfd->fb_imgType = MSMFB_DEFAULT_TYPE;
	else
		mfd->fb_imgType = MDP_RGB_565;

	clk_rate = mfd->panel_info.clk_max;
	if (mddi_pdata &&
	    mddi_pdata->mddi_sel_clk &&
	    mddi_pdata->mddi_sel_clk(&clk_rate))
			printk(KERN_ERR
			  "%s: can't select mddi io clk targate rate = %d\n",
			  __func__, clk_rate);

	if (clk_set_max_rate(mddi_clk, clk_rate) < 0)
		printk(KERN_ERR "%s: clk_set_max_rate failed\n", __func__);
	mfd->panel_info.clk_rate = mfd->panel_info.clk_min;

	if (!mddi_client_type)
		mddi_client_type = mfd->panel_info.lcd.rev;
	else if (!mfd->panel_info.lcd.rev)
		printk(KERN_ERR
		"%s: mddi client is trying to revert back to type 1	!!!\n",
		__func__);

	/*
	 * set driver data
	 */
	platform_set_drvdata(mdp_dev, mfd);
	rc = pm_runtime_set_active(&pdev->dev);
	if (rc < 0)
		printk(KERN_ERR "pm_runtime: fail to set active\n");

	rc = 0;
	pm_runtime_enable(&pdev->dev);
	/*
	 * register in mdp driver
	 */
	rc = platform_device_add(mdp_dev);
	if (rc)
		goto mddi_probe_err;

	pdev_list[pdev_list_cnt++] = pdev;

#ifdef CONFIG_HAS_EARLYSUSPEND
	mfd->mddi_early_suspend.level = EARLY_SUSPEND_LEVEL_DISABLE_FB;
	mfd->mddi_early_suspend.suspend = mddi_early_suspend;
	mfd->mddi_early_suspend.resume = mddi_early_resume;
	register_early_suspend(&mfd->mddi_early_suspend);
#endif

	return 0;

mddi_probe_err:
	platform_device_put(mdp_dev);
	return rc;
}

static int mddi_pad_ctrl;
static int mddi_power_locked;

int mddi_client_power(unsigned int client_id)
{
	int ret = 0;
	if (mddi_pdata && mddi_pdata->mddi_client_power)
		ret = mddi_pdata->mddi_client_power(client_id);
	return ret;
}

void mddi_disable(int lock)
{
	mddi_host_type host_idx = MDDI_HOST_PRIM;

	if (mddi_power_locked)
		return;

	if (lock)
		mddi_power_locked = 1;
	pmdh_clk_enable();

	mddi_pad_ctrl = mddi_host_reg_in(PAD_CTL);
	mddi_host_reg_out(PAD_CTL, 0x0);

	pmdh_clk_disable();

	if (mddi_pdata && mddi_pdata->mddi_power_save)
		mddi_pdata->mddi_power_save(0);
}

#ifdef CONFIG_PM
static int mddi_is_in_suspend;

static int mddi_suspend(struct platform_device *pdev, pm_message_t state)
{
	mddi_host_type host_idx = MDDI_HOST_PRIM;
	if (mddi_is_in_suspend)
		return 0;

	mddi_is_in_suspend = 1;

	if (mddi_power_locked)
		return 0;

	pmdh_clk_enable();

	mddi_pad_ctrl = mddi_host_reg_in(PAD_CTL);
	mddi_host_reg_out(PAD_CTL, 0x0);

	pmdh_clk_disable();

	return 0;
}

static int mddi_resume(struct platform_device *pdev)
{
	mddi_host_type host_idx = MDDI_HOST_PRIM;

	if (!mddi_is_in_suspend)
		return 0;

	mddi_is_in_suspend = 0;

	if (mddi_power_locked)
		return 0;

	pmdh_clk_enable();

	mddi_host_reg_out(PAD_CTL, mddi_pad_ctrl);


	return 0;
}
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
static void mddi_early_suspend(struct early_suspend *h)
{
	pm_message_t state;
	struct msm_fb_data_type *mfd = container_of(h, struct msm_fb_data_type,
							mddi_early_suspend);

	state.event = PM_EVENT_SUSPEND;
	mddi_suspend(mfd->pdev, state);
}

static void mddi_early_resume(struct early_suspend *h)
{
	struct msm_fb_data_type *mfd = container_of(h, struct msm_fb_data_type,
							mddi_early_suspend);
	mddi_resume(mfd->pdev);
}
#endif

static int mddi_remove(struct platform_device *pdev)
{
	pm_runtime_disable(&pdev->dev);
	if (mddi_host_timer.function) {
		mutex_lock(&mddi_timer_lock);
		mddi_timer_shutdown_flag = 1;
		mutex_unlock(&mddi_timer_lock);
		del_timer_sync(&mddi_host_timer);
		mutex_lock(&mddi_timer_lock);
		mddi_timer_shutdown_flag = 0;
		mutex_unlock(&mddi_timer_lock);
	}

	iounmap(msm_pmdh_base);

	return 0;
}

static int mddi_register_driver(void)
{
	return platform_driver_register(&mddi_driver);
}

static int __init mddi_driver_init(void)
{
	int ret;

	ret = mddi_register_driver();
	if (ret) {
		pmdh_clk_disable();
		clk_put(mddi_clk);
		if (mddi_pclk)
			clk_put(mddi_pclk);
		printk(KERN_ERR "mddi_register_driver() failed!\n");
		return ret;
	}

	mddi_init();

	return ret;
}

module_init(mddi_driver_init);
=======
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/gfp.h>
#include <linux/spinlock.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/sched.h>
#include <linux/platform_data/video-msm_fb.h>
#include "mddi_hw.h"

#define FLAG_DISABLE_HIBERNATION 0x0001
#define FLAG_HAVE_CAPS		 0x0002
#define FLAG_HAS_VSYNC_IRQ	 0x0004
#define FLAG_HAVE_STATUS	 0x0008

#define CMD_GET_CLIENT_CAP     0x0601
#define CMD_GET_CLIENT_STATUS  0x0602

union mddi_rev {
	unsigned char raw[MDDI_REV_BUFFER_SIZE];
	struct mddi_rev_packet hdr;
	struct mddi_client_status status;
	struct mddi_client_caps caps;
	struct mddi_register_access reg;
};

struct reg_read_info {
	struct completion done;
	uint32_t reg;
	uint32_t status;
	uint32_t result;
};

struct mddi_info {
	uint16_t flags;
	uint16_t version;
	char __iomem *base;
	int irq;
	struct clk *clk;
	struct msm_mddi_client_data client_data;

	/* buffer for rev encap packets */
	void *rev_data;
	dma_addr_t rev_addr;
	struct mddi_llentry *reg_write_data;
	dma_addr_t reg_write_addr;
	struct mddi_llentry *reg_read_data;
	dma_addr_t reg_read_addr;
	size_t rev_data_curr;

	spinlock_t int_lock;
	uint32_t int_enable;
	uint32_t got_int;
	wait_queue_head_t int_wait;

	struct mutex reg_write_lock;
	struct mutex reg_read_lock;
	struct reg_read_info *reg_read;

	struct mddi_client_caps caps;
	struct mddi_client_status status;

	void (*power_client)(struct msm_mddi_client_data *, int);

	/* client device published to bind us to the
	 * appropriate mddi_client driver
	 */
	char client_name[20];

	struct platform_device client_pdev;
};

static void mddi_init_rev_encap(struct mddi_info *mddi);

#define mddi_readl(r) readl(mddi->base + (MDDI_##r))
#define mddi_writel(v, r) writel((v), mddi->base + (MDDI_##r))

void mddi_activate_link(struct msm_mddi_client_data *cdata)
{
	struct mddi_info *mddi = container_of(cdata, struct mddi_info,
					      client_data);

	mddi_writel(MDDI_CMD_LINK_ACTIVE, CMD);
}

static void mddi_handle_link_list_done(struct mddi_info *mddi)
{
}

static void mddi_reset_rev_encap_ptr(struct mddi_info *mddi)
{
	printk(KERN_INFO "mddi: resetting rev ptr\n");
	mddi->rev_data_curr = 0;
	mddi_writel(mddi->rev_addr, REV_PTR);
	mddi_writel(mddi->rev_addr, REV_PTR);
	mddi_writel(MDDI_CMD_FORCE_NEW_REV_PTR, CMD);
}

static void mddi_handle_rev_data(struct mddi_info *mddi, union mddi_rev *rev)
{
	int i;
	struct reg_read_info *ri;

	if ((rev->hdr.length <= MDDI_REV_BUFFER_SIZE - 2) &&
	   (rev->hdr.length >= sizeof(struct mddi_rev_packet) - 2)) {

		switch (rev->hdr.type) {
		case TYPE_CLIENT_CAPS:
			memcpy(&mddi->caps, &rev->caps,
			       sizeof(struct mddi_client_caps));
			mddi->flags |= FLAG_HAVE_CAPS;
			wake_up(&mddi->int_wait);
			break;
		case TYPE_CLIENT_STATUS:
			memcpy(&mddi->status, &rev->status,
			       sizeof(struct mddi_client_status));
			mddi->flags |= FLAG_HAVE_STATUS;
			wake_up(&mddi->int_wait);
			break;
		case TYPE_REGISTER_ACCESS:
			ri = mddi->reg_read;
			if (ri == 0) {
				printk(KERN_INFO "rev: got reg %x = %x without "
						 " pending read\n",
				       rev->reg.register_address,
				       rev->reg.register_data_list);
				break;
			}
			if (ri->reg != rev->reg.register_address) {
				printk(KERN_INFO "rev: got reg %x = %x for "
						 "wrong register, expected "
						 "%x\n",
				       rev->reg.register_address,
				       rev->reg.register_data_list, ri->reg);
				break;
			}
			mddi->reg_read = NULL;
			ri->status = 0;
			ri->result = rev->reg.register_data_list;
			complete(&ri->done);
			break;
		default:
			printk(KERN_INFO "rev: unknown reverse packet: "
					 "len=%04x type=%04x CURR_REV_PTR=%x\n",
			       rev->hdr.length, rev->hdr.type,
			       mddi_readl(CURR_REV_PTR));
			for (i = 0; i < rev->hdr.length + 2; i++) {
				if ((i % 16) == 0)
					printk(KERN_INFO "\n");
				printk(KERN_INFO " %02x", rev->raw[i]);
			}
			printk(KERN_INFO "\n");
			mddi_reset_rev_encap_ptr(mddi);
		}
	} else {
		printk(KERN_INFO "bad rev length, %d, CURR_REV_PTR %x\n",
		       rev->hdr.length, mddi_readl(CURR_REV_PTR));
		mddi_reset_rev_encap_ptr(mddi);
	}
}

static void mddi_wait_interrupt(struct mddi_info *mddi, uint32_t intmask);

static void mddi_handle_rev_data_avail(struct mddi_info *mddi)
{
	uint32_t rev_data_count;
	uint32_t rev_crc_err_count;
	struct reg_read_info *ri;
	size_t prev_offset;
	uint16_t length;

	union mddi_rev *crev = mddi->rev_data + mddi->rev_data_curr;

	/* clear the interrupt */
	mddi_writel(MDDI_INT_REV_DATA_AVAIL, INT);
	rev_data_count = mddi_readl(REV_PKT_CNT);
	rev_crc_err_count = mddi_readl(REV_CRC_ERR);
	if (rev_data_count > 1)
		printk(KERN_INFO "rev_data_count %d\n", rev_data_count);

	if (rev_crc_err_count) {
		printk(KERN_INFO "rev_crc_err_count %d, INT %x\n",
		       rev_crc_err_count,  mddi_readl(INT));
		ri = mddi->reg_read;
		if (ri == 0) {
			printk(KERN_INFO "rev: got crc error without pending "
			       "read\n");
		} else {
			mddi->reg_read = NULL;
			ri->status = -EIO;
			ri->result = -1;
			complete(&ri->done);
		}
	}

	if (rev_data_count == 0)
		return;

	prev_offset = mddi->rev_data_curr;

	length = *((uint8_t *)mddi->rev_data + mddi->rev_data_curr);
	mddi->rev_data_curr++;
	if (mddi->rev_data_curr == MDDI_REV_BUFFER_SIZE)
		mddi->rev_data_curr = 0;
	length += *((uint8_t *)mddi->rev_data + mddi->rev_data_curr) << 8;
	mddi->rev_data_curr += 1 + length;
	if (mddi->rev_data_curr >= MDDI_REV_BUFFER_SIZE)
		mddi->rev_data_curr =
			mddi->rev_data_curr % MDDI_REV_BUFFER_SIZE;

	if (length > MDDI_REV_BUFFER_SIZE - 2) {
		printk(KERN_INFO "mddi: rev data length greater than buffer"
			"size\n");
		mddi_reset_rev_encap_ptr(mddi);
		return;
	}

	if (prev_offset + 2 + length >= MDDI_REV_BUFFER_SIZE) {
		union mddi_rev tmprev;
		size_t rem = MDDI_REV_BUFFER_SIZE - prev_offset;
		memcpy(&tmprev.raw[0], mddi->rev_data + prev_offset, rem);
		memcpy(&tmprev.raw[rem], mddi->rev_data, 2 + length - rem);
		mddi_handle_rev_data(mddi, &tmprev);
	} else {
		mddi_handle_rev_data(mddi, crev);
	}

	if (prev_offset < MDDI_REV_BUFFER_SIZE / 2 &&
	    mddi->rev_data_curr >= MDDI_REV_BUFFER_SIZE / 2) {
		mddi_writel(mddi->rev_addr, REV_PTR);
	}
}

static irqreturn_t mddi_isr(int irq, void *data)
{
	struct msm_mddi_client_data *cdata = data;
	struct mddi_info *mddi = container_of(cdata, struct mddi_info,
					      client_data);
	uint32_t active, status;

	spin_lock(&mddi->int_lock);

	active = mddi_readl(INT);
	status = mddi_readl(STAT);

	mddi_writel(active, INT);

	/* ignore any interrupts we have disabled */
	active &= mddi->int_enable;

	mddi->got_int |= active;
	wake_up(&mddi->int_wait);

	if (active & MDDI_INT_PRI_LINK_LIST_DONE) {
		mddi->int_enable &= (~MDDI_INT_PRI_LINK_LIST_DONE);
		mddi_handle_link_list_done(mddi);
	}
	if (active & MDDI_INT_REV_DATA_AVAIL)
		mddi_handle_rev_data_avail(mddi);

	if (active & ~MDDI_INT_NEED_CLEAR)
		mddi->int_enable &= ~(active & ~MDDI_INT_NEED_CLEAR);

	if (active & MDDI_INT_LINK_ACTIVE) {
		mddi->int_enable &= (~MDDI_INT_LINK_ACTIVE);
		mddi->int_enable |= MDDI_INT_IN_HIBERNATION;
	}

	if (active & MDDI_INT_IN_HIBERNATION) {
		mddi->int_enable &= (~MDDI_INT_IN_HIBERNATION);
		mddi->int_enable |= MDDI_INT_LINK_ACTIVE;
	}

	mddi_writel(mddi->int_enable, INTEN);
	spin_unlock(&mddi->int_lock);

	return IRQ_HANDLED;
}

static long mddi_wait_interrupt_timeout(struct mddi_info *mddi,
					uint32_t intmask, int timeout)
{
	unsigned long irq_flags;

	spin_lock_irqsave(&mddi->int_lock, irq_flags);
	mddi->got_int &= ~intmask;
	mddi->int_enable |= intmask;
	mddi_writel(mddi->int_enable, INTEN);
	spin_unlock_irqrestore(&mddi->int_lock, irq_flags);
	return wait_event_timeout(mddi->int_wait, mddi->got_int & intmask,
				  timeout);
}

static void mddi_wait_interrupt(struct mddi_info *mddi, uint32_t intmask)
{
	if (mddi_wait_interrupt_timeout(mddi, intmask, HZ/10) == 0)
		printk(KERN_INFO "mddi_wait_interrupt %d, timeout "
		       "waiting for %x, INT = %x, STAT = %x gotint = %x\n",
		       current->pid, intmask, mddi_readl(INT), mddi_readl(STAT),
		       mddi->got_int);
}

static void mddi_init_rev_encap(struct mddi_info *mddi)
{
	memset(mddi->rev_data, 0xee, MDDI_REV_BUFFER_SIZE);
	mddi_writel(mddi->rev_addr, REV_PTR);
	mddi_writel(MDDI_CMD_FORCE_NEW_REV_PTR, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
}

void mddi_set_auto_hibernate(struct msm_mddi_client_data *cdata, int on)
{
	struct mddi_info *mddi = container_of(cdata, struct mddi_info,
					      client_data);
	mddi_writel(MDDI_CMD_POWERDOWN, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_IN_HIBERNATION);
	mddi_writel(MDDI_CMD_HIBERNATE | !!on, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
}


static uint16_t mddi_init_registers(struct mddi_info *mddi)
{
	mddi_writel(0x0001, VERSION);
	mddi_writel(MDDI_HOST_BYTES_PER_SUBFRAME, BPS);
	mddi_writel(0x0003, SPM); /* subframes per media */
	mddi_writel(0x0005, TA1_LEN);
	mddi_writel(MDDI_HOST_TA2_LEN, TA2_LEN);
	mddi_writel(0x0096, DRIVE_HI);
	/* 0x32 normal, 0x50 for Toshiba display */
	mddi_writel(0x0050, DRIVE_LO);
	mddi_writel(0x003C, DISP_WAKE); /* wakeup counter */
	mddi_writel(MDDI_HOST_REV_RATE_DIV, REV_RATE_DIV);

	mddi_writel(MDDI_REV_BUFFER_SIZE, REV_SIZE);
	mddi_writel(MDDI_MAX_REV_PKT_SIZE, REV_ENCAP_SZ);

	/* disable periodic rev encap */
	mddi_writel(MDDI_CMD_PERIODIC_REV_ENCAP, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);

	if (mddi_readl(PAD_CTL) == 0) {
		/* If we are turning on band gap, need to wait 5us before
		 * turning on the rest of the PAD */
		mddi_writel(0x08000, PAD_CTL);
		udelay(5);
	}

	/* Recommendation from PAD hw team */
	mddi_writel(0xa850f, PAD_CTL);


	/* Need an even number for counts */
	mddi_writel(0x60006, DRIVER_START_CNT);

	mddi_set_auto_hibernate(&mddi->client_data, 0);

	mddi_writel(MDDI_CMD_DISP_IGNORE, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);

	mddi_init_rev_encap(mddi);
	return mddi_readl(CORE_VER) & 0xffff;
}

static void mddi_suspend(struct msm_mddi_client_data *cdata)
{
	struct mddi_info *mddi = container_of(cdata, struct mddi_info,
					      client_data);
	/* turn off the client */
	if (mddi->power_client)
		mddi->power_client(&mddi->client_data, 0);
	/* turn off the link */
	mddi_writel(MDDI_CMD_RESET, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
	/* turn off the clock */
	clk_disable(mddi->clk);
}

static void mddi_resume(struct msm_mddi_client_data *cdata)
{
	struct mddi_info *mddi = container_of(cdata, struct mddi_info,
					      client_data);
	mddi_set_auto_hibernate(&mddi->client_data, 0);
	/* turn on the client */
	if (mddi->power_client)
		mddi->power_client(&mddi->client_data, 1);
	/* turn on the clock */
	clk_enable(mddi->clk);
	/* set up the local registers */
	mddi->rev_data_curr = 0;
	mddi_init_registers(mddi);
	mddi_writel(mddi->int_enable, INTEN);
	mddi_writel(MDDI_CMD_LINK_ACTIVE, CMD);
	mddi_writel(MDDI_CMD_SEND_RTD, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
	mddi_set_auto_hibernate(&mddi->client_data, 1);
}

static int mddi_get_client_caps(struct mddi_info *mddi)
{
	int i, j;

	/* clear any stale interrupts */
	mddi_writel(0xffffffff, INT);

	mddi->int_enable = MDDI_INT_LINK_ACTIVE |
			   MDDI_INT_IN_HIBERNATION |
			   MDDI_INT_PRI_LINK_LIST_DONE |
			   MDDI_INT_REV_DATA_AVAIL |
			   MDDI_INT_REV_OVERFLOW |
			   MDDI_INT_REV_OVERWRITE |
			   MDDI_INT_RTD_FAILURE;
	mddi_writel(mddi->int_enable, INTEN);

	mddi_writel(MDDI_CMD_LINK_ACTIVE, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);

	for (j = 0; j < 3; j++) {
		/* the toshiba vga panel does not respond to get
		 * caps unless you SEND_RTD, but the first SEND_RTD
		 * will fail...
		 */
		for (i = 0; i < 4; i++) {
			uint32_t stat;

			mddi_writel(MDDI_CMD_SEND_RTD, CMD);
			mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
			stat = mddi_readl(STAT);
			printk(KERN_INFO "mddi cmd send rtd: int %x, stat %x, "
					"rtd val %x\n", mddi_readl(INT), stat,
					mddi_readl(RTD_VAL));
			if ((stat & MDDI_STAT_RTD_MEAS_FAIL) == 0)
				break;
			msleep(1);
		}

		mddi_writel(CMD_GET_CLIENT_CAP, CMD);
		mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
		wait_event_timeout(mddi->int_wait, mddi->flags & FLAG_HAVE_CAPS,
				   HZ / 100);

		if (mddi->flags & FLAG_HAVE_CAPS)
			break;
		printk(KERN_INFO "mddi_init, timeout waiting for caps\n");
	}
	return mddi->flags & FLAG_HAVE_CAPS;
}

/* link must be active when this is called */
int mddi_check_status(struct mddi_info *mddi)
{
	int ret = -1, retry = 3;
	mutex_lock(&mddi->reg_read_lock);
	mddi_writel(MDDI_CMD_PERIODIC_REV_ENCAP | 1, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);

	do {
		mddi->flags &= ~FLAG_HAVE_STATUS;
		mddi_writel(CMD_GET_CLIENT_STATUS, CMD);
		mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
		wait_event_timeout(mddi->int_wait,
				   mddi->flags & FLAG_HAVE_STATUS,
				   HZ / 100);

		if (mddi->flags & FLAG_HAVE_STATUS) {
			if (mddi->status.crc_error_count)
				printk(KERN_INFO "mddi status: crc_error "
					"count: %d\n",
					mddi->status.crc_error_count);
			else
				ret = 0;
			break;
		} else
			printk(KERN_INFO "mddi status: failed to get client "
				"status\n");
		mddi_writel(MDDI_CMD_SEND_RTD, CMD);
		mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
	} while (--retry);

	mddi_writel(MDDI_CMD_PERIODIC_REV_ENCAP | 0, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
	mutex_unlock(&mddi->reg_read_lock);
	return ret;
}


void mddi_remote_write(struct msm_mddi_client_data *cdata, uint32_t val,
		       uint32_t reg)
{
	struct mddi_info *mddi = container_of(cdata, struct mddi_info,
					      client_data);
	struct mddi_llentry *ll;
	struct mddi_register_access *ra;

	mutex_lock(&mddi->reg_write_lock);

	ll = mddi->reg_write_data;

	ra = &(ll->u.r);
	ra->length = 14 + 4;
	ra->type = TYPE_REGISTER_ACCESS;
	ra->client_id = 0;
	ra->read_write_info = MDDI_WRITE | 1;
	ra->crc16 = 0;

	ra->register_address = reg;
	ra->register_data_list = val;

	ll->flags = 1;
	ll->header_count = 14;
	ll->data_count = 4;
	ll->data = mddi->reg_write_addr + offsetof(struct mddi_llentry,
						   u.r.register_data_list);
	ll->next = 0;
	ll->reserved = 0;

	mddi_writel(mddi->reg_write_addr, PRI_PTR);

	mddi_wait_interrupt(mddi, MDDI_INT_PRI_LINK_LIST_DONE);
	mutex_unlock(&mddi->reg_write_lock);
}

uint32_t mddi_remote_read(struct msm_mddi_client_data *cdata, uint32_t reg)
{
	struct mddi_info *mddi = container_of(cdata, struct mddi_info,
					      client_data);
	struct mddi_llentry *ll;
	struct mddi_register_access *ra;
	struct reg_read_info ri;
	unsigned s;
	int retry_count = 2;
	unsigned long irq_flags;

	mutex_lock(&mddi->reg_read_lock);

	ll = mddi->reg_read_data;

	ra = &(ll->u.r);
	ra->length = 14;
	ra->type = TYPE_REGISTER_ACCESS;
	ra->client_id = 0;
	ra->read_write_info = MDDI_READ | 1;
	ra->crc16 = 0;

	ra->register_address = reg;

	ll->flags = 0x11;
	ll->header_count = 14;
	ll->data_count = 0;
	ll->data = 0;
	ll->next = 0;
	ll->reserved = 0;

	s = mddi_readl(STAT);

	ri.reg = reg;
	ri.status = -1;

	do {
		init_completion(&ri.done);
		mddi->reg_read = &ri;
		mddi_writel(mddi->reg_read_addr, PRI_PTR);

		mddi_wait_interrupt(mddi, MDDI_INT_PRI_LINK_LIST_DONE);

		/* Enable Periodic Reverse Encapsulation. */
		mddi_writel(MDDI_CMD_PERIODIC_REV_ENCAP | 1, CMD);
		mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
		if (wait_for_completion_timeout(&ri.done, HZ/10) == 0 &&
		    !ri.done.done) {
			printk(KERN_INFO "mddi_remote_read(%x) timeout "
					 "(%d %d %d)\n",
			       reg, ri.status, ri.result, ri.done.done);
			spin_lock_irqsave(&mddi->int_lock, irq_flags);
			mddi->reg_read = NULL;
			spin_unlock_irqrestore(&mddi->int_lock, irq_flags);
			ri.status = -1;
			ri.result = -1;
		}
		if (ri.status == 0)
			break;

		mddi_writel(MDDI_CMD_SEND_RTD, CMD);
		mddi_writel(MDDI_CMD_LINK_ACTIVE, CMD);
		mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
		printk(KERN_INFO "mddi_remote_read: failed, sent "
		       "MDDI_CMD_SEND_RTD: int %x, stat %x, rtd val %x "
		       "curr_rev_ptr %x\n", mddi_readl(INT), mddi_readl(STAT),
		       mddi_readl(RTD_VAL), mddi_readl(CURR_REV_PTR));
	} while (retry_count-- > 0);
	/* Disable Periodic Reverse Encapsulation. */
	mddi_writel(MDDI_CMD_PERIODIC_REV_ENCAP | 0, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
	mddi->reg_read = NULL;
	mutex_unlock(&mddi->reg_read_lock);
	return ri.result;
}

static struct mddi_info mddi_info[2];

static int mddi_clk_setup(struct platform_device *pdev, struct mddi_info *mddi,
			  unsigned long clk_rate)
{
	int ret;

	/* set up the clocks */
	mddi->clk = clk_get(&pdev->dev, "mddi_clk");
	if (IS_ERR(mddi->clk)) {
		printk(KERN_INFO "mddi: failed to get clock\n");
		return PTR_ERR(mddi->clk);
	}
	ret =  clk_enable(mddi->clk);
	if (ret)
		goto fail;
	ret = clk_set_rate(mddi->clk, clk_rate);
	if (ret)
		goto fail;
	return 0;

fail:
	clk_put(mddi->clk);
	return ret;
}

static int __init mddi_rev_data_setup(struct mddi_info *mddi)
{
	void *dma;
	dma_addr_t dma_addr;

	/* set up dma buffer */
	dma = dma_alloc_coherent(NULL, 0x1000, &dma_addr, GFP_KERNEL);
	if (dma == 0)
		return -ENOMEM;
	mddi->rev_data = dma;
	mddi->rev_data_curr = 0;
	mddi->rev_addr = dma_addr;
	mddi->reg_write_data = dma + MDDI_REV_BUFFER_SIZE;
	mddi->reg_write_addr = dma_addr + MDDI_REV_BUFFER_SIZE;
	mddi->reg_read_data = mddi->reg_write_data + 1;
	mddi->reg_read_addr = mddi->reg_write_addr +
			      sizeof(*mddi->reg_write_data);
	return 0;
}

static int mddi_probe(struct platform_device *pdev)
{
	struct msm_mddi_platform_data *pdata = pdev->dev.platform_data;
	struct mddi_info *mddi = &mddi_info[pdev->id];
	struct resource *resource;
	int ret, i;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!resource) {
		printk(KERN_ERR "mddi: no associated mem resource!\n");
		return -ENOMEM;
	}
	mddi->base = ioremap(resource->start, resource_size(resource));
	if (!mddi->base) {
		printk(KERN_ERR "mddi: failed to remap base!\n");
		ret = -EINVAL;
		goto error_ioremap;
	}
	resource = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!resource) {
		printk(KERN_ERR "mddi: no associated irq resource!\n");
		ret = -EINVAL;
		goto error_get_irq_resource;
	}
	mddi->irq = resource->start;
	printk(KERN_INFO "mddi: init() base=0x%p irq=%d\n", mddi->base,
	       mddi->irq);
	mddi->power_client = pdata->power_client;

	mutex_init(&mddi->reg_write_lock);
	mutex_init(&mddi->reg_read_lock);
	spin_lock_init(&mddi->int_lock);
	init_waitqueue_head(&mddi->int_wait);

	ret = mddi_clk_setup(pdev, mddi, pdata->clk_rate);
	if (ret) {
		printk(KERN_ERR "mddi: failed to setup clock!\n");
		goto error_clk_setup;
	}

	ret = mddi_rev_data_setup(mddi);
	if (ret) {
		printk(KERN_ERR "mddi: failed to setup rev data!\n");
		goto error_rev_data;
	}

	mddi->int_enable = 0;
	mddi_writel(mddi->int_enable, INTEN);
	ret = request_irq(mddi->irq, mddi_isr, 0, "mddi",
			  &mddi->client_data);
	if (ret) {
		printk(KERN_ERR "mddi: failed to request enable irq!\n");
		goto error_request_irq;
	}

	/* turn on the mddi client bridge chip */
	if (mddi->power_client)
		mddi->power_client(&mddi->client_data, 1);

	/* initialize the mddi registers */
	mddi_set_auto_hibernate(&mddi->client_data, 0);
	mddi_writel(MDDI_CMD_RESET, CMD);
	mddi_wait_interrupt(mddi, MDDI_INT_NO_CMD_PKTS_PEND);
	mddi->version = mddi_init_registers(mddi);
	if (mddi->version < 0x20) {
		printk(KERN_ERR "mddi: unsupported version 0x%x\n",
		       mddi->version);
		ret = -ENODEV;
		goto error_mddi_version;
	}

	/* read the capabilities off the client */
	if (!mddi_get_client_caps(mddi)) {
		printk(KERN_INFO "mddi: no client found\n");
		/* power down the panel */
		mddi_writel(MDDI_CMD_POWERDOWN, CMD);
		printk(KERN_INFO "mddi powerdown: stat %x\n", mddi_readl(STAT));
		msleep(100);
		printk(KERN_INFO "mddi powerdown: stat %x\n", mddi_readl(STAT));
		return 0;
	}
	mddi_set_auto_hibernate(&mddi->client_data, 1);

	if (mddi->caps.Mfr_Name == 0 && mddi->caps.Product_Code == 0)
		pdata->fixup(&mddi->caps.Mfr_Name, &mddi->caps.Product_Code);

	mddi->client_pdev.id = 0;
	for (i = 0; i < pdata->num_clients; i++) {
		if (pdata->client_platform_data[i].product_id ==
		    (mddi->caps.Mfr_Name << 16 | mddi->caps.Product_Code)) {
			mddi->client_data.private_client_data =
				pdata->client_platform_data[i].client_data;
			mddi->client_pdev.name =
				pdata->client_platform_data[i].name;
			mddi->client_pdev.id =
				pdata->client_platform_data[i].id;
			/* XXX: possibly set clock */
			break;
		}
	}

	if (i >= pdata->num_clients)
		mddi->client_pdev.name = "mddi_c_dummy";
	printk(KERN_INFO "mddi: registering panel %s\n",
		mddi->client_pdev.name);

	mddi->client_data.suspend = mddi_suspend;
	mddi->client_data.resume = mddi_resume;
	mddi->client_data.activate_link = mddi_activate_link;
	mddi->client_data.remote_write = mddi_remote_write;
	mddi->client_data.remote_read = mddi_remote_read;
	mddi->client_data.auto_hibernate = mddi_set_auto_hibernate;
	mddi->client_data.fb_resource = pdata->fb_resource;
	if (pdev->id == 0)
		mddi->client_data.interface_type = MSM_MDDI_PMDH_INTERFACE;
	else if (pdev->id == 1)
		mddi->client_data.interface_type = MSM_MDDI_EMDH_INTERFACE;
	else {
		printk(KERN_ERR "mddi: can not determine interface %d!\n",
		       pdev->id);
		ret = -EINVAL;
		goto error_mddi_interface;
	}

	mddi->client_pdev.dev.platform_data = &mddi->client_data;
	printk(KERN_INFO "mddi: publish: %s\n", mddi->client_name);
	platform_device_register(&mddi->client_pdev);
	return 0;

error_mddi_interface:
error_mddi_version:
	free_irq(mddi->irq, 0);
error_request_irq:
	dma_free_coherent(NULL, 0x1000, mddi->rev_data, mddi->rev_addr);
error_rev_data:
error_clk_setup:
error_get_irq_resource:
	iounmap(mddi->base);
error_ioremap:

	printk(KERN_INFO "mddi: mddi_init() failed (%d)\n", ret);
	return ret;
}


static struct platform_driver mddi_driver = {
	.probe = mddi_probe,
	.driver = { .name = "msm_mddi" },
};

static int __init _mddi_init(void)
{
	return platform_driver_register(&mddi_driver);
}

module_init(_mddi_init);
>>>>>>> refs/remotes/origin/master
