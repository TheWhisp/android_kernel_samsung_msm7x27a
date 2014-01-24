/*
 * QLogic Fibre Channel HBA Driver
<<<<<<< HEAD
 * Copyright (c)  2003-2011 QLogic Corporation
=======
 * Copyright (c)  2003-2013 QLogic Corporation
>>>>>>> refs/remotes/origin/master
 *
 * See LICENSE.qla2xxx for copyright and licensing details.
 */

#include "qla_def.h"

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Driver debug definitions.
 */
/* #define QL_DEBUG_LEVEL_1  */ /* Output register accesses to COM1 */
/* #define QL_DEBUG_LEVEL_2  */ /* Output error msgs to COM1 */
/* #define QL_DEBUG_LEVEL_3  */ /* Output function trace msgs to COM1 */
/* #define QL_DEBUG_LEVEL_4  */ /* Output NVRAM trace msgs to COM1 */
/* #define QL_DEBUG_LEVEL_5  */ /* Output ring trace msgs to COM1 */
/* #define QL_DEBUG_LEVEL_6  */ /* Output WATCHDOG timer trace to COM1 */
/* #define QL_DEBUG_LEVEL_7  */ /* Output RISC load trace msgs to COM1 */
/* #define QL_DEBUG_LEVEL_8  */ /* Output ring saturation msgs to COM1 */
/* #define QL_DEBUG_LEVEL_9  */ /* Output IOCTL trace msgs */
/* #define QL_DEBUG_LEVEL_10 */ /* Output IOCTL error msgs */
/* #define QL_DEBUG_LEVEL_11 */ /* Output Mbx Cmd trace msgs */
/* #define QL_DEBUG_LEVEL_12 */ /* Output IP trace msgs */
/* #define QL_DEBUG_LEVEL_13 */ /* Output fdmi function trace msgs */
/* #define QL_DEBUG_LEVEL_14 */ /* Output RSCN trace msgs */
/* #define QL_DEBUG_LEVEL_15 */ /* Output NPIV trace msgs */
/* #define QL_DEBUG_LEVEL_16 */ /* Output ISP84XX trace msgs */
/* #define QL_DEBUG_LEVEL_17 */ /* Output EEH trace messages */
/* #define QL_DEBUG_LEVEL_18 */ /* Output T10 CRC trace messages */

/*
* Macros use for debugging the driver.
*/

#define DEBUG(x)	do { if (ql2xextended_error_logging) { x; } } while (0)

#if defined(QL_DEBUG_LEVEL_1)
#define DEBUG1(x)	do {x;} while (0)
#else
#define DEBUG1(x)	do {} while (0)
#endif

#define DEBUG2(x)	do { if (ql2xextended_error_logging) { x; } } while (0)
#define DEBUG2_3(x)	do { if (ql2xextended_error_logging) { x; } } while (0)
#define DEBUG2_3_11(x)	do { if (ql2xextended_error_logging) { x; } } while (0)
#define DEBUG2_9_10(x)	do { if (ql2xextended_error_logging) { x; } } while (0)
#define DEBUG2_11(x)	do { if (ql2xextended_error_logging) { x; } } while (0)
#define DEBUG2_13(x)	do { if (ql2xextended_error_logging) { x; } } while (0)
#define DEBUG2_16(x)	do { if (ql2xextended_error_logging) { x; } } while (0)
#define DEBUG2_17(x) 	do { if (ql2xextended_error_logging) { x; } } while (0)

#if defined(QL_DEBUG_LEVEL_3)
#define DEBUG3(x)	do {x;} while (0)
#define DEBUG3_11(x)	do {x;} while (0)
#else
#define DEBUG3(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_4)
#define DEBUG4(x)	do {x;} while (0)
#else
#define DEBUG4(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_5)
#define DEBUG5(x)          do {x;} while (0)
#else
#define DEBUG5(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_7)
#define DEBUG7(x)          do {x;} while (0)
#else
#define DEBUG7(x)	   do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_9)
#define DEBUG9(x)       do {x;} while (0)
#define DEBUG9_10(x)    do {x;} while (0)
#else
#define DEBUG9(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_10)
#define DEBUG10(x)      do {x;} while (0)
#define DEBUG9_10(x)	do {x;} while (0)
#else
#define DEBUG10(x)	do {} while (0)
  #if !defined(DEBUG9_10)
  #define DEBUG9_10(x)	do {} while (0)
  #endif
