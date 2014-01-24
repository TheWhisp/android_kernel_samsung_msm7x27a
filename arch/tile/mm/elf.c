/*
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */

#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/binfmts.h>
#include <linux/compat.h>
#include <linux/mman.h>
#include <linux/elf.h>
#include <asm/pgtable.h>
#include <asm/pgalloc.h>
#include <asm/sections.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <arch/sim_def.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/vdso.h>
#include <arch/sim.h>
>>>>>>> refs/remotes/origin/master

/* Notify a running simulator, if any, that an exec just occurred. */
static void sim_notify_exec(const char *binary_name)
{
	unsigned char c;
	do {
		c = *binary_name++;
		__insn_mtspr(SPR_SIM_CONTROL,
			     (SIM_CONTROL_OS_EXEC
			      | (c << _SIM_CONTROL_OPERATOR_BITS)));

	} while (c);
}

<<<<<<< HEAD
static int notify_exec(void)
{
	int retval = 0;  /* failure */
	struct vm_area_struct *vma = current->mm->mmap;
	while (vma) {
		if ((vma->vm_flags & VM_EXECUTABLE) && vma->vm_file)
			break;
		vma = vma->vm_next;
	}
	if (vma) {
		char *buf = (char *) __get_free_page(GFP_KERNEL);
		if (buf) {
			char *path = d_path(&vma->vm_file->f_path,
					    buf, PAGE_SIZE);
			if (!IS_ERR(path)) {
				sim_notify_exec(path);
				retval = 1;
			}
			free_page((unsigned long)buf);
		}
	}
	return retval;
=======
static int notify_exec(struct mm_struct *mm)
{
	char *buf, *path;
	struct vm_area_struct *vma;

	if (!sim_is_simulator())
		return 1;

	if (mm->exe_file == NULL)
		return 0;

	for (vma = current->mm->mmap; ; vma = vma->vm_next) {
		if (vma == NULL)
			return 0;
		if (vma->vm_file == mm->exe_file)
			break;
	}

	buf = (char *) __get_free_page(GFP_KERNEL);
	if (buf == NULL)
		return 0;

	path = d_path(&mm->exe_file->f_path, buf, PAGE_SIZE);
	if (IS_ERR(path)) {
		free_page((unsigned long)buf);
		return 0;
	}

	/*
	 * Notify simulator of an ET_DYN object so we know the load address.
	 * The somewhat cryptic overuse of SIM_CONTROL_DLOPEN allows us
	 * to be backward-compatible with older simulator releases.
	 */
	if (vma->vm_start == (ELF_ET_DYN_BASE & PAGE_MASK)) {
		char buf[64];
		int i;

		snprintf(buf, sizeof(buf), "0x%lx:@", vma->vm_start);
		for (i = 0; ; ++i) {
			char c = buf[i];
			__insn_mtspr(SPR_SIM_CONTROL,
				     (SIM_CONTROL_DLOPEN
				      | (c << _SIM_CONTROL_OPERATOR_BITS)));
			if (c == '\0')
				break;
		}
	}

	sim_notify_exec(path);
	free_page((unsigned long)buf);
	return 1;
>>>>>>> refs/remotes/origin/master
}

/* Notify a running simulator, if any, that we loaded an interpreter. */
static void sim_notify_interp(unsigned long load_addr)
{
	size_t i;
	for (i = 0; i < sizeof(load_addr); i++) {
		unsigned char c = load_addr >> (i * 8);
		__insn_mtspr(SPR_SIM_CONTROL,
			     (SIM_CONTROL_OS_INTERP
			      | (c << _SIM_CONTROL_OPERATOR_BITS)));
	}
}


<<<<<<< HEAD
/* Kernel address of page used to map read-only kernel data into userspace. */
static void *vdso_page;

/* One-entry array used for install_special_mapping. */
static struct page *vdso_pages[1];

static int __init vdso_setup(void)
{
	vdso_page = (void *)get_zeroed_page(GFP_ATOMIC);
	memcpy(vdso_page, __rt_sigreturn, __rt_sigreturn_end - __rt_sigreturn);
	vdso_pages[0] = virt_to_page(vdso_page);
	return 0;
}
device_initcall(vdso_setup);

const char *arch_vma_name(struct vm_area_struct *vma)
{
	if (vma->vm_private_data == vdso_pages)
		return "[vdso]";
#ifndef __tilegx__
	if (vma->vm_start == MEM_USER_INTRPT)
		return "[intrpt]";
#endif
	return NULL;
}

=======
>>>>>>> refs/remotes/origin/master
int arch_setup_additional_pages(struct linux_binprm *bprm,
				int executable_stack)
{
	struct mm_struct *mm = current->mm;
<<<<<<< HEAD
	unsigned long vdso_base;
	int retval = 0;

=======
	int retval = 0;

	down_write(&mm->mmap_sem);

>>>>>>> refs/remotes/origin/master
	/*
	 * Notify the simulator that an exec just occurred.
	 * If we can't find the filename of the mapping, just use
	 * whatever was passed as the linux_binprm filename.
	 */
<<<<<<< HEAD
	if (!notify_exec())
		sim_notify_exec(bprm->filename);

	down_write(&mm->mmap_sem);

	/*
	 * MAYWRITE to allow gdb to COW and set breakpoints
<<<<<<< HEAD
	 *
	 * Make sure the vDSO gets into every core dump.  Dumping its
	 * contents makes post-mortem fully interpretable later
	 * without matching up the same kernel and hardware config to
	 * see what PC values meant.
=======
>>>>>>> refs/remotes/origin/cm-10.0
	 */
	vdso_base = VDSO_BASE;
	retval = install_special_mapping(mm, vdso_base, PAGE_SIZE,
					 VM_READ|VM_EXEC|
<<<<<<< HEAD
					 VM_MAYREAD|VM_MAYWRITE|VM_MAYEXEC|
					 VM_ALWAYSDUMP,
=======
					 VM_MAYREAD|VM_MAYWRITE|VM_MAYEXEC,
>>>>>>> refs/remotes/origin/cm-10.0
					 vdso_pages);
=======
	if (!notify_exec(mm))
		sim_notify_exec(bprm->filename);

	retval = setup_vdso_pages();
>>>>>>> refs/remotes/origin/master

#ifndef __tilegx__
	/*
	 * Set up a user-interrupt mapping here; the user can't
	 * create one themselves since it is above TASK_SIZE.
	 * We make it unwritable by default, so the model for adding
	 * interrupt vectors always involves an mprotect.
	 */
	if (!retval) {
		unsigned long addr = MEM_USER_INTRPT;
		addr = mmap_region(NULL, addr, INTRPT_SIZE,
<<<<<<< HEAD
				   MAP_FIXED|MAP_ANONYMOUS|MAP_PRIVATE,
=======
>>>>>>> refs/remotes/origin/master
				   VM_READ|VM_EXEC|
				   VM_MAYREAD|VM_MAYWRITE|VM_MAYEXEC, 0);
		if (addr > (unsigned long) -PAGE_SIZE)
			retval = (int) addr;
	}
#endif

	up_write(&mm->mmap_sem);

	return retval;
}


void elf_plat_init(struct pt_regs *regs, unsigned long load_addr)
{
	/* Zero all registers. */
	memset(regs, 0, sizeof(*regs));

	/* Report the interpreter's load address. */
	sim_notify_interp(load_addr);
}
