/*
 * Common EFI (Extensible Firmware Interface) support functions
 * Based on Extensible Firmware Interface Specification version 1.0
 *
 * Copyright (C) 1999 VA Linux Systems
 * Copyright (C) 1999 Walt Drummond <drummond@valinux.com>
 * Copyright (C) 1999-2002 Hewlett-Packard Co.
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 *	Stephane Eranian <eranian@hpl.hp.com>
 * Copyright (C) 2005-2008 Intel Co.
 *	Fenghua Yu <fenghua.yu@intel.com>
 *	Bibo Mao <bibo.mao@intel.com>
 *	Chandramouli Narayanan <mouli@linux.intel.com>
 *	Huang Ying <ying.huang@intel.com>
<<<<<<< HEAD
=======
 * Copyright (C) 2013 SuSE Labs
 *	Borislav Petkov <bp@suse.de> - runtime services VA mapping
>>>>>>> refs/remotes/origin/master
 *
 * Copied from efi_32.c to eliminate the duplicated code between EFI
 * 32/64 support code. --ying 2007-10-26
 *
 * All EFI Runtime Services are not implemented yet as EFI only
 * supports physical mode addressing on SoftSDV. This is to be fixed
 * in a future version.  --drummond 1999-07-20
 *
 * Implemented EFI runtime services and virtual mode calls.  --davidm
 *
 * Goutham Rao: <goutham.rao@intel.com>
 *	Skip non-WB memory and ignore empty memory ranges.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/efi.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/efi.h>
<<<<<<< HEAD
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/bootmem.h>
=======
#include <linux/efi-bgrt.h>
#include <linux/export.h>
#include <linux/bootmem.h>
#include <linux/slab.h>
>>>>>>> refs/remotes/origin/master
#include <linux/memblock.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/io.h>
#include <linux/reboot.h>
#include <linux/bcd.h>

#include <asm/setup.h>
#include <asm/efi.h>
#include <asm/time.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/x86_init.h>
<<<<<<< HEAD

#define EFI_DEBUG	1
<<<<<<< HEAD
#define PFX 		"EFI: "

<<<<<<< HEAD
int efi_enabled;
EXPORT_SYMBOL(efi_enabled);

struct efi efi;
=======

=======
>>>>>>> refs/remotes/origin/cm-11.0
struct efi __read_mostly efi = {
	.mps        = EFI_INVALID_TABLE_ADDR,
	.acpi       = EFI_INVALID_TABLE_ADDR,
	.acpi20     = EFI_INVALID_TABLE_ADDR,
	.smbios     = EFI_INVALID_TABLE_ADDR,
	.sal_systab = EFI_INVALID_TABLE_ADDR,
	.boot_info  = EFI_INVALID_TABLE_ADDR,
	.hcdp       = EFI_INVALID_TABLE_ADDR,
	.uga        = EFI_INVALID_TABLE_ADDR,
	.uv_systab  = EFI_INVALID_TABLE_ADDR,
};
>>>>>>> refs/remotes/origin/cm-10.0
EXPORT_SYMBOL(efi);
=======
#include <asm/rtc.h>

#define EFI_DEBUG

#define EFI_MIN_RESERVE 5120

#define EFI_DUMMY_GUID \
	EFI_GUID(0x4424ac57, 0xbe4b, 0x47dd, 0x9e, 0x97, 0xed, 0x50, 0xf0, 0x9f, 0x92, 0xa9)

<<<<<<< HEAD
static efi_char16_t efi_dummy_name[6] = { 'D', 'U', 'M', 'M', 'Y', 0 };
>>>>>>> refs/remotes/origin/master

struct efi_memory_map memmap;

static struct efi efi_phys __initdata;
static efi_system_table_t efi_systab __initdata;

<<<<<<< HEAD
<<<<<<< HEAD
static int __init setup_noefi(char *arg)
{
	efi_enabled = 0;
=======
static inline bool efi_is_native(void)
{
	return IS_ENABLED(CONFIG_X86_64) == efi_enabled(EFI_64BIT);
}

unsigned long x86_efi_facility;

=======
unsigned long x86_efi_facility;

static __initdata efi_config_table_type_t arch_tables[] = {
#ifdef CONFIG_X86_UV
	{UV_SYSTEM_TABLE_GUID, "UVsystab", &efi.uv_systab},
#endif
	{NULL_GUID, NULL, NULL},
};

u64 efi_setup;		/* efi setup_data physical address */

>>>>>>> refs/remotes/origin/master
/*
 * Returns 1 if 'facility' is enabled, 0 otherwise.
 */
int efi_enabled(int facility)
{
	return test_bit(facility, &x86_efi_facility) != 0;
}
EXPORT_SYMBOL(efi_enabled);

