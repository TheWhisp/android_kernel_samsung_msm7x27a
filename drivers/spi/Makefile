#
# Makefile for kernel SPI drivers.
#

ccflags-$(CONFIG_SPI_DEBUG) := -DDEBUG

# small core, mostly translating board-specific
# config declarations into driver model code
obj-$(CONFIG_SPI_MASTER)		+= spi.o
<<<<<<< HEAD
<<<<<<< HEAD

# SPI master controller drivers (bus)
obj-$(CONFIG_SPI_ALTERA)		+= spi_altera.o
obj-$(CONFIG_SPI_ATMEL)			+= atmel_spi.o
obj-$(CONFIG_SPI_ATH79)			+= ath79_spi.o
obj-$(CONFIG_SPI_BFIN)			+= spi_bfin5xx.o
obj-$(CONFIG_SPI_BFIN_SPORT)		+= spi_bfin_sport.o
obj-$(CONFIG_SPI_BITBANG)		+= spi_bitbang.o
obj-$(CONFIG_SPI_AU1550)		+= au1550_spi.o
obj-$(CONFIG_SPI_BUTTERFLY)		+= spi_butterfly.o
obj-$(CONFIG_SPI_COLDFIRE_QSPI)		+= coldfire_qspi.o
obj-$(CONFIG_SPI_DAVINCI)		+= davinci_spi.o
obj-$(CONFIG_SPI_DESIGNWARE)		+= dw_spi.o
obj-$(CONFIG_SPI_DW_PCI)		+= dw_spi_midpci.o
dw_spi_midpci-objs			:= dw_spi_pci.o dw_spi_mid.o
obj-$(CONFIG_SPI_DW_MMIO)		+= dw_spi_mmio.o
obj-$(CONFIG_SPI_EP93XX)		+= ep93xx_spi.o
obj-$(CONFIG_SPI_GPIO)			+= spi_gpio.o
obj-$(CONFIG_SPI_IMX)			+= spi_imx.o
obj-$(CONFIG_SPI_LM70_LLP)		+= spi_lm70llp.o
obj-$(CONFIG_SPI_PXA2XX)		+= pxa2xx_spi.o
obj-$(CONFIG_SPI_PXA2XX_PCI)		+= pxa2xx_spi_pci.o
obj-$(CONFIG_SPI_OC_TINY)		+= spi_oc_tiny.o
obj-$(CONFIG_SPI_OMAP_UWIRE)		+= omap_uwire.o
obj-$(CONFIG_SPI_OMAP24XX)		+= omap2_mcspi.o
obj-$(CONFIG_SPI_OMAP_100K)		+= omap_spi_100k.o
obj-$(CONFIG_SPI_ORION)			+= orion_spi.o
obj-$(CONFIG_SPI_PL022)			+= amba-pl022.o
obj-$(CONFIG_SPI_MPC512x_PSC)		+= mpc512x_psc_spi.o
obj-$(CONFIG_SPI_MPC52xx_PSC)		+= mpc52xx_psc_spi.o
obj-$(CONFIG_SPI_MPC52xx)		+= mpc52xx_spi.o
obj-$(CONFIG_SPI_FSL_LIB)		+= spi_fsl_lib.o
obj-$(CONFIG_SPI_FSL_ESPI)		+= spi_fsl_espi.o
obj-$(CONFIG_SPI_FSL_SPI)		+= spi_fsl_spi.o
obj-$(CONFIG_SPI_PPC4xx)		+= spi_ppc4xx.o
obj-$(CONFIG_SPI_S3C24XX_GPIO)		+= spi_s3c24xx_gpio.o
obj-$(CONFIG_SPI_S3C24XX)		+= spi_s3c24xx_hw.o
obj-$(CONFIG_SPI_S3C64XX)		+= spi_s3c64xx.o
obj-$(CONFIG_SPI_TEGRA)			+= spi_tegra.o
obj-$(CONFIG_SPI_TI_SSP)		+= ti-ssp-spi.o
obj-$(CONFIG_SPI_TOPCLIFF_PCH)		+= spi_topcliff_pch.o
obj-$(CONFIG_SPI_TXX9)			+= spi_txx9.o
obj-$(CONFIG_SPI_XILINX)		+= xilinx_spi.o
obj-$(CONFIG_SPI_SH)			+= spi_sh.o
obj-$(CONFIG_SPI_SH_SCI)		+= spi_sh_sci.o
obj-$(CONFIG_SPI_SH_MSIOF)		+= spi_sh_msiof.o
obj-$(CONFIG_SPI_STMP3XXX)		+= spi_stmp.o
obj-$(CONFIG_SPI_NUC900)		+= spi_nuc900.o
obj-$(CONFIG_SPI_QSD)			+= spi_qsd.o
obj-$(CONFIG_SPI_QUP)			+= spi_qsd.o

