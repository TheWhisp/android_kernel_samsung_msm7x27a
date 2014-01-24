/*
 * Copyright (C) 2008 Maarten Maathuis.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __NOUVEAU_ENCODER_H__
#define __NOUVEAU_ENCODER_H__

<<<<<<< HEAD
#include "drm_encoder_slave.h"
#include "nouveau_drv.h"

#define NV_DPMS_CLEARED 0x80

<<<<<<< HEAD
=======
struct dp_train_func {
	void (*link_set)(struct drm_device *, struct dcb_entry *, int crtc,
			 int nr, u32 bw, bool enhframe);
	void (*train_set)(struct drm_device *, struct dcb_entry *, u8 pattern);
	void (*train_adj)(struct drm_device *, struct dcb_entry *,
			  u8 lane, u8 swing, u8 preem);
};

>>>>>>> refs/remotes/origin/cm-10.0
struct nouveau_encoder {
	struct drm_encoder_slave base;

	struct dcb_entry *dcb;
	int or;
=======
#include <subdev/bios/dcb.h>

#include <drm/drm_encoder_slave.h>
#include "dispnv04/disp.h"

#define NV_DPMS_CLEARED 0x80

struct nouveau_i2c_port;

struct nouveau_encoder {
	struct drm_encoder_slave base;

	struct dcb_output *dcb;
	int or;
	struct nouveau_i2c_port *i2c;
>>>>>>> refs/remotes/origin/master

	/* different to drm_encoder.crtc, this reflects what's
	 * actually programmed on the hw, not the proposed crtc */
	struct drm_crtc *crtc;

	struct drm_display_mode mode;
	int last_dpms;

	struct nv04_output_reg restore;

	union {
		struct {
<<<<<<< HEAD
<<<<<<< HEAD
			int mc_unknown;
			uint32_t unk0;
			uint32_t unk1;
			int dpcd_version;
			int link_nr;
			int link_bw;
			bool enhanced_frame;
=======
=======
>>>>>>> refs/remotes/origin/master
			u8  dpcd[8];
			int link_nr;
			int link_bw;
			u32 datarate;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		} dp;
	};
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
struct nouveau_encoder *
find_encoder(struct drm_connector *connector, int type);

>>>>>>> refs/remotes/origin/cm-10.0
=======
struct nouveau_encoder *
find_encoder(struct drm_connector *connector, int type);

>>>>>>> refs/remotes/origin/master
static inline struct nouveau_encoder *nouveau_encoder(struct drm_encoder *enc)
{
	struct drm_encoder_slave *slave = to_encoder_slave(enc);

	return container_of(slave, struct nouveau_encoder, base);
}

static inline struct drm_encoder *to_drm_encoder(struct nouveau_encoder *enc)
{
	return &enc->base.base;
}

static inline struct drm_encoder_slave_funcs *
get_slave_funcs(struct drm_encoder *enc)
{
	return to_encoder_slave(enc)->slave_funcs;
}

<<<<<<< HEAD
<<<<<<< HEAD
struct nouveau_connector *
nouveau_encoder_connector_get(struct nouveau_encoder *encoder);
int nv50_sor_create(struct drm_connector *, struct dcb_entry *);
int nv50_dac_create(struct drm_connector *, struct dcb_entry *);

struct bit_displayport_encoder_table {
	uint32_t match;
	uint8_t  record_nr;
	uint8_t  unknown;
	uint16_t script0;
	uint16_t script1;
	uint16_t unknown_table;
} __attribute__ ((packed));

struct bit_displayport_encoder_table_entry {
	uint8_t vs_level;
	uint8_t pre_level;
	uint8_t reg0;
	uint8_t reg1;
	uint8_t reg2;
} __attribute__ ((packed));
=======
/* nouveau_dp.c */
int nouveau_dp_auxch(struct nouveau_i2c_chan *auxch, int cmd, int addr,
		     uint8_t *data, int data_nr);
bool nouveau_dp_detect(struct drm_encoder *);
void nouveau_dp_dpms(struct drm_encoder *, int mode, u32 datarate,
		     struct dp_train_func *);
u8 *nouveau_dp_bios_data(struct drm_device *, struct dcb_entry *, u8 **);

struct nouveau_connector *
nouveau_encoder_connector_get(struct nouveau_encoder *encoder);
int nv50_sor_create(struct drm_connector *, struct dcb_entry *);
void nv50_sor_dp_calc_tu(struct drm_device *, int, int, u32, u32);
int nv50_dac_create(struct drm_connector *, struct dcb_entry *);

>>>>>>> refs/remotes/origin/cm-10.0
=======
/* nouveau_dp.c */
bool nouveau_dp_detect(struct drm_encoder *);
void nouveau_dp_dpms(struct drm_encoder *, int mode, u32 datarate,
		     struct nouveau_object *);

struct nouveau_connector *
nouveau_encoder_connector_get(struct nouveau_encoder *encoder);
>>>>>>> refs/remotes/origin/master

#endif /* __NOUVEAU_ENCODER_H__ */
