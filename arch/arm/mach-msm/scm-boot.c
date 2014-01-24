<<<<<<< HEAD
/* Copyright (c) 2010, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
<<<<<<< HEAD
=======
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
>>>>>>> refs/remotes/origin/master
 */

#include <linux/module.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include <mach/scm.h>
=======
#include "scm.h"
>>>>>>> refs/remotes/origin/master
#include "scm-boot.h"

/*
 * Set the cold/warm boot address for one of the CPU cores.
 */
<<<<<<< HEAD
int scm_set_boot_addr(void *addr, int flags)
{
	struct {
		unsigned int flags;
		void *addr;
=======
int scm_set_boot_addr(phys_addr_t addr, int flags)
{
	struct {
		unsigned int flags;
		phys_addr_t  addr;
>>>>>>> refs/remotes/origin/master
	} cmd;

	cmd.addr = addr;
	cmd.flags = flags;
	return scm_call(SCM_SVC_BOOT, SCM_BOOT_ADDR,
			&cmd, sizeof(cmd), NULL, 0);
}
EXPORT_SYMBOL(scm_set_boot_addr);
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
