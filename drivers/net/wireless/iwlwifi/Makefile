<<<<<<< HEAD
# AGN
obj-$(CONFIG_IWLAGN)	+= iwlagn.o
iwlagn-objs		:= iwl-agn.o iwl-agn-rs.o
iwlagn-objs		+= iwl-agn-ucode.o iwl-agn-tx.o
iwlagn-objs		+= iwl-agn-lib.o iwl-agn-calib.o iwl-io.o
iwlagn-objs		+= iwl-agn-tt.o iwl-agn-sta.o iwl-agn-eeprom.o

iwlagn-objs 		+= iwl-core.o iwl-eeprom.o iwl-hcmd.o iwl-power.o
iwlagn-objs 		+= iwl-rx.o iwl-tx.o iwl-sta.o
iwlagn-objs 		+= iwl-scan.o iwl-led.o
iwlagn-objs             += iwl-agn-rxon.o iwl-agn-hcmd.o iwl-agn-ict.o
iwlagn-objs             += iwl-5000.o
iwlagn-objs             += iwl-6000.o
iwlagn-objs             += iwl-1000.o
iwlagn-objs             += iwl-2000.o

iwlagn-$(CONFIG_IWLWIFI_DEBUGFS) += iwl-debugfs.o
iwlagn-$(CONFIG_IWLWIFI_DEVICE_TRACING) += iwl-devtrace.o
iwlagn-$(CONFIG_IWLWIFI_DEVICE_SVTOOL) += iwl-sv-open.o
=======
# WIFI
obj-$(CONFIG_IWLWIFI)	+= iwlwifi.o
iwlwifi-objs		:= iwl-agn.o iwl-agn-rs.o iwl-mac80211.o
iwlwifi-objs		+= iwl-ucode.o iwl-agn-tx.o iwl-debug.o
iwlwifi-objs		+= iwl-agn-lib.o iwl-agn-calib.o iwl-io.o
iwlwifi-objs		+= iwl-agn-tt.o iwl-agn-sta.o iwl-agn-rx.o

iwlwifi-objs		+= iwl-core.o iwl-eeprom.o iwl-power.o
iwlwifi-objs		+= iwl-scan.o iwl-led.o
iwlwifi-objs		+= iwl-agn-rxon.o
iwlwifi-objs		+= iwl-5000.o
iwlwifi-objs		+= iwl-6000.o
iwlwifi-objs		+= iwl-1000.o
iwlwifi-objs		+= iwl-2000.o
iwlwifi-objs		+= iwl-pci.o
iwlwifi-objs		+= iwl-drv.o
iwlwifi-objs		+= iwl-notif-wait.o
iwlwifi-objs		+= iwl-trans-pcie.o iwl-trans-pcie-rx.o iwl-trans-pcie-tx.o

iwlwifi-$(CONFIG_IWLWIFI_DEBUGFS) += iwl-debugfs.o
iwlwifi-$(CONFIG_IWLWIFI_DEVICE_TRACING) += iwl-devtrace.o
iwlwifi-$(CONFIG_IWLWIFI_DEVICE_TESTMODE) += iwl-testmode.o
>>>>>>> refs/remotes/origin/cm-10.0

CFLAGS_iwl-devtrace.o := -I$(src)

ccflags-y += -D__CHECK_ENDIAN__