#endif

#if defined(QL_DEBUG_LEVEL_11)
#define DEBUG11(x)      do{x;} while(0)
#if !defined(DEBUG3_11)
#define DEBUG3_11(x)    do{x;} while(0)
#endif
#else
#define DEBUG11(x)	do{} while(0)
  #if !defined(QL_DEBUG_LEVEL_3)
  #define DEBUG3_11(x)	do{} while(0)
  #endif
#endif

#if defined(QL_DEBUG_LEVEL_12)
#define DEBUG12(x)      do {x;} while (0)
#else
#define DEBUG12(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_13)
#define DEBUG13(x)      do {x;} while (0)
#else
#define DEBUG13(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_14)
#define DEBUG14(x)      do {x;} while (0)
#else
#define DEBUG14(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_15)
#define DEBUG15(x)      do {x;} while (0)
#else
#define DEBUG15(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_16)
#define DEBUG16(x)	do {x;} while (0)
#else
#define DEBUG16(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_17)
#define DEBUG17(x)	do {x;} while (0)
#else
#define DEBUG17(x)	do {} while (0)
#endif

#if defined(QL_DEBUG_LEVEL_18)
#define DEBUG18(x)	do {if (ql2xextended_error_logging) x; } while (0)
#else
#define DEBUG18(x)	do {} while (0)
#endif


/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * Firmware Dump structure definition
 */

struct qla2300_fw_dump {
	uint16_t hccr;
	uint16_t pbiu_reg[8];
	uint16_t risc_host_reg[8];
	uint16_t mailbox_reg[32];
	uint16_t resp_dma_reg[32];
	uint16_t dma_reg[48];
	uint16_t risc_hdw_reg[16];
	uint16_t risc_gp0_reg[16];
	uint16_t risc_gp1_reg[16];
	uint16_t risc_gp2_reg[16];
	uint16_t risc_gp3_reg[16];
	uint16_t risc_gp4_reg[16];
	uint16_t risc_gp5_reg[16];
	uint16_t risc_gp6_reg[16];
	uint16_t risc_gp7_reg[16];
	uint16_t frame_buf_hdw_reg[64];
	uint16_t fpm_b0_reg[64];
	uint16_t fpm_b1_reg[64];
	uint16_t risc_ram[0xf800];
	uint16_t stack_ram[0x1000];
	uint16_t data_ram[1];
};

struct qla2100_fw_dump {
	uint16_t hccr;
	uint16_t pbiu_reg[8];
	uint16_t mailbox_reg[32];
	uint16_t dma_reg[48];
	uint16_t risc_hdw_reg[16];
	uint16_t risc_gp0_reg[16];
	uint16_t risc_gp1_reg[16];
	uint16_t risc_gp2_reg[16];
	uint16_t risc_gp3_reg[16];
	uint16_t risc_gp4_reg[16];
	uint16_t risc_gp5_reg[16];
	uint16_t risc_gp6_reg[16];
	uint16_t risc_gp7_reg[16];
	uint16_t frame_buf_hdw_reg[16];
	uint16_t fpm_b0_reg[64];
	uint16_t fpm_b1_reg[64];
	uint16_t risc_ram[0xf000];
};

