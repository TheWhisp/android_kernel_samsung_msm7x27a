/*
    Copyright (c) 1998 - 2002  Frodo Looijaard <frodol@dds.nl>,
    Philip Edelbrock <phil@netroedge.com>, and Mark D. Studebaker
    <mdsxyz123@yahoo.com>
<<<<<<< HEAD
<<<<<<< HEAD
    Copyright (C) 2007, 2008   Jean Delvare <khali@linux-fr.org>
=======
    Copyright (C) 2007 - 2012  Jean Delvare <khali@linux-fr.org>
>>>>>>> refs/remotes/origin/cm-10.0
=======
    Copyright (C) 2007 - 2012  Jean Delvare <khali@linux-fr.org>
>>>>>>> refs/remotes/origin/master
    Copyright (C) 2010         Intel Corporation,
                               David Woodhouse <dwmw2@infradead.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*
  Supports the following Intel I/O Controller Hubs (ICH):

                                  I/O                     Block   I2C
                                  region  SMBus   Block   proc.   block
  Chip name             PCI ID    size    PEC     buffer  call    read
  ----------------------------------------------------------------------
  82801AA  (ICH)        0x2413     16      no      no      no      no
  82801AB  (ICH0)       0x2423     16      no      no      no      no
  82801BA  (ICH2)       0x2443     16      no      no      no      no
  82801CA  (ICH3)       0x2483     32     soft     no      no      no
  82801DB  (ICH4)       0x24c3     32     hard     yes     no      no
  82801E   (ICH5)       0x24d3     32     hard     yes     yes     yes
  6300ESB               0x25a4     32     hard     yes     yes     yes
  82801F   (ICH6)       0x266a     32     hard     yes     yes     yes
  6310ESB/6320ESB       0x269b     32     hard     yes     yes     yes
  82801G   (ICH7)       0x27da     32     hard     yes     yes     yes
  82801H   (ICH8)       0x283e     32     hard     yes     yes     yes
  82801I   (ICH9)       0x2930     32     hard     yes     yes     yes
  EP80579 (Tolapai)     0x5032     32     hard     yes     yes     yes
  ICH10                 0x3a30     32     hard     yes     yes     yes
  ICH10                 0x3a60     32     hard     yes     yes     yes
  5/3400 Series (PCH)   0x3b30     32     hard     yes     yes     yes
  6 Series (PCH)        0x1c22     32     hard     yes     yes     yes
  Patsburg (PCH)        0x1d22     32     hard     yes     yes     yes
  Patsburg (PCH) IDF    0x1d70     32     hard     yes     yes     yes
  Patsburg (PCH) IDF    0x1d71     32     hard     yes     yes     yes
  Patsburg (PCH) IDF    0x1d72     32     hard     yes     yes     yes
  DH89xxCC (PCH)        0x2330     32     hard     yes     yes     yes
  Panther Point (PCH)   0x1e22     32     hard     yes     yes     yes
<<<<<<< HEAD
<<<<<<< HEAD
=======
  Lynx Point (PCH)      0x8c22     32     hard     yes     yes     yes
  Lynx Point-LP (PCH)   0x9c22     32     hard     yes     yes     yes
>>>>>>> refs/remotes/origin/cm-10.0
=======
  Lynx Point (PCH)      0x8c22     32     hard     yes     yes     yes
  Lynx Point-LP (PCH)   0x9c22     32     hard     yes     yes     yes
  Avoton (SOC)          0x1f3c     32     hard     yes     yes     yes
<<<<<<< HEAD
  Wellsburg (PCH)       0x8d22     32     hard     yes     yes     yes
  Wellsburg (PCH) MS    0x8d7d     32     hard     yes     yes     yes
  Wellsburg (PCH) MS    0x8d7e     32     hard     yes     yes     yes
  Wellsburg (PCH) MS    0x8d7f     32     hard     yes     yes     yes
  Coleto Creek (PCH)    0x23b0     32     hard     yes     yes     yes
  Wildcat Point-LP (PCH)   0x9ca2     32     hard     yes     yes     yes
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

  Features supported by this driver:
  Software PEC                     no
  Hardware PEC                     yes
  Block buffer                     yes
  Block process call transaction   no
  I2C block read transaction       yes  (doesn't use the block buffer)
  Slave mode                       no
<<<<<<< HEAD
=======
  Interrupt processing             yes
>>>>>>> refs/remotes/origin/master

  See the file Documentation/i2c/busses/i2c-i801 for details.
*/

<<<<<<< HEAD
=======
#include <linux/interrupt.h>
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/acpi.h>
#include <linux/io.h>
#include <linux/dmi.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/wait.h>
#include <linux/err.h>

#if (defined CONFIG_I2C_MUX_GPIO || defined CONFIG_I2C_MUX_GPIO_MODULE) && \
		defined CONFIG_DMI
#include <linux/gpio.h>
#include <linux/i2c-mux-gpio.h>
#include <linux/platform_device.h>
#endif
>>>>>>> refs/remotes/origin/master

/* I801 SMBus address offsets */
#define SMBHSTSTS(p)	(0 + (p)->smba)
#define SMBHSTCNT(p)	(2 + (p)->smba)
#define SMBHSTCMD(p)	(3 + (p)->smba)
#define SMBHSTADD(p)	(4 + (p)->smba)
#define SMBHSTDAT0(p)	(5 + (p)->smba)
#define SMBHSTDAT1(p)	(6 + (p)->smba)
#define SMBBLKDAT(p)	(7 + (p)->smba)
#define SMBPEC(p)	(8 + (p)->smba)		/* ICH3 and later */
#define SMBAUXSTS(p)	(12 + (p)->smba)	/* ICH4 and later */
#define SMBAUXCTL(p)	(13 + (p)->smba)	/* ICH4 and later */

/* PCI Address Constants */
#define SMBBAR		4
<<<<<<< HEAD
#define SMBHSTCFG	0x040

=======
#define SMBPCISTS	0x006
#define SMBHSTCFG	0x040

/* Host status bits for SMBPCISTS */
#define SMBPCISTS_INTS		0x08

>>>>>>> refs/remotes/origin/master
/* Host configuration bits for SMBHSTCFG */
#define SMBHSTCFG_HST_EN	1
#define SMBHSTCFG_SMB_SMI_EN	2
#define SMBHSTCFG_I2C_EN	4

/* Auxiliary control register bits, ICH4+ only */
#define SMBAUXCTL_CRC		1
#define SMBAUXCTL_E32B		2

<<<<<<< HEAD
/* kill bit for SMBHSTCNT */
#define SMBHSTCNT_KILL		2

/* Other settings */
<<<<<<< HEAD
#define MAX_TIMEOUT		100
=======
#define MAX_RETRIES		400
>>>>>>> refs/remotes/origin/cm-10.0
#define ENABLE_INT9		0	/* set to 0x01 to enable - untested */
=======
/* Other settings */
#define MAX_RETRIES		400
>>>>>>> refs/remotes/origin/master

/* I801 command constants */
#define I801_QUICK		0x00
#define I801_BYTE		0x04
#define I801_BYTE_DATA		0x08
#define I801_WORD_DATA		0x0C
#define I801_PROC_CALL		0x10	/* unimplemented */
#define I801_BLOCK_DATA		0x14
#define I801_I2C_BLOCK_DATA	0x18	/* ICH5 and later */
<<<<<<< HEAD
#define I801_BLOCK_LAST		0x34
#define I801_I2C_BLOCK_LAST	0x38	/* ICH5 and later */
#define I801_START		0x40
#define I801_PEC_EN		0x80	/* ICH3 and later */
=======

