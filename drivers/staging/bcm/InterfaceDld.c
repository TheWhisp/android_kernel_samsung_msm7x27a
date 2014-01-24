#include "headers.h"

<<<<<<< HEAD
<<<<<<< HEAD

int InterfaceFileDownload( PVOID arg,
                        struct file *flp,
                        unsigned int on_chip_loc)
{
   // unsigned int    reg=0;
    mm_segment_t    oldfs={0};
    int             errno=0, len=0 /*,is_config_file = 0*/;
    loff_t          pos=0;
	PS_INTERFACE_ADAPTER psIntfAdapter = (PS_INTERFACE_ADAPTER)arg;
	//PMINI_ADAPTER Adapter = psIntfAdapter->psAdapter;
    char            *buff=kmalloc(MAX_TRANSFER_CTRL_BYTE_USB, GFP_KERNEL);

    if(!buff)
    {
        return -ENOMEM;
    }
    while(1)
    {
        oldfs=get_fs(); set_fs(get_ds());
        len=vfs_read(flp, (void __force __user *)buff, MAX_TRANSFER_CTRL_BYTE_USB, &pos);
        set_fs(oldfs);
        if(len<=0)
        {
            if(len<0)
            {
                BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "len < 0");
                errno=len;
            }
            else
            {
                errno = 0;
                BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Got end of file!");
            }
            break;
        }
        //BCM_DEBUG_PRINT_BUFFER(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, buff, MAX_TRANSFER_CTRL_BYTE_USB);
        errno = InterfaceWRM(psIntfAdapter, on_chip_loc, buff, len) ;
		if(errno)
		{
            BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_PRINTK, 0, 0, "WRM Failed! status: %d", errno);
			break;

		}
        on_chip_loc+=MAX_TRANSFER_CTRL_BYTE_USB;
	}/* End of for(;;)*/

	kfree(buff);
    return errno;
}

int InterfaceFileReadbackFromChip( PVOID arg,
                        struct file *flp,
                        unsigned int on_chip_loc)
{
    char            *buff, *buff_readback;
    unsigned int    reg=0;
    mm_segment_t    oldfs={0};
    int             errno=0, len=0, is_config_file = 0;
    loff_t          pos=0;
    static int fw_down = 0;
	INT				Status = STATUS_SUCCESS;
	PS_INTERFACE_ADAPTER psIntfAdapter = (PS_INTERFACE_ADAPTER)arg;

    buff=kmalloc(MAX_TRANSFER_CTRL_BYTE_USB, GFP_DMA);
    buff_readback=kmalloc(MAX_TRANSFER_CTRL_BYTE_USB , GFP_DMA);
    if(!buff || !buff_readback)
    {
        kfree(buff);
        kfree(buff_readback);

        return -ENOMEM;
    }

	is_config_file = (on_chip_loc == CONFIG_BEGIN_ADDR)? 1:0;

	memset(buff_readback, 0, MAX_TRANSFER_CTRL_BYTE_USB);
	memset(buff, 0, MAX_TRANSFER_CTRL_BYTE_USB);
    while(1)
    {
        oldfs=get_fs(); set_fs(get_ds());
        len=vfs_read(flp, (void __force __user *)buff, MAX_TRANSFER_CTRL_BYTE_USB, &pos);
        set_fs(oldfs);
        fw_down++;
        if(len<=0)
        {
            if(len<0)
            {
                BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "len < 0");
                errno=len;
            }
            else
            {
                errno = 0;
                BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Got end of file!");
            }
            break;
        }


		Status = InterfaceRDM(psIntfAdapter, on_chip_loc, buff_readback, len);
		if(Status)
		{
            BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "RDM of len %d Failed! %d", len, reg);
			goto exit;
		}
		reg++;
        if((len-sizeof(unsigned int))<4)
        {
            if(memcmp(buff_readback, buff, len))
            {
                BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Firmware Download is not proper %d", fw_down);
				BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT,DBG_LVL_ALL,"Length is: %d",len);
				Status = -EIO;
				goto exit;
            }
        }
        else
        {
            len-=4;
            while(len)
            {
                if(*(unsigned int*)&buff_readback[len] != *(unsigned int *)&buff[len])
                {
                    BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Firmware Download is not proper %d", fw_down);
                    BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Val from Binary %x, Val From Read Back %x ", *(unsigned int *)&buff[len], *(unsigned int*)&buff_readback[len]);
                    BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "len =%x!!!", len);
					Status = -EIO;
					goto exit;
                }
                len-=4;
            }
        }
        on_chip_loc+=MAX_TRANSFER_CTRL_BYTE_USB;
    }/* End of while(1)*/
