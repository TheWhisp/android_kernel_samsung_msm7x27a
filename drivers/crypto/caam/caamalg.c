/*
 * caam - Freescale FSL CAAM support for crypto API
 *
 * Copyright 2008-2011 Freescale Semiconductor, Inc.
 *
 * Based on talitos crypto API driver.
 *
 * relationship of job descriptors to shared descriptors (SteveC Dec 10 2008):
 *
 * ---------------                     ---------------
 * | JobDesc #1  |-------------------->|  ShareDesc  |
 * | *(packet 1) |                     |   (PDB)     |
 * ---------------      |------------->|  (hashKey)  |
 *       .              |              | (cipherKey) |
 *       .              |    |-------->| (operation) |
 * ---------------      |    |         ---------------
 * | JobDesc #2  |------|    |
 * | *(packet 2) |           |
 * ---------------           |
 *       .                   |
 *       .                   |
 * ---------------           |
 * | JobDesc #3  |------------
 * | *(packet 3) |
 * ---------------
 *
 * The SharedDesc never changes for a connection unless rekeyed, but
 * each packet will likely be in a different place. So all we need
 * to know to process the packet is where the input is, where the
 * output goes, and what context we want to process with. Context is
 * in the SharedDesc, packet references in the JobDesc.
 *
 * So, a job desc looks like:
 *
 * ---------------------
 * | Header            |
 * | ShareDesc Pointer |
 * | SEQ_OUT_PTR       |
 * | (output buffer)   |
<<<<<<< HEAD
 * | SEQ_IN_PTR        |
 * | (input buffer)    |
 * | LOAD (to DECO)    |
=======
 * | (output length)   |
 * | SEQ_IN_PTR        |
 * | (input buffer)    |
 * | (input length)    |
>>>>>>> refs/remotes/origin/master
 * ---------------------
 */

#include "compat.h"

#include "regs.h"
#include "intern.h"
#include "desc_constr.h"
#include "jr.h"
#include "error.h"
<<<<<<< HEAD
=======
#include "sg_sw_sec4.h"
#include "key_gen.h"
>>>>>>> refs/remotes/origin/master

/*
 * crypto alg
 */
#define CAAM_CRA_PRIORITY		3000
/* max key is sum of AES_MAX_KEY_SIZE, max split key size */
#define CAAM_MAX_KEY_SIZE		(AES_MAX_KEY_SIZE + \
					 SHA512_DIGEST_SIZE * 2)
/* max IV is max of AES_BLOCK_SIZE, DES3_EDE_BLOCK_SIZE */
#define CAAM_MAX_IV_LENGTH		16

/* length of descriptors text */
<<<<<<< HEAD
<<<<<<< HEAD
#define DESC_AEAD_SHARED_TEXT_LEN	4
#define DESC_AEAD_ENCRYPT_TEXT_LEN 	21
#define DESC_AEAD_DECRYPT_TEXT_LEN 	24
#define DESC_AEAD_GIVENCRYPT_TEXT_LEN 	27
=======
#define DESC_JOB_IO_LEN			(CAAM_CMD_SZ * 3 + CAAM_PTR_SZ * 3)

=======
>>>>>>> refs/remotes/origin/master
#define DESC_AEAD_BASE			(4 * CAAM_CMD_SZ)
#define DESC_AEAD_ENC_LEN		(DESC_AEAD_BASE + 16 * CAAM_CMD_SZ)
#define DESC_AEAD_DEC_LEN		(DESC_AEAD_BASE + 21 * CAAM_CMD_SZ)
#define DESC_AEAD_GIVENC_LEN		(DESC_AEAD_ENC_LEN + 7 * CAAM_CMD_SZ)

#define DESC_ABLKCIPHER_BASE		(3 * CAAM_CMD_SZ)
#define DESC_ABLKCIPHER_ENC_LEN		(DESC_ABLKCIPHER_BASE + \
					 20 * CAAM_CMD_SZ)
#define DESC_ABLKCIPHER_DEC_LEN		(DESC_ABLKCIPHER_BASE + \
					 15 * CAAM_CMD_SZ)

#define DESC_MAX_USED_BYTES		(DESC_AEAD_GIVENC_LEN + \
					 CAAM_MAX_KEY_SIZE)
#define DESC_MAX_USED_LEN		(DESC_MAX_USED_BYTES / CAAM_CMD_SZ)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef DEBUG
/* for print_hex_dumps with line references */
#define xstr(s) str(s)
#define str(s) #s
=======

#ifdef DEBUG
/* for print_hex_dumps with line references */
>>>>>>> refs/remotes/origin/master
#define debug(format, arg...) printk(format, arg)
#else
#define debug(format, arg...)
#endif
<<<<<<< HEAD

<<<<<<< HEAD
=======
=======
static struct list_head alg_list;

>>>>>>> refs/remotes/origin/master
/* Set DK bit in class 1 operation if shared */
static inline void append_dec_op1(u32 *desc, u32 type)
{
	u32 *jump_cmd, *uncond_jump_cmd;

	jump_cmd = append_jump(desc, JUMP_TEST_ALL | JUMP_COND_SHRD);
	append_operation(desc, type | OP_ALG_AS_INITFINAL |
			 OP_ALG_DECRYPT);
	uncond_jump_cmd = append_jump(desc, JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd);
	append_operation(desc, type | OP_ALG_AS_INITFINAL |
			 OP_ALG_DECRYPT | OP_ALG_AAI_DK);
	set_jump_tgt_here(desc, uncond_jump_cmd);
}

/*
 * Wait for completion of class 1 key loading before allowing
 * error propagation
 */
static inline void append_dec_shr_done(u32 *desc)
{
	u32 *jump_cmd;

	jump_cmd = append_jump(desc, JUMP_CLASS_CLASS1 | JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd);
	append_cmd(desc, SET_OK_NO_PROP_ERRORS | CMD_LOAD);
}

/*
 * For aead functions, read payload and write payload,
 * both of which are specified in req->src and req->dst
 */
static inline void aead_append_src_dst(u32 *desc, u32 msg_type)
{
	append_seq_fifo_load(desc, 0, FIFOLD_CLASS_BOTH |
			     KEY_VLF | msg_type | FIFOLD_TYPE_LASTBOTH);
	append_seq_fifo_store(desc, 0, FIFOST_TYPE_MESSAGE_DATA | KEY_VLF);
}

/*
 * For aead encrypt and decrypt, read iv for both classes
 */
static inline void aead_append_ld_iv(u32 *desc, int ivsize)
{
	append_cmd(desc, CMD_SEQ_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB | ivsize);
	append_move(desc, MOVE_SRC_CLASS1CTX | MOVE_DEST_CLASS2INFIFO | ivsize);
}

/*
 * For ablkcipher encrypt and decrypt, read from req->src and
 * write to req->dst
 */
static inline void ablkcipher_append_src_dst(u32 *desc)
{
<<<<<<< HEAD
	append_math_add(desc, VARSEQOUTLEN, SEQINLEN, REG0, CAAM_CMD_SZ); \
	append_math_add(desc, VARSEQINLEN, SEQINLEN, REG0, CAAM_CMD_SZ); \
	append_seq_fifo_load(desc, 0, FIFOLD_CLASS_CLASS1 | \
			     KEY_VLF | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST1); \
	append_seq_fifo_store(desc, 0, FIFOST_TYPE_MESSAGE_DATA | KEY_VLF); \
=======
	append_math_add(desc, VARSEQOUTLEN, SEQINLEN, REG0, CAAM_CMD_SZ);
	append_math_add(desc, VARSEQINLEN, SEQINLEN, REG0, CAAM_CMD_SZ);
	append_seq_fifo_load(desc, 0, FIFOLD_CLASS_CLASS1 |
			     KEY_VLF | FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST1);
	append_seq_fifo_store(desc, 0, FIFOST_TYPE_MESSAGE_DATA | KEY_VLF);
>>>>>>> refs/remotes/origin/master
}

/*
 * If all data, including src (with assoc and iv) or dst (with iv only) are
 * contiguous
 */
#define GIV_SRC_CONTIG		1
#define GIV_DST_CONTIG		(1 << 1)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * per-session context
 */
struct caam_ctx {
	struct device *jrdev;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 *sh_desc;
	dma_addr_t shared_desc_phys;
	u32 class1_alg_type;
	u32 class2_alg_type;
	u32 alg_op;
	u8 *key;
	dma_addr_t key_phys;
=======
=======
>>>>>>> refs/remotes/origin/master
	u32 sh_desc_enc[DESC_MAX_USED_LEN];
	u32 sh_desc_dec[DESC_MAX_USED_LEN];
	u32 sh_desc_givenc[DESC_MAX_USED_LEN];
	dma_addr_t sh_desc_enc_dma;
	dma_addr_t sh_desc_dec_dma;
	dma_addr_t sh_desc_givenc_dma;
	u32 class1_alg_type;
	u32 class2_alg_type;
	u32 alg_op;
	u8 key[CAAM_MAX_KEY_SIZE];
	dma_addr_t key_dma;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	unsigned int enckeylen;
	unsigned int split_key_len;
	unsigned int split_key_pad_len;
	unsigned int authsize;
};

<<<<<<< HEAD
<<<<<<< HEAD
static int aead_authenc_setauthsize(struct crypto_aead *authenc,
=======
=======
>>>>>>> refs/remotes/origin/master
static void append_key_aead(u32 *desc, struct caam_ctx *ctx,
			    int keys_fit_inline)
{
	if (keys_fit_inline) {
		append_key_as_imm(desc, ctx->key, ctx->split_key_pad_len,
				  ctx->split_key_len, CLASS_2 |
				  KEY_DEST_MDHA_SPLIT | KEY_ENC);
		append_key_as_imm(desc, (void *)ctx->key +
				  ctx->split_key_pad_len, ctx->enckeylen,
				  ctx->enckeylen, CLASS_1 | KEY_DEST_CLASS_REG);
	} else {
		append_key(desc, ctx->key_dma, ctx->split_key_len, CLASS_2 |
			   KEY_DEST_MDHA_SPLIT | KEY_ENC);
		append_key(desc, ctx->key_dma + ctx->split_key_pad_len,
			   ctx->enckeylen, CLASS_1 | KEY_DEST_CLASS_REG);
	}
}

static void init_sh_desc_key_aead(u32 *desc, struct caam_ctx *ctx,
				  int keys_fit_inline)
{
	u32 *key_jump_cmd;

<<<<<<< HEAD
	init_sh_desc(desc, HDR_SHARE_WAIT);
=======
	init_sh_desc(desc, HDR_SHARE_SERIAL);
>>>>>>> refs/remotes/origin/master

	/* Skip if already shared */
	key_jump_cmd = append_jump(desc, JUMP_JSL | JUMP_TEST_ALL |
				   JUMP_COND_SHRD);

	append_key_aead(desc, ctx, keys_fit_inline);

	set_jump_tgt_here(desc, key_jump_cmd);

	/* Propagate errors from shared to job descriptor */
	append_cmd(desc, SET_OK_NO_PROP_ERRORS | CMD_LOAD);
}

static int aead_set_sh_desc(struct crypto_aead *aead)
{
	struct aead_tfm *tfm = &aead->base.crt_aead;
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
<<<<<<< HEAD
	bool keys_fit_inline = 0;
=======
	bool keys_fit_inline = false;
>>>>>>> refs/remotes/origin/master
	u32 *key_jump_cmd, *jump_cmd;
	u32 geniv, moveiv;
	u32 *desc;

	if (!ctx->enckeylen || !ctx->authsize)
		return 0;

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (DESC_AEAD_ENC_LEN + DESC_JOB_IO_LEN +
	    ctx->split_key_pad_len + ctx->enckeylen <=
	    CAAM_DESC_BYTES_MAX)
<<<<<<< HEAD
		keys_fit_inline = 1;
=======
		keys_fit_inline = true;
>>>>>>> refs/remotes/origin/master

	/* aead_encrypt shared descriptor */
	desc = ctx->sh_desc_enc;

	init_sh_desc_key_aead(desc, ctx, keys_fit_inline);

	/* Class 2 operation */
	append_operation(desc, ctx->class2_alg_type |
			 OP_ALG_AS_INITFINAL | OP_ALG_ENCRYPT);

	/* cryptlen = seqoutlen - authsize */
	append_math_sub_imm_u32(desc, REG3, SEQOUTLEN, IMM, ctx->authsize);

	/* assoclen + cryptlen = seqinlen - ivsize */
	append_math_sub_imm_u32(desc, REG2, SEQINLEN, IMM, tfm->ivsize);

	/* assoclen + cryptlen = (assoclen + cryptlen) - cryptlen */
	append_math_sub(desc, VARSEQINLEN, REG2, REG3, CAAM_CMD_SZ);

	/* read assoc before reading payload */
	append_seq_fifo_load(desc, 0, FIFOLD_CLASS_CLASS2 | FIFOLD_TYPE_MSG |
			     KEY_VLF);
	aead_append_ld_iv(desc, tfm->ivsize);

	/* Class 1 operation */
	append_operation(desc, ctx->class1_alg_type |
			 OP_ALG_AS_INITFINAL | OP_ALG_ENCRYPT);

	/* Read and write cryptlen bytes */
	append_math_add(desc, VARSEQINLEN, ZERO, REG3, CAAM_CMD_SZ);
	append_math_add(desc, VARSEQOUTLEN, ZERO, REG3, CAAM_CMD_SZ);
	aead_append_src_dst(desc, FIFOLD_TYPE_MSG1OUT2);

	/* Write ICV */
	append_seq_store(desc, ctx->authsize, LDST_CLASS_2_CCB |
			 LDST_SRCDST_BYTE_CONTEXT);

	ctx->sh_desc_enc_dma = dma_map_single(jrdev, desc,
					      desc_bytes(desc),
					      DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, ctx->sh_desc_enc_dma)) {
		dev_err(jrdev, "unable to map shared descriptor\n");
		return -ENOMEM;
	}
#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "aead enc shdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "aead enc shdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, desc,
		       desc_bytes(desc), 1);
#endif

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (DESC_AEAD_DEC_LEN + DESC_JOB_IO_LEN +
	    ctx->split_key_pad_len + ctx->enckeylen <=
	    CAAM_DESC_BYTES_MAX)
<<<<<<< HEAD
		keys_fit_inline = 1;
=======
		keys_fit_inline = true;
>>>>>>> refs/remotes/origin/master

	desc = ctx->sh_desc_dec;

	/* aead_decrypt shared descriptor */
<<<<<<< HEAD
	init_sh_desc(desc, HDR_SHARE_WAIT);
=======
	init_sh_desc(desc, HDR_SHARE_SERIAL);
>>>>>>> refs/remotes/origin/master

	/* Skip if already shared */
	key_jump_cmd = append_jump(desc, JUMP_JSL | JUMP_TEST_ALL |
				   JUMP_COND_SHRD);

	append_key_aead(desc, ctx, keys_fit_inline);

	/* Only propagate error immediately if shared */
	jump_cmd = append_jump(desc, JUMP_TEST_ALL);
	set_jump_tgt_here(desc, key_jump_cmd);
	append_cmd(desc, SET_OK_NO_PROP_ERRORS | CMD_LOAD);
	set_jump_tgt_here(desc, jump_cmd);

	/* Class 2 operation */
	append_operation(desc, ctx->class2_alg_type |
			 OP_ALG_AS_INITFINAL | OP_ALG_DECRYPT | OP_ALG_ICV_ON);

	/* assoclen + cryptlen = seqinlen - ivsize */
	append_math_sub_imm_u32(desc, REG3, SEQINLEN, IMM,
				ctx->authsize + tfm->ivsize)
	/* assoclen = (assoclen + cryptlen) - cryptlen */
	append_math_sub(desc, REG2, SEQOUTLEN, REG0, CAAM_CMD_SZ);
	append_math_sub(desc, VARSEQINLEN, REG3, REG2, CAAM_CMD_SZ);

	/* read assoc before reading payload */
	append_seq_fifo_load(desc, 0, FIFOLD_CLASS_CLASS2 | FIFOLD_TYPE_MSG |
			     KEY_VLF);

	aead_append_ld_iv(desc, tfm->ivsize);

	append_dec_op1(desc, ctx->class1_alg_type);

	/* Read and write cryptlen bytes */
	append_math_add(desc, VARSEQINLEN, ZERO, REG2, CAAM_CMD_SZ);
	append_math_add(desc, VARSEQOUTLEN, ZERO, REG2, CAAM_CMD_SZ);
	aead_append_src_dst(desc, FIFOLD_TYPE_MSG);

	/* Load ICV */
	append_seq_fifo_load(desc, ctx->authsize, FIFOLD_CLASS_CLASS2 |
			     FIFOLD_TYPE_LAST2 | FIFOLD_TYPE_ICV);
	append_dec_shr_done(desc);

	ctx->sh_desc_dec_dma = dma_map_single(jrdev, desc,
					      desc_bytes(desc),
					      DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, ctx->sh_desc_dec_dma)) {
		dev_err(jrdev, "unable to map shared descriptor\n");
		return -ENOMEM;
	}
