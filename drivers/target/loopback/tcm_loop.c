/*******************************************************************************
 *
 * This file contains the Linux/SCSI LLD virtual SCSI initiator driver
 * for emulated SAS initiator ports
 *
<<<<<<< HEAD
 * © Copyright 2011 RisingTide Systems LLC.
=======
 * © Copyright 2011-2013 Datera, Inc.
>>>>>>> refs/remotes/origin/master
 *
 * Licensed to the Linux Foundation under the General Public License (GPL) version 2.
 *
 * Author: Nicholas A. Bellinger <nab@risingtidesystems.com>
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
 ****************************************************************************/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/configfs.h>
#include <scsi/scsi.h>
#include <scsi/scsi_tcq.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_cmnd.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <scsi/scsi_tcq.h>

#include <target/target_core_base.h>
#include <target/target_core_transport.h>
#include <target/target_core_fabric_ops.h>
#include <target/target_core_fabric_configfs.h>
#include <target/target_core_fabric_lib.h>
#include <target/target_core_configfs.h>
#include <target/target_core_device.h>
#include <target/target_core_tpg.h>
#include <target/target_core_tmr.h>
=======
=======
>>>>>>> refs/remotes/origin/master

#include <target/target_core_base.h>
#include <target/target_core_fabric.h>
#include <target/target_core_fabric_configfs.h>
#include <target/target_core_configfs.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#include "tcm_loop.h"

#define to_tcm_loop_hba(hba)	container_of(hba, struct tcm_loop_hba, dev)

/* Local pointer to allocated TCM configfs fabric module */
static struct target_fabric_configfs *tcm_loop_fabric_configfs;

<<<<<<< HEAD
<<<<<<< HEAD
=======
static struct workqueue_struct *tcm_loop_workqueue;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct workqueue_struct *tcm_loop_workqueue;
>>>>>>> refs/remotes/origin/master
static struct kmem_cache *tcm_loop_cmd_cache;

static int tcm_loop_hba_no_cnt;

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Allocate a tcm_loop cmd descriptor from target_core_mod code
 *
 * Can be called from interrupt context in tcm_loop_queuecommand() below
 */
static struct se_cmd *tcm_loop_allocate_core_cmd(
	struct tcm_loop_hba *tl_hba,
	struct se_portal_group *se_tpg,
	struct scsi_cmnd *sc)
{
	struct se_cmd *se_cmd;
	struct se_session *se_sess;
	struct tcm_loop_nexus *tl_nexus = tl_hba->tl_nexus;
	struct tcm_loop_cmd *tl_cmd;
	int sam_task_attr;

	if (!tl_nexus) {
		scmd_printk(KERN_ERR, sc, "TCM_Loop I_T Nexus"
				" does not exist\n");
		set_host_byte(sc, DID_ERROR);
		return NULL;
	}
	se_sess = tl_nexus->se_sess;

	tl_cmd = kmem_cache_zalloc(tcm_loop_cmd_cache, GFP_ATOMIC);
	if (!tl_cmd) {
		printk(KERN_ERR "Unable to allocate struct tcm_loop_cmd\n");
		set_host_byte(sc, DID_ERROR);
		return NULL;
	}
	se_cmd = &tl_cmd->tl_se_cmd;
	/*
	 * Save the pointer to struct scsi_cmnd *sc
	 */
	tl_cmd->sc = sc;
	/*
	 * Locate the SAM Task Attr from struct scsi_cmnd *
	 */
	if (sc->device->tagged_supported) {
		switch (sc->tag) {
		case HEAD_OF_QUEUE_TAG:
			sam_task_attr = MSG_HEAD_TAG;
			break;
		case ORDERED_QUEUE_TAG:
			sam_task_attr = MSG_ORDERED_TAG;
			break;
		default:
			sam_task_attr = MSG_SIMPLE_TAG;
			break;
		}
	} else
		sam_task_attr = MSG_SIMPLE_TAG;

	/*
	 * Initialize struct se_cmd descriptor from target_core_mod infrastructure
	 */
	transport_init_se_cmd(se_cmd, se_tpg->se_tpg_tfo, se_sess,
			scsi_bufflen(sc), sc->sc_data_direction, sam_task_attr,
			&tl_cmd->tl_sense_buf[0]);

	/*
	 * Signal BIDI usage with T_TASK(cmd)->t_tasks_bidi
	 */
	if (scsi_bidi_cmnd(sc))
		T_TASK(se_cmd)->t_tasks_bidi = 1;
	/*
	 * Locate the struct se_lun pointer and attach it to struct se_cmd
	 */
	if (transport_get_lun_for_cmd(se_cmd, NULL, tl_cmd->sc->device->lun) < 0) {
		kmem_cache_free(tcm_loop_cmd_cache, tl_cmd);
		set_host_byte(sc, DID_NO_CONNECT);
		return NULL;
	}
	/*
	 * Because some userspace code via scsi-generic do not memset their
	 * associated read buffers, go ahead and do that here for type
	 * SCF_SCSI_CONTROL_SG_IO_CDB.  Also note that this is currently
	 * guaranteed to be a single SGL for SCF_SCSI_CONTROL_SG_IO_CDB
	 * by target core in transport_generic_allocate_tasks() ->
	 * transport_generic_cmd_sequencer().
	 */
	if (se_cmd->se_cmd_flags & SCF_SCSI_CONTROL_SG_IO_CDB &&
	    se_cmd->data_direction == DMA_FROM_DEVICE) {
		struct scatterlist *sg = scsi_sglist(sc);
		unsigned char *buf = kmap(sg_page(sg)) + sg->offset;

		if (buf != NULL) {
			memset(buf, 0, sg->length);
			kunmap(sg_page(sg));
		}
	}

	transport_device_setup_cmd(se_cmd);
	return se_cmd;
}

/*
 * Called by struct target_core_fabric_ops->new_cmd_map()
 *
 * Always called in process context.  A non zero return value
 * here will signal to handle an exception based on the return code.
 */
static int tcm_loop_new_cmd_map(struct se_cmd *se_cmd)
{
	struct tcm_loop_cmd *tl_cmd = container_of(se_cmd,
				struct tcm_loop_cmd, tl_se_cmd);
	struct scsi_cmnd *sc = tl_cmd->sc;
	void *mem_ptr, *mem_bidi_ptr = NULL;
	u32 sg_no_bidi = 0;
	int ret;
	/*
	 * Allocate the necessary tasks to complete the received CDB+data
	 */
	ret = transport_generic_allocate_tasks(se_cmd, tl_cmd->sc->cmnd);
	if (ret == -1) {
		/* Out of Resources */
		return PYX_TRANSPORT_LU_COMM_FAILURE;
	} else if (ret == -2) {
		/*
		 * Handle case for SAM_STAT_RESERVATION_CONFLICT
		 */
		if (se_cmd->se_cmd_flags & SCF_SCSI_RESERVATION_CONFLICT)
			return PYX_TRANSPORT_RESERVATION_CONFLICT;
		/*
		 * Otherwise, return SAM_STAT_CHECK_CONDITION and return
		 * sense data.
		 */
		return PYX_TRANSPORT_USE_SENSE_REASON;
	}
	/*
	 * Setup the struct scatterlist memory from the received
	 * struct scsi_cmnd.
	 */
	if (scsi_sg_count(sc)) {
		se_cmd->se_cmd_flags |= SCF_PASSTHROUGH_SG_TO_MEM;
		mem_ptr = (void *)scsi_sglist(sc);
		/*
		 * For BIDI commands, pass in the extra READ buffer
		 * to transport_generic_map_mem_to_cmd() below..
		 */
		if (T_TASK(se_cmd)->t_tasks_bidi) {
			struct scsi_data_buffer *sdb = scsi_in(sc);

			mem_bidi_ptr = (void *)sdb->table.sgl;
			sg_no_bidi = sdb->table.nents;
		}
	} else {
		/*
		 * Used for DMA_NONE
		 */
		mem_ptr = NULL;
	}
	/*
	 * Map the SG memory into struct se_mem->page linked list using the same
	 * physical memory at sg->page_link.
	 */
	ret = transport_generic_map_mem_to_cmd(se_cmd, mem_ptr,
			scsi_sg_count(sc), mem_bidi_ptr, sg_no_bidi);
	if (ret < 0)
		return PYX_TRANSPORT_LU_COMM_FAILURE;

	return 0;
}
=======
static int tcm_loop_queue_status(struct se_cmd *se_cmd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int tcm_loop_queue_status(struct se_cmd *se_cmd);
>>>>>>> refs/remotes/origin/master

/*
 * Called from struct target_core_fabric_ops->check_stop_free()
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void tcm_loop_check_stop_free(struct se_cmd *se_cmd)
=======
static int tcm_loop_check_stop_free(struct se_cmd *se_cmd)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int tcm_loop_check_stop_free(struct se_cmd *se_cmd)
>>>>>>> refs/remotes/origin/master
{
	/*
	 * Do not release struct se_cmd's containing a valid TMR
	 * pointer.  These will be released directly in tcm_loop_device_reset()
	 * with transport_generic_free_cmd().
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (se_cmd->se_tmr_req)
		return;
=======
	if (se_cmd->se_cmd_flags & SCF_SCSI_TMR_CDB)
		return 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (se_cmd->se_cmd_flags & SCF_SCSI_TMR_CDB)
		return 0;
>>>>>>> refs/remotes/origin/master
	/*
	 * Release the struct se_cmd, which will make a callback to release
	 * struct tcm_loop_cmd * in tcm_loop_deallocate_core_cmd()
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	transport_generic_free_cmd(se_cmd, 0, 1, 0);
}

/*
 * Called from struct target_core_fabric_ops->release_cmd_to_pool()
 */
static void tcm_loop_deallocate_core_cmd(struct se_cmd *se_cmd)
=======
=======
>>>>>>> refs/remotes/origin/master
	transport_generic_free_cmd(se_cmd, 0);
	return 1;
}

static void tcm_loop_release_cmd(struct se_cmd *se_cmd)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct tcm_loop_cmd *tl_cmd = container_of(se_cmd,
				struct tcm_loop_cmd, tl_se_cmd);

	kmem_cache_free(tcm_loop_cmd_cache, tl_cmd);
}

