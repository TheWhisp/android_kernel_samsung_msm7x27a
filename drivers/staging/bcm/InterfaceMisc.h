#ifndef __INTERFACE_MISC_H
#define __INTERFACE_MISC_H

INT
<<<<<<< HEAD
InterfaceRDM(PS_INTERFACE_ADAPTER psIntfAdapter,
=======
InterfaceRDM(struct bcm_interface_adapter *psIntfAdapter,
>>>>>>> refs/remotes/origin/master
			UINT addr,
			PVOID buff,
			INT len);

INT
<<<<<<< HEAD
InterfaceWRM(PS_INTERFACE_ADAPTER psIntfAdapter,
=======
InterfaceWRM(struct bcm_interface_adapter *psIntfAdapter,
>>>>>>> refs/remotes/origin/master
			UINT addr,
			PVOID buff,
			INT len);


int InterfaceFileDownload( PVOID psIntfAdapter,
                        struct file *flp,
                        unsigned int on_chip_loc);

int InterfaceFileReadbackFromChip( PVOID psIntfAdapter,
                        struct file *flp,
                        unsigned int on_chip_loc);


int BcmRDM(PVOID arg,
			UINT addr,
			PVOID buff,
			INT len);

int BcmWRM(PVOID arg,
			UINT addr,
			PVOID buff,
			INT len);

<<<<<<< HEAD
INT Bcm_clear_halt_of_endpoints(PMINI_ADAPTER Adapter);

VOID Bcm_kill_all_URBs(PS_INTERFACE_ADAPTER psIntfAdapter);
=======
INT Bcm_clear_halt_of_endpoints(struct bcm_mini_adapter *Adapter);

VOID Bcm_kill_all_URBs(struct bcm_interface_adapter *psIntfAdapter);
>>>>>>> refs/remotes/origin/master

#define DISABLE_USB_ZERO_LEN_INT 0x0F011878

#endif // __INTERFACE_MISC_H
