<<<<<<< HEAD
/*
<<<<<<< HEAD
 * Copyright (C) 2007-2011 B.A.T.M.A.N. contributors:
=======
 * Copyright (C) 2007-2012 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Marek Lindner, Simon Wunderlich
=======
/* Copyright (C) 2007-2013 B.A.T.M.A.N. contributors:
 *
 * Marek Lindner, Simon Wunderlich, Antonio Quartulli
>>>>>>> refs/remotes/origin/master
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

#ifndef _NET_BATMAN_ADV_TRANSLATION_TABLE_H_
#define _NET_BATMAN_ADV_TRANSLATION_TABLE_H_

<<<<<<< HEAD
<<<<<<< HEAD
int tt_local_init(struct bat_priv *bat_priv);
void tt_local_add(struct net_device *soft_iface, uint8_t *addr);
void tt_local_remove(struct bat_priv *bat_priv,
		      uint8_t *addr, char *message);
int tt_local_fill_buffer(struct bat_priv *bat_priv,
			  unsigned char *buff, int buff_len);
int tt_local_seq_print_text(struct seq_file *seq, void *offset);
void tt_local_free(struct bat_priv *bat_priv);
int tt_global_init(struct bat_priv *bat_priv);
void tt_global_add_orig(struct bat_priv *bat_priv,
			 struct orig_node *orig_node,
			 unsigned char *tt_buff, int tt_buff_len);
int tt_global_seq_print_text(struct seq_file *seq, void *offset);
void tt_global_del_orig(struct bat_priv *bat_priv,
			 struct orig_node *orig_node, char *message);
void tt_global_free(struct bat_priv *bat_priv);
struct orig_node *transtable_search(struct bat_priv *bat_priv, uint8_t *addr);
=======
int tt_len(int changes_num);
int tt_changes_fill_buffer(struct bat_priv *bat_priv,
			   unsigned char *buff, int buff_len);
int tt_init(struct bat_priv *bat_priv);
void tt_local_add(struct net_device *soft_iface, const uint8_t *addr,
		  int ifindex);
void tt_local_remove(struct bat_priv *bat_priv,
		     const uint8_t *addr, const char *message, bool roaming);
int tt_local_seq_print_text(struct seq_file *seq, void *offset);
void tt_global_add_orig(struct bat_priv *bat_priv, struct orig_node *orig_node,
			const unsigned char *tt_buff, int tt_buff_len);
int tt_global_add(struct bat_priv *bat_priv, struct orig_node *orig_node,
		  const unsigned char *addr, uint8_t ttvn, bool roaming,
		  bool wifi);
int tt_global_seq_print_text(struct seq_file *seq, void *offset);
void tt_global_del_orig(struct bat_priv *bat_priv,
			struct orig_node *orig_node, const char *message);
void tt_global_del(struct bat_priv *bat_priv,
		   struct orig_node *orig_node, const unsigned char *addr,
		   const char *message, bool roaming);
struct orig_node *transtable_search(struct bat_priv *bat_priv,
				    const uint8_t *src, const uint8_t *addr);
void tt_save_orig_buffer(struct bat_priv *bat_priv, struct orig_node *orig_node,
			 const unsigned char *tt_buff, uint8_t tt_num_changes);
uint16_t tt_local_crc(struct bat_priv *bat_priv);
uint16_t tt_global_crc(struct bat_priv *bat_priv, struct orig_node *orig_node);
void tt_free(struct bat_priv *bat_priv);
bool send_tt_response(struct bat_priv *bat_priv,
		      struct tt_query_packet *tt_request);
bool is_my_client(struct bat_priv *bat_priv, const uint8_t *addr);
void handle_tt_response(struct bat_priv *bat_priv,
			struct tt_query_packet *tt_response);
void send_roam_adv(struct bat_priv *bat_priv, uint8_t *client,
		   struct orig_node *orig_node);
void tt_commit_changes(struct bat_priv *bat_priv);
bool is_ap_isolated(struct bat_priv *bat_priv, uint8_t *src, uint8_t *dst);
void tt_update_orig(struct bat_priv *bat_priv, struct orig_node *orig_node,
		    const unsigned char *tt_buff, uint8_t tt_num_changes,
		    uint8_t ttvn, uint16_t tt_crc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int batadv_tt_init(struct batadv_priv *bat_priv);
bool batadv_tt_local_add(struct net_device *soft_iface, const uint8_t *addr,
			 unsigned short vid, int ifindex);
uint16_t batadv_tt_local_remove(struct batadv_priv *bat_priv,
				const uint8_t *addr, unsigned short vid,
				const char *message, bool roaming);
int batadv_tt_local_seq_print_text(struct seq_file *seq, void *offset);
int batadv_tt_global_seq_print_text(struct seq_file *seq, void *offset);
void batadv_tt_global_del_orig(struct batadv_priv *bat_priv,
			       struct batadv_orig_node *orig_node,
			       int32_t match_vid, const char *message);
struct batadv_orig_node *batadv_transtable_search(struct batadv_priv *bat_priv,
						  const uint8_t *src,
						  const uint8_t *addr,
						  unsigned short vid);
void batadv_tt_free(struct batadv_priv *bat_priv);
bool batadv_is_my_client(struct batadv_priv *bat_priv, const uint8_t *addr,
			 unsigned short vid);
bool batadv_is_ap_isolated(struct batadv_priv *bat_priv, uint8_t *src,
			   uint8_t *dst, unsigned short vid);
void batadv_tt_local_commit_changes(struct batadv_priv *bat_priv);
bool batadv_tt_global_client_is_roaming(struct batadv_priv *bat_priv,
					uint8_t *addr, unsigned short vid);
bool batadv_tt_local_client_is_roaming(struct batadv_priv *bat_priv,
				       uint8_t *addr, unsigned short vid);
void batadv_tt_local_resize_to_mtu(struct net_device *soft_iface);
bool batadv_tt_add_temporary_global_entry(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig_node,
					  const unsigned char *addr,
					  unsigned short vid);
>>>>>>> refs/remotes/origin/master

#endif /* _NET_BATMAN_ADV_TRANSLATION_TABLE_H_ */
