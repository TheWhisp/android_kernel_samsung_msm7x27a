/**
@file Qos.C
This file contains the routines related to Quality of Service.
*/
#include "headers.h"

<<<<<<< HEAD
static void EThCSGetPktInfo(PMINI_ADAPTER Adapter,PVOID pvEthPayload,PS_ETHCS_PKT_INFO pstEthCsPktInfo);
static BOOLEAN EThCSClassifyPkt(PMINI_ADAPTER Adapter,struct sk_buff* skb,PS_ETHCS_PKT_INFO pstEthCsPktInfo,S_CLASSIFIER_RULE *pstClassifierRule, B_UINT8 EthCSCupport);

static USHORT	IpVersion4(PMINI_ADAPTER Adapter, struct iphdr *iphd,
			   S_CLASSIFIER_RULE *pstClassifierRule );

static VOID PruneQueue(PMINI_ADAPTER Adapter, INT iIndex);
=======
static void EThCSGetPktInfo(struct bcm_mini_adapter *Adapter, PVOID pvEthPayload, struct bcm_eth_packet_info *pstEthCsPktInfo);
static bool EThCSClassifyPkt(struct bcm_mini_adapter *Adapter, struct sk_buff* skb, struct bcm_eth_packet_info *pstEthCsPktInfo, struct bcm_classifier_rule *pstClassifierRule, B_UINT8 EthCSCupport);

static USHORT	IpVersion4(struct bcm_mini_adapter *Adapter, struct iphdr *iphd,
			   struct bcm_classifier_rule *pstClassifierRule);

static VOID PruneQueue(struct bcm_mini_adapter *Adapter, INT iIndex);
>>>>>>> refs/remotes/origin/master


/*******************************************************************
* Function    - MatchSrcIpAddress()
*
* Description - Checks whether the Source IP address from the packet
*				matches with that of Queue.
*
* Parameters  - pstClassifierRule: Pointer to the packet info structure.
<<<<<<< HEAD
* 			  - ulSrcIP	    : Source IP address from the packet.
*
* Returns     - TRUE(If address matches) else FAIL .
*********************************************************************/
BOOLEAN MatchSrcIpAddress(S_CLASSIFIER_RULE *pstClassifierRule,ULONG ulSrcIP)
{
    UCHAR 	ucLoopIndex=0;

    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);

    ulSrcIP=ntohl(ulSrcIP);
    if(0 == pstClassifierRule->ucIPSourceAddressLength)
       	return TRUE;
    for(ucLoopIndex=0; ucLoopIndex < (pstClassifierRule->ucIPSourceAddressLength);ucLoopIndex++)
    {
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Src Ip Address Mask:0x%x PacketIp:0x%x and Classification:0x%x", (UINT)pstClassifierRule->stSrcIpAddress.ulIpv4Mask[ucLoopIndex], (UINT)ulSrcIP, (UINT)pstClassifierRule->stSrcIpAddress.ulIpv6Addr[ucLoopIndex]);
		if((pstClassifierRule->stSrcIpAddress.ulIpv4Mask[ucLoopIndex] & ulSrcIP)==
				(pstClassifierRule->stSrcIpAddress.ulIpv4Addr[ucLoopIndex] & pstClassifierRule->stSrcIpAddress.ulIpv4Mask[ucLoopIndex] ))
       	{
       		return TRUE;
       	}
    }
    BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Src Ip Address Not Matched");
   	return FALSE;
=======
*		- ulSrcIP	    : Source IP address from the packet.
*
* Returns     - TRUE(If address matches) else FAIL .
*********************************************************************/
static bool MatchSrcIpAddress(struct bcm_classifier_rule *pstClassifierRule, ULONG ulSrcIP)
{
	UCHAR ucLoopIndex = 0;

	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	ulSrcIP = ntohl(ulSrcIP);
	if (0 == pstClassifierRule->ucIPSourceAddressLength)
		return TRUE;
	for (ucLoopIndex = 0; ucLoopIndex < (pstClassifierRule->ucIPSourceAddressLength); ucLoopIndex++)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Src Ip Address Mask:0x%x PacketIp:0x%x and Classification:0x%x", (UINT)pstClassifierRule->stSrcIpAddress.ulIpv4Mask[ucLoopIndex], (UINT)ulSrcIP, (UINT)pstClassifierRule->stSrcIpAddress.ulIpv6Addr[ucLoopIndex]);
		if ((pstClassifierRule->stSrcIpAddress.ulIpv4Mask[ucLoopIndex] & ulSrcIP) ==
				(pstClassifierRule->stSrcIpAddress.ulIpv4Addr[ucLoopIndex] & pstClassifierRule->stSrcIpAddress.ulIpv4Mask[ucLoopIndex]))
		{
			return TRUE;
		}
	}
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Src Ip Address Not Matched");
	return false;
>>>>>>> refs/remotes/origin/master
}


