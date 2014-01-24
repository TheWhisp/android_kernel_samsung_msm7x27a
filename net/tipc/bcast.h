/*
 * net/tipc/bcast.h: Include file for TIPC broadcast code
 *
 * Copyright (c) 2003-2006, Ericsson AB
 * Copyright (c) 2005, 2010-2011, Wind River Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TIPC_BCAST_H
#define _TIPC_BCAST_H

#define MAX_NODES 4096
#define WSIZE 32

/**
 * struct tipc_node_map - set of node identifiers
 * @count: # of nodes in set
 * @map: bitmap of node identifiers that are in the set
 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
struct tipc_node_map {
	u32 count;
	u32 map[MAX_NODES / WSIZE];
};

<<<<<<< HEAD
<<<<<<< HEAD
extern struct tipc_node_map tipc_bcast_nmap;

#define PLSIZE 32

/**
 * struct port_list - set of node local destination ports
=======
=======
>>>>>>> refs/remotes/origin/master
#define PLSIZE 32

/**
 * struct tipc_port_list - set of node local destination ports
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * @count: # of ports in set (only valid for first entry in list)
 * @next: pointer to next entry in list
 * @ports: array of port references
 */
<<<<<<< HEAD

<<<<<<< HEAD
struct port_list {
	int count;
	struct port_list *next;
=======
struct tipc_port_list {
	int count;
	struct tipc_port_list *next;
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct tipc_port_list {
	int count;
	struct tipc_port_list *next;
>>>>>>> refs/remotes/origin/master
	u32 ports[PLSIZE];
};


struct tipc_node;

extern const char tipc_bclink_name[];

void tipc_nmap_add(struct tipc_node_map *nm_ptr, u32 node);
void tipc_nmap_remove(struct tipc_node_map *nm_ptr, u32 node);

/**
 * tipc_nmap_equal - test for equality of node maps
 */
<<<<<<< HEAD

static inline int tipc_nmap_equal(struct tipc_node_map *nm_a, struct tipc_node_map *nm_b)
=======
static inline int tipc_nmap_equal(struct tipc_node_map *nm_a,
				  struct tipc_node_map *nm_b)
>>>>>>> refs/remotes/origin/master
{
	return !memcmp(nm_a, nm_b, sizeof(*nm_a));
}

<<<<<<< HEAD
<<<<<<< HEAD
void tipc_port_list_add(struct port_list *pl_ptr, u32 port);
void tipc_port_list_free(struct port_list *pl_ptr);

int  tipc_bclink_init(void);
void tipc_bclink_stop(void);
=======
=======
>>>>>>> refs/remotes/origin/master
void tipc_port_list_add(struct tipc_port_list *pl_ptr, u32 port);
void tipc_port_list_free(struct tipc_port_list *pl_ptr);

void tipc_bclink_init(void);
void tipc_bclink_stop(void);
void tipc_bclink_add_node(u32 addr);
void tipc_bclink_remove_node(u32 addr);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct tipc_node *tipc_bclink_retransmit_to(void);
void tipc_bclink_acknowledge(struct tipc_node *n_ptr, u32 acked);
int  tipc_bclink_send_msg(struct sk_buff *buf);
void tipc_bclink_recv_pkt(struct sk_buff *buf);
u32  tipc_bclink_get_last_sent(void);
u32  tipc_bclink_acks_missing(struct tipc_node *n_ptr);
<<<<<<< HEAD
<<<<<<< HEAD
void tipc_bclink_check_gap(struct tipc_node *n_ptr, u32 seqno);
=======
void tipc_bclink_update_link_state(struct tipc_node *n_ptr, u32 last_sent);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void tipc_bclink_update_link_state(struct tipc_node *n_ptr, u32 last_sent);
>>>>>>> refs/remotes/origin/master
int  tipc_bclink_stats(char *stats_buf, const u32 buf_size);
int  tipc_bclink_reset_stats(void);
int  tipc_bclink_set_queue_limits(u32 limit);
void tipc_bcbearer_sort(void);
<<<<<<< HEAD
<<<<<<< HEAD
void tipc_bcbearer_push(void);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif
