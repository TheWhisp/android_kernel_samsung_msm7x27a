/* FS-Cache object state machine handler
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * See Documentation/filesystems/caching/object.txt for a description of the
 * object state machine and the in-kernel representations.
 */

#define FSCACHE_DEBUG_LEVEL COOKIE
#include <linux/module.h>
<<<<<<< HEAD
#include "internal.h"

const char *fscache_object_states[FSCACHE_OBJECT__NSTATES] = {
	[FSCACHE_OBJECT_INIT]		= "OBJECT_INIT",
	[FSCACHE_OBJECT_LOOKING_UP]	= "OBJECT_LOOKING_UP",
	[FSCACHE_OBJECT_CREATING]	= "OBJECT_CREATING",
	[FSCACHE_OBJECT_AVAILABLE]	= "OBJECT_AVAILABLE",
	[FSCACHE_OBJECT_ACTIVE]		= "OBJECT_ACTIVE",
	[FSCACHE_OBJECT_UPDATING]	= "OBJECT_UPDATING",
	[FSCACHE_OBJECT_DYING]		= "OBJECT_DYING",
	[FSCACHE_OBJECT_LC_DYING]	= "OBJECT_LC_DYING",
	[FSCACHE_OBJECT_ABORT_INIT]	= "OBJECT_ABORT_INIT",
	[FSCACHE_OBJECT_RELEASING]	= "OBJECT_RELEASING",
	[FSCACHE_OBJECT_RECYCLING]	= "OBJECT_RECYCLING",
	[FSCACHE_OBJECT_WITHDRAWING]	= "OBJECT_WITHDRAWING",
	[FSCACHE_OBJECT_DEAD]		= "OBJECT_DEAD",
};
EXPORT_SYMBOL(fscache_object_states);

