#ifndef _ALPHA_MODULE_H
#define _ALPHA_MODULE_H

<<<<<<< HEAD
=======
#include <asm-generic/module.h>

>>>>>>> refs/remotes/origin/master
struct mod_arch_specific
{
	unsigned int gotsecindex;
};

<<<<<<< HEAD
#define Elf_Sym Elf64_Sym
#define Elf_Shdr Elf64_Shdr
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#define Elf_Dyn Elf64_Dyn
#define Elf_Rel Elf64_Rel
#define Elf_Rela Elf64_Rela

=======
>>>>>>> refs/remotes/origin/master
#define ARCH_SHF_SMALL SHF_ALPHA_GPREL

#ifdef MODULE
asm(".section .got,\"aws\",@progbits; .align 3; .previous");
#endif

#endif /*_ALPHA_MODULE_H*/