exit:
    kfree(buff);
    kfree(buff_readback);
	return Status;
}

static int bcm_download_config_file(PMINI_ADAPTER Adapter,
								FIRMWARE_INFO *psFwInfo)
=======
=======
>>>>>>> refs/remotes/origin/master
int InterfaceFileDownload(PVOID arg, struct file *flp, unsigned int on_chip_loc)
{
	/* unsigned int reg = 0; */
	mm_segment_t oldfs = {0};
	int errno = 0, len = 0; /* ,is_config_file = 0 */
	loff_t pos = 0;
<<<<<<< HEAD
	PS_INTERFACE_ADAPTER psIntfAdapter = (PS_INTERFACE_ADAPTER)arg;
	/* PMINI_ADAPTER Adapter = psIntfAdapter->psAdapter; */
=======
	struct bcm_interface_adapter *psIntfAdapter = arg;
	/* struct bcm_mini_adapter *Adapter = psIntfAdapter->psAdapter; */
>>>>>>> refs/remotes/origin/master
	char *buff = kmalloc(MAX_TRANSFER_CTRL_BYTE_USB, GFP_KERNEL);

	if (!buff)
		return -ENOMEM;

	while (1) {
		oldfs = get_fs();
		set_fs(get_ds());
		len = vfs_read(flp, (void __force __user *)buff,
			MAX_TRANSFER_CTRL_BYTE_USB, &pos);
		set_fs(oldfs);
		if (len <= 0) {
<<<<<<< HEAD
			if (len < 0) {
				BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,
						DBG_TYPE_INITEXIT, MP_INIT,
						DBG_LVL_ALL, "len < 0");
				errno = len;
			} else {
				errno = 0;
				BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,
						DBG_TYPE_INITEXIT, MP_INIT,
						DBG_LVL_ALL,
						"Got end of file!");
			}
=======
			if (len < 0)
				errno = len;
			else
				errno = 0;
>>>>>>> refs/remotes/origin/master
			break;
		}
		/* BCM_DEBUG_PRINT_BUFFER(Adapter,DBG_TYPE_INITEXIT, MP_INIT,
		 *			  DBG_LVL_ALL, buff,
		 *			  MAX_TRANSFER_CTRL_BYTE_USB);
		 */
		errno = InterfaceWRM(psIntfAdapter, on_chip_loc, buff, len);
<<<<<<< HEAD
		if (errno) {
			BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,
					DBG_TYPE_PRINTK, 0, 0,
					"WRM Failed! status: %d", errno);
			break;
		}
=======
		if (errno)
			break;
>>>>>>> refs/remotes/origin/master
		on_chip_loc += MAX_TRANSFER_CTRL_BYTE_USB;
	}

	kfree(buff);
	return errno;
}

<<<<<<< HEAD
int InterfaceFileReadbackFromChip(PVOID arg, struct file *flp, unsigned int on_chip_loc)
=======
int InterfaceFileReadbackFromChip(PVOID arg, struct file *flp,
				unsigned int on_chip_loc)
