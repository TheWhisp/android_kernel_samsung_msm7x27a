/*
 * USB Attached SCSI
 * Note that this is not the same as the USB Mass Storage driver
 *
 * Copyright Matthew Wilcox for Intel Corp, 2010
 * Copyright Sarah Sharp for Intel Corp, 2010
 *
 * Distributed under the terms of the GNU GPL, version two.
 */

#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/types.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/usb.h>
#include <linux/usb/storage.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/usb/storage.h>
#include <linux/usb/uas.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#include <scsi/scsi.h>
#include <scsi/scsi_dbg.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_tcq.h>

<<<<<<< HEAD
<<<<<<< HEAD
/* Common header for all IUs */
struct iu {
	__u8 iu_id;
	__u8 rsvd1;
	__be16 tag;
};

enum {
	IU_ID_COMMAND		= 0x01,
	IU_ID_STATUS		= 0x03,
	IU_ID_RESPONSE		= 0x04,
	IU_ID_TASK_MGMT		= 0x05,
	IU_ID_READ_READY	= 0x06,
	IU_ID_WRITE_READY	= 0x07,
};

struct command_iu {
	__u8 iu_id;
	__u8 rsvd1;
	__be16 tag;
	__u8 prio_attr;
	__u8 rsvd5;
	__u8 len;
	__u8 rsvd7;
	struct scsi_lun lun;
	__u8 cdb[16];	/* XXX: Overflow-checking tools may misunderstand */
};

/*
 * Also used for the Read Ready and Write Ready IUs since they have the
 * same first four bytes
 */
struct sense_iu {
	__u8 iu_id;
	__u8 rsvd1;
	__be16 tag;
	__be16 status_qual;
	__u8 status;
	__u8 rsvd7[7];
	__be16 len;
	__u8 sense[SCSI_SENSE_BUFFERSIZE];
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * The r00-r01c specs define this version of the SENSE IU data structure.
 * It's still in use by several different firmware releases.
 */
struct sense_iu_old {
	__u8 iu_id;
	__u8 rsvd1;
	__be16 tag;
	__be16 len;
	__u8 status;
	__u8 service_response;
	__u8 sense[SCSI_SENSE_BUFFERSIZE];
};

<<<<<<< HEAD
<<<<<<< HEAD
enum {
	CMD_PIPE_ID		= 1,
	STATUS_PIPE_ID		= 2,
	DATA_IN_PIPE_ID		= 3,
	DATA_OUT_PIPE_ID	= 4,

