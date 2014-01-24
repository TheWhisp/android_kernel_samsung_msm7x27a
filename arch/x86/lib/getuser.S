/*
 * __get_user functions.
 *
 * (C) Copyright 1998 Linus Torvalds
 * (C) Copyright 2005 Andi Kleen
 * (C) Copyright 2008 Glauber Costa
 *
 * These functions have a non-standard call interface
 * to make them more efficient, especially as they
 * return an error value in addition to the "real"
 * return value.
 */

/*
 * __get_user_X
 *
 * Inputs:	%[r|e]ax contains the address.
<<<<<<< HEAD
 *		The register is modified, but all changes are undone
 *		before returning because the C code doesn't know about it.
 *
 * Outputs:	%[r|e]ax is error code (0 or -EFAULT)
 *		%[r|e]dx contains zero-extended value
=======
 *
 * Outputs:	%[r|e]ax is error code (0 or -EFAULT)
 *		%[r|e]dx contains zero-extended value
 *		%ecx contains the high half for 32-bit __get_user_8
>>>>>>> refs/remotes/origin/master
 *
 *
 * These functions should not modify any other registers,
 * as they get called from within inline assembly.
 */

#include <linux/linkage.h>
#include <asm/dwarf2.h>
#include <asm/page_types.h>
#include <asm/errno.h>
#include <asm/asm-offsets.h>
#include <asm/thread_info.h>
#include <asm/asm.h>
<<<<<<< HEAD
=======
#include <asm/smap.h>
>>>>>>> refs/remotes/origin/master

	.text
ENTRY(__get_user_1)
	CFI_STARTPROC
	GET_THREAD_INFO(%_ASM_DX)
	cmp TI_addr_limit(%_ASM_DX),%_ASM_AX
	jae bad_get_user
<<<<<<< HEAD
1:	movzb (%_ASM_AX),%edx
	xor %eax,%eax
=======
	ASM_STAC
1:	movzbl (%_ASM_AX),%edx
	xor %eax,%eax
	ASM_CLAC
>>>>>>> refs/remotes/origin/master
	ret
	CFI_ENDPROC
ENDPROC(__get_user_1)

ENTRY(__get_user_2)
	CFI_STARTPROC
	add $1,%_ASM_AX
	jc bad_get_user
	GET_THREAD_INFO(%_ASM_DX)
	cmp TI_addr_limit(%_ASM_DX),%_ASM_AX
	jae bad_get_user
<<<<<<< HEAD
2:	movzwl -1(%_ASM_AX),%edx
	xor %eax,%eax
=======
	ASM_STAC
2:	movzwl -1(%_ASM_AX),%edx
	xor %eax,%eax
	ASM_CLAC
>>>>>>> refs/remotes/origin/master
	ret
	CFI_ENDPROC
ENDPROC(__get_user_2)

ENTRY(__get_user_4)
	CFI_STARTPROC
	add $3,%_ASM_AX
	jc bad_get_user
	GET_THREAD_INFO(%_ASM_DX)
	cmp TI_addr_limit(%_ASM_DX),%_ASM_AX
	jae bad_get_user
<<<<<<< HEAD
3:	mov -3(%_ASM_AX),%edx
	xor %eax,%eax
=======
	ASM_STAC
3:	movl -3(%_ASM_AX),%edx
	xor %eax,%eax
	ASM_CLAC
>>>>>>> refs/remotes/origin/master
	ret
	CFI_ENDPROC
ENDPROC(__get_user_4)

<<<<<<< HEAD
#ifdef CONFIG_X86_64
ENTRY(__get_user_8)
	CFI_STARTPROC
=======
ENTRY(__get_user_8)
	CFI_STARTPROC
#ifdef CONFIG_X86_64
>>>>>>> refs/remotes/origin/master
	add $7,%_ASM_AX
	jc bad_get_user
	GET_THREAD_INFO(%_ASM_DX)
	cmp TI_addr_limit(%_ASM_DX),%_ASM_AX
<<<<<<< HEAD
	jae	bad_get_user
4:	movq -7(%_ASM_AX),%_ASM_DX
	xor %eax,%eax
	ret
	CFI_ENDPROC
ENDPROC(__get_user_8)
#endif
=======
	jae bad_get_user
	ASM_STAC
4:	movq -7(%_ASM_AX),%rdx
	xor %eax,%eax
	ASM_CLAC
	ret
#else
	add $7,%_ASM_AX
	jc bad_get_user_8
	GET_THREAD_INFO(%_ASM_DX)
	cmp TI_addr_limit(%_ASM_DX),%_ASM_AX
	jae bad_get_user_8
	ASM_STAC
4:	movl -7(%_ASM_AX),%edx
5:	movl -3(%_ASM_AX),%ecx
	xor %eax,%eax
	ASM_CLAC
	ret
#endif
	CFI_ENDPROC
ENDPROC(__get_user_8)

>>>>>>> refs/remotes/origin/master

bad_get_user:
	CFI_STARTPROC
	xor %edx,%edx
	mov $(-EFAULT),%_ASM_AX
<<<<<<< HEAD
=======
	ASM_CLAC
>>>>>>> refs/remotes/origin/master
	ret
	CFI_ENDPROC
END(bad_get_user)

<<<<<<< HEAD
.section __ex_table,"a"
	_ASM_PTR 1b,bad_get_user
	_ASM_PTR 2b,bad_get_user
	_ASM_PTR 3b,bad_get_user
#ifdef CONFIG_X86_64
	_ASM_PTR 4b,bad_get_user
=======
#ifdef CONFIG_X86_32
bad_get_user_8:
	CFI_STARTPROC
	xor %edx,%edx
	xor %ecx,%ecx
	mov $(-EFAULT),%_ASM_AX
	ASM_CLAC
	ret
	CFI_ENDPROC
END(bad_get_user_8)
#endif

	_ASM_EXTABLE(1b,bad_get_user)
	_ASM_EXTABLE(2b,bad_get_user)
	_ASM_EXTABLE(3b,bad_get_user)
#ifdef CONFIG_X86_64
	_ASM_EXTABLE(4b,bad_get_user)
#else
	_ASM_EXTABLE(4b,bad_get_user_8)
	_ASM_EXTABLE(5b,bad_get_user_8)
>>>>>>> refs/remotes/origin/master
#endif
