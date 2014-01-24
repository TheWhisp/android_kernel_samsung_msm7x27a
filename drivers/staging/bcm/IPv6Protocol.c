#include "headers.h"

<<<<<<< HEAD
static BOOLEAN MatchSrcIpv6Address(S_CLASSIFIER_RULE *pstClassifierRule,IPV6Header *pstIpv6Header);
static BOOLEAN MatchDestIpv6Address(S_CLASSIFIER_RULE *pstClassifierRule,IPV6Header *pstIpv6Header);
static VOID DumpIpv6Header(IPV6Header *pstIpv6Header);

static UCHAR * GetNextIPV6ChainedHeader(UCHAR **ppucPayload,UCHAR *pucNextHeader,BOOLEAN *bParseDone,USHORT *pusPayloadLength)
=======
static bool MatchSrcIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
	struct bcm_ipv6_hdr *pstIpv6Header);
static bool MatchDestIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
	struct bcm_ipv6_hdr *pstIpv6Header);
static VOID DumpIpv6Header(struct bcm_ipv6_hdr *pstIpv6Header);

static UCHAR *GetNextIPV6ChainedHeader(UCHAR **ppucPayload,
	UCHAR *pucNextHeader, bool *bParseDone, USHORT *pusPayloadLength)
>>>>>>> refs/remotes/origin/master
{
	UCHAR *pucRetHeaderPtr = NULL;
	UCHAR *pucPayloadPtr = NULL;
	USHORT  usNextHeaderOffset = 0 ;
<<<<<<< HEAD
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);

	if((NULL == ppucPayload) || (*pusPayloadLength == 0) || (*bParseDone))
	{
		*bParseDone = TRUE;
		return NULL;

=======
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	if ((ppucPayload == NULL) || (*pusPayloadLength == 0) ||
		(*bParseDone)) {
		*bParseDone = TRUE;
		return NULL;
>>>>>>> refs/remotes/origin/master
	}

	pucRetHeaderPtr = *ppucPayload;
	pucPayloadPtr = *ppucPayload;

<<<<<<< HEAD
	if(!pucRetHeaderPtr || !pucPayloadPtr)
	{
=======
	if (!pucRetHeaderPtr || !pucPayloadPtr) {
>>>>>>> refs/remotes/origin/master
		*bParseDone = TRUE;
		return NULL;
	}

<<<<<<< HEAD
	//Get the Nextt Header Type
	*bParseDone = FALSE;



	switch(*pucNextHeader)
	{
	case IPV6HDR_TYPE_HOPBYHOP:
		{

			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 HopByHop Header");
			usNextHeaderOffset+=sizeof(IPV6HopByHopOptionsHeader);
=======
	/* Get the Nextt Header Type */
	*bParseDone = false;


	switch (*pucNextHeader) {
	case IPV6HDR_TYPE_HOPBYHOP:
		{

			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 HopByHop Header");
			usNextHeaderOffset += sizeof(struct bcm_ipv6_options_hdr);
>>>>>>> refs/remotes/origin/master
		}
		break;

	case IPV6HDR_TYPE_ROUTING:
		{
<<<<<<< HEAD
			IPV6RoutingHeader *pstIpv6RoutingHeader;
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 Routing Header");
			pstIpv6RoutingHeader = (IPV6RoutingHeader *)pucPayloadPtr;
			usNextHeaderOffset += sizeof(IPV6RoutingHeader);
=======
			struct bcm_ipv6_routing_hdr *pstIpv6RoutingHeader;
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Routing Header");
			pstIpv6RoutingHeader = (struct bcm_ipv6_routing_hdr *)pucPayloadPtr;
			usNextHeaderOffset += sizeof(struct bcm_ipv6_routing_hdr);
>>>>>>> refs/remotes/origin/master
			usNextHeaderOffset += pstIpv6RoutingHeader->ucNumAddresses * IPV6_ADDRESS_SIZEINBYTES;

		}
		break;
	case IPV6HDR_TYPE_FRAGMENTATION:
		{
<<<<<<< HEAD
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 Fragmentation Header");
			usNextHeaderOffset+= sizeof(IPV6FragmentHeader);
=======
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL,
					"\nIPv6 Fragmentation Header");
			usNextHeaderOffset += sizeof(struct bcm_ipv6_fragment_hdr);
>>>>>>> refs/remotes/origin/master

		}
		break;
	case IPV6HDR_TYPE_DESTOPTS:
		{
<<<<<<< HEAD
			IPV6DestOptionsHeader *pstIpv6DestOptsHdr = (IPV6DestOptionsHeader *)pucPayloadPtr;
			int nTotalOptions = pstIpv6DestOptsHdr->ucHdrExtLen;
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 DestOpts Header Header");
			usNextHeaderOffset+= sizeof(IPV6DestOptionsHeader);
			usNextHeaderOffset+= nTotalOptions * IPV6_DESTOPTS_HDR_OPTIONSIZE ;
=======
			struct bcm_ipv6_dest_options_hdr *pstIpv6DestOptsHdr = (struct bcm_ipv6_dest_options_hdr *)pucPayloadPtr;
			int nTotalOptions = pstIpv6DestOptsHdr->ucHdrExtLen;
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL,
					"\nIPv6 DestOpts Header Header");
			usNextHeaderOffset += sizeof(struct bcm_ipv6_dest_options_hdr);
			usNextHeaderOffset += nTotalOptions * IPV6_DESTOPTS_HDR_OPTIONSIZE ;
>>>>>>> refs/remotes/origin/master

		}
		break;
	case IPV6HDR_TYPE_AUTHENTICATION:
		{
<<<<<<< HEAD
			IPV6AuthenticationHeader *pstIpv6AuthHdr = (IPV6AuthenticationHeader *)pucPayloadPtr;
			int nHdrLen = pstIpv6AuthHdr->ucLength;
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 Authentication Header");
			usNextHeaderOffset+= nHdrLen * 4;
=======
			struct bcm_ipv6_authentication_hdr *pstIpv6AuthHdr = (struct bcm_ipv6_authentication_hdr *)pucPayloadPtr;
			int nHdrLen = pstIpv6AuthHdr->ucLength;
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL,
					"\nIPv6 Authentication Header");
			usNextHeaderOffset += nHdrLen * 4;
>>>>>>> refs/remotes/origin/master
		}
		break;
	case IPV6HDR_TYPE_ENCRYPTEDSECURITYPAYLOAD:
		{
<<<<<<< HEAD
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 Encrypted Security Payload Header");
=======
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL,
					"\nIPv6 Encrypted Security Payload Header");
>>>>>>> refs/remotes/origin/master
			*bParseDone = TRUE;

		}
		break;
	case IPV6_ICMP_HDR_TYPE:
		{
<<<<<<< HEAD
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, " ICMP Header");
=======
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nICMP Header");
>>>>>>> refs/remotes/origin/master
			*bParseDone = TRUE;
		}
		break;
	case TCP_HEADER_TYPE:
		{
<<<<<<< HEAD
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, " \nTCP Header");
=======
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nTCP Header");
>>>>>>> refs/remotes/origin/master
			*bParseDone = TRUE;
		}
		break;
	case UDP_HEADER_TYPE:
		{
<<<<<<< HEAD
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, " \nUDP Header");
			*bParseDone = TRUE;
		}
		break;
	default :
