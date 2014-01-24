#ifndef __linux_video_edid_h__
#define __linux_video_edid_h__

<<<<<<< HEAD
#if !defined(__KERNEL__) || defined(CONFIG_X86)

=======
>>>>>>> refs/remotes/origin/cm-10.0
struct edid_info {
	unsigned char dummy[128];
};

#ifdef __KERNEL__
<<<<<<< HEAD
extern struct edid_info edid_info;
#endif /* __KERNEL__ */

=======
#ifdef CONFIG_X86
extern struct edid_info edid_info;
#endif
>>>>>>> refs/remotes/origin/cm-10.0
#endif

#endif /* __linux_video_edid_h__ */
