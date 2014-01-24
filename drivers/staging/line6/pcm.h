/*
 * Line6 Linux USB driver - 0.9.1beta
 *
 * Copyright (C) 2004-2010 Markus Grabner (grabner@icg.tugraz.at)
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */

/*
	PCM interface to POD series devices.
*/

#ifndef PCM_H
#define PCM_H

#include <sound/pcm.h>

#include "driver.h"
#include "usbdefs.h"

/* number of URBs */
#define LINE6_ISO_BUFFERS	2

/*
	number of USB frames per URB
	The Line6 Windows driver always transmits two frames per packet, but
	the Linux driver performs significantly better (i.e., lower latency)
	with only one frame per packet.
*/
#define LINE6_ISO_PACKETS	1

/* in a "full speed" device (such as the PODxt Pro) this means 1ms */
#define LINE6_ISO_INTERVAL	1

#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
#define LINE6_IMPULSE_DEFAULT_PERIOD 100
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#define LINE6_BACKUP_MONITOR_SIGNAL 0
#define LINE6_REUSE_DMA_AREA_FOR_PLAYBACK 0

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
	Get substream from Line6 PCM data structure
*/
#define get_substream(line6pcm, stream)	\
		(line6pcm->pcm->streams[stream].substream)

/*
<<<<<<< HEAD
<<<<<<< HEAD
	PCM mode bits and masks.
	"ALSA": operations triggered by applications via ALSA
	"MONITOR": software monitoring
	"IMPULSE": optional impulse response operation
*/
enum {
	/* individual bits: */
	BIT_PCM_ALSA_PLAYBACK,
	BIT_PCM_ALSA_CAPTURE,
	BIT_PCM_MONITOR_PLAYBACK,
	BIT_PCM_MONITOR_CAPTURE,
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	BIT_PCM_IMPULSE_PLAYBACK,
	BIT_PCM_IMPULSE_CAPTURE,
#endif
	BIT_PAUSE_PLAYBACK,
	BIT_PREPARED,

	/* individual masks: */
/* *INDENT-OFF* */
	MASK_PCM_ALSA_PLAYBACK    = 1 << BIT_PCM_ALSA_PLAYBACK,
	MASK_PCM_ALSA_CAPTURE     = 1 << BIT_PCM_ALSA_CAPTURE,
	MASK_PCM_MONITOR_PLAYBACK = 1 << BIT_PCM_MONITOR_PLAYBACK,
	MASK_PCM_MONITOR_CAPTURE  = 1 << BIT_PCM_MONITOR_CAPTURE,
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	MASK_PCM_IMPULSE_PLAYBACK = 1 << BIT_PCM_IMPULSE_PLAYBACK,
	MASK_PCM_IMPULSE_CAPTURE  = 1 << BIT_PCM_IMPULSE_CAPTURE,
#endif
	MASK_PAUSE_PLAYBACK       = 1 << BIT_PAUSE_PLAYBACK,
	MASK_PREPARED             = 1 << BIT_PREPARED,
/* *INDENT-ON* */

	/* combined masks (by operation): */
	MASK_PCM_ALSA = MASK_PCM_ALSA_PLAYBACK | MASK_PCM_ALSA_CAPTURE,
	MASK_PCM_MONITOR = MASK_PCM_MONITOR_PLAYBACK | MASK_PCM_MONITOR_CAPTURE,
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	MASK_PCM_IMPULSE = MASK_PCM_IMPULSE_PLAYBACK | MASK_PCM_IMPULSE_CAPTURE,
#endif

	/* combined masks (by direction): */
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	MASK_PLAYBACK =
	    MASK_PCM_ALSA_PLAYBACK | MASK_PCM_MONITOR_PLAYBACK |
	    MASK_PCM_IMPULSE_PLAYBACK,
	MASK_CAPTURE =
	    MASK_PCM_ALSA_CAPTURE | MASK_PCM_MONITOR_CAPTURE |
	    MASK_PCM_IMPULSE_CAPTURE
#else
	MASK_PLAYBACK = MASK_PCM_ALSA_PLAYBACK | MASK_PCM_MONITOR_PLAYBACK,
	MASK_CAPTURE = MASK_PCM_ALSA_CAPTURE | MASK_PCM_MONITOR_CAPTURE
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
	PCM mode bits.