/* I801 Host Control register bits */
#define SMBHSTCNT_INTREN	0x01
#define SMBHSTCNT_KILL		0x02
#define SMBHSTCNT_LAST_BYTE	0x20
#define SMBHSTCNT_START		0x40
#define SMBHSTCNT_PEC_EN	0x80	/* ICH3 and later */
>>>>>>> refs/remotes/origin/master

/* I801 Hosts Status register bits */
#define SMBHSTSTS_BYTE_DONE	0x80
#define SMBHSTSTS_INUSE_STS	0x40
#define SMBHSTSTS_SMBALERT_STS	0x20
#define SMBHSTSTS_FAILED	0x10
#define SMBHSTSTS_BUS_ERR	0x08
#define SMBHSTSTS_DEV_ERR	0x04
#define SMBHSTSTS_INTR		0x02
#define SMBHSTSTS_HOST_BUSY	0x01

<<<<<<< HEAD
#define STATUS_FLAGS		(SMBHSTSTS_BYTE_DONE | SMBHSTSTS_FAILED | \
				 SMBHSTSTS_BUS_ERR | SMBHSTSTS_DEV_ERR | \
				 SMBHSTSTS_INTR)
=======
#define STATUS_ERROR_FLAGS	(SMBHSTSTS_FAILED | SMBHSTSTS_BUS_ERR | \
				 SMBHSTSTS_DEV_ERR)

#define STATUS_FLAGS		(SMBHSTSTS_BYTE_DONE | SMBHSTSTS_INTR | \
				 STATUS_ERROR_FLAGS)
>>>>>>> refs/remotes/origin/master

/* Older devices have their ID defined in <linux/pci_ids.h> */
#define PCI_DEVICE_ID_INTEL_COUGARPOINT_SMBUS	0x1c22
#define PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS	0x1d22
/* Patsburg also has three 'Integrated Device Function' SMBus controllers */
#define PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS_IDF0	0x1d70
#define PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS_IDF1	0x1d71
#define PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS_IDF2	0x1d72
#define PCI_DEVICE_ID_INTEL_PANTHERPOINT_SMBUS	0x1e22
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define PCI_DEVICE_ID_INTEL_AVOTON_SMBUS	0x1f3c
>>>>>>> refs/remotes/origin/cm-11.0
#define PCI_DEVICE_ID_INTEL_DH89XXCC_SMBUS	0x2330
#define PCI_DEVICE_ID_INTEL_5_3400_SERIES_SMBUS	0x3b30
<<<<<<< HEAD
=======
#define PCI_DEVICE_ID_INTEL_LYNXPOINT_SMBUS	0x8c22
#define PCI_DEVICE_ID_INTEL_LYNXPOINT_LP_SMBUS	0x9c22
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define PCI_DEVICE_ID_INTEL_AVOTON_SMBUS	0x1f3c
#define PCI_DEVICE_ID_INTEL_DH89XXCC_SMBUS	0x2330
#define PCI_DEVICE_ID_INTEL_COLETOCREEK_SMBUS	0x23b0
#define PCI_DEVICE_ID_INTEL_5_3400_SERIES_SMBUS	0x3b30
#define PCI_DEVICE_ID_INTEL_LYNXPOINT_SMBUS	0x8c22
#define PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS	0x8d22
#define PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS_MS0	0x8d7d
#define PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS_MS1	0x8d7e
#define PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS_MS2	0x8d7f
#define PCI_DEVICE_ID_INTEL_LYNXPOINT_LP_SMBUS	0x9c22
#define PCI_DEVICE_ID_INTEL_WILDCATPOINT_LP_SMBUS	0x9ca2

struct i801_mux_config {
	char *gpio_chip;
	unsigned values[3];
	int n_values;
	unsigned classes[3];
	unsigned gpios[2];		/* Relative to gpio_chip->base */
	int n_gpios;
};
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

struct i801_priv {
	struct i2c_adapter adapter;
	unsigned long smba;
	unsigned char original_hstcfg;
	struct pci_dev *pci_dev;
	unsigned int features;
<<<<<<< HEAD
=======

	/* isr processing */
	wait_queue_head_t waitq;
	u8 status;

	/* Command state used by isr for byte-by-byte block transactions */
	u8 cmd;
	bool is_read;
	int count;
	int len;
	u8 *data;

#if (defined CONFIG_I2C_MUX_GPIO || defined CONFIG_I2C_MUX_GPIO_MODULE) && \
		defined CONFIG_DMI
	const struct i801_mux_config *mux_drvdata;
	struct platform_device *mux_pdev;
#endif
>>>>>>> refs/remotes/origin/master
};

static struct pci_driver i801_driver;

#define FEATURE_SMBUS_PEC	(1 << 0)
#define FEATURE_BLOCK_BUFFER	(1 << 1)
#define FEATURE_BLOCK_PROC	(1 << 2)
#define FEATURE_I2C_BLOCK_READ	(1 << 3)
<<<<<<< HEAD
=======
#define FEATURE_IRQ		(1 << 4)
>>>>>>> refs/remotes/origin/master
/* Not really a feature, but it's convenient to handle it as such */
#define FEATURE_IDF		(1 << 15)

static const char *i801_feature_names[] = {
	"SMBus PEC",
	"Block buffer",
	"Block process call",
	"I2C block read",
<<<<<<< HEAD
=======
	"Interrupt",
>>>>>>> refs/remotes/origin/master
};

static unsigned int disable_features;
module_param(disable_features, uint, S_IRUGO | S_IWUSR);
<<<<<<< HEAD
MODULE_PARM_DESC(disable_features, "Disable selected driver features");
=======
MODULE_PARM_DESC(disable_features, "Disable selected driver features:\n"
	"\t\t  0x01  disable SMBus PEC\n"
	"\t\t  0x02  disable the block buffer\n"
	"\t\t  0x08  disable the I2C block read functionality\n"
	"\t\t  0x10  don't use interrupts ");
>>>>>>> refs/remotes/origin/master

/* Make sure the SMBus host is ready to start transmitting.
   Return 0 if it is, -EBUSY if it is not. */
static int i801_check_pre(struct i801_priv *priv)
{
	int status;

	status = inb_p(SMBHSTSTS(priv));
	if (status & SMBHSTSTS_HOST_BUSY) {
		dev_err(&priv->pci_dev->dev, "SMBus is busy, can't use it!\n");
		return -EBUSY;
	}

	status &= STATUS_FLAGS;
	if (status) {
		dev_dbg(&priv->pci_dev->dev, "Clearing status flags (%02x)\n",
			status);
		outb_p(status, SMBHSTSTS(priv));
		status = inb_p(SMBHSTSTS(priv)) & STATUS_FLAGS;
		if (status) {
			dev_err(&priv->pci_dev->dev,
				"Failed clearing status flags (%02x)\n",
				status);
			return -EBUSY;
		}
	}

	return 0;
}

