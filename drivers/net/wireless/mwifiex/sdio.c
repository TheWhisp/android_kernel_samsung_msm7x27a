/*
 * Marvell Wireless LAN device driver: SDIO specific handling
 *
 * Copyright (C) 2011, Marvell International Ltd.
 *
 * This software file (the "File") is distributed by Marvell International
 * Ltd. under the terms of the GNU General Public License Version 2, June 1991
 * (the "License").  You may use, redistribute and/or modify this File in
 * accordance with the terms and conditions of the License, a copy of which
 * is available by writing to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
 * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
 * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
 * this warranty disclaimer.
 */

#include <linux/firmware.h>

#include "decl.h"
#include "ioctl.h"
#include "util.h"
#include "fw.h"
#include "main.h"
#include "wmm.h"
#include "11n.h"
#include "sdio.h"


#define SDIO_VERSION	"1.0"

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/* The mwifiex_sdio_remove() callback function is called when
 * user removes this module from kernel space or ejects
 * the card from the slot. The driver handles these 2 cases
 * differently.
 * If the user is removing the module, the few commands (FUNC_SHUTDOWN,
 * HS_CANCEL etc.) are sent to the firmware.
 * If the card is removed, there is no need to send these command.
 *
 * The variable 'user_rmmod' is used to distinguish these two
 * scenarios. This flag is initialized as FALSE in case the card
 * is removed, and will be set to TRUE for module removal when
 * module_exit function is called.
 */
static u8 user_rmmod;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct mwifiex_if_ops sdio_ops;

static struct semaphore add_remove_card_sem;

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int mwifiex_sdio_resume(struct device *dev);

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * SDIO probe.
 *
 * This function probes an mwifiex device and registers it. It allocates
 * the card structure, enables SDIO function number and initiates the
 * device registration and initialization procedure by adding a logical
 * interface.
 */
