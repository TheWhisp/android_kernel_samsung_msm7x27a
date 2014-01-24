#ifndef _ASM_X86_XEN_EVENTS_H
#define _ASM_X86_XEN_EVENTS_H

enum ipi_vector {
	XEN_RESCHEDULE_VECTOR,
	XEN_CALL_FUNCTION_VECTOR,
	XEN_CALL_FUNCTION_SINGLE_VECTOR,
	XEN_SPIN_UNLOCK_VECTOR,
<<<<<<< HEAD
=======
	XEN_IRQ_WORK_VECTOR,
	XEN_NMI_VECTOR,
>>>>>>> refs/remotes/origin/master

	XEN_NR_IPIS,
};

static inline int xen_irqs_disabled(struct pt_regs *regs)
{
	return raw_irqs_disabled_flags(regs->flags);
}

<<<<<<< HEAD
=======
/* No need for a barrier -- XCHG is a barrier on x86. */
#define xchg_xen_ulong(ptr, val) xchg((ptr), (val))

>>>>>>> refs/remotes/origin/master
#endif /* _ASM_X86_XEN_EVENTS_H */
