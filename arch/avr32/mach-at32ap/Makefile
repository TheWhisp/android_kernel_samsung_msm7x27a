obj-y				+= pdc.o clock.o intc.o extint.o pio.o hsmc.o
obj-y				+= hmatrix.o
obj-$(CONFIG_CPU_AT32AP700X)	+= at32ap700x.o pm-at32ap700x.o
<<<<<<< HEAD
obj-$(CONFIG_CPU_FREQ_AT32AP)	+= cpufreq.o
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_PM)		+= pm.o

ifeq ($(CONFIG_PM_DEBUG),y)
CFLAGS_pm.o	+= -DDEBUG
endif
