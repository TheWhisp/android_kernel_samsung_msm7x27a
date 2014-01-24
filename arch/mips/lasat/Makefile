#
# Makefile for the LASAT specific kernel interface routines under Linux.
#

<<<<<<< HEAD
obj-y	 			+= reset.o setup.o prom.o lasat_board.o \
=======
obj-y				+= reset.o setup.o prom.o lasat_board.o \
>>>>>>> refs/remotes/origin/master
				   at93c.o interrupt.o serial.o

obj-$(CONFIG_LASAT_SYSCTL)	+= sysctl.o
obj-$(CONFIG_DS1603)		+= ds1603.o
obj-$(CONFIG_PICVUE)		+= picvue.o
obj-$(CONFIG_PICVUE_PROC)	+= picvue_proc.o

clean:
	make -C image clean