static int
mwifiex_sdio_probe(struct sdio_func *func, const struct sdio_device_id *id)
{
	int ret;
	struct sdio_mmc_card *card = NULL;

	pr_debug("info: vendor=0x%4.04X device=0x%4.04X class=%d function=%d\n",
<<<<<<< HEAD
<<<<<<< HEAD
	       func->vendor, func->device, func->class, func->num);

	card = kzalloc(sizeof(struct sdio_mmc_card), GFP_KERNEL);
	if (!card) {
		pr_err("%s: failed to alloc memory\n", __func__);
		return -ENOMEM;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
		 func->vendor, func->device, func->class, func->num);

	card = kzalloc(sizeof(struct sdio_mmc_card), GFP_KERNEL);
	if (!card)
		return -ENOMEM;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	card->func = func;

	func->card->quirks |= MMC_QUIRK_BLKSZ_FOR_BYTE_MODE;

<<<<<<< HEAD
=======
	if (id->driver_data) {
		struct mwifiex_sdio_device *data = (void *)id->driver_data;

		card->firmware = data->firmware;
		card->reg = data->reg;
		card->max_ports = data->max_ports;
		card->mp_agg_pkt_limit = data->mp_agg_pkt_limit;
		card->supports_sdio_new_mode = data->supports_sdio_new_mode;
		card->has_control_mask = data->has_control_mask;
	}

>>>>>>> refs/remotes/origin/master
	sdio_claim_host(func);
	ret = sdio_enable_func(func);
	sdio_release_host(func);

	if (ret) {
		pr_err("%s: failed to enable function\n", __func__);
		kfree(card);
		return -EIO;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (mwifiex_add_card(card, &add_remove_card_sem, &sdio_ops)) {
=======
	if (mwifiex_add_card(card, &add_remove_card_sem, &sdio_ops,
			     MWIFIEX_SDIO)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (mwifiex_add_card(card, &add_remove_card_sem, &sdio_ops,
			     MWIFIEX_SDIO)) {
>>>>>>> refs/remotes/origin/master
		pr_err("%s: add card failed\n", __func__);
		kfree(card);
		sdio_claim_host(func);
		ret = sdio_disable_func(func);
		sdio_release_host(func);
		ret = -1;
	}

	return ret;
}

/*
<<<<<<< HEAD
 * SDIO remove.
 *
 * This function removes the interface and frees up the card structure.
 */
static void
mwifiex_sdio_remove(struct sdio_func *func)
{
	struct sdio_mmc_card *card;
<<<<<<< HEAD

	pr_debug("info: SDIO func num=%d\n", func->num);

	if (func) {
		card = sdio_get_drvdata(func);
		if (card) {
			mwifiex_remove_card(card->adapter,
					&add_remove_card_sem);
			kfree(card);
		}
	}
=======
=======
 * SDIO resume.
 *
 * Kernel needs to suspend all functions separately. Therefore all
 * registered functions must have drivers with suspend and resume
 * methods. Failing that the kernel simply removes the whole card.
 *
 * If already not resumed, this function turns on the traffic and
 * sends a host sleep cancel request to the firmware.
 */
static int mwifiex_sdio_resume(struct device *dev)
{
	struct sdio_func *func = dev_to_sdio_func(dev);
	struct sdio_mmc_card *card;
	struct mwifiex_adapter *adapter;
	mmc_pm_flag_t pm_flag = 0;

	if (func) {
		pm_flag = sdio_get_host_pm_caps(func);
		card = sdio_get_drvdata(func);
		if (!card || !card->adapter) {
			pr_err("resume: invalid card or adapter\n");
			return 0;
		}
	} else {
		pr_err("resume: sdio_func is not specified\n");
		return 0;
	}

	adapter = card->adapter;

	if (!adapter->is_suspended) {
		dev_warn(adapter->dev, "device already resumed\n");
		return 0;
	}

	adapter->is_suspended = false;

	/* Disable Host Sleep */
	mwifiex_cancel_hs(mwifiex_get_priv(adapter, MWIFIEX_BSS_ROLE_STA),
			  MWIFIEX_ASYNC_CMD);

	return 0;
}

/*
 * SDIO remove.
 *
 * This function removes the interface and frees up the card structure.
 */
static void
mwifiex_sdio_remove(struct sdio_func *func)
{
	struct sdio_mmc_card *card;
>>>>>>> refs/remotes/origin/master
	struct mwifiex_adapter *adapter;
	struct mwifiex_private *priv;
	int i;

	pr_debug("info: SDIO func num=%d\n", func->num);

	card = sdio_get_drvdata(func);
	if (!card)
		return;

	adapter = card->adapter;
	if (!adapter || !adapter->priv_num)
		return;

<<<<<<< HEAD
=======
	/* In case driver is removed when asynchronous FW load is in progress */
	wait_for_completion(&adapter->fw_load);

>>>>>>> refs/remotes/origin/master
	if (user_rmmod) {
		if (adapter->is_suspended)
			mwifiex_sdio_resume(adapter->dev);

		for (i = 0; i < adapter->priv_num; i++)
			if ((GET_BSS_ROLE(adapter->priv[i]) ==
						MWIFIEX_BSS_ROLE_STA) &&
			    adapter->priv[i]->media_connected)
				mwifiex_deauthenticate(adapter->priv[i], NULL);

		priv = mwifiex_get_priv(adapter, MWIFIEX_BSS_ROLE_ANY);
		mwifiex_disable_auto_ds(priv);
		mwifiex_init_shutdown_fw(priv, MWIFIEX_FUNC_SHUTDOWN);
	}

	mwifiex_remove_card(card->adapter, &add_remove_card_sem);
<<<<<<< HEAD
	kfree(card);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * SDIO suspend.
 *
 * Kernel needs to suspend all functions separately. Therefore all
 * registered functions must have drivers with suspend and resume
 * methods. Failing that the kernel simply removes the whole card.
 *
 * If already not suspended, this function allocates and sends a host
 * sleep activate request to the firmware and turns off the traffic.
 */
static int mwifiex_sdio_suspend(struct device *dev)
{
	struct sdio_func *func = dev_to_sdio_func(dev);
	struct sdio_mmc_card *card;
	struct mwifiex_adapter *adapter;
	mmc_pm_flag_t pm_flag = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	int i;
=======
>>>>>>> refs/remotes/origin/master
	int ret = 0;

	if (func) {
		pm_flag = sdio_get_host_pm_caps(func);
		pr_debug("cmd: %s: suspend: PM flag = 0x%x\n",
<<<<<<< HEAD
<<<<<<< HEAD
		       sdio_func_id(func), pm_flag);
=======
			 sdio_func_id(func), pm_flag);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			 sdio_func_id(func), pm_flag);
>>>>>>> refs/remotes/origin/master
		if (!(pm_flag & MMC_PM_KEEP_POWER)) {
			pr_err("%s: cannot remain alive while host is"
				" suspended\n", sdio_func_id(func));
			return -ENOSYS;
		}

		card = sdio_get_drvdata(func);
		if (!card || !card->adapter) {
			pr_err("suspend: invalid card or adapter\n");
			return 0;
		}
	} else {
		pr_err("suspend: sdio_func is not specified\n");
		return 0;
	}

	adapter = card->adapter;

	/* Enable the Host Sleep */
	if (!mwifiex_enable_hs(adapter)) {
		dev_err(adapter->dev, "cmd: failed to suspend\n");
		return -EFAULT;
	}

	dev_dbg(adapter->dev, "cmd: suspend with MMC_PM_KEEP_POWER\n");
	ret = sdio_set_host_pm_flags(func, MMC_PM_KEEP_POWER);

	/* Indicate device suspended */
	adapter->is_suspended = true;

<<<<<<< HEAD
	for (i = 0; i < adapter->priv_num; i++)
		netif_carrier_off(adapter->priv[i]->netdev);

	return ret;
}

/*
 * SDIO resume.
 *
 * Kernel needs to suspend all functions separately. Therefore all
 * registered functions must have drivers with suspend and resume
 * methods. Failing that the kernel simply removes the whole card.
 *
 * If already not resumed, this function turns on the traffic and
 * sends a host sleep cancel request to the firmware.
 */
static int mwifiex_sdio_resume(struct device *dev)
{
	struct sdio_func *func = dev_to_sdio_func(dev);
	struct sdio_mmc_card *card;
	struct mwifiex_adapter *adapter;
	mmc_pm_flag_t pm_flag = 0;
	int i;

	if (func) {
		pm_flag = sdio_get_host_pm_caps(func);
		card = sdio_get_drvdata(func);
		if (!card || !card->adapter) {
			pr_err("resume: invalid card or adapter\n");
			return 0;
		}
	} else {
		pr_err("resume: sdio_func is not specified\n");
		return 0;
	}

	adapter = card->adapter;

	if (!adapter->is_suspended) {
		dev_warn(adapter->dev, "device already resumed\n");
		return 0;
	}

	adapter->is_suspended = false;

	for (i = 0; i < adapter->priv_num; i++)
		if (adapter->priv[i]->media_connected)
			netif_carrier_on(adapter->priv[i]->netdev);

	/* Disable Host Sleep */
	mwifiex_cancel_hs(mwifiex_get_priv(adapter, MWIFIEX_BSS_ROLE_STA),
			  MWIFIEX_ASYNC_CMD);

	return 0;
}

/* Device ID for SD8787 */
#define SDIO_DEVICE_ID_MARVELL_8787   (0x9119)
<<<<<<< HEAD
=======
/* Device ID for SD8797 */
#define SDIO_DEVICE_ID_MARVELL_8797   (0x9129)
>>>>>>> refs/remotes/origin/cm-10.0

/* WLAN IDs */
static const struct sdio_device_id mwifiex_ids[] = {
	{SDIO_DEVICE(SDIO_VENDOR_ID_MARVELL, SDIO_DEVICE_ID_MARVELL_8787)},
<<<<<<< HEAD
=======
	{SDIO_DEVICE(SDIO_VENDOR_ID_MARVELL, SDIO_DEVICE_ID_MARVELL_8797)},
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return ret;
}

/* Device ID for SD8786 */
#define SDIO_DEVICE_ID_MARVELL_8786   (0x9116)
/* Device ID for SD8787 */
#define SDIO_DEVICE_ID_MARVELL_8787   (0x9119)
/* Device ID for SD8797 */
#define SDIO_DEVICE_ID_MARVELL_8797   (0x9129)
/* Device ID for SD8897 */
#define SDIO_DEVICE_ID_MARVELL_8897   (0x912d)

/* WLAN IDs */
static const struct sdio_device_id mwifiex_ids[] = {
	{SDIO_DEVICE(SDIO_VENDOR_ID_MARVELL, SDIO_DEVICE_ID_MARVELL_8786),
		.driver_data = (unsigned long) &mwifiex_sdio_sd8786},
	{SDIO_DEVICE(SDIO_VENDOR_ID_MARVELL, SDIO_DEVICE_ID_MARVELL_8787),
		.driver_data = (unsigned long) &mwifiex_sdio_sd8787},
	{SDIO_DEVICE(SDIO_VENDOR_ID_MARVELL, SDIO_DEVICE_ID_MARVELL_8797),
		.driver_data = (unsigned long) &mwifiex_sdio_sd8797},
	{SDIO_DEVICE(SDIO_VENDOR_ID_MARVELL, SDIO_DEVICE_ID_MARVELL_8897),
		.driver_data = (unsigned long) &mwifiex_sdio_sd8897},
>>>>>>> refs/remotes/origin/master
	{},
};

MODULE_DEVICE_TABLE(sdio, mwifiex_ids);

static const struct dev_pm_ops mwifiex_sdio_pm_ops = {
	.suspend = mwifiex_sdio_suspend,
	.resume = mwifiex_sdio_resume,
};

static struct sdio_driver mwifiex_sdio = {
	.name = "mwifiex_sdio",
	.id_table = mwifiex_ids,
	.probe = mwifiex_sdio_probe,
	.remove = mwifiex_sdio_remove,
	.drv = {
		.owner = THIS_MODULE,
		.pm = &mwifiex_sdio_pm_ops,
	}
};

<<<<<<< HEAD
=======
/* Write data into SDIO card register. Caller claims SDIO device. */
static int
mwifiex_write_reg_locked(struct sdio_func *func, u32 reg, u8 data)
{
	int ret = -1;
	sdio_writeb(func, data, reg, &ret);
	return ret;
}

>>>>>>> refs/remotes/origin/master
/*
 * This function writes data into SDIO card register.
 */
static int
<<<<<<< HEAD
mwifiex_write_reg(struct mwifiex_adapter *adapter, u32 reg, u32 data)
{
	struct sdio_mmc_card *card = adapter->card;
	int ret = -1;

	sdio_claim_host(card->func);
	sdio_writeb(card->func, (u8) data, reg, &ret);
=======
mwifiex_write_reg(struct mwifiex_adapter *adapter, u32 reg, u8 data)
{
	struct sdio_mmc_card *card = adapter->card;
	int ret;

	sdio_claim_host(card->func);
	ret = mwifiex_write_reg_locked(card->func, reg, data);
>>>>>>> refs/remotes/origin/master
	sdio_release_host(card->func);

	return ret;
}

/*
 * This function reads data from SDIO card register.
 */
static int
<<<<<<< HEAD
mwifiex_read_reg(struct mwifiex_adapter *adapter, u32 reg, u32 *data)
=======
mwifiex_read_reg(struct mwifiex_adapter *adapter, u32 reg, u8 *data)
>>>>>>> refs/remotes/origin/master
{
	struct sdio_mmc_card *card = adapter->card;
	int ret = -1;
	u8 val;

	sdio_claim_host(card->func);
	val = sdio_readb(card->func, reg, &ret);
	sdio_release_host(card->func);

	*data = val;

	return ret;
}

/*
 * This function writes multiple data into SDIO card memory.
 *
 * This does not work in suspended mode.
 */
static int
mwifiex_write_data_sync(struct mwifiex_adapter *adapter,
			u8 *buffer, u32 pkt_len, u32 port)
{
	struct sdio_mmc_card *card = adapter->card;
<<<<<<< HEAD
	int ret = -1;
=======
	int ret;
>>>>>>> refs/remotes/origin/master
	u8 blk_mode =
		(port & MWIFIEX_SDIO_BYTE_MODE_MASK) ? BYTE_MODE : BLOCK_MODE;
	u32 blk_size = (blk_mode == BLOCK_MODE) ? MWIFIEX_SDIO_BLOCK_SIZE : 1;
	u32 blk_cnt =
		(blk_mode ==
		 BLOCK_MODE) ? (pkt_len /
				MWIFIEX_SDIO_BLOCK_SIZE) : pkt_len;
	u32 ioport = (port & MWIFIEX_SDIO_IO_PORT_MASK);

	if (adapter->is_suspended) {
		dev_err(adapter->dev,
			"%s: not allowed while suspended\n", __func__);
		return -1;
	}

	sdio_claim_host(card->func);

<<<<<<< HEAD
	if (!sdio_writesb(card->func, ioport, buffer, blk_cnt * blk_size))
		ret = 0;
=======
	ret = sdio_writesb(card->func, ioport, buffer, blk_cnt * blk_size);
>>>>>>> refs/remotes/origin/master

	sdio_release_host(card->func);

	return ret;
}

/*
 * This function reads multiple data from SDIO card memory.
 */
static int mwifiex_read_data_sync(struct mwifiex_adapter *adapter, u8 *buffer,
				  u32 len, u32 port, u8 claim)
{
	struct sdio_mmc_card *card = adapter->card;
<<<<<<< HEAD
	int ret = -1;
<<<<<<< HEAD
	u8 blk_mode =
		(port & MWIFIEX_SDIO_BYTE_MODE_MASK) ? BYTE_MODE : BLOCK_MODE;
	u32 blk_size = (blk_mode == BLOCK_MODE) ? MWIFIEX_SDIO_BLOCK_SIZE : 1;
	u32 blk_cnt =
		(blk_mode ==
		 BLOCK_MODE) ? (len / MWIFIEX_SDIO_BLOCK_SIZE) : len;
=======
=======
	int ret;
>>>>>>> refs/remotes/origin/master
	u8 blk_mode = (port & MWIFIEX_SDIO_BYTE_MODE_MASK) ? BYTE_MODE
		       : BLOCK_MODE;
	u32 blk_size = (blk_mode == BLOCK_MODE) ? MWIFIEX_SDIO_BLOCK_SIZE : 1;
	u32 blk_cnt = (blk_mode == BLOCK_MODE) ? (len / MWIFIEX_SDIO_BLOCK_SIZE)
			: len;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u32 ioport = (port & MWIFIEX_SDIO_IO_PORT_MASK);

	if (claim)
		sdio_claim_host(card->func);

<<<<<<< HEAD
	if (!sdio_readsb(card->func, buffer, ioport, blk_cnt * blk_size))
		ret = 0;
=======
	ret = sdio_readsb(card->func, buffer, ioport, blk_cnt * blk_size);
>>>>>>> refs/remotes/origin/master

	if (claim)
		sdio_release_host(card->func);

	return ret;
}

/*
 * This function wakes up the card.
 *
 * A host power up command is written to the card configuration
 * register to wake up the card.
 */
static int mwifiex_pm_wakeup_card(struct mwifiex_adapter *adapter)
{
	dev_dbg(adapter->dev, "event: wakeup device...\n");

	return mwifiex_write_reg(adapter, CONFIGURATION_REG, HOST_POWER_UP);
}

/*
 * This function is called after the card has woken up.
 *
 * The card configuration register is reset.
 */
static int mwifiex_pm_wakeup_card_complete(struct mwifiex_adapter *adapter)
{
	dev_dbg(adapter->dev, "cmd: wakeup device completed\n");

	return mwifiex_write_reg(adapter, CONFIGURATION_REG, 0);
}

/*
<<<<<<< HEAD
 * This function initializes the IO ports.
=======
 * This function is used to initialize IO ports for the
 * chipsets supporting SDIO new mode eg SD8897.
 */
static int mwifiex_init_sdio_new_mode(struct mwifiex_adapter *adapter)
{
	u8 reg;

	adapter->ioport = MEM_PORT;

	/* enable sdio new mode */
	if (mwifiex_read_reg(adapter, CARD_CONFIG_2_1_REG, &reg))
		return -1;
	if (mwifiex_write_reg(adapter, CARD_CONFIG_2_1_REG,
			      reg | CMD53_NEW_MODE))
		return -1;

	/* Configure cmd port and enable reading rx length from the register */
	if (mwifiex_read_reg(adapter, CMD_CONFIG_0, &reg))
		return -1;
	if (mwifiex_write_reg(adapter, CMD_CONFIG_0, reg | CMD_PORT_RD_LEN_EN))
		return -1;

	/* Enable Dnld/Upld ready auto reset for cmd port after cmd53 is
	 * completed
	 */
	if (mwifiex_read_reg(adapter, CMD_CONFIG_1, &reg))
		return -1;
	if (mwifiex_write_reg(adapter, CMD_CONFIG_1, reg | CMD_PORT_AUTO_EN))
		return -1;

	return 0;
}

/* This function initializes the IO ports.
>>>>>>> refs/remotes/origin/master
 *
 * The following operations are performed -
 *      - Read the IO ports (0, 1 and 2)
 *      - Set host interrupt Reset-To-Read to clear
 *      - Set auto re-enable interrupt
 */
static int mwifiex_init_sdio_ioport(struct mwifiex_adapter *adapter)
{
<<<<<<< HEAD
	u32 reg;

	adapter->ioport = 0;

=======
	u8 reg;
	struct sdio_mmc_card *card = adapter->card;

	adapter->ioport = 0;

	if (card->supports_sdio_new_mode) {
		if (mwifiex_init_sdio_new_mode(adapter))
			return -1;
		goto cont;
	}

>>>>>>> refs/remotes/origin/master
	/* Read the IO port */
	if (!mwifiex_read_reg(adapter, IO_PORT_0_REG, &reg))
		adapter->ioport |= (reg & 0xff);
	else
		return -1;

	if (!mwifiex_read_reg(adapter, IO_PORT_1_REG, &reg))
		adapter->ioport |= ((reg & 0xff) << 8);
	else
		return -1;

	if (!mwifiex_read_reg(adapter, IO_PORT_2_REG, &reg))
		adapter->ioport |= ((reg & 0xff) << 16);
	else
		return -1;
<<<<<<< HEAD

=======
cont:
>>>>>>> refs/remotes/origin/master
	pr_debug("info: SDIO FUNC1 IO port: %#x\n", adapter->ioport);

	/* Set Host interrupt reset to read to clear */
	if (!mwifiex_read_reg(adapter, HOST_INT_RSR_REG, &reg))
		mwifiex_write_reg(adapter, HOST_INT_RSR_REG,
<<<<<<< HEAD
				  reg | SDIO_INT_MASK);
=======
				  reg | card->reg->sdio_int_mask);
>>>>>>> refs/remotes/origin/master
	else
		return -1;

	/* Dnld/Upld ready set to auto reset */
<<<<<<< HEAD
	if (!mwifiex_read_reg(adapter, CARD_MISC_CFG_REG, &reg))
		mwifiex_write_reg(adapter, CARD_MISC_CFG_REG,
=======
	if (!mwifiex_read_reg(adapter, card->reg->card_misc_cfg_reg, &reg))
		mwifiex_write_reg(adapter, card->reg->card_misc_cfg_reg,
>>>>>>> refs/remotes/origin/master
				  reg | AUTO_RE_ENABLE_INT);
	else
		return -1;

	return 0;
}

/*
 * This function sends data to the card.
 */
static int mwifiex_write_data_to_card(struct mwifiex_adapter *adapter,
				      u8 *payload, u32 pkt_len, u32 port)
{
	u32 i = 0;
	int ret;

	do {
		ret = mwifiex_write_data_sync(adapter, payload, pkt_len, port);
		if (ret) {
			i++;
			dev_err(adapter->dev, "host_to_card, write iomem"
					" (%d) failed: %d\n", i, ret);
<<<<<<< HEAD
<<<<<<< HEAD
			if (mwifiex_write_reg(adapter,
					CONFIGURATION_REG, 0x04))
=======
			if (mwifiex_write_reg(adapter, CONFIGURATION_REG, 0x04))
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (mwifiex_write_reg(adapter, CONFIGURATION_REG, 0x04))
>>>>>>> refs/remotes/origin/master
				dev_err(adapter->dev, "write CFG reg failed\n");

			ret = -1;
			if (i > MAX_WRITE_IOMEM_RETRY)
				return ret;
		}
	} while (ret == -1);

	return ret;
}

/*
 * This function gets the read port.
 *
 * If control port bit is set in MP read bitmap, the control port
 * is returned, otherwise the current read port is returned and
 * the value is increased (provided it does not reach the maximum
 * limit, in which case it is reset to 1)
 */
static int mwifiex_get_rd_port(struct mwifiex_adapter *adapter, u8 *port)
{
	struct sdio_mmc_card *card = adapter->card;
<<<<<<< HEAD
	u16 rd_bitmap = card->mp_rd_bitmap;

	dev_dbg(adapter->dev, "data: mp_rd_bitmap=0x%04x\n", rd_bitmap);

	if (!(rd_bitmap & (CTRL_PORT_MASK | DATA_PORT_MASK)))
		return -1;

	if (card->mp_rd_bitmap & CTRL_PORT_MASK) {
		card->mp_rd_bitmap &= (u16) (~CTRL_PORT_MASK);
		*port = CTRL_PORT;
		dev_dbg(adapter->dev, "data: port=%d mp_rd_bitmap=0x%04x\n",
<<<<<<< HEAD
		       *port, card->mp_rd_bitmap);
	} else {
		if (card->mp_rd_bitmap & (1 << card->curr_rd_port)) {
			card->mp_rd_bitmap &=
				(u16) (~(1 << card->curr_rd_port));
=======
			*port, card->mp_rd_bitmap);
	} else {
		if (card->mp_rd_bitmap & (1 << card->curr_rd_port)) {
			card->mp_rd_bitmap &= (u16)
						(~(1 << card->curr_rd_port));
>>>>>>> refs/remotes/origin/cm-10.0
			*port = card->curr_rd_port;

			if (++card->curr_rd_port == MAX_PORT)
				card->curr_rd_port = 1;
		} else {
			return -1;
		}

		dev_dbg(adapter->dev,
			"data: port=%d mp_rd_bitmap=0x%04x -> 0x%04x\n",
<<<<<<< HEAD
		       *port, rd_bitmap, card->mp_rd_bitmap);
=======
			*port, rd_bitmap, card->mp_rd_bitmap);
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
	const struct mwifiex_sdio_card_reg *reg = card->reg;
	u32 rd_bitmap = card->mp_rd_bitmap;

	dev_dbg(adapter->dev, "data: mp_rd_bitmap=0x%08x\n", rd_bitmap);

	if (card->supports_sdio_new_mode) {
		if (!(rd_bitmap & reg->data_port_mask))
			return -1;
	} else {
		if (!(rd_bitmap & (CTRL_PORT_MASK | reg->data_port_mask)))
			return -1;
	}

	if ((card->has_control_mask) &&
	    (card->mp_rd_bitmap & CTRL_PORT_MASK)) {
		card->mp_rd_bitmap &= (u32) (~CTRL_PORT_MASK);
		*port = CTRL_PORT;
		dev_dbg(adapter->dev, "data: port=%d mp_rd_bitmap=0x%08x\n",
			*port, card->mp_rd_bitmap);
		return 0;
	}

	if (!(card->mp_rd_bitmap & (1 << card->curr_rd_port)))
		return -1;

	/* We are now handling the SDIO data ports */
	card->mp_rd_bitmap &= (u32)(~(1 << card->curr_rd_port));
	*port = card->curr_rd_port;

	if (++card->curr_rd_port == card->max_ports)
		card->curr_rd_port = reg->start_rd_port;

	dev_dbg(adapter->dev,
		"data: port=%d mp_rd_bitmap=0x%08x -> 0x%08x\n",
		*port, rd_bitmap, card->mp_rd_bitmap);

>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * This function gets the write port for data.
 *
 * The current write port is returned if available and the value is
 * increased (provided it does not reach the maximum limit, in which
 * case it is reset to 1)
 */
<<<<<<< HEAD
static int mwifiex_get_wr_port_data(struct mwifiex_adapter *adapter, u8 *port)
{
	struct sdio_mmc_card *card = adapter->card;
	u16 wr_bitmap = card->mp_wr_bitmap;

	dev_dbg(adapter->dev, "data: mp_wr_bitmap=0x%04x\n", wr_bitmap);

	if (!(wr_bitmap & card->mp_data_port_mask))
		return -1;

	if (card->mp_wr_bitmap & (1 << card->curr_wr_port)) {
		card->mp_wr_bitmap &= (u16) (~(1 << card->curr_wr_port));
		*port = card->curr_wr_port;
		if (++card->curr_wr_port == card->mp_end_port)
			card->curr_wr_port = 1;
=======
static int mwifiex_get_wr_port_data(struct mwifiex_adapter *adapter, u32 *port)
{
	struct sdio_mmc_card *card = adapter->card;
	const struct mwifiex_sdio_card_reg *reg = card->reg;
	u32 wr_bitmap = card->mp_wr_bitmap;

	dev_dbg(adapter->dev, "data: mp_wr_bitmap=0x%08x\n", wr_bitmap);

	if (card->supports_sdio_new_mode &&
	    !(wr_bitmap & reg->data_port_mask)) {
		adapter->data_sent = true;
		return -EBUSY;
	} else if (!card->supports_sdio_new_mode &&
		   !(wr_bitmap & card->mp_data_port_mask)) {
		return -1;
	}

	if (card->mp_wr_bitmap & (1 << card->curr_wr_port)) {
		card->mp_wr_bitmap &= (u32) (~(1 << card->curr_wr_port));
		*port = card->curr_wr_port;
		if (((card->supports_sdio_new_mode) &&
		     (++card->curr_wr_port == card->max_ports)) ||
		    ((!card->supports_sdio_new_mode) &&
		     (++card->curr_wr_port == card->mp_end_port)))
			card->curr_wr_port = reg->start_wr_port;
>>>>>>> refs/remotes/origin/master
	} else {
		adapter->data_sent = true;
		return -EBUSY;
	}

<<<<<<< HEAD
	if (*port == CTRL_PORT) {
		dev_err(adapter->dev, "invalid data port=%d cur port=%d"
<<<<<<< HEAD
				" mp_wr_bitmap=0x%04x -> 0x%04x\n",
				*port, card->curr_wr_port, wr_bitmap,
				card->mp_wr_bitmap);
=======
			" mp_wr_bitmap=0x%04x -> 0x%04x\n",
			*port, card->curr_wr_port, wr_bitmap,
			card->mp_wr_bitmap);
>>>>>>> refs/remotes/origin/cm-10.0
		return -1;
	}

	dev_dbg(adapter->dev, "data: port=%d mp_wr_bitmap=0x%04x -> 0x%04x\n",
<<<<<<< HEAD
	       *port, wr_bitmap, card->mp_wr_bitmap);
=======
		*port, wr_bitmap, card->mp_wr_bitmap);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((card->has_control_mask) && (*port == CTRL_PORT)) {
		dev_err(adapter->dev,
			"invalid data port=%d cur port=%d mp_wr_bitmap=0x%08x -> 0x%08x\n",
			*port, card->curr_wr_port, wr_bitmap,
			card->mp_wr_bitmap);
		return -1;
	}

	dev_dbg(adapter->dev, "data: port=%d mp_wr_bitmap=0x%08x -> 0x%08x\n",
		*port, wr_bitmap, card->mp_wr_bitmap);
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * This function polls the card status.
 */
static int
mwifiex_sdio_poll_card_status(struct mwifiex_adapter *adapter, u8 bits)
{
<<<<<<< HEAD
	u32 tries;
	u32 cs;

	for (tries = 0; tries < MAX_POLL_TRIES; tries++) {
		if (mwifiex_read_reg(adapter, CARD_STATUS_REG, &cs))
=======
	struct sdio_mmc_card *card = adapter->card;
	u32 tries;
	u8 cs;

	for (tries = 0; tries < MAX_POLL_TRIES; tries++) {
		if (mwifiex_read_reg(adapter, card->reg->poll_reg, &cs))
>>>>>>> refs/remotes/origin/master
			break;
		else if ((cs & bits) == bits)
			return 0;

<<<<<<< HEAD
<<<<<<< HEAD
		udelay(10);
	}

	dev_err(adapter->dev, "poll card status failed, tries = %d\n",
	       tries);
=======
=======
>>>>>>> refs/remotes/origin/master
		usleep_range(10, 20);
	}

	dev_err(adapter->dev, "poll card status failed, tries = %d\n", tries);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return -1;
}

/*
 * This function reads the firmware status.
 */
static int
mwifiex_sdio_read_fw_status(struct mwifiex_adapter *adapter, u16 *dat)
{
<<<<<<< HEAD
	u32 fws0, fws1;

	if (mwifiex_read_reg(adapter, CARD_FW_STATUS0_REG, &fws0))
		return -1;

	if (mwifiex_read_reg(adapter, CARD_FW_STATUS1_REG, &fws1))
=======
	struct sdio_mmc_card *card = adapter->card;
	const struct mwifiex_sdio_card_reg *reg = card->reg;
	u8 fws0, fws1;

	if (mwifiex_read_reg(adapter, reg->status_reg_0, &fws0))
		return -1;

	if (mwifiex_read_reg(adapter, reg->status_reg_1, &fws1))
>>>>>>> refs/remotes/origin/master
		return -1;

	*dat = (u16) ((fws1 << 8) | fws0);

	return 0;
}

/*
 * This function disables the host interrupt.
 *
 * The host interrupt mask is read, the disable bit is reset and
 * written back to the card host interrupt mask register.
 */
<<<<<<< HEAD
static int mwifiex_sdio_disable_host_int(struct mwifiex_adapter *adapter)
{
	u32 host_int_mask;

	/* Read back the host_int_mask register */
	if (mwifiex_read_reg(adapter, HOST_INT_MASK_REG, &host_int_mask))
		return -1;

	/* Update with the mask and write back to the register */
	host_int_mask &= ~HOST_INT_DISABLE;

	if (mwifiex_write_reg(adapter, HOST_INT_MASK_REG, host_int_mask)) {
		dev_err(adapter->dev, "disable host interrupt failed\n");
		return -1;
	}

	return 0;
=======
static void mwifiex_sdio_disable_host_int(struct mwifiex_adapter *adapter)
{
	struct sdio_mmc_card *card = adapter->card;
	struct sdio_func *func = card->func;

	sdio_claim_host(func);
	mwifiex_write_reg_locked(func, HOST_INT_MASK_REG, 0);
	sdio_release_irq(func);
	sdio_release_host(func);
}

/*
 * This function reads the interrupt status from card.
 */
static void mwifiex_interrupt_status(struct mwifiex_adapter *adapter)
{
	struct sdio_mmc_card *card = adapter->card;
	u8 sdio_ireg;
	unsigned long flags;

	if (mwifiex_read_data_sync(adapter, card->mp_regs,
				   card->reg->max_mp_regs,
				   REG_PORT | MWIFIEX_SDIO_BYTE_MODE_MASK, 0)) {
		dev_err(adapter->dev, "read mp_regs failed\n");
		return;
	}

	sdio_ireg = card->mp_regs[HOST_INTSTATUS_REG];
	if (sdio_ireg) {
		/*
		 * DN_LD_HOST_INT_STATUS and/or UP_LD_HOST_INT_STATUS
		 * For SDIO new mode CMD port interrupts
		 *	DN_LD_CMD_PORT_HOST_INT_STATUS and/or
		 *	UP_LD_CMD_PORT_HOST_INT_STATUS
		 * Clear the interrupt status register
		 */
		dev_dbg(adapter->dev, "int: sdio_ireg = %#x\n", sdio_ireg);
		spin_lock_irqsave(&adapter->int_lock, flags);
		adapter->int_status |= sdio_ireg;
		spin_unlock_irqrestore(&adapter->int_lock, flags);
	}
}

/*
 * SDIO interrupt handler.
 *
 * This function reads the interrupt status from firmware and handles
 * the interrupt in current thread (ksdioirqd) right away.
 */
static void
mwifiex_sdio_interrupt(struct sdio_func *func)
{
	struct mwifiex_adapter *adapter;
	struct sdio_mmc_card *card;

	card = sdio_get_drvdata(func);
	if (!card || !card->adapter) {
		pr_debug("int: func=%p card=%p adapter=%p\n",
			 func, card, card ? card->adapter : NULL);
		return;
	}
	adapter = card->adapter;

	if (!adapter->pps_uapsd_mode && adapter->ps_state == PS_STATE_SLEEP)
		adapter->ps_state = PS_STATE_AWAKE;

	mwifiex_interrupt_status(adapter);
	mwifiex_main_process(adapter);
>>>>>>> refs/remotes/origin/master
}

/*
 * This function enables the host interrupt.
 *
 * The host interrupt enable mask is written to the card
 * host interrupt mask register.
 */
static int mwifiex_sdio_enable_host_int(struct mwifiex_adapter *adapter)
{
<<<<<<< HEAD
	/* Simply write the mask to the register */
	if (mwifiex_write_reg(adapter, HOST_INT_MASK_REG, HOST_INT_ENABLE)) {
		dev_err(adapter->dev, "enable host interrupt failed\n");
		return -1;
	}
	return 0;
=======
	struct sdio_mmc_card *card = adapter->card;
	struct sdio_func *func = card->func;
	int ret;

	sdio_claim_host(func);

	/* Request the SDIO IRQ */
	ret = sdio_claim_irq(func, mwifiex_sdio_interrupt);
	if (ret) {
		dev_err(adapter->dev, "claim irq failed: ret=%d\n", ret);
		goto out;
	}

	/* Simply write the mask to the register */
	ret = mwifiex_write_reg_locked(func, HOST_INT_MASK_REG,
				       card->reg->host_int_enable);
	if (ret) {
		dev_err(adapter->dev, "enable host interrupt failed\n");
		sdio_release_irq(func);
	}

out:
	sdio_release_host(func);
	return ret;
>>>>>>> refs/remotes/origin/master
}

/*
 * This function sends a data buffer to the card.
 */
static int mwifiex_sdio_card_to_host(struct mwifiex_adapter *adapter,
				     u32 *type, u8 *buffer,
				     u32 npayload, u32 ioport)
{
	int ret;
	u32 nb;

	if (!buffer) {
		dev_err(adapter->dev, "%s: buffer is NULL\n", __func__);
		return -1;
	}

	ret = mwifiex_read_data_sync(adapter, buffer, npayload, ioport, 1);

	if (ret) {
		dev_err(adapter->dev, "%s: read iomem failed: %d\n", __func__,
<<<<<<< HEAD
<<<<<<< HEAD
				ret);
=======
			ret);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			ret);
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	nb = le16_to_cpu(*(__le16 *) (buffer));
	if (nb > npayload) {
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(adapter->dev, "%s: invalid packet, nb=%d, npayload=%d\n",
				__func__, nb, npayload);
=======
		dev_err(adapter->dev, "%s: invalid packet, nb=%d npayload=%d\n",
			__func__, nb, npayload);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dev_err(adapter->dev, "%s: invalid packet, nb=%d npayload=%d\n",
			__func__, nb, npayload);
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	*type = le16_to_cpu(*(__le16 *) (buffer + 2));

	return ret;
}

/*
 * This function downloads the firmware to the card.
 *
 * Firmware is downloaded to the card in blocks. Every block download
 * is tested for CRC errors, and retried a number of times before
 * returning failure.
 */
static int mwifiex_prog_fw_w_helper(struct mwifiex_adapter *adapter,
				    struct mwifiex_fw_image *fw)
{
<<<<<<< HEAD
=======
	struct sdio_mmc_card *card = adapter->card;
	const struct mwifiex_sdio_card_reg *reg = card->reg;
>>>>>>> refs/remotes/origin/master
	int ret;
	u8 *firmware = fw->fw_buf;
	u32 firmware_len = fw->fw_len;
	u32 offset = 0;
<<<<<<< HEAD
	u32 base0, base1;
=======
	u8 base0, base1;
>>>>>>> refs/remotes/origin/master
	u8 *fwbuf;
	u16 len = 0;
	u32 txlen, tx_blocks = 0, tries;
	u32 i = 0;

	if (!firmware_len) {
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(adapter->dev, "firmware image not found!"
				" Terminating download\n");
=======
		dev_err(adapter->dev,
			"firmware image not found! Terminating download\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dev_err(adapter->dev,
			"firmware image not found! Terminating download\n");
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	dev_dbg(adapter->dev, "info: downloading FW image (%d bytes)\n",
<<<<<<< HEAD
<<<<<<< HEAD
			firmware_len);
=======
		firmware_len);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Assume that the allocated buffer is 8-byte aligned */
	fwbuf = kzalloc(MWIFIEX_UPLD_SIZE, GFP_KERNEL);
	if (!fwbuf) {
<<<<<<< HEAD
		dev_err(adapter->dev, "unable to alloc buffer for firmware."
				" Terminating download\n");
=======
		dev_err(adapter->dev,
			"unable to alloc buffer for FW. Terminating dnld\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;
	}
=======
		firmware_len);

	/* Assume that the allocated buffer is 8-byte aligned */
	fwbuf = kzalloc(MWIFIEX_UPLD_SIZE, GFP_KERNEL);
	if (!fwbuf)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	/* Perform firmware data transfer */
	do {
		/* The host polls for the DN_LD_CARD_RDY and CARD_IO_READY
		   bits */
		ret = mwifiex_sdio_poll_card_status(adapter, CARD_IO_READY |
						    DN_LD_CARD_RDY);
		if (ret) {
			dev_err(adapter->dev, "FW download with helper:"
<<<<<<< HEAD
<<<<<<< HEAD
					" poll status timeout @ %d\n", offset);
=======
				" poll status timeout @ %d\n", offset);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				" poll status timeout @ %d\n", offset);
>>>>>>> refs/remotes/origin/master
			goto done;
		}

		/* More data? */
		if (offset >= firmware_len)
			break;

		for (tries = 0; tries < MAX_POLL_TRIES; tries++) {
<<<<<<< HEAD
			ret = mwifiex_read_reg(adapter, HOST_F1_RD_BASE_0,
					       &base0);
			if (ret) {
<<<<<<< HEAD
				dev_err(adapter->dev, "dev BASE0 register read"
					" failed: base0=0x%04X(%d). Terminating "
				       "download\n", base0, base0);
=======
=======
			ret = mwifiex_read_reg(adapter, reg->base_0_reg,
					       &base0);
			if (ret) {
>>>>>>> refs/remotes/origin/master
				dev_err(adapter->dev,
					"dev BASE0 register read failed: "
					"base0=%#04X(%d). Terminating dnld\n",
					base0, base0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
				goto done;
			}
			ret = mwifiex_read_reg(adapter, HOST_F1_RD_BASE_1,
					       &base1);
			if (ret) {
<<<<<<< HEAD
				dev_err(adapter->dev, "dev BASE1 register read"
					" failed: base1=0x%04X(%d). Terminating "
				       "download\n", base1, base1);
=======
=======
				goto done;
			}
			ret = mwifiex_read_reg(adapter, reg->base_1_reg,
					       &base1);
			if (ret) {
>>>>>>> refs/remotes/origin/master
				dev_err(adapter->dev,
					"dev BASE1 register read failed: "
					"base1=%#04X(%d). Terminating dnld\n",
					base1, base1);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				goto done;
			}
			len = (u16) (((base1 & 0xff) << 8) | (base0 & 0xff));

			if (len)
				break;

<<<<<<< HEAD
<<<<<<< HEAD
			udelay(10);
=======
			usleep_range(10, 20);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			usleep_range(10, 20);
>>>>>>> refs/remotes/origin/master
		}

		if (!len) {
			break;
		} else if (len > MWIFIEX_UPLD_SIZE) {
<<<<<<< HEAD
<<<<<<< HEAD
			dev_err(adapter->dev, "FW download failed @ %d,"
				" invalid length %d\n", offset, len);
=======
			dev_err(adapter->dev,
				"FW dnld failed @ %d, invalid length %d\n",
				offset, len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			dev_err(adapter->dev,
				"FW dnld failed @ %d, invalid length %d\n",
				offset, len);
>>>>>>> refs/remotes/origin/master
			ret = -1;
			goto done;
		}

		txlen = len;

		if (len & BIT(0)) {
			i++;
			if (i > MAX_WRITE_IOMEM_RETRY) {
<<<<<<< HEAD
<<<<<<< HEAD
				dev_err(adapter->dev, "FW download failed @"
					" %d, over max retry count\n", offset);
=======
				dev_err(adapter->dev,
					"FW dnld failed @ %d, over max retry\n",
					offset);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				dev_err(adapter->dev,
					"FW dnld failed @ %d, over max retry\n",
					offset);
>>>>>>> refs/remotes/origin/master
				ret = -1;
				goto done;
			}
			dev_err(adapter->dev, "CRC indicated by the helper:"
<<<<<<< HEAD
<<<<<<< HEAD
			       " len = 0x%04X, txlen = %d\n", len, txlen);
=======
				" len = 0x%04X, txlen = %d\n", len, txlen);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				" len = 0x%04X, txlen = %d\n", len, txlen);
>>>>>>> refs/remotes/origin/master
			len &= ~BIT(0);
			/* Setting this to 0 to resend from same offset */
			txlen = 0;
		} else {
			i = 0;

			/* Set blocksize to transfer - checking for last
			   block */
			if (firmware_len - offset < txlen)
				txlen = firmware_len - offset;

<<<<<<< HEAD
<<<<<<< HEAD
			tx_blocks = (txlen + MWIFIEX_SDIO_BLOCK_SIZE -
					1) / MWIFIEX_SDIO_BLOCK_SIZE;
=======
			tx_blocks = (txlen + MWIFIEX_SDIO_BLOCK_SIZE - 1)
				    / MWIFIEX_SDIO_BLOCK_SIZE;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			tx_blocks = (txlen + MWIFIEX_SDIO_BLOCK_SIZE - 1)
				    / MWIFIEX_SDIO_BLOCK_SIZE;
>>>>>>> refs/remotes/origin/master

			/* Copy payload to buffer */
			memmove(fwbuf, &firmware[offset], txlen);
		}

		ret = mwifiex_write_data_sync(adapter, fwbuf, tx_blocks *
					      MWIFIEX_SDIO_BLOCK_SIZE,
					      adapter->ioport);
		if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
			dev_err(adapter->dev, "FW download, write iomem (%d)"
					" failed @ %d\n", i, offset);
=======
			dev_err(adapter->dev,
				"FW download, write iomem (%d) failed @ %d\n",
				i, offset);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			dev_err(adapter->dev,
				"FW download, write iomem (%d) failed @ %d\n",
				i, offset);
>>>>>>> refs/remotes/origin/master
			if (mwifiex_write_reg(adapter, CONFIGURATION_REG, 0x04))
				dev_err(adapter->dev, "write CFG reg failed\n");

			ret = -1;
			goto done;
		}

		offset += txlen;
	} while (true);

	dev_dbg(adapter->dev, "info: FW download over, size %d bytes\n",
<<<<<<< HEAD
<<<<<<< HEAD
						offset);
=======
		offset);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		offset);
>>>>>>> refs/remotes/origin/master

	ret = 0;
done:
	kfree(fwbuf);
	return ret;
}

/*
 * This function checks the firmware status in card.
 *
 * The winner interface is also determined by this function.
 */
static int mwifiex_check_fw_status(struct mwifiex_adapter *adapter,
<<<<<<< HEAD
<<<<<<< HEAD
				   u32 poll_num, int *winner)
=======
				   u32 poll_num)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int ret = 0;
	u16 firmware_stat;
	u32 tries;
	u32 winner_status;
=======
				   u32 poll_num)
{
	struct sdio_mmc_card *card = adapter->card;
	int ret = 0;
	u16 firmware_stat;
	u32 tries;
	u8 winner_status;
>>>>>>> refs/remotes/origin/master

	/* Wait for firmware initialization event */
	for (tries = 0; tries < poll_num; tries++) {
		ret = mwifiex_sdio_read_fw_status(adapter, &firmware_stat);
		if (ret)
			continue;
<<<<<<< HEAD
<<<<<<< HEAD
		if (firmware_stat == FIRMWARE_READY) {
=======
		if (firmware_stat == FIRMWARE_READY_SDIO) {
>>>>>>> refs/remotes/origin/cm-10.0
			ret = 0;
			break;
		} else {
			mdelay(100);
=======
		if (firmware_stat == FIRMWARE_READY_SDIO) {
			ret = 0;
			break;
		} else {
			msleep(100);
>>>>>>> refs/remotes/origin/master
			ret = -1;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (winner && ret) {
=======
	if (ret) {
>>>>>>> refs/remotes/origin/cm-10.0
		if (mwifiex_read_reg
		    (adapter, CARD_FW_STATUS0_REG, &winner_status))
			winner_status = 0;

		if (winner_status)
<<<<<<< HEAD
			*winner = 0;
		else
			*winner = 1;
=======
			adapter->winner = 0;
		else
			adapter->winner = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ret) {
		if (mwifiex_read_reg
		    (adapter, card->reg->status_reg_0, &winner_status))
			winner_status = 0;

		if (winner_status)
			adapter->winner = 0;
		else
			adapter->winner = 1;
>>>>>>> refs/remotes/origin/master
	}
	return ret;
}

/*
<<<<<<< HEAD
 * This function reads the interrupt status from card.
 */
static void mwifiex_interrupt_status(struct mwifiex_adapter *adapter)
{
	struct sdio_mmc_card *card = adapter->card;
	u32 sdio_ireg;
	unsigned long flags;

	if (mwifiex_read_data_sync(adapter, card->mp_regs, MAX_MP_REGS,
				   REG_PORT | MWIFIEX_SDIO_BYTE_MODE_MASK,
				   0)) {
		dev_err(adapter->dev, "read mp_regs failed\n");
		return;
	}

	sdio_ireg = card->mp_regs[HOST_INTSTATUS_REG];
	if (sdio_ireg) {
		/*
		 * DN_LD_HOST_INT_STATUS and/or UP_LD_HOST_INT_STATUS
		 * Clear the interrupt status register
		 */
		dev_dbg(adapter->dev, "int: sdio_ireg = %#x\n", sdio_ireg);
		spin_lock_irqsave(&adapter->int_lock, flags);
		adapter->int_status |= sdio_ireg;
		spin_unlock_irqrestore(&adapter->int_lock, flags);
	}
}

/*
 * SDIO interrupt handler.
 *
 * This function reads the interrupt status from firmware and assigns
 * the main process in workqueue which will handle the interrupt.
 */
static void
mwifiex_sdio_interrupt(struct sdio_func *func)
{
	struct mwifiex_adapter *adapter;
	struct sdio_mmc_card *card;

	card = sdio_get_drvdata(func);
	if (!card || !card->adapter) {
		pr_debug("int: func=%p card=%p adapter=%p\n",
<<<<<<< HEAD
		       func, card, card ? card->adapter : NULL);
=======
			 func, card, card ? card->adapter : NULL);
>>>>>>> refs/remotes/origin/cm-10.0
		return;
	}
	adapter = card->adapter;

	if (adapter->surprise_removed)
		return;

	if (!adapter->pps_uapsd_mode && adapter->ps_state == PS_STATE_SLEEP)
		adapter->ps_state = PS_STATE_AWAKE;

	mwifiex_interrupt_status(adapter);
	queue_work(adapter->workqueue, &adapter->main_work);
}

/*
=======
>>>>>>> refs/remotes/origin/master
 * This function decodes a received packet.
 *
 * Based on the type, the packet is treated as either a data, or
 * a command response, or an event, and the correct handler
 * function is invoked.
 */
static int mwifiex_decode_rx_packet(struct mwifiex_adapter *adapter,
				    struct sk_buff *skb, u32 upld_typ)
{
	u8 *cmd_buf;
<<<<<<< HEAD
<<<<<<< HEAD

=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	__le16 *curr_ptr = (__le16 *)skb->data;
	u16 pkt_len = le16_to_cpu(*curr_ptr);

	skb_trim(skb, pkt_len);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	skb_pull(skb, INTF_HEADER_LEN);

	switch (upld_typ) {
	case MWIFIEX_TYPE_DATA:
		dev_dbg(adapter->dev, "info: --- Rx: Data packet ---\n");
		mwifiex_handle_rx_packet(adapter, skb);
		break;

	case MWIFIEX_TYPE_CMD:
		dev_dbg(adapter->dev, "info: --- Rx: Cmd Response ---\n");
		/* take care of curr_cmd = NULL case */
		if (!adapter->curr_cmd) {
			cmd_buf = adapter->upld_buf;

			if (adapter->ps_state == PS_STATE_SLEEP_CFM)
				mwifiex_process_sleep_confirm_resp(adapter,
<<<<<<< HEAD
<<<<<<< HEAD
							skb->data, skb->len);

			memcpy(cmd_buf, skb->data, min_t(u32,
				       MWIFIEX_SIZE_OF_CMD_BUFFER, skb->len));
=======
=======
>>>>>>> refs/remotes/origin/master
								   skb->data,
								   skb->len);

			memcpy(cmd_buf, skb->data,
			       min_t(u32, MWIFIEX_SIZE_OF_CMD_BUFFER,
				     skb->len));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

			dev_kfree_skb_any(skb);
		} else {
			adapter->cmd_resp_received = true;
			adapter->curr_cmd->resp_skb = skb;
		}
		break;

	case MWIFIEX_TYPE_EVENT:
		dev_dbg(adapter->dev, "info: --- Rx: Event ---\n");
<<<<<<< HEAD
		adapter->event_cause = *(u32 *) skb->data;

		skb_pull(skb, MWIFIEX_EVENT_HEADER_LEN);

		if ((skb->len > 0) && (skb->len  < MAX_EVENT_SIZE))
			memcpy(adapter->event_body, skb->data, skb->len);
=======
		adapter->event_cause = le32_to_cpu(*(__le32 *) skb->data);

		if ((skb->len > 0) && (skb->len  < MAX_EVENT_SIZE))
			memcpy(adapter->event_body,
			       skb->data + MWIFIEX_EVENT_HEADER_LEN,
			       skb->len);
>>>>>>> refs/remotes/origin/master

		/* event cause has been saved to adapter->event_cause */
		adapter->event_received = true;
		adapter->event_skb = skb;

		break;

	default:
		dev_err(adapter->dev, "unknown upload type %#x\n", upld_typ);
		dev_kfree_skb_any(skb);
		break;
	}

	return 0;
}

/*
 * This function transfers received packets from card to driver, performing
 * aggregation if required.
 *
 * For data received on control port, or if aggregation is disabled, the
 * received buffers are uploaded as separate packets. However, if aggregation
 * is enabled and required, the buffers are copied onto an aggregation buffer,
 * provided there is space left, processed and finally uploaded.
 */
static int mwifiex_sdio_card_to_host_mp_aggr(struct mwifiex_adapter *adapter,
					     struct sk_buff *skb, u8 port)
{
	struct sdio_mmc_card *card = adapter->card;
	s32 f_do_rx_aggr = 0;
	s32 f_do_rx_cur = 0;
	s32 f_aggr_cur = 0;
	struct sk_buff *skb_deaggr;
	u32 pind;
<<<<<<< HEAD
	u32 pkt_len, pkt_type = 0;
	u8 *curr_ptr;
	u32 rx_len = skb->len;

	if (port == CTRL_PORT) {
		/* Read the command Resp without aggr */
		dev_dbg(adapter->dev, "info: %s: no aggregation for cmd "
<<<<<<< HEAD
				"response\n", __func__);
=======
			"response\n", __func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 pkt_len, pkt_type, mport;
	u8 *curr_ptr;
	u32 rx_len = skb->len;

	if ((card->has_control_mask) && (port == CTRL_PORT)) {
		/* Read the command Resp without aggr */
		dev_dbg(adapter->dev, "info: %s: no aggregation for cmd "
			"response\n", __func__);
>>>>>>> refs/remotes/origin/master

		f_do_rx_cur = 1;
		goto rx_curr_single;
	}

	if (!card->mpa_rx.enabled) {
		dev_dbg(adapter->dev, "info: %s: rx aggregation disabled\n",
<<<<<<< HEAD
<<<<<<< HEAD
						__func__);
=======
			__func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			__func__);
>>>>>>> refs/remotes/origin/master

		f_do_rx_cur = 1;
		goto rx_curr_single;
	}

<<<<<<< HEAD
	if (card->mp_rd_bitmap & (~((u16) CTRL_PORT_MASK))) {
=======
	if ((!card->has_control_mask && (card->mp_rd_bitmap &
					 card->reg->data_port_mask)) ||
	    (card->has_control_mask && (card->mp_rd_bitmap &
					(~((u32) CTRL_PORT_MASK))))) {
>>>>>>> refs/remotes/origin/master
		/* Some more data RX pending */
		dev_dbg(adapter->dev, "info: %s: not last packet\n", __func__);

		if (MP_RX_AGGR_IN_PROGRESS(card)) {
			if (MP_RX_AGGR_BUF_HAS_ROOM(card, skb->len)) {
				f_aggr_cur = 1;
			} else {
				/* No room in Aggr buf, do rx aggr now */
				f_do_rx_aggr = 1;
				f_do_rx_cur = 1;
			}
		} else {
			/* Rx aggr not in progress */
			f_aggr_cur = 1;
		}

	} else {
		/* No more data RX pending */
		dev_dbg(adapter->dev, "info: %s: last packet\n", __func__);

		if (MP_RX_AGGR_IN_PROGRESS(card)) {
			f_do_rx_aggr = 1;
			if (MP_RX_AGGR_BUF_HAS_ROOM(card, skb->len))
				f_aggr_cur = 1;
			else
				/* No room in Aggr buf, do rx aggr now */
				f_do_rx_cur = 1;
		} else {
			f_do_rx_cur = 1;
		}
	}

	if (f_aggr_cur) {
		dev_dbg(adapter->dev, "info: current packet aggregation\n");
		/* Curr pkt can be aggregated */
<<<<<<< HEAD
		MP_RX_AGGR_SETUP(card, skb, port);

		if (MP_RX_AGGR_PKT_LIMIT_REACHED(card) ||
		    MP_RX_AGGR_PORT_LIMIT_REACHED(card)) {
			dev_dbg(adapter->dev, "info: %s: aggregated packet "
<<<<<<< HEAD
					"limit reached\n", __func__);
=======
				"limit reached\n", __func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		mp_rx_aggr_setup(card, skb, port);

		if (MP_RX_AGGR_PKT_LIMIT_REACHED(card) ||
		    mp_rx_aggr_port_limit_reached(card)) {
			dev_dbg(adapter->dev, "info: %s: aggregated packet "
				"limit reached\n", __func__);
>>>>>>> refs/remotes/origin/master
			/* No more pkts allowed in Aggr buf, rx it */
			f_do_rx_aggr = 1;
		}
	}

	if (f_do_rx_aggr) {
		/* do aggr RX now */
		dev_dbg(adapter->dev, "info: do_rx_aggr: num of packets: %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
		       card->mpa_rx.pkt_cnt);
=======
			card->mpa_rx.pkt_cnt);
>>>>>>> refs/remotes/origin/cm-10.0

		if (mwifiex_read_data_sync(adapter, card->mpa_rx.buf,
					   card->mpa_rx.buf_len,
					   (adapter->ioport | 0x1000 |
					    (card->mpa_rx.ports << 4)) +
					   card->mpa_rx.start_port, 1))
<<<<<<< HEAD
			return -1;
=======
			goto error;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			card->mpa_rx.pkt_cnt);

		if (card->supports_sdio_new_mode) {
			int i;
			u32 port_count;

			for (i = 0, port_count = 0; i < card->max_ports; i++)
				if (card->mpa_rx.ports & BIT(i))
					port_count++;

			/* Reading data from "start_port + 0" to "start_port +
			 * port_count -1", so decrease the count by 1
			 */
			port_count--;
			mport = (adapter->ioport | SDIO_MPA_ADDR_BASE |
				 (port_count << 8)) + card->mpa_rx.start_port;
		} else {
			mport = (adapter->ioport | SDIO_MPA_ADDR_BASE |
				 (card->mpa_rx.ports << 4)) +
				 card->mpa_rx.start_port;
		}

		if (mwifiex_read_data_sync(adapter, card->mpa_rx.buf,
					   card->mpa_rx.buf_len, mport, 1))
			goto error;
>>>>>>> refs/remotes/origin/master

		curr_ptr = card->mpa_rx.buf;

		for (pind = 0; pind < card->mpa_rx.pkt_cnt; pind++) {

			/* get curr PKT len & type */
<<<<<<< HEAD
			pkt_len = *(u16 *) &curr_ptr[0];
			pkt_type = *(u16 *) &curr_ptr[2];
=======
			pkt_len = le16_to_cpu(*(__le16 *) &curr_ptr[0]);
			pkt_type = le16_to_cpu(*(__le16 *) &curr_ptr[2]);
>>>>>>> refs/remotes/origin/master

			/* copy pkt to deaggr buf */
			skb_deaggr = card->mpa_rx.skb_arr[pind];

			if ((pkt_type == MWIFIEX_TYPE_DATA) && (pkt_len <=
					 card->mpa_rx.len_arr[pind])) {

				memcpy(skb_deaggr->data, curr_ptr, pkt_len);

				skb_trim(skb_deaggr, pkt_len);

				/* Process de-aggr packet */
				mwifiex_decode_rx_packet(adapter, skb_deaggr,
							 pkt_type);
			} else {
				dev_err(adapter->dev, "wrong aggr pkt:"
					" type=%d len=%d max_len=%d\n",
					pkt_type, pkt_len,
					card->mpa_rx.len_arr[pind]);
				dev_kfree_skb_any(skb_deaggr);
			}
			curr_ptr += card->mpa_rx.len_arr[pind];
		}
		MP_RX_AGGR_BUF_RESET(card);
	}

rx_curr_single:
	if (f_do_rx_cur) {
		dev_dbg(adapter->dev, "info: RX: port: %d, rx_len: %d\n",
			port, rx_len);

		if (mwifiex_sdio_card_to_host(adapter, &pkt_type,
					      skb->data, skb->len,
					      adapter->ioport + port))
<<<<<<< HEAD
<<<<<<< HEAD
			return -1;
=======
			goto error;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			goto error;
>>>>>>> refs/remotes/origin/master

		mwifiex_decode_rx_packet(adapter, skb, pkt_type);
	}

	return 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

error:
	if (MP_RX_AGGR_IN_PROGRESS(card)) {
		/* Multiport-aggregation transfer failed - cleanup */
		for (pind = 0; pind < card->mpa_rx.pkt_cnt; pind++) {
			/* copy pkt to deaggr buf */
			skb_deaggr = card->mpa_rx.skb_arr[pind];
			dev_kfree_skb_any(skb_deaggr);
		}
		MP_RX_AGGR_BUF_RESET(card);
	}

	if (f_do_rx_cur)
		/* Single transfer pending. Free curr buff also */
		dev_kfree_skb_any(skb);

	return -1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * This function checks the current interrupt status.
 *
 * The following interrupts are checked and handled by this function -
 *      - Data sent
 *      - Command sent
 *      - Packets received
 *
 * Since the firmware does not generate download ready interrupt if the
 * port updated is command port only, command sent interrupt checking
 * should be done manually, and for every SDIO interrupt.
 *
 * In case of Rx packets received, the packets are uploaded from card to
 * host and processed accordingly.
 */
static int mwifiex_process_int_status(struct mwifiex_adapter *adapter)
{
	struct sdio_mmc_card *card = adapter->card;
<<<<<<< HEAD
=======
	const struct mwifiex_sdio_card_reg *reg = card->reg;
>>>>>>> refs/remotes/origin/master
	int ret = 0;
	u8 sdio_ireg;
	struct sk_buff *skb;
	u8 port = CTRL_PORT;
	u32 len_reg_l, len_reg_u;
	u32 rx_blocks;
	u16 rx_len;
	unsigned long flags;
<<<<<<< HEAD
=======
	u32 bitmap;
	u8 cr;
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&adapter->int_lock, flags);
	sdio_ireg = adapter->int_status;
	adapter->int_status = 0;
	spin_unlock_irqrestore(&adapter->int_lock, flags);

	if (!sdio_ireg)
		return ret;

<<<<<<< HEAD
	if (sdio_ireg & DN_LD_HOST_INT_STATUS) {
		card->mp_wr_bitmap = ((u16) card->mp_regs[WR_BITMAP_U]) << 8;
		card->mp_wr_bitmap |= (u16) card->mp_regs[WR_BITMAP_L];
		dev_dbg(adapter->dev, "int: DNLD: wr_bitmap=0x%04x\n",
<<<<<<< HEAD
				card->mp_wr_bitmap);
=======
			card->mp_wr_bitmap);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Following interrupt is only for SDIO new mode */
	if (sdio_ireg & DN_LD_CMD_PORT_HOST_INT_STATUS && adapter->cmd_sent)
		adapter->cmd_sent = false;

	/* Following interrupt is only for SDIO new mode */
	if (sdio_ireg & UP_LD_CMD_PORT_HOST_INT_STATUS) {
		u32 pkt_type;

		/* read the len of control packet */
		rx_len = card->mp_regs[CMD_RD_LEN_1] << 8;
		rx_len |= (u16) card->mp_regs[CMD_RD_LEN_0];
		rx_blocks = DIV_ROUND_UP(rx_len, MWIFIEX_SDIO_BLOCK_SIZE);
		if (rx_len <= INTF_HEADER_LEN ||
		    (rx_blocks * MWIFIEX_SDIO_BLOCK_SIZE) >
		     MWIFIEX_RX_DATA_BUF_SIZE)
			return -1;
		rx_len = (u16) (rx_blocks * MWIFIEX_SDIO_BLOCK_SIZE);

		skb = dev_alloc_skb(rx_len);
		if (!skb)
			return -1;

		skb_put(skb, rx_len);

		if (mwifiex_sdio_card_to_host(adapter, &pkt_type, skb->data,
					      skb->len, adapter->ioport |
							CMD_PORT_SLCT)) {
			dev_err(adapter->dev,
				"%s: failed to card_to_host", __func__);
			dev_kfree_skb_any(skb);
			goto term_cmd;
		}

		if ((pkt_type != MWIFIEX_TYPE_CMD) &&
		    (pkt_type != MWIFIEX_TYPE_EVENT))
			dev_err(adapter->dev,
				"%s:Received wrong packet on cmd port",
				__func__);

		mwifiex_decode_rx_packet(adapter, skb, pkt_type);
	}

	if (sdio_ireg & DN_LD_HOST_INT_STATUS) {
		bitmap = (u32) card->mp_regs[reg->wr_bitmap_l];
		bitmap |= ((u32) card->mp_regs[reg->wr_bitmap_u]) << 8;
		if (card->supports_sdio_new_mode) {
			bitmap |=
				((u32) card->mp_regs[reg->wr_bitmap_1l]) << 16;
			bitmap |=
				((u32) card->mp_regs[reg->wr_bitmap_1u]) << 24;
		}
		card->mp_wr_bitmap = bitmap;

		dev_dbg(adapter->dev, "int: DNLD: wr_bitmap=0x%x\n",
			card->mp_wr_bitmap);
>>>>>>> refs/remotes/origin/master
		if (adapter->data_sent &&
		    (card->mp_wr_bitmap & card->mp_data_port_mask)) {
			dev_dbg(adapter->dev,
				"info:  <--- Tx DONE Interrupt --->\n");
			adapter->data_sent = false;
		}
	}

	/* As firmware will not generate download ready interrupt if the port
	   updated is command port only, cmd_sent should be done for any SDIO
	   interrupt. */
<<<<<<< HEAD
	if (adapter->cmd_sent) {
		/* Check if firmware has attach buffer at command port and
		   update just that in wr_bit_map. */
		card->mp_wr_bitmap |=
			(u16) card->mp_regs[WR_BITMAP_L] & CTRL_PORT_MASK;
=======
	if (card->has_control_mask && adapter->cmd_sent) {
		/* Check if firmware has attach buffer at command port and
		   update just that in wr_bit_map. */
		card->mp_wr_bitmap |=
			(u32) card->mp_regs[reg->wr_bitmap_l] & CTRL_PORT_MASK;
>>>>>>> refs/remotes/origin/master
		if (card->mp_wr_bitmap & CTRL_PORT_MASK)
			adapter->cmd_sent = false;
	}

	dev_dbg(adapter->dev, "info: cmd_sent=%d data_sent=%d\n",
<<<<<<< HEAD
<<<<<<< HEAD
	       adapter->cmd_sent, adapter->data_sent);
=======
		adapter->cmd_sent, adapter->data_sent);
>>>>>>> refs/remotes/origin/cm-10.0
	if (sdio_ireg & UP_LD_HOST_INT_STATUS) {
		card->mp_rd_bitmap = ((u16) card->mp_regs[RD_BITMAP_U]) << 8;
		card->mp_rd_bitmap |= (u16) card->mp_regs[RD_BITMAP_L];
		dev_dbg(adapter->dev, "int: UPLD: rd_bitmap=0x%04x\n",
<<<<<<< HEAD
				card->mp_rd_bitmap);
=======
			card->mp_rd_bitmap);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		adapter->cmd_sent, adapter->data_sent);
	if (sdio_ireg & UP_LD_HOST_INT_STATUS) {
		bitmap = (u32) card->mp_regs[reg->rd_bitmap_l];
		bitmap |= ((u32) card->mp_regs[reg->rd_bitmap_u]) << 8;
		if (card->supports_sdio_new_mode) {
			bitmap |=
				((u32) card->mp_regs[reg->rd_bitmap_1l]) << 16;
			bitmap |=
				((u32) card->mp_regs[reg->rd_bitmap_1u]) << 24;
		}
		card->mp_rd_bitmap = bitmap;
		dev_dbg(adapter->dev, "int: UPLD: rd_bitmap=0x%x\n",
			card->mp_rd_bitmap);
>>>>>>> refs/remotes/origin/master

		while (true) {
			ret = mwifiex_get_rd_port(adapter, &port);
			if (ret) {
				dev_dbg(adapter->dev,
					"info: no more rd_port available\n");
				break;
			}
<<<<<<< HEAD
			len_reg_l = RD_LEN_P0_L + (port << 1);
			len_reg_u = RD_LEN_P0_U + (port << 1);
			rx_len = ((u16) card->mp_regs[len_reg_u]) << 8;
			rx_len |= (u16) card->mp_regs[len_reg_l];
			dev_dbg(adapter->dev, "info: RX: port=%d rx_len=%u\n",
<<<<<<< HEAD
					port, rx_len);
			rx_blocks =
				(rx_len + MWIFIEX_SDIO_BLOCK_SIZE -
				 1) / MWIFIEX_SDIO_BLOCK_SIZE;
			if (rx_len <= INTF_HEADER_LEN
			    || (rx_blocks * MWIFIEX_SDIO_BLOCK_SIZE) >
			    MWIFIEX_RX_DATA_BUF_SIZE) {
				dev_err(adapter->dev, "invalid rx_len=%d\n",
						rx_len);
=======
=======
			len_reg_l = reg->rd_len_p0_l + (port << 1);
			len_reg_u = reg->rd_len_p0_u + (port << 1);
			rx_len = ((u16) card->mp_regs[len_reg_u]) << 8;
			rx_len |= (u16) card->mp_regs[len_reg_l];
			dev_dbg(adapter->dev, "info: RX: port=%d rx_len=%u\n",
>>>>>>> refs/remotes/origin/master
				port, rx_len);
			rx_blocks =
				(rx_len + MWIFIEX_SDIO_BLOCK_SIZE -
				 1) / MWIFIEX_SDIO_BLOCK_SIZE;
			if (rx_len <= INTF_HEADER_LEN ||
			    (rx_blocks * MWIFIEX_SDIO_BLOCK_SIZE) >
			     MWIFIEX_RX_DATA_BUF_SIZE) {
				dev_err(adapter->dev, "invalid rx_len=%d\n",
					rx_len);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				return -1;
			}
			rx_len = (u16) (rx_blocks * MWIFIEX_SDIO_BLOCK_SIZE);

			skb = dev_alloc_skb(rx_len);

			if (!skb) {
				dev_err(adapter->dev, "%s: failed to alloc skb",
<<<<<<< HEAD
<<<<<<< HEAD
								__func__);
=======
					__func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
					__func__);
>>>>>>> refs/remotes/origin/master
				return -1;
			}

			skb_put(skb, rx_len);

			dev_dbg(adapter->dev, "info: rx_len = %d skb->len = %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
					rx_len, skb->len);
=======
				rx_len, skb->len);
>>>>>>> refs/remotes/origin/cm-10.0

			if (mwifiex_sdio_card_to_host_mp_aggr(adapter, skb,
							      port)) {
				u32 cr = 0;

				dev_err(adapter->dev, "card_to_host_mpa failed:"
<<<<<<< HEAD
						" int status=%#x\n", sdio_ireg);
				if (mwifiex_read_reg(adapter,
						     CONFIGURATION_REG, &cr))
					dev_err(adapter->dev,
							"read CFG reg failed\n");

				dev_dbg(adapter->dev,
						"info: CFG reg val = %d\n", cr);
=======
					" int status=%#x\n", sdio_ireg);
				if (mwifiex_read_reg(adapter,
						     CONFIGURATION_REG, &cr))
					dev_err(adapter->dev,
						"read CFG reg failed\n");

				dev_dbg(adapter->dev,
					"info: CFG reg val = %d\n", cr);
>>>>>>> refs/remotes/origin/cm-10.0
				if (mwifiex_write_reg(adapter,
						      CONFIGURATION_REG,
						      (cr | 0x04)))
					dev_err(adapter->dev,
<<<<<<< HEAD
							"write CFG reg failed\n");
=======
						"write CFG reg failed\n");
>>>>>>> refs/remotes/origin/cm-10.0

				dev_dbg(adapter->dev, "info: write success\n");
				if (mwifiex_read_reg(adapter,
						     CONFIGURATION_REG, &cr))
					dev_err(adapter->dev,
<<<<<<< HEAD
							"read CFG reg failed\n");

				dev_dbg(adapter->dev,
						"info: CFG reg val =%x\n", cr);
				dev_kfree_skb_any(skb);
=======
						"read CFG reg failed\n");

				dev_dbg(adapter->dev,
					"info: CFG reg val =%x\n", cr);
>>>>>>> refs/remotes/origin/cm-10.0
				return -1;
=======
				rx_len, skb->len);

			if (mwifiex_sdio_card_to_host_mp_aggr(adapter, skb,
							      port)) {
				dev_err(adapter->dev, "card_to_host_mpa failed:"
					" int status=%#x\n", sdio_ireg);
				goto term_cmd;
>>>>>>> refs/remotes/origin/master
			}
		}
	}

	return 0;
<<<<<<< HEAD
=======

term_cmd:
	/* terminate cmd */
	if (mwifiex_read_reg(adapter, CONFIGURATION_REG, &cr))
		dev_err(adapter->dev, "read CFG reg failed\n");
	else
		dev_dbg(adapter->dev, "info: CFG reg val = %d\n", cr);

	if (mwifiex_write_reg(adapter, CONFIGURATION_REG, (cr | 0x04)))
		dev_err(adapter->dev, "write CFG reg failed\n");
	else
		dev_dbg(adapter->dev, "info: write success\n");

	if (mwifiex_read_reg(adapter, CONFIGURATION_REG, &cr))
		dev_err(adapter->dev, "read CFG reg failed\n");
	else
		dev_dbg(adapter->dev, "info: CFG reg val =%x\n", cr);

	return -1;
>>>>>>> refs/remotes/origin/master
}

/*
 * This function aggregates transmission buffers in driver and downloads
 * the aggregated packet to card.
 *
 * The individual packets are aggregated by copying into an aggregation
 * buffer and then downloaded to the card. Previous unsent packets in the
 * aggregation buffer are pre-copied first before new packets are added.
 * Aggregation is done till there is space left in the aggregation buffer,
 * or till new packets are available.
 *
 * The function will only download the packet to the card when aggregation
 * stops, otherwise it will just aggregate the packet in aggregation buffer
 * and return.
 */
