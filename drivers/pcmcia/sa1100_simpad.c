/*
 * drivers/pcmcia/sa1100_simpad.c
 *
 * PCMCIA implementation routines for simpad
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <mach/simpad.h>
#include "sa1100_generic.h"
 
<<<<<<< HEAD
<<<<<<< HEAD
extern long get_cs3_shadow(void);
extern void set_cs3_bit(int value); 
extern void clear_cs3_bit(int value);

static struct pcmcia_irqs irqs[] = {
	{ 1, IRQ_GPIO_CF_CD, "CF_CD" },
};

static int simpad_pcmcia_hw_init(struct soc_pcmcia_socket *skt)
{

	clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);

	skt->socket.pci_irq = IRQ_GPIO_CF_IRQ;

	return soc_pcmcia_request_irqs(skt, irqs, ARRAY_SIZE(irqs));
=======
=======
>>>>>>> refs/remotes/origin/master
static int simpad_pcmcia_hw_init(struct soc_pcmcia_socket *skt)
{

	simpad_clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);

	skt->stat[SOC_STAT_CD].gpio = GPIO_CF_CD;
	skt->stat[SOC_STAT_CD].name = "CF_CD";
	skt->stat[SOC_STAT_RDY].gpio = GPIO_CF_IRQ;
	skt->stat[SOC_STAT_RDY].name = "CF_RDY";

	return 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void simpad_pcmcia_hw_shutdown(struct soc_pcmcia_socket *skt)
{
<<<<<<< HEAD
<<<<<<< HEAD
	soc_pcmcia_free_irqs(skt, irqs, ARRAY_SIZE(irqs));

	/* Disable CF bus: */
	//set_cs3_bit(PCMCIA_BUFF_DIS);
	clear_cs3_bit(PCMCIA_RESET);       
=======
	/* Disable CF bus: */
	/*simpad_set_cs3_bit(PCMCIA_BUFF_DIS);*/
	simpad_clear_cs3_bit(PCMCIA_RESET);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Disable CF bus: */
	/*simpad_set_cs3_bit(PCMCIA_BUFF_DIS);*/
	simpad_clear_cs3_bit(PCMCIA_RESET);
>>>>>>> refs/remotes/origin/master
}

static void
simpad_pcmcia_socket_state(struct soc_pcmcia_socket *skt,
			   struct pcmcia_state *state)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long levels = GPLR;
	long cs3reg = get_cs3_shadow();

	state->detect=((levels & GPIO_CF_CD)==0)?1:0;
	state->ready=(levels & GPIO_CF_IRQ)?1:0;
	state->bvd1=1; /* Not available on Simpad. */
	state->bvd2=1; /* Not available on Simpad. */
	state->wrprot=0; /* Not available on Simpad. */
  
	if((cs3reg & 0x0c) == 0x0c) {
=======
=======
>>>>>>> refs/remotes/origin/master
	long cs3reg = simpad_get_cs3_ro();

	/* the detect signal is inverted - fix that up here */
	state->detect = !state->detect;

	state->bvd1 = 1; /* Might be cs3reg & PCMCIA_BVD1 */
	state->bvd2 = 1; /* Might be cs3reg & PCMCIA_BVD2 */

	if ((cs3reg & (PCMCIA_VS1|PCMCIA_VS2)) ==
			(PCMCIA_VS1|PCMCIA_VS2)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		state->vs_3v=0;
		state->vs_Xv=0;
	} else {
		state->vs_3v=1;
		state->vs_Xv=0;
	}
}

static int
simpad_pcmcia_configure_socket(struct soc_pcmcia_socket *skt,
			       const socket_state_t *state)
{
	unsigned long flags;

	local_irq_save(flags);

	/* Murphy: see table of MIC2562a-1 */
	switch (state->Vcc) {
	case 0:
<<<<<<< HEAD
<<<<<<< HEAD
		clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);
		break;

	case 33:  
		clear_cs3_bit(VCC_3V_EN|EN1);
		set_cs3_bit(VCC_5V_EN|EN0);
		break;

	case 50:
		clear_cs3_bit(VCC_5V_EN|EN1);
		set_cs3_bit(VCC_3V_EN|EN0);
=======
=======
>>>>>>> refs/remotes/origin/master
		simpad_clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);
		break;

	case 33:  
		simpad_clear_cs3_bit(VCC_3V_EN|EN1);
		simpad_set_cs3_bit(VCC_5V_EN|EN0);
		break;

	case 50:
		simpad_clear_cs3_bit(VCC_5V_EN|EN1);
		simpad_set_cs3_bit(VCC_3V_EN|EN0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		break;

	default:
		printk(KERN_ERR "%s(): unrecognized Vcc %u\n",
			__func__, state->Vcc);
<<<<<<< HEAD
<<<<<<< HEAD
		clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);
=======
		simpad_clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		simpad_clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);
>>>>>>> refs/remotes/origin/master
		local_irq_restore(flags);
		return -1;
	}


	local_irq_restore(flags);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void simpad_pcmcia_socket_init(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_enable_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static void simpad_pcmcia_socket_suspend(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_disable_irqs(skt, irqs, ARRAY_SIZE(irqs));
	set_cs3_bit(PCMCIA_RESET);
=======
static void simpad_pcmcia_socket_suspend(struct soc_pcmcia_socket *skt)
{
	simpad_set_cs3_bit(PCMCIA_RESET);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void simpad_pcmcia_socket_suspend(struct soc_pcmcia_socket *skt)
{
	simpad_set_cs3_bit(PCMCIA_RESET);
>>>>>>> refs/remotes/origin/master
}

static struct pcmcia_low_level simpad_pcmcia_ops = { 
	.owner			= THIS_MODULE,
	.hw_init		= simpad_pcmcia_hw_init,
	.hw_shutdown		= simpad_pcmcia_hw_shutdown,
	.socket_state		= simpad_pcmcia_socket_state,
	.configure_socket	= simpad_pcmcia_configure_socket,
<<<<<<< HEAD
<<<<<<< HEAD
	.socket_init		= simpad_pcmcia_socket_init,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.socket_suspend		= simpad_pcmcia_socket_suspend,
};

int __devinit pcmcia_simpad_init(struct device *dev)
=======
	.socket_suspend		= simpad_pcmcia_socket_suspend,
};

int pcmcia_simpad_init(struct device *dev)
>>>>>>> refs/remotes/origin/master
{
	int ret = -ENODEV;

	if (machine_is_simpad())
		ret = sa11xx_drv_pcmcia_probe(dev, &simpad_pcmcia_ops, 1, 1);

	return ret;
}
