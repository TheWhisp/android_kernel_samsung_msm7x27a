#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/slab.h>

#include <scsi/scsi.h>
#include <scsi/scsi_eh.h>
#include <scsi/scsi_device.h>

#include "usb.h"
#include "scsiglue.h"
#include "transport.h"
#include "init.h"

/*
 * ENE_InitMedia():
 */
int ENE_InitMedia(struct us_data *us)
{
	int	result;
	BYTE	MiscReg03 = 0;

<<<<<<< HEAD
	printk(KERN_INFO "--- Init Media ---\n");
	result = ENE_Read_BYTE(us, REG_CARD_STATUS, &MiscReg03);
	if (result != USB_STOR_XFER_GOOD) {
		printk(KERN_ERR "Read register fail !!\n");
		return USB_STOR_TRANSPORT_ERROR;
	}
	printk(KERN_INFO "MiscReg03 = %x\n", MiscReg03);
=======
	dev_info(&us->pusb_dev->dev, "--- Init Media ---\n");
	result = ene_read_byte(us, REG_CARD_STATUS, &MiscReg03);
	if (result != USB_STOR_XFER_GOOD) {
		dev_err(&us->pusb_dev->dev, "Failed to read register\n");
		return USB_STOR_TRANSPORT_ERROR;
	}
	dev_info(&us->pusb_dev->dev, "MiscReg03 = %x\n", MiscReg03);
>>>>>>> refs/remotes/origin/master

	if (MiscReg03 & 0x02) {
		if (!us->SM_Status.Ready && !us->MS_Status.Ready) {
			result = ENE_SMInit(us);
<<<<<<< HEAD
			if (result != USB_STOR_XFER_GOOD) {
<<<<<<< HEAD
				result = ENE_MSInit(us);
				if (result != USB_STOR_XFER_GOOD)
					return USB_STOR_TRANSPORT_ERROR;
=======
				return USB_STOR_TRANSPORT_ERROR;
>>>>>>> refs/remotes/origin/cm-10.0
			}
=======
			if (result != USB_STOR_XFER_GOOD)
				return USB_STOR_TRANSPORT_ERROR;
>>>>>>> refs/remotes/origin/master
		}

	}
	return result;
}

/*
<<<<<<< HEAD
 * ENE_Read_BYTE() :
 */
int ENE_Read_BYTE(struct us_data *us, WORD index, void *buf)
=======
 * ene_read_byte() :
 */
int ene_read_byte(struct us_data *us, WORD index, void *buf)
>>>>>>> refs/remotes/origin/master
{
	struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *) us->iobuf;
	int result;

	memset(bcb, 0, sizeof(struct bulk_cb_wrap));
	bcb->Signature = cpu_to_le32(US_BULK_CB_SIGN);
	bcb->DataTransferLength	= 0x01;
	bcb->Flags			= 0x80;
	bcb->CDB[0]			= 0xED;
	bcb->CDB[2]			= (BYTE)(index>>8);
	bcb->CDB[3]			= (BYTE)index;

	result = ENE_SendScsiCmd(us, FDIR_READ, buf, 0);
	return result;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * ENE_MSInit():
 */
