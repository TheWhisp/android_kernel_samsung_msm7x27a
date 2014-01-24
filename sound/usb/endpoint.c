/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/usb/audio.h>
#include <linux/usb/audio-v2.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/gfp.h>
#include <linux/init.h>
#include <linux/ratelimit.h>
#include <linux/usb.h>
#include <linux/usb/audio.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#include <sound/core.h>
#include <sound/pcm.h>

#include "usbaudio.h"
<<<<<<< HEAD
#include "card.h"
#include "proc.h"
#include "quirks.h"
#include "endpoint.h"
#include "urb.h"
#include "pcm.h"
#include "helper.h"
#include "format.h"
#include "clock.h"

/*
 * free a substream
 */
static void free_substream(struct snd_usb_substream *subs)
{
	struct list_head *p, *n;

	if (!subs->num_formats)
		return; /* not initialized */
	list_for_each_safe(p, n, &subs->fmt_list) {
		struct audioformat *fp = list_entry(p, struct audioformat, list);
		kfree(fp->rate_table);
		kfree(fp);
	}
	kfree(subs->rate_list.list);
}


/*
 * free a usb stream instance
 */
static void snd_usb_audio_stream_free(struct snd_usb_stream *stream)
{
	free_substream(&stream->substream[0]);
	free_substream(&stream->substream[1]);
	list_del(&stream->list);
	kfree(stream);
}

static void snd_usb_audio_pcm_free(struct snd_pcm *pcm)
{
	struct snd_usb_stream *stream = pcm->private_data;
	if (stream) {
		stream->pcm = NULL;
		snd_usb_audio_stream_free(stream);
	}
}


/*
 * add this endpoint to the chip instance.
 * if a stream with the same endpoint already exists, append to it.
 * if not, create a new pcm stream.
 */
int snd_usb_add_audio_endpoint(struct snd_usb_audio *chip, int stream, struct audioformat *fp)
{
	struct list_head *p;
	struct snd_usb_stream *as;
	struct snd_usb_substream *subs;
	struct snd_pcm *pcm;
	int err;

	list_for_each(p, &chip->pcm_list) {
		as = list_entry(p, struct snd_usb_stream, list);
		if (as->fmt_type != fp->fmt_type)
			continue;
		subs = &as->substream[stream];
		if (!subs->endpoint)
			continue;
		if (subs->endpoint == fp->endpoint) {
			list_add_tail(&fp->list, &subs->fmt_list);
			subs->num_formats++;
			subs->formats |= fp->formats;
			return 0;
		}
	}
	/* look for an empty stream */
	list_for_each(p, &chip->pcm_list) {
		as = list_entry(p, struct snd_usb_stream, list);
		if (as->fmt_type != fp->fmt_type)
			continue;
		subs = &as->substream[stream];
		if (subs->endpoint)
			continue;
		err = snd_pcm_new_stream(as->pcm, stream, 1);
		if (err < 0)
			return err;
		snd_usb_init_substream(as, stream, fp);
		return 0;
	}

	/* create a new pcm */
	as = kzalloc(sizeof(*as), GFP_KERNEL);
	if (!as)
		return -ENOMEM;
	as->pcm_index = chip->pcm_devs;
	as->chip = chip;
	as->fmt_type = fp->fmt_type;
	err = snd_pcm_new(chip->card, "USB Audio", chip->pcm_devs,
			  stream == SNDRV_PCM_STREAM_PLAYBACK ? 1 : 0,
			  stream == SNDRV_PCM_STREAM_PLAYBACK ? 0 : 1,
			  &pcm);
	if (err < 0) {
		kfree(as);
		return err;
	}
	as->pcm = pcm;
	pcm->private_data = as;
	pcm->private_free = snd_usb_audio_pcm_free;
	pcm->info_flags = 0;
	if (chip->pcm_devs > 0)
		sprintf(pcm->name, "USB Audio #%d", chip->pcm_devs);
	else
		strcpy(pcm->name, "USB Audio");

	snd_usb_init_substream(as, stream, fp);

	list_add(&as->list, &chip->pcm_list);
	chip->pcm_devs++;

	snd_usb_proc_pcm_format_add(as);

	return 0;
}

static int parse_uac_endpoint_attributes(struct snd_usb_audio *chip,
					 struct usb_host_interface *alts,
					 int protocol, int iface_no)
{
	/* parsed with a v1 header here. that's ok as we only look at the
	 * header first which is the same for both versions */
	struct uac_iso_endpoint_descriptor *csep;
	struct usb_interface_descriptor *altsd = get_iface_desc(alts);
	int attributes = 0;

	csep = snd_usb_find_desc(alts->endpoint[0].extra, alts->endpoint[0].extralen, NULL, USB_DT_CS_ENDPOINT);

	/* Creamware Noah has this descriptor after the 2nd endpoint */
	if (!csep && altsd->bNumEndpoints >= 2)
		csep = snd_usb_find_desc(alts->endpoint[1].extra, alts->endpoint[1].extralen, NULL, USB_DT_CS_ENDPOINT);

	if (!csep || csep->bLength < 7 ||
	    csep->bDescriptorSubtype != UAC_EP_GENERAL) {
		snd_printk(KERN_WARNING "%d:%u:%d : no or invalid"
			   " class specific endpoint descriptor\n",
			   chip->dev->devnum, iface_no,
			   altsd->bAlternateSetting);
		return 0;
	}

	if (protocol == UAC_VERSION_1) {
		attributes = csep->bmAttributes;
	} else {
		struct uac2_iso_endpoint_descriptor *csep2 =
			(struct uac2_iso_endpoint_descriptor *) csep;

		attributes = csep->bmAttributes & UAC_EP_CS_ATTR_FILL_MAX;

		/* emulate the endpoint attributes of a v1 device */
		if (csep2->bmControls & UAC2_CONTROL_PITCH)
			attributes |= UAC_EP_CS_ATTR_PITCH_CONTROL;
	}

	return attributes;
}

static struct uac2_input_terminal_descriptor *
	snd_usb_find_input_terminal_descriptor(struct usb_host_interface *ctrl_iface,
					       int terminal_id)
{
	struct uac2_input_terminal_descriptor *term = NULL;

	while ((term = snd_usb_find_csint_desc(ctrl_iface->extra,
					       ctrl_iface->extralen,
					       term, UAC_INPUT_TERMINAL))) {
		if (term->bTerminalID == terminal_id)
			return term;
	}

	return NULL;
}

static struct uac2_output_terminal_descriptor *
	snd_usb_find_output_terminal_descriptor(struct usb_host_interface *ctrl_iface,
						int terminal_id)
{
	struct uac2_output_terminal_descriptor *term = NULL;

	while ((term = snd_usb_find_csint_desc(ctrl_iface->extra,
					       ctrl_iface->extralen,
					       term, UAC_OUTPUT_TERMINAL))) {
		if (term->bTerminalID == terminal_id)
			return term;
	}

	return NULL;
}

