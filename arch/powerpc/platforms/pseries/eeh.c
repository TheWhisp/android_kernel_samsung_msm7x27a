/*
<<<<<<< HEAD
 * eeh.c
 * Copyright IBM Corporation 2001, 2005, 2006
 * Copyright Dave Engebretsen & Todd Inglett 2001
 * Copyright Linas Vepstas 2005, 2006
=======
 * Copyright IBM Corporation 2001, 2005, 2006
 * Copyright Dave Engebretsen & Todd Inglett 2001
 * Copyright Linas Vepstas 2005, 2006
 * Copyright 2001-2012 IBM Corporation.
>>>>>>> refs/remotes/origin/cm-10.0
 *
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
 * Please address comments and feedback to Linas Vepstas <linas@austin.ibm.com>
 */

#include <linux/delay.h>
<<<<<<< HEAD
=======
#include <linux/sched.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/init.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <linux/proc_fs.h>
#include <linux/rbtree.h>
#include <linux/seq_file.h>
#include <linux/spinlock.h>
<<<<<<< HEAD
#include <linux/of.h>

#include <asm/atomic.h>
=======
#include <linux/export.h>
#include <linux/of.h>

#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/eeh.h>
#include <asm/eeh_event.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/ppc-pci.h>
#include <asm/rtas.h>


/** Overview:
 *  EEH, or "Extended Error Handling" is a PCI bridge technology for
 *  dealing with PCI bus errors that can't be dealt with within the
 *  usual PCI framework, except by check-stopping the CPU.  Systems
 *  that are designed for high-availability/reliability cannot afford
 *  to crash due to a "mere" PCI error, thus the need for EEH.
 *  An EEH-capable bridge operates by converting a detected error
 *  into a "slot freeze", taking the PCI adapter off-line, making
 *  the slot behave, from the OS'es point of view, as if the slot
 *  were "empty": all reads return 0xff's and all writes are silently
 *  ignored.  EEH slot isolation events can be triggered by parity
 *  errors on the address or data busses (e.g. during posted writes),
 *  which in turn might be caused by low voltage on the bus, dust,
 *  vibration, humidity, radioactivity or plain-old failed hardware.
 *
 *  Note, however, that one of the leading causes of EEH slot
 *  freeze events are buggy device drivers, buggy device microcode,
 *  or buggy device hardware.  This is because any attempt by the
 *  device to bus-master data to a memory address that is not
 *  assigned to the device will trigger a slot freeze.   (The idea
 *  is to prevent devices-gone-wild from corrupting system memory).
 *  Buggy hardware/drivers will have a miserable time co-existing
 *  with EEH.
 *
 *  Ideally, a PCI device driver, when suspecting that an isolation
 *  event has occurred (e.g. by reading 0xff's), will then ask EEH
 *  whether this is the case, and then take appropriate steps to
 *  reset the PCI slot, the PCI device, and then resume operations.
 *  However, until that day,  the checking is done here, with the
 *  eeh_check_failure() routine embedded in the MMIO macros.  If
 *  the slot is found to be isolated, an "EEH Event" is synthesized
 *  and sent out for processing.
 */

/* If a device driver keeps reading an MMIO register in an interrupt
 * handler after a slot isolation event, it might be broken.
 * This sets the threshold for how many read attempts we allow
 * before printing an error message.
 */
#define EEH_MAX_FAILS	2100000

/* Time to wait for a PCI slot to report status, in milliseconds */
#define PCI_BUS_RESET_WAIT_MSEC (60*1000)

<<<<<<< HEAD
/* RTAS tokens */
static int ibm_set_eeh_option;
static int ibm_set_slot_reset;
static int ibm_read_slot_reset_state;
static int ibm_read_slot_reset_state2;
static int ibm_slot_error_detail;
static int ibm_get_config_addr_info;
static int ibm_get_config_addr_info2;
static int ibm_configure_bridge;
static int ibm_configure_pe;
=======
/* Platform dependent EEH operations */
struct eeh_ops *eeh_ops = NULL;
>>>>>>> refs/remotes/origin/cm-10.0

int eeh_subsystem_enabled;
EXPORT_SYMBOL(eeh_subsystem_enabled);

/* Lock to avoid races due to multiple reports of an error */
static DEFINE_RAW_SPINLOCK(confirm_error_lock);

<<<<<<< HEAD
/* Buffer for reporting slot-error-detail rtas calls. Its here
 * in BSS, and not dynamically alloced, so that it ends up in
 * RMO where RTAS can access it.
 */
static unsigned char slot_errbuf[RTAS_ERROR_LOG_MAX];
static DEFINE_SPINLOCK(slot_errbuf_lock);
static int eeh_error_buf_size;

=======
>>>>>>> refs/remotes/origin/cm-10.0
/* Buffer for reporting pci register dumps. Its here in BSS, and
 * not dynamically alloced, so that it ends up in RMO where RTAS
 * can access it.
 */
#define EEH_PCI_REGS_LOG_LEN 4096
static unsigned char pci_regs_buf[EEH_PCI_REGS_LOG_LEN];

<<<<<<< HEAD
/* System monitoring statistics */
static unsigned long no_device;
static unsigned long no_dn;
static unsigned long no_cfg_addr;
static unsigned long ignored_check;
static unsigned long total_mmio_ffs;
static unsigned long false_positives;
static unsigned long slot_resets;

#define IS_BRIDGE(class_code) (((class_code)<<16) == PCI_BASE_CLASS_BRIDGE)

/* --------------------------------------------------------------- */
/* Below lies the EEH event infrastructure */

static void rtas_slot_error_detail(struct pci_dn *pdn, int severity,
                                   char *driver_log, size_t loglen)
{
	int config_addr;
	unsigned long flags;
	int rc;

	/* Log the error with the rtas logger */
	spin_lock_irqsave(&slot_errbuf_lock, flags);
	memset(slot_errbuf, 0, eeh_error_buf_size);

	/* Use PE configuration address, if present */
	config_addr = pdn->eeh_config_addr;
	if (pdn->eeh_pe_config_addr)
		config_addr = pdn->eeh_pe_config_addr;

	rc = rtas_call(ibm_slot_error_detail,
	               8, 1, NULL, config_addr,
	               BUID_HI(pdn->phb->buid),
	               BUID_LO(pdn->phb->buid),
	               virt_to_phys(driver_log), loglen,
	               virt_to_phys(slot_errbuf),
	               eeh_error_buf_size,
	               severity);

	if (rc == 0)
		log_error(slot_errbuf, ERR_TYPE_RTAS_LOG, 0);
	spin_unlock_irqrestore(&slot_errbuf_lock, flags);
}

/**
 * gather_pci_data - copy assorted PCI config space registers to buff
 * @pdn: device to report data for
=======
/*
 * The struct is used to maintain the EEH global statistic
 * information. Besides, the EEH global statistics will be
 * exported to user space through procfs
 */
struct eeh_stats {
	u64 no_device;		/* PCI device not found		*/
	u64 no_dn;		/* OF node not found		*/
	u64 no_cfg_addr;	/* Config address not found	*/
	u64 ignored_check;	/* EEH check skipped		*/
	u64 total_mmio_ffs;	/* Total EEH checks		*/
	u64 false_positives;	/* Unnecessary EEH checks	*/
	u64 slot_resets;	/* PE reset			*/
};

static struct eeh_stats eeh_stats;

#define IS_BRIDGE(class_code) (((class_code)<<16) == PCI_BASE_CLASS_BRIDGE)

/**
 * eeh_gather_pci_data - Copy assorted PCI config space registers to buff
 * @edev: device to report data for
>>>>>>> refs/remotes/origin/cm-10.0
 * @buf: point to buffer in which to log
 * @len: amount of room in buffer
 *
 * This routine captures assorted PCI configuration space data,
 * and puts them into a buffer for RTAS error logging.
 */
