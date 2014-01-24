/*
 * drivers/mtd/nand/fsmc_nand.c
 *
 * ST Microelectronics
 * Flexible Static Memory Controller (FSMC)
 * Driver for NAND portions
 *
 * Copyright © 2010 ST Microelectronics
 * Vipin Kumar <vipin.kumar@st.com>
 * Ashish Priyadarshi
 *
 * Based on drivers/mtd/nand/nomadik_nand.c
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/clk.h>
<<<<<<< HEAD
=======
#include <linux/completion.h>
#include <linux/dmaengine.h>
#include <linux/dma-direction.h>
#include <linux/dma-mapping.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/err.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/resource.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/mtd/partitions.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/mtd/fsmc.h>
#include <linux/amba/bus.h>
#include <mtd/mtd-abi.h>

<<<<<<< HEAD
static struct nand_ecclayout fsmc_ecc1_layout = {
=======
static struct nand_ecclayout fsmc_ecc1_128_layout = {
>>>>>>> refs/remotes/origin/cm-10.0
	.eccbytes = 24,
	.eccpos = {2, 3, 4, 18, 19, 20, 34, 35, 36, 50, 51, 52,
		66, 67, 68, 82, 83, 84, 98, 99, 100, 114, 115, 116},
	.oobfree = {
		{.offset = 8, .length = 8},
		{.offset = 24, .length = 8},
		{.offset = 40, .length = 8},
		{.offset = 56, .length = 8},
		{.offset = 72, .length = 8},
		{.offset = 88, .length = 8},
		{.offset = 104, .length = 8},
		{.offset = 120, .length = 8}
	}
};

<<<<<<< HEAD
static struct nand_ecclayout fsmc_ecc4_lp_layout = {
=======
static struct nand_ecclayout fsmc_ecc1_64_layout = {
	.eccbytes = 12,
	.eccpos = {2, 3, 4, 18, 19, 20, 34, 35, 36, 50, 51, 52},
	.oobfree = {
		{.offset = 8, .length = 8},
		{.offset = 24, .length = 8},
		{.offset = 40, .length = 8},
		{.offset = 56, .length = 8},
	}
};

static struct nand_ecclayout fsmc_ecc1_16_layout = {
	.eccbytes = 3,
	.eccpos = {2, 3, 4},
	.oobfree = {
		{.offset = 8, .length = 8},
	}
};

/*
 * ECC4 layout for NAND of pagesize 8192 bytes & OOBsize 256 bytes. 13*16 bytes
 * of OB size is reserved for ECC, Byte no. 0 & 1 reserved for bad block and 46
 * bytes are free for use.
 */
static struct nand_ecclayout fsmc_ecc4_256_layout = {
	.eccbytes = 208,
	.eccpos = {  2,   3,   4,   5,   6,   7,   8,
		9,  10,  11,  12,  13,  14,
		18,  19,  20,  21,  22,  23,  24,
		25,  26,  27,  28,  29,  30,
		34,  35,  36,  37,  38,  39,  40,
		41,  42,  43,  44,  45,  46,
		50,  51,  52,  53,  54,  55,  56,
		57,  58,  59,  60,  61,  62,
		66,  67,  68,  69,  70,  71,  72,
		73,  74,  75,  76,  77,  78,
		82,  83,  84,  85,  86,  87,  88,
		89,  90,  91,  92,  93,  94,
		98,  99, 100, 101, 102, 103, 104,
		105, 106, 107, 108, 109, 110,
		114, 115, 116, 117, 118, 119, 120,
		121, 122, 123, 124, 125, 126,
		130, 131, 132, 133, 134, 135, 136,
		137, 138, 139, 140, 141, 142,
		146, 147, 148, 149, 150, 151, 152,
		153, 154, 155, 156, 157, 158,
		162, 163, 164, 165, 166, 167, 168,
		169, 170, 171, 172, 173, 174,
		178, 179, 180, 181, 182, 183, 184,
		185, 186, 187, 188, 189, 190,
		194, 195, 196, 197, 198, 199, 200,
		201, 202, 203, 204, 205, 206,
		210, 211, 212, 213, 214, 215, 216,
		217, 218, 219, 220, 221, 222,
		226, 227, 228, 229, 230, 231, 232,
		233, 234, 235, 236, 237, 238,
		242, 243, 244, 245, 246, 247, 248,
		249, 250, 251, 252, 253, 254
	},
	.oobfree = {
		{.offset = 15, .length = 3},
		{.offset = 31, .length = 3},
		{.offset = 47, .length = 3},
		{.offset = 63, .length = 3},
		{.offset = 79, .length = 3},
		{.offset = 95, .length = 3},
		{.offset = 111, .length = 3},
		{.offset = 127, .length = 3},
		{.offset = 143, .length = 3},
		{.offset = 159, .length = 3},
		{.offset = 175, .length = 3},
		{.offset = 191, .length = 3},
		{.offset = 207, .length = 3},
		{.offset = 223, .length = 3},
		{.offset = 239, .length = 3},
		{.offset = 255, .length = 1}
	}
};

/*
 * ECC4 layout for NAND of pagesize 4096 bytes & OOBsize 224 bytes. 13*8 bytes
 * of OOB size is reserved for ECC, Byte no. 0 & 1 reserved for bad block & 118
 * bytes are free for use.
 */
static struct nand_ecclayout fsmc_ecc4_224_layout = {
	.eccbytes = 104,
	.eccpos = {  2,   3,   4,   5,   6,   7,   8,
		9,  10,  11,  12,  13,  14,
		18,  19,  20,  21,  22,  23,  24,
		25,  26,  27,  28,  29,  30,
		34,  35,  36,  37,  38,  39,  40,
		41,  42,  43,  44,  45,  46,
		50,  51,  52,  53,  54,  55,  56,
		57,  58,  59,  60,  61,  62,
		66,  67,  68,  69,  70,  71,  72,
		73,  74,  75,  76,  77,  78,
		82,  83,  84,  85,  86,  87,  88,
		89,  90,  91,  92,  93,  94,
		98,  99, 100, 101, 102, 103, 104,
		105, 106, 107, 108, 109, 110,
		114, 115, 116, 117, 118, 119, 120,
		121, 122, 123, 124, 125, 126
	},
	.oobfree = {
		{.offset = 15, .length = 3},
		{.offset = 31, .length = 3},
		{.offset = 47, .length = 3},
		{.offset = 63, .length = 3},
		{.offset = 79, .length = 3},
		{.offset = 95, .length = 3},
		{.offset = 111, .length = 3},
		{.offset = 127, .length = 97}
	}
};

/*
 * ECC4 layout for NAND of pagesize 4096 bytes & OOBsize 128 bytes. 13*8 bytes
 * of OOB size is reserved for ECC, Byte no. 0 & 1 reserved for bad block & 22
 * bytes are free for use.
 */
