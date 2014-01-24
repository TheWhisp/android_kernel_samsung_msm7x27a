/* $Id: diddfunc.c,v 1.14.6.2 2004/08/28 20:03:53 armin Exp $
 *
 * DIDD Interface module for Eicon active cards.
<<<<<<< HEAD
<<<<<<< HEAD
 * 
 * Functions are in dadapter.c 
 * 
 * Copyright 2002-2003 by Armin Schindler (mac@melware.de) 
 * Copyright 2002-2003 Cytronics & Melware (info@melware.de)
 * 
=======
=======
>>>>>>> refs/remotes/origin/master
 *
 * Functions are in dadapter.c
 *
 * Copyright 2002-2003 by Armin Schindler (mac@melware.de)
 * Copyright 2002-2003 Cytronics & Melware (info@melware.de)
 *
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 */

#include "platform.h"
#include "di_defs.h"
#include "dadapter.h"
#include "divasync.h"

#define DBG_MINIMUM  (DL_LOG + DL_FTL + DL_ERR)
#define DBG_DEFAULT  (DBG_MINIMUM + DL_XLOG + DL_REG)


extern void DIVA_DIDD_Read(void *, int);
extern char *DRIVERRELEASE_DIDD;
static dword notify_handle;
static DESCRIPTOR _DAdapter;

/*
 * didd callback function
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void *didd_callback(void *context, DESCRIPTOR * adapter,
=======
static void *didd_callback(void *context, DESCRIPTOR *adapter,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void *didd_callback(void *context, DESCRIPTOR *adapter,
>>>>>>> refs/remotes/origin/master
			   int removal)
{
	if (adapter->type == IDI_DADAPTER) {
		DBG_ERR(("Notification about IDI_DADAPTER change ! Oops."))
<<<<<<< HEAD
<<<<<<< HEAD
		return (NULL);
=======
			return (NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			return (NULL);
>>>>>>> refs/remotes/origin/master
	} else if (adapter->type == IDI_DIMAINT) {
		if (removal) {
			DbgDeregister();
		} else {
			DbgRegister("DIDD", DRIVERRELEASE_DIDD, DBG_DEFAULT);
		}
	}
	return (NULL);
}

/*
 * connect to didd
 */
<<<<<<< HEAD
static int DIVA_INIT_FUNCTION connect_didd(void)
=======
static int __init connect_didd(void)
>>>>>>> refs/remotes/origin/master
{
	int x = 0;
	int dadapter = 0;
	IDI_SYNC_REQ req;
	DESCRIPTOR DIDD_Table[MAX_DESCRIPTORS];

	DIVA_DIDD_Read(DIDD_Table, sizeof(DIDD_Table));

	for (x = 0; x < MAX_DESCRIPTORS; x++) {
		if (DIDD_Table[x].type == IDI_DADAPTER) {	/* DADAPTER found */
			dadapter = 1;
			memcpy(&_DAdapter, &DIDD_Table[x], sizeof(_DAdapter));
			req.didd_notify.e.Req = 0;
			req.didd_notify.e.Rc =
<<<<<<< HEAD
<<<<<<< HEAD
			    IDI_SYNC_REQ_DIDD_REGISTER_ADAPTER_NOTIFY;
			req.didd_notify.info.callback = (void *)didd_callback;
			req.didd_notify.info.context = NULL;
			_DAdapter.request((ENTITY *) & req);
=======
=======
>>>>>>> refs/remotes/origin/master
				IDI_SYNC_REQ_DIDD_REGISTER_ADAPTER_NOTIFY;
			req.didd_notify.info.callback = (void *)didd_callback;
			req.didd_notify.info.context = NULL;
			_DAdapter.request((ENTITY *)&req);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			if (req.didd_notify.e.Rc != 0xff)
				return (0);
			notify_handle = req.didd_notify.info.handle;
		} else if (DIDD_Table[x].type == IDI_DIMAINT) {	/* MAINT found */
			DbgRegister("DIDD", DRIVERRELEASE_DIDD, DBG_DEFAULT);
		}
	}
	return (dadapter);
}

/*
 * disconnect from didd
 */
<<<<<<< HEAD
static void DIVA_EXIT_FUNCTION disconnect_didd(void)
=======
static void __exit disconnect_didd(void)
>>>>>>> refs/remotes/origin/master
{
	IDI_SYNC_REQ req;

	req.didd_notify.e.Req = 0;
	req.didd_notify.e.Rc = IDI_SYNC_REQ_DIDD_REMOVE_ADAPTER_NOTIFY;
	req.didd_notify.info.handle = notify_handle;
<<<<<<< HEAD
<<<<<<< HEAD
	_DAdapter.request((ENTITY *) & req);
=======
	_DAdapter.request((ENTITY *)&req);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	_DAdapter.request((ENTITY *)&req);
>>>>>>> refs/remotes/origin/master
}

/*
 * init
 */
<<<<<<< HEAD
int DIVA_INIT_FUNCTION diddfunc_init(void)
=======
int __init diddfunc_init(void)
>>>>>>> refs/remotes/origin/master
{
	diva_didd_load_time_init();

	if (!connect_didd()) {
		DBG_ERR(("init: failed to connect to DIDD."))
<<<<<<< HEAD
<<<<<<< HEAD
		diva_didd_load_time_finit();
=======
			diva_didd_load_time_finit();
>>>>>>> refs/remotes/origin/cm-10.0
=======
			diva_didd_load_time_finit();
>>>>>>> refs/remotes/origin/master
		return (0);
	}
	return (1);
}

/*
 * finit
 */
<<<<<<< HEAD
void DIVA_EXIT_FUNCTION diddfunc_finit(void)
=======
void __exit diddfunc_finit(void)
>>>>>>> refs/remotes/origin/master
{
	DbgDeregister();
	disconnect_didd();
	diva_didd_load_time_finit();
}
