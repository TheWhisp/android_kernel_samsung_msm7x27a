/*
<<<<<<< HEAD
<<<<<<< HEAD
 *	eeh_event.h
 *
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * Copyright (c) 2005 Linas Vepstas <linas@linas.org>
 */

#ifndef ASM_POWERPC_EEH_EVENT_H
#define ASM_POWERPC_EEH_EVENT_H
#ifdef __KERNEL__

<<<<<<< HEAD
<<<<<<< HEAD
/** EEH event -- structure holding pci controller data that describes
 *  a change in the isolation status of a PCI slot.  A pointer
 *  to this struct is passed as the data pointer in a notify callback.
 */
struct eeh_event {
	struct list_head     list;
	struct device_node 	*dn;   /* struct device node */
	struct pci_dev       *dev;  /* affected device */
};

/**
 * eeh_send_failure_event - generate a PCI error event
 * @dev pci device
 *
 * This routine builds a PCI error event which will be delivered
 * to all listeners on the eeh_notifier_chain.
 *
 * This routine can be called within an interrupt context;
 * the actual event will be delivered in a normal context
 * (from a workqueue).
 */
int eeh_send_failure_event (struct device_node *dn,
                            struct pci_dev *dev);

/* Main recovery function */
struct pci_dn * handle_eeh_events (struct eeh_event *);
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * structure holding pci controller data that describes a
 * change in the isolation status of a PCI slot.  A pointer
 * to this struct is passed as the data pointer in a notify
 * callback.
 */
struct eeh_event {
	struct list_head	list;	/* to form event queue	*/
<<<<<<< HEAD
	struct eeh_dev		*edev;	/* EEH device		*/
};

int eeh_send_failure_event(struct eeh_dev *edev);
struct eeh_dev *handle_eeh_events(struct eeh_event *);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct eeh_pe		*pe;	/* EEH PE		*/
};

int eeh_event_init(void);
int eeh_send_failure_event(struct eeh_pe *pe);
void eeh_remove_event(struct eeh_pe *pe);
void eeh_handle_event(struct eeh_pe *pe);
>>>>>>> refs/remotes/origin/master

#endif /* __KERNEL__ */
#endif /* ASM_POWERPC_EEH_EVENT_H */