#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "aead dec shdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "aead dec shdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, desc,
		       desc_bytes(desc), 1);
#endif

	/*
	 * Job Descriptor and Shared Descriptors
	 * must all fit into the 64-word Descriptor h/w Buffer
	 */
	if (DESC_AEAD_GIVENC_LEN + DESC_JOB_IO_LEN +
	    ctx->split_key_pad_len + ctx->enckeylen <=
	    CAAM_DESC_BYTES_MAX)
<<<<<<< HEAD
		keys_fit_inline = 1;
=======
		keys_fit_inline = true;
>>>>>>> refs/remotes/origin/master

	/* aead_givencrypt shared descriptor */
	desc = ctx->sh_desc_givenc;

	init_sh_desc_key_aead(desc, ctx, keys_fit_inline);

	/* Generate IV */
	geniv = NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_DECO |
		NFIFOENTRY_DTYPE_MSG | NFIFOENTRY_LC1 |
		NFIFOENTRY_PTYPE_RND | (tfm->ivsize << NFIFOENTRY_DLEN_SHIFT);
	append_load_imm_u32(desc, geniv, LDST_CLASS_IND_CCB |
			    LDST_SRCDST_WORD_INFO_FIFO | LDST_IMM);
	append_cmd(desc, CMD_LOAD | DISABLE_AUTO_INFO_FIFO);
	append_move(desc, MOVE_SRC_INFIFO |
		    MOVE_DEST_CLASS1CTX | (tfm->ivsize << MOVE_LEN_SHIFT));
	append_cmd(desc, CMD_LOAD | ENABLE_AUTO_INFO_FIFO);

	/* Copy IV to class 1 context */
	append_move(desc, MOVE_SRC_CLASS1CTX |
		    MOVE_DEST_OUTFIFO | (tfm->ivsize << MOVE_LEN_SHIFT));

	/* Return to encryption */
	append_operation(desc, ctx->class2_alg_type |
			 OP_ALG_AS_INITFINAL | OP_ALG_ENCRYPT);

	/* ivsize + cryptlen = seqoutlen - authsize */
	append_math_sub_imm_u32(desc, REG3, SEQOUTLEN, IMM, ctx->authsize);

	/* assoclen = seqinlen - (ivsize + cryptlen) */
	append_math_sub(desc, VARSEQINLEN, SEQINLEN, REG3, CAAM_CMD_SZ);

	/* read assoc before reading payload */
	append_seq_fifo_load(desc, 0, FIFOLD_CLASS_CLASS2 | FIFOLD_TYPE_MSG |
			     KEY_VLF);

	/* Copy iv from class 1 ctx to class 2 fifo*/
	moveiv = NFIFOENTRY_STYPE_OFIFO | NFIFOENTRY_DEST_CLASS2 |
		 NFIFOENTRY_DTYPE_MSG | (tfm->ivsize << NFIFOENTRY_DLEN_SHIFT);
	append_load_imm_u32(desc, moveiv, LDST_CLASS_IND_CCB |
			    LDST_SRCDST_WORD_INFO_FIFO | LDST_IMM);
	append_load_imm_u32(desc, tfm->ivsize, LDST_CLASS_2_CCB |
			    LDST_SRCDST_WORD_DATASZ_REG | LDST_IMM);

	/* Class 1 operation */
	append_operation(desc, ctx->class1_alg_type |
			 OP_ALG_AS_INITFINAL | OP_ALG_ENCRYPT);

	/* Will write ivsize + cryptlen */
	append_math_add(desc, VARSEQOUTLEN, SEQINLEN, REG0, CAAM_CMD_SZ);

	/* Not need to reload iv */
	append_seq_fifo_load(desc, tfm->ivsize,
			     FIFOLD_CLASS_SKIP);

	/* Will read cryptlen */
	append_math_add(desc, VARSEQINLEN, SEQINLEN, REG0, CAAM_CMD_SZ);
	aead_append_src_dst(desc, FIFOLD_TYPE_MSG1OUT2);

	/* Write ICV */
	append_seq_store(desc, ctx->authsize, LDST_CLASS_2_CCB |
			 LDST_SRCDST_BYTE_CONTEXT);

	ctx->sh_desc_givenc_dma = dma_map_single(jrdev, desc,
						 desc_bytes(desc),
						 DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, ctx->sh_desc_givenc_dma)) {
		dev_err(jrdev, "unable to map shared descriptor\n");
		return -ENOMEM;
	}
#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "aead givenc shdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "aead givenc shdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, desc,
		       desc_bytes(desc), 1);
#endif

	return 0;
}

