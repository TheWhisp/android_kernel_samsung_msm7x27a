/*******************************************************************************
 * Filename:  target_core_device.c (based on iscsi_target_device.c)
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * This file contains the iSCSI Virtual Device and Disk Transport
=======
 * This file contains the TCM Virtual Device and Disk Transport
>>>>>>> refs/remotes/origin/cm-10.0
 * agnostic related functions.
 *
 * Copyright (c) 2003, 2004, 2005 PyX Technologies, Inc.
 * Copyright (c) 2005-2006 SBE, Inc.  All Rights Reserved.
 * Copyright (c) 2007-2010 Rising Tide Systems
 * Copyright (c) 2008-2010 Linux-iSCSI.org
=======
 * This file contains the TCM Virtual Device and Disk Transport
 * agnostic related functions.
 *
 * (c) Copyright 2003-2013 Datera, Inc.
>>>>>>> refs/remotes/origin/master
 *
 * Nicholas A. Bellinger <nab@kernel.org>
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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ******************************************************************************/

#include <linux/net.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/in.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <net/sock.h>
#include <net/tcp.h>
#include <scsi/scsi.h>
#include <scsi/scsi_device.h>

#include <target/target_core_base.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <target/target_core_device.h>
#include <target/target_core_tpg.h>
#include <target/target_core_transport.h>
#include <target/target_core_fabric_ops.h>

#include "target_core_alua.h"
#include "target_core_hba.h"
=======
=======
>>>>>>> refs/remotes/origin/master
#include <target/target_core_backend.h>
#include <target/target_core_fabric.h>

#include "target_core_internal.h"
#include "target_core_alua.h"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#include "target_core_pr.h"
#include "target_core_ua.h"

static void se_dev_start(struct se_device *dev);
static void se_dev_stop(struct se_device *dev);

<<<<<<< HEAD
int transport_get_lun_for_cmd(
	struct se_cmd *se_cmd,
	unsigned char *cdb,
	u32 unpacked_lun)
{
	struct se_dev_entry *deve;
	struct se_lun *se_lun = NULL;
	struct se_session *se_sess = SE_SESS(se_cmd);
	unsigned long flags;
	int read_only = 0;

	spin_lock_irq(&SE_NODE_ACL(se_sess)->device_list_lock);
	deve = se_cmd->se_deve =
			&SE_NODE_ACL(se_sess)->device_list[unpacked_lun];
	if (deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS) {
		if (se_cmd) {
			deve->total_cmds++;
			deve->total_bytes += se_cmd->data_length;

			if (se_cmd->data_direction == DMA_TO_DEVICE) {
				if (deve->lun_flags &
						TRANSPORT_LUNFLAGS_READ_ONLY) {
					read_only = 1;
					goto out;
				}
				deve->write_bytes += se_cmd->data_length;
			} else if (se_cmd->data_direction ==
				   DMA_FROM_DEVICE) {
				deve->read_bytes += se_cmd->data_length;
			}
		}
		deve->deve_cmds++;

		se_lun = se_cmd->se_lun = deve->se_lun;
		se_cmd->pr_res_key = deve->pr_res_key;
		se_cmd->orig_fe_lun = unpacked_lun;
		se_cmd->se_orig_obj_ptr = SE_LUN(se_cmd)->lun_se_dev;
		se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
	}
out:
	spin_unlock_irq(&SE_NODE_ACL(se_sess)->device_list_lock);

	if (!se_lun) {
		if (read_only) {
			se_cmd->scsi_sense_reason = TCM_WRITE_PROTECTED;
			se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
			printk("TARGET_CORE[%s]: Detected WRITE_PROTECTED LUN"
				" Access for 0x%08x\n",
				CMD_TFO(se_cmd)->get_fabric_name(),
				unpacked_lun);
			return -1;
		} else {
			/*
			 * Use the se_portal_group->tpg_virt_lun0 to allow for
			 * REPORT_LUNS, et al to be returned when no active
			 * MappedLUN=0 exists for this Initiator Port.
			 */
			if (unpacked_lun != 0) {
				se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
				se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
				printk("TARGET_CORE[%s]: Detected NON_EXISTENT_LUN"
					" Access for 0x%08x\n",
					CMD_TFO(se_cmd)->get_fabric_name(),
					unpacked_lun);
				return -1;
			}
			/*
			 * Force WRITE PROTECT for virtual LUN 0
			 */
			if ((se_cmd->data_direction != DMA_FROM_DEVICE) &&
			    (se_cmd->data_direction != DMA_NONE)) {
				se_cmd->scsi_sense_reason = TCM_WRITE_PROTECTED;
				se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
				return -1;
			}
#if 0
			printk("TARGET_CORE[%s]: Using virtual LUN0! :-)\n",
				CMD_TFO(se_cmd)->get_fabric_name());
#endif
			se_lun = se_cmd->se_lun = &se_sess->se_tpg->tpg_virt_lun0;
			se_cmd->orig_fe_lun = 0;
			se_cmd->se_orig_obj_ptr = SE_LUN(se_cmd)->lun_se_dev;
			se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
		}
	}
	/*
	 * Determine if the struct se_lun is online.
	 */
/* #warning FIXME: Check for LUN_RESET + UNIT Attention */
	if (se_dev_check_online(se_lun->lun_se_dev) != 0) {
		se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -1;
	}

	{
	struct se_device *dev = se_lun->lun_se_dev;
	spin_lock_irq(&dev->stats_lock);
=======
static struct se_hba *lun0_hba;
static struct se_subsystem_dev *lun0_su_dev;
/* not static, needed by tpg.c */
struct se_device *g_lun0_dev;

int transport_lookup_cmd_lun(struct se_cmd *se_cmd, u32 unpacked_lun)
=======
#include "target_core_pr.h"
#include "target_core_ua.h"

DEFINE_MUTEX(g_device_mutex);
LIST_HEAD(g_device_list);

static struct se_hba *lun0_hba;
/* not static, needed by tpg.c */
struct se_device *g_lun0_dev;

sense_reason_t
transport_lookup_cmd_lun(struct se_cmd *se_cmd, u32 unpacked_lun)
>>>>>>> refs/remotes/origin/master
{
	struct se_lun *se_lun = NULL;
	struct se_session *se_sess = se_cmd->se_sess;
	struct se_device *dev;
	unsigned long flags;

<<<<<<< HEAD
	if (unpacked_lun >= TRANSPORT_MAX_LUNS_PER_TPG) {
		se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -ENODEV;
	}
=======
	if (unpacked_lun >= TRANSPORT_MAX_LUNS_PER_TPG)
		return TCM_NON_EXISTENT_LUN;
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&se_sess->se_node_acl->device_list_lock, flags);
	se_cmd->se_deve = se_sess->se_node_acl->device_list[unpacked_lun];
	if (se_cmd->se_deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS) {
		struct se_dev_entry *deve = se_cmd->se_deve;

		deve->total_cmds++;
<<<<<<< HEAD
		deve->total_bytes += se_cmd->data_length;

		if ((se_cmd->data_direction == DMA_TO_DEVICE) &&
		    (deve->lun_flags & TRANSPORT_LUNFLAGS_READ_ONLY)) {
			se_cmd->scsi_sense_reason = TCM_WRITE_PROTECTED;
			se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
=======

		if ((se_cmd->data_direction == DMA_TO_DEVICE) &&
		    (deve->lun_flags & TRANSPORT_LUNFLAGS_READ_ONLY)) {
>>>>>>> refs/remotes/origin/master
			pr_err("TARGET_CORE[%s]: Detected WRITE_PROTECTED LUN"
				" Access for 0x%08x\n",
				se_cmd->se_tfo->get_fabric_name(),
				unpacked_lun);
			spin_unlock_irqrestore(&se_sess->se_node_acl->device_list_lock, flags);
<<<<<<< HEAD
			return -EACCES;
=======
			return TCM_WRITE_PROTECTED;
>>>>>>> refs/remotes/origin/master
		}

		if (se_cmd->data_direction == DMA_TO_DEVICE)
			deve->write_bytes += se_cmd->data_length;
		else if (se_cmd->data_direction == DMA_FROM_DEVICE)
			deve->read_bytes += se_cmd->data_length;

<<<<<<< HEAD
		deve->deve_cmds++;

=======
>>>>>>> refs/remotes/origin/master
		se_lun = deve->se_lun;
		se_cmd->se_lun = deve->se_lun;
		se_cmd->pr_res_key = deve->pr_res_key;
		se_cmd->orig_fe_lun = unpacked_lun;
		se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
<<<<<<< HEAD
=======

		percpu_ref_get(&se_lun->lun_ref);
		se_cmd->lun_ref_active = true;
>>>>>>> refs/remotes/origin/master
	}
	spin_unlock_irqrestore(&se_sess->se_node_acl->device_list_lock, flags);

	if (!se_lun) {
		/*
		 * Use the se_portal_group->tpg_virt_lun0 to allow for
		 * REPORT_LUNS, et al to be returned when no active
		 * MappedLUN=0 exists for this Initiator Port.
		 */
		if (unpacked_lun != 0) {
<<<<<<< HEAD
			se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
			se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
=======
>>>>>>> refs/remotes/origin/master
			pr_err("TARGET_CORE[%s]: Detected NON_EXISTENT_LUN"
				" Access for 0x%08x\n",
				se_cmd->se_tfo->get_fabric_name(),
				unpacked_lun);
<<<<<<< HEAD
			return -ENODEV;
=======
			return TCM_NON_EXISTENT_LUN;
>>>>>>> refs/remotes/origin/master
		}
		/*
		 * Force WRITE PROTECT for virtual LUN 0
		 */
		if ((se_cmd->data_direction != DMA_FROM_DEVICE) &&
<<<<<<< HEAD
		    (se_cmd->data_direction != DMA_NONE)) {
			se_cmd->scsi_sense_reason = TCM_WRITE_PROTECTED;
			se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
			return -EACCES;
		}
=======
		    (se_cmd->data_direction != DMA_NONE))
			return TCM_WRITE_PROTECTED;
>>>>>>> refs/remotes/origin/master

		se_lun = &se_sess->se_tpg->tpg_virt_lun0;
		se_cmd->se_lun = &se_sess->se_tpg->tpg_virt_lun0;
		se_cmd->orig_fe_lun = 0;
		se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
<<<<<<< HEAD
	}
	/*
	 * Determine if the struct se_lun is online.
	 * FIXME: Check for LUN_RESET + UNIT Attention
	 */
	if (se_dev_check_online(se_lun->lun_se_dev) != 0) {
		se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -ENODEV;
=======

		percpu_ref_get(&se_lun->lun_ref);
		se_cmd->lun_ref_active = true;
>>>>>>> refs/remotes/origin/master
	}

	/* Directly associate cmd with se_dev */
	se_cmd->se_dev = se_lun->lun_se_dev;

<<<<<<< HEAD
	/* TODO: get rid of this and use atomics for stats */
	dev = se_lun->lun_se_dev;
	spin_lock_irqsave(&dev->stats_lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
	dev->num_cmds++;
	if (se_cmd->data_direction == DMA_TO_DEVICE)
		dev->write_bytes += se_cmd->data_length;
	else if (se_cmd->data_direction == DMA_FROM_DEVICE)
		dev->read_bytes += se_cmd->data_length;
<<<<<<< HEAD
	spin_unlock_irq(&dev->stats_lock);
	}

	/*
	 * Add the iscsi_cmd_t to the struct se_lun's cmd list.  This list is used
	 * for tracking state of struct se_cmds during LUN shutdown events.
	 */
	spin_lock_irqsave(&se_lun->lun_cmd_lock, flags);
	list_add_tail(&se_cmd->se_lun_list, &se_lun->lun_cmd_list);
	atomic_set(&T_TASK(se_cmd)->transport_lun_active, 1);
#if 0
	printk(KERN_INFO "Adding ITT: 0x%08x to LUN LIST[%d]\n",
		CMD_TFO(se_cmd)->get_task_tag(se_cmd), se_lun->unpacked_lun);
#endif
=======
	spin_unlock_irqrestore(&dev->stats_lock, flags);

	spin_lock_irqsave(&se_lun->lun_cmd_lock, flags);
	list_add_tail(&se_cmd->se_lun_node, &se_lun->lun_cmd_list);
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irqrestore(&se_lun->lun_cmd_lock, flags);

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(transport_get_lun_for_cmd);

int transport_get_lun_for_tmr(
	struct se_cmd *se_cmd,
	u32 unpacked_lun)
{
	struct se_device *dev = NULL;
	struct se_dev_entry *deve;
	struct se_lun *se_lun = NULL;
	struct se_session *se_sess = SE_SESS(se_cmd);
	struct se_tmr_req *se_tmr = se_cmd->se_tmr_req;

	spin_lock_irq(&SE_NODE_ACL(se_sess)->device_list_lock);
	deve = se_cmd->se_deve =
			&SE_NODE_ACL(se_sess)->device_list[unpacked_lun];
	if (deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS) {
		se_lun = se_cmd->se_lun = se_tmr->tmr_lun = deve->se_lun;
		dev = se_lun->lun_se_dev;
		se_cmd->pr_res_key = deve->pr_res_key;
		se_cmd->orig_fe_lun = unpacked_lun;
		se_cmd->se_orig_obj_ptr = SE_LUN(se_cmd)->lun_se_dev;
/*		se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD; */
	}
	spin_unlock_irq(&SE_NODE_ACL(se_sess)->device_list_lock);

	if (!se_lun) {
		printk(KERN_INFO "TARGET_CORE[%s]: Detected NON_EXISTENT_LUN"
			" Access for 0x%08x\n",
			CMD_TFO(se_cmd)->get_fabric_name(),
			unpacked_lun);
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -1;
	}
	/*
	 * Determine if the struct se_lun is online.
	 */
/* #warning FIXME: Check for LUN_RESET + UNIT Attention */
	if (se_dev_check_online(se_lun->lun_se_dev) != 0) {
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -1;
	}
	se_tmr->tmr_dev = dev;

	spin_lock(&dev->se_tmr_lock);
	list_add_tail(&se_tmr->tmr_list, &dev->dev_tmr_list);
	spin_unlock(&dev->se_tmr_lock);

	return 0;
}
EXPORT_SYMBOL(transport_get_lun_for_tmr);
=======
=======
	dev = se_lun->lun_se_dev;
	atomic_long_inc(&dev->num_cmds);
	if (se_cmd->data_direction == DMA_TO_DEVICE)
		atomic_long_add(se_cmd->data_length, &dev->write_bytes);
	else if (se_cmd->data_direction == DMA_FROM_DEVICE)
		atomic_long_add(se_cmd->data_length, &dev->read_bytes);

	return 0;
}
>>>>>>> refs/remotes/origin/master
EXPORT_SYMBOL(transport_lookup_cmd_lun);

int transport_lookup_tmr_lun(struct se_cmd *se_cmd, u32 unpacked_lun)
{
	struct se_dev_entry *deve;
	struct se_lun *se_lun = NULL;
	struct se_session *se_sess = se_cmd->se_sess;
	struct se_tmr_req *se_tmr = se_cmd->se_tmr_req;
	unsigned long flags;

<<<<<<< HEAD
	if (unpacked_lun >= TRANSPORT_MAX_LUNS_PER_TPG) {
		se_cmd->scsi_sense_reason = TCM_NON_EXISTENT_LUN;
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -ENODEV;
	}
=======
	if (unpacked_lun >= TRANSPORT_MAX_LUNS_PER_TPG)
		return -ENODEV;
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&se_sess->se_node_acl->device_list_lock, flags);
	se_cmd->se_deve = se_sess->se_node_acl->device_list[unpacked_lun];
	deve = se_cmd->se_deve;

	if (deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS) {
		se_tmr->tmr_lun = deve->se_lun;
		se_cmd->se_lun = deve->se_lun;
		se_lun = deve->se_lun;
		se_cmd->pr_res_key = deve->pr_res_key;
		se_cmd->orig_fe_lun = unpacked_lun;
	}
	spin_unlock_irqrestore(&se_sess->se_node_acl->device_list_lock, flags);

	if (!se_lun) {
		pr_debug("TARGET_CORE[%s]: Detected NON_EXISTENT_LUN"
			" Access for 0x%08x\n",
			se_cmd->se_tfo->get_fabric_name(),
			unpacked_lun);
<<<<<<< HEAD
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
		return -ENODEV;
	}
	/*
	 * Determine if the struct se_lun is online.
	 * FIXME: Check for LUN_RESET + UNIT Attention
	 */
	if (se_dev_check_online(se_lun->lun_se_dev) != 0) {
		se_cmd->se_cmd_flags |= SCF_SCSI_CDB_EXCEPTION;
=======
>>>>>>> refs/remotes/origin/master
		return -ENODEV;
	}

	/* Directly associate cmd with se_dev */
	se_cmd->se_dev = se_lun->lun_se_dev;
	se_tmr->tmr_dev = se_lun->lun_se_dev;

	spin_lock_irqsave(&se_tmr->tmr_dev->se_tmr_lock, flags);
	list_add_tail(&se_tmr->tmr_list, &se_tmr->tmr_dev->dev_tmr_list);
	spin_unlock_irqrestore(&se_tmr->tmr_dev->se_tmr_lock, flags);

	return 0;
}
EXPORT_SYMBOL(transport_lookup_tmr_lun);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * This function is called from core_scsi3_emulate_pro_register_and_move()
 * and core_scsi3_decode_spec_i_port(), and will increment &deve->pr_ref_count
 * when a matching rtpi is found.
 */