static struct nand_ecclayout fsmc_ecc4_128_layout = {
>>>>>>> refs/remotes/origin/cm-10.0
	.eccbytes = 104,
	.eccpos = {  2,   3,   4,   5,   6,   7,   8,
		9,  10,  11,  12,  13,  14,
		18,  19,  20,  21,  22,  23,  24,
		25,  26,  27,  28,  29,  30,
		34,  35,  36,  37,  38,  39,  40,
		41,  42,  43,  44,  45,  46,
		50,  51,  52,  53,  54,  55,  56,
		57,  58,  59,  60,  61,  62,
		66,  67,  68,  69,  70,  71,  72,
		73,  74,  75,  76,  77,  78,
		82,  83,  84,  85,  86,  87,  88,
		89,  90,  91,  92,  93,  94,
		98,  99, 100, 101, 102, 103, 104,
		105, 106, 107, 108, 109, 110,
		114, 115, 116, 117, 118, 119, 120,
		121, 122, 123, 124, 125, 126
	},
	.oobfree = {
		{.offset = 15, .length = 3},
		{.offset = 31, .length = 3},
		{.offset = 47, .length = 3},
		{.offset = 63, .length = 3},
		{.offset = 79, .length = 3},
		{.offset = 95, .length = 3},
		{.offset = 111, .length = 3},
		{.offset = 127, .length = 1}
	}
};

/*
<<<<<<< HEAD
=======
 * ECC4 layout for NAND of pagesize 2048 bytes & OOBsize 64 bytes. 13*4 bytes of
 * OOB size is reserved for ECC, Byte no. 0 & 1 reserved for bad block and 10
 * bytes are free for use.
 */
static struct nand_ecclayout fsmc_ecc4_64_layout = {
	.eccbytes = 52,
	.eccpos = {  2,   3,   4,   5,   6,   7,   8,
		9,  10,  11,  12,  13,  14,
		18,  19,  20,  21,  22,  23,  24,
		25,  26,  27,  28,  29,  30,
		34,  35,  36,  37,  38,  39,  40,
		41,  42,  43,  44,  45,  46,
		50,  51,  52,  53,  54,  55,  56,
		57,  58,  59,  60,  61,  62,
	},
	.oobfree = {
		{.offset = 15, .length = 3},
		{.offset = 31, .length = 3},
		{.offset = 47, .length = 3},
		{.offset = 63, .length = 1},
	}
};

/*
 * ECC4 layout for NAND of pagesize 512 bytes & OOBsize 16 bytes. 13 bytes of
 * OOB size is reserved for ECC, Byte no. 4 & 5 reserved for bad block and One
 * byte is free for use.
 */
static struct nand_ecclayout fsmc_ecc4_16_layout = {
	.eccbytes = 13,
	.eccpos = { 0,  1,  2,  3,  6,  7, 8,
		9, 10, 11, 12, 13, 14
	},
	.oobfree = {
		{.offset = 15, .length = 1},
	}
};

/*
>>>>>>> refs/remotes/origin/cm-10.0
 * ECC placement definitions in oobfree type format.
 * There are 13 bytes of ecc for every 512 byte block and it has to be read
 * consecutively and immediately after the 512 byte data block for hardware to
 * generate the error bit offsets in 512 byte data.
 * Managing the ecc bytes in the following way makes it easier for software to
 * read ecc bytes consecutive to data bytes. This way is similar to
 * oobfree structure maintained already in generic nand driver
 */
static struct fsmc_eccplace fsmc_ecc4_lp_place = {
	.eccplace = {
		{.offset = 2, .length = 13},
		{.offset = 18, .length = 13},
		{.offset = 34, .length = 13},
		{.offset = 50, .length = 13},
		{.offset = 66, .length = 13},
		{.offset = 82, .length = 13},
		{.offset = 98, .length = 13},
		{.offset = 114, .length = 13}
	}
};

<<<<<<< HEAD
static struct nand_ecclayout fsmc_ecc4_sp_layout = {
	.eccbytes = 13,
	.eccpos = { 0,  1,  2,  3,  6,  7, 8,
		9, 10, 11, 12, 13, 14
	},
	.oobfree = {
		{.offset = 15, .length = 1},
	}
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct fsmc_eccplace fsmc_ecc4_sp_place = {
	.eccplace = {
		{.offset = 0, .length = 4},
		{.offset = 6, .length = 9}
	}
};

<<<<<<< HEAD
/*
 * Default partition tables to be used if the partition information not
 * provided through platform data.
 *
 * Default partition layout for small page(= 512 bytes) devices
 * Size for "Root file system" is updated in driver based on actual device size
 */
static struct mtd_partition partition_info_16KB_blk[] = {
	{
		.name = "X-loader",
		.offset = 0,
		.size = 4*0x4000,
	},
	{
		.name = "U-Boot",
		.offset = 0x10000,
		.size = 20*0x4000,
	},
	{
		.name = "Kernel",
		.offset = 0x60000,
		.size = 256*0x4000,
	},
	{
		.name = "Root File System",
		.offset = 0x460000,
		.size = 0,
	},
};

/*
 * Default partition layout for large page(> 512 bytes) devices
 * Size for "Root file system" is updated in driver based on actual device size
 */
static struct mtd_partition partition_info_128KB_blk[] = {
	{
		.name = "X-loader",
		.offset = 0,
		.size = 4*0x20000,
	},
	{
		.name = "U-Boot",
		.offset = 0x80000,
		.size = 12*0x20000,
	},
	{
		.name = "Kernel",
		.offset = 0x200000,
		.size = 48*0x20000,
	},
	{
		.name = "Root File System",
		.offset = 0x800000,
		.size = 0,
	},
};

#ifdef CONFIG_MTD_CMDLINE_PARTS
const char *part_probes[] = { "cmdlinepart", NULL };
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
/**
 * struct fsmc_nand_data - structure for FSMC NAND device state
 *
 * @pid:		Part ID on the AMBA PrimeCell format
 * @mtd:		MTD info for a NAND flash.
 * @nand:		Chip related info for a NAND flash.
 * @partitions:		Partition info for a NAND Flash.
 * @nr_partitions:	Total number of partition of a NAND flash.
 *
 * @ecc_place:		ECC placing locations in oobfree type format.
 * @bank:		Bank number for probed device.
 * @clk:		Clock structure for FSMC.
 *
<<<<<<< HEAD
=======
 * @read_dma_chan:	DMA channel for read access
 * @write_dma_chan:	DMA channel for write access to NAND
 * @dma_access_complete: Completion structure
 *
 * @data_pa:		NAND Physical port for Data.
>>>>>>> refs/remotes/origin/cm-10.0
 * @data_va:		NAND port for Data.
 * @cmd_va:		NAND port for Command.
 * @addr_va:		NAND port for Address.
 * @regs_va:		FSMC regs base address.
 */
struct fsmc_nand_data {
	u32			pid;
	struct mtd_info		mtd;
	struct nand_chip	nand;
	struct mtd_partition	*partitions;
	unsigned int		nr_partitions;

	struct fsmc_eccplace	*ecc_place;
	unsigned int		bank;
<<<<<<< HEAD
	struct clk		*clk;

	struct resource		*resregs;
	struct resource		*rescmd;
	struct resource		*resaddr;
	struct resource		*resdata;

=======
	struct device		*dev;
	enum access_mode	mode;
	struct clk		*clk;

	/* DMA related objects */
	struct dma_chan		*read_dma_chan;
	struct dma_chan		*write_dma_chan;
	struct completion	dma_access_complete;

	struct fsmc_nand_timings *dev_timings;

	dma_addr_t		data_pa;
>>>>>>> refs/remotes/origin/cm-10.0
	void __iomem		*data_va;
	void __iomem		*cmd_va;
	void __iomem		*addr_va;
	void __iomem		*regs_va;

	void			(*select_chip)(uint32_t bank, uint32_t busw);
};

/* Assert CS signal based on chipnr */
static void fsmc_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct nand_chip *chip = mtd->priv;
	struct fsmc_nand_data *host;

	host = container_of(mtd, struct fsmc_nand_data, mtd);