int ENE_MSInit(struct us_data *us)
{
	struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *) us->iobuf;
	int	result;
	BYTE	buf[0x200];
	WORD	MSP_BlockSize, MSP_UserAreaBlocks;

	printk(KERN_INFO "transport --- ENE_MSInit\n");
	result = ENE_LoadBinCode(us, MS_INIT_PATTERN);
	if (result != USB_STOR_XFER_GOOD) {
		printk(KERN_ERR "Load MS Init Code Fail !!\n");
		return USB_STOR_TRANSPORT_ERROR;
	}

	memset(bcb, 0, sizeof(struct bulk_cb_wrap));
	bcb->Signature = cpu_to_le32(US_BULK_CB_SIGN);
	bcb->DataTransferLength	= 0x200;
	bcb->Flags			= 0x80;
	bcb->CDB[0]			= 0xF1;
	bcb->CDB[1]			= 0x01;

	result = ENE_SendScsiCmd(us, FDIR_READ, &buf, 0);
	if (result != USB_STOR_XFER_GOOD) {
		printk(KERN_ERR "Execution MS Init Code Fail !!\n");
		return USB_STOR_TRANSPORT_ERROR;
	}

	us->MS_Status = *(PMS_STATUS)&buf[0];

	if (us->MS_Status.Insert && us->MS_Status.Ready) {
		printk(KERN_INFO "Insert     = %x\n", us->MS_Status.Insert);
		printk(KERN_INFO "Ready      = %x\n", us->MS_Status.Ready);
		printk(KERN_INFO "IsMSPro    = %x\n", us->MS_Status.IsMSPro);
		printk(KERN_INFO "IsMSPHG    = %x\n", us->MS_Status.IsMSPHG);
		printk(KERN_INFO "WtP        = %x\n", us->MS_Status.WtP);
		if (us->MS_Status.IsMSPro) {
			MSP_BlockSize      = (buf[6] << 8) | buf[7];
			MSP_UserAreaBlocks = (buf[10] << 8) | buf[11];
			us->MSP_TotalBlock = MSP_BlockSize * MSP_UserAreaBlocks;
		} else {
			MS_CardInit(us);
		}
		printk(KERN_INFO "MS Init Code OK !!\n");
	} else {
		printk(KERN_INFO "MS Card Not Ready --- %x\n", buf[0]);
		return USB_STOR_TRANSPORT_ERROR;
	}

	return USB_STOR_TRANSPORT_GOOD;
}

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *ENE_SMInit()
 */
int ENE_SMInit(struct us_data *us)
{
	struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *) us->iobuf;
	int	result;
	BYTE	buf[0x200];

<<<<<<< HEAD
	printk(KERN_INFO "transport --- ENE_SMInit\n");

	result = ENE_LoadBinCode(us, SM_INIT_PATTERN);
	if (result != USB_STOR_XFER_GOOD) {
		printk(KERN_INFO "Load SM Init Code Fail !!\n");
=======
	dev_dbg(&us->pusb_dev->dev, "transport --- ENE_SMInit\n");

	result = ENE_LoadBinCode(us, SM_INIT_PATTERN);
	if (result != USB_STOR_XFER_GOOD) {
		dev_info(&us->pusb_dev->dev,
			 "Failed to load SmartMedia init code\n: result= %x\n",
			 result);
>>>>>>> refs/remotes/origin/master
		return USB_STOR_TRANSPORT_ERROR;
	}

	memset(bcb, 0, sizeof(struct bulk_cb_wrap));
	bcb->Signature = cpu_to_le32(US_BULK_CB_SIGN);
	bcb->DataTransferLength	= 0x200;
	bcb->Flags			= 0x80;
	bcb->CDB[0]			= 0xF1;
	bcb->CDB[1]			= 0x01;

	result = ENE_SendScsiCmd(us, FDIR_READ, &buf, 0);
	if (result != USB_STOR_XFER_GOOD) {
<<<<<<< HEAD
		printk(KERN_ERR
		       "Execution SM Init Code Fail !! result = %x\n", result);
		return USB_STOR_TRANSPORT_ERROR;
	}

	us->SM_Status = *(PSM_STATUS)&buf[0];
=======
		dev_err(&us->pusb_dev->dev,
			"Failed to load SmartMedia init code: result = %x\n",
			result);
		return USB_STOR_TRANSPORT_ERROR;
	}

	us->SM_Status = *(struct keucr_sm_status *)&buf[0];