static int aead_setauthsize(struct crypto_aead *authenc,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				    unsigned int authsize)
{
	struct caam_ctx *ctx = crypto_aead_ctx(authenc);

	ctx->authsize = authsize;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	aead_set_sh_desc(authenc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	aead_set_sh_desc(authenc);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
struct split_key_result {
	struct completion completion;
	int err;
};

static void split_key_done(struct device *dev, u32 *desc, u32 err,
			   void *context)
{
	struct split_key_result *res = context;

#ifdef DEBUG
	dev_err(dev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/cm-10.0
	if (err) {
		char tmp[CAAM_ERROR_STR_MAX];

		dev_err(dev, "%08x: %s\n", err, caam_jr_strstatus(tmp, err));
	}

	res->err = err;

	complete(&res->completion);
}

/*
get a split ipad/opad key

Split key generation-----------------------------------------------

[00] 0xb0810008    jobdesc: stidx=1 share=never len=8
[01] 0x04000014        key: class2->keyreg len=20
			@0xffe01000
[03] 0x84410014  operation: cls2-op sha1 hmac init dec
[04] 0x24940000     fifold: class2 msgdata-last2 len=0 imm
[05] 0xa4000001       jump: class2 local all ->1 [06]
[06] 0x64260028    fifostr: class2 mdsplit-jdk len=40
			@0xffe04000
*/
static u32 gen_split_key(struct caam_ctx *ctx, const u8 *key_in, u32 authkeylen)
{
	struct device *jrdev = ctx->jrdev;
	u32 *desc;
	struct split_key_result result;
	dma_addr_t dma_addr_in, dma_addr_out;
	int ret = 0;

	desc = kmalloc(CAAM_CMD_SZ * 6 + CAAM_PTR_SZ * 2, GFP_KERNEL | GFP_DMA);

	init_job_desc(desc, 0);

	dma_addr_in = dma_map_single(jrdev, (void *)key_in, authkeylen,
				     DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, dma_addr_in)) {
		dev_err(jrdev, "unable to map key input memory\n");
		kfree(desc);
		return -ENOMEM;
	}
	append_key(desc, dma_addr_in, authkeylen, CLASS_2 |
		       KEY_DEST_CLASS_REG);

	/* Sets MDHA up into an HMAC-INIT */
	append_operation(desc, ctx->alg_op | OP_ALG_DECRYPT |
			     OP_ALG_AS_INIT);

	/*
	 * do a FIFO_LOAD of zero, this will trigger the internal key expansion
	   into both pads inside MDHA
	 */
	append_fifo_load_as_imm(desc, NULL, 0, LDST_CLASS_2_CCB |
				FIFOLD_TYPE_MSG | FIFOLD_TYPE_LAST2);

	/*
	 * FIFO_STORE with the explicit split-key content store
	 * (0x26 output type)
	 */
	dma_addr_out = dma_map_single(jrdev, ctx->key, ctx->split_key_pad_len,
				      DMA_FROM_DEVICE);
	if (dma_mapping_error(jrdev, dma_addr_out)) {
		dev_err(jrdev, "unable to map key output memory\n");
		kfree(desc);
		return -ENOMEM;
	}
	append_fifo_store(desc, dma_addr_out, ctx->split_key_len,
			  LDST_CLASS_2_CCB | FIFOST_TYPE_SPLIT_KEK);

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "ctx.key@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, key_in, authkeylen, 1);
	print_hex_dump(KERN_ERR, "jobdesc@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, desc, desc_bytes(desc), 1);
#endif

	result.err = 0;
	init_completion(&result.completion);

	ret = caam_jr_enqueue(jrdev, desc, split_key_done, &result);
	if (!ret) {
		/* in progress */
		wait_for_completion_interruptible(&result.completion);
		ret = result.err;
#ifdef DEBUG
		print_hex_dump(KERN_ERR, "ctx.key@"xstr(__LINE__)": ",
			       DUMP_PREFIX_ADDRESS, 16, 4, ctx->key,
			       ctx->split_key_pad_len, 1);
#endif
	}

	dma_unmap_single(jrdev, dma_addr_out, ctx->split_key_pad_len,
			 DMA_FROM_DEVICE);
	dma_unmap_single(jrdev, dma_addr_in, authkeylen, DMA_TO_DEVICE);

	kfree(desc);

	return ret;
}

<<<<<<< HEAD
static int build_sh_desc_ipsec(struct caam_ctx *ctx)
{
	struct device *jrdev = ctx->jrdev;
	u32 *sh_desc;
	u32 *jump_cmd;
	bool keys_fit_inline = 0;

	/*
	 * largest Job Descriptor and its Shared Descriptor
	 * must both fit into the 64-word Descriptor h/w Buffer
	 */
	if ((DESC_AEAD_GIVENCRYPT_TEXT_LEN +
	     DESC_AEAD_SHARED_TEXT_LEN) * CAAM_CMD_SZ +
	    ctx->split_key_pad_len + ctx->enckeylen <= CAAM_DESC_BYTES_MAX)
		keys_fit_inline = 1;

	/* build shared descriptor for this session */
	sh_desc = kmalloc(CAAM_CMD_SZ * DESC_AEAD_SHARED_TEXT_LEN +
			  (keys_fit_inline ?
			   ctx->split_key_pad_len + ctx->enckeylen :
			   CAAM_PTR_SZ * 2), GFP_DMA | GFP_KERNEL);
	if (!sh_desc) {
		dev_err(jrdev, "could not allocate shared descriptor\n");
		return -ENOMEM;
	}

	init_sh_desc(sh_desc, HDR_SAVECTX | HDR_SHARE_SERIAL);

	jump_cmd = append_jump(sh_desc, CLASS_BOTH | JUMP_TEST_ALL |
			       JUMP_COND_SHRD | JUMP_COND_SELF);

	/*
	 * process keys, starting with class 2/authentication.
	 */
	if (keys_fit_inline) {
		append_key_as_imm(sh_desc, ctx->key, ctx->split_key_pad_len,
				  ctx->split_key_len,
				  CLASS_2 | KEY_DEST_MDHA_SPLIT | KEY_ENC);

		append_key_as_imm(sh_desc, (void *)ctx->key +
				  ctx->split_key_pad_len, ctx->enckeylen,
				  ctx->enckeylen, CLASS_1 | KEY_DEST_CLASS_REG);
	} else {
		append_key(sh_desc, ctx->key_phys, ctx->split_key_len, CLASS_2 |
			   KEY_DEST_MDHA_SPLIT | KEY_ENC);
		append_key(sh_desc, ctx->key_phys + ctx->split_key_pad_len,
			   ctx->enckeylen, CLASS_1 | KEY_DEST_CLASS_REG);
	}

	/* update jump cmd now that we are at the jump target */
	set_jump_tgt_here(sh_desc, jump_cmd);

	ctx->shared_desc_phys = dma_map_single(jrdev, sh_desc,
					       desc_bytes(sh_desc),
					       DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, ctx->shared_desc_phys)) {
		dev_err(jrdev, "unable to map shared descriptor\n");
		kfree(sh_desc);
		return -ENOMEM;
	}

	ctx->sh_desc = sh_desc;

	return 0;
}

static int aead_authenc_setkey(struct crypto_aead *aead,
=======
static int aead_setkey(struct crypto_aead *aead,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static u32 gen_split_aead_key(struct caam_ctx *ctx, const u8 *key_in,
			      u32 authkeylen)
{
	return gen_split_key(ctx->jrdev, ctx->key, ctx->split_key_len,
			       ctx->split_key_pad_len, key_in, authkeylen,
			       ctx->alg_op);
}

static int aead_setkey(struct crypto_aead *aead,
>>>>>>> refs/remotes/origin/master
			       const u8 *key, unsigned int keylen)
{
	/* Sizes for MDHA pads (*not* keys): MD5, SHA1, 224, 256, 384, 512 */
	static const u8 mdpadlen[] = { 16, 20, 32, 32, 64, 64 };
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	struct rtattr *rta = (void *)key;
	struct crypto_authenc_key_param *param;
	unsigned int authkeylen;
	unsigned int enckeylen;
	int ret = 0;

	param = RTA_DATA(rta);
	enckeylen = be32_to_cpu(param->enckeylen);

	key += RTA_ALIGN(rta->rta_len);
	keylen -= RTA_ALIGN(rta->rta_len);

	if (keylen < enckeylen)
		goto badkey;

	authkeylen = keylen - enckeylen;

	if (keylen > CAAM_MAX_KEY_SIZE)
		goto badkey;

	/* Pick class 2 key length from algorithm submask */
	ctx->split_key_len = mdpadlen[(ctx->alg_op & OP_ALG_ALGSEL_SUBMASK) >>
				      OP_ALG_ALGSEL_SHIFT] * 2;
	ctx->split_key_pad_len = ALIGN(ctx->split_key_len, 16);

#ifdef DEBUG
	printk(KERN_ERR "keylen %d enckeylen %d authkeylen %d\n",
	       keylen, enckeylen, authkeylen);
	printk(KERN_ERR "split_key_len %d split_key_pad_len %d\n",
	       ctx->split_key_len, ctx->split_key_pad_len);
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "key in @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
#endif
<<<<<<< HEAD
	ctx->key = kmalloc(ctx->split_key_pad_len + enckeylen,
			   GFP_KERNEL | GFP_DMA);
	if (!ctx->key) {
		dev_err(jrdev, "could not allocate key output memory\n");
		return -ENOMEM;
	}

	ret = gen_split_key(ctx, key, authkeylen);
	if (ret) {
		kfree(ctx->key);
=======

	ret = gen_split_key(ctx, key, authkeylen);
	if (ret) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	print_hex_dump(KERN_ERR, "key in @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
#endif

	ret = gen_split_aead_key(ctx, key, authkeylen);
	if (ret) {
>>>>>>> refs/remotes/origin/master
		goto badkey;
	}

	/* postpend encryption key to auth split key */
	memcpy(ctx->key + ctx->split_key_pad_len, key + authkeylen, enckeylen);

<<<<<<< HEAD
<<<<<<< HEAD
	ctx->key_phys = dma_map_single(jrdev, ctx->key, ctx->split_key_pad_len +
				       enckeylen, DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, ctx->key_phys)) {
		dev_err(jrdev, "unable to map key i/o memory\n");
		kfree(ctx->key);
=======
=======
>>>>>>> refs/remotes/origin/master
	ctx->key_dma = dma_map_single(jrdev, ctx->key, ctx->split_key_pad_len +
				       enckeylen, DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, ctx->key_dma)) {
		dev_err(jrdev, "unable to map key i/o memory\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;
	}
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "ctx.key@"xstr(__LINE__)": ",
=======
		return -ENOMEM;
	}
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "ctx.key@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, ctx->key,
		       ctx->split_key_pad_len + enckeylen, 1);
#endif

	ctx->enckeylen = enckeylen;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = build_sh_desc_ipsec(ctx);
	if (ret) {
		dma_unmap_single(jrdev, ctx->key_phys, ctx->split_key_pad_len +
				 enckeylen, DMA_TO_DEVICE);
		kfree(ctx->key);
=======
=======
>>>>>>> refs/remotes/origin/master
	ret = aead_set_sh_desc(aead);
	if (ret) {
		dma_unmap_single(jrdev, ctx->key_dma, ctx->split_key_pad_len +
				 enckeylen, DMA_TO_DEVICE);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	return ret;
badkey:
	crypto_aead_set_flags(aead, CRYPTO_TFM_RES_BAD_KEY_LEN);
	return -EINVAL;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int ablkcipher_setkey(struct crypto_ablkcipher *ablkcipher,
			     const u8 *key, unsigned int keylen)
{
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct ablkcipher_tfm *tfm = &ablkcipher->base.crt_ablkcipher;
	struct device *jrdev = ctx->jrdev;
	int ret = 0;
	u32 *key_jump_cmd, *jump_cmd;
	u32 *desc;

#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "key in @"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "key in @"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, key, keylen, 1);
#endif

	memcpy(ctx->key, key, keylen);
	ctx->key_dma = dma_map_single(jrdev, ctx->key, keylen,
				      DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, ctx->key_dma)) {
		dev_err(jrdev, "unable to map key i/o memory\n");
		return -ENOMEM;
	}
	ctx->enckeylen = keylen;

	/* ablkcipher_encrypt shared descriptor */
	desc = ctx->sh_desc_enc;
<<<<<<< HEAD
	init_sh_desc(desc, HDR_SHARE_WAIT);
=======
	init_sh_desc(desc, HDR_SHARE_SERIAL);
>>>>>>> refs/remotes/origin/master
	/* Skip if already shared */
	key_jump_cmd = append_jump(desc, JUMP_JSL | JUMP_TEST_ALL |
				   JUMP_COND_SHRD);

	/* Load class1 key only */
	append_key_as_imm(desc, (void *)ctx->key, ctx->enckeylen,
			  ctx->enckeylen, CLASS_1 |
			  KEY_DEST_CLASS_REG);

	set_jump_tgt_here(desc, key_jump_cmd);

	/* Propagate errors from shared to job descriptor */
	append_cmd(desc, SET_OK_NO_PROP_ERRORS | CMD_LOAD);

	/* Load iv */
	append_cmd(desc, CMD_SEQ_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB | tfm->ivsize);

	/* Load operation */
	append_operation(desc, ctx->class1_alg_type |
			 OP_ALG_AS_INITFINAL | OP_ALG_ENCRYPT);

	/* Perform operation */
	ablkcipher_append_src_dst(desc);

	ctx->sh_desc_enc_dma = dma_map_single(jrdev, desc,
					      desc_bytes(desc),
					      DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, ctx->sh_desc_enc_dma)) {
		dev_err(jrdev, "unable to map shared descriptor\n");
		return -ENOMEM;
	}
#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "ablkcipher enc shdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR,
		       "ablkcipher enc shdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, desc,
		       desc_bytes(desc), 1);
#endif
	/* ablkcipher_decrypt shared descriptor */
	desc = ctx->sh_desc_dec;

<<<<<<< HEAD
	init_sh_desc(desc, HDR_SHARE_WAIT);
=======
	init_sh_desc(desc, HDR_SHARE_SERIAL);
>>>>>>> refs/remotes/origin/master
	/* Skip if already shared */
	key_jump_cmd = append_jump(desc, JUMP_JSL | JUMP_TEST_ALL |
				   JUMP_COND_SHRD);

	/* Load class1 key only */
	append_key_as_imm(desc, (void *)ctx->key, ctx->enckeylen,
			  ctx->enckeylen, CLASS_1 |
			  KEY_DEST_CLASS_REG);

	/* For aead, only propagate error immediately if shared */
	jump_cmd = append_jump(desc, JUMP_TEST_ALL);
	set_jump_tgt_here(desc, key_jump_cmd);
	append_cmd(desc, SET_OK_NO_PROP_ERRORS | CMD_LOAD);
	set_jump_tgt_here(desc, jump_cmd);

	/* load IV */
	append_cmd(desc, CMD_SEQ_LOAD | LDST_SRCDST_BYTE_CONTEXT |
		   LDST_CLASS_1_CCB | tfm->ivsize);

	/* Choose operation */
	append_dec_op1(desc, ctx->class1_alg_type);

	/* Perform operation */
	ablkcipher_append_src_dst(desc);

	/* Wait for key to load before allowing propagating error */
	append_dec_shr_done(desc);

	ctx->sh_desc_dec_dma = dma_map_single(jrdev, desc,
					      desc_bytes(desc),
					      DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, ctx->sh_desc_enc_dma)) {
		dev_err(jrdev, "unable to map shared descriptor\n");
		return -ENOMEM;
	}

#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "ablkcipher dec shdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR,
		       "ablkcipher dec shdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, desc,
		       desc_bytes(desc), 1);
#endif

	return ret;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
struct link_tbl_entry {
	u64 ptr;
	u32 len;
	u8 reserved;
	u8 buf_pool_id;
	u16 offset;
};

/*
<<<<<<< HEAD
 * ipsec_esp_edesc - s/w-extended ipsec_esp descriptor
 * @src_nents: number of segments in input scatterlist
 * @dst_nents: number of segments in output scatterlist
 * @assoc_nents: number of segments in associated data (SPI+Seq) scatterlist
=======
 * aead_edesc - s/w-extended aead descriptor
 * @assoc_nents: number of segments in associated data (SPI+Seq) scatterlist
 * @src_nents: number of segments in input scatterlist
 * @dst_nents: number of segments in output scatterlist
 * @iv_dma: dma address of iv for checking continuity and link table
>>>>>>> refs/remotes/origin/cm-10.0
 * @desc: h/w descriptor (variable length; must not exceed MAX_CAAM_DESCSIZE)
 * @link_tbl_bytes: length of dma mapped link_tbl space
 * @link_tbl_dma: bus physical mapped address of h/w link table
 * @hw_desc: the h/w job descriptor followed by any referenced link tables
 */
<<<<<<< HEAD
struct ipsec_esp_edesc {
	int assoc_nents;
	int src_nents;
	int dst_nents;
=======
struct aead_edesc {
	int assoc_nents;
	int src_nents;
	int dst_nents;
	dma_addr_t iv_dma;
>>>>>>> refs/remotes/origin/cm-10.0
	int link_tbl_bytes;
	dma_addr_t link_tbl_dma;
	struct link_tbl_entry *link_tbl;
	u32 hw_desc[0];
};

<<<<<<< HEAD
static void ipsec_esp_unmap(struct device *dev,
			    struct ipsec_esp_edesc *edesc,
			    struct aead_request *areq)
{
	dma_unmap_sg(dev, areq->assoc, edesc->assoc_nents, DMA_TO_DEVICE);

	if (unlikely(areq->dst != areq->src)) {
		dma_unmap_sg(dev, areq->src, edesc->src_nents,
			     DMA_TO_DEVICE);
		dma_unmap_sg(dev, areq->dst, edesc->dst_nents,
			     DMA_FROM_DEVICE);
	} else {
		dma_unmap_sg(dev, areq->src, edesc->src_nents,
			     DMA_BIDIRECTIONAL);
	}

	if (edesc->link_tbl_bytes)
		dma_unmap_single(dev, edesc->link_tbl_dma,
				 edesc->link_tbl_bytes,
				 DMA_TO_DEVICE);
}

/*
 * ipsec_esp descriptor callbacks
 */
static void ipsec_esp_encrypt_done(struct device *jrdev, u32 *desc, u32 err,
				   void *context)
{
	struct aead_request *areq = context;
	struct ipsec_esp_edesc *edesc;
#ifdef DEBUG
	struct crypto_aead *aead = crypto_aead_reqtfm(areq);
	int ivsize = crypto_aead_ivsize(aead);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);

	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif
	edesc = (struct ipsec_esp_edesc *)((char *)desc -
		 offsetof(struct ipsec_esp_edesc, hw_desc));
=======
/*
 * ablkcipher_edesc - s/w-extended ablkcipher descriptor
 * @src_nents: number of segments in input scatterlist
 * @dst_nents: number of segments in output scatterlist
 * @iv_dma: dma address of iv for checking continuity and link table
 * @desc: h/w descriptor (variable length; must not exceed MAX_CAAM_DESCSIZE)
 * @link_tbl_bytes: length of dma mapped link_tbl space
 * @link_tbl_dma: bus physical mapped address of h/w link table
=======
/*
 * aead_edesc - s/w-extended aead descriptor
 * @assoc_nents: number of segments in associated data (SPI+Seq) scatterlist
 * @assoc_chained: if source is chained
 * @src_nents: number of segments in input scatterlist
 * @src_chained: if source is chained
 * @dst_nents: number of segments in output scatterlist
 * @dst_chained: if destination is chained
 * @iv_dma: dma address of iv for checking continuity and link table
 * @desc: h/w descriptor (variable length; must not exceed MAX_CAAM_DESCSIZE)
 * @sec4_sg_bytes: length of dma mapped sec4_sg space
 * @sec4_sg_dma: bus physical mapped address of h/w link table
 * @hw_desc: the h/w job descriptor followed by any referenced link tables
 */
struct aead_edesc {
	int assoc_nents;
	bool assoc_chained;
	int src_nents;
	bool src_chained;
	int dst_nents;
	bool dst_chained;
	dma_addr_t iv_dma;
	int sec4_sg_bytes;
	dma_addr_t sec4_sg_dma;
	struct sec4_sg_entry *sec4_sg;
	u32 hw_desc[0];
};

/*
 * ablkcipher_edesc - s/w-extended ablkcipher descriptor
 * @src_nents: number of segments in input scatterlist
 * @src_chained: if source is chained
 * @dst_nents: number of segments in output scatterlist
 * @dst_chained: if destination is chained
 * @iv_dma: dma address of iv for checking continuity and link table
 * @desc: h/w descriptor (variable length; must not exceed MAX_CAAM_DESCSIZE)
 * @sec4_sg_bytes: length of dma mapped sec4_sg space
 * @sec4_sg_dma: bus physical mapped address of h/w link table
>>>>>>> refs/remotes/origin/master
 * @hw_desc: the h/w job descriptor followed by any referenced link tables
 */
struct ablkcipher_edesc {
	int src_nents;
<<<<<<< HEAD
	int dst_nents;
	dma_addr_t iv_dma;
	int link_tbl_bytes;
	dma_addr_t link_tbl_dma;
	struct link_tbl_entry *link_tbl;
=======
	bool src_chained;
	int dst_nents;
	bool dst_chained;
	dma_addr_t iv_dma;
	int sec4_sg_bytes;
	dma_addr_t sec4_sg_dma;
	struct sec4_sg_entry *sec4_sg;
>>>>>>> refs/remotes/origin/master
	u32 hw_desc[0];
};

static void caam_unmap(struct device *dev, struct scatterlist *src,
<<<<<<< HEAD
		       struct scatterlist *dst, int src_nents, int dst_nents,
		       dma_addr_t iv_dma, int ivsize, dma_addr_t link_tbl_dma,
		       int link_tbl_bytes)
{
	if (unlikely(dst != src)) {
		dma_unmap_sg(dev, src, src_nents, DMA_TO_DEVICE);
		dma_unmap_sg(dev, dst, dst_nents, DMA_FROM_DEVICE);
	} else {
		dma_unmap_sg(dev, src, src_nents, DMA_BIDIRECTIONAL);
=======
		       struct scatterlist *dst, int src_nents,
		       bool src_chained, int dst_nents, bool dst_chained,
		       dma_addr_t iv_dma, int ivsize, dma_addr_t sec4_sg_dma,
		       int sec4_sg_bytes)
{
	if (dst != src) {
		dma_unmap_sg_chained(dev, src, src_nents ? : 1, DMA_TO_DEVICE,
				     src_chained);
		dma_unmap_sg_chained(dev, dst, dst_nents ? : 1, DMA_FROM_DEVICE,
				     dst_chained);
	} else {
		dma_unmap_sg_chained(dev, src, src_nents ? : 1,
				     DMA_BIDIRECTIONAL, src_chained);
>>>>>>> refs/remotes/origin/master
	}

	if (iv_dma)
		dma_unmap_single(dev, iv_dma, ivsize, DMA_TO_DEVICE);
<<<<<<< HEAD
	if (link_tbl_bytes)
		dma_unmap_single(dev, link_tbl_dma, link_tbl_bytes,
=======
	if (sec4_sg_bytes)
		dma_unmap_single(dev, sec4_sg_dma, sec4_sg_bytes,
>>>>>>> refs/remotes/origin/master
				 DMA_TO_DEVICE);
}

static void aead_unmap(struct device *dev,
		       struct aead_edesc *edesc,
		       struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	int ivsize = crypto_aead_ivsize(aead);

<<<<<<< HEAD
	dma_unmap_sg(dev, req->assoc, edesc->assoc_nents, DMA_TO_DEVICE);

	caam_unmap(dev, req->src, req->dst,
		   edesc->src_nents, edesc->dst_nents,
		   edesc->iv_dma, ivsize, edesc->link_tbl_dma,
		   edesc->link_tbl_bytes);
=======
	dma_unmap_sg_chained(dev, req->assoc, edesc->assoc_nents,
			     DMA_TO_DEVICE, edesc->assoc_chained);

	caam_unmap(dev, req->src, req->dst,
		   edesc->src_nents, edesc->src_chained, edesc->dst_nents,
		   edesc->dst_chained, edesc->iv_dma, ivsize,
		   edesc->sec4_sg_dma, edesc->sec4_sg_bytes);
>>>>>>> refs/remotes/origin/master
}

static void ablkcipher_unmap(struct device *dev,
			     struct ablkcipher_edesc *edesc,
			     struct ablkcipher_request *req)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);

	caam_unmap(dev, req->src, req->dst,
<<<<<<< HEAD
		   edesc->src_nents, edesc->dst_nents,
		   edesc->iv_dma, ivsize, edesc->link_tbl_dma,
		   edesc->link_tbl_bytes);
=======
		   edesc->src_nents, edesc->src_chained, edesc->dst_nents,
		   edesc->dst_chained, edesc->iv_dma, ivsize,
		   edesc->sec4_sg_dma, edesc->sec4_sg_bytes);
>>>>>>> refs/remotes/origin/master
}

static void aead_encrypt_done(struct device *jrdev, u32 *desc, u32 err,
				   void *context)
{
	struct aead_request *req = context;
	struct aead_edesc *edesc;
#ifdef DEBUG
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	int ivsize = crypto_aead_ivsize(aead);

	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif

	edesc = (struct aead_edesc *)((char *)desc -
		 offsetof(struct aead_edesc, hw_desc));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (err) {
		char tmp[CAAM_ERROR_STR_MAX];

		dev_err(jrdev, "%08x: %s\n", err, caam_jr_strstatus(tmp, err));
	}

<<<<<<< HEAD
<<<<<<< HEAD
	ipsec_esp_unmap(jrdev, edesc, areq);

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "assoc  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(areq->assoc),
		       areq->assoclen , 1);
	print_hex_dump(KERN_ERR, "dstiv  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(areq->src) - ivsize,
		       edesc->src_nents ? 100 : ivsize, 1);
	print_hex_dump(KERN_ERR, "dst    @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(areq->src),
		       edesc->src_nents ? 100 : areq->cryptlen +
=======
	aead_unmap(jrdev, edesc, req);

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "assoc  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->assoc),
		       req->assoclen , 1);
	print_hex_dump(KERN_ERR, "dstiv  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src) - ivsize,
		       edesc->src_nents ? 100 : ivsize, 1);
	print_hex_dump(KERN_ERR, "dst    @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
		       edesc->src_nents ? 100 : req->cryptlen +
>>>>>>> refs/remotes/origin/cm-10.0
=======
	aead_unmap(jrdev, edesc, req);

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "assoc  @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->assoc),
		       req->assoclen , 1);
	print_hex_dump(KERN_ERR, "dstiv  @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src) - ivsize,
		       edesc->src_nents ? 100 : ivsize, 1);
	print_hex_dump(KERN_ERR, "dst    @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
		       edesc->src_nents ? 100 : req->cryptlen +
>>>>>>> refs/remotes/origin/master
		       ctx->authsize + 4, 1);
#endif

	kfree(edesc);

<<<<<<< HEAD
<<<<<<< HEAD
	aead_request_complete(areq, err);
}

static void ipsec_esp_decrypt_done(struct device *jrdev, u32 *desc, u32 err,
				   void *context)
{
	struct aead_request *areq = context;
	struct ipsec_esp_edesc *edesc;
#ifdef DEBUG
	struct crypto_aead *aead = crypto_aead_reqtfm(areq);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);

	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif
	edesc = (struct ipsec_esp_edesc *)((char *)desc -
		 offsetof(struct ipsec_esp_edesc, hw_desc));
=======
=======
>>>>>>> refs/remotes/origin/master
	aead_request_complete(req, err);
}

static void aead_decrypt_done(struct device *jrdev, u32 *desc, u32 err,
				   void *context)
{
	struct aead_request *req = context;
	struct aead_edesc *edesc;
#ifdef DEBUG
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	int ivsize = crypto_aead_ivsize(aead);

	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif

	edesc = (struct aead_edesc *)((char *)desc -
		 offsetof(struct aead_edesc, hw_desc));

#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "dstiv  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->iv,
		       ivsize, 1);
	print_hex_dump(KERN_ERR, "dst    @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->dst),
		       req->cryptlen, 1);
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
	print_hex_dump(KERN_ERR, "dstiv  @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->iv,
		       ivsize, 1);
	print_hex_dump(KERN_ERR, "dst    @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->dst),
		       req->cryptlen - ctx->authsize, 1);
#endif
>>>>>>> refs/remotes/origin/master

	if (err) {
		char tmp[CAAM_ERROR_STR_MAX];

		dev_err(jrdev, "%08x: %s\n", err, caam_jr_strstatus(tmp, err));
	}

<<<<<<< HEAD
<<<<<<< HEAD
	ipsec_esp_unmap(jrdev, edesc, areq);
=======
	aead_unmap(jrdev, edesc, req);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	aead_unmap(jrdev, edesc, req);
>>>>>>> refs/remotes/origin/master

	/*
	 * verify hw auth check passed else return -EBADMSG
	 */
	if ((err & JRSTA_CCBERR_ERRID_MASK) == JRSTA_CCBERR_ERRID_ICVCHK)
		err = -EBADMSG;

#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "iphdrout@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4,
<<<<<<< HEAD
		       ((char *)sg_virt(areq->assoc) - sizeof(struct iphdr)),
		       sizeof(struct iphdr) + areq->assoclen +
		       ((areq->cryptlen > 1500) ? 1500 : areq->cryptlen) +
		       ctx->authsize + 36, 1);
	if (!err && edesc->link_tbl_bytes) {
		struct scatterlist *sg = sg_last(areq->src, edesc->src_nents);
=======
=======
	print_hex_dump(KERN_ERR, "iphdrout@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4,
>>>>>>> refs/remotes/origin/master
		       ((char *)sg_virt(req->assoc) - sizeof(struct iphdr)),
		       sizeof(struct iphdr) + req->assoclen +
		       ((req->cryptlen > 1500) ? 1500 : req->cryptlen) +
		       ctx->authsize + 36, 1);
<<<<<<< HEAD
	if (!err && edesc->link_tbl_bytes) {
		struct scatterlist *sg = sg_last(req->src, edesc->src_nents);
>>>>>>> refs/remotes/origin/cm-10.0
		print_hex_dump(KERN_ERR, "sglastout@"xstr(__LINE__)": ",
=======
	if (!err && edesc->sec4_sg_bytes) {
		struct scatterlist *sg = sg_last(req->src, edesc->src_nents);
		print_hex_dump(KERN_ERR, "sglastout@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
			       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(sg),
			sg->length + ctx->authsize + 16, 1);
	}
#endif
<<<<<<< HEAD
<<<<<<< HEAD
	kfree(edesc);

	aead_request_complete(areq, err);
=======
=======
>>>>>>> refs/remotes/origin/master

	kfree(edesc);

	aead_request_complete(req, err);
}

static void ablkcipher_encrypt_done(struct device *jrdev, u32 *desc, u32 err,
				   void *context)
{
	struct ablkcipher_request *req = context;
	struct ablkcipher_edesc *edesc;
#ifdef DEBUG
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);

	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif

	edesc = (struct ablkcipher_edesc *)((char *)desc -
		 offsetof(struct ablkcipher_edesc, hw_desc));

	if (err) {
		char tmp[CAAM_ERROR_STR_MAX];

		dev_err(jrdev, "%08x: %s\n", err, caam_jr_strstatus(tmp, err));
	}

#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "dstiv  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       edesc->src_nents > 1 ? 100 : ivsize, 1);
	print_hex_dump(KERN_ERR, "dst    @"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "dstiv  @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       edesc->src_nents > 1 ? 100 : ivsize, 1);
	print_hex_dump(KERN_ERR, "dst    @"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
		       edesc->dst_nents > 1 ? 100 : req->nbytes, 1);
#endif

	ablkcipher_unmap(jrdev, edesc, req);
	kfree(edesc);

	ablkcipher_request_complete(req, err);
}

static void ablkcipher_decrypt_done(struct device *jrdev, u32 *desc, u32 err,
				    void *context)
{
	struct ablkcipher_request *req = context;
	struct ablkcipher_edesc *edesc;
#ifdef DEBUG
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);

	dev_err(jrdev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif

	edesc = (struct ablkcipher_edesc *)((char *)desc -
		 offsetof(struct ablkcipher_edesc, hw_desc));
	if (err) {
		char tmp[CAAM_ERROR_STR_MAX];

		dev_err(jrdev, "%08x: %s\n", err, caam_jr_strstatus(tmp, err));
	}

#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "dstiv  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       ivsize, 1);
	print_hex_dump(KERN_ERR, "dst    @"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "dstiv  @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       ivsize, 1);
	print_hex_dump(KERN_ERR, "dst    @"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
		       edesc->dst_nents > 1 ? 100 : req->nbytes, 1);
#endif

	ablkcipher_unmap(jrdev, edesc, req);
	kfree(edesc);

	ablkcipher_request_complete(req, err);
}

<<<<<<< HEAD
static void sg_to_link_tbl_one(struct link_tbl_entry *link_tbl_ptr,
			       dma_addr_t dma, u32 len, u32 offset)
{
	link_tbl_ptr->ptr = dma;
	link_tbl_ptr->len = len;
	link_tbl_ptr->reserved = 0;
	link_tbl_ptr->buf_pool_id = 0;
	link_tbl_ptr->offset = offset;
#ifdef DEBUG
	print_hex_dump(KERN_ERR, "link_tbl_ptr@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, link_tbl_ptr,
		       sizeof(struct link_tbl_entry), 1);
#endif
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * convert scatterlist to h/w link table format
<<<<<<< HEAD
 * scatterlist must have been previously dma mapped
 */
static void sg_to_link_tbl(struct scatterlist *sg, int sg_count,
			   struct link_tbl_entry *link_tbl_ptr, u32 offset)
{
	while (sg_count) {
		link_tbl_ptr->ptr = sg_dma_address(sg);
		link_tbl_ptr->len = sg_dma_len(sg);
		link_tbl_ptr->reserved = 0;
		link_tbl_ptr->buf_pool_id = 0;
		link_tbl_ptr->offset = offset;
=======
 * but does not have final bit; instead, returns last entry
 */
static struct link_tbl_entry *sg_to_link_tbl(struct scatterlist *sg,
					     int sg_count, struct link_tbl_entry
					     *link_tbl_ptr, u32 offset)
{
	while (sg_count) {
		sg_to_link_tbl_one(link_tbl_ptr, sg_dma_address(sg),
				   sg_dma_len(sg), offset);
>>>>>>> refs/remotes/origin/cm-10.0
		link_tbl_ptr++;
		sg = sg_next(sg);
		sg_count--;
	}
<<<<<<< HEAD

	/* set Final bit (marks end of link table) */
	link_tbl_ptr--;
=======
	return link_tbl_ptr - 1;
}

/*
 * convert scatterlist to h/w link table format
 * scatterlist must have been previously dma mapped
 */
static void sg_to_link_tbl_last(struct scatterlist *sg, int sg_count,
				struct link_tbl_entry *link_tbl_ptr, u32 offset)
{
	link_tbl_ptr = sg_to_link_tbl(sg, sg_count, link_tbl_ptr, offset);
>>>>>>> refs/remotes/origin/cm-10.0
	link_tbl_ptr->len |= 0x40000000;
}

/*
<<<<<<< HEAD
 * fill in and submit ipsec_esp job descriptor
 */
static int ipsec_esp(struct ipsec_esp_edesc *edesc, struct aead_request *areq,
		     u32 encrypt,
		     void (*callback) (struct device *dev, u32 *desc,
				       u32 err, void *context))
{
	struct crypto_aead *aead = crypto_aead_reqtfm(areq);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	u32 *desc = edesc->hw_desc, options;
	int ret, sg_count, assoc_sg_count;
	int ivsize = crypto_aead_ivsize(aead);
	int authsize = ctx->authsize;
	dma_addr_t ptr, dst_dma, src_dma;
#ifdef DEBUG
	u32 *sh_desc = ctx->sh_desc;

	debug("assoclen %d cryptlen %d authsize %d\n",
	      areq->assoclen, areq->cryptlen, authsize);
	print_hex_dump(KERN_ERR, "assoc  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(areq->assoc),
		       areq->assoclen , 1);
	print_hex_dump(KERN_ERR, "presciv@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(areq->src) - ivsize,
		       edesc->src_nents ? 100 : ivsize, 1);
	print_hex_dump(KERN_ERR, "src    @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(areq->src),
			edesc->src_nents ? 100 : areq->cryptlen + authsize, 1);
=======
=======
/*
>>>>>>> refs/remotes/origin/master
 * Fill in aead job descriptor
 */
static void init_aead_job(u32 *sh_desc, dma_addr_t ptr,
			  struct aead_edesc *edesc,
			  struct aead_request *req,
			  bool all_contig, bool encrypt)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	int ivsize = crypto_aead_ivsize(aead);
	int authsize = ctx->authsize;
	u32 *desc = edesc->hw_desc;
	u32 out_options = 0, in_options;
	dma_addr_t dst_dma, src_dma;
<<<<<<< HEAD
	int len, link_tbl_index = 0;
=======
	int len, sec4_sg_index = 0;
>>>>>>> refs/remotes/origin/master

#ifdef DEBUG
	debug("assoclen %d cryptlen %d authsize %d\n",
	      req->assoclen, req->cryptlen, authsize);
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "assoc  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->assoc),
		       req->assoclen , 1);
	print_hex_dump(KERN_ERR, "presciv@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->iv,
		       edesc->src_nents ? 100 : ivsize, 1);
	print_hex_dump(KERN_ERR, "src    @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
			edesc->src_nents ? 100 : req->cryptlen, 1);
>>>>>>> refs/remotes/origin/cm-10.0
	print_hex_dump(KERN_ERR, "shrdesc@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sh_desc,
		       desc_bytes(sh_desc), 1);
#endif
<<<<<<< HEAD
	assoc_sg_count = dma_map_sg(jrdev, areq->assoc, edesc->assoc_nents ?: 1,
				    DMA_TO_DEVICE);
	if (areq->src == areq->dst)
		sg_count = dma_map_sg(jrdev, areq->src, edesc->src_nents ? : 1,
				      DMA_BIDIRECTIONAL);
	else
		sg_count = dma_map_sg(jrdev, areq->src, edesc->src_nents ? : 1,
				      DMA_TO_DEVICE);

	/* start auth operation */
	append_operation(desc, ctx->class2_alg_type | OP_ALG_AS_INITFINAL |
			 (encrypt ? : OP_ALG_ICV_ON));

	/* Load FIFO with data for Class 2 CHA */
	options = FIFOLD_CLASS_CLASS2 | FIFOLD_TYPE_MSG;
	if (!edesc->assoc_nents) {
		ptr = sg_dma_address(areq->assoc);
	} else {
		sg_to_link_tbl(areq->assoc, edesc->assoc_nents,
			       edesc->link_tbl, 0);
		ptr = edesc->link_tbl_dma;
		options |= LDST_SGF;
	}
	append_fifo_load(desc, ptr, areq->assoclen, options);

	/* copy iv from cipher/class1 input context to class2 infifo */
	append_move(desc, MOVE_SRC_CLASS1CTX | MOVE_DEST_CLASS2INFIFO | ivsize);

	if (!encrypt) {
		u32 *jump_cmd, *uncond_jump_cmd;

		/* JUMP if shared */
		jump_cmd = append_jump(desc, JUMP_TEST_ALL | JUMP_COND_SHRD);

		/* start class 1 (cipher) operation, non-shared version */
		append_operation(desc, ctx->class1_alg_type |
				 OP_ALG_AS_INITFINAL);

		uncond_jump_cmd = append_jump(desc, 0);

		set_jump_tgt_here(desc, jump_cmd);

		/* start class 1 (cipher) operation, shared version */
		append_operation(desc, ctx->class1_alg_type |
				 OP_ALG_AS_INITFINAL | OP_ALG_AAI_DK);
		set_jump_tgt_here(desc, uncond_jump_cmd);
	} else
		append_operation(desc, ctx->class1_alg_type |
				 OP_ALG_AS_INITFINAL | encrypt);

	/* load payload & instruct to class2 to snoop class 1 if encrypting */
	options = 0;
	if (!edesc->src_nents) {
		src_dma = sg_dma_address(areq->src);
	} else {
		sg_to_link_tbl(areq->src, edesc->src_nents, edesc->link_tbl +
			       edesc->assoc_nents, 0);
		src_dma = edesc->link_tbl_dma + edesc->assoc_nents *
			  sizeof(struct link_tbl_entry);
		options |= LDST_SGF;
	}
	append_seq_in_ptr(desc, src_dma, areq->cryptlen + authsize, options);
	append_seq_fifo_load(desc, areq->cryptlen, FIFOLD_CLASS_BOTH |
			     FIFOLD_TYPE_LASTBOTH |
			     (encrypt ? FIFOLD_TYPE_MSG1OUT2
				      : FIFOLD_TYPE_MSG));

	/* specify destination */
	if (areq->src == areq->dst) {
		dst_dma = src_dma;
	} else {
		sg_count = dma_map_sg(jrdev, areq->dst, edesc->dst_nents ? : 1,
				      DMA_FROM_DEVICE);
		if (!edesc->dst_nents) {
			dst_dma = sg_dma_address(areq->dst);
			options = 0;
		} else {
			sg_to_link_tbl(areq->dst, edesc->dst_nents,
				       edesc->link_tbl + edesc->assoc_nents +
				       edesc->src_nents, 0);
			dst_dma = edesc->link_tbl_dma + (edesc->assoc_nents +
				  edesc->src_nents) *
				  sizeof(struct link_tbl_entry);
			options = LDST_SGF;
		}
	}
	append_seq_out_ptr(desc, dst_dma, areq->cryptlen + authsize, options);
	append_seq_fifo_store(desc, areq->cryptlen, FIFOST_TYPE_MESSAGE_DATA);

	/* ICV */
	if (encrypt)
		append_seq_store(desc, authsize, LDST_CLASS_2_CCB |
				 LDST_SRCDST_BYTE_CONTEXT);
	else
		append_seq_fifo_load(desc, authsize, FIFOLD_CLASS_CLASS2 |
				     FIFOLD_TYPE_LAST2 | FIFOLD_TYPE_ICV);

#ifdef DEBUG
	debug("job_desc_len %d\n", desc_len(desc));
	print_hex_dump(KERN_ERR, "jobdesc@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, desc, desc_bytes(desc) , 1);
	print_hex_dump(KERN_ERR, "jdlinkt@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->link_tbl,
			edesc->link_tbl_bytes, 1);
#endif

	ret = caam_jr_enqueue(jrdev, desc, callback, areq);
	if (!ret)
		ret = -EINPROGRESS;
	else {
		ipsec_esp_unmap(jrdev, edesc, areq);
=======
=======
	print_hex_dump(KERN_ERR, "assoc  @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->assoc),
		       req->assoclen , 1);
	print_hex_dump(KERN_ERR, "presciv@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->iv,
		       edesc->src_nents ? 100 : ivsize, 1);
	print_hex_dump(KERN_ERR, "src    @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
			edesc->src_nents ? 100 : req->cryptlen, 1);
	print_hex_dump(KERN_ERR, "shrdesc@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sh_desc,
		       desc_bytes(sh_desc), 1);
#endif
>>>>>>> refs/remotes/origin/master

	len = desc_len(sh_desc);
	init_job_desc_shared(desc, ptr, len, HDR_SHARE_DEFER | HDR_REVERSE);

	if (all_contig) {
		src_dma = sg_dma_address(req->assoc);
		in_options = 0;
	} else {
<<<<<<< HEAD
		src_dma = edesc->link_tbl_dma;
		link_tbl_index += (edesc->assoc_nents ? : 1) + 1 +
				  (edesc->src_nents ? : 1);
		in_options = LDST_SGF;
	}
	if (encrypt)
		append_seq_in_ptr(desc, src_dma, req->assoclen + ivsize +
				  req->cryptlen - authsize, in_options);
	else
		append_seq_in_ptr(desc, src_dma, req->assoclen + ivsize +
				  req->cryptlen, in_options);
=======
		src_dma = edesc->sec4_sg_dma;
		sec4_sg_index += (edesc->assoc_nents ? : 1) + 1 +
				 (edesc->src_nents ? : 1);
		in_options = LDST_SGF;
	}

	append_seq_in_ptr(desc, src_dma, req->assoclen + ivsize + req->cryptlen,
			  in_options);
>>>>>>> refs/remotes/origin/master

	if (likely(req->src == req->dst)) {
		if (all_contig) {
			dst_dma = sg_dma_address(req->src);
		} else {
<<<<<<< HEAD
			dst_dma = src_dma + sizeof(struct link_tbl_entry) *
=======
			dst_dma = src_dma + sizeof(struct sec4_sg_entry) *
>>>>>>> refs/remotes/origin/master
				  ((edesc->assoc_nents ? : 1) + 1);
			out_options = LDST_SGF;
		}
	} else {
		if (!edesc->dst_nents) {
			dst_dma = sg_dma_address(req->dst);
		} else {
<<<<<<< HEAD
			dst_dma = edesc->link_tbl_dma +
				  link_tbl_index *
				  sizeof(struct link_tbl_entry);
=======
			dst_dma = edesc->sec4_sg_dma +
				  sec4_sg_index *
				  sizeof(struct sec4_sg_entry);
>>>>>>> refs/remotes/origin/master
			out_options = LDST_SGF;
		}
	}
	if (encrypt)
<<<<<<< HEAD
		append_seq_out_ptr(desc, dst_dma, req->cryptlen, out_options);
=======
		append_seq_out_ptr(desc, dst_dma, req->cryptlen + authsize,
				   out_options);
>>>>>>> refs/remotes/origin/master
	else
		append_seq_out_ptr(desc, dst_dma, req->cryptlen - authsize,
				   out_options);
}

/*
 * Fill in aead givencrypt job descriptor
 */
static void init_aead_giv_job(u32 *sh_desc, dma_addr_t ptr,
			      struct aead_edesc *edesc,
			      struct aead_request *req,
			      int contig)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	int ivsize = crypto_aead_ivsize(aead);
	int authsize = ctx->authsize;
	u32 *desc = edesc->hw_desc;
	u32 out_options = 0, in_options;
	dma_addr_t dst_dma, src_dma;
<<<<<<< HEAD
	int len, link_tbl_index = 0;
=======
	int len, sec4_sg_index = 0;
>>>>>>> refs/remotes/origin/master

#ifdef DEBUG
	debug("assoclen %d cryptlen %d authsize %d\n",
	      req->assoclen, req->cryptlen, authsize);
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "assoc  @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->assoc),
		       req->assoclen , 1);
	print_hex_dump(KERN_ERR, "presciv@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->iv, ivsize, 1);
	print_hex_dump(KERN_ERR, "src    @"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
			edesc->src_nents > 1 ? 100 : req->cryptlen, 1);
	print_hex_dump(KERN_ERR, "shrdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "assoc  @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->assoc),
		       req->assoclen , 1);
	print_hex_dump(KERN_ERR, "presciv@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->iv, ivsize, 1);
	print_hex_dump(KERN_ERR, "src    @"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
			edesc->src_nents > 1 ? 100 : req->cryptlen, 1);
	print_hex_dump(KERN_ERR, "shrdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, sh_desc,
		       desc_bytes(sh_desc), 1);
#endif

	len = desc_len(sh_desc);
	init_job_desc_shared(desc, ptr, len, HDR_SHARE_DEFER | HDR_REVERSE);

	if (contig & GIV_SRC_CONTIG) {
		src_dma = sg_dma_address(req->assoc);
		in_options = 0;
	} else {
<<<<<<< HEAD
		src_dma = edesc->link_tbl_dma;
		link_tbl_index += edesc->assoc_nents + 1 + edesc->src_nents;
		in_options = LDST_SGF;
	}
	append_seq_in_ptr(desc, src_dma, req->assoclen + ivsize +
			  req->cryptlen - authsize, in_options);
=======
		src_dma = edesc->sec4_sg_dma;
		sec4_sg_index += edesc->assoc_nents + 1 + edesc->src_nents;
		in_options = LDST_SGF;
	}
	append_seq_in_ptr(desc, src_dma, req->assoclen + ivsize + req->cryptlen,
			  in_options);
>>>>>>> refs/remotes/origin/master

	if (contig & GIV_DST_CONTIG) {
		dst_dma = edesc->iv_dma;
	} else {
		if (likely(req->src == req->dst)) {
<<<<<<< HEAD
			dst_dma = src_dma + sizeof(struct link_tbl_entry) *
				  edesc->assoc_nents;
			out_options = LDST_SGF;
		} else {
			dst_dma = edesc->link_tbl_dma +
				  link_tbl_index *
				  sizeof(struct link_tbl_entry);
=======
			dst_dma = src_dma + sizeof(struct sec4_sg_entry) *
				  edesc->assoc_nents;
			out_options = LDST_SGF;
		} else {
			dst_dma = edesc->sec4_sg_dma +
				  sec4_sg_index *
				  sizeof(struct sec4_sg_entry);
>>>>>>> refs/remotes/origin/master
			out_options = LDST_SGF;
		}
	}

<<<<<<< HEAD
	append_seq_out_ptr(desc, dst_dma, ivsize + req->cryptlen, out_options);
=======
	append_seq_out_ptr(desc, dst_dma, ivsize + req->cryptlen + authsize,
			   out_options);
>>>>>>> refs/remotes/origin/master
}

/*
 * Fill in ablkcipher job descriptor
 */
static void init_ablkcipher_job(u32 *sh_desc, dma_addr_t ptr,
				struct ablkcipher_edesc *edesc,
				struct ablkcipher_request *req,
				bool iv_contig)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);
	u32 *desc = edesc->hw_desc;
	u32 out_options = 0, in_options;
	dma_addr_t dst_dma, src_dma;
<<<<<<< HEAD
	int len, link_tbl_index = 0;

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "presciv@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       ivsize, 1);
	print_hex_dump(KERN_ERR, "src    @"xstr(__LINE__)": ",
=======
	int len, sec4_sg_index = 0;

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "presciv@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, req->info,
		       ivsize, 1);
	print_hex_dump(KERN_ERR, "src    @"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
		       edesc->src_nents ? 100 : req->nbytes, 1);
#endif

	len = desc_len(sh_desc);
	init_job_desc_shared(desc, ptr, len, HDR_SHARE_DEFER | HDR_REVERSE);

	if (iv_contig) {
		src_dma = edesc->iv_dma;
		in_options = 0;
	} else {
<<<<<<< HEAD
		src_dma = edesc->link_tbl_dma;
		link_tbl_index += (iv_contig ? 0 : 1) + edesc->src_nents;
=======
		src_dma = edesc->sec4_sg_dma;
		sec4_sg_index += (iv_contig ? 0 : 1) + edesc->src_nents;
>>>>>>> refs/remotes/origin/master
		in_options = LDST_SGF;
	}
	append_seq_in_ptr(desc, src_dma, req->nbytes + ivsize, in_options);

	if (likely(req->src == req->dst)) {
		if (!edesc->src_nents && iv_contig) {
			dst_dma = sg_dma_address(req->src);
		} else {
<<<<<<< HEAD
			dst_dma = edesc->link_tbl_dma +
				sizeof(struct link_tbl_entry);
=======
			dst_dma = edesc->sec4_sg_dma +
				sizeof(struct sec4_sg_entry);
>>>>>>> refs/remotes/origin/master
			out_options = LDST_SGF;
		}
	} else {
		if (!edesc->dst_nents) {
			dst_dma = sg_dma_address(req->dst);
		} else {
<<<<<<< HEAD
			dst_dma = edesc->link_tbl_dma +
				link_tbl_index * sizeof(struct link_tbl_entry);
=======
			dst_dma = edesc->sec4_sg_dma +
				sec4_sg_index * sizeof(struct sec4_sg_entry);
>>>>>>> refs/remotes/origin/master
			out_options = LDST_SGF;
		}
	}
	append_seq_out_ptr(desc, dst_dma, req->nbytes, out_options);
}

/*
<<<<<<< HEAD
 * derive number of elements in scatterlist
 */
static int sg_count(struct scatterlist *sg_list, int nbytes)
{
	struct scatterlist *sg = sg_list;
	int sg_nents = 0;

	while (nbytes > 0) {
		sg_nents++;
		nbytes -= sg->length;
		if (!sg_is_last(sg) && (sg + 1)->length == 0)
			BUG(); /* Not support chaining */
		sg = scatterwalk_sg_next(sg);
	}

	if (likely(sg_nents == 1))
		return 0;

	return sg_nents;
}

/*
 * allocate and map the aead extended descriptor
 */
static struct aead_edesc *aead_edesc_alloc(struct aead_request *req,
					   int desc_bytes, bool *all_contig_ptr)
=======
 * allocate and map the aead extended descriptor
 */
static struct aead_edesc *aead_edesc_alloc(struct aead_request *req,
					   int desc_bytes, bool *all_contig_ptr,
					   bool encrypt)
>>>>>>> refs/remotes/origin/master
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	gfp_t flags = (req->base.flags & (CRYPTO_TFM_REQ_MAY_BACKLOG |
		       CRYPTO_TFM_REQ_MAY_SLEEP)) ? GFP_KERNEL : GFP_ATOMIC;
	int assoc_nents, src_nents, dst_nents = 0;
	struct aead_edesc *edesc;
	dma_addr_t iv_dma = 0;
	int sgc;
	bool all_contig = true;
<<<<<<< HEAD
	int ivsize = crypto_aead_ivsize(aead);
	int link_tbl_index, link_tbl_len = 0, link_tbl_bytes;

	assoc_nents = sg_count(req->assoc, req->assoclen);
	src_nents = sg_count(req->src, req->cryptlen);

	if (unlikely(req->dst != req->src))
		dst_nents = sg_count(req->dst, req->cryptlen);

	sgc = dma_map_sg(jrdev, req->assoc, assoc_nents ? : 1,
			 DMA_BIDIRECTIONAL);
	if (likely(req->src == req->dst)) {
		sgc = dma_map_sg(jrdev, req->src, src_nents ? : 1,
				 DMA_BIDIRECTIONAL);
	} else {
		sgc = dma_map_sg(jrdev, req->src, src_nents ? : 1,
				 DMA_TO_DEVICE);
		sgc = dma_map_sg(jrdev, req->dst, dst_nents ? : 1,
				 DMA_FROM_DEVICE);
=======
	bool assoc_chained = false, src_chained = false, dst_chained = false;
	int ivsize = crypto_aead_ivsize(aead);
	int sec4_sg_index, sec4_sg_len = 0, sec4_sg_bytes;
	unsigned int authsize = ctx->authsize;

	assoc_nents = sg_count(req->assoc, req->assoclen, &assoc_chained);

	if (unlikely(req->dst != req->src)) {
		src_nents = sg_count(req->src, req->cryptlen, &src_chained);
		dst_nents = sg_count(req->dst,
				     req->cryptlen +
					(encrypt ? authsize : (-authsize)),
				     &dst_chained);
	} else {
		src_nents = sg_count(req->src,
				     req->cryptlen +
					(encrypt ? authsize : 0),
				     &src_chained);
	}

	sgc = dma_map_sg_chained(jrdev, req->assoc, assoc_nents ? : 1,
				 DMA_TO_DEVICE, assoc_chained);
	if (likely(req->src == req->dst)) {
		sgc = dma_map_sg_chained(jrdev, req->src, src_nents ? : 1,
					 DMA_BIDIRECTIONAL, src_chained);
	} else {
		sgc = dma_map_sg_chained(jrdev, req->src, src_nents ? : 1,
					 DMA_TO_DEVICE, src_chained);
		sgc = dma_map_sg_chained(jrdev, req->dst, dst_nents ? : 1,
					 DMA_FROM_DEVICE, dst_chained);
>>>>>>> refs/remotes/origin/master
	}

	/* Check if data are contiguous */
	iv_dma = dma_map_single(jrdev, req->iv, ivsize, DMA_TO_DEVICE);
	if (assoc_nents || sg_dma_address(req->assoc) + req->assoclen !=
	    iv_dma || src_nents || iv_dma + ivsize !=
	    sg_dma_address(req->src)) {
		all_contig = false;
		assoc_nents = assoc_nents ? : 1;
		src_nents = src_nents ? : 1;
<<<<<<< HEAD
		link_tbl_len = assoc_nents + 1 + src_nents;
	}
	link_tbl_len += dst_nents;

	link_tbl_bytes = link_tbl_len * sizeof(struct link_tbl_entry);

	/* allocate space for base edesc and hw desc commands, link tables */
	edesc = kmalloc(sizeof(struct aead_edesc) + desc_bytes +
			link_tbl_bytes, GFP_DMA | flags);
=======
		sec4_sg_len = assoc_nents + 1 + src_nents;
	}
	sec4_sg_len += dst_nents;

	sec4_sg_bytes = sec4_sg_len * sizeof(struct sec4_sg_entry);

	/* allocate space for base edesc and hw desc commands, link tables */
	edesc = kmalloc(sizeof(struct aead_edesc) + desc_bytes +
			sec4_sg_bytes, GFP_DMA | flags);
>>>>>>> refs/remotes/origin/master
	if (!edesc) {
		dev_err(jrdev, "could not allocate extended descriptor\n");
		return ERR_PTR(-ENOMEM);
	}

	edesc->assoc_nents = assoc_nents;
<<<<<<< HEAD
	edesc->src_nents = src_nents;
	edesc->dst_nents = dst_nents;
	edesc->iv_dma = iv_dma;
	edesc->link_tbl_bytes = link_tbl_bytes;
	edesc->link_tbl = (void *)edesc + sizeof(struct aead_edesc) +
			  desc_bytes;
	edesc->link_tbl_dma = dma_map_single(jrdev, edesc->link_tbl,
					     link_tbl_bytes, DMA_TO_DEVICE);
	*all_contig_ptr = all_contig;

	link_tbl_index = 0;
	if (!all_contig) {
		sg_to_link_tbl(req->assoc,
			       (assoc_nents ? : 1),
			       edesc->link_tbl +
			       link_tbl_index, 0);
		link_tbl_index += assoc_nents ? : 1;
		sg_to_link_tbl_one(edesc->link_tbl + link_tbl_index,
				   iv_dma, ivsize, 0);
		link_tbl_index += 1;
		sg_to_link_tbl_last(req->src,
				    (src_nents ? : 1),
				    edesc->link_tbl +
				    link_tbl_index, 0);
		link_tbl_index += src_nents ? : 1;
	}
	if (dst_nents) {
		sg_to_link_tbl_last(req->dst, dst_nents,
				    edesc->link_tbl + link_tbl_index, 0);
=======
	edesc->assoc_chained = assoc_chained;
	edesc->src_nents = src_nents;
	edesc->src_chained = src_chained;
	edesc->dst_nents = dst_nents;
	edesc->dst_chained = dst_chained;
	edesc->iv_dma = iv_dma;
	edesc->sec4_sg_bytes = sec4_sg_bytes;
	edesc->sec4_sg = (void *)edesc + sizeof(struct aead_edesc) +
			 desc_bytes;
	edesc->sec4_sg_dma = dma_map_single(jrdev, edesc->sec4_sg,
					    sec4_sg_bytes, DMA_TO_DEVICE);
	*all_contig_ptr = all_contig;

	sec4_sg_index = 0;
	if (!all_contig) {
		sg_to_sec4_sg(req->assoc,
			      (assoc_nents ? : 1),
			      edesc->sec4_sg +
			      sec4_sg_index, 0);
		sec4_sg_index += assoc_nents ? : 1;
		dma_to_sec4_sg_one(edesc->sec4_sg + sec4_sg_index,
				   iv_dma, ivsize, 0);
		sec4_sg_index += 1;
		sg_to_sec4_sg_last(req->src,
				   (src_nents ? : 1),
				   edesc->sec4_sg +
				   sec4_sg_index, 0);
		sec4_sg_index += src_nents ? : 1;
	}
	if (dst_nents) {
		sg_to_sec4_sg_last(req->dst, dst_nents,
				   edesc->sec4_sg + sec4_sg_index, 0);
>>>>>>> refs/remotes/origin/master
	}

	return edesc;
}

static int aead_encrypt(struct aead_request *req)
{
	struct aead_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	bool all_contig;
	u32 *desc;
	int ret = 0;

<<<<<<< HEAD
	req->cryptlen += ctx->authsize;

	/* allocate extended descriptor */
	edesc = aead_edesc_alloc(req, DESC_JOB_IO_LEN *
				 CAAM_CMD_SZ, &all_contig);
=======
	/* allocate extended descriptor */
	edesc = aead_edesc_alloc(req, DESC_JOB_IO_LEN *
				 CAAM_CMD_SZ, &all_contig, true);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor */
	init_aead_job(ctx->sh_desc_enc, ctx->sh_desc_enc_dma, edesc, req,
		      all_contig, true);
#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "aead jobdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "aead jobdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif

	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, aead_encrypt_done, req);
	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		aead_unmap(jrdev, edesc, req);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		kfree(edesc);
	}

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * derive number of elements in scatterlist
 */
static int sg_count(struct scatterlist *sg_list, int nbytes, int *chained)
{
	struct scatterlist *sg = sg_list;
	int sg_nents = 0;

	*chained = 0;
	while (nbytes > 0) {
		sg_nents++;
		nbytes -= sg->length;
		if (!sg_is_last(sg) && (sg + 1)->length == 0)
			*chained = 1;
		sg = scatterwalk_sg_next(sg);
	}

	return sg_nents;
}

/*
 * allocate and map the ipsec_esp extended descriptor
 */
static struct ipsec_esp_edesc *ipsec_esp_edesc_alloc(struct aead_request *areq,
						     int desc_bytes)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(areq);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	gfp_t flags = areq->base.flags & CRYPTO_TFM_REQ_MAY_SLEEP ? GFP_KERNEL :
		      GFP_ATOMIC;
	int assoc_nents, src_nents, dst_nents = 0, chained, link_tbl_bytes;
	struct ipsec_esp_edesc *edesc;

	assoc_nents = sg_count(areq->assoc, areq->assoclen, &chained);
	BUG_ON(chained);
	if (likely(assoc_nents == 1))
		assoc_nents = 0;

	src_nents = sg_count(areq->src, areq->cryptlen + ctx->authsize,
			     &chained);
	BUG_ON(chained);
	if (src_nents == 1)
		src_nents = 0;

	if (unlikely(areq->dst != areq->src)) {
		dst_nents = sg_count(areq->dst, areq->cryptlen + ctx->authsize,
				     &chained);
		BUG_ON(chained);
		if (dst_nents == 1)
			dst_nents = 0;
	}

	link_tbl_bytes = (assoc_nents + src_nents + dst_nents) *
			 sizeof(struct link_tbl_entry);
	debug("link_tbl_bytes %d\n", link_tbl_bytes);

	/* allocate space for base edesc and hw desc commands, link tables */
	edesc = kmalloc(sizeof(struct ipsec_esp_edesc) + desc_bytes +
=======
=======
>>>>>>> refs/remotes/origin/master
static int aead_decrypt(struct aead_request *req)
{
	struct aead_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	bool all_contig;
	u32 *desc;
	int ret = 0;

	/* allocate extended descriptor */
	edesc = aead_edesc_alloc(req, DESC_JOB_IO_LEN *
<<<<<<< HEAD
				 CAAM_CMD_SZ, &all_contig);
=======
				 CAAM_CMD_SZ, &all_contig, false);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "dec src@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "dec src@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
		       req->cryptlen, 1);
#endif

	/* Create and submit job descriptor*/
	init_aead_job(ctx->sh_desc_dec,
		      ctx->sh_desc_dec_dma, edesc, req, all_contig, false);
#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "aead jobdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "aead jobdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif

	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, aead_decrypt_done, req);
	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		aead_unmap(jrdev, edesc, req);
		kfree(edesc);
	}

	return ret;
}