/*******************************************************************
* Function    - MatchDestIpAddress()
*
* Description - Checks whether the Destination IP address from the packet
*				matches with that of Queue.
*
* Parameters  - pstClassifierRule: Pointer to the packet info structure.
<<<<<<< HEAD
* 			  - ulDestIP    : Destination IP address from the packet.
*
* Returns     - TRUE(If address matches) else FAIL .
*********************************************************************/
BOOLEAN MatchDestIpAddress(S_CLASSIFIER_RULE *pstClassifierRule,ULONG ulDestIP)
{
	UCHAR 	ucLoopIndex=0;
    PMINI_ADAPTER	Adapter = GET_BCM_ADAPTER(gblpnetdev);

	ulDestIP=ntohl(ulDestIP);
    if(0 == pstClassifierRule->ucIPDestinationAddressLength)
       	return TRUE;
	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Destination Ip Address 0x%x 0x%x 0x%x  ", (UINT)ulDestIP, (UINT)pstClassifierRule->stDestIpAddress.ulIpv4Mask[ucLoopIndex], (UINT)pstClassifierRule->stDestIpAddress.ulIpv4Addr[ucLoopIndex]);

    for(ucLoopIndex=0;ucLoopIndex<(pstClassifierRule->ucIPDestinationAddressLength);ucLoopIndex++)
    {
		if((pstClassifierRule->stDestIpAddress.ulIpv4Mask[ucLoopIndex] & ulDestIP)==
					(pstClassifierRule->stDestIpAddress.ulIpv4Addr[ucLoopIndex] & pstClassifierRule->stDestIpAddress.ulIpv4Mask[ucLoopIndex]))
       	{
       		return TRUE;
       	}
    }
	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Destination Ip Address Not Matched");
    return FALSE;
=======
*		- ulDestIP    : Destination IP address from the packet.
*
* Returns     - TRUE(If address matches) else FAIL .
*********************************************************************/
static bool MatchDestIpAddress(struct bcm_classifier_rule *pstClassifierRule, ULONG ulDestIP)
{
	UCHAR ucLoopIndex = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	ulDestIP = ntohl(ulDestIP);
	if (0 == pstClassifierRule->ucIPDestinationAddressLength)
		return TRUE;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Destination Ip Address 0x%x 0x%x 0x%x  ", (UINT)ulDestIP, (UINT)pstClassifierRule->stDestIpAddress.ulIpv4Mask[ucLoopIndex], (UINT)pstClassifierRule->stDestIpAddress.ulIpv4Addr[ucLoopIndex]);

	for (ucLoopIndex = 0; ucLoopIndex < (pstClassifierRule->ucIPDestinationAddressLength); ucLoopIndex++)
	{
		if ((pstClassifierRule->stDestIpAddress.ulIpv4Mask[ucLoopIndex] & ulDestIP) ==
				(pstClassifierRule->stDestIpAddress.ulIpv4Addr[ucLoopIndex] & pstClassifierRule->stDestIpAddress.ulIpv4Mask[ucLoopIndex]))
		{
			return TRUE;
		}
	}
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Destination Ip Address Not Matched");
	return false;
>>>>>>> refs/remotes/origin/master
}


/************************************************************************
* Function    - MatchTos()
*
* Description - Checks the TOS from the packet matches with that of queue.
*
* Parameters  - pstClassifierRule   : Pointer to the packet info structure.
<<<<<<< HEAD
* 			  - ucTypeOfService: TOS from the packet.
*
* Returns     - TRUE(If address matches) else FAIL.
**************************************************************************/
BOOLEAN MatchTos(S_CLASSIFIER_RULE *pstClassifierRule,UCHAR ucTypeOfService)
{

	PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
    if( 3 != pstClassifierRule->ucIPTypeOfServiceLength )
       	return TRUE;

    if(((pstClassifierRule->ucTosMask & ucTypeOfService)<=pstClassifierRule->ucTosHigh) && ((pstClassifierRule->ucTosMask & ucTypeOfService)>=pstClassifierRule->ucTosLow))
    {
       	return TRUE;
    }
    BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Type Of Service Not Matched");
    return FALSE;
=======
*		- ucTypeOfService: TOS from the packet.
*
* Returns     - TRUE(If address matches) else FAIL.
**************************************************************************/
static bool MatchTos(struct bcm_classifier_rule *pstClassifierRule, UCHAR ucTypeOfService)
{

	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if (3 != pstClassifierRule->ucIPTypeOfServiceLength)
		return TRUE;

	if (((pstClassifierRule->ucTosMask & ucTypeOfService) <= pstClassifierRule->ucTosHigh) && ((pstClassifierRule->ucTosMask & ucTypeOfService) >= pstClassifierRule->ucTosLow))
	{
		return TRUE;
	}
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Type Of Service Not Matched");
	return false;
>>>>>>> refs/remotes/origin/master
}


/***************************************************************************
* Function    - MatchProtocol()
*
* Description - Checks the protocol from the packet matches with that of queue.
*
* Parameters  - pstClassifierRule: Pointer to the packet info structure.
<<<<<<< HEAD
* 			  - ucProtocol	: Protocol from the packet.
*
* Returns     - TRUE(If address matches) else FAIL.
****************************************************************************/
BOOLEAN MatchProtocol(S_CLASSIFIER_RULE *pstClassifierRule,UCHAR ucProtocol)
{
   	UCHAR 	ucLoopIndex=0;
	PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if(0 == pstClassifierRule->ucProtocolLength)
      	return TRUE;
	for(ucLoopIndex=0;ucLoopIndex<pstClassifierRule->ucProtocolLength;ucLoopIndex++)
    {
       	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Protocol:0x%X Classification Protocol:0x%X",ucProtocol,pstClassifierRule->ucProtocol[ucLoopIndex]);
       	if(pstClassifierRule->ucProtocol[ucLoopIndex]==ucProtocol)
       	{
       		return TRUE;
       	}
    }
    BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Protocol Not Matched");
   	return FALSE;
=======
*		- ucProtocol	: Protocol from the packet.
*
* Returns     - TRUE(If address matches) else FAIL.
****************************************************************************/
bool MatchProtocol(struct bcm_classifier_rule *pstClassifierRule, UCHAR ucProtocol)
{
	UCHAR ucLoopIndex = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if (0 == pstClassifierRule->ucProtocolLength)
		return TRUE;
	for (ucLoopIndex = 0; ucLoopIndex < pstClassifierRule->ucProtocolLength; ucLoopIndex++)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Protocol:0x%X Classification Protocol:0x%X", ucProtocol, pstClassifierRule->ucProtocol[ucLoopIndex]);
		if (pstClassifierRule->ucProtocol[ucLoopIndex] == ucProtocol)
		{
			return TRUE;
		}
	}
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Protocol Not Matched");
	return false;
>>>>>>> refs/remotes/origin/master
}


/***********************************************************************
* Function    - MatchSrcPort()
*
* Description - Checks, Source port from the packet matches with that of queue.
*
* Parameters  - pstClassifierRule: Pointer to the packet info structure.
<<<<<<< HEAD
* 			  - ushSrcPort	: Source port from the packet.
*
* Returns     - TRUE(If address matches) else FAIL.
***************************************************************************/
BOOLEAN MatchSrcPort(S_CLASSIFIER_RULE *pstClassifierRule,USHORT ushSrcPort)
{
    	UCHAR 	ucLoopIndex=0;

		PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);


    	if(0 == pstClassifierRule->ucSrcPortRangeLength)
        	return TRUE;
    	for(ucLoopIndex=0;ucLoopIndex<pstClassifierRule->ucSrcPortRangeLength;ucLoopIndex++)
    	{
        	if(ushSrcPort <= pstClassifierRule->usSrcPortRangeHi[ucLoopIndex] &&
		    ushSrcPort >= pstClassifierRule->usSrcPortRangeLo[ucLoopIndex])
	    	{
		    	return TRUE;
	    	}
    	}
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Src Port: %x Not Matched ",ushSrcPort);
    	return FALSE;
=======
*		- ushSrcPort	: Source port from the packet.
*
* Returns     - TRUE(If address matches) else FAIL.
***************************************************************************/
bool MatchSrcPort(struct bcm_classifier_rule *pstClassifierRule, USHORT ushSrcPort)
{
	UCHAR ucLoopIndex = 0;

	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);


	if (0 == pstClassifierRule->ucSrcPortRangeLength)
		return TRUE;
	for (ucLoopIndex = 0; ucLoopIndex < pstClassifierRule->ucSrcPortRangeLength; ucLoopIndex++)
	{
		if (ushSrcPort <= pstClassifierRule->usSrcPortRangeHi[ucLoopIndex] &&
			ushSrcPort >= pstClassifierRule->usSrcPortRangeLo[ucLoopIndex])
		{
			return TRUE;
		}
	}
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Src Port: %x Not Matched ", ushSrcPort);
	return false;
>>>>>>> refs/remotes/origin/master
}


/***********************************************************************
* Function    - MatchDestPort()
*
* Description - Checks, Destination port from packet matches with that of queue.
*
* Parameters  - pstClassifierRule: Pointer to the packet info structure.
<<<<<<< HEAD
* 			  - ushDestPort	: Destination port from the packet.
*
* Returns     - TRUE(If address matches) else FAIL.
***************************************************************************/
BOOLEAN MatchDestPort(S_CLASSIFIER_RULE *pstClassifierRule,USHORT ushDestPort)
{
    	UCHAR 	ucLoopIndex=0;
		PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);

    	if(0 == pstClassifierRule->ucDestPortRangeLength)
        	return TRUE;

    	for(ucLoopIndex=0;ucLoopIndex<pstClassifierRule->ucDestPortRangeLength;ucLoopIndex++)
    	{
        	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Matching Port:0x%X   0x%X  0x%X",ushDestPort,pstClassifierRule->usDestPortRangeLo[ucLoopIndex],pstClassifierRule->usDestPortRangeHi[ucLoopIndex]);

 		if(ushDestPort <= pstClassifierRule->usDestPortRangeHi[ucLoopIndex] &&
		    ushDestPort >= pstClassifierRule->usDestPortRangeLo[ucLoopIndex])
	    	{
		    return TRUE;
	    	}
    	}
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Dest Port: %x Not Matched",ushDestPort);
    	return FALSE;
=======
*		- ushDestPort	: Destination port from the packet.
*
* Returns     - TRUE(If address matches) else FAIL.
***************************************************************************/
bool MatchDestPort(struct bcm_classifier_rule *pstClassifierRule, USHORT ushDestPort)
{
	UCHAR ucLoopIndex = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	if (0 == pstClassifierRule->ucDestPortRangeLength)
		return TRUE;

	for (ucLoopIndex = 0; ucLoopIndex < pstClassifierRule->ucDestPortRangeLength; ucLoopIndex++)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Matching Port:0x%X   0x%X  0x%X", ushDestPort, pstClassifierRule->usDestPortRangeLo[ucLoopIndex], pstClassifierRule->usDestPortRangeHi[ucLoopIndex]);

		if (ushDestPort <= pstClassifierRule->usDestPortRangeHi[ucLoopIndex] &&
			ushDestPort >= pstClassifierRule->usDestPortRangeLo[ucLoopIndex])
		{
			return TRUE;
		}
	}
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Dest Port: %x Not Matched", ushDestPort);
	return false;
>>>>>>> refs/remotes/origin/master
}
/**
@ingroup tx_functions
Compares IPV4 Ip address and port number
@return Queue Index.
*/
<<<<<<< HEAD
static USHORT	IpVersion4(PMINI_ADAPTER Adapter,
			   struct iphdr *iphd,
			   S_CLASSIFIER_RULE *pstClassifierRule )
{
	xporthdr     		*xprt_hdr=NULL;
	BOOLEAN	bClassificationSucceed=FALSE;

	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "========>");

	xprt_hdr=(xporthdr *)((PUCHAR)iphd + sizeof(struct iphdr));

	do {
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Trying to see Direction = %d %d",
=======
static USHORT	IpVersion4(struct bcm_mini_adapter *Adapter,
			   struct iphdr *iphd,
			   struct bcm_classifier_rule *pstClassifierRule)
{
	struct bcm_transport_header *xprt_hdr = NULL;
	bool	bClassificationSucceed = false;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "========>");

	xprt_hdr = (struct bcm_transport_header *)((PUCHAR)iphd + sizeof(struct iphdr));

	do {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Trying to see Direction = %d %d",
>>>>>>> refs/remotes/origin/master
			pstClassifierRule->ucDirection,
			pstClassifierRule->usVCID_Value);

		//Checking classifier validity
<<<<<<< HEAD
		if(!pstClassifierRule->bUsed || pstClassifierRule->ucDirection == DOWNLINK_DIR)
		{
			bClassificationSucceed = FALSE;
			break;
		}

		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "is IPv6 check!");
		if(pstClassifierRule->bIpv6Protocol)
			break;

		//**************Checking IP header parameter**************************//
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Trying to match Source IP Address");
		if(FALSE == (bClassificationSucceed =
			MatchSrcIpAddress(pstClassifierRule, iphd->saddr)))
			break;
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Source IP Address Matched");

		if(FALSE == (bClassificationSucceed =
			MatchDestIpAddress(pstClassifierRule, iphd->daddr)))
			break;
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Destination IP Address Matched");

		if(FALSE == (bClassificationSucceed =
			MatchTos(pstClassifierRule, iphd->tos)))
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "TOS Match failed\n");
			break;
		}
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "TOS Matched");

		if(FALSE == (bClassificationSucceed =
			MatchProtocol(pstClassifierRule,iphd->protocol)))
			break;
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Protocol Matched");

		//if protocol is not TCP or UDP then no need of comparing source port and destination port
		if(iphd->protocol!=TCP && iphd->protocol!=UDP)
			break;
		//******************Checking Transport Layer Header field if present *****************//
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Source Port %04x",
			(iphd->protocol==UDP)?xprt_hdr->uhdr.source:xprt_hdr->thdr.source);

		if(FALSE == (bClassificationSucceed =
			MatchSrcPort(pstClassifierRule,
				ntohs((iphd->protocol == UDP)?
				xprt_hdr->uhdr.source:xprt_hdr->thdr.source))))
			break;
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Src Port Matched");

		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Destination Port %04x",
			(iphd->protocol==UDP)?xprt_hdr->uhdr.dest:
			xprt_hdr->thdr.dest);
		if(FALSE == (bClassificationSucceed =
			MatchDestPort(pstClassifierRule,
			ntohs((iphd->protocol == UDP)?
			xprt_hdr->uhdr.dest:xprt_hdr->thdr.dest))))
			break;
	} while(0);

	if(TRUE==bClassificationSucceed)
	{
		INT iMatchedSFQueueIndex = 0;
		iMatchedSFQueueIndex = SearchSfid(Adapter,pstClassifierRule->ulSFID);
		if(iMatchedSFQueueIndex >= NO_OF_QUEUES)
		{
			bClassificationSucceed = FALSE;
		}
		else
		{
			if(FALSE == Adapter->PackInfo[iMatchedSFQueueIndex].bActive)
			{
				bClassificationSucceed = FALSE;
=======
		if (!pstClassifierRule->bUsed || pstClassifierRule->ucDirection == DOWNLINK_DIR)
		{
			bClassificationSucceed = false;
			break;
		}

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "is IPv6 check!");
		if (pstClassifierRule->bIpv6Protocol)
			break;

		//**************Checking IP header parameter**************************//
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Trying to match Source IP Address");
		if (false == (bClassificationSucceed =
			MatchSrcIpAddress(pstClassifierRule, iphd->saddr)))
			break;
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Source IP Address Matched");

		if (false == (bClassificationSucceed =
			MatchDestIpAddress(pstClassifierRule, iphd->daddr)))
			break;
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Destination IP Address Matched");

		if (false == (bClassificationSucceed =
			MatchTos(pstClassifierRule, iphd->tos)))
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "TOS Match failed\n");
			break;
		}
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "TOS Matched");

		if (false == (bClassificationSucceed =
			MatchProtocol(pstClassifierRule, iphd->protocol)))
			break;
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Protocol Matched");

		//if protocol is not TCP or UDP then no need of comparing source port and destination port
		if (iphd->protocol != TCP && iphd->protocol != UDP)
			break;
		//******************Checking Transport Layer Header field if present *****************//
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Source Port %04x",
			(iphd->protocol == UDP) ? xprt_hdr->uhdr.source : xprt_hdr->thdr.source);

		if (false == (bClassificationSucceed =
			MatchSrcPort(pstClassifierRule,
				ntohs((iphd->protocol == UDP) ?
				xprt_hdr->uhdr.source : xprt_hdr->thdr.source))))
			break;
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Src Port Matched");

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Destination Port %04x",
			(iphd->protocol == UDP) ? xprt_hdr->uhdr.dest :
			xprt_hdr->thdr.dest);
		if (false == (bClassificationSucceed =
			MatchDestPort(pstClassifierRule,
			ntohs((iphd->protocol == UDP) ?
			xprt_hdr->uhdr.dest : xprt_hdr->thdr.dest))))
			break;
	} while (0);

	if (TRUE == bClassificationSucceed)
	{
		INT iMatchedSFQueueIndex = 0;
		iMatchedSFQueueIndex = SearchSfid(Adapter, pstClassifierRule->ulSFID);
		if (iMatchedSFQueueIndex >= NO_OF_QUEUES)
		{
			bClassificationSucceed = false;
		}
		else
		{
			if (false == Adapter->PackInfo[iMatchedSFQueueIndex].bActive)
			{
				bClassificationSucceed = false;
>>>>>>> refs/remotes/origin/master
			}
		}
	}

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "IpVersion4 <==========");
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "IpVersion4 <==========");
>>>>>>> refs/remotes/origin/master

	return bClassificationSucceed;
}