<<<<<<< HEAD
/* Convert the status register to an error code, and clear it. */
static int i801_check_post(struct i801_priv *priv, int status, int timeout)
{
	int result = 0;

	/* If the SMBus is still busy, we give up */
	if (timeout) {
=======
/*
 * Convert the status register to an error code, and clear it.
 * Note that status only contains the bits we want to clear, not the
 * actual register value.
 */
static int i801_check_post(struct i801_priv *priv, int status)
{
	int result = 0;

	/*
	 * If the SMBus is still busy, we give up
	 * Note: This timeout condition only happens when using polling
	 * transactions.  For interrupt operation, NAK/timeout is indicated by
	 * DEV_ERR.
	 */
	if (unlikely(status < 0)) {
>>>>>>> refs/remotes/origin/master
		dev_err(&priv->pci_dev->dev, "Transaction timeout\n");
		/* try to stop the current command */
		dev_dbg(&priv->pci_dev->dev, "Terminating the current operation\n");
		outb_p(inb_p(SMBHSTCNT(priv)) | SMBHSTCNT_KILL,
		       SMBHSTCNT(priv));
<<<<<<< HEAD
<<<<<<< HEAD
		msleep(1);
=======
		usleep_range(1000, 2000);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		usleep_range(1000, 2000);
>>>>>>> refs/remotes/origin/master
		outb_p(inb_p(SMBHSTCNT(priv)) & (~SMBHSTCNT_KILL),
		       SMBHSTCNT(priv));

		/* Check if it worked */
		status = inb_p(SMBHSTSTS(priv));
		if ((status & SMBHSTSTS_HOST_BUSY) ||
		    !(status & SMBHSTSTS_FAILED))
			dev_err(&priv->pci_dev->dev,
				"Failed terminating the transaction\n");
		outb_p(STATUS_FLAGS, SMBHSTSTS(priv));
		return -ETIMEDOUT;
	}

	if (status & SMBHSTSTS_FAILED) {
		result = -EIO;
		dev_err(&priv->pci_dev->dev, "Transaction failed\n");
	}
	if (status & SMBHSTSTS_DEV_ERR) {
		result = -ENXIO;
		dev_dbg(&priv->pci_dev->dev, "No response\n");
	}
	if (status & SMBHSTSTS_BUS_ERR) {
		result = -EAGAIN;
		dev_dbg(&priv->pci_dev->dev, "Lost arbitration\n");
	}

<<<<<<< HEAD
	if (result) {
		/* Clear error flags */
		outb_p(status & STATUS_FLAGS, SMBHSTSTS(priv));
		status = inb_p(SMBHSTSTS(priv)) & STATUS_FLAGS;
		if (status) {
			dev_warn(&priv->pci_dev->dev, "Failed clearing status "
				 "flags at end of transaction (%02x)\n",
				 status);
		}
	}
=======
	/* Clear status flags except BYTE_DONE, to be cleared by caller */
	outb_p(status, SMBHSTSTS(priv));
>>>>>>> refs/remotes/origin/master

	return result;
}

<<<<<<< HEAD
static int i801_transaction(struct i801_priv *priv, int xact)
{
	int status;
	int result;
	int timeout = 0;

	result = i801_check_pre(priv);
	if (result < 0)
		return result;

	/* the current contents of SMBHSTCNT can be overwritten, since PEC,
	 * INTREN, SMBSCMD are passed in xact */
	outb_p(xact | I801_START, SMBHSTCNT(priv));

	/* We will always wait for a fraction of a second! */
	do {
<<<<<<< HEAD
		msleep(1);
		status = inb_p(SMBHSTSTS(priv));
	} while ((status & SMBHSTSTS_HOST_BUSY) && (timeout++ < MAX_TIMEOUT));

	result = i801_check_post(priv, status, timeout > MAX_TIMEOUT);
=======
		usleep_range(250, 500);
		status = inb_p(SMBHSTSTS(priv));
	} while ((status & SMBHSTSTS_HOST_BUSY) && (timeout++ < MAX_RETRIES));

	result = i801_check_post(priv, status, timeout > MAX_RETRIES);
>>>>>>> refs/remotes/origin/cm-10.0
	if (result < 0)
		return result;

	outb_p(SMBHSTSTS_INTR, SMBHSTSTS(priv));
	return 0;
}

/* wait for INTR bit as advised by Intel */
static void i801_wait_hwpec(struct i801_priv *priv)
=======
/* Wait for BUSY being cleared and either INTR or an error flag being set */
static int i801_wait_intr(struct i801_priv *priv)
{
	int timeout = 0;
	int status;

	/* We will always wait for a fraction of a second! */
	do {
		usleep_range(250, 500);
		status = inb_p(SMBHSTSTS(priv));
	} while (((status & SMBHSTSTS_HOST_BUSY) ||
		  !(status & (STATUS_ERROR_FLAGS | SMBHSTSTS_INTR))) &&
		 (timeout++ < MAX_RETRIES));

	if (timeout > MAX_RETRIES) {
		dev_dbg(&priv->pci_dev->dev, "INTR Timeout!\n");
		return -ETIMEDOUT;
	}
	return status & (STATUS_ERROR_FLAGS | SMBHSTSTS_INTR);
}

/* Wait for either BYTE_DONE or an error flag being set */
static int i801_wait_byte_done(struct i801_priv *priv)
>>>>>>> refs/remotes/origin/master
{
	int timeout = 0;
	int status;

<<<<<<< HEAD
	do {
<<<<<<< HEAD
		msleep(1);
		status = inb_p(SMBHSTSTS(priv));
	} while ((!(status & SMBHSTSTS_INTR))
		 && (timeout++ < MAX_TIMEOUT));

	if (timeout > MAX_TIMEOUT)
=======
		usleep_range(250, 500);
		status = inb_p(SMBHSTSTS(priv));
	} while ((!(status & SMBHSTSTS_INTR))
		 && (timeout++ < MAX_RETRIES));

	if (timeout > MAX_RETRIES)
>>>>>>> refs/remotes/origin/cm-10.0
		dev_dbg(&priv->pci_dev->dev, "PEC Timeout!\n");

	outb_p(status, SMBHSTSTS(priv));
=======
	/* We will always wait for a fraction of a second! */
	do {
		usleep_range(250, 500);
		status = inb_p(SMBHSTSTS(priv));
	} while (!(status & (STATUS_ERROR_FLAGS | SMBHSTSTS_BYTE_DONE)) &&
		 (timeout++ < MAX_RETRIES));

	if (timeout > MAX_RETRIES) {
		dev_dbg(&priv->pci_dev->dev, "BYTE_DONE Timeout!\n");
		return -ETIMEDOUT;
	}
	return status & STATUS_ERROR_FLAGS;
}

static int i801_transaction(struct i801_priv *priv, int xact)
{
	int status;
	int result;

	result = i801_check_pre(priv);
	if (result < 0)
		return result;

	if (priv->features & FEATURE_IRQ) {
		outb_p(xact | SMBHSTCNT_INTREN | SMBHSTCNT_START,
		       SMBHSTCNT(priv));
		wait_event(priv->waitq, (status = priv->status));
		priv->status = 0;
		return i801_check_post(priv, status);
	}

	/* the current contents of SMBHSTCNT can be overwritten, since PEC,
	 * SMBSCMD are passed in xact */
	outb_p(xact | SMBHSTCNT_START, SMBHSTCNT(priv));

	status = i801_wait_intr(priv);
	return i801_check_post(priv, status);
>>>>>>> refs/remotes/origin/master
}