	UAS_SIMPLE_TAG		= 0,
	UAS_HEAD_TAG		= 1,
	UAS_ORDERED_TAG		= 2,
	UAS_ACA			= 4,
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
struct uas_dev_info {
	struct usb_interface *intf;
	struct usb_device *udev;
	int qdepth;
	unsigned cmd_pipe, status_pipe, data_in_pipe, data_out_pipe;
	unsigned use_streams:1;
	unsigned uas_sense_old:1;
<<<<<<< HEAD
=======
	struct scsi_cmnd *cmnd;
	struct urb *status_urb; /* used only if stream support is available */
>>>>>>> refs/remotes/origin/cm-10.0
};

enum {
	ALLOC_STATUS_URB	= (1 << 0),
=======
struct uas_dev_info {
	struct usb_interface *intf;
	struct usb_device *udev;
	struct usb_anchor cmd_urbs;
	struct usb_anchor sense_urbs;
	struct usb_anchor data_urbs;
	int qdepth, resetting;
	struct response_ui response;
	unsigned cmd_pipe, status_pipe, data_in_pipe, data_out_pipe;
	unsigned use_streams:1;
	unsigned uas_sense_old:1;
	struct scsi_cmnd *cmnd;
	spinlock_t lock;
};

enum {
>>>>>>> refs/remotes/origin/master
	SUBMIT_STATUS_URB	= (1 << 1),
	ALLOC_DATA_IN_URB	= (1 << 2),
	SUBMIT_DATA_IN_URB	= (1 << 3),
	ALLOC_DATA_OUT_URB	= (1 << 4),
	SUBMIT_DATA_OUT_URB	= (1 << 5),
	ALLOC_CMD_URB		= (1 << 6),
	SUBMIT_CMD_URB		= (1 << 7),
<<<<<<< HEAD
<<<<<<< HEAD
=======
	COMMAND_INFLIGHT        = (1 << 8),
	DATA_IN_URB_INFLIGHT    = (1 << 9),
	DATA_OUT_URB_INFLIGHT   = (1 << 10),
	COMMAND_COMPLETED       = (1 << 11),
	COMMAND_ABORTED         = (1 << 12),
	UNLINK_DATA_URBS        = (1 << 13),
	IS_IN_WORK_LIST         = (1 << 14),
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

/* Overrides scsi_pointer */
struct uas_cmd_info {
	unsigned int state;
	unsigned int stream;
	struct urb *cmd_urb;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* status_urb is used only if stream support isn't available */
>>>>>>> refs/remotes/origin/cm-10.0
	struct urb *status_urb;
=======
>>>>>>> refs/remotes/origin/master
	struct urb *data_in_urb;
	struct urb *data_out_urb;
	struct list_head list;
};

/* I hate forward declarations, but I actually have a loop */
static int uas_submit_urbs(struct scsi_cmnd *cmnd,
				struct uas_dev_info *devinfo, gfp_t gfp);
<<<<<<< HEAD
<<<<<<< HEAD

=======
static void uas_do_work(struct work_struct *work);

static DECLARE_WORK(uas_work, uas_do_work);
>>>>>>> refs/remotes/origin/cm-10.0
static DEFINE_SPINLOCK(uas_work_lock);
static LIST_HEAD(uas_work_list);

static void uas_do_work(struct work_struct *work)
{
	struct uas_cmd_info *cmdinfo;
<<<<<<< HEAD
	struct list_head list;
=======
	struct uas_cmd_info *temp;
	struct list_head list;
	int err;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void uas_do_work(struct work_struct *work);
static int uas_try_complete(struct scsi_cmnd *cmnd, const char *caller);

static DECLARE_WORK(uas_work, uas_do_work);
static DEFINE_SPINLOCK(uas_work_lock);
static LIST_HEAD(uas_work_list);

static void uas_unlink_data_urbs(struct uas_dev_info *devinfo,
				 struct uas_cmd_info *cmdinfo)
{
	unsigned long flags;

	/*
	 * The UNLINK_DATA_URBS flag makes sure uas_try_complete
	 * (called by urb completion) doesn't release cmdinfo
	 * underneath us.
	 */
	spin_lock_irqsave(&devinfo->lock, flags);
	cmdinfo->state |= UNLINK_DATA_URBS;
	spin_unlock_irqrestore(&devinfo->lock, flags);

	if (cmdinfo->data_in_urb)
		usb_unlink_urb(cmdinfo->data_in_urb);
	if (cmdinfo->data_out_urb)
		usb_unlink_urb(cmdinfo->data_out_urb);

	spin_lock_irqsave(&devinfo->lock, flags);
	cmdinfo->state &= ~UNLINK_DATA_URBS;
	spin_unlock_irqrestore(&devinfo->lock, flags);
}

static void uas_do_work(struct work_struct *work)
{
	struct uas_cmd_info *cmdinfo;
	struct uas_cmd_info *temp;
	struct list_head list;
	unsigned long flags;
	int err;
>>>>>>> refs/remotes/origin/master

	spin_lock_irq(&uas_work_lock);
	list_replace_init(&uas_work_list, &list);
	spin_unlock_irq(&uas_work_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(cmdinfo, &list, list) {
		struct scsi_pointer *scp = (void *)cmdinfo;
		struct scsi_cmnd *cmnd = container_of(scp,
							struct scsi_cmnd, SCp);
		uas_submit_urbs(cmnd, cmnd->device->hostdata, GFP_NOIO);
	}
}

static DECLARE_WORK(uas_work, uas_do_work);

=======
=======
	list_for_each_entry_safe(cmdinfo, temp, &list, list) {
		struct scsi_pointer *scp = (void *)cmdinfo;
		struct scsi_cmnd *cmnd = container_of(scp,
							struct scsi_cmnd, SCp);
		struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;
		spin_lock_irqsave(&devinfo->lock, flags);
		err = uas_submit_urbs(cmnd, cmnd->device->hostdata, GFP_ATOMIC);
		if (!err)
			cmdinfo->state &= ~IS_IN_WORK_LIST;
		spin_unlock_irqrestore(&devinfo->lock, flags);
		if (err) {
			list_del(&cmdinfo->list);
			spin_lock_irq(&uas_work_lock);
			list_add_tail(&cmdinfo->list, &uas_work_list);
			spin_unlock_irq(&uas_work_lock);
			schedule_work(&uas_work);
		}
	}
}

static void uas_abort_work(struct uas_dev_info *devinfo)
{
	struct uas_cmd_info *cmdinfo;
	struct uas_cmd_info *temp;
	struct list_head list;
	unsigned long flags;

	spin_lock_irq(&uas_work_lock);
	list_replace_init(&uas_work_list, &list);
	spin_unlock_irq(&uas_work_lock);

	spin_lock_irqsave(&devinfo->lock, flags);
>>>>>>> refs/remotes/origin/master
	list_for_each_entry_safe(cmdinfo, temp, &list, list) {
		struct scsi_pointer *scp = (void *)cmdinfo;
		struct scsi_cmnd *cmnd = container_of(scp,
							struct scsi_cmnd, SCp);
<<<<<<< HEAD
		err = uas_submit_urbs(cmnd, cmnd->device->hostdata, GFP_NOIO);
		if (err) {
=======
		struct uas_dev_info *di = (void *)cmnd->device->hostdata;

		if (di == devinfo) {
			cmdinfo->state |= COMMAND_ABORTED;
			cmdinfo->state &= ~IS_IN_WORK_LIST;
			if (devinfo->resetting) {
				/* uas_stat_cmplt() will not do that
				 * when a device reset is in
				 * progress */
				cmdinfo->state &= ~COMMAND_INFLIGHT;
			}
			uas_try_complete(cmnd, __func__);
		} else {
			/* not our uas device, relink into list */
>>>>>>> refs/remotes/origin/master
			list_del(&cmdinfo->list);
			spin_lock_irq(&uas_work_lock);
			list_add_tail(&cmdinfo->list, &uas_work_list);
			spin_unlock_irq(&uas_work_lock);
<<<<<<< HEAD
			schedule_work(&uas_work);
		}
	}
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
		}
	}
	spin_unlock_irqrestore(&devinfo->lock, flags);
}

>>>>>>> refs/remotes/origin/master
static void uas_sense(struct urb *urb, struct scsi_cmnd *cmnd)
{
	struct sense_iu *sense_iu = urb->transfer_buffer;
	struct scsi_device *sdev = cmnd->device;

	if (urb->actual_length > 16) {
		unsigned len = be16_to_cpup(&sense_iu->len);
		if (len + 16 != urb->actual_length) {
			int newlen = min(len + 16, urb->actual_length) - 16;
			if (newlen < 0)
				newlen = 0;
			sdev_printk(KERN_INFO, sdev, "%s: urb length %d "
				"disagrees with IU sense data length %d, "
				"using %d bytes of sense data\n", __func__,
					urb->actual_length, len, newlen);
			len = newlen;
		}
		memcpy(cmnd->sense_buffer, sense_iu->sense, len);
	}

	cmnd->result = sense_iu->status;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (sdev->current_cmnd)
		sdev->current_cmnd = NULL;
	cmnd->scsi_done(cmnd);
	usb_free_urb(urb);
=======
	cmnd->scsi_done(cmnd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	cmnd->scsi_done(cmnd);
>>>>>>> refs/remotes/origin/cm-11.0
}

static void uas_sense_old(struct urb *urb, struct scsi_cmnd *cmnd)
{
	struct sense_iu_old *sense_iu = urb->transfer_buffer;
	struct scsi_device *sdev = cmnd->device;

	if (urb->actual_length > 8) {
		unsigned len = be16_to_cpup(&sense_iu->len) - 2;
		if (len + 8 != urb->actual_length) {
			int newlen = min(len + 8, urb->actual_length) - 8;
			if (newlen < 0)
				newlen = 0;
			sdev_printk(KERN_INFO, sdev, "%s: urb length %d "
				"disagrees with IU sense data length %d, "
				"using %d bytes of sense data\n", __func__,
					urb->actual_length, len, newlen);
			len = newlen;
		}
		memcpy(cmnd->sense_buffer, sense_iu->sense, len);
	}

	cmnd->result = sense_iu->status;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (sdev->current_cmnd)
		sdev->current_cmnd = NULL;
	cmnd->scsi_done(cmnd);
	usb_free_urb(urb);
=======
	cmnd->scsi_done(cmnd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cmnd->scsi_done(cmnd);
>>>>>>> refs/remotes/origin/cm-11.0
}

static void uas_xfer_data(struct urb *urb, struct scsi_cmnd *cmnd,
							unsigned direction)
=======
}

static void uas_log_cmd_state(struct scsi_cmnd *cmnd, const char *caller)
{
	struct uas_cmd_info *ci = (void *)&cmnd->SCp;

	scmd_printk(KERN_INFO, cmnd, "%s %p tag %d, inflight:"
		    "%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
		    caller, cmnd, cmnd->request->tag,
		    (ci->state & SUBMIT_STATUS_URB)     ? " s-st"  : "",
		    (ci->state & ALLOC_DATA_IN_URB)     ? " a-in"  : "",
		    (ci->state & SUBMIT_DATA_IN_URB)    ? " s-in"  : "",
		    (ci->state & ALLOC_DATA_OUT_URB)    ? " a-out" : "",
		    (ci->state & SUBMIT_DATA_OUT_URB)   ? " s-out" : "",
		    (ci->state & ALLOC_CMD_URB)         ? " a-cmd" : "",
		    (ci->state & SUBMIT_CMD_URB)        ? " s-cmd" : "",
		    (ci->state & COMMAND_INFLIGHT)      ? " CMD"   : "",
		    (ci->state & DATA_IN_URB_INFLIGHT)  ? " IN"    : "",
		    (ci->state & DATA_OUT_URB_INFLIGHT) ? " OUT"   : "",
		    (ci->state & COMMAND_COMPLETED)     ? " done"  : "",
		    (ci->state & COMMAND_ABORTED)       ? " abort" : "",
		    (ci->state & UNLINK_DATA_URBS)      ? " unlink": "",
		    (ci->state & IS_IN_WORK_LIST)       ? " work"  : "");
}

static int uas_try_complete(struct scsi_cmnd *cmnd, const char *caller)
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;

	WARN_ON(!spin_is_locked(&devinfo->lock));
	if (cmdinfo->state & (COMMAND_INFLIGHT |
			      DATA_IN_URB_INFLIGHT |
			      DATA_OUT_URB_INFLIGHT |
			      UNLINK_DATA_URBS))
		return -EBUSY;
	BUG_ON(cmdinfo->state & COMMAND_COMPLETED);
	cmdinfo->state |= COMMAND_COMPLETED;
	usb_free_urb(cmdinfo->data_in_urb);
	usb_free_urb(cmdinfo->data_out_urb);
	if (cmdinfo->state & COMMAND_ABORTED) {
		scmd_printk(KERN_INFO, cmnd, "abort completed\n");
		cmnd->result = DID_ABORT << 16;
	}
	cmnd->scsi_done(cmnd);
	return 0;
}

static void uas_xfer_data(struct urb *urb, struct scsi_cmnd *cmnd,
			  unsigned direction)
>>>>>>> refs/remotes/origin/master
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	cmdinfo->state = direction | SUBMIT_STATUS_URB;
=======
	cmdinfo->state = direction;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cmdinfo->state |= direction | SUBMIT_STATUS_URB;
>>>>>>> refs/remotes/origin/master
	err = uas_submit_urbs(cmnd, cmnd->device->hostdata, GFP_ATOMIC);
	if (err) {
		spin_lock(&uas_work_lock);
		list_add_tail(&cmdinfo->list, &uas_work_list);
<<<<<<< HEAD
=======
		cmdinfo->state |= IS_IN_WORK_LIST;
>>>>>>> refs/remotes/origin/master
		spin_unlock(&uas_work_lock);
		schedule_work(&uas_work);
	}
}

static void uas_stat_cmplt(struct urb *urb)
{
	struct iu *iu = urb->transfer_buffer;
<<<<<<< HEAD
<<<<<<< HEAD
	struct scsi_device *sdev = urb->context;
	struct uas_dev_info *devinfo = sdev->hostdata;
	struct scsi_cmnd *cmnd;
	u16 tag;

	if (urb->status) {
		dev_err(&urb->dev->dev, "URB BAD STATUS %d\n", urb->status);
		usb_free_urb(urb);
=======
	struct Scsi_Host *shost = urb->context;
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];
	struct scsi_cmnd *cmnd;
	u16 tag;
	int ret;

	if (urb->status) {
		dev_err(&urb->dev->dev, "URB BAD STATUS %d\n", urb->status);
		if (devinfo->use_streams)
			usb_free_urb(urb);
>>>>>>> refs/remotes/origin/cm-10.0
		return;
	}

	tag = be16_to_cpup(&iu->tag) - 1;
<<<<<<< HEAD
	if (sdev->current_cmnd)
		cmnd = sdev->current_cmnd;
	else
		cmnd = scsi_find_tag(sdev, tag);
	if (!cmnd)
		return;

	switch (iu->iu_id) {
	case IU_ID_STATUS:
=======
=======
	struct Scsi_Host *shost = urb->context;
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];
	struct scsi_cmnd *cmnd;
	struct uas_cmd_info *cmdinfo;
	unsigned long flags;
	u16 tag;

	if (urb->status) {
		dev_err(&urb->dev->dev, "URB BAD STATUS %d\n", urb->status);
		usb_free_urb(urb);
		return;
	}

	if (devinfo->resetting) {
		usb_free_urb(urb);
		return;
	}

	spin_lock_irqsave(&devinfo->lock, flags);
	tag = be16_to_cpup(&iu->tag) - 1;
>>>>>>> refs/remotes/origin/master
	if (tag == 0)
		cmnd = devinfo->cmnd;
	else
		cmnd = scsi_host_find_tag(shost, tag - 1);
<<<<<<< HEAD
	if (!cmnd) {
		if (devinfo->use_streams) {
			usb_free_urb(urb);
			return;
		}
		ret = usb_submit_urb(urb, GFP_ATOMIC);
		if (ret)
			dev_err(&urb->dev->dev, "failed submit status urb\n");
		return;
	}

=======

	if (!cmnd) {
		if (iu->iu_id == IU_ID_RESPONSE) {
			/* store results for uas_eh_task_mgmt() */
			memcpy(&devinfo->response, iu, sizeof(devinfo->response));
		}
		usb_free_urb(urb);
		spin_unlock_irqrestore(&devinfo->lock, flags);
		return;
	}

	cmdinfo = (void *)&cmnd->SCp;
>>>>>>> refs/remotes/origin/master
	switch (iu->iu_id) {
	case IU_ID_STATUS:
		if (devinfo->cmnd == cmnd)
			devinfo->cmnd = NULL;

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (urb->actual_length < 16)
			devinfo->uas_sense_old = 1;
		if (devinfo->uas_sense_old)
			uas_sense_old(urb, cmnd);
		else
			uas_sense(urb, cmnd);
<<<<<<< HEAD
=======
		if (cmnd->result != 0) {
			/* cancel data transfers on error */
			spin_unlock_irqrestore(&devinfo->lock, flags);
			uas_unlink_data_urbs(devinfo, cmdinfo);
			spin_lock_irqsave(&devinfo->lock, flags);
		}
		cmdinfo->state &= ~COMMAND_INFLIGHT;
		uas_try_complete(cmnd, __func__);
>>>>>>> refs/remotes/origin/master
		break;
	case IU_ID_READ_READY:
		uas_xfer_data(urb, cmnd, SUBMIT_DATA_IN_URB);
		break;
	case IU_ID_WRITE_READY:
		uas_xfer_data(urb, cmnd, SUBMIT_DATA_OUT_URB);
		break;
	default:
		scmd_printk(KERN_ERR, cmnd,
			"Bogus IU (%d) received on status pipe\n", iu->iu_id);
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======

	if (devinfo->use_streams) {
		usb_free_urb(urb);
		return;
	}

	ret = usb_submit_urb(urb, GFP_ATOMIC);
	if (ret)
		dev_err(&urb->dev->dev, "failed submit status urb\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	usb_free_urb(urb);
	spin_unlock_irqrestore(&devinfo->lock, flags);
>>>>>>> refs/remotes/origin/master
}

static void uas_data_cmplt(struct urb *urb)
<<<<<<< HEAD
{
<<<<<<< HEAD
	struct scsi_data_buffer *sdb = urb->context;
	sdb->resid = sdb->length - urb->actual_length;
	usb_free_urb(urb);
}

static struct urb *uas_alloc_data_urb(struct uas_dev_info *devinfo, gfp_t gfp,
				unsigned int pipe, u16 stream_id,
				struct scsi_data_buffer *sdb,
				enum dma_data_direction dir)
{
	struct usb_device *udev = devinfo->udev;
	struct urb *urb = usb_alloc_urb(0, gfp);

	if (!urb)
		goto out;
	usb_fill_bulk_urb(urb, udev, pipe, NULL, sdb->length, uas_data_cmplt,
									sdb);
=======
	struct scsi_cmnd *cmnd = urb->context;
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;
	struct scsi_data_buffer *sdb = NULL;
	unsigned long flags;

	spin_lock_irqsave(&devinfo->lock, flags);
	if (cmdinfo->data_in_urb == urb) {
		sdb = scsi_in(cmnd);
		cmdinfo->state &= ~DATA_IN_URB_INFLIGHT;
	} else if (cmdinfo->data_out_urb == urb) {
		sdb = scsi_out(cmnd);
		cmdinfo->state &= ~DATA_OUT_URB_INFLIGHT;
	}
	BUG_ON(sdb == NULL);
	if (urb->status) {
		/* error: no data transfered */
		sdb->resid = sdb->length;
	} else {
		sdb->resid = sdb->length - urb->actual_length;
	}
	uas_try_complete(cmnd, __func__);
	spin_unlock_irqrestore(&devinfo->lock, flags);
}

static struct urb *uas_alloc_data_urb(struct uas_dev_info *devinfo, gfp_t gfp,
				      unsigned int pipe, u16 stream_id,
				      struct scsi_cmnd *cmnd,
				      enum dma_data_direction dir)
{
	struct usb_device *udev = devinfo->udev;
	struct urb *urb = usb_alloc_urb(0, gfp);
	struct scsi_data_buffer *sdb = (dir == DMA_FROM_DEVICE)
		? scsi_in(cmnd) : scsi_out(cmnd);

	if (!urb)
		goto out;
	usb_fill_bulk_urb(urb, udev, pipe, NULL, sdb->length,
			  uas_data_cmplt, cmnd);
>>>>>>> refs/remotes/origin/master
=======
{
	struct scsi_data_buffer *sdb = urb->context;
	sdb->resid = sdb->length - urb->actual_length;
	usb_free_urb(urb);
}

static struct urb *uas_alloc_data_urb(struct uas_dev_info *devinfo, gfp_t gfp,
				unsigned int pipe, u16 stream_id,
				struct scsi_data_buffer *sdb,
				enum dma_data_direction dir)
{
	struct usb_device *udev = devinfo->udev;
	struct urb *urb = usb_alloc_urb(0, gfp);

	if (!urb)
		goto out;
	usb_fill_bulk_urb(urb, udev, pipe, NULL, sdb->length, uas_data_cmplt,
									sdb);
>>>>>>> refs/remotes/origin/cm-11.0
	if (devinfo->use_streams)
		urb->stream_id = stream_id;
	urb->num_sgs = udev->bus->sg_tablesize ? sdb->table.nents : 0;
	urb->sg = sdb->table.sgl;
 out:
	return urb;
}

static struct urb *uas_alloc_sense_urb(struct uas_dev_info *devinfo, gfp_t gfp,
<<<<<<< HEAD
<<<<<<< HEAD
					struct scsi_cmnd *cmnd, u16 stream_id)
=======
		struct Scsi_Host *shost, u16 stream_id)
>>>>>>> refs/remotes/origin/cm-10.0
=======
				       struct Scsi_Host *shost, u16 stream_id)
>>>>>>> refs/remotes/origin/master
{
	struct usb_device *udev = devinfo->udev;
	struct urb *urb = usb_alloc_urb(0, gfp);
	struct sense_iu *iu;

	if (!urb)
		goto out;

	iu = kzalloc(sizeof(*iu), gfp);
	if (!iu)
		goto free;

	usb_fill_bulk_urb(urb, udev, devinfo->status_pipe, iu, sizeof(*iu),
<<<<<<< HEAD
<<<<<<< HEAD
						uas_stat_cmplt, cmnd->device);
=======
						uas_stat_cmplt, shost);
>>>>>>> refs/remotes/origin/cm-10.0
=======
						uas_stat_cmplt, shost);
>>>>>>> refs/remotes/origin/master
	urb->stream_id = stream_id;
	urb->transfer_flags |= URB_FREE_BUFFER;
 out:
	return urb;
 free:
	usb_free_urb(urb);
	return NULL;
}

static struct urb *uas_alloc_cmd_urb(struct uas_dev_info *devinfo, gfp_t gfp,
					struct scsi_cmnd *cmnd, u16 stream_id)
{
	struct usb_device *udev = devinfo->udev;
	struct scsi_device *sdev = cmnd->device;
	struct urb *urb = usb_alloc_urb(0, gfp);
	struct command_iu *iu;
	int len;