static int mwifiex_host_to_card_mp_aggr(struct mwifiex_adapter *adapter,
<<<<<<< HEAD
					u8 *payload, u32 pkt_len, u8 port,
=======
					u8 *payload, u32 pkt_len, u32 port,
>>>>>>> refs/remotes/origin/master
					u32 next_pkt_len)
{
	struct sdio_mmc_card *card = adapter->card;
	int ret = 0;
	s32 f_send_aggr_buf = 0;
	s32 f_send_cur_buf = 0;
	s32 f_precopy_cur_buf = 0;
	s32 f_postcopy_cur_buf = 0;
<<<<<<< HEAD

	if ((!card->mpa_tx.enabled) || (port == CTRL_PORT)) {
		dev_dbg(adapter->dev, "info: %s: tx aggregation disabled\n",
<<<<<<< HEAD
						__func__);
=======
			__func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 mport;

	if (!card->mpa_tx.enabled ||
	    (card->has_control_mask && (port == CTRL_PORT)) ||
	    (card->supports_sdio_new_mode && (port == CMD_PORT_SLCT))) {
		dev_dbg(adapter->dev, "info: %s: tx aggregation disabled\n",
			__func__);
>>>>>>> refs/remotes/origin/master

		f_send_cur_buf = 1;
		goto tx_curr_single;
	}

	if (next_pkt_len) {
		/* More pkt in TX queue */
		dev_dbg(adapter->dev, "info: %s: more packets in queue.\n",
<<<<<<< HEAD
<<<<<<< HEAD
						__func__);
=======
			__func__);
>>>>>>> refs/remotes/origin/cm-10.0

		if (MP_TX_AGGR_IN_PROGRESS(card)) {
			if (!MP_TX_AGGR_PORT_LIMIT_REACHED(card) &&
=======
			__func__);

		if (MP_TX_AGGR_IN_PROGRESS(card)) {
			if (!mp_tx_aggr_port_limit_reached(card) &&
>>>>>>> refs/remotes/origin/master
			    MP_TX_AGGR_BUF_HAS_ROOM(card, pkt_len)) {
				f_precopy_cur_buf = 1;

				if (!(card->mp_wr_bitmap &
<<<<<<< HEAD
<<<<<<< HEAD
						(1 << card->curr_wr_port))
						|| !MP_TX_AGGR_BUF_HAS_ROOM(
							card, next_pkt_len))
=======
				      (1 << card->curr_wr_port)) ||
				    !MP_TX_AGGR_BUF_HAS_ROOM(
					    card, pkt_len + next_pkt_len))
>>>>>>> refs/remotes/origin/cm-10.0
=======
				      (1 << card->curr_wr_port)) ||
				    !MP_TX_AGGR_BUF_HAS_ROOM(
					    card, pkt_len + next_pkt_len))
>>>>>>> refs/remotes/origin/master
					f_send_aggr_buf = 1;
			} else {
				/* No room in Aggr buf, send it */
				f_send_aggr_buf = 1;

<<<<<<< HEAD
				if (MP_TX_AGGR_PORT_LIMIT_REACHED(card) ||
=======
				if (mp_tx_aggr_port_limit_reached(card) ||
>>>>>>> refs/remotes/origin/master
				    !(card->mp_wr_bitmap &
				      (1 << card->curr_wr_port)))
					f_send_cur_buf = 1;
				else
					f_postcopy_cur_buf = 1;
			}
		} else {
<<<<<<< HEAD
<<<<<<< HEAD
			if (MP_TX_AGGR_BUF_HAS_ROOM(card, pkt_len)
			    && (card->mp_wr_bitmap & (1 << card->curr_wr_port)))
=======
			if (MP_TX_AGGR_BUF_HAS_ROOM(card, pkt_len) &&
			    (card->mp_wr_bitmap & (1 << card->curr_wr_port)))
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (MP_TX_AGGR_BUF_HAS_ROOM(card, pkt_len) &&
			    (card->mp_wr_bitmap & (1 << card->curr_wr_port)))
>>>>>>> refs/remotes/origin/master
				f_precopy_cur_buf = 1;
			else
				f_send_cur_buf = 1;
		}
	} else {
		/* Last pkt in TX queue */
		dev_dbg(adapter->dev, "info: %s: Last packet in Tx Queue.\n",
<<<<<<< HEAD
<<<<<<< HEAD
						__func__);
=======
			__func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			__func__);
