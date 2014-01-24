ifeq ($(CONFIG_REALVIEW_HIGH_PHYS_OFFSET),y)
<<<<<<< HEAD
<<<<<<< HEAD
   zreladdr-y	:= 0x70008000
params_phys-y	:= 0x70000100
initrd_phys-y	:= 0x70800000
else
   zreladdr-y	:= 0x00008000
=======
=======
>>>>>>> refs/remotes/origin/master
   zreladdr-y	+= 0x70008000
params_phys-y	:= 0x70000100
initrd_phys-y	:= 0x70800000
else
   zreladdr-y	+= 0x00008000
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
params_phys-y	:= 0x00000100
initrd_phys-y	:= 0x00800000
endif
