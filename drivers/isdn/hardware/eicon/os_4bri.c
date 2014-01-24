/* $Id: os_4bri.c,v 1.28.4.4 2005/02/11 19:40:25 armin Exp $ */

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
#include "os_4bri.h"
#include "diva_pci.h"
#include "mi_pc.h"
#include "dsrv4bri.h"
#include "helpers.h"

static void *diva_xdiLoadFileFile = NULL;
static dword diva_xdiLoadFileLength = 0;

/*
**  IMPORTS
*/
extern void prepare_qBri_functions(PISDN_ADAPTER IoAdapter);
extern void prepare_qBri2_functions(PISDN_ADAPTER IoAdapter);
extern void diva_xdi_display_adapter_features(int card);
<<<<<<< HEAD
<<<<<<< HEAD
extern void diva_add_slave_adapter(diva_os_xdi_adapter_t * a);
=======
extern void diva_add_slave_adapter(diva_os_xdi_adapter_t *a);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void diva_add_slave_adapter(diva_os_xdi_adapter_t *a);
>>>>>>> refs/remotes/origin/master

extern int qBri_FPGA_download(PISDN_ADAPTER IoAdapter);
extern void start_qBri_hardware(PISDN_ADAPTER IoAdapter);

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
static unsigned long _4bri_bar_length[4] = {
	0x100,
	0x100,			/* I/O */
	MQ_MEMORY_SIZE,
	0x2000
};
static unsigned long _4bri_v2_bar_length[4] = {
	0x100,
	0x100,			/* I/O */
	MQ2_MEMORY_SIZE,
	0x10000
};
static unsigned long _4bri_v2_bri_bar_length[4] = {
	0x100,
	0x100,			/* I/O */
	BRI2_MEMORY_SIZE,
	0x10000
};


<<<<<<< HEAD
<<<<<<< HEAD
static int diva_4bri_cleanup_adapter(diva_os_xdi_adapter_t * a);
static int _4bri_get_serial_number(diva_os_xdi_adapter_t * a);
static int diva_4bri_cmd_card_proc(struct _diva_os_xdi_adapter *a,
				   diva_xdi_um_cfg_cmd_t * cmd,
				   int length);
static int diva_4bri_cleanup_slave_adapters(diva_os_xdi_adapter_t * a);
static int diva_4bri_write_fpga_image(diva_os_xdi_adapter_t * a,
				      byte * data, dword length);