>>>>>>> refs/remotes/origin/master

	us->SM_DeviceID = buf[1];
	us->SM_CardID   = buf[2];

	if (us->SM_Status.Insert && us->SM_Status.Ready) {
<<<<<<< HEAD
		printk(KERN_INFO "Insert     = %x\n", us->SM_Status.Insert);
		printk(KERN_INFO "Ready      = %x\n", us->SM_Status.Ready);
		printk(KERN_INFO "WtP        = %x\n", us->SM_Status.WtP);
		printk(KERN_INFO "DeviceID   = %x\n", us->SM_DeviceID);
		printk(KERN_INFO "CardID     = %x\n", us->SM_CardID);
		MediaChange = 1;
		Check_D_MediaFmt(us);
	} else {
		printk(KERN_ERR "SM Card Not Ready --- %x\n", buf[0]);
=======
		dev_info(&us->pusb_dev->dev, "Insert     = %x\n",
					     us->SM_Status.Insert);
		dev_info(&us->pusb_dev->dev, "Ready      = %x\n",
					     us->SM_Status.Ready);
		dev_info(&us->pusb_dev->dev, "WtP        = %x\n",
					     us->SM_Status.WtP);
		dev_info(&us->pusb_dev->dev, "DeviceID   = %x\n",
					     us->SM_DeviceID);
		dev_info(&us->pusb_dev->dev, "CardID     = %x\n",
					     us->SM_CardID);
		MediaChange = 1;
		Check_D_MediaFmt(us);
	} else {
		dev_err(&us->pusb_dev->dev,
			"SmartMedia Card Not Ready --- %x\n", buf[0]);
>>>>>>> refs/remotes/origin/master
		return USB_STOR_TRANSPORT_ERROR;
	}

	return USB_STOR_TRANSPORT_GOOD;
}

/*
 * ENE_LoadBinCode()
 */
int ENE_LoadBinCode(struct us_data *us, BYTE flag)
{
	struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *) us->iobuf;
	int result;
	/* void *buf; */
	PBYTE buf;

<<<<<<< HEAD
	/* printk(KERN_INFO "transport --- ENE_LoadBinCode\n"); */
=======
	/* dev_info(&us->pusb_dev->dev, "transport --- ENE_LoadBinCode\n"); */
>>>>>>> refs/remotes/origin/master
	if (us->BIN_FLAG == flag)
		return USB_STOR_TRANSPORT_GOOD;

	buf = kmalloc(0x800, GFP_KERNEL);
	if (buf == NULL)
		return USB_STOR_TRANSPORT_ERROR;
	switch (flag) {
<<<<<<< HEAD
<<<<<<< HEAD
	/* For MS */
	case MS_INIT_PATTERN:
		printk(KERN_INFO "MS_INIT_PATTERN\n");
		memcpy(buf, MS_Init, 0x800);
		break;
	case MSP_RW_PATTERN:
		printk(KERN_INFO "MSP_RW_PATTERN\n");
		memcpy(buf, MSP_Rdwr, 0x800);
		break;
	case MS_RW_PATTERN:
		printk(KERN_INFO "MS_RW_PATTERN\n");
		memcpy(buf, MS_Rdwr, 0x800);
		break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* For SS */
	case SM_INIT_PATTERN:
		printk(KERN_INFO "SM_INIT_PATTERN\n");
		memcpy(buf, SM_Init, 0x800);
		break;
	case SM_RW_PATTERN:
		printk(KERN_INFO "SM_RW_PATTERN\n");
=======
	/* For SS */
	case SM_INIT_PATTERN:
		dev_dbg(&us->pusb_dev->dev, "SM_INIT_PATTERN\n");
		memcpy(buf, SM_Init, 0x800);
		break;
	case SM_RW_PATTERN:
		dev_dbg(&us->pusb_dev->dev, "SM_RW_PATTERN\n");
>>>>>>> refs/remotes/origin/master
		memcpy(buf, SM_Rdwr, 0x800);
		break;
	}

	memset(bcb, 0, sizeof(struct bulk_cb_wrap));
	bcb->Signature = cpu_to_le32(US_BULK_CB_SIGN);
	bcb->DataTransferLength = 0x800;
	bcb->Flags = 0x00;
	bcb->CDB[0] = 0xEF;

	result = ENE_SendScsiCmd(us, FDIR_WRITE, buf, 0);

	kfree(buf);
	us->BIN_FLAG = flag;
	return result;
}

/*
 * ENE_SendScsiCmd():
 */
int ENE_SendScsiCmd(struct us_data *us, BYTE fDir, void *buf, int use_sg)
{
	struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *) us->iobuf;
	struct bulk_cs_wrap *bcs = (struct bulk_cs_wrap *) us->iobuf;

	int result;
	unsigned int transfer_length = bcb->DataTransferLength,
		     cswlen = 0, partial = 0;
	unsigned int residue;

<<<<<<< HEAD
	/* printk(KERN_INFO "transport --- ENE_SendScsiCmd\n"); */
=======
	/* dev_dbg(&us->pusb_dev->dev, "transport --- ENE_SendScsiCmd\n"); */
