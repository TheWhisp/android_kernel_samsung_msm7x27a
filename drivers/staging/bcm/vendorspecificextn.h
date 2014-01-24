
#ifndef __VENDOR_EXTN_NVM_H__
#define __VENDOR_EXTN_NVM_H__

#define CONTINUE_COMMON_PATH 0xFFFF

<<<<<<< HEAD
INT vendorextnGetSectionInfo(PVOID  pContext,PFLASH2X_VENDORSPECIFIC_INFO pVendorInfo);
INT vendorextnExit(PMINI_ADAPTER Adapter);
INT vendorextnInit(PMINI_ADAPTER Adapter);
INT vendorextnIoctl(PMINI_ADAPTER Adapter, UINT cmd, ULONG arg);
INT vendorextnReadSection(PVOID  pContext, PUCHAR pBuffer, FLASH2X_SECTION_VAL SectionVal,
			UINT offset, UINT numOfBytes);
INT vendorextnWriteSection(PVOID  pContext, PUCHAR pBuffer, FLASH2X_SECTION_VAL SectionVal,
			UINT offset, UINT numOfBytes, BOOLEAN bVerify);
INT vendorextnWriteSectionWithoutErase(PVOID  pContext, PUCHAR pBuffer, FLASH2X_SECTION_VAL SectionVal,
=======
INT vendorextnGetSectionInfo(PVOID  pContext, struct bcm_flash2x_vendor_info *pVendorInfo);
INT vendorextnExit(struct bcm_mini_adapter *Adapter);
INT vendorextnInit(struct bcm_mini_adapter *Adapter);
INT vendorextnIoctl(struct bcm_mini_adapter *Adapter, UINT cmd, ULONG arg);
INT vendorextnReadSection(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
			UINT offset, UINT numOfBytes);
INT vendorextnWriteSection(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
			UINT offset, UINT numOfBytes, bool bVerify);
INT vendorextnWriteSectionWithoutErase(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
>>>>>>> refs/remotes/origin/master
			UINT offset, UINT numOfBytes);

#endif /*  */
