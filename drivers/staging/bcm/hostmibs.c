<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * File Name: hostmibs.c
 *
 * Author: Beceem Communications Pvt. Ltd
 *
 * Abstract: This file contains the routines to copy the statistics used by
 * the driver to the Host MIBS structure and giving the same to Application.
<<<<<<< HEAD
<<<<<<< HEAD
 *
 */
#include "headers.h"

INT  ProcessGetHostMibs(PMINI_ADAPTER Adapter, S_MIBS_HOST_STATS_MIBS *pstHostMibs)
{
	S_SERVICEFLOW_ENTRY    *pstServiceFlowEntry = NULL;
	S_PHS_RULE             *pstPhsRule          = NULL;
	S_CLASSIFIER_TABLE     *pstClassifierTable  = NULL;
	S_CLASSIFIER_ENTRY     *pstClassifierRule   = NULL;
	PPHS_DEVICE_EXTENSION  pDeviceExtension     = (PPHS_DEVICE_EXTENSION)&Adapter->stBCMPhsContext;

	UINT nClassifierIndex = 0, nPhsTableIndex = 0,nSfIndex = 0, uiIndex = 0;

	if(pDeviceExtension == NULL)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, HOST_MIBS, DBG_LVL_ALL, "Invalid Device Extension\n");
		return STATUS_FAILURE;
	}

	//Copy the classifier Table
	for(nClassifierIndex=0; nClassifierIndex < MAX_CLASSIFIERS;
			nClassifierIndex++)
	{
		if(Adapter->astClassifierTable[nClassifierIndex].bUsed == TRUE)
			memcpy((PVOID)&pstHostMibs->astClassifierTable[nClassifierIndex],
				(PVOID)&Adapter->astClassifierTable[nClassifierIndex],
				sizeof(S_MIBS_CLASSIFIER_RULE));
	}

  //Copy the SF Table
	for(nSfIndex=0; nSfIndex < NO_OF_QUEUES ; nSfIndex++)
	{
	if(Adapter->PackInfo[nSfIndex].bValid)
	{
			memcpy((PVOID)&pstHostMibs->astSFtable[nSfIndex],(PVOID)&Adapter->PackInfo[nSfIndex],sizeof(S_MIBS_SERVICEFLOW_TABLE));
	}
	else
	{
		//if index in not valid, don't process this for the PHS table. Go For the next entry.
		continue ;
	}

		//Retrieve the SFID Entry Index for requested Service Flow
		if(PHS_INVALID_TABLE_INDEX == GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable,
						  Adapter->PackInfo[nSfIndex].usVCID_Value ,&pstServiceFlowEntry))
		{

		continue;
		}

		pstClassifierTable = pstServiceFlowEntry->pstClassifierTable;


		for(uiIndex = 0; uiIndex < MAX_PHSRULE_PER_SF; uiIndex++)
		{
			pstClassifierRule = &pstClassifierTable->stActivePhsRulesList[uiIndex];

		if(pstClassifierRule->bUsed)
		{
			pstPhsRule = pstClassifierRule->pstPhsRule;

			pstHostMibs->astPhsRulesTable[nPhsTableIndex].ulSFID = Adapter->PackInfo[nSfIndex].ulSFID;

			memcpy(&pstHostMibs->astPhsRulesTable[nPhsTableIndex].u8PHSI,
						&pstPhsRule->u8PHSI,
						sizeof(S_PHS_RULE));
=======
=======
>>>>>>> refs/remotes/origin/master
 */

#include "headers.h"

<<<<<<< HEAD
INT ProcessGetHostMibs(PMINI_ADAPTER Adapter, S_MIBS_HOST_STATS_MIBS *pstHostMibs)
{
	S_SERVICEFLOW_ENTRY *pstServiceFlowEntry = NULL;
	S_PHS_RULE *pstPhsRule = NULL;
	S_CLASSIFIER_TABLE *pstClassifierTable = NULL;
	S_CLASSIFIER_ENTRY *pstClassifierRule = NULL;
	PPHS_DEVICE_EXTENSION pDeviceExtension = (PPHS_DEVICE_EXTENSION) &Adapter->stBCMPhsContext;
=======
INT ProcessGetHostMibs(struct bcm_mini_adapter *Adapter, struct bcm_host_stats_mibs *pstHostMibs)
{
	struct bcm_phs_entry *pstServiceFlowEntry = NULL;
	struct bcm_phs_rule *pstPhsRule = NULL;
	struct bcm_phs_classifier_table *pstClassifierTable = NULL;
	struct bcm_phs_classifier_entry *pstClassifierRule = NULL;
	struct bcm_phs_extension *pDeviceExtension = (struct bcm_phs_extension *) &Adapter->stBCMPhsContext;
>>>>>>> refs/remotes/origin/master

	UINT nClassifierIndex = 0, nPhsTableIndex = 0, nSfIndex = 0, uiIndex = 0;

	if (pDeviceExtension == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, HOST_MIBS, DBG_LVL_ALL, "Invalid Device Extension\n");
		return STATUS_FAILURE;
	}

	/* Copy the classifier Table */
	for (nClassifierIndex = 0; nClassifierIndex < MAX_CLASSIFIERS; nClassifierIndex++) {
		if (Adapter->astClassifierTable[nClassifierIndex].bUsed == TRUE)
			memcpy((PVOID) & pstHostMibs->
			       astClassifierTable[nClassifierIndex],
			       (PVOID) & Adapter->
			       astClassifierTable[nClassifierIndex],
<<<<<<< HEAD
			       sizeof(S_MIBS_CLASSIFIER_RULE));
=======
			       sizeof(struct bcm_mibs_classifier_rule));
>>>>>>> refs/remotes/origin/master
	}

	/* Copy the SF Table */
	for (nSfIndex = 0; nSfIndex < NO_OF_QUEUES; nSfIndex++) {
		if (Adapter->PackInfo[nSfIndex].bValid) {
			memcpy((PVOID) & pstHostMibs->astSFtable[nSfIndex],
			       (PVOID) & Adapter->PackInfo[nSfIndex],
<<<<<<< HEAD
			       sizeof(S_MIBS_SERVICEFLOW_TABLE));
=======
				sizeof(struct bcm_mibs_table));
>>>>>>> refs/remotes/origin/master
		} else {
			/* If index in not valid,
			 * don't process this for the PHS table.
			 * Go For the next entry.
			 */
			continue;
		}

		/* Retrieve the SFID Entry Index for requested Service Flow */
		if (PHS_INVALID_TABLE_INDEX ==
		    GetServiceFlowEntry(pDeviceExtension->
					pstServiceFlowPhsRulesTable,
					Adapter->PackInfo[nSfIndex].
					usVCID_Value, &pstServiceFlowEntry))

			continue;

		pstClassifierTable = pstServiceFlowEntry->pstClassifierTable;

		for (uiIndex = 0; uiIndex < MAX_PHSRULE_PER_SF; uiIndex++) {
			pstClassifierRule = &pstClassifierTable->stActivePhsRulesList[uiIndex];

			if (pstClassifierRule->bUsed) {
				pstPhsRule = pstClassifierRule->pstPhsRule;

				pstHostMibs->astPhsRulesTable[nPhsTableIndex].
				    ulSFID = Adapter->PackInfo[nSfIndex].ulSFID;

				memcpy(&pstHostMibs->
				       astPhsRulesTable[nPhsTableIndex].u8PHSI,
<<<<<<< HEAD
				       &pstPhsRule->u8PHSI, sizeof(S_PHS_RULE));
>>>>>>> refs/remotes/origin/cm-10.0
=======
				       &pstPhsRule->u8PHSI, sizeof(struct bcm_phs_rule));
>>>>>>> refs/remotes/origin/master
				nPhsTableIndex++;

			}

		}

	}

