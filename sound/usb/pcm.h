#ifndef __USBAUDIO_PCM_H
#define __USBAUDIO_PCM_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
snd_pcm_uframes_t snd_usb_pcm_delay(struct snd_usb_substream *subs,
				    unsigned int rate);

>>>>>>> refs/remotes/origin/cm-10.0
=======
snd_pcm_uframes_t snd_usb_pcm_delay(struct snd_usb_substream *subs,
				    unsigned int rate);

>>>>>>> refs/remotes/origin/master
void snd_usb_set_pcm_ops(struct snd_pcm *pcm, int stream);

int snd_usb_init_pitch(struct snd_usb_audio *chip, int iface,
		       struct usb_host_interface *alts,
		       struct audioformat *fmt);


#endif /* __USBAUDIO_PCM_H */