>>>>>>> refs/remotes/origin/master
	/* send cmd to out endpoint */
	result = usb_stor_bulk_transfer_buf(us, us->send_bulk_pipe,
					    bcb, US_BULK_CB_WRAP_LEN, NULL);
	if (result != USB_STOR_XFER_GOOD) {
<<<<<<< HEAD
		printk(KERN_ERR "send cmd to out endpoint fail ---\n");
=======
		dev_err(&us->pusb_dev->dev,
				"send cmd to out endpoint fail ---\n");
>>>>>>> refs/remotes/origin/master
		return USB_STOR_TRANSPORT_ERROR;
	}

	if (buf) {
		unsigned int pipe = fDir;

		if (fDir == FDIR_READ)
			pipe = us->recv_bulk_pipe;
		else
			pipe = us->send_bulk_pipe;

		/* Bulk */
		if (use_sg)
			result = usb_stor_bulk_srb(us, pipe, us->srb);
		else
			result = usb_stor_bulk_transfer_sg(us, pipe, buf,
						transfer_length, 0, &partial);
		if (result != USB_STOR_XFER_GOOD) {
<<<<<<< HEAD
			printk(KERN_ERR "data transfer fail ---\n");
=======
			dev_err(&us->pusb_dev->dev, "data transfer fail ---\n");
>>>>>>> refs/remotes/origin/master
			return USB_STOR_TRANSPORT_ERROR;
		}
	}

	/* Get CSW for device status */
	result = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe, bcs,
						US_BULK_CS_WRAP_LEN, &cswlen);

	if (result == USB_STOR_XFER_SHORT && cswlen == 0) {
<<<<<<< HEAD
		printk(KERN_WARNING "Received 0-length CSW; retrying...\n");
=======
		dev_warn(&us->pusb_dev->dev,
				"Received 0-length CSW; retrying...\n");
>>>>>>> refs/remotes/origin/master
		result = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe,
					bcs, US_BULK_CS_WRAP_LEN, &cswlen);
	}

	if (result == USB_STOR_XFER_STALLED) {
		/* get the status again */
<<<<<<< HEAD
		printk(KERN_WARNING "Attempting to get CSW (2nd try)...\n");
=======
		dev_warn(&us->pusb_dev->dev,
				"Attempting to get CSW (2nd try)...\n");
>>>>>>> refs/remotes/origin/master
		result = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe,
						bcs, US_BULK_CS_WRAP_LEN, NULL);
	}

	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;

	/* check bulk status */
	residue = le32_to_cpu(bcs->Residue);

	/*
	 * try to compute the actual residue, based on how much data
	 * was really transferred and what the device tells us
	 */
	if (residue && !(us->fflags & US_FL_IGNORE_RESIDUE)) {
		residue = min(residue, transfer_length);
		if (us->srb)
			scsi_set_resid(us->srb, max(scsi_get_resid(us->srb),
					(int) residue));
	}

	if (bcs->Status != US_BULK_STAT_OK)
		return USB_STOR_TRANSPORT_ERROR;

	return USB_STOR_TRANSPORT_GOOD;
}

/*
 * ENE_Read_Data()
 */
int ENE_Read_Data(struct us_data *us, void *buf, unsigned int length)
{
	struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *) us->iobuf;
	struct bulk_cs_wrap *bcs = (struct bulk_cs_wrap *) us->iobuf;
	int result;

<<<<<<< HEAD
	/* printk(KERN_INFO "transport --- ENE_Read_Data\n"); */
=======
	/* dev_dbg(&us->pusb_dev->dev, "transport --- ENE_Read_Data\n"); */
>>>>>>> refs/remotes/origin/master
	/* set up the command wrapper */
	memset(bcb, 0, sizeof(struct bulk_cb_wrap));
	bcb->Signature = cpu_to_le32(US_BULK_CB_SIGN);
	bcb->DataTransferLength = length;
	bcb->Flags = 0x80;
	bcb->CDB[0] = 0xED;
	bcb->CDB[2] = 0xFF;
	bcb->CDB[3] = 0x81;

	/* send cmd to out endpoint */
	result = usb_stor_bulk_transfer_buf(us, us->send_bulk_pipe, bcb,
						US_BULK_CB_WRAP_LEN, NULL);
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;

	/* R/W data */
	result = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe,
						buf, length, NULL);
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;

	/* Get CSW for device status */
	result = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe, bcs,
						US_BULK_CS_WRAP_LEN, NULL);
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;
	if (bcs->Status != US_BULK_STAT_OK)
		return USB_STOR_TRANSPORT_ERROR;

	return USB_STOR_TRANSPORT_GOOD;
}

