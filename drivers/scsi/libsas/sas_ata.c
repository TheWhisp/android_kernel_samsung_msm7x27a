/*
 * Support for SATA devices on Serial Attached SCSI (SAS) controllers
 *
 * Copyright (C) 2006 IBM Corporation
 *
 * Written by: Darrick J. Wong <djwong@us.ibm.com>, IBM Corporation
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include <linux/scatterlist.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/async.h>
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/async.h>
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master

#include <scsi/sas_ata.h>
#include "sas_internal.h"
#include <scsi/scsi_host.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_tcq.h>
#include <scsi/scsi.h>
#include <scsi/scsi_transport.h>
#include <scsi/scsi_transport_sas.h>
#include "../scsi_sas_internal.h"
#include "../scsi_transport_api.h"
#include <scsi/scsi_eh.h>

static enum ata_completion_errors sas_to_ata_err(struct task_status_struct *ts)
{
	/* Cheesy attempt to translate SAS errors into ATA.  Hah! */

	/* transport error */
	if (ts->resp == SAS_TASK_UNDELIVERED)
		return AC_ERR_ATA_BUS;

	/* ts->resp == SAS_TASK_COMPLETE */
	/* task delivered, what happened afterwards? */
	switch (ts->stat) {
		case SAS_DEV_NO_RESPONSE:
			return AC_ERR_TIMEOUT;

		case SAS_INTERRUPTED:
		case SAS_PHY_DOWN:
		case SAS_NAK_R_ERR:
			return AC_ERR_ATA_BUS;


		case SAS_DATA_UNDERRUN:
			/*
			 * Some programs that use the taskfile interface
			 * (smartctl in particular) can cause underrun
			 * problems.  Ignore these errors, perhaps at our
			 * peril.
			 */
			return 0;

		case SAS_DATA_OVERRUN:
		case SAS_QUEUE_FULL:
		case SAS_DEVICE_UNKNOWN:
		case SAS_SG_ERR:
			return AC_ERR_INVALID;

		case SAS_OPEN_TO:
		case SAS_OPEN_REJECT:
			SAS_DPRINTK("%s: Saw error %d.  What to do?\n",
				    __func__, ts->stat);
			return AC_ERR_OTHER;

		case SAM_STAT_CHECK_CONDITION:
		case SAS_ABORTED_TASK:
			return AC_ERR_DEV;

		case SAS_PROTO_RESPONSE:
			/* This means the ending_fis has the error
			 * value; return 0 here to collect it */
			return 0;
		default:
			return 0;
	}
}