<<<<<<< HEAD
static int tcm_loop_proc_info(struct Scsi_Host *host, char *buffer,
				char **start, off_t offset,
				int length, int inout)
{
	return sprintf(buffer, "tcm_loop_proc_info()\n");
=======
static int tcm_loop_show_info(struct seq_file *m, struct Scsi_Host *host)
{
	seq_printf(m, "tcm_loop_proc_info()\n");
	return 0;
>>>>>>> refs/remotes/origin/master
}

static int tcm_loop_driver_probe(struct device *);
static int tcm_loop_driver_remove(struct device *);

static int pseudo_lld_bus_match(struct device *dev,
				struct device_driver *dev_driver)
{
	return 1;
}

static struct bus_type tcm_loop_lld_bus = {
	.name			= "tcm_loop_bus",
	.match			= pseudo_lld_bus_match,
	.probe			= tcm_loop_driver_probe,
	.remove			= tcm_loop_driver_remove,
};

static struct device_driver tcm_loop_driverfs = {
	.name			= "tcm_loop",
	.bus			= &tcm_loop_lld_bus,
};
/*
 * Used with root_device_register() in tcm_loop_alloc_core_bus() below
 */
struct device *tcm_loop_primary;

/*
 * Copied from drivers/scsi/libfc/fc_fcp.c:fc_change_queue_depth() and
 * drivers/scsi/libiscsi.c:iscsi_change_queue_depth()
 */
static int tcm_loop_change_queue_depth(
	struct scsi_device *sdev,
	int depth,
	int reason)
{
	switch (reason) {
	case SCSI_QDEPTH_DEFAULT:
		scsi_adjust_queue_depth(sdev, scsi_get_tag_type(sdev), depth);
		break;
	case SCSI_QDEPTH_QFULL:
		scsi_track_queue_full(sdev, depth);
		break;
	case SCSI_QDEPTH_RAMP_UP:
		scsi_adjust_queue_depth(sdev, scsi_get_tag_type(sdev), depth);
		break;
	default:
		return -EOPNOTSUPP;
	}
	return sdev->queue_depth;
}

<<<<<<< HEAD
/*
<<<<<<< HEAD
 * Main entry point from struct scsi_host_template for incoming SCSI CDB+Data
 * from Linux/SCSI subsystem for SCSI low level device drivers (LLDs)
 */
static int tcm_loop_queuecommand(
	struct Scsi_Host *sh,
	struct scsi_cmnd *sc)
{
	struct se_cmd *se_cmd;
	struct se_portal_group *se_tpg;
	struct tcm_loop_hba *tl_hba;
	struct tcm_loop_tpg *tl_tpg;

	TL_CDB_DEBUG("tcm_loop_queuecommand() %d:%d:%d:%d got CDB: 0x%02x"
		" scsi_buf_len: %u\n", sc->device->host->host_no,
		sc->device->id, sc->device->channel, sc->device->lun,
		sc->cmnd[0], scsi_bufflen(sc));
	/*
	 * Locate the tcm_loop_hba_t pointer
	 */
	tl_hba = *(struct tcm_loop_hba **)shost_priv(sc->device->host);
	tl_tpg = &tl_hba->tl_hba_tpgs[sc->device->id];
	se_tpg = &tl_tpg->tl_se_tpg;
	/*
	 * Determine the SAM Task Attribute and allocate tl_cmd and
	 * tl_cmd->tl_se_cmd from TCM infrastructure
	 */
	se_cmd = tcm_loop_allocate_core_cmd(tl_hba, se_tpg, sc);
	if (!se_cmd) {
		sc->scsi_done(sc);
		return 0;
	}
	/*
	 * Queue up the newly allocated to be processed in TCM thread context.
	*/
	transport_generic_handle_cdb_map(se_cmd);
=======
=======
static int tcm_loop_change_queue_type(struct scsi_device *sdev, int tag)
{
	if (sdev->tagged_supported) {
		scsi_set_tag_type(sdev, tag);

		if (tag)
			scsi_activate_tcq(sdev, sdev->queue_depth);
		else
			scsi_deactivate_tcq(sdev, sdev->queue_depth);
	} else
		tag = 0;

	return tag;
}

/*
>>>>>>> refs/remotes/origin/master
 * Locate the SAM Task Attr from struct scsi_cmnd *
 */
static int tcm_loop_sam_attr(struct scsi_cmnd *sc)
{
	if (sc->device->tagged_supported) {
		switch (sc->tag) {
		case HEAD_OF_QUEUE_TAG:
			return MSG_HEAD_TAG;
		case ORDERED_QUEUE_TAG:
			return MSG_ORDERED_TAG;
		default:
			break;
		}
	}

	return MSG_SIMPLE_TAG;
}

static void tcm_loop_submission_work(struct work_struct *work)
{
	struct tcm_loop_cmd *tl_cmd =
		container_of(work, struct tcm_loop_cmd, work);
	struct se_cmd *se_cmd = &tl_cmd->tl_se_cmd;
	struct scsi_cmnd *sc = tl_cmd->sc;
	struct tcm_loop_nexus *tl_nexus;
	struct tcm_loop_hba *tl_hba;
	struct tcm_loop_tpg *tl_tpg;
	struct scatterlist *sgl_bidi = NULL;
	u32 sgl_bidi_count = 0;
<<<<<<< HEAD
	int ret;
=======
	int rc;
>>>>>>> refs/remotes/origin/master

	tl_hba = *(struct tcm_loop_hba **)shost_priv(sc->device->host);
	tl_tpg = &tl_hba->tl_hba_tpgs[sc->device->id];

	/*
	 * Ensure that this tl_tpg reference from the incoming sc->device->id
	 * has already been configured via tcm_loop_make_naa_tpg().
	 */
	if (!tl_tpg->tl_hba) {
		set_host_byte(sc, DID_NO_CONNECT);
		goto out_done;
	}
<<<<<<< HEAD

=======
	if (tl_tpg->tl_transport_status == TCM_TRANSPORT_OFFLINE) {
		set_host_byte(sc, DID_TRANSPORT_DISRUPTED);
		goto out_done;
	}
>>>>>>> refs/remotes/origin/master
	tl_nexus = tl_hba->tl_nexus;
	if (!tl_nexus) {
		scmd_printk(KERN_ERR, sc, "TCM_Loop I_T Nexus"
				" does not exist\n");
		set_host_byte(sc, DID_ERROR);
		goto out_done;
	}
<<<<<<< HEAD

	transport_init_se_cmd(se_cmd, tl_tpg->tl_se_tpg.se_tpg_tfo,
			tl_nexus->se_sess,
			scsi_bufflen(sc), sc->sc_data_direction,
			tcm_loop_sam_attr(sc), &tl_cmd->tl_sense_buf[0]);

=======
>>>>>>> refs/remotes/origin/master
	if (scsi_bidi_cmnd(sc)) {
		struct scsi_data_buffer *sdb = scsi_in(sc);

		sgl_bidi = sdb->table.sgl;
		sgl_bidi_count = sdb->table.nents;
		se_cmd->se_cmd_flags |= SCF_BIDI;

	}
<<<<<<< HEAD

	if (transport_lookup_cmd_lun(se_cmd, tl_cmd->sc->device->lun) < 0) {
		kmem_cache_free(tcm_loop_cmd_cache, tl_cmd);
		set_host_byte(sc, DID_NO_CONNECT);
		goto out_done;
	}

	/*
	 * Because some userspace code via scsi-generic do not memset their
	 * associated read buffers, go ahead and do that here for type
	 * SCF_SCSI_CONTROL_SG_IO_CDB.  Also note that this is currently
	 * guaranteed to be a single SGL for SCF_SCSI_CONTROL_SG_IO_CDB
	 * by target core in target_setup_cmd_from_cdb() ->
	 * transport_generic_cmd_sequencer().
	 */
	if (se_cmd->se_cmd_flags & SCF_SCSI_CONTROL_SG_IO_CDB &&
	    se_cmd->data_direction == DMA_FROM_DEVICE) {
		struct scatterlist *sg = scsi_sglist(sc);
		unsigned char *buf = kmap(sg_page(sg)) + sg->offset;

		if (buf != NULL) {
			memset(buf, 0, sg->length);
			kunmap(sg_page(sg));
		}
	}

	ret = target_setup_cmd_from_cdb(se_cmd, sc->cmnd);
	if (ret == -ENOMEM) {
		transport_send_check_condition_and_sense(se_cmd,
				TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE, 0);
		transport_generic_free_cmd(se_cmd, 0);
		return;
	} else if (ret < 0) {
		if (se_cmd->se_cmd_flags & SCF_SCSI_RESERVATION_CONFLICT)
			tcm_loop_queue_status(se_cmd);
		else
			transport_send_check_condition_and_sense(se_cmd,
					se_cmd->scsi_sense_reason, 0);
		transport_generic_free_cmd(se_cmd, 0);
		return;
	}

	ret = transport_generic_map_mem_to_cmd(se_cmd, scsi_sglist(sc),
			scsi_sg_count(sc), sgl_bidi, sgl_bidi_count);
	if (ret) {
		transport_send_check_condition_and_sense(se_cmd,
					se_cmd->scsi_sense_reason, 0);
		transport_generic_free_cmd(se_cmd, 0);
		return;
	}
	transport_handle_cdb_direct(se_cmd);
=======
	rc = target_submit_cmd_map_sgls(se_cmd, tl_nexus->se_sess, sc->cmnd,
			&tl_cmd->tl_sense_buf[0], tl_cmd->sc->device->lun,
			scsi_bufflen(sc), tcm_loop_sam_attr(sc),
			sc->sc_data_direction, 0,
			scsi_sglist(sc), scsi_sg_count(sc),
			sgl_bidi, sgl_bidi_count);
	if (rc < 0) {
		set_host_byte(sc, DID_NO_CONNECT);
		goto out_done;
	}
>>>>>>> refs/remotes/origin/master
	return;

out_done:
	sc->scsi_done(sc);
	return;
}

/*
 * ->queuecommand can be and usually is called from interrupt context, so
 * defer the actual submission to a workqueue.
 */
static int tcm_loop_queuecommand(struct Scsi_Host *sh, struct scsi_cmnd *sc)
{
	struct tcm_loop_cmd *tl_cmd;

	pr_debug("tcm_loop_queuecommand() %d:%d:%d:%d got CDB: 0x%02x"
		" scsi_buf_len: %u\n", sc->device->host->host_no,
		sc->device->id, sc->device->channel, sc->device->lun,
		sc->cmnd[0], scsi_bufflen(sc));

	tl_cmd = kmem_cache_zalloc(tcm_loop_cmd_cache, GFP_ATOMIC);
	if (!tl_cmd) {
		pr_err("Unable to allocate struct tcm_loop_cmd\n");
		set_host_byte(sc, DID_ERROR);
		sc->scsi_done(sc);
		return 0;
	}

	tl_cmd->sc = sc;
<<<<<<< HEAD
	INIT_WORK(&tl_cmd->work, tcm_loop_submission_work);
	queue_work(tcm_loop_workqueue, &tl_cmd->work);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	tl_cmd->sc_cmd_tag = sc->tag;
	INIT_WORK(&tl_cmd->work, tcm_loop_submission_work);
	queue_work(tcm_loop_workqueue, &tl_cmd->work);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * Called from SCSI EH process context to issue a LUN_RESET TMR
 * to struct scsi_device
 */
<<<<<<< HEAD
static int tcm_loop_device_reset(struct scsi_cmnd *sc)
{
	struct se_cmd *se_cmd = NULL;
	struct se_portal_group *se_tpg;
	struct se_session *se_sess;
	struct tcm_loop_cmd *tl_cmd = NULL;
	struct tcm_loop_hba *tl_hba;
	struct tcm_loop_nexus *tl_nexus;
	struct tcm_loop_tmr *tl_tmr = NULL;
	struct tcm_loop_tpg *tl_tpg;
<<<<<<< HEAD
	int ret = FAILED;
=======
	int ret = FAILED, rc;
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Locate the tcm_loop_hba_t pointer
	 */
	tl_hba = *(struct tcm_loop_hba **)shost_priv(sc->device->host);
	/*
	 * Locate the tl_nexus and se_sess pointers
	 */
	tl_nexus = tl_hba->tl_nexus;
	if (!tl_nexus) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to perform device reset without"
=======
		pr_err("Unable to perform device reset without"
>>>>>>> refs/remotes/origin/cm-10.0
				" active I_T Nexus\n");
		return FAILED;
	}
	se_sess = tl_nexus->se_sess;
	/*
	 * Locate the tl_tpg and se_tpg pointers from TargetID in sc->device->id
	 */
	tl_tpg = &tl_hba->tl_hba_tpgs[sc->device->id];
	se_tpg = &tl_tpg->tl_se_tpg;

	tl_cmd = kmem_cache_zalloc(tcm_loop_cmd_cache, GFP_KERNEL);
	if (!tl_cmd) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to allocate memory for tl_cmd\n");
=======
		pr_err("Unable to allocate memory for tl_cmd\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return FAILED;
=======
static int tcm_loop_issue_tmr(struct tcm_loop_tpg *tl_tpg,
			      struct tcm_loop_nexus *tl_nexus,
			      int lun, int task, enum tcm_tmreq_table tmr)
{
	struct se_cmd *se_cmd = NULL;
	struct se_session *se_sess;
	struct se_portal_group *se_tpg;
	struct tcm_loop_cmd *tl_cmd = NULL;
	struct tcm_loop_tmr *tl_tmr = NULL;
	int ret = TMR_FUNCTION_FAILED, rc;

	tl_cmd = kmem_cache_zalloc(tcm_loop_cmd_cache, GFP_KERNEL);
	if (!tl_cmd) {
		pr_err("Unable to allocate memory for tl_cmd\n");
		return ret;
>>>>>>> refs/remotes/origin/master
	}

	tl_tmr = kzalloc(sizeof(struct tcm_loop_tmr), GFP_KERNEL);
	if (!tl_tmr) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unable to allocate memory for tl_tmr\n");
=======
		pr_err("Unable to allocate memory for tl_tmr\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unable to allocate memory for tl_tmr\n");
>>>>>>> refs/remotes/origin/master
		goto release;
	}
	init_waitqueue_head(&tl_tmr->tl_tmr_wait);

	se_cmd = &tl_cmd->tl_se_cmd;
<<<<<<< HEAD
=======
	se_tpg = &tl_tpg->tl_se_tpg;
	se_sess = tl_nexus->se_sess;
>>>>>>> refs/remotes/origin/master
	/*
	 * Initialize struct se_cmd descriptor from target_core_mod infrastructure
	 */
	transport_init_se_cmd(se_cmd, se_tpg->se_tpg_tfo, se_sess, 0,
				DMA_NONE, MSG_SIMPLE_TAG,
				&tl_cmd->tl_sense_buf[0]);
<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * Allocate the LUN_RESET TMR
	 */
	se_cmd->se_tmr_req = core_tmr_alloc_req(se_cmd, (void *)tl_tmr,
				TMR_LUN_RESET);
	if (IS_ERR(se_cmd->se_tmr_req))
=======

	rc = core_tmr_alloc_req(se_cmd, tl_tmr, TMR_LUN_RESET, GFP_KERNEL);
	if (rc < 0)
>>>>>>> refs/remotes/origin/cm-10.0
		goto release;
	/*
	 * Locate the underlying TCM struct se_lun from sc->device->lun
	 */
<<<<<<< HEAD
	if (transport_get_lun_for_tmr(se_cmd, sc->device->lun) < 0)
=======
	if (transport_lookup_tmr_lun(se_cmd, sc->device->lun) < 0)
>>>>>>> refs/remotes/origin/cm-10.0
		goto release;
	/*
	 * Queue the TMR to TCM Core and sleep waiting for tcm_loop_queue_tm_rsp()
	 * to wake us up.
=======

	rc = core_tmr_alloc_req(se_cmd, tl_tmr, tmr, GFP_KERNEL);
	if (rc < 0)
		goto release;

	if (tmr == TMR_ABORT_TASK)
		se_cmd->se_tmr_req->ref_task_tag = task;

	/*
	 * Locate the underlying TCM struct se_lun
	 */
	if (transport_lookup_tmr_lun(se_cmd, lun) < 0) {
		ret = TMR_LUN_DOES_NOT_EXIST;
		goto release;
	}
	/*
	 * Queue the TMR to TCM Core and sleep waiting for
	 * tcm_loop_queue_tm_rsp() to wake us up.
>>>>>>> refs/remotes/origin/master
	 */
	transport_generic_handle_tmr(se_cmd);
	wait_event(tl_tmr->tl_tmr_wait, atomic_read(&tl_tmr->tmr_complete));
	/*
	 * The TMR LUN_RESET has completed, check the response status and
	 * then release allocations.
	 */
<<<<<<< HEAD
	ret = (se_cmd->se_tmr_req->response == TMR_FUNCTION_COMPLETE) ?
		SUCCESS : FAILED;
release:
	if (se_cmd)
<<<<<<< HEAD
		transport_generic_free_cmd(se_cmd, 1, 1, 0);
=======
		transport_generic_free_cmd(se_cmd, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = se_cmd->se_tmr_req->response;
release:
	if (se_cmd)
		transport_generic_free_cmd(se_cmd, 1);
>>>>>>> refs/remotes/origin/master
	else
		kmem_cache_free(tcm_loop_cmd_cache, tl_cmd);
	kfree(tl_tmr);
	return ret;
}

<<<<<<< HEAD
=======
static int tcm_loop_abort_task(struct scsi_cmnd *sc)
{
	struct tcm_loop_hba *tl_hba;
	struct tcm_loop_nexus *tl_nexus;
	struct tcm_loop_tpg *tl_tpg;
	int ret = FAILED;

	/*
	 * Locate the tcm_loop_hba_t pointer
	 */
	tl_hba = *(struct tcm_loop_hba **)shost_priv(sc->device->host);
	/*
	 * Locate the tl_nexus and se_sess pointers
	 */
	tl_nexus = tl_hba->tl_nexus;
	if (!tl_nexus) {
		pr_err("Unable to perform device reset without"
				" active I_T Nexus\n");
		return FAILED;
	}

	/*
	 * Locate the tl_tpg pointer from TargetID in sc->device->id
	 */
	tl_tpg = &tl_hba->tl_hba_tpgs[sc->device->id];
	ret = tcm_loop_issue_tmr(tl_tpg, tl_nexus, sc->device->lun,
				 sc->tag, TMR_ABORT_TASK);
	return (ret == TMR_FUNCTION_COMPLETE) ? SUCCESS : FAILED;
}

/*
 * Called from SCSI EH process context to issue a LUN_RESET TMR
 * to struct scsi_device
 */
static int tcm_loop_device_reset(struct scsi_cmnd *sc)
{
	struct tcm_loop_hba *tl_hba;
	struct tcm_loop_nexus *tl_nexus;
	struct tcm_loop_tpg *tl_tpg;
	int ret = FAILED;

	/*
	 * Locate the tcm_loop_hba_t pointer
	 */
	tl_hba = *(struct tcm_loop_hba **)shost_priv(sc->device->host);
	/*
	 * Locate the tl_nexus and se_sess pointers
	 */
	tl_nexus = tl_hba->tl_nexus;
	if (!tl_nexus) {
		pr_err("Unable to perform device reset without"
				" active I_T Nexus\n");
		return FAILED;
	}
	/*
	 * Locate the tl_tpg pointer from TargetID in sc->device->id
	 */
	tl_tpg = &tl_hba->tl_hba_tpgs[sc->device->id];
	ret = tcm_loop_issue_tmr(tl_tpg, tl_nexus, sc->device->lun,
				 0, TMR_LUN_RESET);
	return (ret == TMR_FUNCTION_COMPLETE) ? SUCCESS : FAILED;
}

static int tcm_loop_target_reset(struct scsi_cmnd *sc)
{
	struct tcm_loop_hba *tl_hba;
	struct tcm_loop_tpg *tl_tpg;

	/*
	 * Locate the tcm_loop_hba_t pointer
	 */
	tl_hba = *(struct tcm_loop_hba **)shost_priv(sc->device->host);
	if (!tl_hba) {
		pr_err("Unable to perform device reset without"
				" active I_T Nexus\n");
		return FAILED;
	}
	/*
	 * Locate the tl_tpg pointer from TargetID in sc->device->id
	 */
	tl_tpg = &tl_hba->tl_hba_tpgs[sc->device->id];
	if (tl_tpg) {
		tl_tpg->tl_transport_status = TCM_TRANSPORT_ONLINE;
		return SUCCESS;
	}
	return FAILED;
}

>>>>>>> refs/remotes/origin/master
static int tcm_loop_slave_alloc(struct scsi_device *sd)
{
	set_bit(QUEUE_FLAG_BIDI, &sd->request_queue->queue_flags);
	return 0;
}

static int tcm_loop_slave_configure(struct scsi_device *sd)
{
<<<<<<< HEAD
=======
	if (sd->tagged_supported) {
		scsi_activate_tcq(sd, sd->queue_depth);
		scsi_adjust_queue_depth(sd, MSG_SIMPLE_TAG,
					sd->host->cmd_per_lun);
	} else {
		scsi_adjust_queue_depth(sd, 0,
					sd->host->cmd_per_lun);
	}

>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct scsi_host_template tcm_loop_driver_template = {
<<<<<<< HEAD
	.proc_info		= tcm_loop_proc_info,
=======
	.show_info		= tcm_loop_show_info,
>>>>>>> refs/remotes/origin/master
	.proc_name		= "tcm_loopback",
	.name			= "TCM_Loopback",
	.queuecommand		= tcm_loop_queuecommand,
	.change_queue_depth	= tcm_loop_change_queue_depth,
<<<<<<< HEAD
	.eh_device_reset_handler = tcm_loop_device_reset,
<<<<<<< HEAD
	.can_queue		= TL_SCSI_CAN_QUEUE,
	.this_id		= -1,
	.sg_tablesize		= TL_SCSI_SG_TABLESIZE,
	.cmd_per_lun		= TL_SCSI_CMD_PER_LUN,
	.max_sectors		= TL_SCSI_MAX_SECTORS,
=======
=======
	.change_queue_type	= tcm_loop_change_queue_type,
	.eh_abort_handler = tcm_loop_abort_task,
	.eh_device_reset_handler = tcm_loop_device_reset,
	.eh_target_reset_handler = tcm_loop_target_reset,
>>>>>>> refs/remotes/origin/master
	.can_queue		= 1024,
	.this_id		= -1,
	.sg_tablesize		= 256,
	.cmd_per_lun		= 1024,
	.max_sectors		= 0xFFFF,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.use_clustering		= DISABLE_CLUSTERING,
	.slave_alloc		= tcm_loop_slave_alloc,
	.slave_configure	= tcm_loop_slave_configure,
	.module			= THIS_MODULE,
};

static int tcm_loop_driver_probe(struct device *dev)
{
	struct tcm_loop_hba *tl_hba;
	struct Scsi_Host *sh;
	int error;

	tl_hba = to_tcm_loop_hba(dev);

	sh = scsi_host_alloc(&tcm_loop_driver_template,
			sizeof(struct tcm_loop_hba));
	if (!sh) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unable to allocate struct scsi_host\n");
=======
		pr_err("Unable to allocate struct scsi_host\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unable to allocate struct scsi_host\n");
>>>>>>> refs/remotes/origin/master
		return -ENODEV;
	}
	tl_hba->sh = sh;

	/*
	 * Assign the struct tcm_loop_hba pointer to struct Scsi_Host->hostdata
	 */
	*((struct tcm_loop_hba **)sh->hostdata) = tl_hba;
	/*
	 * Setup single ID, Channel and LUN for now..
	 */
	sh->max_id = 2;
	sh->max_lun = 0;
	sh->max_channel = 0;
	sh->max_cmd_len = TL_SCSI_MAX_CMD_LEN;

	error = scsi_add_host(sh, &tl_hba->dev);
	if (error) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "%s: scsi_add_host failed\n", __func__);
=======
		pr_err("%s: scsi_add_host failed\n", __func__);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("%s: scsi_add_host failed\n", __func__);
>>>>>>> refs/remotes/origin/master
		scsi_host_put(sh);
		return -ENODEV;
	}
	return 0;
}

static int tcm_loop_driver_remove(struct device *dev)
{
	struct tcm_loop_hba *tl_hba;
	struct Scsi_Host *sh;

	tl_hba = to_tcm_loop_hba(dev);
	sh = tl_hba->sh;

	scsi_remove_host(sh);
	scsi_host_put(sh);
	return 0;
}

static void tcm_loop_release_adapter(struct device *dev)
{
	struct tcm_loop_hba *tl_hba = to_tcm_loop_hba(dev);

	kfree(tl_hba);
}

/*
 * Called from tcm_loop_make_scsi_hba() in tcm_loop_configfs.c
 */
static int tcm_loop_setup_hba_bus(struct tcm_loop_hba *tl_hba, int tcm_loop_host_id)
{
	int ret;

	tl_hba->dev.bus = &tcm_loop_lld_bus;
	tl_hba->dev.parent = tcm_loop_primary;
	tl_hba->dev.release = &tcm_loop_release_adapter;
	dev_set_name(&tl_hba->dev, "tcm_loop_adapter_%d", tcm_loop_host_id);

	ret = device_register(&tl_hba->dev);
	if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "device_register() failed for"
=======
		pr_err("device_register() failed for"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("device_register() failed for"
>>>>>>> refs/remotes/origin/master
				" tl_hba->dev: %d\n", ret);
		return -ENODEV;
	}

	return 0;
}

/*
 * Called from tcm_loop_fabric_init() in tcl_loop_fabric.c to load the emulated
 * tcm_loop SCSI bus.
 */
static int tcm_loop_alloc_core_bus(void)
{
	int ret;

	tcm_loop_primary = root_device_register("tcm_loop_0");
	if (IS_ERR(tcm_loop_primary)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unable to allocate tcm_loop_primary\n");
=======
		pr_err("Unable to allocate tcm_loop_primary\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unable to allocate tcm_loop_primary\n");
>>>>>>> refs/remotes/origin/master
		return PTR_ERR(tcm_loop_primary);
	}

	ret = bus_register(&tcm_loop_lld_bus);
	if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "bus_register() failed for tcm_loop_lld_bus\n");
=======
		pr_err("bus_register() failed for tcm_loop_lld_bus\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("bus_register() failed for tcm_loop_lld_bus\n");
>>>>>>> refs/remotes/origin/master
		goto dev_unreg;
	}

	ret = driver_register(&tcm_loop_driverfs);
	if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "driver_register() failed for"
=======
		pr_err("driver_register() failed for"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("driver_register() failed for"
>>>>>>> refs/remotes/origin/master
				"tcm_loop_driverfs\n");
		goto bus_unreg;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "Initialized TCM Loop Core Bus\n");
=======
	pr_debug("Initialized TCM Loop Core Bus\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("Initialized TCM Loop Core Bus\n");
>>>>>>> refs/remotes/origin/master
	return ret;

bus_unreg:
	bus_unregister(&tcm_loop_lld_bus);
dev_unreg:
	root_device_unregister(tcm_loop_primary);
	return ret;
}

static void tcm_loop_release_core_bus(void)
{
	driver_unregister(&tcm_loop_driverfs);
	bus_unregister(&tcm_loop_lld_bus);
	root_device_unregister(tcm_loop_primary);

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "Releasing TCM Loop Core BUS\n");
=======
	pr_debug("Releasing TCM Loop Core BUS\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("Releasing TCM Loop Core BUS\n");
>>>>>>> refs/remotes/origin/master
}

static char *tcm_loop_get_fabric_name(void)
{
	return "loopback";
}

static u8 tcm_loop_get_fabric_proto_ident(struct se_portal_group *se_tpg)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcm_loop_tpg *tl_tpg =
			(struct tcm_loop_tpg *)se_tpg->se_tpg_fabric_ptr;
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/master
	struct tcm_loop_hba *tl_hba = tl_tpg->tl_hba;
	/*
	 * tl_proto_id is set at tcm_loop_configfs.c:tcm_loop_make_scsi_hba()
	 * time based on the protocol dependent prefix of the passed configfs group.
	 *
	 * Based upon tl_proto_id, TCM_Loop emulates the requested fabric
	 * ProtocolID using target_core_fabric_lib.c symbols.
	 */
	switch (tl_hba->tl_proto_id) {
	case SCSI_PROTOCOL_SAS:
		return sas_get_fabric_proto_ident(se_tpg);
	case SCSI_PROTOCOL_FCP:
		return fc_get_fabric_proto_ident(se_tpg);
	case SCSI_PROTOCOL_ISCSI:
		return iscsi_get_fabric_proto_ident(se_tpg);
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unknown tl_proto_id: 0x%02x, using"
=======
		pr_err("Unknown tl_proto_id: 0x%02x, using"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unknown tl_proto_id: 0x%02x, using"
>>>>>>> refs/remotes/origin/master
			" SAS emulation\n", tl_hba->tl_proto_id);
		break;
	}

	return sas_get_fabric_proto_ident(se_tpg);
}

static char *tcm_loop_get_endpoint_wwn(struct se_portal_group *se_tpg)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcm_loop_tpg *tl_tpg =
		(struct tcm_loop_tpg *)se_tpg->se_tpg_fabric_ptr;
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/master
	/*
	 * Return the passed NAA identifier for the SAS Target Port
	 */
	return &tl_tpg->tl_hba->tl_wwn_address[0];
}

