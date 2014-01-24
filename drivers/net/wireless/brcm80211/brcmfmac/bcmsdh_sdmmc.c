/*
 * Copyright (c) 2010 Broadcom Corporation
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

<<<<<<< HEAD
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

=======
>>>>>>> refs/remotes/origin/master
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/core.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/card.h>
<<<<<<< HEAD
=======
#include <linux/mmc/host.h>
>>>>>>> refs/remotes/origin/master
#include <linux/suspend.h>
#include <linux/errno.h>
#include <linux/sched.h>	/* request_irq() */
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/platform_device.h>
#include <linux/platform_data/brcmfmac-sdio.h>
>>>>>>> refs/remotes/origin/master
#include <net/cfg80211.h>

#include <defs.h>
#include <brcm_hw_ids.h>
#include <brcmu_utils.h>
#include <brcmu_wifi.h>
#include "sdio_host.h"
<<<<<<< HEAD
=======
#include "sdio_chip.h"
>>>>>>> refs/remotes/origin/master
#include "dhd_dbg.h"
#include "dhd_bus.h"

#define SDIO_VENDOR_ID_BROADCOM		0x02d0

#define DMA_ALIGN_MASK	0x03

<<<<<<< HEAD
#define SDIO_DEVICE_ID_BROADCOM_4329	0x4329
#define SDIO_DEVICE_ID_BROADCOM_4330	0x4330

=======
>>>>>>> refs/remotes/origin/master
#define SDIO_FUNC1_BLOCKSIZE		64
#define SDIO_FUNC2_BLOCKSIZE		512

/* devices we support, null terminated */
static const struct sdio_device_id brcmf_sdmmc_ids[] = {
<<<<<<< HEAD
	{SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4329)},
	{SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4330)},
=======
	{SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_43143)},
	{SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_43241)},
	{SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4329)},
	{SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4330)},
	{SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM, SDIO_DEVICE_ID_BROADCOM_4334)},
	{SDIO_DEVICE(SDIO_VENDOR_ID_BROADCOM,
		     SDIO_DEVICE_ID_BROADCOM_4335_4339)},
>>>>>>> refs/remotes/origin/master
	{ /* end: all zeroes */ },
};
MODULE_DEVICE_TABLE(sdio, brcmf_sdmmc_ids);

<<<<<<< HEAD
static bool
=======
static struct brcmfmac_sdio_platform_data *brcmfmac_sdio_pdata;


bool
>>>>>>> refs/remotes/origin/master
brcmf_pm_resume_error(struct brcmf_sdio_dev *sdiodev)
{
	bool is_err = false;
#ifdef CONFIG_PM_SLEEP
	is_err = atomic_read(&sdiodev->suspend);
#endif
	return is_err;
}

<<<<<<< HEAD
static void
=======
void
>>>>>>> refs/remotes/origin/master
brcmf_pm_resume_wait(struct brcmf_sdio_dev *sdiodev, wait_queue_head_t *wq)
{
#ifdef CONFIG_PM_SLEEP
	int retry = 0;
	while (atomic_read(&sdiodev->suspend) && retry++ != 30)
		wait_event_timeout(*wq, false, HZ/100);
#endif
}

