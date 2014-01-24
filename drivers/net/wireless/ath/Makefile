<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_ATH5K)		+= ath5k/
=======
NOSTDINC_FLAGS := -I$(srctree)/include/compat/ \
        -include $(srctree)/include/compat/linux/compat-2.6.h \
        $(CFLAGS)

obj-$(CONFIG_ATH5K)		+= ath5k/
obj-$(CONFIG_ATH6KL)		+= ath6kl/
>>>>>>> refs/remotes/origin/cm-10.0
obj-$(CONFIG_ATH9K_HW)		+= ath9k/
obj-$(CONFIG_CARL9170)		+= carl9170/
=======
obj-$(CONFIG_ATH5K)		+= ath5k/
obj-$(CONFIG_ATH9K_HW)		+= ath9k/
obj-$(CONFIG_CARL9170)		+= carl9170/
obj-$(CONFIG_ATH6KL)		+= ath6kl/
obj-$(CONFIG_AR5523)		+= ar5523/
obj-$(CONFIG_WIL6210)		+= wil6210/
obj-$(CONFIG_ATH10K)		+= ath10k/
obj-$(CONFIG_WCN36XX)		+= wcn36xx/
>>>>>>> refs/remotes/origin/master

obj-$(CONFIG_ATH_COMMON)	+= ath.o

ath-objs :=	main.o \
		regd.o \
		hw.o \
<<<<<<< HEAD
		key.o

<<<<<<< HEAD
ath-$(CONFIG_ATH_DEBUG) += debug.o
=======
ath-$(CONFIG_ATH_DEBUG) += debug.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
		key.o \
		dfs_pattern_detector.o \
		dfs_pri_detector.o

ath-$(CONFIG_ATH_DEBUG) += debug.o
ccflags-y += -D__CHECK_ENDIAN__
>>>>>>> refs/remotes/origin/master
