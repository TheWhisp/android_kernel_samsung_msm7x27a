/* $Id: boardergo.h,v 1.2.6.1 2001/09/23 22:24:54 kai Exp $
 *
 * Linux driver for HYSDN cards, definitions for ergo type boards (buffers..).
 *
 * Author    Werner Cornelius (werner@titro.de) for Hypercope GmbH
 * Copyright 1999 by Werner Cornelius (werner@titro.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */


/************************************************/
/* defines for the dual port memory of the card */
/************************************************/
#define ERG_DPRAM_PAGE_SIZE 0x2000	/* DPRAM occupies a 8K page */
#define BOOT_IMG_SIZE 4096
#define ERG_DPRAM_FILL_SIZE (ERG_DPRAM_PAGE_SIZE - BOOT_IMG_SIZE)

#define ERG_TO_HY_BUF_SIZE  0x0E00	/* 3072 bytes buffer size to card */
#define ERG_TO_PC_BUF_SIZE  0x0E00	/* 3072 bytes to PC, too */

/* following DPRAM layout copied from OS2-driver boarderg.h */
typedef struct ErgDpram_tag {
<<<<<<< HEAD
<<<<<<< HEAD
/*0000 */ unsigned char ToHyBuf[ERG_TO_HY_BUF_SIZE];
/*0E00 */ unsigned char ToPcBuf[ERG_TO_PC_BUF_SIZE];
=======
	/*0000 */ unsigned char ToHyBuf[ERG_TO_HY_BUF_SIZE];
	/*0E00 */ unsigned char ToPcBuf[ERG_TO_PC_BUF_SIZE];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/*0000 */ unsigned char ToHyBuf[ERG_TO_HY_BUF_SIZE];
	/*0E00 */ unsigned char ToPcBuf[ERG_TO_PC_BUF_SIZE];
>>>>>>> refs/remotes/origin/master

	/*1C00 */ unsigned char bSoftUart[SIZE_RSV_SOFT_UART];
	/* size 0x1B0 */

	/*1DB0 *//* tErrLogEntry */ unsigned char volatile ErrLogMsg[64];
	/* size 64 bytes */
	/*1DB0  unsigned long ulErrType;               */
	/*1DB4  unsigned long ulErrSubtype;            */
	/*1DB8  unsigned long ucTextSize;              */
	/*1DB9  unsigned long ucText[ERRLOG_TEXT_SIZE]; *//* ASCIIZ of len ucTextSize-1 */
	/*1DF0 */

<<<<<<< HEAD
<<<<<<< HEAD
/*1DF0 */ unsigned short volatile ToHyChannel;
/*1DF2 */ unsigned short volatile ToHySize;
=======
	/*1DF0 */ unsigned short volatile ToHyChannel;
	/*1DF2 */ unsigned short volatile ToHySize;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/*1DF0 */ unsigned short volatile ToHyChannel;
	/*1DF2 */ unsigned short volatile ToHySize;
>>>>>>> refs/remotes/origin/master
	/*1DF4 */ unsigned char volatile ToHyFlag;
	/* !=0: msg for Hy waiting */
	/*1DF5 */ unsigned char volatile ToPcFlag;
	/* !=0: msg for PC waiting */
<<<<<<< HEAD
<<<<<<< HEAD
/*1DF6 */ unsigned short volatile ToPcChannel;
/*1DF8 */ unsigned short volatile ToPcSize;
	/*1DFA */ unsigned char bRes1DBA[0x1E00 - 0x1DFA];
	/* 6 bytes */

/*1E00 */ unsigned char bRestOfEntryTbl[0x1F00 - 0x1E00];
/*1F00 */ unsigned long TrapTable[62];
	/*1FF8 */ unsigned char bRes1FF8[0x1FFB - 0x1FF8];
	/* low part of reset vetor */
/*1FFB */ unsigned char ToPcIntMetro;
=======
=======
>>>>>>> refs/remotes/origin/master
	/*1DF6 */ unsigned short volatile ToPcChannel;
	/*1DF8 */ unsigned short volatile ToPcSize;
	/*1DFA */ unsigned char bRes1DBA[0x1E00 - 0x1DFA];
	/* 6 bytes */

	/*1E00 */ unsigned char bRestOfEntryTbl[0x1F00 - 0x1E00];
	/*1F00 */ unsigned long TrapTable[62];
	/*1FF8 */ unsigned char bRes1FF8[0x1FFB - 0x1FF8];
	/* low part of reset vetor */
	/*1FFB */ unsigned char ToPcIntMetro;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* notes:
	 * - metro has 32-bit boot ram - accessing
	 *   ToPcInt and ToHyInt would be the same;
	 *   so we moved ToPcInt to 1FFB.
	 *   Because on the PC side both vars are
	 *   readonly (reseting on int from E1 to PC),
	 *   we can read both vars on both cards
	 *   without destroying anything.
	 * - 1FFB is the high byte of the reset vector,
	 *   so E1 side should NOT change this byte
	 *   when writing!
	 */
<<<<<<< HEAD
<<<<<<< HEAD
/*1FFC */ unsigned char volatile ToHyNoDpramErrLog;
=======
	/*1FFC */ unsigned char volatile ToHyNoDpramErrLog;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/*1FFC */ unsigned char volatile ToHyNoDpramErrLog;
>>>>>>> refs/remotes/origin/master
	/* note: ToHyNoDpramErrLog is used to inform
	 *       boot loader, not to use DPRAM based
	 *       ErrLog; when DOS driver is rewritten
	 *       this becomes obsolete
	 */
<<<<<<< HEAD
<<<<<<< HEAD
/*1FFD */ unsigned char bRes1FFD;
=======
	/*1FFD */ unsigned char bRes1FFD;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/*1FFD */ unsigned char bRes1FFD;
>>>>>>> refs/remotes/origin/master
	/*1FFE */ unsigned char ToPcInt;
	/* E1_intclear; on CHAMP2: E1_intset   */
	/*1FFF */ unsigned char ToHyInt;
	/* E1_intset;   on CHAMP2: E1_intclear */
} tErgDpram;

