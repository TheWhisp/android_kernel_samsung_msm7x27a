#ifndef __KVM_HOST_H
#define __KVM_HOST_H

/*
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#include <linux/types.h>
#include <linux/hardirq.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/signal.h>
#include <linux/sched.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/mm.h>
=======
#include <linux/bug.h>
#include <linux/mm.h>
#include <linux/mmu_notifier.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/bug.h>
#include <linux/mm.h>
#include <linux/mmu_notifier.h>
>>>>>>> refs/remotes/origin/master
#include <linux/preempt.h>
#include <linux/msi.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/ratelimit.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/ratelimit.h>
#include <linux/err.h>
#include <linux/irqflags.h>
#include <linux/context_tracking.h>
>>>>>>> refs/remotes/origin/master
#include <asm/signal.h>

#include <linux/kvm.h>
#include <linux/kvm_para.h>

#include <linux/kvm_types.h>

#include <asm/kvm_host.h>

#ifndef KVM_MMIO_SIZE
#define KVM_MMIO_SIZE 8
#endif

/*
<<<<<<< HEAD
=======
 * The bit 16 ~ bit 31 of kvm_memory_region::flags are internally used
 * in kvm, other bits are visible for userspace which are defined in
 * include/linux/kvm_h.
 */
#define KVM_MEMSLOT_INVALID	(1UL << 16)

/* Two fragments for cross MMIO pages. */
#define KVM_MAX_MMIO_FRAGMENTS	2

/*
 * For the normal pfn, the highest 12 bits should be zero,
 * so we can mask bit 62 ~ bit 52  to indicate the error pfn,
 * mask bit 63 to indicate the noslot pfn.
 */
#define KVM_PFN_ERR_MASK	(0x7ffULL << 52)
#define KVM_PFN_ERR_NOSLOT_MASK	(0xfffULL << 52)
#define KVM_PFN_NOSLOT		(0x1ULL << 63)

#define KVM_PFN_ERR_FAULT	(KVM_PFN_ERR_MASK)
#define KVM_PFN_ERR_HWPOISON	(KVM_PFN_ERR_MASK + 1)
#define KVM_PFN_ERR_RO_FAULT	(KVM_PFN_ERR_MASK + 2)

/*
 * error pfns indicate that the gfn is in slot but faild to
 * translate it to pfn on host.
 */
static inline bool is_error_pfn(pfn_t pfn)
{
	return !!(pfn & KVM_PFN_ERR_MASK);
}

/*
 * error_noslot pfns indicate that the gfn can not be
 * translated to pfn - it is not in slot or failed to
 * translate it to pfn.
 */
static inline bool is_error_noslot_pfn(pfn_t pfn)
{
	return !!(pfn & KVM_PFN_ERR_NOSLOT_MASK);
}

/* noslot pfn indicates that the gfn is not in slot. */
static inline bool is_noslot_pfn(pfn_t pfn)
{
	return pfn == KVM_PFN_NOSLOT;
}

/*
 * architectures with KVM_HVA_ERR_BAD other than PAGE_OFFSET (e.g. s390)
 * provide own defines and kvm_is_error_hva
 */
#ifndef KVM_HVA_ERR_BAD

#define KVM_HVA_ERR_BAD		(PAGE_OFFSET)
#define KVM_HVA_ERR_RO_BAD	(PAGE_OFFSET + PAGE_SIZE)

static inline bool kvm_is_error_hva(unsigned long addr)
{
	return addr >= PAGE_OFFSET;
}

#endif

#define KVM_ERR_PTR_BAD_PAGE	(ERR_PTR(-ENOENT))

static inline bool is_error_page(struct page *page)
{
	return IS_ERR(page);
}

/*
>>>>>>> refs/remotes/origin/master
 * vcpu->requests bit members
 */
#define KVM_REQ_TLB_FLUSH          0
#define KVM_REQ_MIGRATE_TIMER      1
#define KVM_REQ_REPORT_TPR_ACCESS  2
#define KVM_REQ_MMU_RELOAD         3
#define KVM_REQ_TRIPLE_FAULT       4
#define KVM_REQ_PENDING_TIMER      5
#define KVM_REQ_UNHALT             6
#define KVM_REQ_MMU_SYNC           7
#define KVM_REQ_CLOCK_UPDATE       8
#define KVM_REQ_KICK               9
#define KVM_REQ_DEACTIVATE_FPU    10
#define KVM_REQ_EVENT             11
#define KVM_REQ_APF_HALT          12
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define KVM_REQ_STEAL_UPDATE      13
#define KVM_REQ_NMI               14
#define KVM_REQ_IMMEDIATE_EXIT    15
#define KVM_REQ_PMU               16
#define KVM_REQ_PMI               17
>>>>>>> refs/remotes/origin/cm-10.0

#define KVM_USERSPACE_IRQ_SOURCE_ID	0
=======
#define KVM_REQ_STEAL_UPDATE      13
#define KVM_REQ_NMI               14
#define KVM_REQ_PMU               15
#define KVM_REQ_PMI               16
#define KVM_REQ_WATCHDOG          17
#define KVM_REQ_MASTERCLOCK_UPDATE 18
#define KVM_REQ_MCLOCK_INPROGRESS 19
#define KVM_REQ_EPR_EXIT          20
#define KVM_REQ_SCAN_IOAPIC       21
#define KVM_REQ_GLOBAL_CLOCK_UPDATE 22

#define KVM_USERSPACE_IRQ_SOURCE_ID		0
#define KVM_IRQFD_RESAMPLE_IRQ_SOURCE_ID	1
>>>>>>> refs/remotes/origin/master

struct kvm;
struct kvm_vcpu;
extern struct kmem_cache *kvm_vcpu_cache;

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * It would be nice to use something smarter than a linear search, TBD...
 * Thankfully we dont expect many devices to register (famous last words :),
 * so until then it will suffice.  At least its abstracted so we can change
 * in one place.
 */
