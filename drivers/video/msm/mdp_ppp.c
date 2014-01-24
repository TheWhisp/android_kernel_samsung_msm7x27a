<<<<<<< HEAD
/* drivers/video/msm/src/drv/mdp/mdp_ppp.c
 *
 * Copyright (C) 2007 Google Incorporated
 * Copyright (c) 2008-2009, 2012 The Linux Foundation. All rights reserved.
=======
/* drivers/video/msm/mdp_ppp.c
 *
 * Copyright (C) 2007 QUALCOMM Incorporated
 * Copyright (C) 2007 Google Incorporated
>>>>>>> refs/remotes/origin/master
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
<<<<<<< HEAD

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/fb.h>
#include <linux/msm_mdp.h>
#include <linux/file.h>
#include <linux/android_pmem.h>
#include <linux/major.h>

#include "linux/proc_fs.h"

#include <mach/hardware.h>
#include <linux/io.h>

#include <asm/system.h>
#include <asm/mach-types.h>
#include <linux/semaphore.h>
#include <linux/msm_kgsl.h>

#include "mdp.h"
#include "msm_fb.h"

#define MDP_IS_IMGTYPE_BAD(x) (((x) >= MDP_IMGTYPE_LIMIT) && \
				(((x) < MDP_IMGTYPE2_START) || \
				 ((x) >= MDP_IMGTYPE_LIMIT2)))
=======
#include <linux/fb.h>
#include <linux/file.h>
#include <linux/delay.h>
#include <linux/msm_mdp.h>
#include <linux/platform_data/video-msm_fb.h>

#include "mdp_hw.h"
#include "mdp_scale_tables.h"

#define DLOG(x...) do {} while (0)

#define MDP_DOWNSCALE_BLUR (MDP_DOWNSCALE_MAX + 1)
static int downscale_y_table = MDP_DOWNSCALE_MAX;
static int downscale_x_table = MDP_DOWNSCALE_MAX;

struct mdp_regs {
	uint32_t src0;
	uint32_t src1;
	uint32_t dst0;
	uint32_t dst1;
	uint32_t src_cfg;
	uint32_t dst_cfg;
	uint32_t src_pack;
	uint32_t dst_pack;
	uint32_t src_rect;
	uint32_t dst_rect;
	uint32_t src_ystride;
	uint32_t dst_ystride;
	uint32_t op;
	uint32_t src_bpp;
	uint32_t dst_bpp;
	uint32_t edge;
	uint32_t phasex_init;
	uint32_t phasey_init;
	uint32_t phasex_step;
	uint32_t phasey_step;
};

static uint32_t pack_pattern[] = {
	PPP_ARRAY0(PACK_PATTERN)
};

static uint32_t src_img_cfg[] = {
	PPP_ARRAY1(CFG, SRC)
};

static uint32_t dst_img_cfg[] = {
	PPP_ARRAY1(CFG, DST)
};
>>>>>>> refs/remotes/origin/master

static uint32_t bytes_per_pixel[] = {
	[MDP_RGB_565] = 2,
	[MDP_RGB_888] = 3,
	[MDP_XRGB_8888] = 4,
	[MDP_ARGB_8888] = 4,
	[MDP_RGBA_8888] = 4,
	[MDP_BGRA_8888] = 4,
	[MDP_RGBX_8888] = 4,
	[MDP_Y_CBCR_H2V1] = 1,
	[MDP_Y_CBCR_H2V2] = 1,
<<<<<<< HEAD
	[MDP_Y_CBCR_H2V2_ADRENO] = 1,
	[MDP_Y_CRCB_H2V1] = 1,
	[MDP_Y_CRCB_H2V2] = 1,
	[MDP_YCRYCB_H2V1] = 2,
	[MDP_BGR_565] = 2
};

extern uint32 mdp_plv[];
extern struct semaphore mdp_ppp_mutex;
static struct ion_client *ppp_display_iclient;

int mdp_get_bytes_per_pixel(uint32_t format,
				 struct msm_fb_data_type *mfd)
{
	int bpp = -EINVAL;
	if (format == MDP_FB_FORMAT)
		format = mfd->fb_imgType;
	if (format < ARRAY_SIZE(bytes_per_pixel))
		bpp = bytes_per_pixel[format];

	if (bpp <= 0)
		printk(KERN_ERR "%s incorrect format %d\n", __func__, format);
	return bpp;
}

static uint32 mdp_conv_matx_rgb2yuv(uint32 input_pixel,
				    uint16 *matrix_and_bias_vector,
				    uint32 *clamp_vector,
				    uint32 *look_up_table)
{
	uint8 input_C2, input_C0, input_C1;
	uint32 output;
	int32 comp_C2, comp_C1, comp_C0, temp;
	int32 temp1, temp2, temp3;
	int32 matrix[9];
	int32 bias_vector[3];
	int32 Y_low_limit, Y_high_limit, C_low_limit, C_high_limit;
	int32 i;
	uint32 _is_lookup_table_enabled;

	input_C2 = (input_pixel >> 16) & 0xFF;
	input_C1 = (input_pixel >> 8) & 0xFF;
	input_C0 = (input_pixel >> 0) & 0xFF;

	comp_C0 = input_C0;
	comp_C1 = input_C1;
	comp_C2 = input_C2;

	for (i = 0; i < 9; i++)
		matrix[i] =
		    ((int32) (((int32) matrix_and_bias_vector[i]) << 20)) >> 20;

	bias_vector[0] = (int32) (matrix_and_bias_vector[9] & 0xFF);
	bias_vector[1] = (int32) (matrix_and_bias_vector[10] & 0xFF);
	bias_vector[2] = (int32) (matrix_and_bias_vector[11] & 0xFF);

	Y_low_limit = (int32) clamp_vector[0];
	Y_high_limit = (int32) clamp_vector[1];
	C_low_limit = (int32) clamp_vector[2];
	C_high_limit = (int32) clamp_vector[3];

	if (look_up_table == 0)	/* check for NULL point */
		_is_lookup_table_enabled = 0;
	else
		_is_lookup_table_enabled = 1;

	if (_is_lookup_table_enabled == 1) {
		comp_C2 = (look_up_table[comp_C2] >> 16) & 0xFF;
		comp_C1 = (look_up_table[comp_C1] >> 8) & 0xFF;
		comp_C0 = (look_up_table[comp_C0] >> 0) & 0xFF;
	}
	/*
	 * Color Conversion
	 * reorder input colors
	 */
	temp = comp_C2;
	comp_C2 = comp_C1;
	comp_C1 = comp_C0;
	comp_C0 = temp;

	/* matrix multiplication */
	temp1 = comp_C0 * matrix[0] + comp_C1 * matrix[1] + comp_C2 * matrix[2];
	temp2 = comp_C0 * matrix[3] + comp_C1 * matrix[4] + comp_C2 * matrix[5];
	temp3 = comp_C0 * matrix[6] + comp_C1 * matrix[7] + comp_C2 * matrix[8];

	comp_C0 = temp1 + 0x100;
	comp_C1 = temp2 + 0x100;
	comp_C2 = temp3 + 0x100;

	/* take interger part */
	comp_C0 >>= 9;
	comp_C1 >>= 9;
	comp_C2 >>= 9;

	/* post bias (+) */
	comp_C0 += bias_vector[0];
	comp_C1 += bias_vector[1];
	comp_C2 += bias_vector[2];

	/* limit pixel to 8-bit */
	if (comp_C0 < 0)
		comp_C0 = 0;

	if (comp_C0 > 255)
		comp_C0 = 255;

	if (comp_C1 < 0)
		comp_C1 = 0;

	if (comp_C1 > 255)
		comp_C1 = 255;

	if (comp_C2 < 0)
		comp_C2 = 0;

	if (comp_C2 > 255)
		comp_C2 = 255;

	/* clamp */
	if (comp_C0 < Y_low_limit)
		comp_C0 = Y_low_limit;

	if (comp_C0 > Y_high_limit)
		comp_C0 = Y_high_limit;

	if (comp_C1 < C_low_limit)
		comp_C1 = C_low_limit;

	if (comp_C1 > C_high_limit)
		comp_C1 = C_high_limit;

	if (comp_C2 < C_low_limit)
		comp_C2 = C_low_limit;

	if (comp_C2 > C_high_limit)
		comp_C2 = C_high_limit;

	output = (comp_C2 << 16) | (comp_C1 << 8) | comp_C0;
	return output;
}

uint32 mdp_conv_matx_yuv2rgb(uint32 input_pixel,
			     uint16 *matrix_and_bias_vector,
			     uint32 *clamp_vector, uint32 *look_up_table)
{
	uint8 input_C2, input_C0, input_C1;
	uint32 output;
	int32 comp_C2, comp_C1, comp_C0, temp;
	int32 temp1, temp2, temp3;
	int32 matrix[9];
	int32 bias_vector[3];
	int32 Y_low_limit, Y_high_limit, C_low_limit, C_high_limit;
	int32 i;
	uint32 _is_lookup_table_enabled;

	input_C2 = (input_pixel >> 16) & 0xFF;
	input_C1 = (input_pixel >> 8) & 0xFF;
	input_C0 = (input_pixel >> 0) & 0xFF;

	comp_C0 = input_C0;
	comp_C1 = input_C1;
	comp_C2 = input_C2;

	for (i = 0; i < 9; i++)
		matrix[i] =
		    ((int32) (((int32) matrix_and_bias_vector[i]) << 20)) >> 20;

	bias_vector[0] = (int32) (matrix_and_bias_vector[9] & 0xFF);
	bias_vector[1] = (int32) (matrix_and_bias_vector[10] & 0xFF);
	bias_vector[2] = (int32) (matrix_and_bias_vector[11] & 0xFF);

	Y_low_limit = (int32) clamp_vector[0];
	Y_high_limit = (int32) clamp_vector[1];
	C_low_limit = (int32) clamp_vector[2];
	C_high_limit = (int32) clamp_vector[3];

	if (look_up_table == 0)	/* check for NULL point */
		_is_lookup_table_enabled = 0;
	else
		_is_lookup_table_enabled = 1;

	/* clamp */
	if (comp_C0 < Y_low_limit)
		comp_C0 = Y_low_limit;

	if (comp_C0 > Y_high_limit)
		comp_C0 = Y_high_limit;

	if (comp_C1 < C_low_limit)
		comp_C1 = C_low_limit;

	if (comp_C1 > C_high_limit)
		comp_C1 = C_high_limit;

	if (comp_C2 < C_low_limit)
		comp_C2 = C_low_limit;

	if (comp_C2 > C_high_limit)
		comp_C2 = C_high_limit;

	/*
	 * Color Conversion
	 * pre bias (-)
	 */
	comp_C0 -= bias_vector[0];
	comp_C1 -= bias_vector[1];
	comp_C2 -= bias_vector[2];

	/* matrix multiplication */
	temp1 = comp_C0 * matrix[0] + comp_C1 * matrix[1] + comp_C2 * matrix[2];
	temp2 = comp_C0 * matrix[3] + comp_C1 * matrix[4] + comp_C2 * matrix[5];
	temp3 = comp_C0 * matrix[6] + comp_C1 * matrix[7] + comp_C2 * matrix[8];

	comp_C0 = temp1 + 0x100;
	comp_C1 = temp2 + 0x100;
	comp_C2 = temp3 + 0x100;

	/* take interger part */
	comp_C0 >>= 9;
	comp_C1 >>= 9;
	comp_C2 >>= 9;

	/* reorder output colors */
	temp = comp_C0;
	comp_C0 = comp_C1;
	comp_C1 = comp_C2;
	comp_C2 = temp;

	/* limit pixel to 8-bit */
	if (comp_C0 < 0)
		comp_C0 = 0;

	if (comp_C0 > 255)
		comp_C0 = 255;

	if (comp_C1 < 0)
		comp_C1 = 0;

	if (comp_C1 > 255)
		comp_C1 = 255;

	if (comp_C2 < 0)
		comp_C2 = 0;

	if (comp_C2 > 255)
		comp_C2 = 255;

	/* Look-up table */
	if (_is_lookup_table_enabled == 1) {
		comp_C2 = (look_up_table[comp_C2] >> 16) & 0xFF;
		comp_C1 = (look_up_table[comp_C1] >> 8) & 0xFF;
		comp_C0 = (look_up_table[comp_C0] >> 0) & 0xFF;
	}

	output = (comp_C2 << 16) | (comp_C1 << 8) | comp_C0;
	return output;
}

