/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
<<<<<<< HEAD
**  Copyright (C) 2004-2007 Red Hat, Inc.  All rights reserved.
=======
**  Copyright (C) 2004-2011 Red Hat, Inc.  All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
**
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
**
*******************************************************************************
******************************************************************************/

#ifndef __CONFIG_DOT_H__
#define __CONFIG_DOT_H__

<<<<<<< HEAD
=======
struct dlm_config_node {
	int nodeid;
	int weight;
	int new;
	uint32_t comm_seq;
};

>>>>>>> refs/remotes/origin/cm-10.0
#define DLM_MAX_ADDR_COUNT 3

struct dlm_config_info {
	int ci_tcp_port;
	int ci_buffer_size;
	int ci_rsbtbl_size;
<<<<<<< HEAD
	int ci_lkbtbl_size;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	int ci_dirtbl_size;
	int ci_recover_timer;
	int ci_toss_secs;
	int ci_scan_secs;
	int ci_log_debug;
	int ci_protocol;
	int ci_timewarn_cs;
	int ci_waitwarn_us;
<<<<<<< HEAD
=======
	int ci_new_rsb_count;
	int ci_recover_callbacks;
	char ci_cluster_name[DLM_LOCKSPACE_LEN];
>>>>>>> refs/remotes/origin/cm-10.0
};

extern struct dlm_config_info dlm_config;

int dlm_config_init(void);
void dlm_config_exit(void);
<<<<<<< HEAD
int dlm_node_weight(char *lsname, int nodeid);
int dlm_nodeid_list(char *lsname, int **ids_out, int *ids_count_out,
		    int **new_out, int *new_count_out);
=======
int dlm_config_nodes(char *lsname, struct dlm_config_node **nodes_out,
		     int *count_out);
int dlm_comm_seq(int nodeid, uint32_t *seq);
>>>>>>> refs/remotes/origin/cm-10.0
int dlm_nodeid_to_addr(int nodeid, struct sockaddr_storage *addr);
int dlm_addr_to_nodeid(struct sockaddr_storage *addr, int *nodeid);
int dlm_our_nodeid(void);
int dlm_our_addr(struct sockaddr_storage *addr, int num);

#endif				/* __CONFIG_DOT_H__ */

