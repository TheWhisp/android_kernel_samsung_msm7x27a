#
# Makefile for the linux kernel.
#

<<<<<<< HEAD
# Common objects
<<<<<<< HEAD
obj-y				:= timer.o console.o clock.o pm_runtime.o
=======
obj-y				:= timer.o console.o clock.o
>>>>>>> refs/remotes/origin/cm-10.0

# CPU objects
obj-$(CONFIG_ARCH_SH7367)	+= setup-sh7367.o clock-sh7367.o intc-sh7367.o
obj-$(CONFIG_ARCH_SH7377)	+= setup-sh7377.o clock-sh7377.o intc-sh7377.o
obj-$(CONFIG_ARCH_SH7372)	+= setup-sh7372.o clock-sh7372.o intc-sh7372.o
obj-$(CONFIG_ARCH_SH73A0)	+= setup-sh73a0.o clock-sh73a0.o intc-sh73a0.o
<<<<<<< HEAD
=======
obj-$(CONFIG_ARCH_R8A7740)	+= setup-r8a7740.o clock-r8a7740.o intc-r8a7740.o
obj-$(CONFIG_ARCH_R8A7779)	+= setup-r8a7779.o clock-r8a7779.o intc-r8a7779.o
>>>>>>> refs/remotes/origin/cm-10.0

# SMP objects
smp-y				:= platsmp.o headsmp.o
smp-$(CONFIG_HOTPLUG_CPU)	+= hotplug.o
<<<<<<< HEAD
smp-$(CONFIG_LOCAL_TIMERS)	+= localtimer.o
smp-$(CONFIG_ARCH_SH73A0)	+= smp-sh73a0.o
=======
smp-$(CONFIG_ARCH_SH73A0)	+= smp-sh73a0.o
smp-$(CONFIG_ARCH_R8A7779)	+= smp-r8a7779.o
>>>>>>> refs/remotes/origin/cm-10.0

# Pinmux setup
pfc-y				:=
pfc-$(CONFIG_ARCH_SH7367)	+= pfc-sh7367.o
pfc-$(CONFIG_ARCH_SH7377)	+= pfc-sh7377.o
pfc-$(CONFIG_ARCH_SH7372)	+= pfc-sh7372.o
pfc-$(CONFIG_ARCH_SH73A0)	+= pfc-sh73a0.o
<<<<<<< HEAD
=======
pfc-$(CONFIG_ARCH_R8A7740)	+= pfc-r8a7740.o
pfc-$(CONFIG_ARCH_R8A7779)	+= pfc-r8a7779.o
>>>>>>> refs/remotes/origin/cm-10.0

# IRQ objects
obj-$(CONFIG_ARCH_SH7367)	+= entry-intc.o
obj-$(CONFIG_ARCH_SH7377)	+= entry-intc.o
obj-$(CONFIG_ARCH_SH7372)	+= entry-intc.o
<<<<<<< HEAD
obj-$(CONFIG_ARCH_SH73A0)	+= entry-gic.o
=======
obj-$(CONFIG_ARCH_R8A7740)	+= entry-intc.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
ccflags-$(CONFIG_ARCH_MULTIPLATFORM) := -I$(srctree)/arch/arm/mach-shmobile/include

# Common objects
obj-y				:= timer.o console.o

# CPU objects
obj-$(CONFIG_ARCH_SH7372)	+= setup-sh7372.o intc-sh7372.o
obj-$(CONFIG_ARCH_SH73A0)	+= setup-sh73a0.o intc-sh73a0.o
obj-$(CONFIG_ARCH_R8A73A4)	+= setup-r8a73a4.o
obj-$(CONFIG_ARCH_R8A7740)	+= setup-r8a7740.o
obj-$(CONFIG_ARCH_R8A7778)	+= setup-r8a7778.o
obj-$(CONFIG_ARCH_R8A7779)	+= setup-r8a7779.o
obj-$(CONFIG_ARCH_R8A7790)	+= setup-r8a7790.o
obj-$(CONFIG_ARCH_R8A7790)	+= setup-r8a7790.o setup-rcar-gen2.o
obj-$(CONFIG_ARCH_R8A7791)	+= setup-r8a7791.o setup-rcar-gen2.o
obj-$(CONFIG_ARCH_EMEV2)	+= setup-emev2.o
obj-$(CONFIG_ARCH_R7S72100)	+= setup-r7s72100.o

# Clock objects
ifndef CONFIG_COMMON_CLK
obj-y				+= clock.o
obj-$(CONFIG_ARCH_SH7372)	+= clock-sh7372.o
obj-$(CONFIG_ARCH_SH73A0)	+= clock-sh73a0.o
obj-$(CONFIG_ARCH_R8A73A4)	+= clock-r8a73a4.o
obj-$(CONFIG_ARCH_R8A7740)	+= clock-r8a7740.o
obj-$(CONFIG_ARCH_R8A7778)	+= clock-r8a7778.o
obj-$(CONFIG_ARCH_R8A7779)	+= clock-r8a7779.o
obj-$(CONFIG_ARCH_R8A7790)	+= clock-r8a7790.o
obj-$(CONFIG_ARCH_R8A7791)	+= clock-r8a7791.o
obj-$(CONFIG_ARCH_EMEV2)	+= clock-emev2.o
obj-$(CONFIG_ARCH_R7S72100)	+= clock-r7s72100.o
endif

