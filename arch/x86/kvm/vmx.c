/*
 * Kernel-based Virtual Machine driver for Linux
 *
 * This module enables machines with Intel VT-x extensions to run virtual
 * machines without emulation or binary translation.
 *
 * Copyright (C) 2006 Qumranet, Inc.
 * Copyright 2010 Red Hat, Inc. and/or its affiliates.
 *
 * Authors:
 *   Avi Kivity   <avi@qumranet.com>
 *   Yaniv Kamay  <yaniv@qumranet.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 */

#include "irq.h"
#include "mmu.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "cpuid.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "cpuid.h"
>>>>>>> refs/remotes/origin/master

#include <linux/kvm_host.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
<<<<<<< HEAD
=======
#include <linux/mod_devicetable.h>
>>>>>>> refs/remotes/origin/master
#include <linux/ftrace_event.h>
#include <linux/slab.h>
#include <linux/tboot.h>
#include "kvm_cache_regs.h"
#include "x86.h"

#include <asm/io.h>
#include <asm/desc.h>
#include <asm/vmx.h>
#include <asm/virtext.h>
#include <asm/mce.h>
#include <asm/i387.h>
#include <asm/xcr.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/perf_event.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/perf_event.h>
#include <asm/kexec.h>
>>>>>>> refs/remotes/origin/master

#include "trace.h"

#define __ex(x) __kvm_handle_fault_on_reboot(x)
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define __ex_clear(x, reg) \
	____kvm_handle_fault_on_reboot(x, "xor " reg " , " reg)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define __ex_clear(x, reg) \
	____kvm_handle_fault_on_reboot(x, "xor " reg " , " reg)
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Qumranet");
MODULE_LICENSE("GPL");

<<<<<<< HEAD
<<<<<<< HEAD
static int __read_mostly bypass_guest_pf = 1;
module_param(bypass_guest_pf, bool, S_IRUGO);

static int __read_mostly enable_vpid = 1;
module_param_named(vpid, enable_vpid, bool, 0444);

static int __read_mostly flexpriority_enabled = 1;
module_param_named(flexpriority, flexpriority_enabled, bool, S_IRUGO);

static int __read_mostly enable_ept = 1;
module_param_named(ept, enable_ept, bool, S_IRUGO);

static int __read_mostly enable_unrestricted_guest = 1;
module_param_named(unrestricted_guest,
			enable_unrestricted_guest, bool, S_IRUGO);

static int __read_mostly emulate_invalid_guest_state = 0;
module_param(emulate_invalid_guest_state, bool, S_IRUGO);

static int __read_mostly vmm_exclusive = 1;
module_param(vmm_exclusive, bool, S_IRUGO);

static int __read_mostly yield_on_hlt = 1;
module_param(yield_on_hlt, bool, S_IRUGO);
=======
=======
static const struct x86_cpu_id vmx_cpu_id[] = {
	X86_FEATURE_MATCH(X86_FEATURE_VMX),
	{}
};
MODULE_DEVICE_TABLE(x86cpu, vmx_cpu_id);

>>>>>>> refs/remotes/origin/master
static bool __read_mostly enable_vpid = 1;
module_param_named(vpid, enable_vpid, bool, 0444);

static bool __read_mostly flexpriority_enabled = 1;
module_param_named(flexpriority, flexpriority_enabled, bool, S_IRUGO);

static bool __read_mostly enable_ept = 1;
module_param_named(ept, enable_ept, bool, S_IRUGO);

static bool __read_mostly enable_unrestricted_guest = 1;
module_param_named(unrestricted_guest,
			enable_unrestricted_guest, bool, S_IRUGO);

<<<<<<< HEAD
static bool __read_mostly emulate_invalid_guest_state = 0;
=======
static bool __read_mostly enable_ept_ad_bits = 1;
module_param_named(eptad, enable_ept_ad_bits, bool, S_IRUGO);

static bool __read_mostly emulate_invalid_guest_state = true;
>>>>>>> refs/remotes/origin/master
module_param(emulate_invalid_guest_state, bool, S_IRUGO);

static bool __read_mostly vmm_exclusive = 1;
module_param(vmm_exclusive, bool, S_IRUGO);

static bool __read_mostly fasteoi = 1;
module_param(fasteoi, bool, S_IRUGO);

<<<<<<< HEAD
=======
static bool __read_mostly enable_apicv = 1;
module_param(enable_apicv, bool, S_IRUGO);

static bool __read_mostly enable_shadow_vmcs = 1;
module_param_named(enable_shadow_vmcs, enable_shadow_vmcs, bool, S_IRUGO);
>>>>>>> refs/remotes/origin/master
/*
 * If nested=1, nested virtualization is supported, i.e., guests may use
 * VMX and be a hypervisor for its own guests. If nested=0, guests may not
 * use VMX instructions.
 */
static bool __read_mostly nested = 0;
module_param(nested, bool, S_IRUGO);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#define KVM_GUEST_CR0_MASK_UNRESTRICTED_GUEST				\
	(X86_CR0_WP | X86_CR0_NE | X86_CR0_NW | X86_CR0_CD)
#define KVM_GUEST_CR0_MASK						\
	(KVM_GUEST_CR0_MASK_UNRESTRICTED_GUEST | X86_CR0_PG | X86_CR0_PE)
#define KVM_VM_CR0_ALWAYS_ON_UNRESTRICTED_GUEST				\
	(X86_CR0_WP | X86_CR0_NE)
=======

#define KVM_GUEST_CR0_MASK (X86_CR0_NW | X86_CR0_CD)
#define KVM_VM_CR0_ALWAYS_ON_UNRESTRICTED_GUEST (X86_CR0_WP | X86_CR0_NE)
>>>>>>> refs/remotes/origin/master
#define KVM_VM_CR0_ALWAYS_ON						\
	(KVM_VM_CR0_ALWAYS_ON_UNRESTRICTED_GUEST | X86_CR0_PG | X86_CR0_PE)
#define KVM_CR4_GUEST_OWNED_BITS				      \
	(X86_CR4_PVI | X86_CR4_DE | X86_CR4_PCE | X86_CR4_OSFXSR      \
	 | X86_CR4_OSXMMEXCPT)

#define KVM_PMODE_VM_CR4_ALWAYS_ON (X86_CR4_PAE | X86_CR4_VMXE)
#define KVM_RMODE_VM_CR4_ALWAYS_ON (X86_CR4_VME | X86_CR4_PAE | X86_CR4_VMXE)

#define RMODE_GUEST_OWNED_EFLAGS_BITS (~(X86_EFLAGS_IOPL | X86_EFLAGS_VM))

/*
 * These 2 parameters are used to config the controls for Pause-Loop Exiting:
 * ple_gap:    upper bound on the amount of time between two successive
 *             executions of PAUSE in a loop. Also indicate if ple enabled.
 *             According to test, this time is usually smaller than 128 cycles.
 * ple_window: upper bound on the amount of time a guest is allowed to execute
 *             in a PAUSE loop. Tests indicate that most spinlocks are held for
 *             less than 2^12 cycles
 * Time is measured based on a counter that runs at the same rate as the TSC,
 * refer SDM volume 3b section 21.6.13 & 22.1.3.
 */
#define KVM_VMX_DEFAULT_PLE_GAP    128
#define KVM_VMX_DEFAULT_PLE_WINDOW 4096
static int ple_gap = KVM_VMX_DEFAULT_PLE_GAP;
module_param(ple_gap, int, S_IRUGO);

static int ple_window = KVM_VMX_DEFAULT_PLE_WINDOW;
module_param(ple_window, int, S_IRUGO);

<<<<<<< HEAD
<<<<<<< HEAD
#define NR_AUTOLOAD_MSRS 1
=======
#define NR_AUTOLOAD_MSRS 8
#define VMCS02_POOL_SIZE 1
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern const ulong vmx_return;

#define NR_AUTOLOAD_MSRS 8
#define VMCS02_POOL_SIZE 1
>>>>>>> refs/remotes/origin/master

struct vmcs {
	u32 revision_id;
	u32 abort;
	char data[0];
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Track a VMCS that may be loaded on a certain CPU. If it is (cpu!=-1), also
 * remember whether it was VMLAUNCHed, and maintain a linked list of all VMCSs
 * loaded on this CPU (so we can clear them if the CPU goes down).
 */
struct loaded_vmcs {
	struct vmcs *vmcs;
	int cpu;
	int launched;
	struct list_head loaded_vmcss_on_cpu_link;
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct shared_msr_entry {
	unsigned index;
	u64 data;
	u64 mask;
};

<<<<<<< HEAD
<<<<<<< HEAD
struct vcpu_vmx {
	struct kvm_vcpu       vcpu;
	struct list_head      local_vcpus_link;
	unsigned long         host_rsp;
	int                   launched;
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * struct vmcs12 describes the state that our guest hypervisor (L1) keeps for a
 * single nested guest (L2), hence the name vmcs12. Any VMX implementation has
 * a VMCS structure, and vmcs12 is our emulated VMX's VMCS. This structure is
 * stored in guest memory specified by VMPTRLD, but is opaque to the guest,
 * which must access it using VMREAD/VMWRITE/VMCLEAR instructions.
 * More than one of these structures may exist, if L1 runs multiple L2 guests.
 * nested_vmx_run() will use the data here to build a vmcs02: a VMCS for the
 * underlying hardware which will be used to run L2.
 * This structure is packed to ensure that its layout is identical across
 * machines (necessary for live migration).
 * If there are changes in this struct, VMCS12_REVISION must be changed.
 */
typedef u64 natural_width;
struct __packed vmcs12 {
	/* According to the Intel spec, a VMCS region must start with the
	 * following two fields. Then follow implementation-specific data.
	 */
	u32 revision_id;
	u32 abort;

	u32 launch_state; /* set to 0 by VMCLEAR, to 1 by VMLAUNCH */
	u32 padding[7]; /* room for future expansion */

	u64 io_bitmap_a;
	u64 io_bitmap_b;
	u64 msr_bitmap;
	u64 vm_exit_msr_store_addr;
	u64 vm_exit_msr_load_addr;
	u64 vm_entry_msr_load_addr;
	u64 tsc_offset;
	u64 virtual_apic_page_addr;
	u64 apic_access_addr;
	u64 ept_pointer;
	u64 guest_physical_address;
	u64 vmcs_link_pointer;
	u64 guest_ia32_debugctl;
	u64 guest_ia32_pat;
	u64 guest_ia32_efer;
	u64 guest_ia32_perf_global_ctrl;
	u64 guest_pdptr0;
	u64 guest_pdptr1;
	u64 guest_pdptr2;
	u64 guest_pdptr3;
	u64 host_ia32_pat;
	u64 host_ia32_efer;
	u64 host_ia32_perf_global_ctrl;
	u64 padding64[8]; /* room for future expansion */
	/*
	 * To allow migration of L1 (complete with its L2 guests) between
	 * machines of different natural widths (32 or 64 bit), we cannot have
	 * unsigned long fields with no explict size. We use u64 (aliased
	 * natural_width) instead. Luckily, x86 is little-endian.
	 */
	natural_width cr0_guest_host_mask;
	natural_width cr4_guest_host_mask;
	natural_width cr0_read_shadow;
	natural_width cr4_read_shadow;
	natural_width cr3_target_value0;
	natural_width cr3_target_value1;
	natural_width cr3_target_value2;
	natural_width cr3_target_value3;
	natural_width exit_qualification;
	natural_width guest_linear_address;
	natural_width guest_cr0;
	natural_width guest_cr3;
	natural_width guest_cr4;
	natural_width guest_es_base;
	natural_width guest_cs_base;
	natural_width guest_ss_base;
	natural_width guest_ds_base;
	natural_width guest_fs_base;
	natural_width guest_gs_base;
	natural_width guest_ldtr_base;
	natural_width guest_tr_base;
	natural_width guest_gdtr_base;
	natural_width guest_idtr_base;
	natural_width guest_dr7;
	natural_width guest_rsp;
	natural_width guest_rip;
	natural_width guest_rflags;
	natural_width guest_pending_dbg_exceptions;
	natural_width guest_sysenter_esp;
	natural_width guest_sysenter_eip;
	natural_width host_cr0;
	natural_width host_cr3;
	natural_width host_cr4;
	natural_width host_fs_base;
	natural_width host_gs_base;
	natural_width host_tr_base;
	natural_width host_gdtr_base;
	natural_width host_idtr_base;
	natural_width host_ia32_sysenter_esp;
	natural_width host_ia32_sysenter_eip;
	natural_width host_rsp;
	natural_width host_rip;
	natural_width paddingl[8]; /* room for future expansion */
	u32 pin_based_vm_exec_control;
	u32 cpu_based_vm_exec_control;
	u32 exception_bitmap;
	u32 page_fault_error_code_mask;
	u32 page_fault_error_code_match;
	u32 cr3_target_count;
	u32 vm_exit_controls;
	u32 vm_exit_msr_store_count;
	u32 vm_exit_msr_load_count;
	u32 vm_entry_controls;
	u32 vm_entry_msr_load_count;
	u32 vm_entry_intr_info_field;
	u32 vm_entry_exception_error_code;
	u32 vm_entry_instruction_len;
	u32 tpr_threshold;
	u32 secondary_vm_exec_control;
	u32 vm_instruction_error;
	u32 vm_exit_reason;
	u32 vm_exit_intr_info;
	u32 vm_exit_intr_error_code;
	u32 idt_vectoring_info_field;
	u32 idt_vectoring_error_code;
	u32 vm_exit_instruction_len;
	u32 vmx_instruction_info;
	u32 guest_es_limit;
	u32 guest_cs_limit;
	u32 guest_ss_limit;
	u32 guest_ds_limit;
	u32 guest_fs_limit;
	u32 guest_gs_limit;
	u32 guest_ldtr_limit;
	u32 guest_tr_limit;
	u32 guest_gdtr_limit;
	u32 guest_idtr_limit;
	u32 guest_es_ar_bytes;
	u32 guest_cs_ar_bytes;
	u32 guest_ss_ar_bytes;
	u32 guest_ds_ar_bytes;
	u32 guest_fs_ar_bytes;
	u32 guest_gs_ar_bytes;
	u32 guest_ldtr_ar_bytes;
	u32 guest_tr_ar_bytes;
	u32 guest_interruptibility_info;
	u32 guest_activity_state;
	u32 guest_sysenter_cs;
	u32 host_ia32_sysenter_cs;
<<<<<<< HEAD
	u32 padding32[8]; /* room for future expansion */
=======
	u32 vmx_preemption_timer_value;
	u32 padding32[7]; /* room for future expansion */
>>>>>>> refs/remotes/origin/master
	u16 virtual_processor_id;
	u16 guest_es_selector;
	u16 guest_cs_selector;
	u16 guest_ss_selector;
	u16 guest_ds_selector;
	u16 guest_fs_selector;
	u16 guest_gs_selector;
	u16 guest_ldtr_selector;
	u16 guest_tr_selector;
	u16 host_es_selector;
	u16 host_cs_selector;
	u16 host_ss_selector;
	u16 host_ds_selector;
	u16 host_fs_selector;
	u16 host_gs_selector;
	u16 host_tr_selector;
};

/*
 * VMCS12_REVISION is an arbitrary id that should be changed if the content or
 * layout of struct vmcs12 is changed. MSR_IA32_VMX_BASIC returns this id, and
 * VMPTRLD verifies that the VMCS region that L1 is loading contains this id.
 */
#define VMCS12_REVISION 0x11e57ed0

/*
 * VMCS12_SIZE is the number of bytes L1 should allocate for the VMXON region
 * and any VMCS region. Although only sizeof(struct vmcs12) are used by the
 * current implementation, 4K are reserved to avoid future complications.
 */
#define VMCS12_SIZE 0x1000

/* Used to remember the last vmcs02 used for some recently used vmcs12s */
struct vmcs02_list {
	struct list_head list;
	gpa_t vmptr;
	struct loaded_vmcs vmcs02;
};

/*
 * The nested_vmx structure is part of vcpu_vmx, and holds information we need
 * for correct emulation of VMX (i.e., nested VMX) on this vcpu.
 */
struct nested_vmx {
	/* Has the level1 guest done vmxon? */
	bool vmxon;

	/* The guest-physical address of the current VMCS L1 keeps for L2 */
	gpa_t current_vmptr;
	/* The host-usable pointer to the above */
	struct page *current_vmcs12_page;
	struct vmcs12 *current_vmcs12;
<<<<<<< HEAD
=======
	struct vmcs *current_shadow_vmcs;
	/*
	 * Indicates if the shadow vmcs must be updated with the
	 * data hold by vmcs12
	 */
	bool sync_shadow_vmcs;
>>>>>>> refs/remotes/origin/master

	/* vmcs02_list cache of VMCSs recently used to run L2 guests */
	struct list_head vmcs02_pool;
	int vmcs02_num;
	u64 vmcs01_tsc_offset;
	/* L2 must run next, and mustn't decide to exit to L1. */
	bool nested_run_pending;
	/*
	 * Guest pages referred to in vmcs02 with host-physical pointers, so
	 * we must keep them pinned while L2 runs.
	 */
	struct page *apic_access_page;
<<<<<<< HEAD
};

struct vcpu_vmx {
	struct kvm_vcpu       vcpu;
	unsigned long         host_rsp;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u64 msr_ia32_feature_control;
};

#define POSTED_INTR_ON  0
/* Posted-Interrupt Descriptor */
struct pi_desc {
	u32 pir[8];     /* Posted interrupt requested */
	u32 control;	/* bit 0 of control is outstanding notification bit */
	u32 rsvd[7];
} __aligned(64);

static bool pi_test_and_set_on(struct pi_desc *pi_desc)
{
	return test_and_set_bit(POSTED_INTR_ON,
			(unsigned long *)&pi_desc->control);
}

static bool pi_test_and_clear_on(struct pi_desc *pi_desc)
{
	return test_and_clear_bit(POSTED_INTR_ON,
			(unsigned long *)&pi_desc->control);
}

static int pi_test_and_set_pir(int vector, struct pi_desc *pi_desc)
{
	return test_and_set_bit(vector, (unsigned long *)pi_desc->pir);
}

struct vcpu_vmx {
	struct kvm_vcpu       vcpu;
	unsigned long         host_rsp;
>>>>>>> refs/remotes/origin/master
	u8                    fail;
	u8                    cpl;
	bool                  nmi_known_unmasked;
	u32                   exit_intr_info;
	u32                   idt_vectoring_info;
	ulong                 rflags;
	struct shared_msr_entry *guest_msrs;
	int                   nmsrs;
	int                   save_nmsrs;
<<<<<<< HEAD
=======
	unsigned long	      host_idt_base;
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_X86_64
	u64 		      msr_host_kernel_gs_base;
	u64 		      msr_guest_kernel_gs_base;
#endif
<<<<<<< HEAD
<<<<<<< HEAD
	struct vmcs          *vmcs;
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * loaded_vmcs points to the VMCS currently used in this vcpu. For a
	 * non-nested (L1) guest, it always points to vmcs01. For a nested
	 * guest (L2), it points to a different VMCS.
	 */
	struct loaded_vmcs    vmcs01;
	struct loaded_vmcs   *loaded_vmcs;
	bool                  __launched; /* temporary, used in vmx_vcpu_run */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct msr_autoload {
		unsigned nr;
		struct vmx_msr_entry guest[NR_AUTOLOAD_MSRS];
		struct vmx_msr_entry host[NR_AUTOLOAD_MSRS];
	} msr_autoload;
	struct {
		int           loaded;
		u16           fs_sel, gs_sel, ldt_sel;
<<<<<<< HEAD
=======
#ifdef CONFIG_X86_64
		u16           ds_sel, es_sel;
#endif
>>>>>>> refs/remotes/origin/master
		int           gs_ldt_reload_needed;
		int           fs_reload_needed;
	} host_state;
	struct {
		int vm86_active;
		ulong save_rflags;
<<<<<<< HEAD
=======
		struct kvm_segment segs[8];
	} rmode;
	struct {
		u32 bitmask; /* 4 bits per segment (1 bit per field) */
>>>>>>> refs/remotes/origin/master
		struct kvm_save_segment {
			u16 selector;
			unsigned long base;
			u32 limit;
			u32 ar;
<<<<<<< HEAD
		} tr, es, ds, fs, gs;
	} rmode;
	struct {
		u32 bitmask; /* 4 bits per segment (1 bit per field) */
		struct kvm_save_segment seg[8];
=======
		} seg[8];
>>>>>>> refs/remotes/origin/master
	} segment_cache;
	int vpid;
	bool emulation_required;

	/* Support for vnmi-less CPUs */
	int soft_vnmi_blocked;
	ktime_t entry_time;
	s64 vnmi_blocked_time;
	u32 exit_reason;

	bool rdtscp_enabled;
<<<<<<< HEAD
<<<<<<< HEAD
=======

	/* Support for a guest hypervisor (nested VMX) */
	struct nested_vmx nested;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	/* Posted interrupt descriptor */
	struct pi_desc pi_desc;

	/* Support for a guest hypervisor (nested VMX) */
	struct nested_vmx nested;
>>>>>>> refs/remotes/origin/master
};

enum segment_cache_field {
	SEG_FIELD_SEL = 0,
	SEG_FIELD_BASE = 1,
	SEG_FIELD_LIMIT = 2,
	SEG_FIELD_AR = 3,

	SEG_FIELD_NR = 4
};

static inline struct vcpu_vmx *to_vmx(struct kvm_vcpu *vcpu)
{
	return container_of(vcpu, struct vcpu_vmx, vcpu);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#define VMCS12_OFFSET(x) offsetof(struct vmcs12, x)
#define FIELD(number, name)	[number] = VMCS12_OFFSET(name)
#define FIELD64(number, name)	[number] = VMCS12_OFFSET(name), \
				[number##_HIGH] = VMCS12_OFFSET(name)+4

<<<<<<< HEAD
static unsigned short vmcs_field_to_offset_table[] = {
=======

static const unsigned long shadow_read_only_fields[] = {
	/*
	 * We do NOT shadow fields that are modified when L0
	 * traps and emulates any vmx instruction (e.g. VMPTRLD,
	 * VMXON...) executed by L1.
	 * For example, VM_INSTRUCTION_ERROR is read
	 * by L1 if a vmx instruction fails (part of the error path).
	 * Note the code assumes this logic. If for some reason
	 * we start shadowing these fields then we need to
	 * force a shadow sync when L0 emulates vmx instructions
	 * (e.g. force a sync if VM_INSTRUCTION_ERROR is modified
	 * by nested_vmx_failValid)
	 */
	VM_EXIT_REASON,
	VM_EXIT_INTR_INFO,
	VM_EXIT_INSTRUCTION_LEN,
	IDT_VECTORING_INFO_FIELD,
	IDT_VECTORING_ERROR_CODE,
	VM_EXIT_INTR_ERROR_CODE,
	EXIT_QUALIFICATION,
	GUEST_LINEAR_ADDRESS,
	GUEST_PHYSICAL_ADDRESS
};
static const int max_shadow_read_only_fields =
	ARRAY_SIZE(shadow_read_only_fields);

static const unsigned long shadow_read_write_fields[] = {
	GUEST_RIP,
	GUEST_RSP,
	GUEST_CR0,
	GUEST_CR3,
	GUEST_CR4,
	GUEST_INTERRUPTIBILITY_INFO,
	GUEST_RFLAGS,
	GUEST_CS_SELECTOR,
	GUEST_CS_AR_BYTES,
	GUEST_CS_LIMIT,
	GUEST_CS_BASE,
	GUEST_ES_BASE,
	CR0_GUEST_HOST_MASK,
	CR0_READ_SHADOW,
	CR4_READ_SHADOW,
	TSC_OFFSET,
	EXCEPTION_BITMAP,
	CPU_BASED_VM_EXEC_CONTROL,
	VM_ENTRY_EXCEPTION_ERROR_CODE,
	VM_ENTRY_INTR_INFO_FIELD,
	VM_ENTRY_INSTRUCTION_LEN,
	VM_ENTRY_EXCEPTION_ERROR_CODE,
	HOST_FS_BASE,
	HOST_GS_BASE,
	HOST_FS_SELECTOR,
	HOST_GS_SELECTOR
};
static const int max_shadow_read_write_fields =
	ARRAY_SIZE(shadow_read_write_fields);

static const unsigned short vmcs_field_to_offset_table[] = {
>>>>>>> refs/remotes/origin/master
	FIELD(VIRTUAL_PROCESSOR_ID, virtual_processor_id),
	FIELD(GUEST_ES_SELECTOR, guest_es_selector),
	FIELD(GUEST_CS_SELECTOR, guest_cs_selector),
	FIELD(GUEST_SS_SELECTOR, guest_ss_selector),
	FIELD(GUEST_DS_SELECTOR, guest_ds_selector),
	FIELD(GUEST_FS_SELECTOR, guest_fs_selector),
	FIELD(GUEST_GS_SELECTOR, guest_gs_selector),
	FIELD(GUEST_LDTR_SELECTOR, guest_ldtr_selector),
	FIELD(GUEST_TR_SELECTOR, guest_tr_selector),
	FIELD(HOST_ES_SELECTOR, host_es_selector),
	FIELD(HOST_CS_SELECTOR, host_cs_selector),
	FIELD(HOST_SS_SELECTOR, host_ss_selector),
	FIELD(HOST_DS_SELECTOR, host_ds_selector),
	FIELD(HOST_FS_SELECTOR, host_fs_selector),
	FIELD(HOST_GS_SELECTOR, host_gs_selector),
	FIELD(HOST_TR_SELECTOR, host_tr_selector),
	FIELD64(IO_BITMAP_A, io_bitmap_a),
	FIELD64(IO_BITMAP_B, io_bitmap_b),
	FIELD64(MSR_BITMAP, msr_bitmap),
	FIELD64(VM_EXIT_MSR_STORE_ADDR, vm_exit_msr_store_addr),
	FIELD64(VM_EXIT_MSR_LOAD_ADDR, vm_exit_msr_load_addr),
	FIELD64(VM_ENTRY_MSR_LOAD_ADDR, vm_entry_msr_load_addr),
	FIELD64(TSC_OFFSET, tsc_offset),
	FIELD64(VIRTUAL_APIC_PAGE_ADDR, virtual_apic_page_addr),
	FIELD64(APIC_ACCESS_ADDR, apic_access_addr),
	FIELD64(EPT_POINTER, ept_pointer),
	FIELD64(GUEST_PHYSICAL_ADDRESS, guest_physical_address),
	FIELD64(VMCS_LINK_POINTER, vmcs_link_pointer),
	FIELD64(GUEST_IA32_DEBUGCTL, guest_ia32_debugctl),
	FIELD64(GUEST_IA32_PAT, guest_ia32_pat),
	FIELD64(GUEST_IA32_EFER, guest_ia32_efer),
	FIELD64(GUEST_IA32_PERF_GLOBAL_CTRL, guest_ia32_perf_global_ctrl),
	FIELD64(GUEST_PDPTR0, guest_pdptr0),
	FIELD64(GUEST_PDPTR1, guest_pdptr1),
	FIELD64(GUEST_PDPTR2, guest_pdptr2),
	FIELD64(GUEST_PDPTR3, guest_pdptr3),
	FIELD64(HOST_IA32_PAT, host_ia32_pat),
	FIELD64(HOST_IA32_EFER, host_ia32_efer),
	FIELD64(HOST_IA32_PERF_GLOBAL_CTRL, host_ia32_perf_global_ctrl),
	FIELD(PIN_BASED_VM_EXEC_CONTROL, pin_based_vm_exec_control),
	FIELD(CPU_BASED_VM_EXEC_CONTROL, cpu_based_vm_exec_control),
	FIELD(EXCEPTION_BITMAP, exception_bitmap),
	FIELD(PAGE_FAULT_ERROR_CODE_MASK, page_fault_error_code_mask),
	FIELD(PAGE_FAULT_ERROR_CODE_MATCH, page_fault_error_code_match),
	FIELD(CR3_TARGET_COUNT, cr3_target_count),
	FIELD(VM_EXIT_CONTROLS, vm_exit_controls),
	FIELD(VM_EXIT_MSR_STORE_COUNT, vm_exit_msr_store_count),
	FIELD(VM_EXIT_MSR_LOAD_COUNT, vm_exit_msr_load_count),
	FIELD(VM_ENTRY_CONTROLS, vm_entry_controls),
	FIELD(VM_ENTRY_MSR_LOAD_COUNT, vm_entry_msr_load_count),
	FIELD(VM_ENTRY_INTR_INFO_FIELD, vm_entry_intr_info_field),
	FIELD(VM_ENTRY_EXCEPTION_ERROR_CODE, vm_entry_exception_error_code),
	FIELD(VM_ENTRY_INSTRUCTION_LEN, vm_entry_instruction_len),
	FIELD(TPR_THRESHOLD, tpr_threshold),
	FIELD(SECONDARY_VM_EXEC_CONTROL, secondary_vm_exec_control),
	FIELD(VM_INSTRUCTION_ERROR, vm_instruction_error),
	FIELD(VM_EXIT_REASON, vm_exit_reason),
	FIELD(VM_EXIT_INTR_INFO, vm_exit_intr_info),
	FIELD(VM_EXIT_INTR_ERROR_CODE, vm_exit_intr_error_code),
	FIELD(IDT_VECTORING_INFO_FIELD, idt_vectoring_info_field),
	FIELD(IDT_VECTORING_ERROR_CODE, idt_vectoring_error_code),
	FIELD(VM_EXIT_INSTRUCTION_LEN, vm_exit_instruction_len),
	FIELD(VMX_INSTRUCTION_INFO, vmx_instruction_info),
	FIELD(GUEST_ES_LIMIT, guest_es_limit),
	FIELD(GUEST_CS_LIMIT, guest_cs_limit),
	FIELD(GUEST_SS_LIMIT, guest_ss_limit),
	FIELD(GUEST_DS_LIMIT, guest_ds_limit),
	FIELD(GUEST_FS_LIMIT, guest_fs_limit),
	FIELD(GUEST_GS_LIMIT, guest_gs_limit),
	FIELD(GUEST_LDTR_LIMIT, guest_ldtr_limit),
	FIELD(GUEST_TR_LIMIT, guest_tr_limit),
	FIELD(GUEST_GDTR_LIMIT, guest_gdtr_limit),
	FIELD(GUEST_IDTR_LIMIT, guest_idtr_limit),
	FIELD(GUEST_ES_AR_BYTES, guest_es_ar_bytes),
	FIELD(GUEST_CS_AR_BYTES, guest_cs_ar_bytes),
	FIELD(GUEST_SS_AR_BYTES, guest_ss_ar_bytes),
	FIELD(GUEST_DS_AR_BYTES, guest_ds_ar_bytes),
	FIELD(GUEST_FS_AR_BYTES, guest_fs_ar_bytes),
	FIELD(GUEST_GS_AR_BYTES, guest_gs_ar_bytes),
	FIELD(GUEST_LDTR_AR_BYTES, guest_ldtr_ar_bytes),
	FIELD(GUEST_TR_AR_BYTES, guest_tr_ar_bytes),
	FIELD(GUEST_INTERRUPTIBILITY_INFO, guest_interruptibility_info),
	FIELD(GUEST_ACTIVITY_STATE, guest_activity_state),
	FIELD(GUEST_SYSENTER_CS, guest_sysenter_cs),
	FIELD(HOST_IA32_SYSENTER_CS, host_ia32_sysenter_cs),
<<<<<<< HEAD
=======
	FIELD(VMX_PREEMPTION_TIMER_VALUE, vmx_preemption_timer_value),
>>>>>>> refs/remotes/origin/master
	FIELD(CR0_GUEST_HOST_MASK, cr0_guest_host_mask),
	FIELD(CR4_GUEST_HOST_MASK, cr4_guest_host_mask),
	FIELD(CR0_READ_SHADOW, cr0_read_shadow),
	FIELD(CR4_READ_SHADOW, cr4_read_shadow),
	FIELD(CR3_TARGET_VALUE0, cr3_target_value0),
	FIELD(CR3_TARGET_VALUE1, cr3_target_value1),
	FIELD(CR3_TARGET_VALUE2, cr3_target_value2),
	FIELD(CR3_TARGET_VALUE3, cr3_target_value3),
	FIELD(EXIT_QUALIFICATION, exit_qualification),
	FIELD(GUEST_LINEAR_ADDRESS, guest_linear_address),
	FIELD(GUEST_CR0, guest_cr0),
	FIELD(GUEST_CR3, guest_cr3),
	FIELD(GUEST_CR4, guest_cr4),
	FIELD(GUEST_ES_BASE, guest_es_base),
	FIELD(GUEST_CS_BASE, guest_cs_base),
	FIELD(GUEST_SS_BASE, guest_ss_base),
	FIELD(GUEST_DS_BASE, guest_ds_base),
	FIELD(GUEST_FS_BASE, guest_fs_base),
	FIELD(GUEST_GS_BASE, guest_gs_base),
	FIELD(GUEST_LDTR_BASE, guest_ldtr_base),
	FIELD(GUEST_TR_BASE, guest_tr_base),
	FIELD(GUEST_GDTR_BASE, guest_gdtr_base),
	FIELD(GUEST_IDTR_BASE, guest_idtr_base),
	FIELD(GUEST_DR7, guest_dr7),
	FIELD(GUEST_RSP, guest_rsp),
	FIELD(GUEST_RIP, guest_rip),
	FIELD(GUEST_RFLAGS, guest_rflags),
	FIELD(GUEST_PENDING_DBG_EXCEPTIONS, guest_pending_dbg_exceptions),
	FIELD(GUEST_SYSENTER_ESP, guest_sysenter_esp),
	FIELD(GUEST_SYSENTER_EIP, guest_sysenter_eip),
	FIELD(HOST_CR0, host_cr0),
	FIELD(HOST_CR3, host_cr3),
	FIELD(HOST_CR4, host_cr4),
	FIELD(HOST_FS_BASE, host_fs_base),
	FIELD(HOST_GS_BASE, host_gs_base),
	FIELD(HOST_TR_BASE, host_tr_base),
	FIELD(HOST_GDTR_BASE, host_gdtr_base),
	FIELD(HOST_IDTR_BASE, host_idtr_base),
	FIELD(HOST_IA32_SYSENTER_ESP, host_ia32_sysenter_esp),
	FIELD(HOST_IA32_SYSENTER_EIP, host_ia32_sysenter_eip),
	FIELD(HOST_RSP, host_rsp),
	FIELD(HOST_RIP, host_rip),
};
static const int max_vmcs_field = ARRAY_SIZE(vmcs_field_to_offset_table);

static inline short vmcs_field_to_offset(unsigned long field)
{
	if (field >= max_vmcs_field || vmcs_field_to_offset_table[field] == 0)
		return -1;
	return vmcs_field_to_offset_table[field];
}

static inline struct vmcs12 *get_vmcs12(struct kvm_vcpu *vcpu)
{
	return to_vmx(vcpu)->nested.current_vmcs12;
}

static struct page *nested_get_page(struct kvm_vcpu *vcpu, gpa_t addr)
{
	struct page *page = gfn_to_page(vcpu->kvm, addr >> PAGE_SHIFT);
<<<<<<< HEAD
	if (is_error_page(page)) {
		kvm_release_page_clean(page);
		return NULL;
	}
=======
	if (is_error_page(page))
		return NULL;

>>>>>>> refs/remotes/origin/master
	return page;
}

static void nested_release_page(struct page *page)
{
	kvm_release_page_dirty(page);
}

static void nested_release_page_clean(struct page *page)
{
	kvm_release_page_clean(page);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static u64 construct_eptp(unsigned long root_hpa);
static void kvm_cpu_vmxon(u64 addr);
static void kvm_cpu_vmxoff(void);
static void vmx_set_cr3(struct kvm_vcpu *vcpu, unsigned long cr3);
static int vmx_set_tss_addr(struct kvm *kvm, unsigned int addr);
<<<<<<< HEAD
=======
static unsigned long nested_ept_get_cr3(struct kvm_vcpu *vcpu);
static u64 construct_eptp(unsigned long root_hpa);
static void kvm_cpu_vmxon(u64 addr);
static void kvm_cpu_vmxoff(void);
static int vmx_set_tss_addr(struct kvm *kvm, unsigned int addr);
>>>>>>> refs/remotes/origin/master
static void vmx_set_segment(struct kvm_vcpu *vcpu,
			    struct kvm_segment *var, int seg);
static void vmx_get_segment(struct kvm_vcpu *vcpu,
			    struct kvm_segment *var, int seg);
<<<<<<< HEAD

static DEFINE_PER_CPU(struct vmcs *, vmxarea);
static DEFINE_PER_CPU(struct vmcs *, current_vmcs);
static DEFINE_PER_CPU(struct list_head, vcpus_on_cpu);
=======
=======
static bool guest_state_valid(struct kvm_vcpu *vcpu);
static u32 vmx_segment_access_rights(struct kvm_segment *var);
static void vmx_sync_pir_to_irr_dummy(struct kvm_vcpu *vcpu);
static void copy_vmcs12_to_shadow(struct vcpu_vmx *vmx);
static void copy_shadow_to_vmcs12(struct vcpu_vmx *vmx);
>>>>>>> refs/remotes/origin/master

static DEFINE_PER_CPU(struct vmcs *, vmxarea);
static DEFINE_PER_CPU(struct vmcs *, current_vmcs);
/*
 * We maintain a per-CPU linked-list of VMCS loaded on that CPU. This is needed
 * when a CPU is brought down, and we need to VMCLEAR all VMCSs loaded on it.
 */
static DEFINE_PER_CPU(struct list_head, loaded_vmcss_on_cpu);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static DEFINE_PER_CPU(struct desc_ptr, host_gdt);

static unsigned long *vmx_io_bitmap_a;
static unsigned long *vmx_io_bitmap_b;
static unsigned long *vmx_msr_bitmap_legacy;
static unsigned long *vmx_msr_bitmap_longmode;
<<<<<<< HEAD

static bool cpu_has_load_ia32_efer;
<<<<<<< HEAD
=======
static bool cpu_has_load_perf_global_ctrl;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static unsigned long *vmx_msr_bitmap_legacy_x2apic;
static unsigned long *vmx_msr_bitmap_longmode_x2apic;
static unsigned long *vmx_vmread_bitmap;
static unsigned long *vmx_vmwrite_bitmap;

static bool cpu_has_load_ia32_efer;
static bool cpu_has_load_perf_global_ctrl;
>>>>>>> refs/remotes/origin/master

static DECLARE_BITMAP(vmx_vpid_bitmap, VMX_NR_VPIDS);
static DEFINE_SPINLOCK(vmx_vpid_lock);

static struct vmcs_config {
	int size;
	int order;
	u32 revision_id;
	u32 pin_based_exec_ctrl;
	u32 cpu_based_exec_ctrl;
	u32 cpu_based_2nd_exec_ctrl;
	u32 vmexit_ctrl;
	u32 vmentry_ctrl;
} vmcs_config;

static struct vmx_capability {
	u32 ept;
	u32 vpid;
} vmx_capability;

#define VMX_SEGMENT_FIELD(seg)					\
	[VCPU_SREG_##seg] = {                                   \
		.selector = GUEST_##seg##_SELECTOR,		\
		.base = GUEST_##seg##_BASE,		   	\
		.limit = GUEST_##seg##_LIMIT,		   	\
		.ar_bytes = GUEST_##seg##_AR_BYTES,	   	\
	}

<<<<<<< HEAD
static struct kvm_vmx_segment_field {
=======
static const struct kvm_vmx_segment_field {
>>>>>>> refs/remotes/origin/master
	unsigned selector;
	unsigned base;
	unsigned limit;
	unsigned ar_bytes;
} kvm_vmx_segment_fields[] = {
	VMX_SEGMENT_FIELD(CS),
	VMX_SEGMENT_FIELD(DS),
	VMX_SEGMENT_FIELD(ES),
	VMX_SEGMENT_FIELD(FS),
	VMX_SEGMENT_FIELD(GS),
	VMX_SEGMENT_FIELD(SS),
	VMX_SEGMENT_FIELD(TR),
	VMX_SEGMENT_FIELD(LDTR),
};

static u64 host_efer;

static void ept_save_pdptrs(struct kvm_vcpu *vcpu);

/*
 * Keep MSR_STAR at the end, as setup_msrs() will try to optimize it
 * away by decrementing the array size.
 */
static const u32 vmx_msr_index[] = {
#ifdef CONFIG_X86_64
	MSR_SYSCALL_MASK, MSR_LSTAR, MSR_CSTAR,
#endif
	MSR_EFER, MSR_TSC_AUX, MSR_STAR,
};
#define NR_VMX_MSR ARRAY_SIZE(vmx_msr_index)

static inline bool is_page_fault(u32 intr_info)
{
	return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VECTOR_MASK |
			     INTR_INFO_VALID_MASK)) ==
		(INTR_TYPE_HARD_EXCEPTION | PF_VECTOR | INTR_INFO_VALID_MASK);
}

static inline bool is_no_device(u32 intr_info)
{
	return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VECTOR_MASK |
			     INTR_INFO_VALID_MASK)) ==
		(INTR_TYPE_HARD_EXCEPTION | NM_VECTOR | INTR_INFO_VALID_MASK);
}

static inline bool is_invalid_opcode(u32 intr_info)
{
	return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VECTOR_MASK |
			     INTR_INFO_VALID_MASK)) ==
		(INTR_TYPE_HARD_EXCEPTION | UD_VECTOR | INTR_INFO_VALID_MASK);
}

static inline bool is_external_interrupt(u32 intr_info)
{
	return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VALID_MASK))
		== (INTR_TYPE_EXT_INTR | INTR_INFO_VALID_MASK);
}

static inline bool is_machine_check(u32 intr_info)
{
	return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VECTOR_MASK |
			     INTR_INFO_VALID_MASK)) ==
		(INTR_TYPE_HARD_EXCEPTION | MC_VECTOR | INTR_INFO_VALID_MASK);
}

static inline bool cpu_has_vmx_msr_bitmap(void)
{
	return vmcs_config.cpu_based_exec_ctrl & CPU_BASED_USE_MSR_BITMAPS;
}

static inline bool cpu_has_vmx_tpr_shadow(void)
{
	return vmcs_config.cpu_based_exec_ctrl & CPU_BASED_TPR_SHADOW;
}

static inline bool vm_need_tpr_shadow(struct kvm *kvm)
{
	return (cpu_has_vmx_tpr_shadow()) && (irqchip_in_kernel(kvm));
}

static inline bool cpu_has_secondary_exec_ctrls(void)
{
	return vmcs_config.cpu_based_exec_ctrl &
		CPU_BASED_ACTIVATE_SECONDARY_CONTROLS;
}

static inline bool cpu_has_vmx_virtualize_apic_accesses(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
}

<<<<<<< HEAD
=======
static inline bool cpu_has_vmx_virtualize_x2apic_mode(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE;
}

static inline bool cpu_has_vmx_apic_register_virt(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_APIC_REGISTER_VIRT;
}

static inline bool cpu_has_vmx_virtual_intr_delivery(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY;
}

static inline bool cpu_has_vmx_posted_intr(void)
{
	return vmcs_config.pin_based_exec_ctrl & PIN_BASED_POSTED_INTR;
}

static inline bool cpu_has_vmx_apicv(void)
{
	return cpu_has_vmx_apic_register_virt() &&
		cpu_has_vmx_virtual_intr_delivery() &&
		cpu_has_vmx_posted_intr();
}

>>>>>>> refs/remotes/origin/master
static inline bool cpu_has_vmx_flexpriority(void)
{
	return cpu_has_vmx_tpr_shadow() &&
		cpu_has_vmx_virtualize_apic_accesses();
}

static inline bool cpu_has_vmx_ept_execute_only(void)
{
	return vmx_capability.ept & VMX_EPT_EXECUTE_ONLY_BIT;
}

static inline bool cpu_has_vmx_eptp_uncacheable(void)
{
	return vmx_capability.ept & VMX_EPTP_UC_BIT;
}

static inline bool cpu_has_vmx_eptp_writeback(void)
{
	return vmx_capability.ept & VMX_EPTP_WB_BIT;
}

static inline bool cpu_has_vmx_ept_2m_page(void)
{
	return vmx_capability.ept & VMX_EPT_2MB_PAGE_BIT;
}

static inline bool cpu_has_vmx_ept_1g_page(void)
{
	return vmx_capability.ept & VMX_EPT_1GB_PAGE_BIT;
}

static inline bool cpu_has_vmx_ept_4levels(void)
{
	return vmx_capability.ept & VMX_EPT_PAGE_WALK_4_BIT;
}

<<<<<<< HEAD
static inline bool cpu_has_vmx_invept_individual_addr(void)
{
	return vmx_capability.ept & VMX_EPT_EXTENT_INDIVIDUAL_BIT;
=======
static inline bool cpu_has_vmx_ept_ad_bits(void)
{
	return vmx_capability.ept & VMX_EPT_AD_BIT;
>>>>>>> refs/remotes/origin/master
}

static inline bool cpu_has_vmx_invept_context(void)
{
	return vmx_capability.ept & VMX_EPT_EXTENT_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invept_global(void)
{
	return vmx_capability.ept & VMX_EPT_EXTENT_GLOBAL_BIT;
}

static inline bool cpu_has_vmx_invvpid_single(void)
{
	return vmx_capability.vpid & VMX_VPID_EXTENT_SINGLE_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_invvpid_global(void)
{
	return vmx_capability.vpid & VMX_VPID_EXTENT_GLOBAL_CONTEXT_BIT;
}

static inline bool cpu_has_vmx_ept(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_ENABLE_EPT;
}

static inline bool cpu_has_vmx_unrestricted_guest(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_UNRESTRICTED_GUEST;
}

static inline bool cpu_has_vmx_ple(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_PAUSE_LOOP_EXITING;
}

static inline bool vm_need_virtualize_apic_accesses(struct kvm *kvm)
{
	return flexpriority_enabled && irqchip_in_kernel(kvm);
}

static inline bool cpu_has_vmx_vpid(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_ENABLE_VPID;
}

static inline bool cpu_has_vmx_rdtscp(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_RDTSCP;
}

<<<<<<< HEAD
=======
static inline bool cpu_has_vmx_invpcid(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_ENABLE_INVPCID;
}

>>>>>>> refs/remotes/origin/master
static inline bool cpu_has_virtual_nmis(void)
{
	return vmcs_config.pin_based_exec_ctrl & PIN_BASED_VIRTUAL_NMIS;
}

static inline bool cpu_has_vmx_wbinvd_exit(void)
{
	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_WBINVD_EXITING;
}

<<<<<<< HEAD
=======
static inline bool cpu_has_vmx_shadow_vmcs(void)
{
	u64 vmx_msr;
	rdmsrl(MSR_IA32_VMX_MISC, vmx_msr);
	/* check if the cpu supports writing r/o exit information fields */
	if (!(vmx_msr & MSR_IA32_VMX_MISC_VMWRITE_SHADOW_RO_FIELDS))
		return false;

	return vmcs_config.cpu_based_2nd_exec_ctrl &
		SECONDARY_EXEC_SHADOW_VMCS;
}

>>>>>>> refs/remotes/origin/master
static inline bool report_flexpriority(void)
{
	return flexpriority_enabled;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static inline bool nested_cpu_has(struct vmcs12 *vmcs12, u32 bit)
{
	return vmcs12->cpu_based_vm_exec_control & bit;
}

static inline bool nested_cpu_has2(struct vmcs12 *vmcs12, u32 bit)
{
	return (vmcs12->cpu_based_vm_exec_control &
			CPU_BASED_ACTIVATE_SECONDARY_CONTROLS) &&
		(vmcs12->secondary_vm_exec_control & bit);
}

<<<<<<< HEAD
static inline bool nested_cpu_has_virtual_nmis(struct vmcs12 *vmcs12,
	struct kvm_vcpu *vcpu)
=======
static inline bool nested_cpu_has_virtual_nmis(struct vmcs12 *vmcs12)
>>>>>>> refs/remotes/origin/master
{
	return vmcs12->pin_based_vm_exec_control & PIN_BASED_VIRTUAL_NMIS;
}

<<<<<<< HEAD
=======
static inline int nested_cpu_has_ept(struct vmcs12 *vmcs12)
{
	return nested_cpu_has2(vmcs12, SECONDARY_EXEC_ENABLE_EPT);
}

>>>>>>> refs/remotes/origin/master
static inline bool is_exception(u32 intr_info)
{
	return (intr_info & (INTR_INFO_INTR_TYPE_MASK | INTR_INFO_VALID_MASK))
		== (INTR_TYPE_HARD_EXCEPTION | INTR_INFO_VALID_MASK);
}

static void nested_vmx_vmexit(struct kvm_vcpu *vcpu);
static void nested_vmx_entry_failure(struct kvm_vcpu *vcpu,
			struct vmcs12 *vmcs12,
			u32 reason, unsigned long qualification);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int __find_msr_index(struct vcpu_vmx *vmx, u32 msr)
{
	int i;

	for (i = 0; i < vmx->nmsrs; ++i)
		if (vmx_msr_index[vmx->guest_msrs[i].index] == msr)
			return i;
	return -1;
}

static inline void __invvpid(int ext, u16 vpid, gva_t gva)
{
    struct {
	u64 vpid : 16;
	u64 rsvd : 48;
	u64 gva;
    } operand = { vpid, 0, gva };

    asm volatile (__ex(ASM_VMX_INVVPID)
		  /* CF==1 or ZF==1 --> rc = -1 */
		  "; ja 1f ; ud2 ; 1:"
		  : : "a"(&operand), "c"(ext) : "cc", "memory");
}

static inline void __invept(int ext, u64 eptp, gpa_t gpa)
{
	struct {
		u64 eptp, gpa;
	} operand = {eptp, gpa};

	asm volatile (__ex(ASM_VMX_INVEPT)
			/* CF==1 or ZF==1 --> rc = -1 */
			"; ja 1f ; ud2 ; 1:\n"
			: : "a" (&operand), "c" (ext) : "cc", "memory");
}

static struct shared_msr_entry *find_msr_entry(struct vcpu_vmx *vmx, u32 msr)
{
	int i;

	i = __find_msr_index(vmx, msr);
	if (i >= 0)
		return &vmx->guest_msrs[i];
	return NULL;
}

static void vmcs_clear(struct vmcs *vmcs)
{
	u64 phys_addr = __pa(vmcs);
	u8 error;

	asm volatile (__ex(ASM_VMX_VMCLEAR_RAX) "; setna %0"
		      : "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
		      : "cc", "memory");
	if (error)
		printk(KERN_ERR "kvm: vmclear fail: %p/%llx\n",
		       vmcs, phys_addr);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static inline void loaded_vmcs_init(struct loaded_vmcs *loaded_vmcs)
{
	vmcs_clear(loaded_vmcs->vmcs);
	loaded_vmcs->cpu = -1;
	loaded_vmcs->launched = 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void vmcs_load(struct vmcs *vmcs)
{
	u64 phys_addr = __pa(vmcs);
	u8 error;

	asm volatile (__ex(ASM_VMX_VMPTRLD_RAX) "; setna %0"
			: "=qm"(error) : "a"(&phys_addr), "m"(phys_addr)
			: "cc", "memory");
	if (error)
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "kvm: vmptrld %p/%llx fail\n",
		       vmcs, phys_addr);
}

static void __vcpu_clear(void *arg)
{
	struct vcpu_vmx *vmx = arg;
	int cpu = raw_smp_processor_id();

	if (vmx->vcpu.cpu == cpu)
		vmcs_clear(vmx->vmcs);
	if (per_cpu(current_vmcs, cpu) == vmx->vmcs)
		per_cpu(current_vmcs, cpu) = NULL;
	list_del(&vmx->local_vcpus_link);
	vmx->vcpu.cpu = -1;
	vmx->launched = 0;
}

static void vcpu_clear(struct vcpu_vmx *vmx)
{
	if (vmx->vcpu.cpu == -1)
		return;
	smp_call_function_single(vmx->vcpu.cpu, __vcpu_clear, vmx, 1);
=======
		printk(KERN_ERR "kvm: vmptrld %p/%llx failed\n",
		       vmcs, phys_addr);
}
=======
		printk(KERN_ERR "kvm: vmptrld %p/%llx failed\n",
		       vmcs, phys_addr);
}

#ifdef CONFIG_KEXEC
/*
 * This bitmap is used to indicate whether the vmclear
 * operation is enabled on all cpus. All disabled by
 * default.
 */
static cpumask_t crash_vmclear_enabled_bitmap = CPU_MASK_NONE;

static inline void crash_enable_local_vmclear(int cpu)
{
	cpumask_set_cpu(cpu, &crash_vmclear_enabled_bitmap);
}

static inline void crash_disable_local_vmclear(int cpu)
{
	cpumask_clear_cpu(cpu, &crash_vmclear_enabled_bitmap);
}

static inline int crash_local_vmclear_enabled(int cpu)
{
	return cpumask_test_cpu(cpu, &crash_vmclear_enabled_bitmap);
}

static void crash_vmclear_local_loaded_vmcss(void)
{
	int cpu = raw_smp_processor_id();
	struct loaded_vmcs *v;

	if (!crash_local_vmclear_enabled(cpu))
		return;

	list_for_each_entry(v, &per_cpu(loaded_vmcss_on_cpu, cpu),
			    loaded_vmcss_on_cpu_link)
		vmcs_clear(v->vmcs);
}
#else
static inline void crash_enable_local_vmclear(int cpu) { }
static inline void crash_disable_local_vmclear(int cpu) { }
#endif /* CONFIG_KEXEC */
>>>>>>> refs/remotes/origin/master

static void __loaded_vmcs_clear(void *arg)
{
	struct loaded_vmcs *loaded_vmcs = arg;
	int cpu = raw_smp_processor_id();

	if (loaded_vmcs->cpu != cpu)
		return; /* vcpu migration can race with cpu offline */
	if (per_cpu(current_vmcs, cpu) == loaded_vmcs->vmcs)
		per_cpu(current_vmcs, cpu) = NULL;
<<<<<<< HEAD
	list_del(&loaded_vmcs->loaded_vmcss_on_cpu_link);
	loaded_vmcs_init(loaded_vmcs);
=======
	crash_disable_local_vmclear(cpu);
	list_del(&loaded_vmcs->loaded_vmcss_on_cpu_link);

	/*
	 * we should ensure updating loaded_vmcs->loaded_vmcss_on_cpu_link
	 * is before setting loaded_vmcs->vcpu to -1 which is done in
	 * loaded_vmcs_init. Otherwise, other cpu can see vcpu = -1 fist
	 * then adds the vmcs into percpu list before it is deleted.
	 */
	smp_wmb();

	loaded_vmcs_init(loaded_vmcs);
	crash_enable_local_vmclear(cpu);
>>>>>>> refs/remotes/origin/master
}

static void loaded_vmcs_clear(struct loaded_vmcs *loaded_vmcs)
{
<<<<<<< HEAD
	if (loaded_vmcs->cpu != -1)
		smp_call_function_single(
			loaded_vmcs->cpu, __loaded_vmcs_clear, loaded_vmcs, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int cpu = loaded_vmcs->cpu;

	if (cpu != -1)
		smp_call_function_single(cpu,
			 __loaded_vmcs_clear, loaded_vmcs, 1);
>>>>>>> refs/remotes/origin/master
}

static inline void vpid_sync_vcpu_single(struct vcpu_vmx *vmx)
{
	if (vmx->vpid == 0)
		return;

	if (cpu_has_vmx_invvpid_single())
		__invvpid(VMX_VPID_EXTENT_SINGLE_CONTEXT, vmx->vpid, 0);
}

static inline void vpid_sync_vcpu_global(void)
{
	if (cpu_has_vmx_invvpid_global())
		__invvpid(VMX_VPID_EXTENT_ALL_CONTEXT, 0, 0);
}

static inline void vpid_sync_context(struct vcpu_vmx *vmx)
{
	if (cpu_has_vmx_invvpid_single())
		vpid_sync_vcpu_single(vmx);
	else
		vpid_sync_vcpu_global();
}

static inline void ept_sync_global(void)
{
	if (cpu_has_vmx_invept_global())
		__invept(VMX_EPT_EXTENT_GLOBAL, 0, 0);
}

static inline void ept_sync_context(u64 eptp)
{
	if (enable_ept) {
		if (cpu_has_vmx_invept_context())
			__invept(VMX_EPT_EXTENT_CONTEXT, eptp, 0);
		else
			ept_sync_global();
	}
}

<<<<<<< HEAD
static inline void ept_sync_individual_addr(u64 eptp, gpa_t gpa)
{
	if (enable_ept) {
		if (cpu_has_vmx_invept_individual_addr())
			__invept(VMX_EPT_EXTENT_INDIVIDUAL_ADDR,
					eptp, gpa);
		else
			ept_sync_context(eptp);
	}
}

<<<<<<< HEAD
static unsigned long vmcs_readl(unsigned long field)
{
	unsigned long value = 0;

	asm volatile (__ex(ASM_VMX_VMREAD_RDX_RAX)
		      : "+a"(value) : "d"(field) : "cc");
	return value;
}

static u16 vmcs_read16(unsigned long field)
=======
=======
>>>>>>> refs/remotes/origin/master
static __always_inline unsigned long vmcs_readl(unsigned long field)
{
	unsigned long value;

	asm volatile (__ex_clear(ASM_VMX_VMREAD_RDX_RAX, "%0")
		      : "=a"(value) : "d"(field) : "cc");
	return value;
}

static __always_inline u16 vmcs_read16(unsigned long field)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	return vmcs_readl(field);
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 vmcs_read32(unsigned long field)
=======
static __always_inline u32 vmcs_read32(unsigned long field)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static __always_inline u32 vmcs_read32(unsigned long field)
>>>>>>> refs/remotes/origin/master
{
	return vmcs_readl(field);
}

<<<<<<< HEAD
<<<<<<< HEAD
static u64 vmcs_read64(unsigned long field)
=======
static __always_inline u64 vmcs_read64(unsigned long field)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static __always_inline u64 vmcs_read64(unsigned long field)
>>>>>>> refs/remotes/origin/master
{
#ifdef CONFIG_X86_64
	return vmcs_readl(field);
#else
	return vmcs_readl(field) | ((u64)vmcs_readl(field+1) << 32);
#endif
}

static noinline void vmwrite_error(unsigned long field, unsigned long value)
{
	printk(KERN_ERR "vmwrite error: reg %lx value %lx (err %d)\n",
	       field, value, vmcs_read32(VM_INSTRUCTION_ERROR));
	dump_stack();
}

static void vmcs_writel(unsigned long field, unsigned long value)
{
	u8 error;

	asm volatile (__ex(ASM_VMX_VMWRITE_RAX_RDX) "; setna %0"
		       : "=q"(error) : "a"(value), "d"(field) : "cc");
	if (unlikely(error))
		vmwrite_error(field, value);
}

static void vmcs_write16(unsigned long field, u16 value)
{
	vmcs_writel(field, value);
}

static void vmcs_write32(unsigned long field, u32 value)
{
	vmcs_writel(field, value);
}

static void vmcs_write64(unsigned long field, u64 value)
{
	vmcs_writel(field, value);
#ifndef CONFIG_X86_64
	asm volatile ("");
	vmcs_writel(field+1, value >> 32);
#endif
}

static void vmcs_clear_bits(unsigned long field, u32 mask)
{
	vmcs_writel(field, vmcs_readl(field) & ~mask);
}

static void vmcs_set_bits(unsigned long field, u32 mask)
{
	vmcs_writel(field, vmcs_readl(field) | mask);
}

static void vmx_segment_cache_clear(struct vcpu_vmx *vmx)
{
	vmx->segment_cache.bitmask = 0;
}

static bool vmx_segment_cache_test_set(struct vcpu_vmx *vmx, unsigned seg,
				       unsigned field)
{
	bool ret;
	u32 mask = 1 << (seg * SEG_FIELD_NR + field);

	if (!(vmx->vcpu.arch.regs_avail & (1 << VCPU_EXREG_SEGMENTS))) {
		vmx->vcpu.arch.regs_avail |= (1 << VCPU_EXREG_SEGMENTS);
		vmx->segment_cache.bitmask = 0;
	}
	ret = vmx->segment_cache.bitmask & mask;
	vmx->segment_cache.bitmask |= mask;
	return ret;
}

static u16 vmx_read_guest_seg_selector(struct vcpu_vmx *vmx, unsigned seg)
{
	u16 *p = &vmx->segment_cache.seg[seg].selector;

	if (!vmx_segment_cache_test_set(vmx, seg, SEG_FIELD_SEL))
		*p = vmcs_read16(kvm_vmx_segment_fields[seg].selector);
	return *p;
}

static ulong vmx_read_guest_seg_base(struct vcpu_vmx *vmx, unsigned seg)
{
	ulong *p = &vmx->segment_cache.seg[seg].base;

	if (!vmx_segment_cache_test_set(vmx, seg, SEG_FIELD_BASE))
		*p = vmcs_readl(kvm_vmx_segment_fields[seg].base);
	return *p;
}

static u32 vmx_read_guest_seg_limit(struct vcpu_vmx *vmx, unsigned seg)
{
	u32 *p = &vmx->segment_cache.seg[seg].limit;

	if (!vmx_segment_cache_test_set(vmx, seg, SEG_FIELD_LIMIT))
		*p = vmcs_read32(kvm_vmx_segment_fields[seg].limit);
	return *p;
}

static u32 vmx_read_guest_seg_ar(struct vcpu_vmx *vmx, unsigned seg)
{
	u32 *p = &vmx->segment_cache.seg[seg].ar;

	if (!vmx_segment_cache_test_set(vmx, seg, SEG_FIELD_AR))
		*p = vmcs_read32(kvm_vmx_segment_fields[seg].ar_bytes);
	return *p;
}

static void update_exception_bitmap(struct kvm_vcpu *vcpu)
{
	u32 eb;

	eb = (1u << PF_VECTOR) | (1u << UD_VECTOR) | (1u << MC_VECTOR) |
	     (1u << NM_VECTOR) | (1u << DB_VECTOR);
	if ((vcpu->guest_debug &
	     (KVM_GUESTDBG_ENABLE | KVM_GUESTDBG_USE_SW_BP)) ==
	    (KVM_GUESTDBG_ENABLE | KVM_GUESTDBG_USE_SW_BP))
		eb |= 1u << BP_VECTOR;
	if (to_vmx(vcpu)->rmode.vm86_active)
		eb = ~0;
	if (enable_ept)
		eb &= ~(1u << PF_VECTOR); /* bypass_guest_pf = 0 */
	if (vcpu->fpu_active)
		eb &= ~(1u << NM_VECTOR);
<<<<<<< HEAD
<<<<<<< HEAD
	vmcs_write32(EXCEPTION_BITMAP, eb);
}

=======
=======
>>>>>>> refs/remotes/origin/master

	/* When we are running a nested L2 guest and L1 specified for it a
	 * certain exception bitmap, we must trap the same exceptions and pass
	 * them to L1. When running L2, we will only handle the exceptions
	 * specified above if L1 did not want them.
	 */
	if (is_guest_mode(vcpu))
		eb |= get_vmcs12(vcpu)->exception_bitmap;

	vmcs_write32(EXCEPTION_BITMAP, eb);
}

static void clear_atomic_switch_msr_special(unsigned long entry,
		unsigned long exit)
{
	vmcs_clear_bits(VM_ENTRY_CONTROLS, entry);
	vmcs_clear_bits(VM_EXIT_CONTROLS, exit);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void clear_atomic_switch_msr(struct vcpu_vmx *vmx, unsigned msr)
{
	unsigned i;
	struct msr_autoload *m = &vmx->msr_autoload;

<<<<<<< HEAD
<<<<<<< HEAD
	if (msr == MSR_EFER && cpu_has_load_ia32_efer) {
		vmcs_clear_bits(VM_ENTRY_CONTROLS, VM_ENTRY_LOAD_IA32_EFER);
		vmcs_clear_bits(VM_EXIT_CONTROLS, VM_EXIT_LOAD_IA32_EFER);
		return;
=======
=======
>>>>>>> refs/remotes/origin/master
	switch (msr) {
	case MSR_EFER:
		if (cpu_has_load_ia32_efer) {
			clear_atomic_switch_msr_special(VM_ENTRY_LOAD_IA32_EFER,
					VM_EXIT_LOAD_IA32_EFER);
			return;
		}
		break;
	case MSR_CORE_PERF_GLOBAL_CTRL:
		if (cpu_has_load_perf_global_ctrl) {
			clear_atomic_switch_msr_special(
					VM_ENTRY_LOAD_IA32_PERF_GLOBAL_CTRL,
					VM_EXIT_LOAD_IA32_PERF_GLOBAL_CTRL);
			return;
		}
		break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	for (i = 0; i < m->nr; ++i)
		if (m->guest[i].index == msr)
			break;

	if (i == m->nr)
		return;
	--m->nr;
	m->guest[i] = m->guest[m->nr];
	m->host[i] = m->host[m->nr];
	vmcs_write32(VM_ENTRY_MSR_LOAD_COUNT, m->nr);
	vmcs_write32(VM_EXIT_MSR_LOAD_COUNT, m->nr);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void add_atomic_switch_msr_special(unsigned long entry,
		unsigned long exit, unsigned long guest_val_vmcs,
		unsigned long host_val_vmcs, u64 guest_val, u64 host_val)
{
	vmcs_write64(guest_val_vmcs, guest_val);
	vmcs_write64(host_val_vmcs, host_val);
	vmcs_set_bits(VM_ENTRY_CONTROLS, entry);
	vmcs_set_bits(VM_EXIT_CONTROLS, exit);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void add_atomic_switch_msr(struct vcpu_vmx *vmx, unsigned msr,
				  u64 guest_val, u64 host_val)
{
	unsigned i;
	struct msr_autoload *m = &vmx->msr_autoload;

<<<<<<< HEAD
<<<<<<< HEAD
	if (msr == MSR_EFER && cpu_has_load_ia32_efer) {
		vmcs_write64(GUEST_IA32_EFER, guest_val);
		vmcs_write64(HOST_IA32_EFER, host_val);
		vmcs_set_bits(VM_ENTRY_CONTROLS, VM_ENTRY_LOAD_IA32_EFER);
		vmcs_set_bits(VM_EXIT_CONTROLS, VM_EXIT_LOAD_IA32_EFER);
		return;
=======
=======
>>>>>>> refs/remotes/origin/master
	switch (msr) {
	case MSR_EFER:
		if (cpu_has_load_ia32_efer) {
			add_atomic_switch_msr_special(VM_ENTRY_LOAD_IA32_EFER,
					VM_EXIT_LOAD_IA32_EFER,
					GUEST_IA32_EFER,
					HOST_IA32_EFER,
					guest_val, host_val);
			return;
		}
		break;
	case MSR_CORE_PERF_GLOBAL_CTRL:
		if (cpu_has_load_perf_global_ctrl) {
			add_atomic_switch_msr_special(
					VM_ENTRY_LOAD_IA32_PERF_GLOBAL_CTRL,
					VM_EXIT_LOAD_IA32_PERF_GLOBAL_CTRL,
					GUEST_IA32_PERF_GLOBAL_CTRL,
					HOST_IA32_PERF_GLOBAL_CTRL,
					guest_val, host_val);
			return;
		}
		break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	for (i = 0; i < m->nr; ++i)
		if (m->guest[i].index == msr)
			break;

<<<<<<< HEAD
<<<<<<< HEAD
	if (i == m->nr) {
=======
	if (i == NR_AUTOLOAD_MSRS) {
		printk_once(KERN_WARNING"Not enough mst switch entries. "
				"Can't add msr %x\n", msr);
		return;
	} else if (i == m->nr) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (i == NR_AUTOLOAD_MSRS) {
		printk_once(KERN_WARNING "Not enough msr switch entries. "
				"Can't add msr %x\n", msr);
		return;
	} else if (i == m->nr) {
>>>>>>> refs/remotes/origin/master
		++m->nr;
		vmcs_write32(VM_ENTRY_MSR_LOAD_COUNT, m->nr);
		vmcs_write32(VM_EXIT_MSR_LOAD_COUNT, m->nr);
	}

	m->guest[i].index = msr;
	m->guest[i].value = guest_val;
	m->host[i].index = msr;
	m->host[i].value = host_val;
}

static void reload_tss(void)
{
	/*
	 * VT restores TR but not its size.  Useless.
	 */
	struct desc_ptr *gdt = &__get_cpu_var(host_gdt);
	struct desc_struct *descs;

	descs = (void *)gdt->address;
	descs[GDT_ENTRY_TSS].type = 9; /* available TSS */
	load_TR_desc();
}

static bool update_transition_efer(struct vcpu_vmx *vmx, int efer_offset)
{
	u64 guest_efer;
	u64 ignore_bits;

	guest_efer = vmx->vcpu.arch.efer;

	/*
<<<<<<< HEAD
	 * NX is emulated; LMA and LME handled by hardware; SCE meaninless
=======
	 * NX is emulated; LMA and LME handled by hardware; SCE meaningless
>>>>>>> refs/remotes/origin/master
	 * outside long mode
	 */
	ignore_bits = EFER_NX | EFER_SCE;
#ifdef CONFIG_X86_64
	ignore_bits |= EFER_LMA | EFER_LME;
	/* SCE is meaningful only in long mode on Intel */
	if (guest_efer & EFER_LMA)
		ignore_bits &= ~(u64)EFER_SCE;
#endif
	guest_efer &= ~ignore_bits;
	guest_efer |= host_efer & ignore_bits;
	vmx->guest_msrs[efer_offset].data = guest_efer;
	vmx->guest_msrs[efer_offset].mask = ~ignore_bits;

	clear_atomic_switch_msr(vmx, MSR_EFER);
	/* On ept, can't emulate nx, and must switch nx atomically */
	if (enable_ept && ((vmx->vcpu.arch.efer ^ host_efer) & EFER_NX)) {
		guest_efer = vmx->vcpu.arch.efer;
		if (!(guest_efer & EFER_LMA))
			guest_efer &= ~EFER_LME;
		add_atomic_switch_msr(vmx, MSR_EFER, guest_efer, host_efer);
		return false;
	}

	return true;
}

static unsigned long segment_base(u16 selector)
{
	struct desc_ptr *gdt = &__get_cpu_var(host_gdt);
	struct desc_struct *d;
	unsigned long table_base;
	unsigned long v;

	if (!(selector & ~3))
		return 0;

	table_base = gdt->address;

	if (selector & 4) {           /* from ldt */
		u16 ldt_selector = kvm_read_ldt();

		if (!(ldt_selector & ~3))
			return 0;

		table_base = segment_base(ldt_selector);
	}
	d = (struct desc_struct *)(table_base + (selector & ~7));
	v = get_desc_base(d);
#ifdef CONFIG_X86_64
       if (d->s == 0 && (d->type == 2 || d->type == 9 || d->type == 11))
               v |= ((unsigned long)((struct ldttss_desc64 *)d)->base3) << 32;
#endif
	return v;
}

static inline unsigned long kvm_read_tr_base(void)
{
	u16 tr;
	asm("str %0" : "=g"(tr));
	return segment_base(tr);
}

static void vmx_save_host_state(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	int i;

	if (vmx->host_state.loaded)
		return;

	vmx->host_state.loaded = 1;
	/*
	 * Set host fs and gs selectors.  Unfortunately, 22.2.3 does not
	 * allow segment selectors with cpl > 0 or ti == 1.
	 */
	vmx->host_state.ldt_sel = kvm_read_ldt();
	vmx->host_state.gs_ldt_reload_needed = vmx->host_state.ldt_sel;
	savesegment(fs, vmx->host_state.fs_sel);
	if (!(vmx->host_state.fs_sel & 7)) {
		vmcs_write16(HOST_FS_SELECTOR, vmx->host_state.fs_sel);
		vmx->host_state.fs_reload_needed = 0;
	} else {
		vmcs_write16(HOST_FS_SELECTOR, 0);
		vmx->host_state.fs_reload_needed = 1;
	}
	savesegment(gs, vmx->host_state.gs_sel);
	if (!(vmx->host_state.gs_sel & 7))
		vmcs_write16(HOST_GS_SELECTOR, vmx->host_state.gs_sel);
	else {
		vmcs_write16(HOST_GS_SELECTOR, 0);
		vmx->host_state.gs_ldt_reload_needed = 1;
	}

#ifdef CONFIG_X86_64
<<<<<<< HEAD
=======
	savesegment(ds, vmx->host_state.ds_sel);
	savesegment(es, vmx->host_state.es_sel);
#endif

#ifdef CONFIG_X86_64
>>>>>>> refs/remotes/origin/master
	vmcs_writel(HOST_FS_BASE, read_msr(MSR_FS_BASE));
	vmcs_writel(HOST_GS_BASE, read_msr(MSR_GS_BASE));
#else
	vmcs_writel(HOST_FS_BASE, segment_base(vmx->host_state.fs_sel));
	vmcs_writel(HOST_GS_BASE, segment_base(vmx->host_state.gs_sel));
#endif

#ifdef CONFIG_X86_64
	rdmsrl(MSR_KERNEL_GS_BASE, vmx->msr_host_kernel_gs_base);
	if (is_long_mode(&vmx->vcpu))
		wrmsrl(MSR_KERNEL_GS_BASE, vmx->msr_guest_kernel_gs_base);
#endif
	for (i = 0; i < vmx->save_nmsrs; ++i)
		kvm_set_shared_msr(vmx->guest_msrs[i].index,
				   vmx->guest_msrs[i].data,
				   vmx->guest_msrs[i].mask);
}

static void __vmx_load_host_state(struct vcpu_vmx *vmx)
{
	if (!vmx->host_state.loaded)
		return;

	++vmx->vcpu.stat.host_state_reload;
	vmx->host_state.loaded = 0;
#ifdef CONFIG_X86_64
	if (is_long_mode(&vmx->vcpu))
		rdmsrl(MSR_KERNEL_GS_BASE, vmx->msr_guest_kernel_gs_base);
#endif
	if (vmx->host_state.gs_ldt_reload_needed) {
		kvm_load_ldt(vmx->host_state.ldt_sel);
#ifdef CONFIG_X86_64
		load_gs_index(vmx->host_state.gs_sel);
#else
		loadsegment(gs, vmx->host_state.gs_sel);
#endif
	}
	if (vmx->host_state.fs_reload_needed)
		loadsegment(fs, vmx->host_state.fs_sel);
<<<<<<< HEAD
=======
#ifdef CONFIG_X86_64
	if (unlikely(vmx->host_state.ds_sel | vmx->host_state.es_sel)) {
		loadsegment(ds, vmx->host_state.ds_sel);
		loadsegment(es, vmx->host_state.es_sel);
	}
#endif
>>>>>>> refs/remotes/origin/master
	reload_tss();
#ifdef CONFIG_X86_64
	wrmsrl(MSR_KERNEL_GS_BASE, vmx->msr_host_kernel_gs_base);
#endif
<<<<<<< HEAD
<<<<<<< HEAD
	if (__thread_has_fpu(current))
=======
	if (user_has_fpu())
>>>>>>> refs/remotes/origin/cm-10.0
		clts();
=======
	/*
	 * If the FPU is not active (through the host task or
	 * the guest vcpu), then restore the cr0.TS bit.
	 */
	if (!user_has_fpu() && !vmx->vcpu.guest_fpu_loaded)
		stts();
>>>>>>> refs/remotes/origin/master
	load_gdt(&__get_cpu_var(host_gdt));
}

static void vmx_load_host_state(struct vcpu_vmx *vmx)
{
	preempt_disable();
	__vmx_load_host_state(vmx);
	preempt_enable();
}

/*
 * Switches to specified vcpu, until a matching vcpu_put(), but assumes
 * vcpu mutex is already taken.
 */
static void vmx_vcpu_load(struct kvm_vcpu *vcpu, int cpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u64 phys_addr = __pa(per_cpu(vmxarea, cpu));

	if (!vmm_exclusive)
		kvm_cpu_vmxon(phys_addr);
<<<<<<< HEAD
<<<<<<< HEAD
	else if (vcpu->cpu != cpu)
		vcpu_clear(vmx);

	if (per_cpu(current_vmcs, cpu) != vmx->vmcs) {
		per_cpu(current_vmcs, cpu) = vmx->vmcs;
		vmcs_load(vmx->vmcs);
	}

	if (vcpu->cpu != cpu) {
=======
=======
>>>>>>> refs/remotes/origin/master
	else if (vmx->loaded_vmcs->cpu != cpu)
		loaded_vmcs_clear(vmx->loaded_vmcs);

	if (per_cpu(current_vmcs, cpu) != vmx->loaded_vmcs->vmcs) {
		per_cpu(current_vmcs, cpu) = vmx->loaded_vmcs->vmcs;
		vmcs_load(vmx->loaded_vmcs->vmcs);
	}

	if (vmx->loaded_vmcs->cpu != cpu) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		struct desc_ptr *gdt = &__get_cpu_var(host_gdt);
		unsigned long sysenter_esp;

		kvm_make_request(KVM_REQ_TLB_FLUSH, vcpu);
		local_irq_disable();
<<<<<<< HEAD
<<<<<<< HEAD
		list_add(&vmx->local_vcpus_link,
			 &per_cpu(vcpus_on_cpu, cpu));
=======
		list_add(&vmx->loaded_vmcs->loaded_vmcss_on_cpu_link,
			 &per_cpu(loaded_vmcss_on_cpu, cpu));
>>>>>>> refs/remotes/origin/cm-10.0
=======
		crash_disable_local_vmclear(cpu);

		/*
		 * Read loaded_vmcs->cpu should be before fetching
		 * loaded_vmcs->loaded_vmcss_on_cpu_link.
		 * See the comments in __loaded_vmcs_clear().
		 */
		smp_rmb();

		list_add(&vmx->loaded_vmcs->loaded_vmcss_on_cpu_link,
			 &per_cpu(loaded_vmcss_on_cpu, cpu));
		crash_enable_local_vmclear(cpu);
>>>>>>> refs/remotes/origin/master
		local_irq_enable();

		/*
		 * Linux uses per-cpu TSS and GDT, so set these when switching
		 * processors.
		 */
		vmcs_writel(HOST_TR_BASE, kvm_read_tr_base()); /* 22.2.4 */
		vmcs_writel(HOST_GDTR_BASE, gdt->address);   /* 22.2.4 */

		rdmsrl(MSR_IA32_SYSENTER_ESP, sysenter_esp);
		vmcs_writel(HOST_IA32_SYSENTER_ESP, sysenter_esp); /* 22.2.3 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
		vmx->loaded_vmcs->cpu = cpu;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		vmx->loaded_vmcs->cpu = cpu;
>>>>>>> refs/remotes/origin/master
	}
}

static void vmx_vcpu_put(struct kvm_vcpu *vcpu)
{
	__vmx_load_host_state(to_vmx(vcpu));
	if (!vmm_exclusive) {
<<<<<<< HEAD
<<<<<<< HEAD
		__vcpu_clear(to_vmx(vcpu));
=======
		__loaded_vmcs_clear(to_vmx(vcpu)->loaded_vmcs);
		vcpu->cpu = -1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		__loaded_vmcs_clear(to_vmx(vcpu)->loaded_vmcs);
		vcpu->cpu = -1;
>>>>>>> refs/remotes/origin/master
		kvm_cpu_vmxoff();
	}
}

static void vmx_fpu_activate(struct kvm_vcpu *vcpu)
{
	ulong cr0;

	if (vcpu->fpu_active)
		return;
	vcpu->fpu_active = 1;
	cr0 = vmcs_readl(GUEST_CR0);
	cr0 &= ~(X86_CR0_TS | X86_CR0_MP);
	cr0 |= kvm_read_cr0_bits(vcpu, X86_CR0_TS | X86_CR0_MP);
	vmcs_writel(GUEST_CR0, cr0);
	update_exception_bitmap(vcpu);
	vcpu->arch.cr0_guest_owned_bits = X86_CR0_TS;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (is_guest_mode(vcpu))
		vcpu->arch.cr0_guest_owned_bits &=
			~get_vmcs12(vcpu)->cr0_guest_host_mask;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (is_guest_mode(vcpu))
		vcpu->arch.cr0_guest_owned_bits &=
			~get_vmcs12(vcpu)->cr0_guest_host_mask;
>>>>>>> refs/remotes/origin/master
	vmcs_writel(CR0_GUEST_HOST_MASK, ~vcpu->arch.cr0_guest_owned_bits);
}

static void vmx_decache_cr0_guest_bits(struct kvm_vcpu *vcpu);

<<<<<<< HEAD
<<<<<<< HEAD
static void vmx_fpu_deactivate(struct kvm_vcpu *vcpu)
{
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Return the cr0 value that a nested guest would read. This is a combination
 * of the real cr0 used to run the guest (guest_cr0), and the bits shadowed by
 * its hypervisor (cr0_read_shadow).
 */
static inline unsigned long nested_read_cr0(struct vmcs12 *fields)
{
	return (fields->guest_cr0 & ~fields->cr0_guest_host_mask) |
		(fields->cr0_read_shadow & fields->cr0_guest_host_mask);
}
static inline unsigned long nested_read_cr4(struct vmcs12 *fields)
{
	return (fields->guest_cr4 & ~fields->cr4_guest_host_mask) |
		(fields->cr4_read_shadow & fields->cr4_guest_host_mask);
}

static void vmx_fpu_deactivate(struct kvm_vcpu *vcpu)
{
	/* Note that there is no vcpu->fpu_active = 0 here. The caller must
	 * set this *before* calling this function.
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	vmx_decache_cr0_guest_bits(vcpu);
	vmcs_set_bits(GUEST_CR0, X86_CR0_TS | X86_CR0_MP);
	update_exception_bitmap(vcpu);
	vcpu->arch.cr0_guest_owned_bits = 0;
	vmcs_writel(CR0_GUEST_HOST_MASK, ~vcpu->arch.cr0_guest_owned_bits);
<<<<<<< HEAD
<<<<<<< HEAD
	vmcs_writel(CR0_READ_SHADOW, vcpu->arch.cr0);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (is_guest_mode(vcpu)) {
		/*
		 * L1's specified read shadow might not contain the TS bit,
		 * so now that we turned on shadowing of this bit, we need to
		 * set this bit of the shadow. Like in nested_vmx_run we need
		 * nested_read_cr0(vmcs12), but vmcs12->guest_cr0 is not yet
		 * up-to-date here because we just decached cr0.TS (and we'll
		 * only update vmcs12->guest_cr0 on nested exit).
		 */
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
		vmcs12->guest_cr0 = (vmcs12->guest_cr0 & ~X86_CR0_TS) |
			(vcpu->arch.cr0 & X86_CR0_TS);
		vmcs_writel(CR0_READ_SHADOW, nested_read_cr0(vmcs12));
	} else
		vmcs_writel(CR0_READ_SHADOW, vcpu->arch.cr0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static unsigned long vmx_get_rflags(struct kvm_vcpu *vcpu)
{
	unsigned long rflags, save_rflags;

	if (!test_bit(VCPU_EXREG_RFLAGS, (ulong *)&vcpu->arch.regs_avail)) {
		__set_bit(VCPU_EXREG_RFLAGS, (ulong *)&vcpu->arch.regs_avail);
		rflags = vmcs_readl(GUEST_RFLAGS);
		if (to_vmx(vcpu)->rmode.vm86_active) {
			rflags &= RMODE_GUEST_OWNED_EFLAGS_BITS;
			save_rflags = to_vmx(vcpu)->rmode.save_rflags;
			rflags |= save_rflags & ~RMODE_GUEST_OWNED_EFLAGS_BITS;
		}
		to_vmx(vcpu)->rflags = rflags;
	}
	return to_vmx(vcpu)->rflags;
}

static void vmx_set_rflags(struct kvm_vcpu *vcpu, unsigned long rflags)
{
	__set_bit(VCPU_EXREG_RFLAGS, (ulong *)&vcpu->arch.regs_avail);
<<<<<<< HEAD
	__clear_bit(VCPU_EXREG_CPL, (ulong *)&vcpu->arch.regs_avail);
=======
>>>>>>> refs/remotes/origin/master
	to_vmx(vcpu)->rflags = rflags;
	if (to_vmx(vcpu)->rmode.vm86_active) {
		to_vmx(vcpu)->rmode.save_rflags = rflags;
		rflags |= X86_EFLAGS_IOPL | X86_EFLAGS_VM;
	}
	vmcs_writel(GUEST_RFLAGS, rflags);
}

static u32 vmx_get_interrupt_shadow(struct kvm_vcpu *vcpu, int mask)
{
	u32 interruptibility = vmcs_read32(GUEST_INTERRUPTIBILITY_INFO);
	int ret = 0;

	if (interruptibility & GUEST_INTR_STATE_STI)
		ret |= KVM_X86_SHADOW_INT_STI;
	if (interruptibility & GUEST_INTR_STATE_MOV_SS)
		ret |= KVM_X86_SHADOW_INT_MOV_SS;

	return ret & mask;
}

static void vmx_set_interrupt_shadow(struct kvm_vcpu *vcpu, int mask)
{
	u32 interruptibility_old = vmcs_read32(GUEST_INTERRUPTIBILITY_INFO);
	u32 interruptibility = interruptibility_old;

	interruptibility &= ~(GUEST_INTR_STATE_STI | GUEST_INTR_STATE_MOV_SS);

	if (mask & KVM_X86_SHADOW_INT_MOV_SS)
		interruptibility |= GUEST_INTR_STATE_MOV_SS;
	else if (mask & KVM_X86_SHADOW_INT_STI)
		interruptibility |= GUEST_INTR_STATE_STI;

	if ((interruptibility != interruptibility_old))
		vmcs_write32(GUEST_INTERRUPTIBILITY_INFO, interruptibility);
}

static void skip_emulated_instruction(struct kvm_vcpu *vcpu)
{
	unsigned long rip;

	rip = kvm_rip_read(vcpu);
	rip += vmcs_read32(VM_EXIT_INSTRUCTION_LEN);
	kvm_rip_write(vcpu, rip);

	/* skipping an emulated instruction also counts */
	vmx_set_interrupt_shadow(vcpu, 0);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void vmx_clear_hlt(struct kvm_vcpu *vcpu)
{
	/* Ensure that we clear the HLT state in the VMCS.  We don't need to
	 * explicitly skip the instruction because if the HLT state is set, then
	 * the instruction is already executing and RIP has already been
	 * advanced. */
	if (!yield_on_hlt &&
	    vmcs_read32(GUEST_ACTIVITY_STATE) == GUEST_ACTIVITY_HLT)
		vmcs_write32(GUEST_ACTIVITY_STATE, GUEST_ACTIVITY_ACTIVE);
=======
/*
 * KVM wants to inject page-faults which it got to the guest. This function
 * checks whether in a nested guest, we need to inject them to L1 or L2.
 * This function assumes it is called with the exit reason in vmcs02 being
 * a #PF exception (this is the only case in which KVM injects a #PF when L2
 * is running).
 */
static int nested_pf_handled(struct kvm_vcpu *vcpu)
{
	struct vmcs12 *vmcs12 = get_vmcs12(vcpu);

	/* TODO: also check PFEC_MATCH/MASK, not just EB.PF. */
	if (!(vmcs12->exception_bitmap & (1u << PF_VECTOR)))
=======
/*
 * KVM wants to inject page-faults which it got to the guest. This function
 * checks whether in a nested guest, we need to inject them to L1 or L2.
 */
static int nested_vmx_check_exception(struct kvm_vcpu *vcpu, unsigned nr)
{
	struct vmcs12 *vmcs12 = get_vmcs12(vcpu);

	if (!(vmcs12->exception_bitmap & (1u << nr)))
>>>>>>> refs/remotes/origin/master
		return 0;

	nested_vmx_vmexit(vcpu);
	return 1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void vmx_queue_exception(struct kvm_vcpu *vcpu, unsigned nr,
				bool has_error_code, u32 error_code,
				bool reinject)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u32 intr_info = nr | INTR_INFO_VALID_MASK;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (nr == PF_VECTOR && is_guest_mode(vcpu) &&
		nested_pf_handled(vcpu))
		return;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!reinject && is_guest_mode(vcpu) &&
	    nested_vmx_check_exception(vcpu, nr))
		return;

>>>>>>> refs/remotes/origin/master
	if (has_error_code) {
		vmcs_write32(VM_ENTRY_EXCEPTION_ERROR_CODE, error_code);
		intr_info |= INTR_INFO_DELIVER_CODE_MASK;
	}

	if (vmx->rmode.vm86_active) {
		int inc_eip = 0;
		if (kvm_exception_is_soft(nr))
			inc_eip = vcpu->arch.event_exit_inst_len;
		if (kvm_inject_realmode_interrupt(vcpu, nr, inc_eip) != EMULATE_DONE)
			kvm_make_request(KVM_REQ_TRIPLE_FAULT, vcpu);
		return;
	}

	if (kvm_exception_is_soft(nr)) {
		vmcs_write32(VM_ENTRY_INSTRUCTION_LEN,
			     vmx->vcpu.arch.event_exit_inst_len);
		intr_info |= INTR_TYPE_SOFT_EXCEPTION;
	} else
		intr_info |= INTR_TYPE_HARD_EXCEPTION;

	vmcs_write32(VM_ENTRY_INTR_INFO_FIELD, intr_info);
<<<<<<< HEAD
<<<<<<< HEAD
	vmx_clear_hlt(vcpu);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static bool vmx_rdtscp_supported(void)
{
	return cpu_has_vmx_rdtscp();
}

<<<<<<< HEAD
=======
static bool vmx_invpcid_supported(void)
{
	return cpu_has_vmx_invpcid() && enable_ept;
}

>>>>>>> refs/remotes/origin/master
/*
 * Swap MSR entry in host/guest MSR entry array.
 */
static void move_msr_up(struct vcpu_vmx *vmx, int from, int to)
{
	struct shared_msr_entry tmp;

	tmp = vmx->guest_msrs[to];
	vmx->guest_msrs[to] = vmx->guest_msrs[from];
	vmx->guest_msrs[from] = tmp;
}

<<<<<<< HEAD
=======
static void vmx_set_msr_bitmap(struct kvm_vcpu *vcpu)
{
	unsigned long *msr_bitmap;

	if (irqchip_in_kernel(vcpu->kvm) && apic_x2apic_mode(vcpu->arch.apic)) {
		if (is_long_mode(vcpu))
			msr_bitmap = vmx_msr_bitmap_longmode_x2apic;
		else
			msr_bitmap = vmx_msr_bitmap_legacy_x2apic;
	} else {
		if (is_long_mode(vcpu))
			msr_bitmap = vmx_msr_bitmap_longmode;
		else
			msr_bitmap = vmx_msr_bitmap_legacy;
	}

	vmcs_write64(MSR_BITMAP, __pa(msr_bitmap));
}

>>>>>>> refs/remotes/origin/master
/*
 * Set up the vmcs to automatically save and restore system
 * msrs.  Don't touch the 64-bit msrs if the guest is in legacy
 * mode, as fiddling with msrs is very expensive.
 */
static void setup_msrs(struct vcpu_vmx *vmx)
{
	int save_nmsrs, index;
<<<<<<< HEAD
	unsigned long *msr_bitmap;

<<<<<<< HEAD
	vmx_load_host_state(vmx);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	save_nmsrs = 0;
#ifdef CONFIG_X86_64
	if (is_long_mode(&vmx->vcpu)) {
		index = __find_msr_index(vmx, MSR_SYSCALL_MASK);
		if (index >= 0)
			move_msr_up(vmx, index, save_nmsrs++);
		index = __find_msr_index(vmx, MSR_LSTAR);
		if (index >= 0)
			move_msr_up(vmx, index, save_nmsrs++);
		index = __find_msr_index(vmx, MSR_CSTAR);
		if (index >= 0)
			move_msr_up(vmx, index, save_nmsrs++);
		index = __find_msr_index(vmx, MSR_TSC_AUX);
		if (index >= 0 && vmx->rdtscp_enabled)
			move_msr_up(vmx, index, save_nmsrs++);
		/*
		 * MSR_STAR is only needed on long mode guests, and only
		 * if efer.sce is enabled.
		 */
		index = __find_msr_index(vmx, MSR_STAR);
		if ((index >= 0) && (vmx->vcpu.arch.efer & EFER_SCE))
			move_msr_up(vmx, index, save_nmsrs++);
	}
#endif
	index = __find_msr_index(vmx, MSR_EFER);
	if (index >= 0 && update_transition_efer(vmx, index))
		move_msr_up(vmx, index, save_nmsrs++);

	vmx->save_nmsrs = save_nmsrs;

<<<<<<< HEAD
	if (cpu_has_vmx_msr_bitmap()) {
		if (is_long_mode(&vmx->vcpu))
			msr_bitmap = vmx_msr_bitmap_longmode;
		else
			msr_bitmap = vmx_msr_bitmap_legacy;

		vmcs_write64(MSR_BITMAP, __pa(msr_bitmap));
	}
=======
	if (cpu_has_vmx_msr_bitmap())
		vmx_set_msr_bitmap(&vmx->vcpu);
>>>>>>> refs/remotes/origin/master
}

/*
 * reads and returns guest's timestamp counter "register"
 * guest_tsc = host_tsc + tsc_offset    -- 21.3
 */
static u64 guest_read_tsc(void)
{
	u64 host_tsc, tsc_offset;

	rdtscll(host_tsc);
	tsc_offset = vmcs_read64(TSC_OFFSET);
	return host_tsc + tsc_offset;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Empty call-back. Needs to be implemented when VMX enables the SET_TSC_KHZ
 * ioctl. In this case the call-back should update internal vmx state to make
 * the changes effective.
 */
static void vmx_set_tsc_khz(struct kvm_vcpu *vcpu, u32 user_tsc_khz)
{
	/* Nothing to do here */
=======
 * Like guest_read_tsc, but always returns L1's notion of the timestamp
 * counter, even if a nested guest (L2) is currently running.
 */
u64 vmx_read_l1_tsc(struct kvm_vcpu *vcpu)
{
	u64 host_tsc, tsc_offset;

	rdtscll(host_tsc);
=======
 * Like guest_read_tsc, but always returns L1's notion of the timestamp
 * counter, even if a nested guest (L2) is currently running.
 */
u64 vmx_read_l1_tsc(struct kvm_vcpu *vcpu, u64 host_tsc)
{
	u64 tsc_offset;

>>>>>>> refs/remotes/origin/master
	tsc_offset = is_guest_mode(vcpu) ?
		to_vmx(vcpu)->nested.vmcs01_tsc_offset :
		vmcs_read64(TSC_OFFSET);
	return host_tsc + tsc_offset;
}

/*
 * Engage any workarounds for mis-matched TSC rates.  Currently limited to
 * software catchup for faster rates on slower CPUs.
 */
static void vmx_set_tsc_khz(struct kvm_vcpu *vcpu, u32 user_tsc_khz, bool scale)
{
	if (!scale)
		return;

	if (user_tsc_khz > tsc_khz) {
		vcpu->arch.tsc_catchup = 1;
		vcpu->arch.tsc_always_catchup = 1;
	} else
		WARN(1, "user requested TSC rate below hardware speed\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static u64 vmx_read_tsc_offset(struct kvm_vcpu *vcpu)
{
	return vmcs_read64(TSC_OFFSET);
>>>>>>> refs/remotes/origin/master
}

/*
 * writes 'offset' into guest's timestamp counter offset register
 */
static void vmx_write_tsc_offset(struct kvm_vcpu *vcpu, u64 offset)
{
<<<<<<< HEAD
<<<<<<< HEAD
	vmcs_write64(TSC_OFFSET, offset);
}

static void vmx_adjust_tsc_offset(struct kvm_vcpu *vcpu, s64 adjustment)
{
	u64 offset = vmcs_read64(TSC_OFFSET);
	vmcs_write64(TSC_OFFSET, offset + adjustment);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (is_guest_mode(vcpu)) {
		/*
		 * We're here if L1 chose not to trap WRMSR to TSC. According
		 * to the spec, this should set L1's TSC; The offset that L1
		 * set for L2 remains unchanged, and still needs to be added
		 * to the newly set TSC to get L2's TSC.
		 */
		struct vmcs12 *vmcs12;
		to_vmx(vcpu)->nested.vmcs01_tsc_offset = offset;
		/* recalculate vmcs02.TSC_OFFSET: */
		vmcs12 = get_vmcs12(vcpu);
		vmcs_write64(TSC_OFFSET, offset +
			(nested_cpu_has(vmcs12, CPU_BASED_USE_TSC_OFFSETING) ?
			 vmcs12->tsc_offset : 0));
	} else {
<<<<<<< HEAD
=======
		trace_kvm_write_tsc_offset(vcpu->vcpu_id,
					   vmcs_read64(TSC_OFFSET), offset);
>>>>>>> refs/remotes/origin/master
		vmcs_write64(TSC_OFFSET, offset);
	}
}

static void vmx_adjust_tsc_offset(struct kvm_vcpu *vcpu, s64 adjustment, bool host)
{
	u64 offset = vmcs_read64(TSC_OFFSET);
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	vmcs_write64(TSC_OFFSET, offset + adjustment);
	if (is_guest_mode(vcpu)) {
		/* Even when running L2, the adjustment needs to apply to L1 */
		to_vmx(vcpu)->nested.vmcs01_tsc_offset += adjustment;
<<<<<<< HEAD
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	} else
		trace_kvm_write_tsc_offset(vcpu->vcpu_id, offset,
					   offset + adjustment);
>>>>>>> refs/remotes/origin/master
}

static u64 vmx_compute_tsc_offset(struct kvm_vcpu *vcpu, u64 target_tsc)
{
	return target_tsc - native_read_tsc();
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static bool guest_cpuid_has_vmx(struct kvm_vcpu *vcpu)
{
	struct kvm_cpuid_entry2 *best = kvm_find_cpuid_entry(vcpu, 1, 0);
	return best && (best->ecx & (1 << (X86_FEATURE_VMX & 31)));
}

/*
 * nested_vmx_allowed() checks whether a guest should be allowed to use VMX
 * instructions and MSRs (i.e., nested VMX). Nested VMX is disabled for
 * all guests if the "nested" module option is off, and can also be disabled
 * for a single guest by disabling its VMX cpuid bit.
 */
static inline bool nested_vmx_allowed(struct kvm_vcpu *vcpu)
{
	return nested && guest_cpuid_has_vmx(vcpu);
}

/*
 * nested_vmx_setup_ctls_msrs() sets up variables containing the values to be
 * returned for the various VMX controls MSRs when nested VMX is enabled.
 * The same values should also be used to verify that vmcs12 control fields are
 * valid during nested entry from L1 to L2.
 * Each of these control msrs has a low and high 32-bit half: A low bit is on
 * if the corresponding bit in the (32-bit) control field *must* be on, and a
 * bit in the high half is on if the corresponding bit in the control field
 * may be on. See also vmx_control_verify().
 * TODO: allow these variables to be modified (downgraded) by module options
 * or other means.
 */
static u32 nested_vmx_procbased_ctls_low, nested_vmx_procbased_ctls_high;
static u32 nested_vmx_secondary_ctls_low, nested_vmx_secondary_ctls_high;
static u32 nested_vmx_pinbased_ctls_low, nested_vmx_pinbased_ctls_high;
static u32 nested_vmx_exit_ctls_low, nested_vmx_exit_ctls_high;
static u32 nested_vmx_entry_ctls_low, nested_vmx_entry_ctls_high;
<<<<<<< HEAD
=======
static u32 nested_vmx_misc_low, nested_vmx_misc_high;
static u32 nested_vmx_ept_caps;
>>>>>>> refs/remotes/origin/master
static __init void nested_vmx_setup_ctls_msrs(void)
{
	/*
	 * Note that as a general rule, the high half of the MSRs (bits in
	 * the control fields which may be 1) should be initialized by the
	 * intersection of the underlying hardware's MSR (i.e., features which
	 * can be supported) and the list of features we want to expose -
	 * because they are known to be properly supported in our code.
	 * Also, usually, the low half of the MSRs (bits which must be 1) can
	 * be set to 0, meaning that L1 may turn off any of these bits. The
	 * reason is that if one of these bits is necessary, it will appear
	 * in vmcs01 and prepare_vmcs02, when it bitwise-or's the control
	 * fields of vmcs01 and vmcs02, will turn these bits off - and
	 * nested_vmx_exit_handled() will not pass related exits to L1.
	 * These rules have exceptions below.
	 */

	/* pin-based controls */
<<<<<<< HEAD
=======
	rdmsr(MSR_IA32_VMX_PINBASED_CTLS,
	      nested_vmx_pinbased_ctls_low, nested_vmx_pinbased_ctls_high);
>>>>>>> refs/remotes/origin/master
	/*
	 * According to the Intel spec, if bit 55 of VMX_BASIC is off (as it is
	 * in our case), bits 1, 2 and 4 (i.e., 0x16) must be 1 in this MSR.
	 */
<<<<<<< HEAD
	nested_vmx_pinbased_ctls_low = 0x16 ;
	nested_vmx_pinbased_ctls_high = 0x16 |
		PIN_BASED_EXT_INTR_MASK | PIN_BASED_NMI_EXITING |
		PIN_BASED_VIRTUAL_NMIS;

	/* exit controls */
	nested_vmx_exit_ctls_low = 0;
	/* Note that guest use of VM_EXIT_ACK_INTR_ON_EXIT is not supported. */
#ifdef CONFIG_X86_64
	nested_vmx_exit_ctls_high = VM_EXIT_HOST_ADDR_SPACE_SIZE;
#else
	nested_vmx_exit_ctls_high = 0;
#endif
=======
	nested_vmx_pinbased_ctls_low |= PIN_BASED_ALWAYSON_WITHOUT_TRUE_MSR;
	nested_vmx_pinbased_ctls_high &= PIN_BASED_EXT_INTR_MASK |
		PIN_BASED_NMI_EXITING | PIN_BASED_VIRTUAL_NMIS |
		PIN_BASED_VMX_PREEMPTION_TIMER;
	nested_vmx_pinbased_ctls_high |= PIN_BASED_ALWAYSON_WITHOUT_TRUE_MSR;

	/*
	 * Exit controls
	 * If bit 55 of VMX_BASIC is off, bits 0-8 and 10, 11, 13, 14, 16 and
	 * 17 must be 1.
	 */
	rdmsr(MSR_IA32_VMX_EXIT_CTLS,
		nested_vmx_exit_ctls_low, nested_vmx_exit_ctls_high);
	nested_vmx_exit_ctls_low = VM_EXIT_ALWAYSON_WITHOUT_TRUE_MSR;
	/* Note that guest use of VM_EXIT_ACK_INTR_ON_EXIT is not supported. */
	nested_vmx_exit_ctls_high &=
#ifdef CONFIG_X86_64
		VM_EXIT_HOST_ADDR_SPACE_SIZE |
#endif
		VM_EXIT_LOAD_IA32_PAT | VM_EXIT_SAVE_IA32_PAT |
		VM_EXIT_SAVE_VMX_PREEMPTION_TIMER;
	if (!(nested_vmx_pinbased_ctls_high & PIN_BASED_VMX_PREEMPTION_TIMER) ||
	    !(nested_vmx_exit_ctls_high & VM_EXIT_SAVE_VMX_PREEMPTION_TIMER)) {
		nested_vmx_exit_ctls_high &= ~VM_EXIT_SAVE_VMX_PREEMPTION_TIMER;
		nested_vmx_pinbased_ctls_high &= ~PIN_BASED_VMX_PREEMPTION_TIMER;
	}
	nested_vmx_exit_ctls_high |= (VM_EXIT_ALWAYSON_WITHOUT_TRUE_MSR |
		VM_EXIT_LOAD_IA32_EFER | VM_EXIT_SAVE_IA32_EFER);
>>>>>>> refs/remotes/origin/master

	/* entry controls */
	rdmsr(MSR_IA32_VMX_ENTRY_CTLS,
		nested_vmx_entry_ctls_low, nested_vmx_entry_ctls_high);
<<<<<<< HEAD
	nested_vmx_entry_ctls_low = 0;
	nested_vmx_entry_ctls_high &=
		VM_ENTRY_LOAD_IA32_PAT | VM_ENTRY_IA32E_MODE;
=======
	/* If bit 55 of VMX_BASIC is off, bits 0-8 and 12 must be 1. */
	nested_vmx_entry_ctls_low = VM_ENTRY_ALWAYSON_WITHOUT_TRUE_MSR;
	nested_vmx_entry_ctls_high &=
#ifdef CONFIG_X86_64
		VM_ENTRY_IA32E_MODE |
#endif
		VM_ENTRY_LOAD_IA32_PAT;
	nested_vmx_entry_ctls_high |= (VM_ENTRY_ALWAYSON_WITHOUT_TRUE_MSR |
				       VM_ENTRY_LOAD_IA32_EFER);
>>>>>>> refs/remotes/origin/master

	/* cpu-based controls */
	rdmsr(MSR_IA32_VMX_PROCBASED_CTLS,
		nested_vmx_procbased_ctls_low, nested_vmx_procbased_ctls_high);
	nested_vmx_procbased_ctls_low = 0;
	nested_vmx_procbased_ctls_high &=
<<<<<<< HEAD
		CPU_BASED_VIRTUAL_INTR_PENDING | CPU_BASED_USE_TSC_OFFSETING |
=======
		CPU_BASED_VIRTUAL_INTR_PENDING |
		CPU_BASED_VIRTUAL_NMI_PENDING | CPU_BASED_USE_TSC_OFFSETING |
>>>>>>> refs/remotes/origin/master
		CPU_BASED_HLT_EXITING | CPU_BASED_INVLPG_EXITING |
		CPU_BASED_MWAIT_EXITING | CPU_BASED_CR3_LOAD_EXITING |
		CPU_BASED_CR3_STORE_EXITING |
#ifdef CONFIG_X86_64
		CPU_BASED_CR8_LOAD_EXITING | CPU_BASED_CR8_STORE_EXITING |
#endif
		CPU_BASED_MOV_DR_EXITING | CPU_BASED_UNCOND_IO_EXITING |
		CPU_BASED_USE_IO_BITMAPS | CPU_BASED_MONITOR_EXITING |
<<<<<<< HEAD
		CPU_BASED_RDPMC_EXITING |
=======
		CPU_BASED_RDPMC_EXITING | CPU_BASED_RDTSC_EXITING |
		CPU_BASED_PAUSE_EXITING |
>>>>>>> refs/remotes/origin/master
		CPU_BASED_ACTIVATE_SECONDARY_CONTROLS;
	/*
	 * We can allow some features even when not supported by the
	 * hardware. For example, L1 can specify an MSR bitmap - and we
	 * can use it to avoid exits to L1 - even when L0 runs L2
	 * without MSR bitmaps.
	 */
	nested_vmx_procbased_ctls_high |= CPU_BASED_USE_MSR_BITMAPS;

	/* secondary cpu-based controls */
	rdmsr(MSR_IA32_VMX_PROCBASED_CTLS2,
		nested_vmx_secondary_ctls_low, nested_vmx_secondary_ctls_high);
	nested_vmx_secondary_ctls_low = 0;
	nested_vmx_secondary_ctls_high &=
<<<<<<< HEAD
		SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
=======
		SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES |
		SECONDARY_EXEC_UNRESTRICTED_GUEST |
		SECONDARY_EXEC_WBINVD_EXITING;

	if (enable_ept) {
		/* nested EPT: emulate EPT also to L1 */
		nested_vmx_secondary_ctls_high |= SECONDARY_EXEC_ENABLE_EPT;
		nested_vmx_ept_caps = VMX_EPT_PAGE_WALK_4_BIT |
			 VMX_EPTP_WB_BIT | VMX_EPT_2MB_PAGE_BIT |
			 VMX_EPT_INVEPT_BIT;
		nested_vmx_ept_caps &= vmx_capability.ept;
		/*
		 * Since invept is completely emulated we support both global
		 * and context invalidation independent of what host cpu
		 * supports
		 */
		nested_vmx_ept_caps |= VMX_EPT_EXTENT_GLOBAL_BIT |
			VMX_EPT_EXTENT_CONTEXT_BIT;
	} else
		nested_vmx_ept_caps = 0;

	/* miscellaneous data */
	rdmsr(MSR_IA32_VMX_MISC, nested_vmx_misc_low, nested_vmx_misc_high);
	nested_vmx_misc_low &= VMX_MISC_PREEMPTION_TIMER_RATE_MASK |
		VMX_MISC_SAVE_EFER_LMA;
	nested_vmx_misc_high = 0;
>>>>>>> refs/remotes/origin/master
}

static inline bool vmx_control_verify(u32 control, u32 low, u32 high)
{
	/*
	 * Bits 0 in high must be 0, and bits 1 in low must be 1.
	 */
	return ((control & high) | low) == control;
}

static inline u64 vmx_control_msr(u32 low, u32 high)
{
	return low | ((u64)high << 32);
}

/*
 * If we allow our guest to use VMX instructions (i.e., nested VMX), we should
 * also let it use VMX-specific MSRs.
 * vmx_get_vmx_msr() and vmx_set_vmx_msr() return 1 when we handled a
 * VMX-specific MSR, or 0 when we haven't (and the caller should handle it
 * like all other MSRs).
 */
static int vmx_get_vmx_msr(struct kvm_vcpu *vcpu, u32 msr_index, u64 *pdata)
{
	if (!nested_vmx_allowed(vcpu) && msr_index >= MSR_IA32_VMX_BASIC &&
		     msr_index <= MSR_IA32_VMX_TRUE_ENTRY_CTLS) {
		/*
		 * According to the spec, processors which do not support VMX
		 * should throw a #GP(0) when VMX capability MSRs are read.
		 */
		kvm_queue_exception_e(vcpu, GP_VECTOR, 0);
		return 1;
	}

	switch (msr_index) {
	case MSR_IA32_FEATURE_CONTROL:
<<<<<<< HEAD
		*pdata = 0;
		break;
=======
		if (nested_vmx_allowed(vcpu)) {
			*pdata = to_vmx(vcpu)->nested.msr_ia32_feature_control;
			break;
		}
		return 0;
>>>>>>> refs/remotes/origin/master
	case MSR_IA32_VMX_BASIC:
		/*
		 * This MSR reports some information about VMX support. We
		 * should return information about the VMX we emulate for the
		 * guest, and the VMCS structure we give it - not about the
		 * VMX support of the underlying hardware.
		 */
		*pdata = VMCS12_REVISION |
			   ((u64)VMCS12_SIZE << VMX_BASIC_VMCS_SIZE_SHIFT) |
			   (VMX_BASIC_MEM_TYPE_WB << VMX_BASIC_MEM_TYPE_SHIFT);
		break;
	case MSR_IA32_VMX_TRUE_PINBASED_CTLS:
	case MSR_IA32_VMX_PINBASED_CTLS:
		*pdata = vmx_control_msr(nested_vmx_pinbased_ctls_low,
					nested_vmx_pinbased_ctls_high);
		break;
	case MSR_IA32_VMX_TRUE_PROCBASED_CTLS:
	case MSR_IA32_VMX_PROCBASED_CTLS:
		*pdata = vmx_control_msr(nested_vmx_procbased_ctls_low,
					nested_vmx_procbased_ctls_high);
		break;
	case MSR_IA32_VMX_TRUE_EXIT_CTLS:
	case MSR_IA32_VMX_EXIT_CTLS:
		*pdata = vmx_control_msr(nested_vmx_exit_ctls_low,
					nested_vmx_exit_ctls_high);
		break;
	case MSR_IA32_VMX_TRUE_ENTRY_CTLS:
	case MSR_IA32_VMX_ENTRY_CTLS:
		*pdata = vmx_control_msr(nested_vmx_entry_ctls_low,
					nested_vmx_entry_ctls_high);
		break;
	case MSR_IA32_VMX_MISC:
<<<<<<< HEAD
		*pdata = 0;
=======
		*pdata = vmx_control_msr(nested_vmx_misc_low,
					 nested_vmx_misc_high);
>>>>>>> refs/remotes/origin/master
		break;
	/*
	 * These MSRs specify bits which the guest must keep fixed (on or off)
	 * while L1 is in VMXON mode (in L1's root mode, or running an L2).
	 * We picked the standard core2 setting.
	 */
#define VMXON_CR0_ALWAYSON	(X86_CR0_PE | X86_CR0_PG | X86_CR0_NE)
#define VMXON_CR4_ALWAYSON	X86_CR4_VMXE
	case MSR_IA32_VMX_CR0_FIXED0:
		*pdata = VMXON_CR0_ALWAYSON;
		break;
	case MSR_IA32_VMX_CR0_FIXED1:
		*pdata = -1ULL;
		break;
	case MSR_IA32_VMX_CR4_FIXED0:
		*pdata = VMXON_CR4_ALWAYSON;
		break;
	case MSR_IA32_VMX_CR4_FIXED1:
		*pdata = -1ULL;
		break;
	case MSR_IA32_VMX_VMCS_ENUM:
		*pdata = 0x1f;
		break;
	case MSR_IA32_VMX_PROCBASED_CTLS2:
		*pdata = vmx_control_msr(nested_vmx_secondary_ctls_low,
					nested_vmx_secondary_ctls_high);
		break;
	case MSR_IA32_VMX_EPT_VPID_CAP:
<<<<<<< HEAD
		/* Currently, no nested ept or nested vpid */
		*pdata = 0;
=======
		/* Currently, no nested vpid support */
		*pdata = nested_vmx_ept_caps;
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return 0;
	}

	return 1;
}

<<<<<<< HEAD
static int vmx_set_vmx_msr(struct kvm_vcpu *vcpu, u32 msr_index, u64 data)
{
	if (!nested_vmx_allowed(vcpu))
		return 0;

	if (msr_index == MSR_IA32_FEATURE_CONTROL)
		/* TODO: the right thing. */
		return 1;
=======
static int vmx_set_vmx_msr(struct kvm_vcpu *vcpu, struct msr_data *msr_info)
{
	u32 msr_index = msr_info->index;
	u64 data = msr_info->data;
	bool host_initialized = msr_info->host_initiated;

	if (!nested_vmx_allowed(vcpu))
		return 0;

	if (msr_index == MSR_IA32_FEATURE_CONTROL) {
		if (!host_initialized &&
				to_vmx(vcpu)->nested.msr_ia32_feature_control
				& FEATURE_CONTROL_LOCKED)
			return 0;
		to_vmx(vcpu)->nested.msr_ia32_feature_control = data;
		return 1;
	}

>>>>>>> refs/remotes/origin/master
	/*
	 * No need to treat VMX capability MSRs specially: If we don't handle
	 * them, handle_wrmsr will #GP(0), which is correct (they are readonly)
	 */
	return 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Reads an msr value (of 'msr_index') into 'pdata'.
 * Returns 0 on success, non-0 otherwise.
 * Assumes vcpu_load() was already called.
 */
static int vmx_get_msr(struct kvm_vcpu *vcpu, u32 msr_index, u64 *pdata)
{
	u64 data;
	struct shared_msr_entry *msr;

	if (!pdata) {
		printk(KERN_ERR "BUG: get_msr called with NULL pdata\n");
		return -EINVAL;
	}

	switch (msr_index) {
#ifdef CONFIG_X86_64
	case MSR_FS_BASE:
		data = vmcs_readl(GUEST_FS_BASE);
		break;
	case MSR_GS_BASE:
		data = vmcs_readl(GUEST_GS_BASE);
		break;
	case MSR_KERNEL_GS_BASE:
		vmx_load_host_state(to_vmx(vcpu));
		data = to_vmx(vcpu)->msr_guest_kernel_gs_base;
		break;
#endif
	case MSR_EFER:
		return kvm_get_msr_common(vcpu, msr_index, pdata);
	case MSR_IA32_TSC:
		data = guest_read_tsc();
		break;
	case MSR_IA32_SYSENTER_CS:
		data = vmcs_read32(GUEST_SYSENTER_CS);
		break;
	case MSR_IA32_SYSENTER_EIP:
		data = vmcs_readl(GUEST_SYSENTER_EIP);
		break;
	case MSR_IA32_SYSENTER_ESP:
		data = vmcs_readl(GUEST_SYSENTER_ESP);
		break;
	case MSR_TSC_AUX:
		if (!to_vmx(vcpu)->rdtscp_enabled)
			return 1;
		/* Otherwise falls through */
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		vmx_load_host_state(to_vmx(vcpu));
		msr = find_msr_entry(to_vmx(vcpu), msr_index);
		if (msr) {
			vmx_load_host_state(to_vmx(vcpu));
=======
=======
>>>>>>> refs/remotes/origin/master
		if (vmx_get_vmx_msr(vcpu, msr_index, pdata))
			return 0;
		msr = find_msr_entry(to_vmx(vcpu), msr_index);
		if (msr) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			data = msr->data;
			break;
		}
		return kvm_get_msr_common(vcpu, msr_index, pdata);
	}

	*pdata = data;
	return 0;
}

/*
 * Writes msr value into into the appropriate "register".
 * Returns 0 on success, non-0 otherwise.
 * Assumes vcpu_load() was already called.
 */
<<<<<<< HEAD
static int vmx_set_msr(struct kvm_vcpu *vcpu, u32 msr_index, u64 data)
=======
static int vmx_set_msr(struct kvm_vcpu *vcpu, struct msr_data *msr_info)
>>>>>>> refs/remotes/origin/master
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	struct shared_msr_entry *msr;
	int ret = 0;
<<<<<<< HEAD

	switch (msr_index) {
	case MSR_EFER:
<<<<<<< HEAD
		vmx_load_host_state(vmx);
=======
>>>>>>> refs/remotes/origin/cm-10.0
		ret = kvm_set_msr_common(vcpu, msr_index, data);
=======
	u32 msr_index = msr_info->index;
	u64 data = msr_info->data;

	switch (msr_index) {
	case MSR_EFER:
		ret = kvm_set_msr_common(vcpu, msr_info);
>>>>>>> refs/remotes/origin/master
		break;
#ifdef CONFIG_X86_64
	case MSR_FS_BASE:
		vmx_segment_cache_clear(vmx);
		vmcs_writel(GUEST_FS_BASE, data);
		break;
	case MSR_GS_BASE:
		vmx_segment_cache_clear(vmx);
		vmcs_writel(GUEST_GS_BASE, data);
		break;
	case MSR_KERNEL_GS_BASE:
		vmx_load_host_state(vmx);
		vmx->msr_guest_kernel_gs_base = data;
		break;
#endif
	case MSR_IA32_SYSENTER_CS:
		vmcs_write32(GUEST_SYSENTER_CS, data);
		break;
	case MSR_IA32_SYSENTER_EIP:
		vmcs_writel(GUEST_SYSENTER_EIP, data);
		break;
	case MSR_IA32_SYSENTER_ESP:
		vmcs_writel(GUEST_SYSENTER_ESP, data);
		break;
	case MSR_IA32_TSC:
<<<<<<< HEAD
		kvm_write_tsc(vcpu, data);
=======
		kvm_write_tsc(vcpu, msr_info);
>>>>>>> refs/remotes/origin/master
		break;
	case MSR_IA32_CR_PAT:
		if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT) {
			vmcs_write64(GUEST_IA32_PAT, data);
			vcpu->arch.pat = data;
			break;
		}
<<<<<<< HEAD
		ret = kvm_set_msr_common(vcpu, msr_index, data);
=======
		ret = kvm_set_msr_common(vcpu, msr_info);
		break;
	case MSR_IA32_TSC_ADJUST:
		ret = kvm_set_msr_common(vcpu, msr_info);
>>>>>>> refs/remotes/origin/master
		break;
	case MSR_TSC_AUX:
		if (!vmx->rdtscp_enabled)
			return 1;
		/* Check reserved bit, higher 32 bits should be zero */
		if ((data >> 32) != 0)
			return 1;
		/* Otherwise falls through */
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		msr = find_msr_entry(vmx, msr_index);
		if (msr) {
			vmx_load_host_state(vmx);
			msr->data = data;
=======
		if (vmx_set_vmx_msr(vcpu, msr_index, data))
=======
		if (vmx_set_vmx_msr(vcpu, msr_info))
>>>>>>> refs/remotes/origin/master
			break;
		msr = find_msr_entry(vmx, msr_index);
		if (msr) {
			msr->data = data;
			if (msr - vmx->guest_msrs < vmx->save_nmsrs) {
				preempt_disable();
				kvm_set_shared_msr(msr->index, msr->data,
						   msr->mask);
				preempt_enable();
			}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		}
		ret = kvm_set_msr_common(vcpu, msr_index, data);
=======
			break;
		}
		ret = kvm_set_msr_common(vcpu, msr_info);
>>>>>>> refs/remotes/origin/master
	}

	return ret;
}

static void vmx_cache_reg(struct kvm_vcpu *vcpu, enum kvm_reg reg)
{
	__set_bit(reg, (unsigned long *)&vcpu->arch.regs_avail);
	switch (reg) {
	case VCPU_REGS_RSP:
		vcpu->arch.regs[VCPU_REGS_RSP] = vmcs_readl(GUEST_RSP);
		break;
	case VCPU_REGS_RIP:
		vcpu->arch.regs[VCPU_REGS_RIP] = vmcs_readl(GUEST_RIP);
		break;
	case VCPU_EXREG_PDPTR:
		if (enable_ept)
			ept_save_pdptrs(vcpu);
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
static void set_guest_debug(struct kvm_vcpu *vcpu, struct kvm_guest_debug *dbg)
{
	if (vcpu->guest_debug & KVM_GUESTDBG_USE_HW_BP)
		vmcs_writel(GUEST_DR7, dbg->arch.debugreg[7]);
	else
		vmcs_writel(GUEST_DR7, vcpu->arch.dr7);

	update_exception_bitmap(vcpu);
}

=======
>>>>>>> refs/remotes/origin/master
static __init int cpu_has_kvm_support(void)
{
	return cpu_has_vmx();
}

static __init int vmx_disabled_by_bios(void)
{
	u64 msr;

	rdmsrl(MSR_IA32_FEATURE_CONTROL, msr);
	if (msr & FEATURE_CONTROL_LOCKED) {
		/* launched w/ TXT and VMX disabled */
		if (!(msr & FEATURE_CONTROL_VMXON_ENABLED_INSIDE_SMX)
			&& tboot_enabled())
			return 1;
		/* launched w/o TXT and VMX only enabled w/ TXT */
		if (!(msr & FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX)
			&& (msr & FEATURE_CONTROL_VMXON_ENABLED_INSIDE_SMX)
			&& !tboot_enabled()) {
			printk(KERN_WARNING "kvm: disable TXT in the BIOS or "
				"activate TXT before enabling KVM\n");
			return 1;
		}
		/* launched w/o TXT and VMX disabled */
		if (!(msr & FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX)
			&& !tboot_enabled())
			return 1;
	}

	return 0;
}

static void kvm_cpu_vmxon(u64 addr)
{
	asm volatile (ASM_VMX_VMXON_RAX
			: : "a"(&addr), "m"(addr)
			: "memory", "cc");
}

static int hardware_enable(void *garbage)
{
	int cpu = raw_smp_processor_id();
	u64 phys_addr = __pa(per_cpu(vmxarea, cpu));
	u64 old, test_bits;

	if (read_cr4() & X86_CR4_VMXE)
		return -EBUSY;

<<<<<<< HEAD
<<<<<<< HEAD
	INIT_LIST_HEAD(&per_cpu(vcpus_on_cpu, cpu));
=======
	INIT_LIST_HEAD(&per_cpu(loaded_vmcss_on_cpu, cpu));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_LIST_HEAD(&per_cpu(loaded_vmcss_on_cpu, cpu));

	/*
	 * Now we can enable the vmclear operation in kdump
	 * since the loaded_vmcss_on_cpu list on this cpu
	 * has been initialized.
	 *
	 * Though the cpu is not in VMX operation now, there
	 * is no problem to enable the vmclear operation
	 * for the loaded_vmcss_on_cpu list is empty!
	 */
	crash_enable_local_vmclear(cpu);

>>>>>>> refs/remotes/origin/master
	rdmsrl(MSR_IA32_FEATURE_CONTROL, old);

	test_bits = FEATURE_CONTROL_LOCKED;
	test_bits |= FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX;
	if (tboot_enabled())
		test_bits |= FEATURE_CONTROL_VMXON_ENABLED_INSIDE_SMX;

	if ((old & test_bits) != test_bits) {
		/* enable and lock */
		wrmsrl(MSR_IA32_FEATURE_CONTROL, old | test_bits);
	}
	write_cr4(read_cr4() | X86_CR4_VMXE); /* FIXME: not cpu hotplug safe */

	if (vmm_exclusive) {
		kvm_cpu_vmxon(phys_addr);
		ept_sync_global();
	}

<<<<<<< HEAD
	store_gdt(&__get_cpu_var(host_gdt));
=======
	native_store_gdt(&__get_cpu_var(host_gdt));
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void vmclear_local_vcpus(void)
{
	int cpu = raw_smp_processor_id();
	struct vcpu_vmx *vmx, *n;

	list_for_each_entry_safe(vmx, n, &per_cpu(vcpus_on_cpu, cpu),
				 local_vcpus_link)
		__vcpu_clear(vmx);
=======
=======
>>>>>>> refs/remotes/origin/master
static void vmclear_local_loaded_vmcss(void)
{
	int cpu = raw_smp_processor_id();
	struct loaded_vmcs *v, *n;

	list_for_each_entry_safe(v, n, &per_cpu(loaded_vmcss_on_cpu, cpu),
				 loaded_vmcss_on_cpu_link)
		__loaded_vmcs_clear(v);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}


/* Just like cpu_vmxoff(), but with the __kvm_handle_fault_on_reboot()
 * tricks.
 */
static void kvm_cpu_vmxoff(void)
{
	asm volatile (__ex(ASM_VMX_VMXOFF) : : : "cc");
}

static void hardware_disable(void *garbage)
{
	if (vmm_exclusive) {
<<<<<<< HEAD
<<<<<<< HEAD
		vmclear_local_vcpus();
=======
		vmclear_local_loaded_vmcss();
>>>>>>> refs/remotes/origin/cm-10.0
=======
		vmclear_local_loaded_vmcss();
>>>>>>> refs/remotes/origin/master
		kvm_cpu_vmxoff();
	}
	write_cr4(read_cr4() & ~X86_CR4_VMXE);
}

static __init int adjust_vmx_controls(u32 ctl_min, u32 ctl_opt,
				      u32 msr, u32 *result)
{
	u32 vmx_msr_low, vmx_msr_high;
	u32 ctl = ctl_min | ctl_opt;

	rdmsr(msr, vmx_msr_low, vmx_msr_high);

	ctl &= vmx_msr_high; /* bit == 0 in high word ==> must be zero */
	ctl |= vmx_msr_low;  /* bit == 1 in low word  ==> must be one  */

	/* Ensure minimum (required) set of control bits are supported. */
	if (ctl_min & ~ctl)
		return -EIO;

	*result = ctl;
	return 0;
}

static __init bool allow_1_setting(u32 msr, u32 ctl)
{
	u32 vmx_msr_low, vmx_msr_high;

	rdmsr(msr, vmx_msr_low, vmx_msr_high);
	return vmx_msr_high & ctl;
}

static __init int setup_vmcs_config(struct vmcs_config *vmcs_conf)
{
	u32 vmx_msr_low, vmx_msr_high;
	u32 min, opt, min2, opt2;
	u32 _pin_based_exec_control = 0;
	u32 _cpu_based_exec_control = 0;
	u32 _cpu_based_2nd_exec_control = 0;
	u32 _vmexit_control = 0;
	u32 _vmentry_control = 0;

<<<<<<< HEAD
	min = PIN_BASED_EXT_INTR_MASK | PIN_BASED_NMI_EXITING;
	opt = PIN_BASED_VIRTUAL_NMIS;
	if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_PINBASED_CTLS,
				&_pin_based_exec_control) < 0)
		return -EIO;

<<<<<<< HEAD
	min =
=======
	min = CPU_BASED_HLT_EXITING |
>>>>>>> refs/remotes/origin/cm-10.0
=======
	min = CPU_BASED_HLT_EXITING |
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_X86_64
	      CPU_BASED_CR8_LOAD_EXITING |
	      CPU_BASED_CR8_STORE_EXITING |
#endif
	      CPU_BASED_CR3_LOAD_EXITING |
	      CPU_BASED_CR3_STORE_EXITING |
	      CPU_BASED_USE_IO_BITMAPS |
	      CPU_BASED_MOV_DR_EXITING |
	      CPU_BASED_USE_TSC_OFFSETING |
	      CPU_BASED_MWAIT_EXITING |
	      CPU_BASED_MONITOR_EXITING |
<<<<<<< HEAD
<<<<<<< HEAD
	      CPU_BASED_INVLPG_EXITING;

	if (yield_on_hlt)
		min |= CPU_BASED_HLT_EXITING;
=======
	      CPU_BASED_INVLPG_EXITING |
	      CPU_BASED_RDPMC_EXITING;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	      CPU_BASED_INVLPG_EXITING |
	      CPU_BASED_RDPMC_EXITING;
>>>>>>> refs/remotes/origin/master

	opt = CPU_BASED_TPR_SHADOW |
	      CPU_BASED_USE_MSR_BITMAPS |
	      CPU_BASED_ACTIVATE_SECONDARY_CONTROLS;
	if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_PROCBASED_CTLS,
				&_cpu_based_exec_control) < 0)
		return -EIO;
#ifdef CONFIG_X86_64
	if ((_cpu_based_exec_control & CPU_BASED_TPR_SHADOW))
		_cpu_based_exec_control &= ~CPU_BASED_CR8_LOAD_EXITING &
					   ~CPU_BASED_CR8_STORE_EXITING;
#endif
	if (_cpu_based_exec_control & CPU_BASED_ACTIVATE_SECONDARY_CONTROLS) {
		min2 = 0;
		opt2 = SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES |
<<<<<<< HEAD
=======
			SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE |
>>>>>>> refs/remotes/origin/master
			SECONDARY_EXEC_WBINVD_EXITING |
			SECONDARY_EXEC_ENABLE_VPID |
			SECONDARY_EXEC_ENABLE_EPT |
			SECONDARY_EXEC_UNRESTRICTED_GUEST |
			SECONDARY_EXEC_PAUSE_LOOP_EXITING |
<<<<<<< HEAD
			SECONDARY_EXEC_RDTSCP;
=======
			SECONDARY_EXEC_RDTSCP |
			SECONDARY_EXEC_ENABLE_INVPCID |
			SECONDARY_EXEC_APIC_REGISTER_VIRT |
			SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY |
			SECONDARY_EXEC_SHADOW_VMCS;
>>>>>>> refs/remotes/origin/master
		if (adjust_vmx_controls(min2, opt2,
					MSR_IA32_VMX_PROCBASED_CTLS2,
					&_cpu_based_2nd_exec_control) < 0)
			return -EIO;
	}
#ifndef CONFIG_X86_64
	if (!(_cpu_based_2nd_exec_control &
				SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES))
		_cpu_based_exec_control &= ~CPU_BASED_TPR_SHADOW;
#endif
<<<<<<< HEAD
=======

	if (!(_cpu_based_exec_control & CPU_BASED_TPR_SHADOW))
		_cpu_based_2nd_exec_control &= ~(
				SECONDARY_EXEC_APIC_REGISTER_VIRT |
				SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE |
				SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY);

>>>>>>> refs/remotes/origin/master
	if (_cpu_based_2nd_exec_control & SECONDARY_EXEC_ENABLE_EPT) {
		/* CR3 accesses and invlpg don't need to cause VM Exits when EPT
		   enabled */
		_cpu_based_exec_control &= ~(CPU_BASED_CR3_LOAD_EXITING |
					     CPU_BASED_CR3_STORE_EXITING |
					     CPU_BASED_INVLPG_EXITING);
		rdmsr(MSR_IA32_VMX_EPT_VPID_CAP,
		      vmx_capability.ept, vmx_capability.vpid);
	}

	min = 0;
#ifdef CONFIG_X86_64
	min |= VM_EXIT_HOST_ADDR_SPACE_SIZE;
#endif
<<<<<<< HEAD
	opt = VM_EXIT_SAVE_IA32_PAT | VM_EXIT_LOAD_IA32_PAT;
=======
	opt = VM_EXIT_SAVE_IA32_PAT | VM_EXIT_LOAD_IA32_PAT |
		VM_EXIT_ACK_INTR_ON_EXIT;
>>>>>>> refs/remotes/origin/master
	if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_EXIT_CTLS,
				&_vmexit_control) < 0)
		return -EIO;

<<<<<<< HEAD
=======
	min = PIN_BASED_EXT_INTR_MASK | PIN_BASED_NMI_EXITING;
	opt = PIN_BASED_VIRTUAL_NMIS | PIN_BASED_POSTED_INTR;
	if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_PINBASED_CTLS,
				&_pin_based_exec_control) < 0)
		return -EIO;

	if (!(_cpu_based_2nd_exec_control &
		SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY) ||
		!(_vmexit_control & VM_EXIT_ACK_INTR_ON_EXIT))
		_pin_based_exec_control &= ~PIN_BASED_POSTED_INTR;

>>>>>>> refs/remotes/origin/master
	min = 0;
	opt = VM_ENTRY_LOAD_IA32_PAT;
	if (adjust_vmx_controls(min, opt, MSR_IA32_VMX_ENTRY_CTLS,
				&_vmentry_control) < 0)
		return -EIO;

	rdmsr(MSR_IA32_VMX_BASIC, vmx_msr_low, vmx_msr_high);

	/* IA-32 SDM Vol 3B: VMCS size is never greater than 4kB. */
	if ((vmx_msr_high & 0x1fff) > PAGE_SIZE)
		return -EIO;

#ifdef CONFIG_X86_64
	/* IA-32 SDM Vol 3B: 64-bit CPUs always have VMX_BASIC_MSR[48]==0. */
	if (vmx_msr_high & (1u<<16))
		return -EIO;
#endif

	/* Require Write-Back (WB) memory type for VMCS accesses. */
	if (((vmx_msr_high >> 18) & 15) != 6)
		return -EIO;

	vmcs_conf->size = vmx_msr_high & 0x1fff;
	vmcs_conf->order = get_order(vmcs_config.size);
	vmcs_conf->revision_id = vmx_msr_low;

	vmcs_conf->pin_based_exec_ctrl = _pin_based_exec_control;
	vmcs_conf->cpu_based_exec_ctrl = _cpu_based_exec_control;
	vmcs_conf->cpu_based_2nd_exec_ctrl = _cpu_based_2nd_exec_control;
	vmcs_conf->vmexit_ctrl         = _vmexit_control;
	vmcs_conf->vmentry_ctrl        = _vmentry_control;

	cpu_has_load_ia32_efer =
		allow_1_setting(MSR_IA32_VMX_ENTRY_CTLS,
				VM_ENTRY_LOAD_IA32_EFER)
		&& allow_1_setting(MSR_IA32_VMX_EXIT_CTLS,
				   VM_EXIT_LOAD_IA32_EFER);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	cpu_has_load_perf_global_ctrl =
		allow_1_setting(MSR_IA32_VMX_ENTRY_CTLS,
				VM_ENTRY_LOAD_IA32_PERF_GLOBAL_CTRL)
		&& allow_1_setting(MSR_IA32_VMX_EXIT_CTLS,
				   VM_EXIT_LOAD_IA32_PERF_GLOBAL_CTRL);

	/*
	 * Some cpus support VM_ENTRY_(LOAD|SAVE)_IA32_PERF_GLOBAL_CTRL
	 * but due to arrata below it can't be used. Workaround is to use
	 * msr load mechanism to switch IA32_PERF_GLOBAL_CTRL.
	 *
	 * VM Exit May Incorrectly Clear IA32_PERF_GLOBAL_CTRL [34:32]
	 *
	 * AAK155             (model 26)
	 * AAP115             (model 30)
	 * AAT100             (model 37)
	 * BC86,AAY89,BD102   (model 44)
	 * BA97               (model 46)
	 *
	 */
	if (cpu_has_load_perf_global_ctrl && boot_cpu_data.x86 == 0x6) {
		switch (boot_cpu_data.x86_model) {
		case 26:
		case 30:
		case 37:
		case 44:
		case 46:
			cpu_has_load_perf_global_ctrl = false;
			printk_once(KERN_WARNING"kvm: VM_EXIT_LOAD_IA32_PERF_GLOBAL_CTRL "
					"does not work properly. Using workaround\n");
			break;
		default:
			break;
		}
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct vmcs *alloc_vmcs_cpu(int cpu)
{
	int node = cpu_to_node(cpu);
	struct page *pages;
	struct vmcs *vmcs;

	pages = alloc_pages_exact_node(node, GFP_KERNEL, vmcs_config.order);
	if (!pages)
		return NULL;
	vmcs = page_address(pages);
	memset(vmcs, 0, vmcs_config.size);
	vmcs->revision_id = vmcs_config.revision_id; /* vmcs revision id */
	return vmcs;
}

static struct vmcs *alloc_vmcs(void)
{
	return alloc_vmcs_cpu(raw_smp_processor_id());
}

static void free_vmcs(struct vmcs *vmcs)
{
	free_pages((unsigned long)vmcs, vmcs_config.order);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Free a VMCS, but before that VMCLEAR it on the CPU where it was last loaded
 */
static void free_loaded_vmcs(struct loaded_vmcs *loaded_vmcs)
{
	if (!loaded_vmcs->vmcs)
		return;
	loaded_vmcs_clear(loaded_vmcs);
	free_vmcs(loaded_vmcs->vmcs);
	loaded_vmcs->vmcs = NULL;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void free_kvm_area(void)
{
	int cpu;

	for_each_possible_cpu(cpu) {
		free_vmcs(per_cpu(vmxarea, cpu));
		per_cpu(vmxarea, cpu) = NULL;
	}
}

static __init int alloc_kvm_area(void)
{
	int cpu;

	for_each_possible_cpu(cpu) {
		struct vmcs *vmcs;

		vmcs = alloc_vmcs_cpu(cpu);
		if (!vmcs) {
			free_kvm_area();
			return -ENOMEM;
		}

		per_cpu(vmxarea, cpu) = vmcs;
	}
	return 0;
}

static __init int hardware_setup(void)
{
	if (setup_vmcs_config(&vmcs_config) < 0)
		return -EIO;

	if (boot_cpu_has(X86_FEATURE_NX))
		kvm_enable_efer_bits(EFER_NX);

	if (!cpu_has_vmx_vpid())
		enable_vpid = 0;
<<<<<<< HEAD
=======
	if (!cpu_has_vmx_shadow_vmcs())
		enable_shadow_vmcs = 0;
>>>>>>> refs/remotes/origin/master

	if (!cpu_has_vmx_ept() ||
	    !cpu_has_vmx_ept_4levels()) {
		enable_ept = 0;
		enable_unrestricted_guest = 0;
<<<<<<< HEAD
	}

=======
		enable_ept_ad_bits = 0;
	}

	if (!cpu_has_vmx_ept_ad_bits())
		enable_ept_ad_bits = 0;

>>>>>>> refs/remotes/origin/master
	if (!cpu_has_vmx_unrestricted_guest())
		enable_unrestricted_guest = 0;

	if (!cpu_has_vmx_flexpriority())
		flexpriority_enabled = 0;

	if (!cpu_has_vmx_tpr_shadow())
		kvm_x86_ops->update_cr8_intercept = NULL;

	if (enable_ept && !cpu_has_vmx_ept_2m_page())
		kvm_disable_largepages();

	if (!cpu_has_vmx_ple())
		ple_gap = 0;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (nested)
		nested_vmx_setup_ctls_msrs();

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!cpu_has_vmx_apicv())
		enable_apicv = 0;

	if (enable_apicv)
		kvm_x86_ops->update_cr8_intercept = NULL;
	else {
		kvm_x86_ops->hwapic_irr_update = NULL;
		kvm_x86_ops->deliver_posted_interrupt = NULL;
		kvm_x86_ops->sync_pir_to_irr = vmx_sync_pir_to_irr_dummy;
	}

	if (nested)
		nested_vmx_setup_ctls_msrs();

>>>>>>> refs/remotes/origin/master
	return alloc_kvm_area();
}

static __exit void hardware_unsetup(void)
{
	free_kvm_area();
}

<<<<<<< HEAD
static void fix_pmode_dataseg(int seg, struct kvm_save_segment *save)
{
	struct kvm_vmx_segment_field *sf = &kvm_vmx_segment_fields[seg];

	if (vmcs_readl(sf->base) == save->base && (save->base & AR_S_MASK)) {
		vmcs_write16(sf->selector, save->selector);
		vmcs_writel(sf->base, save->base);
		vmcs_write32(sf->limit, save->limit);
		vmcs_write32(sf->ar_bytes, save->ar);
	} else {
		u32 dpl = (vmcs_read16(sf->selector) & SELECTOR_RPL_MASK)
			<< AR_DPL_SHIFT;
		vmcs_write32(sf->ar_bytes, 0x93 | dpl);
	}
=======
static bool emulation_required(struct kvm_vcpu *vcpu)
{
	return emulate_invalid_guest_state && !guest_state_valid(vcpu);
}

static void fix_pmode_seg(struct kvm_vcpu *vcpu, int seg,
		struct kvm_segment *save)
{
	if (!emulate_invalid_guest_state) {
		/*
		 * CS and SS RPL should be equal during guest entry according
		 * to VMX spec, but in reality it is not always so. Since vcpu
		 * is in the middle of the transition from real mode to
		 * protected mode it is safe to assume that RPL 0 is a good
		 * default value.
		 */
		if (seg == VCPU_SREG_CS || seg == VCPU_SREG_SS)
			save->selector &= ~SELECTOR_RPL_MASK;
		save->dpl = save->selector & SELECTOR_RPL_MASK;
		save->s = 1;
	}
	vmx_set_segment(vcpu, save, seg);
>>>>>>> refs/remotes/origin/master
}

static void enter_pmode(struct kvm_vcpu *vcpu)
{
	unsigned long flags;
	struct vcpu_vmx *vmx = to_vmx(vcpu);

<<<<<<< HEAD
	vmx->emulation_required = 1;
=======
	/*
	 * Update real mode segment cache. It may be not up-to-date if sement
	 * register was written while vcpu was in a guest mode.
	 */
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_ES], VCPU_SREG_ES);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_DS], VCPU_SREG_DS);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_FS], VCPU_SREG_FS);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_GS], VCPU_SREG_GS);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_SS], VCPU_SREG_SS);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_CS], VCPU_SREG_CS);

>>>>>>> refs/remotes/origin/master
	vmx->rmode.vm86_active = 0;

	vmx_segment_cache_clear(vmx);

<<<<<<< HEAD
	vmcs_write16(GUEST_TR_SELECTOR, vmx->rmode.tr.selector);
	vmcs_writel(GUEST_TR_BASE, vmx->rmode.tr.base);
	vmcs_write32(GUEST_TR_LIMIT, vmx->rmode.tr.limit);
	vmcs_write32(GUEST_TR_AR_BYTES, vmx->rmode.tr.ar);
=======
	vmx_set_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_TR], VCPU_SREG_TR);
>>>>>>> refs/remotes/origin/master

	flags = vmcs_readl(GUEST_RFLAGS);
	flags &= RMODE_GUEST_OWNED_EFLAGS_BITS;
	flags |= vmx->rmode.save_rflags & ~RMODE_GUEST_OWNED_EFLAGS_BITS;
	vmcs_writel(GUEST_RFLAGS, flags);

	vmcs_writel(GUEST_CR4, (vmcs_readl(GUEST_CR4) & ~X86_CR4_VME) |
			(vmcs_readl(CR4_READ_SHADOW) & X86_CR4_VME));

	update_exception_bitmap(vcpu);

<<<<<<< HEAD
	if (emulate_invalid_guest_state)
		return;

	fix_pmode_dataseg(VCPU_SREG_ES, &vmx->rmode.es);
	fix_pmode_dataseg(VCPU_SREG_DS, &vmx->rmode.ds);
	fix_pmode_dataseg(VCPU_SREG_GS, &vmx->rmode.gs);
	fix_pmode_dataseg(VCPU_SREG_FS, &vmx->rmode.fs);

	vmx_segment_cache_clear(vmx);

	vmcs_write16(GUEST_SS_SELECTOR, 0);
	vmcs_write32(GUEST_SS_AR_BYTES, 0x93);

	vmcs_write16(GUEST_CS_SELECTOR,
		     vmcs_read16(GUEST_CS_SELECTOR) & ~SELECTOR_RPL_MASK);
	vmcs_write32(GUEST_CS_AR_BYTES, 0x9b);
}

static gva_t rmode_tss_base(struct kvm *kvm)
{
	if (!kvm->arch.tss_addr) {
		struct kvm_memslots *slots;
<<<<<<< HEAD
		gfn_t base_gfn;

		slots = kvm_memslots(kvm);
		base_gfn = slots->memslots[0].base_gfn +
				 kvm->memslots->memslots[0].npages - 3;
=======
		struct kvm_memory_slot *slot;
		gfn_t base_gfn;

		slots = kvm_memslots(kvm);
		slot = id_to_memslot(slots, 0);
		base_gfn = slot->base_gfn + slot->npages - 3;

>>>>>>> refs/remotes/origin/cm-10.0
		return base_gfn << PAGE_SHIFT;
	}
	return kvm->arch.tss_addr;
}

static void fix_rmode_seg(int seg, struct kvm_save_segment *save)
{
	struct kvm_vmx_segment_field *sf = &kvm_vmx_segment_fields[seg];

	save->selector = vmcs_read16(sf->selector);
	save->base = vmcs_readl(sf->base);
	save->limit = vmcs_read32(sf->limit);
	save->ar = vmcs_read32(sf->ar_bytes);
	vmcs_write16(sf->selector, save->base >> 4);
	vmcs_write32(sf->base, save->base & 0xffff0);
	vmcs_write32(sf->limit, 0xffff);
	vmcs_write32(sf->ar_bytes, 0xf3);
	if (save->base & 0xf)
		printk_once(KERN_WARNING "kvm: segment base is not paragraph"
			    " aligned when entering protected mode (seg=%d)",
			    seg);
=======
	fix_pmode_seg(vcpu, VCPU_SREG_CS, &vmx->rmode.segs[VCPU_SREG_CS]);
	fix_pmode_seg(vcpu, VCPU_SREG_SS, &vmx->rmode.segs[VCPU_SREG_SS]);
	fix_pmode_seg(vcpu, VCPU_SREG_ES, &vmx->rmode.segs[VCPU_SREG_ES]);
	fix_pmode_seg(vcpu, VCPU_SREG_DS, &vmx->rmode.segs[VCPU_SREG_DS]);
	fix_pmode_seg(vcpu, VCPU_SREG_FS, &vmx->rmode.segs[VCPU_SREG_FS]);
	fix_pmode_seg(vcpu, VCPU_SREG_GS, &vmx->rmode.segs[VCPU_SREG_GS]);

	/* CPL is always 0 when CPU enters protected mode */
	__set_bit(VCPU_EXREG_CPL, (ulong *)&vcpu->arch.regs_avail);
	vmx->cpl = 0;
}

static void fix_rmode_seg(int seg, struct kvm_segment *save)
{
	const struct kvm_vmx_segment_field *sf = &kvm_vmx_segment_fields[seg];
	struct kvm_segment var = *save;

	var.dpl = 0x3;
	if (seg == VCPU_SREG_CS)
		var.type = 0x3;

	if (!emulate_invalid_guest_state) {
		var.selector = var.base >> 4;
		var.base = var.base & 0xffff0;
		var.limit = 0xffff;
		var.g = 0;
		var.db = 0;
		var.present = 1;
		var.s = 1;
		var.l = 0;
		var.unusable = 0;
		var.type = 0x3;
		var.avl = 0;
		if (save->base & 0xf)
			printk_once(KERN_WARNING "kvm: segment base is not "
					"paragraph aligned when entering "
					"protected mode (seg=%d)", seg);
	}

	vmcs_write16(sf->selector, var.selector);
	vmcs_write32(sf->base, var.base);
	vmcs_write32(sf->limit, var.limit);
	vmcs_write32(sf->ar_bytes, vmx_segment_access_rights(&var));
>>>>>>> refs/remotes/origin/master
}

static void enter_rmode(struct kvm_vcpu *vcpu)
{
	unsigned long flags;
	struct vcpu_vmx *vmx = to_vmx(vcpu);
<<<<<<< HEAD
<<<<<<< HEAD
	struct kvm_segment var;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	if (enable_unrestricted_guest)
		return;

	vmx->emulation_required = 1;
=======

	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_TR], VCPU_SREG_TR);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_ES], VCPU_SREG_ES);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_DS], VCPU_SREG_DS);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_FS], VCPU_SREG_FS);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_GS], VCPU_SREG_GS);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_SS], VCPU_SREG_SS);
	vmx_get_segment(vcpu, &vmx->rmode.segs[VCPU_SREG_CS], VCPU_SREG_CS);

>>>>>>> refs/remotes/origin/master
	vmx->rmode.vm86_active = 1;

	/*
	 * Very old userspace does not call KVM_SET_TSS_ADDR before entering
<<<<<<< HEAD
	 * vcpu. Call it here with phys address pointing 16M below 4G.
	 */
	if (!vcpu->kvm->arch.tss_addr) {
		printk_once(KERN_WARNING "kvm: KVM_SET_TSS_ADDR need to be "
			     "called before entering vcpu\n");
		srcu_read_unlock(&vcpu->kvm->srcu, vcpu->srcu_idx);
		vmx_set_tss_addr(vcpu->kvm, 0xfeffd000);
		vcpu->srcu_idx = srcu_read_lock(&vcpu->kvm->srcu);
	}

	vmx_segment_cache_clear(vmx);

	vmx->rmode.tr.selector = vmcs_read16(GUEST_TR_SELECTOR);
	vmx->rmode.tr.base = vmcs_readl(GUEST_TR_BASE);
	vmcs_writel(GUEST_TR_BASE, rmode_tss_base(vcpu->kvm));

	vmx->rmode.tr.limit = vmcs_read32(GUEST_TR_LIMIT);
	vmcs_write32(GUEST_TR_LIMIT, RMODE_TSS_SIZE - 1);

	vmx->rmode.tr.ar = vmcs_read32(GUEST_TR_AR_BYTES);
=======
	 * vcpu. Warn the user that an update is overdue.
	 */
	if (!vcpu->kvm->arch.tss_addr)
		printk_once(KERN_WARNING "kvm: KVM_SET_TSS_ADDR need to be "
			     "called before entering vcpu\n");

	vmx_segment_cache_clear(vmx);

	vmcs_writel(GUEST_TR_BASE, vcpu->kvm->arch.tss_addr);
	vmcs_write32(GUEST_TR_LIMIT, RMODE_TSS_SIZE - 1);
>>>>>>> refs/remotes/origin/master
	vmcs_write32(GUEST_TR_AR_BYTES, 0x008b);

	flags = vmcs_readl(GUEST_RFLAGS);
	vmx->rmode.save_rflags = flags;

	flags |= X86_EFLAGS_IOPL | X86_EFLAGS_VM;

	vmcs_writel(GUEST_RFLAGS, flags);
	vmcs_writel(GUEST_CR4, vmcs_readl(GUEST_CR4) | X86_CR4_VME);
	update_exception_bitmap(vcpu);

<<<<<<< HEAD
	if (emulate_invalid_guest_state)
		goto continue_rmode;

<<<<<<< HEAD
	vmx_get_segment(vcpu, &var, VCPU_SREG_SS);
	vmx_set_segment(vcpu, &var, VCPU_SREG_SS);

	vmx_get_segment(vcpu, &var, VCPU_SREG_CS);
	vmx_set_segment(vcpu, &var, VCPU_SREG_CS);

	vmx_get_segment(vcpu, &var, VCPU_SREG_ES);
	vmx_set_segment(vcpu, &var, VCPU_SREG_ES);

	vmx_get_segment(vcpu, &var, VCPU_SREG_DS);
	vmx_set_segment(vcpu, &var, VCPU_SREG_DS);

	vmx_get_segment(vcpu, &var, VCPU_SREG_GS);
	vmx_set_segment(vcpu, &var, VCPU_SREG_GS);

	vmx_get_segment(vcpu, &var, VCPU_SREG_FS);
	vmx_set_segment(vcpu, &var, VCPU_SREG_FS);
=======
	vmcs_write16(GUEST_SS_SELECTOR, vmcs_readl(GUEST_SS_BASE) >> 4);
	vmcs_write32(GUEST_SS_LIMIT, 0xffff);
	vmcs_write32(GUEST_SS_AR_BYTES, 0xf3);

	vmcs_write32(GUEST_CS_AR_BYTES, 0xf3);
	vmcs_write32(GUEST_CS_LIMIT, 0xffff);
	if (vmcs_readl(GUEST_CS_BASE) == 0xffff0000)
		vmcs_writel(GUEST_CS_BASE, 0xf0000);
	vmcs_write16(GUEST_CS_SELECTOR, vmcs_readl(GUEST_CS_BASE) >> 4);

	fix_rmode_seg(VCPU_SREG_ES, &vmx->rmode.es);
	fix_rmode_seg(VCPU_SREG_DS, &vmx->rmode.ds);
	fix_rmode_seg(VCPU_SREG_GS, &vmx->rmode.gs);
	fix_rmode_seg(VCPU_SREG_FS, &vmx->rmode.fs);
>>>>>>> refs/remotes/origin/cm-10.0

continue_rmode:
=======
	fix_rmode_seg(VCPU_SREG_SS, &vmx->rmode.segs[VCPU_SREG_SS]);
	fix_rmode_seg(VCPU_SREG_CS, &vmx->rmode.segs[VCPU_SREG_CS]);
	fix_rmode_seg(VCPU_SREG_ES, &vmx->rmode.segs[VCPU_SREG_ES]);
	fix_rmode_seg(VCPU_SREG_DS, &vmx->rmode.segs[VCPU_SREG_DS]);
	fix_rmode_seg(VCPU_SREG_GS, &vmx->rmode.segs[VCPU_SREG_GS]);
	fix_rmode_seg(VCPU_SREG_FS, &vmx->rmode.segs[VCPU_SREG_FS]);

>>>>>>> refs/remotes/origin/master
	kvm_mmu_reset_context(vcpu);
}

static void vmx_set_efer(struct kvm_vcpu *vcpu, u64 efer)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	struct shared_msr_entry *msr = find_msr_entry(vmx, MSR_EFER);

	if (!msr)
		return;

	/*
	 * Force kernel_gs_base reloading before EFER changes, as control
	 * of this msr depends on is_long_mode().
	 */
	vmx_load_host_state(to_vmx(vcpu));
	vcpu->arch.efer = efer;
	if (efer & EFER_LMA) {
		vmcs_write32(VM_ENTRY_CONTROLS,
			     vmcs_read32(VM_ENTRY_CONTROLS) |
			     VM_ENTRY_IA32E_MODE);
		msr->data = efer;
	} else {
		vmcs_write32(VM_ENTRY_CONTROLS,
			     vmcs_read32(VM_ENTRY_CONTROLS) &
			     ~VM_ENTRY_IA32E_MODE);

		msr->data = efer & ~EFER_LME;
	}
	setup_msrs(vmx);
}

#ifdef CONFIG_X86_64

static void enter_lmode(struct kvm_vcpu *vcpu)
{
	u32 guest_tr_ar;

	vmx_segment_cache_clear(to_vmx(vcpu));

	guest_tr_ar = vmcs_read32(GUEST_TR_AR_BYTES);
	if ((guest_tr_ar & AR_TYPE_MASK) != AR_TYPE_BUSY_64_TSS) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_DEBUG "%s: tss fixup for long mode. \n",
		       __func__);
=======
		pr_debug_ratelimited("%s: tss fixup for long mode. \n",
				     __func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_debug_ratelimited("%s: tss fixup for long mode. \n",
				     __func__);
>>>>>>> refs/remotes/origin/master
		vmcs_write32(GUEST_TR_AR_BYTES,
			     (guest_tr_ar & ~AR_TYPE_MASK)
			     | AR_TYPE_BUSY_64_TSS);
	}
	vmx_set_efer(vcpu, vcpu->arch.efer | EFER_LMA);
}

static void exit_lmode(struct kvm_vcpu *vcpu)
{
	vmcs_write32(VM_ENTRY_CONTROLS,
		     vmcs_read32(VM_ENTRY_CONTROLS)
		     & ~VM_ENTRY_IA32E_MODE);
	vmx_set_efer(vcpu, vcpu->arch.efer & ~EFER_LMA);
}

#endif

static void vmx_flush_tlb(struct kvm_vcpu *vcpu)
{
	vpid_sync_context(to_vmx(vcpu));
	if (enable_ept) {
		if (!VALID_PAGE(vcpu->arch.mmu.root_hpa))
			return;
		ept_sync_context(construct_eptp(vcpu->arch.mmu.root_hpa));
	}
}

static void vmx_decache_cr0_guest_bits(struct kvm_vcpu *vcpu)
{
	ulong cr0_guest_owned_bits = vcpu->arch.cr0_guest_owned_bits;

	vcpu->arch.cr0 &= ~cr0_guest_owned_bits;
	vcpu->arch.cr0 |= vmcs_readl(GUEST_CR0) & cr0_guest_owned_bits;
}

static void vmx_decache_cr3(struct kvm_vcpu *vcpu)
{
	if (enable_ept && is_paging(vcpu))
		vcpu->arch.cr3 = vmcs_readl(GUEST_CR3);
	__set_bit(VCPU_EXREG_CR3, (ulong *)&vcpu->arch.regs_avail);
}

static void vmx_decache_cr4_guest_bits(struct kvm_vcpu *vcpu)
{
	ulong cr4_guest_owned_bits = vcpu->arch.cr4_guest_owned_bits;

	vcpu->arch.cr4 &= ~cr4_guest_owned_bits;
	vcpu->arch.cr4 |= vmcs_readl(GUEST_CR4) & cr4_guest_owned_bits;
}

static void ept_load_pdptrs(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
=======
	struct kvm_mmu *mmu = vcpu->arch.walk_mmu;

>>>>>>> refs/remotes/origin/master
	if (!test_bit(VCPU_EXREG_PDPTR,
		      (unsigned long *)&vcpu->arch.regs_dirty))
		return;

	if (is_paging(vcpu) && is_pae(vcpu) && !is_long_mode(vcpu)) {
<<<<<<< HEAD
		vmcs_write64(GUEST_PDPTR0, vcpu->arch.mmu.pdptrs[0]);
		vmcs_write64(GUEST_PDPTR1, vcpu->arch.mmu.pdptrs[1]);
		vmcs_write64(GUEST_PDPTR2, vcpu->arch.mmu.pdptrs[2]);
		vmcs_write64(GUEST_PDPTR3, vcpu->arch.mmu.pdptrs[3]);
=======
		vmcs_write64(GUEST_PDPTR0, mmu->pdptrs[0]);
		vmcs_write64(GUEST_PDPTR1, mmu->pdptrs[1]);
		vmcs_write64(GUEST_PDPTR2, mmu->pdptrs[2]);
		vmcs_write64(GUEST_PDPTR3, mmu->pdptrs[3]);
>>>>>>> refs/remotes/origin/master
	}
}

static void ept_save_pdptrs(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	if (is_paging(vcpu) && is_pae(vcpu) && !is_long_mode(vcpu)) {
		vcpu->arch.mmu.pdptrs[0] = vmcs_read64(GUEST_PDPTR0);
		vcpu->arch.mmu.pdptrs[1] = vmcs_read64(GUEST_PDPTR1);
		vcpu->arch.mmu.pdptrs[2] = vmcs_read64(GUEST_PDPTR2);
		vcpu->arch.mmu.pdptrs[3] = vmcs_read64(GUEST_PDPTR3);
=======
	struct kvm_mmu *mmu = vcpu->arch.walk_mmu;

	if (is_paging(vcpu) && is_pae(vcpu) && !is_long_mode(vcpu)) {
		mmu->pdptrs[0] = vmcs_read64(GUEST_PDPTR0);
		mmu->pdptrs[1] = vmcs_read64(GUEST_PDPTR1);
		mmu->pdptrs[2] = vmcs_read64(GUEST_PDPTR2);
		mmu->pdptrs[3] = vmcs_read64(GUEST_PDPTR3);
>>>>>>> refs/remotes/origin/master
	}

	__set_bit(VCPU_EXREG_PDPTR,
		  (unsigned long *)&vcpu->arch.regs_avail);
	__set_bit(VCPU_EXREG_PDPTR,
		  (unsigned long *)&vcpu->arch.regs_dirty);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void vmx_set_cr4(struct kvm_vcpu *vcpu, unsigned long cr4);
=======
static int vmx_set_cr4(struct kvm_vcpu *vcpu, unsigned long cr4);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int vmx_set_cr4(struct kvm_vcpu *vcpu, unsigned long cr4);
>>>>>>> refs/remotes/origin/master

static void ept_update_paging_mode_cr0(unsigned long *hw_cr0,
					unsigned long cr0,
					struct kvm_vcpu *vcpu)
{
	if (!test_bit(VCPU_EXREG_CR3, (ulong *)&vcpu->arch.regs_avail))
		vmx_decache_cr3(vcpu);
	if (!(cr0 & X86_CR0_PG)) {
		/* From paging/starting to nonpaging */
		vmcs_write32(CPU_BASED_VM_EXEC_CONTROL,
			     vmcs_read32(CPU_BASED_VM_EXEC_CONTROL) |
			     (CPU_BASED_CR3_LOAD_EXITING |
			      CPU_BASED_CR3_STORE_EXITING));
		vcpu->arch.cr0 = cr0;
		vmx_set_cr4(vcpu, kvm_read_cr4(vcpu));
	} else if (!is_paging(vcpu)) {
		/* From nonpaging to paging */
		vmcs_write32(CPU_BASED_VM_EXEC_CONTROL,
			     vmcs_read32(CPU_BASED_VM_EXEC_CONTROL) &
			     ~(CPU_BASED_CR3_LOAD_EXITING |
			       CPU_BASED_CR3_STORE_EXITING));
		vcpu->arch.cr0 = cr0;
		vmx_set_cr4(vcpu, kvm_read_cr4(vcpu));
	}

	if (!(cr0 & X86_CR0_WP))
		*hw_cr0 &= ~X86_CR0_WP;
}

static void vmx_set_cr0(struct kvm_vcpu *vcpu, unsigned long cr0)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	unsigned long hw_cr0;

<<<<<<< HEAD
	if (enable_unrestricted_guest)
		hw_cr0 = (cr0 & ~KVM_GUEST_CR0_MASK_UNRESTRICTED_GUEST)
			| KVM_VM_CR0_ALWAYS_ON_UNRESTRICTED_GUEST;
	else
		hw_cr0 = (cr0 & ~KVM_GUEST_CR0_MASK) | KVM_VM_CR0_ALWAYS_ON;

	if (vmx->rmode.vm86_active && (cr0 & X86_CR0_PE))
		enter_pmode(vcpu);

	if (!vmx->rmode.vm86_active && !(cr0 & X86_CR0_PE))
		enter_rmode(vcpu);
=======
	hw_cr0 = (cr0 & ~KVM_GUEST_CR0_MASK);
	if (enable_unrestricted_guest)
		hw_cr0 |= KVM_VM_CR0_ALWAYS_ON_UNRESTRICTED_GUEST;
	else {
		hw_cr0 |= KVM_VM_CR0_ALWAYS_ON;

		if (vmx->rmode.vm86_active && (cr0 & X86_CR0_PE))
			enter_pmode(vcpu);

		if (!vmx->rmode.vm86_active && !(cr0 & X86_CR0_PE))
			enter_rmode(vcpu);
	}
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_X86_64
	if (vcpu->arch.efer & EFER_LME) {
		if (!is_paging(vcpu) && (cr0 & X86_CR0_PG))
			enter_lmode(vcpu);
		if (is_paging(vcpu) && !(cr0 & X86_CR0_PG))
			exit_lmode(vcpu);
	}
#endif

	if (enable_ept)
		ept_update_paging_mode_cr0(&hw_cr0, cr0, vcpu);

	if (!vcpu->fpu_active)
		hw_cr0 |= X86_CR0_TS | X86_CR0_MP;

	vmcs_writel(CR0_READ_SHADOW, cr0);
	vmcs_writel(GUEST_CR0, hw_cr0);
	vcpu->arch.cr0 = cr0;
<<<<<<< HEAD
	__clear_bit(VCPU_EXREG_CPL, (ulong *)&vcpu->arch.regs_avail);
=======

	/* depends on vcpu->arch.cr0 to be set to a new value */
	vmx->emulation_required = emulation_required(vcpu);
>>>>>>> refs/remotes/origin/master
}

static u64 construct_eptp(unsigned long root_hpa)
{
	u64 eptp;

	/* TODO write the value reading from MSR */
	eptp = VMX_EPT_DEFAULT_MT |
		VMX_EPT_DEFAULT_GAW << VMX_EPT_GAW_EPTP_SHIFT;
<<<<<<< HEAD
=======
	if (enable_ept_ad_bits)
		eptp |= VMX_EPT_AD_ENABLE_BIT;
>>>>>>> refs/remotes/origin/master
	eptp |= (root_hpa & PAGE_MASK);

	return eptp;
}

static void vmx_set_cr3(struct kvm_vcpu *vcpu, unsigned long cr3)
{
	unsigned long guest_cr3;
	u64 eptp;

	guest_cr3 = cr3;
	if (enable_ept) {
		eptp = construct_eptp(cr3);
		vmcs_write64(EPT_POINTER, eptp);
<<<<<<< HEAD
		guest_cr3 = is_paging(vcpu) ? kvm_read_cr3(vcpu) :
			vcpu->kvm->arch.ept_identity_map_addr;
=======
		if (is_paging(vcpu) || is_guest_mode(vcpu))
			guest_cr3 = kvm_read_cr3(vcpu);
		else
			guest_cr3 = vcpu->kvm->arch.ept_identity_map_addr;
>>>>>>> refs/remotes/origin/master
		ept_load_pdptrs(vcpu);
	}

	vmx_flush_tlb(vcpu);
	vmcs_writel(GUEST_CR3, guest_cr3);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void vmx_set_cr4(struct kvm_vcpu *vcpu, unsigned long cr4)
=======
static int vmx_set_cr4(struct kvm_vcpu *vcpu, unsigned long cr4)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int vmx_set_cr4(struct kvm_vcpu *vcpu, unsigned long cr4)
>>>>>>> refs/remotes/origin/master
{
	unsigned long hw_cr4 = cr4 | (to_vmx(vcpu)->rmode.vm86_active ?
		    KVM_RMODE_VM_CR4_ALWAYS_ON : KVM_PMODE_VM_CR4_ALWAYS_ON);

<<<<<<< HEAD
<<<<<<< HEAD
	vcpu->arch.cr4 = cr4;
	if (enable_ept) {
		if (!is_paging(vcpu)) {
=======
=======
>>>>>>> refs/remotes/origin/master
	if (cr4 & X86_CR4_VMXE) {
		/*
		 * To use VMXON (and later other VMX instructions), a guest
		 * must first be able to turn on cr4.VMXE (see handle_vmon()).
		 * So basically the check on whether to allow nested VMX
		 * is here.
		 */
		if (!nested_vmx_allowed(vcpu))
			return 1;
<<<<<<< HEAD
	} else if (to_vmx(vcpu)->nested.vmxon)
=======
	}
	if (to_vmx(vcpu)->nested.vmxon &&
	    ((cr4 & VMXON_CR4_ALWAYSON) != VMXON_CR4_ALWAYSON))
>>>>>>> refs/remotes/origin/master
		return 1;

	vcpu->arch.cr4 = cr4;
	if (enable_ept) {
		if (!is_paging(vcpu)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			hw_cr4 &= ~X86_CR4_PAE;
			hw_cr4 |= X86_CR4_PSE;
=======
			hw_cr4 &= ~X86_CR4_PAE;
			hw_cr4 |= X86_CR4_PSE;
			/*
			 * SMEP is disabled if CPU is in non-paging mode in
			 * hardware. However KVM always uses paging mode to
			 * emulate guest non-paging mode with TDP.
			 * To emulate this behavior, SMEP needs to be manually
			 * disabled when guest switches to non-paging mode.
			 */
			hw_cr4 &= ~X86_CR4_SMEP;
>>>>>>> refs/remotes/origin/master
		} else if (!(cr4 & X86_CR4_PAE)) {
			hw_cr4 &= ~X86_CR4_PAE;
		}
	}

	vmcs_writel(CR4_READ_SHADOW, cr4);
	vmcs_writel(GUEST_CR4, hw_cr4);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return 0;
>>>>>>> refs/remotes/origin/master
}

static void vmx_get_segment(struct kvm_vcpu *vcpu,
			    struct kvm_segment *var, int seg)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
<<<<<<< HEAD
	struct kvm_save_segment *save;
	u32 ar;

	if (vmx->rmode.vm86_active
	    && (seg == VCPU_SREG_TR || seg == VCPU_SREG_ES
		|| seg == VCPU_SREG_DS || seg == VCPU_SREG_FS
		|| seg == VCPU_SREG_GS)
	    && !emulate_invalid_guest_state) {
		switch (seg) {
		case VCPU_SREG_TR: save = &vmx->rmode.tr; break;
		case VCPU_SREG_ES: save = &vmx->rmode.es; break;
		case VCPU_SREG_DS: save = &vmx->rmode.ds; break;
		case VCPU_SREG_FS: save = &vmx->rmode.fs; break;
		case VCPU_SREG_GS: save = &vmx->rmode.gs; break;
		default: BUG();
		}
		var->selector = save->selector;
		var->base = save->base;
		var->limit = save->limit;
		ar = save->ar;
		if (seg == VCPU_SREG_TR
		    || var->selector == vmx_read_guest_seg_selector(vmx, seg))
			goto use_saved_rmode_seg;
=======
	u32 ar;

	if (vmx->rmode.vm86_active && seg != VCPU_SREG_LDTR) {
		*var = vmx->rmode.segs[seg];
		if (seg == VCPU_SREG_TR
		    || var->selector == vmx_read_guest_seg_selector(vmx, seg))
			return;
		var->base = vmx_read_guest_seg_base(vmx, seg);
		var->selector = vmx_read_guest_seg_selector(vmx, seg);
		return;
>>>>>>> refs/remotes/origin/master
	}
	var->base = vmx_read_guest_seg_base(vmx, seg);
	var->limit = vmx_read_guest_seg_limit(vmx, seg);
	var->selector = vmx_read_guest_seg_selector(vmx, seg);
	ar = vmx_read_guest_seg_ar(vmx, seg);
<<<<<<< HEAD
use_saved_rmode_seg:
	if ((ar & AR_UNUSABLE_MASK) && !emulate_invalid_guest_state)
		ar = 0;
	var->type = ar & 15;
	var->s = (ar >> 4) & 1;
	var->dpl = (ar >> 5) & 3;
	var->present = (ar >> 7) & 1;
=======
	var->unusable = (ar >> 16) & 1;
	var->type = ar & 15;
	var->s = (ar >> 4) & 1;
	var->dpl = (ar >> 5) & 3;
	/*
	 * Some userspaces do not preserve unusable property. Since usable
	 * segment has to be present according to VMX spec we can use present
	 * property to amend userspace bug by making unusable segment always
	 * nonpresent. vmx_segment_access_rights() already marks nonpresent
	 * segment as unusable.
	 */
	var->present = !var->unusable;
>>>>>>> refs/remotes/origin/master
	var->avl = (ar >> 12) & 1;
	var->l = (ar >> 13) & 1;
	var->db = (ar >> 14) & 1;
	var->g = (ar >> 15) & 1;
<<<<<<< HEAD
	var->unusable = (ar >> 16) & 1;
=======
>>>>>>> refs/remotes/origin/master
}

static u64 vmx_get_segment_base(struct kvm_vcpu *vcpu, int seg)
{
	struct kvm_segment s;

	if (to_vmx(vcpu)->rmode.vm86_active) {
		vmx_get_segment(vcpu, &s, seg);
		return s.base;
	}
	return vmx_read_guest_seg_base(to_vmx(vcpu), seg);
}

<<<<<<< HEAD
static int __vmx_get_cpl(struct kvm_vcpu *vcpu)
{
=======
static int vmx_get_cpl(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

>>>>>>> refs/remotes/origin/master
	if (!is_protmode(vcpu))
		return 0;

	if (!is_long_mode(vcpu)
	    && (kvm_get_rflags(vcpu) & X86_EFLAGS_VM)) /* if virtual 8086 */
		return 3;

<<<<<<< HEAD
	return vmx_read_guest_seg_selector(to_vmx(vcpu), VCPU_SREG_CS) & 3;
}

static int vmx_get_cpl(struct kvm_vcpu *vcpu)
{
	if (!test_bit(VCPU_EXREG_CPL, (ulong *)&vcpu->arch.regs_avail)) {
		__set_bit(VCPU_EXREG_CPL, (ulong *)&vcpu->arch.regs_avail);
		to_vmx(vcpu)->cpl = __vmx_get_cpl(vcpu);
	}
	return to_vmx(vcpu)->cpl;
=======
	if (!test_bit(VCPU_EXREG_CPL, (ulong *)&vcpu->arch.regs_avail)) {
		__set_bit(VCPU_EXREG_CPL, (ulong *)&vcpu->arch.regs_avail);
		vmx->cpl = vmx_read_guest_seg_selector(vmx, VCPU_SREG_CS) & 3;
	}

	return vmx->cpl;
>>>>>>> refs/remotes/origin/master
}


static u32 vmx_segment_access_rights(struct kvm_segment *var)
{
	u32 ar;

<<<<<<< HEAD
	if (var->unusable)
=======
	if (var->unusable || !var->present)
>>>>>>> refs/remotes/origin/master
		ar = 1 << 16;
	else {
		ar = var->type & 15;
		ar |= (var->s & 1) << 4;
		ar |= (var->dpl & 3) << 5;
		ar |= (var->present & 1) << 7;
		ar |= (var->avl & 1) << 12;
		ar |= (var->l & 1) << 13;
		ar |= (var->db & 1) << 14;
		ar |= (var->g & 1) << 15;
	}
<<<<<<< HEAD
	if (ar == 0) /* a 0 value means unusable */
		ar = AR_UNUSABLE_MASK;
=======
>>>>>>> refs/remotes/origin/master

	return ar;
}

static void vmx_set_segment(struct kvm_vcpu *vcpu,
			    struct kvm_segment *var, int seg)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
<<<<<<< HEAD
	struct kvm_vmx_segment_field *sf = &kvm_vmx_segment_fields[seg];
	u32 ar;

	vmx_segment_cache_clear(vmx);

	if (vmx->rmode.vm86_active && seg == VCPU_SREG_TR) {
		vmcs_write16(sf->selector, var->selector);
		vmx->rmode.tr.selector = var->selector;
		vmx->rmode.tr.base = var->base;
		vmx->rmode.tr.limit = var->limit;
		vmx->rmode.tr.ar = vmx_segment_access_rights(var);
		return;
	}
	vmcs_writel(sf->base, var->base);
	vmcs_write32(sf->limit, var->limit);
	vmcs_write16(sf->selector, var->selector);
	if (vmx->rmode.vm86_active && var->s) {
		/*
		 * Hack real-mode segments into vm86 compatibility.
		 */
		if (var->base == 0xffff0000 && var->selector == 0xf000)
			vmcs_writel(sf->base, 0xf0000);
		ar = 0xf3;
	} else
		ar = vmx_segment_access_rights(var);
=======
	const struct kvm_vmx_segment_field *sf = &kvm_vmx_segment_fields[seg];

	vmx_segment_cache_clear(vmx);
	if (seg == VCPU_SREG_CS)
		__clear_bit(VCPU_EXREG_CPL, (ulong *)&vcpu->arch.regs_avail);

	if (vmx->rmode.vm86_active && seg != VCPU_SREG_LDTR) {
		vmx->rmode.segs[seg] = *var;
		if (seg == VCPU_SREG_TR)
			vmcs_write16(sf->selector, var->selector);
		else if (var->s)
			fix_rmode_seg(seg, &vmx->rmode.segs[seg]);
		goto out;
	}

	vmcs_writel(sf->base, var->base);
	vmcs_write32(sf->limit, var->limit);
	vmcs_write16(sf->selector, var->selector);
>>>>>>> refs/remotes/origin/master

	/*
	 *   Fix the "Accessed" bit in AR field of segment registers for older
	 * qemu binaries.
	 *   IA32 arch specifies that at the time of processor reset the
	 * "Accessed" bit in the AR field of segment registers is 1. And qemu
<<<<<<< HEAD
	 * is setting it to 0 in the usedland code. This causes invalid guest
=======
	 * is setting it to 0 in the userland code. This causes invalid guest
>>>>>>> refs/remotes/origin/master
	 * state vmexit when "unrestricted guest" mode is turned on.
	 *    Fix for this setup issue in cpu_reset is being pushed in the qemu
	 * tree. Newer qemu binaries with that qemu fix would not need this
	 * kvm hack.
	 */
	if (enable_unrestricted_guest && (seg != VCPU_SREG_LDTR))
<<<<<<< HEAD
		ar |= 0x1; /* Accessed */

	vmcs_write32(sf->ar_bytes, ar);
	__clear_bit(VCPU_EXREG_CPL, (ulong *)&vcpu->arch.regs_avail);
<<<<<<< HEAD

	/*
	 * Fix segments for real mode guest in hosts that don't have
	 * "unrestricted_mode" or it was disabled.
	 * This is done to allow migration of the guests from hosts with
	 * unrestricted guest like Westmere to older host that don't have
	 * unrestricted guest like Nehelem.
	 */
	if (!enable_unrestricted_guest && vmx->rmode.vm86_active) {
		switch (seg) {
		case VCPU_SREG_CS:
			vmcs_write32(GUEST_CS_AR_BYTES, 0xf3);
			vmcs_write32(GUEST_CS_LIMIT, 0xffff);
			if (vmcs_readl(GUEST_CS_BASE) == 0xffff0000)
				vmcs_writel(GUEST_CS_BASE, 0xf0000);
			vmcs_write16(GUEST_CS_SELECTOR,
				     vmcs_readl(GUEST_CS_BASE) >> 4);
			break;
		case VCPU_SREG_ES:
			fix_rmode_seg(VCPU_SREG_ES, &vmx->rmode.es);
			break;
		case VCPU_SREG_DS:
			fix_rmode_seg(VCPU_SREG_DS, &vmx->rmode.ds);
			break;
		case VCPU_SREG_GS:
			fix_rmode_seg(VCPU_SREG_GS, &vmx->rmode.gs);
			break;
		case VCPU_SREG_FS:
			fix_rmode_seg(VCPU_SREG_FS, &vmx->rmode.fs);
			break;
		case VCPU_SREG_SS:
			vmcs_write16(GUEST_SS_SELECTOR,
				     vmcs_readl(GUEST_SS_BASE) >> 4);
			vmcs_write32(GUEST_SS_LIMIT, 0xffff);
			vmcs_write32(GUEST_SS_AR_BYTES, 0xf3);
			break;
		}
	}
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		var->type |= 0x1; /* Accessed */

	vmcs_write32(sf->ar_bytes, vmx_segment_access_rights(var));

out:
	vmx->emulation_required |= emulation_required(vcpu);
>>>>>>> refs/remotes/origin/master
}

static void vmx_get_cs_db_l_bits(struct kvm_vcpu *vcpu, int *db, int *l)
{
	u32 ar = vmx_read_guest_seg_ar(to_vmx(vcpu), VCPU_SREG_CS);

	*db = (ar >> 14) & 1;
	*l = (ar >> 13) & 1;
}

static void vmx_get_idt(struct kvm_vcpu *vcpu, struct desc_ptr *dt)
{
	dt->size = vmcs_read32(GUEST_IDTR_LIMIT);
	dt->address = vmcs_readl(GUEST_IDTR_BASE);
}

static void vmx_set_idt(struct kvm_vcpu *vcpu, struct desc_ptr *dt)
{
	vmcs_write32(GUEST_IDTR_LIMIT, dt->size);
	vmcs_writel(GUEST_IDTR_BASE, dt->address);
}

static void vmx_get_gdt(struct kvm_vcpu *vcpu, struct desc_ptr *dt)
{
	dt->size = vmcs_read32(GUEST_GDTR_LIMIT);
	dt->address = vmcs_readl(GUEST_GDTR_BASE);
}

static void vmx_set_gdt(struct kvm_vcpu *vcpu, struct desc_ptr *dt)
{
	vmcs_write32(GUEST_GDTR_LIMIT, dt->size);
	vmcs_writel(GUEST_GDTR_BASE, dt->address);
}

static bool rmode_segment_valid(struct kvm_vcpu *vcpu, int seg)
{
	struct kvm_segment var;
	u32 ar;

	vmx_get_segment(vcpu, &var, seg);
<<<<<<< HEAD
=======
	var.dpl = 0x3;
	if (seg == VCPU_SREG_CS)
		var.type = 0x3;
>>>>>>> refs/remotes/origin/master
	ar = vmx_segment_access_rights(&var);

	if (var.base != (var.selector << 4))
		return false;
	if (var.limit != 0xffff)
		return false;
	if (ar != 0xf3)
		return false;

	return true;
}

static bool code_segment_valid(struct kvm_vcpu *vcpu)
{
	struct kvm_segment cs;
	unsigned int cs_rpl;

	vmx_get_segment(vcpu, &cs, VCPU_SREG_CS);
	cs_rpl = cs.selector & SELECTOR_RPL_MASK;

	if (cs.unusable)
		return false;
	if (~cs.type & (AR_TYPE_CODE_MASK|AR_TYPE_ACCESSES_MASK))
		return false;
	if (!cs.s)
		return false;
	if (cs.type & AR_TYPE_WRITEABLE_MASK) {
		if (cs.dpl > cs_rpl)
			return false;
	} else {
		if (cs.dpl != cs_rpl)
			return false;
	}
	if (!cs.present)
		return false;

	/* TODO: Add Reserved field check, this'll require a new member in the kvm_segment_field structure */
	return true;
}

static bool stack_segment_valid(struct kvm_vcpu *vcpu)
{
	struct kvm_segment ss;
	unsigned int ss_rpl;

	vmx_get_segment(vcpu, &ss, VCPU_SREG_SS);
	ss_rpl = ss.selector & SELECTOR_RPL_MASK;

	if (ss.unusable)
		return true;
	if (ss.type != 3 && ss.type != 7)
		return false;
	if (!ss.s)
		return false;
	if (ss.dpl != ss_rpl) /* DPL != RPL */
		return false;
	if (!ss.present)
		return false;

	return true;
}

static bool data_segment_valid(struct kvm_vcpu *vcpu, int seg)
{
	struct kvm_segment var;
	unsigned int rpl;

	vmx_get_segment(vcpu, &var, seg);
	rpl = var.selector & SELECTOR_RPL_MASK;

	if (var.unusable)
		return true;
	if (!var.s)
		return false;
	if (!var.present)
		return false;
	if (~var.type & (AR_TYPE_CODE_MASK|AR_TYPE_WRITEABLE_MASK)) {
		if (var.dpl < rpl) /* DPL < RPL */
			return false;
	}

	/* TODO: Add other members to kvm_segment_field to allow checking for other access
	 * rights flags
	 */
	return true;
}

static bool tr_valid(struct kvm_vcpu *vcpu)
{
	struct kvm_segment tr;

	vmx_get_segment(vcpu, &tr, VCPU_SREG_TR);

	if (tr.unusable)
		return false;
	if (tr.selector & SELECTOR_TI_MASK)	/* TI = 1 */
		return false;
	if (tr.type != 3 && tr.type != 11) /* TODO: Check if guest is in IA32e mode */
		return false;
	if (!tr.present)
		return false;

	return true;
}

static bool ldtr_valid(struct kvm_vcpu *vcpu)
{
	struct kvm_segment ldtr;

	vmx_get_segment(vcpu, &ldtr, VCPU_SREG_LDTR);

	if (ldtr.unusable)
		return true;
	if (ldtr.selector & SELECTOR_TI_MASK)	/* TI = 1 */
		return false;
	if (ldtr.type != 2)
		return false;
	if (!ldtr.present)
		return false;

	return true;
}

static bool cs_ss_rpl_check(struct kvm_vcpu *vcpu)
{
	struct kvm_segment cs, ss;

	vmx_get_segment(vcpu, &cs, VCPU_SREG_CS);
	vmx_get_segment(vcpu, &ss, VCPU_SREG_SS);

	return ((cs.selector & SELECTOR_RPL_MASK) ==
		 (ss.selector & SELECTOR_RPL_MASK));
}

/*
 * Check if guest state is valid. Returns true if valid, false if
 * not.
 * We assume that registers are always usable
 */
static bool guest_state_valid(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	/* real mode guest state checks */
	if (!is_protmode(vcpu)) {
=======
	if (enable_unrestricted_guest)
		return true;

	/* real mode guest state checks */
	if (!is_protmode(vcpu) || (vmx_get_rflags(vcpu) & X86_EFLAGS_VM)) {
>>>>>>> refs/remotes/origin/master
		if (!rmode_segment_valid(vcpu, VCPU_SREG_CS))
			return false;
		if (!rmode_segment_valid(vcpu, VCPU_SREG_SS))
			return false;
		if (!rmode_segment_valid(vcpu, VCPU_SREG_DS))
			return false;
		if (!rmode_segment_valid(vcpu, VCPU_SREG_ES))
			return false;
		if (!rmode_segment_valid(vcpu, VCPU_SREG_FS))
			return false;
		if (!rmode_segment_valid(vcpu, VCPU_SREG_GS))
			return false;
	} else {
	/* protected mode guest state checks */
		if (!cs_ss_rpl_check(vcpu))
			return false;
		if (!code_segment_valid(vcpu))
			return false;
		if (!stack_segment_valid(vcpu))
			return false;
		if (!data_segment_valid(vcpu, VCPU_SREG_DS))
			return false;
		if (!data_segment_valid(vcpu, VCPU_SREG_ES))
			return false;
		if (!data_segment_valid(vcpu, VCPU_SREG_FS))
			return false;
		if (!data_segment_valid(vcpu, VCPU_SREG_GS))
			return false;
		if (!tr_valid(vcpu))
			return false;
		if (!ldtr_valid(vcpu))
			return false;
	}
	/* TODO:
	 * - Add checks on RIP
	 * - Add checks on RFLAGS
	 */

	return true;
}

static int init_rmode_tss(struct kvm *kvm)
{
	gfn_t fn;
	u16 data = 0;
	int r, idx, ret = 0;

	idx = srcu_read_lock(&kvm->srcu);
<<<<<<< HEAD
	fn = rmode_tss_base(kvm) >> PAGE_SHIFT;
=======
	fn = kvm->arch.tss_addr >> PAGE_SHIFT;
>>>>>>> refs/remotes/origin/master
	r = kvm_clear_guest_page(kvm, fn, 0, PAGE_SIZE);
	if (r < 0)
		goto out;
	data = TSS_BASE_SIZE + TSS_REDIRECTION_SIZE;
	r = kvm_write_guest_page(kvm, fn++, &data,
			TSS_IOPB_BASE_OFFSET, sizeof(u16));
	if (r < 0)
		goto out;
	r = kvm_clear_guest_page(kvm, fn++, 0, PAGE_SIZE);
	if (r < 0)
		goto out;
	r = kvm_clear_guest_page(kvm, fn, 0, PAGE_SIZE);
	if (r < 0)
		goto out;
	data = ~0;
	r = kvm_write_guest_page(kvm, fn, &data,
				 RMODE_TSS_SIZE - 2 * PAGE_SIZE - 1,
				 sizeof(u8));
	if (r < 0)
		goto out;

	ret = 1;
out:
	srcu_read_unlock(&kvm->srcu, idx);
	return ret;
}

static int init_rmode_identity_map(struct kvm *kvm)
{
	int i, idx, r, ret;
	pfn_t identity_map_pfn;
	u32 tmp;

	if (!enable_ept)
		return 1;
	if (unlikely(!kvm->arch.ept_identity_pagetable)) {
		printk(KERN_ERR "EPT: identity-mapping pagetable "
			"haven't been allocated!\n");
		return 0;
	}
	if (likely(kvm->arch.ept_identity_pagetable_done))
		return 1;
	ret = 0;
	identity_map_pfn = kvm->arch.ept_identity_map_addr >> PAGE_SHIFT;
	idx = srcu_read_lock(&kvm->srcu);
	r = kvm_clear_guest_page(kvm, identity_map_pfn, 0, PAGE_SIZE);
	if (r < 0)
		goto out;
	/* Set up identity-mapping pagetable for EPT in real mode */
	for (i = 0; i < PT32_ENT_PER_PAGE; i++) {
		tmp = (i << 22) + (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER |
			_PAGE_ACCESSED | _PAGE_DIRTY | _PAGE_PSE);
		r = kvm_write_guest_page(kvm, identity_map_pfn,
				&tmp, i * sizeof(tmp), sizeof(tmp));
		if (r < 0)
			goto out;
	}
	kvm->arch.ept_identity_pagetable_done = true;
	ret = 1;
out:
	srcu_read_unlock(&kvm->srcu, idx);
	return ret;
}

static void seg_setup(int seg)
{
<<<<<<< HEAD
	struct kvm_vmx_segment_field *sf = &kvm_vmx_segment_fields[seg];
=======
	const struct kvm_vmx_segment_field *sf = &kvm_vmx_segment_fields[seg];
>>>>>>> refs/remotes/origin/master
	unsigned int ar;

	vmcs_write16(sf->selector, 0);
	vmcs_writel(sf->base, 0);
	vmcs_write32(sf->limit, 0xffff);
<<<<<<< HEAD
	if (enable_unrestricted_guest) {
		ar = 0x93;
		if (seg == VCPU_SREG_CS)
			ar |= 0x08; /* code segment */
	} else
		ar = 0xf3;
=======
	ar = 0x93;
	if (seg == VCPU_SREG_CS)
		ar |= 0x08; /* code segment */
>>>>>>> refs/remotes/origin/master

	vmcs_write32(sf->ar_bytes, ar);
}

static int alloc_apic_access_page(struct kvm *kvm)
{
<<<<<<< HEAD
=======
	struct page *page;
>>>>>>> refs/remotes/origin/master
	struct kvm_userspace_memory_region kvm_userspace_mem;
	int r = 0;

	mutex_lock(&kvm->slots_lock);
	if (kvm->arch.apic_access_page)
		goto out;
	kvm_userspace_mem.slot = APIC_ACCESS_PAGE_PRIVATE_MEMSLOT;
	kvm_userspace_mem.flags = 0;
	kvm_userspace_mem.guest_phys_addr = 0xfee00000ULL;
	kvm_userspace_mem.memory_size = PAGE_SIZE;
<<<<<<< HEAD
	r = __kvm_set_memory_region(kvm, &kvm_userspace_mem, 0);
	if (r)
		goto out;

	kvm->arch.apic_access_page = gfn_to_page(kvm, 0xfee00);
=======
	r = __kvm_set_memory_region(kvm, &kvm_userspace_mem);
	if (r)
		goto out;

	page = gfn_to_page(kvm, 0xfee00);
	if (is_error_page(page)) {
		r = -EFAULT;
		goto out;
	}

	kvm->arch.apic_access_page = page;
>>>>>>> refs/remotes/origin/master
out:
	mutex_unlock(&kvm->slots_lock);
	return r;
}

static int alloc_identity_pagetable(struct kvm *kvm)
{
<<<<<<< HEAD
=======
	struct page *page;
>>>>>>> refs/remotes/origin/master
	struct kvm_userspace_memory_region kvm_userspace_mem;
	int r = 0;

	mutex_lock(&kvm->slots_lock);
	if (kvm->arch.ept_identity_pagetable)
		goto out;
	kvm_userspace_mem.slot = IDENTITY_PAGETABLE_PRIVATE_MEMSLOT;
	kvm_userspace_mem.flags = 0;
	kvm_userspace_mem.guest_phys_addr =
		kvm->arch.ept_identity_map_addr;
	kvm_userspace_mem.memory_size = PAGE_SIZE;
<<<<<<< HEAD
	r = __kvm_set_memory_region(kvm, &kvm_userspace_mem, 0);
	if (r)
		goto out;

	kvm->arch.ept_identity_pagetable = gfn_to_page(kvm,
			kvm->arch.ept_identity_map_addr >> PAGE_SHIFT);
=======
	r = __kvm_set_memory_region(kvm, &kvm_userspace_mem);
	if (r)
		goto out;

	page = gfn_to_page(kvm, kvm->arch.ept_identity_map_addr >> PAGE_SHIFT);
	if (is_error_page(page)) {
		r = -EFAULT;
		goto out;
	}

	kvm->arch.ept_identity_pagetable = page;
>>>>>>> refs/remotes/origin/master
out:
	mutex_unlock(&kvm->slots_lock);
	return r;
}

static void allocate_vpid(struct vcpu_vmx *vmx)
{
	int vpid;

	vmx->vpid = 0;
	if (!enable_vpid)
		return;
	spin_lock(&vmx_vpid_lock);
	vpid = find_first_zero_bit(vmx_vpid_bitmap, VMX_NR_VPIDS);
	if (vpid < VMX_NR_VPIDS) {
		vmx->vpid = vpid;
		__set_bit(vpid, vmx_vpid_bitmap);
	}
	spin_unlock(&vmx_vpid_lock);
}

static void free_vpid(struct vcpu_vmx *vmx)
{
	if (!enable_vpid)
		return;
	spin_lock(&vmx_vpid_lock);
	if (vmx->vpid != 0)
		__clear_bit(vmx->vpid, vmx_vpid_bitmap);
	spin_unlock(&vmx_vpid_lock);
}

<<<<<<< HEAD
static void __vmx_disable_intercept_for_msr(unsigned long *msr_bitmap, u32 msr)
=======
#define MSR_TYPE_R	1
#define MSR_TYPE_W	2
static void __vmx_disable_intercept_for_msr(unsigned long *msr_bitmap,
						u32 msr, int type)
{
	int f = sizeof(unsigned long);

	if (!cpu_has_vmx_msr_bitmap())
		return;

	/*
	 * See Intel PRM Vol. 3, 20.6.9 (MSR-Bitmap Address). Early manuals
	 * have the write-low and read-high bitmap offsets the wrong way round.
	 * We can control MSRs 0x00000000-0x00001fff and 0xc0000000-0xc0001fff.
	 */
	if (msr <= 0x1fff) {
		if (type & MSR_TYPE_R)
			/* read-low */
			__clear_bit(msr, msr_bitmap + 0x000 / f);

		if (type & MSR_TYPE_W)
			/* write-low */
			__clear_bit(msr, msr_bitmap + 0x800 / f);

	} else if ((msr >= 0xc0000000) && (msr <= 0xc0001fff)) {
		msr &= 0x1fff;
		if (type & MSR_TYPE_R)
			/* read-high */
			__clear_bit(msr, msr_bitmap + 0x400 / f);

		if (type & MSR_TYPE_W)
			/* write-high */
			__clear_bit(msr, msr_bitmap + 0xc00 / f);

	}
}

static void __vmx_enable_intercept_for_msr(unsigned long *msr_bitmap,
						u32 msr, int type)
>>>>>>> refs/remotes/origin/master
{
	int f = sizeof(unsigned long);

	if (!cpu_has_vmx_msr_bitmap())
		return;

	/*
	 * See Intel PRM Vol. 3, 20.6.9 (MSR-Bitmap Address). Early manuals
	 * have the write-low and read-high bitmap offsets the wrong way round.
	 * We can control MSRs 0x00000000-0x00001fff and 0xc0000000-0xc0001fff.
	 */
	if (msr <= 0x1fff) {
<<<<<<< HEAD
		__clear_bit(msr, msr_bitmap + 0x000 / f); /* read-low */
		__clear_bit(msr, msr_bitmap + 0x800 / f); /* write-low */
	} else if ((msr >= 0xc0000000) && (msr <= 0xc0001fff)) {
		msr &= 0x1fff;
		__clear_bit(msr, msr_bitmap + 0x400 / f); /* read-high */
		__clear_bit(msr, msr_bitmap + 0xc00 / f); /* write-high */
=======
		if (type & MSR_TYPE_R)
			/* read-low */
			__set_bit(msr, msr_bitmap + 0x000 / f);

		if (type & MSR_TYPE_W)
			/* write-low */
			__set_bit(msr, msr_bitmap + 0x800 / f);

	} else if ((msr >= 0xc0000000) && (msr <= 0xc0001fff)) {
		msr &= 0x1fff;
		if (type & MSR_TYPE_R)
			/* read-high */
			__set_bit(msr, msr_bitmap + 0x400 / f);

		if (type & MSR_TYPE_W)
			/* write-high */
			__set_bit(msr, msr_bitmap + 0xc00 / f);

>>>>>>> refs/remotes/origin/master
	}
}

static void vmx_disable_intercept_for_msr(u32 msr, bool longmode_only)
{
	if (!longmode_only)
<<<<<<< HEAD
		__vmx_disable_intercept_for_msr(vmx_msr_bitmap_legacy, msr);
	__vmx_disable_intercept_for_msr(vmx_msr_bitmap_longmode, msr);
}

/*
<<<<<<< HEAD
=======
=======
		__vmx_disable_intercept_for_msr(vmx_msr_bitmap_legacy,
						msr, MSR_TYPE_R | MSR_TYPE_W);
	__vmx_disable_intercept_for_msr(vmx_msr_bitmap_longmode,
						msr, MSR_TYPE_R | MSR_TYPE_W);
}

static void vmx_enable_intercept_msr_read_x2apic(u32 msr)
{
	__vmx_enable_intercept_for_msr(vmx_msr_bitmap_legacy_x2apic,
			msr, MSR_TYPE_R);
	__vmx_enable_intercept_for_msr(vmx_msr_bitmap_longmode_x2apic,
			msr, MSR_TYPE_R);
}

static void vmx_disable_intercept_msr_read_x2apic(u32 msr)
{
	__vmx_disable_intercept_for_msr(vmx_msr_bitmap_legacy_x2apic,
			msr, MSR_TYPE_R);
	__vmx_disable_intercept_for_msr(vmx_msr_bitmap_longmode_x2apic,
			msr, MSR_TYPE_R);
}

static void vmx_disable_intercept_msr_write_x2apic(u32 msr)
{
	__vmx_disable_intercept_for_msr(vmx_msr_bitmap_legacy_x2apic,
			msr, MSR_TYPE_W);
	__vmx_disable_intercept_for_msr(vmx_msr_bitmap_longmode_x2apic,
			msr, MSR_TYPE_W);
}

static int vmx_vm_has_apicv(struct kvm *kvm)
{
	return enable_apicv && irqchip_in_kernel(kvm);
}

/*
 * Send interrupt to vcpu via posted interrupt way.
 * 1. If target vcpu is running(non-root mode), send posted interrupt
 * notification to vcpu and hardware will sync PIR to vIRR atomically.
 * 2. If target vcpu isn't running(root mode), kick it to pick up the
 * interrupt from PIR in next vmentry.
 */
static void vmx_deliver_posted_interrupt(struct kvm_vcpu *vcpu, int vector)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	int r;

	if (pi_test_and_set_pir(vector, &vmx->pi_desc))
		return;

	r = pi_test_and_set_on(&vmx->pi_desc);
	kvm_make_request(KVM_REQ_EVENT, vcpu);
#ifdef CONFIG_SMP
	if (!r && (vcpu->mode == IN_GUEST_MODE))
		apic->send_IPI_mask(get_cpu_mask(vcpu->cpu),
				POSTED_INTR_VECTOR);
	else
#endif
		kvm_vcpu_kick(vcpu);
}

static void vmx_sync_pir_to_irr(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

	if (!pi_test_and_clear_on(&vmx->pi_desc))
		return;

	kvm_apic_update_irr(vcpu, vmx->pi_desc.pir);
}

static void vmx_sync_pir_to_irr_dummy(struct kvm_vcpu *vcpu)
{
	return;
}

/*
>>>>>>> refs/remotes/origin/master
 * Set up the vmcs's constant host-state fields, i.e., host-state fields that
 * will not change in the lifetime of the guest.
 * Note that host-state that does change is set elsewhere. E.g., host-state
 * that is set differently for each CPU is set in vmx_vcpu_load(), not here.
 */
<<<<<<< HEAD
static void vmx_set_constant_host_state(void)
=======
static void vmx_set_constant_host_state(struct vcpu_vmx *vmx)
>>>>>>> refs/remotes/origin/master
{
	u32 low32, high32;
	unsigned long tmpl;
	struct desc_ptr dt;

<<<<<<< HEAD
	vmcs_writel(HOST_CR0, read_cr0() | X86_CR0_TS);  /* 22.2.3 */
=======
	vmcs_writel(HOST_CR0, read_cr0() & ~X86_CR0_TS);  /* 22.2.3 */
>>>>>>> refs/remotes/origin/master
	vmcs_writel(HOST_CR4, read_cr4());  /* 22.2.3, 22.2.5 */
	vmcs_writel(HOST_CR3, read_cr3());  /* 22.2.3  FIXME: shadow tables */

	vmcs_write16(HOST_CS_SELECTOR, __KERNEL_CS);  /* 22.2.4 */
<<<<<<< HEAD
	vmcs_write16(HOST_DS_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
	vmcs_write16(HOST_ES_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
=======
#ifdef CONFIG_X86_64
	/*
	 * Load null selectors, so we can avoid reloading them in
	 * __vmx_load_host_state(), in case userspace uses the null selectors
	 * too (the expected case).
	 */
	vmcs_write16(HOST_DS_SELECTOR, 0);
	vmcs_write16(HOST_ES_SELECTOR, 0);
#else
	vmcs_write16(HOST_DS_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
	vmcs_write16(HOST_ES_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
#endif
>>>>>>> refs/remotes/origin/master
	vmcs_write16(HOST_SS_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
	vmcs_write16(HOST_TR_SELECTOR, GDT_ENTRY_TSS*8);  /* 22.2.4 */

	native_store_idt(&dt);
	vmcs_writel(HOST_IDTR_BASE, dt.address);   /* 22.2.4 */
<<<<<<< HEAD

	asm("mov $.Lkvm_vmx_return, %0" : "=r"(tmpl));
	vmcs_writel(HOST_RIP, tmpl); /* 22.2.5 */
=======
	vmx->host_idt_base = dt.address;

	vmcs_writel(HOST_RIP, vmx_return); /* 22.2.5 */
>>>>>>> refs/remotes/origin/master

	rdmsr(MSR_IA32_SYSENTER_CS, low32, high32);
	vmcs_write32(HOST_IA32_SYSENTER_CS, low32);
	rdmsrl(MSR_IA32_SYSENTER_EIP, tmpl);
	vmcs_writel(HOST_IA32_SYSENTER_EIP, tmpl);   /* 22.2.3 */

	if (vmcs_config.vmexit_ctrl & VM_EXIT_LOAD_IA32_PAT) {
		rdmsr(MSR_IA32_CR_PAT, low32, high32);
		vmcs_write64(HOST_IA32_PAT, low32 | ((u64) high32 << 32));
	}
}

static void set_cr4_guest_host_mask(struct vcpu_vmx *vmx)
{
	vmx->vcpu.arch.cr4_guest_owned_bits = KVM_CR4_GUEST_OWNED_BITS;
	if (enable_ept)
		vmx->vcpu.arch.cr4_guest_owned_bits |= X86_CR4_PGE;
	if (is_guest_mode(&vmx->vcpu))
		vmx->vcpu.arch.cr4_guest_owned_bits &=
			~get_vmcs12(&vmx->vcpu)->cr4_guest_host_mask;
	vmcs_writel(CR4_GUEST_HOST_MASK, ~vmx->vcpu.arch.cr4_guest_owned_bits);
}

<<<<<<< HEAD
=======
static u32 vmx_pin_based_exec_ctrl(struct vcpu_vmx *vmx)
{
	u32 pin_based_exec_ctrl = vmcs_config.pin_based_exec_ctrl;

	if (!vmx_vm_has_apicv(vmx->vcpu.kvm))
		pin_based_exec_ctrl &= ~PIN_BASED_POSTED_INTR;
	return pin_based_exec_ctrl;
}

>>>>>>> refs/remotes/origin/master
static u32 vmx_exec_control(struct vcpu_vmx *vmx)
{
	u32 exec_control = vmcs_config.cpu_based_exec_ctrl;
	if (!vm_need_tpr_shadow(vmx->vcpu.kvm)) {
		exec_control &= ~CPU_BASED_TPR_SHADOW;
#ifdef CONFIG_X86_64
		exec_control |= CPU_BASED_CR8_STORE_EXITING |
				CPU_BASED_CR8_LOAD_EXITING;
#endif
	}
	if (!enable_ept)
		exec_control |= CPU_BASED_CR3_STORE_EXITING |
				CPU_BASED_CR3_LOAD_EXITING  |
				CPU_BASED_INVLPG_EXITING;
	return exec_control;
}

static u32 vmx_secondary_exec_control(struct vcpu_vmx *vmx)
{
	u32 exec_control = vmcs_config.cpu_based_2nd_exec_ctrl;
	if (!vm_need_virtualize_apic_accesses(vmx->vcpu.kvm))
		exec_control &= ~SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
	if (vmx->vpid == 0)
		exec_control &= ~SECONDARY_EXEC_ENABLE_VPID;
	if (!enable_ept) {
		exec_control &= ~SECONDARY_EXEC_ENABLE_EPT;
		enable_unrestricted_guest = 0;
<<<<<<< HEAD
=======
		/* Enable INVPCID for non-ept guests may cause performance regression. */
		exec_control &= ~SECONDARY_EXEC_ENABLE_INVPCID;
>>>>>>> refs/remotes/origin/master
	}
	if (!enable_unrestricted_guest)
		exec_control &= ~SECONDARY_EXEC_UNRESTRICTED_GUEST;
	if (!ple_gap)
		exec_control &= ~SECONDARY_EXEC_PAUSE_LOOP_EXITING;
<<<<<<< HEAD
=======
	if (!vmx_vm_has_apicv(vmx->vcpu.kvm))
		exec_control &= ~(SECONDARY_EXEC_APIC_REGISTER_VIRT |
				  SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY);
	exec_control &= ~SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE;
	/* SECONDARY_EXEC_SHADOW_VMCS is enabled when L1 executes VMPTRLD
	   (handle_vmptrld).
	   We can NOT enable shadow_vmcs here because we don't have yet
	   a current VMCS12
	*/
	exec_control &= ~SECONDARY_EXEC_SHADOW_VMCS;
>>>>>>> refs/remotes/origin/master
	return exec_control;
}

static void ept_set_mmio_spte_mask(void)
{
	/*
	 * EPT Misconfigurations can be generated if the value of bits 2:0
	 * of an EPT paging-structure entry is 110b (write/execute).
<<<<<<< HEAD
	 * Also, magic bits (0xffull << 49) is set to quickly identify mmio
	 * spte.
	 */
	kvm_mmu_set_mmio_spte_mask(0xffull << 49 | 0x6ull);
}

/*
>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * Also, magic bits (0x3ull << 62) is set to quickly identify mmio
	 * spte.
	 */
	kvm_mmu_set_mmio_spte_mask((0x3ull << 62) | 0x6ull);
}

/*
>>>>>>> refs/remotes/origin/master
 * Sets up the vmcs for emulated real mode.
 */
static int vmx_vcpu_setup(struct vcpu_vmx *vmx)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 host_sysenter_cs, msr_low, msr_high;
	u32 junk;
	u64 host_pat;
	unsigned long a;
	struct desc_ptr dt;
	int i;
	unsigned long kvm_vmx_return;
	u32 exec_control;
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_X86_64
	unsigned long a;
#endif
	int i;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* I/O */
	vmcs_write64(IO_BITMAP_A, __pa(vmx_io_bitmap_a));
	vmcs_write64(IO_BITMAP_B, __pa(vmx_io_bitmap_b));

<<<<<<< HEAD
=======
	if (enable_shadow_vmcs) {
		vmcs_write64(VMREAD_BITMAP, __pa(vmx_vmread_bitmap));
		vmcs_write64(VMWRITE_BITMAP, __pa(vmx_vmwrite_bitmap));
	}
>>>>>>> refs/remotes/origin/master
	if (cpu_has_vmx_msr_bitmap())
		vmcs_write64(MSR_BITMAP, __pa(vmx_msr_bitmap_legacy));

	vmcs_write64(VMCS_LINK_POINTER, -1ull); /* 22.3.1.5 */

	/* Control */
<<<<<<< HEAD
	vmcs_write32(PIN_BASED_VM_EXEC_CONTROL,
		vmcs_config.pin_based_exec_ctrl);

<<<<<<< HEAD
	exec_control = vmcs_config.cpu_based_exec_ctrl;
	if (!vm_need_tpr_shadow(vmx->vcpu.kvm)) {
		exec_control &= ~CPU_BASED_TPR_SHADOW;
#ifdef CONFIG_X86_64
		exec_control |= CPU_BASED_CR8_STORE_EXITING |
				CPU_BASED_CR8_LOAD_EXITING;
#endif
	}
	if (!enable_ept)
		exec_control |= CPU_BASED_CR3_STORE_EXITING |
				CPU_BASED_CR3_LOAD_EXITING  |
				CPU_BASED_INVLPG_EXITING;
	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, exec_control);

	if (cpu_has_secondary_exec_ctrls()) {
		exec_control = vmcs_config.cpu_based_2nd_exec_ctrl;
		if (!vm_need_virtualize_apic_accesses(vmx->vcpu.kvm))
			exec_control &=
				~SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
		if (vmx->vpid == 0)
			exec_control &= ~SECONDARY_EXEC_ENABLE_VPID;
		if (!enable_ept) {
			exec_control &= ~SECONDARY_EXEC_ENABLE_EPT;
			enable_unrestricted_guest = 0;
		}
		if (!enable_unrestricted_guest)
			exec_control &= ~SECONDARY_EXEC_UNRESTRICTED_GUEST;
		if (!ple_gap)
			exec_control &= ~SECONDARY_EXEC_PAUSE_LOOP_EXITING;
		vmcs_write32(SECONDARY_VM_EXEC_CONTROL, exec_control);
=======
=======
	vmcs_write32(PIN_BASED_VM_EXEC_CONTROL, vmx_pin_based_exec_ctrl(vmx));

>>>>>>> refs/remotes/origin/master
	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, vmx_exec_control(vmx));

	if (cpu_has_secondary_exec_ctrls()) {
		vmcs_write32(SECONDARY_VM_EXEC_CONTROL,
				vmx_secondary_exec_control(vmx));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	}

	if (vmx_vm_has_apicv(vmx->vcpu.kvm)) {
		vmcs_write64(EOI_EXIT_BITMAP0, 0);
		vmcs_write64(EOI_EXIT_BITMAP1, 0);
		vmcs_write64(EOI_EXIT_BITMAP2, 0);
		vmcs_write64(EOI_EXIT_BITMAP3, 0);

		vmcs_write16(GUEST_INTR_STATUS, 0);

		vmcs_write64(POSTED_INTR_NV, POSTED_INTR_VECTOR);
		vmcs_write64(POSTED_INTR_DESC_ADDR, __pa((&vmx->pi_desc)));
>>>>>>> refs/remotes/origin/master
	}

	if (ple_gap) {
		vmcs_write32(PLE_GAP, ple_gap);
		vmcs_write32(PLE_WINDOW, ple_window);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MASK, !!bypass_guest_pf);
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MATCH, !!bypass_guest_pf);
	vmcs_write32(CR3_TARGET_COUNT, 0);           /* 22.2.1 */

	vmcs_writel(HOST_CR0, read_cr0() | X86_CR0_TS);  /* 22.2.3 */
	vmcs_writel(HOST_CR4, read_cr4());  /* 22.2.3, 22.2.5 */
	vmcs_writel(HOST_CR3, read_cr3());  /* 22.2.3  FIXME: shadow tables */

	vmcs_write16(HOST_CS_SELECTOR, __KERNEL_CS);  /* 22.2.4 */
	vmcs_write16(HOST_DS_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
	vmcs_write16(HOST_ES_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
	vmcs_write16(HOST_FS_SELECTOR, 0);            /* 22.2.4 */
	vmcs_write16(HOST_GS_SELECTOR, 0);            /* 22.2.4 */
	vmcs_write16(HOST_SS_SELECTOR, __KERNEL_DS);  /* 22.2.4 */
=======
=======
>>>>>>> refs/remotes/origin/master
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MASK, 0);
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MATCH, 0);
	vmcs_write32(CR3_TARGET_COUNT, 0);           /* 22.2.1 */

	vmcs_write16(HOST_FS_SELECTOR, 0);            /* 22.2.4 */
	vmcs_write16(HOST_GS_SELECTOR, 0);            /* 22.2.4 */
<<<<<<< HEAD
	vmx_set_constant_host_state();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	vmx_set_constant_host_state(vmx);
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_X86_64
	rdmsrl(MSR_FS_BASE, a);
	vmcs_writel(HOST_FS_BASE, a); /* 22.2.4 */
	rdmsrl(MSR_GS_BASE, a);
	vmcs_writel(HOST_GS_BASE, a); /* 22.2.4 */
#else
	vmcs_writel(HOST_FS_BASE, 0); /* 22.2.4 */
	vmcs_writel(HOST_GS_BASE, 0); /* 22.2.4 */
#endif

<<<<<<< HEAD
<<<<<<< HEAD
	vmcs_write16(HOST_TR_SELECTOR, GDT_ENTRY_TSS*8);  /* 22.2.4 */

	native_store_idt(&dt);
	vmcs_writel(HOST_IDTR_BASE, dt.address);   /* 22.2.4 */

	asm("mov $.Lkvm_vmx_return, %0" : "=r"(kvm_vmx_return));
	vmcs_writel(HOST_RIP, kvm_vmx_return); /* 22.2.5 */
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	vmcs_write32(VM_EXIT_MSR_STORE_COUNT, 0);
	vmcs_write32(VM_EXIT_MSR_LOAD_COUNT, 0);
	vmcs_write64(VM_EXIT_MSR_LOAD_ADDR, __pa(vmx->msr_autoload.host));
	vmcs_write32(VM_ENTRY_MSR_LOAD_COUNT, 0);
	vmcs_write64(VM_ENTRY_MSR_LOAD_ADDR, __pa(vmx->msr_autoload.guest));

<<<<<<< HEAD
<<<<<<< HEAD
	rdmsr(MSR_IA32_SYSENTER_CS, host_sysenter_cs, junk);
	vmcs_write32(HOST_IA32_SYSENTER_CS, host_sysenter_cs);
	rdmsrl(MSR_IA32_SYSENTER_ESP, a);
	vmcs_writel(HOST_IA32_SYSENTER_ESP, a);   /* 22.2.3 */
	rdmsrl(MSR_IA32_SYSENTER_EIP, a);
	vmcs_writel(HOST_IA32_SYSENTER_EIP, a);   /* 22.2.3 */

	if (vmcs_config.vmexit_ctrl & VM_EXIT_LOAD_IA32_PAT) {
		rdmsr(MSR_IA32_CR_PAT, msr_low, msr_high);
		host_pat = msr_low | ((u64) msr_high << 32);
		vmcs_write64(HOST_IA32_PAT, host_pat);
	}
	if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT) {
=======
	if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT) {
		u32 msr_low, msr_high;
		u64 host_pat;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT) {
		u32 msr_low, msr_high;
		u64 host_pat;
>>>>>>> refs/remotes/origin/master
		rdmsr(MSR_IA32_CR_PAT, msr_low, msr_high);
		host_pat = msr_low | ((u64) msr_high << 32);
		/* Write the default value follow host pat */
		vmcs_write64(GUEST_IA32_PAT, host_pat);
		/* Keep arch.pat sync with GUEST_IA32_PAT */
		vmx->vcpu.arch.pat = host_pat;
	}

	for (i = 0; i < NR_VMX_MSR; ++i) {
		u32 index = vmx_msr_index[i];
		u32 data_low, data_high;
		int j = vmx->nmsrs;

		if (rdmsr_safe(index, &data_low, &data_high) < 0)
			continue;
		if (wrmsr_safe(index, data_low, data_high) < 0)
			continue;
		vmx->guest_msrs[j].index = i;
		vmx->guest_msrs[j].data = 0;
		vmx->guest_msrs[j].mask = -1ull;
		++vmx->nmsrs;
	}

	vmcs_write32(VM_EXIT_CONTROLS, vmcs_config.vmexit_ctrl);

	/* 22.2.1, 20.8.1 */
	vmcs_write32(VM_ENTRY_CONTROLS, vmcs_config.vmentry_ctrl);

	vmcs_writel(CR0_GUEST_HOST_MASK, ~0UL);
<<<<<<< HEAD
<<<<<<< HEAD
	vmx->vcpu.arch.cr4_guest_owned_bits = KVM_CR4_GUEST_OWNED_BITS;
	if (enable_ept)
		vmx->vcpu.arch.cr4_guest_owned_bits |= X86_CR4_PGE;
	vmcs_writel(CR4_GUEST_HOST_MASK, ~vmx->vcpu.arch.cr4_guest_owned_bits);
=======
	set_cr4_guest_host_mask(vmx);
>>>>>>> refs/remotes/origin/cm-10.0

	kvm_write_tsc(&vmx->vcpu, 0);
=======
	set_cr4_guest_host_mask(vmx);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int vmx_vcpu_reset(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u64 msr;
	int ret;

	vcpu->arch.regs_avail = ~((1 << VCPU_REGS_RIP) | (1 << VCPU_REGS_RSP));
=======
static void vmx_vcpu_reset(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u64 msr;
>>>>>>> refs/remotes/origin/master

	vmx->rmode.vm86_active = 0;

	vmx->soft_vnmi_blocked = 0;

	vmx->vcpu.arch.regs[VCPU_REGS_RDX] = get_rdx_init_val();
	kvm_set_cr8(&vmx->vcpu, 0);
	msr = 0xfee00000 | MSR_IA32_APICBASE_ENABLE;
	if (kvm_vcpu_is_bsp(&vmx->vcpu))
		msr |= MSR_IA32_APICBASE_BSP;
	kvm_set_apic_base(&vmx->vcpu, msr);

<<<<<<< HEAD
	ret = fx_init(&vmx->vcpu);
	if (ret != 0)
		goto out;

	vmx_segment_cache_clear(vmx);

	seg_setup(VCPU_SREG_CS);
	/*
	 * GUEST_CS_BASE should really be 0xffff0000, but VT vm86 mode
	 * insists on having GUEST_CS_BASE == GUEST_CS_SELECTOR << 4.  Sigh.
	 */
	if (kvm_vcpu_is_bsp(&vmx->vcpu)) {
		vmcs_write16(GUEST_CS_SELECTOR, 0xf000);
		vmcs_writel(GUEST_CS_BASE, 0x000f0000);
	} else {
		vmcs_write16(GUEST_CS_SELECTOR, vmx->vcpu.arch.sipi_vector << 8);
		vmcs_writel(GUEST_CS_BASE, vmx->vcpu.arch.sipi_vector << 12);
	}
=======
	vmx_segment_cache_clear(vmx);

	seg_setup(VCPU_SREG_CS);
	vmcs_write16(GUEST_CS_SELECTOR, 0xf000);
	vmcs_write32(GUEST_CS_BASE, 0xffff0000);
>>>>>>> refs/remotes/origin/master

	seg_setup(VCPU_SREG_DS);
	seg_setup(VCPU_SREG_ES);
	seg_setup(VCPU_SREG_FS);
	seg_setup(VCPU_SREG_GS);
	seg_setup(VCPU_SREG_SS);

	vmcs_write16(GUEST_TR_SELECTOR, 0);
	vmcs_writel(GUEST_TR_BASE, 0);
	vmcs_write32(GUEST_TR_LIMIT, 0xffff);
	vmcs_write32(GUEST_TR_AR_BYTES, 0x008b);

	vmcs_write16(GUEST_LDTR_SELECTOR, 0);
	vmcs_writel(GUEST_LDTR_BASE, 0);
	vmcs_write32(GUEST_LDTR_LIMIT, 0xffff);
	vmcs_write32(GUEST_LDTR_AR_BYTES, 0x00082);

	vmcs_write32(GUEST_SYSENTER_CS, 0);
	vmcs_writel(GUEST_SYSENTER_ESP, 0);
	vmcs_writel(GUEST_SYSENTER_EIP, 0);

	vmcs_writel(GUEST_RFLAGS, 0x02);
<<<<<<< HEAD
	if (kvm_vcpu_is_bsp(&vmx->vcpu))
		kvm_rip_write(vcpu, 0xfff0);
	else
		kvm_rip_write(vcpu, 0);
	kvm_register_write(vcpu, VCPU_REGS_RSP, 0);

	vmcs_writel(GUEST_DR7, 0x400);
=======
	kvm_rip_write(vcpu, 0xfff0);
>>>>>>> refs/remotes/origin/master

	vmcs_writel(GUEST_GDTR_BASE, 0);
	vmcs_write32(GUEST_GDTR_LIMIT, 0xffff);

	vmcs_writel(GUEST_IDTR_BASE, 0);
	vmcs_write32(GUEST_IDTR_LIMIT, 0xffff);

	vmcs_write32(GUEST_ACTIVITY_STATE, GUEST_ACTIVITY_ACTIVE);
	vmcs_write32(GUEST_INTERRUPTIBILITY_INFO, 0);
	vmcs_write32(GUEST_PENDING_DBG_EXCEPTIONS, 0);

	/* Special registers */
	vmcs_write64(GUEST_IA32_DEBUGCTL, 0);

	setup_msrs(vmx);

	vmcs_write32(VM_ENTRY_INTR_INFO_FIELD, 0);  /* 22.2.1 */

	if (cpu_has_vmx_tpr_shadow()) {
		vmcs_write64(VIRTUAL_APIC_PAGE_ADDR, 0);
		if (vm_need_tpr_shadow(vmx->vcpu.kvm))
			vmcs_write64(VIRTUAL_APIC_PAGE_ADDR,
				     __pa(vmx->vcpu.arch.apic->regs));
		vmcs_write32(TPR_THRESHOLD, 0);
	}

	if (vm_need_virtualize_apic_accesses(vmx->vcpu.kvm))
		vmcs_write64(APIC_ACCESS_ADDR,
			     page_to_phys(vmx->vcpu.kvm->arch.apic_access_page));

<<<<<<< HEAD
=======
	if (vmx_vm_has_apicv(vcpu->kvm))
		memset(&vmx->pi_desc, 0, sizeof(struct pi_desc));

>>>>>>> refs/remotes/origin/master
	if (vmx->vpid != 0)
		vmcs_write16(VIRTUAL_PROCESSOR_ID, vmx->vpid);

	vmx->vcpu.arch.cr0 = X86_CR0_NW | X86_CR0_CD | X86_CR0_ET;
<<<<<<< HEAD
<<<<<<< HEAD
	vmx_set_cr0(&vmx->vcpu, kvm_read_cr0(vcpu)); /* enter rmode */
=======
	vcpu->srcu_idx = srcu_read_lock(&vcpu->kvm->srcu);
	vmx_set_cr0(&vmx->vcpu, kvm_read_cr0(vcpu)); /* enter rmode */
	srcu_read_unlock(&vcpu->kvm->srcu, vcpu->srcu_idx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	vmx_set_cr0(&vmx->vcpu, kvm_read_cr0(vcpu)); /* enter rmode */
>>>>>>> refs/remotes/origin/master
	vmx_set_cr4(&vmx->vcpu, 0);
	vmx_set_efer(&vmx->vcpu, 0);
	vmx_fpu_activate(&vmx->vcpu);
	update_exception_bitmap(&vmx->vcpu);

	vpid_sync_context(vmx);
<<<<<<< HEAD

	ret = 0;

	/* HACK: Don't enable emulation on guest boot/reset */
	vmx->emulation_required = 0;

out:
	return ret;
}

<<<<<<< HEAD
static void enable_irq_window(struct kvm_vcpu *vcpu)
{
	u32 cpu_based_vm_exec_control;
=======
=======
}

>>>>>>> refs/remotes/origin/master
/*
 * In nested virtualization, check if L1 asked to exit on external interrupts.
 * For most existing hypervisors, this will always return true.
 */
static bool nested_exit_on_intr(struct kvm_vcpu *vcpu)
{
	return get_vmcs12(vcpu)->pin_based_vm_exec_control &
		PIN_BASED_EXT_INTR_MASK;
}

<<<<<<< HEAD
static void enable_irq_window(struct kvm_vcpu *vcpu)
{
	u32 cpu_based_vm_exec_control;
	if (is_guest_mode(vcpu) && nested_exit_on_intr(vcpu)) {
		/*
		 * We get here if vmx_interrupt_allowed() said we can't
		 * inject to L1 now because L2 must run. Ask L2 to exit
		 * right after entry, so we can inject to L1 more promptly.
		 */
		kvm_make_request(KVM_REQ_IMMEDIATE_EXIT, vcpu);
		return;
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool nested_exit_on_nmi(struct kvm_vcpu *vcpu)
{
	return get_vmcs12(vcpu)->pin_based_vm_exec_control &
		PIN_BASED_NMI_EXITING;
}

static int enable_irq_window(struct kvm_vcpu *vcpu)
{
	u32 cpu_based_vm_exec_control;

	if (is_guest_mode(vcpu) && nested_exit_on_intr(vcpu))
		/*
		 * We get here if vmx_interrupt_allowed() said we can't
		 * inject to L1 now because L2 must run. The caller will have
		 * to make L2 exit right after entry, so we can inject to L1
		 * more promptly.
		 */
		return -EBUSY;
>>>>>>> refs/remotes/origin/master

	cpu_based_vm_exec_control = vmcs_read32(CPU_BASED_VM_EXEC_CONTROL);
	cpu_based_vm_exec_control |= CPU_BASED_VIRTUAL_INTR_PENDING;
	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, cpu_based_vm_exec_control);
<<<<<<< HEAD
}

static void enable_nmi_window(struct kvm_vcpu *vcpu)
{
	u32 cpu_based_vm_exec_control;

	if (!cpu_has_virtual_nmis()) {
		enable_irq_window(vcpu);
		return;
	}

	if (vmcs_read32(GUEST_INTERRUPTIBILITY_INFO) & GUEST_INTR_STATE_STI) {
		enable_irq_window(vcpu);
		return;
	}
	cpu_based_vm_exec_control = vmcs_read32(CPU_BASED_VM_EXEC_CONTROL);
	cpu_based_vm_exec_control |= CPU_BASED_VIRTUAL_NMI_PENDING;
	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, cpu_based_vm_exec_control);
=======
	return 0;
}

static int enable_nmi_window(struct kvm_vcpu *vcpu)
{
	u32 cpu_based_vm_exec_control;

	if (!cpu_has_virtual_nmis())
		return enable_irq_window(vcpu);

	if (vmcs_read32(GUEST_INTERRUPTIBILITY_INFO) & GUEST_INTR_STATE_STI)
		return enable_irq_window(vcpu);

	cpu_based_vm_exec_control = vmcs_read32(CPU_BASED_VM_EXEC_CONTROL);
	cpu_based_vm_exec_control |= CPU_BASED_VIRTUAL_NMI_PENDING;
	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, cpu_based_vm_exec_control);
	return 0;
>>>>>>> refs/remotes/origin/master
}

static void vmx_inject_irq(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	uint32_t intr;
	int irq = vcpu->arch.interrupt.nr;

	trace_kvm_inj_virq(irq);

	++vcpu->stat.irq_injections;
	if (vmx->rmode.vm86_active) {
		int inc_eip = 0;
		if (vcpu->arch.interrupt.soft)
			inc_eip = vcpu->arch.event_exit_inst_len;
		if (kvm_inject_realmode_interrupt(vcpu, irq, inc_eip) != EMULATE_DONE)
			kvm_make_request(KVM_REQ_TRIPLE_FAULT, vcpu);
		return;
	}
	intr = irq | INTR_INFO_VALID_MASK;
	if (vcpu->arch.interrupt.soft) {
		intr |= INTR_TYPE_SOFT_INTR;
		vmcs_write32(VM_ENTRY_INSTRUCTION_LEN,
			     vmx->vcpu.arch.event_exit_inst_len);
	} else
		intr |= INTR_TYPE_EXT_INTR;
	vmcs_write32(VM_ENTRY_INTR_INFO_FIELD, intr);
<<<<<<< HEAD
<<<<<<< HEAD
	vmx_clear_hlt(vcpu);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void vmx_inject_nmi(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (is_guest_mode(vcpu))
		return;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (is_guest_mode(vcpu))
		return;

>>>>>>> refs/remotes/origin/master
	if (!cpu_has_virtual_nmis()) {
		/*
		 * Tracking the NMI-blocked state in software is built upon
		 * finding the next open IRQ window. This, in turn, depends on
		 * well-behaving guests: They have to keep IRQs disabled at
		 * least as long as the NMI handler runs. Otherwise we may
		 * cause NMI nesting, maybe breaking the guest. But as this is
		 * highly unlikely, we can live with the residual risk.
		 */
		vmx->soft_vnmi_blocked = 1;
		vmx->vnmi_blocked_time = 0;
	}

	++vcpu->stat.nmi_injections;
	vmx->nmi_known_unmasked = false;
	if (vmx->rmode.vm86_active) {
		if (kvm_inject_realmode_interrupt(vcpu, NMI_VECTOR, 0) != EMULATE_DONE)
			kvm_make_request(KVM_REQ_TRIPLE_FAULT, vcpu);
		return;
	}
	vmcs_write32(VM_ENTRY_INTR_INFO_FIELD,
			INTR_TYPE_NMI_INTR | INTR_INFO_VALID_MASK | NMI_VECTOR);
<<<<<<< HEAD
<<<<<<< HEAD
	vmx_clear_hlt(vcpu);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

static int vmx_nmi_allowed(struct kvm_vcpu *vcpu)
{
	if (!cpu_has_virtual_nmis() && to_vmx(vcpu)->soft_vnmi_blocked)
		return 0;

	return	!(vmcs_read32(GUEST_INTERRUPTIBILITY_INFO) &
		  (GUEST_INTR_STATE_MOV_SS | GUEST_INTR_STATE_STI
		   | GUEST_INTR_STATE_NMI));
=======
>>>>>>> refs/remotes/origin/master
}

static bool vmx_get_nmi_mask(struct kvm_vcpu *vcpu)
{
	if (!cpu_has_virtual_nmis())
		return to_vmx(vcpu)->soft_vnmi_blocked;
	if (to_vmx(vcpu)->nmi_known_unmasked)
		return false;
	return vmcs_read32(GUEST_INTERRUPTIBILITY_INFO)	& GUEST_INTR_STATE_NMI;
}

static void vmx_set_nmi_mask(struct kvm_vcpu *vcpu, bool masked)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

	if (!cpu_has_virtual_nmis()) {
		if (vmx->soft_vnmi_blocked != masked) {
			vmx->soft_vnmi_blocked = masked;
			vmx->vnmi_blocked_time = 0;
		}
	} else {
		vmx->nmi_known_unmasked = !masked;
		if (masked)
			vmcs_set_bits(GUEST_INTERRUPTIBILITY_INFO,
				      GUEST_INTR_STATE_NMI);
		else
			vmcs_clear_bits(GUEST_INTERRUPTIBILITY_INFO,
					GUEST_INTR_STATE_NMI);
	}
}

<<<<<<< HEAD
static int vmx_interrupt_allowed(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
=======
	if (is_guest_mode(vcpu) && nested_exit_on_intr(vcpu)) {
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
		if (to_vmx(vcpu)->nested.nested_run_pending ||
		    (vmcs12->idt_vectoring_info_field &
		     VECTORING_INFO_VALID_MASK))
			return 0;
		nested_vmx_vmexit(vcpu);
		vmcs12->vm_exit_reason = EXIT_REASON_EXTERNAL_INTERRUPT;
		vmcs12->vm_exit_intr_info = 0;
		/* fall through to normal code, but now in L1, not L2 */
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static int vmx_nmi_allowed(struct kvm_vcpu *vcpu)
{
	if (is_guest_mode(vcpu)) {
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);

		if (to_vmx(vcpu)->nested.nested_run_pending)
			return 0;
		if (nested_exit_on_nmi(vcpu)) {
			nested_vmx_vmexit(vcpu);
			vmcs12->vm_exit_reason = EXIT_REASON_EXCEPTION_NMI;
			vmcs12->vm_exit_intr_info = NMI_VECTOR |
				INTR_TYPE_NMI_INTR | INTR_INFO_VALID_MASK;
			/*
			 * The NMI-triggered VM exit counts as injection:
			 * clear this one and block further NMIs.
			 */
			vcpu->arch.nmi_pending = 0;
			vmx_set_nmi_mask(vcpu, true);
			return 0;
		}
	}

	if (!cpu_has_virtual_nmis() && to_vmx(vcpu)->soft_vnmi_blocked)
		return 0;

	return	!(vmcs_read32(GUEST_INTERRUPTIBILITY_INFO) &
		  (GUEST_INTR_STATE_MOV_SS | GUEST_INTR_STATE_STI
		   | GUEST_INTR_STATE_NMI));
}

static int vmx_interrupt_allowed(struct kvm_vcpu *vcpu)
{
	if (is_guest_mode(vcpu)) {
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);

		if (to_vmx(vcpu)->nested.nested_run_pending)
			return 0;
		if (nested_exit_on_intr(vcpu)) {
			nested_vmx_vmexit(vcpu);
			vmcs12->vm_exit_reason =
				EXIT_REASON_EXTERNAL_INTERRUPT;
			vmcs12->vm_exit_intr_info = 0;
			/*
			 * fall through to normal code, but now in L1, not L2
			 */
		}
	}

>>>>>>> refs/remotes/origin/master
	return (vmcs_readl(GUEST_RFLAGS) & X86_EFLAGS_IF) &&
		!(vmcs_read32(GUEST_INTERRUPTIBILITY_INFO) &
			(GUEST_INTR_STATE_STI | GUEST_INTR_STATE_MOV_SS));
}

static int vmx_set_tss_addr(struct kvm *kvm, unsigned int addr)
{
	int ret;
	struct kvm_userspace_memory_region tss_mem = {
		.slot = TSS_PRIVATE_MEMSLOT,
		.guest_phys_addr = addr,
		.memory_size = PAGE_SIZE * 3,
		.flags = 0,
	};

<<<<<<< HEAD
	ret = kvm_set_memory_region(kvm, &tss_mem, 0);
=======
	ret = kvm_set_memory_region(kvm, &tss_mem);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;
	kvm->arch.tss_addr = addr;
	if (!init_rmode_tss(kvm))
		return  -ENOMEM;

	return 0;
}

<<<<<<< HEAD
static int handle_rmode_exception(struct kvm_vcpu *vcpu,
				  int vec, u32 err_code)
{
	/*
	 * Instruction with address size override prefix opcode 0x67
	 * Cause the #SS fault with 0 error code in VM86 mode.
	 */
	if (((vec == GP_VECTOR) || (vec == SS_VECTOR)) && err_code == 0)
		if (emulate_instruction(vcpu, 0) == EMULATE_DONE)
			return 1;
	/*
	 * Forward all other exceptions that are valid in real mode.
	 * FIXME: Breaks guest debugging in real mode, needs to be fixed with
	 *        the required debugging infrastructure rework.
	 */
	switch (vec) {
	case DB_VECTOR:
		if (vcpu->guest_debug &
		    (KVM_GUESTDBG_SINGLESTEP | KVM_GUESTDBG_USE_HW_BP))
			return 0;
		kvm_queue_exception(vcpu, vec);
		return 1;
=======
static bool rmode_exception(struct kvm_vcpu *vcpu, int vec)
{
	switch (vec) {
>>>>>>> refs/remotes/origin/master
	case BP_VECTOR:
		/*
		 * Update instruction length as we may reinject the exception
		 * from user space while in guest debugging mode.
		 */
		to_vmx(vcpu)->vcpu.arch.event_exit_inst_len =
			vmcs_read32(VM_EXIT_INSTRUCTION_LEN);
		if (vcpu->guest_debug & KVM_GUESTDBG_USE_SW_BP)
<<<<<<< HEAD
			return 0;
=======
			return false;
		/* fall through */
	case DB_VECTOR:
		if (vcpu->guest_debug &
			(KVM_GUESTDBG_SINGLESTEP | KVM_GUESTDBG_USE_HW_BP))
			return false;
>>>>>>> refs/remotes/origin/master
		/* fall through */
	case DE_VECTOR:
	case OF_VECTOR:
	case BR_VECTOR:
	case UD_VECTOR:
	case DF_VECTOR:
	case SS_VECTOR:
	case GP_VECTOR:
	case MF_VECTOR:
<<<<<<< HEAD
		kvm_queue_exception(vcpu, vec);
		return 1;
	}
	return 0;
=======
		return true;
	break;
	}
	return false;
}

static int handle_rmode_exception(struct kvm_vcpu *vcpu,
				  int vec, u32 err_code)
{
	/*
	 * Instruction with address size override prefix opcode 0x67
	 * Cause the #SS fault with 0 error code in VM86 mode.
	 */
	if (((vec == GP_VECTOR) || (vec == SS_VECTOR)) && err_code == 0) {
		if (emulate_instruction(vcpu, 0) == EMULATE_DONE) {
			if (vcpu->arch.halt_request) {
				vcpu->arch.halt_request = 0;
				return kvm_emulate_halt(vcpu);
			}
			return 1;
		}
		return 0;
	}

	/*
	 * Forward all other exceptions that are valid in real mode.
	 * FIXME: Breaks guest debugging in real mode, needs to be fixed with
	 *        the required debugging infrastructure rework.
	 */
	kvm_queue_exception(vcpu, vec);
	return 1;
>>>>>>> refs/remotes/origin/master
}

/*
 * Trigger machine check on the host. We assume all the MSRs are already set up
 * by the CPU and that we still run on the same CPU as the MCE occurred on.
 * We pass a fake environment to the machine check handler because we want
 * the guest to be always treated like user space, no matter what context
 * it used internally.
 */
static void kvm_machine_check(void)
{
#if defined(CONFIG_X86_MCE) && defined(CONFIG_X86_64)
	struct pt_regs regs = {
		.cs = 3, /* Fake ring 3 no matter what the guest ran on */
		.flags = X86_EFLAGS_IF,
	};

	do_machine_check(&regs, 0);
#endif
}

static int handle_machine_check(struct kvm_vcpu *vcpu)
{
	/* already handled by vcpu_run */
	return 1;
}

static int handle_exception(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	struct kvm_run *kvm_run = vcpu->run;
	u32 intr_info, ex_no, error_code;
	unsigned long cr2, rip, dr6;
	u32 vect_info;
	enum emulation_result er;

	vect_info = vmx->idt_vectoring_info;
	intr_info = vmx->exit_intr_info;

	if (is_machine_check(intr_info))
		return handle_machine_check(vcpu);

<<<<<<< HEAD
	if ((vect_info & VECTORING_INFO_VALID_MASK) &&
	    !is_page_fault(intr_info)) {
		vcpu->run->exit_reason = KVM_EXIT_INTERNAL_ERROR;
		vcpu->run->internal.suberror = KVM_INTERNAL_ERROR_SIMUL_EX;
		vcpu->run->internal.ndata = 2;
		vcpu->run->internal.data[0] = vect_info;
		vcpu->run->internal.data[1] = intr_info;
		return 0;
	}

=======
>>>>>>> refs/remotes/origin/master
	if ((intr_info & INTR_INFO_INTR_TYPE_MASK) == INTR_TYPE_NMI_INTR)
		return 1;  /* already handled by vmx_vcpu_run() */

	if (is_no_device(intr_info)) {
		vmx_fpu_activate(vcpu);
		return 1;
	}

	if (is_invalid_opcode(intr_info)) {
		er = emulate_instruction(vcpu, EMULTYPE_TRAP_UD);
		if (er != EMULATE_DONE)
			kvm_queue_exception(vcpu, UD_VECTOR);
		return 1;
	}

	error_code = 0;
	if (intr_info & INTR_INFO_DELIVER_CODE_MASK)
		error_code = vmcs_read32(VM_EXIT_INTR_ERROR_CODE);
<<<<<<< HEAD
	if (is_page_fault(intr_info)) {
		/* EPT won't cause page fault directly */
<<<<<<< HEAD
		if (enable_ept)
			BUG();
=======
		BUG_ON(enable_ept);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	/*
	 * The #PF with PFEC.RSVD = 1 indicates the guest is accessing
	 * MMIO, it is better to report an internal error.
	 * See the comments in vmx_handle_exit.
	 */
	if ((vect_info & VECTORING_INFO_VALID_MASK) &&
	    !(is_page_fault(intr_info) && !(error_code & PFERR_RSVD_MASK))) {
		vcpu->run->exit_reason = KVM_EXIT_INTERNAL_ERROR;
		vcpu->run->internal.suberror = KVM_INTERNAL_ERROR_SIMUL_EX;
		vcpu->run->internal.ndata = 2;
		vcpu->run->internal.data[0] = vect_info;
		vcpu->run->internal.data[1] = intr_info;
		return 0;
	}

	if (is_page_fault(intr_info)) {
		/* EPT won't cause page fault directly */
		BUG_ON(enable_ept);
>>>>>>> refs/remotes/origin/master
		cr2 = vmcs_readl(EXIT_QUALIFICATION);
		trace_kvm_page_fault(cr2, error_code);

		if (kvm_event_needs_reinjection(vcpu))
			kvm_mmu_unprotect_page_virt(vcpu, cr2);
		return kvm_mmu_page_fault(vcpu, cr2, error_code, NULL, 0);
	}

<<<<<<< HEAD
	if (vmx->rmode.vm86_active &&
	    handle_rmode_exception(vcpu, intr_info & INTR_INFO_VECTOR_MASK,
								error_code)) {
		if (vcpu->arch.halt_request) {
			vcpu->arch.halt_request = 0;
			return kvm_emulate_halt(vcpu);
		}
		return 1;
	}

	ex_no = intr_info & INTR_INFO_VECTOR_MASK;
=======
	ex_no = intr_info & INTR_INFO_VECTOR_MASK;

	if (vmx->rmode.vm86_active && rmode_exception(vcpu, ex_no))
		return handle_rmode_exception(vcpu, ex_no, error_code);

>>>>>>> refs/remotes/origin/master
	switch (ex_no) {
	case DB_VECTOR:
		dr6 = vmcs_readl(EXIT_QUALIFICATION);
		if (!(vcpu->guest_debug &
		      (KVM_GUESTDBG_SINGLESTEP | KVM_GUESTDBG_USE_HW_BP))) {
			vcpu->arch.dr6 = dr6 | DR6_FIXED_1;
			kvm_queue_exception(vcpu, DB_VECTOR);
			return 1;
		}
		kvm_run->debug.arch.dr6 = dr6 | DR6_FIXED_1;
		kvm_run->debug.arch.dr7 = vmcs_readl(GUEST_DR7);
		/* fall through */
	case BP_VECTOR:
		/*
		 * Update instruction length as we may reinject #BP from
		 * user space while in guest debugging mode. Reading it for
		 * #DB as well causes no harm, it is not used in that case.
		 */
		vmx->vcpu.arch.event_exit_inst_len =
			vmcs_read32(VM_EXIT_INSTRUCTION_LEN);
		kvm_run->exit_reason = KVM_EXIT_DEBUG;
		rip = kvm_rip_read(vcpu);
		kvm_run->debug.arch.pc = vmcs_readl(GUEST_CS_BASE) + rip;
		kvm_run->debug.arch.exception = ex_no;
		break;
	default:
		kvm_run->exit_reason = KVM_EXIT_EXCEPTION;
		kvm_run->ex.exception = ex_no;
		kvm_run->ex.error_code = error_code;
		break;
	}
	return 0;
}

static int handle_external_interrupt(struct kvm_vcpu *vcpu)
{
	++vcpu->stat.irq_exits;
	return 1;
}

static int handle_triple_fault(struct kvm_vcpu *vcpu)
{
	vcpu->run->exit_reason = KVM_EXIT_SHUTDOWN;
	return 0;
}

static int handle_io(struct kvm_vcpu *vcpu)
{
	unsigned long exit_qualification;
	int size, in, string;
	unsigned port;

	exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	string = (exit_qualification & 16) != 0;
	in = (exit_qualification & 8) != 0;

	++vcpu->stat.io_exits;

	if (string || in)
		return emulate_instruction(vcpu, 0) == EMULATE_DONE;

	port = exit_qualification >> 16;
	size = (exit_qualification & 7) + 1;
	skip_emulated_instruction(vcpu);

	return kvm_fast_pio_out(vcpu, size, port);
}

static void
vmx_patch_hypercall(struct kvm_vcpu *vcpu, unsigned char *hypercall)
{
	/*
	 * Patch in the VMCALL instruction:
	 */
	hypercall[0] = 0x0f;
	hypercall[1] = 0x01;
	hypercall[2] = 0xc1;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* called to set cr0 as approriate for a mov-to-cr0 exit. */
static int handle_set_cr0(struct kvm_vcpu *vcpu, unsigned long val)
{
	if (to_vmx(vcpu)->nested.vmxon &&
	    ((val & VMXON_CR0_ALWAYSON) != VMXON_CR0_ALWAYSON))
		return 1;

	if (is_guest_mode(vcpu)) {
		/*
		 * We get here when L2 changed cr0 in a way that did not change
		 * any of L1's shadowed bits (see nested_vmx_exit_handled_cr),
		 * but did change L0 shadowed bits. This can currently happen
		 * with the TS bit: L0 may want to leave TS on (for lazy fpu
		 * loading) while pretending to allow the guest to change it.
		 */
		if (kvm_set_cr0(vcpu, (val & vcpu->arch.cr0_guest_owned_bits) |
			 (vcpu->arch.cr0 & ~vcpu->arch.cr0_guest_owned_bits)))
			return 1;
		vmcs_writel(CR0_READ_SHADOW, val);
		return 0;
	} else
		return kvm_set_cr0(vcpu, val);
=======
static bool nested_cr0_valid(struct vmcs12 *vmcs12, unsigned long val)
{
	unsigned long always_on = VMXON_CR0_ALWAYSON;

	if (nested_vmx_secondary_ctls_high &
		SECONDARY_EXEC_UNRESTRICTED_GUEST &&
	    nested_cpu_has2(vmcs12, SECONDARY_EXEC_UNRESTRICTED_GUEST))
		always_on &= ~(X86_CR0_PE | X86_CR0_PG);
	return (val & always_on) == always_on;
}

/* called to set cr0 as appropriate for a mov-to-cr0 exit. */
static int handle_set_cr0(struct kvm_vcpu *vcpu, unsigned long val)
{
	if (is_guest_mode(vcpu)) {
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
		unsigned long orig_val = val;

		/*
		 * We get here when L2 changed cr0 in a way that did not change
		 * any of L1's shadowed bits (see nested_vmx_exit_handled_cr),
		 * but did change L0 shadowed bits. So we first calculate the
		 * effective cr0 value that L1 would like to write into the
		 * hardware. It consists of the L2-owned bits from the new
		 * value combined with the L1-owned bits from L1's guest_cr0.
		 */
		val = (val & ~vmcs12->cr0_guest_host_mask) |
			(vmcs12->guest_cr0 & vmcs12->cr0_guest_host_mask);

		if (!nested_cr0_valid(vmcs12, val))
			return 1;

		if (kvm_set_cr0(vcpu, val))
			return 1;
		vmcs_writel(CR0_READ_SHADOW, orig_val);
		return 0;
	} else {
		if (to_vmx(vcpu)->nested.vmxon &&
		    ((val & VMXON_CR0_ALWAYSON) != VMXON_CR0_ALWAYSON))
			return 1;
		return kvm_set_cr0(vcpu, val);
	}
>>>>>>> refs/remotes/origin/master
}

static int handle_set_cr4(struct kvm_vcpu *vcpu, unsigned long val)
{
	if (is_guest_mode(vcpu)) {
<<<<<<< HEAD
		if (kvm_set_cr4(vcpu, (val & vcpu->arch.cr4_guest_owned_bits) |
			 (vcpu->arch.cr4 & ~vcpu->arch.cr4_guest_owned_bits)))
			return 1;
		vmcs_writel(CR4_READ_SHADOW, val);
=======
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
		unsigned long orig_val = val;

		/* analogously to handle_set_cr0 */
		val = (val & ~vmcs12->cr4_guest_host_mask) |
			(vmcs12->guest_cr4 & vmcs12->cr4_guest_host_mask);
		if (kvm_set_cr4(vcpu, val))
			return 1;
		vmcs_writel(CR4_READ_SHADOW, orig_val);
>>>>>>> refs/remotes/origin/master
		return 0;
	} else
		return kvm_set_cr4(vcpu, val);
}

/* called to set cr0 as approriate for clts instruction exit. */
static void handle_clts(struct kvm_vcpu *vcpu)
{
	if (is_guest_mode(vcpu)) {
		/*
		 * We get here when L2 did CLTS, and L1 didn't shadow CR0.TS
		 * but we did (!fpu_active). We need to keep GUEST_CR0.TS on,
		 * just pretend it's off (also in arch.cr0 for fpu_activate).
		 */
		vmcs_writel(CR0_READ_SHADOW,
			vmcs_readl(CR0_READ_SHADOW) & ~X86_CR0_TS);
		vcpu->arch.cr0 &= ~X86_CR0_TS;
	} else
		vmx_set_cr0(vcpu, kvm_read_cr0_bits(vcpu, ~X86_CR0_TS));
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int handle_cr(struct kvm_vcpu *vcpu)
{
	unsigned long exit_qualification, val;
	int cr;
	int reg;
	int err;

	exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	cr = exit_qualification & 15;
	reg = (exit_qualification >> 8) & 15;
	switch ((exit_qualification >> 4) & 3) {
	case 0: /* mov to cr */
		val = kvm_register_read(vcpu, reg);
		trace_kvm_cr_write(cr, val);
		switch (cr) {
		case 0:
<<<<<<< HEAD
<<<<<<< HEAD
			err = kvm_set_cr0(vcpu, val);
=======
			err = handle_set_cr0(vcpu, val);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			err = handle_set_cr0(vcpu, val);
>>>>>>> refs/remotes/origin/master
			kvm_complete_insn_gp(vcpu, err);
			return 1;
		case 3:
			err = kvm_set_cr3(vcpu, val);
			kvm_complete_insn_gp(vcpu, err);
			return 1;
		case 4:
<<<<<<< HEAD
<<<<<<< HEAD
			err = kvm_set_cr4(vcpu, val);
=======
			err = handle_set_cr4(vcpu, val);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			err = handle_set_cr4(vcpu, val);
>>>>>>> refs/remotes/origin/master
			kvm_complete_insn_gp(vcpu, err);
			return 1;
		case 8: {
				u8 cr8_prev = kvm_get_cr8(vcpu);
				u8 cr8 = kvm_register_read(vcpu, reg);
				err = kvm_set_cr8(vcpu, cr8);
				kvm_complete_insn_gp(vcpu, err);
				if (irqchip_in_kernel(vcpu->kvm))
					return 1;
				if (cr8_prev <= cr8)
					return 1;
				vcpu->run->exit_reason = KVM_EXIT_SET_TPR;
				return 0;
			}
<<<<<<< HEAD
		};
		break;
	case 2: /* clts */
<<<<<<< HEAD
		vmx_set_cr0(vcpu, kvm_read_cr0_bits(vcpu, ~X86_CR0_TS));
=======
		handle_clts(vcpu);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		}
		break;
	case 2: /* clts */
		handle_clts(vcpu);
>>>>>>> refs/remotes/origin/master
		trace_kvm_cr_write(0, kvm_read_cr0(vcpu));
		skip_emulated_instruction(vcpu);
		vmx_fpu_activate(vcpu);
		return 1;
	case 1: /*mov from cr*/
		switch (cr) {
		case 3:
			val = kvm_read_cr3(vcpu);
			kvm_register_write(vcpu, reg, val);
			trace_kvm_cr_read(cr, val);
			skip_emulated_instruction(vcpu);
			return 1;
		case 8:
			val = kvm_get_cr8(vcpu);
			kvm_register_write(vcpu, reg, val);
			trace_kvm_cr_read(cr, val);
			skip_emulated_instruction(vcpu);
			return 1;
		}
		break;
	case 3: /* lmsw */
		val = (exit_qualification >> LMSW_SOURCE_DATA_SHIFT) & 0x0f;
		trace_kvm_cr_write(0, (kvm_read_cr0(vcpu) & ~0xful) | val);
		kvm_lmsw(vcpu, val);

		skip_emulated_instruction(vcpu);
		return 1;
	default:
		break;
	}
	vcpu->run->exit_reason = 0;
<<<<<<< HEAD
	pr_unimpl(vcpu, "unhandled control register: op %d cr %d\n",
=======
	vcpu_unimpl(vcpu, "unhandled control register: op %d cr %d\n",
>>>>>>> refs/remotes/origin/master
	       (int)(exit_qualification >> 4) & 3, cr);
	return 0;
}

static int handle_dr(struct kvm_vcpu *vcpu)
{
	unsigned long exit_qualification;
	int dr, reg;

	/* Do not handle if the CPL > 0, will trigger GP on re-entry */
	if (!kvm_require_cpl(vcpu, 0))
		return 1;
	dr = vmcs_readl(GUEST_DR7);
	if (dr & DR7_GD) {
		/*
		 * As the vm-exit takes precedence over the debug trap, we
		 * need to emulate the latter, either for the host or the
		 * guest debugging itself.
		 */
		if (vcpu->guest_debug & KVM_GUESTDBG_USE_HW_BP) {
			vcpu->run->debug.arch.dr6 = vcpu->arch.dr6;
			vcpu->run->debug.arch.dr7 = dr;
			vcpu->run->debug.arch.pc =
				vmcs_readl(GUEST_CS_BASE) +
				vmcs_readl(GUEST_RIP);
			vcpu->run->debug.arch.exception = DB_VECTOR;
			vcpu->run->exit_reason = KVM_EXIT_DEBUG;
			return 0;
		} else {
			vcpu->arch.dr7 &= ~DR7_GD;
			vcpu->arch.dr6 |= DR6_BD;
			vmcs_writel(GUEST_DR7, vcpu->arch.dr7);
			kvm_queue_exception(vcpu, DB_VECTOR);
			return 1;
		}
	}

	exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	dr = exit_qualification & DEBUG_REG_ACCESS_NUM;
	reg = DEBUG_REG_ACCESS_REG(exit_qualification);
	if (exit_qualification & TYPE_MOV_FROM_DR) {
		unsigned long val;
		if (!kvm_get_dr(vcpu, dr, &val))
			kvm_register_write(vcpu, reg, val);
	} else
		kvm_set_dr(vcpu, dr, vcpu->arch.regs[reg]);
	skip_emulated_instruction(vcpu);
	return 1;
}

static void vmx_set_dr7(struct kvm_vcpu *vcpu, unsigned long val)
{
	vmcs_writel(GUEST_DR7, val);
}

static int handle_cpuid(struct kvm_vcpu *vcpu)
{
	kvm_emulate_cpuid(vcpu);
	return 1;
}

static int handle_rdmsr(struct kvm_vcpu *vcpu)
{
	u32 ecx = vcpu->arch.regs[VCPU_REGS_RCX];
	u64 data;

	if (vmx_get_msr(vcpu, ecx, &data)) {
		trace_kvm_msr_read_ex(ecx);
		kvm_inject_gp(vcpu, 0);
		return 1;
	}

	trace_kvm_msr_read(ecx, data);

	/* FIXME: handling of bits 32:63 of rax, rdx */
	vcpu->arch.regs[VCPU_REGS_RAX] = data & -1u;
	vcpu->arch.regs[VCPU_REGS_RDX] = (data >> 32) & -1u;
	skip_emulated_instruction(vcpu);
	return 1;
}

static int handle_wrmsr(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
=======
	struct msr_data msr;
>>>>>>> refs/remotes/origin/master
	u32 ecx = vcpu->arch.regs[VCPU_REGS_RCX];
	u64 data = (vcpu->arch.regs[VCPU_REGS_RAX] & -1u)
		| ((u64)(vcpu->arch.regs[VCPU_REGS_RDX] & -1u) << 32);

<<<<<<< HEAD
	if (vmx_set_msr(vcpu, ecx, data) != 0) {
=======
	msr.data = data;
	msr.index = ecx;
	msr.host_initiated = false;
	if (vmx_set_msr(vcpu, &msr) != 0) {
>>>>>>> refs/remotes/origin/master
		trace_kvm_msr_write_ex(ecx, data);
		kvm_inject_gp(vcpu, 0);
		return 1;
	}

	trace_kvm_msr_write(ecx, data);
	skip_emulated_instruction(vcpu);
	return 1;
}

static int handle_tpr_below_threshold(struct kvm_vcpu *vcpu)
{
	kvm_make_request(KVM_REQ_EVENT, vcpu);
	return 1;
}

static int handle_interrupt_window(struct kvm_vcpu *vcpu)
{
	u32 cpu_based_vm_exec_control;

	/* clear pending irq */
	cpu_based_vm_exec_control = vmcs_read32(CPU_BASED_VM_EXEC_CONTROL);
	cpu_based_vm_exec_control &= ~CPU_BASED_VIRTUAL_INTR_PENDING;
	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, cpu_based_vm_exec_control);

	kvm_make_request(KVM_REQ_EVENT, vcpu);

	++vcpu->stat.irq_window_exits;

	/*
	 * If the user space waits to inject interrupts, exit as soon as
	 * possible
	 */
	if (!irqchip_in_kernel(vcpu->kvm) &&
	    vcpu->run->request_interrupt_window &&
	    !kvm_cpu_has_interrupt(vcpu)) {
		vcpu->run->exit_reason = KVM_EXIT_IRQ_WINDOW_OPEN;
		return 0;
	}
	return 1;
}

static int handle_halt(struct kvm_vcpu *vcpu)
{
	skip_emulated_instruction(vcpu);
	return kvm_emulate_halt(vcpu);
}

static int handle_vmcall(struct kvm_vcpu *vcpu)
{
	skip_emulated_instruction(vcpu);
	kvm_emulate_hypercall(vcpu);
	return 1;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int handle_vmx_insn(struct kvm_vcpu *vcpu)
{
	kvm_queue_exception(vcpu, UD_VECTOR);
	return 1;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int handle_invd(struct kvm_vcpu *vcpu)
{
	return emulate_instruction(vcpu, 0) == EMULATE_DONE;
}

static int handle_invlpg(struct kvm_vcpu *vcpu)
{
	unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);

	kvm_mmu_invlpg(vcpu, exit_qualification);
	skip_emulated_instruction(vcpu);
	return 1;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int handle_rdpmc(struct kvm_vcpu *vcpu)
{
	int err;

	err = kvm_rdpmc(vcpu);
	kvm_complete_insn_gp(vcpu, err);

	return 1;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int handle_wbinvd(struct kvm_vcpu *vcpu)
{
	skip_emulated_instruction(vcpu);
	kvm_emulate_wbinvd(vcpu);
	return 1;
}

static int handle_xsetbv(struct kvm_vcpu *vcpu)
{
	u64 new_bv = kvm_read_edx_eax(vcpu);
	u32 index = kvm_register_read(vcpu, VCPU_REGS_RCX);

	if (kvm_set_xcr(vcpu, index, new_bv) == 0)
		skip_emulated_instruction(vcpu);
	return 1;
}

static int handle_apic_access(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (likely(fasteoi)) {
		unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
		int access_type, offset;

		access_type = exit_qualification & APIC_ACCESS_TYPE;
		offset = exit_qualification & APIC_ACCESS_OFFSET;
		/*
		 * Sane guest uses MOV to write EOI, with written value
		 * not cared. So make a short-circuit here by avoiding
		 * heavy instruction emulation.
		 */
		if ((access_type == TYPE_LINEAR_APIC_INST_WRITE) &&
		    (offset == APIC_EOI)) {
			kvm_lapic_set_eoi(vcpu);
			skip_emulated_instruction(vcpu);
			return 1;
		}
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return emulate_instruction(vcpu, 0) == EMULATE_DONE;
}

=======
	return emulate_instruction(vcpu, 0) == EMULATE_DONE;
}

static int handle_apic_eoi_induced(struct kvm_vcpu *vcpu)
{
	unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	int vector = exit_qualification & 0xff;

	/* EOI-induced VM exit is trap-like and thus no need to adjust IP */
	kvm_apic_set_eoi_accelerated(vcpu, vector);
	return 1;
}

static int handle_apic_write(struct kvm_vcpu *vcpu)
{
	unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	u32 offset = exit_qualification & 0xfff;

	/* APIC-write VM exit is trap-like and thus no need to adjust IP */
	kvm_apic_write_nodecode(vcpu, offset);
	return 1;
}

>>>>>>> refs/remotes/origin/master
static int handle_task_switch(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	unsigned long exit_qualification;
	bool has_error_code = false;
	u32 error_code = 0;
	u16 tss_selector;
<<<<<<< HEAD
<<<<<<< HEAD
	int reason, type, idt_v;

	idt_v = (vmx->idt_vectoring_info & VECTORING_INFO_VALID_MASK);
=======
=======
>>>>>>> refs/remotes/origin/master
	int reason, type, idt_v, idt_index;

	idt_v = (vmx->idt_vectoring_info & VECTORING_INFO_VALID_MASK);
	idt_index = (vmx->idt_vectoring_info & VECTORING_INFO_VECTOR_MASK);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	type = (vmx->idt_vectoring_info & VECTORING_INFO_TYPE_MASK);

	exit_qualification = vmcs_readl(EXIT_QUALIFICATION);

	reason = (u32)exit_qualification >> 30;
	if (reason == TASK_SWITCH_GATE && idt_v) {
		switch (type) {
		case INTR_TYPE_NMI_INTR:
			vcpu->arch.nmi_injected = false;
			vmx_set_nmi_mask(vcpu, true);
			break;
		case INTR_TYPE_EXT_INTR:
		case INTR_TYPE_SOFT_INTR:
			kvm_clear_interrupt_queue(vcpu);
			break;
		case INTR_TYPE_HARD_EXCEPTION:
			if (vmx->idt_vectoring_info &
			    VECTORING_INFO_DELIVER_CODE_MASK) {
				has_error_code = true;
				error_code =
					vmcs_read32(IDT_VECTORING_ERROR_CODE);
			}
			/* fall through */
		case INTR_TYPE_SOFT_EXCEPTION:
			kvm_clear_exception_queue(vcpu);
			break;
		default:
			break;
		}
	}
	tss_selector = exit_qualification;

	if (!idt_v || (type != INTR_TYPE_HARD_EXCEPTION &&
		       type != INTR_TYPE_EXT_INTR &&
		       type != INTR_TYPE_NMI_INTR))
		skip_emulated_instruction(vcpu);

<<<<<<< HEAD
<<<<<<< HEAD
	if (kvm_task_switch(vcpu, tss_selector, reason,
				has_error_code, error_code) == EMULATE_FAIL) {
=======
	if (kvm_task_switch(vcpu, tss_selector,
			    type == INTR_TYPE_SOFT_INTR ? idt_index : -1, reason,
			    has_error_code, error_code) == EMULATE_FAIL) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (kvm_task_switch(vcpu, tss_selector,
			    type == INTR_TYPE_SOFT_INTR ? idt_index : -1, reason,
			    has_error_code, error_code) == EMULATE_FAIL) {
>>>>>>> refs/remotes/origin/master
		vcpu->run->exit_reason = KVM_EXIT_INTERNAL_ERROR;
		vcpu->run->internal.suberror = KVM_INTERNAL_ERROR_EMULATION;
		vcpu->run->internal.ndata = 0;
		return 0;
	}

	/* clear all local breakpoint enable flags */
	vmcs_writel(GUEST_DR7, vmcs_readl(GUEST_DR7) & ~55);

	/*
	 * TODO: What about debug traps on tss switch?
	 *       Are we supposed to inject them and update dr6?
	 */

	return 1;
}

static int handle_ept_violation(struct kvm_vcpu *vcpu)
{
	unsigned long exit_qualification;
	gpa_t gpa;
<<<<<<< HEAD
=======
	u32 error_code;
>>>>>>> refs/remotes/origin/master
	int gla_validity;

	exit_qualification = vmcs_readl(EXIT_QUALIFICATION);

<<<<<<< HEAD
	if (exit_qualification & (1 << 6)) {
		printk(KERN_ERR "EPT: GPA exceeds GAW!\n");
		return -EINVAL;
	}

=======
>>>>>>> refs/remotes/origin/master
	gla_validity = (exit_qualification >> 7) & 0x3;
	if (gla_validity != 0x3 && gla_validity != 0x1 && gla_validity != 0) {
		printk(KERN_ERR "EPT: Handling EPT violation failed!\n");
		printk(KERN_ERR "EPT: GPA: 0x%lx, GVA: 0x%lx\n",
			(long unsigned int)vmcs_read64(GUEST_PHYSICAL_ADDRESS),
			vmcs_readl(GUEST_LINEAR_ADDRESS));
		printk(KERN_ERR "EPT: Exit qualification is 0x%lx\n",
			(long unsigned int)exit_qualification);
		vcpu->run->exit_reason = KVM_EXIT_UNKNOWN;
		vcpu->run->hw.hardware_exit_reason = EXIT_REASON_EPT_VIOLATION;
		return 0;
	}

<<<<<<< HEAD
	gpa = vmcs_read64(GUEST_PHYSICAL_ADDRESS);
	trace_kvm_page_fault(gpa, exit_qualification);
	return kvm_mmu_page_fault(vcpu, gpa, exit_qualification & 0x3, NULL, 0);
=======
	/*
	 * EPT violation happened while executing iret from NMI,
	 * "blocked by NMI" bit has to be set before next VM entry.
	 * There are errata that may cause this bit to not be set:
	 * AAK134, BY25.
	 */
	if (!(to_vmx(vcpu)->idt_vectoring_info & VECTORING_INFO_VALID_MASK) &&
			cpu_has_virtual_nmis() &&
			(exit_qualification & INTR_INFO_UNBLOCK_NMI))
		vmcs_set_bits(GUEST_INTERRUPTIBILITY_INFO, GUEST_INTR_STATE_NMI);

	gpa = vmcs_read64(GUEST_PHYSICAL_ADDRESS);
	trace_kvm_page_fault(gpa, exit_qualification);

	/* It is a write fault? */
	error_code = exit_qualification & (1U << 1);
	/* It is a fetch fault? */
	error_code |= (exit_qualification & (1U << 2)) << 2;
	/* ept page table is present? */
	error_code |= (exit_qualification >> 3) & 0x1;

	vcpu->arch.exit_qualification = exit_qualification;

	return kvm_mmu_page_fault(vcpu, gpa, error_code, NULL, 0);
>>>>>>> refs/remotes/origin/master
}

static u64 ept_rsvd_mask(u64 spte, int level)
{
	int i;
	u64 mask = 0;

	for (i = 51; i > boot_cpu_data.x86_phys_bits; i--)
		mask |= (1ULL << i);

	if (level > 2)
		/* bits 7:3 reserved */
		mask |= 0xf8;
	else if (level == 2) {
		if (spte & (1ULL << 7))
			/* 2MB ref, bits 20:12 reserved */
			mask |= 0x1ff000;
		else
			/* bits 6:3 reserved */
			mask |= 0x78;
	}

	return mask;
}

static void ept_misconfig_inspect_spte(struct kvm_vcpu *vcpu, u64 spte,
				       int level)
{
	printk(KERN_ERR "%s: spte 0x%llx level %d\n", __func__, spte, level);

	/* 010b (write-only) */
	WARN_ON((spte & 0x7) == 0x2);

	/* 110b (write/execute) */
	WARN_ON((spte & 0x7) == 0x6);

	/* 100b (execute-only) and value not supported by logical processor */
	if (!cpu_has_vmx_ept_execute_only())
		WARN_ON((spte & 0x7) == 0x4);

	/* not 000b */
	if ((spte & 0x7)) {
		u64 rsvd_bits = spte & ept_rsvd_mask(spte, level);

		if (rsvd_bits != 0) {
			printk(KERN_ERR "%s: rsvd_bits = 0x%llx\n",
					 __func__, rsvd_bits);
			WARN_ON(1);
		}

		if (level == 1 || (level == 2 && (spte & (1ULL << 7)))) {
			u64 ept_mem_type = (spte & 0x38) >> 3;

			if (ept_mem_type == 2 || ept_mem_type == 3 ||
			    ept_mem_type == 7) {
				printk(KERN_ERR "%s: ept_mem_type=0x%llx\n",
						__func__, ept_mem_type);
				WARN_ON(1);
			}
		}
	}
}

static int handle_ept_misconfig(struct kvm_vcpu *vcpu)
{
	u64 sptes[4];
<<<<<<< HEAD
<<<<<<< HEAD
	int nr_sptes, i;
=======
	int nr_sptes, i, ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int nr_sptes, i, ret;
>>>>>>> refs/remotes/origin/master
	gpa_t gpa;

	gpa = vmcs_read64(GUEST_PHYSICAL_ADDRESS);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	ret = handle_mmio_page_fault_common(vcpu, gpa, true);
	if (likely(ret == 1))
		return x86_emulate_instruction(vcpu, gpa, 0, NULL, 0) ==
					      EMULATE_DONE;
	if (unlikely(!ret))
		return 1;

	/* It is the real ept misconfig */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = handle_mmio_page_fault_common(vcpu, gpa, true);
	if (likely(ret == RET_MMIO_PF_EMULATE))
		return x86_emulate_instruction(vcpu, gpa, 0, NULL, 0) ==
					      EMULATE_DONE;

	if (unlikely(ret == RET_MMIO_PF_INVALID))
		return kvm_mmu_page_fault(vcpu, gpa, 0, NULL, 0);

	if (unlikely(ret == RET_MMIO_PF_RETRY))
		return 1;

	/* It is the real ept misconfig */
>>>>>>> refs/remotes/origin/master
	printk(KERN_ERR "EPT: Misconfiguration.\n");
	printk(KERN_ERR "EPT: GPA: 0x%llx\n", gpa);

	nr_sptes = kvm_mmu_get_spte_hierarchy(vcpu, gpa, sptes);

	for (i = PT64_ROOT_LEVEL; i > PT64_ROOT_LEVEL - nr_sptes; --i)
		ept_misconfig_inspect_spte(vcpu, sptes[i-1], i);

	vcpu->run->exit_reason = KVM_EXIT_UNKNOWN;
	vcpu->run->hw.hardware_exit_reason = EXIT_REASON_EPT_MISCONFIG;

	return 0;
}

static int handle_nmi_window(struct kvm_vcpu *vcpu)
{
	u32 cpu_based_vm_exec_control;

	/* clear pending NMI */
	cpu_based_vm_exec_control = vmcs_read32(CPU_BASED_VM_EXEC_CONTROL);
	cpu_based_vm_exec_control &= ~CPU_BASED_VIRTUAL_NMI_PENDING;
	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, cpu_based_vm_exec_control);
	++vcpu->stat.nmi_window_exits;
	kvm_make_request(KVM_REQ_EVENT, vcpu);

	return 1;
}

static int handle_invalid_guest_state(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	enum emulation_result err = EMULATE_DONE;
	int ret = 1;
	u32 cpu_exec_ctrl;
	bool intr_window_requested;
<<<<<<< HEAD
=======
	unsigned count = 130;
>>>>>>> refs/remotes/origin/master

	cpu_exec_ctrl = vmcs_read32(CPU_BASED_VM_EXEC_CONTROL);
	intr_window_requested = cpu_exec_ctrl & CPU_BASED_VIRTUAL_INTR_PENDING;

<<<<<<< HEAD
	while (!guest_state_valid(vcpu)) {
		if (intr_window_requested
		    && (kvm_get_rflags(&vmx->vcpu) & X86_EFLAGS_IF))
			return handle_interrupt_window(&vmx->vcpu);

		err = emulate_instruction(vcpu, 0);

		if (err == EMULATE_DO_MMIO) {
=======
	while (!guest_state_valid(vcpu) && count-- != 0) {
		if (intr_window_requested && vmx_interrupt_allowed(vcpu))
			return handle_interrupt_window(&vmx->vcpu);

		if (test_bit(KVM_REQ_EVENT, &vcpu->requests))
			return 1;

		err = emulate_instruction(vcpu, EMULTYPE_NO_REEXECUTE);

		if (err == EMULATE_USER_EXIT) {
			++vcpu->stat.mmio_exits;
>>>>>>> refs/remotes/origin/master
			ret = 0;
			goto out;
		}

<<<<<<< HEAD
		if (err != EMULATE_DONE)
			return 0;
=======
		if (err != EMULATE_DONE) {
			vcpu->run->exit_reason = KVM_EXIT_INTERNAL_ERROR;
			vcpu->run->internal.suberror = KVM_INTERNAL_ERROR_EMULATION;
			vcpu->run->internal.ndata = 0;
			return 0;
		}
>>>>>>> refs/remotes/origin/master

		if (vcpu->arch.halt_request) {
			vcpu->arch.halt_request = 0;
			ret = kvm_emulate_halt(vcpu);
			goto out;
		}

		if (vcpu->arch.halt_request) {
			vcpu->arch.halt_request = 0;
			ret = kvm_emulate_halt(vcpu);
			goto out;
		}

		if (signal_pending(current))
			goto out;
		if (need_resched())
			schedule();
	}

<<<<<<< HEAD
	vmx->emulation_required = 0;
=======
	vmx->emulation_required = emulation_required(vcpu);
>>>>>>> refs/remotes/origin/master
out:
	return ret;
}

/*
 * Indicate a busy-waiting vcpu in spinlock. We do not enable the PAUSE
 * exiting, so only get here on cpu with PAUSE-Loop-Exiting.
 */
static int handle_pause(struct kvm_vcpu *vcpu)
{
	skip_emulated_instruction(vcpu);
	kvm_vcpu_on_spin(vcpu);

	return 1;
}

static int handle_invalid_op(struct kvm_vcpu *vcpu)
{
	kvm_queue_exception(vcpu, UD_VECTOR);
	return 1;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * The exit handlers return 1 if the exit was handled fully and guest execution
 * may resume.  Otherwise they set the kvm_run parameter to indicate what needs
 * to be done to userspace and return 0.
 */
static int (*kvm_vmx_exit_handlers[])(struct kvm_vcpu *vcpu) = {
	[EXIT_REASON_EXCEPTION_NMI]           = handle_exception,
	[EXIT_REASON_EXTERNAL_INTERRUPT]      = handle_external_interrupt,
	[EXIT_REASON_TRIPLE_FAULT]            = handle_triple_fault,
	[EXIT_REASON_NMI_WINDOW]	      = handle_nmi_window,
	[EXIT_REASON_IO_INSTRUCTION]          = handle_io,
	[EXIT_REASON_CR_ACCESS]               = handle_cr,
	[EXIT_REASON_DR_ACCESS]               = handle_dr,
	[EXIT_REASON_CPUID]                   = handle_cpuid,
	[EXIT_REASON_MSR_READ]                = handle_rdmsr,
	[EXIT_REASON_MSR_WRITE]               = handle_wrmsr,
	[EXIT_REASON_PENDING_INTERRUPT]       = handle_interrupt_window,
	[EXIT_REASON_HLT]                     = handle_halt,
	[EXIT_REASON_INVD]		      = handle_invd,
	[EXIT_REASON_INVLPG]		      = handle_invlpg,
	[EXIT_REASON_VMCALL]                  = handle_vmcall,
	[EXIT_REASON_VMCLEAR]	              = handle_vmx_insn,
	[EXIT_REASON_VMLAUNCH]                = handle_vmx_insn,
	[EXIT_REASON_VMPTRLD]                 = handle_vmx_insn,
	[EXIT_REASON_VMPTRST]                 = handle_vmx_insn,
	[EXIT_REASON_VMREAD]                  = handle_vmx_insn,
	[EXIT_REASON_VMRESUME]                = handle_vmx_insn,
	[EXIT_REASON_VMWRITE]                 = handle_vmx_insn,
	[EXIT_REASON_VMOFF]                   = handle_vmx_insn,
	[EXIT_REASON_VMON]                    = handle_vmx_insn,
	[EXIT_REASON_TPR_BELOW_THRESHOLD]     = handle_tpr_below_threshold,
	[EXIT_REASON_APIC_ACCESS]             = handle_apic_access,
	[EXIT_REASON_WBINVD]                  = handle_wbinvd,
	[EXIT_REASON_XSETBV]                  = handle_xsetbv,
	[EXIT_REASON_TASK_SWITCH]             = handle_task_switch,
	[EXIT_REASON_MCE_DURING_VMENTRY]      = handle_machine_check,
	[EXIT_REASON_EPT_VIOLATION]	      = handle_ept_violation,
	[EXIT_REASON_EPT_MISCONFIG]           = handle_ept_misconfig,
	[EXIT_REASON_PAUSE_INSTRUCTION]       = handle_pause,
	[EXIT_REASON_MWAIT_INSTRUCTION]	      = handle_invalid_op,
	[EXIT_REASON_MONITOR_INSTRUCTION]     = handle_invalid_op,
};

static const int kvm_vmx_max_exit_handlers =
	ARRAY_SIZE(kvm_vmx_exit_handlers);

static void vmx_get_exit_info(struct kvm_vcpu *vcpu, u64 *info1, u64 *info2)
{
	*info1 = vmcs_readl(EXIT_QUALIFICATION);
	*info2 = vmcs_read32(VM_EXIT_INTR_INFO);
}

/*
 * The guest has exited.  See if we can fix it or if we need userspace
 * assistance.
 */
static int vmx_handle_exit(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u32 exit_reason = vmx->exit_reason;
	u32 vectoring_info = vmx->idt_vectoring_info;

	trace_kvm_exit(exit_reason, vcpu, KVM_ISA_VMX);

	/* If guest state is invalid, start emulating */
	if (vmx->emulation_required && emulate_invalid_guest_state)
		return handle_invalid_guest_state(vcpu);

	if (exit_reason & VMX_EXIT_REASONS_FAILED_VMENTRY) {
		vcpu->run->exit_reason = KVM_EXIT_FAIL_ENTRY;
		vcpu->run->fail_entry.hardware_entry_failure_reason
			= exit_reason;
		return 0;
	}

	if (unlikely(vmx->fail)) {
		vcpu->run->exit_reason = KVM_EXIT_FAIL_ENTRY;
		vcpu->run->fail_entry.hardware_entry_failure_reason
			= vmcs_read32(VM_INSTRUCTION_ERROR);
		return 0;
	}

	if ((vectoring_info & VECTORING_INFO_VALID_MASK) &&
=======
=======
>>>>>>> refs/remotes/origin/master
 * To run an L2 guest, we need a vmcs02 based on the L1-specified vmcs12.
 * We could reuse a single VMCS for all the L2 guests, but we also want the
 * option to allocate a separate vmcs02 for each separate loaded vmcs12 - this
 * allows keeping them loaded on the processor, and in the future will allow
 * optimizations where prepare_vmcs02 doesn't need to set all the fields on
 * every entry if they never change.
 * So we keep, in vmx->nested.vmcs02_pool, a cache of size VMCS02_POOL_SIZE
 * (>=0) with a vmcs02 for each recently loaded vmcs12s, most recent first.
 *
 * The following functions allocate and free a vmcs02 in this pool.
 */

/* Get a VMCS from the pool to use as vmcs02 for the current vmcs12. */
static struct loaded_vmcs *nested_get_current_vmcs02(struct vcpu_vmx *vmx)
{
	struct vmcs02_list *item;
	list_for_each_entry(item, &vmx->nested.vmcs02_pool, list)
		if (item->vmptr == vmx->nested.current_vmptr) {
			list_move(&item->list, &vmx->nested.vmcs02_pool);
			return &item->vmcs02;
		}

	if (vmx->nested.vmcs02_num >= max(VMCS02_POOL_SIZE, 1)) {
		/* Recycle the least recently used VMCS. */
		item = list_entry(vmx->nested.vmcs02_pool.prev,
			struct vmcs02_list, list);
		item->vmptr = vmx->nested.current_vmptr;
		list_move(&item->list, &vmx->nested.vmcs02_pool);
		return &item->vmcs02;
	}

	/* Create a new VMCS */
<<<<<<< HEAD
	item = (struct vmcs02_list *)
		kmalloc(sizeof(struct vmcs02_list), GFP_KERNEL);
=======
	item = kmalloc(sizeof(struct vmcs02_list), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!item)
		return NULL;
	item->vmcs02.vmcs = alloc_vmcs();
	if (!item->vmcs02.vmcs) {
		kfree(item);
		return NULL;
	}
	loaded_vmcs_init(&item->vmcs02);
	item->vmptr = vmx->nested.current_vmptr;
	list_add(&(item->list), &(vmx->nested.vmcs02_pool));
	vmx->nested.vmcs02_num++;
	return &item->vmcs02;
}

/* Free and remove from pool a vmcs02 saved for a vmcs12 (if there is one) */
static void nested_free_vmcs02(struct vcpu_vmx *vmx, gpa_t vmptr)
{
	struct vmcs02_list *item;
	list_for_each_entry(item, &vmx->nested.vmcs02_pool, list)
		if (item->vmptr == vmptr) {
			free_loaded_vmcs(&item->vmcs02);
			list_del(&item->list);
			kfree(item);
			vmx->nested.vmcs02_num--;
			return;
		}
}

/*
 * Free all VMCSs saved for this vcpu, except the one pointed by
 * vmx->loaded_vmcs. These include the VMCSs in vmcs02_pool (except the one
 * currently used, if running L2), and vmcs01 when running L2.
 */
static void nested_free_all_saved_vmcss(struct vcpu_vmx *vmx)
{
	struct vmcs02_list *item, *n;
	list_for_each_entry_safe(item, n, &vmx->nested.vmcs02_pool, list) {
		if (vmx->loaded_vmcs != &item->vmcs02)
			free_loaded_vmcs(&item->vmcs02);
		list_del(&item->list);
		kfree(item);
	}
	vmx->nested.vmcs02_num = 0;

	if (vmx->loaded_vmcs != &vmx->vmcs01)
		free_loaded_vmcs(&vmx->vmcs01);
}

/*
<<<<<<< HEAD
=======
 * The following 3 functions, nested_vmx_succeed()/failValid()/failInvalid(),
 * set the success or error code of an emulated VMX instruction, as specified
 * by Vol 2B, VMX Instruction Reference, "Conventions".
 */
static void nested_vmx_succeed(struct kvm_vcpu *vcpu)
{
	vmx_set_rflags(vcpu, vmx_get_rflags(vcpu)
			& ~(X86_EFLAGS_CF | X86_EFLAGS_PF | X86_EFLAGS_AF |
			    X86_EFLAGS_ZF | X86_EFLAGS_SF | X86_EFLAGS_OF));
}

static void nested_vmx_failInvalid(struct kvm_vcpu *vcpu)
{
	vmx_set_rflags(vcpu, (vmx_get_rflags(vcpu)
			& ~(X86_EFLAGS_PF | X86_EFLAGS_AF | X86_EFLAGS_ZF |
			    X86_EFLAGS_SF | X86_EFLAGS_OF))
			| X86_EFLAGS_CF);
}

static void nested_vmx_failValid(struct kvm_vcpu *vcpu,
					u32 vm_instruction_error)
{
	if (to_vmx(vcpu)->nested.current_vmptr == -1ull) {
		/*
		 * failValid writes the error number to the current VMCS, which
		 * can't be done there isn't a current VMCS.
		 */
		nested_vmx_failInvalid(vcpu);
		return;
	}
	vmx_set_rflags(vcpu, (vmx_get_rflags(vcpu)
			& ~(X86_EFLAGS_CF | X86_EFLAGS_PF | X86_EFLAGS_AF |
			    X86_EFLAGS_SF | X86_EFLAGS_OF))
			| X86_EFLAGS_ZF);
	get_vmcs12(vcpu)->vm_instruction_error = vm_instruction_error;
	/*
	 * We don't need to force a shadow sync because
	 * VM_INSTRUCTION_ERROR is not shadowed
	 */
}

/*
>>>>>>> refs/remotes/origin/master
 * Emulate the VMXON instruction.
 * Currently, we just remember that VMX is active, and do not save or even
 * inspect the argument to VMXON (the so-called "VMXON pointer") because we
 * do not currently need to store anything in that guest-allocated memory
 * region. Consequently, VMCLEAR and VMPTRLD also do not verify that the their
 * argument is different from the VMXON pointer (which the spec says they do).
 */
static int handle_vmon(struct kvm_vcpu *vcpu)
{
	struct kvm_segment cs;
	struct vcpu_vmx *vmx = to_vmx(vcpu);
<<<<<<< HEAD
=======
	struct vmcs *shadow_vmcs;
	const u64 VMXON_NEEDED_FEATURES = FEATURE_CONTROL_LOCKED
		| FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX;
>>>>>>> refs/remotes/origin/master

	/* The Intel VMX Instruction Reference lists a bunch of bits that
	 * are prerequisite to running VMXON, most notably cr4.VMXE must be
	 * set to 1 (see vmx_set_cr4() for when we allow the guest to set this).
	 * Otherwise, we should fail with #UD. We test these now:
	 */
	if (!kvm_read_cr4_bits(vcpu, X86_CR4_VMXE) ||
	    !kvm_read_cr0_bits(vcpu, X86_CR0_PE) ||
	    (vmx_get_rflags(vcpu) & X86_EFLAGS_VM)) {
		kvm_queue_exception(vcpu, UD_VECTOR);
		return 1;
	}

	vmx_get_segment(vcpu, &cs, VCPU_SREG_CS);
	if (is_long_mode(vcpu) && !cs.l) {
		kvm_queue_exception(vcpu, UD_VECTOR);
		return 1;
	}

	if (vmx_get_cpl(vcpu)) {
		kvm_inject_gp(vcpu, 0);
		return 1;
	}
<<<<<<< HEAD
=======
	if (vmx->nested.vmxon) {
		nested_vmx_failValid(vcpu, VMXERR_VMXON_IN_VMX_ROOT_OPERATION);
		skip_emulated_instruction(vcpu);
		return 1;
	}

	if ((vmx->nested.msr_ia32_feature_control & VMXON_NEEDED_FEATURES)
			!= VMXON_NEEDED_FEATURES) {
		kvm_inject_gp(vcpu, 0);
		return 1;
	}

	if (enable_shadow_vmcs) {
		shadow_vmcs = alloc_vmcs();
		if (!shadow_vmcs)
			return -ENOMEM;
		/* mark vmcs as shadow */
		shadow_vmcs->revision_id |= (1u << 31);
		/* init shadow vmcs */
		vmcs_clear(shadow_vmcs);
		vmx->nested.current_shadow_vmcs = shadow_vmcs;
	}
>>>>>>> refs/remotes/origin/master

	INIT_LIST_HEAD(&(vmx->nested.vmcs02_pool));
	vmx->nested.vmcs02_num = 0;

	vmx->nested.vmxon = true;

	skip_emulated_instruction(vcpu);
<<<<<<< HEAD
=======
	nested_vmx_succeed(vcpu);
>>>>>>> refs/remotes/origin/master
	return 1;
}

/*
 * Intel's VMX Instruction Reference specifies a common set of prerequisites
 * for running VMX instructions (except VMXON, whose prerequisites are
 * slightly different). It also specifies what exception to inject otherwise.
 */
static int nested_vmx_check_permission(struct kvm_vcpu *vcpu)
{
	struct kvm_segment cs;
	struct vcpu_vmx *vmx = to_vmx(vcpu);

	if (!vmx->nested.vmxon) {
		kvm_queue_exception(vcpu, UD_VECTOR);
		return 0;
	}

	vmx_get_segment(vcpu, &cs, VCPU_SREG_CS);
	if ((vmx_get_rflags(vcpu) & X86_EFLAGS_VM) ||
	    (is_long_mode(vcpu) && !cs.l)) {
		kvm_queue_exception(vcpu, UD_VECTOR);
		return 0;
	}

	if (vmx_get_cpl(vcpu)) {
		kvm_inject_gp(vcpu, 0);
		return 0;
	}

	return 1;
}

<<<<<<< HEAD
=======
static inline void nested_release_vmcs12(struct vcpu_vmx *vmx)
{
	u32 exec_control;
	if (enable_shadow_vmcs) {
		if (vmx->nested.current_vmcs12 != NULL) {
			/* copy to memory all shadowed fields in case
			   they were modified */
			copy_shadow_to_vmcs12(vmx);
			vmx->nested.sync_shadow_vmcs = false;
			exec_control = vmcs_read32(SECONDARY_VM_EXEC_CONTROL);
			exec_control &= ~SECONDARY_EXEC_SHADOW_VMCS;
			vmcs_write32(SECONDARY_VM_EXEC_CONTROL, exec_control);
			vmcs_write64(VMCS_LINK_POINTER, -1ull);
		}
	}
	kunmap(vmx->nested.current_vmcs12_page);
	nested_release_page(vmx->nested.current_vmcs12_page);
}

>>>>>>> refs/remotes/origin/master
/*
 * Free whatever needs to be freed from vmx->nested when L1 goes down, or
 * just stops using VMX.
 */
static void free_nested(struct vcpu_vmx *vmx)
{
	if (!vmx->nested.vmxon)
		return;
	vmx->nested.vmxon = false;
	if (vmx->nested.current_vmptr != -1ull) {
<<<<<<< HEAD
		kunmap(vmx->nested.current_vmcs12_page);
		nested_release_page(vmx->nested.current_vmcs12_page);
		vmx->nested.current_vmptr = -1ull;
		vmx->nested.current_vmcs12 = NULL;
	}
=======
		nested_release_vmcs12(vmx);
		vmx->nested.current_vmptr = -1ull;
		vmx->nested.current_vmcs12 = NULL;
	}
	if (enable_shadow_vmcs)
		free_vmcs(vmx->nested.current_shadow_vmcs);
>>>>>>> refs/remotes/origin/master
	/* Unpin physical memory we referred to in current vmcs02 */
	if (vmx->nested.apic_access_page) {
		nested_release_page(vmx->nested.apic_access_page);
		vmx->nested.apic_access_page = 0;
	}

	nested_free_all_saved_vmcss(vmx);
}

/* Emulate the VMXOFF instruction */
static int handle_vmoff(struct kvm_vcpu *vcpu)
{
	if (!nested_vmx_check_permission(vcpu))
		return 1;
	free_nested(to_vmx(vcpu));
	skip_emulated_instruction(vcpu);
<<<<<<< HEAD
=======
	nested_vmx_succeed(vcpu);
>>>>>>> refs/remotes/origin/master
	return 1;
}

/*
 * Decode the memory-address operand of a vmx instruction, as recorded on an
 * exit caused by such an instruction (run by a guest hypervisor).
 * On success, returns 0. When the operand is invalid, returns 1 and throws
 * #UD or #GP.
 */
static int get_vmx_mem_address(struct kvm_vcpu *vcpu,
				 unsigned long exit_qualification,
				 u32 vmx_instruction_info, gva_t *ret)
{
	/*
	 * According to Vol. 3B, "Information for VM Exits Due to Instruction
	 * Execution", on an exit, vmx_instruction_info holds most of the
	 * addressing components of the operand. Only the displacement part
	 * is put in exit_qualification (see 3B, "Basic VM-Exit Information").
	 * For how an actual address is calculated from all these components,
	 * refer to Vol. 1, "Operand Addressing".
	 */
	int  scaling = vmx_instruction_info & 3;
	int  addr_size = (vmx_instruction_info >> 7) & 7;
	bool is_reg = vmx_instruction_info & (1u << 10);
	int  seg_reg = (vmx_instruction_info >> 15) & 7;
	int  index_reg = (vmx_instruction_info >> 18) & 0xf;
	bool index_is_valid = !(vmx_instruction_info & (1u << 22));
	int  base_reg       = (vmx_instruction_info >> 23) & 0xf;
	bool base_is_valid  = !(vmx_instruction_info & (1u << 27));

	if (is_reg) {
		kvm_queue_exception(vcpu, UD_VECTOR);
		return 1;
	}

	/* Addr = segment_base + offset */
	/* offset = base + [index * scale] + displacement */
	*ret = vmx_get_segment_base(vcpu, seg_reg);
	if (base_is_valid)
		*ret += kvm_register_read(vcpu, base_reg);
	if (index_is_valid)
		*ret += kvm_register_read(vcpu, index_reg)<<scaling;
	*ret += exit_qualification; /* holds the displacement */

	if (addr_size == 1) /* 32 bit */
		*ret &= 0xffffffff;

	/*
	 * TODO: throw #GP (and return 1) in various cases that the VM*
	 * instructions require it - e.g., offset beyond segment limit,
	 * unusable or unreadable/unwritable segment, non-canonical 64-bit
	 * address, and so on. Currently these are not checked.
	 */
	return 0;
}

<<<<<<< HEAD
/*
 * The following 3 functions, nested_vmx_succeed()/failValid()/failInvalid(),
 * set the success or error code of an emulated VMX instruction, as specified
 * by Vol 2B, VMX Instruction Reference, "Conventions".
 */
static void nested_vmx_succeed(struct kvm_vcpu *vcpu)
{
	vmx_set_rflags(vcpu, vmx_get_rflags(vcpu)
			& ~(X86_EFLAGS_CF | X86_EFLAGS_PF | X86_EFLAGS_AF |
			    X86_EFLAGS_ZF | X86_EFLAGS_SF | X86_EFLAGS_OF));
}

static void nested_vmx_failInvalid(struct kvm_vcpu *vcpu)
{
	vmx_set_rflags(vcpu, (vmx_get_rflags(vcpu)
			& ~(X86_EFLAGS_PF | X86_EFLAGS_AF | X86_EFLAGS_ZF |
			    X86_EFLAGS_SF | X86_EFLAGS_OF))
			| X86_EFLAGS_CF);
}

static void nested_vmx_failValid(struct kvm_vcpu *vcpu,
					u32 vm_instruction_error)
{
	if (to_vmx(vcpu)->nested.current_vmptr == -1ull) {
		/*
		 * failValid writes the error number to the current VMCS, which
		 * can't be done there isn't a current VMCS.
		 */
		nested_vmx_failInvalid(vcpu);
		return;
	}
	vmx_set_rflags(vcpu, (vmx_get_rflags(vcpu)
			& ~(X86_EFLAGS_CF | X86_EFLAGS_PF | X86_EFLAGS_AF |
			    X86_EFLAGS_SF | X86_EFLAGS_OF))
			| X86_EFLAGS_ZF);
	get_vmcs12(vcpu)->vm_instruction_error = vm_instruction_error;
}

=======
>>>>>>> refs/remotes/origin/master
/* Emulate the VMCLEAR instruction */
static int handle_vmclear(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	gva_t gva;
	gpa_t vmptr;
	struct vmcs12 *vmcs12;
	struct page *page;
	struct x86_exception e;

	if (!nested_vmx_check_permission(vcpu))
		return 1;

	if (get_vmx_mem_address(vcpu, vmcs_readl(EXIT_QUALIFICATION),
			vmcs_read32(VMX_INSTRUCTION_INFO), &gva))
		return 1;

	if (kvm_read_guest_virt(&vcpu->arch.emulate_ctxt, gva, &vmptr,
				sizeof(vmptr), &e)) {
		kvm_inject_page_fault(vcpu, &e);
		return 1;
	}

	if (!IS_ALIGNED(vmptr, PAGE_SIZE)) {
		nested_vmx_failValid(vcpu, VMXERR_VMCLEAR_INVALID_ADDRESS);
		skip_emulated_instruction(vcpu);
		return 1;
	}

	if (vmptr == vmx->nested.current_vmptr) {
<<<<<<< HEAD
		kunmap(vmx->nested.current_vmcs12_page);
		nested_release_page(vmx->nested.current_vmcs12_page);
=======
		nested_release_vmcs12(vmx);
>>>>>>> refs/remotes/origin/master
		vmx->nested.current_vmptr = -1ull;
		vmx->nested.current_vmcs12 = NULL;
	}

	page = nested_get_page(vcpu, vmptr);
	if (page == NULL) {
		/*
		 * For accurate processor emulation, VMCLEAR beyond available
		 * physical memory should do nothing at all. However, it is
		 * possible that a nested vmx bug, not a guest hypervisor bug,
		 * resulted in this case, so let's shut down before doing any
		 * more damage:
		 */
		kvm_make_request(KVM_REQ_TRIPLE_FAULT, vcpu);
		return 1;
	}
	vmcs12 = kmap(page);
	vmcs12->launch_state = 0;
	kunmap(page);
	nested_release_page(page);

	nested_free_vmcs02(vmx, vmptr);

	skip_emulated_instruction(vcpu);
	nested_vmx_succeed(vcpu);
	return 1;
}

static int nested_vmx_run(struct kvm_vcpu *vcpu, bool launch);

/* Emulate the VMLAUNCH instruction */
static int handle_vmlaunch(struct kvm_vcpu *vcpu)
{
	return nested_vmx_run(vcpu, true);
}

/* Emulate the VMRESUME instruction */
static int handle_vmresume(struct kvm_vcpu *vcpu)
{

	return nested_vmx_run(vcpu, false);
}

enum vmcs_field_type {
	VMCS_FIELD_TYPE_U16 = 0,
	VMCS_FIELD_TYPE_U64 = 1,
	VMCS_FIELD_TYPE_U32 = 2,
	VMCS_FIELD_TYPE_NATURAL_WIDTH = 3
};

static inline int vmcs_field_type(unsigned long field)
{
	if (0x1 & field)	/* the *_HIGH fields are all 32 bit */
		return VMCS_FIELD_TYPE_U32;
	return (field >> 13) & 0x3 ;
}

static inline int vmcs_field_readonly(unsigned long field)
{
	return (((field >> 10) & 0x3) == 1);
}

/*
 * Read a vmcs12 field. Since these can have varying lengths and we return
 * one type, we chose the biggest type (u64) and zero-extend the return value
 * to that size. Note that the caller, handle_vmread, might need to use only
 * some of the bits we return here (e.g., on 32-bit guests, only 32 bits of
 * 64-bit fields are to be returned).
 */
static inline bool vmcs12_read_any(struct kvm_vcpu *vcpu,
					unsigned long field, u64 *ret)
{
	short offset = vmcs_field_to_offset(field);
	char *p;

	if (offset < 0)
		return 0;

	p = ((char *)(get_vmcs12(vcpu))) + offset;

	switch (vmcs_field_type(field)) {
	case VMCS_FIELD_TYPE_NATURAL_WIDTH:
		*ret = *((natural_width *)p);
		return 1;
	case VMCS_FIELD_TYPE_U16:
		*ret = *((u16 *)p);
		return 1;
	case VMCS_FIELD_TYPE_U32:
		*ret = *((u32 *)p);
		return 1;
	case VMCS_FIELD_TYPE_U64:
		*ret = *((u64 *)p);
		return 1;
	default:
		return 0; /* can never happen. */
	}
}

<<<<<<< HEAD
=======

static inline bool vmcs12_write_any(struct kvm_vcpu *vcpu,
				    unsigned long field, u64 field_value){
	short offset = vmcs_field_to_offset(field);
	char *p = ((char *) get_vmcs12(vcpu)) + offset;
	if (offset < 0)
		return false;

	switch (vmcs_field_type(field)) {
	case VMCS_FIELD_TYPE_U16:
		*(u16 *)p = field_value;
		return true;
	case VMCS_FIELD_TYPE_U32:
		*(u32 *)p = field_value;
		return true;
	case VMCS_FIELD_TYPE_U64:
		*(u64 *)p = field_value;
		return true;
	case VMCS_FIELD_TYPE_NATURAL_WIDTH:
		*(natural_width *)p = field_value;
		return true;
	default:
		return false; /* can never happen. */
	}

}

static void copy_shadow_to_vmcs12(struct vcpu_vmx *vmx)
{
	int i;
	unsigned long field;
	u64 field_value;
	struct vmcs *shadow_vmcs = vmx->nested.current_shadow_vmcs;
	const unsigned long *fields = shadow_read_write_fields;
	const int num_fields = max_shadow_read_write_fields;

	vmcs_load(shadow_vmcs);

	for (i = 0; i < num_fields; i++) {
		field = fields[i];
		switch (vmcs_field_type(field)) {
		case VMCS_FIELD_TYPE_U16:
			field_value = vmcs_read16(field);
			break;
		case VMCS_FIELD_TYPE_U32:
			field_value = vmcs_read32(field);
			break;
		case VMCS_FIELD_TYPE_U64:
			field_value = vmcs_read64(field);
			break;
		case VMCS_FIELD_TYPE_NATURAL_WIDTH:
			field_value = vmcs_readl(field);
			break;
		}
		vmcs12_write_any(&vmx->vcpu, field, field_value);
	}

	vmcs_clear(shadow_vmcs);
	vmcs_load(vmx->loaded_vmcs->vmcs);
}

static void copy_vmcs12_to_shadow(struct vcpu_vmx *vmx)
{
	const unsigned long *fields[] = {
		shadow_read_write_fields,
		shadow_read_only_fields
	};
	const int max_fields[] = {
		max_shadow_read_write_fields,
		max_shadow_read_only_fields
	};
	int i, q;
	unsigned long field;
	u64 field_value = 0;
	struct vmcs *shadow_vmcs = vmx->nested.current_shadow_vmcs;

	vmcs_load(shadow_vmcs);

	for (q = 0; q < ARRAY_SIZE(fields); q++) {
		for (i = 0; i < max_fields[q]; i++) {
			field = fields[q][i];
			vmcs12_read_any(&vmx->vcpu, field, &field_value);

			switch (vmcs_field_type(field)) {
			case VMCS_FIELD_TYPE_U16:
				vmcs_write16(field, (u16)field_value);
				break;
			case VMCS_FIELD_TYPE_U32:
				vmcs_write32(field, (u32)field_value);
				break;
			case VMCS_FIELD_TYPE_U64:
				vmcs_write64(field, (u64)field_value);
				break;
			case VMCS_FIELD_TYPE_NATURAL_WIDTH:
				vmcs_writel(field, (long)field_value);
				break;
			}
		}
	}

	vmcs_clear(shadow_vmcs);
	vmcs_load(vmx->loaded_vmcs->vmcs);
}

>>>>>>> refs/remotes/origin/master
/*
 * VMX instructions which assume a current vmcs12 (i.e., that VMPTRLD was
 * used before) all generate the same failure when it is missing.
 */
static int nested_vmx_check_vmcs12(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	if (vmx->nested.current_vmptr == -1ull) {
		nested_vmx_failInvalid(vcpu);
		skip_emulated_instruction(vcpu);
		return 0;
	}
	return 1;
}

static int handle_vmread(struct kvm_vcpu *vcpu)
{
	unsigned long field;
	u64 field_value;
	unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	u32 vmx_instruction_info = vmcs_read32(VMX_INSTRUCTION_INFO);
	gva_t gva = 0;

	if (!nested_vmx_check_permission(vcpu) ||
	    !nested_vmx_check_vmcs12(vcpu))
		return 1;

	/* Decode instruction info and find the field to read */
	field = kvm_register_read(vcpu, (((vmx_instruction_info) >> 28) & 0xf));
	/* Read the field, zero-extended to a u64 field_value */
	if (!vmcs12_read_any(vcpu, field, &field_value)) {
		nested_vmx_failValid(vcpu, VMXERR_UNSUPPORTED_VMCS_COMPONENT);
		skip_emulated_instruction(vcpu);
		return 1;
	}
	/*
	 * Now copy part of this value to register or memory, as requested.
	 * Note that the number of bits actually copied is 32 or 64 depending
	 * on the guest's mode (32 or 64 bit), not on the given field's length.
	 */
	if (vmx_instruction_info & (1u << 10)) {
		kvm_register_write(vcpu, (((vmx_instruction_info) >> 3) & 0xf),
			field_value);
	} else {
		if (get_vmx_mem_address(vcpu, exit_qualification,
				vmx_instruction_info, &gva))
			return 1;
		/* _system ok, as nested_vmx_check_permission verified cpl=0 */
		kvm_write_guest_virt_system(&vcpu->arch.emulate_ctxt, gva,
			     &field_value, (is_long_mode(vcpu) ? 8 : 4), NULL);
	}

	nested_vmx_succeed(vcpu);
	skip_emulated_instruction(vcpu);
	return 1;
}


static int handle_vmwrite(struct kvm_vcpu *vcpu)
{
	unsigned long field;
	gva_t gva;
	unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	u32 vmx_instruction_info = vmcs_read32(VMX_INSTRUCTION_INFO);
<<<<<<< HEAD
	char *p;
	short offset;
=======
>>>>>>> refs/remotes/origin/master
	/* The value to write might be 32 or 64 bits, depending on L1's long
	 * mode, and eventually we need to write that into a field of several
	 * possible lengths. The code below first zero-extends the value to 64
	 * bit (field_value), and then copies only the approriate number of
	 * bits into the vmcs12 field.
	 */
	u64 field_value = 0;
	struct x86_exception e;

	if (!nested_vmx_check_permission(vcpu) ||
	    !nested_vmx_check_vmcs12(vcpu))
		return 1;

	if (vmx_instruction_info & (1u << 10))
		field_value = kvm_register_read(vcpu,
			(((vmx_instruction_info) >> 3) & 0xf));
	else {
		if (get_vmx_mem_address(vcpu, exit_qualification,
				vmx_instruction_info, &gva))
			return 1;
		if (kvm_read_guest_virt(&vcpu->arch.emulate_ctxt, gva,
			   &field_value, (is_long_mode(vcpu) ? 8 : 4), &e)) {
			kvm_inject_page_fault(vcpu, &e);
			return 1;
		}
	}


	field = kvm_register_read(vcpu, (((vmx_instruction_info) >> 28) & 0xf));
	if (vmcs_field_readonly(field)) {
		nested_vmx_failValid(vcpu,
			VMXERR_VMWRITE_READ_ONLY_VMCS_COMPONENT);
		skip_emulated_instruction(vcpu);
		return 1;
	}

<<<<<<< HEAD
	offset = vmcs_field_to_offset(field);
	if (offset < 0) {
		nested_vmx_failValid(vcpu, VMXERR_UNSUPPORTED_VMCS_COMPONENT);
		skip_emulated_instruction(vcpu);
		return 1;
	}
	p = ((char *) get_vmcs12(vcpu)) + offset;

	switch (vmcs_field_type(field)) {
	case VMCS_FIELD_TYPE_U16:
		*(u16 *)p = field_value;
		break;
	case VMCS_FIELD_TYPE_U32:
		*(u32 *)p = field_value;
		break;
	case VMCS_FIELD_TYPE_U64:
		*(u64 *)p = field_value;
		break;
	case VMCS_FIELD_TYPE_NATURAL_WIDTH:
		*(natural_width *)p = field_value;
		break;
	default:
=======
	if (!vmcs12_write_any(vcpu, field, field_value)) {
>>>>>>> refs/remotes/origin/master
		nested_vmx_failValid(vcpu, VMXERR_UNSUPPORTED_VMCS_COMPONENT);
		skip_emulated_instruction(vcpu);
		return 1;
	}

	nested_vmx_succeed(vcpu);
	skip_emulated_instruction(vcpu);
	return 1;
}

/* Emulate the VMPTRLD instruction */
static int handle_vmptrld(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	gva_t gva;
	gpa_t vmptr;
	struct x86_exception e;
<<<<<<< HEAD
=======
	u32 exec_control;
>>>>>>> refs/remotes/origin/master

	if (!nested_vmx_check_permission(vcpu))
		return 1;

	if (get_vmx_mem_address(vcpu, vmcs_readl(EXIT_QUALIFICATION),
			vmcs_read32(VMX_INSTRUCTION_INFO), &gva))
		return 1;

	if (kvm_read_guest_virt(&vcpu->arch.emulate_ctxt, gva, &vmptr,
				sizeof(vmptr), &e)) {
		kvm_inject_page_fault(vcpu, &e);
		return 1;
	}

	if (!IS_ALIGNED(vmptr, PAGE_SIZE)) {
		nested_vmx_failValid(vcpu, VMXERR_VMPTRLD_INVALID_ADDRESS);
		skip_emulated_instruction(vcpu);
		return 1;
	}

	if (vmx->nested.current_vmptr != vmptr) {
		struct vmcs12 *new_vmcs12;
		struct page *page;
		page = nested_get_page(vcpu, vmptr);
		if (page == NULL) {
			nested_vmx_failInvalid(vcpu);
			skip_emulated_instruction(vcpu);
			return 1;
		}
		new_vmcs12 = kmap(page);
		if (new_vmcs12->revision_id != VMCS12_REVISION) {
			kunmap(page);
			nested_release_page_clean(page);
			nested_vmx_failValid(vcpu,
				VMXERR_VMPTRLD_INCORRECT_VMCS_REVISION_ID);
			skip_emulated_instruction(vcpu);
			return 1;
		}
<<<<<<< HEAD
		if (vmx->nested.current_vmptr != -1ull) {
			kunmap(vmx->nested.current_vmcs12_page);
			nested_release_page(vmx->nested.current_vmcs12_page);
		}
=======
		if (vmx->nested.current_vmptr != -1ull)
			nested_release_vmcs12(vmx);
>>>>>>> refs/remotes/origin/master

		vmx->nested.current_vmptr = vmptr;
		vmx->nested.current_vmcs12 = new_vmcs12;
		vmx->nested.current_vmcs12_page = page;
<<<<<<< HEAD
=======
		if (enable_shadow_vmcs) {
			exec_control = vmcs_read32(SECONDARY_VM_EXEC_CONTROL);
			exec_control |= SECONDARY_EXEC_SHADOW_VMCS;
			vmcs_write32(SECONDARY_VM_EXEC_CONTROL, exec_control);
			vmcs_write64(VMCS_LINK_POINTER,
				     __pa(vmx->nested.current_shadow_vmcs));
			vmx->nested.sync_shadow_vmcs = true;
		}
>>>>>>> refs/remotes/origin/master
	}

	nested_vmx_succeed(vcpu);
	skip_emulated_instruction(vcpu);
	return 1;
}

/* Emulate the VMPTRST instruction */
static int handle_vmptrst(struct kvm_vcpu *vcpu)
{
	unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	u32 vmx_instruction_info = vmcs_read32(VMX_INSTRUCTION_INFO);
	gva_t vmcs_gva;
	struct x86_exception e;

	if (!nested_vmx_check_permission(vcpu))
		return 1;

	if (get_vmx_mem_address(vcpu, exit_qualification,
			vmx_instruction_info, &vmcs_gva))
		return 1;
	/* ok to use *_system, as nested_vmx_check_permission verified cpl=0 */
	if (kvm_write_guest_virt_system(&vcpu->arch.emulate_ctxt, vmcs_gva,
				 (void *)&to_vmx(vcpu)->nested.current_vmptr,
				 sizeof(u64), &e)) {
		kvm_inject_page_fault(vcpu, &e);
		return 1;
	}
	nested_vmx_succeed(vcpu);
	skip_emulated_instruction(vcpu);
	return 1;
}

<<<<<<< HEAD
=======
/* Emulate the INVEPT instruction */
static int handle_invept(struct kvm_vcpu *vcpu)
{
	u32 vmx_instruction_info, types;
	unsigned long type;
	gva_t gva;
	struct x86_exception e;
	struct {
		u64 eptp, gpa;
	} operand;
	u64 eptp_mask = ((1ull << 51) - 1) & PAGE_MASK;

	if (!(nested_vmx_secondary_ctls_high & SECONDARY_EXEC_ENABLE_EPT) ||
	    !(nested_vmx_ept_caps & VMX_EPT_INVEPT_BIT)) {
		kvm_queue_exception(vcpu, UD_VECTOR);
		return 1;
	}

	if (!nested_vmx_check_permission(vcpu))
		return 1;

	if (!kvm_read_cr0_bits(vcpu, X86_CR0_PE)) {
		kvm_queue_exception(vcpu, UD_VECTOR);
		return 1;
	}

	vmx_instruction_info = vmcs_read32(VMX_INSTRUCTION_INFO);
	type = kvm_register_read(vcpu, (vmx_instruction_info >> 28) & 0xf);

	types = (nested_vmx_ept_caps >> VMX_EPT_EXTENT_SHIFT) & 6;

	if (!(types & (1UL << type))) {
		nested_vmx_failValid(vcpu,
				VMXERR_INVALID_OPERAND_TO_INVEPT_INVVPID);
		return 1;
	}

	/* According to the Intel VMX instruction reference, the memory
	 * operand is read even if it isn't needed (e.g., for type==global)
	 */
	if (get_vmx_mem_address(vcpu, vmcs_readl(EXIT_QUALIFICATION),
			vmx_instruction_info, &gva))
		return 1;
	if (kvm_read_guest_virt(&vcpu->arch.emulate_ctxt, gva, &operand,
				sizeof(operand), &e)) {
		kvm_inject_page_fault(vcpu, &e);
		return 1;
	}

	switch (type) {
	case VMX_EPT_EXTENT_CONTEXT:
		if ((operand.eptp & eptp_mask) !=
				(nested_ept_get_cr3(vcpu) & eptp_mask))
			break;
	case VMX_EPT_EXTENT_GLOBAL:
		kvm_mmu_sync_roots(vcpu);
		kvm_mmu_flush_tlb(vcpu);
		nested_vmx_succeed(vcpu);
		break;
	default:
		BUG_ON(1);
		break;
	}

	skip_emulated_instruction(vcpu);
	return 1;
}

>>>>>>> refs/remotes/origin/master
/*
 * The exit handlers return 1 if the exit was handled fully and guest execution
 * may resume.  Otherwise they set the kvm_run parameter to indicate what needs
 * to be done to userspace and return 0.
 */
<<<<<<< HEAD
static int (*kvm_vmx_exit_handlers[])(struct kvm_vcpu *vcpu) = {
=======
static int (*const kvm_vmx_exit_handlers[])(struct kvm_vcpu *vcpu) = {
>>>>>>> refs/remotes/origin/master
	[EXIT_REASON_EXCEPTION_NMI]           = handle_exception,
	[EXIT_REASON_EXTERNAL_INTERRUPT]      = handle_external_interrupt,
	[EXIT_REASON_TRIPLE_FAULT]            = handle_triple_fault,
	[EXIT_REASON_NMI_WINDOW]	      = handle_nmi_window,
	[EXIT_REASON_IO_INSTRUCTION]          = handle_io,
	[EXIT_REASON_CR_ACCESS]               = handle_cr,
	[EXIT_REASON_DR_ACCESS]               = handle_dr,
	[EXIT_REASON_CPUID]                   = handle_cpuid,
	[EXIT_REASON_MSR_READ]                = handle_rdmsr,
	[EXIT_REASON_MSR_WRITE]               = handle_wrmsr,
	[EXIT_REASON_PENDING_INTERRUPT]       = handle_interrupt_window,
	[EXIT_REASON_HLT]                     = handle_halt,
	[EXIT_REASON_INVD]		      = handle_invd,
	[EXIT_REASON_INVLPG]		      = handle_invlpg,
	[EXIT_REASON_RDPMC]                   = handle_rdpmc,
	[EXIT_REASON_VMCALL]                  = handle_vmcall,
	[EXIT_REASON_VMCLEAR]	              = handle_vmclear,
	[EXIT_REASON_VMLAUNCH]                = handle_vmlaunch,
	[EXIT_REASON_VMPTRLD]                 = handle_vmptrld,
	[EXIT_REASON_VMPTRST]                 = handle_vmptrst,
	[EXIT_REASON_VMREAD]                  = handle_vmread,
	[EXIT_REASON_VMRESUME]                = handle_vmresume,
	[EXIT_REASON_VMWRITE]                 = handle_vmwrite,
	[EXIT_REASON_VMOFF]                   = handle_vmoff,
	[EXIT_REASON_VMON]                    = handle_vmon,
	[EXIT_REASON_TPR_BELOW_THRESHOLD]     = handle_tpr_below_threshold,
	[EXIT_REASON_APIC_ACCESS]             = handle_apic_access,
<<<<<<< HEAD
=======
	[EXIT_REASON_APIC_WRITE]              = handle_apic_write,
	[EXIT_REASON_EOI_INDUCED]             = handle_apic_eoi_induced,
>>>>>>> refs/remotes/origin/master
	[EXIT_REASON_WBINVD]                  = handle_wbinvd,
	[EXIT_REASON_XSETBV]                  = handle_xsetbv,
	[EXIT_REASON_TASK_SWITCH]             = handle_task_switch,
	[EXIT_REASON_MCE_DURING_VMENTRY]      = handle_machine_check,
	[EXIT_REASON_EPT_VIOLATION]	      = handle_ept_violation,
	[EXIT_REASON_EPT_MISCONFIG]           = handle_ept_misconfig,
	[EXIT_REASON_PAUSE_INSTRUCTION]       = handle_pause,
	[EXIT_REASON_MWAIT_INSTRUCTION]	      = handle_invalid_op,
	[EXIT_REASON_MONITOR_INSTRUCTION]     = handle_invalid_op,
<<<<<<< HEAD
=======
	[EXIT_REASON_INVEPT]                  = handle_invept,
>>>>>>> refs/remotes/origin/master
};

static const int kvm_vmx_max_exit_handlers =
	ARRAY_SIZE(kvm_vmx_exit_handlers);

<<<<<<< HEAD
=======
static bool nested_vmx_exit_handled_io(struct kvm_vcpu *vcpu,
				       struct vmcs12 *vmcs12)
{
	unsigned long exit_qualification;
	gpa_t bitmap, last_bitmap;
	unsigned int port;
	int size;
	u8 b;

	if (nested_cpu_has(vmcs12, CPU_BASED_UNCOND_IO_EXITING))
		return 1;

	if (!nested_cpu_has(vmcs12, CPU_BASED_USE_IO_BITMAPS))
		return 0;

	exit_qualification = vmcs_readl(EXIT_QUALIFICATION);

	port = exit_qualification >> 16;
	size = (exit_qualification & 7) + 1;

	last_bitmap = (gpa_t)-1;
	b = -1;

	while (size > 0) {
		if (port < 0x8000)
			bitmap = vmcs12->io_bitmap_a;
		else if (port < 0x10000)
			bitmap = vmcs12->io_bitmap_b;
		else
			return 1;
		bitmap += (port & 0x7fff) / 8;

		if (last_bitmap != bitmap)
			if (kvm_read_guest(vcpu->kvm, bitmap, &b, 1))
				return 1;
		if (b & (1 << (port & 7)))
			return 1;

		port++;
		size--;
		last_bitmap = bitmap;
	}

	return 0;
}

>>>>>>> refs/remotes/origin/master
/*
 * Return 1 if we should exit from L2 to L1 to handle an MSR access access,
 * rather than handle it ourselves in L0. I.e., check whether L1 expressed
 * disinterest in the current event (read or write a specific MSR) by using an
 * MSR bitmap. This may be the case even when L0 doesn't use MSR bitmaps.
 */
static bool nested_vmx_exit_handled_msr(struct kvm_vcpu *vcpu,
	struct vmcs12 *vmcs12, u32 exit_reason)
{
	u32 msr_index = vcpu->arch.regs[VCPU_REGS_RCX];
	gpa_t bitmap;

<<<<<<< HEAD
	if (!nested_cpu_has(get_vmcs12(vcpu), CPU_BASED_USE_MSR_BITMAPS))
=======
	if (!nested_cpu_has(vmcs12, CPU_BASED_USE_MSR_BITMAPS))
>>>>>>> refs/remotes/origin/master
		return 1;

	/*
	 * The MSR_BITMAP page is divided into four 1024-byte bitmaps,
	 * for the four combinations of read/write and low/high MSR numbers.
	 * First we need to figure out which of the four to use:
	 */
	bitmap = vmcs12->msr_bitmap;
	if (exit_reason == EXIT_REASON_MSR_WRITE)
		bitmap += 2048;
	if (msr_index >= 0xc0000000) {
		msr_index -= 0xc0000000;
		bitmap += 1024;
	}

	/* Then read the msr_index'th bit from this bitmap: */
	if (msr_index < 1024*8) {
		unsigned char b;
<<<<<<< HEAD
		kvm_read_guest(vcpu->kvm, bitmap + msr_index/8, &b, 1);
=======
		if (kvm_read_guest(vcpu->kvm, bitmap + msr_index/8, &b, 1))
			return 1;
>>>>>>> refs/remotes/origin/master
		return 1 & (b >> (msr_index & 7));
	} else
		return 1; /* let L1 handle the wrong parameter */
}

/*
 * Return 1 if we should exit from L2 to L1 to handle a CR access exit,
 * rather than handle it ourselves in L0. I.e., check if L1 wanted to
 * intercept (via guest_host_mask etc.) the current event.
 */
static bool nested_vmx_exit_handled_cr(struct kvm_vcpu *vcpu,
	struct vmcs12 *vmcs12)
{
	unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	int cr = exit_qualification & 15;
	int reg = (exit_qualification >> 8) & 15;
	unsigned long val = kvm_register_read(vcpu, reg);

	switch ((exit_qualification >> 4) & 3) {
	case 0: /* mov to cr */
		switch (cr) {
		case 0:
			if (vmcs12->cr0_guest_host_mask &
			    (val ^ vmcs12->cr0_read_shadow))
				return 1;
			break;
		case 3:
			if ((vmcs12->cr3_target_count >= 1 &&
					vmcs12->cr3_target_value0 == val) ||
				(vmcs12->cr3_target_count >= 2 &&
					vmcs12->cr3_target_value1 == val) ||
				(vmcs12->cr3_target_count >= 3 &&
					vmcs12->cr3_target_value2 == val) ||
				(vmcs12->cr3_target_count >= 4 &&
					vmcs12->cr3_target_value3 == val))
				return 0;
			if (nested_cpu_has(vmcs12, CPU_BASED_CR3_LOAD_EXITING))
				return 1;
			break;
		case 4:
			if (vmcs12->cr4_guest_host_mask &
			    (vmcs12->cr4_read_shadow ^ val))
				return 1;
			break;
		case 8:
			if (nested_cpu_has(vmcs12, CPU_BASED_CR8_LOAD_EXITING))
				return 1;
			break;
		}
		break;
	case 2: /* clts */
		if ((vmcs12->cr0_guest_host_mask & X86_CR0_TS) &&
		    (vmcs12->cr0_read_shadow & X86_CR0_TS))
			return 1;
		break;
	case 1: /* mov from cr */
		switch (cr) {
		case 3:
			if (vmcs12->cpu_based_vm_exec_control &
			    CPU_BASED_CR3_STORE_EXITING)
				return 1;
			break;
		case 8:
			if (vmcs12->cpu_based_vm_exec_control &
			    CPU_BASED_CR8_STORE_EXITING)
				return 1;
			break;
		}
		break;
	case 3: /* lmsw */
		/*
		 * lmsw can change bits 1..3 of cr0, and only set bit 0 of
		 * cr0. Other attempted changes are ignored, with no exit.
		 */
		if (vmcs12->cr0_guest_host_mask & 0xe &
		    (val ^ vmcs12->cr0_read_shadow))
			return 1;
		if ((vmcs12->cr0_guest_host_mask & 0x1) &&
		    !(vmcs12->cr0_read_shadow & 0x1) &&
		    (val & 0x1))
			return 1;
		break;
	}
	return 0;
}

/*
 * Return 1 if we should exit from L2 to L1 to handle an exit, or 0 if we
 * should handle it ourselves in L0 (and then continue L2). Only call this
 * when in is_guest_mode (L2).
 */
static bool nested_vmx_exit_handled(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	u32 exit_reason = vmcs_read32(VM_EXIT_REASON);
	u32 intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
=======
	u32 intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
	u32 exit_reason = vmx->exit_reason;
>>>>>>> refs/remotes/origin/master

	if (vmx->nested.nested_run_pending)
		return 0;

	if (unlikely(vmx->fail)) {
		pr_info_ratelimited("%s failed vm entry %x\n", __func__,
				    vmcs_read32(VM_INSTRUCTION_ERROR));
		return 1;
	}

	switch (exit_reason) {
	case EXIT_REASON_EXCEPTION_NMI:
		if (!is_exception(intr_info))
			return 0;
		else if (is_page_fault(intr_info))
			return enable_ept;
<<<<<<< HEAD
=======
		else if (is_no_device(intr_info) &&
			 !(nested_read_cr0(vmcs12) & X86_CR0_TS))
			return 0;
>>>>>>> refs/remotes/origin/master
		return vmcs12->exception_bitmap &
				(1u << (intr_info & INTR_INFO_VECTOR_MASK));
	case EXIT_REASON_EXTERNAL_INTERRUPT:
		return 0;
	case EXIT_REASON_TRIPLE_FAULT:
		return 1;
	case EXIT_REASON_PENDING_INTERRUPT:
<<<<<<< HEAD
	case EXIT_REASON_NMI_WINDOW:
		/*
		 * prepare_vmcs02() set the CPU_BASED_VIRTUAL_INTR_PENDING bit
		 * (aka Interrupt Window Exiting) only when L1 turned it on,
		 * so if we got a PENDING_INTERRUPT exit, this must be for L1.
		 * Same for NMI Window Exiting.
		 */
		return 1;
=======
		return nested_cpu_has(vmcs12, CPU_BASED_VIRTUAL_INTR_PENDING);
	case EXIT_REASON_NMI_WINDOW:
		return nested_cpu_has(vmcs12, CPU_BASED_VIRTUAL_NMI_PENDING);
>>>>>>> refs/remotes/origin/master
	case EXIT_REASON_TASK_SWITCH:
		return 1;
	case EXIT_REASON_CPUID:
		return 1;
	case EXIT_REASON_HLT:
		return nested_cpu_has(vmcs12, CPU_BASED_HLT_EXITING);
	case EXIT_REASON_INVD:
		return 1;
	case EXIT_REASON_INVLPG:
		return nested_cpu_has(vmcs12, CPU_BASED_INVLPG_EXITING);
	case EXIT_REASON_RDPMC:
		return nested_cpu_has(vmcs12, CPU_BASED_RDPMC_EXITING);
	case EXIT_REASON_RDTSC:
		return nested_cpu_has(vmcs12, CPU_BASED_RDTSC_EXITING);
	case EXIT_REASON_VMCALL: case EXIT_REASON_VMCLEAR:
	case EXIT_REASON_VMLAUNCH: case EXIT_REASON_VMPTRLD:
	case EXIT_REASON_VMPTRST: case EXIT_REASON_VMREAD:
	case EXIT_REASON_VMRESUME: case EXIT_REASON_VMWRITE:
	case EXIT_REASON_VMOFF: case EXIT_REASON_VMON:
<<<<<<< HEAD
=======
	case EXIT_REASON_INVEPT:
>>>>>>> refs/remotes/origin/master
		/*
		 * VMX instructions trap unconditionally. This allows L1 to
		 * emulate them for its L2 guest, i.e., allows 3-level nesting!
		 */
		return 1;
	case EXIT_REASON_CR_ACCESS:
		return nested_vmx_exit_handled_cr(vcpu, vmcs12);
	case EXIT_REASON_DR_ACCESS:
		return nested_cpu_has(vmcs12, CPU_BASED_MOV_DR_EXITING);
	case EXIT_REASON_IO_INSTRUCTION:
<<<<<<< HEAD
		/* TODO: support IO bitmaps */
		return 1;
=======
		return nested_vmx_exit_handled_io(vcpu, vmcs12);
>>>>>>> refs/remotes/origin/master
	case EXIT_REASON_MSR_READ:
	case EXIT_REASON_MSR_WRITE:
		return nested_vmx_exit_handled_msr(vcpu, vmcs12, exit_reason);
	case EXIT_REASON_INVALID_STATE:
		return 1;
	case EXIT_REASON_MWAIT_INSTRUCTION:
		return nested_cpu_has(vmcs12, CPU_BASED_MWAIT_EXITING);
	case EXIT_REASON_MONITOR_INSTRUCTION:
		return nested_cpu_has(vmcs12, CPU_BASED_MONITOR_EXITING);
	case EXIT_REASON_PAUSE_INSTRUCTION:
		return nested_cpu_has(vmcs12, CPU_BASED_PAUSE_EXITING) ||
			nested_cpu_has2(vmcs12,
				SECONDARY_EXEC_PAUSE_LOOP_EXITING);
	case EXIT_REASON_MCE_DURING_VMENTRY:
		return 0;
	case EXIT_REASON_TPR_BELOW_THRESHOLD:
		return 1;
	case EXIT_REASON_APIC_ACCESS:
		return nested_cpu_has2(vmcs12,
			SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES);
	case EXIT_REASON_EPT_VIOLATION:
<<<<<<< HEAD
	case EXIT_REASON_EPT_MISCONFIG:
		return 0;
=======
		/*
		 * L0 always deals with the EPT violation. If nested EPT is
		 * used, and the nested mmu code discovers that the address is
		 * missing in the guest EPT table (EPT12), the EPT violation
		 * will be injected with nested_ept_inject_page_fault()
		 */
		return 0;
	case EXIT_REASON_EPT_MISCONFIG:
		/*
		 * L2 never uses directly L1's EPT, but rather L0's own EPT
		 * table (shadow on EPT) or a merged EPT table that L0 built
		 * (EPT on EPT). So any problems with the structure of the
		 * table is L0's fault.
		 */
		return 0;
	case EXIT_REASON_PREEMPTION_TIMER:
		return vmcs12->pin_based_vm_exec_control &
			PIN_BASED_VMX_PREEMPTION_TIMER;
>>>>>>> refs/remotes/origin/master
	case EXIT_REASON_WBINVD:
		return nested_cpu_has2(vmcs12, SECONDARY_EXEC_WBINVD_EXITING);
	case EXIT_REASON_XSETBV:
		return 1;
	default:
		return 1;
	}
}

static void vmx_get_exit_info(struct kvm_vcpu *vcpu, u64 *info1, u64 *info2)
{
	*info1 = vmcs_readl(EXIT_QUALIFICATION);
	*info2 = vmcs_read32(VM_EXIT_INTR_INFO);
}

<<<<<<< HEAD
=======
static void nested_adjust_preemption_timer(struct kvm_vcpu *vcpu)
{
	u64 delta_tsc_l1;
	u32 preempt_val_l1, preempt_val_l2, preempt_scale;

	if (!(get_vmcs12(vcpu)->pin_based_vm_exec_control &
			PIN_BASED_VMX_PREEMPTION_TIMER))
		return;
	preempt_scale = native_read_msr(MSR_IA32_VMX_MISC) &
			MSR_IA32_VMX_MISC_PREEMPTION_TIMER_SCALE;
	preempt_val_l2 = vmcs_read32(VMX_PREEMPTION_TIMER_VALUE);
	delta_tsc_l1 = vmx_read_l1_tsc(vcpu, native_read_tsc())
		- vcpu->arch.last_guest_tsc;
	preempt_val_l1 = delta_tsc_l1 >> preempt_scale;
	if (preempt_val_l2 <= preempt_val_l1)
		preempt_val_l2 = 0;
	else
		preempt_val_l2 -= preempt_val_l1;
	vmcs_write32(VMX_PREEMPTION_TIMER_VALUE, preempt_val_l2);
}

>>>>>>> refs/remotes/origin/master
/*
 * The guest has exited.  See if we can fix it or if we need userspace
 * assistance.
 */
static int vmx_handle_exit(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u32 exit_reason = vmx->exit_reason;
	u32 vectoring_info = vmx->idt_vectoring_info;

	/* If guest state is invalid, start emulating */
<<<<<<< HEAD
	if (vmx->emulation_required && emulate_invalid_guest_state)
		return handle_invalid_guest_state(vcpu);

	/*
	 * the KVM_REQ_EVENT optimization bit is only on for one entry, and if
	 * we did not inject a still-pending event to L1 now because of
	 * nested_run_pending, we need to re-enable this bit.
	 */
	if (vmx->nested.nested_run_pending)
		kvm_make_request(KVM_REQ_EVENT, vcpu);

	if (!is_guest_mode(vcpu) && (exit_reason == EXIT_REASON_VMLAUNCH ||
	    exit_reason == EXIT_REASON_VMRESUME))
		vmx->nested.nested_run_pending = 1;
	else
		vmx->nested.nested_run_pending = 0;

=======
	if (vmx->emulation_required)
		return handle_invalid_guest_state(vcpu);

>>>>>>> refs/remotes/origin/master
	if (is_guest_mode(vcpu) && nested_vmx_exit_handled(vcpu)) {
		nested_vmx_vmexit(vcpu);
		return 1;
	}

	if (exit_reason & VMX_EXIT_REASONS_FAILED_VMENTRY) {
		vcpu->run->exit_reason = KVM_EXIT_FAIL_ENTRY;
		vcpu->run->fail_entry.hardware_entry_failure_reason
			= exit_reason;
		return 0;
	}

	if (unlikely(vmx->fail)) {
		vcpu->run->exit_reason = KVM_EXIT_FAIL_ENTRY;
		vcpu->run->fail_entry.hardware_entry_failure_reason
			= vmcs_read32(VM_INSTRUCTION_ERROR);
		return 0;
	}

<<<<<<< HEAD
	if ((vectoring_info & VECTORING_INFO_VALID_MASK) &&
>>>>>>> refs/remotes/origin/cm-10.0
			(exit_reason != EXIT_REASON_EXCEPTION_NMI &&
			exit_reason != EXIT_REASON_EPT_VIOLATION &&
			exit_reason != EXIT_REASON_TASK_SWITCH))
		printk(KERN_WARNING "%s: unexpected, valid vectoring info "
		       "(0x%x) and exit reason is 0x%x\n",
		       __func__, vectoring_info, exit_reason);

<<<<<<< HEAD
	if (unlikely(!cpu_has_virtual_nmis() && vmx->soft_vnmi_blocked)) {
=======
	if (unlikely(!cpu_has_virtual_nmis() && vmx->soft_vnmi_blocked &&
	    !(is_guest_mode(vcpu) && nested_cpu_has_virtual_nmis(
	                                get_vmcs12(vcpu), vcpu)))) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/*
	 * Note:
	 * Do not try to fix EXIT_REASON_EPT_MISCONFIG if it caused by
	 * delivery event since it indicates guest is accessing MMIO.
	 * The vm-exit can be triggered again after return to guest that
	 * will cause infinite loop.
	 */
	if ((vectoring_info & VECTORING_INFO_VALID_MASK) &&
			(exit_reason != EXIT_REASON_EXCEPTION_NMI &&
			exit_reason != EXIT_REASON_EPT_VIOLATION &&
			exit_reason != EXIT_REASON_TASK_SWITCH)) {
		vcpu->run->exit_reason = KVM_EXIT_INTERNAL_ERROR;
		vcpu->run->internal.suberror = KVM_INTERNAL_ERROR_DELIVERY_EV;
		vcpu->run->internal.ndata = 2;
		vcpu->run->internal.data[0] = vectoring_info;
		vcpu->run->internal.data[1] = exit_reason;
		return 0;
	}

	if (unlikely(!cpu_has_virtual_nmis() && vmx->soft_vnmi_blocked &&
	    !(is_guest_mode(vcpu) && nested_cpu_has_virtual_nmis(
					get_vmcs12(vcpu))))) {
>>>>>>> refs/remotes/origin/master
		if (vmx_interrupt_allowed(vcpu)) {
			vmx->soft_vnmi_blocked = 0;
		} else if (vmx->vnmi_blocked_time > 1000000000LL &&
			   vcpu->arch.nmi_pending) {
			/*
			 * This CPU don't support us in finding the end of an
			 * NMI-blocked window if the guest runs with IRQs
			 * disabled. So we pull the trigger after 1 s of
			 * futile waiting, but inform the user about this.
			 */
			printk(KERN_WARNING "%s: Breaking out of NMI-blocked "
			       "state on VCPU %d after 1 s timeout\n",
			       __func__, vcpu->vcpu_id);
			vmx->soft_vnmi_blocked = 0;
		}
	}

	if (exit_reason < kvm_vmx_max_exit_handlers
	    && kvm_vmx_exit_handlers[exit_reason])
		return kvm_vmx_exit_handlers[exit_reason](vcpu);
	else {
		vcpu->run->exit_reason = KVM_EXIT_UNKNOWN;
		vcpu->run->hw.hardware_exit_reason = exit_reason;
	}
	return 0;
}

static void update_cr8_intercept(struct kvm_vcpu *vcpu, int tpr, int irr)
{
	if (irr == -1 || tpr < irr) {
		vmcs_write32(TPR_THRESHOLD, 0);
		return;
	}

	vmcs_write32(TPR_THRESHOLD, irr);
}

<<<<<<< HEAD
=======
static void vmx_set_virtual_x2apic_mode(struct kvm_vcpu *vcpu, bool set)
{
	u32 sec_exec_control;

	/*
	 * There is not point to enable virtualize x2apic without enable
	 * apicv
	 */
	if (!cpu_has_vmx_virtualize_x2apic_mode() ||
				!vmx_vm_has_apicv(vcpu->kvm))
		return;

	if (!vm_need_tpr_shadow(vcpu->kvm))
		return;

	sec_exec_control = vmcs_read32(SECONDARY_VM_EXEC_CONTROL);

	if (set) {
		sec_exec_control &= ~SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
		sec_exec_control |= SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE;
	} else {
		sec_exec_control &= ~SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE;
		sec_exec_control |= SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
	}
	vmcs_write32(SECONDARY_VM_EXEC_CONTROL, sec_exec_control);

	vmx_set_msr_bitmap(vcpu);
}

static void vmx_hwapic_isr_update(struct kvm *kvm, int isr)
{
	u16 status;
	u8 old;

	if (!vmx_vm_has_apicv(kvm))
		return;

	if (isr == -1)
		isr = 0;

	status = vmcs_read16(GUEST_INTR_STATUS);
	old = status >> 8;
	if (isr != old) {
		status &= 0xff;
		status |= isr << 8;
		vmcs_write16(GUEST_INTR_STATUS, status);
	}
}

static void vmx_set_rvi(int vector)
{
	u16 status;
	u8 old;

	status = vmcs_read16(GUEST_INTR_STATUS);
	old = (u8)status & 0xff;
	if ((u8)vector != old) {
		status &= ~0xff;
		status |= (u8)vector;
		vmcs_write16(GUEST_INTR_STATUS, status);
	}
}

static void vmx_hwapic_irr_update(struct kvm_vcpu *vcpu, int max_irr)
{
	if (max_irr == -1)
		return;

	vmx_set_rvi(max_irr);
}

static void vmx_load_eoi_exitmap(struct kvm_vcpu *vcpu, u64 *eoi_exit_bitmap)
{
	if (!vmx_vm_has_apicv(vcpu->kvm))
		return;

	vmcs_write64(EOI_EXIT_BITMAP0, eoi_exit_bitmap[0]);
	vmcs_write64(EOI_EXIT_BITMAP1, eoi_exit_bitmap[1]);
	vmcs_write64(EOI_EXIT_BITMAP2, eoi_exit_bitmap[2]);
	vmcs_write64(EOI_EXIT_BITMAP3, eoi_exit_bitmap[3]);
}

>>>>>>> refs/remotes/origin/master
static void vmx_complete_atomic_exit(struct vcpu_vmx *vmx)
{
	u32 exit_intr_info;

	if (!(vmx->exit_reason == EXIT_REASON_MCE_DURING_VMENTRY
	      || vmx->exit_reason == EXIT_REASON_EXCEPTION_NMI))
		return;

	vmx->exit_intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
	exit_intr_info = vmx->exit_intr_info;

	/* Handle machine checks before interrupts are enabled */
	if (is_machine_check(exit_intr_info))
		kvm_machine_check();

	/* We need to handle NMIs before interrupts are enabled */
	if ((exit_intr_info & INTR_INFO_INTR_TYPE_MASK) == INTR_TYPE_NMI_INTR &&
	    (exit_intr_info & INTR_INFO_VALID_MASK)) {
		kvm_before_handle_nmi(&vmx->vcpu);
		asm("int $2");
		kvm_after_handle_nmi(&vmx->vcpu);
	}
}

<<<<<<< HEAD
=======
static void vmx_handle_external_intr(struct kvm_vcpu *vcpu)
{
	u32 exit_intr_info = vmcs_read32(VM_EXIT_INTR_INFO);

	/*
	 * If external interrupt exists, IF bit is set in rflags/eflags on the
	 * interrupt stack frame, and interrupt will be enabled on a return
	 * from interrupt handler.
	 */
	if ((exit_intr_info & (INTR_INFO_VALID_MASK | INTR_INFO_INTR_TYPE_MASK))
			== (INTR_INFO_VALID_MASK | INTR_TYPE_EXT_INTR)) {
		unsigned int vector;
		unsigned long entry;
		gate_desc *desc;
		struct vcpu_vmx *vmx = to_vmx(vcpu);
#ifdef CONFIG_X86_64
		unsigned long tmp;
#endif

		vector =  exit_intr_info & INTR_INFO_VECTOR_MASK;
		desc = (gate_desc *)vmx->host_idt_base + vector;
		entry = gate_offset(*desc);
		asm volatile(
#ifdef CONFIG_X86_64
			"mov %%" _ASM_SP ", %[sp]\n\t"
			"and $0xfffffffffffffff0, %%" _ASM_SP "\n\t"
			"push $%c[ss]\n\t"
			"push %[sp]\n\t"
#endif
			"pushf\n\t"
			"orl $0x200, (%%" _ASM_SP ")\n\t"
			__ASM_SIZE(push) " $%c[cs]\n\t"
			"call *%[entry]\n\t"
			:
#ifdef CONFIG_X86_64
			[sp]"=&r"(tmp)
#endif
			:
			[entry]"r"(entry),
			[ss]"i"(__KERNEL_DS),
			[cs]"i"(__KERNEL_CS)
			);
	} else
		local_irq_enable();
}

>>>>>>> refs/remotes/origin/master
static void vmx_recover_nmi_blocking(struct vcpu_vmx *vmx)
{
	u32 exit_intr_info;
	bool unblock_nmi;
	u8 vector;
	bool idtv_info_valid;

	idtv_info_valid = vmx->idt_vectoring_info & VECTORING_INFO_VALID_MASK;

	if (cpu_has_virtual_nmis()) {
		if (vmx->nmi_known_unmasked)
			return;
		/*
		 * Can't use vmx->exit_intr_info since we're not sure what
		 * the exit reason is.
		 */
		exit_intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
		unblock_nmi = (exit_intr_info & INTR_INFO_UNBLOCK_NMI) != 0;
		vector = exit_intr_info & INTR_INFO_VECTOR_MASK;
		/*
		 * SDM 3: 27.7.1.2 (September 2008)
		 * Re-set bit "block by NMI" before VM entry if vmexit caused by
		 * a guest IRET fault.
		 * SDM 3: 23.2.2 (September 2008)
		 * Bit 12 is undefined in any of the following cases:
		 *  If the VM exit sets the valid bit in the IDT-vectoring
		 *   information field.
		 *  If the VM exit is due to a double fault.
		 */
		if ((exit_intr_info & INTR_INFO_VALID_MASK) && unblock_nmi &&
		    vector != DF_VECTOR && !idtv_info_valid)
			vmcs_set_bits(GUEST_INTERRUPTIBILITY_INFO,
				      GUEST_INTR_STATE_NMI);
		else
			vmx->nmi_known_unmasked =
				!(vmcs_read32(GUEST_INTERRUPTIBILITY_INFO)
				  & GUEST_INTR_STATE_NMI);
	} else if (unlikely(vmx->soft_vnmi_blocked))
		vmx->vnmi_blocked_time +=
			ktime_to_ns(ktime_sub(ktime_get(), vmx->entry_time));
}

<<<<<<< HEAD
static void __vmx_complete_interrupts(struct vcpu_vmx *vmx,
=======
static void __vmx_complete_interrupts(struct kvm_vcpu *vcpu,
>>>>>>> refs/remotes/origin/master
				      u32 idt_vectoring_info,
				      int instr_len_field,
				      int error_code_field)
{
	u8 vector;
	int type;
	bool idtv_info_valid;

	idtv_info_valid = idt_vectoring_info & VECTORING_INFO_VALID_MASK;

<<<<<<< HEAD
	vmx->vcpu.arch.nmi_injected = false;
	kvm_clear_exception_queue(&vmx->vcpu);
	kvm_clear_interrupt_queue(&vmx->vcpu);
=======
	vcpu->arch.nmi_injected = false;
	kvm_clear_exception_queue(vcpu);
	kvm_clear_interrupt_queue(vcpu);
>>>>>>> refs/remotes/origin/master

	if (!idtv_info_valid)
		return;

<<<<<<< HEAD
	kvm_make_request(KVM_REQ_EVENT, &vmx->vcpu);
=======
	kvm_make_request(KVM_REQ_EVENT, vcpu);
>>>>>>> refs/remotes/origin/master

	vector = idt_vectoring_info & VECTORING_INFO_VECTOR_MASK;
	type = idt_vectoring_info & VECTORING_INFO_TYPE_MASK;

	switch (type) {
	case INTR_TYPE_NMI_INTR:
<<<<<<< HEAD
		vmx->vcpu.arch.nmi_injected = true;
=======
		vcpu->arch.nmi_injected = true;
>>>>>>> refs/remotes/origin/master
		/*
		 * SDM 3: 27.7.1.2 (September 2008)
		 * Clear bit "block by NMI" before VM entry if a NMI
		 * delivery faulted.
		 */
<<<<<<< HEAD
		vmx_set_nmi_mask(&vmx->vcpu, false);
		break;
	case INTR_TYPE_SOFT_EXCEPTION:
		vmx->vcpu.arch.event_exit_inst_len =
			vmcs_read32(instr_len_field);
=======
		vmx_set_nmi_mask(vcpu, false);
		break;
	case INTR_TYPE_SOFT_EXCEPTION:
		vcpu->arch.event_exit_inst_len = vmcs_read32(instr_len_field);
>>>>>>> refs/remotes/origin/master
		/* fall through */
	case INTR_TYPE_HARD_EXCEPTION:
		if (idt_vectoring_info & VECTORING_INFO_DELIVER_CODE_MASK) {
			u32 err = vmcs_read32(error_code_field);
<<<<<<< HEAD
			kvm_queue_exception_e(&vmx->vcpu, vector, err);
		} else
			kvm_queue_exception(&vmx->vcpu, vector);
		break;
	case INTR_TYPE_SOFT_INTR:
		vmx->vcpu.arch.event_exit_inst_len =
			vmcs_read32(instr_len_field);
		/* fall through */
	case INTR_TYPE_EXT_INTR:
		kvm_queue_interrupt(&vmx->vcpu, vector,
			type == INTR_TYPE_SOFT_INTR);
=======
			kvm_requeue_exception_e(vcpu, vector, err);
		} else
			kvm_requeue_exception(vcpu, vector);
		break;
	case INTR_TYPE_SOFT_INTR:
		vcpu->arch.event_exit_inst_len = vmcs_read32(instr_len_field);
		/* fall through */
	case INTR_TYPE_EXT_INTR:
		kvm_queue_interrupt(vcpu, vector, type == INTR_TYPE_SOFT_INTR);
>>>>>>> refs/remotes/origin/master
		break;
	default:
		break;
	}
}

static void vmx_complete_interrupts(struct vcpu_vmx *vmx)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (is_guest_mode(&vmx->vcpu))
		return;
>>>>>>> refs/remotes/origin/cm-10.0
	__vmx_complete_interrupts(vmx, vmx->idt_vectoring_info,
=======
	__vmx_complete_interrupts(&vmx->vcpu, vmx->idt_vectoring_info,
>>>>>>> refs/remotes/origin/master
				  VM_EXIT_INSTRUCTION_LEN,
				  IDT_VECTORING_ERROR_CODE);
}

static void vmx_cancel_injection(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (is_guest_mode(vcpu))
		return;
>>>>>>> refs/remotes/origin/cm-10.0
	__vmx_complete_interrupts(to_vmx(vcpu),
=======
	__vmx_complete_interrupts(vcpu,
>>>>>>> refs/remotes/origin/master
				  vmcs_read32(VM_ENTRY_INTR_INFO_FIELD),
				  VM_ENTRY_INSTRUCTION_LEN,
				  VM_ENTRY_EXCEPTION_ERROR_CODE);

	vmcs_write32(VM_ENTRY_INTR_INFO_FIELD, 0);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void atomic_switch_perf_msrs(struct vcpu_vmx *vmx)
{
	int i, nr_msrs;
	struct perf_guest_switch_msr *msrs;

	msrs = perf_guest_get_msrs(&nr_msrs);

	if (!msrs)
		return;

	for (i = 0; i < nr_msrs; i++)
		if (msrs[i].host == msrs[i].guest)
			clear_atomic_switch_msr(vmx, msrs[i].msr);
		else
			add_atomic_switch_msr(vmx, msrs[i].msr, msrs[i].guest,
					msrs[i].host);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_X86_64
#define R "r"
#define Q "q"
#else
#define R "e"
#define Q "l"
#endif

static void __noclone vmx_vcpu_run(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

<<<<<<< HEAD
=======
	if (is_guest_mode(vcpu) && !vmx->nested.nested_run_pending) {
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
		if (vmcs12->idt_vectoring_info_field &
				VECTORING_INFO_VALID_MASK) {
			vmcs_write32(VM_ENTRY_INTR_INFO_FIELD,
				vmcs12->idt_vectoring_info_field);
			vmcs_write32(VM_ENTRY_INSTRUCTION_LEN,
				vmcs12->vm_exit_instruction_len);
			if (vmcs12->idt_vectoring_info_field &
					VECTORING_INFO_DELIVER_CODE_MASK)
				vmcs_write32(VM_ENTRY_EXCEPTION_ERROR_CODE,
					vmcs12->idt_vectoring_error_code);
		}
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static void __noclone vmx_vcpu_run(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	unsigned long debugctlmsr;

>>>>>>> refs/remotes/origin/master
	/* Record the guest's net vcpu time for enforced NMI injections. */
	if (unlikely(!cpu_has_virtual_nmis() && vmx->soft_vnmi_blocked))
		vmx->entry_time = ktime_get();

	/* Don't enter VMX if guest state is invalid, let the exit handler
	   start emulation until we arrive back to a valid state */
<<<<<<< HEAD
	if (vmx->emulation_required && emulate_invalid_guest_state)
		return;

=======
	if (vmx->emulation_required)
		return;

	if (vmx->nested.sync_shadow_vmcs) {
		copy_vmcs12_to_shadow(vmx);
		vmx->nested.sync_shadow_vmcs = false;
	}

>>>>>>> refs/remotes/origin/master
	if (test_bit(VCPU_REGS_RSP, (unsigned long *)&vcpu->arch.regs_dirty))
		vmcs_writel(GUEST_RSP, vcpu->arch.regs[VCPU_REGS_RSP]);
	if (test_bit(VCPU_REGS_RIP, (unsigned long *)&vcpu->arch.regs_dirty))
		vmcs_writel(GUEST_RIP, vcpu->arch.regs[VCPU_REGS_RIP]);

	/* When single-stepping over STI and MOV SS, we must clear the
	 * corresponding interruptibility bits in the guest state. Otherwise
	 * vmentry fails as it then expects bit 14 (BS) in pending debug
	 * exceptions being set, but that's not correct for the guest debugging
	 * case. */
	if (vcpu->guest_debug & KVM_GUESTDBG_SINGLESTEP)
		vmx_set_interrupt_shadow(vcpu, 0);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	atomic_switch_perf_msrs(vmx);

	vmx->__launched = vmx->loaded_vmcs->launched;
>>>>>>> refs/remotes/origin/cm-10.0
	asm(
		/* Store host registers */
		"push %%"R"dx; push %%"R"bp;"
		"push %%"R"cx \n\t" /* placeholder for guest rcx */
		"push %%"R"cx \n\t"
		"cmp %%"R"sp, %c[host_rsp](%0) \n\t"
		"je 1f \n\t"
		"mov %%"R"sp, %c[host_rsp](%0) \n\t"
		__ex(ASM_VMX_VMWRITE_RSP_RDX) "\n\t"
		"1: \n\t"
		/* Reload cr2 if changed */
		"mov %c[cr2](%0), %%"R"ax \n\t"
		"mov %%cr2, %%"R"dx \n\t"
		"cmp %%"R"ax, %%"R"dx \n\t"
		"je 2f \n\t"
		"mov %%"R"ax, %%cr2 \n\t"
=======
	atomic_switch_perf_msrs(vmx);
	debugctlmsr = get_debugctlmsr();

	if (is_guest_mode(vcpu) && !vmx->nested.nested_run_pending)
		nested_adjust_preemption_timer(vcpu);
	vmx->__launched = vmx->loaded_vmcs->launched;
	asm(
		/* Store host registers */
		"push %%" _ASM_DX "; push %%" _ASM_BP ";"
		"push %%" _ASM_CX " \n\t" /* placeholder for guest rcx */
		"push %%" _ASM_CX " \n\t"
		"cmp %%" _ASM_SP ", %c[host_rsp](%0) \n\t"
		"je 1f \n\t"
		"mov %%" _ASM_SP ", %c[host_rsp](%0) \n\t"
		__ex(ASM_VMX_VMWRITE_RSP_RDX) "\n\t"
		"1: \n\t"
		/* Reload cr2 if changed */
		"mov %c[cr2](%0), %%" _ASM_AX " \n\t"
		"mov %%cr2, %%" _ASM_DX " \n\t"
		"cmp %%" _ASM_AX ", %%" _ASM_DX " \n\t"
		"je 2f \n\t"
		"mov %%" _ASM_AX", %%cr2 \n\t"
>>>>>>> refs/remotes/origin/master
		"2: \n\t"
		/* Check if vmlaunch of vmresume is needed */
		"cmpl $0, %c[launched](%0) \n\t"
		/* Load guest registers.  Don't clobber flags. */
<<<<<<< HEAD
		"mov %c[rax](%0), %%"R"ax \n\t"
		"mov %c[rbx](%0), %%"R"bx \n\t"
		"mov %c[rdx](%0), %%"R"dx \n\t"
		"mov %c[rsi](%0), %%"R"si \n\t"
		"mov %c[rdi](%0), %%"R"di \n\t"
		"mov %c[rbp](%0), %%"R"bp \n\t"
=======
		"mov %c[rax](%0), %%" _ASM_AX " \n\t"
		"mov %c[rbx](%0), %%" _ASM_BX " \n\t"
		"mov %c[rdx](%0), %%" _ASM_DX " \n\t"
		"mov %c[rsi](%0), %%" _ASM_SI " \n\t"
		"mov %c[rdi](%0), %%" _ASM_DI " \n\t"
		"mov %c[rbp](%0), %%" _ASM_BP " \n\t"
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_X86_64
		"mov %c[r8](%0),  %%r8  \n\t"
		"mov %c[r9](%0),  %%r9  \n\t"
		"mov %c[r10](%0), %%r10 \n\t"
		"mov %c[r11](%0), %%r11 \n\t"
		"mov %c[r12](%0), %%r12 \n\t"
		"mov %c[r13](%0), %%r13 \n\t"
		"mov %c[r14](%0), %%r14 \n\t"
		"mov %c[r15](%0), %%r15 \n\t"
#endif
<<<<<<< HEAD
		"mov %c[rcx](%0), %%"R"cx \n\t" /* kills %0 (ecx) */

		/* Enter guest mode */
		"jne .Llaunched \n\t"
		__ex(ASM_VMX_VMLAUNCH) "\n\t"
		"jmp .Lkvm_vmx_return \n\t"
		".Llaunched: " __ex(ASM_VMX_VMRESUME) "\n\t"
		".Lkvm_vmx_return: "
		/* Save guest registers, load host registers, keep flags */
		"mov %0, %c[wordsize](%%"R"sp) \n\t"
		"pop %0 \n\t"
		"mov %%"R"ax, %c[rax](%0) \n\t"
		"mov %%"R"bx, %c[rbx](%0) \n\t"
		"pop"Q" %c[rcx](%0) \n\t"
		"mov %%"R"dx, %c[rdx](%0) \n\t"
		"mov %%"R"si, %c[rsi](%0) \n\t"
		"mov %%"R"di, %c[rdi](%0) \n\t"
		"mov %%"R"bp, %c[rbp](%0) \n\t"
=======
		"mov %c[rcx](%0), %%" _ASM_CX " \n\t" /* kills %0 (ecx) */

		/* Enter guest mode */
		"jne 1f \n\t"
		__ex(ASM_VMX_VMLAUNCH) "\n\t"
		"jmp 2f \n\t"
		"1: " __ex(ASM_VMX_VMRESUME) "\n\t"
		"2: "
		/* Save guest registers, load host registers, keep flags */
		"mov %0, %c[wordsize](%%" _ASM_SP ") \n\t"
		"pop %0 \n\t"
		"mov %%" _ASM_AX ", %c[rax](%0) \n\t"
		"mov %%" _ASM_BX ", %c[rbx](%0) \n\t"
		__ASM_SIZE(pop) " %c[rcx](%0) \n\t"
		"mov %%" _ASM_DX ", %c[rdx](%0) \n\t"
		"mov %%" _ASM_SI ", %c[rsi](%0) \n\t"
		"mov %%" _ASM_DI ", %c[rdi](%0) \n\t"
		"mov %%" _ASM_BP ", %c[rbp](%0) \n\t"
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_X86_64
		"mov %%r8,  %c[r8](%0) \n\t"
		"mov %%r9,  %c[r9](%0) \n\t"
		"mov %%r10, %c[r10](%0) \n\t"
		"mov %%r11, %c[r11](%0) \n\t"
		"mov %%r12, %c[r12](%0) \n\t"
		"mov %%r13, %c[r13](%0) \n\t"
		"mov %%r14, %c[r14](%0) \n\t"
		"mov %%r15, %c[r15](%0) \n\t"
#endif
<<<<<<< HEAD
		"mov %%cr2, %%"R"ax   \n\t"
		"mov %%"R"ax, %c[cr2](%0) \n\t"

		"pop  %%"R"bp; pop  %%"R"dx \n\t"
		"setbe %c[fail](%0) \n\t"
	      : : "c"(vmx), "d"((unsigned long)HOST_RSP),
<<<<<<< HEAD
		[launched]"i"(offsetof(struct vcpu_vmx, launched)),
=======
		[launched]"i"(offsetof(struct vcpu_vmx, __launched)),
>>>>>>> refs/remotes/origin/cm-10.0
=======
		"mov %%cr2, %%" _ASM_AX "   \n\t"
		"mov %%" _ASM_AX ", %c[cr2](%0) \n\t"

		"pop  %%" _ASM_BP "; pop  %%" _ASM_DX " \n\t"
		"setbe %c[fail](%0) \n\t"
		".pushsection .rodata \n\t"
		".global vmx_return \n\t"
		"vmx_return: " _ASM_PTR " 2b \n\t"
		".popsection"
	      : : "c"(vmx), "d"((unsigned long)HOST_RSP),
		[launched]"i"(offsetof(struct vcpu_vmx, __launched)),
>>>>>>> refs/remotes/origin/master
		[fail]"i"(offsetof(struct vcpu_vmx, fail)),
		[host_rsp]"i"(offsetof(struct vcpu_vmx, host_rsp)),
		[rax]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_RAX])),
		[rbx]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_RBX])),
		[rcx]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_RCX])),
		[rdx]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_RDX])),
		[rsi]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_RSI])),
		[rdi]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_RDI])),
		[rbp]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_RBP])),
#ifdef CONFIG_X86_64
		[r8]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_R8])),
		[r9]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_R9])),
		[r10]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_R10])),
		[r11]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_R11])),
		[r12]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_R12])),
		[r13]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_R13])),
		[r14]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_R14])),
		[r15]"i"(offsetof(struct vcpu_vmx, vcpu.arch.regs[VCPU_REGS_R15])),
#endif
		[cr2]"i"(offsetof(struct vcpu_vmx, vcpu.arch.cr2)),
		[wordsize]"i"(sizeof(ulong))
	      : "cc", "memory"
<<<<<<< HEAD
		, R"ax", R"bx", R"di", R"si"
#ifdef CONFIG_X86_64
		, "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
#endif
	      );

<<<<<<< HEAD
=======
#ifdef CONFIG_X86_64
		, "rax", "rbx", "rdi", "rsi"
		, "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
#else
		, "eax", "ebx", "edi", "esi"
#endif
	      );

	/* MSR_IA32_DEBUGCTLMSR is zeroed on vmexit. Restore it if needed */
	if (debugctlmsr)
		update_debugctlmsr(debugctlmsr);

>>>>>>> refs/remotes/origin/master
#ifndef CONFIG_X86_64
	/*
	 * The sysexit path does not restore ds/es, so we must set them to
	 * a reasonable value ourselves.
	 *
	 * We can't defer this to vmx_load_host_state() since that function
	 * may be executed in interrupt context, which saves and restore segments
	 * around it, nullifying its effect.
	 */
	loadsegment(ds, __USER_DS);
	loadsegment(es, __USER_DS);
#endif

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	vcpu->arch.regs_avail = ~((1 << VCPU_REGS_RIP) | (1 << VCPU_REGS_RSP)
				  | (1 << VCPU_EXREG_RFLAGS)
				  | (1 << VCPU_EXREG_CPL)
				  | (1 << VCPU_EXREG_PDPTR)
				  | (1 << VCPU_EXREG_SEGMENTS)
				  | (1 << VCPU_EXREG_CR3));
	vcpu->arch.regs_dirty = 0;

	vmx->idt_vectoring_info = vmcs_read32(IDT_VECTORING_INFO_FIELD);

<<<<<<< HEAD
<<<<<<< HEAD
	asm("mov %0, %%ds; mov %0, %%es" : : "r"(__USER_DS));
	vmx->launched = 1;

	vmx->exit_reason = vmcs_read32(VM_EXIT_REASON);
=======
	if (is_guest_mode(vcpu)) {
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
		vmcs12->idt_vectoring_info_field = vmx->idt_vectoring_info;
		if (vmx->idt_vectoring_info & VECTORING_INFO_VALID_MASK) {
			vmcs12->idt_vectoring_error_code =
				vmcs_read32(IDT_VECTORING_ERROR_CODE);
			vmcs12->vm_exit_instruction_len =
				vmcs_read32(VM_EXIT_INSTRUCTION_LEN);
		}
	}

	asm("mov %0, %%ds; mov %0, %%es" : : "r"(__USER_DS));
=======
>>>>>>> refs/remotes/origin/master
	vmx->loaded_vmcs->launched = 1;

	vmx->exit_reason = vmcs_read32(VM_EXIT_REASON);
	trace_kvm_exit(vmx->exit_reason, vcpu, KVM_ISA_VMX);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

	/*
	 * the KVM_REQ_EVENT optimization bit is only on for one entry, and if
	 * we did not inject a still-pending event to L1 now because of
	 * nested_run_pending, we need to re-enable this bit.
	 */
	if (vmx->nested.nested_run_pending)
		kvm_make_request(KVM_REQ_EVENT, vcpu);

	vmx->nested.nested_run_pending = 0;
>>>>>>> refs/remotes/origin/master

	vmx_complete_atomic_exit(vmx);
	vmx_recover_nmi_blocking(vmx);
	vmx_complete_interrupts(vmx);
}

<<<<<<< HEAD
#undef R
#undef Q

<<<<<<< HEAD
static void vmx_free_vmcs(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

	if (vmx->vmcs) {
		vcpu_clear(vmx);
		free_vmcs(vmx->vmcs);
		vmx->vmcs = NULL;
	}
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void vmx_free_vcpu(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

	free_vpid(vmx);
<<<<<<< HEAD
<<<<<<< HEAD
	vmx_free_vmcs(vcpu);
=======
	free_nested(vmx);
	free_loaded_vmcs(vmx->loaded_vmcs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	free_nested(vmx);
	free_loaded_vmcs(vmx->loaded_vmcs);
>>>>>>> refs/remotes/origin/master
	kfree(vmx->guest_msrs);
	kvm_vcpu_uninit(vcpu);
	kmem_cache_free(kvm_vcpu_cache, vmx);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void vmcs_init(struct vmcs *vmcs)
{
	u64 phys_addr = __pa(per_cpu(vmxarea, raw_smp_processor_id()));

	if (!vmm_exclusive)
		kvm_cpu_vmxon(phys_addr);

	vmcs_clear(vmcs);

	if (!vmm_exclusive)
		kvm_cpu_vmxoff();
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct kvm_vcpu *vmx_create_vcpu(struct kvm *kvm, unsigned int id)
{
	int err;
	struct vcpu_vmx *vmx = kmem_cache_zalloc(kvm_vcpu_cache, GFP_KERNEL);
	int cpu;

	if (!vmx)
		return ERR_PTR(-ENOMEM);

	allocate_vpid(vmx);

	err = kvm_vcpu_init(&vmx->vcpu, kvm, id);
	if (err)
		goto free_vcpu;

	vmx->guest_msrs = kmalloc(PAGE_SIZE, GFP_KERNEL);
	err = -ENOMEM;
	if (!vmx->guest_msrs) {
		goto uninit_vcpu;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	vmx->vmcs = alloc_vmcs();
	if (!vmx->vmcs)
		goto free_msrs;

	vmcs_init(vmx->vmcs);
=======
=======
>>>>>>> refs/remotes/origin/master
	vmx->loaded_vmcs = &vmx->vmcs01;
	vmx->loaded_vmcs->vmcs = alloc_vmcs();
	if (!vmx->loaded_vmcs->vmcs)
		goto free_msrs;
	if (!vmm_exclusive)
		kvm_cpu_vmxon(__pa(per_cpu(vmxarea, raw_smp_processor_id())));
	loaded_vmcs_init(vmx->loaded_vmcs);
	if (!vmm_exclusive)
		kvm_cpu_vmxoff();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	cpu = get_cpu();
	vmx_vcpu_load(&vmx->vcpu, cpu);
	vmx->vcpu.cpu = cpu;
	err = vmx_vcpu_setup(vmx);
	vmx_vcpu_put(&vmx->vcpu);
	put_cpu();
	if (err)
		goto free_vmcs;
<<<<<<< HEAD
	if (vm_need_virtualize_apic_accesses(kvm))
		err = alloc_apic_access_page(kvm);
		if (err)
			goto free_vmcs;
=======
	if (vm_need_virtualize_apic_accesses(kvm)) {
		err = alloc_apic_access_page(kvm);
		if (err)
			goto free_vmcs;
	}
>>>>>>> refs/remotes/origin/master

	if (enable_ept) {
		if (!kvm->arch.ept_identity_map_addr)
			kvm->arch.ept_identity_map_addr =
				VMX_EPT_IDENTITY_PAGETABLE_ADDR;
		err = -ENOMEM;
		if (alloc_identity_pagetable(kvm) != 0)
			goto free_vmcs;
		if (!init_rmode_identity_map(kvm))
			goto free_vmcs;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	return &vmx->vcpu;

free_vmcs:
	free_vmcs(vmx->vmcs);
=======
=======
>>>>>>> refs/remotes/origin/master
	vmx->nested.current_vmptr = -1ull;
	vmx->nested.current_vmcs12 = NULL;

	return &vmx->vcpu;

free_vmcs:
<<<<<<< HEAD
	free_vmcs(vmx->loaded_vmcs->vmcs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	free_loaded_vmcs(vmx->loaded_vmcs);
>>>>>>> refs/remotes/origin/master
free_msrs:
	kfree(vmx->guest_msrs);
uninit_vcpu:
	kvm_vcpu_uninit(&vmx->vcpu);
free_vcpu:
	free_vpid(vmx);
	kmem_cache_free(kvm_vcpu_cache, vmx);
	return ERR_PTR(err);
}

static void __init vmx_check_processor_compat(void *rtn)
{
	struct vmcs_config vmcs_conf;

	*(int *)rtn = 0;
	if (setup_vmcs_config(&vmcs_conf) < 0)
		*(int *)rtn = -EIO;
	if (memcmp(&vmcs_config, &vmcs_conf, sizeof(struct vmcs_config)) != 0) {
		printk(KERN_ERR "kvm: CPU %d feature inconsistency!\n",
				smp_processor_id());
		*(int *)rtn = -EIO;
	}
}

static int get_ept_level(void)
{
	return VMX_EPT_DEFAULT_GAW + 1;
}

static u64 vmx_get_mt_mask(struct kvm_vcpu *vcpu, gfn_t gfn, bool is_mmio)
{
	u64 ret;

	/* For VT-d and EPT combination
	 * 1. MMIO: always map as UC
	 * 2. EPT with VT-d:
	 *   a. VT-d without snooping control feature: can't guarantee the
	 *	result, try to trust guest.
	 *   b. VT-d with snooping control feature: snooping control feature of
	 *	VT-d engine can guarantee the cache correctness. Just set it
	 *	to WB to keep consistent with host. So the same as item 3.
	 * 3. EPT without VT-d: always map as WB and set IPAT=1 to keep
	 *    consistent with host MTRR
	 */
	if (is_mmio)
		ret = MTRR_TYPE_UNCACHABLE << VMX_EPT_MT_EPTE_SHIFT;
<<<<<<< HEAD
	else if (vcpu->kvm->arch.iommu_domain &&
		!(vcpu->kvm->arch.iommu_flags & KVM_IOMMU_CACHE_COHERENCY))
=======
	else if (kvm_arch_has_noncoherent_dma(vcpu->kvm))
>>>>>>> refs/remotes/origin/master
		ret = kvm_get_guest_memory_type(vcpu, gfn) <<
		      VMX_EPT_MT_EPTE_SHIFT;
	else
		ret = (MTRR_TYPE_WRBACK << VMX_EPT_MT_EPTE_SHIFT)
			| VMX_EPT_IPAT_BIT;

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
#define _ER(x) { EXIT_REASON_##x, #x }

static const struct trace_print_flags vmx_exit_reasons_str[] = {
	_ER(EXCEPTION_NMI),
	_ER(EXTERNAL_INTERRUPT),
	_ER(TRIPLE_FAULT),
	_ER(PENDING_INTERRUPT),
	_ER(NMI_WINDOW),
	_ER(TASK_SWITCH),
	_ER(CPUID),
	_ER(HLT),
	_ER(INVLPG),
	_ER(RDPMC),
	_ER(RDTSC),
	_ER(VMCALL),
	_ER(VMCLEAR),
	_ER(VMLAUNCH),
	_ER(VMPTRLD),
	_ER(VMPTRST),
	_ER(VMREAD),
	_ER(VMRESUME),
	_ER(VMWRITE),
	_ER(VMOFF),
	_ER(VMON),
	_ER(CR_ACCESS),
	_ER(DR_ACCESS),
	_ER(IO_INSTRUCTION),
	_ER(MSR_READ),
	_ER(MSR_WRITE),
	_ER(MWAIT_INSTRUCTION),
	_ER(MONITOR_INSTRUCTION),
	_ER(PAUSE_INSTRUCTION),
	_ER(MCE_DURING_VMENTRY),
	_ER(TPR_BELOW_THRESHOLD),
	_ER(APIC_ACCESS),
	_ER(EPT_VIOLATION),
	_ER(EPT_MISCONFIG),
	_ER(WBINVD),
	{ -1, NULL }
};

#undef _ER

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int vmx_get_lpage_level(void)
{
	if (enable_ept && !cpu_has_vmx_ept_1g_page())
		return PT_DIRECTORY_LEVEL;
	else
		/* For shadow and EPT supported 1GB page */
		return PT_PDPE_LEVEL;
}

static void vmx_cpuid_update(struct kvm_vcpu *vcpu)
{
	struct kvm_cpuid_entry2 *best;
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u32 exec_control;

	vmx->rdtscp_enabled = false;
	if (vmx_rdtscp_supported()) {
		exec_control = vmcs_read32(SECONDARY_VM_EXEC_CONTROL);
		if (exec_control & SECONDARY_EXEC_RDTSCP) {
			best = kvm_find_cpuid_entry(vcpu, 0x80000001, 0);
			if (best && (best->edx & bit(X86_FEATURE_RDTSCP)))
				vmx->rdtscp_enabled = true;
			else {
				exec_control &= ~SECONDARY_EXEC_RDTSCP;
				vmcs_write32(SECONDARY_VM_EXEC_CONTROL,
						exec_control);
			}
		}
	}
<<<<<<< HEAD
=======

	/* Exposing INVPCID only when PCID is exposed */
	best = kvm_find_cpuid_entry(vcpu, 0x7, 0);
	if (vmx_invpcid_supported() &&
	    best && (best->ebx & bit(X86_FEATURE_INVPCID)) &&
	    guest_cpuid_has_pcid(vcpu)) {
		exec_control = vmcs_read32(SECONDARY_VM_EXEC_CONTROL);
		exec_control |= SECONDARY_EXEC_ENABLE_INVPCID;
		vmcs_write32(SECONDARY_VM_EXEC_CONTROL,
			     exec_control);
	} else {
		if (cpu_has_secondary_exec_ctrls()) {
			exec_control = vmcs_read32(SECONDARY_VM_EXEC_CONTROL);
			exec_control &= ~SECONDARY_EXEC_ENABLE_INVPCID;
			vmcs_write32(SECONDARY_VM_EXEC_CONTROL,
				     exec_control);
		}
		if (best)
			best->ebx &= ~bit(X86_FEATURE_INVPCID);
	}
>>>>>>> refs/remotes/origin/master
}

static void vmx_set_supported_cpuid(u32 func, struct kvm_cpuid_entry2 *entry)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (func == 1 && nested)
		entry->ecx |= bit(X86_FEATURE_VMX);
}

<<<<<<< HEAD
=======
static void nested_ept_inject_page_fault(struct kvm_vcpu *vcpu,
		struct x86_exception *fault)
{
	struct vmcs12 *vmcs12;
	nested_vmx_vmexit(vcpu);
	vmcs12 = get_vmcs12(vcpu);

	if (fault->error_code & PFERR_RSVD_MASK)
		vmcs12->vm_exit_reason = EXIT_REASON_EPT_MISCONFIG;
	else
		vmcs12->vm_exit_reason = EXIT_REASON_EPT_VIOLATION;
	vmcs12->exit_qualification = vcpu->arch.exit_qualification;
	vmcs12->guest_physical_address = fault->address;
}

/* Callbacks for nested_ept_init_mmu_context: */

static unsigned long nested_ept_get_cr3(struct kvm_vcpu *vcpu)
{
	/* return the page table to be shadowed - in our case, EPT12 */
	return get_vmcs12(vcpu)->ept_pointer;
}

static void nested_ept_init_mmu_context(struct kvm_vcpu *vcpu)
{
	kvm_init_shadow_ept_mmu(vcpu, &vcpu->arch.mmu,
			nested_vmx_ept_caps & VMX_EPT_EXECUTE_ONLY_BIT);

	vcpu->arch.mmu.set_cr3           = vmx_set_cr3;
	vcpu->arch.mmu.get_cr3           = nested_ept_get_cr3;
	vcpu->arch.mmu.inject_page_fault = nested_ept_inject_page_fault;

	vcpu->arch.walk_mmu              = &vcpu->arch.nested_mmu;
}

static void nested_ept_uninit_mmu_context(struct kvm_vcpu *vcpu)
{
	vcpu->arch.walk_mmu = &vcpu->arch.mmu;
}

static void vmx_inject_page_fault_nested(struct kvm_vcpu *vcpu,
		struct x86_exception *fault)
{
	struct vmcs12 *vmcs12 = get_vmcs12(vcpu);

	WARN_ON(!is_guest_mode(vcpu));

	/* TODO: also check PFEC_MATCH/MASK, not just EB.PF. */
	if (vmcs12->exception_bitmap & (1u << PF_VECTOR))
		nested_vmx_vmexit(vcpu);
	else
		kvm_inject_page_fault(vcpu, fault);
}

>>>>>>> refs/remotes/origin/master
/*
 * prepare_vmcs02 is called when the L1 guest hypervisor runs its nested
 * L2 guest. L1 has a vmcs for L2 (vmcs12), and this function "merges" it
 * with L0's requirements for its guest (a.k.a. vmsc01), so we can run the L2
 * guest in a way that will both be appropriate to L1's requests, and our
 * needs. In addition to modifying the active vmcs (which is vmcs02), this
 * function also has additional necessary side-effects, like setting various
 * vcpu->arch fields.
 */
static void prepare_vmcs02(struct kvm_vcpu *vcpu, struct vmcs12 *vmcs12)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u32 exec_control;
<<<<<<< HEAD
=======
	u32 exit_control;
>>>>>>> refs/remotes/origin/master

	vmcs_write16(GUEST_ES_SELECTOR, vmcs12->guest_es_selector);
	vmcs_write16(GUEST_CS_SELECTOR, vmcs12->guest_cs_selector);
	vmcs_write16(GUEST_SS_SELECTOR, vmcs12->guest_ss_selector);
	vmcs_write16(GUEST_DS_SELECTOR, vmcs12->guest_ds_selector);
	vmcs_write16(GUEST_FS_SELECTOR, vmcs12->guest_fs_selector);
	vmcs_write16(GUEST_GS_SELECTOR, vmcs12->guest_gs_selector);
	vmcs_write16(GUEST_LDTR_SELECTOR, vmcs12->guest_ldtr_selector);
	vmcs_write16(GUEST_TR_SELECTOR, vmcs12->guest_tr_selector);
	vmcs_write32(GUEST_ES_LIMIT, vmcs12->guest_es_limit);
	vmcs_write32(GUEST_CS_LIMIT, vmcs12->guest_cs_limit);
	vmcs_write32(GUEST_SS_LIMIT, vmcs12->guest_ss_limit);
	vmcs_write32(GUEST_DS_LIMIT, vmcs12->guest_ds_limit);
	vmcs_write32(GUEST_FS_LIMIT, vmcs12->guest_fs_limit);
	vmcs_write32(GUEST_GS_LIMIT, vmcs12->guest_gs_limit);
	vmcs_write32(GUEST_LDTR_LIMIT, vmcs12->guest_ldtr_limit);
	vmcs_write32(GUEST_TR_LIMIT, vmcs12->guest_tr_limit);
	vmcs_write32(GUEST_GDTR_LIMIT, vmcs12->guest_gdtr_limit);
	vmcs_write32(GUEST_IDTR_LIMIT, vmcs12->guest_idtr_limit);
	vmcs_write32(GUEST_ES_AR_BYTES, vmcs12->guest_es_ar_bytes);
	vmcs_write32(GUEST_CS_AR_BYTES, vmcs12->guest_cs_ar_bytes);
	vmcs_write32(GUEST_SS_AR_BYTES, vmcs12->guest_ss_ar_bytes);
	vmcs_write32(GUEST_DS_AR_BYTES, vmcs12->guest_ds_ar_bytes);
	vmcs_write32(GUEST_FS_AR_BYTES, vmcs12->guest_fs_ar_bytes);
	vmcs_write32(GUEST_GS_AR_BYTES, vmcs12->guest_gs_ar_bytes);
	vmcs_write32(GUEST_LDTR_AR_BYTES, vmcs12->guest_ldtr_ar_bytes);
	vmcs_write32(GUEST_TR_AR_BYTES, vmcs12->guest_tr_ar_bytes);
	vmcs_writel(GUEST_ES_BASE, vmcs12->guest_es_base);
	vmcs_writel(GUEST_CS_BASE, vmcs12->guest_cs_base);
	vmcs_writel(GUEST_SS_BASE, vmcs12->guest_ss_base);
	vmcs_writel(GUEST_DS_BASE, vmcs12->guest_ds_base);
	vmcs_writel(GUEST_FS_BASE, vmcs12->guest_fs_base);
	vmcs_writel(GUEST_GS_BASE, vmcs12->guest_gs_base);
	vmcs_writel(GUEST_LDTR_BASE, vmcs12->guest_ldtr_base);
	vmcs_writel(GUEST_TR_BASE, vmcs12->guest_tr_base);
	vmcs_writel(GUEST_GDTR_BASE, vmcs12->guest_gdtr_base);
	vmcs_writel(GUEST_IDTR_BASE, vmcs12->guest_idtr_base);

	vmcs_write64(GUEST_IA32_DEBUGCTL, vmcs12->guest_ia32_debugctl);
	vmcs_write32(VM_ENTRY_INTR_INFO_FIELD,
		vmcs12->vm_entry_intr_info_field);
	vmcs_write32(VM_ENTRY_EXCEPTION_ERROR_CODE,
		vmcs12->vm_entry_exception_error_code);
	vmcs_write32(VM_ENTRY_INSTRUCTION_LEN,
		vmcs12->vm_entry_instruction_len);
	vmcs_write32(GUEST_INTERRUPTIBILITY_INFO,
		vmcs12->guest_interruptibility_info);
<<<<<<< HEAD
	vmcs_write32(GUEST_ACTIVITY_STATE, vmcs12->guest_activity_state);
	vmcs_write32(GUEST_SYSENTER_CS, vmcs12->guest_sysenter_cs);
	vmcs_writel(GUEST_DR7, vmcs12->guest_dr7);
	vmcs_writel(GUEST_RFLAGS, vmcs12->guest_rflags);
=======
	vmcs_write32(GUEST_SYSENTER_CS, vmcs12->guest_sysenter_cs);
	kvm_set_dr(vcpu, 7, vmcs12->guest_dr7);
	vmx_set_rflags(vcpu, vmcs12->guest_rflags);
>>>>>>> refs/remotes/origin/master
	vmcs_writel(GUEST_PENDING_DBG_EXCEPTIONS,
		vmcs12->guest_pending_dbg_exceptions);
	vmcs_writel(GUEST_SYSENTER_ESP, vmcs12->guest_sysenter_esp);
	vmcs_writel(GUEST_SYSENTER_EIP, vmcs12->guest_sysenter_eip);

	vmcs_write64(VMCS_LINK_POINTER, -1ull);

	vmcs_write32(PIN_BASED_VM_EXEC_CONTROL,
		(vmcs_config.pin_based_exec_ctrl |
		 vmcs12->pin_based_vm_exec_control));

<<<<<<< HEAD
=======
	if (vmcs12->pin_based_vm_exec_control & PIN_BASED_VMX_PREEMPTION_TIMER)
		vmcs_write32(VMX_PREEMPTION_TIMER_VALUE,
			     vmcs12->vmx_preemption_timer_value);

>>>>>>> refs/remotes/origin/master
	/*
	 * Whether page-faults are trapped is determined by a combination of
	 * 3 settings: PFEC_MASK, PFEC_MATCH and EXCEPTION_BITMAP.PF.
	 * If enable_ept, L0 doesn't care about page faults and we should
	 * set all of these to L1's desires. However, if !enable_ept, L0 does
	 * care about (at least some) page faults, and because it is not easy
	 * (if at all possible?) to merge L0 and L1's desires, we simply ask
	 * to exit on each and every L2 page fault. This is done by setting
	 * MASK=MATCH=0 and (see below) EB.PF=1.
	 * Note that below we don't need special code to set EB.PF beyond the
	 * "or"ing of the EB of vmcs01 and vmcs12, because when enable_ept,
	 * vmcs01's EB.PF is 0 so the "or" will take vmcs12's value, and when
	 * !enable_ept, EB.PF is 1, so the "or" will always be 1.
	 *
	 * A problem with this approach (when !enable_ept) is that L1 may be
	 * injected with more page faults than it asked for. This could have
	 * caused problems, but in practice existing hypervisors don't care.
	 * To fix this, we will need to emulate the PFEC checking (on the L1
	 * page tables), using walk_addr(), when injecting PFs to L1.
	 */
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MASK,
		enable_ept ? vmcs12->page_fault_error_code_mask : 0);
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MATCH,
		enable_ept ? vmcs12->page_fault_error_code_match : 0);

	if (cpu_has_secondary_exec_ctrls()) {
		u32 exec_control = vmx_secondary_exec_control(vmx);
		if (!vmx->rdtscp_enabled)
			exec_control &= ~SECONDARY_EXEC_RDTSCP;
		/* Take the following fields only from vmcs12 */
		exec_control &= ~SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
		if (nested_cpu_has(vmcs12,
				CPU_BASED_ACTIVATE_SECONDARY_CONTROLS))
			exec_control |= vmcs12->secondary_vm_exec_control;

		if (exec_control & SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES) {
			/*
			 * Translate L1 physical address to host physical
			 * address for vmcs02. Keep the page pinned, so this
			 * physical address remains valid. We keep a reference
			 * to it so we can release it later.
			 */
			if (vmx->nested.apic_access_page) /* shouldn't happen */
				nested_release_page(vmx->nested.apic_access_page);
			vmx->nested.apic_access_page =
				nested_get_page(vcpu, vmcs12->apic_access_addr);
			/*
			 * If translation failed, no matter: This feature asks
			 * to exit when accessing the given address, and if it
			 * can never be accessed, this feature won't do
			 * anything anyway.
			 */
			if (!vmx->nested.apic_access_page)
				exec_control &=
				  ~SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
			else
				vmcs_write64(APIC_ACCESS_ADDR,
				  page_to_phys(vmx->nested.apic_access_page));
		}

		vmcs_write32(SECONDARY_VM_EXEC_CONTROL, exec_control);
	}


	/*
	 * Set host-state according to L0's settings (vmcs12 is irrelevant here)
	 * Some constant fields are set here by vmx_set_constant_host_state().
	 * Other fields are different per CPU, and will be set later when
	 * vmx_vcpu_load() is called, and when vmx_save_host_state() is called.
	 */
<<<<<<< HEAD
	vmx_set_constant_host_state();
=======
	vmx_set_constant_host_state(vmx);
>>>>>>> refs/remotes/origin/master

	/*
	 * HOST_RSP is normally set correctly in vmx_vcpu_run() just before
	 * entry, but only if the current (host) sp changed from the value
	 * we wrote last (vmx->host_rsp). This cache is no longer relevant
	 * if we switch vmcs, and rather than hold a separate cache per vmcs,
	 * here we just force the write to happen on entry.
	 */
	vmx->host_rsp = 0;

	exec_control = vmx_exec_control(vmx); /* L0's desires */
	exec_control &= ~CPU_BASED_VIRTUAL_INTR_PENDING;
	exec_control &= ~CPU_BASED_VIRTUAL_NMI_PENDING;
	exec_control &= ~CPU_BASED_TPR_SHADOW;
	exec_control |= vmcs12->cpu_based_vm_exec_control;
	/*
	 * Merging of IO and MSR bitmaps not currently supported.
	 * Rather, exit every time.
	 */
	exec_control &= ~CPU_BASED_USE_MSR_BITMAPS;
	exec_control &= ~CPU_BASED_USE_IO_BITMAPS;
	exec_control |= CPU_BASED_UNCOND_IO_EXITING;

	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, exec_control);

	/* EXCEPTION_BITMAP and CR0_GUEST_HOST_MASK should basically be the
	 * bitwise-or of what L1 wants to trap for L2, and what we want to
	 * trap. Note that CR0.TS also needs updating - we do this later.
	 */
	update_exception_bitmap(vcpu);
	vcpu->arch.cr0_guest_owned_bits &= ~vmcs12->cr0_guest_host_mask;
	vmcs_writel(CR0_GUEST_HOST_MASK, ~vcpu->arch.cr0_guest_owned_bits);

<<<<<<< HEAD
	/* Note: IA32_MODE, LOAD_IA32_EFER are modified by vmx_set_efer below */
	vmcs_write32(VM_EXIT_CONTROLS,
		vmcs12->vm_exit_controls | vmcs_config.vmexit_ctrl);
	vmcs_write32(VM_ENTRY_CONTROLS, vmcs12->vm_entry_controls |
		(vmcs_config.vmentry_ctrl & ~VM_ENTRY_IA32E_MODE));

	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_IA32_PAT)
		vmcs_write64(GUEST_IA32_PAT, vmcs12->guest_ia32_pat);
	else if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT)
=======
	/* L2->L1 exit controls are emulated - the hardware exit is to L0 so
	 * we should use its exit controls. Note that VM_EXIT_LOAD_IA32_EFER
	 * bits are further modified by vmx_set_efer() below.
	 */
	exit_control = vmcs_config.vmexit_ctrl;
	if (vmcs12->pin_based_vm_exec_control & PIN_BASED_VMX_PREEMPTION_TIMER)
		exit_control |= VM_EXIT_SAVE_VMX_PREEMPTION_TIMER;
	vmcs_write32(VM_EXIT_CONTROLS, exit_control);

	/* vmcs12's VM_ENTRY_LOAD_IA32_EFER and VM_ENTRY_IA32E_MODE are
	 * emulated by vmx_set_efer(), below.
	 */
	vmcs_write32(VM_ENTRY_CONTROLS,
		(vmcs12->vm_entry_controls & ~VM_ENTRY_LOAD_IA32_EFER &
			~VM_ENTRY_IA32E_MODE) |
		(vmcs_config.vmentry_ctrl & ~VM_ENTRY_IA32E_MODE));

	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_IA32_PAT) {
		vmcs_write64(GUEST_IA32_PAT, vmcs12->guest_ia32_pat);
		vcpu->arch.pat = vmcs12->guest_ia32_pat;
	} else if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT)
>>>>>>> refs/remotes/origin/master
		vmcs_write64(GUEST_IA32_PAT, vmx->vcpu.arch.pat);


	set_cr4_guest_host_mask(vmx);

	if (vmcs12->cpu_based_vm_exec_control & CPU_BASED_USE_TSC_OFFSETING)
		vmcs_write64(TSC_OFFSET,
			vmx->nested.vmcs01_tsc_offset + vmcs12->tsc_offset);
	else
		vmcs_write64(TSC_OFFSET, vmx->nested.vmcs01_tsc_offset);

	if (enable_vpid) {
		/*
		 * Trivially support vpid by letting L2s share their parent
		 * L1's vpid. TODO: move to a more elaborate solution, giving
		 * each L2 its own vpid and exposing the vpid feature to L1.
		 */
		vmcs_write16(VIRTUAL_PROCESSOR_ID, vmx->vpid);
		vmx_flush_tlb(vcpu);
	}

<<<<<<< HEAD
	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_IA32_EFER)
		vcpu->arch.efer = vmcs12->guest_ia32_efer;
	if (vmcs12->vm_entry_controls & VM_ENTRY_IA32E_MODE)
=======
	if (nested_cpu_has_ept(vmcs12)) {
		kvm_mmu_unload(vcpu);
		nested_ept_init_mmu_context(vcpu);
	}

	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_IA32_EFER)
		vcpu->arch.efer = vmcs12->guest_ia32_efer;
	else if (vmcs12->vm_entry_controls & VM_ENTRY_IA32E_MODE)
>>>>>>> refs/remotes/origin/master
		vcpu->arch.efer |= (EFER_LMA | EFER_LME);
	else
		vcpu->arch.efer &= ~(EFER_LMA | EFER_LME);
	/* Note: modifies VM_ENTRY/EXIT_CONTROLS and GUEST/HOST_IA32_EFER */
	vmx_set_efer(vcpu, vcpu->arch.efer);

	/*
	 * This sets GUEST_CR0 to vmcs12->guest_cr0, with possibly a modified
	 * TS bit (for lazy fpu) and bits which we consider mandatory enabled.
	 * The CR0_READ_SHADOW is what L2 should have expected to read given
	 * the specifications by L1; It's not enough to take
	 * vmcs12->cr0_read_shadow because on our cr0_guest_host_mask we we
	 * have more bits than L1 expected.
	 */
	vmx_set_cr0(vcpu, vmcs12->guest_cr0);
	vmcs_writel(CR0_READ_SHADOW, nested_read_cr0(vmcs12));

	vmx_set_cr4(vcpu, vmcs12->guest_cr4);
	vmcs_writel(CR4_READ_SHADOW, nested_read_cr4(vmcs12));

	/* shadow page tables on either EPT or shadow page tables */
	kvm_set_cr3(vcpu, vmcs12->guest_cr3);
	kvm_mmu_reset_context(vcpu);

<<<<<<< HEAD
=======
	if (!enable_ept)
		vcpu->arch.walk_mmu->inject_page_fault = vmx_inject_page_fault_nested;

	/*
	 * L1 may access the L2's PDPTR, so save them to construct vmcs12
	 */
	if (enable_ept) {
		vmcs_write64(GUEST_PDPTR0, vmcs12->guest_pdptr0);
		vmcs_write64(GUEST_PDPTR1, vmcs12->guest_pdptr1);
		vmcs_write64(GUEST_PDPTR2, vmcs12->guest_pdptr2);
		vmcs_write64(GUEST_PDPTR3, vmcs12->guest_pdptr3);
	}

>>>>>>> refs/remotes/origin/master
	kvm_register_write(vcpu, VCPU_REGS_RSP, vmcs12->guest_rsp);
	kvm_register_write(vcpu, VCPU_REGS_RIP, vmcs12->guest_rip);
}

/*
 * nested_vmx_run() handles a nested entry, i.e., a VMLAUNCH or VMRESUME on L1
 * for running an L2 nested guest.
 */
static int nested_vmx_run(struct kvm_vcpu *vcpu, bool launch)
{
	struct vmcs12 *vmcs12;
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	int cpu;
	struct loaded_vmcs *vmcs02;
<<<<<<< HEAD
=======
	bool ia32e;
>>>>>>> refs/remotes/origin/master

	if (!nested_vmx_check_permission(vcpu) ||
	    !nested_vmx_check_vmcs12(vcpu))
		return 1;

	skip_emulated_instruction(vcpu);
	vmcs12 = get_vmcs12(vcpu);

<<<<<<< HEAD
=======
	if (enable_shadow_vmcs)
		copy_shadow_to_vmcs12(vmx);

>>>>>>> refs/remotes/origin/master
	/*
	 * The nested entry process starts with enforcing various prerequisites
	 * on vmcs12 as required by the Intel SDM, and act appropriately when
	 * they fail: As the SDM explains, some conditions should cause the
	 * instruction to fail, while others will cause the instruction to seem
	 * to succeed, but return an EXIT_REASON_INVALID_STATE.
	 * To speed up the normal (success) code path, we should avoid checking
	 * for misconfigurations which will anyway be caught by the processor
	 * when using the merged vmcs02.
	 */
	if (vmcs12->launch_state == launch) {
		nested_vmx_failValid(vcpu,
			launch ? VMXERR_VMLAUNCH_NONCLEAR_VMCS
			       : VMXERR_VMRESUME_NONLAUNCHED_VMCS);
		return 1;
	}

<<<<<<< HEAD
=======
	if (vmcs12->guest_activity_state != GUEST_ACTIVITY_ACTIVE) {
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

>>>>>>> refs/remotes/origin/master
	if ((vmcs12->cpu_based_vm_exec_control & CPU_BASED_USE_MSR_BITMAPS) &&
			!IS_ALIGNED(vmcs12->msr_bitmap, PAGE_SIZE)) {
		/*TODO: Also verify bits beyond physical address width are 0*/
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (nested_cpu_has2(vmcs12, SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES) &&
			!IS_ALIGNED(vmcs12->apic_access_addr, PAGE_SIZE)) {
		/*TODO: Also verify bits beyond physical address width are 0*/
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (vmcs12->vm_entry_msr_load_count > 0 ||
	    vmcs12->vm_exit_msr_load_count > 0 ||
	    vmcs12->vm_exit_msr_store_count > 0) {
		pr_warn_ratelimited("%s: VMCS MSR_{LOAD,STORE} unsupported\n",
				    __func__);
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (!vmx_control_verify(vmcs12->cpu_based_vm_exec_control,
	      nested_vmx_procbased_ctls_low, nested_vmx_procbased_ctls_high) ||
	    !vmx_control_verify(vmcs12->secondary_vm_exec_control,
	      nested_vmx_secondary_ctls_low, nested_vmx_secondary_ctls_high) ||
	    !vmx_control_verify(vmcs12->pin_based_vm_exec_control,
	      nested_vmx_pinbased_ctls_low, nested_vmx_pinbased_ctls_high) ||
	    !vmx_control_verify(vmcs12->vm_exit_controls,
	      nested_vmx_exit_ctls_low, nested_vmx_exit_ctls_high) ||
	    !vmx_control_verify(vmcs12->vm_entry_controls,
	      nested_vmx_entry_ctls_low, nested_vmx_entry_ctls_high))
	{
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (((vmcs12->host_cr0 & VMXON_CR0_ALWAYSON) != VMXON_CR0_ALWAYSON) ||
	    ((vmcs12->host_cr4 & VMXON_CR4_ALWAYSON) != VMXON_CR4_ALWAYSON)) {
		nested_vmx_failValid(vcpu,
			VMXERR_ENTRY_INVALID_HOST_STATE_FIELD);
		return 1;
	}

<<<<<<< HEAD
	if (((vmcs12->guest_cr0 & VMXON_CR0_ALWAYSON) != VMXON_CR0_ALWAYSON) ||
=======
	if (!nested_cr0_valid(vmcs12, vmcs12->guest_cr0) ||
>>>>>>> refs/remotes/origin/master
	    ((vmcs12->guest_cr4 & VMXON_CR4_ALWAYSON) != VMXON_CR4_ALWAYSON)) {
		nested_vmx_entry_failure(vcpu, vmcs12,
			EXIT_REASON_INVALID_STATE, ENTRY_FAIL_DEFAULT);
		return 1;
	}
	if (vmcs12->vmcs_link_pointer != -1ull) {
		nested_vmx_entry_failure(vcpu, vmcs12,
			EXIT_REASON_INVALID_STATE, ENTRY_FAIL_VMCS_LINK_PTR);
		return 1;
	}

	/*
<<<<<<< HEAD
=======
	 * If the load IA32_EFER VM-entry control is 1, the following checks
	 * are performed on the field for the IA32_EFER MSR:
	 * - Bits reserved in the IA32_EFER MSR must be 0.
	 * - Bit 10 (corresponding to IA32_EFER.LMA) must equal the value of
	 *   the IA-32e mode guest VM-exit control. It must also be identical
	 *   to bit 8 (LME) if bit 31 in the CR0 field (corresponding to
	 *   CR0.PG) is 1.
	 */
	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_IA32_EFER) {
		ia32e = (vmcs12->vm_entry_controls & VM_ENTRY_IA32E_MODE) != 0;
		if (!kvm_valid_efer(vcpu, vmcs12->guest_ia32_efer) ||
		    ia32e != !!(vmcs12->guest_ia32_efer & EFER_LMA) ||
		    ((vmcs12->guest_cr0 & X86_CR0_PG) &&
		     ia32e != !!(vmcs12->guest_ia32_efer & EFER_LME))) {
			nested_vmx_entry_failure(vcpu, vmcs12,
				EXIT_REASON_INVALID_STATE, ENTRY_FAIL_DEFAULT);
			return 1;
		}
	}

	/*
	 * If the load IA32_EFER VM-exit control is 1, bits reserved in the
	 * IA32_EFER MSR must be 0 in the field for that register. In addition,
	 * the values of the LMA and LME bits in the field must each be that of
	 * the host address-space size VM-exit control.
	 */
	if (vmcs12->vm_exit_controls & VM_EXIT_LOAD_IA32_EFER) {
		ia32e = (vmcs12->vm_exit_controls &
			 VM_EXIT_HOST_ADDR_SPACE_SIZE) != 0;
		if (!kvm_valid_efer(vcpu, vmcs12->host_ia32_efer) ||
		    ia32e != !!(vmcs12->host_ia32_efer & EFER_LMA) ||
		    ia32e != !!(vmcs12->host_ia32_efer & EFER_LME)) {
			nested_vmx_entry_failure(vcpu, vmcs12,
				EXIT_REASON_INVALID_STATE, ENTRY_FAIL_DEFAULT);
			return 1;
		}
	}

	/*
>>>>>>> refs/remotes/origin/master
	 * We're finally done with prerequisite checking, and can start with
	 * the nested entry.
	 */

	vmcs02 = nested_get_current_vmcs02(vmx);
	if (!vmcs02)
		return -ENOMEM;

	enter_guest_mode(vcpu);

<<<<<<< HEAD
=======
	vmx->nested.nested_run_pending = 1;

>>>>>>> refs/remotes/origin/master
	vmx->nested.vmcs01_tsc_offset = vmcs_read64(TSC_OFFSET);

	cpu = get_cpu();
	vmx->loaded_vmcs = vmcs02;
	vmx_vcpu_put(vcpu);
	vmx_vcpu_load(vcpu, cpu);
	vcpu->cpu = cpu;
	put_cpu();

<<<<<<< HEAD
=======
	vmx_segment_cache_clear(vmx);

>>>>>>> refs/remotes/origin/master
	vmcs12->launch_state = 1;

	prepare_vmcs02(vcpu, vmcs12);

	/*
	 * Note no nested_vmx_succeed or nested_vmx_fail here. At this point
	 * we are no longer running L1, and VMLAUNCH/VMRESUME has not yet
	 * returned as far as L1 is concerned. It will only return (and set
	 * the success flag) when L2 exits (see nested_vmx_vmexit()).
	 */
	return 1;
}

/*
 * On a nested exit from L2 to L1, vmcs12.guest_cr0 might not be up-to-date
 * because L2 may have changed some cr0 bits directly (CRO_GUEST_HOST_MASK).
 * This function returns the new value we should put in vmcs12.guest_cr0.
 * It's not enough to just return the vmcs02 GUEST_CR0. Rather,
 *  1. Bits that neither L0 nor L1 trapped, were set directly by L2 and are now
 *     available in vmcs02 GUEST_CR0. (Note: It's enough to check that L0
 *     didn't trap the bit, because if L1 did, so would L0).
 *  2. Bits that L1 asked to trap (and therefore L0 also did) could not have
 *     been modified by L2, and L1 knows it. So just leave the old value of
 *     the bit from vmcs12.guest_cr0. Note that the bit from vmcs02 GUEST_CR0
 *     isn't relevant, because if L0 traps this bit it can set it to anything.
 *  3. Bits that L1 didn't trap, but L0 did. L1 believes the guest could have
 *     changed these bits, and therefore they need to be updated, but L0
 *     didn't necessarily allow them to be changed in GUEST_CR0 - and rather
 *     put them in vmcs02 CR0_READ_SHADOW. So take these bits from there.
 */
static inline unsigned long
vmcs12_guest_cr0(struct kvm_vcpu *vcpu, struct vmcs12 *vmcs12)
{
	return
	/*1*/	(vmcs_readl(GUEST_CR0) & vcpu->arch.cr0_guest_owned_bits) |
	/*2*/	(vmcs12->guest_cr0 & vmcs12->cr0_guest_host_mask) |
	/*3*/	(vmcs_readl(CR0_READ_SHADOW) & ~(vmcs12->cr0_guest_host_mask |
			vcpu->arch.cr0_guest_owned_bits));
}

static inline unsigned long
vmcs12_guest_cr4(struct kvm_vcpu *vcpu, struct vmcs12 *vmcs12)
{
	return
	/*1*/	(vmcs_readl(GUEST_CR4) & vcpu->arch.cr4_guest_owned_bits) |
	/*2*/	(vmcs12->guest_cr4 & vmcs12->cr4_guest_host_mask) |
	/*3*/	(vmcs_readl(CR4_READ_SHADOW) & ~(vmcs12->cr4_guest_host_mask |
			vcpu->arch.cr4_guest_owned_bits));
}

<<<<<<< HEAD
=======
static void vmcs12_save_pending_event(struct kvm_vcpu *vcpu,
				       struct vmcs12 *vmcs12)
{
	u32 idt_vectoring;
	unsigned int nr;

	if (vcpu->arch.exception.pending && vcpu->arch.exception.reinject) {
		nr = vcpu->arch.exception.nr;
		idt_vectoring = nr | VECTORING_INFO_VALID_MASK;

		if (kvm_exception_is_soft(nr)) {
			vmcs12->vm_exit_instruction_len =
				vcpu->arch.event_exit_inst_len;
			idt_vectoring |= INTR_TYPE_SOFT_EXCEPTION;
		} else
			idt_vectoring |= INTR_TYPE_HARD_EXCEPTION;

		if (vcpu->arch.exception.has_error_code) {
			idt_vectoring |= VECTORING_INFO_DELIVER_CODE_MASK;
			vmcs12->idt_vectoring_error_code =
				vcpu->arch.exception.error_code;
		}

		vmcs12->idt_vectoring_info_field = idt_vectoring;
	} else if (vcpu->arch.nmi_injected) {
		vmcs12->idt_vectoring_info_field =
			INTR_TYPE_NMI_INTR | INTR_INFO_VALID_MASK | NMI_VECTOR;
	} else if (vcpu->arch.interrupt.pending) {
		nr = vcpu->arch.interrupt.nr;
		idt_vectoring = nr | VECTORING_INFO_VALID_MASK;

		if (vcpu->arch.interrupt.soft) {
			idt_vectoring |= INTR_TYPE_SOFT_INTR;
			vmcs12->vm_entry_instruction_len =
				vcpu->arch.event_exit_inst_len;
		} else
			idt_vectoring |= INTR_TYPE_EXT_INTR;

		vmcs12->idt_vectoring_info_field = idt_vectoring;
	}
}

>>>>>>> refs/remotes/origin/master
/*
 * prepare_vmcs12 is part of what we need to do when the nested L2 guest exits
 * and we want to prepare to run its L1 parent. L1 keeps a vmcs for L2 (vmcs12),
 * and this function updates it to reflect the changes to the guest state while
 * L2 was running (and perhaps made some exits which were handled directly by L0
 * without going back to L1), and to reflect the exit reason.
 * Note that we do not have to copy here all VMCS fields, just those that
 * could have changed by the L2 guest or the exit - i.e., the guest-state and
 * exit-information fields only. Other fields are modified by L1 with VMWRITE,
 * which already writes to vmcs12 directly.
 */
<<<<<<< HEAD
void prepare_vmcs12(struct kvm_vcpu *vcpu, struct vmcs12 *vmcs12)
=======
static void prepare_vmcs12(struct kvm_vcpu *vcpu, struct vmcs12 *vmcs12)
>>>>>>> refs/remotes/origin/master
{
	/* update guest state fields: */
	vmcs12->guest_cr0 = vmcs12_guest_cr0(vcpu, vmcs12);
	vmcs12->guest_cr4 = vmcs12_guest_cr4(vcpu, vmcs12);

	kvm_get_dr(vcpu, 7, (unsigned long *)&vmcs12->guest_dr7);
	vmcs12->guest_rsp = kvm_register_read(vcpu, VCPU_REGS_RSP);
	vmcs12->guest_rip = kvm_register_read(vcpu, VCPU_REGS_RIP);
	vmcs12->guest_rflags = vmcs_readl(GUEST_RFLAGS);

	vmcs12->guest_es_selector = vmcs_read16(GUEST_ES_SELECTOR);
	vmcs12->guest_cs_selector = vmcs_read16(GUEST_CS_SELECTOR);
	vmcs12->guest_ss_selector = vmcs_read16(GUEST_SS_SELECTOR);
	vmcs12->guest_ds_selector = vmcs_read16(GUEST_DS_SELECTOR);
	vmcs12->guest_fs_selector = vmcs_read16(GUEST_FS_SELECTOR);
	vmcs12->guest_gs_selector = vmcs_read16(GUEST_GS_SELECTOR);
	vmcs12->guest_ldtr_selector = vmcs_read16(GUEST_LDTR_SELECTOR);
	vmcs12->guest_tr_selector = vmcs_read16(GUEST_TR_SELECTOR);
	vmcs12->guest_es_limit = vmcs_read32(GUEST_ES_LIMIT);
	vmcs12->guest_cs_limit = vmcs_read32(GUEST_CS_LIMIT);
	vmcs12->guest_ss_limit = vmcs_read32(GUEST_SS_LIMIT);
	vmcs12->guest_ds_limit = vmcs_read32(GUEST_DS_LIMIT);
	vmcs12->guest_fs_limit = vmcs_read32(GUEST_FS_LIMIT);
	vmcs12->guest_gs_limit = vmcs_read32(GUEST_GS_LIMIT);
	vmcs12->guest_ldtr_limit = vmcs_read32(GUEST_LDTR_LIMIT);
	vmcs12->guest_tr_limit = vmcs_read32(GUEST_TR_LIMIT);
	vmcs12->guest_gdtr_limit = vmcs_read32(GUEST_GDTR_LIMIT);
	vmcs12->guest_idtr_limit = vmcs_read32(GUEST_IDTR_LIMIT);
	vmcs12->guest_es_ar_bytes = vmcs_read32(GUEST_ES_AR_BYTES);
	vmcs12->guest_cs_ar_bytes = vmcs_read32(GUEST_CS_AR_BYTES);
	vmcs12->guest_ss_ar_bytes = vmcs_read32(GUEST_SS_AR_BYTES);
	vmcs12->guest_ds_ar_bytes = vmcs_read32(GUEST_DS_AR_BYTES);
	vmcs12->guest_fs_ar_bytes = vmcs_read32(GUEST_FS_AR_BYTES);
	vmcs12->guest_gs_ar_bytes = vmcs_read32(GUEST_GS_AR_BYTES);
	vmcs12->guest_ldtr_ar_bytes = vmcs_read32(GUEST_LDTR_AR_BYTES);
	vmcs12->guest_tr_ar_bytes = vmcs_read32(GUEST_TR_AR_BYTES);
	vmcs12->guest_es_base = vmcs_readl(GUEST_ES_BASE);
	vmcs12->guest_cs_base = vmcs_readl(GUEST_CS_BASE);
	vmcs12->guest_ss_base = vmcs_readl(GUEST_SS_BASE);
	vmcs12->guest_ds_base = vmcs_readl(GUEST_DS_BASE);
	vmcs12->guest_fs_base = vmcs_readl(GUEST_FS_BASE);
	vmcs12->guest_gs_base = vmcs_readl(GUEST_GS_BASE);
	vmcs12->guest_ldtr_base = vmcs_readl(GUEST_LDTR_BASE);
	vmcs12->guest_tr_base = vmcs_readl(GUEST_TR_BASE);
	vmcs12->guest_gdtr_base = vmcs_readl(GUEST_GDTR_BASE);
	vmcs12->guest_idtr_base = vmcs_readl(GUEST_IDTR_BASE);

<<<<<<< HEAD
	vmcs12->guest_activity_state = vmcs_read32(GUEST_ACTIVITY_STATE);
=======
>>>>>>> refs/remotes/origin/master
	vmcs12->guest_interruptibility_info =
		vmcs_read32(GUEST_INTERRUPTIBILITY_INFO);
	vmcs12->guest_pending_dbg_exceptions =
		vmcs_readl(GUEST_PENDING_DBG_EXCEPTIONS);

<<<<<<< HEAD
	/* TODO: These cannot have changed unless we have MSR bitmaps and
	 * the relevant bit asks not to trap the change */
	vmcs12->guest_ia32_debugctl = vmcs_read64(GUEST_IA32_DEBUGCTL);
	if (vmcs12->vm_entry_controls & VM_EXIT_SAVE_IA32_PAT)
		vmcs12->guest_ia32_pat = vmcs_read64(GUEST_IA32_PAT);
=======
	if ((vmcs12->pin_based_vm_exec_control & PIN_BASED_VMX_PREEMPTION_TIMER) &&
	    (vmcs12->vm_exit_controls & VM_EXIT_SAVE_VMX_PREEMPTION_TIMER))
		vmcs12->vmx_preemption_timer_value =
			vmcs_read32(VMX_PREEMPTION_TIMER_VALUE);

	/*
	 * In some cases (usually, nested EPT), L2 is allowed to change its
	 * own CR3 without exiting. If it has changed it, we must keep it.
	 * Of course, if L0 is using shadow page tables, GUEST_CR3 was defined
	 * by L0, not L1 or L2, so we mustn't unconditionally copy it to vmcs12.
	 *
	 * Additionally, restore L2's PDPTR to vmcs12.
	 */
	if (enable_ept) {
		vmcs12->guest_cr3 = vmcs_read64(GUEST_CR3);
		vmcs12->guest_pdptr0 = vmcs_read64(GUEST_PDPTR0);
		vmcs12->guest_pdptr1 = vmcs_read64(GUEST_PDPTR1);
		vmcs12->guest_pdptr2 = vmcs_read64(GUEST_PDPTR2);
		vmcs12->guest_pdptr3 = vmcs_read64(GUEST_PDPTR3);
	}

	vmcs12->vm_entry_controls =
		(vmcs12->vm_entry_controls & ~VM_ENTRY_IA32E_MODE) |
		(vmcs_read32(VM_ENTRY_CONTROLS) & VM_ENTRY_IA32E_MODE);

	/* TODO: These cannot have changed unless we have MSR bitmaps and
	 * the relevant bit asks not to trap the change */
	vmcs12->guest_ia32_debugctl = vmcs_read64(GUEST_IA32_DEBUGCTL);
	if (vmcs12->vm_exit_controls & VM_EXIT_SAVE_IA32_PAT)
		vmcs12->guest_ia32_pat = vmcs_read64(GUEST_IA32_PAT);
	if (vmcs12->vm_exit_controls & VM_EXIT_SAVE_IA32_EFER)
		vmcs12->guest_ia32_efer = vcpu->arch.efer;
>>>>>>> refs/remotes/origin/master
	vmcs12->guest_sysenter_cs = vmcs_read32(GUEST_SYSENTER_CS);
	vmcs12->guest_sysenter_esp = vmcs_readl(GUEST_SYSENTER_ESP);
	vmcs12->guest_sysenter_eip = vmcs_readl(GUEST_SYSENTER_EIP);

	/* update exit information fields: */

<<<<<<< HEAD
	vmcs12->vm_exit_reason  = vmcs_read32(VM_EXIT_REASON);
	vmcs12->exit_qualification = vmcs_readl(EXIT_QUALIFICATION);

	vmcs12->vm_exit_intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
	vmcs12->vm_exit_intr_error_code = vmcs_read32(VM_EXIT_INTR_ERROR_CODE);
	vmcs12->idt_vectoring_info_field =
		vmcs_read32(IDT_VECTORING_INFO_FIELD);
	vmcs12->idt_vectoring_error_code =
		vmcs_read32(IDT_VECTORING_ERROR_CODE);
	vmcs12->vm_exit_instruction_len = vmcs_read32(VM_EXIT_INSTRUCTION_LEN);
	vmcs12->vmx_instruction_info = vmcs_read32(VMX_INSTRUCTION_INFO);

	/* clear vm-entry fields which are to be cleared on exit */
	if (!(vmcs12->vm_exit_reason & VMX_EXIT_REASONS_FAILED_VMENTRY))
		vmcs12->vm_entry_intr_info_field &= ~INTR_INFO_VALID_MASK;
=======
	vmcs12->vm_exit_reason  = to_vmx(vcpu)->exit_reason;
	vmcs12->exit_qualification = vmcs_readl(EXIT_QUALIFICATION);

	vmcs12->vm_exit_intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
	if ((vmcs12->vm_exit_intr_info &
	     (INTR_INFO_VALID_MASK | INTR_INFO_DELIVER_CODE_MASK)) ==
	    (INTR_INFO_VALID_MASK | INTR_INFO_DELIVER_CODE_MASK))
		vmcs12->vm_exit_intr_error_code =
			vmcs_read32(VM_EXIT_INTR_ERROR_CODE);
	vmcs12->idt_vectoring_info_field = 0;
	vmcs12->vm_exit_instruction_len = vmcs_read32(VM_EXIT_INSTRUCTION_LEN);
	vmcs12->vmx_instruction_info = vmcs_read32(VMX_INSTRUCTION_INFO);

	if (!(vmcs12->vm_exit_reason & VMX_EXIT_REASONS_FAILED_VMENTRY)) {
		/* vm_entry_intr_info_field is cleared on exit. Emulate this
		 * instead of reading the real value. */
		vmcs12->vm_entry_intr_info_field &= ~INTR_INFO_VALID_MASK;

		/*
		 * Transfer the event that L0 or L1 may wanted to inject into
		 * L2 to IDT_VECTORING_INFO_FIELD.
		 */
		vmcs12_save_pending_event(vcpu, vmcs12);
	}

	/*
	 * Drop what we picked up for L2 via vmx_complete_interrupts. It is
	 * preserved above and would only end up incorrectly in L1.
	 */
	vcpu->arch.nmi_injected = false;
	kvm_clear_exception_queue(vcpu);
	kvm_clear_interrupt_queue(vcpu);
>>>>>>> refs/remotes/origin/master
}

/*
 * A part of what we need to when the nested L2 guest exits and we want to
 * run its L1 parent, is to reset L1's guest state to the host state specified
 * in vmcs12.
 * This function is to be called not only on normal nested exit, but also on
 * a nested entry failure, as explained in Intel's spec, 3B.23.7 ("VM-Entry
 * Failures During or After Loading Guest State").
 * This function should be called when the active VMCS is L1's (vmcs01).
 */
<<<<<<< HEAD
void load_vmcs12_host_state(struct kvm_vcpu *vcpu, struct vmcs12 *vmcs12)
{
	if (vmcs12->vm_exit_controls & VM_EXIT_LOAD_IA32_EFER)
		vcpu->arch.efer = vmcs12->host_ia32_efer;
	if (vmcs12->vm_exit_controls & VM_EXIT_HOST_ADDR_SPACE_SIZE)
=======
static void load_vmcs12_host_state(struct kvm_vcpu *vcpu,
				   struct vmcs12 *vmcs12)
{
	struct kvm_segment seg;

	if (vmcs12->vm_exit_controls & VM_EXIT_LOAD_IA32_EFER)
		vcpu->arch.efer = vmcs12->host_ia32_efer;
	else if (vmcs12->vm_exit_controls & VM_EXIT_HOST_ADDR_SPACE_SIZE)
>>>>>>> refs/remotes/origin/master
		vcpu->arch.efer |= (EFER_LMA | EFER_LME);
	else
		vcpu->arch.efer &= ~(EFER_LMA | EFER_LME);
	vmx_set_efer(vcpu, vcpu->arch.efer);

	kvm_register_write(vcpu, VCPU_REGS_RSP, vmcs12->host_rsp);
	kvm_register_write(vcpu, VCPU_REGS_RIP, vmcs12->host_rip);
<<<<<<< HEAD
=======
	vmx_set_rflags(vcpu, X86_EFLAGS_FIXED);
>>>>>>> refs/remotes/origin/master
	/*
	 * Note that calling vmx_set_cr0 is important, even if cr0 hasn't
	 * actually changed, because it depends on the current state of
	 * fpu_active (which may have changed).
	 * Note that vmx_set_cr0 refers to efer set above.
	 */
<<<<<<< HEAD
	kvm_set_cr0(vcpu, vmcs12->host_cr0);
=======
	vmx_set_cr0(vcpu, vmcs12->host_cr0);
>>>>>>> refs/remotes/origin/master
	/*
	 * If we did fpu_activate()/fpu_deactivate() during L2's run, we need
	 * to apply the same changes to L1's vmcs. We just set cr0 correctly,
	 * but we also need to update cr0_guest_host_mask and exception_bitmap.
	 */
	update_exception_bitmap(vcpu);
	vcpu->arch.cr0_guest_owned_bits = (vcpu->fpu_active ? X86_CR0_TS : 0);
	vmcs_writel(CR0_GUEST_HOST_MASK, ~vcpu->arch.cr0_guest_owned_bits);

	/*
	 * Note that CR4_GUEST_HOST_MASK is already set in the original vmcs01
	 * (KVM doesn't change it)- no reason to call set_cr4_guest_host_mask();
	 */
	vcpu->arch.cr4_guest_owned_bits = ~vmcs_readl(CR4_GUEST_HOST_MASK);
	kvm_set_cr4(vcpu, vmcs12->host_cr4);

<<<<<<< HEAD
	/* shadow page tables on either EPT or shadow page tables */
	kvm_set_cr3(vcpu, vmcs12->host_cr3);
	kvm_mmu_reset_context(vcpu);

=======
	nested_ept_uninit_mmu_context(vcpu);

	kvm_set_cr3(vcpu, vmcs12->host_cr3);
	kvm_mmu_reset_context(vcpu);

	if (!enable_ept)
		vcpu->arch.walk_mmu->inject_page_fault = kvm_inject_page_fault;

>>>>>>> refs/remotes/origin/master
	if (enable_vpid) {
		/*
		 * Trivially support vpid by letting L2s share their parent
		 * L1's vpid. TODO: move to a more elaborate solution, giving
		 * each L2 its own vpid and exposing the vpid feature to L1.
		 */
		vmx_flush_tlb(vcpu);
	}


	vmcs_write32(GUEST_SYSENTER_CS, vmcs12->host_ia32_sysenter_cs);
	vmcs_writel(GUEST_SYSENTER_ESP, vmcs12->host_ia32_sysenter_esp);
	vmcs_writel(GUEST_SYSENTER_EIP, vmcs12->host_ia32_sysenter_eip);
	vmcs_writel(GUEST_IDTR_BASE, vmcs12->host_idtr_base);
	vmcs_writel(GUEST_GDTR_BASE, vmcs12->host_gdtr_base);
<<<<<<< HEAD
	vmcs_writel(GUEST_TR_BASE, vmcs12->host_tr_base);
	vmcs_writel(GUEST_GS_BASE, vmcs12->host_gs_base);
	vmcs_writel(GUEST_FS_BASE, vmcs12->host_fs_base);
	vmcs_write16(GUEST_ES_SELECTOR, vmcs12->host_es_selector);
	vmcs_write16(GUEST_CS_SELECTOR, vmcs12->host_cs_selector);
	vmcs_write16(GUEST_SS_SELECTOR, vmcs12->host_ss_selector);
	vmcs_write16(GUEST_DS_SELECTOR, vmcs12->host_ds_selector);
	vmcs_write16(GUEST_FS_SELECTOR, vmcs12->host_fs_selector);
	vmcs_write16(GUEST_GS_SELECTOR, vmcs12->host_gs_selector);
	vmcs_write16(GUEST_TR_SELECTOR, vmcs12->host_tr_selector);

	if (vmcs12->vm_exit_controls & VM_EXIT_LOAD_IA32_PAT)
		vmcs_write64(GUEST_IA32_PAT, vmcs12->host_ia32_pat);
	if (vmcs12->vm_exit_controls & VM_EXIT_LOAD_IA32_PERF_GLOBAL_CTRL)
		vmcs_write64(GUEST_IA32_PERF_GLOBAL_CTRL,
			vmcs12->host_ia32_perf_global_ctrl);
=======

	if (vmcs12->vm_exit_controls & VM_EXIT_LOAD_IA32_PAT) {
		vmcs_write64(GUEST_IA32_PAT, vmcs12->host_ia32_pat);
		vcpu->arch.pat = vmcs12->host_ia32_pat;
	}
	if (vmcs12->vm_exit_controls & VM_EXIT_LOAD_IA32_PERF_GLOBAL_CTRL)
		vmcs_write64(GUEST_IA32_PERF_GLOBAL_CTRL,
			vmcs12->host_ia32_perf_global_ctrl);

	/* Set L1 segment info according to Intel SDM
	    27.5.2 Loading Host Segment and Descriptor-Table Registers */
	seg = (struct kvm_segment) {
		.base = 0,
		.limit = 0xFFFFFFFF,
		.selector = vmcs12->host_cs_selector,
		.type = 11,
		.present = 1,
		.s = 1,
		.g = 1
	};
	if (vmcs12->vm_exit_controls & VM_EXIT_HOST_ADDR_SPACE_SIZE)
		seg.l = 1;
	else
		seg.db = 1;
	vmx_set_segment(vcpu, &seg, VCPU_SREG_CS);
	seg = (struct kvm_segment) {
		.base = 0,
		.limit = 0xFFFFFFFF,
		.type = 3,
		.present = 1,
		.s = 1,
		.db = 1,
		.g = 1
	};
	seg.selector = vmcs12->host_ds_selector;
	vmx_set_segment(vcpu, &seg, VCPU_SREG_DS);
	seg.selector = vmcs12->host_es_selector;
	vmx_set_segment(vcpu, &seg, VCPU_SREG_ES);
	seg.selector = vmcs12->host_ss_selector;
	vmx_set_segment(vcpu, &seg, VCPU_SREG_SS);
	seg.selector = vmcs12->host_fs_selector;
	seg.base = vmcs12->host_fs_base;
	vmx_set_segment(vcpu, &seg, VCPU_SREG_FS);
	seg.selector = vmcs12->host_gs_selector;
	seg.base = vmcs12->host_gs_base;
	vmx_set_segment(vcpu, &seg, VCPU_SREG_GS);
	seg = (struct kvm_segment) {
		.base = vmcs12->host_tr_base,
		.limit = 0x67,
		.selector = vmcs12->host_tr_selector,
		.type = 11,
		.present = 1
	};
	vmx_set_segment(vcpu, &seg, VCPU_SREG_TR);

	kvm_set_dr(vcpu, 7, 0x400);
	vmcs_write64(GUEST_IA32_DEBUGCTL, 0);
>>>>>>> refs/remotes/origin/master
}

/*
 * Emulate an exit from nested guest (L2) to L1, i.e., prepare to run L1
 * and modify vmcs12 to make it see what it would expect to see there if
 * L2 was its real guest. Must only be called when in L2 (is_guest_mode())
 */
static void nested_vmx_vmexit(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	int cpu;
	struct vmcs12 *vmcs12 = get_vmcs12(vcpu);

<<<<<<< HEAD
=======
	/* trying to cancel vmlaunch/vmresume is a bug */
	WARN_ON_ONCE(vmx->nested.nested_run_pending);

>>>>>>> refs/remotes/origin/master
	leave_guest_mode(vcpu);
	prepare_vmcs12(vcpu, vmcs12);

	cpu = get_cpu();
	vmx->loaded_vmcs = &vmx->vmcs01;
	vmx_vcpu_put(vcpu);
	vmx_vcpu_load(vcpu, cpu);
	vcpu->cpu = cpu;
	put_cpu();

<<<<<<< HEAD
=======
	vmx_segment_cache_clear(vmx);

>>>>>>> refs/remotes/origin/master
	/* if no vmcs02 cache requested, remove the one we used */
	if (VMCS02_POOL_SIZE == 0)
		nested_free_vmcs02(vmx, vmx->nested.current_vmptr);

	load_vmcs12_host_state(vcpu, vmcs12);

	/* Update TSC_OFFSET if TSC was changed while L2 ran */
	vmcs_write64(TSC_OFFSET, vmx->nested.vmcs01_tsc_offset);

	/* This is needed for same reason as it was needed in prepare_vmcs02 */
	vmx->host_rsp = 0;

	/* Unpin physical memory we referred to in vmcs02 */
	if (vmx->nested.apic_access_page) {
		nested_release_page(vmx->nested.apic_access_page);
		vmx->nested.apic_access_page = 0;
	}

	/*
	 * Exiting from L2 to L1, we're now back to L1 which thinks it just
	 * finished a VMLAUNCH or VMRESUME instruction, so we need to set the
	 * success or failure flag accordingly.
	 */
	if (unlikely(vmx->fail)) {
		vmx->fail = 0;
		nested_vmx_failValid(vcpu, vmcs_read32(VM_INSTRUCTION_ERROR));
	} else
		nested_vmx_succeed(vcpu);
<<<<<<< HEAD
=======
	if (enable_shadow_vmcs)
		vmx->nested.sync_shadow_vmcs = true;
>>>>>>> refs/remotes/origin/master
}

/*
 * L1's failure to enter L2 is a subset of a normal exit, as explained in
 * 23.7 "VM-entry failures during or after loading guest state" (this also
 * lists the acceptable exit-reason and exit-qualification parameters).
 * It should only be called before L2 actually succeeded to run, and when
 * vmcs01 is current (it doesn't leave_guest_mode() or switch vmcss).
 */
static void nested_vmx_entry_failure(struct kvm_vcpu *vcpu,
			struct vmcs12 *vmcs12,
			u32 reason, unsigned long qualification)
{
	load_vmcs12_host_state(vcpu, vmcs12);
	vmcs12->vm_exit_reason = reason | VMX_EXIT_REASONS_FAILED_VMENTRY;
	vmcs12->exit_qualification = qualification;
	nested_vmx_succeed(vcpu);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (enable_shadow_vmcs)
		to_vmx(vcpu)->nested.sync_shadow_vmcs = true;
>>>>>>> refs/remotes/origin/master
}

static int vmx_check_intercept(struct kvm_vcpu *vcpu,
			       struct x86_instruction_info *info,
			       enum x86_intercept_stage stage)
{
	return X86EMUL_CONTINUE;
}

static struct kvm_x86_ops vmx_x86_ops = {
	.cpu_has_kvm_support = cpu_has_kvm_support,
	.disabled_by_bios = vmx_disabled_by_bios,
	.hardware_setup = hardware_setup,
	.hardware_unsetup = hardware_unsetup,
	.check_processor_compatibility = vmx_check_processor_compat,
	.hardware_enable = hardware_enable,
	.hardware_disable = hardware_disable,
	.cpu_has_accelerated_tpr = report_flexpriority,

	.vcpu_create = vmx_create_vcpu,
	.vcpu_free = vmx_free_vcpu,
	.vcpu_reset = vmx_vcpu_reset,

	.prepare_guest_switch = vmx_save_host_state,
	.vcpu_load = vmx_vcpu_load,
	.vcpu_put = vmx_vcpu_put,

<<<<<<< HEAD
	.set_guest_debug = set_guest_debug,
=======
	.update_db_bp_intercept = update_exception_bitmap,
>>>>>>> refs/remotes/origin/master
	.get_msr = vmx_get_msr,
	.set_msr = vmx_set_msr,
	.get_segment_base = vmx_get_segment_base,
	.get_segment = vmx_get_segment,
	.set_segment = vmx_set_segment,
	.get_cpl = vmx_get_cpl,
	.get_cs_db_l_bits = vmx_get_cs_db_l_bits,
	.decache_cr0_guest_bits = vmx_decache_cr0_guest_bits,
	.decache_cr3 = vmx_decache_cr3,
	.decache_cr4_guest_bits = vmx_decache_cr4_guest_bits,
	.set_cr0 = vmx_set_cr0,
	.set_cr3 = vmx_set_cr3,
	.set_cr4 = vmx_set_cr4,
	.set_efer = vmx_set_efer,
	.get_idt = vmx_get_idt,
	.set_idt = vmx_set_idt,
	.get_gdt = vmx_get_gdt,
	.set_gdt = vmx_set_gdt,
	.set_dr7 = vmx_set_dr7,
	.cache_reg = vmx_cache_reg,
	.get_rflags = vmx_get_rflags,
	.set_rflags = vmx_set_rflags,
	.fpu_activate = vmx_fpu_activate,
	.fpu_deactivate = vmx_fpu_deactivate,

	.tlb_flush = vmx_flush_tlb,

	.run = vmx_vcpu_run,
	.handle_exit = vmx_handle_exit,
	.skip_emulated_instruction = skip_emulated_instruction,
	.set_interrupt_shadow = vmx_set_interrupt_shadow,
	.get_interrupt_shadow = vmx_get_interrupt_shadow,
	.patch_hypercall = vmx_patch_hypercall,
	.set_irq = vmx_inject_irq,
	.set_nmi = vmx_inject_nmi,
	.queue_exception = vmx_queue_exception,
	.cancel_injection = vmx_cancel_injection,
	.interrupt_allowed = vmx_interrupt_allowed,
	.nmi_allowed = vmx_nmi_allowed,
	.get_nmi_mask = vmx_get_nmi_mask,
	.set_nmi_mask = vmx_set_nmi_mask,
	.enable_nmi_window = enable_nmi_window,
	.enable_irq_window = enable_irq_window,
	.update_cr8_intercept = update_cr8_intercept,
<<<<<<< HEAD
=======
	.set_virtual_x2apic_mode = vmx_set_virtual_x2apic_mode,
	.vm_has_apicv = vmx_vm_has_apicv,
	.load_eoi_exitmap = vmx_load_eoi_exitmap,
	.hwapic_irr_update = vmx_hwapic_irr_update,
	.hwapic_isr_update = vmx_hwapic_isr_update,
	.sync_pir_to_irr = vmx_sync_pir_to_irr,
	.deliver_posted_interrupt = vmx_deliver_posted_interrupt,
>>>>>>> refs/remotes/origin/master

	.set_tss_addr = vmx_set_tss_addr,
	.get_tdp_level = get_ept_level,
	.get_mt_mask = vmx_get_mt_mask,

	.get_exit_info = vmx_get_exit_info,
<<<<<<< HEAD
<<<<<<< HEAD
	.exit_reasons_str = vmx_exit_reasons_str,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	.get_lpage_level = vmx_get_lpage_level,

	.cpuid_update = vmx_cpuid_update,

	.rdtscp_supported = vmx_rdtscp_supported,
<<<<<<< HEAD
=======
	.invpcid_supported = vmx_invpcid_supported,
>>>>>>> refs/remotes/origin/master

	.set_supported_cpuid = vmx_set_supported_cpuid,

	.has_wbinvd_exit = cpu_has_vmx_wbinvd_exit,

	.set_tsc_khz = vmx_set_tsc_khz,
<<<<<<< HEAD
	.write_tsc_offset = vmx_write_tsc_offset,
	.adjust_tsc_offset = vmx_adjust_tsc_offset,
	.compute_tsc_offset = vmx_compute_tsc_offset,
<<<<<<< HEAD
=======
	.read_l1_tsc = vmx_read_l1_tsc,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.read_tsc_offset = vmx_read_tsc_offset,
	.write_tsc_offset = vmx_write_tsc_offset,
	.adjust_tsc_offset = vmx_adjust_tsc_offset,
	.compute_tsc_offset = vmx_compute_tsc_offset,
	.read_l1_tsc = vmx_read_l1_tsc,
>>>>>>> refs/remotes/origin/master

	.set_tdp_cr3 = vmx_set_cr3,

	.check_intercept = vmx_check_intercept,
<<<<<<< HEAD
=======
	.handle_external_intr = vmx_handle_external_intr,
>>>>>>> refs/remotes/origin/master
};

static int __init vmx_init(void)
{
<<<<<<< HEAD
	int r, i;
=======
	int r, i, msr;
>>>>>>> refs/remotes/origin/master

	rdmsrl_safe(MSR_EFER, &host_efer);

	for (i = 0; i < NR_VMX_MSR; ++i)
		kvm_define_shared_msr(i, vmx_msr_index[i]);

	vmx_io_bitmap_a = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_io_bitmap_a)
		return -ENOMEM;

<<<<<<< HEAD
	vmx_io_bitmap_b = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_io_bitmap_b) {
		r = -ENOMEM;
		goto out;
	}

	vmx_msr_bitmap_legacy = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_msr_bitmap_legacy) {
		r = -ENOMEM;
		goto out1;
	}

	vmx_msr_bitmap_longmode = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_msr_bitmap_longmode) {
		r = -ENOMEM;
		goto out2;
	}
=======
	r = -ENOMEM;

	vmx_io_bitmap_b = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_io_bitmap_b)
		goto out;

	vmx_msr_bitmap_legacy = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_msr_bitmap_legacy)
		goto out1;

	vmx_msr_bitmap_legacy_x2apic =
				(unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_msr_bitmap_legacy_x2apic)
		goto out2;

	vmx_msr_bitmap_longmode = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_msr_bitmap_longmode)
		goto out3;

	vmx_msr_bitmap_longmode_x2apic =
				(unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_msr_bitmap_longmode_x2apic)
		goto out4;
	vmx_vmread_bitmap = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_vmread_bitmap)
		goto out5;

	vmx_vmwrite_bitmap = (unsigned long *)__get_free_page(GFP_KERNEL);
	if (!vmx_vmwrite_bitmap)
		goto out6;

	memset(vmx_vmread_bitmap, 0xff, PAGE_SIZE);
	memset(vmx_vmwrite_bitmap, 0xff, PAGE_SIZE);
	/* shadowed read/write fields */
	for (i = 0; i < max_shadow_read_write_fields; i++) {
		clear_bit(shadow_read_write_fields[i], vmx_vmwrite_bitmap);
		clear_bit(shadow_read_write_fields[i], vmx_vmread_bitmap);
	}
	/* shadowed read only fields */
	for (i = 0; i < max_shadow_read_only_fields; i++)
		clear_bit(shadow_read_only_fields[i], vmx_vmread_bitmap);
>>>>>>> refs/remotes/origin/master

	/*
	 * Allow direct access to the PC debug port (it is often used for I/O
	 * delays, but the vmexits simply slow things down).
	 */
	memset(vmx_io_bitmap_a, 0xff, PAGE_SIZE);
	clear_bit(0x80, vmx_io_bitmap_a);

	memset(vmx_io_bitmap_b, 0xff, PAGE_SIZE);

	memset(vmx_msr_bitmap_legacy, 0xff, PAGE_SIZE);
	memset(vmx_msr_bitmap_longmode, 0xff, PAGE_SIZE);

	set_bit(0, vmx_vpid_bitmap); /* 0 is reserved for host */

	r = kvm_init(&vmx_x86_ops, sizeof(struct vcpu_vmx),
		     __alignof__(struct vcpu_vmx), THIS_MODULE);
	if (r)
<<<<<<< HEAD
		goto out3;
=======
		goto out7;

#ifdef CONFIG_KEXEC
	rcu_assign_pointer(crash_vmclear_loaded_vmcss,
			   crash_vmclear_local_loaded_vmcss);
#endif
>>>>>>> refs/remotes/origin/master

	vmx_disable_intercept_for_msr(MSR_FS_BASE, false);
	vmx_disable_intercept_for_msr(MSR_GS_BASE, false);
	vmx_disable_intercept_for_msr(MSR_KERNEL_GS_BASE, true);
	vmx_disable_intercept_for_msr(MSR_IA32_SYSENTER_CS, false);
	vmx_disable_intercept_for_msr(MSR_IA32_SYSENTER_ESP, false);
	vmx_disable_intercept_for_msr(MSR_IA32_SYSENTER_EIP, false);
<<<<<<< HEAD

	if (enable_ept) {
<<<<<<< HEAD
		bypass_guest_pf = 0;
		kvm_mmu_set_mask_ptes(0ull, 0ull, 0ull, 0ull,
				VMX_EPT_EXECUTABLE_MASK);
=======
		kvm_mmu_set_mask_ptes(0ull, 0ull, 0ull, 0ull,
				VMX_EPT_EXECUTABLE_MASK);
		ept_set_mmio_spte_mask();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memcpy(vmx_msr_bitmap_legacy_x2apic,
			vmx_msr_bitmap_legacy, PAGE_SIZE);
	memcpy(vmx_msr_bitmap_longmode_x2apic,
			vmx_msr_bitmap_longmode, PAGE_SIZE);

	if (enable_apicv) {
		for (msr = 0x800; msr <= 0x8ff; msr++)
			vmx_disable_intercept_msr_read_x2apic(msr);

		/* According SDM, in x2apic mode, the whole id reg is used.
		 * But in KVM, it only use the highest eight bits. Need to
		 * intercept it */
		vmx_enable_intercept_msr_read_x2apic(0x802);
		/* TMCCT */
		vmx_enable_intercept_msr_read_x2apic(0x839);
		/* TPR */
		vmx_disable_intercept_msr_write_x2apic(0x808);
		/* EOI */
		vmx_disable_intercept_msr_write_x2apic(0x80b);
		/* SELF-IPI */
		vmx_disable_intercept_msr_write_x2apic(0x83f);
	}

	if (enable_ept) {
		kvm_mmu_set_mask_ptes(0ull,
			(enable_ept_ad_bits) ? VMX_EPT_ACCESS_BIT : 0ull,
			(enable_ept_ad_bits) ? VMX_EPT_DIRTY_BIT : 0ull,
			0ull, VMX_EPT_EXECUTABLE_MASK);
		ept_set_mmio_spte_mask();
>>>>>>> refs/remotes/origin/master
		kvm_enable_tdp();
	} else
		kvm_disable_tdp();

<<<<<<< HEAD
<<<<<<< HEAD
	if (bypass_guest_pf)
		kvm_mmu_set_nonpresent_ptes(~0xffeull, 0ull);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;

out3:
	free_page((unsigned long)vmx_msr_bitmap_longmode);
=======
	return 0;

out7:
	free_page((unsigned long)vmx_vmwrite_bitmap);
out6:
	free_page((unsigned long)vmx_vmread_bitmap);
out5:
	free_page((unsigned long)vmx_msr_bitmap_longmode_x2apic);
out4:
	free_page((unsigned long)vmx_msr_bitmap_longmode);
out3:
	free_page((unsigned long)vmx_msr_bitmap_legacy_x2apic);
>>>>>>> refs/remotes/origin/master
out2:
	free_page((unsigned long)vmx_msr_bitmap_legacy);
out1:
	free_page((unsigned long)vmx_io_bitmap_b);
out:
	free_page((unsigned long)vmx_io_bitmap_a);
	return r;
}

static void __exit vmx_exit(void)
{
<<<<<<< HEAD
=======
	free_page((unsigned long)vmx_msr_bitmap_legacy_x2apic);
	free_page((unsigned long)vmx_msr_bitmap_longmode_x2apic);
>>>>>>> refs/remotes/origin/master
	free_page((unsigned long)vmx_msr_bitmap_legacy);
	free_page((unsigned long)vmx_msr_bitmap_longmode);
	free_page((unsigned long)vmx_io_bitmap_b);
	free_page((unsigned long)vmx_io_bitmap_a);
<<<<<<< HEAD
=======
	free_page((unsigned long)vmx_vmwrite_bitmap);
	free_page((unsigned long)vmx_vmread_bitmap);

#ifdef CONFIG_KEXEC
	rcu_assign_pointer(crash_vmclear_loaded_vmcss, NULL);
	synchronize_rcu();
#endif
>>>>>>> refs/remotes/origin/master

	kvm_exit();
}

module_init(vmx_init)
module_exit(vmx_exit)
