#
# Makefile for the kernel security code
#

obj-$(CONFIG_KEYS)			+= keys/
subdir-$(CONFIG_SECURITY_SELINUX)	+= selinux
subdir-$(CONFIG_SECURITY_SMACK)		+= smack
subdir-$(CONFIG_SECURITY_TOMOYO)        += tomoyo
subdir-$(CONFIG_SECURITY_APPARMOR)	+= apparmor
<<<<<<< HEAD
<<<<<<< HEAD
=======
subdir-$(CONFIG_SECURITY_YAMA)		+= yama
>>>>>>> refs/remotes/origin/cm-10.0
=======
subdir-$(CONFIG_SECURITY_YAMA)		+= yama
>>>>>>> refs/remotes/origin/master

# always enable default capabilities
obj-y					+= commoncap.o
obj-$(CONFIG_MMU)			+= min_addr.o

# Object file lists
obj-$(CONFIG_SECURITY)			+= security.o capability.o
obj-$(CONFIG_SECURITYFS)		+= inode.o
<<<<<<< HEAD
# Must precede capability.o in order to stack properly.
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_SECURITY_SELINUX)		+= selinux/built-in.o
obj-$(CONFIG_SECURITY_SMACK)		+= smack/built-in.o
obj-$(CONFIG_AUDIT)			+= lsm_audit.o
obj-$(CONFIG_SECURITY_TOMOYO)		+= tomoyo/built-in.o
obj-$(CONFIG_SECURITY_APPARMOR)		+= apparmor/built-in.o
<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_CGROUP_DEVICE)		+= device_cgroup.o

# Object integrity file lists
subdir-$(CONFIG_IMA)			+= integrity/ima
obj-$(CONFIG_IMA)			+= integrity/ima/built-in.o
=======
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_SECURITY_YAMA)		+= yama/built-in.o
obj-$(CONFIG_CGROUP_DEVICE)		+= device_cgroup.o

# Object integrity file lists
subdir-$(CONFIG_INTEGRITY)		+= integrity
obj-$(CONFIG_INTEGRITY)			+= integrity/built-in.o
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
