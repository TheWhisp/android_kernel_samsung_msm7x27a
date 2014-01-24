/*
 *  linux/arch/x86_64/kernel/head.S -- start in 32bit and switch to 64bit
 *
 *  Copyright (C) 2000 Andrea Arcangeli <andrea@suse.de> SuSE
 *  Copyright (C) 2000 Pavel Machek <pavel@suse.cz>
 *  Copyright (C) 2000 Karsten Keil <kkeil@suse.de>
 *  Copyright (C) 2001,2002 Andi Kleen <ak@suse.de>
 *  Copyright (C) 2005 Eric Biederman <ebiederm@xmission.com>
 */


#include <linux/linkage.h>
#include <linux/threads.h>
#include <linux/init.h>
#include <asm/segment.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/msr.h>
#include <asm/cache.h>
#include <asm/processor-flags.h>
#include <asm/percpu.h>
<<<<<<< HEAD
=======
#include <asm/nops.h>
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_PARAVIRT
#include <asm/asm-offsets.h>
#include <asm/paravirt.h>
<<<<<<< HEAD
#else
#define GET_CR2_INTO_RCX movq %cr2, %rcx
=======
#define GET_CR2_INTO(reg) GET_CR2_INTO_RAX ; movq %rax, reg
#else
#define GET_CR2_INTO(reg) movq %cr2, reg
#define INTERRUPT_RETURN iretq
>>>>>>> refs/remotes/origin/master
#endif

/* we are not able to switch in one step to the final KERNEL ADDRESS SPACE
 * because we need identity-mapped pages.
 *
 */

#define pud_index(x)	(((x) >> PUD_SHIFT) & (PTRS_PER_PUD-1))

L4_PAGE_OFFSET = pgd_index(__PAGE_OFFSET)
L3_PAGE_OFFSET = pud_index(__PAGE_OFFSET)
L4_START_KERNEL = pgd_index(__START_KERNEL_map)
L3_START_KERNEL = pud_index(__START_KERNEL_map)

	.text
	__HEAD
	.code64
	.globl startup_64
startup_64:
<<<<<<< HEAD

	/*
	 * At this point the CPU runs in 64bit mode CS.L = 1 CS.D = 1,
=======
	/*
	 * At this point the CPU runs in 64bit mode CS.L = 1 CS.D = 0,
>>>>>>> refs/remotes/origin/master
	 * and someone has loaded an identity mapped page table
	 * for us.  These identity mapped page tables map all of the
	 * kernel pages and possibly all of memory.
	 *
<<<<<<< HEAD
	 * %esi holds a physical pointer to real_mode_data.
=======
	 * %rsi holds a physical pointer to real_mode_data.
>>>>>>> refs/remotes/origin/master
	 *
	 * We come here either directly from a 64bit bootloader, or from
	 * arch/x86_64/boot/compressed/head.S.
	 *
	 * We only come here initially at boot nothing else comes here.
	 *
	 * Since we may be loaded at an address different from what we were
	 * compiled to run at we first fixup the physical addresses in our page
	 * tables and then reload them.
	 */

<<<<<<< HEAD
	/* Compute the delta between the address I am compiled to run at and the
=======
	/*
	 * Compute the delta between the address I am compiled to run at and the
>>>>>>> refs/remotes/origin/master
	 * address I am actually running at.
	 */
	leaq	_text(%rip), %rbp
	subq	$_text - __START_KERNEL_map, %rbp

	/* Is the address not 2M aligned? */
	movq	%rbp, %rax
	andl	$~PMD_PAGE_MASK, %eax
	testl	%eax, %eax
	jnz	bad_address

<<<<<<< HEAD
	/* Is the address too large? */
	leaq	_text(%rip), %rdx
	movq	$PGDIR_SIZE, %rax
	cmpq	%rax, %rdx
	jae	bad_address

	/* Fixup the physical addresses in the page table
	 */
	addq	%rbp, init_level4_pgt + 0(%rip)
	addq	%rbp, init_level4_pgt + (L4_PAGE_OFFSET*8)(%rip)
	addq	%rbp, init_level4_pgt + (L4_START_KERNEL*8)(%rip)

	addq	%rbp, level3_ident_pgt + 0(%rip)
=======
	/*
	 * Is the address too large?
	 */
	leaq	_text(%rip), %rax
	shrq	$MAX_PHYSMEM_BITS, %rax
	jnz	bad_address

	/*
	 * Fixup the physical addresses in the page table
	 */
	addq	%rbp, early_level4_pgt + (L4_START_KERNEL*8)(%rip)