<<<<<<< HEAD
static bool disable_runtime = false;
static int __init setup_noefi(char *arg)
{
	disable_runtime = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool __initdata disable_runtime = false;
static int __init setup_noefi(char *arg)
{
	disable_runtime = true;
>>>>>>> refs/remotes/origin/master
=======
static struct efi efi_phys __initdata;
static efi_system_table_t efi_systab __initdata;

static inline bool efi_is_native(void)
{
	return IS_ENABLED(CONFIG_X86_64) == efi_enabled(EFI_64BIT);
}

unsigned long x86_efi_facility;

/*
 * Returns 1 if 'facility' is enabled, 0 otherwise.
 */
int efi_enabled(int facility)
{
	return test_bit(facility, &x86_efi_facility) != 0;
}
EXPORT_SYMBOL(efi_enabled);

static bool disable_runtime = false;
static int __init setup_noefi(char *arg)
{
	disable_runtime = true;
>>>>>>> refs/remotes/origin/cm-11.0
	return 0;
}
early_param("noefi", setup_noefi);

int add_efi_memmap;
EXPORT_SYMBOL(add_efi_memmap);

static int __init setup_add_efi_memmap(char *arg)
{
	add_efi_memmap = 1;
	return 0;
}
early_param("add_efi_memmap", setup_add_efi_memmap);

<<<<<<< HEAD

static efi_status_t virt_efi_get_time(efi_time_t *tm, efi_time_cap_t *tc)
{
<<<<<<< HEAD
	return efi_call_virt2(get_time, tm, tc);
=======
=======
static bool efi_no_storage_paranoia;

static int __init setup_storage_paranoia(char *arg)
{
	efi_no_storage_paranoia = true;
	return 0;
}
early_param("efi_no_storage_paranoia", setup_storage_paranoia);

static efi_status_t virt_efi_get_time(efi_time_t *tm, efi_time_cap_t *tc)
{
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	efi_status_t status;

	spin_lock_irqsave(&rtc_lock, flags);
	status = efi_call_virt2(get_time, tm, tc);
	spin_unlock_irqrestore(&rtc_lock, flags);
	return status;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static efi_status_t virt_efi_set_time(efi_time_t *tm)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return efi_call_virt1(set_time, tm);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	efi_status_t status;

	spin_lock_irqsave(&rtc_lock, flags);
	status = efi_call_virt1(set_time, tm);
	spin_unlock_irqrestore(&rtc_lock, flags);
	return status;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static efi_status_t virt_efi_get_wakeup_time(efi_bool_t *enabled,
					     efi_bool_t *pending,
					     efi_time_t *tm)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return efi_call_virt3(get_wakeup_time,
			      enabled, pending, tm);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	efi_status_t status;

	spin_lock_irqsave(&rtc_lock, flags);
	status = efi_call_virt3(get_wakeup_time,
				enabled, pending, tm);
	spin_unlock_irqrestore(&rtc_lock, flags);
	return status;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static efi_status_t virt_efi_set_wakeup_time(efi_bool_t enabled, efi_time_t *tm)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return efi_call_virt2(set_wakeup_time,
			      enabled, tm);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	efi_status_t status;

	spin_lock_irqsave(&rtc_lock, flags);
	status = efi_call_virt2(set_wakeup_time,
				enabled, tm);
	spin_unlock_irqrestore(&rtc_lock, flags);
	return status;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static efi_status_t virt_efi_get_variable(efi_char16_t *name,
					  efi_guid_t *vendor,
					  u32 *attr,
					  unsigned long *data_size,
					  void *data)
{
	return efi_call_virt5(get_variable,
			      name, vendor, attr,
			      data_size, data);
}

static efi_status_t virt_efi_get_next_variable(unsigned long *name_size,
					       efi_char16_t *name,
					       efi_guid_t *vendor)
{
	return efi_call_virt3(get_next_variable,
			      name_size, name, vendor);
}

static efi_status_t virt_efi_set_variable(efi_char16_t *name,
					  efi_guid_t *vendor,
<<<<<<< HEAD
<<<<<<< HEAD
					  unsigned long attr,
=======
					  u32 attr,
>>>>>>> refs/remotes/origin/cm-10.0
=======
					  u32 attr,
>>>>>>> refs/remotes/origin/master
					  unsigned long data_size,
					  void *data)
{
	return efi_call_virt5(set_variable,
			      name, vendor, attr,
			      data_size, data);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static efi_status_t virt_efi_query_variable_info(u32 attr,
						 u64 *storage_space,
						 u64 *remaining_space,
						 u64 *max_variable_size)
{
	if (efi.runtime_version < EFI_2_00_SYSTEM_TABLE_REVISION)
		return EFI_UNSUPPORTED;

	return efi_call_virt4(query_variable_info, attr, storage_space,
			      remaining_space, max_variable_size);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static efi_status_t virt_efi_get_next_high_mono_count(u32 *count)
{
	return efi_call_virt1(get_next_high_mono_count, count);
}

static void virt_efi_reset_system(int reset_type,
				  efi_status_t status,
				  unsigned long data_size,
				  efi_char16_t *data)
{
	efi_call_virt4(reset_system, reset_type, status,
		       data_size, data);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static efi_status_t virt_efi_update_capsule(efi_capsule_header_t **capsules,
					    unsigned long count,
					    unsigned long sg_list)
{
	if (efi.runtime_version < EFI_2_00_SYSTEM_TABLE_REVISION)
		return EFI_UNSUPPORTED;

	return efi_call_virt3(update_capsule, capsules, count, sg_list);
}

static efi_status_t virt_efi_query_capsule_caps(efi_capsule_header_t **capsules,
						unsigned long count,
						u64 *max_size,
						int *reset_type)
{
	if (efi.runtime_version < EFI_2_00_SYSTEM_TABLE_REVISION)
		return EFI_UNSUPPORTED;

	return efi_call_virt4(query_capsule_caps, capsules, count, max_size,
			      reset_type);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static efi_status_t __init phys_efi_set_virtual_address_map(
	unsigned long memory_map_size,
	unsigned long descriptor_size,
	u32 descriptor_version,
	efi_memory_desc_t *virtual_map)
{
	efi_status_t status;

	efi_call_phys_prelog();
	status = efi_call_phys4(efi_phys.set_virtual_address_map,
				memory_map_size, descriptor_size,
				descriptor_version, virtual_map);
	efi_call_phys_epilog();
	return status;
}

static efi_status_t __init phys_efi_get_time(efi_time_t *tm,
					     efi_time_cap_t *tc)
{
<<<<<<< HEAD
<<<<<<< HEAD
	efi_status_t status;

	efi_call_phys_prelog();
	status = efi_call_phys2(efi_phys.get_time, tm, tc);
	efi_call_phys_epilog();
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	efi_status_t status;

	spin_lock_irqsave(&rtc_lock, flags);
	efi_call_phys_prelog();
	status = efi_call_phys2(efi_phys.get_time, virt_to_phys(tm),
				virt_to_phys(tc));
	efi_call_phys_epilog();
	spin_unlock_irqrestore(&rtc_lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return status;
}

int efi_set_rtc_mmss(unsigned long nowtime)
{
	int real_seconds, real_minutes;
	efi_status_t 	status;
	efi_time_t 	eft;
	efi_time_cap_t 	cap;

	status = efi.get_time(&eft, &cap);
	if (status != EFI_SUCCESS) {
<<<<<<< HEAD
		printk(KERN_ERR "Oops: efitime: can't read time!\n");
=======
		pr_err("Oops: efitime: can't read time!\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -1;
	}

	real_seconds = nowtime % 60;
	real_minutes = nowtime / 60;
	if (((abs(real_minutes - eft.minute) + 15)/30) & 1)
		real_minutes += 30;
	real_minutes %= 60;
	eft.minute = real_minutes;
	eft.second = real_seconds;

	status = efi.set_time(&eft);
	if (status != EFI_SUCCESS) {
<<<<<<< HEAD
		printk(KERN_ERR "Oops: efitime: can't write time!\n");
=======
		pr_err("Oops: efitime: can't write time!\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return status;
}

int efi_set_rtc_mmss(const struct timespec *now)
{
	unsigned long nowtime = now->tv_sec;
	efi_status_t 	status;
	efi_time_t 	eft;
	efi_time_cap_t 	cap;
	struct rtc_time	tm;

	status = efi.get_time(&eft, &cap);
	if (status != EFI_SUCCESS) {
		pr_err("Oops: efitime: can't read time!\n");
		return -1;
	}

	rtc_time_to_tm(nowtime, &tm);
	if (!rtc_valid_tm(&tm)) {
		eft.year = tm.tm_year + 1900;
		eft.month = tm.tm_mon + 1;
		eft.day = tm.tm_mday;
		eft.minute = tm.tm_min;
		eft.second = tm.tm_sec;
		eft.nanosecond = 0;
	} else {
		printk(KERN_ERR
		       "%s: Invalid EFI RTC value: write of %lx to EFI RTC failed\n",
		       __FUNCTION__, nowtime);
		return -1;
	}

	status = efi.set_time(&eft);
	if (status != EFI_SUCCESS) {
		pr_err("Oops: efitime: can't write time!\n");
>>>>>>> refs/remotes/origin/master
		return -1;
	}
	return 0;
}

<<<<<<< HEAD
unsigned long efi_get_time(void)
=======
void efi_get_time(struct timespec *now)
>>>>>>> refs/remotes/origin/master
{
	efi_status_t status;
	efi_time_t eft;
	efi_time_cap_t cap;

	status = efi.get_time(&eft, &cap);
	if (status != EFI_SUCCESS)
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Oops: efitime: can't read time!\n");
=======
		pr_err("Oops: efitime: can't read time!\n");
>>>>>>> refs/remotes/origin/cm-10.0

	return mktime(eft.year, eft.month, eft.day, eft.hour,
		      eft.minute, eft.second);
=======
		pr_err("Oops: efitime: can't read time!\n");

	now->tv_sec = mktime(eft.year, eft.month, eft.day, eft.hour,
			     eft.minute, eft.second);
	now->tv_nsec = 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * Tell the kernel about the EFI memory map.  This might include
 * more than the max 128 entries that can fit in the e820 legacy
 * (zeropage) memory map.
 */

static void __init do_add_efi_memmap(void)
{
	void *p;

	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		efi_memory_desc_t *md = p;
		unsigned long long start = md->phys_addr;
		unsigned long long size = md->num_pages << EFI_PAGE_SHIFT;
		int e820_type;

		switch (md->type) {
		case EFI_LOADER_CODE:
		case EFI_LOADER_DATA:
		case EFI_BOOT_SERVICES_CODE:
		case EFI_BOOT_SERVICES_DATA:
		case EFI_CONVENTIONAL_MEMORY:
			if (md->attribute & EFI_MEMORY_WB)
				e820_type = E820_RAM;
			else
				e820_type = E820_RESERVED;
			break;
		case EFI_ACPI_RECLAIM_MEMORY:
			e820_type = E820_ACPI;
			break;
		case EFI_ACPI_MEMORY_NVS:
			e820_type = E820_NVS;
			break;
		case EFI_UNUSABLE_MEMORY:
			e820_type = E820_UNUSABLE;
			break;
		default:
			/*
			 * EFI_RESERVED_TYPE EFI_RUNTIME_SERVICES_CODE
			 * EFI_RUNTIME_SERVICES_DATA EFI_MEMORY_MAPPED_IO
			 * EFI_MEMORY_MAPPED_IO_PORT_SPACE EFI_PAL_CODE
			 */
			e820_type = E820_RESERVED;
			break;
		}
		e820_add_region(start, size, e820_type);
	}
	sanitize_e820_map(e820.map, ARRAY_SIZE(e820.map), &e820.nr_map);
}

<<<<<<< HEAD
<<<<<<< HEAD
void __init efi_memblock_x86_reserve_range(void)
=======
int __init efi_memblock_x86_reserve_range(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned long pmap;

#ifdef CONFIG_X86_32
<<<<<<< HEAD
=======
	/* Can't handle data above 4GB at this time */
	if (boot_params.efi_info.efi_memmap_hi) {
		pr_err("Memory map is above 4GB, disabling EFI.\n");
		return -EINVAL;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	pmap = boot_params.efi_info.efi_memmap;
#else
	pmap = (boot_params.efi_info.efi_memmap |
		((__u64)boot_params.efi_info.efi_memmap_hi<<32));
#endif
	memmap.phys_map = (void *)pmap;
	memmap.nr_map = boot_params.efi_info.efi_memmap_size /
		boot_params.efi_info.efi_memdesc_size;
	memmap.desc_version = boot_params.efi_info.efi_memdesc_version;
	memmap.desc_size = boot_params.efi_info.efi_memdesc_size;
<<<<<<< HEAD
	memblock_x86_reserve_range(pmap, pmap + memmap.nr_map * memmap.desc_size,
		      "EFI memmap");
=======
	memblock_reserve(pmap, memmap.nr_map * memmap.desc_size);

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

#if EFI_DEBUG
static void __init print_efi_memmap(void)
{
=======
int __init efi_memblock_x86_reserve_range(void)
{
	struct efi_info *e = &boot_params.efi_info;
	unsigned long pmap;

#ifdef CONFIG_X86_32
	/* Can't handle data above 4GB at this time */
	if (e->efi_memmap_hi) {
		pr_err("Memory map is above 4GB, disabling EFI.\n");
		return -EINVAL;
	}
	pmap =  e->efi_memmap;
#else
	pmap = (e->efi_memmap |	((__u64)e->efi_memmap_hi << 32));
#endif
	memmap.phys_map		= (void *)pmap;
	memmap.nr_map		= e->efi_memmap_size /
				  e->efi_memdesc_size;
	memmap.desc_size	= e->efi_memdesc_size;
	memmap.desc_version	= e->efi_memdesc_version;

	memblock_reserve(pmap, memmap.nr_map * memmap.desc_size);

	efi.memmap = &memmap;

	return 0;
}

static void __init print_efi_memmap(void)
{
#ifdef EFI_DEBUG
>>>>>>> refs/remotes/origin/master
	efi_memory_desc_t *md;
	void *p;
	int i;

	for (p = memmap.map, i = 0;
	     p < memmap.map_end;
	     p += memmap.desc_size, i++) {
		md = p;
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_INFO PFX "mem%02u: type=%u, attr=0x%llx, "
=======
		pr_info("mem%02u: type=%u, attr=0x%llx, "
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_info("mem%02u: type=%u, attr=0x%llx, "
>>>>>>> refs/remotes/origin/master
			"range=[0x%016llx-0x%016llx) (%lluMB)\n",
			i, md->type, md->attribute, md->phys_addr,
			md->phys_addr + (md->num_pages << EFI_PAGE_SHIFT),
			(md->num_pages >> (20 - EFI_PAGE_SHIFT)));
	}
<<<<<<< HEAD
}
#endif  /*  EFI_DEBUG  */
=======
#endif  /*  EFI_DEBUG  */
}
>>>>>>> refs/remotes/origin/master

void __init efi_reserve_boot_services(void)
{
	void *p;

	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		efi_memory_desc_t *md = p;
		u64 start = md->phys_addr;
		u64 size = md->num_pages << EFI_PAGE_SHIFT;

		if (md->type != EFI_BOOT_SERVICES_CODE &&
		    md->type != EFI_BOOT_SERVICES_DATA)
			continue;
		/* Only reserve where possible:
		 * - Not within any already allocated areas
		 * - Not over any memory area (really needed, if above?)
		 * - Not within any part of the kernel
		 * - Not the bios reserved area
		*/
<<<<<<< HEAD
		if ((start+size >= virt_to_phys(_text)
				&& start <= virt_to_phys(_end)) ||
			!e820_all_mapped(start, start+size, E820_RAM) ||
<<<<<<< HEAD
			memblock_x86_check_reserved_size(&start, &size,
							1<<EFI_PAGE_SHIFT)) {
			/* Could not reserve, skip it */
			md->num_pages = 0;
			memblock_dbg(PFX "Could not reserve boot range "
					"[0x%010llx-0x%010llx]\n",
						start, start+size-1);
		} else
			memblock_x86_reserve_range(start, start+size,
							"EFI Boot");
	}
}

static void __init efi_free_boot_services(void)
{
	void *p;

=======
=======
		if ((start + size > __pa_symbol(_text)
				&& start <= __pa_symbol(_end)) ||
			!e820_all_mapped(start, start+size, E820_RAM) ||
>>>>>>> refs/remotes/origin/master
			memblock_is_region_reserved(start, size)) {
			/* Could not reserve, skip it */
			md->num_pages = 0;
			memblock_dbg("Could not reserve boot range "
					"[0x%010llx-0x%010llx]\n",
						start, start+size-1);
		} else
			memblock_reserve(start, size);
	}
}

void __init efi_unmap_memmap(void)
{
	clear_bit(EFI_MEMMAP, &x86_efi_facility);
	if (memmap.map) {
		early_iounmap(memmap.map, memmap.nr_map * memmap.desc_size);
		memmap.map = NULL;
	}
}

void __init efi_free_boot_services(void)
{
	void *p;

	if (!efi_is_native())
		return;

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		efi_memory_desc_t *md = p;
		unsigned long long start = md->phys_addr;
		unsigned long long size = md->num_pages << EFI_PAGE_SHIFT;

		if (md->type != EFI_BOOT_SERVICES_CODE &&
		    md->type != EFI_BOOT_SERVICES_DATA)
			continue;

		/* Could not reserve boot area */
		if (!size)
			continue;

		free_bootmem_late(start, size);
	}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
}

void __init efi_init(void)
{
	efi_config_table_t *config_tables;
	efi_runtime_services_t *runtime;
	efi_char16_t *c16;
	char vendor[100] = "unknown";
	int i = 0;
	void *tmp;

#ifdef CONFIG_X86_32
	efi_phys.systab = (efi_system_table_t *)boot_params.efi_info.efi_systab;
#else
	efi_phys.systab = (efi_system_table_t *)
		(boot_params.efi_info.efi_systab |
		 ((__u64)boot_params.efi_info.efi_systab_hi<<32));
#endif

	efi.systab = early_ioremap((unsigned long)efi_phys.systab,
				   sizeof(efi_system_table_t));
	if (efi.systab == NULL)
		printk(KERN_ERR "Couldn't map the EFI system table!\n");
	memcpy(&efi_systab, efi.systab, sizeof(efi_system_table_t));
	early_iounmap(efi.systab, sizeof(efi_system_table_t));
=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	efi_unmap_memmap();
}

static int __init efi_systab_init(void *phys)
{
	if (efi_enabled(EFI_64BIT)) {
		efi_system_table_64_t *systab64;
<<<<<<< HEAD
		u64 tmp = 0;

=======
		struct efi_setup_data *data = NULL;
		u64 tmp = 0;

		if (efi_setup) {
			data = early_memremap(efi_setup, sizeof(*data));
			if (!data)
				return -ENOMEM;
		}
>>>>>>> refs/remotes/origin/master
		systab64 = early_ioremap((unsigned long)phys,
					 sizeof(*systab64));
		if (systab64 == NULL) {
			pr_err("Couldn't map the system table!\n");
<<<<<<< HEAD
=======
			if (data)
				early_iounmap(data, sizeof(*data));
>>>>>>> refs/remotes/origin/master
			return -ENOMEM;
		}

		efi_systab.hdr = systab64->hdr;
<<<<<<< HEAD
		efi_systab.fw_vendor = systab64->fw_vendor;
		tmp |= systab64->fw_vendor;
=======
		efi_systab.fw_vendor = data ? (unsigned long)data->fw_vendor :
					      systab64->fw_vendor;
		tmp |= data ? data->fw_vendor : systab64->fw_vendor;
>>>>>>> refs/remotes/origin/master
		efi_systab.fw_revision = systab64->fw_revision;
		efi_systab.con_in_handle = systab64->con_in_handle;
		tmp |= systab64->con_in_handle;
		efi_systab.con_in = systab64->con_in;
		tmp |= systab64->con_in;
		efi_systab.con_out_handle = systab64->con_out_handle;
		tmp |= systab64->con_out_handle;
		efi_systab.con_out = systab64->con_out;
		tmp |= systab64->con_out;
		efi_systab.stderr_handle = systab64->stderr_handle;
		tmp |= systab64->stderr_handle;
		efi_systab.stderr = systab64->stderr;
		tmp |= systab64->stderr;
<<<<<<< HEAD
		efi_systab.runtime = (void *)(unsigned long)systab64->runtime;
		tmp |= systab64->runtime;
		efi_systab.boottime = (void *)(unsigned long)systab64->boottime;
		tmp |= systab64->boottime;
		efi_systab.nr_tables = systab64->nr_tables;
		efi_systab.tables = systab64->tables;
		tmp |= systab64->tables;

		early_iounmap(systab64, sizeof(*systab64));
=======
		efi_systab.runtime = data ?
				     (void *)(unsigned long)data->runtime :
				     (void *)(unsigned long)systab64->runtime;
		tmp |= data ? data->runtime : systab64->runtime;
		efi_systab.boottime = (void *)(unsigned long)systab64->boottime;
		tmp |= systab64->boottime;
		efi_systab.nr_tables = systab64->nr_tables;
		efi_systab.tables = data ? (unsigned long)data->tables :
					   systab64->tables;
		tmp |= data ? data->tables : systab64->tables;

		early_iounmap(systab64, sizeof(*systab64));
		if (data)
			early_iounmap(data, sizeof(*data));
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_X86_32
		if (tmp >> 32) {
			pr_err("EFI data located above 4GB, disabling EFI.\n");
			return -EINVAL;
		}
#endif
	} else {
		efi_system_table_32_t *systab32;

		systab32 = early_ioremap((unsigned long)phys,
					 sizeof(*systab32));
		if (systab32 == NULL) {
			pr_err("Couldn't map the system table!\n");
			return -ENOMEM;
		}

		efi_systab.hdr = systab32->hdr;
		efi_systab.fw_vendor = systab32->fw_vendor;
		efi_systab.fw_revision = systab32->fw_revision;
		efi_systab.con_in_handle = systab32->con_in_handle;
		efi_systab.con_in = systab32->con_in;
		efi_systab.con_out_handle = systab32->con_out_handle;
		efi_systab.con_out = systab32->con_out;
		efi_systab.stderr_handle = systab32->stderr_handle;
		efi_systab.stderr = systab32->stderr;
		efi_systab.runtime = (void *)(unsigned long)systab32->runtime;
		efi_systab.boottime = (void *)(unsigned long)systab32->boottime;
		efi_systab.nr_tables = systab32->nr_tables;
		efi_systab.tables = systab32->tables;

		early_iounmap(systab32, sizeof(*systab32));
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	efi.systab = &efi_systab;

	/*
	 * Verify the EFI Table
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (efi.systab->hdr.signature != EFI_SYSTEM_TABLE_SIGNATURE)
		printk(KERN_ERR "EFI system table signature incorrect!\n");
	if ((efi.systab->hdr.revision >> 16) == 0)
		printk(KERN_ERR "Warning: EFI system table version "
=======
=======
>>>>>>> refs/remotes/origin/master
	if (efi.systab->hdr.signature != EFI_SYSTEM_TABLE_SIGNATURE) {
		pr_err("System table signature incorrect!\n");
		return -EINVAL;
	}
	if ((efi.systab->hdr.revision >> 16) == 0)
		pr_err("Warning: System table version "
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		       "%d.%02d, expected 1.00 or greater!\n",
		       efi.systab->hdr.revision >> 16,
		       efi.systab->hdr.revision & 0xffff);

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * Show what we know for posterity
	 */
	c16 = tmp = early_ioremap(efi.systab->fw_vendor, 2);
	if (c16) {
		for (i = 0; i < sizeof(vendor) - 1 && *c16; ++i)
			vendor[i] = *c16++;
		vendor[i] = '\0';
	} else
		printk(KERN_ERR PFX "Could not map the firmware vendor!\n");
	early_iounmap(tmp, 2);

	printk(KERN_INFO "EFI v%u.%.02u by %s\n",
	       efi.systab->hdr.revision >> 16,
	       efi.systab->hdr.revision & 0xffff, vendor);
=======
	return 0;
}

static int __init efi_config_init(u64 tables, int nr_tables)
{
	void *config_tables, *tablep;
	int i, sz;

	if (efi_enabled(EFI_64BIT))
		sz = sizeof(efi_config_table_64_t);
	else
		sz = sizeof(efi_config_table_32_t);
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * Let's see what config tables the firmware passed to us.
	 */
<<<<<<< HEAD
	config_tables = early_ioremap(
		efi.systab->tables,
		efi.systab->nr_tables * sizeof(efi_config_table_t));
	if (config_tables == NULL)
		printk(KERN_ERR "Could not map EFI Configuration Table!\n");

	printk(KERN_INFO);
	for (i = 0; i < efi.systab->nr_tables; i++) {
		if (!efi_guidcmp(config_tables[i].guid, MPS_TABLE_GUID)) {
			efi.mps = config_tables[i].table;
			printk(" MPS=0x%lx ", config_tables[i].table);
		} else if (!efi_guidcmp(config_tables[i].guid,
					ACPI_20_TABLE_GUID)) {
			efi.acpi20 = config_tables[i].table;
			printk(" ACPI 2.0=0x%lx ", config_tables[i].table);
		} else if (!efi_guidcmp(config_tables[i].guid,
					ACPI_TABLE_GUID)) {
			efi.acpi = config_tables[i].table;
			printk(" ACPI=0x%lx ", config_tables[i].table);
		} else if (!efi_guidcmp(config_tables[i].guid,
					SMBIOS_TABLE_GUID)) {
			efi.smbios = config_tables[i].table;
			printk(" SMBIOS=0x%lx ", config_tables[i].table);
#ifdef CONFIG_X86_UV
		} else if (!efi_guidcmp(config_tables[i].guid,
					UV_SYSTEM_TABLE_GUID)) {
			efi.uv_systab = config_tables[i].table;
			printk(" UVsystab=0x%lx ", config_tables[i].table);
#endif
		} else if (!efi_guidcmp(config_tables[i].guid,
					HCDP_TABLE_GUID)) {
			efi.hcdp = config_tables[i].table;
			printk(" HCDP=0x%lx ", config_tables[i].table);
		} else if (!efi_guidcmp(config_tables[i].guid,
					UGA_IO_PROTOCOL_GUID)) {
			efi.uga = config_tables[i].table;
			printk(" UGA=0x%lx ", config_tables[i].table);
		}
	}
	printk("\n");
	early_iounmap(config_tables,
			  efi.systab->nr_tables * sizeof(efi_config_table_t));
=======
	config_tables = early_ioremap(tables, nr_tables * sz);
	if (config_tables == NULL) {
		pr_err("Could not map Configuration table!\n");
		return -ENOMEM;
	}

	tablep = config_tables;
	pr_info("");
	for (i = 0; i < efi.systab->nr_tables; i++) {
		efi_guid_t guid;
		unsigned long table;

		if (efi_enabled(EFI_64BIT)) {
			u64 table64;
			guid = ((efi_config_table_64_t *)tablep)->guid;
			table64 = ((efi_config_table_64_t *)tablep)->table;
			table = table64;
#ifdef CONFIG_X86_32
			if (table64 >> 32) {
				pr_cont("\n");
				pr_err("Table located above 4GB, disabling EFI.\n");
				early_iounmap(config_tables,
					      efi.systab->nr_tables * sz);
				return -EINVAL;
			}
#endif
		} else {
			guid = ((efi_config_table_32_t *)tablep)->guid;
			table = ((efi_config_table_32_t *)tablep)->table;
		}
		if (!efi_guidcmp(guid, MPS_TABLE_GUID)) {
			efi.mps = table;
			pr_cont(" MPS=0x%lx ", table);
		} else if (!efi_guidcmp(guid, ACPI_20_TABLE_GUID)) {
			efi.acpi20 = table;
			pr_cont(" ACPI 2.0=0x%lx ", table);
		} else if (!efi_guidcmp(guid, ACPI_TABLE_GUID)) {
			efi.acpi = table;
			pr_cont(" ACPI=0x%lx ", table);
		} else if (!efi_guidcmp(guid, SMBIOS_TABLE_GUID)) {
			efi.smbios = table;
			pr_cont(" SMBIOS=0x%lx ", table);
#ifdef CONFIG_X86_UV
		} else if (!efi_guidcmp(guid, UV_SYSTEM_TABLE_GUID)) {
			efi.uv_systab = table;
			pr_cont(" UVsystab=0x%lx ", table);
#endif
		} else if (!efi_guidcmp(guid, HCDP_TABLE_GUID)) {
			efi.hcdp = table;
			pr_cont(" HCDP=0x%lx ", table);
		} else if (!efi_guidcmp(guid, UGA_IO_PROTOCOL_GUID)) {
			efi.uga = table;
			pr_cont(" UGA=0x%lx ", table);
		}
		tablep += sz;
	}
	pr_cont("\n");
	early_iounmap(config_tables, efi.systab->nr_tables * sz);
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int __init efi_runtime_init(void)
{
	efi_runtime_services_t *runtime;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Check out the runtime services table. We need to map
	 * the runtime services table so that we can grab the physical
	 * address of several of the EFI runtime functions, needed to
	 * set the firmware into virtual mode.
	 */
	runtime = early_ioremap((unsigned long)efi.systab->runtime,
				sizeof(efi_runtime_services_t));
<<<<<<< HEAD
<<<<<<< HEAD
	if (runtime != NULL) {
		/*
		 * We will only need *early* access to the following
		 * two EFI runtime services before set_virtual_address_map
		 * is invoked.
		 */
		efi_phys.get_time = (efi_get_time_t *)runtime->get_time;
		efi_phys.set_virtual_address_map =
			(efi_set_virtual_address_map_t *)
			runtime->set_virtual_address_map;
		/*
		 * Make efi_get_time can be called before entering
		 * virtual mode.
		 */
		efi.get_time = phys_efi_get_time;
	} else
		printk(KERN_ERR "Could not map the EFI runtime service "
		       "table!\n");
	early_iounmap(runtime, sizeof(efi_runtime_services_t));

	/* Map the EFI memory map */
	memmap.map = early_ioremap((unsigned long)memmap.phys_map,
				   memmap.nr_map * memmap.desc_size);
	if (memmap.map == NULL)
		printk(KERN_ERR "Could not map the EFI memory map!\n");
	memmap.map_end = memmap.map + (memmap.nr_map * memmap.desc_size);

	if (memmap.desc_size != sizeof(efi_memory_desc_t))
		printk(KERN_WARNING
		  "Kernel-defined memdesc doesn't match the one from EFI!\n");

	if (add_efi_memmap)
		do_add_efi_memmap();

<<<<<<< HEAD
#ifdef CONFIG_X86_32
	x86_platform.get_wallclock = efi_get_time;
	x86_platform.set_wallclock = efi_set_rtc_mmss;
#endif

=======
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!runtime) {
		pr_err("Could not map the runtime service table!\n");
		return -ENOMEM;
	}
	/*
	 * We will only need *early* access to the following
	 * two EFI runtime services before set_virtual_address_map
	 * is invoked.
	 */
	efi_phys.get_time = (efi_get_time_t *)runtime->get_time;
	efi_phys.set_virtual_address_map =
		(efi_set_virtual_address_map_t *)
		runtime->set_virtual_address_map;
	/*
	 * Make efi_get_time can be called before entering
	 * virtual mode.
	 */
	efi.get_time = phys_efi_get_time;
	early_iounmap(runtime, sizeof(efi_runtime_services_t));

	return 0;
}

static int __init efi_memmap_init(void)
{
	/* Map the EFI memory map */
	memmap.map = early_ioremap((unsigned long)memmap.phys_map,
				   memmap.nr_map * memmap.desc_size);
	if (memmap.map == NULL) {
		pr_err("Could not map the memory map!\n");
		return -ENOMEM;
	}
	memmap.map_end = memmap.map + (memmap.nr_map * memmap.desc_size);

	if (add_efi_memmap)
		do_add_efi_memmap();

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

=======
	return 0;
}

/*
 * A number of config table entries get remapped to virtual addresses
 * after entering EFI virtual mode. However, the kexec kernel requires
 * their physical addresses therefore we pass them via setup_data and
 * correct those entries to their respective physical addresses here.
 *
 * Currently only handles smbios which is necessary for some firmware
 * implementation.
 */
static int __init efi_reuse_config(u64 tables, int nr_tables)
{
	int i, sz, ret = 0;
	void *p, *tablep;
	struct efi_setup_data *data;

	if (!efi_setup)
		return 0;

	if (!efi_enabled(EFI_64BIT))
		return 0;

	data = early_memremap(efi_setup, sizeof(*data));
	if (!data) {
		ret = -ENOMEM;
		goto out;
	}

	if (!data->smbios)
		goto out_memremap;

	sz = sizeof(efi_config_table_64_t);

	p = tablep = early_memremap(tables, nr_tables * sz);
	if (!p) {
		pr_err("Could not map Configuration table!\n");
		ret = -ENOMEM;
		goto out_memremap;
	}

	for (i = 0; i < efi.systab->nr_tables; i++) {
		efi_guid_t guid;

		guid = ((efi_config_table_64_t *)p)->guid;

		if (!efi_guidcmp(guid, SMBIOS_TABLE_GUID))
			((efi_config_table_64_t *)p)->table = data->smbios;
		p += sz;
	}
	early_iounmap(tablep, nr_tables * sz);

out_memremap:
	early_iounmap(data, sizeof(*data));
out:
	return ret;
}

>>>>>>> refs/remotes/origin/master
void __init efi_init(void)
{
	efi_char16_t *c16;
	char vendor[100] = "unknown";
	int i = 0;
	void *tmp;

#ifdef CONFIG_X86_32
	if (boot_params.efi_info.efi_systab_hi ||
	    boot_params.efi_info.efi_memmap_hi) {
		pr_info("Table located above 4GB, disabling EFI.\n");
		return;
	}
	efi_phys.systab = (efi_system_table_t *)boot_params.efi_info.efi_systab;
#else
	efi_phys.systab = (efi_system_table_t *)
			  (boot_params.efi_info.efi_systab |
			  ((__u64)boot_params.efi_info.efi_systab_hi<<32));
#endif

	if (efi_systab_init(efi_phys.systab))
		return;
<<<<<<< HEAD
=======

	set_bit(EFI_SYSTEM_TABLES, &x86_efi_facility);
>>>>>>> refs/remotes/origin/cm-11.0

	set_bit(EFI_SYSTEM_TABLES, &x86_efi_facility);

<<<<<<< HEAD
=======
	efi.config_table = (unsigned long)efi.systab->tables;
	efi.fw_vendor	 = (unsigned long)efi.systab->fw_vendor;
	efi.runtime	 = (unsigned long)efi.systab->runtime;

>>>>>>> refs/remotes/origin/master
	/*
	 * Show what we know for posterity
	 */
	c16 = tmp = early_ioremap(efi.systab->fw_vendor, 2);
	if (c16) {
		for (i = 0; i < sizeof(vendor) - 1 && *c16; ++i)
			vendor[i] = *c16++;
		vendor[i] = '\0';
	} else
		pr_err("Could not map the firmware vendor!\n");
	early_iounmap(tmp, 2);

	pr_info("EFI v%u.%.02u by %s\n",
		efi.systab->hdr.revision >> 16,
		efi.systab->hdr.revision & 0xffff, vendor);

<<<<<<< HEAD
<<<<<<< HEAD
	if (efi_config_init(efi.systab->tables, efi.systab->nr_tables))
=======
	if (efi_reuse_config(efi.systab->tables, efi.systab->nr_tables))
		return;

	if (efi_config_init(arch_tables))
>>>>>>> refs/remotes/origin/master
		return;

=======
	if (efi_config_init(efi.systab->tables, efi.systab->nr_tables))
		return;

>>>>>>> refs/remotes/origin/cm-11.0
	set_bit(EFI_CONFIG_TABLES, &x86_efi_facility);

	/*
	 * Note: We currently don't support runtime services on an EFI
	 * that doesn't match the kernel 32/64-bit mode.
	 */

	if (!efi_is_native())
		pr_info("No EFI runtime due to 32/64-bit mismatch with kernel\n");
	else {
		if (disable_runtime || efi_runtime_init())
			return;
		set_bit(EFI_RUNTIME_SERVICES, &x86_efi_facility);
	}
<<<<<<< HEAD

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (efi_memmap_init())
		return;

	set_bit(EFI_MEMMAP, &x86_efi_facility);
<<<<<<< HEAD

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 15c6df1... Squashed update of kernel from 3.4.74 to 3.4.75
=======
#ifdef CONFIG_X86_32
	if (efi_is_native()) {
		x86_platform.get_wallclock = efi_get_time;
		x86_platform.set_wallclock = efi_set_rtc_mmss;
	}
#endif
=======
>>>>>>> refs/remotes/origin/cm-11.0

>>>>>>> refs/remotes/origin/cm-10.0
#if EFI_DEBUG
	print_efi_memmap();
#endif
=======
	print_efi_memmap();
}

void __init efi_late_init(void)
{
	efi_bgrt_init();
>>>>>>> refs/remotes/origin/master
}

void __init efi_set_executable(efi_memory_desc_t *md, bool executable)
{
	u64 addr, npages;

	addr = md->virt_addr;
	npages = md->num_pages;

	memrange_efi_to_native(&addr, &npages);

	if (executable)
		set_memory_x(addr, npages);
	else
		set_memory_nx(addr, npages);
}

static void __init runtime_code_page_mkexec(void)
{
	efi_memory_desc_t *md;
	void *p;

	/* Make EFI runtime service code area executable */
	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		md = p;

		if (md->type != EFI_RUNTIME_SERVICES_CODE)
			continue;

		efi_set_executable(md, true);
	}
}

<<<<<<< HEAD
/*
 * This function will switch the EFI runtime services to virtual mode.
 * Essentially, look through the EFI memmap and map every region that
 * has the runtime attribute bit set in its memory descriptor and update
 * that memory descriptor with the virtual address obtained from ioremap().
 * This enables the runtime services to be called without having to
 * thunk back into physical mode for every invocation.
 */
void __init efi_enter_virtual_mode(void)
{
	efi_memory_desc_t *md, *prev_md = NULL;
	efi_status_t status;
	unsigned long size;
	u64 end, systab, addr, npages, end_pfn;
	void *p, *va, *new_memmap = NULL;
	int count = 0;

	efi.systab = NULL;

<<<<<<< HEAD
=======
	/*
	 * We don't do virtual mode, since we don't do runtime services, on
	 * non-native EFI
	 */

	if (!efi_is_native()) {
		efi_unmap_memmap();
		return;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	/* Merge contiguous regions of the same type and attribute */
=======
void efi_memory_uc(u64 addr, unsigned long size)
{
	unsigned long page_shift = 1UL << EFI_PAGE_SHIFT;
	u64 npages;

	npages = round_up(size, page_shift) / page_shift;
	memrange_efi_to_native(&addr, &npages);
	set_memory_uc(addr, npages);
}

void __init old_map_region(efi_memory_desc_t *md)
{
	u64 start_pfn, end_pfn, end;
	unsigned long size;
	void *va;

	start_pfn = PFN_DOWN(md->phys_addr);
	size	  = md->num_pages << PAGE_SHIFT;
	end	  = md->phys_addr + size;
	end_pfn   = PFN_UP(end);

	if (pfn_range_is_mapped(start_pfn, end_pfn)) {
		va = __va(md->phys_addr);

		if (!(md->attribute & EFI_MEMORY_WB))
			efi_memory_uc((u64)(unsigned long)va, size);
	} else
		va = efi_ioremap(md->phys_addr, size,
				 md->type, md->attribute);

	md->virt_addr = (u64) (unsigned long) va;
	if (!va)
		pr_err("ioremap of 0x%llX failed!\n",
		       (unsigned long long)md->phys_addr);
}

/* Merge contiguous regions of the same type and attribute */
static void __init efi_merge_regions(void)
{
	void *p;
	efi_memory_desc_t *md, *prev_md = NULL;

>>>>>>> refs/remotes/origin/master
	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		u64 prev_size;
		md = p;

		if (!prev_md) {
			prev_md = md;
			continue;
		}

		if (prev_md->type != md->type ||
		    prev_md->attribute != md->attribute) {
			prev_md = md;
			continue;
		}

		prev_size = prev_md->num_pages << EFI_PAGE_SHIFT;

		if (md->phys_addr == (prev_md->phys_addr + prev_size)) {
			prev_md->num_pages += md->num_pages;
			md->type = EFI_RESERVED_TYPE;
			md->attribute = 0;
			continue;
		}
		prev_md = md;
	}
<<<<<<< HEAD
=======
}

static void __init get_systab_virt_addr(efi_memory_desc_t *md)
{
	unsigned long size;
	u64 end, systab;

	size = md->num_pages << EFI_PAGE_SHIFT;
	end = md->phys_addr + size;
	systab = (u64)(unsigned long)efi_phys.systab;
	if (md->phys_addr <= systab && systab < end) {
		systab += md->virt_addr - md->phys_addr;
		efi.systab = (efi_system_table_t *)(unsigned long)systab;
	}
}

static int __init save_runtime_map(void)
{
	efi_memory_desc_t *md;
	void *tmp, *p, *q = NULL;
	int count = 0;

	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		md = p;
<<<<<<< HEAD

		if (!(md->attribute & EFI_MEMORY_RUNTIME) ||
		    (md->type == EFI_BOOT_SERVICES_CODE) ||
		    (md->type == EFI_BOOT_SERVICES_DATA))
			continue;
		tmp = krealloc(q, (count + 1) * memmap.desc_size, GFP_KERNEL);
		if (!tmp)
			goto out;
		q = tmp;

		memcpy(q + count * memmap.desc_size, md, memmap.desc_size);
		count++;
	}

	efi_runtime_map_setup(q, count, memmap.desc_size);

	return 0;
out:
	kfree(q);
	return -ENOMEM;
}

/*
 * Map efi regions which were passed via setup_data. The virt_addr is a fixed
 * addr which was used in first kernel of a kexec boot.
 */
static void __init efi_map_regions_fixed(void)
{
	void *p;
	efi_memory_desc_t *md;

	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		md = p;
		efi_map_region_fixed(md); /* FIXME: add error handling */
		get_systab_virt_addr(md);
	}

}
=======
		if (!(md->attribute & EFI_MEMORY_RUNTIME)) {
#ifdef CONFIG_X86_64
			if (md->type != EFI_BOOT_SERVICES_CODE &&
			    md->type != EFI_BOOT_SERVICES_DATA)
#endif
				continue;
		}
>>>>>>> refs/remotes/origin/cm-11.0

/*
 * Map efi memory ranges for runtime serivce and update new_memmap with virtual
 * addresses.
 */
static void * __init efi_map_regions(int *count)
{
	efi_memory_desc_t *md;
	void *p, *tmp, *new_memmap = NULL;
>>>>>>> refs/remotes/origin/master

	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		md = p;
		if (!(md->attribute & EFI_MEMORY_RUNTIME)) {
#ifdef CONFIG_X86_64
			if (md->type != EFI_BOOT_SERVICES_CODE &&
			    md->type != EFI_BOOT_SERVICES_DATA)
#endif
				continue;
		}

<<<<<<< HEAD
		size = md->num_pages << EFI_PAGE_SHIFT;
		end = md->phys_addr + size;

		end_pfn = PFN_UP(end);
		if (end_pfn <= max_low_pfn_mapped
		    || (end_pfn > (1UL << (32 - PAGE_SHIFT))
			&& end_pfn <= max_pfn_mapped))
			va = __va(md->phys_addr);
		else
			va = efi_ioremap(md->phys_addr, size, md->type);

		md->virt_addr = (u64) (unsigned long) va;

		if (!va) {
<<<<<<< HEAD
			printk(KERN_ERR PFX "ioremap of 0x%llX failed!\n",
=======
			pr_err("ioremap of 0x%llX failed!\n",
>>>>>>> refs/remotes/origin/cm-10.0
			       (unsigned long long)md->phys_addr);
			continue;
		}

		if (!(md->attribute & EFI_MEMORY_WB)) {
			addr = md->virt_addr;
			npages = md->num_pages;
			memrange_efi_to_native(&addr, &npages);
			set_memory_uc(addr, npages);
		}

		systab = (u64) (unsigned long) efi_phys.systab;
		if (md->phys_addr <= systab && systab < end) {
			systab += md->virt_addr - md->phys_addr;
			efi.systab = (efi_system_table_t *) (unsigned long) systab;
		}
		new_memmap = krealloc(new_memmap,
				      (count + 1) * memmap.desc_size,
				      GFP_KERNEL);
		memcpy(new_memmap + (count * memmap.desc_size), md,
		       memmap.desc_size);
		count++;
	}

	BUG_ON(!efi.systab);

	status = phys_efi_set_virtual_address_map(
		memmap.desc_size * count,
		memmap.desc_size,
		memmap.desc_version,
		(efi_memory_desc_t *)__pa(new_memmap));

	if (status != EFI_SUCCESS) {
<<<<<<< HEAD
		printk(KERN_ALERT "Unable to switch EFI into virtual mode "
		       "(status=%lx)!\n", status);
=======
		pr_alert("Unable to switch EFI into virtual mode "
			 "(status=%lx)!\n", status);
>>>>>>> refs/remotes/origin/cm-10.0
		panic("EFI call to SetVirtualAddressMap() failed!");
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	 * Thankfully, it does seem that no runtime services other than
	 * SetVirtualAddressMap() will touch boot services code, so we can
	 * get rid of it all at this point
	 */
	efi_free_boot_services();

	/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		efi_map_region(md);
		get_systab_virt_addr(md);

		tmp = krealloc(new_memmap, (*count + 1) * memmap.desc_size,
			       GFP_KERNEL);
		if (!tmp)
			goto out;
		new_memmap = tmp;
		memcpy(new_memmap + (*count * memmap.desc_size), md,
		       memmap.desc_size);
		(*count)++;
	}

	return new_memmap;
out:
	kfree(new_memmap);
	return NULL;
}

/*
 * This function will switch the EFI runtime services to virtual mode.
 * Essentially, we look through the EFI memmap and map every region that
 * has the runtime attribute bit set in its memory descriptor into the
 * ->trampoline_pgd page table using a top-down VA allocation scheme.
 *
 * The old method which used to update that memory descriptor with the
 * virtual address obtained from ioremap() is still supported when the
 * kernel is booted with efi=old_map on its command line. Same old
 * method enabled the runtime services to be called without having to
 * thunk back into physical mode for every invocation.
 *
 * The new method does a pagetable switch in a preemption-safe manner
 * so that we're in a different address space when calling a runtime
 * function. For function arguments passing we do copy the PGDs of the
 * kernel page table into ->trampoline_pgd prior to each call.
 *
 * Specially for kexec boot, efi runtime maps in previous kernel should
 * be passed in via setup_data. In that case runtime ranges will be mapped
 * to the same virtual addresses as the first kernel.
 */
void __init efi_enter_virtual_mode(void)
{
	efi_status_t status;
	void *new_memmap = NULL;
	int err, count = 0;

	efi.systab = NULL;

	/*
	 * We don't do virtual mode, since we don't do runtime services, on
	 * non-native EFI
	 */
	if (!efi_is_native()) {
		efi_unmap_memmap();
		return;
	}

	if (efi_setup) {
		efi_map_regions_fixed();
	} else {
		efi_merge_regions();
		new_memmap = efi_map_regions(&count);
		if (!new_memmap) {
			pr_err("Error reallocating memory, EFI runtime non-functional!\n");
			return;
		}
	}

	err = save_runtime_map();
	if (err)
		pr_err("Error saving runtime map, efi runtime on kexec non-functional!!\n");

	BUG_ON(!efi.systab);

	efi_setup_page_tables();
	efi_sync_low_kernel_mappings();

	if (!efi_setup) {
		status = phys_efi_set_virtual_address_map(
			memmap.desc_size * count,
			memmap.desc_size,
			memmap.desc_version,
			(efi_memory_desc_t *)__pa(new_memmap));

		if (status != EFI_SUCCESS) {
			pr_alert("Unable to switch EFI into virtual mode (status=%lx)!\n",
				 status);
			panic("EFI call to SetVirtualAddressMap() failed!");
		}
	}

	/*
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	 * Now that EFI is in virtual mode, update the function
	 * pointers in the runtime service table to the new virtual addresses.
	 *
	 * Call EFI services through wrapper functions.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	efi.runtime_version = efi_systab.hdr.revision;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	efi.runtime_version = efi_systab.hdr.revision;
>>>>>>> refs/remotes/origin/master
=======
	efi.runtime_version = efi_systab.hdr.revision;
>>>>>>> refs/remotes/origin/cm-11.0
	efi.get_time = virt_efi_get_time;
	efi.set_time = virt_efi_set_time;
	efi.get_wakeup_time = virt_efi_get_wakeup_time;
	efi.set_wakeup_time = virt_efi_set_wakeup_time;
	efi.get_variable = virt_efi_get_variable;
	efi.get_next_variable = virt_efi_get_next_variable;
	efi.set_variable = virt_efi_set_variable;
	efi.get_next_high_mono_count = virt_efi_get_next_high_mono_count;
	efi.reset_system = virt_efi_reset_system;
	efi.set_virtual_address_map = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	if (__supported_pte_mask & _PAGE_NX)
		runtime_code_page_mkexec();
	early_iounmap(memmap.map, memmap.nr_map * memmap.desc_size);
	memmap.map = NULL;
=======
	efi.query_variable_info = virt_efi_query_variable_info;
	efi.update_capsule = virt_efi_update_capsule;
	efi.query_capsule_caps = virt_efi_query_capsule_caps;
	if (__supported_pte_mask & _PAGE_NX)
		runtime_code_page_mkexec();

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(new_memmap);
=======
	efi.query_variable_info = virt_efi_query_variable_info;
	efi.update_capsule = virt_efi_update_capsule;
	efi.query_capsule_caps = virt_efi_query_capsule_caps;

	if (efi_enabled(EFI_OLD_MEMMAP) && (__supported_pte_mask & _PAGE_NX))
		runtime_code_page_mkexec();

=======
>>>>>>> refs/remotes/origin/cm-11.0
	kfree(new_memmap);

	/* clean DUMMY object */
	efi.set_variable(efi_dummy_name, &EFI_DUMMY_GUID,
			 EFI_VARIABLE_NON_VOLATILE |
			 EFI_VARIABLE_BOOTSERVICE_ACCESS |
			 EFI_VARIABLE_RUNTIME_ACCESS,
			 0, NULL);
>>>>>>> refs/remotes/origin/master
}

/*
 * Convenience functions to obtain memory types and attributes
 */
u32 efi_mem_type(unsigned long phys_addr)
{
	efi_memory_desc_t *md;
	void *p;

<<<<<<< HEAD
=======
	if (!efi_enabled(EFI_MEMMAP))
		return 0;

>>>>>>> refs/remotes/origin/master
	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		md = p;
		if ((md->phys_addr <= phys_addr) &&
		    (phys_addr < (md->phys_addr +
				  (md->num_pages << EFI_PAGE_SHIFT))))
			return md->type;
	}
	return 0;
}

u64 efi_mem_attributes(unsigned long phys_addr)
{
	efi_memory_desc_t *md;
	void *p;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (!efi_enabled(EFI_MEMMAP))
		return 0;

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	if (!efi_enabled(EFI_MEMMAP))
		return 0;

>>>>>>> refs/remotes/origin/cm-11.0
	for (p = memmap.map; p < memmap.map_end; p += memmap.desc_size) {
		md = p;
		if ((md->phys_addr <= phys_addr) &&
		    (phys_addr < (md->phys_addr +
				  (md->num_pages << EFI_PAGE_SHIFT))))
			return md->attribute;
	}
	return 0;
}
<<<<<<< HEAD
=======

/*
 * Some firmware has serious problems when using more than 50% of the EFI
 * variable store, i.e. it triggers bugs that can brick machines. Ensure that
 * we never use more than this safe limit.
 *
 * Return EFI_SUCCESS if it is safe to write 'size' bytes to the variable
 * store.
 */
efi_status_t efi_query_variable_store(u32 attributes, unsigned long size)
{
	efi_status_t status;
	u64 storage_size, remaining_size, max_size;

	if (!(attributes & EFI_VARIABLE_NON_VOLATILE))
		return 0;

	status = efi.query_variable_info(attributes, &storage_size,
					 &remaining_size, &max_size);
	if (status != EFI_SUCCESS)
		return status;

	/*
	 * Some firmware implementations refuse to boot if there's insufficient
	 * space in the variable store. We account for that by refusing the
	 * write if permitting it would reduce the available space to under
	 * 5KB. This figure was provided by Samsung, so should be safe.
	 */
	if ((remaining_size - size < EFI_MIN_RESERVE) &&
		!efi_no_storage_paranoia) {

		/*
		 * Triggering garbage collection may require that the firmware
		 * generate a real EFI_OUT_OF_RESOURCES error. We can force
		 * that by attempting to use more space than is available.
		 */
		unsigned long dummy_size = remaining_size + 1024;
		void *dummy = kzalloc(dummy_size, GFP_ATOMIC);

		if (!dummy)
			return EFI_OUT_OF_RESOURCES;

		status = efi.set_variable(efi_dummy_name, &EFI_DUMMY_GUID,
					  EFI_VARIABLE_NON_VOLATILE |
					  EFI_VARIABLE_BOOTSERVICE_ACCESS |
					  EFI_VARIABLE_RUNTIME_ACCESS,
					  dummy_size, dummy);

		if (status == EFI_SUCCESS) {
			/*
			 * This should have failed, so if it didn't make sure
			 * that we delete it...
			 */
			efi.set_variable(efi_dummy_name, &EFI_DUMMY_GUID,
					 EFI_VARIABLE_NON_VOLATILE |
					 EFI_VARIABLE_BOOTSERVICE_ACCESS |
					 EFI_VARIABLE_RUNTIME_ACCESS,
					 0, dummy);
		}

		kfree(dummy);

		/*
		 * The runtime code may now have triggered a garbage collection
		 * run, so check the variable info again
		 */
		status = efi.query_variable_info(attributes, &storage_size,
						 &remaining_size, &max_size);

		if (status != EFI_SUCCESS)
			return status;

		/*
		 * There still isn't enough room, so return an error
		 */
		if (remaining_size - size < EFI_MIN_RESERVE)
			return EFI_OUT_OF_RESOURCES;
	}

	return EFI_SUCCESS;
}
EXPORT_SYMBOL_GPL(efi_query_variable_store);

static int __init parse_efi_cmdline(char *str)
{
	if (*str == '=')
		str++;

	if (!strncmp(str, "old_map", 7))
		set_bit(EFI_OLD_MEMMAP, &x86_efi_facility);

	return 0;
}
early_param("efi", parse_efi_cmdline);
>>>>>>> refs/remotes/origin/master
