/******************************************************************************
 *
<<<<<<< HEAD
 * Name: acenv.h - Generation environment specific items
=======
 * Name: acenv.h - Host and compiler configuration
>>>>>>> refs/remotes/origin/master
 *
 *****************************************************************************/

/*
<<<<<<< HEAD
 * Copyright (C) 2000 - 2011, Intel Corp.
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

#ifndef __ACENV_H__
#define __ACENV_H__

<<<<<<< HEAD
=======
/*
 * Environment configuration. The purpose of this file is to interface ACPICA
 * to the local environment. This includes compiler-specific, OS-specific,
 * and machine-specific configuration.
 */

>>>>>>> refs/remotes/origin/master
/* Types for ACPI_MUTEX_TYPE */

#define ACPI_BINARY_SEMAPHORE       0
#define ACPI_OSL_MUTEX              1

/* Types for DEBUGGER_THREADING */

#define DEBUGGER_SINGLE_THREADED    0
#define DEBUGGER_MULTI_THREADED     1

/******************************************************************************
 *
 * Configuration for ACPI tools and utilities
 *
 *****************************************************************************/

<<<<<<< HEAD
#ifdef ACPI_LIBRARY
/*
 * Note: The non-debug version of the acpi_library does not contain any
 * debug support, for minimal size. The debug version uses ACPI_FULL_DEBUG
 */
#define ACPI_USE_LOCAL_CACHE
#endif

#ifdef ACPI_ASL_COMPILER
#define ACPI_DEBUG_OUTPUT
#define ACPI_APPLICATION
#define ACPI_DISASSEMBLER
#define ACPI_CONSTANT_EVAL_ONLY
#define ACPI_LARGE_NAMESPACE_NODE
#define ACPI_DATA_TABLE_DISASSEMBLY
#endif

#ifdef ACPI_EXEC_APP
#undef DEBUGGER_THREADING
#define DEBUGGER_THREADING      DEBUGGER_SINGLE_THREADED
#define ACPI_FULL_DEBUG
#define ACPI_APPLICATION
#define ACPI_DEBUGGER
=======
/* iASL configuration */

#ifdef ACPI_ASL_COMPILER
#define ACPI_APPLICATION
#define ACPI_DISASSEMBLER
#define ACPI_DEBUG_OUTPUT
#define ACPI_CONSTANT_EVAL_ONLY
#define ACPI_LARGE_NAMESPACE_NODE
#define ACPI_DATA_TABLE_DISASSEMBLY
#define ACPI_SINGLE_THREADED
#endif

/* acpi_exec configuration. Multithreaded with full AML debugger */

#ifdef ACPI_EXEC_APP
#define ACPI_APPLICATION
#define ACPI_FULL_DEBUG
>>>>>>> refs/remotes/origin/master
#define ACPI_MUTEX_DEBUG
#define ACPI_DBG_TRACK_ALLOCATIONS
#endif

<<<<<<< HEAD
=======
/* acpi_names configuration. Single threaded with debugger output enabled. */

#ifdef ACPI_NAMES_APP
#define ACPI_DEBUGGER
#define ACPI_APPLICATION
#define ACPI_SINGLE_THREADED
#endif

/*
 * acpi_bin/acpi_dump/acpi_src/acpi_xtract configuration. All single
 * threaded, with no debug output.
 */
#if (defined ACPI_BIN_APP)   || \
	(defined ACPI_DUMP_APP)  || \
	(defined ACPI_SRC_APP)   || \
	(defined ACPI_XTRACT_APP)
#define ACPI_APPLICATION
#define ACPI_SINGLE_THREADED
#endif

#ifdef ACPI_HELP_APP
#define ACPI_APPLICATION
#define ACPI_SINGLE_THREADED
#define ACPI_NO_ERROR_MESSAGES
#endif

/* Linkable ACPICA library */

