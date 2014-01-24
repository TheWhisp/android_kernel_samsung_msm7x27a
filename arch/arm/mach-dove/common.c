/*
 * arch/arm/mach-dove/common.c
 *
 * Core functions for Marvell Dove 88AP510 System On Chip
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pci.h>
<<<<<<< HEAD
#include <linux/serial_8250.h>
#include <linux/clk.h>
#include <linux/mbus.h>
#include <linux/ata_platform.h>
#include <linux/serial_8250.h>
=======
#include <linux/clk.h>
#include <linux/ata_platform.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/gpio.h>
#include <asm/page.h>
#include <asm/setup.h>
#include <asm/timex.h>
#include <asm/hardware/cache-tauros2.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>
#include <asm/mach/pci.h>
#include <mach/dove.h>
#include <mach/bridge-regs.h>
#include <asm/mach/arch.h>
#include <linux/irq.h>
#include <plat/time.h>
#include <plat/ehci-orion.h>
#include <plat/common.h>
<<<<<<< HEAD
=======
#include <plat/addr-map.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "common.h"

static int get_tclk(void);
=======
#include <linux/clk-provider.h>
#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_data/dma-mv_xor.h>
#include <linux/platform_data/usb-ehci-orion.h>
#include <linux/platform_device.h>
#include <asm/hardware/cache-tauros2.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>
#include <mach/bridge-regs.h>
#include <mach/pm.h>
#include <plat/common.h>
#include <plat/irq.h>
#include <plat/time.h>
#include "common.h"

/* These can go away once Dove uses the mvebu-mbus DT binding */
#define DOVE_MBUS_PCIE0_MEM_TARGET    0x4
#define DOVE_MBUS_PCIE0_MEM_ATTR      0xe8
#define DOVE_MBUS_PCIE0_IO_TARGET     0x4
#define DOVE_MBUS_PCIE0_IO_ATTR       0xe0
#define DOVE_MBUS_PCIE1_MEM_TARGET    0x8
#define DOVE_MBUS_PCIE1_MEM_ATTR      0xe8
#define DOVE_MBUS_PCIE1_IO_TARGET     0x8
#define DOVE_MBUS_PCIE1_IO_ATTR       0xe0
#define DOVE_MBUS_CESA_TARGET         0x3
#define DOVE_MBUS_CESA_ATTR           0x1
#define DOVE_MBUS_BOOTROM_TARGET      0x1
#define DOVE_MBUS_BOOTROM_ATTR        0xfd
#define DOVE_MBUS_SCRATCHPAD_TARGET   0xd
#define DOVE_MBUS_SCRATCHPAD_ATTR     0x0
>>>>>>> refs/remotes/origin/master

/*****************************************************************************
 * I/O Address Mapping
 ****************************************************************************/
static struct map_desc dove_io_desc[] __initdata = {
	{
<<<<<<< HEAD
		.virtual	= DOVE_SB_REGS_VIRT_BASE,
=======
		.virtual	= (unsigned long) DOVE_SB_REGS_VIRT_BASE,
>>>>>>> refs/remotes/origin/master
		.pfn		= __phys_to_pfn(DOVE_SB_REGS_PHYS_BASE),
		.length		= DOVE_SB_REGS_SIZE,
		.type		= MT_DEVICE,
	}, {
<<<<<<< HEAD
		.virtual	= DOVE_NB_REGS_VIRT_BASE,
		.pfn		= __phys_to_pfn(DOVE_NB_REGS_PHYS_BASE),
		.length		= DOVE_NB_REGS_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= DOVE_PCIE0_IO_VIRT_BASE,
		.pfn		= __phys_to_pfn(DOVE_PCIE0_IO_PHYS_BASE),
		.length		= DOVE_PCIE0_IO_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= DOVE_PCIE1_IO_VIRT_BASE,
		.pfn		= __phys_to_pfn(DOVE_PCIE1_IO_PHYS_BASE),
		.length		= DOVE_PCIE1_IO_SIZE,
		.type		= MT_DEVICE,
=======
		.virtual	= (unsigned long) DOVE_NB_REGS_VIRT_BASE,
		.pfn		= __phys_to_pfn(DOVE_NB_REGS_PHYS_BASE),
		.length		= DOVE_NB_REGS_SIZE,
		.type		= MT_DEVICE,
>>>>>>> refs/remotes/origin/master
	},
};

void __init dove_map_io(void)
{
	iotable_init(dove_io_desc, ARRAY_SIZE(dove_io_desc));
}

