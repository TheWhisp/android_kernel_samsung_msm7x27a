/*
 *  drivers/mtd/nand/au1550nd.c
 *
 *  Copyright (C) 2004 Embedded Edge, LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/io.h>

#include <asm/mach-au1x00/au1xxx.h>
#include <asm/mach-db1x00/bcsr.h>

/*
 * MTD structure for NAND controller
 */
static struct mtd_info *au1550_mtd = NULL;
static void __iomem *p_nand;
static int nand_width = 1;	/* default x8 */
static void (*au1550_write_byte)(struct mtd_info *, u_char);

/*
 * Define partitions for flash device
 */
static const struct mtd_partition partition_info[] = {
	{
	 .name = "NAND FS 0",
	 .offset = 0,
	 .size = 8 * 1024 * 1024},
	{
	 .name = "NAND FS 1",
	 .offset = MTDPART_OFS_APPEND,
	 .size = MTDPART_SIZ_FULL}
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <asm/io.h>
#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-au1x00/au1550nd.h>


struct au1550nd_ctx {
	struct mtd_info info;
	struct nand_chip chip;

	int cs;
	void __iomem *base;
	void (*write_byte)(struct mtd_info *, u_char);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

/**
 * au_read_byte -  read one byte from the chip
 * @mtd:	MTD device structure
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  read function for 8bit buswith
=======
 * read function for 8bit buswidth
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * read function for 8bit buswidth
>>>>>>> refs/remotes/origin/master
 */
static u_char au_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	u_char ret = readb(this->IO_ADDR_R);
	au_sync();
	return ret;
}

/**
 * au_write_byte -  write one byte to the chip
 * @mtd:	MTD device structure
 * @byte:	pointer to data byte to write
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  write function for 8it buswith
=======
 * write function for 8it buswidth
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * write function for 8it buswidth
>>>>>>> refs/remotes/origin/master
 */
static void au_write_byte(struct mtd_info *mtd, u_char byte)
{
	struct nand_chip *this = mtd->priv;
	writeb(byte, this->IO_ADDR_W);
	au_sync();
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * au_read_byte16 -  read one byte endianess aware from the chip
 * @mtd:	MTD device structure
 *
 *  read function for 16bit buswith with
 * endianess conversion
=======
=======
>>>>>>> refs/remotes/origin/master
 * au_read_byte16 -  read one byte endianness aware from the chip
 * @mtd:	MTD device structure
 *
 * read function for 16bit buswidth with endianness conversion
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */
static u_char au_read_byte16(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	u_char ret = (u_char) cpu_to_le16(readw(this->IO_ADDR_R));
	au_sync();
	return ret;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * au_write_byte16 -  write one byte endianess aware to the chip
 * @mtd:	MTD device structure
 * @byte:	pointer to data byte to write
 *
 *  write function for 16bit buswith with
 * endianess conversion
=======
=======
>>>>>>> refs/remotes/origin/master
 * au_write_byte16 -  write one byte endianness aware to the chip
 * @mtd:	MTD device structure
 * @byte:	pointer to data byte to write
 *
 * write function for 16bit buswidth with endianness conversion
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */
static void au_write_byte16(struct mtd_info *mtd, u_char byte)
{
	struct nand_chip *this = mtd->priv;
	writew(le16_to_cpu((u16) byte), this->IO_ADDR_W);
	au_sync();
}

/**
 * au_read_word -  read one word from the chip
 * @mtd:	MTD device structure
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  read function for 16bit buswith without
 * endianess conversion
=======
 * read function for 16bit buswidth without endianness conversion
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * read function for 16bit buswidth without endianness conversion
>>>>>>> refs/remotes/origin/master
 */
static u16 au_read_word(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	u16 ret = readw(this->IO_ADDR_R);
	au_sync();
	return ret;
}

/**
 * au_write_buf -  write buffer to chip
 * @mtd:	MTD device structure
 * @buf:	data buffer
 * @len:	number of bytes to write
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  write function for 8bit buswith
=======
 * write function for 8bit buswidth
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * write function for 8bit buswidth
>>>>>>> refs/remotes/origin/master
 */
static void au_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;

	for (i = 0; i < len; i++) {
		writeb(buf[i], this->IO_ADDR_W);
		au_sync();
	}
}

/**
 * au_read_buf -  read chip data into buffer
 * @mtd:	MTD device structure
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  read function for 8bit buswith
=======
 * read function for 8bit buswidth
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * read function for 8bit buswidth
>>>>>>> refs/remotes/origin/master
 */
static void au_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;

	for (i = 0; i < len; i++) {
		buf[i] = readb(this->IO_ADDR_R);
		au_sync();
	}
}

/**
<<<<<<< HEAD
 * au_verify_buf -  Verify chip data against buffer
 * @mtd:	MTD device structure
 * @buf:	buffer containing the data to compare
 * @len:	number of bytes to compare
 *
<<<<<<< HEAD
 *  verify function for 8bit buswith
=======
 * verify function for 8bit buswidth
>>>>>>> refs/remotes/origin/cm-10.0
 */
static int au_verify_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;

	for (i = 0; i < len; i++) {
		if (buf[i] != readb(this->IO_ADDR_R))
			return -EFAULT;
		au_sync();
	}

	return 0;
}