>>>>>>> refs/remotes/origin/master

	addq	%rbp, level3_kernel_pgt + (510*8)(%rip)
	addq	%rbp, level3_kernel_pgt + (511*8)(%rip)

	addq	%rbp, level2_fixmap_pgt + (506*8)(%rip)

<<<<<<< HEAD
	/* Add an Identity mapping if I am above 1G */
	leaq	_text(%rip), %rdi
	andq	$PMD_PAGE_MASK, %rdi

	movq	%rdi, %rax
	shrq	$PUD_SHIFT, %rax
	andq	$(PTRS_PER_PUD - 1), %rax
	jz	ident_complete

	leaq	(level2_spare_pgt - __START_KERNEL_map + _KERNPG_TABLE)(%rbp), %rdx
	leaq	level3_ident_pgt(%rip), %rbx
	movq	%rdx, 0(%rbx, %rax, 8)

	movq	%rdi, %rax
	shrq	$PMD_SHIFT, %rax
	andq	$(PTRS_PER_PMD - 1), %rax
	leaq	__PAGE_KERNEL_IDENT_LARGE_EXEC(%rdi), %rdx
	leaq	level2_spare_pgt(%rip), %rbx
	movq	%rdx, 0(%rbx, %rax, 8)
ident_complete:
=======
	/*
	 * Set up the identity mapping for the switchover.  These
	 * entries should *NOT* have the global bit set!  This also
	 * creates a bunch of nonsense entries but that is fine --
	 * it avoids problems around wraparound.
	 */
	leaq	_text(%rip), %rdi
	leaq	early_level4_pgt(%rip), %rbx

	movq	%rdi, %rax
	shrq	$PGDIR_SHIFT, %rax

	leaq	(4096 + _KERNPG_TABLE)(%rbx), %rdx
	movq	%rdx, 0(%rbx,%rax,8)
	movq	%rdx, 8(%rbx,%rax,8)

	addq	$4096, %rdx
	movq	%rdi, %rax
	shrq	$PUD_SHIFT, %rax
	andl	$(PTRS_PER_PUD-1), %eax
	movq	%rdx, 4096(%rbx,%rax,8)
	incl	%eax
	andl	$(PTRS_PER_PUD-1), %eax
	movq	%rdx, 4096(%rbx,%rax,8)

	addq	$8192, %rbx
	movq	%rdi, %rax
	shrq	$PMD_SHIFT, %rdi
	addq	$(__PAGE_KERNEL_LARGE_EXEC & ~_PAGE_GLOBAL), %rax
	leaq	(_end - 1)(%rip), %rcx
	shrq	$PMD_SHIFT, %rcx
	subq	%rdi, %rcx
	incl	%ecx

1:
	andq	$(PTRS_PER_PMD - 1), %rdi
	movq	%rax, (%rbx,%rdi,8)
	incq	%rdi
	addq	$PMD_SIZE, %rax
	decl	%ecx
	jnz	1b
>>>>>>> refs/remotes/origin/master

	/*
	 * Fixup the kernel text+data virtual addresses. Note that
	 * we might write invalid pmds, when the kernel is relocated
	 * cleanup_highmap() fixes this up along with the mappings
	 * beyond _end.
	 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	leaq	level2_kernel_pgt(%rip), %rdi
	leaq	4096(%rdi), %r8
	/* See if it is a valid page table entry */
1:	testq	$1, 0(%rdi)
	jz	2f
	addq	%rbp, 0(%rdi)
	/* Go to the next page */
2:	addq	$8, %rdi
	cmp	%r8, %rdi
	jne	1b

	/* Fixup phys_base */
	addq	%rbp, phys_base(%rip)

<<<<<<< HEAD
	/* Fixup trampoline */
	addq	%rbp, trampoline_level4_pgt + 0(%rip)
	addq	%rbp, trampoline_level4_pgt + (511*8)(%rip)

	/* Due to ENTRY(), sometimes the empty space gets filled with
	 * zeros. Better take a jmp than relying on empty space being
	 * filled with 0x90 (nop)
	 */
	jmp secondary_startup_64
