<<<<<<< HEAD
/*
<<<<<<< HEAD
 * Copyright (C) 2007-2011 B.A.T.M.A.N. contributors:
=======
 * Copyright (C) 2007-2012 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (C) 2007-2013 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/master
 *
 * Marek Lindner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
<<<<<<< HEAD
 *
=======
>>>>>>> refs/remotes/origin/master
 */

#ifndef _NET_BATMAN_ADV_ICMP_SOCKET_H_
#define _NET_BATMAN_ADV_ICMP_SOCKET_H_

<<<<<<< HEAD
#define ICMP_SOCKET "socket"

void bat_socket_init(void);
int bat_socket_setup(struct bat_priv *bat_priv);
void bat_socket_receive_packet(struct icmp_packet_rr *icmp_packet,
			       size_t icmp_len);
=======
#define BATADV_ICMP_SOCKET "socket"

void batadv_socket_init(void);
int batadv_socket_setup(struct batadv_priv *bat_priv);
void batadv_socket_receive_packet(struct batadv_icmp_header *icmph,
				  size_t icmp_len);
>>>>>>> refs/remotes/origin/master

#endif /* _NET_BATMAN_ADV_ICMP_SOCKET_H_ */
