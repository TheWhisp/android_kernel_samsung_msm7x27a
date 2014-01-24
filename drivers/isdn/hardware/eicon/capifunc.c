/* $Id: capifunc.c,v 1.61.4.7 2005/02/11 19:40:25 armin Exp $
 *
 * ISDN interface module for Eicon active cards DIVA.
 * CAPI Interface common functions
<<<<<<< HEAD
<<<<<<< HEAD
 * 
 * Copyright 2000-2003 by Armin Schindler (mac@melware.de) 
 * Copyright 2000-2003 Cytronics & Melware (info@melware.de)
 * 
=======
=======
>>>>>>> refs/remotes/origin/master
 *
 * Copyright 2000-2003 by Armin Schindler (mac@melware.de)
 * Copyright 2000-2003 Cytronics & Melware (info@melware.de)
 *
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#include "platform.h"
#include "os_capi.h"
#include "di_defs.h"
#include "capi20.h"
#include "divacapi.h"
#include "divasync.h"
#include "capifunc.h"

#define DBG_MINIMUM  (DL_LOG + DL_FTL + DL_ERR)
#define DBG_DEFAULT  (DBG_MINIMUM + DL_XLOG + DL_REG)

DIVA_CAPI_ADAPTER *adapter = (DIVA_CAPI_ADAPTER *) NULL;
APPL *application = (APPL *) NULL;
byte max_appl = MAX_APPL;
byte max_adapter = 0;
static CAPI_MSG *mapped_msg = (CAPI_MSG *) NULL;

byte UnMapController(byte);
char DRIVERRELEASE_CAPI[32];

extern void AutomaticLaw(DIVA_CAPI_ADAPTER *);
extern void callback(ENTITY *);
extern word api_remove_start(void);
extern word CapiRelease(word);
extern word CapiRegister(word);
extern word api_put(APPL *, CAPI_MSG *);

static diva_os_spin_lock_t api_lock;

static LIST_HEAD(cards);

static dword notify_handle;
<<<<<<< HEAD
<<<<<<< HEAD
static void DIRequest(ENTITY * e);
=======
static void DIRequest(ENTITY *e);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void DIRequest(ENTITY *e);
>>>>>>> refs/remotes/origin/master
static DESCRIPTOR MAdapter;
static DESCRIPTOR DAdapter;
static byte ControllerMap[MAX_DESCRIPTORS + 1];


static void diva_register_appl(struct capi_ctr *, __u16,
			       capi_register_params *);
static void diva_release_appl(struct capi_ctr *, __u16);
static char *diva_procinfo(struct capi_ctr *);
static u16 diva_send_message(struct capi_ctr *,
			     diva_os_message_buffer_s *);
extern void diva_os_set_controller_struct(struct capi_ctr *);

extern void DIVA_DIDD_Read(DESCRIPTOR *, int);

/*
 * debug
 */
static void no_printf(unsigned char *, ...);
#include "debuglib.c"
static void xlog(char *x, ...)
{
#ifndef DIVA_NO_DEBUGLIB
	va_list ap;
	if (myDriverDebugHandle.dbgMask & DL_XLOG) {
		va_start(ap, x);
		if (myDriverDebugHandle.dbg_irq) {
			myDriverDebugHandle.dbg_irq(myDriverDebugHandle.id,
						    DLI_XLOG, x, ap);
		} else if (myDriverDebugHandle.dbg_old) {
			myDriverDebugHandle.dbg_old(myDriverDebugHandle.id,
						    x, ap);
		}
		va_end(ap);
	}
#endif
}

/*
 * info for proc
 */
static char *diva_procinfo(struct capi_ctr *ctrl)
{
	return (ctrl->serial);
}

/*
 * stop debugging
 */
static void stop_dbg(void)
{
	DbgDeregister();
	memset(&MAdapter, 0, sizeof(MAdapter));
	dprintf = no_printf;
}

/*
 * dummy debug function
 */
static void no_printf(unsigned char *x, ...)
{
}

/*
 * Controller mapping
 */
byte MapController(byte Controller)
{
	byte i;
	byte MappedController = 0;
	byte ctrl = Controller & 0x7f;	/* mask external controller bit off */

	for (i = 1; i < max_adapter + 1; i++) {
		if (ctrl == ControllerMap[i]) {
			MappedController = (byte) i;
			break;
		}
	}
	if (i > max_adapter) {
		ControllerMap[0] = ctrl;
		MappedController = 0;
	}
	return (MappedController | (Controller & 0x80));	/* put back external controller bit */
}

/*
 * Controller unmapping
 */
byte UnMapController(byte MappedController)
{
	byte Controller;
	byte ctrl = MappedController & 0x7f;	/* mask external controller bit off */

	if (ctrl <= max_adapter) {
		Controller = ControllerMap[ctrl];
	} else {
		Controller = 0;
	}

	return (Controller | (MappedController & 0x80));	/* put back external controller bit */
}

/*
 * find a new free id
 */
static int find_free_id(void)
{
	int num = 0;
	DIVA_CAPI_ADAPTER *a;

	while (num < MAX_DESCRIPTORS) {
		a = &adapter[num];
		if (!a->Id)
			break;
		num++;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	return(num + 1);
=======
	return (num + 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return (num + 1);
>>>>>>> refs/remotes/origin/master
}

/*
 * find a card structure by controller number
 */
static diva_card *find_card_by_ctrl(word controller)
{
	struct list_head *tmp;
	diva_card *card;

	list_for_each(tmp, &cards) {
		card = list_entry(tmp, diva_card, list);
		if (ControllerMap[card->Id] == controller) {
			if (card->remove_in_progress)
				card = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
			return(card);
=======
			return (card);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			return (card);
>>>>>>> refs/remotes/origin/master
		}
	}
	return (diva_card *) 0;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Buffer RX/TX 
 */
void *TransmitBufferSet(APPL * appl, dword ref)
{
	appl->xbuffer_used[ref] = true;
	DBG_PRV1(("%d:xbuf_used(%d)", appl->Id, ref + 1))
	    return (void *)(long)ref;
}

void *TransmitBufferGet(APPL * appl, void *p)
=======
=======
>>>>>>> refs/remotes/origin/master
 * Buffer RX/TX
 */
void *TransmitBufferSet(APPL *appl, dword ref)
{
	appl->xbuffer_used[ref] = true;
	DBG_PRV1(("%d:xbuf_used(%d)", appl->Id, ref + 1))
		return (void *)(long)ref;
}

void *TransmitBufferGet(APPL *appl, void *p)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	if (appl->xbuffer_internal[(dword)(long)p])
		return appl->xbuffer_internal[(dword)(long)p];

	return appl->xbuffer_ptr[(dword)(long)p];
}

<<<<<<< HEAD
<<<<<<< HEAD
void TransmitBufferFree(APPL * appl, void *p)
{
	appl->xbuffer_used[(dword)(long)p] = false;
	DBG_PRV1(("%d:xbuf_free(%d)", appl->Id, ((dword)(long)p) + 1))
}

void *ReceiveBufferGet(APPL * appl, int Num)
=======
=======
>>>>>>> refs/remotes/origin/master
void TransmitBufferFree(APPL *appl, void *p)
{
	appl->xbuffer_used[(dword)(long)p] = false;
	DBG_PRV1(("%d:xbuf_free(%d)", appl->Id, ((dword)(long)p) + 1))
		}

void *ReceiveBufferGet(APPL *appl, int Num)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	return &appl->ReceiveBuffer[Num * appl->MaxDataLength];
}

/*
 * api_remove_start/complete for cleanup
 */
void api_remove_complete(void)
{
	DBG_PRV1(("api_remove_complete"))
<<<<<<< HEAD
<<<<<<< HEAD
}
=======
		}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		}
