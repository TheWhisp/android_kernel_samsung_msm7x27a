<<<<<<< HEAD
//-----------------------------------------------------------------------------
//	File:
//		Dot11d.c
//
//	Description:
//		Implement 802.11d.
//
//-----------------------------------------------------------------------------

#include "dot11d.h"

void
Dot11d_Init(struct ieee80211_device *ieee)
=======
#include "dot11d.h"

void Dot11d_Init(struct ieee80211_device *ieee)
>>>>>>> refs/remotes/origin/master
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(ieee);

	pDot11dInfo->bEnabled = 0;

	pDot11dInfo->State = DOT11D_STATE_NONE;
	pDot11dInfo->CountryIeLen = 0;
	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER+1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER+1);
	RESET_CIE_WATCHDOG(ieee);

<<<<<<< HEAD
	printk("Dot11d_Init()\n");
}

//
//	Description:
//		Reset to the state as we are just entering a regulatory domain.
//
void
Dot11d_Reset(struct ieee80211_device *ieee)
=======
	netdev_info(ieee->dev, "Dot11d_Init()\n");
}

/* Reset to the state as we are just entering a regulatory domain. */
void Dot11d_Reset(struct ieee80211_device *ieee)
>>>>>>> refs/remotes/origin/master
{
	u32 i;
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(ieee);

<<<<<<< HEAD
	// Clear old channel map
	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER+1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER+1);
	// Set new channel map
	for (i=1; i<=11; i++) {
		(pDot11dInfo->channel_map)[i] = 1;
	}
	for (i=12; i<=14; i++) {
		(pDot11dInfo->channel_map)[i] = 2;
	}
=======
	/* Clear old channel map */
	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER+1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER+1);
	/* Set new channel map */
	for (i = 1; i <= 11; i++)
		(pDot11dInfo->channel_map)[i] = 1;

	for (i = 12; i <= 14; i++)
		(pDot11dInfo->channel_map)[i] = 2;
>>>>>>> refs/remotes/origin/master

	pDot11dInfo->State = DOT11D_STATE_NONE;
	pDot11dInfo->CountryIeLen = 0;
	RESET_CIE_WATCHDOG(ieee);
<<<<<<< HEAD

	//printk("Dot11d_Reset()\n");
}

//
//	Description:
//		Update country IE from Beacon or Probe Resopnse
//		and configure PHY for operation in the regulatory domain.
//
//	TODO:
//		Configure Tx power.
//
//	Assumption:
//		1. IS_DOT11D_ENABLE() is TRUE.
//		2. Input IE is an valid one.
//
void
Dot11d_UpdateCountryIe(
	struct ieee80211_device *dev,
	u8 *		pTaddr,
	u16	CoutryIeLen,
	u8 * pCoutryIe
	)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 i, j, NumTriples, MaxChnlNum;
	PCHNL_TXPOWER_TRIPLE pTriple;

	if((CoutryIeLen - 3)%3 != 0)
	{
		printk("Dot11d_UpdateCountryIe(): Invalid country IE, skip it........1\n");
=======
}

/*
 * Description:
 *	Update country IE from Beacon or Probe Response and configure PHY for
 *	operation in the regulatory domain.
 *
 * TODO:
 *	Configure Tx power.
 *
 * Assumption:
 *	1. IS_DOT11D_ENABLE() is TRUE.
 *	2. Input IE is an valid one.
 */
void Dot11d_UpdateCountryIe(struct ieee80211_device *dev, u8 *pTaddr,
			    u16 CoutryIeLen, u8 *pCoutryIe)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 i, j, NumTriples, MaxChnlNum;
	u8 index, MaxTxPowerInDbm;
	PCHNL_TXPOWER_TRIPLE pTriple;

	if ((CoutryIeLen - 3)%3 != 0) {
		netdev_info(dev->dev, "Dot11d_UpdateCountryIe(): Invalid country IE, skip it........1\n");
>>>>>>> refs/remotes/origin/master
		Dot11d_Reset(dev);
		return;
	}

	memset(pDot11dInfo->channel_map, 0, MAX_CHANNEL_NUMBER+1);
	memset(pDot11dInfo->MaxTxPwrDbmList, 0xFF, MAX_CHANNEL_NUMBER+1);
	MaxChnlNum = 0;
