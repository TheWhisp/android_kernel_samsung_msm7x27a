# Makefile for kernel virtual machines on s390
#
# Copyright IBM Corp. 2008
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License (version 2 only)
# as published by the Free Software Foundation.

common-objs = $(addprefix ../../../virt/kvm/, kvm_main.o)

ccflags-y := -Ivirt/kvm -Iarch/s390/kvm

<<<<<<< HEAD
kvm-objs := $(common-objs) kvm-s390.o sie64a.o intercept.o interrupt.o priv.o sigp.o diag.o
=======
kvm-objs := $(common-objs) kvm-s390.o intercept.o interrupt.o priv.o sigp.o diag.o
>>>>>>> refs/remotes/origin/cm-10.0
obj-$(CONFIG_KVM) += kvm.o
