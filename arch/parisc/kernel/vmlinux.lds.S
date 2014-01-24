/*    Kernel link layout for various "sections"
 *
 *    Copyright (C) 1999-2003 Matthew Wilcox <willy at parisc-linux.org>
 *    Copyright (C) 2000-2003 Paul Bame <bame at parisc-linux.org>
 *    Copyright (C) 2000 John Marvin <jsm at parisc-linux.org>
 *    Copyright (C) 2000 Michael Ang <mang with subcarrier.org>
 *    Copyright (C) 2002 Randolph Chung <tausq with parisc-linux.org>
 *    Copyright (C) 2003 James Bottomley <jejb with parisc-linux.org>
<<<<<<< HEAD
 *    Copyright (C) 2006 Helge Deller <deller@gmx.de>
 *
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <asm-generic/vmlinux.lds.h>
=======
 *    Copyright (C) 2006-2013 Helge Deller <deller@gmx.de>
 */

/*
 * Put page table entries (swapper_pg_dir) as the first thing in .bss. This
 * will ensure that it has .bss alignment (PAGE_SIZE).
 */
#define BSS_FIRST_SECTIONS	*(.data..vm0.pmd) \
				*(.data..vm0.pgd) \
				*(.data..vm0.pte)

#include <asm-generic/vmlinux.lds.h>

>>>>>>> refs/remotes/origin/master
/* needed for the processor specific cache alignment size */	
#include <asm/cache.h>
#include <asm/page.h>
#include <asm/asm-offsets.h>
#include <asm/thread_info.h>
	
/* ld script to make hppa Linux kernel */
#ifndef CONFIG_64BIT
OUTPUT_FORMAT("elf32-hppa-linux")
OUTPUT_ARCH(hppa)
#else
OUTPUT_FORMAT("elf64-hppa-linux")
OUTPUT_ARCH(hppa:hppa2.0w)
#endif

<<<<<<< HEAD
ENTRY(_stext)
=======
ENTRY(parisc_kernel_start)
>>>>>>> refs/remotes/origin/master
#ifndef CONFIG_64BIT
jiffies = jiffies_64 + 4;
#else
jiffies = jiffies_64;
#endif
SECTIONS
{
	. = KERNEL_BINARY_TEXT_START;

<<<<<<< HEAD
	_text = .;		/* Text and read-only data */
	.head ALIGN(16) : {
		HEAD_TEXT
	} = 0
	.text ALIGN(16) : {
<<<<<<< HEAD
=======
	__init_begin = .;
	HEAD_TEXT_SECTION
	INIT_TEXT_SECTION(8)

	. = ALIGN(PAGE_SIZE);
	INIT_DATA_SECTION(PAGE_SIZE)
	/* we have to discard exit text and such at runtime, not link time */
	.exit.text :
	{
		EXIT_TEXT
	}
	.exit.data :
	{
		EXIT_DATA
	}
	PERCPU_SECTION(8)
	. = ALIGN(PAGE_SIZE);
	__init_end = .;
	/* freed after init ends here */

	_text = .;		/* Text and read-only data */
	_stext = .;
	.text ALIGN(PAGE_SIZE) : {
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		TEXT_TEXT
		SCHED_TEXT
		LOCK_TEXT
		KPROBES_TEXT
		IRQENTRY_TEXT
		*(.text.do_softirq)
		*(.text.sys_exit)
		*(.text.do_sigaltstack)
		*(.text.do_fork)
		*(.text.*)
		*(.fixup)
		*(.lock.text)		/* out-of-line lock text */
		*(.gnu.warning)
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* End of text section */
	_etext = .;
=======
	. = ALIGN(PAGE_SIZE);
	_etext = .;
	/* End of text section */
>>>>>>> refs/remotes/origin/master

	/* Start of data section */
	_sdata = .;

<<<<<<< HEAD
	RODATA

	/* writeable */
	/* Make sure this is page aligned so
	 * that we can properly leave these
	 * as writable
	 */
	. = ALIGN(PAGE_SIZE);
	data_start = .;
=======
	RO_DATA_SECTION(8)

#ifdef CONFIG_64BIT
	. = ALIGN(16);
	/* Linkage tables */
	.opd : {
		*(.opd)
	} PROVIDE (__gp = .);
	.plt : {
		*(.plt)
	}
	.dlt : {
		*(.dlt)
	}
#endif
>>>>>>> refs/remotes/origin/master

	/* unwind info */
	.PARISC.unwind : {
		__start___unwind = .;
		*(.PARISC.unwind)
		__stop___unwind = .;
	}

<<<<<<< HEAD
	EXCEPTION_TABLE(16)
	NOTES

	/* Data */
	RW_DATA_SECTION(L1_CACHE_BYTES, PAGE_SIZE, THREAD_SIZE)
=======
	/* writeable */
	/* Make sure this is page aligned so
	 * that we can properly leave these
	 * as writable
	 */
	. = ALIGN(PAGE_SIZE);
	data_start = .;

	EXCEPTION_TABLE(8)
	NOTES

	/* Data */
	RW_DATA_SECTION(L1_CACHE_BYTES, PAGE_SIZE, PAGE_SIZE)
>>>>>>> refs/remotes/origin/master

	/* PA-RISC locks requires 16-byte alignment */
	. = ALIGN(16);
	.data..lock_aligned : {
		*(.data..lock_aligned)
	}

	/* End of data section */
	_edata = .;

	/* BSS */
<<<<<<< HEAD
	__bss_start = .;
	/* page table entries need to be PAGE_SIZE aligned */
	. = ALIGN(PAGE_SIZE);
	.data..vmpages : {
		*(.data..vm0.pmd)
		*(.data..vm0.pgd)
		*(.data..vm0.pte)
	}
	.bss : {
		*(.bss)
		*(COMMON)
	}
	__bss_stop = .;

#ifdef CONFIG_64BIT
	. = ALIGN(16);
	/* Linkage tables */
	.opd : {
		*(.opd)
	} PROVIDE (__gp = .); 
	.plt : {
		*(.plt)
	} 
	.dlt : {
		*(.dlt)
	}
#endif

	/* reserve space for interrupt stack by aligning __init* to 16k */
	. = ALIGN(16384);
	__init_begin = .;
	INIT_TEXT_SECTION(16384)
	. = ALIGN(PAGE_SIZE);
	INIT_DATA_SECTION(16)
	/* we have to discard exit text and such at runtime, not link time */
	.exit.text :
	{
		EXIT_TEXT
	}
	.exit.data :
	{
		EXIT_DATA
	}

	PERCPU_SECTION(L1_CACHE_BYTES)
	. = ALIGN(PAGE_SIZE);
	__init_end = .;
	/* freed after init ends here */
=======
	BSS_SECTION(PAGE_SIZE, PAGE_SIZE, 8)

>>>>>>> refs/remotes/origin/master
	_end = . ;

	STABS_DEBUG
	.note 0 : { *(.note) }

	/* Sections to be discarded */
	DISCARDS
	/DISCARD/ : {
#ifdef CONFIG_64BIT
		/* temporary hack until binutils is fixed to not emit these
	 	 * for static binaries
		 */
		*(.interp)
		*(.dynsym)
		*(.dynstr)
		*(.dynamic)
		*(.hash)
		*(.gnu.hash)
#endif
	}
}
