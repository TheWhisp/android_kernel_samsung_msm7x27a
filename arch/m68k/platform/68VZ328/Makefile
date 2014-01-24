#
# Makefile for arch/m68knommu/platform/68VZ328.
#

obj-y		:= config.o
<<<<<<< HEAD
logo-$(UCDIMM)	:= bootlogo.rh
logo-$(DRAGEN2)	:= screen.h
extra-y		:= $(logo-y)

$(obj)/bootlogo.rh: $(src)/../68EZ328/bootlogo.h
	perl $(src)/bootlogo.pl < $(src)/../68328/bootlogo.h > $(obj)/bootlogo.rh
=======
extra-$(DRAGEN2):= screen.h
>>>>>>> refs/remotes/origin/cm-10.0

$(obj)/screen.h: $(src)/screen.xbm $(src)/xbm2lcd.pl
	perl $(src)/xbm2lcd.pl < $(src)/screen.xbm > $(obj)/screen.h

<<<<<<< HEAD
clean-files := $(obj)/screen.h $(obj)/bootlogo.rh
=======
clean-files := $(obj)/screen.h
>>>>>>> refs/remotes/origin/cm-10.0