	switch (chipnr) {
	case -1:
		chip->cmd_ctrl(mtd, NAND_CMD_NONE, 0 | NAND_CTRL_CHANGE);
		break;
	case 0:
	case 1:
	case 2:
	case 3:
		if (host->select_chip)
			host->select_chip(chipnr,
					chip->options & NAND_BUSWIDTH_16);
		break;

	default:
		BUG();
	}
}

/*
 * fsmc_cmd_ctrl - For facilitaing Hardware access
 * This routine allows hardware specific access to control-lines(ALE,CLE)
 */
static void fsmc_cmd_ctrl(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *this = mtd->priv;
	struct fsmc_nand_data *host = container_of(mtd,
					struct fsmc_nand_data, mtd);
<<<<<<< HEAD
	struct fsmc_regs *regs = host->regs_va;
	unsigned int bank = host->bank;

	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_CLE) {
			this->IO_ADDR_R = (void __iomem *)host->cmd_va;
			this->IO_ADDR_W = (void __iomem *)host->cmd_va;
		} else if (ctrl & NAND_ALE) {
			this->IO_ADDR_R = (void __iomem *)host->addr_va;
			this->IO_ADDR_W = (void __iomem *)host->addr_va;
		} else {
			this->IO_ADDR_R = (void __iomem *)host->data_va;
			this->IO_ADDR_W = (void __iomem *)host->data_va;
		}

		if (ctrl & NAND_NCE) {
			writel(readl(&regs->bank_regs[bank].pc) | FSMC_ENABLE,
					&regs->bank_regs[bank].pc);
		} else {
			writel(readl(&regs->bank_regs[bank].pc) & ~FSMC_ENABLE,
				       &regs->bank_regs[bank].pc);
		}
=======
	void *__iomem *regs = host->regs_va;
	unsigned int bank = host->bank;

	if (ctrl & NAND_CTRL_CHANGE) {
		u32 pc;

		if (ctrl & NAND_CLE) {
			this->IO_ADDR_R = host->cmd_va;
			this->IO_ADDR_W = host->cmd_va;
		} else if (ctrl & NAND_ALE) {
			this->IO_ADDR_R = host->addr_va;
			this->IO_ADDR_W = host->addr_va;
		} else {
			this->IO_ADDR_R = host->data_va;
			this->IO_ADDR_W = host->data_va;
		}

		pc = readl(FSMC_NAND_REG(regs, bank, PC));
		if (ctrl & NAND_NCE)
			pc |= FSMC_ENABLE;
		else
			pc &= ~FSMC_ENABLE;
		writel(pc, FSMC_NAND_REG(regs, bank, PC));
>>>>>>> refs/remotes/origin/cm-10.0
	}

	mb();

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, this->IO_ADDR_W);
}

/*
 * fsmc_nand_setup - FSMC (Flexible Static Memory Controller) init routine
 *
 * This routine initializes timing parameters related to NAND memory access in
 * FSMC registers
 */
