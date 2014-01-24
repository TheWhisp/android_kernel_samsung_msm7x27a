#
# Makefile for the Linux/M32R kernel.
#

<<<<<<< HEAD
extra-y	:= head.o init_task.o vmlinux.lds
=======
extra-y	:= head.o vmlinux.lds
>>>>>>> refs/remotes/origin/master

obj-y	:= process.o entry.o traps.o align.o irq.o setup.o time.o \
	m32r_ksyms.o sys_m32r.o signal.o ptrace.o

obj-$(CONFIG_SMP)		+= smp.o smpboot.o
obj-$(CONFIG_MODULES)		+= module.o
