/*
<<<<<<< HEAD
<<<<<<< HEAD
 *
 * Copyright (C) 2005-2007 Takahiro Hirofuchi
 */

#include "usbip_network.h"

void pack_uint32_t(int pack, uint32_t *num)
=======
=======
>>>>>>> refs/remotes/origin/master
 * Copyright (C) 2011 matt mooney <mfm@muteddisk.com>
 *               2005-2007 Takahiro Hirofuchi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/socket.h>

#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <unistd.h>

<<<<<<< HEAD
#include "usbip_common.h"
#include "usbip_network.h"

void usbip_net_pack_uint32_t(int pack, uint32_t *num)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef HAVE_LIBWRAP
#include <tcpd.h>
#endif

#include "usbip_common.h"
#include "usbip_network.h"

int usbip_port = 3240;
char *usbip_port_string = "3240";

void usbip_setup_port_number(char *arg)
{
	dbg("parsing port arg '%s'", arg);
	char *end;
	unsigned long int port = strtoul(arg, &end, 10);

	if (end == arg) {
		err("port: could not parse '%s' as a decimal integer", arg);
		return;
	}

	if (*end != '\0') {
		err("port: garbage at end of '%s'", arg);
		return;
	}

	if (port > UINT16_MAX) {
		err("port: %s too high (max=%d)",
		    arg, UINT16_MAX);
		return;
	}

	usbip_port = port;
	usbip_port_string = arg;
	info("using port %d (\"%s\")", usbip_port, usbip_port_string);
}

void usbip_net_pack_uint32_t(int pack, uint32_t *num)
>>>>>>> refs/remotes/origin/master
{
	uint32_t i;

	if (pack)
		i = htonl(*num);
	else
		i = ntohl(*num);

	*num = i;
}

<<<<<<< HEAD
<<<<<<< HEAD
void pack_uint16_t(int pack, uint16_t *num)
=======
void usbip_net_pack_uint16_t(int pack, uint16_t *num)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void usbip_net_pack_uint16_t(int pack, uint16_t *num)
>>>>>>> refs/remotes/origin/master
{
	uint16_t i;

	if (pack)
		i = htons(*num);
	else
		i = ntohs(*num);

	*num = i;
}

<<<<<<< HEAD
<<<<<<< HEAD
void pack_usb_device(int pack, struct usb_device *udev)
{
	pack_uint32_t(pack, &udev->busnum);
	pack_uint32_t(pack, &udev->devnum);
	pack_uint32_t(pack, &udev->speed );

	pack_uint16_t(pack, &udev->idVendor );
	pack_uint16_t(pack, &udev->idProduct);
	pack_uint16_t(pack, &udev->bcdDevice);
}

void pack_usb_interface(int pack __attribute__((unused)),
			struct usb_interface *udev __attribute__((unused)))
=======
=======
>>>>>>> refs/remotes/origin/master
void usbip_net_pack_usb_device(int pack, struct usbip_usb_device *udev)
{
	usbip_net_pack_uint32_t(pack, &udev->busnum);
	usbip_net_pack_uint32_t(pack, &udev->devnum);
<<<<<<< HEAD
	usbip_net_pack_uint32_t(pack, &udev->speed );
=======
	usbip_net_pack_uint32_t(pack, &udev->speed);
>>>>>>> refs/remotes/origin/master

	usbip_net_pack_uint16_t(pack, &udev->idVendor);
	usbip_net_pack_uint16_t(pack, &udev->idProduct);
	usbip_net_pack_uint16_t(pack, &udev->bcdDevice);
}

void usbip_net_pack_usb_interface(int pack __attribute__((unused)),
				  struct usbip_usb_interface *udev
				  __attribute__((unused)))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	/* uint8_t members need nothing */
}

<<<<<<< HEAD
<<<<<<< HEAD