<<<<<<< HEAD
static void __init fsmc_nand_setup(struct fsmc_regs *regs, uint32_t bank,
				   uint32_t busw)
{
	uint32_t value = FSMC_DEVTYPE_NAND | FSMC_ENABLE | FSMC_WAITON;

	if (busw)
		writel(value | FSMC_DEVWID_16, &regs->bank_regs[bank].pc);
	else
		writel(value | FSMC_DEVWID_8, &regs->bank_regs[bank].pc);

	writel(readl(&regs->bank_regs[bank].pc) | FSMC_TCLR_1 | FSMC_TAR_1,
	       &regs->bank_regs[bank].pc);
	writel(FSMC_THIZ_1 | FSMC_THOLD_4 | FSMC_TWAIT_6 | FSMC_TSET_0,
	       &regs->bank_regs[bank].comm);
	writel(FSMC_THIZ_1 | FSMC_THOLD_4 | FSMC_TWAIT_6 | FSMC_TSET_0,
	       &regs->bank_regs[bank].attrib);
=======
static void fsmc_nand_setup(void __iomem *regs, uint32_t bank,
			   uint32_t busw, struct fsmc_nand_timings *timings)
{
	uint32_t value = FSMC_DEVTYPE_NAND | FSMC_ENABLE | FSMC_WAITON;
	uint32_t tclr, tar, thiz, thold, twait, tset;
	struct fsmc_nand_timings *tims;
	struct fsmc_nand_timings default_timings = {
		.tclr	= FSMC_TCLR_1,
		.tar	= FSMC_TAR_1,
		.thiz	= FSMC_THIZ_1,
		.thold	= FSMC_THOLD_4,
		.twait	= FSMC_TWAIT_6,
		.tset	= FSMC_TSET_0,
	};

	if (timings)
		tims = timings;
	else
		tims = &default_timings;

	tclr = (tims->tclr & FSMC_TCLR_MASK) << FSMC_TCLR_SHIFT;
	tar = (tims->tar & FSMC_TAR_MASK) << FSMC_TAR_SHIFT;
	thiz = (tims->thiz & FSMC_THIZ_MASK) << FSMC_THIZ_SHIFT;
	thold = (tims->thold & FSMC_THOLD_MASK) << FSMC_THOLD_SHIFT;
	twait = (tims->twait & FSMC_TWAIT_MASK) << FSMC_TWAIT_SHIFT;
	tset = (tims->tset & FSMC_TSET_MASK) << FSMC_TSET_SHIFT;

	if (busw)
		writel(value | FSMC_DEVWID_16, FSMC_NAND_REG(regs, bank, PC));
	else
		writel(value | FSMC_DEVWID_8, FSMC_NAND_REG(regs, bank, PC));

	writel(readl(FSMC_NAND_REG(regs, bank, PC)) | tclr | tar,
			FSMC_NAND_REG(regs, bank, PC));
	writel(thiz | thold | twait | tset, FSMC_NAND_REG(regs, bank, COMM));
	writel(thiz | thold | twait | tset, FSMC_NAND_REG(regs, bank, ATTRIB));
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * fsmc_enable_hwecc - Enables Hardware ECC through FSMC registers
 */
static void fsmc_enable_hwecc(struct mtd_info *mtd, int mode)
{
	struct fsmc_nand_data *host = container_of(mtd,
					struct fsmc_nand_data, mtd);
<<<<<<< HEAD
	struct fsmc_regs *regs = host->regs_va;
	uint32_t bank = host->bank;

	writel(readl(&regs->bank_regs[bank].pc) & ~FSMC_ECCPLEN_256,
		       &regs->bank_regs[bank].pc);
	writel(readl(&regs->bank_regs[bank].pc) & ~FSMC_ECCEN,
			&regs->bank_regs[bank].pc);
	writel(readl(&regs->bank_regs[bank].pc) | FSMC_ECCEN,
			&regs->bank_regs[bank].pc);
=======
	void __iomem *regs = host->regs_va;
	uint32_t bank = host->bank;

	writel(readl(FSMC_NAND_REG(regs, bank, PC)) & ~FSMC_ECCPLEN_256,
			FSMC_NAND_REG(regs, bank, PC));
	writel(readl(FSMC_NAND_REG(regs, bank, PC)) & ~FSMC_ECCEN,
			FSMC_NAND_REG(regs, bank, PC));
	writel(readl(FSMC_NAND_REG(regs, bank, PC)) | FSMC_ECCEN,
			FSMC_NAND_REG(regs, bank, PC));
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * fsmc_read_hwecc_ecc4 - Hardware ECC calculator for ecc4 option supported by
 * FSMC. ECC is 13 bytes for 512 bytes of data (supports error correction up to
 * max of 8-bits)
 */
static int fsmc_read_hwecc_ecc4(struct mtd_info *mtd, const uint8_t *data,
				uint8_t *ecc)
{
	struct fsmc_nand_data *host = container_of(mtd,
					struct fsmc_nand_data, mtd);
<<<<<<< HEAD
	struct fsmc_regs *regs = host->regs_va;
=======
	void __iomem *regs = host->regs_va;
>>>>>>> refs/remotes/origin/cm-10.0
	uint32_t bank = host->bank;
	uint32_t ecc_tmp;
	unsigned long deadline = jiffies + FSMC_BUSY_WAIT_TIMEOUT;

	do {
<<<<<<< HEAD
		if (readl(&regs->bank_regs[bank].sts) & FSMC_CODE_RDY)
=======
		if (readl(FSMC_NAND_REG(regs, bank, STS)) & FSMC_CODE_RDY)
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		else
			cond_resched();
	} while (!time_after_eq(jiffies, deadline));

<<<<<<< HEAD
	ecc_tmp = readl(&regs->bank_regs[bank].ecc1);
=======
	if (time_after_eq(jiffies, deadline)) {
		dev_err(host->dev, "calculate ecc timed out\n");
		return -ETIMEDOUT;
	}

	ecc_tmp = readl(FSMC_NAND_REG(regs, bank, ECC1));
>>>>>>> refs/remotes/origin/cm-10.0
	ecc[0] = (uint8_t) (ecc_tmp >> 0);
	ecc[1] = (uint8_t) (ecc_tmp >> 8);
	ecc[2] = (uint8_t) (ecc_tmp >> 16);
	ecc[3] = (uint8_t) (ecc_tmp >> 24);

<<<<<<< HEAD
	ecc_tmp = readl(&regs->bank_regs[bank].ecc2);
=======
	ecc_tmp = readl(FSMC_NAND_REG(regs, bank, ECC2));
>>>>>>> refs/remotes/origin/cm-10.0
	ecc[4] = (uint8_t) (ecc_tmp >> 0);
	ecc[5] = (uint8_t) (ecc_tmp >> 8);
	ecc[6] = (uint8_t) (ecc_tmp >> 16);
	ecc[7] = (uint8_t) (ecc_tmp >> 24);

<<<<<<< HEAD
	ecc_tmp = readl(&regs->bank_regs[bank].ecc3);
=======
	ecc_tmp = readl(FSMC_NAND_REG(regs, bank, ECC3));
>>>>>>> refs/remotes/origin/cm-10.0
	ecc[8] = (uint8_t) (ecc_tmp >> 0);
	ecc[9] = (uint8_t) (ecc_tmp >> 8);
	ecc[10] = (uint8_t) (ecc_tmp >> 16);
	ecc[11] = (uint8_t) (ecc_tmp >> 24);

<<<<<<< HEAD
	ecc_tmp = readl(&regs->bank_regs[bank].sts);
=======
	ecc_tmp = readl(FSMC_NAND_REG(regs, bank, STS));
>>>>>>> refs/remotes/origin/cm-10.0
	ecc[12] = (uint8_t) (ecc_tmp >> 16);

	return 0;
}

/*
 * fsmc_read_hwecc_ecc1 - Hardware ECC calculator for ecc1 option supported by
 * FSMC. ECC is 3 bytes for 512 bytes of data (supports error correction up to
 * max of 1-bit)
 */
static int fsmc_read_hwecc_ecc1(struct mtd_info *mtd, const uint8_t *data,
				uint8_t *ecc)
{
	struct fsmc_nand_data *host = container_of(mtd,
					struct fsmc_nand_data, mtd);
<<<<<<< HEAD
	struct fsmc_regs *regs = host->regs_va;
	uint32_t bank = host->bank;
	uint32_t ecc_tmp;

	ecc_tmp = readl(&regs->bank_regs[bank].ecc1);
=======
	void __iomem *regs = host->regs_va;
	uint32_t bank = host->bank;
	uint32_t ecc_tmp;

	ecc_tmp = readl(FSMC_NAND_REG(regs, bank, ECC1));
>>>>>>> refs/remotes/origin/cm-10.0
	ecc[0] = (uint8_t) (ecc_tmp >> 0);
	ecc[1] = (uint8_t) (ecc_tmp >> 8);
	ecc[2] = (uint8_t) (ecc_tmp >> 16);

	return 0;
}

<<<<<<< HEAD
=======
/* Count the number of 0's in buff upto a max of max_bits */
static int count_written_bits(uint8_t *buff, int size, int max_bits)
{
	int k, written_bits = 0;

	for (k = 0; k < size; k++) {
		written_bits += hweight8(~buff[k]);
		if (written_bits > max_bits)
			break;
	}

	return written_bits;
}

static void dma_complete(void *param)
{
	struct fsmc_nand_data *host = param;

	complete(&host->dma_access_complete);
}

static int dma_xfer(struct fsmc_nand_data *host, void *buffer, int len,
		enum dma_data_direction direction)
{
	struct dma_chan *chan;
	struct dma_device *dma_dev;
	struct dma_async_tx_descriptor *tx;
	dma_addr_t dma_dst, dma_src, dma_addr;
	dma_cookie_t cookie;
	unsigned long flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT;
	int ret;

	if (direction == DMA_TO_DEVICE)
		chan = host->write_dma_chan;
	else if (direction == DMA_FROM_DEVICE)
		chan = host->read_dma_chan;
	else
		return -EINVAL;

	dma_dev = chan->device;
	dma_addr = dma_map_single(dma_dev->dev, buffer, len, direction);

	if (direction == DMA_TO_DEVICE) {
		dma_src = dma_addr;
		dma_dst = host->data_pa;
		flags |= DMA_COMPL_SRC_UNMAP_SINGLE | DMA_COMPL_SKIP_DEST_UNMAP;
	} else {
		dma_src = host->data_pa;
		dma_dst = dma_addr;
		flags |= DMA_COMPL_DEST_UNMAP_SINGLE | DMA_COMPL_SKIP_SRC_UNMAP;
	}

	tx = dma_dev->device_prep_dma_memcpy(chan, dma_dst, dma_src,
			len, flags);

	if (!tx) {
		dev_err(host->dev, "device_prep_dma_memcpy error\n");
		dma_unmap_single(dma_dev->dev, dma_addr, len, direction);
		return -EIO;
	}

	tx->callback = dma_complete;
	tx->callback_param = host;
	cookie = tx->tx_submit(tx);

	ret = dma_submit_error(cookie);
	if (ret) {
		dev_err(host->dev, "dma_submit_error %d\n", cookie);
		return ret;
	}

	dma_async_issue_pending(chan);

	ret =
	wait_for_completion_interruptible_timeout(&host->dma_access_complete,
				msecs_to_jiffies(3000));
	if (ret <= 0) {
		chan->device->device_control(chan, DMA_TERMINATE_ALL, 0);
		dev_err(host->dev, "wait_for_completion_timeout\n");
		return ret ? ret : -ETIMEDOUT;
	}

	return 0;
}

/*
 * fsmc_write_buf - write buffer to chip
 * @mtd:	MTD device structure
 * @buf:	data buffer
 * @len:	number of bytes to write
 */
static void fsmc_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	int i;
	struct nand_chip *chip = mtd->priv;

	if (IS_ALIGNED((uint32_t)buf, sizeof(uint32_t)) &&
			IS_ALIGNED(len, sizeof(uint32_t))) {
		uint32_t *p = (uint32_t *)buf;
		len = len >> 2;
		for (i = 0; i < len; i++)
			writel(p[i], chip->IO_ADDR_W);
	} else {
		for (i = 0; i < len; i++)
			writeb(buf[i], chip->IO_ADDR_W);
	}
}

/*
 * fsmc_read_buf - read chip data into buffer
 * @mtd:	MTD device structure
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 */
static void fsmc_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	int i;
	struct nand_chip *chip = mtd->priv;

	if (IS_ALIGNED((uint32_t)buf, sizeof(uint32_t)) &&
			IS_ALIGNED(len, sizeof(uint32_t))) {
		uint32_t *p = (uint32_t *)buf;
		len = len >> 2;
		for (i = 0; i < len; i++)
			p[i] = readl(chip->IO_ADDR_R);
	} else {
		for (i = 0; i < len; i++)
			buf[i] = readb(chip->IO_ADDR_R);
	}
}

/*
 * fsmc_read_buf_dma - read chip data into buffer
 * @mtd:	MTD device structure
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 */
static void fsmc_read_buf_dma(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct fsmc_nand_data *host;

	host = container_of(mtd, struct fsmc_nand_data, mtd);
	dma_xfer(host, buf, len, DMA_FROM_DEVICE);
}

/*
 * fsmc_write_buf_dma - write buffer to chip
 * @mtd:	MTD device structure
 * @buf:	data buffer
 * @len:	number of bytes to write
 */
static void fsmc_write_buf_dma(struct mtd_info *mtd, const uint8_t *buf,
		int len)
{
	struct fsmc_nand_data *host;

	host = container_of(mtd, struct fsmc_nand_data, mtd);
	dma_xfer(host, (void *)buf, len, DMA_TO_DEVICE);
}

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * fsmc_read_page_hwecc
 * @mtd:	mtd info structure
 * @chip:	nand chip info structure
 * @buf:	buffer to store read data
 * @page:	page number to read
 *
 * This routine is needed for fsmc version 8 as reading from NAND chip has to be
 * performed in a strict sequence as follows:
 * data(512 byte) -> ecc(13 byte)
 * After this read, fsmc hardware generates and reports error data bits(up to a
 * max of 8 bits)
 */
static int fsmc_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
				 uint8_t *buf, int page)
{
	struct fsmc_nand_data *host = container_of(mtd,
					struct fsmc_nand_data, mtd);
	struct fsmc_eccplace *ecc_place = host->ecc_place;
	int i, j, s, stat, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	uint8_t *p = buf;
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	uint8_t *ecc_code = chip->buffers->ecccode;
	int off, len, group = 0;
	/*
	 * ecc_oob is intentionally taken as uint16_t. In 16bit devices, we
	 * end up reading 14 bytes (7 words) from oob. The local array is
	 * to maintain word alignment
	 */
	uint16_t ecc_oob[7];
	uint8_t *oob = (uint8_t *)&ecc_oob[0];

	for (i = 0, s = 0; s < eccsteps; s++, i += eccbytes, p += eccsize) {
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
		chip->cmdfunc(mtd, NAND_CMD_READ0, s * eccsize, page);
		chip->ecc.hwctl(mtd, NAND_ECC_READ);
		chip->read_buf(mtd, p, eccsize);

		for (j = 0; j < eccbytes;) {
			off = ecc_place->eccplace[group].offset;
			len = ecc_place->eccplace[group].length;
			group++;

			/*
<<<<<<< HEAD
			* length is intentionally kept a higher multiple of 2
			* to read at least 13 bytes even in case of 16 bit NAND
			* devices
			*/
			len = roundup(len, 2);
=======
			 * length is intentionally kept a higher multiple of 2
			 * to read at least 13 bytes even in case of 16 bit NAND
			 * devices
			 */
			if (chip->options & NAND_BUSWIDTH_16)
				len = roundup(len, 2);

>>>>>>> refs/remotes/origin/cm-10.0
			chip->cmdfunc(mtd, NAND_CMD_READOOB, off, page);
			chip->read_buf(mtd, oob + j, len);
			j += len;
		}

<<<<<<< HEAD
		memcpy(&ecc_code[i], oob, 13);
=======
		memcpy(&ecc_code[i], oob, chip->ecc.bytes);
>>>>>>> refs/remotes/origin/cm-10.0
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);

		stat = chip->ecc.correct(mtd, p, &ecc_code[i], &ecc_calc[i]);
		if (stat < 0)
			mtd->ecc_stats.failed++;
		else
			mtd->ecc_stats.corrected += stat;
	}

	return 0;
}