struct qla24xx_fw_dump {
	uint32_t host_status;
	uint32_t host_reg[32];
	uint32_t shadow_reg[7];
	uint16_t mailbox_reg[32];
	uint32_t xseq_gp_reg[128];
	uint32_t xseq_0_reg[16];
	uint32_t xseq_1_reg[16];
	uint32_t rseq_gp_reg[128];
	uint32_t rseq_0_reg[16];
	uint32_t rseq_1_reg[16];
	uint32_t rseq_2_reg[16];
	uint32_t cmd_dma_reg[16];
	uint32_t req0_dma_reg[15];
	uint32_t resp0_dma_reg[15];
	uint32_t req1_dma_reg[15];
	uint32_t xmt0_dma_reg[32];
	uint32_t xmt1_dma_reg[32];
	uint32_t xmt2_dma_reg[32];
	uint32_t xmt3_dma_reg[32];
	uint32_t xmt4_dma_reg[32];
	uint32_t xmt_data_dma_reg[16];
	uint32_t rcvt0_data_dma_reg[32];
	uint32_t rcvt1_data_dma_reg[32];
	uint32_t risc_gp_reg[128];
	uint32_t lmc_reg[112];
	uint32_t fpm_hdw_reg[192];
	uint32_t fb_hdw_reg[176];
	uint32_t code_ram[0x2000];
	uint32_t ext_mem[1];
};

struct qla25xx_fw_dump {
	uint32_t host_status;
	uint32_t host_risc_reg[32];
	uint32_t pcie_regs[4];
	uint32_t host_reg[32];
	uint32_t shadow_reg[11];
	uint32_t risc_io_reg;
	uint16_t mailbox_reg[32];
	uint32_t xseq_gp_reg[128];
	uint32_t xseq_0_reg[48];
	uint32_t xseq_1_reg[16];
	uint32_t rseq_gp_reg[128];
	uint32_t rseq_0_reg[32];
	uint32_t rseq_1_reg[16];
	uint32_t rseq_2_reg[16];
	uint32_t aseq_gp_reg[128];
	uint32_t aseq_0_reg[32];
	uint32_t aseq_1_reg[16];
	uint32_t aseq_2_reg[16];
	uint32_t cmd_dma_reg[16];
	uint32_t req0_dma_reg[15];
	uint32_t resp0_dma_reg[15];
	uint32_t req1_dma_reg[15];
	uint32_t xmt0_dma_reg[32];
	uint32_t xmt1_dma_reg[32];
	uint32_t xmt2_dma_reg[32];
	uint32_t xmt3_dma_reg[32];
	uint32_t xmt4_dma_reg[32];
	uint32_t xmt_data_dma_reg[16];
	uint32_t rcvt0_data_dma_reg[32];
	uint32_t rcvt1_data_dma_reg[32];
	uint32_t risc_gp_reg[128];
	uint32_t lmc_reg[128];
	uint32_t fpm_hdw_reg[192];
	uint32_t fb_hdw_reg[192];
	uint32_t code_ram[0x2000];
	uint32_t ext_mem[1];
};

