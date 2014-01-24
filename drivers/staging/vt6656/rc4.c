/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * File: rc4.c
 *
 * Purpose:
 *
 * Functions:
 *
 * Revision History:
 *
 * Author: Kyle Hsu
 *
 * Date: Sep 4, 2002
 *
 */

#include "rc4.h"

<<<<<<< HEAD
void rc4_init(PRC4Ext pRC4, PBYTE pbyKey, unsigned int cbKey_len)
=======
void rc4_init(PRC4Ext pRC4, u8 * pbyKey, unsigned int cbKey_len)
>>>>>>> refs/remotes/origin/master
{
	unsigned int  ust1, ust2;
	unsigned int  keyindex;
	unsigned int  stateindex;
<<<<<<< HEAD
	PBYTE pbyst;
=======
	u8 * pbyst;
>>>>>>> refs/remotes/origin/master
	unsigned int  idx;

	pbyst = pRC4->abystate;
	pRC4->ux = 0;
	pRC4->uy = 0;
	for (idx = 0; idx < 256; idx++)
<<<<<<< HEAD
		pbyst[idx] = (BYTE)idx;
=======
		pbyst[idx] = (u8)idx;
>>>>>>> refs/remotes/origin/master
	keyindex = 0;
	stateindex = 0;
	for (idx = 0; idx < 256; idx++) {
		ust1 = pbyst[idx];
		stateindex = (stateindex + pbyKey[keyindex] + ust1) & 0xff;
		ust2 = pbyst[stateindex];
<<<<<<< HEAD
		pbyst[stateindex] = (BYTE)ust1;
		pbyst[idx] = (BYTE)ust2;
=======
		pbyst[stateindex] = (u8)ust1;
		pbyst[idx] = (u8)ust2;
>>>>>>> refs/remotes/origin/master
		if (++keyindex >= cbKey_len)
			keyindex = 0;
	}
}

unsigned int rc4_byte(PRC4Ext pRC4)
{
	unsigned int ux;
	unsigned int uy;
	unsigned int ustx, usty;
<<<<<<< HEAD
	PBYTE pbyst;
=======
	u8 * pbyst;
>>>>>>> refs/remotes/origin/master

	pbyst = pRC4->abystate;
	ux = (pRC4->ux + 1) & 0xff;
	ustx = pbyst[ux];
	uy = (ustx + pRC4->uy) & 0xff;
	usty = pbyst[uy];
	pRC4->ux = ux;
	pRC4->uy = uy;
<<<<<<< HEAD
	pbyst[uy] = (BYTE)ustx;
	pbyst[ux] = (BYTE)usty;
=======
	pbyst[uy] = (u8)ustx;
	pbyst[ux] = (u8)usty;
>>>>>>> refs/remotes/origin/master

	return pbyst[(ustx + usty) & 0xff];
}

<<<<<<< HEAD
void rc4_encrypt(PRC4Ext pRC4, PBYTE pbyDest,
			PBYTE pbySrc, unsigned int cbData_len)
{
	unsigned int ii;
	for (ii = 0; ii < cbData_len; ii++)
		pbyDest[ii] = (BYTE)(pbySrc[ii] ^ rc4_byte(pRC4));
=======
void rc4_encrypt(PRC4Ext pRC4, u8 * pbyDest,
			u8 * pbySrc, unsigned int cbData_len)
{
	unsigned int ii;
	for (ii = 0; ii < cbData_len; ii++)
		pbyDest[ii] = (u8)(pbySrc[ii] ^ rc4_byte(pRC4));
>>>>>>> refs/remotes/origin/master
}
