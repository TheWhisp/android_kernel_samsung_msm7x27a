/*
 * Network port table
 *
 * SELinux must keep a mapping of network ports to labels/SIDs.  This
 * mapping is maintained as part of the normal policy but a fast cache is
 * needed to reduce the lookup overhead.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Author: Paul Moore <paul.moore@hp.com>
=======
 * Author: Paul Moore <paul@paul-moore.com>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Author: Paul Moore <paul@paul-moore.com>
>>>>>>> refs/remotes/origin/master
 *
 */

/*
 * (c) Copyright Hewlett-Packard Development Company, L.P., 2008
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _SELINUX_NETPORT_H
#define _SELINUX_NETPORT_H

int sel_netport_sid(u8 protocol, u16 pnum, u32 *sid);

#endif