<<<<<<< HEAD
VOID PruneQueueAllSF(PMINI_ADAPTER Adapter)
{
	UINT iIndex = 0;

	for(iIndex = 0; iIndex < HiPriority; iIndex++)
	{
		if(!Adapter->PackInfo[iIndex].bValid)
=======
VOID PruneQueueAllSF(struct bcm_mini_adapter *Adapter)
{
	UINT iIndex = 0;

	for (iIndex = 0; iIndex < HiPriority; iIndex++)
	{
		if (!Adapter->PackInfo[iIndex].bValid)
>>>>>>> refs/remotes/origin/master
			continue;

		PruneQueue(Adapter, iIndex);
	}
}


/**
@ingroup tx_functions
This function checks if the max queue size for a queue
is less than number of bytes in the queue. If so -
drops packets from the Head till the number of bytes is
less than or equal to max queue size for the queue.
*/
<<<<<<< HEAD
static VOID PruneQueue(PMINI_ADAPTER Adapter, INT iIndex)
{
	struct sk_buff* PacketToDrop=NULL;
	struct net_device_stats *netstats;

	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "=====> Index %d",iIndex);

   	if(iIndex == HiPriority)
		return;

	if(!Adapter || (iIndex < 0) || (iIndex > HiPriority))
=======
static VOID PruneQueue(struct bcm_mini_adapter *Adapter, INT iIndex)
{
	struct sk_buff* PacketToDrop = NULL;
	struct net_device_stats *netstats;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "=====> Index %d", iIndex);

	if (iIndex == HiPriority)
		return;

	if (!Adapter || (iIndex < 0) || (iIndex > HiPriority))
>>>>>>> refs/remotes/origin/master
		return;

	/* To Store the netdevice statistic */
	netstats = &Adapter->dev->stats;

	spin_lock_bh(&Adapter->PackInfo[iIndex].SFQueueLock);

<<<<<<< HEAD
	while(1)
//	while((UINT)Adapter->PackInfo[iIndex].uiCurrentPacketsOnHost >
//		SF_MAX_ALLOWED_PACKETS_TO_BACKUP)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "uiCurrentBytesOnHost:%x uiMaxBucketSize :%x",
=======
	while (1)
//	while((UINT)Adapter->PackInfo[iIndex].uiCurrentPacketsOnHost >
//		SF_MAX_ALLOWED_PACKETS_TO_BACKUP)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "uiCurrentBytesOnHost:%x uiMaxBucketSize :%x",
>>>>>>> refs/remotes/origin/master
		Adapter->PackInfo[iIndex].uiCurrentBytesOnHost,
		Adapter->PackInfo[iIndex].uiMaxBucketSize);

		PacketToDrop = Adapter->PackInfo[iIndex].FirstTxQueue;

<<<<<<< HEAD
		if(PacketToDrop == NULL)
			break;
		if((Adapter->PackInfo[iIndex].uiCurrentPacketsOnHost < SF_MAX_ALLOWED_PACKETS_TO_BACKUP) &&
			((1000*(jiffies - *((B_UINT32 *)(PacketToDrop->cb)+SKB_CB_LATENCY_OFFSET))/HZ) <= Adapter->PackInfo[iIndex].uiMaxLatency))
			break;

		if(PacketToDrop)
		{
			if (netif_msg_tx_err(Adapter))
				pr_info(PFX "%s: tx queue %d overlimit\n", 
=======
		if (PacketToDrop == NULL)
			break;
		if ((Adapter->PackInfo[iIndex].uiCurrentPacketsOnHost < SF_MAX_ALLOWED_PACKETS_TO_BACKUP) &&
			((1000*(jiffies - *((B_UINT32 *)(PacketToDrop->cb)+SKB_CB_LATENCY_OFFSET))/HZ) <= Adapter->PackInfo[iIndex].uiMaxLatency))
			break;

		if (PacketToDrop)
		{
			if (netif_msg_tx_err(Adapter))
				pr_info(PFX "%s: tx queue %d overlimit\n",
>>>>>>> refs/remotes/origin/master
					Adapter->dev->name, iIndex);

			netstats->tx_dropped++;

			DEQUEUEPACKET(Adapter->PackInfo[iIndex].FirstTxQueue,
						Adapter->PackInfo[iIndex].LastTxQueue);
			/// update current bytes and packets count
			Adapter->PackInfo[iIndex].uiCurrentBytesOnHost -=
				PacketToDrop->len;
			Adapter->PackInfo[iIndex].uiCurrentPacketsOnHost--;
			/// update dropped bytes and packets counts
			Adapter->PackInfo[iIndex].uiDroppedCountBytes += PacketToDrop->len;
			Adapter->PackInfo[iIndex].uiDroppedCountPackets++;
			dev_kfree_skb(PacketToDrop);

		}

<<<<<<< HEAD
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "Dropped Bytes:%x Dropped Packets:%x",
=======
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "Dropped Bytes:%x Dropped Packets:%x",
>>>>>>> refs/remotes/origin/master
			Adapter->PackInfo[iIndex].uiDroppedCountBytes,
			Adapter->PackInfo[iIndex].uiDroppedCountPackets);

		atomic_dec(&Adapter->TotalPacketCount);
	}

	spin_unlock_bh(&Adapter->PackInfo[iIndex].SFQueueLock);

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "TotalPacketCount:%x",
		atomic_read(&Adapter->TotalPacketCount));
	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "<=====");
}

