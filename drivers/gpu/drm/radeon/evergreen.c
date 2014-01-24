/*
 * Copyright 2010 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Alex Deucher
 */
#include <linux/firmware.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include "drmP.h"
#include "radeon.h"
#include "radeon_asic.h"
#include "radeon_drm.h"
=======
#include <drm/drmP.h>
#include "radeon.h"
#include "radeon_asic.h"
#include <drm/radeon_drm.h>
>>>>>>> refs/remotes/origin/master
#include "evergreend.h"
#include "atom.h"
#include "avivod.h"
#include "evergreen_reg.h"
#include "evergreen_blit_shaders.h"
<<<<<<< HEAD

#define EVERGREEN_PFP_UCODE_SIZE 1120
#define EVERGREEN_PM4_UCODE_SIZE 1376

<<<<<<< HEAD
<<<<<<< HEAD
static void evergreen_gpu_init(struct radeon_device *rdev);
void evergreen_fini(struct radeon_device *rdev);
static void evergreen_pcie_gen2_enable(struct radeon_device *rdev);
=======
=======
#include "radeon_ucode.h"

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static const u32 crtc_offsets[6] =
{
	EVERGREEN_CRTC0_REGISTER_OFFSET,
	EVERGREEN_CRTC1_REGISTER_OFFSET,
	EVERGREEN_CRTC2_REGISTER_OFFSET,
	EVERGREEN_CRTC3_REGISTER_OFFSET,
	EVERGREEN_CRTC4_REGISTER_OFFSET,
	EVERGREEN_CRTC5_REGISTER_OFFSET
};

<<<<<<< HEAD
<<<<<<< HEAD
static void evergreen_gpu_init(struct radeon_device *rdev);
void evergreen_fini(struct radeon_device *rdev);
void evergreen_pcie_gen2_enable(struct radeon_device *rdev);
extern void cayman_cp_int_cntl_setup(struct radeon_device *rdev,
				     int ring, u32 cp_int_cntl);
=======
#include "clearstate_evergreen.h"

static const u32 sumo_rlc_save_restore_register_list[] =
{
	0x98fc,
	0x9830,
	0x9834,
	0x9838,
	0x9870,
	0x9874,
	0x8a14,
	0x8b24,
	0x8bcc,
	0x8b10,
	0x8d00,
	0x8d04,
	0x8c00,
	0x8c04,
	0x8c08,
	0x8c0c,
	0x8d8c,
	0x8c20,
	0x8c24,
	0x8c28,
	0x8c18,
	0x8c1c,
	0x8cf0,
	0x8e2c,
	0x8e38,
	0x8c30,
	0x9508,
	0x9688,
	0x9608,
	0x960c,
	0x9610,
	0x9614,
	0x88c4,
	0x88d4,
	0xa008,
	0x900c,
	0x9100,
	0x913c,
	0x98f8,
	0x98f4,
	0x9b7c,
	0x3f8c,
	0x8950,
	0x8954,
	0x8a18,
	0x8b28,
	0x9144,
	0x9148,
	0x914c,
	0x3f90,
	0x3f94,
	0x915c,
	0x9160,
	0x9178,
	0x917c,
	0x9180,
	0x918c,
	0x9190,
	0x9194,
	0x9198,
	0x919c,
	0x91a8,
	0x91ac,
	0x91b0,
	0x91b4,
	0x91b8,
	0x91c4,
	0x91c8,
	0x91cc,
	0x91d0,
	0x91d4,
	0x91e0,
	0x91e4,
	0x91ec,
	0x91f0,
	0x91f4,
	0x9200,
	0x9204,
	0x929c,
	0x9150,
	0x802c,
};

=======
>>>>>>> refs/remotes/origin/cm-11.0
static void evergreen_gpu_init(struct radeon_device *rdev);
void evergreen_fini(struct radeon_device *rdev);
void evergreen_pcie_gen2_enable(struct radeon_device *rdev);
void evergreen_program_aspm(struct radeon_device *rdev);
extern void cayman_cp_int_cntl_setup(struct radeon_device *rdev,
				     int ring, u32 cp_int_cntl);
extern void cayman_vm_decode_fault(struct radeon_device *rdev,
				   u32 status, u32 addr);
void cik_init_cp_pg_table(struct radeon_device *rdev);

extern u32 si_get_csb_size(struct radeon_device *rdev);
extern void si_get_csb_buffer(struct radeon_device *rdev, volatile u32 *buffer);
extern u32 cik_get_csb_size(struct radeon_device *rdev);
extern void cik_get_csb_buffer(struct radeon_device *rdev, volatile u32 *buffer);

static const u32 evergreen_golden_registers[] =
{
	0x3f90, 0xffff0000, 0xff000000,
	0x9148, 0xffff0000, 0xff000000,
	0x3f94, 0xffff0000, 0xff000000,
	0x914c, 0xffff0000, 0xff000000,
	0x9b7c, 0xffffffff, 0x00000000,
	0x8a14, 0xffffffff, 0x00000007,
	0x8b10, 0xffffffff, 0x00000000,
	0x960c, 0xffffffff, 0x54763210,
	0x88c4, 0xffffffff, 0x000000c2,
	0x88d4, 0xffffffff, 0x00000010,
	0x8974, 0xffffffff, 0x00000000,
	0xc78, 0x00000080, 0x00000080,
	0x5eb4, 0xffffffff, 0x00000002,
	0x5e78, 0xffffffff, 0x001000f0,
	0x6104, 0x01000300, 0x00000000,
	0x5bc0, 0x00300000, 0x00000000,
	0x7030, 0xffffffff, 0x00000011,
	0x7c30, 0xffffffff, 0x00000011,
	0x10830, 0xffffffff, 0x00000011,
	0x11430, 0xffffffff, 0x00000011,
	0x12030, 0xffffffff, 0x00000011,
	0x12c30, 0xffffffff, 0x00000011,
	0xd02c, 0xffffffff, 0x08421000,
	0x240c, 0xffffffff, 0x00000380,
	0x8b24, 0xffffffff, 0x00ff0fff,
	0x28a4c, 0x06000000, 0x06000000,
	0x10c, 0x00000001, 0x00000001,
	0x8d00, 0xffffffff, 0x100e4848,
	0x8d04, 0xffffffff, 0x00164745,
	0x8c00, 0xffffffff, 0xe4000003,
	0x8c04, 0xffffffff, 0x40600060,
	0x8c08, 0xffffffff, 0x001c001c,
	0x8cf0, 0xffffffff, 0x08e00620,
	0x8c20, 0xffffffff, 0x00800080,
	0x8c24, 0xffffffff, 0x00800080,
	0x8c18, 0xffffffff, 0x20202078,
	0x8c1c, 0xffffffff, 0x00001010,
	0x28350, 0xffffffff, 0x00000000,
	0xa008, 0xffffffff, 0x00010000,
	0x5cc, 0xffffffff, 0x00000001,
	0x9508, 0xffffffff, 0x00000002,
	0x913c, 0x0000000f, 0x0000000a
};

static const u32 evergreen_golden_registers2[] =
{
	0x2f4c, 0xffffffff, 0x00000000,
	0x54f4, 0xffffffff, 0x00000000,
	0x54f0, 0xffffffff, 0x00000000,
	0x5498, 0xffffffff, 0x00000000,
	0x549c, 0xffffffff, 0x00000000,
	0x5494, 0xffffffff, 0x00000000,
	0x53cc, 0xffffffff, 0x00000000,
	0x53c8, 0xffffffff, 0x00000000,
	0x53c4, 0xffffffff, 0x00000000,
	0x53c0, 0xffffffff, 0x00000000,
	0x53bc, 0xffffffff, 0x00000000,
	0x53b8, 0xffffffff, 0x00000000,
	0x53b4, 0xffffffff, 0x00000000,
	0x53b0, 0xffffffff, 0x00000000
};

static const u32 cypress_mgcg_init[] =
{
	0x802c, 0xffffffff, 0xc0000000,
	0x5448, 0xffffffff, 0x00000100,
	0x55e4, 0xffffffff, 0x00000100,
	0x160c, 0xffffffff, 0x00000100,
	0x5644, 0xffffffff, 0x00000100,
	0xc164, 0xffffffff, 0x00000100,
	0x8a18, 0xffffffff, 0x00000100,
	0x897c, 0xffffffff, 0x06000100,
	0x8b28, 0xffffffff, 0x00000100,
	0x9144, 0xffffffff, 0x00000100,
	0x9a60, 0xffffffff, 0x00000100,
	0x9868, 0xffffffff, 0x00000100,
	0x8d58, 0xffffffff, 0x00000100,
	0x9510, 0xffffffff, 0x00000100,
	0x949c, 0xffffffff, 0x00000100,
	0x9654, 0xffffffff, 0x00000100,
	0x9030, 0xffffffff, 0x00000100,
	0x9034, 0xffffffff, 0x00000100,
	0x9038, 0xffffffff, 0x00000100,
	0x903c, 0xffffffff, 0x00000100,
	0x9040, 0xffffffff, 0x00000100,
	0xa200, 0xffffffff, 0x00000100,
	0xa204, 0xffffffff, 0x00000100,
	0xa208, 0xffffffff, 0x00000100,
	0xa20c, 0xffffffff, 0x00000100,
	0x971c, 0xffffffff, 0x00000100,
	0x977c, 0xffffffff, 0x00000100,
	0x3f80, 0xffffffff, 0x00000100,
	0xa210, 0xffffffff, 0x00000100,
	0xa214, 0xffffffff, 0x00000100,
	0x4d8, 0xffffffff, 0x00000100,
	0x9784, 0xffffffff, 0x00000100,
	0x9698, 0xffffffff, 0x00000100,
	0x4d4, 0xffffffff, 0x00000200,
	0x30cc, 0xffffffff, 0x00000100,
	0xd0c0, 0xffffffff, 0xff000100,
	0x802c, 0xffffffff, 0x40000000,
	0x915c, 0xffffffff, 0x00010000,
	0x9160, 0xffffffff, 0x00030002,
	0x9178, 0xffffffff, 0x00070000,
	0x917c, 0xffffffff, 0x00030002,
	0x9180, 0xffffffff, 0x00050004,
	0x918c, 0xffffffff, 0x00010006,
	0x9190, 0xffffffff, 0x00090008,
	0x9194, 0xffffffff, 0x00070000,
	0x9198, 0xffffffff, 0x00030002,
	0x919c, 0xffffffff, 0x00050004,
	0x91a8, 0xffffffff, 0x00010006,
	0x91ac, 0xffffffff, 0x00090008,
	0x91b0, 0xffffffff, 0x00070000,
	0x91b4, 0xffffffff, 0x00030002,
	0x91b8, 0xffffffff, 0x00050004,
	0x91c4, 0xffffffff, 0x00010006,
	0x91c8, 0xffffffff, 0x00090008,
	0x91cc, 0xffffffff, 0x00070000,
	0x91d0, 0xffffffff, 0x00030002,
	0x91d4, 0xffffffff, 0x00050004,
	0x91e0, 0xffffffff, 0x00010006,
	0x91e4, 0xffffffff, 0x00090008,
	0x91e8, 0xffffffff, 0x00000000,
	0x91ec, 0xffffffff, 0x00070000,
	0x91f0, 0xffffffff, 0x00030002,
	0x91f4, 0xffffffff, 0x00050004,
	0x9200, 0xffffffff, 0x00010006,
	0x9204, 0xffffffff, 0x00090008,
	0x9208, 0xffffffff, 0x00070000,
	0x920c, 0xffffffff, 0x00030002,
	0x9210, 0xffffffff, 0x00050004,
	0x921c, 0xffffffff, 0x00010006,
	0x9220, 0xffffffff, 0x00090008,
	0x9224, 0xffffffff, 0x00070000,
	0x9228, 0xffffffff, 0x00030002,
	0x922c, 0xffffffff, 0x00050004,
	0x9238, 0xffffffff, 0x00010006,
	0x923c, 0xffffffff, 0x00090008,
	0x9240, 0xffffffff, 0x00070000,
	0x9244, 0xffffffff, 0x00030002,
	0x9248, 0xffffffff, 0x00050004,
	0x9254, 0xffffffff, 0x00010006,
	0x9258, 0xffffffff, 0x00090008,
	0x925c, 0xffffffff, 0x00070000,
	0x9260, 0xffffffff, 0x00030002,
	0x9264, 0xffffffff, 0x00050004,
	0x9270, 0xffffffff, 0x00010006,
	0x9274, 0xffffffff, 0x00090008,
	0x9278, 0xffffffff, 0x00070000,
	0x927c, 0xffffffff, 0x00030002,
	0x9280, 0xffffffff, 0x00050004,
	0x928c, 0xffffffff, 0x00010006,
	0x9290, 0xffffffff, 0x00090008,
	0x9294, 0xffffffff, 0x00000000,
	0x929c, 0xffffffff, 0x00000001,
	0x802c, 0xffffffff, 0x40010000,
	0x915c, 0xffffffff, 0x00010000,
	0x9160, 0xffffffff, 0x00030002,
	0x9178, 0xffffffff, 0x00070000,
	0x917c, 0xffffffff, 0x00030002,
	0x9180, 0xffffffff, 0x00050004,
	0x918c, 0xffffffff, 0x00010006,
	0x9190, 0xffffffff, 0x00090008,
	0x9194, 0xffffffff, 0x00070000,
	0x9198, 0xffffffff, 0x00030002,
	0x919c, 0xffffffff, 0x00050004,
	0x91a8, 0xffffffff, 0x00010006,
	0x91ac, 0xffffffff, 0x00090008,
	0x91b0, 0xffffffff, 0x00070000,
	0x91b4, 0xffffffff, 0x00030002,
	0x91b8, 0xffffffff, 0x00050004,
	0x91c4, 0xffffffff, 0x00010006,
	0x91c8, 0xffffffff, 0x00090008,
	0x91cc, 0xffffffff, 0x00070000,
	0x91d0, 0xffffffff, 0x00030002,
	0x91d4, 0xffffffff, 0x00050004,
	0x91e0, 0xffffffff, 0x00010006,
	0x91e4, 0xffffffff, 0x00090008,
	0x91e8, 0xffffffff, 0x00000000,
	0x91ec, 0xffffffff, 0x00070000,
	0x91f0, 0xffffffff, 0x00030002,
	0x91f4, 0xffffffff, 0x00050004,
	0x9200, 0xffffffff, 0x00010006,
	0x9204, 0xffffffff, 0x00090008,
	0x9208, 0xffffffff, 0x00070000,
	0x920c, 0xffffffff, 0x00030002,
	0x9210, 0xffffffff, 0x00050004,
	0x921c, 0xffffffff, 0x00010006,
	0x9220, 0xffffffff, 0x00090008,
	0x9224, 0xffffffff, 0x00070000,
	0x9228, 0xffffffff, 0x00030002,
	0x922c, 0xffffffff, 0x00050004,
	0x9238, 0xffffffff, 0x00010006,
	0x923c, 0xffffffff, 0x00090008,
	0x9240, 0xffffffff, 0x00070000,
	0x9244, 0xffffffff, 0x00030002,
	0x9248, 0xffffffff, 0x00050004,
	0x9254, 0xffffffff, 0x00010006,
	0x9258, 0xffffffff, 0x00090008,
	0x925c, 0xffffffff, 0x00070000,
	0x9260, 0xffffffff, 0x00030002,
	0x9264, 0xffffffff, 0x00050004,
	0x9270, 0xffffffff, 0x00010006,
	0x9274, 0xffffffff, 0x00090008,
	0x9278, 0xffffffff, 0x00070000,
	0x927c, 0xffffffff, 0x00030002,
	0x9280, 0xffffffff, 0x00050004,
	0x928c, 0xffffffff, 0x00010006,
	0x9290, 0xffffffff, 0x00090008,
	0x9294, 0xffffffff, 0x00000000,
	0x929c, 0xffffffff, 0x00000001,
	0x802c, 0xffffffff, 0xc0000000
};

static const u32 redwood_mgcg_init[] =
{
	0x802c, 0xffffffff, 0xc0000000,
	0x5448, 0xffffffff, 0x00000100,
	0x55e4, 0xffffffff, 0x00000100,
	0x160c, 0xffffffff, 0x00000100,
	0x5644, 0xffffffff, 0x00000100,
	0xc164, 0xffffffff, 0x00000100,
	0x8a18, 0xffffffff, 0x00000100,
	0x897c, 0xffffffff, 0x06000100,
	0x8b28, 0xffffffff, 0x00000100,
	0x9144, 0xffffffff, 0x00000100,
	0x9a60, 0xffffffff, 0x00000100,
	0x9868, 0xffffffff, 0x00000100,
	0x8d58, 0xffffffff, 0x00000100,
	0x9510, 0xffffffff, 0x00000100,
	0x949c, 0xffffffff, 0x00000100,
	0x9654, 0xffffffff, 0x00000100,
	0x9030, 0xffffffff, 0x00000100,
	0x9034, 0xffffffff, 0x00000100,
	0x9038, 0xffffffff, 0x00000100,
	0x903c, 0xffffffff, 0x00000100,
	0x9040, 0xffffffff, 0x00000100,
	0xa200, 0xffffffff, 0x00000100,
	0xa204, 0xffffffff, 0x00000100,
	0xa208, 0xffffffff, 0x00000100,
	0xa20c, 0xffffffff, 0x00000100,
	0x971c, 0xffffffff, 0x00000100,
	0x977c, 0xffffffff, 0x00000100,
	0x3f80, 0xffffffff, 0x00000100,
	0xa210, 0xffffffff, 0x00000100,
	0xa214, 0xffffffff, 0x00000100,
	0x4d8, 0xffffffff, 0x00000100,
	0x9784, 0xffffffff, 0x00000100,
	0x9698, 0xffffffff, 0x00000100,
	0x4d4, 0xffffffff, 0x00000200,
	0x30cc, 0xffffffff, 0x00000100,
	0xd0c0, 0xffffffff, 0xff000100,
	0x802c, 0xffffffff, 0x40000000,
	0x915c, 0xffffffff, 0x00010000,
	0x9160, 0xffffffff, 0x00030002,
	0x9178, 0xffffffff, 0x00070000,
	0x917c, 0xffffffff, 0x00030002,
	0x9180, 0xffffffff, 0x00050004,
	0x918c, 0xffffffff, 0x00010006,
	0x9190, 0xffffffff, 0x00090008,
	0x9194, 0xffffffff, 0x00070000,
	0x9198, 0xffffffff, 0x00030002,
	0x919c, 0xffffffff, 0x00050004,
	0x91a8, 0xffffffff, 0x00010006,
	0x91ac, 0xffffffff, 0x00090008,
	0x91b0, 0xffffffff, 0x00070000,
	0x91b4, 0xffffffff, 0x00030002,
	0x91b8, 0xffffffff, 0x00050004,
	0x91c4, 0xffffffff, 0x00010006,
	0x91c8, 0xffffffff, 0x00090008,
	0x91cc, 0xffffffff, 0x00070000,
	0x91d0, 0xffffffff, 0x00030002,
	0x91d4, 0xffffffff, 0x00050004,
	0x91e0, 0xffffffff, 0x00010006,
	0x91e4, 0xffffffff, 0x00090008,
	0x91e8, 0xffffffff, 0x00000000,
	0x91ec, 0xffffffff, 0x00070000,
	0x91f0, 0xffffffff, 0x00030002,
	0x91f4, 0xffffffff, 0x00050004,
	0x9200, 0xffffffff, 0x00010006,
	0x9204, 0xffffffff, 0x00090008,
	0x9294, 0xffffffff, 0x00000000,
	0x929c, 0xffffffff, 0x00000001,
	0x802c, 0xffffffff, 0xc0000000
};

static const u32 cedar_golden_registers[] =
{
	0x3f90, 0xffff0000, 0xff000000,
	0x9148, 0xffff0000, 0xff000000,
	0x3f94, 0xffff0000, 0xff000000,
	0x914c, 0xffff0000, 0xff000000,
	0x9b7c, 0xffffffff, 0x00000000,
	0x8a14, 0xffffffff, 0x00000007,
	0x8b10, 0xffffffff, 0x00000000,
	0x960c, 0xffffffff, 0x54763210,
	0x88c4, 0xffffffff, 0x000000c2,
	0x88d4, 0xffffffff, 0x00000000,
	0x8974, 0xffffffff, 0x00000000,
	0xc78, 0x00000080, 0x00000080,
	0x5eb4, 0xffffffff, 0x00000002,
	0x5e78, 0xffffffff, 0x001000f0,
	0x6104, 0x01000300, 0x00000000,
	0x5bc0, 0x00300000, 0x00000000,
	0x7030, 0xffffffff, 0x00000011,
	0x7c30, 0xffffffff, 0x00000011,
	0x10830, 0xffffffff, 0x00000011,
	0x11430, 0xffffffff, 0x00000011,
	0xd02c, 0xffffffff, 0x08421000,
	0x240c, 0xffffffff, 0x00000380,
	0x8b24, 0xffffffff, 0x00ff0fff,
	0x28a4c, 0x06000000, 0x06000000,
	0x10c, 0x00000001, 0x00000001,
	0x8d00, 0xffffffff, 0x100e4848,
	0x8d04, 0xffffffff, 0x00164745,
	0x8c00, 0xffffffff, 0xe4000003,
	0x8c04, 0xffffffff, 0x40600060,
	0x8c08, 0xffffffff, 0x001c001c,
	0x8cf0, 0xffffffff, 0x08e00410,
	0x8c20, 0xffffffff, 0x00800080,
	0x8c24, 0xffffffff, 0x00800080,
	0x8c18, 0xffffffff, 0x20202078,
	0x8c1c, 0xffffffff, 0x00001010,
	0x28350, 0xffffffff, 0x00000000,
	0xa008, 0xffffffff, 0x00010000,
	0x5cc, 0xffffffff, 0x00000001,
	0x9508, 0xffffffff, 0x00000002
};

static const u32 cedar_mgcg_init[] =
{
	0x802c, 0xffffffff, 0xc0000000,
	0x5448, 0xffffffff, 0x00000100,
	0x55e4, 0xffffffff, 0x00000100,
	0x160c, 0xffffffff, 0x00000100,
	0x5644, 0xffffffff, 0x00000100,
	0xc164, 0xffffffff, 0x00000100,
	0x8a18, 0xffffffff, 0x00000100,
	0x897c, 0xffffffff, 0x06000100,
	0x8b28, 0xffffffff, 0x00000100,
	0x9144, 0xffffffff, 0x00000100,
	0x9a60, 0xffffffff, 0x00000100,
	0x9868, 0xffffffff, 0x00000100,
	0x8d58, 0xffffffff, 0x00000100,
	0x9510, 0xffffffff, 0x00000100,
	0x949c, 0xffffffff, 0x00000100,
	0x9654, 0xffffffff, 0x00000100,
	0x9030, 0xffffffff, 0x00000100,
	0x9034, 0xffffffff, 0x00000100,
	0x9038, 0xffffffff, 0x00000100,
	0x903c, 0xffffffff, 0x00000100,
	0x9040, 0xffffffff, 0x00000100,
	0xa200, 0xffffffff, 0x00000100,
	0xa204, 0xffffffff, 0x00000100,
	0xa208, 0xffffffff, 0x00000100,
	0xa20c, 0xffffffff, 0x00000100,
	0x971c, 0xffffffff, 0x00000100,
	0x977c, 0xffffffff, 0x00000100,
	0x3f80, 0xffffffff, 0x00000100,
	0xa210, 0xffffffff, 0x00000100,
	0xa214, 0xffffffff, 0x00000100,
	0x4d8, 0xffffffff, 0x00000100,
	0x9784, 0xffffffff, 0x00000100,
	0x9698, 0xffffffff, 0x00000100,
	0x4d4, 0xffffffff, 0x00000200,
	0x30cc, 0xffffffff, 0x00000100,
	0xd0c0, 0xffffffff, 0xff000100,
	0x802c, 0xffffffff, 0x40000000,
	0x915c, 0xffffffff, 0x00010000,
	0x9178, 0xffffffff, 0x00050000,
	0x917c, 0xffffffff, 0x00030002,
	0x918c, 0xffffffff, 0x00010004,
	0x9190, 0xffffffff, 0x00070006,
	0x9194, 0xffffffff, 0x00050000,
	0x9198, 0xffffffff, 0x00030002,
	0x91a8, 0xffffffff, 0x00010004,
	0x91ac, 0xffffffff, 0x00070006,
	0x91e8, 0xffffffff, 0x00000000,
	0x9294, 0xffffffff, 0x00000000,
	0x929c, 0xffffffff, 0x00000001,
	0x802c, 0xffffffff, 0xc0000000
};

static const u32 juniper_mgcg_init[] =
{
	0x802c, 0xffffffff, 0xc0000000,
	0x5448, 0xffffffff, 0x00000100,
	0x55e4, 0xffffffff, 0x00000100,
	0x160c, 0xffffffff, 0x00000100,
	0x5644, 0xffffffff, 0x00000100,
	0xc164, 0xffffffff, 0x00000100,
	0x8a18, 0xffffffff, 0x00000100,
	0x897c, 0xffffffff, 0x06000100,
	0x8b28, 0xffffffff, 0x00000100,
	0x9144, 0xffffffff, 0x00000100,
	0x9a60, 0xffffffff, 0x00000100,
	0x9868, 0xffffffff, 0x00000100,
	0x8d58, 0xffffffff, 0x00000100,
	0x9510, 0xffffffff, 0x00000100,
	0x949c, 0xffffffff, 0x00000100,
	0x9654, 0xffffffff, 0x00000100,
	0x9030, 0xffffffff, 0x00000100,
	0x9034, 0xffffffff, 0x00000100,
	0x9038, 0xffffffff, 0x00000100,
	0x903c, 0xffffffff, 0x00000100,
	0x9040, 0xffffffff, 0x00000100,
	0xa200, 0xffffffff, 0x00000100,
	0xa204, 0xffffffff, 0x00000100,
	0xa208, 0xffffffff, 0x00000100,
	0xa20c, 0xffffffff, 0x00000100,
	0x971c, 0xffffffff, 0x00000100,
	0xd0c0, 0xffffffff, 0xff000100,
	0x802c, 0xffffffff, 0x40000000,
	0x915c, 0xffffffff, 0x00010000,
	0x9160, 0xffffffff, 0x00030002,
	0x9178, 0xffffffff, 0x00070000,
	0x917c, 0xffffffff, 0x00030002,
	0x9180, 0xffffffff, 0x00050004,
	0x918c, 0xffffffff, 0x00010006,
	0x9190, 0xffffffff, 0x00090008,
	0x9194, 0xffffffff, 0x00070000,
	0x9198, 0xffffffff, 0x00030002,
	0x919c, 0xffffffff, 0x00050004,
	0x91a8, 0xffffffff, 0x00010006,
	0x91ac, 0xffffffff, 0x00090008,
	0x91b0, 0xffffffff, 0x00070000,
	0x91b4, 0xffffffff, 0x00030002,
	0x91b8, 0xffffffff, 0x00050004,
	0x91c4, 0xffffffff, 0x00010006,
	0x91c8, 0xffffffff, 0x00090008,
	0x91cc, 0xffffffff, 0x00070000,
	0x91d0, 0xffffffff, 0x00030002,
	0x91d4, 0xffffffff, 0x00050004,
	0x91e0, 0xffffffff, 0x00010006,
	0x91e4, 0xffffffff, 0x00090008,
	0x91e8, 0xffffffff, 0x00000000,
	0x91ec, 0xffffffff, 0x00070000,
	0x91f0, 0xffffffff, 0x00030002,
	0x91f4, 0xffffffff, 0x00050004,
	0x9200, 0xffffffff, 0x00010006,
	0x9204, 0xffffffff, 0x00090008,
	0x9208, 0xffffffff, 0x00070000,
	0x920c, 0xffffffff, 0x00030002,
	0x9210, 0xffffffff, 0x00050004,
	0x921c, 0xffffffff, 0x00010006,
	0x9220, 0xffffffff, 0x00090008,
	0x9224, 0xffffffff, 0x00070000,
	0x9228, 0xffffffff, 0x00030002,
	0x922c, 0xffffffff, 0x00050004,
	0x9238, 0xffffffff, 0x00010006,
	0x923c, 0xffffffff, 0x00090008,
	0x9240, 0xffffffff, 0x00070000,
	0x9244, 0xffffffff, 0x00030002,
	0x9248, 0xffffffff, 0x00050004,
	0x9254, 0xffffffff, 0x00010006,
	0x9258, 0xffffffff, 0x00090008,
	0x925c, 0xffffffff, 0x00070000,
	0x9260, 0xffffffff, 0x00030002,
	0x9264, 0xffffffff, 0x00050004,
	0x9270, 0xffffffff, 0x00010006,
	0x9274, 0xffffffff, 0x00090008,
	0x9278, 0xffffffff, 0x00070000,
	0x927c, 0xffffffff, 0x00030002,
	0x9280, 0xffffffff, 0x00050004,
	0x928c, 0xffffffff, 0x00010006,
	0x9290, 0xffffffff, 0x00090008,
	0x9294, 0xffffffff, 0x00000000,
	0x929c, 0xffffffff, 0x00000001,
	0x802c, 0xffffffff, 0xc0000000,
	0x977c, 0xffffffff, 0x00000100,
	0x3f80, 0xffffffff, 0x00000100,
	0xa210, 0xffffffff, 0x00000100,
	0xa214, 0xffffffff, 0x00000100,
	0x4d8, 0xffffffff, 0x00000100,
	0x9784, 0xffffffff, 0x00000100,
	0x9698, 0xffffffff, 0x00000100,
	0x4d4, 0xffffffff, 0x00000200,
	0x30cc, 0xffffffff, 0x00000100,
	0x802c, 0xffffffff, 0xc0000000
};

static const u32 supersumo_golden_registers[] =
{
	0x5eb4, 0xffffffff, 0x00000002,
	0x5cc, 0xffffffff, 0x00000001,
	0x7030, 0xffffffff, 0x00000011,
	0x7c30, 0xffffffff, 0x00000011,
	0x6104, 0x01000300, 0x00000000,
	0x5bc0, 0x00300000, 0x00000000,
	0x8c04, 0xffffffff, 0x40600060,
	0x8c08, 0xffffffff, 0x001c001c,
	0x8c20, 0xffffffff, 0x00800080,
	0x8c24, 0xffffffff, 0x00800080,
	0x8c18, 0xffffffff, 0x20202078,
	0x8c1c, 0xffffffff, 0x00001010,
	0x918c, 0xffffffff, 0x00010006,
	0x91a8, 0xffffffff, 0x00010006,
	0x91c4, 0xffffffff, 0x00010006,
	0x91e0, 0xffffffff, 0x00010006,
	0x9200, 0xffffffff, 0x00010006,
	0x9150, 0xffffffff, 0x6e944040,
	0x917c, 0xffffffff, 0x00030002,
	0x9180, 0xffffffff, 0x00050004,
	0x9198, 0xffffffff, 0x00030002,
	0x919c, 0xffffffff, 0x00050004,
	0x91b4, 0xffffffff, 0x00030002,
	0x91b8, 0xffffffff, 0x00050004,
	0x91d0, 0xffffffff, 0x00030002,
	0x91d4, 0xffffffff, 0x00050004,
	0x91f0, 0xffffffff, 0x00030002,
	0x91f4, 0xffffffff, 0x00050004,
	0x915c, 0xffffffff, 0x00010000,
	0x9160, 0xffffffff, 0x00030002,
	0x3f90, 0xffff0000, 0xff000000,
	0x9178, 0xffffffff, 0x00070000,
	0x9194, 0xffffffff, 0x00070000,
	0x91b0, 0xffffffff, 0x00070000,
	0x91cc, 0xffffffff, 0x00070000,
	0x91ec, 0xffffffff, 0x00070000,
	0x9148, 0xffff0000, 0xff000000,
	0x9190, 0xffffffff, 0x00090008,
	0x91ac, 0xffffffff, 0x00090008,
	0x91c8, 0xffffffff, 0x00090008,
	0x91e4, 0xffffffff, 0x00090008,
	0x9204, 0xffffffff, 0x00090008,
	0x3f94, 0xffff0000, 0xff000000,
	0x914c, 0xffff0000, 0xff000000,
	0x929c, 0xffffffff, 0x00000001,
	0x8a18, 0xffffffff, 0x00000100,
	0x8b28, 0xffffffff, 0x00000100,
	0x9144, 0xffffffff, 0x00000100,
	0x5644, 0xffffffff, 0x00000100,
	0x9b7c, 0xffffffff, 0x00000000,
	0x8030, 0xffffffff, 0x0000100a,
	0x8a14, 0xffffffff, 0x00000007,
	0x8b24, 0xffffffff, 0x00ff0fff,
	0x8b10, 0xffffffff, 0x00000000,
	0x28a4c, 0x06000000, 0x06000000,
	0x4d8, 0xffffffff, 0x00000100,
	0x913c, 0xffff000f, 0x0100000a,
	0x960c, 0xffffffff, 0x54763210,
	0x88c4, 0xffffffff, 0x000000c2,
	0x88d4, 0xffffffff, 0x00000010,
	0x8974, 0xffffffff, 0x00000000,
	0xc78, 0x00000080, 0x00000080,
	0x5e78, 0xffffffff, 0x001000f0,
	0xd02c, 0xffffffff, 0x08421000,
	0xa008, 0xffffffff, 0x00010000,
	0x8d00, 0xffffffff, 0x100e4848,
	0x8d04, 0xffffffff, 0x00164745,
	0x8c00, 0xffffffff, 0xe4000003,
	0x8cf0, 0x1fffffff, 0x08e00620,
	0x28350, 0xffffffff, 0x00000000,
	0x9508, 0xffffffff, 0x00000002
};

static const u32 sumo_golden_registers[] =
{
	0x900c, 0x00ffffff, 0x0017071f,
	0x8c18, 0xffffffff, 0x10101060,
	0x8c1c, 0xffffffff, 0x00001010,
	0x8c30, 0x0000000f, 0x00000005,
	0x9688, 0x0000000f, 0x00000007
};

static const u32 wrestler_golden_registers[] =
{
	0x5eb4, 0xffffffff, 0x00000002,
	0x5cc, 0xffffffff, 0x00000001,
	0x7030, 0xffffffff, 0x00000011,
	0x7c30, 0xffffffff, 0x00000011,
	0x6104, 0x01000300, 0x00000000,
	0x5bc0, 0x00300000, 0x00000000,
	0x918c, 0xffffffff, 0x00010006,
	0x91a8, 0xffffffff, 0x00010006,
	0x9150, 0xffffffff, 0x6e944040,
	0x917c, 0xffffffff, 0x00030002,
	0x9198, 0xffffffff, 0x00030002,
	0x915c, 0xffffffff, 0x00010000,
	0x3f90, 0xffff0000, 0xff000000,
	0x9178, 0xffffffff, 0x00070000,
	0x9194, 0xffffffff, 0x00070000,
	0x9148, 0xffff0000, 0xff000000,
	0x9190, 0xffffffff, 0x00090008,
	0x91ac, 0xffffffff, 0x00090008,
	0x3f94, 0xffff0000, 0xff000000,
	0x914c, 0xffff0000, 0xff000000,
	0x929c, 0xffffffff, 0x00000001,
	0x8a18, 0xffffffff, 0x00000100,
	0x8b28, 0xffffffff, 0x00000100,
	0x9144, 0xffffffff, 0x00000100,
	0x9b7c, 0xffffffff, 0x00000000,
	0x8030, 0xffffffff, 0x0000100a,
	0x8a14, 0xffffffff, 0x00000001,
	0x8b24, 0xffffffff, 0x00ff0fff,
	0x8b10, 0xffffffff, 0x00000000,
	0x28a4c, 0x06000000, 0x06000000,
	0x4d8, 0xffffffff, 0x00000100,
	0x913c, 0xffff000f, 0x0100000a,
	0x960c, 0xffffffff, 0x54763210,
	0x88c4, 0xffffffff, 0x000000c2,
	0x88d4, 0xffffffff, 0x00000010,
	0x8974, 0xffffffff, 0x00000000,
	0xc78, 0x00000080, 0x00000080,
	0x5e78, 0xffffffff, 0x001000f0,
	0xd02c, 0xffffffff, 0x08421000,
	0xa008, 0xffffffff, 0x00010000,
	0x8d00, 0xffffffff, 0x100e4848,
	0x8d04, 0xffffffff, 0x00164745,
	0x8c00, 0xffffffff, 0xe4000003,
	0x8cf0, 0x1fffffff, 0x08e00410,
	0x28350, 0xffffffff, 0x00000000,
	0x9508, 0xffffffff, 0x00000002,
	0x900c, 0xffffffff, 0x0017071f,
	0x8c18, 0xffffffff, 0x10101060,
	0x8c1c, 0xffffffff, 0x00001010
};

static const u32 barts_golden_registers[] =
{
	0x5eb4, 0xffffffff, 0x00000002,
	0x5e78, 0x8f311ff1, 0x001000f0,
	0x3f90, 0xffff0000, 0xff000000,
	0x9148, 0xffff0000, 0xff000000,
	0x3f94, 0xffff0000, 0xff000000,
	0x914c, 0xffff0000, 0xff000000,
	0xc78, 0x00000080, 0x00000080,
	0xbd4, 0x70073777, 0x00010001,
	0xd02c, 0xbfffff1f, 0x08421000,
	0xd0b8, 0x03773777, 0x02011003,
	0x5bc0, 0x00200000, 0x50100000,
	0x98f8, 0x33773777, 0x02011003,
	0x98fc, 0xffffffff, 0x76543210,
	0x7030, 0x31000311, 0x00000011,
	0x2f48, 0x00000007, 0x02011003,
	0x6b28, 0x00000010, 0x00000012,
	0x7728, 0x00000010, 0x00000012,
	0x10328, 0x00000010, 0x00000012,
	0x10f28, 0x00000010, 0x00000012,
	0x11b28, 0x00000010, 0x00000012,
	0x12728, 0x00000010, 0x00000012,
	0x240c, 0x000007ff, 0x00000380,
	0x8a14, 0xf000001f, 0x00000007,
	0x8b24, 0x3fff3fff, 0x00ff0fff,
	0x8b10, 0x0000ff0f, 0x00000000,
	0x28a4c, 0x07ffffff, 0x06000000,
	0x10c, 0x00000001, 0x00010003,
	0xa02c, 0xffffffff, 0x0000009b,
	0x913c, 0x0000000f, 0x0100000a,
	0x8d00, 0xffff7f7f, 0x100e4848,
	0x8d04, 0x00ffffff, 0x00164745,
	0x8c00, 0xfffc0003, 0xe4000003,
	0x8c04, 0xf8ff00ff, 0x40600060,
	0x8c08, 0x00ff00ff, 0x001c001c,
	0x8cf0, 0x1fff1fff, 0x08e00620,
	0x8c20, 0x0fff0fff, 0x00800080,
	0x8c24, 0x0fff0fff, 0x00800080,
	0x8c18, 0xffffffff, 0x20202078,
	0x8c1c, 0x0000ffff, 0x00001010,
	0x28350, 0x00000f01, 0x00000000,
	0x9508, 0x3700001f, 0x00000002,
	0x960c, 0xffffffff, 0x54763210,
	0x88c4, 0x001f3ae3, 0x000000c2,
	0x88d4, 0x0000001f, 0x00000010,
	0x8974, 0xffffffff, 0x00000000
};

