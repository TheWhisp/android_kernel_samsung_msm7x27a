/* $Id: fsm.c,v 1.14.6.4 2001/09/23 22:24:47 kai Exp $
 *
 * Finite state machine
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 *              by Kai Germaschewski <kai.germaschewski@gmx.de>
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
 * Thanks to    Jan den Ouden
 *              Fritz Elfert
 *
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include "hisax.h"

#define FSM_TIMER_DEBUG 0

int
FsmNew(struct Fsm *fsm, struct FsmNode *fnlist, int fncount)
{
	int i;

<<<<<<< HEAD
	fsm->jumpmatrix = (FSMFNPTR *)
<<<<<<< HEAD
		kzalloc(sizeof (FSMFNPTR) * fsm->state_count * fsm->event_count, GFP_KERNEL);
	if (!fsm->jumpmatrix)
		return -ENOMEM;

	for (i = 0; i < fncount; i++) 
		if ((fnlist[i].state>=fsm->state_count) || (fnlist[i].event>=fsm->event_count)) {
			printk(KERN_ERR "FsmNew Error line %d st(%ld/%ld) ev(%ld/%ld)\n",
				i,(long)fnlist[i].state,(long)fsm->state_count,
				(long)fnlist[i].event,(long)fsm->event_count);
		} else		
			fsm->jumpmatrix[fsm->state_count * fnlist[i].event +
				fnlist[i].state] = (FSMFNPTR) fnlist[i].routine;
=======
=======
	fsm->jumpmatrix =
>>>>>>> refs/remotes/origin/master
		kzalloc(sizeof(FSMFNPTR) * fsm->state_count * fsm->event_count, GFP_KERNEL);
	if (!fsm->jumpmatrix)
		return -ENOMEM;

	for (i = 0; i < fncount; i++)
		if ((fnlist[i].state >= fsm->state_count) || (fnlist[i].event >= fsm->event_count)) {
			printk(KERN_ERR "FsmNew Error line %d st(%ld/%ld) ev(%ld/%ld)\n",
			       i, (long)fnlist[i].state, (long)fsm->state_count,
			       (long)fnlist[i].event, (long)fsm->event_count);
		} else
			fsm->jumpmatrix[fsm->state_count * fnlist[i].event +
					fnlist[i].state] = (FSMFNPTR)fnlist[i].routine;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

void
FsmFree(struct Fsm *fsm)
{
	kfree((void *) fsm->jumpmatrix);
}

int
FsmEvent(struct FsmInst *fi, int event, void *arg)
{
	FSMFNPTR r;

<<<<<<< HEAD
<<<<<<< HEAD
	if ((fi->state>=fi->fsm->state_count) || (event >= fi->fsm->event_count)) {
		printk(KERN_ERR "FsmEvent Error st(%ld/%ld) ev(%d/%ld)\n",
			(long)fi->state,(long)fi->fsm->state_count,event,(long)fi->fsm->event_count);
		return(1);
=======
=======
>>>>>>> refs/remotes/origin/master
	if ((fi->state >= fi->fsm->state_count) || (event >= fi->fsm->event_count)) {
		printk(KERN_ERR "FsmEvent Error st(%ld/%ld) ev(%d/%ld)\n",
		       (long)fi->state, (long)fi->fsm->state_count, event, (long)fi->fsm->event_count);
		return (1);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	r = fi->fsm->jumpmatrix[fi->fsm->state_count * event + fi->state];
	if (r) {
		if (fi->debug)
			fi->printdebug(fi, "State %s Event %s",
<<<<<<< HEAD
<<<<<<< HEAD
				fi->fsm->strState[fi->state],
				fi->fsm->strEvent[event]);
=======
				       fi->fsm->strState[fi->state],
				       fi->fsm->strEvent[event]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				       fi->fsm->strState[fi->state],
				       fi->fsm->strEvent[event]);
>>>>>>> refs/remotes/origin/master
		r(fi, event, arg);
		return (0);
	} else {
		if (fi->debug)
			fi->printdebug(fi, "State %s Event %s no routine",
<<<<<<< HEAD
<<<<<<< HEAD
				fi->fsm->strState[fi->state],
				fi->fsm->strEvent[event]);
=======
				       fi->fsm->strState[fi->state],
				       fi->fsm->strEvent[event]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				       fi->fsm->strState[fi->state],
				       fi->fsm->strEvent[event]);
>>>>>>> refs/remotes/origin/master
		return (!0);
	}
}

void
FsmChangeState(struct FsmInst *fi, int newstate)
{
	fi->state = newstate;
	if (fi->debug)
		fi->printdebug(fi, "ChangeState %s",
<<<<<<< HEAD
<<<<<<< HEAD
			fi->fsm->strState[newstate]);
=======
			       fi->fsm->strState[newstate]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			       fi->fsm->strState[newstate]);
>>>>>>> refs/remotes/origin/master
}

static void
FsmExpireTimer(struct FsmTimer *ft)
{
#if FSM_TIMER_DEBUG
	if (ft->fi->debug)
		ft->fi->printdebug(ft->fi, "FsmExpireTimer %lx", (long) ft);
#endif
	FsmEvent(ft->fi, ft->event, ft->arg);
}

void
FsmInitTimer(struct FsmInst *fi, struct FsmTimer *ft)
{
	ft->fi = fi;
	ft->tl.function = (void *) FsmExpireTimer;
	ft->tl.data = (long) ft;
#if FSM_TIMER_DEBUG
	if (ft->fi->debug)
		ft->fi->printdebug(ft->fi, "FsmInitTimer %lx", (long) ft);
#endif
	init_timer(&ft->tl);
}

void
FsmDelTimer(struct FsmTimer *ft, int where)
{
#if FSM_TIMER_DEBUG
	if (ft->fi->debug)
		ft->fi->printdebug(ft->fi, "FsmDelTimer %lx %d", (long) ft, where);
#endif
	del_timer(&ft->tl);
}

int
FsmAddTimer(struct FsmTimer *ft,
	    int millisec, int event, void *arg, int where)
{

#if FSM_TIMER_DEBUG
	if (ft->fi->debug)
		ft->fi->printdebug(ft->fi, "FsmAddTimer %lx %d %d",
<<<<<<< HEAD
<<<<<<< HEAD
			(long) ft, millisec, where);
=======
				   (long) ft, millisec, where);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				   (long) ft, millisec, where);
>>>>>>> refs/remotes/origin/master
#endif

	if (timer_pending(&ft->tl)) {
		printk(KERN_WARNING "FsmAddTimer: timer already active!\n");
		ft->fi->printdebug(ft->fi, "FsmAddTimer already active!");
		return -1;
	}
	init_timer(&ft->tl);
	ft->event = event;
	ft->arg = arg;
	ft->tl.expires = jiffies + (millisec * HZ) / 1000;
	add_timer(&ft->tl);
	return 0;
}

void
FsmRestartTimer(struct FsmTimer *ft,
<<<<<<< HEAD
<<<<<<< HEAD
	    int millisec, int event, void *arg, int where)
=======
		int millisec, int event, void *arg, int where)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		int millisec, int event, void *arg, int where)
>>>>>>> refs/remotes/origin/master
{

#if FSM_TIMER_DEBUG
	if (ft->fi->debug)
		ft->fi->printdebug(ft->fi, "FsmRestartTimer %lx %d %d",
<<<<<<< HEAD
<<<<<<< HEAD
			(long) ft, millisec, where);
=======
				   (long) ft, millisec, where);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				   (long) ft, millisec, where);
>>>>>>> refs/remotes/origin/master
#endif

	if (timer_pending(&ft->tl))
		del_timer(&ft->tl);
	init_timer(&ft->tl);
	ft->event = event;
	ft->arg = arg;
	ft->tl.expires = jiffies + (millisec * HZ) / 1000;
	add_timer(&ft->tl);
}