=======
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nUDP Header");
			*bParseDone = TRUE;
		}
		break;
	default:
>>>>>>> refs/remotes/origin/master
		{
			*bParseDone = TRUE;

		}
		break;


	}

<<<<<<< HEAD
	if(*bParseDone == FALSE)
	{
		if(*pusPayloadLength <= usNextHeaderOffset)
		{
			*bParseDone = TRUE;
		}
		else
		{
			*pucNextHeader = *pucPayloadPtr;
			pucPayloadPtr+=usNextHeaderOffset;
			(*pusPayloadLength)-=usNextHeaderOffset;
=======
	if (*bParseDone == false) {
		if (*pusPayloadLength <= usNextHeaderOffset) {
			*bParseDone = TRUE;
		} else {
			*pucNextHeader = *pucPayloadPtr;
			pucPayloadPtr += usNextHeaderOffset;
			(*pusPayloadLength) -= usNextHeaderOffset;
>>>>>>> refs/remotes/origin/master
		}

	}

<<<<<<< HEAD


=======
>>>>>>> refs/remotes/origin/master
	*ppucPayload = pucPayloadPtr;
	return pucRetHeaderPtr;
}


<<<<<<< HEAD
static UCHAR GetIpv6ProtocolPorts(UCHAR *pucPayload,USHORT *pusSrcPort,USHORT *pusDestPort,USHORT usPayloadLength,UCHAR ucNextHeader)
{
	UCHAR *pIpv6HdrScanContext = pucPayload;
	BOOLEAN bDone = FALSE;
	UCHAR ucHeaderType =0;
	UCHAR *pucNextHeader = NULL;
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);

	if( !pucPayload || (usPayloadLength == 0))
	{
		return 0;
	}

	*pusSrcPort = *pusDestPort = 0;
	ucHeaderType = ucNextHeader;
	while(!bDone)
	{
		pucNextHeader = GetNextIPV6ChainedHeader(&pIpv6HdrScanContext,&ucHeaderType,&bDone,&usPayloadLength);
		if(bDone)
		{
			if((ucHeaderType==TCP_HEADER_TYPE) || (ucHeaderType == UDP_HEADER_TYPE))
			{
				 *pusSrcPort=*((PUSHORT)(pucNextHeader));
				 *pusDestPort=*((PUSHORT)(pucNextHeader+2));
				 BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, " \nProtocol Ports - Src Port :0x%x Dest Port : 0x%x",ntohs(*pusSrcPort),ntohs(*pusDestPort));
=======
static UCHAR GetIpv6ProtocolPorts(UCHAR *pucPayload, USHORT *pusSrcPort,
	USHORT *pusDestPort, USHORT usPayloadLength, UCHAR ucNextHeader)
{
	UCHAR *pIpv6HdrScanContext = pucPayload;
	bool bDone = false;
	UCHAR ucHeaderType = 0;
	UCHAR *pucNextHeader = NULL;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	if (!pucPayload || (usPayloadLength == 0))
		return 0;

	*pusSrcPort = *pusDestPort = 0;
	ucHeaderType = ucNextHeader;
	while (!bDone) {
		pucNextHeader = GetNextIPV6ChainedHeader(&pIpv6HdrScanContext,
					&ucHeaderType, &bDone, &usPayloadLength);
		if (bDone) {
			if ((ucHeaderType == TCP_HEADER_TYPE) ||
				(ucHeaderType == UDP_HEADER_TYPE)) {
				*pusSrcPort = *((PUSHORT)(pucNextHeader));
				*pusDestPort = *((PUSHORT)(pucNextHeader+2));
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
						DBG_LVL_ALL,
						"\nProtocol Ports - Src Port :0x%x Dest Port : 0x%x",
						ntohs(*pusSrcPort),
						ntohs(*pusDestPort));
>>>>>>> refs/remotes/origin/master
			}
			break;

		}
	}
	return ucHeaderType;
}


<<<<<<< HEAD

USHORT	IpVersion6(PMINI_ADAPTER Adapter, /**< Pointer to the driver control structure */
					PVOID pcIpHeader, /**<Pointer to the IP Hdr of the packet*/
					S_CLASSIFIER_RULE *pstClassifierRule )
{
	USHORT	ushDestPort = 0;
	USHORT	ushSrcPort = 0;
	UCHAR   ucNextProtocolAboveIP =0;
	IPV6Header *pstIpv6Header = NULL;
	BOOLEAN bClassificationSucceed = FALSE;

	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "IpVersion6 ==========>\n");

	pstIpv6Header = (IPV6Header *)pcIpHeader;

	DumpIpv6Header(pstIpv6Header);

	//Try to get the next higher layer protocol and the Ports Nos if TCP or UDP
	ucNextProtocolAboveIP = GetIpv6ProtocolPorts((UCHAR *)(pcIpHeader + sizeof(IPV6Header)),
=======
/*
 * Arg 1 struct bcm_mini_adapter *Adapter is a pointer ot the driver contorl structure
 * Arg 2 PVOID pcIpHeader is a pointer to the IP header of the packet
 */
USHORT	IpVersion6(struct bcm_mini_adapter *Adapter, PVOID pcIpHeader,
					struct bcm_classifier_rule *pstClassifierRule)
{
	USHORT	ushDestPort = 0;
	USHORT	ushSrcPort = 0;
	UCHAR   ucNextProtocolAboveIP = 0;
	struct bcm_ipv6_hdr *pstIpv6Header = NULL;
	bool bClassificationSucceed = false;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
			DBG_LVL_ALL, "IpVersion6 ==========>\n");

	pstIpv6Header = pcIpHeader;

	DumpIpv6Header(pstIpv6Header);

	/*
	 * Try to get the next higher layer protocol
	 * and the Ports Nos if TCP or UDP
	 */
	ucNextProtocolAboveIP = GetIpv6ProtocolPorts((UCHAR *)(pcIpHeader + sizeof(struct bcm_ipv6_hdr)),
>>>>>>> refs/remotes/origin/master
							&ushSrcPort,
							&ushDestPort,
							pstIpv6Header->usPayloadLength,
							pstIpv6Header->ucNextHeader);

<<<<<<< HEAD
	do
	{
		if(0 == pstClassifierRule->ucDirection)
		{
			//cannot be processed for classification.
		   // it is a down link connection
			break;
		}

		if(!pstClassifierRule->bIpv6Protocol)
		{
			//We are looking for Ipv6 Classifiers . Lets ignore this classifier and try the next one.
			break;
		}

		bClassificationSucceed=MatchSrcIpv6Address(pstClassifierRule,pstIpv6Header);
        if(!bClassificationSucceed)
            break;

        bClassificationSucceed=MatchDestIpv6Address(pstClassifierRule,pstIpv6Header);
        if(!bClassificationSucceed)
            break;

		//Match the protocol type.For IPv6 the next protocol at end of Chain of IPv6 prot headers
		bClassificationSucceed=MatchProtocol(pstClassifierRule,ucNextProtocolAboveIP);
        if(!bClassificationSucceed)
            break;
        BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 Protocol Matched");

		if((ucNextProtocolAboveIP == TCP_HEADER_TYPE) || (ucNextProtocolAboveIP == UDP_HEADER_TYPE))
		{
			//Match Src Port
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 Source Port:%x\n",ntohs(ushSrcPort));
			bClassificationSucceed=MatchSrcPort(pstClassifierRule,ntohs(ushSrcPort));
			if(!bClassificationSucceed)
				break;

			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 Src Port Matched");

			//Match Dest Port
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 Destination Port:%x\n",ntohs(ushDestPort));
			bClassificationSucceed=MatchDestPort(pstClassifierRule,ntohs(ushDestPort));
			if(!bClassificationSucceed)
				break;
			BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\nIPv6 Dest Port Matched");
		}
	}while(0);

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
			}
=======
	do {
		if (pstClassifierRule->ucDirection == 0) {
			/*
			 * cannot be processed for classification.
			 * it is a down link connection
			 */
			break;
		}

		if (!pstClassifierRule->bIpv6Protocol) {
			/*
			 * We are looking for Ipv6 Classifiers
			 * Lets ignore this classifier and try the next one
			 */
			break;
		}

		bClassificationSucceed = MatchSrcIpv6Address(pstClassifierRule,
								pstIpv6Header);
		if (!bClassificationSucceed)
			break;

		bClassificationSucceed = MatchDestIpv6Address(pstClassifierRule,
								pstIpv6Header);
		if (!bClassificationSucceed)
			break;

		/*
		 * Match the protocol type.
		 * For IPv6 the next protocol at end of
		 * Chain of IPv6 prot headers
		 */
		bClassificationSucceed = MatchProtocol(pstClassifierRule,
							ucNextProtocolAboveIP);
		if (!bClassificationSucceed)
			break;

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
				DBG_LVL_ALL, "\nIPv6 Protocol Matched");

		if ((ucNextProtocolAboveIP == TCP_HEADER_TYPE) ||
			(ucNextProtocolAboveIP == UDP_HEADER_TYPE)) {
			/* Match Src Port */
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Source Port:%x\n",
					ntohs(ushSrcPort));
			bClassificationSucceed = MatchSrcPort(pstClassifierRule,
							ntohs(ushSrcPort));
			if (!bClassificationSucceed)
				break;

			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Src Port Matched");

			/* Match Dest Port */
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Destination Port:%x\n",
					ntohs(ushDestPort));
			bClassificationSucceed = MatchDestPort(pstClassifierRule,
							ntohs(ushDestPort));
			if (!bClassificationSucceed)
				break;
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Dest Port Matched");
		}
	} while (0);

	if (bClassificationSucceed == TRUE) {
		INT iMatchedSFQueueIndex = 0;
		iMatchedSFQueueIndex = SearchSfid(Adapter, pstClassifierRule->ulSFID);
		if (iMatchedSFQueueIndex >= NO_OF_QUEUES) {
			bClassificationSucceed = false;
		} else {
			if (Adapter->PackInfo[iMatchedSFQueueIndex].bActive == false)
				bClassificationSucceed = false;
>>>>>>> refs/remotes/origin/master
		}
	}

	return bClassificationSucceed;
}


