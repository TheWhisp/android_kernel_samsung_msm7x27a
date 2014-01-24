/*
 *  linux/arch/arm/lib/changebit.S
 *
 *  Copyright (C) 1995-1996 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
#include "bitops.h"
                .text

<<<<<<< HEAD
<<<<<<< HEAD
ENTRY(_change_bit)
	bitop	eor
ENDPROC(_change_bit)
=======
bitop	_change_bit, eor
>>>>>>> refs/remotes/origin/cm-10.0
=======
bitop	_change_bit, eor
>>>>>>> refs/remotes/origin/master