/*
<<<<<<< HEAD
 * fsmc_correct_data
=======
 * fsmc_bch8_correct_data
>>>>>>> refs/remotes/origin/cm-10.0
 * @mtd:	mtd info structure
 * @dat:	buffer of read data
 * @read_ecc:	ecc read from device spare area
 * @calc_ecc:	ecc calculated from read data
 *
 * calc_ecc is a 104 bit information containing maximum of 8 error
 * offset informations of 13 bits each in 512 bytes of read data.
 */
<<<<<<< HEAD
static int fsmc_correct_data(struct mtd_info *mtd, uint8_t *dat,
=======
static int fsmc_bch8_correct_data(struct mtd_info *mtd, uint8_t *dat,
>>>>>>> refs/remotes/origin/cm-10.0
			     uint8_t *read_ecc, uint8_t *calc_ecc)
{
	struct fsmc_nand_data *host = container_of(mtd,
					struct fsmc_nand_data, mtd);
<<<<<<< HEAD
	struct fsmc_regs *regs = host->regs_va;
	unsigned int bank = host->bank;
	uint16_t err_idx[8];
	uint64_t ecc_data[2];
	uint32_t num_err, i;

	/* The calculated ecc is actually the correction index in data */
	memcpy(ecc_data, calc_ecc, 13);
=======
	struct nand_chip *chip = mtd->priv;
	void __iomem *regs = host->regs_va;
	unsigned int bank = host->bank;
	uint32_t err_idx[8];
	uint32_t num_err, i;
	uint32_t ecc1, ecc2, ecc3, ecc4;

	num_err = (readl(FSMC_NAND_REG(regs, bank, STS)) >> 10) & 0xF;

	/* no bit flipping */
	if (likely(num_err == 0))
		return 0;

	/* too many errors */
	if (unlikely(num_err > 8)) {
		/*
		 * This is a temporary erase check. A newly erased page read
		 * would result in an ecc error because the oob data is also
		 * erased to FF and the calculated ecc for an FF data is not
		 * FF..FF.
		 * This is a workaround to skip performing correction in case
		 * data is FF..FF
		 *
		 * Logic:
		 * For every page, each bit written as 0 is counted until these
		 * number of bits are greater than 8 (the maximum correction
		 * capability of FSMC for each 512 + 13 bytes)
		 */

		int bits_ecc = count_written_bits(read_ecc, chip->ecc.bytes, 8);
		int bits_data = count_written_bits(dat, chip->ecc.size, 8);

		if ((bits_ecc + bits_data) <= 8) {
			if (bits_data)
				memset(dat, 0xff, chip->ecc.size);
			return bits_data;
		}

		return -EBADMSG;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * ------------------- calc_ecc[] bit wise -----------|--13 bits--|
	 * |---idx[7]--|--.....-----|---idx[2]--||---idx[1]--||---idx[0]--|
	 *
	 * calc_ecc is a 104 bit information containing maximum of 8 error
	 * offset informations of 13 bits each. calc_ecc is copied into a
	 * uint64_t array and error offset indexes are populated in err_idx
	 * array
	 */
<<<<<<< HEAD
	for (i = 0; i < 8; i++) {
		if (i == 4) {
			err_idx[4] = ((ecc_data[1] & 0x1) << 12) | ecc_data[0];
			ecc_data[1] >>= 1;
			continue;
		}
		err_idx[i] = (ecc_data[i/4] & 0x1FFF);
		ecc_data[i/4] >>= 13;
	}

	num_err = (readl(&regs->bank_regs[bank].sts) >> 10) & 0xF;

	if (num_err == 0xF)
		return -EBADMSG;
=======
	ecc1 = readl(FSMC_NAND_REG(regs, bank, ECC1));
	ecc2 = readl(FSMC_NAND_REG(regs, bank, ECC2));
	ecc3 = readl(FSMC_NAND_REG(regs, bank, ECC3));
	ecc4 = readl(FSMC_NAND_REG(regs, bank, STS));

	err_idx[0] = (ecc1 >> 0) & 0x1FFF;
	err_idx[1] = (ecc1 >> 13) & 0x1FFF;
	err_idx[2] = (((ecc2 >> 0) & 0x7F) << 6) | ((ecc1 >> 26) & 0x3F);
	err_idx[3] = (ecc2 >> 7) & 0x1FFF;
	err_idx[4] = (((ecc3 >> 0) & 0x1) << 12) | ((ecc2 >> 20) & 0xFFF);
	err_idx[5] = (ecc3 >> 1) & 0x1FFF;
	err_idx[6] = (ecc3 >> 14) & 0x1FFF;
	err_idx[7] = (((ecc4 >> 16) & 0xFF) << 5) | ((ecc3 >> 27) & 0x1F);
>>>>>>> refs/remotes/origin/cm-10.0

	i = 0;
	while (num_err--) {
		change_bit(0, (unsigned long *)&err_idx[i]);
		change_bit(1, (unsigned long *)&err_idx[i]);

<<<<<<< HEAD
		if (err_idx[i] <= 512 * 8) {
=======
		if (err_idx[i] < chip->ecc.size * 8) {
>>>>>>> refs/remotes/origin/cm-10.0
			change_bit(err_idx[i], (unsigned long *)dat);
			i++;
		}
	}
	return i;
}

<<<<<<< HEAD
=======
static bool filter(struct dma_chan *chan, void *slave)
{
	chan->private = slave;
	return true;
}

#ifdef CONFIG_OF
static int __devinit fsmc_nand_probe_config_dt(struct platform_device *pdev,
					       struct device_node *np)
{
	struct fsmc_nand_platform_data *pdata = dev_get_platdata(&pdev->dev);
	u32 val;

	/* Set default NAND width to 8 bits */
	pdata->width = 8;
	if (!of_property_read_u32(np, "bank-width", &val)) {
		if (val == 2) {
			pdata->width = 16;
		} else if (val != 1) {
			dev_err(&pdev->dev, "invalid bank-width %u\n", val);
			return -EINVAL;
		}
	}
	of_property_read_u32(np, "st,ale-off", &pdata->ale_off);
	of_property_read_u32(np, "st,cle-off", &pdata->cle_off);
	if (of_get_property(np, "nand-skip-bbtscan", NULL))
		pdata->options = NAND_SKIP_BBTSCAN;

	return 0;
}
#else
static int __devinit fsmc_nand_probe_config_dt(struct platform_device *pdev,
					       struct device_node *np)
{
	return -ENOSYS;
}
#endif

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * fsmc_nand_probe - Probe function
 * @pdev:       platform device structure
 */
static int __init fsmc_nand_probe(struct platform_device *pdev)
{
	struct fsmc_nand_platform_data *pdata = dev_get_platdata(&pdev->dev);
<<<<<<< HEAD
	struct fsmc_nand_data *host;
	struct mtd_info *mtd;
	struct nand_chip *nand;
	struct fsmc_regs *regs;
	struct resource *res;
=======
	struct device_node __maybe_unused *np = pdev->dev.of_node;
	struct mtd_part_parser_data ppdata = {};
	struct fsmc_nand_data *host;
	struct mtd_info *mtd;
	struct nand_chip *nand;
	struct resource *res;
	dma_cap_mask_t mask;
>>>>>>> refs/remotes/origin/cm-10.0
	int ret = 0;
	u32 pid;
	int i;

<<<<<<< HEAD
=======
	if (np) {
		pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
		pdev->dev.platform_data = pdata;
		ret = fsmc_nand_probe_config_dt(pdev, np);
		if (ret) {
			dev_err(&pdev->dev, "no platform data\n");
			return -ENODEV;
		}
	}

>>>>>>> refs/remotes/origin/cm-10.0
	if (!pdata) {
		dev_err(&pdev->dev, "platform data is NULL\n");
		return -EINVAL;
	}

	/* Allocate memory for the device structure (and zero it) */
<<<<<<< HEAD
	host = kzalloc(sizeof(*host), GFP_KERNEL);
=======
	host = devm_kzalloc(&pdev->dev, sizeof(*host), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!host) {
		dev_err(&pdev->dev, "failed to allocate device structure\n");
		return -ENOMEM;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "nand_data");
<<<<<<< HEAD
	if (!res) {
		ret = -EIO;
		goto err_probe1;
	}

	host->resdata = request_mem_region(res->start, resource_size(res),
			pdev->name);
	if (!host->resdata) {
		ret = -EIO;
		goto err_probe1;
	}

	host->data_va = ioremap(res->start, resource_size(res));
	if (!host->data_va) {
		ret = -EIO;
		goto err_probe1;
	}

	host->resaddr = request_mem_region(res->start + PLAT_NAND_ALE,
			resource_size(res), pdev->name);
	if (!host->resaddr) {
		ret = -EIO;
		goto err_probe1;
	}

	host->addr_va = ioremap(res->start + PLAT_NAND_ALE, resource_size(res));
	if (!host->addr_va) {
		ret = -EIO;
		goto err_probe1;
	}

	host->rescmd = request_mem_region(res->start + PLAT_NAND_CLE,
			resource_size(res), pdev->name);
	if (!host->rescmd) {
		ret = -EIO;
		goto err_probe1;
	}

	host->cmd_va = ioremap(res->start + PLAT_NAND_CLE, resource_size(res));
	if (!host->cmd_va) {
		ret = -EIO;
		goto err_probe1;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "fsmc_regs");
	if (!res) {
		ret = -EIO;
		goto err_probe1;
	}

	host->resregs = request_mem_region(res->start, resource_size(res),
			pdev->name);
	if (!host->resregs) {
		ret = -EIO;
		goto err_probe1;
	}

	host->regs_va = ioremap(res->start, resource_size(res));
	if (!host->regs_va) {
		ret = -EIO;
		goto err_probe1;
=======
	if (!res)
		return -EINVAL;

	if (!devm_request_mem_region(&pdev->dev, res->start, resource_size(res),
				pdev->name)) {
		dev_err(&pdev->dev, "Failed to get memory data resourse\n");
		return -ENOENT;
	}

	host->data_pa = (dma_addr_t)res->start;
	host->data_va = devm_ioremap(&pdev->dev, res->start,
			resource_size(res));
	if (!host->data_va) {
		dev_err(&pdev->dev, "data ioremap failed\n");
		return -ENOMEM;
	}

	if (!devm_request_mem_region(&pdev->dev, res->start + pdata->ale_off,
			resource_size(res), pdev->name)) {
		dev_err(&pdev->dev, "Failed to get memory ale resourse\n");
		return -ENOENT;
	}

	host->addr_va = devm_ioremap(&pdev->dev, res->start + pdata->ale_off,
			resource_size(res));
	if (!host->addr_va) {
		dev_err(&pdev->dev, "ale ioremap failed\n");
		return -ENOMEM;
	}

	if (!devm_request_mem_region(&pdev->dev, res->start + pdata->cle_off,
			resource_size(res), pdev->name)) {
		dev_err(&pdev->dev, "Failed to get memory cle resourse\n");
		return -ENOENT;
	}

	host->cmd_va = devm_ioremap(&pdev->dev, res->start + pdata->cle_off,
			resource_size(res));
	if (!host->cmd_va) {
		dev_err(&pdev->dev, "ale ioremap failed\n");
		return -ENOMEM;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "fsmc_regs");
	if (!res)
		return -EINVAL;

	if (!devm_request_mem_region(&pdev->dev, res->start, resource_size(res),
			pdev->name)) {
		dev_err(&pdev->dev, "Failed to get memory regs resourse\n");
		return -ENOENT;
	}

	host->regs_va = devm_ioremap(&pdev->dev, res->start,
			resource_size(res));
	if (!host->regs_va) {
		dev_err(&pdev->dev, "regs ioremap failed\n");
		return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	host->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(host->clk)) {
		dev_err(&pdev->dev, "failed to fetch block clock\n");
<<<<<<< HEAD
		ret = PTR_ERR(host->clk);
		host->clk = NULL;
		goto err_probe1;
=======
		return PTR_ERR(host->clk);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	ret = clk_enable(host->clk);
	if (ret)
<<<<<<< HEAD
		goto err_probe1;
=======
		goto err_clk_enable;
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * This device ID is actually a common AMBA ID as used on the
	 * AMBA PrimeCell bus. However it is not a PrimeCell.
	 */
	for (pid = 0, i = 0; i < 4; i++)
		pid |= (readl(host->regs_va + resource_size(res) - 0x20 + 4 * i) & 255) << (i * 8);
	host->pid = pid;
	dev_info(&pdev->dev, "FSMC device partno %03x, manufacturer %02x, "
		 "revision %02x, config %02x\n",
		 AMBA_PART_BITS(pid), AMBA_MANF_BITS(pid),
		 AMBA_REV_BITS(pid), AMBA_CONFIG_BITS(pid));

	host->bank = pdata->bank;
	host->select_chip = pdata->select_bank;
<<<<<<< HEAD
	regs = host->regs_va;
=======
	host->partitions = pdata->partitions;
	host->nr_partitions = pdata->nr_partitions;
	host->dev = &pdev->dev;
	host->dev_timings = pdata->nand_timings;
	host->mode = pdata->mode;

	if (host->mode == USE_DMA_ACCESS)
		init_completion(&host->dma_access_complete);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Link all private pointers */
	mtd = &host->mtd;
	nand = &host->nand;
	mtd->priv = nand;
	nand->priv = host;

	host->mtd.owner = THIS_MODULE;
	nand->IO_ADDR_R = host->data_va;
	nand->IO_ADDR_W = host->data_va;
	nand->cmd_ctrl = fsmc_cmd_ctrl;
	nand->chip_delay = 30;

	nand->ecc.mode = NAND_ECC_HW;
	nand->ecc.hwctl = fsmc_enable_hwecc;
	nand->ecc.size = 512;
	nand->options = pdata->options;
	nand->select_chip = fsmc_select_chip;
<<<<<<< HEAD
=======
	nand->badblockbits = 7;
>>>>>>> refs/remotes/origin/cm-10.0

	if (pdata->width == FSMC_NAND_BW16)
		nand->options |= NAND_BUSWIDTH_16;

<<<<<<< HEAD
	fsmc_nand_setup(regs, host->bank, nand->options & NAND_BUSWIDTH_16);
=======
	switch (host->mode) {
	case USE_DMA_ACCESS:
		dma_cap_zero(mask);
		dma_cap_set(DMA_MEMCPY, mask);
		host->read_dma_chan = dma_request_channel(mask, filter,
				pdata->read_dma_priv);
		if (!host->read_dma_chan) {
			dev_err(&pdev->dev, "Unable to get read dma channel\n");
			goto err_req_read_chnl;
		}
		host->write_dma_chan = dma_request_channel(mask, filter,
				pdata->write_dma_priv);
		if (!host->write_dma_chan) {
			dev_err(&pdev->dev, "Unable to get write dma channel\n");
			goto err_req_write_chnl;
		}
		nand->read_buf = fsmc_read_buf_dma;
		nand->write_buf = fsmc_write_buf_dma;
		break;

	default:
	case USE_WORD_ACCESS:
		nand->read_buf = fsmc_read_buf;
		nand->write_buf = fsmc_write_buf;
		break;
	}

	fsmc_nand_setup(host->regs_va, host->bank,
			nand->options & NAND_BUSWIDTH_16,
			host->dev_timings);
>>>>>>> refs/remotes/origin/cm-10.0

	if (AMBA_REV_BITS(host->pid) >= 8) {
		nand->ecc.read_page = fsmc_read_page_hwecc;
		nand->ecc.calculate = fsmc_read_hwecc_ecc4;
<<<<<<< HEAD
		nand->ecc.correct = fsmc_correct_data;
		nand->ecc.bytes = 13;
=======
		nand->ecc.correct = fsmc_bch8_correct_data;
		nand->ecc.bytes = 13;
		nand->ecc.strength = 8;
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		nand->ecc.calculate = fsmc_read_hwecc_ecc1;
		nand->ecc.correct = nand_correct_data;
		nand->ecc.bytes = 3;
<<<<<<< HEAD
=======
		nand->ecc.strength = 1;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/*
	 * Scan to find existence of the device
	 */
	if (nand_scan_ident(&host->mtd, 1, NULL)) {
		ret = -ENXIO;
		dev_err(&pdev->dev, "No NAND Device found!\n");
<<<<<<< HEAD
		goto err_probe;
	}

	if (AMBA_REV_BITS(host->pid) >= 8) {
		if (host->mtd.writesize == 512) {
			nand->ecc.layout = &fsmc_ecc4_sp_layout;
			host->ecc_place = &fsmc_ecc4_sp_place;
		} else {
			nand->ecc.layout = &fsmc_ecc4_lp_layout;
			host->ecc_place = &fsmc_ecc4_lp_place;
		}
	} else {
		nand->ecc.layout = &fsmc_ecc1_layout;
=======
		goto err_scan_ident;
	}

	if (AMBA_REV_BITS(host->pid) >= 8) {
		switch (host->mtd.oobsize) {
		case 16:
			nand->ecc.layout = &fsmc_ecc4_16_layout;
			host->ecc_place = &fsmc_ecc4_sp_place;
			break;
		case 64:
			nand->ecc.layout = &fsmc_ecc4_64_layout;
			host->ecc_place = &fsmc_ecc4_lp_place;
			break;
		case 128:
			nand->ecc.layout = &fsmc_ecc4_128_layout;
			host->ecc_place = &fsmc_ecc4_lp_place;
			break;
		case 224:
			nand->ecc.layout = &fsmc_ecc4_224_layout;
			host->ecc_place = &fsmc_ecc4_lp_place;
			break;
		case 256:
			nand->ecc.layout = &fsmc_ecc4_256_layout;
			host->ecc_place = &fsmc_ecc4_lp_place;
			break;
		default:
			printk(KERN_WARNING "No oob scheme defined for "
			       "oobsize %d\n", mtd->oobsize);
			BUG();
		}
	} else {
		switch (host->mtd.oobsize) {
		case 16:
			nand->ecc.layout = &fsmc_ecc1_16_layout;
			break;
		case 64:
			nand->ecc.layout = &fsmc_ecc1_64_layout;
			break;
		case 128:
			nand->ecc.layout = &fsmc_ecc1_128_layout;
			break;
		default:
			printk(KERN_WARNING "No oob scheme defined for "
			       "oobsize %d\n", mtd->oobsize);
			BUG();
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* Second stage of scan to fill MTD data-structures */
	if (nand_scan_tail(&host->mtd)) {
		ret = -ENXIO;
		goto err_probe;
	}

	/*
	 * The partition information can is accessed by (in the same precedence)
	 *
	 * command line through Bootloader,
	 * platform data,
	 * default partition information present in driver.
	 */
<<<<<<< HEAD
#ifdef CONFIG_MTD_CMDLINE_PARTS
	/*
	 * Check if partition info passed via command line
	 */
	host->mtd.name = "nand";
	host->nr_partitions = parse_mtd_partitions(&host->mtd, part_probes,
			&host->partitions, 0);
	if (host->nr_partitions <= 0) {
#endif
		/*
		 * Check if partition info passed via command line
		 */
		if (pdata->partitions) {
			host->partitions = pdata->partitions;
			host->nr_partitions = pdata->nr_partitions;
		} else {
			struct mtd_partition *partition;
			int i;

			/* Select the default partitions info */
			switch (host->mtd.size) {
			case 0x01000000:
			case 0x02000000:
			case 0x04000000:
				host->partitions = partition_info_16KB_blk;
				host->nr_partitions =
					sizeof(partition_info_16KB_blk) /
					sizeof(struct mtd_partition);
				break;
			case 0x08000000:
			case 0x10000000:
			case 0x20000000:
			case 0x40000000:
				host->partitions = partition_info_128KB_blk;
				host->nr_partitions =
					sizeof(partition_info_128KB_blk) /
					sizeof(struct mtd_partition);
				break;
			default:
				ret = -ENXIO;
				pr_err("Unsupported NAND size\n");
				goto err_probe;
			}

			partition = host->partitions;
			for (i = 0; i < host->nr_partitions; i++, partition++) {
				if (partition->size == 0) {
					partition->size = host->mtd.size -
						partition->offset;
					break;
				}
			}
		}
#ifdef CONFIG_MTD_CMDLINE_PARTS
	}
#endif

	ret = mtd_device_register(&host->mtd, host->partitions,
				  host->nr_partitions);
=======
	/*
	 * Check for partition info passed
	 */
	host->mtd.name = "nand";
	ppdata.of_node = np;
	ret = mtd_device_parse_register(&host->mtd, NULL, &ppdata,
					host->partitions, host->nr_partitions);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		goto err_probe;

	platform_set_drvdata(pdev, host);
	dev_info(&pdev->dev, "FSMC NAND driver registration successful\n");
	return 0;

err_probe:
<<<<<<< HEAD
	clk_disable(host->clk);
err_probe1:
	if (host->clk)
		clk_put(host->clk);
	if (host->regs_va)
		iounmap(host->regs_va);
	if (host->resregs)
		release_mem_region(host->resregs->start,
				resource_size(host->resregs));
	if (host->cmd_va)
		iounmap(host->cmd_va);
	if (host->rescmd)
		release_mem_region(host->rescmd->start,
				resource_size(host->rescmd));
	if (host->addr_va)
		iounmap(host->addr_va);
	if (host->resaddr)
		release_mem_region(host->resaddr->start,
				resource_size(host->resaddr));
	if (host->data_va)
		iounmap(host->data_va);
	if (host->resdata)
		release_mem_region(host->resdata->start,
				resource_size(host->resdata));

	kfree(host);
=======
err_scan_ident:
	if (host->mode == USE_DMA_ACCESS)
		dma_release_channel(host->write_dma_chan);
err_req_write_chnl:
	if (host->mode == USE_DMA_ACCESS)
		dma_release_channel(host->read_dma_chan);
err_req_read_chnl:
	clk_disable(host->clk);
err_clk_enable:
	clk_put(host->clk);
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

/*
 * Clean up routine
 */
static int fsmc_nand_remove(struct platform_device *pdev)
{
	struct fsmc_nand_data *host = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);

	if (host) {
<<<<<<< HEAD
		mtd_device_unregister(&host->mtd);
		clk_disable(host->clk);
		clk_put(host->clk);

		iounmap(host->regs_va);
		release_mem_region(host->resregs->start,
				resource_size(host->resregs));
		iounmap(host->cmd_va);
		release_mem_region(host->rescmd->start,
				resource_size(host->rescmd));
		iounmap(host->addr_va);
		release_mem_region(host->resaddr->start,
				resource_size(host->resaddr));
		iounmap(host->data_va);
		release_mem_region(host->resdata->start,
				resource_size(host->resdata));

		kfree(host);
	}
=======
		nand_release(&host->mtd);

		if (host->mode == USE_DMA_ACCESS) {
			dma_release_channel(host->write_dma_chan);
			dma_release_channel(host->read_dma_chan);
		}
		clk_disable(host->clk);
		clk_put(host->clk);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

#ifdef CONFIG_PM
static int fsmc_nand_suspend(struct device *dev)
{
	struct fsmc_nand_data *host = dev_get_drvdata(dev);
	if (host)
		clk_disable(host->clk);
	return 0;
}

static int fsmc_nand_resume(struct device *dev)
{
	struct fsmc_nand_data *host = dev_get_drvdata(dev);
<<<<<<< HEAD
	if (host)
		clk_enable(host->clk);
	return 0;
}

static const struct dev_pm_ops fsmc_nand_pm_ops = {
	.suspend = fsmc_nand_suspend,
	.resume = fsmc_nand_resume,
};
=======
	if (host) {
		clk_enable(host->clk);
		fsmc_nand_setup(host->regs_va, host->bank,
				host->nand.options & NAND_BUSWIDTH_16,
				host->dev_timings);
	}
	return 0;
}

static SIMPLE_DEV_PM_OPS(fsmc_nand_pm_ops, fsmc_nand_suspend, fsmc_nand_resume);
#endif

#ifdef CONFIG_OF
static const struct of_device_id fsmc_nand_id_table[] = {
	{ .compatible = "st,spear600-fsmc-nand" },
	{}
};
MODULE_DEVICE_TABLE(of, fsmc_nand_id_table);
>>>>>>> refs/remotes/origin/cm-10.0
#endif

static struct platform_driver fsmc_nand_driver = {
	.remove = fsmc_nand_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "fsmc-nand",
<<<<<<< HEAD
=======
		.of_match_table = of_match_ptr(fsmc_nand_id_table),
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_PM
		.pm = &fsmc_nand_pm_ops,
#endif
	},
};

static int __init fsmc_nand_init(void)
{
	return platform_driver_probe(&fsmc_nand_driver,
				     fsmc_nand_probe);
}
module_init(fsmc_nand_init);

static void __exit fsmc_nand_exit(void)
{
	platform_driver_unregister(&fsmc_nand_driver);
}
module_exit(fsmc_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vipin Kumar <vipin.kumar@st.com>, Ashish Priyadarshi");
MODULE_DESCRIPTION("NAND driver for SPEAr Platforms");
