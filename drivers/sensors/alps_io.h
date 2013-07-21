#ifndef ___ALPS_IO_H_INCLUDED
#define ___ALPS_IO_H_INCLUDED

#include <linux/ioctl.h>

#define ALPSIO   0xAF

#define ALPSIO_SET_MAGACTIVATE   _IOW(ALPSIO, 0, int)
#define ALPSIO_SET_ACCACTIVATE   _IOW(ALPSIO, 1, int)
#define ALPSIO_SET_DELAY         _IOW(ALPSIO, 2, int)

#endif