/*
 * allocate and map the aead extended descriptor for aead givencrypt
 */
static struct aead_edesc *aead_giv_edesc_alloc(struct aead_givcrypt_request
					       *greq, int desc_bytes,
					       u32 *contig_ptr)
{
	struct aead_request *req = &greq->areq;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	gfp_t flags = (req->base.flags & (CRYPTO_TFM_REQ_MAY_BACKLOG |
		       CRYPTO_TFM_REQ_MAY_SLEEP)) ? GFP_KERNEL : GFP_ATOMIC;
	int assoc_nents, src_nents, dst_nents = 0;
	struct aead_edesc *edesc;
	dma_addr_t iv_dma = 0;
	int sgc;
	u32 contig = GIV_SRC_CONTIG | GIV_DST_CONTIG;
	int ivsize = crypto_aead_ivsize(aead);
<<<<<<< HEAD
	int link_tbl_index, link_tbl_len = 0, link_tbl_bytes;

	assoc_nents = sg_count(req->assoc, req->assoclen);
	src_nents = sg_count(req->src, req->cryptlen);

	if (unlikely(req->dst != req->src))
		dst_nents = sg_count(req->dst, req->cryptlen);

	sgc = dma_map_sg(jrdev, req->assoc, assoc_nents ? : 1,
			 DMA_BIDIRECTIONAL);
	if (likely(req->src == req->dst)) {
		sgc = dma_map_sg(jrdev, req->src, src_nents ? : 1,
				 DMA_BIDIRECTIONAL);
	} else {
		sgc = dma_map_sg(jrdev, req->src, src_nents ? : 1,
				 DMA_TO_DEVICE);
		sgc = dma_map_sg(jrdev, req->dst, dst_nents ? : 1,
				 DMA_FROM_DEVICE);
=======
	bool assoc_chained = false, src_chained = false, dst_chained = false;
	int sec4_sg_index, sec4_sg_len = 0, sec4_sg_bytes;

	assoc_nents = sg_count(req->assoc, req->assoclen, &assoc_chained);
	src_nents = sg_count(req->src, req->cryptlen, &src_chained);

	if (unlikely(req->dst != req->src))
		dst_nents = sg_count(req->dst, req->cryptlen + ctx->authsize,
				     &dst_chained);

	sgc = dma_map_sg_chained(jrdev, req->assoc, assoc_nents ? : 1,
				 DMA_TO_DEVICE, assoc_chained);
	if (likely(req->src == req->dst)) {
		sgc = dma_map_sg_chained(jrdev, req->src, src_nents ? : 1,
					 DMA_BIDIRECTIONAL, src_chained);
	} else {
		sgc = dma_map_sg_chained(jrdev, req->src, src_nents ? : 1,
					 DMA_TO_DEVICE, src_chained);
		sgc = dma_map_sg_chained(jrdev, req->dst, dst_nents ? : 1,
					 DMA_FROM_DEVICE, dst_chained);
>>>>>>> refs/remotes/origin/master
	}

	/* Check if data are contiguous */
	iv_dma = dma_map_single(jrdev, greq->giv, ivsize, DMA_TO_DEVICE);
	if (assoc_nents || sg_dma_address(req->assoc) + req->assoclen !=
	    iv_dma || src_nents || iv_dma + ivsize != sg_dma_address(req->src))
		contig &= ~GIV_SRC_CONTIG;
	if (dst_nents || iv_dma + ivsize != sg_dma_address(req->dst))
		contig &= ~GIV_DST_CONTIG;
<<<<<<< HEAD
		if (unlikely(req->src != req->dst)) {
			dst_nents = dst_nents ? : 1;
			link_tbl_len += 1;
		}
	if (!(contig & GIV_SRC_CONTIG)) {
		assoc_nents = assoc_nents ? : 1;
		src_nents = src_nents ? : 1;
		link_tbl_len += assoc_nents + 1 + src_nents;
		if (likely(req->src == req->dst))
			contig &= ~GIV_DST_CONTIG;
	}
	link_tbl_len += dst_nents;

	link_tbl_bytes = link_tbl_len * sizeof(struct link_tbl_entry);

	/* allocate space for base edesc and hw desc commands, link tables */
	edesc = kmalloc(sizeof(struct aead_edesc) + desc_bytes +
>>>>>>> refs/remotes/origin/cm-10.0
			link_tbl_bytes, GFP_DMA | flags);
=======
	if (unlikely(req->src != req->dst)) {
		dst_nents = dst_nents ? : 1;
		sec4_sg_len += 1;
	}
	if (!(contig & GIV_SRC_CONTIG)) {
		assoc_nents = assoc_nents ? : 1;
		src_nents = src_nents ? : 1;
		sec4_sg_len += assoc_nents + 1 + src_nents;
		if (likely(req->src == req->dst))
			contig &= ~GIV_DST_CONTIG;
	}
	sec4_sg_len += dst_nents;

	sec4_sg_bytes = sec4_sg_len * sizeof(struct sec4_sg_entry);

	/* allocate space for base edesc and hw desc commands, link tables */
	edesc = kmalloc(sizeof(struct aead_edesc) + desc_bytes +
			sec4_sg_bytes, GFP_DMA | flags);
>>>>>>> refs/remotes/origin/master
	if (!edesc) {
		dev_err(jrdev, "could not allocate extended descriptor\n");
		return ERR_PTR(-ENOMEM);
	}

	edesc->assoc_nents = assoc_nents;
<<<<<<< HEAD
	edesc->src_nents = src_nents;
	edesc->dst_nents = dst_nents;
<<<<<<< HEAD
	edesc->link_tbl = (void *)edesc + sizeof(struct ipsec_esp_edesc) +
			  desc_bytes;
	edesc->link_tbl_dma = dma_map_single(jrdev, edesc->link_tbl,
					     link_tbl_bytes, DMA_TO_DEVICE);
	edesc->link_tbl_bytes = link_tbl_bytes;
=======
	edesc->iv_dma = iv_dma;
	edesc->link_tbl_bytes = link_tbl_bytes;
	edesc->link_tbl = (void *)edesc + sizeof(struct aead_edesc) +
			  desc_bytes;
	edesc->link_tbl_dma = dma_map_single(jrdev, edesc->link_tbl,
					     link_tbl_bytes, DMA_TO_DEVICE);
	*contig_ptr = contig;

	link_tbl_index = 0;
	if (!(contig & GIV_SRC_CONTIG)) {
		sg_to_link_tbl(req->assoc, assoc_nents,
			       edesc->link_tbl +
			       link_tbl_index, 0);
		link_tbl_index += assoc_nents;
		sg_to_link_tbl_one(edesc->link_tbl + link_tbl_index,
				   iv_dma, ivsize, 0);
		link_tbl_index += 1;
		sg_to_link_tbl_last(req->src, src_nents,
				    edesc->link_tbl +
				    link_tbl_index, 0);
		link_tbl_index += src_nents;
	}
	if (unlikely(req->src != req->dst && !(contig & GIV_DST_CONTIG))) {
		sg_to_link_tbl_one(edesc->link_tbl + link_tbl_index,
				   iv_dma, ivsize, 0);
		link_tbl_index += 1;
		sg_to_link_tbl_last(req->dst, dst_nents,
				    edesc->link_tbl + link_tbl_index, 0);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	edesc->assoc_chained = assoc_chained;
	edesc->src_nents = src_nents;
	edesc->src_chained = src_chained;
	edesc->dst_nents = dst_nents;
	edesc->dst_chained = dst_chained;
	edesc->iv_dma = iv_dma;
	edesc->sec4_sg_bytes = sec4_sg_bytes;
	edesc->sec4_sg = (void *)edesc + sizeof(struct aead_edesc) +
			 desc_bytes;
	edesc->sec4_sg_dma = dma_map_single(jrdev, edesc->sec4_sg,
					    sec4_sg_bytes, DMA_TO_DEVICE);
	*contig_ptr = contig;

	sec4_sg_index = 0;
	if (!(contig & GIV_SRC_CONTIG)) {
		sg_to_sec4_sg(req->assoc, assoc_nents,
			      edesc->sec4_sg +
			      sec4_sg_index, 0);
		sec4_sg_index += assoc_nents;
		dma_to_sec4_sg_one(edesc->sec4_sg + sec4_sg_index,
				   iv_dma, ivsize, 0);
		sec4_sg_index += 1;
		sg_to_sec4_sg_last(req->src, src_nents,
				   edesc->sec4_sg +
				   sec4_sg_index, 0);
		sec4_sg_index += src_nents;
	}
	if (unlikely(req->src != req->dst && !(contig & GIV_DST_CONTIG))) {
		dma_to_sec4_sg_one(edesc->sec4_sg + sec4_sg_index,
				   iv_dma, ivsize, 0);
		sec4_sg_index += 1;
		sg_to_sec4_sg_last(req->dst, dst_nents,
				   edesc->sec4_sg + sec4_sg_index, 0);
	}
>>>>>>> refs/remotes/origin/master

	return edesc;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int aead_authenc_encrypt(struct aead_request *areq)
{
	struct ipsec_esp_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(areq);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	int ivsize = crypto_aead_ivsize(aead);
	u32 *desc;
	dma_addr_t iv_dma;

	/* allocate extended descriptor */
	edesc = ipsec_esp_edesc_alloc(areq, DESC_AEAD_ENCRYPT_TEXT_LEN *
				      CAAM_CMD_SZ);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	desc = edesc->hw_desc;

	/* insert shared descriptor pointer */
	init_job_desc_shared(desc, ctx->shared_desc_phys,
			     desc_len(ctx->sh_desc), HDR_SHARE_DEFER);

	iv_dma = dma_map_single(jrdev, areq->iv, ivsize, DMA_TO_DEVICE);
	/* check dma error */

	append_load(desc, iv_dma, ivsize,
		    LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);

	return ipsec_esp(edesc, areq, OP_ALG_ENCRYPT, ipsec_esp_encrypt_done);
}

static int aead_authenc_decrypt(struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	int ivsize = crypto_aead_ivsize(aead);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	struct ipsec_esp_edesc *edesc;
	u32 *desc;
	dma_addr_t iv_dma;

	req->cryptlen -= ctx->authsize;

	/* allocate extended descriptor */
	edesc = ipsec_esp_edesc_alloc(req, DESC_AEAD_DECRYPT_TEXT_LEN *
				      CAAM_CMD_SZ);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	desc = edesc->hw_desc;

	/* insert shared descriptor pointer */
	init_job_desc_shared(desc, ctx->shared_desc_phys,
			     desc_len(ctx->sh_desc), HDR_SHARE_DEFER);

	iv_dma = dma_map_single(jrdev, req->iv, ivsize, DMA_TO_DEVICE);
	/* check dma error */

	append_load(desc, iv_dma, ivsize,
		    LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);

	return ipsec_esp(edesc, req, !OP_ALG_ENCRYPT, ipsec_esp_decrypt_done);
}

static int aead_authenc_givencrypt(struct aead_givcrypt_request *req)
{
	struct aead_request *areq = &req->areq;
	struct ipsec_esp_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(areq);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	int ivsize = crypto_aead_ivsize(aead);
	dma_addr_t iv_dma;
	u32 *desc;

	iv_dma = dma_map_single(jrdev, req->giv, ivsize, DMA_FROM_DEVICE);

	debug("%s: giv %p\n", __func__, req->giv);

	/* allocate extended descriptor */
	edesc = ipsec_esp_edesc_alloc(areq, DESC_AEAD_GIVENCRYPT_TEXT_LEN *
				      CAAM_CMD_SZ);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	desc = edesc->hw_desc;

	/* insert shared descriptor pointer */
	init_job_desc_shared(desc, ctx->shared_desc_phys,
			     desc_len(ctx->sh_desc), HDR_SHARE_DEFER);

	/*
	 * LOAD IMM Info FIFO
	 * to DECO, Last, Padding, Random, Message, 16 bytes
	 */
	append_load_imm_u32(desc, NFIFOENTRY_DEST_DECO | NFIFOENTRY_LC1 |
			    NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DTYPE_MSG |
			    NFIFOENTRY_PTYPE_RND | ivsize,
			    LDST_SRCDST_WORD_INFO_FIFO);

	/*
	 * disable info fifo entries since the above serves as the entry
	 * this way, the MOVE command won't generate an entry.
	 * Note that this isn't required in more recent versions of
	 * SEC as a MOVE that doesn't do info FIFO entries is available.
	 */
	append_cmd(desc, CMD_LOAD | DISABLE_AUTO_INFO_FIFO);

	/* MOVE DECO Alignment -> C1 Context 16 bytes */
	append_move(desc, MOVE_SRC_INFIFO | MOVE_DEST_CLASS1CTX | ivsize);

	/* re-enable info fifo entries */
	append_cmd(desc, CMD_LOAD | ENABLE_AUTO_INFO_FIFO);

	/* MOVE C1 Context -> OFIFO 16 bytes */
	append_move(desc, MOVE_SRC_CLASS1CTX | MOVE_DEST_OUTFIFO | ivsize);

	append_fifo_store(desc, iv_dma, ivsize, FIFOST_TYPE_MESSAGE_DATA);

	return ipsec_esp(edesc, areq, OP_ALG_ENCRYPT, ipsec_esp_encrypt_done);
}

=======
=======
>>>>>>> refs/remotes/origin/master
static int aead_givencrypt(struct aead_givcrypt_request *areq)
{
	struct aead_request *req = &areq->areq;
	struct aead_edesc *edesc;
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct caam_ctx *ctx = crypto_aead_ctx(aead);
	struct device *jrdev = ctx->jrdev;
	u32 contig;
	u32 *desc;
	int ret = 0;

<<<<<<< HEAD
	req->cryptlen += ctx->authsize;

=======
>>>>>>> refs/remotes/origin/master
	/* allocate extended descriptor */
	edesc = aead_giv_edesc_alloc(areq, DESC_JOB_IO_LEN *
				     CAAM_CMD_SZ, &contig);

	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "giv src@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "giv src@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, sg_virt(req->src),
		       req->cryptlen, 1);
#endif

	/* Create and submit job descriptor*/
	init_aead_giv_job(ctx->sh_desc_givenc,
			  ctx->sh_desc_givenc_dma, edesc, req, contig);
#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "aead jobdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "aead jobdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif

	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, aead_encrypt_done, req);
	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		aead_unmap(jrdev, edesc, req);
		kfree(edesc);
	}

	return ret;
}

