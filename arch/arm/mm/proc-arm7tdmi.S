/*
 *  linux/arch/arm/mm/proc-arm7tdmi.S: utility functions for ARM7TDMI
 *
 *  Copyright (C) 2003-2006 Hyok S. Choi <hyok.choi@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/assembler.h>
#include <asm/asm-offsets.h>
#include <asm/hwcap.h>
#include <asm/pgtable-hwdef.h>
#include <asm/pgtable.h>
#include <asm/ptrace.h>

<<<<<<< HEAD
=======
#include "proc-macros.S"

>>>>>>> refs/remotes/origin/cm-10.0
	.text
/*
 * cpu_arm7tdmi_proc_init()
 * cpu_arm7tdmi_do_idle()
 * cpu_arm7tdmi_dcache_clean_area()
 * cpu_arm7tdmi_switch_mm()
 *
 * These are not required.
 */
ENTRY(cpu_arm7tdmi_proc_init)
ENTRY(cpu_arm7tdmi_do_idle)
ENTRY(cpu_arm7tdmi_dcache_clean_area)
ENTRY(cpu_arm7tdmi_switch_mm)
		mov	pc, lr

/*
 * cpu_arm7tdmi_proc_fin()
 */
ENTRY(cpu_arm7tdmi_proc_fin)
		mov	pc, lr

/*
 * Function: cpu_arm7tdmi_reset(loc)
 * Params  : loc(r0)	address to jump to
 * Purpose : Sets up everything for a reset and jump to the location for soft reset.
 */
<<<<<<< HEAD
ENTRY(cpu_arm7tdmi_reset)
		mov	pc, r0
=======
		.pushsection	.idmap.text, "ax"
ENTRY(cpu_arm7tdmi_reset)
		mov	pc, r0
ENDPROC(cpu_arm7tdmi_reset)
		.popsection
>>>>>>> refs/remotes/origin/cm-10.0

		__CPUINIT

		.type	__arm7tdmi_setup, #function
__arm7tdmi_setup:
		mov	pc, lr
		.size	__arm7tdmi_setup, . - __arm7tdmi_setup

		__INITDATA

<<<<<<< HEAD
/*
 * Purpose : Function pointers used to access above functions - all calls
 *	     come through these
 */
		.type	arm7tdmi_processor_functions, #object
ENTRY(arm7tdmi_processor_functions)
		.word	v4t_late_abort
		.word	legacy_pabort
		.word	cpu_arm7tdmi_proc_init
		.word	cpu_arm7tdmi_proc_fin
		.word	cpu_arm7tdmi_reset
		.word	cpu_arm7tdmi_do_idle
		.word	cpu_arm7tdmi_dcache_clean_area
		.word	cpu_arm7tdmi_switch_mm
		.word	0		@ cpu_*_set_pte
		.word	0
		.word	0
		.word	0
		.size	arm7tdmi_processor_functions, . - arm7tdmi_processor_functions

		.section ".rodata"

		.type	cpu_arch_name, #object
cpu_arch_name:
		.asciz	"armv4t"
		.size	cpu_arch_name, . - cpu_arch_name

		.type	cpu_elf_name, #object
cpu_elf_name:
		.asciz	"v4"
		.size	cpu_elf_name, . - cpu_elf_name

		.type	cpu_arm7tdmi_name, #object
cpu_arm7tdmi_name:
		.asciz	"ARM7TDMI"
		.size	cpu_arm7tdmi_name, . - cpu_arm7tdmi_name

		.type	cpu_triscenda7_name, #object
cpu_triscenda7_name:
		.asciz	"Triscend-A7x"
		.size	cpu_triscenda7_name, . - cpu_triscenda7_name

		.type	cpu_at91_name, #object
cpu_at91_name:
		.asciz	"Atmel-AT91M40xxx"
		.size	cpu_at91_name, . - cpu_at91_name

		.type	cpu_s3c3410_name, #object