/*****************************************************************************
<<<<<<< HEAD
=======
 * CLK tree
 ****************************************************************************/
static int dove_tclk;

static DEFINE_SPINLOCK(gating_lock);
static struct clk *tclk;

static struct clk __init *dove_register_gate(const char *name,
					     const char *parent, u8 bit_idx)
{
	return clk_register_gate(NULL, name, parent, 0,
				 (void __iomem *)CLOCK_GATING_CONTROL,
				 bit_idx, 0, &gating_lock);
}

static void __init dove_clk_init(void)
{
	struct clk *usb0, *usb1, *sata, *pex0, *pex1, *sdio0, *sdio1;
	struct clk *nand, *camera, *i2s0, *i2s1, *crypto, *ac97, *pdma;
	struct clk *xor0, *xor1, *ge, *gephy;

	tclk = clk_register_fixed_rate(NULL, "tclk", NULL, CLK_IS_ROOT,
				       dove_tclk);

	usb0 = dove_register_gate("usb0", "tclk", CLOCK_GATING_BIT_USB0);
	usb1 = dove_register_gate("usb1", "tclk", CLOCK_GATING_BIT_USB1);
	sata = dove_register_gate("sata", "tclk", CLOCK_GATING_BIT_SATA);
	pex0 = dove_register_gate("pex0", "tclk", CLOCK_GATING_BIT_PCIE0);
	pex1 = dove_register_gate("pex1", "tclk", CLOCK_GATING_BIT_PCIE1);
	sdio0 = dove_register_gate("sdio0", "tclk", CLOCK_GATING_BIT_SDIO0);
	sdio1 = dove_register_gate("sdio1", "tclk", CLOCK_GATING_BIT_SDIO1);
	nand = dove_register_gate("nand", "tclk", CLOCK_GATING_BIT_NAND);
	camera = dove_register_gate("camera", "tclk", CLOCK_GATING_BIT_CAMERA);
	i2s0 = dove_register_gate("i2s0", "tclk", CLOCK_GATING_BIT_I2S0);
	i2s1 = dove_register_gate("i2s1", "tclk", CLOCK_GATING_BIT_I2S1);
	crypto = dove_register_gate("crypto", "tclk", CLOCK_GATING_BIT_CRYPTO);
	ac97 = dove_register_gate("ac97", "tclk", CLOCK_GATING_BIT_AC97);
	pdma = dove_register_gate("pdma", "tclk", CLOCK_GATING_BIT_PDMA);
	xor0 = dove_register_gate("xor0", "tclk", CLOCK_GATING_BIT_XOR0);
	xor1 = dove_register_gate("xor1", "tclk", CLOCK_GATING_BIT_XOR1);
	gephy = dove_register_gate("gephy", "tclk", CLOCK_GATING_BIT_GIGA_PHY);
	ge = dove_register_gate("ge", "gephy", CLOCK_GATING_BIT_GBE);

	orion_clkdev_add(NULL, "orion_spi.0", tclk);
	orion_clkdev_add(NULL, "orion_spi.1", tclk);
	orion_clkdev_add(NULL, "orion_wdt", tclk);
	orion_clkdev_add(NULL, "mv64xxx_i2c.0", tclk);

	orion_clkdev_add(NULL, "orion-ehci.0", usb0);
	orion_clkdev_add(NULL, "orion-ehci.1", usb1);
	orion_clkdev_add(NULL, "mv643xx_eth_port.0", ge);
	orion_clkdev_add(NULL, "sata_mv.0", sata);
	orion_clkdev_add("0", "pcie", pex0);
	orion_clkdev_add("1", "pcie", pex1);
	orion_clkdev_add(NULL, "sdhci-dove.0", sdio0);
	orion_clkdev_add(NULL, "sdhci-dove.1", sdio1);
	orion_clkdev_add(NULL, "orion_nand", nand);
	orion_clkdev_add(NULL, "cafe1000-ccic.0", camera);
	orion_clkdev_add(NULL, "mvebu-audio.0", i2s0);
	orion_clkdev_add(NULL, "mvebu-audio.1", i2s1);
	orion_clkdev_add(NULL, "mv_crypto", crypto);
	orion_clkdev_add(NULL, "dove-ac97", ac97);
	orion_clkdev_add(NULL, "dove-pdma", pdma);
	orion_clkdev_add(NULL, MV_XOR_NAME ".0", xor0);
	orion_clkdev_add(NULL, MV_XOR_NAME ".1", xor1);
}

