<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2002,2008-2011, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2002,2008-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (c) 2002,2008-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __ADRENO_DEBUGFS_H
#define __ADRENO_DEBUGFS_H

#ifdef CONFIG_DEBUG_FS

int adreno_debugfs_init(struct kgsl_device *device);

<<<<<<< HEAD
<<<<<<< HEAD
extern int kgsl_pm_regs_enabled;
extern int adreno_ib_dump_on_pagef_enabled;

static inline int kgsl_pmregs_enabled(void)
{
	return kgsl_pm_regs_enabled;
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
extern int adreno_pm_regs_enabled;
extern int adreno_pm_ib_enabled;

static inline int is_adreno_pm_regs_enabled(void)
{
	return adreno_pm_regs_enabled;
}

static inline int is_adreno_pm_ib_enabled(void)
{
	return adreno_pm_ib_enabled;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

#else
static inline int adreno_debugfs_init(struct kgsl_device *device)
{
	return 0;
}

static inline int kgsl_pmregs_enabled(void)
{
	/* If debugfs is turned off, then always print registers */
	return 1;
}
<<<<<<< HEAD
<<<<<<< HEAD

#endif

static inline int is_adreno_ib_dump_on_pagef_enabled(struct kgsl_device *device)
{
	return adreno_ib_dump_on_pagef_enabled;
}

=======
#endif

>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif

>>>>>>> refs/remotes/origin/cm-11.0
#endif /* __ADRENO_DEBUGFS_H */