/**********************************************/
/* PCI9050 controller local register offsets: */
/* copied from boarderg.c                     */
/**********************************************/
#define PCI9050_INTR_REG    0x4C	/* Interrupt register */
#define PCI9050_USER_IO     0x51	/* User I/O  register */

<<<<<<< HEAD
<<<<<<< HEAD
				    /* bitmask for PCI9050_INTR_REG: */
=======
/* bitmask for PCI9050_INTR_REG: */
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* bitmask for PCI9050_INTR_REG: */
>>>>>>> refs/remotes/origin/master
#define PCI9050_INTR_REG_EN1    0x01	/* 1= enable (def.), 0= disable */
#define PCI9050_INTR_REG_POL1   0x02	/* 1= active high (def.), 0= active low */
#define PCI9050_INTR_REG_STAT1  0x04	/* 1= intr. active, 0= intr. not active (def.) */
#define PCI9050_INTR_REG_ENPCI  0x40	/* 1= PCI interrupts enable (def.) */

<<<<<<< HEAD
<<<<<<< HEAD
				    /* bitmask for PCI9050_USER_IO: */
=======
/* bitmask for PCI9050_USER_IO: */
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* bitmask for PCI9050_USER_IO: */
>>>>>>> refs/remotes/origin/master
#define PCI9050_USER_IO_EN3     0x02	/* 1= disable      , 0= enable (def.) */
#define PCI9050_USER_IO_DIR3    0x04	/* 1= output (def.), 0= input         */
#define PCI9050_USER_IO_DAT3    0x08	/* 1= high (def.)  , 0= low           */

<<<<<<< HEAD
<<<<<<< HEAD
#define PCI9050_E1_RESET    (                     PCI9050_USER_IO_DIR3)		/* 0x04 */
#define PCI9050_E1_RUN      (PCI9050_USER_IO_DAT3|PCI9050_USER_IO_DIR3)		/* 0x0C */
=======
#define PCI9050_E1_RESET    (PCI9050_USER_IO_DIR3)		/* 0x04 */
#define PCI9050_E1_RUN      (PCI9050_USER_IO_DAT3 | PCI9050_USER_IO_DIR3)		/* 0x0C */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define PCI9050_E1_RESET    (PCI9050_USER_IO_DIR3)		/* 0x04 */
#define PCI9050_E1_RUN      (PCI9050_USER_IO_DAT3 | PCI9050_USER_IO_DIR3)		/* 0x0C */
>>>>>>> refs/remotes/origin/master