struct se_dev_entry *core_get_se_deve_from_rtpi(
	struct se_node_acl *nacl,
	u16 rtpi)
{
	struct se_dev_entry *deve;
	struct se_lun *lun;
	struct se_port *port;
	struct se_portal_group *tpg = nacl->se_tpg;
	u32 i;

	spin_lock_irq(&nacl->device_list_lock);
	for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		deve = &nacl->device_list[i];
=======
		deve = nacl->device_list[i];
>>>>>>> refs/remotes/origin/cm-10.0
=======
		deve = nacl->device_list[i];
>>>>>>> refs/remotes/origin/master

		if (!(deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS))
			continue;

		lun = deve->se_lun;
<<<<<<< HEAD
<<<<<<< HEAD
		if (!(lun)) {
			printk(KERN_ERR "%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
				TPG_TFO(tpg)->get_fabric_name());
			continue;
		}
		port = lun->lun_sep;
		if (!(port)) {
			printk(KERN_ERR "%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
				TPG_TFO(tpg)->get_fabric_name());
=======
=======
>>>>>>> refs/remotes/origin/master
		if (!lun) {
			pr_err("%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
				tpg->se_tpg_tfo->get_fabric_name());
			continue;
		}
		port = lun->lun_sep;
		if (!port) {
			pr_err("%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
				tpg->se_tpg_tfo->get_fabric_name());
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			continue;
		}
		if (port->sep_rtpi != rtpi)
			continue;

		atomic_inc(&deve->pr_ref_count);
		smp_mb__after_atomic_inc();
		spin_unlock_irq(&nacl->device_list_lock);

		return deve;
	}
	spin_unlock_irq(&nacl->device_list_lock);

	return NULL;
}

int core_free_device_list_for_node(
	struct se_node_acl *nacl,
	struct se_portal_group *tpg)
{
	struct se_dev_entry *deve;
	struct se_lun *lun;
	u32 i;

	if (!nacl->device_list)
		return 0;

	spin_lock_irq(&nacl->device_list_lock);
	for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		deve = &nacl->device_list[i];
=======
		deve = nacl->device_list[i];
>>>>>>> refs/remotes/origin/cm-10.0
=======
		deve = nacl->device_list[i];
>>>>>>> refs/remotes/origin/master

		if (!(deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS))
			continue;

		if (!deve->se_lun) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_ERR "%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
				TPG_TFO(tpg)->get_fabric_name());
=======
			pr_err("%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
				tpg->se_tpg_tfo->get_fabric_name());
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_err("%s device entries device pointer is"
				" NULL, but Initiator has access.\n",
				tpg->se_tpg_tfo->get_fabric_name());
>>>>>>> refs/remotes/origin/master
			continue;
		}
		lun = deve->se_lun;

		spin_unlock_irq(&nacl->device_list_lock);
<<<<<<< HEAD
		core_update_device_list_for_node(lun, NULL, deve->mapped_lun,
			TRANSPORT_LUNFLAGS_NO_ACCESS, nacl, tpg, 0);
=======
		core_disable_device_list_for_node(lun, NULL, deve->mapped_lun,
			TRANSPORT_LUNFLAGS_NO_ACCESS, nacl, tpg);
>>>>>>> refs/remotes/origin/master
		spin_lock_irq(&nacl->device_list_lock);
	}
	spin_unlock_irq(&nacl->device_list_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	kfree(nacl->device_list);
=======
	array_free(nacl->device_list, TRANSPORT_MAX_LUNS_PER_TPG);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	array_free(nacl->device_list, TRANSPORT_MAX_LUNS_PER_TPG);
>>>>>>> refs/remotes/origin/master
	nacl->device_list = NULL;

	return 0;
}