	There are several features of the Line6 USB driver which require PCM
	data to be exchanged with the device:
	*) PCM playback and capture via ALSA
	*) software monitoring (for devices without hardware monitoring)
	*) optional impulse response measurement
	However, from the device's point of view, there is just a single
	capture and playback stream, which must be shared between these
	subsystems. It is therefore necessary to maintain the state of the
	subsystems with respect to PCM usage. We define several constants of
	the form LINE6_BIT_PCM_<subsystem>_<direction>_<resource> with the
	following meanings:
	*) <subsystem> is one of
	-) ALSA: PCM playback and capture via ALSA
	-) MONITOR: software monitoring
	-) IMPULSE: optional impulse response measurement
	*) <direction> is one of
	-) PLAYBACK: audio output (from host to device)
	-) CAPTURE: audio input (from device to host)
	*) <resource> is one of
	-) BUFFER: buffer required by PCM data stream
	-) STREAM: actual PCM data stream

	The subsystems call line6_pcm_acquire() to acquire the (shared)
	resources needed for a particular operation (e.g., allocate the buffer
	for ALSA playback or start the capture stream for software monitoring).
	When a resource is no longer needed, it is released by calling
	line6_pcm_release(). Buffer allocation and stream startup are handled
	separately to allow the ALSA kernel driver to perform them at
	appropriate places (since the callback which starts a PCM stream is not
	allowed to sleep).
*/
enum {
	/* individual bit indices: */
	LINE6_INDEX_PCM_ALSA_PLAYBACK_BUFFER,
	LINE6_INDEX_PCM_ALSA_PLAYBACK_STREAM,
	LINE6_INDEX_PCM_ALSA_CAPTURE_BUFFER,
	LINE6_INDEX_PCM_ALSA_CAPTURE_STREAM,
	LINE6_INDEX_PCM_MONITOR_PLAYBACK_BUFFER,
	LINE6_INDEX_PCM_MONITOR_PLAYBACK_STREAM,
	LINE6_INDEX_PCM_MONITOR_CAPTURE_BUFFER,
	LINE6_INDEX_PCM_MONITOR_CAPTURE_STREAM,
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	LINE6_INDEX_PCM_IMPULSE_PLAYBACK_BUFFER,
	LINE6_INDEX_PCM_IMPULSE_PLAYBACK_STREAM,
	LINE6_INDEX_PCM_IMPULSE_CAPTURE_BUFFER,
	LINE6_INDEX_PCM_IMPULSE_CAPTURE_STREAM,
#endif
	LINE6_INDEX_PAUSE_PLAYBACK,
	LINE6_INDEX_PREPARED,

	/* individual bit masks: */
	LINE6_BIT(PCM_ALSA_PLAYBACK_BUFFER),
	LINE6_BIT(PCM_ALSA_PLAYBACK_STREAM),
	LINE6_BIT(PCM_ALSA_CAPTURE_BUFFER),
	LINE6_BIT(PCM_ALSA_CAPTURE_STREAM),
	LINE6_BIT(PCM_MONITOR_PLAYBACK_BUFFER),
	LINE6_BIT(PCM_MONITOR_PLAYBACK_STREAM),
	LINE6_BIT(PCM_MONITOR_CAPTURE_BUFFER),
	LINE6_BIT(PCM_MONITOR_CAPTURE_STREAM),
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	LINE6_BIT(PCM_IMPULSE_PLAYBACK_BUFFER),
	LINE6_BIT(PCM_IMPULSE_PLAYBACK_STREAM),
	LINE6_BIT(PCM_IMPULSE_CAPTURE_BUFFER),
	LINE6_BIT(PCM_IMPULSE_CAPTURE_STREAM),
#endif
	LINE6_BIT(PAUSE_PLAYBACK),
	LINE6_BIT(PREPARED),

	/* combined bit masks (by operation): */
	LINE6_BITS_PCM_ALSA_BUFFER =
	    LINE6_BIT_PCM_ALSA_PLAYBACK_BUFFER |
	    LINE6_BIT_PCM_ALSA_CAPTURE_BUFFER,

	LINE6_BITS_PCM_ALSA_STREAM =
	    LINE6_BIT_PCM_ALSA_PLAYBACK_STREAM |
	    LINE6_BIT_PCM_ALSA_CAPTURE_STREAM,

