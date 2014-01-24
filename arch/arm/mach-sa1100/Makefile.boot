<<<<<<< HEAD
<<<<<<< HEAD
   zreladdr-y	:= 0xc0008000
ifeq ($(CONFIG_ARCH_SA1100),y)
   zreladdr-$(CONFIG_SA1111)		:= 0xc0208000
=======
=======
>>>>>>> refs/remotes/origin/master
ifeq ($(CONFIG_SA1111),y)
   zreladdr-y	+= 0xc0208000
else
   zreladdr-y	+= 0xc0008000
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
endif
params_phys-y	:= 0xc0000100
initrd_phys-y	:= 0xc0800000

