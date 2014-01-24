#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

<<<<<<< HEAD
GZIP = gzip
GZIP_FLAGS = -v9fc
=======
>>>>>>> refs/remotes/origin/master
ifeq ($(BIG_ENDIAN),1)
OBJCOPY_ARGS 	:= -O elf32-xtensa-be
else
OBJCOPY_ARGS 	:= -O elf32-xtensa-le
endif

LD_ARGS	= -T $(srctree)/$(obj)/boot.ld

boot-y	:= bootstrap.o

OBJS	:= $(addprefix $(obj)/,$(boot-y))
LIBS	:= arch/xtensa/boot/lib/lib.a arch/xtensa/lib/lib.a

LIBGCC := $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

<<<<<<< HEAD
zImage: vmlinux $(OBJS) $(LIBS)
	$(OBJCOPY) --strip-all -R .comment -R .note.gnu.build-id -O binary \
		vmlinux vmlinux.tmp
	gzip -vf9 vmlinux.tmp
	$(OBJCOPY) $(OBJCOPY_ARGS) -R .comment \
		--add-section image=vmlinux.tmp.gz \
		--set-section-flags image=contents,alloc,load,load,data \
		$(OBJS) $@.tmp
	$(LD) $(LD_ARGS) -o $@.elf $@.tmp $(LIBS) -L/xtensa-elf/lib $(LIBGCC)
	$(OBJCOPY) -S -O binary $@.elf arch/$(ARCH)/boot/$@.redboot
	rm -f $@.tmp $@.elf vmlinux.tmp.gz

=======
$(obj)/zImage.o: vmlinux.bin.gz $(OBJS)
	$(Q)$(OBJCOPY) $(OBJCOPY_ARGS) -R .comment \
		--add-section image=vmlinux.bin.gz \
		--set-section-flags image=contents,alloc,load,load,data \
		$(OBJS) $@

$(obj)/zImage.elf: $(obj)/zImage.o $(LIBS)
	$(Q)$(LD) $(LD_ARGS) -o $@ $^ -L/xtensa-elf/lib $(LIBGCC)

$(obj)/../zImage.redboot: $(obj)/zImage.elf
	$(Q)$(OBJCOPY) -S -O binary $< $@
	$(Q)$(kecho) '  Kernel: $@ is ready'

zImage: $(obj)/../zImage.redboot
>>>>>>> refs/remotes/origin/master