ENTRY(secondary_startup_64)
	/*
	 * At this point the CPU runs in 64bit mode CS.L = 1 CS.D = 1,
	 * and someone has loaded a mapped page table.
	 *
	 * %esi holds a physical pointer to real_mode_data.
=======
	movq	$(early_level4_pgt - __START_KERNEL_map), %rax
	jmp 1f
ENTRY(secondary_startup_64)
	/*
	 * At this point the CPU runs in 64bit mode CS.L = 1 CS.D = 0,
	 * and someone has loaded a mapped page table.
	 *
	 * %rsi holds a physical pointer to real_mode_data.
>>>>>>> refs/remotes/origin/master
	 *
	 * We come here either from startup_64 (using physical addresses)
	 * or from trampoline.S (using virtual addresses).
	 *
	 * Using virtual addresses from trampoline.S removes the need
	 * to have any identity mapped pages in the kernel page table
	 * after the boot processor executes this code.
	 */

<<<<<<< HEAD
	/* Enable PAE mode and PGE */
	movl	$(X86_CR4_PAE | X86_CR4_PGE), %eax
	movq	%rax, %cr4

	/* Setup early boot stage 4 level pagetables. */
	movq	$(init_level4_pgt - __START_KERNEL_map), %rax
=======
	movq	$(init_level4_pgt - __START_KERNEL_map), %rax
1:

	/* Enable PAE mode and PGE */
	movl	$(X86_CR4_PAE | X86_CR4_PGE), %ecx
	movq	%rcx, %cr4

	/* Setup early boot stage 4 level pagetables. */
>>>>>>> refs/remotes/origin/master
	addq	phys_base(%rip), %rax
	movq	%rax, %cr3

	/* Ensure I am executing from virtual addresses */
	movq	$1f, %rax
	jmp	*%rax
1:

	/* Check if nx is implemented */
	movl	$0x80000001, %eax
	cpuid
	movl	%edx,%edi

	/* Setup EFER (Extended Feature Enable Register) */
	movl	$MSR_EFER, %ecx
	rdmsr
	btsl	$_EFER_SCE, %eax	/* Enable System Call */
	btl	$20,%edi		/* No Execute supported? */
	jnc     1f
	btsl	$_EFER_NX, %eax
<<<<<<< HEAD
=======
	btsq	$_PAGE_BIT_NX,early_pmd_flags(%rip)
>>>>>>> refs/remotes/origin/master
1:	wrmsr				/* Make changes effective */

	/* Setup cr0 */
#define CR0_STATE	(X86_CR0_PE | X86_CR0_MP | X86_CR0_ET | \
			 X86_CR0_NE | X86_CR0_WP | X86_CR0_AM | \
			 X86_CR0_PG)
	movl	$CR0_STATE, %eax
	/* Make changes effective */
	movq	%rax, %cr0

	/* Setup a boot time stack */
<<<<<<< HEAD
	movq stack_start(%rip),%rsp
=======
	movq stack_start(%rip), %rsp
>>>>>>> refs/remotes/origin/master

	/* zero EFLAGS after setting rsp */
	pushq $0
	popfq

	/*
	 * We must switch to a new descriptor in kernel space for the GDT
	 * because soon the kernel won't have access anymore to the userspace
	 * addresses where we're currently running on. We have to do that here
	 * because in 32bit we couldn't load a 64bit linear address.
	 */
	lgdt	early_gdt_descr(%rip)

	/* set up data segments */
	xorl %eax,%eax
	movl %eax,%ds
	movl %eax,%ss
	movl %eax,%es

	/*
	 * We don't really need to load %fs or %gs, but load them anyway
	 * to kill any stale realmode selectors.  This allows execution
	 * under VT hardware.
	 */
	movl %eax,%fs
	movl %eax,%gs

	/* Set up %gs.
	 *
	 * The base of %gs always points to the bottom of the irqstack
	 * union.  If the stack protector canary is enabled, it is
	 * located at %gs:40.  Note that, on SMP, the boot cpu uses
	 * init data section till per cpu areas are set up.
	 */
	movl	$MSR_GS_BASE,%ecx
	movl	initial_gs(%rip),%eax
	movl	initial_gs+4(%rip),%edx
	wrmsr	

<<<<<<< HEAD
	/* esi is pointer to real mode structure with interesting info.
	   pass it to C */
	movl	%esi, %edi
=======
	/* rsi is pointer to real mode structure with interesting info.
	   pass it to C */
	movq	%rsi, %rdi
