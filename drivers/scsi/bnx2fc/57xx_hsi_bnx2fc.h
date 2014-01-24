#ifndef __57XX_FCOE_HSI_LINUX_LE__
#define __57XX_FCOE_HSI_LINUX_LE__

/*
 * common data for all protocols
 */
struct b577xx_doorbell_hdr {
	u8 header;
#define B577XX_DOORBELL_HDR_RX (0x1<<0)
#define B577XX_DOORBELL_HDR_RX_SHIFT 0
#define B577XX_DOORBELL_HDR_DB_TYPE (0x1<<1)
#define B577XX_DOORBELL_HDR_DB_TYPE_SHIFT 1
#define B577XX_DOORBELL_HDR_DPM_SIZE (0x3<<2)
#define B577XX_DOORBELL_HDR_DPM_SIZE_SHIFT 2
#define B577XX_DOORBELL_HDR_CONN_TYPE (0xF<<4)
#define B577XX_DOORBELL_HDR_CONN_TYPE_SHIFT 4
};

/*
 * doorbell message sent to the chip
 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
struct b577xx_doorbell {
#if defined(__BIG_ENDIAN)
	u16 zero_fill2;
	u8 zero_fill1;
	struct b577xx_doorbell_hdr header;
#elif defined(__LITTLE_ENDIAN)
	struct b577xx_doorbell_hdr header;
	u8 zero_fill1;
	u16 zero_fill2;
#endif
};



/*
 * doorbell message sent to the chip
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct b577xx_doorbell_set_prod {
#if defined(__BIG_ENDIAN)
	u16 prod;
	u8 zero_fill1;
	struct b577xx_doorbell_hdr header;
#elif defined(__LITTLE_ENDIAN)
	struct b577xx_doorbell_hdr header;
	u8 zero_fill1;
	u16 prod;
#endif
};


struct regpair {
	__le32 lo;
	__le32 hi;
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Fixed size structure in order to plant it in Union structure
 */