#ifdef ACPI_LIBRARY
#define ACPI_USE_LOCAL_CACHE
#define ACPI_FUTURE_USAGE
#endif

/* Common for all ACPICA applications */

>>>>>>> refs/remotes/origin/master
#ifdef ACPI_APPLICATION
#define ACPI_USE_SYSTEM_CLIBRARY
#define ACPI_USE_LOCAL_CACHE
#endif

<<<<<<< HEAD
=======
/* Common debug support */

>>>>>>> refs/remotes/origin/master
#ifdef ACPI_FULL_DEBUG
#define ACPI_DEBUGGER
#define ACPI_DEBUG_OUTPUT
#define ACPI_DISASSEMBLER
#endif

<<<<<<< HEAD
/*
 * Environment configuration.  The purpose of this file is to interface to the
 * local generation environment.
 *
 * 1) ACPI_USE_SYSTEM_CLIBRARY - Define this if linking to an actual C library.
 *      Otherwise, local versions of string/memory functions will be used.
 * 2) ACPI_USE_STANDARD_HEADERS - Define this if linking to a C library and
 *      the standard header files may be used.
 *
 * The ACPI subsystem only uses low level C library functions that do not call
 * operating system services and may therefore be inlined in the code.
 *
 * It may be necessary to tailor these include files to the target
 * generation environment.
 *
 *
 * Functions and constants used from each header:
 *
 * string.h:    memcpy
 *              memset
 *              strcat
 *              strcmp
 *              strcpy
 *              strlen
 *              strncmp
 *              strncat
 *              strncpy
 *
 * stdlib.h:    strtoul
 *
 * stdarg.h:    va_list
 *              va_arg
 *              va_start
 *              va_end
 *
 */

/*! [Begin] no source code translation */

#if defined(_LINUX) || defined(__linux__)
#include "aclinux.h"

#elif defined(_AED_EFI)
#include "acefi.h"

#elif defined(WIN32)
#include "acwin.h"

#elif defined(WIN64)
#include "acwin64.h"

#elif defined(MSDOS)		/* Must appear after WIN32 and WIN64 check */
#include "acdos16.h"
=======

/*! [Begin] no source code translation */

/******************************************************************************
 *
 * Host configuration files. The compiler configuration files are included
 * by the host files.
 *
 *****************************************************************************/

#if defined(_LINUX) || defined(__linux__)
#include <acpi/platform/aclinux.h>

#elif defined(_APPLE) || defined(__APPLE__)
#include "acmacosx.h"
>>>>>>> refs/remotes/origin/master

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#include "acfreebsd.h"

#elif defined(__NetBSD__)
#include "acnetbsd.h"

<<<<<<< HEAD
=======
#elif defined(__sun)
#include "acsolaris.h"

>>>>>>> refs/remotes/origin/master
#elif defined(MODESTO)
#include "acmodesto.h"

#elif defined(NETWARE)
#include "acnetware.h"

<<<<<<< HEAD
#elif defined(__sun)
#include "acsolaris.h"

#else

/* All other environments */

#define ACPI_USE_STANDARD_HEADERS

#define COMPILER_DEPENDENT_INT64   long long
#define COMPILER_DEPENDENT_UINT64  unsigned long long

=======
#elif defined(_CYGWIN)
#include "accygwin.h"

#elif defined(WIN32)
#include "acwin.h"

#elif defined(WIN64)
#include "acwin64.h"

#elif defined(_WRS_LIB_BUILD)
#include "acvxworks.h"

#elif defined(__OS2__)
#include "acos2.h"

#elif defined(_AED_EFI)
#include "acefi.h"

#elif defined(__HAIKU__)
#include "achaiku.h"

#else

/* Unknown environment */

#error Unknown target environment
>>>>>>> refs/remotes/origin/master
#endif

/*! [End] no source code translation !*/

/******************************************************************************
 *
<<<<<<< HEAD
 * Miscellaneous configuration
 *
 *****************************************************************************/

