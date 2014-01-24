ifdef CONFIG_FUNCTION_TRACER
CFLAGS_REMOVE_core.o = -pg
endif

<<<<<<< HEAD
<<<<<<< HEAD
obj-y := core.o
=======
obj-y := core.o ring_buffer.o callchain.o
>>>>>>> refs/remotes/origin/cm-10.0
obj-$(CONFIG_HAVE_HW_BREAKPOINT) += hw_breakpoint.o
=======
obj-y := core.o ring_buffer.o callchain.o

obj-$(CONFIG_HAVE_HW_BREAKPOINT) += hw_breakpoint.o
obj-$(CONFIG_UPROBES) += uprobes.o

>>>>>>> refs/remotes/origin/master