# special build for s3c24xx spi driver with fiq support
spi_s3c24xx_hw-y			:= spi_s3c24xx.o
spi_s3c24xx_hw-$(CONFIG_SPI_S3C24XX_FIQ) += spi_s3c24xx_fiq.o

# 	... add above this line ...

# SPI protocol drivers (device/link on bus)
obj-$(CONFIG_SPI_SPIDEV)	+= spidev.o
obj-$(CONFIG_SPI_TLE62X0)	+= tle62x0.o
# 	... add above this line ...

# SPI slave controller drivers (upstream link)
# 	... add above this line ...

# SPI slave drivers (protocol for that link)
# 	... add above this line ...
=======
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_SPI_SPIDEV)		+= spidev.o

# SPI master controller drivers (bus)
obj-$(CONFIG_SPI_ALTERA)		+= spi-altera.o
obj-$(CONFIG_SPI_ATMEL)			+= spi-atmel.o
obj-$(CONFIG_SPI_ATH79)			+= spi-ath79.o
obj-$(CONFIG_SPI_AU1550)		+= spi-au1550.o
<<<<<<< HEAD
obj-$(CONFIG_SPI_BCM63XX)		+= spi-bcm63xx.o
obj-$(CONFIG_SPI_BFIN5XX)		+= spi-bfin5xx.o
obj-$(CONFIG_SPI_BFIN_SPORT)		+= spi-bfin-sport.o
obj-$(CONFIG_SPI_BITBANG)		+= spi-bitbang.o
obj-$(CONFIG_SPI_BUTTERFLY)		+= spi-butterfly.o
=======
obj-$(CONFIG_SPI_BCM2835)		+= spi-bcm2835.o
obj-$(CONFIG_SPI_BCM63XX)		+= spi-bcm63xx.o
obj-$(CONFIG_SPI_BFIN5XX)		+= spi-bfin5xx.o
obj-$(CONFIG_SPI_BFIN_V3)               += spi-bfin-v3.o
obj-$(CONFIG_SPI_BFIN_SPORT)		+= spi-bfin-sport.o
obj-$(CONFIG_SPI_BITBANG)		+= spi-bitbang.o
obj-$(CONFIG_SPI_BUTTERFLY)		+= spi-butterfly.o
obj-$(CONFIG_SPI_CLPS711X)		+= spi-clps711x.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_SPI_COLDFIRE_QSPI)		+= spi-coldfire-qspi.o
obj-$(CONFIG_SPI_DAVINCI)		+= spi-davinci.o
obj-$(CONFIG_SPI_DESIGNWARE)		+= spi-dw.o
obj-$(CONFIG_SPI_DW_MMIO)		+= spi-dw-mmio.o
obj-$(CONFIG_SPI_DW_PCI)		+= spi-dw-midpci.o
spi-dw-midpci-objs			:= spi-dw-pci.o spi-dw-mid.o
<<<<<<< HEAD
obj-$(CONFIG_SPI_EP93XX)		+= spi-ep93xx.o
=======
obj-$(CONFIG_SPI_EFM32)			+= spi-efm32.o
obj-$(CONFIG_SPI_EP93XX)		+= spi-ep93xx.o
obj-$(CONFIG_SPI_FALCON)		+= spi-falcon.o
obj-$(CONFIG_SPI_FSL_CPM)		+= spi-fsl-cpm.o
obj-$(CONFIG_SPI_FSL_DSPI)		+= spi-fsl-dspi.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_SPI_FSL_LIB)		+= spi-fsl-lib.o
obj-$(CONFIG_SPI_FSL_ESPI)		+= spi-fsl-espi.o
obj-$(CONFIG_SPI_FSL_SPI)		+= spi-fsl-spi.o
obj-$(CONFIG_SPI_GPIO)			+= spi-gpio.o
obj-$(CONFIG_SPI_IMX)			+= spi-imx.o
obj-$(CONFIG_SPI_LM70_LLP)		+= spi-lm70llp.o
obj-$(CONFIG_SPI_MPC512x_PSC)		+= spi-mpc512x-psc.o
obj-$(CONFIG_SPI_MPC52xx_PSC)		+= spi-mpc52xx-psc.o
obj-$(CONFIG_SPI_MPC52xx)		+= spi-mpc52xx.o
<<<<<<< HEAD
obj-$(CONFIG_SPI_NUC900)		+= spi-nuc900.o
obj-$(CONFIG_SPI_OC_TINY)		+= spi-oc-tiny.o
obj-$(CONFIG_SPI_OMAP_UWIRE)		+= spi-omap-uwire.o
obj-$(CONFIG_SPI_OMAP_100K)		+= spi-omap-100k.o
obj-$(CONFIG_SPI_OMAP24XX)		+= spi-omap2-mcspi.o
obj-$(CONFIG_SPI_ORION)			+= spi-orion.o
obj-$(CONFIG_SPI_PL022)			+= spi-pl022.o
obj-$(CONFIG_SPI_PPC4xx)		+= spi-ppc4xx.o
obj-$(CONFIG_SPI_PXA2XX)		+= spi-pxa2xx.o
obj-$(CONFIG_SPI_PXA2XX_PCI)		+= spi-pxa2xx-pci.o
obj-$(CONFIG_SPI_QSD)			+= spi_qsd.o
obj-$(CONFIG_SPI_QUP)			+= spi_qsd.o
<<<<<<< HEAD
=======
obj-$(CONFIG_SPI_MXS)			+= spi-mxs.o
obj-$(CONFIG_SPI_NUC900)		+= spi-nuc900.o
obj-$(CONFIG_SPI_OC_TINY)		+= spi-oc-tiny.o
obj-$(CONFIG_SPI_OCTEON)		+= spi-octeon.o
obj-$(CONFIG_SPI_OMAP_UWIRE)		+= spi-omap-uwire.o
obj-$(CONFIG_SPI_OMAP_100K)		+= spi-omap-100k.o
obj-$(CONFIG_SPI_OMAP24XX)		+= spi-omap2-mcspi.o
obj-$(CONFIG_SPI_TI_QSPI)		+= spi-ti-qspi.o
obj-$(CONFIG_SPI_ORION)			+= spi-orion.o
obj-$(CONFIG_SPI_PL022)			+= spi-pl022.o
obj-$(CONFIG_SPI_PPC4xx)		+= spi-ppc4xx.o
spi-pxa2xx-platform-objs		:= spi-pxa2xx.o
spi-pxa2xx-platform-$(CONFIG_SPI_PXA2XX_PXADMA)	+= spi-pxa2xx-pxadma.o
spi-pxa2xx-platform-$(CONFIG_SPI_PXA2XX_DMA)	+= spi-pxa2xx-dma.o
obj-$(CONFIG_SPI_PXA2XX)		+= spi-pxa2xx-platform.o
obj-$(CONFIG_SPI_PXA2XX_PCI)		+= spi-pxa2xx-pci.o
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
obj-$(CONFIG_SPI_RSPI)			+= spi-rspi.o
obj-$(CONFIG_SPI_S3C24XX)		+= spi-s3c24xx-hw.o
spi-s3c24xx-hw-y			:= spi-s3c24xx.o
spi-s3c24xx-hw-$(CONFIG_SPI_S3C24XX_FIQ) += spi-s3c24xx-fiq.o
obj-$(CONFIG_SPI_S3C64XX)		+= spi-s3c64xx.o
<<<<<<< HEAD
=======
obj-$(CONFIG_SPI_SC18IS602)		+= spi-sc18is602.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_SPI_SH)			+= spi-sh.o
obj-$(CONFIG_SPI_SH_HSPI)		+= spi-sh-hspi.o
obj-$(CONFIG_SPI_SH_MSIOF)		+= spi-sh-msiof.o
obj-$(CONFIG_SPI_SH_SCI)		+= spi-sh-sci.o
obj-$(CONFIG_SPI_SIRF)		+= spi-sirf.o
<<<<<<< HEAD
obj-$(CONFIG_SPI_STMP3XXX)		+= spi-stmp.o
obj-$(CONFIG_SPI_TEGRA)			+= spi-tegra.o
=======
obj-$(CONFIG_SPI_TEGRA114)		+= spi-tegra114.o
obj-$(CONFIG_SPI_TEGRA20_SFLASH)	+= spi-tegra20-sflash.o
obj-$(CONFIG_SPI_TEGRA20_SLINK)		+= spi-tegra20-slink.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_SPI_TI_SSP)		+= spi-ti-ssp.o
obj-$(CONFIG_SPI_TLE62X0)		+= spi-tle62x0.o
obj-$(CONFIG_SPI_TOPCLIFF_PCH)		+= spi-topcliff-pch.o
obj-$(CONFIG_SPI_TXX9)			+= spi-txx9.o
<<<<<<< HEAD
obj-$(CONFIG_SPI_XILINX)		+= spi-xilinx.o

>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-$(CONFIG_SPI_XCOMM)		+= spi-xcomm.o
obj-$(CONFIG_SPI_XILINX)		+= spi-xilinx.o
>>>>>>> refs/remotes/origin/master