>>>>>>> refs/remotes/origin/master
{
	char *buff, *buff_readback;
	unsigned int reg = 0;
	mm_segment_t oldfs = {0};
	int errno = 0, len = 0, is_config_file = 0;
	loff_t pos = 0;
	static int fw_down;
	INT Status = STATUS_SUCCESS;
<<<<<<< HEAD
	PS_INTERFACE_ADAPTER psIntfAdapter = (PS_INTERFACE_ADAPTER)arg;
=======
	struct bcm_interface_adapter *psIntfAdapter = arg;
>>>>>>> refs/remotes/origin/master
	int bytes;

	buff = kmalloc(MAX_TRANSFER_CTRL_BYTE_USB, GFP_DMA);
	buff_readback = kmalloc(MAX_TRANSFER_CTRL_BYTE_USB , GFP_DMA);
	if (!buff || !buff_readback) {
		kfree(buff);
		kfree(buff_readback);

		return -ENOMEM;
	}

	is_config_file = (on_chip_loc == CONFIG_BEGIN_ADDR) ? 1 : 0;

	memset(buff_readback, 0, MAX_TRANSFER_CTRL_BYTE_USB);
	memset(buff, 0, MAX_TRANSFER_CTRL_BYTE_USB);
	while (1) {
		oldfs = get_fs();
		set_fs(get_ds());
<<<<<<< HEAD
		len = vfs_read(flp, (void __force __user *)buff, MAX_TRANSFER_CTRL_BYTE_USB, &pos);
=======
		len = vfs_read(flp, (void __force __user *)buff,
				MAX_TRANSFER_CTRL_BYTE_USB, &pos);
>>>>>>> refs/remotes/origin/master
		set_fs(oldfs);
		fw_down++;

		if (len <= 0) {
<<<<<<< HEAD
			if (len < 0) {
				BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "len < 0");
				errno = len;
			} else {
				errno = 0;
				BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Got end of file!");
			}
			break;
		}

		bytes = InterfaceRDM(psIntfAdapter, on_chip_loc, buff_readback, len);
		if (bytes < 0) {
			Status = bytes;
			BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "RDM of len %d Failed! %d", len, reg);
=======
			if (len < 0)
				errno = len;
			else
				errno = 0;
			break;
		}

		bytes = InterfaceRDM(psIntfAdapter, on_chip_loc,
					buff_readback, len);
		if (bytes < 0) {
			Status = bytes;
>>>>>>> refs/remotes/origin/master
			goto exit;
		}
		reg++;
		if ((len-sizeof(unsigned int)) < 4) {
			if (memcmp(buff_readback, buff, len)) {
<<<<<<< HEAD
				BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Firmware Download is not proper %d", fw_down);
				BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Length is: %d", len);
=======
>>>>>>> refs/remotes/origin/master
				Status = -EIO;
				goto exit;
			}
		} else {
			len -= 4;

			while (len) {
<<<<<<< HEAD
				if (*(unsigned int *)&buff_readback[len] != *(unsigned int *)&buff[len]) {
					BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Firmware Download is not proper %d", fw_down);
					BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Val from Binary %x, Val From Read Back %x ", *(unsigned int *)&buff[len], *(unsigned int*)&buff_readback[len]);
					BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "len =%x!!!", len);
=======
				if (*(unsigned int *)&buff_readback[len] !=
						 *(unsigned int *)&buff[len]) {
>>>>>>> refs/remotes/origin/master
					Status = -EIO;
					goto exit;
				}
				len -= 4;
			}
		}
		on_chip_loc += MAX_TRANSFER_CTRL_BYTE_USB;
	} /* End of while(1) */

exit:
	kfree(buff);
	kfree(buff_readback);
	return Status;
}

<<<<<<< HEAD
static int bcm_download_config_file(PMINI_ADAPTER Adapter, FIRMWARE_INFO *psFwInfo)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int bcm_download_config_file(struct bcm_mini_adapter *Adapter,
				struct bcm_firmware_info *psFwInfo)