static inline int brcmf_sdioh_f0_write_byte(struct brcmf_sdio_dev *sdiodev,
					    uint regaddr, u8 *byte)
{
	struct sdio_func *sdfunc = sdiodev->func[0];
	int err_ret;

	/*
	 * Can only directly write to some F0 registers.
	 * Handle F2 enable/disable and Abort command
	 * as a special case.
	 */
	if (regaddr == SDIO_CCCR_IOEx) {
		sdfunc = sdiodev->func[2];
		if (sdfunc) {
<<<<<<< HEAD
			sdio_claim_host(sdfunc);
=======
>>>>>>> refs/remotes/origin/master
			if (*byte & SDIO_FUNC_ENABLE_2) {
				/* Enable Function 2 */
				err_ret = sdio_enable_func(sdfunc);
				if (err_ret)
<<<<<<< HEAD
					brcmf_dbg(ERROR,
						  "enable F2 failed:%d\n",
=======
					brcmf_err("enable F2 failed:%d\n",
>>>>>>> refs/remotes/origin/master
						  err_ret);
			} else {
				/* Disable Function 2 */
				err_ret = sdio_disable_func(sdfunc);
				if (err_ret)
<<<<<<< HEAD
					brcmf_dbg(ERROR,
						  "Disable F2 failed:%d\n",
						  err_ret);
			}
			sdio_release_host(sdfunc);
		}
	} else if (regaddr == SDIO_CCCR_ABORT) {
=======
					brcmf_err("Disable F2 failed:%d\n",
						  err_ret);
			}
		} else {
			err_ret = -ENOENT;
		}
	} else if ((regaddr == SDIO_CCCR_ABORT) ||
		   (regaddr == SDIO_CCCR_IENx)) {
>>>>>>> refs/remotes/origin/master
		sdfunc = kmemdup(sdiodev->func[0], sizeof(struct sdio_func),
				 GFP_KERNEL);
		if (!sdfunc)
			return -ENOMEM;
		sdfunc->num = 0;
<<<<<<< HEAD
		sdio_claim_host(sdfunc);
		sdio_writeb(sdfunc, *byte, regaddr, &err_ret);
		sdio_release_host(sdfunc);
		kfree(sdfunc);
	} else if (regaddr < 0xF0) {
		brcmf_dbg(ERROR, "F0 Wr:0x%02x: write disallowed\n", regaddr);
		err_ret = -EPERM;
	} else {
		sdio_claim_host(sdfunc);
		sdio_f0_writeb(sdfunc, *byte, regaddr, &err_ret);
		sdio_release_host(sdfunc);
=======
		sdio_writeb(sdfunc, *byte, regaddr, &err_ret);
		kfree(sdfunc);
	} else if (regaddr < 0xF0) {
		brcmf_err("F0 Wr:0x%02x: write disallowed\n", regaddr);
		err_ret = -EPERM;
	} else {
		sdio_f0_writeb(sdfunc, *byte, regaddr, &err_ret);
>>>>>>> refs/remotes/origin/master
	}

	return err_ret;
}

int brcmf_sdioh_request_byte(struct brcmf_sdio_dev *sdiodev, uint rw, uint func,
			     uint regaddr, u8 *byte)
{
	int err_ret;

<<<<<<< HEAD
	brcmf_dbg(INFO, "rw=%d, func=%d, addr=0x%05x\n", rw, func, regaddr);
=======
	brcmf_dbg(SDIO, "rw=%d, func=%d, addr=0x%05x\n", rw, func, regaddr);
>>>>>>> refs/remotes/origin/master

	brcmf_pm_resume_wait(sdiodev, &sdiodev->request_byte_wait);
	if (brcmf_pm_resume_error(sdiodev))
		return -EIO;

	if (rw && func == 0) {
		/* handle F0 separately */
		err_ret = brcmf_sdioh_f0_write_byte(sdiodev, regaddr, byte);
	} else {
<<<<<<< HEAD
		sdio_claim_host(sdiodev->func[func]);
=======
>>>>>>> refs/remotes/origin/master
		if (rw) /* CMD52 Write */
			sdio_writeb(sdiodev->func[func], *byte, regaddr,
				    &err_ret);
		else if (func == 0) {
			*byte = sdio_f0_readb(sdiodev->func[func], regaddr,
					      &err_ret);
		} else {
			*byte = sdio_readb(sdiodev->func[func], regaddr,
					   &err_ret);
		}
<<<<<<< HEAD
		sdio_release_host(sdiodev->func[func]);
	}

	if (err_ret)
		brcmf_dbg(ERROR, "Failed to %s byte F%d:@0x%05x=%02x, Err: %d\n",
=======
	}

	if (err_ret)
		brcmf_err("Failed to %s byte F%d:@0x%05x=%02x, Err: %d\n",
>>>>>>> refs/remotes/origin/master
			  rw ? "write" : "read", func, regaddr, *byte, err_ret);

	return err_ret;
}

int brcmf_sdioh_request_word(struct brcmf_sdio_dev *sdiodev,
			     uint rw, uint func, uint addr, u32 *word,
			     uint nbytes)
{
	int err_ret = -EIO;

	if (func == 0) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "Only CMD52 allowed to F0\n");
		return -EINVAL;
	}

	brcmf_dbg(INFO, "rw=%d, func=%d, addr=0x%05x, nbytes=%d\n",
=======
		brcmf_err("Only CMD52 allowed to F0\n");
		return -EINVAL;
	}

	brcmf_dbg(SDIO, "rw=%d, func=%d, addr=0x%05x, nbytes=%d\n",
>>>>>>> refs/remotes/origin/master
		  rw, func, addr, nbytes);

	brcmf_pm_resume_wait(sdiodev, &sdiodev->request_word_wait);
	if (brcmf_pm_resume_error(sdiodev))
		return -EIO;