const char fscache_object_states_short[FSCACHE_OBJECT__NSTATES][5] = {
	[FSCACHE_OBJECT_INIT]		= "INIT",
	[FSCACHE_OBJECT_LOOKING_UP]	= "LOOK",
	[FSCACHE_OBJECT_CREATING]	= "CRTN",
	[FSCACHE_OBJECT_AVAILABLE]	= "AVBL",
	[FSCACHE_OBJECT_ACTIVE]		= "ACTV",
	[FSCACHE_OBJECT_UPDATING]	= "UPDT",
	[FSCACHE_OBJECT_DYING]		= "DYNG",
	[FSCACHE_OBJECT_LC_DYING]	= "LCDY",
	[FSCACHE_OBJECT_ABORT_INIT]	= "ABTI",
	[FSCACHE_OBJECT_RELEASING]	= "RELS",
	[FSCACHE_OBJECT_RECYCLING]	= "RCYC",
	[FSCACHE_OBJECT_WITHDRAWING]	= "WTHD",
	[FSCACHE_OBJECT_DEAD]		= "DEAD",
=======
#include <linux/slab.h>
#include <linux/prefetch.h>
#include "internal.h"

static const struct fscache_state *fscache_abort_initialisation(struct fscache_object *, int);
static const struct fscache_state *fscache_kill_dependents(struct fscache_object *, int);
static const struct fscache_state *fscache_drop_object(struct fscache_object *, int);
static const struct fscache_state *fscache_initialise_object(struct fscache_object *, int);
static const struct fscache_state *fscache_invalidate_object(struct fscache_object *, int);
static const struct fscache_state *fscache_jumpstart_dependents(struct fscache_object *, int);
static const struct fscache_state *fscache_kill_object(struct fscache_object *, int);
static const struct fscache_state *fscache_lookup_failure(struct fscache_object *, int);
static const struct fscache_state *fscache_look_up_object(struct fscache_object *, int);
static const struct fscache_state *fscache_object_available(struct fscache_object *, int);
static const struct fscache_state *fscache_parent_ready(struct fscache_object *, int);
static const struct fscache_state *fscache_update_object(struct fscache_object *, int);

#define __STATE_NAME(n) fscache_osm_##n
#define STATE(n) (&__STATE_NAME(n))

/*
 * Define a work state.  Work states are execution states.  No event processing
 * is performed by them.  The function attached to a work state returns a
 * pointer indicating the next state to which the state machine should
 * transition.  Returning NO_TRANSIT repeats the current state, but goes back
 * to the scheduler first.
 */
#define WORK_STATE(n, sn, f) \
	const struct fscache_state __STATE_NAME(n) = {			\
		.name = #n,						\
		.short_name = sn,					\
		.work = f						\
	}

/*
 * Returns from work states.
 */
#define transit_to(state) ({ prefetch(&STATE(state)->work); STATE(state); })

#define NO_TRANSIT ((struct fscache_state *)NULL)

/*
 * Define a wait state.  Wait states are event processing states.  No execution
 * is performed by them.  Wait states are just tables of "if event X occurs,
 * clear it and transition to state Y".  The dispatcher returns to the
 * scheduler if none of the events in which the wait state has an interest are
 * currently pending.
 */
#define WAIT_STATE(n, sn, ...) \
	const struct fscache_state __STATE_NAME(n) = {			\
		.name = #n,						\
		.short_name = sn,					\
		.work = NULL,						\
		.transitions = { __VA_ARGS__, { 0, NULL } }		\
	}

#define TRANSIT_TO(state, emask) \
	{ .events = (emask), .transit_to = STATE(state) }

/*
 * The object state machine.
 */
static WORK_STATE(INIT_OBJECT,		"INIT", fscache_initialise_object);
static WORK_STATE(PARENT_READY,		"PRDY", fscache_parent_ready);
static WORK_STATE(ABORT_INIT,		"ABRT", fscache_abort_initialisation);
static WORK_STATE(LOOK_UP_OBJECT,	"LOOK", fscache_look_up_object);
static WORK_STATE(CREATE_OBJECT,	"CRTO", fscache_look_up_object);
static WORK_STATE(OBJECT_AVAILABLE,	"AVBL", fscache_object_available);
static WORK_STATE(JUMPSTART_DEPS,	"JUMP", fscache_jumpstart_dependents);

static WORK_STATE(INVALIDATE_OBJECT,	"INVL", fscache_invalidate_object);
static WORK_STATE(UPDATE_OBJECT,	"UPDT", fscache_update_object);

static WORK_STATE(LOOKUP_FAILURE,	"LCFL", fscache_lookup_failure);
static WORK_STATE(KILL_OBJECT,		"KILL", fscache_kill_object);
static WORK_STATE(KILL_DEPENDENTS,	"KDEP", fscache_kill_dependents);
static WORK_STATE(DROP_OBJECT,		"DROP", fscache_drop_object);
static WORK_STATE(OBJECT_DEAD,		"DEAD", (void*)2UL);

static WAIT_STATE(WAIT_FOR_INIT,	"?INI",
		  TRANSIT_TO(INIT_OBJECT,	1 << FSCACHE_OBJECT_EV_NEW_CHILD));

static WAIT_STATE(WAIT_FOR_PARENT,	"?PRN",
		  TRANSIT_TO(PARENT_READY,	1 << FSCACHE_OBJECT_EV_PARENT_READY));

static WAIT_STATE(WAIT_FOR_CMD,		"?CMD",
		  TRANSIT_TO(INVALIDATE_OBJECT,	1 << FSCACHE_OBJECT_EV_INVALIDATE),
		  TRANSIT_TO(UPDATE_OBJECT,	1 << FSCACHE_OBJECT_EV_UPDATE),
		  TRANSIT_TO(JUMPSTART_DEPS,	1 << FSCACHE_OBJECT_EV_NEW_CHILD));

static WAIT_STATE(WAIT_FOR_CLEARANCE,	"?CLR",
		  TRANSIT_TO(KILL_OBJECT,	1 << FSCACHE_OBJECT_EV_CLEARED));

/*
 * Out-of-band event transition tables.  These are for handling unexpected
 * events, such as an I/O error.  If an OOB event occurs, the state machine
 * clears and disables the event and forces a transition to the nominated work
 * state (acurrently executing work states will complete first).
 *
 * In such a situation, object->state remembers the state the machine should
 * have been in/gone to and returning NO_TRANSIT returns to that.
 */
static const struct fscache_transition fscache_osm_init_oob[] = {
	   TRANSIT_TO(ABORT_INIT,
		      (1 << FSCACHE_OBJECT_EV_ERROR) |
		      (1 << FSCACHE_OBJECT_EV_KILL)),
	   { 0, NULL }
};

static const struct fscache_transition fscache_osm_lookup_oob[] = {
	   TRANSIT_TO(LOOKUP_FAILURE,
		      (1 << FSCACHE_OBJECT_EV_ERROR) |
		      (1 << FSCACHE_OBJECT_EV_KILL)),
	   { 0, NULL }
};

static const struct fscache_transition fscache_osm_run_oob[] = {
	   TRANSIT_TO(KILL_OBJECT,
		      (1 << FSCACHE_OBJECT_EV_ERROR) |
		      (1 << FSCACHE_OBJECT_EV_KILL)),
	   { 0, NULL }
>>>>>>> refs/remotes/origin/master
};

static int  fscache_get_object(struct fscache_object *);
static void fscache_put_object(struct fscache_object *);
<<<<<<< HEAD
static void fscache_initialise_object(struct fscache_object *);
static void fscache_lookup_object(struct fscache_object *);
static void fscache_object_available(struct fscache_object *);
static void fscache_release_object(struct fscache_object *);
static void fscache_withdraw_object(struct fscache_object *);
static void fscache_enqueue_dependents(struct fscache_object *);
=======
static bool fscache_enqueue_dependents(struct fscache_object *, int);
>>>>>>> refs/remotes/origin/master
static void fscache_dequeue_object(struct fscache_object *);

/*
 * we need to notify the parent when an op completes that we had outstanding
 * upon it
 */
static inline void fscache_done_parent_op(struct fscache_object *object)
{
	struct fscache_object *parent = object->parent;

	_enter("OBJ%x {OBJ%x,%x}",
	       object->debug_id, parent->debug_id, parent->n_ops);

	spin_lock_nested(&parent->lock, 1);
<<<<<<< HEAD
	parent->n_ops--;
	parent->n_obj_ops--;
=======
	parent->n_obj_ops--;
	parent->n_ops--;
>>>>>>> refs/remotes/origin/master
	if (parent->n_ops == 0)
		fscache_raise_event(parent, FSCACHE_OBJECT_EV_CLEARED);
	spin_unlock(&parent->lock);
}

/*
<<<<<<< HEAD
 * process events that have been sent to an object's state machine
 * - initiates parent lookup
 * - does object lookup
 * - does object creation
 * - does object recycling and retirement
 * - does object withdrawal
 */
static void fscache_object_state_machine(struct fscache_object *object)
{
	enum fscache_object_state new_state;
	struct fscache_cookie *cookie;
=======
 * Object state machine dispatcher.
 */
static void fscache_object_sm_dispatcher(struct fscache_object *object)
{
	const struct fscache_transition *t;
	const struct fscache_state *state, *new_state;
	unsigned long events, event_mask;
	int event = -1;
>>>>>>> refs/remotes/origin/master

	ASSERT(object != NULL);

	_enter("{OBJ%x,%s,%lx}",
<<<<<<< HEAD
	       object->debug_id, fscache_object_states[object->state],
	       object->events);

	switch (object->state) {
		/* wait for the parent object to become ready */
	case FSCACHE_OBJECT_INIT:
		object->event_mask =
			ULONG_MAX & ~(1 << FSCACHE_OBJECT_EV_CLEARED);
		fscache_initialise_object(object);
		goto done;

		/* look up the object metadata on disk */
	case FSCACHE_OBJECT_LOOKING_UP:
		fscache_lookup_object(object);
		goto lookup_transit;

		/* create the object metadata on disk */
	case FSCACHE_OBJECT_CREATING:
		fscache_lookup_object(object);
		goto lookup_transit;

		/* handle an object becoming available; start pending
		 * operations and queue dependent operations for processing */
	case FSCACHE_OBJECT_AVAILABLE:
		fscache_object_available(object);
		goto active_transit;

		/* normal running state */
	case FSCACHE_OBJECT_ACTIVE:
		goto active_transit;

		/* update the object metadata on disk */
	case FSCACHE_OBJECT_UPDATING:
		clear_bit(FSCACHE_OBJECT_EV_UPDATE, &object->events);
		fscache_stat(&fscache_n_updates_run);
		fscache_stat(&fscache_n_cop_update_object);
		object->cache->ops->update_object(object);
		fscache_stat_d(&fscache_n_cop_update_object);
		goto active_transit;

		/* handle an object dying during lookup or creation */
	case FSCACHE_OBJECT_LC_DYING:
		object->event_mask &= ~(1 << FSCACHE_OBJECT_EV_UPDATE);
		fscache_stat(&fscache_n_cop_lookup_complete);
		object->cache->ops->lookup_complete(object);
		fscache_stat_d(&fscache_n_cop_lookup_complete);

		spin_lock(&object->lock);
		object->state = FSCACHE_OBJECT_DYING;
		cookie = object->cookie;
		if (cookie) {
			if (test_and_clear_bit(FSCACHE_COOKIE_LOOKING_UP,
					       &cookie->flags))
				wake_up_bit(&cookie->flags,
					    FSCACHE_COOKIE_LOOKING_UP);
			if (test_and_clear_bit(FSCACHE_COOKIE_CREATING,
					       &cookie->flags))
				wake_up_bit(&cookie->flags,
					    FSCACHE_COOKIE_CREATING);
		}
		spin_unlock(&object->lock);

		fscache_done_parent_op(object);

		/* wait for completion of all active operations on this object
		 * and the death of all child objects of this object */
	case FSCACHE_OBJECT_DYING:
	dying:
		clear_bit(FSCACHE_OBJECT_EV_CLEARED, &object->events);
		spin_lock(&object->lock);
		_debug("dying OBJ%x {%d,%d}",
		       object->debug_id, object->n_ops, object->n_children);
		if (object->n_ops == 0 && object->n_children == 0) {
			object->event_mask &=
				~(1 << FSCACHE_OBJECT_EV_CLEARED);
			object->event_mask |=
				(1 << FSCACHE_OBJECT_EV_WITHDRAW) |
				(1 << FSCACHE_OBJECT_EV_RETIRE) |
				(1 << FSCACHE_OBJECT_EV_RELEASE) |
				(1 << FSCACHE_OBJECT_EV_ERROR);
		} else {
			object->event_mask &=
				~((1 << FSCACHE_OBJECT_EV_WITHDRAW) |
				  (1 << FSCACHE_OBJECT_EV_RETIRE) |
				  (1 << FSCACHE_OBJECT_EV_RELEASE) |
				  (1 << FSCACHE_OBJECT_EV_ERROR));
			object->event_mask |=
				1 << FSCACHE_OBJECT_EV_CLEARED;
		}
		spin_unlock(&object->lock);
		fscache_enqueue_dependents(object);
		fscache_start_operations(object);
		goto terminal_transit;

		/* handle an abort during initialisation */
	case FSCACHE_OBJECT_ABORT_INIT:
		_debug("handle abort init %lx", object->events);
		object->event_mask &= ~(1 << FSCACHE_OBJECT_EV_UPDATE);

		spin_lock(&object->lock);
		fscache_dequeue_object(object);

		object->state = FSCACHE_OBJECT_DYING;
		if (test_and_clear_bit(FSCACHE_COOKIE_CREATING,
				       &object->cookie->flags))
			wake_up_bit(&object->cookie->flags,
				    FSCACHE_COOKIE_CREATING);
		spin_unlock(&object->lock);
		goto dying;

		/* handle the netfs releasing an object and possibly marking it
		 * obsolete too */
	case FSCACHE_OBJECT_RELEASING:
	case FSCACHE_OBJECT_RECYCLING:
		object->event_mask &=
			~((1 << FSCACHE_OBJECT_EV_WITHDRAW) |
			  (1 << FSCACHE_OBJECT_EV_RETIRE) |
			  (1 << FSCACHE_OBJECT_EV_RELEASE) |
			  (1 << FSCACHE_OBJECT_EV_ERROR));
		fscache_release_object(object);
		spin_lock(&object->lock);
		object->state = FSCACHE_OBJECT_DEAD;
		spin_unlock(&object->lock);
		fscache_stat(&fscache_n_object_dead);
		goto terminal_transit;

		/* handle the parent cache of this object being withdrawn from
		 * active service */
	case FSCACHE_OBJECT_WITHDRAWING:
		object->event_mask &=
			~((1 << FSCACHE_OBJECT_EV_WITHDRAW) |
			  (1 << FSCACHE_OBJECT_EV_RETIRE) |
			  (1 << FSCACHE_OBJECT_EV_RELEASE) |
			  (1 << FSCACHE_OBJECT_EV_ERROR));
		fscache_withdraw_object(object);
		spin_lock(&object->lock);
		object->state = FSCACHE_OBJECT_DEAD;
		spin_unlock(&object->lock);
		fscache_stat(&fscache_n_object_dead);
		goto terminal_transit;

		/* complain about the object being woken up once it is
		 * deceased */
	case FSCACHE_OBJECT_DEAD:
		printk(KERN_ERR "FS-Cache:"
		       " Unexpected event in dead state %lx\n",
		       object->events & object->event_mask);
		BUG();

	default:
		printk(KERN_ERR "FS-Cache: Unknown object state %u\n",
		       object->state);
		BUG();
	}

	/* determine the transition from a lookup state */
lookup_transit:
	switch (fls(object->events & object->event_mask) - 1) {
	case FSCACHE_OBJECT_EV_WITHDRAW:
	case FSCACHE_OBJECT_EV_RETIRE:
	case FSCACHE_OBJECT_EV_RELEASE:
	case FSCACHE_OBJECT_EV_ERROR:
		new_state = FSCACHE_OBJECT_LC_DYING;
		goto change_state;
	case FSCACHE_OBJECT_EV_REQUEUE:
		goto done;
	case -1:
		goto done; /* sleep until event */
	default:
		goto unsupported_event;
	}

	/* determine the transition from an active state */
active_transit:
	switch (fls(object->events & object->event_mask) - 1) {
	case FSCACHE_OBJECT_EV_WITHDRAW:
	case FSCACHE_OBJECT_EV_RETIRE:
	case FSCACHE_OBJECT_EV_RELEASE:
	case FSCACHE_OBJECT_EV_ERROR:
		new_state = FSCACHE_OBJECT_DYING;
		goto change_state;
	case FSCACHE_OBJECT_EV_UPDATE:
		new_state = FSCACHE_OBJECT_UPDATING;
		goto change_state;
	case -1:
		new_state = FSCACHE_OBJECT_ACTIVE;
		goto change_state; /* sleep until event */
	default:
		goto unsupported_event;
	}

	/* determine the transition from a terminal state */
terminal_transit:
	switch (fls(object->events & object->event_mask) - 1) {
	case FSCACHE_OBJECT_EV_WITHDRAW:
		new_state = FSCACHE_OBJECT_WITHDRAWING;
		goto change_state;
	case FSCACHE_OBJECT_EV_RETIRE:
		new_state = FSCACHE_OBJECT_RECYCLING;
		goto change_state;
	case FSCACHE_OBJECT_EV_RELEASE:
		new_state = FSCACHE_OBJECT_RELEASING;
		goto change_state;
	case FSCACHE_OBJECT_EV_ERROR:
		new_state = FSCACHE_OBJECT_WITHDRAWING;
		goto change_state;
	case FSCACHE_OBJECT_EV_CLEARED:
		new_state = FSCACHE_OBJECT_DYING;
		goto change_state;
	case -1:
		goto done; /* sleep until event */
	default:
		goto unsupported_event;
	}

change_state:
	spin_lock(&object->lock);
	object->state = new_state;
	spin_unlock(&object->lock);

done:
	_leave(" [->%s]", fscache_object_states[object->state]);
	return;

unsupported_event:
	printk(KERN_ERR "FS-Cache:"
	       " Unsupported event %lx [mask %lx] in state %s\n",
	       object->events, object->event_mask,
	       fscache_object_states[object->state]);
	BUG();
=======
	       object->debug_id, object->state->name, object->events);

	event_mask = object->event_mask;
restart:
	object->event_mask = 0; /* Mask normal event handling */
	state = object->state;
restart_masked:
	events = object->events;

	/* Handle any out-of-band events (typically an error) */
	if (events & object->oob_event_mask) {
		_debug("{OBJ%x} oob %lx",
		       object->debug_id, events & object->oob_event_mask);
		for (t = object->oob_table; t->events; t++) {
			if (events & t->events) {
				state = t->transit_to;
				ASSERT(state->work != NULL);
				event = fls(events & t->events) - 1;
				__clear_bit(event, &object->oob_event_mask);
				clear_bit(event, &object->events);
				goto execute_work_state;
			}
		}
	}

	/* Wait states are just transition tables */
	if (!state->work) {
		if (events & event_mask) {
			for (t = state->transitions; t->events; t++) {
				if (events & t->events) {
					new_state = t->transit_to;
					event = fls(events & t->events) - 1;
					clear_bit(event, &object->events);
					_debug("{OBJ%x} ev %d: %s -> %s",
					       object->debug_id, event,
					       state->name, new_state->name);
					object->state = state = new_state;
					goto execute_work_state;
				}
			}

			/* The event mask didn't include all the tabled bits */
			BUG();
		}
		/* Randomly woke up */
		goto unmask_events;
	}

execute_work_state:
	_debug("{OBJ%x} exec %s", object->debug_id, state->name);

	new_state = state->work(object, event);
	event = -1;
	if (new_state == NO_TRANSIT) {
		_debug("{OBJ%x} %s notrans", object->debug_id, state->name);
		fscache_enqueue_object(object);
		event_mask = object->oob_event_mask;
		goto unmask_events;
	}

	_debug("{OBJ%x} %s -> %s",
	       object->debug_id, state->name, new_state->name);
	object->state = state = new_state;

	if (state->work) {
		if (unlikely(state->work == ((void *)2UL))) {
			_leave(" [dead]");
			return;
		}
		goto restart_masked;
	}

	/* Transited to wait state */
	event_mask = object->oob_event_mask;
	for (t = state->transitions; t->events; t++)
		event_mask |= t->events;

unmask_events:
	object->event_mask = event_mask;
	smp_mb();
	events = object->events;
	if (events & event_mask)
		goto restart;
	_leave(" [msk %lx]", event_mask);
>>>>>>> refs/remotes/origin/master
}

/*
 * execute an object
 */
<<<<<<< HEAD
void fscache_object_work_func(struct work_struct *work)
=======
static void fscache_object_work_func(struct work_struct *work)
>>>>>>> refs/remotes/origin/master
{
	struct fscache_object *object =
		container_of(work, struct fscache_object, work);
	unsigned long start;

	_enter("{OBJ%x}", object->debug_id);

	start = jiffies;
<<<<<<< HEAD
	fscache_object_state_machine(object);
	fscache_hist(fscache_objs_histogram, start);
	if (object->events & object->event_mask)
		fscache_enqueue_object(object);
	clear_bit(FSCACHE_OBJECT_EV_REQUEUE, &object->events);
	fscache_put_object(object);
}
EXPORT_SYMBOL(fscache_object_work_func);
=======
	fscache_object_sm_dispatcher(object);
	fscache_hist(fscache_objs_histogram, start);
	fscache_put_object(object);
}