VOID flush_all_queues(PMINI_ADAPTER Adapter)
{
	INT		iQIndex;
	UINT	uiTotalPacketLength;
	struct sk_buff*			PacketToDrop=NULL;

	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "=====>");

//	down(&Adapter->data_packet_queue_lock);
	for(iQIndex=LowPriority; iQIndex<HiPriority; iQIndex++)
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "TotalPacketCount:%x",
		atomic_read(&Adapter->TotalPacketCount));
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, PRUNE_QUEUE, DBG_LVL_ALL, "<=====");
}

VOID flush_all_queues(struct bcm_mini_adapter *Adapter)
{
	INT		iQIndex;
	UINT	uiTotalPacketLength;
	struct sk_buff*			PacketToDrop = NULL;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "=====>");

//	down(&Adapter->data_packet_queue_lock);
	for (iQIndex = LowPriority; iQIndex < HiPriority; iQIndex++)
>>>>>>> refs/remotes/origin/master
	{
		struct net_device_stats *netstats = &Adapter->dev->stats;

		spin_lock_bh(&Adapter->PackInfo[iQIndex].SFQueueLock);
<<<<<<< HEAD
		while(Adapter->PackInfo[iQIndex].FirstTxQueue)
		{
			PacketToDrop = Adapter->PackInfo[iQIndex].FirstTxQueue;
			if(PacketToDrop)
=======
		while (Adapter->PackInfo[iQIndex].FirstTxQueue)
		{
			PacketToDrop = Adapter->PackInfo[iQIndex].FirstTxQueue;
			if (PacketToDrop)
>>>>>>> refs/remotes/origin/master
			{
				uiTotalPacketLength = PacketToDrop->len;
				netstats->tx_dropped++;
			}
			else
				uiTotalPacketLength = 0;

			DEQUEUEPACKET(Adapter->PackInfo[iQIndex].FirstTxQueue,
						Adapter->PackInfo[iQIndex].LastTxQueue);

			/* Free the skb */
			dev_kfree_skb(PacketToDrop);

			/// update current bytes and packets count
			Adapter->PackInfo[iQIndex].uiCurrentBytesOnHost -= uiTotalPacketLength;
			Adapter->PackInfo[iQIndex].uiCurrentPacketsOnHost--;

			/// update dropped bytes and packets counts
			Adapter->PackInfo[iQIndex].uiDroppedCountBytes += uiTotalPacketLength;
			Adapter->PackInfo[iQIndex].uiDroppedCountPackets++;

<<<<<<< HEAD
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "Dropped Bytes:%x Dropped Packets:%x",
=======
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "Dropped Bytes:%x Dropped Packets:%x",
>>>>>>> refs/remotes/origin/master
					Adapter->PackInfo[iQIndex].uiDroppedCountBytes,
					Adapter->PackInfo[iQIndex].uiDroppedCountPackets);
			atomic_dec(&Adapter->TotalPacketCount);
		}
		spin_unlock_bh(&Adapter->PackInfo[iQIndex].SFQueueLock);
	}
//	up(&Adapter->data_packet_queue_lock);
<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "<=====");
}

USHORT ClassifyPacket(PMINI_ADAPTER Adapter,struct sk_buff* skb)
{
	INT			uiLoopIndex=0;
	S_CLASSIFIER_RULE *pstClassifierRule = NULL;
	S_ETHCS_PKT_INFO stEthCsPktInfo;
	PVOID pvEThPayload = NULL;
	struct iphdr 		*pIpHeader = NULL;
	INT	  uiSfIndex=0;
	USHORT	usIndex=Adapter->usBestEffortQueueIndex;
	BOOLEAN	bFragmentedPkt=FALSE,bClassificationSucceed=FALSE;
	USHORT	usCurrFragment =0;

	PTCP_HEADER pTcpHeader;
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "<=====");
}

USHORT ClassifyPacket(struct bcm_mini_adapter *Adapter, struct sk_buff* skb)
{
	INT			uiLoopIndex = 0;
	struct bcm_classifier_rule *pstClassifierRule = NULL;
	struct bcm_eth_packet_info stEthCsPktInfo;
	PVOID pvEThPayload = NULL;
	struct iphdr *pIpHeader = NULL;
	INT	  uiSfIndex = 0;
	USHORT	usIndex = Adapter->usBestEffortQueueIndex;
	bool	bFragmentedPkt = false, bClassificationSucceed = false;
	USHORT	usCurrFragment = 0;

	struct bcm_tcp_header *pTcpHeader;
>>>>>>> refs/remotes/origin/master
	UCHAR IpHeaderLength;
	UCHAR TcpHeaderLength;

	pvEThPayload = skb->data;
<<<<<<< HEAD
	*((UINT32*) (skb->cb) +SKB_CB_TCPACK_OFFSET ) = 0;
	EThCSGetPktInfo(Adapter,pvEThPayload,&stEthCsPktInfo);

	switch(stEthCsPktInfo.eNwpktEthFrameType)
=======
	*((UINT32*) (skb->cb) +SKB_CB_TCPACK_OFFSET) = 0;
	EThCSGetPktInfo(Adapter, pvEThPayload, &stEthCsPktInfo);

	switch (stEthCsPktInfo.eNwpktEthFrameType)
>>>>>>> refs/remotes/origin/master
	{
		case eEth802LLCFrame:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "ClassifyPacket : 802LLCFrame\n");
<<<<<<< HEAD
            pIpHeader = pvEThPayload + sizeof(ETH_CS_802_LLC_FRAME);
=======
			pIpHeader = pvEThPayload + sizeof(struct bcm_eth_llc_frame);
>>>>>>> refs/remotes/origin/master
			break;
		}

		case eEth802LLCSNAPFrame:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "ClassifyPacket : 802LLC SNAP Frame\n");