	if (!urb)
		goto out;

	len = cmnd->cmd_len - 16;
	if (len < 0)
		len = 0;
	len = ALIGN(len, 4);
	iu = kzalloc(sizeof(*iu) + len, gfp);
	if (!iu)
		goto free;

	iu->iu_id = IU_ID_COMMAND;
<<<<<<< HEAD
<<<<<<< HEAD
	iu->tag = cpu_to_be16(stream_id);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (blk_rq_tagged(cmnd->request))
		iu->tag = cpu_to_be16(cmnd->request->tag + 2);
	else
		iu->tag = cpu_to_be16(1);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	iu->prio_attr = UAS_SIMPLE_TAG;
	iu->len = len;
	int_to_scsilun(sdev->lun, &iu->lun);
	memcpy(iu->cdb, cmnd->cmnd, cmnd->cmd_len);

	usb_fill_bulk_urb(urb, udev, devinfo->cmd_pipe, iu, sizeof(*iu) + len,
							usb_free_urb, NULL);
	urb->transfer_flags |= URB_FREE_BUFFER;
 out:
	return urb;
 free:
	usb_free_urb(urb);
	return NULL;
}

<<<<<<< HEAD
=======
static int uas_submit_task_urb(struct scsi_cmnd *cmnd, gfp_t gfp,
			       u8 function, u16 stream_id)
{
	struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;
	struct usb_device *udev = devinfo->udev;
	struct urb *urb = usb_alloc_urb(0, gfp);
	struct task_mgmt_iu *iu;
	int err = -ENOMEM;

	if (!urb)
		goto err;

	iu = kzalloc(sizeof(*iu), gfp);
	if (!iu)
		goto err;

	iu->iu_id = IU_ID_TASK_MGMT;
	iu->tag = cpu_to_be16(stream_id);
	int_to_scsilun(cmnd->device->lun, &iu->lun);

	iu->function = function;
	switch (function) {
	case TMF_ABORT_TASK:
		if (blk_rq_tagged(cmnd->request))
			iu->task_tag = cpu_to_be16(cmnd->request->tag + 2);
		else
			iu->task_tag = cpu_to_be16(1);
		break;
	}

	usb_fill_bulk_urb(urb, udev, devinfo->cmd_pipe, iu, sizeof(*iu),
			  usb_free_urb, NULL);
	urb->transfer_flags |= URB_FREE_BUFFER;

	err = usb_submit_urb(urb, gfp);
	if (err)
		goto err;
	usb_anchor_urb(urb, &devinfo->cmd_urbs);

	return 0;

err:
	usb_free_urb(urb);
	return err;
}

>>>>>>> refs/remotes/origin/master
/*
 * Why should I request the Status IU before sending the Command IU?  Spec
 * says to, but also says the device may receive them in any order.  Seems
 * daft to me.
 */

<<<<<<< HEAD
static int uas_submit_urbs(struct scsi_cmnd *cmnd,
					struct uas_dev_info *devinfo, gfp_t gfp)
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;

