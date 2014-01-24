<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_IWLWIFI_LEGACY)	+= iwl-legacy.o
iwl-legacy-objs 		:= iwl-core.o iwl-eeprom.o iwl-hcmd.o iwl-power.o
iwl-legacy-objs 		+= iwl-rx.o iwl-tx.o iwl-sta.o
iwl-legacy-objs 		+= iwl-scan.o iwl-led.o
iwl-legacy-$(CONFIG_IWLWIFI_LEGACY_DEBUGFS) += iwl-debugfs.o
iwl-legacy-$(CONFIG_IWLWIFI_LEGACY_DEVICE_TRACING) += iwl-devtrace.o

iwl-legacy-objs += $(iwl-legacy-m)

CFLAGS_iwl-devtrace.o := -I$(src)

# 4965
obj-$(CONFIG_IWL4965)	+= iwl4965.o
iwl4965-objs		:= iwl-4965.o iwl4965-base.o iwl-4965-rs.o iwl-4965-led.o
iwl4965-objs		+= iwl-4965-ucode.o iwl-4965-tx.o
iwl4965-objs		+= iwl-4965-lib.o iwl-4965-rx.o iwl-4965-calib.o
iwl4965-objs		+= iwl-4965-sta.o iwl-4965-eeprom.o
iwl4965-$(CONFIG_IWLWIFI_LEGACY_DEBUGFS) += iwl-4965-debugfs.o

# 3945
obj-$(CONFIG_IWL3945)	+= iwl3945.o
iwl3945-objs		:= iwl3945-base.o iwl-3945.o iwl-3945-rs.o iwl-3945-led.o
iwl3945-$(CONFIG_IWLWIFI_LEGACY_DEBUGFS) += iwl-3945-debugfs.o
=======
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_IWLEGACY)	+= iwlegacy.o
iwlegacy-objs 		:= common.o
iwlegacy-$(CONFIG_IWLEGACY_DEBUGFS) += debug.o

iwlegacy-objs += $(iwlegacy-m)

# 4965
obj-$(CONFIG_IWL4965)	+= iwl4965.o
iwl4965-objs		:= 4965.o 4965-mac.o 4965-rs.o 4965-calib.o
iwl4965-$(CONFIG_IWLEGACY_DEBUGFS) += 4965-debug.o

# 3945
obj-$(CONFIG_IWL3945)	+= iwl3945.o
iwl3945-objs		:= 3945-mac.o 3945.o 3945-rs.o
iwl3945-$(CONFIG_IWLEGACY_DEBUGFS) += 3945-debug.o
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

ccflags-y += -D__CHECK_ENDIAN__
