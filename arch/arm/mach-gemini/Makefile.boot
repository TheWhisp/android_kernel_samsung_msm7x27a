ifeq ($(CONFIG_GEMINI_MEM_SWAP),y)
<<<<<<< HEAD
<<<<<<< HEAD
   zreladdr-y	:= 0x00008000
params_phys-y	:= 0x00000100
initrd_phys-y	:= 0x00800000
else
   zreladdr-y	:= 0x10008000
=======
=======
>>>>>>> refs/remotes/origin/master
   zreladdr-y	+= 0x00008000
params_phys-y	:= 0x00000100
initrd_phys-y	:= 0x00800000
else
   zreladdr-y	+= 0x10008000
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
params_phys-y	:= 0x10000100
initrd_phys-y	:= 0x10800000
endif