	if (cmdinfo->state & ALLOC_STATUS_URB) {
<<<<<<< HEAD
		cmdinfo->status_urb = uas_alloc_sense_urb(devinfo, gfp, cmnd,
							  cmdinfo->stream);
=======
		cmdinfo->status_urb = uas_alloc_sense_urb(devinfo, gfp,
				cmnd->device->host, cmdinfo->stream);
>>>>>>> refs/remotes/origin/cm-10.0
		if (!cmdinfo->status_urb)
			return SCSI_MLQUEUE_DEVICE_BUSY;
		cmdinfo->state &= ~ALLOC_STATUS_URB;
	}

	if (cmdinfo->state & SUBMIT_STATUS_URB) {
		if (usb_submit_urb(cmdinfo->status_urb, gfp)) {
			scmd_printk(KERN_INFO, cmnd,
					"sense urb submission failure\n");
			return SCSI_MLQUEUE_DEVICE_BUSY;
=======
static int uas_submit_sense_urb(struct Scsi_Host *shost,
				gfp_t gfp, unsigned int stream)
{
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];
	struct urb *urb;

	urb = uas_alloc_sense_urb(devinfo, gfp, shost, stream);
	if (!urb)
		return SCSI_MLQUEUE_DEVICE_BUSY;
	if (usb_submit_urb(urb, gfp)) {
		shost_printk(KERN_INFO, shost,
			     "sense urb submission failure\n");
		usb_free_urb(urb);
		return SCSI_MLQUEUE_DEVICE_BUSY;
	}
	usb_anchor_urb(urb, &devinfo->sense_urbs);
	return 0;
}

static int uas_submit_urbs(struct scsi_cmnd *cmnd,
			   struct uas_dev_info *devinfo, gfp_t gfp)
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	int err;

	WARN_ON(!spin_is_locked(&devinfo->lock));
	if (cmdinfo->state & SUBMIT_STATUS_URB) {
		err = uas_submit_sense_urb(cmnd->device->host, gfp,
					   cmdinfo->stream);
		if (err) {
			return err;
>>>>>>> refs/remotes/origin/master
		}
		cmdinfo->state &= ~SUBMIT_STATUS_URB;
	}

	if (cmdinfo->state & ALLOC_DATA_IN_URB) {
		cmdinfo->data_in_urb = uas_alloc_data_urb(devinfo, gfp,
					devinfo->data_in_pipe, cmdinfo->stream,
<<<<<<< HEAD
<<<<<<< HEAD
					scsi_in(cmnd), DMA_FROM_DEVICE);
=======
					cmnd, DMA_FROM_DEVICE);
>>>>>>> refs/remotes/origin/master
=======
					scsi_in(cmnd), DMA_FROM_DEVICE);
>>>>>>> refs/remotes/origin/cm-11.0
		if (!cmdinfo->data_in_urb)
			return SCSI_MLQUEUE_DEVICE_BUSY;
		cmdinfo->state &= ~ALLOC_DATA_IN_URB;
	}