/**
=======
>>>>>>> refs/remotes/origin/master
 * au_write_buf16 -  write buffer to chip
 * @mtd:	MTD device structure
 * @buf:	data buffer
 * @len:	number of bytes to write
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  write function for 16bit buswith
=======
 * write function for 16bit buswidth
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * write function for 16bit buswidth
>>>>>>> refs/remotes/origin/master
 */
static void au_write_buf16(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;
	u16 *p = (u16 *) buf;
	len >>= 1;

	for (i = 0; i < len; i++) {
		writew(p[i], this->IO_ADDR_W);
		au_sync();
	}

}

/**
 * au_read_buf16 -  read chip data into buffer
 * @mtd:	MTD device structure
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  read function for 16bit buswith
=======
 * read function for 16bit buswidth
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * read function for 16bit buswidth
>>>>>>> refs/remotes/origin/master
 */
static void au_read_buf16(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;
	u16 *p = (u16 *) buf;
	len >>= 1;

	for (i = 0; i < len; i++) {
		p[i] = readw(this->IO_ADDR_R);
		au_sync();
	}
}

<<<<<<< HEAD
/**
 * au_verify_buf16 -  Verify chip data against buffer
 * @mtd:	MTD device structure
 * @buf:	buffer containing the data to compare
 * @len:	number of bytes to compare
 *
<<<<<<< HEAD
 *  verify function for 16bit buswith
=======
 * verify function for 16bit buswidth
>>>>>>> refs/remotes/origin/cm-10.0
 */