static const u32 turks_golden_registers[] =
{
	0x5eb4, 0xffffffff, 0x00000002,
	0x5e78, 0x8f311ff1, 0x001000f0,
	0x8c8, 0x00003000, 0x00001070,
	0x8cc, 0x000fffff, 0x00040035,
	0x3f90, 0xffff0000, 0xfff00000,
	0x9148, 0xffff0000, 0xfff00000,
	0x3f94, 0xffff0000, 0xfff00000,
	0x914c, 0xffff0000, 0xfff00000,
	0xc78, 0x00000080, 0x00000080,
	0xbd4, 0x00073007, 0x00010002,
	0xd02c, 0xbfffff1f, 0x08421000,
	0xd0b8, 0x03773777, 0x02010002,
	0x5bc0, 0x00200000, 0x50100000,
	0x98f8, 0x33773777, 0x00010002,
	0x98fc, 0xffffffff, 0x33221100,
	0x7030, 0x31000311, 0x00000011,
	0x2f48, 0x33773777, 0x00010002,
	0x6b28, 0x00000010, 0x00000012,
	0x7728, 0x00000010, 0x00000012,
	0x10328, 0x00000010, 0x00000012,
	0x10f28, 0x00000010, 0x00000012,
	0x11b28, 0x00000010, 0x00000012,
	0x12728, 0x00000010, 0x00000012,
	0x240c, 0x000007ff, 0x00000380,
	0x8a14, 0xf000001f, 0x00000007,
	0x8b24, 0x3fff3fff, 0x00ff0fff,
	0x8b10, 0x0000ff0f, 0x00000000,
	0x28a4c, 0x07ffffff, 0x06000000,
	0x10c, 0x00000001, 0x00010003,
	0xa02c, 0xffffffff, 0x0000009b,
	0x913c, 0x0000000f, 0x0100000a,
	0x8d00, 0xffff7f7f, 0x100e4848,
	0x8d04, 0x00ffffff, 0x00164745,
	0x8c00, 0xfffc0003, 0xe4000003,
	0x8c04, 0xf8ff00ff, 0x40600060,
	0x8c08, 0x00ff00ff, 0x001c001c,
	0x8cf0, 0x1fff1fff, 0x08e00410,
	0x8c20, 0x0fff0fff, 0x00800080,
	0x8c24, 0x0fff0fff, 0x00800080,
	0x8c18, 0xffffffff, 0x20202078,
	0x8c1c, 0x0000ffff, 0x00001010,
	0x28350, 0x00000f01, 0x00000000,
	0x9508, 0x3700001f, 0x00000002,
	0x960c, 0xffffffff, 0x54763210,
	0x88c4, 0x001f3ae3, 0x000000c2,
	0x88d4, 0x0000001f, 0x00000010,
	0x8974, 0xffffffff, 0x00000000
};

static const u32 caicos_golden_registers[] =
{
	0x5eb4, 0xffffffff, 0x00000002,
	0x5e78, 0x8f311ff1, 0x001000f0,
	0x8c8, 0x00003420, 0x00001450,
	0x8cc, 0x000fffff, 0x00040035,
	0x3f90, 0xffff0000, 0xfffc0000,
	0x9148, 0xffff0000, 0xfffc0000,
	0x3f94, 0xffff0000, 0xfffc0000,
	0x914c, 0xffff0000, 0xfffc0000,
	0xc78, 0x00000080, 0x00000080,
	0xbd4, 0x00073007, 0x00010001,
	0xd02c, 0xbfffff1f, 0x08421000,
	0xd0b8, 0x03773777, 0x02010001,
	0x5bc0, 0x00200000, 0x50100000,
	0x98f8, 0x33773777, 0x02010001,
	0x98fc, 0xffffffff, 0x33221100,
	0x7030, 0x31000311, 0x00000011,
	0x2f48, 0x33773777, 0x02010001,
	0x6b28, 0x00000010, 0x00000012,
	0x7728, 0x00000010, 0x00000012,
	0x10328, 0x00000010, 0x00000012,
	0x10f28, 0x00000010, 0x00000012,
	0x11b28, 0x00000010, 0x00000012,
	0x12728, 0x00000010, 0x00000012,
	0x240c, 0x000007ff, 0x00000380,
	0x8a14, 0xf000001f, 0x00000001,
	0x8b24, 0x3fff3fff, 0x00ff0fff,
	0x8b10, 0x0000ff0f, 0x00000000,
	0x28a4c, 0x07ffffff, 0x06000000,
	0x10c, 0x00000001, 0x00010003,
	0xa02c, 0xffffffff, 0x0000009b,
	0x913c, 0x0000000f, 0x0100000a,
	0x8d00, 0xffff7f7f, 0x100e4848,
	0x8d04, 0x00ffffff, 0x00164745,
	0x8c00, 0xfffc0003, 0xe4000003,
	0x8c04, 0xf8ff00ff, 0x40600060,
	0x8c08, 0x00ff00ff, 0x001c001c,
	0x8cf0, 0x1fff1fff, 0x08e00410,
	0x8c20, 0x0fff0fff, 0x00800080,
	0x8c24, 0x0fff0fff, 0x00800080,
	0x8c18, 0xffffffff, 0x20202078,
	0x8c1c, 0x0000ffff, 0x00001010,
	0x28350, 0x00000f01, 0x00000000,
	0x9508, 0x3700001f, 0x00000002,
	0x960c, 0xffffffff, 0x54763210,
	0x88c4, 0x001f3ae3, 0x000000c2,
	0x88d4, 0x0000001f, 0x00000010,
	0x8974, 0xffffffff, 0x00000000
};

static void evergreen_init_golden_registers(struct radeon_device *rdev)
{
	switch (rdev->family) {
	case CHIP_CYPRESS:
	case CHIP_HEMLOCK:
		radeon_program_register_sequence(rdev,
						 evergreen_golden_registers,
						 (const u32)ARRAY_SIZE(evergreen_golden_registers));
		radeon_program_register_sequence(rdev,
						 evergreen_golden_registers2,
						 (const u32)ARRAY_SIZE(evergreen_golden_registers2));
		radeon_program_register_sequence(rdev,
						 cypress_mgcg_init,
						 (const u32)ARRAY_SIZE(cypress_mgcg_init));
		break;
	case CHIP_JUNIPER:
		radeon_program_register_sequence(rdev,
						 evergreen_golden_registers,
						 (const u32)ARRAY_SIZE(evergreen_golden_registers));
		radeon_program_register_sequence(rdev,
						 evergreen_golden_registers2,
						 (const u32)ARRAY_SIZE(evergreen_golden_registers2));
		radeon_program_register_sequence(rdev,
						 juniper_mgcg_init,
						 (const u32)ARRAY_SIZE(juniper_mgcg_init));
		break;
	case CHIP_REDWOOD:
		radeon_program_register_sequence(rdev,
						 evergreen_golden_registers,
						 (const u32)ARRAY_SIZE(evergreen_golden_registers));
		radeon_program_register_sequence(rdev,
						 evergreen_golden_registers2,
						 (const u32)ARRAY_SIZE(evergreen_golden_registers2));
		radeon_program_register_sequence(rdev,
						 redwood_mgcg_init,
						 (const u32)ARRAY_SIZE(redwood_mgcg_init));
		break;
	case CHIP_CEDAR:
		radeon_program_register_sequence(rdev,
						 cedar_golden_registers,
						 (const u32)ARRAY_SIZE(cedar_golden_registers));
		radeon_program_register_sequence(rdev,
						 evergreen_golden_registers2,
						 (const u32)ARRAY_SIZE(evergreen_golden_registers2));
		radeon_program_register_sequence(rdev,
						 cedar_mgcg_init,
						 (const u32)ARRAY_SIZE(cedar_mgcg_init));
		break;
	case CHIP_PALM:
		radeon_program_register_sequence(rdev,
						 wrestler_golden_registers,
						 (const u32)ARRAY_SIZE(wrestler_golden_registers));
		break;
	case CHIP_SUMO:
		radeon_program_register_sequence(rdev,
						 supersumo_golden_registers,
						 (const u32)ARRAY_SIZE(supersumo_golden_registers));
		break;
	case CHIP_SUMO2:
		radeon_program_register_sequence(rdev,
						 supersumo_golden_registers,
						 (const u32)ARRAY_SIZE(supersumo_golden_registers));
		radeon_program_register_sequence(rdev,
						 sumo_golden_registers,
						 (const u32)ARRAY_SIZE(sumo_golden_registers));
		break;
	case CHIP_BARTS:
		radeon_program_register_sequence(rdev,
						 barts_golden_registers,
						 (const u32)ARRAY_SIZE(barts_golden_registers));
		break;
	case CHIP_TURKS:
		radeon_program_register_sequence(rdev,
						 turks_golden_registers,
						 (const u32)ARRAY_SIZE(turks_golden_registers));
		break;
	case CHIP_CAICOS:
		radeon_program_register_sequence(rdev,
						 caicos_golden_registers,
						 (const u32)ARRAY_SIZE(caicos_golden_registers));
		break;
	default:
		break;
	}
}
>>>>>>> refs/remotes/origin/master

void evergreen_tiling_fields(unsigned tiling_flags, unsigned *bankw,
			     unsigned *bankh, unsigned *mtaspect,
			     unsigned *tile_split)
{
	*bankw = (tiling_flags >> RADEON_TILING_EG_BANKW_SHIFT) & RADEON_TILING_EG_BANKW_MASK;
	*bankh = (tiling_flags >> RADEON_TILING_EG_BANKH_SHIFT) & RADEON_TILING_EG_BANKH_MASK;
	*mtaspect = (tiling_flags >> RADEON_TILING_EG_MACRO_TILE_ASPECT_SHIFT) & RADEON_TILING_EG_MACRO_TILE_ASPECT_MASK;
	*tile_split = (tiling_flags >> RADEON_TILING_EG_TILE_SPLIT_SHIFT) & RADEON_TILING_EG_TILE_SPLIT_MASK;
	switch (*bankw) {
	default:
	case 1: *bankw = EVERGREEN_ADDR_SURF_BANK_WIDTH_1; break;
	case 2: *bankw = EVERGREEN_ADDR_SURF_BANK_WIDTH_2; break;
	case 4: *bankw = EVERGREEN_ADDR_SURF_BANK_WIDTH_4; break;
	case 8: *bankw = EVERGREEN_ADDR_SURF_BANK_WIDTH_8; break;
	}
	switch (*bankh) {
	default:
	case 1: *bankh = EVERGREEN_ADDR_SURF_BANK_HEIGHT_1; break;
	case 2: *bankh = EVERGREEN_ADDR_SURF_BANK_HEIGHT_2; break;
	case 4: *bankh = EVERGREEN_ADDR_SURF_BANK_HEIGHT_4; break;
	case 8: *bankh = EVERGREEN_ADDR_SURF_BANK_HEIGHT_8; break;
	}
	switch (*mtaspect) {
	default:
	case 1: *mtaspect = EVERGREEN_ADDR_SURF_MACRO_TILE_ASPECT_1; break;
	case 2: *mtaspect = EVERGREEN_ADDR_SURF_MACRO_TILE_ASPECT_2; break;
	case 4: *mtaspect = EVERGREEN_ADDR_SURF_MACRO_TILE_ASPECT_4; break;
	case 8: *mtaspect = EVERGREEN_ADDR_SURF_MACRO_TILE_ASPECT_8; break;
	}
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

void evergreen_fix_pci_max_read_req_size(struct radeon_device *rdev)
{
	u16 ctl, v;
	int cap, err;

	cap = pci_pcie_cap(rdev->pdev);
	if (!cap)
		return;

	err = pci_read_config_word(rdev->pdev, cap + PCI_EXP_DEVCTL, &ctl);
	if (err)
		return;

	v = (ctl & PCI_EXP_DEVCTL_READRQ) >> 12;

	/* if bios or OS sets MAX_READ_REQUEST_SIZE to an invalid value, fix it
	 * to avoid hangs or perfomance issues
	 */
	if ((v == 0) || (v == 6) || (v == 7)) {
		ctl &= ~PCI_EXP_DEVCTL_READRQ;
		ctl |= (2 << 12);
		pci_write_config_word(rdev->pdev, cap + PCI_EXP_DEVCTL, ctl);
	}
}

<<<<<<< HEAD
=======
void dce4_wait_for_vblank(struct radeon_device *rdev, int crtc)
{
	int i;
=======

static int sumo_set_uvd_clock(struct radeon_device *rdev, u32 clock,
			      u32 cntl_reg, u32 status_reg)
{
	int r, i;
	struct atom_clock_dividers dividers;

        r = radeon_atom_get_clock_dividers(rdev, COMPUTE_ENGINE_PLL_PARAM,
					   clock, false, &dividers);
	if (r)
		return r;

	WREG32_P(cntl_reg, dividers.post_div, ~(DCLK_DIR_CNTL_EN|DCLK_DIVIDER_MASK));

	for (i = 0; i < 100; i++) {
		if (RREG32(status_reg) & DCLK_STATUS)
			break;
		mdelay(10);
	}
	if (i == 100)
		return -ETIMEDOUT;

	return 0;
}

int sumo_set_uvd_clocks(struct radeon_device *rdev, u32 vclk, u32 dclk)
{
	int r = 0;
	u32 cg_scratch = RREG32(CG_SCRATCH1);

	r = sumo_set_uvd_clock(rdev, vclk, CG_VCLK_CNTL, CG_VCLK_STATUS);
	if (r)
		goto done;
	cg_scratch &= 0xffff0000;
	cg_scratch |= vclk / 100; /* Mhz */

	r = sumo_set_uvd_clock(rdev, dclk, CG_DCLK_CNTL, CG_DCLK_STATUS);
	if (r)
		goto done;
	cg_scratch &= 0x0000ffff;
	cg_scratch |= (dclk / 100) << 16; /* Mhz */

done:
	WREG32(CG_SCRATCH1, cg_scratch);

	return r;
}

int evergreen_set_uvd_clocks(struct radeon_device *rdev, u32 vclk, u32 dclk)
{
	/* start off with something large */
	unsigned fb_div = 0, vclk_div = 0, dclk_div = 0;
	int r;

	/* bypass vclk and dclk with bclk */
	WREG32_P(CG_UPLL_FUNC_CNTL_2,
		VCLK_SRC_SEL(1) | DCLK_SRC_SEL(1),
		~(VCLK_SRC_SEL_MASK | DCLK_SRC_SEL_MASK));

	/* put PLL in bypass mode */
	WREG32_P(CG_UPLL_FUNC_CNTL, UPLL_BYPASS_EN_MASK, ~UPLL_BYPASS_EN_MASK);

	if (!vclk || !dclk) {
		/* keep the Bypass mode, put PLL to sleep */
		WREG32_P(CG_UPLL_FUNC_CNTL, UPLL_SLEEP_MASK, ~UPLL_SLEEP_MASK);
		return 0;
	}

	r = radeon_uvd_calc_upll_dividers(rdev, vclk, dclk, 125000, 250000,
					  16384, 0x03FFFFFF, 0, 128, 5,
					  &fb_div, &vclk_div, &dclk_div);
	if (r)
		return r;

	/* set VCO_MODE to 1 */
	WREG32_P(CG_UPLL_FUNC_CNTL, UPLL_VCO_MODE_MASK, ~UPLL_VCO_MODE_MASK);

	/* toggle UPLL_SLEEP to 1 then back to 0 */
	WREG32_P(CG_UPLL_FUNC_CNTL, UPLL_SLEEP_MASK, ~UPLL_SLEEP_MASK);
	WREG32_P(CG_UPLL_FUNC_CNTL, 0, ~UPLL_SLEEP_MASK);

	/* deassert UPLL_RESET */
	WREG32_P(CG_UPLL_FUNC_CNTL, 0, ~UPLL_RESET_MASK);

	mdelay(1);

	r = radeon_uvd_send_upll_ctlreq(rdev, CG_UPLL_FUNC_CNTL);
	if (r)
		return r;

	/* assert UPLL_RESET again */
	WREG32_P(CG_UPLL_FUNC_CNTL, UPLL_RESET_MASK, ~UPLL_RESET_MASK);

<<<<<<< HEAD
	/* disable spread spectrum. */
	WREG32_P(CG_UPLL_SPREAD_SPECTRUM, 0, ~SSEN_MASK);

	/* set feedback divider */
	WREG32_P(CG_UPLL_FUNC_CNTL_3, UPLL_FB_DIV(fb_div), ~UPLL_FB_DIV_MASK);

	/* set ref divider to 0 */
	WREG32_P(CG_UPLL_FUNC_CNTL, 0, ~UPLL_REF_DIV_MASK);

	if (fb_div < 307200)
		WREG32_P(CG_UPLL_FUNC_CNTL_4, 0, ~UPLL_SPARE_ISPARE9);
	else
		WREG32_P(CG_UPLL_FUNC_CNTL_4, UPLL_SPARE_ISPARE9, ~UPLL_SPARE_ISPARE9);

	/* set PDIV_A and PDIV_B */
	WREG32_P(CG_UPLL_FUNC_CNTL_2,
		UPLL_PDIV_A(vclk_div) | UPLL_PDIV_B(dclk_div),
		~(UPLL_PDIV_A_MASK | UPLL_PDIV_B_MASK));

	/* give the PLL some time to settle */
	mdelay(15);

	/* deassert PLL_RESET */
	WREG32_P(CG_UPLL_FUNC_CNTL, 0, ~UPLL_RESET_MASK);

	mdelay(15);

	/* switch from bypass mode to normal mode */
	WREG32_P(CG_UPLL_FUNC_CNTL, 0, ~UPLL_BYPASS_EN_MASK);

	r = radeon_uvd_send_upll_ctlreq(rdev, CG_UPLL_FUNC_CNTL);
	if (r)
		return r;

	/* switch VCLK and DCLK selection */
	WREG32_P(CG_UPLL_FUNC_CNTL_2,
		VCLK_SRC_SEL(2) | DCLK_SRC_SEL(2),
		~(VCLK_SRC_SEL_MASK | DCLK_SRC_SEL_MASK));

	mdelay(100);

	return 0;
}

void evergreen_fix_pci_max_read_req_size(struct radeon_device *rdev)
{
	int readrq;
	u16 v;

	readrq = pcie_get_readrq(rdev->pdev);
	v = ffs(readrq) - 8;
	/* if bios or OS sets MAX_READ_REQUEST_SIZE to an invalid value, fix it
	 * to avoid hangs or perfomance issues
	 */
	if ((v == 0) || (v == 6) || (v == 7))
		pcie_set_readrq(rdev->pdev, 512);
}

void dce4_program_fmt(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
	struct radeon_crtc *radeon_crtc = to_radeon_crtc(encoder->crtc);
	struct drm_connector *connector = radeon_get_connector_for_encoder(encoder);
	int bpc = 0;
	u32 tmp = 0;
	enum radeon_connector_dither dither = RADEON_FMT_DITHER_DISABLE;

	if (connector) {
		struct radeon_connector *radeon_connector = to_radeon_connector(connector);
		bpc = radeon_get_monitor_bpc(connector);
		dither = radeon_connector->dither;
	}

	/* LVDS/eDP FMT is set up by atom */
	if (radeon_encoder->devices & ATOM_DEVICE_LCD_SUPPORT)
		return;

	/* not needed for analog */
	if ((radeon_encoder->encoder_id == ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1) ||
	    (radeon_encoder->encoder_id == ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2))
		return;

	if (bpc == 0)
		return;

	switch (bpc) {
	case 6:
		if (dither == RADEON_FMT_DITHER_ENABLE)
			/* XXX sort out optimal dither settings */
			tmp |= (FMT_FRAME_RANDOM_ENABLE | FMT_HIGHPASS_RANDOM_ENABLE |
				FMT_SPATIAL_DITHER_EN);
		else
			tmp |= FMT_TRUNCATE_EN;
		break;
	case 8:
		if (dither == RADEON_FMT_DITHER_ENABLE)
			/* XXX sort out optimal dither settings */
			tmp |= (FMT_FRAME_RANDOM_ENABLE | FMT_HIGHPASS_RANDOM_ENABLE |
				FMT_RGB_RANDOM_ENABLE |
				FMT_SPATIAL_DITHER_EN | FMT_SPATIAL_DITHER_DEPTH);
		else
			tmp |= (FMT_TRUNCATE_EN | FMT_TRUNCATE_DEPTH);
		break;
	case 10:
	default:
		/* not needed */
		break;
	}

	WREG32(FMT_BIT_DEPTH_CONTROL + radeon_crtc->crtc_offset, tmp);
}

static bool dce4_is_in_vblank(struct radeon_device *rdev, int crtc)
{
	if (RREG32(EVERGREEN_CRTC_STATUS + crtc_offsets[crtc]) & EVERGREEN_CRTC_V_BLANK)
		return true;
	else
		return false;
}

static bool dce4_is_counter_moving(struct radeon_device *rdev, int crtc)
{
	u32 pos1, pos2;

	pos1 = RREG32(EVERGREEN_CRTC_STATUS_POSITION + crtc_offsets[crtc]);
	pos2 = RREG32(EVERGREEN_CRTC_STATUS_POSITION + crtc_offsets[crtc]);

	if (pos1 != pos2)
		return true;
	else
		return false;
}

/**
 * dce4_wait_for_vblank - vblank wait asic callback.
 *
 * @rdev: radeon_device pointer
 * @crtc: crtc to wait for vblank on
 *
 * Wait for vblank on the requested crtc (evergreen+).
 */
void dce4_wait_for_vblank(struct radeon_device *rdev, int crtc)
{
	unsigned i = 0;
>>>>>>> refs/remotes/origin/master

	if (crtc >= rdev->num_crtc)
		return;

<<<<<<< HEAD
=======
	if (crtc >= rdev->num_crtc)
		return;

>>>>>>> refs/remotes/origin/cm-11.0
	if (RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[crtc]) & EVERGREEN_CRTC_MASTER_EN) {
		for (i = 0; i < rdev->usec_timeout; i++) {
			if (!(RREG32(EVERGREEN_CRTC_STATUS + crtc_offsets[crtc]) & EVERGREEN_CRTC_V_BLANK))
				break;
			udelay(1);
		}
		for (i = 0; i < rdev->usec_timeout; i++) {
			if (RREG32(EVERGREEN_CRTC_STATUS + crtc_offsets[crtc]) & EVERGREEN_CRTC_V_BLANK)
				break;
			udelay(1);
=======
	if (!(RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[crtc]) & EVERGREEN_CRTC_MASTER_EN))
		return;

	/* depending on when we hit vblank, we may be close to active; if so,
	 * wait for another frame.
	 */
	while (dce4_is_in_vblank(rdev, crtc)) {
		if (i++ % 100 == 0) {
			if (!dce4_is_counter_moving(rdev, crtc))
				break;
		}
	}

	while (!dce4_is_in_vblank(rdev, crtc)) {
		if (i++ % 100 == 0) {
			if (!dce4_is_counter_moving(rdev, crtc))
				break;
>>>>>>> refs/remotes/origin/master
		}
	}
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
/**
 * radeon_irq_kms_pflip_irq_get - pre-pageflip callback.
 *
 * @rdev: radeon_device pointer
 * @crtc: crtc to prepare for pageflip on
 *
 * Pre-pageflip callback (evergreen+).
 * Enables the pageflip irq (vblank irq).
 */
>>>>>>> refs/remotes/origin/master
void evergreen_pre_page_flip(struct radeon_device *rdev, int crtc)
{
	/* enable the pflip int */
	radeon_irq_kms_pflip_irq_get(rdev, crtc);
}

<<<<<<< HEAD
=======
/**
 * evergreen_post_page_flip - pos-pageflip callback.
 *
 * @rdev: radeon_device pointer
 * @crtc: crtc to cleanup pageflip on
 *
 * Post-pageflip callback (evergreen+).
 * Disables the pageflip irq (vblank irq).
 */
>>>>>>> refs/remotes/origin/master
void evergreen_post_page_flip(struct radeon_device *rdev, int crtc)
{
	/* disable the pflip int */
	radeon_irq_kms_pflip_irq_put(rdev, crtc);
}

<<<<<<< HEAD
=======
/**
 * evergreen_page_flip - pageflip callback.
 *
 * @rdev: radeon_device pointer
 * @crtc_id: crtc to cleanup pageflip on
 * @crtc_base: new address of the crtc (GPU MC address)
 *
 * Does the actual pageflip (evergreen+).
 * During vblank we take the crtc lock and wait for the update_pending
 * bit to go high, when it does, we release the lock, and allow the
 * double buffered update to take place.
 * Returns the current update pending status.
 */
>>>>>>> refs/remotes/origin/master
u32 evergreen_page_flip(struct radeon_device *rdev, int crtc_id, u64 crtc_base)
{
	struct radeon_crtc *radeon_crtc = rdev->mode_info.crtcs[crtc_id];
	u32 tmp = RREG32(EVERGREEN_GRPH_UPDATE + radeon_crtc->crtc_offset);
	int i;

	/* Lock the graphics update lock */
	tmp |= EVERGREEN_GRPH_UPDATE_LOCK;
	WREG32(EVERGREEN_GRPH_UPDATE + radeon_crtc->crtc_offset, tmp);

	/* update the scanout addresses */
	WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS_HIGH + radeon_crtc->crtc_offset,
	       upper_32_bits(crtc_base));
	WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset,
	       (u32)crtc_base);

	WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS_HIGH + radeon_crtc->crtc_offset,
	       upper_32_bits(crtc_base));
	WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset,
	       (u32)crtc_base);

	/* Wait for update_pending to go high. */
	for (i = 0; i < rdev->usec_timeout; i++) {
		if (RREG32(EVERGREEN_GRPH_UPDATE + radeon_crtc->crtc_offset) & EVERGREEN_GRPH_SURFACE_UPDATE_PENDING)
			break;
		udelay(1);
	}
	DRM_DEBUG("Update pending now high. Unlocking vupdate_lock.\n");

	/* Unlock the lock, so double-buffering can take place inside vblank */
	tmp &= ~EVERGREEN_GRPH_UPDATE_LOCK;
	WREG32(EVERGREEN_GRPH_UPDATE + radeon_crtc->crtc_offset, tmp);

	/* Return current update_pending status: */
	return RREG32(EVERGREEN_GRPH_UPDATE + radeon_crtc->crtc_offset) & EVERGREEN_GRPH_SURFACE_UPDATE_PENDING;
}

/* get temperature in millidegrees */
int evergreen_get_temp(struct radeon_device *rdev)
{
	u32 temp, toffset;
	int actual_temp = 0;

	if (rdev->family == CHIP_JUNIPER) {
		toffset = (RREG32(CG_THERMAL_CTRL) & TOFFSET_MASK) >>
			TOFFSET_SHIFT;
		temp = (RREG32(CG_TS0_STATUS) & TS0_ADC_DOUT_MASK) >>
			TS0_ADC_DOUT_SHIFT;

		if (toffset & 0x100)
			actual_temp = temp / 2 - (0x200 - toffset);
		else
			actual_temp = temp / 2 + toffset;

		actual_temp = actual_temp * 1000;

	} else {
		temp = (RREG32(CG_MULT_THERMAL_STATUS) & ASIC_T_MASK) >>
			ASIC_T_SHIFT;

		if (temp & 0x400)
			actual_temp = -256;
		else if (temp & 0x200)
			actual_temp = 255;
		else if (temp & 0x100) {
			actual_temp = temp & 0x1ff;
			actual_temp |= ~0x1ff;
		} else
			actual_temp = temp & 0xff;

		actual_temp = (actual_temp * 1000) / 2;
	}

	return actual_temp;
}

int sumo_get_temp(struct radeon_device *rdev)
{
	u32 temp = RREG32(CG_THERMAL_STATUS) & 0xff;
	int actual_temp = temp - 49;

	return actual_temp * 1000;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
/**
 * sumo_pm_init_profile - Initialize power profiles callback.
 *
 * @rdev: radeon_device pointer
 *
 * Initialize the power states used in profile mode
 * (sumo, trinity, SI).
 * Used for profile mode only.
 */
>>>>>>> refs/remotes/origin/master
void sumo_pm_init_profile(struct radeon_device *rdev)
{
	int idx;

	/* default */
	rdev->pm.profiles[PM_PROFILE_DEFAULT_IDX].dpms_off_ps_idx = rdev->pm.default_power_state_index;
	rdev->pm.profiles[PM_PROFILE_DEFAULT_IDX].dpms_on_ps_idx = rdev->pm.default_power_state_index;
	rdev->pm.profiles[PM_PROFILE_DEFAULT_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_DEFAULT_IDX].dpms_on_cm_idx = 0;

	/* low,mid sh/mh */
	if (rdev->flags & RADEON_IS_MOBILITY)
		idx = radeon_pm_get_type_index(rdev, POWER_STATE_TYPE_BATTERY, 0);
	else
		idx = radeon_pm_get_type_index(rdev, POWER_STATE_TYPE_PERFORMANCE, 0);

	rdev->pm.profiles[PM_PROFILE_LOW_SH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_LOW_SH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_LOW_SH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_LOW_SH_IDX].dpms_on_cm_idx = 0;

	rdev->pm.profiles[PM_PROFILE_LOW_MH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_LOW_MH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_LOW_MH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_LOW_MH_IDX].dpms_on_cm_idx = 0;

	rdev->pm.profiles[PM_PROFILE_MID_SH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_MID_SH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_MID_SH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_MID_SH_IDX].dpms_on_cm_idx = 0;

	rdev->pm.profiles[PM_PROFILE_MID_MH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_MID_MH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_MID_MH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_MID_MH_IDX].dpms_on_cm_idx = 0;

	/* high sh/mh */
	idx = radeon_pm_get_type_index(rdev, POWER_STATE_TYPE_PERFORMANCE, 0);
	rdev->pm.profiles[PM_PROFILE_HIGH_SH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_HIGH_SH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_HIGH_SH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_HIGH_SH_IDX].dpms_on_cm_idx =
		rdev->pm.power_state[idx].num_clock_modes - 1;

	rdev->pm.profiles[PM_PROFILE_HIGH_MH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_HIGH_MH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_HIGH_MH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_HIGH_MH_IDX].dpms_on_cm_idx =
		rdev->pm.power_state[idx].num_clock_modes - 1;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
/**
 * btc_pm_init_profile - Initialize power profiles callback.
 *
 * @rdev: radeon_device pointer
 *
 * Initialize the power states used in profile mode
 * (BTC, cayman).
 * Used for profile mode only.
 */
void btc_pm_init_profile(struct radeon_device *rdev)
{
	int idx;

	/* default */
	rdev->pm.profiles[PM_PROFILE_DEFAULT_IDX].dpms_off_ps_idx = rdev->pm.default_power_state_index;
	rdev->pm.profiles[PM_PROFILE_DEFAULT_IDX].dpms_on_ps_idx = rdev->pm.default_power_state_index;
	rdev->pm.profiles[PM_PROFILE_DEFAULT_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_DEFAULT_IDX].dpms_on_cm_idx = 2;
	/* starting with BTC, there is one state that is used for both
	 * MH and SH.  Difference is that we always use the high clock index for
	 * mclk.
	 */
	if (rdev->flags & RADEON_IS_MOBILITY)
		idx = radeon_pm_get_type_index(rdev, POWER_STATE_TYPE_BATTERY, 0);
	else
		idx = radeon_pm_get_type_index(rdev, POWER_STATE_TYPE_PERFORMANCE, 0);
	/* low sh */
	rdev->pm.profiles[PM_PROFILE_LOW_SH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_LOW_SH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_LOW_SH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_LOW_SH_IDX].dpms_on_cm_idx = 0;
	/* mid sh */
	rdev->pm.profiles[PM_PROFILE_MID_SH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_MID_SH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_MID_SH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_MID_SH_IDX].dpms_on_cm_idx = 1;
	/* high sh */
	rdev->pm.profiles[PM_PROFILE_HIGH_SH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_HIGH_SH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_HIGH_SH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_HIGH_SH_IDX].dpms_on_cm_idx = 2;
	/* low mh */
	rdev->pm.profiles[PM_PROFILE_LOW_MH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_LOW_MH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_LOW_MH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_LOW_MH_IDX].dpms_on_cm_idx = 0;
	/* mid mh */
	rdev->pm.profiles[PM_PROFILE_MID_MH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_MID_MH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_MID_MH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_MID_MH_IDX].dpms_on_cm_idx = 1;
	/* high mh */
	rdev->pm.profiles[PM_PROFILE_HIGH_MH_IDX].dpms_off_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_HIGH_MH_IDX].dpms_on_ps_idx = idx;
	rdev->pm.profiles[PM_PROFILE_HIGH_MH_IDX].dpms_off_cm_idx = 0;
	rdev->pm.profiles[PM_PROFILE_HIGH_MH_IDX].dpms_on_cm_idx = 2;
}

/**
 * evergreen_pm_misc - set additional pm hw parameters callback.
 *
 * @rdev: radeon_device pointer
 *
 * Set non-clock parameters associated with a power state
 * (voltage, etc.) (evergreen+).
 */
>>>>>>> refs/remotes/origin/master
void evergreen_pm_misc(struct radeon_device *rdev)
{
	int req_ps_idx = rdev->pm.requested_power_state_index;
	int req_cm_idx = rdev->pm.requested_clock_mode_index;
	struct radeon_power_state *ps = &rdev->pm.power_state[req_ps_idx];
	struct radeon_voltage *voltage = &ps->clock_info[req_cm_idx].voltage;

	if (voltage->type == VOLTAGE_SW) {
<<<<<<< HEAD
		/* 0xff01 is a flag rather then an actual voltage */
		if (voltage->voltage == 0xff01)
=======
		/* 0xff0x are flags rather then an actual voltage */
		if ((voltage->voltage & 0xff00) == 0xff00)
>>>>>>> refs/remotes/origin/master
			return;
		if (voltage->voltage && (voltage->voltage != rdev->pm.current_vddc)) {
			radeon_atom_set_voltage(rdev, voltage->voltage, SET_VOLTAGE_TYPE_ASIC_VDDC);
			rdev->pm.current_vddc = voltage->voltage;
			DRM_DEBUG("Setting: vddc: %d\n", voltage->voltage);
		}
<<<<<<< HEAD
		/* 0xff01 is a flag rather then an actual voltage */
		if (voltage->vddci == 0xff01)
=======

		/* starting with BTC, there is one state that is used for both
		 * MH and SH.  Difference is that we always use the high clock index for
		 * mclk and vddci.
		 */
		if ((rdev->pm.pm_method == PM_METHOD_PROFILE) &&
		    (rdev->family >= CHIP_BARTS) &&
		    rdev->pm.active_crtc_count &&
		    ((rdev->pm.profile_index == PM_PROFILE_MID_MH_IDX) ||
		     (rdev->pm.profile_index == PM_PROFILE_LOW_MH_IDX)))
			voltage = &rdev->pm.power_state[req_ps_idx].
				clock_info[rdev->pm.profiles[PM_PROFILE_HIGH_MH_IDX].dpms_on_cm_idx].voltage;

		/* 0xff0x are flags rather then an actual voltage */
		if ((voltage->vddci & 0xff00) == 0xff00)
>>>>>>> refs/remotes/origin/master
			return;
		if (voltage->vddci && (voltage->vddci != rdev->pm.current_vddci)) {
			radeon_atom_set_voltage(rdev, voltage->vddci, SET_VOLTAGE_TYPE_ASIC_VDDCI);
			rdev->pm.current_vddci = voltage->vddci;
			DRM_DEBUG("Setting: vddci: %d\n", voltage->vddci);
		}
	}
}

<<<<<<< HEAD
=======
/**
 * evergreen_pm_prepare - pre-power state change callback.
 *
 * @rdev: radeon_device pointer
 *
 * Prepare for a power state change (evergreen+).
 */
>>>>>>> refs/remotes/origin/master
void evergreen_pm_prepare(struct radeon_device *rdev)
{
	struct drm_device *ddev = rdev->ddev;
	struct drm_crtc *crtc;
	struct radeon_crtc *radeon_crtc;
	u32 tmp;

	/* disable any active CRTCs */
	list_for_each_entry(crtc, &ddev->mode_config.crtc_list, head) {
		radeon_crtc = to_radeon_crtc(crtc);
		if (radeon_crtc->enabled) {
			tmp = RREG32(EVERGREEN_CRTC_CONTROL + radeon_crtc->crtc_offset);
			tmp |= EVERGREEN_CRTC_DISP_READ_REQUEST_DISABLE;
			WREG32(EVERGREEN_CRTC_CONTROL + radeon_crtc->crtc_offset, tmp);
		}
	}
}

<<<<<<< HEAD
=======
/**
 * evergreen_pm_finish - post-power state change callback.
 *
 * @rdev: radeon_device pointer
 *
 * Clean up after a power state change (evergreen+).
 */
>>>>>>> refs/remotes/origin/master
void evergreen_pm_finish(struct radeon_device *rdev)
{
	struct drm_device *ddev = rdev->ddev;
	struct drm_crtc *crtc;
	struct radeon_crtc *radeon_crtc;
	u32 tmp;

	/* enable any active CRTCs */
	list_for_each_entry(crtc, &ddev->mode_config.crtc_list, head) {
		radeon_crtc = to_radeon_crtc(crtc);
		if (radeon_crtc->enabled) {
			tmp = RREG32(EVERGREEN_CRTC_CONTROL + radeon_crtc->crtc_offset);
			tmp &= ~EVERGREEN_CRTC_DISP_READ_REQUEST_DISABLE;
			WREG32(EVERGREEN_CRTC_CONTROL + radeon_crtc->crtc_offset, tmp);
		}
	}
}

<<<<<<< HEAD
=======
/**
 * evergreen_hpd_sense - hpd sense callback.
 *
 * @rdev: radeon_device pointer
 * @hpd: hpd (hotplug detect) pin
 *
 * Checks if a digital monitor is connected (evergreen+).
 * Returns true if connected, false if not connected.
 */
>>>>>>> refs/remotes/origin/master
bool evergreen_hpd_sense(struct radeon_device *rdev, enum radeon_hpd_id hpd)
{
	bool connected = false;

	switch (hpd) {
	case RADEON_HPD_1:
		if (RREG32(DC_HPD1_INT_STATUS) & DC_HPDx_SENSE)
			connected = true;
		break;
	case RADEON_HPD_2:
		if (RREG32(DC_HPD2_INT_STATUS) & DC_HPDx_SENSE)
			connected = true;
		break;
	case RADEON_HPD_3:
		if (RREG32(DC_HPD3_INT_STATUS) & DC_HPDx_SENSE)
			connected = true;
		break;
	case RADEON_HPD_4:
		if (RREG32(DC_HPD4_INT_STATUS) & DC_HPDx_SENSE)
			connected = true;
		break;
	case RADEON_HPD_5:
		if (RREG32(DC_HPD5_INT_STATUS) & DC_HPDx_SENSE)
			connected = true;
		break;
	case RADEON_HPD_6:
		if (RREG32(DC_HPD6_INT_STATUS) & DC_HPDx_SENSE)
			connected = true;
			break;
	default:
		break;
	}

	return connected;
}

<<<<<<< HEAD
=======
/**
 * evergreen_hpd_set_polarity - hpd set polarity callback.
 *
 * @rdev: radeon_device pointer
 * @hpd: hpd (hotplug detect) pin
 *
 * Set the polarity of the hpd pin (evergreen+).
 */
>>>>>>> refs/remotes/origin/master
void evergreen_hpd_set_polarity(struct radeon_device *rdev,
				enum radeon_hpd_id hpd)
{
	u32 tmp;
	bool connected = evergreen_hpd_sense(rdev, hpd);

	switch (hpd) {
	case RADEON_HPD_1:
		tmp = RREG32(DC_HPD1_INT_CONTROL);
		if (connected)
			tmp &= ~DC_HPDx_INT_POLARITY;
		else
			tmp |= DC_HPDx_INT_POLARITY;
		WREG32(DC_HPD1_INT_CONTROL, tmp);
		break;
	case RADEON_HPD_2:
		tmp = RREG32(DC_HPD2_INT_CONTROL);
		if (connected)
			tmp &= ~DC_HPDx_INT_POLARITY;
		else
			tmp |= DC_HPDx_INT_POLARITY;
		WREG32(DC_HPD2_INT_CONTROL, tmp);
		break;
	case RADEON_HPD_3:
		tmp = RREG32(DC_HPD3_INT_CONTROL);
		if (connected)
			tmp &= ~DC_HPDx_INT_POLARITY;
		else
			tmp |= DC_HPDx_INT_POLARITY;
		WREG32(DC_HPD3_INT_CONTROL, tmp);
		break;
	case RADEON_HPD_4:
		tmp = RREG32(DC_HPD4_INT_CONTROL);
		if (connected)
			tmp &= ~DC_HPDx_INT_POLARITY;
		else
			tmp |= DC_HPDx_INT_POLARITY;
		WREG32(DC_HPD4_INT_CONTROL, tmp);
		break;
	case RADEON_HPD_5:
		tmp = RREG32(DC_HPD5_INT_CONTROL);
		if (connected)
			tmp &= ~DC_HPDx_INT_POLARITY;
		else
			tmp |= DC_HPDx_INT_POLARITY;
		WREG32(DC_HPD5_INT_CONTROL, tmp);
			break;
	case RADEON_HPD_6:
		tmp = RREG32(DC_HPD6_INT_CONTROL);
		if (connected)
			tmp &= ~DC_HPDx_INT_POLARITY;
		else
			tmp |= DC_HPDx_INT_POLARITY;
		WREG32(DC_HPD6_INT_CONTROL, tmp);
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
=======
/**
 * evergreen_hpd_init - hpd setup callback.
 *
 * @rdev: radeon_device pointer
 *
 * Setup the hpd pins used by the card (evergreen+).
 * Enable the pin, set the polarity, and enable the hpd interrupts.
 */
>>>>>>> refs/remotes/origin/master
void evergreen_hpd_init(struct radeon_device *rdev)
{
	struct drm_device *dev = rdev->ddev;
	struct drm_connector *connector;
<<<<<<< HEAD
=======
	unsigned enabled = 0;
>>>>>>> refs/remotes/origin/master
	u32 tmp = DC_HPDx_CONNECTION_TIMER(0x9c4) |
		DC_HPDx_RX_INT_TIMER(0xfa) | DC_HPDx_EN;

	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		struct radeon_connector *radeon_connector = to_radeon_connector(connector);

		if (connector->connector_type == DRM_MODE_CONNECTOR_eDP ||
		    connector->connector_type == DRM_MODE_CONNECTOR_LVDS) {
			/* don't try to enable hpd on eDP or LVDS avoid breaking the
			 * aux dp channel on imac and help (but not completely fix)
			 * https://bugzilla.redhat.com/show_bug.cgi?id=726143
			 * also avoid interrupt storms during dpms.
			 */
			continue;
		}
		switch (radeon_connector->hpd.hpd) {
		case RADEON_HPD_1:
			WREG32(DC_HPD1_CONTROL, tmp);
<<<<<<< HEAD
			rdev->irq.hpd[0] = true;
			break;
		case RADEON_HPD_2:
			WREG32(DC_HPD2_CONTROL, tmp);
			rdev->irq.hpd[1] = true;
			break;
		case RADEON_HPD_3:
			WREG32(DC_HPD3_CONTROL, tmp);
			rdev->irq.hpd[2] = true;
			break;
		case RADEON_HPD_4:
			WREG32(DC_HPD4_CONTROL, tmp);
			rdev->irq.hpd[3] = true;
			break;
		case RADEON_HPD_5:
			WREG32(DC_HPD5_CONTROL, tmp);
			rdev->irq.hpd[4] = true;
			break;
		case RADEON_HPD_6:
			WREG32(DC_HPD6_CONTROL, tmp);
			rdev->irq.hpd[5] = true;
=======
			break;
		case RADEON_HPD_2:
			WREG32(DC_HPD2_CONTROL, tmp);
			break;
		case RADEON_HPD_3:
			WREG32(DC_HPD3_CONTROL, tmp);
			break;
		case RADEON_HPD_4:
			WREG32(DC_HPD4_CONTROL, tmp);
			break;
		case RADEON_HPD_5:
			WREG32(DC_HPD5_CONTROL, tmp);
			break;
		case RADEON_HPD_6:
			WREG32(DC_HPD6_CONTROL, tmp);
>>>>>>> refs/remotes/origin/master
			break;
		default:
			break;
		}
		radeon_hpd_set_polarity(rdev, radeon_connector->hpd.hpd);
<<<<<<< HEAD
	}
	if (rdev->irq.installed)
		evergreen_irq_set(rdev);
}

=======
		enabled |= 1 << radeon_connector->hpd.hpd;
	}
	radeon_irq_kms_enable_hpd(rdev, enabled);
}