	if (cmdinfo->state & SUBMIT_DATA_IN_URB) {
		if (usb_submit_urb(cmdinfo->data_in_urb, gfp)) {
			scmd_printk(KERN_INFO, cmnd,
					"data in urb submission failure\n");
			return SCSI_MLQUEUE_DEVICE_BUSY;
		}
		cmdinfo->state &= ~SUBMIT_DATA_IN_URB;
<<<<<<< HEAD
=======
		cmdinfo->state |= DATA_IN_URB_INFLIGHT;
		usb_anchor_urb(cmdinfo->data_in_urb, &devinfo->data_urbs);
>>>>>>> refs/remotes/origin/master
	}

	if (cmdinfo->state & ALLOC_DATA_OUT_URB) {
		cmdinfo->data_out_urb = uas_alloc_data_urb(devinfo, gfp,
					devinfo->data_out_pipe, cmdinfo->stream,
<<<<<<< HEAD
<<<<<<< HEAD
					scsi_out(cmnd), DMA_TO_DEVICE);
=======
					cmnd, DMA_TO_DEVICE);
>>>>>>> refs/remotes/origin/master
=======
					scsi_out(cmnd), DMA_TO_DEVICE);
>>>>>>> refs/remotes/origin/cm-11.0
		if (!cmdinfo->data_out_urb)
			return SCSI_MLQUEUE_DEVICE_BUSY;
		cmdinfo->state &= ~ALLOC_DATA_OUT_URB;
	}

	if (cmdinfo->state & SUBMIT_DATA_OUT_URB) {
		if (usb_submit_urb(cmdinfo->data_out_urb, gfp)) {
			scmd_printk(KERN_INFO, cmnd,
					"data out urb submission failure\n");
			return SCSI_MLQUEUE_DEVICE_BUSY;
		}
		cmdinfo->state &= ~SUBMIT_DATA_OUT_URB;
<<<<<<< HEAD
=======
		cmdinfo->state |= DATA_OUT_URB_INFLIGHT;
		usb_anchor_urb(cmdinfo->data_out_urb, &devinfo->data_urbs);
>>>>>>> refs/remotes/origin/master
	}

	if (cmdinfo->state & ALLOC_CMD_URB) {
		cmdinfo->cmd_urb = uas_alloc_cmd_urb(devinfo, gfp, cmnd,
<<<<<<< HEAD
							cmdinfo->stream);
=======
						     cmdinfo->stream);