<<<<<<< HEAD
	/* Claim host controller */
	sdio_claim_host(sdiodev->func[func]);
=======
>>>>>>> refs/remotes/origin/master

	if (rw) {		/* CMD52 Write */
		if (nbytes == 4)
			sdio_writel(sdiodev->func[func], *word, addr,
				    &err_ret);
		else if (nbytes == 2)
			sdio_writew(sdiodev->func[func], (*word & 0xFFFF),
				    addr, &err_ret);
		else
<<<<<<< HEAD
			brcmf_dbg(ERROR, "Invalid nbytes: %d\n", nbytes);
=======
			brcmf_err("Invalid nbytes: %d\n", nbytes);
>>>>>>> refs/remotes/origin/master
	} else {		/* CMD52 Read */
		if (nbytes == 4)
			*word = sdio_readl(sdiodev->func[func], addr, &err_ret);
		else if (nbytes == 2)
			*word = sdio_readw(sdiodev->func[func], addr,
					   &err_ret) & 0xFFFF;
		else
<<<<<<< HEAD
			brcmf_dbg(ERROR, "Invalid nbytes: %d\n", nbytes);
	}

	/* Release host controller */
	sdio_release_host(sdiodev->func[func]);

	if (err_ret)
		brcmf_dbg(ERROR, "Failed to %s word, Err: 0x%08x\n",
=======
			brcmf_err("Invalid nbytes: %d\n", nbytes);
	}

	if (err_ret)
		brcmf_err("Failed to %s word, Err: 0x%08x\n",
>>>>>>> refs/remotes/origin/master
			  rw ? "write" : "read", err_ret);

	return err_ret;
}

<<<<<<< HEAD
/* precondition: host controller is claimed */
static int
brcmf_sdioh_request_data(struct brcmf_sdio_dev *sdiodev, uint write, bool fifo,
			 uint func, uint addr, struct sk_buff *pkt, uint pktlen)
{
	int err_ret = 0;

	if ((write) && (!fifo)) {
		err_ret = sdio_memcpy_toio(sdiodev->func[func], addr,
					   ((u8 *) (pkt->data)), pktlen);
	} else if (write) {
		err_ret = sdio_memcpy_toio(sdiodev->func[func], addr,
					   ((u8 *) (pkt->data)), pktlen);
	} else if (fifo) {
		err_ret = sdio_readsb(sdiodev->func[func],
				      ((u8 *) (pkt->data)), addr, pktlen);
	} else {
		err_ret = sdio_memcpy_fromio(sdiodev->func[func],
					     ((u8 *) (pkt->data)),
					     addr, pktlen);
	}

	return err_ret;
}

/*
 * This function takes a queue of packets. The packets on the queue
 * are assumed to be properly aligned by the caller.
 */
int
brcmf_sdioh_request_chain(struct brcmf_sdio_dev *sdiodev, uint fix_inc,
			  uint write, uint func, uint addr,
			  struct sk_buff_head *pktq)
{
	bool fifo = (fix_inc == SDIOH_DATA_FIX);
	u32 SGCount = 0;
	int err_ret = 0;

	struct sk_buff *pkt;

	brcmf_dbg(TRACE, "Enter\n");

	brcmf_pm_resume_wait(sdiodev, &sdiodev->request_chain_wait);
	if (brcmf_pm_resume_error(sdiodev))
		return -EIO;

	/* Claim host controller */
	sdio_claim_host(sdiodev->func[func]);

	skb_queue_walk(pktq, pkt) {
		uint pkt_len = pkt->len;
		pkt_len += 3;
		pkt_len &= 0xFFFFFFFC;

		err_ret = brcmf_sdioh_request_data(sdiodev, write, fifo, func,
						   addr, pkt, pkt_len);
		if (err_ret) {
			brcmf_dbg(ERROR, "%s FAILED %p[%d], addr=0x%05x, pkt_len=%d, ERR=0x%08x\n",
				  write ? "TX" : "RX", pkt, SGCount, addr,
				  pkt_len, err_ret);
		} else {
			brcmf_dbg(TRACE, "%s xfr'd %p[%d], addr=0x%05x, len=%d\n",
				  write ? "TX" : "RX", pkt, SGCount, addr,
				  pkt_len);
		}
		if (!fifo)
			addr += pkt_len;

		SGCount++;
	}