struct fcoe_abts_rsp_union {
	u32 r_ctl;
	u32 abts_rsp_payload[7];
=======
=======
>>>>>>> refs/remotes/origin/master
 * ABTS info $$KEEP_ENDIANNESS$$
 */
struct fcoe_abts_info {
	__le16 aborted_task_id;
	__le16 reserved0;
	__le32 reserved1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * 4 regs size
 */
struct fcoe_bd_ctx {
	u32 buf_addr_hi;
	u32 buf_addr_lo;
#if defined(__BIG_ENDIAN)
	u16 rsrv0;
	u16 buf_len;
#elif defined(__LITTLE_ENDIAN)
	u16 buf_len;
	u16 rsrv0;
#endif
#if defined(__BIG_ENDIAN)
	u16 rsrv1;
	u16 flags;
#elif defined(__LITTLE_ENDIAN)
	u16 flags;
	u16 rsrv1;
#endif
};


struct fcoe_cleanup_flow_info {
#if defined(__BIG_ENDIAN)
	u16 reserved1;
	u16 task_id;
#elif defined(__LITTLE_ENDIAN)
	u16 task_id;
	u16 reserved1;
#endif
	u32 reserved2[7];
};


struct fcoe_fcp_cmd_payload {
	u32 opaque[8];
};

struct fcoe_fc_hdr {
#if defined(__BIG_ENDIAN)
	u8 cs_ctl;
	u8 s_id[3];
#elif defined(__LITTLE_ENDIAN)
	u8 s_id[3];
	u8 cs_ctl;
#endif
#if defined(__BIG_ENDIAN)
	u8 r_ctl;
	u8 d_id[3];
#elif defined(__LITTLE_ENDIAN)
	u8 d_id[3];
	u8 r_ctl;
#endif
#if defined(__BIG_ENDIAN)
	u8 seq_id;
	u8 df_ctl;
	u16 seq_cnt;
#elif defined(__LITTLE_ENDIAN)
	u16 seq_cnt;
	u8 df_ctl;
	u8 seq_id;
#endif
#if defined(__BIG_ENDIAN)
	u8 type;
	u8 f_ctl[3];
#elif defined(__LITTLE_ENDIAN)
	u8 f_ctl[3];
	u8 type;
#endif
	u32 parameters;
#if defined(__BIG_ENDIAN)
	u16 ox_id;
	u16 rx_id;
#elif defined(__LITTLE_ENDIAN)
	u16 rx_id;
	u16 ox_id;
#endif
};

struct fcoe_fc_frame {
	struct fcoe_fc_hdr fc_hdr;
	u32 reserved0[2];
};

union fcoe_cmd_flow_info {
	struct fcoe_fcp_cmd_payload fcp_cmd_payload;
	struct fcoe_fc_frame mp_fc_frame;
};



=======
=======
>>>>>>> refs/remotes/origin/master
 * Fixed size structure in order to plant it in Union structure
 * $$KEEP_ENDIANNESS$$
 */
struct fcoe_abts_rsp_union {
	u8 r_ctl;
	u8 rsrv[3];
	__le32 abts_rsp_payload[7];
};


/*
 * 4 regs size $$KEEP_ENDIANNESS$$
 */
struct fcoe_bd_ctx {
	__le32 buf_addr_hi;
	__le32 buf_addr_lo;
	__le16 buf_len;
	__le16 rsrv0;
	__le16 flags;
	__le16 rsrv1;
};


/*
 * FCoE cached sges context $$KEEP_ENDIANNESS$$
 */
struct fcoe_cached_sge_ctx {
	struct regpair cur_buf_addr;
	__le16 cur_buf_rem;
	__le16 second_buf_rem;
	struct regpair second_buf_addr;
};


/*
 * Cleanup info $$KEEP_ENDIANNESS$$
 */
struct fcoe_cleanup_info {
	__le16 cleaned_task_id;
	__le16 rolled_tx_seq_cnt;
	__le32 rolled_tx_data_offset;
};


/*
 * Fcp RSP flags $$KEEP_ENDIANNESS$$
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct fcoe_fcp_rsp_flags {
	u8 flags;
#define FCOE_FCP_RSP_FLAGS_FCP_RSP_LEN_VALID (0x1<<0)
#define FCOE_FCP_RSP_FLAGS_FCP_RSP_LEN_VALID_SHIFT 0
#define FCOE_FCP_RSP_FLAGS_FCP_SNS_LEN_VALID (0x1<<1)
#define FCOE_FCP_RSP_FLAGS_FCP_SNS_LEN_VALID_SHIFT 1
#define FCOE_FCP_RSP_FLAGS_FCP_RESID_OVER (0x1<<2)
#define FCOE_FCP_RSP_FLAGS_FCP_RESID_OVER_SHIFT 2
#define FCOE_FCP_RSP_FLAGS_FCP_RESID_UNDER (0x1<<3)
#define FCOE_FCP_RSP_FLAGS_FCP_RESID_UNDER_SHIFT 3
#define FCOE_FCP_RSP_FLAGS_FCP_CONF_REQ (0x1<<4)
#define FCOE_FCP_RSP_FLAGS_FCP_CONF_REQ_SHIFT 4
#define FCOE_FCP_RSP_FLAGS_FCP_BIDI_FLAGS (0x7<<5)
#define FCOE_FCP_RSP_FLAGS_FCP_BIDI_FLAGS_SHIFT 5
};

<<<<<<< HEAD
<<<<<<< HEAD

struct fcoe_fcp_rsp_payload {
	struct regpair reserved0;
	u32 fcp_resid;
#if defined(__BIG_ENDIAN)
	u16 retry_delay_timer;
	struct fcoe_fcp_rsp_flags fcp_flags;
	u8 scsi_status_code;
#elif defined(__LITTLE_ENDIAN)
	u8 scsi_status_code;
	struct fcoe_fcp_rsp_flags fcp_flags;
	u16 retry_delay_timer;
#endif
	u32 fcp_rsp_len;
	u32 fcp_sns_len;
};


/*
 * Fixed size structure in order to plant it in Union structure
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Fcp RSP payload $$KEEP_ENDIANNESS$$
 */
struct fcoe_fcp_rsp_payload {
	struct regpair reserved0;
	__le32 fcp_resid;
	u8 scsi_status_code;
	struct fcoe_fcp_rsp_flags fcp_flags;
	__le16 retry_delay_timer;
	__le32 fcp_rsp_len;
	__le32 fcp_sns_len;
};

/*
 * Fixed size structure in order to plant it in Union structure
 * $$KEEP_ENDIANNESS$$
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */
struct fcoe_fcp_rsp_union {
	struct fcoe_fcp_rsp_payload payload;
	struct regpair reserved0;
};

<<<<<<< HEAD
<<<<<<< HEAD

struct fcoe_fcp_xfr_rdy_payload {
	u32 burst_len;
	u32 data_ro;
};

struct fcoe_read_flow_info {
	struct fcoe_fc_hdr fc_data_in_hdr;
	u32 reserved[2];
};

struct fcoe_write_flow_info {
	struct fcoe_fc_hdr fc_data_out_hdr;
	struct fcoe_fcp_xfr_rdy_payload fcp_xfr_payload;
};

union fcoe_rsp_flow_info {
	struct fcoe_fcp_rsp_union fcp_rsp;
	struct fcoe_abts_rsp_union abts_rsp;
};

/*
 * 32 bytes used for general purposes
 */
union fcoe_general_task_ctx {
	union fcoe_cmd_flow_info cmd_info;
	struct fcoe_read_flow_info read_info;
	struct fcoe_write_flow_info write_info;
	union fcoe_rsp_flow_info rsp_info;
	struct fcoe_cleanup_flow_info cleanup_info;
	u32 comp_info[8];
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * FC header $$KEEP_ENDIANNESS$$
 */
struct fcoe_fc_hdr {
	u8 s_id[3];
	u8 cs_ctl;
	u8 d_id[3];
	u8 r_ctl;
	__le16 seq_cnt;
	u8 df_ctl;
	u8 seq_id;
	u8 f_ctl[3];
	u8 type;
	__le32 parameters;
	__le16 rx_id;
	__le16 ox_id;
};

/*
 * FC header union $$KEEP_ENDIANNESS$$
 */
struct fcoe_mp_rsp_union {
	struct fcoe_fc_hdr fc_hdr;
	__le32 mp_payload_len;
	__le32 rsrv;
};

/*
 * Completion information $$KEEP_ENDIANNESS$$
 */
union fcoe_comp_flow_info {
	struct fcoe_fcp_rsp_union fcp_rsp;
	struct fcoe_abts_rsp_union abts_rsp;
	struct fcoe_mp_rsp_union mp_rsp;
	__le32 opaque[8];
};


/*
 * External ABTS info $$KEEP_ENDIANNESS$$
 */
struct fcoe_ext_abts_info {
	__le32 rsrv0[6];
	struct fcoe_abts_info ctx;
};


/*
 * External cleanup info $$KEEP_ENDIANNESS$$
 */
struct fcoe_ext_cleanup_info {
	__le32 rsrv0[6];
	struct fcoe_cleanup_info ctx;
};


/*
 * Fcoe FW Tx sequence context $$KEEP_ENDIANNESS$$
 */
struct fcoe_fw_tx_seq_ctx {
	__le32 data_offset;
	__le16 seq_cnt;
	__le16 rsrv0;
};

/*
 * Fcoe external FW Tx sequence context $$KEEP_ENDIANNESS$$
 */
struct fcoe_ext_fw_tx_seq_ctx {
	__le32 rsrv0[6];
	struct fcoe_fw_tx_seq_ctx ctx;
};


/*
 * FCoE multiple sges context $$KEEP_ENDIANNESS$$
 */
struct fcoe_mul_sges_ctx {
	struct regpair cur_sge_addr;
	__le16 cur_sge_off;
	u8 cur_sge_idx;
	u8 sgl_size;
};

/*
 * FCoE external multiple sges context $$KEEP_ENDIANNESS$$
 */
struct fcoe_ext_mul_sges_ctx {
	struct fcoe_mul_sges_ctx mul_sgl;
	struct regpair rsrv0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE KCQ CQE parameters
 */
union fcoe_kcqe_params {
	u32 reserved0[4];
};

/*
 * FCoE KCQ CQE
 */
struct fcoe_kcqe {
	u32 fcoe_conn_id;
	u32 completion_status;
	u32 fcoe_conn_context_id;
	union fcoe_kcqe_params params;
#if defined(__BIG_ENDIAN)
	u8 flags;
#define FCOE_KCQE_RESERVED0 (0x7<<0)
#define FCOE_KCQE_RESERVED0_SHIFT 0
#define FCOE_KCQE_RAMROD_COMPLETION (0x1<<3)
#define FCOE_KCQE_RAMROD_COMPLETION_SHIFT 3
#define FCOE_KCQE_LAYER_CODE (0x7<<4)
#define FCOE_KCQE_LAYER_CODE_SHIFT 4
#define FCOE_KCQE_LINKED_WITH_NEXT (0x1<<7)
#define FCOE_KCQE_LINKED_WITH_NEXT_SHIFT 7
	u8 op_code;
	u16 qe_self_seq;
#elif defined(__LITTLE_ENDIAN)
	u16 qe_self_seq;
=======
=======
>>>>>>> refs/remotes/origin/master
 * FCP CMD payload $$KEEP_ENDIANNESS$$
 */
struct fcoe_fcp_cmd_payload {
	__le32 opaque[8];
};





/*
 * Fcp xfr rdy payload $$KEEP_ENDIANNESS$$
 */
struct fcoe_fcp_xfr_rdy_payload {
	__le32 burst_len;
	__le32 data_ro;
};


/*
 * FC frame $$KEEP_ENDIANNESS$$
 */
struct fcoe_fc_frame {
	struct fcoe_fc_hdr fc_hdr;
	__le32 reserved0[2];
};




/*
 * FCoE KCQ CQE parameters $$KEEP_ENDIANNESS$$
 */
union fcoe_kcqe_params {
	__le32 reserved0[4];
};

/*
 * FCoE KCQ CQE $$KEEP_ENDIANNESS$$
 */
struct fcoe_kcqe {
	__le32 fcoe_conn_id;
	__le32 completion_status;
	__le32 fcoe_conn_context_id;
	union fcoe_kcqe_params params;
	__le16 qe_self_seq;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u8 op_code;
	u8 flags;
#define FCOE_KCQE_RESERVED0 (0x7<<0)
#define FCOE_KCQE_RESERVED0_SHIFT 0
#define FCOE_KCQE_RAMROD_COMPLETION (0x1<<3)
#define FCOE_KCQE_RAMROD_COMPLETION_SHIFT 3
#define FCOE_KCQE_LAYER_CODE (0x7<<4)
#define FCOE_KCQE_LAYER_CODE_SHIFT 4
#define FCOE_KCQE_LINKED_WITH_NEXT (0x1<<7)
#define FCOE_KCQE_LINKED_WITH_NEXT_SHIFT 7
<<<<<<< HEAD
<<<<<<< HEAD
#endif
};

/*
 * FCoE KWQE header
 */
struct fcoe_kwqe_header {
#if defined(__BIG_ENDIAN)
	u8 flags;
#define FCOE_KWQE_HEADER_RESERVED0 (0xF<<0)
#define FCOE_KWQE_HEADER_RESERVED0_SHIFT 0
#define FCOE_KWQE_HEADER_LAYER_CODE (0x7<<4)
#define FCOE_KWQE_HEADER_LAYER_CODE_SHIFT 4
#define FCOE_KWQE_HEADER_RESERVED1 (0x1<<7)
#define FCOE_KWQE_HEADER_RESERVED1_SHIFT 7
	u8 op_code;
#elif defined(__LITTLE_ENDIAN)
=======
=======
>>>>>>> refs/remotes/origin/master
};



/*
 * FCoE KWQE header $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_header {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u8 op_code;
	u8 flags;
#define FCOE_KWQE_HEADER_RESERVED0 (0xF<<0)
#define FCOE_KWQE_HEADER_RESERVED0_SHIFT 0
#define FCOE_KWQE_HEADER_LAYER_CODE (0x7<<4)
#define FCOE_KWQE_HEADER_LAYER_CODE_SHIFT 4
#define FCOE_KWQE_HEADER_RESERVED1 (0x1<<7)
#define FCOE_KWQE_HEADER_RESERVED1_SHIFT 7
<<<<<<< HEAD
<<<<<<< HEAD
#endif
};

/*
 * FCoE firmware init request 1
 */
struct fcoe_kwqe_init1 {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 num_tasks;
#elif defined(__LITTLE_ENDIAN)
	u16 num_tasks;
	struct fcoe_kwqe_header hdr;
#endif
	u32 task_list_pbl_addr_lo;
	u32 task_list_pbl_addr_hi;
	u32 dummy_buffer_addr_lo;
	u32 dummy_buffer_addr_hi;
#if defined(__BIG_ENDIAN)
	u16 rq_num_wqes;
	u16 sq_num_wqes;
#elif defined(__LITTLE_ENDIAN)
	u16 sq_num_wqes;
	u16 rq_num_wqes;
#endif
#if defined(__BIG_ENDIAN)
	u16 cq_num_wqes;
	u16 rq_buffer_log_size;
#elif defined(__LITTLE_ENDIAN)
	u16 rq_buffer_log_size;
	u16 cq_num_wqes;
#endif
#if defined(__BIG_ENDIAN)
	u8 flags;
#define FCOE_KWQE_INIT1_LOG_PAGE_SIZE (0xF<<0)
#define FCOE_KWQE_INIT1_LOG_PAGE_SIZE_SHIFT 0
#define FCOE_KWQE_INIT1_LOG_CACHED_PBES_PER_FUNC (0x7<<4)
#define FCOE_KWQE_INIT1_LOG_CACHED_PBES_PER_FUNC_SHIFT 4
#define FCOE_KWQE_INIT1_RESERVED1 (0x1<<7)
#define FCOE_KWQE_INIT1_RESERVED1_SHIFT 7
	u8 num_sessions_log;
	u16 mtu;
#elif defined(__LITTLE_ENDIAN)
	u16 mtu;
=======
=======
>>>>>>> refs/remotes/origin/master
};

/*
 * FCoE firmware init request 1 $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_init1 {
	__le16 num_tasks;
	struct fcoe_kwqe_header hdr;
	__le32 task_list_pbl_addr_lo;
	__le32 task_list_pbl_addr_hi;
	__le32 dummy_buffer_addr_lo;
	__le32 dummy_buffer_addr_hi;
	__le16 sq_num_wqes;
	__le16 rq_num_wqes;
	__le16 rq_buffer_log_size;
	__le16 cq_num_wqes;
	__le16 mtu;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u8 num_sessions_log;
	u8 flags;
#define FCOE_KWQE_INIT1_LOG_PAGE_SIZE (0xF<<0)
#define FCOE_KWQE_INIT1_LOG_PAGE_SIZE_SHIFT 0
#define FCOE_KWQE_INIT1_LOG_CACHED_PBES_PER_FUNC (0x7<<4)
#define FCOE_KWQE_INIT1_LOG_CACHED_PBES_PER_FUNC_SHIFT 4
#define FCOE_KWQE_INIT1_RESERVED1 (0x1<<7)
#define FCOE_KWQE_INIT1_RESERVED1_SHIFT 7
<<<<<<< HEAD
<<<<<<< HEAD
#endif
};

/*
 * FCoE firmware init request 2
 */
struct fcoe_kwqe_init2 {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 reserved0;
#elif defined(__LITTLE_ENDIAN)
	u16 reserved0;
	struct fcoe_kwqe_header hdr;
#endif
	u32 hash_tbl_pbl_addr_lo;
	u32 hash_tbl_pbl_addr_hi;
	u32 t2_hash_tbl_addr_lo;
	u32 t2_hash_tbl_addr_hi;
	u32 t2_ptr_hash_tbl_addr_lo;
	u32 t2_ptr_hash_tbl_addr_hi;
	u32 free_list_count;
};

/*
 * FCoE firmware init request 3
 */
struct fcoe_kwqe_init3 {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 reserved0;
#elif defined(__LITTLE_ENDIAN)
	u16 reserved0;
	struct fcoe_kwqe_header hdr;
#endif
	u32 error_bit_map_lo;
	u32 error_bit_map_hi;
#if defined(__BIG_ENDIAN)
	u8 reserved21[3];
	u8 cached_session_enable;
#elif defined(__LITTLE_ENDIAN)
	u8 cached_session_enable;
	u8 reserved21[3];
#endif
	u32 reserved2[4];
};

/*
 * FCoE connection offload request 1
 */
struct fcoe_kwqe_conn_offload1 {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 fcoe_conn_id;
#elif defined(__LITTLE_ENDIAN)
	u16 fcoe_conn_id;
	struct fcoe_kwqe_header hdr;
#endif
	u32 sq_addr_lo;
	u32 sq_addr_hi;
	u32 rq_pbl_addr_lo;
	u32 rq_pbl_addr_hi;
	u32 rq_first_pbe_addr_lo;
	u32 rq_first_pbe_addr_hi;
#if defined(__BIG_ENDIAN)
	u16 reserved0;
	u16 rq_prod;
#elif defined(__LITTLE_ENDIAN)
	u16 rq_prod;
	u16 reserved0;
#endif
};

/*
 * FCoE connection offload request 2
 */
struct fcoe_kwqe_conn_offload2 {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 tx_max_fc_pay_len;
#elif defined(__LITTLE_ENDIAN)
	u16 tx_max_fc_pay_len;
	struct fcoe_kwqe_header hdr;
#endif
	u32 cq_addr_lo;
	u32 cq_addr_hi;
	u32 xferq_addr_lo;
	u32 xferq_addr_hi;
	u32 conn_db_addr_lo;
	u32 conn_db_addr_hi;
	u32 reserved1;
};

/*
 * FCoE connection offload request 3
 */
struct fcoe_kwqe_conn_offload3 {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 vlan_tag;
#define FCOE_KWQE_CONN_OFFLOAD3_VLAN_ID (0xFFF<<0)
#define FCOE_KWQE_CONN_OFFLOAD3_VLAN_ID_SHIFT 0
#define FCOE_KWQE_CONN_OFFLOAD3_CFI (0x1<<12)
#define FCOE_KWQE_CONN_OFFLOAD3_CFI_SHIFT 12
#define FCOE_KWQE_CONN_OFFLOAD3_PRIORITY (0x7<<13)
#define FCOE_KWQE_CONN_OFFLOAD3_PRIORITY_SHIFT 13
#elif defined(__LITTLE_ENDIAN)
	u16 vlan_tag;
=======
=======
>>>>>>> refs/remotes/origin/master
};

/*
 * FCoE firmware init request 2 $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_init2 {
	u8 hsi_major_version;
	u8 hsi_minor_version;
	struct fcoe_kwqe_header hdr;
	__le32 hash_tbl_pbl_addr_lo;
	__le32 hash_tbl_pbl_addr_hi;
	__le32 t2_hash_tbl_addr_lo;
	__le32 t2_hash_tbl_addr_hi;
	__le32 t2_ptr_hash_tbl_addr_lo;
	__le32 t2_ptr_hash_tbl_addr_hi;
	__le32 free_list_count;
};

/*
 * FCoE firmware init request 3 $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_init3 {
	__le16 reserved0;
	struct fcoe_kwqe_header hdr;
	__le32 error_bit_map_lo;
	__le32 error_bit_map_hi;
	u8 perf_config;
	u8 reserved21[3];
	__le32 reserved2[4];
};

/*
 * FCoE connection offload request 1 $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_conn_offload1 {
	__le16 fcoe_conn_id;
	struct fcoe_kwqe_header hdr;
	__le32 sq_addr_lo;
	__le32 sq_addr_hi;
	__le32 rq_pbl_addr_lo;
	__le32 rq_pbl_addr_hi;
	__le32 rq_first_pbe_addr_lo;
	__le32 rq_first_pbe_addr_hi;
	__le16 rq_prod;
	__le16 reserved0;
};

/*
 * FCoE connection offload request 2 $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_conn_offload2 {
	__le16 tx_max_fc_pay_len;
	struct fcoe_kwqe_header hdr;
	__le32 cq_addr_lo;
	__le32 cq_addr_hi;
	__le32 xferq_addr_lo;
	__le32 xferq_addr_hi;
	__le32 conn_db_addr_lo;
	__le32 conn_db_addr_hi;
	__le32 reserved1;
};

/*
 * FCoE connection offload request 3 $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_conn_offload3 {
	__le16 vlan_tag;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define FCOE_KWQE_CONN_OFFLOAD3_VLAN_ID (0xFFF<<0)
#define FCOE_KWQE_CONN_OFFLOAD3_VLAN_ID_SHIFT 0
#define FCOE_KWQE_CONN_OFFLOAD3_CFI (0x1<<12)
#define FCOE_KWQE_CONN_OFFLOAD3_CFI_SHIFT 12
#define FCOE_KWQE_CONN_OFFLOAD3_PRIORITY (0x7<<13)
#define FCOE_KWQE_CONN_OFFLOAD3_PRIORITY_SHIFT 13
	struct fcoe_kwqe_header hdr;
<<<<<<< HEAD
<<<<<<< HEAD
#endif
#if defined(__BIG_ENDIAN)
	u8 tx_max_conc_seqs_c3;
	u8 s_id[3];
#elif defined(__LITTLE_ENDIAN)
	u8 s_id[3];
	u8 tx_max_conc_seqs_c3;
#endif
#if defined(__BIG_ENDIAN)
	u8 flags;
#define FCOE_KWQE_CONN_OFFLOAD3_B_MUL_N_PORT_IDS (0x1<<0)
#define FCOE_KWQE_CONN_OFFLOAD3_B_MUL_N_PORT_IDS_SHIFT 0
#define FCOE_KWQE_CONN_OFFLOAD3_B_E_D_TOV_RES (0x1<<1)
#define FCOE_KWQE_CONN_OFFLOAD3_B_E_D_TOV_RES_SHIFT 1
#define FCOE_KWQE_CONN_OFFLOAD3_B_CONT_INCR_SEQ_CNT (0x1<<2)
#define FCOE_KWQE_CONN_OFFLOAD3_B_CONT_INCR_SEQ_CNT_SHIFT 2
#define FCOE_KWQE_CONN_OFFLOAD3_B_CONF_REQ (0x1<<3)
#define FCOE_KWQE_CONN_OFFLOAD3_B_CONF_REQ_SHIFT 3
#define FCOE_KWQE_CONN_OFFLOAD3_B_REC_VALID (0x1<<4)
#define FCOE_KWQE_CONN_OFFLOAD3_B_REC_VALID_SHIFT 4
#define FCOE_KWQE_CONN_OFFLOAD3_B_C2_VALID (0x1<<5)
#define FCOE_KWQE_CONN_OFFLOAD3_B_C2_VALID_SHIFT 5
#define FCOE_KWQE_CONN_OFFLOAD3_B_ACK_0 (0x1<<6)
#define FCOE_KWQE_CONN_OFFLOAD3_B_ACK_0_SHIFT 6
#define FCOE_KWQE_CONN_OFFLOAD3_B_VLAN_FLAG (0x1<<7)
#define FCOE_KWQE_CONN_OFFLOAD3_B_VLAN_FLAG_SHIFT 7
	u8 d_id[3];
#elif defined(__LITTLE_ENDIAN)
=======
	u8 s_id[3];
	u8 tx_max_conc_seqs_c3;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u8 s_id[3];
	u8 tx_max_conc_seqs_c3;
>>>>>>> refs/remotes/origin/master
	u8 d_id[3];
	u8 flags;
#define FCOE_KWQE_CONN_OFFLOAD3_B_MUL_N_PORT_IDS (0x1<<0)
#define FCOE_KWQE_CONN_OFFLOAD3_B_MUL_N_PORT_IDS_SHIFT 0
#define FCOE_KWQE_CONN_OFFLOAD3_B_E_D_TOV_RES (0x1<<1)
#define FCOE_KWQE_CONN_OFFLOAD3_B_E_D_TOV_RES_SHIFT 1
#define FCOE_KWQE_CONN_OFFLOAD3_B_CONT_INCR_SEQ_CNT (0x1<<2)
#define FCOE_KWQE_CONN_OFFLOAD3_B_CONT_INCR_SEQ_CNT_SHIFT 2
#define FCOE_KWQE_CONN_OFFLOAD3_B_CONF_REQ (0x1<<3)
#define FCOE_KWQE_CONN_OFFLOAD3_B_CONF_REQ_SHIFT 3
#define FCOE_KWQE_CONN_OFFLOAD3_B_REC_VALID (0x1<<4)
#define FCOE_KWQE_CONN_OFFLOAD3_B_REC_VALID_SHIFT 4
#define FCOE_KWQE_CONN_OFFLOAD3_B_C2_VALID (0x1<<5)
#define FCOE_KWQE_CONN_OFFLOAD3_B_C2_VALID_SHIFT 5
#define FCOE_KWQE_CONN_OFFLOAD3_B_ACK_0 (0x1<<6)
#define FCOE_KWQE_CONN_OFFLOAD3_B_ACK_0_SHIFT 6
#define FCOE_KWQE_CONN_OFFLOAD3_B_VLAN_FLAG (0x1<<7)
#define FCOE_KWQE_CONN_OFFLOAD3_B_VLAN_FLAG_SHIFT 7
<<<<<<< HEAD
<<<<<<< HEAD
#endif
	u32 reserved;
	u32 confq_first_pbe_addr_lo;
	u32 confq_first_pbe_addr_hi;
#if defined(__BIG_ENDIAN)
	u16 rx_max_fc_pay_len;
	u16 tx_total_conc_seqs;
#elif defined(__LITTLE_ENDIAN)
	u16 tx_total_conc_seqs;
	u16 rx_max_fc_pay_len;
#endif
#if defined(__BIG_ENDIAN)
	u8 rx_open_seqs_exch_c3;
	u8 rx_max_conc_seqs_c3;
	u16 rx_total_conc_seqs;
#elif defined(__LITTLE_ENDIAN)
	u16 rx_total_conc_seqs;
	u8 rx_max_conc_seqs_c3;
	u8 rx_open_seqs_exch_c3;
#endif
};

/*
 * FCoE connection offload request 4
 */
struct fcoe_kwqe_conn_offload4 {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u8 reserved2;
	u8 e_d_tov_timer_val;
#elif defined(__LITTLE_ENDIAN)
	u8 e_d_tov_timer_val;
	u8 reserved2;
	struct fcoe_kwqe_header hdr;
#endif
	u8 src_mac_addr_lo32[4];
#if defined(__BIG_ENDIAN)
	u8 dst_mac_addr_hi16[2];
	u8 src_mac_addr_hi16[2];
#elif defined(__LITTLE_ENDIAN)
	u8 src_mac_addr_hi16[2];
	u8 dst_mac_addr_hi16[2];
#endif
	u8 dst_mac_addr_lo32[4];
	u32 lcq_addr_lo;
	u32 lcq_addr_hi;
	u32 confq_pbl_base_addr_lo;
	u32 confq_pbl_base_addr_hi;
};

/*
 * FCoE connection enable request
 */
struct fcoe_kwqe_conn_enable_disable {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 reserved0;
#elif defined(__LITTLE_ENDIAN)
	u16 reserved0;
	struct fcoe_kwqe_header hdr;
#endif
	u8 src_mac_addr_lo32[4];
#if defined(__BIG_ENDIAN)
	u16 vlan_tag;
#define FCOE_KWQE_CONN_ENABLE_DISABLE_VLAN_ID (0xFFF<<0)
#define FCOE_KWQE_CONN_ENABLE_DISABLE_VLAN_ID_SHIFT 0
#define FCOE_KWQE_CONN_ENABLE_DISABLE_CFI (0x1<<12)
#define FCOE_KWQE_CONN_ENABLE_DISABLE_CFI_SHIFT 12
#define FCOE_KWQE_CONN_ENABLE_DISABLE_PRIORITY (0x7<<13)
#define FCOE_KWQE_CONN_ENABLE_DISABLE_PRIORITY_SHIFT 13
	u8 src_mac_addr_hi16[2];
#elif defined(__LITTLE_ENDIAN)
	u8 src_mac_addr_hi16[2];
=======
=======
>>>>>>> refs/remotes/origin/master
	__le32 reserved;
	__le32 confq_first_pbe_addr_lo;
	__le32 confq_first_pbe_addr_hi;
	__le16 tx_total_conc_seqs;
	__le16 rx_max_fc_pay_len;
	__le16 rx_total_conc_seqs;
	u8 rx_max_conc_seqs_c3;
	u8 rx_open_seqs_exch_c3;
};

/*
 * FCoE connection offload request 4 $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_conn_offload4 {
	u8 e_d_tov_timer_val;
	u8 reserved2;
	struct fcoe_kwqe_header hdr;
	u8 src_mac_addr_lo[2];
	u8 src_mac_addr_mid[2];
	u8 src_mac_addr_hi[2];
	u8 dst_mac_addr_hi[2];
	u8 dst_mac_addr_lo[2];
	u8 dst_mac_addr_mid[2];
	__le32 lcq_addr_lo;
	__le32 lcq_addr_hi;
	__le32 confq_pbl_base_addr_lo;
	__le32 confq_pbl_base_addr_hi;
};

/*
 * FCoE connection enable request $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_conn_enable_disable {
	__le16 reserved0;
	struct fcoe_kwqe_header hdr;
	u8 src_mac_addr_lo[2];
	u8 src_mac_addr_mid[2];
	u8 src_mac_addr_hi[2];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u16 vlan_tag;
#define FCOE_KWQE_CONN_ENABLE_DISABLE_VLAN_ID (0xFFF<<0)
#define FCOE_KWQE_CONN_ENABLE_DISABLE_VLAN_ID_SHIFT 0
#define FCOE_KWQE_CONN_ENABLE_DISABLE_CFI (0x1<<12)
#define FCOE_KWQE_CONN_ENABLE_DISABLE_CFI_SHIFT 12
#define FCOE_KWQE_CONN_ENABLE_DISABLE_PRIORITY (0x7<<13)
#define FCOE_KWQE_CONN_ENABLE_DISABLE_PRIORITY_SHIFT 13
<<<<<<< HEAD
<<<<<<< HEAD
#endif
	u8 dst_mac_addr_lo32[4];
#if defined(__BIG_ENDIAN)
	u16 reserved1;
	u8 dst_mac_addr_hi16[2];
#elif defined(__LITTLE_ENDIAN)
	u8 dst_mac_addr_hi16[2];
	u16 reserved1;
#endif
#if defined(__BIG_ENDIAN)
	u8 vlan_flag;
	u8 s_id[3];
#elif defined(__LITTLE_ENDIAN)
	u8 s_id[3];
	u8 vlan_flag;
#endif
#if defined(__BIG_ENDIAN)
	u8 reserved3;
	u8 d_id[3];
#elif defined(__LITTLE_ENDIAN)
	u8 d_id[3];
	u8 reserved3;
#endif
	u32 context_id;
	u32 conn_id;
	u32 reserved4;
};

/*
 * FCoE connection destroy request
 */
struct fcoe_kwqe_conn_destroy {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 reserved0;
#elif defined(__LITTLE_ENDIAN)
	u16 reserved0;
	struct fcoe_kwqe_header hdr;
#endif
	u32 context_id;
	u32 conn_id;
	u32 reserved1[5];
};

/*
 * FCoe destroy request
 */
struct fcoe_kwqe_destroy {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 reserved0;
#elif defined(__LITTLE_ENDIAN)
	u16 reserved0;
	struct fcoe_kwqe_header hdr;
#endif
	u32 reserved1[7];
};

/*
 * FCoe statistics request
 */
struct fcoe_kwqe_stat {
#if defined(__BIG_ENDIAN)
	struct fcoe_kwqe_header hdr;
	u16 reserved0;
#elif defined(__LITTLE_ENDIAN)
	u16 reserved0;
	struct fcoe_kwqe_header hdr;
#endif
	u32 stat_params_addr_lo;
	u32 stat_params_addr_hi;
	u32 reserved1[5];
};

/*
 * FCoE KWQ WQE
=======
=======
>>>>>>> refs/remotes/origin/master
	u8 dst_mac_addr_lo[2];
	u8 dst_mac_addr_mid[2];
	u8 dst_mac_addr_hi[2];
	__le16 reserved1;
	u8 s_id[3];
	u8 vlan_flag;
	u8 d_id[3];
	u8 reserved3;
	__le32 context_id;
	__le32 conn_id;
	__le32 reserved4;
};

/*
 * FCoE connection destroy request $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_conn_destroy {
	__le16 reserved0;
	struct fcoe_kwqe_header hdr;
	__le32 context_id;
	__le32 conn_id;
	__le32 reserved1[5];
};

/*
 * FCoe destroy request $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_destroy {
	__le16 reserved0;
	struct fcoe_kwqe_header hdr;
	__le32 reserved1[7];
};

/*
 * FCoe statistics request $$KEEP_ENDIANNESS$$
 */
struct fcoe_kwqe_stat {
	__le16 reserved0;
	struct fcoe_kwqe_header hdr;
	__le32 stat_params_addr_lo;
	__le32 stat_params_addr_hi;
	__le32 reserved1[5];
};

/*
 * FCoE KWQ WQE $$KEEP_ENDIANNESS$$
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */
union fcoe_kwqe {
	struct fcoe_kwqe_init1 init1;
	struct fcoe_kwqe_init2 init2;
	struct fcoe_kwqe_init3 init3;
	struct fcoe_kwqe_conn_offload1 conn_offload1;
	struct fcoe_kwqe_conn_offload2 conn_offload2;
	struct fcoe_kwqe_conn_offload3 conn_offload3;
	struct fcoe_kwqe_conn_offload4 conn_offload4;
	struct fcoe_kwqe_conn_enable_disable conn_enable_disable;
	struct fcoe_kwqe_conn_destroy conn_destroy;
	struct fcoe_kwqe_destroy destroy;
	struct fcoe_kwqe_stat statistics;
};

<<<<<<< HEAD
<<<<<<< HEAD
struct fcoe_mul_sges_ctx {
	struct regpair cur_sge_addr;
#if defined(__BIG_ENDIAN)
	u8 sgl_size;
	u8 cur_sge_idx;
	u16 cur_sge_off;
#elif defined(__LITTLE_ENDIAN)
	u16 cur_sge_off;
	u8 cur_sge_idx;
	u8 sgl_size;
#endif
};

=======
=======
>>>>>>> refs/remotes/origin/master















/*
 * TX SGL context $$KEEP_ENDIANNESS$$
 */
union fcoe_sgl_union_ctx {
	struct fcoe_cached_sge_ctx cached_sge;
	struct fcoe_ext_mul_sges_ctx sgl;
	__le32 opaque[5];
};

/*
 * Data-In/ELS/BLS information $$KEEP_ENDIANNESS$$
 */
struct fcoe_read_flow_info {
	union fcoe_sgl_union_ctx sgl_ctx;
	__le32 rsrv0[3];
};


/*
 * Fcoe stat context $$KEEP_ENDIANNESS$$
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct fcoe_s_stat_ctx {
	u8 flags;
#define FCOE_S_STAT_CTX_ACTIVE (0x1<<0)
#define FCOE_S_STAT_CTX_ACTIVE_SHIFT 0
#define FCOE_S_STAT_CTX_ACK_ABORT_SEQ_COND (0x1<<1)
#define FCOE_S_STAT_CTX_ACK_ABORT_SEQ_COND_SHIFT 1
#define FCOE_S_STAT_CTX_ABTS_PERFORMED (0x1<<2)
#define FCOE_S_STAT_CTX_ABTS_PERFORMED_SHIFT 2
#define FCOE_S_STAT_CTX_SEQ_TIMEOUT (0x1<<3)
#define FCOE_S_STAT_CTX_SEQ_TIMEOUT_SHIFT 3
#define FCOE_S_STAT_CTX_P_RJT (0x1<<4)
#define FCOE_S_STAT_CTX_P_RJT_SHIFT 4
#define FCOE_S_STAT_CTX_ACK_EOFT (0x1<<5)
#define FCOE_S_STAT_CTX_ACK_EOFT_SHIFT 5
#define FCOE_S_STAT_CTX_RSRV1 (0x3<<6)
#define FCOE_S_STAT_CTX_RSRV1_SHIFT 6
};

<<<<<<< HEAD
<<<<<<< HEAD
struct fcoe_seq_ctx {
#if defined(__BIG_ENDIAN)
	u16 low_seq_cnt;
	struct fcoe_s_stat_ctx s_stat;
	u8 seq_id;
#elif defined(__LITTLE_ENDIAN)
	u8 seq_id;
	struct fcoe_s_stat_ctx s_stat;
	u16 low_seq_cnt;
#endif
#if defined(__BIG_ENDIAN)
	u16 err_seq_cnt;
	u16 high_seq_cnt;
#elif defined(__LITTLE_ENDIAN)
	u16 high_seq_cnt;
	u16 err_seq_cnt;
#endif
	u32 low_exp_ro;
	u32 high_exp_ro;
};


struct fcoe_single_sge_ctx {
	struct regpair cur_buf_addr;
#if defined(__BIG_ENDIAN)
	u16 reserved0;
	u16 cur_buf_rem;
#elif defined(__LITTLE_ENDIAN)
	u16 cur_buf_rem;
	u16 reserved0;
#endif
};

union fcoe_sgl_ctx {
	struct fcoe_single_sge_ctx single_sge;
	struct fcoe_mul_sges_ctx mul_sges;
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Fcoe rx seq context $$KEEP_ENDIANNESS$$
 */
struct fcoe_rx_seq_ctx {
	u8 seq_id;
	struct fcoe_s_stat_ctx s_stat;
	__le16 seq_cnt;
	__le32 low_exp_ro;
	__le32 high_exp_ro;
};


/*
 * Fcoe rx_wr union context $$KEEP_ENDIANNESS$$
 */
union fcoe_rx_wr_union_ctx {
	struct fcoe_read_flow_info read_info;
	union fcoe_comp_flow_info comp_info;
	__le32 opaque[8];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};



/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE SQ element
 */
struct fcoe_sqe {
	u16 wqe;
=======
=======
>>>>>>> refs/remotes/origin/master
 * FCoE SQ element $$KEEP_ENDIANNESS$$
 */
struct fcoe_sqe {
	__le16 wqe;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define FCOE_SQE_TASK_ID (0x7FFF<<0)
#define FCOE_SQE_TASK_ID_SHIFT 0
#define FCOE_SQE_TOGGLE_BIT (0x1<<15)
#define FCOE_SQE_TOGGLE_BIT_SHIFT 15
};



<<<<<<< HEAD
<<<<<<< HEAD
struct fcoe_task_ctx_entry_tx_only {
	union fcoe_sgl_ctx sgl_ctx;
};

struct fcoe_task_ctx_entry_txwr_rxrd {
#if defined(__BIG_ENDIAN)
	u16 verify_tx_seq;
	u8 init_flags;
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_TASK_TYPE (0x7<<0)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_TASK_TYPE_SHIFT 0
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_DEV_TYPE (0x1<<3)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_DEV_TYPE_SHIFT 3
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_CLASS_TYPE (0x1<<4)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_CLASS_TYPE_SHIFT 4
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_SINGLE_SGE (0x1<<5)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_SINGLE_SGE_SHIFT 5
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_RSRV5 (0x3<<6)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_RSRV5_SHIFT 6
	u8 tx_flags;
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_TX_STATE (0xF<<0)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_TX_STATE_SHIFT 0
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_RSRV4 (0xF<<4)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_RSRV4_SHIFT 4
#elif defined(__LITTLE_ENDIAN)
	u8 tx_flags;
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_TX_STATE (0xF<<0)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_TX_STATE_SHIFT 0
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_RSRV4 (0xF<<4)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_RSRV4_SHIFT 4
	u8 init_flags;
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_TASK_TYPE (0x7<<0)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_TASK_TYPE_SHIFT 0
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_DEV_TYPE (0x1<<3)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_DEV_TYPE_SHIFT 3
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_CLASS_TYPE (0x1<<4)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_CLASS_TYPE_SHIFT 4
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_SINGLE_SGE (0x1<<5)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_SINGLE_SGE_SHIFT 5
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_RSRV5 (0x3<<6)
#define FCOE_TASK_CTX_ENTRY_TXWR_RXRD_RSRV5_SHIFT 6
	u16 verify_tx_seq;
#endif
};

/*
 * Common section. Both TX and RX processing might write and read from it in
 * different flows
 */
struct fcoe_task_ctx_entry_tx_rx_cmn {
	u32 data_2_trns;
	union fcoe_general_task_ctx general;
#if defined(__BIG_ENDIAN)
	u16 tx_low_seq_cnt;
	struct fcoe_s_stat_ctx tx_s_stat;
	u8 tx_seq_id;
#elif defined(__LITTLE_ENDIAN)
	u8 tx_seq_id;
	struct fcoe_s_stat_ctx tx_s_stat;
	u16 tx_low_seq_cnt;
#endif
	u32 common_flags;
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_CID (0xFFFFFF<<0)
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_CID_SHIFT 0
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_VALID (0x1<<24)
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_VALID_SHIFT 24
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_SEQ_INIT (0x1<<25)
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_SEQ_INIT_SHIFT 25
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_PEND_XFER (0x1<<26)
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_PEND_XFER_SHIFT 26
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_PEND_CONF (0x1<<27)
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_PEND_CONF_SHIFT 27
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_EXP_FIRST_FRAME (0x1<<28)
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_EXP_FIRST_FRAME_SHIFT 28
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_RSRV (0x7<<29)
#define FCOE_TASK_CTX_ENTRY_TX_RX_CMN_RSRV_SHIFT 29
};

struct fcoe_task_ctx_entry_rxwr_txrd {
#if defined(__BIG_ENDIAN)
	u16 rx_id;
	u16 rx_flags;
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_RX_STATE (0xF<<0)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_RX_STATE_SHIFT 0
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_NUM_RQ_WQE (0x7<<4)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_NUM_RQ_WQE_SHIFT 4
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_CONF_REQ (0x1<<7)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_CONF_REQ_SHIFT 7
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_MISS_FRAME (0x1<<8)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_MISS_FRAME_SHIFT 8
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_RESERVED0 (0x7F<<9)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_RESERVED0_SHIFT 9
#elif defined(__LITTLE_ENDIAN)
	u16 rx_flags;
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_RX_STATE (0xF<<0)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_RX_STATE_SHIFT 0
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_NUM_RQ_WQE (0x7<<4)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_NUM_RQ_WQE_SHIFT 4
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_CONF_REQ (0x1<<7)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_CONF_REQ_SHIFT 7
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_MISS_FRAME (0x1<<8)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_MISS_FRAME_SHIFT 8
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_RESERVED0 (0x7F<<9)
#define FCOE_TASK_CTX_ENTRY_RXWR_TXRD_RESERVED0_SHIFT 9
	u16 rx_id;
#endif
};

struct fcoe_task_ctx_entry_rx_only {
	struct fcoe_seq_ctx seq_ctx;
	struct fcoe_seq_ctx ooo_seq_ctx;
	u32 rsrv3;
	union fcoe_sgl_ctx sgl_ctx;
};

struct fcoe_task_ctx_entry {
	struct fcoe_task_ctx_entry_tx_only tx_wr_only;
	struct fcoe_task_ctx_entry_txwr_rxrd tx_wr_rx_rd;
	struct fcoe_task_ctx_entry_tx_rx_cmn cmn;
	struct fcoe_task_ctx_entry_rxwr_txrd rx_wr_tx_rd;
	struct fcoe_task_ctx_entry_rx_only rx_wr_only;
	u32 reserved[4];
};


/*
 * FCoE XFRQ element
 */
struct fcoe_xfrqe {
	u16 wqe;
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * 14 regs $$KEEP_ENDIANNESS$$
 */
struct fcoe_tce_tx_only {
	union fcoe_sgl_union_ctx sgl_ctx;
	__le32 rsrv0;
};

/*
 * 32 bytes (8 regs) used for TX only purposes $$KEEP_ENDIANNESS$$
 */
union fcoe_tx_wr_rx_rd_union_ctx {
	struct fcoe_fc_frame tx_frame;
	struct fcoe_fcp_cmd_payload fcp_cmd;
	struct fcoe_ext_cleanup_info cleanup;
	struct fcoe_ext_abts_info abts;
	struct fcoe_ext_fw_tx_seq_ctx tx_seq;
	__le32 opaque[8];
};

/*
 * tce_tx_wr_rx_rd_const $$KEEP_ENDIANNESS$$
 */
struct fcoe_tce_tx_wr_rx_rd_const {
	u8 init_flags;
#define FCOE_TCE_TX_WR_RX_RD_CONST_TASK_TYPE (0x7<<0)
#define FCOE_TCE_TX_WR_RX_RD_CONST_TASK_TYPE_SHIFT 0
#define FCOE_TCE_TX_WR_RX_RD_CONST_DEV_TYPE (0x1<<3)
#define FCOE_TCE_TX_WR_RX_RD_CONST_DEV_TYPE_SHIFT 3
#define FCOE_TCE_TX_WR_RX_RD_CONST_CLASS_TYPE (0x1<<4)
#define FCOE_TCE_TX_WR_RX_RD_CONST_CLASS_TYPE_SHIFT 4
#define FCOE_TCE_TX_WR_RX_RD_CONST_CACHED_SGE (0x3<<5)
#define FCOE_TCE_TX_WR_RX_RD_CONST_CACHED_SGE_SHIFT 5
#define FCOE_TCE_TX_WR_RX_RD_CONST_SUPPORT_REC_TOV (0x1<<7)
#define FCOE_TCE_TX_WR_RX_RD_CONST_SUPPORT_REC_TOV_SHIFT 7
	u8 tx_flags;
#define FCOE_TCE_TX_WR_RX_RD_CONST_TX_VALID (0x1<<0)
#define FCOE_TCE_TX_WR_RX_RD_CONST_TX_VALID_SHIFT 0
#define FCOE_TCE_TX_WR_RX_RD_CONST_TX_STATE (0xF<<1)
#define FCOE_TCE_TX_WR_RX_RD_CONST_TX_STATE_SHIFT 1
#define FCOE_TCE_TX_WR_RX_RD_CONST_RSRV1 (0x1<<5)
#define FCOE_TCE_TX_WR_RX_RD_CONST_RSRV1_SHIFT 5
#define FCOE_TCE_TX_WR_RX_RD_CONST_TX_SEQ_INIT (0x1<<6)
#define FCOE_TCE_TX_WR_RX_RD_CONST_TX_SEQ_INIT_SHIFT 6
#define FCOE_TCE_TX_WR_RX_RD_CONST_RSRV2 (0x1<<7)
#define FCOE_TCE_TX_WR_RX_RD_CONST_RSRV2_SHIFT 7
	__le16 rsrv3;
	__le32 verify_tx_seq;
};

/*
 * tce_tx_wr_rx_rd $$KEEP_ENDIANNESS$$
 */
struct fcoe_tce_tx_wr_rx_rd {
	union fcoe_tx_wr_rx_rd_union_ctx union_ctx;
	struct fcoe_tce_tx_wr_rx_rd_const const_ctx;
};

/*
 * tce_rx_wr_tx_rd_const $$KEEP_ENDIANNESS$$
 */
struct fcoe_tce_rx_wr_tx_rd_const {
	__le32 data_2_trns;
	__le32 init_flags;
#define FCOE_TCE_RX_WR_TX_RD_CONST_CID (0xFFFFFF<<0)
#define FCOE_TCE_RX_WR_TX_RD_CONST_CID_SHIFT 0
#define FCOE_TCE_RX_WR_TX_RD_CONST_RSRV0 (0xFF<<24)
#define FCOE_TCE_RX_WR_TX_RD_CONST_RSRV0_SHIFT 24
};

/*
 * tce_rx_wr_tx_rd_var $$KEEP_ENDIANNESS$$
 */
struct fcoe_tce_rx_wr_tx_rd_var {
	__le16 rx_flags;
#define FCOE_TCE_RX_WR_TX_RD_VAR_RSRV1 (0xF<<0)
#define FCOE_TCE_RX_WR_TX_RD_VAR_RSRV1_SHIFT 0
#define FCOE_TCE_RX_WR_TX_RD_VAR_NUM_RQ_WQE (0x7<<4)
#define FCOE_TCE_RX_WR_TX_RD_VAR_NUM_RQ_WQE_SHIFT 4
#define FCOE_TCE_RX_WR_TX_RD_VAR_CONF_REQ (0x1<<7)
#define FCOE_TCE_RX_WR_TX_RD_VAR_CONF_REQ_SHIFT 7
#define FCOE_TCE_RX_WR_TX_RD_VAR_RX_STATE (0xF<<8)
#define FCOE_TCE_RX_WR_TX_RD_VAR_RX_STATE_SHIFT 8
#define FCOE_TCE_RX_WR_TX_RD_VAR_EXP_FIRST_FRAME (0x1<<12)
#define FCOE_TCE_RX_WR_TX_RD_VAR_EXP_FIRST_FRAME_SHIFT 12
#define FCOE_TCE_RX_WR_TX_RD_VAR_RX_SEQ_INIT (0x1<<13)
#define FCOE_TCE_RX_WR_TX_RD_VAR_RX_SEQ_INIT_SHIFT 13
#define FCOE_TCE_RX_WR_TX_RD_VAR_RSRV2 (0x1<<14)
#define FCOE_TCE_RX_WR_TX_RD_VAR_RSRV2_SHIFT 14
#define FCOE_TCE_RX_WR_TX_RD_VAR_RX_VALID (0x1<<15)
#define FCOE_TCE_RX_WR_TX_RD_VAR_RX_VALID_SHIFT 15
	__le16 rx_id;
	struct fcoe_fcp_xfr_rdy_payload fcp_xfr_rdy;
};

/*
 * tce_rx_wr_tx_rd $$KEEP_ENDIANNESS$$
 */
struct fcoe_tce_rx_wr_tx_rd {
	struct fcoe_tce_rx_wr_tx_rd_const const_ctx;
	struct fcoe_tce_rx_wr_tx_rd_var var_ctx;
};

/*
 * tce_rx_only $$KEEP_ENDIANNESS$$
 */
struct fcoe_tce_rx_only {
	struct fcoe_rx_seq_ctx rx_seq_ctx;
	union fcoe_rx_wr_union_ctx union_ctx;
};

/*
 * task_ctx_entry $$KEEP_ENDIANNESS$$
 */
struct fcoe_task_ctx_entry {
	struct fcoe_tce_tx_only txwr_only;
	struct fcoe_tce_tx_wr_rx_rd txwr_rxrd;
	struct fcoe_tce_rx_wr_tx_rd rxwr_txrd;
	struct fcoe_tce_rx_only rxwr_only;
};










/*
 * FCoE XFRQ element $$KEEP_ENDIANNESS$$
 */
struct fcoe_xfrqe {
	__le16 wqe;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define FCOE_XFRQE_TASK_ID (0x7FFF<<0)
#define FCOE_XFRQE_TASK_ID_SHIFT 0
#define FCOE_XFRQE_TOGGLE_BIT (0x1<<15)
#define FCOE_XFRQE_TOGGLE_BIT_SHIFT 15
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE CONFQ element
 */
struct fcoe_confqe {
#if defined(__BIG_ENDIAN)
	u16 rx_id;
	u16 ox_id;
#elif defined(__LITTLE_ENDIAN)
	u16 ox_id;
	u16 rx_id;
#endif
	u32 param;
=======
=======
>>>>>>> refs/remotes/origin/master
 * fcoe rx doorbell message sent to the chip $$KEEP_ENDIANNESS$$
 */
struct b577xx_fcoe_rx_doorbell {
	struct b577xx_doorbell_hdr hdr;
	u8 params;
#define B577XX_FCOE_RX_DOORBELL_NEGATIVE_ARM (0x1F<<0)
#define B577XX_FCOE_RX_DOORBELL_NEGATIVE_ARM_SHIFT 0
#define B577XX_FCOE_RX_DOORBELL_OPCODE (0x7<<5)
#define B577XX_FCOE_RX_DOORBELL_OPCODE_SHIFT 5
	__le16 doorbell_cq_cons;
};


/*
 * FCoE CONFQ element $$KEEP_ENDIANNESS$$
 */
struct fcoe_confqe {
	__le16 ox_id;
	__le16 rx_id;
	__le32 param;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE connection data base
=======
 * FCoE conection data base
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * FCoE conection data base
>>>>>>> refs/remotes/origin/master
 */
struct fcoe_conn_db {
#if defined(__BIG_ENDIAN)
	u16 rsrv0;
	u16 rq_prod;
#elif defined(__LITTLE_ENDIAN)
	u16 rq_prod;
	u16 rsrv0;
#endif
	u32 rsrv1;
	struct regpair cq_arm;
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE CQ element
 */
struct fcoe_cqe {
	u16 wqe;
=======
=======
>>>>>>> refs/remotes/origin/master
 * FCoE CQ element $$KEEP_ENDIANNESS$$
 */
struct fcoe_cqe {
	__le16 wqe;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define FCOE_CQE_CQE_INFO (0x3FFF<<0)
#define FCOE_CQE_CQE_INFO_SHIFT 0
#define FCOE_CQE_CQE_TYPE (0x1<<14)
#define FCOE_CQE_CQE_TYPE_SHIFT 14
#define FCOE_CQE_TOGGLE_BIT (0x1<<15)
#define FCOE_CQE_TOGGLE_BIT_SHIFT 15
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE error/warning resporting entry
 */
struct fcoe_err_report_entry {
	u32 err_warn_bitmap_lo;
	u32 err_warn_bitmap_hi;
	u32 tx_buf_off;
	u32 rx_buf_off;
=======
=======
>>>>>>> refs/remotes/origin/master
 * FCoE error/warning reporting entry $$KEEP_ENDIANNESS$$
 */
struct fcoe_partial_err_report_entry {
	__le32 err_warn_bitmap_lo;
	__le32 err_warn_bitmap_hi;
	__le32 tx_buf_off;
	__le32 rx_buf_off;
};

/*
 * FCoE error/warning reporting entry $$KEEP_ENDIANNESS$$
 */
struct fcoe_err_report_entry {
	struct fcoe_partial_err_report_entry data;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct fcoe_fc_hdr fc_hdr;
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE hash table entry (32 bytes)
 */
struct fcoe_hash_table_entry {
#if defined(__BIG_ENDIAN)
	u8 d_id_0;
	u8 s_id_2;
	u8 s_id_1;
	u8 s_id_0;
#elif defined(__LITTLE_ENDIAN)
=======
 * FCoE hash table entry (32 bytes) $$KEEP_ENDIANNESS$$
 */
struct fcoe_hash_table_entry {
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * FCoE hash table entry (32 bytes) $$KEEP_ENDIANNESS$$
 */
struct fcoe_hash_table_entry {
>>>>>>> refs/remotes/origin/master
	u8 s_id_0;
	u8 s_id_1;
	u8 s_id_2;
	u8 d_id_0;
<<<<<<< HEAD
<<<<<<< HEAD
#endif
#if defined(__BIG_ENDIAN)
	u16 dst_mac_addr_hi;
	u8 d_id_2;
	u8 d_id_1;
#elif defined(__LITTLE_ENDIAN)
	u8 d_id_1;
	u8 d_id_2;
	u16 dst_mac_addr_hi;
#endif
	u32 dst_mac_addr_lo;
#if defined(__BIG_ENDIAN)
	u16 vlan_id;
	u16 src_mac_addr_hi;
#elif defined(__LITTLE_ENDIAN)
	u16 src_mac_addr_hi;
	u16 vlan_id;
#endif
	u32 src_mac_addr_lo;
#if defined(__BIG_ENDIAN)
	u16 reserved1;
	u8 reserved0;
	u8 vlan_flag;
#elif defined(__LITTLE_ENDIAN)
	u8 vlan_flag;
	u8 reserved0;
	u16 reserved1;
#endif
	u32 reserved2;
	u32 field_id;
=======
=======
>>>>>>> refs/remotes/origin/master
	u8 d_id_1;
	u8 d_id_2;
	__le16 dst_mac_addr_hi;
	__le16 dst_mac_addr_mid;
	__le16 dst_mac_addr_lo;
	__le16 src_mac_addr_hi;
	__le16 vlan_id;
	__le16 src_mac_addr_lo;
	__le16 src_mac_addr_mid;
	u8 vlan_flag;
	u8 reserved0;
	__le16 reserved1;
	__le32 reserved2;
	__le32 field_id;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define FCOE_HASH_TABLE_ENTRY_CID (0xFFFFFF<<0)
#define FCOE_HASH_TABLE_ENTRY_CID_SHIFT 0
#define FCOE_HASH_TABLE_ENTRY_RESERVED3 (0x7F<<24)
#define FCOE_HASH_TABLE_ENTRY_RESERVED3_SHIFT 24
#define FCOE_HASH_TABLE_ENTRY_VALID (0x1<<31)
#define FCOE_HASH_TABLE_ENTRY_VALID_SHIFT 31
};

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * FCoE pending work request CQE
 */
struct fcoe_pend_wq_cqe {
	u16 wqe;
=======
=======
>>>>>>> refs/remotes/origin/master

/*
 * FCoE LCQ element $$KEEP_ENDIANNESS$$
 */
struct fcoe_lcqe {
	__le32 wqe;
#define FCOE_LCQE_TASK_ID (0xFFFF<<0)
#define FCOE_LCQE_TASK_ID_SHIFT 0
#define FCOE_LCQE_LCQE_TYPE (0xFF<<16)
#define FCOE_LCQE_LCQE_TYPE_SHIFT 16
#define FCOE_LCQE_RESERVED (0xFF<<24)
#define FCOE_LCQE_RESERVED_SHIFT 24
};



/*
 * FCoE pending work request CQE $$KEEP_ENDIANNESS$$
 */
struct fcoe_pend_wq_cqe {
	__le16 wqe;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define FCOE_PEND_WQ_CQE_TASK_ID (0x3FFF<<0)
#define FCOE_PEND_WQ_CQE_TASK_ID_SHIFT 0
#define FCOE_PEND_WQ_CQE_CQE_TYPE (0x1<<14)
#define FCOE_PEND_WQ_CQE_CQE_TYPE_SHIFT 14
#define FCOE_PEND_WQ_CQE_TOGGLE_BIT (0x1<<15)
#define FCOE_PEND_WQ_CQE_TOGGLE_BIT_SHIFT 15
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE RX statistics parameters section#0
 */
struct fcoe_rx_stat_params_section0 {
	u32 fcoe_ver_cnt;
	u32 fcoe_rx_pkt_cnt;
	u32 fcoe_rx_byte_cnt;
	u32 fcoe_rx_drop_pkt_cnt;
=======
=======
>>>>>>> refs/remotes/origin/master
 * FCoE RX statistics parameters section#0 $$KEEP_ENDIANNESS$$
 */
struct fcoe_rx_stat_params_section0 {
	__le32 fcoe_rx_pkt_cnt;
	__le32 fcoe_rx_byte_cnt;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE RX statistics parameters section#1
 */
struct fcoe_rx_stat_params_section1 {
	u32 fc_crc_cnt;
	u32 eofa_del_cnt;
	u32 miss_frame_cnt;
	u32 seq_timeout_cnt;
	u32 drop_seq_cnt;
	u32 fcoe_rx_drop_pkt_cnt;
	u32 fcp_rx_pkt_cnt;
	u32 reserved0;
=======
=======
>>>>>>> refs/remotes/origin/master
 * FCoE RX statistics parameters section#1 $$KEEP_ENDIANNESS$$
 */
struct fcoe_rx_stat_params_section1 {
	__le32 fcoe_ver_cnt;
	__le32 fcoe_rx_drop_pkt_cnt;
};


/*
 * FCoE RX statistics parameters section#2 $$KEEP_ENDIANNESS$$
 */
struct fcoe_rx_stat_params_section2 {
	__le32 fc_crc_cnt;
	__le32 eofa_del_cnt;
	__le32 miss_frame_cnt;
	__le32 seq_timeout_cnt;
	__le32 drop_seq_cnt;
	__le32 fcoe_rx_drop_pkt_cnt;
	__le32 fcp_rx_pkt_cnt;
	__le32 reserved0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE TX statistics parameters
 */
struct fcoe_tx_stat_params {
	u32 fcoe_tx_pkt_cnt;
	u32 fcoe_tx_byte_cnt;
	u32 fcp_tx_pkt_cnt;
	u32 reserved0;
};

/*
 * FCoE statistics parameters
=======
=======
>>>>>>> refs/remotes/origin/master
 * FCoE TX statistics parameters $$KEEP_ENDIANNESS$$
 */
struct fcoe_tx_stat_params {
	__le32 fcoe_tx_pkt_cnt;
	__le32 fcoe_tx_byte_cnt;
	__le32 fcp_tx_pkt_cnt;
	__le32 reserved0;
};

/*
 * FCoE statistics parameters $$KEEP_ENDIANNESS$$
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */
struct fcoe_statistics_params {
	struct fcoe_tx_stat_params tx_stat;
	struct fcoe_rx_stat_params_section0 rx_stat0;
	struct fcoe_rx_stat_params_section1 rx_stat1;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct fcoe_rx_stat_params_section2 rx_stat2;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct fcoe_rx_stat_params_section2 rx_stat2;
>>>>>>> refs/remotes/origin/master
};


/*
<<<<<<< HEAD
<<<<<<< HEAD
 * FCoE t2 hash table entry (64 bytes)
=======
 * FCoE t2 hash table entry (64 bytes) $$KEEP_ENDIANNESS$$
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * FCoE t2 hash table entry (64 bytes) $$KEEP_ENDIANNESS$$
>>>>>>> refs/remotes/origin/master
 */
struct fcoe_t2_hash_table_entry {
	struct fcoe_hash_table_entry data;
	struct regpair next;
	struct regpair reserved0[3];
};

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * FCoE unsolicited CQE
 */
struct fcoe_unsolicited_cqe {
	u16 wqe;
=======
=======
>>>>>>> refs/remotes/origin/master


/*
 * FCoE unsolicited CQE $$KEEP_ENDIANNESS$$
 */
struct fcoe_unsolicited_cqe {
	__le16 wqe;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define FCOE_UNSOLICITED_CQE_SUBTYPE (0x3<<0)
#define FCOE_UNSOLICITED_CQE_SUBTYPE_SHIFT 0
#define FCOE_UNSOLICITED_CQE_PKT_LEN (0xFFF<<2)
#define FCOE_UNSOLICITED_CQE_PKT_LEN_SHIFT 2
#define FCOE_UNSOLICITED_CQE_CQE_TYPE (0x1<<14)
#define FCOE_UNSOLICITED_CQE_CQE_TYPE_SHIFT 14
#define FCOE_UNSOLICITED_CQE_TOGGLE_BIT (0x1<<15)
#define FCOE_UNSOLICITED_CQE_TOGGLE_BIT_SHIFT 15
};

<<<<<<< HEAD
<<<<<<< HEAD


=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* __57XX_FCOE_HSI_LINUX_LE__ */