>>>>>>> refs/remotes/origin/master
{
	int retval = STATUS_SUCCESS;
	B_UINT32 value = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	if(Adapter->pstargetparams == NULL)
    {
        if((Adapter->pstargetparams =
            kmalloc(sizeof(STARGETPARAMS), GFP_KERNEL)) == NULL)
        {
            return -ENOMEM;
        }
    }
	if(psFwInfo->u32FirmwareLength != sizeof(STARGETPARAMS))
	{
		return -EIO;
	}
	retval = copy_from_user(Adapter->pstargetparams,
			psFwInfo->pvMappedFirmwareAddress, psFwInfo->u32FirmwareLength);
	if(retval)
	{
=======
	if (Adapter->pstargetparams == NULL) {
		Adapter->pstargetparams = kmalloc(sizeof(STARGETPARAMS), GFP_KERNEL);
=======
	if (Adapter->pstargetparams == NULL) {
		Adapter->pstargetparams =
			kmalloc(sizeof(struct bcm_target_params), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
		if (Adapter->pstargetparams == NULL)
			return -ENOMEM;
	}

<<<<<<< HEAD
	if (psFwInfo->u32FirmwareLength != sizeof(STARGETPARAMS))
		return -EIO;

	retval = copy_from_user(Adapter->pstargetparams, psFwInfo->pvMappedFirmwareAddress, psFwInfo->u32FirmwareLength);
	if (retval) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (psFwInfo->u32FirmwareLength != sizeof(struct bcm_target_params))
		return -EIO;

	retval = copy_from_user(Adapter->pstargetparams,
			psFwInfo->pvMappedFirmwareAddress,
			psFwInfo->u32FirmwareLength);
	if (retval) {
>>>>>>> refs/remotes/origin/master
		kfree(Adapter->pstargetparams);
		Adapter->pstargetparams = NULL;
		return -EFAULT;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	/* Parse the structure and then Download the Firmware */
	beceem_parse_target_struct(Adapter);

	//Initializing the NVM.
	BcmInitNVM(Adapter);

	retval = InitLedSettings (Adapter);

	if(retval)
	{
		BCM_DEBUG_PRINT (Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "INIT LED Failed\n");
		return retval;
	}

	if(Adapter->LEDInfo.led_thread_running & BCM_LED_THREAD_RUNNING_ACTIVELY)
	{
=======
=======
>>>>>>> refs/remotes/origin/master

	/* Parse the structure and then Download the Firmware */
	beceem_parse_target_struct(Adapter);

	/* Initializing the NVM. */
	BcmInitNVM(Adapter);
	retval = InitLedSettings(Adapter);

<<<<<<< HEAD
	if (retval) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "INIT LED Failed\n");
		return retval;
	}

	if (Adapter->LEDInfo.led_thread_running & BCM_LED_THREAD_RUNNING_ACTIVELY) {
>>>>>>> refs/remotes/origin/cm-10.0
		Adapter->LEDInfo.bLedInitDone = FALSE;
=======
	if (retval)
		return retval;

	if (Adapter->LEDInfo.led_thread_running &
			BCM_LED_THREAD_RUNNING_ACTIVELY) {
		Adapter->LEDInfo.bLedInitDone = false;
>>>>>>> refs/remotes/origin/master
		Adapter->DriverState = DRIVER_INIT;
		wake_up(&Adapter->LEDInfo.notify_led_event);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if(Adapter->LEDInfo.led_thread_running & BCM_LED_THREAD_RUNNING_ACTIVELY)
	{
=======
	if (Adapter->LEDInfo.led_thread_running & BCM_LED_THREAD_RUNNING_ACTIVELY) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (Adapter->LEDInfo.led_thread_running &
			BCM_LED_THREAD_RUNNING_ACTIVELY) {
>>>>>>> refs/remotes/origin/master
		Adapter->DriverState = FW_DOWNLOAD;
		wake_up(&Adapter->LEDInfo.notify_led_event);
	}

	/* Initialize the DDR Controller */
	retval = ddr_init(Adapter);
<<<<<<< HEAD
<<<<<<< HEAD
	if(retval)
	{
		BCM_DEBUG_PRINT (Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "DDR Init Failed\n");
=======
	if (retval) {
		BCM_DEBUG_PRINT (Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "DDR Init Failed\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return retval;
	}

	value = 0;
	wrmalt(Adapter, EEPROM_CAL_DATA_INTERNAL_LOC - 4, &value, sizeof(value));
	wrmalt(Adapter, EEPROM_CAL_DATA_INTERNAL_LOC - 8, &value, sizeof(value));

<<<<<<< HEAD
	if(Adapter->eNVMType == NVM_FLASH)
	{
		retval = PropagateCalParamsFromFlashToMemory(Adapter);
		if(retval)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL,"propagaion of cal param failed with status :%d", retval);
=======
	if (Adapter->eNVMType == NVM_FLASH) {
		retval = PropagateCalParamsFromFlashToMemory(Adapter);
		if (retval) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "propagaion of cal param failed with status :%d", retval);
>>>>>>> refs/remotes/origin/cm-10.0
			return retval;
		}
	}

<<<<<<< HEAD

	retval =buffDnldVerify(Adapter,(PUCHAR)Adapter->pstargetparams,sizeof(STARGETPARAMS),CONFIG_BEGIN_ADDR);

	if(retval)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "configuration file not downloaded properly");
	}
	else
		Adapter->bCfgDownloaded = TRUE;


	return retval;
}
static int bcm_compare_buff_contents(unsigned char *readbackbuff,
	unsigned char *buff,unsigned int len)
{
	int retval = STATUS_SUCCESS;
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
    if((len-sizeof(unsigned int))<4)
	{
		if(memcmp(readbackbuff , buff, len))
		{
			retval=-EINVAL;
		}
	}
	else
	{
		len-=4;
		while(len)
		{
			if(*(unsigned int*)&readbackbuff[len] !=
					*(unsigned int *)&buff[len])
			{
				BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Firmware Download is not proper");
				BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Val from Binary %x, Val From Read Back %x ", *(unsigned int *)&buff[len], *(unsigned int*)&readbackbuff[len]);
				BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "len =%x!!!", len);
				retval=-EINVAL;
				break;
			}
			len-=4;
=======
	retval = buffDnldVerify(Adapter, (PUCHAR)Adapter->pstargetparams, sizeof(STARGETPARAMS), CONFIG_BEGIN_ADDR);

	if (retval)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "configuration file not downloaded properly");
=======
	if (retval)
		return retval;

	value = 0;
	wrmalt(Adapter, EEPROM_CAL_DATA_INTERNAL_LOC - 4,
				&value, sizeof(value));
	wrmalt(Adapter, EEPROM_CAL_DATA_INTERNAL_LOC - 8,
				&value, sizeof(value));

	if (Adapter->eNVMType == NVM_FLASH) {
		retval = PropagateCalParamsFromFlashToMemory(Adapter);
		if (retval)
			return retval;
	}

	retval = buffDnldVerify(Adapter, (PUCHAR)Adapter->pstargetparams,
			sizeof(struct bcm_target_params), CONFIG_BEGIN_ADDR);

	if (retval)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT,
				MP_INIT, DBG_LVL_ALL,
				"configuration file not downloaded properly");
>>>>>>> refs/remotes/origin/master
	else
		Adapter->bCfgDownloaded = TRUE;

	return retval;
}

<<<<<<< HEAD
static int bcm_compare_buff_contents(unsigned char *readbackbuff, unsigned char *buff, unsigned int len)
{
	int retval = STATUS_SUCCESS;
	PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if ((len-sizeof(unsigned int)) < 4) {
		if (memcmp(readbackbuff , buff, len))
			retval = -EINVAL;
	} else {
		len -= 4;

		while (len) {
			if (*(unsigned int *)&readbackbuff[len] != *(unsigned int *)&buff[len]) {
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Firmware Download is not proper");
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Val from Binary %x, Val From Read Back %x ", *(unsigned int *)&buff[len], *(unsigned int*)&readbackbuff[len]);
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "len =%x!!!", len);
				retval = -EINVAL;
				break;
			}
			len -= 4;
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}
	return retval;
}
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/cm-10.0
int bcm_ioctl_fw_download(PMINI_ADAPTER Adapter, FIRMWARE_INFO *psFwInfo)
=======
int bcm_ioctl_fw_download(struct bcm_mini_adapter *Adapter,
			struct bcm_firmware_info *psFwInfo)
>>>>>>> refs/remotes/origin/master
{
	int retval = STATUS_SUCCESS;
	PUCHAR buff = NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	/*  Config File is needed for the Driver to download the Config file and
		Firmware. Check for the Config file to be first to be sent from the
		Application
	*/
	atomic_set (&Adapter->uiMBupdate, FALSE);
	if(!Adapter->bCfgDownloaded &&
		psFwInfo->u32StartingAddress != CONFIG_BEGIN_ADDR)
	{
		/*Can't Download Firmware.*/
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL,"Download the config File first\n");
=======
=======
>>>>>>> refs/remotes/origin/master
	/* Config File is needed for the Driver to download the Config file and
	 * Firmware. Check for the Config file to be first to be sent from the
	 * Application
	 */
<<<<<<< HEAD
	atomic_set(&Adapter->uiMBupdate, FALSE);
	if (!Adapter->bCfgDownloaded && psFwInfo->u32StartingAddress != CONFIG_BEGIN_ADDR) {
		/* Can't Download Firmware. */
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Download the config File first\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	atomic_set(&Adapter->uiMBupdate, false);
	if (!Adapter->bCfgDownloaded &&
		psFwInfo->u32StartingAddress != CONFIG_BEGIN_ADDR) {
		/* Can't Download Firmware. */
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	/* If Config File, Finish the DDR Settings and then Download CFG File */
<<<<<<< HEAD
<<<<<<< HEAD
    if(psFwInfo->u32StartingAddress == CONFIG_BEGIN_ADDR)
    {
		retval = bcm_download_config_file (Adapter, psFwInfo);
	}
	else
	{

		buff = kzalloc(psFwInfo->u32FirmwareLength,GFP_KERNEL);
		if(buff==NULL)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL,"Failed in allocation memory");
			return -ENOMEM;
		}
		retval = copy_from_user(buff,psFwInfo->pvMappedFirmwareAddress, psFwInfo->u32FirmwareLength);
		if(retval != STATUS_SUCCESS)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "copying buffer from user space failed");
			retval = -EFAULT;
			goto error ;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (psFwInfo->u32StartingAddress == CONFIG_BEGIN_ADDR) {
		retval = bcm_download_config_file(Adapter, psFwInfo);
	} else {
		buff = kzalloc(psFwInfo->u32FirmwareLength, GFP_KERNEL);
<<<<<<< HEAD
		if (buff == NULL) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Failed in allocation memory");
			return -ENOMEM;
		}

		retval = copy_from_user(buff, psFwInfo->pvMappedFirmwareAddress, psFwInfo->u32FirmwareLength);
		if (retval != STATUS_SUCCESS) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "copying buffer from user space failed");
			retval = -EFAULT;
			goto error;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (buff == NULL)
			return -ENOMEM;

		retval = copy_from_user(buff,
			psFwInfo->pvMappedFirmwareAddress,
			psFwInfo->u32FirmwareLength);
		if (retval != STATUS_SUCCESS) {
			retval = -EFAULT;
			goto error;
>>>>>>> refs/remotes/origin/master
		}

		retval = buffDnldVerify(Adapter,
					buff,
					psFwInfo->u32FirmwareLength,
					psFwInfo->u32StartingAddress);
<<<<<<< HEAD
<<<<<<< HEAD
		if(retval != STATUS_SUCCESS)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL,"f/w download failed status :%d", retval);
			goto error;
		}
	}