static int i801_block_transaction_by_block(struct i801_priv *priv,
					   union i2c_smbus_data *data,
					   char read_write, int hwpec)
{
	int i, len;
	int status;

	inb_p(SMBHSTCNT(priv)); /* reset the data buffer index */

	/* Use 32-byte buffer to process this transaction */
	if (read_write == I2C_SMBUS_WRITE) {
		len = data->block[0];
		outb_p(len, SMBHSTDAT0(priv));
		for (i = 0; i < len; i++)
			outb_p(data->block[i+1], SMBBLKDAT(priv));
	}

<<<<<<< HEAD
	status = i801_transaction(priv, I801_BLOCK_DATA | ENABLE_INT9 |
				  I801_PEC_EN * hwpec);
=======
	status = i801_transaction(priv, I801_BLOCK_DATA |
				  (hwpec ? SMBHSTCNT_PEC_EN : 0));
>>>>>>> refs/remotes/origin/master
	if (status)
		return status;

	if (read_write == I2C_SMBUS_READ) {
		len = inb_p(SMBHSTDAT0(priv));
		if (len < 1 || len > I2C_SMBUS_BLOCK_MAX)
			return -EPROTO;

		data->block[0] = len;
		for (i = 0; i < len; i++)
			data->block[i + 1] = inb_p(SMBBLKDAT(priv));
	}
	return 0;
}

<<<<<<< HEAD
=======
static void i801_isr_byte_done(struct i801_priv *priv)
{
	if (priv->is_read) {
		/* For SMBus block reads, length is received with first byte */
		if (((priv->cmd & 0x1c) == I801_BLOCK_DATA) &&
		    (priv->count == 0)) {
			priv->len = inb_p(SMBHSTDAT0(priv));
			if (priv->len < 1 || priv->len > I2C_SMBUS_BLOCK_MAX) {
				dev_err(&priv->pci_dev->dev,
					"Illegal SMBus block read size %d\n",
					priv->len);
				/* FIXME: Recover */
				priv->len = I2C_SMBUS_BLOCK_MAX;
			} else {
				dev_dbg(&priv->pci_dev->dev,
					"SMBus block read size is %d\n",
					priv->len);
			}
			priv->data[-1] = priv->len;
		}

		/* Read next byte */
		if (priv->count < priv->len)
			priv->data[priv->count++] = inb(SMBBLKDAT(priv));
		else
			dev_dbg(&priv->pci_dev->dev,
				"Discarding extra byte on block read\n");

		/* Set LAST_BYTE for last byte of read transaction */
		if (priv->count == priv->len - 1)
			outb_p(priv->cmd | SMBHSTCNT_LAST_BYTE,
			       SMBHSTCNT(priv));
	} else if (priv->count < priv->len - 1) {
		/* Write next byte, except for IRQ after last byte */
		outb_p(priv->data[++priv->count], SMBBLKDAT(priv));
	}

	/* Clear BYTE_DONE to continue with next byte */
	outb_p(SMBHSTSTS_BYTE_DONE, SMBHSTSTS(priv));
}

/*
 * There are two kinds of interrupts:
 *
 * 1) i801 signals transaction completion with one of these interrupts:
 *      INTR - Success
 *      DEV_ERR - Invalid command, NAK or communication timeout
 *      BUS_ERR - SMI# transaction collision
 *      FAILED - transaction was canceled due to a KILL request
 *    When any of these occur, update ->status and wake up the waitq.
 *    ->status must be cleared before kicking off the next transaction.
 *
 * 2) For byte-by-byte (I2C read/write) transactions, one BYTE_DONE interrupt
 *    occurs for each byte of a byte-by-byte to prepare the next byte.
 */
static irqreturn_t i801_isr(int irq, void *dev_id)
{
	struct i801_priv *priv = dev_id;
	u16 pcists;
	u8 status;

	/* Confirm this is our interrupt */
	pci_read_config_word(priv->pci_dev, SMBPCISTS, &pcists);
	if (!(pcists & SMBPCISTS_INTS))
		return IRQ_NONE;

	status = inb_p(SMBHSTSTS(priv));
	if (status != 0x42)
		dev_dbg(&priv->pci_dev->dev, "irq: status = %02x\n", status);

	if (status & SMBHSTSTS_BYTE_DONE)
		i801_isr_byte_done(priv);

	/*
	 * Clear irq sources and report transaction result.
	 * ->status must be cleared before the next transaction is started.
	 */
	status &= SMBHSTSTS_INTR | STATUS_ERROR_FLAGS;
	if (status) {
		outb_p(status, SMBHSTSTS(priv));
		priv->status |= status;
		wake_up(&priv->waitq);
	}

	return IRQ_HANDLED;
}

/*
 * For "byte-by-byte" block transactions:
 *   I2C write uses cmd=I801_BLOCK_DATA, I2C_EN=1
 *   I2C read uses cmd=I801_I2C_BLOCK_DATA
 */
>>>>>>> refs/remotes/origin/master
static int i801_block_transaction_byte_by_byte(struct i801_priv *priv,
					       union i2c_smbus_data *data,
					       char read_write, int command,
					       int hwpec)
{
	int i, len;
	int smbcmd;
	int status;
	int result;
<<<<<<< HEAD
	int timeout;
=======
>>>>>>> refs/remotes/origin/master

	result = i801_check_pre(priv);
	if (result < 0)
		return result;

	len = data->block[0];

	if (read_write == I2C_SMBUS_WRITE) {
		outb_p(len, SMBHSTDAT0(priv));
		outb_p(data->block[1], SMBBLKDAT(priv));
	}

<<<<<<< HEAD
	for (i = 1; i <= len; i++) {
		if (i == len && read_write == I2C_SMBUS_READ) {
			if (command == I2C_SMBUS_I2C_BLOCK_DATA)
				smbcmd = I801_I2C_BLOCK_LAST;
			else
				smbcmd = I801_BLOCK_LAST;
		} else {
			if (command == I2C_SMBUS_I2C_BLOCK_DATA
			 && read_write == I2C_SMBUS_READ)
				smbcmd = I801_I2C_BLOCK_DATA;
			else
				smbcmd = I801_BLOCK_DATA;
		}
		outb_p(smbcmd | ENABLE_INT9, SMBHSTCNT(priv));

		if (i == 1)
			outb_p(inb(SMBHSTCNT(priv)) | I801_START,
			       SMBHSTCNT(priv));

		/* We will always wait for a fraction of a second! */
		timeout = 0;
		do {
<<<<<<< HEAD
			msleep(1);
			status = inb_p(SMBHSTSTS(priv));
		} while ((!(status & SMBHSTSTS_BYTE_DONE))
			 && (timeout++ < MAX_TIMEOUT));

		result = i801_check_post(priv, status, timeout > MAX_TIMEOUT);
=======
			usleep_range(250, 500);
			status = inb_p(SMBHSTSTS(priv));
		} while ((!(status & SMBHSTSTS_BYTE_DONE))
			 && (timeout++ < MAX_RETRIES));

		result = i801_check_post(priv, status, timeout > MAX_RETRIES);
>>>>>>> refs/remotes/origin/cm-10.0
		if (result < 0)
			return result;
=======
	if (command == I2C_SMBUS_I2C_BLOCK_DATA &&
	    read_write == I2C_SMBUS_READ)
		smbcmd = I801_I2C_BLOCK_DATA;
	else
		smbcmd = I801_BLOCK_DATA;

	if (priv->features & FEATURE_IRQ) {
		priv->is_read = (read_write == I2C_SMBUS_READ);
		if (len == 1 && priv->is_read)
			smbcmd |= SMBHSTCNT_LAST_BYTE;
		priv->cmd = smbcmd | SMBHSTCNT_INTREN;
		priv->len = len;
		priv->count = 0;
		priv->data = &data->block[1];

		outb_p(priv->cmd | SMBHSTCNT_START, SMBHSTCNT(priv));
		wait_event(priv->waitq, (status = priv->status));
		priv->status = 0;
		return i801_check_post(priv, status);
	}

	for (i = 1; i <= len; i++) {
		if (i == len && read_write == I2C_SMBUS_READ)
			smbcmd |= SMBHSTCNT_LAST_BYTE;
		outb_p(smbcmd, SMBHSTCNT(priv));

		if (i == 1)
			outb_p(inb(SMBHSTCNT(priv)) | SMBHSTCNT_START,
			       SMBHSTCNT(priv));

		status = i801_wait_byte_done(priv);
		if (status)
			goto exit;
>>>>>>> refs/remotes/origin/master

		if (i == 1 && read_write == I2C_SMBUS_READ
		 && command != I2C_SMBUS_I2C_BLOCK_DATA) {
			len = inb_p(SMBHSTDAT0(priv));
			if (len < 1 || len > I2C_SMBUS_BLOCK_MAX) {
				dev_err(&priv->pci_dev->dev,
					"Illegal SMBus block read size %d\n",
					len);
				/* Recover */
				while (inb_p(SMBHSTSTS(priv)) &
				       SMBHSTSTS_HOST_BUSY)
					outb_p(SMBHSTSTS_BYTE_DONE,
					       SMBHSTSTS(priv));
				outb_p(SMBHSTSTS_INTR, SMBHSTSTS(priv));
				return -EPROTO;
			}
			data->block[0] = len;
		}

		/* Retrieve/store value in SMBBLKDAT */
		if (read_write == I2C_SMBUS_READ)
			data->block[i] = inb_p(SMBBLKDAT(priv));
		if (read_write == I2C_SMBUS_WRITE && i+1 <= len)
			outb_p(data->block[i+1], SMBBLKDAT(priv));

		/* signals SMBBLKDAT ready */
<<<<<<< HEAD
		outb_p(SMBHSTSTS_BYTE_DONE | SMBHSTSTS_INTR, SMBHSTSTS(priv));
	}

	return 0;
=======
		outb_p(SMBHSTSTS_BYTE_DONE, SMBHSTSTS(priv));
	}

	status = i801_wait_intr(priv);
