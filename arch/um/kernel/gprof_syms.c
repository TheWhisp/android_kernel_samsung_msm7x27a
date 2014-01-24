/* 
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

<<<<<<< HEAD
#include "linux/module.h"
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master

extern void mcount(void);
EXPORT_SYMBOL(mcount);
