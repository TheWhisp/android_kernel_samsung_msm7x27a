/* arch/arm/mach-msm/smd_private.h
 *
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2007-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2007-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2007 QUALCOMM Incorporated
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2007-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _ARCH_ARM_MACH_MSM_MSM_SMD_PRIVATE_H_
#define _ARCH_ARM_MACH_MSM_MSM_SMD_PRIVATE_H_

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/types.h>
#include <linux/spinlock.h>
#include <mach/msm_smsm.h>
<<<<<<< HEAD
=======
#include <mach/msm_smd.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "smem_vendor_type.h"

#define PC_APPS  0
#define PC_MODEM 1

#define VERSION_QDSP6     4
#define VERSION_APPS_SBL  6
#define VERSION_MODEM_SBL 7
#define VERSION_APPS      8
#define VERSION_MODEM     9
#define VERSION_DSPS      10

#define SMD_HEAP_SIZE 512
=======
#include <linux/platform_device.h>
=======
#include <linux/types.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux/spinlock.h>
#include <mach/msm_smsm.h>
#include <mach/msm_smd.h>
#include "smem_vendor_type.h"

<<<<<<< HEAD
#include <mach/msm_iomap.h>
>>>>>>> refs/remotes/origin/master
=======
#define PC_APPS  0
#define PC_MODEM 1

#define VERSION_QDSP6     4
#define VERSION_APPS_SBL  6
#define VERSION_MODEM_SBL 7
#define VERSION_APPS      8
#define VERSION_MODEM     9
#define VERSION_DSPS      10

#define SMD_HEAP_SIZE 512
>>>>>>> refs/remotes/origin/cm-11.0

struct smem_heap_info {
	unsigned initialized;
	unsigned free_offset;
	unsigned heap_remaining;
	unsigned reserved;
};

struct smem_heap_entry {
	unsigned allocated;
	unsigned offset;
	unsigned size;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned reserved;
};
=======
	unsigned reserved; /* bits 1:0 reserved, bits 31:2 aux smem base addr */
};
#define BASE_ADDR_MASK 0xfffffffc
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned reserved;
};
>>>>>>> refs/remotes/origin/master
=======
	unsigned reserved; /* bits 1:0 reserved, bits 31:2 aux smem base addr */
};
#define BASE_ADDR_MASK 0xfffffffc
>>>>>>> refs/remotes/origin/cm-11.0

struct smem_proc_comm {
	unsigned command;
	unsigned status;
	unsigned data1;
	unsigned data2;
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define PC_APPS  0
#define PC_MODEM 1

#define VERSION_SMD       0
#define VERSION_QDSP6     4
#define VERSION_APPS_SBL  6
#define VERSION_MODEM_SBL 7
#define VERSION_APPS      8
#define VERSION_MODEM     9

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct smem_shared {
	struct smem_proc_comm proc_comm[4];
	unsigned version[32];
	struct smem_heap_info heap_info;
<<<<<<< HEAD
<<<<<<< HEAD
	struct smem_heap_entry heap_toc[SMD_HEAP_SIZE];
};

#if defined(CONFIG_MSM_SMD_PKG4)
struct smsm_interrupt_info {
	uint32_t aArm_en_mask;
	uint32_t aArm_interrupts_pending;
	uint32_t aArm_wakeup_reason;
	uint32_t aArm_rpc_prog;
	uint32_t aArm_rpc_proc;
	char aArm_smd_port_name[20];
	uint32_t aArm_gpio_info;
};
#elif defined(CONFIG_MSM_SMD_PKG3)
struct smsm_interrupt_info {
  uint32_t aArm_en_mask;
  uint32_t aArm_interrupts_pending;
  uint32_t aArm_wakeup_reason;
};
#elif !defined(CONFIG_MSM_SMD)
<<<<<<< HEAD
void *smem_alloc(unsigned id, unsigned size)
{
	return NULL;
}
=======
/* Don't trigger the error */
>>>>>>> refs/remotes/origin/cm-10.0
#else
#error No SMD Package Specified; aborting
=======
	struct smem_heap_entry heap_toc[512];
=======
	struct smem_heap_entry heap_toc[SMD_HEAP_SIZE];
>>>>>>> refs/remotes/origin/cm-11.0
};

