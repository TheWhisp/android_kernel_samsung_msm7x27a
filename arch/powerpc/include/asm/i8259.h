#ifndef _ASM_POWERPC_I8259_H
#define _ASM_POWERPC_I8259_H
#ifdef __KERNEL__

#include <linux/irq.h>

extern void i8259_init(struct device_node *node, unsigned long intack_addr);
extern unsigned int i8259_irq(void);
<<<<<<< HEAD
<<<<<<< HEAD
extern struct irq_host *i8259_get_host(void);
=======
extern struct irq_domain *i8259_get_host(void);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern struct irq_domain *i8259_get_host(void);
>>>>>>> refs/remotes/origin/master

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_I8259_H */