=======

		if (retval != STATUS_SUCCESS) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "f/w download failed status :%d", retval);
			goto error;
		}
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======

		if (retval != STATUS_SUCCESS)
			goto error;
	}

>>>>>>> refs/remotes/origin/master
error:
	kfree(buff);
	return retval;
}

<<<<<<< HEAD
<<<<<<< HEAD
static INT buffDnld(PMINI_ADAPTER Adapter, PUCHAR mappedbuffer, UINT u32FirmwareLength,
		ULONG u32StartingAddress)
{

	unsigned int	len = 0;
	int retval = STATUS_SUCCESS;
	len = u32FirmwareLength;

	while(u32FirmwareLength)
	{
		len = MIN_VAL (u32FirmwareLength, MAX_TRANSFER_CTRL_BYTE_USB);
		retval = wrm (Adapter, u32StartingAddress, mappedbuffer, len);
		if(retval)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "wrm failed with status :%d", retval);
			break;
		}
		u32StartingAddress	+= len;
		u32FirmwareLength	-= len;
		mappedbuffer		+=len;
	}
	return retval;

}

static INT buffRdbkVerify(PMINI_ADAPTER Adapter,
			PUCHAR mappedbuffer, UINT u32FirmwareLength,
			ULONG u32StartingAddress)
{
	UINT len = u32FirmwareLength;
	INT retval = STATUS_SUCCESS;
	PUCHAR readbackbuff = kzalloc(MAX_TRANSFER_CTRL_BYTE_USB,GFP_KERNEL);

	if(NULL == readbackbuff)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "MEMORY ALLOCATION FAILED");
		return -ENOMEM;
	}
	while (u32FirmwareLength && !retval)
	{

		len = MIN_VAL (u32FirmwareLength, MAX_TRANSFER_CTRL_BYTE_USB);

		retval = rdm (Adapter, u32StartingAddress, readbackbuff, len);
		if(retval)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "rdm failed with status %d" ,retval);
			break;
		}

		if (STATUS_SUCCESS != (retval = bcm_compare_buff_contents (readbackbuff, mappedbuffer, len)))
		{
			break;
		}
		u32StartingAddress 	+= len;
		u32FirmwareLength  	-= len;
		mappedbuffer	   	+=len;
	}/* end of while (u32FirmwareLength && !retval) */
