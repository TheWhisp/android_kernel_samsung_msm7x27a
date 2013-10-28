
/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/moduleparam.h>
#include "regd.h"

static unsigned short reg_domain = 0xffff;
module_param(reg_domain, ushort, 0644);

/*
 * Number of bytes in board data that we are interested
 * in while setting regulatory domain from host
 */
#define REG_DMN_BOARD_DATA_LEN	16

/* Modifies regulatory domain in board data in target RAM */
int ath6kl_set_reg_dmn(struct ath6kl *ar)
{
	u8 buf[REG_DMN_BOARD_DATA_LEN];
	__le16 old_sum, old_ver, old_rd, old_rd_next;
	__le32 brd_dat_addr = 0, new_sum, new_rd;
	int ret;

	if (reg_domain == 0xffff)
		return 0;

	ret = ath6kl_bmi_read(ar, AR6003_BOARD_DATA_ADDR,
			      (u8 *)&brd_dat_addr, 4);
	if (ret)
		return ret;

	memset(buf, 0, sizeof(buf));
	ret = ath6kl_bmi_read(ar, brd_dat_addr, buf, sizeof(buf));
	if (ret)
		return ret;

	memcpy((u8 *)&old_sum, buf + AR6003_BOARD_DATA_OFFSET, 2);
	memcpy((u8 *)&old_ver, buf + AR6003_BOARD_DATA_OFFSET + 2, 2);
	memcpy((u8 *)&old_rd, buf + AR6003_RD_OFFSET, 2);
	memcpy((u8 *)&old_rd_next, buf + AR6003_RD_OFFSET + 2, 2);

	/*
	 * Overwrite the new regulatory domain and preserve the
	 * MAC addr which is in the same word.
	 */
	new_rd = cpu_to_le32((le32_to_cpu(old_rd_next) << 16) + reg_domain);
	ret = ath6kl_bmi_write(ar,
		cpu_to_le32(le32_to_cpu(brd_dat_addr) + AR6003_RD_OFFSET),
		(u8 *)&new_rd, 4);
	if (ret)
		return ret;

	/*
	 * Recompute the board data checksum with the new regulatory
	 * domain, preserve the version information which is in the
	 * same word.
	 */
	new_sum = cpu_to_le32((le32_to_cpu(old_ver) << 16) +
			      (le32_to_cpu(old_sum) ^ le32_to_cpu(old_rd) ^
			       reg_domain));
	ret = ath6kl_bmi_write(ar,
		cpu_to_le32(le32_to_cpu(brd_dat_addr) +
		AR6003_BOARD_DATA_OFFSET),
		(u8 *)&new_sum, 4);

	return ret;
}
