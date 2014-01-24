#
# arch/blackfin/kernel/Makefile
#

<<<<<<< HEAD
extra-y := init_task.o vmlinux.lds
=======
extra-y := vmlinux.lds
>>>>>>> refs/remotes/origin/master

obj-y := \
	entry.o process.o bfin_ksyms.o ptrace.o setup.o signal.o \
	sys_bfin.o traps.o irqchip.o dma-mapping.o flat.o \
<<<<<<< HEAD
<<<<<<< HEAD
	fixed_code.o reboot.o bfin_gpio.o bfin_dma_5xx.o \
=======
	fixed_code.o reboot.o bfin_gpio.o bfin_dma.o \
>>>>>>> refs/remotes/origin/cm-10.0
=======
	fixed_code.o reboot.o bfin_dma.o \
>>>>>>> refs/remotes/origin/master
	exception.o dumpstack.o

ifeq ($(CONFIG_GENERIC_CLOCKEVENTS),y)
    obj-y += time-ts.o
else
    obj-y += time.o
endif

<<<<<<< HEAD
=======
obj-$(CONFIG_GPIO_ADI)               += bfin_gpio.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_DYNAMIC_FTRACE)         += ftrace.o
obj-$(CONFIG_FUNCTION_TRACER)        += ftrace-entry.o
obj-$(CONFIG_FUNCTION_GRAPH_TRACER)  += ftrace.o
CFLAGS_REMOVE_ftrace.o = -pg

<<<<<<< HEAD
<<<<<<< HEAD
=======
obj-$(CONFIG_HAVE_PWM)               += pwm.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_IPIPE)                  += ipipe.o
obj-$(CONFIG_BFIN_GPTIMERS)          += gptimers.o
obj-$(CONFIG_CPLB_INFO)              += cplbinfo.o
obj-$(CONFIG_MODULES)                += module.o
obj-$(CONFIG_KGDB)                   += kgdb.o
obj-$(CONFIG_KGDB_TESTS)             += kgdb_test.o
obj-$(CONFIG_NMI_WATCHDOG)           += nmi.o
obj-$(CONFIG_EARLY_PRINTK)           += early_printk.o
obj-$(CONFIG_EARLY_PRINTK)           += shadow_console.o
obj-$(CONFIG_STACKTRACE)             += stacktrace.o
obj-$(CONFIG_DEBUG_VERBOSE)          += trace.o
obj-$(CONFIG_BFIN_PSEUDODBG_INSNS)   += pseudodbg.o
obj-$(CONFIG_PERF_EVENTS)            += perf_event.o

# the kgdb test puts code into L2 and without linker
# relaxation, we need to force long calls to/from it
<<<<<<< HEAD
<<<<<<< HEAD
CFLAGS_kgdb_test.o := -mlong-calls -O0
=======
CFLAGS_kgdb_test.o := -mlong-calls
>>>>>>> refs/remotes/origin/cm-10.0
=======
CFLAGS_kgdb_test.o := -mlong-calls
>>>>>>> refs/remotes/origin/master

obj-$(CONFIG_DEBUG_MMRS)             += debug-mmrs.o