struct qla81xx_fw_dump {
	uint32_t host_status;
	uint32_t host_risc_reg[32];
	uint32_t pcie_regs[4];
	uint32_t host_reg[32];
	uint32_t shadow_reg[11];
	uint32_t risc_io_reg;
	uint16_t mailbox_reg[32];
	uint32_t xseq_gp_reg[128];
	uint32_t xseq_0_reg[48];
	uint32_t xseq_1_reg[16];
	uint32_t rseq_gp_reg[128];
	uint32_t rseq_0_reg[32];
	uint32_t rseq_1_reg[16];
	uint32_t rseq_2_reg[16];
	uint32_t aseq_gp_reg[128];
	uint32_t aseq_0_reg[32];
	uint32_t aseq_1_reg[16];
	uint32_t aseq_2_reg[16];
	uint32_t cmd_dma_reg[16];
	uint32_t req0_dma_reg[15];
	uint32_t resp0_dma_reg[15];
	uint32_t req1_dma_reg[15];
	uint32_t xmt0_dma_reg[32];
	uint32_t xmt1_dma_reg[32];
	uint32_t xmt2_dma_reg[32];
	uint32_t xmt3_dma_reg[32];
	uint32_t xmt4_dma_reg[32];
	uint32_t xmt_data_dma_reg[16];
	uint32_t rcvt0_data_dma_reg[32];
	uint32_t rcvt1_data_dma_reg[32];
	uint32_t risc_gp_reg[128];
	uint32_t lmc_reg[128];
	uint32_t fpm_hdw_reg[224];
	uint32_t fb_hdw_reg[208];
	uint32_t code_ram[0x2000];
	uint32_t ext_mem[1];
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
struct qla83xx_fw_dump {
	uint32_t host_status;
	uint32_t host_risc_reg[48];
	uint32_t pcie_regs[4];
	uint32_t host_reg[32];
	uint32_t shadow_reg[11];
	uint32_t risc_io_reg;
	uint16_t mailbox_reg[32];
	uint32_t xseq_gp_reg[256];
	uint32_t xseq_0_reg[48];
	uint32_t xseq_1_reg[16];
	uint32_t xseq_2_reg[16];
	uint32_t rseq_gp_reg[256];
	uint32_t rseq_0_reg[32];
	uint32_t rseq_1_reg[16];
	uint32_t rseq_2_reg[16];
	uint32_t rseq_3_reg[16];
	uint32_t aseq_gp_reg[256];
	uint32_t aseq_0_reg[32];
	uint32_t aseq_1_reg[16];
	uint32_t aseq_2_reg[16];
	uint32_t aseq_3_reg[16];
	uint32_t cmd_dma_reg[64];
	uint32_t req0_dma_reg[15];
	uint32_t resp0_dma_reg[15];
	uint32_t req1_dma_reg[15];
	uint32_t xmt0_dma_reg[32];
	uint32_t xmt1_dma_reg[32];
	uint32_t xmt2_dma_reg[32];
	uint32_t xmt3_dma_reg[32];
	uint32_t xmt4_dma_reg[32];
	uint32_t xmt_data_dma_reg[16];
	uint32_t rcvt0_data_dma_reg[32];
	uint32_t rcvt1_data_dma_reg[32];
	uint32_t risc_gp_reg[128];
	uint32_t lmc_reg[128];
	uint32_t fpm_hdw_reg[256];
	uint32_t rq0_array_reg[256];
	uint32_t rq1_array_reg[256];
	uint32_t rp0_array_reg[256];
	uint32_t rp1_array_reg[256];
	uint32_t queue_control_reg[16];
	uint32_t fb_hdw_reg[432];
	uint32_t at0_array_reg[128];
	uint32_t code_ram[0x2400];
	uint32_t ext_mem[1];
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define EFT_NUM_BUFFERS		4
#define EFT_BYTES_PER_BUFFER	0x4000
#define EFT_SIZE		((EFT_BYTES_PER_BUFFER) * (EFT_NUM_BUFFERS))

#define FCE_NUM_BUFFERS		64
#define FCE_BYTES_PER_BUFFER	0x400
#define FCE_SIZE		((FCE_BYTES_PER_BUFFER) * (FCE_NUM_BUFFERS))
#define fce_calc_size(b)	((FCE_BYTES_PER_BUFFER) * (b))

struct qla2xxx_fce_chain {
	uint32_t type;
	uint32_t chain_size;

	uint32_t size;
	uint32_t addr_l;
	uint32_t addr_h;
	uint32_t eregs[8];
};

struct qla2xxx_mq_chain {
	uint32_t type;
	uint32_t chain_size;

	uint32_t count;
	uint32_t qregs[4 * QLA_MQ_SIZE];
};

<<<<<<< HEAD
<<<<<<< HEAD
#define DUMP_CHAIN_VARIANT	0x80000000
#define DUMP_CHAIN_FCE		0x7FFFFAF0
#define DUMP_CHAIN_MQ		0x7FFFFAF1
=======
=======
>>>>>>> refs/remotes/origin/master
struct qla2xxx_mqueue_header {
	uint32_t queue;
#define TYPE_REQUEST_QUEUE	0x1
#define TYPE_RESPONSE_QUEUE	0x2
<<<<<<< HEAD
=======
#define TYPE_ATIO_QUEUE		0x3
>>>>>>> refs/remotes/origin/master
	uint32_t number;
	uint32_t size;
};

struct qla2xxx_mqueue_chain {
	uint32_t type;
	uint32_t chain_size;
};

#define DUMP_CHAIN_VARIANT	0x80000000
#define DUMP_CHAIN_FCE		0x7FFFFAF0
#define DUMP_CHAIN_MQ		0x7FFFFAF1
#define DUMP_CHAIN_QUEUE	0x7FFFFAF2
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define DUMP_CHAIN_LAST		0x80000000

struct qla2xxx_fw_dump {
	uint8_t signature[4];
	uint32_t version;