>>>>>>> refs/remotes/origin/master

/*
 * main function called by message.c
 */
<<<<<<< HEAD
<<<<<<< HEAD
void sendf(APPL * appl, word command, dword Id, word Number, byte * format, ...)
=======
void sendf(APPL *appl, word command, dword Id, word Number, byte *format, ...)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void sendf(APPL *appl, word command, dword Id, word Number, byte *format, ...)
>>>>>>> refs/remotes/origin/master
{
	word i, j;
	word length = 12, dlength = 0;
	byte *write;
	CAPI_MSG msg;
	byte *string = NULL;
	va_list ap;
	diva_os_message_buffer_s *dmb;
	diva_card *card = NULL;
	dword tmp;

	if (!appl)
		return;

	DBG_PRV1(("sendf(a=%d,cmd=%x,format=%s)",
		  appl->Id, command, (byte *) format))

<<<<<<< HEAD
<<<<<<< HEAD
	PUT_WORD(&msg.header.appl_id, appl->Id);
=======
		PUT_WORD(&msg.header.appl_id, appl->Id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		PUT_WORD(&msg.header.appl_id, appl->Id);
>>>>>>> refs/remotes/origin/master
	PUT_WORD(&msg.header.command, command);
	if ((byte) (command >> 8) == 0x82)
		Number = appl->Number++;
	PUT_WORD(&msg.header.number, Number);

	PUT_DWORD(&msg.header.controller, Id);
<<<<<<< HEAD
<<<<<<< HEAD
	write = (byte *) & msg;
=======
	write = (byte *)&msg;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	write = (byte *)&msg;
>>>>>>> refs/remotes/origin/master
	write += 12;

	va_start(ap, format);
	for (i = 0; format[i]; i++) {
		switch (format[i]) {
		case 'b':
			tmp = va_arg(ap, dword);
			*(byte *) write = (byte) (tmp & 0xff);
			write += 1;
			length += 1;
			break;
		case 'w':
			tmp = va_arg(ap, dword);
			PUT_WORD(write, (tmp & 0xffff));
			write += 2;
			length += 2;
			break;
		case 'd':
			tmp = va_arg(ap, dword);
			PUT_DWORD(write, tmp);
			write += 4;
			length += 4;
			break;
		case 's':
		case 'S':
			string = va_arg(ap, byte *);
			length += string[0] + 1;
			for (j = 0; j <= string[0]; j++)
				*write++ = string[j];
			break;
		}
	}
	va_end(ap);

	PUT_WORD(&msg.header.length, length);
	msg.header.controller = UnMapController(msg.header.controller);

	if (command == _DATA_B3_I)
		dlength = GET_WORD(
<<<<<<< HEAD
<<<<<<< HEAD
			      ((byte *) & msg.info.data_b3_ind.Data_Length));

	if (!(dmb = diva_os_alloc_message_buffer(length + dlength,
					  (void **) &write))) {
		DBG_ERR(("sendf: alloc_message_buffer failed, incoming msg dropped."))
		return;
	}

	/* copy msg header to sk_buff */
	memcpy(write, (byte *) & msg, length);
=======
=======
>>>>>>> refs/remotes/origin/master
			((byte *)&msg.info.data_b3_ind.Data_Length));

	if (!(dmb = diva_os_alloc_message_buffer(length + dlength,
						 (void **) &write))) {
		DBG_ERR(("sendf: alloc_message_buffer failed, incoming msg dropped."))
			return;
	}

	/* copy msg header to sk_buff */
	memcpy(write, (byte *)&msg, length);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* if DATA_B3_IND, copy data too */
	if (command == _DATA_B3_I) {
		dword data = GET_DWORD(&msg.info.data_b3_ind.Data);
		memcpy(write + length, (void *)(long)data, dlength);
	}

#ifndef DIVA_NO_DEBUGLIB
	if (myDriverDebugHandle.dbgMask & DL_XLOG) {
		switch (command) {
		default:
			xlog("\x00\x02", &msg, 0x81, length);
			break;
		case _DATA_B3_R | CONFIRM:
			if (myDriverDebugHandle.dbgMask & DL_BLK)
				xlog("\x00\x02", &msg, 0x81, length);
			break;
		case _DATA_B3_I:
			if (myDriverDebugHandle.dbgMask & DL_BLK) {
				xlog("\x00\x02", &msg, 0x81, length);
				for (i = 0; i < dlength; i += 256) {
<<<<<<< HEAD
<<<<<<< HEAD
				  DBG_BLK((((char *)(long)GET_DWORD(&msg.info.data_b3_ind.Data)) + i,
				  	((dlength - i) < 256) ? (dlength - i) : 256))
				  if (!(myDriverDebugHandle.dbgMask & DL_PRV0))
					  break; /* not more if not explicitly requested */
=======
=======
>>>>>>> refs/remotes/origin/master
					DBG_BLK((((char *)(long)GET_DWORD(&msg.info.data_b3_ind.Data)) + i,
						 ((dlength - i) < 256) ? (dlength - i) : 256))
						if (!(myDriverDebugHandle.dbgMask & DL_PRV0))
							break; /* not more if not explicitly requested */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				}
			}
			break;
		}
	}
#endif

	/* find the card structure for this controller */
	if (!(card = find_card_by_ctrl(write[8] & 0x7f))) {
		DBG_ERR(("sendf - controller %d not found, incoming msg dropped",
			 write[8] & 0x7f))
<<<<<<< HEAD
<<<<<<< HEAD
		diva_os_free_message_buffer(dmb);
=======
			diva_os_free_message_buffer(dmb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_os_free_message_buffer(dmb);
>>>>>>> refs/remotes/origin/master
		return;
	}
	/* send capi msg to capi layer */
	capi_ctr_handle_message(&card->capi_ctrl, appl->Id, dmb);
}

/*
 * cleanup adapter
 */
static void clean_adapter(int id, struct list_head *free_mem_q)
{
	DIVA_CAPI_ADAPTER *a;
	int i, k;

	a = &adapter[id];
	k = li_total_channels - a->li_channels;
	if (k == 0) {
		if (li_config_table) {
			list_add((struct list_head *)li_config_table, free_mem_q);
			li_config_table = NULL;
		}
	} else {
		if (a->li_base < k) {
			memmove(&li_config_table[a->li_base],
				&li_config_table[a->li_base + a->li_channels],
				(k - a->li_base) * sizeof(LI_CONFIG));
			for (i = 0; i < k; i++) {
				memmove(&li_config_table[i].flag_table[a->li_base],
					&li_config_table[i].flag_table[a->li_base + a->li_channels],
					k - a->li_base);
				memmove(&li_config_table[i].
					coef_table[a->li_base],
					&li_config_table[i].coef_table[a->li_base + a->li_channels],
					k - a->li_base);
			}
		}
	}
	li_total_channels = k;
	for (i = id; i < max_adapter; i++) {
		if (adapter[i].request)
			adapter[i].li_base -= a->li_channels;
	}
	if (a->plci)
		list_add((struct list_head *)a->plci, free_mem_q);

	memset(a, 0x00, sizeof(DIVA_CAPI_ADAPTER));
	while ((max_adapter != 0) && !adapter[max_adapter - 1].request)
		max_adapter--;
}

/*
 * remove a card, but ensures consistent state of LI tables
 * in the time adapter is removed
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void divacapi_remove_card(DESCRIPTOR * d)
=======
static void divacapi_remove_card(DESCRIPTOR *d)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void divacapi_remove_card(DESCRIPTOR *d)
>>>>>>> refs/remotes/origin/master
{
	diva_card *card = NULL;
	diva_os_spin_lock_magic_t old_irql;
	LIST_HEAD(free_mem_q);
	struct list_head *link;
	struct list_head *tmp;

	/*
	 * Set "remove in progress flag".
	 * Ensures that there is no call from sendf to CAPI in
	 * the time CAPI controller is about to be removed.
	 */
	diva_os_enter_spin_lock(&api_lock, &old_irql, "remove card");
	list_for_each(tmp, &cards) {
		card = list_entry(tmp, diva_card, list);
		if (card->d.request == d->request) {
			card->remove_in_progress = 1;
			list_del(tmp);
			break;
		}
	}
	diva_os_leave_spin_lock(&api_lock, &old_irql, "remove card");

	if (card) {
		/*
		 * Detach CAPI. Sendf cannot call to CAPI any more.
		 * After detach no call to send_message() is done too.
		 */
		detach_capi_ctr(&card->capi_ctrl);

		/*
		 * Now get API lock (to ensure stable state of LI tables)
		 * and update the adapter map/LI table.
		 */
		diva_os_enter_spin_lock(&api_lock, &old_irql, "remove card");

		clean_adapter(card->Id - 1, &free_mem_q);
		DBG_TRC(("DelAdapterMap (%d) -> (%d)",
<<<<<<< HEAD
<<<<<<< HEAD
				ControllerMap[card->Id], card->Id))
				ControllerMap[card->Id] = 0;
		DBG_TRC(("adapter remove, max_adapter=%d",
				max_adapter));
		diva_os_leave_spin_lock(&api_lock, &old_irql, "remove card");
		
		/* After releasing the lock, we can free the memory */
		diva_os_free (0, card);
=======
=======
>>>>>>> refs/remotes/origin/master
			 ControllerMap[card->Id], card->Id))
			ControllerMap[card->Id] = 0;
		DBG_TRC(("adapter remove, max_adapter=%d",
			 max_adapter));
		diva_os_leave_spin_lock(&api_lock, &old_irql, "remove card");

		/* After releasing the lock, we can free the memory */
		diva_os_free(0, card);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	/* free queued memory areas */
	list_for_each_safe(link, tmp, &free_mem_q) {
		list_del(link);
		diva_os_free(0, link);
	}
}