cpu_s3c3410_name:
		.asciz	"Samsung-S3C3410"
		.size	cpu_s3c3410_name, . - cpu_s3c3410_name

		.type	cpu_s3c44b0x_name, #object
cpu_s3c44b0x_name:
		.asciz	"Samsung-S3C44B0x"
		.size	cpu_s3c44b0x_name, . - cpu_s3c44b0x_name

		.type	cpu_s3c4510b, #object
cpu_s3c4510b_name:
		.asciz	"Samsung-S3C4510B"
		.size	cpu_s3c4510b_name, . - cpu_s3c4510b_name

		.type	cpu_s3c4530_name, #object
cpu_s3c4530_name:
		.asciz	"Samsung-S3C4530"
		.size	cpu_s3c4530_name, . - cpu_s3c4530_name

		.type	cpu_netarm_name, #object
cpu_netarm_name:
		.asciz	"NETARM"
		.size	cpu_netarm_name, . - cpu_netarm_name
=======
		@ define struct processor (see <asm/proc-fns.h> and proc-macros.S)
		define_processor_functions arm7tdmi, dabort=v4t_late_abort, pabort=legacy_pabort, nommu=1

		.section ".rodata"

		string	cpu_arch_name, "armv4t"
		string	cpu_elf_name, "v4"
		string	cpu_arm7tdmi_name, "ARM7TDMI"
		string	cpu_triscenda7_name, "Triscend-A7x"
		string	cpu_at91_name, "Atmel-AT91M40xxx"
		string	cpu_s3c3410_name, "Samsung-S3C3410"
		string	cpu_s3c44b0x_name, "Samsung-S3C44B0x"
		string	cpu_s3c4510b_name, "Samsung-S3C4510B"
		string	cpu_s3c4530_name, "Samsung-S3C4530"
		string	cpu_netarm_name, "NETARM"
>>>>>>> refs/remotes/origin/cm-10.0

		.align

		.section ".proc.info.init", #alloc, #execinstr

<<<<<<< HEAD
		.type	__arm7tdmi_proc_info, #object
__arm7tdmi_proc_info:
		.long	0x41007700
		.long	0xfff8ff00
		.long	0
		.long	0
		b	__arm7tdmi_setup
		.long	cpu_arch_name
		.long	cpu_elf_name
		.long	HWCAP_SWP | HWCAP_26BIT
		.long	cpu_arm7tdmi_name
		.long	arm7tdmi_processor_functions
		.long	0
		.long	0
		.long	v4_cache_fns
		.size	__arm7tdmi_proc_info, . - __arm7tdmi_proc_info

		.type	__triscenda7_proc_info, #object
__triscenda7_proc_info:
		.long	0x0001d2ff
		.long	0x0001ffff
		.long	0
		.long	0
		b	__arm7tdmi_setup
		.long	cpu_arch_name
		.long	cpu_elf_name
		.long	HWCAP_SWP | HWCAP_THUMB | HWCAP_26BIT
		.long	cpu_triscenda7_name
		.long	arm7tdmi_processor_functions
		.long	0
		.long	0
		.long	v4_cache_fns
		.size	__triscenda7_proc_info, . - __triscenda7_proc_info

		.type	__at91_proc_info, #object
__at91_proc_info:
		.long	0x14000040
		.long	0xfff000e0
		.long	0
		.long	0
		b	__arm7tdmi_setup
		.long	cpu_arch_name
		.long	cpu_elf_name
		.long	HWCAP_SWP | HWCAP_THUMB | HWCAP_26BIT
		.long	cpu_at91_name
		.long	arm7tdmi_processor_functions
		.long	0
		.long	0
		.long	v4_cache_fns
		.size	__at91_proc_info, . - __at91_proc_info

		.type	__s3c4510b_proc_info, #object
