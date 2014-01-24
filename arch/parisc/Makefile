#
# parisc/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1994 by Linus Torvalds
# Portions Copyright (C) 1999 The Puffin Group
#
# Modified for PA-RISC Linux by Paul Lahaie, Alex deVries, 
# Mike Shaver, Helge Deller and Martin K. Petersen
#

<<<<<<< HEAD
=======
KBUILD_IMAGE := vmlinuz

>>>>>>> refs/remotes/origin/master
KBUILD_DEFCONFIG := default_defconfig

NM		= sh $(srctree)/arch/parisc/nm
CHECKFLAGS	+= -D__hppa__=1
<<<<<<< HEAD

MACHINE		:= $(shell uname -m)
ifeq ($(MACHINE),parisc*)
NATIVE		:= 1
endif
=======
LIBGCC		= $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)
>>>>>>> refs/remotes/origin/master

ifdef CONFIG_64BIT
UTS_MACHINE	:= parisc64
CHECKFLAGS	+= -D__LP64__=1 -m64
<<<<<<< HEAD
WIDTH		:= 64
<<<<<<< HEAD
CROSS_COMPILE	:= hppa64-linux-gnu-
=======

# FIXME: if no default set, should really try to locate dynamically
ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE	:= hppa64-linux-gnu-
endif
>>>>>>> refs/remotes/origin/cm-10.0
else # 32-bit
WIDTH		:=
endif

# attempt to help out folks who are cross-compiling
ifeq ($(NATIVE),1)
CROSS_COMPILE	:= hppa$(WIDTH)-linux-
=======
CC_ARCHES	= hppa64
else # 32-bit
CC_ARCHES	= hppa hppa2.0 hppa1.1
endif

ifneq ($(SUBARCH),$(UTS_MACHINE))
	ifeq ($(CROSS_COMPILE),)
		CC_SUFFIXES = linux linux-gnu unknown-linux-gnu
		CROSS_COMPILE := $(call cc-cross-prefix, \
			$(foreach a,$(CC_ARCHES), \
			$(foreach s,$(CC_SUFFIXES),$(a)-$(s)-)))
	endif
>>>>>>> refs/remotes/origin/master
endif

OBJCOPY_FLAGS =-O binary -R .note -R .comment -S

cflags-y	:= -pipe

# These flags should be implied by an hppa-linux configuration, but they
# are not in gcc 3.2.
cflags-y	+= -mno-space-regs -mfast-indirect-calls

# Currently we save and restore fpregs on all kernel entry/interruption paths.
# If that gets optimized, we might need to disable the use of fpregs in the
# kernel.
cflags-y	+= -mdisable-fpregs

# Without this, "ld -r" results in .text sections that are too big
# (> 0x40000) for branches to reach stubs.
ifndef CONFIG_FUNCTION_TRACER
  cflags-y	+= -ffunction-sections
endif

<<<<<<< HEAD
# select which processor to optimise for
cflags-$(CONFIG_PA7100)		+= -march=1.1 -mschedule=7100
=======
# Use long jumps instead of long branches (needed if your linker fails to
# link a too big vmlinux executable). Not enabled for building modules.
ifdef CONFIG_MLONGCALLS
KBUILD_CFLAGS_KERNEL += -mlong-calls
endif

# select which processor to optimise for
cflags-$(CONFIG_PA7000)		+= -march=1.1 -mschedule=7100
>>>>>>> refs/remotes/origin/master
cflags-$(CONFIG_PA7200)		+= -march=1.1 -mschedule=7200
cflags-$(CONFIG_PA7100LC)	+= -march=1.1 -mschedule=7100LC
cflags-$(CONFIG_PA7300LC)	+= -march=1.1 -mschedule=7300
cflags-$(CONFIG_PA8X00)		+= -march=2.0 -mschedule=8000

head-y			:= arch/parisc/kernel/head.o 

KBUILD_CFLAGS	+= $(cflags-y)

<<<<<<< HEAD
kernel-y			:= mm/ kernel/ math-emu/ kernel/init_task.o
kernel-$(CONFIG_HPUX)		+= hpux/

