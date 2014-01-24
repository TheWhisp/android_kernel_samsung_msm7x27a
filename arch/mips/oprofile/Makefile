<<<<<<< HEAD
ccflags-y := -Werror

=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_OPROFILE) += oprofile.o

DRIVER_OBJS = $(addprefix ../../../drivers/oprofile/, \
		oprof.o cpu_buffer.o buffer_sync.o \
		event_buffer.o oprofile_files.o \
		oprofilefs.o oprofile_stats.o \
		timer_int.o )

<<<<<<< HEAD
<<<<<<< HEAD
oprofile-y				:= $(DRIVER_OBJS) common.o
=======
oprofile-y				:= $(DRIVER_OBJS) common.o backtrace.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
oprofile-y				:= $(DRIVER_OBJS) common.o backtrace.o
>>>>>>> refs/remotes/origin/master

oprofile-$(CONFIG_CPU_MIPS32)		+= op_model_mipsxx.o
oprofile-$(CONFIG_CPU_MIPS64)		+= op_model_mipsxx.o
oprofile-$(CONFIG_CPU_R10000)		+= op_model_mipsxx.o
oprofile-$(CONFIG_CPU_SB1)		+= op_model_mipsxx.o
<<<<<<< HEAD
oprofile-$(CONFIG_CPU_RM9000)		+= op_model_rm9000.o
=======
oprofile-$(CONFIG_CPU_XLR)		+= op_model_mipsxx.o
>>>>>>> refs/remotes/origin/master
oprofile-$(CONFIG_CPU_LOONGSON2)	+= op_model_loongson2.o