>>>>>>> refs/remotes/origin/master

		if (MP_TX_AGGR_IN_PROGRESS(card)) {
			/* some packs in Aggr buf already */
			f_send_aggr_buf = 1;

			if (MP_TX_AGGR_BUF_HAS_ROOM(card, pkt_len))
				f_precopy_cur_buf = 1;
			else
				/* No room in Aggr buf, send it */
				f_send_cur_buf = 1;
		} else {
			f_send_cur_buf = 1;
		}
	}

	if (f_precopy_cur_buf) {
		dev_dbg(adapter->dev, "data: %s: precopy current buffer\n",
<<<<<<< HEAD
<<<<<<< HEAD
						__func__);
=======
			__func__);
>>>>>>> refs/remotes/origin/cm-10.0
		MP_TX_AGGR_BUF_PUT(card, payload, pkt_len, port);

		if (MP_TX_AGGR_PKT_LIMIT_REACHED(card) ||
		    MP_TX_AGGR_PORT_LIMIT_REACHED(card))
=======
			__func__);
		MP_TX_AGGR_BUF_PUT(card, payload, pkt_len, port);

		if (MP_TX_AGGR_PKT_LIMIT_REACHED(card) ||
		    mp_tx_aggr_port_limit_reached(card))
>>>>>>> refs/remotes/origin/master
			/* No more pkts allowed in Aggr buf, send it */
			f_send_aggr_buf = 1;
	}

	if (f_send_aggr_buf) {
		dev_dbg(adapter->dev, "data: %s: send aggr buffer: %d %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
				__func__,
=======
			__func__,
>>>>>>> refs/remotes/origin/cm-10.0
				card->mpa_tx.start_port, card->mpa_tx.ports);
		ret = mwifiex_write_data_to_card(adapter, card->mpa_tx.buf,
						 card->mpa_tx.buf_len,
						 (adapter->ioport | 0x1000 |
						 (card->mpa_tx.ports << 4)) +
						  card->mpa_tx.start_port);
=======
			__func__,
				card->mpa_tx.start_port, card->mpa_tx.ports);
		if (card->supports_sdio_new_mode) {
			u32 port_count;
			int i;

			for (i = 0, port_count = 0; i < card->max_ports; i++)
				if (card->mpa_tx.ports & BIT(i))
					port_count++;

			/* Writing data from "start_port + 0" to "start_port +
			 * port_count -1", so decrease the count by 1
			 */
			port_count--;
			mport = (adapter->ioport | SDIO_MPA_ADDR_BASE |
				 (port_count << 8)) + card->mpa_tx.start_port;
		} else {
			mport = (adapter->ioport | SDIO_MPA_ADDR_BASE |
				 (card->mpa_tx.ports << 4)) +
				 card->mpa_tx.start_port;
		}

		ret = mwifiex_write_data_to_card(adapter, card->mpa_tx.buf,
						 card->mpa_tx.buf_len, mport);
