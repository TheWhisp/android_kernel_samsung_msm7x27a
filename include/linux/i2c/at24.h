<<<<<<< HEAD
=======
/*
 * at24.h - platform_data for the at24 (generic eeprom) driver
 * (C) Copyright 2008 by Pengutronix
 * (C) Copyright 2012 by Wolfram Sang
 * same license as the driver
 */

>>>>>>> refs/remotes/origin/cm-10.0
#ifndef _LINUX_AT24_H
#define _LINUX_AT24_H

#include <linux/types.h>
#include <linux/memory.h>

<<<<<<< HEAD
/*
 * As seen through Linux I2C, differences between the most common types of I2C
 * memory include:
 * - How much memory is available (usually specified in bit)?
 * - What write page size does it support?
 * - Special flags (16 bit addresses, read_only, world readable...)?
=======
/**
 * struct at24_platform_data - data to set up at24 (generic eeprom) driver
 * @byte_len: size of eeprom in byte
 * @page_size: number of byte which can be written in one go
 * @flags: tunable options, check AT24_FLAG_* defines
 * @setup: an optional callback invoked after eeprom is probed; enables kernel
	code to access eeprom via memory_accessor, see example
 * @context: optional parameter passed to setup()
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * If you set up a custom eeprom type, please double-check the parameters.
 * Especially page_size needs extra care, as you risk data loss if your value
 * is bigger than what the chip actually supports!
<<<<<<< HEAD
=======
 *
 * An example in pseudo code for a setup() callback:
 *
 * void get_mac_addr(struct memory_accessor *mem_acc, void *context)
 * {
 * 	u8 *mac_addr = ethernet_pdata->mac_addr;
 *	off_t offset = context;
 *
 *	// Read MAC addr from EEPROM
 *	if (mem_acc->read(mem_acc, mac_addr, offset, ETH_ALEN) == ETH_ALEN)
 *		pr_info("Read MAC addr from EEPROM: %pM\n", mac_addr);
 * }
 *
 * This function pointer and context can now be set up in at24_platform_data.
>>>>>>> refs/remotes/origin/cm-10.0
 */

struct at24_platform_data {
	u32		byte_len;		/* size (sum of all addr) */
	u16		page_size;		/* for writes */
	u8		flags;
#define AT24_FLAG_ADDR16	0x80	/* address pointer is 16 bit */
#define AT24_FLAG_READONLY	0x40	/* sysfs-entry will be read-only */
#define AT24_FLAG_IRUGO		0x20	/* sysfs-entry will be world-readable */
#define AT24_FLAG_TAKE8ADDR	0x10	/* take always 8 addresses (24c00) */

	void		(*setup)(struct memory_accessor *, void *context);
	void		*context;
};

#endif /* _LINUX_AT24_H */
