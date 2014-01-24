/*
 * Copyright (C) ST-Ericsson AB 2010
<<<<<<< HEAD
 * Author:	Sjur Brendeland/sjur.brandeland@stericsson.com
=======
 * Author:	Sjur Brendeland
>>>>>>> refs/remotes/origin/master
 * License terms: GNU General Public License (GPL) version 2
 */

#ifndef CFSERL_H_
#define CFSERL_H_
#include <net/caif/caif_layer.h>

<<<<<<< HEAD
<<<<<<< HEAD
struct cflayer *cfserl_create(int type, int instance, bool use_stx);
#endif				/* CFSERL_H_ */
=======
struct cflayer *cfserl_create(int instance, bool use_stx);
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct cflayer *cfserl_create(int instance, bool use_stx);
#endif
>>>>>>> refs/remotes/origin/master