static u16 tcm_loop_get_tag(struct se_portal_group *se_tpg)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcm_loop_tpg *tl_tpg =
		(struct tcm_loop_tpg *)se_tpg->se_tpg_fabric_ptr;
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/master
	/*
	 * This Tag is used when forming SCSI Name identifier in EVPD=1 0x83
	 * to represent the SCSI Target Port.
	 */
	return tl_tpg->tl_tpgt;
}

static u32 tcm_loop_get_default_depth(struct se_portal_group *se_tpg)
{
	return 1;
}

static u32 tcm_loop_get_pr_transport_id(
	struct se_portal_group *se_tpg,
	struct se_node_acl *se_nacl,
	struct t10_pr_registration *pr_reg,
	int *format_code,
	unsigned char *buf)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcm_loop_tpg *tl_tpg =
			(struct tcm_loop_tpg *)se_tpg->se_tpg_fabric_ptr;
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/master
	struct tcm_loop_hba *tl_hba = tl_tpg->tl_hba;

	switch (tl_hba->tl_proto_id) {
	case SCSI_PROTOCOL_SAS:
		return sas_get_pr_transport_id(se_tpg, se_nacl, pr_reg,
					format_code, buf);
	case SCSI_PROTOCOL_FCP:
		return fc_get_pr_transport_id(se_tpg, se_nacl, pr_reg,
					format_code, buf);
	case SCSI_PROTOCOL_ISCSI:
		return iscsi_get_pr_transport_id(se_tpg, se_nacl, pr_reg,
					format_code, buf);
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unknown tl_proto_id: 0x%02x, using"
=======
		pr_err("Unknown tl_proto_id: 0x%02x, using"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unknown tl_proto_id: 0x%02x, using"
>>>>>>> refs/remotes/origin/master
			" SAS emulation\n", tl_hba->tl_proto_id);
		break;
	}

	return sas_get_pr_transport_id(se_tpg, se_nacl, pr_reg,
			format_code, buf);
}

