<<<<<<< HEAD
#include <variant/core.h>
=======
/*
 *  linux/arch/xtensa/boot/boot-elf/boot.lds.S
 *
 *  Copyright (C) 2008 - 2013 by Tensilica Inc.
 *
 *  Chris Zankel <chris@zankel.net>
 *  Marc Gauthier <marc@tensilica.com
 *  Pete Delaney <piet@tensilica.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <asm/vectors.h>
>>>>>>> refs/remotes/origin/master
OUTPUT_ARCH(xtensa)
ENTRY(_ResetVector)

SECTIONS
{
<<<<<<< HEAD
	.start 0xD0000000 : { *(.start) }

	.text 0xD0000000:
	{
		__reloc_start = . ;
		_text_start = . ;
		*(.literal .text.literal .text)
		_text_end = . ;
	}

	.rodata ALIGN(0x04):
	{
		*(.rodata)
		*(.rodata1)
	}

	.data ALIGN(0x04):
	{
		*(.data)
		*(.data1)
		*(.sdata)
		*(.sdata2)
		*(.got.plt)
		*(.got)
		*(.dynamic)
	}

	__reloc_end = . ;

	.initrd ALIGN(0x10) :
	{
		boot_initrd_start = . ;
		*(.initrd)
		boot_initrd_end = .;
	}

	. = ALIGN(0x10);
	__image_load = . ;
	.image 0xd0001000:
=======
	.ResetVector.text XCHAL_RESET_VECTOR_VADDR :
	{
		*(.ResetVector.text)
	}

	.image KERNELOFFSET: AT (LOAD_MEMORY_ADDRESS)
>>>>>>> refs/remotes/origin/master
	{
		_image_start = .;
		*(image)
		. = (. + 3) & ~ 3;
		_image_end = .	;
	}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	.bss ((LOADADDR(.image) + SIZEOF(.image) + 3) & ~ 3):
	{
		__bss_start = .;
		*(.sbss)
		*(.scommon)
		*(.dynbss)
		*(.bss)
		__bss_end = .;
	}
<<<<<<< HEAD
	_end = .;
	_param_start = .;

	.ResetVector.text XCHAL_RESET_VECTOR_VADDR :
	{
		*(.ResetVector.text)
	}


	PROVIDE (end = .);
=======

	/*
	 * This is a remapped copy of the Reset Vector Code.
	 * It keeps gdb in sync with the PC after switching
	 * to the temporary mapping used while setting up
	 * the V2 MMU mappings for Linux.
	 */
	.ResetVector.remapped_text 0x46000000 (INFO):
	{
		*(.ResetVector.remapped_text)
	}
>>>>>>> refs/remotes/origin/master
}