static ssize_t usbip_xmit(int sockfd, void *buff, size_t bufflen, int sending)
{
=======
=======
>>>>>>> refs/remotes/origin/master
static ssize_t usbip_net_xmit(int sockfd, void *buff, size_t bufflen,
			      int sending)
{
	ssize_t nbytes;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ssize_t total = 0;

	if (!bufflen)
		return 0;

	do {
<<<<<<< HEAD
<<<<<<< HEAD
		ssize_t nbytes;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (sending)
			nbytes = send(sockfd, buff, bufflen, 0);
		else
			nbytes = recv(sockfd, buff, bufflen, MSG_WAITALL);

		if (nbytes <= 0)
			return -1;

<<<<<<< HEAD
<<<<<<< HEAD
		buff	= (void *) ((intptr_t) buff + nbytes);
=======
		buff	 = (void *)((intptr_t) buff + nbytes);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		buff	 = (void *)((intptr_t) buff + nbytes);
>>>>>>> refs/remotes/origin/master
		bufflen	-= nbytes;
		total	+= nbytes;

	} while (bufflen > 0);

<<<<<<< HEAD
<<<<<<< HEAD

	return total;
}

ssize_t usbip_recv(int sockfd, void *buff, size_t bufflen)
{
	return usbip_xmit(sockfd, buff, bufflen, 0);
}

ssize_t usbip_send(int sockfd, void *buff, size_t bufflen)
{
	return usbip_xmit(sockfd, buff, bufflen, 1);
}

int usbip_send_op_common(int sockfd, uint32_t code, uint32_t status)
{
	int ret;
	struct op_common op_common;

	bzero(&op_common, sizeof(op_common));

	op_common.version	= USBIP_VERSION;
	op_common.code		= code;
	op_common.status	= status;

	PACK_OP_COMMON(1, &op_common);

	ret = usbip_send(sockfd, (void *) &op_common, sizeof(op_common));
	if (ret < 0) {
		err("send op_common");
=======
=======
>>>>>>> refs/remotes/origin/master
	return total;
}

ssize_t usbip_net_recv(int sockfd, void *buff, size_t bufflen)
{
	return usbip_net_xmit(sockfd, buff, bufflen, 0);
}

ssize_t usbip_net_send(int sockfd, void *buff, size_t bufflen)
{
	return usbip_net_xmit(sockfd, buff, bufflen, 1);
}

int usbip_net_send_op_common(int sockfd, uint32_t code, uint32_t status)
{
	struct op_common op_common;
	int rc;

	memset(&op_common, 0, sizeof(op_common));

	op_common.version = USBIP_VERSION;
	op_common.code    = code;
	op_common.status  = status;

	PACK_OP_COMMON(1, &op_common);

	rc = usbip_net_send(sockfd, &op_common, sizeof(op_common));
	if (rc < 0) {
		dbg("usbip_net_send failed: %d", rc);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
int usbip_recv_op_common(int sockfd, uint16_t *code)
{
	int ret;
	struct op_common op_common;

	bzero(&op_common, sizeof(op_common));

	ret = usbip_recv(sockfd, (void *) &op_common, sizeof(op_common));
	if (ret < 0) {
		err("recv op_common, %d", ret);
=======
=======
>>>>>>> refs/remotes/origin/master
int usbip_net_recv_op_common(int sockfd, uint16_t *code)
{
	struct op_common op_common;
	int rc;

	memset(&op_common, 0, sizeof(op_common));

	rc = usbip_net_recv(sockfd, &op_common, sizeof(op_common));
	if (rc < 0) {
		dbg("usbip_net_recv failed: %d", rc);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		goto err;
	}

	PACK_OP_COMMON(0, &op_common);

	if (op_common.version != USBIP_VERSION) {
<<<<<<< HEAD
<<<<<<< HEAD
		err("version mismatch, %d %d", op_common.version, USBIP_VERSION);
		goto err;
	}

	switch(*code) {
		case OP_UNSPEC:
			break;
		default:
			if (op_common.code != *code) {
				info("unexpected pdu %d for %d", op_common.code, *code);
				goto err;
			}
	}

	if (op_common.status != ST_OK) {
		info("request failed at peer, %d", op_common.status);
=======
=======
>>>>>>> refs/remotes/origin/master
		dbg("version mismatch: %d %d", op_common.version,
		    USBIP_VERSION);
		goto err;
	}

	switch (*code) {
	case OP_UNSPEC:
		break;
	default:
		if (op_common.code != *code) {
			dbg("unexpected pdu %#0x for %#0x", op_common.code,
			    *code);
			goto err;
		}
	}

	if (op_common.status != ST_OK) {
		dbg("request failed at peer: %d", op_common.status);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		goto err;
	}

	*code = op_common.code;

	return 0;
err:
	return -1;
}

<<<<<<< HEAD
<<<<<<< HEAD

int usbip_set_reuseaddr(int sockfd)
=======
int usbip_net_set_reuseaddr(int sockfd)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int usbip_net_set_reuseaddr(int sockfd)
>>>>>>> refs/remotes/origin/master
{
	const int val = 1;
	int ret;

	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	if (ret < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		err("setsockopt SO_REUSEADDR");
=======
		dbg("setsockopt: SO_REUSEADDR");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dbg("setsockopt: SO_REUSEADDR");
>>>>>>> refs/remotes/origin/master

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
int usbip_set_nodelay(int sockfd)
=======
int usbip_net_set_nodelay(int sockfd)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int usbip_net_set_nodelay(int sockfd)
>>>>>>> refs/remotes/origin/master
{
	const int val = 1;
	int ret;

	ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
	if (ret < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		err("setsockopt TCP_NODELAY");
=======
		dbg("setsockopt: TCP_NODELAY");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dbg("setsockopt: TCP_NODELAY");
>>>>>>> refs/remotes/origin/master

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
int usbip_set_keepalive(int sockfd)
=======
int usbip_net_set_keepalive(int sockfd)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int usbip_net_set_keepalive(int sockfd)
>>>>>>> refs/remotes/origin/master
{
	const int val = 1;
	int ret;

	ret = setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val));
	if (ret < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		err("setsockopt SO_KEEPALIVE");
=======
		dbg("setsockopt: SO_KEEPALIVE");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dbg("setsockopt: SO_KEEPALIVE");
>>>>>>> refs/remotes/origin/master

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* IPv6 Ready */
/*
 * moved here from vhci_attach.c
 */
int tcp_connect(char *hostname, char *service)
{
	struct addrinfo hints, *res, *res0;
	int sockfd;
	int err;


	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;

	/* get all possible addresses */
	err = getaddrinfo(hostname, service, &hints, &res0);
	if (err) {
		err("%s %s: %s", hostname, service, gai_strerror(err));
		return -1;
	}

	/* try all the addresses */
	for (res = res0; res; res = res->ai_next) {
		char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

		err = getnameinfo(res->ai_addr, res->ai_addrlen,
				hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
		if (err) {
			err("%s %s: %s", hostname, service, gai_strerror(err));
			continue;
		}

		dbg("trying %s port %s\n", hbuf, sbuf);

		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd < 0) {
			err("socket");
			continue;
		}

		/* should set TCP_NODELAY for usbip */
		usbip_set_nodelay(sockfd);
		/* TODO: write code for heatbeat */
		usbip_set_keepalive(sockfd);

		err = connect(sockfd, res->ai_addr, res->ai_addrlen);
		if (err < 0) {
			close(sockfd);
			continue;
		}

		/* connected */
		dbg("connected to %s:%s", hbuf, sbuf);
		freeaddrinfo(res0);
		return sockfd;
	}


	dbg("%s:%s, %s", hostname, service, "no destination to connect to");
	freeaddrinfo(res0);

	return -1;
=======
=======
int usbip_net_set_v6only(int sockfd)
{
	const int val = 1;
	int ret;

	ret = setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &val, sizeof(val));
	if (ret < 0)
		dbg("setsockopt: IPV6_V6ONLY");

	return ret;
}

>>>>>>> refs/remotes/origin/master
/*
 * IPv6 Ready
 */
int usbip_net_tcp_connect(char *hostname, char *service)
{
	struct addrinfo hints, *res, *rp;
	int sockfd;
	int ret;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	/* get all possible addresses */
	ret = getaddrinfo(hostname, service, &hints, &res);
	if (ret < 0) {
		dbg("getaddrinfo: %s service %s: %s", hostname, service,
		    gai_strerror(ret));
		return ret;
	}

	/* try the addresses */
	for (rp = res; rp; rp = rp->ai_next) {
		sockfd = socket(rp->ai_family, rp->ai_socktype,
				rp->ai_protocol);
		if (sockfd < 0)
			continue;

		/* should set TCP_NODELAY for usbip */
		usbip_net_set_nodelay(sockfd);
		/* TODO: write code for heartbeat */
		usbip_net_set_keepalive(sockfd);

		if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(sockfd);
	}

<<<<<<< HEAD
	if (!rp)
		return EAI_SYSTEM;

	freeaddrinfo(res);

	return sockfd;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	freeaddrinfo(res);

	if (!rp)
		return EAI_SYSTEM;

	return sockfd;
>>>>>>> refs/remotes/origin/master
}