	/* Release host controller */
	sdio_release_host(sdiodev->func[func]);

	brcmf_dbg(TRACE, "Exit\n");
	return err_ret;
}

/*
 * This function takes a single DMA-able packet.
 */
int brcmf_sdioh_request_buffer(struct brcmf_sdio_dev *sdiodev,
			       uint fix_inc, uint write, uint func, uint addr,
			       struct sk_buff *pkt)
{
	int status;
	uint pkt_len;
	bool fifo = (fix_inc == SDIOH_DATA_FIX);

	brcmf_dbg(TRACE, "Enter\n");

	if (pkt == NULL)
		return -EINVAL;
	pkt_len = pkt->len;

	brcmf_pm_resume_wait(sdiodev, &sdiodev->request_buffer_wait);
	if (brcmf_pm_resume_error(sdiodev))
		return -EIO;

	/* Claim host controller */
	sdio_claim_host(sdiodev->func[func]);

	pkt_len += 3;
	pkt_len &= (uint)~3;

	status = brcmf_sdioh_request_data(sdiodev, write, fifo, func,
					   addr, pkt, pkt_len);
	if (status) {
		brcmf_dbg(ERROR, "%s FAILED %p, addr=0x%05x, pkt_len=%d, ERR=0x%08x\n",
			  write ? "TX" : "RX", pkt, addr, pkt_len, status);
	} else {
		brcmf_dbg(TRACE, "%s xfr'd %p, addr=0x%05x, len=%d\n",
			  write ? "TX" : "RX", pkt, addr, pkt_len);
	}

	/* Release host controller */
	sdio_release_host(sdiodev->func[func]);

	return status;
}

/* Read client card reg */
static int
brcmf_sdioh_card_regread(struct brcmf_sdio_dev *sdiodev, int func, u32 regaddr,
			 int regsize, u32 *data)
{

	if ((func == 0) || (regsize == 1)) {
		u8 temp = 0;

		brcmf_sdioh_request_byte(sdiodev, SDIOH_READ, func, regaddr,
					 &temp);
		*data = temp;
		*data &= 0xff;
		brcmf_dbg(DATA, "byte read data=0x%02x\n", *data);
	} else {
		brcmf_sdioh_request_word(sdiodev, SDIOH_READ, func, regaddr,
					 data, regsize);
		if (regsize == 2)
			*data &= 0xffff;

		brcmf_dbg(DATA, "word read data=0x%08x\n", *data);
	}

	return SUCCESS;
}