>>>>>>> refs/remotes/origin/master

		MP_TX_AGGR_BUF_RESET(card);
	}

tx_curr_single:
	if (f_send_cur_buf) {
		dev_dbg(adapter->dev, "data: %s: send current buffer %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
						__func__, port);
=======
			__func__, port);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			__func__, port);
>>>>>>> refs/remotes/origin/master
		ret = mwifiex_write_data_to_card(adapter, payload, pkt_len,
						 adapter->ioport + port);
	}

	if (f_postcopy_cur_buf) {
		dev_dbg(adapter->dev, "data: %s: postcopy current buffer\n",
<<<<<<< HEAD
<<<<<<< HEAD
						__func__);
=======
			__func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			__func__);
>>>>>>> refs/remotes/origin/master
		MP_TX_AGGR_BUF_PUT(card, payload, pkt_len, port);
	}

	return ret;
}

/*
 * This function downloads data from driver to card.
 *
 * Both commands and data packets are transferred to the card by this
 * function.
 *
 * This function adds the SDIO specific header to the front of the buffer
 * before transferring. The header contains the length of the packet and
 * the type. The firmware handles the packets based upon this set type.
 */
static int mwifiex_sdio_host_to_card(struct mwifiex_adapter *adapter,
<<<<<<< HEAD
<<<<<<< HEAD
				     u8 type, u8 *payload, u32 pkt_len,
=======
				     u8 type, struct sk_buff *skb,
>>>>>>> refs/remotes/origin/cm-10.0
=======
				     u8 type, struct sk_buff *skb,
>>>>>>> refs/remotes/origin/master
				     struct mwifiex_tx_param *tx_param)
{
	struct sdio_mmc_card *card = adapter->card;
	int ret;
	u32 buf_block_len;
	u32 blk_size;
<<<<<<< HEAD
	u8 port = CTRL_PORT;
<<<<<<< HEAD
=======
	u8 *payload = (u8 *)skb->data;
	u32 pkt_len = skb->len;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 port = CTRL_PORT;
	u8 *payload = (u8 *)skb->data;
	u32 pkt_len = skb->len;
>>>>>>> refs/remotes/origin/master