<<<<<<< HEAD
	NumTriples = (CoutryIeLen - 3) / 3; // skip 3-byte country string.
	pTriple = (PCHNL_TXPOWER_TRIPLE)(pCoutryIe + 3);
	for(i = 0; i < NumTriples; i++)
	{
		if(MaxChnlNum >= pTriple->FirstChnl)
		{ // It is not in a monotonically increasing order, so stop processing.
			printk("Dot11d_UpdateCountryIe(): Invalid country IE, skip it........1\n");
			Dot11d_Reset(dev);
			return;
		}
		if(MAX_CHANNEL_NUMBER < (pTriple->FirstChnl + pTriple->NumChnls))
		{ // It is not a valid set of channel id, so stop processing.
			printk("Dot11d_UpdateCountryIe(): Invalid country IE, skip it........2\n");
=======
	NumTriples = (CoutryIeLen - 3) / 3; /* skip 3-byte country string. */
	pTriple = (PCHNL_TXPOWER_TRIPLE)(pCoutryIe + 3);
	for (i = 0; i < NumTriples; i++) {
		if (MaxChnlNum >= pTriple->FirstChnl) {
			/*
			 * It is not in a monotonically increasing order,
			 * so stop processing.
			 */
			netdev_info(dev->dev,
				    "Dot11d_UpdateCountryIe(): Invalid country IE, skip it........1\n");
			Dot11d_Reset(dev);
			return;
		}
		if (MAX_CHANNEL_NUMBER <
		    (pTriple->FirstChnl + pTriple->NumChnls)) {
			/*
			 * It is not a valid set of channel id,
			 * so stop processing
			 */
			netdev_info(dev->dev,
				    "Dot11d_UpdateCountryIe(): Invalid country IE, skip it........2\n");
>>>>>>> refs/remotes/origin/master
			Dot11d_Reset(dev);
			return;
		}

<<<<<<< HEAD
		for(j = 0 ; j < pTriple->NumChnls; j++)
		{
			pDot11dInfo->channel_map[pTriple->FirstChnl + j] = 1;
			pDot11dInfo->MaxTxPwrDbmList[pTriple->FirstChnl + j] = pTriple->MaxTxPowerInDbm;
			MaxChnlNum = pTriple->FirstChnl + j;
		}

		pTriple = (PCHNL_TXPOWER_TRIPLE)((u8*)pTriple + 3);
	}
#if 1
	//printk("Dot11d_UpdateCountryIe(): Channel List:\n");
	printk("Channel List:");
	for(i=1; i<= MAX_CHANNEL_NUMBER; i++)
		if(pDot11dInfo->channel_map[i] > 0)
			printk(" %d", i);
	printk("\n");
=======
		for (j = 0; j < pTriple->NumChnls; j++) {
			index = pTriple->FirstChnl + j;
			pDot11dInfo->channel_map[index] = 1;
			MaxTxPowerInDbm = pTriple->MaxTxPowerInDbm;
			pDot11dInfo->MaxTxPwrDbmList[index] = MaxTxPowerInDbm;
			MaxChnlNum = pTriple->FirstChnl + j;
		}

		pTriple = (PCHNL_TXPOWER_TRIPLE)((u8 *)pTriple + 3);
	}
#if 1
	netdev_info(dev->dev, "Channel List:");
	for (i = 1; i <= MAX_CHANNEL_NUMBER; i++)
		if (pDot11dInfo->channel_map[i] > 0)
			netdev_info(dev->dev, " %d", i);
	netdev_info(dev->dev, "\n");
>>>>>>> refs/remotes/origin/master
#endif

	UPDATE_CIE_SRC(dev, pTaddr);

	pDot11dInfo->CountryIeLen = CoutryIeLen;
<<<<<<< HEAD
	memcpy(pDot11dInfo->CountryIeBuf, pCoutryIe,CoutryIeLen);
	pDot11dInfo->State = DOT11D_STATE_LEARNED;
}

u8
DOT11D_GetMaxTxPwrInDbm(
	struct ieee80211_device *dev,
	u8 Channel
	)
=======
	memcpy(pDot11dInfo->CountryIeBuf, pCoutryIe, CoutryIeLen);
	pDot11dInfo->State = DOT11D_STATE_LEARNED;
}