/*
 * Are mutexes supported by the host? default is no, use binary semaphores.
 */
=======
 * Setup defaults for the required symbols that were not defined in one of
 * the host/compiler files above.
 *
 *****************************************************************************/

/* 64-bit data types */

#ifndef COMPILER_DEPENDENT_INT64
#define COMPILER_DEPENDENT_INT64   long long
#endif

#ifndef COMPILER_DEPENDENT_UINT64
#define COMPILER_DEPENDENT_UINT64  unsigned long long
#endif

/* Type of mutex supported by host. Default is binary semaphores. */
>>>>>>> refs/remotes/origin/master
#ifndef ACPI_MUTEX_TYPE
#define ACPI_MUTEX_TYPE             ACPI_BINARY_SEMAPHORE
#endif

<<<<<<< HEAD
=======
/* Global Lock acquire/release */

#ifndef ACPI_ACQUIRE_GLOBAL_LOCK
#define ACPI_ACQUIRE_GLOBAL_LOCK(Glptr, acquired) acquired = 1
#endif

#ifndef ACPI_RELEASE_GLOBAL_LOCK
#define ACPI_RELEASE_GLOBAL_LOCK(Glptr, pending) pending = 0
#endif

/* Flush CPU cache - used when going to sleep. Wbinvd or similar. */

#ifndef ACPI_FLUSH_CPU_CACHE
#define ACPI_FLUSH_CPU_CACHE()
#endif

>>>>>>> refs/remotes/origin/master
/* "inline" keywords - configurable since inline is not standardized */

#ifndef ACPI_INLINE
#define ACPI_INLINE
#endif

/*
<<<<<<< HEAD
=======
 * Configurable calling conventions:
 *
 * ACPI_SYSTEM_XFACE        - Interfaces to host OS (handlers, threads)
 * ACPI_EXTERNAL_XFACE      - External ACPI interfaces
 * ACPI_INTERNAL_XFACE      - Internal ACPI interfaces
 * ACPI_INTERNAL_VAR_XFACE  - Internal variable-parameter list interfaces
 */
#ifndef ACPI_SYSTEM_XFACE
#define ACPI_SYSTEM_XFACE
#endif

#ifndef ACPI_EXTERNAL_XFACE
#define ACPI_EXTERNAL_XFACE
#endif

#ifndef ACPI_INTERNAL_XFACE
#define ACPI_INTERNAL_XFACE
#endif

#ifndef ACPI_INTERNAL_VAR_XFACE
#define ACPI_INTERNAL_VAR_XFACE
#endif

/*
>>>>>>> refs/remotes/origin/master
 * Debugger threading model
 * Use single threaded if the entire subsystem is contained in an application
 * Use multiple threaded when the subsystem is running in the kernel.
 *
 * By default the model is single threaded if ACPI_APPLICATION is set,
 * multi-threaded if ACPI_APPLICATION is not set.
 */
#ifndef DEBUGGER_THREADING
#ifdef ACPI_APPLICATION
#define DEBUGGER_THREADING          DEBUGGER_SINGLE_THREADED

#else
#define DEBUGGER_THREADING          DEBUGGER_MULTI_THREADED
#endif
#endif				/* !DEBUGGER_THREADING */

/******************************************************************************
 *
 * C library configuration
 *
 *****************************************************************************/

<<<<<<< HEAD
#define ACPI_IS_ASCII(c)  ((c) < 0x80)

#ifdef ACPI_USE_SYSTEM_CLIBRARY
/*
 * Use the standard C library headers.
 * We want to keep these to a minimum.
 */
#ifdef ACPI_USE_STANDARD_HEADERS
/*
 * Use the standard headers from the standard locations
 */
