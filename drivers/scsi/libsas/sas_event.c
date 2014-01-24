/*
 * Serial Attached SCSI (SAS) Event processing
 *
 * Copyright (C) 2005 Adaptec, Inc.  All rights reserved.
 * Copyright (C) 2005 Luben Tuikov <luben_tuikov@adaptec.com>
 *
 * This file is licensed under GPLv2.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <scsi/scsi_host.h>
#include "sas_internal.h"
#include "sas_dump.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
void sas_queue_work(struct sas_ha_struct *ha, struct sas_work *sw)
{
	if (!test_bit(SAS_HA_REGISTERED, &ha->state))
		return;

	if (test_bit(SAS_HA_DRAINING, &ha->state)) {
		/* add it to the defer list, if not already pending */
		if (list_empty(&sw->drain_node))
			list_add(&sw->drain_node, &ha->defer_q);
	} else
		scsi_queue_work(ha->core.shost, &sw->work);
}

static void sas_queue_event(int event, unsigned long *pending,
			    struct sas_work *work,
			    struct sas_ha_struct *ha)
{
	if (!test_and_set_bit(event, pending)) {
		unsigned long flags;

<<<<<<< HEAD
		spin_lock_irqsave(&ha->state_lock, flags);
		sas_queue_work(ha, work);
		spin_unlock_irqrestore(&ha->state_lock, flags);
=======
		spin_lock_irqsave(&ha->lock, flags);
		sas_queue_work(ha, work);
		spin_unlock_irqrestore(&ha->lock, flags);
>>>>>>> refs/remotes/origin/master
	}
}


void __sas_drain_work(struct sas_ha_struct *ha)
{
	struct workqueue_struct *wq = ha->core.shost->work_q;
	struct sas_work *sw, *_sw;

	set_bit(SAS_HA_DRAINING, &ha->state);
	/* flush submitters */
<<<<<<< HEAD
	spin_lock_irq(&ha->state_lock);
	spin_unlock_irq(&ha->state_lock);

	drain_workqueue(wq);

	spin_lock_irq(&ha->state_lock);
=======
	spin_lock_irq(&ha->lock);
	spin_unlock_irq(&ha->lock);

	drain_workqueue(wq);

	spin_lock_irq(&ha->lock);
>>>>>>> refs/remotes/origin/master
	clear_bit(SAS_HA_DRAINING, &ha->state);
	list_for_each_entry_safe(sw, _sw, &ha->defer_q, drain_node) {
		list_del_init(&sw->drain_node);
		sas_queue_work(ha, sw);
	}
<<<<<<< HEAD
	spin_unlock_irq(&ha->state_lock);
=======
	spin_unlock_irq(&ha->lock);
>>>>>>> refs/remotes/origin/master
}

int sas_drain_work(struct sas_ha_struct *ha)
{
	int err;

	err = mutex_lock_interruptible(&ha->drain_mutex);
	if (err)
		return err;
	if (test_bit(SAS_HA_REGISTERED, &ha->state))
		__sas_drain_work(ha);
	mutex_unlock(&ha->drain_mutex);

	return 0;
}
EXPORT_SYMBOL_GPL(sas_drain_work);

void sas_disable_revalidation(struct sas_ha_struct *ha)
{
	mutex_lock(&ha->disco_mutex);
	set_bit(SAS_HA_ATA_EH_ACTIVE, &ha->state);
	mutex_unlock(&ha->disco_mutex);
}

void sas_enable_revalidation(struct sas_ha_struct *ha)
{
	int i;

	mutex_lock(&ha->disco_mutex);
	clear_bit(SAS_HA_ATA_EH_ACTIVE, &ha->state);
	for (i = 0; i < ha->num_phys; i++) {
		struct asd_sas_port *port = ha->sas_port[i];
		const int ev = DISCE_REVALIDATE_DOMAIN;
		struct sas_discovery *d = &port->disc;

		if (!test_and_clear_bit(ev, &d->pending))
			continue;

		sas_queue_event(ev, &d->pending, &d->disc_work[ev].work, ha);
	}
	mutex_unlock(&ha->disco_mutex);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void notify_ha_event(struct sas_ha_struct *sas_ha, enum ha_event event)
{
	BUG_ON(event >= HA_NUM_EVENTS);

<<<<<<< HEAD
<<<<<<< HEAD
	sas_queue_event(event, &sas_ha->event_lock, &sas_ha->pending,
=======
	sas_queue_event(event, &sas_ha->pending,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sas_queue_event(event, &sas_ha->pending,
>>>>>>> refs/remotes/origin/master
			&sas_ha->ha_events[event].work, sas_ha);
}

static void notify_port_event(struct asd_sas_phy *phy, enum port_event event)
{
	struct sas_ha_struct *ha = phy->ha;

	BUG_ON(event >= PORT_NUM_EVENTS);

<<<<<<< HEAD
<<<<<<< HEAD
	sas_queue_event(event, &ha->event_lock, &phy->port_events_pending,
=======
	sas_queue_event(event, &phy->port_events_pending,
>>>>>>> refs/remotes/origin/cm-10.0
			&phy->port_events[event].work, ha);
}

static void notify_phy_event(struct asd_sas_phy *phy, enum phy_event event)
=======
	sas_queue_event(event, &phy->port_events_pending,
			&phy->port_events[event].work, ha);
}

void sas_notify_phy_event(struct asd_sas_phy *phy, enum phy_event event)
>>>>>>> refs/remotes/origin/master
{
	struct sas_ha_struct *ha = phy->ha;

	BUG_ON(event >= PHY_NUM_EVENTS);

<<<<<<< HEAD
<<<<<<< HEAD
	sas_queue_event(event, &ha->event_lock, &phy->phy_events_pending,
=======
	sas_queue_event(event, &phy->phy_events_pending,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sas_queue_event(event, &phy->phy_events_pending,
>>>>>>> refs/remotes/origin/master
			&phy->phy_events[event].work, ha);
}

int sas_init_events(struct sas_ha_struct *sas_ha)
{
	static const work_func_t sas_ha_event_fns[HA_NUM_EVENTS] = {
		[HAE_RESET] = sas_hae_reset,
	};

	int i;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_init(&sas_ha->event_lock);

	for (i = 0; i < HA_NUM_EVENTS; i++) {
		INIT_WORK(&sas_ha->ha_events[i].work, sas_ha_event_fns[i]);
=======
	for (i = 0; i < HA_NUM_EVENTS; i++) {
		INIT_SAS_WORK(&sas_ha->ha_events[i].work, sas_ha_event_fns[i]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (i = 0; i < HA_NUM_EVENTS; i++) {
		INIT_SAS_WORK(&sas_ha->ha_events[i].work, sas_ha_event_fns[i]);
>>>>>>> refs/remotes/origin/master
		sas_ha->ha_events[i].ha = sas_ha;
	}

	sas_ha->notify_ha_event = notify_ha_event;
	sas_ha->notify_port_event = notify_port_event;
<<<<<<< HEAD
	sas_ha->notify_phy_event = notify_phy_event;
=======
	sas_ha->notify_phy_event = sas_notify_phy_event;
>>>>>>> refs/remotes/origin/master

	return 0;
}