u8 DOT11D_GetMaxTxPwrInDbm(struct ieee80211_device *dev, u8 Channel)
>>>>>>> refs/remotes/origin/master
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 MaxTxPwrInDbm = 255;

<<<<<<< HEAD
	if(MAX_CHANNEL_NUMBER < Channel)
	{
		printk("DOT11D_GetMaxTxPwrInDbm(): Invalid Channel\n");
		return MaxTxPwrInDbm;
	}
	if(pDot11dInfo->channel_map[Channel])
	{
		MaxTxPwrInDbm = pDot11dInfo->MaxTxPwrDbmList[Channel];
	}
=======
	if (MAX_CHANNEL_NUMBER < Channel) {
		netdev_info(dev->dev, "DOT11D_GetMaxTxPwrInDbm(): Invalid Channel\n");
		return MaxTxPwrInDbm;
	}
	if (pDot11dInfo->channel_map[Channel])
		MaxTxPwrInDbm = pDot11dInfo->MaxTxPwrDbmList[Channel];
>>>>>>> refs/remotes/origin/master

	return MaxTxPwrInDbm;
}


<<<<<<< HEAD
void
DOT11D_ScanComplete(
	struct ieee80211_device * dev
	)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);

	switch(pDot11dInfo->State)
	{
=======
void DOT11D_ScanComplete(struct ieee80211_device *dev)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);

	switch (pDot11dInfo->State) {
>>>>>>> refs/remotes/origin/master
	case DOT11D_STATE_LEARNED:
		pDot11dInfo->State = DOT11D_STATE_DONE;
		break;

	case DOT11D_STATE_DONE:
<<<<<<< HEAD
		if( GET_CIE_WATCHDOG(dev) == 0 )
		{ // Reset country IE if previous one is gone.
=======
		if (GET_CIE_WATCHDOG(dev) == 0) {
			/* Reset country IE if previous one is gone. */
>>>>>>> refs/remotes/origin/master
			Dot11d_Reset(dev);
		}
		break;
	case DOT11D_STATE_NONE:
		break;
	}
}

<<<<<<< HEAD
int IsLegalChannel(
	struct ieee80211_device * dev,
	u8 channel
)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);

	if(MAX_CHANNEL_NUMBER < channel)
	{
		printk("IsLegalChannel(): Invalid Channel\n");
		return 0;
	}
	if(pDot11dInfo->channel_map[channel] > 0)
=======
int IsLegalChannel(struct ieee80211_device *dev, u8 channel)
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);

	if (MAX_CHANNEL_NUMBER < channel) {
		netdev_info(dev->dev, "IsLegalChannel(): Invalid Channel\n");
		return 0;
	}
	if (pDot11dInfo->channel_map[channel] > 0)
>>>>>>> refs/remotes/origin/master
		return 1;
	return 0;
}

<<<<<<< HEAD
int ToLegalChannel(
	struct ieee80211_device * dev,
	u8 channel
)
=======
int ToLegalChannel(struct ieee80211_device *dev, u8 channel)
>>>>>>> refs/remotes/origin/master
{
	PRT_DOT11D_INFO pDot11dInfo = GET_DOT11D_INFO(dev);
	u8 default_chn = 0;
	u32 i = 0;

<<<<<<< HEAD
	for (i=1; i<= MAX_CHANNEL_NUMBER; i++)
	{
		if(pDot11dInfo->channel_map[i] > 0)
		{
=======
	for (i = 1; i <= MAX_CHANNEL_NUMBER; i++) {
		if (pDot11dInfo->channel_map[i] > 0) {
>>>>>>> refs/remotes/origin/master
			default_chn = i;
			break;
		}
	}

<<<<<<< HEAD
	if(MAX_CHANNEL_NUMBER < channel)
	{
		printk("IsLegalChannel(): Invalid Channel\n");
		return default_chn;
	}

	if(pDot11dInfo->channel_map[channel] > 0)
=======
	if (MAX_CHANNEL_NUMBER < channel) {
		netdev_info(dev->dev, "IsLegalChannel(): Invalid Channel\n");
		return default_chn;
	}

	if (pDot11dInfo->channel_map[channel] > 0)
>>>>>>> refs/remotes/origin/master
		return channel;

	return default_chn;
}