/**
 * fscache_object_init - Initialise a cache object description
 * @object: Object description
 * @cookie: Cookie object will be attached to
 * @cache: Cache in which backing object will be found
 *
 * Initialise a cache object description to its basic values.
 *
 * See Documentation/filesystems/caching/backend-api.txt for a complete
 * description.
 */
void fscache_object_init(struct fscache_object *object,
			 struct fscache_cookie *cookie,
			 struct fscache_cache *cache)
{
	const struct fscache_transition *t;

	atomic_inc(&cache->object_count);

	object->state = STATE(WAIT_FOR_INIT);
	object->oob_table = fscache_osm_init_oob;
	object->flags = 1 << FSCACHE_OBJECT_IS_LIVE;
	spin_lock_init(&object->lock);
	INIT_LIST_HEAD(&object->cache_link);
	INIT_HLIST_NODE(&object->cookie_link);
	INIT_WORK(&object->work, fscache_object_work_func);
	INIT_LIST_HEAD(&object->dependents);
	INIT_LIST_HEAD(&object->dep_link);
	INIT_LIST_HEAD(&object->pending_ops);
	object->n_children = 0;
	object->n_ops = object->n_in_progress = object->n_exclusive = 0;
	object->events = 0;
	object->store_limit = 0;
	object->store_limit_l = 0;
	object->cache = cache;
	object->cookie = cookie;
	object->parent = NULL;