/*****************************************************************************
>>>>>>> refs/remotes/origin/master
 * EHCI0
 ****************************************************************************/
void __init dove_ehci0_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	orion_ehci_init(&dove_mbus_dram_info,
			DOVE_USB0_PHYS_BASE, IRQ_DOVE_USB0, EHCI_PHY_NA);
=======
	orion_ehci_init(DOVE_USB0_PHYS_BASE, IRQ_DOVE_USB0, EHCI_PHY_NA);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	orion_ehci_init(DOVE_USB0_PHYS_BASE, IRQ_DOVE_USB0, EHCI_PHY_NA);
>>>>>>> refs/remotes/origin/master
}

/*****************************************************************************
 * EHCI1
 ****************************************************************************/
void __init dove_ehci1_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	orion_ehci_1_init(&dove_mbus_dram_info,
			  DOVE_USB1_PHYS_BASE, IRQ_DOVE_USB1);
=======
	orion_ehci_1_init(DOVE_USB1_PHYS_BASE, IRQ_DOVE_USB1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	orion_ehci_1_init(DOVE_USB1_PHYS_BASE, IRQ_DOVE_USB1);
>>>>>>> refs/remotes/origin/master
}

/*****************************************************************************
 * GE00
 ****************************************************************************/
void __init dove_ge00_init(struct mv643xx_eth_platform_data *eth_data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	orion_ge00_init(eth_data, &dove_mbus_dram_info,
=======
	orion_ge00_init(eth_data,
>>>>>>> refs/remotes/origin/cm-10.0
			DOVE_GE00_PHYS_BASE, IRQ_DOVE_GE00_SUM,
			0, get_tclk());
=======
	orion_ge00_init(eth_data, DOVE_GE00_PHYS_BASE,
			IRQ_DOVE_GE00_SUM, IRQ_DOVE_GE00_ERR,
			1600);
>>>>>>> refs/remotes/origin/master
}

/*****************************************************************************
 * SoC RTC
 ****************************************************************************/
void __init dove_rtc_init(void)
{
	orion_rtc_init(DOVE_RTC_PHYS_BASE, IRQ_DOVE_RTC);
}

/*****************************************************************************
 * SATA
 ****************************************************************************/
void __init dove_sata_init(struct mv_sata_platform_data *sata_data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	orion_sata_init(sata_data, &dove_mbus_dram_info,
			DOVE_SATA_PHYS_BASE, IRQ_DOVE_SATA);
=======
	orion_sata_init(sata_data, DOVE_SATA_PHYS_BASE, IRQ_DOVE_SATA);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	orion_sata_init(sata_data, DOVE_SATA_PHYS_BASE, IRQ_DOVE_SATA);
>>>>>>> refs/remotes/origin/master

}

/*****************************************************************************
 * UART0
 ****************************************************************************/
void __init dove_uart0_init(void)
{
	orion_uart0_init(DOVE_UART0_VIRT_BASE, DOVE_UART0_PHYS_BASE,
<<<<<<< HEAD
			 IRQ_DOVE_UART_0, get_tclk());
=======
			 IRQ_DOVE_UART_0, tclk);
>>>>>>> refs/remotes/origin/master
}

/*****************************************************************************
 * UART1
 ****************************************************************************/
void __init dove_uart1_init(void)
{
	orion_uart1_init(DOVE_UART1_VIRT_BASE, DOVE_UART1_PHYS_BASE,
<<<<<<< HEAD
			 IRQ_DOVE_UART_1, get_tclk());
=======
			 IRQ_DOVE_UART_1, tclk);
>>>>>>> refs/remotes/origin/master
}

/*****************************************************************************
 * UART2
 ****************************************************************************/
void __init dove_uart2_init(void)
{
	orion_uart2_init(DOVE_UART2_VIRT_BASE, DOVE_UART2_PHYS_BASE,
<<<<<<< HEAD
			 IRQ_DOVE_UART_2, get_tclk());
=======
			 IRQ_DOVE_UART_2, tclk);
>>>>>>> refs/remotes/origin/master
}

/*****************************************************************************
 * UART3
 ****************************************************************************/
void __init dove_uart3_init(void)
{
	orion_uart3_init(DOVE_UART3_VIRT_BASE, DOVE_UART3_PHYS_BASE,
<<<<<<< HEAD
			 IRQ_DOVE_UART_3, get_tclk());
=======
			 IRQ_DOVE_UART_3, tclk);
>>>>>>> refs/remotes/origin/master
}

/*****************************************************************************
 * SPI
 ****************************************************************************/
