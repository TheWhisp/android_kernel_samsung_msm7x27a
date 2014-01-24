#include <linux/interrupt.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <asm/irq.h>
#include <asm/traps.h>
#include <asm/apollohw.h>

void dn_process_int(unsigned int irq, struct pt_regs *fp)
{
	__m68k_handle_int(irq, fp);

	*(volatile unsigned char *)(pica)=0x20;
	*(volatile unsigned char *)(picb)=0x20;
}

int apollo_irq_startup(unsigned int irq)
{
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/irq.h>

#include <asm/traps.h>
#include <asm/apollohw.h>

unsigned int apollo_irq_startup(struct irq_data *data)
{
	unsigned int irq = data->irq;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (irq < 8)
		*(volatile unsigned char *)(pica+1) &= ~(1 << irq);
	else
		*(volatile unsigned char *)(picb+1) &= ~(1 << (irq - 8));
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
void apollo_irq_shutdown(unsigned int irq)
{
=======
=======
>>>>>>> refs/remotes/origin/master
void apollo_irq_shutdown(struct irq_data *data)
{
	unsigned int irq = data->irq;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (irq < 8)
		*(volatile unsigned char *)(pica+1) |= (1 << irq);
	else
		*(volatile unsigned char *)(picb+1) |= (1 << (irq - 8));
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct irq_controller apollo_irq_controller = {
	.name           = "apollo",
	.lock           = __SPIN_LOCK_UNLOCKED(apollo_irq_controller.lock),
	.startup        = apollo_irq_startup,
	.shutdown       = apollo_irq_shutdown,
=======
=======
>>>>>>> refs/remotes/origin/master
void apollo_irq_eoi(struct irq_data *data)
{
	*(volatile unsigned char *)(pica) = 0x20;
	*(volatile unsigned char *)(picb) = 0x20;
}

static struct irq_chip apollo_irq_chip = {
	.name           = "apollo",
	.irq_startup    = apollo_irq_startup,
	.irq_shutdown   = apollo_irq_shutdown,
	.irq_eoi	= apollo_irq_eoi,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};


void __init dn_init_IRQ(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	m68k_setup_user_interrupt(VEC_USER + 96, 16, dn_process_int);
	m68k_setup_irq_controller(&apollo_irq_controller, IRQ_APOLLO, 16);
=======
	m68k_setup_user_interrupt(VEC_USER + 96, 16);
	m68k_setup_irq_controller(&apollo_irq_chip, handle_fasteoi_irq,
				  IRQ_APOLLO, 16);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	m68k_setup_user_interrupt(VEC_USER + 96, 16);
	m68k_setup_irq_controller(&apollo_irq_chip, handle_fasteoi_irq,
				  IRQ_APOLLO, 16);
>>>>>>> refs/remotes/origin/master
}