#if defined(CONFIG_MSM_SMD_PKG4)
struct smsm_interrupt_info {
	uint32_t aArm_en_mask;
	uint32_t aArm_interrupts_pending;
	uint32_t aArm_wakeup_reason;
	uint32_t aArm_rpc_prog;
	uint32_t aArm_rpc_proc;
	char aArm_smd_port_name[20];
	uint32_t aArm_gpio_info;
};
#elif defined(CONFIG_MSM_SMD_PKG3)
struct smsm_interrupt_info {
  uint32_t aArm_en_mask;
  uint32_t aArm_interrupts_pending;
  uint32_t aArm_wakeup_reason;
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
#elif !defined(CONFIG_MSM_SMD)
/* Don't trigger the error */
#else
#error No SMD Package Specified; aborting
>>>>>>> refs/remotes/origin/cm-11.0
#endif

#define SZ_DIAG_ERR_MSG 0xC8
#define ID_DIAG_ERR_MSG SMEM_DIAG_ERR_MESSAGE
#define ID_SMD_CHANNELS SMEM_SMD_BASE_ID
#define ID_SHARED_STATE SMEM_SMSM_SHARED_STATE
#define ID_CH_ALLOC_TBL SMEM_CHANNEL_ALLOC_TBL

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define SMD_SS_CLOSED            0x00000000
#define SMD_SS_OPENING           0x00000001
#define SMD_SS_OPENED            0x00000002
#define SMD_SS_FLUSHING          0x00000003
#define SMD_SS_CLOSING           0x00000004
#define SMD_SS_RESET             0x00000005
#define SMD_SS_RESET_OPENING     0x00000006

#define SMD_BUF_SIZE             8192
#define SMD_CHANNELS             64
#define SMD_HEADER_SIZE          20

/* 'type' field of smd_alloc_elm structure
 * has the following breakup
 * bits 0-7   -> channel type
 * bits 8-11  -> xfer type
 * bits 12-31 -> reserved
 */
<<<<<<< HEAD
struct smd_alloc_elm {
	char name[20];
	uint32_t cid;
	uint32_t type;
	uint32_t ref_count;
};

#define SMD_CHANNEL_TYPE(x) ((x) & 0x000000FF)
#define SMD_XFER_TYPE(x)    (((x) & 0x00000F00) >> 8)

=======
#define SMSM_INIT		0x00000001
#define SMSM_SMDINIT		0x00000008
#define SMSM_RPCINIT		0x00000020
#define SMSM_RESET		0x00000040
#define SMSM_RSA		0x00000080
#define SMSM_RUN		0x00000100
#define SMSM_PWRC		0x00000200
#define SMSM_TIMEWAIT		0x00000400
#define SMSM_TIMEINIT		0x00000800
#define SMSM_PWRC_EARLY_EXIT	0x00001000
#define SMSM_WFPI		0x00002000
#define SMSM_SLEEP		0x00004000
#define SMSM_SLEEPEXIT		0x00008000
#define SMSM_APPS_REBOOT	0x00020000
#define SMSM_SYSTEM_POWER_DOWN	0x00040000
#define SMSM_SYSTEM_REBOOT	0x00080000
#define SMSM_SYSTEM_DOWNLOAD	0x00100000
#define SMSM_PWRC_SUSPEND	0x00200000
#define SMSM_APPS_SHUTDOWN	0x00400000
#define SMSM_SMD_LOOPBACK	0x00800000
#define SMSM_RUN_QUIET		0x01000000
#define SMSM_MODEM_WAIT		0x02000000
#define SMSM_MODEM_BREAK	0x04000000
#define SMSM_MODEM_CONTINUE	0x08000000
#define SMSM_UNKNOWN		0x80000000

#define SMSM_WKUP_REASON_RPC	0x00000001
#define SMSM_WKUP_REASON_INT	0x00000002
#define SMSM_WKUP_REASON_GPIO	0x00000004
#define SMSM_WKUP_REASON_TIMER	0x00000008
#define SMSM_WKUP_REASON_ALARM	0x00000010
#define SMSM_WKUP_REASON_RESET	0x00000020

#ifdef CONFIG_ARCH_MSM7X00A
enum smsm_state_item {
	SMSM_STATE_APPS = 1,
	SMSM_STATE_MODEM = 3,
	SMSM_STATE_COUNT,
};
#else
enum smsm_state_item {
	SMSM_STATE_APPS,
	SMSM_STATE_MODEM,
	SMSM_STATE_HEXAGON,
	SMSM_STATE_APPS_DEM,
	SMSM_STATE_MODEM_DEM,
	SMSM_STATE_QDSP6_DEM,
	SMSM_STATE_POWER_MASTER_DEM,
	SMSM_STATE_TIME_MASTER_DEM,
	SMSM_STATE_COUNT,
};
#endif

void *smem_alloc(unsigned id, unsigned size);
int smsm_change_state(enum smsm_state_item item, uint32_t clear_mask, uint32_t set_mask);
uint32_t smsm_get_state(enum smsm_state_item item);
int smsm_set_sleep_duration(uint32_t delay);
void smsm_print_sleep_info(void);

#define SMEM_NUM_SMD_CHANNELS        64

typedef enum {
	/* fixed items */
	SMEM_PROC_COMM = 0,
	SMEM_HEAP_INFO,
	SMEM_ALLOCATION_TABLE,
	SMEM_VERSION_INFO,
	SMEM_HW_RESET_DETECT,
	SMEM_AARM_WARM_BOOT,
	SMEM_DIAG_ERR_MESSAGE,
	SMEM_SPINLOCK_ARRAY,
	SMEM_MEMORY_BARRIER_LOCATION,

	/* dynamic items */
	SMEM_AARM_PARTITION_TABLE,
	SMEM_AARM_BAD_BLOCK_TABLE,
	SMEM_RESERVE_BAD_BLOCKS,
	SMEM_WM_UUID,
	SMEM_CHANNEL_ALLOC_TBL,
	SMEM_SMD_BASE_ID,
	SMEM_SMEM_LOG_IDX = SMEM_SMD_BASE_ID + SMEM_NUM_SMD_CHANNELS,
	SMEM_SMEM_LOG_EVENTS,
	SMEM_SMEM_STATIC_LOG_IDX,
	SMEM_SMEM_STATIC_LOG_EVENTS,
	SMEM_SMEM_SLOW_CLOCK_SYNC,
	SMEM_SMEM_SLOW_CLOCK_VALUE,
	SMEM_BIO_LED_BUF,
	SMEM_SMSM_SHARED_STATE,
	SMEM_SMSM_INT_INFO,
	SMEM_SMSM_SLEEP_DELAY,
	SMEM_SMSM_LIMIT_SLEEP,
	SMEM_SLEEP_POWER_COLLAPSE_DISABLED,
	SMEM_KEYPAD_KEYS_PRESSED,
	SMEM_KEYPAD_STATE_UPDATED,
	SMEM_KEYPAD_STATE_IDX,
	SMEM_GPIO_INT,
	SMEM_MDDI_LCD_IDX,
	SMEM_MDDI_HOST_DRIVER_STATE,
	SMEM_MDDI_LCD_DISP_STATE,
	SMEM_LCD_CUR_PANEL,
	SMEM_MARM_BOOT_SEGMENT_INFO,
	SMEM_AARM_BOOT_SEGMENT_INFO,
	SMEM_SLEEP_STATIC,
	SMEM_SCORPION_FREQUENCY,
	SMEM_SMD_PROFILES,
	SMEM_TSSC_BUSY,
	SMEM_HS_SUSPEND_FILTER_INFO,
	SMEM_BATT_INFO,
	SMEM_APPS_BOOT_MODE,
	SMEM_VERSION_FIRST,
	SMEM_VERSION_LAST = SMEM_VERSION_FIRST + 24,
	SMEM_OSS_RRCASN1_BUF1,
	SMEM_OSS_RRCASN1_BUF2,
	SMEM_ID_VENDOR0,
	SMEM_ID_VENDOR1,
	SMEM_ID_VENDOR2,
	SMEM_HW_SW_BUILD_ID,
	SMEM_SMD_BLOCK_PORT_BASE_ID,
	SMEM_SMD_BLOCK_PORT_PROC0_HEAP = SMEM_SMD_BLOCK_PORT_BASE_ID + SMEM_NUM_SMD_CHANNELS,
	SMEM_SMD_BLOCK_PORT_PROC1_HEAP = SMEM_SMD_BLOCK_PORT_PROC0_HEAP + SMEM_NUM_SMD_CHANNELS,
	SMEM_I2C_MUTEX = SMEM_SMD_BLOCK_PORT_PROC1_HEAP + SMEM_NUM_SMD_CHANNELS,
	SMEM_SCLK_CONVERSION,
	SMEM_SMD_SMSM_INTR_MUX,
	SMEM_SMSM_CPU_INTR_MASK,
	SMEM_APPS_DEM_SLAVE_DATA,
	SMEM_QDSP6_DEM_SLAVE_DATA,
	SMEM_CLKREGIM_BSP,
	SMEM_CLKREGIM_SOURCES,
	SMEM_SMD_FIFO_BASE_ID,
	SMEM_USABLE_RAM_PARTITION_TABLE = SMEM_SMD_FIFO_BASE_ID + SMEM_NUM_SMD_CHANNELS,
	SMEM_POWER_ON_STATUS_INFO,
	SMEM_DAL_AREA,
	SMEM_SMEM_LOG_POWER_IDX,
	SMEM_SMEM_LOG_POWER_WRAP,
	SMEM_SMEM_LOG_POWER_EVENTS,
	SMEM_ERR_CRASH_LOG,
	SMEM_ERR_F3_TRACE_LOG,
	SMEM_NUM_ITEMS,
} smem_mem_type;


#define SMD_SS_CLOSED		0x00000000
#define SMD_SS_OPENING		0x00000001
#define SMD_SS_OPENED		0x00000002
#define SMD_SS_FLUSHING		0x00000003
#define SMD_SS_CLOSING		0x00000004
#define SMD_SS_RESET		0x00000005
#define SMD_SS_RESET_OPENING	0x00000006

#define SMD_BUF_SIZE		8192
#define SMD_CHANNELS		64

#define SMD_HEADER_SIZE		20

=======
>>>>>>> refs/remotes/origin/cm-11.0
struct smd_alloc_elm {
	char name[20];
	uint32_t cid;
	uint32_t type;
	uint32_t ref_count;
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
#define SMD_CHANNEL_TYPE(x) ((x) & 0x000000FF)
#define SMD_XFER_TYPE(x)    (((x) & 0x00000F00) >> 8)

>>>>>>> refs/remotes/origin/cm-11.0
struct smd_half_channel {
	unsigned state;
	unsigned char fDSR;
	unsigned char fCTS;
	unsigned char fCD;
	unsigned char fRI;
	unsigned char fHEAD;
	unsigned char fTAIL;
	unsigned char fSTATE;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned char fBLOCKREADINTR;
	unsigned tail;
	unsigned head;
};

<<<<<<< HEAD
=======
struct smd_half_channel_word_access {
	unsigned state;
	unsigned fDSR;
	unsigned fCTS;
	unsigned fCD;
	unsigned fRI;
	unsigned fHEAD;
	unsigned fTAIL;
	unsigned fSTATE;
	unsigned fBLOCKREADINTR;
	unsigned tail;
	unsigned head;
};

struct smd_half_channel_access {
	void (*set_state)(volatile void *half_channel, unsigned data);
	unsigned (*get_state)(volatile void *half_channel);
	void (*set_fDSR)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fDSR)(volatile void *half_channel);
	void (*set_fCTS)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fCTS)(volatile void *half_channel);
	void (*set_fCD)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fCD)(volatile void *half_channel);
	void (*set_fRI)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fRI)(volatile void *half_channel);
	void (*set_fHEAD)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fHEAD)(volatile void *half_channel);
	void (*set_fTAIL)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fTAIL)(volatile void *half_channel);
	void (*set_fSTATE)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fSTATE)(volatile void *half_channel);
	void (*set_fBLOCKREADINTR)(volatile void *half_channel,
					unsigned char data);
	unsigned (*get_fBLOCKREADINTR)(volatile void *half_channel);
	void (*set_tail)(volatile void *half_channel, unsigned data);
	unsigned (*get_tail)(volatile void *half_channel);
	void (*set_head)(volatile void *half_channel, unsigned data);
	unsigned (*get_head)(volatile void *half_channel);
};

