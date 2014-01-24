#
# Makefile for the kernel DVB device drivers.
#

<<<<<<< HEAD
dvb-core-objs := dvbdev.o dmxdev.o dvb_demux.o dvb_filter.o 	\
		 dvb_ca_en50221.o dvb_frontend.o 		\
		 dvb_net.o dvb_ringbuffer.o dvb_math.o
=======
dvb-net-$(CONFIG_DVB_NET) := dvb_net.o

dvb-core-objs := dvbdev.o dmxdev.o dvb_demux.o dvb_filter.o 	\
		 dvb_ca_en50221.o dvb_frontend.o 		\
		 $(dvb-net-y) dvb_ringbuffer.o dvb_math.o
>>>>>>> refs/remotes/origin/cm-10.0

obj-$(CONFIG_DVB_CORE) += dvb-core.o
