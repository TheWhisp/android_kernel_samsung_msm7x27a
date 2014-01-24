#ifndef ASMARM_VGA_H
#define ASMARM_VGA_H

#include <linux/io.h>
<<<<<<< HEAD
#include <mach/hardware.h>

#define VGA_MAP_MEM(x,s)	(PCIMEM_BASE + (x))
=======

extern unsigned long vga_base;

#define VGA_MAP_MEM(x,s)	(vga_base + (x))
>>>>>>> refs/remotes/origin/cm-10.0

#define vga_readb(x)	(*((volatile unsigned char *)x))
#define vga_writeb(x,y)	(*((volatile unsigned char *)y) = (x))

#endif
