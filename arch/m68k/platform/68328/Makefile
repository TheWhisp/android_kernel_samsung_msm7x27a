#
# Makefile for arch/m68knommu/platform/68328.
#

<<<<<<< HEAD
head-y			= head-$(MODEL).o
=======
model-y			  := ram
model-$(CONFIG_ROMKERNEL) := rom

head-y			= head-$(model-y).o
>>>>>>> refs/remotes/origin/cm-10.0
head-$(CONFIG_PILOT)	= head-pilot.o
head-$(CONFIG_DRAGEN2)	= head-de2.o

obj-y			+= entry.o ints.o timers.o
obj-$(CONFIG_M68328)	+= config.o
obj-$(CONFIG_ROM)	+= romvec.o

extra-y			:= head.o
<<<<<<< HEAD
extra-$(CONFIG_M68328)	+= bootlogo.rh head.o

$(obj)/bootlogo.rh: $(src)/bootlogo.h
	perl $(src)/bootlogo.pl < $(src)/bootlogo.h > $(obj)/bootlogo.rh
=======
>>>>>>> refs/remotes/origin/cm-10.0

$(obj)/head.o: $(obj)/$(head-y)
	ln -sf $(head-y) $(obj)/head.o

<<<<<<< HEAD
clean-files := $(obj)/bootlogo.rh $(obj)/head.o $(head-y)
=======
clean-files := $(obj)/head.o $(head-y)
>>>>>>> refs/remotes/origin/cm-10.0