/**
 * evergreen_hpd_fini - hpd tear down callback.
 *
 * @rdev: radeon_device pointer
 *
 * Tear down the hpd pins used by the card (evergreen+).
 * Disable the hpd interrupts.
 */
>>>>>>> refs/remotes/origin/master
void evergreen_hpd_fini(struct radeon_device *rdev)
{
	struct drm_device *dev = rdev->ddev;
	struct drm_connector *connector;
<<<<<<< HEAD
=======
	unsigned disabled = 0;
>>>>>>> refs/remotes/origin/master

	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		struct radeon_connector *radeon_connector = to_radeon_connector(connector);
		switch (radeon_connector->hpd.hpd) {
		case RADEON_HPD_1:
			WREG32(DC_HPD1_CONTROL, 0);
<<<<<<< HEAD
			rdev->irq.hpd[0] = false;
			break;
		case RADEON_HPD_2:
			WREG32(DC_HPD2_CONTROL, 0);
			rdev->irq.hpd[1] = false;
			break;
		case RADEON_HPD_3:
			WREG32(DC_HPD3_CONTROL, 0);
			rdev->irq.hpd[2] = false;
			break;
		case RADEON_HPD_4:
			WREG32(DC_HPD4_CONTROL, 0);
			rdev->irq.hpd[3] = false;
			break;
		case RADEON_HPD_5:
			WREG32(DC_HPD5_CONTROL, 0);
			rdev->irq.hpd[4] = false;
			break;
		case RADEON_HPD_6:
			WREG32(DC_HPD6_CONTROL, 0);
			rdev->irq.hpd[5] = false;
=======
			break;
		case RADEON_HPD_2:
			WREG32(DC_HPD2_CONTROL, 0);
			break;
		case RADEON_HPD_3:
			WREG32(DC_HPD3_CONTROL, 0);
			break;
		case RADEON_HPD_4:
			WREG32(DC_HPD4_CONTROL, 0);
			break;
		case RADEON_HPD_5:
			WREG32(DC_HPD5_CONTROL, 0);
			break;
		case RADEON_HPD_6:
			WREG32(DC_HPD6_CONTROL, 0);
>>>>>>> refs/remotes/origin/master
			break;
		default:
			break;
		}
<<<<<<< HEAD
	}
=======
		disabled |= 1 << radeon_connector->hpd.hpd;
	}
	radeon_irq_kms_disable_hpd(rdev, disabled);
>>>>>>> refs/remotes/origin/master
}

/* watermark setup */

static u32 evergreen_line_buffer_adjust(struct radeon_device *rdev,
					struct radeon_crtc *radeon_crtc,
					struct drm_display_mode *mode,
					struct drm_display_mode *other_mode)
{
	u32 tmp, buffer_alloc, i;
	u32 pipe_offset = radeon_crtc->crtc_id * 0x20;
	/*
	 * Line Buffer Setup
	 * There are 3 line buffers, each one shared by 2 display controllers.
	 * DC_LB_MEMORY_SPLIT controls how that line buffer is shared between
	 * the display controllers.  The paritioning is done via one of four
	 * preset allocations specified in bits 2:0:
	 * first display controller
	 *  0 - first half of lb (3840 * 2)
	 *  1 - first 3/4 of lb (5760 * 2)
	 *  2 - whole lb (7680 * 2), other crtc must be disabled
	 *  3 - first 1/4 of lb (1920 * 2)
	 * second display controller
	 *  4 - second half of lb (3840 * 2)
	 *  5 - second 3/4 of lb (5760 * 2)
	 *  6 - whole lb (7680 * 2), other crtc must be disabled
	 *  7 - last 1/4 of lb (1920 * 2)
	 */
	/* this can get tricky if we have two large displays on a paired group
	 * of crtcs.  Ideally for multiple large displays we'd assign them to
	 * non-linked crtcs for maximum line buffer allocation.
	 */
	if (radeon_crtc->base.enabled && mode) {
		if (other_mode) {
			tmp = 0; /* 1/2 */
			buffer_alloc = 1;
		} else {
			tmp = 2; /* whole */
			buffer_alloc = 2;
		}
	} else {
		tmp = 0;
		buffer_alloc = 0;
	}

	/* second controller of the pair uses second half of the lb */
	if (radeon_crtc->crtc_id % 2)
		tmp += 4;
	WREG32(DC_LB_MEMORY_SPLIT + radeon_crtc->crtc_offset, tmp);

	if (ASIC_IS_DCE41(rdev) || ASIC_IS_DCE5(rdev)) {
		WREG32(PIPE0_DMIF_BUFFER_CONTROL + pipe_offset,
		       DMIF_BUFFERS_ALLOCATED(buffer_alloc));
		for (i = 0; i < rdev->usec_timeout; i++) {
			if (RREG32(PIPE0_DMIF_BUFFER_CONTROL + pipe_offset) &
			    DMIF_BUFFERS_ALLOCATED_COMPLETED)
				break;
			udelay(1);
		}
	}

	if (radeon_crtc->base.enabled && mode) {
		switch (tmp) {
		case 0:
		case 4:
		default:
			if (ASIC_IS_DCE5(rdev))
				return 4096 * 2;
			else
				return 3840 * 2;
		case 1:
		case 5:
			if (ASIC_IS_DCE5(rdev))
				return 6144 * 2;
			else
				return 5760 * 2;
		case 2:
		case 6:
			if (ASIC_IS_DCE5(rdev))
				return 8192 * 2;
			else
				return 7680 * 2;
		case 3:
		case 7:
			if (ASIC_IS_DCE5(rdev))
				return 2048 * 2;
			else
				return 1920 * 2;
		}
	}

	/* controller not enabled, so no lb used */
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 evergreen_get_number_of_dram_channels(struct radeon_device *rdev)
=======
u32 evergreen_get_number_of_dram_channels(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
u32 evergreen_get_number_of_dram_channels(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/master
{
	u32 tmp = RREG32(MC_SHARED_CHMAP);

	switch ((tmp & NOOFCHAN_MASK) >> NOOFCHAN_SHIFT) {
	case 0:
	default:
		return 1;
	case 1:
		return 2;
	case 2:
		return 4;
	case 3:
		return 8;
	}
}

struct evergreen_wm_params {
	u32 dram_channels; /* number of dram channels */
	u32 yclk;          /* bandwidth per dram data pin in kHz */
	u32 sclk;          /* engine clock in kHz */
	u32 disp_clk;      /* display clock in kHz */
	u32 src_width;     /* viewport width */
	u32 active_time;   /* active display time in ns */
	u32 blank_time;    /* blank time in ns */
	bool interlaced;    /* mode is interlaced */
	fixed20_12 vsc;    /* vertical scale ratio */
	u32 num_heads;     /* number of active crtcs */
	u32 bytes_per_pixel; /* bytes per pixel display + overlay */
	u32 lb_size;       /* line buffer allocated to pipe */
	u32 vtaps;         /* vertical scaler taps */
};

static u32 evergreen_dram_bandwidth(struct evergreen_wm_params *wm)
{
	/* Calculate DRAM Bandwidth and the part allocated to display. */
	fixed20_12 dram_efficiency; /* 0.7 */
	fixed20_12 yclk, dram_channels, bandwidth;
	fixed20_12 a;

	a.full = dfixed_const(1000);
	yclk.full = dfixed_const(wm->yclk);
	yclk.full = dfixed_div(yclk, a);
	dram_channels.full = dfixed_const(wm->dram_channels * 4);
	a.full = dfixed_const(10);
	dram_efficiency.full = dfixed_const(7);
	dram_efficiency.full = dfixed_div(dram_efficiency, a);
	bandwidth.full = dfixed_mul(dram_channels, yclk);
	bandwidth.full = dfixed_mul(bandwidth, dram_efficiency);

	return dfixed_trunc(bandwidth);
}

static u32 evergreen_dram_bandwidth_for_display(struct evergreen_wm_params *wm)
{
	/* Calculate DRAM Bandwidth and the part allocated to display. */
	fixed20_12 disp_dram_allocation; /* 0.3 to 0.7 */
	fixed20_12 yclk, dram_channels, bandwidth;
	fixed20_12 a;

	a.full = dfixed_const(1000);
	yclk.full = dfixed_const(wm->yclk);
	yclk.full = dfixed_div(yclk, a);
	dram_channels.full = dfixed_const(wm->dram_channels * 4);
	a.full = dfixed_const(10);
	disp_dram_allocation.full = dfixed_const(3); /* XXX worse case value 0.3 */
	disp_dram_allocation.full = dfixed_div(disp_dram_allocation, a);
	bandwidth.full = dfixed_mul(dram_channels, yclk);
	bandwidth.full = dfixed_mul(bandwidth, disp_dram_allocation);

	return dfixed_trunc(bandwidth);
}

static u32 evergreen_data_return_bandwidth(struct evergreen_wm_params *wm)
{
	/* Calculate the display Data return Bandwidth */
	fixed20_12 return_efficiency; /* 0.8 */
	fixed20_12 sclk, bandwidth;
	fixed20_12 a;

	a.full = dfixed_const(1000);
	sclk.full = dfixed_const(wm->sclk);
	sclk.full = dfixed_div(sclk, a);
	a.full = dfixed_const(10);
	return_efficiency.full = dfixed_const(8);
	return_efficiency.full = dfixed_div(return_efficiency, a);
	a.full = dfixed_const(32);
	bandwidth.full = dfixed_mul(a, sclk);
	bandwidth.full = dfixed_mul(bandwidth, return_efficiency);

	return dfixed_trunc(bandwidth);
}

static u32 evergreen_dmif_request_bandwidth(struct evergreen_wm_params *wm)
{
	/* Calculate the DMIF Request Bandwidth */
	fixed20_12 disp_clk_request_efficiency; /* 0.8 */
	fixed20_12 disp_clk, bandwidth;
	fixed20_12 a;

	a.full = dfixed_const(1000);
	disp_clk.full = dfixed_const(wm->disp_clk);
	disp_clk.full = dfixed_div(disp_clk, a);
	a.full = dfixed_const(10);
	disp_clk_request_efficiency.full = dfixed_const(8);
	disp_clk_request_efficiency.full = dfixed_div(disp_clk_request_efficiency, a);
	a.full = dfixed_const(32);
	bandwidth.full = dfixed_mul(a, disp_clk);
	bandwidth.full = dfixed_mul(bandwidth, disp_clk_request_efficiency);

	return dfixed_trunc(bandwidth);
}

static u32 evergreen_available_bandwidth(struct evergreen_wm_params *wm)
{
	/* Calculate the Available bandwidth. Display can use this temporarily but not in average. */
	u32 dram_bandwidth = evergreen_dram_bandwidth(wm);
	u32 data_return_bandwidth = evergreen_data_return_bandwidth(wm);
	u32 dmif_req_bandwidth = evergreen_dmif_request_bandwidth(wm);

	return min(dram_bandwidth, min(data_return_bandwidth, dmif_req_bandwidth));
}

static u32 evergreen_average_bandwidth(struct evergreen_wm_params *wm)
{
	/* Calculate the display mode Average Bandwidth
	 * DisplayMode should contain the source and destination dimensions,
	 * timing, etc.
	 */
	fixed20_12 bpp;
	fixed20_12 line_time;
	fixed20_12 src_width;
	fixed20_12 bandwidth;
	fixed20_12 a;

	a.full = dfixed_const(1000);
	line_time.full = dfixed_const(wm->active_time + wm->blank_time);
	line_time.full = dfixed_div(line_time, a);
	bpp.full = dfixed_const(wm->bytes_per_pixel);
	src_width.full = dfixed_const(wm->src_width);
	bandwidth.full = dfixed_mul(src_width, bpp);
	bandwidth.full = dfixed_mul(bandwidth, wm->vsc);
	bandwidth.full = dfixed_div(bandwidth, line_time);

	return dfixed_trunc(bandwidth);
}

static u32 evergreen_latency_watermark(struct evergreen_wm_params *wm)
{
	/* First calcualte the latency in ns */
	u32 mc_latency = 2000; /* 2000 ns. */
	u32 available_bandwidth = evergreen_available_bandwidth(wm);
	u32 worst_chunk_return_time = (512 * 8 * 1000) / available_bandwidth;
	u32 cursor_line_pair_return_time = (128 * 4 * 1000) / available_bandwidth;
	u32 dc_latency = 40000000 / wm->disp_clk; /* dc pipe latency */
	u32 other_heads_data_return_time = ((wm->num_heads + 1) * worst_chunk_return_time) +
		(wm->num_heads * cursor_line_pair_return_time);
	u32 latency = mc_latency + other_heads_data_return_time + dc_latency;
	u32 max_src_lines_per_dst_line, lb_fill_bw, line_fill_time;
	fixed20_12 a, b, c;

	if (wm->num_heads == 0)
		return 0;

	a.full = dfixed_const(2);
	b.full = dfixed_const(1);
	if ((wm->vsc.full > a.full) ||
	    ((wm->vsc.full > b.full) && (wm->vtaps >= 3)) ||
	    (wm->vtaps >= 5) ||
	    ((wm->vsc.full >= a.full) && wm->interlaced))
		max_src_lines_per_dst_line = 4;
	else
		max_src_lines_per_dst_line = 2;

	a.full = dfixed_const(available_bandwidth);
	b.full = dfixed_const(wm->num_heads);
	a.full = dfixed_div(a, b);

	b.full = dfixed_const(1000);
	c.full = dfixed_const(wm->disp_clk);
	b.full = dfixed_div(c, b);
	c.full = dfixed_const(wm->bytes_per_pixel);
	b.full = dfixed_mul(b, c);

	lb_fill_bw = min(dfixed_trunc(a), dfixed_trunc(b));

	a.full = dfixed_const(max_src_lines_per_dst_line * wm->src_width * wm->bytes_per_pixel);
	b.full = dfixed_const(1000);
	c.full = dfixed_const(lb_fill_bw);
	b.full = dfixed_div(c, b);
	a.full = dfixed_div(a, b);
	line_fill_time = dfixed_trunc(a);

	if (line_fill_time < wm->active_time)
		return latency;
	else
		return latency + (line_fill_time - wm->active_time);

}

static bool evergreen_average_bandwidth_vs_dram_bandwidth_for_display(struct evergreen_wm_params *wm)
{
	if (evergreen_average_bandwidth(wm) <=
	    (evergreen_dram_bandwidth_for_display(wm) / wm->num_heads))
		return true;
	else
		return false;
};

static bool evergreen_average_bandwidth_vs_available_bandwidth(struct evergreen_wm_params *wm)
{
	if (evergreen_average_bandwidth(wm) <=
	    (evergreen_available_bandwidth(wm) / wm->num_heads))
		return true;
	else
		return false;
};

static bool evergreen_check_latency_hiding(struct evergreen_wm_params *wm)
{
	u32 lb_partitions = wm->lb_size / wm->src_width;
	u32 line_time = wm->active_time + wm->blank_time;
	u32 latency_tolerant_lines;
	u32 latency_hiding;
	fixed20_12 a;

	a.full = dfixed_const(1);
	if (wm->vsc.full > a.full)
		latency_tolerant_lines = 1;
	else {
		if (lb_partitions <= (wm->vtaps + 1))
			latency_tolerant_lines = 1;
		else
			latency_tolerant_lines = 2;
	}

	latency_hiding = (latency_tolerant_lines * line_time + wm->blank_time);

	if (evergreen_latency_watermark(wm) <= latency_hiding)
		return true;
	else
		return false;
}

static void evergreen_program_watermarks(struct radeon_device *rdev,
					 struct radeon_crtc *radeon_crtc,
					 u32 lb_size, u32 num_heads)
{
	struct drm_display_mode *mode = &radeon_crtc->base.mode;
<<<<<<< HEAD
	struct evergreen_wm_params wm;
=======
	struct evergreen_wm_params wm_low, wm_high;
	u32 dram_channels;
>>>>>>> refs/remotes/origin/master
	u32 pixel_period;
	u32 line_time = 0;
	u32 latency_watermark_a = 0, latency_watermark_b = 0;
	u32 priority_a_mark = 0, priority_b_mark = 0;
	u32 priority_a_cnt = PRIORITY_OFF;
	u32 priority_b_cnt = PRIORITY_OFF;
	u32 pipe_offset = radeon_crtc->crtc_id * 16;
	u32 tmp, arb_control3;
	fixed20_12 a, b, c;

	if (radeon_crtc->base.enabled && num_heads && mode) {
		pixel_period = 1000000 / (u32)mode->clock;
		line_time = min((u32)mode->crtc_htotal * pixel_period, (u32)65535);
		priority_a_cnt = 0;
		priority_b_cnt = 0;
<<<<<<< HEAD

		wm.yclk = rdev->pm.current_mclk * 10;
		wm.sclk = rdev->pm.current_sclk * 10;
		wm.disp_clk = mode->clock;
		wm.src_width = mode->crtc_hdisplay;
		wm.active_time = mode->crtc_hdisplay * pixel_period;
		wm.blank_time = line_time - wm.active_time;
		wm.interlaced = false;
		if (mode->flags & DRM_MODE_FLAG_INTERLACE)
			wm.interlaced = true;
		wm.vsc = radeon_crtc->vsc;
		wm.vtaps = 1;
		if (radeon_crtc->rmx_type != RMX_OFF)
			wm.vtaps = 2;
		wm.bytes_per_pixel = 4; /* XXX: get this from fb config */
		wm.lb_size = lb_size;
		wm.dram_channels = evergreen_get_number_of_dram_channels(rdev);
		wm.num_heads = num_heads;

		/* set for high clocks */
		latency_watermark_a = min(evergreen_latency_watermark(&wm), (u32)65535);
		/* set for low clocks */
		/* wm.yclk = low clk; wm.sclk = low clk */
		latency_watermark_b = min(evergreen_latency_watermark(&wm), (u32)65535);

		/* possibly force display priority to high */
		/* should really do this at mode validation time... */
		if (!evergreen_average_bandwidth_vs_dram_bandwidth_for_display(&wm) ||
		    !evergreen_average_bandwidth_vs_available_bandwidth(&wm) ||
		    !evergreen_check_latency_hiding(&wm) ||
		    (rdev->disp_priority == 2)) {
<<<<<<< HEAD
			DRM_INFO("force priority to high\n");
=======
			DRM_DEBUG_KMS("force priority to high\n");
>>>>>>> refs/remotes/origin/cm-10.0
			priority_a_cnt |= PRIORITY_ALWAYS_ON;
=======
		dram_channels = evergreen_get_number_of_dram_channels(rdev);

		/* watermark for high clocks */
		if ((rdev->pm.pm_method == PM_METHOD_DPM) && rdev->pm.dpm_enabled) {
			wm_high.yclk =
				radeon_dpm_get_mclk(rdev, false) * 10;
			wm_high.sclk =
				radeon_dpm_get_sclk(rdev, false) * 10;
		} else {
			wm_high.yclk = rdev->pm.current_mclk * 10;
			wm_high.sclk = rdev->pm.current_sclk * 10;
		}

		wm_high.disp_clk = mode->clock;
		wm_high.src_width = mode->crtc_hdisplay;
		wm_high.active_time = mode->crtc_hdisplay * pixel_period;
		wm_high.blank_time = line_time - wm_high.active_time;
		wm_high.interlaced = false;
		if (mode->flags & DRM_MODE_FLAG_INTERLACE)
			wm_high.interlaced = true;
		wm_high.vsc = radeon_crtc->vsc;
		wm_high.vtaps = 1;
		if (radeon_crtc->rmx_type != RMX_OFF)
			wm_high.vtaps = 2;
		wm_high.bytes_per_pixel = 4; /* XXX: get this from fb config */
		wm_high.lb_size = lb_size;
		wm_high.dram_channels = dram_channels;
		wm_high.num_heads = num_heads;

		/* watermark for low clocks */
		if ((rdev->pm.pm_method == PM_METHOD_DPM) && rdev->pm.dpm_enabled) {
			wm_low.yclk =
				radeon_dpm_get_mclk(rdev, true) * 10;
			wm_low.sclk =
				radeon_dpm_get_sclk(rdev, true) * 10;
		} else {
			wm_low.yclk = rdev->pm.current_mclk * 10;
			wm_low.sclk = rdev->pm.current_sclk * 10;
		}

		wm_low.disp_clk = mode->clock;
		wm_low.src_width = mode->crtc_hdisplay;
		wm_low.active_time = mode->crtc_hdisplay * pixel_period;
		wm_low.blank_time = line_time - wm_low.active_time;
		wm_low.interlaced = false;
		if (mode->flags & DRM_MODE_FLAG_INTERLACE)
			wm_low.interlaced = true;
		wm_low.vsc = radeon_crtc->vsc;
		wm_low.vtaps = 1;
		if (radeon_crtc->rmx_type != RMX_OFF)
			wm_low.vtaps = 2;
		wm_low.bytes_per_pixel = 4; /* XXX: get this from fb config */
		wm_low.lb_size = lb_size;
		wm_low.dram_channels = dram_channels;
		wm_low.num_heads = num_heads;

		/* set for high clocks */
		latency_watermark_a = min(evergreen_latency_watermark(&wm_high), (u32)65535);
		/* set for low clocks */
		latency_watermark_b = min(evergreen_latency_watermark(&wm_low), (u32)65535);

		/* possibly force display priority to high */
		/* should really do this at mode validation time... */
		if (!evergreen_average_bandwidth_vs_dram_bandwidth_for_display(&wm_high) ||
		    !evergreen_average_bandwidth_vs_available_bandwidth(&wm_high) ||
		    !evergreen_check_latency_hiding(&wm_high) ||
		    (rdev->disp_priority == 2)) {
			DRM_DEBUG_KMS("force priority a to high\n");
			priority_a_cnt |= PRIORITY_ALWAYS_ON;
		}
		if (!evergreen_average_bandwidth_vs_dram_bandwidth_for_display(&wm_low) ||
		    !evergreen_average_bandwidth_vs_available_bandwidth(&wm_low) ||
		    !evergreen_check_latency_hiding(&wm_low) ||
		    (rdev->disp_priority == 2)) {
			DRM_DEBUG_KMS("force priority b to high\n");
>>>>>>> refs/remotes/origin/master
			priority_b_cnt |= PRIORITY_ALWAYS_ON;
		}

		a.full = dfixed_const(1000);
		b.full = dfixed_const(mode->clock);
		b.full = dfixed_div(b, a);
		c.full = dfixed_const(latency_watermark_a);
		c.full = dfixed_mul(c, b);
		c.full = dfixed_mul(c, radeon_crtc->hsc);
		c.full = dfixed_div(c, a);
		a.full = dfixed_const(16);
		c.full = dfixed_div(c, a);
		priority_a_mark = dfixed_trunc(c);
		priority_a_cnt |= priority_a_mark & PRIORITY_MARK_MASK;

		a.full = dfixed_const(1000);
		b.full = dfixed_const(mode->clock);
		b.full = dfixed_div(b, a);
		c.full = dfixed_const(latency_watermark_b);
		c.full = dfixed_mul(c, b);
		c.full = dfixed_mul(c, radeon_crtc->hsc);
		c.full = dfixed_div(c, a);
		a.full = dfixed_const(16);
		c.full = dfixed_div(c, a);
		priority_b_mark = dfixed_trunc(c);
		priority_b_cnt |= priority_b_mark & PRIORITY_MARK_MASK;
	}

	/* select wm A */
	arb_control3 = RREG32(PIPE0_ARBITRATION_CONTROL3 + pipe_offset);
	tmp = arb_control3;
	tmp &= ~LATENCY_WATERMARK_MASK(3);
	tmp |= LATENCY_WATERMARK_MASK(1);
	WREG32(PIPE0_ARBITRATION_CONTROL3 + pipe_offset, tmp);
	WREG32(PIPE0_LATENCY_CONTROL + pipe_offset,
	       (LATENCY_LOW_WATERMARK(latency_watermark_a) |
		LATENCY_HIGH_WATERMARK(line_time)));
	/* select wm B */
	tmp = RREG32(PIPE0_ARBITRATION_CONTROL3 + pipe_offset);
	tmp &= ~LATENCY_WATERMARK_MASK(3);
	tmp |= LATENCY_WATERMARK_MASK(2);
	WREG32(PIPE0_ARBITRATION_CONTROL3 + pipe_offset, tmp);
	WREG32(PIPE0_LATENCY_CONTROL + pipe_offset,
	       (LATENCY_LOW_WATERMARK(latency_watermark_b) |
		LATENCY_HIGH_WATERMARK(line_time)));
	/* restore original selection */
	WREG32(PIPE0_ARBITRATION_CONTROL3 + pipe_offset, arb_control3);

	/* write the priority marks */
	WREG32(PRIORITY_A_CNT + radeon_crtc->crtc_offset, priority_a_cnt);
	WREG32(PRIORITY_B_CNT + radeon_crtc->crtc_offset, priority_b_cnt);

<<<<<<< HEAD
}

=======
	/* save values for DPM */
	radeon_crtc->line_time = line_time;
	radeon_crtc->wm_high = latency_watermark_a;
	radeon_crtc->wm_low = latency_watermark_b;
}

/**
 * evergreen_bandwidth_update - update display watermarks callback.
 *
 * @rdev: radeon_device pointer
 *
 * Update the display watermarks based on the requested mode(s)
 * (evergreen+).
 */
>>>>>>> refs/remotes/origin/master
void evergreen_bandwidth_update(struct radeon_device *rdev)
{
	struct drm_display_mode *mode0 = NULL;
	struct drm_display_mode *mode1 = NULL;
	u32 num_heads = 0, lb_size;
	int i;

	radeon_update_display_priority(rdev);

	for (i = 0; i < rdev->num_crtc; i++) {
		if (rdev->mode_info.crtcs[i]->base.enabled)
			num_heads++;
	}
	for (i = 0; i < rdev->num_crtc; i += 2) {
		mode0 = &rdev->mode_info.crtcs[i]->base.mode;
		mode1 = &rdev->mode_info.crtcs[i+1]->base.mode;
		lb_size = evergreen_line_buffer_adjust(rdev, rdev->mode_info.crtcs[i], mode0, mode1);
		evergreen_program_watermarks(rdev, rdev->mode_info.crtcs[i], lb_size, num_heads);
		lb_size = evergreen_line_buffer_adjust(rdev, rdev->mode_info.crtcs[i+1], mode1, mode0);
		evergreen_program_watermarks(rdev, rdev->mode_info.crtcs[i+1], lb_size, num_heads);
	}
}

<<<<<<< HEAD
=======
/**
 * evergreen_mc_wait_for_idle - wait for MC idle callback.
 *
 * @rdev: radeon_device pointer
 *
 * Wait for the MC (memory controller) to be idle.
 * (evergreen+).
 * Returns 0 if the MC is idle, -1 if not.
 */
>>>>>>> refs/remotes/origin/master
int evergreen_mc_wait_for_idle(struct radeon_device *rdev)
{
	unsigned i;
	u32 tmp;

	for (i = 0; i < rdev->usec_timeout; i++) {
		/* read MC_STATUS */
		tmp = RREG32(SRBM_STATUS) & 0x1F00;
		if (!tmp)
			return 0;
		udelay(1);
	}
	return -1;
}

/*
 * GART
 */
void evergreen_pcie_gart_tlb_flush(struct radeon_device *rdev)
{
	unsigned i;
	u32 tmp;

	WREG32(HDP_MEM_COHERENCY_FLUSH_CNTL, 0x1);

	WREG32(VM_CONTEXT0_REQUEST_RESPONSE, REQUEST_TYPE(1));
	for (i = 0; i < rdev->usec_timeout; i++) {
		/* read MC_STATUS */
		tmp = RREG32(VM_CONTEXT0_REQUEST_RESPONSE);
		tmp = (tmp & RESPONSE_TYPE_MASK) >> RESPONSE_TYPE_SHIFT;
		if (tmp == 2) {
			printk(KERN_WARNING "[drm] r600 flush TLB failed\n");
			return;
		}
		if (tmp) {
			return;
		}
		udelay(1);
	}
}

<<<<<<< HEAD
int evergreen_pcie_gart_enable(struct radeon_device *rdev)
=======
static int evergreen_pcie_gart_enable(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/master
{
	u32 tmp;
	int r;

<<<<<<< HEAD
<<<<<<< HEAD
	if (rdev->gart.table.vram.robj == NULL) {
=======
	if (rdev->gart.robj == NULL) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (rdev->gart.robj == NULL) {
>>>>>>> refs/remotes/origin/master
		dev_err(rdev->dev, "No VRAM object for PCIE GART.\n");
		return -EINVAL;
	}
	r = radeon_gart_table_vram_pin(rdev);
	if (r)
		return r;
	radeon_gart_restore(rdev);
	/* Setup L2 cache */
	WREG32(VM_L2_CNTL, ENABLE_L2_CACHE | ENABLE_L2_FRAGMENT_PROCESSING |
				ENABLE_L2_PTE_CACHE_LRU_UPDATE_BY_WRITE |
				EFFECTIVE_L2_QUEUE_SIZE(7));
	WREG32(VM_L2_CNTL2, 0);
	WREG32(VM_L2_CNTL3, BANK_SELECT(0) | CACHE_UPDATE_MODE(2));
	/* Setup TLB control */
	tmp = ENABLE_L1_TLB | ENABLE_L1_FRAGMENT_PROCESSING |
		SYSTEM_ACCESS_MODE_NOT_IN_SYS |
		SYSTEM_APERTURE_UNMAPPED_ACCESS_PASS_THRU |
		EFFECTIVE_L1_TLB_SIZE(5) | EFFECTIVE_L1_QUEUE_SIZE(5);
	if (rdev->flags & RADEON_IS_IGP) {
		WREG32(FUS_MC_VM_MD_L1_TLB0_CNTL, tmp);
		WREG32(FUS_MC_VM_MD_L1_TLB1_CNTL, tmp);
		WREG32(FUS_MC_VM_MD_L1_TLB2_CNTL, tmp);
	} else {
		WREG32(MC_VM_MD_L1_TLB0_CNTL, tmp);
		WREG32(MC_VM_MD_L1_TLB1_CNTL, tmp);
		WREG32(MC_VM_MD_L1_TLB2_CNTL, tmp);
		if ((rdev->family == CHIP_JUNIPER) ||
		    (rdev->family == CHIP_CYPRESS) ||
		    (rdev->family == CHIP_HEMLOCK) ||
		    (rdev->family == CHIP_BARTS))
			WREG32(MC_VM_MD_L1_TLB3_CNTL, tmp);
	}
	WREG32(MC_VM_MB_L1_TLB0_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB1_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB2_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB3_CNTL, tmp);
	WREG32(VM_CONTEXT0_PAGE_TABLE_START_ADDR, rdev->mc.gtt_start >> 12);
	WREG32(VM_CONTEXT0_PAGE_TABLE_END_ADDR, rdev->mc.gtt_end >> 12);
	WREG32(VM_CONTEXT0_PAGE_TABLE_BASE_ADDR, rdev->gart.table_addr >> 12);
	WREG32(VM_CONTEXT0_CNTL, ENABLE_CONTEXT | PAGE_TABLE_DEPTH(0) |
				RANGE_PROTECTION_FAULT_ENABLE_DEFAULT);
	WREG32(VM_CONTEXT0_PROTECTION_FAULT_DEFAULT_ADDR,
			(u32)(rdev->dummy_page.addr >> 12));
	WREG32(VM_CONTEXT1_CNTL, 0);

	evergreen_pcie_gart_tlb_flush(rdev);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	DRM_INFO("PCIE GART of %uM enabled (table at 0x%016llX).\n",
		 (unsigned)(rdev->mc.gtt_size >> 20),
		 (unsigned long long)rdev->gart.table_addr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DRM_INFO("PCIE GART of %uM enabled (table at 0x%016llX).\n",
		 (unsigned)(rdev->mc.gtt_size >> 20),
		 (unsigned long long)rdev->gart.table_addr);
>>>>>>> refs/remotes/origin/master
	rdev->gart.ready = true;
	return 0;
}

<<<<<<< HEAD
void evergreen_pcie_gart_disable(struct radeon_device *rdev)
{
	u32 tmp;
<<<<<<< HEAD
	int r;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void evergreen_pcie_gart_disable(struct radeon_device *rdev)
{
	u32 tmp;
>>>>>>> refs/remotes/origin/master

	/* Disable all tables */
	WREG32(VM_CONTEXT0_CNTL, 0);
	WREG32(VM_CONTEXT1_CNTL, 0);

	/* Setup L2 cache */
	WREG32(VM_L2_CNTL, ENABLE_L2_FRAGMENT_PROCESSING |
				EFFECTIVE_L2_QUEUE_SIZE(7));
	WREG32(VM_L2_CNTL2, 0);
	WREG32(VM_L2_CNTL3, BANK_SELECT(0) | CACHE_UPDATE_MODE(2));
	/* Setup TLB control */
	tmp = EFFECTIVE_L1_TLB_SIZE(5) | EFFECTIVE_L1_QUEUE_SIZE(5);
	WREG32(MC_VM_MD_L1_TLB0_CNTL, tmp);
	WREG32(MC_VM_MD_L1_TLB1_CNTL, tmp);
	WREG32(MC_VM_MD_L1_TLB2_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB0_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB1_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB2_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB3_CNTL, tmp);
<<<<<<< HEAD
<<<<<<< HEAD
	if (rdev->gart.table.vram.robj) {
		r = radeon_bo_reserve(rdev->gart.table.vram.robj, false);
		if (likely(r == 0)) {
			radeon_bo_kunmap(rdev->gart.table.vram.robj);
			radeon_bo_unpin(rdev->gart.table.vram.robj);
			radeon_bo_unreserve(rdev->gart.table.vram.robj);
		}
	}
=======
	radeon_gart_table_vram_unpin(rdev);
>>>>>>> refs/remotes/origin/cm-10.0
}

void evergreen_pcie_gart_fini(struct radeon_device *rdev)
=======
	radeon_gart_table_vram_unpin(rdev);
}

static void evergreen_pcie_gart_fini(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/master
{
	evergreen_pcie_gart_disable(rdev);
	radeon_gart_table_vram_free(rdev);
	radeon_gart_fini(rdev);
}


<<<<<<< HEAD
void evergreen_agp_enable(struct radeon_device *rdev)
=======
static void evergreen_agp_enable(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/master
{
	u32 tmp;

	/* Setup L2 cache */
	WREG32(VM_L2_CNTL, ENABLE_L2_CACHE | ENABLE_L2_FRAGMENT_PROCESSING |
				ENABLE_L2_PTE_CACHE_LRU_UPDATE_BY_WRITE |
				EFFECTIVE_L2_QUEUE_SIZE(7));
	WREG32(VM_L2_CNTL2, 0);
	WREG32(VM_L2_CNTL3, BANK_SELECT(0) | CACHE_UPDATE_MODE(2));
	/* Setup TLB control */
	tmp = ENABLE_L1_TLB | ENABLE_L1_FRAGMENT_PROCESSING |
		SYSTEM_ACCESS_MODE_NOT_IN_SYS |
		SYSTEM_APERTURE_UNMAPPED_ACCESS_PASS_THRU |
		EFFECTIVE_L1_TLB_SIZE(5) | EFFECTIVE_L1_QUEUE_SIZE(5);
	WREG32(MC_VM_MD_L1_TLB0_CNTL, tmp);
	WREG32(MC_VM_MD_L1_TLB1_CNTL, tmp);
	WREG32(MC_VM_MD_L1_TLB2_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB0_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB1_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB2_CNTL, tmp);
	WREG32(MC_VM_MB_L1_TLB3_CNTL, tmp);
	WREG32(VM_CONTEXT0_CNTL, 0);
	WREG32(VM_CONTEXT1_CNTL, 0);
}

void evergreen_mc_stop(struct radeon_device *rdev, struct evergreen_mc_save *save)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	u32 crtc_enabled, tmp, frame_count, blackout;
	int i, j;

>>>>>>> refs/remotes/origin/cm-11.0
	save->vga_render_control = RREG32(VGA_RENDER_CONTROL);
	save->vga_hdp_control = RREG32(VGA_HDP_CONTROL);

	/* disable VGA render */
	WREG32(VGA_RENDER_CONTROL, 0);
	/* blank the display controllers */
	for (i = 0; i < rdev->num_crtc; i++) {
		crtc_enabled = RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i]) & EVERGREEN_CRTC_MASTER_EN;
		if (crtc_enabled) {
			save->crtc_enabled[i] = true;
			if (ASIC_IS_DCE6(rdev)) {
				tmp = RREG32(EVERGREEN_CRTC_BLANK_CONTROL + crtc_offsets[i]);
				if (!(tmp & EVERGREEN_CRTC_BLANK_DATA_EN)) {
					radeon_wait_for_vblank(rdev, i);
					WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 1);
					tmp |= EVERGREEN_CRTC_BLANK_DATA_EN;
					WREG32(EVERGREEN_CRTC_BLANK_CONTROL + crtc_offsets[i], tmp);
				}
			} else {
				tmp = RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i]);
				if (!(tmp & EVERGREEN_CRTC_DISP_READ_REQUEST_DISABLE)) {
					radeon_wait_for_vblank(rdev, i);
					WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 1);
					tmp |= EVERGREEN_CRTC_DISP_READ_REQUEST_DISABLE;
					WREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i], tmp);
					WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 0);
				}
			}
			/* wait for the next frame */
			frame_count = radeon_get_vblank_counter(rdev, i);
			for (j = 0; j < rdev->usec_timeout; j++) {
				if (radeon_get_vblank_counter(rdev, i) != frame_count)
					break;
				udelay(1);
			}

