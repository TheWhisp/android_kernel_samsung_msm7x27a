/*******************************************************************************
 * Filename:  target_core_alua.c
 *
 * This file contains SPC-3 compliant asymmetric logical unit assigntment (ALUA)
 *
<<<<<<< HEAD
 * Copyright (c) 2009-2010 Rising Tide Systems
 * Copyright (c) 2009-2010 Linux-iSCSI.org
=======
 * (c) Copyright 2009-2013 Datera, Inc.
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

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/configfs.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/configfs.h>
#include <linux/export.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/file.h>
>>>>>>> refs/remotes/origin/master
#include <scsi/scsi.h>
#include <scsi/scsi_cmnd.h>
#include <asm/unaligned.h>

#include <target/target_core_base.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <target/target_core_device.h>
#include <target/target_core_transport.h>
#include <target/target_core_fabric_ops.h>
#include <target/target_core_configfs.h>

#include "target_core_alua.h"
#include "target_core_hba.h"
=======
=======
>>>>>>> refs/remotes/origin/master
#include <target/target_core_backend.h>
#include <target/target_core_fabric.h>
#include <target/target_core_configfs.h>

#include "target_core_internal.h"
#include "target_core_alua.h"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#include "target_core_ua.h"

static int core_alua_check_transition(int state, int *primary);
static int core_alua_set_tg_pt_secondary_state(
		struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem,
		struct se_port *port, int explict, int offline);

<<<<<<< HEAD
=======
=======
#include "target_core_ua.h"

static sense_reason_t core_alua_check_transition(int state, int *primary);
static int core_alua_set_tg_pt_secondary_state(
		struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem,
		struct se_port *port, int explicit, int offline);

>>>>>>> refs/remotes/origin/master
static u16 alua_lu_gps_counter;
static u32 alua_lu_gps_count;

static DEFINE_SPINLOCK(lu_gps_lock);
static LIST_HEAD(lu_gps_list);

struct t10_alua_lu_gp *default_lu_gp;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * REPORT_TARGET_PORT_GROUPS
 *
 * See spc4r17 section 6.27
 */