int is_word_access_ch(unsigned ch_type);

struct smd_half_channel_access *get_half_ch_funcs(unsigned ch_type);

>>>>>>> refs/remotes/origin/cm-10.0
struct smem_ram_ptn {
	char name[16];
	unsigned start;
	unsigned size;

	/* RAM Partition attribute: READ_ONLY, READWRITE etc.  */
	unsigned attr;

	/* RAM Partition category: EBI0, EBI1, IRAM, IMEM */
	unsigned category;

	/* RAM Partition domain: APPS, MODEM, APPS & MODEM (SHARED) etc. */
	unsigned domain;

	/* RAM Partition type: system, bootloader, appsboot, apps etc. */
	unsigned type;

	/* reserved for future expansion without changing version number */
	unsigned reserved2, reserved3, reserved4, reserved5;
} __attribute__ ((__packed__));


struct smem_ram_ptable {
	#define _SMEM_RAM_PTABLE_MAGIC_1 0x9DA5E0A8
	#define _SMEM_RAM_PTABLE_MAGIC_2 0xAF9EC4E2
	unsigned magic[2];
	unsigned version;
	unsigned reserved1;
	unsigned len;
	struct smem_ram_ptn parts[32];
	unsigned buf;
} __attribute__ ((__packed__));

/* SMEM RAM Partition */
enum {
	DEFAULT_ATTRB = ~0x0,
	READ_ONLY = 0x0,
	READWRITE,
};