			/* XXX this is a hack to avoid strange behavior with EFI on certain systems */
			WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 1);
			tmp = RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i]);
			tmp &= ~EVERGREEN_CRTC_MASTER_EN;
			WREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i], tmp);
			WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 0);
			save->crtc_enabled[i] = false;
			/* ***** */
		} else {
			save->crtc_enabled[i] = false;
		}
	}

	radeon_mc_wait_for_idle(rdev);

	blackout = RREG32(MC_SHARED_BLACKOUT_CNTL);
	if ((blackout & BLACKOUT_MODE_MASK) != 1) {
		/* Block CPU access */
		WREG32(BIF_FB_EN, 0);
		/* blackout the MC */
		blackout &= ~BLACKOUT_MODE_MASK;
		WREG32(MC_SHARED_BLACKOUT_CNTL, blackout | 1);
	}
	/* wait for the MC to settle */
	udelay(100);

	/* lock double buffered regs */
	for (i = 0; i < rdev->num_crtc; i++) {
		if (save->crtc_enabled[i]) {
			tmp = RREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i]);
			if (!(tmp & EVERGREEN_GRPH_UPDATE_LOCK)) {
				tmp |= EVERGREEN_GRPH_UPDATE_LOCK;
				WREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i], tmp);
			}
			tmp = RREG32(EVERGREEN_MASTER_UPDATE_LOCK + crtc_offsets[i]);
			if (!(tmp & 1)) {
				tmp |= 1;
				WREG32(EVERGREEN_MASTER_UPDATE_LOCK + crtc_offsets[i], tmp);
			}
		}
	}
	/* wait for the MC to settle */
	udelay(100);
=======
	u32 crtc_enabled, tmp, frame_count, blackout;
	int i, j;

	save->vga_render_control = RREG32(VGA_RENDER_CONTROL);
	save->vga_hdp_control = RREG32(VGA_HDP_CONTROL);

	/* disable VGA render */
	WREG32(VGA_RENDER_CONTROL, 0);
=======
	u32 crtc_enabled, tmp, frame_count, blackout;
	int i, j;

	if (!ASIC_IS_NODCE(rdev)) {
		save->vga_render_control = RREG32(VGA_RENDER_CONTROL);
		save->vga_hdp_control = RREG32(VGA_HDP_CONTROL);

		/* disable VGA render */
		WREG32(VGA_RENDER_CONTROL, 0);
	}
>>>>>>> refs/remotes/origin/master
	/* blank the display controllers */
	for (i = 0; i < rdev->num_crtc; i++) {
		crtc_enabled = RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i]) & EVERGREEN_CRTC_MASTER_EN;
		if (crtc_enabled) {
			save->crtc_enabled[i] = true;
			if (ASIC_IS_DCE6(rdev)) {
				tmp = RREG32(EVERGREEN_CRTC_BLANK_CONTROL + crtc_offsets[i]);
				if (!(tmp & EVERGREEN_CRTC_BLANK_DATA_EN)) {
					radeon_wait_for_vblank(rdev, i);
					WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 1);
					tmp |= EVERGREEN_CRTC_BLANK_DATA_EN;
					WREG32(EVERGREEN_CRTC_BLANK_CONTROL + crtc_offsets[i], tmp);
				}
			} else {
				tmp = RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i]);
				if (!(tmp & EVERGREEN_CRTC_DISP_READ_REQUEST_DISABLE)) {
					radeon_wait_for_vblank(rdev, i);
					WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 1);
					tmp |= EVERGREEN_CRTC_DISP_READ_REQUEST_DISABLE;
					WREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i], tmp);
					WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 0);
				}
			}
			/* wait for the next frame */
			frame_count = radeon_get_vblank_counter(rdev, i);
			for (j = 0; j < rdev->usec_timeout; j++) {
				if (radeon_get_vblank_counter(rdev, i) != frame_count)
					break;
				udelay(1);
			}

			/* XXX this is a hack to avoid strange behavior with EFI on certain systems */
			WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 1);
			tmp = RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i]);
			tmp &= ~EVERGREEN_CRTC_MASTER_EN;
			WREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i], tmp);
			WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 0);
			save->crtc_enabled[i] = false;
			/* ***** */
		} else {
			save->crtc_enabled[i] = false;
		}
	}

	radeon_mc_wait_for_idle(rdev);

	blackout = RREG32(MC_SHARED_BLACKOUT_CNTL);
	if ((blackout & BLACKOUT_MODE_MASK) != 1) {
		/* Block CPU access */
		WREG32(BIF_FB_EN, 0);
		/* blackout the MC */
		blackout &= ~BLACKOUT_MODE_MASK;
		WREG32(MC_SHARED_BLACKOUT_CNTL, blackout | 1);
	}
	/* wait for the MC to settle */
	udelay(100);

	/* lock double buffered regs */
	for (i = 0; i < rdev->num_crtc; i++) {
		if (save->crtc_enabled[i]) {
			tmp = RREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i]);
			if (!(tmp & EVERGREEN_GRPH_UPDATE_LOCK)) {
				tmp |= EVERGREEN_GRPH_UPDATE_LOCK;
				WREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i], tmp);
			}
			tmp = RREG32(EVERGREEN_MASTER_UPDATE_LOCK + crtc_offsets[i]);
			if (!(tmp & 1)) {
				tmp |= 1;
				WREG32(EVERGREEN_MASTER_UPDATE_LOCK + crtc_offsets[i], tmp);
			}
		}
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

void evergreen_mc_resume(struct radeon_device *rdev, struct evergreen_mc_save *save)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS_HIGH + EVERGREEN_CRTC0_REGISTER_OFFSET,
	       upper_32_bits(rdev->mc.vram_start));
	WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS_HIGH + EVERGREEN_CRTC0_REGISTER_OFFSET,
	       upper_32_bits(rdev->mc.vram_start));
	WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS + EVERGREEN_CRTC0_REGISTER_OFFSET,
	       (u32)rdev->mc.vram_start);
	WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS + EVERGREEN_CRTC0_REGISTER_OFFSET,
	       (u32)rdev->mc.vram_start);

	WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS_HIGH + EVERGREEN_CRTC1_REGISTER_OFFSET,
	       upper_32_bits(rdev->mc.vram_start));
	WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS_HIGH + EVERGREEN_CRTC1_REGISTER_OFFSET,
	       upper_32_bits(rdev->mc.vram_start));
	WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS + EVERGREEN_CRTC1_REGISTER_OFFSET,
	       (u32)rdev->mc.vram_start);
	WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS + EVERGREEN_CRTC1_REGISTER_OFFSET,
	       (u32)rdev->mc.vram_start);

	if (rdev->num_crtc >= 4) {
		WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS_HIGH + EVERGREEN_CRTC2_REGISTER_OFFSET,
		       upper_32_bits(rdev->mc.vram_start));
		WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS_HIGH + EVERGREEN_CRTC2_REGISTER_OFFSET,
		       upper_32_bits(rdev->mc.vram_start));
		WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS + EVERGREEN_CRTC2_REGISTER_OFFSET,
		       (u32)rdev->mc.vram_start);
		WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS + EVERGREEN_CRTC2_REGISTER_OFFSET,
		       (u32)rdev->mc.vram_start);
=======
	u32 tmp, frame_count;
	int i, j;
>>>>>>> refs/remotes/origin/cm-11.0

	/* update crtc base addresses */
	for (i = 0; i < rdev->num_crtc; i++) {
		WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS_HIGH + crtc_offsets[i],
		       upper_32_bits(rdev->mc.vram_start));
		WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS_HIGH + crtc_offsets[i],
		       upper_32_bits(rdev->mc.vram_start));
		WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS + crtc_offsets[i],
		       (u32)rdev->mc.vram_start);
		WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS + crtc_offsets[i],
		       (u32)rdev->mc.vram_start);
	}
	WREG32(EVERGREEN_VGA_MEMORY_BASE_ADDRESS_HIGH, upper_32_bits(rdev->mc.vram_start));
	WREG32(EVERGREEN_VGA_MEMORY_BASE_ADDRESS, (u32)rdev->mc.vram_start);

	/* unlock regs and wait for update */
	for (i = 0; i < rdev->num_crtc; i++) {
		if (save->crtc_enabled[i]) {
			tmp = RREG32(EVERGREEN_MASTER_UPDATE_MODE + crtc_offsets[i]);
			if ((tmp & 0x3) != 0) {
				tmp &= ~0x3;
				WREG32(EVERGREEN_MASTER_UPDATE_MODE + crtc_offsets[i], tmp);
			}
			tmp = RREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i]);
			if (tmp & EVERGREEN_GRPH_UPDATE_LOCK) {
				tmp &= ~EVERGREEN_GRPH_UPDATE_LOCK;
				WREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i], tmp);
			}
			tmp = RREG32(EVERGREEN_MASTER_UPDATE_LOCK + crtc_offsets[i]);
			if (tmp & 1) {
				tmp &= ~1;
				WREG32(EVERGREEN_MASTER_UPDATE_LOCK + crtc_offsets[i], tmp);
			}
			for (j = 0; j < rdev->usec_timeout; j++) {
				tmp = RREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i]);
				if ((tmp & EVERGREEN_GRPH_SURFACE_UPDATE_PENDING) == 0)
					break;
				udelay(1);
			}
		}
	}

<<<<<<< HEAD
	WREG32(EVERGREEN_VGA_MEMORY_BASE_ADDRESS_HIGH, upper_32_bits(rdev->mc.vram_start));
	WREG32(EVERGREEN_VGA_MEMORY_BASE_ADDRESS, (u32)rdev->mc.vram_start);
	/* Unlock host access */
=======
=======
>>>>>>> refs/remotes/origin/master
	u32 tmp, frame_count;
	int i, j;

	/* update crtc base addresses */
	for (i = 0; i < rdev->num_crtc; i++) {
		WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS_HIGH + crtc_offsets[i],
		       upper_32_bits(rdev->mc.vram_start));
		WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS_HIGH + crtc_offsets[i],
		       upper_32_bits(rdev->mc.vram_start));
		WREG32(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS + crtc_offsets[i],
		       (u32)rdev->mc.vram_start);
		WREG32(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS + crtc_offsets[i],
		       (u32)rdev->mc.vram_start);
	}
<<<<<<< HEAD
	WREG32(EVERGREEN_VGA_MEMORY_BASE_ADDRESS_HIGH, upper_32_bits(rdev->mc.vram_start));
	WREG32(EVERGREEN_VGA_MEMORY_BASE_ADDRESS, (u32)rdev->mc.vram_start);
=======

	if (!ASIC_IS_NODCE(rdev)) {
		WREG32(EVERGREEN_VGA_MEMORY_BASE_ADDRESS_HIGH, upper_32_bits(rdev->mc.vram_start));
		WREG32(EVERGREEN_VGA_MEMORY_BASE_ADDRESS, (u32)rdev->mc.vram_start);
	}
>>>>>>> refs/remotes/origin/master

	/* unlock regs and wait for update */
	for (i = 0; i < rdev->num_crtc; i++) {
		if (save->crtc_enabled[i]) {
			tmp = RREG32(EVERGREEN_MASTER_UPDATE_MODE + crtc_offsets[i]);
			if ((tmp & 0x3) != 0) {
				tmp &= ~0x3;
				WREG32(EVERGREEN_MASTER_UPDATE_MODE + crtc_offsets[i], tmp);
			}
			tmp = RREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i]);
			if (tmp & EVERGREEN_GRPH_UPDATE_LOCK) {
				tmp &= ~EVERGREEN_GRPH_UPDATE_LOCK;
				WREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i], tmp);
			}
			tmp = RREG32(EVERGREEN_MASTER_UPDATE_LOCK + crtc_offsets[i]);
			if (tmp & 1) {
				tmp &= ~1;
				WREG32(EVERGREEN_MASTER_UPDATE_LOCK + crtc_offsets[i], tmp);
			}
			for (j = 0; j < rdev->usec_timeout; j++) {
				tmp = RREG32(EVERGREEN_GRPH_UPDATE + crtc_offsets[i]);
				if ((tmp & EVERGREEN_GRPH_SURFACE_UPDATE_PENDING) == 0)
					break;
				udelay(1);
			}
		}
	}

=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* unblackout the MC */
	tmp = RREG32(MC_SHARED_BLACKOUT_CNTL);
	tmp &= ~BLACKOUT_MODE_MASK;
	WREG32(MC_SHARED_BLACKOUT_CNTL, tmp);
	/* allow CPU access */
	WREG32(BIF_FB_EN, FB_READ_EN | FB_WRITE_EN);

	for (i = 0; i < rdev->num_crtc; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (save->crtc_enabled) {
=======
		if (save->crtc_enabled[i]) {
>>>>>>> refs/remotes/origin/master
=======
		if (save->crtc_enabled) {
>>>>>>> refs/remotes/origin/cm-11.0
			if (ASIC_IS_DCE6(rdev)) {
				tmp = RREG32(EVERGREEN_CRTC_BLANK_CONTROL + crtc_offsets[i]);
				tmp |= EVERGREEN_CRTC_BLANK_DATA_EN;
				WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 1);
				WREG32(EVERGREEN_CRTC_BLANK_CONTROL + crtc_offsets[i], tmp);
				WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 0);
			} else {
				tmp = RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i]);
				tmp &= ~EVERGREEN_CRTC_DISP_READ_REQUEST_DISABLE;
				WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 1);
				WREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i], tmp);
				WREG32(EVERGREEN_CRTC_UPDATE_LOCK + crtc_offsets[i], 0);
			}
			/* wait for the next frame */
			frame_count = radeon_get_vblank_counter(rdev, i);
			for (j = 0; j < rdev->usec_timeout; j++) {
				if (radeon_get_vblank_counter(rdev, i) != frame_count)
					break;
				udelay(1);
			}
		}
	}
<<<<<<< HEAD
<<<<<<< HEAD
	/* Unlock vga access */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Unlock vga access */
>>>>>>> refs/remotes/origin/cm-11.0
	WREG32(VGA_HDP_CONTROL, save->vga_hdp_control);
	mdelay(1);
	WREG32(VGA_RENDER_CONTROL, save->vga_render_control);
=======
	if (!ASIC_IS_NODCE(rdev)) {
		/* Unlock vga access */
		WREG32(VGA_HDP_CONTROL, save->vga_hdp_control);
		mdelay(1);
		WREG32(VGA_RENDER_CONTROL, save->vga_render_control);
	}
>>>>>>> refs/remotes/origin/master
}

