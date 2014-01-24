#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

<<<<<<< HEAD
#include <linux/hardirq.h>
#include <linux/types.h>

enum interruption_class {
	EXTERNAL_INTERRUPT,
	IO_INTERRUPT,
	EXTINT_CLK,
<<<<<<< HEAD
	EXTINT_IPI,
=======
	EXTINT_EXC,
	EXTINT_EMS,
>>>>>>> refs/remotes/origin/cm-10.0
	EXTINT_TMR,
	EXTINT_TLA,
	EXTINT_PFL,
	EXTINT_DSD,
	EXTINT_VRT,
	EXTINT_SCP,
	EXTINT_IUC,
	EXTINT_CPM,
<<<<<<< HEAD
	IOINT_QAI,
	IOINT_QDI,
=======
	IOINT_CIO,
	IOINT_QAI,
>>>>>>> refs/remotes/origin/cm-10.0
	IOINT_DAS,
	IOINT_C15,
	IOINT_C70,
	IOINT_TAP,
	IOINT_VMR,
	IOINT_LCS,
	IOINT_CLW,
	IOINT_CTC,
	IOINT_APB,
<<<<<<< HEAD
=======
	IOINT_CSC,
>>>>>>> refs/remotes/origin/cm-10.0
	NMI_NMI,
	NR_IRQS,
};

<<<<<<< HEAD
typedef void (*ext_int_handler_t)(unsigned int, unsigned int, unsigned long);
=======
=======
#define EXT_INTERRUPT	1
#define IO_INTERRUPT	2
#define THIN_INTERRUPT	3

#define NR_IRQS_BASE	4

#ifdef CONFIG_PCI_NR_MSI
# define NR_IRQS	(NR_IRQS_BASE + CONFIG_PCI_NR_MSI)
#else
# define NR_IRQS	NR_IRQS_BASE
#endif

/* This number is used when no interrupt has been assigned */
#define NO_IRQ		0

#ifndef __ASSEMBLY__

#include <linux/hardirq.h>
#include <linux/percpu.h>
#include <linux/cache.h>
#include <linux/types.h>

enum interruption_class {
	IRQEXT_CLK,
	IRQEXT_EXC,
	IRQEXT_EMS,
	IRQEXT_TMR,
	IRQEXT_TLA,
	IRQEXT_PFL,
	IRQEXT_DSD,
	IRQEXT_VRT,
	IRQEXT_SCP,
	IRQEXT_IUC,
	IRQEXT_CMS,
	IRQEXT_CMC,
	IRQEXT_CMR,
	IRQIO_CIO,
	IRQIO_QAI,
	IRQIO_DAS,
	IRQIO_C15,
	IRQIO_C70,
	IRQIO_TAP,
	IRQIO_VMR,
	IRQIO_LCS,
	IRQIO_CLW,
	IRQIO_CTC,
	IRQIO_APB,
	IRQIO_ADM,
	IRQIO_CSC,
	IRQIO_PCI,
	IRQIO_MSI,
	IRQIO_VIR,
	NMI_NMI,
	CPU_RST,
	NR_ARCH_IRQS
};

struct irq_stat {
	unsigned int irqs[NR_ARCH_IRQS];
};

DECLARE_PER_CPU_SHARED_ALIGNED(struct irq_stat, irq_stat);

static __always_inline void inc_irq_stat(enum interruption_class irq)
{
	__get_cpu_var(irq_stat).irqs[irq]++;
}

>>>>>>> refs/remotes/origin/master
struct ext_code {
	unsigned short subcode;
	unsigned short code;
};

typedef void (*ext_int_handler_t)(struct ext_code, unsigned int, unsigned long);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

int register_external_interrupt(u16 code, ext_int_handler_t handler);
int unregister_external_interrupt(u16 code, ext_int_handler_t handler);
void service_subclass_irq_register(void);
void service_subclass_irq_unregister(void);
<<<<<<< HEAD
=======
void measurement_alert_subclass_register(void);
void measurement_alert_subclass_unregister(void);
>>>>>>> refs/remotes/origin/cm-10.0
=======

int register_external_interrupt(u16 code, ext_int_handler_t handler);
int unregister_external_interrupt(u16 code, ext_int_handler_t handler);

enum irq_subclass {
	IRQ_SUBCLASS_MEASUREMENT_ALERT = 5,
	IRQ_SUBCLASS_SERVICE_SIGNAL = 9,
};

void irq_subclass_register(enum irq_subclass subclass);
void irq_subclass_unregister(enum irq_subclass subclass);

#define irq_canonicalize(irq)  (irq)

#endif /* __ASSEMBLY__ */
>>>>>>> refs/remotes/origin/master

#endif /* _ASM_IRQ_H */
