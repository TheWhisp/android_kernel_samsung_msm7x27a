/******************************************************************************
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright(c) 2009 - 2011 Intel Corporation. All rights reserved.
=======
 * Copyright(c) 2009 - 2012 Intel Corporation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright(c) 2009 - 2013 Intel Corporation. All rights reserved.
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 *  Intel Linux Wireless <ilw@linux.intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 *****************************************************************************/

#include <linux/module.h>

/* sparse doesn't like tracepoint macros */
#ifndef __CHECKER__
<<<<<<< HEAD
<<<<<<< HEAD
#include "iwl-dev.h"
=======
#include "iwl-trans.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "iwl-trans.h"
>>>>>>> refs/remotes/origin/master

#define CREATE_TRACE_POINTS
#include "iwl-devtrace.h"

EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dev_iowrite8);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dev_ioread32);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dev_iowrite32);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dev_rx);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dev_tx);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dev_ucode_event);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dev_ucode_error);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dev_ucode_cont_event);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dev_ucode_wrap_event);
<<<<<<< HEAD
=======
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_info);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_warn);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_crit);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_err);
EXPORT_TRACEPOINT_SYMBOL(iwlwifi_dbg);
>>>>>>> refs/remotes/origin/master
#endif