void __init dove_spi0_init(void)
{
<<<<<<< HEAD
	orion_spi_init(DOVE_SPI0_PHYS_BASE, get_tclk());
=======
	orion_spi_init(DOVE_SPI0_PHYS_BASE);
>>>>>>> refs/remotes/origin/master
}

void __init dove_spi1_init(void)
{
<<<<<<< HEAD
	orion_spi_1_init(DOVE_SPI1_PHYS_BASE, get_tclk());
=======
	orion_spi_1_init(DOVE_SPI1_PHYS_BASE);
>>>>>>> refs/remotes/origin/master
}

/*****************************************************************************
 * I2C
 ****************************************************************************/
void __init dove_i2c_init(void)
{
	orion_i2c_init(DOVE_I2C_PHYS_BASE, IRQ_DOVE_I2C, 10);
}

/*****************************************************************************
 * Time handling
 ****************************************************************************/
void __init dove_init_early(void)
{
	orion_time_set_base(TIMER_VIRT_BASE);
<<<<<<< HEAD
}

static int get_tclk(void)
{
	/* use DOVE_RESET_SAMPLE_HI/LO to detect tclk */
	return 166666667;
}

static void dove_timer_init(void)
{
	orion_time_init(BRIDGE_VIRT_BASE, BRIDGE_INT_TIMER1_CLR,
			IRQ_DOVE_BRIDGE, get_tclk());
}

struct sys_timer dove_timer = {
	.init = dove_timer_init,
};
=======
	mvebu_mbus_init("marvell,dove-mbus",
			BRIDGE_WINS_BASE, BRIDGE_WINS_SZ,
			DOVE_MC_WINS_BASE, DOVE_MC_WINS_SZ);
}

static int __init dove_find_tclk(void)
{
	return 166666667;
}

void __init dove_timer_init(void)
{
	dove_tclk = dove_find_tclk();
	orion_time_init(BRIDGE_VIRT_BASE, BRIDGE_INT_TIMER1_CLR,
			IRQ_DOVE_BRIDGE, dove_tclk);
}

/*****************************************************************************
 * Cryptographic Engines and Security Accelerator (CESA)
 ****************************************************************************/
void __init dove_crypto_init(void)
{
	orion_crypto_init(DOVE_CRYPT_PHYS_BASE, DOVE_CESA_PHYS_BASE,
			  DOVE_CESA_SIZE, IRQ_DOVE_CRYPTO);
}
>>>>>>> refs/remotes/origin/master

/*****************************************************************************
 * XOR 0
 ****************************************************************************/