<<<<<<< HEAD
static BOOLEAN MatchSrcIpv6Address(S_CLASSIFIER_RULE *pstClassifierRule,IPV6Header *pstIpv6Header)
{
	UINT uiLoopIndex=0;
	UINT  uiIpv6AddIndex=0;
	UINT  uiIpv6AddrNoLongWords = 4;
	ULONG aulSrcIP[4];
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
	/*
	//This is the no. of Src Addresses ie Range of IP Addresses contained
	//in the classifier rule for which we need to match
	*/
	UINT  uiCountIPSrcAddresses = (UINT)pstClassifierRule->ucIPSourceAddressLength;


	if(0 == uiCountIPSrcAddresses)
		return TRUE;


	//First Convert the Ip Address in the packet to Host Endian order
	for(uiIpv6AddIndex=0;uiIpv6AddIndex<uiIpv6AddrNoLongWords;uiIpv6AddIndex++)
	{
		aulSrcIP[uiIpv6AddIndex]=ntohl(pstIpv6Header->ulSrcIpAddress[uiIpv6AddIndex]);
	}

	for(uiLoopIndex=0;uiLoopIndex<uiCountIPSrcAddresses;uiLoopIndex+=uiIpv6AddrNoLongWords)
	{
		BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\n Src Ipv6 Address In Received Packet : \n ");
		DumpIpv6Address(aulSrcIP);
		BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\n Src Ipv6 Mask In Classifier Rule: \n");
		DumpIpv6Address(&pstClassifierRule->stSrcIpAddress.ulIpv6Mask[uiLoopIndex]);
		BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\n Src Ipv6 Address In Classifier Rule : \n");
		DumpIpv6Address(&pstClassifierRule->stSrcIpAddress.ulIpv6Addr[uiLoopIndex]);

		for(uiIpv6AddIndex=0;uiIpv6AddIndex<uiIpv6AddrNoLongWords;uiIpv6AddIndex++)
		{
			if((pstClassifierRule->stSrcIpAddress.ulIpv6Mask[uiLoopIndex+uiIpv6AddIndex] & aulSrcIP[uiIpv6AddIndex])
				!= pstClassifierRule->stSrcIpAddress.ulIpv6Addr[uiLoopIndex+uiIpv6AddIndex])
			{
				//Match failed for current Ipv6 Address.Try next Ipv6 Address
				break;
			}

			if(uiIpv6AddIndex ==  uiIpv6AddrNoLongWords-1)
			{
				//Match Found
				BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Ipv6 Src Ip Address Matched\n");
=======
static bool MatchSrcIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
	struct bcm_ipv6_hdr *pstIpv6Header)
{
	UINT uiLoopIndex = 0;
	UINT uiIpv6AddIndex = 0;
	UINT uiIpv6AddrNoLongWords = 4;
	ULONG aulSrcIP[4];
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	/*
	 * This is the no. of Src Addresses ie Range of IP Addresses contained
	 * in the classifier rule for which we need to match
	 */
	UINT  uiCountIPSrcAddresses = (UINT)pstClassifierRule->ucIPSourceAddressLength;


	if (uiCountIPSrcAddresses == 0)
		return TRUE;


	/* First Convert the Ip Address in the packet to Host Endian order */
	for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++)
		aulSrcIP[uiIpv6AddIndex] = ntohl(pstIpv6Header->ulSrcIpAddress[uiIpv6AddIndex]);

	for (uiLoopIndex = 0; uiLoopIndex < uiCountIPSrcAddresses; uiLoopIndex += uiIpv6AddrNoLongWords) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Src Ipv6 Address In Received Packet :\n ");
		DumpIpv6Address(aulSrcIP);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Src Ipv6 Mask In Classifier Rule:\n");
		DumpIpv6Address(&pstClassifierRule->stSrcIpAddress.ulIpv6Mask[uiLoopIndex]);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Src Ipv6 Address In Classifier Rule :\n");
		DumpIpv6Address(&pstClassifierRule->stSrcIpAddress.ulIpv6Addr[uiLoopIndex]);

		for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++) {
			if ((pstClassifierRule->stSrcIpAddress.ulIpv6Mask[uiLoopIndex+uiIpv6AddIndex] & aulSrcIP[uiIpv6AddIndex])
				!= pstClassifierRule->stSrcIpAddress.ulIpv6Addr[uiLoopIndex+uiIpv6AddIndex]) {
				/*
				 * Match failed for current Ipv6 Address
				 * Try next Ipv6 Address
				 */
				break;
			}

			if (uiIpv6AddIndex ==  uiIpv6AddrNoLongWords-1) {
				/* Match Found */
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
						DBG_LVL_ALL,
						"Ipv6 Src Ip Address Matched\n");
>>>>>>> refs/remotes/origin/master
				return TRUE;
			}
		}
	}