# SMP objects
smp-y				:= platsmp.o headsmp.o
smp-$(CONFIG_ARCH_SH73A0)	+= smp-sh73a0.o headsmp-scu.o platsmp-scu.o
smp-$(CONFIG_ARCH_R8A7779)	+= smp-r8a7779.o headsmp-scu.o platsmp-scu.o
smp-$(CONFIG_ARCH_R8A7790)	+= smp-r8a7790.o platsmp-apmu.o
smp-$(CONFIG_ARCH_R8A7791)	+= smp-r8a7791.o platsmp-apmu.o
smp-$(CONFIG_ARCH_EMEV2)	+= smp-emev2.o headsmp-scu.o platsmp-scu.o

# IRQ objects
obj-$(CONFIG_ARCH_SH7372)	+= entry-intc.o
>>>>>>> refs/remotes/origin/master

# PM objects
obj-$(CONFIG_SUSPEND)		+= suspend.o
obj-$(CONFIG_CPU_IDLE)		+= cpuidle.o
<<<<<<< HEAD
obj-$(CONFIG_ARCH_SH7372)	+= pm-sh7372.o sleep-sh7372.o
<<<<<<< HEAD
=======
obj-$(CONFIG_ARCH_R8A7779)	+= pm-r8a7779.o
>>>>>>> refs/remotes/origin/cm-10.0

# Board objects
obj-$(CONFIG_MACH_G3EVM)	+= board-g3evm.o
obj-$(CONFIG_MACH_G4EVM)	+= board-g4evm.o
obj-$(CONFIG_MACH_AP4EVB)	+= board-ap4evb.o
obj-$(CONFIG_MACH_AG5EVM)	+= board-ag5evm.o
obj-$(CONFIG_MACH_MACKEREL)	+= board-mackerel.o
<<<<<<< HEAD
=======
obj-$(CONFIG_MACH_KOTA2)	+= board-kota2.o
obj-$(CONFIG_MACH_BONITO)	+= board-bonito.o
obj-$(CONFIG_MACH_MARZEN)	+= board-marzen.o
>>>>>>> refs/remotes/origin/cm-10.0

# Framework support
obj-$(CONFIG_SMP)		+= $(smp-y)
obj-$(CONFIG_GENERIC_GPIO)	+= $(pfc-y)
=======
obj-$(CONFIG_ARCH_SH7372)	+= pm-sh7372.o sleep-sh7372.o pm-rmobile.o
obj-$(CONFIG_ARCH_SH73A0)	+= pm-sh73a0.o
obj-$(CONFIG_ARCH_R8A7740)	+= pm-r8a7740.o pm-rmobile.o
obj-$(CONFIG_ARCH_R8A7779)	+= pm-r8a7779.o

# Board objects
ifdef CONFIG_ARCH_SHMOBILE_MULTI
obj-$(CONFIG_MACH_KZM9D)	+= board-kzm9d-reference.o
else
obj-$(CONFIG_MACH_APE6EVM)	+= board-ape6evm.o
obj-$(CONFIG_MACH_APE6EVM_REFERENCE)	+= board-ape6evm-reference.o
obj-$(CONFIG_MACH_MACKEREL)	+= board-mackerel.o
obj-$(CONFIG_MACH_BOCKW)	+= board-bockw.o
obj-$(CONFIG_MACH_BOCKW_REFERENCE)	+= board-bockw-reference.o
obj-$(CONFIG_MACH_GENMAI)	+= board-genmai.o
obj-$(CONFIG_MACH_MARZEN)	+= board-marzen.o
obj-$(CONFIG_MACH_MARZEN_REFERENCE)	+= board-marzen-reference.o
obj-$(CONFIG_MACH_LAGER)	+= board-lager.o
obj-$(CONFIG_MACH_LAGER_REFERENCE)	+= board-lager-reference.o
obj-$(CONFIG_MACH_ARMADILLO800EVA)	+= board-armadillo800eva.o
obj-$(CONFIG_MACH_ARMADILLO800EVA_REFERENCE)	+= board-armadillo800eva-reference.o
obj-$(CONFIG_MACH_KOELSCH)	+= board-koelsch.o
obj-$(CONFIG_MACH_KZM9D)	+= board-kzm9d.o
obj-$(CONFIG_MACH_KZM9G)	+= board-kzm9g.o
obj-$(CONFIG_MACH_KZM9G_REFERENCE)	+= board-kzm9g-reference.o
endif

# Framework support
obj-$(CONFIG_SMP)		+= $(smp-y)
>>>>>>> refs/remotes/origin/master