static int diva_4bri_reset_adapter(PISDN_ADAPTER IoAdapter);
static int diva_4bri_write_sdram_block(PISDN_ADAPTER IoAdapter,
				       dword address,
				       const byte * data,
=======
=======
>>>>>>> refs/remotes/origin/master
static int diva_4bri_cleanup_adapter(diva_os_xdi_adapter_t *a);
static int _4bri_get_serial_number(diva_os_xdi_adapter_t *a);
static int diva_4bri_cmd_card_proc(struct _diva_os_xdi_adapter *a,
				   diva_xdi_um_cfg_cmd_t *cmd,
				   int length);
static int diva_4bri_cleanup_slave_adapters(diva_os_xdi_adapter_t *a);
static int diva_4bri_write_fpga_image(diva_os_xdi_adapter_t *a,
				      byte *data, dword length);
static int diva_4bri_reset_adapter(PISDN_ADAPTER IoAdapter);
static int diva_4bri_write_sdram_block(PISDN_ADAPTER IoAdapter,
				       dword address,
				       const byte *data,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				       dword length, dword limit);
static int diva_4bri_start_adapter(PISDN_ADAPTER IoAdapter,
				   dword start_address, dword features);
static int check_qBri_interrupt(PISDN_ADAPTER IoAdapter);
<<<<<<< HEAD
<<<<<<< HEAD
static int diva_4bri_stop_adapter(diva_os_xdi_adapter_t * a);
=======
static int diva_4bri_stop_adapter(diva_os_xdi_adapter_t *a);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int diva_4bri_stop_adapter(diva_os_xdi_adapter_t *a);
>>>>>>> refs/remotes/origin/master

static int _4bri_is_rev_2_card(int card_ordinal)
{
	switch (card_ordinal) {
	case CARDTYPE_DIVASRV_Q_8M_V2_PCI:
	case CARDTYPE_DIVASRV_VOICE_Q_8M_V2_PCI:
	case CARDTYPE_DIVASRV_B_2M_V2_PCI:
	case CARDTYPE_DIVASRV_B_2F_PCI:
	case CARDTYPE_DIVASRV_VOICE_B_2M_V2_PCI:
		return (1);
	}
	return (0);
}

static int _4bri_is_rev_2_bri_card(int card_ordinal)
{
	switch (card_ordinal) {
	case CARDTYPE_DIVASRV_B_2M_V2_PCI:
	case CARDTYPE_DIVASRV_B_2F_PCI:
	case CARDTYPE_DIVASRV_VOICE_B_2M_V2_PCI:
		return (1);
	}
	return (0);
}

static void diva_4bri_set_addresses(diva_os_xdi_adapter_t *a)
{
	dword offset = a->resources.pci.qoffset;
	dword c_offset = offset * a->xdi_adapter.ControllerNumber;

	a->resources.pci.mem_type_id[MEM_TYPE_RAM] = 2;
	a->resources.pci.mem_type_id[MEM_TYPE_ADDRESS] = 2;
	a->resources.pci.mem_type_id[MEM_TYPE_CONTROL] = 2;
	a->resources.pci.mem_type_id[MEM_TYPE_RESET] = 0;
	a->resources.pci.mem_type_id[MEM_TYPE_CTLREG] = 3;
	a->resources.pci.mem_type_id[MEM_TYPE_PROM] = 0;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Set up hardware related pointers
	 */
=======
	  Set up hardware related pointers
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Set up hardware related pointers
	*/
>>>>>>> refs/remotes/origin/master
	a->xdi_adapter.Address = a->resources.pci.addr[2];	/* BAR2 SDRAM  */
	a->xdi_adapter.Address += c_offset;

	a->xdi_adapter.Control = a->resources.pci.addr[2];	/* BAR2 SDRAM  */

	a->xdi_adapter.ram = a->resources.pci.addr[2];	/* BAR2 SDRAM  */
	a->xdi_adapter.ram += c_offset + (offset - MQ_SHARED_RAM_SIZE);
<<<<<<< HEAD
<<<<<<< HEAD
	
	a->xdi_adapter.reset = a->resources.pci.addr[0];	/* BAR0 CONFIG */
	/*
	   ctlReg contains the register address for the MIPS CPU reset control
	 */
	a->xdi_adapter.ctlReg = a->resources.pci.addr[3];	/* BAR3 CNTRL  */
	/*
	   prom contains the register address for FPGA and EEPROM programming
	 */
=======
=======
>>>>>>> refs/remotes/origin/master

	a->xdi_adapter.reset = a->resources.pci.addr[0];	/* BAR0 CONFIG */
	/*
	  ctlReg contains the register address for the MIPS CPU reset control
	*/
	a->xdi_adapter.ctlReg = a->resources.pci.addr[3];	/* BAR3 CNTRL  */
	/*
	  prom contains the register address for FPGA and EEPROM programming
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	a->xdi_adapter.prom = &a->xdi_adapter.reset[0x6E];
}

/*
**  BAR0 - MEM - 0x100    - CONFIG MEM
**  BAR1 - I/O - 0x100    - UNUSED
**  BAR2 - MEM - MQ_MEMORY_SIZE (MQ2_MEMORY_SIZE on Rev.2) - SDRAM
**  BAR3 - MEM - 0x2000 (0x10000 on Rev.2)   - CNTRL
**
**  Called by master adapter, that will initialize and add slave adapters
*/
<<<<<<< HEAD
<<<<<<< HEAD
int diva_4bri_init_card(diva_os_xdi_adapter_t * a)
=======
int diva_4bri_init_card(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int diva_4bri_init_card(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	int bar, i;
	byte __iomem *p;
	PADAPTER_LIST_ENTRY quadro_list;
	diva_os_xdi_adapter_t *diva_current;
	diva_os_xdi_adapter_t *adapter_list[4];
	PISDN_ADAPTER Slave;
	unsigned long bar_length[ARRAY_SIZE(_4bri_bar_length)];
	int v2 = _4bri_is_rev_2_card(a->CardOrdinal);
	int tasks = _4bri_is_rev_2_bri_card(a->CardOrdinal) ? 1 : MQ_INSTANCE_COUNT;
	int factor = (tasks == 1) ? 1 : 2;

	if (v2) {
		if (_4bri_is_rev_2_bri_card(a->CardOrdinal)) {
			memcpy(bar_length, _4bri_v2_bri_bar_length,
			       sizeof(bar_length));
		} else {
			memcpy(bar_length, _4bri_v2_bar_length,
			       sizeof(bar_length));
		}
	} else {
		memcpy(bar_length, _4bri_bar_length, sizeof(bar_length));
	}
	DBG_TRC(("SDRAM_LENGTH=%08x, tasks=%d, factor=%d",
		 bar_length[2], tasks, factor))

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	   Get Serial Number
	   The serial number of 4BRI is accessible in accordance with PCI spec
	   via command register located in configuration space, also we do not
	   have to map any BAR before we can access it
	 */
	if (!_4bri_get_serial_number(a)) {
		DBG_ERR(("A: 4BRI can't get Serial Number"))
		diva_4bri_cleanup_adapter(a);
		return (-1);
	}

	/*
	   Set properties
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
		/*
		  Get Serial Number
		  The serial number of 4BRI is accessible in accordance with PCI spec
		  via command register located in configuration space, also we do not
		  have to map any BAR before we can access it
		*/
		if (!_4bri_get_serial_number(a)) {
			DBG_ERR(("A: 4BRI can't get Serial Number"))
				diva_4bri_cleanup_adapter(a);
			return (-1);
		}

	/*
	  Set properties
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	a->xdi_adapter.Properties = CardProperties[a->CardOrdinal];
	DBG_LOG(("Load %s, SN:%ld, bus:%02x, func:%02x",
		 a->xdi_adapter.Properties.Name,
		 a->xdi_adapter.serialNo,
		 a->resources.pci.bus, a->resources.pci.func))

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	   First initialization step: get and check hardware resoures.
	   Do not map resources and do not access card at this step
	 */
	for (bar = 0; bar < 4; bar++) {
		a->resources.pci.bar[bar] =
		    divasa_get_pci_bar(a->resources.pci.bus,
				       a->resources.pci.func, bar,
				       a->resources.pci.hdev);
		if (!a->resources.pci.bar[bar]
		    || (a->resources.pci.bar[bar] == 0xFFFFFFF0)) {
			DBG_ERR(
				("A: invalid bar[%d]=%08x", bar,
				 a->resources.pci.bar[bar]))
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
		/*
		  First initialization step: get and check hardware resoures.
		  Do not map resources and do not access card at this step
		*/
		for (bar = 0; bar < 4; bar++) {
			a->resources.pci.bar[bar] =
				divasa_get_pci_bar(a->resources.pci.bus,
						   a->resources.pci.func, bar,
						   a->resources.pci.hdev);
			if (!a->resources.pci.bar[bar]
			    || (a->resources.pci.bar[bar] == 0xFFFFFFF0)) {
				DBG_ERR(
					("A: invalid bar[%d]=%08x", bar,
					 a->resources.pci.bar[bar]))
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

	a->xdi_adapter.sdram_bar = a->resources.pci.bar[2];

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Map all MEMORY BAR's
	 */
	for (bar = 0; bar < 4; bar++) {
		if (bar != 1) {	/* ignore I/O */
			a->resources.pci.addr[bar] =
			    divasa_remap_pci_bar(a, bar, a->resources.pci.bar[bar],
						 bar_length[bar]);
			if (!a->resources.pci.addr[bar]) {
				DBG_ERR(("A: 4BRI: can't map bar[%d]", bar))
				diva_4bri_cleanup_adapter(a);
=======
=======
>>>>>>> refs/remotes/origin/master
	  Map all MEMORY BAR's
	*/
	for (bar = 0; bar < 4; bar++) {
		if (bar != 1) {	/* ignore I/O */
			a->resources.pci.addr[bar] =
				divasa_remap_pci_bar(a, bar, a->resources.pci.bar[bar],
						     bar_length[bar]);
			if (!a->resources.pci.addr[bar]) {
				DBG_ERR(("A: 4BRI: can't map bar[%d]", bar))
					diva_4bri_cleanup_adapter(a);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				return (-1);
			}
		}
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Register I/O port
	 */
=======
	  Register I/O port
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Register I/O port
	*/
>>>>>>> refs/remotes/origin/master
	sprintf(&a->port_name[0], "DIVA 4BRI %ld", (long) a->xdi_adapter.serialNo);

	if (diva_os_register_io_port(a, 1, a->resources.pci.bar[1],
				     bar_length[1], &a->port_name[0], 1)) {
		DBG_ERR(("A: 4BRI: can't register bar[1]"))
<<<<<<< HEAD
<<<<<<< HEAD
		diva_4bri_cleanup_adapter(a);
=======
			diva_4bri_cleanup_adapter(a);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_4bri_cleanup_adapter(a);
>>>>>>> refs/remotes/origin/master
		return (-1);
	}

	a->resources.pci.addr[1] =
		(void *) (unsigned long) a->resources.pci.bar[1];

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Set cleanup pointer for base adapter only, so slave adapter
	   will be unable to get cleanup
	 */
	a->interface.cleanup_adapter_proc = diva_4bri_cleanup_adapter;

	/*
	   Create slave adapters
	 */
	if (tasks > 1) {
		if (!(a->slave_adapters[0] =
		     (diva_os_xdi_adapter_t *) diva_os_malloc(0, sizeof(*a))))
=======
=======
>>>>>>> refs/remotes/origin/master
	  Set cleanup pointer for base adapter only, so slave adapter
	  will be unable to get cleanup
	*/
	a->interface.cleanup_adapter_proc = diva_4bri_cleanup_adapter;

	/*
	  Create slave adapters
	*/
	if (tasks > 1) {
		if (!(a->slave_adapters[0] =
		      (diva_os_xdi_adapter_t *) diva_os_malloc(0, sizeof(*a))))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		{
			diva_4bri_cleanup_adapter(a);
			return (-1);
		}
		if (!(a->slave_adapters[1] =
<<<<<<< HEAD
<<<<<<< HEAD
		     (diva_os_xdi_adapter_t *) diva_os_malloc(0, sizeof(*a))))
=======
		      (diva_os_xdi_adapter_t *) diva_os_malloc(0, sizeof(*a))))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		      (diva_os_xdi_adapter_t *) diva_os_malloc(0, sizeof(*a))))
>>>>>>> refs/remotes/origin/master
		{
			diva_os_free(0, a->slave_adapters[0]);
			a->slave_adapters[0] = NULL;
			diva_4bri_cleanup_adapter(a);
			return (-1);
		}
		if (!(a->slave_adapters[2] =
<<<<<<< HEAD
<<<<<<< HEAD
		     (diva_os_xdi_adapter_t *) diva_os_malloc(0, sizeof(*a))))
=======
		      (diva_os_xdi_adapter_t *) diva_os_malloc(0, sizeof(*a))))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		      (diva_os_xdi_adapter_t *) diva_os_malloc(0, sizeof(*a))))
>>>>>>> refs/remotes/origin/master
		{
			diva_os_free(0, a->slave_adapters[0]);
			diva_os_free(0, a->slave_adapters[1]);
			a->slave_adapters[0] = NULL;
			a->slave_adapters[1] = NULL;
			diva_4bri_cleanup_adapter(a);
			return (-1);
		}
		memset(a->slave_adapters[0], 0x00, sizeof(*a));
		memset(a->slave_adapters[1], 0x00, sizeof(*a));
		memset(a->slave_adapters[2], 0x00, sizeof(*a));
	}

	adapter_list[0] = a;
	adapter_list[1] = a->slave_adapters[0];
	adapter_list[2] = a->slave_adapters[1];
	adapter_list[3] = a->slave_adapters[2];

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Allocate slave list
	 */
	quadro_list =
	    (PADAPTER_LIST_ENTRY) diva_os_malloc(0, sizeof(*quadro_list));
=======
=======
>>>>>>> refs/remotes/origin/master
	  Allocate slave list
	*/
	quadro_list =
		(PADAPTER_LIST_ENTRY) diva_os_malloc(0, sizeof(*quadro_list));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!(a->slave_list = quadro_list)) {
		for (i = 0; i < (tasks - 1); i++) {
			diva_os_free(0, a->slave_adapters[i]);
			a->slave_adapters[i] = NULL;
		}
		diva_4bri_cleanup_adapter(a);
		return (-1);
	}
	memset(quadro_list, 0x00, sizeof(*quadro_list));

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Set interfaces
	 */
=======
	  Set interfaces
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Set interfaces
	*/
>>>>>>> refs/remotes/origin/master
	a->xdi_adapter.QuadroList = quadro_list;
	for (i = 0; i < tasks; i++) {
		adapter_list[i]->xdi_adapter.ControllerNumber = i;
		adapter_list[i]->xdi_adapter.tasks = tasks;
		quadro_list->QuadroAdapter[i] =
<<<<<<< HEAD
<<<<<<< HEAD
		    &adapter_list[i]->xdi_adapter;
=======
			&adapter_list[i]->xdi_adapter;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			&adapter_list[i]->xdi_adapter;
>>>>>>> refs/remotes/origin/master
	}

	for (i = 0; i < tasks; i++) {
		diva_current = adapter_list[i];

		diva_current->dsp_mask = 0x00000003;

		diva_current->xdi_adapter.a.io =
<<<<<<< HEAD
<<<<<<< HEAD
		    &diva_current->xdi_adapter;
		diva_current->xdi_adapter.DIRequest = request;
		diva_current->interface.cmd_proc = diva_4bri_cmd_card_proc;
		diva_current->xdi_adapter.Properties =
		    CardProperties[a->CardOrdinal];
		diva_current->CardOrdinal = a->CardOrdinal;

		diva_current->xdi_adapter.Channels =
		    CardProperties[a->CardOrdinal].Channels;
		diva_current->xdi_adapter.e_max =
		    CardProperties[a->CardOrdinal].E_info;
		diva_current->xdi_adapter.e_tbl =
		    diva_os_malloc(0,
				   diva_current->xdi_adapter.e_max *
				   sizeof(E_INFO));
=======
=======
>>>>>>> refs/remotes/origin/master
			&diva_current->xdi_adapter;
		diva_current->xdi_adapter.DIRequest = request;
		diva_current->interface.cmd_proc = diva_4bri_cmd_card_proc;
		diva_current->xdi_adapter.Properties =
			CardProperties[a->CardOrdinal];
		diva_current->CardOrdinal = a->CardOrdinal;

		diva_current->xdi_adapter.Channels =
			CardProperties[a->CardOrdinal].Channels;
		diva_current->xdi_adapter.e_max =
			CardProperties[a->CardOrdinal].E_info;
		diva_current->xdi_adapter.e_tbl =
			diva_os_malloc(0,
				       diva_current->xdi_adapter.e_max *
				       sizeof(E_INFO));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		if (!diva_current->xdi_adapter.e_tbl) {
			diva_4bri_cleanup_slave_adapters(a);
			diva_4bri_cleanup_adapter(a);
			for (i = 1; i < (tasks - 1); i++) {
				diva_os_free(0, adapter_list[i]);
			}
			return (-1);
		}
		memset(diva_current->xdi_adapter.e_tbl, 0x00,
		       diva_current->xdi_adapter.e_max * sizeof(E_INFO));

		if (diva_os_initialize_spin_lock(&diva_current->xdi_adapter.isr_spin_lock, "isr")) {
			diva_4bri_cleanup_slave_adapters(a);
			diva_4bri_cleanup_adapter(a);
			for (i = 1; i < (tasks - 1); i++) {
				diva_os_free(0, adapter_list[i]);
			}
			return (-1);
		}
		if (diva_os_initialize_spin_lock(&diva_current->xdi_adapter.data_spin_lock, "data")) {
			diva_4bri_cleanup_slave_adapters(a);
			diva_4bri_cleanup_adapter(a);
			for (i = 1; i < (tasks - 1); i++) {
				diva_os_free(0, adapter_list[i]);
			}
			return (-1);
		}

		strcpy(diva_current->xdi_adapter.req_soft_isr. dpc_thread_name, "kdivas4brid");

<<<<<<< HEAD
<<<<<<< HEAD
		if (diva_os_initialize_soft_isr (&diva_current->xdi_adapter.req_soft_isr, DIDpcRoutine,
		     &diva_current->xdi_adapter)) {
=======
		if (diva_os_initialize_soft_isr(&diva_current->xdi_adapter.req_soft_isr, DIDpcRoutine,
						&diva_current->xdi_adapter)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (diva_os_initialize_soft_isr(&diva_current->xdi_adapter.req_soft_isr, DIDpcRoutine,
						&diva_current->xdi_adapter)) {
>>>>>>> refs/remotes/origin/master
			diva_4bri_cleanup_slave_adapters(a);
			diva_4bri_cleanup_adapter(a);
			for (i = 1; i < (tasks - 1); i++) {
				diva_os_free(0, adapter_list[i]);
			}
			return (-1);
		}

		/*
<<<<<<< HEAD
<<<<<<< HEAD
		   Do not initialize second DPC - only one thread will be created
		 */
		diva_current->xdi_adapter.isr_soft_isr.object =
		    diva_current->xdi_adapter.req_soft_isr.object;
=======
=======
>>>>>>> refs/remotes/origin/master
		  Do not initialize second DPC - only one thread will be created
		*/
		diva_current->xdi_adapter.isr_soft_isr.object =
			diva_current->xdi_adapter.req_soft_isr.object;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	if (v2) {
		prepare_qBri2_functions(&a->xdi_adapter);
	} else {
		prepare_qBri_functions(&a->xdi_adapter);
	}

	for (i = 0; i < tasks; i++) {
		diva_current = adapter_list[i];
		if (i)
			memcpy(&diva_current->resources, &a->resources, sizeof(divas_card_resources_t));
<<<<<<< HEAD
<<<<<<< HEAD
		diva_current->resources.pci.qoffset = (a->xdi_adapter.MemorySize >> factor); 
	}

	/*
	   Set up hardware related pointers
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
		diva_current->resources.pci.qoffset = (a->xdi_adapter.MemorySize >> factor);
	}

	/*
	  Set up hardware related pointers
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	a->xdi_adapter.cfg = (void *) (unsigned long) a->resources.pci.bar[0];	/* BAR0 CONFIG */
	a->xdi_adapter.port = (void *) (unsigned long) a->resources.pci.bar[1];	/* BAR1        */
	a->xdi_adapter.ctlReg = (void *) (unsigned long) a->resources.pci.bar[3];	/* BAR3 CNTRL  */

	for (i = 0; i < tasks; i++) {
		diva_current = adapter_list[i];
		diva_4bri_set_addresses(diva_current);
		Slave = a->xdi_adapter.QuadroList->QuadroAdapter[i];
		Slave->MultiMaster = &a->xdi_adapter;
		Slave->sdram_bar = a->xdi_adapter.sdram_bar;
		if (i) {
			Slave->serialNo = ((dword) (Slave->ControllerNumber << 24)) |
<<<<<<< HEAD
<<<<<<< HEAD
					a->xdi_adapter.serialNo;
=======
				a->xdi_adapter.serialNo;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				a->xdi_adapter.serialNo;
>>>>>>> refs/remotes/origin/master
			Slave->cardType = a->xdi_adapter.cardType;
		}
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   reset contains the base address for the PLX 9054 register set
	 */
=======
	  reset contains the base address for the PLX 9054 register set
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  reset contains the base address for the PLX 9054 register set
	*/
>>>>>>> refs/remotes/origin/master
	p = DIVA_OS_MEM_ATTACH_RESET(&a->xdi_adapter);
	WRITE_BYTE(&p[PLX9054_INTCSR], 0x00);	/* disable PCI interrupts */
	DIVA_OS_MEM_DETACH_RESET(&a->xdi_adapter, p);

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
	sprintf(a->xdi_adapter.irq_info.irq_name, "DIVA 4BRI %ld",
		(long) a->xdi_adapter.serialNo);

	if (diva_os_register_irq(a, a->xdi_adapter.irq_info.irq_nr,
				 a->xdi_adapter.irq_info.irq_name)) {
		diva_4bri_cleanup_slave_adapters(a);
		diva_4bri_cleanup_adapter(a);
		for (i = 1; i < (tasks - 1); i++) {
			diva_os_free(0, adapter_list[i]);
		}
		return (-1);
	}

	a->xdi_adapter.irq_info.registered = 1;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Add three slave adapters
	 */
=======
	  Add three slave adapters
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Add three slave adapters
	*/
>>>>>>> refs/remotes/origin/master
	if (tasks > 1) {
		diva_add_slave_adapter(adapter_list[1]);
		diva_add_slave_adapter(adapter_list[2]);
		diva_add_slave_adapter(adapter_list[3]);
	}

	diva_log_info("%s IRQ:%d SerNo:%d", a->xdi_adapter.Properties.Name,
		      a->resources.pci.irq, a->xdi_adapter.serialNo);

	return (0);
}

/*
**  Cleanup function will be called for master adapter only
**  this is guaranteed by design: cleanup callback is set
**  by master adapter only
*/
<<<<<<< HEAD
<<<<<<< HEAD
static int diva_4bri_cleanup_adapter(diva_os_xdi_adapter_t * a)
=======
static int diva_4bri_cleanup_adapter(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int diva_4bri_cleanup_adapter(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	int bar;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Stop adapter if running
	 */
=======
	  Stop adapter if running
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Stop adapter if running
	*/
>>>>>>> refs/remotes/origin/master
	if (a->xdi_adapter.Initialized) {
		diva_4bri_stop_adapter(a);
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Remove IRQ handler
	 */
=======
	  Remove IRQ handler
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Remove IRQ handler
	*/
>>>>>>> refs/remotes/origin/master
	if (a->xdi_adapter.irq_info.registered) {
		diva_os_remove_irq(a, a->xdi_adapter.irq_info.irq_nr);
	}
	a->xdi_adapter.irq_info.registered = 0;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Free DPC's and spin locks on all adapters
	 */
	diva_4bri_cleanup_slave_adapters(a);

	/*
	   Unmap all BARS
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
	  Free DPC's and spin locks on all adapters
	*/
	diva_4bri_cleanup_slave_adapters(a);

	/*
	  Unmap all BARS
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for (bar = 0; bar < 4; bar++) {
		if (bar != 1) {
			if (a->resources.pci.bar[bar]
			    && a->resources.pci.addr[bar]) {
				divasa_unmap_pci_bar(a->resources.pci.addr[bar]);
				a->resources.pci.bar[bar] = 0;
				a->resources.pci.addr[bar] = NULL;
			}
		}
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Unregister I/O
	 */
=======
	  Unregister I/O
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Unregister I/O
	*/
>>>>>>> refs/remotes/origin/master
	if (a->resources.pci.bar[1] && a->resources.pci.addr[1]) {
		diva_os_register_io_port(a, 0, a->resources.pci.bar[1],
					 _4bri_is_rev_2_card(a->
							     CardOrdinal) ?
					 _4bri_v2_bar_length[1] :
					 _4bri_bar_length[1],
					 &a->port_name[0], 1);
		a->resources.pci.bar[1] = 0;
		a->resources.pci.addr[1] = NULL;
	}

	if (a->slave_list) {
		diva_os_free(0, a->slave_list);
		a->slave_list = NULL;
	}

	return (0);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int _4bri_get_serial_number(diva_os_xdi_adapter_t * a)
=======
static int _4bri_get_serial_number(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int _4bri_get_serial_number(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	dword data[64];
	dword serNo;
	word addr, status, i, j;
	byte Bus, Slot;
	void *hdev;

	Bus = a->resources.pci.bus;
	Slot = a->resources.pci.func;
	hdev = a->resources.pci.hdev;

	for (i = 0; i < 64; ++i) {
		addr = i * 4;
		for (j = 0; j < 5; ++j) {
			PCIwrite(Bus, Slot, 0x4E, &addr, sizeof(addr),
				 hdev);
			diva_os_wait(1);
			PCIread(Bus, Slot, 0x4E, &status, sizeof(status),
				hdev);
			if (status & 0x8000)
				break;
		}
		if (j >= 5) {
			DBG_ERR(("EEPROM[%d] read failed (0x%x)", i * 4, addr))
<<<<<<< HEAD
<<<<<<< HEAD
			return (0);
=======
				return (0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				return (0);
>>>>>>> refs/remotes/origin/master
		}
		PCIread(Bus, Slot, 0x50, &data[i], sizeof(data[i]), hdev);
	}
	DBG_BLK(((char *) &data[0], sizeof(data)))

<<<<<<< HEAD
<<<<<<< HEAD
	serNo = data[32];
=======
		serNo = data[32];
>>>>>>> refs/remotes/origin/cm-10.0
=======
		serNo = data[32];
>>>>>>> refs/remotes/origin/master
	if (serNo == 0 || serNo == 0xffffffff)
		serNo = data[63];

	if (!serNo) {
		DBG_LOG(("W: Serial Number == 0, create one serial number"));
		serNo = a->resources.pci.bar[1] & 0xffff0000;
		serNo |= a->resources.pci.bus << 8;
		serNo |= a->resources.pci.func;
	}

	a->xdi_adapter.serialNo = serNo;

	DBG_REG(("Serial No.          : %ld", a->xdi_adapter.serialNo))

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
**  Release resources of slave adapters
*/
<<<<<<< HEAD
<<<<<<< HEAD
static int diva_4bri_cleanup_slave_adapters(diva_os_xdi_adapter_t * a)
=======
static int diva_4bri_cleanup_slave_adapters(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int diva_4bri_cleanup_slave_adapters(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	diva_os_xdi_adapter_t *adapter_list[4];
	diva_os_xdi_adapter_t *diva_current;
	int i;

	adapter_list[0] = a;
	adapter_list[1] = a->slave_adapters[0];
	adapter_list[2] = a->slave_adapters[1];
	adapter_list[3] = a->slave_adapters[2];

	for (i = 0; i < a->xdi_adapter.tasks; i++) {
		diva_current = adapter_list[i];
		if (diva_current) {
			diva_os_destroy_spin_lock(&diva_current->
						  xdi_adapter.
						  isr_spin_lock, "unload");
			diva_os_destroy_spin_lock(&diva_current->
						  xdi_adapter.
						  data_spin_lock,
						  "unload");

			diva_os_cancel_soft_isr(&diva_current->xdi_adapter.
						req_soft_isr);
			diva_os_cancel_soft_isr(&diva_current->xdi_adapter.
						isr_soft_isr);

			diva_os_remove_soft_isr(&diva_current->xdi_adapter.
						req_soft_isr);
			diva_current->xdi_adapter.isr_soft_isr.object = NULL;

			if (diva_current->xdi_adapter.e_tbl) {
				diva_os_free(0,
					     diva_current->xdi_adapter.
					     e_tbl);
			}
			diva_current->xdi_adapter.e_tbl = NULL;
			diva_current->xdi_adapter.e_max = 0;
			diva_current->xdi_adapter.e_count = 0;
		}
	}

	return (0);
}

static int
diva_4bri_cmd_card_proc(struct _diva_os_xdi_adapter *a,
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
		DBG_ERR(("A: 4bri_cmd, invalid controller=%d != %d",
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
		if (!a->xdi_adapter.ControllerNumber) {
			/*
<<<<<<< HEAD
<<<<<<< HEAD
			   Only master adapter can access hardware config
			 */
			a->xdi_mbox.data_length = sizeof(dword) * 9;
			a->xdi_mbox.data =
			    diva_os_malloc(0, a->xdi_mbox.data_length);
=======
=======
>>>>>>> refs/remotes/origin/master
			  Only master adapter can access hardware config
			*/
			a->xdi_mbox.data_length = sizeof(dword) * 9;
			a->xdi_mbox.data =
				diva_os_malloc(0, a->xdi_mbox.data_length);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
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
		}
		break;

	case DIVA_XDI_UM_CMD_GET_CARD_STATE:
		if (!a->xdi_adapter.ControllerNumber) {
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
				if (!a->xdi_adapter.ram
				    || !a->xdi_adapter.reset
				    || !a->xdi_adapter.cfg) {
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
		}
		break;

	case DIVA_XDI_UM_CMD_WRITE_FPGA:
		if (!a->xdi_adapter.ControllerNumber) {
			ret =
<<<<<<< HEAD
<<<<<<< HEAD
			    diva_4bri_write_fpga_image(a,
						       (byte *) & cmd[1],
						       cmd->command_data.
						       write_fpga.
						       image_length);
=======
=======
>>>>>>> refs/remotes/origin/master
				diva_4bri_write_fpga_image(a,
							   (byte *)&cmd[1],
							   cmd->command_data.
							   write_fpga.
							   image_length);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
		break;

	case DIVA_XDI_UM_CMD_RESET_ADAPTER:
		if (!a->xdi_adapter.ControllerNumber) {
			ret = diva_4bri_reset_adapter(&a->xdi_adapter);
		}
		break;

	case DIVA_XDI_UM_CMD_WRITE_SDRAM_BLOCK:
		if (!a->xdi_adapter.ControllerNumber) {
			ret = diva_4bri_write_sdram_block(&a->xdi_adapter,
							  cmd->
							  command_data.
							  write_sdram.
							  offset,
							  (byte *) &
							  cmd[1],
							  cmd->
							  command_data.
							  write_sdram.
							  length,
							  a->xdi_adapter.
							  MemorySize);
		}
		break;

	case DIVA_XDI_UM_CMD_START_ADAPTER:
		if (!a->xdi_adapter.ControllerNumber) {
			ret = diva_4bri_start_adapter(&a->xdi_adapter,
						      cmd->command_data.
						      start.offset,
						      cmd->command_data.
						      start.features);
		}
		break;

	case DIVA_XDI_UM_CMD_SET_PROTOCOL_FEATURES:
		if (!a->xdi_adapter.ControllerNumber) {
			a->xdi_adapter.features =
<<<<<<< HEAD
<<<<<<< HEAD
			    cmd->command_data.features.features;
			a->xdi_adapter.a.protocol_capabilities =
			    a->xdi_adapter.features;
			DBG_TRC(("Set raw protocol features (%08x)",
				 a->xdi_adapter.features))
			ret = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
				cmd->command_data.features.features;
			a->xdi_adapter.a.protocol_capabilities =
				a->xdi_adapter.features;
			DBG_TRC(("Set raw protocol features (%08x)",
				 a->xdi_adapter.features))
				ret = 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
		break;

	case DIVA_XDI_UM_CMD_STOP_ADAPTER:
		if (!a->xdi_adapter.ControllerNumber) {
			ret = diva_4bri_stop_adapter(a);
		}
		break;

	case DIVA_XDI_UM_CMD_READ_XLOG_ENTRY:
		ret = diva_card_read_xlog(a);
		break;

	case DIVA_XDI_UM_CMD_READ_SDRAM:
		if (!a->xdi_adapter.ControllerNumber
		    && a->xdi_adapter.Address) {
			if (
<<<<<<< HEAD
<<<<<<< HEAD
			    (a->xdi_mbox.data_length =
			     cmd->command_data.read_sdram.length)) {
				if (
				    (a->xdi_mbox.data_length +
				     cmd->command_data.read_sdram.offset) <
				    a->xdi_adapter.MemorySize) {
					a->xdi_mbox.data =
					    diva_os_malloc(0,
							   a->xdi_mbox.
							   data_length);
=======
=======
>>>>>>> refs/remotes/origin/master
				(a->xdi_mbox.data_length =
				 cmd->command_data.read_sdram.length)) {
				if (
					(a->xdi_mbox.data_length +
					 cmd->command_data.read_sdram.offset) <
					a->xdi_adapter.MemorySize) {
					a->xdi_mbox.data =
						diva_os_malloc(0,
							       a->xdi_mbox.
							       data_length);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
					if (a->xdi_mbox.data) {
						byte __iomem *p = DIVA_OS_MEM_ATTACH_ADDRESS(&a->xdi_adapter);
						byte __iomem *src = p;
						byte *dst = a->xdi_mbox.data;
						dword len = a->xdi_mbox.data_length;

						src += cmd->command_data.read_sdram.offset;

						while (len--) {
							*dst++ = READ_BYTE(src++);
						}
						DIVA_OS_MEM_DETACH_ADDRESS(&a->xdi_adapter, p);
						a->xdi_mbox.status = DIVA_XDI_MBOX_BUSY;
						ret = 0;
					}
				}
			}
		}
		break;

	default:
		DBG_ERR(("A: A(%d) invalid cmd=%d", a->controller,
			 cmd->command))
<<<<<<< HEAD
<<<<<<< HEAD
	}
=======
			}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			}
>>>>>>> refs/remotes/origin/master

	return (ret);
}

void *xdiLoadFile(char *FileName, dword *FileLength,
		  unsigned long lim)
{
	void *ret = diva_xdiLoadFileFile;

	if (FileLength) {
		*FileLength = diva_xdiLoadFileLength;
	}
	diva_xdiLoadFileFile = NULL;
	diva_xdiLoadFileLength = 0;

	return (ret);
}

void diva_os_set_qBri_functions(PISDN_ADAPTER IoAdapter)
{
}

void diva_os_set_qBri2_functions(PISDN_ADAPTER IoAdapter)
{
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
diva_4bri_write_fpga_image(diva_os_xdi_adapter_t * a, byte * data,
=======
diva_4bri_write_fpga_image(diva_os_xdi_adapter_t *a, byte *data,
>>>>>>> refs/remotes/origin/cm-10.0
=======
diva_4bri_write_fpga_image(diva_os_xdi_adapter_t *a, byte *data,
>>>>>>> refs/remotes/origin/master
			   dword length)
{
	int ret;

	diva_xdiLoadFileFile = data;
	diva_xdiLoadFileLength = length;

	ret = qBri_FPGA_download(&a->xdi_adapter);

	diva_xdiLoadFileFile = NULL;
	diva_xdiLoadFileLength = 0;

	return (ret ? 0 : -1);
}

static int diva_4bri_reset_adapter(PISDN_ADAPTER IoAdapter)
{
	PISDN_ADAPTER Slave;
	int i;

	if (!IoAdapter->Address || !IoAdapter->reset) {
		return (-1);
	}
	if (IoAdapter->Initialized) {
		DBG_ERR(("A: A(%d) can't reset 4BRI adapter - please stop first",
			 IoAdapter->ANum))
<<<<<<< HEAD
<<<<<<< HEAD
		return (-1);
	}

	/*
	   Forget all entities on all adapters
	 */
=======
=======
>>>>>>> refs/remotes/origin/master
			return (-1);
	}

	/*
	  Forget all entities on all adapters
	*/
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; ((i < IoAdapter->tasks) && IoAdapter->QuadroList); i++) {
		Slave = IoAdapter->QuadroList->QuadroAdapter[i];
		Slave->e_count = 0;
		if (Slave->e_tbl) {
			memset(Slave->e_tbl, 0x00,
			       Slave->e_max * sizeof(E_INFO));
		}
		Slave->head = 0;
		Slave->tail = 0;
		Slave->assign = 0;
		Slave->trapped = 0;

		memset(&Slave->a.IdTable[0], 0x00,
		       sizeof(Slave->a.IdTable));
		memset(&Slave->a.IdTypeTable[0], 0x00,
		       sizeof(Slave->a.IdTypeTable));
		memset(&Slave->a.FlowControlIdTable[0], 0x00,
		       sizeof(Slave->a.FlowControlIdTable));
		memset(&Slave->a.FlowControlSkipTable[0], 0x00,
		       sizeof(Slave->a.FlowControlSkipTable));
		memset(&Slave->a.misc_flags_table[0], 0x00,
		       sizeof(Slave->a.misc_flags_table));
		memset(&Slave->a.rx_stream[0], 0x00,
		       sizeof(Slave->a.rx_stream));
		memset(&Slave->a.tx_stream[0], 0x00,
		       sizeof(Slave->a.tx_stream));
		memset(&Slave->a.tx_pos[0], 0x00, sizeof(Slave->a.tx_pos));
		memset(&Slave->a.rx_pos[0], 0x00, sizeof(Slave->a.rx_pos));
	}

	return (0);
}


static int
diva_4bri_write_sdram_block(PISDN_ADAPTER IoAdapter,
			    dword address,
<<<<<<< HEAD
<<<<<<< HEAD
			    const byte * data, dword length, dword limit)
=======
			    const byte *data, dword length, dword limit)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			    const byte *data, dword length, dword limit)
>>>>>>> refs/remotes/origin/master
{
	byte __iomem *p = DIVA_OS_MEM_ATTACH_ADDRESS(IoAdapter);
	byte __iomem *mem = p;

	if (((address + length) >= limit) || !mem) {
		DIVA_OS_MEM_DETACH_ADDRESS(IoAdapter, p);
		DBG_ERR(("A: A(%d) write 4BRI address=0x%08lx",
			 IoAdapter->ANum, address + length))
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
	mem += address;

	while (length--) {
		WRITE_BYTE(mem++, *data++);
	}

	DIVA_OS_MEM_DETACH_ADDRESS(IoAdapter, p);
	return (0);
}

static int
diva_4bri_start_adapter(PISDN_ADAPTER IoAdapter,
			dword start_address, dword features)
{
	volatile word __iomem *signature;
	int started = 0;
	int i;
	byte __iomem *p;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   start adapter
	 */
=======
	  start adapter
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  start adapter
	*/
>>>>>>> refs/remotes/origin/master
	start_qBri_hardware(IoAdapter);

	p = DIVA_OS_MEM_ATTACH_RAM(IoAdapter);
	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   wait for signature in shared memory (max. 3 seconds)
	 */
=======
	  wait for signature in shared memory (max. 3 seconds)
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  wait for signature in shared memory (max. 3 seconds)
	*/
>>>>>>> refs/remotes/origin/master
	signature = (volatile word __iomem *) (&p[0x1E]);

	for (i = 0; i < 300; ++i) {
		diva_os_wait(10);
		if (READ_WORD(&signature[0]) == 0x4447) {
			DBG_TRC(("Protocol startup time %d.%02d seconds",
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

	for (i = 1; i < IoAdapter->tasks; i++) {
		IoAdapter->QuadroList->QuadroAdapter[i]->features =
<<<<<<< HEAD
<<<<<<< HEAD
		    IoAdapter->features;
		IoAdapter->QuadroList->QuadroAdapter[i]->a.
		    protocol_capabilities = IoAdapter->features;
=======
			IoAdapter->features;
		IoAdapter->QuadroList->QuadroAdapter[i]->a.
			protocol_capabilities = IoAdapter->features;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			IoAdapter->features;
		IoAdapter->QuadroList->QuadroAdapter[i]->a.
			protocol_capabilities = IoAdapter->features;
>>>>>>> refs/remotes/origin/master
	}

	if (!started) {
		DBG_FTL(("%s: Adapter selftest failed, signature=%04x",
			 IoAdapter->Properties.Name,
			 READ_WORD(&signature[0])))
<<<<<<< HEAD
<<<<<<< HEAD
		DIVA_OS_MEM_DETACH_RAM(IoAdapter, p);
=======
			DIVA_OS_MEM_DETACH_RAM(IoAdapter, p);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			DIVA_OS_MEM_DETACH_RAM(IoAdapter, p);
>>>>>>> refs/remotes/origin/master
		(*(IoAdapter->trapFnc)) (IoAdapter);
		IoAdapter->stop(IoAdapter);
		return (-1);
	}
	DIVA_OS_MEM_DETACH_RAM(IoAdapter, p);

	for (i = 0; i < IoAdapter->tasks; i++) {
		IoAdapter->QuadroList->QuadroAdapter[i]->Initialized = 1;
		IoAdapter->QuadroList->QuadroAdapter[i]->IrqCount = 0;
	}

	if (check_qBri_interrupt(IoAdapter)) {
		DBG_ERR(("A: A(%d) interrupt test failed",
			 IoAdapter->ANum))
<<<<<<< HEAD
<<<<<<< HEAD
		for (i = 0; i < IoAdapter->tasks; i++) {
			IoAdapter->QuadroList->QuadroAdapter[i]->Initialized = 0;
		}
=======
			for (i = 0; i < IoAdapter->tasks; i++) {
				IoAdapter->QuadroList->QuadroAdapter[i]->Initialized = 0;
			}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			for (i = 0; i < IoAdapter->tasks; i++) {
				IoAdapter->QuadroList->QuadroAdapter[i]->Initialized = 0;
			}
>>>>>>> refs/remotes/origin/master
		IoAdapter->stop(IoAdapter);
		return (-1);
	}

	IoAdapter->Properties.Features = (word) features;
	diva_xdi_display_adapter_features(IoAdapter->ANum);

	for (i = 0; i < IoAdapter->tasks; i++) {
		DBG_LOG(("A(%d) %s adapter successfully started",
			 IoAdapter->QuadroList->QuadroAdapter[i]->ANum,
			 (IoAdapter->tasks == 1) ? "BRI 2.0" : "4BRI"))
<<<<<<< HEAD
<<<<<<< HEAD
		diva_xdi_didd_register_adapter(IoAdapter->QuadroList->QuadroAdapter[i]->ANum);
=======
			diva_xdi_didd_register_adapter(IoAdapter->QuadroList->QuadroAdapter[i]->ANum);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_xdi_didd_register_adapter(IoAdapter->QuadroList->QuadroAdapter[i]->ANum);
>>>>>>> refs/remotes/origin/master
		IoAdapter->QuadroList->QuadroAdapter[i]->Properties.Features = (word) features;
	}

	return (0);
}

static int check_qBri_interrupt(PISDN_ADAPTER IoAdapter)
{
#ifdef	SUPPORT_INTERRUPT_TEST_ON_4BRI
	int i;
	ADAPTER *a = &IoAdapter->a;
	byte __iomem *p;

	IoAdapter->IrqCount = 0;

	if (IoAdapter->ControllerNumber > 0)
		return (-1);

	p = DIVA_OS_MEM_ATTACH_RESET(IoAdapter);
	WRITE_BYTE(&p[PLX9054_INTCSR], PLX9054_INT_ENABLE);
	DIVA_OS_MEM_DETACH_RESET(IoAdapter, p);
	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   interrupt test
	 */
=======
	  interrupt test
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  interrupt test
	*/
>>>>>>> refs/remotes/origin/master
	a->ReadyInt = 1;
	a->ram_out(a, &PR_RAM->ReadyInt, 1);

	for (i = 100; !IoAdapter->IrqCount && (i-- > 0); diva_os_wait(10));

	return ((IoAdapter->IrqCount > 0) ? 0 : -1);
#else
	dword volatile __iomem *qBriIrq;
	byte __iomem *p;
	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Reset on-board interrupt register
	 */
	IoAdapter->IrqCount = 0;
	p = DIVA_OS_MEM_ATTACH_CTLREG(IoAdapter);
	qBriIrq = (dword volatile __iomem *) (&p[_4bri_is_rev_2_card
				       (IoAdapter->
					cardType) ? (MQ2_BREG_IRQ_TEST)
				       : (MQ_BREG_IRQ_TEST)]);
=======
=======
>>>>>>> refs/remotes/origin/master
	  Reset on-board interrupt register
	*/
	IoAdapter->IrqCount = 0;
	p = DIVA_OS_MEM_ATTACH_CTLREG(IoAdapter);
	qBriIrq = (dword volatile __iomem *) (&p[_4bri_is_rev_2_card
						 (IoAdapter->
						  cardType) ? (MQ2_BREG_IRQ_TEST)
						 : (MQ_BREG_IRQ_TEST)]);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	WRITE_DWORD(qBriIrq, MQ_IRQ_REQ_OFF);
	DIVA_OS_MEM_DETACH_CTLREG(IoAdapter, p);

	p = DIVA_OS_MEM_ATTACH_RESET(IoAdapter);
	WRITE_BYTE(&p[PLX9054_INTCSR], PLX9054_INT_ENABLE);
	DIVA_OS_MEM_DETACH_RESET(IoAdapter, p);

	diva_os_wait(100);

	return (0);
#endif				/* SUPPORT_INTERRUPT_TEST_ON_4BRI */
}

<<<<<<< HEAD
<<<<<<< HEAD
static void diva_4bri_clear_interrupts(diva_os_xdi_adapter_t * a)
=======
static void diva_4bri_clear_interrupts(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void diva_4bri_clear_interrupts(diva_os_xdi_adapter_t *a)
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

<<<<<<< HEAD
<<<<<<< HEAD
static int diva_4bri_stop_adapter(diva_os_xdi_adapter_t * a)
=======
static int diva_4bri_stop_adapter(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int diva_4bri_stop_adapter(diva_os_xdi_adapter_t *a)
>>>>>>> refs/remotes/origin/master
{
	PISDN_ADAPTER IoAdapter = &a->xdi_adapter;
	int i;

	if (!IoAdapter->ram) {
		return (-1);
	}

	if (!IoAdapter->Initialized) {
		DBG_ERR(("A: A(%d) can't stop PRI adapter - not running",
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

	for (i = 0; i < IoAdapter->tasks; i++) {
		IoAdapter->QuadroList->QuadroAdapter[i]->Initialized = 0;
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Disconnect Adapters from DIDD
	 */
=======
	  Disconnect Adapters from DIDD
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Disconnect Adapters from DIDD
	*/
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < IoAdapter->tasks; i++) {
		diva_xdi_didd_remove_adapter(IoAdapter->QuadroList->QuadroAdapter[i]->ANum);
	}

	i = 100;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	   Stop interrupts
	 */
=======
	  Stop interrupts
	*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  Stop interrupts
	*/
>>>>>>> refs/remotes/origin/master
	a->clear_interrupts_proc = diva_4bri_clear_interrupts;
	IoAdapter->a.ReadyInt = 1;
	IoAdapter->a.ram_inc(&IoAdapter->a, &PR_RAM->ReadyInt);
	do {
		diva_os_sleep(10);
	} while (i-- && a->clear_interrupts_proc);

	if (a->clear_interrupts_proc) {
		diva_4bri_clear_interrupts(a);
		a->clear_interrupts_proc = NULL;
		DBG_ERR(("A: A(%d) no final interrupt from 4BRI adapter",
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
