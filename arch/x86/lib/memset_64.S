/* Copyright 2002 Andi Kleen, SuSE Labs */

#include <linux/linkage.h>
#include <asm/dwarf2.h>
#include <asm/cpufeature.h>
#include <asm/alternative-asm.h>

/*
 * ISO C memset - set a memory block to a byte value. This function uses fast
 * string to get better performance than the original function. The code is
 * simpler and shorter than the orignal function as well.
 *	
 * rdi   destination
 * rsi   value (char) 
 * rdx   count (bytes) 
 * 
 * rax   original destination
 */	
	.section .altinstr_replacement, "ax", @progbits
.Lmemset_c:
	movq %rdi,%r9
<<<<<<< HEAD
	movl %edx,%r8d
	andl $7,%r8d
	movl %edx,%ecx
	shrl $3,%ecx
	/* expand byte value  */
	movzbl %sil,%esi
	movabs $0x0101010101010101,%rax
	mulq %rsi		/* with rax, clobbers rdx */
	rep stosq
	movl %r8d,%ecx
=======
	movq %rdx,%rcx
	andl $7,%edx
	shrq $3,%rcx
	/* expand byte value  */
	movzbl %sil,%esi
	movabs $0x0101010101010101,%rax
	imulq %rsi,%rax
	rep stosq
	movl %edx,%ecx
>>>>>>> refs/remotes/origin/cm-10.0
	rep stosb
	movq %r9,%rax
	ret
.Lmemset_e:
	.previous

/*
 * ISO C memset - set a memory block to a byte value. This function uses
 * enhanced rep stosb to override the fast string function.
 * The code is simpler and shorter than the fast string function as well.
 *
 * rdi   destination
 * rsi   value (char)
 * rdx   count (bytes)
 *
 * rax   original destination
 */
	.section .altinstr_replacement, "ax", @progbits
.Lmemset_c_e:
	movq %rdi,%r9
	movb %sil,%al
<<<<<<< HEAD
	movl %edx,%ecx
=======
	movq %rdx,%rcx
>>>>>>> refs/remotes/origin/cm-10.0
	rep stosb
	movq %r9,%rax
	ret
.Lmemset_e_e:
	.previous

ENTRY(memset)
ENTRY(__memset)
	CFI_STARTPROC
	movq %rdi,%r10
<<<<<<< HEAD
	movq %rdx,%r11
=======
>>>>>>> refs/remotes/origin/cm-10.0

	/* expand byte value  */
	movzbl %sil,%ecx
	movabs $0x0101010101010101,%rax
<<<<<<< HEAD
	mul    %rcx		/* with rax, clobbers rdx */
=======
	imulq  %rcx,%rax
>>>>>>> refs/remotes/origin/cm-10.0

	/* align dst */
	movl  %edi,%r9d
	andl  $7,%r9d
	jnz  .Lbad_alignment
	CFI_REMEMBER_STATE
.Lafter_bad_alignment:

<<<<<<< HEAD
	movl %r11d,%ecx
	shrl $6,%ecx
=======
	movq  %rdx,%rcx
	shrq  $6,%rcx
>>>>>>> refs/remotes/origin/cm-10.0
	jz	 .Lhandle_tail

	.p2align 4
.Lloop_64:
<<<<<<< HEAD
	decl   %ecx
=======
	decq  %rcx
>>>>>>> refs/remotes/origin/cm-10.0
	movq  %rax,(%rdi)
	movq  %rax,8(%rdi)
	movq  %rax,16(%rdi)
	movq  %rax,24(%rdi)
	movq  %rax,32(%rdi)
	movq  %rax,40(%rdi)
	movq  %rax,48(%rdi)
	movq  %rax,56(%rdi)
	leaq  64(%rdi),%rdi
	jnz    .Lloop_64

	/* Handle tail in loops. The loops should be faster than hard
	   to predict jump tables. */
	.p2align 4
.Lhandle_tail:
<<<<<<< HEAD
	movl	%r11d,%ecx
=======
	movl	%edx,%ecx
>>>>>>> refs/remotes/origin/cm-10.0
	andl    $63&(~7),%ecx
	jz 		.Lhandle_7
	shrl	$3,%ecx
	.p2align 4
.Lloop_8:
	decl   %ecx
	movq  %rax,(%rdi)
	leaq  8(%rdi),%rdi
	jnz    .Lloop_8

.Lhandle_7:
<<<<<<< HEAD
	movl	%r11d,%ecx
	andl	$7,%ecx
	jz      .Lende
	.p2align 4
.Lloop_1:
	decl    %ecx
=======
	andl	$7,%edx
	jz      .Lende
	.p2align 4
.Lloop_1:
	decl    %edx
>>>>>>> refs/remotes/origin/cm-10.0
	movb 	%al,(%rdi)
	leaq	1(%rdi),%rdi
	jnz     .Lloop_1

.Lende:
	movq	%r10,%rax
	ret

	CFI_RESTORE_STATE
.Lbad_alignment:
<<<<<<< HEAD
	cmpq $7,%r11
=======
	cmpq $7,%rdx
>>>>>>> refs/remotes/origin/cm-10.0
	jbe	.Lhandle_7
	movq %rax,(%rdi)	/* unaligned store */
	movq $8,%r8
	subq %r9,%r8
	addq %r8,%rdi
<<<<<<< HEAD
	subq %r8,%r11
=======
	subq %r8,%rdx
>>>>>>> refs/remotes/origin/cm-10.0
	jmp .Lafter_bad_alignment
.Lfinal:
	CFI_ENDPROC
ENDPROC(memset)
ENDPROC(__memset)

	/* Some CPUs support enhanced REP MOVSB/STOSB feature.
	 * It is recommended to use this when possible.
	 *
	 * If enhanced REP MOVSB/STOSB feature is not available, use fast string
	 * instructions.
	 *
	 * Otherwise, use original memset function.
	 *
	 * In .altinstructions section, ERMS feature is placed after REG_GOOD
         * feature to implement the right patch order.
	 */
	.section .altinstructions,"a"
	altinstruction_entry memset,.Lmemset_c,X86_FEATURE_REP_GOOD,\
			     .Lfinal-memset,.Lmemset_e-.Lmemset_c
	altinstruction_entry memset,.Lmemset_c_e,X86_FEATURE_ERMS, \
			     .Lfinal-memset,.Lmemset_e_e-.Lmemset_c_e
	.previous
