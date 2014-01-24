#
# Makefile for the Linux/SCORE kernel.
#

extra-y	:= head.o vmlinux.lds

<<<<<<< HEAD
obj-y += entry.o init_task.o irq.o process.o ptrace.o \
=======
obj-y += entry.o irq.o process.o ptrace.o \
>>>>>>> refs/remotes/origin/master
	setup.o signal.o sys_score.o time.o traps.o \
	sys_call_table.o

obj-$(CONFIG_MODULES) += module.o