static uint32 mdp_calc_tpval(MDPIMG *mdpImg)
{
	uint32 tpVal;
	uint8 plane_tp;

	tpVal = 0;
	if ((mdpImg->imgType == MDP_RGB_565)
	    || (mdpImg->imgType == MDP_BGR_565)) {
		/*
		 * transparent color conversion into 24 bpp
		 *
		 * C2R_8BIT
		 * left shift the entire bit and or it with the upper most bits
		 */
		plane_tp = (uint8) ((mdpImg->tpVal & 0xF800) >> 11);
		tpVal |= ((plane_tp << 3) | ((plane_tp & 0x1C) >> 2)) << 16;

		/* C1B_8BIT */
		plane_tp = (uint8) (mdpImg->tpVal & 0x1F);
		tpVal |= ((plane_tp << 3) | ((plane_tp & 0x1C) >> 2)) << 8;

		/* C0G_8BIT */
		plane_tp = (uint8) ((mdpImg->tpVal & 0x7E0) >> 5);
		tpVal |= ((plane_tp << 2) | ((plane_tp & 0x30) >> 4));
	} else {
		/* 24bit RGB to RBG conversion */

		tpVal = (mdpImg->tpVal & 0xFF00) >> 8;
		tpVal |= (mdpImg->tpVal & 0xFF) << 8;
		tpVal |= (mdpImg->tpVal & 0xFF0000);
	}

	return tpVal;
}

static uint8 *mdp_get_chroma_addr(MDPIBUF *iBuf)
{
	uint8 *dest1;

	dest1 = NULL;
	switch (iBuf->ibuf_type) {
	case MDP_Y_CBCR_H2V2:
	case MDP_Y_CRCB_H2V2:
	case MDP_Y_CBCR_H2V1:
	case MDP_Y_CRCB_H2V1:
		dest1 = (uint8 *) iBuf->buf;
		dest1 += iBuf->ibuf_width * iBuf->ibuf_height * iBuf->bpp;
		break;

	default:
		break;
	}

	return dest1;
}

static void mdp_ppp_setbg(MDPIBUF *iBuf)
{
	uint8 *bg0_addr;
	uint8 *bg1_addr;
	uint32 bg0_ystride, bg1_ystride;
	uint32 ppp_src_cfg_reg, unpack_pattern;
	int v_slice, h_slice;

	v_slice = h_slice = 1;
	bg0_addr = (uint8 *) iBuf->buf;
	bg1_addr = mdp_get_chroma_addr(iBuf);

	bg0_ystride = iBuf->ibuf_width * iBuf->bpp;
	bg1_ystride = iBuf->ibuf_width * iBuf->bpp;

	switch (iBuf->ibuf_type) {
	case MDP_BGR_565:
	case MDP_RGB_565:
		/* 888 = 3bytes
		 * RGB = 3Components
		 * RGB interleaved
		 */
		ppp_src_cfg_reg = PPP_SRC_C2R_5BITS | PPP_SRC_C0G_6BITS |
			PPP_SRC_C1B_5BITS | PPP_SRC_BPP_INTERLVD_2BYTES |
			PPP_SRC_INTERLVD_3COMPONENTS | PPP_SRC_UNPACK_TIGHT |
			PPP_SRC_UNPACK_ALIGN_LSB |
			PPP_SRC_FETCH_PLANES_INTERLVD;

		if (iBuf->ibuf_type == MDP_RGB_565)
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(0, CLR_R, CLR_G, CLR_B, 8);
		else
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(0, CLR_B, CLR_G, CLR_R, 8);
		break;

	case MDP_RGB_888:
		/*
		 * 888 = 3bytes
		 * RGB = 3Components
		 * RGB interleaved
		 */
		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS | PPP_SRC_C0G_8BITS |
		PPP_SRC_C1B_8BITS | PPP_SRC_BPP_INTERLVD_3BYTES |
		PPP_SRC_INTERLVD_3COMPONENTS | PPP_SRC_UNPACK_TIGHT |
		PPP_SRC_UNPACK_ALIGN_LSB | PPP_SRC_FETCH_PLANES_INTERLVD;

		unpack_pattern =
		    MDP_GET_PACK_PATTERN(0, CLR_R, CLR_G, CLR_B, 8);
		break;

	case MDP_BGRA_8888:
	case MDP_RGBA_8888:
	case MDP_ARGB_8888:
	case MDP_XRGB_8888:
	case MDP_RGBX_8888:
		/*
		 * 8888 = 4bytes
		 * ARGB = 4Components
		 * ARGB interleaved
		 */
		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS | PPP_SRC_C0G_8BITS |
		PPP_SRC_C1B_8BITS | PPP_SRC_C3A_8BITS | PPP_SRC_C3_ALPHA_EN |
		PPP_SRC_BPP_INTERLVD_4BYTES | PPP_SRC_INTERLVD_4COMPONENTS |
		PPP_SRC_UNPACK_TIGHT | PPP_SRC_UNPACK_ALIGN_LSB |
		PPP_SRC_FETCH_PLANES_INTERLVD;

		if (iBuf->ibuf_type == MDP_BGRA_8888)
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_R, CLR_G, CLR_B,
						 8);
		else if (iBuf->ibuf_type == MDP_RGBA_8888 ||
				 iBuf->ibuf_type == MDP_RGBX_8888)
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_B, CLR_G, CLR_R,
						 8);
		else if (iBuf->ibuf_type == MDP_XRGB_8888)
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_R, CLR_G, CLR_B,
						 8);
		else
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_R, CLR_G, CLR_B,
						 8);
		break;

	case MDP_Y_CBCR_H2V2:
	case MDP_Y_CRCB_H2V2:
		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS |
		    PPP_SRC_C0G_8BITS |
		    PPP_SRC_C1B_8BITS |
		    PPP_SRC_C3A_8BITS |
		    PPP_SRC_BPP_INTERLVD_2BYTES |
		    PPP_SRC_INTERLVD_2COMPONENTS |
		    PPP_SRC_UNPACK_TIGHT |
		    PPP_SRC_UNPACK_ALIGN_LSB | PPP_SRC_FETCH_PLANES_PSEUDOPLNR;

		if (iBuf->ibuf_type == MDP_Y_CBCR_H2V1)
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CB, CLR_CR, 8);
		else
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CR, CLR_CB, 8);
		v_slice = h_slice = 2;
		break;

	case MDP_YCRYCB_H2V1:
		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS |
		    PPP_SRC_C0G_8BITS |
		    PPP_SRC_C1B_8BITS |
		    PPP_SRC_C3A_8BITS |
		    PPP_SRC_BPP_INTERLVD_2BYTES |
		    PPP_SRC_INTERLVD_4COMPONENTS |
		    PPP_SRC_UNPACK_TIGHT | PPP_SRC_UNPACK_ALIGN_LSB;

		unpack_pattern =
		    MDP_GET_PACK_PATTERN(CLR_Y, CLR_CR, CLR_Y, CLR_CB, 8);
		h_slice = 2;
		break;

	case MDP_Y_CBCR_H2V1:
	case MDP_Y_CRCB_H2V1:
		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS |
		    PPP_SRC_C0G_8BITS |
		    PPP_SRC_C1B_8BITS |
		    PPP_SRC_C3A_8BITS |
		    PPP_SRC_BPP_INTERLVD_2BYTES |
		    PPP_SRC_INTERLVD_2COMPONENTS |
		    PPP_SRC_UNPACK_TIGHT |
		    PPP_SRC_UNPACK_ALIGN_LSB | PPP_SRC_FETCH_PLANES_PSEUDOPLNR;

		if (iBuf->ibuf_type == MDP_Y_CBCR_H2V1)
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CB, CLR_CR, 8);
		else
			unpack_pattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CR, CLR_CB, 8);
		h_slice = 2;
		break;

	default:
		return;
	}

	/* starting input address adjustment */
	mdp_adjust_start_addr(&bg0_addr, &bg1_addr, v_slice, h_slice,
			      iBuf->roi.lcd_x, iBuf->roi.lcd_y,
			      iBuf->ibuf_width, iBuf->ibuf_height, iBuf->bpp,
			      iBuf, 1);

	/*
	 * 0x01c0: background plane 0 addr
	 * 0x01c4: background plane 1 addr
	 * 0x01c8: background plane 2 addr
	 * 0x01cc: bg y stride for plane 0 and 1
	 * 0x01d0: bg y stride for plane 2
	 * 0x01d4: bg src PPP config
	 * 0x01d8: unpack pattern
	 */
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x01c0, bg0_addr);
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x01c4, bg1_addr);

	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x01cc,
		 (bg1_ystride << 16) | bg0_ystride);
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x01d4, ppp_src_cfg_reg);

	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x01d8, unpack_pattern);
}

#define IS_PSEUDOPLNR(img) ((img == MDP_Y_CRCB_H2V2) | \
				(img == MDP_Y_CBCR_H2V2) | \
				(img == MDP_Y_CBCR_H2V2_ADRENO) | \
				(img == MDP_Y_CRCB_H2V1) | \
				(img == MDP_Y_CBCR_H2V1))

#define IMG_LEN(rect_h, w, rect_w, bpp) (((rect_h) * w) * bpp)

#define Y_TO_CRCB_RATIO(format) \
	((format == MDP_Y_CBCR_H2V2 || format == MDP_Y_CBCR_H2V2_ADRENO || \
	  format == MDP_Y_CRCB_H2V2) ?  2 : (format == MDP_Y_CBCR_H2V1 || \
	  format == MDP_Y_CRCB_H2V1) ?  1 : 1)

#ifdef CONFIG_ANDROID_PMEM
static void get_len(struct mdp_img *img, struct mdp_rect *rect, uint32_t bpp,
			uint32_t *len0, uint32_t *len1)
{
	*len0 = IMG_LEN(rect->h, img->width, rect->w, bpp);
	if (IS_PSEUDOPLNR(img->format))
		*len1 = *len0/Y_TO_CRCB_RATIO(img->format);
	else
		*len1 = 0;
}

static void flush_imgs(struct mdp_blit_req *req, int src_bpp, int dst_bpp,
			struct file *p_src_file, struct file *p_dst_file)
{
	uint32_t src0_len, src1_len;

	if (!(req->flags & MDP_BLIT_NON_CACHED)) {
		/* flush src images to memory before dma to mdp */
		get_len(&req->src, &req->src_rect, src_bpp,
		&src0_len, &src1_len);

		flush_pmem_file(p_src_file,
		req->src.offset, src0_len);

		if (IS_PSEUDOPLNR(req->src.format))
			flush_pmem_file(p_src_file,
				req->src.offset + src0_len, src1_len);
	}

}
#else
static void flush_imgs(struct mdp_blit_req *req, int src_bpp, int dst_bpp,
			struct file *p_src_file, struct file *p_dst_file) { }
#endif

static void mdp_start_ppp(struct msm_fb_data_type *mfd, MDPIBUF *iBuf,
struct mdp_blit_req *req, struct file *p_src_file, struct file *p_dst_file)
{
	uint8 *src0, *src1;
	uint8 *dest0, *dest1;
	uint16 inpBpp;
	uint32 dest0_ystride;
	uint32 src_width;
	uint32 src_height;
	uint32 src0_ystride;
	uint32 src0_y1stride;
	uint32 dst_roi_width;
	uint32 dst_roi_height;
	uint32 ppp_src_cfg_reg, ppp_operation_reg, ppp_dst_cfg_reg;
	uint32 alpha, tpVal;
	uint32 packPattern;
	uint32 dst_packPattern;
	boolean inputRGB, outputRGB, pseudoplanr_output;
	int sv_slice, sh_slice;
	int dv_slice, dh_slice;
	boolean perPixelAlpha = FALSE;
	boolean ppp_lookUp_enable = FALSE;

	sv_slice = sh_slice = dv_slice = dh_slice = 1;
	alpha = tpVal = 0;
	src_width = iBuf->mdpImg.width;
	src_height = iBuf->roi.y + iBuf->roi.height;
	src1 = NULL;
	dest1 = NULL;

	inputRGB = outputRGB = TRUE;
	pseudoplanr_output = FALSE;
	ppp_operation_reg = 0;
	ppp_dst_cfg_reg = 0;
	ppp_src_cfg_reg = 0;

	/* Wait for the pipe to clear */
	do { } while (mdp_ppp_pipe_wait() <= 0);

