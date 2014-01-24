#
# arch/microblaze/boot/Makefile
#

<<<<<<< HEAD
MKIMAGE := $(srctree)/scripts/mkuboot.sh

=======
>>>>>>> refs/remotes/origin/cm-10.0
obj-y += linked_dtb.o

targets := linux.bin linux.bin.gz simpleImage.%

<<<<<<< HEAD
OBJCOPYFLAGS := -O binary
=======
OBJCOPYFLAGS := -R .note -R .comment -R .note.gnu.build-id -O binary
>>>>>>> refs/remotes/origin/cm-10.0

# Ensure system.dtb exists
$(obj)/linked_dtb.o: $(obj)/system.dtb

# Generate system.dtb from $(DTB).dtb
ifneq ($(DTB),system)
$(obj)/system.dtb: $(obj)/$(DTB).dtb
	$(call if_changed,cp)
endif

$(obj)/linux.bin: vmlinux FORCE
	$(call if_changed,objcopy)
	$(call if_changed,uimage)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

$(obj)/linux.bin.gz: $(obj)/linux.bin FORCE
	$(call if_changed,gzip)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

quiet_cmd_cp = CP      $< $@$2
	cmd_cp = cat $< >$@$2 || (rm -f $@ && echo false)

quiet_cmd_strip = STRIP   $@
	cmd_strip = $(STRIP) -K microblaze_start -K _end -K __log_buf \
				-K _fdt_start vmlinux -o $@

<<<<<<< HEAD
quiet_cmd_uimage = UIMAGE  $@.ub
	cmd_uimage = $(CONFIG_SHELL) $(MKIMAGE) -A microblaze -O linux -T kernel \
		-C none -n 'Linux-$(KERNELRELEASE)' \
		-a $(CONFIG_KERNEL_BASE_ADDR) -e $(CONFIG_KERNEL_BASE_ADDR) \
		-d $@ $@.ub
=======
UIMAGE_IN = $@
UIMAGE_OUT = $@.ub
UIMAGE_LOADADDR = $(CONFIG_KERNEL_BASE_ADDR)
>>>>>>> refs/remotes/origin/cm-10.0

$(obj)/simpleImage.%: vmlinux FORCE
	$(call if_changed,cp,.unstrip)
	$(call if_changed,objcopy)
	$(call if_changed,uimage)
	$(call if_changed,strip)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'


# Rule to build device tree blobs
DTC_FLAGS := -p 1024

$(obj)/%.dtb: $(src)/dts/%.dts FORCE
<<<<<<< HEAD
	$(call cmd,dtc)
=======
	$(call if_changed_dep,dtc)
>>>>>>> refs/remotes/origin/cm-10.0

clean-files += *.dtb simpleImage.*.unstrip linux.bin.ub