<<<<<<< HEAD
			pIpHeader = pvEThPayload + sizeof(ETH_CS_802_LLC_SNAP_FRAME);
=======
			pIpHeader = pvEThPayload + sizeof(struct bcm_eth_llc_snap_frame);
>>>>>>> refs/remotes/origin/master
			break;
		}
		case eEth802QVLANFrame:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "ClassifyPacket : 802.1Q VLANFrame\n");
<<<<<<< HEAD
			pIpHeader = pvEThPayload + sizeof(ETH_CS_802_Q_FRAME);
=======
			pIpHeader = pvEThPayload + sizeof(struct bcm_eth_q_frame);
>>>>>>> refs/remotes/origin/master
			break;
		}
		case eEthOtherFrame:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "ClassifyPacket : ETH Other Frame\n");
<<<<<<< HEAD
			pIpHeader = pvEThPayload + sizeof(ETH_CS_ETH2_FRAME);
=======
			pIpHeader = pvEThPayload + sizeof(struct bcm_ethernet2_frame);
>>>>>>> refs/remotes/origin/master
			break;
		}
		default:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "ClassifyPacket : Unrecognized ETH Frame\n");
<<<<<<< HEAD
			pIpHeader = pvEThPayload + sizeof(ETH_CS_ETH2_FRAME);
=======
			pIpHeader = pvEThPayload + sizeof(struct bcm_ethernet2_frame);
>>>>>>> refs/remotes/origin/master
			break;
		}
	}

<<<<<<< HEAD
	if(stEthCsPktInfo.eNwpktIPFrameType == eIPv4Packet)
	{
		usCurrFragment = (ntohs(pIpHeader->frag_off) & IP_OFFSET);
		if((ntohs(pIpHeader->frag_off) & IP_MF) || usCurrFragment)
			bFragmentedPkt = TRUE;

		if(bFragmentedPkt)
		{
				//Fragmented  Packet. Get Frag Classifier Entry.
			pstClassifierRule = GetFragIPClsEntry(Adapter,pIpHeader->id, pIpHeader->saddr);
			if(pstClassifierRule)
			{
					BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,"It is next Fragmented pkt");
					bClassificationSucceed=TRUE;
			}
			if(!(ntohs(pIpHeader->frag_off) & IP_MF))
			{
				//Fragmented Last packet . Remove Frag Classifier Entry
				BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,"This is the last fragmented Pkt");
				DelFragIPClsEntry(Adapter,pIpHeader->id, pIpHeader->saddr);
=======
	if (stEthCsPktInfo.eNwpktIPFrameType == eIPv4Packet)
	{
		usCurrFragment = (ntohs(pIpHeader->frag_off) & IP_OFFSET);
		if ((ntohs(pIpHeader->frag_off) & IP_MF) || usCurrFragment)
			bFragmentedPkt = TRUE;

		if (bFragmentedPkt)
		{
				//Fragmented  Packet. Get Frag Classifier Entry.
			pstClassifierRule = GetFragIPClsEntry(Adapter, pIpHeader->id, pIpHeader->saddr);
			if (pstClassifierRule)
			{
					BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "It is next Fragmented pkt");
					bClassificationSucceed = TRUE;
			}
			if (!(ntohs(pIpHeader->frag_off) & IP_MF))
			{
				//Fragmented Last packet . Remove Frag Classifier Entry
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "This is the last fragmented Pkt");
				DelFragIPClsEntry(Adapter, pIpHeader->id, pIpHeader->saddr);
>>>>>>> refs/remotes/origin/master
			}
		}
	}

<<<<<<< HEAD
	for(uiLoopIndex = MAX_CLASSIFIERS - 1; uiLoopIndex >= 0; uiLoopIndex--)
	{
		if(bClassificationSucceed)
=======
	for (uiLoopIndex = MAX_CLASSIFIERS - 1; uiLoopIndex >= 0; uiLoopIndex--)
	{
		if (bClassificationSucceed)
>>>>>>> refs/remotes/origin/master
			break;
		//Iterate through all classifiers which are already in order of priority
		//to classify the packet until match found
		do
		{
<<<<<<< HEAD
			if(FALSE==Adapter->astClassifierTable[uiLoopIndex].bUsed)
			{
				bClassificationSucceed=FALSE;
				break;
			}
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "Adapter->PackInfo[%d].bvalid=True\n",uiLoopIndex);

			if(0 == Adapter->astClassifierTable[uiLoopIndex].ucDirection)
			{
				bClassificationSucceed=FALSE;//cannot be processed for classification.
=======
			if (false == Adapter->astClassifierTable[uiLoopIndex].bUsed)
			{
				bClassificationSucceed = false;
				break;
			}
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "Adapter->PackInfo[%d].bvalid=True\n", uiLoopIndex);

			if (0 == Adapter->astClassifierTable[uiLoopIndex].ucDirection)
			{
				bClassificationSucceed = false;//cannot be processed for classification.
>>>>>>> refs/remotes/origin/master
				break;						// it is a down link connection
			}

			pstClassifierRule = &Adapter->astClassifierTable[uiLoopIndex];

<<<<<<< HEAD
			uiSfIndex = SearchSfid(Adapter,pstClassifierRule->ulSFID);
=======
			uiSfIndex = SearchSfid(Adapter, pstClassifierRule->ulSFID);
>>>>>>> refs/remotes/origin/master
			if (uiSfIndex >= NO_OF_QUEUES) {
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "Queue Not Valid. SearchSfid for this classifier Failed\n");
				break;
			}

<<<<<<< HEAD
			if(Adapter->PackInfo[uiSfIndex].bEthCSSupport)
			{

				if(eEthUnsupportedFrame==stEthCsPktInfo.eNwpktEthFrameType)
				{
					BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  " ClassifyPacket : Packet Not a Valid Supported Ethernet Frame \n");
					bClassificationSucceed = FALSE;
=======
			if (Adapter->PackInfo[uiSfIndex].bEthCSSupport)
			{

				if (eEthUnsupportedFrame == stEthCsPktInfo.eNwpktEthFrameType)
				{
					BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, " ClassifyPacket : Packet Not a Valid Supported Ethernet Frame\n");
					bClassificationSucceed = false;
>>>>>>> refs/remotes/origin/master
					break;
				}



<<<<<<< HEAD
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "Performing ETH CS Classification on Classifier Rule ID : %x Service Flow ID : %lx\n",pstClassifierRule->uiClassifierRuleIndex,Adapter->PackInfo[uiSfIndex].ulSFID);
				bClassificationSucceed = EThCSClassifyPkt(Adapter,skb,&stEthCsPktInfo,pstClassifierRule, Adapter->PackInfo[uiSfIndex].bEthCSSupport);

				if(!bClassificationSucceed)
=======
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "Performing ETH CS Classification on Classifier Rule ID : %x Service Flow ID : %lx\n", pstClassifierRule->uiClassifierRuleIndex, Adapter->PackInfo[uiSfIndex].ulSFID);
				bClassificationSucceed = EThCSClassifyPkt(Adapter, skb, &stEthCsPktInfo, pstClassifierRule, Adapter->PackInfo[uiSfIndex].bEthCSSupport);

				if (!bClassificationSucceed)
>>>>>>> refs/remotes/origin/master
				{
					BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "ClassifyPacket : Ethernet CS Classification Failed\n");
					break;
				}
			}

			else // No ETH Supported on this SF
			{
<<<<<<< HEAD
				if(eEthOtherFrame != stEthCsPktInfo.eNwpktEthFrameType)
				{
					BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  " ClassifyPacket : Packet Not a 802.3 Ethernet Frame... hence not allowed over non-ETH CS SF \n");
					bClassificationSucceed = FALSE;
=======
				if (eEthOtherFrame != stEthCsPktInfo.eNwpktEthFrameType)
				{
					BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, " ClassifyPacket : Packet Not a 802.3 Ethernet Frame... hence not allowed over non-ETH CS SF\n");
					bClassificationSucceed = false;
>>>>>>> refs/remotes/origin/master
					break;
				}
			}

			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "Proceeding to IP CS Clasification");