	uint32_t fw_major_version;
	uint32_t fw_minor_version;
	uint32_t fw_subminor_version;
	uint32_t fw_attributes;

	uint32_t vendor;
	uint32_t device;
	uint32_t subsystem_vendor;
	uint32_t subsystem_device;

	uint32_t fixed_size;
	uint32_t mem_size;
	uint32_t req_q_size;
	uint32_t rsp_q_size;

	uint32_t eft_size;
	uint32_t eft_addr_l;
	uint32_t eft_addr_h;

	uint32_t header_size;

	union {
		struct qla2100_fw_dump isp21;
		struct qla2300_fw_dump isp23;
		struct qla24xx_fw_dump isp24;
		struct qla25xx_fw_dump isp25;
		struct qla81xx_fw_dump isp81;
<<<<<<< HEAD
<<<<<<< HEAD
	} isp;
};
=======
=======
>>>>>>> refs/remotes/origin/master
		struct qla83xx_fw_dump isp83;
	} isp;
};

#define QL_MSGHDR "qla2xxx"
#define QL_DBG_DEFAULT1_MASK    0x1e400000

#define ql_log_fatal		0 /* display fatal errors */
#define ql_log_warn		1 /* display critical errors */
#define ql_log_info		2 /* display all recovered errors */
#define ql_log_all		3 /* This value is only used by ql_errlev.
				   * No messages will use this value.
				   * This should be always highest value
				   * as compared to other log levels.
				   */

extern int ql_errlev;

void __attribute__((format (printf, 4, 5)))
ql_dbg(uint32_t, scsi_qla_host_t *vha, int32_t, const char *fmt, ...);
void __attribute__((format (printf, 4, 5)))
ql_dbg_pci(uint32_t, struct pci_dev *pdev, int32_t, const char *fmt, ...);

void __attribute__((format (printf, 4, 5)))
ql_log(uint32_t, scsi_qla_host_t *vha, int32_t, const char *fmt, ...);
void __attribute__((format (printf, 4, 5)))
ql_log_pci(uint32_t, struct pci_dev *pdev, int32_t, const char *fmt, ...);

/* Debug Levels */
/* The 0x40000000 is the max value any debug level can have
 * as ql2xextended_error_logging is of type signed int
 */
#define ql_dbg_init	0x40000000 /* Init Debug */
#define ql_dbg_mbx	0x20000000 /* MBX Debug */
#define ql_dbg_disc	0x10000000 /* Device Discovery Debug */
#define ql_dbg_io	0x08000000 /* IO Tracing Debug */
#define ql_dbg_dpc	0x04000000 /* DPC Thead Debug */
#define ql_dbg_async	0x02000000 /* Async events Debug */
#define ql_dbg_timer	0x01000000 /* Timer Debug */
#define ql_dbg_user	0x00800000 /* User Space Interations Debug */
#define ql_dbg_taskm	0x00400000 /* Task Management Debug */
#define ql_dbg_aer	0x00200000 /* AER/EEH Debug */
#define ql_dbg_multiq	0x00100000 /* MultiQ Debug */
#define ql_dbg_p3p	0x00080000 /* P3P specific Debug */
#define ql_dbg_vport	0x00040000 /* Virtual Port Debug */
#define ql_dbg_buffer	0x00020000 /* For dumping the buffer/regs */
#define ql_dbg_misc	0x00010000 /* For dumping everything that is not
				    * not covered by upper categories
				    */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define ql_dbg_verbose	0x00008000 /* More verbosity for each level
				    * This is to be used with other levels where
				    * more verbosity is required. It might not
				    * be applicable to all the levels.
				    */
#define ql_dbg_tgt	0x00004000 /* Target mode */
#define ql_dbg_tgt_mgt	0x00002000 /* Target mode management */
#define ql_dbg_tgt_tmr	0x00001000 /* Target mode task management */
>>>>>>> refs/remotes/origin/master