static u32 tcm_loop_get_pr_transport_id_len(
	struct se_portal_group *se_tpg,
	struct se_node_acl *se_nacl,
	struct t10_pr_registration *pr_reg,
	int *format_code)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcm_loop_tpg *tl_tpg =
			(struct tcm_loop_tpg *)se_tpg->se_tpg_fabric_ptr;
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/master
	struct tcm_loop_hba *tl_hba = tl_tpg->tl_hba;

	switch (tl_hba->tl_proto_id) {
	case SCSI_PROTOCOL_SAS:
		return sas_get_pr_transport_id_len(se_tpg, se_nacl, pr_reg,
					format_code);
	case SCSI_PROTOCOL_FCP:
		return fc_get_pr_transport_id_len(se_tpg, se_nacl, pr_reg,
					format_code);
	case SCSI_PROTOCOL_ISCSI:
		return iscsi_get_pr_transport_id_len(se_tpg, se_nacl, pr_reg,
					format_code);
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unknown tl_proto_id: 0x%02x, using"
=======
		pr_err("Unknown tl_proto_id: 0x%02x, using"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unknown tl_proto_id: 0x%02x, using"
>>>>>>> refs/remotes/origin/master
			" SAS emulation\n", tl_hba->tl_proto_id);
		break;
	}

	return sas_get_pr_transport_id_len(se_tpg, se_nacl, pr_reg,
			format_code);
}

/*
 * Used for handling SCSI fabric dependent TransportIDs in SPC-3 and above
 * Persistent Reservation SPEC_I_PT=1 and PROUT REGISTER_AND_MOVE operations.
 */
static char *tcm_loop_parse_pr_out_transport_id(
	struct se_portal_group *se_tpg,
	const char *buf,
	u32 *out_tid_len,
	char **port_nexus_ptr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcm_loop_tpg *tl_tpg =
			(struct tcm_loop_tpg *)se_tpg->se_tpg_fabric_ptr;
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tcm_loop_tpg *tl_tpg = se_tpg->se_tpg_fabric_ptr;
>>>>>>> refs/remotes/origin/master
	struct tcm_loop_hba *tl_hba = tl_tpg->tl_hba;

	switch (tl_hba->tl_proto_id) {
	case SCSI_PROTOCOL_SAS:
		return sas_parse_pr_out_transport_id(se_tpg, buf, out_tid_len,
					port_nexus_ptr);
	case SCSI_PROTOCOL_FCP:
		return fc_parse_pr_out_transport_id(se_tpg, buf, out_tid_len,
					port_nexus_ptr);
	case SCSI_PROTOCOL_ISCSI:
		return iscsi_parse_pr_out_transport_id(se_tpg, buf, out_tid_len,
					port_nexus_ptr);
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unknown tl_proto_id: 0x%02x, using"
=======
		pr_err("Unknown tl_proto_id: 0x%02x, using"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unknown tl_proto_id: 0x%02x, using"
>>>>>>> refs/remotes/origin/master
			" SAS emulation\n", tl_hba->tl_proto_id);
		break;
	}

	return sas_parse_pr_out_transport_id(se_tpg, buf, out_tid_len,
			port_nexus_ptr);
}

