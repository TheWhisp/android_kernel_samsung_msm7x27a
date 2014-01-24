#ifndef TARGET_CORE_RD_H
#define TARGET_CORE_RD_H

#define RD_HBA_VERSION		"v4.0"
<<<<<<< HEAD
#define RD_DR_VERSION		"4.0"
=======
>>>>>>> refs/remotes/origin/master
#define RD_MCP_VERSION		"4.0"

/* Largest piece of memory kmalloc can allocate */
#define RD_MAX_ALLOCATION_SIZE	65536
<<<<<<< HEAD
<<<<<<< HEAD
/* Maximum queuedepth for the Ramdisk HBA */
#define RD_HBA_QUEUE_DEPTH	256
=======
>>>>>>> refs/remotes/origin/cm-10.0
#define RD_DEVICE_QUEUE_DEPTH	32
#define RD_MAX_DEVICE_QUEUE_DEPTH 128
#define RD_BLOCKSIZE		512
#define RD_MAX_SECTORS		1024
=======
#define RD_DEVICE_QUEUE_DEPTH	32
#define RD_MAX_DEVICE_QUEUE_DEPTH 128
#define RD_BLOCKSIZE		512
>>>>>>> refs/remotes/origin/master

/* Used in target_core_init_configfs() for virtual LUN 0 access */
int __init rd_module_init(void);
void rd_module_exit(void);

<<<<<<< HEAD
#define RRF_EMULATE_CDB		0x01
#define RRF_GOT_LBA		0x02

struct rd_request {
	struct se_task	rd_task;

<<<<<<< HEAD
	/* SCSI CDB from iSCSI Command PDU */
	unsigned char	rd_scsi_cdb[TCM_MAX_COMMAND_SIZE];
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* Offset from start of page */
	u32		rd_offset;
	/* Starting page in Ramdisk for request */
	u32		rd_page;
	/* Total number of pages needed for request */
	u32		rd_page_count;
	/* Scatterlist count */
	u32		rd_size;
<<<<<<< HEAD
	/* Ramdisk device */
	struct rd_dev	*rd_dev;
=======
>>>>>>> refs/remotes/origin/cm-10.0
} ____cacheline_aligned;

=======
>>>>>>> refs/remotes/origin/master
struct rd_dev_sg_table {
	u32		page_start_offset;
	u32		page_end_offset;
	u32		rd_sg_count;
	struct scatterlist *sg_table;
} ____cacheline_aligned;

#define RDF_HAS_PAGE_COUNT	0x01
<<<<<<< HEAD

struct rd_dev {
	int		rd_direct;
=======
#define RDF_NULLIO		0x02

struct rd_dev {
	struct se_device dev;
>>>>>>> refs/remotes/origin/master
	u32		rd_flags;
	/* Unique Ramdisk Device ID in Ramdisk HBA */
	u32		rd_dev_id;
	/* Total page count for ramdisk device */
	u32		rd_page_count;
	/* Number of SG tables in sg_table_array */
	u32		sg_table_count;
<<<<<<< HEAD
	u32		rd_queue_depth;
=======
>>>>>>> refs/remotes/origin/master
	/* Array of rd_dev_sg_table_t containing scatterlists */
	struct rd_dev_sg_table *sg_table_array;
	/* Ramdisk HBA device is connected to */
	struct rd_host *rd_host;
} ____cacheline_aligned;

struct rd_host {
	u32		rd_host_dev_id_count;
	u32		rd_host_id;		/* Unique Ramdisk Host ID */
} ____cacheline_aligned;

#endif /* TARGET_CORE_RD_H */
