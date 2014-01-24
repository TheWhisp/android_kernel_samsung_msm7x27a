/*
 * Copyright (c) 2000-2001 Adaptec Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * String handling code courtesy of Gerard Roudier's <groudier@club-internet.fr>
 * sym driver.
 *
 * $Id: //depot/aic7xxx/linux/drivers/scsi/aic7xxx/aic79xx_proc.c#19 $
 */
#include "aic79xx_osm.h"
#include "aic79xx_inline.h"

<<<<<<< HEAD
static void	copy_mem_info(struct info_str *info, char *data, int len);
static int	copy_info(struct info_str *info, char *fmt, ...);
static void	ahd_dump_target_state(struct ahd_softc *ahd,
				      struct info_str *info,
				      u_int our_id, char channel,
				      u_int target_id);
static void	ahd_dump_device_state(struct info_str *info,
				      struct scsi_device *sdev);
static int	ahd_proc_write_seeprom(struct ahd_softc *ahd,
				       char *buffer, int length);
=======
static void	ahd_dump_target_state(struct ahd_softc *ahd,
				      struct seq_file *m,
				      u_int our_id, char channel,
				      u_int target_id);
static void	ahd_dump_device_state(struct seq_file *m,
				      struct scsi_device *sdev);
>>>>>>> refs/remotes/origin/master

/*
 * Table of syncrates that don't follow the "divisible by 4"
 * rule. This table will be expanded in future SCSI specs.
 */
static const struct {
	u_int period_factor;
	u_int period;	/* in 100ths of ns */
} scsi_syncrates[] = {
	{ 0x08, 625 },	/* FAST-160 */
	{ 0x09, 1250 },	/* FAST-80 */
	{ 0x0a, 2500 },	/* FAST-40 40MHz */
	{ 0x0b, 3030 },	/* FAST-40 33MHz */
	{ 0x0c, 5000 }	/* FAST-20 */
};

/*
 * Return the frequency in kHz corresponding to the given
 * sync period factor.
 */
static u_int
ahd_calc_syncsrate(u_int period_factor)
{
	int i;

	/* See if the period is in the "exception" table */
	for (i = 0; i < ARRAY_SIZE(scsi_syncrates); i++) {

		if (period_factor == scsi_syncrates[i].period_factor) {
			/* Period in kHz */
			return (100000000 / scsi_syncrates[i].period);
		}
	}

	/*
	 * Wasn't in the table, so use the standard
	 * 4 times conversion.
	 */
	return (10000000 / (period_factor * 4 * 10));
}

<<<<<<< HEAD

static void
copy_mem_info(struct info_str *info, char *data, int len)
{
	if (info->pos + len > info->offset + info->length)
		len = info->offset + info->length - info->pos;

	if (info->pos + len < info->offset) {
		info->pos += len;
		return;
	}

	if (info->pos < info->offset) {
		off_t partial;

		partial = info->offset - info->pos;
		data += partial;
		info->pos += partial;
		len  -= partial;
	}

	if (len > 0) {
		memcpy(info->buffer, data, len);
		info->pos += len;
		info->buffer += len;
	}
}

static int
copy_info(struct info_str *info, char *fmt, ...)
{
	va_list args;
	char buf[256];
	int len;

	va_start(args, fmt);
	len = vsprintf(buf, fmt, args);
	va_end(args);

	copy_mem_info(info, buf, len);
	return (len);
}

