#ifndef _INTERFACE_ISR_H
#define _INTERFACE_ISR_H

<<<<<<< HEAD
int CreateInterruptUrb(PS_INTERFACE_ADAPTER psIntfAdapter);


INT StartInterruptUrb(PS_INTERFACE_ADAPTER psIntfAdapter);


VOID InterfaceEnableInterrupt(PMINI_ADAPTER Adapter);

VOID InterfaceDisableInterrupt(PMINI_ADAPTER Adapter);
=======
int CreateInterruptUrb(struct bcm_interface_adapter *psIntfAdapter);


INT StartInterruptUrb(struct bcm_interface_adapter *psIntfAdapter);


VOID InterfaceEnableInterrupt(struct bcm_mini_adapter *Adapter);

VOID InterfaceDisableInterrupt(struct bcm_mini_adapter *Adapter);
>>>>>>> refs/remotes/origin/master

#endif