<<<<<<< HEAD
	return FALSE;
}

static BOOLEAN MatchDestIpv6Address(S_CLASSIFIER_RULE *pstClassifierRule,IPV6Header *pstIpv6Header)
{
	UINT uiLoopIndex=0;
	UINT  uiIpv6AddIndex=0;
	UINT  uiIpv6AddrNoLongWords = 4;
	ULONG aulDestIP[4];
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
	/*
	//This is the no. of Destination Addresses ie Range of IP Addresses contained
	//in the classifier rule for which we need to match
	*/
	UINT  uiCountIPDestinationAddresses = (UINT)pstClassifierRule->ucIPDestinationAddressLength;


	if(0 == uiCountIPDestinationAddresses)
		return TRUE;


	//First Convert the Ip Address in the packet to Host Endian order
	for(uiIpv6AddIndex=0;uiIpv6AddIndex<uiIpv6AddrNoLongWords;uiIpv6AddIndex++)
	{
		aulDestIP[uiIpv6AddIndex]=ntohl(pstIpv6Header->ulDestIpAddress[uiIpv6AddIndex]);
	}

	for(uiLoopIndex=0;uiLoopIndex<uiCountIPDestinationAddresses;uiLoopIndex+=uiIpv6AddrNoLongWords)
	{
		BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\n Destination Ipv6 Address In Received Packet : \n ");
		DumpIpv6Address(aulDestIP);
		BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\n Destination Ipv6 Mask In Classifier Rule: \n");
		DumpIpv6Address(&pstClassifierRule->stDestIpAddress.ulIpv6Mask[uiLoopIndex]);
		BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "\n Destination Ipv6 Address In Classifier Rule : \n");
		DumpIpv6Address(&pstClassifierRule->stDestIpAddress.ulIpv6Addr[uiLoopIndex]);

		for(uiIpv6AddIndex=0;uiIpv6AddIndex<uiIpv6AddrNoLongWords;uiIpv6AddIndex++)
		{
			if((pstClassifierRule->stDestIpAddress.ulIpv6Mask[uiLoopIndex+uiIpv6AddIndex] & aulDestIP[uiIpv6AddIndex])
				!= pstClassifierRule->stDestIpAddress.ulIpv6Addr[uiLoopIndex+uiIpv6AddIndex])
			{
				//Match failed for current Ipv6 Address.Try next Ipv6 Address
				break;
			}

			if(uiIpv6AddIndex ==  uiIpv6AddrNoLongWords-1)
			{
				//Match Found
				BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Ipv6 Destination Ip Address Matched\n");
=======
	return false;
}

static bool MatchDestIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
	struct bcm_ipv6_hdr *pstIpv6Header)
{
	UINT uiLoopIndex = 0;
	UINT uiIpv6AddIndex = 0;
	UINT uiIpv6AddrNoLongWords = 4;
	ULONG aulDestIP[4];
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	/*
	 * This is the no. of Destination Addresses
	 * ie Range of IP Addresses contained in the classifier rule
	 * for which we need to match
	 */
	UINT  uiCountIPDestinationAddresses = (UINT)pstClassifierRule->ucIPDestinationAddressLength;


	if (uiCountIPDestinationAddresses == 0)
		return TRUE;


	/* First Convert the Ip Address in the packet to Host Endian order */
	for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++)
		aulDestIP[uiIpv6AddIndex] = ntohl(pstIpv6Header->ulDestIpAddress[uiIpv6AddIndex]);

	for (uiLoopIndex = 0; uiLoopIndex < uiCountIPDestinationAddresses; uiLoopIndex += uiIpv6AddrNoLongWords) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Destination Ipv6 Address In Received Packet :\n ");
		DumpIpv6Address(aulDestIP);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Destination Ipv6 Mask In Classifier Rule :\n");
		DumpIpv6Address(&pstClassifierRule->stDestIpAddress.ulIpv6Mask[uiLoopIndex]);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Destination Ipv6 Address In Classifier Rule :\n");
		DumpIpv6Address(&pstClassifierRule->stDestIpAddress.ulIpv6Addr[uiLoopIndex]);

		for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++) {
			if ((pstClassifierRule->stDestIpAddress.ulIpv6Mask[uiLoopIndex+uiIpv6AddIndex] & aulDestIP[uiIpv6AddIndex])
				!= pstClassifierRule->stDestIpAddress.ulIpv6Addr[uiLoopIndex+uiIpv6AddIndex]) {
				/*
				 * Match failed for current Ipv6 Address.
				 * Try next Ipv6 Address
				 */
				break;
			}

			if (uiIpv6AddIndex ==  uiIpv6AddrNoLongWords-1) {
				/* Match Found */
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
						DBG_LVL_ALL,
						"Ipv6 Destination Ip Address Matched\n");
>>>>>>> refs/remotes/origin/master
				return TRUE;
			}
		}
	}