<<<<<<< HEAD
static size_t gather_pci_data(struct pci_dn *pdn, char * buf, size_t len)
{
	struct pci_dev *dev = pdn->pcidev;
=======
static size_t eeh_gather_pci_data(struct eeh_dev *edev, char * buf, size_t len)
{
	struct device_node *dn = eeh_dev_to_of_node(edev);
	struct pci_dev *dev = eeh_dev_to_pci_dev(edev);
>>>>>>> refs/remotes/origin/cm-10.0
	u32 cfg;
	int cap, i;
	int n = 0;

<<<<<<< HEAD
	n += scnprintf(buf+n, len-n, "%s\n", pdn->node->full_name);
	printk(KERN_WARNING "EEH: of node=%s\n", pdn->node->full_name);

	rtas_read_config(pdn, PCI_VENDOR_ID, 4, &cfg);
	n += scnprintf(buf+n, len-n, "dev/vend:%08x\n", cfg);
	printk(KERN_WARNING "EEH: PCI device/vendor: %08x\n", cfg);

	rtas_read_config(pdn, PCI_COMMAND, 4, &cfg);
=======
	n += scnprintf(buf+n, len-n, "%s\n", dn->full_name);
	printk(KERN_WARNING "EEH: of node=%s\n", dn->full_name);

	eeh_ops->read_config(dn, PCI_VENDOR_ID, 4, &cfg);
	n += scnprintf(buf+n, len-n, "dev/vend:%08x\n", cfg);
	printk(KERN_WARNING "EEH: PCI device/vendor: %08x\n", cfg);

	eeh_ops->read_config(dn, PCI_COMMAND, 4, &cfg);
>>>>>>> refs/remotes/origin/cm-10.0
	n += scnprintf(buf+n, len-n, "cmd/stat:%x\n", cfg);
	printk(KERN_WARNING "EEH: PCI cmd/status register: %08x\n", cfg);

	if (!dev) {
		printk(KERN_WARNING "EEH: no PCI device for this of node\n");
		return n;
	}

	/* Gather bridge-specific registers */
	if (dev->class >> 16 == PCI_BASE_CLASS_BRIDGE) {
<<<<<<< HEAD
		rtas_read_config(pdn, PCI_SEC_STATUS, 2, &cfg);
		n += scnprintf(buf+n, len-n, "sec stat:%x\n", cfg);
		printk(KERN_WARNING "EEH: Bridge secondary status: %04x\n", cfg);

		rtas_read_config(pdn, PCI_BRIDGE_CONTROL, 2, &cfg);
=======
		eeh_ops->read_config(dn, PCI_SEC_STATUS, 2, &cfg);
		n += scnprintf(buf+n, len-n, "sec stat:%x\n", cfg);
		printk(KERN_WARNING "EEH: Bridge secondary status: %04x\n", cfg);

		eeh_ops->read_config(dn, PCI_BRIDGE_CONTROL, 2, &cfg);
>>>>>>> refs/remotes/origin/cm-10.0
		n += scnprintf(buf+n, len-n, "brdg ctl:%x\n", cfg);
		printk(KERN_WARNING "EEH: Bridge control: %04x\n", cfg);
	}

	/* Dump out the PCI-X command and status regs */
	cap = pci_find_capability(dev, PCI_CAP_ID_PCIX);
	if (cap) {
<<<<<<< HEAD
		rtas_read_config(pdn, cap, 4, &cfg);
		n += scnprintf(buf+n, len-n, "pcix-cmd:%x\n", cfg);
		printk(KERN_WARNING "EEH: PCI-X cmd: %08x\n", cfg);

		rtas_read_config(pdn, cap+4, 4, &cfg);
=======
		eeh_ops->read_config(dn, cap, 4, &cfg);
		n += scnprintf(buf+n, len-n, "pcix-cmd:%x\n", cfg);
		printk(KERN_WARNING "EEH: PCI-X cmd: %08x\n", cfg);

		eeh_ops->read_config(dn, cap+4, 4, &cfg);
>>>>>>> refs/remotes/origin/cm-10.0
		n += scnprintf(buf+n, len-n, "pcix-stat:%x\n", cfg);
		printk(KERN_WARNING "EEH: PCI-X status: %08x\n", cfg);
	}

	/* If PCI-E capable, dump PCI-E cap 10, and the AER */
	cap = pci_find_capability(dev, PCI_CAP_ID_EXP);
	if (cap) {
		n += scnprintf(buf+n, len-n, "pci-e cap10:\n");
		printk(KERN_WARNING
		       "EEH: PCI-E capabilities and status follow:\n");

		for (i=0; i<=8; i++) {
<<<<<<< HEAD
			rtas_read_config(pdn, cap+4*i, 4, &cfg);
=======
			eeh_ops->read_config(dn, cap+4*i, 4, &cfg);
>>>>>>> refs/remotes/origin/cm-10.0
			n += scnprintf(buf+n, len-n, "%02x:%x\n", 4*i, cfg);
			printk(KERN_WARNING "EEH: PCI-E %02x: %08x\n", i, cfg);
		}

		cap = pci_find_ext_capability(dev, PCI_EXT_CAP_ID_ERR);
		if (cap) {
			n += scnprintf(buf+n, len-n, "pci-e AER:\n");
			printk(KERN_WARNING
			       "EEH: PCI-E AER capability register set follows:\n");

			for (i=0; i<14; i++) {
<<<<<<< HEAD
				rtas_read_config(pdn, cap+4*i, 4, &cfg);
=======
				eeh_ops->read_config(dn, cap+4*i, 4, &cfg);
>>>>>>> refs/remotes/origin/cm-10.0
				n += scnprintf(buf+n, len-n, "%02x:%x\n", 4*i, cfg);
				printk(KERN_WARNING "EEH: PCI-E AER %02x: %08x\n", i, cfg);
			}
		}
	}

	/* Gather status on devices under the bridge */
	if (dev->class >> 16 == PCI_BASE_CLASS_BRIDGE) {
<<<<<<< HEAD
		struct device_node *dn;

		for_each_child_of_node(pdn->node, dn) {
			pdn = PCI_DN(dn);
			if (pdn)
				n += gather_pci_data(pdn, buf+n, len-n);
=======
		struct device_node *child;

		for_each_child_of_node(dn, child) {
			if (of_node_to_eeh_dev(child))
				n += eeh_gather_pci_data(of_node_to_eeh_dev(child), buf+n, len-n);
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}

	return n;
}

<<<<<<< HEAD
void eeh_slot_error_detail(struct pci_dn *pdn, int severity)
{
	size_t loglen = 0;
	pci_regs_buf[0] = 0;

	rtas_pci_enable(pdn, EEH_THAW_MMIO);
	rtas_configure_bridge(pdn);
	eeh_restore_bars(pdn);
	loglen = gather_pci_data(pdn, pci_regs_buf, EEH_PCI_REGS_LOG_LEN);

	rtas_slot_error_detail(pdn, severity, pci_regs_buf, loglen);
}

/**
 * read_slot_reset_state - Read the reset state of a device node's slot
 * @dn: device node to read
 * @rets: array to return results in
 */
static int read_slot_reset_state(struct pci_dn *pdn, int rets[])
{
	int token, outputs;
	int config_addr;

	if (ibm_read_slot_reset_state2 != RTAS_UNKNOWN_SERVICE) {
		token = ibm_read_slot_reset_state2;
		outputs = 4;
	} else {
		token = ibm_read_slot_reset_state;
		rets[2] = 0; /* fake PE Unavailable info */
		outputs = 3;
	}

	/* Use PE configuration address, if present */
	config_addr = pdn->eeh_config_addr;
	if (pdn->eeh_pe_config_addr)
		config_addr = pdn->eeh_pe_config_addr;

	return rtas_call(token, 3, outputs, rets, config_addr,
			 BUID_HI(pdn->phb->buid), BUID_LO(pdn->phb->buid));
}

/**
 * eeh_wait_for_slot_status - returns error status of slot
 * @pdn pci device node
 * @max_wait_msecs maximum number to millisecs to wait
 *
 * Return negative value if a permanent error, else return
 * Partition Endpoint (PE) status value.
 *
 * If @max_wait_msecs is positive, then this routine will
 * sleep until a valid status can be obtained, or until
 * the max allowed wait time is exceeded, in which case
 * a -2 is returned.
 */
int
eeh_wait_for_slot_status(struct pci_dn *pdn, int max_wait_msecs)
{
	int rc;
	int rets[3];
	int mwait;

	while (1) {
		rc = read_slot_reset_state(pdn, rets);
		if (rc) return rc;
		if (rets[1] == 0) return -1;  /* EEH is not supported */

		if (rets[0] != 5) return rets[0]; /* return actual status */

		if (rets[2] == 0) return -1; /* permanently unavailable */

		if (max_wait_msecs <= 0) break;

		mwait = rets[2];
		if (mwait <= 0) {
			printk (KERN_WARNING
			        "EEH: Firmware returned bad wait value=%d\n", mwait);
			mwait = 1000;
		} else if (mwait > 300*1000) {
			printk (KERN_WARNING
			        "EEH: Firmware is taking too long, time=%d\n", mwait);
			mwait = 300*1000;
		}
		max_wait_msecs -= mwait;
		msleep (mwait);
	}

	printk(KERN_WARNING "EEH: Timed out waiting for slot status\n");
	return -2;
}

/**
 * eeh_token_to_phys - convert EEH address token to phys address
 * @token i/o token, should be address in the form 0xA....
=======
/**
 * eeh_slot_error_detail - Generate combined log including driver log and error log
 * @edev: device to report error log for
 * @severity: temporary or permanent error log
 *
 * This routine should be called to generate the combined log, which
 * is comprised of driver log and error log. The driver log is figured
 * out from the config space of the corresponding PCI device, while
 * the error log is fetched through platform dependent function call.
 */
void eeh_slot_error_detail(struct eeh_dev *edev, int severity)
{
	size_t loglen = 0;
	pci_regs_buf[0] = 0;

	eeh_pci_enable(edev, EEH_OPT_THAW_MMIO);
	eeh_ops->configure_bridge(eeh_dev_to_of_node(edev));
	eeh_restore_bars(edev);
	loglen = eeh_gather_pci_data(edev, pci_regs_buf, EEH_PCI_REGS_LOG_LEN);

	eeh_ops->get_log(eeh_dev_to_of_node(edev), severity, pci_regs_buf, loglen);
}

/**
 * eeh_token_to_phys - Convert EEH address token to phys address
 * @token: I/O token, should be address in the form 0xA....
 *
 * This routine should be called to convert virtual I/O address
 * to physical one.
>>>>>>> refs/remotes/origin/cm-10.0
 */
static inline unsigned long eeh_token_to_phys(unsigned long token)
{
	pte_t *ptep;
	unsigned long pa;

	ptep = find_linux_pte(init_mm.pgd, token);
	if (!ptep)
		return token;
	pa = pte_pfn(*ptep) << PAGE_SHIFT;

	return pa | (token & (PAGE_SIZE-1));
}

<<<<<<< HEAD
/** 
 * Return the "partitionable endpoint" (pe) under which this device lies
 */
struct device_node * find_device_pe(struct device_node *dn)
{
	while ((dn->parent) && PCI_DN(dn->parent) &&
	      (PCI_DN(dn->parent)->eeh_mode & EEH_MODE_SUPPORTED)) {
=======
/**
 * eeh_find_device_pe - Retrieve the PE for the given device
 * @dn: device node
 *
 * Return the PE under which this device lies
 */
struct device_node *eeh_find_device_pe(struct device_node *dn)
{
	while (dn->parent && of_node_to_eeh_dev(dn->parent) &&
	       (of_node_to_eeh_dev(dn->parent)->mode & EEH_MODE_SUPPORTED)) {
>>>>>>> refs/remotes/origin/cm-10.0
		dn = dn->parent;
	}
	return dn;
}

<<<<<<< HEAD
/** Mark all devices that are children of this device as failed.
 *  Mark the device driver too, so that it can see the failure
 *  immediately; this is critical, since some drivers poll
 *  status registers in interrupts ... If a driver is polling,
 *  and the slot is frozen, then the driver can deadlock in
 *  an interrupt context, which is bad.
 */

=======
/**
 * __eeh_mark_slot - Mark all child devices as failed
 * @parent: parent device
 * @mode_flag: failure flag
 *
 * Mark all devices that are children of this device as failed.
 * Mark the device driver too, so that it can see the failure
 * immediately; this is critical, since some drivers poll
 * status registers in interrupts ... If a driver is polling,
 * and the slot is frozen, then the driver can deadlock in
 * an interrupt context, which is bad.
 */
>>>>>>> refs/remotes/origin/cm-10.0
static void __eeh_mark_slot(struct device_node *parent, int mode_flag)
{
	struct device_node *dn;

	for_each_child_of_node(parent, dn) {
<<<<<<< HEAD
		if (PCI_DN(dn)) {
			/* Mark the pci device driver too */
			struct pci_dev *dev = PCI_DN(dn)->pcidev;

			PCI_DN(dn)->eeh_mode |= mode_flag;
=======
		if (of_node_to_eeh_dev(dn)) {
			/* Mark the pci device driver too */
			struct pci_dev *dev = of_node_to_eeh_dev(dn)->pdev;

			of_node_to_eeh_dev(dn)->mode |= mode_flag;
>>>>>>> refs/remotes/origin/cm-10.0

			if (dev && dev->driver)
				dev->error_state = pci_channel_io_frozen;

			__eeh_mark_slot(dn, mode_flag);
		}
	}
}

<<<<<<< HEAD
void eeh_mark_slot (struct device_node *dn, int mode_flag)
{
	struct pci_dev *dev;
	dn = find_device_pe (dn);

	/* Back up one, since config addrs might be shared */
	if (!pcibios_find_pci_bus(dn) && PCI_DN(dn->parent))
		dn = dn->parent;

	PCI_DN(dn)->eeh_mode |= mode_flag;

	/* Mark the pci device too */
	dev = PCI_DN(dn)->pcidev;
=======
/**
 * eeh_mark_slot - Mark the indicated device and its children as failed
 * @dn: parent device
 * @mode_flag: failure flag
 *
 * Mark the indicated device and its child devices as failed.
 * The device drivers are marked as failed as well.
 */
void eeh_mark_slot(struct device_node *dn, int mode_flag)
{
	struct pci_dev *dev;
	dn = eeh_find_device_pe(dn);

	/* Back up one, since config addrs might be shared */
	if (!pcibios_find_pci_bus(dn) && of_node_to_eeh_dev(dn->parent))
		dn = dn->parent;

	of_node_to_eeh_dev(dn)->mode |= mode_flag;

	/* Mark the pci device too */
	dev = of_node_to_eeh_dev(dn)->pdev;
>>>>>>> refs/remotes/origin/cm-10.0
	if (dev)
		dev->error_state = pci_channel_io_frozen;

	__eeh_mark_slot(dn, mode_flag);
}

<<<<<<< HEAD
=======
/**
 * __eeh_clear_slot - Clear failure flag for the child devices
 * @parent: parent device
 * @mode_flag: flag to be cleared
 *
 * Clear failure flag for the child devices.
 */
>>>>>>> refs/remotes/origin/cm-10.0
static void __eeh_clear_slot(struct device_node *parent, int mode_flag)
{
	struct device_node *dn;

	for_each_child_of_node(parent, dn) {
<<<<<<< HEAD
		if (PCI_DN(dn)) {
			PCI_DN(dn)->eeh_mode &= ~mode_flag;
			PCI_DN(dn)->eeh_check_count = 0;
=======
		if (of_node_to_eeh_dev(dn)) {
			of_node_to_eeh_dev(dn)->mode &= ~mode_flag;
			of_node_to_eeh_dev(dn)->check_count = 0;
>>>>>>> refs/remotes/origin/cm-10.0
			__eeh_clear_slot(dn, mode_flag);
		}
	}
}

<<<<<<< HEAD
void eeh_clear_slot (struct device_node *dn, int mode_flag)
=======
/**
 * eeh_clear_slot - Clear failure flag for the indicated device and its children
 * @dn: parent device
 * @mode_flag: flag to be cleared
 *
 * Clear failure flag for the indicated device and its children.
 */
void eeh_clear_slot(struct device_node *dn, int mode_flag)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned long flags;
	raw_spin_lock_irqsave(&confirm_error_lock, flags);
	
<<<<<<< HEAD
	dn = find_device_pe (dn);
	
	/* Back up one, since config addrs might be shared */
	if (!pcibios_find_pci_bus(dn) && PCI_DN(dn->parent))
		dn = dn->parent;

	PCI_DN(dn)->eeh_mode &= ~mode_flag;
	PCI_DN(dn)->eeh_check_count = 0;
=======
	dn = eeh_find_device_pe(dn);
	
	/* Back up one, since config addrs might be shared */
	if (!pcibios_find_pci_bus(dn) && of_node_to_eeh_dev(dn->parent))
		dn = dn->parent;

	of_node_to_eeh_dev(dn)->mode &= ~mode_flag;
	of_node_to_eeh_dev(dn)->check_count = 0;
>>>>>>> refs/remotes/origin/cm-10.0
	__eeh_clear_slot(dn, mode_flag);
	raw_spin_unlock_irqrestore(&confirm_error_lock, flags);
}

<<<<<<< HEAD
void __eeh_set_pe_freset(struct device_node *parent, unsigned int *freset)
{
	struct device_node *dn;

	for_each_child_of_node(parent, dn) {
		if (PCI_DN(dn)) {

			struct pci_dev *dev = PCI_DN(dn)->pcidev;

			if (dev && dev->driver)
				*freset |= dev->needs_freset;

			__eeh_set_pe_freset(dn, freset);
		}
	}
}

void eeh_set_pe_freset(struct device_node *dn, unsigned int *freset)
{
	struct pci_dev *dev;
	dn = find_device_pe(dn);

	/* Back up one, since config addrs might be shared */
	if (!pcibios_find_pci_bus(dn) && PCI_DN(dn->parent))
		dn = dn->parent;

	dev = PCI_DN(dn)->pcidev;
	if (dev)
		*freset |= dev->needs_freset;

	__eeh_set_pe_freset(dn, freset);
}

/**
 * eeh_dn_check_failure - check if all 1's data is due to EEH slot freeze
 * @dn device node
 * @dev pci device, if known
=======
/**
 * eeh_dn_check_failure - Check if all 1's data is due to EEH slot freeze
 * @dn: device node
 * @dev: pci device, if known
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Check for an EEH failure for the given device node.  Call this
 * routine if the result of a read was all 0xff's and you want to
 * find out if this is due to an EEH slot freeze.  This routine
 * will query firmware for the EEH status.
 *
 * Returns 0 if there has not been an EEH error; otherwise returns
 * a non-zero value and queues up a slot isolation event notification.
 *
 * It is safe to call this routine in an interrupt context.
 */
int eeh_dn_check_failure(struct device_node *dn, struct pci_dev *dev)
{
	int ret;
<<<<<<< HEAD
	int rets[3];
	unsigned long flags;
	struct pci_dn *pdn;
	int rc = 0;
	const char *location;

	total_mmio_ffs++;
=======
	unsigned long flags;
	struct eeh_dev *edev;
	int rc = 0;
	const char *location;

	eeh_stats.total_mmio_ffs++;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!eeh_subsystem_enabled)
		return 0;

	if (!dn) {
<<<<<<< HEAD
		no_dn++;
		return 0;
	}
	dn = find_device_pe(dn);
	pdn = PCI_DN(dn);

	/* Access to IO BARs might get this far and still not want checking. */
	if (!(pdn->eeh_mode & EEH_MODE_SUPPORTED) ||
	    pdn->eeh_mode & EEH_MODE_NOCHECK) {
		ignored_check++;
		pr_debug("EEH: Ignored check (%x) for %s %s\n",
			 pdn->eeh_mode, eeh_pci_name(dev), dn->full_name);
		return 0;
	}

	if (!pdn->eeh_config_addr && !pdn->eeh_pe_config_addr) {
		no_cfg_addr++;
=======
		eeh_stats.no_dn++;
		return 0;
	}
	dn = eeh_find_device_pe(dn);
	edev = of_node_to_eeh_dev(dn);

	/* Access to IO BARs might get this far and still not want checking. */
	if (!(edev->mode & EEH_MODE_SUPPORTED) ||
	    edev->mode & EEH_MODE_NOCHECK) {
		eeh_stats.ignored_check++;
		pr_debug("EEH: Ignored check (%x) for %s %s\n",
			edev->mode, eeh_pci_name(dev), dn->full_name);
		return 0;
	}

	if (!edev->config_addr && !edev->pe_config_addr) {
		eeh_stats.no_cfg_addr++;
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;
	}

	/* If we already have a pending isolation event for this
	 * slot, we know it's bad already, we don't need to check.
	 * Do this checking under a lock; as multiple PCI devices
	 * in one slot might report errors simultaneously, and we
	 * only want one error recovery routine running.
	 */
	raw_spin_lock_irqsave(&confirm_error_lock, flags);
	rc = 1;
<<<<<<< HEAD
	if (pdn->eeh_mode & EEH_MODE_ISOLATED) {
		pdn->eeh_check_count ++;
		if (pdn->eeh_check_count % EEH_MAX_FAILS == 0) {
			location = of_get_property(dn, "ibm,loc-code", NULL);
			printk (KERN_ERR "EEH: %d reads ignored for recovering device at "
				"location=%s driver=%s pci addr=%s\n",
				pdn->eeh_check_count, location,
				dev->driver->name, eeh_pci_name(dev));
			printk (KERN_ERR "EEH: Might be infinite loop in %s driver\n",
				dev->driver->name);
=======
	if (edev->mode & EEH_MODE_ISOLATED) {
		edev->check_count++;
		if (edev->check_count % EEH_MAX_FAILS == 0) {
			location = of_get_property(dn, "ibm,loc-code", NULL);
			printk(KERN_ERR "EEH: %d reads ignored for recovering device at "
				"location=%s driver=%s pci addr=%s\n",
				edev->check_count, location,
				eeh_driver_name(dev), eeh_pci_name(dev));
			printk(KERN_ERR "EEH: Might be infinite loop in %s driver\n",
				eeh_driver_name(dev));
>>>>>>> refs/remotes/origin/cm-10.0
			dump_stack();
		}
		goto dn_unlock;
	}

	/*
	 * Now test for an EEH failure.  This is VERY expensive.
	 * Note that the eeh_config_addr may be a parent device
	 * in the case of a device behind a bridge, or it may be
	 * function zero of a multi-function device.
	 * In any case they must share a common PHB.
	 */
<<<<<<< HEAD
	ret = read_slot_reset_state(pdn, rets);

	/* If the call to firmware failed, punt */
	if (ret != 0) {
		printk(KERN_WARNING "EEH: read_slot_reset_state() failed; rc=%d dn=%s\n",
		       ret, dn->full_name);
		false_positives++;
		pdn->eeh_false_positives ++;
		rc = 0;
		goto dn_unlock;
	}

	/* Note that config-io to empty slots may fail;
	 * they are empty when they don't have children. */
	if ((rets[0] == 5) && (rets[2] == 0) && (dn->child == NULL)) {
		false_positives++;
		pdn->eeh_false_positives ++;
		rc = 0;
		goto dn_unlock;
	}

	/* If EEH is not supported on this device, punt. */
	if (rets[1] != 1) {
		printk(KERN_WARNING "EEH: event on unsupported device, rc=%d dn=%s\n",
		       ret, dn->full_name);
		false_positives++;
		pdn->eeh_false_positives ++;
		rc = 0;
		goto dn_unlock;
	}

	/* If not the kind of error we know about, punt. */
	if (rets[0] != 1 && rets[0] != 2 && rets[0] != 4 && rets[0] != 5) {
		false_positives++;
		pdn->eeh_false_positives ++;
=======
	ret = eeh_ops->get_state(dn, NULL);

	/* Note that config-io to empty slots may fail;
	 * they are empty when they don't have children.
	 * We will punt with the following conditions: Failure to get
	 * PE's state, EEH not support and Permanently unavailable
	 * state, PE is in good state.
	 */
	if ((ret < 0) ||
	    (ret == EEH_STATE_NOT_SUPPORT) ||
	    (ret & (EEH_STATE_MMIO_ACTIVE | EEH_STATE_DMA_ACTIVE)) ==
	    (EEH_STATE_MMIO_ACTIVE | EEH_STATE_DMA_ACTIVE)) {
		eeh_stats.false_positives++;
		edev->false_positives ++;
>>>>>>> refs/remotes/origin/cm-10.0
		rc = 0;
		goto dn_unlock;
	}

<<<<<<< HEAD
	slot_resets++;
 
	/* Avoid repeated reports of this failure, including problems
	 * with other functions on this device, and functions under
	 * bridges. */
	eeh_mark_slot (dn, EEH_MODE_ISOLATED);
	raw_spin_unlock_irqrestore(&confirm_error_lock, flags);

	eeh_send_failure_event (dn, dev);

	/* Most EEH events are due to device driver bugs.  Having
	 * a stack trace will help the device-driver authors figure
	 * out what happened.  So print that out. */
=======
	eeh_stats.slot_resets++;
 
	/* Avoid repeated reports of this failure, including problems
	 * with other functions on this device, and functions under
	 * bridges.
	 */
	eeh_mark_slot(dn, EEH_MODE_ISOLATED);
	raw_spin_unlock_irqrestore(&confirm_error_lock, flags);

	eeh_send_failure_event(edev);

	/* Most EEH events are due to device driver bugs.  Having
	 * a stack trace will help the device-driver authors figure
	 * out what happened.  So print that out.
	 */
>>>>>>> refs/remotes/origin/cm-10.0
	dump_stack();
	return 1;

dn_unlock:
	raw_spin_unlock_irqrestore(&confirm_error_lock, flags);
	return rc;
}

EXPORT_SYMBOL_GPL(eeh_dn_check_failure);

/**
<<<<<<< HEAD
 * eeh_check_failure - check if all 1's data is due to EEH slot freeze
 * @token i/o token, should be address in the form 0xA....
 * @val value, should be all 1's (XXX why do we need this arg??)
=======
 * eeh_check_failure - Check if all 1's data is due to EEH slot freeze
 * @token: I/O token, should be address in the form 0xA....
 * @val: value, should be all 1's (XXX why do we need this arg??)
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Check for an EEH failure at the given token address.  Call this
 * routine if the result of a read was all 0xff's and you want to
 * find out if this is due to an EEH slot freeze event.  This routine
 * will query firmware for the EEH status.
 *
 * Note this routine is safe to call in an interrupt context.
 */
unsigned long eeh_check_failure(const volatile void __iomem *token, unsigned long val)
{
	unsigned long addr;
	struct pci_dev *dev;
	struct device_node *dn;

	/* Finding the phys addr + pci device; this is pretty quick. */
	addr = eeh_token_to_phys((unsigned long __force) token);
<<<<<<< HEAD
	dev = pci_get_device_by_addr(addr);
	if (!dev) {
		no_device++;
=======
	dev = pci_addr_cache_get_device(addr);
	if (!dev) {
		eeh_stats.no_device++;
>>>>>>> refs/remotes/origin/cm-10.0
		return val;
	}

	dn = pci_device_to_OF_node(dev);
<<<<<<< HEAD
	eeh_dn_check_failure (dn, dev);
=======
	eeh_dn_check_failure(dn, dev);
>>>>>>> refs/remotes/origin/cm-10.0

	pci_dev_put(dev);
	return val;
}

EXPORT_SYMBOL(eeh_check_failure);

<<<<<<< HEAD
/* ------------------------------------------------------------- */
/* The code below deals with error recovery */

/**
 * rtas_pci_enable - enable MMIO or DMA transfers for this slot
 * @pdn pci device node
 */

int
rtas_pci_enable(struct pci_dn *pdn, int function)
{
	int config_addr;
	int rc;

	/* Use PE configuration address, if present */
	config_addr = pdn->eeh_config_addr;
	if (pdn->eeh_pe_config_addr)
		config_addr = pdn->eeh_pe_config_addr;

	rc = rtas_call(ibm_set_eeh_option, 4, 1, NULL,
	               config_addr,
	               BUID_HI(pdn->phb->buid),
	               BUID_LO(pdn->phb->buid),
		            function);

	if (rc)
		printk(KERN_WARNING "EEH: Unexpected state change %d, err=%d dn=%s\n",
		        function, rc, pdn->node->full_name);

	rc = eeh_wait_for_slot_status (pdn, PCI_BUS_RESET_WAIT_MSEC);
	if ((rc == 4) && (function == EEH_THAW_MMIO))
=======

/**
 * eeh_pci_enable - Enable MMIO or DMA transfers for this slot
 * @edev: pci device node
 *
 * This routine should be called to reenable frozen MMIO or DMA
 * so that it would work correctly again. It's useful while doing
 * recovery or log collection on the indicated device.
 */
int eeh_pci_enable(struct eeh_dev *edev, int function)
{
	int rc;
	struct device_node *dn = eeh_dev_to_of_node(edev);

	rc = eeh_ops->set_option(dn, function);
	if (rc)
		printk(KERN_WARNING "EEH: Unexpected state change %d, err=%d dn=%s\n",
		        function, rc, dn->full_name);

	rc = eeh_ops->wait_state(dn, PCI_BUS_RESET_WAIT_MSEC);
	if (rc > 0 && (rc & EEH_STATE_MMIO_ENABLED) &&
	   (function == EEH_OPT_THAW_MMIO))
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;

	return rc;
}

/**
<<<<<<< HEAD
 * rtas_pci_slot_reset - raises/lowers the pci #RST line
 * @pdn pci device node
 * @state: 1/0 to raise/lower the #RST
 *
 * Clear the EEH-frozen condition on a slot.  This routine
 * asserts the PCI #RST line if the 'state' argument is '1',
 * and drops the #RST line if 'state is '0'.  This routine is
 * safe to call in an interrupt context.
 *
 */

static void
rtas_pci_slot_reset(struct pci_dn *pdn, int state)
{
	int config_addr;
	int rc;

	BUG_ON (pdn==NULL); 

	if (!pdn->phb) {
		printk (KERN_WARNING "EEH: in slot reset, device node %s has no phb\n",
		        pdn->node->full_name);
		return;
	}

	/* Use PE configuration address, if present */
	config_addr = pdn->eeh_config_addr;
	if (pdn->eeh_pe_config_addr)
		config_addr = pdn->eeh_pe_config_addr;

	rc = rtas_call(ibm_set_slot_reset, 4, 1, NULL,
	               config_addr,
	               BUID_HI(pdn->phb->buid),
	               BUID_LO(pdn->phb->buid),
	               state);

	/* Fundamental-reset not supported on this PE, try hot-reset */
	if (rc == -8 && state == 3) {
		rc = rtas_call(ibm_set_slot_reset, 4, 1, NULL,
			       config_addr,
			       BUID_HI(pdn->phb->buid),
			       BUID_LO(pdn->phb->buid), 1);
		if (rc)
			printk(KERN_WARNING
				"EEH: Unable to reset the failed slot,"
				" #RST=%d dn=%s\n",
				rc, pdn->node->full_name);
	}
}

/**
 * pcibios_set_pcie_slot_reset - Set PCI-E reset state
 * @dev:	pci device struct
 * @state:	reset state to enter
 *
 * Return value:
 * 	0 if success
 **/
int pcibios_set_pcie_reset_state(struct pci_dev *dev, enum pcie_reset_state state)
{
	struct device_node *dn = pci_device_to_OF_node(dev);
	struct pci_dn *pdn = PCI_DN(dn);

	switch (state) {
	case pcie_deassert_reset:
		rtas_pci_slot_reset(pdn, 0);
		break;
	case pcie_hot_reset:
		rtas_pci_slot_reset(pdn, 1);
		break;
	case pcie_warm_reset:
		rtas_pci_slot_reset(pdn, 3);
=======
 * pcibios_set_pcie_slot_reset - Set PCI-E reset state
 * @dev: pci device struct
 * @state: reset state to enter
 *
 * Return value:
 * 	0 if success
 */
int pcibios_set_pcie_reset_state(struct pci_dev *dev, enum pcie_reset_state state)
{
	struct device_node *dn = pci_device_to_OF_node(dev);

	switch (state) {
	case pcie_deassert_reset:
		eeh_ops->reset(dn, EEH_RESET_DEACTIVATE);
		break;
	case pcie_hot_reset:
		eeh_ops->reset(dn, EEH_RESET_HOT);
		break;
	case pcie_warm_reset:
		eeh_ops->reset(dn, EEH_RESET_FUNDAMENTAL);
>>>>>>> refs/remotes/origin/cm-10.0
		break;
	default:
		return -EINVAL;
	};

	return 0;
}

/**
<<<<<<< HEAD
 * rtas_set_slot_reset -- assert the pci #RST line for 1/4 second
 * @pdn: pci device node to be reset.
 */

static void __rtas_set_slot_reset(struct pci_dn *pdn)
{
	unsigned int freset = 0;
=======
 * __eeh_set_pe_freset - Check the required reset for child devices
 * @parent: parent device
 * @freset: return value
 *
 * Each device might have its preferred reset type: fundamental or
 * hot reset. The routine is used to collect the information from
 * the child devices so that they could be reset accordingly.
 */
void __eeh_set_pe_freset(struct device_node *parent, unsigned int *freset)
{
	struct device_node *dn;

	for_each_child_of_node(parent, dn) {
		if (of_node_to_eeh_dev(dn)) {
			struct pci_dev *dev = of_node_to_eeh_dev(dn)->pdev;

			if (dev && dev->driver)
				*freset |= dev->needs_freset;

			__eeh_set_pe_freset(dn, freset);
		}
	}
}

/**
 * eeh_set_pe_freset - Check the required reset for the indicated device and its children
 * @dn: parent device
 * @freset: return value
 *
 * Each device might have its preferred reset type: fundamental or
 * hot reset. The routine is used to collected the information for
 * the indicated device and its children so that the bunch of the
 * devices could be reset properly.
 */
void eeh_set_pe_freset(struct device_node *dn, unsigned int *freset)
{
	struct pci_dev *dev;
	dn = eeh_find_device_pe(dn);

	/* Back up one, since config addrs might be shared */
	if (!pcibios_find_pci_bus(dn) && of_node_to_eeh_dev(dn->parent))
		dn = dn->parent;

	dev = of_node_to_eeh_dev(dn)->pdev;
	if (dev)
		*freset |= dev->needs_freset;

	__eeh_set_pe_freset(dn, freset);
}

/**
 * eeh_reset_pe_once - Assert the pci #RST line for 1/4 second
 * @edev: pci device node to be reset.
 *
 * Assert the PCI #RST line for 1/4 second.
 */
static void eeh_reset_pe_once(struct eeh_dev *edev)
{
	unsigned int freset = 0;
	struct device_node *dn = eeh_dev_to_of_node(edev);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Determine type of EEH reset required for
	 * Partitionable Endpoint, a hot-reset (1)
	 * or a fundamental reset (3).
	 * A fundamental reset required by any device under
	 * Partitionable Endpoint trumps hot-reset.
  	 */
<<<<<<< HEAD
	eeh_set_pe_freset(pdn->node, &freset);

	if (freset)
		rtas_pci_slot_reset(pdn, 3);
	else
		rtas_pci_slot_reset(pdn, 1);

	/* The PCI bus requires that the reset be held high for at least
	 * a 100 milliseconds. We wait a bit longer 'just in case'.  */

#define PCI_BUS_RST_HOLD_TIME_MSEC 250
	msleep (PCI_BUS_RST_HOLD_TIME_MSEC);
	
	/* We might get hit with another EEH freeze as soon as the 
	 * pci slot reset line is dropped. Make sure we don't miss
	 * these, and clear the flag now. */
	eeh_clear_slot (pdn->node, EEH_MODE_ISOLATED);

	rtas_pci_slot_reset (pdn, 0);

	/* After a PCI slot has been reset, the PCI Express spec requires
	 * a 1.5 second idle time for the bus to stabilize, before starting
	 * up traffic. */
#define PCI_BUS_SETTLE_TIME_MSEC 1800
	msleep (PCI_BUS_SETTLE_TIME_MSEC);
}

int rtas_set_slot_reset(struct pci_dn *pdn)
{
	int i, rc;

	/* Take three shots at resetting the bus */
	for (i=0; i<3; i++) {
		__rtas_set_slot_reset(pdn);

		rc = eeh_wait_for_slot_status(pdn, PCI_BUS_RESET_WAIT_MSEC);
		if (rc == 0)
=======
	eeh_set_pe_freset(dn, &freset);

	if (freset)
		eeh_ops->reset(dn, EEH_RESET_FUNDAMENTAL);
	else
		eeh_ops->reset(dn, EEH_RESET_HOT);

	/* The PCI bus requires that the reset be held high for at least
	 * a 100 milliseconds. We wait a bit longer 'just in case'.
	 */
#define PCI_BUS_RST_HOLD_TIME_MSEC 250
	msleep(PCI_BUS_RST_HOLD_TIME_MSEC);
	
	/* We might get hit with another EEH freeze as soon as the 
	 * pci slot reset line is dropped. Make sure we don't miss
	 * these, and clear the flag now.
	 */
	eeh_clear_slot(dn, EEH_MODE_ISOLATED);

	eeh_ops->reset(dn, EEH_RESET_DEACTIVATE);

	/* After a PCI slot has been reset, the PCI Express spec requires
	 * a 1.5 second idle time for the bus to stabilize, before starting
	 * up traffic.
	 */
#define PCI_BUS_SETTLE_TIME_MSEC 1800
	msleep(PCI_BUS_SETTLE_TIME_MSEC);
}

/**
 * eeh_reset_pe - Reset the indicated PE
 * @edev: PCI device associated EEH device
 *
 * This routine should be called to reset indicated device, including
 * PE. A PE might include multiple PCI devices and sometimes PCI bridges
 * might be involved as well.
 */
int eeh_reset_pe(struct eeh_dev *edev)
{
	int i, rc;
	struct device_node *dn = eeh_dev_to_of_node(edev);

	/* Take three shots at resetting the bus */
	for (i=0; i<3; i++) {
		eeh_reset_pe_once(edev);

		rc = eeh_ops->wait_state(dn, PCI_BUS_RESET_WAIT_MSEC);
		if (rc == (EEH_STATE_MMIO_ACTIVE | EEH_STATE_DMA_ACTIVE))
>>>>>>> refs/remotes/origin/cm-10.0
			return 0;

		if (rc < 0) {
			printk(KERN_ERR "EEH: unrecoverable slot failure %s\n",
<<<<<<< HEAD
			       pdn->node->full_name);
			return -1;
		}
		printk(KERN_ERR "EEH: bus reset %d failed on slot %s, rc=%d\n",
		       i+1, pdn->node->full_name, rc);
=======
			       dn->full_name);
			return -1;
		}
		printk(KERN_ERR "EEH: bus reset %d failed on slot %s, rc=%d\n",
		       i+1, dn->full_name, rc);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return -1;
}

<<<<<<< HEAD
/* ------------------------------------------------------- */
=======
>>>>>>> refs/remotes/origin/cm-10.0
/** Save and restore of PCI BARs
 *
 * Although firmware will set up BARs during boot, it doesn't
 * set up device BAR's after a device reset, although it will,
 * if requested, set up bridge configuration. Thus, we need to
 * configure the PCI devices ourselves.  
 */

/**
<<<<<<< HEAD
 * __restore_bars - Restore the Base Address Registers
 * @pdn: pci device node
=======
 * eeh_restore_one_device_bars - Restore the Base Address Registers for one device
 * @edev: PCI device associated EEH device
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Loads the PCI configuration space base address registers,
 * the expansion ROM base address, the latency timer, and etc.
 * from the saved values in the device node.
 */
<<<<<<< HEAD
static inline void __restore_bars (struct pci_dn *pdn)
{
	int i;
	u32 cmd;

	if (NULL==pdn->phb) return;
	for (i=4; i<10; i++) {
		rtas_write_config(pdn, i*4, 4, pdn->config_space[i]);
	}

	/* 12 == Expansion ROM Address */
	rtas_write_config(pdn, 12*4, 4, pdn->config_space[12]);

#define BYTE_SWAP(OFF) (8*((OFF)/4)+3-(OFF))
#define SAVED_BYTE(OFF) (((u8 *)(pdn->config_space))[BYTE_SWAP(OFF)])

	rtas_write_config (pdn, PCI_CACHE_LINE_SIZE, 1,
	            SAVED_BYTE(PCI_CACHE_LINE_SIZE));

	rtas_write_config (pdn, PCI_LATENCY_TIMER, 1,
	            SAVED_BYTE(PCI_LATENCY_TIMER));

	/* max latency, min grant, interrupt pin and line */
	rtas_write_config(pdn, 15*4, 4, pdn->config_space[15]);

	/* Restore PERR & SERR bits, some devices require it,
	   don't touch the other command bits */
	rtas_read_config(pdn, PCI_COMMAND, 4, &cmd);
	if (pdn->config_space[1] & PCI_COMMAND_PARITY)
		cmd |= PCI_COMMAND_PARITY;
	else
		cmd &= ~PCI_COMMAND_PARITY;
	if (pdn->config_space[1] & PCI_COMMAND_SERR)
		cmd |= PCI_COMMAND_SERR;
	else
		cmd &= ~PCI_COMMAND_SERR;
	rtas_write_config(pdn, PCI_COMMAND, 4, cmd);
}

/**
 * eeh_restore_bars - restore the PCI config space info
=======
static inline void eeh_restore_one_device_bars(struct eeh_dev *edev)
{
	int i;
	u32 cmd;
	struct device_node *dn = eeh_dev_to_of_node(edev);

	if (!edev->phb)
		return;

	for (i=4; i<10; i++) {
		eeh_ops->write_config(dn, i*4, 4, edev->config_space[i]);
	}

	/* 12 == Expansion ROM Address */
	eeh_ops->write_config(dn, 12*4, 4, edev->config_space[12]);

#define BYTE_SWAP(OFF) (8*((OFF)/4)+3-(OFF))
#define SAVED_BYTE(OFF) (((u8 *)(edev->config_space))[BYTE_SWAP(OFF)])

	eeh_ops->write_config(dn, PCI_CACHE_LINE_SIZE, 1,
	            SAVED_BYTE(PCI_CACHE_LINE_SIZE));

	eeh_ops->write_config(dn, PCI_LATENCY_TIMER, 1,
	            SAVED_BYTE(PCI_LATENCY_TIMER));

	/* max latency, min grant, interrupt pin and line */
	eeh_ops->write_config(dn, 15*4, 4, edev->config_space[15]);

	/* Restore PERR & SERR bits, some devices require it,
	 * don't touch the other command bits
	 */
	eeh_ops->read_config(dn, PCI_COMMAND, 4, &cmd);
	if (edev->config_space[1] & PCI_COMMAND_PARITY)
		cmd |= PCI_COMMAND_PARITY;
	else
		cmd &= ~PCI_COMMAND_PARITY;
	if (edev->config_space[1] & PCI_COMMAND_SERR)
		cmd |= PCI_COMMAND_SERR;
	else
		cmd &= ~PCI_COMMAND_SERR;
	eeh_ops->write_config(dn, PCI_COMMAND, 4, cmd);
}

/**
 * eeh_restore_bars - Restore the PCI config space info
 * @edev: EEH device
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * This routine performs a recursive walk to the children
 * of this device as well.
 */
<<<<<<< HEAD
void eeh_restore_bars(struct pci_dn *pdn)
{
	struct device_node *dn;
	if (!pdn) 
		return;
	
	if ((pdn->eeh_mode & EEH_MODE_SUPPORTED) && !IS_BRIDGE(pdn->class_code))
		__restore_bars (pdn);

	for_each_child_of_node(pdn->node, dn)
		eeh_restore_bars (PCI_DN(dn));
}

/**
 * eeh_save_bars - save device bars
=======
void eeh_restore_bars(struct eeh_dev *edev)
{
	struct device_node *dn;
	if (!edev)
		return;
	
	if ((edev->mode & EEH_MODE_SUPPORTED) && !IS_BRIDGE(edev->class_code))
		eeh_restore_one_device_bars(edev);

	for_each_child_of_node(eeh_dev_to_of_node(edev), dn)
		eeh_restore_bars(of_node_to_eeh_dev(dn));
}

/**
 * eeh_save_bars - Save device bars
 * @edev: PCI device associated EEH device
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Save the values of the device bars. Unlike the restore
 * routine, this routine is *not* recursive. This is because
 * PCI devices are added individually; but, for the restore,
 * an entire slot is reset at a time.
 */
<<<<<<< HEAD
static void eeh_save_bars(struct pci_dn *pdn)
{
	int i;

	if (!pdn )
		return;
	
	for (i = 0; i < 16; i++)
		rtas_read_config(pdn, i * 4, 4, &pdn->config_space[i]);
}

void
rtas_configure_bridge(struct pci_dn *pdn)
{
	int config_addr;
	int rc;
	int token;

	/* Use PE configuration address, if present */
	config_addr = pdn->eeh_config_addr;
	if (pdn->eeh_pe_config_addr)
		config_addr = pdn->eeh_pe_config_addr;

	/* Use new configure-pe function, if supported */
	if (ibm_configure_pe != RTAS_UNKNOWN_SERVICE)
		token = ibm_configure_pe;
	else
		token = ibm_configure_bridge;

	rc = rtas_call(token, 3, 1, NULL,
	               config_addr,
	               BUID_HI(pdn->phb->buid),
	               BUID_LO(pdn->phb->buid));
	if (rc) {
		printk (KERN_WARNING "EEH: Unable to configure device bridge (%d) for %s\n",
		        rc, pdn->node->full_name);
	}
}

/* ------------------------------------------------------------- */
/* The code below deals with enabling EEH for devices during  the
 * early boot sequence.  EEH must be enabled before any PCI probing
 * can be done.
 */

#define EEH_ENABLE 1

struct eeh_early_enable_info {
	unsigned int buid_hi;
	unsigned int buid_lo;
};

static int get_pe_addr (int config_addr,
                        struct eeh_early_enable_info *info)
{
	unsigned int rets[3];
	int ret;

	/* Use latest config-addr token on power6 */
	if (ibm_get_config_addr_info2 != RTAS_UNKNOWN_SERVICE) {
		/* Make sure we have a PE in hand */
		ret = rtas_call (ibm_get_config_addr_info2, 4, 2, rets,
			config_addr, info->buid_hi, info->buid_lo, 1);
		if (ret || (rets[0]==0))
			return 0;

		ret = rtas_call (ibm_get_config_addr_info2, 4, 2, rets,
			config_addr, info->buid_hi, info->buid_lo, 0);
		if (ret)
			return 0;
		return rets[0];
	}

	/* Use older config-addr token on power5 */
	if (ibm_get_config_addr_info != RTAS_UNKNOWN_SERVICE) {
		ret = rtas_call (ibm_get_config_addr_info, 4, 2, rets,
			config_addr, info->buid_hi, info->buid_lo, 0);
		if (ret)
			return 0;
		return rets[0];
	}
	return 0;
}

/* Enable eeh for the given device node. */
static void *early_enable_eeh(struct device_node *dn, void *data)
{
	unsigned int rets[3];
	struct eeh_early_enable_info *info = data;
=======
static void eeh_save_bars(struct eeh_dev *edev)
{
	int i;
	struct device_node *dn;

	if (!edev)
		return;
	dn = eeh_dev_to_of_node(edev);
	
	for (i = 0; i < 16; i++)
		eeh_ops->read_config(dn, i * 4, 4, &edev->config_space[i]);
}

/**
 * eeh_early_enable - Early enable EEH on the indicated device
 * @dn: device node
 * @data: BUID
 *
 * Enable EEH functionality on the specified PCI device. The function
 * is expected to be called before real PCI probing is done. However,
 * the PHBs have been initialized at this point.
 */
static void *eeh_early_enable(struct device_node *dn, void *data)
{
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;
	const u32 *class_code = of_get_property(dn, "class-code", NULL);
	const u32 *vendor_id = of_get_property(dn, "vendor-id", NULL);
	const u32 *device_id = of_get_property(dn, "device-id", NULL);
	const u32 *regs;
	int enable;
<<<<<<< HEAD
	struct pci_dn *pdn = PCI_DN(dn);

	pdn->class_code = 0;
	pdn->eeh_mode = 0;
	pdn->eeh_check_count = 0;
	pdn->eeh_freeze_count = 0;
	pdn->eeh_false_positives = 0;
=======
	struct eeh_dev *edev = of_node_to_eeh_dev(dn);

	edev->class_code = 0;
	edev->mode = 0;
	edev->check_count = 0;
	edev->freeze_count = 0;
	edev->false_positives = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!of_device_is_available(dn))
		return NULL;

	/* Ignore bad nodes. */
	if (!class_code || !vendor_id || !device_id)
		return NULL;

	/* There is nothing to check on PCI to ISA bridges */
	if (dn->type && !strcmp(dn->type, "isa")) {
<<<<<<< HEAD
		pdn->eeh_mode |= EEH_MODE_NOCHECK;
		return NULL;
	}
	pdn->class_code = *class_code;

	/* Ok... see if this device supports EEH.  Some do, some don't,
	 * and the only way to find out is to check each and every one. */
=======
		edev->mode |= EEH_MODE_NOCHECK;
		return NULL;
	}
	edev->class_code = *class_code;

	/* Ok... see if this device supports EEH.  Some do, some don't,
	 * and the only way to find out is to check each and every one.
	 */
>>>>>>> refs/remotes/origin/cm-10.0
	regs = of_get_property(dn, "reg", NULL);
	if (regs) {
		/* First register entry is addr (00BBSS00)  */
		/* Try to enable eeh */
<<<<<<< HEAD
		ret = rtas_call(ibm_set_eeh_option, 4, 1, NULL,
		                regs[0], info->buid_hi, info->buid_lo,
		                EEH_ENABLE);

		enable = 0;
		if (ret == 0) {
			pdn->eeh_config_addr = regs[0];

			/* If the newer, better, ibm,get-config-addr-info is supported, 
			 * then use that instead. */
			pdn->eeh_pe_config_addr = get_pe_addr(pdn->eeh_config_addr, info);
=======
		ret = eeh_ops->set_option(dn, EEH_OPT_ENABLE);

		enable = 0;
		if (ret == 0) {
			edev->config_addr = regs[0];

			/* If the newer, better, ibm,get-config-addr-info is supported, 
			 * then use that instead.
			 */
			edev->pe_config_addr = eeh_ops->get_pe_addr(dn);
>>>>>>> refs/remotes/origin/cm-10.0

			/* Some older systems (Power4) allow the
			 * ibm,set-eeh-option call to succeed even on nodes
			 * where EEH is not supported. Verify support
<<<<<<< HEAD
			 * explicitly. */
			ret = read_slot_reset_state(pdn, rets);
			if ((ret == 0) && (rets[1] == 1))
=======
			 * explicitly.
			 */
			ret = eeh_ops->get_state(dn, NULL);
			if (ret > 0 && ret != EEH_STATE_NOT_SUPPORT)
>>>>>>> refs/remotes/origin/cm-10.0
				enable = 1;
		}

		if (enable) {
			eeh_subsystem_enabled = 1;
<<<<<<< HEAD
			pdn->eeh_mode |= EEH_MODE_SUPPORTED;

			pr_debug("EEH: %s: eeh enabled, config=%x pe_config=%x\n",
				 dn->full_name, pdn->eeh_config_addr,
				 pdn->eeh_pe_config_addr);
		} else {

			/* This device doesn't support EEH, but it may have an
			 * EEH parent, in which case we mark it as supported. */
			if (dn->parent && PCI_DN(dn->parent)
			    && (PCI_DN(dn->parent)->eeh_mode & EEH_MODE_SUPPORTED)) {
				/* Parent supports EEH. */
				pdn->eeh_mode |= EEH_MODE_SUPPORTED;
				pdn->eeh_config_addr = PCI_DN(dn->parent)->eeh_config_addr;
=======
			edev->mode |= EEH_MODE_SUPPORTED;

			pr_debug("EEH: %s: eeh enabled, config=%x pe_config=%x\n",
				 dn->full_name, edev->config_addr,
				 edev->pe_config_addr);
		} else {

			/* This device doesn't support EEH, but it may have an
			 * EEH parent, in which case we mark it as supported.
			 */
			if (dn->parent && of_node_to_eeh_dev(dn->parent) &&
			    (of_node_to_eeh_dev(dn->parent)->mode & EEH_MODE_SUPPORTED)) {
				/* Parent supports EEH. */
				edev->mode |= EEH_MODE_SUPPORTED;
				edev->config_addr = of_node_to_eeh_dev(dn->parent)->config_addr;
>>>>>>> refs/remotes/origin/cm-10.0
				return NULL;
			}
		}
	} else {
		printk(KERN_WARNING "EEH: %s: unable to get reg property.\n",
		       dn->full_name);
	}

<<<<<<< HEAD
	eeh_save_bars(pdn);
	return NULL;
}

/*
=======
	eeh_save_bars(edev);
	return NULL;
}

/**
 * eeh_ops_register - Register platform dependent EEH operations
 * @ops: platform dependent EEH operations
 *
 * Register the platform dependent EEH operation callback
 * functions. The platform should call this function before
 * any other EEH operations.
 */
int __init eeh_ops_register(struct eeh_ops *ops)
{
	if (!ops->name) {
		pr_warning("%s: Invalid EEH ops name for %p\n",
			__func__, ops);
		return -EINVAL;
	}

	if (eeh_ops && eeh_ops != ops) {
		pr_warning("%s: EEH ops of platform %s already existing (%s)\n",
			__func__, eeh_ops->name, ops->name);
		return -EEXIST;
	}

	eeh_ops = ops;

	return 0;
}

/**
 * eeh_ops_unregister - Unreigster platform dependent EEH operations
 * @name: name of EEH platform operations
 *
 * Unregister the platform dependent EEH operation callback
 * functions.
 */
int __exit eeh_ops_unregister(const char *name)
{
	if (!name || !strlen(name)) {
		pr_warning("%s: Invalid EEH ops name\n",
			__func__);
		return -EINVAL;
	}

	if (eeh_ops && !strcmp(eeh_ops->name, name)) {
		eeh_ops = NULL;
		return 0;
	}

	return -EEXIST;
}

/**
 * eeh_init - EEH initialization
 *
>>>>>>> refs/remotes/origin/cm-10.0
 * Initialize EEH by trying to enable it for all of the adapters in the system.
 * As a side effect we can determine here if eeh is supported at all.
 * Note that we leave EEH on so failed config cycles won't cause a machine
 * check.  If a user turns off EEH for a particular adapter they are really
 * telling Linux to ignore errors.  Some hardware (e.g. POWER5) won't
 * grant access to a slot if EEH isn't enabled, and so we always enable
 * EEH for all slots/all devices.
 *
 * The eeh-force-off option disables EEH checking globally, for all slots.
 * Even if force-off is set, the EEH hardware is still enabled, so that
 * newer systems can boot.
 */
void __init eeh_init(void)
{
<<<<<<< HEAD
	struct device_node *phb, *np;
	struct eeh_early_enable_info info;

	raw_spin_lock_init(&confirm_error_lock);
	spin_lock_init(&slot_errbuf_lock);

	np = of_find_node_by_path("/rtas");
	if (np == NULL)
		return;

	ibm_set_eeh_option = rtas_token("ibm,set-eeh-option");
	ibm_set_slot_reset = rtas_token("ibm,set-slot-reset");
	ibm_read_slot_reset_state2 = rtas_token("ibm,read-slot-reset-state2");
	ibm_read_slot_reset_state = rtas_token("ibm,read-slot-reset-state");
	ibm_slot_error_detail = rtas_token("ibm,slot-error-detail");
	ibm_get_config_addr_info = rtas_token("ibm,get-config-addr-info");
	ibm_get_config_addr_info2 = rtas_token("ibm,get-config-addr-info2");
	ibm_configure_bridge = rtas_token ("ibm,configure-bridge");
	ibm_configure_pe = rtas_token("ibm,configure-pe");

	if (ibm_set_eeh_option == RTAS_UNKNOWN_SERVICE)
		return;

	eeh_error_buf_size = rtas_token("rtas-error-log-max");
	if (eeh_error_buf_size == RTAS_UNKNOWN_SERVICE) {
		eeh_error_buf_size = 1024;
	}
	if (eeh_error_buf_size > RTAS_ERROR_LOG_MAX) {
		printk(KERN_WARNING "EEH: rtas-error-log-max is bigger than allocated "
		      "buffer ! (%d vs %d)", eeh_error_buf_size, RTAS_ERROR_LOG_MAX);
		eeh_error_buf_size = RTAS_ERROR_LOG_MAX;
	}

	/* Enable EEH for all adapters.  Note that eeh requires buid's */
	for (phb = of_find_node_by_name(NULL, "pci"); phb;
	     phb = of_find_node_by_name(phb, "pci")) {
		unsigned long buid;

		buid = get_phb_buid(phb);
		if (buid == 0 || PCI_DN(phb) == NULL)
			continue;

		info.buid_lo = BUID_LO(buid);
		info.buid_hi = BUID_HI(buid);
		traverse_pci_devices(phb, early_enable_eeh, &info);
=======
	struct pci_controller *hose, *tmp;
	struct device_node *phb;
	int ret;

	/* call platform initialization function */
	if (!eeh_ops) {
		pr_warning("%s: Platform EEH operation not found\n",
			__func__);
		return;
	} else if ((ret = eeh_ops->init())) {
		pr_warning("%s: Failed to call platform init function (%d)\n",
			__func__, ret);
		return;
	}

	raw_spin_lock_init(&confirm_error_lock);

	/* Enable EEH for all adapters */
	list_for_each_entry_safe(hose, tmp, &hose_list, list_node) {
		phb = hose->dn;
		traverse_pci_devices(phb, eeh_early_enable, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	if (eeh_subsystem_enabled)
		printk(KERN_INFO "EEH: PCI Enhanced I/O Error Handling Enabled\n");
	else
		printk(KERN_WARNING "EEH: No capable adapters found\n");
}

/**
<<<<<<< HEAD
 * eeh_add_device_early - enable EEH for the indicated device_node
=======
 * eeh_add_device_early - Enable EEH for the indicated device_node
>>>>>>> refs/remotes/origin/cm-10.0
 * @dn: device node for which to set up EEH
 *
 * This routine must be used to perform EEH initialization for PCI
 * devices that were added after system boot (e.g. hotplug, dlpar).
 * This routine must be called before any i/o is performed to the
 * adapter (inluding any config-space i/o).
 * Whether this actually enables EEH or not for this device depends
 * on the CEC architecture, type of the device, on earlier boot
 * command-line arguments & etc.
 */
static void eeh_add_device_early(struct device_node *dn)
{
	struct pci_controller *phb;
<<<<<<< HEAD
	struct eeh_early_enable_info info;

<<<<<<< HEAD
	if (!dn || !PCI_DN(dn))
		return;
	phb = PCI_DN(dn)->phb;
=======

=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (!of_node_to_eeh_dev(dn))
		return;
	phb = of_node_to_eeh_dev(dn)->phb;
>>>>>>> refs/remotes/origin/cm-10.0

	/* USB Bus children of PCI devices will not have BUID's */
	if (NULL == phb || 0 == phb->buid)
		return;

<<<<<<< HEAD
	info.buid_hi = BUID_HI(phb->buid);
	info.buid_lo = BUID_LO(phb->buid);
	early_enable_eeh(dn, &info);
}

=======
	eeh_early_enable(dn, NULL);
}

/**
 * eeh_add_device_tree_early - Enable EEH for the indicated device
 * @dn: device node
 *
 * This routine must be used to perform EEH initialization for the
 * indicated PCI device that was added after system boot (e.g.
 * hotplug, dlpar).
 */
>>>>>>> refs/remotes/origin/cm-10.0
void eeh_add_device_tree_early(struct device_node *dn)
{
	struct device_node *sib;

	for_each_child_of_node(dn, sib)
		eeh_add_device_tree_early(sib);
	eeh_add_device_early(dn);
}
EXPORT_SYMBOL_GPL(eeh_add_device_tree_early);

/**
<<<<<<< HEAD
 * eeh_add_device_late - perform EEH initialization for the indicated pci device
=======
 * eeh_add_device_late - Perform EEH initialization for the indicated pci device
>>>>>>> refs/remotes/origin/cm-10.0
 * @dev: pci device for which to set up EEH
 *
 * This routine must be used to complete EEH initialization for PCI
 * devices that were added after system boot (e.g. hotplug, dlpar).
 */
static void eeh_add_device_late(struct pci_dev *dev)
{
	struct device_node *dn;
<<<<<<< HEAD
	struct pci_dn *pdn;
=======
	struct eeh_dev *edev;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!dev || !eeh_subsystem_enabled)
		return;

	pr_debug("EEH: Adding device %s\n", pci_name(dev));

	dn = pci_device_to_OF_node(dev);
<<<<<<< HEAD
	pdn = PCI_DN(dn);
	if (pdn->pcidev == dev) {
		pr_debug("EEH: Already referenced !\n");
		return;
	}
	WARN_ON(pdn->pcidev);

	pci_dev_get (dev);
	pdn->pcidev = dev;
=======
	edev = of_node_to_eeh_dev(dn);
	if (edev->pdev == dev) {
		pr_debug("EEH: Already referenced !\n");
		return;
	}
	WARN_ON(edev->pdev);

	pci_dev_get(dev);
	edev->pdev = dev;
	dev->dev.archdata.edev = edev;
>>>>>>> refs/remotes/origin/cm-10.0

	pci_addr_cache_insert_device(dev);
	eeh_sysfs_add_device(dev);
}

<<<<<<< HEAD
=======
/**
 * eeh_add_device_tree_late - Perform EEH initialization for the indicated PCI bus
 * @bus: PCI bus
 *
 * This routine must be used to perform EEH initialization for PCI
 * devices which are attached to the indicated PCI bus. The PCI bus
 * is added after system boot through hotplug or dlpar.
 */
>>>>>>> refs/remotes/origin/cm-10.0
void eeh_add_device_tree_late(struct pci_bus *bus)
{
	struct pci_dev *dev;

	list_for_each_entry(dev, &bus->devices, bus_list) {
 		eeh_add_device_late(dev);
 		if (dev->hdr_type == PCI_HEADER_TYPE_BRIDGE) {
 			struct pci_bus *subbus = dev->subordinate;
 			if (subbus)
 				eeh_add_device_tree_late(subbus);
 		}
	}
}
EXPORT_SYMBOL_GPL(eeh_add_device_tree_late);

/**
<<<<<<< HEAD
 * eeh_remove_device - undo EEH setup for the indicated pci device
=======
 * eeh_remove_device - Undo EEH setup for the indicated pci device
>>>>>>> refs/remotes/origin/cm-10.0
 * @dev: pci device to be removed
 *
 * This routine should be called when a device is removed from
 * a running system (e.g. by hotplug or dlpar).  It unregisters
 * the PCI device from the EEH subsystem.  I/O errors affecting
 * this device will no longer be detected after this call; thus,
 * i/o errors affecting this slot may leave this device unusable.
 */
static void eeh_remove_device(struct pci_dev *dev)
{
<<<<<<< HEAD
	struct device_node *dn;
	if (!dev || !eeh_subsystem_enabled)
		return;
=======
	struct eeh_dev *edev;

	if (!dev || !eeh_subsystem_enabled)
		return;
	edev = pci_dev_to_eeh_dev(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Unregister the device with the EEH/PCI address search system */
	pr_debug("EEH: Removing device %s\n", pci_name(dev));

<<<<<<< HEAD
	dn = pci_device_to_OF_node(dev);
	if (PCI_DN(dn)->pcidev == NULL) {
		pr_debug("EEH: Not referenced !\n");
		return;
	}
	PCI_DN(dn)->pcidev = NULL;
	pci_dev_put (dev);
=======
	if (!edev || !edev->pdev) {
		pr_debug("EEH: Not referenced !\n");
		return;
	}
	edev->pdev = NULL;
	dev->dev.archdata.edev = NULL;
	pci_dev_put(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	pci_addr_cache_remove_device(dev);
	eeh_sysfs_remove_device(dev);
}

<<<<<<< HEAD
=======
/**
 * eeh_remove_bus_device - Undo EEH setup for the indicated PCI device
 * @dev: PCI device
 *
 * This routine must be called when a device is removed from the
 * running system through hotplug or dlpar. The corresponding
 * PCI address cache will be removed.
 */
>>>>>>> refs/remotes/origin/cm-10.0
void eeh_remove_bus_device(struct pci_dev *dev)
{
	struct pci_bus *bus = dev->subordinate;
	struct pci_dev *child, *tmp;

	eeh_remove_device(dev);

	if (bus && dev->hdr_type == PCI_HEADER_TYPE_BRIDGE) {
		list_for_each_entry_safe(child, tmp, &bus->devices, bus_list)
			 eeh_remove_bus_device(child);
	}
}
EXPORT_SYMBOL_GPL(eeh_remove_bus_device);

static int proc_eeh_show(struct seq_file *m, void *v)
{
	if (0 == eeh_subsystem_enabled) {
		seq_printf(m, "EEH Subsystem is globally disabled\n");
<<<<<<< HEAD
		seq_printf(m, "eeh_total_mmio_ffs=%ld\n", total_mmio_ffs);
	} else {
		seq_printf(m, "EEH Subsystem is enabled\n");
		seq_printf(m,
				"no device=%ld\n"
				"no device node=%ld\n"
				"no config address=%ld\n"
				"check not wanted=%ld\n"
				"eeh_total_mmio_ffs=%ld\n"
				"eeh_false_positives=%ld\n"
				"eeh_slot_resets=%ld\n",
				no_device, no_dn, no_cfg_addr, 
				ignored_check, total_mmio_ffs, 
				false_positives,
				slot_resets);
=======
		seq_printf(m, "eeh_total_mmio_ffs=%llu\n", eeh_stats.total_mmio_ffs);
	} else {
		seq_printf(m, "EEH Subsystem is enabled\n");
		seq_printf(m,
				"no device=%llu\n"
				"no device node=%llu\n"
				"no config address=%llu\n"
				"check not wanted=%llu\n"
				"eeh_total_mmio_ffs=%llu\n"
				"eeh_false_positives=%llu\n"
				"eeh_slot_resets=%llu\n",
				eeh_stats.no_device,
				eeh_stats.no_dn,
				eeh_stats.no_cfg_addr,
				eeh_stats.ignored_check,
				eeh_stats.total_mmio_ffs,
				eeh_stats.false_positives,
				eeh_stats.slot_resets);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return 0;
}

static int proc_eeh_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_eeh_show, NULL);
}

static const struct file_operations proc_eeh_operations = {
	.open      = proc_eeh_open,
	.read      = seq_read,
	.llseek    = seq_lseek,
	.release   = single_release,
};

static int __init eeh_init_proc(void)
{
	if (machine_is(pseries))
		proc_create("powerpc/eeh", 0, NULL, &proc_eeh_operations);
	return 0;
}
__initcall(eeh_init_proc);
