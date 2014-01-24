
#
# Makefile for m68k-specific library files..
#

lib-y	:= ashldi3.o ashrdi3.o lshrdi3.o muldi3.o \
	   memcpy.o memset.o memmove.o

<<<<<<< HEAD
<<<<<<< HEAD
ifdef CONFIG_MMU
lib-y	+= string.o uaccess.o checksum_mm.o
else
lib-y	+= mulsi3.o divsi3.o udivsi3.o modsi3.o umodsi3.o delay.o checksum_no.o
=======
lib-$(CONFIG_MMU) += string.o uaccess.o
=======
lib-$(CONFIG_MMU) += uaccess.o
>>>>>>> refs/remotes/origin/master
lib-$(CONFIG_CPU_HAS_NO_MULDIV64) += mulsi3.o divsi3.o udivsi3.o
lib-$(CONFIG_CPU_HAS_NO_MULDIV64) += modsi3.o umodsi3.o

ifndef CONFIG_GENERIC_CSUM
lib-y	+= checksum.o
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
endif