void evergreen_mc_program(struct radeon_device *rdev)
{
	struct evergreen_mc_save save;
	u32 tmp;
	int i, j;

	/* Initialize HDP */
	for (i = 0, j = 0; i < 32; i++, j += 0x18) {
		WREG32((0x2c14 + j), 0x00000000);
		WREG32((0x2c18 + j), 0x00000000);
		WREG32((0x2c1c + j), 0x00000000);
		WREG32((0x2c20 + j), 0x00000000);
		WREG32((0x2c24 + j), 0x00000000);
	}
	WREG32(HDP_REG_COHERENCY_FLUSH_CNTL, 0);

	evergreen_mc_stop(rdev, &save);
	if (evergreen_mc_wait_for_idle(rdev)) {
		dev_warn(rdev->dev, "Wait for MC idle timedout !\n");
	}
	/* Lockout access through VGA aperture*/
	WREG32(VGA_HDP_CONTROL, VGA_MEMORY_DISABLE);
	/* Update configuration */
	if (rdev->flags & RADEON_IS_AGP) {
		if (rdev->mc.vram_start < rdev->mc.gtt_start) {
			/* VRAM before AGP */
			WREG32(MC_VM_SYSTEM_APERTURE_LOW_ADDR,
				rdev->mc.vram_start >> 12);
			WREG32(MC_VM_SYSTEM_APERTURE_HIGH_ADDR,
				rdev->mc.gtt_end >> 12);
		} else {
			/* VRAM after AGP */
			WREG32(MC_VM_SYSTEM_APERTURE_LOW_ADDR,
				rdev->mc.gtt_start >> 12);
			WREG32(MC_VM_SYSTEM_APERTURE_HIGH_ADDR,
				rdev->mc.vram_end >> 12);
		}
	} else {
		WREG32(MC_VM_SYSTEM_APERTURE_LOW_ADDR,
			rdev->mc.vram_start >> 12);
		WREG32(MC_VM_SYSTEM_APERTURE_HIGH_ADDR,
			rdev->mc.vram_end >> 12);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	WREG32(MC_VM_SYSTEM_APERTURE_DEFAULT_ADDR, 0);
	if (rdev->flags & RADEON_IS_IGP) {
=======
=======
>>>>>>> refs/remotes/origin/master
	WREG32(MC_VM_SYSTEM_APERTURE_DEFAULT_ADDR, rdev->vram_scratch.gpu_addr >> 12);
	/* llano/ontario only */
	if ((rdev->family == CHIP_PALM) ||
	    (rdev->family == CHIP_SUMO) ||
	    (rdev->family == CHIP_SUMO2)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		tmp = RREG32(MC_FUS_VM_FB_OFFSET) & 0x000FFFFF;
		tmp |= ((rdev->mc.vram_end >> 20) & 0xF) << 24;
		tmp |= ((rdev->mc.vram_start >> 20) & 0xF) << 20;
		WREG32(MC_FUS_VM_FB_OFFSET, tmp);
	}
	tmp = ((rdev->mc.vram_end >> 24) & 0xFFFF) << 16;
	tmp |= ((rdev->mc.vram_start >> 24) & 0xFFFF);
	WREG32(MC_VM_FB_LOCATION, tmp);
	WREG32(HDP_NONSURFACE_BASE, (rdev->mc.vram_start >> 8));
	WREG32(HDP_NONSURFACE_INFO, (2 << 7) | (1 << 30));
	WREG32(HDP_NONSURFACE_SIZE, 0x3FFFFFFF);
	if (rdev->flags & RADEON_IS_AGP) {
		WREG32(MC_VM_AGP_TOP, rdev->mc.gtt_end >> 16);
		WREG32(MC_VM_AGP_BOT, rdev->mc.gtt_start >> 16);
		WREG32(MC_VM_AGP_BASE, rdev->mc.agp_base >> 22);
	} else {
		WREG32(MC_VM_AGP_BASE, 0);
		WREG32(MC_VM_AGP_TOP, 0x0FFFFFFF);
		WREG32(MC_VM_AGP_BOT, 0x0FFFFFFF);
	}
	if (evergreen_mc_wait_for_idle(rdev)) {
		dev_warn(rdev->dev, "Wait for MC idle timedout !\n");
	}
	evergreen_mc_resume(rdev, &save);
	/* we need to own VRAM, so turn off the VGA renderer here
	 * to stop it overwriting our objects */
	rv515_vga_render_disable(rdev);
}

/*
 * CP.
 */
void evergreen_ring_ib_execute(struct radeon_device *rdev, struct radeon_ib *ib)
{
<<<<<<< HEAD
<<<<<<< HEAD
	/* set to DX10/11 mode */
	radeon_ring_write(rdev, PACKET3(PACKET3_MODE_CONTROL, 0));
	radeon_ring_write(rdev, 1);
	/* FIXME: implement */
	radeon_ring_write(rdev, PACKET3(PACKET3_INDIRECT_BUFFER, 2));
	radeon_ring_write(rdev,
=======
	struct radeon_ring *ring = &rdev->ring[ib->fence->ring];
=======
	struct radeon_ring *ring = &rdev->ring[ib->ring];
	u32 next_rptr;
>>>>>>> refs/remotes/origin/master

	/* set to DX10/11 mode */
	radeon_ring_write(ring, PACKET3(PACKET3_MODE_CONTROL, 0));
	radeon_ring_write(ring, 1);
<<<<<<< HEAD
	/* FIXME: implement */
	radeon_ring_write(ring, PACKET3(PACKET3_INDIRECT_BUFFER, 2));
	radeon_ring_write(ring,
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (ring->rptr_save_reg) {
		next_rptr = ring->wptr + 3 + 4;
		radeon_ring_write(ring, PACKET3(PACKET3_SET_CONFIG_REG, 1));
		radeon_ring_write(ring, ((ring->rptr_save_reg - 
					  PACKET3_SET_CONFIG_REG_START) >> 2));
		radeon_ring_write(ring, next_rptr);
	} else if (rdev->wb.enabled) {
		next_rptr = ring->wptr + 5 + 4;
		radeon_ring_write(ring, PACKET3(PACKET3_MEM_WRITE, 3));
		radeon_ring_write(ring, ring->next_rptr_gpu_addr & 0xfffffffc);
		radeon_ring_write(ring, (upper_32_bits(ring->next_rptr_gpu_addr) & 0xff) | (1 << 18));
		radeon_ring_write(ring, next_rptr);
		radeon_ring_write(ring, 0);
	}

	radeon_ring_write(ring, PACKET3(PACKET3_INDIRECT_BUFFER, 2));
	radeon_ring_write(ring,
>>>>>>> refs/remotes/origin/master
#ifdef __BIG_ENDIAN
			  (2 << 0) |
#endif
			  (ib->gpu_addr & 0xFFFFFFFC));
<<<<<<< HEAD
<<<<<<< HEAD
	radeon_ring_write(rdev, upper_32_bits(ib->gpu_addr) & 0xFF);
	radeon_ring_write(rdev, ib->length_dw);
=======
	radeon_ring_write(ring, upper_32_bits(ib->gpu_addr) & 0xFF);
	radeon_ring_write(ring, ib->length_dw);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	radeon_ring_write(ring, upper_32_bits(ib->gpu_addr) & 0xFF);
	radeon_ring_write(ring, ib->length_dw);
>>>>>>> refs/remotes/origin/master
}


static int evergreen_cp_load_microcode(struct radeon_device *rdev)
{
	const __be32 *fw_data;
	int i;

	if (!rdev->me_fw || !rdev->pfp_fw)
		return -EINVAL;

	r700_cp_stop(rdev);
	WREG32(CP_RB_CNTL,
#ifdef __BIG_ENDIAN
	       BUF_SWAP_32BIT |
#endif
	       RB_NO_UPDATE | RB_BLKSZ(15) | RB_BUFSZ(3));

	fw_data = (const __be32 *)rdev->pfp_fw->data;
	WREG32(CP_PFP_UCODE_ADDR, 0);
	for (i = 0; i < EVERGREEN_PFP_UCODE_SIZE; i++)
		WREG32(CP_PFP_UCODE_DATA, be32_to_cpup(fw_data++));
	WREG32(CP_PFP_UCODE_ADDR, 0);

	fw_data = (const __be32 *)rdev->me_fw->data;
	WREG32(CP_ME_RAM_WADDR, 0);
	for (i = 0; i < EVERGREEN_PM4_UCODE_SIZE; i++)
		WREG32(CP_ME_RAM_DATA, be32_to_cpup(fw_data++));

	WREG32(CP_PFP_UCODE_ADDR, 0);
	WREG32(CP_ME_RAM_WADDR, 0);
	WREG32(CP_ME_RAM_RADDR, 0);
	return 0;
}

static int evergreen_cp_start(struct radeon_device *rdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int r, i;
	uint32_t cp_me;

	r = radeon_ring_lock(rdev, 7);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct radeon_ring *ring = &rdev->ring[RADEON_RING_TYPE_GFX_INDEX];
	int r, i;
	uint32_t cp_me;

	r = radeon_ring_lock(rdev, ring, 7);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (r) {
		DRM_ERROR("radeon: cp failed to lock ring (%d).\n", r);
		return r;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	radeon_ring_write(rdev, PACKET3(PACKET3_ME_INITIALIZE, 5));
	radeon_ring_write(rdev, 0x1);
	radeon_ring_write(rdev, 0x0);
	radeon_ring_write(rdev, rdev->config.evergreen.max_hw_contexts - 1);
	radeon_ring_write(rdev, PACKET3_ME_INITIALIZE_DEVICE_ID(1));
	radeon_ring_write(rdev, 0);
	radeon_ring_write(rdev, 0);
	radeon_ring_unlock_commit(rdev);
=======
=======
>>>>>>> refs/remotes/origin/master
	radeon_ring_write(ring, PACKET3(PACKET3_ME_INITIALIZE, 5));
	radeon_ring_write(ring, 0x1);
	radeon_ring_write(ring, 0x0);
	radeon_ring_write(ring, rdev->config.evergreen.max_hw_contexts - 1);
	radeon_ring_write(ring, PACKET3_ME_INITIALIZE_DEVICE_ID(1));
	radeon_ring_write(ring, 0);
	radeon_ring_write(ring, 0);
	radeon_ring_unlock_commit(rdev, ring);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	cp_me = 0xff;
	WREG32(CP_ME_CNTL, cp_me);

<<<<<<< HEAD
<<<<<<< HEAD
	r = radeon_ring_lock(rdev, evergreen_default_size + 19);
=======
	r = radeon_ring_lock(rdev, ring, evergreen_default_size + 19);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	r = radeon_ring_lock(rdev, ring, evergreen_default_size + 19);
>>>>>>> refs/remotes/origin/master
	if (r) {
		DRM_ERROR("radeon: cp failed to lock ring (%d).\n", r);
		return r;
	}

	/* setup clear context state */
<<<<<<< HEAD
<<<<<<< HEAD
	radeon_ring_write(rdev, PACKET3(PACKET3_PREAMBLE_CNTL, 0));
	radeon_ring_write(rdev, PACKET3_PREAMBLE_BEGIN_CLEAR_STATE);

	for (i = 0; i < evergreen_default_size; i++)
		radeon_ring_write(rdev, evergreen_default_state[i]);

	radeon_ring_write(rdev, PACKET3(PACKET3_PREAMBLE_CNTL, 0));
	radeon_ring_write(rdev, PACKET3_PREAMBLE_END_CLEAR_STATE);

	/* set clear context state */
	radeon_ring_write(rdev, PACKET3(PACKET3_CLEAR_STATE, 0));
	radeon_ring_write(rdev, 0);

	/* SQ_VTX_BASE_VTX_LOC */
	radeon_ring_write(rdev, 0xc0026f00);
	radeon_ring_write(rdev, 0x00000000);
	radeon_ring_write(rdev, 0x00000000);
	radeon_ring_write(rdev, 0x00000000);

	/* Clear consts */
	radeon_ring_write(rdev, 0xc0036f00);
	radeon_ring_write(rdev, 0x00000bc4);
	radeon_ring_write(rdev, 0xffffffff);
	radeon_ring_write(rdev, 0xffffffff);
	radeon_ring_write(rdev, 0xffffffff);

	radeon_ring_write(rdev, 0xc0026900);
	radeon_ring_write(rdev, 0x00000316);
	radeon_ring_write(rdev, 0x0000000e); /* VGT_VERTEX_REUSE_BLOCK_CNTL */
	radeon_ring_write(rdev, 0x00000010); /*  */

	radeon_ring_unlock_commit(rdev);
=======
=======
>>>>>>> refs/remotes/origin/master
	radeon_ring_write(ring, PACKET3(PACKET3_PREAMBLE_CNTL, 0));
	radeon_ring_write(ring, PACKET3_PREAMBLE_BEGIN_CLEAR_STATE);

	for (i = 0; i < evergreen_default_size; i++)
		radeon_ring_write(ring, evergreen_default_state[i]);

	radeon_ring_write(ring, PACKET3(PACKET3_PREAMBLE_CNTL, 0));
	radeon_ring_write(ring, PACKET3_PREAMBLE_END_CLEAR_STATE);

	/* set clear context state */
	radeon_ring_write(ring, PACKET3(PACKET3_CLEAR_STATE, 0));
	radeon_ring_write(ring, 0);

	/* SQ_VTX_BASE_VTX_LOC */
	radeon_ring_write(ring, 0xc0026f00);
	radeon_ring_write(ring, 0x00000000);
	radeon_ring_write(ring, 0x00000000);
	radeon_ring_write(ring, 0x00000000);

	/* Clear consts */
	radeon_ring_write(ring, 0xc0036f00);
	radeon_ring_write(ring, 0x00000bc4);
	radeon_ring_write(ring, 0xffffffff);
	radeon_ring_write(ring, 0xffffffff);
	radeon_ring_write(ring, 0xffffffff);

	radeon_ring_write(ring, 0xc0026900);
	radeon_ring_write(ring, 0x00000316);
	radeon_ring_write(ring, 0x0000000e); /* VGT_VERTEX_REUSE_BLOCK_CNTL */
	radeon_ring_write(ring, 0x00000010); /*  */

	radeon_ring_unlock_commit(rdev, ring);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
int evergreen_cp_resume(struct radeon_device *rdev)
{
<<<<<<< HEAD
=======
	struct radeon_ring *ring = &rdev->ring[RADEON_RING_TYPE_GFX_INDEX];
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int evergreen_cp_resume(struct radeon_device *rdev)
{
	struct radeon_ring *ring = &rdev->ring[RADEON_RING_TYPE_GFX_INDEX];
>>>>>>> refs/remotes/origin/master
	u32 tmp;
	u32 rb_bufsz;
	int r;

	/* Reset cp; if cp is reset, then PA, SH, VGT also need to be reset */
	WREG32(GRBM_SOFT_RESET, (SOFT_RESET_CP |
				 SOFT_RESET_PA |
				 SOFT_RESET_SH |
				 SOFT_RESET_VGT |
				 SOFT_RESET_SPI |
				 SOFT_RESET_SX));
	RREG32(GRBM_SOFT_RESET);
	mdelay(15);
	WREG32(GRBM_SOFT_RESET, 0);
	RREG32(GRBM_SOFT_RESET);

	/* Set ring buffer size */
<<<<<<< HEAD
<<<<<<< HEAD
	rb_bufsz = drm_order(rdev->cp.ring_size / 8);
=======
	rb_bufsz = drm_order(ring->ring_size / 8);
>>>>>>> refs/remotes/origin/cm-10.0
	tmp = (drm_order(RADEON_GPU_PAGE_SIZE/8) << 8) | rb_bufsz;
=======
	rb_bufsz = order_base_2(ring->ring_size / 8);
	tmp = (order_base_2(RADEON_GPU_PAGE_SIZE/8) << 8) | rb_bufsz;
>>>>>>> refs/remotes/origin/master
#ifdef __BIG_ENDIAN
	tmp |= BUF_SWAP_32BIT;
#endif
	WREG32(CP_RB_CNTL, tmp);
<<<<<<< HEAD
<<<<<<< HEAD
	WREG32(CP_SEM_WAIT_TIMER, 0x4);
=======
	WREG32(CP_SEM_WAIT_TIMER, 0x0);
	WREG32(CP_SEM_INCOMPLETE_TIMER_CNTL, 0x0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	WREG32(CP_SEM_WAIT_TIMER, 0x0);
	WREG32(CP_SEM_INCOMPLETE_TIMER_CNTL, 0x0);
>>>>>>> refs/remotes/origin/master

	/* Set the write pointer delay */
	WREG32(CP_RB_WPTR_DELAY, 0);

	/* Initialize the ring buffer's read and write pointers */
	WREG32(CP_RB_CNTL, tmp | RB_RPTR_WR_ENA);
	WREG32(CP_RB_RPTR_WR, 0);
<<<<<<< HEAD
<<<<<<< HEAD
	rdev->cp.wptr = 0;
	WREG32(CP_RB_WPTR, rdev->cp.wptr);

	/* set the wb address wether it's enabled or not */
	WREG32(CP_RB_RPTR_ADDR,
#ifdef __BIG_ENDIAN
	       RB_RPTR_SWAP(2) |
#endif
=======
	ring->wptr = 0;
	WREG32(CP_RB_WPTR, ring->wptr);

	/* set the wb address wether it's enabled or not */
	WREG32(CP_RB_RPTR_ADDR,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ring->wptr = 0;
	WREG32(CP_RB_WPTR, ring->wptr);

	/* set the wb address whether it's enabled or not */
	WREG32(CP_RB_RPTR_ADDR,
>>>>>>> refs/remotes/origin/master
	       ((rdev->wb.gpu_addr + RADEON_WB_CP_RPTR_OFFSET) & 0xFFFFFFFC));
	WREG32(CP_RB_RPTR_ADDR_HI, upper_32_bits(rdev->wb.gpu_addr + RADEON_WB_CP_RPTR_OFFSET) & 0xFF);
	WREG32(SCRATCH_ADDR, ((rdev->wb.gpu_addr + RADEON_WB_SCRATCH_OFFSET) >> 8) & 0xFFFFFFFF);

	if (rdev->wb.enabled)
		WREG32(SCRATCH_UMSK, 0xff);
	else {
		tmp |= RB_NO_UPDATE;
		WREG32(SCRATCH_UMSK, 0);
	}

	mdelay(1);
	WREG32(CP_RB_CNTL, tmp);

<<<<<<< HEAD
<<<<<<< HEAD
	WREG32(CP_RB_BASE, rdev->cp.gpu_addr >> 8);
	WREG32(CP_DEBUG, (1 << 27) | (1 << 28));

	rdev->cp.rptr = RREG32(CP_RB_RPTR);

	evergreen_cp_start(rdev);
	rdev->cp.ready = true;
	r = radeon_ring_test(rdev);
	if (r) {
		rdev->cp.ready = false;
=======
=======
>>>>>>> refs/remotes/origin/master
	WREG32(CP_RB_BASE, ring->gpu_addr >> 8);
	WREG32(CP_DEBUG, (1 << 27) | (1 << 28));

	ring->rptr = RREG32(CP_RB_RPTR);

	evergreen_cp_start(rdev);
	ring->ready = true;
	r = radeon_ring_test(rdev, RADEON_RING_TYPE_GFX_INDEX, ring);
	if (r) {
		ring->ready = false;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return r;
	}
	return 0;
}

/*
 * Core functions
 */
<<<<<<< HEAD
static u32 evergreen_get_tile_pipe_to_backend_map(struct radeon_device *rdev,
						  u32 num_tile_pipes,
						  u32 num_backends,
						  u32 backend_disable_mask)
{
	u32 backend_map = 0;
	u32 enabled_backends_mask = 0;
	u32 enabled_backends_count = 0;
	u32 cur_pipe;
	u32 swizzle_pipe[EVERGREEN_MAX_PIPES];
	u32 cur_backend = 0;
	u32 i;
	bool force_no_swizzle;

	if (num_tile_pipes > EVERGREEN_MAX_PIPES)
		num_tile_pipes = EVERGREEN_MAX_PIPES;
	if (num_tile_pipes < 1)
		num_tile_pipes = 1;
	if (num_backends > EVERGREEN_MAX_BACKENDS)
		num_backends = EVERGREEN_MAX_BACKENDS;
	if (num_backends < 1)
		num_backends = 1;

	for (i = 0; i < EVERGREEN_MAX_BACKENDS; ++i) {
		if (((backend_disable_mask >> i) & 1) == 0) {
			enabled_backends_mask |= (1 << i);
			++enabled_backends_count;
		}
		if (enabled_backends_count == num_backends)
			break;
	}

	if (enabled_backends_count == 0) {
		enabled_backends_mask = 1;
		enabled_backends_count = 1;
	}

	if (enabled_backends_count != num_backends)
		num_backends = enabled_backends_count;

	memset((uint8_t *)&swizzle_pipe[0], 0, sizeof(u32) * EVERGREEN_MAX_PIPES);
	switch (rdev->family) {
	case CHIP_CEDAR:
	case CHIP_REDWOOD:
	case CHIP_PALM:
	case CHIP_SUMO:
	case CHIP_SUMO2:
	case CHIP_TURKS:
	case CHIP_CAICOS:
		force_no_swizzle = false;
		break;
	case CHIP_CYPRESS:
	case CHIP_HEMLOCK:
	case CHIP_JUNIPER:
	case CHIP_BARTS:
	default:
		force_no_swizzle = true;
		break;
	}
	if (force_no_swizzle) {
		bool last_backend_enabled = false;

		force_no_swizzle = false;
		for (i = 0; i < EVERGREEN_MAX_BACKENDS; ++i) {
			if (((enabled_backends_mask >> i) & 1) == 1) {
				if (last_backend_enabled)
					force_no_swizzle = true;
				last_backend_enabled = true;
			} else
				last_backend_enabled = false;
		}
	}

	switch (num_tile_pipes) {
	case 1:
	case 3:
	case 5:
	case 7:
		DRM_ERROR("odd number of pipes!\n");
		break;
	case 2:
		swizzle_pipe[0] = 0;
		swizzle_pipe[1] = 1;
		break;
	case 4:
		if (force_no_swizzle) {
			swizzle_pipe[0] = 0;
			swizzle_pipe[1] = 1;
			swizzle_pipe[2] = 2;
			swizzle_pipe[3] = 3;
		} else {
			swizzle_pipe[0] = 0;
			swizzle_pipe[1] = 2;
			swizzle_pipe[2] = 1;
			swizzle_pipe[3] = 3;
		}
		break;
	case 6:
		if (force_no_swizzle) {
			swizzle_pipe[0] = 0;
			swizzle_pipe[1] = 1;
			swizzle_pipe[2] = 2;
			swizzle_pipe[3] = 3;
			swizzle_pipe[4] = 4;
			swizzle_pipe[5] = 5;
		} else {
			swizzle_pipe[0] = 0;
			swizzle_pipe[1] = 2;
			swizzle_pipe[2] = 4;
			swizzle_pipe[3] = 1;
			swizzle_pipe[4] = 3;
			swizzle_pipe[5] = 5;
		}
		break;
	case 8:
		if (force_no_swizzle) {
			swizzle_pipe[0] = 0;
			swizzle_pipe[1] = 1;
			swizzle_pipe[2] = 2;
			swizzle_pipe[3] = 3;
			swizzle_pipe[4] = 4;
			swizzle_pipe[5] = 5;
			swizzle_pipe[6] = 6;
			swizzle_pipe[7] = 7;
		} else {
			swizzle_pipe[0] = 0;
			swizzle_pipe[1] = 2;
			swizzle_pipe[2] = 4;
			swizzle_pipe[3] = 6;
			swizzle_pipe[4] = 1;
			swizzle_pipe[5] = 3;
			swizzle_pipe[6] = 5;
			swizzle_pipe[7] = 7;
		}
		break;
	}

	for (cur_pipe = 0; cur_pipe < num_tile_pipes; ++cur_pipe) {
		while (((1 << cur_backend) & enabled_backends_mask) == 0)
			cur_backend = (cur_backend + 1) % EVERGREEN_MAX_BACKENDS;

		backend_map |= (((cur_backend & 0xf) << (swizzle_pipe[cur_pipe] * 4)));

		cur_backend = (cur_backend + 1) % EVERGREEN_MAX_BACKENDS;
	}

	return backend_map;
}

static void evergreen_gpu_init(struct radeon_device *rdev)
{
	u32 cc_rb_backend_disable = 0;
	u32 cc_gc_shader_pipe_config;
	u32 gb_addr_config = 0;
	u32 mc_shared_chmap, mc_arb_ramcfg;
	u32 gb_backend_map;
	u32 grbm_gfx_index;
=======
static void evergreen_gpu_init(struct radeon_device *rdev)
{
	u32 gb_addr_config;
	u32 mc_shared_chmap, mc_arb_ramcfg;
>>>>>>> refs/remotes/origin/master
	u32 sx_debug_1;
	u32 smx_dc_ctl0;
	u32 sq_config;
	u32 sq_lds_resource_mgmt;
	u32 sq_gpr_resource_mgmt_1;
	u32 sq_gpr_resource_mgmt_2;
	u32 sq_gpr_resource_mgmt_3;
	u32 sq_thread_resource_mgmt;
	u32 sq_thread_resource_mgmt_2;
	u32 sq_stack_resource_mgmt_1;
	u32 sq_stack_resource_mgmt_2;
	u32 sq_stack_resource_mgmt_3;
	u32 vgt_cache_invalidation;
	u32 hdp_host_path_cntl, tmp;
<<<<<<< HEAD
=======
	u32 disabled_rb_mask;
>>>>>>> refs/remotes/origin/master
	int i, j, num_shader_engines, ps_thread_count;

	switch (rdev->family) {
	case CHIP_CYPRESS:
	case CHIP_HEMLOCK:
		rdev->config.evergreen.num_ses = 2;
		rdev->config.evergreen.max_pipes = 4;
		rdev->config.evergreen.max_tile_pipes = 8;
		rdev->config.evergreen.max_simds = 10;
		rdev->config.evergreen.max_backends = 4 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 248;
		rdev->config.evergreen.max_gs_threads = 32;
		rdev->config.evergreen.max_stack_entries = 512;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 256;
		rdev->config.evergreen.sx_max_export_pos_size = 64;
		rdev->config.evergreen.sx_max_export_smx_size = 192;
		rdev->config.evergreen.max_hw_contexts = 8;
		rdev->config.evergreen.sq_num_cf_insts = 2;

		rdev->config.evergreen.sc_prim_fifo_size = 0x100;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
=======
		gb_addr_config = CYPRESS_GB_ADDR_CONFIG_GOLDEN;
>>>>>>> refs/remotes/origin/master
		break;
	case CHIP_JUNIPER:
		rdev->config.evergreen.num_ses = 1;
		rdev->config.evergreen.max_pipes = 4;
		rdev->config.evergreen.max_tile_pipes = 4;
		rdev->config.evergreen.max_simds = 10;
		rdev->config.evergreen.max_backends = 4 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 248;
		rdev->config.evergreen.max_gs_threads = 32;
		rdev->config.evergreen.max_stack_entries = 512;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 256;
		rdev->config.evergreen.sx_max_export_pos_size = 64;
		rdev->config.evergreen.sx_max_export_smx_size = 192;
		rdev->config.evergreen.max_hw_contexts = 8;
		rdev->config.evergreen.sq_num_cf_insts = 2;

		rdev->config.evergreen.sc_prim_fifo_size = 0x100;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
=======
		gb_addr_config = JUNIPER_GB_ADDR_CONFIG_GOLDEN;
>>>>>>> refs/remotes/origin/master
		break;
	case CHIP_REDWOOD:
		rdev->config.evergreen.num_ses = 1;
		rdev->config.evergreen.max_pipes = 4;
		rdev->config.evergreen.max_tile_pipes = 4;
		rdev->config.evergreen.max_simds = 5;
		rdev->config.evergreen.max_backends = 2 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 248;
		rdev->config.evergreen.max_gs_threads = 32;
		rdev->config.evergreen.max_stack_entries = 256;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 256;
		rdev->config.evergreen.sx_max_export_pos_size = 64;
		rdev->config.evergreen.sx_max_export_smx_size = 192;
		rdev->config.evergreen.max_hw_contexts = 8;
		rdev->config.evergreen.sq_num_cf_insts = 2;

		rdev->config.evergreen.sc_prim_fifo_size = 0x100;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
=======
		gb_addr_config = REDWOOD_GB_ADDR_CONFIG_GOLDEN;
>>>>>>> refs/remotes/origin/master
		break;
	case CHIP_CEDAR:
	default:
		rdev->config.evergreen.num_ses = 1;
		rdev->config.evergreen.max_pipes = 2;
		rdev->config.evergreen.max_tile_pipes = 2;
		rdev->config.evergreen.max_simds = 2;
		rdev->config.evergreen.max_backends = 1 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 192;
		rdev->config.evergreen.max_gs_threads = 16;
		rdev->config.evergreen.max_stack_entries = 256;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 128;
		rdev->config.evergreen.sx_max_export_pos_size = 32;
		rdev->config.evergreen.sx_max_export_smx_size = 96;
		rdev->config.evergreen.max_hw_contexts = 4;
		rdev->config.evergreen.sq_num_cf_insts = 1;

		rdev->config.evergreen.sc_prim_fifo_size = 0x40;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
=======
		gb_addr_config = CEDAR_GB_ADDR_CONFIG_GOLDEN;
>>>>>>> refs/remotes/origin/master
		break;
	case CHIP_PALM:
		rdev->config.evergreen.num_ses = 1;
		rdev->config.evergreen.max_pipes = 2;
		rdev->config.evergreen.max_tile_pipes = 2;
		rdev->config.evergreen.max_simds = 2;
		rdev->config.evergreen.max_backends = 1 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 192;
		rdev->config.evergreen.max_gs_threads = 16;
		rdev->config.evergreen.max_stack_entries = 256;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 128;
		rdev->config.evergreen.sx_max_export_pos_size = 32;
		rdev->config.evergreen.sx_max_export_smx_size = 96;
		rdev->config.evergreen.max_hw_contexts = 4;
		rdev->config.evergreen.sq_num_cf_insts = 1;

		rdev->config.evergreen.sc_prim_fifo_size = 0x40;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
=======
		gb_addr_config = CEDAR_GB_ADDR_CONFIG_GOLDEN;
>>>>>>> refs/remotes/origin/master
		break;
	case CHIP_SUMO:
		rdev->config.evergreen.num_ses = 1;
		rdev->config.evergreen.max_pipes = 4;
<<<<<<< HEAD
		rdev->config.evergreen.max_tile_pipes = 2;
=======
		rdev->config.evergreen.max_tile_pipes = 4;
>>>>>>> refs/remotes/origin/master
		if (rdev->pdev->device == 0x9648)
			rdev->config.evergreen.max_simds = 3;
		else if ((rdev->pdev->device == 0x9647) ||
			 (rdev->pdev->device == 0x964a))
			rdev->config.evergreen.max_simds = 4;
		else
			rdev->config.evergreen.max_simds = 5;
		rdev->config.evergreen.max_backends = 2 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 248;
		rdev->config.evergreen.max_gs_threads = 32;
		rdev->config.evergreen.max_stack_entries = 256;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 256;
		rdev->config.evergreen.sx_max_export_pos_size = 64;
		rdev->config.evergreen.sx_max_export_smx_size = 192;
		rdev->config.evergreen.max_hw_contexts = 8;
		rdev->config.evergreen.sq_num_cf_insts = 2;

		rdev->config.evergreen.sc_prim_fifo_size = 0x40;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
=======
		gb_addr_config = SUMO_GB_ADDR_CONFIG_GOLDEN;
>>>>>>> refs/remotes/origin/master
		break;
	case CHIP_SUMO2:
		rdev->config.evergreen.num_ses = 1;
		rdev->config.evergreen.max_pipes = 4;
		rdev->config.evergreen.max_tile_pipes = 4;
		rdev->config.evergreen.max_simds = 2;
		rdev->config.evergreen.max_backends = 1 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 248;
		rdev->config.evergreen.max_gs_threads = 32;
		rdev->config.evergreen.max_stack_entries = 512;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 256;
		rdev->config.evergreen.sx_max_export_pos_size = 64;
		rdev->config.evergreen.sx_max_export_smx_size = 192;
		rdev->config.evergreen.max_hw_contexts = 4;
		rdev->config.evergreen.sq_num_cf_insts = 2;

		rdev->config.evergreen.sc_prim_fifo_size = 0x40;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
=======
		gb_addr_config = SUMO2_GB_ADDR_CONFIG_GOLDEN;
>>>>>>> refs/remotes/origin/master
		break;
	case CHIP_BARTS:
		rdev->config.evergreen.num_ses = 2;
		rdev->config.evergreen.max_pipes = 4;
		rdev->config.evergreen.max_tile_pipes = 8;
		rdev->config.evergreen.max_simds = 7;
		rdev->config.evergreen.max_backends = 4 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 248;
		rdev->config.evergreen.max_gs_threads = 32;
		rdev->config.evergreen.max_stack_entries = 512;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 256;
		rdev->config.evergreen.sx_max_export_pos_size = 64;
		rdev->config.evergreen.sx_max_export_smx_size = 192;
		rdev->config.evergreen.max_hw_contexts = 8;
		rdev->config.evergreen.sq_num_cf_insts = 2;

		rdev->config.evergreen.sc_prim_fifo_size = 0x100;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
=======
		gb_addr_config = BARTS_GB_ADDR_CONFIG_GOLDEN;
>>>>>>> refs/remotes/origin/master
		break;
	case CHIP_TURKS:
		rdev->config.evergreen.num_ses = 1;
		rdev->config.evergreen.max_pipes = 4;
		rdev->config.evergreen.max_tile_pipes = 4;
		rdev->config.evergreen.max_simds = 6;
		rdev->config.evergreen.max_backends = 2 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 248;
		rdev->config.evergreen.max_gs_threads = 32;
		rdev->config.evergreen.max_stack_entries = 256;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 256;
		rdev->config.evergreen.sx_max_export_pos_size = 64;
		rdev->config.evergreen.sx_max_export_smx_size = 192;
		rdev->config.evergreen.max_hw_contexts = 8;
		rdev->config.evergreen.sq_num_cf_insts = 2;

		rdev->config.evergreen.sc_prim_fifo_size = 0x100;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
		break;
	case CHIP_CAICOS:
		rdev->config.evergreen.num_ses = 1;
		rdev->config.evergreen.max_pipes = 4;
=======
		gb_addr_config = TURKS_GB_ADDR_CONFIG_GOLDEN;
		break;
	case CHIP_CAICOS:
		rdev->config.evergreen.num_ses = 1;
		rdev->config.evergreen.max_pipes = 2;
>>>>>>> refs/remotes/origin/master
		rdev->config.evergreen.max_tile_pipes = 2;
		rdev->config.evergreen.max_simds = 2;
		rdev->config.evergreen.max_backends = 1 * rdev->config.evergreen.num_ses;
		rdev->config.evergreen.max_gprs = 256;
		rdev->config.evergreen.max_threads = 192;
		rdev->config.evergreen.max_gs_threads = 16;
		rdev->config.evergreen.max_stack_entries = 256;
		rdev->config.evergreen.sx_num_of_sets = 4;
		rdev->config.evergreen.sx_max_export_size = 128;
		rdev->config.evergreen.sx_max_export_pos_size = 32;
		rdev->config.evergreen.sx_max_export_smx_size = 96;
		rdev->config.evergreen.max_hw_contexts = 4;
		rdev->config.evergreen.sq_num_cf_insts = 1;

		rdev->config.evergreen.sc_prim_fifo_size = 0x40;
		rdev->config.evergreen.sc_hiz_tile_fifo_size = 0x30;
		rdev->config.evergreen.sc_earlyz_tile_fifo_size = 0x130;
<<<<<<< HEAD
=======
		gb_addr_config = CAICOS_GB_ADDR_CONFIG_GOLDEN;
>>>>>>> refs/remotes/origin/master
		break;
	}

	/* Initialize HDP */
	for (i = 0, j = 0; i < 32; i++, j += 0x18) {
		WREG32((0x2c14 + j), 0x00000000);
		WREG32((0x2c18 + j), 0x00000000);
		WREG32((0x2c1c + j), 0x00000000);
		WREG32((0x2c20 + j), 0x00000000);
		WREG32((0x2c24 + j), 0x00000000);
	}

	WREG32(GRBM_CNTL, GRBM_READ_TIMEOUT(0xff));

	evergreen_fix_pci_max_read_req_size(rdev);

<<<<<<< HEAD
	cc_gc_shader_pipe_config = RREG32(CC_GC_SHADER_PIPE_CONFIG) & ~2;

	cc_gc_shader_pipe_config |=
		INACTIVE_QD_PIPES((EVERGREEN_MAX_PIPES_MASK << rdev->config.evergreen.max_pipes)
				  & EVERGREEN_MAX_PIPES_MASK);
	cc_gc_shader_pipe_config |=
		INACTIVE_SIMDS((EVERGREEN_MAX_SIMDS_MASK << rdev->config.evergreen.max_simds)
			       & EVERGREEN_MAX_SIMDS_MASK);

	cc_rb_backend_disable =
		BACKEND_DISABLE((EVERGREEN_MAX_BACKENDS_MASK << rdev->config.evergreen.max_backends)
				& EVERGREEN_MAX_BACKENDS_MASK);


	mc_shared_chmap = RREG32(MC_SHARED_CHMAP);
<<<<<<< HEAD
	if (rdev->flags & RADEON_IS_IGP)
=======
	if ((rdev->family == CHIP_PALM) ||
	    (rdev->family == CHIP_SUMO) ||
	    (rdev->family == CHIP_SUMO2))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mc_shared_chmap = RREG32(MC_SHARED_CHMAP);
	if ((rdev->family == CHIP_PALM) ||
	    (rdev->family == CHIP_SUMO) ||
	    (rdev->family == CHIP_SUMO2))
>>>>>>> refs/remotes/origin/master
		mc_arb_ramcfg = RREG32(FUS_MC_ARB_RAMCFG);
	else
		mc_arb_ramcfg = RREG32(MC_ARB_RAMCFG);

<<<<<<< HEAD
	switch (rdev->config.evergreen.max_tile_pipes) {
	case 1:
	default:
		gb_addr_config |= NUM_PIPES(0);
		break;
	case 2:
		gb_addr_config |= NUM_PIPES(1);
		break;
	case 4:
		gb_addr_config |= NUM_PIPES(2);
		break;
	case 8:
		gb_addr_config |= NUM_PIPES(3);
		break;
	}

	gb_addr_config |= PIPE_INTERLEAVE_SIZE((mc_arb_ramcfg & BURSTLENGTH_MASK) >> BURSTLENGTH_SHIFT);
	gb_addr_config |= BANK_INTERLEAVE_SIZE(0);
	gb_addr_config |= NUM_SHADER_ENGINES(rdev->config.evergreen.num_ses - 1);
	gb_addr_config |= SHADER_ENGINE_TILE_SIZE(1);
	gb_addr_config |= NUM_GPUS(0); /* Hemlock? */
	gb_addr_config |= MULTI_GPU_TILE_SIZE(2);

	if (((mc_arb_ramcfg & NOOFCOLS_MASK) >> NOOFCOLS_SHIFT) > 2)
		gb_addr_config |= ROW_SIZE(2);
	else
		gb_addr_config |= ROW_SIZE((mc_arb_ramcfg & NOOFCOLS_MASK) >> NOOFCOLS_SHIFT);

	if (rdev->ddev->pdev->device == 0x689e) {
		u32 efuse_straps_4;
		u32 efuse_straps_3;
		u8 efuse_box_bit_131_124;

		WREG32(RCU_IND_INDEX, 0x204);
		efuse_straps_4 = RREG32(RCU_IND_DATA);
		WREG32(RCU_IND_INDEX, 0x203);
		efuse_straps_3 = RREG32(RCU_IND_DATA);
		efuse_box_bit_131_124 = (u8)(((efuse_straps_4 & 0xf) << 4) | ((efuse_straps_3 & 0xf0000000) >> 28));

		switch(efuse_box_bit_131_124) {
		case 0x00:
			gb_backend_map = 0x76543210;
			break;
		case 0x55:
			gb_backend_map = 0x77553311;
			break;
		case 0x56:
			gb_backend_map = 0x77553300;
			break;
		case 0x59:
			gb_backend_map = 0x77552211;
			break;
		case 0x66:
			gb_backend_map = 0x77443300;
			break;
		case 0x99:
			gb_backend_map = 0x66552211;
			break;
		case 0x5a:
			gb_backend_map = 0x77552200;
			break;
		case 0xaa:
			gb_backend_map = 0x66442200;
			break;
		case 0x95:
			gb_backend_map = 0x66553311;
			break;
		default:
			DRM_ERROR("bad backend map, using default\n");
			gb_backend_map =
				evergreen_get_tile_pipe_to_backend_map(rdev,
								       rdev->config.evergreen.max_tile_pipes,
								       rdev->config.evergreen.max_backends,
								       ((EVERGREEN_MAX_BACKENDS_MASK <<
								   rdev->config.evergreen.max_backends) &
									EVERGREEN_MAX_BACKENDS_MASK));
			break;
		}
	} else if (rdev->ddev->pdev->device == 0x68b9) {
		u32 efuse_straps_3;
		u8 efuse_box_bit_127_124;

		WREG32(RCU_IND_INDEX, 0x203);
		efuse_straps_3 = RREG32(RCU_IND_DATA);
		efuse_box_bit_127_124 = (u8)((efuse_straps_3 & 0xF0000000) >> 28);

		switch(efuse_box_bit_127_124) {
		case 0x0:
			gb_backend_map = 0x00003210;
			break;
		case 0x5:
		case 0x6:
		case 0x9:
		case 0xa:
			gb_backend_map = 0x00003311;
			break;
		default:
			DRM_ERROR("bad backend map, using default\n");
			gb_backend_map =
				evergreen_get_tile_pipe_to_backend_map(rdev,
								       rdev->config.evergreen.max_tile_pipes,
								       rdev->config.evergreen.max_backends,
								       ((EVERGREEN_MAX_BACKENDS_MASK <<
								   rdev->config.evergreen.max_backends) &
									EVERGREEN_MAX_BACKENDS_MASK));
			break;
		}
	} else {
		switch (rdev->family) {
		case CHIP_CYPRESS:
		case CHIP_HEMLOCK:
		case CHIP_BARTS:
			gb_backend_map = 0x66442200;
			break;
		case CHIP_JUNIPER:
			gb_backend_map = 0x00002200;
			break;
		default:
			gb_backend_map =
				evergreen_get_tile_pipe_to_backend_map(rdev,
								       rdev->config.evergreen.max_tile_pipes,
								       rdev->config.evergreen.max_backends,
								       ((EVERGREEN_MAX_BACKENDS_MASK <<
									 rdev->config.evergreen.max_backends) &
									EVERGREEN_MAX_BACKENDS_MASK));
		}
	}

=======
>>>>>>> refs/remotes/origin/master
	/* setup tiling info dword.  gb_addr_config is not adequate since it does
	 * not have bank info, so create a custom tiling dword.
	 * bits 3:0   num_pipes
	 * bits 7:4   num_banks
	 * bits 11:8  group_size
	 * bits 15:12 row_size
	 */
	rdev->config.evergreen.tile_config = 0;
	switch (rdev->config.evergreen.max_tile_pipes) {
	case 1:
	default:
		rdev->config.evergreen.tile_config |= (0 << 0);
		break;
	case 2:
		rdev->config.evergreen.tile_config |= (1 << 0);
		break;
	case 4:
		rdev->config.evergreen.tile_config |= (2 << 0);
		break;
	case 8:
		rdev->config.evergreen.tile_config |= (3 << 0);
		break;
	}
	/* num banks is 8 on all fusion asics. 0 = 4, 1 = 8, 2 = 16 */
	if (rdev->flags & RADEON_IS_IGP)
		rdev->config.evergreen.tile_config |= 1 << 4;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	else
		rdev->config.evergreen.tile_config |=
			((mc_arb_ramcfg & NOOFBANK_MASK) >> NOOFBANK_SHIFT) << 4;
=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	else {
		switch ((mc_arb_ramcfg & NOOFBANK_MASK) >> NOOFBANK_SHIFT) {
		case 0: /* four banks */
			rdev->config.evergreen.tile_config |= 0 << 4;
			break;
		case 1: /* eight banks */
			rdev->config.evergreen.tile_config |= 1 << 4;
			break;
		case 2: /* sixteen banks */
		default:
			rdev->config.evergreen.tile_config |= 2 << 4;
			break;
		}
	}
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	rdev->config.evergreen.tile_config |=
		((mc_arb_ramcfg & BURSTLENGTH_MASK) >> BURSTLENGTH_SHIFT) << 8;
	rdev->config.evergreen.tile_config |=
		((gb_addr_config & 0x30000000) >> 28) << 12;

<<<<<<< HEAD
=======
	rdev->config.evergreen.backend_map = gb_backend_map;
>>>>>>> refs/remotes/origin/cm-10.0
	WREG32(GB_BACKEND_MAP, gb_backend_map);
	WREG32(GB_ADDR_CONFIG, gb_addr_config);
	WREG32(DMIF_ADDR_CONFIG, gb_addr_config);
	WREG32(HDP_ADDR_CONFIG, gb_addr_config);

	num_shader_engines = ((RREG32(GB_ADDR_CONFIG) & NUM_SHADER_ENGINES(3)) >> 12) + 1;
	grbm_gfx_index = INSTANCE_BROADCAST_WRITES;

	for (i = 0; i < rdev->config.evergreen.num_ses; i++) {
		u32 rb = cc_rb_backend_disable | (0xf0 << 16);
		u32 sp = cc_gc_shader_pipe_config;
		u32 gfx = grbm_gfx_index | SE_INDEX(i);

		if (i == num_shader_engines) {
			rb |= BACKEND_DISABLE(EVERGREEN_MAX_BACKENDS_MASK);
			sp |= INACTIVE_SIMDS(EVERGREEN_MAX_SIMDS_MASK);
		}

		WREG32(GRBM_GFX_INDEX, gfx);
		WREG32(RLC_GFX_INDEX, gfx);

		WREG32(CC_RB_BACKEND_DISABLE, rb);
		WREG32(CC_SYS_RB_BACKEND_DISABLE, rb);
		WREG32(GC_USER_RB_BACKEND_DISABLE, rb);
		WREG32(CC_GC_SHADER_PIPE_CONFIG, sp);
	}

	grbm_gfx_index = INSTANCE_BROADCAST_WRITES | SE_BROADCAST_WRITES;
	WREG32(GRBM_GFX_INDEX, grbm_gfx_index);
	WREG32(RLC_GFX_INDEX, grbm_gfx_index);
=======
	rdev->config.evergreen.tile_config |= 0 << 8;
	rdev->config.evergreen.tile_config |=
		((gb_addr_config & 0x30000000) >> 28) << 12;

	num_shader_engines = (gb_addr_config & NUM_SHADER_ENGINES(3) >> 12) + 1;

	if ((rdev->family >= CHIP_CEDAR) && (rdev->family <= CHIP_HEMLOCK)) {
		u32 efuse_straps_4;
		u32 efuse_straps_3;

		efuse_straps_4 = RREG32_RCU(0x204);
		efuse_straps_3 = RREG32_RCU(0x203);
		tmp = (((efuse_straps_4 & 0xf) << 4) |
		      ((efuse_straps_3 & 0xf0000000) >> 28));
	} else {
		tmp = 0;
		for (i = (rdev->config.evergreen.num_ses - 1); i >= 0; i--) {
			u32 rb_disable_bitmap;

			WREG32(GRBM_GFX_INDEX, INSTANCE_BROADCAST_WRITES | SE_INDEX(i));
			WREG32(RLC_GFX_INDEX, INSTANCE_BROADCAST_WRITES | SE_INDEX(i));
			rb_disable_bitmap = (RREG32(CC_RB_BACKEND_DISABLE) & 0x00ff0000) >> 16;
			tmp <<= 4;
			tmp |= rb_disable_bitmap;
		}
	}
	/* enabled rb are just the one not disabled :) */
	disabled_rb_mask = tmp;
	tmp = 0;
	for (i = 0; i < rdev->config.evergreen.max_backends; i++)
		tmp |= (1 << i);
	/* if all the backends are disabled, fix it up here */
	if ((disabled_rb_mask & tmp) == tmp) {
		for (i = 0; i < rdev->config.evergreen.max_backends; i++)
			disabled_rb_mask &= ~(1 << i);
	}

	WREG32(GRBM_GFX_INDEX, INSTANCE_BROADCAST_WRITES | SE_BROADCAST_WRITES);
	WREG32(RLC_GFX_INDEX, INSTANCE_BROADCAST_WRITES | SE_BROADCAST_WRITES);

	WREG32(GB_ADDR_CONFIG, gb_addr_config);
	WREG32(DMIF_ADDR_CONFIG, gb_addr_config);
	WREG32(HDP_ADDR_CONFIG, gb_addr_config);
	WREG32(DMA_TILING_CONFIG, gb_addr_config);
	WREG32(UVD_UDEC_ADDR_CONFIG, gb_addr_config);
	WREG32(UVD_UDEC_DB_ADDR_CONFIG, gb_addr_config);
	WREG32(UVD_UDEC_DBW_ADDR_CONFIG, gb_addr_config);

	if ((rdev->config.evergreen.max_backends == 1) &&
	    (rdev->flags & RADEON_IS_IGP)) {
		if ((disabled_rb_mask & 3) == 1) {
			/* RB0 disabled, RB1 enabled */
			tmp = 0x11111111;
		} else {
			/* RB1 disabled, RB0 enabled */
			tmp = 0x00000000;
		}
	} else {
		tmp = gb_addr_config & NUM_PIPES_MASK;
		tmp = r6xx_remap_render_backend(rdev, tmp, rdev->config.evergreen.max_backends,
						EVERGREEN_MAX_BACKENDS, disabled_rb_mask);
	}
	WREG32(GB_BACKEND_MAP, tmp);
>>>>>>> refs/remotes/origin/master

	WREG32(CGTS_SYS_TCC_DISABLE, 0);
	WREG32(CGTS_TCC_DISABLE, 0);
	WREG32(CGTS_USER_SYS_TCC_DISABLE, 0);
	WREG32(CGTS_USER_TCC_DISABLE, 0);

	/* set HW defaults for 3D engine */
	WREG32(CP_QUEUE_THRESHOLDS, (ROQ_IB1_START(0x16) |
				     ROQ_IB2_START(0x2b)));

	WREG32(CP_MEQ_THRESHOLDS, STQ_SPLIT(0x30));

	WREG32(TA_CNTL_AUX, (DISABLE_CUBE_ANISO |
			     SYNC_GRADIENT |
			     SYNC_WALKER |
			     SYNC_ALIGNER));

	sx_debug_1 = RREG32(SX_DEBUG_1);
	sx_debug_1 |= ENABLE_NEW_SMX_ADDRESS;
	WREG32(SX_DEBUG_1, sx_debug_1);


	smx_dc_ctl0 = RREG32(SMX_DC_CTL0);
	smx_dc_ctl0 &= ~NUMBER_OF_SETS(0x1ff);
	smx_dc_ctl0 |= NUMBER_OF_SETS(rdev->config.evergreen.sx_num_of_sets);
	WREG32(SMX_DC_CTL0, smx_dc_ctl0);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (rdev->family <= CHIP_SUMO2)
		WREG32(SMX_SAR_CTL0, 0x00010000);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (rdev->family <= CHIP_SUMO2)
		WREG32(SMX_SAR_CTL0, 0x00010000);

>>>>>>> refs/remotes/origin/master
=======
	if (rdev->family <= CHIP_SUMO2)
		WREG32(SMX_SAR_CTL0, 0x00010000);

>>>>>>> refs/remotes/origin/cm-11.0
	WREG32(SX_EXPORT_BUFFER_SIZES, (COLOR_BUFFER_SIZE((rdev->config.evergreen.sx_max_export_size / 4) - 1) |
					POSITION_BUFFER_SIZE((rdev->config.evergreen.sx_max_export_pos_size / 4) - 1) |
					SMX_BUFFER_SIZE((rdev->config.evergreen.sx_max_export_smx_size / 4) - 1)));

	WREG32(PA_SC_FIFO_SIZE, (SC_PRIM_FIFO_SIZE(rdev->config.evergreen.sc_prim_fifo_size) |
				 SC_HIZ_TILE_FIFO_SIZE(rdev->config.evergreen.sc_hiz_tile_fifo_size) |
				 SC_EARLYZ_TILE_FIFO_SIZE(rdev->config.evergreen.sc_earlyz_tile_fifo_size)));

	WREG32(VGT_NUM_INSTANCES, 1);
	WREG32(SPI_CONFIG_CNTL, 0);
	WREG32(SPI_CONFIG_CNTL_1, VTX_DONE_DELAY(4));
	WREG32(CP_PERFMON_CNTL, 0);

	WREG32(SQ_MS_FIFO_SIZES, (CACHE_FIFO_SIZE(16 * rdev->config.evergreen.sq_num_cf_insts) |
				  FETCH_FIFO_HIWATER(0x4) |
				  DONE_FIFO_HIWATER(0xe0) |
				  ALU_UPDATE_FIFO_HIWATER(0x8)));

	sq_config = RREG32(SQ_CONFIG);
	sq_config &= ~(PS_PRIO(3) |
		       VS_PRIO(3) |
		       GS_PRIO(3) |
		       ES_PRIO(3));
	sq_config |= (VC_ENABLE |
		      EXPORT_SRC_C |
		      PS_PRIO(0) |
		      VS_PRIO(1) |
		      GS_PRIO(2) |
		      ES_PRIO(3));

	switch (rdev->family) {
	case CHIP_CEDAR:
	case CHIP_PALM:
	case CHIP_SUMO:
	case CHIP_SUMO2:
	case CHIP_CAICOS:
		/* no vertex cache */
		sq_config &= ~VC_ENABLE;
		break;
	default:
		break;
	}

	sq_lds_resource_mgmt = RREG32(SQ_LDS_RESOURCE_MGMT);

	sq_gpr_resource_mgmt_1 = NUM_PS_GPRS((rdev->config.evergreen.max_gprs - (4 * 2))* 12 / 32);
	sq_gpr_resource_mgmt_1 |= NUM_VS_GPRS((rdev->config.evergreen.max_gprs - (4 * 2)) * 6 / 32);
	sq_gpr_resource_mgmt_1 |= NUM_CLAUSE_TEMP_GPRS(4);
	sq_gpr_resource_mgmt_2 = NUM_GS_GPRS((rdev->config.evergreen.max_gprs - (4 * 2)) * 4 / 32);
	sq_gpr_resource_mgmt_2 |= NUM_ES_GPRS((rdev->config.evergreen.max_gprs - (4 * 2)) * 4 / 32);
	sq_gpr_resource_mgmt_3 = NUM_HS_GPRS((rdev->config.evergreen.max_gprs - (4 * 2)) * 3 / 32);
	sq_gpr_resource_mgmt_3 |= NUM_LS_GPRS((rdev->config.evergreen.max_gprs - (4 * 2)) * 3 / 32);

	switch (rdev->family) {
	case CHIP_CEDAR:
	case CHIP_PALM:
	case CHIP_SUMO:
	case CHIP_SUMO2:
		ps_thread_count = 96;
		break;
	default:
		ps_thread_count = 128;
		break;
	}

	sq_thread_resource_mgmt = NUM_PS_THREADS(ps_thread_count);
	sq_thread_resource_mgmt |= NUM_VS_THREADS((((rdev->config.evergreen.max_threads - ps_thread_count) / 6) / 8) * 8);
	sq_thread_resource_mgmt |= NUM_GS_THREADS((((rdev->config.evergreen.max_threads - ps_thread_count) / 6) / 8) * 8);
	sq_thread_resource_mgmt |= NUM_ES_THREADS((((rdev->config.evergreen.max_threads - ps_thread_count) / 6) / 8) * 8);
	sq_thread_resource_mgmt_2 = NUM_HS_THREADS((((rdev->config.evergreen.max_threads - ps_thread_count) / 6) / 8) * 8);
	sq_thread_resource_mgmt_2 |= NUM_LS_THREADS((((rdev->config.evergreen.max_threads - ps_thread_count) / 6) / 8) * 8);

	sq_stack_resource_mgmt_1 = NUM_PS_STACK_ENTRIES((rdev->config.evergreen.max_stack_entries * 1) / 6);
	sq_stack_resource_mgmt_1 |= NUM_VS_STACK_ENTRIES((rdev->config.evergreen.max_stack_entries * 1) / 6);
	sq_stack_resource_mgmt_2 = NUM_GS_STACK_ENTRIES((rdev->config.evergreen.max_stack_entries * 1) / 6);
	sq_stack_resource_mgmt_2 |= NUM_ES_STACK_ENTRIES((rdev->config.evergreen.max_stack_entries * 1) / 6);
	sq_stack_resource_mgmt_3 = NUM_HS_STACK_ENTRIES((rdev->config.evergreen.max_stack_entries * 1) / 6);
	sq_stack_resource_mgmt_3 |= NUM_LS_STACK_ENTRIES((rdev->config.evergreen.max_stack_entries * 1) / 6);

	WREG32(SQ_CONFIG, sq_config);
	WREG32(SQ_GPR_RESOURCE_MGMT_1, sq_gpr_resource_mgmt_1);
	WREG32(SQ_GPR_RESOURCE_MGMT_2, sq_gpr_resource_mgmt_2);
	WREG32(SQ_GPR_RESOURCE_MGMT_3, sq_gpr_resource_mgmt_3);
	WREG32(SQ_THREAD_RESOURCE_MGMT, sq_thread_resource_mgmt);
	WREG32(SQ_THREAD_RESOURCE_MGMT_2, sq_thread_resource_mgmt_2);
	WREG32(SQ_STACK_RESOURCE_MGMT_1, sq_stack_resource_mgmt_1);
	WREG32(SQ_STACK_RESOURCE_MGMT_2, sq_stack_resource_mgmt_2);
	WREG32(SQ_STACK_RESOURCE_MGMT_3, sq_stack_resource_mgmt_3);
	WREG32(SQ_DYN_GPR_CNTL_PS_FLUSH_REQ, 0);
	WREG32(SQ_LDS_RESOURCE_MGMT, sq_lds_resource_mgmt);

	WREG32(PA_SC_FORCE_EOV_MAX_CNTS, (FORCE_EOV_MAX_CLK_CNT(4095) |
					  FORCE_EOV_MAX_REZ_CNT(255)));

	switch (rdev->family) {
	case CHIP_CEDAR:
	case CHIP_PALM:
	case CHIP_SUMO:
	case CHIP_SUMO2:
	case CHIP_CAICOS:
		vgt_cache_invalidation = CACHE_INVALIDATION(TC_ONLY);
		break;
	default:
		vgt_cache_invalidation = CACHE_INVALIDATION(VC_AND_TC);
		break;
	}
	vgt_cache_invalidation |= AUTO_INVLD_EN(ES_AND_GS_AUTO);
	WREG32(VGT_CACHE_INVALIDATION, vgt_cache_invalidation);

	WREG32(VGT_GS_VERTEX_REUSE, 16);
	WREG32(PA_SU_LINE_STIPPLE_VALUE, 0);
	WREG32(PA_SC_LINE_STIPPLE_STATE, 0);

	WREG32(VGT_VERTEX_REUSE_BLOCK_CNTL, 14);
	WREG32(VGT_OUT_DEALLOC_CNTL, 16);

	WREG32(CB_PERF_CTR0_SEL_0, 0);
	WREG32(CB_PERF_CTR0_SEL_1, 0);
	WREG32(CB_PERF_CTR1_SEL_0, 0);
	WREG32(CB_PERF_CTR1_SEL_1, 0);
	WREG32(CB_PERF_CTR2_SEL_0, 0);
	WREG32(CB_PERF_CTR2_SEL_1, 0);
	WREG32(CB_PERF_CTR3_SEL_0, 0);
	WREG32(CB_PERF_CTR3_SEL_1, 0);

	/* clear render buffer base addresses */
	WREG32(CB_COLOR0_BASE, 0);
	WREG32(CB_COLOR1_BASE, 0);
	WREG32(CB_COLOR2_BASE, 0);
	WREG32(CB_COLOR3_BASE, 0);
	WREG32(CB_COLOR4_BASE, 0);
	WREG32(CB_COLOR5_BASE, 0);
	WREG32(CB_COLOR6_BASE, 0);
	WREG32(CB_COLOR7_BASE, 0);
	WREG32(CB_COLOR8_BASE, 0);
	WREG32(CB_COLOR9_BASE, 0);
	WREG32(CB_COLOR10_BASE, 0);
	WREG32(CB_COLOR11_BASE, 0);

	/* set the shader const cache sizes to 0 */
	for (i = SQ_ALU_CONST_BUFFER_SIZE_PS_0; i < 0x28200; i += 4)
		WREG32(i, 0);
	for (i = SQ_ALU_CONST_BUFFER_SIZE_HS_0; i < 0x29000; i += 4)
		WREG32(i, 0);

	tmp = RREG32(HDP_MISC_CNTL);
	tmp |= HDP_FLUSH_INVALIDATE_CACHE;
	WREG32(HDP_MISC_CNTL, tmp);

	hdp_host_path_cntl = RREG32(HDP_HOST_PATH_CNTL);
	WREG32(HDP_HOST_PATH_CNTL, hdp_host_path_cntl);

	WREG32(PA_CL_ENHANCE, CLIP_VTX_REORDER_ENA | NUM_CLIP_SEQ(3));

	udelay(50);

}

int evergreen_mc_init(struct radeon_device *rdev)
{
	u32 tmp;
	int chansize, numchan;

	/* Get VRAM informations */
	rdev->mc.vram_is_ddr = true;
<<<<<<< HEAD
<<<<<<< HEAD
	if (rdev->flags & RADEON_IS_IGP)
=======
	if ((rdev->family == CHIP_PALM) ||
	    (rdev->family == CHIP_SUMO) ||
	    (rdev->family == CHIP_SUMO2))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((rdev->family == CHIP_PALM) ||
	    (rdev->family == CHIP_SUMO) ||
	    (rdev->family == CHIP_SUMO2))
>>>>>>> refs/remotes/origin/master
		tmp = RREG32(FUS_MC_ARB_RAMCFG);
	else
		tmp = RREG32(MC_ARB_RAMCFG);
	if (tmp & CHANSIZE_OVERRIDE) {
		chansize = 16;
	} else if (tmp & CHANSIZE_MASK) {
		chansize = 64;
	} else {
		chansize = 32;
	}
	tmp = RREG32(MC_SHARED_CHMAP);
	switch ((tmp & NOOFCHAN_MASK) >> NOOFCHAN_SHIFT) {
	case 0:
	default:
		numchan = 1;
		break;
	case 1:
		numchan = 2;
		break;
	case 2:
		numchan = 4;
		break;
	case 3:
		numchan = 8;
		break;
	}
	rdev->mc.vram_width = numchan * chansize;
	/* Could aper size report 0 ? */
	rdev->mc.aper_base = pci_resource_start(rdev->pdev, 0);
	rdev->mc.aper_size = pci_resource_len(rdev->pdev, 0);
	/* Setup GPU memory space */
<<<<<<< HEAD
<<<<<<< HEAD
	if (rdev->flags & RADEON_IS_IGP) {
=======
	if ((rdev->family == CHIP_PALM) ||
	    (rdev->family == CHIP_SUMO) ||
	    (rdev->family == CHIP_SUMO2)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((rdev->family == CHIP_PALM) ||
	    (rdev->family == CHIP_SUMO) ||
	    (rdev->family == CHIP_SUMO2)) {
>>>>>>> refs/remotes/origin/master
		/* size in bytes on fusion */
		rdev->mc.mc_vram_size = RREG32(CONFIG_MEMSIZE);
		rdev->mc.real_vram_size = RREG32(CONFIG_MEMSIZE);
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		/* size in MB on evergreen */
		rdev->mc.mc_vram_size = RREG32(CONFIG_MEMSIZE) * 1024 * 1024;
		rdev->mc.real_vram_size = RREG32(CONFIG_MEMSIZE) * 1024 * 1024;
=======
		/* size in MB on evergreen/cayman/tn */
		rdev->mc.mc_vram_size = RREG32(CONFIG_MEMSIZE) * 1024ULL * 1024ULL;
		rdev->mc.real_vram_size = RREG32(CONFIG_MEMSIZE) * 1024ULL * 1024ULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		/* size in MB on evergreen/cayman/tn */
		rdev->mc.mc_vram_size = RREG32(CONFIG_MEMSIZE) * 1024ULL * 1024ULL;
		rdev->mc.real_vram_size = RREG32(CONFIG_MEMSIZE) * 1024ULL * 1024ULL;
>>>>>>> refs/remotes/origin/master
=======
		/* size in MB on evergreen/cayman/tn */
		rdev->mc.mc_vram_size = RREG32(CONFIG_MEMSIZE) * 1024ULL * 1024ULL;
		rdev->mc.real_vram_size = RREG32(CONFIG_MEMSIZE) * 1024ULL * 1024ULL;
>>>>>>> refs/remotes/origin/cm-11.0
	}
	rdev->mc.visible_vram_size = rdev->mc.aper_size;
	r700_vram_gtt_location(rdev, &rdev->mc);
	radeon_update_bandwidth_info(rdev);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
bool evergreen_gpu_is_lockup(struct radeon_device *rdev)
=======
bool evergreen_gpu_is_lockup(struct radeon_device *rdev, struct radeon_ring *ring)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u32 srbm_status;
	u32 grbm_status;
	u32 grbm_status_se0, grbm_status_se1;
	struct r100_gpu_lockup *lockup = &rdev->config.evergreen.lockup;
	int r;

	srbm_status = RREG32(SRBM_STATUS);
	grbm_status = RREG32(GRBM_STATUS);
	grbm_status_se0 = RREG32(GRBM_STATUS_SE0);
	grbm_status_se1 = RREG32(GRBM_STATUS_SE1);
	if (!(grbm_status & GUI_ACTIVE)) {
<<<<<<< HEAD
		r100_gpu_lockup_update(lockup, &rdev->cp);
		return false;
	}
	/* force CP activities */
	r = radeon_ring_lock(rdev, 2);
	if (!r) {
		/* PACKET2 NOP */
		radeon_ring_write(rdev, 0x80000000);
		radeon_ring_write(rdev, 0x80000000);
		radeon_ring_unlock_commit(rdev);
	}
	rdev->cp.rptr = RREG32(CP_RB_RPTR);
	return r100_gpu_cp_is_lockup(rdev, lockup, &rdev->cp);
=======
		r100_gpu_lockup_update(lockup, ring);
		return false;
	}
	/* force CP activities */
	r = radeon_ring_lock(rdev, ring, 2);
	if (!r) {
		/* PACKET2 NOP */
		radeon_ring_write(ring, 0x80000000);
		radeon_ring_write(ring, 0x80000000);
		radeon_ring_unlock_commit(rdev, ring);
	}
	ring->rptr = RREG32(CP_RB_RPTR);
	return r100_gpu_cp_is_lockup(rdev, lockup, ring);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int evergreen_gpu_soft_reset(struct radeon_device *rdev)
{
	struct evergreen_mc_save save;
	u32 grbm_reset = 0;

	if (!(RREG32(GRBM_STATUS) & GUI_ACTIVE))
		return 0;

	dev_info(rdev->dev, "GPU softreset \n");
	dev_info(rdev->dev, "  GRBM_STATUS=0x%08X\n",
		RREG32(GRBM_STATUS));
	dev_info(rdev->dev, "  GRBM_STATUS_SE0=0x%08X\n",
		RREG32(GRBM_STATUS_SE0));
	dev_info(rdev->dev, "  GRBM_STATUS_SE1=0x%08X\n",
		RREG32(GRBM_STATUS_SE1));
	dev_info(rdev->dev, "  SRBM_STATUS=0x%08X\n",
		RREG32(SRBM_STATUS));
=======
void evergreen_print_gpu_status_regs(struct radeon_device *rdev)
{
	dev_info(rdev->dev, "  GRBM_STATUS               = 0x%08X\n",
		RREG32(GRBM_STATUS));
	dev_info(rdev->dev, "  GRBM_STATUS_SE0           = 0x%08X\n",
		RREG32(GRBM_STATUS_SE0));
	dev_info(rdev->dev, "  GRBM_STATUS_SE1           = 0x%08X\n",
		RREG32(GRBM_STATUS_SE1));
	dev_info(rdev->dev, "  SRBM_STATUS               = 0x%08X\n",
		RREG32(SRBM_STATUS));
	dev_info(rdev->dev, "  SRBM_STATUS2              = 0x%08X\n",
		RREG32(SRBM_STATUS2));
	dev_info(rdev->dev, "  R_008674_CP_STALLED_STAT1 = 0x%08X\n",
		RREG32(CP_STALLED_STAT1));
	dev_info(rdev->dev, "  R_008678_CP_STALLED_STAT2 = 0x%08X\n",
		RREG32(CP_STALLED_STAT2));
	dev_info(rdev->dev, "  R_00867C_CP_BUSY_STAT     = 0x%08X\n",
		RREG32(CP_BUSY_STAT));
	dev_info(rdev->dev, "  R_008680_CP_STAT          = 0x%08X\n",
		RREG32(CP_STAT));
	dev_info(rdev->dev, "  R_00D034_DMA_STATUS_REG   = 0x%08X\n",
		RREG32(DMA_STATUS_REG));
	if (rdev->family >= CHIP_CAYMAN) {
		dev_info(rdev->dev, "  R_00D834_DMA_STATUS_REG   = 0x%08X\n",
			 RREG32(DMA_STATUS_REG + 0x800));
	}
}

bool evergreen_is_display_hung(struct radeon_device *rdev)
{
	u32 crtc_hung = 0;
	u32 crtc_status[6];
	u32 i, j, tmp;

	for (i = 0; i < rdev->num_crtc; i++) {
		if (RREG32(EVERGREEN_CRTC_CONTROL + crtc_offsets[i]) & EVERGREEN_CRTC_MASTER_EN) {
			crtc_status[i] = RREG32(EVERGREEN_CRTC_STATUS_HV_COUNT + crtc_offsets[i]);
			crtc_hung |= (1 << i);
		}
	}

	for (j = 0; j < 10; j++) {
		for (i = 0; i < rdev->num_crtc; i++) {
			if (crtc_hung & (1 << i)) {
				tmp = RREG32(EVERGREEN_CRTC_STATUS_HV_COUNT + crtc_offsets[i]);
				if (tmp != crtc_status[i])
					crtc_hung &= ~(1 << i);
			}
		}
		if (crtc_hung == 0)
			return false;
		udelay(100);
	}

	return true;
}

u32 evergreen_gpu_check_soft_reset(struct radeon_device *rdev)
{
	u32 reset_mask = 0;
	u32 tmp;

	/* GRBM_STATUS */
	tmp = RREG32(GRBM_STATUS);
	if (tmp & (PA_BUSY | SC_BUSY |
		   SH_BUSY | SX_BUSY |
		   TA_BUSY | VGT_BUSY |
		   DB_BUSY | CB_BUSY |
		   SPI_BUSY | VGT_BUSY_NO_DMA))
		reset_mask |= RADEON_RESET_GFX;

	if (tmp & (CF_RQ_PENDING | PF_RQ_PENDING |
		   CP_BUSY | CP_COHERENCY_BUSY))
		reset_mask |= RADEON_RESET_CP;

	if (tmp & GRBM_EE_BUSY)
		reset_mask |= RADEON_RESET_GRBM | RADEON_RESET_GFX | RADEON_RESET_CP;

	/* DMA_STATUS_REG */
	tmp = RREG32(DMA_STATUS_REG);
	if (!(tmp & DMA_IDLE))
		reset_mask |= RADEON_RESET_DMA;

	/* SRBM_STATUS2 */
	tmp = RREG32(SRBM_STATUS2);
	if (tmp & DMA_BUSY)
		reset_mask |= RADEON_RESET_DMA;

	/* SRBM_STATUS */
	tmp = RREG32(SRBM_STATUS);
	if (tmp & (RLC_RQ_PENDING | RLC_BUSY))
		reset_mask |= RADEON_RESET_RLC;

	if (tmp & IH_BUSY)
		reset_mask |= RADEON_RESET_IH;

	if (tmp & SEM_BUSY)
		reset_mask |= RADEON_RESET_SEM;

	if (tmp & GRBM_RQ_PENDING)
		reset_mask |= RADEON_RESET_GRBM;

	if (tmp & VMC_BUSY)
		reset_mask |= RADEON_RESET_VMC;

	if (tmp & (MCB_BUSY | MCB_NON_DISPLAY_BUSY |
		   MCC_BUSY | MCD_BUSY))
		reset_mask |= RADEON_RESET_MC;

	if (evergreen_is_display_hung(rdev))
		reset_mask |= RADEON_RESET_DISPLAY;

	/* VM_L2_STATUS */
	tmp = RREG32(VM_L2_STATUS);
	if (tmp & L2_BUSY)
		reset_mask |= RADEON_RESET_VMC;

	/* Skip MC reset as it's mostly likely not hung, just busy */
	if (reset_mask & RADEON_RESET_MC) {
		DRM_DEBUG("MC busy: 0x%08X, clearing.\n", reset_mask);
		reset_mask &= ~RADEON_RESET_MC;
	}

	return reset_mask;
}

static void evergreen_gpu_soft_reset(struct radeon_device *rdev, u32 reset_mask)
{
	struct evergreen_mc_save save;
	u32 grbm_soft_reset = 0, srbm_soft_reset = 0;
	u32 tmp;

	if (reset_mask == 0)
		return;

	dev_info(rdev->dev, "GPU softreset: 0x%08X\n", reset_mask);

	evergreen_print_gpu_status_regs(rdev);

	/* Disable CP parsing/prefetching */
	WREG32(CP_ME_CNTL, CP_ME_HALT | CP_PFP_HALT);

	if (reset_mask & RADEON_RESET_DMA) {
		/* Disable DMA */
		tmp = RREG32(DMA_RB_CNTL);
		tmp &= ~DMA_RB_ENABLE;
		WREG32(DMA_RB_CNTL, tmp);
	}

	udelay(50);

>>>>>>> refs/remotes/origin/master
	evergreen_mc_stop(rdev, &save);
	if (evergreen_mc_wait_for_idle(rdev)) {
		dev_warn(rdev->dev, "Wait for MC idle timedout !\n");
	}
<<<<<<< HEAD
	/* Disable CP parsing/prefetching */
	WREG32(CP_ME_CNTL, CP_ME_HALT | CP_PFP_HALT);

	/* reset all the gfx blocks */
	grbm_reset = (SOFT_RESET_CP |
		      SOFT_RESET_CB |
		      SOFT_RESET_DB |
		      SOFT_RESET_PA |
		      SOFT_RESET_SC |
		      SOFT_RESET_SPI |
		      SOFT_RESET_SH |
		      SOFT_RESET_SX |
		      SOFT_RESET_TC |
		      SOFT_RESET_TA |
		      SOFT_RESET_VC |
		      SOFT_RESET_VGT);

	dev_info(rdev->dev, "  GRBM_SOFT_RESET=0x%08X\n", grbm_reset);
	WREG32(GRBM_SOFT_RESET, grbm_reset);
	(void)RREG32(GRBM_SOFT_RESET);
	udelay(50);
	WREG32(GRBM_SOFT_RESET, 0);
	(void)RREG32(GRBM_SOFT_RESET);
	/* Wait a little for things to settle down */
	udelay(50);
	dev_info(rdev->dev, "  GRBM_STATUS=0x%08X\n",
		RREG32(GRBM_STATUS));
	dev_info(rdev->dev, "  GRBM_STATUS_SE0=0x%08X\n",
		RREG32(GRBM_STATUS_SE0));
	dev_info(rdev->dev, "  GRBM_STATUS_SE1=0x%08X\n",
		RREG32(GRBM_STATUS_SE1));
	dev_info(rdev->dev, "  SRBM_STATUS=0x%08X\n",
		RREG32(SRBM_STATUS));
	evergreen_mc_resume(rdev, &save);
	return 0;
}

int evergreen_asic_reset(struct radeon_device *rdev)
{
	return evergreen_gpu_soft_reset(rdev);
=======

	if (reset_mask & (RADEON_RESET_GFX | RADEON_RESET_COMPUTE)) {
		grbm_soft_reset |= SOFT_RESET_DB |
			SOFT_RESET_CB |
			SOFT_RESET_PA |
			SOFT_RESET_SC |
			SOFT_RESET_SPI |
			SOFT_RESET_SX |
			SOFT_RESET_SH |
			SOFT_RESET_TC |
			SOFT_RESET_TA |
			SOFT_RESET_VC |
			SOFT_RESET_VGT;
	}

	if (reset_mask & RADEON_RESET_CP) {
		grbm_soft_reset |= SOFT_RESET_CP |
			SOFT_RESET_VGT;

		srbm_soft_reset |= SOFT_RESET_GRBM;
	}

	if (reset_mask & RADEON_RESET_DMA)
		srbm_soft_reset |= SOFT_RESET_DMA;

	if (reset_mask & RADEON_RESET_DISPLAY)
		srbm_soft_reset |= SOFT_RESET_DC;

	if (reset_mask & RADEON_RESET_RLC)
		srbm_soft_reset |= SOFT_RESET_RLC;

	if (reset_mask & RADEON_RESET_SEM)
		srbm_soft_reset |= SOFT_RESET_SEM;

	if (reset_mask & RADEON_RESET_IH)
		srbm_soft_reset |= SOFT_RESET_IH;

	if (reset_mask & RADEON_RESET_GRBM)
		srbm_soft_reset |= SOFT_RESET_GRBM;

	if (reset_mask & RADEON_RESET_VMC)
		srbm_soft_reset |= SOFT_RESET_VMC;

	if (!(rdev->flags & RADEON_IS_IGP)) {
		if (reset_mask & RADEON_RESET_MC)
			srbm_soft_reset |= SOFT_RESET_MC;
	}

	if (grbm_soft_reset) {
		tmp = RREG32(GRBM_SOFT_RESET);
		tmp |= grbm_soft_reset;
		dev_info(rdev->dev, "GRBM_SOFT_RESET=0x%08X\n", tmp);
		WREG32(GRBM_SOFT_RESET, tmp);
		tmp = RREG32(GRBM_SOFT_RESET);

		udelay(50);

		tmp &= ~grbm_soft_reset;
		WREG32(GRBM_SOFT_RESET, tmp);
		tmp = RREG32(GRBM_SOFT_RESET);
	}

	if (srbm_soft_reset) {
		tmp = RREG32(SRBM_SOFT_RESET);
		tmp |= srbm_soft_reset;
		dev_info(rdev->dev, "SRBM_SOFT_RESET=0x%08X\n", tmp);
		WREG32(SRBM_SOFT_RESET, tmp);
		tmp = RREG32(SRBM_SOFT_RESET);

		udelay(50);

		tmp &= ~srbm_soft_reset;
		WREG32(SRBM_SOFT_RESET, tmp);
		tmp = RREG32(SRBM_SOFT_RESET);
	}

	/* Wait a little for things to settle down */
	udelay(50);

	evergreen_mc_resume(rdev, &save);
	udelay(50);

	evergreen_print_gpu_status_regs(rdev);
}

int evergreen_asic_reset(struct radeon_device *rdev)
{
	u32 reset_mask;

	reset_mask = evergreen_gpu_check_soft_reset(rdev);

	if (reset_mask)
		r600_set_bios_scratch_engine_hung(rdev, true);

	evergreen_gpu_soft_reset(rdev, reset_mask);

	reset_mask = evergreen_gpu_check_soft_reset(rdev);

	if (!reset_mask)
		r600_set_bios_scratch_engine_hung(rdev, false);

	return 0;
}

/**
 * evergreen_gfx_is_lockup - Check if the GFX engine is locked up
 *
 * @rdev: radeon_device pointer
 * @ring: radeon_ring structure holding ring information
 *
 * Check if the GFX engine is locked up.
 * Returns true if the engine appears to be locked up, false if not.
 */
bool evergreen_gfx_is_lockup(struct radeon_device *rdev, struct radeon_ring *ring)
{
	u32 reset_mask = evergreen_gpu_check_soft_reset(rdev);

	if (!(reset_mask & (RADEON_RESET_GFX |
			    RADEON_RESET_COMPUTE |
			    RADEON_RESET_CP))) {
		radeon_ring_lockup_update(ring);
		return false;
	}
	/* force CP activities */
	radeon_ring_force_activity(rdev, ring);
	return radeon_ring_test_lockup(rdev, ring);
}

/*
 * RLC
 */
#define RLC_SAVE_RESTORE_LIST_END_MARKER    0x00000000
#define RLC_CLEAR_STATE_END_MARKER          0x00000001

void sumo_rlc_fini(struct radeon_device *rdev)
{
	int r;

	/* save restore block */
	if (rdev->rlc.save_restore_obj) {
		r = radeon_bo_reserve(rdev->rlc.save_restore_obj, false);
		if (unlikely(r != 0))
			dev_warn(rdev->dev, "(%d) reserve RLC sr bo failed\n", r);
		radeon_bo_unpin(rdev->rlc.save_restore_obj);
		radeon_bo_unreserve(rdev->rlc.save_restore_obj);

		radeon_bo_unref(&rdev->rlc.save_restore_obj);
		rdev->rlc.save_restore_obj = NULL;
	}

	/* clear state block */
	if (rdev->rlc.clear_state_obj) {
		r = radeon_bo_reserve(rdev->rlc.clear_state_obj, false);
		if (unlikely(r != 0))
			dev_warn(rdev->dev, "(%d) reserve RLC c bo failed\n", r);
		radeon_bo_unpin(rdev->rlc.clear_state_obj);
		radeon_bo_unreserve(rdev->rlc.clear_state_obj);

		radeon_bo_unref(&rdev->rlc.clear_state_obj);
		rdev->rlc.clear_state_obj = NULL;
	}

	/* clear state block */
	if (rdev->rlc.cp_table_obj) {
		r = radeon_bo_reserve(rdev->rlc.cp_table_obj, false);
		if (unlikely(r != 0))
			dev_warn(rdev->dev, "(%d) reserve RLC cp table bo failed\n", r);
		radeon_bo_unpin(rdev->rlc.cp_table_obj);
		radeon_bo_unreserve(rdev->rlc.cp_table_obj);

		radeon_bo_unref(&rdev->rlc.cp_table_obj);
		rdev->rlc.cp_table_obj = NULL;
	}
}

#define CP_ME_TABLE_SIZE    96

int sumo_rlc_init(struct radeon_device *rdev)
{
	const u32 *src_ptr;
	volatile u32 *dst_ptr;
	u32 dws, data, i, j, k, reg_num;
	u32 reg_list_num, reg_list_hdr_blk_index, reg_list_blk_index = 0;
	u64 reg_list_mc_addr;
	const struct cs_section_def *cs_data;
	int r;

	src_ptr = rdev->rlc.reg_list;
	dws = rdev->rlc.reg_list_size;
	if (rdev->family >= CHIP_BONAIRE) {
		dws += (5 * 16) + 48 + 48 + 64;
	}
	cs_data = rdev->rlc.cs_data;

	if (src_ptr) {
		/* save restore block */
		if (rdev->rlc.save_restore_obj == NULL) {
			r = radeon_bo_create(rdev, dws * 4, PAGE_SIZE, true,
					     RADEON_GEM_DOMAIN_VRAM, NULL, &rdev->rlc.save_restore_obj);
			if (r) {
				dev_warn(rdev->dev, "(%d) create RLC sr bo failed\n", r);
				return r;
			}
		}

		r = radeon_bo_reserve(rdev->rlc.save_restore_obj, false);
		if (unlikely(r != 0)) {
			sumo_rlc_fini(rdev);
			return r;
		}
		r = radeon_bo_pin(rdev->rlc.save_restore_obj, RADEON_GEM_DOMAIN_VRAM,
				  &rdev->rlc.save_restore_gpu_addr);
		if (r) {
			radeon_bo_unreserve(rdev->rlc.save_restore_obj);
			dev_warn(rdev->dev, "(%d) pin RLC sr bo failed\n", r);
			sumo_rlc_fini(rdev);
			return r;
		}

		r = radeon_bo_kmap(rdev->rlc.save_restore_obj, (void **)&rdev->rlc.sr_ptr);
		if (r) {
			dev_warn(rdev->dev, "(%d) map RLC sr bo failed\n", r);
			sumo_rlc_fini(rdev);
			return r;
		}
		/* write the sr buffer */
		dst_ptr = rdev->rlc.sr_ptr;
		if (rdev->family >= CHIP_TAHITI) {
			/* SI */
			for (i = 0; i < rdev->rlc.reg_list_size; i++)
				dst_ptr[i] = cpu_to_le32(src_ptr[i]);
		} else {
			/* ON/LN/TN */
			/* format:
			 * dw0: (reg2 << 16) | reg1
			 * dw1: reg1 save space
			 * dw2: reg2 save space
			 */
			for (i = 0; i < dws; i++) {
				data = src_ptr[i] >> 2;
				i++;
				if (i < dws)
					data |= (src_ptr[i] >> 2) << 16;
				j = (((i - 1) * 3) / 2);
				dst_ptr[j] = cpu_to_le32(data);
			}
			j = ((i * 3) / 2);
			dst_ptr[j] = cpu_to_le32(RLC_SAVE_RESTORE_LIST_END_MARKER);
		}
		radeon_bo_kunmap(rdev->rlc.save_restore_obj);
		radeon_bo_unreserve(rdev->rlc.save_restore_obj);
	}

	if (cs_data) {
		/* clear state block */
		if (rdev->family >= CHIP_BONAIRE) {
			rdev->rlc.clear_state_size = dws = cik_get_csb_size(rdev);
		} else if (rdev->family >= CHIP_TAHITI) {
			rdev->rlc.clear_state_size = si_get_csb_size(rdev);
			dws = rdev->rlc.clear_state_size + (256 / 4);
		} else {
			reg_list_num = 0;
			dws = 0;
			for (i = 0; cs_data[i].section != NULL; i++) {
				for (j = 0; cs_data[i].section[j].extent != NULL; j++) {
					reg_list_num++;
					dws += cs_data[i].section[j].reg_count;
				}
			}
			reg_list_blk_index = (3 * reg_list_num + 2);
			dws += reg_list_blk_index;
			rdev->rlc.clear_state_size = dws;
		}

		if (rdev->rlc.clear_state_obj == NULL) {
			r = radeon_bo_create(rdev, dws * 4, PAGE_SIZE, true,
					     RADEON_GEM_DOMAIN_VRAM, NULL, &rdev->rlc.clear_state_obj);
			if (r) {
				dev_warn(rdev->dev, "(%d) create RLC c bo failed\n", r);
				sumo_rlc_fini(rdev);
				return r;
			}
		}
		r = radeon_bo_reserve(rdev->rlc.clear_state_obj, false);
		if (unlikely(r != 0)) {
			sumo_rlc_fini(rdev);
			return r;
		}
		r = radeon_bo_pin(rdev->rlc.clear_state_obj, RADEON_GEM_DOMAIN_VRAM,
				  &rdev->rlc.clear_state_gpu_addr);
		if (r) {
			radeon_bo_unreserve(rdev->rlc.clear_state_obj);
			dev_warn(rdev->dev, "(%d) pin RLC c bo failed\n", r);
			sumo_rlc_fini(rdev);
			return r;
		}

		r = radeon_bo_kmap(rdev->rlc.clear_state_obj, (void **)&rdev->rlc.cs_ptr);
		if (r) {
			dev_warn(rdev->dev, "(%d) map RLC c bo failed\n", r);
			sumo_rlc_fini(rdev);
			return r;
		}
		/* set up the cs buffer */
		dst_ptr = rdev->rlc.cs_ptr;
		if (rdev->family >= CHIP_BONAIRE) {
			cik_get_csb_buffer(rdev, dst_ptr);
		} else if (rdev->family >= CHIP_TAHITI) {
			reg_list_mc_addr = rdev->rlc.clear_state_gpu_addr + 256;
			dst_ptr[0] = cpu_to_le32(upper_32_bits(reg_list_mc_addr));
			dst_ptr[1] = cpu_to_le32(lower_32_bits(reg_list_mc_addr));
			dst_ptr[2] = cpu_to_le32(rdev->rlc.clear_state_size);
			si_get_csb_buffer(rdev, &dst_ptr[(256/4)]);
		} else {
			reg_list_hdr_blk_index = 0;
			reg_list_mc_addr = rdev->rlc.clear_state_gpu_addr + (reg_list_blk_index * 4);
			data = upper_32_bits(reg_list_mc_addr);
			dst_ptr[reg_list_hdr_blk_index] = cpu_to_le32(data);
			reg_list_hdr_blk_index++;
			for (i = 0; cs_data[i].section != NULL; i++) {
				for (j = 0; cs_data[i].section[j].extent != NULL; j++) {
					reg_num = cs_data[i].section[j].reg_count;
					data = reg_list_mc_addr & 0xffffffff;
					dst_ptr[reg_list_hdr_blk_index] = cpu_to_le32(data);
					reg_list_hdr_blk_index++;

					data = (cs_data[i].section[j].reg_index * 4) & 0xffffffff;
					dst_ptr[reg_list_hdr_blk_index] = cpu_to_le32(data);
					reg_list_hdr_blk_index++;

					data = 0x08000000 | (reg_num * 4);
					dst_ptr[reg_list_hdr_blk_index] = cpu_to_le32(data);
					reg_list_hdr_blk_index++;

					for (k = 0; k < reg_num; k++) {
						data = cs_data[i].section[j].extent[k];
						dst_ptr[reg_list_blk_index + k] = cpu_to_le32(data);
					}
					reg_list_mc_addr += reg_num * 4;
					reg_list_blk_index += reg_num;
				}
			}
			dst_ptr[reg_list_hdr_blk_index] = cpu_to_le32(RLC_CLEAR_STATE_END_MARKER);
		}
		radeon_bo_kunmap(rdev->rlc.clear_state_obj);
		radeon_bo_unreserve(rdev->rlc.clear_state_obj);
	}

	if (rdev->rlc.cp_table_size) {
		if (rdev->rlc.cp_table_obj == NULL) {
			r = radeon_bo_create(rdev, rdev->rlc.cp_table_size, PAGE_SIZE, true,
					     RADEON_GEM_DOMAIN_VRAM, NULL, &rdev->rlc.cp_table_obj);
			if (r) {
				dev_warn(rdev->dev, "(%d) create RLC cp table bo failed\n", r);
				sumo_rlc_fini(rdev);
				return r;
			}
		}

		r = radeon_bo_reserve(rdev->rlc.cp_table_obj, false);
		if (unlikely(r != 0)) {
			dev_warn(rdev->dev, "(%d) reserve RLC cp table bo failed\n", r);
			sumo_rlc_fini(rdev);
			return r;
		}
		r = radeon_bo_pin(rdev->rlc.cp_table_obj, RADEON_GEM_DOMAIN_VRAM,
				  &rdev->rlc.cp_table_gpu_addr);
		if (r) {
			radeon_bo_unreserve(rdev->rlc.cp_table_obj);
			dev_warn(rdev->dev, "(%d) pin RLC cp_table bo failed\n", r);
			sumo_rlc_fini(rdev);
			return r;
		}
		r = radeon_bo_kmap(rdev->rlc.cp_table_obj, (void **)&rdev->rlc.cp_table_ptr);
		if (r) {
			dev_warn(rdev->dev, "(%d) map RLC cp table bo failed\n", r);
			sumo_rlc_fini(rdev);
			return r;
		}

		cik_init_cp_pg_table(rdev);

		radeon_bo_kunmap(rdev->rlc.cp_table_obj);
		radeon_bo_unreserve(rdev->rlc.cp_table_obj);

	}

	return 0;
}

static void evergreen_rlc_start(struct radeon_device *rdev)
{
	u32 mask = RLC_ENABLE;

	if (rdev->flags & RADEON_IS_IGP) {
		mask |= GFX_POWER_GATING_ENABLE | GFX_POWER_GATING_SRC;
	}

	WREG32(RLC_CNTL, mask);
}

int evergreen_rlc_resume(struct radeon_device *rdev)
{
	u32 i;
	const __be32 *fw_data;

	if (!rdev->rlc_fw)
		return -EINVAL;

	r600_rlc_stop(rdev);

	WREG32(RLC_HB_CNTL, 0);

	if (rdev->flags & RADEON_IS_IGP) {
		if (rdev->family == CHIP_ARUBA) {
			u32 always_on_bitmap =
				3 | (3 << (16 * rdev->config.cayman.max_shader_engines));
			/* find out the number of active simds */
			u32 tmp = (RREG32(CC_GC_SHADER_PIPE_CONFIG) & 0xffff0000) >> 16;
			tmp |= 0xffffffff << rdev->config.cayman.max_simds_per_se;
			tmp = hweight32(~tmp);
			if (tmp == rdev->config.cayman.max_simds_per_se) {
				WREG32(TN_RLC_LB_ALWAYS_ACTIVE_SIMD_MASK, always_on_bitmap);
				WREG32(TN_RLC_LB_PARAMS, 0x00601004);
				WREG32(TN_RLC_LB_INIT_SIMD_MASK, 0xffffffff);
				WREG32(TN_RLC_LB_CNTR_INIT, 0x00000000);
				WREG32(TN_RLC_LB_CNTR_MAX, 0x00002000);
			}
		} else {
			WREG32(RLC_HB_WPTR_LSB_ADDR, 0);
			WREG32(RLC_HB_WPTR_MSB_ADDR, 0);
		}
		WREG32(TN_RLC_SAVE_AND_RESTORE_BASE, rdev->rlc.save_restore_gpu_addr >> 8);
		WREG32(TN_RLC_CLEAR_STATE_RESTORE_BASE, rdev->rlc.clear_state_gpu_addr >> 8);
	} else {
		WREG32(RLC_HB_BASE, 0);
		WREG32(RLC_HB_RPTR, 0);
		WREG32(RLC_HB_WPTR, 0);
		WREG32(RLC_HB_WPTR_LSB_ADDR, 0);
		WREG32(RLC_HB_WPTR_MSB_ADDR, 0);
	}
	WREG32(RLC_MC_CNTL, 0);
	WREG32(RLC_UCODE_CNTL, 0);

	fw_data = (const __be32 *)rdev->rlc_fw->data;
	if (rdev->family >= CHIP_ARUBA) {
		for (i = 0; i < ARUBA_RLC_UCODE_SIZE; i++) {
			WREG32(RLC_UCODE_ADDR, i);
			WREG32(RLC_UCODE_DATA, be32_to_cpup(fw_data++));
		}
	} else if (rdev->family >= CHIP_CAYMAN) {
		for (i = 0; i < CAYMAN_RLC_UCODE_SIZE; i++) {
			WREG32(RLC_UCODE_ADDR, i);
			WREG32(RLC_UCODE_DATA, be32_to_cpup(fw_data++));
		}
	} else {
		for (i = 0; i < EVERGREEN_RLC_UCODE_SIZE; i++) {
			WREG32(RLC_UCODE_ADDR, i);
			WREG32(RLC_UCODE_DATA, be32_to_cpup(fw_data++));
		}
	}
	WREG32(RLC_UCODE_ADDR, 0);

	evergreen_rlc_start(rdev);

	return 0;
>>>>>>> refs/remotes/origin/master
}

/* Interrupts */

u32 evergreen_get_vblank_counter(struct radeon_device *rdev, int crtc)
{
<<<<<<< HEAD
	switch (crtc) {
	case 0:
		return RREG32(CRTC_STATUS_FRAME_COUNT + EVERGREEN_CRTC0_REGISTER_OFFSET);
	case 1:
		return RREG32(CRTC_STATUS_FRAME_COUNT + EVERGREEN_CRTC1_REGISTER_OFFSET);
	case 2:
		return RREG32(CRTC_STATUS_FRAME_COUNT + EVERGREEN_CRTC2_REGISTER_OFFSET);
	case 3:
		return RREG32(CRTC_STATUS_FRAME_COUNT + EVERGREEN_CRTC3_REGISTER_OFFSET);
	case 4:
		return RREG32(CRTC_STATUS_FRAME_COUNT + EVERGREEN_CRTC4_REGISTER_OFFSET);
	case 5:
		return RREG32(CRTC_STATUS_FRAME_COUNT + EVERGREEN_CRTC5_REGISTER_OFFSET);
	default:
		return 0;
	}
=======
	if (crtc >= rdev->num_crtc)
		return 0;
	else
		return RREG32(CRTC_STATUS_FRAME_COUNT + crtc_offsets[crtc]);
>>>>>>> refs/remotes/origin/master
}

void evergreen_disable_interrupt_state(struct radeon_device *rdev)
{
	u32 tmp;

<<<<<<< HEAD
<<<<<<< HEAD
	WREG32(CP_INT_CNTL, CNTX_BUSY_INT_ENABLE | CNTX_EMPTY_INT_ENABLE);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (rdev->family >= CHIP_CAYMAN) {
		cayman_cp_int_cntl_setup(rdev, 0,
					 CNTX_BUSY_INT_ENABLE | CNTX_EMPTY_INT_ENABLE);
		cayman_cp_int_cntl_setup(rdev, 1, 0);
		cayman_cp_int_cntl_setup(rdev, 2, 0);
<<<<<<< HEAD
	} else
		WREG32(CP_INT_CNTL, CNTX_BUSY_INT_ENABLE | CNTX_EMPTY_INT_ENABLE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		tmp = RREG32(CAYMAN_DMA1_CNTL) & ~TRAP_ENABLE;
		WREG32(CAYMAN_DMA1_CNTL, tmp);
	} else
		WREG32(CP_INT_CNTL, CNTX_BUSY_INT_ENABLE | CNTX_EMPTY_INT_ENABLE);
	tmp = RREG32(DMA_CNTL) & ~TRAP_ENABLE;
	WREG32(DMA_CNTL, tmp);
>>>>>>> refs/remotes/origin/master
	WREG32(GRBM_INT_CNTL, 0);
	WREG32(INT_MASK + EVERGREEN_CRTC0_REGISTER_OFFSET, 0);
	WREG32(INT_MASK + EVERGREEN_CRTC1_REGISTER_OFFSET, 0);
	if (rdev->num_crtc >= 4) {
		WREG32(INT_MASK + EVERGREEN_CRTC2_REGISTER_OFFSET, 0);
		WREG32(INT_MASK + EVERGREEN_CRTC3_REGISTER_OFFSET, 0);
	}
	if (rdev->num_crtc >= 6) {
		WREG32(INT_MASK + EVERGREEN_CRTC4_REGISTER_OFFSET, 0);
		WREG32(INT_MASK + EVERGREEN_CRTC5_REGISTER_OFFSET, 0);
	}

	WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC0_REGISTER_OFFSET, 0);
	WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC1_REGISTER_OFFSET, 0);
	if (rdev->num_crtc >= 4) {
		WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC2_REGISTER_OFFSET, 0);
		WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC3_REGISTER_OFFSET, 0);
	}
	if (rdev->num_crtc >= 6) {
		WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC4_REGISTER_OFFSET, 0);
		WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC5_REGISTER_OFFSET, 0);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	WREG32(DACA_AUTODETECT_INT_CONTROL, 0);
=======
	/* only one DAC on DCE6 */
	if (!ASIC_IS_DCE6(rdev))
		WREG32(DACA_AUTODETECT_INT_CONTROL, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* only one DAC on DCE6 */
	if (!ASIC_IS_DCE6(rdev))
		WREG32(DACA_AUTODETECT_INT_CONTROL, 0);
>>>>>>> refs/remotes/origin/master
	WREG32(DACB_AUTODETECT_INT_CONTROL, 0);

	tmp = RREG32(DC_HPD1_INT_CONTROL) & DC_HPDx_INT_POLARITY;
	WREG32(DC_HPD1_INT_CONTROL, tmp);
	tmp = RREG32(DC_HPD2_INT_CONTROL) & DC_HPDx_INT_POLARITY;
	WREG32(DC_HPD2_INT_CONTROL, tmp);
	tmp = RREG32(DC_HPD3_INT_CONTROL) & DC_HPDx_INT_POLARITY;
	WREG32(DC_HPD3_INT_CONTROL, tmp);
	tmp = RREG32(DC_HPD4_INT_CONTROL) & DC_HPDx_INT_POLARITY;
	WREG32(DC_HPD4_INT_CONTROL, tmp);
	tmp = RREG32(DC_HPD5_INT_CONTROL) & DC_HPDx_INT_POLARITY;
	WREG32(DC_HPD5_INT_CONTROL, tmp);
	tmp = RREG32(DC_HPD6_INT_CONTROL) & DC_HPDx_INT_POLARITY;
	WREG32(DC_HPD6_INT_CONTROL, tmp);

}

int evergreen_irq_set(struct radeon_device *rdev)
{
	u32 cp_int_cntl = CNTX_BUSY_INT_ENABLE | CNTX_EMPTY_INT_ENABLE;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	u32 cp_int_cntl1 = 0, cp_int_cntl2 = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 cp_int_cntl1 = 0, cp_int_cntl2 = 0;
>>>>>>> refs/remotes/origin/master
	u32 crtc1 = 0, crtc2 = 0, crtc3 = 0, crtc4 = 0, crtc5 = 0, crtc6 = 0;
	u32 hpd1, hpd2, hpd3, hpd4, hpd5, hpd6;
	u32 grbm_int_cntl = 0;
	u32 grph1 = 0, grph2 = 0, grph3 = 0, grph4 = 0, grph5 = 0, grph6 = 0;
<<<<<<< HEAD
=======
	u32 afmt1 = 0, afmt2 = 0, afmt3 = 0, afmt4 = 0, afmt5 = 0, afmt6 = 0;
	u32 dma_cntl, dma_cntl1 = 0;
	u32 thermal_int = 0;
>>>>>>> refs/remotes/origin/master

	if (!rdev->irq.installed) {
		WARN(1, "Can't enable IRQ/MSI because no handler is installed\n");
		return -EINVAL;
	}
	/* don't enable anything if the ih is disabled */
	if (!rdev->ih.enabled) {
		r600_disable_interrupts(rdev);
		/* force the active interrupt state to all disabled */
		evergreen_disable_interrupt_state(rdev);
		return 0;
	}

	hpd1 = RREG32(DC_HPD1_INT_CONTROL) & ~DC_HPDx_INT_EN;
	hpd2 = RREG32(DC_HPD2_INT_CONTROL) & ~DC_HPDx_INT_EN;
	hpd3 = RREG32(DC_HPD3_INT_CONTROL) & ~DC_HPDx_INT_EN;
	hpd4 = RREG32(DC_HPD4_INT_CONTROL) & ~DC_HPDx_INT_EN;
	hpd5 = RREG32(DC_HPD5_INT_CONTROL) & ~DC_HPDx_INT_EN;
	hpd6 = RREG32(DC_HPD6_INT_CONTROL) & ~DC_HPDx_INT_EN;
<<<<<<< HEAD

<<<<<<< HEAD
	if (rdev->irq.sw_int) {
		DRM_DEBUG("evergreen_irq_set: sw int\n");
		cp_int_cntl |= RB_INT_ENABLE;
		cp_int_cntl |= TIME_STAMP_INT_ENABLE;
	}
=======
	if (rdev->family >= CHIP_CAYMAN) {
		/* enable CP interrupts on all rings */
		if (rdev->irq.sw_int[RADEON_RING_TYPE_GFX_INDEX]) {
			DRM_DEBUG("evergreen_irq_set: sw int gfx\n");
			cp_int_cntl |= TIME_STAMP_INT_ENABLE;
		}
		if (rdev->irq.sw_int[CAYMAN_RING_TYPE_CP1_INDEX]) {
			DRM_DEBUG("evergreen_irq_set: sw int cp1\n");
			cp_int_cntl1 |= TIME_STAMP_INT_ENABLE;
		}
		if (rdev->irq.sw_int[CAYMAN_RING_TYPE_CP2_INDEX]) {
=======
	if (rdev->family == CHIP_ARUBA)
		thermal_int = RREG32(TN_CG_THERMAL_INT_CTRL) &
			~(THERM_INT_MASK_HIGH | THERM_INT_MASK_LOW);
	else
		thermal_int = RREG32(CG_THERMAL_INT) &
			~(THERM_INT_MASK_HIGH | THERM_INT_MASK_LOW);

	afmt1 = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC0_REGISTER_OFFSET) & ~AFMT_AZ_FORMAT_WTRIG_MASK;
	afmt2 = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC1_REGISTER_OFFSET) & ~AFMT_AZ_FORMAT_WTRIG_MASK;
	afmt3 = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC2_REGISTER_OFFSET) & ~AFMT_AZ_FORMAT_WTRIG_MASK;
	afmt4 = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC3_REGISTER_OFFSET) & ~AFMT_AZ_FORMAT_WTRIG_MASK;
	afmt5 = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC4_REGISTER_OFFSET) & ~AFMT_AZ_FORMAT_WTRIG_MASK;
	afmt6 = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC5_REGISTER_OFFSET) & ~AFMT_AZ_FORMAT_WTRIG_MASK;

	dma_cntl = RREG32(DMA_CNTL) & ~TRAP_ENABLE;

	if (rdev->family >= CHIP_CAYMAN) {
		/* enable CP interrupts on all rings */
		if (atomic_read(&rdev->irq.ring_int[RADEON_RING_TYPE_GFX_INDEX])) {
			DRM_DEBUG("evergreen_irq_set: sw int gfx\n");
			cp_int_cntl |= TIME_STAMP_INT_ENABLE;
		}
		if (atomic_read(&rdev->irq.ring_int[CAYMAN_RING_TYPE_CP1_INDEX])) {
			DRM_DEBUG("evergreen_irq_set: sw int cp1\n");
			cp_int_cntl1 |= TIME_STAMP_INT_ENABLE;
		}
		if (atomic_read(&rdev->irq.ring_int[CAYMAN_RING_TYPE_CP2_INDEX])) {
>>>>>>> refs/remotes/origin/master
			DRM_DEBUG("evergreen_irq_set: sw int cp2\n");
			cp_int_cntl2 |= TIME_STAMP_INT_ENABLE;
		}
	} else {
<<<<<<< HEAD
		if (rdev->irq.sw_int[RADEON_RING_TYPE_GFX_INDEX]) {
=======
		if (atomic_read(&rdev->irq.ring_int[RADEON_RING_TYPE_GFX_INDEX])) {
>>>>>>> refs/remotes/origin/master
			DRM_DEBUG("evergreen_irq_set: sw int gfx\n");
			cp_int_cntl |= RB_INT_ENABLE;
			cp_int_cntl |= TIME_STAMP_INT_ENABLE;
		}
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (rdev->irq.crtc_vblank_int[0] ||
	    rdev->irq.pflip[0]) {
=======
	if (atomic_read(&rdev->irq.ring_int[R600_RING_TYPE_DMA_INDEX])) {
		DRM_DEBUG("r600_irq_set: sw int dma\n");
		dma_cntl |= TRAP_ENABLE;
	}

	if (rdev->family >= CHIP_CAYMAN) {
		dma_cntl1 = RREG32(CAYMAN_DMA1_CNTL) & ~TRAP_ENABLE;
		if (atomic_read(&rdev->irq.ring_int[CAYMAN_RING_TYPE_DMA1_INDEX])) {
			DRM_DEBUG("r600_irq_set: sw int dma1\n");
			dma_cntl1 |= TRAP_ENABLE;
		}
	}

	if (rdev->irq.dpm_thermal) {
		DRM_DEBUG("dpm thermal\n");
		thermal_int |= THERM_INT_MASK_HIGH | THERM_INT_MASK_LOW;
	}

	if (rdev->irq.crtc_vblank_int[0] ||
	    atomic_read(&rdev->irq.pflip[0])) {
>>>>>>> refs/remotes/origin/master
		DRM_DEBUG("evergreen_irq_set: vblank 0\n");
		crtc1 |= VBLANK_INT_MASK;
	}
	if (rdev->irq.crtc_vblank_int[1] ||
<<<<<<< HEAD
	    rdev->irq.pflip[1]) {
=======
	    atomic_read(&rdev->irq.pflip[1])) {
>>>>>>> refs/remotes/origin/master
		DRM_DEBUG("evergreen_irq_set: vblank 1\n");
		crtc2 |= VBLANK_INT_MASK;
	}
	if (rdev->irq.crtc_vblank_int[2] ||
<<<<<<< HEAD
	    rdev->irq.pflip[2]) {
=======
	    atomic_read(&rdev->irq.pflip[2])) {
>>>>>>> refs/remotes/origin/master
		DRM_DEBUG("evergreen_irq_set: vblank 2\n");
		crtc3 |= VBLANK_INT_MASK;
	}
	if (rdev->irq.crtc_vblank_int[3] ||
<<<<<<< HEAD
	    rdev->irq.pflip[3]) {
=======
	    atomic_read(&rdev->irq.pflip[3])) {
>>>>>>> refs/remotes/origin/master
		DRM_DEBUG("evergreen_irq_set: vblank 3\n");
		crtc4 |= VBLANK_INT_MASK;
	}
	if (rdev->irq.crtc_vblank_int[4] ||
<<<<<<< HEAD
	    rdev->irq.pflip[4]) {
=======
	    atomic_read(&rdev->irq.pflip[4])) {
>>>>>>> refs/remotes/origin/master
		DRM_DEBUG("evergreen_irq_set: vblank 4\n");
		crtc5 |= VBLANK_INT_MASK;
	}
	if (rdev->irq.crtc_vblank_int[5] ||
<<<<<<< HEAD
	    rdev->irq.pflip[5]) {
=======
	    atomic_read(&rdev->irq.pflip[5])) {
>>>>>>> refs/remotes/origin/master
		DRM_DEBUG("evergreen_irq_set: vblank 5\n");
		crtc6 |= VBLANK_INT_MASK;
	}
	if (rdev->irq.hpd[0]) {
		DRM_DEBUG("evergreen_irq_set: hpd 1\n");
		hpd1 |= DC_HPDx_INT_EN;
	}
	if (rdev->irq.hpd[1]) {
		DRM_DEBUG("evergreen_irq_set: hpd 2\n");
		hpd2 |= DC_HPDx_INT_EN;
	}
	if (rdev->irq.hpd[2]) {
		DRM_DEBUG("evergreen_irq_set: hpd 3\n");
		hpd3 |= DC_HPDx_INT_EN;
	}
	if (rdev->irq.hpd[3]) {
		DRM_DEBUG("evergreen_irq_set: hpd 4\n");
		hpd4 |= DC_HPDx_INT_EN;
	}
	if (rdev->irq.hpd[4]) {
		DRM_DEBUG("evergreen_irq_set: hpd 5\n");
		hpd5 |= DC_HPDx_INT_EN;
	}
	if (rdev->irq.hpd[5]) {
		DRM_DEBUG("evergreen_irq_set: hpd 6\n");
		hpd6 |= DC_HPDx_INT_EN;
	}
<<<<<<< HEAD
	if (rdev->irq.gui_idle) {
		DRM_DEBUG("gui idle\n");
		grbm_int_cntl |= GUI_IDLE_INT_ENABLE;
	}

<<<<<<< HEAD
	WREG32(CP_INT_CNTL, cp_int_cntl);
=======
=======
	if (rdev->irq.afmt[0]) {
		DRM_DEBUG("evergreen_irq_set: hdmi 0\n");
		afmt1 |= AFMT_AZ_FORMAT_WTRIG_MASK;
	}
	if (rdev->irq.afmt[1]) {
		DRM_DEBUG("evergreen_irq_set: hdmi 1\n");
		afmt2 |= AFMT_AZ_FORMAT_WTRIG_MASK;
	}
	if (rdev->irq.afmt[2]) {
		DRM_DEBUG("evergreen_irq_set: hdmi 2\n");
		afmt3 |= AFMT_AZ_FORMAT_WTRIG_MASK;
	}
	if (rdev->irq.afmt[3]) {
		DRM_DEBUG("evergreen_irq_set: hdmi 3\n");
		afmt4 |= AFMT_AZ_FORMAT_WTRIG_MASK;
	}
	if (rdev->irq.afmt[4]) {
		DRM_DEBUG("evergreen_irq_set: hdmi 4\n");
		afmt5 |= AFMT_AZ_FORMAT_WTRIG_MASK;
	}
	if (rdev->irq.afmt[5]) {
		DRM_DEBUG("evergreen_irq_set: hdmi 5\n");
		afmt6 |= AFMT_AZ_FORMAT_WTRIG_MASK;
	}

>>>>>>> refs/remotes/origin/master
	if (rdev->family >= CHIP_CAYMAN) {
		cayman_cp_int_cntl_setup(rdev, 0, cp_int_cntl);
		cayman_cp_int_cntl_setup(rdev, 1, cp_int_cntl1);
		cayman_cp_int_cntl_setup(rdev, 2, cp_int_cntl2);
	} else
		WREG32(CP_INT_CNTL, cp_int_cntl);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

	WREG32(DMA_CNTL, dma_cntl);

	if (rdev->family >= CHIP_CAYMAN)
		WREG32(CAYMAN_DMA1_CNTL, dma_cntl1);

>>>>>>> refs/remotes/origin/master
	WREG32(GRBM_INT_CNTL, grbm_int_cntl);

	WREG32(INT_MASK + EVERGREEN_CRTC0_REGISTER_OFFSET, crtc1);
	WREG32(INT_MASK + EVERGREEN_CRTC1_REGISTER_OFFSET, crtc2);
	if (rdev->num_crtc >= 4) {
		WREG32(INT_MASK + EVERGREEN_CRTC2_REGISTER_OFFSET, crtc3);
		WREG32(INT_MASK + EVERGREEN_CRTC3_REGISTER_OFFSET, crtc4);
	}
	if (rdev->num_crtc >= 6) {
		WREG32(INT_MASK + EVERGREEN_CRTC4_REGISTER_OFFSET, crtc5);
		WREG32(INT_MASK + EVERGREEN_CRTC5_REGISTER_OFFSET, crtc6);
	}

	WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC0_REGISTER_OFFSET, grph1);
	WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC1_REGISTER_OFFSET, grph2);
	if (rdev->num_crtc >= 4) {
		WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC2_REGISTER_OFFSET, grph3);
		WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC3_REGISTER_OFFSET, grph4);
	}
	if (rdev->num_crtc >= 6) {
		WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC4_REGISTER_OFFSET, grph5);
		WREG32(GRPH_INT_CONTROL + EVERGREEN_CRTC5_REGISTER_OFFSET, grph6);
	}

	WREG32(DC_HPD1_INT_CONTROL, hpd1);
	WREG32(DC_HPD2_INT_CONTROL, hpd2);
	WREG32(DC_HPD3_INT_CONTROL, hpd3);
	WREG32(DC_HPD4_INT_CONTROL, hpd4);
	WREG32(DC_HPD5_INT_CONTROL, hpd5);
	WREG32(DC_HPD6_INT_CONTROL, hpd6);
<<<<<<< HEAD
=======
	if (rdev->family == CHIP_ARUBA)
		WREG32(TN_CG_THERMAL_INT_CTRL, thermal_int);
	else
		WREG32(CG_THERMAL_INT, thermal_int);

	WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC0_REGISTER_OFFSET, afmt1);
	WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC1_REGISTER_OFFSET, afmt2);
	WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC2_REGISTER_OFFSET, afmt3);
	WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC3_REGISTER_OFFSET, afmt4);
	WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC4_REGISTER_OFFSET, afmt5);
	WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC5_REGISTER_OFFSET, afmt6);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void evergreen_irq_ack(struct radeon_device *rdev)