/*
 * allocate and map the ablkcipher extended descriptor for ablkcipher
 */
static struct ablkcipher_edesc *ablkcipher_edesc_alloc(struct ablkcipher_request
						       *req, int desc_bytes,
						       bool *iv_contig_out)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct device *jrdev = ctx->jrdev;
	gfp_t flags = (req->base.flags & (CRYPTO_TFM_REQ_MAY_BACKLOG |
					  CRYPTO_TFM_REQ_MAY_SLEEP)) ?
		       GFP_KERNEL : GFP_ATOMIC;
<<<<<<< HEAD
	int src_nents, dst_nents = 0, link_tbl_bytes;
=======
	int src_nents, dst_nents = 0, sec4_sg_bytes;
>>>>>>> refs/remotes/origin/master
	struct ablkcipher_edesc *edesc;
	dma_addr_t iv_dma = 0;
	bool iv_contig = false;
	int sgc;
	int ivsize = crypto_ablkcipher_ivsize(ablkcipher);
<<<<<<< HEAD
	int link_tbl_index;

	src_nents = sg_count(req->src, req->nbytes);

	if (unlikely(req->dst != req->src))
		dst_nents = sg_count(req->dst, req->nbytes);

	if (likely(req->src == req->dst)) {
		sgc = dma_map_sg(jrdev, req->src, src_nents ? : 1,
				 DMA_BIDIRECTIONAL);
	} else {
		sgc = dma_map_sg(jrdev, req->src, src_nents ? : 1,
				 DMA_TO_DEVICE);
		sgc = dma_map_sg(jrdev, req->dst, dst_nents ? : 1,
				 DMA_FROM_DEVICE);
=======
	bool src_chained = false, dst_chained = false;
	int sec4_sg_index;

	src_nents = sg_count(req->src, req->nbytes, &src_chained);

	if (req->dst != req->src)
		dst_nents = sg_count(req->dst, req->nbytes, &dst_chained);

	if (likely(req->src == req->dst)) {
		sgc = dma_map_sg_chained(jrdev, req->src, src_nents ? : 1,
					 DMA_BIDIRECTIONAL, src_chained);
	} else {
		sgc = dma_map_sg_chained(jrdev, req->src, src_nents ? : 1,
					 DMA_TO_DEVICE, src_chained);
		sgc = dma_map_sg_chained(jrdev, req->dst, dst_nents ? : 1,
					 DMA_FROM_DEVICE, dst_chained);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Check if iv can be contiguous with source and destination.
	 * If so, include it. If not, create scatterlist.
	 */
	iv_dma = dma_map_single(jrdev, req->info, ivsize, DMA_TO_DEVICE);
	if (!src_nents && iv_dma + ivsize == sg_dma_address(req->src))
		iv_contig = true;
	else
		src_nents = src_nents ? : 1;
<<<<<<< HEAD
	link_tbl_bytes = ((iv_contig ? 0 : 1) + src_nents + dst_nents) *
			 sizeof(struct link_tbl_entry);

	/* allocate space for base edesc and hw desc commands, link tables */
	edesc = kmalloc(sizeof(struct ablkcipher_edesc) + desc_bytes +
			link_tbl_bytes, GFP_DMA | flags);
=======
	sec4_sg_bytes = ((iv_contig ? 0 : 1) + src_nents + dst_nents) *
			sizeof(struct sec4_sg_entry);

	/* allocate space for base edesc and hw desc commands, link tables */
	edesc = kmalloc(sizeof(struct ablkcipher_edesc) + desc_bytes +
			sec4_sg_bytes, GFP_DMA | flags);
>>>>>>> refs/remotes/origin/master
	if (!edesc) {
		dev_err(jrdev, "could not allocate extended descriptor\n");
		return ERR_PTR(-ENOMEM);
	}

	edesc->src_nents = src_nents;
<<<<<<< HEAD
	edesc->dst_nents = dst_nents;
	edesc->link_tbl_bytes = link_tbl_bytes;
	edesc->link_tbl = (void *)edesc + sizeof(struct ablkcipher_edesc) +
			  desc_bytes;

	link_tbl_index = 0;
	if (!iv_contig) {
		sg_to_link_tbl_one(edesc->link_tbl, iv_dma, ivsize, 0);
		sg_to_link_tbl_last(req->src, src_nents,
				    edesc->link_tbl + 1, 0);
		link_tbl_index += 1 + src_nents;
	}

	if (unlikely(dst_nents)) {
		sg_to_link_tbl_last(req->dst, dst_nents,
			edesc->link_tbl + link_tbl_index, 0);
	}

	edesc->link_tbl_dma = dma_map_single(jrdev, edesc->link_tbl,
					     link_tbl_bytes, DMA_TO_DEVICE);
	edesc->iv_dma = iv_dma;

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "ablkcipher link_tbl@"xstr(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->link_tbl,
		       link_tbl_bytes, 1);
=======
	edesc->src_chained = src_chained;
	edesc->dst_nents = dst_nents;
	edesc->dst_chained = dst_chained;
	edesc->sec4_sg_bytes = sec4_sg_bytes;
	edesc->sec4_sg = (void *)edesc + sizeof(struct ablkcipher_edesc) +
			 desc_bytes;

	sec4_sg_index = 0;
	if (!iv_contig) {
		dma_to_sec4_sg_one(edesc->sec4_sg, iv_dma, ivsize, 0);
		sg_to_sec4_sg_last(req->src, src_nents,
				   edesc->sec4_sg + 1, 0);
		sec4_sg_index += 1 + src_nents;
	}

	if (dst_nents) {
		sg_to_sec4_sg_last(req->dst, dst_nents,
			edesc->sec4_sg + sec4_sg_index, 0);
	}

	edesc->sec4_sg_dma = dma_map_single(jrdev, edesc->sec4_sg,
					    sec4_sg_bytes, DMA_TO_DEVICE);
	edesc->iv_dma = iv_dma;

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "ablkcipher sec4_sg@"__stringify(__LINE__)": ",
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->sec4_sg,
		       sec4_sg_bytes, 1);
>>>>>>> refs/remotes/origin/master
#endif

	*iv_contig_out = iv_contig;
	return edesc;
}