=======
static INT buffDnld(PMINI_ADAPTER Adapter, PUCHAR mappedbuffer, UINT u32FirmwareLength, ULONG u32StartingAddress)
{
=======
static INT buffDnld(struct bcm_mini_adapter *Adapter,
			PUCHAR mappedbuffer, UINT u32FirmwareLength,
			ULONG u32StartingAddress)
{
>>>>>>> refs/remotes/origin/master
	unsigned int len = 0;
	int retval = STATUS_SUCCESS;
	len = u32FirmwareLength;

	while (u32FirmwareLength) {
		len = MIN_VAL(u32FirmwareLength, MAX_TRANSFER_CTRL_BYTE_USB);
		retval = wrm(Adapter, u32StartingAddress, mappedbuffer, len);

<<<<<<< HEAD
		if (retval) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "wrm failed with status :%d", retval);
			break;
		}
=======
		if (retval)
			break;
>>>>>>> refs/remotes/origin/master
		u32StartingAddress += len;
		u32FirmwareLength -= len;
		mappedbuffer += len;
	}
	return retval;
}

<<<<<<< HEAD
static INT buffRdbkVerify(PMINI_ADAPTER Adapter, PUCHAR mappedbuffer, UINT u32FirmwareLength, ULONG u32StartingAddress)
=======
static INT buffRdbkVerify(struct bcm_mini_adapter *Adapter,
			PUCHAR mappedbuffer, UINT u32FirmwareLength,
			ULONG u32StartingAddress)