	LINE6_BITS_PCM_MONITOR =
	    LINE6_BIT_PCM_MONITOR_PLAYBACK_BUFFER |
	    LINE6_BIT_PCM_MONITOR_PLAYBACK_STREAM |
	    LINE6_BIT_PCM_MONITOR_CAPTURE_BUFFER |
	    LINE6_BIT_PCM_MONITOR_CAPTURE_STREAM,

#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	LINE6_BITS_PCM_IMPULSE =
	    LINE6_BIT_PCM_IMPULSE_PLAYBACK_BUFFER |
	    LINE6_BIT_PCM_IMPULSE_PLAYBACK_STREAM |
	    LINE6_BIT_PCM_IMPULSE_CAPTURE_BUFFER |
	    LINE6_BIT_PCM_IMPULSE_CAPTURE_STREAM,
#endif

	/* combined bit masks (by direction): */
	LINE6_BITS_PLAYBACK_BUFFER =
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	    LINE6_BIT_PCM_IMPULSE_PLAYBACK_BUFFER |
#endif
	    LINE6_BIT_PCM_ALSA_PLAYBACK_BUFFER |
	    LINE6_BIT_PCM_MONITOR_PLAYBACK_BUFFER ,

	LINE6_BITS_PLAYBACK_STREAM =
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	    LINE6_BIT_PCM_IMPULSE_PLAYBACK_STREAM |
#endif
	    LINE6_BIT_PCM_ALSA_PLAYBACK_STREAM |
	    LINE6_BIT_PCM_MONITOR_PLAYBACK_STREAM ,

	LINE6_BITS_CAPTURE_BUFFER =
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	    LINE6_BIT_PCM_IMPULSE_CAPTURE_BUFFER |
#endif
	    LINE6_BIT_PCM_ALSA_CAPTURE_BUFFER |
	    LINE6_BIT_PCM_MONITOR_CAPTURE_BUFFER ,

	LINE6_BITS_CAPTURE_STREAM =
#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	    LINE6_BIT_PCM_IMPULSE_CAPTURE_STREAM |
#endif
	    LINE6_BIT_PCM_ALSA_CAPTURE_STREAM |
	    LINE6_BIT_PCM_MONITOR_CAPTURE_STREAM,
<<<<<<< HEAD
	
	LINE6_BITS_STREAM =
	    LINE6_BITS_PLAYBACK_STREAM |
	    LINE6_BITS_CAPTURE_STREAM
>>>>>>> refs/remotes/origin/cm-10.0
=======

	LINE6_BITS_STREAM =
	    LINE6_BITS_PLAYBACK_STREAM |
	    LINE6_BITS_CAPTURE_STREAM
>>>>>>> refs/remotes/origin/master
};

struct line6_pcm_properties {
	struct snd_pcm_hardware snd_line6_playback_hw, snd_line6_capture_hw;
	struct snd_pcm_hw_constraint_ratdens snd_line6_rates;
	int bytes_per_frame;
};

struct snd_line6_pcm {
	/**
		 Pointer back to the Line6 driver data structure.
	*/
	struct usb_line6 *line6;

	/**
		 Properties.
	*/
	struct line6_pcm_properties *properties;

	/**
		 ALSA pcm stream
	*/
	struct snd_pcm *pcm;

	/**
		 URBs for audio playback.
	*/
	struct urb *urb_audio_out[LINE6_ISO_BUFFERS];

	/**
		 URBs for audio capture.
	*/
	struct urb *urb_audio_in[LINE6_ISO_BUFFERS];

	/**
		 Temporary buffer for playback.
		 Since the packet size is not known in advance, this buffer is
		 large enough to store maximum size packets.
	*/
	unsigned char *buffer_out;

	/**
		 Temporary buffer for capture.
		 Since the packet size is not known in advance, this buffer is
		 large enough to store maximum size packets.
	*/
	unsigned char *buffer_in;

	/**
<<<<<<< HEAD
<<<<<<< HEAD
		 Temporary buffer index for playback.
	*/
	int index_out;

	/**
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		 Previously captured frame (for software monitoring).
	*/
	unsigned char *prev_fbuf;

	/**
		 Size of previously captured frame (for software monitoring).
	*/
	int prev_fsize;

	/**
		 Free frame position in the playback buffer.
	*/
	snd_pcm_uframes_t pos_out;

	/**
		 Count processed bytes for playback.
		 This is modulo period size (to determine when a period is
		 finished).
	*/
	unsigned bytes_out;

