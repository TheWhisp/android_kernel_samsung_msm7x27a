/* ----------------------------------------------------------------------- *
 *
 *   Copyright (C) 1991, 1992 Linus Torvalds
 *   Copyright 2007 rPath, Inc. - All Rights Reserved
 *
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2.
 *
 * ----------------------------------------------------------------------- */

#include <linux/linkage.h>

/*
 * Memory copy routines
 */

<<<<<<< HEAD
	.code16gcc
=======
	.code16
>>>>>>> refs/remotes/origin/master
	.text

GLOBAL(memcpy)
	pushw	%si
	pushw	%di
	movw	%ax, %di
	movw	%dx, %si
	pushw	%cx
	shrw	$2, %cx
	rep; movsl
	popw	%cx
	andw	$3, %cx
	rep; movsb
	popw	%di
	popw	%si
<<<<<<< HEAD
	ret
=======
	retl
>>>>>>> refs/remotes/origin/master
ENDPROC(memcpy)

GLOBAL(memset)
	pushw	%di
	movw	%ax, %di
	movzbl	%dl, %eax
	imull	$0x01010101,%eax
	pushw	%cx
	shrw	$2, %cx
	rep; stosl
	popw	%cx
	andw	$3, %cx
	rep; stosb
	popw	%di
<<<<<<< HEAD
	ret
=======
	retl
>>>>>>> refs/remotes/origin/master
ENDPROC(memset)

GLOBAL(copy_from_fs)
	pushw	%ds
	pushw	%fs
	popw	%ds
<<<<<<< HEAD
	call	memcpy
	popw	%ds
	ret
=======
	calll	memcpy
	popw	%ds
	retl
>>>>>>> refs/remotes/origin/master
ENDPROC(copy_from_fs)

GLOBAL(copy_to_fs)
	pushw	%es
	pushw	%fs
	popw	%es
<<<<<<< HEAD
	call	memcpy
	popw	%es
	ret
=======
	calll	memcpy
	popw	%es
	retl
>>>>>>> refs/remotes/origin/master
ENDPROC(copy_to_fs)

#if 0 /* Not currently used, but can be enabled as needed */
GLOBAL(copy_from_gs)
	pushw	%ds
	pushw	%gs
	popw	%ds
<<<<<<< HEAD
	call	memcpy
	popw	%ds
	ret
=======
	calll	memcpy
	popw	%ds
	retl
>>>>>>> refs/remotes/origin/master
ENDPROC(copy_from_gs)

GLOBAL(copy_to_gs)
	pushw	%es
	pushw	%gs
	popw	%es
<<<<<<< HEAD
	call	memcpy
	popw	%es
	ret
=======
	calll	memcpy
	popw	%es
	retl
>>>>>>> refs/remotes/origin/master
ENDPROC(copy_to_gs)
#endif
