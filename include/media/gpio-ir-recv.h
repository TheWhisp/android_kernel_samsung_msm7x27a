<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __GPIO_IR_RECV_H__
#define __GPIO_IR_RECV_H__

struct gpio_ir_recv_platform_data {
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int gpio_nr;
	bool active_low;
	bool can_wakeup;
=======
	int		gpio_nr;
	bool		active_low;
	u64		allowed_protos;
	const char	*map_name;
>>>>>>> refs/remotes/origin/master
=======
	unsigned int gpio_nr;
	bool active_low;
	bool can_wakeup;
>>>>>>> refs/remotes/origin/cm-11.0
};

#endif /* __GPIO_IR_RECV_H__ */

