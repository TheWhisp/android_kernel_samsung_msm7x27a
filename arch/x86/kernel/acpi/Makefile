<<<<<<< HEAD
subdir-				:= realmode

obj-$(CONFIG_ACPI)		+= boot.o
obj-$(CONFIG_ACPI_SLEEP)	+= sleep.o wakeup_rm.o wakeup_$(BITS).o
=======
obj-$(CONFIG_ACPI)		+= boot.o
obj-$(CONFIG_ACPI_SLEEP)	+= sleep.o wakeup_$(BITS).o
>>>>>>> refs/remotes/origin/master

ifneq ($(CONFIG_ACPI_PROCESSOR),)
obj-y				+= cstate.o
endif

<<<<<<< HEAD
$(obj)/wakeup_rm.o:    $(obj)/realmode/wakeup.bin

$(obj)/realmode/wakeup.bin: FORCE
	$(Q)$(MAKE) $(build)=$(obj)/realmode

=======
>>>>>>> refs/remotes/origin/master
