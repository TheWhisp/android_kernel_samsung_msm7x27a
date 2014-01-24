<<<<<<< HEAD
/* Copyright (c) 2010, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
*/
#include <linux/msm_audio.h>
<<<<<<< HEAD
=======
#include "q6audio_common.h"
>>>>>>> refs/remotes/origin/cm-10.0

#define FRAME_NUM	(8)

#define PCM_BUF_COUNT		(2)

#define AUD_EOS_SET  0x01
#define TUNNEL_MODE     0x0000
#define NON_TUNNEL_MODE 0x0001

#define NO_BUF_ALLOC	0x00
#define BUF_ALLOC_IN    0x01
#define BUF_ALLOC_OUT   0x02
#define BUF_ALLOC_INOUT 0x03
#define ALIGN_BUF_SIZE(size) ((size + 4095) & (~4095))

<<<<<<< HEAD
struct timestamp{
=======
struct timestamp {
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long lowpart;
	unsigned long highpart;
} __attribute__ ((packed));

<<<<<<< HEAD
struct meta_in{
=======
struct meta_in {
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned short offset;
	struct timestamp ntimestamp;
	unsigned int nflags;
} __attribute__ ((packed));

<<<<<<< HEAD
struct meta_out_dsp{
=======
struct meta_out_dsp {
>>>>>>> refs/remotes/origin/cm-10.0
	u32 offset_to_frame;
	u32 frame_size;
	u32 encoded_pcm_samples;
	u32 msw_ts;
	u32 lsw_ts;
	u32 nflags;
} __attribute__ ((packed));

<<<<<<< HEAD
struct meta_out{
=======
struct meta_out {
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned char num_of_frames;
	struct meta_out_dsp meta_out_dsp[];
} __attribute__ ((packed));

<<<<<<< HEAD
struct q6audio_in{
=======
struct q6audio_in {
>>>>>>> refs/remotes/origin/cm-10.0
	spinlock_t			dsp_lock;
	atomic_t			in_bytes;
	atomic_t			in_samples;

	struct mutex			lock;
	struct mutex			read_lock;
	struct mutex			write_lock;
	wait_queue_head_t		read_wait;
	wait_queue_head_t		write_wait;

	struct audio_client             *ac;
	struct msm_audio_stream_config  str_cfg;
	void				*enc_cfg;
	struct msm_audio_buf_cfg        buf_cfg;
	struct msm_audio_config		pcm_cfg;
	void				*codec_cfg;

	/* number of buffers available to read/write */
	atomic_t			in_count;
	atomic_t			out_count;

	/* first idx: num of frames per buf, second idx: offset to frame */
	uint32_t			out_frame_info[FRAME_NUM][2];
	int				eos_rsp;
	int				opened;
	int				enabled;
	int				stopped;
	int				feedback; /* Flag indicates whether used
							in Non Tunnel mode */
	int				rflush;
	int				wflush;
	int				buf_alloc;
	uint16_t			min_frame_size;
	uint16_t			max_frames_per_buf;
	long (*enc_ioctl)(struct file *, unsigned int, unsigned long);
};

<<<<<<< HEAD
void  audio_in_get_dsp_frames(struct q6audio_in *audio,
		uint32_t token,	uint32_t *payload);
=======
>>>>>>> refs/remotes/origin/cm-10.0
int audio_in_enable(struct q6audio_in  *audio);
int audio_in_disable(struct q6audio_in  *audio);
int audio_in_buf_alloc(struct q6audio_in *audio);
long audio_in_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg);
ssize_t audio_in_read(struct file *file, char __user *buf,
		size_t count, loff_t *pos);
ssize_t audio_in_write(struct file *file, const char __user *buf,
		size_t count, loff_t *pos);
int audio_in_release(struct inode *inode, struct file *file);

