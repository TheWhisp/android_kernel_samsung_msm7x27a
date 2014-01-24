/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */
=======
=======
>>>>>>> refs/remotes/origin/master
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifndef _BLACKFIN_PAGE_H
#define _BLACKFIN_PAGE_H

<<<<<<< HEAD
#include <asm-generic/page.h>
#define MAP_NR(addr) (((unsigned long)(addr)-PAGE_OFFSET) >> PAGE_SHIFT)
=======
#define ARCH_PFN_OFFSET (CONFIG_PHY_RAM_BASE_ADDRESS >> PAGE_SHIFT)
#define MAP_NR(addr) ((unsigned long)(addr) >> PAGE_SHIFT)
>>>>>>> refs/remotes/origin/master

#define VM_DATA_DEFAULT_FLAGS \
	(VM_READ | VM_WRITE | \
	((current->personality & READ_IMPLIES_EXEC) ? VM_EXEC : 0 ) | \
		 VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

<<<<<<< HEAD
=======
#include <asm-generic/page.h>
>>>>>>> refs/remotes/origin/master
#include <asm-generic/memory_model.h>
#include <asm-generic/getorder.h>

#endif