static int au_verify_buf16(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;
	u16 *p = (u16 *) buf;
	len >>= 1;

	for (i = 0; i < len; i++) {
		if (p[i] != readw(this->IO_ADDR_R))
			return -EFAULT;
		au_sync();
	}
	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
/* Select the chip by setting nCE to low */
#define NAND_CTL_SETNCE		1
/* Deselect the chip by setting nCE to high */
#define NAND_CTL_CLRNCE		2
/* Select the command latch by setting CLE to high */
#define NAND_CTL_SETCLE		3
/* Deselect the command latch by setting CLE to low */
#define NAND_CTL_CLRCLE		4
/* Select the address latch by setting ALE to high */
#define NAND_CTL_SETALE		5
/* Deselect the address latch by setting ALE to low */
#define NAND_CTL_CLRALE		6

static void au1550_hwcontrol(struct mtd_info *mtd, int cmd)
{
<<<<<<< HEAD
<<<<<<< HEAD
	register struct nand_chip *this = mtd->priv;
=======
	struct au1550nd_ctx *ctx = container_of(mtd, struct au1550nd_ctx, info);
	struct nand_chip *this = mtd->priv;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct au1550nd_ctx *ctx = container_of(mtd, struct au1550nd_ctx, info);
	struct nand_chip *this = mtd->priv;
>>>>>>> refs/remotes/origin/master

	switch (cmd) {

	case NAND_CTL_SETCLE:
<<<<<<< HEAD
<<<<<<< HEAD
		this->IO_ADDR_W = p_nand + MEM_STNAND_CMD;
		break;

	case NAND_CTL_CLRCLE:
		this->IO_ADDR_W = p_nand + MEM_STNAND_DATA;
		break;

	case NAND_CTL_SETALE:
		this->IO_ADDR_W = p_nand + MEM_STNAND_ADDR;
		break;

	case NAND_CTL_CLRALE:
		this->IO_ADDR_W = p_nand + MEM_STNAND_DATA;
=======
=======
>>>>>>> refs/remotes/origin/master
		this->IO_ADDR_W = ctx->base + MEM_STNAND_CMD;
		break;

	case NAND_CTL_CLRCLE:
		this->IO_ADDR_W = ctx->base + MEM_STNAND_DATA;
		break;

	case NAND_CTL_SETALE:
		this->IO_ADDR_W = ctx->base + MEM_STNAND_ADDR;
		break;

	case NAND_CTL_CLRALE:
		this->IO_ADDR_W = ctx->base + MEM_STNAND_DATA;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		/* FIXME: Nobody knows why this is necessary,
		 * but it works only that way */
		udelay(1);
		break;

	case NAND_CTL_SETNCE:
		/* assert (force assert) chip enable */
<<<<<<< HEAD
<<<<<<< HEAD
		au_writel((1 << (4 + NAND_CS)), MEM_STNDCTL);
=======
		au_writel((1 << (4 + ctx->cs)), MEM_STNDCTL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		au_writel((1 << (4 + ctx->cs)), MEM_STNDCTL);
>>>>>>> refs/remotes/origin/master
		break;

	case NAND_CTL_CLRNCE:
		/* deassert chip enable */
		au_writel(0, MEM_STNDCTL);
		break;
	}

	this->IO_ADDR_R = this->IO_ADDR_W;

	/* Drain the writebuffer */
	au_sync();
}

int au1550_device_ready(struct mtd_info *mtd)
{
	int ret = (au_readl(MEM_STSTAT) & 0x1) ? 1 : 0;
	au_sync();
	return ret;
}

/**
 * au1550_select_chip - control -CE line
 *	Forbid driving -CE manually permitting the NAND controller to do this.
 *	Keeping -CE asserted during the whole sector reads interferes with the
 *	NOR flash and PCMCIA drivers as it causes contention on the static bus.
 *	We only have to hold -CE low for the NAND read commands since the flash
 *	chip needs it to be asserted during chip not ready time but the NAND
 *	controller keeps it released.
 *
 * @mtd:	MTD device structure
 * @chip:	chipnumber to select, -1 for deselect
 */
static void au1550_select_chip(struct mtd_info *mtd, int chip)
{
}

/**
 * au1550_command - Send command to NAND device
 * @mtd:	MTD device structure
 * @command:	the command to be sent
 * @column:	the column address for this command, -1 if none
 * @page_addr:	the page address for this command, -1 if none
 */
static void au1550_command(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	register struct nand_chip *this = mtd->priv;
	int ce_override = 0, i;
	ulong flags;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct au1550nd_ctx *ctx = container_of(mtd, struct au1550nd_ctx, info);
	struct nand_chip *this = mtd->priv;
	int ce_override = 0, i;
	unsigned long flags = 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Begin command latch cycle */
	au1550_hwcontrol(mtd, NAND_CTL_SETCLE);
	/*
	 * Write out the command to the device.
	 */
	if (command == NAND_CMD_SEQIN) {
		int readcmd;

		if (column >= mtd->writesize) {
			/* OOB area */
			column -= mtd->writesize;
			readcmd = NAND_CMD_READOOB;
		} else if (column < 256) {
			/* First 256 bytes --> READ0 */
			readcmd = NAND_CMD_READ0;
		} else {
			column -= 256;
			readcmd = NAND_CMD_READ1;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		au1550_write_byte(mtd, readcmd);
	}
	au1550_write_byte(mtd, command);
=======
		ctx->write_byte(mtd, readcmd);
	}
	ctx->write_byte(mtd, command);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ctx->write_byte(mtd, readcmd);
	}
	ctx->write_byte(mtd, command);
>>>>>>> refs/remotes/origin/master

	/* Set ALE and clear CLE to start address cycle */
	au1550_hwcontrol(mtd, NAND_CTL_CLRCLE);

	if (column != -1 || page_addr != -1) {
		au1550_hwcontrol(mtd, NAND_CTL_SETALE);

		/* Serially input address */
		if (column != -1) {
			/* Adjust columns for 16 bit buswidth */
			if (this->options & NAND_BUSWIDTH_16)
				column >>= 1;
<<<<<<< HEAD
<<<<<<< HEAD
			au1550_write_byte(mtd, column);
		}
		if (page_addr != -1) {
			au1550_write_byte(mtd, (u8)(page_addr & 0xff));
=======
=======
>>>>>>> refs/remotes/origin/master
			ctx->write_byte(mtd, column);
		}
		if (page_addr != -1) {
			ctx->write_byte(mtd, (u8)(page_addr & 0xff));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

			if (command == NAND_CMD_READ0 ||
			    command == NAND_CMD_READ1 ||
			    command == NAND_CMD_READOOB) {
				/*
				 * NAND controller will release -CE after
				 * the last address byte is written, so we'll
				 * have to forcibly assert it. No interrupts
				 * are allowed while we do this as we don't
				 * want the NOR flash or PCMCIA drivers to
				 * steal our precious bytes of data...
				 */
				ce_override = 1;
				local_irq_save(flags);
				au1550_hwcontrol(mtd, NAND_CTL_SETNCE);
			}

<<<<<<< HEAD
<<<<<<< HEAD
			au1550_write_byte(mtd, (u8)(page_addr >> 8));

			/* One more address cycle for devices > 32MiB */
			if (this->chipsize > (32 << 20))
				au1550_write_byte(mtd, (u8)((page_addr >> 16) & 0x0f));
=======
=======
>>>>>>> refs/remotes/origin/master
			ctx->write_byte(mtd, (u8)(page_addr >> 8));

			/* One more address cycle for devices > 32MiB */
			if (this->chipsize > (32 << 20))
				ctx->write_byte(mtd,
						((page_addr >> 16) & 0x0f));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
		/* Latch in address */
		au1550_hwcontrol(mtd, NAND_CTL_CLRALE);
	}

	/*
	 * Program and erase have their own busy handlers.
	 * Status and sequential in need no delay.
	 */
	switch (command) {

	case NAND_CMD_PAGEPROG:
	case NAND_CMD_ERASE1:
	case NAND_CMD_ERASE2:
	case NAND_CMD_SEQIN:
	case NAND_CMD_STATUS:
		return;

	case NAND_CMD_RESET:
		break;

	case NAND_CMD_READ0:
	case NAND_CMD_READ1:
	case NAND_CMD_READOOB:
		/* Check if we're really driving -CE low (just in case) */
		if (unlikely(!ce_override))
			break;

		/* Apply a short delay always to ensure that we do wait tWB. */
		ndelay(100);
		/* Wait for a chip to become ready... */
		for (i = this->chip_delay; !this->dev_ready(mtd) && i > 0; --i)
			udelay(1);

		/* Release -CE and re-enable interrupts. */
		au1550_hwcontrol(mtd, NAND_CTL_CLRNCE);
		local_irq_restore(flags);
		return;
	}
	/* Apply this short delay always to ensure that we do wait tWB. */
	ndelay(100);

	while(!this->dev_ready(mtd));
}

<<<<<<< HEAD
<<<<<<< HEAD

/*
 * Main initialization routine
 */
static int __init au1xxx_nand_init(void)
{
	struct nand_chip *this;
	u16 boot_swapboot = 0;	/* default value */
	int retval;
	u32 mem_staddr;
	u32 nand_phys;

	/* Allocate memory for MTD device structure and private data */
	au1550_mtd = kzalloc(sizeof(struct mtd_info) + sizeof(struct nand_chip), GFP_KERNEL);
	if (!au1550_mtd) {
		printk("Unable to allocate NAND MTD dev structure.\n");
		return -ENOMEM;
	}

	/* Get pointer to private data */
	this = (struct nand_chip *)(&au1550_mtd[1]);

	/* Link the private data with the MTD structure */
	au1550_mtd->priv = this;
	au1550_mtd->owner = THIS_MODULE;


	/* MEM_STNDCTL: disable ints, disable nand boot */
	au_writel(0, MEM_STNDCTL);

#ifdef CONFIG_MIPS_PB1550
	/* set gpio206 high */
	gpio_direction_input(206);

	boot_swapboot = (au_readl(MEM_STSTAT) & (0x7 << 1)) | ((bcsr_read(BCSR_STATUS) >> 6) & 0x1);

	switch (boot_swapboot) {
	case 0:
	case 2:
	case 8:
	case 0xC:
	case 0xD:
		/* x16 NAND Flash */
		nand_width = 0;
		break;
	case 1:
	case 9:
	case 3:
	case 0xE:
	case 0xF:
		/* x8 NAND Flash */
		nand_width = 1;
		break;
	default:
		printk("Pb1550 NAND: bad boot:swap\n");
		retval = -EINVAL;
		goto outmem;
	}
#endif

	/* Configure chip-select; normally done by boot code, e.g. YAMON */
#ifdef NAND_STCFG
	if (NAND_CS == 0) {
		au_writel(NAND_STCFG,  MEM_STCFG0);
		au_writel(NAND_STTIME, MEM_STTIME0);
		au_writel(NAND_STADDR, MEM_STADDR0);
	}
	if (NAND_CS == 1) {
		au_writel(NAND_STCFG,  MEM_STCFG1);
		au_writel(NAND_STTIME, MEM_STTIME1);
		au_writel(NAND_STADDR, MEM_STADDR1);
	}
	if (NAND_CS == 2) {
		au_writel(NAND_STCFG,  MEM_STCFG2);
		au_writel(NAND_STTIME, MEM_STTIME2);
		au_writel(NAND_STADDR, MEM_STADDR2);
	}
	if (NAND_CS == 3) {
		au_writel(NAND_STCFG,  MEM_STCFG3);
		au_writel(NAND_STTIME, MEM_STTIME3);
		au_writel(NAND_STADDR, MEM_STADDR3);
	}
#endif

	/* Locate NAND chip-select in order to determine NAND phys address */
	mem_staddr = 0x00000000;
	if (((au_readl(MEM_STCFG0) & 0x7) == 0x5) && (NAND_CS == 0))
		mem_staddr = au_readl(MEM_STADDR0);
	else if (((au_readl(MEM_STCFG1) & 0x7) == 0x5) && (NAND_CS == 1))
		mem_staddr = au_readl(MEM_STADDR1);
	else if (((au_readl(MEM_STCFG2) & 0x7) == 0x5) && (NAND_CS == 2))
		mem_staddr = au_readl(MEM_STADDR2);
	else if (((au_readl(MEM_STCFG3) & 0x7) == 0x5) && (NAND_CS == 3))
		mem_staddr = au_readl(MEM_STADDR3);

	if (mem_staddr == 0x00000000) {
		printk("Au1xxx NAND: ERROR WITH NAND CHIP-SELECT\n");
		kfree(au1550_mtd);
		return 1;
	}
	nand_phys = (mem_staddr << 4) & 0xFFFC0000;

	p_nand = ioremap(nand_phys, 0x1000);

	/* make controller and MTD agree */
	if (NAND_CS == 0)
		nand_width = au_readl(MEM_STCFG0) & (1 << 22);
	if (NAND_CS == 1)
		nand_width = au_readl(MEM_STCFG1) & (1 << 22);
	if (NAND_CS == 2)
		nand_width = au_readl(MEM_STCFG2) & (1 << 22);
	if (NAND_CS == 3)
		nand_width = au_readl(MEM_STCFG3) & (1 << 22);

	/* Set address of hardware control function */
=======
static int __devinit find_nand_cs(unsigned long nand_base)
=======
static int find_nand_cs(unsigned long nand_base)
>>>>>>> refs/remotes/origin/master
{
	void __iomem *base =
			(void __iomem *)KSEG1ADDR(AU1000_STATIC_MEM_PHYS_ADDR);
	unsigned long addr, staddr, start, mask, end;
	int i;

	for (i = 0; i < 4; i++) {
		addr = 0x1000 + (i * 0x10);			/* CSx */
		staddr = __raw_readl(base + addr + 0x08);	/* STADDRx */
		/* figure out the decoded range of this CS */
		start = (staddr << 4) & 0xfffc0000;
		mask = (staddr << 18) & 0xfffc0000;
		end = (start | (start - 1)) & ~(start ^ mask);
		if ((nand_base >= start) && (nand_base < end))
			return i;
	}

	return -ENODEV;
}

<<<<<<< HEAD
static int __devinit au1550nd_probe(struct platform_device *pdev)
=======
static int au1550nd_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct au1550nd_platdata *pd;
	struct au1550nd_ctx *ctx;
	struct nand_chip *this;
	struct resource *r;
	int ret, cs;

<<<<<<< HEAD
	pd = pdev->dev.platform_data;
=======
	pd = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master
	if (!pd) {
		dev_err(&pdev->dev, "missing platform data\n");
		return -ENODEV;
	}

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx) {
		dev_err(&pdev->dev, "no memory for NAND context\n");
		return -ENOMEM;
	}

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		dev_err(&pdev->dev, "no NAND memory resource\n");
		ret = -ENODEV;
		goto out1;
	}
	if (request_mem_region(r->start, resource_size(r), "au1550-nand")) {
		dev_err(&pdev->dev, "cannot claim NAND memory area\n");
		ret = -ENOMEM;
		goto out1;
	}

	ctx->base = ioremap_nocache(r->start, 0x1000);
	if (!ctx->base) {
		dev_err(&pdev->dev, "cannot remap NAND memory area\n");
		ret = -ENODEV;
		goto out2;
	}

	this = &ctx->chip;
	ctx->info.priv = this;
	ctx->info.owner = THIS_MODULE;

	/* figure out which CS# r->start belongs to */
	cs = find_nand_cs(r->start);
	if (cs < 0) {
		dev_err(&pdev->dev, "cannot detect NAND chipselect\n");
		ret = -ENODEV;
		goto out3;
	}
	ctx->cs = cs;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	this->dev_ready = au1550_device_ready;
	this->select_chip = au1550_select_chip;
	this->cmdfunc = au1550_command;

	/* 30 us command delay time */
	this->chip_delay = 30;
	this->ecc.mode = NAND_ECC_SOFT;

<<<<<<< HEAD
	this->options = NAND_NO_AUTOINCR;

<<<<<<< HEAD
	if (!nand_width)
		this->options |= NAND_BUSWIDTH_16;

	this->read_byte = (!nand_width) ? au_read_byte16 : au_read_byte;
	au1550_write_byte = (!nand_width) ? au_write_byte16 : au_write_byte;
	this->read_word = au_read_word;
	this->write_buf = (!nand_width) ? au_write_buf16 : au_write_buf;
	this->read_buf = (!nand_width) ? au_read_buf16 : au_read_buf;
	this->verify_buf = (!nand_width) ? au_verify_buf16 : au_verify_buf;

	/* Scan to find existence of the device */
	if (nand_scan(au1550_mtd, 1)) {
		retval = -ENXIO;
		goto outio;
	}

	/* Register the partitions */
	mtd_device_register(au1550_mtd, partition_info,
			    ARRAY_SIZE(partition_info));

	return 0;

 outio:
	iounmap(p_nand);

 outmem:
	kfree(au1550_mtd);
	return retval;
}