	/*
	 * destination config
	 */
	switch (iBuf->ibuf_type) {
	case MDP_RGB_888:
		dst_packPattern =
		    MDP_GET_PACK_PATTERN(0, CLR_R, CLR_G, CLR_B, 8);
		ppp_dst_cfg_reg =
		    PPP_DST_C0G_8BIT | PPP_DST_C1B_8BIT | PPP_DST_C2R_8BIT |
		    PPP_DST_PACKET_CNT_INTERLVD_3ELEM | PPP_DST_PACK_TIGHT |
		    PPP_DST_PACK_ALIGN_LSB | PPP_DST_OUT_SEL_AXI |
		    PPP_DST_BPP_3BYTES | PPP_DST_PLANE_INTERLVD;
		break;

	case MDP_BGRA_8888:
	case MDP_XRGB_8888:
	case MDP_ARGB_8888:
	case MDP_RGBA_8888:
	case MDP_RGBX_8888:
		if (iBuf->ibuf_type == MDP_BGRA_8888)
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_R, CLR_G, CLR_B,
						 8);
		else if (iBuf->ibuf_type == MDP_RGBA_8888 ||
				 iBuf->ibuf_type == MDP_RGBX_8888)
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_B, CLR_G, CLR_R,
						 8);
		else if (iBuf->ibuf_type == MDP_XRGB_8888)
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_R, CLR_G, CLR_B,
						 8);
		else
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_R, CLR_G, CLR_B,
						 8);

		ppp_dst_cfg_reg = PPP_DST_C0G_8BIT |
		    PPP_DST_C1B_8BIT |
		    PPP_DST_C2R_8BIT |
		    PPP_DST_C3A_8BIT |
		    PPP_DST_C3ALPHA_EN |
		    PPP_DST_PACKET_CNT_INTERLVD_4ELEM |
		    PPP_DST_PACK_TIGHT |
		    PPP_DST_PACK_ALIGN_LSB |
		    PPP_DST_OUT_SEL_AXI |
		    PPP_DST_BPP_4BYTES | PPP_DST_PLANE_INTERLVD;
		break;

	case MDP_Y_CBCR_H2V2:
	case MDP_Y_CRCB_H2V2:
		if (iBuf->ibuf_type == MDP_Y_CBCR_H2V2)
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CB, CLR_CR, 8);
		else
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CR, CLR_CB, 8);

		ppp_dst_cfg_reg = PPP_DST_C2R_8BIT |
		    PPP_DST_C0G_8BIT |
		    PPP_DST_C1B_8BIT |
		    PPP_DST_C3A_8BIT |
		    PPP_DST_PACKET_CNT_INTERLVD_2ELEM |
		    PPP_DST_PACK_TIGHT |
		    PPP_DST_PACK_ALIGN_LSB |
		    PPP_DST_OUT_SEL_AXI | PPP_DST_BPP_2BYTES;

		ppp_operation_reg |= PPP_OP_DST_CHROMA_420;
		outputRGB = FALSE;
		pseudoplanr_output = TRUE;
		/*
		 * vertically (y direction) and horizontally (x direction)
		 * sample reduction by 2
		 */

		/*
		 * H2V2(YUV420) Cosite
		 *
		 * Y    Y    Y    Y
		 * CbCr      CbCr
		 * Y    Y    Y    Y
		 * Y    Y    Y    Y
		 * CbCr      CbCr
		 * Y    Y    Y    Y
		 */
		dv_slice = dh_slice = 2;

		/* (x,y) and (width,height) must be even numbern */
		iBuf->roi.lcd_x = (iBuf->roi.lcd_x / 2) * 2;
		iBuf->roi.dst_width = (iBuf->roi.dst_width / 2) * 2;
		iBuf->roi.x = (iBuf->roi.x / 2) * 2;
		iBuf->roi.width = (iBuf->roi.width / 2) * 2;

		iBuf->roi.lcd_y = (iBuf->roi.lcd_y / 2) * 2;
		iBuf->roi.dst_height = (iBuf->roi.dst_height / 2) * 2;
		iBuf->roi.y = (iBuf->roi.y / 2) * 2;
		iBuf->roi.height = (iBuf->roi.height / 2) * 2;
		break;

	case MDP_YCRYCB_H2V1:
		dst_packPattern =
		    MDP_GET_PACK_PATTERN(CLR_Y, CLR_CR, CLR_Y, CLR_CB, 8);
		ppp_dst_cfg_reg =
		    PPP_DST_C2R_8BIT | PPP_DST_C0G_8BIT | PPP_DST_C1B_8BIT |
		    PPP_DST_C3A_8BIT | PPP_DST_PACKET_CNT_INTERLVD_4ELEM |
		    PPP_DST_PACK_TIGHT | PPP_DST_PACK_ALIGN_LSB |
		    PPP_DST_OUT_SEL_AXI | PPP_DST_BPP_2BYTES |
		    PPP_DST_PLANE_INTERLVD;

		ppp_operation_reg |= PPP_OP_DST_CHROMA_H2V1;
		outputRGB = FALSE;
		/*
		 * horizontally (x direction) sample reduction by 2
		 *
		 * H2V1(YUV422) Cosite
		 *
		 * YCbCr    Y    YCbCr    Y
		 * YCbCr    Y    YCbCr    Y
		 * YCbCr    Y    YCbCr    Y
		 * YCbCr    Y    YCbCr    Y
		 */
		dh_slice = 2;

		/*
		 * if it's TV-Out/MDP_YCRYCB_H2V1, let's go through the
		 * preloaded gamma setting of 2.2 when the content is
		 * non-linear ppp_lookUp_enable = TRUE;
		 */

		/* x and width must be even number */
		iBuf->roi.lcd_x = (iBuf->roi.lcd_x / 2) * 2;
		iBuf->roi.dst_width = (iBuf->roi.dst_width / 2) * 2;
		iBuf->roi.x = (iBuf->roi.x / 2) * 2;
		iBuf->roi.width = (iBuf->roi.width / 2) * 2;
		break;

	case MDP_Y_CBCR_H2V1:
	case MDP_Y_CRCB_H2V1:
		if (iBuf->ibuf_type == MDP_Y_CBCR_H2V1)
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CB, CLR_CR, 8);
		else
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CR, CLR_CB, 8);

		ppp_dst_cfg_reg = PPP_DST_C2R_8BIT |
		    PPP_DST_C0G_8BIT |
		    PPP_DST_C1B_8BIT |
		    PPP_DST_C3A_8BIT |
		    PPP_DST_PACKET_CNT_INTERLVD_2ELEM |
		    PPP_DST_PACK_TIGHT |
		    PPP_DST_PACK_ALIGN_LSB |
		    PPP_DST_OUT_SEL_AXI | PPP_DST_BPP_2BYTES;

		ppp_operation_reg |= PPP_OP_DST_CHROMA_H2V1;
		outputRGB = FALSE;
		pseudoplanr_output = TRUE;
		/* horizontally (x direction) sample reduction by 2 */
		dh_slice = 2;

		/* x and width must be even number */
		iBuf->roi.lcd_x = (iBuf->roi.lcd_x / 2) * 2;
		iBuf->roi.dst_width = (iBuf->roi.dst_width / 2) * 2;
		iBuf->roi.x = (iBuf->roi.x / 2) * 2;
		iBuf->roi.width = (iBuf->roi.width / 2) * 2;
		break;

	case MDP_BGR_565:
	case MDP_RGB_565:
	default:
		if (iBuf->ibuf_type == MDP_RGB_565)
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(0, CLR_R, CLR_G, CLR_B, 8);
		else
			dst_packPattern =
			    MDP_GET_PACK_PATTERN(0, CLR_B, CLR_G, CLR_R, 8);

		ppp_dst_cfg_reg = PPP_DST_C0G_6BIT |
		    PPP_DST_C1B_5BIT |
		    PPP_DST_C2R_5BIT |
		    PPP_DST_PACKET_CNT_INTERLVD_3ELEM |
		    PPP_DST_PACK_TIGHT |
		    PPP_DST_PACK_ALIGN_LSB |
		    PPP_DST_OUT_SEL_AXI |
		    PPP_DST_BPP_2BYTES | PPP_DST_PLANE_INTERLVD;
		break;
	}

	/* source config */
	switch (iBuf->mdpImg.imgType) {
	case MDP_RGB_888:
		inpBpp = 3;
		/*
		 * 565 = 2bytes
		 * RGB = 3Components
		 * RGB interleaved
		 */
		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS | PPP_SRC_C0G_8BITS |
			PPP_SRC_C1B_8BITS | PPP_SRC_BPP_INTERLVD_3BYTES |
			PPP_SRC_INTERLVD_3COMPONENTS | PPP_SRC_UNPACK_TIGHT |
			PPP_SRC_UNPACK_ALIGN_LSB |
			PPP_SRC_FETCH_PLANES_INTERLVD;

		packPattern = MDP_GET_PACK_PATTERN(0, CLR_R, CLR_G, CLR_B, 8);

		ppp_operation_reg |= PPP_OP_COLOR_SPACE_RGB |
		    PPP_OP_SRC_CHROMA_RGB | PPP_OP_DST_CHROMA_RGB;
		break;

	case MDP_BGRA_8888:
	case MDP_RGBA_8888:
	case MDP_ARGB_8888:
		perPixelAlpha = TRUE;
	case MDP_XRGB_8888:
	case MDP_RGBX_8888:
		inpBpp = 4;
		/*
		 * 8888 = 4bytes
		 * ARGB = 4Components
		 * ARGB interleaved
		 */
		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS | PPP_SRC_C0G_8BITS |
			PPP_SRC_C1B_8BITS | PPP_SRC_C3A_8BITS |
			PPP_SRC_C3_ALPHA_EN | PPP_SRC_BPP_INTERLVD_4BYTES |
			PPP_SRC_INTERLVD_4COMPONENTS | PPP_SRC_UNPACK_TIGHT |
			PPP_SRC_UNPACK_ALIGN_LSB |
			PPP_SRC_FETCH_PLANES_INTERLVD;

		if (iBuf->mdpImg.imgType == MDP_BGRA_8888)
			packPattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_R, CLR_G, CLR_B,
						 8);
		else if (iBuf->mdpImg.imgType == MDP_RGBA_8888 ||
				 iBuf->mdpImg.imgType == MDP_RGBX_8888)
			packPattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_B, CLR_G, CLR_R,
						 8);
		else if (iBuf->ibuf_type == MDP_XRGB_8888)
			packPattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_R, CLR_G, CLR_B,
						 8);
		else
			packPattern =
			    MDP_GET_PACK_PATTERN(CLR_ALPHA, CLR_R, CLR_G, CLR_B,
						 8);

		ppp_operation_reg |= PPP_OP_COLOR_SPACE_RGB |
		    PPP_OP_SRC_CHROMA_RGB | PPP_OP_DST_CHROMA_RGB;
		break;

	case MDP_Y_CBCR_H2V2:
	case MDP_Y_CBCR_H2V2_ADRENO:
	case MDP_Y_CRCB_H2V2:
		inpBpp = 1;
		src1 = (uint8 *) iBuf->mdpImg.cbcr_addr;

		/*
		 * CbCr = 2bytes
		 * CbCr = 2Components
		 * Y+CbCr
		 */
		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS | PPP_SRC_C0G_8BITS |
			PPP_SRC_C1B_8BITS | PPP_SRC_BPP_INTERLVD_2BYTES |
			PPP_SRC_INTERLVD_2COMPONENTS | PPP_SRC_UNPACK_TIGHT |
			PPP_SRC_UNPACK_ALIGN_LSB |
			PPP_SRC_FETCH_PLANES_PSEUDOPLNR;

		if (iBuf->mdpImg.imgType == MDP_Y_CRCB_H2V2)
			packPattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CR, CLR_CB, 8);
		else
			packPattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CB, CLR_CR, 8);

		ppp_operation_reg |= PPP_OP_COLOR_SPACE_YCBCR |
		    PPP_OP_SRC_CHROMA_420 |
		    PPP_OP_SRC_CHROMA_COSITE |
		    PPP_OP_DST_CHROMA_RGB | PPP_OP_DST_CHROMA_COSITE;

		inputRGB = FALSE;
		sh_slice = sv_slice = 2;
		break;

	case MDP_YCRYCB_H2V1:
		inpBpp = 2;
		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS |
		    PPP_SRC_C0G_8BITS |
		    PPP_SRC_C1B_8BITS |
		    PPP_SRC_C3A_8BITS |
		    PPP_SRC_BPP_INTERLVD_2BYTES |
		    PPP_SRC_INTERLVD_4COMPONENTS |
		    PPP_SRC_UNPACK_TIGHT | PPP_SRC_UNPACK_ALIGN_LSB;

		packPattern =
		    MDP_GET_PACK_PATTERN(CLR_Y, CLR_CR, CLR_Y, CLR_CB, 8);

		ppp_operation_reg |= PPP_OP_SRC_CHROMA_H2V1 |
		    PPP_OP_SRC_CHROMA_COSITE | PPP_OP_DST_CHROMA_COSITE;

		/*
		 * if it's TV-Out/MDP_YCRYCB_H2V1, let's go through the
		 * preloaded inverse gamma setting of 2.2 since they're
		 * symetric when the content is non-linear
		 * ppp_lookUp_enable = TRUE;
		 */

		/* x and width must be even number */
		iBuf->roi.lcd_x = (iBuf->roi.lcd_x / 2) * 2;
		iBuf->roi.dst_width = (iBuf->roi.dst_width / 2) * 2;
		iBuf->roi.x = (iBuf->roi.x / 2) * 2;
		iBuf->roi.width = (iBuf->roi.width / 2) * 2;

		inputRGB = FALSE;
		sh_slice = 2;
		break;

	case MDP_Y_CBCR_H2V1:
	case MDP_Y_CRCB_H2V1:
		inpBpp = 1;
		src1 = (uint8 *) iBuf->mdpImg.cbcr_addr;

		ppp_src_cfg_reg = PPP_SRC_C2R_8BITS |
		    PPP_SRC_C0G_8BITS |
		    PPP_SRC_C1B_8BITS |
		    PPP_SRC_C3A_8BITS |
		    PPP_SRC_BPP_INTERLVD_2BYTES |
		    PPP_SRC_INTERLVD_2COMPONENTS |
		    PPP_SRC_UNPACK_TIGHT |
		    PPP_SRC_UNPACK_ALIGN_LSB | PPP_SRC_FETCH_PLANES_PSEUDOPLNR;

		if (iBuf->mdpImg.imgType == MDP_Y_CBCR_H2V1)
			packPattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CB, CLR_CR, 8);
		else
			packPattern =
			    MDP_GET_PACK_PATTERN(0, 0, CLR_CR, CLR_CB, 8);

		ppp_operation_reg |= PPP_OP_SRC_CHROMA_H2V1 |
		    PPP_OP_SRC_CHROMA_COSITE | PPP_OP_DST_CHROMA_COSITE;
		inputRGB = FALSE;
		sh_slice = 2;
		break;

	case MDP_BGR_565:
	case MDP_RGB_565:
	default:
		inpBpp = 2;
		/*
		 * 565 = 2bytes
		 * RGB = 3Components
		 * RGB interleaved
		 */
		ppp_src_cfg_reg = PPP_SRC_C2R_5BITS | PPP_SRC_C0G_6BITS |
			PPP_SRC_C1B_5BITS | PPP_SRC_BPP_INTERLVD_2BYTES |
			PPP_SRC_INTERLVD_3COMPONENTS | PPP_SRC_UNPACK_TIGHT |
			PPP_SRC_UNPACK_ALIGN_LSB |
			PPP_SRC_FETCH_PLANES_INTERLVD;

		if (iBuf->mdpImg.imgType == MDP_RGB_565)
			packPattern =
			    MDP_GET_PACK_PATTERN(0, CLR_R, CLR_G, CLR_B, 8);
		else
			packPattern =
			    MDP_GET_PACK_PATTERN(0, CLR_B, CLR_G, CLR_R, 8);

		ppp_operation_reg |= PPP_OP_COLOR_SPACE_RGB |
		    PPP_OP_SRC_CHROMA_RGB | PPP_OP_DST_CHROMA_RGB;
		break;

	}

	if (pseudoplanr_output)
		ppp_dst_cfg_reg |= PPP_DST_PLANE_PSEUDOPLN;

	/* YCbCr to RGB color conversion flag */
	if ((!inputRGB) && (outputRGB)) {
		ppp_operation_reg |= PPP_OP_CONVERT_YCBCR2RGB |
		    PPP_OP_CONVERT_ON;

		/*
		 * primary/secondary is sort of misleading term...but
		 * in mdp2.2/3.0 we only use primary matrix (forward/rev)
		 * in mdp3.1 we use set1(prim) and set2(secd)
		 */
#ifdef CONFIG_FB_MSM_MDP31
		ppp_operation_reg |= PPP_OP_CONVERT_MATRIX_SECONDARY |
					PPP_OP_DST_RGB;
		MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0240, 0);