	/**
		 Counter to create desired playback sample rate.
	*/
	unsigned count_out;

	/**
		 Playback period size in bytes
	*/
	unsigned period_out;

	/**
		 Processed frame position in the playback buffer.
		 The contents of the output ring buffer have been consumed by
		 the USB subsystem (i.e., sent to the USB device) up to this
		 position.
	*/
	snd_pcm_uframes_t pos_out_done;

	/**
		 Count processed bytes for capture.
		 This is modulo period size (to determine when a period is
		 finished).
	*/
	unsigned bytes_in;

	/**
		 Counter to create desired capture sample rate.
	*/
	unsigned count_in;

	/**
		 Capture period size in bytes
	*/
	unsigned period_in;

	/**
		 Processed frame position in the capture buffer.
		 The contents of the output ring buffer have been consumed by
		 the USB subsystem (i.e., sent to the USB device) up to this
		 position.
	*/
	snd_pcm_uframes_t pos_in_done;

	/**
		 Bit mask of active playback URBs.
	*/
	unsigned long active_urb_out;

	/**
		 Maximum size of USB packet.
	*/
	int max_packet_size;

	/**
		 USB endpoint for listening to audio data.
	*/
	int ep_audio_read;

	/**
		 USB endpoint for writing audio data.
	*/
	int ep_audio_write;

	/**
		 Bit mask of active capture URBs.
	*/
	unsigned long active_urb_in;

	/**
		 Bit mask of playback URBs currently being unlinked.
	*/
	unsigned long unlink_urb_out;

	/**
		 Bit mask of capture URBs currently being unlinked.
	*/
	unsigned long unlink_urb_in;

	/**
		 Spin lock to protect updates of the playback buffer positions (not
		 contents!)
	*/
	spinlock_t lock_audio_out;

	/**
		 Spin lock to protect updates of the capture buffer positions (not
		 contents!)
	*/
	spinlock_t lock_audio_in;

	/**
		 Spin lock to protect trigger.
	*/
	spinlock_t lock_trigger;

	/**
		 PCM playback volume (left and right).
	*/
	int volume_playback[2];

	/**
		 PCM monitor volume.
	*/
	int volume_monitor;

#ifdef CONFIG_LINE6_USB_IMPULSE_RESPONSE
	/**
		 Volume of impulse response test signal (if zero, test is disabled).
	*/
	int impulse_volume;

	/**
		 Period of impulse response test signal.
	*/
	int impulse_period;

	/**
		 Counter for impulse response test signal.
	*/
	int impulse_count;
#endif

	/**
<<<<<<< HEAD
<<<<<<< HEAD
		 Several status bits (see BIT_*).
=======
		 Several status bits (see LINE6_BIT_*).
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 Several status bits (see LINE6_BIT_*).
>>>>>>> refs/remotes/origin/master
	*/
	unsigned long flags;

	int last_frame_in, last_frame_out;
};

extern int line6_init_pcm(struct usb_line6 *line6,
			  struct line6_pcm_properties *properties);
extern int snd_line6_trigger(struct snd_pcm_substream *substream, int cmd);
extern int snd_line6_prepare(struct snd_pcm_substream *substream);
extern void line6_pcm_disconnect(struct snd_line6_pcm *line6pcm);
<<<<<<< HEAD
<<<<<<< HEAD
extern int line6_pcm_start(struct snd_line6_pcm *line6pcm, int channels);
extern int line6_pcm_stop(struct snd_line6_pcm *line6pcm, int channels);

#define PRINT_FRAME_DIFF(op) {						\
	static int diff_prev = 1000;					\
	int diff = line6pcm->last_frame_out - line6pcm->last_frame_in;	\
	if ((diff != diff_prev) && (abs(diff) < 100)) {			\
		printk(KERN_INFO "%s frame diff = %d\n", op, diff);	\
		diff_prev = diff;					\
	}								\
}
=======
extern int line6_pcm_acquire(struct snd_line6_pcm *line6pcm, int channels);
extern int line6_pcm_release(struct snd_line6_pcm *line6pcm, int channels);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int line6_pcm_acquire(struct snd_line6_pcm *line6pcm, int channels);
extern int line6_pcm_release(struct snd_line6_pcm *line6pcm, int channels);
>>>>>>> refs/remotes/origin/master

#endif
