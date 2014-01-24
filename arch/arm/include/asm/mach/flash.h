/*
 *  arch/arm/include/asm/mach/flash.h
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef ASMARM_MACH_FLASH_H
#define ASMARM_MACH_FLASH_H

struct mtd_partition;
struct mtd_info;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
enum sw_version {
	VERSION_1 = 0,
	VERSION_2,
};

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * map_name:	the map probe function name
 * name:	flash device name (eg, as used with mtdparts=)
 * width:	width of mapped device
<<<<<<< HEAD
<<<<<<< HEAD
 * interleave:  interleave mode feature support
=======
>>>>>>> refs/remotes/origin/master
=======
 * interleave:  interleave mode feature support
>>>>>>> refs/remotes/origin/cm-11.0
 * init:	method called at driver/device initialisation
 * exit:	method called at driver/device removal
 * set_vpp:	method called to enable or disable VPP
 * mmcontrol:	method called to enable or disable Sync. Burst Read in OneNAND
 * parts:	optional array of mtd_partitions for static partitioning
 * nr_parts:	number of mtd_partitions for static partitoning
<<<<<<< HEAD
<<<<<<< HEAD
 * version:	software register interface version
=======
>>>>>>> refs/remotes/origin/master
=======
 * version:	software register interface version
>>>>>>> refs/remotes/origin/cm-11.0
 */
struct flash_platform_data {
	const char	*map_name;
	const char	*name;
	unsigned int	width;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int    interleave;
=======
>>>>>>> refs/remotes/origin/master
=======
	unsigned int    interleave;
>>>>>>> refs/remotes/origin/cm-11.0
	int		(*init)(void);
	void		(*exit)(void);
	void		(*set_vpp)(int on);
	void		(*mmcontrol)(struct mtd_info *mtd, int sync_read);
	struct mtd_partition *parts;
	unsigned int	nr_parts;
<<<<<<< HEAD
<<<<<<< HEAD
	enum sw_version	version;
=======
>>>>>>> refs/remotes/origin/master
=======
	enum sw_version	version;
>>>>>>> refs/remotes/origin/cm-11.0
};

#endif