/*
 * ENE_Write_Data():
 */
int ENE_Write_Data(struct us_data *us, void *buf, unsigned int length)
{
	struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *) us->iobuf;
	struct bulk_cs_wrap *bcs = (struct bulk_cs_wrap *) us->iobuf;
	int result;

	/* printk("transport --- ENE_Write_Data\n"); */
	/* set up the command wrapper */
	memset(bcb, 0, sizeof(struct bulk_cb_wrap));
	bcb->Signature = cpu_to_le32(US_BULK_CB_SIGN);
	bcb->DataTransferLength = length;
	bcb->Flags = 0x00;
	bcb->CDB[0] = 0xEE;
	bcb->CDB[2] = 0xFF;
	bcb->CDB[3] = 0x81;

	/* send cmd to out endpoint */
	result = usb_stor_bulk_transfer_buf(us, us->send_bulk_pipe, bcb,
						US_BULK_CB_WRAP_LEN, NULL);
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;

	/* R/W data */
	result = usb_stor_bulk_transfer_buf(us, us->send_bulk_pipe,
						buf, length, NULL);
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;

	/* Get CSW for device status */
	result = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe, bcs,
						US_BULK_CS_WRAP_LEN, NULL);
	if (result != USB_STOR_XFER_GOOD)
		return USB_STOR_TRANSPORT_ERROR;
	if (bcs->Status != US_BULK_STAT_OK)
		return USB_STOR_TRANSPORT_ERROR;

	return USB_STOR_TRANSPORT_GOOD;
}

<<<<<<< HEAD
/*
 * usb_stor_print_cmd():
 */
void usb_stor_print_cmd(struct scsi_cmnd *srb)
{
	PBYTE	Cdb = srb->cmnd;
	DWORD	cmd = Cdb[0];
	DWORD	bn  =	((Cdb[2] << 24) & 0xff000000) |
			((Cdb[3] << 16) & 0x00ff0000) |
			((Cdb[4] << 8) & 0x0000ff00) |
			((Cdb[5] << 0) & 0x000000ff);
	WORD	blen = ((Cdb[7] << 8) & 0xff00) | ((Cdb[8] << 0) & 0x00ff);

	switch (cmd) {
	case TEST_UNIT_READY:
		/* printk(KERN_INFO
			 "scsi cmd %X --- SCSIOP_TEST_UNIT_READY\n", cmd); */
		break;
	case INQUIRY:
		printk(KERN_INFO "scsi cmd %X --- SCSIOP_INQUIRY\n", cmd);
		break;
	case MODE_SENSE:
		printk(KERN_INFO "scsi cmd %X --- SCSIOP_MODE_SENSE\n", cmd);
		break;
	case START_STOP:
		printk(KERN_INFO "scsi cmd %X --- SCSIOP_START_STOP\n", cmd);
		break;
	case READ_CAPACITY:
		printk(KERN_INFO "scsi cmd %X --- SCSIOP_READ_CAPACITY\n", cmd);
		break;
	case READ_10:
		/*  printk(KERN_INFO
			   "scsi cmd %X --- SCSIOP_READ,bn = %X, blen = %X\n"
			   ,cmd, bn, blen); */
		break;
	case WRITE_10:
		/* printk(KERN_INFO
			  "scsi cmd %X --- SCSIOP_WRITE,
			  bn = %X, blen = %X\n" , cmd, bn, blen); */
		break;
	case ALLOW_MEDIUM_REMOVAL:
		printk(KERN_INFO
			"scsi cmd %X --- SCSIOP_ALLOW_MEDIUM_REMOVAL\n", cmd);
		break;
	default:
		printk(KERN_INFO "scsi cmd %X --- Other cmd\n", cmd);
		break;
	}
	bn = 0;
	blen = 0;
}

=======
>>>>>>> refs/remotes/origin/master