	/* Allocate buffer and copy payload */
	blk_size = MWIFIEX_SDIO_BLOCK_SIZE;
	buf_block_len = (pkt_len + blk_size - 1) / blk_size;
	*(__le16 *)&payload[0] = cpu_to_le16((u16)pkt_len);
	*(__le16 *)&payload[2] = cpu_to_le16(type);

	/*
	 * This is SDIO specific header
	 *  u16 length,
	 *  u16 type (MWIFIEX_TYPE_DATA = 0, MWIFIEX_TYPE_CMD = 1,
	 *  MWIFIEX_TYPE_EVENT = 3)
	 */
	if (type == MWIFIEX_TYPE_DATA) {
		ret = mwifiex_get_wr_port_data(adapter, &port);
		if (ret) {
			dev_err(adapter->dev, "%s: no wr_port available\n",
<<<<<<< HEAD
<<<<<<< HEAD
						__func__);
=======
				__func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				__func__);
>>>>>>> refs/remotes/origin/master
			return ret;
		}
	} else {
		adapter->cmd_sent = true;
		/* Type must be MWIFIEX_TYPE_CMD */

		if (pkt_len <= INTF_HEADER_LEN ||
		    pkt_len > MWIFIEX_UPLD_SIZE)
			dev_err(adapter->dev, "%s: payload=%p, nb=%d\n",
<<<<<<< HEAD
<<<<<<< HEAD
					__func__, payload, pkt_len);
=======
				__func__, payload, pkt_len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				__func__, payload, pkt_len);

		if (card->supports_sdio_new_mode)
			port = CMD_PORT_SLCT;
>>>>>>> refs/remotes/origin/master
	}

	/* Transfer data to card */
	pkt_len = buf_block_len * blk_size;

	if (tx_param)
		ret = mwifiex_host_to_card_mp_aggr(adapter, payload, pkt_len,
<<<<<<< HEAD
<<<<<<< HEAD
				port, tx_param->next_pkt_len);
	else
		ret = mwifiex_host_to_card_mp_aggr(adapter, payload, pkt_len,
				port, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
						   port, tx_param->next_pkt_len
						   );
	else
		ret = mwifiex_host_to_card_mp_aggr(adapter, payload, pkt_len,
						   port, 0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (ret) {
		if (type == MWIFIEX_TYPE_CMD)
			adapter->cmd_sent = false;
		if (type == MWIFIEX_TYPE_DATA)
			adapter->data_sent = false;
	} else {
		if (type == MWIFIEX_TYPE_DATA) {
			if (!(card->mp_wr_bitmap & (1 << card->curr_wr_port)))
				adapter->data_sent = true;
			else
				adapter->data_sent = false;
		}
	}

	return ret;
}

/*
 * This function allocates the MPA Tx and Rx buffers.
 */
static int mwifiex_alloc_sdio_mpa_buffers(struct mwifiex_adapter *adapter,
				   u32 mpa_tx_buf_size, u32 mpa_rx_buf_size)
{
	struct sdio_mmc_card *card = adapter->card;
	int ret = 0;

	card->mpa_tx.buf = kzalloc(mpa_tx_buf_size, GFP_KERNEL);
	if (!card->mpa_tx.buf) {
<<<<<<< HEAD
		dev_err(adapter->dev, "could not alloc buffer for MP-A TX\n");
=======
>>>>>>> refs/remotes/origin/master
		ret = -1;
		goto error;
	}

	card->mpa_tx.buf_size = mpa_tx_buf_size;

	card->mpa_rx.buf = kzalloc(mpa_rx_buf_size, GFP_KERNEL);
	if (!card->mpa_rx.buf) {
<<<<<<< HEAD
		dev_err(adapter->dev, "could not alloc buffer for MP-A RX\n");
=======
>>>>>>> refs/remotes/origin/master
		ret = -1;
		goto error;
	}

	card->mpa_rx.buf_size = mpa_rx_buf_size;

error:
	if (ret) {
		kfree(card->mpa_tx.buf);
		kfree(card->mpa_rx.buf);
	}

	return ret;
}

/*
 * This function unregisters the SDIO device.
 *
 * The SDIO IRQ is released, the function is disabled and driver
 * data is set to null.
 */
static void
mwifiex_unregister_dev(struct mwifiex_adapter *adapter)
{
	struct sdio_mmc_card *card = adapter->card;

	if (adapter->card) {
<<<<<<< HEAD
		/* Release the SDIO IRQ */
		sdio_claim_host(card->func);
		sdio_release_irq(card->func);
		sdio_disable_func(card->func);
		sdio_release_host(card->func);
		sdio_set_drvdata(card->func, NULL);
=======
		sdio_claim_host(card->func);
		sdio_disable_func(card->func);
		sdio_release_host(card->func);
>>>>>>> refs/remotes/origin/master
	}
}

/*
 * This function registers the SDIO device.
 *
 * SDIO IRQ is claimed, block size is set and driver data is initialized.
 */