>>>>>>> refs/remotes/origin/master
		if (!cmdinfo->cmd_urb)
			return SCSI_MLQUEUE_DEVICE_BUSY;
		cmdinfo->state &= ~ALLOC_CMD_URB;
	}

	if (cmdinfo->state & SUBMIT_CMD_URB) {
<<<<<<< HEAD
=======
		usb_get_urb(cmdinfo->cmd_urb);
>>>>>>> refs/remotes/origin/master
		if (usb_submit_urb(cmdinfo->cmd_urb, gfp)) {
			scmd_printk(KERN_INFO, cmnd,
					"cmd urb submission failure\n");
			return SCSI_MLQUEUE_DEVICE_BUSY;
		}
<<<<<<< HEAD
		cmdinfo->state &= ~SUBMIT_CMD_URB;
=======
		usb_anchor_urb(cmdinfo->cmd_urb, &devinfo->cmd_urbs);
		usb_put_urb(cmdinfo->cmd_urb);
		cmdinfo->cmd_urb = NULL;
		cmdinfo->state &= ~SUBMIT_CMD_URB;
		cmdinfo->state |= COMMAND_INFLIGHT;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

static int uas_queuecommand_lck(struct scsi_cmnd *cmnd,
					void (*done)(struct scsi_cmnd *))
{
	struct scsi_device *sdev = cmnd->device;
	struct uas_dev_info *devinfo = sdev->hostdata;
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> refs/remotes/origin/master
	int err;

	BUILD_BUG_ON(sizeof(struct uas_cmd_info) > sizeof(struct scsi_pointer));

<<<<<<< HEAD
<<<<<<< HEAD
	if (!cmdinfo->status_urb && sdev->current_cmnd)
		return SCSI_MLQUEUE_DEVICE_BUSY;

	if (blk_rq_tagged(cmnd->request)) {
		cmdinfo->stream = cmnd->request->tag + 1;
	} else {
		sdev->current_cmnd = cmnd;
=======
	if (devinfo->cmnd)
		return SCSI_MLQUEUE_DEVICE_BUSY;
=======
	if (devinfo->resetting) {
		cmnd->result = DID_ERROR << 16;
		cmnd->scsi_done(cmnd);
		return 0;
	}

	spin_lock_irqsave(&devinfo->lock, flags);
	if (devinfo->cmnd) {
		spin_unlock_irqrestore(&devinfo->lock, flags);
		return SCSI_MLQUEUE_DEVICE_BUSY;
	}
>>>>>>> refs/remotes/origin/master

	if (blk_rq_tagged(cmnd->request)) {
		cmdinfo->stream = cmnd->request->tag + 2;
	} else {
		devinfo->cmnd = cmnd;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		cmdinfo->stream = 1;
	}

	cmnd->scsi_done = done;

<<<<<<< HEAD
	cmdinfo->state = ALLOC_STATUS_URB | SUBMIT_STATUS_URB |
=======
	cmdinfo->state = SUBMIT_STATUS_URB |
>>>>>>> refs/remotes/origin/master
			ALLOC_CMD_URB | SUBMIT_CMD_URB;

	switch (cmnd->sc_data_direction) {
	case DMA_FROM_DEVICE:
		cmdinfo->state |= ALLOC_DATA_IN_URB | SUBMIT_DATA_IN_URB;
		break;
	case DMA_BIDIRECTIONAL:
		cmdinfo->state |= ALLOC_DATA_IN_URB | SUBMIT_DATA_IN_URB;
	case DMA_TO_DEVICE:
		cmdinfo->state |= ALLOC_DATA_OUT_URB | SUBMIT_DATA_OUT_URB;
	case DMA_NONE:
		break;
	}

	if (!devinfo->use_streams) {
<<<<<<< HEAD
<<<<<<< HEAD
		cmdinfo->state &= ~(SUBMIT_DATA_IN_URB | SUBMIT_DATA_OUT_URB);
=======
		cmdinfo->state &= ~(SUBMIT_DATA_IN_URB | SUBMIT_DATA_OUT_URB |
				ALLOC_STATUS_URB | SUBMIT_STATUS_URB);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cmdinfo->state &= ~(SUBMIT_DATA_IN_URB | SUBMIT_DATA_OUT_URB);
>>>>>>> refs/remotes/origin/master
		cmdinfo->stream = 0;
	}

	err = uas_submit_urbs(cmnd, devinfo, GFP_ATOMIC);
	if (err) {
		/* If we did nothing, give up now */
		if (cmdinfo->state & SUBMIT_STATUS_URB) {
<<<<<<< HEAD
			usb_free_urb(cmdinfo->status_urb);
=======
			spin_unlock_irqrestore(&devinfo->lock, flags);
>>>>>>> refs/remotes/origin/master
			return SCSI_MLQUEUE_DEVICE_BUSY;
		}
		spin_lock(&uas_work_lock);
		list_add_tail(&cmdinfo->list, &uas_work_list);
<<<<<<< HEAD
=======
		cmdinfo->state |= IS_IN_WORK_LIST;
>>>>>>> refs/remotes/origin/master
		spin_unlock(&uas_work_lock);
		schedule_work(&uas_work);
	}

<<<<<<< HEAD
=======
	spin_unlock_irqrestore(&devinfo->lock, flags);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static DEF_SCSI_QCMD(uas_queuecommand)

<<<<<<< HEAD
static int uas_eh_abort_handler(struct scsi_cmnd *cmnd)
{
	struct scsi_device *sdev = cmnd->device;
	sdev_printk(KERN_INFO, sdev, "%s tag %d\n", __func__,
							cmnd->request->tag);

/* XXX: Send ABORT TASK Task Management command */
	return FAILED;
}

static int uas_eh_device_reset_handler(struct scsi_cmnd *cmnd)
{
	struct scsi_device *sdev = cmnd->device;
	sdev_printk(KERN_INFO, sdev, "%s tag %d\n", __func__,
							cmnd->request->tag);

/* XXX: Send LOGICAL UNIT RESET Task Management command */
	return FAILED;
}

static int uas_eh_target_reset_handler(struct scsi_cmnd *cmnd)
{
	struct scsi_device *sdev = cmnd->device;
	sdev_printk(KERN_INFO, sdev, "%s tag %d\n", __func__,
							cmnd->request->tag);

/* XXX: Can we reset just the one USB interface?
 * Would calling usb_set_interface() have the right effect?
 */
	return FAILED;
=======
static int uas_eh_task_mgmt(struct scsi_cmnd *cmnd,
			    const char *fname, u8 function)
{
	struct Scsi_Host *shost = cmnd->device->host;
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];
	u16 tag = devinfo->qdepth - 1;
	unsigned long flags;

	spin_lock_irqsave(&devinfo->lock, flags);
	memset(&devinfo->response, 0, sizeof(devinfo->response));
	if (uas_submit_sense_urb(shost, GFP_ATOMIC, tag)) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s: submit sense urb failed\n",
			     __func__, fname);
		spin_unlock_irqrestore(&devinfo->lock, flags);
		return FAILED;
	}
	if (uas_submit_task_urb(cmnd, GFP_ATOMIC, function, tag)) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s: submit task mgmt urb failed\n",
			     __func__, fname);
		spin_unlock_irqrestore(&devinfo->lock, flags);
		return FAILED;
	}
	spin_unlock_irqrestore(&devinfo->lock, flags);

	if (usb_wait_anchor_empty_timeout(&devinfo->sense_urbs, 3000) == 0) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s timed out\n", __func__, fname);
		return FAILED;
	}
	if (be16_to_cpu(devinfo->response.tag) != tag) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s failed (wrong tag %d/%d)\n", __func__,
			     fname, be16_to_cpu(devinfo->response.tag), tag);
		return FAILED;
	}
	if (devinfo->response.response_code != RC_TMF_COMPLETE) {
		shost_printk(KERN_INFO, shost,
			     "%s: %s failed (rc 0x%x)\n", __func__,
			     fname, devinfo->response.response_code);
		return FAILED;
	}
	return SUCCESS;
}

static int uas_eh_abort_handler(struct scsi_cmnd *cmnd)
{
	struct uas_cmd_info *cmdinfo = (void *)&cmnd->SCp;
	struct uas_dev_info *devinfo = (void *)cmnd->device->hostdata;
	unsigned long flags;
	int ret;

	uas_log_cmd_state(cmnd, __func__);
	spin_lock_irqsave(&devinfo->lock, flags);
	cmdinfo->state |= COMMAND_ABORTED;
	if (cmdinfo->state & IS_IN_WORK_LIST) {
		spin_lock(&uas_work_lock);
		list_del(&cmdinfo->list);
		cmdinfo->state &= ~IS_IN_WORK_LIST;
		spin_unlock(&uas_work_lock);
	}
	if (cmdinfo->state & COMMAND_INFLIGHT) {
		spin_unlock_irqrestore(&devinfo->lock, flags);
		ret = uas_eh_task_mgmt(cmnd, "ABORT TASK", TMF_ABORT_TASK);
	} else {
		spin_unlock_irqrestore(&devinfo->lock, flags);
		uas_unlink_data_urbs(devinfo, cmdinfo);
		spin_lock_irqsave(&devinfo->lock, flags);
		uas_try_complete(cmnd, __func__);
		spin_unlock_irqrestore(&devinfo->lock, flags);
		ret = SUCCESS;
	}
	return ret;
}