#endif

		if (ppp_lookUp_enable) {
			ppp_operation_reg |= PPP_OP_LUT_C0_ON |
			    PPP_OP_LUT_C1_ON | PPP_OP_LUT_C2_ON;
		}
	}
	/* RGB to YCbCr color conversion flag */
	if ((inputRGB) && (!outputRGB)) {
		ppp_operation_reg |= PPP_OP_CONVERT_RGB2YCBCR |
		    PPP_OP_CONVERT_ON;

#ifdef CONFIG_FB_MSM_MDP31
		ppp_operation_reg |= PPP_OP_CONVERT_MATRIX_PRIMARY |
					PPP_OP_DST_YCBCR;
		MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0240, 0x1e);
#endif

		if (ppp_lookUp_enable) {
			ppp_operation_reg |= PPP_OP_LUT_C0_ON |
			    PPP_OP_LUT_C1_ON | PPP_OP_LUT_C2_ON;
		}
	}
	/* YCbCr to YCbCr color conversion flag */
	if ((!inputRGB) && (!outputRGB)) {
		if ((ppp_lookUp_enable) &&
		    (iBuf->mdpImg.imgType != iBuf->ibuf_type)) {
			ppp_operation_reg |= PPP_OP_LUT_C0_ON;
		}
	}

	ppp_src_cfg_reg |= (iBuf->roi.x % 2) ? PPP_SRC_BPP_ROI_ODD_X : 0;
	ppp_src_cfg_reg |= (iBuf->roi.y % 2) ? PPP_SRC_BPP_ROI_ODD_Y : 0;

	if (req->flags & MDP_DEINTERLACE)
		ppp_operation_reg |= PPP_OP_DEINT_EN;

	/* Dither at DMA side only since iBuf format is RGB888 */
	if (iBuf->mdpImg.mdpOp & MDPOP_DITHER)
		ppp_operation_reg |= PPP_OP_DITHER_EN;

	if (iBuf->mdpImg.mdpOp & MDPOP_ROTATION) {
		ppp_operation_reg |= PPP_OP_ROT_ON;

		if (iBuf->mdpImg.mdpOp & MDPOP_ROT90) {
			ppp_operation_reg |= PPP_OP_ROT_90;
		}
		if (iBuf->mdpImg.mdpOp & MDPOP_LR) {
			ppp_operation_reg |= PPP_OP_FLIP_LR;
		}
		if (iBuf->mdpImg.mdpOp & MDPOP_UD) {
			ppp_operation_reg |= PPP_OP_FLIP_UD;
		}
	}

	if (iBuf->mdpImg.imgType == MDP_Y_CBCR_H2V2_ADRENO)
		src0_ystride = ALIGN(src_width, 32) * inpBpp;
	else
		src0_ystride = src_width * inpBpp;

	if (iBuf->mdpImg.imgType == MDP_Y_CBCR_H2V2_ADRENO)
		src0_y1stride = 2 * ALIGN(src_width/2, 32);
	else
		src0_y1stride = src0_ystride;

	dest0_ystride = iBuf->ibuf_width * iBuf->bpp;

	/* no need to care about rotation since it's the real-XY. */
	dst_roi_width = iBuf->roi.dst_width;
	dst_roi_height = iBuf->roi.dst_height;

	src0 = (uint8 *) iBuf->mdpImg.bmy_addr;
	dest0 = (uint8 *) iBuf->buf;

	/* Jumping from Y-Plane to Chroma Plane */
	dest1 = mdp_get_chroma_addr(iBuf);

	/* first pixel addr calculation */
	mdp_adjust_start_addr(&src0, &src1, sv_slice, sh_slice, iBuf->roi.x,
			      iBuf->roi.y, src_width, src_height, inpBpp, iBuf,
			      0);
	mdp_adjust_start_addr(&dest0, &dest1, dv_slice, dh_slice,
			      iBuf->roi.lcd_x, iBuf->roi.lcd_y,
			      iBuf->ibuf_width, iBuf->ibuf_height, iBuf->bpp,
			      iBuf, 2);

	/* set scale operation */
	mdp_set_scale(iBuf, dst_roi_width, dst_roi_height,
		      inputRGB, outputRGB, &ppp_operation_reg);

	/*
	 * setting background source for blending
	 */
	mdp_set_blend_attr(iBuf, &alpha, &tpVal, perPixelAlpha,
			   &ppp_operation_reg);

	if (ppp_operation_reg & PPP_OP_BLEND_ON) {
		mdp_ppp_setbg(iBuf);

		if (iBuf->ibuf_type == MDP_YCRYCB_H2V1) {
			ppp_operation_reg |= PPP_OP_BG_CHROMA_H2V1;

			if (iBuf->mdpImg.mdpOp & MDPOP_TRANSP) {
				tpVal = mdp_conv_matx_rgb2yuv(tpVal,
						      (uint16 *) &
						      mdp_ccs_rgb2yuv,
						      &mdp_plv[0], NULL);
			}
		}
	}

	/*
	 * 0x0004: enable dbg bus
	 * 0x0100: "don't care" Edge Condit until scaling is on
	 * 0x0104: xrc tile x&y size u7.6 format = 7bit.6bit
	 * 0x0108: src pixel size
	 * 0x010c: component plane 0 starting address
	 * 0x011c: component plane 0 ystride
	 * 0x0124: PPP source config register
	 * 0x0128: unpacked pattern from lsb to msb (eg. RGB->BGR)
	 */
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0108, (iBuf->roi.height << 16 |
						      iBuf->roi.width));
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x010c, src0); /* comp.plane 0 */
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0110, src1); /* comp.plane 1 */
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x011c,
		 (src0_y1stride << 16 | src0_ystride));

	/* setup for rgb 565 */
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0124, ppp_src_cfg_reg);
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0128, packPattern);
	/*
	 * 0x0138: PPP destination operation register
	 * 0x014c: constant_alpha|transparent_color
	 * 0x0150: PPP destination config register
	 * 0x0154: PPP packing pattern
	 */
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0138, ppp_operation_reg);
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x014c, alpha << 24 | (tpVal &
								0xffffff));
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0150, ppp_dst_cfg_reg);
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0154, dst_packPattern);

	/*
	 * 0x0164: ROI height and width
	 * 0x0168: Component Plane 0 starting addr
	 * 0x016c: Component Plane 1 starting addr
	 * 0x0178: Component Plane 1/0 y stride
	 */
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0164,
		 (dst_roi_height << 16 | dst_roi_width));
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0168, dest0);
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x016c, dest1);
	MDP_OUTP(MDP_CMD_DEBUG_ACCESS_BASE + 0x0178,
		 (dest0_ystride << 16 | dest0_ystride));

	flush_imgs(req, inpBpp, iBuf->bpp, p_src_file, p_dst_file);
#ifdef	CONFIG_FB_MSM_MDP31
	MDP_OUTP(MDP_BASE + 0x00100, 0xFF00);
#endif
	mdp_pipe_kickoff(MDP_PPP_TERM, mfd);
}