>>>>>>> refs/remotes/origin/master
	
	/* Finally jump to run C code and to be on real kernel address
	 * Since we are running on identity-mapped space we have to jump
	 * to the full 64bit address, this is only possible as indirect
	 * jump.  In addition we need to ensure %cs is set so we make this
	 * a far return.
<<<<<<< HEAD
=======
	 *
	 * Note: do not change to far jump indirect with 64bit offset.
	 *
	 * AMD does not support far jump indirect with 64bit offset.
	 * AMD64 Architecture Programmer's Manual, Volume 3: states only
	 *	JMP FAR mem16:16 FF /5 Far jump indirect,
	 *		with the target specified by a far pointer in memory.
	 *	JMP FAR mem16:32 FF /5 Far jump indirect,
	 *		with the target specified by a far pointer in memory.
	 *
	 * Intel64 does support 64bit offset.
	 * Software Developer Manual Vol 2: states:
	 *	FF /5 JMP m16:16 Jump far, absolute indirect,
	 *		address given in m16:16
	 *	FF /5 JMP m16:32 Jump far, absolute indirect,
	 *		address given in m16:32.
	 *	REX.W + FF /5 JMP m16:64 Jump far, absolute indirect,
	 *		address given in m16:64.
>>>>>>> refs/remotes/origin/master
	 */
	movq	initial_code(%rip),%rax
	pushq	$0		# fake return address to stop unwinder
	pushq	$__KERNEL_CS	# set correct cs
	pushq	%rax		# target address in negative space
	lretq

<<<<<<< HEAD
	/* SMP bootup changes these two */
	__REFDATA
	.align	8
	ENTRY(initial_code)
	.quad	x86_64_start_kernel
	ENTRY(initial_gs)
	.quad	INIT_PER_CPU_VAR(irq_stack_union)

	ENTRY(stack_start)
=======
#ifdef CONFIG_HOTPLUG_CPU
/*
 * Boot CPU0 entry point. It's called from play_dead(). Everything has been set
 * up already except stack. We just set up stack here. Then call
 * start_secondary().
 */
ENTRY(start_cpu0)
	movq stack_start(%rip),%rsp
	movq	initial_code(%rip),%rax
	pushq	$0		# fake return address to stop unwinder
	pushq	$__KERNEL_CS	# set correct cs
	pushq	%rax		# target address in negative space
	lretq
ENDPROC(start_cpu0)
#endif

	/* SMP bootup changes these two */
	__REFDATA
	.balign	8
	GLOBAL(initial_code)
	.quad	x86_64_start_kernel
	GLOBAL(initial_gs)
	.quad	INIT_PER_CPU_VAR(irq_stack_union)

	GLOBAL(stack_start)
>>>>>>> refs/remotes/origin/master
	.quad  init_thread_union+THREAD_SIZE-8
	.word  0
	__FINITDATA

bad_address:
	jmp bad_address

<<<<<<< HEAD
	.section ".init.text","ax"
#ifdef CONFIG_EARLY_PRINTK
	.globl early_idt_handlers
early_idt_handlers:
	i = 0
	.rept NUM_EXCEPTION_VECTORS
	movl $i, %esi
	jmp early_idt_handler
	i = i + 1
	.endr
#endif

ENTRY(early_idt_handler)
#ifdef CONFIG_EARLY_PRINTK
	cmpl $2,early_recursion_flag(%rip)
	jz  1f
	incl early_recursion_flag(%rip)
	GET_CR2_INTO_RCX
	movq %rcx,%r9
	xorl %r8d,%r8d		# zero for error code
	movl %esi,%ecx		# get vector number
	# Test %ecx against mask of vectors that push error code.
	cmpl $31,%ecx
	ja 0f
	movl $1,%eax
	salq %cl,%rax
	testl $0x27d00,%eax
	je 0f
	popq %r8		# get error code
0:	movq 0(%rsp),%rcx	# get ip
	movq 8(%rsp),%rdx	# get cs
=======
	__INIT
	.globl early_idt_handlers
early_idt_handlers:
	# 104(%rsp) %rflags
	#  96(%rsp) %cs
	#  88(%rsp) %rip
	#  80(%rsp) error code
	i = 0
	.rept NUM_EXCEPTION_VECTORS
	.if (EXCEPTION_ERRCODE_MASK >> i) & 1
	ASM_NOP2
	.else
	pushq $0		# Dummy error code, to make stack frame uniform
	.endif
	pushq $i		# 72(%rsp) Vector number
	jmp early_idt_handler
	i = i + 1
	.endr

