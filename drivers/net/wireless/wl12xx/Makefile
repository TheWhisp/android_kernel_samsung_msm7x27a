wl12xx-objs		= main.o cmd.o io.o event.o tx.o rx.o ps.o acx.o \
			  boot.o init.o debugfs.o scan.o

<<<<<<< HEAD
wl12xx_spi-objs	= spi.o
wl12xx_sdio-objs	= sdio.o
wl12xx_sdio_test-objs = sdio_test.o
=======
wl12xx_spi-objs 	= spi.o
wl12xx_sdio-objs	= sdio.o
>>>>>>> refs/remotes/origin/cm-10.0

wl12xx-$(CONFIG_NL80211_TESTMODE)	+= testmode.o
obj-$(CONFIG_WL12XX)			+= wl12xx.o
obj-$(CONFIG_WL12XX_SPI)		+= wl12xx_spi.o
obj-$(CONFIG_WL12XX_SDIO)		+= wl12xx_sdio.o

<<<<<<< HEAD
obj-$(CONFIG_WL12XX_SDIO_TEST)	+= wl12xx_sdio_test.o

# small builtin driver bit
obj-$(CONFIG_WL12XX_PLATFORM_DATA)	+= wl12xx_platform_data.o
=======
# small builtin driver bit
obj-$(CONFIG_WL12XX_PLATFORM_DATA)	+= wl12xx_platform_data.o

ccflags-y += -D__CHECK_ENDIAN__
>>>>>>> refs/remotes/origin/cm-10.0
