#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

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
struct ext_code {
	unsigned short subcode;
	unsigned short code;
};

typedef void (*ext_int_handler_t)(struct ext_code, unsigned int, unsigned long);
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

#endif /* _ASM_IRQ_H */