int snd_usb_parse_audio_endpoints(struct snd_usb_audio *chip, int iface_no)
{
	struct usb_device *dev;
	struct usb_interface *iface;
	struct usb_host_interface *alts;
	struct usb_interface_descriptor *altsd;
	int i, altno, err, stream;
	int format = 0, num_channels = 0;
	struct audioformat *fp = NULL;
	int num, protocol, clock = 0;
	struct uac_format_type_i_continuous_descriptor *fmt;

	dev = chip->dev;

	/* parse the interface's altsettings */
	iface = usb_ifnum_to_if(dev, iface_no);

	num = iface->num_altsetting;

	/*
	 * Dallas DS4201 workaround: It presents 5 altsettings, but the last
	 * one misses syncpipe, and does not produce any sound.
	 */
	if (chip->usb_id == USB_ID(0x04fa, 0x4201))
		num = 4;

	for (i = 0; i < num; i++) {
		alts = &iface->altsetting[i];
		altsd = get_iface_desc(alts);
		protocol = altsd->bInterfaceProtocol;
		/* skip invalid one */
		if ((altsd->bInterfaceClass != USB_CLASS_AUDIO &&
		     altsd->bInterfaceClass != USB_CLASS_VENDOR_SPEC) ||
		    (altsd->bInterfaceSubClass != USB_SUBCLASS_AUDIOSTREAMING &&
		     altsd->bInterfaceSubClass != USB_SUBCLASS_VENDOR_SPEC) ||
		    altsd->bNumEndpoints < 1 ||
		    le16_to_cpu(get_endpoint(alts, 0)->wMaxPacketSize) == 0)
			continue;
		/* must be isochronous */
		if ((get_endpoint(alts, 0)->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) !=
		    USB_ENDPOINT_XFER_ISOC)
			continue;
		/* check direction */
		stream = (get_endpoint(alts, 0)->bEndpointAddress & USB_DIR_IN) ?
			SNDRV_PCM_STREAM_CAPTURE : SNDRV_PCM_STREAM_PLAYBACK;
		altno = altsd->bAlternateSetting;

		if (snd_usb_apply_interface_quirk(chip, iface_no, altno))
			continue;

		/* get audio formats */
		switch (protocol) {
		default:
			snd_printdd(KERN_WARNING "%d:%u:%d: unknown interface protocol %#02x, assuming v1\n",
				    dev->devnum, iface_no, altno, protocol);
			protocol = UAC_VERSION_1;
			/* fall through */

		case UAC_VERSION_1: {
			struct uac1_as_header_descriptor *as =
				snd_usb_find_csint_desc(alts->extra, alts->extralen, NULL, UAC_AS_GENERAL);

			if (!as) {
				snd_printk(KERN_ERR "%d:%u:%d : UAC_AS_GENERAL descriptor not found\n",
					   dev->devnum, iface_no, altno);
				continue;
			}

			if (as->bLength < sizeof(*as)) {
				snd_printk(KERN_ERR "%d:%u:%d : invalid UAC_AS_GENERAL desc\n",
					   dev->devnum, iface_no, altno);
				continue;
			}

			format = le16_to_cpu(as->wFormatTag); /* remember the format value */
			break;
		}

		case UAC_VERSION_2: {
			struct uac2_input_terminal_descriptor *input_term;
			struct uac2_output_terminal_descriptor *output_term;
			struct uac2_as_header_descriptor *as =
				snd_usb_find_csint_desc(alts->extra, alts->extralen, NULL, UAC_AS_GENERAL);

			if (!as) {
				snd_printk(KERN_ERR "%d:%u:%d : UAC_AS_GENERAL descriptor not found\n",
					   dev->devnum, iface_no, altno);
				continue;
			}

			if (as->bLength < sizeof(*as)) {
				snd_printk(KERN_ERR "%d:%u:%d : invalid UAC_AS_GENERAL desc\n",
					   dev->devnum, iface_no, altno);
				continue;
			}

			num_channels = as->bNrChannels;
			format = le32_to_cpu(as->bmFormats);

			/* lookup the terminal associated to this interface
			 * to extract the clock */
			input_term = snd_usb_find_input_terminal_descriptor(chip->ctrl_intf,
									    as->bTerminalLink);
			if (input_term) {
				clock = input_term->bCSourceID;
				break;
			}

			output_term = snd_usb_find_output_terminal_descriptor(chip->ctrl_intf,
									      as->bTerminalLink);
			if (output_term) {
				clock = output_term->bCSourceID;
				break;
			}

			snd_printk(KERN_ERR "%d:%u:%d : bogus bTerminalLink %d\n",
				   dev->devnum, iface_no, altno, as->bTerminalLink);
			continue;
		}
		}

		/* get format type */
		fmt = snd_usb_find_csint_desc(alts->extra, alts->extralen, NULL, UAC_FORMAT_TYPE);
		if (!fmt) {
			snd_printk(KERN_ERR "%d:%u:%d : no UAC_FORMAT_TYPE desc\n",
				   dev->devnum, iface_no, altno);
			continue;
		}
		if (((protocol == UAC_VERSION_1) && (fmt->bLength < 8)) ||
		    ((protocol == UAC_VERSION_2) && (fmt->bLength < 6))) {
			snd_printk(KERN_ERR "%d:%u:%d : invalid UAC_FORMAT_TYPE desc\n",
				   dev->devnum, iface_no, altno);
			continue;
		}

		/*
		 * Blue Microphones workaround: The last altsetting is identical
		 * with the previous one, except for a larger packet size, but
		 * is actually a mislabeled two-channel setting; ignore it.
		 */
		if (fmt->bNrChannels == 1 &&
		    fmt->bSubframeSize == 2 &&
		    altno == 2 && num == 3 &&
		    fp && fp->altsetting == 1 && fp->channels == 1 &&
		    fp->formats == SNDRV_PCM_FMTBIT_S16_LE &&
		    protocol == UAC_VERSION_1 &&
		    le16_to_cpu(get_endpoint(alts, 0)->wMaxPacketSize) ==
							fp->maxpacksize * 2)
			continue;

		fp = kzalloc(sizeof(*fp), GFP_KERNEL);
		if (! fp) {
			snd_printk(KERN_ERR "cannot malloc\n");
			return -ENOMEM;
		}

		fp->iface = iface_no;
		fp->altsetting = altno;
		fp->altset_idx = i;
		fp->endpoint = get_endpoint(alts, 0)->bEndpointAddress;
		fp->ep_attr = get_endpoint(alts, 0)->bmAttributes;
		fp->datainterval = snd_usb_parse_datainterval(chip, alts);
		fp->maxpacksize = le16_to_cpu(get_endpoint(alts, 0)->wMaxPacketSize);
		/* num_channels is only set for v2 interfaces */
		fp->channels = num_channels;
		if (snd_usb_get_speed(dev) == USB_SPEED_HIGH)
			fp->maxpacksize = (((fp->maxpacksize >> 11) & 3) + 1)
					* (fp->maxpacksize & 0x7ff);
		fp->attributes = parse_uac_endpoint_attributes(chip, alts, protocol, iface_no);
		fp->clock = clock;

		/* some quirks for attributes here */

		switch (chip->usb_id) {
		case USB_ID(0x0a92, 0x0053): /* AudioTrak Optoplay */
			/* Optoplay sets the sample rate attribute although
			 * it seems not supporting it in fact.
			 */
			fp->attributes &= ~UAC_EP_CS_ATTR_SAMPLE_RATE;
			break;
		case USB_ID(0x041e, 0x3020): /* Creative SB Audigy 2 NX */
		case USB_ID(0x0763, 0x2003): /* M-Audio Audiophile USB */
			/* doesn't set the sample rate attribute, but supports it */
			fp->attributes |= UAC_EP_CS_ATTR_SAMPLE_RATE;
			break;
		case USB_ID(0x047f, 0x0ca1): /* plantronics headset */
		case USB_ID(0x077d, 0x07af): /* Griffin iMic (note that there is
						an older model 77d:223) */
		/*
		 * plantronics headset and Griffin iMic have set adaptive-in
		 * although it's really not...
		 */
			fp->ep_attr &= ~USB_ENDPOINT_SYNCTYPE;
			if (stream == SNDRV_PCM_STREAM_PLAYBACK)
				fp->ep_attr |= USB_ENDPOINT_SYNC_ADAPTIVE;
			else
				fp->ep_attr |= USB_ENDPOINT_SYNC_SYNC;
			break;
		}

		/* ok, let's parse further... */
		if (snd_usb_parse_audio_format(chip, fp, format, fmt, stream, alts) < 0) {
			kfree(fp->rate_table);
			kfree(fp);
			fp = NULL;
			continue;
		}

		snd_printdd(KERN_INFO "%d:%u:%d: add audio endpoint %#x\n", dev->devnum, iface_no, altno, fp->endpoint);
		err = snd_usb_add_audio_endpoint(chip, stream, fp);
		if (err < 0) {
			kfree(fp->rate_table);
			kfree(fp);
			return err;
		}
		/* try to set the interface... */
		usb_set_interface(chip->dev, iface_no, altno);
		snd_usb_init_pitch(chip, iface_no, alts, fp);
		snd_usb_init_sample_rate(chip, iface_no, alts, fp, fp->rate_max);
	}
=======
#include "helper.h"
#include "card.h"
#include "endpoint.h"
#include "pcm.h"

/*
 * convert a sampling rate into our full speed format (fs/1000 in Q16.16)
 * this will overflow at approx 524 kHz
 */
static inline unsigned get_usb_full_speed_rate(unsigned int rate)
{
	return ((rate << 13) + 62) / 125;
}

/*
 * convert a sampling rate into USB high speed format (fs/8000 in Q16.16)
 * this will overflow at approx 4 MHz
 */
static inline unsigned get_usb_high_speed_rate(unsigned int rate)
{
	return ((rate << 10) + 62) / 125;
}

/*
 * unlink active urbs.
 */
static int deactivate_urbs(struct snd_usb_substream *subs, int force, int can_sleep)
{
	struct snd_usb_audio *chip = subs->stream->chip;
	unsigned int i;
	int async;

	subs->running = 0;

	if (!force && subs->stream->chip->shutdown) /* to be sure... */
		return -EBADFD;

	async = !can_sleep && chip->async_unlink;

	if (!async && in_interrupt())
		return 0;

	for (i = 0; i < subs->nurbs; i++) {
		if (test_bit(i, &subs->active_mask)) {
			if (!test_and_set_bit(i, &subs->unlink_mask)) {
				struct urb *u = subs->dataurb[i].urb;
				if (async)
					usb_unlink_urb(u);
				else
					usb_kill_urb(u);
			}
		}
	}
	if (subs->syncpipe) {
		for (i = 0; i < SYNC_URBS; i++) {
			if (test_bit(i+16, &subs->active_mask)) {
				if (!test_and_set_bit(i+16, &subs->unlink_mask)) {
					struct urb *u = subs->syncurb[i].urb;
					if (async)
						usb_unlink_urb(u);
					else
						usb_kill_urb(u);
				}
			}
		}
	}
	return 0;
}


/*
 * release a urb data
 */
static void release_urb_ctx(struct snd_urb_ctx *u)
{
	if (u->urb) {
		if (u->buffer_size)
			usb_free_coherent(u->subs->dev, u->buffer_size,
					u->urb->transfer_buffer,
					u->urb->transfer_dma);
		usb_free_urb(u->urb);
		u->urb = NULL;
	}
=======
#include <linux/slab.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>

#include "usbaudio.h"
#include "helper.h"
#include "card.h"
#include "endpoint.h"
#include "pcm.h"
#include "quirks.h"

#define EP_FLAG_RUNNING		1
#define EP_FLAG_STOPPING	2

/*
 * snd_usb_endpoint is a model that abstracts everything related to an
 * USB endpoint and its streaming.
 *
 * There are functions to activate and deactivate the streaming URBs and
 * optional callbacks to let the pcm logic handle the actual content of the
 * packets for playback and record. Thus, the bus streaming and the audio
 * handlers are fully decoupled.
 *
 * There are two different types of endpoints in audio applications.
 *
 * SND_USB_ENDPOINT_TYPE_DATA handles full audio data payload for both
 * inbound and outbound traffic.
 *
 * SND_USB_ENDPOINT_TYPE_SYNC endpoints are for inbound traffic only and
 * expect the payload to carry Q10.14 / Q16.16 formatted sync information
 * (3 or 4 bytes).
 *
 * Each endpoint has to be configured prior to being used by calling
 * snd_usb_endpoint_set_params().
 *
 * The model incorporates a reference counting, so that multiple users
 * can call snd_usb_endpoint_start() and snd_usb_endpoint_stop(), and
 * only the first user will effectively start the URBs, and only the last
 * one to stop it will tear the URBs down again.
 */

/*
 * convert a sampling rate into our full speed format (fs/1000 in Q16.16)
 * this will overflow at approx 524 kHz
 */
static inline unsigned get_usb_full_speed_rate(unsigned int rate)
{
	return ((rate << 13) + 62) / 125;
}

/*
 * convert a sampling rate into USB high speed format (fs/8000 in Q16.16)
 * this will overflow at approx 4 MHz
 */
static inline unsigned get_usb_high_speed_rate(unsigned int rate)
{
	return ((rate << 10) + 62) / 125;
}

/*
 * release a urb data
 */
static void release_urb_ctx(struct snd_urb_ctx *u)
{
	if (u->buffer_size)
		usb_free_coherent(u->ep->chip->dev, u->buffer_size,
				  u->urb->transfer_buffer,
				  u->urb->transfer_dma);
	usb_free_urb(u->urb);
	u->urb = NULL;
}

static const char *usb_error_string(int err)
{
	switch (err) {
	case -ENODEV:
		return "no device";
	case -ENOENT:
		return "endpoint not enabled";
	case -EPIPE:
		return "endpoint stalled";
	case -ENOSPC:
		return "not enough bandwidth";
	case -ESHUTDOWN:
		return "device disabled";
	case -EHOSTUNREACH:
		return "device suspended";
	case -EINVAL:
	case -EAGAIN:
	case -EFBIG:
	case -EMSGSIZE:
		return "internal error";
	default:
		return "unknown error";
	}
}

/**
 * snd_usb_endpoint_implicit_feedback_sink: Report endpoint usage type
 *
 * @ep: The snd_usb_endpoint
 *
 * Determine whether an endpoint is driven by an implicit feedback
 * data endpoint source.
 */
int snd_usb_endpoint_implicit_feedback_sink(struct snd_usb_endpoint *ep)
{
	return  ep->sync_master &&
		ep->sync_master->type == SND_USB_ENDPOINT_TYPE_DATA &&
		ep->type == SND_USB_ENDPOINT_TYPE_DATA &&
		usb_pipeout(ep->pipe);
}

/*
 * For streaming based on information derived from sync endpoints,
 * prepare_outbound_urb_sizes() will call next_packet_size() to
 * determine the number of samples to be sent in the next packet.
 *
 * For implicit feedback, next_packet_size() is unused.
 */
int snd_usb_endpoint_next_packet_size(struct snd_usb_endpoint *ep)
{
	unsigned long flags;
	int ret;

	if (ep->fill_max)
		return ep->maxframesize;

	spin_lock_irqsave(&ep->lock, flags);
	ep->phase = (ep->phase & 0xffff)
		+ (ep->freqm << ep->datainterval);
	ret = min(ep->phase >> 16, ep->maxframesize);
	spin_unlock_irqrestore(&ep->lock, flags);

	return ret;
}

static void retire_outbound_urb(struct snd_usb_endpoint *ep,
				struct snd_urb_ctx *urb_ctx)
{
	if (ep->retire_data_urb)
		ep->retire_data_urb(ep->data_subs, urb_ctx->urb);
}

static void retire_inbound_urb(struct snd_usb_endpoint *ep,
			       struct snd_urb_ctx *urb_ctx)
{
	struct urb *urb = urb_ctx->urb;

	if (unlikely(ep->skip_packets > 0)) {
		ep->skip_packets--;
		return;
	}

	if (ep->sync_slave)
		snd_usb_handle_sync_urb(ep->sync_slave, ep, urb);

	if (ep->retire_data_urb)
		ep->retire_data_urb(ep->data_subs, urb);
}

/*
 * Prepare a PLAYBACK urb for submission to the bus.
 */
static void prepare_outbound_urb(struct snd_usb_endpoint *ep,
				 struct snd_urb_ctx *ctx)
{
	int i;
	struct urb *urb = ctx->urb;
	unsigned char *cp = urb->transfer_buffer;

	urb->dev = ep->chip->dev; /* we need to set this at each time */

	switch (ep->type) {
	case SND_USB_ENDPOINT_TYPE_DATA:
		if (ep->prepare_data_urb) {
			ep->prepare_data_urb(ep->data_subs, urb);
		} else {
			/* no data provider, so send silence */
			unsigned int offs = 0;
			for (i = 0; i < ctx->packets; ++i) {
				int counts;

				if (ctx->packet_size[i])
					counts = ctx->packet_size[i];
				else
					counts = snd_usb_endpoint_next_packet_size(ep);

				urb->iso_frame_desc[i].offset = offs * ep->stride;
				urb->iso_frame_desc[i].length = counts * ep->stride;
				offs += counts;
			}

			urb->number_of_packets = ctx->packets;
			urb->transfer_buffer_length = offs * ep->stride;
			memset(urb->transfer_buffer, ep->silence_value,
			       offs * ep->stride);
		}
		break;

	case SND_USB_ENDPOINT_TYPE_SYNC:
		if (snd_usb_get_speed(ep->chip->dev) >= USB_SPEED_HIGH) {
			/*
			 * fill the length and offset of each urb descriptor.
			 * the fixed 12.13 frequency is passed as 16.16 through the pipe.
			 */
			urb->iso_frame_desc[0].length = 4;
			urb->iso_frame_desc[0].offset = 0;
			cp[0] = ep->freqn;
			cp[1] = ep->freqn >> 8;
			cp[2] = ep->freqn >> 16;
			cp[3] = ep->freqn >> 24;
		} else {
			/*
			 * fill the length and offset of each urb descriptor.
			 * the fixed 10.14 frequency is passed through the pipe.
			 */
			urb->iso_frame_desc[0].length = 3;
			urb->iso_frame_desc[0].offset = 0;
			cp[0] = ep->freqn >> 2;
			cp[1] = ep->freqn >> 10;
			cp[2] = ep->freqn >> 18;
		}

		break;
	}
}

/*
 * Prepare a CAPTURE or SYNC urb for submission to the bus.
 */
static inline void prepare_inbound_urb(struct snd_usb_endpoint *ep,
				       struct snd_urb_ctx *urb_ctx)
{
	int i, offs;
	struct urb *urb = urb_ctx->urb;

	urb->dev = ep->chip->dev; /* we need to set this at each time */

	switch (ep->type) {
	case SND_USB_ENDPOINT_TYPE_DATA:
		offs = 0;
		for (i = 0; i < urb_ctx->packets; i++) {
			urb->iso_frame_desc[i].offset = offs;
			urb->iso_frame_desc[i].length = ep->curpacksize;
			offs += ep->curpacksize;
		}

		urb->transfer_buffer_length = offs;
		urb->number_of_packets = urb_ctx->packets;
		break;

	case SND_USB_ENDPOINT_TYPE_SYNC:
		urb->iso_frame_desc[0].length = min(4u, ep->syncmaxsize);
		urb->iso_frame_desc[0].offset = 0;
		break;
	}
}

/*
 * Send output urbs that have been prepared previously. URBs are dequeued
 * from ep->ready_playback_urbs and in case there there aren't any available
 * or there are no packets that have been prepared, this function does
 * nothing.
 *
 * The reason why the functionality of sending and preparing URBs is separated
 * is that host controllers don't guarantee the order in which they return
 * inbound and outbound packets to their submitters.
 *
 * This function is only used for implicit feedback endpoints. For endpoints
 * driven by dedicated sync endpoints, URBs are immediately re-submitted
 * from their completion handler.
 */
static void queue_pending_output_urbs(struct snd_usb_endpoint *ep)
{
	while (test_bit(EP_FLAG_RUNNING, &ep->flags)) {

		unsigned long flags;
		struct snd_usb_packet_info *uninitialized_var(packet);
		struct snd_urb_ctx *ctx = NULL;
		struct urb *urb;
		int err, i;

		spin_lock_irqsave(&ep->lock, flags);
		if (ep->next_packet_read_pos != ep->next_packet_write_pos) {
			packet = ep->next_packet + ep->next_packet_read_pos;
			ep->next_packet_read_pos++;
			ep->next_packet_read_pos %= MAX_URBS;

			/* take URB out of FIFO */
			if (!list_empty(&ep->ready_playback_urbs))
				ctx = list_first_entry(&ep->ready_playback_urbs,
					       struct snd_urb_ctx, ready_list);
		}
		spin_unlock_irqrestore(&ep->lock, flags);

		if (ctx == NULL)
			return;

		list_del_init(&ctx->ready_list);
		urb = ctx->urb;

		/* copy over the length information */
		for (i = 0; i < packet->packets; i++)
			ctx->packet_size[i] = packet->packet_size[i];

		/* call the data handler to fill in playback data */
		prepare_outbound_urb(ep, ctx);

		err = usb_submit_urb(ctx->urb, GFP_ATOMIC);
		if (err < 0)
			snd_printk(KERN_ERR "Unable to submit urb #%d: %d (urb %p)\n",
				   ctx->index, err, ctx->urb);
		else
			set_bit(ctx->index, &ep->active_mask);
	}
}

/*
 * complete callback for urbs
 */
static void snd_complete_urb(struct urb *urb)
{
	struct snd_urb_ctx *ctx = urb->context;
	struct snd_usb_endpoint *ep = ctx->ep;
	int err;

	if (unlikely(urb->status == -ENOENT ||		/* unlinked */
		     urb->status == -ENODEV ||		/* device removed */
		     urb->status == -ECONNRESET ||	/* unlinked */
		     urb->status == -ESHUTDOWN ||	/* device disabled */
		     ep->chip->shutdown))		/* device disconnected */
		goto exit_clear;

	if (usb_pipeout(ep->pipe)) {
		retire_outbound_urb(ep, ctx);
		/* can be stopped during retire callback */
		if (unlikely(!test_bit(EP_FLAG_RUNNING, &ep->flags)))
			goto exit_clear;

		if (snd_usb_endpoint_implicit_feedback_sink(ep)) {
			unsigned long flags;

			spin_lock_irqsave(&ep->lock, flags);
			list_add_tail(&ctx->ready_list, &ep->ready_playback_urbs);
			spin_unlock_irqrestore(&ep->lock, flags);
			queue_pending_output_urbs(ep);

			goto exit_clear;
		}

		prepare_outbound_urb(ep, ctx);
	} else {
		retire_inbound_urb(ep, ctx);
		/* can be stopped during retire callback */
		if (unlikely(!test_bit(EP_FLAG_RUNNING, &ep->flags)))
			goto exit_clear;

		prepare_inbound_urb(ep, ctx);
	}

	err = usb_submit_urb(urb, GFP_ATOMIC);
	if (err == 0)
		return;

	snd_printk(KERN_ERR "cannot submit urb (err = %d)\n", err);
	//snd_pcm_stop(substream, SNDRV_PCM_STATE_XRUN);

exit_clear:
	clear_bit(ctx->index, &ep->active_mask);
}

/**
 * snd_usb_add_endpoint: Add an endpoint to an USB audio chip
 *
 * @chip: The chip
 * @alts: The USB host interface
 * @ep_num: The number of the endpoint to use
 * @direction: SNDRV_PCM_STREAM_PLAYBACK or SNDRV_PCM_STREAM_CAPTURE
 * @type: SND_USB_ENDPOINT_TYPE_DATA or SND_USB_ENDPOINT_TYPE_SYNC
 *
 * If the requested endpoint has not been added to the given chip before,
 * a new instance is created. Otherwise, a pointer to the previoulsy
 * created instance is returned. In case of any error, NULL is returned.
 *
 * New endpoints will be added to chip->ep_list and must be freed by
 * calling snd_usb_endpoint_free().
 */
struct snd_usb_endpoint *snd_usb_add_endpoint(struct snd_usb_audio *chip,
					      struct usb_host_interface *alts,
					      int ep_num, int direction, int type)
{
	struct snd_usb_endpoint *ep;
	int is_playback = direction == SNDRV_PCM_STREAM_PLAYBACK;

	if (WARN_ON(!alts))
		return NULL;

	mutex_lock(&chip->mutex);

	list_for_each_entry(ep, &chip->ep_list, list) {
		if (ep->ep_num == ep_num &&
		    ep->iface == alts->desc.bInterfaceNumber &&
		    ep->altsetting == alts->desc.bAlternateSetting) {
			snd_printdd(KERN_DEBUG "Re-using EP %x in iface %d,%d @%p\n",
					ep_num, ep->iface, ep->altsetting, ep);
			goto __exit_unlock;
		}
	}

	snd_printdd(KERN_DEBUG "Creating new %s %s endpoint #%x\n",
		    is_playback ? "playback" : "capture",
		    type == SND_USB_ENDPOINT_TYPE_DATA ? "data" : "sync",
		    ep_num);

	ep = kzalloc(sizeof(*ep), GFP_KERNEL);
	if (!ep)
		goto __exit_unlock;

	ep->chip = chip;
	spin_lock_init(&ep->lock);
	ep->type = type;
	ep->ep_num = ep_num;
	ep->iface = alts->desc.bInterfaceNumber;
	ep->altsetting = alts->desc.bAlternateSetting;
	INIT_LIST_HEAD(&ep->ready_playback_urbs);
	ep_num &= USB_ENDPOINT_NUMBER_MASK;

	if (is_playback)
		ep->pipe = usb_sndisocpipe(chip->dev, ep_num);
	else
		ep->pipe = usb_rcvisocpipe(chip->dev, ep_num);

	if (type == SND_USB_ENDPOINT_TYPE_SYNC) {
		if (get_endpoint(alts, 1)->bLength >= USB_DT_ENDPOINT_AUDIO_SIZE &&
		    get_endpoint(alts, 1)->bRefresh >= 1 &&
		    get_endpoint(alts, 1)->bRefresh <= 9)
			ep->syncinterval = get_endpoint(alts, 1)->bRefresh;
		else if (snd_usb_get_speed(chip->dev) == USB_SPEED_FULL)
			ep->syncinterval = 1;
		else if (get_endpoint(alts, 1)->bInterval >= 1 &&
			 get_endpoint(alts, 1)->bInterval <= 16)
			ep->syncinterval = get_endpoint(alts, 1)->bInterval - 1;
		else
			ep->syncinterval = 3;

		ep->syncmaxsize = le16_to_cpu(get_endpoint(alts, 1)->wMaxPacketSize);
	}

	list_add_tail(&ep->list, &chip->ep_list);

__exit_unlock:
	mutex_unlock(&chip->mutex);

	return ep;
>>>>>>> refs/remotes/origin/master
}

/*
 *  wait until all urbs are processed.
 */
<<<<<<< HEAD
static int wait_clear_urbs(struct snd_usb_substream *subs)
{
	unsigned long end_time = jiffies + msecs_to_jiffies(1000);
	unsigned int i;
	int alive;

	do {
		alive = 0;
		for (i = 0; i < subs->nurbs; i++) {
			if (test_bit(i, &subs->active_mask))
				alive++;
		}
		if (subs->syncpipe) {
			for (i = 0; i < SYNC_URBS; i++) {
				if (test_bit(i + 16, &subs->active_mask))
					alive++;
			}
		}
		if (! alive)
			break;
		schedule_timeout_uninterruptible(1);
	} while (time_before(jiffies, end_time));
	if (alive)
		snd_printk(KERN_ERR "timeout: still %d active urbs..\n", alive);
	return 0;
}

/*
 * release a substream
 */
void snd_usb_release_substream_urbs(struct snd_usb_substream *subs, int force)
{
	int i;

	/* stop urbs (to be sure) */
	deactivate_urbs(subs, force, 1);
	wait_clear_urbs(subs);

	for (i = 0; i < MAX_URBS; i++)
		release_urb_ctx(&subs->dataurb[i]);
	for (i = 0; i < SYNC_URBS; i++)
		release_urb_ctx(&subs->syncurb[i]);
	usb_free_coherent(subs->dev, SYNC_URBS * 4,
			subs->syncbuf, subs->sync_dma);
	subs->syncbuf = NULL;
	subs->nurbs = 0;
}

/*
 * complete callback from data urb
 */
static void snd_complete_urb(struct urb *urb)
{
	struct snd_urb_ctx *ctx = urb->context;
	struct snd_usb_substream *subs = ctx->subs;
	struct snd_pcm_substream *substream = ctx->subs->pcm_substream;
	int err = 0;

	if ((subs->running && subs->ops.retire(subs, substream->runtime, urb)) ||
	    !subs->running || /* can be stopped during retire callback */
	    (err = subs->ops.prepare(subs, substream->runtime, urb)) < 0 ||
	    (err = usb_submit_urb(urb, GFP_ATOMIC)) < 0) {
		clear_bit(ctx->index, &subs->active_mask);
		if (err < 0) {
			snd_printd(KERN_ERR "cannot submit urb (err = %d)\n", err);
			snd_pcm_stop(substream, SNDRV_PCM_STATE_XRUN);
		}
	}
}


/*
 * complete callback from sync urb
 */
static void snd_complete_sync_urb(struct urb *urb)
{
	struct snd_urb_ctx *ctx = urb->context;
	struct snd_usb_substream *subs = ctx->subs;
	struct snd_pcm_substream *substream = ctx->subs->pcm_substream;
	int err = 0;

	if ((subs->running && subs->ops.retire_sync(subs, substream->runtime, urb)) ||
	    !subs->running || /* can be stopped during retire callback */
	    (err = subs->ops.prepare_sync(subs, substream->runtime, urb)) < 0 ||
	    (err = usb_submit_urb(urb, GFP_ATOMIC)) < 0) {
		clear_bit(ctx->index + 16, &subs->active_mask);
		if (err < 0) {
			snd_printd(KERN_ERR "cannot submit sync urb (err = %d)\n", err);
			snd_pcm_stop(substream, SNDRV_PCM_STATE_XRUN);
		}
	}
}


/*
 * initialize a substream for plaback/capture
 */
int snd_usb_init_substream_urbs(struct snd_usb_substream *subs,
				unsigned int period_bytes,
				unsigned int rate,
				unsigned int frame_bits)
{
	unsigned int maxsize, i;
	int is_playback = subs->direction == SNDRV_PCM_STREAM_PLAYBACK;
	unsigned int urb_packs, total_packs, packs_per_ms;
	struct snd_usb_audio *chip = subs->stream->chip;

	/* calculate the frequency in 16.16 format */
	if (snd_usb_get_speed(subs->dev) == USB_SPEED_FULL)
		subs->freqn = get_usb_full_speed_rate(rate);
	else
		subs->freqn = get_usb_high_speed_rate(rate);
	subs->freqm = subs->freqn;
	subs->freqshift = INT_MIN;
	/* calculate max. frequency */
	if (subs->maxpacksize) {
		/* whatever fits into a max. size packet */
		maxsize = subs->maxpacksize;
		subs->freqmax = (maxsize / (frame_bits >> 3))
				<< (16 - subs->datainterval);
	} else {
		/* no max. packet size: just take 25% higher than nominal */
		subs->freqmax = subs->freqn + (subs->freqn >> 2);
		maxsize = ((subs->freqmax + 0xffff) * (frame_bits >> 3))
				>> (16 - subs->datainterval);
	}
	subs->phase = 0;

	if (subs->fill_max)
		subs->curpacksize = subs->maxpacksize;
	else
		subs->curpacksize = maxsize;

	if (snd_usb_get_speed(subs->dev) != USB_SPEED_FULL)
		packs_per_ms = 8 >> subs->datainterval;
	else
		packs_per_ms = 1;

	if (is_playback) {
		urb_packs = max(chip->nrpacks, 1);
		urb_packs = min(urb_packs, (unsigned int)MAX_PACKS);
	} else
		urb_packs = 1;
	urb_packs *= packs_per_ms;
	if (subs->syncpipe)
		urb_packs = min(urb_packs, 1U << subs->syncinterval);

	/* decide how many packets to be used */
	if (is_playback) {
		unsigned int minsize, maxpacks;
		/* determine how small a packet can be */
		minsize = (subs->freqn >> (16 - subs->datainterval))
			  * (frame_bits >> 3);
		/* with sync from device, assume it can be 12% lower */
		if (subs->syncpipe)
			minsize -= minsize >> 3;
		minsize = max(minsize, 1u);
		total_packs = (period_bytes + minsize - 1) / minsize;
		/* we need at least two URBs for queueing */
		if (total_packs < 2) {
			total_packs = 2;
		} else {
			/* and we don't want too long a queue either */
			maxpacks = max(MAX_QUEUE * packs_per_ms, urb_packs * 2);
			total_packs = min(total_packs, maxpacks);
		}
	} else {
		while (urb_packs > 1 && urb_packs * maxsize >= period_bytes)
			urb_packs >>= 1;
		total_packs = MAX_URBS * urb_packs;
	}
	subs->nurbs = (total_packs + urb_packs - 1) / urb_packs;
	if (subs->nurbs > MAX_URBS) {
		/* too much... */
		subs->nurbs = MAX_URBS;
		total_packs = MAX_URBS * urb_packs;
	} else if (subs->nurbs < 2) {
		/* too little - we need at least two packets
		 * to ensure contiguous playback/capture
		 */
		subs->nurbs = 2;
	}

	/* allocate and initialize data urbs */
	for (i = 0; i < subs->nurbs; i++) {
		struct snd_urb_ctx *u = &subs->dataurb[i];
		u->index = i;
		u->subs = subs;
		u->packets = (i + 1) * total_packs / subs->nurbs
			- i * total_packs / subs->nurbs;
		u->buffer_size = maxsize * u->packets;
		if (subs->fmt_type == UAC_FORMAT_TYPE_II)
=======
static int wait_clear_urbs(struct snd_usb_endpoint *ep)
{
	unsigned long end_time = jiffies + msecs_to_jiffies(1000);
	int alive;

	do {
		alive = bitmap_weight(&ep->active_mask, ep->nurbs);
		if (!alive)
			break;

		schedule_timeout_uninterruptible(1);
	} while (time_before(jiffies, end_time));

	if (alive)
		snd_printk(KERN_ERR "timeout: still %d active urbs on EP #%x\n",
					alive, ep->ep_num);
	clear_bit(EP_FLAG_STOPPING, &ep->flags);

	return 0;
}

/* sync the pending stop operation;
 * this function itself doesn't trigger the stop operation
 */
void snd_usb_endpoint_sync_pending_stop(struct snd_usb_endpoint *ep)
{
	if (ep && test_bit(EP_FLAG_STOPPING, &ep->flags))
		wait_clear_urbs(ep);
}

/*
 * unlink active urbs.
 */
static int deactivate_urbs(struct snd_usb_endpoint *ep, bool force)
{
	unsigned int i;

	if (!force && ep->chip->shutdown) /* to be sure... */
		return -EBADFD;

	clear_bit(EP_FLAG_RUNNING, &ep->flags);

	INIT_LIST_HEAD(&ep->ready_playback_urbs);
	ep->next_packet_read_pos = 0;
	ep->next_packet_write_pos = 0;

	for (i = 0; i < ep->nurbs; i++) {
		if (test_bit(i, &ep->active_mask)) {
			if (!test_and_set_bit(i, &ep->unlink_mask)) {
				struct urb *u = ep->urb[i].urb;
				usb_unlink_urb(u);
			}
		}
	}

	return 0;
}

/*
 * release an endpoint's urbs
 */
static void release_urbs(struct snd_usb_endpoint *ep, int force)
{
	int i;

	/* route incoming urbs to nirvana */
	ep->retire_data_urb = NULL;
	ep->prepare_data_urb = NULL;

	/* stop urbs */
	deactivate_urbs(ep, force);
	wait_clear_urbs(ep);

	for (i = 0; i < ep->nurbs; i++)
		release_urb_ctx(&ep->urb[i]);

	if (ep->syncbuf)
		usb_free_coherent(ep->chip->dev, SYNC_URBS * 4,
				  ep->syncbuf, ep->sync_dma);

	ep->syncbuf = NULL;
	ep->nurbs = 0;
}

/*
 * configure a data endpoint
 */
static int data_ep_set_params(struct snd_usb_endpoint *ep,
			      snd_pcm_format_t pcm_format,
			      unsigned int channels,
			      unsigned int period_bytes,
			      unsigned int frames_per_period,
			      unsigned int periods_per_buffer,
			      struct audioformat *fmt,
			      struct snd_usb_endpoint *sync_ep)
{
	unsigned int maxsize, minsize, packs_per_ms, max_packs_per_urb;
	unsigned int max_packs_per_period, urbs_per_period, urb_packs;
	unsigned int max_urbs, i;
	int frame_bits = snd_pcm_format_physical_width(pcm_format) * channels;

	if (pcm_format == SNDRV_PCM_FORMAT_DSD_U16_LE && fmt->dsd_dop) {
		/*
		 * When operating in DSD DOP mode, the size of a sample frame
		 * in hardware differs from the actual physical format width
		 * because we need to make room for the DOP markers.
		 */
		frame_bits += channels << 3;
	}

	ep->datainterval = fmt->datainterval;
	ep->stride = frame_bits >> 3;
	ep->silence_value = pcm_format == SNDRV_PCM_FORMAT_U8 ? 0x80 : 0;

	/* assume max. frequency is 25% higher than nominal */
	ep->freqmax = ep->freqn + (ep->freqn >> 2);
	maxsize = ((ep->freqmax + 0xffff) * (frame_bits >> 3))
				>> (16 - ep->datainterval);
	/* but wMaxPacketSize might reduce this */
	if (ep->maxpacksize && ep->maxpacksize < maxsize) {
		/* whatever fits into a max. size packet */
		maxsize = ep->maxpacksize;
		ep->freqmax = (maxsize / (frame_bits >> 3))
				<< (16 - ep->datainterval);
	}

	if (ep->fill_max)
		ep->curpacksize = ep->maxpacksize;
	else
		ep->curpacksize = maxsize;

	if (snd_usb_get_speed(ep->chip->dev) != USB_SPEED_FULL) {
		packs_per_ms = 8 >> ep->datainterval;
		max_packs_per_urb = MAX_PACKS_HS;
	} else {
		packs_per_ms = 1;
		max_packs_per_urb = MAX_PACKS;
	}
	if (sync_ep && !snd_usb_endpoint_implicit_feedback_sink(ep))
		max_packs_per_urb = min(max_packs_per_urb,
					1U << sync_ep->syncinterval);
	max_packs_per_urb = max(1u, max_packs_per_urb >> ep->datainterval);

	/*
	 * Capture endpoints need to use small URBs because there's no way
	 * to tell in advance where the next period will end, and we don't
	 * want the next URB to complete much after the period ends.
	 *
	 * Playback endpoints with implicit sync much use the same parameters
	 * as their corresponding capture endpoint.
	 */
	if (usb_pipein(ep->pipe) ||
			snd_usb_endpoint_implicit_feedback_sink(ep)) {

		urb_packs = packs_per_ms;
		/*
		 * Wireless devices can poll at a max rate of once per 4ms.
		 * For dataintervals less than 5, increase the packet count to
		 * allow the host controller to use bursting to fill in the
		 * gaps.
		 */
		if (snd_usb_get_speed(ep->chip->dev) == USB_SPEED_WIRELESS) {
			int interval = ep->datainterval;
			while (interval < 5) {
				urb_packs <<= 1;
				++interval;
			}
		}
		/* make capture URBs <= 1 ms and smaller than a period */
		urb_packs = min(max_packs_per_urb, urb_packs);
		while (urb_packs > 1 && urb_packs * maxsize >= period_bytes)
			urb_packs >>= 1;
		ep->nurbs = MAX_URBS;

	/*
	 * Playback endpoints without implicit sync are adjusted so that
	 * a period fits as evenly as possible in the smallest number of
	 * URBs.  The total number of URBs is adjusted to the size of the
	 * ALSA buffer, subject to the MAX_URBS and MAX_QUEUE limits.
	 */
	} else {
		/* determine how small a packet can be */
		minsize = (ep->freqn >> (16 - ep->datainterval)) *
				(frame_bits >> 3);
		/* with sync from device, assume it can be 12% lower */
		if (sync_ep)
			minsize -= minsize >> 3;
		minsize = max(minsize, 1u);

		/* how many packets will contain an entire ALSA period? */
		max_packs_per_period = DIV_ROUND_UP(period_bytes, minsize);

		/* how many URBs will contain a period? */
		urbs_per_period = DIV_ROUND_UP(max_packs_per_period,
				max_packs_per_urb);
		/* how many packets are needed in each URB? */
		urb_packs = DIV_ROUND_UP(max_packs_per_period, urbs_per_period);

		/* limit the number of frames in a single URB */
		ep->max_urb_frames = DIV_ROUND_UP(frames_per_period,
					urbs_per_period);

		/* try to use enough URBs to contain an entire ALSA buffer */
		max_urbs = min((unsigned) MAX_URBS,
				MAX_QUEUE * packs_per_ms / urb_packs);
		ep->nurbs = min(max_urbs, urbs_per_period * periods_per_buffer);
	}

	/* allocate and initialize data urbs */
	for (i = 0; i < ep->nurbs; i++) {
		struct snd_urb_ctx *u = &ep->urb[i];
		u->index = i;
		u->ep = ep;
		u->packets = urb_packs;
		u->buffer_size = maxsize * u->packets;

		if (fmt->fmt_type == UAC_FORMAT_TYPE_II)
>>>>>>> refs/remotes/origin/master
			u->packets++; /* for transfer delimiter */
		u->urb = usb_alloc_urb(u->packets, GFP_KERNEL);
		if (!u->urb)
			goto out_of_memory;
<<<<<<< HEAD
		u->urb->transfer_buffer =
			usb_alloc_coherent(subs->dev, u->buffer_size,
					   GFP_KERNEL, &u->urb->transfer_dma);
		if (!u->urb->transfer_buffer)
			goto out_of_memory;
		u->urb->pipe = subs->datapipe;
		u->urb->transfer_flags = URB_ISO_ASAP | URB_NO_TRANSFER_DMA_MAP;
		u->urb->interval = 1 << subs->datainterval;
		u->urb->context = u;
		u->urb->complete = snd_complete_urb;
	}

	if (subs->syncpipe) {
		/* allocate and initialize sync urbs */
		subs->syncbuf = usb_alloc_coherent(subs->dev, SYNC_URBS * 4,
						 GFP_KERNEL, &subs->sync_dma);
		if (!subs->syncbuf)
			goto out_of_memory;
		for (i = 0; i < SYNC_URBS; i++) {
			struct snd_urb_ctx *u = &subs->syncurb[i];
			u->index = i;
			u->subs = subs;
			u->packets = 1;
			u->urb = usb_alloc_urb(1, GFP_KERNEL);
			if (!u->urb)
				goto out_of_memory;
			u->urb->transfer_buffer = subs->syncbuf + i * 4;
			u->urb->transfer_dma = subs->sync_dma + i * 4;
			u->urb->transfer_buffer_length = 4;
			u->urb->pipe = subs->syncpipe;
			u->urb->transfer_flags = URB_ISO_ASAP |
						 URB_NO_TRANSFER_DMA_MAP;
			u->urb->number_of_packets = 1;
			u->urb->interval = 1 << subs->syncinterval;
			u->urb->context = u;
			u->urb->complete = snd_complete_sync_urb;
		}
	}
	return 0;

out_of_memory:
	snd_usb_release_substream_urbs(subs, 0);
=======

		u->urb->transfer_buffer =
			usb_alloc_coherent(ep->chip->dev, u->buffer_size,
					   GFP_KERNEL, &u->urb->transfer_dma);
		if (!u->urb->transfer_buffer)
			goto out_of_memory;
		u->urb->pipe = ep->pipe;
		u->urb->transfer_flags = URB_NO_TRANSFER_DMA_MAP;
		u->urb->interval = 1 << ep->datainterval;
		u->urb->context = u;
		u->urb->complete = snd_complete_urb;
		INIT_LIST_HEAD(&u->ready_list);
	}

	return 0;

out_of_memory:
	release_urbs(ep, 0);
>>>>>>> refs/remotes/origin/master
	return -ENOMEM;
}

/*
<<<<<<< HEAD
 * prepare urb for full speed capture sync pipe
 *
 * fill the length and offset of each urb descriptor.
 * the fixed 10.14 frequency is passed through the pipe.
 */
static int prepare_capture_sync_urb(struct snd_usb_substream *subs,
				    struct snd_pcm_runtime *runtime,
				    struct urb *urb)
{
	unsigned char *cp = urb->transfer_buffer;
	struct snd_urb_ctx *ctx = urb->context;

	urb->dev = ctx->subs->dev; /* we need to set this at each time */
	urb->iso_frame_desc[0].length = 3;
	urb->iso_frame_desc[0].offset = 0;
	cp[0] = subs->freqn >> 2;
	cp[1] = subs->freqn >> 10;
	cp[2] = subs->freqn >> 18;
	return 0;
}

/*
 * prepare urb for high speed capture sync pipe
 *
 * fill the length and offset of each urb descriptor.
 * the fixed 12.13 frequency is passed as 16.16 through the pipe.
 */
static int prepare_capture_sync_urb_hs(struct snd_usb_substream *subs,
				       struct snd_pcm_runtime *runtime,
				       struct urb *urb)
{
	unsigned char *cp = urb->transfer_buffer;
	struct snd_urb_ctx *ctx = urb->context;

	urb->dev = ctx->subs->dev; /* we need to set this at each time */
	urb->iso_frame_desc[0].length = 4;
	urb->iso_frame_desc[0].offset = 0;
	cp[0] = subs->freqn;
	cp[1] = subs->freqn >> 8;
	cp[2] = subs->freqn >> 16;
	cp[3] = subs->freqn >> 24;
	return 0;
}

/*
 * process after capture sync complete
 * - nothing to do
 */
static int retire_capture_sync_urb(struct snd_usb_substream *subs,
				   struct snd_pcm_runtime *runtime,
				   struct urb *urb)
{
	return 0;
}

/*
 * prepare urb for capture data pipe
 *
 * fill the offset and length of each descriptor.
 *
 * we use a temporary buffer to write the captured data.
 * since the length of written data is determined by host, we cannot
 * write onto the pcm buffer directly...  the data is thus copied
 * later at complete callback to the global buffer.
 */
static int prepare_capture_urb(struct snd_usb_substream *subs,
			       struct snd_pcm_runtime *runtime,
			       struct urb *urb)
{
	int i, offs;
	struct snd_urb_ctx *ctx = urb->context;

	offs = 0;
	urb->dev = ctx->subs->dev; /* we need to set this at each time */
	for (i = 0; i < ctx->packets; i++) {
		urb->iso_frame_desc[i].offset = offs;
		urb->iso_frame_desc[i].length = subs->curpacksize;
		offs += subs->curpacksize;
	}
	urb->transfer_buffer_length = offs;
	urb->number_of_packets = ctx->packets;
	return 0;
}

/*
 * process after capture complete
 *
 * copy the data from each desctiptor to the pcm buffer, and
 * update the current position.
 */
static int retire_capture_urb(struct snd_usb_substream *subs,
			      struct snd_pcm_runtime *runtime,
			      struct urb *urb)
{
	unsigned long flags;
	unsigned char *cp;
	int i;
	unsigned int stride, frames, bytes, oldptr;
	int period_elapsed = 0;

	stride = runtime->frame_bits >> 3;

	for (i = 0; i < urb->number_of_packets; i++) {
		cp = (unsigned char *)urb->transfer_buffer + urb->iso_frame_desc[i].offset;
		if (urb->iso_frame_desc[i].status && printk_ratelimit()) {
			snd_printdd("frame %d active: %d\n", i, urb->iso_frame_desc[i].status);
			// continue;
		}
		bytes = urb->iso_frame_desc[i].actual_length;
		frames = bytes / stride;
		if (!subs->txfr_quirk)
			bytes = frames * stride;
		if (bytes % (runtime->sample_bits >> 3) != 0) {
#ifdef CONFIG_SND_DEBUG_VERBOSE
			int oldbytes = bytes;
#endif
			bytes = frames * stride;
			snd_printdd(KERN_ERR "Corrected urb data len. %d->%d\n",
							oldbytes, bytes);
		}
		/* update the current pointer */
		spin_lock_irqsave(&subs->lock, flags);
		oldptr = subs->hwptr_done;
		subs->hwptr_done += bytes;
		if (subs->hwptr_done >= runtime->buffer_size * stride)
			subs->hwptr_done -= runtime->buffer_size * stride;
		frames = (bytes + (oldptr % stride)) / stride;
		subs->transfer_done += frames;
		if (subs->transfer_done >= runtime->period_size) {
			subs->transfer_done -= runtime->period_size;
			period_elapsed = 1;
		}
		spin_unlock_irqrestore(&subs->lock, flags);
		/* copy a data chunk */
		if (oldptr + bytes > runtime->buffer_size * stride) {
			unsigned int bytes1 =
					runtime->buffer_size * stride - oldptr;
			memcpy(runtime->dma_area + oldptr, cp, bytes1);
			memcpy(runtime->dma_area, cp + bytes1, bytes - bytes1);
		} else {
			memcpy(runtime->dma_area + oldptr, cp, bytes);
		}
	}
	if (period_elapsed)
		snd_pcm_period_elapsed(subs->pcm_substream);
	return 0;
}

/*
 * Process after capture complete when paused.  Nothing to do.
 */
static int retire_paused_capture_urb(struct snd_usb_substream *subs,
				     struct snd_pcm_runtime *runtime,
				     struct urb *urb)
{
	return 0;
}


/*
 * prepare urb for playback sync pipe
 *
 * set up the offset and length to receive the current frequency.
 */
static int prepare_playback_sync_urb(struct snd_usb_substream *subs,
				     struct snd_pcm_runtime *runtime,
				     struct urb *urb)
{
	struct snd_urb_ctx *ctx = urb->context;

	urb->dev = ctx->subs->dev; /* we need to set this at each time */
	urb->iso_frame_desc[0].length = min(4u, ctx->subs->syncmaxsize);
	urb->iso_frame_desc[0].offset = 0;
	return 0;
}

/*
 * process after playback sync complete
 *
 * Full speed devices report feedback values in 10.14 format as samples per
 * frame, high speed devices in 16.16 format as samples per microframe.
 * Because the Audio Class 1 spec was written before USB 2.0, many high speed
 * devices use a wrong interpretation, some others use an entirely different
 * format.  Therefore, we cannot predict what format any particular device uses
 * and must detect it automatically.
 */
static int retire_playback_sync_urb(struct snd_usb_substream *subs,
				    struct snd_pcm_runtime *runtime,
				    struct urb *urb)
{
	unsigned int f;
	int shift;
	unsigned long flags;

	if (urb->iso_frame_desc[0].status != 0 ||
	    urb->iso_frame_desc[0].actual_length < 3)
		return 0;
=======
 * configure a sync endpoint
 */
static int sync_ep_set_params(struct snd_usb_endpoint *ep)
{
	int i;

	ep->syncbuf = usb_alloc_coherent(ep->chip->dev, SYNC_URBS * 4,
					 GFP_KERNEL, &ep->sync_dma);
	if (!ep->syncbuf)
		return -ENOMEM;

	for (i = 0; i < SYNC_URBS; i++) {
		struct snd_urb_ctx *u = &ep->urb[i];
		u->index = i;
		u->ep = ep;
		u->packets = 1;
		u->urb = usb_alloc_urb(1, GFP_KERNEL);
		if (!u->urb)
			goto out_of_memory;
		u->urb->transfer_buffer = ep->syncbuf + i * 4;
		u->urb->transfer_dma = ep->sync_dma + i * 4;
		u->urb->transfer_buffer_length = 4;
		u->urb->pipe = ep->pipe;
		u->urb->transfer_flags = URB_NO_TRANSFER_DMA_MAP;
		u->urb->number_of_packets = 1;
		u->urb->interval = 1 << ep->syncinterval;
		u->urb->context = u;
		u->urb->complete = snd_complete_urb;
	}

	ep->nurbs = SYNC_URBS;

	return 0;

out_of_memory:
	release_urbs(ep, 0);
	return -ENOMEM;
}

/**
 * snd_usb_endpoint_set_params: configure an snd_usb_endpoint
 *
 * @ep: the snd_usb_endpoint to configure
 * @pcm_format: the audio fomat.
 * @channels: the number of audio channels.
 * @period_bytes: the number of bytes in one alsa period.
 * @period_frames: the number of frames in one alsa period.
 * @buffer_periods: the number of periods in one alsa buffer.
 * @rate: the frame rate.
 * @fmt: the USB audio format information
 * @sync_ep: the sync endpoint to use, if any
 *
 * Determine the number of URBs to be used on this endpoint.
 * An endpoint must be configured before it can be started.
 * An endpoint that is already running can not be reconfigured.
 */
int snd_usb_endpoint_set_params(struct snd_usb_endpoint *ep,
				snd_pcm_format_t pcm_format,
				unsigned int channels,
				unsigned int period_bytes,
				unsigned int period_frames,
				unsigned int buffer_periods,
				unsigned int rate,
				struct audioformat *fmt,
				struct snd_usb_endpoint *sync_ep)
{
	int err;

	if (ep->use_count != 0) {
		snd_printk(KERN_WARNING "Unable to change format on ep #%x: already in use\n",
			   ep->ep_num);
		return -EBUSY;
	}

	/* release old buffers, if any */
	release_urbs(ep, 0);

	ep->datainterval = fmt->datainterval;
	ep->maxpacksize = fmt->maxpacksize;
	ep->fill_max = !!(fmt->attributes & UAC_EP_CS_ATTR_FILL_MAX);

	if (snd_usb_get_speed(ep->chip->dev) == USB_SPEED_FULL)
		ep->freqn = get_usb_full_speed_rate(rate);
	else
		ep->freqn = get_usb_high_speed_rate(rate);

	/* calculate the frequency in 16.16 format */
	ep->freqm = ep->freqn;
	ep->freqshift = INT_MIN;

	ep->phase = 0;

	switch (ep->type) {
	case  SND_USB_ENDPOINT_TYPE_DATA:
		err = data_ep_set_params(ep, pcm_format, channels,
					 period_bytes, period_frames,
					 buffer_periods, fmt, sync_ep);
		break;
	case  SND_USB_ENDPOINT_TYPE_SYNC:
		err = sync_ep_set_params(ep);
		break;
	default:
		err = -EINVAL;
	}

	snd_printdd(KERN_DEBUG "Setting params for ep #%x (type %d, %d urbs), ret=%d\n",
		   ep->ep_num, ep->type, ep->nurbs, err);

	return err;
}

/**
 * snd_usb_endpoint_start: start an snd_usb_endpoint
 *
 * @ep:		the endpoint to start
 * @can_sleep:	flag indicating whether the operation is executed in
 * 		non-atomic context
 *
 * A call to this function will increment the use count of the endpoint.
 * In case it is not already running, the URBs for this endpoint will be
 * submitted. Otherwise, this function does nothing.
 *
 * Must be balanced to calls of snd_usb_endpoint_stop().
 *
 * Returns an error if the URB submission failed, 0 in all other cases.
 */
int snd_usb_endpoint_start(struct snd_usb_endpoint *ep, bool can_sleep)
{
	int err;
	unsigned int i;

	if (ep->chip->shutdown)
		return -EBADFD;

	/* already running? */
	if (++ep->use_count != 1)
		return 0;

	/* just to be sure */
	deactivate_urbs(ep, false);
	if (can_sleep)
		wait_clear_urbs(ep);

	ep->active_mask = 0;
	ep->unlink_mask = 0;
	ep->phase = 0;

	snd_usb_endpoint_start_quirk(ep);

	/*
	 * If this endpoint has a data endpoint as implicit feedback source,
	 * don't start the urbs here. Instead, mark them all as available,
	 * wait for the record urbs to return and queue the playback urbs
	 * from that context.
	 */

	set_bit(EP_FLAG_RUNNING, &ep->flags);

	if (snd_usb_endpoint_implicit_feedback_sink(ep)) {
		for (i = 0; i < ep->nurbs; i++) {
			struct snd_urb_ctx *ctx = ep->urb + i;
			list_add_tail(&ctx->ready_list, &ep->ready_playback_urbs);
		}

		return 0;
	}

	for (i = 0; i < ep->nurbs; i++) {
		struct urb *urb = ep->urb[i].urb;

		if (snd_BUG_ON(!urb))
			goto __error;

		if (usb_pipeout(ep->pipe)) {
			prepare_outbound_urb(ep, urb->context);
		} else {
			prepare_inbound_urb(ep, urb->context);
		}

		err = usb_submit_urb(urb, GFP_ATOMIC);
		if (err < 0) {
			snd_printk(KERN_ERR "cannot submit urb %d, error %d: %s\n",
				   i, err, usb_error_string(err));
			goto __error;
		}
		set_bit(i, &ep->active_mask);
	}

	return 0;

__error:
	clear_bit(EP_FLAG_RUNNING, &ep->flags);
	ep->use_count--;
	deactivate_urbs(ep, false);
	return -EPIPE;
}

/**
 * snd_usb_endpoint_stop: stop an snd_usb_endpoint
 *
 * @ep: the endpoint to stop (may be NULL)
 *
 * A call to this function will decrement the use count of the endpoint.
 * In case the last user has requested the endpoint stop, the URBs will
 * actually be deactivated.
 *
 * Must be balanced to calls of snd_usb_endpoint_start().
 *
 * The caller needs to synchronize the pending stop operation via
 * snd_usb_endpoint_sync_pending_stop().
 */
void snd_usb_endpoint_stop(struct snd_usb_endpoint *ep)
{
	if (!ep)
		return;

	if (snd_BUG_ON(ep->use_count == 0))
		return;

	if (--ep->use_count == 0) {
		deactivate_urbs(ep, false);
		ep->data_subs = NULL;
		ep->sync_slave = NULL;
		ep->retire_data_urb = NULL;
		ep->prepare_data_urb = NULL;
		set_bit(EP_FLAG_STOPPING, &ep->flags);
	}
}

/**
 * snd_usb_endpoint_deactivate: deactivate an snd_usb_endpoint
 *
 * @ep: the endpoint to deactivate
 *
 * If the endpoint is not currently in use, this functions will
 * deactivate its associated URBs.
 *
 * In case of any active users, this functions does nothing.
 */
void snd_usb_endpoint_deactivate(struct snd_usb_endpoint *ep)
{
	if (!ep)
		return;

	if (ep->use_count != 0)
		return;

	deactivate_urbs(ep, true);
	wait_clear_urbs(ep);
}

/**
 * snd_usb_endpoint_free: Free the resources of an snd_usb_endpoint
 *
 * @ep: the list header of the endpoint to free
 *
 * This function does not care for the endpoint's use count but will tear
 * down all the streaming URBs immediately and free all resources.
 */
void snd_usb_endpoint_free(struct list_head *head)
{
	struct snd_usb_endpoint *ep;

	ep = list_entry(head, struct snd_usb_endpoint, list);
	release_urbs(ep, 1);
	kfree(ep);
}

/**
 * snd_usb_handle_sync_urb: parse an USB sync packet
 *
 * @ep: the endpoint to handle the packet
 * @sender: the sending endpoint
 * @urb: the received packet
 *
 * This function is called from the context of an endpoint that received
 * the packet and is used to let another endpoint object handle the payload.
 */
void snd_usb_handle_sync_urb(struct snd_usb_endpoint *ep,
			     struct snd_usb_endpoint *sender,
			     const struct urb *urb)
{
	int shift;
	unsigned int f;
	unsigned long flags;

	snd_BUG_ON(ep == sender);

	/*
	 * In case the endpoint is operating in implicit feedback mode, prepare
	 * a new outbound URB that has the same layout as the received packet
	 * and add it to the list of pending urbs. queue_pending_output_urbs()
	 * will take care of them later.
	 */
	if (snd_usb_endpoint_implicit_feedback_sink(ep) &&
	    ep->use_count != 0) {

		/* implicit feedback case */
		int i, bytes = 0;
		struct snd_urb_ctx *in_ctx;
		struct snd_usb_packet_info *out_packet;

		in_ctx = urb->context;

		/* Count overall packet size */
		for (i = 0; i < in_ctx->packets; i++)
			if (urb->iso_frame_desc[i].status == 0)
				bytes += urb->iso_frame_desc[i].actual_length;

		/*
		 * skip empty packets. At least M-Audio's Fast Track Ultra stops
		 * streaming once it received a 0-byte OUT URB
		 */
		if (bytes == 0)
			return;

		spin_lock_irqsave(&ep->lock, flags);
		out_packet = ep->next_packet + ep->next_packet_write_pos;

		/*
		 * Iterate through the inbound packet and prepare the lengths
		 * for the output packet. The OUT packet we are about to send
		 * will have the same amount of payload bytes per stride as the
		 * IN packet we just received. Since the actual size is scaled
		 * by the stride, use the sender stride to calculate the length
		 * in case the number of channels differ between the implicitly
		 * fed-back endpoint and the synchronizing endpoint.
		 */

		out_packet->packets = in_ctx->packets;
		for (i = 0; i < in_ctx->packets; i++) {
			if (urb->iso_frame_desc[i].status == 0)
				out_packet->packet_size[i] =
					urb->iso_frame_desc[i].actual_length / sender->stride;
			else
				out_packet->packet_size[i] = 0;
		}

		ep->next_packet_write_pos++;
		ep->next_packet_write_pos %= MAX_URBS;
		spin_unlock_irqrestore(&ep->lock, flags);
		queue_pending_output_urbs(ep);

		return;
	}

	/*
	 * process after playback sync complete
	 *
	 * Full speed devices report feedback values in 10.14 format as samples
	 * per frame, high speed devices in 16.16 format as samples per
	 * microframe.
	 *
	 * Because the Audio Class 1 spec was written before USB 2.0, many high
	 * speed devices use a wrong interpretation, some others use an
	 * entirely different format.
	 *
	 * Therefore, we cannot predict what format any particular device uses
	 * and must detect it automatically.
	 */

	if (urb->iso_frame_desc[0].status != 0 ||
	    urb->iso_frame_desc[0].actual_length < 3)
		return;
>>>>>>> refs/remotes/origin/master

	f = le32_to_cpup(urb->transfer_buffer);
	if (urb->iso_frame_desc[0].actual_length == 3)
		f &= 0x00ffffff;
	else
		f &= 0x0fffffff;
<<<<<<< HEAD
	if (f == 0)
		return 0;

	if (unlikely(subs->freqshift == INT_MIN)) {
=======

	if (f == 0)
		return;

	if (unlikely(ep->freqshift == INT_MIN)) {
>>>>>>> refs/remotes/origin/master
		/*
		 * The first time we see a feedback value, determine its format
		 * by shifting it left or right until it matches the nominal
		 * frequency value.  This assumes that the feedback does not
		 * differ from the nominal value more than +50% or -25%.
		 */
		shift = 0;
<<<<<<< HEAD
		while (f < subs->freqn - subs->freqn / 4) {
			f <<= 1;
			shift++;
		}
		while (f > subs->freqn + subs->freqn / 2) {
			f >>= 1;
			shift--;
		}
		subs->freqshift = shift;
	}
	else if (subs->freqshift >= 0)
		f <<= subs->freqshift;
	else
		f >>= -subs->freqshift;

	if (likely(f >= subs->freqn - subs->freqn / 8 && f <= subs->freqmax)) {
=======
		while (f < ep->freqn - ep->freqn / 4) {
			f <<= 1;
			shift++;
		}
		while (f > ep->freqn + ep->freqn / 2) {
			f >>= 1;
			shift--;
		}
		ep->freqshift = shift;
	} else if (ep->freqshift >= 0)
		f <<= ep->freqshift;
	else
		f >>= -ep->freqshift;

	if (likely(f >= ep->freqn - ep->freqn / 8 && f <= ep->freqmax)) {
>>>>>>> refs/remotes/origin/master
		/*
		 * If the frequency looks valid, set it.
		 * This value is referred to in prepare_playback_urb().
		 */
<<<<<<< HEAD
		spin_lock_irqsave(&subs->lock, flags);
		subs->freqm = f;
		spin_unlock_irqrestore(&subs->lock, flags);
=======
		spin_lock_irqsave(&ep->lock, flags);
		ep->freqm = f;
		spin_unlock_irqrestore(&ep->lock, flags);
>>>>>>> refs/remotes/origin/master
	} else {
		/*
		 * Out of range; maybe the shift value is wrong.
		 * Reset it so that we autodetect again the next time.
		 */
<<<<<<< HEAD
		subs->freqshift = INT_MIN;
	}

	return 0;
}

/* determine the number of frames in the next packet */
static int snd_usb_audio_next_packet_size(struct snd_usb_substream *subs)
{
	if (subs->fill_max)
		return subs->maxframesize;
	else {
		subs->phase = (subs->phase & 0xffff)
			+ (subs->freqm << subs->datainterval);
		return min(subs->phase >> 16, subs->maxframesize);
	}
}

/*
 * Prepare urb for streaming before playback starts or when paused.
 *
 * We don't have any data, so we send silence.
 */
static int prepare_nodata_playback_urb(struct snd_usb_substream *subs,
				       struct snd_pcm_runtime *runtime,
				       struct urb *urb)
{
	unsigned int i, offs, counts;
	struct snd_urb_ctx *ctx = urb->context;
	int stride = runtime->frame_bits >> 3;

	offs = 0;
	urb->dev = ctx->subs->dev;
	for (i = 0; i < ctx->packets; ++i) {
		counts = snd_usb_audio_next_packet_size(subs);
		urb->iso_frame_desc[i].offset = offs * stride;
		urb->iso_frame_desc[i].length = counts * stride;
		offs += counts;
	}
	urb->number_of_packets = ctx->packets;
	urb->transfer_buffer_length = offs * stride;
	memset(urb->transfer_buffer,
	       runtime->format == SNDRV_PCM_FORMAT_U8 ? 0x80 : 0,
	       offs * stride);
	return 0;
}

/*
 * prepare urb for playback data pipe
 *
 * Since a URB can handle only a single linear buffer, we must use double
 * buffering when the data to be transferred overflows the buffer boundary.
 * To avoid inconsistencies when updating hwptr_done, we use double buffering
 * for all URBs.
 */
static int prepare_playback_urb(struct snd_usb_substream *subs,
				struct snd_pcm_runtime *runtime,
				struct urb *urb)
{
	int i, stride;
	unsigned int counts, frames, bytes;
	unsigned long flags;
	int period_elapsed = 0;
	struct snd_urb_ctx *ctx = urb->context;

	stride = runtime->frame_bits >> 3;

	frames = 0;
	urb->dev = ctx->subs->dev; /* we need to set this at each time */
	urb->number_of_packets = 0;
	spin_lock_irqsave(&subs->lock, flags);
	for (i = 0; i < ctx->packets; i++) {
		counts = snd_usb_audio_next_packet_size(subs);
		/* set up descriptor */
		urb->iso_frame_desc[i].offset = frames * stride;
		urb->iso_frame_desc[i].length = counts * stride;
		frames += counts;
		urb->number_of_packets++;
		subs->transfer_done += counts;
		if (subs->transfer_done >= runtime->period_size) {
			subs->transfer_done -= runtime->period_size;
			period_elapsed = 1;
			if (subs->fmt_type == UAC_FORMAT_TYPE_II) {
				if (subs->transfer_done > 0) {
					/* FIXME: fill-max mode is not
					 * supported yet */
					frames -= subs->transfer_done;
					counts -= subs->transfer_done;
					urb->iso_frame_desc[i].length =
						counts * stride;
					subs->transfer_done = 0;
				}
				i++;
				if (i < ctx->packets) {
					/* add a transfer delimiter */
					urb->iso_frame_desc[i].offset =
						frames * stride;
					urb->iso_frame_desc[i].length = 0;
					urb->number_of_packets++;
				}
				break;
			}
		}
		if (period_elapsed) /* finish at the period boundary */
			break;
	}
	bytes = frames * stride;
	if (subs->hwptr_done + bytes > runtime->buffer_size * stride) {
		/* err, the transferred area goes over buffer boundary. */
		unsigned int bytes1 =
			runtime->buffer_size * stride - subs->hwptr_done;
		memcpy(urb->transfer_buffer,
		       runtime->dma_area + subs->hwptr_done, bytes1);
		memcpy(urb->transfer_buffer + bytes1,
		       runtime->dma_area, bytes - bytes1);
	} else {
		memcpy(urb->transfer_buffer,
		       runtime->dma_area + subs->hwptr_done, bytes);
	}
	subs->hwptr_done += bytes;
	if (subs->hwptr_done >= runtime->buffer_size * stride)
		subs->hwptr_done -= runtime->buffer_size * stride;

	/* update delay with exact number of samples queued */
	runtime->delay = subs->last_delay;
	runtime->delay += frames;
	subs->last_delay = runtime->delay;

	/* realign last_frame_number */
	subs->last_frame_number = usb_get_current_frame_number(subs->dev);
	subs->last_frame_number &= 0xFF; /* keep 8 LSBs */

	spin_unlock_irqrestore(&subs->lock, flags);
	urb->transfer_buffer_length = bytes;
	if (period_elapsed)
		snd_pcm_period_elapsed(subs->pcm_substream);
	return 0;
}

/*
 * process after playback data complete
 * - decrease the delay count again
 */
static int retire_playback_urb(struct snd_usb_substream *subs,
			       struct snd_pcm_runtime *runtime,
			       struct urb *urb)
{
	unsigned long flags;
	int stride = runtime->frame_bits >> 3;
	int processed = urb->transfer_buffer_length / stride;
	int est_delay;

	spin_lock_irqsave(&subs->lock, flags);

	est_delay = snd_usb_pcm_delay(subs, runtime->rate);
	/* update delay with exact number of samples played */
	if (processed > subs->last_delay)
		subs->last_delay = 0;
	else
		subs->last_delay -= processed;
	runtime->delay = subs->last_delay;

	/*
	 * Report when delay estimate is off by more than 2ms.
	 * The error should be lower than 2ms since the estimate relies
	 * on two reads of a counter updated every ms.
	 */
	if (abs(est_delay - subs->last_delay) * 1000 > runtime->rate * 2)
		snd_printk(KERN_DEBUG "delay: estimated %d, actual %d\n",
			est_delay, subs->last_delay);

	spin_unlock_irqrestore(&subs->lock, flags);
	return 0;
}

static const char *usb_error_string(int err)
{
	switch (err) {
	case -ENODEV:
		return "no device";
	case -ENOENT:
		return "endpoint not enabled";
	case -EPIPE:
		return "endpoint stalled";
	case -ENOSPC:
		return "not enough bandwidth";
	case -ESHUTDOWN:
		return "device disabled";
	case -EHOSTUNREACH:
		return "device suspended";
	case -EINVAL:
	case -EAGAIN:
	case -EFBIG:
	case -EMSGSIZE:
		return "internal error";
	default:
		return "unknown error";
	}
}

/*
 * set up and start data/sync urbs
 */
static int start_urbs(struct snd_usb_substream *subs, struct snd_pcm_runtime *runtime)
{
	unsigned int i;
	int err;

	if (subs->stream->chip->shutdown)
		return -EBADFD;

	for (i = 0; i < subs->nurbs; i++) {
		if (snd_BUG_ON(!subs->dataurb[i].urb))
			return -EINVAL;
		if (subs->ops.prepare(subs, runtime, subs->dataurb[i].urb) < 0) {
			snd_printk(KERN_ERR "cannot prepare datapipe for urb %d\n", i);
			goto __error;
		}
	}
	if (subs->syncpipe) {
		for (i = 0; i < SYNC_URBS; i++) {
			if (snd_BUG_ON(!subs->syncurb[i].urb))
				return -EINVAL;
			if (subs->ops.prepare_sync(subs, runtime, subs->syncurb[i].urb) < 0) {
				snd_printk(KERN_ERR "cannot prepare syncpipe for urb %d\n", i);
				goto __error;
			}
		}
	}

	subs->active_mask = 0;
	subs->unlink_mask = 0;
	subs->running = 1;
	for (i = 0; i < subs->nurbs; i++) {
		err = usb_submit_urb(subs->dataurb[i].urb, GFP_ATOMIC);
		if (err < 0) {
			snd_printk(KERN_ERR "cannot submit datapipe "
				   "for urb %d, error %d: %s\n",
				   i, err, usb_error_string(err));
			goto __error;
		}
		set_bit(i, &subs->active_mask);
	}
	if (subs->syncpipe) {
		for (i = 0; i < SYNC_URBS; i++) {
			err = usb_submit_urb(subs->syncurb[i].urb, GFP_ATOMIC);
			if (err < 0) {
				snd_printk(KERN_ERR "cannot submit syncpipe "
					   "for urb %d, error %d: %s\n",
					   i, err, usb_error_string(err));
				goto __error;
			}
			set_bit(i + 16, &subs->active_mask);
		}
	}
	return 0;

 __error:
	// snd_pcm_stop(subs->pcm_substream, SNDRV_PCM_STATE_XRUN);
	deactivate_urbs(subs, 0, 0);
	return -EPIPE;
}


/*
 */
static struct snd_urb_ops audio_urb_ops[2] = {
	{
		.prepare =	prepare_nodata_playback_urb,
		.retire =	retire_playback_urb,
		.prepare_sync =	prepare_playback_sync_urb,
		.retire_sync =	retire_playback_sync_urb,
	},
	{
		.prepare =	prepare_capture_urb,
		.retire =	retire_capture_urb,
		.prepare_sync =	prepare_capture_sync_urb,
		.retire_sync =	retire_capture_sync_urb,
	},
};

/*
 * initialize the substream instance.
 */

void snd_usb_init_substream(struct snd_usb_stream *as,
			    int stream, struct audioformat *fp)
{
	struct snd_usb_substream *subs = &as->substream[stream];

	INIT_LIST_HEAD(&subs->fmt_list);
	spin_lock_init(&subs->lock);

	subs->stream = as;
	subs->direction = stream;
	subs->dev = as->chip->dev;
	subs->txfr_quirk = as->chip->txfr_quirk;
	subs->ops = audio_urb_ops[stream];
	subs->speed = snd_usb_get_speed(subs->dev);
	if (subs->speed >= USB_SPEED_HIGH)
		subs->ops.prepare_sync = prepare_capture_sync_urb_hs;

	snd_usb_set_pcm_ops(as->pcm, stream);

	list_add_tail(&fp->list, &subs->fmt_list);
	subs->formats |= fp->formats;
	subs->endpoint = fp->endpoint;
	subs->num_formats++;
	subs->fmt_type = fp->fmt_type;
}

int snd_usb_substream_playback_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct snd_usb_substream *subs = substream->runtime->private_data;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		subs->ops.prepare = prepare_playback_urb;
		return 0;
	case SNDRV_PCM_TRIGGER_STOP:
		return deactivate_urbs(subs, 0, 0);
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		subs->ops.prepare = prepare_nodata_playback_urb;
		return 0;
	}

	return -EINVAL;
}

int snd_usb_substream_capture_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct snd_usb_substream *subs = substream->runtime->private_data;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		subs->ops.retire = retire_capture_urb;
		return start_urbs(subs, substream->runtime);
	case SNDRV_PCM_TRIGGER_STOP:
		return deactivate_urbs(subs, 0, 0);
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		subs->ops.retire = retire_paused_capture_urb;
		return 0;
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		subs->ops.retire = retire_capture_urb;
		return 0;
	}

	return -EINVAL;
}

int snd_usb_substream_prepare(struct snd_usb_substream *subs,
			      struct snd_pcm_runtime *runtime)
{
	/* clear urbs (to be sure) */
	deactivate_urbs(subs, 0, 1);
	wait_clear_urbs(subs);

	/* for playback, submit the URBs now; otherwise, the first hwptr_done
	 * updates for all URBs would happen at the same time when starting */
	if (subs->direction == SNDRV_PCM_STREAM_PLAYBACK) {
		subs->ops.prepare = prepare_nodata_playback_urb;
		return start_urbs(subs, runtime);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

=======
		ep->freqshift = INT_MIN;
	}
}

>>>>>>> refs/remotes/origin/master
