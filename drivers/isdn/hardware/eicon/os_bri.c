/* $Id: os_bri.c,v 1.21 2004/03/21 17:26:01 armin Exp $ */

#include "platform.h"
#include "debuglib.h"
#include "cardtype.h"
#include "pc.h"
#include "pr_pc.h"
#include "di_defs.h"
#include "dsp_defs.h"
#include "di.h"
#include "io.h"

#include "xdi_msg.h"
#include "xdi_adapter.h"
#include "os_bri.h"
#include "diva_pci.h"
#include "mi_pc.h"
#include "pc_maint.h"
#include "dsrv_bri.h"

/*
**  IMPORTS
*/
extern void prepare_maestra_functions(PISDN_ADAPTER IoAdapter);
extern void diva_xdi_display_adapter_features(int card);
<<<<<<< HEAD
<<<<<<< HEAD
extern int diva_card_read_xlog(diva_os_xdi_adapter_t * a);
=======
extern int diva_card_read_xlog(diva_os_xdi_adapter_t *a);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int diva_card_read_xlog(diva_os_xdi_adapter_t *a);
>>>>>>> refs/remotes/origin/master

/*
**  LOCALS
*/
static int bri_bar_length[3] = {
	0x80,
	0x80,
	0x20
};
<<<<<<< HEAD
<<<<<<< HEAD
static int diva_bri_cleanup_adapter(diva_os_xdi_adapter_t * a);
static dword diva_bri_get_serial_number(diva_os_xdi_adapter_t * a);
static int diva_bri_cmd_card_proc(struct _diva_os_xdi_adapter *a,
				  diva_xdi_um_cfg_cmd_t * cmd, int length);
static int diva_bri_reregister_io(diva_os_xdi_adapter_t * a);
static int diva_bri_reset_adapter(PISDN_ADAPTER IoAdapter);
static int diva_bri_write_sdram_block(PISDN_ADAPTER IoAdapter,
				      dword address,
				      const byte * data, dword length);
static int diva_bri_start_adapter(PISDN_ADAPTER IoAdapter,
				  dword start_address, dword features);
static int diva_bri_stop_adapter(diva_os_xdi_adapter_t * a);

static void diva_bri_set_addresses(diva_os_xdi_adapter_t * a)
=======
=======
>>>>>>> refs/remotes/origin/master
static int diva_bri_cleanup_adapter(diva_os_xdi_adapter_t *a);
static dword diva_bri_get_serial_number(diva_os_xdi_adapter_t *a);
static int diva_bri_cmd_card_proc(struct _diva_os_xdi_adapter *a,
				  diva_xdi_um_cfg_cmd_t *cmd, int length);
static int diva_bri_reregister_io(diva_os_xdi_adapter_t *a);
static int diva_bri_reset_adapter(PISDN_ADAPTER IoAdapter);
static int diva_bri_write_sdram_block(PISDN_ADAPTER IoAdapter,
				      dword address,
				      const byte *data, dword length);
static int diva_bri_start_adapter(PISDN_ADAPTER IoAdapter,
				  dword start_address, dword features);
static int diva_bri_stop_adapter(diva_os_xdi_adapter_t *a);

static void diva_bri_set_addresses(diva_os_xdi_adapter_t *a)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	a->resources.pci.mem_type_id[MEM_TYPE_RAM] = 0;
	a->resources.pci.mem_type_id[MEM_TYPE_CFG] = 1;
	a->resources.pci.mem_type_id[MEM_TYPE_ADDRESS] = 2;
	a->resources.pci.mem_type_id[MEM_TYPE_RESET] = 1;
	a->resources.pci.mem_type_id[MEM_TYPE_PORT] = 2;
	a->resources.pci.mem_type_id[MEM_TYPE_CTLREG] = 2;
<<<<<<< HEAD
<<<<<<< HEAD
	
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	a->xdi_adapter.ram = a->resources.pci.addr[0];
	a->xdi_adapter.cfg = a->resources.pci.addr[1];
	a->xdi_adapter.Address = a->resources.pci.addr[2];

	a->xdi_adapter.reset = a->xdi_adapter.cfg;
	a->xdi_adapter.port = a->xdi_adapter.Address;

	a->xdi_adapter.ctlReg = a->xdi_adapter.port + M_PCI_RESET;

	a->xdi_adapter.reset += 0x4C;	/* PLX 9050 !! */
}

