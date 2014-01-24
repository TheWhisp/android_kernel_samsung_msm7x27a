#ifndef _ASM_M68K_MODULE_H
#define _ASM_M68K_MODULE_H

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_MMU
=======
=======
#include <asm-generic/module.h>

>>>>>>> refs/remotes/origin/master
enum m68k_fixup_type {
	m68k_fixup_memoffset,
	m68k_fixup_vnode_shift,
};

struct m68k_fixup_info {
	enum m68k_fixup_type type;
	void *addr;
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

struct mod_arch_specific {
	struct m68k_fixup_info *fixup_start, *fixup_end;
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifdef CONFIG_MMU

>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_MMU

>>>>>>> refs/remotes/origin/master
#define MODULE_ARCH_INIT {				\
	.fixup_start		= __start_fixup,	\
	.fixup_end		= __stop_fixup,		\
}


<<<<<<< HEAD
<<<<<<< HEAD
enum m68k_fixup_type {
	m68k_fixup_memoffset,
	m68k_fixup_vnode_shift,
};

struct m68k_fixup_info {
	enum m68k_fixup_type type;
	void *addr;
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define m68k_fixup(type, addr)			\
	"	.section \".m68k_fixup\",\"aw\"\n"	\
	"	.long " #type "," #addr "\n"	\
	"	.previous\n"

<<<<<<< HEAD
<<<<<<< HEAD
=======
#endif /* CONFIG_MMU */

>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif /* CONFIG_MMU */

>>>>>>> refs/remotes/origin/master
extern struct m68k_fixup_info __start_fixup[], __stop_fixup[];

struct module;
extern void module_fixup(struct module *mod, struct m68k_fixup_info *start,
			 struct m68k_fixup_info *end);

<<<<<<< HEAD
<<<<<<< HEAD
#else

struct mod_arch_specific {
};

#endif /* CONFIG_MMU */

=======
>>>>>>> refs/remotes/origin/cm-10.0
#define Elf_Shdr Elf32_Shdr
#define Elf_Sym Elf32_Sym
#define Elf_Ehdr Elf32_Ehdr

=======
>>>>>>> refs/remotes/origin/master
#endif /* _ASM_M68K_MODULE_H */
