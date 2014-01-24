/*
 * apple_bl exported symbols
 */

#ifndef _LINUX_APPLE_BL_H
#define _LINUX_APPLE_BL_H

<<<<<<< HEAD
#ifdef CONFIG_BACKLIGHT_APPLE
=======
#if defined(CONFIG_BACKLIGHT_APPLE) || defined(CONFIG_BACKLIGHT_APPLE_MODULE)
>>>>>>> refs/remotes/origin/master

extern int apple_bl_register(void);
extern void apple_bl_unregister(void);

#else /* !CONFIG_BACKLIGHT_APPLE */

static inline int apple_bl_register(void)
{
	return 0;
}

static inline void apple_bl_unregister(void)
{
}

#endif /* !CONFIG_BACKLIGHT_APPLE */

#endif /* _LINUX_APPLE_BL_H */