<<<<<<< HEAD
<<<<<<< HEAD

	//copy other Host Statistics parameters
	pstHostMibs->stHostInfo.GoodTransmits = Adapter->dev->stats.tx_packets;
	pstHostMibs->stHostInfo.GoodReceives = Adapter->dev->stats.rx_packets;
	pstHostMibs->stHostInfo.CurrNumFreeDesc =
			atomic_read(&Adapter->CurrNumFreeTxDesc);
=======
=======
>>>>>>> refs/remotes/origin/master
	/* Copy other Host Statistics parameters */
	pstHostMibs->stHostInfo.GoodTransmits = Adapter->dev->stats.tx_packets;
	pstHostMibs->stHostInfo.GoodReceives = Adapter->dev->stats.rx_packets;
	pstHostMibs->stHostInfo.CurrNumFreeDesc = atomic_read(&Adapter->CurrNumFreeTxDesc);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	pstHostMibs->stHostInfo.BEBucketSize = Adapter->BEBucketSize;
	pstHostMibs->stHostInfo.rtPSBucketSize = Adapter->rtPSBucketSize;
	pstHostMibs->stHostInfo.TimerActive = Adapter->TimerActive;
	pstHostMibs->stHostInfo.u32TotalDSD = Adapter->u32TotalDSD;