<<<<<<< HEAD
	return FALSE;
=======
	return false;
>>>>>>> refs/remotes/origin/master

}

VOID DumpIpv6Address(ULONG *puIpv6Address)
{
	UINT uiIpv6AddrNoLongWords = 4;
<<<<<<< HEAD
	UINT  uiIpv6AddIndex=0;
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
	for(uiIpv6AddIndex=0;uiIpv6AddIndex<uiIpv6AddrNoLongWords;uiIpv6AddIndex++)
	{
		BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, ":%lx",puIpv6Address[uiIpv6AddIndex]);
=======
	UINT uiIpv6AddIndex = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				":%lx", puIpv6Address[uiIpv6AddIndex]);
>>>>>>> refs/remotes/origin/master
	}

}

<<<<<<< HEAD
static VOID DumpIpv6Header(IPV6Header *pstIpv6Header)
{
	UCHAR ucVersion;
	UCHAR  ucPrio ;
    PMINI_ADAPTER Adapter = GET_BCM_ADAPTER(gblpnetdev);
	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "----Ipv6 Header---");
	ucVersion = pstIpv6Header->ucVersionPrio & 0xf0;
	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Version : %x \n",ucVersion);
	ucPrio = pstIpv6Header->ucVersionPrio & 0x0f;
	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Priority : %x \n",ucPrio);
	//BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Flow Label : %x \n",(pstIpv6Header->ucVersionPrio &0xf0);
	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Payload Length : %x \n",ntohs(pstIpv6Header->usPayloadLength));
	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Next Header : %x \n",pstIpv6Header->ucNextHeader);
	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Hop Limit : %x \n",pstIpv6Header->ucHopLimit);
	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Src Address :\n");
	DumpIpv6Address(pstIpv6Header->ulSrcIpAddress);
	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "Dest Address :\n");
	DumpIpv6Address(pstIpv6Header->ulDestIpAddress);
	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL, "----Ipv6 Header End---");
=======
static VOID DumpIpv6Header(struct bcm_ipv6_hdr *pstIpv6Header)
{
	UCHAR ucVersion;
	UCHAR ucPrio;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"----Ipv6 Header---");
	ucVersion = pstIpv6Header->ucVersionPrio & 0xf0;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Version : %x\n", ucVersion);
	ucPrio = pstIpv6Header->ucVersionPrio & 0x0f;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Priority : %x\n", ucPrio);
	/*
	 * BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
	 * "Flow Label : %x\n",(pstIpv6Header->ucVersionPrio &0xf0);
	 */
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Payload Length : %x\n",
			ntohs(pstIpv6Header->usPayloadLength));
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Next Header : %x\n", pstIpv6Header->ucNextHeader);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Hop Limit : %x\n", pstIpv6Header->ucHopLimit);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Src Address :\n");
	DumpIpv6Address(pstIpv6Header->ulSrcIpAddress);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Dest Address :\n");
	DumpIpv6Address(pstIpv6Header->ulDestIpAddress);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"----Ipv6 Header End---");
>>>>>>> refs/remotes/origin/master


}
