obj-$(CONFIG_OMAP2_DSS) += omapdss.o
<<<<<<< HEAD
<<<<<<< HEAD
omapdss-y := core.o dss.o dss_features.o dispc.o display.o manager.o overlay.o
=======
omapdss-y := core.o dss.o dss_features.o dispc.o dispc_coefs.o display.o \
	manager.o overlay.o apply.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
# Core DSS files
omapdss-y := core.o dss.o dss_features.o dispc.o dispc_coefs.o display.o \
	output.o
# DSS compat layer files
omapdss-y += manager.o manager-sysfs.o overlay.o overlay-sysfs.o apply.o \
	dispc-compat.o display-sysfs.o
>>>>>>> refs/remotes/origin/master
omapdss-$(CONFIG_OMAP2_DSS_DPI) += dpi.o
omapdss-$(CONFIG_OMAP2_DSS_RFBI) += rfbi.o
omapdss-$(CONFIG_OMAP2_DSS_VENC) += venc.o
omapdss-$(CONFIG_OMAP2_DSS_SDI) += sdi.o
omapdss-$(CONFIG_OMAP2_DSS_DSI) += dsi.o
<<<<<<< HEAD
omapdss-$(CONFIG_OMAP4_DSS_HDMI) += hdmi.o \
<<<<<<< HEAD
				    hdmi_omap4_panel.o
=======
				    hdmi_panel.o ti_hdmi_4xxx_ip.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
omapdss-$(CONFIG_OMAP4_DSS_HDMI) += hdmi4.o hdmi_common.o hdmi_wp.o hdmi_pll.o \
	hdmi_phy.o hdmi4_core.o
ccflags-$(CONFIG_OMAP2_DSS_DEBUG) += -DDEBUG
>>>>>>> refs/remotes/origin/master
