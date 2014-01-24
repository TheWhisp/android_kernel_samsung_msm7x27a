#
# Makefile for the Linux/Xtensa kernel.
#

extra-y := head.o vmlinux.lds

<<<<<<< HEAD
obj-y := align.o entry.o irq.o coprocessor.o process.o ptrace.o \
	 setup.o signal.o syscall.o time.o traps.o vectors.o platform.o  \
	 pci-dma.o init_task.o io.o
=======
obj-y := align.o coprocessor.o entry.o irq.o pci-dma.o platform.o process.o \
	 ptrace.o setup.o signal.o stacktrace.o syscall.o time.o traps.o \
	 vectors.o
>>>>>>> refs/remotes/origin/master

obj-$(CONFIG_KGDB) += xtensa-stub.o
obj-$(CONFIG_PCI) += pci.o
obj-$(CONFIG_MODULES) += xtensa_ksyms.o module.o
<<<<<<< HEAD
=======
obj-$(CONFIG_FUNCTION_TRACER) += mcount.o

AFLAGS_head.o += -mtext-section-literals
>>>>>>> refs/remotes/origin/master

# In the Xtensa architecture, assembly generates literals which must always
# precede the L32R instruction with a relative offset less than 256 kB.
# Therefore, the .text and .literal section must be combined in parenthesis
# in the linker script, such as: *(.literal .text).
#
# We need to post-process the generated vmlinux.lds scripts to convert
# *(xxx.text) to  *(xxx.literal xxx.text) for the following text sections:
#   .text .ref.text .*init.text .*exit.text .text.*
#
# Replicate rules in scripts/Makefile.build

<<<<<<< HEAD
sed-y = -e 's/\*(\(\.[a-z]*it\|\.ref\|\)\.text)/*(\1.literal \1.text)/g'    \
<<<<<<< HEAD
=======
	-e 's/\.text\.unlikely/.literal.unlikely .text.unlikely/g' \
>>>>>>> refs/remotes/origin/cm-10.0
	-e 's/\*(\(\.text\.[a-z]*\))/*(\1.literal \1)/g'

quiet_cmd__cpp_lds_S = LDS     $@
      cmd__cpp_lds_S = $(CPP) $(cpp_flags) -P -C -Uxtensa -D__ASSEMBLY__ $< \
                       | sed $(sed-y) >$@
=======
sed-y = -e 's/\*(\(\.[a-z]*it\|\.ref\|\)\.text)/*(\1.literal \1.text)/g' \
	-e 's/\.text\.unlikely/.literal.unlikely .text.unlikely/g'	 \
	-e 's/\*(\(\.text\.[a-z]*\))/*(\1.literal \1)/g'

quiet_cmd__cpp_lds_S = LDS     $@
cmd__cpp_lds_S = $(CPP) $(cpp_flags) -P -C -Uxtensa -D__ASSEMBLY__ $<    \
                 | sed $(sed-y) >$@
>>>>>>> refs/remotes/origin/master

$(obj)/vmlinux.lds: $(src)/vmlinux.lds.S FORCE
	$(call if_changed_dep,_cpp_lds_S)
