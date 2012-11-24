#ifndef SMEM_VENDOR_TYPE_H
#define SMEM_VENDOR_TYPE_H
/*
* SMEM_ID_VENDOR1 type definition
*/

#define SMEM_VENDOR1_CAPTURE_BUF_SIZE (128*1024)

typedef struct MODEM_RAMDUMP_TYPE {
	unsigned char modem;
	unsigned int modem_line;
	unsigned char modem_file[40];
	unsigned char modem_format[40];
	unsigned char modem_qxdm_message[80];
} modem_ramdump;

typedef struct APPS_RAMDUMP_TYPE {
	unsigned char apps;
	size_t apps_regs[18];
	size_t apps_pid;
	size_t mmu_table[4];
	unsigned char apps_process[100];
	unsigned char apps_string[100];
} apps_ramdump;

struct dpram_intr_dbg {
	unsigned int sent;
	unsigned int rcvd;
};

typedef struct {
	struct dpram_intr_dbg m2a;
	struct dpram_intr_dbg a2m;
} dpram_dbg_type;

typedef struct SMEM_VENDOR1_ID_TYPE {
	/* This must be exactly the same with smem_vendor_types.h
	* in CP boot, CP, AP boot and AP !
	*/
	unsigned int reboot_reason;
	unsigned int silent_reset;
	unsigned char hw_version;
	unsigned char download_mode;
	modem_ramdump modem_dump;
	apps_ramdump  apps_dump;
	unsigned char capture_buffer[SMEM_VENDOR1_CAPTURE_BUF_SIZE];
	unsigned int efs_start_block;
	unsigned int efs_length;
	dpram_dbg_type dpram_debug;
	unsigned int fota_mode;
	unsigned int uart_console;
	unsigned int ram_dump_level;
	unsigned int fota_update_result;

	unsigned int AP_reserved[10];
	unsigned int CP_reserved[10];
} __attribute__((aligned(8))) __attribute__((packed)) samsung_vendor1_id;
#endif /* SMEM_VENDOR_TYPE_H */