/* This is global to keep gas from relaxing the jumps */
ENTRY(early_idt_handler)
	cld

	cmpl $2,early_recursion_flag(%rip)
	jz  1f
	incl early_recursion_flag(%rip)

	pushq %rax		# 64(%rsp)
	pushq %rcx		# 56(%rsp)
	pushq %rdx		# 48(%rsp)
	pushq %rsi		# 40(%rsp)
	pushq %rdi		# 32(%rsp)
	pushq %r8		# 24(%rsp)
	pushq %r9		# 16(%rsp)
	pushq %r10		#  8(%rsp)
	pushq %r11		#  0(%rsp)

	cmpl $__KERNEL_CS,96(%rsp)
	jne 11f

	cmpl $14,72(%rsp)	# Page fault?
	jnz 10f
	GET_CR2_INTO(%rdi)	# can clobber any volatile register if pv
	call early_make_pgtable
	andl %eax,%eax
	jz 20f			# All good

10:
	leaq 88(%rsp),%rdi	# Pointer to %rip
	call early_fixup_exception
	andl %eax,%eax
	jnz 20f			# Found an exception entry

11:
#ifdef CONFIG_EARLY_PRINTK
	GET_CR2_INTO(%r9)	# can clobber any volatile register if pv
	movl 80(%rsp),%r8d	# error code
	movl 72(%rsp),%esi	# vector number
	movl 96(%rsp),%edx	# %cs
	movq 88(%rsp),%rcx	# %rip
>>>>>>> refs/remotes/origin/master
	xorl %eax,%eax
	leaq early_idt_msg(%rip),%rdi
	call early_printk
	cmpl $2,early_recursion_flag(%rip)
	jz  1f
	call dump_stack
#ifdef CONFIG_KALLSYMS	
	leaq early_idt_ripmsg(%rip),%rdi
<<<<<<< HEAD
	movq 0(%rsp),%rsi	# get rip again
=======
	movq 40(%rsp),%rsi	# %rip again
>>>>>>> refs/remotes/origin/master
	call __print_symbol
#endif
#endif /* EARLY_PRINTK */
1:	hlt
	jmp 1b

<<<<<<< HEAD
#ifdef CONFIG_EARLY_PRINTK
early_recursion_flag:
	.long 0

=======
20:	# Exception table entry found or page table generated
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rax
	addq $16,%rsp		# drop vector number and error code
	decl early_recursion_flag(%rip)
	INTERRUPT_RETURN
ENDPROC(early_idt_handler)

	__INITDATA

	.balign 4
early_recursion_flag:
	.long 0

#ifdef CONFIG_EARLY_PRINTK
>>>>>>> refs/remotes/origin/master
early_idt_msg:
	.asciz "PANIC: early exception %02lx rip %lx:%lx error %lx cr2 %lx\n"
early_idt_ripmsg:
	.asciz "RIP %s\n"
#endif /* CONFIG_EARLY_PRINTK */
<<<<<<< HEAD
	.previous

#define NEXT_PAGE(name) \
	.balign	PAGE_SIZE; \
ENTRY(name)
=======

#define NEXT_PAGE(name) \
	.balign	PAGE_SIZE; \
GLOBAL(name)
>>>>>>> refs/remotes/origin/master

/* Automate the creation of 1 to 1 mapping pmd entries */
#define PMDS(START, PERM, COUNT)			\
	i = 0 ;						\
	.rept (COUNT) ;					\
	.quad	(START) + (i << PMD_SHIFT) + (PERM) ;	\
	i = i + 1 ;					\
	.endr

<<<<<<< HEAD
	.data
	/*
	 * This default setting generates an ident mapping at address 0x100000
	 * and a mapping for the kernel that precisely maps virtual address
	 * 0xffffffff80000000 to physical address 0x000000. (always using
	 * 2Mbyte large pages provided by PAE mode)
	 */
NEXT_PAGE(init_level4_pgt)
	.quad	level3_ident_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.org	init_level4_pgt + L4_PAGE_OFFSET*8, 0
	.quad	level3_ident_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.org	init_level4_pgt + L4_START_KERNEL*8, 0
	/* (2^48-(2*1024*1024*1024))/(2^39) = 511 */
	.quad	level3_kernel_pgt - __START_KERNEL_map + _PAGE_TABLE

NEXT_PAGE(level3_ident_pgt)
	.quad	level2_ident_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.fill	511,8,0
=======
	__INITDATA
NEXT_PAGE(early_level4_pgt)
	.fill	511,8,0
	.quad	level3_kernel_pgt - __START_KERNEL_map + _PAGE_TABLE

NEXT_PAGE(early_dynamic_pgts)
	.fill	512*EARLY_DYNAMIC_PAGE_TABLES,8,0

	.data