static int mdp_ppp_verify_req(struct mdp_blit_req *req)
{
	u32 src_width, src_height, dst_width, dst_height;

	if (req == NULL) {
		printk(KERN_ERR "\n%s(): Error in Line %u", __func__,
			__LINE__);
		return -1;
	}

	if (MDP_IS_IMGTYPE_BAD(req->src.format) ||
	    MDP_IS_IMGTYPE_BAD(req->dst.format)) {
		printk(KERN_ERR "\n%s(): Error in Line %u", __func__,
			__LINE__);
		return -1;
	}

	if ((req->src.width == 0) || (req->src.height == 0) ||
	    (req->src_rect.w == 0) || (req->src_rect.h == 0) ||
	    (req->dst.width == 0) || (req->dst.height == 0) ||
	    (req->dst_rect.w == 0) || (req->dst_rect.h == 0)) {
		printk(KERN_ERR "\n%s(): Error in Line %u", __func__,
			__LINE__);

		return -1;
	}

	if (((req->src_rect.x + req->src_rect.w) > req->src.width) ||
	    ((req->src_rect.y + req->src_rect.h) > req->src.height)) {
		printk(KERN_ERR "\n%s(): Error in Line %u", __func__,
			__LINE__);
		return -1;
	}

	if (((req->dst_rect.x + req->dst_rect.w) > req->dst.width) ||
	    ((req->dst_rect.y + req->dst_rect.h) > req->dst.height)) {
		printk(KERN_ERR "\n%s(): Error in Line %u", __func__,
			__LINE__);
		return -1;
	}

	/*
	 * scaling range check
	 */
	src_width = req->src_rect.w;
	src_height = req->src_rect.h;

	if (req->flags & MDP_ROT_90) {
		dst_width = req->dst_rect.h;
		dst_height = req->dst_rect.w;
	} else {
		dst_width = req->dst_rect.w;
		dst_height = req->dst_rect.h;
	}

	switch (req->dst.format) {
	case MDP_Y_CRCB_H2V2:
	case MDP_Y_CBCR_H2V2:
		src_width = (src_width / 2) * 2;
		src_height = (src_height / 2) * 2;
		dst_width = (src_width / 2) * 2;
		dst_height = (src_height / 2) * 2;
		break;

	case MDP_Y_CRCB_H2V1:
	case MDP_Y_CBCR_H2V1:
	case MDP_YCRYCB_H2V1:
		src_width = (src_width / 2) * 2;
		dst_width = (src_width / 2) * 2;
		break;

	default:
		break;
	}

	if (((MDP_SCALE_Q_FACTOR * dst_width) / src_width >
	     MDP_MAX_X_SCALE_FACTOR)
	    || ((MDP_SCALE_Q_FACTOR * dst_width) / src_width <
		MDP_MIN_X_SCALE_FACTOR)) {
		printk(KERN_ERR "\n%s(): Error in Line %u", __func__,
			__LINE__);
		return -1;
	}

	if (((MDP_SCALE_Q_FACTOR * dst_height) / src_height >
	     MDP_MAX_Y_SCALE_FACTOR)
	    || ((MDP_SCALE_Q_FACTOR * dst_height) / src_height <
		MDP_MIN_Y_SCALE_FACTOR)) {
		printk(KERN_ERR "\n%s(): Error in Line %u", __func__,
			__LINE__);
		return -1;
	}
	return 0;
}

int get_gem_img(struct mdp_img *img, unsigned long *start, unsigned long *len)
{
	/* Set len to zero to appropriately error out if
	   kgsl_gem_obj_addr fails */

	*len = 0;
	return kgsl_gem_obj_addr(img->memory_id, (int) img->priv, start, len);
}

int get_img(struct mdp_img *img, struct mdp_blit_req *req,
		struct fb_info *info, unsigned long *start, unsigned long *len,
		struct file **srcp_file, struct ion_handle **srcp_ihdl)
{
	int put_needed, fb_num, ret = 0;
	struct file *file;
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
	struct msm_fb_data_type *mfd = (struct msm_fb_data_type *)info->par;
#endif
#ifdef CONFIG_ANDROID_PMEM
	unsigned long vstart;
#endif

	if (req->flags & MDP_MEMORY_ID_TYPE_FB) {
		file = fget_light(img->memory_id, &put_needed);
		if (file == NULL)
			return -EINVAL;

		if (MAJOR(file->f_dentry->d_inode->i_rdev) == FB_MAJOR) {
			fb_num = MINOR(file->f_dentry->d_inode->i_rdev);
			if (get_fb_phys_info(start, len, fb_num,
				DISPLAY_SUBSYSTEM_ID)) {
				pr_err("get_fb_phys_info() failed\n");
				fput_light(file, put_needed);
			} else {
				*srcp_file = file;
			}

			return ret;
		} else {
			fput_light(file, put_needed);
		}
	}
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
<<<<<<< HEAD
		*srcp_ihdl = ion_import_fd(mfd->iclient, img->memory_id);
=======
		*srcp_ihdl = ion_import_dma_buf(mfd->iclient, img->memory_id);
>>>>>>> refs/remotes/origin/cm-10.0
		if (IS_ERR_OR_NULL(*srcp_ihdl))
			return PTR_ERR(*srcp_ihdl);

		if (!ion_phys(mfd->iclient, *srcp_ihdl, start, (size_t *) len))
			return ret;
		 else
			return -EINVAL;
#endif

#ifdef CONFIG_ANDROID_PMEM
	if (!get_pmem_file(img->memory_id, start, &vstart, len, srcp_file))
		return ret;
	else
		return -EINVAL;
#endif
}

void put_img(struct file *p_src_file, struct ion_handle *p_ihdl)
{
#ifdef CONFIG_ANDROID_PMEM
	if (p_src_file)
		put_pmem_file(p_src_file);
#endif

#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
	if (!IS_ERR_OR_NULL(p_ihdl))
		ion_free(ppp_display_iclient, p_ihdl);
#endif
}


static int mdp_ppp_blit_addr(struct fb_info *info, struct mdp_blit_req *req,
	unsigned long srcp0_start, unsigned long srcp0_len,
	unsigned long srcp1_start, unsigned long srcp1_len,
	unsigned long dst_start, unsigned long dst_len,
	struct file *p_src_file, struct file *p_dst_file,
	struct ion_handle **src_ihdl, struct ion_handle **dst_ihdl)
{
	MDPIBUF iBuf;
	u32 dst_width, dst_height;
	struct msm_fb_data_type *mfd = info->par;

	if (req->dst.format == MDP_FB_FORMAT)
		req->dst.format =  mfd->fb_imgType;
	if (req->src.format == MDP_FB_FORMAT)
		req->src.format = mfd->fb_imgType;

	if (mdp_ppp_verify_req(req)) {
		pr_err("mdp_ppp: invalid image!\n");
		put_img(p_src_file, *src_ihdl);
		put_img(p_dst_file, *dst_ihdl);
		return -1;
	}

	iBuf.ibuf_width = req->dst.width;
	iBuf.ibuf_height = req->dst.height;
	iBuf.bpp = bytes_per_pixel[req->dst.format];

	iBuf.ibuf_type = req->dst.format;
	iBuf.buf = (uint8 *) dst_start;
	iBuf.buf += req->dst.offset;

	iBuf.roi.lcd_x = req->dst_rect.x;
	iBuf.roi.lcd_y = req->dst_rect.y;
	iBuf.roi.dst_width = req->dst_rect.w;
	iBuf.roi.dst_height = req->dst_rect.h;

	iBuf.roi.x = req->src_rect.x;
	iBuf.roi.width = req->src_rect.w;
	iBuf.roi.y = req->src_rect.y;
	iBuf.roi.height = req->src_rect.h;

	iBuf.mdpImg.width = req->src.width;
	iBuf.mdpImg.imgType = req->src.format;


	iBuf.mdpImg.bmy_addr = (uint32 *) (srcp0_start + req->src.offset);
	if (iBuf.mdpImg.imgType == MDP_Y_CBCR_H2V2_ADRENO)
		iBuf.mdpImg.cbcr_addr =
			(uint32 *) ((uint32) iBuf.mdpImg.bmy_addr +
				ALIGN((ALIGN(req->src.width, 32) *
				ALIGN(req->src.height, 32)), 4096));
	else
		iBuf.mdpImg.cbcr_addr = srcp1_start ? (uint32 *)srcp1_start :
			(uint32 *) ((uint32) iBuf.mdpImg.bmy_addr +
			req->src.width * req->src.height);

	iBuf.mdpImg.mdpOp = MDPOP_NOP;

<<<<<<< HEAD
=======
	if (req->flags & MDP_IS_FG)
		iBuf.mdpImg.mdpOp |= MDPOP_LAYER_IS_FG;

>>>>>>> refs/remotes/origin/cm-10.0
	/* blending check */
	if (req->transp_mask != MDP_TRANSP_NOP) {
		iBuf.mdpImg.mdpOp |= MDPOP_TRANSP;
		iBuf.mdpImg.tpVal = req->transp_mask;
		iBuf.mdpImg.tpVal = mdp_calc_tpval(&iBuf.mdpImg);
	} else {
		iBuf.mdpImg.tpVal = 0;
	}

	req->alpha &= 0xff;
	if (req->alpha < MDP_ALPHA_NOP) {
		iBuf.mdpImg.mdpOp |= MDPOP_ALPHAB;
		iBuf.mdpImg.alpha = req->alpha;
	} else {
		iBuf.mdpImg.alpha = 0xff;
	}

	/* rotation check */
	if (req->flags & MDP_FLIP_LR)
		iBuf.mdpImg.mdpOp |= MDPOP_LR;
	if (req->flags & MDP_FLIP_UD)
		iBuf.mdpImg.mdpOp |= MDPOP_UD;
	if (req->flags & MDP_ROT_90)
		iBuf.mdpImg.mdpOp |= MDPOP_ROT90;
	if (req->flags & MDP_DITHER)
		iBuf.mdpImg.mdpOp |= MDPOP_DITHER;

	if (req->flags & MDP_BLEND_FG_PREMULT) {
#if defined(CONFIG_FB_MSM_MDP31) || defined(CONFIG_FB_MSM_MDP303)
		iBuf.mdpImg.mdpOp |= MDPOP_FG_PM_ALPHA;
#else
		put_img(p_src_file, *src_ihdl);
		put_img(p_dst_file, *dst_ihdl);
		return -EINVAL;
#endif
	}

	if (req->flags & MDP_DEINTERLACE) {
#ifdef CONFIG_FB_MSM_MDP31
		if ((req->src.format != MDP_Y_CBCR_H2V2) &&
			(req->src.format != MDP_Y_CRCB_H2V2)) {
#endif
			put_img(p_src_file, *src_ihdl);
			put_img(p_dst_file, *dst_ihdl);
			return -EINVAL;
#ifdef CONFIG_FB_MSM_MDP31
		}
#endif
	}

	/* scale check */
	if (req->flags & MDP_ROT_90) {
		dst_width = req->dst_rect.h;
		dst_height = req->dst_rect.w;
	} else {
		dst_width = req->dst_rect.w;
		dst_height = req->dst_rect.h;
	}

	if ((iBuf.roi.width != dst_width) || (iBuf.roi.height != dst_height))
		iBuf.mdpImg.mdpOp |= MDPOP_ASCALE;

	if (req->flags & MDP_BLUR) {
#ifdef CONFIG_FB_MSM_MDP31
		if (req->flags & MDP_SHARPENING)
			printk(KERN_WARNING
				"mdp: MDP_SHARPENING is set with MDP_BLUR!\n");
		req->flags |= MDP_SHARPENING;
		req->sharpening_strength = -127;
#else
		iBuf.mdpImg.mdpOp |= MDPOP_ASCALE | MDPOP_BLUR;

#endif
	}

	if (req->flags & MDP_SHARPENING) {
#ifdef CONFIG_FB_MSM_MDP31
		if ((req->sharpening_strength > 127) ||
			(req->sharpening_strength < -127)) {
			printk(KERN_ERR
				"%s: sharpening strength out of range\n",
				__func__);
			put_img(p_src_file, *src_ihdl);
			put_img(p_dst_file, *dst_ihdl);
			return -EINVAL;
		}

		iBuf.mdpImg.mdpOp |= MDPOP_ASCALE | MDPOP_SHARPENING;
		iBuf.mdpImg.sp_value = req->sharpening_strength & 0xff;
#else
		put_img(p_src_file, *src_ihdl);
		put_img(p_dst_file, *dst_ihdl);
		return -EINVAL;
#endif
	}

	down(&mdp_ppp_mutex);
	/* MDP cmd block enable */
	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);

#ifndef CONFIG_FB_MSM_MDP22
	mdp_start_ppp(mfd, &iBuf, req, p_src_file, p_dst_file);