>>>>>>> refs/remotes/origin/master
{
	UINT len = u32FirmwareLength;
	INT retval = STATUS_SUCCESS;
	PUCHAR readbackbuff = kzalloc(MAX_TRANSFER_CTRL_BYTE_USB, GFP_KERNEL);
	int bytes;

<<<<<<< HEAD
	if (NULL == readbackbuff) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "MEMORY ALLOCATION FAILED");
		return -ENOMEM;
	}
=======
	if (NULL == readbackbuff)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	while (u32FirmwareLength && !retval) {
		len = MIN_VAL(u32FirmwareLength, MAX_TRANSFER_CTRL_BYTE_USB);
		bytes = rdm(Adapter, u32StartingAddress, readbackbuff, len);

		if (bytes < 0) {
			retval = bytes;
<<<<<<< HEAD
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "rdm failed with status %d", retval);
			break;
		}

		retval = bcm_compare_buff_contents(readbackbuff, mappedbuffer, len);
		if (STATUS_SUCCESS != retval)
			break;
=======
			break;
		}

		if (memcmp(readbackbuff, mappedbuffer, len) != 0) {
			pr_err("%s() failed.  The firmware doesn't match what was written",
			       __func__);
			retval = -EIO;
		}
>>>>>>> refs/remotes/origin/master

		u32StartingAddress += len;
		u32FirmwareLength -= len;
		mappedbuffer += len;

	} /* end of while (u32FirmwareLength && !retval) */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	kfree(readbackbuff);
	return retval;
}

