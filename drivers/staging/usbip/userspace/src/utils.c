/*
<<<<<<< HEAD
 *
 * Copyright (C) 2005-2007 Takahiro Hirofuchi
 */

#include "utils.h"

int read_integer(char *path)
{
	char buff[100];
	int fd;
	int ret = 0;

	bzero(buff, sizeof(buff));

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return -1;

	ret = read(fd, buff, sizeof(buff));
	if (ret < 0) {
		close(fd);
		return -1;
	}

	sscanf(buff, "%d", &ret);

	close(fd);

	return ret;
}

int read_string(char *path, char *string, size_t len)
{
	int fd;
	int ret = 0;
	char  *p;

	bzero(string, len);

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		string = NULL;
		return -1;
	}

	ret = read(fd, string, len-1);
	if (ret < 0) {
		string = NULL;
		close(fd);
		return -1;
	}

	p = strchr(string, '\n');
	*p = '\0';

	close(fd);

	return 0;
}

int write_integer(char *path, int value)
{
	int fd;
	int ret;
	char buff[100];

	snprintf(buff, sizeof(buff), "%d", value);

	fd = open(path, O_WRONLY);
	if (fd < 0)
		return -1;

	ret = write(fd, buff, strlen(buff));
	if (ret < 0) {
		close(fd);
		return -1;
	}

	close(fd);

	return 0;
}

int read_bConfigurationValue(char *busid)
{
	char path[PATH_MAX];

	snprintf(path, PATH_MAX, "/sys/bus/usb/devices/%s/bConfigurationValue", busid);

	return read_integer(path);
}

int write_bConfigurationValue(char *busid, int config)
{
	char path[PATH_MAX];

	snprintf(path, PATH_MAX, "/sys/bus/usb/devices/%s/bConfigurationValue", busid);

	return write_integer(path, config);
}

int read_bNumInterfaces(char *busid)
{
	char path[PATH_MAX];

	snprintf(path, PATH_MAX, "/sys/bus/usb/devices/%s/bNumInterfaces", busid);

	return read_integer(path);
}

int read_bDeviceClass(char *busid)
{
	char path[PATH_MAX];

	snprintf(path, PATH_MAX, "/sys/bus/usb/devices/%s/bDeviceClass", busid);

	return read_integer(path);
}

int getdriver(char *busid, int conf, int infnum, char *driver, size_t len)
{
	char path[PATH_MAX];
	char linkto[PATH_MAX];
	int ret;

	snprintf(path, PATH_MAX, "/sys/bus/usb/devices/%s:%d.%d/driver", busid, conf, infnum);

	/* readlink does not add NULL */
	bzero(linkto, sizeof(linkto));
	ret = readlink(path, linkto, sizeof(linkto)-1);
	if (ret < 0) {
		strncpy(driver, "none", len);
		return -1;
	} else {
		strncpy(driver, basename(linkto), len);
		return 0;
	}
}

int getdevicename(char *busid, char *name, size_t len)
{
	char path[PATH_MAX];
	char idProduct[10], idVendor[10];

	snprintf(path, PATH_MAX, "/sys/bus/usb/devices/%s/idVendor", busid);
	read_string(path, idVendor, sizeof(idVendor));

	snprintf(path, PATH_MAX, "/sys/bus/usb/devices/%s/idProduct", busid);
	read_string(path, idProduct, sizeof(idProduct));

	if (!idVendor[0] || !idProduct[0])
		return -1;

	snprintf(name, len, "%s:%s", idVendor, idProduct);

	return 0;
}

#define MAXLINE 100

/* if this cannot read a whole line, return -1 */
int readline(int sockfd, char *buff, int bufflen)
{
	int ret;
	char c;
	int index = 0;


	while (index < bufflen) {
		ret = read(sockfd, &c, sizeof(c));
		if (ret < 0 && errno == EINTR)
			continue;
		if (ret <= 0) {
			return -1;
		}

		buff[index] = c;

		if ( index > 0 && buff[index-1] == '\r'  && buff[index] == '\n') {
			/* end of line */
			buff[index-1] = '\0';	/* get rid of delimitor */
			return index;
		} else
			index++;
	}

	return -1;
}

#if 0
int writeline(int sockfd, char *str, int strlen)
{
	int ret;
	int index = 0;
	int len;
	char buff[MAXLINE];

	if (strlen + 3 > MAXLINE)
		return -1;

	strncpy(buff, str, strlen);
	buff[strlen+1] = '\r';
	buff[strlen+2] = '\n';
	buff[strlen+3] = '\0';

	len = strlen + 3;

	while (len > 0) {
		ret = write(sockfd, buff+index, len);
		if (ret <= 0) {
			return -1;
		}

		len -= ret;
		index += ret;
	}

	return index;
}
#endif

int writeline(int sockfd, char *str, int strlen)
{
	int ret;
	int index = 0;
	int len;
	char buff[MAXLINE];

	len = strnlen(str, strlen);

	if (strlen + 2 > MAXLINE)
		return -1;

	memcpy(buff, str, strlen);
	buff[strlen] = '\r';
	buff[strlen+1] = '\n';		/* strlen+1 <= MAXLINE-1 */

	len = strlen + 2;

	while (len > 0) {
		ret = write(sockfd, buff+index, len);
		if (ret < 0 && errno == EINTR)
			continue;
		if (ret <= 0) {
			return -1;
		}

		len -= ret;
		index += ret;
	}

	return index;
}

=======
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

#include <sysfs/libsysfs.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "usbip_common.h"
#include "utils.h"

int modify_match_busid(char *busid, int add)
{
	char bus_type[] = "usb";
	char attr_name[] = "match_busid";
	char buff[SYSFS_BUS_ID_SIZE + 4];
	char sysfs_mntpath[SYSFS_PATH_MAX];
	char match_busid_attr_path[SYSFS_PATH_MAX];
	struct sysfs_attribute *match_busid_attr;
	int rc, ret = 0;

	if (strnlen(busid, SYSFS_BUS_ID_SIZE) > SYSFS_BUS_ID_SIZE - 1) {
		dbg("busid is too long");
		return -1;
	}

	rc = sysfs_get_mnt_path(sysfs_mntpath, SYSFS_PATH_MAX);
	if (rc < 0) {
		err("sysfs must be mounted: %s", strerror(errno));
		return -1;
	}

	snprintf(match_busid_attr_path, sizeof(match_busid_attr_path),
		 "%s/%s/%s/%s/%s/%s", sysfs_mntpath, SYSFS_BUS_NAME, bus_type,
		 SYSFS_DRIVERS_NAME, USBIP_HOST_DRV_NAME, attr_name);

	match_busid_attr = sysfs_open_attribute(match_busid_attr_path);
	if (!match_busid_attr) {
		dbg("problem getting match_busid attribute: %s",
		    strerror(errno));
		return -1;
	}

	if (add)
		snprintf(buff, SYSFS_BUS_ID_SIZE + 4, "add %s", busid);
	else
		snprintf(buff, SYSFS_BUS_ID_SIZE + 4, "del %s", busid);

	dbg("write \"%s\" to %s", buff, match_busid_attr->path);

	rc = sysfs_write_attribute(match_busid_attr, buff, sizeof(buff));
	if (rc < 0) {
		dbg("failed to write match_busid: %s", strerror(errno));
		ret = -1;
	}

	sysfs_close_attribute(match_busid_attr);

	return ret;
}
>>>>>>> refs/remotes/origin/cm-10.0