struct kvm_io_bus {
	int                   dev_count;
#define NR_IOBUS_DEVS 200
	struct kvm_io_device *devs[NR_IOBUS_DEVS];
=======
=======
extern spinlock_t kvm_lock;
extern struct list_head vm_list;

>>>>>>> refs/remotes/origin/master
struct kvm_io_range {
	gpa_t addr;
	int len;
	struct kvm_io_device *dev;
};

<<<<<<< HEAD
struct kvm_io_bus {
	int                   dev_count;
#define NR_IOBUS_DEVS 300
	struct kvm_io_range range[NR_IOBUS_DEVS];
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define NR_IOBUS_DEVS 1000

struct kvm_io_bus {
	int dev_count;
	int ioeventfd_count;
	struct kvm_io_range range[];
>>>>>>> refs/remotes/origin/master
};

enum kvm_bus {
	KVM_MMIO_BUS,
	KVM_PIO_BUS,
<<<<<<< HEAD
=======
	KVM_VIRTIO_CCW_NOTIFY_BUS,
>>>>>>> refs/remotes/origin/master
	KVM_NR_BUSES
};

int kvm_io_bus_write(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
		     int len, const void *val);
<<<<<<< HEAD
int kvm_io_bus_read(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr, int len,
		    void *val);
<<<<<<< HEAD
int kvm_io_bus_register_dev(struct kvm *kvm, enum kvm_bus bus_idx,
			    struct kvm_io_device *dev);
=======
int kvm_io_bus_register_dev(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			    int len, struct kvm_io_device *dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int kvm_io_bus_write_cookie(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			    int len, const void *val, long cookie);
int kvm_io_bus_read(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr, int len,
		    void *val);
int kvm_io_bus_read_cookie(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			   int len, void *val, long cookie);
int kvm_io_bus_register_dev(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			    int len, struct kvm_io_device *dev);
>>>>>>> refs/remotes/origin/master
int kvm_io_bus_unregister_dev(struct kvm *kvm, enum kvm_bus bus_idx,
			      struct kvm_io_device *dev);

#ifdef CONFIG_KVM_ASYNC_PF
struct kvm_async_pf {
	struct work_struct work;
	struct list_head link;
	struct list_head queue;
	struct kvm_vcpu *vcpu;
	struct mm_struct *mm;
	gva_t gva;
	unsigned long addr;
	struct kvm_arch_async_pf arch;
<<<<<<< HEAD
	struct page *page;
	bool done;
=======
	bool   wakeup_all;
>>>>>>> refs/remotes/origin/master
};

void kvm_clear_async_pf_completion_queue(struct kvm_vcpu *vcpu);
void kvm_check_async_pf_completion(struct kvm_vcpu *vcpu);
int kvm_setup_async_pf(struct kvm_vcpu *vcpu, gva_t gva, gfn_t gfn,
		       struct kvm_arch_async_pf *arch);
int kvm_async_pf_wakeup_all(struct kvm_vcpu *vcpu);
#endif

enum {
	OUTSIDE_GUEST_MODE,
	IN_GUEST_MODE,
<<<<<<< HEAD
	EXITING_GUEST_MODE
=======
	EXITING_GUEST_MODE,
	READING_SHADOW_PAGE_TABLES,
};

/*
 * Sometimes a large or cross-page mmio needs to be broken up into separate
 * exits for userspace servicing.
 */
struct kvm_mmio_fragment {
	gpa_t gpa;
	void *data;
	unsigned len;
>>>>>>> refs/remotes/origin/master
};

struct kvm_vcpu {
	struct kvm *kvm;
#ifdef CONFIG_PREEMPT_NOTIFIERS
	struct preempt_notifier preempt_notifier;
#endif
	int cpu;
	int vcpu_id;
	int srcu_idx;
	int mode;
	unsigned long requests;
	unsigned long guest_debug;

	struct mutex mutex;
	struct kvm_run *run;

	int fpu_active;
	int guest_fpu_loaded, guest_xcr0_loaded;
	wait_queue_head_t wq;
	struct pid *pid;
	int sigset_active;
	sigset_t sigset;
	struct kvm_vcpu_stat stat;

#ifdef CONFIG_HAS_IOMEM
	int mmio_needed;
	int mmio_read_completed;
	int mmio_is_write;
<<<<<<< HEAD
	int mmio_size;
	int mmio_index;
	unsigned char mmio_data[KVM_MMIO_SIZE];
	gpa_t mmio_phys_addr;
=======
	int mmio_cur_fragment;
	int mmio_nr_fragments;
	struct kvm_mmio_fragment mmio_fragments[KVM_MAX_MMIO_FRAGMENTS];
>>>>>>> refs/remotes/origin/master
#endif

#ifdef CONFIG_KVM_ASYNC_PF
	struct {
		u32 queued;
		struct list_head queue;
		struct list_head done;
		spinlock_t lock;
	} async_pf;
#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_HAVE_KVM_CPU_RELAX_INTERCEPT
	/*
	 * Cpu relax intercept or pause loop exit optimization
	 * in_spin_loop: set when a vcpu does a pause loop exit
	 *  or cpu relax intercepted.
	 * dy_eligible: indicates whether vcpu is eligible for directed yield.
	 */
	struct {
		bool in_spin_loop;
		bool dy_eligible;
	} spin_loop;
#endif
	bool preempted;
>>>>>>> refs/remotes/origin/master
	struct kvm_vcpu_arch arch;
};

static inline int kvm_vcpu_exiting_guest_mode(struct kvm_vcpu *vcpu)
{
	return cmpxchg(&vcpu->mode, IN_GUEST_MODE, EXITING_GUEST_MODE);
}

/*
 * Some of the bitops functions do not support too long bitmaps.
 * This number must be determined not to exceed such limits.
 */
#define KVM_MEM_MAX_NR_PAGES ((1UL << 31) - 1)

<<<<<<< HEAD
<<<<<<< HEAD
struct kvm_lpage_info {
	unsigned long rmap_pde;
	int write_count;
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
struct kvm_memory_slot {
	gfn_t base_gfn;
	unsigned long npages;
	unsigned long flags;
	unsigned long *rmap;
	unsigned long *dirty_bitmap;
	unsigned long *dirty_bitmap_head;
<<<<<<< HEAD
	struct kvm_lpage_info *lpage_info[KVM_NR_PAGE_SIZES - 1];
=======
	unsigned long nr_dirty_pages;
	struct kvm_arch_memory_slot arch;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long userspace_addr;
	int user_alloc;
	int id;
=======
struct kvm_memory_slot {
	gfn_t base_gfn;
	unsigned long npages;
	unsigned long *dirty_bitmap;
	struct kvm_arch_memory_slot arch;
	unsigned long userspace_addr;
	u32 flags;
	short id;
>>>>>>> refs/remotes/origin/master
};

static inline unsigned long kvm_dirty_bitmap_bytes(struct kvm_memory_slot *memslot)
{
	return ALIGN(memslot->npages, BITS_PER_LONG) / 8;
}

struct kvm_kernel_irq_routing_entry {
	u32 gsi;
	u32 type;
	int (*set)(struct kvm_kernel_irq_routing_entry *e,
<<<<<<< HEAD
		   struct kvm *kvm, int irq_source_id, int level);
=======
		   struct kvm *kvm, int irq_source_id, int level,
		   bool line_status);
>>>>>>> refs/remotes/origin/master
	union {
		struct {
			unsigned irqchip;
			unsigned pin;
		} irqchip;
		struct msi_msg msi;
	};
	struct hlist_node link;
};

<<<<<<< HEAD
#ifdef __KVM_HAVE_IOAPIC

struct kvm_irq_routing_table {
	int chip[KVM_NR_IRQCHIPS][KVM_IOAPIC_NUM_PINS];
=======
#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING

struct kvm_irq_routing_table {
	int chip[KVM_NR_IRQCHIPS][KVM_IRQCHIP_NUM_PINS];
>>>>>>> refs/remotes/origin/master
	struct kvm_kernel_irq_routing_entry *rt_entries;
	u32 nr_rt_entries;
	/*
	 * Array indexed by gsi. Each entry contains list of irq chips
	 * the gsi is connected to.
	 */
	struct hlist_head map[0];
};

#else

struct kvm_irq_routing_table {};

#endif

<<<<<<< HEAD
<<<<<<< HEAD
struct kvm_memslots {
	int nmemslots;
	u64 generation;
	struct kvm_memory_slot memslots[KVM_MEMORY_SLOTS +
					KVM_PRIVATE_MEM_SLOTS];
=======
#ifndef KVM_MEM_SLOTS_NUM
#define KVM_MEM_SLOTS_NUM (KVM_MEMORY_SLOTS + KVM_PRIVATE_MEM_SLOTS)
=======
#ifndef KVM_PRIVATE_MEM_SLOTS
#define KVM_PRIVATE_MEM_SLOTS 0
#endif

#ifndef KVM_MEM_SLOTS_NUM
#define KVM_MEM_SLOTS_NUM (KVM_USER_MEM_SLOTS + KVM_PRIVATE_MEM_SLOTS)
>>>>>>> refs/remotes/origin/master
#endif

/*
 * Note:
 * memslots are not sorted by id anymore, please use id_to_memslot()
 * to get the memslot by its id.
 */
struct kvm_memslots {
	u64 generation;
	struct kvm_memory_slot memslots[KVM_MEM_SLOTS_NUM];
	/* The mapping table from slot id to the index in memslots[]. */
<<<<<<< HEAD
	int id_to_index[KVM_MEM_SLOTS_NUM];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	short id_to_index[KVM_MEM_SLOTS_NUM];
>>>>>>> refs/remotes/origin/master
};

struct kvm {
	spinlock_t mmu_lock;
	struct mutex slots_lock;
	struct mm_struct *mm; /* userspace tied to this vm */
	struct kvm_memslots *memslots;
	struct srcu_struct srcu;
#ifdef CONFIG_KVM_APIC_ARCHITECTURE
	u32 bsp_vcpu_id;
<<<<<<< HEAD
<<<<<<< HEAD
	struct kvm_vcpu *bsp_vcpu;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif
	struct kvm_vcpu *vcpus[KVM_MAX_VCPUS];
	atomic_t online_vcpus;
	int last_boosted_vcpu;
	struct list_head vm_list;
	struct mutex lock;
	struct kvm_io_bus *buses[KVM_NR_BUSES];
#ifdef CONFIG_HAVE_KVM_EVENTFD
	struct {
		spinlock_t        lock;
		struct list_head  items;
<<<<<<< HEAD
=======
		struct list_head  resampler_list;
		struct mutex      resampler_lock;
>>>>>>> refs/remotes/origin/master
	} irqfds;
	struct list_head ioeventfds;
#endif
	struct kvm_vm_stat stat;
	struct kvm_arch arch;
	atomic_t users_count;
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
<<<<<<< HEAD
<<<<<<< HEAD
	struct kvm_coalesced_mmio_dev *coalesced_mmio_dev;
	struct kvm_coalesced_mmio_ring *coalesced_mmio_ring;
=======
	struct kvm_coalesced_mmio_ring *coalesced_mmio_ring;
	spinlock_t ring_lock;
	struct list_head coalesced_zones;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct kvm_coalesced_mmio_ring *coalesced_mmio_ring;
	spinlock_t ring_lock;
	struct list_head coalesced_zones;
>>>>>>> refs/remotes/origin/master
#endif

	struct mutex irq_lock;
#ifdef CONFIG_HAVE_KVM_IRQCHIP
	/*
	 * Update side is protected by irq_lock and,
	 * if configured, irqfds.lock.
	 */
	struct kvm_irq_routing_table __rcu *irq_routing;
	struct hlist_head mask_notifier_list;
	struct hlist_head irq_ack_notifier_list;
#endif

<<<<<<< HEAD
#ifdef KVM_ARCH_WANT_MMU_NOTIFIER
=======
#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
>>>>>>> refs/remotes/origin/master
	struct mmu_notifier mmu_notifier;
	unsigned long mmu_notifier_seq;
	long mmu_notifier_count;
#endif
	long tlbs_dirty;
<<<<<<< HEAD
};

/* The guest did something we don't support. */
#define pr_unimpl(vcpu, fmt, ...)					\
<<<<<<< HEAD
 do {									\
	if (printk_ratelimit())						\
		printk(KERN_ERR "kvm: %i: cpu%i " fmt,			\
		       current->tgid, (vcpu)->vcpu_id , ## __VA_ARGS__); \
 } while (0)
=======
	pr_err_ratelimited("kvm: %i: cpu%i " fmt,			\
			   current->tgid, (vcpu)->vcpu_id , ## __VA_ARGS__)
>>>>>>> refs/remotes/origin/cm-10.0

#define kvm_printf(kvm, fmt ...) printk(KERN_DEBUG fmt)
#define vcpu_printf(vcpu, fmt...) kvm_printf(vcpu->kvm, fmt)
=======
	struct list_head devices;
};

#define kvm_err(fmt, ...) \
	pr_err("kvm [%i]: " fmt, task_pid_nr(current), ## __VA_ARGS__)
#define kvm_info(fmt, ...) \
	pr_info("kvm [%i]: " fmt, task_pid_nr(current), ## __VA_ARGS__)
#define kvm_debug(fmt, ...) \
	pr_debug("kvm [%i]: " fmt, task_pid_nr(current), ## __VA_ARGS__)
#define kvm_pr_unimpl(fmt, ...) \
	pr_err_ratelimited("kvm [%i]: " fmt, \
			   task_tgid_nr(current), ## __VA_ARGS__)

/* The guest did something we don't support. */
#define vcpu_unimpl(vcpu, fmt, ...)					\
	kvm_pr_unimpl("vcpu%i " fmt, (vcpu)->vcpu_id, ## __VA_ARGS__)
>>>>>>> refs/remotes/origin/master

static inline struct kvm_vcpu *kvm_get_vcpu(struct kvm *kvm, int i)
{
	smp_rmb();
	return kvm->vcpus[i];
}

#define kvm_for_each_vcpu(idx, vcpup, kvm) \
	for (idx = 0; \
	     idx < atomic_read(&kvm->online_vcpus) && \
	     (vcpup = kvm_get_vcpu(kvm, idx)) != NULL; \
	     idx++)

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#define kvm_for_each_memslot(memslot, slots)	\
	for (memslot = &slots->memslots[0];	\
	      memslot < slots->memslots + KVM_MEM_SLOTS_NUM && memslot->npages;\
		memslot++)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
int kvm_vcpu_init(struct kvm_vcpu *vcpu, struct kvm *kvm, unsigned id);
void kvm_vcpu_uninit(struct kvm_vcpu *vcpu);

void vcpu_load(struct kvm_vcpu *vcpu);
void vcpu_put(struct kvm_vcpu *vcpu);

=======
int kvm_vcpu_init(struct kvm_vcpu *vcpu, struct kvm *kvm, unsigned id);
void kvm_vcpu_uninit(struct kvm_vcpu *vcpu);

int __must_check vcpu_load(struct kvm_vcpu *vcpu);
void vcpu_put(struct kvm_vcpu *vcpu);

#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING
int kvm_irqfd_init(void);
void kvm_irqfd_exit(void);
#else
static inline int kvm_irqfd_init(void)
{
	return 0;
}

static inline void kvm_irqfd_exit(void)
{
}
#endif
>>>>>>> refs/remotes/origin/master
int kvm_init(void *opaque, unsigned vcpu_size, unsigned vcpu_align,
		  struct module *module);
void kvm_exit(void);

void kvm_get_kvm(struct kvm *kvm);
void kvm_put_kvm(struct kvm *kvm);
<<<<<<< HEAD
<<<<<<< HEAD
=======
void update_memslots(struct kvm_memslots *slots, struct kvm_memory_slot *new);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void update_memslots(struct kvm_memslots *slots, struct kvm_memory_slot *new,
		     u64 last_generation);
>>>>>>> refs/remotes/origin/master

static inline struct kvm_memslots *kvm_memslots(struct kvm *kvm)
{
	return rcu_dereference_check(kvm->memslots,
			srcu_read_lock_held(&kvm->srcu)
			|| lockdep_is_held(&kvm->slots_lock));
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static inline struct kvm_memory_slot *
id_to_memslot(struct kvm_memslots *slots, int id)
{
	int index = slots->id_to_index[id];
	struct kvm_memory_slot *slot;

	slot = &slots->memslots[index];

	WARN_ON(slot->id != id);
	return slot;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#define HPA_MSB ((sizeof(hpa_t) * 8) - 1)
#define HPA_ERR_MASK ((hpa_t)1 << HPA_MSB)
static inline int is_error_hpa(hpa_t hpa) { return hpa >> HPA_MSB; }

extern struct page *bad_page;
<<<<<<< HEAD
extern pfn_t bad_pfn;
=======
extern struct page *fault_page;

extern pfn_t bad_pfn;
extern pfn_t fault_pfn;
>>>>>>> refs/remotes/origin/cm-10.0

int is_error_page(struct page *page);
int is_error_pfn(pfn_t pfn);
int is_hwpoison_pfn(pfn_t pfn);
int is_fault_pfn(pfn_t pfn);
<<<<<<< HEAD
=======
int is_noslot_pfn(pfn_t pfn);
int is_invalid_pfn(pfn_t pfn);
>>>>>>> refs/remotes/origin/cm-10.0
int kvm_is_error_hva(unsigned long addr);
int kvm_set_memory_region(struct kvm *kvm,
			  struct kvm_userspace_memory_region *mem,
			  int user_alloc);
int __kvm_set_memory_region(struct kvm *kvm,
			    struct kvm_userspace_memory_region *mem,
			    int user_alloc);
<<<<<<< HEAD
=======
void kvm_arch_free_memslot(struct kvm_memory_slot *free,
			   struct kvm_memory_slot *dont);
int kvm_arch_create_memslot(struct kvm_memory_slot *slot, unsigned long npages);
>>>>>>> refs/remotes/origin/cm-10.0
int kvm_arch_prepare_memory_region(struct kvm *kvm,
				struct kvm_memory_slot *memslot,
				struct kvm_memory_slot old,
				struct kvm_userspace_memory_region *mem,
				int user_alloc);
void kvm_arch_commit_memory_region(struct kvm *kvm,
				struct kvm_userspace_memory_region *mem,
				struct kvm_memory_slot old,
				int user_alloc);
<<<<<<< HEAD
=======
bool kvm_largepages_enabled(void);
>>>>>>> refs/remotes/origin/cm-10.0
void kvm_disable_largepages(void);
void kvm_arch_flush_shadow(struct kvm *kvm);
=======
/*
 * KVM_SET_USER_MEMORY_REGION ioctl allows the following operations:
 * - create a new memory slot
 * - delete an existing memory slot
 * - modify an existing memory slot
 *   -- move it in the guest physical memory space
 *   -- just change its flags
 *
 * Since flags can be changed by some of these operations, the following
 * differentiation is the best we can do for __kvm_set_memory_region():
 */
enum kvm_mr_change {
	KVM_MR_CREATE,
	KVM_MR_DELETE,
	KVM_MR_MOVE,
	KVM_MR_FLAGS_ONLY,
};

int kvm_set_memory_region(struct kvm *kvm,
			  struct kvm_userspace_memory_region *mem);
int __kvm_set_memory_region(struct kvm *kvm,
			    struct kvm_userspace_memory_region *mem);
void kvm_arch_free_memslot(struct kvm *kvm, struct kvm_memory_slot *free,
			   struct kvm_memory_slot *dont);
int kvm_arch_create_memslot(struct kvm *kvm, struct kvm_memory_slot *slot,
			    unsigned long npages);
void kvm_arch_memslots_updated(struct kvm *kvm);
int kvm_arch_prepare_memory_region(struct kvm *kvm,
				struct kvm_memory_slot *memslot,
				struct kvm_userspace_memory_region *mem,
				enum kvm_mr_change change);
void kvm_arch_commit_memory_region(struct kvm *kvm,
				struct kvm_userspace_memory_region *mem,
				const struct kvm_memory_slot *old,
				enum kvm_mr_change change);
bool kvm_largepages_enabled(void);
void kvm_disable_largepages(void);
/* flush all memory translations */
void kvm_arch_flush_shadow_all(struct kvm *kvm);
/* flush memory translations pointing to 'slot' */
void kvm_arch_flush_shadow_memslot(struct kvm *kvm,
				   struct kvm_memory_slot *slot);
>>>>>>> refs/remotes/origin/master

int gfn_to_page_many_atomic(struct kvm *kvm, gfn_t gfn, struct page **pages,
			    int nr_pages);

struct page *gfn_to_page(struct kvm *kvm, gfn_t gfn);
unsigned long gfn_to_hva(struct kvm *kvm, gfn_t gfn);
<<<<<<< HEAD
=======
unsigned long gfn_to_hva_prot(struct kvm *kvm, gfn_t gfn, bool *writable);
unsigned long gfn_to_hva_memslot(struct kvm_memory_slot *slot, gfn_t gfn);
>>>>>>> refs/remotes/origin/master
void kvm_release_page_clean(struct page *page);
void kvm_release_page_dirty(struct page *page);
void kvm_set_page_dirty(struct page *page);
void kvm_set_page_accessed(struct page *page);

<<<<<<< HEAD
pfn_t hva_to_pfn_atomic(struct kvm *kvm, unsigned long addr);
=======
>>>>>>> refs/remotes/origin/master
pfn_t gfn_to_pfn_atomic(struct kvm *kvm, gfn_t gfn);
pfn_t gfn_to_pfn_async(struct kvm *kvm, gfn_t gfn, bool *async,
		       bool write_fault, bool *writable);
pfn_t gfn_to_pfn(struct kvm *kvm, gfn_t gfn);
pfn_t gfn_to_pfn_prot(struct kvm *kvm, gfn_t gfn, bool write_fault,
		      bool *writable);
<<<<<<< HEAD
pfn_t gfn_to_pfn_memslot(struct kvm *kvm,
			 struct kvm_memory_slot *slot, gfn_t gfn);
void kvm_release_pfn_dirty(pfn_t);
=======
pfn_t gfn_to_pfn_memslot(struct kvm_memory_slot *slot, gfn_t gfn);
pfn_t gfn_to_pfn_memslot_atomic(struct kvm_memory_slot *slot, gfn_t gfn);

void kvm_release_pfn_dirty(pfn_t pfn);
>>>>>>> refs/remotes/origin/master
void kvm_release_pfn_clean(pfn_t pfn);
void kvm_set_pfn_dirty(pfn_t pfn);
void kvm_set_pfn_accessed(pfn_t pfn);
void kvm_get_pfn(pfn_t pfn);

int kvm_read_guest_page(struct kvm *kvm, gfn_t gfn, void *data, int offset,
			int len);
int kvm_read_guest_atomic(struct kvm *kvm, gpa_t gpa, void *data,
			  unsigned long len);
int kvm_read_guest(struct kvm *kvm, gpa_t gpa, void *data, unsigned long len);
<<<<<<< HEAD
<<<<<<< HEAD
=======
int kvm_read_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			   void *data, unsigned long len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int kvm_read_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			   void *data, unsigned long len);
>>>>>>> refs/remotes/origin/master
int kvm_write_guest_page(struct kvm *kvm, gfn_t gfn, const void *data,
			 int offset, int len);
int kvm_write_guest(struct kvm *kvm, gpa_t gpa, const void *data,
		    unsigned long len);
int kvm_write_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			   void *data, unsigned long len);
int kvm_gfn_to_hva_cache_init(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			      gpa_t gpa, unsigned long len);
int kvm_clear_guest_page(struct kvm *kvm, gfn_t gfn, int offset, int len);
int kvm_clear_guest(struct kvm *kvm, gpa_t gpa, unsigned long len);
struct kvm_memory_slot *gfn_to_memslot(struct kvm *kvm, gfn_t gfn);
int kvm_is_visible_gfn(struct kvm *kvm, gfn_t gfn);
unsigned long kvm_host_page_size(struct kvm *kvm, gfn_t gfn);
void mark_page_dirty(struct kvm *kvm, gfn_t gfn);
void mark_page_dirty_in_slot(struct kvm *kvm, struct kvm_memory_slot *memslot,
			     gfn_t gfn);

void kvm_vcpu_block(struct kvm_vcpu *vcpu);
<<<<<<< HEAD
=======
void kvm_vcpu_kick(struct kvm_vcpu *vcpu);
bool kvm_vcpu_yield_to(struct kvm_vcpu *target);
>>>>>>> refs/remotes/origin/master
void kvm_vcpu_on_spin(struct kvm_vcpu *vcpu);
void kvm_resched(struct kvm_vcpu *vcpu);
void kvm_load_guest_fpu(struct kvm_vcpu *vcpu);
void kvm_put_guest_fpu(struct kvm_vcpu *vcpu);

void kvm_flush_remote_tlbs(struct kvm *kvm);
void kvm_reload_remote_mmus(struct kvm *kvm);
<<<<<<< HEAD
=======
void kvm_make_mclock_inprogress_request(struct kvm *kvm);
void kvm_make_scan_ioapic_request(struct kvm *kvm);
>>>>>>> refs/remotes/origin/master

long kvm_arch_dev_ioctl(struct file *filp,
			unsigned int ioctl, unsigned long arg);
long kvm_arch_vcpu_ioctl(struct file *filp,
			 unsigned int ioctl, unsigned long arg);
<<<<<<< HEAD
<<<<<<< HEAD
=======
int kvm_arch_vcpu_fault(struct kvm_vcpu *vcpu, struct vm_fault *vmf);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int kvm_arch_vcpu_fault(struct kvm_vcpu *vcpu, struct vm_fault *vmf);
>>>>>>> refs/remotes/origin/master

int kvm_dev_ioctl_check_extension(long ext);

int kvm_get_dirty_log(struct kvm *kvm,
			struct kvm_dirty_log *log, int *is_dirty);
int kvm_vm_ioctl_get_dirty_log(struct kvm *kvm,
				struct kvm_dirty_log *log);

int kvm_vm_ioctl_set_memory_region(struct kvm *kvm,
<<<<<<< HEAD
				   struct
				   kvm_userspace_memory_region *mem,
				   int user_alloc);
=======
				   struct kvm_userspace_memory_region *mem);
int kvm_vm_ioctl_irq_line(struct kvm *kvm, struct kvm_irq_level *irq_level,
			bool line_status);
>>>>>>> refs/remotes/origin/master
long kvm_arch_vm_ioctl(struct file *filp,
		       unsigned int ioctl, unsigned long arg);

int kvm_arch_vcpu_ioctl_get_fpu(struct kvm_vcpu *vcpu, struct kvm_fpu *fpu);
int kvm_arch_vcpu_ioctl_set_fpu(struct kvm_vcpu *vcpu, struct kvm_fpu *fpu);

int kvm_arch_vcpu_ioctl_translate(struct kvm_vcpu *vcpu,
				    struct kvm_translation *tr);

int kvm_arch_vcpu_ioctl_get_regs(struct kvm_vcpu *vcpu, struct kvm_regs *regs);
int kvm_arch_vcpu_ioctl_set_regs(struct kvm_vcpu *vcpu, struct kvm_regs *regs);
int kvm_arch_vcpu_ioctl_get_sregs(struct kvm_vcpu *vcpu,
				  struct kvm_sregs *sregs);
int kvm_arch_vcpu_ioctl_set_sregs(struct kvm_vcpu *vcpu,
				  struct kvm_sregs *sregs);
int kvm_arch_vcpu_ioctl_get_mpstate(struct kvm_vcpu *vcpu,
				    struct kvm_mp_state *mp_state);
int kvm_arch_vcpu_ioctl_set_mpstate(struct kvm_vcpu *vcpu,
				    struct kvm_mp_state *mp_state);
int kvm_arch_vcpu_ioctl_set_guest_debug(struct kvm_vcpu *vcpu,
					struct kvm_guest_debug *dbg);
int kvm_arch_vcpu_ioctl_run(struct kvm_vcpu *vcpu, struct kvm_run *kvm_run);

int kvm_arch_init(void *opaque);
void kvm_arch_exit(void);

int kvm_arch_vcpu_init(struct kvm_vcpu *vcpu);
void kvm_arch_vcpu_uninit(struct kvm_vcpu *vcpu);

void kvm_arch_vcpu_free(struct kvm_vcpu *vcpu);
void kvm_arch_vcpu_load(struct kvm_vcpu *vcpu, int cpu);
void kvm_arch_vcpu_put(struct kvm_vcpu *vcpu);
struct kvm_vcpu *kvm_arch_vcpu_create(struct kvm *kvm, unsigned int id);
int kvm_arch_vcpu_setup(struct kvm_vcpu *vcpu);
<<<<<<< HEAD
void kvm_arch_vcpu_destroy(struct kvm_vcpu *vcpu);

int kvm_arch_vcpu_reset(struct kvm_vcpu *vcpu);
=======
int kvm_arch_vcpu_postcreate(struct kvm_vcpu *vcpu);
void kvm_arch_vcpu_destroy(struct kvm_vcpu *vcpu);

>>>>>>> refs/remotes/origin/master
int kvm_arch_hardware_enable(void *garbage);
void kvm_arch_hardware_disable(void *garbage);
int kvm_arch_hardware_setup(void);
void kvm_arch_hardware_unsetup(void);
void kvm_arch_check_processor_compat(void *rtn);
int kvm_arch_vcpu_runnable(struct kvm_vcpu *vcpu);
<<<<<<< HEAD

void kvm_free_physmem(struct kvm *kvm);

=======
int kvm_arch_vcpu_should_kick(struct kvm_vcpu *vcpu);

void kvm_free_physmem(struct kvm *kvm);

void *kvm_kvzalloc(unsigned long size);
void kvm_kvfree(const void *addr);

>>>>>>> refs/remotes/origin/master
#ifndef __KVM_HAVE_ARCH_VM_ALLOC
static inline struct kvm *kvm_arch_alloc_vm(void)
{
	return kzalloc(sizeof(struct kvm), GFP_KERNEL);
}

static inline void kvm_arch_free_vm(struct kvm *kvm)
{
	kfree(kvm);
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
int kvm_arch_init_vm(struct kvm *kvm);
=======
int kvm_arch_init_vm(struct kvm *kvm, unsigned long type);
>>>>>>> refs/remotes/origin/cm-10.0
void kvm_arch_destroy_vm(struct kvm *kvm);
void kvm_free_all_assigned_devices(struct kvm *kvm);
=======
#ifdef __KVM_HAVE_ARCH_NONCOHERENT_DMA
void kvm_arch_register_noncoherent_dma(struct kvm *kvm);
void kvm_arch_unregister_noncoherent_dma(struct kvm *kvm);
bool kvm_arch_has_noncoherent_dma(struct kvm *kvm);
#else
static inline void kvm_arch_register_noncoherent_dma(struct kvm *kvm)
{
}

static inline void kvm_arch_unregister_noncoherent_dma(struct kvm *kvm)
{
}

static inline bool kvm_arch_has_noncoherent_dma(struct kvm *kvm)
{
	return false;
}
#endif

static inline wait_queue_head_t *kvm_arch_vcpu_wq(struct kvm_vcpu *vcpu)
{
#ifdef __KVM_HAVE_ARCH_WQP
	return vcpu->arch.wqp;
#else
	return &vcpu->wq;
#endif
}

int kvm_arch_init_vm(struct kvm *kvm, unsigned long type);
void kvm_arch_destroy_vm(struct kvm *kvm);
>>>>>>> refs/remotes/origin/master
void kvm_arch_sync_events(struct kvm *kvm);

int kvm_cpu_has_pending_timer(struct kvm_vcpu *vcpu);
void kvm_vcpu_kick(struct kvm_vcpu *vcpu);

<<<<<<< HEAD
int kvm_is_mmio_pfn(pfn_t pfn);
=======
bool kvm_is_mmio_pfn(pfn_t pfn);
>>>>>>> refs/remotes/origin/master

struct kvm_irq_ack_notifier {
	struct hlist_node link;
	unsigned gsi;
	void (*irq_acked)(struct kvm_irq_ack_notifier *kian);
};

struct kvm_assigned_dev_kernel {
	struct kvm_irq_ack_notifier ack_notifier;
	struct list_head list;
	int assigned_dev_id;
	int host_segnr;
	int host_busnr;
	int host_devfn;
	unsigned int entries_nr;
	int host_irq;
	bool host_irq_disabled;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool pci_2_3;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool pci_2_3;
>>>>>>> refs/remotes/origin/master
	struct msix_entry *host_msix_entries;
	int guest_irq;
	struct msix_entry *guest_msix_entries;
	unsigned long irq_requested_type;
	int irq_source_id;
	int flags;
	struct pci_dev *dev;
	struct kvm *kvm;
	spinlock_t intx_lock;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	spinlock_t intx_mask_lock;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spinlock_t intx_mask_lock;
>>>>>>> refs/remotes/origin/master
	char irq_name[32];
	struct pci_saved_state *pci_saved_state;
};

struct kvm_irq_mask_notifier {
	void (*func)(struct kvm_irq_mask_notifier *kimn, bool masked);
	int irq;
	struct hlist_node link;
};

void kvm_register_irq_mask_notifier(struct kvm *kvm, int irq,
				    struct kvm_irq_mask_notifier *kimn);
void kvm_unregister_irq_mask_notifier(struct kvm *kvm, int irq,
				      struct kvm_irq_mask_notifier *kimn);
void kvm_fire_mask_notifiers(struct kvm *kvm, unsigned irqchip, unsigned pin,
			     bool mask);

<<<<<<< HEAD
#ifdef __KVM_HAVE_IOAPIC
void kvm_get_intr_delivery_bitmask(struct kvm_ioapic *ioapic,
				   union kvm_ioapic_redirect_entry *entry,
				   unsigned long *deliver_bitmask);
#endif
int kvm_set_irq(struct kvm *kvm, int irq_source_id, u32 irq, int level);
int kvm_set_msi(struct kvm_kernel_irq_routing_entry *irq_entry, struct kvm *kvm,
		int irq_source_id, int level);
=======
int kvm_set_irq(struct kvm *kvm, int irq_source_id, u32 irq, int level,
		bool line_status);
int kvm_set_irq_inatomic(struct kvm *kvm, int irq_source_id, u32 irq, int level);
int kvm_set_msi(struct kvm_kernel_irq_routing_entry *irq_entry, struct kvm *kvm,
		int irq_source_id, int level, bool line_status);
bool kvm_irq_has_notifier(struct kvm *kvm, unsigned irqchip, unsigned pin);
>>>>>>> refs/remotes/origin/master
void kvm_notify_acked_irq(struct kvm *kvm, unsigned irqchip, unsigned pin);
void kvm_register_irq_ack_notifier(struct kvm *kvm,
				   struct kvm_irq_ack_notifier *kian);
void kvm_unregister_irq_ack_notifier(struct kvm *kvm,
				   struct kvm_irq_ack_notifier *kian);
int kvm_request_irq_source_id(struct kvm *kvm);
void kvm_free_irq_source_id(struct kvm *kvm, int irq_source_id);

<<<<<<< HEAD
/* For vcpu->arch.iommu_flags */
#define KVM_IOMMU_CACHE_COHERENCY	0x1

#ifdef CONFIG_IOMMU_API
=======
#ifdef CONFIG_KVM_DEVICE_ASSIGNMENT
>>>>>>> refs/remotes/origin/master
int kvm_iommu_map_pages(struct kvm *kvm, struct kvm_memory_slot *slot);
void kvm_iommu_unmap_pages(struct kvm *kvm, struct kvm_memory_slot *slot);
int kvm_iommu_map_guest(struct kvm *kvm);
int kvm_iommu_unmap_guest(struct kvm *kvm);
int kvm_assign_device(struct kvm *kvm,
		      struct kvm_assigned_dev_kernel *assigned_dev);
int kvm_deassign_device(struct kvm *kvm,
			struct kvm_assigned_dev_kernel *assigned_dev);
<<<<<<< HEAD
#else /* CONFIG_IOMMU_API */
=======
#else
>>>>>>> refs/remotes/origin/master
static inline int kvm_iommu_map_pages(struct kvm *kvm,
				      struct kvm_memory_slot *slot)
{
	return 0;
}

static inline void kvm_iommu_unmap_pages(struct kvm *kvm,
					 struct kvm_memory_slot *slot)
{
}

<<<<<<< HEAD
static inline int kvm_iommu_map_guest(struct kvm *kvm)
{
	return -ENODEV;
}

=======
>>>>>>> refs/remotes/origin/master
static inline int kvm_iommu_unmap_guest(struct kvm *kvm)
{
	return 0;
}
<<<<<<< HEAD

static inline int kvm_assign_device(struct kvm *kvm,
		struct kvm_assigned_dev_kernel *assigned_dev)
{
	return 0;
}

static inline int kvm_deassign_device(struct kvm *kvm,
		struct kvm_assigned_dev_kernel *assigned_dev)
{
	return 0;
}
#endif /* CONFIG_IOMMU_API */

static inline void kvm_guest_enter(void)
{
	BUG_ON(preemptible());
	account_system_vtime(current);
	current->flags |= PF_VCPU;
	/* KVM does not hold any references to rcu protected data when it
	 * switches CPU into a guest mode. In fact switching to a guest mode
	 * is very similar to exiting to userspase from rcu point of view. In
=======
#endif

static inline void kvm_guest_enter(void)
{
	unsigned long flags;

	BUG_ON(preemptible());

	local_irq_save(flags);
	guest_enter();
	local_irq_restore(flags);

	/* KVM does not hold any references to rcu protected data when it
	 * switches CPU into a guest mode. In fact switching to a guest mode
	 * is very similar to exiting to userspace from rcu point of view. In
>>>>>>> refs/remotes/origin/master
	 * addition CPU may stay in a guest mode for quite a long time (up to
	 * one time slice). Lets treat guest mode as quiescent state, just like
	 * we do with user-mode execution.
	 */
	rcu_virt_note_context_switch(smp_processor_id());
}

static inline void kvm_guest_exit(void)
{
<<<<<<< HEAD
	account_system_vtime(current);
	current->flags &= ~PF_VCPU;
}

<<<<<<< HEAD
=======
=======
	unsigned long flags;

	local_irq_save(flags);
	guest_exit();
	local_irq_restore(flags);
}

>>>>>>> refs/remotes/origin/master
/*
 * search_memslots() and __gfn_to_memslot() are here because they are
 * used in non-modular code in arch/powerpc/kvm/book3s_hv_rm_mmu.c.
 * gfn_to_memslot() itself isn't here as an inline because that would
 * bloat other code too much.
 */
static inline struct kvm_memory_slot *
search_memslots(struct kvm_memslots *slots, gfn_t gfn)
{
	struct kvm_memory_slot *memslot;

	kvm_for_each_memslot(memslot, slots)
		if (gfn >= memslot->base_gfn &&
		      gfn < memslot->base_gfn + memslot->npages)
			return memslot;

	return NULL;
}

static inline struct kvm_memory_slot *
__gfn_to_memslot(struct kvm_memslots *slots, gfn_t gfn)
{
	return search_memslots(slots, gfn);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static inline int memslot_id(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_memslot(kvm, gfn)->id;
}

<<<<<<< HEAD
=======
static inline gfn_t gfn_to_index(gfn_t gfn, gfn_t base_gfn, int level)
{
	/* KVM_HPAGE_GFN_SHIFT(PT_PAGE_TABLE_LEVEL) must be 0. */
	return (gfn >> KVM_HPAGE_GFN_SHIFT(level)) -
		(base_gfn >> KVM_HPAGE_GFN_SHIFT(level));
}

>>>>>>> refs/remotes/origin/cm-10.0
static inline unsigned long gfn_to_hva_memslot(struct kvm_memory_slot *slot,
					       gfn_t gfn)
{
	return slot->userspace_addr + (gfn - slot->base_gfn) * PAGE_SIZE;
=======
static inline unsigned long
__gfn_to_hva_memslot(struct kvm_memory_slot *slot, gfn_t gfn)
{
	return slot->userspace_addr + (gfn - slot->base_gfn) * PAGE_SIZE;
}

static inline int memslot_id(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_memslot(kvm, gfn)->id;
}

static inline gfn_t
hva_to_gfn_memslot(unsigned long hva, struct kvm_memory_slot *slot)
{
	gfn_t gfn_offset = (hva - slot->userspace_addr) >> PAGE_SHIFT;

	return slot->base_gfn + gfn_offset;
>>>>>>> refs/remotes/origin/master
}

static inline gpa_t gfn_to_gpa(gfn_t gfn)
{
	return (gpa_t)gfn << PAGE_SHIFT;
}

static inline gfn_t gpa_to_gfn(gpa_t gpa)
{
	return (gfn_t)(gpa >> PAGE_SHIFT);
}

static inline hpa_t pfn_to_hpa(pfn_t pfn)
{
	return (hpa_t)pfn << PAGE_SHIFT;
}

static inline void kvm_migrate_timers(struct kvm_vcpu *vcpu)
{
	set_bit(KVM_REQ_MIGRATE_TIMER, &vcpu->requests);
}

enum kvm_stat_kind {
	KVM_STAT_VM,
	KVM_STAT_VCPU,
};

struct kvm_stats_debugfs_item {
	const char *name;
	int offset;
	enum kvm_stat_kind kind;
	struct dentry *dentry;
};
extern struct kvm_stats_debugfs_item debugfs_entries[];
extern struct dentry *kvm_debugfs_dir;

<<<<<<< HEAD
#ifdef KVM_ARCH_WANT_MMU_NOTIFIER
static inline int mmu_notifier_retry(struct kvm_vcpu *vcpu, unsigned long mmu_seq)
{
	if (unlikely(vcpu->kvm->mmu_notifier_count))
		return 1;
	/*
<<<<<<< HEAD
	 * Both reads happen under the mmu_lock and both values are
	 * modified under mmu_lock, so there's no need of smb_rmb()
	 * here in between, otherwise mmu_notifier_count should be
	 * read before mmu_notifier_seq, see
	 * mmu_notifier_invalidate_range_end write side.
	 */
=======
=======
#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
static inline int mmu_notifier_retry(struct kvm *kvm, unsigned long mmu_seq)
{
	if (unlikely(kvm->mmu_notifier_count))
		return 1;
	/*
>>>>>>> refs/remotes/origin/master
	 * Ensure the read of mmu_notifier_count happens before the read
	 * of mmu_notifier_seq.  This interacts with the smp_wmb() in
	 * mmu_notifier_invalidate_range_end to make sure that the caller
	 * either sees the old (non-zero) value of mmu_notifier_count or
	 * the new (incremented) value of mmu_notifier_seq.
	 * PowerPC Book3s HV KVM calls this under a per-page lock
	 * rather than under kvm->mmu_lock, for scalability, so
	 * can't rely on kvm->mmu_lock to keep things ordered.
	 */
	smp_rmb();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (vcpu->kvm->mmu_notifier_seq != mmu_seq)
=======
	if (kvm->mmu_notifier_seq != mmu_seq)
>>>>>>> refs/remotes/origin/master
		return 1;
	return 0;
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_HAVE_KVM_IRQCHIP
=======
#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING
>>>>>>> refs/remotes/origin/master

#define KVM_MAX_IRQ_ROUTES 1024

int kvm_setup_default_irq_routing(struct kvm *kvm);
int kvm_set_irq_routing(struct kvm *kvm,
			const struct kvm_irq_routing_entry *entries,
			unsigned nr,
			unsigned flags);
<<<<<<< HEAD
void kvm_free_irq_routing(struct kvm *kvm);

=======
int kvm_set_routing_entry(struct kvm_irq_routing_table *rt,
			  struct kvm_kernel_irq_routing_entry *e,
			  const struct kvm_irq_routing_entry *ue);
void kvm_free_irq_routing(struct kvm *kvm);

int kvm_send_userspace_msi(struct kvm *kvm, struct kvm_msi *msi);

>>>>>>> refs/remotes/origin/master
#else

static inline void kvm_free_irq_routing(struct kvm *kvm) {}

#endif

#ifdef CONFIG_HAVE_KVM_EVENTFD

void kvm_eventfd_init(struct kvm *kvm);
<<<<<<< HEAD
int kvm_irqfd(struct kvm *kvm, int fd, int gsi, int flags);
void kvm_irqfd_release(struct kvm *kvm);
void kvm_irq_routing_update(struct kvm *, struct kvm_irq_routing_table *);
int kvm_ioeventfd(struct kvm *kvm, struct kvm_ioeventfd *args);
=======
int kvm_ioeventfd(struct kvm *kvm, struct kvm_ioeventfd *args);

#ifdef CONFIG_HAVE_KVM_IRQCHIP
int kvm_irqfd(struct kvm *kvm, struct kvm_irqfd *args);
void kvm_irqfd_release(struct kvm *kvm);
void kvm_irq_routing_update(struct kvm *, struct kvm_irq_routing_table *);
#else
static inline int kvm_irqfd(struct kvm *kvm, struct kvm_irqfd *args)
{
	return -EINVAL;
}

static inline void kvm_irqfd_release(struct kvm *kvm) {}
#endif
>>>>>>> refs/remotes/origin/master

#else

static inline void kvm_eventfd_init(struct kvm *kvm) {}

<<<<<<< HEAD
static inline int kvm_irqfd(struct kvm *kvm, int fd, int gsi, int flags)
=======
static inline int kvm_irqfd(struct kvm *kvm, struct kvm_irqfd *args)
>>>>>>> refs/remotes/origin/master
{
	return -EINVAL;
}

static inline void kvm_irqfd_release(struct kvm *kvm) {}

#ifdef CONFIG_HAVE_KVM_IRQCHIP
static inline void kvm_irq_routing_update(struct kvm *kvm,
					  struct kvm_irq_routing_table *irq_rt)
{
	rcu_assign_pointer(kvm->irq_routing, irq_rt);
}
#endif

static inline int kvm_ioeventfd(struct kvm *kvm, struct kvm_ioeventfd *args)
{
	return -ENOSYS;
}

#endif /* CONFIG_HAVE_KVM_EVENTFD */

#ifdef CONFIG_KVM_APIC_ARCHITECTURE
static inline bool kvm_vcpu_is_bsp(struct kvm_vcpu *vcpu)
{
	return vcpu->kvm->bsp_vcpu_id == vcpu->vcpu_id;
}

bool kvm_vcpu_compatible(struct kvm_vcpu *vcpu);

#else

static inline bool kvm_vcpu_compatible(struct kvm_vcpu *vcpu) { return true; }

#endif

<<<<<<< HEAD
#ifdef __KVM_HAVE_DEVICE_ASSIGNMENT
=======
#ifdef CONFIG_KVM_DEVICE_ASSIGNMENT
>>>>>>> refs/remotes/origin/master

long kvm_vm_ioctl_assigned_device(struct kvm *kvm, unsigned ioctl,
				  unsigned long arg);

<<<<<<< HEAD
=======
void kvm_free_all_assigned_devices(struct kvm *kvm);

>>>>>>> refs/remotes/origin/master
#else

static inline long kvm_vm_ioctl_assigned_device(struct kvm *kvm, unsigned ioctl,
						unsigned long arg)
{
	return -ENOTTY;
}

<<<<<<< HEAD
=======
static inline void kvm_free_all_assigned_devices(struct kvm *kvm) {}

>>>>>>> refs/remotes/origin/master
#endif

static inline void kvm_make_request(int req, struct kvm_vcpu *vcpu)
{
	set_bit(req, &vcpu->requests);
}

static inline bool kvm_check_request(int req, struct kvm_vcpu *vcpu)
{
	if (test_bit(req, &vcpu->requests)) {
		clear_bit(req, &vcpu->requests);
		return true;
	} else {
		return false;
	}
}

<<<<<<< HEAD
=======
extern bool kvm_rebooting;

struct kvm_device_ops;

struct kvm_device {
	struct kvm_device_ops *ops;
	struct kvm *kvm;
	void *private;
	struct list_head vm_node;
};

/* create, destroy, and name are mandatory */
struct kvm_device_ops {
	const char *name;
	int (*create)(struct kvm_device *dev, u32 type);

	/*
	 * Destroy is responsible for freeing dev.
	 *
	 * Destroy may be called before or after destructors are called
	 * on emulated I/O regions, depending on whether a reference is
	 * held by a vcpu or other kvm component that gets destroyed
	 * after the emulated I/O.
	 */
	void (*destroy)(struct kvm_device *dev);

	int (*set_attr)(struct kvm_device *dev, struct kvm_device_attr *attr);
	int (*get_attr)(struct kvm_device *dev, struct kvm_device_attr *attr);
	int (*has_attr)(struct kvm_device *dev, struct kvm_device_attr *attr);
	long (*ioctl)(struct kvm_device *dev, unsigned int ioctl,
		      unsigned long arg);
};

void kvm_device_get(struct kvm_device *dev);
void kvm_device_put(struct kvm_device *dev);
struct kvm_device *kvm_device_from_filp(struct file *filp);

extern struct kvm_device_ops kvm_mpic_ops;
extern struct kvm_device_ops kvm_xics_ops;
extern struct kvm_device_ops kvm_vfio_ops;

#ifdef CONFIG_HAVE_KVM_CPU_RELAX_INTERCEPT

static inline void kvm_vcpu_set_in_spin_loop(struct kvm_vcpu *vcpu, bool val)
{
	vcpu->spin_loop.in_spin_loop = val;
}
static inline void kvm_vcpu_set_dy_eligible(struct kvm_vcpu *vcpu, bool val)
{
	vcpu->spin_loop.dy_eligible = val;
}

#else /* !CONFIG_HAVE_KVM_CPU_RELAX_INTERCEPT */

static inline void kvm_vcpu_set_in_spin_loop(struct kvm_vcpu *vcpu, bool val)
{
}

static inline void kvm_vcpu_set_dy_eligible(struct kvm_vcpu *vcpu, bool val)
{
}

static inline bool kvm_vcpu_eligible_for_directed_yield(struct kvm_vcpu *vcpu)
{
	return true;
}

#endif /* CONFIG_HAVE_KVM_CPU_RELAX_INTERCEPT */
>>>>>>> refs/remotes/origin/master
#endif