<<<<<<< HEAD
<<<<<<< HEAD
INT buffDnldVerify(PMINI_ADAPTER Adapter, unsigned char *mappedbuffer, unsigned int u32FirmwareLength,
		unsigned long u32StartingAddress)
{
	INT status = STATUS_SUCCESS;

	status = buffDnld(Adapter,mappedbuffer,u32FirmwareLength,u32StartingAddress);
	if(status != STATUS_SUCCESS)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL,"Buffer download failed");
		goto error;
	}

	status= buffRdbkVerify(Adapter,mappedbuffer,u32FirmwareLength,u32StartingAddress);
	if(status != STATUS_SUCCESS)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL,"Buffer readback verifier failed");
=======
INT buffDnldVerify(PMINI_ADAPTER Adapter, unsigned char *mappedbuffer, unsigned int u32FirmwareLength, unsigned long u32StartingAddress)
{
	INT status = STATUS_SUCCESS;

	status = buffDnld(Adapter, mappedbuffer, u32FirmwareLength, u32StartingAddress);
	if (status != STATUS_SUCCESS) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Buffer download failed");
		goto error;
	}

	status = buffRdbkVerify(Adapter, mappedbuffer, u32FirmwareLength, u32StartingAddress);
	if (status != STATUS_SUCCESS) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_INITEXIT, MP_INIT, DBG_LVL_ALL, "Buffer readback verifier failed");
>>>>>>> refs/remotes/origin/cm-10.0
		goto error;
	}
error:
	return status;
}
<<<<<<< HEAD


=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
INT buffDnldVerify(struct bcm_mini_adapter *Adapter,
			unsigned char *mappedbuffer,
			unsigned int u32FirmwareLength,
			unsigned long u32StartingAddress)
{
	INT status = STATUS_SUCCESS;

	status = buffDnld(Adapter, mappedbuffer,
			u32FirmwareLength, u32StartingAddress);
	if (status != STATUS_SUCCESS)
		goto error;

	status = buffRdbkVerify(Adapter, mappedbuffer,
			u32FirmwareLength, u32StartingAddress);
	if (status != STATUS_SUCCESS)
		goto error;
error:
	return status;
}
>>>>>>> refs/remotes/origin/master