__s3c4510b_proc_info:
		.long	0x36365000
		.long	0xfffff000
		.long	0
		.long	0
		b	__arm7tdmi_setup
		.long	cpu_arch_name
		.long	cpu_elf_name
		.long	HWCAP_SWP | HWCAP_THUMB | HWCAP_26BIT
		.long	cpu_s3c4510b_name
		.long	arm7tdmi_processor_functions
		.long	0
		.long	0
		.long	v4_cache_fns
		.size	__s3c4510b_proc_info, . - __s3c4510b_proc_info

		.type	__s3c4530_proc_info, #object
__s3c4530_proc_info:
		.long	0x4c000000
		.long	0xfff000e0
		.long	0
		.long	0
		b	__arm7tdmi_setup
		.long	cpu_arch_name
		.long	cpu_elf_name
		.long	HWCAP_SWP | HWCAP_THUMB | HWCAP_26BIT
		.long	cpu_s3c4530_name
		.long	arm7tdmi_processor_functions
		.long	0
		.long	0
		.long	v4_cache_fns
		.size	__s3c4530_proc_info, . - __s3c4530_proc_info

		.type	__s3c3410_proc_info, #object
__s3c3410_proc_info:
		.long	0x34100000
		.long	0xffff0000
		.long	0
		.long	0
		b	__arm7tdmi_setup
		.long	cpu_arch_name
		.long	cpu_elf_name
		.long	HWCAP_SWP | HWCAP_THUMB | HWCAP_26BIT
		.long	cpu_s3c3410_name
		.long	arm7tdmi_processor_functions
		.long	0
		.long	0
		.long	v4_cache_fns
		.size	__s3c3410_proc_info, . - __s3c3410_proc_info

		.type	__s3c44b0x_proc_info, #object
__s3c44b0x_proc_info:
		.long	0x44b00000
		.long	0xffff0000
=======
.macro arm7tdmi_proc_info name:req, cpu_val:req, cpu_mask:req, cpu_name:req, \
	extra_hwcaps=0
		.type	__\name\()_proc_info, #object
__\name\()_proc_info:
		.long	\cpu_val
		.long	\cpu_mask
>>>>>>> refs/remotes/origin/cm-10.0
		.long	0
		.long	0
		b	__arm7tdmi_setup
		.long	cpu_arch_name
		.long	cpu_elf_name
<<<<<<< HEAD
		.long	HWCAP_SWP | HWCAP_THUMB | HWCAP_26BIT
		.long	cpu_s3c44b0x_name
=======
		.long	HWCAP_SWP | HWCAP_26BIT | ( \extra_hwcaps )
		.long	\cpu_name
>>>>>>> refs/remotes/origin/cm-10.0
		.long	arm7tdmi_processor_functions
		.long	0
		.long	0
		.long	v4_cache_fns
<<<<<<< HEAD
		.size	__s3c44b0x_proc_info, . - __s3c44b0x_proc_info
=======
		.size	__\name\()_proc_info, . - __\name\()_proc_info
.endm

		arm7tdmi_proc_info arm7tdmi, 0x41007700, 0xfff8ff00, \
			cpu_arm7tdmi_name
		arm7tdmi_proc_info triscenda7, 0x0001d2ff, 0x0001ffff, \
			cpu_triscenda7_name, extra_hwcaps=HWCAP_THUMB
		arm7tdmi_proc_info at91, 0x14000040, 0xfff000e0, \
			cpu_at91_name, extra_hwcaps=HWCAP_THUMB
		arm7tdmi_proc_info s3c4510b, 0x36365000, 0xfffff000, \
			cpu_s3c4510b_name, extra_hwcaps=HWCAP_THUMB
		arm7tdmi_proc_info s3c4530, 0x4c000000, 0xfff000e0, \
			cpu_s3c4530_name, extra_hwcaps=HWCAP_THUMB
		arm7tdmi_proc_info s3c3410, 0x34100000, 0xffff0000, \
			cpu_s3c3410_name, extra_hwcaps=HWCAP_THUMB
		arm7tdmi_proc_info s3c44b0x, 0x44b00000, 0xffff0000, \
			cpu_s3c44b0x_name, extra_hwcaps=HWCAP_THUMB
>>>>>>> refs/remotes/origin/cm-10.0