void __init dove_xor0_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	orion_xor0_init(&dove_mbus_dram_info,
			DOVE_XOR0_PHYS_BASE, DOVE_XOR0_HIGH_PHYS_BASE,
=======
	orion_xor0_init(DOVE_XOR0_PHYS_BASE, DOVE_XOR0_HIGH_PHYS_BASE,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	orion_xor0_init(DOVE_XOR0_PHYS_BASE, DOVE_XOR0_HIGH_PHYS_BASE,
>>>>>>> refs/remotes/origin/master
			IRQ_DOVE_XOR_00, IRQ_DOVE_XOR_01);
}

/*****************************************************************************
 * XOR 1
 ****************************************************************************/
void __init dove_xor1_init(void)
{
	orion_xor1_init(DOVE_XOR1_PHYS_BASE, DOVE_XOR1_HIGH_PHYS_BASE,
			IRQ_DOVE_XOR_10, IRQ_DOVE_XOR_11);
}

/*****************************************************************************
 * SDIO
 ****************************************************************************/
static u64 sdio_dmamask = DMA_BIT_MASK(32);

static struct resource dove_sdio0_resources[] = {
	{
		.start	= DOVE_SDIO0_PHYS_BASE,
		.end	= DOVE_SDIO0_PHYS_BASE + 0xff,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= IRQ_DOVE_SDIO0,
		.end	= IRQ_DOVE_SDIO0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device dove_sdio0 = {
	.name		= "sdhci-dove",
	.id		= 0,
	.dev		= {
		.dma_mask		= &sdio_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
	.resource	= dove_sdio0_resources,
	.num_resources	= ARRAY_SIZE(dove_sdio0_resources),
};

void __init dove_sdio0_init(void)
{
	platform_device_register(&dove_sdio0);
}

static struct resource dove_sdio1_resources[] = {
	{
		.start	= DOVE_SDIO1_PHYS_BASE,
		.end	= DOVE_SDIO1_PHYS_BASE + 0xff,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= IRQ_DOVE_SDIO1,
		.end	= IRQ_DOVE_SDIO1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device dove_sdio1 = {
	.name		= "sdhci-dove",
	.id		= 1,
	.dev		= {
		.dma_mask		= &sdio_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
	.resource	= dove_sdio1_resources,
	.num_resources	= ARRAY_SIZE(dove_sdio1_resources),
};

void __init dove_sdio1_init(void)
{
	platform_device_register(&dove_sdio1);
}

<<<<<<< HEAD
void __init dove_init(void)
{
	int tclk;

	tclk = get_tclk();

	printk(KERN_INFO "Dove 88AP510 SoC, ");
	printk(KERN_INFO "TCLK = %dMHz\n", (tclk + 499999) / 1000000);

#ifdef CONFIG_CACHE_TAUROS2
	tauros2_init();
#endif
	dove_setup_cpu_mbus();
=======
void __init dove_setup_cpu_wins(void)
{
	/*
	 * The PCIe windows will no longer be statically allocated
	 * here once Dove is migrated to the pci-mvebu driver. The
	 * non-PCIe windows will no longer be created here once Dove
	 * fully moves to DT.
	 */
	mvebu_mbus_add_window_remap_by_id(DOVE_MBUS_PCIE0_IO_TARGET,
					  DOVE_MBUS_PCIE0_IO_ATTR,
					  DOVE_PCIE0_IO_PHYS_BASE,
					  DOVE_PCIE0_IO_SIZE,
					  DOVE_PCIE0_IO_BUS_BASE);
	mvebu_mbus_add_window_remap_by_id(DOVE_MBUS_PCIE1_IO_TARGET,
					  DOVE_MBUS_PCIE1_IO_ATTR,
					  DOVE_PCIE1_IO_PHYS_BASE,
					  DOVE_PCIE1_IO_SIZE,
					  DOVE_PCIE1_IO_BUS_BASE);
	mvebu_mbus_add_window_by_id(DOVE_MBUS_PCIE0_MEM_TARGET,
				    DOVE_MBUS_PCIE0_MEM_ATTR,
				    DOVE_PCIE0_MEM_PHYS_BASE,
				    DOVE_PCIE0_MEM_SIZE);
	mvebu_mbus_add_window_by_id(DOVE_MBUS_PCIE1_MEM_TARGET,
				    DOVE_MBUS_PCIE1_MEM_ATTR,
				    DOVE_PCIE1_MEM_PHYS_BASE,
				    DOVE_PCIE1_MEM_SIZE);
	mvebu_mbus_add_window_by_id(DOVE_MBUS_CESA_TARGET,
				    DOVE_MBUS_CESA_ATTR,
				    DOVE_CESA_PHYS_BASE,
				    DOVE_CESA_SIZE);
	mvebu_mbus_add_window_by_id(DOVE_MBUS_BOOTROM_TARGET,
				    DOVE_MBUS_BOOTROM_ATTR,
				    DOVE_BOOTROM_PHYS_BASE,
				    DOVE_BOOTROM_SIZE);
	mvebu_mbus_add_window_by_id(DOVE_MBUS_SCRATCHPAD_TARGET,
				    DOVE_MBUS_SCRATCHPAD_ATTR,
				    DOVE_SCRATCHPAD_PHYS_BASE,
				    DOVE_SCRATCHPAD_SIZE);
}

void __init dove_init(void)
{
	pr_info("Dove 88AP510 SoC, TCLK = %d MHz.\n",
		(dove_tclk + 499999) / 1000000);

#ifdef CONFIG_CACHE_TAUROS2
	tauros2_init(0);
#endif
	dove_setup_cpu_wins();

	/* Setup root of clk tree */
	dove_clk_init();
>>>>>>> refs/remotes/origin/master

	/* internal devices that every board has */
	dove_rtc_init();
	dove_xor0_init();
	dove_xor1_init();
}
<<<<<<< HEAD
<<<<<<< HEAD
=======

void dove_restart(char mode, const char *cmd)
=======

void dove_restart(enum reboot_mode mode, const char *cmd)
>>>>>>> refs/remotes/origin/master
{
	/*
	 * Enable soft reset to assert RSTOUTn.
	 */
	writel(SOFT_RESET_OUT_EN, RSTOUTn_MASK);

	/*
	 * Assert soft reset.
	 */
	writel(SOFT_RESET, SYSTEM_SOFT_RESET);

	while (1)
		;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