<<<<<<< HEAD
void core_dec_lacl_count(struct se_node_acl *se_nacl, struct se_cmd *se_cmd)
{
	struct se_dev_entry *deve;
<<<<<<< HEAD

	spin_lock_irq(&se_nacl->device_list_lock);
	deve = &se_nacl->device_list[se_cmd->orig_fe_lun];
	deve->deve_cmds--;
	spin_unlock_irq(&se_nacl->device_list_lock);

	return;
=======
	unsigned long flags;

	spin_lock_irqsave(&se_nacl->device_list_lock, flags);
	deve = se_nacl->device_list[se_cmd->orig_fe_lun];
	deve->deve_cmds--;
	spin_unlock_irqrestore(&se_nacl->device_list_lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
>>>>>>> refs/remotes/origin/master
void core_update_device_list_access(
	u32 mapped_lun,
	u32 lun_access,
	struct se_node_acl *nacl)
{
	struct se_dev_entry *deve;

	spin_lock_irq(&nacl->device_list_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	deve = &nacl->device_list[mapped_lun];
=======
	deve = nacl->device_list[mapped_lun];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	deve = nacl->device_list[mapped_lun];
>>>>>>> refs/remotes/origin/master
	if (lun_access & TRANSPORT_LUNFLAGS_READ_WRITE) {
		deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_ONLY;
		deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_WRITE;
	} else {
		deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_WRITE;
		deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_ONLY;
	}
	spin_unlock_irq(&nacl->device_list_lock);
<<<<<<< HEAD
<<<<<<< HEAD

	return;
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

/*      core_update_device_list_for_node():
 *
 *
 */
int core_update_device_list_for_node(
=======
}

/*      core_enable_device_list_for_node():
 *
 *
 */
int core_enable_device_list_for_node(
>>>>>>> refs/remotes/origin/master
	struct se_lun *lun,
	struct se_lun_acl *lun_acl,
	u32 mapped_lun,
	u32 lun_access,
	struct se_node_acl *nacl,
<<<<<<< HEAD
	struct se_portal_group *tpg,
	int enable)
{
	struct se_port *port = lun->lun_sep;
<<<<<<< HEAD
	struct se_dev_entry *deve = &nacl->device_list[mapped_lun];
=======
	struct se_dev_entry *deve = nacl->device_list[mapped_lun];
>>>>>>> refs/remotes/origin/cm-10.0
	int trans = 0;
	/*
	 * If the MappedLUN entry is being disabled, the entry in
	 * port->sep_alua_list must be removed now before clearing the
	 * struct se_dev_entry pointers below as logic in
	 * core_alua_do_transition_tg_pt() depends on these being present.
	 */
<<<<<<< HEAD
	if (!(enable)) {
=======
	if (!enable) {
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * deve->se_lun_acl will be NULL for demo-mode created LUNs
		 * that have not been explicitly concerted to MappedLUNs ->
		 * struct se_lun_acl, but we remove deve->alua_port_list from
		 * port->sep_alua_list. This also means that active UAs and
		 * NodeACL context specific PR metadata for demo-mode
		 * MappedLUN *deve will be released below..
		 */
		spin_lock_bh(&port->sep_alua_lock);
		list_del(&deve->alua_port_list);
		spin_unlock_bh(&port->sep_alua_lock);
	}

	spin_lock_irq(&nacl->device_list_lock);
	if (enable) {
		/*
		 * Check if the call is handling demo mode -> explict LUN ACL
		 * transition.  This transition must be for the same struct se_lun
		 * + mapped_lun that was setup in demo mode..
		 */
		if (deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS) {
			if (deve->se_lun_acl != NULL) {
<<<<<<< HEAD
				printk(KERN_ERR "struct se_dev_entry->se_lun_acl"
					" already set for demo mode -> explict"
					" LUN ACL transition\n");
				spin_unlock_irq(&nacl->device_list_lock);
				return -1;
			}
			if (deve->se_lun != lun) {
				printk(KERN_ERR "struct se_dev_entry->se_lun does"
					" match passed struct se_lun for demo mode"
					" -> explict LUN ACL transition\n");
				spin_unlock_irq(&nacl->device_list_lock);
				return -1;
=======
				pr_err("struct se_dev_entry->se_lun_acl"
					" already set for demo mode -> explict"
					" LUN ACL transition\n");
				spin_unlock_irq(&nacl->device_list_lock);
				return -EINVAL;
			}
			if (deve->se_lun != lun) {
				pr_err("struct se_dev_entry->se_lun does"
					" match passed struct se_lun for demo mode"
					" -> explict LUN ACL transition\n");
				spin_unlock_irq(&nacl->device_list_lock);
				return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
			}
			deve->se_lun_acl = lun_acl;
			trans = 1;
		} else {
			deve->se_lun = lun;
			deve->se_lun_acl = lun_acl;
			deve->mapped_lun = mapped_lun;
			deve->lun_flags |= TRANSPORT_LUNFLAGS_INITIATOR_ACCESS;
		}
=======
	struct se_portal_group *tpg)
{
	struct se_port *port = lun->lun_sep;
	struct se_dev_entry *deve;

	spin_lock_irq(&nacl->device_list_lock);

	deve = nacl->device_list[mapped_lun];

	/*
	 * Check if the call is handling demo mode -> explicit LUN ACL
	 * transition.  This transition must be for the same struct se_lun
	 * + mapped_lun that was setup in demo mode..
	 */
	if (deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS) {
		if (deve->se_lun_acl != NULL) {
			pr_err("struct se_dev_entry->se_lun_acl"
			       " already set for demo mode -> explicit"
			       " LUN ACL transition\n");
			spin_unlock_irq(&nacl->device_list_lock);
			return -EINVAL;
		}
		if (deve->se_lun != lun) {
			pr_err("struct se_dev_entry->se_lun does"
			       " match passed struct se_lun for demo mode"
			       " -> explicit LUN ACL transition\n");
			spin_unlock_irq(&nacl->device_list_lock);
			return -EINVAL;
		}
		deve->se_lun_acl = lun_acl;
>>>>>>> refs/remotes/origin/master

		if (lun_access & TRANSPORT_LUNFLAGS_READ_WRITE) {
			deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_ONLY;
			deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_WRITE;
		} else {
			deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_WRITE;
			deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_ONLY;
		}

<<<<<<< HEAD
		if (trans) {
			spin_unlock_irq(&nacl->device_list_lock);
			return 0;
		}
		deve->creation_time = get_jiffies_64();
		deve->attach_count++;
		spin_unlock_irq(&nacl->device_list_lock);

		spin_lock_bh(&port->sep_alua_lock);
		list_add_tail(&deve->alua_port_list, &port->sep_alua_list);
		spin_unlock_bh(&port->sep_alua_lock);

		return 0;
	}
=======
		spin_unlock_irq(&nacl->device_list_lock);
		return 0;
	}

	deve->se_lun = lun;
	deve->se_lun_acl = lun_acl;
	deve->mapped_lun = mapped_lun;
	deve->lun_flags |= TRANSPORT_LUNFLAGS_INITIATOR_ACCESS;

	if (lun_access & TRANSPORT_LUNFLAGS_READ_WRITE) {
		deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_ONLY;
		deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_WRITE;
	} else {
		deve->lun_flags &= ~TRANSPORT_LUNFLAGS_READ_WRITE;
		deve->lun_flags |= TRANSPORT_LUNFLAGS_READ_ONLY;
	}

	deve->creation_time = get_jiffies_64();
	deve->attach_count++;
	spin_unlock_irq(&nacl->device_list_lock);

	spin_lock_bh(&port->sep_alua_lock);
	list_add_tail(&deve->alua_port_list, &port->sep_alua_list);
	spin_unlock_bh(&port->sep_alua_lock);

	return 0;
}

/*      core_disable_device_list_for_node():
 *
 *
 */
int core_disable_device_list_for_node(
	struct se_lun *lun,
	struct se_lun_acl *lun_acl,
	u32 mapped_lun,
	u32 lun_access,
	struct se_node_acl *nacl,
	struct se_portal_group *tpg)
{
	struct se_port *port = lun->lun_sep;
	struct se_dev_entry *deve = nacl->device_list[mapped_lun];

	/*
	 * If the MappedLUN entry is being disabled, the entry in
	 * port->sep_alua_list must be removed now before clearing the
	 * struct se_dev_entry pointers below as logic in
	 * core_alua_do_transition_tg_pt() depends on these being present.
	 *
	 * deve->se_lun_acl will be NULL for demo-mode created LUNs
	 * that have not been explicitly converted to MappedLUNs ->
	 * struct se_lun_acl, but we remove deve->alua_port_list from
	 * port->sep_alua_list. This also means that active UAs and
	 * NodeACL context specific PR metadata for demo-mode
	 * MappedLUN *deve will be released below..
	 */
	spin_lock_bh(&port->sep_alua_lock);
	list_del(&deve->alua_port_list);
	spin_unlock_bh(&port->sep_alua_lock);
>>>>>>> refs/remotes/origin/master
	/*
	 * Wait for any in process SPEC_I_PT=1 or REGISTER_AND_MOVE
	 * PR operation to complete.
	 */
<<<<<<< HEAD
	spin_unlock_irq(&nacl->device_list_lock);
	while (atomic_read(&deve->pr_ref_count) != 0)
		cpu_relax();
=======
	while (atomic_read(&deve->pr_ref_count) != 0)
		cpu_relax();

>>>>>>> refs/remotes/origin/master
	spin_lock_irq(&nacl->device_list_lock);
	/*
	 * Disable struct se_dev_entry LUN ACL mapping
	 */
	core_scsi3_ua_release_all(deve);
	deve->se_lun = NULL;
	deve->se_lun_acl = NULL;
	deve->lun_flags = 0;
	deve->creation_time = 0;
	deve->attach_count--;
	spin_unlock_irq(&nacl->device_list_lock);

	core_scsi3_free_pr_reg_from_nacl(lun->lun_se_dev, nacl);
	return 0;
}

/*      core_clear_lun_from_tpg():
 *
 *
 */
void core_clear_lun_from_tpg(struct se_lun *lun, struct se_portal_group *tpg)
{
	struct se_node_acl *nacl;
	struct se_dev_entry *deve;
	u32 i;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_bh(&tpg->acl_node_lock);
	list_for_each_entry(nacl, &tpg->acl_node_list, acl_list) {
		spin_unlock_bh(&tpg->acl_node_lock);

		spin_lock_irq(&nacl->device_list_lock);
		for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
			deve = &nacl->device_list[i];
=======
=======
>>>>>>> refs/remotes/origin/master
	spin_lock_irq(&tpg->acl_node_lock);
	list_for_each_entry(nacl, &tpg->acl_node_list, acl_list) {
		spin_unlock_irq(&tpg->acl_node_lock);

		spin_lock_irq(&nacl->device_list_lock);
		for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
			deve = nacl->device_list[i];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			if (lun != deve->se_lun)
				continue;
			spin_unlock_irq(&nacl->device_list_lock);

<<<<<<< HEAD
			core_update_device_list_for_node(lun, NULL,
				deve->mapped_lun, TRANSPORT_LUNFLAGS_NO_ACCESS,
				nacl, tpg, 0);
=======
			core_disable_device_list_for_node(lun, NULL,
				deve->mapped_lun, TRANSPORT_LUNFLAGS_NO_ACCESS,
				nacl, tpg);
>>>>>>> refs/remotes/origin/master

			spin_lock_irq(&nacl->device_list_lock);
		}
		spin_unlock_irq(&nacl->device_list_lock);

<<<<<<< HEAD
<<<<<<< HEAD
		spin_lock_bh(&tpg->acl_node_lock);
	}
	spin_unlock_bh(&tpg->acl_node_lock);

	return;
=======
		spin_lock_irq(&tpg->acl_node_lock);
	}
	spin_unlock_irq(&tpg->acl_node_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_lock_irq(&tpg->acl_node_lock);
	}
	spin_unlock_irq(&tpg->acl_node_lock);
>>>>>>> refs/remotes/origin/master
}

static struct se_port *core_alloc_port(struct se_device *dev)
{
	struct se_port *port, *port_tmp;

	port = kzalloc(sizeof(struct se_port), GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(port)) {
		printk(KERN_ERR "Unable to allocate struct se_port\n");
		return NULL;
=======
	if (!port) {
		pr_err("Unable to allocate struct se_port\n");
		return ERR_PTR(-ENOMEM);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!port) {
		pr_err("Unable to allocate struct se_port\n");
		return ERR_PTR(-ENOMEM);
>>>>>>> refs/remotes/origin/master
	}
	INIT_LIST_HEAD(&port->sep_alua_list);
	INIT_LIST_HEAD(&port->sep_list);
	atomic_set(&port->sep_tg_pt_secondary_offline, 0);
	spin_lock_init(&port->sep_alua_lock);
	mutex_init(&port->sep_tg_pt_md_mutex);

	spin_lock(&dev->se_port_lock);
	if (dev->dev_port_count == 0x0000ffff) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_WARNING "Reached dev->dev_port_count =="
				" 0x0000ffff\n");
		spin_unlock(&dev->se_port_lock);
		return NULL;
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_warn("Reached dev->dev_port_count =="
				" 0x0000ffff\n");
		spin_unlock(&dev->se_port_lock);
		return ERR_PTR(-ENOSPC);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}
again:
	/*
	 * Allocate the next RELATIVE TARGET PORT IDENTIFER for this struct se_device
=======
	}
again:
	/*
	 * Allocate the next RELATIVE TARGET PORT IDENTIFIER for this struct se_device
>>>>>>> refs/remotes/origin/master
	 * Here is the table from spc4r17 section 7.7.3.8.
	 *
	 *    Table 473 -- RELATIVE TARGET PORT IDENTIFIER field
	 *
	 * Code      Description
	 * 0h        Reserved
	 * 1h        Relative port 1, historically known as port A
	 * 2h        Relative port 2, historically known as port B
	 * 3h to FFFFh    Relative port 3 through 65 535
	 */
	port->sep_rtpi = dev->dev_rpti_counter++;
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(port->sep_rtpi))
=======
	if (!port->sep_rtpi)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!port->sep_rtpi)
>>>>>>> refs/remotes/origin/master
		goto again;

	list_for_each_entry(port_tmp, &dev->dev_sep_list, sep_list) {
		/*
<<<<<<< HEAD
		 * Make sure RELATIVE TARGET PORT IDENTIFER is unique
=======
		 * Make sure RELATIVE TARGET PORT IDENTIFIER is unique
>>>>>>> refs/remotes/origin/master
		 * for 16-bit wrap..
		 */
		if (port->sep_rtpi == port_tmp->sep_rtpi)
			goto again;
	}
	spin_unlock(&dev->se_port_lock);

	return port;
}

static void core_export_port(
	struct se_device *dev,
	struct se_portal_group *tpg,
	struct se_port *port,
	struct se_lun *lun)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = SU_DEV(dev);
=======
	struct se_subsystem_dev *su_dev = dev->se_sub_dev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem = NULL;

	spin_lock(&dev->se_port_lock);
	spin_lock(&lun->lun_sep_lock);
	port->sep_tpg = tpg;
	port->sep_lun = lun;
	lun->lun_sep = port;
	spin_unlock(&lun->lun_sep_lock);

	list_add_tail(&port->sep_list, &dev->dev_sep_list);
	spin_unlock(&dev->se_port_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	if (T10_ALUA(su_dev)->alua_type == SPC3_ALUA_EMULATED) {
		tg_pt_gp_mem = core_alua_allocate_tg_pt_gp_mem(port);
		if (IS_ERR(tg_pt_gp_mem) || !tg_pt_gp_mem) {
			printk(KERN_ERR "Unable to allocate t10_alua_tg_pt"
=======
	if (su_dev->t10_alua.alua_type == SPC3_ALUA_EMULATED) {
		tg_pt_gp_mem = core_alua_allocate_tg_pt_gp_mem(port);
		if (IS_ERR(tg_pt_gp_mem) || !tg_pt_gp_mem) {
			pr_err("Unable to allocate t10_alua_tg_pt"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (dev->transport->transport_type != TRANSPORT_PLUGIN_PHBA_PDEV &&
	    !(dev->se_hba->hba_flags & HBA_FLAGS_INTERNAL_USE)) {
		tg_pt_gp_mem = core_alua_allocate_tg_pt_gp_mem(port);
		if (IS_ERR(tg_pt_gp_mem) || !tg_pt_gp_mem) {
			pr_err("Unable to allocate t10_alua_tg_pt"
>>>>>>> refs/remotes/origin/master
					"_gp_member_t\n");
			return;
		}
		spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
		__core_alua_attach_tg_pt_gp_mem(tg_pt_gp_mem,
<<<<<<< HEAD
<<<<<<< HEAD
			T10_ALUA(su_dev)->default_tg_pt_gp);
		spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
		printk(KERN_INFO "%s/%s: Adding to default ALUA Target Port"
			" Group: alua/default_tg_pt_gp\n",
			TRANSPORT(dev)->name, TPG_TFO(tpg)->get_fabric_name());
=======
			su_dev->t10_alua.default_tg_pt_gp);
=======
			dev->t10_alua.default_tg_pt_gp);
>>>>>>> refs/remotes/origin/master
		spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
		pr_debug("%s/%s: Adding to default ALUA Target Port"
			" Group: alua/default_tg_pt_gp\n",
			dev->transport->name, tpg->se_tpg_tfo->get_fabric_name());
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}

	dev->dev_port_count++;
	port->sep_index = port->sep_rtpi; /* RELATIVE TARGET PORT IDENTIFER */
=======
	}

	dev->dev_port_count++;
	port->sep_index = port->sep_rtpi; /* RELATIVE TARGET PORT IDENTIFIER */
>>>>>>> refs/remotes/origin/master
}

/*
 *	Called with struct se_device->se_port_lock spinlock held.
 */
static void core_release_port(struct se_device *dev, struct se_port *port)
	__releases(&dev->se_port_lock) __acquires(&dev->se_port_lock)
{
	/*
	 * Wait for any port reference for PR ALL_TG_PT=1 operation
	 * to complete in __core_scsi3_alloc_registration()
	 */
	spin_unlock(&dev->se_port_lock);
	if (atomic_read(&port->sep_tg_pt_ref_cnt))
		cpu_relax();
	spin_lock(&dev->se_port_lock);

	core_alua_free_tg_pt_gp_mem(port);

	list_del(&port->sep_list);
	dev->dev_port_count--;
	kfree(port);
<<<<<<< HEAD
<<<<<<< HEAD

	return;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

int core_dev_export(
	struct se_device *dev,
	struct se_portal_group *tpg,
	struct se_lun *lun)
{
<<<<<<< HEAD
	struct se_port *port;

	port = core_alloc_port(dev);
<<<<<<< HEAD
	if (!(port))
		return -1;
=======
	if (IS_ERR(port))
		return PTR_ERR(port);
>>>>>>> refs/remotes/origin/cm-10.0

	lun->lun_se_dev = dev;
	se_dev_start(dev);

	atomic_inc(&dev->dev_export_obj.obj_access_count);
=======
	struct se_hba *hba = dev->se_hba;
	struct se_port *port;

	port = core_alloc_port(dev);
	if (IS_ERR(port))
		return PTR_ERR(port);

	lun->lun_se_dev = dev;

	spin_lock(&hba->device_lock);
	dev->export_count++;
	spin_unlock(&hba->device_lock);

>>>>>>> refs/remotes/origin/master
	core_export_port(dev, tpg, port, lun);
	return 0;
}

void core_dev_unexport(
	struct se_device *dev,
	struct se_portal_group *tpg,
	struct se_lun *lun)
{
<<<<<<< HEAD
=======
	struct se_hba *hba = dev->se_hba;
>>>>>>> refs/remotes/origin/master
	struct se_port *port = lun->lun_sep;

	spin_lock(&lun->lun_sep_lock);
	if (lun->lun_se_dev == NULL) {
		spin_unlock(&lun->lun_sep_lock);
		return;
	}
	spin_unlock(&lun->lun_sep_lock);

	spin_lock(&dev->se_port_lock);
<<<<<<< HEAD
	atomic_dec(&dev->dev_export_obj.obj_access_count);
	core_release_port(dev, port);
	spin_unlock(&dev->se_port_lock);

	se_dev_stop(dev);
	lun->lun_se_dev = NULL;
}

<<<<<<< HEAD
int transport_core_report_lun_response(struct se_cmd *se_cmd)
{
	struct se_dev_entry *deve;
	struct se_lun *se_lun;
	struct se_session *se_sess = SE_SESS(se_cmd);
	struct se_task *se_task;
	unsigned char *buf = (unsigned char *)T_TASK(se_cmd)->t_task_buf;
	u32 cdb_offset = 0, lun_count = 0, offset = 8, i;

	list_for_each_entry(se_task, &T_TASK(se_cmd)->t_task_list, t_list)
		break;

	if (!(se_task)) {
		printk(KERN_ERR "Unable to locate struct se_task for struct se_cmd\n");
		return PYX_TRANSPORT_LU_COMM_FAILURE;
	}
=======
int target_report_luns(struct se_task *se_task)
{
	struct se_cmd *se_cmd = se_task->task_se_cmd;
	struct se_dev_entry *deve;
	struct se_session *se_sess = se_cmd->se_sess;
	unsigned char *buf;
	u32 lun_count = 0, offset = 8, i;

	buf = transport_kmap_data_sg(se_cmd);
	if (!buf)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * If no struct se_session pointer is present, this struct se_cmd is
	 * coming via a target_core_mod PASSTHROUGH op, and not through
	 * a $FABRIC_MOD.  In that case, report LUN=0 only.
	 */
<<<<<<< HEAD
	if (!(se_sess)) {
=======
	if (!se_sess) {
>>>>>>> refs/remotes/origin/cm-10.0
		int_to_scsilun(0, (struct scsi_lun *)&buf[offset]);
		lun_count = 1;
		goto done;
	}

<<<<<<< HEAD
	spin_lock_irq(&SE_NODE_ACL(se_sess)->device_list_lock);
	for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
		deve = &SE_NODE_ACL(se_sess)->device_list[i];
		if (!(deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS))
			continue;
		se_lun = deve->se_lun;
=======
	spin_lock_irq(&se_sess->se_node_acl->device_list_lock);
	for (i = 0; i < TRANSPORT_MAX_LUNS_PER_TPG; i++) {
		deve = se_sess->se_node_acl->device_list[i];
		if (!(deve->lun_flags & TRANSPORT_LUNFLAGS_INITIATOR_ACCESS))
			continue;
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * We determine the correct LUN LIST LENGTH even once we
		 * have reached the initial allocation length.
		 * See SPC2-R20 7.19.
		 */
		lun_count++;
<<<<<<< HEAD
		if ((cdb_offset + 8) >= se_cmd->data_length)
=======
		if ((offset + 8) > se_cmd->data_length)
>>>>>>> refs/remotes/origin/cm-10.0
			continue;

		int_to_scsilun(deve->mapped_lun, (struct scsi_lun *)&buf[offset]);
		offset += 8;
<<<<<<< HEAD
		cdb_offset += 8;
	}
	spin_unlock_irq(&SE_NODE_ACL(se_sess)->device_list_lock);
=======
	}
	spin_unlock_irq(&se_sess->se_node_acl->device_list_lock);
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * See SPC3 r07, page 159.
	 */
done:
	lun_count *= 8;
	buf[0] = ((lun_count >> 24) & 0xff);
	buf[1] = ((lun_count >> 16) & 0xff);
	buf[2] = ((lun_count >> 8) & 0xff);
	buf[3] = (lun_count & 0xff);
<<<<<<< HEAD

	return PYX_TRANSPORT_SENT_TO_TRANSPORT;
=======
	transport_kunmap_data_sg(se_cmd);

	se_task->task_scsi_status = GOOD;
	transport_complete_task(se_task, 1);
	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*	se_release_device_for_hba():
 *
 *
 */
void se_release_device_for_hba(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;

	if ((dev->dev_status & TRANSPORT_DEVICE_ACTIVATED) ||
	    (dev->dev_status & TRANSPORT_DEVICE_DEACTIVATED) ||
	    (dev->dev_status & TRANSPORT_DEVICE_SHUTDOWN) ||
	    (dev->dev_status & TRANSPORT_DEVICE_OFFLINE_ACTIVATED) ||
	    (dev->dev_status & TRANSPORT_DEVICE_OFFLINE_DEACTIVATED))
		se_dev_stop(dev);

	if (dev->dev_ptr) {
		kthread_stop(dev->process_thread);
		if (dev->transport->free_device)
			dev->transport->free_device(dev->dev_ptr);
	}

	spin_lock(&hba->device_lock);
	list_del(&dev->dev_list);
	hba->dev_count--;
	spin_unlock(&hba->device_lock);

	core_scsi3_free_all_registrations(dev);
	se_release_vpd_for_dev(dev);

<<<<<<< HEAD
	kfree(dev->dev_status_queue_obj);
	kfree(dev->dev_queue_obj);
	kfree(dev);

	return;
=======
	kfree(dev);
>>>>>>> refs/remotes/origin/cm-10.0
}

void se_release_vpd_for_dev(struct se_device *dev)
{
	struct t10_vpd *vpd, *vpd_tmp;

<<<<<<< HEAD
	spin_lock(&DEV_T10_WWN(dev)->t10_vpd_lock);
	list_for_each_entry_safe(vpd, vpd_tmp,
			&DEV_T10_WWN(dev)->t10_vpd_list, vpd_list) {
		list_del(&vpd->vpd_list);
		kfree(vpd);
	}
	spin_unlock(&DEV_T10_WWN(dev)->t10_vpd_lock);

	return;
=======
	spin_lock(&dev->se_sub_dev->t10_wwn.t10_vpd_lock);
	list_for_each_entry_safe(vpd, vpd_tmp,
			&dev->se_sub_dev->t10_wwn.t10_vpd_list, vpd_list) {
		list_del(&vpd->vpd_list);
		kfree(vpd);
	}
	spin_unlock(&dev->se_sub_dev->t10_wwn.t10_vpd_lock);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*	se_free_virtual_device():
 *
 *	Used for IBLOCK, RAMDISK, and FILEIO Transport Drivers.
 */
int se_free_virtual_device(struct se_device *dev, struct se_hba *hba)
{
	if (!list_empty(&dev->dev_sep_list))
		dump_stack();

	core_alua_free_lu_gp_mem(dev);
	se_release_device_for_hba(dev);

	return 0;
}

static void se_dev_start(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;

	spin_lock(&hba->device_lock);
	atomic_inc(&dev->dev_obj.obj_access_count);
	if (atomic_read(&dev->dev_obj.obj_access_count) == 1) {
		if (dev->dev_status & TRANSPORT_DEVICE_DEACTIVATED) {
			dev->dev_status &= ~TRANSPORT_DEVICE_DEACTIVATED;
			dev->dev_status |= TRANSPORT_DEVICE_ACTIVATED;
		} else if (dev->dev_status &
			   TRANSPORT_DEVICE_OFFLINE_DEACTIVATED) {
			dev->dev_status &=
				~TRANSPORT_DEVICE_OFFLINE_DEACTIVATED;
			dev->dev_status |= TRANSPORT_DEVICE_OFFLINE_ACTIVATED;
		}
	}
	spin_unlock(&hba->device_lock);
}

static void se_dev_stop(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;

	spin_lock(&hba->device_lock);
	atomic_dec(&dev->dev_obj.obj_access_count);
	if (atomic_read(&dev->dev_obj.obj_access_count) == 0) {
		if (dev->dev_status & TRANSPORT_DEVICE_ACTIVATED) {
			dev->dev_status &= ~TRANSPORT_DEVICE_ACTIVATED;
			dev->dev_status |= TRANSPORT_DEVICE_DEACTIVATED;
		} else if (dev->dev_status &
			   TRANSPORT_DEVICE_OFFLINE_ACTIVATED) {
			dev->dev_status &= ~TRANSPORT_DEVICE_OFFLINE_ACTIVATED;
			dev->dev_status |= TRANSPORT_DEVICE_OFFLINE_DEACTIVATED;
		}
	}
	spin_unlock(&hba->device_lock);
}

int se_dev_check_online(struct se_device *dev)
{
<<<<<<< HEAD
	int ret;

	spin_lock_irq(&dev->dev_status_lock);
	ret = ((dev->dev_status & TRANSPORT_DEVICE_ACTIVATED) ||
	       (dev->dev_status & TRANSPORT_DEVICE_DEACTIVATED)) ? 0 : 1;
	spin_unlock_irq(&dev->dev_status_lock);
=======
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&dev->dev_status_lock, flags);
	ret = ((dev->dev_status & TRANSPORT_DEVICE_ACTIVATED) ||
	       (dev->dev_status & TRANSPORT_DEVICE_DEACTIVATED)) ? 0 : 1;
	spin_unlock_irqrestore(&dev->dev_status_lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0

	return ret;
}

int se_dev_check_shutdown(struct se_device *dev)
{
	int ret;

	spin_lock_irq(&dev->dev_status_lock);
	ret = (dev->dev_status & TRANSPORT_DEVICE_SHUTDOWN);
	spin_unlock_irq(&dev->dev_status_lock);

	return ret;
}

<<<<<<< HEAD
=======
u32 se_dev_align_max_sectors(u32 max_sectors, u32 block_size)
<<<<<<< HEAD
=======
	core_release_port(dev, port);
	spin_unlock(&dev->se_port_lock);

	spin_lock(&hba->device_lock);
	dev->export_count--;
	spin_unlock(&hba->device_lock);

	lun->lun_se_dev = NULL;
}

static void se_release_vpd_for_dev(struct se_device *dev)
{
	struct t10_vpd *vpd, *vpd_tmp;

	spin_lock(&dev->t10_wwn.t10_vpd_lock);
	list_for_each_entry_safe(vpd, vpd_tmp,
			&dev->t10_wwn.t10_vpd_list, vpd_list) {
		list_del(&vpd->vpd_list);
		kfree(vpd);
	}
	spin_unlock(&dev->t10_wwn.t10_vpd_lock);
}

static u32 se_dev_align_max_sectors(u32 max_sectors, u32 block_size)
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
{
	u32 aligned_max_sectors;
	u32 alignment;
	/*
	 * Limit max_sectors to a PAGE_SIZE aligned value for modern
	 * transport_allocate_data_tasks() operation.
	 */
	alignment = max(1ul, PAGE_SIZE / block_size);
	aligned_max_sectors = rounddown(max_sectors, alignment);

	if (max_sectors != aligned_max_sectors)
		pr_info("Rounding down aligned max_sectors from %u to %u\n",
			max_sectors, aligned_max_sectors);

	return aligned_max_sectors;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
void se_dev_set_default_attribs(
	struct se_device *dev,
	struct se_dev_limits *dev_limits)
{
	struct queue_limits *limits = &dev_limits->limits;

<<<<<<< HEAD
	DEV_ATTRIB(dev)->emulate_dpo = DA_EMULATE_DPO;
	DEV_ATTRIB(dev)->emulate_fua_write = DA_EMULATE_FUA_WRITE;
	DEV_ATTRIB(dev)->emulate_fua_read = DA_EMULATE_FUA_READ;
	DEV_ATTRIB(dev)->emulate_write_cache = DA_EMULATE_WRITE_CACHE;
	DEV_ATTRIB(dev)->emulate_ua_intlck_ctrl = DA_EMULATE_UA_INTLLCK_CTRL;
	DEV_ATTRIB(dev)->emulate_tas = DA_EMULATE_TAS;
	DEV_ATTRIB(dev)->emulate_tpu = DA_EMULATE_TPU;
	DEV_ATTRIB(dev)->emulate_tpws = DA_EMULATE_TPWS;
	DEV_ATTRIB(dev)->emulate_reservations = DA_EMULATE_RESERVATIONS;
	DEV_ATTRIB(dev)->emulate_alua = DA_EMULATE_ALUA;
	DEV_ATTRIB(dev)->enforce_pr_isids = DA_ENFORCE_PR_ISIDS;
=======
	dev->se_sub_dev->se_dev_attrib.emulate_dpo = DA_EMULATE_DPO;
	dev->se_sub_dev->se_dev_attrib.emulate_fua_write = DA_EMULATE_FUA_WRITE;
	dev->se_sub_dev->se_dev_attrib.emulate_fua_read = DA_EMULATE_FUA_READ;
	dev->se_sub_dev->se_dev_attrib.emulate_write_cache = DA_EMULATE_WRITE_CACHE;
	dev->se_sub_dev->se_dev_attrib.emulate_ua_intlck_ctrl = DA_EMULATE_UA_INTLLCK_CTRL;
	dev->se_sub_dev->se_dev_attrib.emulate_tas = DA_EMULATE_TAS;
	dev->se_sub_dev->se_dev_attrib.emulate_tpu = DA_EMULATE_TPU;
	dev->se_sub_dev->se_dev_attrib.emulate_tpws = DA_EMULATE_TPWS;
	dev->se_sub_dev->se_dev_attrib.emulate_reservations = DA_EMULATE_RESERVATIONS;
	dev->se_sub_dev->se_dev_attrib.emulate_alua = DA_EMULATE_ALUA;
	dev->se_sub_dev->se_dev_attrib.enforce_pr_isids = DA_ENFORCE_PR_ISIDS;
	dev->se_sub_dev->se_dev_attrib.is_nonrot = DA_IS_NONROT;
	dev->se_sub_dev->se_dev_attrib.emulate_rest_reord = DA_EMULATE_REST_REORD;
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * The TPU=1 and TPWS=1 settings will be set in TCM/IBLOCK
	 * iblock_create_virtdevice() from struct queue_limits values
	 * if blk_queue_discard()==1
	 */
<<<<<<< HEAD
	DEV_ATTRIB(dev)->max_unmap_lba_count = DA_MAX_UNMAP_LBA_COUNT;
	DEV_ATTRIB(dev)->max_unmap_block_desc_count =
				DA_MAX_UNMAP_BLOCK_DESC_COUNT;
	DEV_ATTRIB(dev)->unmap_granularity = DA_UNMAP_GRANULARITY_DEFAULT;
	DEV_ATTRIB(dev)->unmap_granularity_alignment =
=======
	dev->se_sub_dev->se_dev_attrib.max_unmap_lba_count = DA_MAX_UNMAP_LBA_COUNT;
	dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count =
		DA_MAX_UNMAP_BLOCK_DESC_COUNT;
	dev->se_sub_dev->se_dev_attrib.unmap_granularity = DA_UNMAP_GRANULARITY_DEFAULT;
	dev->se_sub_dev->se_dev_attrib.unmap_granularity_alignment =
>>>>>>> refs/remotes/origin/cm-10.0
				DA_UNMAP_GRANULARITY_ALIGNMENT_DEFAULT;
	/*
	 * block_size is based on subsystem plugin dependent requirements.
	 */
<<<<<<< HEAD
	DEV_ATTRIB(dev)->hw_block_size = limits->logical_block_size;
	DEV_ATTRIB(dev)->block_size = limits->logical_block_size;
	/*
	 * max_sectors is based on subsystem plugin dependent requirements.
	 */
	DEV_ATTRIB(dev)->hw_max_sectors = limits->max_hw_sectors;
	DEV_ATTRIB(dev)->max_sectors = limits->max_sectors;
	/*
	 * Set optimal_sectors from max_sectors, which can be lowered via
	 * configfs.
	 */
	DEV_ATTRIB(dev)->optimal_sectors = limits->max_sectors;
	/*
	 * queue_depth is based on subsystem plugin dependent requirements.
	 */
	DEV_ATTRIB(dev)->hw_queue_depth = dev_limits->hw_queue_depth;
	DEV_ATTRIB(dev)->queue_depth = dev_limits->queue_depth;
}

int se_dev_set_task_timeout(struct se_device *dev, u32 task_timeout)
{
	if (task_timeout > DA_TASK_TIMEOUT_MAX) {
		printk(KERN_ERR "dev[%p]: Passed task_timeout: %u larger then"
			" DA_TASK_TIMEOUT_MAX\n", dev, task_timeout);
		return -1;
	} else {
		DEV_ATTRIB(dev)->task_timeout = task_timeout;
		printk(KERN_INFO "dev[%p]: Set SE Device task_timeout: %u\n",
			dev, task_timeout);
	}

	return 0;
=======
	dev->se_sub_dev->se_dev_attrib.hw_block_size = limits->logical_block_size;
	dev->se_sub_dev->se_dev_attrib.block_size = limits->logical_block_size;
	/*
	 * max_sectors is based on subsystem plugin dependent requirements.
	 */
	dev->se_sub_dev->se_dev_attrib.hw_max_sectors = limits->max_hw_sectors;
	/*
	 * Align max_sectors down to PAGE_SIZE to follow transport_allocate_data_tasks()
	 */
	limits->max_sectors = se_dev_align_max_sectors(limits->max_sectors,
						limits->logical_block_size);
	dev->se_sub_dev->se_dev_attrib.max_sectors = limits->max_sectors;
	/*
	 * Set fabric_max_sectors, which is reported in block limits
	 * VPD page (B0h).
	 */
	dev->se_sub_dev->se_dev_attrib.fabric_max_sectors = DA_FABRIC_MAX_SECTORS;
	/*
	 * Set optimal_sectors from fabric_max_sectors, which can be
	 * lowered via configfs.
	 */
	dev->se_sub_dev->se_dev_attrib.optimal_sectors = DA_FABRIC_MAX_SECTORS;
	/*
	 * queue_depth is based on subsystem plugin dependent requirements.
	 */
	dev->se_sub_dev->se_dev_attrib.hw_queue_depth = dev_limits->hw_queue_depth;
	dev->se_sub_dev->se_dev_attrib.queue_depth = dev_limits->queue_depth;
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
>>>>>>> refs/remotes/origin/master
int se_dev_set_max_unmap_lba_count(
	struct se_device *dev,
	u32 max_unmap_lba_count)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DEV_ATTRIB(dev)->max_unmap_lba_count = max_unmap_lba_count;
	printk(KERN_INFO "dev[%p]: Set max_unmap_lba_count: %u\n",
			dev, DEV_ATTRIB(dev)->max_unmap_lba_count);
=======
	dev->se_sub_dev->se_dev_attrib.max_unmap_lba_count = max_unmap_lba_count;
	pr_debug("dev[%p]: Set max_unmap_lba_count: %u\n",
			dev, dev->se_sub_dev->se_dev_attrib.max_unmap_lba_count);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev->dev_attrib.max_unmap_lba_count = max_unmap_lba_count;
	pr_debug("dev[%p]: Set max_unmap_lba_count: %u\n",
			dev, dev->dev_attrib.max_unmap_lba_count);
>>>>>>> refs/remotes/origin/master
	return 0;
}

int se_dev_set_max_unmap_block_desc_count(
	struct se_device *dev,
	u32 max_unmap_block_desc_count)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DEV_ATTRIB(dev)->max_unmap_block_desc_count = max_unmap_block_desc_count;
	printk(KERN_INFO "dev[%p]: Set max_unmap_block_desc_count: %u\n",
			dev, DEV_ATTRIB(dev)->max_unmap_block_desc_count);
=======
	dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count =
		max_unmap_block_desc_count;
	pr_debug("dev[%p]: Set max_unmap_block_desc_count: %u\n",
			dev, dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev->dev_attrib.max_unmap_block_desc_count =
		max_unmap_block_desc_count;
	pr_debug("dev[%p]: Set max_unmap_block_desc_count: %u\n",
			dev, dev->dev_attrib.max_unmap_block_desc_count);
>>>>>>> refs/remotes/origin/master
	return 0;
}

int se_dev_set_unmap_granularity(
	struct se_device *dev,
	u32 unmap_granularity)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DEV_ATTRIB(dev)->unmap_granularity = unmap_granularity;
	printk(KERN_INFO "dev[%p]: Set unmap_granularity: %u\n",
			dev, DEV_ATTRIB(dev)->unmap_granularity);
=======
	dev->se_sub_dev->se_dev_attrib.unmap_granularity = unmap_granularity;
	pr_debug("dev[%p]: Set unmap_granularity: %u\n",
			dev, dev->se_sub_dev->se_dev_attrib.unmap_granularity);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev->dev_attrib.unmap_granularity = unmap_granularity;
	pr_debug("dev[%p]: Set unmap_granularity: %u\n",
			dev, dev->dev_attrib.unmap_granularity);
>>>>>>> refs/remotes/origin/master
	return 0;
}

int se_dev_set_unmap_granularity_alignment(
	struct se_device *dev,
	u32 unmap_granularity_alignment)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DEV_ATTRIB(dev)->unmap_granularity_alignment = unmap_granularity_alignment;
	printk(KERN_INFO "dev[%p]: Set unmap_granularity_alignment: %u\n",
			dev, DEV_ATTRIB(dev)->unmap_granularity_alignment);
=======
	dev->se_sub_dev->se_dev_attrib.unmap_granularity_alignment = unmap_granularity_alignment;
	pr_debug("dev[%p]: Set unmap_granularity_alignment: %u\n",
			dev, dev->se_sub_dev->se_dev_attrib.unmap_granularity_alignment);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

int se_dev_set_emulate_dpo(struct se_device *dev, int flag)
{
<<<<<<< HEAD
	if ((flag != 0) && (flag != 1)) {
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -1;
	}
	if (TRANSPORT(dev)->dpo_emulated == NULL) {
		printk(KERN_ERR "TRANSPORT(dev)->dpo_emulated is NULL\n");
		return -1;
	}
	if (TRANSPORT(dev)->dpo_emulated(dev) == 0) {
		printk(KERN_ERR "TRANSPORT(dev)->dpo_emulated not supported\n");
		return -1;
	}
	DEV_ATTRIB(dev)->emulate_dpo = flag;
	printk(KERN_INFO "dev[%p]: SE Device Page Out (DPO) Emulation"
			" bit: %d\n", dev, DEV_ATTRIB(dev)->emulate_dpo);
=======
=======
	dev->dev_attrib.unmap_granularity_alignment = unmap_granularity_alignment;
	pr_debug("dev[%p]: Set unmap_granularity_alignment: %u\n",
			dev, dev->dev_attrib.unmap_granularity_alignment);
	return 0;
}

int se_dev_set_max_write_same_len(
	struct se_device *dev,
	u32 max_write_same_len)
{
	dev->dev_attrib.max_write_same_len = max_write_same_len;
	pr_debug("dev[%p]: Set max_write_same_len: %u\n",
			dev, dev->dev_attrib.max_write_same_len);
	return 0;
}

static void dev_set_t10_wwn_model_alias(struct se_device *dev)
{
	const char *configname;

	configname = config_item_name(&dev->dev_group.cg_item);
	if (strlen(configname) >= 16) {
		pr_warn("dev[%p]: Backstore name '%s' is too long for "
			"INQUIRY_MODEL, truncating to 16 bytes\n", dev,
			configname);
	}
	snprintf(&dev->t10_wwn.model[0], 16, "%s", configname);
}

int se_dev_set_emulate_model_alias(struct se_device *dev, int flag)
{
	if (dev->export_count) {
		pr_err("dev[%p]: Unable to change model alias"
			" while export_count is %d\n",
			dev, dev->export_count);
			return -EINVAL;
	}

	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

	if (flag) {
		dev_set_t10_wwn_model_alias(dev);
	} else {
		strncpy(&dev->t10_wwn.model[0],
			dev->transport->inquiry_prod, 16);
	}
	dev->dev_attrib.emulate_model_alias = flag;

	return 0;
}

int se_dev_set_emulate_dpo(struct se_device *dev, int flag)
{
>>>>>>> refs/remotes/origin/master
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

	if (flag) {
		pr_err("dpo_emulated not supported\n");
		return -EINVAL;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

int se_dev_set_emulate_fua_write(struct se_device *dev, int flag)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if ((flag != 0) && (flag != 1)) {
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -1;
	}
	if (TRANSPORT(dev)->fua_write_emulated == NULL) {
		printk(KERN_ERR "TRANSPORT(dev)->fua_write_emulated is NULL\n");
		return -1;
	}
	if (TRANSPORT(dev)->fua_write_emulated(dev) == 0) {
		printk(KERN_ERR "TRANSPORT(dev)->fua_write_emulated not supported\n");
		return -1;
	}
	DEV_ATTRIB(dev)->emulate_fua_write = flag;
	printk(KERN_INFO "dev[%p]: SE Device Forced Unit Access WRITEs: %d\n",
			dev, DEV_ATTRIB(dev)->emulate_fua_write);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

<<<<<<< HEAD
	if (flag && dev->transport->fua_write_emulated == 0) {
		pr_err("fua_write_emulated not supported\n");
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.emulate_fua_write = flag;
	pr_debug("dev[%p]: SE Device Forced Unit Access WRITEs: %d\n",
			dev, dev->se_sub_dev->se_dev_attrib.emulate_fua_write);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (flag &&
	    dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		pr_err("emulate_fua_write not supported for pSCSI\n");
		return -EINVAL;
	}
	dev->dev_attrib.emulate_fua_write = flag;
	pr_debug("dev[%p]: SE Device Forced Unit Access WRITEs: %d\n",
			dev, dev->dev_attrib.emulate_fua_write);
>>>>>>> refs/remotes/origin/master
	return 0;
}

int se_dev_set_emulate_fua_read(struct se_device *dev, int flag)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if ((flag != 0) && (flag != 1)) {
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -1;
	}
	if (TRANSPORT(dev)->fua_read_emulated == NULL) {
		printk(KERN_ERR "TRANSPORT(dev)->fua_read_emulated is NULL\n");
		return -1;
	}
	if (TRANSPORT(dev)->fua_read_emulated(dev) == 0) {
		printk(KERN_ERR "TRANSPORT(dev)->fua_read_emulated not supported\n");
		return -1;
	}
	DEV_ATTRIB(dev)->emulate_fua_read = flag;
	printk(KERN_INFO "dev[%p]: SE Device Forced Unit Access READs: %d\n",
			dev, DEV_ATTRIB(dev)->emulate_fua_read);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

	if (flag) {
		pr_err("ua read emulated not supported\n");
		return -EINVAL;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

int se_dev_set_emulate_write_cache(struct se_device *dev, int flag)
{
<<<<<<< HEAD
	if ((flag != 0) && (flag != 1)) {
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -1;
	}
	if (TRANSPORT(dev)->write_cache_emulated == NULL) {
		printk(KERN_ERR "TRANSPORT(dev)->write_cache_emulated is NULL\n");
		return -1;
	}
	if (TRANSPORT(dev)->write_cache_emulated(dev) == 0) {
		printk(KERN_ERR "TRANSPORT(dev)->write_cache_emulated not supported\n");
		return -1;
	}
	DEV_ATTRIB(dev)->emulate_write_cache = flag;
	printk(KERN_INFO "dev[%p]: SE Device WRITE_CACHE_EMULATION flag: %d\n",
			dev, DEV_ATTRIB(dev)->emulate_write_cache);
=======
=======
	return 0;
}

int se_dev_set_emulate_write_cache(struct se_device *dev, int flag)
{
>>>>>>> refs/remotes/origin/master
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}
<<<<<<< HEAD
	if (flag && dev->transport->write_cache_emulated == 0) {
		pr_err("write_cache_emulated not supported\n");
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.emulate_write_cache = flag;
	pr_debug("dev[%p]: SE Device WRITE_CACHE_EMULATION flag: %d\n",
			dev, dev->se_sub_dev->se_dev_attrib.emulate_write_cache);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (flag &&
	    dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		pr_err("emulate_write_cache not supported for pSCSI\n");
		return -EINVAL;
	}
	if (dev->transport->get_write_cache) {
		pr_warn("emulate_write_cache cannot be changed when underlying"
			" HW reports WriteCacheEnabled, ignoring request\n");
		return 0;
	}

	dev->dev_attrib.emulate_write_cache = flag;
	pr_debug("dev[%p]: SE Device WRITE_CACHE_EMULATION flag: %d\n",
			dev, dev->dev_attrib.emulate_write_cache);
>>>>>>> refs/remotes/origin/master
	return 0;
}

int se_dev_set_emulate_ua_intlck_ctrl(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1) && (flag != 2)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -1;
	}

	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		printk(KERN_ERR "dev[%p]: Unable to change SE Device"
			" UA_INTRLCK_CTRL while dev_export_obj: %d count"
			" exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -1;
	}
	DEV_ATTRIB(dev)->emulate_ua_intlck_ctrl = flag;
	printk(KERN_INFO "dev[%p]: SE Device UA_INTRLCK_CTRL flag: %d\n",
		dev, DEV_ATTRIB(dev)->emulate_ua_intlck_ctrl);
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

<<<<<<< HEAD
	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device"
			" UA_INTRLCK_CTRL while dev_export_obj: %d count"
			" exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.emulate_ua_intlck_ctrl = flag;
	pr_debug("dev[%p]: SE Device UA_INTRLCK_CTRL flag: %d\n",
		dev, dev->se_sub_dev->se_dev_attrib.emulate_ua_intlck_ctrl);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (dev->export_count) {
		pr_err("dev[%p]: Unable to change SE Device"
			" UA_INTRLCK_CTRL while export_count is %d\n",
			dev, dev->export_count);
		return -EINVAL;
	}
	dev->dev_attrib.emulate_ua_intlck_ctrl = flag;
	pr_debug("dev[%p]: SE Device UA_INTRLCK_CTRL flag: %d\n",
		dev, dev->dev_attrib.emulate_ua_intlck_ctrl);
>>>>>>> refs/remotes/origin/master

	return 0;
}

int se_dev_set_emulate_tas(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -1;
	}

	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		printk(KERN_ERR "dev[%p]: Unable to change SE Device TAS while"
			" dev_export_obj: %d count exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -1;
	}
	DEV_ATTRIB(dev)->emulate_tas = flag;
	printk(KERN_INFO "dev[%p]: SE Device TASK_ABORTED status bit: %s\n",
		dev, (DEV_ATTRIB(dev)->emulate_tas) ? "Enabled" : "Disabled");
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}

<<<<<<< HEAD
	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device TAS while"
			" dev_export_obj: %d count exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.emulate_tas = flag;
	pr_debug("dev[%p]: SE Device TASK_ABORTED status bit: %s\n",
		dev, (dev->se_sub_dev->se_dev_attrib.emulate_tas) ? "Enabled" : "Disabled");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (dev->export_count) {
		pr_err("dev[%p]: Unable to change SE Device TAS while"
			" export_count is %d\n",
			dev, dev->export_count);
		return -EINVAL;
	}
	dev->dev_attrib.emulate_tas = flag;
	pr_debug("dev[%p]: SE Device TASK_ABORTED status bit: %s\n",
		dev, (dev->dev_attrib.emulate_tas) ? "Enabled" : "Disabled");
>>>>>>> refs/remotes/origin/master

	return 0;
}

int se_dev_set_emulate_tpu(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -1;
=======
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * We expect this value to be non-zero when generic Block Layer
	 * Discard supported is detected iblock_create_virtdevice().
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(DEV_ATTRIB(dev)->max_unmap_block_desc_count)) {
		printk(KERN_ERR "Generic Block Discard not supported\n");
		return -ENOSYS;
	}

	DEV_ATTRIB(dev)->emulate_tpu = flag;
	printk(KERN_INFO "dev[%p]: SE Device Thin Provisioning UNMAP bit: %d\n",
=======
	if (flag && !dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count) {
=======
	if (flag && !dev->dev_attrib.max_unmap_block_desc_count) {
>>>>>>> refs/remotes/origin/master
		pr_err("Generic Block Discard not supported\n");
		return -ENOSYS;
	}

<<<<<<< HEAD
	dev->se_sub_dev->se_dev_attrib.emulate_tpu = flag;
	pr_debug("dev[%p]: SE Device Thin Provisioning UNMAP bit: %d\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev->dev_attrib.emulate_tpu = flag;
	pr_debug("dev[%p]: SE Device Thin Provisioning UNMAP bit: %d\n",
>>>>>>> refs/remotes/origin/master
				dev, flag);
	return 0;
}

int se_dev_set_emulate_tpws(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -1;
=======
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * We expect this value to be non-zero when generic Block Layer
	 * Discard supported is detected iblock_create_virtdevice().
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(DEV_ATTRIB(dev)->max_unmap_block_desc_count)) {
		printk(KERN_ERR "Generic Block Discard not supported\n");
		return -ENOSYS;
	}

	DEV_ATTRIB(dev)->emulate_tpws = flag;
	printk(KERN_INFO "dev[%p]: SE Device Thin Provisioning WRITE_SAME: %d\n",
=======
	if (flag && !dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count) {
=======
	if (flag && !dev->dev_attrib.max_unmap_block_desc_count) {
>>>>>>> refs/remotes/origin/master
		pr_err("Generic Block Discard not supported\n");
		return -ENOSYS;
	}

<<<<<<< HEAD
	dev->se_sub_dev->se_dev_attrib.emulate_tpws = flag;
	pr_debug("dev[%p]: SE Device Thin Provisioning WRITE_SAME: %d\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev->dev_attrib.emulate_tpws = flag;
	pr_debug("dev[%p]: SE Device Thin Provisioning WRITE_SAME: %d\n",
>>>>>>> refs/remotes/origin/master
				dev, flag);
	return 0;
}

<<<<<<< HEAD
int se_dev_set_enforce_pr_isids(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
<<<<<<< HEAD
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -1;
	}
	DEV_ATTRIB(dev)->enforce_pr_isids = flag;
	printk(KERN_INFO "dev[%p]: SE Device enforce_pr_isids bit: %s\n", dev,
		(DEV_ATTRIB(dev)->enforce_pr_isids) ? "Enabled" : "Disabled");
=======
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}
	dev->se_sub_dev->se_dev_attrib.enforce_pr_isids = flag;
	pr_debug("dev[%p]: SE Device enforce_pr_isids bit: %s\n", dev,
		(dev->se_sub_dev->se_dev_attrib.enforce_pr_isids) ? "Enabled" : "Disabled");
=======
int se_dev_set_emulate_caw(struct se_device *dev, int flag)
{
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}
	dev->dev_attrib.emulate_caw = flag;
	pr_debug("dev[%p]: SE Device CompareAndWrite (AtomicTestandSet): %d\n",
		 dev, flag);

	return 0;
}

int se_dev_set_emulate_3pc(struct se_device *dev, int flag)
{
	if (flag != 0 && flag != 1) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}
	dev->dev_attrib.emulate_3pc = flag;
	pr_debug("dev[%p]: SE Device 3rd Party Copy (EXTENDED_COPY): %d\n",
		dev, flag);

	return 0;
}

int se_dev_set_enforce_pr_isids(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
		pr_err("Illegal value %d\n", flag);
		return -EINVAL;
	}
	dev->dev_attrib.enforce_pr_isids = flag;
	pr_debug("dev[%p]: SE Device enforce_pr_isids bit: %s\n", dev,
		(dev->dev_attrib.enforce_pr_isids) ? "Enabled" : "Disabled");
>>>>>>> refs/remotes/origin/master
	return 0;
}

int se_dev_set_is_nonrot(struct se_device *dev, int flag)
{
	if ((flag != 0) && (flag != 1)) {
		printk(KERN_ERR "Illegal value %d\n", flag);
		return -EINVAL;
	}
<<<<<<< HEAD
	dev->se_sub_dev->se_dev_attrib.is_nonrot = flag;
=======
	dev->dev_attrib.is_nonrot = flag;
>>>>>>> refs/remotes/origin/master
	pr_debug("dev[%p]: SE Device is_nonrot bit: %d\n",
	       dev, flag);
	return 0;
}

int se_dev_set_emulate_rest_reord(struct se_device *dev, int flag)
{
	if (flag != 0) {
		printk(KERN_ERR "dev[%p]: SE Device emulatation of restricted"
			" reordering not implemented\n", dev);
		return -ENOSYS;
	}
<<<<<<< HEAD
	dev->se_sub_dev->se_dev_attrib.emulate_rest_reord = flag;
	pr_debug("dev[%p]: SE Device emulate_rest_reord: %d\n", dev, flag);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev->dev_attrib.emulate_rest_reord = flag;
	pr_debug("dev[%p]: SE Device emulate_rest_reord: %d\n", dev, flag);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * Note, this can only be called on unexported SE Device Object.
 */
int se_dev_set_queue_depth(struct se_device *dev, u32 queue_depth)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 orig_queue_depth = dev->queue_depth;

	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		printk(KERN_ERR "dev[%p]: Unable to change SE Device TCQ while"
			" dev_export_obj: %d count exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -1;
	}
	if (!(queue_depth)) {
		printk(KERN_ERR "dev[%p]: Illegal ZERO value for queue"
			"_depth\n", dev);
		return -1;
	}

	if (TRANSPORT(dev)->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		if (queue_depth > DEV_ATTRIB(dev)->hw_queue_depth) {
			printk(KERN_ERR "dev[%p]: Passed queue_depth: %u"
				" exceeds TCM/SE_Device TCQ: %u\n",
				dev, queue_depth,
				DEV_ATTRIB(dev)->hw_queue_depth);
			return -1;
		}
	} else {
		if (queue_depth > DEV_ATTRIB(dev)->queue_depth) {
			if (queue_depth > DEV_ATTRIB(dev)->hw_queue_depth) {
				printk(KERN_ERR "dev[%p]: Passed queue_depth:"
					" %u exceeds TCM/SE_Device MAX"
					" TCQ: %u\n", dev, queue_depth,
					DEV_ATTRIB(dev)->hw_queue_depth);
				return -1;
=======
	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device TCQ while"
			" dev_export_obj: %d count exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
=======
	if (dev->export_count) {
		pr_err("dev[%p]: Unable to change SE Device TCQ while"
			" export_count is %d\n",
			dev, dev->export_count);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}
	if (!queue_depth) {
		pr_err("dev[%p]: Illegal ZERO value for queue"
			"_depth\n", dev);
		return -EINVAL;
	}

	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
<<<<<<< HEAD
		if (queue_depth > dev->se_sub_dev->se_dev_attrib.hw_queue_depth) {
			pr_err("dev[%p]: Passed queue_depth: %u"
				" exceeds TCM/SE_Device TCQ: %u\n",
				dev, queue_depth,
				dev->se_sub_dev->se_dev_attrib.hw_queue_depth);
			return -EINVAL;
		}
	} else {
		if (queue_depth > dev->se_sub_dev->se_dev_attrib.queue_depth) {
			if (queue_depth > dev->se_sub_dev->se_dev_attrib.hw_queue_depth) {
				pr_err("dev[%p]: Passed queue_depth:"
					" %u exceeds TCM/SE_Device MAX"
					" TCQ: %u\n", dev, queue_depth,
					dev->se_sub_dev->se_dev_attrib.hw_queue_depth);
				return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (queue_depth > dev->dev_attrib.hw_queue_depth) {
			pr_err("dev[%p]: Passed queue_depth: %u"
				" exceeds TCM/SE_Device TCQ: %u\n",
				dev, queue_depth,
				dev->dev_attrib.hw_queue_depth);
			return -EINVAL;
		}
	} else {
		if (queue_depth > dev->dev_attrib.queue_depth) {
			if (queue_depth > dev->dev_attrib.hw_queue_depth) {
				pr_err("dev[%p]: Passed queue_depth:"
					" %u exceeds TCM/SE_Device MAX"
					" TCQ: %u\n", dev, queue_depth,
					dev->dev_attrib.hw_queue_depth);
				return -EINVAL;
>>>>>>> refs/remotes/origin/master
			}
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	DEV_ATTRIB(dev)->queue_depth = dev->queue_depth = queue_depth;
	if (queue_depth > orig_queue_depth)
		atomic_add(queue_depth - orig_queue_depth, &dev->depth_left);
	else if (queue_depth < orig_queue_depth)
		atomic_sub(orig_queue_depth - queue_depth, &dev->depth_left);

	printk(KERN_INFO "dev[%p]: SE Device TCQ Depth changed to: %u\n",
=======
	dev->se_sub_dev->se_dev_attrib.queue_depth = dev->queue_depth = queue_depth;
	pr_debug("dev[%p]: SE Device TCQ Depth changed to: %u\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev->dev_attrib.queue_depth = dev->queue_depth = queue_depth;
	pr_debug("dev[%p]: SE Device TCQ Depth changed to: %u\n",
>>>>>>> refs/remotes/origin/master
			dev, queue_depth);
	return 0;
}

<<<<<<< HEAD
int se_dev_set_max_sectors(struct se_device *dev, u32 max_sectors)
{
	int force = 0; /* Force setting for VDEVS */

	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
<<<<<<< HEAD
		printk(KERN_ERR "dev[%p]: Unable to change SE Device"
			" max_sectors while dev_export_obj: %d count exists\n",
			dev, atomic_read(&dev->dev_export_obj.obj_access_count));
		return -1;
	}
	if (!(max_sectors)) {
		printk(KERN_ERR "dev[%p]: Illegal ZERO value for"
			" max_sectors\n", dev);
		return -1;
	}
	if (max_sectors < DA_STATUS_MAX_SECTORS_MIN) {
		printk(KERN_ERR "dev[%p]: Passed max_sectors: %u less than"
			" DA_STATUS_MAX_SECTORS_MIN: %u\n", dev, max_sectors,
				DA_STATUS_MAX_SECTORS_MIN);
		return -1;
	}
	if (TRANSPORT(dev)->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		if (max_sectors > DEV_ATTRIB(dev)->hw_max_sectors) {
			printk(KERN_ERR "dev[%p]: Passed max_sectors: %u"
				" greater than TCM/SE_Device max_sectors:"
				" %u\n", dev, max_sectors,
				DEV_ATTRIB(dev)->hw_max_sectors);
			 return -1;
		}
	} else {
		if (!(force) && (max_sectors >
				 DEV_ATTRIB(dev)->hw_max_sectors)) {
			printk(KERN_ERR "dev[%p]: Passed max_sectors: %u"
				" greater than TCM/SE_Device max_sectors"
				": %u, use force=1 to override.\n", dev,
				max_sectors, DEV_ATTRIB(dev)->hw_max_sectors);
			return -1;
		}
		if (max_sectors > DA_STATUS_MAX_SECTORS_MAX) {
			printk(KERN_ERR "dev[%p]: Passed max_sectors: %u"
				" greater than DA_STATUS_MAX_SECTORS_MAX:"
				" %u\n", dev, max_sectors,
				DA_STATUS_MAX_SECTORS_MAX);
			return -1;
		}
	}

	DEV_ATTRIB(dev)->max_sectors = max_sectors;
	printk("dev[%p]: SE Device max_sectors changed to %u\n",
=======
		pr_err("dev[%p]: Unable to change SE Device"
			" max_sectors while dev_export_obj: %d count exists\n",
			dev, atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
	if (!max_sectors) {
		pr_err("dev[%p]: Illegal ZERO value for"
			" max_sectors\n", dev);
		return -EINVAL;
	}
	if (max_sectors < DA_STATUS_MAX_SECTORS_MIN) {
		pr_err("dev[%p]: Passed max_sectors: %u less than"
			" DA_STATUS_MAX_SECTORS_MIN: %u\n", dev, max_sectors,
				DA_STATUS_MAX_SECTORS_MIN);
		return -EINVAL;
	}
	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		if (max_sectors > dev->se_sub_dev->se_dev_attrib.hw_max_sectors) {
			pr_err("dev[%p]: Passed max_sectors: %u"
				" greater than TCM/SE_Device max_sectors:"
				" %u\n", dev, max_sectors,
				dev->se_sub_dev->se_dev_attrib.hw_max_sectors);
			 return -EINVAL;
		}
	} else {
		if (!force && (max_sectors >
				 dev->se_sub_dev->se_dev_attrib.hw_max_sectors)) {
			pr_err("dev[%p]: Passed max_sectors: %u"
				" greater than TCM/SE_Device max_sectors"
				": %u, use force=1 to override.\n", dev,
				max_sectors, dev->se_sub_dev->se_dev_attrib.hw_max_sectors);
			return -EINVAL;
		}
		if (max_sectors > DA_STATUS_MAX_SECTORS_MAX) {
			pr_err("dev[%p]: Passed max_sectors: %u"
				" greater than DA_STATUS_MAX_SECTORS_MAX:"
				" %u\n", dev, max_sectors,
				DA_STATUS_MAX_SECTORS_MAX);
			return -EINVAL;
		}
	}
	/*
	 * Align max_sectors down to PAGE_SIZE to follow transport_allocate_data_tasks()
	 */
	max_sectors = se_dev_align_max_sectors(max_sectors,
				dev->se_sub_dev->se_dev_attrib.block_size);

	dev->se_sub_dev->se_dev_attrib.max_sectors = max_sectors;
	pr_debug("dev[%p]: SE Device max_sectors changed to %u\n",
>>>>>>> refs/remotes/origin/cm-10.0
			dev, max_sectors);
	return 0;
}

<<<<<<< HEAD
int se_dev_set_optimal_sectors(struct se_device *dev, u32 optimal_sectors)
{
	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		printk(KERN_ERR "dev[%p]: Unable to change SE Device"
=======
int se_dev_set_fabric_max_sectors(struct se_device *dev, u32 fabric_max_sectors)
{
	int block_size = dev->se_sub_dev->se_dev_attrib.block_size;

	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device"
			" fabric_max_sectors while dev_export_obj: %d count exists\n",
			dev, atomic_read(&dev->dev_export_obj.obj_access_count));
=======
int se_dev_set_fabric_max_sectors(struct se_device *dev, u32 fabric_max_sectors)
{
	int block_size = dev->dev_attrib.block_size;

	if (dev->export_count) {
		pr_err("dev[%p]: Unable to change SE Device"
			" fabric_max_sectors while export_count is %d\n",
			dev, dev->export_count);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}
	if (!fabric_max_sectors) {
		pr_err("dev[%p]: Illegal ZERO value for"
			" fabric_max_sectors\n", dev);
		return -EINVAL;
	}
	if (fabric_max_sectors < DA_STATUS_MAX_SECTORS_MIN) {
		pr_err("dev[%p]: Passed fabric_max_sectors: %u less than"
			" DA_STATUS_MAX_SECTORS_MIN: %u\n", dev, fabric_max_sectors,
				DA_STATUS_MAX_SECTORS_MIN);
		return -EINVAL;
	}
	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
<<<<<<< HEAD
		if (fabric_max_sectors > dev->se_sub_dev->se_dev_attrib.hw_max_sectors) {
			pr_err("dev[%p]: Passed fabric_max_sectors: %u"
				" greater than TCM/SE_Device max_sectors:"
				" %u\n", dev, fabric_max_sectors,
				dev->se_sub_dev->se_dev_attrib.hw_max_sectors);
=======
		if (fabric_max_sectors > dev->dev_attrib.hw_max_sectors) {
			pr_err("dev[%p]: Passed fabric_max_sectors: %u"
				" greater than TCM/SE_Device max_sectors:"
				" %u\n", dev, fabric_max_sectors,
				dev->dev_attrib.hw_max_sectors);
>>>>>>> refs/remotes/origin/master
			 return -EINVAL;
		}
	} else {
		if (fabric_max_sectors > DA_STATUS_MAX_SECTORS_MAX) {
			pr_err("dev[%p]: Passed fabric_max_sectors: %u"
				" greater than DA_STATUS_MAX_SECTORS_MAX:"
				" %u\n", dev, fabric_max_sectors,
				DA_STATUS_MAX_SECTORS_MAX);
			return -EINVAL;
		}
	}
	/*
	 * Align max_sectors down to PAGE_SIZE to follow transport_allocate_data_tasks()
	 */
	if (!block_size) {
		block_size = 512;
		pr_warn("Defaulting to 512 for zero block_size\n");
	}
	fabric_max_sectors = se_dev_align_max_sectors(fabric_max_sectors,
						      block_size);

<<<<<<< HEAD
	dev->se_sub_dev->se_dev_attrib.fabric_max_sectors = fabric_max_sectors;
=======
	dev->dev_attrib.fabric_max_sectors = fabric_max_sectors;
>>>>>>> refs/remotes/origin/master
	pr_debug("dev[%p]: SE Device max_sectors changed to %u\n",
			dev, fabric_max_sectors);
	return 0;
}

int se_dev_set_optimal_sectors(struct se_device *dev, u32 optimal_sectors)
{
<<<<<<< HEAD
	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
		pr_err("dev[%p]: Unable to change SE Device"
>>>>>>> refs/remotes/origin/cm-10.0
			" optimal_sectors while dev_export_obj: %d count exists\n",
			dev, atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
	}
<<<<<<< HEAD
	if (TRANSPORT(dev)->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		printk(KERN_ERR "dev[%p]: Passed optimal_sectors cannot be"
				" changed for TCM/pSCSI\n", dev);
		return -EINVAL;
	}
	if (optimal_sectors > DEV_ATTRIB(dev)->max_sectors) {
		printk(KERN_ERR "dev[%p]: Passed optimal_sectors %u cannot be"
			" greater than max_sectors: %u\n", dev,
			optimal_sectors, DEV_ATTRIB(dev)->max_sectors);
		return -EINVAL;
	}

	DEV_ATTRIB(dev)->optimal_sectors = optimal_sectors;
	printk(KERN_INFO "dev[%p]: SE Device optimal_sectors changed to %u\n",
=======
=======
	if (dev->export_count) {
		pr_err("dev[%p]: Unable to change SE Device"
			" optimal_sectors while export_count is %d\n",
			dev, dev->export_count);
		return -EINVAL;
	}
>>>>>>> refs/remotes/origin/master
	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		pr_err("dev[%p]: Passed optimal_sectors cannot be"
				" changed for TCM/pSCSI\n", dev);
		return -EINVAL;
	}
<<<<<<< HEAD
	if (optimal_sectors > dev->se_sub_dev->se_dev_attrib.fabric_max_sectors) {
		pr_err("dev[%p]: Passed optimal_sectors %u cannot be"
			" greater than fabric_max_sectors: %u\n", dev,
			optimal_sectors, dev->se_sub_dev->se_dev_attrib.fabric_max_sectors);
		return -EINVAL;
	}

	dev->se_sub_dev->se_dev_attrib.optimal_sectors = optimal_sectors;
	pr_debug("dev[%p]: SE Device optimal_sectors changed to %u\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (optimal_sectors > dev->dev_attrib.fabric_max_sectors) {
		pr_err("dev[%p]: Passed optimal_sectors %u cannot be"
			" greater than fabric_max_sectors: %u\n", dev,
			optimal_sectors, dev->dev_attrib.fabric_max_sectors);
		return -EINVAL;
	}

	dev->dev_attrib.optimal_sectors = optimal_sectors;
	pr_debug("dev[%p]: SE Device optimal_sectors changed to %u\n",
>>>>>>> refs/remotes/origin/master
			dev, optimal_sectors);
	return 0;
}

int se_dev_set_block_size(struct se_device *dev, u32 block_size)
{
<<<<<<< HEAD
	if (atomic_read(&dev->dev_export_obj.obj_access_count)) {
<<<<<<< HEAD
		printk(KERN_ERR "dev[%p]: Unable to change SE Device block_size"
			" while dev_export_obj: %d count exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -1;
=======
		pr_err("dev[%p]: Unable to change SE Device block_size"
			" while dev_export_obj: %d count exists\n", dev,
			atomic_read(&dev->dev_export_obj.obj_access_count));
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (dev->export_count) {
		pr_err("dev[%p]: Unable to change SE Device block_size"
			" while export_count is %d\n",
			dev, dev->export_count);
		return -EINVAL;
>>>>>>> refs/remotes/origin/master
	}

	if ((block_size != 512) &&
	    (block_size != 1024) &&
	    (block_size != 2048) &&
	    (block_size != 4096)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "dev[%p]: Illegal value for block_device: %u"
			" for SE device, must be 512, 1024, 2048 or 4096\n",
			dev, block_size);
		return -1;
	}

	if (TRANSPORT(dev)->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		printk(KERN_ERR "dev[%p]: Not allowed to change block_size for"
			" Physical Device, use for Linux/SCSI to change"
			" block_size for underlying hardware\n", dev);
		return -1;
	}

	DEV_ATTRIB(dev)->block_size = block_size;
	printk(KERN_INFO "dev[%p]: SE Device block_size changed to %u\n",
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_err("dev[%p]: Illegal value for block_device: %u"
			" for SE device, must be 512, 1024, 2048 or 4096\n",
			dev, block_size);
		return -EINVAL;
	}

	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) {
		pr_err("dev[%p]: Not allowed to change block_size for"
			" Physical Device, use for Linux/SCSI to change"
			" block_size for underlying hardware\n", dev);
		return -EINVAL;
	}

<<<<<<< HEAD
	dev->se_sub_dev->se_dev_attrib.block_size = block_size;
	pr_debug("dev[%p]: SE Device block_size changed to %u\n",
>>>>>>> refs/remotes/origin/cm-10.0
			dev, block_size);
=======
	dev->dev_attrib.block_size = block_size;
	pr_debug("dev[%p]: SE Device block_size changed to %u\n",
			dev, block_size);

	if (dev->dev_attrib.max_bytes_per_io)
		dev->dev_attrib.hw_max_sectors =
			dev->dev_attrib.max_bytes_per_io / block_size;

>>>>>>> refs/remotes/origin/master
	return 0;
}

struct se_lun *core_dev_add_lun(
	struct se_portal_group *tpg,
<<<<<<< HEAD
	struct se_hba *hba,
=======
>>>>>>> refs/remotes/origin/master
	struct se_device *dev,
	u32 lun)
{
	struct se_lun *lun_p;
<<<<<<< HEAD
	u32 lun_access = 0;
<<<<<<< HEAD

	if (atomic_read(&dev->dev_access_obj.obj_access_count) != 0) {
		printk(KERN_ERR "Unable to export struct se_device while dev_access_obj: %d\n",
			atomic_read(&dev->dev_access_obj.obj_access_count));
		return NULL;
	}

	lun_p = core_tpg_pre_addlun(tpg, lun);
	if ((IS_ERR(lun_p)) || !(lun_p))
		return NULL;
=======
	int rc;

	if (atomic_read(&dev->dev_access_obj.obj_access_count) != 0) {
		pr_err("Unable to export struct se_device while dev_access_obj: %d\n",
			atomic_read(&dev->dev_access_obj.obj_access_count));
		return ERR_PTR(-EACCES);
	}

	lun_p = core_tpg_pre_addlun(tpg, lun);
	if (IS_ERR(lun_p))
		return lun_p;
>>>>>>> refs/remotes/origin/cm-10.0

	if (dev->dev_flags & DF_READ_ONLY)
		lun_access = TRANSPORT_LUNFLAGS_READ_ONLY;
	else
		lun_access = TRANSPORT_LUNFLAGS_READ_WRITE;

<<<<<<< HEAD
	if (core_tpg_post_addlun(tpg, lun_p, lun_access, dev) < 0)
		return NULL;

	printk(KERN_INFO "%s_TPG[%u]_LUN[%u] - Activated %s Logical Unit from"
		" CORE HBA: %u\n", TPG_TFO(tpg)->get_fabric_name(),
		TPG_TFO(tpg)->tpg_get_tag(tpg), lun_p->unpacked_lun,
		TPG_TFO(tpg)->get_fabric_name(), hba->hba_id);
=======
	rc = core_tpg_post_addlun(tpg, lun_p, lun_access, dev);
=======
	int rc;

	lun_p = core_tpg_pre_addlun(tpg, lun);
	if (IS_ERR(lun_p))
		return lun_p;

	rc = core_tpg_post_addlun(tpg, lun_p,
				TRANSPORT_LUNFLAGS_READ_WRITE, dev);
>>>>>>> refs/remotes/origin/master
	if (rc < 0)
		return ERR_PTR(rc);

	pr_debug("%s_TPG[%u]_LUN[%u] - Activated %s Logical Unit from"
		" CORE HBA: %u\n", tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg), lun_p->unpacked_lun,
<<<<<<< HEAD
		tpg->se_tpg_tfo->get_fabric_name(), hba->hba_id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		tpg->se_tpg_tfo->get_fabric_name(), dev->se_hba->hba_id);
>>>>>>> refs/remotes/origin/master
	/*
	 * Update LUN maps for dynamically added initiators when
	 * generate_node_acl is enabled.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (TPG_TFO(tpg)->tpg_check_demo_mode(tpg)) {
		struct se_node_acl *acl;
		spin_lock_bh(&tpg->acl_node_lock);
		list_for_each_entry(acl, &tpg->acl_node_list, acl_list) {
			if (acl->dynamic_node_acl) {
				spin_unlock_bh(&tpg->acl_node_lock);
				core_tpg_add_node_to_devs(acl, tpg);
				spin_lock_bh(&tpg->acl_node_lock);
			}
		}
		spin_unlock_bh(&tpg->acl_node_lock);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (tpg->se_tpg_tfo->tpg_check_demo_mode(tpg)) {
		struct se_node_acl *acl;
		spin_lock_irq(&tpg->acl_node_lock);
		list_for_each_entry(acl, &tpg->acl_node_list, acl_list) {
			if (acl->dynamic_node_acl &&
			    (!tpg->se_tpg_tfo->tpg_check_demo_mode_login_only ||
			     !tpg->se_tpg_tfo->tpg_check_demo_mode_login_only(tpg))) {
				spin_unlock_irq(&tpg->acl_node_lock);
				core_tpg_add_node_to_devs(acl, tpg);
				spin_lock_irq(&tpg->acl_node_lock);
			}
		}
		spin_unlock_irq(&tpg->acl_node_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return lun_p;
}

/*      core_dev_del_lun():
 *
 *
 */
int core_dev_del_lun(
	struct se_portal_group *tpg,
	u32 unpacked_lun)
{
	struct se_lun *lun;
<<<<<<< HEAD
<<<<<<< HEAD
	int ret = 0;

	lun = core_tpg_pre_dellun(tpg, unpacked_lun, &ret);
	if (!(lun))
		return ret;

	core_tpg_post_dellun(tpg, lun);

	printk(KERN_INFO "%s_TPG[%u]_LUN[%u] - Deactivated %s Logical Unit from"
		" device object\n", TPG_TFO(tpg)->get_fabric_name(),
		TPG_TFO(tpg)->tpg_get_tag(tpg), unpacked_lun,
		TPG_TFO(tpg)->get_fabric_name());
=======
=======
>>>>>>> refs/remotes/origin/master

	lun = core_tpg_pre_dellun(tpg, unpacked_lun);
	if (IS_ERR(lun))
		return PTR_ERR(lun);

	core_tpg_post_dellun(tpg, lun);

	pr_debug("%s_TPG[%u]_LUN[%u] - Deactivated %s Logical Unit from"
		" device object\n", tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg), unpacked_lun,
		tpg->se_tpg_tfo->get_fabric_name());
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

struct se_lun *core_get_lun_from_tpg(struct se_portal_group *tpg, u32 unpacked_lun)
{
	struct se_lun *lun;

	spin_lock(&tpg->tpg_lun_lock);
	if (unpacked_lun > (TRANSPORT_MAX_LUNS_PER_TPG-1)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "%s LUN: %u exceeds TRANSPORT_MAX_LUNS"
			"_PER_TPG-1: %u for Target Portal Group: %hu\n",
			TPG_TFO(tpg)->get_fabric_name(), unpacked_lun,
			TRANSPORT_MAX_LUNS_PER_TPG-1,
			TPG_TFO(tpg)->tpg_get_tag(tpg));
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	lun = &tpg->tpg_lun_list[unpacked_lun];

	if (lun->lun_status != TRANSPORT_LUN_STATUS_FREE) {
		printk(KERN_ERR "%s Logical Unit Number: %u is not free on"
			" Target Portal Group: %hu, ignoring request.\n",
			TPG_TFO(tpg)->get_fabric_name(), unpacked_lun,
			TPG_TFO(tpg)->tpg_get_tag(tpg));
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_err("%s LUN: %u exceeds TRANSPORT_MAX_LUNS"
			"_PER_TPG-1: %u for Target Portal Group: %hu\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			TRANSPORT_MAX_LUNS_PER_TPG-1,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	lun = tpg->tpg_lun_list[unpacked_lun];

	if (lun->lun_status != TRANSPORT_LUN_STATUS_FREE) {
		pr_err("%s Logical Unit Number: %u is not free on"
			" Target Portal Group: %hu, ignoring request.\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	spin_unlock(&tpg->tpg_lun_lock);

	return lun;
}

/*      core_dev_get_lun():
 *
 *
 */
static struct se_lun *core_dev_get_lun(struct se_portal_group *tpg, u32 unpacked_lun)
{
	struct se_lun *lun;

	spin_lock(&tpg->tpg_lun_lock);
	if (unpacked_lun > (TRANSPORT_MAX_LUNS_PER_TPG-1)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "%s LUN: %u exceeds TRANSPORT_MAX_LUNS_PER"
			"_TPG-1: %u for Target Portal Group: %hu\n",
			TPG_TFO(tpg)->get_fabric_name(), unpacked_lun,
			TRANSPORT_MAX_LUNS_PER_TPG-1,
			TPG_TFO(tpg)->tpg_get_tag(tpg));
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	lun = &tpg->tpg_lun_list[unpacked_lun];

	if (lun->lun_status != TRANSPORT_LUN_STATUS_ACTIVE) {
		printk(KERN_ERR "%s Logical Unit Number: %u is not active on"
			" Target Portal Group: %hu, ignoring request.\n",
			TPG_TFO(tpg)->get_fabric_name(), unpacked_lun,
			TPG_TFO(tpg)->tpg_get_tag(tpg));
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_err("%s LUN: %u exceeds TRANSPORT_MAX_LUNS_PER"
			"_TPG-1: %u for Target Portal Group: %hu\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			TRANSPORT_MAX_LUNS_PER_TPG-1,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	lun = tpg->tpg_lun_list[unpacked_lun];

	if (lun->lun_status != TRANSPORT_LUN_STATUS_ACTIVE) {
		pr_err("%s Logical Unit Number: %u is not active on"
			" Target Portal Group: %hu, ignoring request.\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		spin_unlock(&tpg->tpg_lun_lock);
		return NULL;
	}
	spin_unlock(&tpg->tpg_lun_lock);

	return lun;
}

struct se_lun_acl *core_dev_init_initiator_node_lun_acl(
	struct se_portal_group *tpg,
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct se_node_acl *nacl,
>>>>>>> refs/remotes/origin/cm-11.0
	u32 mapped_lun,
	int *ret)
{
	struct se_lun_acl *lacl;

<<<<<<< HEAD
	if (strlen(initiatorname) >= TRANSPORT_IQN_LEN) {
		printk(KERN_ERR "%s InitiatorName exceeds maximum size.\n",
			TPG_TFO(tpg)->get_fabric_name());
		*ret = -EOVERFLOW;
		return NULL;
	}
	nacl = core_tpg_get_initiator_node_acl(tpg, initiatorname);
	if (!(nacl)) {
		*ret = -EINVAL;
		return NULL;
	}
=======
	if (strlen(nacl->initiatorname) >= TRANSPORT_IQN_LEN) {
		pr_err("%s InitiatorName exceeds maximum size.\n",
			tpg->se_tpg_tfo->get_fabric_name());
		*ret = -EOVERFLOW;
		return NULL;
	}
>>>>>>> refs/remotes/origin/cm-11.0
	lacl = kzalloc(sizeof(struct se_lun_acl), GFP_KERNEL);
	if (!(lacl)) {
		printk(KERN_ERR "Unable to allocate memory for struct se_lun_acl.\n");
=======
=======
>>>>>>> refs/remotes/origin/master
	struct se_node_acl *nacl,
	u32 mapped_lun,
	int *ret)
{
	struct se_lun_acl *lacl;

	if (strlen(nacl->initiatorname) >= TRANSPORT_IQN_LEN) {
		pr_err("%s InitiatorName exceeds maximum size.\n",
			tpg->se_tpg_tfo->get_fabric_name());
		*ret = -EOVERFLOW;
		return NULL;
	}
	lacl = kzalloc(sizeof(struct se_lun_acl), GFP_KERNEL);
	if (!lacl) {
		pr_err("Unable to allocate memory for struct se_lun_acl.\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		*ret = -ENOMEM;
		return NULL;
	}

	INIT_LIST_HEAD(&lacl->lacl_list);
	lacl->mapped_lun = mapped_lun;
	lacl->se_lun_nacl = nacl;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	snprintf(lacl->initiatorname, TRANSPORT_IQN_LEN, "%s", initiatorname);
=======
	snprintf(lacl->initiatorname, TRANSPORT_IQN_LEN, "%s",
		 nacl->initiatorname);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	snprintf(lacl->initiatorname, TRANSPORT_IQN_LEN, "%s",
		 nacl->initiatorname);
>>>>>>> refs/remotes/origin/master
=======
	snprintf(lacl->initiatorname, TRANSPORT_IQN_LEN, "%s",
		 nacl->initiatorname);
>>>>>>> refs/remotes/origin/cm-11.0

	return lacl;
}

int core_dev_add_initiator_node_lun_acl(
	struct se_portal_group *tpg,
	struct se_lun_acl *lacl,
	u32 unpacked_lun,
	u32 lun_access)
{
	struct se_lun *lun;
	struct se_node_acl *nacl;

	lun = core_dev_get_lun(tpg, unpacked_lun);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(lun)) {
		printk(KERN_ERR "%s Logical Unit Number: %u is not active on"
			" Target Portal Group: %hu, ignoring request.\n",
			TPG_TFO(tpg)->get_fabric_name(), unpacked_lun,
			TPG_TFO(tpg)->tpg_get_tag(tpg));
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!lun) {
		pr_err("%s Logical Unit Number: %u is not active on"
			" Target Portal Group: %hu, ignoring request.\n",
			tpg->se_tpg_tfo->get_fabric_name(), unpacked_lun,
			tpg->se_tpg_tfo->tpg_get_tag(tpg));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	nacl = lacl->se_lun_nacl;
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(nacl))
=======
	if (!nacl)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!nacl)
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	if ((lun->lun_access & TRANSPORT_LUNFLAGS_READ_ONLY) &&
	    (lun_access & TRANSPORT_LUNFLAGS_READ_WRITE))
		lun_access = TRANSPORT_LUNFLAGS_READ_ONLY;

	lacl->se_lun = lun;

<<<<<<< HEAD
	if (core_update_device_list_for_node(lun, lacl, lacl->mapped_lun,
			lun_access, nacl, tpg, 1) < 0)
=======
	if (core_enable_device_list_for_node(lun, lacl, lacl->mapped_lun,
			lun_access, nacl, tpg) < 0)
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	spin_lock(&lun->lun_acl_lock);
	list_add_tail(&lacl->lacl_list, &lun->lun_acl_list);
	atomic_inc(&lun->lun_acl_count);
	smp_mb__after_atomic_inc();
	spin_unlock(&lun->lun_acl_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "%s_TPG[%hu]_LUN[%u->%u] - Added %s ACL for "
		" InitiatorNode: %s\n", TPG_TFO(tpg)->get_fabric_name(),
		TPG_TFO(tpg)->tpg_get_tag(tpg), unpacked_lun, lacl->mapped_lun,
=======
	pr_debug("%s_TPG[%hu]_LUN[%u->%u] - Added %s ACL for "
		" InitiatorNode: %s\n", tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg), unpacked_lun, lacl->mapped_lun,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("%s_TPG[%hu]_LUN[%u->%u] - Added %s ACL for "
		" InitiatorNode: %s\n", tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg), unpacked_lun, lacl->mapped_lun,
>>>>>>> refs/remotes/origin/master
		(lun_access & TRANSPORT_LUNFLAGS_READ_WRITE) ? "RW" : "RO",
		lacl->initiatorname);
	/*
	 * Check to see if there are any existing persistent reservation APTPL
	 * pre-registrations that need to be enabled for this LUN ACL..
	 */
	core_scsi3_check_aptpl_registration(lun->lun_se_dev, tpg, lun, lacl);
	return 0;
}

/*      core_dev_del_initiator_node_lun_acl():
 *
 *
 */
int core_dev_del_initiator_node_lun_acl(
	struct se_portal_group *tpg,
	struct se_lun *lun,
	struct se_lun_acl *lacl)
{
	struct se_node_acl *nacl;

	nacl = lacl->se_lun_nacl;
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(nacl))
=======
	if (!nacl)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!nacl)
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	spin_lock(&lun->lun_acl_lock);
	list_del(&lacl->lacl_list);
	atomic_dec(&lun->lun_acl_count);
	smp_mb__after_atomic_dec();
	spin_unlock(&lun->lun_acl_lock);

<<<<<<< HEAD
	core_update_device_list_for_node(lun, NULL, lacl->mapped_lun,
		TRANSPORT_LUNFLAGS_NO_ACCESS, nacl, tpg, 0);

	lacl->se_lun = NULL;

<<<<<<< HEAD
	printk(KERN_INFO "%s_TPG[%hu]_LUN[%u] - Removed ACL for"
		" InitiatorNode: %s Mapped LUN: %u\n",
		TPG_TFO(tpg)->get_fabric_name(),
		TPG_TFO(tpg)->tpg_get_tag(tpg), lun->unpacked_lun,
=======
=======
	core_disable_device_list_for_node(lun, NULL, lacl->mapped_lun,
		TRANSPORT_LUNFLAGS_NO_ACCESS, nacl, tpg);

	lacl->se_lun = NULL;

>>>>>>> refs/remotes/origin/master
	pr_debug("%s_TPG[%hu]_LUN[%u] - Removed ACL for"
		" InitiatorNode: %s Mapped LUN: %u\n",
		tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg), lun->unpacked_lun,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		lacl->initiatorname, lacl->mapped_lun);

	return 0;
}

void core_dev_free_initiator_node_lun_acl(
	struct se_portal_group *tpg,
	struct se_lun_acl *lacl)
{
<<<<<<< HEAD
<<<<<<< HEAD
	printk("%s_TPG[%hu] - Freeing ACL for %s InitiatorNode: %s"
		" Mapped LUN: %u\n", TPG_TFO(tpg)->get_fabric_name(),
		TPG_TFO(tpg)->tpg_get_tag(tpg),
		TPG_TFO(tpg)->get_fabric_name(),
=======
=======
>>>>>>> refs/remotes/origin/master
	pr_debug("%s_TPG[%hu] - Freeing ACL for %s InitiatorNode: %s"
		" Mapped LUN: %u\n", tpg->se_tpg_tfo->get_fabric_name(),
		tpg->se_tpg_tfo->tpg_get_tag(tpg),
		tpg->se_tpg_tfo->get_fabric_name(),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		lacl->initiatorname, lacl->mapped_lun);

	kfree(lacl);
}

<<<<<<< HEAD
int core_dev_setup_virtual_lun0(void)
{
	struct se_hba *hba;
	struct se_device *dev;
	struct se_subsystem_dev *se_dev = NULL;
	struct se_subsystem_api *t;
	char buf[16];
	int ret;

<<<<<<< HEAD
	hba = core_alloc_hba("rd_dr", 0, HBA_FLAGS_INTERNAL_USE);
	if (IS_ERR(hba))
		return PTR_ERR(hba);

	se_global->g_lun0_hba = hba;
	t = hba->transport;

	se_dev = kzalloc(sizeof(struct se_subsystem_dev), GFP_KERNEL);
	if (!(se_dev)) {
		printk(KERN_ERR "Unable to allocate memory for"
=======
	hba = core_alloc_hba("rd_mcp", 0, HBA_FLAGS_INTERNAL_USE);
	if (IS_ERR(hba))
		return PTR_ERR(hba);

	lun0_hba = hba;
	t = hba->transport;

	se_dev = kzalloc(sizeof(struct se_subsystem_dev), GFP_KERNEL);
	if (!se_dev) {
		pr_err("Unable to allocate memory for"
>>>>>>> refs/remotes/origin/cm-10.0
				" struct se_subsystem_dev\n");
		ret = -ENOMEM;
		goto out;
	}
<<<<<<< HEAD
	INIT_LIST_HEAD(&se_dev->g_se_dev_list);
	INIT_LIST_HEAD(&se_dev->t10_wwn.t10_vpd_list);
	spin_lock_init(&se_dev->t10_wwn.t10_vpd_lock);
	INIT_LIST_HEAD(&se_dev->t10_reservation.registration_list);
	INIT_LIST_HEAD(&se_dev->t10_reservation.aptpl_reg_list);
	spin_lock_init(&se_dev->t10_reservation.registration_lock);
	spin_lock_init(&se_dev->t10_reservation.aptpl_reg_lock);
	INIT_LIST_HEAD(&se_dev->t10_alua.tg_pt_gps_list);
	spin_lock_init(&se_dev->t10_alua.tg_pt_gps_lock);
	spin_lock_init(&se_dev->se_dev_lock);
	se_dev->t10_reservation.pr_aptpl_buf_len = PR_APTPL_BUF_LEN;
=======
	INIT_LIST_HEAD(&se_dev->t10_wwn.t10_vpd_list);
	spin_lock_init(&se_dev->t10_wwn.t10_vpd_lock);
	INIT_LIST_HEAD(&se_dev->t10_pr.registration_list);
	INIT_LIST_HEAD(&se_dev->t10_pr.aptpl_reg_list);
	spin_lock_init(&se_dev->t10_pr.registration_lock);
	spin_lock_init(&se_dev->t10_pr.aptpl_reg_lock);
	INIT_LIST_HEAD(&se_dev->t10_alua.tg_pt_gps_list);
	spin_lock_init(&se_dev->t10_alua.tg_pt_gps_lock);
	spin_lock_init(&se_dev->se_dev_lock);
	se_dev->t10_pr.pr_aptpl_buf_len = PR_APTPL_BUF_LEN;
>>>>>>> refs/remotes/origin/cm-10.0
	se_dev->t10_wwn.t10_sub_dev = se_dev;
	se_dev->t10_alua.t10_sub_dev = se_dev;
	se_dev->se_dev_attrib.da_sub_dev = se_dev;
	se_dev->se_dev_hba = hba;

	se_dev->se_dev_su_ptr = t->allocate_virtdevice(hba, "virt_lun0");
<<<<<<< HEAD
	if (!(se_dev->se_dev_su_ptr)) {
		printk(KERN_ERR "Unable to locate subsystem dependent pointer"
=======
	if (!se_dev->se_dev_su_ptr) {
		pr_err("Unable to locate subsystem dependent pointer"
>>>>>>> refs/remotes/origin/cm-10.0
			" from allocate_virtdevice()\n");
		ret = -ENOMEM;
		goto out;
	}
<<<<<<< HEAD
	se_global->g_lun0_su_dev = se_dev;
=======
	lun0_su_dev = se_dev;
>>>>>>> refs/remotes/origin/cm-10.0

	memset(buf, 0, 16);
	sprintf(buf, "rd_pages=8");
	t->set_configfs_dev_params(hba, se_dev, buf, sizeof(buf));

	dev = t->create_virtdevice(hba, se_dev, se_dev->se_dev_su_ptr);
<<<<<<< HEAD
	if (!(dev) || IS_ERR(dev)) {
		ret = -ENOMEM;
		goto out;
	}
	se_dev->se_dev_ptr = dev;
	se_global->g_lun0_dev = dev;

	return 0;
out:
	se_global->g_lun0_su_dev = NULL;
	kfree(se_dev);
	if (se_global->g_lun0_hba) {
		core_delete_hba(se_global->g_lun0_hba);
		se_global->g_lun0_hba = NULL;
=======
	if (IS_ERR(dev)) {
		ret = PTR_ERR(dev);
		goto out;
	}
	dev->dev_link_magic = SE_DEV_LINK_MAGIC;
	se_dev->se_dev_ptr = dev;
	g_lun0_dev = dev;

	return 0;
out:
	lun0_su_dev = NULL;
	kfree(se_dev);
	if (lun0_hba) {
		core_delete_hba(lun0_hba);
		lun0_hba = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
static void scsi_dump_inquiry(struct se_device *dev)
{
	struct t10_wwn *wwn = &dev->t10_wwn;
	char buf[17];
	int i, device_type;
	/*
	 * Print Linux/SCSI style INQUIRY formatting to the kernel ring buffer
	 */
	for (i = 0; i < 8; i++)
		if (wwn->vendor[i] >= 0x20)
			buf[i] = wwn->vendor[i];
		else
			buf[i] = ' ';
	buf[i] = '\0';
	pr_debug("  Vendor: %s\n", buf);

	for (i = 0; i < 16; i++)
		if (wwn->model[i] >= 0x20)
			buf[i] = wwn->model[i];
		else
			buf[i] = ' ';
	buf[i] = '\0';
	pr_debug("  Model: %s\n", buf);

	for (i = 0; i < 4; i++)
		if (wwn->revision[i] >= 0x20)
			buf[i] = wwn->revision[i];
		else
			buf[i] = ' ';
	buf[i] = '\0';
	pr_debug("  Revision: %s\n", buf);

	device_type = dev->transport->get_device_type(dev);
	pr_debug("  Type:   %s ", scsi_device_type(device_type));
}

struct se_device *target_alloc_device(struct se_hba *hba, const char *name)
{
	struct se_device *dev;
	struct se_lun *xcopy_lun;

	dev = hba->transport->alloc_device(hba, name);
	if (!dev)
		return NULL;

	dev->dev_link_magic = SE_DEV_LINK_MAGIC;
	dev->se_hba = hba;
	dev->transport = hba->transport;

	INIT_LIST_HEAD(&dev->dev_list);
	INIT_LIST_HEAD(&dev->dev_sep_list);
	INIT_LIST_HEAD(&dev->dev_tmr_list);
	INIT_LIST_HEAD(&dev->delayed_cmd_list);
	INIT_LIST_HEAD(&dev->state_list);
	INIT_LIST_HEAD(&dev->qf_cmd_list);
	INIT_LIST_HEAD(&dev->g_dev_node);
	spin_lock_init(&dev->execute_task_lock);
	spin_lock_init(&dev->delayed_cmd_lock);
	spin_lock_init(&dev->dev_reservation_lock);
	spin_lock_init(&dev->se_port_lock);
	spin_lock_init(&dev->se_tmr_lock);
	spin_lock_init(&dev->qf_cmd_lock);
	sema_init(&dev->caw_sem, 1);
	atomic_set(&dev->dev_ordered_id, 0);
	INIT_LIST_HEAD(&dev->t10_wwn.t10_vpd_list);
	spin_lock_init(&dev->t10_wwn.t10_vpd_lock);
	INIT_LIST_HEAD(&dev->t10_pr.registration_list);
	INIT_LIST_HEAD(&dev->t10_pr.aptpl_reg_list);
	spin_lock_init(&dev->t10_pr.registration_lock);
	spin_lock_init(&dev->t10_pr.aptpl_reg_lock);
	INIT_LIST_HEAD(&dev->t10_alua.tg_pt_gps_list);
	spin_lock_init(&dev->t10_alua.tg_pt_gps_lock);

	dev->t10_wwn.t10_dev = dev;
	dev->t10_alua.t10_dev = dev;

	dev->dev_attrib.da_dev = dev;
	dev->dev_attrib.emulate_model_alias = DA_EMULATE_MODEL_ALIAS;
	dev->dev_attrib.emulate_dpo = DA_EMULATE_DPO;
	dev->dev_attrib.emulate_fua_write = DA_EMULATE_FUA_WRITE;
	dev->dev_attrib.emulate_fua_read = DA_EMULATE_FUA_READ;
	dev->dev_attrib.emulate_write_cache = DA_EMULATE_WRITE_CACHE;
	dev->dev_attrib.emulate_ua_intlck_ctrl = DA_EMULATE_UA_INTLLCK_CTRL;
	dev->dev_attrib.emulate_tas = DA_EMULATE_TAS;
	dev->dev_attrib.emulate_tpu = DA_EMULATE_TPU;
	dev->dev_attrib.emulate_tpws = DA_EMULATE_TPWS;
	dev->dev_attrib.emulate_caw = DA_EMULATE_CAW;
	dev->dev_attrib.emulate_3pc = DA_EMULATE_3PC;
	dev->dev_attrib.enforce_pr_isids = DA_ENFORCE_PR_ISIDS;
	dev->dev_attrib.is_nonrot = DA_IS_NONROT;
	dev->dev_attrib.emulate_rest_reord = DA_EMULATE_REST_REORD;
	dev->dev_attrib.max_unmap_lba_count = DA_MAX_UNMAP_LBA_COUNT;
	dev->dev_attrib.max_unmap_block_desc_count =
		DA_MAX_UNMAP_BLOCK_DESC_COUNT;
	dev->dev_attrib.unmap_granularity = DA_UNMAP_GRANULARITY_DEFAULT;
	dev->dev_attrib.unmap_granularity_alignment =
				DA_UNMAP_GRANULARITY_ALIGNMENT_DEFAULT;
	dev->dev_attrib.max_write_same_len = DA_MAX_WRITE_SAME_LEN;
	dev->dev_attrib.fabric_max_sectors = DA_FABRIC_MAX_SECTORS;
	dev->dev_attrib.optimal_sectors = DA_FABRIC_MAX_SECTORS;

	xcopy_lun = &dev->xcopy_lun;
	xcopy_lun->lun_se_dev = dev;
	init_completion(&xcopy_lun->lun_shutdown_comp);
	INIT_LIST_HEAD(&xcopy_lun->lun_acl_list);
	spin_lock_init(&xcopy_lun->lun_acl_lock);
	spin_lock_init(&xcopy_lun->lun_sep_lock);
	init_completion(&xcopy_lun->lun_ref_comp);

	return dev;
}

int target_configure_device(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;
	int ret;

	if (dev->dev_flags & DF_CONFIGURED) {
		pr_err("se_dev->se_dev_ptr already set for storage"
				" object\n");
		return -EEXIST;
	}

	ret = dev->transport->configure_device(dev);
	if (ret)
		goto out;
	dev->dev_flags |= DF_CONFIGURED;

	/*
	 * XXX: there is not much point to have two different values here..
	 */
	dev->dev_attrib.block_size = dev->dev_attrib.hw_block_size;
	dev->dev_attrib.queue_depth = dev->dev_attrib.hw_queue_depth;

	/*
	 * Align max_hw_sectors down to PAGE_SIZE I/O transfers
	 */
	dev->dev_attrib.hw_max_sectors =
		se_dev_align_max_sectors(dev->dev_attrib.hw_max_sectors,
					 dev->dev_attrib.hw_block_size);

	dev->dev_index = scsi_get_new_index(SCSI_DEVICE_INDEX);
	dev->creation_time = get_jiffies_64();

	ret = core_setup_alua(dev);
	if (ret)
		goto out;

	/*
	 * Startup the struct se_device processing thread
	 */
	dev->tmr_wq = alloc_workqueue("tmr-%s", WQ_MEM_RECLAIM | WQ_UNBOUND, 1,
				      dev->transport->name);
	if (!dev->tmr_wq) {
		pr_err("Unable to create tmr workqueue for %s\n",
			dev->transport->name);
		ret = -ENOMEM;
		goto out_free_alua;
	}

	/*
	 * Setup work_queue for QUEUE_FULL
	 */
	INIT_WORK(&dev->qf_work_queue, target_qf_do_work);

	/*
	 * Preload the initial INQUIRY const values if we are doing
	 * anything virtual (IBLOCK, FILEIO, RAMDISK), but not for TCM/pSCSI
	 * passthrough because this is being provided by the backend LLD.
	 */
	if (dev->transport->transport_type != TRANSPORT_PLUGIN_PHBA_PDEV) {
		strncpy(&dev->t10_wwn.vendor[0], "LIO-ORG", 8);
		strncpy(&dev->t10_wwn.model[0],
			dev->transport->inquiry_prod, 16);
		strncpy(&dev->t10_wwn.revision[0],
			dev->transport->inquiry_rev, 4);
	}

	scsi_dump_inquiry(dev);

	spin_lock(&hba->device_lock);
	hba->dev_count++;
	spin_unlock(&hba->device_lock);

	mutex_lock(&g_device_mutex);
	list_add_tail(&dev->g_dev_node, &g_device_list);
	mutex_unlock(&g_device_mutex);

	return 0;

out_free_alua:
	core_alua_free_lu_gp_mem(dev);
out:
	se_release_vpd_for_dev(dev);
	return ret;
}

void target_free_device(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;

	WARN_ON(!list_empty(&dev->dev_sep_list));

	if (dev->dev_flags & DF_CONFIGURED) {
		destroy_workqueue(dev->tmr_wq);

		mutex_lock(&g_device_mutex);
		list_del(&dev->g_dev_node);
		mutex_unlock(&g_device_mutex);

		spin_lock(&hba->device_lock);
		hba->dev_count--;
		spin_unlock(&hba->device_lock);
	}

	core_alua_free_lu_gp_mem(dev);
	core_scsi3_free_all_registrations(dev);
	se_release_vpd_for_dev(dev);

	dev->transport->free_device(dev);
}

int core_dev_setup_virtual_lun0(void)
{
	struct se_hba *hba;
	struct se_device *dev;
	char buf[] = "rd_pages=8,rd_nullio=1";
	int ret;

	hba = core_alloc_hba("rd_mcp", 0, HBA_FLAGS_INTERNAL_USE);
	if (IS_ERR(hba))
		return PTR_ERR(hba);

	dev = target_alloc_device(hba, "virt_lun0");
	if (!dev) {
		ret = -ENOMEM;
		goto out_free_hba;
	}

	hba->transport->set_configfs_dev_params(dev, buf, sizeof(buf));

	ret = target_configure_device(dev);
	if (ret)
		goto out_free_se_dev;

	lun0_hba = hba;
	g_lun0_dev = dev;
	return 0;

out_free_se_dev:
	target_free_device(dev);
out_free_hba:
	core_delete_hba(hba);
>>>>>>> refs/remotes/origin/master
	return ret;
}


void core_dev_release_virtual_lun0(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct se_hba *hba = se_global->g_lun0_hba;
	struct se_subsystem_dev *su_dev = se_global->g_lun0_su_dev;

	if (!(hba))
		return;

	if (se_global->g_lun0_dev)
		se_free_virtual_device(se_global->g_lun0_dev, hba);
=======
	struct se_hba *hba = lun0_hba;
	struct se_subsystem_dev *su_dev = lun0_su_dev;
=======
	struct se_hba *hba = lun0_hba;
>>>>>>> refs/remotes/origin/master

	if (!hba)
		return;

	if (g_lun0_dev)
<<<<<<< HEAD
		se_free_virtual_device(g_lun0_dev, hba);
>>>>>>> refs/remotes/origin/cm-10.0

	kfree(su_dev);
=======
		target_free_device(g_lun0_dev);
>>>>>>> refs/remotes/origin/master
	core_delete_hba(hba);
}