	object->oob_event_mask = 0;
	for (t = object->oob_table; t->events; t++)
		object->oob_event_mask |= t->events;
	object->event_mask = object->oob_event_mask;
	for (t = object->state->transitions; t->events; t++)
		object->event_mask |= t->events;
}
EXPORT_SYMBOL(fscache_object_init);

/*
 * Abort object initialisation before we start it.
 */
static const struct fscache_state *fscache_abort_initialisation(struct fscache_object *object,
								int event)
{
	_enter("{OBJ%x},%d", object->debug_id, event);

	object->oob_event_mask = 0;
	fscache_dequeue_object(object);
	return transit_to(KILL_OBJECT);
}
>>>>>>> refs/remotes/origin/master

/*
 * initialise an object
 * - check the specified object's parent to see if we can make use of it
 *   immediately to do a creation
 * - we may need to start the process of creating a parent and we need to wait
 *   for the parent's lookup and creation to complete if it's not there yet
<<<<<<< HEAD
 * - an object's cookie is pinned until we clear FSCACHE_COOKIE_CREATING on the
 *   leaf-most cookies of the object and all its children
 */
static void fscache_initialise_object(struct fscache_object *object)
{
	struct fscache_object *parent;

	_enter("");
	ASSERT(object->cookie != NULL);
	ASSERT(object->cookie->parent != NULL);

	if (object->events & ((1 << FSCACHE_OBJECT_EV_ERROR) |
			      (1 << FSCACHE_OBJECT_EV_RELEASE) |
			      (1 << FSCACHE_OBJECT_EV_RETIRE) |
			      (1 << FSCACHE_OBJECT_EV_WITHDRAW))) {
		_debug("abort init %lx", object->events);
		spin_lock(&object->lock);
		object->state = FSCACHE_OBJECT_ABORT_INIT;
		spin_unlock(&object->lock);
		return;
	}

	spin_lock(&object->cookie->lock);
	spin_lock_nested(&object->cookie->parent->lock, 1);

	parent = object->parent;
	if (!parent) {
		_debug("no parent");
		set_bit(FSCACHE_OBJECT_EV_WITHDRAW, &object->events);
	} else {
		spin_lock(&object->lock);
		spin_lock_nested(&parent->lock, 1);
		_debug("parent %s", fscache_object_states[parent->state]);

		if (parent->state >= FSCACHE_OBJECT_DYING) {
			_debug("bad parent");
			set_bit(FSCACHE_OBJECT_EV_WITHDRAW, &object->events);
		} else if (parent->state < FSCACHE_OBJECT_AVAILABLE) {
			_debug("wait");

			/* we may get woken up in this state by child objects
			 * binding on to us, so we need to make sure we don't
			 * add ourself to the list multiple times */
			if (list_empty(&object->dep_link)) {
				fscache_stat(&fscache_n_cop_grab_object);
				object->cache->ops->grab_object(object);
				fscache_stat_d(&fscache_n_cop_grab_object);
				list_add(&object->dep_link,
					 &parent->dependents);

				/* fscache_acquire_non_index_cookie() uses this
				 * to wake the chain up */
				if (parent->state == FSCACHE_OBJECT_INIT)
					fscache_enqueue_object(parent);
			}
		} else {
			_debug("go");
			parent->n_ops++;
			parent->n_obj_ops++;
			object->lookup_jif = jiffies;
			object->state = FSCACHE_OBJECT_LOOKING_UP;
			set_bit(FSCACHE_OBJECT_EV_REQUEUE, &object->events);
		}

		spin_unlock(&parent->lock);
		spin_unlock(&object->lock);
	}

	spin_unlock(&object->cookie->parent->lock);
	spin_unlock(&object->cookie->lock);
	_leave("");
=======
 */
static const struct fscache_state *fscache_initialise_object(struct fscache_object *object,
							     int event)
{
	struct fscache_object *parent;
	bool success;

	_enter("{OBJ%x},%d", object->debug_id, event);

	ASSERT(list_empty(&object->dep_link));

	parent = object->parent;
	if (!parent) {
		_leave(" [no parent]");
		return transit_to(DROP_OBJECT);
	}

	_debug("parent: %s of:%lx", parent->state->name, parent->flags);

	if (fscache_object_is_dying(parent)) {
		_leave(" [bad parent]");
		return transit_to(DROP_OBJECT);
	}

	if (fscache_object_is_available(parent)) {
		_leave(" [ready]");
		return transit_to(PARENT_READY);
	}

	_debug("wait");

	spin_lock(&parent->lock);
	fscache_stat(&fscache_n_cop_grab_object);
	success = false;
	if (fscache_object_is_live(parent) &&
	    object->cache->ops->grab_object(object)) {
		list_add(&object->dep_link, &parent->dependents);
		success = true;
	}
	fscache_stat_d(&fscache_n_cop_grab_object);
	spin_unlock(&parent->lock);
	if (!success) {
		_leave(" [grab failed]");
		return transit_to(DROP_OBJECT);
	}

	/* fscache_acquire_non_index_cookie() uses this
	 * to wake the chain up */
	fscache_raise_event(parent, FSCACHE_OBJECT_EV_NEW_CHILD);
	_leave(" [wait]");
	return transit_to(WAIT_FOR_PARENT);
}

/*
 * Once the parent object is ready, we should kick off our lookup op.
 */
static const struct fscache_state *fscache_parent_ready(struct fscache_object *object,
							int event)
{
	struct fscache_object *parent = object->parent;

	_enter("{OBJ%x},%d", object->debug_id, event);

	ASSERT(parent != NULL);

	spin_lock(&parent->lock);
	parent->n_ops++;
	parent->n_obj_ops++;
	object->lookup_jif = jiffies;
	spin_unlock(&parent->lock);

	_leave("");
	return transit_to(LOOK_UP_OBJECT);
>>>>>>> refs/remotes/origin/master
}

/*
 * look an object up in the cache from which it was allocated
 * - we hold an "access lock" on the parent object, so the parent object cannot
 *   be withdrawn by either party till we've finished
<<<<<<< HEAD
 * - an object's cookie is pinned until we clear FSCACHE_COOKIE_CREATING on the
 *   leaf-most cookies of the object and all its children
 */
static void fscache_lookup_object(struct fscache_object *object)
{
	struct fscache_cookie *cookie = object->cookie;
	struct fscache_object *parent;
	int ret;

	_enter("");

	parent = object->parent;
=======
 */
static const struct fscache_state *fscache_look_up_object(struct fscache_object *object,
							  int event)
{
	struct fscache_cookie *cookie = object->cookie;
	struct fscache_object *parent = object->parent;
	int ret;

	_enter("{OBJ%x},%d", object->debug_id, event);

	object->oob_table = fscache_osm_lookup_oob;

>>>>>>> refs/remotes/origin/master
	ASSERT(parent != NULL);
	ASSERTCMP(parent->n_ops, >, 0);
	ASSERTCMP(parent->n_obj_ops, >, 0);

	/* make sure the parent is still available */
<<<<<<< HEAD
	ASSERTCMP(parent->state, >=, FSCACHE_OBJECT_AVAILABLE);

	if (parent->state >= FSCACHE_OBJECT_DYING ||
	    test_bit(FSCACHE_IOERROR, &object->cache->flags)) {
		_debug("unavailable");
		set_bit(FSCACHE_OBJECT_EV_WITHDRAW, &object->events);
		_leave("");
		return;
	}

	_debug("LOOKUP \"%s/%s\" in \"%s\"",
	       parent->cookie->def->name, cookie->def->name,
	       object->cache->tag->name);
=======
	ASSERT(fscache_object_is_available(parent));

	if (fscache_object_is_dying(parent) ||
	    test_bit(FSCACHE_IOERROR, &object->cache->flags) ||
	    !fscache_use_cookie(object)) {
		_leave(" [unavailable]");
		return transit_to(LOOKUP_FAILURE);
	}

	_debug("LOOKUP \"%s\" in \"%s\"",
	       cookie->def->name, object->cache->tag->name);
>>>>>>> refs/remotes/origin/master

	fscache_stat(&fscache_n_object_lookups);
	fscache_stat(&fscache_n_cop_lookup_object);
	ret = object->cache->ops->lookup_object(object);
	fscache_stat_d(&fscache_n_cop_lookup_object);

<<<<<<< HEAD
	if (test_bit(FSCACHE_OBJECT_EV_ERROR, &object->events))
		set_bit(FSCACHE_COOKIE_UNAVAILABLE, &cookie->flags);
=======
	fscache_unuse_cookie(object);
>>>>>>> refs/remotes/origin/master

	if (ret == -ETIMEDOUT) {
		/* probably stuck behind another object, so move this one to
		 * the back of the queue */
		fscache_stat(&fscache_n_object_lookups_timed_out);
<<<<<<< HEAD
		set_bit(FSCACHE_OBJECT_EV_REQUEUE, &object->events);
	}

	_leave("");
=======
		_leave(" [timeout]");
		return NO_TRANSIT;
	}

	if (ret < 0) {
		_leave(" [error]");
		return transit_to(LOOKUP_FAILURE);
	}

	_leave(" [ok]");
	return transit_to(OBJECT_AVAILABLE);
>>>>>>> refs/remotes/origin/master
}

/**
 * fscache_object_lookup_negative - Note negative cookie lookup
 * @object: Object pointing to cookie to mark
 *
 * Note negative lookup, permitting those waiting to read data from an already
 * existing backing object to continue as there's no data for them to read.
 */
void fscache_object_lookup_negative(struct fscache_object *object)
{
	struct fscache_cookie *cookie = object->cookie;

<<<<<<< HEAD
	_enter("{OBJ%x,%s}",
	       object->debug_id, fscache_object_states[object->state]);

	spin_lock(&object->lock);
	if (object->state == FSCACHE_OBJECT_LOOKING_UP) {
		fscache_stat(&fscache_n_object_lookups_negative);

		/* transit here to allow write requests to begin stacking up
		 * and read requests to begin returning ENODATA */
		object->state = FSCACHE_OBJECT_CREATING;
		spin_unlock(&object->lock);

		set_bit(FSCACHE_COOKIE_PENDING_FILL, &cookie->flags);
		set_bit(FSCACHE_COOKIE_NO_DATA_YET, &cookie->flags);

		_debug("wake up lookup %p", &cookie->flags);
		smp_mb__before_clear_bit();
		clear_bit(FSCACHE_COOKIE_LOOKING_UP, &cookie->flags);
		smp_mb__after_clear_bit();
		wake_up_bit(&cookie->flags, FSCACHE_COOKIE_LOOKING_UP);
		set_bit(FSCACHE_OBJECT_EV_REQUEUE, &object->events);
	} else {
		ASSERTCMP(object->state, ==, FSCACHE_OBJECT_CREATING);
		spin_unlock(&object->lock);
	}

=======
	_enter("{OBJ%x,%s}", object->debug_id, object->state->name);

	if (!test_and_set_bit(FSCACHE_OBJECT_IS_LOOKED_UP, &object->flags)) {
		fscache_stat(&fscache_n_object_lookups_negative);

		/* Allow write requests to begin stacking up and read requests to begin
		 * returning ENODATA.
		 */
		set_bit(FSCACHE_COOKIE_NO_DATA_YET, &cookie->flags);
		clear_bit(FSCACHE_COOKIE_UNAVAILABLE, &cookie->flags);

		_debug("wake up lookup %p", &cookie->flags);
		clear_bit_unlock(FSCACHE_COOKIE_LOOKING_UP, &cookie->flags);
		wake_up_bit(&cookie->flags, FSCACHE_COOKIE_LOOKING_UP);
	}
>>>>>>> refs/remotes/origin/master
	_leave("");
}
EXPORT_SYMBOL(fscache_object_lookup_negative);

/**
 * fscache_obtained_object - Note successful object lookup or creation
 * @object: Object pointing to cookie to mark
 *
 * Note successful lookup and/or creation, permitting those waiting to write
 * data to a backing object to continue.
 *
 * Note that after calling this, an object's cookie may be relinquished by the
 * netfs, and so must be accessed with object lock held.
 */
void fscache_obtained_object(struct fscache_object *object)
{
	struct fscache_cookie *cookie = object->cookie;

<<<<<<< HEAD
	_enter("{OBJ%x,%s}",
	       object->debug_id, fscache_object_states[object->state]);

	/* if we were still looking up, then we must have a positive lookup
	 * result, in which case there may be data available */
	spin_lock(&object->lock);
	if (object->state == FSCACHE_OBJECT_LOOKING_UP) {
		fscache_stat(&fscache_n_object_lookups_positive);

		clear_bit(FSCACHE_COOKIE_NO_DATA_YET, &cookie->flags);

		object->state = FSCACHE_OBJECT_AVAILABLE;
		spin_unlock(&object->lock);

		smp_mb__before_clear_bit();
		clear_bit(FSCACHE_COOKIE_LOOKING_UP, &cookie->flags);
		smp_mb__after_clear_bit();
		wake_up_bit(&cookie->flags, FSCACHE_COOKIE_LOOKING_UP);
		set_bit(FSCACHE_OBJECT_EV_REQUEUE, &object->events);
	} else {
		ASSERTCMP(object->state, ==, FSCACHE_OBJECT_CREATING);
		fscache_stat(&fscache_n_object_created);

		object->state = FSCACHE_OBJECT_AVAILABLE;
		spin_unlock(&object->lock);
		set_bit(FSCACHE_OBJECT_EV_REQUEUE, &object->events);
		smp_wmb();
	}

	if (test_and_clear_bit(FSCACHE_COOKIE_CREATING, &cookie->flags))
		wake_up_bit(&cookie->flags, FSCACHE_COOKIE_CREATING);

=======
	_enter("{OBJ%x,%s}", object->debug_id, object->state->name);

	/* if we were still looking up, then we must have a positive lookup
	 * result, in which case there may be data available */
	if (!test_and_set_bit(FSCACHE_OBJECT_IS_LOOKED_UP, &object->flags)) {
		fscache_stat(&fscache_n_object_lookups_positive);

		/* We do (presumably) have data */
		clear_bit_unlock(FSCACHE_COOKIE_NO_DATA_YET, &cookie->flags);
		clear_bit(FSCACHE_COOKIE_UNAVAILABLE, &cookie->flags);

		/* Allow write requests to begin stacking up and read requests
		 * to begin shovelling data.
		 */
		clear_bit_unlock(FSCACHE_COOKIE_LOOKING_UP, &cookie->flags);
		wake_up_bit(&cookie->flags, FSCACHE_COOKIE_LOOKING_UP);
	} else {
		fscache_stat(&fscache_n_object_created);
	}

	set_bit(FSCACHE_OBJECT_IS_AVAILABLE, &object->flags);
>>>>>>> refs/remotes/origin/master
	_leave("");
}
EXPORT_SYMBOL(fscache_obtained_object);

/*
 * handle an object that has just become available
 */
<<<<<<< HEAD
static void fscache_object_available(struct fscache_object *object)
{
	_enter("{OBJ%x}", object->debug_id);

	spin_lock(&object->lock);

	if (object->cookie &&
	    test_and_clear_bit(FSCACHE_COOKIE_CREATING, &object->cookie->flags))
		wake_up_bit(&object->cookie->flags, FSCACHE_COOKIE_CREATING);
=======
static const struct fscache_state *fscache_object_available(struct fscache_object *object,
							    int event)
{
	_enter("{OBJ%x},%d", object->debug_id, event);

	object->oob_table = fscache_osm_run_oob;

	spin_lock(&object->lock);
>>>>>>> refs/remotes/origin/master

	fscache_done_parent_op(object);
	if (object->n_in_progress == 0) {
		if (object->n_ops > 0) {
			ASSERTCMP(object->n_ops, >=, object->n_obj_ops);
<<<<<<< HEAD
			ASSERTIF(object->n_ops > object->n_obj_ops,
				 !list_empty(&object->pending_ops));
=======
>>>>>>> refs/remotes/origin/master
			fscache_start_operations(object);
		} else {
			ASSERT(list_empty(&object->pending_ops));
		}
	}
	spin_unlock(&object->lock);

	fscache_stat(&fscache_n_cop_lookup_complete);
	object->cache->ops->lookup_complete(object);
	fscache_stat_d(&fscache_n_cop_lookup_complete);
<<<<<<< HEAD
	fscache_enqueue_dependents(object);
=======
>>>>>>> refs/remotes/origin/master

	fscache_hist(fscache_obj_instantiate_histogram, object->lookup_jif);
	fscache_stat(&fscache_n_object_avail);

	_leave("");
<<<<<<< HEAD
}

/*
 * drop an object's attachments
 */
static void fscache_drop_object(struct fscache_object *object)
{
	struct fscache_object *parent = object->parent;
	struct fscache_cache *cache = object->cache;

	_enter("{OBJ%x,%d}", object->debug_id, object->n_children);

	ASSERTCMP(object->cookie, ==, NULL);
	ASSERT(hlist_unhashed(&object->cookie_link));

	spin_lock(&cache->object_list_lock);
	list_del_init(&object->cache_link);
	spin_unlock(&cache->object_list_lock);

	fscache_stat(&fscache_n_cop_drop_object);
	cache->ops->drop_object(object);
	fscache_stat_d(&fscache_n_cop_drop_object);

	if (parent) {
		_debug("release parent OBJ%x {%d}",
		       parent->debug_id, parent->n_children);

		spin_lock(&parent->lock);
		parent->n_children--;
		if (parent->n_children == 0)
			fscache_raise_event(parent, FSCACHE_OBJECT_EV_CLEARED);
		spin_unlock(&parent->lock);
		object->parent = NULL;
	}

	/* this just shifts the object release to the work processor */
	fscache_put_object(object);

	_leave("");
}

/*
 * release or recycle an object that the netfs has discarded
 */
static void fscache_release_object(struct fscache_object *object)
{
	_enter("");

	fscache_drop_object(object);
}

/*
 * withdraw an object from active service
 */
static void fscache_withdraw_object(struct fscache_object *object)
{
	struct fscache_cookie *cookie;
	bool detached;

	_enter("");

	spin_lock(&object->lock);
	cookie = object->cookie;
	if (cookie) {
		/* need to get the cookie lock before the object lock, starting
		 * from the object pointer */
		atomic_inc(&cookie->usage);
		spin_unlock(&object->lock);

		detached = false;
		spin_lock(&cookie->lock);
		spin_lock(&object->lock);

		if (object->cookie == cookie) {
			hlist_del_init(&object->cookie_link);
			object->cookie = NULL;
			detached = true;
		}
		spin_unlock(&cookie->lock);
		fscache_cookie_put(cookie);
		if (detached)
			fscache_cookie_put(cookie);
	}

	spin_unlock(&object->lock);

	fscache_drop_object(object);
}

/*
 * withdraw an object from active service at the behest of the cache
 * - need break the links to a cached object cookie
 * - called under two situations:
 *   (1) recycler decides to reclaim an in-use object
 *   (2) a cache is unmounted
 * - have to take care as the cookie can be being relinquished by the netfs
 *   simultaneously
 * - the object is pinned by the caller holding a refcount on it
 */
void fscache_withdrawing_object(struct fscache_cache *cache,
				struct fscache_object *object)
{
	bool enqueue = false;

	_enter(",OBJ%x", object->debug_id);

	spin_lock(&object->lock);
	if (object->state < FSCACHE_OBJECT_WITHDRAWING) {
		object->state = FSCACHE_OBJECT_WITHDRAWING;
		enqueue = true;
	}
	spin_unlock(&object->lock);

	if (enqueue)
		fscache_enqueue_object(object);

	_leave("");
=======
	return transit_to(JUMPSTART_DEPS);
}

/*
 * Wake up this object's dependent objects now that we've become available.
 */
static const struct fscache_state *fscache_jumpstart_dependents(struct fscache_object *object,
								int event)
{
	_enter("{OBJ%x},%d", object->debug_id, event);

	if (!fscache_enqueue_dependents(object, FSCACHE_OBJECT_EV_PARENT_READY))
		return NO_TRANSIT; /* Not finished; requeue */
	return transit_to(WAIT_FOR_CMD);
}

/*
 * Handle lookup or creation failute.
 */
static const struct fscache_state *fscache_lookup_failure(struct fscache_object *object,
							  int event)
{
	struct fscache_cookie *cookie;

	_enter("{OBJ%x},%d", object->debug_id, event);

	object->oob_event_mask = 0;

	fscache_stat(&fscache_n_cop_lookup_complete);
	object->cache->ops->lookup_complete(object);
	fscache_stat_d(&fscache_n_cop_lookup_complete);

	cookie = object->cookie;
	set_bit(FSCACHE_COOKIE_UNAVAILABLE, &cookie->flags);
	if (test_and_clear_bit(FSCACHE_COOKIE_LOOKING_UP, &cookie->flags))
		wake_up_bit(&cookie->flags, FSCACHE_COOKIE_LOOKING_UP);

	fscache_done_parent_op(object);
	return transit_to(KILL_OBJECT);
}

/*
 * Wait for completion of all active operations on this object and the death of
 * all child objects of this object.
 */
static const struct fscache_state *fscache_kill_object(struct fscache_object *object,
						       int event)
{
	_enter("{OBJ%x,%d,%d},%d",
	       object->debug_id, object->n_ops, object->n_children, event);

	clear_bit(FSCACHE_OBJECT_IS_LIVE, &object->flags);
	object->oob_event_mask = 0;

	if (list_empty(&object->dependents) &&
	    object->n_ops == 0 &&
	    object->n_children == 0)
		return transit_to(DROP_OBJECT);

	if (object->n_in_progress == 0) {
		spin_lock(&object->lock);
		if (object->n_ops > 0 && object->n_in_progress == 0)
			fscache_start_operations(object);
		spin_unlock(&object->lock);
	}

	if (!list_empty(&object->dependents))
		return transit_to(KILL_DEPENDENTS);

	return transit_to(WAIT_FOR_CLEARANCE);
}

/*
 * Kill dependent objects.
 */
static const struct fscache_state *fscache_kill_dependents(struct fscache_object *object,
							   int event)
{
	_enter("{OBJ%x},%d", object->debug_id, event);

	if (!fscache_enqueue_dependents(object, FSCACHE_OBJECT_EV_KILL))
		return NO_TRANSIT; /* Not finished */
	return transit_to(WAIT_FOR_CLEARANCE);
}

/*
 * Drop an object's attachments
 */
static const struct fscache_state *fscache_drop_object(struct fscache_object *object,
						       int event)
{
	struct fscache_object *parent = object->parent;
	struct fscache_cookie *cookie = object->cookie;
	struct fscache_cache *cache = object->cache;
	bool awaken = false;

	_enter("{OBJ%x,%d},%d", object->debug_id, object->n_children, event);

	ASSERT(cookie != NULL);
	ASSERT(!hlist_unhashed(&object->cookie_link));

	/* Make sure the cookie no longer points here and that the netfs isn't
	 * waiting for us.
	 */
	spin_lock(&cookie->lock);
	hlist_del_init(&object->cookie_link);
	if (hlist_empty(&cookie->backing_objects) &&
	    test_and_clear_bit(FSCACHE_COOKIE_INVALIDATING, &cookie->flags))
		awaken = true;
	spin_unlock(&cookie->lock);

	if (awaken)
		wake_up_bit(&cookie->flags, FSCACHE_COOKIE_INVALIDATING);

	/* Prevent a race with our last child, which has to signal EV_CLEARED
	 * before dropping our spinlock.
	 */
	spin_lock(&object->lock);
	spin_unlock(&object->lock);

	/* Discard from the cache's collection of objects */
	spin_lock(&cache->object_list_lock);
	list_del_init(&object->cache_link);
	spin_unlock(&cache->object_list_lock);

	fscache_stat(&fscache_n_cop_drop_object);
	cache->ops->drop_object(object);
	fscache_stat_d(&fscache_n_cop_drop_object);

	/* The parent object wants to know when all it dependents have gone */
	if (parent) {
		_debug("release parent OBJ%x {%d}",
		       parent->debug_id, parent->n_children);

		spin_lock(&parent->lock);
		parent->n_children--;
		if (parent->n_children == 0)
			fscache_raise_event(parent, FSCACHE_OBJECT_EV_CLEARED);
		spin_unlock(&parent->lock);
		object->parent = NULL;
	}

	/* this just shifts the object release to the work processor */
	fscache_put_object(object);
	fscache_stat(&fscache_n_object_dead);

	_leave("");
	return transit_to(OBJECT_DEAD);
>>>>>>> refs/remotes/origin/master
}

/*
 * get a ref on an object
 */
static int fscache_get_object(struct fscache_object *object)
{
	int ret;

	fscache_stat(&fscache_n_cop_grab_object);
	ret = object->cache->ops->grab_object(object) ? 0 : -EAGAIN;
	fscache_stat_d(&fscache_n_cop_grab_object);
	return ret;
}

/*
<<<<<<< HEAD
 * discard a ref on a work item
=======
 * Discard a ref on an object
>>>>>>> refs/remotes/origin/master
 */
static void fscache_put_object(struct fscache_object *object)
{
	fscache_stat(&fscache_n_cop_put_object);
	object->cache->ops->put_object(object);
	fscache_stat_d(&fscache_n_cop_put_object);
}

<<<<<<< HEAD
=======
/**
 * fscache_object_destroy - Note that a cache object is about to be destroyed
 * @object: The object to be destroyed
 *
 * Note the imminent destruction and deallocation of a cache object record.
 */
void fscache_object_destroy(struct fscache_object *object)
{
	fscache_objlist_remove(object);

	/* We can get rid of the cookie now */
	fscache_cookie_put(object->cookie);
	object->cookie = NULL;
}
EXPORT_SYMBOL(fscache_object_destroy);

>>>>>>> refs/remotes/origin/master
/*
 * enqueue an object for metadata-type processing
 */
void fscache_enqueue_object(struct fscache_object *object)
{
	_enter("{OBJ%x}", object->debug_id);

	if (fscache_get_object(object) >= 0) {
		wait_queue_head_t *cong_wq =
			&get_cpu_var(fscache_object_cong_wait);

		if (queue_work(fscache_object_wq, &object->work)) {
			if (fscache_object_congested())
				wake_up(cong_wq);
		} else
			fscache_put_object(object);

		put_cpu_var(fscache_object_cong_wait);
	}
}

/**
 * fscache_object_sleep_till_congested - Sleep until object wq is congested
<<<<<<< HEAD
 * @timoutp: Scheduler sleep timeout
=======
 * @timeoutp: Scheduler sleep timeout
>>>>>>> refs/remotes/origin/master
 *
 * Allow an object handler to sleep until the object workqueue is congested.
 *
 * The caller must set up a wake up event before calling this and must have set
 * the appropriate sleep mode (such as TASK_UNINTERRUPTIBLE) and tested its own
 * condition before calling this function as no test is made here.
 *
 * %true is returned if the object wq is congested, %false otherwise.
 */
bool fscache_object_sleep_till_congested(signed long *timeoutp)
{
<<<<<<< HEAD
	wait_queue_head_t *cong_wq = &__get_cpu_var(fscache_object_cong_wait);
=======
	wait_queue_head_t *cong_wq = this_cpu_ptr(&fscache_object_cong_wait);
>>>>>>> refs/remotes/origin/master
	DEFINE_WAIT(wait);

	if (fscache_object_congested())
		return true;

	add_wait_queue_exclusive(cong_wq, &wait);
	if (!fscache_object_congested())
		*timeoutp = schedule_timeout(*timeoutp);
	finish_wait(cong_wq, &wait);

	return fscache_object_congested();
}
EXPORT_SYMBOL_GPL(fscache_object_sleep_till_congested);

/*
<<<<<<< HEAD
 * enqueue the dependents of an object for metadata-type processing
 * - the caller must hold the object's lock
 * - this may cause an already locked object to wind up being processed again
 */
static void fscache_enqueue_dependents(struct fscache_object *object)
{
	struct fscache_object *dep;
=======
 * Enqueue the dependents of an object for metadata-type processing.
 *
 * If we don't manage to finish the list before the scheduler wants to run
 * again then return false immediately.  We return true if the list was
 * cleared.
 */
static bool fscache_enqueue_dependents(struct fscache_object *object, int event)
{
	struct fscache_object *dep;
	bool ret = true;
>>>>>>> refs/remotes/origin/master

	_enter("{OBJ%x}", object->debug_id);

	if (list_empty(&object->dependents))
<<<<<<< HEAD
		return;
=======
		return true;
>>>>>>> refs/remotes/origin/master

	spin_lock(&object->lock);

	while (!list_empty(&object->dependents)) {
		dep = list_entry(object->dependents.next,
				 struct fscache_object, dep_link);
		list_del_init(&dep->dep_link);

<<<<<<< HEAD

		/* sort onto appropriate lists */
		fscache_enqueue_object(dep);
		fscache_put_object(dep);

		if (!list_empty(&object->dependents))
			cond_resched_lock(&object->lock);
	}

	spin_unlock(&object->lock);
=======
		fscache_raise_event(dep, event);
		fscache_put_object(dep);

		if (!list_empty(&object->dependents) && need_resched()) {
			ret = false;
			break;
		}
	}

	spin_unlock(&object->lock);
	return ret;
>>>>>>> refs/remotes/origin/master
}

/*
 * remove an object from whatever queue it's waiting on
<<<<<<< HEAD
 * - the caller must hold object->lock
 */
void fscache_dequeue_object(struct fscache_object *object)
=======
 */
static void fscache_dequeue_object(struct fscache_object *object)
>>>>>>> refs/remotes/origin/master
{
	_enter("{OBJ%x}", object->debug_id);

	if (!list_empty(&object->dep_link)) {
		spin_lock(&object->parent->lock);
		list_del_init(&object->dep_link);
		spin_unlock(&object->parent->lock);
	}

	_leave("");
}

/**
 * fscache_check_aux - Ask the netfs whether an object on disk is still valid
 * @object: The object to ask about
 * @data: The auxiliary data for the object
 * @datalen: The size of the auxiliary data
 *
<<<<<<< HEAD
 * This function consults the netfs about the coherency state of an object
=======
 * This function consults the netfs about the coherency state of an object.
 * The caller must be holding a ref on cookie->n_active (held by
 * fscache_look_up_object() on behalf of the cache backend during object lookup
 * and creation).
>>>>>>> refs/remotes/origin/master
 */
enum fscache_checkaux fscache_check_aux(struct fscache_object *object,
					const void *data, uint16_t datalen)
{
	enum fscache_checkaux result;

	if (!object->cookie->def->check_aux) {
		fscache_stat(&fscache_n_checkaux_none);
		return FSCACHE_CHECKAUX_OKAY;
	}

	result = object->cookie->def->check_aux(object->cookie->netfs_data,
						data, datalen);
	switch (result) {
		/* entry okay as is */
	case FSCACHE_CHECKAUX_OKAY:
		fscache_stat(&fscache_n_checkaux_okay);
		break;

		/* entry requires update */
	case FSCACHE_CHECKAUX_NEEDS_UPDATE:
		fscache_stat(&fscache_n_checkaux_update);
		break;

		/* entry requires deletion */
	case FSCACHE_CHECKAUX_OBSOLETE:
		fscache_stat(&fscache_n_checkaux_obsolete);
		break;

	default:
		BUG();
	}

	return result;
}
EXPORT_SYMBOL(fscache_check_aux);
<<<<<<< HEAD
=======

/*
 * Asynchronously invalidate an object.
 */
static const struct fscache_state *_fscache_invalidate_object(struct fscache_object *object,
							      int event)
{
	struct fscache_operation *op;
	struct fscache_cookie *cookie = object->cookie;

	_enter("{OBJ%x},%d", object->debug_id, event);

	/* We're going to need the cookie.  If the cookie is not available then
	 * retire the object instead.
	 */
	if (!fscache_use_cookie(object)) {
		ASSERT(object->cookie->stores.rnode == NULL);
		set_bit(FSCACHE_OBJECT_RETIRED, &object->flags);
		_leave(" [no cookie]");
		return transit_to(KILL_OBJECT);
	}

	/* Reject any new read/write ops and abort any that are pending. */
	fscache_invalidate_writes(cookie);
	clear_bit(FSCACHE_OBJECT_PENDING_WRITE, &object->flags);
	fscache_cancel_all_ops(object);

	/* Now we have to wait for in-progress reads and writes */
	op = kzalloc(sizeof(*op), GFP_KERNEL);
	if (!op)
		goto nomem;

	fscache_operation_init(op, object->cache->ops->invalidate_object, NULL);
	op->flags = FSCACHE_OP_ASYNC |
		(1 << FSCACHE_OP_EXCLUSIVE) |
		(1 << FSCACHE_OP_UNUSE_COOKIE);

	spin_lock(&cookie->lock);
	if (fscache_submit_exclusive_op(object, op) < 0)
		goto submit_op_failed;
	spin_unlock(&cookie->lock);
	fscache_put_operation(op);

	/* Once we've completed the invalidation, we know there will be no data
	 * stored in the cache and thus we can reinstate the data-check-skip
	 * optimisation.
	 */
	set_bit(FSCACHE_COOKIE_NO_DATA_YET, &cookie->flags);

	/* We can allow read and write requests to come in once again.  They'll
	 * queue up behind our exclusive invalidation operation.
	 */
	if (test_and_clear_bit(FSCACHE_COOKIE_INVALIDATING, &cookie->flags))
		wake_up_bit(&cookie->flags, FSCACHE_COOKIE_INVALIDATING);
	_leave(" [ok]");
	return transit_to(UPDATE_OBJECT);

nomem:
	clear_bit(FSCACHE_OBJECT_IS_LIVE, &object->flags);
	fscache_unuse_cookie(object);
	_leave(" [ENOMEM]");
	return transit_to(KILL_OBJECT);

submit_op_failed:
	clear_bit(FSCACHE_OBJECT_IS_LIVE, &object->flags);
	spin_unlock(&cookie->lock);
	kfree(op);
	_leave(" [EIO]");
	return transit_to(KILL_OBJECT);
}

static const struct fscache_state *fscache_invalidate_object(struct fscache_object *object,
							     int event)
{
	const struct fscache_state *s;

	fscache_stat(&fscache_n_invalidates_run);
	fscache_stat(&fscache_n_cop_invalidate_object);
	s = _fscache_invalidate_object(object, event);
	fscache_stat_d(&fscache_n_cop_invalidate_object);
	return s;
}

/*
 * Asynchronously update an object.
 */
static const struct fscache_state *fscache_update_object(struct fscache_object *object,
							 int event)
{
	_enter("{OBJ%x},%d", object->debug_id, event);

	fscache_stat(&fscache_n_updates_run);
	fscache_stat(&fscache_n_cop_update_object);
	object->cache->ops->update_object(object);
	fscache_stat_d(&fscache_n_cop_update_object);

	_leave("");
	return transit_to(WAIT_FOR_CMD);
}
>>>>>>> refs/remotes/origin/master
