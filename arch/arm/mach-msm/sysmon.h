/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
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
 *
 */

#ifndef __MSM_SYSMON_H
#define __MSM_SYSMON_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <mach/msm_smd.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <mach/msm_smd.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <mach/subsystem_notif.h>

/**
 * enum subsys_id - Destination subsystems for events.
 */
enum subsys_id {
<<<<<<< HEAD
<<<<<<< HEAD
	SYSMON_SS_MODEM,
	SYSMON_SS_LPASS,
	SYSMON_SS_WCNSS,
	SYSMON_SS_DSPS,
	SYSMON_SS_Q6FW,
	SYSMON_NUM_SS
};


/**
 * sysmon_send_event() - Notify a subsystem of another's state change.
 * @dest_ss:	ID of subsystem the notification should be sent to.
 * @event_ss:	String name of the subsystem that generated the notification.
 * @notif:	ID of the notification type (ex. SUBSYS_BEFORE_SHUTDOWN)
 *
 * Returns 0 for success, -EINVAL for invalid destination or notification IDs,
 * -ENODEV if the SMD channel is not open, -ETIMEDOUT if the destination
 * subsystem does not respond, and -ENOSYS if the destination subsystem
 * responds, but with something other than an acknowledgement.
 *
 * If CONFIG_MSM_SYSMON_COMM is not defined, always return success (0).
 */
#ifdef CONFIG_MSM_SYSMON_COMM
int sysmon_send_event(enum subsys_id dest_ss, const char *event_ss,
		      enum subsys_notif_type notif);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* SMD subsystems */
	SYSMON_SS_MODEM     = SMD_APPS_MODEM,
	SYSMON_SS_LPASS     = SMD_APPS_QDSP,
	SYSMON_SS_WCNSS     = SMD_APPS_WCNSS,
	SYSMON_SS_DSPS      = SMD_APPS_DSPS,
	SYSMON_SS_Q6FW      = SMD_APPS_Q6FW,

	/* Non-SMD subsystems */
	SYSMON_SS_EXT_MODEM = SMD_NUM_TYPE,
	SYSMON_NUM_SS
};

#ifdef CONFIG_MSM_SYSMON_COMM
int sysmon_send_event(enum subsys_id dest_ss, const char *event_ss,
		      enum subsys_notif_type notif);
int sysmon_get_reason(enum subsys_id dest_ss, char *buf, size_t len);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#else
static inline int sysmon_send_event(enum subsys_id dest_ss,
				    const char *event_ss,
				    enum subsys_notif_type notif)
{
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static inline int sysmon_get_reason(enum subsys_id dest_ss, char *buf,
				    size_t len)
{
	return 0;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#endif

#endif