static int mwifiex_register_dev(struct mwifiex_adapter *adapter)
{
<<<<<<< HEAD
	int ret = 0;
=======
	int ret;
>>>>>>> refs/remotes/origin/master
	struct sdio_mmc_card *card = adapter->card;
	struct sdio_func *func = card->func;

	/* save adapter pointer in card */
	card->adapter = adapter;

	sdio_claim_host(func);

<<<<<<< HEAD
	/* Request the SDIO IRQ */
	ret = sdio_claim_irq(func, mwifiex_sdio_interrupt);
	if (ret) {
		pr_err("claim irq failed: ret=%d\n", ret);
		goto disable_func;
	}

	/* Set block size */
	ret = sdio_set_block_size(card->func, MWIFIEX_SDIO_BLOCK_SIZE);
	if (ret) {
		pr_err("cannot set SDIO block size\n");
		ret = -1;
		goto release_irq;
	}

	sdio_release_host(func);
	sdio_set_drvdata(func, card);

	adapter->dev = &func->dev;

<<<<<<< HEAD
=======
	switch (func->device) {
	case SDIO_DEVICE_ID_MARVELL_8797:
		strcpy(adapter->fw_name, SD8797_DEFAULT_FW_NAME);
		break;
	case SDIO_DEVICE_ID_MARVELL_8787:
	default:
		strcpy(adapter->fw_name, SD8787_DEFAULT_FW_NAME);
		break;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;

release_irq:
	sdio_release_irq(func);
disable_func:
	sdio_disable_func(func);
	sdio_release_host(func);
	adapter->card = NULL;

	return -1;
=======
	/* Set block size */
	ret = sdio_set_block_size(card->func, MWIFIEX_SDIO_BLOCK_SIZE);
	sdio_release_host(func);
	if (ret) {
		pr_err("cannot set SDIO block size\n");
		return ret;
	}


	adapter->dev = &func->dev;

	strcpy(adapter->fw_name, card->firmware);

	return 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * This function initializes the SDIO driver.
 *
 * The following initializations steps are followed -
 *      - Read the Host interrupt status register to acknowledge
 *        the first interrupt got from bootloader
 *      - Disable host interrupt mask register
 *      - Get SDIO port
<<<<<<< HEAD
<<<<<<< HEAD
 *      - Get revision ID
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *      - Initialize SDIO variables in card
 *      - Allocate MP registers
 *      - Allocate MPA Tx and Rx buffers
 */
static int mwifiex_init_sdio(struct mwifiex_adapter *adapter)
{
	struct sdio_mmc_card *card = adapter->card;
<<<<<<< HEAD
	int ret;
	u32 sdio_ireg;
=======
	const struct mwifiex_sdio_card_reg *reg = card->reg;
	int ret;
	u8 sdio_ireg;

	sdio_set_drvdata(card->func, card);
>>>>>>> refs/remotes/origin/master

	/*
	 * Read the HOST_INT_STATUS_REG for ACK the first interrupt got
	 * from the bootloader. If we don't do this we get a interrupt
	 * as soon as we register the irq.
	 */
	mwifiex_read_reg(adapter, HOST_INTSTATUS_REG, &sdio_ireg);

<<<<<<< HEAD
	/* Disable host interrupt mask register for SDIO */
	mwifiex_sdio_disable_host_int(adapter);

	/* Get SDIO ioport */
	mwifiex_init_sdio_ioport(adapter);

<<<<<<< HEAD
	/* Get revision ID */
#define REV_ID_REG	0x5c
	mwifiex_read_reg(adapter, REV_ID_REG, &adapter->revision_id);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* Initialize SDIO variables in card */
	card->mp_rd_bitmap = 0;
	card->mp_wr_bitmap = 0;
	card->curr_rd_port = 1;
	card->curr_wr_port = 1;

	card->mp_data_port_mask = DATA_PORT_MASK;
=======
	/* Get SDIO ioport */
	mwifiex_init_sdio_ioport(adapter);

	/* Initialize SDIO variables in card */
	card->mp_rd_bitmap = 0;
	card->mp_wr_bitmap = 0;
	card->curr_rd_port = reg->start_rd_port;
	card->curr_wr_port = reg->start_wr_port;

	card->mp_data_port_mask = reg->data_port_mask;
>>>>>>> refs/remotes/origin/master

	card->mpa_tx.buf_len = 0;
	card->mpa_tx.pkt_cnt = 0;
	card->mpa_tx.start_port = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	card->mpa_tx.enabled = 0;
=======
	card->mpa_tx.enabled = 1;
>>>>>>> refs/remotes/origin/cm-10.0
	card->mpa_tx.pkt_aggr_limit = SDIO_MP_AGGR_DEF_PKT_LIMIT;
=======
	card->mpa_tx.enabled = 1;
	card->mpa_tx.pkt_aggr_limit = card->mp_agg_pkt_limit;
>>>>>>> refs/remotes/origin/master

	card->mpa_rx.buf_len = 0;
	card->mpa_rx.pkt_cnt = 0;
	card->mpa_rx.start_port = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	card->mpa_rx.enabled = 0;
=======
	card->mpa_rx.enabled = 1;
>>>>>>> refs/remotes/origin/cm-10.0
	card->mpa_rx.pkt_aggr_limit = SDIO_MP_AGGR_DEF_PKT_LIMIT;

	/* Allocate buffers for SDIO MP-A */
	card->mp_regs = kzalloc(MAX_MP_REGS, GFP_KERNEL);
	if (!card->mp_regs) {
		dev_err(adapter->dev, "failed to alloc mp_regs\n");
		return -ENOMEM;
	}

=======
	card->mpa_rx.enabled = 1;
	card->mpa_rx.pkt_aggr_limit = card->mp_agg_pkt_limit;

	/* Allocate buffers for SDIO MP-A */
	card->mp_regs = kzalloc(reg->max_mp_regs, GFP_KERNEL);
	if (!card->mp_regs)
		return -ENOMEM;

	/* Allocate skb pointer buffers */
	card->mpa_rx.skb_arr = kzalloc((sizeof(void *)) *
				       card->mp_agg_pkt_limit, GFP_KERNEL);
	card->mpa_rx.len_arr = kzalloc(sizeof(*card->mpa_rx.len_arr) *
				       card->mp_agg_pkt_limit, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	ret = mwifiex_alloc_sdio_mpa_buffers(adapter,
					     SDIO_MP_TX_AGGR_DEF_BUF_SIZE,
					     SDIO_MP_RX_AGGR_DEF_BUF_SIZE);
	if (ret) {
		dev_err(adapter->dev, "failed to alloc sdio mp-a buffers\n");
		kfree(card->mp_regs);
		return -1;
	}

	return ret;
}

/*
 * This function resets the MPA Tx and Rx buffers.
 */
static void mwifiex_cleanup_mpa_buf(struct mwifiex_adapter *adapter)
{
	struct sdio_mmc_card *card = adapter->card;

	MP_TX_AGGR_BUF_RESET(card);
	MP_RX_AGGR_BUF_RESET(card);
}

/*
 * This function cleans up the allocated card buffers.
 *
 * The following are freed by this function -
 *      - MP registers
 *      - MPA Tx buffer
 *      - MPA Rx buffer
 */
static void mwifiex_cleanup_sdio(struct mwifiex_adapter *adapter)
{
	struct sdio_mmc_card *card = adapter->card;

	kfree(card->mp_regs);
<<<<<<< HEAD
	kfree(card->mpa_tx.buf);
	kfree(card->mpa_rx.buf);
=======
	kfree(card->mpa_rx.skb_arr);
	kfree(card->mpa_rx.len_arr);
	kfree(card->mpa_tx.buf);
	kfree(card->mpa_rx.buf);
	sdio_set_drvdata(card->func, NULL);
	kfree(card);
>>>>>>> refs/remotes/origin/master
}

/*
 * This function updates the MP end port in card.
 */
static void
mwifiex_update_mp_end_port(struct mwifiex_adapter *adapter, u16 port)
{
	struct sdio_mmc_card *card = adapter->card;
<<<<<<< HEAD
=======
	const struct mwifiex_sdio_card_reg *reg = card->reg;
>>>>>>> refs/remotes/origin/master
	int i;

	card->mp_end_port = port;

<<<<<<< HEAD
	card->mp_data_port_mask = DATA_PORT_MASK;

	for (i = 1; i <= MAX_PORT - card->mp_end_port; i++)
		card->mp_data_port_mask &= ~(1 << (MAX_PORT - i));

	card->curr_wr_port = 1;

	dev_dbg(adapter->dev, "cmd: mp_end_port %d, data port mask 0x%x\n",
<<<<<<< HEAD
	       port, card->mp_data_port_mask);
=======
		port, card->mp_data_port_mask);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	card->mp_data_port_mask = reg->data_port_mask;

	if (reg->start_wr_port) {
		for (i = 1; i <= card->max_ports - card->mp_end_port; i++)
			card->mp_data_port_mask &=
					~(1 << (card->max_ports - i));
	}

	card->curr_wr_port = reg->start_wr_port;

	dev_dbg(adapter->dev, "cmd: mp_end_port %d, data port mask 0x%x\n",
		port, card->mp_data_port_mask);
}

static struct mmc_host *reset_host;
static void sdio_card_reset_worker(struct work_struct *work)
{
	struct mmc_host *target = reset_host;

	/* The actual reset operation must be run outside of driver thread.
	 * This is because mmc_remove_host() will cause the device to be
	 * instantly destroyed, and the driver then needs to end its thread,
	 * leading to a deadlock.
	 *
	 * We run it in a totally independent workqueue.
	 */

	pr_err("Resetting card...\n");
	mmc_remove_host(target);
	/* 20ms delay is based on experiment with sdhci controller */
	mdelay(20);
	mmc_add_host(target);
}
static DECLARE_WORK(card_reset_work, sdio_card_reset_worker);

/* This function resets the card */
static void mwifiex_sdio_card_reset(struct mwifiex_adapter *adapter)
{
	struct sdio_mmc_card *card = adapter->card;

	reset_host = card->func->card->host;
	schedule_work(&card_reset_work);
>>>>>>> refs/remotes/origin/master
}

static struct mwifiex_if_ops sdio_ops = {
	.init_if = mwifiex_init_sdio,
	.cleanup_if = mwifiex_cleanup_sdio,
	.check_fw_status = mwifiex_check_fw_status,
	.prog_fw = mwifiex_prog_fw_w_helper,
	.register_dev = mwifiex_register_dev,
	.unregister_dev = mwifiex_unregister_dev,
	.enable_int = mwifiex_sdio_enable_host_int,
<<<<<<< HEAD
=======
	.disable_int = mwifiex_sdio_disable_host_int,
>>>>>>> refs/remotes/origin/master
	.process_int_status = mwifiex_process_int_status,
	.host_to_card = mwifiex_sdio_host_to_card,
	.wakeup = mwifiex_pm_wakeup_card,
	.wakeup_complete = mwifiex_pm_wakeup_card_complete,

	/* SDIO specific */
	.update_mp_end_port = mwifiex_update_mp_end_port,
	.cleanup_mpa_buf = mwifiex_cleanup_mpa_buf,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.cmdrsp_complete = mwifiex_sdio_cmdrsp_complete,
	.event_complete = mwifiex_sdio_event_complete,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.cmdrsp_complete = mwifiex_sdio_cmdrsp_complete,
	.event_complete = mwifiex_sdio_event_complete,
	.card_reset = mwifiex_sdio_card_reset,
>>>>>>> refs/remotes/origin/master
};

/*
 * This function initializes the SDIO driver.
 *
 * This initiates the semaphore and registers the device with
 * SDIO bus.
 */
static int
mwifiex_sdio_init_module(void)
{
	sema_init(&add_remove_card_sem, 1);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* Clear the flag in case user removes the card. */
	user_rmmod = 0;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Clear the flag in case user removes the card. */
	user_rmmod = 0;

>>>>>>> refs/remotes/origin/master
	return sdio_register_driver(&mwifiex_sdio);
}

/*
 * This function cleans up the SDIO driver.
 *
 * The following major steps are followed for cleanup -
 *      - Resume the device if its suspended
 *      - Disconnect the device if connected
 *      - Shutdown the firmware
 *      - Unregister the device from SDIO bus.
 */
static void
mwifiex_sdio_cleanup_module(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct mwifiex_adapter *adapter = g_adapter;
	int i;

	if (down_interruptible(&add_remove_card_sem))
		goto exit_sem_err;

	if (!adapter || !adapter->priv_num)
		goto exit;

	if (adapter->is_suspended)
		mwifiex_sdio_resume(adapter->dev);

	for (i = 0; i < adapter->priv_num; i++)
		if ((GET_BSS_ROLE(adapter->priv[i]) == MWIFIEX_BSS_ROLE_STA) &&
		    adapter->priv[i]->media_connected)
			mwifiex_deauthenticate(adapter->priv[i], NULL);

	if (!adapter->surprise_removed)
		mwifiex_init_shutdown_fw(mwifiex_get_priv(adapter,
							  MWIFIEX_BSS_ROLE_ANY),
					 MWIFIEX_FUNC_SHUTDOWN);

exit:
	up(&add_remove_card_sem);

exit_sem_err:
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!down_interruptible(&add_remove_card_sem))
		up(&add_remove_card_sem);

	/* Set the flag as user is removing this module. */
	user_rmmod = 1;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cancel_work_sync(&card_reset_work);
>>>>>>> refs/remotes/origin/master
	sdio_unregister_driver(&mwifiex_sdio);
}

module_init(mwifiex_sdio_init_module);
module_exit(mwifiex_sdio_cleanup_module);

MODULE_AUTHOR("Marvell International Ltd.");
MODULE_DESCRIPTION("Marvell WiFi-Ex SDIO Driver version " SDIO_VERSION);
MODULE_VERSION(SDIO_VERSION);
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
<<<<<<< HEAD
MODULE_FIRMWARE("sd8787.bin");
=======
MODULE_FIRMWARE(SD8787_DEFAULT_FW_NAME);
MODULE_FIRMWARE(SD8797_DEFAULT_FW_NAME);
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_FIRMWARE(SD8786_DEFAULT_FW_NAME);
MODULE_FIRMWARE(SD8787_DEFAULT_FW_NAME);
MODULE_FIRMWARE(SD8797_DEFAULT_FW_NAME);
MODULE_FIRMWARE(SD8897_DEFAULT_FW_NAME);
>>>>>>> refs/remotes/origin/master
