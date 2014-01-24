/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright 1999 Ralf Baechle (ralf@gnu.org)
 * Copyright 1999 Silicon Graphics, Inc.
 */
#ifndef _ASM_ARC_TYPES_H
#define _ASM_ARC_TYPES_H


<<<<<<< HEAD
#ifdef CONFIG_ARC32
=======
#ifdef CONFIG_FW_ARC32
>>>>>>> refs/remotes/origin/master

typedef char		CHAR;
typedef short		SHORT;
typedef long		LARGE_INTEGER __attribute__ ((__mode__ (__DI__)));
<<<<<<< HEAD
typedef	long		LONG __attribute__ ((__mode__ (__SI__)));
=======
typedef long		LONG __attribute__ ((__mode__ (__SI__)));
>>>>>>> refs/remotes/origin/master
typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG __attribute__ ((__mode__ (__SI__)));
typedef void		VOID;

/* The pointer types.  Note that we're using a 64-bit compiler but all
   pointer in the ARC structures are only 32-bit, so we need some disgusting
<<<<<<< HEAD
   workarounds.  Keep your vomit bag handy.  */
typedef LONG		_PCHAR;
typedef LONG		_PSHORT;
typedef LONG		_PLARGE_INTEGER;
typedef	LONG		_PLONG;
=======
   workarounds.	 Keep your vomit bag handy.  */
typedef LONG		_PCHAR;
typedef LONG		_PSHORT;
typedef LONG		_PLARGE_INTEGER;
typedef LONG		_PLONG;
>>>>>>> refs/remotes/origin/master
typedef LONG		_PUCHAR;
typedef LONG		_PUSHORT;
typedef LONG		_PULONG;
typedef LONG		_PVOID;

<<<<<<< HEAD
#endif /* CONFIG_ARC32 */

#ifdef CONFIG_ARC64
=======
#endif /* CONFIG_FW_ARC32 */

#ifdef CONFIG_FW_ARC64
>>>>>>> refs/remotes/origin/master

typedef char		CHAR;
typedef short		SHORT;
typedef long		LARGE_INTEGER __attribute__ ((__mode__ (__DI__)));
<<<<<<< HEAD
typedef	long		LONG __attribute__ ((__mode__ (__DI__)));
=======
typedef long		LONG __attribute__ ((__mode__ (__DI__)));
>>>>>>> refs/remotes/origin/master
typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG __attribute__ ((__mode__ (__DI__)));
typedef void		VOID;

/* The pointer types.  We're 64-bit and the firmware is also 64-bit, so
   live is sane ...  */
typedef CHAR		*_PCHAR;
typedef SHORT		*_PSHORT;
typedef LARGE_INTEGER	*_PLARGE_INTEGER;
<<<<<<< HEAD
typedef	LONG		*_PLONG;
=======
typedef LONG		*_PLONG;
>>>>>>> refs/remotes/origin/master
typedef UCHAR		*_PUCHAR;
typedef USHORT		*_PUSHORT;
typedef ULONG		*_PULONG;
typedef VOID		*_PVOID;

<<<<<<< HEAD
#endif /* CONFIG_ARC64  */
=======
#endif /* CONFIG_FW_ARC64  */
>>>>>>> refs/remotes/origin/master

typedef CHAR		*PCHAR;
typedef SHORT		*PSHORT;
typedef LARGE_INTEGER	*PLARGE_INTEGER;
<<<<<<< HEAD
typedef	LONG		*PLONG;
=======
typedef LONG		*PLONG;
>>>>>>> refs/remotes/origin/master
typedef UCHAR		*PUCHAR;
typedef USHORT		*PUSHORT;
typedef ULONG		*PULONG;
typedef VOID		*PVOID;

/*
 * Return type of ArcGetDisplayStatus()
 */
typedef struct {
	USHORT	CursorXPosition;
	USHORT	CursorYPosition;
	USHORT	CursorMaxXPosition;
	USHORT	CursorMaxYPosition;
	USHORT	ForegroundColor;
	USHORT	BackgroundColor;
	UCHAR	HighIntensity;
	UCHAR	Underscored;
	UCHAR	ReverseVideo;
} DISPLAY_STATUS;

#endif /* _ASM_ARC_TYPES_H */