exit:
	return i801_check_post(priv, status);
>>>>>>> refs/remotes/origin/master
}

static int i801_set_block_buffer_mode(struct i801_priv *priv)
{
	outb_p(inb_p(SMBAUXCTL(priv)) | SMBAUXCTL_E32B, SMBAUXCTL(priv));
	if ((inb_p(SMBAUXCTL(priv)) & SMBAUXCTL_E32B) == 0)
		return -EIO;
	return 0;
}

/* Block transaction function */
static int i801_block_transaction(struct i801_priv *priv,
				  union i2c_smbus_data *data, char read_write,
				  int command, int hwpec)
{
	int result = 0;
	unsigned char hostc;

	if (command == I2C_SMBUS_I2C_BLOCK_DATA) {
		if (read_write == I2C_SMBUS_WRITE) {
			/* set I2C_EN bit in configuration register */
			pci_read_config_byte(priv->pci_dev, SMBHSTCFG, &hostc);
			pci_write_config_byte(priv->pci_dev, SMBHSTCFG,
					      hostc | SMBHSTCFG_I2C_EN);
		} else if (!(priv->features & FEATURE_I2C_BLOCK_READ)) {
			dev_err(&priv->pci_dev->dev,
				"I2C block read is unsupported!\n");
			return -EOPNOTSUPP;
		}
	}

	if (read_write == I2C_SMBUS_WRITE
	 || command == I2C_SMBUS_I2C_BLOCK_DATA) {
		if (data->block[0] < 1)
			data->block[0] = 1;
		if (data->block[0] > I2C_SMBUS_BLOCK_MAX)
			data->block[0] = I2C_SMBUS_BLOCK_MAX;
	} else {
		data->block[0] = 32;	/* max for SMBus block reads */
	}

	/* Experience has shown that the block buffer can only be used for
	   SMBus (not I2C) block transactions, even though the datasheet
	   doesn't mention this limitation. */
	if ((priv->features & FEATURE_BLOCK_BUFFER)
	 && command != I2C_SMBUS_I2C_BLOCK_DATA
	 && i801_set_block_buffer_mode(priv) == 0)
		result = i801_block_transaction_by_block(priv, data,
							 read_write, hwpec);
	else
		result = i801_block_transaction_byte_by_byte(priv, data,
							     read_write,
							     command, hwpec);

<<<<<<< HEAD
	if (result == 0 && hwpec)
		i801_wait_hwpec(priv);

=======
>>>>>>> refs/remotes/origin/master
	if (command == I2C_SMBUS_I2C_BLOCK_DATA
	 && read_write == I2C_SMBUS_WRITE) {
		/* restore saved configuration register value */
		pci_write_config_byte(priv->pci_dev, SMBHSTCFG, hostc);
	}
	return result;
}

/* Return negative errno on error. */
static s32 i801_access(struct i2c_adapter *adap, u16 addr,
		       unsigned short flags, char read_write, u8 command,
		       int size, union i2c_smbus_data *data)
{
	int hwpec;
	int block = 0;
	int ret, xact = 0;
	struct i801_priv *priv = i2c_get_adapdata(adap);

	hwpec = (priv->features & FEATURE_SMBUS_PEC) && (flags & I2C_CLIENT_PEC)
		&& size != I2C_SMBUS_QUICK
		&& size != I2C_SMBUS_I2C_BLOCK_DATA;

	switch (size) {
	case I2C_SMBUS_QUICK:
		outb_p(((addr & 0x7f) << 1) | (read_write & 0x01),
		       SMBHSTADD(priv));
		xact = I801_QUICK;
		break;
	case I2C_SMBUS_BYTE:
		outb_p(((addr & 0x7f) << 1) | (read_write & 0x01),
		       SMBHSTADD(priv));
		if (read_write == I2C_SMBUS_WRITE)
			outb_p(command, SMBHSTCMD(priv));
		xact = I801_BYTE;
		break;
	case I2C_SMBUS_BYTE_DATA:
		outb_p(((addr & 0x7f) << 1) | (read_write & 0x01),
		       SMBHSTADD(priv));
		outb_p(command, SMBHSTCMD(priv));
		if (read_write == I2C_SMBUS_WRITE)
			outb_p(data->byte, SMBHSTDAT0(priv));
		xact = I801_BYTE_DATA;
		break;
	case I2C_SMBUS_WORD_DATA:
		outb_p(((addr & 0x7f) << 1) | (read_write & 0x01),
		       SMBHSTADD(priv));
		outb_p(command, SMBHSTCMD(priv));
		if (read_write == I2C_SMBUS_WRITE) {
			outb_p(data->word & 0xff, SMBHSTDAT0(priv));
			outb_p((data->word & 0xff00) >> 8, SMBHSTDAT1(priv));
		}
		xact = I801_WORD_DATA;
		break;
	case I2C_SMBUS_BLOCK_DATA:
		outb_p(((addr & 0x7f) << 1) | (read_write & 0x01),
		       SMBHSTADD(priv));
		outb_p(command, SMBHSTCMD(priv));
		block = 1;
		break;
	case I2C_SMBUS_I2C_BLOCK_DATA:
		/* NB: page 240 of ICH5 datasheet shows that the R/#W
		 * bit should be cleared here, even when reading */
		outb_p((addr & 0x7f) << 1, SMBHSTADD(priv));
		if (read_write == I2C_SMBUS_READ) {
			/* NB: page 240 of ICH5 datasheet also shows
			 * that DATA1 is the cmd field when reading */
			outb_p(command, SMBHSTDAT1(priv));
		} else
			outb_p(command, SMBHSTCMD(priv));
		block = 1;
		break;
	default:
		dev_err(&priv->pci_dev->dev, "Unsupported transaction %d\n",
			size);
		return -EOPNOTSUPP;
	}

	if (hwpec)	/* enable/disable hardware PEC */
		outb_p(inb_p(SMBAUXCTL(priv)) | SMBAUXCTL_CRC, SMBAUXCTL(priv));
	else
		outb_p(inb_p(SMBAUXCTL(priv)) & (~SMBAUXCTL_CRC),
		       SMBAUXCTL(priv));

	if (block)
		ret = i801_block_transaction(priv, data, read_write, size,
					     hwpec);
	else
<<<<<<< HEAD
		ret = i801_transaction(priv, xact | ENABLE_INT9);
=======
		ret = i801_transaction(priv, xact);
>>>>>>> refs/remotes/origin/master

	/* Some BIOSes don't like it when PEC is enabled at reboot or resume
	   time, so we forcibly disable it after every transaction. Turn off
	   E32B for the same reason. */
	if (hwpec || block)
		outb_p(inb_p(SMBAUXCTL(priv)) &
		       ~(SMBAUXCTL_CRC | SMBAUXCTL_E32B), SMBAUXCTL(priv));

	if (block)
		return ret;
	if (ret)
		return ret;
	if ((read_write == I2C_SMBUS_WRITE) || (xact == I801_QUICK))
		return 0;

	switch (xact & 0x7f) {
	case I801_BYTE:	/* Result put in SMBHSTDAT0 */
	case I801_BYTE_DATA:
		data->byte = inb_p(SMBHSTDAT0(priv));
		break;
	case I801_WORD_DATA:
		data->word = inb_p(SMBHSTDAT0(priv)) +
			     (inb_p(SMBHSTDAT1(priv)) << 8);
		break;
	}
	return 0;
}


