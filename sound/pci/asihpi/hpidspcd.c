/***********************************************************************/
<<<<<<< HEAD
<<<<<<< HEAD
/*!

    AudioScience HPI driver
    Copyright (C) 1997-2010  AudioScience Inc. <support@audioscience.com>
=======
=======
>>>>>>> refs/remotes/origin/master
/**

    AudioScience HPI driver
    Copyright (C) 1997-2011  AudioScience Inc. <support@audioscience.com>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

    This program is free software; you can redistribute it and/or modify
    it under the terms of version 2 of the GNU General Public License as
    published by the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

\file
<<<<<<< HEAD
<<<<<<< HEAD
Functions for reading DSP code to load into DSP

(Linux only:) If DSPCODE_FIRMWARE_LOADER is defined, code is read using
hotplug firmware loader from individual dsp code files

If neither of the above is defined, code is read from linked arrays.
DSPCODE_ARRAY is defined.

HPI_INCLUDE_**** must be defined
and the appropriate hzz?????.c or hex?????.c linked in

 */
=======
Functions for reading DSP code using
hotplug firmware loader from individual dsp code files
*/
>>>>>>> refs/remotes/origin/cm-10.0
=======
Functions for reading DSP code using
hotplug firmware loader from individual dsp code files
*/
>>>>>>> refs/remotes/origin/master
/***********************************************************************/
#define SOURCEFILE_NAME "hpidspcd.c"
#include "hpidspcd.h"
#include "hpidebug.h"
<<<<<<< HEAD
<<<<<<< HEAD

/**
 Header structure for binary dsp code file (see asidsp.doc)
 This structure must match that used in s2bin.c for generation of asidsp.bin
 */

#ifndef DISABLE_PRAGMA_PACK1
#pragma pack(push, 1)
#endif

struct code_header {
	u32 size;
	char type[4];
	u32 adapter;
	u32 version;
	u32 crc;
};

#ifndef DISABLE_PRAGMA_PACK1
#pragma pack(pop)
#endif

#define HPI_VER_DECIMAL ((int)(HPI_VER_MAJOR(HPI_VER) * 10000 + \
	    HPI_VER_MINOR(HPI_VER) * 100 + HPI_VER_RELEASE(HPI_VER)))