#else
	/* bg tile fetching HW workaround */
	if (((iBuf.mdpImg.mdpOp & (MDPOP_TRANSP | MDPOP_ALPHAB)) ||
	     (req->src.format == MDP_ARGB_8888) ||
	     (req->src.format == MDP_BGRA_8888) ||
	     (req->src.format == MDP_RGBA_8888)) &&
	    (iBuf.mdpImg.mdpOp & MDPOP_ROT90) && (req->dst_rect.w <= 16)) {
		int dst_h, src_w, i;
		uint32 mdpOp = iBuf.mdpImg.mdpOp;

		src_w = req->src_rect.w;
		dst_h = iBuf.roi.dst_height;

		for (i = 0; i < (req->dst_rect.h / 16); i++) {
			/* this tile size */
			iBuf.roi.dst_height = 16;
			iBuf.roi.width =
			    (16 * req->src_rect.w) / req->dst_rect.h;

			/* if it's out of scale range... */
			if (((MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) /
			     iBuf.roi.width) > MDP_MAX_X_SCALE_FACTOR)
				iBuf.roi.width =
				    (MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) /
				    MDP_MAX_X_SCALE_FACTOR;
			else if (((MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) /
				  iBuf.roi.width) < MDP_MIN_X_SCALE_FACTOR)
				iBuf.roi.width =
				    (MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) /
				    MDP_MIN_X_SCALE_FACTOR;

			mdp_start_ppp(mfd, &iBuf, req, p_src_file, p_dst_file);

			/* next tile location */
			iBuf.roi.lcd_y += 16;
			iBuf.roi.x += iBuf.roi.width;

			/* this is for a remainder update */
			dst_h -= 16;
			src_w -= iBuf.roi.width;
			/* restore mdpOp since MDPOP_ASCALE have been cleared */
			iBuf.mdpImg.mdpOp = mdpOp;
		}

		if ((dst_h < 0) || (src_w < 0))
			printk
			    ("msm_fb: mdp_blt_ex() unexpected result! line:%d\n",
			     __LINE__);

		/* remainder update */
		if ((dst_h > 0) && (src_w > 0)) {
			u32 tmp_v;

			iBuf.roi.dst_height = dst_h;
			iBuf.roi.width = src_w;

			if (((MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) /
			     iBuf.roi.width) > MDP_MAX_X_SCALE_FACTOR) {
				tmp_v =
				    (MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) /
				    MDP_MAX_X_SCALE_FACTOR +
				    (MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) %
				    MDP_MAX_X_SCALE_FACTOR ? 1 : 0;

				/* move x location as roi width gets bigger */
				iBuf.roi.x -= tmp_v - iBuf.roi.width;
				iBuf.roi.width = tmp_v;
			} else
			    if (((MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) /
				 iBuf.roi.width) < MDP_MIN_X_SCALE_FACTOR) {
				tmp_v =
				    (MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) /
				    MDP_MIN_X_SCALE_FACTOR +
				    (MDP_SCALE_Q_FACTOR * iBuf.roi.dst_height) %
				    MDP_MIN_X_SCALE_FACTOR ? 1 : 0;

				/*
				 * we don't move x location for continuity of
				 * source image
				 */
				iBuf.roi.width = tmp_v;
			}

			mdp_start_ppp(mfd, &iBuf, req, p_src_file, p_dst_file);
		}
	} else {
		mdp_start_ppp(mfd, &iBuf, req, p_src_file, p_dst_file);
	}
#endif

	/* MDP cmd block disable */
	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);
	up(&mdp_ppp_mutex);

	put_img(p_src_file, *src_ihdl);
	put_img(p_dst_file, *dst_ihdl);
	return 0;
}

int mdp_ppp_blit(struct fb_info *info, struct mdp_blit_req *req)
{
	unsigned long src_start, dst_start;
	unsigned long src_len = 0;
	unsigned long dst_len = 0;
	struct file *p_src_file = 0 , *p_dst_file = 0;
	struct ion_handle *src_ihdl = NULL;
	struct ion_handle *dst_ihdl = NULL;
	struct msm_fb_data_type *mfd = info->par;
	ppp_display_iclient = mfd->iclient;

	if (req->flags & MDP_BLIT_SRC_GEM)
		get_gem_img(&req->src, &src_start, &src_len);
	else
		get_img(&req->src, req, info, &src_start, &src_len, &p_src_file,
			&src_ihdl);
	if (src_len == 0) {
		pr_err("mdp_ppp: could not retrieve source image from "
		       "memory\n");
		return -EINVAL;
	}
	if (req->flags & MDP_BLIT_DST_GEM)
		get_gem_img(&req->dst, &dst_start, &dst_len);
	else
		get_img(&req->dst, req, info, &dst_start, &dst_len, &p_dst_file,
			&dst_ihdl);

	if (dst_len == 0) {
		put_img(p_src_file, src_ihdl);
		pr_err("mdp_ppp: could not retrieve destination image from "
		       "memory\n");
		return -EINVAL;
	}

	return mdp_ppp_blit_addr(info, req, src_start, src_len, 0, 0, dst_start,
		dst_len, p_src_file, p_dst_file, &src_ihdl, &dst_ihdl);
}

static struct mdp_blit_req overlay_req;
static bool mdp_overlay_req_set;

int mdp_ppp_v4l2_overlay_set(struct fb_info *info, struct mdp_overlay *req)
{
	memset(&overlay_req, 0, sizeof(struct mdp_blit_req));

	overlay_req.src.width  = req->src.width;
	overlay_req.src.height = req->src.height;
	overlay_req.src.format = req->src.format;


	overlay_req.dst.width  = req->dst_rect.w;
	overlay_req.dst.height = req->dst_rect.h;
	overlay_req.dst.format = MDP_FB_FORMAT;
	overlay_req.transp_mask = req->transp_mask;
	overlay_req.flags = req->flags;
	overlay_req.alpha = req->alpha;

	overlay_req.src_rect.x = req->src_rect.x;
	overlay_req.src_rect.y = req->src_rect.y;
	overlay_req.src_rect.w = req->src_rect.w;
	overlay_req.src_rect.h = req->src_rect.h;
	overlay_req.dst_rect.x = req->dst_rect.x;
	overlay_req.dst_rect.y = req->dst_rect.y;
	overlay_req.dst_rect.w = req->dst_rect.w;
	overlay_req.dst_rect.h = req->dst_rect.h;
	mdp_overlay_req_set = true;

	pr_debug("%s: Overlay parameters:", __func__);
	pr_debug("Src_Image (%u %u)\n", overlay_req.src.width,
	overlay_req.src.height);

	if (overlay_req.src.format == MDP_Y_CRCB_H2V2)
		pr_debug("Overlay format MDP_Y_CRCB_H2V2\n");
	else if (overlay_req.src.format == MDP_RGB_565)
		pr_debug("Overlay format MDP_RGB_565\n");
	else
		pr_debug("Overlay format(%u) unknown\n",
		overlay_req.src.format);

	pr_debug("Dst_Image (%u %u)\n", overlay_req.dst.width,
		overlay_req.dst.height);
	pr_debug("Src rect: (%u,%u,%u,%u), Dst rect: (%u,%u,%u,%u)\n",
		overlay_req.src_rect.x, overlay_req.src_rect.y,
		overlay_req.src_rect.w, overlay_req.src_rect.h,
		overlay_req.dst_rect.x, overlay_req.dst_rect.y,
		overlay_req.dst_rect.w, overlay_req.dst_rect.h);
	return 0;
}

int mdp_ppp_v4l2_overlay_clear(void)
{
	memset(&overlay_req, 0, sizeof(struct mdp_overlay));
	mdp_overlay_req_set = false;
	return 0;
}

int mdp_ppp_v4l2_overlay_play(struct fb_info *info,
	unsigned long srcp0_addr, unsigned long srcp0_size,
	unsigned long srcp1_addr, unsigned long srcp1_size)
{
	int ret;

	if (!mdp_overlay_req_set) {
		pr_err("mdp_ppp:v4l2:No overlay set, ignore play req\n");
		return -EINVAL;
	}

	overlay_req.dst.width = info->var.xres;
	overlay_req.dst.height = info->var.yres;

	ret = mdp_ppp_blit_addr(info, &overlay_req,
		srcp0_addr, srcp0_size, srcp1_addr, srcp1_size,
		info->fix.smem_start, info->fix.smem_len, NULL, NULL,
		NULL, NULL);

	if (ret)
		pr_err("%s:Blitting overlay failed(%d)\n", __func__, ret);

	return ret;
=======
	[MDP_Y_CRCB_H2V1] = 1,
	[MDP_Y_CRCB_H2V2] = 1,
	[MDP_YCRYCB_H2V1] = 2
};

static uint32_t dst_op_chroma[] = {
	PPP_ARRAY1(CHROMA_SAMP, DST)
};

static uint32_t src_op_chroma[] = {
	PPP_ARRAY1(CHROMA_SAMP, SRC)
};

static uint32_t bg_op_chroma[] = {
	PPP_ARRAY1(CHROMA_SAMP, BG)
};

static void rotate_dst_addr_x(struct mdp_blit_req *req, struct mdp_regs *regs)
{
	regs->dst0 += (req->dst_rect.w -
		       min((uint32_t)16, req->dst_rect.w)) * regs->dst_bpp;
	regs->dst1 += (req->dst_rect.w -
		       min((uint32_t)16, req->dst_rect.w)) * regs->dst_bpp;
}

static void rotate_dst_addr_y(struct mdp_blit_req *req, struct mdp_regs *regs)
{
	regs->dst0 += (req->dst_rect.h -
		       min((uint32_t)16, req->dst_rect.h)) *
		       regs->dst_ystride;
	regs->dst1 += (req->dst_rect.h -
		       min((uint32_t)16, req->dst_rect.h)) *
		       regs->dst_ystride;
}

static void blit_rotate(struct mdp_blit_req *req,
			struct mdp_regs *regs)
{
	if (req->flags == MDP_ROT_NOP)
		return;

	regs->op |= PPP_OP_ROT_ON;
	if ((req->flags & MDP_ROT_90 || req->flags & MDP_FLIP_LR) &&
	    !(req->flags & MDP_ROT_90 && req->flags & MDP_FLIP_LR))
		rotate_dst_addr_x(req, regs);
	if (req->flags & MDP_ROT_90)
		regs->op |= PPP_OP_ROT_90;
	if (req->flags & MDP_FLIP_UD) {
		regs->op |= PPP_OP_FLIP_UD;
		rotate_dst_addr_y(req, regs);
	}
	if (req->flags & MDP_FLIP_LR)
		regs->op |= PPP_OP_FLIP_LR;
}

static void blit_convert(struct mdp_blit_req *req, struct mdp_regs *regs)
{
	if (req->src.format == req->dst.format)
		return;
	if (IS_RGB(req->src.format) && IS_YCRCB(req->dst.format)) {
		regs->op |= PPP_OP_CONVERT_RGB2YCBCR | PPP_OP_CONVERT_ON;
	} else if (IS_YCRCB(req->src.format) && IS_RGB(req->dst.format)) {
		regs->op |= PPP_OP_CONVERT_YCBCR2RGB | PPP_OP_CONVERT_ON;
		if (req->dst.format == MDP_RGB_565)
			regs->op |= PPP_OP_CONVERT_MATRIX_SECONDARY;
	}
}

#define GET_BIT_RANGE(value, high, low) \
	(((1 << (high - low + 1)) - 1) & (value >> low))
static uint32_t transp_convert(struct mdp_blit_req *req)
{
	uint32_t transp = 0;
	if (req->src.format == MDP_RGB_565) {
		/* pad each value to 8 bits by copying the high bits into the
		 * low end, convert RGB to RBG by switching low 2 components */
		transp |= ((GET_BIT_RANGE(req->transp_mask, 15, 11) << 3) |
			   (GET_BIT_RANGE(req->transp_mask, 15, 13))) << 16;

		transp |= ((GET_BIT_RANGE(req->transp_mask, 4, 0) << 3) |
			   (GET_BIT_RANGE(req->transp_mask, 4, 2))) << 8;

		transp |= (GET_BIT_RANGE(req->transp_mask, 10, 5) << 2) |
			  (GET_BIT_RANGE(req->transp_mask, 10, 9));
	} else {
		/* convert RGB to RBG */
		transp |= (GET_BIT_RANGE(req->transp_mask, 15, 8)) |
			  (GET_BIT_RANGE(req->transp_mask, 23, 16) << 16) |
			  (GET_BIT_RANGE(req->transp_mask, 7, 0) << 8);
	}
	return transp;
}
#undef GET_BIT_RANGE

static void blit_blend(struct mdp_blit_req *req, struct mdp_regs *regs)
{
	/* TRANSP BLEND */
	if (req->transp_mask != MDP_TRANSP_NOP) {
		req->transp_mask = transp_convert(req);
		if (req->alpha != MDP_ALPHA_NOP) {
			/* use blended transparancy mode
			 * pixel = (src == transp) ? dst : blend
			 * blend is combo of blend_eq_sel and
			 * blend_alpha_sel */
			regs->op |= PPP_OP_ROT_ON | PPP_OP_BLEND_ON |
				PPP_OP_BLEND_ALPHA_BLEND_NORMAL |
				PPP_OP_BLEND_CONSTANT_ALPHA |
				PPP_BLEND_ALPHA_TRANSP;
		} else {
			/* simple transparancy mode
			 * pixel = (src == transp) ? dst : src */
			regs->op |= PPP_OP_ROT_ON | PPP_OP_BLEND_ON |
				PPP_OP_BLEND_SRCPIXEL_TRANSP;
		}
	}

	req->alpha &= 0xff;
	/* ALPHA BLEND */
	if (HAS_ALPHA(req->src.format)) {
		regs->op |= PPP_OP_ROT_ON | PPP_OP_BLEND_ON |
			PPP_OP_BLEND_SRCPIXEL_ALPHA;
	} else if (req->alpha < MDP_ALPHA_NOP) {
		/* just blend by alpha */
		regs->op |= PPP_OP_ROT_ON | PPP_OP_BLEND_ON |
			PPP_OP_BLEND_ALPHA_BLEND_NORMAL |
			PPP_OP_BLEND_CONSTANT_ALPHA;
	}

	regs->op |= bg_op_chroma[req->dst.format];
}