static u32 i801_func(struct i2c_adapter *adapter)
{
	struct i801_priv *priv = i2c_get_adapdata(adapter);

	return I2C_FUNC_SMBUS_QUICK | I2C_FUNC_SMBUS_BYTE |
	       I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_WORD_DATA |
	       I2C_FUNC_SMBUS_BLOCK_DATA | I2C_FUNC_SMBUS_WRITE_I2C_BLOCK |
	       ((priv->features & FEATURE_SMBUS_PEC) ? I2C_FUNC_SMBUS_PEC : 0) |
	       ((priv->features & FEATURE_I2C_BLOCK_READ) ?
		I2C_FUNC_SMBUS_READ_I2C_BLOCK : 0);
}

static const struct i2c_algorithm smbus_algorithm = {
	.smbus_xfer	= i801_access,
	.functionality	= i801_func,
};

<<<<<<< HEAD
<<<<<<< HEAD
static const struct pci_device_id i801_ids[] = {
=======
static DEFINE_PCI_DEVICE_TABLE(i801_ids) = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static DEFINE_PCI_DEVICE_TABLE(i801_ids) = {
>>>>>>> refs/remotes/origin/master
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82801AA_3) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82801AB_3) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82801BA_2) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82801CA_3) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82801DB_3) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_82801EB_3) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_ESB_4) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_ICH6_16) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_ICH7_17) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_ESB2_17) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_ICH8_5) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_ICH9_6) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_EP80579_1) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_ICH10_4) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_ICH10_5) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_5_3400_SERIES_SMBUS) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_COUGARPOINT_SMBUS) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS_IDF0) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS_IDF1) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS_IDF2) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_DH89XXCC_SMBUS) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_PANTHERPOINT_SMBUS) },
<<<<<<< HEAD
<<<<<<< HEAD
=======
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_LYNXPOINT_SMBUS) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_LYNXPOINT_LP_SMBUS) },
>>>>>>> refs/remotes/origin/cm-10.0
=======
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_LYNXPOINT_SMBUS) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_LYNXPOINT_LP_SMBUS) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_AVOTON_SMBUS) },
<<<<<<< HEAD
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS_MS0) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS_MS1) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS_MS2) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_COLETOCREEK_SMBUS) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_WILDCATPOINT_LP_SMBUS) },
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	{ 0, }
};

MODULE_DEVICE_TABLE(pci, i801_ids);

#if defined CONFIG_X86 && defined CONFIG_DMI
static unsigned char apanel_addr;

/* Scan the system ROM for the signature "FJKEYINF" */
static __init const void __iomem *bios_signature(const void __iomem *bios)
{
	ssize_t offset;
	const unsigned char signature[] = "FJKEYINF";

	for (offset = 0; offset < 0x10000; offset += 0x10) {
		if (check_signature(bios + offset, signature,
				    sizeof(signature)-1))
			return bios + offset;
	}
	return NULL;
}

static void __init input_apanel_init(void)
{
	void __iomem *bios;
	const void __iomem *p;

	bios = ioremap(0xF0000, 0x10000); /* Can't fail */
	p = bios_signature(bios);
	if (p) {
		/* just use the first address */
		apanel_addr = readb(p + 8 + 3) >> 1;
	}
	iounmap(bios);
}

struct dmi_onboard_device_info {
	const char *name;
	u8 type;
	unsigned short i2c_addr;
	const char *i2c_type;
};