core-y	+= $(addprefix arch/parisc/, $(kernel-y))
libs-y	+= arch/parisc/lib/ `$(CC) -print-libgcc-file-name`
=======
kernel-y			:= mm/ kernel/ math-emu/
kernel-$(CONFIG_HPUX)		+= hpux/

core-y	+= $(addprefix arch/parisc/, $(kernel-y))
libs-y	+= arch/parisc/lib/ $(LIBGCC)
>>>>>>> refs/remotes/origin/master

drivers-$(CONFIG_OPROFILE)		+= arch/parisc/oprofile/

PALO := $(shell if (which palo 2>&1); then : ; \
	elif [ -x /sbin/palo ]; then echo /sbin/palo; \
	fi)

PALOCONF := $(shell if [ -f $(src)/palo.conf ]; then echo $(src)/palo.conf; \
	else echo $(obj)/palo.conf; \
	fi)

<<<<<<< HEAD
palo: vmlinux
=======
palo lifimage: vmlinuz
>>>>>>> refs/remotes/origin/master
	@if test ! -x "$(PALO)"; then \
		echo 'ERROR: Please install palo first (apt-get install palo)';\
		echo 'or build it from source and install it somewhere in your $$PATH';\
		false; \
	fi
	@if test ! -f "$(PALOCONF)"; then \
		cp $(src)/arch/parisc/defpalo.conf $(obj)/palo.conf; \
		echo 'A generic palo config file ($(obj)/palo.conf) has been created for you.'; \
		echo 'You should check it and re-run "make palo".'; \
		echo 'WARNING: the "lifimage" file is now placed in this directory by default!'; \
		false; \
	fi
	$(PALO) -f $(PALOCONF)

<<<<<<< HEAD
# Shorthands for known targets not supported by parisc, use vmlinux as default
Image zImage bzImage: vmlinux

kernel_install: vmlinux
	sh $(src)/arch/parisc/install.sh \
			$(KERNELRELEASE) $< System.map "$(INSTALL_PATH)"

install: kernel_install modules_install
=======
BOOT_TARGETS    = zImage Image palo lifimage
INSTALL_TARGETS = zinstall install

PHONY += bzImage $(BOOT_TARGETS) $(INSTALL_TARGETS)

bzImage zImage: vmlinuz
Image: vmlinux

vmlinuz: vmlinux
	@gzip -cf -9 $< > $@

install:
	$(CONFIG_SHELL) $(src)/arch/parisc/install.sh \
			$(KERNELRELEASE) vmlinux System.map "$(INSTALL_PATH)"
zinstall:
	$(CONFIG_SHELL) $(src)/arch/parisc/install.sh \
			$(KERNELRELEASE) vmlinuz System.map "$(INSTALL_PATH)"
>>>>>>> refs/remotes/origin/master

CLEAN_FILES	+= lifimage
MRPROPER_FILES	+= palo.conf

define archhelp
	@echo  '* vmlinux	- Uncompressed kernel image (./vmlinux)'
<<<<<<< HEAD
	@echo  '  palo		- Bootable image (./lifimage)'
	@echo  '  install	- Install kernel using'
	@echo  '		  (your) ~/bin/$(INSTALLKERNEL) or'
	@echo  '		  (distribution) /sbin/$(INSTALLKERNEL) or'
	@echo  '		  copy to $$(INSTALL_PATH)'
=======
	@echo  '  vmlinuz	- Compressed kernel image (./vmlinuz)'
	@echo  '  palo		- Bootable image (./lifimage)'
	@echo  '  install	- Install uncompressed vmlinux kernel using'
	@echo  '		  (your) ~/bin/$(INSTALLKERNEL) or'
	@echo  '		  (distribution) /sbin/$(INSTALLKERNEL) or'
	@echo  '		  copy to $$(INSTALL_PATH)'
	@echo  '  zinstall	- Install compressed vmlinuz kernel'
>>>>>>> refs/remotes/origin/master
endef

# we require gcc 3.3 or above to compile the kernel
archprepare: checkbin
checkbin:
	@if test "$(call cc-version)" -lt "0303"; then \
		echo -n "Sorry, GCC v3.3 or above is required to build " ; \
		echo "the kernel." ; \
		false ; \
	fi
