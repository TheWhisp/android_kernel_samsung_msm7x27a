<<<<<<< HEAD
ccflags-$(CONFIG_PPC64)			:= -mno-minimal-toc
=======
ccflags-$(CONFIG_PPC64)			:= $(NO_MINIMAL_TOC)
>>>>>>> refs/remotes/origin/master
ccflags-$(CONFIG_PPC_PSERIES_DEBUG)	+= -DDEBUG

obj-y			:= lpar.o hvCall.o nvram.o reconfig.o \
			   setup.o iommu.o event_sources.o ras.o \
<<<<<<< HEAD
			   firmware.o power.o dlpar.o mobility.o
obj-$(CONFIG_SMP)	+= smp.o
obj-$(CONFIG_SCANLOG)	+= scanlog.o
<<<<<<< HEAD
obj-$(CONFIG_EEH)	+= eeh.o eeh_cache.o eeh_driver.o eeh_event.o eeh_sysfs.o
=======
obj-$(CONFIG_EEH)	+= eeh.o eeh_dev.o eeh_cache.o eeh_driver.o \
			   eeh_event.o eeh_sysfs.o eeh_pseries.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
			   firmware.o power.o dlpar.o mobility.o rng.o
obj-$(CONFIG_SMP)	+= smp.o
obj-$(CONFIG_SCANLOG)	+= scanlog.o
obj-$(CONFIG_EEH)	+= eeh_pseries.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_KEXEC)	+= kexec.o
obj-$(CONFIG_PCI)	+= pci.o pci_dlpar.o
obj-$(CONFIG_PSERIES_MSI)	+= msi.o
obj-$(CONFIG_PSERIES_ENERGY)	+= pseries_energy.o

obj-$(CONFIG_HOTPLUG_CPU)	+= hotplug-cpu.o
obj-$(CONFIG_MEMORY_HOTPLUG)	+= hotplug-memory.o

obj-$(CONFIG_HVC_CONSOLE)	+= hvconsole.o
obj-$(CONFIG_HVCS)		+= hvcserver.o
obj-$(CONFIG_HCALL_STATS)	+= hvCall_inst.o
<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_PHYP_DUMP)		+= phyp_dump.o
obj-$(CONFIG_CMM)		+= cmm.o
obj-$(CONFIG_DTL)		+= dtl.o
obj-$(CONFIG_IO_EVENT_IRQ)	+= io_event_irq.o
=======
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_CMM)		+= cmm.o
obj-$(CONFIG_DTL)		+= dtl.o
obj-$(CONFIG_IO_EVENT_IRQ)	+= io_event_irq.o
obj-$(CONFIG_PSERIES_IDLE)	+= processor_idle.o
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-$(CONFIG_LPARCFG)		+= lparcfg.o
>>>>>>> refs/remotes/origin/master

ifeq ($(CONFIG_PPC_PSERIES),y)
obj-$(CONFIG_SUSPEND)		+= suspend.o
endif
