<<<<<<< HEAD
/*
<<<<<<< HEAD
 * Copyright (C) 2009-2011 B.A.T.M.A.N. contributors:
=======
 * Copyright (C) 2009-2012 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (C) 2009-2013 B.A.T.M.A.N. contributors:
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
#include "gateway_common.h"
#include "gateway_client.h"

<<<<<<< HEAD
/* calculates the gateway class from kbit */
static void kbit_to_gw_bandwidth(int down, int up, long *gw_srv_class)
{
	int mdown = 0, tdown, tup, difference;
	uint8_t sbit, part;

	*gw_srv_class = 0;
	difference = 0x0FFFFFFF;

	/* test all downspeeds */
	for (sbit = 0; sbit < 2; sbit++) {
		for (part = 0; part < 16; part++) {
			tdown = 32 * (sbit + 2) * (1 << part);

			if (abs(tdown - down) < difference) {
				*gw_srv_class = (sbit << 7) + (part << 3);
				difference = abs(tdown - down);
				mdown = tdown;
			}
		}
	}

	/* test all upspeeds */
	difference = 0x0FFFFFFF;

	for (part = 0; part < 8; part++) {
		tup = ((part + 1) * (mdown)) / 8;

		if (abs(tup - up) < difference) {
			*gw_srv_class = (*gw_srv_class & 0xF8) | part;
			difference = abs(tup - up);
		}
	}
}

/* returns the up and downspeeds in kbit, calculated from the class */
void gw_bandwidth_to_kbit(uint8_t gw_srv_class, int *down, int *up)
{
<<<<<<< HEAD
	char sbit = (gw_srv_class & 0x80) >> 7;
	char dpart = (gw_srv_class & 0x78) >> 3;
	char upart = (gw_srv_class & 0x07);
=======
	int sbit = (gw_srv_class & 0x80) >> 7;
	int dpart = (gw_srv_class & 0x78) >> 3;
	int upart = (gw_srv_class & 0x07);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!gw_srv_class) {
		*down = 0;
		*up = 0;
		return;
	}

	*down = 32 * (sbit + 2) * (1 << dpart);
	*up = ((upart + 1) * (*down)) / 8;
}

static bool parse_gw_bandwidth(struct net_device *net_dev, char *buff,
<<<<<<< HEAD
			       long *up, long *down)
{
	int ret, multi = 1;
	char *slash_ptr, *tmp_ptr;
=======
			       int *up, int *down)
{
	int ret, multi = 1;
	char *slash_ptr, *tmp_ptr;
	long ldown, lup;
>>>>>>> refs/remotes/origin/cm-10.0
=======
/**
 * batadv_parse_gw_bandwidth - parse supplied string buffer to extract download
 *  and upload bandwidth information
 * @net_dev: the soft interface net device
 * @buff: string buffer to parse
 * @down: pointer holding the returned download bandwidth information
 * @up: pointer holding the returned upload bandwidth information
 *
 * Returns false on parse error and true otherwise.
 */
static bool batadv_parse_gw_bandwidth(struct net_device *net_dev, char *buff,
				      uint32_t *down, uint32_t *up)
{
	enum batadv_bandwidth_units bw_unit_type = BATADV_BW_UNIT_KBIT;
	char *slash_ptr, *tmp_ptr;
	long ldown, lup;
	int ret;
>>>>>>> refs/remotes/origin/master

	slash_ptr = strchr(buff, '/');
	if (slash_ptr)
		*slash_ptr = 0;

	if (strlen(buff) > 4) {
		tmp_ptr = buff + strlen(buff) - 4;

		if (strnicmp(tmp_ptr, "mbit", 4) == 0)
<<<<<<< HEAD
			multi = 1024;

		if ((strnicmp(tmp_ptr, "kbit", 4) == 0) ||
<<<<<<< HEAD
			(multi > 1))
			*tmp_ptr = '\0';
	}

	ret = strict_strtoul(buff, 10, down);
=======
		    (multi > 1))
=======
			bw_unit_type = BATADV_BW_UNIT_MBIT;

		if ((strnicmp(tmp_ptr, "kbit", 4) == 0) ||
		    (bw_unit_type == BATADV_BW_UNIT_MBIT))
>>>>>>> refs/remotes/origin/master
			*tmp_ptr = '\0';
	}

	ret = kstrtol(buff, 10, &ldown);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		bat_err(net_dev,
			"Download speed of gateway mode invalid: %s\n",
			buff);
		return false;
	}

<<<<<<< HEAD
	*down *= multi;
=======
	*down = ldown * multi;
