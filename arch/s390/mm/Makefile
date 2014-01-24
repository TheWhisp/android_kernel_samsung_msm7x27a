#
# Makefile for the linux s390-specific parts of the memory manager.
#

<<<<<<< HEAD
obj-y	 := init.o fault.o extmem.o mmap.o vmem.o pgtable.o maccess.o \
	    page-states.o gup.o
obj-$(CONFIG_CMM) += cmm.o
obj-$(CONFIG_HUGETLB_PAGE) += hugetlbpage.o
obj-$(CONFIG_DEBUG_SET_MODULE_RONX) += pageattr.o
=======
obj-y		:= init.o fault.o extmem.o mmap.o vmem.o pgtable.o maccess.o
obj-y		+= page-states.o gup.o extable.o pageattr.o mem_detect.o

obj-$(CONFIG_CMM)		+= cmm.o
obj-$(CONFIG_HUGETLB_PAGE)	+= hugetlbpage.o
obj-$(CONFIG_S390_PTDUMP)	+= dump_pagetables.o
>>>>>>> refs/remotes/origin/master
