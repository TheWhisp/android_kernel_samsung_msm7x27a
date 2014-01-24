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

#include "main.h"
#include <linux/debugfs.h>
#include <linux/slab.h>
#include "icmp_socket.h"
#include "send.h"
#include "hash.h"
#include "originator.h"
#include "hard-interface.h"

<<<<<<< HEAD
static struct socket_client *socket_client_hash[256];

static void bat_socket_add_packet(struct socket_client *socket_client,
				  struct icmp_packet_rr *icmp_packet,
				  size_t icmp_len);

void bat_socket_init(void)
{
	memset(socket_client_hash, 0, sizeof(socket_client_hash));
}

static int bat_socket_open(struct inode *inode, struct file *file)
{
	unsigned int i;
	struct socket_client *socket_client;

	nonseekable_open(inode, file);

<<<<<<< HEAD
	socket_client = kmalloc(sizeof(struct socket_client), GFP_KERNEL);
=======
	socket_client = kmalloc(sizeof(*socket_client), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!socket_client)
		return -ENOMEM;

	for (i = 0; i < ARRAY_SIZE(socket_client_hash); i++) {
		if (!socket_client_hash[i]) {
			socket_client_hash[i] = socket_client;
=======
static struct batadv_socket_client *batadv_socket_client_hash[256];

static void batadv_socket_add_packet(struct batadv_socket_client *socket_client,
				     struct batadv_icmp_header *icmph,
				     size_t icmp_len);

void batadv_socket_init(void)
{
	memset(batadv_socket_client_hash, 0, sizeof(batadv_socket_client_hash));
}

static int batadv_socket_open(struct inode *inode, struct file *file)
{
	unsigned int i;
	struct batadv_socket_client *socket_client;

	if (!try_module_get(THIS_MODULE))
		return -EBUSY;

	nonseekable_open(inode, file);

	socket_client = kmalloc(sizeof(*socket_client), GFP_KERNEL);
	if (!socket_client) {
		module_put(THIS_MODULE);
		return -ENOMEM;
	}

	for (i = 0; i < ARRAY_SIZE(batadv_socket_client_hash); i++) {
		if (!batadv_socket_client_hash[i]) {
			batadv_socket_client_hash[i] = socket_client;
>>>>>>> refs/remotes/origin/master
			break;
		}
	}

<<<<<<< HEAD
	if (i == ARRAY_SIZE(socket_client_hash)) {
<<<<<<< HEAD
		pr_err("Error - can't add another packet client: "
		       "maximum number of clients reached\n");
=======
		pr_err("Error - can't add another packet client: maximum number of clients reached\n");
>>>>>>> refs/remotes/origin/cm-10.0
		kfree(socket_client);
=======
	if (i == ARRAY_SIZE(batadv_socket_client_hash)) {
		pr_err("Error - can't add another packet client: maximum number of clients reached\n");
		kfree(socket_client);
		module_put(THIS_MODULE);
>>>>>>> refs/remotes/origin/master
		return -EXFULL;
	}

	INIT_LIST_HEAD(&socket_client->queue_list);
	socket_client->queue_len = 0;
	socket_client->index = i;
	socket_client->bat_priv = inode->i_private;
	spin_lock_init(&socket_client->lock);
	init_waitqueue_head(&socket_client->queue_wait);

	file->private_data = socket_client;

<<<<<<< HEAD
	inc_module_count();
	return 0;
}

static int bat_socket_release(struct inode *inode, struct file *file)
{
	struct socket_client *socket_client = file->private_data;
	struct socket_packet *socket_packet;
=======
	return 0;
}

static int batadv_socket_release(struct inode *inode, struct file *file)
{
	struct batadv_socket_client *socket_client = file->private_data;
	struct batadv_socket_packet *socket_packet;
>>>>>>> refs/remotes/origin/master
	struct list_head *list_pos, *list_pos_tmp;

	spin_lock_bh(&socket_client->lock);

	/* for all packets in the queue ... */
	list_for_each_safe(list_pos, list_pos_tmp, &socket_client->queue_list) {
		socket_packet = list_entry(list_pos,
<<<<<<< HEAD
					   struct socket_packet, list);
=======
					   struct batadv_socket_packet, list);
>>>>>>> refs/remotes/origin/master

		list_del(list_pos);
		kfree(socket_packet);
	}

<<<<<<< HEAD
	socket_client_hash[socket_client->index] = NULL;
	spin_unlock_bh(&socket_client->lock);

	kfree(socket_client);
	dec_module_count();
=======
	batadv_socket_client_hash[socket_client->index] = NULL;
	spin_unlock_bh(&socket_client->lock);

	kfree(socket_client);
	module_put(THIS_MODULE);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static ssize_t bat_socket_read(struct file *file, char __user *buf,
			       size_t count, loff_t *ppos)
{
	struct socket_client *socket_client = file->private_data;
	struct socket_packet *socket_packet;
=======
static ssize_t batadv_socket_read(struct file *file, char __user *buf,
				  size_t count, loff_t *ppos)
{
	struct batadv_socket_client *socket_client = file->private_data;
	struct batadv_socket_packet *socket_packet;
>>>>>>> refs/remotes/origin/master
	size_t packet_len;
	int error;

	if ((file->f_flags & O_NONBLOCK) && (socket_client->queue_len == 0))
		return -EAGAIN;

<<<<<<< HEAD
	if ((!buf) || (count < sizeof(struct icmp_packet)))
=======
	if ((!buf) || (count < sizeof(struct batadv_icmp_packet)))
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	if (!access_ok(VERIFY_WRITE, buf, count))
		return -EFAULT;

	error = wait_event_interruptible(socket_client->queue_wait,
					 socket_client->queue_len);

	if (error)
		return error;

	spin_lock_bh(&socket_client->lock);

	socket_packet = list_first_entry(&socket_client->queue_list,
<<<<<<< HEAD
					 struct socket_packet, list);
=======
					 struct batadv_socket_packet, list);
>>>>>>> refs/remotes/origin/master
	list_del(&socket_packet->list);
	socket_client->queue_len--;

	spin_unlock_bh(&socket_client->lock);

	packet_len = min(count, socket_packet->icmp_len);
	error = copy_to_user(buf, &socket_packet->icmp_packet, packet_len);

	kfree(socket_packet);

	if (error)
		return -EFAULT;

	return packet_len;
}

<<<<<<< HEAD
static ssize_t bat_socket_write(struct file *file, const char __user *buff,
				size_t len, loff_t *off)
{
	struct socket_client *socket_client = file->private_data;
	struct bat_priv *bat_priv = socket_client->bat_priv;
	struct hard_iface *primary_if = NULL;
	struct sk_buff *skb;
	struct icmp_packet_rr *icmp_packet;

	struct orig_node *orig_node = NULL;
	struct neigh_node *neigh_node = NULL;
	size_t packet_len = sizeof(struct icmp_packet);

	if (len < sizeof(struct icmp_packet)) {
		bat_dbg(DBG_BATMAN, bat_priv,
<<<<<<< HEAD
			"Error - can't send packet from char device: "
			"invalid packet size\n");
=======
			"Error - can't send packet from char device: invalid packet size\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}

	primary_if = primary_if_get_selected(bat_priv);
=======
static ssize_t batadv_socket_write(struct file *file, const char __user *buff,
				   size_t len, loff_t *off)
{
	struct batadv_socket_client *socket_client = file->private_data;
	struct batadv_priv *bat_priv = socket_client->bat_priv;
	struct batadv_hard_iface *primary_if = NULL;
	struct sk_buff *skb;
	struct batadv_icmp_packet_rr *icmp_packet_rr;
	struct batadv_icmp_header *icmp_header;
	struct batadv_orig_node *orig_node = NULL;
	struct batadv_neigh_node *neigh_node = NULL;
	size_t packet_len = sizeof(struct batadv_icmp_packet);

	if (len < sizeof(struct batadv_icmp_header)) {
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "Error - can't send packet from char device: invalid packet size\n");
		return -EINVAL;
	}

	primary_if = batadv_primary_if_get_selected(bat_priv);
>>>>>>> refs/remotes/origin/master

	if (!primary_if) {
		len = -EFAULT;
		goto out;
	}

<<<<<<< HEAD
	if (len >= sizeof(struct icmp_packet_rr))
		packet_len = sizeof(struct icmp_packet_rr);

	skb = dev_alloc_skb(packet_len + sizeof(struct ethhdr));
=======
	if (len >= BATADV_ICMP_MAX_PACKET_SIZE)
		packet_len = BATADV_ICMP_MAX_PACKET_SIZE;
	else
		packet_len = len;

	skb = netdev_alloc_skb_ip_align(NULL, packet_len + ETH_HLEN);
>>>>>>> refs/remotes/origin/master
	if (!skb) {
		len = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
	skb_reserve(skb, sizeof(struct ethhdr));
	icmp_packet = (struct icmp_packet_rr *)skb_put(skb, packet_len);

<<<<<<< HEAD
	if (!access_ok(VERIFY_READ, buff, packet_len)) {
=======
	if (copy_from_user(icmp_packet, buff, packet_len)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	skb->priority = TC_PRIO_CONTROL;
	skb_reserve(skb, ETH_HLEN);
	icmp_header = (struct batadv_icmp_header *)skb_put(skb, packet_len);

	if (copy_from_user(icmp_header, buff, packet_len)) {
>>>>>>> refs/remotes/origin/master
		len = -EFAULT;
		goto free_skb;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (__copy_from_user(icmp_packet, buff, packet_len)) {
		len = -EFAULT;
		goto free_skb;
	}

	if (icmp_packet->packet_type != BAT_ICMP) {
		bat_dbg(DBG_BATMAN, bat_priv,
			"Error - can't send packet from char device: "
			"got bogus packet type (expected: BAT_ICMP)\n");
=======
	if (icmp_packet->header.packet_type != BAT_ICMP) {
		bat_dbg(DBG_BATMAN, bat_priv,
			"Error - can't send packet from char device: got bogus packet type (expected: BAT_ICMP)\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (icmp_header->packet_type != BATADV_ICMP) {
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "Error - can't send packet from char device: got bogus packet type (expected: BAT_ICMP)\n");
>>>>>>> refs/remotes/origin/master
		len = -EINVAL;
		goto free_skb;
	}

<<<<<<< HEAD
	if (icmp_packet->msg_type != ECHO_REQUEST) {
		bat_dbg(DBG_BATMAN, bat_priv,
<<<<<<< HEAD
			"Error - can't send packet from char device: "
			"got bogus message type (expected: ECHO_REQUEST)\n");
=======
			"Error - can't send packet from char device: got bogus message type (expected: ECHO_REQUEST)\n");
>>>>>>> refs/remotes/origin/cm-10.0
		len = -EINVAL;
		goto free_skb;
	}

	icmp_packet->uid = socket_client->index;

<<<<<<< HEAD
	if (icmp_packet->version != COMPAT_VERSION) {
		icmp_packet->msg_type = PARAMETER_PROBLEM;
		icmp_packet->ttl = COMPAT_VERSION;
=======
	if (icmp_packet->header.version != COMPAT_VERSION) {
		icmp_packet->msg_type = PARAMETER_PROBLEM;
		icmp_packet->header.version = COMPAT_VERSION;
>>>>>>> refs/remotes/origin/cm-10.0
		bat_socket_add_packet(socket_client, icmp_packet, packet_len);
		goto free_skb;
	}

	if (atomic_read(&bat_priv->mesh_state) != MESH_ACTIVE)
		goto dst_unreach;

	orig_node = orig_hash_find(bat_priv, icmp_packet->dst);
	if (!orig_node)
		goto dst_unreach;

	neigh_node = orig_node_get_router(orig_node);
	if (!neigh_node)
		goto dst_unreach;

	if (!neigh_node->if_incoming)
		goto dst_unreach;

	if (neigh_node->if_incoming->if_status != IF_ACTIVE)
		goto dst_unreach;

	memcpy(icmp_packet->orig,
	       primary_if->net_dev->dev_addr, ETH_ALEN);

	if (packet_len == sizeof(struct icmp_packet_rr))
		memcpy(icmp_packet->rr,
		       neigh_node->if_incoming->net_dev->dev_addr, ETH_ALEN);

	send_skb_packet(skb, neigh_node->if_incoming, neigh_node->addr);
	goto out;

dst_unreach:
	icmp_packet->msg_type = DESTINATION_UNREACHABLE;
	bat_socket_add_packet(socket_client, icmp_packet, packet_len);
=======
	switch (icmp_header->msg_type) {
	case BATADV_ECHO_REQUEST:
		if (len < sizeof(struct batadv_icmp_packet)) {
			batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
				   "Error - can't send packet from char device: invalid packet size\n");
			len = -EINVAL;
			goto free_skb;
		}

		if (atomic_read(&bat_priv->mesh_state) != BATADV_MESH_ACTIVE)
			goto dst_unreach;

		orig_node = batadv_orig_hash_find(bat_priv, icmp_header->dst);
		if (!orig_node)
			goto dst_unreach;

		neigh_node = batadv_orig_node_get_router(orig_node);
		if (!neigh_node)
			goto dst_unreach;

		if (!neigh_node->if_incoming)
			goto dst_unreach;

		if (neigh_node->if_incoming->if_status != BATADV_IF_ACTIVE)
			goto dst_unreach;

		icmp_packet_rr = (struct batadv_icmp_packet_rr *)icmp_header;
		if (packet_len == sizeof(*icmp_packet_rr))
			memcpy(icmp_packet_rr->rr,
			       neigh_node->if_incoming->net_dev->dev_addr,
			       ETH_ALEN);

		break;
	default:
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "Error - can't send packet from char device: got unknown message type\n");
		len = -EINVAL;
		goto free_skb;
	}

	icmp_header->uid = socket_client->index;

	if (icmp_header->version != BATADV_COMPAT_VERSION) {
		icmp_header->msg_type = BATADV_PARAMETER_PROBLEM;
		icmp_header->version = BATADV_COMPAT_VERSION;
		batadv_socket_add_packet(socket_client, icmp_header,
					 packet_len);
		goto free_skb;
	}

	memcpy(icmp_header->orig, primary_if->net_dev->dev_addr, ETH_ALEN);

	batadv_send_skb_packet(skb, neigh_node->if_incoming, neigh_node->addr);
	goto out;

dst_unreach:
	icmp_header->msg_type = BATADV_DESTINATION_UNREACHABLE;
	batadv_socket_add_packet(socket_client, icmp_header, packet_len);
>>>>>>> refs/remotes/origin/master
free_skb:
	kfree_skb(skb);
out:
	if (primary_if)
<<<<<<< HEAD
		hardif_free_ref(primary_if);
	if (neigh_node)
		neigh_node_free_ref(neigh_node);
	if (orig_node)
		orig_node_free_ref(orig_node);
	return len;
}

static unsigned int bat_socket_poll(struct file *file, poll_table *wait)
{
	struct socket_client *socket_client = file->private_data;
=======
		batadv_hardif_free_ref(primary_if);
	if (neigh_node)
		batadv_neigh_node_free_ref(neigh_node);
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
	return len;
}

static unsigned int batadv_socket_poll(struct file *file, poll_table *wait)
{
	struct batadv_socket_client *socket_client = file->private_data;
>>>>>>> refs/remotes/origin/master

	poll_wait(file, &socket_client->queue_wait, wait);

	if (socket_client->queue_len > 0)
		return POLLIN | POLLRDNORM;

	return 0;
}

<<<<<<< HEAD
static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = bat_socket_open,
	.release = bat_socket_release,
	.read = bat_socket_read,
	.write = bat_socket_write,
	.poll = bat_socket_poll,
	.llseek = no_llseek,
};

int bat_socket_setup(struct bat_priv *bat_priv)
=======
static const struct file_operations batadv_fops = {
	.owner = THIS_MODULE,
	.open = batadv_socket_open,
	.release = batadv_socket_release,
	.read = batadv_socket_read,
	.write = batadv_socket_write,
	.poll = batadv_socket_poll,
	.llseek = no_llseek,
};

int batadv_socket_setup(struct batadv_priv *bat_priv)
>>>>>>> refs/remotes/origin/master
{
	struct dentry *d;

	if (!bat_priv->debug_dir)
		goto err;

<<<<<<< HEAD
	d = debugfs_create_file(ICMP_SOCKET, S_IFREG | S_IWUSR | S_IRUSR,
				bat_priv->debug_dir, bat_priv, &fops);
	if (d)
=======
	d = debugfs_create_file(BATADV_ICMP_SOCKET, S_IFREG | S_IWUSR | S_IRUSR,
				bat_priv->debug_dir, bat_priv, &batadv_fops);
	if (!d)
>>>>>>> refs/remotes/origin/master
		goto err;

	return 0;

err:
<<<<<<< HEAD
	return 1;
}

static void bat_socket_add_packet(struct socket_client *socket_client,
				  struct icmp_packet_rr *icmp_packet,
				  size_t icmp_len)
{
	struct socket_packet *socket_packet;

<<<<<<< HEAD
	socket_packet = kmalloc(sizeof(struct socket_packet), GFP_ATOMIC);
=======
	socket_packet = kmalloc(sizeof(*socket_packet), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return -ENOMEM;
}

/**
 * batadv_socket_receive_packet - schedule an icmp packet to be sent to userspace
 *  on an icmp socket.
 * @socket_client: the socket this packet belongs to
 * @icmph: pointer to the header of the icmp packet
 * @icmp_len: total length of the icmp packet
 */
static void batadv_socket_add_packet(struct batadv_socket_client *socket_client,
				     struct batadv_icmp_header *icmph,
				     size_t icmp_len)
{
	struct batadv_socket_packet *socket_packet;
	size_t len;

	socket_packet = kmalloc(sizeof(*socket_packet), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/master

	if (!socket_packet)
		return;

<<<<<<< HEAD
	INIT_LIST_HEAD(&socket_packet->list);
	memcpy(&socket_packet->icmp_packet, icmp_packet, icmp_len);
	socket_packet->icmp_len = icmp_len;
=======
	len = icmp_len;
	/* check the maximum length before filling the buffer */
	if (len > sizeof(socket_packet->icmp_packet))
		len = sizeof(socket_packet->icmp_packet);

	INIT_LIST_HEAD(&socket_packet->list);
	memcpy(&socket_packet->icmp_packet, icmph, len);
	socket_packet->icmp_len = len;
>>>>>>> refs/remotes/origin/master

	spin_lock_bh(&socket_client->lock);

	/* while waiting for the lock the socket_client could have been
<<<<<<< HEAD
	 * deleted */
	if (!socket_client_hash[icmp_packet->uid]) {
=======
	 * deleted
	 */
	if (!batadv_socket_client_hash[icmph->uid]) {
>>>>>>> refs/remotes/origin/master
		spin_unlock_bh(&socket_client->lock);
		kfree(socket_packet);
		return;
	}

	list_add_tail(&socket_packet->list, &socket_client->queue_list);
	socket_client->queue_len++;

	if (socket_client->queue_len > 100) {
		socket_packet = list_first_entry(&socket_client->queue_list,
<<<<<<< HEAD
						 struct socket_packet, list);
=======
						 struct batadv_socket_packet,
						 list);
>>>>>>> refs/remotes/origin/master

		list_del(&socket_packet->list);
		kfree(socket_packet);
		socket_client->queue_len--;
	}

	spin_unlock_bh(&socket_client->lock);

	wake_up(&socket_client->queue_wait);
}

<<<<<<< HEAD
void bat_socket_receive_packet(struct icmp_packet_rr *icmp_packet,
			       size_t icmp_len)
{
	struct socket_client *hash = socket_client_hash[icmp_packet->uid];

	if (hash)
		bat_socket_add_packet(hash, icmp_packet, icmp_len);
=======
/**
 * batadv_socket_receive_packet - schedule an icmp packet to be received
 *  locally and sent to userspace.
 * @icmph: pointer to the header of the icmp packet
 * @icmp_len: total length of the icmp packet
 */
void batadv_socket_receive_packet(struct batadv_icmp_header *icmph,
				  size_t icmp_len)
{
	struct batadv_socket_client *hash;

	hash = batadv_socket_client_hash[icmph->uid];
	if (hash)
		batadv_socket_add_packet(hash, icmph, icmp_len);
>>>>>>> refs/remotes/origin/master
}