/***********************************************************************/
#include <linux/pci.h>
/*-------------------------------------------------------------------*/
short hpi_dsp_code_open(u32 adapter, struct dsp_code *ps_dsp_code,
	u32 *pos_error_code)
{
	const struct firmware *ps_firmware = ps_dsp_code->ps_firmware;
	struct code_header header;
	char fw_name[20];
=======
=======
>>>>>>> refs/remotes/origin/master
#include "hpi_version.h"

struct dsp_code_private {
	/**  Firmware descriptor */
	const struct firmware *firmware;
	struct pci_dev *dev;
};

/*-------------------------------------------------------------------*/
short hpi_dsp_code_open(u32 adapter, void *os_data, struct dsp_code *dsp_code,
	u32 *os_error_code)
{
	const struct firmware *firmware;
	struct pci_dev *dev = os_data;
	struct code_header header;
	char fw_name[20];
	short err_ret = HPI_ERROR_DSP_FILE_NOT_FOUND;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int err;

	sprintf(fw_name, "asihpi/dsp%04x.bin", adapter);

<<<<<<< HEAD
<<<<<<< HEAD
	err = request_firmware(&ps_firmware, fw_name,
		&ps_dsp_code->ps_dev->dev);

	if (err != 0) {
		dev_printk(KERN_ERR, &ps_dsp_code->ps_dev->dev,
=======
	err = request_firmware(&firmware, fw_name, &dev->dev);

	if (err || !firmware) {
		dev_printk(KERN_ERR, &dev->dev,
>>>>>>> refs/remotes/origin/cm-10.0
			"%d, request_firmware failed for  %s\n", err,
			fw_name);
		goto error1;
	}
<<<<<<< HEAD
	if (ps_firmware->size < sizeof(header)) {
		dev_printk(KERN_ERR, &ps_dsp_code->ps_dev->dev,
			"Header size too small %s\n", fw_name);
		goto error2;
	}
	memcpy(&header, ps_firmware->data, sizeof(header));
	if (header.adapter != adapter) {
		dev_printk(KERN_ERR, &ps_dsp_code->ps_dev->dev,
			"Adapter type incorrect %4x != %4x\n", header.adapter,
			adapter);
		goto error2;
	}
	if (header.size != ps_firmware->size) {
		dev_printk(KERN_ERR, &ps_dsp_code->ps_dev->dev,
			"Code size wrong  %d != %ld\n", header.size,
			(unsigned long)ps_firmware->size);
		goto error2;
	}

	if (header.version / 100 != HPI_VER_DECIMAL / 100) {
		dev_printk(KERN_ERR, &ps_dsp_code->ps_dev->dev,
			"Incompatible firmware version "
			"DSP image %d != Driver %d\n", header.version,
			HPI_VER_DECIMAL);
		goto error2;
	}

	if (header.version != HPI_VER_DECIMAL) {
		dev_printk(KERN_WARNING, &ps_dsp_code->ps_dev->dev,
			"Firmware: release version mismatch  DSP image %d != Driver %d\n",
			header.version, HPI_VER_DECIMAL);
	}

	HPI_DEBUG_LOG(DEBUG, "dsp code %s opened\n", fw_name);
	ps_dsp_code->ps_firmware = ps_firmware;
	ps_dsp_code->block_length = header.size / sizeof(u32);
	ps_dsp_code->word_count = sizeof(header) / sizeof(u32);
	ps_dsp_code->version = header.version;
	ps_dsp_code->crc = header.crc;
	return 0;

error2:
	release_firmware(ps_firmware);
error1:
	ps_dsp_code->ps_firmware = NULL;
	ps_dsp_code->block_length = 0;
	return HPI_ERROR_DSP_FILE_NOT_FOUND;
}

/*-------------------------------------------------------------------*/
void hpi_dsp_code_close(struct dsp_code *ps_dsp_code)
{
	if (ps_dsp_code->ps_firmware != NULL) {
		HPI_DEBUG_LOG(DEBUG, "dsp code closed\n");
		release_firmware(ps_dsp_code->ps_firmware);
		ps_dsp_code->ps_firmware = NULL;
	}
}

/*-------------------------------------------------------------------*/
void hpi_dsp_code_rewind(struct dsp_code *ps_dsp_code)
{
	/* Go back to start of  data, after header */
	ps_dsp_code->word_count = sizeof(struct code_header) / sizeof(u32);
}

/*-------------------------------------------------------------------*/
short hpi_dsp_code_read_word(struct dsp_code *ps_dsp_code, u32 *pword)
{
	if (ps_dsp_code->word_count + 1 > ps_dsp_code->block_length)
		return HPI_ERROR_DSP_FILE_FORMAT;

	*pword = ((u32 *)(ps_dsp_code->ps_firmware->data))[ps_dsp_code->
		word_count];
	ps_dsp_code->word_count++;
=======
	if (firmware->size < sizeof(header)) {
		dev_printk(KERN_ERR, &dev->dev, "Header size too small %s\n",
			fw_name);
=======
	err = request_firmware(&firmware, fw_name, &dev->dev);

	if (err || !firmware) {
		dev_err(&dev->dev, "%d, request_firmware failed for %s\n",
			err, fw_name);
		goto error1;
	}
	if (firmware->size < sizeof(header)) {
		dev_err(&dev->dev, "Header size too small %s\n", fw_name);
>>>>>>> refs/remotes/origin/master
		goto error2;
	}
	memcpy(&header, firmware->data, sizeof(header));

	if ((header.type != 0x45444F43) ||	/* "CODE" */
		(header.adapter != adapter)
		|| (header.size != firmware->size)) {
<<<<<<< HEAD
		dev_printk(KERN_ERR, &dev->dev,
=======
		dev_err(&dev->dev,
>>>>>>> refs/remotes/origin/master
			"Invalid firmware header size %d != file %zd\n",
			header.size, firmware->size);
		goto error2;
	}

	if ((header.version >> 9) != (HPI_VER >> 9)) {
		/* Consider even and subsequent odd minor versions to be compatible */
<<<<<<< HEAD
		dev_printk(KERN_ERR, &dev->dev,
			"Incompatible firmware version "
			"DSP image %X != Driver %X\n", header.version,
			HPI_VER);
=======
		dev_err(&dev->dev, "Incompatible firmware version DSP image %X != Driver %X\n",
			header.version, HPI_VER);
>>>>>>> refs/remotes/origin/master
		goto error2;
	}

	if (header.version != HPI_VER) {
<<<<<<< HEAD
		dev_printk(KERN_INFO, &dev->dev,
			"Firmware: release version mismatch  DSP image %X != Driver %X\n",
			header.version, HPI_VER);
=======
		dev_info(&dev->dev,
			 "Firmware: release version mismatch  DSP image %X != Driver %X\n",
			 header.version, HPI_VER);
>>>>>>> refs/remotes/origin/master
	}

	HPI_DEBUG_LOG(DEBUG, "dsp code %s opened\n", fw_name);
	dsp_code->pvt = kmalloc(sizeof(*dsp_code->pvt), GFP_KERNEL);
	if (!dsp_code->pvt) {
		err_ret = HPI_ERROR_MEMORY_ALLOC;
		goto error2;
	}

	dsp_code->pvt->dev = dev;
	dsp_code->pvt->firmware = firmware;
	dsp_code->header = header;
	dsp_code->block_length = header.size / sizeof(u32);
	dsp_code->word_count = sizeof(header) / sizeof(u32);
	return 0;

error2:
	release_firmware(firmware);
error1:
	dsp_code->block_length = 0;
	return err_ret;
}

/*-------------------------------------------------------------------*/
void hpi_dsp_code_close(struct dsp_code *dsp_code)
{
	HPI_DEBUG_LOG(DEBUG, "dsp code closed\n");
	release_firmware(dsp_code->pvt->firmware);
	kfree(dsp_code->pvt);
}

/*-------------------------------------------------------------------*/
void hpi_dsp_code_rewind(struct dsp_code *dsp_code)
{
	/* Go back to start of  data, after header */
	dsp_code->word_count = sizeof(struct code_header) / sizeof(u32);
}

/*-------------------------------------------------------------------*/
short hpi_dsp_code_read_word(struct dsp_code *dsp_code, u32 *pword)
{
	if (dsp_code->word_count + 1 > dsp_code->block_length)
		return HPI_ERROR_DSP_FILE_FORMAT;

	*pword = ((u32 *)(dsp_code->pvt->firmware->data))[dsp_code->
		word_count];
	dsp_code->word_count++;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*-------------------------------------------------------------------*/
short hpi_dsp_code_read_block(size_t words_requested,
<<<<<<< HEAD
<<<<<<< HEAD
	struct dsp_code *ps_dsp_code, u32 **ppblock)
{
	if (ps_dsp_code->word_count + words_requested >
		ps_dsp_code->block_length)
		return HPI_ERROR_DSP_FILE_FORMAT;

	*ppblock =
		((u32 *)(ps_dsp_code->ps_firmware->data)) +
		ps_dsp_code->word_count;
	ps_dsp_code->word_count += words_requested;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct dsp_code *dsp_code, u32 **ppblock)
{
	if (dsp_code->word_count + words_requested > dsp_code->block_length)
		return HPI_ERROR_DSP_FILE_FORMAT;

	*ppblock =
		((u32 *)(dsp_code->pvt->firmware->data)) +
		dsp_code->word_count;
	dsp_code->word_count += words_requested;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}
