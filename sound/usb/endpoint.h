#ifndef __USBAUDIO_ENDPOINT_H
#define __USBAUDIO_ENDPOINT_H

<<<<<<< HEAD
<<<<<<< HEAD
int snd_usb_parse_audio_endpoints(struct snd_usb_audio *chip,
				  int iface_no);

int snd_usb_add_audio_endpoint(struct snd_usb_audio *chip,
			       int stream,
			       struct audioformat *fp);
=======
void snd_usb_init_substream(struct snd_usb_stream *as,
			    int stream,
			    struct audioformat *fp);

int snd_usb_init_substream_urbs(struct snd_usb_substream *subs,
				unsigned int period_bytes,
				unsigned int rate,
				unsigned int frame_bits);

void snd_usb_release_substream_urbs(struct snd_usb_substream *subs, int force);

int snd_usb_substream_prepare(struct snd_usb_substream *subs,
			      struct snd_pcm_runtime *runtime);

int snd_usb_substream_playback_trigger(struct snd_pcm_substream *substream, int cmd);
int snd_usb_substream_capture_trigger(struct snd_pcm_substream *substream, int cmd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define SND_USB_ENDPOINT_TYPE_DATA     0
#define SND_USB_ENDPOINT_TYPE_SYNC     1

struct snd_usb_endpoint *snd_usb_add_endpoint(struct snd_usb_audio *chip,
					      struct usb_host_interface *alts,
					      int ep_num, int direction, int type);

int snd_usb_endpoint_set_params(struct snd_usb_endpoint *ep,
				snd_pcm_format_t pcm_format,
				unsigned int channels,
				unsigned int period_bytes,
				unsigned int period_frames,
				unsigned int buffer_periods,
				unsigned int rate,
				struct audioformat *fmt,
				struct snd_usb_endpoint *sync_ep);

int  snd_usb_endpoint_start(struct snd_usb_endpoint *ep, bool can_sleep);
void snd_usb_endpoint_stop(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_sync_pending_stop(struct snd_usb_endpoint *ep);
int  snd_usb_endpoint_activate(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_deactivate(struct snd_usb_endpoint *ep);
void snd_usb_endpoint_free(struct list_head *head);

int snd_usb_endpoint_implicit_feedback_sink(struct snd_usb_endpoint *ep);
int snd_usb_endpoint_next_packet_size(struct snd_usb_endpoint *ep);

void snd_usb_handle_sync_urb(struct snd_usb_endpoint *ep,
			     struct snd_usb_endpoint *sender,
			     const struct urb *urb);
>>>>>>> refs/remotes/origin/master

#endif /* __USBAUDIO_ENDPOINT_H */
