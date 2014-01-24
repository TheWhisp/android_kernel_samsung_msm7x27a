<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/******************************************************************************
 *
 * Name: hwtimer.c - ACPI Power Management Timer Interface
 *
 *****************************************************************************/

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 2000 - 2011, Intel Corp.
=======
 * Copyright (C) 2000 - 2012, Intel Corp.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 2000 - 2013, Intel Corp.
>>>>>>> refs/remotes/origin/master
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define EXPORT_ACPI_INTERFACES

>>>>>>> refs/remotes/origin/master
#include <acpi/acpi.h>
#include "accommon.h"

#define _COMPONENT          ACPI_HARDWARE
ACPI_MODULE_NAME("hwtimer")

<<<<<<< HEAD
<<<<<<< HEAD
=======
#if (!ACPI_REDUCED_HARDWARE)	/* Entire module */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if (!ACPI_REDUCED_HARDWARE)	/* Entire module */
>>>>>>> refs/remotes/origin/master
/******************************************************************************
 *
 * FUNCTION:    acpi_get_timer_resolution
 *
<<<<<<< HEAD
 * PARAMETERS:  Resolution          - Where the resolution is returned
=======
 * PARAMETERS:  resolution          - Where the resolution is returned
>>>>>>> refs/remotes/origin/master
 *
 * RETURN:      Status and timer resolution
 *
 * DESCRIPTION: Obtains resolution of the ACPI PM Timer (24 or 32 bits).
 *
 ******************************************************************************/
acpi_status acpi_get_timer_resolution(u32 * resolution)
{
	ACPI_FUNCTION_TRACE(acpi_get_timer_resolution);

	if (!resolution) {
		return_ACPI_STATUS(AE_BAD_PARAMETER);
	}

	if ((acpi_gbl_FADT.flags & ACPI_FADT_32BIT_TIMER) == 0) {
		*resolution = 24;
	} else {
		*resolution = 32;
	}

	return_ACPI_STATUS(AE_OK);
}

ACPI_EXPORT_SYMBOL(acpi_get_timer_resolution)

/******************************************************************************
 *
 * FUNCTION:    acpi_get_timer
 *
<<<<<<< HEAD
 * PARAMETERS:  Ticks               - Where the timer value is returned
=======
 * PARAMETERS:  ticks               - Where the timer value is returned
>>>>>>> refs/remotes/origin/master
 *
 * RETURN:      Status and current timer value (ticks)
 *
 * DESCRIPTION: Obtains current value of ACPI PM Timer (in ticks).
 *
 ******************************************************************************/
acpi_status acpi_get_timer(u32 * ticks)
{
	acpi_status status;

	ACPI_FUNCTION_TRACE(acpi_get_timer);

	if (!ticks) {
		return_ACPI_STATUS(AE_BAD_PARAMETER);
	}

<<<<<<< HEAD
	status =
	    acpi_hw_read(ticks, &acpi_gbl_FADT.xpm_timer_block);

=======
	/* ACPI 5.0A: PM Timer is optional */

	if (!acpi_gbl_FADT.xpm_timer_block.address) {
		return_ACPI_STATUS(AE_SUPPORT);
	}

	status = acpi_hw_read(ticks, &acpi_gbl_FADT.xpm_timer_block);
>>>>>>> refs/remotes/origin/master
	return_ACPI_STATUS(status);
}

ACPI_EXPORT_SYMBOL(acpi_get_timer)

/******************************************************************************
 *
 * FUNCTION:    acpi_get_timer_duration
 *
 * PARAMETERS:  start_ticks         - Starting timestamp
 *              end_ticks           - End timestamp
 *              time_elapsed        - Where the elapsed time is returned
 *
 * RETURN:      Status and time_elapsed
 *
 * DESCRIPTION: Computes the time elapsed (in microseconds) between two
 *              PM Timer time stamps, taking into account the possibility of
 *              rollovers, the timer resolution, and timer frequency.
 *
 *              The PM Timer's clock ticks at roughly 3.6 times per
 *              _microsecond_, and its clock continues through Cx state
 *              transitions (unlike many CPU timestamp counters) -- making it
 *              a versatile and accurate timer.
 *
 *              Note that this function accommodates only a single timer
<<<<<<< HEAD
 *              rollover.  Thus for 24-bit timers, this function should only
=======
 *              rollover. Thus for 24-bit timers, this function should only
>>>>>>> refs/remotes/origin/master
 *              be used for calculating durations less than ~4.6 seconds
 *              (~20 minutes for 32-bit timers) -- calculations below:
 *
 *              2**24 Ticks / 3,600,000 Ticks/Sec = 4.66 sec
 *              2**32 Ticks / 3,600,000 Ticks/Sec = 1193 sec or 19.88 minutes
 *
 ******************************************************************************/
acpi_status
acpi_get_timer_duration(u32 start_ticks, u32 end_ticks, u32 * time_elapsed)
{
	acpi_status status;
	u32 delta_ticks;
	u64 quotient;

	ACPI_FUNCTION_TRACE(acpi_get_timer_duration);

	if (!time_elapsed) {
		return_ACPI_STATUS(AE_BAD_PARAMETER);
	}

<<<<<<< HEAD
=======
	/* ACPI 5.0A: PM Timer is optional */

	if (!acpi_gbl_FADT.xpm_timer_block.address) {
		return_ACPI_STATUS(AE_SUPPORT);
	}

>>>>>>> refs/remotes/origin/master
	/*
	 * Compute Tick Delta:
	 * Handle (max one) timer rollovers on 24-bit versus 32-bit timers.
	 */
	if (start_ticks < end_ticks) {
		delta_ticks = end_ticks - start_ticks;
	} else if (start_ticks > end_ticks) {
		if ((acpi_gbl_FADT.flags & ACPI_FADT_32BIT_TIMER) == 0) {

			/* 24-bit Timer */

			delta_ticks =
			    (((0x00FFFFFF - start_ticks) +
			      end_ticks) & 0x00FFFFFF);
		} else {
			/* 32-bit Timer */

			delta_ticks = (0xFFFFFFFF - start_ticks) + end_ticks;
		}
	} else {		/* start_ticks == end_ticks */

		*time_elapsed = 0;
		return_ACPI_STATUS(AE_OK);
	}

	/*
	 * Compute Duration (Requires a 64-bit multiply and divide):
	 *
<<<<<<< HEAD
	 * time_elapsed = (delta_ticks * 1000000) / PM_TIMER_FREQUENCY;
	 */
	status = acpi_ut_short_divide(((u64) delta_ticks) * 1000000,
				      PM_TIMER_FREQUENCY, &quotient, NULL);
=======
	 * time_elapsed (microseconds) =
	 *  (delta_ticks * ACPI_USEC_PER_SEC) / ACPI_PM_TIMER_FREQUENCY;
	 */
	status = acpi_ut_short_divide(((u64)delta_ticks) * ACPI_USEC_PER_SEC,
				      ACPI_PM_TIMER_FREQUENCY, &quotient, NULL);
>>>>>>> refs/remotes/origin/master

	*time_elapsed = (u32) quotient;
	return_ACPI_STATUS(status);
}

ACPI_EXPORT_SYMBOL(acpi_get_timer_duration)
<<<<<<< HEAD
<<<<<<< HEAD
=======
#endif				/* !ACPI_REDUCED_HARDWARE */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif				/* !ACPI_REDUCED_HARDWARE */
>>>>>>> refs/remotes/origin/master