=======
static void evergreen_irq_ack(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void evergreen_irq_ack(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/master
{
	u32 tmp;

	rdev->irq.stat_regs.evergreen.disp_int = RREG32(DISP_INTERRUPT_STATUS);
	rdev->irq.stat_regs.evergreen.disp_int_cont = RREG32(DISP_INTERRUPT_STATUS_CONTINUE);
	rdev->irq.stat_regs.evergreen.disp_int_cont2 = RREG32(DISP_INTERRUPT_STATUS_CONTINUE2);
	rdev->irq.stat_regs.evergreen.disp_int_cont3 = RREG32(DISP_INTERRUPT_STATUS_CONTINUE3);
	rdev->irq.stat_regs.evergreen.disp_int_cont4 = RREG32(DISP_INTERRUPT_STATUS_CONTINUE4);
	rdev->irq.stat_regs.evergreen.disp_int_cont5 = RREG32(DISP_INTERRUPT_STATUS_CONTINUE5);
	rdev->irq.stat_regs.evergreen.d1grph_int = RREG32(GRPH_INT_STATUS + EVERGREEN_CRTC0_REGISTER_OFFSET);
	rdev->irq.stat_regs.evergreen.d2grph_int = RREG32(GRPH_INT_STATUS + EVERGREEN_CRTC1_REGISTER_OFFSET);
	if (rdev->num_crtc >= 4) {
		rdev->irq.stat_regs.evergreen.d3grph_int = RREG32(GRPH_INT_STATUS + EVERGREEN_CRTC2_REGISTER_OFFSET);
		rdev->irq.stat_regs.evergreen.d4grph_int = RREG32(GRPH_INT_STATUS + EVERGREEN_CRTC3_REGISTER_OFFSET);
	}
	if (rdev->num_crtc >= 6) {
		rdev->irq.stat_regs.evergreen.d5grph_int = RREG32(GRPH_INT_STATUS + EVERGREEN_CRTC4_REGISTER_OFFSET);
		rdev->irq.stat_regs.evergreen.d6grph_int = RREG32(GRPH_INT_STATUS + EVERGREEN_CRTC5_REGISTER_OFFSET);
	}

<<<<<<< HEAD
=======
	rdev->irq.stat_regs.evergreen.afmt_status1 = RREG32(AFMT_STATUS + EVERGREEN_CRTC0_REGISTER_OFFSET);
	rdev->irq.stat_regs.evergreen.afmt_status2 = RREG32(AFMT_STATUS + EVERGREEN_CRTC1_REGISTER_OFFSET);
	rdev->irq.stat_regs.evergreen.afmt_status3 = RREG32(AFMT_STATUS + EVERGREEN_CRTC2_REGISTER_OFFSET);
	rdev->irq.stat_regs.evergreen.afmt_status4 = RREG32(AFMT_STATUS + EVERGREEN_CRTC3_REGISTER_OFFSET);
	rdev->irq.stat_regs.evergreen.afmt_status5 = RREG32(AFMT_STATUS + EVERGREEN_CRTC4_REGISTER_OFFSET);
	rdev->irq.stat_regs.evergreen.afmt_status6 = RREG32(AFMT_STATUS + EVERGREEN_CRTC5_REGISTER_OFFSET);

>>>>>>> refs/remotes/origin/master
	if (rdev->irq.stat_regs.evergreen.d1grph_int & GRPH_PFLIP_INT_OCCURRED)
		WREG32(GRPH_INT_STATUS + EVERGREEN_CRTC0_REGISTER_OFFSET, GRPH_PFLIP_INT_CLEAR);
	if (rdev->irq.stat_regs.evergreen.d2grph_int & GRPH_PFLIP_INT_OCCURRED)
		WREG32(GRPH_INT_STATUS + EVERGREEN_CRTC1_REGISTER_OFFSET, GRPH_PFLIP_INT_CLEAR);
	if (rdev->irq.stat_regs.evergreen.disp_int & LB_D1_VBLANK_INTERRUPT)
		WREG32(VBLANK_STATUS + EVERGREEN_CRTC0_REGISTER_OFFSET, VBLANK_ACK);
	if (rdev->irq.stat_regs.evergreen.disp_int & LB_D1_VLINE_INTERRUPT)
		WREG32(VLINE_STATUS + EVERGREEN_CRTC0_REGISTER_OFFSET, VLINE_ACK);
	if (rdev->irq.stat_regs.evergreen.disp_int_cont & LB_D2_VBLANK_INTERRUPT)
		WREG32(VBLANK_STATUS + EVERGREEN_CRTC1_REGISTER_OFFSET, VBLANK_ACK);
	if (rdev->irq.stat_regs.evergreen.disp_int_cont & LB_D2_VLINE_INTERRUPT)
		WREG32(VLINE_STATUS + EVERGREEN_CRTC1_REGISTER_OFFSET, VLINE_ACK);

	if (rdev->num_crtc >= 4) {
		if (rdev->irq.stat_regs.evergreen.d3grph_int & GRPH_PFLIP_INT_OCCURRED)
			WREG32(GRPH_INT_STATUS + EVERGREEN_CRTC2_REGISTER_OFFSET, GRPH_PFLIP_INT_CLEAR);
		if (rdev->irq.stat_regs.evergreen.d4grph_int & GRPH_PFLIP_INT_OCCURRED)
			WREG32(GRPH_INT_STATUS + EVERGREEN_CRTC3_REGISTER_OFFSET, GRPH_PFLIP_INT_CLEAR);
		if (rdev->irq.stat_regs.evergreen.disp_int_cont2 & LB_D3_VBLANK_INTERRUPT)
			WREG32(VBLANK_STATUS + EVERGREEN_CRTC2_REGISTER_OFFSET, VBLANK_ACK);
		if (rdev->irq.stat_regs.evergreen.disp_int_cont2 & LB_D3_VLINE_INTERRUPT)
			WREG32(VLINE_STATUS + EVERGREEN_CRTC2_REGISTER_OFFSET, VLINE_ACK);
		if (rdev->irq.stat_regs.evergreen.disp_int_cont3 & LB_D4_VBLANK_INTERRUPT)
			WREG32(VBLANK_STATUS + EVERGREEN_CRTC3_REGISTER_OFFSET, VBLANK_ACK);
		if (rdev->irq.stat_regs.evergreen.disp_int_cont3 & LB_D4_VLINE_INTERRUPT)
			WREG32(VLINE_STATUS + EVERGREEN_CRTC3_REGISTER_OFFSET, VLINE_ACK);
	}

	if (rdev->num_crtc >= 6) {
		if (rdev->irq.stat_regs.evergreen.d5grph_int & GRPH_PFLIP_INT_OCCURRED)
			WREG32(GRPH_INT_STATUS + EVERGREEN_CRTC4_REGISTER_OFFSET, GRPH_PFLIP_INT_CLEAR);
		if (rdev->irq.stat_regs.evergreen.d6grph_int & GRPH_PFLIP_INT_OCCURRED)
			WREG32(GRPH_INT_STATUS + EVERGREEN_CRTC5_REGISTER_OFFSET, GRPH_PFLIP_INT_CLEAR);
		if (rdev->irq.stat_regs.evergreen.disp_int_cont4 & LB_D5_VBLANK_INTERRUPT)
			WREG32(VBLANK_STATUS + EVERGREEN_CRTC4_REGISTER_OFFSET, VBLANK_ACK);
		if (rdev->irq.stat_regs.evergreen.disp_int_cont4 & LB_D5_VLINE_INTERRUPT)
			WREG32(VLINE_STATUS + EVERGREEN_CRTC4_REGISTER_OFFSET, VLINE_ACK);
		if (rdev->irq.stat_regs.evergreen.disp_int_cont5 & LB_D6_VBLANK_INTERRUPT)
			WREG32(VBLANK_STATUS + EVERGREEN_CRTC5_REGISTER_OFFSET, VBLANK_ACK);
		if (rdev->irq.stat_regs.evergreen.disp_int_cont5 & LB_D6_VLINE_INTERRUPT)
			WREG32(VLINE_STATUS + EVERGREEN_CRTC5_REGISTER_OFFSET, VLINE_ACK);
	}

	if (rdev->irq.stat_regs.evergreen.disp_int & DC_HPD1_INTERRUPT) {
		tmp = RREG32(DC_HPD1_INT_CONTROL);
		tmp |= DC_HPDx_INT_ACK;
		WREG32(DC_HPD1_INT_CONTROL, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.disp_int_cont & DC_HPD2_INTERRUPT) {
		tmp = RREG32(DC_HPD2_INT_CONTROL);
		tmp |= DC_HPDx_INT_ACK;
		WREG32(DC_HPD2_INT_CONTROL, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.disp_int_cont2 & DC_HPD3_INTERRUPT) {
		tmp = RREG32(DC_HPD3_INT_CONTROL);
		tmp |= DC_HPDx_INT_ACK;
		WREG32(DC_HPD3_INT_CONTROL, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.disp_int_cont3 & DC_HPD4_INTERRUPT) {
		tmp = RREG32(DC_HPD4_INT_CONTROL);
		tmp |= DC_HPDx_INT_ACK;
		WREG32(DC_HPD4_INT_CONTROL, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.disp_int_cont4 & DC_HPD5_INTERRUPT) {
		tmp = RREG32(DC_HPD5_INT_CONTROL);
		tmp |= DC_HPDx_INT_ACK;
		WREG32(DC_HPD5_INT_CONTROL, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.disp_int_cont5 & DC_HPD6_INTERRUPT) {
		tmp = RREG32(DC_HPD5_INT_CONTROL);
		tmp |= DC_HPDx_INT_ACK;
		WREG32(DC_HPD6_INT_CONTROL, tmp);
	}
<<<<<<< HEAD
}

void evergreen_irq_disable(struct radeon_device *rdev)
=======
	if (rdev->irq.stat_regs.evergreen.afmt_status1 & AFMT_AZ_FORMAT_WTRIG) {
		tmp = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC0_REGISTER_OFFSET);
		tmp |= AFMT_AZ_FORMAT_WTRIG_ACK;
		WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC0_REGISTER_OFFSET, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.afmt_status2 & AFMT_AZ_FORMAT_WTRIG) {
		tmp = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC1_REGISTER_OFFSET);
		tmp |= AFMT_AZ_FORMAT_WTRIG_ACK;
		WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC1_REGISTER_OFFSET, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.afmt_status3 & AFMT_AZ_FORMAT_WTRIG) {
		tmp = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC2_REGISTER_OFFSET);
		tmp |= AFMT_AZ_FORMAT_WTRIG_ACK;
		WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC2_REGISTER_OFFSET, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.afmt_status4 & AFMT_AZ_FORMAT_WTRIG) {
		tmp = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC3_REGISTER_OFFSET);
		tmp |= AFMT_AZ_FORMAT_WTRIG_ACK;
		WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC3_REGISTER_OFFSET, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.afmt_status5 & AFMT_AZ_FORMAT_WTRIG) {
		tmp = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC4_REGISTER_OFFSET);
		tmp |= AFMT_AZ_FORMAT_WTRIG_ACK;
		WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC4_REGISTER_OFFSET, tmp);
	}
	if (rdev->irq.stat_regs.evergreen.afmt_status6 & AFMT_AZ_FORMAT_WTRIG) {
		tmp = RREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC5_REGISTER_OFFSET);
		tmp |= AFMT_AZ_FORMAT_WTRIG_ACK;
		WREG32(AFMT_AUDIO_PACKET_CONTROL + EVERGREEN_CRTC5_REGISTER_OFFSET, tmp);
	}
}