/*
 * Returning (1) here allows for target_core_mod struct se_node_acl to be generated
 * based upon the incoming fabric dependent SCSI Initiator Port
 */
static int tcm_loop_check_demo_mode(struct se_portal_group *se_tpg)
{
	return 1;
}

static int tcm_loop_check_demo_mode_cache(struct se_portal_group *se_tpg)
{
	return 0;
}

/*
 * Allow I_T Nexus full READ-WRITE access without explict Initiator Node ACLs for
 * local virtual Linux/SCSI LLD passthrough into VM hypervisor guest
 */
static int tcm_loop_check_demo_mode_write_protect(struct se_portal_group *se_tpg)
{
	return 0;
}

/*
 * Because TCM_Loop does not use explict ACLs and MappedLUNs, this will
 * never be called for TCM_Loop by target_core_fabric_configfs.c code.
 * It has been added here as a nop for target_fabric_tf_ops_check()
 */
static int tcm_loop_check_prod_mode_write_protect(struct se_portal_group *se_tpg)
{
	return 0;
}

static struct se_node_acl *tcm_loop_tpg_alloc_fabric_acl(
	struct se_portal_group *se_tpg)
{
	struct tcm_loop_nacl *tl_nacl;

	tl_nacl = kzalloc(sizeof(struct tcm_loop_nacl), GFP_KERNEL);
	if (!tl_nacl) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unable to allocate struct tcm_loop_nacl\n");
=======
		pr_err("Unable to allocate struct tcm_loop_nacl\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unable to allocate struct tcm_loop_nacl\n");
>>>>>>> refs/remotes/origin/master
		return NULL;
	}

	return &tl_nacl->se_node_acl;
}

static void tcm_loop_tpg_release_fabric_acl(
	struct se_portal_group *se_tpg,
	struct se_node_acl *se_nacl)
{
	struct tcm_loop_nacl *tl_nacl = container_of(se_nacl,
				struct tcm_loop_nacl, se_node_acl);

	kfree(tl_nacl);
}