<<<<<<< HEAD
<<<<<<< HEAD
	memcpy(pstHostMibs->stHostInfo.aTxPktSizeHist,Adapter->aTxPktSizeHist,sizeof(UINT32)*MIBS_MAX_HIST_ENTRIES);
	memcpy(pstHostMibs->stHostInfo.aRxPktSizeHist,Adapter->aRxPktSizeHist,sizeof(UINT32)*MIBS_MAX_HIST_ENTRIES);
=======
	memcpy(pstHostMibs->stHostInfo.aTxPktSizeHist, Adapter->aTxPktSizeHist, sizeof(UINT32) * MIBS_MAX_HIST_ENTRIES);
	memcpy(pstHostMibs->stHostInfo.aRxPktSizeHist, Adapter->aRxPktSizeHist, sizeof(UINT32) * MIBS_MAX_HIST_ENTRIES);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memcpy(pstHostMibs->stHostInfo.aTxPktSizeHist, Adapter->aTxPktSizeHist, sizeof(UINT32) * MIBS_MAX_HIST_ENTRIES);
	memcpy(pstHostMibs->stHostInfo.aRxPktSizeHist, Adapter->aRxPktSizeHist, sizeof(UINT32) * MIBS_MAX_HIST_ENTRIES);
>>>>>>> refs/remotes/origin/master

	return STATUS_SUCCESS;
}

<<<<<<< HEAD
<<<<<<< HEAD

VOID GetDroppedAppCntrlPktMibs(S_MIBS_HOST_STATS_MIBS *pstHostMibs, const PPER_TARANG_DATA pTarang)
{
	memcpy(&(pstHostMibs->stDroppedAppCntrlMsgs),
	       &(pTarang->stDroppedAppCntrlMsgs),sizeof(S_MIBS_DROPPED_APP_CNTRL_MESSAGES));
}


VOID CopyMIBSExtendedSFParameters(PMINI_ADAPTER Adapter,
		CServiceFlowParamSI *psfLocalSet, UINT uiSearchRuleIndex)
{
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfSfid = psfLocalSet->u32SFID;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsMaxSustainedRate = psfLocalSet->u32MaxSustainedTrafficRate;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsMaxTrafficBurst = psfLocalSet->u32MaxTrafficBurst;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsMinReservedRate = psfLocalSet->u32MinReservedTrafficRate;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsToleratedJitter = psfLocalSet->u32ToleratedJitter;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsMaxLatency = psfLocalSet->u32MaximumLatency;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsFixedVsVariableSduInd = psfLocalSet->u8FixedLengthVSVariableLengthSDUIndicator;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsFixedVsVariableSduInd = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsFixedVsVariableSduInd);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsSduSize = psfLocalSet->u8SDUSize;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsSduSize = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsSduSize);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsSfSchedulingType = psfLocalSet->u8ServiceFlowSchedulingType;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsSfSchedulingType = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsSfSchedulingType);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqEnable = psfLocalSet->u8ARQEnable;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqEnable = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqEnable);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqWindowSize = ntohs(psfLocalSet->u16ARQWindowSize);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqWindowSize = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqWindowSize);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqBlockLifetime = ntohs(psfLocalSet->u16ARQBlockLifeTime);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqBlockLifetime = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqBlockLifetime);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqSyncLossTimeout = ntohs(psfLocalSet->u16ARQSyncLossTimeOut);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqSyncLossTimeout = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqSyncLossTimeout);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqDeliverInOrder = psfLocalSet->u8ARQDeliverInOrder;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqDeliverInOrder = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqDeliverInOrder);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqRxPurgeTimeout = ntohs(psfLocalSet->u16ARQRxPurgeTimeOut);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqRxPurgeTimeout = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqRxPurgeTimeout);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqBlockSize = ntohs(psfLocalSet->u16ARQBlockSize);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqBlockSize = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsArqBlockSize);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsReqTxPolicy = psfLocalSet->u8RequesttransmissionPolicy;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsReqTxPolicy = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsReqTxPolicy);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnSfCsSpecification = psfLocalSet->u8CSSpecification;
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnSfCsSpecification = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnSfCsSpecification);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsTargetSaid = ntohs(psfLocalSet->u16TargetSAID);
	Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsTargetSaid = ntohl(Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable.wmanIfCmnCpsTargetSaid);
=======
VOID GetDroppedAppCntrlPktMibs(S_MIBS_HOST_STATS_MIBS *pstHostMibs, const PPER_TARANG_DATA pTarang)
{
	memcpy(&(pstHostMibs->stDroppedAppCntrlMsgs),
	       &(pTarang->stDroppedAppCntrlMsgs),
	       sizeof(S_MIBS_DROPPED_APP_CNTRL_MESSAGES));
}