/*
 * remove cards
 */
static void divacapi_remove_cards(void)
{
	DESCRIPTOR d;
	struct list_head *tmp;
	diva_card *card;
	diva_os_spin_lock_magic_t old_irql;

rescan:
	diva_os_enter_spin_lock(&api_lock, &old_irql, "remove cards");
	list_for_each(tmp, &cards) {
		card = list_entry(tmp, diva_card, list);
		diva_os_leave_spin_lock(&api_lock, &old_irql, "remove cards");
		d.request = card->d.request;
		divacapi_remove_card(&d);
		goto rescan;
	}
	diva_os_leave_spin_lock(&api_lock, &old_irql, "remove cards");
}

/*
 * sync_callback
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void sync_callback(ENTITY * e)
=======
static void sync_callback(ENTITY *e)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void sync_callback(ENTITY *e)
>>>>>>> refs/remotes/origin/master
{
	diva_os_spin_lock_magic_t old_irql;

	DBG_TRC(("cb:Id=%x,Rc=%x,Ind=%x", e->Id, e->Rc, e->Ind))

<<<<<<< HEAD
<<<<<<< HEAD
	diva_os_enter_spin_lock(&api_lock, &old_irql, "sync_callback");
=======
		diva_os_enter_spin_lock(&api_lock, &old_irql, "sync_callback");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		diva_os_enter_spin_lock(&api_lock, &old_irql, "sync_callback");
>>>>>>> refs/remotes/origin/master
	callback(e);
	diva_os_leave_spin_lock(&api_lock, &old_irql, "sync_callback");
}

/*
 * add a new card
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int diva_add_card(DESCRIPTOR * d)
=======
static int diva_add_card(DESCRIPTOR *d)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int diva_add_card(DESCRIPTOR *d)
>>>>>>> refs/remotes/origin/master
{
	int k = 0, i = 0;
	diva_os_spin_lock_magic_t old_irql;
	diva_card *card = NULL;
	struct capi_ctr *ctrl = NULL;
	DIVA_CAPI_ADAPTER *a = NULL;
	IDI_SYNC_REQ sync_req;
	char serial[16];
<<<<<<< HEAD
<<<<<<< HEAD
	void* mem_to_free;
=======
	void *mem_to_free;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	void *mem_to_free;
>>>>>>> refs/remotes/origin/master
	LI_CONFIG *new_li_config_table;
	int j;

	if (!(card = (diva_card *) diva_os_malloc(0, sizeof(diva_card)))) {
		DBG_ERR(("diva_add_card: failed to allocate card struct."))
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
	memset((char *) card, 0x00, sizeof(diva_card));
	memcpy(&card->d, d, sizeof(DESCRIPTOR));
	sync_req.GetName.Req = 0;
	sync_req.GetName.Rc = IDI_SYNC_REQ_GET_NAME;
<<<<<<< HEAD
<<<<<<< HEAD
	card->d.request((ENTITY *) & sync_req);
=======
	card->d.request((ENTITY *)&sync_req);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	card->d.request((ENTITY *)&sync_req);
>>>>>>> refs/remotes/origin/master
	strlcpy(card->name, sync_req.GetName.name, sizeof(card->name));
	ctrl = &card->capi_ctrl;
	strcpy(ctrl->name, card->name);
	ctrl->register_appl = diva_register_appl;
	ctrl->release_appl = diva_release_appl;
	ctrl->send_message = diva_send_message;
	ctrl->procinfo = diva_procinfo;
	ctrl->driverdata = card;
	diva_os_set_controller_struct(ctrl);

	if (attach_capi_ctr(ctrl)) {
		DBG_ERR(("diva_add_card: failed to attach controller."))
<<<<<<< HEAD
<<<<<<< HEAD
		    diva_os_free(0, card);
		return (0);
	}
	
	diva_os_enter_spin_lock(&api_lock, &old_irql, "find id");
	card->Id = find_free_id();
	diva_os_leave_spin_lock(&api_lock, &old_irql, "find id");
	
=======
=======
>>>>>>> refs/remotes/origin/master
			diva_os_free(0, card);
		return (0);
	}

	diva_os_enter_spin_lock(&api_lock, &old_irql, "find id");
	card->Id = find_free_id();
	diva_os_leave_spin_lock(&api_lock, &old_irql, "find id");

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	strlcpy(ctrl->manu, M_COMPANY, sizeof(ctrl->manu));
	ctrl->version.majorversion = 2;
	ctrl->version.minorversion = 0;
	ctrl->version.majormanuversion = DRRELMAJOR;
	ctrl->version.minormanuversion = DRRELMINOR;
	sync_req.GetSerial.Req = 0;
	sync_req.GetSerial.Rc = IDI_SYNC_REQ_GET_SERIAL;
	sync_req.GetSerial.serial = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	card->d.request((ENTITY *) & sync_req);
=======
	card->d.request((ENTITY *)&sync_req);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	card->d.request((ENTITY *)&sync_req);
>>>>>>> refs/remotes/origin/master
	if ((i = ((sync_req.GetSerial.serial & 0xff000000) >> 24))) {
		sprintf(serial, "%ld-%d",
			sync_req.GetSerial.serial & 0x00ffffff, i + 1);
	} else {
		sprintf(serial, "%ld", sync_req.GetSerial.serial);
	}
	serial[CAPI_SERIAL_LEN - 1] = 0;
	strlcpy(ctrl->serial, serial, sizeof(ctrl->serial));

	a = &adapter[card->Id - 1];
	card->adapter = a;
	a->os_card = card;
	ControllerMap[card->Id] = (byte) (ctrl->cnr);

	DBG_TRC(("AddAdapterMap (%d) -> (%d)", ctrl->cnr, card->Id))

<<<<<<< HEAD
<<<<<<< HEAD
	    sync_req.xdi_capi_prms.Req = 0;
	sync_req.xdi_capi_prms.Rc = IDI_SYNC_REQ_XDI_GET_CAPI_PARAMS;
	sync_req.xdi_capi_prms.info.structure_length =
	    sizeof(diva_xdi_get_capi_parameters_t);
	card->d.request((ENTITY *) & sync_req);
	a->flag_dynamic_l1_down =
	    sync_req.xdi_capi_prms.info.flag_dynamic_l1_down;
	a->group_optimization_enabled =
	    sync_req.xdi_capi_prms.info.group_optimization_enabled;
=======
=======
>>>>>>> refs/remotes/origin/master
		sync_req.xdi_capi_prms.Req = 0;
	sync_req.xdi_capi_prms.Rc = IDI_SYNC_REQ_XDI_GET_CAPI_PARAMS;
	sync_req.xdi_capi_prms.info.structure_length =
		sizeof(diva_xdi_get_capi_parameters_t);
	card->d.request((ENTITY *)&sync_req);
	a->flag_dynamic_l1_down =
		sync_req.xdi_capi_prms.info.flag_dynamic_l1_down;
	a->group_optimization_enabled =
		sync_req.xdi_capi_prms.info.group_optimization_enabled;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	a->request = DIRequest;	/* card->d.request; */
	a->max_plci = card->d.channels + 30;
	a->max_listen = (card->d.channels > 2) ? 8 : 2;
	if (!
	    (a->plci =
	     (PLCI *) diva_os_malloc(0, sizeof(PLCI) * a->max_plci))) {
		DBG_ERR(("diva_add_card: failed alloc plci struct."))
<<<<<<< HEAD
<<<<<<< HEAD
		    memset(a, 0, sizeof(DIVA_CAPI_ADAPTER));
=======
			memset(a, 0, sizeof(DIVA_CAPI_ADAPTER));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			memset(a, 0, sizeof(DIVA_CAPI_ADAPTER));
>>>>>>> refs/remotes/origin/master
		return (0);
	}
	memset(a->plci, 0, sizeof(PLCI) * a->max_plci);

	for (k = 0; k < a->max_plci; k++) {
		a->Id = (byte) card->Id;
		a->plci[k].Sig.callback = sync_callback;
		a->plci[k].Sig.XNum = 1;
		a->plci[k].Sig.X = a->plci[k].XData;
		a->plci[k].Sig.user[0] = (word) (card->Id - 1);
		a->plci[k].Sig.user[1] = (word) k;
		a->plci[k].NL.callback = sync_callback;
		a->plci[k].NL.XNum = 1;
		a->plci[k].NL.X = a->plci[k].XData;
		a->plci[k].NL.user[0] = (word) ((card->Id - 1) | 0x8000);
		a->plci[k].NL.user[1] = (word) k;
		a->plci[k].adapter = a;
	}

	a->profile.Number = card->Id;
	a->profile.Channels = card->d.channels;
	if (card->d.features & DI_FAX3) {
		a->profile.Global_Options = 0x71;
		if (card->d.features & DI_CODEC)
			a->profile.Global_Options |= 0x6;
#if IMPLEMENT_DTMF
		a->profile.Global_Options |= 0x8;
#endif				/* IMPLEMENT_DTMF */
		a->profile.Global_Options |= 0x80; /* Line Interconnect */
#if IMPLEMENT_ECHO_CANCELLER
		a->profile.Global_Options |= 0x100;
#endif				/* IMPLEMENT_ECHO_CANCELLER */
		a->profile.B1_Protocols = 0xdf;
		a->profile.B2_Protocols = 0x1fdb;
		a->profile.B3_Protocols = 0xb7;
		a->manufacturer_features = MANUFACTURER_FEATURE_HARDDTMF;
	} else {
		a->profile.Global_Options = 0x71;
		if (card->d.features & DI_CODEC)
			a->profile.Global_Options |= 0x2;
		a->profile.B1_Protocols = 0x43;
		a->profile.B2_Protocols = 0x1f0f;
		a->profile.B3_Protocols = 0x07;
		a->manufacturer_features = 0;
	}

	a->li_pri = (a->profile.Channels > 2);
	a->li_channels = a->li_pri ? MIXER_CHANNELS_PRI : MIXER_CHANNELS_BRI;
	a->li_base = 0;
	for (i = 0; &adapter[i] != a; i++) {
		if (adapter[i].request)
			a->li_base = adapter[i].li_base + adapter[i].li_channels;
	}
	k = li_total_channels + a->li_channels;
	new_li_config_table =
		(LI_CONFIG *) diva_os_malloc(0, ((k * sizeof(LI_CONFIG) + 3) & ~3) + (2 * k) * ((k + 3) & ~3));
	if (new_li_config_table == NULL) {
		DBG_ERR(("diva_add_card: failed alloc li_config table."))
<<<<<<< HEAD
<<<<<<< HEAD
		memset(a, 0, sizeof(DIVA_CAPI_ADAPTER));
=======
			memset(a, 0, sizeof(DIVA_CAPI_ADAPTER));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			memset(a, 0, sizeof(DIVA_CAPI_ADAPTER));
>>>>>>> refs/remotes/origin/master
		return (0);
	}

	/* Prevent access to line interconnect table in process update */
	diva_os_enter_spin_lock(&api_lock, &old_irql, "add card");