static int uas_eh_device_reset_handler(struct scsi_cmnd *cmnd)
{
	sdev_printk(KERN_INFO, cmnd->device, "%s\n", __func__);
	return uas_eh_task_mgmt(cmnd, "LOGICAL UNIT RESET",
				TMF_LOGICAL_UNIT_RESET);
>>>>>>> refs/remotes/origin/master
}

static int uas_eh_bus_reset_handler(struct scsi_cmnd *cmnd)
{
	struct scsi_device *sdev = cmnd->device;
	struct uas_dev_info *devinfo = sdev->hostdata;
	struct usb_device *udev = devinfo->udev;
<<<<<<< HEAD

	sdev_printk(KERN_INFO, sdev, "%s tag %d\n", __func__,
							cmnd->request->tag);

	if (usb_reset_device(udev))
		return SUCCESS;

	return FAILED;
=======
	int err;

	devinfo->resetting = 1;
	uas_abort_work(devinfo);
	usb_kill_anchored_urbs(&devinfo->cmd_urbs);
	usb_kill_anchored_urbs(&devinfo->sense_urbs);
	usb_kill_anchored_urbs(&devinfo->data_urbs);
	err = usb_reset_device(udev);
	devinfo->resetting = 0;

	if (err) {
		shost_printk(KERN_INFO, sdev->host, "%s FAILED\n", __func__);
		return FAILED;
	}

	shost_printk(KERN_INFO, sdev->host, "%s success\n", __func__);
	return SUCCESS;
>>>>>>> refs/remotes/origin/master
}

static int uas_slave_alloc(struct scsi_device *sdev)
{
	sdev->hostdata = (void *)sdev->host->hostdata[0];
	return 0;
}

static int uas_slave_configure(struct scsi_device *sdev)
{
	struct uas_dev_info *devinfo = sdev->hostdata;
	scsi_set_tag_type(sdev, MSG_ORDERED_TAG);
<<<<<<< HEAD
<<<<<<< HEAD
	scsi_activate_tcq(sdev, devinfo->qdepth - 1);
=======
	scsi_activate_tcq(sdev, devinfo->qdepth - 2);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	scsi_activate_tcq(sdev, devinfo->qdepth - 3);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct scsi_host_template uas_host_template = {
	.module = THIS_MODULE,
	.name = "uas",
	.queuecommand = uas_queuecommand,
	.slave_alloc = uas_slave_alloc,
	.slave_configure = uas_slave_configure,
	.eh_abort_handler = uas_eh_abort_handler,
	.eh_device_reset_handler = uas_eh_device_reset_handler,
<<<<<<< HEAD
	.eh_target_reset_handler = uas_eh_target_reset_handler,
=======
>>>>>>> refs/remotes/origin/master
	.eh_bus_reset_handler = uas_eh_bus_reset_handler,
	.can_queue = 65536,	/* Is there a limit on the _host_ ? */
	.this_id = -1,
	.sg_tablesize = SG_NONE,
	.cmd_per_lun = 1,	/* until we override it */
	.skip_settle_delay = 1,
	.ordered_tag = 1,
};

static struct usb_device_id uas_usb_ids[] = {
	{ USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, USB_PR_BULK) },
	{ USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, USB_PR_UAS) },
	/* 0xaa is a prototype device I happen to have access to */
	{ USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, 0xaa) },
	{ }
};
MODULE_DEVICE_TABLE(usb, uas_usb_ids);

