#include <linux/elf.h>
#include <linux/fs.h>
#include <linux/mm.h>
<<<<<<< HEAD

#include <asm/elf.h>

=======
#include <linux/binfmts.h>
>>>>>>> refs/remotes/origin/master

Elf_Half __weak elf_core_extra_phdrs(void)
{
	return 0;
}

<<<<<<< HEAD
int __weak elf_core_write_extra_phdrs(struct file *file, loff_t offset, size_t *size,
				      unsigned long limit)
=======
int __weak elf_core_write_extra_phdrs(struct coredump_params *cprm, loff_t offset)
>>>>>>> refs/remotes/origin/master
{
	return 1;
}

<<<<<<< HEAD
int __weak elf_core_write_extra_data(struct file *file, size_t *size,
				     unsigned long limit)
=======
int __weak elf_core_write_extra_data(struct coredump_params *cprm)
>>>>>>> refs/remotes/origin/master
{
	return 1;
}

size_t __weak elf_core_extra_data_size(void)
{
	return 0;
}
