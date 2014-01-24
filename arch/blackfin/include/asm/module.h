/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright 2004-2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */
=======
=======
>>>>>>> refs/remotes/origin/master
 * Copyright 2004-2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifndef _ASM_BFIN_MODULE_H
#define _ASM_BFIN_MODULE_H

<<<<<<< HEAD
#define Elf_Shdr        Elf32_Shdr
#define Elf_Sym         Elf32_Sym
#define Elf_Ehdr        Elf32_Ehdr
=======
#include <asm-generic/module.h>
>>>>>>> refs/remotes/origin/master

struct mod_arch_specific {
	Elf_Shdr	*text_l1;
	Elf_Shdr	*data_a_l1;
	Elf_Shdr	*bss_a_l1;
	Elf_Shdr	*data_b_l1;
	Elf_Shdr	*bss_b_l1;
	Elf_Shdr	*text_l2;
	Elf_Shdr	*data_l2;
	Elf_Shdr	*bss_l2;
};
#endif				/* _ASM_BFIN_MODULE_H */