#define ONE_HALF	(1LL << 32)
#define ONE		(1LL << 33)
#define TWO		(2LL << 33)
#define THREE		(3LL << 33)
#define FRAC_MASK (ONE - 1)
#define INT_MASK (~FRAC_MASK)

static int scale_params(uint32_t dim_in, uint32_t dim_out, uint32_t origin,
			uint32_t *phase_init, uint32_t *phase_step)
{
	/* to improve precicsion calculations are done in U31.33 and converted
	 * to U3.29 at the end */
	int64_t k1, k2, k3, k4, tmp;
	uint64_t n, d, os, os_p, od, od_p, oreq;
	unsigned rpa = 0;
	int64_t ip64, delta;

	if (dim_out % 3 == 0)
		rpa = !(dim_in % (dim_out / 3));

	n = ((uint64_t)dim_out) << 34;
	d = dim_in;
	if (!d)
		return -1;
	do_div(n, d);
	k3 = (n + 1) >> 1;
	if ((k3 >> 4) < (1LL << 27) || (k3 >> 4) > (1LL << 31)) {
		DLOG("crap bad scale\n");
		return -1;
	}
	n = ((uint64_t)dim_in) << 34;
	d = (uint64_t)dim_out;
	if (!d)
		return -1;
	do_div(n, d);
	k1 = (n + 1) >> 1;
	k2 = (k1 - ONE) >> 1;

	*phase_init = (int)(k2 >> 4);
	k4 = (k3 - ONE) >> 1;

	if (rpa) {
		os = ((uint64_t)origin << 33) - ONE_HALF;
		tmp = (dim_out * os) + ONE_HALF;
		if (!dim_in)
			return -1;
		do_div(tmp, dim_in);
		od = tmp - ONE_HALF;
	} else {
		os = ((uint64_t)origin << 1) - 1;
		od = (((k3 * os) >> 1) + k4);
	}

	od_p = od & INT_MASK;
	if (od_p != od)
		od_p += ONE;

	if (rpa) {
		tmp = (dim_in * od_p) + ONE_HALF;
		if (!dim_in)
			return -1;
		do_div(tmp, dim_in);
		os_p = tmp - ONE_HALF;
	} else {
		os_p = ((k1 * (od_p >> 33)) + k2);
	}

	oreq = (os_p & INT_MASK) - ONE;

	ip64 = os_p - oreq;
	delta = ((int64_t)(origin) << 33) - oreq;
	ip64 -= delta;
	/* limit to valid range before the left shift */
	delta = (ip64 & (1LL << 63)) ? 4 : -4;
	delta <<= 33;
	while (abs((int)(ip64 >> 33)) > 4)
		ip64 += delta;
	*phase_init = (int)(ip64 >> 4);
	*phase_step = (uint32_t)(k1 >> 4);
	return 0;
}

static void load_scale_table(const struct mdp_info *mdp,
			     struct mdp_table_entry *table, int len)
{
	int i;
	for (i = 0; i < len; i++)
		mdp_writel(mdp, table[i].val, table[i].reg);
}

enum {
IMG_LEFT,
IMG_RIGHT,
IMG_TOP,
IMG_BOTTOM,
};

static void get_edge_info(uint32_t src, uint32_t src_coord, uint32_t dst,
			  uint32_t *interp1, uint32_t *interp2,
			  uint32_t *repeat1, uint32_t *repeat2) {
	if (src > 3 * dst) {
		*interp1 = 0;
		*interp2 = src - 1;
		*repeat1 = 0;
		*repeat2 = 0;
	} else if (src == 3 * dst) {
		*interp1 = 0;
		*interp2 = src;
		*repeat1 = 0;
		*repeat2 = 1;
	} else if (src > dst && src < 3 * dst) {
		*interp1 = -1;
		*interp2 = src;
		*repeat1 = 1;
		*repeat2 = 1;
	} else if (src == dst) {
		*interp1 = -1;
		*interp2 = src + 1;
		*repeat1 = 1;
		*repeat2 = 2;
	} else {
		*interp1 = -2;
		*interp2 = src + 1;
		*repeat1 = 2;
		*repeat2 = 2;
	}
	*interp1 += src_coord;
	*interp2 += src_coord;
}

static int get_edge_cond(struct mdp_blit_req *req, struct mdp_regs *regs)
{
	int32_t luma_interp[4];
	int32_t luma_repeat[4];
	int32_t chroma_interp[4];
	int32_t chroma_bound[4];
	int32_t chroma_repeat[4];
	uint32_t dst_w, dst_h;

	memset(&luma_interp, 0, sizeof(int32_t) * 4);
	memset(&luma_repeat, 0, sizeof(int32_t) * 4);
	memset(&chroma_interp, 0, sizeof(int32_t) * 4);
	memset(&chroma_bound, 0, sizeof(int32_t) * 4);
	memset(&chroma_repeat, 0, sizeof(int32_t) * 4);
	regs->edge = 0;

	if (req->flags & MDP_ROT_90) {
		dst_w = req->dst_rect.h;
		dst_h = req->dst_rect.w;
	} else {
		dst_w = req->dst_rect.w;
		dst_h = req->dst_rect.h;
	}

	if (regs->op & (PPP_OP_SCALE_Y_ON | PPP_OP_SCALE_X_ON)) {
		get_edge_info(req->src_rect.h, req->src_rect.y, dst_h,
			      &luma_interp[IMG_TOP], &luma_interp[IMG_BOTTOM],
			      &luma_repeat[IMG_TOP], &luma_repeat[IMG_BOTTOM]);
		get_edge_info(req->src_rect.w, req->src_rect.x, dst_w,
			      &luma_interp[IMG_LEFT], &luma_interp[IMG_RIGHT],
			      &luma_repeat[IMG_LEFT], &luma_repeat[IMG_RIGHT]);
	} else {
		luma_interp[IMG_LEFT] = req->src_rect.x;
		luma_interp[IMG_RIGHT] = req->src_rect.x + req->src_rect.w - 1;
		luma_interp[IMG_TOP] = req->src_rect.y;
		luma_interp[IMG_BOTTOM] = req->src_rect.y + req->src_rect.h - 1;
		luma_repeat[IMG_LEFT] = 0;
		luma_repeat[IMG_TOP] = 0;
		luma_repeat[IMG_RIGHT] = 0;
		luma_repeat[IMG_BOTTOM] = 0;
	}

	chroma_interp[IMG_LEFT] = luma_interp[IMG_LEFT];
	chroma_interp[IMG_RIGHT] = luma_interp[IMG_RIGHT];
	chroma_interp[IMG_TOP] = luma_interp[IMG_TOP];
	chroma_interp[IMG_BOTTOM] = luma_interp[IMG_BOTTOM];

	chroma_bound[IMG_LEFT] = req->src_rect.x;
	chroma_bound[IMG_RIGHT] = req->src_rect.x + req->src_rect.w - 1;
	chroma_bound[IMG_TOP] = req->src_rect.y;
	chroma_bound[IMG_BOTTOM] = req->src_rect.y + req->src_rect.h - 1;

	if (IS_YCRCB(req->src.format)) {
		chroma_interp[IMG_LEFT] = chroma_interp[IMG_LEFT] >> 1;
		chroma_interp[IMG_RIGHT] = (chroma_interp[IMG_RIGHT] + 1) >> 1;

		chroma_bound[IMG_LEFT] = chroma_bound[IMG_LEFT] >> 1;
		chroma_bound[IMG_RIGHT] = chroma_bound[IMG_RIGHT] >> 1;
	}

	if (req->src.format == MDP_Y_CBCR_H2V2 ||
	    req->src.format == MDP_Y_CRCB_H2V2) {
		chroma_interp[IMG_TOP] = (chroma_interp[IMG_TOP] - 1) >> 1;
		chroma_interp[IMG_BOTTOM] = (chroma_interp[IMG_BOTTOM] + 1)
					    >> 1;
		chroma_bound[IMG_TOP] = (chroma_bound[IMG_TOP] + 1) >> 1;
		chroma_bound[IMG_BOTTOM] = chroma_bound[IMG_BOTTOM] >> 1;
	}

	chroma_repeat[IMG_LEFT] = chroma_bound[IMG_LEFT] -
				  chroma_interp[IMG_LEFT];
	chroma_repeat[IMG_RIGHT] = chroma_interp[IMG_RIGHT] -
				  chroma_bound[IMG_RIGHT];
	chroma_repeat[IMG_TOP] = chroma_bound[IMG_TOP] -
				  chroma_interp[IMG_TOP];
	chroma_repeat[IMG_BOTTOM] = chroma_interp[IMG_BOTTOM] -
				  chroma_bound[IMG_BOTTOM];

	if (chroma_repeat[IMG_LEFT] < 0 || chroma_repeat[IMG_LEFT] > 3 ||
	    chroma_repeat[IMG_RIGHT] < 0 || chroma_repeat[IMG_RIGHT] > 3 ||
	    chroma_repeat[IMG_TOP] < 0 || chroma_repeat[IMG_TOP] > 3 ||
	    chroma_repeat[IMG_BOTTOM] < 0 || chroma_repeat[IMG_BOTTOM] > 3 ||
	    luma_repeat[IMG_LEFT] < 0 || luma_repeat[IMG_LEFT] > 3 ||
	    luma_repeat[IMG_RIGHT] < 0 || luma_repeat[IMG_RIGHT] > 3 ||
	    luma_repeat[IMG_TOP] < 0 || luma_repeat[IMG_TOP] > 3 ||
	    luma_repeat[IMG_BOTTOM] < 0 || luma_repeat[IMG_BOTTOM] > 3)
		return -1;

	regs->edge |= (chroma_repeat[IMG_LEFT] & 3) << MDP_LEFT_CHROMA;
	regs->edge |= (chroma_repeat[IMG_RIGHT] & 3) << MDP_RIGHT_CHROMA;
	regs->edge |= (chroma_repeat[IMG_TOP] & 3) << MDP_TOP_CHROMA;
	regs->edge |= (chroma_repeat[IMG_BOTTOM] & 3) << MDP_BOTTOM_CHROMA;
	regs->edge |= (luma_repeat[IMG_LEFT] & 3) << MDP_LEFT_LUMA;
	regs->edge |= (luma_repeat[IMG_RIGHT] & 3) << MDP_RIGHT_LUMA;
	regs->edge |= (luma_repeat[IMG_TOP] & 3) << MDP_TOP_LUMA;
	regs->edge |= (luma_repeat[IMG_BOTTOM] & 3) << MDP_BOTTOM_LUMA;
	return 0;
}

static int blit_scale(const struct mdp_info *mdp, struct mdp_blit_req *req,
		      struct mdp_regs *regs)
{
	uint32_t phase_init_x, phase_init_y, phase_step_x, phase_step_y;
	uint32_t scale_factor_x, scale_factor_y;
	uint32_t downscale;
	uint32_t dst_w, dst_h;

	if (req->flags & MDP_ROT_90) {
		dst_w = req->dst_rect.h;
		dst_h = req->dst_rect.w;
	} else {
		dst_w = req->dst_rect.w;
		dst_h = req->dst_rect.h;
	}
	if ((req->src_rect.w == dst_w)  && (req->src_rect.h == dst_h) &&
	    !(req->flags & MDP_BLUR)) {
		regs->phasex_init = 0;
		regs->phasey_init = 0;
		regs->phasex_step = 0;
		regs->phasey_step = 0;
		return 0;
	}

	if (scale_params(req->src_rect.w, dst_w, 1, &phase_init_x,
			 &phase_step_x) ||
	    scale_params(req->src_rect.h, dst_h, 1, &phase_init_y,
			 &phase_step_y))
		return -1;

	scale_factor_x = (dst_w * 10) / req->src_rect.w;
	scale_factor_y = (dst_h * 10) / req->src_rect.h;