<<<<<<< HEAD
			if(Adapter->PackInfo[uiSfIndex].bIPCSSupport)
			{

				if(stEthCsPktInfo.eNwpktIPFrameType == eNonIPPacket)
				{
					BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  " ClassifyPacket : Packet is Not an IP Packet \n");
					bClassificationSucceed = FALSE;
					break;
				}
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "Dump IP Header : \n");
				DumpFullPacket((PUCHAR)pIpHeader,20);

				if(stEthCsPktInfo.eNwpktIPFrameType == eIPv4Packet)
					bClassificationSucceed = IpVersion4(Adapter,pIpHeader,pstClassifierRule);
				else if(stEthCsPktInfo.eNwpktIPFrameType == eIPv6Packet)
					bClassificationSucceed = IpVersion6(Adapter,pIpHeader,pstClassifierRule);
			}

		}while(0);
	}

	if(bClassificationSucceed == TRUE)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "CF id : %d, SF ID is =%lu",pstClassifierRule->uiClassifierRuleIndex, pstClassifierRule->ulSFID);

		//Store The matched Classifier in SKB
		*((UINT32*)(skb->cb)+SKB_CB_CLASSIFICATION_OFFSET) = pstClassifierRule->uiClassifierRuleIndex;
		if((TCP == pIpHeader->protocol ) && !bFragmentedPkt && (ETH_AND_IP_HEADER_LEN + TCP_HEADER_LEN <= skb->len) )
		{
			 IpHeaderLength   = pIpHeader->ihl;
			 pTcpHeader = (PTCP_HEADER)(((PUCHAR)pIpHeader)+(IpHeaderLength*4));
			 TcpHeaderLength  = GET_TCP_HEADER_LEN(pTcpHeader->HeaderLength);

			if((pTcpHeader->ucFlags & TCP_ACK) &&
			   (ntohs(pIpHeader->tot_len) == (IpHeaderLength*4)+(TcpHeaderLength*4)))
			{
    			*((UINT32*) (skb->cb) +SKB_CB_TCPACK_OFFSET ) = TCP_ACK;
=======
			if (Adapter->PackInfo[uiSfIndex].bIPCSSupport)
			{

				if (stEthCsPktInfo.eNwpktIPFrameType == eNonIPPacket)
				{
					BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, " ClassifyPacket : Packet is Not an IP Packet\n");
					bClassificationSucceed = false;
					break;
				}
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Dump IP Header :\n");
				DumpFullPacket((PUCHAR)pIpHeader, 20);

				if (stEthCsPktInfo.eNwpktIPFrameType == eIPv4Packet)
					bClassificationSucceed = IpVersion4(Adapter, pIpHeader, pstClassifierRule);
				else if (stEthCsPktInfo.eNwpktIPFrameType == eIPv6Packet)
					bClassificationSucceed = IpVersion6(Adapter, pIpHeader, pstClassifierRule);
			}

		} while (0);
	}

	if (bClassificationSucceed == TRUE)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "CF id : %d, SF ID is =%lu", pstClassifierRule->uiClassifierRuleIndex, pstClassifierRule->ulSFID);

		//Store The matched Classifier in SKB
		*((UINT32*)(skb->cb)+SKB_CB_CLASSIFICATION_OFFSET) = pstClassifierRule->uiClassifierRuleIndex;
		if ((TCP == pIpHeader->protocol) && !bFragmentedPkt && (ETH_AND_IP_HEADER_LEN + TCP_HEADER_LEN <= skb->len))
		{
			 IpHeaderLength   = pIpHeader->ihl;
			 pTcpHeader = (struct bcm_tcp_header *)(((PUCHAR)pIpHeader)+(IpHeaderLength*4));
			 TcpHeaderLength  = GET_TCP_HEADER_LEN(pTcpHeader->HeaderLength);

			if ((pTcpHeader->ucFlags & TCP_ACK) &&
			   (ntohs(pIpHeader->tot_len) == (IpHeaderLength*4)+(TcpHeaderLength*4)))
			{
				*((UINT32*) (skb->cb) + SKB_CB_TCPACK_OFFSET) = TCP_ACK;
>>>>>>> refs/remotes/origin/master
			}
		}

		usIndex = SearchSfid(Adapter, pstClassifierRule->ulSFID);
<<<<<<< HEAD
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "index is	=%d", usIndex);

		//If this is the first fragment of a Fragmented pkt, add this CF. Only This CF should be used for all other fragment of this Pkt.
		if(bFragmentedPkt && (usCurrFragment == 0))
		{
			//First Fragment of Fragmented Packet. Create Frag CLS Entry
			S_FRAGMENTED_PACKET_INFO stFragPktInfo;
=======
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "index is	=%d", usIndex);

		//If this is the first fragment of a Fragmented pkt, add this CF. Only This CF should be used for all other fragment of this Pkt.
		if (bFragmentedPkt && (usCurrFragment == 0))
		{
			//First Fragment of Fragmented Packet. Create Frag CLS Entry
			struct bcm_fragmented_packet_info stFragPktInfo;
>>>>>>> refs/remotes/origin/master
			stFragPktInfo.bUsed = TRUE;
			stFragPktInfo.ulSrcIpAddress = pIpHeader->saddr;
			stFragPktInfo.usIpIdentification = pIpHeader->id;
			stFragPktInfo.pstMatchedClassifierEntry = pstClassifierRule;
<<<<<<< HEAD
			stFragPktInfo.bOutOfOrderFragment = FALSE;
			AddFragIPClsEntry(Adapter,&stFragPktInfo);
=======
			stFragPktInfo.bOutOfOrderFragment = false;
			AddFragIPClsEntry(Adapter, &stFragPktInfo);
>>>>>>> refs/remotes/origin/master
		}


	}

<<<<<<< HEAD
	if(bClassificationSucceed)
=======
	if (bClassificationSucceed)
>>>>>>> refs/remotes/origin/master
		return usIndex;
	else
		return INVALID_QUEUE_INDEX;
}

<<<<<<< HEAD
static BOOLEAN EthCSMatchSrcMACAddress(S_CLASSIFIER_RULE *pstClassifierRule,PUCHAR Mac)
{
	UINT i=0;
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if(pstClassifierRule->ucEthCSSrcMACLen==0)
		return TRUE;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s \n",__FUNCTION__);
	for(i=0;i<MAC_ADDRESS_SIZE;i++)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "SRC MAC[%x] = %x ClassifierRuleSrcMAC = %x Mask : %x\n",i,Mac[i],pstClassifierRule->au8EThCSSrcMAC[i],pstClassifierRule->au8EThCSSrcMACMask[i]);
		if((pstClassifierRule->au8EThCSSrcMAC[i] & pstClassifierRule->au8EThCSSrcMACMask[i])!=
			(Mac[i] & pstClassifierRule->au8EThCSSrcMACMask[i]))
			return FALSE;
=======
static bool EthCSMatchSrcMACAddress(struct bcm_classifier_rule *pstClassifierRule, PUCHAR Mac)
{
	UINT i = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if (pstClassifierRule->ucEthCSSrcMACLen == 0)
		return TRUE;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s\n", __FUNCTION__);
	for (i = 0; i < MAC_ADDRESS_SIZE; i++)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "SRC MAC[%x] = %x ClassifierRuleSrcMAC = %x Mask : %x\n", i, Mac[i], pstClassifierRule->au8EThCSSrcMAC[i], pstClassifierRule->au8EThCSSrcMACMask[i]);
		if ((pstClassifierRule->au8EThCSSrcMAC[i] & pstClassifierRule->au8EThCSSrcMACMask[i]) !=
			(Mac[i] & pstClassifierRule->au8EThCSSrcMACMask[i]))
			return false;
>>>>>>> refs/remotes/origin/master
	}
	return TRUE;
}

<<<<<<< HEAD
static BOOLEAN EthCSMatchDestMACAddress(S_CLASSIFIER_RULE *pstClassifierRule,PUCHAR Mac)
{
	UINT i=0;
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if(pstClassifierRule->ucEthCSDestMACLen==0)
		return TRUE;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s \n",__FUNCTION__);
	for(i=0;i<MAC_ADDRESS_SIZE;i++)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "SRC MAC[%x] = %x ClassifierRuleSrcMAC = %x Mask : %x\n",i,Mac[i],pstClassifierRule->au8EThCSDestMAC[i],pstClassifierRule->au8EThCSDestMACMask[i]);
		if((pstClassifierRule->au8EThCSDestMAC[i] & pstClassifierRule->au8EThCSDestMACMask[i])!=
			(Mac[i] & pstClassifierRule->au8EThCSDestMACMask[i]))
			return FALSE;