static void evergreen_irq_disable(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/master
{
	r600_disable_interrupts(rdev);
	/* Wait and acknowledge irq */
	mdelay(1);
	evergreen_irq_ack(rdev);
	evergreen_disable_interrupt_state(rdev);
}

void evergreen_irq_suspend(struct radeon_device *rdev)
{
	evergreen_irq_disable(rdev);
	r600_rlc_stop(rdev);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline u32 evergreen_get_ih_wptr(struct radeon_device *rdev)
=======
static u32 evergreen_get_ih_wptr(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static u32 evergreen_get_ih_wptr(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/master
{
	u32 wptr, tmp;

	if (rdev->wb.enabled)
		wptr = le32_to_cpu(rdev->wb.wb[R600_WB_IH_WPTR_OFFSET/4]);
	else
		wptr = RREG32(IH_RB_WPTR);

	if (wptr & RB_OVERFLOW) {
		/* When a ring buffer overflow happen start parsing interrupt
		 * from the last not overwritten vector (wptr + 16). Hopefully
		 * this should allow us to catchup.
		 */
		dev_warn(rdev->dev, "IH ring buffer overflow (0x%08X, %d, %d)\n",
			wptr, rdev->ih.rptr, (wptr + 16) + rdev->ih.ptr_mask);
		rdev->ih.rptr = (wptr + 16) & rdev->ih.ptr_mask;
		tmp = RREG32(IH_RB_CNTL);
		tmp |= IH_WPTR_OVERFLOW_CLEAR;
		WREG32(IH_RB_CNTL, tmp);
	}
	return (wptr & rdev->ih.ptr_mask);
}

int evergreen_irq_process(struct radeon_device *rdev)
{
	u32 wptr;
	u32 rptr;
	u32 src_id, src_data;
	u32 ring_index;
<<<<<<< HEAD
	unsigned long flags;
	bool queue_hotplug = false;
=======
	bool queue_hotplug = false;
	bool queue_hdmi = false;
	bool queue_thermal = false;
	u32 status, addr;
>>>>>>> refs/remotes/origin/master

	if (!rdev->ih.enabled || rdev->shutdown)
		return IRQ_NONE;

	wptr = evergreen_get_ih_wptr(rdev);
<<<<<<< HEAD
	rptr = rdev->ih.rptr;
	DRM_DEBUG("r600_irq_process start: rptr %d, wptr %d\n", rptr, wptr);

	spin_lock_irqsave(&rdev->ih.lock, flags);
	if (rptr == wptr) {
		spin_unlock_irqrestore(&rdev->ih.lock, flags);
		return IRQ_NONE;
	}
restart_ih:
<<<<<<< HEAD
=======
	/* Order reading of wptr vs. reading of IH ring data */
	rmb();

>>>>>>> refs/remotes/origin/cm-10.0
	/* display interrupts */
	evergreen_irq_ack(rdev);

	rdev->ih.wptr = wptr;
=======

restart_ih:
	/* is somebody else already processing irqs? */
	if (atomic_xchg(&rdev->ih.lock, 1))
		return IRQ_NONE;

	rptr = rdev->ih.rptr;
	DRM_DEBUG("r600_irq_process start: rptr %d, wptr %d\n", rptr, wptr);

	/* Order reading of wptr vs. reading of IH ring data */
	rmb();

	/* display interrupts */
	evergreen_irq_ack(rdev);

>>>>>>> refs/remotes/origin/master
	while (rptr != wptr) {
		/* wptr/rptr are in bytes! */
		ring_index = rptr / 4;
		src_id =  le32_to_cpu(rdev->ih.ring[ring_index]) & 0xff;
		src_data = le32_to_cpu(rdev->ih.ring[ring_index + 1]) & 0xfffffff;

		switch (src_id) {
		case 1: /* D1 vblank/vline */
			switch (src_data) {
			case 0: /* D1 vblank */
				if (rdev->irq.stat_regs.evergreen.disp_int & LB_D1_VBLANK_INTERRUPT) {
					if (rdev->irq.crtc_vblank_int[0]) {
						drm_handle_vblank(rdev->ddev, 0);
						rdev->pm.vblank_sync = true;
						wake_up(&rdev->irq.vblank_queue);
					}
<<<<<<< HEAD
					if (rdev->irq.pflip[0])
=======
					if (atomic_read(&rdev->irq.pflip[0]))
>>>>>>> refs/remotes/origin/master
						radeon_crtc_handle_flip(rdev, 0);
					rdev->irq.stat_regs.evergreen.disp_int &= ~LB_D1_VBLANK_INTERRUPT;
					DRM_DEBUG("IH: D1 vblank\n");
				}
				break;
			case 1: /* D1 vline */
				if (rdev->irq.stat_regs.evergreen.disp_int & LB_D1_VLINE_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int &= ~LB_D1_VLINE_INTERRUPT;
					DRM_DEBUG("IH: D1 vline\n");
				}
				break;
			default:
				DRM_DEBUG("Unhandled interrupt: %d %d\n", src_id, src_data);
				break;
			}
			break;
		case 2: /* D2 vblank/vline */
			switch (src_data) {
			case 0: /* D2 vblank */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont & LB_D2_VBLANK_INTERRUPT) {
					if (rdev->irq.crtc_vblank_int[1]) {
						drm_handle_vblank(rdev->ddev, 1);
						rdev->pm.vblank_sync = true;
						wake_up(&rdev->irq.vblank_queue);
					}
<<<<<<< HEAD
					if (rdev->irq.pflip[1])
=======
					if (atomic_read(&rdev->irq.pflip[1]))
>>>>>>> refs/remotes/origin/master
						radeon_crtc_handle_flip(rdev, 1);
					rdev->irq.stat_regs.evergreen.disp_int_cont &= ~LB_D2_VBLANK_INTERRUPT;
					DRM_DEBUG("IH: D2 vblank\n");
				}
				break;
			case 1: /* D2 vline */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont & LB_D2_VLINE_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont &= ~LB_D2_VLINE_INTERRUPT;
					DRM_DEBUG("IH: D2 vline\n");
				}
				break;
			default:
				DRM_DEBUG("Unhandled interrupt: %d %d\n", src_id, src_data);
				break;
			}
			break;
		case 3: /* D3 vblank/vline */
			switch (src_data) {
			case 0: /* D3 vblank */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont2 & LB_D3_VBLANK_INTERRUPT) {
					if (rdev->irq.crtc_vblank_int[2]) {
						drm_handle_vblank(rdev->ddev, 2);
						rdev->pm.vblank_sync = true;
						wake_up(&rdev->irq.vblank_queue);
					}
<<<<<<< HEAD
					if (rdev->irq.pflip[2])
=======
					if (atomic_read(&rdev->irq.pflip[2]))
>>>>>>> refs/remotes/origin/master
						radeon_crtc_handle_flip(rdev, 2);
					rdev->irq.stat_regs.evergreen.disp_int_cont2 &= ~LB_D3_VBLANK_INTERRUPT;
					DRM_DEBUG("IH: D3 vblank\n");
				}
				break;
			case 1: /* D3 vline */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont2 & LB_D3_VLINE_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont2 &= ~LB_D3_VLINE_INTERRUPT;
					DRM_DEBUG("IH: D3 vline\n");
				}
				break;
			default:
				DRM_DEBUG("Unhandled interrupt: %d %d\n", src_id, src_data);
				break;
			}
			break;
		case 4: /* D4 vblank/vline */
			switch (src_data) {
			case 0: /* D4 vblank */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont3 & LB_D4_VBLANK_INTERRUPT) {
					if (rdev->irq.crtc_vblank_int[3]) {
						drm_handle_vblank(rdev->ddev, 3);
						rdev->pm.vblank_sync = true;
						wake_up(&rdev->irq.vblank_queue);
					}
<<<<<<< HEAD
					if (rdev->irq.pflip[3])
=======
					if (atomic_read(&rdev->irq.pflip[3]))
>>>>>>> refs/remotes/origin/master
						radeon_crtc_handle_flip(rdev, 3);
					rdev->irq.stat_regs.evergreen.disp_int_cont3 &= ~LB_D4_VBLANK_INTERRUPT;
					DRM_DEBUG("IH: D4 vblank\n");
				}
				break;
			case 1: /* D4 vline */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont3 & LB_D4_VLINE_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont3 &= ~LB_D4_VLINE_INTERRUPT;
					DRM_DEBUG("IH: D4 vline\n");
				}
				break;
			default:
				DRM_DEBUG("Unhandled interrupt: %d %d\n", src_id, src_data);
				break;
			}
			break;
		case 5: /* D5 vblank/vline */
			switch (src_data) {
			case 0: /* D5 vblank */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont4 & LB_D5_VBLANK_INTERRUPT) {
					if (rdev->irq.crtc_vblank_int[4]) {
						drm_handle_vblank(rdev->ddev, 4);
						rdev->pm.vblank_sync = true;
						wake_up(&rdev->irq.vblank_queue);
					}
<<<<<<< HEAD
					if (rdev->irq.pflip[4])
=======
					if (atomic_read(&rdev->irq.pflip[4]))
>>>>>>> refs/remotes/origin/master
						radeon_crtc_handle_flip(rdev, 4);
					rdev->irq.stat_regs.evergreen.disp_int_cont4 &= ~LB_D5_VBLANK_INTERRUPT;
					DRM_DEBUG("IH: D5 vblank\n");
				}
				break;
			case 1: /* D5 vline */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont4 & LB_D5_VLINE_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont4 &= ~LB_D5_VLINE_INTERRUPT;
					DRM_DEBUG("IH: D5 vline\n");
				}
				break;
			default:
				DRM_DEBUG("Unhandled interrupt: %d %d\n", src_id, src_data);
				break;
			}
			break;
		case 6: /* D6 vblank/vline */
			switch (src_data) {
			case 0: /* D6 vblank */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont5 & LB_D6_VBLANK_INTERRUPT) {
					if (rdev->irq.crtc_vblank_int[5]) {
						drm_handle_vblank(rdev->ddev, 5);
						rdev->pm.vblank_sync = true;
						wake_up(&rdev->irq.vblank_queue);
					}
<<<<<<< HEAD
					if (rdev->irq.pflip[5])
=======
					if (atomic_read(&rdev->irq.pflip[5]))
>>>>>>> refs/remotes/origin/master
						radeon_crtc_handle_flip(rdev, 5);
					rdev->irq.stat_regs.evergreen.disp_int_cont5 &= ~LB_D6_VBLANK_INTERRUPT;
					DRM_DEBUG("IH: D6 vblank\n");
				}
				break;
			case 1: /* D6 vline */
				if (rdev->irq.stat_regs.evergreen.disp_int_cont5 & LB_D6_VLINE_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont5 &= ~LB_D6_VLINE_INTERRUPT;
					DRM_DEBUG("IH: D6 vline\n");
				}
				break;
			default:
				DRM_DEBUG("Unhandled interrupt: %d %d\n", src_id, src_data);
				break;
			}
			break;
		case 42: /* HPD hotplug */
			switch (src_data) {
			case 0:
				if (rdev->irq.stat_regs.evergreen.disp_int & DC_HPD1_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int &= ~DC_HPD1_INTERRUPT;
					queue_hotplug = true;
					DRM_DEBUG("IH: HPD1\n");
				}
				break;
			case 1:
				if (rdev->irq.stat_regs.evergreen.disp_int_cont & DC_HPD2_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont &= ~DC_HPD2_INTERRUPT;
					queue_hotplug = true;
					DRM_DEBUG("IH: HPD2\n");
				}
				break;
			case 2:
				if (rdev->irq.stat_regs.evergreen.disp_int_cont2 & DC_HPD3_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont2 &= ~DC_HPD3_INTERRUPT;
					queue_hotplug = true;
					DRM_DEBUG("IH: HPD3\n");
				}
				break;
			case 3:
				if (rdev->irq.stat_regs.evergreen.disp_int_cont3 & DC_HPD4_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont3 &= ~DC_HPD4_INTERRUPT;
					queue_hotplug = true;
					DRM_DEBUG("IH: HPD4\n");
				}
				break;
			case 4:
				if (rdev->irq.stat_regs.evergreen.disp_int_cont4 & DC_HPD5_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont4 &= ~DC_HPD5_INTERRUPT;
					queue_hotplug = true;
					DRM_DEBUG("IH: HPD5\n");
				}
				break;
			case 5:
				if (rdev->irq.stat_regs.evergreen.disp_int_cont5 & DC_HPD6_INTERRUPT) {
					rdev->irq.stat_regs.evergreen.disp_int_cont5 &= ~DC_HPD6_INTERRUPT;
					queue_hotplug = true;
					DRM_DEBUG("IH: HPD6\n");
				}
				break;
			default:
				DRM_DEBUG("Unhandled interrupt: %d %d\n", src_id, src_data);
				break;
			}
			break;