	if (scale_factor_x > 8)
		downscale = MDP_DOWNSCALE_PT8TO1;
	else if (scale_factor_x > 6)
		downscale = MDP_DOWNSCALE_PT6TOPT8;
	else if (scale_factor_x > 4)
		downscale = MDP_DOWNSCALE_PT4TOPT6;
	else
		downscale = MDP_DOWNSCALE_PT2TOPT4;
	if (downscale != downscale_x_table) {
		load_scale_table(mdp, mdp_downscale_x_table[downscale], 64);
		downscale_x_table = downscale;
	}

	if (scale_factor_y > 8)
		downscale = MDP_DOWNSCALE_PT8TO1;
	else if (scale_factor_y > 6)
		downscale = MDP_DOWNSCALE_PT6TOPT8;
	else if (scale_factor_y > 4)
		downscale = MDP_DOWNSCALE_PT4TOPT6;
	else
		downscale = MDP_DOWNSCALE_PT2TOPT4;
	if (downscale != downscale_y_table) {
		load_scale_table(mdp, mdp_downscale_y_table[downscale], 64);
		downscale_y_table = downscale;
	}

	regs->phasex_init = phase_init_x;
	regs->phasey_init = phase_init_y;
	regs->phasex_step = phase_step_x;
	regs->phasey_step = phase_step_y;
	regs->op |= (PPP_OP_SCALE_Y_ON | PPP_OP_SCALE_X_ON);
	return 0;

}

static void blit_blur(const struct mdp_info *mdp, struct mdp_blit_req *req,
		      struct mdp_regs *regs)
{
	if (!(req->flags & MDP_BLUR))
		return;

	if (!(downscale_x_table == MDP_DOWNSCALE_BLUR &&
	      downscale_y_table == MDP_DOWNSCALE_BLUR)) {
		load_scale_table(mdp, mdp_gaussian_blur_table, 128);
		downscale_x_table = MDP_DOWNSCALE_BLUR;
		downscale_y_table = MDP_DOWNSCALE_BLUR;
	}

	regs->op |= (PPP_OP_SCALE_Y_ON | PPP_OP_SCALE_X_ON);
}


#define IMG_LEN(rect_h, w, rect_w, bpp) (((rect_h) * w) * bpp)

#define Y_TO_CRCB_RATIO(format) \
	((format == MDP_Y_CBCR_H2V2 || format == MDP_Y_CRCB_H2V2) ?  2 :\
	 (format == MDP_Y_CBCR_H2V1 || format == MDP_Y_CRCB_H2V1) ?  1 : 1)

static void get_len(struct mdp_img *img, struct mdp_rect *rect, uint32_t bpp,
		    uint32_t *len0, uint32_t *len1)
{
	*len0 = IMG_LEN(rect->h, img->width, rect->w, bpp);
	if (IS_PSEUDOPLNR(img->format))
		*len1 = *len0/Y_TO_CRCB_RATIO(img->format);
	else
		*len1 = 0;
}

static int valid_src_dst(unsigned long src_start, unsigned long src_len,
			 unsigned long dst_start, unsigned long dst_len,
			 struct mdp_blit_req *req, struct mdp_regs *regs)
{
	unsigned long src_min_ok = src_start;
	unsigned long src_max_ok = src_start + src_len;
	unsigned long dst_min_ok = dst_start;
	unsigned long dst_max_ok = dst_start + dst_len;
	uint32_t src0_len, src1_len, dst0_len, dst1_len;
	get_len(&req->src, &req->src_rect, regs->src_bpp, &src0_len,
		 &src1_len);
	get_len(&req->dst, &req->dst_rect, regs->dst_bpp, &dst0_len,
		 &dst1_len);

	if (regs->src0 < src_min_ok || regs->src0 > src_max_ok ||
	    regs->src0 + src0_len > src_max_ok) {
		DLOG("invalid_src %x %x %lx %lx\n", regs->src0,
		      src0_len, src_min_ok, src_max_ok);
		return 0;
	}
	if (regs->src_cfg & PPP_SRC_PLANE_PSEUDOPLNR) {
		if (regs->src1 < src_min_ok || regs->src1 > src_max_ok ||
		    regs->src1 + src1_len > src_max_ok) {
			DLOG("invalid_src1");
			return 0;
		}
	}
	if (regs->dst0 < dst_min_ok || regs->dst0 > dst_max_ok ||
	    regs->dst0 + dst0_len > dst_max_ok) {
		DLOG("invalid_dst");
		return 0;
	}
	if (regs->dst_cfg & PPP_SRC_PLANE_PSEUDOPLNR) {
		if (regs->dst1 < dst_min_ok || regs->dst1 > dst_max_ok ||
		    regs->dst1 + dst1_len > dst_max_ok) {
			DLOG("invalid_dst1");
			return 0;
		}
	}
	return 1;
}


static void flush_imgs(struct mdp_blit_req *req, struct mdp_regs *regs,
		       struct file *src_file, struct file *dst_file)
{
}

static void get_chroma_addr(struct mdp_img *img, struct mdp_rect *rect,
			    uint32_t base, uint32_t bpp, uint32_t cfg,
			    uint32_t *addr, uint32_t *ystride)
{
	uint32_t compress_v = Y_TO_CRCB_RATIO(img->format);
	uint32_t compress_h = 2;
	uint32_t  offset;

	if (IS_PSEUDOPLNR(img->format)) {
		offset = (rect->x / compress_h) * compress_h;
		offset += rect->y == 0 ? 0 :
			  ((rect->y + 1) / compress_v) * img->width;
		*addr = base + (img->width * img->height * bpp);
		*addr += offset * bpp;
		*ystride |= *ystride << 16;
	} else {
		*addr = 0;
	}
}

static int send_blit(const struct mdp_info *mdp, struct mdp_blit_req *req,
		     struct mdp_regs *regs, struct file *src_file,
		     struct file *dst_file)
{
	mdp_writel(mdp, 1, 0x060);
	mdp_writel(mdp, regs->src_rect, PPP_ADDR_SRC_ROI);
	mdp_writel(mdp, regs->src0, PPP_ADDR_SRC0);
	mdp_writel(mdp, regs->src1, PPP_ADDR_SRC1);
	mdp_writel(mdp, regs->src_ystride, PPP_ADDR_SRC_YSTRIDE);
	mdp_writel(mdp, regs->src_cfg, PPP_ADDR_SRC_CFG);
	mdp_writel(mdp, regs->src_pack, PPP_ADDR_SRC_PACK_PATTERN);

	mdp_writel(mdp, regs->op, PPP_ADDR_OPERATION);
	mdp_writel(mdp, regs->phasex_init, PPP_ADDR_PHASEX_INIT);
	mdp_writel(mdp, regs->phasey_init, PPP_ADDR_PHASEY_INIT);
	mdp_writel(mdp, regs->phasex_step, PPP_ADDR_PHASEX_STEP);
	mdp_writel(mdp, regs->phasey_step, PPP_ADDR_PHASEY_STEP);

	mdp_writel(mdp, (req->alpha << 24) | (req->transp_mask & 0xffffff),
	       PPP_ADDR_ALPHA_TRANSP);

	mdp_writel(mdp, regs->dst_cfg, PPP_ADDR_DST_CFG);
	mdp_writel(mdp, regs->dst_pack, PPP_ADDR_DST_PACK_PATTERN);
	mdp_writel(mdp, regs->dst_rect, PPP_ADDR_DST_ROI);
	mdp_writel(mdp, regs->dst0, PPP_ADDR_DST0);
	mdp_writel(mdp, regs->dst1, PPP_ADDR_DST1);
	mdp_writel(mdp, regs->dst_ystride, PPP_ADDR_DST_YSTRIDE);

	mdp_writel(mdp, regs->edge, PPP_ADDR_EDGE);
	if (regs->op & PPP_OP_BLEND_ON) {
		mdp_writel(mdp, regs->dst0, PPP_ADDR_BG0);
		mdp_writel(mdp, regs->dst1, PPP_ADDR_BG1);
		mdp_writel(mdp, regs->dst_ystride, PPP_ADDR_BG_YSTRIDE);
		mdp_writel(mdp, src_img_cfg[req->dst.format], PPP_ADDR_BG_CFG);
		mdp_writel(mdp, pack_pattern[req->dst.format],
			   PPP_ADDR_BG_PACK_PATTERN);
	}
	flush_imgs(req, regs, src_file, dst_file);
	mdp_writel(mdp, 0x1000, MDP_DISPLAY0_START);
	return 0;
}

int mdp_ppp_blit(const struct mdp_info *mdp, struct mdp_blit_req *req,
		 struct file *src_file, unsigned long src_start, unsigned long src_len,
		 struct file *dst_file, unsigned long dst_start, unsigned long dst_len)
{
	struct mdp_regs regs = {0};

	if (unlikely(req->src.format >= MDP_IMGTYPE_LIMIT ||
		     req->dst.format >= MDP_IMGTYPE_LIMIT)) {
		printk(KERN_ERR "mpd_ppp: img is of wrong format\n");
		return -EINVAL;
	}

	if (unlikely(req->src_rect.x > req->src.width ||
		     req->src_rect.y > req->src.height ||
		     req->dst_rect.x > req->dst.width ||
		     req->dst_rect.y > req->dst.height)) {
		printk(KERN_ERR "mpd_ppp: img rect is outside of img!\n");
		return -EINVAL;
	}

	/* set the src image configuration */
	regs.src_cfg = src_img_cfg[req->src.format];
	regs.src_cfg |= (req->src_rect.x & 0x1) ? PPP_SRC_BPP_ROI_ODD_X : 0;
	regs.src_cfg |= (req->src_rect.y & 0x1) ? PPP_SRC_BPP_ROI_ODD_Y : 0;
	regs.src_rect = (req->src_rect.h << 16) | req->src_rect.w;
	regs.src_pack = pack_pattern[req->src.format];

	/* set the dest image configuration */
	regs.dst_cfg = dst_img_cfg[req->dst.format] | PPP_DST_OUT_SEL_AXI;
	regs.dst_rect = (req->dst_rect.h << 16) | req->dst_rect.w;
	regs.dst_pack = pack_pattern[req->dst.format];

	/* set src, bpp, start pixel and ystride */
	regs.src_bpp = bytes_per_pixel[req->src.format];
	regs.src0 = src_start + req->src.offset;
	regs.src_ystride = req->src.width * regs.src_bpp;
	get_chroma_addr(&req->src, &req->src_rect, regs.src0, regs.src_bpp,
			regs.src_cfg, &regs.src1, &regs.src_ystride);
	regs.src0 += (req->src_rect.x + (req->src_rect.y * req->src.width)) *
		      regs.src_bpp;

	/* set dst, bpp, start pixel and ystride */
	regs.dst_bpp = bytes_per_pixel[req->dst.format];
	regs.dst0 = dst_start + req->dst.offset;
	regs.dst_ystride = req->dst.width * regs.dst_bpp;
	get_chroma_addr(&req->dst, &req->dst_rect, regs.dst0, regs.dst_bpp,
			regs.dst_cfg, &regs.dst1, &regs.dst_ystride);
	regs.dst0 += (req->dst_rect.x + (req->dst_rect.y * req->dst.width)) *
		      regs.dst_bpp;

	if (!valid_src_dst(src_start, src_len, dst_start, dst_len, req,
			   &regs)) {
		printk(KERN_ERR "mpd_ppp: final src or dst location is "
			"invalid, are you trying to make an image too large "
			"or to place it outside the screen?\n");
		return -EINVAL;
	}

	/* set up operation register */
	regs.op = 0;
	blit_rotate(req, &regs);
	blit_convert(req, &regs);
	if (req->flags & MDP_DITHER)
		regs.op |= PPP_OP_DITHER_EN;
	blit_blend(req, &regs);
	if (blit_scale(mdp, req, &regs)) {
		printk(KERN_ERR "mpd_ppp: error computing scale for img.\n");
		return -EINVAL;
	}
	blit_blur(mdp, req, &regs);
	regs.op |= dst_op_chroma[req->dst.format] |
		   src_op_chroma[req->src.format];

	/* if the image is YCRYCB, the x and w must be even */
	if (unlikely(req->src.format == MDP_YCRYCB_H2V1)) {
		req->src_rect.x = req->src_rect.x & (~0x1);
		req->src_rect.w = req->src_rect.w & (~0x1);
		req->dst_rect.x = req->dst_rect.x & (~0x1);
		req->dst_rect.w = req->dst_rect.w & (~0x1);
	}
	if (get_edge_cond(req, &regs))
		return -EINVAL;

	send_blit(mdp, req, &regs, src_file, dst_file);
	return 0;
>>>>>>> refs/remotes/origin/master
}