static void
ahd_format_transinfo(struct info_str *info, struct ahd_transinfo *tinfo)
=======
static void
ahd_format_transinfo(struct seq_file *m, struct ahd_transinfo *tinfo)
>>>>>>> refs/remotes/origin/master
{
	u_int speed;
	u_int freq;
	u_int mb;

	if (tinfo->period == AHD_PERIOD_UNKNOWN) {
<<<<<<< HEAD
		copy_info(info, "Renegotiation Pending\n");
=======
		seq_printf(m, "Renegotiation Pending\n");
>>>>>>> refs/remotes/origin/master
		return;
	}
        speed = 3300;
        freq = 0;
	if (tinfo->offset != 0) {
		freq = ahd_calc_syncsrate(tinfo->period);
		speed = freq;
	}
	speed *= (0x01 << tinfo->width);
        mb = speed / 1000;
        if (mb > 0)
<<<<<<< HEAD
		copy_info(info, "%d.%03dMB/s transfers", mb, speed % 1000);
        else
		copy_info(info, "%dKB/s transfers", speed);
=======
		seq_printf(m, "%d.%03dMB/s transfers", mb, speed % 1000);
        else
		seq_printf(m, "%dKB/s transfers", speed);
>>>>>>> refs/remotes/origin/master

	if (freq != 0) {
		int	printed_options;

		printed_options = 0;
<<<<<<< HEAD
		copy_info(info, " (%d.%03dMHz", freq / 1000, freq % 1000);
		if ((tinfo->ppr_options & MSG_EXT_PPR_RD_STRM) != 0) {
			copy_info(info, " RDSTRM");
			printed_options++;
		}
		if ((tinfo->ppr_options & MSG_EXT_PPR_DT_REQ) != 0) {
			copy_info(info, "%s", printed_options ? "|DT" : " DT");
			printed_options++;
		}
		if ((tinfo->ppr_options & MSG_EXT_PPR_IU_REQ) != 0) {
			copy_info(info, "%s", printed_options ? "|IU" : " IU");
			printed_options++;
		}
		if ((tinfo->ppr_options & MSG_EXT_PPR_RTI) != 0) {
			copy_info(info, "%s",
=======
		seq_printf(m, " (%d.%03dMHz", freq / 1000, freq % 1000);
		if ((tinfo->ppr_options & MSG_EXT_PPR_RD_STRM) != 0) {
			seq_printf(m, " RDSTRM");
			printed_options++;
		}
		if ((tinfo->ppr_options & MSG_EXT_PPR_DT_REQ) != 0) {
			seq_printf(m, "%s", printed_options ? "|DT" : " DT");
			printed_options++;
		}
		if ((tinfo->ppr_options & MSG_EXT_PPR_IU_REQ) != 0) {
			seq_printf(m, "%s", printed_options ? "|IU" : " IU");
			printed_options++;
		}
		if ((tinfo->ppr_options & MSG_EXT_PPR_RTI) != 0) {
			seq_printf(m, "%s",
>>>>>>> refs/remotes/origin/master
				  printed_options ? "|RTI" : " RTI");
			printed_options++;
		}
		if ((tinfo->ppr_options & MSG_EXT_PPR_QAS_REQ) != 0) {
<<<<<<< HEAD
			copy_info(info, "%s",
=======
			seq_printf(m, "%s",
>>>>>>> refs/remotes/origin/master
				  printed_options ? "|QAS" : " QAS");
			printed_options++;
		}
	}

	if (tinfo->width > 0) {
		if (freq != 0) {
<<<<<<< HEAD
			copy_info(info, ", ");
		} else {
			copy_info(info, " (");
		}
		copy_info(info, "%dbit)", 8 * (0x01 << tinfo->width));
	} else if (freq != 0) {
		copy_info(info, ")");
	}
	copy_info(info, "\n");
}

static void
ahd_dump_target_state(struct ahd_softc *ahd, struct info_str *info,
=======
			seq_printf(m, ", ");
		} else {
			seq_printf(m, " (");
		}
		seq_printf(m, "%dbit)", 8 * (0x01 << tinfo->width));
	} else if (freq != 0) {
		seq_printf(m, ")");
	}
	seq_printf(m, "\n");
}

static void
ahd_dump_target_state(struct ahd_softc *ahd, struct seq_file *m,
>>>>>>> refs/remotes/origin/master
		      u_int our_id, char channel, u_int target_id)
{
	struct  scsi_target *starget;
	struct	ahd_initiator_tinfo *tinfo;
	struct	ahd_tmode_tstate *tstate;
	int	lun;

	tinfo = ahd_fetch_transinfo(ahd, channel, our_id,
				    target_id, &tstate);
<<<<<<< HEAD
	copy_info(info, "Target %d Negotiation Settings\n", target_id);
	copy_info(info, "\tUser: ");
	ahd_format_transinfo(info, &tinfo->user);
=======
	seq_printf(m, "Target %d Negotiation Settings\n", target_id);
	seq_printf(m, "\tUser: ");
	ahd_format_transinfo(m, &tinfo->user);
>>>>>>> refs/remotes/origin/master
	starget = ahd->platform_data->starget[target_id];
	if (starget == NULL)
		return;

<<<<<<< HEAD
	copy_info(info, "\tGoal: ");
	ahd_format_transinfo(info, &tinfo->goal);
	copy_info(info, "\tCurr: ");
	ahd_format_transinfo(info, &tinfo->curr);
=======
	seq_printf(m, "\tGoal: ");
	ahd_format_transinfo(m, &tinfo->goal);
	seq_printf(m, "\tCurr: ");
	ahd_format_transinfo(m, &tinfo->curr);
>>>>>>> refs/remotes/origin/master

	for (lun = 0; lun < AHD_NUM_LUNS; lun++) {
		struct scsi_device *dev;

		dev = scsi_device_lookup_by_target(starget, lun);

		if (dev == NULL)
			continue;

<<<<<<< HEAD
		ahd_dump_device_state(info, dev);
=======
		ahd_dump_device_state(m, dev);
>>>>>>> refs/remotes/origin/master
	}
}

static void
<<<<<<< HEAD
ahd_dump_device_state(struct info_str *info, struct scsi_device *sdev)
{
	struct ahd_linux_device *dev = scsi_transport_device_data(sdev);

	copy_info(info, "\tChannel %c Target %d Lun %d Settings\n",
		  sdev->sdev_target->channel + 'A',
		  sdev->sdev_target->id, sdev->lun);

	copy_info(info, "\t\tCommands Queued %ld\n", dev->commands_issued);
	copy_info(info, "\t\tCommands Active %d\n", dev->active);
	copy_info(info, "\t\tCommand Openings %d\n", dev->openings);
	copy_info(info, "\t\tMax Tagged Openings %d\n", dev->maxtags);
	copy_info(info, "\t\tDevice Queue Frozen Count %d\n", dev->qfrozen);
}

static int
ahd_proc_write_seeprom(struct ahd_softc *ahd, char *buffer, int length)
{
=======
ahd_dump_device_state(struct seq_file *m, struct scsi_device *sdev)
{
	struct ahd_linux_device *dev = scsi_transport_device_data(sdev);

	seq_printf(m, "\tChannel %c Target %d Lun %d Settings\n",
		  sdev->sdev_target->channel + 'A',
		  sdev->sdev_target->id, sdev->lun);

	seq_printf(m, "\t\tCommands Queued %ld\n", dev->commands_issued);
	seq_printf(m, "\t\tCommands Active %d\n", dev->active);
	seq_printf(m, "\t\tCommand Openings %d\n", dev->openings);
	seq_printf(m, "\t\tMax Tagged Openings %d\n", dev->maxtags);
	seq_printf(m, "\t\tDevice Queue Frozen Count %d\n", dev->qfrozen);
}

int
ahd_proc_write_seeprom(struct Scsi_Host *shost, char *buffer, int length)
{
	struct	ahd_softc *ahd = *(struct ahd_softc **)shost->hostdata;
>>>>>>> refs/remotes/origin/master
	ahd_mode_state saved_modes;
	int have_seeprom;
	u_long s;
	int paused;
	int written;

	/* Default to failure. */
	written = -EINVAL;
	ahd_lock(ahd, &s);
	paused = ahd_is_paused(ahd);
	if (!paused)
		ahd_pause(ahd);

	saved_modes = ahd_save_modes(ahd);
	ahd_set_modes(ahd, AHD_MODE_SCSI, AHD_MODE_SCSI);
	if (length != sizeof(struct seeprom_config)) {
		printk("ahd_proc_write_seeprom: incorrect buffer size\n");
		goto done;
	}

	have_seeprom = ahd_verify_cksum((struct seeprom_config*)buffer);
	if (have_seeprom == 0) {
		printk("ahd_proc_write_seeprom: cksum verification failed\n");
		goto done;
	}

	have_seeprom = ahd_acquire_seeprom(ahd);
	if (!have_seeprom) {
		printk("ahd_proc_write_seeprom: No Serial EEPROM\n");
		goto done;
	} else {
		u_int start_addr;

		if (ahd->seep_config == NULL) {
			ahd->seep_config = kmalloc(sizeof(*ahd->seep_config), GFP_ATOMIC);
			if (ahd->seep_config == NULL) {
				printk("aic79xx: Unable to allocate serial "
				       "eeprom buffer.  Write failing\n");
				goto done;
			}
		}
		printk("aic79xx: Writing Serial EEPROM\n");
		start_addr = 32 * (ahd->channel - 'A');
		ahd_write_seeprom(ahd, (u_int16_t *)buffer, start_addr,
				  sizeof(struct seeprom_config)/2);
		ahd_read_seeprom(ahd, (uint16_t *)ahd->seep_config,
				 start_addr, sizeof(struct seeprom_config)/2,
				 /*ByteStream*/FALSE);
		ahd_release_seeprom(ahd);
		written = length;
	}

done:
	ahd_restore_modes(ahd, saved_modes);
	if (!paused)
		ahd_unpause(ahd);
	ahd_unlock(ahd, &s);
	return (written);
}
/*
 * Return information to handle /proc support for the driver.
 */
int
<<<<<<< HEAD
ahd_linux_proc_info(struct Scsi_Host *shost, char *buffer, char **start,
		    off_t offset, int length, int inout)
{
	struct	ahd_softc *ahd = *(struct ahd_softc **)shost->hostdata;
	struct	info_str info;
	char	ahd_info[256];
	u_int	max_targ;
	u_int	i;
	int	retval;

	 /* Has data been written to the file? */ 
	if (inout == TRUE) {
		retval = ahd_proc_write_seeprom(ahd, buffer, length);
		goto done;
	}

	if (start)
		*start = buffer;

	info.buffer	= buffer;
	info.length	= length;
	info.offset	= offset;
	info.pos	= 0;

	copy_info(&info, "Adaptec AIC79xx driver version: %s\n",
		  AIC79XX_DRIVER_VERSION);
	copy_info(&info, "%s\n", ahd->description);
	ahd_controller_info(ahd, ahd_info);
	copy_info(&info, "%s\n", ahd_info);
	copy_info(&info, "Allocated SCBs: %d, SG List Length: %d\n\n",
=======
ahd_linux_show_info(struct seq_file *m, struct Scsi_Host *shost)
{
	struct	ahd_softc *ahd = *(struct ahd_softc **)shost->hostdata;
	char	ahd_info[256];
	u_int	max_targ;
	u_int	i;

	seq_printf(m, "Adaptec AIC79xx driver version: %s\n",
		  AIC79XX_DRIVER_VERSION);
	seq_printf(m, "%s\n", ahd->description);
	ahd_controller_info(ahd, ahd_info);
	seq_printf(m, "%s\n", ahd_info);
	seq_printf(m, "Allocated SCBs: %d, SG List Length: %d\n\n",
>>>>>>> refs/remotes/origin/master
		  ahd->scb_data.numscbs, AHD_NSEG);

	max_targ = 16;

	if (ahd->seep_config == NULL)
<<<<<<< HEAD
		copy_info(&info, "No Serial EEPROM\n");
	else {
		copy_info(&info, "Serial EEPROM:\n");
		for (i = 0; i < sizeof(*ahd->seep_config)/2; i++) {
			if (((i % 8) == 0) && (i != 0)) {
				copy_info(&info, "\n");
			}
			copy_info(&info, "0x%.4x ",
				  ((uint16_t*)ahd->seep_config)[i]);
		}
		copy_info(&info, "\n");
	}
	copy_info(&info, "\n");
=======
		seq_printf(m, "No Serial EEPROM\n");
	else {
		seq_printf(m, "Serial EEPROM:\n");
		for (i = 0; i < sizeof(*ahd->seep_config)/2; i++) {
			if (((i % 8) == 0) && (i != 0)) {
				seq_printf(m, "\n");
			}
			seq_printf(m, "0x%.4x ",
				  ((uint16_t*)ahd->seep_config)[i]);
		}
		seq_printf(m, "\n");
	}
	seq_printf(m, "\n");
>>>>>>> refs/remotes/origin/master

	if ((ahd->features & AHD_WIDE) == 0)
		max_targ = 8;

	for (i = 0; i < max_targ; i++) {

<<<<<<< HEAD
		ahd_dump_target_state(ahd, &info, ahd->our_id, 'A',
				      /*target_id*/i);
	}
	retval = info.pos > info.offset ? info.pos - info.offset : 0;
done:
	return (retval);
=======
		ahd_dump_target_state(ahd, m, ahd->our_id, 'A',
				      /*target_id*/i);
	}
	return 0;
>>>>>>> refs/remotes/origin/master
}
