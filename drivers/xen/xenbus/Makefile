obj-y	+= xenbus.o
<<<<<<< HEAD
<<<<<<< HEAD
=======
obj-y	+= xenbus_dev_frontend.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-y	+= xenbus_dev_frontend.o
>>>>>>> refs/remotes/origin/master

xenbus-objs =
xenbus-objs += xenbus_client.o
xenbus-objs += xenbus_comms.o
xenbus-objs += xenbus_xs.o
xenbus-objs += xenbus_probe.o

xenbus-be-objs-$(CONFIG_XEN_BACKEND) += xenbus_probe_backend.o
xenbus-objs += $(xenbus-be-objs-y)

<<<<<<< HEAD
<<<<<<< HEAD
=======
obj-$(CONFIG_XEN_BACKEND) += xenbus_dev_backend.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-$(CONFIG_XEN_BACKEND) += xenbus_dev_backend.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_XEN_XENBUS_FRONTEND) += xenbus_probe_frontend.o