=======
static bool EthCSMatchDestMACAddress(struct bcm_classifier_rule *pstClassifierRule, PUCHAR Mac)
{
	UINT i = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if (pstClassifierRule->ucEthCSDestMACLen == 0)
		return TRUE;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "%s\n", __FUNCTION__);
	for (i = 0; i < MAC_ADDRESS_SIZE; i++)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "SRC MAC[%x] = %x ClassifierRuleSrcMAC = %x Mask : %x\n", i, Mac[i], pstClassifierRule->au8EThCSDestMAC[i], pstClassifierRule->au8EThCSDestMACMask[i]);
		if ((pstClassifierRule->au8EThCSDestMAC[i] & pstClassifierRule->au8EThCSDestMACMask[i]) !=
			(Mac[i] & pstClassifierRule->au8EThCSDestMACMask[i]))
			return false;
>>>>>>> refs/remotes/origin/master
	}
	return TRUE;
}

<<<<<<< HEAD
static BOOLEAN EthCSMatchEThTypeSAP(S_CLASSIFIER_RULE *pstClassifierRule,struct sk_buff* skb,PS_ETHCS_PKT_INFO pstEthCsPktInfo)
{
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if((pstClassifierRule->ucEtherTypeLen==0)||
		(pstClassifierRule->au8EthCSEtherType[0] == 0))
		return TRUE;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s SrcEtherType:%x CLS EtherType[0]:%x\n",__FUNCTION__,pstEthCsPktInfo->usEtherType,pstClassifierRule->au8EthCSEtherType[0]);
	if(pstClassifierRule->au8EthCSEtherType[0] == 1)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s  CLS EtherType[1]:%x EtherType[2]:%x\n",__FUNCTION__,pstClassifierRule->au8EthCSEtherType[1],pstClassifierRule->au8EthCSEtherType[2]);

		if(memcmp(&pstEthCsPktInfo->usEtherType,&pstClassifierRule->au8EthCSEtherType[1],2)==0)
			return TRUE;
		else
			return FALSE;
	}

	if(pstClassifierRule->au8EthCSEtherType[0] == 2)
	{
		if(eEth802LLCFrame != pstEthCsPktInfo->eNwpktEthFrameType)
			return FALSE;

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s  EthCS DSAP:%x EtherType[2]:%x\n",__FUNCTION__,pstEthCsPktInfo->ucDSAP,pstClassifierRule->au8EthCSEtherType[2]);
		if(pstEthCsPktInfo->ucDSAP == pstClassifierRule->au8EthCSEtherType[2])
			return TRUE;
		else
			return FALSE;

	}

	return FALSE;

}

static BOOLEAN EthCSMatchVLANRules(S_CLASSIFIER_RULE *pstClassifierRule,struct sk_buff* skb,PS_ETHCS_PKT_INFO pstEthCsPktInfo)
{
	BOOLEAN bClassificationSucceed = FALSE;
	USHORT usVLANID;
	B_UINT8 uPriority = 0;
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s  CLS UserPrio:%x CLS VLANID:%x\n",__FUNCTION__,ntohs(*((USHORT *)pstClassifierRule->usUserPriority)),pstClassifierRule->usVLANID);

	/* In case FW didn't receive the TLV, the priority field should be ignored */
	if(pstClassifierRule->usValidityBitMap & (1<<PKT_CLASSIFICATION_USER_PRIORITY_VALID))
	{
		if(pstEthCsPktInfo->eNwpktEthFrameType!=eEth802QVLANFrame)
				return FALSE;

		uPriority = (ntohs(*(USHORT *)(skb->data + sizeof(ETH_HEADER_STRUC))) & 0xF000) >> 13;

		if((uPriority >= pstClassifierRule->usUserPriority[0]) && (uPriority <= pstClassifierRule->usUserPriority[1]))
				bClassificationSucceed = TRUE;

		if(!bClassificationSucceed)
			return FALSE;
=======
static bool EthCSMatchEThTypeSAP(struct bcm_classifier_rule *pstClassifierRule, struct sk_buff* skb, struct bcm_eth_packet_info *pstEthCsPktInfo)
{
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	if ((pstClassifierRule->ucEtherTypeLen == 0) ||
		(pstClassifierRule->au8EthCSEtherType[0] == 0))
		return TRUE;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s SrcEtherType:%x CLS EtherType[0]:%x\n", __FUNCTION__, pstEthCsPktInfo->usEtherType, pstClassifierRule->au8EthCSEtherType[0]);
	if (pstClassifierRule->au8EthCSEtherType[0] == 1)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s  CLS EtherType[1]:%x EtherType[2]:%x\n", __FUNCTION__, pstClassifierRule->au8EthCSEtherType[1], pstClassifierRule->au8EthCSEtherType[2]);

		if (memcmp(&pstEthCsPktInfo->usEtherType, &pstClassifierRule->au8EthCSEtherType[1], 2) == 0)
			return TRUE;
		else
			return false;
	}

	if (pstClassifierRule->au8EthCSEtherType[0] == 2)
	{
		if (eEth802LLCFrame != pstEthCsPktInfo->eNwpktEthFrameType)
			return false;

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s  EthCS DSAP:%x EtherType[2]:%x\n", __FUNCTION__, pstEthCsPktInfo->ucDSAP, pstClassifierRule->au8EthCSEtherType[2]);
		if (pstEthCsPktInfo->ucDSAP == pstClassifierRule->au8EthCSEtherType[2])
			return TRUE;
		else
			return false;

	}

	return false;

}

static bool EthCSMatchVLANRules(struct bcm_classifier_rule *pstClassifierRule, struct sk_buff* skb, struct bcm_eth_packet_info *pstEthCsPktInfo)
{
	bool bClassificationSucceed = false;
	USHORT usVLANID;
	B_UINT8 uPriority = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s  CLS UserPrio:%x CLS VLANID:%x\n", __FUNCTION__, ntohs(*((USHORT *)pstClassifierRule->usUserPriority)), pstClassifierRule->usVLANID);

	/* In case FW didn't receive the TLV, the priority field should be ignored */
	if (pstClassifierRule->usValidityBitMap & (1<<PKT_CLASSIFICATION_USER_PRIORITY_VALID))
	{
		if (pstEthCsPktInfo->eNwpktEthFrameType != eEth802QVLANFrame)
				return false;

		uPriority = (ntohs(*(USHORT *)(skb->data + sizeof(struct bcm_eth_header))) & 0xF000) >> 13;

		if ((uPriority >= pstClassifierRule->usUserPriority[0]) && (uPriority <= pstClassifierRule->usUserPriority[1]))
				bClassificationSucceed = TRUE;

		if (!bClassificationSucceed)
			return false;
>>>>>>> refs/remotes/origin/master
	}

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "ETH CS 802.1 D  User Priority Rule Matched\n");

<<<<<<< HEAD
	bClassificationSucceed = FALSE;

	if(pstClassifierRule->usValidityBitMap & (1<<PKT_CLASSIFICATION_VLANID_VALID))
	{
		if(pstEthCsPktInfo->eNwpktEthFrameType!=eEth802QVLANFrame)
				return FALSE;

		usVLANID = ntohs(*(USHORT *)(skb->data + sizeof(ETH_HEADER_STRUC))) & 0xFFF;

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s  Pkt VLANID %x Priority: %d\n",__FUNCTION__,usVLANID, uPriority);

		if(usVLANID == ((pstClassifierRule->usVLANID & 0xFFF0) >> 4))
			bClassificationSucceed = TRUE;

		if(!bClassificationSucceed)
			return FALSE;
=======
	bClassificationSucceed = false;

	if (pstClassifierRule->usValidityBitMap & (1<<PKT_CLASSIFICATION_VLANID_VALID))
	{
		if (pstEthCsPktInfo->eNwpktEthFrameType != eEth802QVLANFrame)
				return false;

		usVLANID = ntohs(*(USHORT *)(skb->data + sizeof(struct bcm_eth_header))) & 0xFFF;

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "%s  Pkt VLANID %x Priority: %d\n", __FUNCTION__, usVLANID, uPriority);

		if (usVLANID == ((pstClassifierRule->usVLANID & 0xFFF0) >> 4))
			bClassificationSucceed = TRUE;

		if (!bClassificationSucceed)
			return false;
>>>>>>> refs/remotes/origin/master
	}

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "ETH CS 802.1 Q VLAN ID Rule Matched\n");

	return TRUE;
}


