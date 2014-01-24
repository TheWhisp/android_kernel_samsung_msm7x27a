/* 
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __UM_TUNTAP_H
#define __UM_TUNTAP_H

<<<<<<< HEAD
#include "net_user.h"
=======
#include <net_user.h>
>>>>>>> refs/remotes/origin/master

struct tuntap_data {
	char *dev_name;
	int fixed_config;
	char *gate_addr;
	int fd;
	void *dev;
};

extern const struct net_user_info tuntap_user_info;

#endif