<<<<<<< HEAD
static struct dmi_onboard_device_info __devinitdata dmi_devices[] = {
=======
static const struct dmi_onboard_device_info dmi_devices[] = {
>>>>>>> refs/remotes/origin/master
	{ "Syleus", DMI_DEV_TYPE_OTHER, 0x73, "fscsyl" },
	{ "Hermes", DMI_DEV_TYPE_OTHER, 0x73, "fscher" },
	{ "Hades",  DMI_DEV_TYPE_OTHER, 0x73, "fschds" },
};

<<<<<<< HEAD
static void __devinit dmi_check_onboard_device(u8 type, const char *name,
					       struct i2c_adapter *adap)
=======
static void dmi_check_onboard_device(u8 type, const char *name,
				     struct i2c_adapter *adap)
>>>>>>> refs/remotes/origin/master
{
	int i;
	struct i2c_board_info info;

	for (i = 0; i < ARRAY_SIZE(dmi_devices); i++) {
		/* & ~0x80, ignore enabled/disabled bit */
		if ((type & ~0x80) != dmi_devices[i].type)
			continue;
		if (strcasecmp(name, dmi_devices[i].name))
			continue;

		memset(&info, 0, sizeof(struct i2c_board_info));
		info.addr = dmi_devices[i].i2c_addr;
		strlcpy(info.type, dmi_devices[i].i2c_type, I2C_NAME_SIZE);
		i2c_new_device(adap, &info);
		break;
	}
}

/* We use our own function to check for onboard devices instead of
   dmi_find_device() as some buggy BIOS's have the devices we are interested
   in marked as disabled */
<<<<<<< HEAD
static void __devinit dmi_check_onboard_devices(const struct dmi_header *dm,
						void *adap)
=======
static void dmi_check_onboard_devices(const struct dmi_header *dm, void *adap)
>>>>>>> refs/remotes/origin/master
{
	int i, count;

	if (dm->type != 10)
		return;

	count = (dm->length - sizeof(struct dmi_header)) / 2;
	for (i = 0; i < count; i++) {
		const u8 *d = (char *)(dm + 1) + (i * 2);
		const char *name = ((char *) dm) + dm->length;
		u8 type = d[0];
		u8 s = d[1];

		if (!s)
			continue;
		s--;
		while (s > 0 && name[0]) {
			name += strlen(name) + 1;
			s--;
		}
		if (name[0] == 0) /* Bogus string reference */
			continue;

		dmi_check_onboard_device(type, name, adap);
	}
}

/* Register optional slaves */
<<<<<<< HEAD
static void __devinit i801_probe_optional_slaves(struct i801_priv *priv)
=======
static void i801_probe_optional_slaves(struct i801_priv *priv)
>>>>>>> refs/remotes/origin/master
{
	/* Only register slaves on main SMBus channel */
	if (priv->features & FEATURE_IDF)
		return;

	if (apanel_addr) {
		struct i2c_board_info info;

		memset(&info, 0, sizeof(struct i2c_board_info));
		info.addr = apanel_addr;
		strlcpy(info.type, "fujitsu_apanel", I2C_NAME_SIZE);
		i2c_new_device(&priv->adapter, &info);
	}

	if (dmi_name_in_vendors("FUJITSU"))
		dmi_walk(dmi_check_onboard_devices, &priv->adapter);
}
#else
static void __init input_apanel_init(void) {}
<<<<<<< HEAD
static void __devinit i801_probe_optional_slaves(struct i801_priv *priv) {}
#endif	/* CONFIG_X86 && CONFIG_DMI */

static int __devinit i801_probe(struct pci_dev *dev,
				const struct pci_device_id *id)
=======
static void i801_probe_optional_slaves(struct i801_priv *priv) {}
#endif	/* CONFIG_X86 && CONFIG_DMI */

#if (defined CONFIG_I2C_MUX_GPIO || defined CONFIG_I2C_MUX_GPIO_MODULE) && \
		defined CONFIG_DMI
static struct i801_mux_config i801_mux_config_asus_z8_d12 = {
	.gpio_chip = "gpio_ich",
	.values = { 0x02, 0x03 },
	.n_values = 2,
	.classes = { I2C_CLASS_SPD, I2C_CLASS_SPD },
	.gpios = { 52, 53 },
	.n_gpios = 2,
};

static struct i801_mux_config i801_mux_config_asus_z8_d18 = {
	.gpio_chip = "gpio_ich",
	.values = { 0x02, 0x03, 0x01 },
	.n_values = 3,
	.classes = { I2C_CLASS_SPD, I2C_CLASS_SPD, I2C_CLASS_SPD },
	.gpios = { 52, 53 },
	.n_gpios = 2,
};

static const struct dmi_system_id mux_dmi_table[] = {
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "Z8NA-D6(C)"),
		},
		.driver_data = &i801_mux_config_asus_z8_d12,
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "Z8P(N)E-D12(X)"),
		},
		.driver_data = &i801_mux_config_asus_z8_d12,
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "Z8NH-D12"),
		},
		.driver_data = &i801_mux_config_asus_z8_d12,
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "Z8PH-D12/IFB"),
		},
		.driver_data = &i801_mux_config_asus_z8_d12,
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "Z8NR-D12"),
		},
		.driver_data = &i801_mux_config_asus_z8_d12,
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "Z8P(N)H-D12"),
		},
		.driver_data = &i801_mux_config_asus_z8_d12,
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "Z8PG-D18"),
		},
		.driver_data = &i801_mux_config_asus_z8_d18,
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "Z8PE-D18"),
		},
		.driver_data = &i801_mux_config_asus_z8_d18,
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ASUSTeK Computer INC."),
			DMI_MATCH(DMI_BOARD_NAME, "Z8PS-D12"),
		},
		.driver_data = &i801_mux_config_asus_z8_d12,
	},
	{ }
};

/* Setup multiplexing if needed */
static int i801_add_mux(struct i801_priv *priv)
{
	struct device *dev = &priv->adapter.dev;
	const struct i801_mux_config *mux_config;
	struct i2c_mux_gpio_platform_data gpio_data;
	int err;

	if (!priv->mux_drvdata)
		return 0;
	mux_config = priv->mux_drvdata;

	/* Prepare the platform data */
	memset(&gpio_data, 0, sizeof(struct i2c_mux_gpio_platform_data));
	gpio_data.parent = priv->adapter.nr;
	gpio_data.values = mux_config->values;
	gpio_data.n_values = mux_config->n_values;
	gpio_data.classes = mux_config->classes;
	gpio_data.gpio_chip = mux_config->gpio_chip;
	gpio_data.gpios = mux_config->gpios;
	gpio_data.n_gpios = mux_config->n_gpios;
	gpio_data.idle = I2C_MUX_GPIO_NO_IDLE;

	/* Register the mux device */
	priv->mux_pdev = platform_device_register_data(dev, "i2c-mux-gpio",
				PLATFORM_DEVID_AUTO, &gpio_data,
				sizeof(struct i2c_mux_gpio_platform_data));
	if (IS_ERR(priv->mux_pdev)) {
		err = PTR_ERR(priv->mux_pdev);
		priv->mux_pdev = NULL;
		dev_err(dev, "Failed to register i2c-mux-gpio device\n");
		return err;
	}

	return 0;
}

static void i801_del_mux(struct i801_priv *priv)
{
	if (priv->mux_pdev)
		platform_device_unregister(priv->mux_pdev);
}

static unsigned int i801_get_adapter_class(struct i801_priv *priv)
{
	const struct dmi_system_id *id;
	const struct i801_mux_config *mux_config;
	unsigned int class = I2C_CLASS_HWMON | I2C_CLASS_SPD;
	int i;

	id = dmi_first_match(mux_dmi_table);
	if (id) {
		/* Remove branch classes from trunk */
		mux_config = id->driver_data;
		for (i = 0; i < mux_config->n_values; i++)
			class &= ~mux_config->classes[i];

		/* Remember for later */
		priv->mux_drvdata = mux_config;
	}

	return class;
}
#else
static inline int i801_add_mux(struct i801_priv *priv) { return 0; }
static inline void i801_del_mux(struct i801_priv *priv) { }

static inline unsigned int i801_get_adapter_class(struct i801_priv *priv)
{
	return I2C_CLASS_HWMON | I2C_CLASS_SPD;
}
#endif