>>>>>>> refs/remotes/origin/cm-10.0

	/* we also got some upload info */
	if (slash_ptr) {
		multi = 1;
=======
	if (ret) {
		batadv_err(net_dev,
			   "Download speed of gateway mode invalid: %s\n",
			   buff);
		return false;
	}

	switch (bw_unit_type) {
	case BATADV_BW_UNIT_MBIT:
		*down = ldown * 10;
		break;
	case BATADV_BW_UNIT_KBIT:
	default:
		*down = ldown / 100;
		break;
	}

	/* we also got some upload info */
	if (slash_ptr) {
		bw_unit_type = BATADV_BW_UNIT_KBIT;
>>>>>>> refs/remotes/origin/master

		if (strlen(slash_ptr + 1) > 4) {
			tmp_ptr = slash_ptr + 1 - 4 + strlen(slash_ptr + 1);

			if (strnicmp(tmp_ptr, "mbit", 4) == 0)
<<<<<<< HEAD
				multi = 1024;

			if ((strnicmp(tmp_ptr, "kbit", 4) == 0) ||
<<<<<<< HEAD
				(multi > 1))
				*tmp_ptr = '\0';
		}

		ret = strict_strtoul(slash_ptr + 1, 10, up);
		if (ret) {
			bat_err(net_dev,
				"Upload speed of gateway mode invalid: "
				"%s\n", slash_ptr + 1);
			return false;
		}

		*up *= multi;
=======
			    (multi > 1))
=======
				bw_unit_type = BATADV_BW_UNIT_MBIT;

			if ((strnicmp(tmp_ptr, "kbit", 4) == 0) ||
			    (bw_unit_type == BATADV_BW_UNIT_MBIT))
>>>>>>> refs/remotes/origin/master
				*tmp_ptr = '\0';
		}

		ret = kstrtol(slash_ptr + 1, 10, &lup);
		if (ret) {
<<<<<<< HEAD
			bat_err(net_dev,
				"Upload speed of gateway mode invalid: %s\n",
				slash_ptr + 1);
			return false;
		}

		*up = lup * multi;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			batadv_err(net_dev,
				   "Upload speed of gateway mode invalid: %s\n",
				   slash_ptr + 1);
			return false;
		}

		switch (bw_unit_type) {
		case BATADV_BW_UNIT_MBIT:
			*up = lup * 10;
			break;
		case BATADV_BW_UNIT_KBIT:
		default:
			*up = lup / 100;
			break;
		}
>>>>>>> refs/remotes/origin/master
	}

	return true;
}

