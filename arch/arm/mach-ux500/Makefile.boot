<<<<<<< HEAD
<<<<<<< HEAD
   zreladdr-y	:= 0x00008000
params_phys-y	:= 0x00000100
initrd_phys-y	:= 0x00800000

=======
   zreladdr-y	+= 0x00008000
params_phys-y	:= 0x00000100
initrd_phys-y	:= 0x00800000

dtb-$(CONFIG_MACH_SNOWBALL) += snowball.dtb
>>>>>>> refs/remotes/origin/cm-10.0
=======
   zreladdr-y	+= 0x00008000
params_phys-y	:= 0x00000100
initrd_phys-y	:= 0x00800000
>>>>>>> refs/remotes/origin/master