enum {
	DEFAULT_CATEGORY = ~0x0,
	SMI = 0x0,
	EBI1,
	EBI2,
	QDSP6,
	IRAM,
	IMEM,
	EBI0_CS0,
	EBI0_CS1,
	EBI1_CS0,
	EBI1_CS1,
	SDRAM = 0xE,
};

enum {
	DEFAULT_DOMAIN = 0x0,
	APPS_DOMAIN,
	MODEM_DOMAIN,
	SHARED_DOMAIN,
};

enum {
	SYS_MEMORY = 1,        /* system memory*/
	BOOT_REGION_MEMORY1,   /* boot loader memory 1*/
	BOOT_REGION_MEMORY2,   /* boot loader memory 2,reserved*/
	APPSBL_MEMORY,         /* apps boot loader memory*/
	APPS_MEMORY,           /* apps  usage memory*/
};

extern spinlock_t smem_lock;


void smd_diag(void);

<<<<<<< HEAD
=======
struct interrupt_stat {
	uint32_t smd_in_count;
	uint32_t smd_out_hardcode_count;
	uint32_t smd_out_config_count;

	uint32_t smsm_in_count;
	uint32_t smsm_out_hardcode_count;
	uint32_t smsm_out_config_count;
};
extern struct interrupt_stat interrupt_stats[NUM_SMD_SUBSYSTEMS];

