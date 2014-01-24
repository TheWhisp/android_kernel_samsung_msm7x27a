#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 2001 - 2005  Tensilica Inc.
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture

# Core configuration.
# (Use VAR=<xtensa_config> to use another default compiler.)

variant-$(CONFIG_XTENSA_VARIANT_FSF)		:= fsf
variant-$(CONFIG_XTENSA_VARIANT_DC232B)		:= dc232b
<<<<<<< HEAD
=======
variant-$(CONFIG_XTENSA_VARIANT_DC233C)		:= dc233c
>>>>>>> refs/remotes/origin/master
variant-$(CONFIG_XTENSA_VARIANT_S6000)		:= s6000
variant-$(CONFIG_XTENSA_VARIANT_LINUX_CUSTOM)	:= custom

VARIANT = $(variant-y)
export VARIANT

<<<<<<< HEAD
=======
# Test for cross compiling

ifneq ($(VARIANT),)
  COMPILE_ARCH = $(shell uname -m)

  ifneq ($(COMPILE_ARCH), xtensa)
    ifndef CROSS_COMPILE
      CROSS_COMPILE = xtensa_$(VARIANT)-
    endif
  endif
endif

>>>>>>> refs/remotes/origin/master
# Platform configuration

platform-$(CONFIG_XTENSA_PLATFORM_XT2000)	:= xt2000
platform-$(CONFIG_XTENSA_PLATFORM_ISS)		:= iss
platform-$(CONFIG_XTENSA_PLATFORM_S6105)	:= s6105
<<<<<<< HEAD
=======
platform-$(CONFIG_XTENSA_PLATFORM_XTFPGA)	:= xtfpga
>>>>>>> refs/remotes/origin/master

PLATFORM = $(platform-y)
export PLATFORM

# temporarily until string.h is fixed
<<<<<<< HEAD
KBUILD_CFLAGS += -ffreestanding
=======
KBUILD_CFLAGS += -ffreestanding -D__linux__
>>>>>>> refs/remotes/origin/master

KBUILD_CFLAGS += -pipe -mlongcalls

KBUILD_CFLAGS += $(call cc-option,-mforce-no-pic,)

<<<<<<< HEAD
=======
ifneq ($(CONFIG_LD_NO_RELAX),)
LDFLAGS := --no-relax
endif

ifeq ($(shell echo __XTENSA_EB__ | $(CC) -E - | grep -v "\#"),1)
CHECKFLAGS += -D__XTENSA_EB__
endif
ifeq ($(shell echo __XTENSA_EL__ | $(CC) -E - | grep -v "\#"),1)
CHECKFLAGS += -D__XTENSA_EL__
endif

>>>>>>> refs/remotes/origin/master
vardirs := $(patsubst %,arch/xtensa/variants/%/,$(variant-y))
plfdirs := $(patsubst %,arch/xtensa/platforms/%/,$(platform-y))

ifeq ($(KBUILD_SRC),)
KBUILD_CPPFLAGS += $(patsubst %,-I%include,$(vardirs) $(plfdirs))
else
KBUILD_CPPFLAGS += $(patsubst %,-I$(srctree)/%include,$(vardirs) $(plfdirs))
endif

KBUILD_DEFCONFIG := iss_defconfig

<<<<<<< HEAD
# ramdisk/initrd support
# You need a compressed ramdisk image, named ramdisk.gz in
# arch/xtensa/boot/ramdisk

core-$(CONFIG_EMBEDDED_RAMDISK)	+= arch/xtensa/boot/ramdisk/

# Test for cross compiling

ifneq ($(VARIANT),)
  COMPILE_ARCH = $(shell uname -m)

  ifneq ($(COMPILE_ARCH), xtensa)
    ifndef CROSS_COMPILE
      CROSS_COMPILE = xtensa_$(VARIANT)-
    endif
  endif
endif

# Only build variant and/or platform if it includes a Makefile

buildvar := $(shell test -a $(srctree)/arch/xtensa/variants/$(VARIANT)/Makefile && echo arch/xtensa/variants/$(VARIANT)/)
buildplf := $(shell test -a $(srctree)/arch/xtensa/platforms/$(PLATFORM)/Makefile && echo arch/xtensa/platforms/$(PLATFORM)/)
=======
# Only build variant and/or platform if it includes a Makefile

buildvar := $(shell test -e $(srctree)/arch/xtensa/variants/$(VARIANT)/Makefile && echo arch/xtensa/variants/$(VARIANT)/)
buildplf := $(shell test -e $(srctree)/arch/xtensa/platforms/$(PLATFORM)/Makefile && echo arch/xtensa/platforms/$(PLATFORM)/)
>>>>>>> refs/remotes/origin/master

# Find libgcc.a

LIBGCC := $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

head-y		:= arch/xtensa/kernel/head.o
core-y		+= arch/xtensa/kernel/ arch/xtensa/mm/
core-y		+= $(buildvar) $(buildplf)

libs-y		+= arch/xtensa/lib/ $(LIBGCC)
<<<<<<< HEAD
=======
drivers-$(CONFIG_OPROFILE)	+= arch/xtensa/oprofile/

ifneq ($(CONFIG_BUILTIN_DTB),"")
core-$(CONFIG_OF) += arch/xtensa/boot/dts/
endif
>>>>>>> refs/remotes/origin/master

boot		:= arch/xtensa/boot

all: zImage

bzImage : zImage

<<<<<<< HEAD
zImage zImage.initrd: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $@

define archhelp
  @echo '* zImage      - Compressed kernel image (arch/xtensa/boot/images/zImage.*)'
endef

=======
zImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $@

%.dtb:
	$(Q)$(MAKE) $(build)=$(boot)/dts $(boot)/dts/$@

define archhelp
  @echo '* zImage      - Compressed kernel image (arch/xtensa/boot/images/zImage.*)'
endef
>>>>>>> refs/remotes/origin/master
