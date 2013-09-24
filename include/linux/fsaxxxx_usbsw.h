/* linux/fsa_microusb.h
 *
 * header for FSA9480, FSA88x USB switch device.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#ifndef _FSA_MICROUSB_H_
#define _FSA_MICROUSB_H_

#include <linux/types.h>

#define FSA9480_ATTACHED (1)
#define FSA9480_DETACHED (0)
#define FSA_ATTACHED	FSA9480_ATTACHED
#define FSA_DETACHED	FSA9480_DETACHED

#define USB_PATH_NA		-1
#define USB_PATH_AP		0
#define USB_PATH_CP		1

#define USB_POWER_AP	"USB_AP"
#define USB_POWER_CP	"USB_CP"

struct fsausb_ops {
	void (*attach_handler)(int);
	void (*detach_handler)(void);
};

struct fsausb_platform_data {
       int intb_gpio;
       void (*usb_cb) (u8 attached, struct fsausb_ops *ops);
       void (*uart_cb) (u8 attached, struct fsausb_ops *ops);
       void (*charger_cb) (u8 attached, struct fsausb_ops *ops);
       void (*jig_cb) (u8 attached, struct fsausb_ops *ops);
	void (*ovp_cb) (u8 attached, struct fsausb_ops *ops);
       void (*reset_cb) (void);
};

void fsausb_set_switch(const char *buf);
ssize_t fsausb_get_switch(char *buf);
int fsa_microusb_connection_handler_register(
		void (*detach), void (*attach));

#endif /* _FSA_MICROUSB_H_ */

