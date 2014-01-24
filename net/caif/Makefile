ccflags-$(CONFIG_CAIF_DEBUG)     :=      -DDEBUG

caif-y := caif_dev.o \
	cfcnfg.o cfmuxl.o cfctrl.o  \
	cffrml.o cfveil.o cfdbgl.o\
	cfserl.o cfdgml.o  \
	cfrfml.o cfvidl.o cfutill.o \
	cfsrvl.o cfpkt_skbuff.o

obj-$(CONFIG_CAIF) += caif.o
obj-$(CONFIG_CAIF_NETDEV) += chnl_net.o
obj-$(CONFIG_CAIF) += caif_socket.o
<<<<<<< HEAD
=======
obj-$(CONFIG_CAIF_USB) += caif_usb.o
>>>>>>> refs/remotes/origin/cm-10.0

export-y := caif.o
