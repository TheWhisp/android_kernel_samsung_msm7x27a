/*
  This is part of the rtl8180-sa2400 driver
  released under the GPL (See file COPYING for details).
<<<<<<< HEAD
  Copyright (c) 2005 Andrea Merello <andreamrl@tiscali.it>
=======
  Copyright (c) 2005 Andrea Merello <andrea.merello@gmail.com>
>>>>>>> refs/remotes/origin/master

  This files contains programming code for the rtl8256
  radio frontend.

  *Many* thanks to Realtek Corp. for their great support!

*/

#ifndef RTL8225H
#define RTL8225H

<<<<<<< HEAD
#ifdef RTL8190P
#define RTL819X_TOTAL_RF_PATH 4 //for 90P
#else
#define RTL819X_TOTAL_RF_PATH 2 //for 8192U
#endif
extern void PHY_SetRF8256Bandwidth(struct net_device* dev , HT_CHANNEL_WIDTH Bandwidth);
extern void PHY_RF8256_Config(struct net_device* dev);
extern void phy_RF8256_Config_ParaFile(struct net_device* dev);
extern void PHY_SetRF8256CCKTxPower(struct net_device*	dev, u8	powerlevel);
extern void PHY_SetRF8256OFDMTxPower(struct net_device* dev, u8 powerlevel);
=======
#define RTL819X_TOTAL_RF_PATH 2 //for 8192U
extern void PHY_SetRF8256Bandwidth(struct net_device *dev , HT_CHANNEL_WIDTH Bandwidth);
extern void PHY_RF8256_Config(struct net_device *dev);
extern void phy_RF8256_Config_ParaFile(struct net_device *dev);
extern void PHY_SetRF8256CCKTxPower(struct net_device *dev, u8	powerlevel);
extern void PHY_SetRF8256OFDMTxPower(struct net_device *dev, u8 powerlevel);
>>>>>>> refs/remotes/origin/master

#endif