static u32 tcm_loop_get_inst_index(struct se_portal_group *se_tpg)
{
	return 1;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void tcm_loop_new_cmd_failure(struct se_cmd *se_cmd)
{
	/*
	 * Since TCM_loop is already passing struct scatterlist data from
	 * struct scsi_cmnd, no more Linux/SCSI failure dependent state need
	 * to be handled here.
	 */
	return;
}

static int tcm_loop_is_state_remove(struct se_cmd *se_cmd)
{
	/*
	 * Assume struct scsi_cmnd is not in remove state..
	 */
	return 0;
}

static int tcm_loop_sess_logged_in(struct se_session *se_sess)
{
	/*
	 * Assume that TL Nexus is always active
	 */
	return 1;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static u32 tcm_loop_sess_get_index(struct se_session *se_sess)
{
	return 1;
}

static void tcm_loop_set_default_node_attributes(struct se_node_acl *se_acl)
{
	return;
}

static u32 tcm_loop_get_task_tag(struct se_cmd *se_cmd)
{
<<<<<<< HEAD
	return 1;
=======
	struct tcm_loop_cmd *tl_cmd = container_of(se_cmd,
			struct tcm_loop_cmd, tl_se_cmd);

	return tl_cmd->sc_cmd_tag;
>>>>>>> refs/remotes/origin/master
}

static int tcm_loop_get_cmd_state(struct se_cmd *se_cmd)
{
	struct tcm_loop_cmd *tl_cmd = container_of(se_cmd,
			struct tcm_loop_cmd, tl_se_cmd);

	return tl_cmd->sc_cmd_state;
}

static int tcm_loop_shutdown_session(struct se_session *se_sess)
{
	return 0;
}

static void tcm_loop_close_session(struct se_session *se_sess)
{
	return;
};

<<<<<<< HEAD
<<<<<<< HEAD
static void tcm_loop_stop_session(
	struct se_session *se_sess,
	int sess_sleep,
	int conn_sleep)
{
	return;
}

static void tcm_loop_fall_back_to_erl0(struct se_session *se_sess)
{
	return;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int tcm_loop_write_pending(struct se_cmd *se_cmd)
{
	/*
	 * Since Linux/SCSI has already sent down a struct scsi_cmnd
	 * sc->sc_data_direction of DMA_TO_DEVICE with struct scatterlist array
	 * memory, and memory has already been mapped to struct se_cmd->t_mem_list
	 * format with transport_generic_map_mem_to_cmd().
	 *
	 * We now tell TCM to add this WRITE CDB directly into the TCM storage
	 * object execution queue.
	 */
<<<<<<< HEAD
	transport_generic_process_write(se_cmd);
=======
	target_execute_cmd(se_cmd);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int tcm_loop_write_pending_status(struct se_cmd *se_cmd)
{
	return 0;
}

static int tcm_loop_queue_data_in(struct se_cmd *se_cmd)
{
	struct tcm_loop_cmd *tl_cmd = container_of(se_cmd,
				struct tcm_loop_cmd, tl_se_cmd);
	struct scsi_cmnd *sc = tl_cmd->sc;

<<<<<<< HEAD
<<<<<<< HEAD
	TL_CDB_DEBUG("tcm_loop_queue_data_in() called for scsi_cmnd: %p"
=======
	pr_debug("tcm_loop_queue_data_in() called for scsi_cmnd: %p"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("tcm_loop_queue_data_in() called for scsi_cmnd: %p"
>>>>>>> refs/remotes/origin/master
		     " cdb: 0x%02x\n", sc, sc->cmnd[0]);

	sc->result = SAM_STAT_GOOD;
	set_host_byte(sc, DID_OK);
	if ((se_cmd->se_cmd_flags & SCF_OVERFLOW_BIT) ||
	    (se_cmd->se_cmd_flags & SCF_UNDERFLOW_BIT))
		scsi_set_resid(sc, se_cmd->residual_count);
	sc->scsi_done(sc);
	return 0;
}

static int tcm_loop_queue_status(struct se_cmd *se_cmd)
{
	struct tcm_loop_cmd *tl_cmd = container_of(se_cmd,
				struct tcm_loop_cmd, tl_se_cmd);
	struct scsi_cmnd *sc = tl_cmd->sc;

<<<<<<< HEAD
<<<<<<< HEAD
	TL_CDB_DEBUG("tcm_loop_queue_status() called for scsi_cmnd: %p"
=======
	pr_debug("tcm_loop_queue_status() called for scsi_cmnd: %p"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("tcm_loop_queue_status() called for scsi_cmnd: %p"
>>>>>>> refs/remotes/origin/master
			" cdb: 0x%02x\n", sc, sc->cmnd[0]);

	if (se_cmd->sense_buffer &&
	   ((se_cmd->se_cmd_flags & SCF_TRANSPORT_TASK_SENSE) ||
	    (se_cmd->se_cmd_flags & SCF_EMULATED_TASK_SENSE))) {

<<<<<<< HEAD
<<<<<<< HEAD
		memcpy((void *)sc->sense_buffer, (void *)se_cmd->sense_buffer,
=======
		memcpy(sc->sense_buffer, se_cmd->sense_buffer,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		memcpy(sc->sense_buffer, se_cmd->sense_buffer,
>>>>>>> refs/remotes/origin/master
				SCSI_SENSE_BUFFERSIZE);
		sc->result = SAM_STAT_CHECK_CONDITION;
		set_driver_byte(sc, DRIVER_SENSE);
	} else
		sc->result = se_cmd->scsi_status;

	set_host_byte(sc, DID_OK);
	if ((se_cmd->se_cmd_flags & SCF_OVERFLOW_BIT) ||
	    (se_cmd->se_cmd_flags & SCF_UNDERFLOW_BIT))
		scsi_set_resid(sc, se_cmd->residual_count);
	sc->scsi_done(sc);
	return 0;
}

<<<<<<< HEAD
static int tcm_loop_queue_tm_rsp(struct se_cmd *se_cmd)
=======
static void tcm_loop_queue_tm_rsp(struct se_cmd *se_cmd)
>>>>>>> refs/remotes/origin/master
{
	struct se_tmr_req *se_tmr = se_cmd->se_tmr_req;
	struct tcm_loop_tmr *tl_tmr = se_tmr->fabric_tmr_ptr;
	/*
	 * The SCSI EH thread will be sleeping on se_tmr->tl_tmr_wait, go ahead
	 * and wake up the wait_queue_head_t in tcm_loop_device_reset()
	 */
	atomic_set(&tl_tmr->tmr_complete, 1);
	wake_up(&tl_tmr->tl_tmr_wait);
<<<<<<< HEAD
	return 0;
}

static u16 tcm_loop_set_fabric_sense_len(struct se_cmd *se_cmd, u32 sense_length)
{
	return 0;
}

static u16 tcm_loop_get_fabric_sense_len(void)
{
	return 0;
=======
>>>>>>> refs/remotes/origin/master
}

static char *tcm_loop_dump_proto_id(struct tcm_loop_hba *tl_hba)
{
	switch (tl_hba->tl_proto_id) {
	case SCSI_PROTOCOL_SAS:
		return "SAS";
	case SCSI_PROTOCOL_FCP:
		return "FCP";
	case SCSI_PROTOCOL_ISCSI:
		return "iSCSI";
	default:
		break;
	}

	return "Unknown";
}

/* Start items for tcm_loop_port_cit */

static int tcm_loop_port_link(
	struct se_portal_group *se_tpg,
	struct se_lun *lun)
{
	struct tcm_loop_tpg *tl_tpg = container_of(se_tpg,
				struct tcm_loop_tpg, tl_se_tpg);
	struct tcm_loop_hba *tl_hba = tl_tpg->tl_hba;

	atomic_inc(&tl_tpg->tl_tpg_port_count);
	smp_mb__after_atomic_inc();
	/*
	 * Add Linux/SCSI struct scsi_device by HCTL
	 */
	scsi_add_device(tl_hba->sh, 0, tl_tpg->tl_tpgt, lun->unpacked_lun);

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "TCM_Loop_ConfigFS: Port Link Successful\n");
=======
	pr_debug("TCM_Loop_ConfigFS: Port Link Successful\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("TCM_Loop_ConfigFS: Port Link Successful\n");
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void tcm_loop_port_unlink(
	struct se_portal_group *se_tpg,
	struct se_lun *se_lun)
{
	struct scsi_device *sd;
	struct tcm_loop_hba *tl_hba;
	struct tcm_loop_tpg *tl_tpg;

	tl_tpg = container_of(se_tpg, struct tcm_loop_tpg, tl_se_tpg);
	tl_hba = tl_tpg->tl_hba;

	sd = scsi_device_lookup(tl_hba->sh, 0, tl_tpg->tl_tpgt,
				se_lun->unpacked_lun);
	if (!sd) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unable to locate struct scsi_device for %d:%d:"
=======
		pr_err("Unable to locate struct scsi_device for %d:%d:"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unable to locate struct scsi_device for %d:%d:"
>>>>>>> refs/remotes/origin/master
			"%d\n", 0, tl_tpg->tl_tpgt, se_lun->unpacked_lun);
		return;
	}
	/*
	 * Remove Linux/SCSI struct scsi_device by HCTL
	 */
	scsi_remove_device(sd);
	scsi_device_put(sd);

	atomic_dec(&tl_tpg->tl_tpg_port_count);
	smp_mb__after_atomic_dec();

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "TCM_Loop_ConfigFS: Port Unlink Successful\n");
=======
	pr_debug("TCM_Loop_ConfigFS: Port Unlink Successful\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("TCM_Loop_ConfigFS: Port Unlink Successful\n");
>>>>>>> refs/remotes/origin/master
}

/* End items for tcm_loop_port_cit */

/* Start items for tcm_loop_nexus_cit */

static int tcm_loop_make_nexus(
	struct tcm_loop_tpg *tl_tpg,
	const char *name)
{
	struct se_portal_group *se_tpg;
	struct tcm_loop_hba *tl_hba = tl_tpg->tl_hba;
	struct tcm_loop_nexus *tl_nexus;
	int ret = -ENOMEM;

	if (tl_tpg->tl_hba->tl_nexus) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_INFO "tl_tpg->tl_hba->tl_nexus already exists\n");
=======
		pr_debug("tl_tpg->tl_hba->tl_nexus already exists\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_debug("tl_tpg->tl_hba->tl_nexus already exists\n");
>>>>>>> refs/remotes/origin/master
		return -EEXIST;
	}
	se_tpg = &tl_tpg->tl_se_tpg;

	tl_nexus = kzalloc(sizeof(struct tcm_loop_nexus), GFP_KERNEL);
	if (!tl_nexus) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unable to allocate struct tcm_loop_nexus\n");
=======
		pr_err("Unable to allocate struct tcm_loop_nexus\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unable to allocate struct tcm_loop_nexus\n");
>>>>>>> refs/remotes/origin/master
		return -ENOMEM;
	}
	/*
	 * Initialize the struct se_session pointer
	 */
	tl_nexus->se_sess = transport_init_session();
	if (IS_ERR(tl_nexus->se_sess)) {
		ret = PTR_ERR(tl_nexus->se_sess);
		goto out;
	}
	/*
	 * Since we are running in 'demo mode' this call with generate a
	 * struct se_node_acl for the tcm_loop struct se_portal_group with the SCSI
	 * Initiator port name of the passed configfs group 'name'.
	 */
	tl_nexus->se_sess->se_node_acl = core_tpg_check_initiator_node_acl(
				se_tpg, (unsigned char *)name);
	if (!tl_nexus->se_sess->se_node_acl) {
		transport_free_session(tl_nexus->se_sess);
		goto out;
	}
	/*
	 * Now, register the SAS I_T Nexus as active with the call to
	 * transport_register_session()
	 */
	__transport_register_session(se_tpg, tl_nexus->se_sess->se_node_acl,
<<<<<<< HEAD
<<<<<<< HEAD
			tl_nexus->se_sess, (void *)tl_nexus);
	tl_tpg->tl_hba->tl_nexus = tl_nexus;
	printk(KERN_INFO "TCM_Loop_ConfigFS: Established I_T Nexus to emulated"
=======
			tl_nexus->se_sess, tl_nexus);
	tl_tpg->tl_hba->tl_nexus = tl_nexus;
	pr_debug("TCM_Loop_ConfigFS: Established I_T Nexus to emulated"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			tl_nexus->se_sess, tl_nexus);
	tl_tpg->tl_hba->tl_nexus = tl_nexus;
	pr_debug("TCM_Loop_ConfigFS: Established I_T Nexus to emulated"
>>>>>>> refs/remotes/origin/master
		" %s Initiator Port: %s\n", tcm_loop_dump_proto_id(tl_hba),
		name);
	return 0;

out:
	kfree(tl_nexus);
	return ret;
}

static int tcm_loop_drop_nexus(
	struct tcm_loop_tpg *tpg)
{
	struct se_session *se_sess;
	struct tcm_loop_nexus *tl_nexus;
	struct tcm_loop_hba *tl_hba = tpg->tl_hba;

<<<<<<< HEAD
	tl_nexus = tpg->tl_hba->tl_nexus;
=======
	if (!tl_hba)
		return -ENODEV;

	tl_nexus = tl_hba->tl_nexus;
>>>>>>> refs/remotes/origin/master
	if (!tl_nexus)
		return -ENODEV;

	se_sess = tl_nexus->se_sess;
	if (!se_sess)
		return -ENODEV;

	if (atomic_read(&tpg->tl_tpg_port_count)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unable to remove TCM_Loop I_T Nexus with"
=======
		pr_err("Unable to remove TCM_Loop I_T Nexus with"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unable to remove TCM_Loop I_T Nexus with"
>>>>>>> refs/remotes/origin/master
			" active TPG port count: %d\n",
			atomic_read(&tpg->tl_tpg_port_count));
		return -EPERM;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "TCM_Loop_ConfigFS: Removing I_T Nexus to emulated"
=======
	pr_debug("TCM_Loop_ConfigFS: Removing I_T Nexus to emulated"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("TCM_Loop_ConfigFS: Removing I_T Nexus to emulated"
>>>>>>> refs/remotes/origin/master
		" %s Initiator Port: %s\n", tcm_loop_dump_proto_id(tl_hba),
		tl_nexus->se_sess->se_node_acl->initiatorname);
	/*
	 * Release the SCSI I_T Nexus to the emulated SAS Target Port
	 */
	transport_deregister_session(tl_nexus->se_sess);
	tpg->tl_hba->tl_nexus = NULL;
	kfree(tl_nexus);
	return 0;
}

/* End items for tcm_loop_nexus_cit */

static ssize_t tcm_loop_tpg_show_nexus(
	struct se_portal_group *se_tpg,
	char *page)
{
	struct tcm_loop_tpg *tl_tpg = container_of(se_tpg,
			struct tcm_loop_tpg, tl_se_tpg);
	struct tcm_loop_nexus *tl_nexus;
	ssize_t ret;

	tl_nexus = tl_tpg->tl_hba->tl_nexus;
	if (!tl_nexus)
		return -ENODEV;

	ret = snprintf(page, PAGE_SIZE, "%s\n",
		tl_nexus->se_sess->se_node_acl->initiatorname);

	return ret;
}

static ssize_t tcm_loop_tpg_store_nexus(
	struct se_portal_group *se_tpg,
	const char *page,
	size_t count)
{
	struct tcm_loop_tpg *tl_tpg = container_of(se_tpg,
			struct tcm_loop_tpg, tl_se_tpg);
	struct tcm_loop_hba *tl_hba = tl_tpg->tl_hba;
	unsigned char i_port[TL_WWN_ADDR_LEN], *ptr, *port_ptr;
	int ret;
	/*
	 * Shutdown the active I_T nexus if 'NULL' is passed..
	 */
	if (!strncmp(page, "NULL", 4)) {
		ret = tcm_loop_drop_nexus(tl_tpg);
		return (!ret) ? count : ret;
	}
	/*
	 * Otherwise make sure the passed virtual Initiator port WWN matches
	 * the fabric protocol_id set in tcm_loop_make_scsi_hba(), and call
	 * tcm_loop_make_nexus()
	 */
	if (strlen(page) >= TL_WWN_ADDR_LEN) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Emulated NAA Sas Address: %s, exceeds"
=======
		pr_err("Emulated NAA Sas Address: %s, exceeds"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Emulated NAA Sas Address: %s, exceeds"
>>>>>>> refs/remotes/origin/master
				" max: %d\n", page, TL_WWN_ADDR_LEN);
		return -EINVAL;
	}
	snprintf(&i_port[0], TL_WWN_ADDR_LEN, "%s", page);

	ptr = strstr(i_port, "naa.");
	if (ptr) {
		if (tl_hba->tl_proto_id != SCSI_PROTOCOL_SAS) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_ERR "Passed SAS Initiator Port %s does not"
=======
			pr_err("Passed SAS Initiator Port %s does not"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_err("Passed SAS Initiator Port %s does not"
>>>>>>> refs/remotes/origin/master
				" match target port protoid: %s\n", i_port,
				tcm_loop_dump_proto_id(tl_hba));
			return -EINVAL;
		}
		port_ptr = &i_port[0];
		goto check_newline;
	}
	ptr = strstr(i_port, "fc.");
	if (ptr) {
		if (tl_hba->tl_proto_id != SCSI_PROTOCOL_FCP) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_ERR "Passed FCP Initiator Port %s does not"
=======
			pr_err("Passed FCP Initiator Port %s does not"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_err("Passed FCP Initiator Port %s does not"
>>>>>>> refs/remotes/origin/master
				" match target port protoid: %s\n", i_port,
				tcm_loop_dump_proto_id(tl_hba));
			return -EINVAL;
		}
		port_ptr = &i_port[3]; /* Skip over "fc." */
		goto check_newline;
	}
	ptr = strstr(i_port, "iqn.");
	if (ptr) {
		if (tl_hba->tl_proto_id != SCSI_PROTOCOL_ISCSI) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_ERR "Passed iSCSI Initiator Port %s does not"
=======
			pr_err("Passed iSCSI Initiator Port %s does not"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_err("Passed iSCSI Initiator Port %s does not"
>>>>>>> refs/remotes/origin/master
				" match target port protoid: %s\n", i_port,
				tcm_loop_dump_proto_id(tl_hba));
			return -EINVAL;
		}
		port_ptr = &i_port[0];
		goto check_newline;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_ERR "Unable to locate prefix for emulated Initiator Port:"
=======
	pr_err("Unable to locate prefix for emulated Initiator Port:"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_err("Unable to locate prefix for emulated Initiator Port:"
>>>>>>> refs/remotes/origin/master
			" %s\n", i_port);
	return -EINVAL;
	/*
	 * Clear any trailing newline for the NAA WWN
	 */
check_newline:
	if (i_port[strlen(i_port)-1] == '\n')
		i_port[strlen(i_port)-1] = '\0';

	ret = tcm_loop_make_nexus(tl_tpg, port_ptr);
	if (ret < 0)
		return ret;

	return count;
}

TF_TPG_BASE_ATTR(tcm_loop, nexus, S_IRUGO | S_IWUSR);

<<<<<<< HEAD
static struct configfs_attribute *tcm_loop_tpg_attrs[] = {
	&tcm_loop_tpg_nexus.attr,
=======
static ssize_t tcm_loop_tpg_show_transport_status(
	struct se_portal_group *se_tpg,
	char *page)
{
	struct tcm_loop_tpg *tl_tpg = container_of(se_tpg,
			struct tcm_loop_tpg, tl_se_tpg);
	const char *status = NULL;
	ssize_t ret = -EINVAL;

	switch (tl_tpg->tl_transport_status) {
	case TCM_TRANSPORT_ONLINE:
		status = "online";
		break;
	case TCM_TRANSPORT_OFFLINE:
		status = "offline";
		break;
	default:
		break;
	}

	if (status)
		ret = snprintf(page, PAGE_SIZE, "%s\n", status);

	return ret;
}

static ssize_t tcm_loop_tpg_store_transport_status(
	struct se_portal_group *se_tpg,
	const char *page,
	size_t count)
{
	struct tcm_loop_tpg *tl_tpg = container_of(se_tpg,
			struct tcm_loop_tpg, tl_se_tpg);

	if (!strncmp(page, "online", 6)) {
		tl_tpg->tl_transport_status = TCM_TRANSPORT_ONLINE;
		return count;
	}
	if (!strncmp(page, "offline", 7)) {
		tl_tpg->tl_transport_status = TCM_TRANSPORT_OFFLINE;
		return count;
	}
	return -EINVAL;
}

TF_TPG_BASE_ATTR(tcm_loop, transport_status, S_IRUGO | S_IWUSR);

static struct configfs_attribute *tcm_loop_tpg_attrs[] = {
	&tcm_loop_tpg_nexus.attr,
	&tcm_loop_tpg_transport_status.attr,
>>>>>>> refs/remotes/origin/master
	NULL,
};

/* Start items for tcm_loop_naa_cit */

struct se_portal_group *tcm_loop_make_naa_tpg(
	struct se_wwn *wwn,
	struct config_group *group,
	const char *name)
{
	struct tcm_loop_hba *tl_hba = container_of(wwn,
			struct tcm_loop_hba, tl_hba_wwn);
	struct tcm_loop_tpg *tl_tpg;
	char *tpgt_str, *end_ptr;
	int ret;
	unsigned short int tpgt;

	tpgt_str = strstr(name, "tpgt_");
	if (!tpgt_str) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unable to locate \"tpgt_#\" directory"
=======
		pr_err("Unable to locate \"tpgt_#\" directory"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unable to locate \"tpgt_#\" directory"
>>>>>>> refs/remotes/origin/master
				" group\n");
		return ERR_PTR(-EINVAL);
	}
	tpgt_str += 5; /* Skip ahead of "tpgt_" */
	tpgt = (unsigned short int) simple_strtoul(tpgt_str, &end_ptr, 0);

<<<<<<< HEAD
<<<<<<< HEAD
	if (tpgt > TL_TPGS_PER_HBA) {
		printk(KERN_ERR "Passed tpgt: %hu exceeds TL_TPGS_PER_HBA:"
=======
	if (tpgt >= TL_TPGS_PER_HBA) {
		pr_err("Passed tpgt: %hu exceeds TL_TPGS_PER_HBA:"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (tpgt >= TL_TPGS_PER_HBA) {
		pr_err("Passed tpgt: %hu exceeds TL_TPGS_PER_HBA:"
>>>>>>> refs/remotes/origin/master
				" %u\n", tpgt, TL_TPGS_PER_HBA);
		return ERR_PTR(-EINVAL);
	}
	tl_tpg = &tl_hba->tl_hba_tpgs[tpgt];
	tl_tpg->tl_hba = tl_hba;
	tl_tpg->tl_tpgt = tpgt;
	/*
	 * Register the tl_tpg as a emulated SAS TCM Target Endpoint
	 */
	ret = core_tpg_register(&tcm_loop_fabric_configfs->tf_ops,
<<<<<<< HEAD
<<<<<<< HEAD
			wwn, &tl_tpg->tl_se_tpg, (void *)tl_tpg,
=======
			wwn, &tl_tpg->tl_se_tpg, tl_tpg,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			wwn, &tl_tpg->tl_se_tpg, tl_tpg,
>>>>>>> refs/remotes/origin/master
			TRANSPORT_TPG_TYPE_NORMAL);
	if (ret < 0)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "TCM_Loop_ConfigFS: Allocated Emulated %s"
=======
	pr_debug("TCM_Loop_ConfigFS: Allocated Emulated %s"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("TCM_Loop_ConfigFS: Allocated Emulated %s"
>>>>>>> refs/remotes/origin/master
		" Target Port %s,t,0x%04x\n", tcm_loop_dump_proto_id(tl_hba),
		config_item_name(&wwn->wwn_group.cg_item), tpgt);

	return &tl_tpg->tl_se_tpg;
}

void tcm_loop_drop_naa_tpg(
	struct se_portal_group *se_tpg)
{
	struct se_wwn *wwn = se_tpg->se_tpg_wwn;
	struct tcm_loop_tpg *tl_tpg = container_of(se_tpg,
				struct tcm_loop_tpg, tl_se_tpg);
	struct tcm_loop_hba *tl_hba;
	unsigned short tpgt;

	tl_hba = tl_tpg->tl_hba;
	tpgt = tl_tpg->tl_tpgt;
	/*
	 * Release the I_T Nexus for the Virtual SAS link if present
	 */
	tcm_loop_drop_nexus(tl_tpg);
	/*
	 * Deregister the tl_tpg as a emulated SAS TCM Target Endpoint
	 */
	core_tpg_deregister(se_tpg);

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "TCM_Loop_ConfigFS: Deallocated Emulated %s"
=======
=======
>>>>>>> refs/remotes/origin/master
	tl_tpg->tl_hba = NULL;
	tl_tpg->tl_tpgt = 0;

	pr_debug("TCM_Loop_ConfigFS: Deallocated Emulated %s"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		" Target Port %s,t,0x%04x\n", tcm_loop_dump_proto_id(tl_hba),
		config_item_name(&wwn->wwn_group.cg_item), tpgt);
}

/* End items for tcm_loop_naa_cit */

/* Start items for tcm_loop_cit */

struct se_wwn *tcm_loop_make_scsi_hba(
	struct target_fabric_configfs *tf,
	struct config_group *group,
	const char *name)
{
	struct tcm_loop_hba *tl_hba;
	struct Scsi_Host *sh;
	char *ptr;
	int ret, off = 0;

	tl_hba = kzalloc(sizeof(struct tcm_loop_hba), GFP_KERNEL);
	if (!tl_hba) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unable to allocate struct tcm_loop_hba\n");
=======
		pr_err("Unable to allocate struct tcm_loop_hba\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unable to allocate struct tcm_loop_hba\n");
>>>>>>> refs/remotes/origin/master
		return ERR_PTR(-ENOMEM);
	}
	/*
	 * Determine the emulated Protocol Identifier and Target Port Name
	 * based on the incoming configfs directory name.
	 */
	ptr = strstr(name, "naa.");
	if (ptr) {
		tl_hba->tl_proto_id = SCSI_PROTOCOL_SAS;
		goto check_len;
	}
	ptr = strstr(name, "fc.");
	if (ptr) {
		tl_hba->tl_proto_id = SCSI_PROTOCOL_FCP;
		off = 3; /* Skip over "fc." */
		goto check_len;
	}
	ptr = strstr(name, "iqn.");
<<<<<<< HEAD
<<<<<<< HEAD
	if (ptr) {
		tl_hba->tl_proto_id = SCSI_PROTOCOL_ISCSI;
		goto check_len;
	}

	printk(KERN_ERR "Unable to locate prefix for emulated Target Port:"
			" %s\n", name);
	return ERR_PTR(-EINVAL);

check_len:
	if (strlen(name) >= TL_WWN_ADDR_LEN) {
		printk(KERN_ERR "Emulated NAA %s Address: %s, exceeds"
			" max: %d\n", name, tcm_loop_dump_proto_id(tl_hba),
			TL_WWN_ADDR_LEN);
		kfree(tl_hba);
		return ERR_PTR(-EINVAL);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!ptr) {
		pr_err("Unable to locate prefix for emulated Target "
				"Port: %s\n", name);
		ret = -EINVAL;
		goto out;
	}
	tl_hba->tl_proto_id = SCSI_PROTOCOL_ISCSI;

check_len:
	if (strlen(name) >= TL_WWN_ADDR_LEN) {
		pr_err("Emulated NAA %s Address: %s, exceeds"
			" max: %d\n", name, tcm_loop_dump_proto_id(tl_hba),
			TL_WWN_ADDR_LEN);
		ret = -EINVAL;
		goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	snprintf(&tl_hba->tl_wwn_address[0], TL_WWN_ADDR_LEN, "%s", &name[off]);

	/*
	 * Call device_register(tl_hba->dev) to register the emulated
	 * Linux/SCSI LLD of type struct Scsi_Host at tl_hba->sh after
	 * device_register() callbacks in tcm_loop_driver_probe()
	 */
	ret = tcm_loop_setup_hba_bus(tl_hba, tcm_loop_hba_no_cnt);
	if (ret)
		goto out;

	sh = tl_hba->sh;
	tcm_loop_hba_no_cnt++;
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "TCM_Loop_ConfigFS: Allocated emulated Target"
=======
	pr_debug("TCM_Loop_ConfigFS: Allocated emulated Target"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("TCM_Loop_ConfigFS: Allocated emulated Target"
>>>>>>> refs/remotes/origin/master
		" %s Address: %s at Linux/SCSI Host ID: %d\n",
		tcm_loop_dump_proto_id(tl_hba), name, sh->host_no);

	return &tl_hba->tl_hba_wwn;
out:
	kfree(tl_hba);
	return ERR_PTR(ret);
}

void tcm_loop_drop_scsi_hba(
	struct se_wwn *wwn)
{
	struct tcm_loop_hba *tl_hba = container_of(wwn,
				struct tcm_loop_hba, tl_hba_wwn);
<<<<<<< HEAD
<<<<<<< HEAD
	int host_no = tl_hba->sh->host_no;
=======
=======
>>>>>>> refs/remotes/origin/master

	pr_debug("TCM_Loop_ConfigFS: Deallocating emulated Target"
		" SAS Address: %s at Linux/SCSI Host ID: %d\n",
		tl_hba->tl_wwn_address, tl_hba->sh->host_no);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Call device_unregister() on the original tl_hba->dev.
	 * tcm_loop_fabric_scsi.c:tcm_loop_release_adapter() will
	 * release *tl_hba;
	 */
	device_unregister(&tl_hba->dev);
<<<<<<< HEAD
<<<<<<< HEAD

	printk(KERN_INFO "TCM_Loop_ConfigFS: Deallocated emulated Target"
		" SAS Address: %s at Linux/SCSI Host ID: %d\n",
		config_item_name(&wwn->wwn_group.cg_item), host_no);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/* Start items for tcm_loop_cit */
static ssize_t tcm_loop_wwn_show_attr_version(
	struct target_fabric_configfs *tf,
	char *page)
{
	return sprintf(page, "TCM Loopback Fabric module %s\n", TCM_LOOP_VERSION);
}

TF_WWN_ATTR_RO(tcm_loop, version);

static struct configfs_attribute *tcm_loop_wwn_attrs[] = {
	&tcm_loop_wwn_version.attr,
	NULL,
};

/* End items for tcm_loop_cit */

static int tcm_loop_register_configfs(void)
{
	struct target_fabric_configfs *fabric;
<<<<<<< HEAD
<<<<<<< HEAD
	struct config_group *tf_cg;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int ret;
	/*
	 * Set the TCM Loop HBA counter to zero
	 */
	tcm_loop_hba_no_cnt = 0;
	/*
	 * Register the top level struct config_item_type with TCM core
	 */
	fabric = target_fabric_configfs_init(THIS_MODULE, "loopback");
<<<<<<< HEAD
<<<<<<< HEAD
	if (!fabric) {
		printk(KERN_ERR "tcm_loop_register_configfs() failed!\n");
		return -1;
=======
	if (IS_ERR(fabric)) {
		pr_err("tcm_loop_register_configfs() failed!\n");
		return PTR_ERR(fabric);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (IS_ERR(fabric)) {
		pr_err("tcm_loop_register_configfs() failed!\n");
		return PTR_ERR(fabric);
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * Setup the fabric API of function pointers used by target_core_mod
	 */
	fabric->tf_ops.get_fabric_name = &tcm_loop_get_fabric_name;
	fabric->tf_ops.get_fabric_proto_ident = &tcm_loop_get_fabric_proto_ident;
	fabric->tf_ops.tpg_get_wwn = &tcm_loop_get_endpoint_wwn;
	fabric->tf_ops.tpg_get_tag = &tcm_loop_get_tag;
	fabric->tf_ops.tpg_get_default_depth = &tcm_loop_get_default_depth;
	fabric->tf_ops.tpg_get_pr_transport_id = &tcm_loop_get_pr_transport_id;
	fabric->tf_ops.tpg_get_pr_transport_id_len =
					&tcm_loop_get_pr_transport_id_len;
	fabric->tf_ops.tpg_parse_pr_out_transport_id =
					&tcm_loop_parse_pr_out_transport_id;
	fabric->tf_ops.tpg_check_demo_mode = &tcm_loop_check_demo_mode;
	fabric->tf_ops.tpg_check_demo_mode_cache =
					&tcm_loop_check_demo_mode_cache;
	fabric->tf_ops.tpg_check_demo_mode_write_protect =
					&tcm_loop_check_demo_mode_write_protect;
	fabric->tf_ops.tpg_check_prod_mode_write_protect =
					&tcm_loop_check_prod_mode_write_protect;
	/*
	 * The TCM loopback fabric module runs in demo-mode to a local
	 * virtual SCSI device, so fabric dependent initator ACLs are
	 * not required.
	 */
	fabric->tf_ops.tpg_alloc_fabric_acl = &tcm_loop_tpg_alloc_fabric_acl;
	fabric->tf_ops.tpg_release_fabric_acl =
					&tcm_loop_tpg_release_fabric_acl;
	fabric->tf_ops.tpg_get_inst_index = &tcm_loop_get_inst_index;
	/*
<<<<<<< HEAD
<<<<<<< HEAD
	 * Since tcm_loop is mapping physical memory from Linux/SCSI
	 * struct scatterlist arrays for each struct scsi_cmnd I/O,
	 * we do not need TCM to allocate a iovec array for
	 * virtual memory address mappings
	 */
	fabric->tf_ops.alloc_cmd_iovecs = NULL;
	/*
	 * Used for setting up remaining TCM resources in process context
	 */
	fabric->tf_ops.new_cmd_map = &tcm_loop_new_cmd_map;
	fabric->tf_ops.check_stop_free = &tcm_loop_check_stop_free;
	fabric->tf_ops.release_cmd_to_pool = &tcm_loop_deallocate_core_cmd;
	fabric->tf_ops.release_cmd_direct = &tcm_loop_deallocate_core_cmd;
	fabric->tf_ops.shutdown_session = &tcm_loop_shutdown_session;
	fabric->tf_ops.close_session = &tcm_loop_close_session;
	fabric->tf_ops.stop_session = &tcm_loop_stop_session;
	fabric->tf_ops.fall_back_to_erl0 = &tcm_loop_fall_back_to_erl0;
	fabric->tf_ops.sess_logged_in = &tcm_loop_sess_logged_in;
=======
=======
>>>>>>> refs/remotes/origin/master
	 * Used for setting up remaining TCM resources in process context
	 */
	fabric->tf_ops.check_stop_free = &tcm_loop_check_stop_free;
	fabric->tf_ops.release_cmd = &tcm_loop_release_cmd;
	fabric->tf_ops.shutdown_session = &tcm_loop_shutdown_session;
	fabric->tf_ops.close_session = &tcm_loop_close_session;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	fabric->tf_ops.sess_get_index = &tcm_loop_sess_get_index;
	fabric->tf_ops.sess_get_initiator_sid = NULL;
	fabric->tf_ops.write_pending = &tcm_loop_write_pending;
	fabric->tf_ops.write_pending_status = &tcm_loop_write_pending_status;
	/*
	 * Not used for TCM loopback
	 */
	fabric->tf_ops.set_default_node_attributes =
					&tcm_loop_set_default_node_attributes;
	fabric->tf_ops.get_task_tag = &tcm_loop_get_task_tag;
	fabric->tf_ops.get_cmd_state = &tcm_loop_get_cmd_state;
<<<<<<< HEAD
<<<<<<< HEAD
	fabric->tf_ops.new_cmd_failure = &tcm_loop_new_cmd_failure;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	fabric->tf_ops.queue_data_in = &tcm_loop_queue_data_in;
	fabric->tf_ops.queue_status = &tcm_loop_queue_status;
	fabric->tf_ops.queue_tm_rsp = &tcm_loop_queue_tm_rsp;
	fabric->tf_ops.set_fabric_sense_len = &tcm_loop_set_fabric_sense_len;
	fabric->tf_ops.get_fabric_sense_len = &tcm_loop_get_fabric_sense_len;
<<<<<<< HEAD
	fabric->tf_ops.is_state_remove = &tcm_loop_is_state_remove;

	tf_cg = &fabric->tf_group;
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======
	fabric->tf_ops.queue_data_in = &tcm_loop_queue_data_in;
	fabric->tf_ops.queue_status = &tcm_loop_queue_status;
	fabric->tf_ops.queue_tm_rsp = &tcm_loop_queue_tm_rsp;

>>>>>>> refs/remotes/origin/master
	/*
	 * Setup function pointers for generic logic in target_core_fabric_configfs.c
	 */
	fabric->tf_ops.fabric_make_wwn = &tcm_loop_make_scsi_hba;
	fabric->tf_ops.fabric_drop_wwn = &tcm_loop_drop_scsi_hba;
	fabric->tf_ops.fabric_make_tpg = &tcm_loop_make_naa_tpg;
	fabric->tf_ops.fabric_drop_tpg = &tcm_loop_drop_naa_tpg;
	/*
	 * fabric_post_link() and fabric_pre_unlink() are used for
	 * registration and release of TCM Loop Virtual SCSI LUNs.
	 */
	fabric->tf_ops.fabric_post_link = &tcm_loop_port_link;
	fabric->tf_ops.fabric_pre_unlink = &tcm_loop_port_unlink;
	fabric->tf_ops.fabric_make_np = NULL;
	fabric->tf_ops.fabric_drop_np = NULL;
	/*
	 * Setup default attribute lists for various fabric->tf_cit_tmpl
	 */
<<<<<<< HEAD
	TF_CIT_TMPL(fabric)->tfc_wwn_cit.ct_attrs = tcm_loop_wwn_attrs;
	TF_CIT_TMPL(fabric)->tfc_tpg_base_cit.ct_attrs = tcm_loop_tpg_attrs;
	TF_CIT_TMPL(fabric)->tfc_tpg_attrib_cit.ct_attrs = NULL;
	TF_CIT_TMPL(fabric)->tfc_tpg_param_cit.ct_attrs = NULL;
	TF_CIT_TMPL(fabric)->tfc_tpg_np_base_cit.ct_attrs = NULL;
=======
	fabric->tf_cit_tmpl.tfc_wwn_cit.ct_attrs = tcm_loop_wwn_attrs;
	fabric->tf_cit_tmpl.tfc_tpg_base_cit.ct_attrs = tcm_loop_tpg_attrs;
	fabric->tf_cit_tmpl.tfc_tpg_attrib_cit.ct_attrs = NULL;
	fabric->tf_cit_tmpl.tfc_tpg_param_cit.ct_attrs = NULL;
	fabric->tf_cit_tmpl.tfc_tpg_np_base_cit.ct_attrs = NULL;
>>>>>>> refs/remotes/origin/master
	/*
	 * Once fabric->tf_ops has been setup, now register the fabric for
	 * use within TCM
	 */
	ret = target_fabric_configfs_register(fabric);
	if (ret < 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "target_fabric_configfs_register() for"
=======
		pr_err("target_fabric_configfs_register() for"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("target_fabric_configfs_register() for"
>>>>>>> refs/remotes/origin/master
				" TCM_Loop failed!\n");
		target_fabric_configfs_free(fabric);
		return -1;
	}
	/*
	 * Setup our local pointer to *fabric.
	 */
	tcm_loop_fabric_configfs = fabric;
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "TCM_LOOP[0] - Set fabric ->"
=======
	pr_debug("TCM_LOOP[0] - Set fabric ->"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("TCM_LOOP[0] - Set fabric ->"
>>>>>>> refs/remotes/origin/master
			" tcm_loop_fabric_configfs\n");
	return 0;
}

static void tcm_loop_deregister_configfs(void)
{
	if (!tcm_loop_fabric_configfs)
		return;

	target_fabric_configfs_deregister(tcm_loop_fabric_configfs);
	tcm_loop_fabric_configfs = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "TCM_LOOP[0] - Cleared"
=======
	pr_debug("TCM_LOOP[0] - Cleared"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("TCM_LOOP[0] - Cleared"
>>>>>>> refs/remotes/origin/master
				" tcm_loop_fabric_configfs\n");
}

static int __init tcm_loop_fabric_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int ret;
=======
=======
>>>>>>> refs/remotes/origin/master
	int ret = -ENOMEM;

	tcm_loop_workqueue = alloc_workqueue("tcm_loop", 0, 0);
	if (!tcm_loop_workqueue)
		goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	tcm_loop_cmd_cache = kmem_cache_create("tcm_loop_cmd_cache",
				sizeof(struct tcm_loop_cmd),
				__alignof__(struct tcm_loop_cmd),
				0, NULL);
	if (!tcm_loop_cmd_cache) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "kmem_cache_create() for"
			" tcm_loop_cmd_cache failed\n");
		return -ENOMEM;
=======
		pr_debug("kmem_cache_create() for"
			" tcm_loop_cmd_cache failed\n");
		goto out_destroy_workqueue;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_debug("kmem_cache_create() for"
			" tcm_loop_cmd_cache failed\n");
		goto out_destroy_workqueue;
>>>>>>> refs/remotes/origin/master
	}

	ret = tcm_loop_alloc_core_bus();
	if (ret)
<<<<<<< HEAD
<<<<<<< HEAD
		return ret;

	ret = tcm_loop_register_configfs();
	if (ret) {
		tcm_loop_release_core_bus();
		return ret;
	}

	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
		goto out_destroy_cache;

	ret = tcm_loop_register_configfs();
	if (ret)
		goto out_release_core_bus;

	return 0;

out_release_core_bus:
	tcm_loop_release_core_bus();
out_destroy_cache:
	kmem_cache_destroy(tcm_loop_cmd_cache);
out_destroy_workqueue:
	destroy_workqueue(tcm_loop_workqueue);
out:
	return ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void __exit tcm_loop_fabric_exit(void)
{
	tcm_loop_deregister_configfs();
	tcm_loop_release_core_bus();
	kmem_cache_destroy(tcm_loop_cmd_cache);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	destroy_workqueue(tcm_loop_workqueue);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	destroy_workqueue(tcm_loop_workqueue);
>>>>>>> refs/remotes/origin/master
}

MODULE_DESCRIPTION("TCM loopback virtual Linux/SCSI fabric module");
MODULE_AUTHOR("Nicholas A. Bellinger <nab@risingtidesystems.com>");
MODULE_LICENSE("GPL");
module_init(tcm_loop_fabric_init);
module_exit(tcm_loop_fabric_exit);