>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned char fUNUSED;
=======
	unsigned char fBLOCKREADINTR;
>>>>>>> refs/remotes/origin/cm-11.0
	unsigned tail;
	unsigned head;
};

struct smd_half_channel_word_access {
	unsigned state;
	unsigned fDSR;
	unsigned fCTS;
	unsigned fCD;
	unsigned fRI;
	unsigned fHEAD;
	unsigned fTAIL;
	unsigned fSTATE;
	unsigned fBLOCKREADINTR;
	unsigned tail;
	unsigned head;
};

struct smd_half_channel_access {
	void (*set_state)(volatile void *half_channel, unsigned data);
	unsigned (*get_state)(volatile void *half_channel);
	void (*set_fDSR)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fDSR)(volatile void *half_channel);
	void (*set_fCTS)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fCTS)(volatile void *half_channel);
	void (*set_fCD)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fCD)(volatile void *half_channel);
	void (*set_fRI)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fRI)(volatile void *half_channel);
	void (*set_fHEAD)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fHEAD)(volatile void *half_channel);
	void (*set_fTAIL)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fTAIL)(volatile void *half_channel);
	void (*set_fSTATE)(volatile void *half_channel, unsigned char data);
	unsigned (*get_fSTATE)(volatile void *half_channel);
	void (*set_fBLOCKREADINTR)(volatile void *half_channel,
					unsigned char data);
	unsigned (*get_fBLOCKREADINTR)(volatile void *half_channel);
	void (*set_tail)(volatile void *half_channel, unsigned data);
	unsigned (*get_tail)(volatile void *half_channel);
	void (*set_head)(volatile void *half_channel, unsigned data);
	unsigned (*get_head)(volatile void *half_channel);
};

