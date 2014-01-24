/******************************************************************************
 *
<<<<<<< HEAD
 * Module Name: utdebug - Debug print routines
=======
 * Module Name: utdebug - Debug print/trace routines
>>>>>>> refs/remotes/origin/master
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

#define _COMPONENT          ACPI_UTILITIES
ACPI_MODULE_NAME("utdebug")
<<<<<<< HEAD
#ifdef ACPI_DEBUG_OUTPUT
static acpi_thread_id acpi_gbl_prev_thread_id;
=======

#ifdef ACPI_DEBUG_OUTPUT
static acpi_thread_id acpi_gbl_prev_thread_id = (acpi_thread_id) 0xFFFFFFFF;
>>>>>>> refs/remotes/origin/master
static char *acpi_gbl_fn_entry_str = "----Entry";
static char *acpi_gbl_fn_exit_str = "----Exit-";

/* Local prototypes */

static const char *acpi_ut_trim_function_name(const char *function_name);

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_init_stack_ptr_trace
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Save the current CPU stack pointer at subsystem startup
 *
 ******************************************************************************/

void acpi_ut_init_stack_ptr_trace(void)
{
	acpi_size current_sp;

	acpi_gbl_entry_stack_pointer = &current_sp;
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_track_stack_ptr
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Save the current CPU stack pointer
 *
 ******************************************************************************/

void acpi_ut_track_stack_ptr(void)
{
	acpi_size current_sp;

	if (&current_sp < acpi_gbl_lowest_stack_pointer) {
		acpi_gbl_lowest_stack_pointer = &current_sp;
	}

	if (acpi_gbl_nesting_level > acpi_gbl_deepest_nesting) {
		acpi_gbl_deepest_nesting = acpi_gbl_nesting_level;
	}
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_trim_function_name
 *
 * PARAMETERS:  function_name       - Ascii string containing a procedure name
 *
 * RETURN:      Updated pointer to the function name
 *
 * DESCRIPTION: Remove the "Acpi" prefix from the function name, if present.
<<<<<<< HEAD
 *              This allows compiler macros such as __func__ to be used
=======
 *              This allows compiler macros such as __FUNCTION__ to be used
>>>>>>> refs/remotes/origin/master
 *              with no change to the debug output.
 *
 ******************************************************************************/

static const char *acpi_ut_trim_function_name(const char *function_name)
{

	/* All Function names are longer than 4 chars, check is safe */

	if (*(ACPI_CAST_PTR(u32, function_name)) == ACPI_PREFIX_MIXED) {

		/* This is the case where the original source has not been modified */

		return (function_name + 4);
	}

	if (*(ACPI_CAST_PTR(u32, function_name)) == ACPI_PREFIX_LOWER) {

		/* This is the case where the source has been 'linuxized' */

		return (function_name + 5);
	}

	return (function_name);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_debug_print
 *
 * PARAMETERS:  requested_debug_level - Requested debug print level
 *              line_number         - Caller's line number (for error output)
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
<<<<<<< HEAD
 *              Format              - Printf format field
=======
 *              format              - Printf format field
>>>>>>> refs/remotes/origin/master
 *              ...                 - Optional printf arguments
 *
 * RETURN:      None
 *
 * DESCRIPTION: Print error message with prefix consisting of the module name,
 *              line number, and component ID.
 *
 ******************************************************************************/

void ACPI_INTERNAL_VAR_XFACE
acpi_debug_print(u32 requested_debug_level,
		 u32 line_number,
		 const char *function_name,
		 const char *module_name,
		 u32 component_id, const char *format, ...)
{
	acpi_thread_id thread_id;
	va_list args;

<<<<<<< HEAD
	/*
	 * Stay silent if the debug level or component ID is disabled
	 */
	if (!(requested_debug_level & acpi_dbg_level) ||
	    !(component_id & acpi_dbg_layer)) {
=======
	/* Check if debug output enabled */

	if (!ACPI_IS_DEBUG_ENABLED(requested_debug_level, component_id)) {
>>>>>>> refs/remotes/origin/master
		return;
	}

	/*
	 * Thread tracking and context switch notification
	 */
	thread_id = acpi_os_get_thread_id();
	if (thread_id != acpi_gbl_prev_thread_id) {
		if (ACPI_LV_THREADS & acpi_dbg_level) {
			acpi_os_printf
			    ("\n**** Context Switch from TID %u to TID %u ****\n\n",
			     (u32)acpi_gbl_prev_thread_id, (u32)thread_id);
		}

		acpi_gbl_prev_thread_id = thread_id;
<<<<<<< HEAD
=======
		acpi_gbl_nesting_level = 0;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Display the module name, current line number, thread ID (if requested),
	 * current procedure nesting level, and the current procedure name
	 */
<<<<<<< HEAD
	acpi_os_printf("%8s-%04ld ", module_name, line_number);

=======
	acpi_os_printf("%9s-%04ld ", module_name, line_number);

#ifdef ACPI_EXEC_APP
	/*
	 * For acpi_exec only, emit the thread ID and nesting level.
	 * Note: nesting level is really only useful during a single-thread
	 * execution. Otherwise, multiple threads will keep resetting the
	 * level.
	 */
>>>>>>> refs/remotes/origin/master
	if (ACPI_LV_THREADS & acpi_dbg_level) {
		acpi_os_printf("[%u] ", (u32)thread_id);
	}

<<<<<<< HEAD
	acpi_os_printf("[%02ld] %-22.22s: ",
		       acpi_gbl_nesting_level,
		       acpi_ut_trim_function_name(function_name));
=======
	acpi_os_printf("[%02ld] ", acpi_gbl_nesting_level);
#endif

	acpi_os_printf("%-22.22s: ", acpi_ut_trim_function_name(function_name));
>>>>>>> refs/remotes/origin/master

	va_start(args, format);
	acpi_os_vprintf(format, args);
	va_end(args);
}

ACPI_EXPORT_SYMBOL(acpi_debug_print)

/*******************************************************************************
 *
 * FUNCTION:    acpi_debug_print_raw
 *
 * PARAMETERS:  requested_debug_level - Requested debug print level
 *              line_number         - Caller's line number
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
<<<<<<< HEAD
 *              Format              - Printf format field
=======
 *              format              - Printf format field
>>>>>>> refs/remotes/origin/master
 *              ...                 - Optional printf arguments
 *
 * RETURN:      None
 *
<<<<<<< HEAD
 * DESCRIPTION: Print message with no headers.  Has same interface as
=======
 * DESCRIPTION: Print message with no headers. Has same interface as
>>>>>>> refs/remotes/origin/master
 *              debug_print so that the same macros can be used.
 *
 ******************************************************************************/
void ACPI_INTERNAL_VAR_XFACE
acpi_debug_print_raw(u32 requested_debug_level,
		     u32 line_number,
		     const char *function_name,
		     const char *module_name,
		     u32 component_id, const char *format, ...)
{
	va_list args;

<<<<<<< HEAD
	if (!(requested_debug_level & acpi_dbg_level) ||
	    !(component_id & acpi_dbg_layer)) {
=======
	/* Check if debug output enabled */

	if (!ACPI_IS_DEBUG_ENABLED(requested_debug_level, component_id)) {
>>>>>>> refs/remotes/origin/master
		return;
	}

	va_start(args, format);
	acpi_os_vprintf(format, args);
	va_end(args);
}

ACPI_EXPORT_SYMBOL(acpi_debug_print_raw)

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_trace
 *
 * PARAMETERS:  line_number         - Caller's line number
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
 *
 * RETURN:      None
 *
<<<<<<< HEAD
 * DESCRIPTION: Function entry trace.  Prints only if TRACE_FUNCTIONS bit is
=======
 * DESCRIPTION: Function entry trace. Prints only if TRACE_FUNCTIONS bit is
>>>>>>> refs/remotes/origin/master
 *              set in debug_level
 *
 ******************************************************************************/
void
acpi_ut_trace(u32 line_number,
	      const char *function_name,
	      const char *module_name, u32 component_id)
{

	acpi_gbl_nesting_level++;
	acpi_ut_track_stack_ptr();

<<<<<<< HEAD
	acpi_debug_print(ACPI_LV_FUNCTIONS,
			 line_number, function_name, module_name, component_id,
			 "%s\n", acpi_gbl_fn_entry_str);
=======
	/* Check if enabled up-front for performance */

	if (ACPI_IS_DEBUG_ENABLED(ACPI_LV_FUNCTIONS, component_id)) {
		acpi_debug_print(ACPI_LV_FUNCTIONS,
				 line_number, function_name, module_name,
				 component_id, "%s\n", acpi_gbl_fn_entry_str);
	}
>>>>>>> refs/remotes/origin/master
}

ACPI_EXPORT_SYMBOL(acpi_ut_trace)

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_trace_ptr
 *
 * PARAMETERS:  line_number         - Caller's line number
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
<<<<<<< HEAD
 *              Pointer             - Pointer to display
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function entry trace.  Prints only if TRACE_FUNCTIONS bit is
=======
 *              pointer             - Pointer to display
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function entry trace. Prints only if TRACE_FUNCTIONS bit is
>>>>>>> refs/remotes/origin/master
 *              set in debug_level
 *
 ******************************************************************************/
void
acpi_ut_trace_ptr(u32 line_number,
		  const char *function_name,
		  const char *module_name, u32 component_id, void *pointer)
{
<<<<<<< HEAD
	acpi_gbl_nesting_level++;
	acpi_ut_track_stack_ptr();

	acpi_debug_print(ACPI_LV_FUNCTIONS,
			 line_number, function_name, module_name, component_id,
			 "%s %p\n", acpi_gbl_fn_entry_str, pointer);
=======

	acpi_gbl_nesting_level++;
	acpi_ut_track_stack_ptr();

	/* Check if enabled up-front for performance */

	if (ACPI_IS_DEBUG_ENABLED(ACPI_LV_FUNCTIONS, component_id)) {
		acpi_debug_print(ACPI_LV_FUNCTIONS,
				 line_number, function_name, module_name,
				 component_id, "%s %p\n", acpi_gbl_fn_entry_str,
				 pointer);
	}
>>>>>>> refs/remotes/origin/master
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_trace_str
 *
 * PARAMETERS:  line_number         - Caller's line number
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
<<<<<<< HEAD
 *              String              - Additional string to display
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function entry trace.  Prints only if TRACE_FUNCTIONS bit is
=======
 *              string              - Additional string to display
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function entry trace. Prints only if TRACE_FUNCTIONS bit is
>>>>>>> refs/remotes/origin/master
 *              set in debug_level
 *
 ******************************************************************************/

void
acpi_ut_trace_str(u32 line_number,
		  const char *function_name,
		  const char *module_name, u32 component_id, char *string)
{

	acpi_gbl_nesting_level++;
	acpi_ut_track_stack_ptr();

<<<<<<< HEAD
	acpi_debug_print(ACPI_LV_FUNCTIONS,
			 line_number, function_name, module_name, component_id,
			 "%s %s\n", acpi_gbl_fn_entry_str, string);
=======
	/* Check if enabled up-front for performance */

	if (ACPI_IS_DEBUG_ENABLED(ACPI_LV_FUNCTIONS, component_id)) {
		acpi_debug_print(ACPI_LV_FUNCTIONS,
				 line_number, function_name, module_name,
				 component_id, "%s %s\n", acpi_gbl_fn_entry_str,
				 string);
	}
>>>>>>> refs/remotes/origin/master
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_trace_u32
 *
 * PARAMETERS:  line_number         - Caller's line number
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
<<<<<<< HEAD
 *              Integer             - Integer to display
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function entry trace.  Prints only if TRACE_FUNCTIONS bit is
=======
 *              integer             - Integer to display
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function entry trace. Prints only if TRACE_FUNCTIONS bit is
>>>>>>> refs/remotes/origin/master
 *              set in debug_level
 *
 ******************************************************************************/

void
acpi_ut_trace_u32(u32 line_number,
		  const char *function_name,
		  const char *module_name, u32 component_id, u32 integer)
{

	acpi_gbl_nesting_level++;
	acpi_ut_track_stack_ptr();

<<<<<<< HEAD
	acpi_debug_print(ACPI_LV_FUNCTIONS,
			 line_number, function_name, module_name, component_id,
			 "%s %08X\n", acpi_gbl_fn_entry_str, integer);
=======
	/* Check if enabled up-front for performance */

	if (ACPI_IS_DEBUG_ENABLED(ACPI_LV_FUNCTIONS, component_id)) {
		acpi_debug_print(ACPI_LV_FUNCTIONS,
				 line_number, function_name, module_name,
				 component_id, "%s %08X\n",
				 acpi_gbl_fn_entry_str, integer);
	}
>>>>>>> refs/remotes/origin/master
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_exit
 *
 * PARAMETERS:  line_number         - Caller's line number
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
 *
 * RETURN:      None
 *
<<<<<<< HEAD
 * DESCRIPTION: Function exit trace.  Prints only if TRACE_FUNCTIONS bit is
=======
 * DESCRIPTION: Function exit trace. Prints only if TRACE_FUNCTIONS bit is
>>>>>>> refs/remotes/origin/master
 *              set in debug_level
 *
 ******************************************************************************/

void
acpi_ut_exit(u32 line_number,
	     const char *function_name,
	     const char *module_name, u32 component_id)
{

<<<<<<< HEAD
	acpi_debug_print(ACPI_LV_FUNCTIONS,
			 line_number, function_name, module_name, component_id,
			 "%s\n", acpi_gbl_fn_exit_str);

	acpi_gbl_nesting_level--;
=======
	/* Check if enabled up-front for performance */

	if (ACPI_IS_DEBUG_ENABLED(ACPI_LV_FUNCTIONS, component_id)) {
		acpi_debug_print(ACPI_LV_FUNCTIONS,
				 line_number, function_name, module_name,
				 component_id, "%s\n", acpi_gbl_fn_exit_str);
	}

	if (acpi_gbl_nesting_level) {
		acpi_gbl_nesting_level--;
	}
>>>>>>> refs/remotes/origin/master
}

ACPI_EXPORT_SYMBOL(acpi_ut_exit)

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_status_exit
 *
 * PARAMETERS:  line_number         - Caller's line number
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
<<<<<<< HEAD
 *              Status              - Exit status code
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function exit trace.  Prints only if TRACE_FUNCTIONS bit is
=======
 *              status              - Exit status code
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function exit trace. Prints only if TRACE_FUNCTIONS bit is
>>>>>>> refs/remotes/origin/master
 *              set in debug_level. Prints exit status also.
 *
 ******************************************************************************/
void
acpi_ut_status_exit(u32 line_number,
		    const char *function_name,
		    const char *module_name,
		    u32 component_id, acpi_status status)
{

<<<<<<< HEAD
	if (ACPI_SUCCESS(status)) {
		acpi_debug_print(ACPI_LV_FUNCTIONS,
				 line_number, function_name, module_name,
				 component_id, "%s %s\n", acpi_gbl_fn_exit_str,
				 acpi_format_exception(status));
	} else {
		acpi_debug_print(ACPI_LV_FUNCTIONS,
				 line_number, function_name, module_name,
				 component_id, "%s ****Exception****: %s\n",
				 acpi_gbl_fn_exit_str,
				 acpi_format_exception(status));
	}

	acpi_gbl_nesting_level--;
=======
	/* Check if enabled up-front for performance */

	if (ACPI_IS_DEBUG_ENABLED(ACPI_LV_FUNCTIONS, component_id)) {
		if (ACPI_SUCCESS(status)) {
			acpi_debug_print(ACPI_LV_FUNCTIONS,
					 line_number, function_name,
					 module_name, component_id, "%s %s\n",
					 acpi_gbl_fn_exit_str,
					 acpi_format_exception(status));
		} else {
			acpi_debug_print(ACPI_LV_FUNCTIONS,
					 line_number, function_name,
					 module_name, component_id,
					 "%s ****Exception****: %s\n",
					 acpi_gbl_fn_exit_str,
					 acpi_format_exception(status));
		}
	}

	if (acpi_gbl_nesting_level) {
		acpi_gbl_nesting_level--;
	}
>>>>>>> refs/remotes/origin/master
}

ACPI_EXPORT_SYMBOL(acpi_ut_status_exit)

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_value_exit
 *
 * PARAMETERS:  line_number         - Caller's line number
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
<<<<<<< HEAD
 *              Value               - Value to be printed with exit msg
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function exit trace.  Prints only if TRACE_FUNCTIONS bit is
=======
 *              value               - Value to be printed with exit msg
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function exit trace. Prints only if TRACE_FUNCTIONS bit is
>>>>>>> refs/remotes/origin/master
 *              set in debug_level. Prints exit value also.
 *
 ******************************************************************************/
void
acpi_ut_value_exit(u32 line_number,
		   const char *function_name,
		   const char *module_name, u32 component_id, u64 value)
{

<<<<<<< HEAD
	acpi_debug_print(ACPI_LV_FUNCTIONS,
			 line_number, function_name, module_name, component_id,
			 "%s %8.8X%8.8X\n", acpi_gbl_fn_exit_str,
			 ACPI_FORMAT_UINT64(value));

	acpi_gbl_nesting_level--;
=======
	/* Check if enabled up-front for performance */

	if (ACPI_IS_DEBUG_ENABLED(ACPI_LV_FUNCTIONS, component_id)) {
		acpi_debug_print(ACPI_LV_FUNCTIONS,
				 line_number, function_name, module_name,
				 component_id, "%s %8.8X%8.8X\n",
				 acpi_gbl_fn_exit_str,
				 ACPI_FORMAT_UINT64(value));
	}

	if (acpi_gbl_nesting_level) {
		acpi_gbl_nesting_level--;
	}
>>>>>>> refs/remotes/origin/master
}

ACPI_EXPORT_SYMBOL(acpi_ut_value_exit)

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_ptr_exit
 *
 * PARAMETERS:  line_number         - Caller's line number
 *              function_name       - Caller's procedure name
 *              module_name         - Caller's module name
 *              component_id        - Caller's component ID
<<<<<<< HEAD
 *              Ptr                 - Pointer to display
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function exit trace.  Prints only if TRACE_FUNCTIONS bit is
=======
 *              ptr                 - Pointer to display
 *
 * RETURN:      None
 *
 * DESCRIPTION: Function exit trace. Prints only if TRACE_FUNCTIONS bit is
>>>>>>> refs/remotes/origin/master
 *              set in debug_level. Prints exit value also.
 *
 ******************************************************************************/
void
acpi_ut_ptr_exit(u32 line_number,
		 const char *function_name,
		 const char *module_name, u32 component_id, u8 *ptr)
{

<<<<<<< HEAD
	acpi_debug_print(ACPI_LV_FUNCTIONS,
			 line_number, function_name, module_name, component_id,
			 "%s %p\n", acpi_gbl_fn_exit_str, ptr);

	acpi_gbl_nesting_level--;
}

#endif

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_dump_buffer
 *
 * PARAMETERS:  Buffer              - Buffer to dump
 *              Count               - Amount to dump, in bytes
 *              Display             - BYTE, WORD, DWORD, or QWORD display
 *              component_iD        - Caller's component ID
 *
 * RETURN:      None
 *
 * DESCRIPTION: Generic dump buffer in both hex and ascii.
 *
 ******************************************************************************/

void acpi_ut_dump_buffer2(u8 * buffer, u32 count, u32 display)
{
	u32 i = 0;
	u32 j;
	u32 temp32;
	u8 buf_char;

	if (!buffer) {
		acpi_os_printf("Null Buffer Pointer in DumpBuffer!\n");
		return;
	}

	if ((count < 4) || (count & 0x01)) {
		display = DB_BYTE_DISPLAY;
	}

	/* Nasty little dump buffer routine! */

	while (i < count) {

		/* Print current offset */

		acpi_os_printf("%6.4X: ", i);

		/* Print 16 hex chars */

		for (j = 0; j < 16;) {
			if (i + j >= count) {

				/* Dump fill spaces */

				acpi_os_printf("%*s", ((display * 2) + 1), " ");
				j += display;
				continue;
			}

			switch (display) {
			case DB_BYTE_DISPLAY:
			default:	/* Default is BYTE display */

				acpi_os_printf("%02X ",
					       buffer[(acpi_size) i + j]);
				break;

			case DB_WORD_DISPLAY:

				ACPI_MOVE_16_TO_32(&temp32,
						   &buffer[(acpi_size) i + j]);
				acpi_os_printf("%04X ", temp32);
				break;

			case DB_DWORD_DISPLAY:

				ACPI_MOVE_32_TO_32(&temp32,
						   &buffer[(acpi_size) i + j]);
				acpi_os_printf("%08X ", temp32);
				break;

			case DB_QWORD_DISPLAY:

				ACPI_MOVE_32_TO_32(&temp32,
						   &buffer[(acpi_size) i + j]);
				acpi_os_printf("%08X", temp32);

				ACPI_MOVE_32_TO_32(&temp32,
						   &buffer[(acpi_size) i + j +
							   4]);
				acpi_os_printf("%08X ", temp32);
				break;
			}

			j += display;
		}

		/*
		 * Print the ASCII equivalent characters but watch out for the bad
		 * unprintable ones (printable chars are 0x20 through 0x7E)
		 */
		acpi_os_printf(" ");
		for (j = 0; j < 16; j++) {
			if (i + j >= count) {
				acpi_os_printf("\n");
				return;
			}

			buf_char = buffer[(acpi_size) i + j];
			if (ACPI_IS_PRINT(buf_char)) {
				acpi_os_printf("%c", buf_char);
			} else {
				acpi_os_printf(".");
			}
		}

		/* Done with that line. */

		acpi_os_printf("\n");
		i += 16;
	}

	return;
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_dump_buffer
 *
 * PARAMETERS:  Buffer              - Buffer to dump
 *              Count               - Amount to dump, in bytes
 *              Display             - BYTE, WORD, DWORD, or QWORD display
 *              component_iD        - Caller's component ID
 *
 * RETURN:      None
 *
 * DESCRIPTION: Generic dump buffer in both hex and ascii.
 *
 ******************************************************************************/

void acpi_ut_dump_buffer(u8 * buffer, u32 count, u32 display, u32 component_id)
{

	/* Only dump the buffer if tracing is enabled */

	if (!((ACPI_LV_TABLES & acpi_dbg_level) &&
	      (component_id & acpi_dbg_layer))) {
		return;
	}

	acpi_ut_dump_buffer2(buffer, count, display);
}
=======
	/* Check if enabled up-front for performance */

	if (ACPI_IS_DEBUG_ENABLED(ACPI_LV_FUNCTIONS, component_id)) {
		acpi_debug_print(ACPI_LV_FUNCTIONS,
				 line_number, function_name, module_name,
				 component_id, "%s %p\n", acpi_gbl_fn_exit_str,
				 ptr);
	}

	if (acpi_gbl_nesting_level) {
		acpi_gbl_nesting_level--;
	}
}

#endif
>>>>>>> refs/remotes/origin/master