=======
/*
 * ACPI_USE_SYSTEM_CLIBRARY - Define this if linking to an actual C library.
 *      Otherwise, local versions of string/memory functions will be used.
 * ACPI_USE_STANDARD_HEADERS - Define this if linking to a C library and
 *      the standard header files may be used.
 *
 * The ACPICA subsystem only uses low level C library functions that do not call
 * operating system services and may therefore be inlined in the code.
 *
 * It may be necessary to tailor these include files to the target
 * generation environment.
 */
#ifdef ACPI_USE_SYSTEM_CLIBRARY

/* Use the standard C library headers. We want to keep these to a minimum. */

#ifdef ACPI_USE_STANDARD_HEADERS

/* Use the standard headers from the standard locations */

>>>>>>> refs/remotes/origin/master
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#endif				/* ACPI_USE_STANDARD_HEADERS */

<<<<<<< HEAD
/*
 * We will be linking to the standard Clib functions
 */
=======
/* We will be linking to the standard Clib functions */

>>>>>>> refs/remotes/origin/master
#define ACPI_STRSTR(s1,s2)      strstr((s1), (s2))
#define ACPI_STRCHR(s1,c)       strchr((s1), (c))
#define ACPI_STRLEN(s)          (acpi_size) strlen((s))
#define ACPI_STRCPY(d,s)        (void) strcpy((d), (s))
#define ACPI_STRNCPY(d,s,n)     (void) strncpy((d), (s), (acpi_size)(n))
#define ACPI_STRNCMP(d,s,n)     strncmp((d), (s), (acpi_size)(n))
#define ACPI_STRCMP(d,s)        strcmp((d), (s))
#define ACPI_STRCAT(d,s)        (void) strcat((d), (s))
#define ACPI_STRNCAT(d,s,n)     strncat((d), (s), (acpi_size)(n))
#define ACPI_STRTOUL(d,s,n)     strtoul((d), (s), (acpi_size)(n))
#define ACPI_MEMCMP(s1,s2,n)    memcmp((const char *)(s1), (const char *)(s2), (acpi_size)(n))
#define ACPI_MEMCPY(d,s,n)      (void) memcpy((d), (s), (acpi_size)(n))
#define ACPI_MEMSET(d,s,n)      (void) memset((d), (s), (acpi_size)(n))

#define ACPI_TOUPPER(i)         toupper((int) (i))
#define ACPI_TOLOWER(i)         tolower((int) (i))
#define ACPI_IS_XDIGIT(i)       isxdigit((int) (i))
#define ACPI_IS_DIGIT(i)        isdigit((int) (i))
#define ACPI_IS_SPACE(i)        isspace((int) (i))
#define ACPI_IS_UPPER(i)        isupper((int) (i))
#define ACPI_IS_PRINT(i)        isprint((int) (i))
#define ACPI_IS_ALPHA(i)        isalpha((int) (i))

#else

/******************************************************************************
 *
 * Not using native C library, use local implementations
 *
 *****************************************************************************/

<<<<<<< HEAD
 /*
  * Use local definitions of C library macros and functions
  * NOTE: The function implementations may not be as efficient
  * as an inline or assembly code implementation provided by a
  * native C library.
  */

=======
/*
 * Use local definitions of C library macros and functions. These function
 * implementations may not be as efficient as an inline or assembly code
 * implementation provided by a native C library, but they are functionally
 * equivalent.
 */
>>>>>>> refs/remotes/origin/master
#ifndef va_arg

#ifndef _VALIST
#define _VALIST
typedef char *va_list;
#endif				/* _VALIST */

<<<<<<< HEAD
/*
 * Storage alignment properties
 */
#define  _AUPBND                (sizeof (acpi_native_int) - 1)
#define  _ADNBND                (sizeof (acpi_native_int) - 1)

/*
 * Variable argument list macro definitions
 */
#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)              (void) 0
=======
/* Storage alignment properties */

#define  _AUPBND                (sizeof (acpi_native_int) - 1)
#define  _ADNBND                (sizeof (acpi_native_int) - 1)

/* Variable argument list macro definitions */