/*
**  BAR0 - MEM Addr  - 0x80  - NOT USED
**  BAR1 - I/O Addr  - 0x80
**  BAR2 - I/O Addr  - 0x20
*/
<<<<<<< HEAD
<<<<<<< HEAD
int diva_bri_init_card(diva_os_xdi_adapter_t * a)
=======
int diva_bri_init_card(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int diva_bri_init_card(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	int bar;
	dword bar2 = 0, bar2_length = 0xffffffff;
	word cmd = 0, cmd_org;
	byte Bus, Slot;
	void *hdev;
	byte __iomem *p;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Set properties
	 */
	a->xdi_adapter.Properties = CardProperties[a->CardOrdinal];
	DBG_LOG(("Load %s", a->xdi_adapter.Properties.Name))

	    /*
	       Get resources
	     */
	    for (bar = 0; bar < 3; bar++) {
		a->resources.pci.bar[bar] =
		    divasa_get_pci_bar(a->resources.pci.bus,
				       a->resources.pci.func, bar,
				       a->resources.pci.hdev);
		if (!a->resources.pci.bar[bar]) {
			DBG_ERR(("A: can't get BAR[%d]", bar))
			return (-1);
		}
	}

	a->resources.pci.irq =
	    (byte) divasa_get_pci_irq(a->resources.pci.bus,
				      a->resources.pci.func,
				      a->resources.pci.hdev);
=======
=======
>>>>>>> refs/remotes/origin/master
	  Set properties
	*/
	a->xdi_adapter.Properties = CardProperties[a->CardOrdinal];
	DBG_LOG(("Load %s", a->xdi_adapter.Properties.Name))

		/*
		  Get resources
		*/
		for (bar = 0; bar < 3; bar++) {
			a->resources.pci.bar[bar] =
				divasa_get_pci_bar(a->resources.pci.bus,
						   a->resources.pci.func, bar,
						   a->resources.pci.hdev);
			if (!a->resources.pci.bar[bar]) {
				DBG_ERR(("A: can't get BAR[%d]", bar))
					return (-1);
			}
		}

	a->resources.pci.irq =
		(byte) divasa_get_pci_irq(a->resources.pci.bus,
					  a->resources.pci.func,
					  a->resources.pci.hdev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!a->resources.pci.irq) {
		DBG_ERR(("A: invalid irq"));
		return (-1);
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Get length of I/O bar 2 - it is different by older
	   EEPROM version
	 */
=======
	  Get length of I/O bar 2 - it is different by older
	  EEPROM version
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Get length of I/O bar 2 - it is different by older
	  EEPROM version
	*/
>>>>>>> refs/remotes/origin/master
	Bus = a->resources.pci.bus;
	Slot = a->resources.pci.func;
	hdev = a->resources.pci.hdev;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Get plain original values of the BAR2 CDM registers
	 */
	PCIread(Bus, Slot, 0x18, &bar2, sizeof(bar2), hdev);
	PCIread(Bus, Slot, 0x04, &cmd_org, sizeof(cmd_org), hdev);
	/*
	   Disable device and get BAR2 length
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
	  Get plain original values of the BAR2 CDM registers
	*/
	PCIread(Bus, Slot, 0x18, &bar2, sizeof(bar2), hdev);
	PCIread(Bus, Slot, 0x04, &cmd_org, sizeof(cmd_org), hdev);
	/*
	  Disable device and get BAR2 length
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	PCIwrite(Bus, Slot, 0x04, &cmd, sizeof(cmd), hdev);
	PCIwrite(Bus, Slot, 0x18, &bar2_length, sizeof(bar2_length), hdev);
	PCIread(Bus, Slot, 0x18, &bar2_length, sizeof(bar2_length), hdev);
	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Restore BAR2 and CMD registers
	 */
=======
	  Restore BAR2 and CMD registers
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Restore BAR2 and CMD registers
	*/
>>>>>>> refs/remotes/origin/master
	PCIwrite(Bus, Slot, 0x18, &bar2, sizeof(bar2), hdev);
	PCIwrite(Bus, Slot, 0x04, &cmd_org, sizeof(cmd_org), hdev);

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Calculate BAR2 length
	 */
	bar2_length = (~(bar2_length & ~7)) + 1;
	DBG_LOG(("BAR[2] length=%lx", bar2_length))

	    /*
	       Map and register resources
	     */
	    if (!(a->resources.pci.addr[0] =
		 divasa_remap_pci_bar(a, 0, a->resources.pci.bar[0],
				      bri_bar_length[0]))) {
		DBG_ERR(("A: BRI, can't map BAR[0]"))
		diva_bri_cleanup_adapter(a);
		return (-1);
	}
=======
=======
>>>>>>> refs/remotes/origin/master
	  Calculate BAR2 length
	*/
	bar2_length = (~(bar2_length & ~7)) + 1;
	DBG_LOG(("BAR[2] length=%lx", bar2_length))

		/*
		  Map and register resources
		*/
		if (!(a->resources.pci.addr[0] =
		      divasa_remap_pci_bar(a, 0, a->resources.pci.bar[0],
					   bri_bar_length[0]))) {
			DBG_ERR(("A: BRI, can't map BAR[0]"))
				diva_bri_cleanup_adapter(a);
			return (-1);
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	sprintf(&a->port_name[0], "BRI %02x:%02x",
		a->resources.pci.bus, a->resources.pci.func);

	if (diva_os_register_io_port(a, 1, a->resources.pci.bar[1],
				     bri_bar_length[1], &a->port_name[0], 1)) {
		DBG_ERR(("A: BRI, can't register BAR[1]"))
<<<<<<< HEAD
<<<<<<< HEAD
		diva_bri_cleanup_adapter(a);
=======
			diva_bri_cleanup_adapter(a);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_bri_cleanup_adapter(a);
>>>>>>> refs/remotes/origin/master
		return (-1);
	}
	a->resources.pci.addr[1] = (void *) (unsigned long) a->resources.pci.bar[1];
	a->resources.pci.length[1] = bri_bar_length[1];

	if (diva_os_register_io_port(a, 1, a->resources.pci.bar[2],
				     bar2_length, &a->port_name[0], 2)) {
		DBG_ERR(("A: BRI, can't register BAR[2]"))
<<<<<<< HEAD
<<<<<<< HEAD
		diva_bri_cleanup_adapter(a);
=======
			diva_bri_cleanup_adapter(a);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_bri_cleanup_adapter(a);
>>>>>>> refs/remotes/origin/master
		return (-1);
	}
	a->resources.pci.addr[2] = (void *) (unsigned long) a->resources.pci.bar[2];
	a->resources.pci.length[2] = bar2_length;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Set all memory areas
	 */
	diva_bri_set_addresses(a);

	/*
	   Get Serial Number
	 */
	a->xdi_adapter.serialNo = diva_bri_get_serial_number(a);

	/*
	   Register I/O ports with correct name now
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
	  Set all memory areas
	*/
	diva_bri_set_addresses(a);

	/*
	  Get Serial Number
	*/
	a->xdi_adapter.serialNo = diva_bri_get_serial_number(a);

	/*
	  Register I/O ports with correct name now
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (diva_bri_reregister_io(a)) {
		diva_bri_cleanup_adapter(a);
		return (-1);
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Initialize OS dependent objects
	 */
=======
	  Initialize OS dependent objects
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Initialize OS dependent objects
	*/
>>>>>>> refs/remotes/origin/master
	if (diva_os_initialize_spin_lock
	    (&a->xdi_adapter.isr_spin_lock, "isr")) {
		diva_bri_cleanup_adapter(a);
		return (-1);
	}
	if (diva_os_initialize_spin_lock
	    (&a->xdi_adapter.data_spin_lock, "data")) {
		diva_bri_cleanup_adapter(a);
		return (-1);
	}

	strcpy(a->xdi_adapter.req_soft_isr.dpc_thread_name, "kdivasbrid");

	if (diva_os_initialize_soft_isr(&a->xdi_adapter.req_soft_isr,
					DIDpcRoutine, &a->xdi_adapter)) {
		diva_bri_cleanup_adapter(a);
		return (-1);
	}
	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Do not initialize second DPC - only one thread will be created
	 */
	a->xdi_adapter.isr_soft_isr.object = a->xdi_adapter.req_soft_isr.object;

	/*
	   Create entity table
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
	  Do not initialize second DPC - only one thread will be created
	*/
	a->xdi_adapter.isr_soft_isr.object = a->xdi_adapter.req_soft_isr.object;

	/*
	  Create entity table
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	a->xdi_adapter.Channels = CardProperties[a->CardOrdinal].Channels;
	a->xdi_adapter.e_max = CardProperties[a->CardOrdinal].E_info;
	a->xdi_adapter.e_tbl = diva_os_malloc(0, a->xdi_adapter.e_max * sizeof(E_INFO));
	if (!a->xdi_adapter.e_tbl) {
		diva_bri_cleanup_adapter(a);
		return (-1);
	}
	memset(a->xdi_adapter.e_tbl, 0x00, a->xdi_adapter.e_max * sizeof(E_INFO));

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Set up interface
	 */
=======
	  Set up interface
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Set up interface
	*/
>>>>>>> refs/remotes/origin/master
	a->xdi_adapter.a.io = &a->xdi_adapter;
	a->xdi_adapter.DIRequest = request;
	a->interface.cleanup_adapter_proc = diva_bri_cleanup_adapter;
	a->interface.cmd_proc = diva_bri_cmd_card_proc;

	p = DIVA_OS_MEM_ATTACH_RESET(&a->xdi_adapter);
	outpp(p, 0x41);
	DIVA_OS_MEM_DETACH_RESET(&a->xdi_adapter, p);

	prepare_maestra_functions(&a->xdi_adapter);

	a->dsp_mask = 0x00000003;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Set IRQ handler
	 */
=======
	  Set IRQ handler
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Set IRQ handler
	*/
>>>>>>> refs/remotes/origin/master
	a->xdi_adapter.irq_info.irq_nr = a->resources.pci.irq;
	sprintf(a->xdi_adapter.irq_info.irq_name, "DIVA BRI %ld",
		(long) a->xdi_adapter.serialNo);
	if (diva_os_register_irq(a, a->xdi_adapter.irq_info.irq_nr,
				 a->xdi_adapter.irq_info.irq_name)) {
		diva_bri_cleanup_adapter(a);
		return (-1);
	}
	a->xdi_adapter.irq_info.registered = 1;

	diva_log_info("%s IRQ:%d SerNo:%d", a->xdi_adapter.Properties.Name,
		      a->resources.pci.irq, a->xdi_adapter.serialNo);

	return (0);
}


<<<<<<< HEAD
<<<<<<< HEAD
static int diva_bri_cleanup_adapter(diva_os_xdi_adapter_t * a)
=======
static int diva_bri_cleanup_adapter(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int diva_bri_cleanup_adapter(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	int i;

	if (a->xdi_adapter.Initialized) {
		diva_bri_stop_adapter(a);
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Remove ISR Handler
	 */
=======
	  Remove ISR Handler
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Remove ISR Handler
	*/
>>>>>>> refs/remotes/origin/master
	if (a->xdi_adapter.irq_info.registered) {
		diva_os_remove_irq(a, a->xdi_adapter.irq_info.irq_nr);
	}
	a->xdi_adapter.irq_info.registered = 0;

	if (a->resources.pci.addr[0] && a->resources.pci.bar[0]) {
		divasa_unmap_pci_bar(a->resources.pci.addr[0]);
		a->resources.pci.addr[0] = NULL;
		a->resources.pci.bar[0] = 0;
	}

	for (i = 1; i < 3; i++) {
		if (a->resources.pci.addr[i] && a->resources.pci.bar[i]) {
			diva_os_register_io_port(a, 0,
						 a->resources.pci.bar[i],
						 a->resources.pci.
						 length[i],
						 &a->port_name[0], i);
			a->resources.pci.addr[i] = NULL;
			a->resources.pci.bar[i] = 0;
		}
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Free OS objects
	 */
=======
	  Free OS objects
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Free OS objects
	*/
>>>>>>> refs/remotes/origin/master
	diva_os_cancel_soft_isr(&a->xdi_adapter.req_soft_isr);
	diva_os_cancel_soft_isr(&a->xdi_adapter.isr_soft_isr);

	diva_os_remove_soft_isr(&a->xdi_adapter.req_soft_isr);
	a->xdi_adapter.isr_soft_isr.object = NULL;

	diva_os_destroy_spin_lock(&a->xdi_adapter.isr_spin_lock, "rm");
	diva_os_destroy_spin_lock(&a->xdi_adapter.data_spin_lock, "rm");

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Free memory
	 */
=======
	  Free memory
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Free memory
	*/
>>>>>>> refs/remotes/origin/master
	if (a->xdi_adapter.e_tbl) {
		diva_os_free(0, a->xdi_adapter.e_tbl);
		a->xdi_adapter.e_tbl = NULL;
	}

	return (0);
}

void diva_os_prepare_maestra_functions(PISDN_ADAPTER IoAdapter)
{
}

/*
**  Get serial number
*/
<<<<<<< HEAD
<<<<<<< HEAD
static dword diva_bri_get_serial_number(diva_os_xdi_adapter_t * a)
=======
static dword diva_bri_get_serial_number(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static dword diva_bri_get_serial_number(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	dword serNo = 0;
	byte __iomem *confIO;
	word serHi, serLo;
	word __iomem *confMem;

	confIO = DIVA_OS_MEM_ATTACH_CFG(&a->xdi_adapter);
	serHi = (word) (inppw(&confIO[0x22]) & 0x0FFF);
	serLo = (word) (inppw(&confIO[0x26]) & 0x0FFF);
	serNo = ((dword) serHi << 16) | (dword) serLo;
	DIVA_OS_MEM_DETACH_CFG(&a->xdi_adapter, confIO);

	if ((serNo == 0) || (serNo == 0xFFFFFFFF)) {
		DBG_FTL(("W: BRI use BAR[0] to get card serial number"))

<<<<<<< HEAD
<<<<<<< HEAD
		confMem = (word __iomem *)DIVA_OS_MEM_ATTACH_RAM(&a->xdi_adapter);
=======
			confMem = (word __iomem *)DIVA_OS_MEM_ATTACH_RAM(&a->xdi_adapter);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			confMem = (word __iomem *)DIVA_OS_MEM_ATTACH_RAM(&a->xdi_adapter);
>>>>>>> refs/remotes/origin/master
		serHi = (word) (READ_WORD(&confMem[0x11]) & 0x0FFF);
		serLo = (word) (READ_WORD(&confMem[0x13]) & 0x0FFF);
		serNo = (((dword) serHi) << 16) | ((dword) serLo);
		DIVA_OS_MEM_DETACH_RAM(&a->xdi_adapter, confMem);
	}

	DBG_LOG(("Serial Number=%ld", serNo))

<<<<<<< HEAD
<<<<<<< HEAD
	return (serNo);
=======
		return (serNo);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return (serNo);
>>>>>>> refs/remotes/origin/master
}

/*
**  Unregister I/O and register it with new name,
**  based on Serial Number
*/
<<<<<<< HEAD
<<<<<<< HEAD
static int diva_bri_reregister_io(diva_os_xdi_adapter_t * a)
=======
static int diva_bri_reregister_io(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int diva_bri_reregister_io(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	int i;

	for (i = 1; i < 3; i++) {
		diva_os_register_io_port(a, 0, a->resources.pci.bar[i],
					 a->resources.pci.length[i],
					 &a->port_name[0], i);
		a->resources.pci.addr[i] = NULL;
	}

	sprintf(a->port_name, "DIVA BRI %ld",
		(long) a->xdi_adapter.serialNo);

	for (i = 1; i < 3; i++) {
		if (diva_os_register_io_port(a, 1, a->resources.pci.bar[i],
					     a->resources.pci.length[i],
					     &a->port_name[0], i)) {
			DBG_ERR(("A: failed to reregister BAR[%d]", i))
<<<<<<< HEAD
<<<<<<< HEAD
			return (-1);
		}
		a->resources.pci.addr[i] =
		    (void *) (unsigned long) a->resources.pci.bar[i];
=======
=======
>>>>>>> refs/remotes/origin/master
				return (-1);
		}
		a->resources.pci.addr[i] =
			(void *) (unsigned long) a->resources.pci.bar[i];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return (0);
}

/*
**  Process command from user mode
*/
static int
diva_bri_cmd_card_proc(struct _diva_os_xdi_adapter *a,
<<<<<<< HEAD
<<<<<<< HEAD
		       diva_xdi_um_cfg_cmd_t * cmd, int length)
=======
		       diva_xdi_um_cfg_cmd_t *cmd, int length)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       diva_xdi_um_cfg_cmd_t *cmd, int length)
>>>>>>> refs/remotes/origin/master
{
	int ret = -1;

	if (cmd->adapter != a->controller) {
		DBG_ERR(("A: pri_cmd, invalid controller=%d != %d",
			 cmd->adapter, a->controller))
<<<<<<< HEAD
<<<<<<< HEAD
		return (-1);
=======
			return (-1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			return (-1);
>>>>>>> refs/remotes/origin/master
	}

	switch (cmd->command) {
	case DIVA_XDI_UM_CMD_GET_CARD_ORDINAL:
		a->xdi_mbox.data_length = sizeof(dword);
		a->xdi_mbox.data =
<<<<<<< HEAD
<<<<<<< HEAD
		    diva_os_malloc(0, a->xdi_mbox.data_length);
		if (a->xdi_mbox.data) {
			*(dword *) a->xdi_mbox.data =
			    (dword) a->CardOrdinal;
=======
=======
>>>>>>> refs/remotes/origin/master
			diva_os_malloc(0, a->xdi_mbox.data_length);
		if (a->xdi_mbox.data) {
			*(dword *) a->xdi_mbox.data =
				(dword) a->CardOrdinal;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			a->xdi_mbox.status = DIVA_XDI_MBOX_BUSY;
			ret = 0;
		}
		break;

	case DIVA_XDI_UM_CMD_GET_SERIAL_NR:
		a->xdi_mbox.data_length = sizeof(dword);
		a->xdi_mbox.data =
<<<<<<< HEAD
<<<<<<< HEAD
		    diva_os_malloc(0, a->xdi_mbox.data_length);
		if (a->xdi_mbox.data) {
			*(dword *) a->xdi_mbox.data =
			    (dword) a->xdi_adapter.serialNo;
=======
=======
>>>>>>> refs/remotes/origin/master
			diva_os_malloc(0, a->xdi_mbox.data_length);
		if (a->xdi_mbox.data) {
			*(dword *) a->xdi_mbox.data =
				(dword) a->xdi_adapter.serialNo;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			a->xdi_mbox.status = DIVA_XDI_MBOX_BUSY;
			ret = 0;
		}
		break;

	case DIVA_XDI_UM_CMD_GET_PCI_HW_CONFIG:
		a->xdi_mbox.data_length = sizeof(dword) * 9;
		a->xdi_mbox.data =
<<<<<<< HEAD
<<<<<<< HEAD
		    diva_os_malloc(0, a->xdi_mbox.data_length);
=======
			diva_os_malloc(0, a->xdi_mbox.data_length);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_os_malloc(0, a->xdi_mbox.data_length);
>>>>>>> refs/remotes/origin/master
		if (a->xdi_mbox.data) {
			int i;
			dword *data = (dword *) a->xdi_mbox.data;

			for (i = 0; i < 8; i++) {
				*data++ = a->resources.pci.bar[i];
			}
			*data++ = (dword) a->resources.pci.irq;
			a->xdi_mbox.status = DIVA_XDI_MBOX_BUSY;
			ret = 0;
		}
		break;

	case DIVA_XDI_UM_CMD_GET_CARD_STATE:
		a->xdi_mbox.data_length = sizeof(dword);
		a->xdi_mbox.data =
<<<<<<< HEAD
<<<<<<< HEAD
		    diva_os_malloc(0, a->xdi_mbox.data_length);
=======
			diva_os_malloc(0, a->xdi_mbox.data_length);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_os_malloc(0, a->xdi_mbox.data_length);
>>>>>>> refs/remotes/origin/master
		if (a->xdi_mbox.data) {
			dword *data = (dword *) a->xdi_mbox.data;
			if (!a->xdi_adapter.port) {
				*data = 3;
			} else if (a->xdi_adapter.trapped) {
				*data = 2;
			} else if (a->xdi_adapter.Initialized) {
				*data = 1;
			} else {
				*data = 0;
			}
			a->xdi_mbox.status = DIVA_XDI_MBOX_BUSY;
			ret = 0;
		}
		break;

	case DIVA_XDI_UM_CMD_RESET_ADAPTER:
		ret = diva_bri_reset_adapter(&a->xdi_adapter);
		break;

	case DIVA_XDI_UM_CMD_WRITE_SDRAM_BLOCK:
		ret = diva_bri_write_sdram_block(&a->xdi_adapter,
						 cmd->command_data.
						 write_sdram.offset,
<<<<<<< HEAD
<<<<<<< HEAD
						 (byte *) & cmd[1],
=======
						 (byte *)&cmd[1],
>>>>>>> refs/remotes/origin/cm-10.0
=======
						 (byte *)&cmd[1],
>>>>>>> refs/remotes/origin/master
						 cmd->command_data.
						 write_sdram.length);
		break;

	case DIVA_XDI_UM_CMD_START_ADAPTER:
		ret = diva_bri_start_adapter(&a->xdi_adapter,
					     cmd->command_data.start.
					     offset,
					     cmd->command_data.start.
					     features);
		break;

	case DIVA_XDI_UM_CMD_SET_PROTOCOL_FEATURES:
		a->xdi_adapter.features =
<<<<<<< HEAD
<<<<<<< HEAD
		    cmd->command_data.features.features;
		a->xdi_adapter.a.protocol_capabilities =
		    a->xdi_adapter.features;
=======
			cmd->command_data.features.features;
		a->xdi_adapter.a.protocol_capabilities =
			a->xdi_adapter.features;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			cmd->command_data.features.features;
		a->xdi_adapter.a.protocol_capabilities =
			a->xdi_adapter.features;
>>>>>>> refs/remotes/origin/master
		DBG_TRC(
			("Set raw protocol features (%08x)",
			 a->xdi_adapter.features)) ret = 0;
		break;

	case DIVA_XDI_UM_CMD_STOP_ADAPTER:
		ret = diva_bri_stop_adapter(a);
		break;

	case DIVA_XDI_UM_CMD_READ_XLOG_ENTRY:
		ret = diva_card_read_xlog(a);
		break;

	default:
		DBG_ERR(
			("A: A(%d) invalid cmd=%d", a->controller,
			 cmd->command))}

	return (ret);
}

static int diva_bri_reset_adapter(PISDN_ADAPTER IoAdapter)
{
	byte __iomem *addrHi, *addrLo, *ioaddr;
	dword i;
	byte __iomem *Port;

	if (!IoAdapter->port) {
		return (-1);
	}
	if (IoAdapter->Initialized) {
		DBG_ERR(("A: A(%d) can't reset BRI adapter - please stop first",
			 IoAdapter->ANum)) return (-1);
	}
	(*(IoAdapter->rstFnc)) (IoAdapter);
	diva_os_wait(100);
	Port = DIVA_OS_MEM_ATTACH_PORT(IoAdapter);
	addrHi = Port +
<<<<<<< HEAD
<<<<<<< HEAD
	    ((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
	addrLo = Port + ADDR;
	ioaddr = Port + DATA;
	/*
	   recover
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
		((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
	addrLo = Port + ADDR;
	ioaddr = Port + DATA;
	/*
	  recover
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	outpp(addrHi, (byte) 0);
	outppw(addrLo, (word) 0);
	outppw(ioaddr, (word) 0);
	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   clear shared memory
	 */
=======
	  clear shared memory
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  clear shared memory
	*/
>>>>>>> refs/remotes/origin/master
	outpp(addrHi,
	      (byte) (
		      (IoAdapter->MemoryBase + IoAdapter->MemorySize -
		       BRI_SHARED_RAM_SIZE) >> 16));
	outppw(addrLo, 0);
	for (i = 0; i < 0x8000; outppw(ioaddr, 0), ++i);
	diva_os_wait(100);

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   clear signature
	 */
=======
	  clear signature
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  clear signature
	*/
>>>>>>> refs/remotes/origin/master
	outpp(addrHi,
	      (byte) (
		      (IoAdapter->MemoryBase + IoAdapter->MemorySize -
		       BRI_SHARED_RAM_SIZE) >> 16));
	outppw(addrLo, 0x1e);
	outpp(ioaddr, 0);
	outpp(ioaddr, 0);

	outpp(addrHi, (byte) 0);
	outppw(addrLo, (word) 0);
	outppw(ioaddr, (word) 0);

	DIVA_OS_MEM_DETACH_PORT(IoAdapter, Port);

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Forget all outstanding entities
	 */
=======
	  Forget all outstanding entities
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Forget all outstanding entities
	*/
>>>>>>> refs/remotes/origin/master
	IoAdapter->e_count = 0;
	if (IoAdapter->e_tbl) {
		memset(IoAdapter->e_tbl, 0x00,
		       IoAdapter->e_max * sizeof(E_INFO));
	}
	IoAdapter->head = 0;
	IoAdapter->tail = 0;
	IoAdapter->assign = 0;
	IoAdapter->trapped = 0;

	memset(&IoAdapter->a.IdTable[0], 0x00,
	       sizeof(IoAdapter->a.IdTable));
	memset(&IoAdapter->a.IdTypeTable[0], 0x00,
	       sizeof(IoAdapter->a.IdTypeTable));
	memset(&IoAdapter->a.FlowControlIdTable[0], 0x00,
	       sizeof(IoAdapter->a.FlowControlIdTable));
	memset(&IoAdapter->a.FlowControlSkipTable[0], 0x00,
	       sizeof(IoAdapter->a.FlowControlSkipTable));
	memset(&IoAdapter->a.misc_flags_table[0], 0x00,
	       sizeof(IoAdapter->a.misc_flags_table));
	memset(&IoAdapter->a.rx_stream[0], 0x00,
	       sizeof(IoAdapter->a.rx_stream));
	memset(&IoAdapter->a.tx_stream[0], 0x00,
	       sizeof(IoAdapter->a.tx_stream));
	memset(&IoAdapter->a.tx_pos[0], 0x00, sizeof(IoAdapter->a.tx_pos));
	memset(&IoAdapter->a.rx_pos[0], 0x00, sizeof(IoAdapter->a.rx_pos));

	return (0);
}

static int
diva_bri_write_sdram_block(PISDN_ADAPTER IoAdapter,
<<<<<<< HEAD
<<<<<<< HEAD
			   dword address, const byte * data, dword length)
=======
			   dword address, const byte *data, dword length)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			   dword address, const byte *data, dword length)
>>>>>>> refs/remotes/origin/master
{
	byte __iomem *addrHi, *addrLo, *ioaddr;
	byte __iomem *Port;

	if (!IoAdapter->port) {
		return (-1);
	}

	Port = DIVA_OS_MEM_ATTACH_PORT(IoAdapter);
	addrHi = Port +
<<<<<<< HEAD
<<<<<<< HEAD
	    ((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
=======
		((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
>>>>>>> refs/remotes/origin/master
	addrLo = Port + ADDR;
	ioaddr = Port + DATA;

	while (length--) {
		outpp(addrHi, (word) (address >> 16));
		outppw(addrLo, (word) (address & 0x0000ffff));
		outpp(ioaddr, *data++);
		address++;
	}

	DIVA_OS_MEM_DETACH_PORT(IoAdapter, Port);
	return (0);
}

static int
diva_bri_start_adapter(PISDN_ADAPTER IoAdapter,
		       dword start_address, dword features)
{
	byte __iomem *Port;
	dword i, test;
	byte __iomem *addrHi, *addrLo, *ioaddr;
	int started = 0;
	ADAPTER *a = &IoAdapter->a;

	if (IoAdapter->Initialized) {
		DBG_ERR(
			("A: A(%d) bri_start_adapter, adapter already running",
			 IoAdapter->ANum)) return (-1);
	}
	if (!IoAdapter->port) {
		DBG_ERR(("A: A(%d) bri_start_adapter, adapter not mapped",
			 IoAdapter->ANum)) return (-1);
	}

	sprintf(IoAdapter->Name, "A(%d)", (int) IoAdapter->ANum);
	DBG_LOG(("A(%d) start BRI", IoAdapter->ANum))

<<<<<<< HEAD
<<<<<<< HEAD
	Port = DIVA_OS_MEM_ATTACH_PORT(IoAdapter);
	addrHi = Port +
	    ((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
=======
		Port = DIVA_OS_MEM_ATTACH_PORT(IoAdapter);
	addrHi = Port +
		((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		Port = DIVA_OS_MEM_ATTACH_PORT(IoAdapter);
	addrHi = Port +
		((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
>>>>>>> refs/remotes/origin/master
	addrLo = Port + ADDR;
	ioaddr = Port + DATA;

	outpp(addrHi,
	      (byte) (
		      (IoAdapter->MemoryBase + IoAdapter->MemorySize -
		       BRI_SHARED_RAM_SIZE) >> 16));
	outppw(addrLo, 0x1e);
	outppw(ioaddr, 0x00);
	DIVA_OS_MEM_DETACH_PORT(IoAdapter, Port);

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   start the protocol code
	 */
=======
	  start the protocol code
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  start the protocol code
	*/
>>>>>>> refs/remotes/origin/master
	Port = DIVA_OS_MEM_ATTACH_CTLREG(IoAdapter);
	outpp(Port, 0x08);
	DIVA_OS_MEM_DETACH_CTLREG(IoAdapter, Port);

	Port = DIVA_OS_MEM_ATTACH_PORT(IoAdapter);
	addrHi = Port +
<<<<<<< HEAD
<<<<<<< HEAD
	    ((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
	addrLo = Port + ADDR;
	ioaddr = Port + DATA;
	/*
	   wait for signature (max. 3 seconds)
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
		((IoAdapter->Properties.Bus == BUS_PCI) ? M_PCI_ADDRH : ADDRH);
	addrLo = Port + ADDR;
	ioaddr = Port + DATA;
	/*
	  wait for signature (max. 3 seconds)
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < 300; ++i) {
		diva_os_wait(10);
		outpp(addrHi,
		      (byte) (
			      (IoAdapter->MemoryBase +
			       IoAdapter->MemorySize -
			       BRI_SHARED_RAM_SIZE) >> 16));
		outppw(addrLo, 0x1e);
		test = (dword) inppw(ioaddr);
		if (test == 0x4447) {
			DBG_LOG(
				("Protocol startup time %d.%02d seconds",
				 (i / 100), (i % 100)))
<<<<<<< HEAD
<<<<<<< HEAD
			started = 1;
=======
				started = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				started = 1;
>>>>>>> refs/remotes/origin/master
			break;
		}
	}
	DIVA_OS_MEM_DETACH_PORT(IoAdapter, Port);

	if (!started) {
		DBG_FTL(("A: A(%d) %s: Adapter selftest failed 0x%04X",
			 IoAdapter->ANum, IoAdapter->Properties.Name,
			 test))
<<<<<<< HEAD
<<<<<<< HEAD
		(*(IoAdapter->trapFnc)) (IoAdapter);
=======
			(*(IoAdapter->trapFnc)) (IoAdapter);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			(*(IoAdapter->trapFnc)) (IoAdapter);
>>>>>>> refs/remotes/origin/master
		return (-1);
	}

	IoAdapter->Initialized = 1;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Check Interrupt
	 */
=======
	  Check Interrupt
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Check Interrupt
	*/
>>>>>>> refs/remotes/origin/master
	IoAdapter->IrqCount = 0;
	a->ReadyInt = 1;

	if (IoAdapter->reset) {
		Port = DIVA_OS_MEM_ATTACH_RESET(IoAdapter);
		outpp(Port, 0x41);
		DIVA_OS_MEM_DETACH_RESET(IoAdapter, Port);
	}

	a->ram_out(a, &PR_RAM->ReadyInt, 1);
	for (i = 0; ((!IoAdapter->IrqCount) && (i < 100)); i++) {
		diva_os_wait(10);
	}
	if (!IoAdapter->IrqCount) {
		DBG_ERR(
			("A: A(%d) interrupt test failed",
			 IoAdapter->ANum))
<<<<<<< HEAD
<<<<<<< HEAD
		IoAdapter->Initialized = 0;
=======
			IoAdapter->Initialized = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			IoAdapter->Initialized = 0;
>>>>>>> refs/remotes/origin/master
		IoAdapter->stop(IoAdapter);
		return (-1);
	}

	IoAdapter->Properties.Features = (word) features;
	diva_xdi_display_adapter_features(IoAdapter->ANum);
	DBG_LOG(("A(%d) BRI adapter successfully started", IoAdapter->ANum))
<<<<<<< HEAD
<<<<<<< HEAD
	    /*
	       Register with DIDD
	     */
	diva_xdi_didd_register_adapter(IoAdapter->ANum);
=======
=======
>>>>>>> refs/remotes/origin/master
		/*
		  Register with DIDD
		*/
		diva_xdi_didd_register_adapter(IoAdapter->ANum);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return (0);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void diva_bri_clear_interrupts(diva_os_xdi_adapter_t * a)
=======
static void diva_bri_clear_interrupts(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void diva_bri_clear_interrupts(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	PISDN_ADAPTER IoAdapter = &a->xdi_adapter;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   clear any pending interrupt
	 */
=======
	  clear any pending interrupt
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  clear any pending interrupt
	*/
>>>>>>> refs/remotes/origin/master
	IoAdapter->disIrq(IoAdapter);

	IoAdapter->tst_irq(&IoAdapter->a);
	IoAdapter->clr_irq(&IoAdapter->a);
	IoAdapter->tst_irq(&IoAdapter->a);

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   kill pending dpcs
	 */
=======
	  kill pending dpcs
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  kill pending dpcs
	*/
>>>>>>> refs/remotes/origin/master
	diva_os_cancel_soft_isr(&IoAdapter->req_soft_isr);
	diva_os_cancel_soft_isr(&IoAdapter->isr_soft_isr);
}

/*
**  Stop card
*/
<<<<<<< HEAD
<<<<<<< HEAD
static int diva_bri_stop_adapter(diva_os_xdi_adapter_t * a)
=======
static int diva_bri_stop_adapter(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int diva_bri_stop_adapter(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	PISDN_ADAPTER IoAdapter = &a->xdi_adapter;
	int i = 100;

	if (!IoAdapter->port) {
		return (-1);
	}
	if (!IoAdapter->Initialized) {
		DBG_ERR(("A: A(%d) can't stop BRI adapter - not running",
			 IoAdapter->ANum))
<<<<<<< HEAD
<<<<<<< HEAD
		return (-1);	/* nothing to stop */
=======
			return (-1);	/* nothing to stop */
>>>>>>> refs/remotes/origin/cm-10.0
=======
			return (-1);	/* nothing to stop */
>>>>>>> refs/remotes/origin/master
	}
	IoAdapter->Initialized = 0;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Disconnect Adapter from DIDD
	 */
	diva_xdi_didd_remove_adapter(IoAdapter->ANum);

	/*
	   Stop interrupts
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
	  Disconnect Adapter from DIDD
	*/
	diva_xdi_didd_remove_adapter(IoAdapter->ANum);

	/*
	  Stop interrupts
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	a->clear_interrupts_proc = diva_bri_clear_interrupts;
	IoAdapter->a.ReadyInt = 1;
	IoAdapter->a.ram_inc(&IoAdapter->a, &PR_RAM->ReadyInt);
	do {
		diva_os_sleep(10);
	} while (i-- && a->clear_interrupts_proc);
	if (a->clear_interrupts_proc) {
		diva_bri_clear_interrupts(a);
		a->clear_interrupts_proc = NULL;
		DBG_ERR(("A: A(%d) no final interrupt from BRI adapter",
			 IoAdapter->ANum))
<<<<<<< HEAD
<<<<<<< HEAD
	}
	IoAdapter->a.ReadyInt = 0;

	/*
	   Stop and reset adapter
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
			}
	IoAdapter->a.ReadyInt = 0;

	/*
	  Stop and reset adapter
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	IoAdapter->stop(IoAdapter);

	return (0);
}
