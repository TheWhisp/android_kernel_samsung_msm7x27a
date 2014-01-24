/* 
 *
 * parport.h: ia32-compatible parport initialisation
 *
 * This file should only be included by drivers/parport/parport_pc.c.
 */
#ifndef _ASM_PARPORT_H
#define _ASM_PARPORT_H 1


<<<<<<< HEAD
static int __devinit parport_pc_find_nonpci_ports (int autoirq, int autodma)
=======
static int parport_pc_find_nonpci_ports (int autoirq, int autodma)
>>>>>>> refs/remotes/origin/master
{
	/* nothing ! */
	return 0;
}


#endif /* !(_ASM_PARPORT_H) */