<<<<<<< HEAD
<<<<<<< HEAD
	
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	j = 0;
	for (i = 0; i < k; i++) {
		if ((i >= a->li_base) && (i < a->li_base + a->li_channels))
			memset(&new_li_config_table[i], 0, sizeof(LI_CONFIG));
		else
			memcpy(&new_li_config_table[i], &li_config_table[j], sizeof(LI_CONFIG));
		new_li_config_table[i].flag_table =
			((byte *) new_li_config_table) + (((k * sizeof(LI_CONFIG) + 3) & ~3) + (2 * i) * ((k + 3) & ~3));
		new_li_config_table[i].coef_table =
			((byte *) new_li_config_table) + (((k * sizeof(LI_CONFIG) + 3) & ~3) + (2 * i + 1) * ((k + 3) & ~3));
		if ((i >= a->li_base) && (i < a->li_base + a->li_channels)) {
			new_li_config_table[i].adapter = a;
			memset(&new_li_config_table[i].flag_table[0], 0, k);
			memset(&new_li_config_table[i].coef_table[0], 0, k);
		} else {
			if (a->li_base != 0) {
				memcpy(&new_li_config_table[i].flag_table[0],
				       &li_config_table[j].flag_table[0],
				       a->li_base);
				memcpy(&new_li_config_table[i].coef_table[0],
				       &li_config_table[j].coef_table[0],
				       a->li_base);
			}
			memset(&new_li_config_table[i].flag_table[a->li_base], 0, a->li_channels);
			memset(&new_li_config_table[i].coef_table[a->li_base], 0, a->li_channels);
			if (a->li_base + a->li_channels < k) {
				memcpy(&new_li_config_table[i].flag_table[a->li_base +
<<<<<<< HEAD
<<<<<<< HEAD
				       a->li_channels],
				       &li_config_table[j].flag_table[a->li_base],
				       k - (a->li_base + a->li_channels));
				memcpy(&new_li_config_table[i].coef_table[a->li_base +
				       a->li_channels],
=======
=======
>>>>>>> refs/remotes/origin/master
									  a->li_channels],
				       &li_config_table[j].flag_table[a->li_base],
				       k - (a->li_base + a->li_channels));
				memcpy(&new_li_config_table[i].coef_table[a->li_base +
									  a->li_channels],
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				       &li_config_table[j].coef_table[a->li_base],
				       k - (a->li_base + a->li_channels));
			}
			j++;
		}
	}
	li_total_channels = k;

	mem_to_free = li_config_table;

	li_config_table = new_li_config_table;
	for (i = card->Id; i < max_adapter; i++) {
		if (adapter[i].request)
			adapter[i].li_base += a->li_channels;
	}

	if (a == &adapter[max_adapter])
		max_adapter++;

	list_add(&(card->list), &cards);
	AutomaticLaw(a);

	diva_os_leave_spin_lock(&api_lock, &old_irql, "add card");

	if (mem_to_free) {
<<<<<<< HEAD
<<<<<<< HEAD
		diva_os_free (0, mem_to_free);
=======
		diva_os_free(0, mem_to_free);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		diva_os_free(0, mem_to_free);
>>>>>>> refs/remotes/origin/master
	}

	i = 0;
	while (i++ < 30) {
		if (a->automatic_law > 3)
			break;
		diva_os_sleep(10);
	}

	/* profile information */
	PUT_WORD(&ctrl->profile.nbchannel, card->d.channels);
	ctrl->profile.goptions = a->profile.Global_Options;
	ctrl->profile.support1 = a->profile.B1_Protocols;
	ctrl->profile.support2 = a->profile.B2_Protocols;
	ctrl->profile.support3 = a->profile.B3_Protocols;
	/* manufacturer profile information */
	ctrl->profile.manu[0] = a->man_profile.private_options;
	ctrl->profile.manu[1] = a->man_profile.rtp_primary_payloads;
	ctrl->profile.manu[2] = a->man_profile.rtp_additional_payloads;
	ctrl->profile.manu[3] = 0;
	ctrl->profile.manu[4] = 0;

	capi_ctr_ready(ctrl);

	DBG_TRC(("adapter added, max_adapter=%d", max_adapter));
	return (1);
}