<<<<<<< HEAD
static BOOLEAN EThCSClassifyPkt(PMINI_ADAPTER Adapter,struct sk_buff* skb,
				PS_ETHCS_PKT_INFO pstEthCsPktInfo,
				S_CLASSIFIER_RULE *pstClassifierRule,
				B_UINT8 EthCSCupport)
{
	BOOLEAN bClassificationSucceed = FALSE;
	bClassificationSucceed = EthCSMatchSrcMACAddress(pstClassifierRule,((ETH_HEADER_STRUC *)(skb->data))->au8SourceAddress);
	if(!bClassificationSucceed)
		return FALSE;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "ETH CS SrcMAC Matched\n");

	bClassificationSucceed = EthCSMatchDestMACAddress(pstClassifierRule,((ETH_HEADER_STRUC*)(skb->data))->au8DestinationAddress);
	if(!bClassificationSucceed)
		return FALSE;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "ETH CS DestMAC Matched\n");

	//classify on ETHType/802.2SAP TLV
	bClassificationSucceed = EthCSMatchEThTypeSAP(pstClassifierRule,skb,pstEthCsPktInfo);
	if(!bClassificationSucceed)
		return FALSE;
=======
static bool EThCSClassifyPkt(struct bcm_mini_adapter *Adapter, struct sk_buff* skb,
				struct bcm_eth_packet_info *pstEthCsPktInfo,
				struct bcm_classifier_rule *pstClassifierRule,
				B_UINT8 EthCSCupport)
{
	bool bClassificationSucceed = false;
	bClassificationSucceed = EthCSMatchSrcMACAddress(pstClassifierRule, ((struct bcm_eth_header *)(skb->data))->au8SourceAddress);
	if (!bClassificationSucceed)
		return false;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "ETH CS SrcMAC Matched\n");

	bClassificationSucceed = EthCSMatchDestMACAddress(pstClassifierRule, ((struct bcm_eth_header *)(skb->data))->au8DestinationAddress);
	if (!bClassificationSucceed)
		return false;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "ETH CS DestMAC Matched\n");

	//classify on ETHType/802.2SAP TLV
	bClassificationSucceed = EthCSMatchEThTypeSAP(pstClassifierRule, skb, pstEthCsPktInfo);
	if (!bClassificationSucceed)
		return false;
>>>>>>> refs/remotes/origin/master

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "ETH CS EthType/802.2SAP Matched\n");

	//classify on 802.1VLAN Header Parameters

<<<<<<< HEAD
	bClassificationSucceed = EthCSMatchVLANRules(pstClassifierRule,skb,pstEthCsPktInfo);
	if(!bClassificationSucceed)
		return FALSE;
=======
	bClassificationSucceed = EthCSMatchVLANRules(pstClassifierRule, skb, pstEthCsPktInfo);
	if (!bClassificationSucceed)
		return false;
>>>>>>> refs/remotes/origin/master
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "ETH CS 802.1 VLAN Rules Matched\n");

	return bClassificationSucceed;
}

<<<<<<< HEAD
static void EThCSGetPktInfo(PMINI_ADAPTER Adapter,PVOID pvEthPayload,
			    PS_ETHCS_PKT_INFO pstEthCsPktInfo)
{
	USHORT u16Etype = ntohs(((ETH_HEADER_STRUC*)pvEthPayload)->u16Etype);

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "EthCSGetPktInfo : Eth Hdr Type : %X\n",u16Etype);
	if(u16Etype > 0x5dc)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "EthCSGetPktInfo : ETH2 Frame \n");
		//ETH2 Frame
		if(u16Etype == ETHERNET_FRAMETYPE_802QVLAN)
		{
			//802.1Q VLAN Header
			pstEthCsPktInfo->eNwpktEthFrameType = eEth802QVLANFrame;
			u16Etype = ((ETH_CS_802_Q_FRAME*)pvEthPayload)->EthType;
=======
static void EThCSGetPktInfo(struct bcm_mini_adapter *Adapter, PVOID pvEthPayload,
			    struct bcm_eth_packet_info *pstEthCsPktInfo)
{
	USHORT u16Etype = ntohs(((struct bcm_eth_header *)pvEthPayload)->u16Etype);

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "EthCSGetPktInfo : Eth Hdr Type : %X\n", u16Etype);
	if (u16Etype > 0x5dc)
	{
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "EthCSGetPktInfo : ETH2 Frame\n");
		//ETH2 Frame
		if (u16Etype == ETHERNET_FRAMETYPE_802QVLAN)
		{
			//802.1Q VLAN Header
			pstEthCsPktInfo->eNwpktEthFrameType = eEth802QVLANFrame;
			u16Etype = ((struct bcm_eth_q_frame *)pvEthPayload)->EthType;
>>>>>>> refs/remotes/origin/master
			//((ETH_CS_802_Q_FRAME*)pvEthPayload)->UserPriority
		}
		else
		{
			pstEthCsPktInfo->eNwpktEthFrameType = eEthOtherFrame;
			u16Etype = ntohs(u16Etype);
		}

	}
	else
	{
		//802.2 LLC
<<<<<<< HEAD
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "802.2 LLC Frame \n");
		pstEthCsPktInfo->eNwpktEthFrameType = eEth802LLCFrame;
		pstEthCsPktInfo->ucDSAP = ((ETH_CS_802_LLC_FRAME*)pvEthPayload)->DSAP;
		if(pstEthCsPktInfo->ucDSAP == 0xAA && ((ETH_CS_802_LLC_FRAME*)pvEthPayload)->SSAP == 0xAA)
		{
			//SNAP Frame
			pstEthCsPktInfo->eNwpktEthFrameType = eEth802LLCSNAPFrame;
			u16Etype = ((ETH_CS_802_LLC_SNAP_FRAME*)pvEthPayload)->usEtherType;
		}
	}
	if(u16Etype == ETHERNET_FRAMETYPE_IPV4)
		pstEthCsPktInfo->eNwpktIPFrameType = eIPv4Packet;
	else if(u16Etype == ETHERNET_FRAMETYPE_IPV6)
=======
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "802.2 LLC Frame\n");
		pstEthCsPktInfo->eNwpktEthFrameType = eEth802LLCFrame;
		pstEthCsPktInfo->ucDSAP = ((struct bcm_eth_llc_frame *)pvEthPayload)->DSAP;
		if (pstEthCsPktInfo->ucDSAP == 0xAA && ((struct bcm_eth_llc_frame *)pvEthPayload)->SSAP == 0xAA)
		{
			//SNAP Frame
			pstEthCsPktInfo->eNwpktEthFrameType = eEth802LLCSNAPFrame;
			u16Etype = ((struct bcm_eth_llc_snap_frame *)pvEthPayload)->usEtherType;
		}
	}
	if (u16Etype == ETHERNET_FRAMETYPE_IPV4)
		pstEthCsPktInfo->eNwpktIPFrameType = eIPv4Packet;
	else if (u16Etype == ETHERNET_FRAMETYPE_IPV6)
>>>>>>> refs/remotes/origin/master
		pstEthCsPktInfo->eNwpktIPFrameType = eIPv6Packet;
	else
		pstEthCsPktInfo->eNwpktIPFrameType = eNonIPPacket;

<<<<<<< HEAD
	pstEthCsPktInfo->usEtherType = ((ETH_HEADER_STRUC*)pvEthPayload)->u16Etype;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "EthCsPktInfo->eNwpktIPFrameType : %x\n",pstEthCsPktInfo->eNwpktIPFrameType);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "EthCsPktInfo->eNwpktEthFrameType : %x\n",pstEthCsPktInfo->eNwpktEthFrameType);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "EthCsPktInfo->usEtherType : %x\n",pstEthCsPktInfo->usEtherType);
=======
	pstEthCsPktInfo->usEtherType = ((struct bcm_eth_header *)pvEthPayload)->u16Etype;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "EthCsPktInfo->eNwpktIPFrameType : %x\n", pstEthCsPktInfo->eNwpktIPFrameType);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "EthCsPktInfo->eNwpktEthFrameType : %x\n", pstEthCsPktInfo->eNwpktEthFrameType);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,  "EthCsPktInfo->usEtherType : %x\n", pstEthCsPktInfo->usEtherType);
>>>>>>> refs/remotes/origin/master
}



