#
# Makefile for arch/m68knommu/platform/68360.
#
<<<<<<< HEAD
<<<<<<< HEAD
=======
model-y			  := ram
model-$(CONFIG_ROMKERNEL) := rom
>>>>>>> refs/remotes/origin/cm-10.0
=======
model-y			  := ram
model-$(CONFIG_ROMKERNEL) := rom
>>>>>>> refs/remotes/origin/master

obj-y := config.o commproc.o entry.o ints.o

extra-y := head.o

<<<<<<< HEAD
<<<<<<< HEAD
$(obj)/head.o: $(obj)/head-$(MODEL).o
	ln -sf head-$(MODEL).o $(obj)/head.o
=======
$(obj)/head.o: $(obj)/head-$(model-y).o
	ln -sf head-$(model-y).o $(obj)/head.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
$(obj)/head.o: $(obj)/head-$(model-y).o
	ln -sf head-$(model-y).o $(obj)/head.o
>>>>>>> refs/remotes/origin/master