static int uas_is_interface(struct usb_host_interface *intf)
{
	return (intf->desc.bInterfaceClass == USB_CLASS_MASS_STORAGE &&
		intf->desc.bInterfaceSubClass == USB_SC_SCSI &&
		intf->desc.bInterfaceProtocol == USB_PR_UAS);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int uas_isnt_supported(struct usb_device *udev)
{
	struct usb_hcd *hcd = bus_to_hcd(udev->bus);

	dev_warn(&udev->dev, "The driver for the USB controller %s does not "
			"support scatter-gather which is\n",
			hcd->driver->description);
	dev_warn(&udev->dev, "required by the UAS driver. Please try an"
			"alternative USB controller if you wish to use UAS.\n");
	return -ENODEV;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int uas_switch_interface(struct usb_device *udev,
						struct usb_interface *intf)
{
	int i;
<<<<<<< HEAD
<<<<<<< HEAD

	if (uas_is_interface(intf->cur_altsetting))
		return 0;

	for (i = 0; i < intf->num_altsetting; i++) {
		struct usb_host_interface *alt = &intf->altsetting[i];
		if (alt == intf->cur_altsetting)
			continue;
		if (uas_is_interface(alt))
			return usb_set_interface(udev,
						alt->desc.bInterfaceNumber,
						alt->desc.bAlternateSetting);
=======
=======
>>>>>>> refs/remotes/origin/master
	int sg_supported = udev->bus->sg_tablesize != 0;

	for (i = 0; i < intf->num_altsetting; i++) {
		struct usb_host_interface *alt = &intf->altsetting[i];

		if (uas_is_interface(alt)) {
			if (!sg_supported)
				return uas_isnt_supported(udev);
			return usb_set_interface(udev,
						alt->desc.bInterfaceNumber,
						alt->desc.bAlternateSetting);
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return -ENODEV;
}

static void uas_configure_endpoints(struct uas_dev_info *devinfo)
{
	struct usb_host_endpoint *eps[4] = { };
	struct usb_interface *intf = devinfo->intf;
	struct usb_device *udev = devinfo->udev;
	struct usb_host_endpoint *endpoint = intf->cur_altsetting->endpoint;
	unsigned i, n_endpoints = intf->cur_altsetting->desc.bNumEndpoints;

	devinfo->uas_sense_old = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	devinfo->cmnd = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	devinfo->cmnd = NULL;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < n_endpoints; i++) {
		unsigned char *extra = endpoint[i].extra;
		int len = endpoint[i].extralen;
		while (len > 1) {
			if (extra[1] == USB_DT_PIPE_USAGE) {
				unsigned pipe_id = extra[2];
				if (pipe_id > 0 && pipe_id < 5)
					eps[pipe_id - 1] = &endpoint[i];
				break;
			}
			len -= extra[0];
			extra += extra[0];
		}
	}

	/*
	 * Assume that if we didn't find a control pipe descriptor, we're
	 * using a device with old firmware that happens to be set up like
	 * this.
	 */
	if (!eps[0]) {
		devinfo->cmd_pipe = usb_sndbulkpipe(udev, 1);
		devinfo->status_pipe = usb_rcvbulkpipe(udev, 1);
		devinfo->data_in_pipe = usb_rcvbulkpipe(udev, 2);
		devinfo->data_out_pipe = usb_sndbulkpipe(udev, 2);

		eps[1] = usb_pipe_endpoint(udev, devinfo->status_pipe);
		eps[2] = usb_pipe_endpoint(udev, devinfo->data_in_pipe);
		eps[3] = usb_pipe_endpoint(udev, devinfo->data_out_pipe);
	} else {
		devinfo->cmd_pipe = usb_sndbulkpipe(udev,
						eps[0]->desc.bEndpointAddress);
		devinfo->status_pipe = usb_rcvbulkpipe(udev,
						eps[1]->desc.bEndpointAddress);
		devinfo->data_in_pipe = usb_rcvbulkpipe(udev,
						eps[2]->desc.bEndpointAddress);
		devinfo->data_out_pipe = usb_sndbulkpipe(udev,
						eps[3]->desc.bEndpointAddress);
	}

	devinfo->qdepth = usb_alloc_streams(devinfo->intf, eps + 1, 3, 256,
								GFP_KERNEL);
	if (devinfo->qdepth < 0) {
		devinfo->qdepth = 256;
		devinfo->use_streams = 0;
	} else {
		devinfo->use_streams = 1;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int uas_alloc_status_urb(struct uas_dev_info *devinfo,
		struct Scsi_Host *shost)
{
	if (devinfo->use_streams) {
		devinfo->status_urb = NULL;
		return 0;
	}

	devinfo->status_urb = uas_alloc_sense_urb(devinfo, GFP_KERNEL,
			shost, 0);
	if (!devinfo->status_urb)
		goto err_s_urb;

	if (usb_submit_urb(devinfo->status_urb, GFP_KERNEL))
		goto err_submit_urb;

	return 0;
err_submit_urb:
	usb_free_urb(devinfo->status_urb);
err_s_urb:
	return -ENOMEM;
}

=======
>>>>>>> refs/remotes/origin/master
static void uas_free_streams(struct uas_dev_info *devinfo)
{
	struct usb_device *udev = devinfo->udev;
	struct usb_host_endpoint *eps[3];

	eps[0] = usb_pipe_endpoint(udev, devinfo->status_pipe);
	eps[1] = usb_pipe_endpoint(udev, devinfo->data_in_pipe);
	eps[2] = usb_pipe_endpoint(udev, devinfo->data_out_pipe);
	usb_free_streams(devinfo->intf, eps, 3, GFP_KERNEL);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * XXX: What I'd like to do here is register a SCSI host for each USB host in
 * the system.  Follow usb-storage's design of registering a SCSI host for
 * each USB device for the moment.  Can implement this by walking up the
 * USB hierarchy until we find a USB host.
 */
static int uas_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	int result;
	struct Scsi_Host *shost;
	struct uas_dev_info *devinfo;
	struct usb_device *udev = interface_to_usbdev(intf);

	if (uas_switch_interface(udev, intf))
		return -ENODEV;

	devinfo = kmalloc(sizeof(struct uas_dev_info), GFP_KERNEL);
	if (!devinfo)
		return -ENOMEM;

	result = -ENOMEM;
	shost = scsi_host_alloc(&uas_host_template, sizeof(void *));
	if (!shost)
		goto free;

	shost->max_cmd_len = 16 + 252;
	shost->max_id = 1;
<<<<<<< HEAD
	shost->sg_tablesize = udev->bus->sg_tablesize;

<<<<<<< HEAD
	result = scsi_add_host(shost, &intf->dev);
	if (result)
		goto free;
	shost->hostdata[0] = (unsigned long)devinfo;

	devinfo->intf = intf;
	devinfo->udev = udev;
	uas_configure_endpoints(devinfo);
=======
	devinfo->intf = intf;
	devinfo->udev = udev;
	uas_configure_endpoints(devinfo);

	result = scsi_init_shared_tag_map(shost, devinfo->qdepth - 2);
=======
	shost->max_lun = 256;
	shost->max_channel = 0;
	shost->sg_tablesize = udev->bus->sg_tablesize;

	devinfo->intf = intf;
	devinfo->udev = udev;
	devinfo->resetting = 0;
	init_usb_anchor(&devinfo->cmd_urbs);
	init_usb_anchor(&devinfo->sense_urbs);
	init_usb_anchor(&devinfo->data_urbs);
	spin_lock_init(&devinfo->lock);
	uas_configure_endpoints(devinfo);

	result = scsi_init_shared_tag_map(shost, devinfo->qdepth - 3);
>>>>>>> refs/remotes/origin/master
	if (result)
		goto free;

	result = scsi_add_host(shost, &intf->dev);
	if (result)
		goto deconfig_eps;

	shost->hostdata[0] = (unsigned long)devinfo;

<<<<<<< HEAD
	result = uas_alloc_status_urb(devinfo, shost);
	if (result)
		goto err_alloc_status;
>>>>>>> refs/remotes/origin/cm-10.0

	scsi_scan_host(shost);
	usb_set_intfdata(intf, shost);
	return result;
<<<<<<< HEAD
=======

err_alloc_status:
	scsi_remove_host(shost);
	shost = NULL;
deconfig_eps:
	uas_free_streams(devinfo);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	scsi_scan_host(shost);
	usb_set_intfdata(intf, shost);
	return result;

deconfig_eps:
	uas_free_streams(devinfo);
>>>>>>> refs/remotes/origin/master
 free:
	kfree(devinfo);
	if (shost)
		scsi_host_put(shost);
	return result;
}

static int uas_pre_reset(struct usb_interface *intf)
{
/* XXX: Need to return 1 if it's not our device in error handling */
	return 0;
}

static int uas_post_reset(struct usb_interface *intf)
{
/* XXX: Need to return 1 if it's not our device in error handling */
	return 0;
}

static void uas_disconnect(struct usb_interface *intf)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct usb_device *udev = interface_to_usbdev(intf);
	struct usb_host_endpoint *eps[3];
=======
>>>>>>> refs/remotes/origin/cm-10.0
	struct Scsi_Host *shost = usb_get_intfdata(intf);
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];

	scsi_remove_host(shost);
<<<<<<< HEAD

	eps[0] = usb_pipe_endpoint(udev, devinfo->status_pipe);
	eps[1] = usb_pipe_endpoint(udev, devinfo->data_in_pipe);
	eps[2] = usb_pipe_endpoint(udev, devinfo->data_out_pipe);
	usb_free_streams(intf, eps, 3, GFP_KERNEL);

=======
	usb_kill_urb(devinfo->status_urb);
	usb_free_urb(devinfo->status_urb);
	uas_free_streams(devinfo);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct Scsi_Host *shost = usb_get_intfdata(intf);
	struct uas_dev_info *devinfo = (void *)shost->hostdata[0];

	devinfo->resetting = 1;
	uas_abort_work(devinfo);
	usb_kill_anchored_urbs(&devinfo->cmd_urbs);
	usb_kill_anchored_urbs(&devinfo->sense_urbs);
	usb_kill_anchored_urbs(&devinfo->data_urbs);
	scsi_remove_host(shost);
	uas_free_streams(devinfo);
>>>>>>> refs/remotes/origin/master
	kfree(devinfo);
}

/*
 * XXX: Should this plug into libusual so we can auto-upgrade devices from
 * Bulk-Only to UAS?
 */
static struct usb_driver uas_driver = {
	.name = "uas",
	.probe = uas_probe,
	.disconnect = uas_disconnect,
	.pre_reset = uas_pre_reset,
	.post_reset = uas_post_reset,
	.id_table = uas_usb_ids,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int uas_init(void)
{
	return usb_register(&uas_driver);
}

static void uas_exit(void)
{
	usb_deregister(&uas_driver);
}

module_init(uas_init);
module_exit(uas_exit);
=======
module_usb_driver(uas_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_usb_driver(uas_driver);
>>>>>>> refs/remotes/origin/master

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthew Wilcox and Sarah Sharp");