int is_word_access_ch(unsigned ch_type);

struct smd_half_channel_access *get_half_ch_funcs(unsigned ch_type);

struct smem_ram_ptn {
	char name[16];
	unsigned start;
	unsigned size;

	/* RAM Partition attribute: READ_ONLY, READWRITE etc.  */
	unsigned attr;

	/* RAM Partition category: EBI0, EBI1, IRAM, IMEM */
	unsigned category;

	/* RAM Partition domain: APPS, MODEM, APPS & MODEM (SHARED) etc. */
	unsigned domain;

	/* RAM Partition type: system, bootloader, appsboot, apps etc. */
	unsigned type;

	/* reserved for future expansion without changing version number */
	unsigned reserved2, reserved3, reserved4, reserved5;
} __attribute__ ((__packed__));


struct smem_ram_ptable {
	#define _SMEM_RAM_PTABLE_MAGIC_1 0x9DA5E0A8
	#define _SMEM_RAM_PTABLE_MAGIC_2 0xAF9EC4E2
	unsigned magic[2];
	unsigned version;
	unsigned reserved1;
	unsigned len;
	struct smem_ram_ptn parts[32];
	unsigned buf;
} __attribute__ ((__packed__));

/* SMEM RAM Partition */
enum {
	DEFAULT_ATTRB = ~0x0,
	READ_ONLY = 0x0,
	READWRITE,
};

enum {
	DEFAULT_CATEGORY = ~0x0,
	SMI = 0x0,
	EBI1,
	EBI2,
	QDSP6,
	IRAM,
	IMEM,
	EBI0_CS0,
	EBI0_CS1,
	EBI1_CS0,
	EBI1_CS1,
	SDRAM = 0xE,
};

enum {
	DEFAULT_DOMAIN = 0x0,
	APPS_DOMAIN,
	MODEM_DOMAIN,
	SHARED_DOMAIN,
};

enum {
	SYS_MEMORY = 1,        /* system memory*/
	BOOT_REGION_MEMORY1,   /* boot loader memory 1*/
	BOOT_REGION_MEMORY2,   /* boot loader memory 2,reserved*/
	APPSBL_MEMORY,         /* apps boot loader memory*/
	APPS_MEMORY,           /* apps  usage memory*/
};

extern spinlock_t smem_lock;


void smd_diag(void);

struct interrupt_stat {
	uint32_t smd_in_count;
	uint32_t smd_out_hardcode_count;
	uint32_t smd_out_config_count;

	uint32_t smsm_in_count;
	uint32_t smsm_out_hardcode_count;
	uint32_t smsm_out_config_count;
};
extern struct interrupt_stat interrupt_stats[NUM_SMD_SUBSYSTEMS];

>>>>>>> refs/remotes/origin/master
#endif
