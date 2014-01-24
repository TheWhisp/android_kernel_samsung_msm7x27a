#ifndef __PCM037_H__
#define __PCM037_H__

enum pcm037_board_variant {
	PCM037_PCM970,
	PCM037_EET,
};

extern enum pcm037_board_variant pcm037_variant(void);

<<<<<<< HEAD
=======
#ifdef CONFIG_MACH_PCM037_EET
int pcm037_eet_init_devices(void);
#else
static inline int pcm037_eet_init_devices(void) { return 0; }
#endif

>>>>>>> refs/remotes/origin/master
#endif
