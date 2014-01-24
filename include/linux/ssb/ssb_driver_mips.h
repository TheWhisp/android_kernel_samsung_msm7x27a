#ifndef LINUX_SSB_MIPSCORE_H_
#define LINUX_SSB_MIPSCORE_H_

#ifdef CONFIG_SSB_DRIVER_MIPS

struct ssb_device;

struct ssb_serial_port {
	void *regs;
	unsigned long clockspeed;
	unsigned int irq;
	unsigned int baud_base;
	unsigned int reg_shift;
};

<<<<<<< HEAD
=======
struct ssb_pflash {
	bool present;
	u8 buswidth;
	u32 window;
	u32 window_size;
};

#ifdef CONFIG_SSB_SFLASH
struct ssb_sflash {
	bool present;
	u32 window;
	u32 blocksize;
	u16 numblocks;
	u32 size;

	void *priv;
};
#endif
>>>>>>> refs/remotes/origin/master

struct ssb_mipscore {
	struct ssb_device *dev;

	int nr_serial_ports;
	struct ssb_serial_port serial_ports[4];

<<<<<<< HEAD
	u8 flash_buswidth;
	u32 flash_window;
	u32 flash_window_size;
=======
	struct ssb_pflash pflash;
#ifdef CONFIG_SSB_SFLASH
	struct ssb_sflash sflash;
#endif
>>>>>>> refs/remotes/origin/master
};

extern void ssb_mipscore_init(struct ssb_mipscore *mcore);
extern u32 ssb_cpu_clock(struct ssb_mipscore *mcore);

extern unsigned int ssb_mips_irq(struct ssb_device *dev);


#else /* CONFIG_SSB_DRIVER_MIPS */

struct ssb_mipscore {
};

static inline
void ssb_mipscore_init(struct ssb_mipscore *mcore)
{
}

<<<<<<< HEAD
=======
static inline unsigned int ssb_mips_irq(struct ssb_device *dev)
{
	return 0;
}

>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_SSB_DRIVER_MIPS */

#endif /* LINUX_SSB_MIPSCORE_H_ */
