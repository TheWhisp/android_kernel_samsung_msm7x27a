<<<<<<< HEAD
void __init eseries_fixup(struct machine_desc *desc,
	struct tag *tags, char **cmdline, struct meminfo *mi);
=======
void __init eseries_fixup(struct tag *tags, char **cmdline, struct meminfo *mi);
>>>>>>> refs/remotes/origin/cm-10.0

extern struct pxa2xx_udc_mach_info e7xx_udc_mach_info;
extern struct pxaficp_platform_data e7xx_ficp_platform_data;
extern int e7xx_irda_init(void);

extern int eseries_tmio_enable(struct platform_device *dev);
extern int eseries_tmio_disable(struct platform_device *dev);
extern int eseries_tmio_suspend(struct platform_device *dev);
extern int eseries_tmio_resume(struct platform_device *dev);
extern void eseries_get_tmio_gpios(void);
extern struct resource eseries_tmio_resources[];
extern struct platform_device e300_tc6387xb_device;
<<<<<<< HEAD
extern void eseries_register_clks(void);
=======
>>>>>>> refs/remotes/origin/cm-10.0

