<<<<<<< HEAD
#ifndef _ASM_X86_IST_H
#define _ASM_X86_IST_H

=======
>>>>>>> refs/remotes/origin/master
/*
 * Include file for the interface to IST BIOS
 * Copyright 2002 Andy Grover <andrew.grover@intel.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */
<<<<<<< HEAD


#include <linux/types.h>

struct ist_info {
	__u32 signature;
	__u32 command;
	__u32 event;
	__u32 perf_level;
};

#ifdef __KERNEL__

extern struct ist_info ist_info;

#endif	/* __KERNEL__ */
=======
#ifndef _ASM_X86_IST_H
#define _ASM_X86_IST_H

#include <uapi/asm/ist.h>


extern struct ist_info ist_info;

>>>>>>> refs/remotes/origin/master
#endif /* _ASM_X86_IST_H */
