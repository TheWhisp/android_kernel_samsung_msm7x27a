#ifndef __USBAUDIO_ENDPOINT_H
#define __USBAUDIO_ENDPOINT_H

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

#endif /* __USBAUDIO_ENDPOINT_H */
