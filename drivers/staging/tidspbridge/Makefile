<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_TIDSPBRIDGE)	+= bridgedriver.o
=======
obj-$(CONFIG_TIDSPBRIDGE)	+= tidspbridge.o
>>>>>>> refs/remotes/origin/cm-10.0

libgen = gen/gh.o gen/uuidutil.o
=======
obj-$(CONFIG_TIDSPBRIDGE)	+= tidspbridge.o

libgen = gen/gh.o
>>>>>>> refs/remotes/origin/master
libcore = core/chnl_sm.o core/msg_sm.o core/io_sm.o core/tiomap3430.o \
		core/tiomap3430_pwr.o core/tiomap_io.o \
		core/ue_deh.o core/wdt.o core/dsp-clock.o core/sync.o
libpmgr = pmgr/chnl.o pmgr/io.o pmgr/msg.o pmgr/cod.o pmgr/dev.o pmgr/dspapi.o \
		pmgr/dmm.o pmgr/cmm.o pmgr/dbll.o
librmgr = rmgr/dbdcd.o rmgr/disp.o rmgr/drv.o rmgr/mgr.o rmgr/node.o \
		rmgr/proc.o rmgr/pwr.o rmgr/rmm.o rmgr/strm.o rmgr/dspdrv.o \
		rmgr/nldr.o rmgr/drv_interface.o
libdload = dynload/cload.o dynload/getsection.o dynload/reloc.o \
		 dynload/tramp.o
libhw = hw/hw_mmu.o

<<<<<<< HEAD
<<<<<<< HEAD
bridgedriver-y := $(libgen) $(libservices) $(libcore) $(libpmgr) $(librmgr) \
=======
tidspbridge-y := $(libgen) $(libservices) $(libcore) $(libpmgr) $(librmgr) \
>>>>>>> refs/remotes/origin/cm-10.0
=======
tidspbridge-y := $(libgen) $(libservices) $(libcore) $(libpmgr) $(librmgr) \
>>>>>>> refs/remotes/origin/master
			$(libdload) $(libhw)

#Machine dependent
ccflags-y += -D_TI_ -D_DB_TIOMAP -DTMS32060 \
		-DTICFG_PROC_VER -DTICFG_EVM_TYPE -DCHNL_SMCLASS \
		-DCHNL_MESSAGES -DUSE_LEVEL_1_MACROS

ccflags-y += -Idrivers/staging/tidspbridge/include
ccflags-y += -Idrivers/staging/tidspbridge/services
ccflags-y += -Idrivers/staging/tidspbridge/core
ccflags-y += -Idrivers/staging/tidspbridge/pmgr
ccflags-y += -Idrivers/staging/tidspbridge/rmgr
ccflags-y += -Idrivers/staging/tidspbridge/dynload
ccflags-y += -Idrivers/staging/tidspbridge/hw
ccflags-y += -Iarch/arm