module_init(au1xxx_nand_init);

/*
 * Clean up routine
 */
static void __exit au1550_cleanup(void)
{
	/* Release resources, unregister device */
	nand_release(au1550_mtd);

	/* Free the MTD device structure */
	kfree(au1550_mtd);

	/* Unmap */
	iounmap(p_nand);
}

module_exit(au1550_cleanup);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (pd->devwidth)
		this->options |= NAND_BUSWIDTH_16;

	this->read_byte = (pd->devwidth) ? au_read_byte16 : au_read_byte;
	ctx->write_byte = (pd->devwidth) ? au_write_byte16 : au_write_byte;
	this->read_word = au_read_word;
	this->write_buf = (pd->devwidth) ? au_write_buf16 : au_write_buf;
	this->read_buf = (pd->devwidth) ? au_read_buf16 : au_read_buf;
<<<<<<< HEAD
	this->verify_buf = (pd->devwidth) ? au_verify_buf16 : au_verify_buf;
=======
>>>>>>> refs/remotes/origin/master

	ret = nand_scan(&ctx->info, 1);
	if (ret) {
		dev_err(&pdev->dev, "NAND scan failed with %d\n", ret);
		goto out3;
	}

	mtd_device_register(&ctx->info, pd->parts, pd->num_parts);

	return 0;

out3:
	iounmap(ctx->base);
out2:
	release_mem_region(r->start, resource_size(r));
out1:
	kfree(ctx);
	return ret;
}

<<<<<<< HEAD
static int __devexit au1550nd_remove(struct platform_device *pdev)
=======
static int au1550nd_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct au1550nd_ctx *ctx = platform_get_drvdata(pdev);
	struct resource *r = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	nand_release(&ctx->info);
	iounmap(ctx->base);
	release_mem_region(r->start, 0x1000);
	kfree(ctx);
	return 0;
}

static struct platform_driver au1550nd_driver = {
	.driver = {
		.name	= "au1550-nand",
		.owner	= THIS_MODULE,
	},
	.probe		= au1550nd_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(au1550nd_remove),
};

module_platform_driver(au1550nd_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove		= au1550nd_remove,
};

module_platform_driver(au1550nd_driver);
>>>>>>> refs/remotes/origin/master

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Edge, LLC");
MODULE_DESCRIPTION("Board-specific glue layer for NAND flash on Pb1550 board");
