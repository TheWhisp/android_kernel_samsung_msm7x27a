#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/slab.h>

#include <scsi/scsi.h>
#include <scsi/scsi_eh.h>
#include <scsi/scsi_device.h>

#include "usb.h"
#include "scsiglue.h"
#include "transport.h"
<<<<<<< HEAD
<<<<<<< HEAD
//#include "smcommon.h"
#include "smil.h"

int SM_SCSI_Test_Unit_Ready (struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Inquiry         (struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Mode_Sense      (struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Start_Stop      (struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Read_Capacity   (struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Read            (struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Write           (struct us_data *us, struct scsi_cmnd *srb);
=======
#include "smil.h"

int SM_SCSI_Test_Unit_Ready(struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Inquiry(struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Mode_Sense(struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Start_Stop(struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Read_Capacity(struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Read(struct us_data *us, struct scsi_cmnd *srb);
int SM_SCSI_Write(struct us_data *us, struct scsi_cmnd *srb);
>>>>>>> refs/remotes/origin/cm-10.0

extern PBYTE                SMHostAddr;
extern DWORD                ErrXDCode;

<<<<<<< HEAD
//----- SM_SCSIIrp() --------------------------------------------------
=======
/* ----- SM_SCSIIrp() -------------------------------------------------- */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "smil.h"

static int SM_SCSI_Test_Unit_Ready(struct us_data *us, struct scsi_cmnd *srb);
static int SM_SCSI_Inquiry(struct us_data *us, struct scsi_cmnd *srb);
static int SM_SCSI_Mode_Sense(struct us_data *us, struct scsi_cmnd *srb);
static int SM_SCSI_Read_Capacity(struct us_data *us, struct scsi_cmnd *srb);
static int SM_SCSI_Read(struct us_data *us, struct scsi_cmnd *srb);
static int SM_SCSI_Write(struct us_data *us, struct scsi_cmnd *srb);

/* ----- SM_SCSIIrp() -------------------------------------------------- */
>>>>>>> refs/remotes/origin/master
int SM_SCSIIrp(struct us_data *us, struct scsi_cmnd *srb)
{
	int    result;

	us->SrbStatus = SS_SUCCESS;
<<<<<<< HEAD
<<<<<<< HEAD
	switch (srb->cmnd[0])
	{
		case TEST_UNIT_READY :  result = SM_SCSI_Test_Unit_Ready (us, srb);  break; //0x00
		case INQUIRY         :  result = SM_SCSI_Inquiry         (us, srb);  break; //0x12
		case MODE_SENSE      :  result = SM_SCSI_Mode_Sense      (us, srb);  break; //0x1A
		case READ_CAPACITY   :  result = SM_SCSI_Read_Capacity   (us, srb);  break; //0x25
		case READ_10         :  result = SM_SCSI_Read            (us, srb);  break; //0x28
		case WRITE_10        :  result = SM_SCSI_Write           (us, srb);  break; //0x2A

		default:
=======
=======
>>>>>>> refs/remotes/origin/master
	switch (srb->cmnd[0]) {
	case TEST_UNIT_READY:
		result = SM_SCSI_Test_Unit_Ready(us, srb);
		break;		/* 0x00 */
	case INQUIRY:
		result = SM_SCSI_Inquiry(us, srb);
		break;		/* 0x12 */
	case MODE_SENSE:
		result = SM_SCSI_Mode_Sense(us, srb);
		break;		/* 0x1A */
	case READ_CAPACITY:
		result = SM_SCSI_Read_Capacity(us, srb);
		break;		/* 0x25 */
	case READ_10:
		result = SM_SCSI_Read(us, srb);
		break;		/* 0x28 */
	case WRITE_10:
		result = SM_SCSI_Write(us, srb);
		break;		/* 0x2A */

	default:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			us->SrbStatus = SS_ILLEGAL_REQUEST;
			result = USB_STOR_TRANSPORT_FAILED;
			break;
	}
	return result;
}

<<<<<<< HEAD
<<<<<<< HEAD
//----- SM_SCSI_Test_Unit_Ready() --------------------------------------------------
int SM_SCSI_Test_Unit_Ready(struct us_data *us, struct scsi_cmnd *srb)
{
	//printk("SM_SCSI_Test_Unit_Ready\n");
	if (us->SM_Status.Insert && us->SM_Status.Ready)
		return USB_STOR_TRANSPORT_GOOD;
	else
	{
		ENE_SMInit(us);
		return USB_STOR_TRANSPORT_GOOD;
	}
		
	return USB_STOR_TRANSPORT_GOOD;
}

//----- SM_SCSI_Inquiry() --------------------------------------------------
int SM_SCSI_Inquiry(struct us_data *us, struct scsi_cmnd *srb)
{
	//printk("SM_SCSI_Inquiry\n");
=======
/* ----- SM_SCSI_Test_Unit_Ready() -------------------------------------------------- */
int SM_SCSI_Test_Unit_Ready(struct us_data *us, struct scsi_cmnd *srb)
=======
/* ----- SM_SCSI_Test_Unit_Ready() ------------------------------------- */
static int SM_SCSI_Test_Unit_Ready(struct us_data *us, struct scsi_cmnd *srb)
>>>>>>> refs/remotes/origin/master
{
	if (us->SM_Status.Insert && us->SM_Status.Ready)
		return USB_STOR_TRANSPORT_GOOD;
	else {
		ENE_SMInit(us);
		return USB_STOR_TRANSPORT_GOOD;
	}

	return USB_STOR_TRANSPORT_GOOD;
}

<<<<<<< HEAD
/* ----- SM_SCSI_Inquiry() -------------------------------------------------- */
int SM_SCSI_Inquiry(struct us_data *us, struct scsi_cmnd *srb)
{
>>>>>>> refs/remotes/origin/cm-10.0
	BYTE data_ptr[36] = {0x00, 0x80, 0x02, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x55, 0x53, 0x42, 0x32, 0x2E, 0x30, 0x20, 0x20, 0x43, 0x61, 0x72, 0x64, 0x52, 0x65, 0x61, 0x64, 0x65, 0x72, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x30, 0x31, 0x30, 0x30};
=======
/* ----- SM_SCSI_Inquiry() --------------------------------------------- */
static int SM_SCSI_Inquiry(struct us_data *us, struct scsi_cmnd *srb)
{
	BYTE data_ptr[36] = {0x00, 0x80, 0x02, 0x00, 0x1F, 0x00, 0x00, 0x00,
				 0x55, 0x53, 0x42, 0x32, 0x2E, 0x30, 0x20,
				 0x20, 0x43, 0x61, 0x72, 0x64, 0x52, 0x65,
				 0x61, 0x64, 0x65, 0x72, 0x20, 0x20, 0x20,
				 0x20, 0x20, 0x20, 0x30, 0x31, 0x30, 0x30};
>>>>>>> refs/remotes/origin/master

	usb_stor_set_xfer_buf(us, data_ptr, 36, srb, TO_XFER_BUF);
	return USB_STOR_TRANSPORT_GOOD;
}


<<<<<<< HEAD
<<<<<<< HEAD
//----- SM_SCSI_Mode_Sense() --------------------------------------------------
int SM_SCSI_Mode_Sense(struct us_data *us, struct scsi_cmnd *srb)
{
	BYTE	mediaNoWP[12] = {0x0b,0x00,0x00,0x08,0x00,0x00,0x71,0xc0,0x00,0x00,0x02,0x00};
	BYTE	mediaWP[12]   = {0x0b,0x00,0x80,0x08,0x00,0x00,0x71,0xc0,0x00,0x00,0x02,0x00};
=======
/* ----- SM_SCSI_Mode_Sense() -------------------------------------------------- */
int SM_SCSI_Mode_Sense(struct us_data *us, struct scsi_cmnd *srb)
{
	BYTE	mediaNoWP[12] = {0x0b, 0x00, 0x00, 0x08, 0x00, 0x00, 0x71, 0xc0, 0x00, 0x00, 0x02, 0x00};
	BYTE	mediaWP[12]   = {0x0b, 0x00, 0x80, 0x08, 0x00, 0x00, 0x71, 0xc0, 0x00, 0x00, 0x02, 0x00};
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* ----- SM_SCSI_Mode_Sense() ------------------------------------------ */
static int SM_SCSI_Mode_Sense(struct us_data *us, struct scsi_cmnd *srb)
{
	BYTE	mediaNoWP[12] = {0x0b, 0x00, 0x00, 0x08, 0x00, 0x00,
				0x71, 0xc0, 0x00, 0x00, 0x02, 0x00};
	BYTE	mediaWP[12]   = {0x0b, 0x00, 0x80, 0x08, 0x00, 0x00,
				0x71, 0xc0, 0x00, 0x00, 0x02, 0x00};
>>>>>>> refs/remotes/origin/master

	if (us->SM_Status.WtP)
		usb_stor_set_xfer_buf(us, mediaWP, 12, srb, TO_XFER_BUF);
	else
		usb_stor_set_xfer_buf(us, mediaNoWP, 12, srb, TO_XFER_BUF);


	return USB_STOR_TRANSPORT_GOOD;
}

<<<<<<< HEAD
<<<<<<< HEAD
//----- SM_SCSI_Read_Capacity() --------------------------------------------------
=======
/* ----- SM_SCSI_Read_Capacity() -------------------------------------------------- */
>>>>>>> refs/remotes/origin/cm-10.0
int SM_SCSI_Read_Capacity(struct us_data *us, struct scsi_cmnd *srb)
=======
/* ----- SM_SCSI_Read_Capacity() --------------------------------------- */
static int SM_SCSI_Read_Capacity(struct us_data *us, struct scsi_cmnd *srb)
>>>>>>> refs/remotes/origin/master
{
	unsigned int offset = 0;
	struct scatterlist *sg = NULL;
	DWORD   bl_num;
	WORD    bl_len;
	BYTE    buf[8];

<<<<<<< HEAD
	printk("SM_SCSI_Read_Capacity\n");

	bl_len = 0x200;
	bl_num = Ssfdc.MaxLogBlocks * Ssfdc.MaxSectors * Ssfdc.MaxZones - 1;
<<<<<<< HEAD
	//printk("MaxLogBlocks = %x\n", Ssfdc.MaxLogBlocks);
	//printk("MaxSectors   = %x\n", Ssfdc.MaxSectors);
	//printk("MaxZones     = %x\n", Ssfdc.MaxZones);
	//printk("bl_num       = %x\n", bl_num);
=======
>>>>>>> refs/remotes/origin/cm-10.0

	us->bl_num = bl_num;
	printk("bl_len = %x\n", bl_len);
	printk("bl_num = %x\n", bl_num);

<<<<<<< HEAD
	//srb->request_bufflen = 8;
	buf[0] = (bl_num>>24) & 0xff;
	buf[1] = (bl_num>>16) & 0xff;
	buf[2] = (bl_num>> 8) & 0xff;
	buf[3] = (bl_num>> 0) & 0xff;
	buf[4] = (bl_len>>24) & 0xff;
	buf[5] = (bl_len>>16) & 0xff;
	buf[6] = (bl_len>> 8) & 0xff;
	buf[7] = (bl_len>> 0) & 0xff;
	
	usb_stor_access_xfer_buf(us, buf, 8, srb, &sg, &offset, TO_XFER_BUF);
	//usb_stor_set_xfer_buf(us, buf, srb->request_bufflen, srb, TO_XFER_BUF);
=======
=======
	dev_dbg(&us->pusb_dev->dev, "SM_SCSI_Read_Capacity\n");

	bl_len = 0x200;
	bl_num = Ssfdc.MaxLogBlocks * Ssfdc.MaxSectors * Ssfdc.MaxZones - 1;

	us->bl_num = bl_num;
	dev_dbg(&us->pusb_dev->dev, "bl_len = %x\n", bl_len);
	dev_dbg(&us->pusb_dev->dev, "bl_num = %x\n", bl_num);

>>>>>>> refs/remotes/origin/master
	buf[0] = (bl_num >> 24) & 0xff;
	buf[1] = (bl_num >> 16) & 0xff;
	buf[2] = (bl_num >> 8) & 0xff;
	buf[3] = (bl_num >> 0) & 0xff;
	buf[4] = (bl_len >> 24) & 0xff;
	buf[5] = (bl_len >> 16) & 0xff;
	buf[6] = (bl_len >> 8) & 0xff;
	buf[7] = (bl_len >> 0) & 0xff;

	usb_stor_access_xfer_buf(us, buf, 8, srb, &sg, &offset, TO_XFER_BUF);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return USB_STOR_TRANSPORT_GOOD;
}

<<<<<<< HEAD
<<<<<<< HEAD
//----- SM_SCSI_Read() --------------------------------------------------
int SM_SCSI_Read(struct us_data *us, struct scsi_cmnd *srb)
{
	//struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *) us->iobuf;
	int result=0;
	PBYTE	Cdb = srb->cmnd;
	DWORD bn  =  ((Cdb[2]<<24) & 0xff000000) | ((Cdb[3]<<16) & 0x00ff0000) |
                   ((Cdb[4]<< 8) & 0x0000ff00) | ((Cdb[5]<< 0) & 0x000000ff);
	WORD  blen = ((Cdb[7]<< 8) & 0xff00)     | ((Cdb[8]<< 0) & 0x00ff);
	DWORD	blenByte = blen * 0x200;
	void	*buf;

	//printk("SCSIOP_READ --- bn = %X, blen = %X, srb->use_sg = %X\n", bn, blen, srb->use_sg);
	
=======
/* ----- SM_SCSI_Read() -------------------------------------------------- */
int SM_SCSI_Read(struct us_data *us, struct scsi_cmnd *srb)
{
	int result = 0;
	PBYTE	Cdb = srb->cmnd;
	DWORD bn  =  ((Cdb[2] << 24) & 0xff000000) | ((Cdb[3] << 16) & 0x00ff0000) |
		((Cdb[4] << 8) & 0x0000ff00) | ((Cdb[5] << 0) & 0x000000ff);
=======
/* ----- SM_SCSI_Read() -------------------------------------------------- */
static int SM_SCSI_Read(struct us_data *us, struct scsi_cmnd *srb)
{
	int result = 0;
	PBYTE	Cdb = srb->cmnd;
	DWORD bn  =  ((Cdb[2] << 24) & 0xff000000) |
			((Cdb[3] << 16) & 0x00ff0000) |
			((Cdb[4] << 8) & 0x0000ff00) |
			((Cdb[5] << 0) & 0x000000ff);
>>>>>>> refs/remotes/origin/master
	WORD  blen = ((Cdb[7] << 8) & 0xff00)     | ((Cdb[8] << 0) & 0x00ff);
	DWORD	blenByte = blen * 0x200;
	void	*buf;


<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (bn > us->bl_num)
		return USB_STOR_TRANSPORT_ERROR;

	buf = kmalloc(blenByte, GFP_KERNEL);
	if (buf == NULL)
		return USB_STOR_TRANSPORT_ERROR;
	result = Media_D_ReadSector(us, bn, blen, buf);
	usb_stor_set_xfer_buf(us, buf, blenByte, srb, TO_XFER_BUF);
	kfree(buf);

	if (!result)
		return USB_STOR_TRANSPORT_GOOD;
	else
		return USB_STOR_TRANSPORT_ERROR;

	return USB_STOR_TRANSPORT_GOOD;
}

<<<<<<< HEAD
<<<<<<< HEAD
//----- SM_SCSI_Write() --------------------------------------------------
int SM_SCSI_Write(struct us_data *us, struct scsi_cmnd *srb)
{
	//struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap *) us->iobuf;
	int result=0;
	PBYTE	Cdb = srb->cmnd;
	DWORD bn  =  ((Cdb[2]<<24) & 0xff000000) | ((Cdb[3]<<16) & 0x00ff0000) |
                   ((Cdb[4]<< 8) & 0x0000ff00) | ((Cdb[5]<< 0) & 0x000000ff);
	WORD  blen = ((Cdb[7]<< 8) & 0xff00)     | ((Cdb[8]<< 0) & 0x00ff);
	DWORD	blenByte = blen * 0x200;
	void	*buf;

	//printk("SCSIOP_Write --- bn = %X, blen = %X, srb->use_sg = %X\n", bn, blen, srb->use_sg);
=======
/* ----- SM_SCSI_Write() -------------------------------------------------- */
int SM_SCSI_Write(struct us_data *us, struct scsi_cmnd *srb)
{
	int result = 0;
	PBYTE	Cdb = srb->cmnd;
	DWORD bn  =  ((Cdb[2] << 24) & 0xff000000) | ((Cdb[3] << 16) & 0x00ff0000) |
		((Cdb[4] << 8) & 0x0000ff00) | ((Cdb[5] << 0) & 0x000000ff);
=======
/* ----- SM_SCSI_Write() -------------------------------------------------- */
static int SM_SCSI_Write(struct us_data *us, struct scsi_cmnd *srb)
{
	int result = 0;
	PBYTE	Cdb = srb->cmnd;
	DWORD bn  =  ((Cdb[2] << 24) & 0xff000000) |
			((Cdb[3] << 16) & 0x00ff0000) |
			((Cdb[4] << 8) & 0x0000ff00) |
			((Cdb[5] << 0) & 0x000000ff);
>>>>>>> refs/remotes/origin/master
	WORD  blen = ((Cdb[7] << 8) & 0xff00)     | ((Cdb[8] << 0) & 0x00ff);
	DWORD	blenByte = blen * 0x200;
	void	*buf;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (bn > us->bl_num)
		return USB_STOR_TRANSPORT_ERROR;

	buf = kmalloc(blenByte, GFP_KERNEL);
	if (buf == NULL)
		return USB_STOR_TRANSPORT_ERROR;
	usb_stor_set_xfer_buf(us, buf, blenByte, srb, FROM_XFER_BUF);
	result = Media_D_CopySector(us, bn, blen, buf);
	kfree(buf);

	if (!result)
		return USB_STOR_TRANSPORT_GOOD;
	else
		return USB_STOR_TRANSPORT_ERROR;

	return USB_STOR_TRANSPORT_GOOD;
}

