<<<<<<< HEAD
#ifndef _ALPHA_RTC_H
#define _ALPHA_RTC_H

<<<<<<< HEAD
#if defined(CONFIG_ALPHA_GENERIC)
# define get_rtc_time		alpha_mv.rtc_get_time
# define set_rtc_time		alpha_mv.rtc_set_time
#else
# if defined(CONFIG_ALPHA_MARVEL) && defined(CONFIG_SMP)
#  define get_rtc_time		marvel_get_rtc_time
#  define set_rtc_time		marvel_set_rtc_time
# endif
=======
#if defined(CONFIG_ALPHA_MARVEL) && defined(CONFIG_SMP) \
 || defined(CONFIG_ALPHA_GENERIC)
# define get_rtc_time		alpha_mv.rtc_get_time
# define set_rtc_time		alpha_mv.rtc_set_time
>>>>>>> refs/remotes/origin/cm-10.0
#endif

#include <asm-generic/rtc.h>

#endif
=======
#include <asm-generic/rtc.h>
>>>>>>> refs/remotes/origin/master