<<<<<<< HEAD
<<<<<<< HEAD
int core_emulate_report_target_port_groups(struct se_cmd *cmd)
{
	struct se_subsystem_dev *su_dev = SE_DEV(cmd)->se_sub_dev;
	struct se_port *port;
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem;
	unsigned char *buf = (unsigned char *)T_TASK(cmd)->t_task_buf;
=======
int target_emulate_report_target_port_groups(struct se_task *task)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct se_subsystem_dev *su_dev = cmd->se_dev->se_sub_dev;
=======
sense_reason_t
target_emulate_report_target_port_groups(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
>>>>>>> refs/remotes/origin/master
	struct se_port *port;
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem;
	unsigned char *buf;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	u32 rd_len = 0, off = 4; /* Skip over RESERVED area to first
				    Target port group descriptor */
	/*
	 * Need at least 4 bytes of response data or else we can't
	 * even fit the return data length.
	 */
	if (cmd->data_length < 4) {
		pr_warn("REPORT TARGET PORT GROUPS allocation length %u"
			" too small\n", cmd->data_length);
		return -EINVAL;
	}

<<<<<<< HEAD
	spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
	list_for_each_entry(tg_pt_gp, &T10_ALUA(su_dev)->tg_pt_gps_list,
=======
	buf = transport_kmap_data_sg(cmd);

	spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
	list_for_each_entry(tg_pt_gp, &su_dev->t10_alua.tg_pt_gps_list,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 rd_len = 0, off;
	int ext_hdr = (cmd->t_task_cdb[1] & 0x20);

	/*
	 * Skip over RESERVED area to first Target port group descriptor
	 * depending on the PARAMETER DATA FORMAT type..
	 */
	if (ext_hdr != 0)
		off = 8;
	else
		off = 4;

	if (cmd->data_length < off) {
		pr_warn("REPORT TARGET PORT GROUPS allocation length %u too"
			" small for %s header\n", cmd->data_length,
			(ext_hdr) ? "extended" : "normal");
		return TCM_INVALID_CDB_FIELD;
	}
	buf = transport_kmap_data_sg(cmd);
	if (!buf)
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

	spin_lock(&dev->t10_alua.tg_pt_gps_lock);
	list_for_each_entry(tg_pt_gp, &dev->t10_alua.tg_pt_gps_list,
>>>>>>> refs/remotes/origin/master
			tg_pt_gp_list) {
		/*
		 * Check if the Target port group and Target port descriptor list
		 * based on tg_pt_gp_members count will fit into the response payload.
		 * Otherwise, bump rd_len to let the initiator know we have exceeded
		 * the allocation length and the response is truncated.
		 */
		if ((off + 8 + (tg_pt_gp->tg_pt_gp_members * 4)) >
		     cmd->data_length) {
			rd_len += 8 + (tg_pt_gp->tg_pt_gp_members * 4);
			continue;
		}
		/*
		 * PREF: Preferred target port bit, determine if this
		 * bit should be set for port group.
		 */
		if (tg_pt_gp->tg_pt_gp_pref)
			buf[off] = 0x80;
		/*
		 * Set the ASYMMETRIC ACCESS State
		 */
		buf[off++] |= (atomic_read(
			&tg_pt_gp->tg_pt_gp_alua_access_state) & 0xff);
		/*
		 * Set supported ASYMMETRIC ACCESS State bits
		 */
<<<<<<< HEAD
		buf[off] = 0x80; /* T_SUP */
		buf[off] |= 0x40; /* O_SUP */
		buf[off] |= 0x8; /* U_SUP */
		buf[off] |= 0x4; /* S_SUP */
		buf[off] |= 0x2; /* AN_SUP */
		buf[off++] |= 0x1; /* AO_SUP */
=======
		buf[off++] |= tg_pt_gp->tg_pt_gp_alua_supported_states;
>>>>>>> refs/remotes/origin/master
		/*
		 * TARGET PORT GROUP
		 */
		buf[off++] = ((tg_pt_gp->tg_pt_gp_id >> 8) & 0xff);
		buf[off++] = (tg_pt_gp->tg_pt_gp_id & 0xff);

		off++; /* Skip over Reserved */
		/*
		 * STATUS CODE
		 */
		buf[off++] = (tg_pt_gp->tg_pt_gp_alua_access_status & 0xff);
		/*
		 * Vendor Specific field
		 */
		buf[off++] = 0x00;
		/*
		 * TARGET PORT COUNT
		 */
		buf[off++] = (tg_pt_gp->tg_pt_gp_members & 0xff);
		rd_len += 8;

		spin_lock(&tg_pt_gp->tg_pt_gp_lock);
		list_for_each_entry(tg_pt_gp_mem, &tg_pt_gp->tg_pt_gp_mem_list,
				tg_pt_gp_mem_list) {
			port = tg_pt_gp_mem->tg_pt;
			/*
			 * Start Target Port descriptor format
			 *
			 * See spc4r17 section 6.2.7 Table 247
			 */
			off += 2; /* Skip over Obsolete */
			/*
			 * Set RELATIVE TARGET PORT IDENTIFIER
			 */
			buf[off++] = ((port->sep_rtpi >> 8) & 0xff);
			buf[off++] = (port->sep_rtpi & 0xff);
			rd_len += 4;
		}
		spin_unlock(&tg_pt_gp->tg_pt_gp_lock);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
	spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Set the RETURN DATA LENGTH set in the header of the DataIN Payload
	 */
	buf[0] = ((rd_len >> 24) & 0xff);
	buf[1] = ((rd_len >> 16) & 0xff);
	buf[2] = ((rd_len >> 8) & 0xff);
	buf[3] = (rd_len & 0xff);

<<<<<<< HEAD
=======
	transport_kunmap_data_sg(cmd);

	task->task_scsi_status = GOOD;
	transport_complete_task(task, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
	/*
	 * Set the RETURN DATA LENGTH set in the header of the DataIN Payload
	 */
	put_unaligned_be32(rd_len, &buf[0]);

	/*
	 * Fill in the Extended header parameter data format if requested
	 */
	if (ext_hdr != 0) {
		buf[4] = 0x10;
		/*
		 * Set the implicit transition time (in seconds) for the application
		 * client to use as a base for it's transition timeout value.
		 *
		 * Use the current tg_pt_gp_mem -> tg_pt_gp membership from the LUN
		 * this CDB was received upon to determine this value individually
		 * for ALUA target port group.
		 */
		port = cmd->se_lun->lun_sep;
		tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
		if (tg_pt_gp_mem) {
			spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
			tg_pt_gp = tg_pt_gp_mem->tg_pt_gp;
			if (tg_pt_gp)
				buf[5] = tg_pt_gp->tg_pt_gp_implicit_trans_secs;
			spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
		}
	}
	transport_kunmap_data_sg(cmd);

	target_complete_cmd(cmd, GOOD);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
<<<<<<< HEAD
 * SET_TARGET_PORT_GROUPS for explict ALUA operation.
 *
 * See spc4r17 section 6.35
 */
<<<<<<< HEAD
int core_emulate_set_target_port_groups(struct se_cmd *cmd)
{
	struct se_device *dev = SE_DEV(cmd);
	struct se_subsystem_dev *su_dev = SE_DEV(cmd)->se_sub_dev;
	struct se_port *port, *l_port = SE_LUN(cmd)->lun_sep;
	struct se_node_acl *nacl = SE_SESS(cmd)->se_node_acl;
	struct t10_alua_tg_pt_gp *tg_pt_gp = NULL, *l_tg_pt_gp;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem, *l_tg_pt_gp_mem;
	unsigned char *buf = (unsigned char *)T_TASK(cmd)->t_task_buf;
	unsigned char *ptr = &buf[4]; /* Skip over RESERVED area in header */
=======
int target_emulate_set_target_port_groups(struct se_task *task)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct se_device *dev = cmd->se_dev;
	struct se_subsystem_dev *su_dev = dev->se_sub_dev;
=======
 * SET_TARGET_PORT_GROUPS for explicit ALUA operation.
 *
 * See spc4r17 section 6.35
 */
sense_reason_t
target_emulate_set_target_port_groups(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
>>>>>>> refs/remotes/origin/master
	struct se_port *port, *l_port = cmd->se_lun->lun_sep;
	struct se_node_acl *nacl = cmd->se_sess->se_node_acl;
	struct t10_alua_tg_pt_gp *tg_pt_gp = NULL, *l_tg_pt_gp;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem, *l_tg_pt_gp_mem;
	unsigned char *buf;
	unsigned char *ptr;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	u32 len = 4; /* Skip over RESERVED area in header */
	int alua_access_state, primary = 0, rc;
	u16 tg_pt_id, rtpi;

<<<<<<< HEAD
	if (!(l_port))
		return PYX_TRANSPORT_LU_COMM_FAILURE;
=======
	if (!l_port) {
		cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		return -EINVAL;
	}
	buf = transport_kmap_data_sg(cmd);

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Determine if explict ALUA via SET_TARGET_PORT_GROUPS is allowed
	 * for the local tg_pt_gp.
	 */
	l_tg_pt_gp_mem = l_port->sep_alua_tg_pt_gp_mem;
<<<<<<< HEAD
	if (!(l_tg_pt_gp_mem)) {
		printk(KERN_ERR "Unable to access l_port->sep_alua_tg_pt_gp_mem\n");
		return PYX_TRANSPORT_UNKNOWN_SAM_OPCODE;
	}
	spin_lock(&l_tg_pt_gp_mem->tg_pt_gp_mem_lock);
	l_tg_pt_gp = l_tg_pt_gp_mem->tg_pt_gp;
	if (!(l_tg_pt_gp)) {
		spin_unlock(&l_tg_pt_gp_mem->tg_pt_gp_mem_lock);
		printk(KERN_ERR "Unable to access *l_tg_pt_gp_mem->tg_pt_gp\n");
		return PYX_TRANSPORT_UNKNOWN_SAM_OPCODE;
=======
	if (!l_tg_pt_gp_mem) {
		pr_err("Unable to access l_port->sep_alua_tg_pt_gp_mem\n");
		cmd->scsi_sense_reason = TCM_UNSUPPORTED_SCSI_OPCODE;
		rc = -EINVAL;
=======
	sense_reason_t rc = TCM_NO_SENSE;
	u32 len = 4; /* Skip over RESERVED area in header */
	int alua_access_state, primary = 0;
	u16 tg_pt_id, rtpi;

	if (!l_port)
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

	if (cmd->data_length < 4) {
		pr_warn("SET TARGET PORT GROUPS parameter list length %u too"
			" small\n", cmd->data_length);
		return TCM_INVALID_PARAMETER_LIST;
	}

	buf = transport_kmap_data_sg(cmd);
	if (!buf)
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

	/*
	 * Determine if explicit ALUA via SET_TARGET_PORT_GROUPS is allowed
	 * for the local tg_pt_gp.
	 */
	l_tg_pt_gp_mem = l_port->sep_alua_tg_pt_gp_mem;
	if (!l_tg_pt_gp_mem) {
		pr_err("Unable to access l_port->sep_alua_tg_pt_gp_mem\n");
		rc = TCM_UNSUPPORTED_SCSI_OPCODE;
>>>>>>> refs/remotes/origin/master
		goto out;
	}
	spin_lock(&l_tg_pt_gp_mem->tg_pt_gp_mem_lock);
	l_tg_pt_gp = l_tg_pt_gp_mem->tg_pt_gp;
	if (!l_tg_pt_gp) {
		spin_unlock(&l_tg_pt_gp_mem->tg_pt_gp_mem_lock);
		pr_err("Unable to access *l_tg_pt_gp_mem->tg_pt_gp\n");
<<<<<<< HEAD
		cmd->scsi_sense_reason = TCM_UNSUPPORTED_SCSI_OPCODE;
		rc = -EINVAL;
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
	}
	rc = (l_tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_EXPLICT_ALUA);
	spin_unlock(&l_tg_pt_gp_mem->tg_pt_gp_mem_lock);

<<<<<<< HEAD
	if (!(rc)) {
		printk(KERN_INFO "Unable to process SET_TARGET_PORT_GROUPS"
				" while TPGS_EXPLICT_ALUA is disabled\n");
		return PYX_TRANSPORT_UNKNOWN_SAM_OPCODE;
	}

=======
	if (!rc) {
		pr_debug("Unable to process SET_TARGET_PORT_GROUPS"
				" while TPGS_EXPLICT_ALUA is disabled\n");
		cmd->scsi_sense_reason = TCM_UNSUPPORTED_SCSI_OPCODE;
		rc = -EINVAL;
=======
		rc = TCM_UNSUPPORTED_SCSI_OPCODE;
		goto out;
	}
	spin_unlock(&l_tg_pt_gp_mem->tg_pt_gp_mem_lock);

	if (!(l_tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_EXPLICIT_ALUA)) {
		pr_debug("Unable to process SET_TARGET_PORT_GROUPS"
				" while TPGS_EXPLICIT_ALUA is disabled\n");
		rc = TCM_UNSUPPORTED_SCSI_OPCODE;
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	ptr = &buf[4]; /* Skip over RESERVED area in header */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	while (len < cmd->data_length) {
=======
	while (len < cmd->data_length) {
		bool found = false;
>>>>>>> refs/remotes/origin/master
		alua_access_state = (ptr[0] & 0x0f);
		/*
		 * Check the received ALUA access state, and determine if
		 * the state is a primary or secondary target port asymmetric
		 * access state.
		 */
		rc = core_alua_check_transition(alua_access_state, &primary);
<<<<<<< HEAD
		if (rc != 0) {
=======
		if (rc) {
>>>>>>> refs/remotes/origin/master
			/*
			 * If the SET TARGET PORT GROUPS attempts to establish
			 * an invalid combination of target port asymmetric
			 * access states or attempts to establish an
			 * unsupported target port asymmetric access state,
			 * then the command shall be terminated with CHECK
			 * CONDITION status, with the sense key set to ILLEGAL
			 * REQUEST, and the additional sense code set to INVALID
			 * FIELD IN PARAMETER LIST.
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			return PYX_TRANSPORT_INVALID_PARAMETER_LIST;
=======
			cmd->scsi_sense_reason = TCM_INVALID_PARAMETER_LIST;
			rc = -EINVAL;
			goto out;
>>>>>>> refs/remotes/origin/cm-10.0
		}
		rc = -1;
=======
			goto out;
		}

>>>>>>> refs/remotes/origin/master
		/*
		 * If the ASYMMETRIC ACCESS STATE field (see table 267)
		 * specifies a primary target port asymmetric access state,
		 * then the TARGET PORT GROUP OR TARGET PORT field specifies
		 * a primary target port group for which the primary target
		 * port asymmetric access state shall be changed. If the
		 * ASYMMETRIC ACCESS STATE field specifies a secondary target
		 * port asymmetric access state, then the TARGET PORT GROUP OR
		 * TARGET PORT field specifies the relative target port
		 * identifier (see 3.1.120) of the target port for which the
		 * secondary target port asymmetric access state shall be
		 * changed.
		 */
		if (primary) {
			tg_pt_id = get_unaligned_be16(ptr + 2);
			/*
			 * Locate the matching target port group ID from
			 * the global tg_pt_gp list
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
			list_for_each_entry(tg_pt_gp,
					&T10_ALUA(su_dev)->tg_pt_gps_list,
					tg_pt_gp_list) {
				if (!(tg_pt_gp->tg_pt_gp_valid_id))
=======
			spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
			list_for_each_entry(tg_pt_gp,
					&su_dev->t10_alua.tg_pt_gps_list,
					tg_pt_gp_list) {
				if (!tg_pt_gp->tg_pt_gp_valid_id)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			spin_lock(&dev->t10_alua.tg_pt_gps_lock);
			list_for_each_entry(tg_pt_gp,
					&dev->t10_alua.tg_pt_gps_list,
					tg_pt_gp_list) {
				if (!tg_pt_gp->tg_pt_gp_valid_id)
>>>>>>> refs/remotes/origin/master
					continue;

				if (tg_pt_id != tg_pt_gp->tg_pt_gp_id)
					continue;

				atomic_inc(&tg_pt_gp->tg_pt_gp_ref_cnt);
				smp_mb__after_atomic_inc();
<<<<<<< HEAD
<<<<<<< HEAD
				spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
				spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0

				rc = core_alua_do_port_transition(tg_pt_gp,
						dev, l_port, nacl,
						alua_access_state, 1);

<<<<<<< HEAD
				spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
				spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======

				spin_unlock(&dev->t10_alua.tg_pt_gps_lock);

				if (!core_alua_do_port_transition(tg_pt_gp,
						dev, l_port, nacl,
						alua_access_state, 1))
					found = true;

				spin_lock(&dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/master
				atomic_dec(&tg_pt_gp->tg_pt_gp_ref_cnt);
				smp_mb__after_atomic_dec();
				break;
			}
<<<<<<< HEAD
<<<<<<< HEAD
			spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
			spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
			/*
			 * If not matching target port group ID can be located
			 * throw an exception with ASCQ: INVALID_PARAMETER_LIST
			 */
<<<<<<< HEAD
			if (rc != 0)
				return PYX_TRANSPORT_INVALID_PARAMETER_LIST;
=======
			if (rc != 0) {
				cmd->scsi_sense_reason = TCM_INVALID_PARAMETER_LIST;
				rc = -EINVAL;
				goto out;
			}
>>>>>>> refs/remotes/origin/cm-10.0
		} else {
			/*
			 * Extact the RELATIVE TARGET PORT IDENTIFIER to identify
=======
			spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
		} else {
			/*
			 * Extract the RELATIVE TARGET PORT IDENTIFIER to identify
>>>>>>> refs/remotes/origin/master
			 * the Target Port in question for the the incoming
			 * SET_TARGET_PORT_GROUPS op.
			 */
			rtpi = get_unaligned_be16(ptr + 2);
			/*
<<<<<<< HEAD
			 * Locate the matching relative target port identifer
=======
			 * Locate the matching relative target port identifier
>>>>>>> refs/remotes/origin/master
			 * for the struct se_device storage object.
			 */
			spin_lock(&dev->se_port_lock);
			list_for_each_entry(port, &dev->dev_sep_list,
							sep_list) {
				if (port->sep_rtpi != rtpi)
					continue;

				tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
<<<<<<< HEAD
				spin_unlock(&dev->se_port_lock);

				rc = core_alua_set_tg_pt_secondary_state(
						tg_pt_gp_mem, port, 1, 1);
=======

				spin_unlock(&dev->se_port_lock);

				if (!core_alua_set_tg_pt_secondary_state(
						tg_pt_gp_mem, port, 1, 1))
					found = true;
>>>>>>> refs/remotes/origin/master

				spin_lock(&dev->se_port_lock);
				break;
			}
			spin_unlock(&dev->se_port_lock);
<<<<<<< HEAD
			/*
			 * If not matching relative target port identifier can
			 * be located, throw an exception with ASCQ:
			 * INVALID_PARAMETER_LIST
			 */
<<<<<<< HEAD
			if (rc != 0)
				return PYX_TRANSPORT_INVALID_PARAMETER_LIST;
=======
			if (rc != 0) {
				cmd->scsi_sense_reason = TCM_INVALID_PARAMETER_LIST;
				rc = -EINVAL;
				goto out;
			}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		}

		if (!found) {
			rc = TCM_INVALID_PARAMETER_LIST;
			goto out;
>>>>>>> refs/remotes/origin/master
		}

		ptr += 4;
		len += 4;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
=======
out:
	transport_kunmap_data_sg(cmd);
	if (!rc) {
		task->task_scsi_status = GOOD;
		transport_complete_task(task, 1);
	}
	return rc;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
out:
	transport_kunmap_data_sg(cmd);
	if (!rc)
		target_complete_cmd(cmd, GOOD);
	return rc;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static inline int core_alua_state_nonoptimized(
	struct se_cmd *cmd,
	unsigned char *cdb,
	int nonop_delay_msecs,
	u8 *alua_ascq)
{
	/*
	 * Set SCF_ALUA_NON_OPTIMIZED here, this value will be checked
	 * later to determine if processing of this cmd needs to be
	 * temporarily delayed for the Active/NonOptimized primary access state.
	 */
	cmd->se_cmd_flags |= SCF_ALUA_NON_OPTIMIZED;
	cmd->alua_nonop_delay = nonop_delay_msecs;
	return 0;
}

static inline int core_alua_state_standby(
	struct se_cmd *cmd,
	unsigned char *cdb,
	u8 *alua_ascq)
{
	/*
	 * Allowed CDBs for ALUA_ACCESS_STATE_STANDBY as defined by
	 * spc4r17 section 5.9.2.4.4
	 */
	switch (cdb[0]) {
	case INQUIRY:
	case LOG_SELECT:
	case LOG_SENSE:
	case MODE_SELECT:
	case MODE_SENSE:
	case REPORT_LUNS:
	case RECEIVE_DIAGNOSTIC:
	case SEND_DIAGNOSTIC:
		return 0;
	case MAINTENANCE_IN:
<<<<<<< HEAD
		switch (cdb[1]) {
=======
		switch (cdb[1] & 0x1f) {
>>>>>>> refs/remotes/origin/master
		case MI_REPORT_TARGET_PGS:
			return 0;
		default:
			*alua_ascq = ASCQ_04H_ALUA_TG_PT_STANDBY;
			return 1;
		}
	case MAINTENANCE_OUT:
		switch (cdb[1]) {
		case MO_SET_TARGET_PGS:
			return 0;
		default:
			*alua_ascq = ASCQ_04H_ALUA_TG_PT_STANDBY;
			return 1;
		}
	case REQUEST_SENSE:
	case PERSISTENT_RESERVE_IN:
	case PERSISTENT_RESERVE_OUT:
	case READ_BUFFER:
	case WRITE_BUFFER:
		return 0;
	default:
		*alua_ascq = ASCQ_04H_ALUA_TG_PT_STANDBY;
		return 1;
	}

	return 0;
}

static inline int core_alua_state_unavailable(
	struct se_cmd *cmd,
	unsigned char *cdb,
	u8 *alua_ascq)
{
	/*
	 * Allowed CDBs for ALUA_ACCESS_STATE_UNAVAILABLE as defined by
	 * spc4r17 section 5.9.2.4.5
	 */
	switch (cdb[0]) {
	case INQUIRY:
	case REPORT_LUNS:
		return 0;
	case MAINTENANCE_IN:
<<<<<<< HEAD
		switch (cdb[1]) {
=======
		switch (cdb[1] & 0x1f) {
>>>>>>> refs/remotes/origin/master
		case MI_REPORT_TARGET_PGS:
			return 0;
		default:
			*alua_ascq = ASCQ_04H_ALUA_TG_PT_UNAVAILABLE;
			return 1;
		}
	case MAINTENANCE_OUT:
		switch (cdb[1]) {
		case MO_SET_TARGET_PGS:
			return 0;
		default:
			*alua_ascq = ASCQ_04H_ALUA_TG_PT_UNAVAILABLE;
			return 1;
		}
	case REQUEST_SENSE:
	case READ_BUFFER:
	case WRITE_BUFFER:
		return 0;
	default:
		*alua_ascq = ASCQ_04H_ALUA_TG_PT_UNAVAILABLE;
		return 1;
	}

	return 0;
}

static inline int core_alua_state_transition(
	struct se_cmd *cmd,
	unsigned char *cdb,
	u8 *alua_ascq)
{
	/*
<<<<<<< HEAD
	 * Allowed CDBs for ALUA_ACCESS_STATE_TRANSITIO as defined by
=======
	 * Allowed CDBs for ALUA_ACCESS_STATE_TRANSITION as defined by
>>>>>>> refs/remotes/origin/master
	 * spc4r17 section 5.9.2.5
	 */
	switch (cdb[0]) {
	case INQUIRY:
	case REPORT_LUNS:
		return 0;
	case MAINTENANCE_IN:
<<<<<<< HEAD
		switch (cdb[1]) {
=======
		switch (cdb[1] & 0x1f) {
>>>>>>> refs/remotes/origin/master
		case MI_REPORT_TARGET_PGS:
			return 0;
		default:
			*alua_ascq = ASCQ_04H_ALUA_STATE_TRANSITION;
			return 1;
		}
	case REQUEST_SENSE:
	case READ_BUFFER:
	case WRITE_BUFFER:
		return 0;
	default:
		*alua_ascq = ASCQ_04H_ALUA_STATE_TRANSITION;
		return 1;
	}

	return 0;
}

/*
<<<<<<< HEAD
 * Used for alua_type SPC_ALUA_PASSTHROUGH and SPC2_ALUA_DISABLED
 * in transport_cmd_sequencer().  This function is assigned to
 * struct t10_alua *->state_check() in core_setup_alua()
 */
static int core_alua_state_check_nop(
	struct se_cmd *cmd,
	unsigned char *cdb,
	u8 *alua_ascq)
{
	return 0;
}

/*
 * Used for alua_type SPC3_ALUA_EMULATED in transport_cmd_sequencer().
 * This function is assigned to struct t10_alua *->state_check() in
 * core_setup_alua()
 *
 * Also, this function can return three different return codes to
 * signal transport_generic_cmd_sequencer()
 *
 * return 1: Is used to signal LUN not accecsable, and check condition/not ready
 * return 0: Used to signal success
 * reutrn -1: Used to signal failure, and invalid cdb field
 */
static int core_alua_state_check(
	struct se_cmd *cmd,
	unsigned char *cdb,
	u8 *alua_ascq)
{
<<<<<<< HEAD
	struct se_lun *lun = SE_LUN(cmd);
=======
	struct se_lun *lun = cmd->se_lun;
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * return 1: Is used to signal LUN not accessible, and check condition/not ready
 * return 0: Used to signal success
 * return -1: Used to signal failure, and invalid cdb field
 */
sense_reason_t
target_alua_state_check(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	unsigned char *cdb = cmd->t_task_cdb;
	struct se_lun *lun = cmd->se_lun;
>>>>>>> refs/remotes/origin/master
	struct se_port *port = lun->lun_sep;
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem;
	int out_alua_state, nonop_delay_msecs;
<<<<<<< HEAD

<<<<<<< HEAD
	if (!(port))
=======
	if (!port)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u8 alua_ascq;
	int ret;

	if (dev->se_hba->hba_flags & HBA_FLAGS_INTERNAL_USE)
		return 0;
	if (dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV)
		return 0;

	if (!port)
>>>>>>> refs/remotes/origin/master
		return 0;
	/*
	 * First, check for a struct se_port specific secondary ALUA target port
	 * access state: OFFLINE
	 */
	if (atomic_read(&port->sep_tg_pt_secondary_offline)) {
<<<<<<< HEAD
		*alua_ascq = ASCQ_04H_ALUA_OFFLINE;
<<<<<<< HEAD
		printk(KERN_INFO "ALUA: Got secondary offline status for local"
=======
		pr_debug("ALUA: Got secondary offline status for local"
>>>>>>> refs/remotes/origin/cm-10.0
				" target port\n");
		*alua_ascq = ASCQ_04H_ALUA_OFFLINE;
		return 1;
=======
		pr_debug("ALUA: Got secondary offline status for local"
				" target port\n");
		alua_ascq = ASCQ_04H_ALUA_OFFLINE;
		ret = 1;
		goto out;
>>>>>>> refs/remotes/origin/master
	}
	 /*
	 * Second, obtain the struct t10_alua_tg_pt_gp_member pointer to the
	 * ALUA target port group, to obtain current ALUA access state.
	 * Otherwise look for the underlying struct se_device association with
	 * a ALUA logical unit group.
	 */
	tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
<<<<<<< HEAD
=======
	if (!tg_pt_gp_mem)
		return 0;

>>>>>>> refs/remotes/origin/master
	spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
	tg_pt_gp = tg_pt_gp_mem->tg_pt_gp;
	out_alua_state = atomic_read(&tg_pt_gp->tg_pt_gp_alua_access_state);
	nonop_delay_msecs = tg_pt_gp->tg_pt_gp_nonop_delay_msecs;
	spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
	/*
<<<<<<< HEAD
	 * Process ALUA_ACCESS_STATE_ACTIVE_OPTMIZED in a separate conditional
=======
	 * Process ALUA_ACCESS_STATE_ACTIVE_OPTIMIZED in a separate conditional
>>>>>>> refs/remotes/origin/master
	 * statement so the compiler knows explicitly to check this case first.
	 * For the Optimized ALUA access state case, we want to process the
	 * incoming fabric cmd ASAP..
	 */
<<<<<<< HEAD
	if (out_alua_state == ALUA_ACCESS_STATE_ACTIVE_OPTMIZED)
=======
	if (out_alua_state == ALUA_ACCESS_STATE_ACTIVE_OPTIMIZED)
>>>>>>> refs/remotes/origin/master
		return 0;

	switch (out_alua_state) {
	case ALUA_ACCESS_STATE_ACTIVE_NON_OPTIMIZED:
<<<<<<< HEAD
		return core_alua_state_nonoptimized(cmd, cdb,
					nonop_delay_msecs, alua_ascq);
	case ALUA_ACCESS_STATE_STANDBY:
		return core_alua_state_standby(cmd, cdb, alua_ascq);
	case ALUA_ACCESS_STATE_UNAVAILABLE:
		return core_alua_state_unavailable(cmd, cdb, alua_ascq);
	case ALUA_ACCESS_STATE_TRANSITION:
		return core_alua_state_transition(cmd, cdb, alua_ascq);
=======
		ret = core_alua_state_nonoptimized(cmd, cdb,
					nonop_delay_msecs, &alua_ascq);
		break;
	case ALUA_ACCESS_STATE_STANDBY:
		ret = core_alua_state_standby(cmd, cdb, &alua_ascq);
		break;
	case ALUA_ACCESS_STATE_UNAVAILABLE:
		ret = core_alua_state_unavailable(cmd, cdb, &alua_ascq);
		break;
	case ALUA_ACCESS_STATE_TRANSITION:
		ret = core_alua_state_transition(cmd, cdb, &alua_ascq);
		break;
>>>>>>> refs/remotes/origin/master
	/*
	 * OFFLINE is a secondary ALUA target port group access state, that is
	 * handled above with struct se_port->sep_tg_pt_secondary_offline=1
	 */
	case ALUA_ACCESS_STATE_OFFLINE:
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unknown ALUA access state: 0x%02x\n",
				out_alua_state);
		return -1;
=======
		pr_err("Unknown ALUA access state: 0x%02x\n",
				out_alua_state);
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unknown ALUA access state: 0x%02x\n",
				out_alua_state);
		return TCM_INVALID_CDB_FIELD;
	}

out:
	if (ret > 0) {
		/*
		 * Set SCSI additional sense code (ASC) to 'LUN Not Accessible';
		 * The ALUA additional sense code qualifier (ASCQ) is determined
		 * by the ALUA primary or secondary access state..
		 */
		pr_debug("[%s]: ALUA TG Port not available, "
			"SenseKey: NOT_READY, ASC/ASCQ: "
			"0x04/0x%02x\n",
			cmd->se_tfo->get_fabric_name(), alua_ascq);

		cmd->scsi_asc = 0x04;
		cmd->scsi_ascq = alua_ascq;
		return TCM_CHECK_CONDITION_NOT_READY;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

/*
<<<<<<< HEAD
 * Check implict and explict ALUA state change request.
 */
static int core_alua_check_transition(int state, int *primary)
{
	switch (state) {
	case ALUA_ACCESS_STATE_ACTIVE_OPTMIZED:
=======
 * Check implicit and explicit ALUA state change request.
 */
static sense_reason_t
core_alua_check_transition(int state, int *primary)
{
	switch (state) {
	case ALUA_ACCESS_STATE_ACTIVE_OPTIMIZED:
>>>>>>> refs/remotes/origin/master
	case ALUA_ACCESS_STATE_ACTIVE_NON_OPTIMIZED:
	case ALUA_ACCESS_STATE_STANDBY:
	case ALUA_ACCESS_STATE_UNAVAILABLE:
		/*
		 * OPTIMIZED, NON-OPTIMIZED, STANDBY and UNAVAILABLE are
		 * defined as primary target port asymmetric access states.
		 */
		*primary = 1;
		break;
	case ALUA_ACCESS_STATE_OFFLINE:
		/*
		 * OFFLINE state is defined as a secondary target port
		 * asymmetric access state.
		 */
		*primary = 0;
		break;
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Unknown ALUA access state: 0x%02x\n", state);
		return -1;
=======
		pr_err("Unknown ALUA access state: 0x%02x\n", state);
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("Unknown ALUA access state: 0x%02x\n", state);
		return TCM_INVALID_PARAMETER_LIST;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

static char *core_alua_dump_state(int state)
{
	switch (state) {
<<<<<<< HEAD
	case ALUA_ACCESS_STATE_ACTIVE_OPTMIZED:
=======
	case ALUA_ACCESS_STATE_ACTIVE_OPTIMIZED:
>>>>>>> refs/remotes/origin/master
		return "Active/Optimized";
	case ALUA_ACCESS_STATE_ACTIVE_NON_OPTIMIZED:
		return "Active/NonOptimized";
	case ALUA_ACCESS_STATE_STANDBY:
		return "Standby";
	case ALUA_ACCESS_STATE_UNAVAILABLE:
		return "Unavailable";
	case ALUA_ACCESS_STATE_OFFLINE:
		return "Offline";
	default:
		return "Unknown";
	}

	return NULL;
}

char *core_alua_dump_status(int status)
{
	switch (status) {
	case ALUA_STATUS_NONE:
		return "None";
<<<<<<< HEAD
	case ALUA_STATUS_ALTERED_BY_EXPLICT_STPG:
		return "Altered by Explict STPG";
	case ALUA_STATUS_ALTERED_BY_IMPLICT_ALUA:
		return "Altered by Implict ALUA";
=======
	case ALUA_STATUS_ALTERED_BY_EXPLICIT_STPG:
		return "Altered by Explicit STPG";
	case ALUA_STATUS_ALTERED_BY_IMPLICIT_ALUA:
		return "Altered by Implicit ALUA";
>>>>>>> refs/remotes/origin/master
	default:
		return "Unknown";
	}

	return NULL;
}

/*
 * Used by fabric modules to determine when we need to delay processing
 * for the Active/NonOptimized paths..
 */
int core_alua_check_nonop_delay(
	struct se_cmd *cmd)
{
	if (!(cmd->se_cmd_flags & SCF_ALUA_NON_OPTIMIZED))
		return 0;
	if (in_interrupt())
		return 0;
	/*
	 * The ALUA Active/NonOptimized access state delay can be disabled
	 * in via configfs with a value of zero
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(cmd->alua_nonop_delay))
=======
	if (!cmd->alua_nonop_delay)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!cmd->alua_nonop_delay)
>>>>>>> refs/remotes/origin/master
		return 0;
	/*
	 * struct se_cmd->alua_nonop_delay gets set by a target port group
	 * defined interval in core_alua_state_nonoptimized()
	 */
	msleep_interruptible(cmd->alua_nonop_delay);
	return 0;
}
EXPORT_SYMBOL(core_alua_check_nonop_delay);

/*
 * Called with tg_pt_gp->tg_pt_gp_md_mutex or tg_pt_gp_mem->sep_tg_pt_md_mutex
 *
 */
static int core_alua_write_tpg_metadata(
	const char *path,
	unsigned char *md_buf,
	u32 md_buf_len)
{
<<<<<<< HEAD
	mm_segment_t old_fs;
	struct file *file;
	struct iovec iov[1];
	int flags = O_RDWR | O_CREAT | O_TRUNC, ret;

	memset(iov, 0, sizeof(struct iovec));

	file = filp_open(path, flags, 0600);
	if (IS_ERR(file) || !file || !file->f_dentry) {
<<<<<<< HEAD
		printk(KERN_ERR "filp_open(%s) for ALUA metadata failed\n",
=======
		pr_err("filp_open(%s) for ALUA metadata failed\n",
>>>>>>> refs/remotes/origin/cm-10.0
			path);
		return -ENODEV;
	}

	iov[0].iov_base = &md_buf[0];
	iov[0].iov_len = md_buf_len;

	old_fs = get_fs();
	set_fs(get_ds());
	ret = vfs_writev(file, &iov[0], 1, &file->f_pos);
	set_fs(old_fs);

	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "Error writing ALUA metadata file: %s\n", path);
=======
		pr_err("Error writing ALUA metadata file: %s\n", path);
>>>>>>> refs/remotes/origin/cm-10.0
		filp_close(file, NULL);
		return -EIO;
	}
	filp_close(file, NULL);

	return 0;
=======
	struct file *file = filp_open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
	int ret;

	if (IS_ERR(file)) {
		pr_err("filp_open(%s) for ALUA metadata failed\n", path);
		return -ENODEV;
	}
	ret = kernel_write(file, md_buf, md_buf_len, 0);
	if (ret < 0)
		pr_err("Error writing ALUA metadata file: %s\n", path);
	fput(file);
	return (ret < 0) ? -EIO : 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * Called with tg_pt_gp->tg_pt_gp_md_mutex held
 */
static int core_alua_update_tpg_primary_metadata(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	int primary_state,
	unsigned char *md_buf)
{
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = tg_pt_gp->tg_pt_gp_su_dev;
	struct t10_wwn *wwn = &su_dev->t10_wwn;
=======
	struct t10_wwn *wwn = &tg_pt_gp->tg_pt_gp_dev->t10_wwn;
>>>>>>> refs/remotes/origin/master
	char path[ALUA_METADATA_PATH_LEN];
	int len;

	memset(path, 0, ALUA_METADATA_PATH_LEN);

	len = snprintf(md_buf, tg_pt_gp->tg_pt_gp_md_buf_len,
			"tg_pt_gp_id=%hu\n"
			"alua_access_state=0x%02x\n"
			"alua_access_status=0x%02x\n",
			tg_pt_gp->tg_pt_gp_id, primary_state,
			tg_pt_gp->tg_pt_gp_alua_access_status);

	snprintf(path, ALUA_METADATA_PATH_LEN,
		"/var/target/alua/tpgs_%s/%s", &wwn->unit_serial[0],
		config_item_name(&tg_pt_gp->tg_pt_gp_group.cg_item));

	return core_alua_write_tpg_metadata(path, md_buf, len);
}

static int core_alua_do_transition_tg_pt(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	struct se_port *l_port,
	struct se_node_acl *nacl,
	unsigned char *md_buf,
	int new_state,
<<<<<<< HEAD
	int explict)
=======
	int explicit)
>>>>>>> refs/remotes/origin/master
{
	struct se_dev_entry *se_deve;
	struct se_lun_acl *lacl;
	struct se_port *port;
	struct t10_alua_tg_pt_gp_member *mem;
	int old_state = 0;
	/*
	 * Save the old primary ALUA access state, and set the current state
	 * to ALUA_ACCESS_STATE_TRANSITION.
	 */
	old_state = atomic_read(&tg_pt_gp->tg_pt_gp_alua_access_state);
	atomic_set(&tg_pt_gp->tg_pt_gp_alua_access_state,
			ALUA_ACCESS_STATE_TRANSITION);
<<<<<<< HEAD
	tg_pt_gp->tg_pt_gp_alua_access_status = (explict) ?
				ALUA_STATUS_ALTERED_BY_EXPLICT_STPG :
				ALUA_STATUS_ALTERED_BY_IMPLICT_ALUA;
=======
	tg_pt_gp->tg_pt_gp_alua_access_status = (explicit) ?
				ALUA_STATUS_ALTERED_BY_EXPLICIT_STPG :
				ALUA_STATUS_ALTERED_BY_IMPLICIT_ALUA;
>>>>>>> refs/remotes/origin/master
	/*
	 * Check for the optional ALUA primary state transition delay
	 */
	if (tg_pt_gp->tg_pt_gp_trans_delay_msecs != 0)
		msleep_interruptible(tg_pt_gp->tg_pt_gp_trans_delay_msecs);

	spin_lock(&tg_pt_gp->tg_pt_gp_lock);
	list_for_each_entry(mem, &tg_pt_gp->tg_pt_gp_mem_list,
				tg_pt_gp_mem_list) {
		port = mem->tg_pt;
		/*
		 * After an implicit target port asymmetric access state
		 * change, a device server shall establish a unit attention
		 * condition for the initiator port associated with every I_T
		 * nexus with the additional sense code set to ASYMMETRIC
<<<<<<< HEAD
		 * ACCESS STATE CHAGED.
=======
		 * ACCESS STATE CHANGED.
>>>>>>> refs/remotes/origin/master
		 *
		 * After an explicit target port asymmetric access state
		 * change, a device server shall establish a unit attention
		 * condition with the additional sense code set to ASYMMETRIC
		 * ACCESS STATE CHANGED for the initiator port associated with
		 * every I_T nexus other than the I_T nexus on which the SET
		 * TARGET PORT GROUPS command
		 */
		atomic_inc(&mem->tg_pt_gp_mem_ref_cnt);
		smp_mb__after_atomic_inc();
		spin_unlock(&tg_pt_gp->tg_pt_gp_lock);

		spin_lock_bh(&port->sep_alua_lock);
		list_for_each_entry(se_deve, &port->sep_alua_list,
					alua_port_list) {
			lacl = se_deve->se_lun_acl;
			/*
			 * se_deve->se_lun_acl pointer may be NULL for a
<<<<<<< HEAD
			 * entry created without explict Node+MappedLUN ACLs
			 */
<<<<<<< HEAD
			if (!(lacl))
=======
			if (!lacl)
>>>>>>> refs/remotes/origin/cm-10.0
				continue;

			if (explict &&
=======
			 * entry created without explicit Node+MappedLUN ACLs
			 */
			if (!lacl)
				continue;

			if (explicit &&
>>>>>>> refs/remotes/origin/master
			   (nacl != NULL) && (nacl == lacl->se_lun_nacl) &&
			   (l_port != NULL) && (l_port == port))
				continue;

			core_scsi3_ua_allocate(lacl->se_lun_nacl,
				se_deve->mapped_lun, 0x2A,
				ASCQ_2AH_ASYMMETRIC_ACCESS_STATE_CHANGED);
		}
		spin_unlock_bh(&port->sep_alua_lock);

		spin_lock(&tg_pt_gp->tg_pt_gp_lock);
		atomic_dec(&mem->tg_pt_gp_mem_ref_cnt);
		smp_mb__after_atomic_dec();
	}
	spin_unlock(&tg_pt_gp->tg_pt_gp_lock);
	/*
	 * Update the ALUA metadata buf that has been allocated in
	 * core_alua_do_port_transition(), this metadata will be written
	 * to struct file.
	 *
	 * Note that there is the case where we do not want to update the
	 * metadata when the saved metadata is being parsed in userspace
	 * when setting the existing port access state and access status.
	 *
	 * Also note that the failure to write out the ALUA metadata to
	 * struct file does NOT affect the actual ALUA transition.
	 */
	if (tg_pt_gp->tg_pt_gp_write_metadata) {
		mutex_lock(&tg_pt_gp->tg_pt_gp_md_mutex);
		core_alua_update_tpg_primary_metadata(tg_pt_gp,
					new_state, md_buf);
		mutex_unlock(&tg_pt_gp->tg_pt_gp_md_mutex);
	}
	/*
	 * Set the current primary ALUA access state to the requested new state
	 */
	atomic_set(&tg_pt_gp->tg_pt_gp_alua_access_state, new_state);

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "Successful %s ALUA transition TG PT Group: %s ID: %hu"
=======
	pr_debug("Successful %s ALUA transition TG PT Group: %s ID: %hu"
>>>>>>> refs/remotes/origin/cm-10.0
		" from primary access state %s to %s\n", (explict) ? "explict" :
		"implict", config_item_name(&tg_pt_gp->tg_pt_gp_group.cg_item),
=======
	pr_debug("Successful %s ALUA transition TG PT Group: %s ID: %hu"
		" from primary access state %s to %s\n", (explicit) ? "explicit" :
		"implicit", config_item_name(&tg_pt_gp->tg_pt_gp_group.cg_item),
>>>>>>> refs/remotes/origin/master
		tg_pt_gp->tg_pt_gp_id, core_alua_dump_state(old_state),
		core_alua_dump_state(new_state));

	return 0;
}

int core_alua_do_port_transition(
	struct t10_alua_tg_pt_gp *l_tg_pt_gp,
	struct se_device *l_dev,
	struct se_port *l_port,
	struct se_node_acl *l_nacl,
	int new_state,
<<<<<<< HEAD
	int explict)
{
	struct se_device *dev;
	struct se_port *port;
	struct se_subsystem_dev *su_dev;
=======
	int explicit)
{
	struct se_device *dev;
	struct se_port *port;
>>>>>>> refs/remotes/origin/master
	struct se_node_acl *nacl;
	struct t10_alua_lu_gp *lu_gp;
	struct t10_alua_lu_gp_member *lu_gp_mem, *local_lu_gp_mem;
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	unsigned char *md_buf;
	int primary;

	if (core_alua_check_transition(new_state, &primary) != 0)
		return -EINVAL;

	md_buf = kzalloc(l_tg_pt_gp->tg_pt_gp_md_buf_len, GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(md_buf)) {
		printk("Unable to allocate buf for ALUA metadata\n");
=======
	if (!md_buf) {
		pr_err("Unable to allocate buf for ALUA metadata\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!md_buf) {
		pr_err("Unable to allocate buf for ALUA metadata\n");
>>>>>>> refs/remotes/origin/master
		return -ENOMEM;
	}

	local_lu_gp_mem = l_dev->dev_alua_lu_gp_mem;
	spin_lock(&local_lu_gp_mem->lu_gp_mem_lock);
	lu_gp = local_lu_gp_mem->lu_gp;
	atomic_inc(&lu_gp->lu_gp_ref_cnt);
	smp_mb__after_atomic_inc();
	spin_unlock(&local_lu_gp_mem->lu_gp_mem_lock);
	/*
	 * For storage objects that are members of the 'default_lu_gp',
	 * we only do transition on the passed *l_tp_pt_gp, and not
	 * on all of the matching target port groups IDs in default_lu_gp.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(lu_gp->lu_gp_id)) {
=======
	if (!lu_gp->lu_gp_id) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!lu_gp->lu_gp_id) {
>>>>>>> refs/remotes/origin/master
		/*
		 * core_alua_do_transition_tg_pt() will always return
		 * success.
		 */
		core_alua_do_transition_tg_pt(l_tg_pt_gp, l_port, l_nacl,
<<<<<<< HEAD
					md_buf, new_state, explict);
=======
					md_buf, new_state, explicit);
>>>>>>> refs/remotes/origin/master
		atomic_dec(&lu_gp->lu_gp_ref_cnt);
		smp_mb__after_atomic_dec();
		kfree(md_buf);
		return 0;
	}
	/*
	 * For all other LU groups aside from 'default_lu_gp', walk all of
	 * the associated storage objects looking for a matching target port
	 * group ID from the local target port group.
	 */
	spin_lock(&lu_gp->lu_gp_lock);
	list_for_each_entry(lu_gp_mem, &lu_gp->lu_gp_mem_list,
				lu_gp_mem_list) {

		dev = lu_gp_mem->lu_gp_mem_dev;
<<<<<<< HEAD
		su_dev = dev->se_sub_dev;
=======
>>>>>>> refs/remotes/origin/master
		atomic_inc(&lu_gp_mem->lu_gp_mem_ref_cnt);
		smp_mb__after_atomic_inc();
		spin_unlock(&lu_gp->lu_gp_lock);

<<<<<<< HEAD
<<<<<<< HEAD
		spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
		list_for_each_entry(tg_pt_gp,
				&T10_ALUA(su_dev)->tg_pt_gps_list,
				tg_pt_gp_list) {

			if (!(tg_pt_gp->tg_pt_gp_valid_id))
=======
		spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
		list_for_each_entry(tg_pt_gp,
				&su_dev->t10_alua.tg_pt_gps_list,
				tg_pt_gp_list) {

			if (!tg_pt_gp->tg_pt_gp_valid_id)
>>>>>>> refs/remotes/origin/cm-10.0
				continue;
			/*
			 * If the target behavior port asymmetric access state
			 * is changed for any target port group accessiable via
=======
		spin_lock(&dev->t10_alua.tg_pt_gps_lock);
		list_for_each_entry(tg_pt_gp,
				&dev->t10_alua.tg_pt_gps_list,
				tg_pt_gp_list) {

			if (!tg_pt_gp->tg_pt_gp_valid_id)
				continue;
			/*
			 * If the target behavior port asymmetric access state
			 * is changed for any target port group accessible via
>>>>>>> refs/remotes/origin/master
			 * a logical unit within a LU group, the target port
			 * behavior group asymmetric access states for the same
			 * target port group accessible via other logical units
			 * in that LU group will also change.
			 */
			if (l_tg_pt_gp->tg_pt_gp_id != tg_pt_gp->tg_pt_gp_id)
				continue;

			if (l_tg_pt_gp == tg_pt_gp) {
				port = l_port;
				nacl = l_nacl;
			} else {
				port = NULL;
				nacl = NULL;
			}
			atomic_inc(&tg_pt_gp->tg_pt_gp_ref_cnt);
			smp_mb__after_atomic_inc();
<<<<<<< HEAD
<<<<<<< HEAD
			spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
			spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/master
			/*
			 * core_alua_do_transition_tg_pt() will always return
			 * success.
			 */
			core_alua_do_transition_tg_pt(tg_pt_gp, port,
<<<<<<< HEAD
					nacl, md_buf, new_state, explict);

<<<<<<< HEAD
			spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
			atomic_dec(&tg_pt_gp->tg_pt_gp_ref_cnt);
			smp_mb__after_atomic_dec();
		}
		spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
			spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
			atomic_dec(&tg_pt_gp->tg_pt_gp_ref_cnt);
			smp_mb__after_atomic_dec();
		}
		spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
					nacl, md_buf, new_state, explicit);

			spin_lock(&dev->t10_alua.tg_pt_gps_lock);
			atomic_dec(&tg_pt_gp->tg_pt_gp_ref_cnt);
			smp_mb__after_atomic_dec();
		}
		spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/master

		spin_lock(&lu_gp->lu_gp_lock);
		atomic_dec(&lu_gp_mem->lu_gp_mem_ref_cnt);
		smp_mb__after_atomic_dec();
	}
	spin_unlock(&lu_gp->lu_gp_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "Successfully processed LU Group: %s all ALUA TG PT"
=======
	pr_debug("Successfully processed LU Group: %s all ALUA TG PT"
>>>>>>> refs/remotes/origin/cm-10.0
		" Group IDs: %hu %s transition to primary state: %s\n",
		config_item_name(&lu_gp->lu_gp_group.cg_item),
		l_tg_pt_gp->tg_pt_gp_id, (explict) ? "explict" : "implict",
=======
	pr_debug("Successfully processed LU Group: %s all ALUA TG PT"
		" Group IDs: %hu %s transition to primary state: %s\n",
		config_item_name(&lu_gp->lu_gp_group.cg_item),
		l_tg_pt_gp->tg_pt_gp_id, (explicit) ? "explicit" : "implicit",
>>>>>>> refs/remotes/origin/master
		core_alua_dump_state(new_state));

	atomic_dec(&lu_gp->lu_gp_ref_cnt);
	smp_mb__after_atomic_dec();
	kfree(md_buf);
	return 0;
}

/*
 * Called with tg_pt_gp_mem->sep_tg_pt_md_mutex held
 */
static int core_alua_update_tpg_secondary_metadata(
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem,
	struct se_port *port,
	unsigned char *md_buf,
	u32 md_buf_len)
{
	struct se_portal_group *se_tpg = port->sep_tpg;
	char path[ALUA_METADATA_PATH_LEN], wwn[ALUA_SECONDARY_METADATA_WWN_LEN];
	int len;

	memset(path, 0, ALUA_METADATA_PATH_LEN);
	memset(wwn, 0, ALUA_SECONDARY_METADATA_WWN_LEN);

	len = snprintf(wwn, ALUA_SECONDARY_METADATA_WWN_LEN, "%s",
<<<<<<< HEAD
<<<<<<< HEAD
			TPG_TFO(se_tpg)->tpg_get_wwn(se_tpg));

	if (TPG_TFO(se_tpg)->tpg_get_tag != NULL)
		snprintf(wwn+len, ALUA_SECONDARY_METADATA_WWN_LEN-len, "+%hu",
				TPG_TFO(se_tpg)->tpg_get_tag(se_tpg));
=======
=======
>>>>>>> refs/remotes/origin/master
			se_tpg->se_tpg_tfo->tpg_get_wwn(se_tpg));

	if (se_tpg->se_tpg_tfo->tpg_get_tag != NULL)
		snprintf(wwn+len, ALUA_SECONDARY_METADATA_WWN_LEN-len, "+%hu",
				se_tpg->se_tpg_tfo->tpg_get_tag(se_tpg));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	len = snprintf(md_buf, md_buf_len, "alua_tg_pt_offline=%d\n"
			"alua_tg_pt_status=0x%02x\n",
			atomic_read(&port->sep_tg_pt_secondary_offline),
			port->sep_tg_pt_secondary_stat);

	snprintf(path, ALUA_METADATA_PATH_LEN, "/var/target/alua/%s/%s/lun_%u",
<<<<<<< HEAD
<<<<<<< HEAD
			TPG_TFO(se_tpg)->get_fabric_name(), wwn,
=======
			se_tpg->se_tpg_tfo->get_fabric_name(), wwn,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			se_tpg->se_tpg_tfo->get_fabric_name(), wwn,
>>>>>>> refs/remotes/origin/master
			port->sep_lun->unpacked_lun);

	return core_alua_write_tpg_metadata(path, md_buf, len);
}

static int core_alua_set_tg_pt_secondary_state(
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem,
	struct se_port *port,
<<<<<<< HEAD
	int explict,
=======
	int explicit,
>>>>>>> refs/remotes/origin/master
	int offline)
{
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	unsigned char *md_buf;
	u32 md_buf_len;
	int trans_delay_msecs;

	spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
	tg_pt_gp = tg_pt_gp_mem->tg_pt_gp;
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(tg_pt_gp)) {
		spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
		printk(KERN_ERR "Unable to complete secondary state"
				" transition\n");
		return -1;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!tg_pt_gp) {
		spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
		pr_err("Unable to complete secondary state"
				" transition\n");
		return -EINVAL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	trans_delay_msecs = tg_pt_gp->tg_pt_gp_trans_delay_msecs;
	/*
	 * Set the secondary ALUA target port access state to OFFLINE
	 * or release the previously secondary state for struct se_port
	 */
	if (offline)
		atomic_set(&port->sep_tg_pt_secondary_offline, 1);
	else
		atomic_set(&port->sep_tg_pt_secondary_offline, 0);

	md_buf_len = tg_pt_gp->tg_pt_gp_md_buf_len;
<<<<<<< HEAD
	port->sep_tg_pt_secondary_stat = (explict) ?
			ALUA_STATUS_ALTERED_BY_EXPLICT_STPG :
			ALUA_STATUS_ALTERED_BY_IMPLICT_ALUA;

<<<<<<< HEAD
	printk(KERN_INFO "Successful %s ALUA transition TG PT Group: %s ID: %hu"
=======
	pr_debug("Successful %s ALUA transition TG PT Group: %s ID: %hu"
>>>>>>> refs/remotes/origin/cm-10.0
		" to secondary access state: %s\n", (explict) ? "explict" :
		"implict", config_item_name(&tg_pt_gp->tg_pt_gp_group.cg_item),
=======
	port->sep_tg_pt_secondary_stat = (explicit) ?
			ALUA_STATUS_ALTERED_BY_EXPLICIT_STPG :
			ALUA_STATUS_ALTERED_BY_IMPLICIT_ALUA;

	pr_debug("Successful %s ALUA transition TG PT Group: %s ID: %hu"
		" to secondary access state: %s\n", (explicit) ? "explicit" :
		"implicit", config_item_name(&tg_pt_gp->tg_pt_gp_group.cg_item),
>>>>>>> refs/remotes/origin/master
		tg_pt_gp->tg_pt_gp_id, (offline) ? "OFFLINE" : "ONLINE");

	spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
	/*
	 * Do the optional transition delay after we set the secondary
	 * ALUA access state.
	 */
	if (trans_delay_msecs != 0)
		msleep_interruptible(trans_delay_msecs);
	/*
	 * See if we need to update the ALUA fabric port metadata for
	 * secondary state and status
	 */
	if (port->sep_tg_pt_secondary_write_md) {
		md_buf = kzalloc(md_buf_len, GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
		if (!(md_buf)) {
			printk(KERN_ERR "Unable to allocate md_buf for"
				" secondary ALUA access metadata\n");
			return -1;
=======
=======
>>>>>>> refs/remotes/origin/master
		if (!md_buf) {
			pr_err("Unable to allocate md_buf for"
				" secondary ALUA access metadata\n");
			return -ENOMEM;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
		mutex_lock(&port->sep_tg_pt_md_mutex);
		core_alua_update_tpg_secondary_metadata(tg_pt_gp_mem, port,
				md_buf, md_buf_len);
		mutex_unlock(&port->sep_tg_pt_md_mutex);

		kfree(md_buf);
	}

	return 0;
}

struct t10_alua_lu_gp *
core_alua_allocate_lu_gp(const char *name, int def_group)
{
	struct t10_alua_lu_gp *lu_gp;

	lu_gp = kmem_cache_zalloc(t10_alua_lu_gp_cache, GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(lu_gp)) {
		printk(KERN_ERR "Unable to allocate struct t10_alua_lu_gp\n");
		return ERR_PTR(-ENOMEM);
	}
	INIT_LIST_HEAD(&lu_gp->lu_gp_list);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!lu_gp) {
		pr_err("Unable to allocate struct t10_alua_lu_gp\n");
		return ERR_PTR(-ENOMEM);
	}
	INIT_LIST_HEAD(&lu_gp->lu_gp_node);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&lu_gp->lu_gp_mem_list);
	spin_lock_init(&lu_gp->lu_gp_lock);
	atomic_set(&lu_gp->lu_gp_ref_cnt, 0);

	if (def_group) {
<<<<<<< HEAD
<<<<<<< HEAD
		lu_gp->lu_gp_id = se_global->alua_lu_gps_counter++;
		lu_gp->lu_gp_valid_id = 1;
		se_global->alua_lu_gps_count++;
=======
		lu_gp->lu_gp_id = alua_lu_gps_counter++;
		lu_gp->lu_gp_valid_id = 1;
		alua_lu_gps_count++;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		lu_gp->lu_gp_id = alua_lu_gps_counter++;
		lu_gp->lu_gp_valid_id = 1;
		alua_lu_gps_count++;
>>>>>>> refs/remotes/origin/master
	}

	return lu_gp;
}

int core_alua_set_lu_gp_id(struct t10_alua_lu_gp *lu_gp, u16 lu_gp_id)
{
	struct t10_alua_lu_gp *lu_gp_tmp;
	u16 lu_gp_id_tmp;
	/*
	 * The lu_gp->lu_gp_id may only be set once..
	 */
	if (lu_gp->lu_gp_valid_id) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_WARNING "ALUA LU Group already has a valid ID,"
			" ignoring request\n");
		return -1;
	}

	spin_lock(&se_global->lu_gps_lock);
	if (se_global->alua_lu_gps_count == 0x0000ffff) {
		printk(KERN_ERR "Maximum ALUA se_global->alua_lu_gps_count:"
				" 0x0000ffff reached\n");
		spin_unlock(&se_global->lu_gps_lock);
		kmem_cache_free(t10_alua_lu_gp_cache, lu_gp);
		return -1;
	}
again:
	lu_gp_id_tmp = (lu_gp_id != 0) ? lu_gp_id :
				se_global->alua_lu_gps_counter++;

	list_for_each_entry(lu_gp_tmp, &se_global->g_lu_gps_list, lu_gp_list) {
		if (lu_gp_tmp->lu_gp_id == lu_gp_id_tmp) {
			if (!(lu_gp_id))
				goto again;

			printk(KERN_WARNING "ALUA Logical Unit Group ID: %hu"
				" already exists, ignoring request\n",
				lu_gp_id);
			spin_unlock(&se_global->lu_gps_lock);
			return -1;
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_warn("ALUA LU Group already has a valid ID,"
			" ignoring request\n");
		return -EINVAL;
	}

	spin_lock(&lu_gps_lock);
	if (alua_lu_gps_count == 0x0000ffff) {
		pr_err("Maximum ALUA alua_lu_gps_count:"
				" 0x0000ffff reached\n");
		spin_unlock(&lu_gps_lock);
		kmem_cache_free(t10_alua_lu_gp_cache, lu_gp);
		return -ENOSPC;
	}
again:
	lu_gp_id_tmp = (lu_gp_id != 0) ? lu_gp_id :
				alua_lu_gps_counter++;

	list_for_each_entry(lu_gp_tmp, &lu_gps_list, lu_gp_node) {
		if (lu_gp_tmp->lu_gp_id == lu_gp_id_tmp) {
			if (!lu_gp_id)
				goto again;

			pr_warn("ALUA Logical Unit Group ID: %hu"
				" already exists, ignoring request\n",
				lu_gp_id);
			spin_unlock(&lu_gps_lock);
			return -EINVAL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}

	lu_gp->lu_gp_id = lu_gp_id_tmp;
	lu_gp->lu_gp_valid_id = 1;
<<<<<<< HEAD
<<<<<<< HEAD
	list_add_tail(&lu_gp->lu_gp_list, &se_global->g_lu_gps_list);
	se_global->alua_lu_gps_count++;
	spin_unlock(&se_global->lu_gps_lock);
=======
	list_add_tail(&lu_gp->lu_gp_node, &lu_gps_list);
	alua_lu_gps_count++;
	spin_unlock(&lu_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	list_add_tail(&lu_gp->lu_gp_node, &lu_gps_list);
	alua_lu_gps_count++;
	spin_unlock(&lu_gps_lock);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static struct t10_alua_lu_gp_member *
core_alua_allocate_lu_gp_mem(struct se_device *dev)
{
	struct t10_alua_lu_gp_member *lu_gp_mem;

	lu_gp_mem = kmem_cache_zalloc(t10_alua_lu_gp_mem_cache, GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(lu_gp_mem)) {
		printk(KERN_ERR "Unable to allocate struct t10_alua_lu_gp_member\n");
=======
	if (!lu_gp_mem) {
		pr_err("Unable to allocate struct t10_alua_lu_gp_member\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!lu_gp_mem) {
		pr_err("Unable to allocate struct t10_alua_lu_gp_member\n");
>>>>>>> refs/remotes/origin/master
		return ERR_PTR(-ENOMEM);
	}
	INIT_LIST_HEAD(&lu_gp_mem->lu_gp_mem_list);
	spin_lock_init(&lu_gp_mem->lu_gp_mem_lock);
	atomic_set(&lu_gp_mem->lu_gp_mem_ref_cnt, 0);

	lu_gp_mem->lu_gp_mem_dev = dev;
	dev->dev_alua_lu_gp_mem = lu_gp_mem;

	return lu_gp_mem;
}

void core_alua_free_lu_gp(struct t10_alua_lu_gp *lu_gp)
{
	struct t10_alua_lu_gp_member *lu_gp_mem, *lu_gp_mem_tmp;
	/*
	 * Once we have reached this point, config_item_put() has
	 * already been called from target_core_alua_drop_lu_gp().
	 *
	 * Here, we remove the *lu_gp from the global list so that
	 * no associations can be made while we are releasing
	 * struct t10_alua_lu_gp.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&se_global->lu_gps_lock);
	atomic_set(&lu_gp->lu_gp_shutdown, 1);
	list_del(&lu_gp->lu_gp_list);
	se_global->alua_lu_gps_count--;
	spin_unlock(&se_global->lu_gps_lock);
=======
=======
>>>>>>> refs/remotes/origin/master
	spin_lock(&lu_gps_lock);
	list_del(&lu_gp->lu_gp_node);
	alua_lu_gps_count--;
	spin_unlock(&lu_gps_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Allow struct t10_alua_lu_gp * referenced by core_alua_get_lu_gp_by_name()
	 * in target_core_configfs.c:target_core_store_alua_lu_gp() to be
	 * released with core_alua_put_lu_gp_from_name()
	 */
	while (atomic_read(&lu_gp->lu_gp_ref_cnt))
		cpu_relax();
	/*
	 * Release reference to struct t10_alua_lu_gp * from all associated
	 * struct se_device.
	 */
	spin_lock(&lu_gp->lu_gp_lock);
	list_for_each_entry_safe(lu_gp_mem, lu_gp_mem_tmp,
				&lu_gp->lu_gp_mem_list, lu_gp_mem_list) {
		if (lu_gp_mem->lu_gp_assoc) {
			list_del(&lu_gp_mem->lu_gp_mem_list);
			lu_gp->lu_gp_members--;
			lu_gp_mem->lu_gp_assoc = 0;
		}
		spin_unlock(&lu_gp->lu_gp_lock);
		/*
		 *
		 * lu_gp_mem is associated with a single
		 * struct se_device->dev_alua_lu_gp_mem, and is released when
		 * struct se_device is released via core_alua_free_lu_gp_mem().
		 *
		 * If the passed lu_gp does NOT match the default_lu_gp, assume
<<<<<<< HEAD
		 * we want to re-assocate a given lu_gp_mem with default_lu_gp.
		 */
		spin_lock(&lu_gp_mem->lu_gp_mem_lock);
<<<<<<< HEAD
		if (lu_gp != se_global->default_lu_gp)
			__core_alua_attach_lu_gp_mem(lu_gp_mem,
					se_global->default_lu_gp);
=======
		if (lu_gp != default_lu_gp)
			__core_alua_attach_lu_gp_mem(lu_gp_mem,
					default_lu_gp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 * we want to re-associate a given lu_gp_mem with default_lu_gp.
		 */
		spin_lock(&lu_gp_mem->lu_gp_mem_lock);
		if (lu_gp != default_lu_gp)
			__core_alua_attach_lu_gp_mem(lu_gp_mem,
					default_lu_gp);
>>>>>>> refs/remotes/origin/master
		else
			lu_gp_mem->lu_gp = NULL;
		spin_unlock(&lu_gp_mem->lu_gp_mem_lock);

		spin_lock(&lu_gp->lu_gp_lock);
	}
	spin_unlock(&lu_gp->lu_gp_lock);

	kmem_cache_free(t10_alua_lu_gp_cache, lu_gp);
}

void core_alua_free_lu_gp_mem(struct se_device *dev)
{
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = dev->se_sub_dev;
<<<<<<< HEAD
	struct t10_alua *alua = T10_ALUA(su_dev);
=======
	struct t10_alua *alua = &su_dev->t10_alua;
>>>>>>> refs/remotes/origin/cm-10.0
	struct t10_alua_lu_gp *lu_gp;
	struct t10_alua_lu_gp_member *lu_gp_mem;

	if (alua->alua_type != SPC3_ALUA_EMULATED)
		return;

	lu_gp_mem = dev->dev_alua_lu_gp_mem;
<<<<<<< HEAD
	if (!(lu_gp_mem))
=======
	if (!lu_gp_mem)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct t10_alua_lu_gp *lu_gp;
	struct t10_alua_lu_gp_member *lu_gp_mem;

	lu_gp_mem = dev->dev_alua_lu_gp_mem;
	if (!lu_gp_mem)
>>>>>>> refs/remotes/origin/master
		return;

	while (atomic_read(&lu_gp_mem->lu_gp_mem_ref_cnt))
		cpu_relax();

	spin_lock(&lu_gp_mem->lu_gp_mem_lock);
	lu_gp = lu_gp_mem->lu_gp;
<<<<<<< HEAD
<<<<<<< HEAD
	if ((lu_gp)) {
=======
	if (lu_gp) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (lu_gp) {
>>>>>>> refs/remotes/origin/master
		spin_lock(&lu_gp->lu_gp_lock);
		if (lu_gp_mem->lu_gp_assoc) {
			list_del(&lu_gp_mem->lu_gp_mem_list);
			lu_gp->lu_gp_members--;
			lu_gp_mem->lu_gp_assoc = 0;
		}
		spin_unlock(&lu_gp->lu_gp_lock);
		lu_gp_mem->lu_gp = NULL;
	}
	spin_unlock(&lu_gp_mem->lu_gp_mem_lock);

	kmem_cache_free(t10_alua_lu_gp_mem_cache, lu_gp_mem);
}

struct t10_alua_lu_gp *core_alua_get_lu_gp_by_name(const char *name)
{
	struct t10_alua_lu_gp *lu_gp;
	struct config_item *ci;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&se_global->lu_gps_lock);
	list_for_each_entry(lu_gp, &se_global->g_lu_gps_list, lu_gp_list) {
		if (!(lu_gp->lu_gp_valid_id))
			continue;
		ci = &lu_gp->lu_gp_group.cg_item;
		if (!(strcmp(config_item_name(ci), name))) {
			atomic_inc(&lu_gp->lu_gp_ref_cnt);
			spin_unlock(&se_global->lu_gps_lock);
			return lu_gp;
		}
	}
	spin_unlock(&se_global->lu_gps_lock);
=======
=======
>>>>>>> refs/remotes/origin/master
	spin_lock(&lu_gps_lock);
	list_for_each_entry(lu_gp, &lu_gps_list, lu_gp_node) {
		if (!lu_gp->lu_gp_valid_id)
			continue;
		ci = &lu_gp->lu_gp_group.cg_item;
		if (!strcmp(config_item_name(ci), name)) {
			atomic_inc(&lu_gp->lu_gp_ref_cnt);
			spin_unlock(&lu_gps_lock);
			return lu_gp;
		}
	}
	spin_unlock(&lu_gps_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return NULL;
}

void core_alua_put_lu_gp_from_name(struct t10_alua_lu_gp *lu_gp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&se_global->lu_gps_lock);
	atomic_dec(&lu_gp->lu_gp_ref_cnt);
	spin_unlock(&se_global->lu_gps_lock);
=======
	spin_lock(&lu_gps_lock);
	atomic_dec(&lu_gp->lu_gp_ref_cnt);
	spin_unlock(&lu_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_lock(&lu_gps_lock);
	atomic_dec(&lu_gp->lu_gp_ref_cnt);
	spin_unlock(&lu_gps_lock);
>>>>>>> refs/remotes/origin/master
}

/*
 * Called with struct t10_alua_lu_gp_member->lu_gp_mem_lock
 */
void __core_alua_attach_lu_gp_mem(
	struct t10_alua_lu_gp_member *lu_gp_mem,
	struct t10_alua_lu_gp *lu_gp)
{
	spin_lock(&lu_gp->lu_gp_lock);
	lu_gp_mem->lu_gp = lu_gp;
	lu_gp_mem->lu_gp_assoc = 1;
	list_add_tail(&lu_gp_mem->lu_gp_mem_list, &lu_gp->lu_gp_mem_list);
	lu_gp->lu_gp_members++;
	spin_unlock(&lu_gp->lu_gp_lock);
}

/*
 * Called with struct t10_alua_lu_gp_member->lu_gp_mem_lock
 */
void __core_alua_drop_lu_gp_mem(
	struct t10_alua_lu_gp_member *lu_gp_mem,
	struct t10_alua_lu_gp *lu_gp)
{
	spin_lock(&lu_gp->lu_gp_lock);
	list_del(&lu_gp_mem->lu_gp_mem_list);
	lu_gp_mem->lu_gp = NULL;
	lu_gp_mem->lu_gp_assoc = 0;
	lu_gp->lu_gp_members--;
	spin_unlock(&lu_gp->lu_gp_lock);
}

<<<<<<< HEAD
struct t10_alua_tg_pt_gp *core_alua_allocate_tg_pt_gp(
	struct se_subsystem_dev *su_dev,
	const char *name,
	int def_group)
=======
struct t10_alua_tg_pt_gp *core_alua_allocate_tg_pt_gp(struct se_device *dev,
		const char *name, int def_group)
>>>>>>> refs/remotes/origin/master
{
	struct t10_alua_tg_pt_gp *tg_pt_gp;

	tg_pt_gp = kmem_cache_zalloc(t10_alua_tg_pt_gp_cache, GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(tg_pt_gp)) {
		printk(KERN_ERR "Unable to allocate struct t10_alua_tg_pt_gp\n");
=======
	if (!tg_pt_gp) {
		pr_err("Unable to allocate struct t10_alua_tg_pt_gp\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!tg_pt_gp) {
		pr_err("Unable to allocate struct t10_alua_tg_pt_gp\n");
>>>>>>> refs/remotes/origin/master
		return NULL;
	}
	INIT_LIST_HEAD(&tg_pt_gp->tg_pt_gp_list);
	INIT_LIST_HEAD(&tg_pt_gp->tg_pt_gp_mem_list);
	mutex_init(&tg_pt_gp->tg_pt_gp_md_mutex);
	spin_lock_init(&tg_pt_gp->tg_pt_gp_lock);
	atomic_set(&tg_pt_gp->tg_pt_gp_ref_cnt, 0);
<<<<<<< HEAD
	tg_pt_gp->tg_pt_gp_su_dev = su_dev;
	tg_pt_gp->tg_pt_gp_md_buf_len = ALUA_MD_BUF_LEN;
	atomic_set(&tg_pt_gp->tg_pt_gp_alua_access_state,
		ALUA_ACCESS_STATE_ACTIVE_OPTMIZED);
	/*
	 * Enable both explict and implict ALUA support by default
	 */
	tg_pt_gp->tg_pt_gp_alua_access_type =
			TPGS_EXPLICT_ALUA | TPGS_IMPLICT_ALUA;
=======
	tg_pt_gp->tg_pt_gp_dev = dev;
	tg_pt_gp->tg_pt_gp_md_buf_len = ALUA_MD_BUF_LEN;
	atomic_set(&tg_pt_gp->tg_pt_gp_alua_access_state,
		ALUA_ACCESS_STATE_ACTIVE_OPTIMIZED);
	/*
	 * Enable both explicit and implicit ALUA support by default
	 */
	tg_pt_gp->tg_pt_gp_alua_access_type =
			TPGS_EXPLICIT_ALUA | TPGS_IMPLICIT_ALUA;
>>>>>>> refs/remotes/origin/master
	/*
	 * Set the default Active/NonOptimized Delay in milliseconds
	 */
	tg_pt_gp->tg_pt_gp_nonop_delay_msecs = ALUA_DEFAULT_NONOP_DELAY_MSECS;
	tg_pt_gp->tg_pt_gp_trans_delay_msecs = ALUA_DEFAULT_TRANS_DELAY_MSECS;
<<<<<<< HEAD

	if (def_group) {
<<<<<<< HEAD
		spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
		tg_pt_gp->tg_pt_gp_id =
				T10_ALUA(su_dev)->alua_tg_pt_gps_counter++;
		tg_pt_gp->tg_pt_gp_valid_id = 1;
		T10_ALUA(su_dev)->alua_tg_pt_gps_count++;
		list_add_tail(&tg_pt_gp->tg_pt_gp_list,
			      &T10_ALUA(su_dev)->tg_pt_gps_list);
		spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
		spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
		tg_pt_gp->tg_pt_gp_id =
				su_dev->t10_alua.alua_tg_pt_gps_counter++;
		tg_pt_gp->tg_pt_gp_valid_id = 1;
		su_dev->t10_alua.alua_tg_pt_gps_count++;
		list_add_tail(&tg_pt_gp->tg_pt_gp_list,
			      &su_dev->t10_alua.tg_pt_gps_list);
		spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	tg_pt_gp->tg_pt_gp_implicit_trans_secs = ALUA_DEFAULT_IMPLICIT_TRANS_SECS;

	/*
	 * Enable all supported states
	 */
	tg_pt_gp->tg_pt_gp_alua_supported_states =
	    ALUA_T_SUP | ALUA_O_SUP |
	    ALUA_U_SUP | ALUA_S_SUP | ALUA_AN_SUP | ALUA_AO_SUP;

	if (def_group) {
		spin_lock(&dev->t10_alua.tg_pt_gps_lock);
		tg_pt_gp->tg_pt_gp_id =
				dev->t10_alua.alua_tg_pt_gps_counter++;
		tg_pt_gp->tg_pt_gp_valid_id = 1;
		dev->t10_alua.alua_tg_pt_gps_count++;
		list_add_tail(&tg_pt_gp->tg_pt_gp_list,
			      &dev->t10_alua.tg_pt_gps_list);
		spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/master
	}

	return tg_pt_gp;
}

int core_alua_set_tg_pt_gp_id(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	u16 tg_pt_gp_id)
{
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = tg_pt_gp->tg_pt_gp_su_dev;
	struct t10_alua_tg_pt_gp *tg_pt_gp_tmp;
	u16 tg_pt_gp_id_tmp;
=======
	struct se_device *dev = tg_pt_gp->tg_pt_gp_dev;
	struct t10_alua_tg_pt_gp *tg_pt_gp_tmp;
	u16 tg_pt_gp_id_tmp;

>>>>>>> refs/remotes/origin/master
	/*
	 * The tg_pt_gp->tg_pt_gp_id may only be set once..
	 */
	if (tg_pt_gp->tg_pt_gp_valid_id) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_WARNING "ALUA TG PT Group already has a valid ID,"
			" ignoring request\n");
		return -1;
	}

	spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
	if (T10_ALUA(su_dev)->alua_tg_pt_gps_count == 0x0000ffff) {
		printk(KERN_ERR "Maximum ALUA alua_tg_pt_gps_count:"
			" 0x0000ffff reached\n");
		spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
		kmem_cache_free(t10_alua_tg_pt_gp_cache, tg_pt_gp);
		return -1;
	}
again:
	tg_pt_gp_id_tmp = (tg_pt_gp_id != 0) ? tg_pt_gp_id :
			T10_ALUA(su_dev)->alua_tg_pt_gps_counter++;

	list_for_each_entry(tg_pt_gp_tmp, &T10_ALUA(su_dev)->tg_pt_gps_list,
			tg_pt_gp_list) {
		if (tg_pt_gp_tmp->tg_pt_gp_id == tg_pt_gp_id_tmp) {
			if (!(tg_pt_gp_id))
				goto again;

			printk(KERN_ERR "ALUA Target Port Group ID: %hu already"
				" exists, ignoring request\n", tg_pt_gp_id);
			spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
			return -1;
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_warn("ALUA TG PT Group already has a valid ID,"
			" ignoring request\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
	if (su_dev->t10_alua.alua_tg_pt_gps_count == 0x0000ffff) {
		pr_err("Maximum ALUA alua_tg_pt_gps_count:"
			" 0x0000ffff reached\n");
		spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
=======
	spin_lock(&dev->t10_alua.tg_pt_gps_lock);
	if (dev->t10_alua.alua_tg_pt_gps_count == 0x0000ffff) {
		pr_err("Maximum ALUA alua_tg_pt_gps_count:"
			" 0x0000ffff reached\n");
		spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/master
		kmem_cache_free(t10_alua_tg_pt_gp_cache, tg_pt_gp);
		return -ENOSPC;
	}
again:
	tg_pt_gp_id_tmp = (tg_pt_gp_id != 0) ? tg_pt_gp_id :
<<<<<<< HEAD
			su_dev->t10_alua.alua_tg_pt_gps_counter++;

	list_for_each_entry(tg_pt_gp_tmp, &su_dev->t10_alua.tg_pt_gps_list,
=======
			dev->t10_alua.alua_tg_pt_gps_counter++;

	list_for_each_entry(tg_pt_gp_tmp, &dev->t10_alua.tg_pt_gps_list,
>>>>>>> refs/remotes/origin/master
			tg_pt_gp_list) {
		if (tg_pt_gp_tmp->tg_pt_gp_id == tg_pt_gp_id_tmp) {
			if (!tg_pt_gp_id)
				goto again;

			pr_err("ALUA Target Port Group ID: %hu already"
				" exists, ignoring request\n", tg_pt_gp_id);
<<<<<<< HEAD
			spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
			return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
			return -EINVAL;
>>>>>>> refs/remotes/origin/master
		}
	}

	tg_pt_gp->tg_pt_gp_id = tg_pt_gp_id_tmp;
	tg_pt_gp->tg_pt_gp_valid_id = 1;
	list_add_tail(&tg_pt_gp->tg_pt_gp_list,
<<<<<<< HEAD
<<<<<<< HEAD
			&T10_ALUA(su_dev)->tg_pt_gps_list);
	T10_ALUA(su_dev)->alua_tg_pt_gps_count++;
	spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
			&su_dev->t10_alua.tg_pt_gps_list);
	su_dev->t10_alua.alua_tg_pt_gps_count++;
	spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			&dev->t10_alua.tg_pt_gps_list);
	dev->t10_alua.alua_tg_pt_gps_count++;
	spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/master

	return 0;
}

struct t10_alua_tg_pt_gp_member *core_alua_allocate_tg_pt_gp_mem(
	struct se_port *port)
{
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem;

	tg_pt_gp_mem = kmem_cache_zalloc(t10_alua_tg_pt_gp_mem_cache,
				GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(tg_pt_gp_mem)) {
		printk(KERN_ERR "Unable to allocate struct t10_alua_tg_pt_gp_member\n");
=======
	if (!tg_pt_gp_mem) {
		pr_err("Unable to allocate struct t10_alua_tg_pt_gp_member\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!tg_pt_gp_mem) {
		pr_err("Unable to allocate struct t10_alua_tg_pt_gp_member\n");
>>>>>>> refs/remotes/origin/master
		return ERR_PTR(-ENOMEM);
	}
	INIT_LIST_HEAD(&tg_pt_gp_mem->tg_pt_gp_mem_list);
	spin_lock_init(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
	atomic_set(&tg_pt_gp_mem->tg_pt_gp_mem_ref_cnt, 0);

	tg_pt_gp_mem->tg_pt = port;
	port->sep_alua_tg_pt_gp_mem = tg_pt_gp_mem;
<<<<<<< HEAD
<<<<<<< HEAD
	atomic_set(&port->sep_tg_pt_gp_active, 1);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return tg_pt_gp_mem;
}

void core_alua_free_tg_pt_gp(
	struct t10_alua_tg_pt_gp *tg_pt_gp)
{
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = tg_pt_gp->tg_pt_gp_su_dev;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem, *tg_pt_gp_mem_tmp;
=======
	struct se_device *dev = tg_pt_gp->tg_pt_gp_dev;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem, *tg_pt_gp_mem_tmp;

>>>>>>> refs/remotes/origin/master
	/*
	 * Once we have reached this point, config_item_put() has already
	 * been called from target_core_alua_drop_tg_pt_gp().
	 *
	 * Here we remove *tg_pt_gp from the global list so that
<<<<<<< HEAD
	 * no assications *OR* explict ALUA via SET_TARGET_PORT_GROUPS
	 * can be made while we are releasing struct t10_alua_tg_pt_gp.
	 */
<<<<<<< HEAD
	spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
	list_del(&tg_pt_gp->tg_pt_gp_list);
	T10_ALUA(su_dev)->alua_tg_pt_gps_counter--;
	spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
	spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
	list_del(&tg_pt_gp->tg_pt_gp_list);
	su_dev->t10_alua.alua_tg_pt_gps_counter--;
	spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * no associations *OR* explicit ALUA via SET_TARGET_PORT_GROUPS
	 * can be made while we are releasing struct t10_alua_tg_pt_gp.
	 */
	spin_lock(&dev->t10_alua.tg_pt_gps_lock);
	list_del(&tg_pt_gp->tg_pt_gp_list);
	dev->t10_alua.alua_tg_pt_gps_counter--;
	spin_unlock(&dev->t10_alua.tg_pt_gps_lock);

>>>>>>> refs/remotes/origin/master
	/*
	 * Allow a struct t10_alua_tg_pt_gp_member * referenced by
	 * core_alua_get_tg_pt_gp_by_name() in
	 * target_core_configfs.c:target_core_store_alua_tg_pt_gp()
	 * to be released with core_alua_put_tg_pt_gp_from_name().
	 */
	while (atomic_read(&tg_pt_gp->tg_pt_gp_ref_cnt))
		cpu_relax();
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	/*
	 * Release reference to struct t10_alua_tg_pt_gp from all associated
	 * struct se_port.
	 */
	spin_lock(&tg_pt_gp->tg_pt_gp_lock);
	list_for_each_entry_safe(tg_pt_gp_mem, tg_pt_gp_mem_tmp,
			&tg_pt_gp->tg_pt_gp_mem_list, tg_pt_gp_mem_list) {
		if (tg_pt_gp_mem->tg_pt_gp_assoc) {
			list_del(&tg_pt_gp_mem->tg_pt_gp_mem_list);
			tg_pt_gp->tg_pt_gp_members--;
			tg_pt_gp_mem->tg_pt_gp_assoc = 0;
		}
		spin_unlock(&tg_pt_gp->tg_pt_gp_lock);
		/*
		 * tg_pt_gp_mem is associated with a single
		 * se_port->sep_alua_tg_pt_gp_mem, and is released via
		 * core_alua_free_tg_pt_gp_mem().
		 *
		 * If the passed tg_pt_gp does NOT match the default_tg_pt_gp,
<<<<<<< HEAD
		 * assume we want to re-assocate a given tg_pt_gp_mem with
		 * default_tg_pt_gp.
		 */
		spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
<<<<<<< HEAD
		if (tg_pt_gp != T10_ALUA(su_dev)->default_tg_pt_gp) {
			__core_alua_attach_tg_pt_gp_mem(tg_pt_gp_mem,
					T10_ALUA(su_dev)->default_tg_pt_gp);
=======
		if (tg_pt_gp != su_dev->t10_alua.default_tg_pt_gp) {
			__core_alua_attach_tg_pt_gp_mem(tg_pt_gp_mem,
					su_dev->t10_alua.default_tg_pt_gp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 * assume we want to re-associate a given tg_pt_gp_mem with
		 * default_tg_pt_gp.
		 */
		spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
		if (tg_pt_gp != dev->t10_alua.default_tg_pt_gp) {
			__core_alua_attach_tg_pt_gp_mem(tg_pt_gp_mem,
					dev->t10_alua.default_tg_pt_gp);
>>>>>>> refs/remotes/origin/master
		} else
			tg_pt_gp_mem->tg_pt_gp = NULL;
		spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);

		spin_lock(&tg_pt_gp->tg_pt_gp_lock);
	}
	spin_unlock(&tg_pt_gp->tg_pt_gp_lock);

	kmem_cache_free(t10_alua_tg_pt_gp_cache, tg_pt_gp);
}

void core_alua_free_tg_pt_gp_mem(struct se_port *port)
{
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = port->sep_lun->lun_se_dev->se_sub_dev;
<<<<<<< HEAD
	struct t10_alua *alua = T10_ALUA(su_dev);
=======
	struct t10_alua *alua = &su_dev->t10_alua;
>>>>>>> refs/remotes/origin/cm-10.0
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem;

	if (alua->alua_type != SPC3_ALUA_EMULATED)
		return;

	tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
<<<<<<< HEAD
	if (!(tg_pt_gp_mem))
=======
	if (!tg_pt_gp_mem)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem;

	tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
	if (!tg_pt_gp_mem)
>>>>>>> refs/remotes/origin/master
		return;

	while (atomic_read(&tg_pt_gp_mem->tg_pt_gp_mem_ref_cnt))
		cpu_relax();

	spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
	tg_pt_gp = tg_pt_gp_mem->tg_pt_gp;
<<<<<<< HEAD
<<<<<<< HEAD
	if ((tg_pt_gp)) {
=======
	if (tg_pt_gp) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (tg_pt_gp) {
>>>>>>> refs/remotes/origin/master
		spin_lock(&tg_pt_gp->tg_pt_gp_lock);
		if (tg_pt_gp_mem->tg_pt_gp_assoc) {
			list_del(&tg_pt_gp_mem->tg_pt_gp_mem_list);
			tg_pt_gp->tg_pt_gp_members--;
			tg_pt_gp_mem->tg_pt_gp_assoc = 0;
		}
		spin_unlock(&tg_pt_gp->tg_pt_gp_lock);
		tg_pt_gp_mem->tg_pt_gp = NULL;
	}
	spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);

	kmem_cache_free(t10_alua_tg_pt_gp_mem_cache, tg_pt_gp_mem);
}

static struct t10_alua_tg_pt_gp *core_alua_get_tg_pt_gp_by_name(
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev,
	const char *name)
=======
		struct se_device *dev, const char *name)
>>>>>>> refs/remotes/origin/master
{
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	struct config_item *ci;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
	list_for_each_entry(tg_pt_gp, &T10_ALUA(su_dev)->tg_pt_gps_list,
			tg_pt_gp_list) {
		if (!(tg_pt_gp->tg_pt_gp_valid_id))
			continue;
		ci = &tg_pt_gp->tg_pt_gp_group.cg_item;
		if (!(strcmp(config_item_name(ci), name))) {
			atomic_inc(&tg_pt_gp->tg_pt_gp_ref_cnt);
			spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
			return tg_pt_gp;
		}
	}
	spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
	spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
	list_for_each_entry(tg_pt_gp, &su_dev->t10_alua.tg_pt_gps_list,
=======
	spin_lock(&dev->t10_alua.tg_pt_gps_lock);
	list_for_each_entry(tg_pt_gp, &dev->t10_alua.tg_pt_gps_list,
>>>>>>> refs/remotes/origin/master
			tg_pt_gp_list) {
		if (!tg_pt_gp->tg_pt_gp_valid_id)
			continue;
		ci = &tg_pt_gp->tg_pt_gp_group.cg_item;
		if (!strcmp(config_item_name(ci), name)) {
			atomic_inc(&tg_pt_gp->tg_pt_gp_ref_cnt);
<<<<<<< HEAD
			spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
			return tg_pt_gp;
		}
	}
	spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
			return tg_pt_gp;
		}
	}
	spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/master

	return NULL;
}

static void core_alua_put_tg_pt_gp_from_name(
	struct t10_alua_tg_pt_gp *tg_pt_gp)
{
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = tg_pt_gp->tg_pt_gp_su_dev;

<<<<<<< HEAD
	spin_lock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
	atomic_dec(&tg_pt_gp->tg_pt_gp_ref_cnt);
	spin_unlock(&T10_ALUA(su_dev)->tg_pt_gps_lock);
=======
	spin_lock(&su_dev->t10_alua.tg_pt_gps_lock);
	atomic_dec(&tg_pt_gp->tg_pt_gp_ref_cnt);
	spin_unlock(&su_dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct se_device *dev = tg_pt_gp->tg_pt_gp_dev;

	spin_lock(&dev->t10_alua.tg_pt_gps_lock);
	atomic_dec(&tg_pt_gp->tg_pt_gp_ref_cnt);
	spin_unlock(&dev->t10_alua.tg_pt_gps_lock);
>>>>>>> refs/remotes/origin/master
}

/*
 * Called with struct t10_alua_tg_pt_gp_member->tg_pt_gp_mem_lock held
 */
void __core_alua_attach_tg_pt_gp_mem(
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem,
	struct t10_alua_tg_pt_gp *tg_pt_gp)
{
	spin_lock(&tg_pt_gp->tg_pt_gp_lock);
	tg_pt_gp_mem->tg_pt_gp = tg_pt_gp;
	tg_pt_gp_mem->tg_pt_gp_assoc = 1;
	list_add_tail(&tg_pt_gp_mem->tg_pt_gp_mem_list,
			&tg_pt_gp->tg_pt_gp_mem_list);
	tg_pt_gp->tg_pt_gp_members++;
	spin_unlock(&tg_pt_gp->tg_pt_gp_lock);
}

/*
 * Called with struct t10_alua_tg_pt_gp_member->tg_pt_gp_mem_lock held
 */
static void __core_alua_drop_tg_pt_gp_mem(
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem,
	struct t10_alua_tg_pt_gp *tg_pt_gp)
{
	spin_lock(&tg_pt_gp->tg_pt_gp_lock);
	list_del(&tg_pt_gp_mem->tg_pt_gp_mem_list);
	tg_pt_gp_mem->tg_pt_gp = NULL;
	tg_pt_gp_mem->tg_pt_gp_assoc = 0;
	tg_pt_gp->tg_pt_gp_members--;
	spin_unlock(&tg_pt_gp->tg_pt_gp_lock);
}

ssize_t core_alua_show_tg_pt_gp_info(struct se_port *port, char *page)
{
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = port->sep_lun->lun_se_dev->se_sub_dev;
	struct config_item *tg_pt_ci;
<<<<<<< HEAD
	struct t10_alua *alua = T10_ALUA(su_dev);
=======
	struct t10_alua *alua = &su_dev->t10_alua;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct config_item *tg_pt_ci;
>>>>>>> refs/remotes/origin/master
	struct t10_alua_tg_pt_gp *tg_pt_gp;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem;
	ssize_t len = 0;

<<<<<<< HEAD
	if (alua->alua_type != SPC3_ALUA_EMULATED)
		return len;

	tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
<<<<<<< HEAD
	if (!(tg_pt_gp_mem))
=======
	if (!tg_pt_gp_mem)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
	if (!tg_pt_gp_mem)
>>>>>>> refs/remotes/origin/master
		return len;

	spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
	tg_pt_gp = tg_pt_gp_mem->tg_pt_gp;
<<<<<<< HEAD
<<<<<<< HEAD
	if ((tg_pt_gp)) {
=======
	if (tg_pt_gp) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (tg_pt_gp) {
>>>>>>> refs/remotes/origin/master
		tg_pt_ci = &tg_pt_gp->tg_pt_gp_group.cg_item;
		len += sprintf(page, "TG Port Alias: %s\nTG Port Group ID:"
			" %hu\nTG Port Primary Access State: %s\nTG Port "
			"Primary Access Status: %s\nTG Port Secondary Access"
			" State: %s\nTG Port Secondary Access Status: %s\n",
			config_item_name(tg_pt_ci), tg_pt_gp->tg_pt_gp_id,
			core_alua_dump_state(atomic_read(
					&tg_pt_gp->tg_pt_gp_alua_access_state)),
			core_alua_dump_status(
				tg_pt_gp->tg_pt_gp_alua_access_status),
			(atomic_read(&port->sep_tg_pt_secondary_offline)) ?
			"Offline" : "None",
			core_alua_dump_status(port->sep_tg_pt_secondary_stat));
	}
	spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);

	return len;
}

ssize_t core_alua_store_tg_pt_gp_info(
	struct se_port *port,
	const char *page,
	size_t count)
{
	struct se_portal_group *tpg;
	struct se_lun *lun;
<<<<<<< HEAD
	struct se_subsystem_dev *su_dev = port->sep_lun->lun_se_dev->se_sub_dev;
=======
	struct se_device *dev = port->sep_lun->lun_se_dev;
>>>>>>> refs/remotes/origin/master
	struct t10_alua_tg_pt_gp *tg_pt_gp = NULL, *tg_pt_gp_new = NULL;
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem;
	unsigned char buf[TG_PT_GROUP_NAME_BUF];
	int move = 0;

	tpg = port->sep_tpg;
	lun = port->sep_lun;

<<<<<<< HEAD
<<<<<<< HEAD
	if (T10_ALUA(su_dev)->alua_type != SPC3_ALUA_EMULATED) {
		printk(KERN_WARNING "SPC3_ALUA_EMULATED not enabled for"
			" %s/tpgt_%hu/%s\n", TPG_TFO(tpg)->tpg_get_wwn(tpg),
			TPG_TFO(tpg)->tpg_get_tag(tpg),
=======
	if (su_dev->t10_alua.alua_type != SPC3_ALUA_EMULATED) {
		pr_warn("SPC3_ALUA_EMULATED not enabled for"
			" %s/tpgt_%hu/%s\n", tpg->se_tpg_tfo->tpg_get_wwn(tpg),
			tpg->se_tpg_tfo->tpg_get_tag(tpg),
>>>>>>> refs/remotes/origin/cm-10.0
			config_item_name(&lun->lun_group.cg_item));
		return -EINVAL;
	}

	if (count > TG_PT_GROUP_NAME_BUF) {
<<<<<<< HEAD
		printk(KERN_ERR "ALUA Target Port Group alias too large!\n");
=======
		pr_err("ALUA Target Port Group alias too large!\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
	if (!tg_pt_gp_mem)
		return 0;

	if (count > TG_PT_GROUP_NAME_BUF) {
		pr_err("ALUA Target Port Group alias too large!\n");
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}
	memset(buf, 0, TG_PT_GROUP_NAME_BUF);
	memcpy(buf, page, count);
	/*
	 * Any ALUA target port group alias besides "NULL" means we will be
	 * making a new group association.
	 */
	if (strcmp(strstrip(buf), "NULL")) {
		/*
		 * core_alua_get_tg_pt_gp_by_name() will increment reference to
		 * struct t10_alua_tg_pt_gp.  This reference is released with
		 * core_alua_put_tg_pt_gp_from_name() below.
		 */
<<<<<<< HEAD
		tg_pt_gp_new = core_alua_get_tg_pt_gp_by_name(su_dev,
					strstrip(buf));
<<<<<<< HEAD
		if (!(tg_pt_gp_new))
			return -ENODEV;
	}
	tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
	if (!(tg_pt_gp_mem)) {
		if (tg_pt_gp_new)
			core_alua_put_tg_pt_gp_from_name(tg_pt_gp_new);
		printk(KERN_ERR "NULL struct se_port->sep_alua_tg_pt_gp_mem pointer\n");
=======
		if (!tg_pt_gp_new)
			return -ENODEV;
	}
	tg_pt_gp_mem = port->sep_alua_tg_pt_gp_mem;
	if (!tg_pt_gp_mem) {
		if (tg_pt_gp_new)
			core_alua_put_tg_pt_gp_from_name(tg_pt_gp_new);
		pr_err("NULL struct se_port->sep_alua_tg_pt_gp_mem pointer\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}

	spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
	tg_pt_gp = tg_pt_gp_mem->tg_pt_gp;
<<<<<<< HEAD
	if ((tg_pt_gp)) {
=======
	if (tg_pt_gp) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		tg_pt_gp_new = core_alua_get_tg_pt_gp_by_name(dev,
					strstrip(buf));
		if (!tg_pt_gp_new)
			return -ENODEV;
	}

	spin_lock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
	tg_pt_gp = tg_pt_gp_mem->tg_pt_gp;
	if (tg_pt_gp) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Clearing an existing tg_pt_gp association, and replacing
		 * with the default_tg_pt_gp.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (!(tg_pt_gp_new)) {
			printk(KERN_INFO "Target_Core_ConfigFS: Moving"
				" %s/tpgt_%hu/%s from ALUA Target Port Group:"
				" alua/%s, ID: %hu back to"
				" default_tg_pt_gp\n",
				TPG_TFO(tpg)->tpg_get_wwn(tpg),
				TPG_TFO(tpg)->tpg_get_tag(tpg),
=======
=======
>>>>>>> refs/remotes/origin/master
		if (!tg_pt_gp_new) {
			pr_debug("Target_Core_ConfigFS: Moving"
				" %s/tpgt_%hu/%s from ALUA Target Port Group:"
				" alua/%s, ID: %hu back to"
				" default_tg_pt_gp\n",
				tpg->se_tpg_tfo->tpg_get_wwn(tpg),
				tpg->se_tpg_tfo->tpg_get_tag(tpg),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				config_item_name(&lun->lun_group.cg_item),
				config_item_name(
					&tg_pt_gp->tg_pt_gp_group.cg_item),
				tg_pt_gp->tg_pt_gp_id);

			__core_alua_drop_tg_pt_gp_mem(tg_pt_gp_mem, tg_pt_gp);
			__core_alua_attach_tg_pt_gp_mem(tg_pt_gp_mem,
<<<<<<< HEAD
<<<<<<< HEAD
					T10_ALUA(su_dev)->default_tg_pt_gp);
=======
					su_dev->t10_alua.default_tg_pt_gp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
					dev->t10_alua.default_tg_pt_gp);
>>>>>>> refs/remotes/origin/master
			spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);

			return count;
		}
		/*
		 * Removing existing association of tg_pt_gp_mem with tg_pt_gp
		 */
		__core_alua_drop_tg_pt_gp_mem(tg_pt_gp_mem, tg_pt_gp);
		move = 1;
	}
	/*
	 * Associate tg_pt_gp_mem with tg_pt_gp_new.
	 */
	__core_alua_attach_tg_pt_gp_mem(tg_pt_gp_mem, tg_pt_gp_new);
	spin_unlock(&tg_pt_gp_mem->tg_pt_gp_mem_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "Target_Core_ConfigFS: %s %s/tpgt_%hu/%s to ALUA"
		" Target Port Group: alua/%s, ID: %hu\n", (move) ?
		"Moving" : "Adding", TPG_TFO(tpg)->tpg_get_wwn(tpg),
		TPG_TFO(tpg)->tpg_get_tag(tpg),
=======
=======
>>>>>>> refs/remotes/origin/master
	pr_debug("Target_Core_ConfigFS: %s %s/tpgt_%hu/%s to ALUA"
		" Target Port Group: alua/%s, ID: %hu\n", (move) ?
		"Moving" : "Adding", tpg->se_tpg_tfo->tpg_get_wwn(tpg),
		tpg->se_tpg_tfo->tpg_get_tag(tpg),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		config_item_name(&lun->lun_group.cg_item),
		config_item_name(&tg_pt_gp_new->tg_pt_gp_group.cg_item),
		tg_pt_gp_new->tg_pt_gp_id);

	core_alua_put_tg_pt_gp_from_name(tg_pt_gp_new);
	return count;
}

ssize_t core_alua_show_access_type(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	char *page)
{
<<<<<<< HEAD
	if ((tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_EXPLICT_ALUA) &&
	    (tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_IMPLICT_ALUA))
		return sprintf(page, "Implict and Explict\n");
	else if (tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_IMPLICT_ALUA)
		return sprintf(page, "Implict\n");
	else if (tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_EXPLICT_ALUA)
		return sprintf(page, "Explict\n");
=======
	if ((tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_EXPLICIT_ALUA) &&
	    (tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_IMPLICIT_ALUA))
		return sprintf(page, "Implicit and Explicit\n");
	else if (tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_IMPLICIT_ALUA)
		return sprintf(page, "Implicit\n");
	else if (tg_pt_gp->tg_pt_gp_alua_access_type & TPGS_EXPLICIT_ALUA)
		return sprintf(page, "Explicit\n");
>>>>>>> refs/remotes/origin/master
	else
		return sprintf(page, "None\n");
}

ssize_t core_alua_store_access_type(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	const char *page,
	size_t count)
{
	unsigned long tmp;
	int ret;

<<<<<<< HEAD
	ret = strict_strtoul(page, 0, &tmp);
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to extract alua_access_type\n");
		return -EINVAL;
	}
	if ((tmp != 0) && (tmp != 1) && (tmp != 2) && (tmp != 3)) {
		printk(KERN_ERR "Illegal value for alua_access_type:"
=======
		pr_err("Unable to extract alua_access_type\n");
		return -EINVAL;
	}
	if ((tmp != 0) && (tmp != 1) && (tmp != 2) && (tmp != 3)) {
		pr_err("Illegal value for alua_access_type:"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = kstrtoul(page, 0, &tmp);
	if (ret < 0) {
		pr_err("Unable to extract alua_access_type\n");
		return ret;
	}
	if ((tmp != 0) && (tmp != 1) && (tmp != 2) && (tmp != 3)) {
		pr_err("Illegal value for alua_access_type:"
>>>>>>> refs/remotes/origin/master
				" %lu\n", tmp);
		return -EINVAL;
	}
	if (tmp == 3)
		tg_pt_gp->tg_pt_gp_alua_access_type =
<<<<<<< HEAD
			TPGS_IMPLICT_ALUA | TPGS_EXPLICT_ALUA;
	else if (tmp == 2)
		tg_pt_gp->tg_pt_gp_alua_access_type = TPGS_EXPLICT_ALUA;
	else if (tmp == 1)
		tg_pt_gp->tg_pt_gp_alua_access_type = TPGS_IMPLICT_ALUA;
=======
			TPGS_IMPLICIT_ALUA | TPGS_EXPLICIT_ALUA;
	else if (tmp == 2)
		tg_pt_gp->tg_pt_gp_alua_access_type = TPGS_EXPLICIT_ALUA;
	else if (tmp == 1)
		tg_pt_gp->tg_pt_gp_alua_access_type = TPGS_IMPLICIT_ALUA;
>>>>>>> refs/remotes/origin/master
	else
		tg_pt_gp->tg_pt_gp_alua_access_type = 0;

	return count;
}

ssize_t core_alua_show_nonop_delay_msecs(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	char *page)
{
	return sprintf(page, "%d\n", tg_pt_gp->tg_pt_gp_nonop_delay_msecs);
}

ssize_t core_alua_store_nonop_delay_msecs(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	const char *page,
	size_t count)
{
	unsigned long tmp;
	int ret;

<<<<<<< HEAD
	ret = strict_strtoul(page, 0, &tmp);
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to extract nonop_delay_msecs\n");
		return -EINVAL;
	}
	if (tmp > ALUA_MAX_NONOP_DELAY_MSECS) {
		printk(KERN_ERR "Passed nonop_delay_msecs: %lu, exceeds"
=======
		pr_err("Unable to extract nonop_delay_msecs\n");
		return -EINVAL;
	}
	if (tmp > ALUA_MAX_NONOP_DELAY_MSECS) {
		pr_err("Passed nonop_delay_msecs: %lu, exceeds"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = kstrtoul(page, 0, &tmp);
	if (ret < 0) {
		pr_err("Unable to extract nonop_delay_msecs\n");
		return ret;
	}
	if (tmp > ALUA_MAX_NONOP_DELAY_MSECS) {
		pr_err("Passed nonop_delay_msecs: %lu, exceeds"
>>>>>>> refs/remotes/origin/master
			" ALUA_MAX_NONOP_DELAY_MSECS: %d\n", tmp,
			ALUA_MAX_NONOP_DELAY_MSECS);
		return -EINVAL;
	}
	tg_pt_gp->tg_pt_gp_nonop_delay_msecs = (int)tmp;

	return count;
}

ssize_t core_alua_show_trans_delay_msecs(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	char *page)
{
	return sprintf(page, "%d\n", tg_pt_gp->tg_pt_gp_trans_delay_msecs);
}

ssize_t core_alua_store_trans_delay_msecs(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	const char *page,
	size_t count)
{
	unsigned long tmp;
	int ret;

<<<<<<< HEAD
	ret = strict_strtoul(page, 0, &tmp);
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to extract trans_delay_msecs\n");
		return -EINVAL;
	}
	if (tmp > ALUA_MAX_TRANS_DELAY_MSECS) {
		printk(KERN_ERR "Passed trans_delay_msecs: %lu, exceeds"
=======
		pr_err("Unable to extract trans_delay_msecs\n");
		return -EINVAL;
	}
	if (tmp > ALUA_MAX_TRANS_DELAY_MSECS) {
		pr_err("Passed trans_delay_msecs: %lu, exceeds"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = kstrtoul(page, 0, &tmp);
	if (ret < 0) {
		pr_err("Unable to extract trans_delay_msecs\n");
		return ret;
	}
	if (tmp > ALUA_MAX_TRANS_DELAY_MSECS) {
		pr_err("Passed trans_delay_msecs: %lu, exceeds"
>>>>>>> refs/remotes/origin/master
			" ALUA_MAX_TRANS_DELAY_MSECS: %d\n", tmp,
			ALUA_MAX_TRANS_DELAY_MSECS);
		return -EINVAL;
	}
	tg_pt_gp->tg_pt_gp_trans_delay_msecs = (int)tmp;

	return count;
}

<<<<<<< HEAD
=======
ssize_t core_alua_show_implicit_trans_secs(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	char *page)
{
	return sprintf(page, "%d\n", tg_pt_gp->tg_pt_gp_implicit_trans_secs);
}

ssize_t core_alua_store_implicit_trans_secs(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	const char *page,
	size_t count)
{
	unsigned long tmp;
	int ret;

	ret = kstrtoul(page, 0, &tmp);
	if (ret < 0) {
		pr_err("Unable to extract implicit_trans_secs\n");
		return ret;
	}
	if (tmp > ALUA_MAX_IMPLICIT_TRANS_SECS) {
		pr_err("Passed implicit_trans_secs: %lu, exceeds"
			" ALUA_MAX_IMPLICIT_TRANS_SECS: %d\n", tmp,
			ALUA_MAX_IMPLICIT_TRANS_SECS);
		return  -EINVAL;
	}
	tg_pt_gp->tg_pt_gp_implicit_trans_secs = (int)tmp;

	return count;
}

>>>>>>> refs/remotes/origin/master
ssize_t core_alua_show_preferred_bit(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	char *page)
{
	return sprintf(page, "%d\n", tg_pt_gp->tg_pt_gp_pref);
}

ssize_t core_alua_store_preferred_bit(
	struct t10_alua_tg_pt_gp *tg_pt_gp,
	const char *page,
	size_t count)
{
	unsigned long tmp;
	int ret;

<<<<<<< HEAD
	ret = strict_strtoul(page, 0, &tmp);
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to extract preferred ALUA value\n");
		return -EINVAL;
	}
	if ((tmp != 0) && (tmp != 1)) {
		printk(KERN_ERR "Illegal value for preferred ALUA: %lu\n", tmp);
=======
		pr_err("Unable to extract preferred ALUA value\n");
		return -EINVAL;
	}
	if ((tmp != 0) && (tmp != 1)) {
		pr_err("Illegal value for preferred ALUA: %lu\n", tmp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = kstrtoul(page, 0, &tmp);
	if (ret < 0) {
		pr_err("Unable to extract preferred ALUA value\n");
		return ret;
	}
	if ((tmp != 0) && (tmp != 1)) {
		pr_err("Illegal value for preferred ALUA: %lu\n", tmp);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}
	tg_pt_gp->tg_pt_gp_pref = (int)tmp;

	return count;
}

ssize_t core_alua_show_offline_bit(struct se_lun *lun, char *page)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(lun->lun_sep))
=======
	if (!lun->lun_sep)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!lun->lun_sep)
>>>>>>> refs/remotes/origin/master
		return -ENODEV;

	return sprintf(page, "%d\n",
		atomic_read(&lun->lun_sep->sep_tg_pt_secondary_offline));
}

ssize_t core_alua_store_offline_bit(
	struct se_lun *lun,
	const char *page,
	size_t count)
{
	struct t10_alua_tg_pt_gp_member *tg_pt_gp_mem;
	unsigned long tmp;
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!(lun->lun_sep))
=======
	if (!lun->lun_sep)
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENODEV;

	ret = strict_strtoul(page, 0, &tmp);
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to extract alua_tg_pt_offline value\n");
		return -EINVAL;
	}
	if ((tmp != 0) && (tmp != 1)) {
		printk(KERN_ERR "Illegal value for alua_tg_pt_offline: %lu\n",
=======
		pr_err("Unable to extract alua_tg_pt_offline value\n");
		return -EINVAL;
	}
	if ((tmp != 0) && (tmp != 1)) {
		pr_err("Illegal value for alua_tg_pt_offline: %lu\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!lun->lun_sep)
		return -ENODEV;

	ret = kstrtoul(page, 0, &tmp);
	if (ret < 0) {
		pr_err("Unable to extract alua_tg_pt_offline value\n");
		return ret;
	}
	if ((tmp != 0) && (tmp != 1)) {
		pr_err("Illegal value for alua_tg_pt_offline: %lu\n",
>>>>>>> refs/remotes/origin/master
				tmp);
		return -EINVAL;
	}
	tg_pt_gp_mem = lun->lun_sep->sep_alua_tg_pt_gp_mem;
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(tg_pt_gp_mem)) {
		printk(KERN_ERR "Unable to locate *tg_pt_gp_mem\n");
=======
	if (!tg_pt_gp_mem) {
		pr_err("Unable to locate *tg_pt_gp_mem\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!tg_pt_gp_mem) {
		pr_err("Unable to locate *tg_pt_gp_mem\n");
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	ret = core_alua_set_tg_pt_secondary_state(tg_pt_gp_mem,
			lun->lun_sep, 0, (int)tmp);
	if (ret < 0)
		return -EINVAL;

	return count;
}

ssize_t core_alua_show_secondary_status(
	struct se_lun *lun,
	char *page)
{
	return sprintf(page, "%d\n", lun->lun_sep->sep_tg_pt_secondary_stat);
}

ssize_t core_alua_store_secondary_status(
	struct se_lun *lun,
	const char *page,
	size_t count)
{
	unsigned long tmp;
	int ret;

<<<<<<< HEAD
	ret = strict_strtoul(page, 0, &tmp);
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to extract alua_tg_pt_status\n");
=======
		pr_err("Unable to extract alua_tg_pt_status\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}
	if ((tmp != ALUA_STATUS_NONE) &&
	    (tmp != ALUA_STATUS_ALTERED_BY_EXPLICT_STPG) &&
	    (tmp != ALUA_STATUS_ALTERED_BY_IMPLICT_ALUA)) {
<<<<<<< HEAD
		printk(KERN_ERR "Illegal value for alua_tg_pt_status: %lu\n",
=======
		pr_err("Illegal value for alua_tg_pt_status: %lu\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = kstrtoul(page, 0, &tmp);
	if (ret < 0) {
		pr_err("Unable to extract alua_tg_pt_status\n");
		return ret;
	}
	if ((tmp != ALUA_STATUS_NONE) &&
	    (tmp != ALUA_STATUS_ALTERED_BY_EXPLICIT_STPG) &&
	    (tmp != ALUA_STATUS_ALTERED_BY_IMPLICIT_ALUA)) {
		pr_err("Illegal value for alua_tg_pt_status: %lu\n",
>>>>>>> refs/remotes/origin/master
				tmp);
		return -EINVAL;
	}
	lun->lun_sep->sep_tg_pt_secondary_stat = (int)tmp;

	return count;
}

ssize_t core_alua_show_secondary_write_metadata(
	struct se_lun *lun,
	char *page)
{
	return sprintf(page, "%d\n",
			lun->lun_sep->sep_tg_pt_secondary_write_md);
}

ssize_t core_alua_store_secondary_write_metadata(
	struct se_lun *lun,
	const char *page,
	size_t count)
{
	unsigned long tmp;
	int ret;

<<<<<<< HEAD
	ret = strict_strtoul(page, 0, &tmp);
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "Unable to extract alua_tg_pt_write_md\n");
		return -EINVAL;
	}
	if ((tmp != 0) && (tmp != 1)) {
		printk(KERN_ERR "Illegal value for alua_tg_pt_write_md:"
=======
		pr_err("Unable to extract alua_tg_pt_write_md\n");
		return -EINVAL;
	}
	if ((tmp != 0) && (tmp != 1)) {
		pr_err("Illegal value for alua_tg_pt_write_md:"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = kstrtoul(page, 0, &tmp);
	if (ret < 0) {
		pr_err("Unable to extract alua_tg_pt_write_md\n");
		return ret;
	}
	if ((tmp != 0) && (tmp != 1)) {
		pr_err("Illegal value for alua_tg_pt_write_md:"
>>>>>>> refs/remotes/origin/master
				" %lu\n", tmp);
		return -EINVAL;
	}
	lun->lun_sep->sep_tg_pt_secondary_write_md = (int)tmp;

	return count;
}

<<<<<<< HEAD
int core_setup_alua(struct se_device *dev, int force_pt)
{
	struct se_subsystem_dev *su_dev = dev->se_sub_dev;
<<<<<<< HEAD
	struct t10_alua *alua = T10_ALUA(su_dev);
=======
	struct t10_alua *alua = &su_dev->t10_alua;
>>>>>>> refs/remotes/origin/cm-10.0
	struct t10_alua_lu_gp_member *lu_gp_mem;
	/*
	 * If this device is from Target_Core_Mod/pSCSI, use the ALUA logic
	 * of the Underlying SCSI hardware.  In Linux/SCSI terms, this can
	 * cause a problem because libata and some SATA RAID HBAs appear
	 * under Linux/SCSI, but emulate SCSI logic themselves.
	 */
<<<<<<< HEAD
	if (((TRANSPORT(dev)->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) &&
	    !(DEV_ATTRIB(dev)->emulate_alua)) || force_pt) {
		alua->alua_type = SPC_ALUA_PASSTHROUGH;
		alua->alua_state_check = &core_alua_state_check_nop;
		printk(KERN_INFO "%s: Using SPC_ALUA_PASSTHROUGH, no ALUA"
			" emulation\n", TRANSPORT(dev)->name);
=======
	if (((dev->transport->transport_type == TRANSPORT_PLUGIN_PHBA_PDEV) &&
	    !(dev->se_sub_dev->se_dev_attrib.emulate_alua)) || force_pt) {
		alua->alua_type = SPC_ALUA_PASSTHROUGH;
		alua->alua_state_check = &core_alua_state_check_nop;
		pr_debug("%s: Using SPC_ALUA_PASSTHROUGH, no ALUA"
			" emulation\n", dev->transport->name);
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;
	}
	/*
	 * If SPC-3 or above is reported by real or emulated struct se_device,
	 * use emulated ALUA.
	 */
<<<<<<< HEAD
	if (TRANSPORT(dev)->get_device_rev(dev) >= SCSI_3) {
		printk(KERN_INFO "%s: Enabling ALUA Emulation for SPC-3"
			" device\n", TRANSPORT(dev)->name);
=======
	if (dev->transport->get_device_rev(dev) >= SCSI_3) {
		pr_debug("%s: Enabling ALUA Emulation for SPC-3"
			" device\n", dev->transport->name);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int core_setup_alua(struct se_device *dev)
{
	if (dev->transport->transport_type != TRANSPORT_PLUGIN_PHBA_PDEV &&
	    !(dev->se_hba->hba_flags & HBA_FLAGS_INTERNAL_USE)) {
		struct t10_alua_lu_gp_member *lu_gp_mem;

>>>>>>> refs/remotes/origin/master
		/*
		 * Associate this struct se_device with the default ALUA
		 * LUN Group.
		 */
		lu_gp_mem = core_alua_allocate_lu_gp_mem(dev);
<<<<<<< HEAD
<<<<<<< HEAD
		if (IS_ERR(lu_gp_mem) || !lu_gp_mem)
			return -1;
=======
		if (IS_ERR(lu_gp_mem))
			return PTR_ERR(lu_gp_mem);
>>>>>>> refs/remotes/origin/cm-10.0

		alua->alua_type = SPC3_ALUA_EMULATED;
		alua->alua_state_check = &core_alua_state_check;
		spin_lock(&lu_gp_mem->lu_gp_mem_lock);
		__core_alua_attach_lu_gp_mem(lu_gp_mem,
<<<<<<< HEAD
				se_global->default_lu_gp);
		spin_unlock(&lu_gp_mem->lu_gp_mem_lock);

		printk(KERN_INFO "%s: Adding to default ALUA LU Group:"
			" core/alua/lu_gps/default_lu_gp\n",
			TRANSPORT(dev)->name);
	} else {
		alua->alua_type = SPC2_ALUA_DISABLED;
		alua->alua_state_check = &core_alua_state_check_nop;
		printk(KERN_INFO "%s: Disabling ALUA Emulation for SPC-2"
			" device\n", TRANSPORT(dev)->name);
=======
=======
		if (IS_ERR(lu_gp_mem))
			return PTR_ERR(lu_gp_mem);

		spin_lock(&lu_gp_mem->lu_gp_mem_lock);
		__core_alua_attach_lu_gp_mem(lu_gp_mem,
>>>>>>> refs/remotes/origin/master
				default_lu_gp);
		spin_unlock(&lu_gp_mem->lu_gp_mem_lock);

		pr_debug("%s: Adding to default ALUA LU Group:"
			" core/alua/lu_gps/default_lu_gp\n",
			dev->transport->name);
<<<<<<< HEAD
	} else {
		alua->alua_type = SPC2_ALUA_DISABLED;
		alua->alua_state_check = &core_alua_state_check_nop;
		pr_debug("%s: Disabling ALUA Emulation for SPC-2"
			" device\n", dev->transport->name);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}