/*
 *  register appl
 */
static void diva_register_appl(struct capi_ctr *ctrl, __u16 appl,
<<<<<<< HEAD
<<<<<<< HEAD
			       capi_register_params * rp)
=======
			       capi_register_params *rp)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			       capi_register_params *rp)
>>>>>>> refs/remotes/origin/master
{
	APPL *this;
	word bnum, xnum;
	int i = 0;
	unsigned char *p;
	void *DataNCCI, *DataFlags, *ReceiveBuffer, *xbuffer_used;
	void **xbuffer_ptr, **xbuffer_internal;
	diva_os_spin_lock_magic_t old_irql;
	unsigned int mem_len;
	int nconn = rp->level3cnt;


	if (diva_os_in_irq()) {
		DBG_ERR(("CAPI_REGISTER - in irq context !"))
<<<<<<< HEAD
<<<<<<< HEAD
		return;
=======
			return;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			return;
>>>>>>> refs/remotes/origin/master
	}

	DBG_TRC(("application register Id=%d", appl))

<<<<<<< HEAD
<<<<<<< HEAD
	if (appl > MAX_APPL) {
		DBG_ERR(("CAPI_REGISTER - appl.Id exceeds MAX_APPL"))
		return;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
		if (appl > MAX_APPL) {
			DBG_ERR(("CAPI_REGISTER - appl.Id exceeds MAX_APPL"))
				return;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (nconn <= 0)
		nconn = ctrl->profile.nbchannel * -nconn;

<<<<<<< HEAD
<<<<<<< HEAD
        if (nconn == 0)
		nconn = ctrl->profile.nbchannel;

	DBG_LOG(("CAPI_REGISTER - Id = %d", appl))
	DBG_LOG(("  MaxLogicalConnections = %d(%d)", nconn, rp->level3cnt))
	DBG_LOG(("  MaxBDataBuffers       = %d", rp->datablkcnt))
	DBG_LOG(("  MaxBDataLength        = %d", rp->datablklen))

	if (nconn < 1 ||
	    nconn > 255 ||
	    rp->datablklen < 80 ||
	    rp->datablklen > 2150 || rp->datablkcnt > 255) {
		DBG_ERR(("CAPI_REGISTER - invalid parameters"))
		return;
	}

	if (application[appl - 1].Id == appl) {
		DBG_LOG(("CAPI_REGISTER - appl already registered"))
		return;	/* appl already registered */
=======
=======
>>>>>>> refs/remotes/origin/master
	if (nconn == 0)
		nconn = ctrl->profile.nbchannel;

	DBG_LOG(("CAPI_REGISTER - Id = %d", appl))
		DBG_LOG(("  MaxLogicalConnections = %d(%d)", nconn, rp->level3cnt))
		DBG_LOG(("  MaxBDataBuffers       = %d", rp->datablkcnt))
		DBG_LOG(("  MaxBDataLength        = %d", rp->datablklen))

		if (nconn < 1 ||
		    nconn > 255 ||
		    rp->datablklen < 80 ||
		    rp->datablklen > 2150 || rp->datablkcnt > 255) {
			DBG_ERR(("CAPI_REGISTER - invalid parameters"))
				return;
		}

	if (application[appl - 1].Id == appl) {
		DBG_LOG(("CAPI_REGISTER - appl already registered"))
			return;	/* appl already registered */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	/* alloc memory */

	bnum = nconn * rp->datablkcnt;
	xnum = nconn * MAX_DATA_B3;

	mem_len  = bnum * sizeof(word);		/* DataNCCI */
	mem_len += bnum * sizeof(word);		/* DataFlags */
	mem_len += bnum * rp->datablklen;	/* ReceiveBuffer */
	mem_len += xnum;			/* xbuffer_used */
	mem_len += xnum * sizeof(void *);	/* xbuffer_ptr */
	mem_len += xnum * sizeof(void *);	/* xbuffer_internal */
	mem_len += xnum * rp->datablklen;	/* xbuffer_ptr[xnum] */

	DBG_LOG(("  Allocated Memory      = %d", mem_len))
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(p = diva_os_malloc(0, mem_len))) {
		DBG_ERR(("CAPI_REGISTER - memory allocation failed"))
		return;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
		if (!(p = diva_os_malloc(0, mem_len))) {
			DBG_ERR(("CAPI_REGISTER - memory allocation failed"))
				return;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	memset(p, 0, mem_len);

	DataNCCI = (void *)p;
	p += bnum * sizeof(word);
	DataFlags = (void *)p;
	p += bnum * sizeof(word);
	ReceiveBuffer = (void *)p;
	p += bnum * rp->datablklen;
	xbuffer_used = (void *)p;
	p += xnum;
	xbuffer_ptr = (void **)p;
	p += xnum * sizeof(void *);
	xbuffer_internal = (void **)p;
	p += xnum * sizeof(void *);
	for (i = 0; i < xnum; i++) {
		xbuffer_ptr[i] = (void *)p;
		p += rp->datablklen;
	}

	/* initialize application data */
	diva_os_enter_spin_lock(&api_lock, &old_irql, "register_appl");

	this = &application[appl - 1];
	memset(this, 0, sizeof(APPL));

	this->Id = appl;

	for (i = 0; i < max_adapter; i++) {
		adapter[i].CIP_Mask[appl - 1] = 0;
	}

	this->queue_size = 1000;

	this->MaxNCCI = (byte) nconn;
	this->MaxNCCIData = (byte) rp->datablkcnt;
	this->MaxBuffer = bnum;
	this->MaxDataLength = rp->datablklen;

	this->DataNCCI = DataNCCI;
	this->DataFlags = DataFlags;
	this->ReceiveBuffer = ReceiveBuffer;
	this->xbuffer_used = xbuffer_used;
	this->xbuffer_ptr = xbuffer_ptr;
	this->xbuffer_internal = xbuffer_internal;
	for (i = 0; i < xnum; i++) {
		this->xbuffer_ptr[i] = xbuffer_ptr[i];
	}

	CapiRegister(this->Id);
	diva_os_leave_spin_lock(&api_lock, &old_irql, "register_appl");

}

/*
 *  release appl
 */
static void diva_release_appl(struct capi_ctr *ctrl, __u16 appl)
{
	diva_os_spin_lock_magic_t old_irql;
	APPL *this = &application[appl - 1];
	void *mem_to_free = NULL;

	DBG_TRC(("application %d(%d) cleanup", this->Id, appl))

<<<<<<< HEAD
<<<<<<< HEAD
	if (diva_os_in_irq()) {
		DBG_ERR(("CAPI_RELEASE - in irq context !"))
		return;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
		if (diva_os_in_irq()) {
			DBG_ERR(("CAPI_RELEASE - in irq context !"))
				return;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	diva_os_enter_spin_lock(&api_lock, &old_irql, "release_appl");
	if (this->Id) {
		CapiRelease(this->Id);
		mem_to_free = this->DataNCCI;
		this->DataNCCI = NULL;
		this->Id = 0;
	}
	diva_os_leave_spin_lock(&api_lock, &old_irql, "release_appl");

	if (mem_to_free)
		diva_os_free(0, mem_to_free);

}

/*
 *  send message
 */
static u16 diva_send_message(struct capi_ctr *ctrl,
<<<<<<< HEAD
<<<<<<< HEAD
			     diva_os_message_buffer_s * dmb)
=======
			     diva_os_message_buffer_s *dmb)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			     diva_os_message_buffer_s *dmb)
>>>>>>> refs/remotes/origin/master
{
	int i = 0;
	word ret = 0;
	diva_os_spin_lock_magic_t old_irql;
	CAPI_MSG *msg = (CAPI_MSG *) DIVA_MESSAGE_BUFFER_DATA(dmb);
	APPL *this = &application[GET_WORD(&msg->header.appl_id) - 1];
	diva_card *card = ctrl->driverdata;
	__u32 length = DIVA_MESSAGE_BUFFER_LEN(dmb);
	word clength = GET_WORD(&msg->header.length);
	word command = GET_WORD(&msg->header.command);
	u16 retval = CAPI_NOERROR;

	if (diva_os_in_irq()) {
		DBG_ERR(("CAPI_SEND_MSG - in irq context !"))
<<<<<<< HEAD
<<<<<<< HEAD
		return CAPI_REGOSRESOURCEERR;
	}
	DBG_PRV1(("Write - appl = %d, cmd = 0x%x", this->Id, command))

	if (card->remove_in_progress) {
		DBG_ERR(("CAPI_SEND_MSG - remove in progress!"))
		return CAPI_REGOSRESOURCEERR;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
			return CAPI_REGOSRESOURCEERR;
	}
	DBG_PRV1(("Write - appl = %d, cmd = 0x%x", this->Id, command))

		if (card->remove_in_progress) {
			DBG_ERR(("CAPI_SEND_MSG - remove in progress!"))
				return CAPI_REGOSRESOURCEERR;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	diva_os_enter_spin_lock(&api_lock, &old_irql, "send message");

	if (!this->Id) {
		diva_os_leave_spin_lock(&api_lock, &old_irql, "send message");
		return CAPI_ILLAPPNR;
	}

	/* patch controller number */
	msg->header.controller = ControllerMap[card->Id]
<<<<<<< HEAD
<<<<<<< HEAD
	    | (msg->header.controller & 0x80);	/* preserve external controller bit */
=======
		| (msg->header.controller & 0x80);	/* preserve external controller bit */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		| (msg->header.controller & 0x80);	/* preserve external controller bit */
>>>>>>> refs/remotes/origin/master

	switch (command) {
	default:
		xlog("\x00\x02", msg, 0x80, clength);
		break;

	case _DATA_B3_I | RESPONSE:
#ifndef DIVA_NO_DEBUGLIB
		if (myDriverDebugHandle.dbgMask & DL_BLK)
			xlog("\x00\x02", msg, 0x80, clength);
#endif
		break;

	case _DATA_B3_R:
#ifndef DIVA_NO_DEBUGLIB
		if (myDriverDebugHandle.dbgMask & DL_BLK)
			xlog("\x00\x02", msg, 0x80, clength);
#endif

		if (clength == 24)
			clength = 22;	/* workaround for PPcom bug */
		/* header is always 22      */
		if (GET_WORD(&msg->info.data_b3_req.Data_Length) >
		    this->MaxDataLength
		    || GET_WORD(&msg->info.data_b3_req.Data_Length) >
		    (length - clength)) {
			DBG_ERR(("Write - invalid message size"))
<<<<<<< HEAD
<<<<<<< HEAD
			retval = CAPI_ILLCMDORSUBCMDORMSGTOSMALL;
=======
				retval = CAPI_ILLCMDORSUBCMDORMSGTOSMALL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				retval = CAPI_ILLCMDORSUBCMDORMSGTOSMALL;
>>>>>>> refs/remotes/origin/master
			goto write_end;
		}

		for (i = 0; i < (MAX_DATA_B3 * this->MaxNCCI)
<<<<<<< HEAD
<<<<<<< HEAD
		     && this->xbuffer_used[i]; i++);
		if (i == (MAX_DATA_B3 * this->MaxNCCI)) {
			DBG_ERR(("Write - too many data pending"))
			retval = CAPI_SENDQUEUEFULL;
=======
=======
>>>>>>> refs/remotes/origin/master
			     && this->xbuffer_used[i]; i++);
		if (i == (MAX_DATA_B3 * this->MaxNCCI)) {
			DBG_ERR(("Write - too many data pending"))
				retval = CAPI_SENDQUEUEFULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			goto write_end;
		}
		msg->info.data_b3_req.Data = i;

		this->xbuffer_internal[i] = NULL;
		memcpy(this->xbuffer_ptr[i], &((__u8 *) msg)[clength],
		       GET_WORD(&msg->info.data_b3_req.Data_Length));

#ifndef DIVA_NO_DEBUGLIB
		if ((myDriverDebugHandle.dbgMask & DL_BLK)
		    && (myDriverDebugHandle.dbgMask & DL_XLOG)) {
			int j;
			for (j = 0; j <
<<<<<<< HEAD
<<<<<<< HEAD
			     GET_WORD(&msg->info.data_b3_req.Data_Length);
			     j += 256) {
				DBG_BLK((((char *) this->xbuffer_ptr[i]) + j,
					((GET_WORD(&msg->info.data_b3_req.Data_Length) - j) <
					  256) ? (GET_WORD(&msg->info.data_b3_req.Data_Length) - j) : 256))
				if (!(myDriverDebugHandle.dbgMask & DL_PRV0))
					break;	/* not more if not explicitly requested */
=======
=======
>>>>>>> refs/remotes/origin/master
				     GET_WORD(&msg->info.data_b3_req.Data_Length);
			     j += 256) {
				DBG_BLK((((char *) this->xbuffer_ptr[i]) + j,
					 ((GET_WORD(&msg->info.data_b3_req.Data_Length) - j) <
					  256) ? (GET_WORD(&msg->info.data_b3_req.Data_Length) - j) : 256))
					if (!(myDriverDebugHandle.dbgMask & DL_PRV0))
						break;	/* not more if not explicitly requested */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			}
		}
#endif
		break;
	}

	memcpy(mapped_msg, msg, (__u32) clength);
	mapped_msg->header.controller = MapController(mapped_msg->header.controller);
	mapped_msg->header.length = clength;
	mapped_msg->header.command = command;
	mapped_msg->header.number = GET_WORD(&msg->header.number);

	ret = api_put(this, mapped_msg);
	switch (ret) {
	case 0:
		break;
	case _BAD_MSG:
		DBG_ERR(("Write - bad message"))
<<<<<<< HEAD
<<<<<<< HEAD
		retval = CAPI_ILLCMDORSUBCMDORMSGTOSMALL;
		break;
	case _QUEUE_FULL:
		DBG_ERR(("Write - queue full"))
		retval = CAPI_SENDQUEUEFULL;
		break;
	default:
		DBG_ERR(("Write - api_put returned unknown error"))
		retval = CAPI_UNKNOWNNOTPAR;
		break;
	}

      write_end:
=======
=======
>>>>>>> refs/remotes/origin/master
			retval = CAPI_ILLCMDORSUBCMDORMSGTOSMALL;
		break;
	case _QUEUE_FULL:
		DBG_ERR(("Write - queue full"))
			retval = CAPI_SENDQUEUEFULL;
		break;
	default:
		DBG_ERR(("Write - api_put returned unknown error"))
			retval = CAPI_UNKNOWNNOTPAR;
		break;
	}

write_end:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	diva_os_leave_spin_lock(&api_lock, &old_irql, "send message");
	if (retval == CAPI_NOERROR)
		diva_os_free_message_buffer(dmb);
	return retval;
}


/*
 * cards request function
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void DIRequest(ENTITY * e)
=======
static void DIRequest(ENTITY *e)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void DIRequest(ENTITY *e)
>>>>>>> refs/remotes/origin/master
{
	DIVA_CAPI_ADAPTER *a = &(adapter[(byte) e->user[0]]);
	diva_card *os_card = (diva_card *) a->os_card;

	if (e->Req && (a->FlowControlIdTable[e->ReqCh] == e->Id)) {
		a->FlowControlSkipTable[e->ReqCh] = 1;
	}

	(*(os_card->d.request)) (e);
}

/*
 * callback function from didd
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void didd_callback(void *context, DESCRIPTOR * adapter, int removal)
=======
static void didd_callback(void *context, DESCRIPTOR *adapter, int removal)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void didd_callback(void *context, DESCRIPTOR *adapter, int removal)
>>>>>>> refs/remotes/origin/master
{
	if (adapter->type == IDI_DADAPTER) {
		DBG_ERR(("Notification about IDI_DADAPTER change ! Oops."));
		return;
	} else if (adapter->type == IDI_DIMAINT) {
		if (removal) {
			stop_dbg();
		} else {
			memcpy(&MAdapter, adapter, sizeof(MAdapter));
			dprintf = (DIVA_DI_PRINTF) MAdapter.request;
			DbgRegister("CAPI20", DRIVERRELEASE_CAPI, DBG_DEFAULT);
		}
	} else if ((adapter->type > 0) && (adapter->type < 16)) {	/* IDI Adapter */
		if (removal) {
			divacapi_remove_card(adapter);
		} else {
			diva_add_card(adapter);
		}
	}
	return;
}

/*
 * connect to didd
 */
static int divacapi_connect_didd(void)
{
	int x = 0;
	int dadapter = 0;
	IDI_SYNC_REQ req;
	DESCRIPTOR DIDD_Table[MAX_DESCRIPTORS];

	DIVA_DIDD_Read(DIDD_Table, sizeof(DIDD_Table));

	for (x = 0; x < MAX_DESCRIPTORS; x++) {
		if (DIDD_Table[x].type == IDI_DIMAINT) {	/* MAINT found */
			memcpy(&MAdapter, &DIDD_Table[x], sizeof(DAdapter));
			dprintf = (DIVA_DI_PRINTF) MAdapter.request;
			DbgRegister("CAPI20", DRIVERRELEASE_CAPI, DBG_DEFAULT);
			break;
		}
	}
	for (x = 0; x < MAX_DESCRIPTORS; x++) {
		if (DIDD_Table[x].type == IDI_DADAPTER) {	/* DADAPTER found */
			dadapter = 1;
			memcpy(&DAdapter, &DIDD_Table[x], sizeof(DAdapter));
			req.didd_notify.e.Req = 0;
			req.didd_notify.e.Rc =
<<<<<<< HEAD
<<<<<<< HEAD
			    IDI_SYNC_REQ_DIDD_REGISTER_ADAPTER_NOTIFY;
			req.didd_notify.info.callback = (void *)didd_callback;
			req.didd_notify.info.context = NULL;
			DAdapter.request((ENTITY *) & req);
=======
=======
>>>>>>> refs/remotes/origin/master
				IDI_SYNC_REQ_DIDD_REGISTER_ADAPTER_NOTIFY;
			req.didd_notify.info.callback = (void *)didd_callback;
			req.didd_notify.info.context = NULL;
			DAdapter.request((ENTITY *)&req);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			if (req.didd_notify.e.Rc != 0xff) {
				stop_dbg();
				return (0);
			}
			notify_handle = req.didd_notify.info.handle;
		}
<<<<<<< HEAD
<<<<<<< HEAD
			else if ((DIDD_Table[x].type > 0) && (DIDD_Table[x].type < 16)) {	/* IDI Adapter found */
=======
		else if ((DIDD_Table[x].type > 0) && (DIDD_Table[x].type < 16)) {	/* IDI Adapter found */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		else if ((DIDD_Table[x].type > 0) && (DIDD_Table[x].type < 16)) {	/* IDI Adapter found */
>>>>>>> refs/remotes/origin/master
			diva_add_card(&DIDD_Table[x]);
		}
	}

	if (!dadapter) {
		stop_dbg();
	}

	return (dadapter);
}

/*
 * diconnect from didd
 */
static void divacapi_disconnect_didd(void)
{
	IDI_SYNC_REQ req;

	stop_dbg();

	req.didd_notify.e.Req = 0;
	req.didd_notify.e.Rc = IDI_SYNC_REQ_DIDD_REMOVE_ADAPTER_NOTIFY;
	req.didd_notify.info.handle = notify_handle;
<<<<<<< HEAD
<<<<<<< HEAD
	DAdapter.request((ENTITY *) & req);
=======
	DAdapter.request((ENTITY *)&req);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DAdapter.request((ENTITY *)&req);
>>>>>>> refs/remotes/origin/master
}

/*
 * we do not provide date/time here,
<<<<<<< HEAD
<<<<<<< HEAD
 * the application should do this. 
=======
 * the application should do this.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * the application should do this.
>>>>>>> refs/remotes/origin/master
 */
int fax_head_line_time(char *buffer)
{
	return (0);
}

/*
 * init (alloc) main structures
 */
<<<<<<< HEAD
static int DIVA_INIT_FUNCTION init_main_structs(void)
{
	if (!(mapped_msg = (CAPI_MSG *) diva_os_malloc(0, MAX_MSG_SIZE))) {
		DBG_ERR(("init: failed alloc mapped_msg."))
<<<<<<< HEAD
		    return 0;
=======
			return 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int __init init_main_structs(void)
{
	if (!(mapped_msg = (CAPI_MSG *) diva_os_malloc(0, MAX_MSG_SIZE))) {
		DBG_ERR(("init: failed alloc mapped_msg."))
			return 0;
>>>>>>> refs/remotes/origin/master
	}

	if (!(adapter = diva_os_malloc(0, sizeof(DIVA_CAPI_ADAPTER) * MAX_DESCRIPTORS))) {
		DBG_ERR(("init: failed alloc adapter struct."))
<<<<<<< HEAD
<<<<<<< HEAD
		diva_os_free(0, mapped_msg);
=======
			diva_os_free(0, mapped_msg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_os_free(0, mapped_msg);
>>>>>>> refs/remotes/origin/master
		return 0;
	}
	memset(adapter, 0, sizeof(DIVA_CAPI_ADAPTER) * MAX_DESCRIPTORS);

	if (!(application = diva_os_malloc(0, sizeof(APPL) * MAX_APPL))) {
		DBG_ERR(("init: failed alloc application struct."))
<<<<<<< HEAD
<<<<<<< HEAD
		diva_os_free(0, mapped_msg);
=======
			diva_os_free(0, mapped_msg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_os_free(0, mapped_msg);
>>>>>>> refs/remotes/origin/master
		diva_os_free(0, adapter);
		return 0;
	}
	memset(application, 0, sizeof(APPL) * MAX_APPL);

	return (1);
}

/*
 * remove (free) main structures
 */
static void remove_main_structs(void)
{
	if (application)
		diva_os_free(0, application);
	if (adapter)
		diva_os_free(0, adapter);
	if (mapped_msg)
		diva_os_free(0, mapped_msg);
}

/*
 * api_remove_start
 */
static void do_api_remove_start(void)
{
	diva_os_spin_lock_magic_t old_irql;
	int ret = 1, count = 100;

	do {
		diva_os_enter_spin_lock(&api_lock, &old_irql, "api remove start");
		ret = api_remove_start();
		diva_os_leave_spin_lock(&api_lock, &old_irql, "api remove start");

		diva_os_sleep(10);
	} while (ret && count--);

	if (ret)
		DBG_ERR(("could not remove signaling ID's"))
<<<<<<< HEAD
<<<<<<< HEAD
}
=======
			}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			}
>>>>>>> refs/remotes/origin/master

/*
 * init
 */
<<<<<<< HEAD
int DIVA_INIT_FUNCTION init_capifunc(void)
=======
int __init init_capifunc(void)
>>>>>>> refs/remotes/origin/master
{
	diva_os_initialize_spin_lock(&api_lock, "capifunc");
	memset(ControllerMap, 0, MAX_DESCRIPTORS + 1);
	max_adapter = 0;


	if (!init_main_structs()) {
		DBG_ERR(("init: failed to init main structs."))
<<<<<<< HEAD
<<<<<<< HEAD
		diva_os_destroy_spin_lock(&api_lock, "capifunc");
=======
			diva_os_destroy_spin_lock(&api_lock, "capifunc");
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_os_destroy_spin_lock(&api_lock, "capifunc");
>>>>>>> refs/remotes/origin/master
		return (0);
	}

	if (!divacapi_connect_didd()) {
		DBG_ERR(("init: failed to connect to DIDD."))
<<<<<<< HEAD
<<<<<<< HEAD
		do_api_remove_start();
=======
			do_api_remove_start();
>>>>>>> refs/remotes/origin/cm-10.0
=======
			do_api_remove_start();
>>>>>>> refs/remotes/origin/master
		divacapi_remove_cards();
		remove_main_structs();
		diva_os_destroy_spin_lock(&api_lock, "capifunc");
		return (0);
	}

	return (1);
}

/*
 * finit
 */
<<<<<<< HEAD
void DIVA_EXIT_FUNCTION finit_capifunc(void)
=======
void __exit finit_capifunc(void)
>>>>>>> refs/remotes/origin/master
{
	do_api_remove_start();
	divacapi_disconnect_didd();
	divacapi_remove_cards();
	remove_main_structs();
	diva_os_destroy_spin_lock(&api_lock, "capifunc");
}