#ifndef CONFIG_XEN
NEXT_PAGE(init_level4_pgt)
	.fill	512,8,0
#else
NEXT_PAGE(init_level4_pgt)
	.quad   level3_ident_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.org    init_level4_pgt + L4_PAGE_OFFSET*8, 0
	.quad   level3_ident_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.org    init_level4_pgt + L4_START_KERNEL*8, 0
	/* (2^48-(2*1024*1024*1024))/(2^39) = 511 */
	.quad   level3_kernel_pgt - __START_KERNEL_map + _PAGE_TABLE

NEXT_PAGE(level3_ident_pgt)
	.quad	level2_ident_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.fill	511, 8, 0
NEXT_PAGE(level2_ident_pgt)
	/* Since I easily can, map the first 1G.
	 * Don't set NX because code runs from these pages.
	 */
	PMDS(0, __PAGE_KERNEL_IDENT_LARGE_EXEC, PTRS_PER_PMD)
#endif
>>>>>>> refs/remotes/origin/master

NEXT_PAGE(level3_kernel_pgt)
	.fill	L3_START_KERNEL,8,0
	/* (2^48-(2*1024*1024*1024)-((2^39)*511))/(2^30) = 510 */
	.quad	level2_kernel_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.quad	level2_fixmap_pgt - __START_KERNEL_map + _PAGE_TABLE

<<<<<<< HEAD
NEXT_PAGE(level2_fixmap_pgt)
	.fill	506,8,0
	.quad	level1_fixmap_pgt - __START_KERNEL_map + _PAGE_TABLE
	/* 8MB reserved for vsyscalls + a 2MB hole = 4 + 1 entries */
	.fill	5,8,0

NEXT_PAGE(level1_fixmap_pgt)
	.fill	512,8,0

NEXT_PAGE(level2_ident_pgt)
	/* Since I easily can, map the first 1G.
	 * Don't set NX because code runs from these pages.
	 */
	PMDS(0, __PAGE_KERNEL_IDENT_LARGE_EXEC, PTRS_PER_PMD)

=======
>>>>>>> refs/remotes/origin/master
NEXT_PAGE(level2_kernel_pgt)
	/*
	 * 512 MB kernel mapping. We spend a full page on this pagetable
	 * anyway.
	 *
	 * The kernel code+data+bss must not be bigger than that.
	 *
	 * (NOTE: at +512MB starts the module area, see MODULES_VADDR.
	 *  If you want to increase this then increase MODULES_VADDR
	 *  too.)
	 */
	PMDS(0, __PAGE_KERNEL_LARGE_EXEC,
		KERNEL_IMAGE_SIZE/PMD_SIZE)

<<<<<<< HEAD
NEXT_PAGE(level2_spare_pgt)
	.fill   512, 8, 0

#undef PMDS
#undef NEXT_PAGE
=======
NEXT_PAGE(level2_fixmap_pgt)
	.fill	506,8,0
	.quad	level1_fixmap_pgt - __START_KERNEL_map + _PAGE_TABLE
	/* 8MB reserved for vsyscalls + a 2MB hole = 4 + 1 entries */
	.fill	5,8,0

NEXT_PAGE(level1_fixmap_pgt)
	.fill	512,8,0

#undef PMDS
>>>>>>> refs/remotes/origin/master

	.data
	.align 16
	.globl early_gdt_descr
early_gdt_descr:
	.word	GDT_ENTRIES*8-1
early_gdt_descr_base:
	.quad	INIT_PER_CPU_VAR(gdt_page)

ENTRY(phys_base)
	/* This must match the first entry in level2_kernel_pgt */
	.quad   0x0000000000000000

#include "../../x86/xen/xen-head.S"
	
<<<<<<< HEAD
	.section .bss, "aw", @nobits
	.align L1_CACHE_BYTES
ENTRY(idt_table)
	.skip IDT_ENTRIES * 16

<<<<<<< HEAD
=======
	.align L1_CACHE_BYTES
ENTRY(nmi_idt_table)
	.skip IDT_ENTRIES * 16

>>>>>>> refs/remotes/origin/cm-10.0
	__PAGE_ALIGNED_BSS
	.align PAGE_SIZE
ENTRY(empty_zero_page)
=======
	__PAGE_ALIGNED_BSS
NEXT_PAGE(empty_zero_page)
>>>>>>> refs/remotes/origin/master
	.skip PAGE_SIZE
