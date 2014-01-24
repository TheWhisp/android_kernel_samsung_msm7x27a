# Address where decompressor will be written and eventually executed.
#
# default to SDRAM
<<<<<<< HEAD
zreladdr-y      := $(CONFIG_BCM_ZRELADDR)
=======
zreladdr-y      += $(CONFIG_BCM_ZRELADDR)
>>>>>>> refs/remotes/origin/cm-10.0
params_phys-y   := 0x00000800