<<<<<<< HEAD
=======
		case 44: /* hdmi */
			switch (src_data) {
			case 0:
				if (rdev->irq.stat_regs.evergreen.afmt_status1 & AFMT_AZ_FORMAT_WTRIG) {
					rdev->irq.stat_regs.evergreen.afmt_status1 &= ~AFMT_AZ_FORMAT_WTRIG;
					queue_hdmi = true;
					DRM_DEBUG("IH: HDMI0\n");
				}
				break;
			case 1:
				if (rdev->irq.stat_regs.evergreen.afmt_status2 & AFMT_AZ_FORMAT_WTRIG) {
					rdev->irq.stat_regs.evergreen.afmt_status2 &= ~AFMT_AZ_FORMAT_WTRIG;
					queue_hdmi = true;
					DRM_DEBUG("IH: HDMI1\n");
				}
				break;
			case 2:
				if (rdev->irq.stat_regs.evergreen.afmt_status3 & AFMT_AZ_FORMAT_WTRIG) {
					rdev->irq.stat_regs.evergreen.afmt_status3 &= ~AFMT_AZ_FORMAT_WTRIG;
					queue_hdmi = true;
					DRM_DEBUG("IH: HDMI2\n");
				}
				break;
			case 3:
				if (rdev->irq.stat_regs.evergreen.afmt_status4 & AFMT_AZ_FORMAT_WTRIG) {
					rdev->irq.stat_regs.evergreen.afmt_status4 &= ~AFMT_AZ_FORMAT_WTRIG;
					queue_hdmi = true;
					DRM_DEBUG("IH: HDMI3\n");
				}
				break;
			case 4:
				if (rdev->irq.stat_regs.evergreen.afmt_status5 & AFMT_AZ_FORMAT_WTRIG) {
					rdev->irq.stat_regs.evergreen.afmt_status5 &= ~AFMT_AZ_FORMAT_WTRIG;
					queue_hdmi = true;
					DRM_DEBUG("IH: HDMI4\n");
				}
				break;
			case 5:
				if (rdev->irq.stat_regs.evergreen.afmt_status6 & AFMT_AZ_FORMAT_WTRIG) {
					rdev->irq.stat_regs.evergreen.afmt_status6 &= ~AFMT_AZ_FORMAT_WTRIG;
					queue_hdmi = true;
					DRM_DEBUG("IH: HDMI5\n");
				}
				break;
			default:
				DRM_ERROR("Unhandled interrupt: %d %d\n", src_id, src_data);
				break;
			}
		case 124: /* UVD */
			DRM_DEBUG("IH: UVD int: 0x%08x\n", src_data);
			radeon_fence_process(rdev, R600_RING_TYPE_UVD_INDEX);
			break;
		case 146:
		case 147:
			addr = RREG32(VM_CONTEXT1_PROTECTION_FAULT_ADDR);
			status = RREG32(VM_CONTEXT1_PROTECTION_FAULT_STATUS);
			dev_err(rdev->dev, "GPU fault detected: %d 0x%08x\n", src_id, src_data);
			dev_err(rdev->dev, "  VM_CONTEXT1_PROTECTION_FAULT_ADDR   0x%08X\n",
				addr);
			dev_err(rdev->dev, "  VM_CONTEXT1_PROTECTION_FAULT_STATUS 0x%08X\n",
				status);
			cayman_vm_decode_fault(rdev, status, addr);
			/* reset addr and status */
			WREG32_P(VM_CONTEXT1_CNTL2, 1, ~1);
			break;
>>>>>>> refs/remotes/origin/master
		case 176: /* CP_INT in ring buffer */
		case 177: /* CP_INT in IB1 */
		case 178: /* CP_INT in IB2 */
			DRM_DEBUG("IH: CP int: 0x%08x\n", src_data);
<<<<<<< HEAD
<<<<<<< HEAD
			radeon_fence_process(rdev);
			break;
		case 181: /* CP EOP event */
			DRM_DEBUG("IH: CP EOP\n");
			radeon_fence_process(rdev);
=======
=======
>>>>>>> refs/remotes/origin/master
			radeon_fence_process(rdev, RADEON_RING_TYPE_GFX_INDEX);
			break;
		case 181: /* CP EOP event */
			DRM_DEBUG("IH: CP EOP\n");
			if (rdev->family >= CHIP_CAYMAN) {
				switch (src_data) {
				case 0:
					radeon_fence_process(rdev, RADEON_RING_TYPE_GFX_INDEX);
					break;
				case 1:
					radeon_fence_process(rdev, CAYMAN_RING_TYPE_CP1_INDEX);
					break;
				case 2:
					radeon_fence_process(rdev, CAYMAN_RING_TYPE_CP2_INDEX);
					break;
				}
			} else
				radeon_fence_process(rdev, RADEON_RING_TYPE_GFX_INDEX);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		case 233: /* GUI IDLE */
			DRM_DEBUG("IH: GUI idle\n");
			rdev->pm.gui_idle = true;
			wake_up(&rdev->irq.idle_queue);
=======
			break;
		case 224: /* DMA trap event */
			DRM_DEBUG("IH: DMA trap\n");
			radeon_fence_process(rdev, R600_RING_TYPE_DMA_INDEX);
			break;
		case 230: /* thermal low to high */
			DRM_DEBUG("IH: thermal low to high\n");
			rdev->pm.dpm.thermal.high_to_low = false;
			queue_thermal = true;
			break;
		case 231: /* thermal high to low */
			DRM_DEBUG("IH: thermal high to low\n");
			rdev->pm.dpm.thermal.high_to_low = true;
			queue_thermal = true;
			break;
		case 233: /* GUI IDLE */
			DRM_DEBUG("IH: GUI idle\n");
			break;
		case 244: /* DMA trap event */
			if (rdev->family >= CHIP_CAYMAN) {
				DRM_DEBUG("IH: DMA1 trap\n");
				radeon_fence_process(rdev, CAYMAN_RING_TYPE_DMA1_INDEX);
			}
>>>>>>> refs/remotes/origin/master
			break;
		default:
			DRM_DEBUG("Unhandled interrupt: %d %d\n", src_id, src_data);
			break;
		}

		/* wptr/rptr are in bytes! */
		rptr += 16;
		rptr &= rdev->ih.ptr_mask;
	}
<<<<<<< HEAD
	/* make sure wptr hasn't changed while processing */
	wptr = evergreen_get_ih_wptr(rdev);
	if (wptr != rdev->ih.wptr)
		goto restart_ih;
	if (queue_hotplug)
		schedule_work(&rdev->hotplug_work);
	rdev->ih.rptr = rptr;
	WREG32(IH_RB_RPTR, rdev->ih.rptr);
	spin_unlock_irqrestore(&rdev->ih.lock, flags);
=======
	if (queue_hotplug)
		schedule_work(&rdev->hotplug_work);
	if (queue_hdmi)
		schedule_work(&rdev->audio_work);
	if (queue_thermal && rdev->pm.dpm_enabled)
		schedule_work(&rdev->pm.dpm.thermal.work);
	rdev->ih.rptr = rptr;
	WREG32(IH_RB_RPTR, rdev->ih.rptr);
	atomic_set(&rdev->ih.lock, 0);

	/* make sure wptr hasn't changed while processing */
	wptr = evergreen_get_ih_wptr(rdev);
	if (wptr != rptr)
		goto restart_ih;

>>>>>>> refs/remotes/origin/master
	return IRQ_HANDLED;
}

static int evergreen_startup(struct radeon_device *rdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int r;

	/* enable pcie gen2 link */
	if (!ASIC_IS_DCE5(rdev))
		evergreen_pcie_gen2_enable(rdev);
=======
	struct radeon_ring *ring = &rdev->ring[RADEON_RING_TYPE_GFX_INDEX];
=======
	struct radeon_ring *ring;
>>>>>>> refs/remotes/origin/master
	int r;

	/* enable pcie gen2 link */
	evergreen_pcie_gen2_enable(rdev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* enable aspm */
	evergreen_program_aspm(rdev);

	/* scratch needs to be initialized before MC */
	r = r600_vram_scratch_init(rdev);
	if (r)
		return r;

	evergreen_mc_program(rdev);
>>>>>>> refs/remotes/origin/master

	if (ASIC_IS_DCE5(rdev)) {
		if (!rdev->me_fw || !rdev->pfp_fw || !rdev->rlc_fw || !rdev->mc_fw) {
			r = ni_init_microcode(rdev);
			if (r) {
				DRM_ERROR("Failed to load firmware!\n");
				return r;
			}
		}
		r = ni_mc_load_microcode(rdev);
		if (r) {
			DRM_ERROR("Failed to load MC firmware!\n");
			return r;
		}
	} else {
		if (!rdev->me_fw || !rdev->pfp_fw || !rdev->rlc_fw) {
			r = r600_init_microcode(rdev);
			if (r) {
				DRM_ERROR("Failed to load firmware!\n");
				return r;
			}
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	r = r600_vram_scratch_init(rdev);
	if (r)
		return r;

>>>>>>> refs/remotes/origin/cm-10.0
	evergreen_mc_program(rdev);
=======
>>>>>>> refs/remotes/origin/master
	if (rdev->flags & RADEON_IS_AGP) {
		evergreen_agp_enable(rdev);
	} else {
		r = evergreen_pcie_gart_enable(rdev);
		if (r)
			return r;
	}
	evergreen_gpu_init(rdev);

<<<<<<< HEAD
	r = evergreen_blit_init(rdev);
	if (r) {
<<<<<<< HEAD
		evergreen_blit_fini(rdev);
		rdev->asic->copy = NULL;
=======
		r600_blit_fini(rdev);
		rdev->asic->copy.copy = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
		dev_warn(rdev->dev, "failed blitter (%d) falling back to memcpy\n", r);
=======
	/* allocate rlc buffers */
	if (rdev->flags & RADEON_IS_IGP) {
		rdev->rlc.reg_list = sumo_rlc_save_restore_register_list;
		rdev->rlc.reg_list_size =
			(u32)ARRAY_SIZE(sumo_rlc_save_restore_register_list);
		rdev->rlc.cs_data = evergreen_cs_data;
		r = sumo_rlc_init(rdev);
		if (r) {
			DRM_ERROR("Failed to init rlc BOs!\n");
			return r;
		}
>>>>>>> refs/remotes/origin/master
	}

	/* allocate wb buffer */
	r = radeon_wb_init(rdev);
	if (r)
		return r;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Enable IRQ */
=======
=======
>>>>>>> refs/remotes/origin/master
	r = radeon_fence_driver_start_ring(rdev, RADEON_RING_TYPE_GFX_INDEX);
	if (r) {
		dev_err(rdev->dev, "failed initializing CP fences (%d).\n", r);
		return r;
	}

<<<<<<< HEAD
=======
	r = radeon_fence_driver_start_ring(rdev, R600_RING_TYPE_DMA_INDEX);
	if (r) {
		dev_err(rdev->dev, "failed initializing DMA fences (%d).\n", r);
		return r;
	}

	r = uvd_v2_2_resume(rdev);
	if (!r) {
		r = radeon_fence_driver_start_ring(rdev,
						   R600_RING_TYPE_UVD_INDEX);
		if (r)
			dev_err(rdev->dev, "UVD fences init error (%d).\n", r);
	}

	if (r)
		rdev->ring[R600_RING_TYPE_UVD_INDEX].ring_size = 0;

>>>>>>> refs/remotes/origin/master
	/* Enable IRQ */
	if (!rdev->irq.installed) {
		r = radeon_irq_kms_init(rdev);
		if (r)
			return r;
	}

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	r = r600_irq_init(rdev);
	if (r) {
		DRM_ERROR("radeon: IH init failed (%d).\n", r);
		radeon_irq_kms_fini(rdev);
		return r;
	}
	evergreen_irq_set(rdev);

<<<<<<< HEAD
<<<<<<< HEAD
	r = radeon_ring_init(rdev, rdev->cp.ring_size);
=======
	r = radeon_ring_init(rdev, ring, ring->ring_size, RADEON_WB_CP_RPTR_OFFSET,
			     R600_CP_RB_RPTR, R600_CP_RB_WPTR,
			     0, 0xfffff, RADEON_CP_PACKET2);
>>>>>>> refs/remotes/origin/cm-10.0
	if (r)
		return r;
=======
	ring = &rdev->ring[RADEON_RING_TYPE_GFX_INDEX];
	r = radeon_ring_init(rdev, ring, ring->ring_size, RADEON_WB_CP_RPTR_OFFSET,
			     R600_CP_RB_RPTR, R600_CP_RB_WPTR,
			     RADEON_CP_PACKET2);
	if (r)
		return r;

	ring = &rdev->ring[R600_RING_TYPE_DMA_INDEX];
	r = radeon_ring_init(rdev, ring, ring->ring_size, R600_WB_DMA_RPTR_OFFSET,
			     DMA_RB_RPTR, DMA_RB_WPTR,
			     DMA_PACKET(DMA_PACKET_NOP, 0, 0));
	if (r)
		return r;

>>>>>>> refs/remotes/origin/master
	r = evergreen_cp_load_microcode(rdev);
	if (r)
		return r;
	r = evergreen_cp_resume(rdev);
	if (r)
		return r;
<<<<<<< HEAD

<<<<<<< HEAD
=======
	r = radeon_ib_pool_start(rdev);
	if (r)
		return r;

	r = radeon_ib_test(rdev, RADEON_RING_TYPE_GFX_INDEX, &rdev->ring[RADEON_RING_TYPE_GFX_INDEX]);
	if (r) {
		DRM_ERROR("radeon: failed testing IB (%d).\n", r);
		rdev->accel_working = false;
=======
	r = r600_dma_resume(rdev);
	if (r)
		return r;

	ring = &rdev->ring[R600_RING_TYPE_UVD_INDEX];
	if (ring->ring_size) {
		r = radeon_ring_init(rdev, ring, ring->ring_size, 0,
				     UVD_RBC_RB_RPTR, UVD_RBC_RB_WPTR,
				     RADEON_CP_PACKET2);
		if (!r)
			r = uvd_v1_0_init(rdev);

		if (r)
			DRM_ERROR("radeon: error initializing UVD (%d).\n", r);
	}

	r = radeon_ib_pool_init(rdev);
	if (r) {
		dev_err(rdev->dev, "IB initialization failed (%d).\n", r);
>>>>>>> refs/remotes/origin/master
		return r;
	}

	r = r600_audio_init(rdev);
	if (r) {
		DRM_ERROR("radeon: audio init failed\n");
		return r;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

int evergreen_resume(struct radeon_device *rdev)
{
	int r;

	/* reset the asic, the gfx blocks are often in a bad state
	 * after the driver is unloaded or after a resume
	 */
	if (radeon_asic_reset(rdev))
		dev_warn(rdev->dev, "GPU reset failed !\n");
	/* Do not reset GPU before posting, on rv770 hw unlike on r500 hw,
	 * posting will perform necessary task to bring back GPU into good
	 * shape.
	 */
	/* post card */
	atom_asic_init(rdev->mode_info.atom_context);

<<<<<<< HEAD
<<<<<<< HEAD
	r = evergreen_startup(rdev);
	if (r) {
		DRM_ERROR("evergreen startup failed on resume\n");
		return r;
	}

	r = r600_ib_test(rdev);
	if (r) {
		DRM_ERROR("radeon: failed testing IB (%d).\n", r);
=======
=======
	/* init golden registers */
	evergreen_init_golden_registers(rdev);

>>>>>>> refs/remotes/origin/master
	rdev->accel_working = true;
	r = evergreen_startup(rdev);
	if (r) {
		DRM_ERROR("evergreen startup failed on resume\n");
		rdev->accel_working = false;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return r;
	}

	return r;

}

int evergreen_suspend(struct radeon_device *rdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int r;

	/* FIXME: we should wait for ring to be empty */
	r700_cp_stop(rdev);
	rdev->cp.ready = false;
=======
	struct radeon_ring *ring = &rdev->ring[RADEON_RING_TYPE_GFX_INDEX];

	r600_audio_fini(rdev);
	/* FIXME: we should wait for ring to be empty */
	radeon_ib_pool_suspend(rdev);
	r600_blit_suspend(rdev);
	r700_cp_stop(rdev);
	ring->ready = false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	r600_audio_fini(rdev);
	uvd_v1_0_fini(rdev);
	radeon_uvd_suspend(rdev);
	r700_cp_stop(rdev);
	r600_dma_stop(rdev);
>>>>>>> refs/remotes/origin/master
	evergreen_irq_suspend(rdev);
	radeon_wb_disable(rdev);
	evergreen_pcie_gart_disable(rdev);

<<<<<<< HEAD
<<<<<<< HEAD
	/* unpin shaders bo */
	r = radeon_bo_reserve(rdev->r600_blit.shader_obj, false);
	if (likely(r == 0)) {
		radeon_bo_unpin(rdev->r600_blit.shader_obj);
		radeon_bo_unreserve(rdev->r600_blit.shader_obj);
	}

	return 0;
}

int evergreen_copy_blit(struct radeon_device *rdev,
			uint64_t src_offset,
			uint64_t dst_offset,
			unsigned num_gpu_pages,
			struct radeon_fence *fence)
{
	int r;

	mutex_lock(&rdev->r600_blit.mutex);
	rdev->r600_blit.vb_ib = NULL;
	r = evergreen_blit_prepare_copy(rdev, num_gpu_pages * RADEON_GPU_PAGE_SIZE);
	if (r) {
		if (rdev->r600_blit.vb_ib)
			radeon_ib_free(rdev, &rdev->r600_blit.vb_ib);
		mutex_unlock(&rdev->r600_blit.mutex);
		return r;
	}
	evergreen_kms_blit_copy(rdev, src_offset, dst_offset, num_gpu_pages * RADEON_GPU_PAGE_SIZE);
	evergreen_blit_done_copy(rdev, fence);
	mutex_unlock(&rdev->r600_blit.mutex);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

/* Plan is to move initialization in that function and use
 * helper function so that radeon_device_init pretty much
 * do nothing more than calling asic specific function. This
 * should also allow to remove a bunch of callback function
 * like vram_info.
 */
int evergreen_init(struct radeon_device *rdev)
{
	int r;

<<<<<<< HEAD
	/* This don't do much */
	r = radeon_gem_init(rdev);
	if (r)
		return r;
=======
>>>>>>> refs/remotes/origin/master
	/* Read BIOS */
	if (!radeon_get_bios(rdev)) {
		if (ASIC_IS_AVIVO(rdev))
			return -EINVAL;
	}
	/* Must be an ATOMBIOS */
	if (!rdev->is_atom_bios) {
		dev_err(rdev->dev, "Expecting atombios for evergreen GPU\n");
		return -EINVAL;
	}
	r = radeon_atombios_init(rdev);
	if (r)
		return r;
	/* reset the asic, the gfx blocks are often in a bad state
	 * after the driver is unloaded or after a resume
	 */
	if (radeon_asic_reset(rdev))
		dev_warn(rdev->dev, "GPU reset failed !\n");
	/* Post card if necessary */
	if (!radeon_card_posted(rdev)) {
		if (!rdev->bios) {
			dev_err(rdev->dev, "Card not posted and no BIOS - ignoring\n");
			return -EINVAL;
		}
		DRM_INFO("GPU not posted. posting now...\n");
		atom_asic_init(rdev->mode_info.atom_context);
	}
<<<<<<< HEAD
=======
	/* init golden registers */
	evergreen_init_golden_registers(rdev);
>>>>>>> refs/remotes/origin/master
	/* Initialize scratch registers */
	r600_scratch_init(rdev);
	/* Initialize surface registers */
	radeon_surface_init(rdev);
	/* Initialize clocks */
	radeon_get_clock_info(rdev->ddev);
	/* Fence driver */
	r = radeon_fence_driver_init(rdev);
	if (r)
		return r;
	/* initialize AGP */
	if (rdev->flags & RADEON_IS_AGP) {
		r = radeon_agp_init(rdev);
		if (r)
			radeon_agp_disable(rdev);
	}
	/* initialize memory controller */
	r = evergreen_mc_init(rdev);
	if (r)
		return r;
	/* Memory manager */
	r = radeon_bo_init(rdev);
	if (r)
		return r;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	r = radeon_irq_kms_init(rdev);
	if (r)
		return r;

	rdev->cp.ring_obj = NULL;
	r600_ring_init(rdev, 1024 * 1024);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	rdev->ring[RADEON_RING_TYPE_GFX_INDEX].ring_obj = NULL;
	r600_ring_init(rdev, &rdev->ring[RADEON_RING_TYPE_GFX_INDEX], 1024 * 1024);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rdev->ring[RADEON_RING_TYPE_GFX_INDEX].ring_obj = NULL;
	r600_ring_init(rdev, &rdev->ring[RADEON_RING_TYPE_GFX_INDEX], 1024 * 1024);

	rdev->ring[R600_RING_TYPE_DMA_INDEX].ring_obj = NULL;
	r600_ring_init(rdev, &rdev->ring[R600_RING_TYPE_DMA_INDEX], 64 * 1024);

	r = radeon_uvd_init(rdev);
	if (!r) {
		rdev->ring[R600_RING_TYPE_UVD_INDEX].ring_obj = NULL;
		r600_ring_init(rdev, &rdev->ring[R600_RING_TYPE_UVD_INDEX],
			       4096);
	}
>>>>>>> refs/remotes/origin/master

	rdev->ih.ring_obj = NULL;
	r600_ih_ring_init(rdev, 64 * 1024);

	r = r600_pcie_gart_init(rdev);
	if (r)
		return r;

<<<<<<< HEAD
<<<<<<< HEAD
	rdev->accel_working = true;
=======
	r = radeon_ib_pool_init(rdev);
	rdev->accel_working = true;
	if (r) {
		dev_err(rdev->dev, "IB initialization failed (%d).\n", r);
		rdev->accel_working = false;
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	rdev->accel_working = true;
>>>>>>> refs/remotes/origin/master
	r = evergreen_startup(rdev);
	if (r) {
		dev_err(rdev->dev, "disabling GPU acceleration\n");
		r700_cp_fini(rdev);
<<<<<<< HEAD
		r600_irq_fini(rdev);
		radeon_wb_fini(rdev);
<<<<<<< HEAD
=======
		r100_ib_fini(rdev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		r600_dma_fini(rdev);
		r600_irq_fini(rdev);
		if (rdev->flags & RADEON_IS_IGP)
			sumo_rlc_fini(rdev);
		radeon_wb_fini(rdev);
		radeon_ib_pool_fini(rdev);
>>>>>>> refs/remotes/origin/master
		radeon_irq_kms_fini(rdev);
		evergreen_pcie_gart_fini(rdev);
		rdev->accel_working = false;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	if (rdev->accel_working) {
		r = radeon_ib_pool_init(rdev);
		if (r) {
			DRM_ERROR("radeon: failed initializing IB pool (%d).\n", r);
			rdev->accel_working = false;
		}
		r = r600_ib_test(rdev);
		if (r) {
			DRM_ERROR("radeon: failed testing IB (%d).\n", r);
			rdev->accel_working = false;
		}
	}
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Don't start up if the MC ucode is missing on BTC parts.
	 * The default clocks and voltages before the MC ucode
	 * is loaded are not suffient for advanced operations.
	 */
	if (ASIC_IS_DCE5(rdev)) {
		if (!rdev->mc_fw && !(rdev->flags & RADEON_IS_IGP)) {
			DRM_ERROR("radeon: MC ucode required for NI+.\n");
			return -EINVAL;
		}
	}

	return 0;
}

void evergreen_fini(struct radeon_device *rdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	evergreen_blit_fini(rdev);
	r700_cp_fini(rdev);
	r600_irq_fini(rdev);
	radeon_wb_fini(rdev);
	radeon_ib_pool_fini(rdev);
	radeon_irq_kms_fini(rdev);
	evergreen_pcie_gart_fini(rdev);
	radeon_gem_fini(rdev);
=======
	r600_audio_fini(rdev);
	r600_blit_fini(rdev);
	r700_cp_fini(rdev);
	r600_irq_fini(rdev);
	radeon_wb_fini(rdev);
	r100_ib_fini(rdev);
	radeon_irq_kms_fini(rdev);
	evergreen_pcie_gart_fini(rdev);
	r600_vram_scratch_fini(rdev);
	radeon_gem_fini(rdev);
	radeon_semaphore_driver_fini(rdev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	r600_audio_fini(rdev);
	r700_cp_fini(rdev);
	r600_dma_fini(rdev);
	r600_irq_fini(rdev);
	if (rdev->flags & RADEON_IS_IGP)
		sumo_rlc_fini(rdev);
	radeon_wb_fini(rdev);
	radeon_ib_pool_fini(rdev);
	radeon_irq_kms_fini(rdev);
	evergreen_pcie_gart_fini(rdev);
	uvd_v1_0_fini(rdev);
	radeon_uvd_fini(rdev);
	r600_vram_scratch_fini(rdev);
	radeon_gem_fini(rdev);
>>>>>>> refs/remotes/origin/master
	radeon_fence_driver_fini(rdev);
	radeon_agp_fini(rdev);
	radeon_bo_fini(rdev);
	radeon_atombios_fini(rdev);
	kfree(rdev->bios);
	rdev->bios = NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void evergreen_pcie_gen2_enable(struct radeon_device *rdev)
=======
void evergreen_pcie_gen2_enable(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void evergreen_pcie_gen2_enable(struct radeon_device *rdev)
>>>>>>> refs/remotes/origin/master
{
	u32 link_width_cntl, speed_cntl;

	if (radeon_pcie_gen2 == 0)
		return;

	if (rdev->flags & RADEON_IS_IGP)
		return;

	if (!(rdev->flags & RADEON_IS_PCIE))
		return;

	/* x2 cards have a special sequence */
	if (ASIC_IS_X2(rdev))
		return;

<<<<<<< HEAD
	speed_cntl = RREG32_PCIE_P(PCIE_LC_SPEED_CNTL);
	if ((speed_cntl & LC_OTHER_SIDE_EVER_SENT_GEN2) ||
	    (speed_cntl & LC_OTHER_SIDE_SUPPORTS_GEN2)) {

		link_width_cntl = RREG32_PCIE_P(PCIE_LC_LINK_WIDTH_CNTL);
		link_width_cntl &= ~LC_UPCONFIGURE_DIS;
		WREG32_PCIE_P(PCIE_LC_LINK_WIDTH_CNTL, link_width_cntl);

		speed_cntl = RREG32_PCIE_P(PCIE_LC_SPEED_CNTL);
		speed_cntl &= ~LC_TARGET_LINK_SPEED_OVERRIDE_EN;
		WREG32_PCIE_P(PCIE_LC_SPEED_CNTL, speed_cntl);

		speed_cntl = RREG32_PCIE_P(PCIE_LC_SPEED_CNTL);
		speed_cntl |= LC_CLR_FAILED_SPD_CHANGE_CNT;
		WREG32_PCIE_P(PCIE_LC_SPEED_CNTL, speed_cntl);

		speed_cntl = RREG32_PCIE_P(PCIE_LC_SPEED_CNTL);
		speed_cntl &= ~LC_CLR_FAILED_SPD_CHANGE_CNT;
		WREG32_PCIE_P(PCIE_LC_SPEED_CNTL, speed_cntl);

		speed_cntl = RREG32_PCIE_P(PCIE_LC_SPEED_CNTL);
		speed_cntl |= LC_GEN2_EN_STRAP;
		WREG32_PCIE_P(PCIE_LC_SPEED_CNTL, speed_cntl);

	} else {
		link_width_cntl = RREG32_PCIE_P(PCIE_LC_LINK_WIDTH_CNTL);
=======
	if ((rdev->pdev->bus->max_bus_speed != PCIE_SPEED_5_0GT) &&
		(rdev->pdev->bus->max_bus_speed != PCIE_SPEED_8_0GT))
		return;

	speed_cntl = RREG32_PCIE_PORT(PCIE_LC_SPEED_CNTL);
	if (speed_cntl & LC_CURRENT_DATA_RATE) {
		DRM_INFO("PCIE gen 2 link speeds already enabled\n");
		return;
	}

	DRM_INFO("enabling PCIE gen 2 link speeds, disable with radeon.pcie_gen2=0\n");

	if ((speed_cntl & LC_OTHER_SIDE_EVER_SENT_GEN2) ||
	    (speed_cntl & LC_OTHER_SIDE_SUPPORTS_GEN2)) {

		link_width_cntl = RREG32_PCIE_PORT(PCIE_LC_LINK_WIDTH_CNTL);
		link_width_cntl &= ~LC_UPCONFIGURE_DIS;
		WREG32_PCIE_PORT(PCIE_LC_LINK_WIDTH_CNTL, link_width_cntl);

		speed_cntl = RREG32_PCIE_PORT(PCIE_LC_SPEED_CNTL);
		speed_cntl &= ~LC_TARGET_LINK_SPEED_OVERRIDE_EN;
		WREG32_PCIE_PORT(PCIE_LC_SPEED_CNTL, speed_cntl);

		speed_cntl = RREG32_PCIE_PORT(PCIE_LC_SPEED_CNTL);
		speed_cntl |= LC_CLR_FAILED_SPD_CHANGE_CNT;
		WREG32_PCIE_PORT(PCIE_LC_SPEED_CNTL, speed_cntl);

		speed_cntl = RREG32_PCIE_PORT(PCIE_LC_SPEED_CNTL);
		speed_cntl &= ~LC_CLR_FAILED_SPD_CHANGE_CNT;
		WREG32_PCIE_PORT(PCIE_LC_SPEED_CNTL, speed_cntl);

		speed_cntl = RREG32_PCIE_PORT(PCIE_LC_SPEED_CNTL);
		speed_cntl |= LC_GEN2_EN_STRAP;
		WREG32_PCIE_PORT(PCIE_LC_SPEED_CNTL, speed_cntl);

	} else {
		link_width_cntl = RREG32_PCIE_PORT(PCIE_LC_LINK_WIDTH_CNTL);
>>>>>>> refs/remotes/origin/master
		/* XXX: only disable it if gen1 bridge vendor == 0x111d or 0x1106 */
		if (1)
			link_width_cntl |= LC_UPCONFIGURE_DIS;
		else
			link_width_cntl &= ~LC_UPCONFIGURE_DIS;
<<<<<<< HEAD
		WREG32_PCIE_P(PCIE_LC_LINK_WIDTH_CNTL, link_width_cntl);
	}
=======
		WREG32_PCIE_PORT(PCIE_LC_LINK_WIDTH_CNTL, link_width_cntl);
	}
}

void evergreen_program_aspm(struct radeon_device *rdev)
{
	u32 data, orig;
	u32 pcie_lc_cntl, pcie_lc_cntl_old;
	bool disable_l0s, disable_l1 = false, disable_plloff_in_l1 = false;
	/* fusion_platform = true
	 * if the system is a fusion system
	 * (APU or DGPU in a fusion system).
	 * todo: check if the system is a fusion platform.
	 */
	bool fusion_platform = false;

	if (radeon_aspm == 0)
		return;

	if (!(rdev->flags & RADEON_IS_PCIE))
		return;

	switch (rdev->family) {
	case CHIP_CYPRESS:
	case CHIP_HEMLOCK:
	case CHIP_JUNIPER:
	case CHIP_REDWOOD:
	case CHIP_CEDAR:
	case CHIP_SUMO:
	case CHIP_SUMO2:
	case CHIP_PALM:
	case CHIP_ARUBA:
		disable_l0s = true;
		break;
	default:
		disable_l0s = false;
		break;
	}

	if (rdev->flags & RADEON_IS_IGP)
		fusion_platform = true; /* XXX also dGPUs in a fusion system */

	data = orig = RREG32_PIF_PHY0(PB0_PIF_PAIRING);
	if (fusion_platform)
		data &= ~MULTI_PIF;
	else
		data |= MULTI_PIF;
	if (data != orig)
		WREG32_PIF_PHY0(PB0_PIF_PAIRING, data);

	data = orig = RREG32_PIF_PHY1(PB1_PIF_PAIRING);
	if (fusion_platform)
		data &= ~MULTI_PIF;
	else
		data |= MULTI_PIF;
	if (data != orig)
		WREG32_PIF_PHY1(PB1_PIF_PAIRING, data);

	pcie_lc_cntl = pcie_lc_cntl_old = RREG32_PCIE_PORT(PCIE_LC_CNTL);
	pcie_lc_cntl &= ~(LC_L0S_INACTIVITY_MASK | LC_L1_INACTIVITY_MASK);
	if (!disable_l0s) {
		if (rdev->family >= CHIP_BARTS)
			pcie_lc_cntl |= LC_L0S_INACTIVITY(7);
		else
			pcie_lc_cntl |= LC_L0S_INACTIVITY(3);
	}

	if (!disable_l1) {
		if (rdev->family >= CHIP_BARTS)
			pcie_lc_cntl |= LC_L1_INACTIVITY(7);
		else
			pcie_lc_cntl |= LC_L1_INACTIVITY(8);

		if (!disable_plloff_in_l1) {
			data = orig = RREG32_PIF_PHY0(PB0_PIF_PWRDOWN_0);
			data &= ~(PLL_POWER_STATE_IN_OFF_0_MASK | PLL_POWER_STATE_IN_TXS2_0_MASK);
			data |= PLL_POWER_STATE_IN_OFF_0(7) | PLL_POWER_STATE_IN_TXS2_0(7);
			if (data != orig)
				WREG32_PIF_PHY0(PB0_PIF_PWRDOWN_0, data);

			data = orig = RREG32_PIF_PHY0(PB0_PIF_PWRDOWN_1);
			data &= ~(PLL_POWER_STATE_IN_OFF_1_MASK | PLL_POWER_STATE_IN_TXS2_1_MASK);
			data |= PLL_POWER_STATE_IN_OFF_1(7) | PLL_POWER_STATE_IN_TXS2_1(7);
			if (data != orig)
				WREG32_PIF_PHY0(PB0_PIF_PWRDOWN_1, data);

			data = orig = RREG32_PIF_PHY1(PB1_PIF_PWRDOWN_0);
			data &= ~(PLL_POWER_STATE_IN_OFF_0_MASK | PLL_POWER_STATE_IN_TXS2_0_MASK);
			data |= PLL_POWER_STATE_IN_OFF_0(7) | PLL_POWER_STATE_IN_TXS2_0(7);
			if (data != orig)
				WREG32_PIF_PHY1(PB1_PIF_PWRDOWN_0, data);

			data = orig = RREG32_PIF_PHY1(PB1_PIF_PWRDOWN_1);
			data &= ~(PLL_POWER_STATE_IN_OFF_1_MASK | PLL_POWER_STATE_IN_TXS2_1_MASK);
			data |= PLL_POWER_STATE_IN_OFF_1(7) | PLL_POWER_STATE_IN_TXS2_1(7);
			if (data != orig)
				WREG32_PIF_PHY1(PB1_PIF_PWRDOWN_1, data);

			if (rdev->family >= CHIP_BARTS) {
				data = orig = RREG32_PIF_PHY0(PB0_PIF_PWRDOWN_0);
				data &= ~PLL_RAMP_UP_TIME_0_MASK;
				data |= PLL_RAMP_UP_TIME_0(4);
				if (data != orig)
					WREG32_PIF_PHY0(PB0_PIF_PWRDOWN_0, data);

				data = orig = RREG32_PIF_PHY0(PB0_PIF_PWRDOWN_1);
				data &= ~PLL_RAMP_UP_TIME_1_MASK;
				data |= PLL_RAMP_UP_TIME_1(4);
				if (data != orig)
					WREG32_PIF_PHY0(PB0_PIF_PWRDOWN_1, data);

				data = orig = RREG32_PIF_PHY1(PB1_PIF_PWRDOWN_0);
				data &= ~PLL_RAMP_UP_TIME_0_MASK;
				data |= PLL_RAMP_UP_TIME_0(4);
				if (data != orig)
					WREG32_PIF_PHY1(PB1_PIF_PWRDOWN_0, data);

				data = orig = RREG32_PIF_PHY1(PB1_PIF_PWRDOWN_1);
				data &= ~PLL_RAMP_UP_TIME_1_MASK;
				data |= PLL_RAMP_UP_TIME_1(4);
				if (data != orig)
					WREG32_PIF_PHY1(PB1_PIF_PWRDOWN_1, data);
			}

			data = orig = RREG32_PCIE_PORT(PCIE_LC_LINK_WIDTH_CNTL);
			data &= ~LC_DYN_LANES_PWR_STATE_MASK;
			data |= LC_DYN_LANES_PWR_STATE(3);
			if (data != orig)
				WREG32_PCIE_PORT(PCIE_LC_LINK_WIDTH_CNTL, data);

			if (rdev->family >= CHIP_BARTS) {
				data = orig = RREG32_PIF_PHY0(PB0_PIF_CNTL);
				data &= ~LS2_EXIT_TIME_MASK;
				data |= LS2_EXIT_TIME(1);
				if (data != orig)
					WREG32_PIF_PHY0(PB0_PIF_CNTL, data);

				data = orig = RREG32_PIF_PHY1(PB1_PIF_CNTL);
				data &= ~LS2_EXIT_TIME_MASK;
				data |= LS2_EXIT_TIME(1);
				if (data != orig)
					WREG32_PIF_PHY1(PB1_PIF_CNTL, data);
			}
		}
	}

	/* evergreen parts only */
	if (rdev->family < CHIP_BARTS)
		pcie_lc_cntl |= LC_PMI_TO_L1_DIS;

	if (pcie_lc_cntl != pcie_lc_cntl_old)
		WREG32_PCIE_PORT(PCIE_LC_CNTL, pcie_lc_cntl);
>>>>>>> refs/remotes/origin/master
}