static int i801_probe(struct pci_dev *dev, const struct pci_device_id *id)
>>>>>>> refs/remotes/origin/master
{
	unsigned char temp;
	int err, i;
	struct i801_priv *priv;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	i2c_set_adapdata(&priv->adapter, priv);
	priv->adapter.owner = THIS_MODULE;
<<<<<<< HEAD
	priv->adapter.class = I2C_CLASS_HWMON | I2C_CLASS_SPD;
=======
	priv->adapter.class = i801_get_adapter_class(priv);
>>>>>>> refs/remotes/origin/master
	priv->adapter.algo = &smbus_algorithm;

	priv->pci_dev = dev;
	switch (dev->device) {
	case PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS_IDF0:
	case PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS_IDF1:
	case PCI_DEVICE_ID_INTEL_PATSBURG_SMBUS_IDF2:
<<<<<<< HEAD
=======
	case PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS_MS0:
	case PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS_MS1:
	case PCI_DEVICE_ID_INTEL_WELLSBURG_SMBUS_MS2:
>>>>>>> refs/remotes/origin/master
		priv->features |= FEATURE_IDF;
		/* fall through */
	default:
		priv->features |= FEATURE_I2C_BLOCK_READ;
<<<<<<< HEAD
=======
		priv->features |= FEATURE_IRQ;
>>>>>>> refs/remotes/origin/master
		/* fall through */
	case PCI_DEVICE_ID_INTEL_82801DB_3:
		priv->features |= FEATURE_SMBUS_PEC;
		priv->features |= FEATURE_BLOCK_BUFFER;
		/* fall through */
	case PCI_DEVICE_ID_INTEL_82801CA_3:
	case PCI_DEVICE_ID_INTEL_82801BA_2:
	case PCI_DEVICE_ID_INTEL_82801AB_3:
	case PCI_DEVICE_ID_INTEL_82801AA_3:
		break;
	}

	/* Disable features on user request */
	for (i = 0; i < ARRAY_SIZE(i801_feature_names); i++) {
		if (priv->features & disable_features & (1 << i))
			dev_notice(&dev->dev, "%s disabled by user\n",
				   i801_feature_names[i]);
	}
	priv->features &= ~disable_features;

	err = pci_enable_device(dev);
	if (err) {
		dev_err(&dev->dev, "Failed to enable SMBus PCI device (%d)\n",
			err);
		goto exit;
	}

	/* Determine the address of the SMBus area */
	priv->smba = pci_resource_start(dev, SMBBAR);
	if (!priv->smba) {
		dev_err(&dev->dev, "SMBus base address uninitialized, "
			"upgrade BIOS\n");
		err = -ENODEV;
		goto exit;
	}

	err = acpi_check_resource_conflict(&dev->resource[SMBBAR]);
	if (err) {
		err = -ENODEV;
		goto exit;
	}

	err = pci_request_region(dev, SMBBAR, i801_driver.name);
	if (err) {
		dev_err(&dev->dev, "Failed to request SMBus region "
			"0x%lx-0x%Lx\n", priv->smba,
			(unsigned long long)pci_resource_end(dev, SMBBAR));
		goto exit;
	}

	pci_read_config_byte(priv->pci_dev, SMBHSTCFG, &temp);
	priv->original_hstcfg = temp;
	temp &= ~SMBHSTCFG_I2C_EN;	/* SMBus timing */
	if (!(temp & SMBHSTCFG_HST_EN)) {
		dev_info(&dev->dev, "Enabling SMBus device\n");
		temp |= SMBHSTCFG_HST_EN;
	}
	pci_write_config_byte(priv->pci_dev, SMBHSTCFG, temp);

<<<<<<< HEAD
	if (temp & SMBHSTCFG_SMB_SMI_EN)
		dev_dbg(&dev->dev, "SMBus using interrupt SMI#\n");
	else
		dev_dbg(&dev->dev, "SMBus using PCI Interrupt\n");
=======
	if (temp & SMBHSTCFG_SMB_SMI_EN) {
		dev_dbg(&dev->dev, "SMBus using interrupt SMI#\n");
		/* Disable SMBus interrupt feature if SMBus using SMI# */
		priv->features &= ~FEATURE_IRQ;
	}
>>>>>>> refs/remotes/origin/master

	/* Clear special mode bits */
	if (priv->features & (FEATURE_SMBUS_PEC | FEATURE_BLOCK_BUFFER))
		outb_p(inb_p(SMBAUXCTL(priv)) &
		       ~(SMBAUXCTL_CRC | SMBAUXCTL_E32B), SMBAUXCTL(priv));

<<<<<<< HEAD
=======
	if (priv->features & FEATURE_IRQ) {
		init_waitqueue_head(&priv->waitq);

		err = request_irq(dev->irq, i801_isr, IRQF_SHARED,
				  i801_driver.name, priv);
		if (err) {
			dev_err(&dev->dev, "Failed to allocate irq %d: %d\n",
				dev->irq, err);
			goto exit_release;
		}
		dev_info(&dev->dev, "SMBus using PCI Interrupt\n");
	}

>>>>>>> refs/remotes/origin/master
	/* set up the sysfs linkage to our parent device */
	priv->adapter.dev.parent = &dev->dev;

	/* Retry up to 3 times on lost arbitration */
	priv->adapter.retries = 3;

	snprintf(priv->adapter.name, sizeof(priv->adapter.name),
		"SMBus I801 adapter at %04lx", priv->smba);
	err = i2c_add_adapter(&priv->adapter);
	if (err) {
		dev_err(&dev->dev, "Failed to add SMBus adapter\n");
<<<<<<< HEAD
		goto exit_release;
	}

	i801_probe_optional_slaves(priv);

	pci_set_drvdata(dev, priv);
	return 0;

=======
		goto exit_free_irq;
	}

	i801_probe_optional_slaves(priv);
	/* We ignore errors - multiplexing is optional */
	i801_add_mux(priv);

	pci_set_drvdata(dev, priv);

	return 0;

exit_free_irq:
	if (priv->features & FEATURE_IRQ)
		free_irq(dev->irq, priv);
>>>>>>> refs/remotes/origin/master
exit_release:
	pci_release_region(dev, SMBBAR);
exit:
	kfree(priv);
	return err;
}

<<<<<<< HEAD
static void __devexit i801_remove(struct pci_dev *dev)
{
	struct i801_priv *priv = pci_get_drvdata(dev);

	i2c_del_adapter(&priv->adapter);
	pci_write_config_byte(dev, SMBHSTCFG, priv->original_hstcfg);
	pci_release_region(dev, SMBBAR);
	pci_set_drvdata(dev, NULL);
=======
static void i801_remove(struct pci_dev *dev)
{
	struct i801_priv *priv = pci_get_drvdata(dev);

	i801_del_mux(priv);
	i2c_del_adapter(&priv->adapter);
	pci_write_config_byte(dev, SMBHSTCFG, priv->original_hstcfg);

	if (priv->features & FEATURE_IRQ)
		free_irq(dev->irq, priv);
	pci_release_region(dev, SMBBAR);

>>>>>>> refs/remotes/origin/master
	kfree(priv);
	/*
	 * do not call pci_disable_device(dev) since it can cause hard hangs on
	 * some systems during power-off (eg. Fujitsu-Siemens Lifebook E8010)
	 */
}

#ifdef CONFIG_PM
static int i801_suspend(struct pci_dev *dev, pm_message_t mesg)
{
	struct i801_priv *priv = pci_get_drvdata(dev);

	pci_save_state(dev);
	pci_write_config_byte(dev, SMBHSTCFG, priv->original_hstcfg);
	pci_set_power_state(dev, pci_choose_state(dev, mesg));
	return 0;
}

static int i801_resume(struct pci_dev *dev)
{
	pci_set_power_state(dev, PCI_D0);
	pci_restore_state(dev);
	return pci_enable_device(dev);
}
#else
#define i801_suspend NULL
#define i801_resume NULL
#endif

static struct pci_driver i801_driver = {
	.name		= "i801_smbus",
	.id_table	= i801_ids,
	.probe		= i801_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(i801_remove),
=======
	.remove		= i801_remove,
>>>>>>> refs/remotes/origin/master
	.suspend	= i801_suspend,
	.resume		= i801_resume,
};

static int __init i2c_i801_init(void)
{
	if (dmi_name_in_vendors("FUJITSU"))
		input_apanel_init();
	return pci_register_driver(&i801_driver);
}

static void __exit i2c_i801_exit(void)
{
	pci_unregister_driver(&i801_driver);
}

MODULE_AUTHOR("Mark D. Studebaker <mdsxyz123@yahoo.com>, "
	      "Jean Delvare <khali@linux-fr.org>");
MODULE_DESCRIPTION("I801 SMBus driver");
MODULE_LICENSE("GPL");

module_init(i2c_i801_init);
module_exit(i2c_i801_exit);
