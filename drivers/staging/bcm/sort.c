#include "headers.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/sort.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/sort.h>
>>>>>>> refs/remotes/origin/master

/*
 * File Name: sort.c
 *
 * Author: Beceem Communications Pvt. Ltd
 *
 * Abstract: This file contains the routines sorting the classification rules.
 *
 * Copyright (c) 2007 Beceem Communications Pvt. Ltd
 */

<<<<<<< HEAD
<<<<<<< HEAD
VOID SortPackInfo(PMINI_ADAPTER Adapter)
{
	UINT nIndex1;
	UINT nIndex2;

	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_OTHERS, CONN_MSG, DBG_LVL_ALL, "<=======");

	for(nIndex1 = 0; nIndex1 < NO_OF_QUEUES -2 ; nIndex1++)
	{
		for(nIndex2 = nIndex1 + 1 ; nIndex2 < NO_OF_QUEUES -1  ; nIndex2++)
		{
			if(Adapter->PackInfo[nIndex1].bValid && Adapter->PackInfo[nIndex2].bValid)
			{
				if(Adapter->PackInfo[nIndex2].u8TrafficPriority <
						Adapter->PackInfo[nIndex1].u8TrafficPriority)
				{
					PacketInfo stTemppackInfo = Adapter->PackInfo[nIndex2];
					Adapter->PackInfo[nIndex2] = Adapter->PackInfo[nIndex1];
					Adapter->PackInfo[nIndex1] = stTemppackInfo;

				}
			}
		}
	}
=======
static int compare_packet_info(void const *a, void const *b)
{
	PacketInfo const *pa = a;
	PacketInfo const *pb = b;
=======
static int compare_packet_info(void const *a, void const *b)
{
	struct bcm_packet_info const *pa = a;
	struct bcm_packet_info const *pb = b;
>>>>>>> refs/remotes/origin/master

	if (!pa->bValid || !pb->bValid)
		return 0;

	return pa->u8TrafficPriority - pb->u8TrafficPriority;
}

<<<<<<< HEAD
VOID SortPackInfo(PMINI_ADAPTER Adapter)
=======
VOID SortPackInfo(struct bcm_mini_adapter *Adapter)
>>>>>>> refs/remotes/origin/master
{
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, CONN_MSG,
			DBG_LVL_ALL, "<=======");

<<<<<<< HEAD
	sort(Adapter->PackInfo, NO_OF_QUEUES, sizeof(PacketInfo),
=======
	sort(Adapter->PackInfo, NO_OF_QUEUES, sizeof(struct bcm_packet_info),
>>>>>>> refs/remotes/origin/master
		compare_packet_info, NULL);
}

static int compare_classifiers(void const *a, void const *b)
{
<<<<<<< HEAD
	S_CLASSIFIER_RULE const *pa = a;
	S_CLASSIFIER_RULE const *pb = b;
=======
	struct bcm_classifier_rule const *pa = a;
	struct bcm_classifier_rule const *pb = b;
>>>>>>> refs/remotes/origin/master

	if (!pa->bUsed || !pb->bUsed)
		return 0;

	return pa->u8ClassifierRulePriority - pb->u8ClassifierRulePriority;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

VOID SortClassifiers(PMINI_ADAPTER Adapter)
{
<<<<<<< HEAD
	UINT nIndex1;
	UINT nIndex2;

	BCM_DEBUG_PRINT( Adapter,DBG_TYPE_OTHERS, CONN_MSG, DBG_LVL_ALL, "<=======");

	for(nIndex1 = 0; nIndex1 < MAX_CLASSIFIERS -1 ; nIndex1++)
	{
		for(nIndex2 = nIndex1 + 1 ; nIndex2 < MAX_CLASSIFIERS  ; nIndex2++)
		{
			if(Adapter->astClassifierTable[nIndex1].bUsed && Adapter->astClassifierTable[nIndex2].bUsed)
			{
				if(Adapter->astClassifierTable[nIndex2].u8ClassifierRulePriority <
					Adapter->astClassifierTable[nIndex1].u8ClassifierRulePriority)
				{
					S_CLASSIFIER_RULE stTempClassifierRule = Adapter->astClassifierTable[nIndex2];
					Adapter->astClassifierTable[nIndex2] = Adapter->astClassifierTable[nIndex1];
					Adapter->astClassifierTable[nIndex1] = stTempClassifierRule;

				}
			}
		}
	}
=======
=======
}

VOID SortClassifiers(struct bcm_mini_adapter *Adapter)
{
>>>>>>> refs/remotes/origin/master
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, CONN_MSG,
			DBG_LVL_ALL, "<=======");

	sort(Adapter->astClassifierTable, MAX_CLASSIFIERS,
<<<<<<< HEAD
		sizeof(S_CLASSIFIER_RULE), compare_classifiers, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		sizeof(struct bcm_classifier_rule), compare_classifiers, NULL);
>>>>>>> refs/remotes/origin/master
}
