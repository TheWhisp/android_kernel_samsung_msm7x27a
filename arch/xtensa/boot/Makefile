#
# arch/xtensa/boot/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
#


# KBUILD_CFLAGS used when building rest of boot (takes effect recursively)
KBUILD_CFLAGS	+= -fno-builtin -Iarch/$(ARCH)/boot/include
HOSTFLAGS	+= -Iarch/$(ARCH)/boot/include

<<<<<<< HEAD
BIG_ENDIAN	:= $(shell echo -e __XTENSA_EB__ | $(CC) -E - | grep -v "\#")
=======
BIG_ENDIAN	:= $(shell echo __XTENSA_EB__ | $(CC) -E - | grep -v "\#")
>>>>>>> refs/remotes/origin/master

export ccflags-y
export BIG_ENDIAN

subdir-y	:= lib

# Subdirs for the boot loader(s)

bootdir-$(CONFIG_XTENSA_PLATFORM_ISS)	 += boot-elf
<<<<<<< HEAD
bootdir-$(CONFIG_XTENSA_PLATFORM_XT2000) += boot-redboot boot-elf


zImage zImage.initrd Image Image.initrd: $(bootdir-y)
=======
bootdir-$(CONFIG_XTENSA_PLATFORM_XT2000) += boot-redboot boot-elf boot-uboot
bootdir-$(CONFIG_XTENSA_PLATFORM_XTFPGA) += boot-redboot boot-elf boot-uboot

zImage Image: $(bootdir-y)
>>>>>>> refs/remotes/origin/master

$(bootdir-y): $(addprefix $(obj)/,$(subdir-y)) \
	      $(addprefix $(obj)/,$(host-progs))
	$(Q)$(MAKE) $(build)=$(obj)/$@ $(MAKECMDGOALS)

<<<<<<< HEAD
=======
OBJCOPYFLAGS = --strip-all -R .comment -R .note.gnu.build-id -O binary

vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

vmlinux.bin.gz: vmlinux.bin FORCE
	$(call if_changed,gzip)

boot-elf: vmlinux.bin
boot-redboot: vmlinux.bin.gz
boot-uboot: vmlinux.bin.gz
>>>>>>> refs/remotes/origin/master
