/*
 *  linux/arch/i386/kernel/head32.c -- prepare to run common code
 *
 *  Copyright (C) 2000 Andrea Arcangeli <andrea@suse.de> SuSE
 *  Copyright (C) 2007 Eric Biederman <ebiederm@xmission.com>
 */

#include <linux/init.h>
#include <linux/start_kernel.h>
#include <linux/mm.h>
#include <linux/memblock.h>

#include <asm/setup.h>
#include <asm/sections.h>
#include <asm/e820.h>
#include <asm/page.h>
<<<<<<< HEAD
#include <asm/trampoline.h>
=======
>>>>>>> refs/remotes/origin/master
#include <asm/apic.h>
#include <asm/io_apic.h>
#include <asm/bios_ebda.h>
#include <asm/tlbflush.h>
<<<<<<< HEAD
=======
#include <asm/bootparam_utils.h>
>>>>>>> refs/remotes/origin/master

static void __init i386_default_early_setup(void)
{
	/* Initialize 32bit specific setup functions */
	x86_init.resources.reserve_resources = i386_reserve_resources;
	x86_init.mpparse.setup_ioapic_ids = setup_ioapic_ids_from_mpc;

	reserve_ebda_region();
}

<<<<<<< HEAD
void __init i386_start_kernel(void)
{
<<<<<<< HEAD
	memblock_init();

	memblock_x86_reserve_range(__pa_symbol(&_text), __pa_symbol(&__bss_stop), "TEXT DATA BSS");
=======
	memblock_reserve(__pa_symbol(&_text),
			 __pa_symbol(&__bss_stop) - __pa_symbol(&_text));
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_BLK_DEV_INITRD
	/* Reserve INITRD */
	if (boot_params.hdr.type_of_loader && boot_params.hdr.ramdisk_image) {
		/* Assume only end is not page aligned */
		u64 ramdisk_image = boot_params.hdr.ramdisk_image;
		u64 ramdisk_size  = boot_params.hdr.ramdisk_size;
		u64 ramdisk_end   = PAGE_ALIGN(ramdisk_image + ramdisk_size);
<<<<<<< HEAD
		memblock_x86_reserve_range(ramdisk_image, ramdisk_end, "RAMDISK");
=======
		memblock_reserve(ramdisk_image, ramdisk_end - ramdisk_image);
>>>>>>> refs/remotes/origin/cm-10.0
	}
#endif

	/* Call the subarch specific early setup function */
	switch (boot_params.hdr.hardware_subarch) {
	case X86_SUBARCH_MRST:
		x86_mrst_early_setup();
=======
asmlinkage void __init i386_start_kernel(void)
{
	sanitize_boot_params(&boot_params);

	/* Call the subarch specific early setup function */
	switch (boot_params.hdr.hardware_subarch) {
	case X86_SUBARCH_INTEL_MID:
		x86_intel_mid_early_setup();
>>>>>>> refs/remotes/origin/master
		break;
	case X86_SUBARCH_CE4100:
		x86_ce4100_early_setup();
		break;
	default:
		i386_default_early_setup();
		break;
	}

<<<<<<< HEAD
	/*
	 * At this point everything still needed from the boot loader
	 * or BIOS or kernel text should be early reserved or marked not
	 * RAM in e820. All other memory is free game.
	 */

=======
>>>>>>> refs/remotes/origin/master
	start_kernel();
}
