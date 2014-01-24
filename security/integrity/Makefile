#
# Makefile for caching inode integrity data (iint)
#

obj-$(CONFIG_INTEGRITY) += integrity.o
<<<<<<< HEAD
obj-$(CONFIG_INTEGRITY_SIGNATURE) += digsig.o
=======
obj-$(CONFIG_INTEGRITY_AUDIT) += integrity_audit.o
obj-$(CONFIG_INTEGRITY_SIGNATURE) += digsig.o
obj-$(CONFIG_INTEGRITY_ASYMMETRIC_KEYS) += digsig_asymmetric.o
>>>>>>> refs/remotes/origin/master

integrity-y := iint.o

subdir-$(CONFIG_IMA)			+= ima
obj-$(CONFIG_IMA)			+= ima/built-in.o
subdir-$(CONFIG_EVM)			+= evm
obj-$(CONFIG_EVM)			+= evm/built-in.o
