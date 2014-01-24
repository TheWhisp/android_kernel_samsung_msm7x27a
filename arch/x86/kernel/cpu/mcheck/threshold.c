/*
 * Common corrected MCE threshold handler code:
 */
#include <linux/interrupt.h>
#include <linux/kernel.h>

#include <asm/irq_vectors.h>
#include <asm/apic.h>
#include <asm/idle.h>
#include <asm/mce.h>
<<<<<<< HEAD
=======
#include <asm/trace/irq_vectors.h>
>>>>>>> refs/remotes/origin/master

static void default_threshold_interrupt(void)
{
	printk(KERN_ERR "Unexpected threshold interrupt at vector %x\n",
			 THRESHOLD_APIC_VECTOR);
}

void (*mce_threshold_vector)(void) = default_threshold_interrupt;

<<<<<<< HEAD
asmlinkage void smp_threshold_interrupt(void)
{
<<<<<<< HEAD
	exit_idle();
	irq_enter();
=======
	irq_enter();
	exit_idle();
>>>>>>> refs/remotes/origin/cm-10.0
	inc_irq_stat(irq_threshold_count);
	mce_threshold_vector();
	irq_exit();
	/* Ack only at the end to avoid potential reentry */
	ack_APIC_irq();
=======
static inline void __smp_threshold_interrupt(void)
{
	inc_irq_stat(irq_threshold_count);
	mce_threshold_vector();
}

asmlinkage void smp_threshold_interrupt(void)
{
	entering_irq();
	__smp_threshold_interrupt();
	exiting_ack_irq();
}

asmlinkage void smp_trace_threshold_interrupt(void)
{
	entering_irq();
	trace_threshold_apic_entry(THRESHOLD_APIC_VECTOR);
	__smp_threshold_interrupt();
	trace_threshold_apic_exit(THRESHOLD_APIC_VECTOR);
	exiting_ack_irq();
>>>>>>> refs/remotes/origin/master
}
