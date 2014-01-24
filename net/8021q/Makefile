#
# Makefile for the Linux VLAN layer.
#
obj-$(subst m,y,$(CONFIG_VLAN_8021Q))	+= vlan_core.o
obj-$(CONFIG_VLAN_8021Q)		+= 8021q.o

8021q-y					:= vlan.o vlan_dev.o vlan_netlink.o
8021q-$(CONFIG_VLAN_8021Q_GVRP)		+= vlan_gvrp.o
<<<<<<< HEAD
=======
8021q-$(CONFIG_VLAN_8021Q_MVRP)		+= vlan_mvrp.o
>>>>>>> refs/remotes/origin/master
8021q-$(CONFIG_PROC_FS)			+= vlanproc.o