VOID CopyMIBSExtendedSFParameters(PMINI_ADAPTER Adapter, CServiceFlowParamSI *psfLocalSet, UINT uiSearchRuleIndex)
{
	S_MIBS_EXTSERVICEFLOW_PARAMETERS *t = &Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable;
=======
VOID GetDroppedAppCntrlPktMibs(struct bcm_host_stats_mibs *pstHostMibs, struct bcm_tarang_data *pTarang)
{
	memcpy(&(pstHostMibs->stDroppedAppCntrlMsgs),
	       &(pTarang->stDroppedAppCntrlMsgs),
	       sizeof(struct bcm_mibs_dropped_cntrl_msg));
}

VOID CopyMIBSExtendedSFParameters(struct bcm_mini_adapter *Adapter, struct bcm_connect_mgr_params *psfLocalSet, UINT uiSearchRuleIndex)
{
	struct bcm_mibs_parameters *t = &Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable;
>>>>>>> refs/remotes/origin/master

	t->wmanIfSfid = psfLocalSet->u32SFID;
	t->wmanIfCmnCpsMaxSustainedRate = psfLocalSet->u32MaxSustainedTrafficRate;
	t->wmanIfCmnCpsMaxTrafficBurst = psfLocalSet->u32MaxTrafficBurst;
	t->wmanIfCmnCpsMinReservedRate = psfLocalSet->u32MinReservedTrafficRate;
	t->wmanIfCmnCpsToleratedJitter = psfLocalSet->u32ToleratedJitter;
	t->wmanIfCmnCpsMaxLatency = psfLocalSet->u32MaximumLatency;
	t->wmanIfCmnCpsFixedVsVariableSduInd = psfLocalSet->u8FixedLengthVSVariableLengthSDUIndicator;
	t->wmanIfCmnCpsFixedVsVariableSduInd = ntohl(t->wmanIfCmnCpsFixedVsVariableSduInd);
	t->wmanIfCmnCpsSduSize = psfLocalSet->u8SDUSize;
	t->wmanIfCmnCpsSduSize = ntohl(t->wmanIfCmnCpsSduSize);
	t->wmanIfCmnCpsSfSchedulingType = psfLocalSet->u8ServiceFlowSchedulingType;
	t->wmanIfCmnCpsSfSchedulingType = ntohl(t->wmanIfCmnCpsSfSchedulingType);
	t->wmanIfCmnCpsArqEnable = psfLocalSet->u8ARQEnable;
	t->wmanIfCmnCpsArqEnable = ntohl(t->wmanIfCmnCpsArqEnable);
	t->wmanIfCmnCpsArqWindowSize = ntohs(psfLocalSet->u16ARQWindowSize);
	t->wmanIfCmnCpsArqWindowSize = ntohl(t->wmanIfCmnCpsArqWindowSize);
	t->wmanIfCmnCpsArqBlockLifetime = ntohs(psfLocalSet->u16ARQBlockLifeTime);
	t->wmanIfCmnCpsArqBlockLifetime = ntohl(t->wmanIfCmnCpsArqBlockLifetime);
	t->wmanIfCmnCpsArqSyncLossTimeout = ntohs(psfLocalSet->u16ARQSyncLossTimeOut);
	t->wmanIfCmnCpsArqSyncLossTimeout = ntohl(t->wmanIfCmnCpsArqSyncLossTimeout);
	t->wmanIfCmnCpsArqDeliverInOrder = psfLocalSet->u8ARQDeliverInOrder;
	t->wmanIfCmnCpsArqDeliverInOrder = ntohl(t->wmanIfCmnCpsArqDeliverInOrder);
	t->wmanIfCmnCpsArqRxPurgeTimeout = ntohs(psfLocalSet->u16ARQRxPurgeTimeOut);
	t->wmanIfCmnCpsArqRxPurgeTimeout = ntohl(t->wmanIfCmnCpsArqRxPurgeTimeout);
	t->wmanIfCmnCpsArqBlockSize = ntohs(psfLocalSet->u16ARQBlockSize);
	t->wmanIfCmnCpsArqBlockSize = ntohl(t->wmanIfCmnCpsArqBlockSize);
	t->wmanIfCmnCpsReqTxPolicy = psfLocalSet->u8RequesttransmissionPolicy;
	t->wmanIfCmnCpsReqTxPolicy = ntohl(t->wmanIfCmnCpsReqTxPolicy);
	t->wmanIfCmnSfCsSpecification = psfLocalSet->u8CSSpecification;
	t->wmanIfCmnSfCsSpecification = ntohl(t->wmanIfCmnSfCsSpecification);
	t->wmanIfCmnCpsTargetSaid = ntohs(psfLocalSet->u16TargetSAID);
	t->wmanIfCmnCpsTargetSaid = ntohl(t->wmanIfCmnCpsTargetSaid);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

}
