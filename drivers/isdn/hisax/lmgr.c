/* $Id: lmgr.c,v 1.7.6.2 2001/09/23 22:24:50 kai Exp $
 *
 * Layermanagement module
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
<<<<<<< HEAD
<<<<<<< HEAD
 * 
=======
 *
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *
>>>>>>> refs/remotes/origin/master
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#include "hisax.h"

static void
error_handling_dchan(struct PStack *st, int Error)
{
	switch (Error) {
<<<<<<< HEAD
<<<<<<< HEAD
		case 'C':
		case 'D':
		case 'G':
		case 'H':
			st->l2.l2tei(st, MDL_ERROR | REQUEST, NULL);
			break;
=======
=======
>>>>>>> refs/remotes/origin/master
	case 'C':
	case 'D':
	case 'G':
	case 'H':
		st->l2.l2tei(st, MDL_ERROR | REQUEST, NULL);
		break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
}

static void
hisax_manager(struct PStack *st, int pr, void *arg)
{
	long Code;

	switch (pr) {
<<<<<<< HEAD
<<<<<<< HEAD
		case (MDL_ERROR | INDICATION):
			Code = (long) arg;
			HiSax_putstatus(st->l1.hardware, "manager: MDL_ERROR",
				" %c %s", (char)Code, 
				test_bit(FLG_LAPD, &st->l2.flag) ?
				"D-channel" : "B-channel");
			if (test_bit(FLG_LAPD, &st->l2.flag))
				error_handling_dchan(st, Code);
			break;
=======
=======
>>>>>>> refs/remotes/origin/master
	case (MDL_ERROR | INDICATION):
		Code = (long) arg;
		HiSax_putstatus(st->l1.hardware, "manager: MDL_ERROR",
				" %c %s", (char)Code,
				test_bit(FLG_LAPD, &st->l2.flag) ?
				"D-channel" : "B-channel");
		if (test_bit(FLG_LAPD, &st->l2.flag))
			error_handling_dchan(st, Code);
		break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
}

void
setstack_manager(struct PStack *st)
{
	st->ma.layer = hisax_manager;
}