static void sas_ata_task_done(struct sas_task *task)
{
	struct ata_queued_cmd *qc = task->uldd_task;
<<<<<<< HEAD
<<<<<<< HEAD
	struct domain_device *dev;
	struct task_status_struct *stat = &task->task_status;
	struct ata_task_resp *resp = (struct ata_task_resp *)stat->buf;
	struct sas_ha_struct *sas_ha;
	enum ata_completion_errors ac;
	unsigned long flags;
	struct ata_link *link;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct domain_device *dev = task->dev;
	struct task_status_struct *stat = &task->task_status;
	struct ata_task_resp *resp = (struct ata_task_resp *)stat->buf;
	struct sas_ha_struct *sas_ha = dev->port->ha;
	enum ata_completion_errors ac;
	unsigned long flags;
	struct ata_link *link;
	struct ata_port *ap;

	spin_lock_irqsave(&dev->done_lock, flags);
	if (test_bit(SAS_HA_FROZEN, &sas_ha->state))
		task = NULL;
	else if (qc && qc->scsicmd)
		ASSIGN_SAS_TASK(qc->scsicmd, NULL);
	spin_unlock_irqrestore(&dev->done_lock, flags);

	/* check if libsas-eh got to the task before us */
	if (unlikely(!task))
		return;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (!qc)
		goto qc_already_gone;

<<<<<<< HEAD
<<<<<<< HEAD
	dev = qc->ap->private_data;
	sas_ha = dev->port->ha;
	link = &dev->sata_dev.ap->link;

	spin_lock_irqsave(dev->sata_dev.ap->lock, flags);
	if (stat->stat == SAS_PROTO_RESPONSE || stat->stat == SAM_STAT_GOOD ||
	    ((stat->stat == SAM_STAT_CHECK_CONDITION &&
	      dev->sata_dev.command_set == ATAPI_COMMAND_SET))) {
		ata_tf_from_fis(resp->ending_fis, &dev->sata_dev.tf);

		if (!link->sactive) {
			qc->err_mask |= ac_err_mask(dev->sata_dev.tf.command);
		} else {
			link->eh_info.err_mask |= ac_err_mask(dev->sata_dev.tf.command);
			if (unlikely(link->eh_info.err_mask))
				qc->flags |= ATA_QCFLAG_FAILED;
		}

		dev->sata_dev.sstatus = resp->sstatus;
		dev->sata_dev.serror = resp->serror;
		dev->sata_dev.scontrol = resp->scontrol;
=======
=======
>>>>>>> refs/remotes/origin/master
	ap = qc->ap;
	link = &ap->link;

	spin_lock_irqsave(ap->lock, flags);
	/* check if we lost the race with libata/sas_ata_post_internal() */
	if (unlikely(ap->pflags & ATA_PFLAG_FROZEN)) {
		spin_unlock_irqrestore(ap->lock, flags);
		if (qc->scsicmd)
			goto qc_already_gone;
		else {
			/* if eh is not involved and the port is frozen then the
			 * ata internal abort process has taken responsibility
			 * for this sas_task
			 */
			return;
		}
	}

	if (stat->stat == SAS_PROTO_RESPONSE || stat->stat == SAM_STAT_GOOD ||
	    ((stat->stat == SAM_STAT_CHECK_CONDITION &&
	      dev->sata_dev.command_set == ATAPI_COMMAND_SET))) {
		memcpy(dev->sata_dev.fis, resp->ending_fis, ATA_RESP_FIS_SIZE);

		if (!link->sactive) {
			qc->err_mask |= ac_err_mask(dev->sata_dev.fis[2]);
		} else {
			link->eh_info.err_mask |= ac_err_mask(dev->sata_dev.fis[2]);
			if (unlikely(link->eh_info.err_mask))
				qc->flags |= ATA_QCFLAG_FAILED;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	} else {
		ac = sas_to_ata_err(stat);
		if (ac) {
			SAS_DPRINTK("%s: SAS error %x\n", __func__,
				    stat->stat);
			/* We saw a SAS error. Send a vague error. */
			if (!link->sactive) {
				qc->err_mask = ac;
			} else {
				link->eh_info.err_mask |= AC_ERR_DEV;
				qc->flags |= ATA_QCFLAG_FAILED;
			}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
			dev->sata_dev.tf.feature = 0x04; /* status err */
			dev->sata_dev.tf.command = ATA_ERR;
=======
			dev->sata_dev.fis[3] = 0x04; /* status err */
			dev->sata_dev.fis[2] = ATA_ERR;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			dev->sata_dev.fis[3] = 0x04; /* status err */
			dev->sata_dev.fis[2] = ATA_ERR;
>>>>>>> refs/remotes/origin/master
=======
			dev->sata_dev.fis[3] = 0x04; /* status err */
			dev->sata_dev.fis[2] = ATA_ERR;
>>>>>>> refs/remotes/origin/cm-11.0
		}
	}

	qc->lldd_task = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	if (qc->scsicmd)
		ASSIGN_SAS_TASK(qc->scsicmd, NULL);
	ata_qc_complete(qc);
	spin_unlock_irqrestore(dev->sata_dev.ap->lock, flags);

	/*
	 * If the sas_task has an ata qc, a scsi_cmnd and the aborted
	 * flag is set, then we must have come in via the libsas EH
	 * functions.  When we exit this function, we need to put the
	 * scsi_cmnd on the list of finished errors.  The ata_qc_complete
	 * call cleans up the libata side of things but we're protected
	 * from the scsi_cmnd going away because the scsi_cmnd is owned
	 * by the EH, making libata's call to scsi_done a NOP.
	 */
	spin_lock_irqsave(&task->task_state_lock, flags);
	if (qc->scsicmd && task->task_state_flags & SAS_TASK_STATE_ABORTED)
		scsi_eh_finish_cmd(qc->scsicmd, &sas_ha->eh_done_q);
	spin_unlock_irqrestore(&task->task_state_lock, flags);
=======
	ata_qc_complete(qc);
	spin_unlock_irqrestore(ap->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ata_qc_complete(qc);
	spin_unlock_irqrestore(ap->lock, flags);
>>>>>>> refs/remotes/origin/master

qc_already_gone:
	list_del_init(&task->list);
	sas_free_task(task);
}

static unsigned int sas_ata_qc_issue(struct ata_queued_cmd *qc)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int res;
	struct sas_task *task;
	struct domain_device *dev = qc->ap->private_data;
	struct sas_ha_struct *sas_ha = dev->port->ha;
	struct Scsi_Host *host = sas_ha->core.shost;
	struct sas_internal *i = to_sas_internal(host->transportt);
	struct scatterlist *sg;
	unsigned int xfer = 0;
	unsigned int si;

	/* If the device fell off, no sense in issuing commands */
	if (dev->gone)
		return AC_ERR_SYSTEM;

	task = sas_alloc_task(GFP_ATOMIC);
	if (!task)
		return AC_ERR_SYSTEM;
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	struct sas_task *task;
	struct scatterlist *sg;
	int ret = AC_ERR_SYSTEM;
	unsigned int si, xfer = 0;
	struct ata_port *ap = qc->ap;
	struct domain_device *dev = ap->private_data;
	struct sas_ha_struct *sas_ha = dev->port->ha;
	struct Scsi_Host *host = sas_ha->core.shost;
	struct sas_internal *i = to_sas_internal(host->transportt);

	/* TODO: audit callers to ensure they are ready for qc_issue to
	 * unconditionally re-enable interrupts
	 */
	local_irq_save(flags);
	spin_unlock(ap->lock);

	/* If the device fell off, no sense in issuing commands */
	if (test_bit(SAS_DEV_GONE, &dev->state))
		goto out;

	task = sas_alloc_task(GFP_ATOMIC);
	if (!task)
		goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	task->dev = dev;
	task->task_proto = SAS_PROTOCOL_STP;
	task->task_done = sas_ata_task_done;

	if (qc->tf.command == ATA_CMD_FPDMA_WRITE ||
	    qc->tf.command == ATA_CMD_FPDMA_READ) {
		/* Need to zero out the tag libata assigned us */
		qc->tf.nsect = 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	ata_tf_to_fis(&qc->tf, 1, 0, (u8*)&task->ata_task.fis);
=======
	ata_tf_to_fis(&qc->tf, qc->dev->link->pmp, 1, (u8 *)&task->ata_task.fis);
>>>>>>> refs/remotes/origin/master
=======
	ata_tf_to_fis(&qc->tf, qc->dev->link->pmp, 1, (u8 *)&task->ata_task.fis);
>>>>>>> refs/remotes/origin/cm-11.0
	task->uldd_task = qc;
	if (ata_is_atapi(qc->tf.protocol)) {
		memcpy(task->ata_task.atapi_packet, qc->cdb, qc->dev->cdb_len);
		task->total_xfer_len = qc->nbytes;
		task->num_scatter = qc->n_elem;
	} else {
		for_each_sg(qc->sg, sg, qc->n_elem, si)
			xfer += sg->length;

		task->total_xfer_len = xfer;
		task->num_scatter = si;
	}

	task->data_dir = qc->dma_dir;
	task->scatter = qc->sg;
	task->ata_task.retry_count = 1;
	task->task_state_flags = SAS_TASK_STATE_PENDING;
	qc->lldd_task = task;

	switch (qc->tf.protocol) {
	case ATA_PROT_NCQ:
		task->ata_task.use_ncq = 1;
		/* fall through */
	case ATAPI_PROT_DMA:
	case ATA_PROT_DMA:
		task->ata_task.dma_xfer = 1;
		break;
	}

	if (qc->scsicmd)
		ASSIGN_SAS_TASK(qc->scsicmd, task);

	if (sas_ha->lldd_max_execute_num < 2)
<<<<<<< HEAD
<<<<<<< HEAD
		res = i->dft->lldd_execute_task(task, 1, GFP_ATOMIC);
	else
		res = sas_queue_up(task);

	/* Examine */
	if (res) {
		SAS_DPRINTK("lldd_execute_task returned: %d\n", res);
=======
=======
>>>>>>> refs/remotes/origin/master
		ret = i->dft->lldd_execute_task(task, 1, GFP_ATOMIC);
	else
		ret = sas_queue_up(task);

	/* Examine */
	if (ret) {
		SAS_DPRINTK("lldd_execute_task returned: %d\n", ret);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		if (qc->scsicmd)
			ASSIGN_SAS_TASK(qc->scsicmd, NULL);
		sas_free_task(task);
<<<<<<< HEAD
<<<<<<< HEAD
		return AC_ERR_SYSTEM;
	}

	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
		ret = AC_ERR_SYSTEM;
	}

 out:
	spin_lock(ap->lock);
	local_irq_restore(flags);
	return ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static bool sas_ata_qc_fill_rtf(struct ata_queued_cmd *qc)
{
	struct domain_device *dev = qc->ap->private_data;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	memcpy(&qc->result_tf, &dev->sata_dev.tf, sizeof(qc->result_tf));
=======
	ata_tf_from_fis(dev->sata_dev.fis, &qc->result_tf);
>>>>>>> refs/remotes/origin/cm-11.0
	return true;
}

static int sas_ata_hard_reset(struct ata_link *link, unsigned int *class,
			       unsigned long deadline)
{
	struct ata_port *ap = link->ap;
	struct domain_device *dev = ap->private_data;
	struct sas_internal *i =
		to_sas_internal(dev->port->ha->core.shost->transportt);
	int res = TMF_RESP_FUNC_FAILED;
	int ret = 0;

	if (i->dft->lldd_I_T_nexus_reset)
		res = i->dft->lldd_I_T_nexus_reset(dev);

	if (res != TMF_RESP_FUNC_COMPLETE) {
		SAS_DPRINTK("%s: Unable to reset I T nexus?\n", __func__);
		ret = -EAGAIN;
	}

	switch (dev->sata_dev.command_set) {
		case ATA_COMMAND_SET:
			SAS_DPRINTK("%s: Found ATA device.\n", __func__);
			*class = ATA_DEV_ATA;
			break;
		case ATAPI_COMMAND_SET:
			SAS_DPRINTK("%s: Found ATAPI device.\n", __func__);
			*class = ATA_DEV_ATAPI;
			break;
		default:
			SAS_DPRINTK("%s: Unknown SATA command set: %d.\n",
				    __func__,
				    dev->sata_dev.command_set);
			*class = ATA_DEV_UNKNOWN;
			break;
	}

	ap->cbl = ATA_CBL_SATA;
	return ret;
}

static int sas_ata_soft_reset(struct ata_link *link, unsigned int *class,
			       unsigned long deadline)
{
	struct ata_port *ap = link->ap;
	struct domain_device *dev = ap->private_data;
	struct sas_internal *i =
		to_sas_internal(dev->port->ha->core.shost->transportt);
	int res = TMF_RESP_FUNC_FAILED;
	int ret = 0;

	if (i->dft->lldd_ata_soft_reset)
		res = i->dft->lldd_ata_soft_reset(dev);

	if (res != TMF_RESP_FUNC_COMPLETE) {
		SAS_DPRINTK("%s: Unable to soft reset\n", __func__);
		ret = -EAGAIN;
	}

	switch (dev->sata_dev.command_set) {
	case ATA_COMMAND_SET:
		SAS_DPRINTK("%s: Found ATA device.\n", __func__);
		*class = ATA_DEV_ATA;
		break;
	case ATAPI_COMMAND_SET:
		SAS_DPRINTK("%s: Found ATAPI device.\n", __func__);
		*class = ATA_DEV_ATAPI;
		break;
	default:
		SAS_DPRINTK("%s: Unknown SATA command set: %d.\n",
			    __func__, dev->sata_dev.command_set);
		*class = ATA_DEV_UNKNOWN;
		break;
=======
=======
>>>>>>> refs/remotes/origin/master
	ata_tf_from_fis(dev->sata_dev.fis, &qc->result_tf);
	return true;
}

static struct sas_internal *dev_to_sas_internal(struct domain_device *dev)
{
	return to_sas_internal(dev->port->ha->core.shost->transportt);
}

static void sas_get_ata_command_set(struct domain_device *dev);

int sas_get_ata_info(struct domain_device *dev, struct ex_phy *phy)
{
	if (phy->attached_tproto & SAS_PROTOCOL_STP)
		dev->tproto = phy->attached_tproto;
	if (phy->attached_sata_dev)
<<<<<<< HEAD
		dev->tproto |= SATA_DEV;

	if (phy->attached_dev_type == SATA_PENDING)
		dev->dev_type = SATA_PENDING;
	else {
		int res;

		dev->dev_type = SATA_DEV;
=======
		dev->tproto |= SAS_SATA_DEV;

	if (phy->attached_dev_type == SAS_SATA_PENDING)
		dev->dev_type = SAS_SATA_PENDING;
	else {
		int res;

		dev->dev_type = SAS_SATA_DEV;
>>>>>>> refs/remotes/origin/master
		res = sas_get_report_phy_sata(dev->parent, phy->phy_id,
					      &dev->sata_dev.rps_resp);
		if (res) {
			SAS_DPRINTK("report phy sata to %016llx:0x%x returned "
				    "0x%x\n", SAS_ADDR(dev->parent->sas_addr),
				    phy->phy_id, res);
			return res;
		}
		memcpy(dev->frame_rcvd, &dev->sata_dev.rps_resp.rps.fis,
		       sizeof(struct dev_to_host_fis));
		/* TODO switch to ata_dev_classify() */
		sas_get_ata_command_set(dev);
	}
	return 0;
}

static int sas_ata_clear_pending(struct domain_device *dev, struct ex_phy *phy)
{
	int res;

	/* we weren't pending, so successfully end the reset sequence now */
<<<<<<< HEAD
	if (dev->dev_type != SATA_PENDING)
=======
	if (dev->dev_type != SAS_SATA_PENDING)
>>>>>>> refs/remotes/origin/master
		return 1;

	/* hmmm, if this succeeds do we need to repost the domain_device to the
	 * lldd so it can pick up new parameters?
	 */
	res = sas_get_ata_info(dev, phy);
	if (res)
		return 0; /* retry */
	else
		return 1;
}

static int smp_ata_check_ready(struct ata_link *link)
{
	int res;
	struct ata_port *ap = link->ap;
	struct domain_device *dev = ap->private_data;
	struct domain_device *ex_dev = dev->parent;
	struct sas_phy *phy = sas_get_local_phy(dev);
	struct ex_phy *ex_phy = &ex_dev->ex_dev.ex_phy[phy->number];

	res = sas_ex_phy_discover(ex_dev, phy->number);
	sas_put_local_phy(phy);

	/* break the wait early if the expander is unreachable,
	 * otherwise keep polling
	 */
	if (res == -ECOMM)
		return res;
	if (res != SMP_RESP_FUNC_ACC)
		return 0;

	switch (ex_phy->attached_dev_type) {
<<<<<<< HEAD
	case SATA_PENDING:
		return 0;
	case SAS_END_DEV:
=======
	case SAS_SATA_PENDING:
		return 0;
	case SAS_END_DEVICE:
>>>>>>> refs/remotes/origin/master
		if (ex_phy->attached_sata_dev)
			return sas_ata_clear_pending(dev, ex_phy);
	default:
		return -ENODEV;
	}
}

static int local_ata_check_ready(struct ata_link *link)
{
	struct ata_port *ap = link->ap;
	struct domain_device *dev = ap->private_data;
	struct sas_internal *i = dev_to_sas_internal(dev);

	if (i->dft->lldd_ata_check_ready)
		return i->dft->lldd_ata_check_ready(dev);
	else {
		/* lldd's that don't implement 'ready' checking get the
		 * old default behavior of not coordinating reset
		 * recovery with libata
		 */
		return 1;
	}
}

static int sas_ata_printk(const char *level, const struct domain_device *ddev,
			  const char *fmt, ...)
{
	struct ata_port *ap = ddev->sata_dev.ap;
	struct device *dev = &ddev->rphy->dev;
	struct va_format vaf;
	va_list args;
	int r;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	r = printk("%ssas: ata%u: %s: %pV",
		   level, ap->print_id, dev_name(dev), &vaf);

	va_end(args);

	return r;
}

static int sas_ata_hard_reset(struct ata_link *link, unsigned int *class,
			      unsigned long deadline)
{
	int ret = 0, res;
	struct sas_phy *phy;
	struct ata_port *ap = link->ap;
	int (*check_ready)(struct ata_link *link);
	struct domain_device *dev = ap->private_data;
	struct sas_internal *i = dev_to_sas_internal(dev);

	res = i->dft->lldd_I_T_nexus_reset(dev);
	if (res == -ENODEV)
		return res;

	if (res != TMF_RESP_FUNC_COMPLETE)
		sas_ata_printk(KERN_DEBUG, dev, "Unable to reset ata device?\n");

	phy = sas_get_local_phy(dev);
	if (scsi_is_sas_phy_local(phy))
		check_ready = local_ata_check_ready;
	else
		check_ready = smp_ata_check_ready;
	sas_put_local_phy(phy);

	ret = ata_wait_after_reset(link, deadline, check_ready);
	if (ret && ret != -EAGAIN)
		sas_ata_printk(KERN_ERR, dev, "reset failed (errno=%d)\n", ret);

	/* XXX: if the class changes during the reset the upper layer
	 * should be informed, if the device has gone away we assume
	 * libsas will eventually delete it
	 */
	switch (dev->sata_dev.command_set) {
	case ATA_COMMAND_SET:
		*class = ATA_DEV_ATA;
		break;
	case ATAPI_COMMAND_SET:
		*class = ATA_DEV_ATAPI;
		break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	ap->cbl = ATA_CBL_SATA;
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * notify the lldd to forget the sas_task for this internal ata command
 * that bypasses scsi-eh
 */
static void sas_ata_internal_abort(struct sas_task *task)
{
	struct sas_internal *si = dev_to_sas_internal(task->dev);
	unsigned long flags;
	int res;

	spin_lock_irqsave(&task->task_state_lock, flags);
	if (task->task_state_flags & SAS_TASK_STATE_ABORTED ||
	    task->task_state_flags & SAS_TASK_STATE_DONE) {
		spin_unlock_irqrestore(&task->task_state_lock, flags);
		SAS_DPRINTK("%s: Task %p already finished.\n", __func__,
			    task);
		goto out;
	}
	task->task_state_flags |= SAS_TASK_STATE_ABORTED;
	spin_unlock_irqrestore(&task->task_state_lock, flags);

	res = si->dft->lldd_abort_task(task);

	spin_lock_irqsave(&task->task_state_lock, flags);
	if (task->task_state_flags & SAS_TASK_STATE_DONE ||
	    res == TMF_RESP_FUNC_COMPLETE) {
		spin_unlock_irqrestore(&task->task_state_lock, flags);
		goto out;
	}

	/* XXX we are not prepared to deal with ->lldd_abort_task()
	 * failures.  TODO: lldds need to unconditionally forget about
	 * aborted ata tasks, otherwise we (likely) leak the sas task
	 * here
	 */
	SAS_DPRINTK("%s: Task %p leaked.\n", __func__, task);

	if (!(task->task_state_flags & SAS_TASK_STATE_DONE))
		task->task_state_flags &= ~SAS_TASK_STATE_ABORTED;
	spin_unlock_irqrestore(&task->task_state_lock, flags);

	return;
 out:
	list_del_init(&task->list);
	sas_free_task(task);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void sas_ata_post_internal(struct ata_queued_cmd *qc)
{
	if (qc->flags & ATA_QCFLAG_FAILED)
		qc->err_mask |= AC_ERR_OTHER;

	if (qc->err_mask) {
		/*
<<<<<<< HEAD
<<<<<<< HEAD
		 * Find the sas_task and kill it.  By this point,
		 * libata has decided to kill the qc, so we needn't
		 * bother with sas_ata_task_done.  But we still
		 * ought to abort the task.
		 */
		struct sas_task *task = qc->lldd_task;
		unsigned long flags;

		qc->lldd_task = NULL;
		if (task) {
			/* Should this be a AT(API) device reset? */
			spin_lock_irqsave(&task->task_state_lock, flags);
			task->task_state_flags |= SAS_TASK_NEED_DEV_RESET;
			spin_unlock_irqrestore(&task->task_state_lock, flags);

			task->uldd_task = NULL;
			__sas_task_abort(task);
		}
	}
}

static struct ata_port_operations sas_sata_ops = {
	.prereset		= ata_std_prereset,
	.softreset		= sas_ata_soft_reset,
=======
=======
>>>>>>> refs/remotes/origin/master
		 * Find the sas_task and kill it.  By this point, libata
		 * has decided to kill the qc and has frozen the port.
		 * In this state sas_ata_task_done() will no longer free
		 * the sas_task, so we need to notify the lldd (via
		 * ->lldd_abort_task) that the task is dead and free it
		 *  ourselves.
		 */
		struct sas_task *task = qc->lldd_task;

		qc->lldd_task = NULL;
		if (!task)
			return;
		task->uldd_task = NULL;
		sas_ata_internal_abort(task);
	}
}


static void sas_ata_set_dmamode(struct ata_port *ap, struct ata_device *ata_dev)
{
	struct domain_device *dev = ap->private_data;
	struct sas_internal *i = dev_to_sas_internal(dev);

	if (i->dft->lldd_ata_set_dmamode)
		i->dft->lldd_ata_set_dmamode(dev);
}

<<<<<<< HEAD
static struct ata_port_operations sas_sata_ops = {
	.prereset		= ata_std_prereset,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void sas_ata_sched_eh(struct ata_port *ap)
{
	struct domain_device *dev = ap->private_data;
	struct sas_ha_struct *ha = dev->port->ha;
	unsigned long flags;

	spin_lock_irqsave(&ha->lock, flags);
	if (!test_and_set_bit(SAS_DEV_EH_PENDING, &dev->state))
		ha->eh_active++;
	ata_std_sched_eh(ap);
	spin_unlock_irqrestore(&ha->lock, flags);
}

void sas_ata_end_eh(struct ata_port *ap)
{
	struct domain_device *dev = ap->private_data;
	struct sas_ha_struct *ha = dev->port->ha;
	unsigned long flags;

	spin_lock_irqsave(&ha->lock, flags);
	if (test_and_clear_bit(SAS_DEV_EH_PENDING, &dev->state))
		ha->eh_active--;
	spin_unlock_irqrestore(&ha->lock, flags);
}

static struct ata_port_operations sas_sata_ops = {
	.prereset		= ata_std_prereset,
>>>>>>> refs/remotes/origin/master
	.hardreset		= sas_ata_hard_reset,
	.postreset		= ata_std_postreset,
	.error_handler		= ata_std_error_handler,
	.post_internal_cmd	= sas_ata_post_internal,
	.qc_defer               = ata_std_qc_defer,
	.qc_prep		= ata_noop_qc_prep,
	.qc_issue		= sas_ata_qc_issue,
	.qc_fill_rtf		= sas_ata_qc_fill_rtf,
	.port_start		= ata_sas_port_start,
	.port_stop		= ata_sas_port_stop,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.set_dmamode		= sas_ata_set_dmamode,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.set_dmamode		= sas_ata_set_dmamode,
	.sched_eh		= sas_ata_sched_eh,
	.end_eh			= sas_ata_end_eh,
>>>>>>> refs/remotes/origin/master
};

static struct ata_port_info sata_port_info = {
	.flags = ATA_FLAG_SATA | ATA_FLAG_PIO_DMA | ATA_FLAG_NCQ,
	.pio_mask = ATA_PIO4,
	.mwdma_mask = ATA_MWDMA2,
	.udma_mask = ATA_UDMA6,
	.port_ops = &sas_sata_ops
};

<<<<<<< HEAD
<<<<<<< HEAD
int sas_ata_init_host_and_port(struct domain_device *found_dev,
			       struct scsi_target *starget)
{
	struct Scsi_Host *shost = dev_to_shost(&starget->dev);
	struct sas_ha_struct *ha = SHOST_TO_SAS_HA(shost);
	struct ata_port *ap;
=======
=======
>>>>>>> refs/remotes/origin/master
int sas_ata_init(struct domain_device *found_dev)
{
	struct sas_ha_struct *ha = found_dev->port->ha;
	struct Scsi_Host *shost = ha->core.shost;
	struct ata_port *ap;
	int rc;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	ata_host_init(&found_dev->sata_dev.ata_host,
		      ha->dev,
		      sata_port_info.flags,
		      &sas_sata_ops);
=======

	ata_host_init(&found_dev->sata_dev.ata_host, ha->dev, &sas_sata_ops);
>>>>>>> refs/remotes/origin/master
	ap = ata_sas_port_alloc(&found_dev->sata_dev.ata_host,
				&sata_port_info,
				shost);
	if (!ap) {
		SAS_DPRINTK("ata_sas_port_alloc failed.\n");
		return -ENODEV;
	}

	ap->private_data = found_dev;
	ap->cbl = ATA_CBL_SATA;
	ap->scsi_host = shost;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	rc = ata_sas_port_init(ap);
	if (rc) {
		ata_sas_port_destroy(ap);
		return rc;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	found_dev->sata_dev.ap = ap;

	return 0;
}

void sas_ata_task_abort(struct sas_task *task)
{
	struct ata_queued_cmd *qc = task->uldd_task;
	struct completion *waiting;

	/* Bounce SCSI-initiated commands to the SCSI EH */
	if (qc->scsicmd) {
		struct request_queue *q = qc->scsicmd->device->request_queue;
		unsigned long flags;

		spin_lock_irqsave(q->queue_lock, flags);
		blk_abort_request(qc->scsicmd->request);
		spin_unlock_irqrestore(q->queue_lock, flags);
<<<<<<< HEAD
		scsi_schedule_eh(qc->scsicmd->device->host);
=======
>>>>>>> refs/remotes/origin/master
		return;
	}

	/* Internal command, fake a timeout and complete. */
	qc->flags &= ~ATA_QCFLAG_ACTIVE;
	qc->flags |= ATA_QCFLAG_FAILED;
	qc->err_mask |= AC_ERR_TIMEOUT;
	waiting = qc->private_data;
	complete(waiting);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void sas_task_timedout(unsigned long _task)
{
	struct sas_task *task = (void *) _task;
	unsigned long flags;

	spin_lock_irqsave(&task->task_state_lock, flags);
	if (!(task->task_state_flags & SAS_TASK_STATE_DONE))
		task->task_state_flags |= SAS_TASK_STATE_ABORTED;
	spin_unlock_irqrestore(&task->task_state_lock, flags);

	complete(&task->completion);
}

static void sas_disc_task_done(struct sas_task *task)
{
	if (!del_timer(&task->timer))
		return;
	complete(&task->completion);
}

#define SAS_DEV_TIMEOUT 10

/**
 * sas_execute_task -- Basic task processing for discovery
 * @task: the task to be executed
 * @buffer: pointer to buffer to do I/O
 * @size: size of @buffer
 * @dma_dir: DMA direction.  DMA_xxx
 */
static int sas_execute_task(struct sas_task *task, void *buffer, int size,
			    enum dma_data_direction dma_dir)
{
	int res = 0;
	struct scatterlist *scatter = NULL;
	struct task_status_struct *ts = &task->task_status;
	int num_scatter = 0;
	int retries = 0;
	struct sas_internal *i =
		to_sas_internal(task->dev->port->ha->core.shost->transportt);

	if (dma_dir != DMA_NONE) {
		scatter = kzalloc(sizeof(*scatter), GFP_KERNEL);
		if (!scatter)
			goto out;

		sg_init_one(scatter, buffer, size);
		num_scatter = 1;
	}

	task->task_proto = task->dev->tproto;
	task->scatter = scatter;
	task->num_scatter = num_scatter;
	task->total_xfer_len = size;
	task->data_dir = dma_dir;
	task->task_done = sas_disc_task_done;
	if (dma_dir != DMA_NONE &&
	    sas_protocol_ata(task->task_proto)) {
		task->num_scatter = dma_map_sg(task->dev->port->ha->dev,
					       task->scatter,
					       task->num_scatter,
					       task->data_dir);
	}

	for (retries = 0; retries < 5; retries++) {
		task->task_state_flags = SAS_TASK_STATE_PENDING;
		init_completion(&task->completion);

		task->timer.data = (unsigned long) task;
		task->timer.function = sas_task_timedout;
		task->timer.expires = jiffies + SAS_DEV_TIMEOUT*HZ;
		add_timer(&task->timer);

		res = i->dft->lldd_execute_task(task, 1, GFP_KERNEL);
		if (res) {
			del_timer(&task->timer);
			SAS_DPRINTK("executing SAS discovery task failed:%d\n",
				    res);
			goto ex_err;
		}
		wait_for_completion(&task->completion);
		res = -ECOMM;
		if (task->task_state_flags & SAS_TASK_STATE_ABORTED) {
			int res2;
			SAS_DPRINTK("task aborted, flags:0x%x\n",
				    task->task_state_flags);
			res2 = i->dft->lldd_abort_task(task);
			SAS_DPRINTK("came back from abort task\n");
			if (!(task->task_state_flags & SAS_TASK_STATE_DONE)) {
				if (res2 == TMF_RESP_FUNC_COMPLETE)
					continue; /* Retry the task */
				else
					goto ex_err;
			}
		}
		if (task->task_status.stat == SAM_STAT_BUSY ||
			   task->task_status.stat == SAM_STAT_TASK_SET_FULL ||
			   task->task_status.stat == SAS_QUEUE_FULL) {
			SAS_DPRINTK("task: q busy, sleeping...\n");
			schedule_timeout_interruptible(HZ);
		} else if (task->task_status.stat == SAM_STAT_CHECK_CONDITION) {
			struct scsi_sense_hdr shdr;

			if (!scsi_normalize_sense(ts->buf, ts->buf_valid_size,
						  &shdr)) {
				SAS_DPRINTK("couldn't normalize sense\n");
				continue;
			}
			if ((shdr.sense_key == 6 && shdr.asc == 0x29) ||
			    (shdr.sense_key == 2 && shdr.asc == 4 &&
			     shdr.ascq == 1)) {
				SAS_DPRINTK("device %016llx LUN: %016llx "
					    "powering up or not ready yet, "
					    "sleeping...\n",
					    SAS_ADDR(task->dev->sas_addr),
					    SAS_ADDR(task->ssp_task.LUN));

				schedule_timeout_interruptible(5*HZ);
			} else if (shdr.sense_key == 1) {
				res = 0;
				break;
			} else if (shdr.sense_key == 5) {
				break;
			} else {
				SAS_DPRINTK("dev %016llx LUN: %016llx "
					    "sense key:0x%x ASC:0x%x ASCQ:0x%x"
					    "\n",
					    SAS_ADDR(task->dev->sas_addr),
					    SAS_ADDR(task->ssp_task.LUN),
					    shdr.sense_key,
					    shdr.asc, shdr.ascq);
			}
		} else if (task->task_status.resp != SAS_TASK_COMPLETE ||
			   task->task_status.stat != SAM_STAT_GOOD) {
			SAS_DPRINTK("task finished with resp:0x%x, "
				    "stat:0x%x\n",
				    task->task_status.resp,
				    task->task_status.stat);
			goto ex_err;
		} else {
			res = 0;
			break;
		}
	}
ex_err:
	if (dma_dir != DMA_NONE) {
		if (sas_protocol_ata(task->task_proto))
			dma_unmap_sg(task->dev->port->ha->dev,
				     task->scatter, task->num_scatter,
				     task->data_dir);
		kfree(scatter);
	}
out:
	return res;
}

/* ---------- SATA ---------- */

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void sas_get_ata_command_set(struct domain_device *dev)
{
	struct dev_to_host_fis *fis =
		(struct dev_to_host_fis *) dev->frame_rcvd;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (dev->dev_type == SATA_PENDING)
		return;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (dev->dev_type == SAS_SATA_PENDING)
		return;

>>>>>>> refs/remotes/origin/master
	if ((fis->sector_count == 1 && /* ATA */
	     fis->lbal         == 1 &&
	     fis->lbam         == 0 &&
	     fis->lbah         == 0 &&
	     fis->device       == 0)
	    ||
	    (fis->sector_count == 0 && /* CE-ATA (mATA) */
	     fis->lbal         == 0 &&
	     fis->lbam         == 0xCE &&
	     fis->lbah         == 0xAA &&
	     (fis->device & ~0x10) == 0))

		dev->sata_dev.command_set = ATA_COMMAND_SET;

	else if ((fis->interrupt_reason == 1 &&	/* ATAPI */
		  fis->lbal             == 1 &&
		  fis->byte_count_low   == 0x14 &&
		  fis->byte_count_high  == 0xEB &&
		  (fis->device & ~0x10) == 0))

		dev->sata_dev.command_set = ATAPI_COMMAND_SET;

	else if ((fis->sector_count == 1 && /* SEMB */
		  fis->lbal         == 1 &&
		  fis->lbam         == 0x3C &&
		  fis->lbah         == 0xC3 &&
		  fis->device       == 0)
		||
		 (fis->interrupt_reason == 1 &&	/* SATA PM */
		  fis->lbal             == 1 &&
		  fis->byte_count_low   == 0x69 &&
		  fis->byte_count_high  == 0x96 &&
		  (fis->device & ~0x10) == 0))

		/* Treat it as a superset? */
		dev->sata_dev.command_set = ATAPI_COMMAND_SET;
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * sas_issue_ata_cmd -- Basic SATA command processing for discovery
 * @dev: the device to send the command to
 * @command: the command register
 * @features: the features register
 * @buffer: pointer to buffer to do I/O
 * @size: size of @buffer
 * @dma_dir: DMA direction.  DMA_xxx
 */
static int sas_issue_ata_cmd(struct domain_device *dev, u8 command,
			     u8 features, void *buffer, int size,
			     enum dma_data_direction dma_dir)
{
	int res = 0;
	struct sas_task *task;
	struct dev_to_host_fis *d2h_fis = (struct dev_to_host_fis *)
		&dev->frame_rcvd[0];

	res = -ENOMEM;
	task = sas_alloc_task(GFP_KERNEL);
	if (!task)
		goto out;

	task->dev = dev;

	task->ata_task.fis.fis_type = 0x27;
	task->ata_task.fis.command = command;
	task->ata_task.fis.features = features;
	task->ata_task.fis.device = d2h_fis->device;
	task->ata_task.retry_count = 1;

	res = sas_execute_task(task, buffer, size, dma_dir);

	sas_free_task(task);
out:
	return res;
}

#define ATA_IDENTIFY_DEV         0xEC
#define ATA_IDENTIFY_PACKET_DEV  0xA1
#define ATA_SET_FEATURES         0xEF
#define ATA_FEATURE_PUP_STBY_SPIN_UP 0x07

/**
 * sas_discover_sata_dev -- discover a STP/SATA device (SATA_DEV)
 * @dev: STP/SATA device of interest (ATA/ATAPI)
 *
 * The LLDD has already been notified of this device, so that we can
 * send FISes to it.  Here we try to get IDENTIFY DEVICE or IDENTIFY
 * PACKET DEVICE, if ATAPI device, so that the LLDD can fine-tune its
 * performance for this device.
 */
static int sas_discover_sata_dev(struct domain_device *dev)
{
	int     res;
	__le16  *identify_x;
	u8      command;

	identify_x = kzalloc(512, GFP_KERNEL);
	if (!identify_x)
		return -ENOMEM;

	if (dev->sata_dev.command_set == ATA_COMMAND_SET) {
		dev->sata_dev.identify_device = identify_x;
		command = ATA_IDENTIFY_DEV;
	} else {
		dev->sata_dev.identify_packet_device = identify_x;
		command = ATA_IDENTIFY_PACKET_DEV;
	}

	res = sas_issue_ata_cmd(dev, command, 0, identify_x, 512,
				DMA_FROM_DEVICE);
	if (res)
		goto out_err;

	/* lives on the media? */
	if (le16_to_cpu(identify_x[0]) & 4) {
		/* incomplete response */
		SAS_DPRINTK("sending SET FEATURE/PUP_STBY_SPIN_UP to "
			    "dev %llx\n", SAS_ADDR(dev->sas_addr));
		if (!(identify_x[83] & cpu_to_le16(1<<6)))
			goto cont1;
		res = sas_issue_ata_cmd(dev, ATA_SET_FEATURES,
					ATA_FEATURE_PUP_STBY_SPIN_UP,
					NULL, 0, DMA_NONE);
		if (res)
			goto cont1;

		schedule_timeout_interruptible(5*HZ); /* More time? */
		res = sas_issue_ata_cmd(dev, command, 0, identify_x, 512,
					DMA_FROM_DEVICE);
		if (res)
			goto out_err;
	}
cont1:
	/* XXX Hint: register this SATA device with SATL.
	   When this returns, dev->sata_dev->lu is alive and
	   present.
	sas_satl_register_dev(dev);
	*/

	sas_fill_in_rphy(dev, dev->rphy);

	return 0;
out_err:
	dev->sata_dev.identify_packet_device = NULL;
	dev->sata_dev.identify_device = NULL;
	kfree(identify_x);
	return res;
}

static int sas_discover_sata_pm(struct domain_device *dev)
{
	return -ENODEV;
=======
void sas_probe_sata(struct asd_sas_port *port)
{
	struct domain_device *dev, *n;

	mutex_lock(&port->ha->disco_mutex);
	list_for_each_entry(dev, &port->disco_list, disco_list_node) {
		if (!dev_is_sata(dev))
			continue;

		ata_sas_async_probe(dev->sata_dev.ap);
	}
	mutex_unlock(&port->ha->disco_mutex);

	list_for_each_entry_safe(dev, n, &port->disco_list, disco_list_node) {
		if (!dev_is_sata(dev))
			continue;

		sas_ata_wait_eh(dev);

		/* if libata could not bring the link up, don't surface
		 * the device
		 */
		if (ata_dev_disabled(sas_to_ata_dev(dev)))
			sas_fail_probe(dev, __func__, -ENODEV);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
void sas_probe_sata(struct asd_sas_port *port)
{
	struct domain_device *dev, *n;

	mutex_lock(&port->ha->disco_mutex);
	list_for_each_entry(dev, &port->disco_list, disco_list_node) {
		if (!dev_is_sata(dev))
			continue;

		ata_sas_async_probe(dev->sata_dev.ap);
	}
	mutex_unlock(&port->ha->disco_mutex);

	list_for_each_entry_safe(dev, n, &port->disco_list, disco_list_node) {
		if (!dev_is_sata(dev))
			continue;

		sas_ata_wait_eh(dev);

		/* if libata could not bring the link up, don't surface
		 * the device
		 */
		if (ata_dev_disabled(sas_to_ata_dev(dev)))
			sas_fail_probe(dev, __func__, -ENODEV);
	}

}

static bool sas_ata_flush_pm_eh(struct asd_sas_port *port, const char *func)
{
	struct domain_device *dev, *n;
	bool retry = false;

	list_for_each_entry_safe(dev, n, &port->dev_list, dev_list_node) {
		int rc;

		if (!dev_is_sata(dev))
			continue;

		sas_ata_wait_eh(dev);
		rc = dev->sata_dev.pm_result;
		if (rc == -EAGAIN)
			retry = true;
		else if (rc) {
			/* since we don't have a
			 * ->port_{suspend|resume} routine in our
			 *  ata_port ops, and no entanglements with
			 *  acpi, suspend should just be mechanical trip
			 *  through eh, catch cases where these
			 *  assumptions are invalidated
			 */
			WARN_ONCE(1, "failed %s %s error: %d\n", func,
				 dev_name(&dev->rphy->dev), rc);
		}

		/* if libata failed to power manage the device, tear it down */
		if (ata_dev_disabled(sas_to_ata_dev(dev)))
			sas_fail_probe(dev, func, -ENODEV);
	}

	return retry;
}

void sas_suspend_sata(struct asd_sas_port *port)
{
	struct domain_device *dev;

 retry:
	mutex_lock(&port->ha->disco_mutex);
	list_for_each_entry(dev, &port->dev_list, dev_list_node) {
		struct sata_device *sata;

		if (!dev_is_sata(dev))
			continue;

		sata = &dev->sata_dev;
		if (sata->ap->pm_mesg.event == PM_EVENT_SUSPEND)
			continue;

		sata->pm_result = -EIO;
		ata_sas_port_async_suspend(sata->ap, &sata->pm_result);
	}
	mutex_unlock(&port->ha->disco_mutex);

	if (sas_ata_flush_pm_eh(port, __func__))
		goto retry;
}

void sas_resume_sata(struct asd_sas_port *port)
{
	struct domain_device *dev;

 retry:
	mutex_lock(&port->ha->disco_mutex);
	list_for_each_entry(dev, &port->dev_list, dev_list_node) {
		struct sata_device *sata;

		if (!dev_is_sata(dev))
			continue;

		sata = &dev->sata_dev;
		if (sata->ap->pm_mesg.event == PM_EVENT_ON)
			continue;

		sata->pm_result = -EIO;
		ata_sas_port_async_resume(sata->ap, &sata->pm_result);
	}
	mutex_unlock(&port->ha->disco_mutex);

	if (sas_ata_flush_pm_eh(port, __func__))
		goto retry;
>>>>>>> refs/remotes/origin/master
}

/**
 * sas_discover_sata -- discover an STP/SATA domain device
 * @dev: pointer to struct domain_device of interest
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * First we notify the LLDD of this device, so we can send frames to
 * it.  Then depending on the type of device we call the appropriate
 * discover functions.  Once device discover is done, we notify the
 * LLDD so that it can fine-tune its parameters for the device, by
 * removing it and then adding it.  That is, the second time around,
 * the driver would have certain fields, that it is looking at, set.
 * Finally we initialize the kobj so that the device can be added to
 * the system at registration time.  Devices directly attached to a HA
 * port, have no parents.  All other devices do, and should have their
 * "parent" pointer set appropriately before calling this function.
=======
 * Devices directly attached to a HA port, have no parents.  All other
 * devices do, and should have their "parent" pointer set appropriately
 * before calling this function.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Devices directly attached to a HA port, have no parents.  All other
 * devices do, and should have their "parent" pointer set appropriately
 * before calling this function.
>>>>>>> refs/remotes/origin/master
 */
int sas_discover_sata(struct domain_device *dev)
{
	int res;

<<<<<<< HEAD
<<<<<<< HEAD
	sas_get_ata_command_set(dev);
=======
	if (dev->dev_type == SATA_PM)
=======
	if (dev->dev_type == SAS_SATA_PM)
>>>>>>> refs/remotes/origin/master
		return -ENODEV;

	sas_get_ata_command_set(dev);
	sas_fill_in_rphy(dev, dev->rphy);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	res = sas_notify_lldd_dev_found(dev);
	if (res)
		return res;

<<<<<<< HEAD
<<<<<<< HEAD
	switch (dev->dev_type) {
	case SATA_DEV:
		res = sas_discover_sata_dev(dev);
		break;
	case SATA_PM:
		res = sas_discover_sata_pm(dev);
		break;
	default:
		break;
	}
	sas_notify_lldd_dev_gone(dev);
	if (!res) {
		sas_notify_lldd_dev_found(dev);
		res = sas_rphy_add(dev->rphy);
	}

	return res;
=======
=======
>>>>>>> refs/remotes/origin/master
	sas_discover_event(dev->port, DISCE_PROBE);
	return 0;
}

static void async_sas_ata_eh(void *data, async_cookie_t cookie)
{
	struct domain_device *dev = data;
	struct ata_port *ap = dev->sata_dev.ap;
	struct sas_ha_struct *ha = dev->port->ha;

<<<<<<< HEAD
	/* hold a reference over eh since we may be racing with final
	 * remove once all commands are completed
	 */
	kref_get(&dev->kref);
	sas_ata_printk(KERN_DEBUG, dev, "dev error handler\n");
	ata_scsi_port_error_handler(ha->core.shost, ap);
	sas_put_device(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sas_ata_printk(KERN_DEBUG, dev, "dev error handler\n");
	ata_scsi_port_error_handler(ha->core.shost, ap);
	sas_put_device(dev);
>>>>>>> refs/remotes/origin/master
}

void sas_ata_strategy_handler(struct Scsi_Host *shost)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct scsi_device *sdev;

	shost_for_each_device(sdev, shost) {
		struct domain_device *ddev = sdev_to_domain_dev(sdev);
		struct ata_port *ap = ddev->sata_dev.ap;

		if (!dev_is_sata(ddev))
			continue;

		ata_port_printk(ap, KERN_DEBUG, "sas eh calling libata port error handler");
		ata_scsi_port_error_handler(shost, ap);
	}
}

int sas_ata_timed_out(struct scsi_cmnd *cmd, struct sas_task *task,
		      enum blk_eh_timer_return *rtn)
{
	struct domain_device *ddev = cmd_to_domain_dev(cmd);

	if (!dev_is_sata(ddev) || task)
		return 0;

	/* we're a sata device with no task, so this must be a libata
	 * eh timeout.  Ideally should hook into libata timeout
	 * handling, but there's no point, it just wants to activate
	 * the eh thread */
	*rtn = BLK_EH_NOT_HANDLED;
	return 1;
}

int sas_ata_eh(struct Scsi_Host *shost, struct list_head *work_q,
	       struct list_head *done_q)
{
	int rtn = 0;
	struct scsi_cmnd *cmd, *n;
	struct ata_port *ap;

	do {
		LIST_HEAD(sata_q);

		ap = NULL;
=======
	struct sas_ha_struct *sas_ha = SHOST_TO_SAS_HA(shost);
	LIST_HEAD(async);
=======
	struct sas_ha_struct *sas_ha = SHOST_TO_SAS_HA(shost);
	ASYNC_DOMAIN_EXCLUSIVE(async);
>>>>>>> refs/remotes/origin/master
	int i;

	/* it's ok to defer revalidation events during ata eh, these
	 * disks are in one of three states:
	 * 1/ present for initial domain discovery, and these
	 *    resets will cause bcn flutters
	 * 2/ hot removed, we'll discover that after eh fails
	 * 3/ hot added after initial discovery, lost the race, and need
	 *    to catch the next train.
	 */
	sas_disable_revalidation(sas_ha);

	spin_lock_irq(&sas_ha->phy_port_lock);
	for (i = 0; i < sas_ha->num_phys; i++) {
		struct asd_sas_port *port = sas_ha->sas_port[i];
		struct domain_device *dev;

		spin_lock(&port->dev_list_lock);
		list_for_each_entry(dev, &port->dev_list, dev_list_node) {
			if (!dev_is_sata(dev))
				continue;
<<<<<<< HEAD
=======

			/* hold a reference over eh since we may be
			 * racing with final remove once all commands
			 * are completed
			 */
			kref_get(&dev->kref);

>>>>>>> refs/remotes/origin/master
			async_schedule_domain(async_sas_ata_eh, dev, &async);
		}
		spin_unlock(&port->dev_list_lock);
	}
	spin_unlock_irq(&sas_ha->phy_port_lock);

	async_synchronize_full_domain(&async);

	sas_enable_revalidation(sas_ha);
}

void sas_ata_eh(struct Scsi_Host *shost, struct list_head *work_q,
		struct list_head *done_q)
{
	struct scsi_cmnd *cmd, *n;
	struct domain_device *eh_dev;

	do {
		LIST_HEAD(sata_q);
		eh_dev = NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		list_for_each_entry_safe(cmd, n, work_q, eh_entry) {
			struct domain_device *ddev = cmd_to_domain_dev(cmd);

			if (!dev_is_sata(ddev) || TO_SAS_TASK(cmd))
				continue;
<<<<<<< HEAD
<<<<<<< HEAD
			if (ap && ap != ddev->sata_dev.ap)
				continue;
			ap = ddev->sata_dev.ap;
			rtn = 1;
=======
			if (eh_dev && eh_dev != ddev)
				continue;
			eh_dev = ddev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (eh_dev && eh_dev != ddev)
				continue;
			eh_dev = ddev;
>>>>>>> refs/remotes/origin/master
			list_move(&cmd->eh_entry, &sata_q);
		}

		if (!list_empty(&sata_q)) {
<<<<<<< HEAD
<<<<<<< HEAD
			ata_port_printk(ap, KERN_DEBUG, "sas eh calling libata cmd error handler\n");
=======
			struct ata_port *ap = eh_dev->sata_dev.ap;

			sas_ata_printk(KERN_DEBUG, eh_dev, "cmd error handler\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
			struct ata_port *ap = eh_dev->sata_dev.ap;

			sas_ata_printk(KERN_DEBUG, eh_dev, "cmd error handler\n");
>>>>>>> refs/remotes/origin/master
			ata_scsi_cmd_error_handler(shost, ap, &sata_q);
			/*
			 * ata's error handler may leave the cmd on the list
			 * so make sure they don't remain on a stack list
			 * about to go out of scope.
			 *
			 * This looks strange, since the commands are
			 * now part of no list, but the next error
			 * action will be ata_port_error_handler()
			 * which takes no list and sweeps them up
			 * anyway from the ata tag array.
			 */
			while (!list_empty(&sata_q))
				list_del_init(sata_q.next);
		}
<<<<<<< HEAD
<<<<<<< HEAD
	} while (ap);

	return rtn;
=======
=======
>>>>>>> refs/remotes/origin/master
	} while (eh_dev);
}

void sas_ata_schedule_reset(struct domain_device *dev)
{
	struct ata_eh_info *ehi;
	struct ata_port *ap;
	unsigned long flags;

	if (!dev_is_sata(dev))
		return;

	ap = dev->sata_dev.ap;
	ehi = &ap->link.eh_info;

	spin_lock_irqsave(ap->lock, flags);
	ehi->err_mask |= AC_ERR_TIMEOUT;
	ehi->action |= ATA_EH_RESET;
	ata_port_schedule_eh(ap);
	spin_unlock_irqrestore(ap->lock, flags);
}
EXPORT_SYMBOL_GPL(sas_ata_schedule_reset);

void sas_ata_wait_eh(struct domain_device *dev)
{
	struct ata_port *ap;

	if (!dev_is_sata(dev))
		return;

	ap = dev->sata_dev.ap;
	ata_port_wait_eh(ap);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