#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)              (ap = (va_list) NULL)
>>>>>>> refs/remotes/origin/master
#define va_start(ap, A)         (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))

#endif				/* va_arg */

<<<<<<< HEAD
=======
/* Use the local (ACPICA) definitions of the clib functions */

>>>>>>> refs/remotes/origin/master
#define ACPI_STRSTR(s1,s2)      acpi_ut_strstr ((s1), (s2))
#define ACPI_STRCHR(s1,c)       acpi_ut_strchr ((s1), (c))
#define ACPI_STRLEN(s)          (acpi_size) acpi_ut_strlen ((s))
#define ACPI_STRCPY(d,s)        (void) acpi_ut_strcpy ((d), (s))
#define ACPI_STRNCPY(d,s,n)     (void) acpi_ut_strncpy ((d), (s), (acpi_size)(n))
#define ACPI_STRNCMP(d,s,n)     acpi_ut_strncmp ((d), (s), (acpi_size)(n))
#define ACPI_STRCMP(d,s)        acpi_ut_strcmp ((d), (s))
#define ACPI_STRCAT(d,s)        (void) acpi_ut_strcat ((d), (s))
#define ACPI_STRNCAT(d,s,n)     acpi_ut_strncat ((d), (s), (acpi_size)(n))
#define ACPI_STRTOUL(d,s,n)     acpi_ut_strtoul ((d), (s), (acpi_size)(n))
#define ACPI_MEMCMP(s1,s2,n)    acpi_ut_memcmp((const char *)(s1), (const char *)(s2), (acpi_size)(n))
#define ACPI_MEMCPY(d,s,n)      (void) acpi_ut_memcpy ((d), (s), (acpi_size)(n))
#define ACPI_MEMSET(d,v,n)      (void) acpi_ut_memset ((d), (v), (acpi_size)(n))
#define ACPI_TOUPPER(c)         acpi_ut_to_upper ((int) (c))
#define ACPI_TOLOWER(c)         acpi_ut_to_lower ((int) (c))

#endif				/* ACPI_USE_SYSTEM_CLIBRARY */

<<<<<<< HEAD
/******************************************************************************
 *
 * Assembly code macros
 *
 *****************************************************************************/

/*
 * Handle platform- and compiler-specific assembly language differences.
 * These should already have been defined by the platform includes above.
 *
 * Notes:
 * 1) Interrupt 3 is used to break into a debugger
 * 2) Interrupts are turned off during ACPI register setup
 */

/* Unrecognized compiler, use defaults */

#ifndef ACPI_ASM_MACROS

/*
 * Calling conventions:
 *
 * ACPI_SYSTEM_XFACE        - Interfaces to host OS (handlers, threads)
 * ACPI_EXTERNAL_XFACE      - External ACPI interfaces
 * ACPI_INTERNAL_XFACE      - Internal ACPI interfaces
 * ACPI_INTERNAL_VAR_XFACE  - Internal variable-parameter list interfaces
 */
#define ACPI_SYSTEM_XFACE
#define ACPI_EXTERNAL_XFACE
#define ACPI_INTERNAL_XFACE
#define ACPI_INTERNAL_VAR_XFACE

#define ACPI_ASM_MACROS
#define BREAKPOINT3
#define ACPI_DISABLE_IRQS()
#define ACPI_ENABLE_IRQS()
#define ACPI_ACQUIRE_GLOBAL_LOCK(Glptr, acq)
#define ACPI_RELEASE_GLOBAL_LOCK(Glptr, acq)

#endif				/* ACPI_ASM_MACROS */

#ifdef ACPI_APPLICATION

/* Don't want software interrupts within a ring3 application */

#undef BREAKPOINT3
#define BREAKPOINT3
#endif

/******************************************************************************
 *
 * Compiler-specific information is contained in the compiler-specific
 * headers.
 *
 *****************************************************************************/
=======
>>>>>>> refs/remotes/origin/master
#endif				/* __ACENV_H__ */