static int brcmf_sdioh_get_cisaddr(struct brcmf_sdio_dev *sdiodev, u32 regaddr)
{
	/* read 24 bits and return valid 17 bit addr */
	int i;
=======
static int brcmf_sdioh_get_cisaddr(struct brcmf_sdio_dev *sdiodev, u32 regaddr)
{
	/* read 24 bits and return valid 17 bit addr */
	int i, ret;
>>>>>>> refs/remotes/origin/master
	u32 scratch, regdata;
	__le32 scratch_le;
	u8 *ptr = (u8 *)&scratch_le;

	for (i = 0; i < 3; i++) {
<<<<<<< HEAD
		if ((brcmf_sdioh_card_regread(sdiodev, 0, regaddr, 1,
				&regdata)) != SUCCESS)
			brcmf_dbg(ERROR, "Can't read!\n");
=======
		regdata = brcmf_sdio_regrl(sdiodev, regaddr, &ret);
		if (ret != 0)
			brcmf_err("Can't read!\n");
>>>>>>> refs/remotes/origin/master

		*ptr++ = (u8) regdata;
		regaddr++;
	}

	/* Only the lower 17-bits are valid */
	scratch = le32_to_cpu(scratch_le);
	scratch &= 0x0001FFFF;
	return scratch;
}

static int brcmf_sdioh_enablefuncs(struct brcmf_sdio_dev *sdiodev)
{
	int err_ret;
	u32 fbraddr;
	u8 func;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "\n");
=======
	brcmf_dbg(SDIO, "\n");
>>>>>>> refs/remotes/origin/master

	/* Get the Card's common CIS address */
	sdiodev->func_cis_ptr[0] = brcmf_sdioh_get_cisaddr(sdiodev,
							   SDIO_CCCR_CIS);
<<<<<<< HEAD
	brcmf_dbg(INFO, "Card's Common CIS Ptr = 0x%x\n",
=======
	brcmf_dbg(SDIO, "Card's Common CIS Ptr = 0x%x\n",
>>>>>>> refs/remotes/origin/master
		  sdiodev->func_cis_ptr[0]);

	/* Get the Card's function CIS (for each function) */
	for (fbraddr = SDIO_FBR_BASE(1), func = 1;
	     func <= sdiodev->num_funcs; func++, fbraddr += SDIOD_FBR_SIZE) {
		sdiodev->func_cis_ptr[func] =
		    brcmf_sdioh_get_cisaddr(sdiodev, SDIO_FBR_CIS + fbraddr);
<<<<<<< HEAD
		brcmf_dbg(INFO, "Function %d CIS Ptr = 0x%x\n",
=======
		brcmf_dbg(SDIO, "Function %d CIS Ptr = 0x%x\n",
>>>>>>> refs/remotes/origin/master
			  func, sdiodev->func_cis_ptr[func]);
	}

	/* Enable Function 1 */
<<<<<<< HEAD
	sdio_claim_host(sdiodev->func[1]);
	err_ret = sdio_enable_func(sdiodev->func[1]);
	sdio_release_host(sdiodev->func[1]);
	if (err_ret)
		brcmf_dbg(ERROR, "Failed to enable F1 Err: 0x%08x\n", err_ret);
=======
	err_ret = sdio_enable_func(sdiodev->func[1]);
	if (err_ret)
		brcmf_err("Failed to enable F1 Err: 0x%08x\n", err_ret);
>>>>>>> refs/remotes/origin/master

	return false;
}

/*
 *	Public entry points & extern's
 */
int brcmf_sdioh_attach(struct brcmf_sdio_dev *sdiodev)
{
	int err_ret = 0;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "\n");
=======
	brcmf_dbg(SDIO, "\n");
>>>>>>> refs/remotes/origin/master

	sdiodev->num_funcs = 2;

	sdio_claim_host(sdiodev->func[1]);
<<<<<<< HEAD
	err_ret = sdio_set_block_size(sdiodev->func[1], SDIO_FUNC1_BLOCKSIZE);
	sdio_release_host(sdiodev->func[1]);
	if (err_ret) {
		brcmf_dbg(ERROR, "Failed to set F1 blocksize\n");
		goto out;
	}

	sdio_claim_host(sdiodev->func[2]);
	err_ret = sdio_set_block_size(sdiodev->func[2], SDIO_FUNC2_BLOCKSIZE);
	sdio_release_host(sdiodev->func[2]);
	if (err_ret) {
		brcmf_dbg(ERROR, "Failed to set F2 blocksize\n");
=======

	err_ret = sdio_set_block_size(sdiodev->func[1], SDIO_FUNC1_BLOCKSIZE);
	if (err_ret) {
		brcmf_err("Failed to set F1 blocksize\n");
		goto out;
	}

	err_ret = sdio_set_block_size(sdiodev->func[2], SDIO_FUNC2_BLOCKSIZE);
	if (err_ret) {
		brcmf_err("Failed to set F2 blocksize\n");
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	brcmf_sdioh_enablefuncs(sdiodev);

out:
<<<<<<< HEAD
	brcmf_dbg(TRACE, "Done\n");
=======
	sdio_release_host(sdiodev->func[1]);
	brcmf_dbg(SDIO, "Done\n");
>>>>>>> refs/remotes/origin/master
	return err_ret;
}

void brcmf_sdioh_detach(struct brcmf_sdio_dev *sdiodev)
{
<<<<<<< HEAD
	brcmf_dbg(TRACE, "\n");
=======
	brcmf_dbg(SDIO, "\n");
>>>>>>> refs/remotes/origin/master

	/* Disable Function 2 */
	sdio_claim_host(sdiodev->func[2]);
	sdio_disable_func(sdiodev->func[2]);
	sdio_release_host(sdiodev->func[2]);

	/* Disable Function 1 */
	sdio_claim_host(sdiodev->func[1]);
	sdio_disable_func(sdiodev->func[1]);
	sdio_release_host(sdiodev->func[1]);

}

static int brcmf_ops_sdio_probe(struct sdio_func *func,
<<<<<<< HEAD
			      const struct sdio_device_id *id)
{
	int ret = 0;
	struct brcmf_sdio_dev *sdiodev;
	struct brcmf_bus *bus_if;
	brcmf_dbg(TRACE, "Enter\n");
	brcmf_dbg(TRACE, "func->class=%x\n", func->class);
	brcmf_dbg(TRACE, "sdio_vendor: 0x%04x\n", func->vendor);
	brcmf_dbg(TRACE, "sdio_device: 0x%04x\n", func->device);
	brcmf_dbg(TRACE, "Function#: 0x%04x\n", func->num);

	if (func->num == 1) {
		if (dev_get_drvdata(&func->card->dev)) {
			brcmf_dbg(ERROR, "card private drvdata occupied\n");
			return -ENXIO;
		}
		bus_if = kzalloc(sizeof(struct brcmf_bus), GFP_KERNEL);
		if (!bus_if)
			return -ENOMEM;
		sdiodev = kzalloc(sizeof(struct brcmf_sdio_dev), GFP_KERNEL);
		if (!sdiodev) {
			kfree(bus_if);
			return -ENOMEM;
		}
		sdiodev->func[0] = func;
		sdiodev->func[1] = func;
		sdiodev->bus_if = bus_if;
		bus_if->bus_priv.sdio = sdiodev;
		bus_if->type = SDIO_BUS;
		bus_if->align = BRCMF_SDALIGN;
		dev_set_drvdata(&func->card->dev, sdiodev);

		atomic_set(&sdiodev->suspend, false);
		init_waitqueue_head(&sdiodev->request_byte_wait);
		init_waitqueue_head(&sdiodev->request_word_wait);
		init_waitqueue_head(&sdiodev->request_chain_wait);
		init_waitqueue_head(&sdiodev->request_buffer_wait);
	}

	if (func->num == 2) {
		sdiodev = dev_get_drvdata(&func->card->dev);
		if ((!sdiodev) || (sdiodev->func[1]->card != func->card))
			return -ENODEV;
		sdiodev->func[2] = func;

		bus_if = sdiodev->bus_if;
		sdiodev->dev = &func->dev;
		dev_set_drvdata(&func->dev, bus_if);

		brcmf_dbg(TRACE, "F2 found, calling brcmf_sdio_probe...\n");
		ret = brcmf_sdio_probe(sdiodev);
	}

	return ret;
=======
				const struct sdio_device_id *id)
{
	int err;
	struct brcmf_sdio_dev *sdiodev;
	struct brcmf_bus *bus_if;
	struct mmc_host *host;
	uint max_blocks;

	brcmf_dbg(SDIO, "Enter\n");
	brcmf_dbg(SDIO, "Class=%x\n", func->class);
	brcmf_dbg(SDIO, "sdio vendor ID: 0x%04x\n", func->vendor);
	brcmf_dbg(SDIO, "sdio device ID: 0x%04x\n", func->device);
	brcmf_dbg(SDIO, "Function#: %d\n", func->num);

	/* Consume func num 1 but dont do anything with it. */
	if (func->num == 1)
		return 0;

	/* Ignore anything but func 2 */
	if (func->num != 2)
		return -ENODEV;

	bus_if = kzalloc(sizeof(struct brcmf_bus), GFP_KERNEL);
	if (!bus_if)
		return -ENOMEM;
	sdiodev = kzalloc(sizeof(struct brcmf_sdio_dev), GFP_KERNEL);
	if (!sdiodev) {
		kfree(bus_if);
		return -ENOMEM;
	}

	sdiodev->func[0] = func->card->sdio_func[0];
	sdiodev->func[1] = func->card->sdio_func[0];
	sdiodev->func[2] = func;

	sdiodev->bus_if = bus_if;
	bus_if->bus_priv.sdio = sdiodev;
	dev_set_drvdata(&func->dev, bus_if);
	dev_set_drvdata(&sdiodev->func[1]->dev, bus_if);
	sdiodev->dev = &sdiodev->func[1]->dev;
	sdiodev->pdata = brcmfmac_sdio_pdata;

	atomic_set(&sdiodev->suspend, false);
	init_waitqueue_head(&sdiodev->request_byte_wait);
	init_waitqueue_head(&sdiodev->request_word_wait);
	init_waitqueue_head(&sdiodev->request_buffer_wait);

	brcmf_dbg(SDIO, "F2 found, calling brcmf_sdio_probe...\n");
	err = brcmf_sdio_probe(sdiodev);
	if (err) {
		brcmf_err("F2 error, probe failed %d...\n", err);
		goto fail;
	}

	/*
	 * determine host related variables after brcmf_sdio_probe()
	 * as func->cur_blksize is properly set and F2 init has been
	 * completed successfully.
	 */
	host = func->card->host;
	sdiodev->sg_support = host->max_segs > 1;
	max_blocks = min_t(uint, host->max_blk_count, 511u);
	sdiodev->max_request_size = min_t(uint, host->max_req_size,
					  max_blocks * func->cur_blksize);
	sdiodev->max_segment_count = min_t(uint, host->max_segs,
					   SG_MAX_SINGLE_ALLOC);
	sdiodev->max_segment_size = host->max_seg_size;
	brcmf_dbg(SDIO, "F2 init completed...\n");
	return 0;

fail:
	dev_set_drvdata(&func->dev, NULL);
	dev_set_drvdata(&sdiodev->func[1]->dev, NULL);
	kfree(sdiodev);
	kfree(bus_if);
	return err;
>>>>>>> refs/remotes/origin/master
}

static void brcmf_ops_sdio_remove(struct sdio_func *func)
{
	struct brcmf_bus *bus_if;
	struct brcmf_sdio_dev *sdiodev;
<<<<<<< HEAD
	brcmf_dbg(TRACE, "Enter\n");
	brcmf_dbg(INFO, "func->class=%x\n", func->class);
	brcmf_dbg(INFO, "sdio_vendor: 0x%04x\n", func->vendor);
	brcmf_dbg(INFO, "sdio_device: 0x%04x\n", func->device);
	brcmf_dbg(INFO, "Function#: 0x%04x\n", func->num);

	if (func->num == 2) {
		bus_if = dev_get_drvdata(&func->dev);
		sdiodev = bus_if->bus_priv.sdio;
		brcmf_dbg(TRACE, "F2 found, calling brcmf_sdio_remove...\n");
		brcmf_sdio_remove(sdiodev);
		dev_set_drvdata(&func->card->dev, NULL);
		dev_set_drvdata(&func->dev, NULL);
		kfree(bus_if);
		kfree(sdiodev);
	}
=======

	brcmf_dbg(SDIO, "Enter\n");
	brcmf_dbg(SDIO, "sdio vendor ID: 0x%04x\n", func->vendor);
	brcmf_dbg(SDIO, "sdio device ID: 0x%04x\n", func->device);
	brcmf_dbg(SDIO, "Function: %d\n", func->num);

	if (func->num != 1 && func->num != 2)
		return;

	bus_if = dev_get_drvdata(&func->dev);
	if (bus_if) {
		sdiodev = bus_if->bus_priv.sdio;
		brcmf_sdio_remove(sdiodev);

		dev_set_drvdata(&sdiodev->func[1]->dev, NULL);
		dev_set_drvdata(&sdiodev->func[2]->dev, NULL);

		kfree(bus_if);
		kfree(sdiodev);
	}

	brcmf_dbg(SDIO, "Exit\n");
>>>>>>> refs/remotes/origin/master
}

#ifdef CONFIG_PM_SLEEP
static int brcmf_sdio_suspend(struct device *dev)
{
	mmc_pm_flag_t sdio_flags;
<<<<<<< HEAD
	struct sdio_func *func = dev_to_sdio_func(dev);
	struct brcmf_sdio_dev *sdiodev = dev_get_drvdata(&func->card->dev);
	int ret = 0;

	brcmf_dbg(TRACE, "\n");
=======
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_sdio_dev *sdiodev = bus_if->bus_priv.sdio;
	int ret = 0;

	brcmf_dbg(SDIO, "\n");
>>>>>>> refs/remotes/origin/master

	atomic_set(&sdiodev->suspend, true);

	sdio_flags = sdio_get_host_pm_caps(sdiodev->func[1]);
	if (!(sdio_flags & MMC_PM_KEEP_POWER)) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "Host can't keep power while suspended\n");
=======
		brcmf_err("Host can't keep power while suspended\n");
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	ret = sdio_set_host_pm_flags(sdiodev->func[1], MMC_PM_KEEP_POWER);
	if (ret) {
<<<<<<< HEAD
		brcmf_dbg(ERROR, "Failed to set pm_flags\n");
=======
		brcmf_err("Failed to set pm_flags\n");
>>>>>>> refs/remotes/origin/master
		return ret;
	}

	brcmf_sdio_wdtmr_enable(sdiodev, false);

	return ret;
}

static int brcmf_sdio_resume(struct device *dev)
{
<<<<<<< HEAD
	struct sdio_func *func = dev_to_sdio_func(dev);
	struct brcmf_sdio_dev *sdiodev = dev_get_drvdata(&func->card->dev);
=======
	struct brcmf_bus *bus_if = dev_get_drvdata(dev);
	struct brcmf_sdio_dev *sdiodev = bus_if->bus_priv.sdio;
>>>>>>> refs/remotes/origin/master

	brcmf_sdio_wdtmr_enable(sdiodev, true);
	atomic_set(&sdiodev->suspend, false);
	return 0;
}

static const struct dev_pm_ops brcmf_sdio_pm_ops = {
	.suspend	= brcmf_sdio_suspend,
	.resume		= brcmf_sdio_resume,
};
#endif	/* CONFIG_PM_SLEEP */

static struct sdio_driver brcmf_sdmmc_driver = {
	.probe = brcmf_ops_sdio_probe,
	.remove = brcmf_ops_sdio_remove,
<<<<<<< HEAD
	.name = "brcmfmac",
=======
	.name = BRCMFMAC_SDIO_PDATA_NAME,
>>>>>>> refs/remotes/origin/master
	.id_table = brcmf_sdmmc_ids,
#ifdef CONFIG_PM_SLEEP
	.drv = {
		.pm = &brcmf_sdio_pm_ops,
	},
#endif	/* CONFIG_PM_SLEEP */
};

<<<<<<< HEAD
void brcmf_sdio_exit(void)
{
	brcmf_dbg(TRACE, "Enter\n");

	sdio_unregister_driver(&brcmf_sdmmc_driver);
}

void brcmf_sdio_init(void)
{
	int ret;

	brcmf_dbg(TRACE, "Enter\n");

	ret = sdio_register_driver(&brcmf_sdmmc_driver);

	if (ret)
		brcmf_dbg(ERROR, "sdio_register_driver failed: %d\n", ret);
=======
static int brcmf_sdio_pd_probe(struct platform_device *pdev)
{
	brcmf_dbg(SDIO, "Enter\n");

	brcmfmac_sdio_pdata = dev_get_platdata(&pdev->dev);

	if (brcmfmac_sdio_pdata->power_on)
		brcmfmac_sdio_pdata->power_on();

	return 0;
}

static int brcmf_sdio_pd_remove(struct platform_device *pdev)
{
	brcmf_dbg(SDIO, "Enter\n");

	if (brcmfmac_sdio_pdata->power_off)
		brcmfmac_sdio_pdata->power_off();

	sdio_unregister_driver(&brcmf_sdmmc_driver);

	return 0;
}

static struct platform_driver brcmf_sdio_pd = {
	.remove		= brcmf_sdio_pd_remove,
	.driver		= {
		.name	= BRCMFMAC_SDIO_PDATA_NAME,
		.owner	= THIS_MODULE,
	}
};

void brcmf_sdio_register(void)
{
	int ret;

	ret = sdio_register_driver(&brcmf_sdmmc_driver);
	if (ret)
		brcmf_err("sdio_register_driver failed: %d\n", ret);
}

void brcmf_sdio_exit(void)
{
	brcmf_dbg(SDIO, "Enter\n");

	if (brcmfmac_sdio_pdata)
		platform_driver_unregister(&brcmf_sdio_pd);
	else
		sdio_unregister_driver(&brcmf_sdmmc_driver);
}

void __init brcmf_sdio_init(void)
{
	int ret;

	brcmf_dbg(SDIO, "Enter\n");

	ret = platform_driver_probe(&brcmf_sdio_pd, brcmf_sdio_pd_probe);
	if (ret == -ENODEV)
		brcmf_dbg(SDIO, "No platform data available.\n");
>>>>>>> refs/remotes/origin/master
}