static int ablkcipher_encrypt(struct ablkcipher_request *req)
{
	struct ablkcipher_edesc *edesc;
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct device *jrdev = ctx->jrdev;
	bool iv_contig;
	u32 *desc;
	int ret = 0;

	/* allocate extended descriptor */
	edesc = ablkcipher_edesc_alloc(req, DESC_JOB_IO_LEN *
				       CAAM_CMD_SZ, &iv_contig);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor*/
	init_ablkcipher_job(ctx->sh_desc_enc,
		ctx->sh_desc_enc_dma, edesc, req, iv_contig);
#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "ablkcipher jobdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "ablkcipher jobdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif
	desc = edesc->hw_desc;
	ret = caam_jr_enqueue(jrdev, desc, ablkcipher_encrypt_done, req);

	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		ablkcipher_unmap(jrdev, edesc, req);
		kfree(edesc);
	}

	return ret;
}

static int ablkcipher_decrypt(struct ablkcipher_request *req)
{
	struct ablkcipher_edesc *edesc;
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct caam_ctx *ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct device *jrdev = ctx->jrdev;
	bool iv_contig;
	u32 *desc;
	int ret = 0;

	/* allocate extended descriptor */
	edesc = ablkcipher_edesc_alloc(req, DESC_JOB_IO_LEN *
				       CAAM_CMD_SZ, &iv_contig);
	if (IS_ERR(edesc))
		return PTR_ERR(edesc);

	/* Create and submit job descriptor*/
	init_ablkcipher_job(ctx->sh_desc_dec,
		ctx->sh_desc_dec_dma, edesc, req, iv_contig);
	desc = edesc->hw_desc;
#ifdef DEBUG
<<<<<<< HEAD
	print_hex_dump(KERN_ERR, "ablkcipher jobdesc@"xstr(__LINE__)": ",
=======
	print_hex_dump(KERN_ERR, "ablkcipher jobdesc@"__stringify(__LINE__)": ",
>>>>>>> refs/remotes/origin/master
		       DUMP_PREFIX_ADDRESS, 16, 4, edesc->hw_desc,
		       desc_bytes(edesc->hw_desc), 1);
#endif

	ret = caam_jr_enqueue(jrdev, desc, ablkcipher_decrypt_done, req);
	if (!ret) {
		ret = -EINPROGRESS;
	} else {
		ablkcipher_unmap(jrdev, edesc, req);
		kfree(edesc);
	}

	return ret;
}

#define template_aead		template_u.aead
#define template_ablkcipher	template_u.ablkcipher
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct caam_alg_template {
	char name[CRYPTO_MAX_ALG_NAME];
	char driver_name[CRYPTO_MAX_ALG_NAME];
	unsigned int blocksize;
<<<<<<< HEAD
<<<<<<< HEAD
	struct aead_alg aead;
=======
=======
>>>>>>> refs/remotes/origin/master
	u32 type;
	union {
		struct ablkcipher_alg ablkcipher;
		struct aead_alg aead;
		struct blkcipher_alg blkcipher;
		struct cipher_alg cipher;
		struct compress_alg compress;
		struct rng_alg rng;
	} template_u;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u32 class1_alg_type;
	u32 class2_alg_type;
	u32 alg_op;
};

static struct caam_alg_template driver_algs[] = {
	/* single-pass ipsec_esp descriptor */
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "authenc(hmac(sha1),cbc(aes))",
		.driver_name = "authenc-hmac-sha1-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.aead = {
			.setkey = aead_authenc_setkey,
			.setauthsize = aead_authenc_setauthsize,
			.encrypt = aead_authenc_encrypt,
			.decrypt = aead_authenc_decrypt,
			.givencrypt = aead_authenc_givencrypt,
=======
=======
>>>>>>> refs/remotes/origin/master
		.name = "authenc(hmac(md5),cbc(aes))",
		.driver_name = "authenc-hmac-md5-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
			.geniv = "<built-in>",
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_MD5 | OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_MD5 | OP_ALG_AAI_HMAC,
	},
	{
		.name = "authenc(hmac(sha1),cbc(aes))",
		.driver_name = "authenc-hmac-sha1-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			.geniv = "<built-in>",
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA1 | OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA1 | OP_ALG_AAI_HMAC,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "authenc(hmac(sha256),cbc(aes))",
		.driver_name = "authenc-hmac-sha256-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.aead = {
			.setkey = aead_authenc_setkey,
			.setauthsize = aead_authenc_setauthsize,
			.encrypt = aead_authenc_encrypt,
			.decrypt = aead_authenc_decrypt,
			.givencrypt = aead_authenc_givencrypt,
=======
		.name = "authenc(hmac(sha224),cbc(aes))",
		.driver_name = "authenc-hmac-sha224-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
=======
		.name = "authenc(hmac(sha224),cbc(aes))",
		.driver_name = "authenc-hmac-sha224-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
>>>>>>> refs/remotes/origin/master
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
			.geniv = "<built-in>",
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA224 | OP_ALG_AAI_HMAC,
	},
	{
		.name = "authenc(hmac(sha256),cbc(aes))",
		.driver_name = "authenc-hmac-sha256-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			.geniv = "<built-in>",
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA256 | OP_ALG_AAI_HMAC,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "authenc(hmac(sha512),cbc(aes))",
		.driver_name = "authenc-hmac-sha512-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.aead = {
			.setkey = aead_authenc_setkey,
			.setauthsize = aead_authenc_setauthsize,
			.encrypt = aead_authenc_encrypt,
			.decrypt = aead_authenc_decrypt,
			.givencrypt = aead_authenc_givencrypt,
=======
		.name = "authenc(hmac(sha384),cbc(aes))",
		.driver_name = "authenc-hmac-sha384-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
=======
		.name = "authenc(hmac(sha384),cbc(aes))",
		.driver_name = "authenc-hmac-sha384-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
>>>>>>> refs/remotes/origin/master
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
			.geniv = "<built-in>",
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA384 | OP_ALG_AAI_HMAC,
	},

	{
		.name = "authenc(hmac(sha512),cbc(aes))",
		.driver_name = "authenc-hmac-sha512-cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			.geniv = "<built-in>",
			.ivsize = AES_BLOCK_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA512 | OP_ALG_AAI_HMAC,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "authenc(hmac(sha1),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha1-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.aead = {
			.setkey = aead_authenc_setkey,
			.setauthsize = aead_authenc_setauthsize,
			.encrypt = aead_authenc_encrypt,
			.decrypt = aead_authenc_decrypt,
			.givencrypt = aead_authenc_givencrypt,
=======
=======
>>>>>>> refs/remotes/origin/master
		.name = "authenc(hmac(md5),cbc(des3_ede))",
		.driver_name = "authenc-hmac-md5-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
			.geniv = "<built-in>",
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_MD5 | OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_MD5 | OP_ALG_AAI_HMAC,
	},
	{
		.name = "authenc(hmac(sha1),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha1-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			.geniv = "<built-in>",
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA1 | OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA1 | OP_ALG_AAI_HMAC,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "authenc(hmac(sha256),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha256-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.aead = {
			.setkey = aead_authenc_setkey,
			.setauthsize = aead_authenc_setauthsize,
			.encrypt = aead_authenc_encrypt,
			.decrypt = aead_authenc_decrypt,
			.givencrypt = aead_authenc_givencrypt,
=======
		.name = "authenc(hmac(sha224),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha224-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
=======
		.name = "authenc(hmac(sha224),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha224-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
>>>>>>> refs/remotes/origin/master
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
			.geniv = "<built-in>",
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA224 | OP_ALG_AAI_HMAC,
	},
	{
		.name = "authenc(hmac(sha256),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha256-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			.geniv = "<built-in>",
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA256 | OP_ALG_AAI_HMAC,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "authenc(hmac(sha512),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha512-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.aead = {
			.setkey = aead_authenc_setkey,
			.setauthsize = aead_authenc_setauthsize,
			.encrypt = aead_authenc_encrypt,
			.decrypt = aead_authenc_decrypt,
			.givencrypt = aead_authenc_givencrypt,
=======
		.name = "authenc(hmac(sha384),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha384-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
=======
		.name = "authenc(hmac(sha384),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha384-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
>>>>>>> refs/remotes/origin/master
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
			.geniv = "<built-in>",
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA384 | OP_ALG_AAI_HMAC,
	},
	{
		.name = "authenc(hmac(sha512),cbc(des3_ede))",
		.driver_name = "authenc-hmac-sha512-cbc-des3_ede-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			.geniv = "<built-in>",
			.ivsize = DES3_EDE_BLOCK_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA512 | OP_ALG_AAI_HMAC,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "authenc(hmac(sha1),cbc(des))",
		.driver_name = "authenc-hmac-sha1-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.aead = {
			.setkey = aead_authenc_setkey,
			.setauthsize = aead_authenc_setauthsize,
			.encrypt = aead_authenc_encrypt,
			.decrypt = aead_authenc_decrypt,
			.givencrypt = aead_authenc_givencrypt,
=======
=======
>>>>>>> refs/remotes/origin/master
		.name = "authenc(hmac(md5),cbc(des))",
		.driver_name = "authenc-hmac-md5-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
			.geniv = "<built-in>",
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = MD5_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_MD5 | OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_MD5 | OP_ALG_AAI_HMAC,
	},
	{
		.name = "authenc(hmac(sha1),cbc(des))",
		.driver_name = "authenc-hmac-sha1-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			.geniv = "<built-in>",
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA1_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA1 | OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA1 | OP_ALG_AAI_HMAC,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "authenc(hmac(sha256),cbc(des))",
		.driver_name = "authenc-hmac-sha256-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.aead = {
			.setkey = aead_authenc_setkey,
			.setauthsize = aead_authenc_setauthsize,
			.encrypt = aead_authenc_encrypt,
			.decrypt = aead_authenc_decrypt,
			.givencrypt = aead_authenc_givencrypt,
=======
		.name = "authenc(hmac(sha224),cbc(des))",
		.driver_name = "authenc-hmac-sha224-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
=======
		.name = "authenc(hmac(sha224),cbc(des))",
		.driver_name = "authenc-hmac-sha224-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
>>>>>>> refs/remotes/origin/master
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
			.geniv = "<built-in>",
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA224_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA224 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA224 | OP_ALG_AAI_HMAC,
	},
	{
		.name = "authenc(hmac(sha256),cbc(des))",
		.driver_name = "authenc-hmac-sha256-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			.geniv = "<built-in>",
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA256_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA256 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA256 | OP_ALG_AAI_HMAC,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "authenc(hmac(sha512),cbc(des))",
		.driver_name = "authenc-hmac-sha512-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.aead = {
			.setkey = aead_authenc_setkey,
			.setauthsize = aead_authenc_setauthsize,
			.encrypt = aead_authenc_encrypt,
			.decrypt = aead_authenc_decrypt,
			.givencrypt = aead_authenc_givencrypt,
=======
		.name = "authenc(hmac(sha384),cbc(des))",
		.driver_name = "authenc-hmac-sha384-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
=======
		.name = "authenc(hmac(sha384),cbc(des))",
		.driver_name = "authenc-hmac-sha384-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
>>>>>>> refs/remotes/origin/master
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
			.geniv = "<built-in>",
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA384_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA384 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA384 | OP_ALG_AAI_HMAC,
	},
	{
		.name = "authenc(hmac(sha512),cbc(des))",
		.driver_name = "authenc-hmac-sha512-cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_AEAD,
		.template_aead = {
			.setkey = aead_setkey,
			.setauthsize = aead_setauthsize,
			.encrypt = aead_encrypt,
			.decrypt = aead_decrypt,
			.givencrypt = aead_givencrypt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			.geniv = "<built-in>",
			.ivsize = DES_BLOCK_SIZE,
			.maxauthsize = SHA512_DIGEST_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
		.class2_alg_type = OP_ALG_ALGSEL_SHA512 |
				   OP_ALG_AAI_HMAC_PRECOMP,
		.alg_op = OP_ALG_ALGSEL_SHA512 | OP_ALG_AAI_HMAC,
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/* ablkcipher descriptor */
	{
		.name = "cbc(aes)",
		.driver_name = "cbc-aes-caam",
		.blocksize = AES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_ABLKCIPHER,
		.template_ablkcipher = {
			.setkey = ablkcipher_setkey,
			.encrypt = ablkcipher_encrypt,
			.decrypt = ablkcipher_decrypt,
			.geniv = "eseqiv",
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_AES | OP_ALG_AAI_CBC,
	},
	{
		.name = "cbc(des3_ede)",
		.driver_name = "cbc-3des-caam",
		.blocksize = DES3_EDE_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_ABLKCIPHER,
		.template_ablkcipher = {
			.setkey = ablkcipher_setkey,
			.encrypt = ablkcipher_encrypt,
			.decrypt = ablkcipher_decrypt,
			.geniv = "eseqiv",
			.min_keysize = DES3_EDE_KEY_SIZE,
			.max_keysize = DES3_EDE_KEY_SIZE,
			.ivsize = DES3_EDE_BLOCK_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_3DES | OP_ALG_AAI_CBC,
	},
	{
		.name = "cbc(des)",
		.driver_name = "cbc-des-caam",
		.blocksize = DES_BLOCK_SIZE,
		.type = CRYPTO_ALG_TYPE_ABLKCIPHER,
		.template_ablkcipher = {
			.setkey = ablkcipher_setkey,
			.encrypt = ablkcipher_encrypt,
			.decrypt = ablkcipher_decrypt,
			.geniv = "eseqiv",
			.min_keysize = DES_KEY_SIZE,
			.max_keysize = DES_KEY_SIZE,
			.ivsize = DES_BLOCK_SIZE,
			},
		.class1_alg_type = OP_ALG_ALGSEL_DES | OP_ALG_AAI_CBC,
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

struct caam_crypto_alg {
	struct list_head entry;
<<<<<<< HEAD
	struct device *ctrldev;
=======
>>>>>>> refs/remotes/origin/master
	int class1_alg_type;
	int class2_alg_type;
	int alg_op;
	struct crypto_alg crypto_alg;
};

static int caam_cra_init(struct crypto_tfm *tfm)
{
	struct crypto_alg *alg = tfm->__crt_alg;
	struct caam_crypto_alg *caam_alg =
		 container_of(alg, struct caam_crypto_alg, crypto_alg);
	struct caam_ctx *ctx = crypto_tfm_ctx(tfm);
<<<<<<< HEAD
	struct caam_drv_private *priv = dev_get_drvdata(caam_alg->ctrldev);
	int tgt_jr = atomic_inc_return(&priv->tfm_count);

	/*
	 * distribute tfms across job rings to ensure in-order
	 * crypto request processing per tfm
	 */
	ctx->jrdev = priv->algapi_jr[(tgt_jr / 2) % priv->num_jrs_for_algapi];
=======

	ctx->jrdev = caam_jr_alloc();
	if (IS_ERR(ctx->jrdev)) {
		pr_err("Job Ring Device allocation for transform failed\n");
		return PTR_ERR(ctx->jrdev);
	}
>>>>>>> refs/remotes/origin/master

	/* copy descriptor header template value */
	ctx->class1_alg_type = OP_TYPE_CLASS1_ALG | caam_alg->class1_alg_type;
	ctx->class2_alg_type = OP_TYPE_CLASS2_ALG | caam_alg->class2_alg_type;
	ctx->alg_op = OP_TYPE_CLASS2_ALG | caam_alg->alg_op;

	return 0;
}

static void caam_cra_exit(struct crypto_tfm *tfm)
{
	struct caam_ctx *ctx = crypto_tfm_ctx(tfm);

<<<<<<< HEAD
<<<<<<< HEAD
	if (!dma_mapping_error(ctx->jrdev, ctx->shared_desc_phys))
		dma_unmap_single(ctx->jrdev, ctx->shared_desc_phys,
				 desc_bytes(ctx->sh_desc), DMA_TO_DEVICE);
	kfree(ctx->sh_desc);

	if (!dma_mapping_error(ctx->jrdev, ctx->key_phys))
		dma_unmap_single(ctx->jrdev, ctx->key_phys,
				 ctx->split_key_pad_len + ctx->enckeylen,
				 DMA_TO_DEVICE);
	kfree(ctx->key);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (ctx->sh_desc_enc_dma &&
	    !dma_mapping_error(ctx->jrdev, ctx->sh_desc_enc_dma))
		dma_unmap_single(ctx->jrdev, ctx->sh_desc_enc_dma,
				 desc_bytes(ctx->sh_desc_enc), DMA_TO_DEVICE);
	if (ctx->sh_desc_dec_dma &&
	    !dma_mapping_error(ctx->jrdev, ctx->sh_desc_dec_dma))
		dma_unmap_single(ctx->jrdev, ctx->sh_desc_dec_dma,
				 desc_bytes(ctx->sh_desc_dec), DMA_TO_DEVICE);
	if (ctx->sh_desc_givenc_dma &&
	    !dma_mapping_error(ctx->jrdev, ctx->sh_desc_givenc_dma))
		dma_unmap_single(ctx->jrdev, ctx->sh_desc_givenc_dma,
				 desc_bytes(ctx->sh_desc_givenc),
				 DMA_TO_DEVICE);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

	caam_jr_free(ctx->jrdev);
>>>>>>> refs/remotes/origin/master
}

static void __exit caam_algapi_exit(void)
{

<<<<<<< HEAD
	struct device_node *dev_node;
	struct platform_device *pdev;
	struct device *ctrldev;
	struct caam_drv_private *priv;
	struct caam_crypto_alg *t_alg, *n;
	int i, err;

	dev_node = of_find_compatible_node(NULL, NULL, "fsl,sec-v4.0");
	if (!dev_node)
		return;

	pdev = of_find_device_by_node(dev_node);
	if (!pdev)
		return;

	ctrldev = &pdev->dev;
	of_node_put(dev_node);
	priv = dev_get_drvdata(ctrldev);

	if (!priv->alg_list.next)
		return;

	list_for_each_entry_safe(t_alg, n, &priv->alg_list, entry) {
=======
	struct caam_crypto_alg *t_alg, *n;

	if (!alg_list.next)
		return;

	list_for_each_entry_safe(t_alg, n, &alg_list, entry) {
>>>>>>> refs/remotes/origin/master
		crypto_unregister_alg(&t_alg->crypto_alg);
		list_del(&t_alg->entry);
		kfree(t_alg);
	}
<<<<<<< HEAD

	for (i = 0; i < priv->total_jobrs; i++) {
		err = caam_jr_deregister(priv->algapi_jr[i]);
		if (err < 0)
			break;
	}
	kfree(priv->algapi_jr);
}

static struct caam_crypto_alg *caam_alg_alloc(struct device *ctrldev,
					      struct caam_alg_template
=======
}

static struct caam_crypto_alg *caam_alg_alloc(struct caam_alg_template
>>>>>>> refs/remotes/origin/master
					      *template)
{
	struct caam_crypto_alg *t_alg;
	struct crypto_alg *alg;

	t_alg = kzalloc(sizeof(struct caam_crypto_alg), GFP_KERNEL);
	if (!t_alg) {
<<<<<<< HEAD
		dev_err(ctrldev, "failed to allocate t_alg\n");
=======
		pr_err("failed to allocate t_alg\n");
>>>>>>> refs/remotes/origin/master
		return ERR_PTR(-ENOMEM);
	}

	alg = &t_alg->crypto_alg;

	snprintf(alg->cra_name, CRYPTO_MAX_ALG_NAME, "%s", template->name);
	snprintf(alg->cra_driver_name, CRYPTO_MAX_ALG_NAME, "%s",
		 template->driver_name);
	alg->cra_module = THIS_MODULE;
	alg->cra_init = caam_cra_init;
	alg->cra_exit = caam_cra_exit;
	alg->cra_priority = CAAM_CRA_PRIORITY;
<<<<<<< HEAD
<<<<<<< HEAD
	alg->cra_flags = CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC;
	alg->cra_blocksize = template->blocksize;
	alg->cra_alignmask = 0;
	alg->cra_type = &crypto_aead_type;
	alg->cra_ctxsize = sizeof(struct caam_ctx);
	alg->cra_u.aead = template->aead;
=======
=======
>>>>>>> refs/remotes/origin/master
	alg->cra_blocksize = template->blocksize;
	alg->cra_alignmask = 0;
	alg->cra_ctxsize = sizeof(struct caam_ctx);
	alg->cra_flags = CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY |
			 template->type;
	switch (template->type) {
	case CRYPTO_ALG_TYPE_ABLKCIPHER:
		alg->cra_type = &crypto_ablkcipher_type;
		alg->cra_ablkcipher = template->template_ablkcipher;
		break;
	case CRYPTO_ALG_TYPE_AEAD:
		alg->cra_type = &crypto_aead_type;
		alg->cra_aead = template->template_aead;
		break;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	t_alg->class1_alg_type = template->class1_alg_type;
	t_alg->class2_alg_type = template->class2_alg_type;
	t_alg->alg_op = template->alg_op;
<<<<<<< HEAD
	t_alg->ctrldev = ctrldev;
=======
>>>>>>> refs/remotes/origin/master

	return t_alg;
}

static int __init caam_algapi_init(void)
{
<<<<<<< HEAD
	struct device_node *dev_node;
	struct platform_device *pdev;
	struct device *ctrldev, **jrdev;
	struct caam_drv_private *priv;
	int i = 0, err = 0;

	dev_node = of_find_compatible_node(NULL, NULL, "fsl,sec-v4.0");
	if (!dev_node)
		return -ENODEV;

	pdev = of_find_device_by_node(dev_node);
	if (!pdev)
		return -ENODEV;

	ctrldev = &pdev->dev;
	priv = dev_get_drvdata(ctrldev);
	of_node_put(dev_node);

	INIT_LIST_HEAD(&priv->alg_list);

	jrdev = kmalloc(sizeof(*jrdev) * priv->total_jobrs, GFP_KERNEL);
	if (!jrdev)
		return -ENOMEM;

	for (i = 0; i < priv->total_jobrs; i++) {
		err = caam_jr_register(ctrldev, &jrdev[i]);
		if (err < 0)
			break;
	}
	if (err < 0 && i == 0) {
		dev_err(ctrldev, "algapi error in job ring registration: %d\n",
			err);
		kfree(jrdev);
		return err;
	}

	priv->num_jrs_for_algapi = i;
	priv->algapi_jr = jrdev;
	atomic_set(&priv->tfm_count, -1);
=======
	int i = 0, err = 0;

	INIT_LIST_HEAD(&alg_list);
>>>>>>> refs/remotes/origin/master

	/* register crypto algorithms the device supports */
	for (i = 0; i < ARRAY_SIZE(driver_algs); i++) {
		/* TODO: check if h/w supports alg */
		struct caam_crypto_alg *t_alg;

<<<<<<< HEAD
		t_alg = caam_alg_alloc(ctrldev, &driver_algs[i]);
		if (IS_ERR(t_alg)) {
			err = PTR_ERR(t_alg);
			dev_warn(ctrldev, "%s alg allocation failed\n",
				 driver_algs[i].driver_name);
=======
		t_alg = caam_alg_alloc(&driver_algs[i]);
		if (IS_ERR(t_alg)) {
			err = PTR_ERR(t_alg);
			pr_warn("%s alg allocation failed\n",
				driver_algs[i].driver_name);
>>>>>>> refs/remotes/origin/master
			continue;
		}

		err = crypto_register_alg(&t_alg->crypto_alg);
		if (err) {
<<<<<<< HEAD
			dev_warn(ctrldev, "%s alg registration failed\n",
				t_alg->crypto_alg.cra_driver_name);
			kfree(t_alg);
<<<<<<< HEAD
		} else {
			list_add_tail(&t_alg->entry, &priv->alg_list);
			dev_info(ctrldev, "%s\n",
				 t_alg->crypto_alg.cra_driver_name);
		}
	}
=======
		} else
			list_add_tail(&t_alg->entry, &priv->alg_list);
	}
	if (!list_empty(&priv->alg_list))
		dev_info(ctrldev, "%s algorithms registered in /proc/crypto\n",
			 (char *)of_get_property(dev_node, "compatible", NULL));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_warn("%s alg registration failed\n",
				t_alg->crypto_alg.cra_driver_name);
			kfree(t_alg);
		} else
			list_add_tail(&t_alg->entry, &alg_list);
	}
	if (!list_empty(&alg_list))
		pr_info("caam algorithms registered in /proc/crypto\n");
>>>>>>> refs/remotes/origin/master

	return err;
}

module_init(caam_algapi_init);
module_exit(caam_algapi_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("FSL CAAM support for crypto API");
MODULE_AUTHOR("Freescale Semiconductor - NMG/STC");