<<<<<<< HEAD
ssize_t gw_bandwidth_set(struct net_device *net_dev, char *buff, size_t count)
{
	struct bat_priv *bat_priv = netdev_priv(net_dev);
<<<<<<< HEAD
	long gw_bandwidth_tmp = 0, up = 0, down = 0;
=======
	long gw_bandwidth_tmp = 0;
	int up = 0, down = 0;
>>>>>>> refs/remotes/origin/cm-10.0
	bool ret;

	ret = parse_gw_bandwidth(net_dev, buff, &up, &down);
	if (!ret)
		goto end;

	if ((!down) || (down < 256))
		down = 2000;

	if (!up)
		up = down / 5;

	kbit_to_gw_bandwidth(down, up, &gw_bandwidth_tmp);

	/**
	 * the gw bandwidth we guessed above might not match the given
	 * speeds, hence we need to calculate it back to show the number
	 * that is going to be propagated
	 **/
<<<<<<< HEAD
	gw_bandwidth_to_kbit((uint8_t)gw_bandwidth_tmp,
			     (int *)&down, (int *)&up);

	gw_deselect(bat_priv);
	bat_info(net_dev, "Changing gateway bandwidth from: '%i' to: '%ld' "
		 "(propagating: %ld%s/%ld%s)\n",
=======
	gw_bandwidth_to_kbit((uint8_t)gw_bandwidth_tmp, &down, &up);

	gw_deselect(bat_priv);
	bat_info(net_dev,
		 "Changing gateway bandwidth from: '%i' to: '%ld' (propagating: %d%s/%d%s)\n",
>>>>>>> refs/remotes/origin/cm-10.0
		 atomic_read(&bat_priv->gw_bandwidth), gw_bandwidth_tmp,
		 (down > 2048 ? down / 1024 : down),
		 (down > 2048 ? "MBit" : "KBit"),
		 (up > 2048 ? up / 1024 : up),
		 (up > 2048 ? "MBit" : "KBit"));

	atomic_set(&bat_priv->gw_bandwidth, gw_bandwidth_tmp);
=======
/**
 * batadv_gw_tvlv_container_update - update the gw tvlv container after gateway
 *  setting change
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_gw_tvlv_container_update(struct batadv_priv *bat_priv)
{
	struct batadv_tvlv_gateway_data gw;
	uint32_t down, up;
	char gw_mode;

	gw_mode = atomic_read(&bat_priv->gw_mode);

	switch (gw_mode) {
	case BATADV_GW_MODE_OFF:
	case BATADV_GW_MODE_CLIENT:
		batadv_tvlv_container_unregister(bat_priv, BATADV_TVLV_GW, 1);
		break;
	case BATADV_GW_MODE_SERVER:
		down = atomic_read(&bat_priv->gw.bandwidth_down);
		up = atomic_read(&bat_priv->gw.bandwidth_up);
		gw.bandwidth_down = htonl(down);
		gw.bandwidth_up = htonl(up);
		batadv_tvlv_container_register(bat_priv, BATADV_TVLV_GW, 1,
					       &gw, sizeof(gw));
		break;
	}
}

ssize_t batadv_gw_bandwidth_set(struct net_device *net_dev, char *buff,
				size_t count)
{
	struct batadv_priv *bat_priv = netdev_priv(net_dev);
	uint32_t down_curr, up_curr, down_new = 0, up_new = 0;
	bool ret;

	down_curr = (unsigned int)atomic_read(&bat_priv->gw.bandwidth_down);
	up_curr = (unsigned int)atomic_read(&bat_priv->gw.bandwidth_up);

	ret = batadv_parse_gw_bandwidth(net_dev, buff, &down_new, &up_new);
	if (!ret)
		goto end;

	if (!down_new)
		down_new = 1;

	if (!up_new)
		up_new = down_new / 5;

	if (!up_new)
		up_new = 1;

	if ((down_curr == down_new) && (up_curr == up_new))
		return count;

	batadv_gw_deselect(bat_priv);
	batadv_info(net_dev,
		    "Changing gateway bandwidth from: '%u.%u/%u.%u MBit' to: '%u.%u/%u.%u MBit'\n",
		    down_curr / 10, down_curr % 10, up_curr / 10, up_curr % 10,
		    down_new / 10, down_new % 10, up_new / 10, up_new % 10);

	atomic_set(&bat_priv->gw.bandwidth_down, down_new);
	atomic_set(&bat_priv->gw.bandwidth_up, up_new);
	batadv_gw_tvlv_container_update(bat_priv);
>>>>>>> refs/remotes/origin/master

end:
	return count;
}
<<<<<<< HEAD
=======

/**
 * batadv_gw_tvlv_ogm_handler_v1 - process incoming gateway tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @flags: flags indicating the tvlv state (see batadv_tvlv_handler_flags)
 * @tvlv_value: tvlv buffer containing the gateway data
 * @tvlv_value_len: tvlv buffer length
 */
static void batadv_gw_tvlv_ogm_handler_v1(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig,
					  uint8_t flags,
					  void *tvlv_value,
					  uint16_t tvlv_value_len)
{
	struct batadv_tvlv_gateway_data gateway, *gateway_ptr;

	/* only fetch the tvlv value if the handler wasn't called via the
	 * CIFNOTFND flag and if there is data to fetch
	 */
	if ((flags & BATADV_TVLV_HANDLER_OGM_CIFNOTFND) ||
	    (tvlv_value_len < sizeof(gateway))) {
		gateway.bandwidth_down = 0;
		gateway.bandwidth_up = 0;
	} else {
		gateway_ptr = tvlv_value;
		gateway.bandwidth_down = gateway_ptr->bandwidth_down;
		gateway.bandwidth_up = gateway_ptr->bandwidth_up;
		if ((gateway.bandwidth_down == 0) ||
		    (gateway.bandwidth_up == 0)) {
			gateway.bandwidth_down = 0;
			gateway.bandwidth_up = 0;
		}
	}

	batadv_gw_node_update(bat_priv, orig, &gateway);

	/* restart gateway selection if fast or late switching was enabled */
	if ((gateway.bandwidth_down != 0) &&
	    (atomic_read(&bat_priv->gw_mode) == BATADV_GW_MODE_CLIENT) &&
	    (atomic_read(&bat_priv->gw_sel_class) > 2))
		batadv_gw_check_election(bat_priv, orig);
}

/**
 * batadv_gw_init - initialise the gateway handling internals
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_gw_init(struct batadv_priv *bat_priv)
{
	batadv_tvlv_handler_register(bat_priv, batadv_gw_tvlv_ogm_handler_v1,
				     NULL, BATADV_TVLV_GW, 1,
				     BATADV_TVLV_HANDLER_OGM_CIFNOTFND);
}

/**
 * batadv_gw_free - free the gateway handling internals
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_gw_free(struct batadv_priv *bat_priv)
{
	batadv_tvlv_container_unregister(bat_priv, BATADV_TVLV_GW, 1);
	batadv_tvlv_handler_unregister(bat_priv, BATADV_TVLV_GW, 1);
}
>>>>>>> refs/remotes/origin/master
