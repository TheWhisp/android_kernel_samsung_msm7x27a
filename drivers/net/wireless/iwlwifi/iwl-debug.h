/******************************************************************************
 *
<<<<<<< HEAD
 * Copyright(c) 2003 - 2011 Intel Corporation. All rights reserved.
=======
 * Copyright(c) 2003 - 2012 Intel Corporation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Portions of this file are derived from the ipw3945 project.
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

#ifndef __iwl_debug_h__
#define __iwl_debug_h__

<<<<<<< HEAD
struct iwl_priv;
extern u32 iwl_debug_level;

#define IWL_ERR(p, f, a...) dev_err(&((p)->pci_dev->dev), f, ## a)
#define IWL_WARN(p, f, a...) dev_warn(&((p)->pci_dev->dev), f, ## a)
#define IWL_INFO(p, f, a...) dev_info(&((p)->pci_dev->dev), f, ## a)
#define IWL_CRIT(p, f, a...) dev_crit(&((p)->pci_dev->dev), f, ## a)

#define iwl_print_hex_error(priv, p, len) 				\
=======
#include "iwl-shared.h"
#include "iwl-devtrace.h"

struct iwl_priv;

void __iwl_err(struct device *dev, bool rfkill_prefix, bool only_trace,
		const char *fmt, ...);
void __iwl_warn(struct device *dev, const char *fmt, ...);
void __iwl_info(struct device *dev, const char *fmt, ...);
void __iwl_crit(struct device *dev, const char *fmt, ...);

/* No matter what is m (priv, bus, trans), this will work */
#define IWL_ERR(m, f, a...) __iwl_err(trans(m)->dev, false, false, f, ## a)
#define IWL_WARN(m, f, a...) __iwl_warn(trans(m)->dev, f, ## a)
#define IWL_INFO(m, f, a...) __iwl_info(trans(m)->dev, f, ## a)
#define IWL_CRIT(m, f, a...) __iwl_crit(trans(m)->dev, f, ## a)

#if defined(CONFIG_IWLWIFI_DEBUG) || defined(CONFIG_IWLWIFI_DEVICE_TRACING)
void __iwl_dbg(struct device *dev,
	       u32 level, bool limit, const char *function,
	       const char *fmt, ...);
#else
static inline void
__iwl_dbg(struct device *dev,
	  u32 level, bool limit, const char *function,
	  const char *fmt, ...)
{}
#endif

#define iwl_print_hex_error(m, p, len)					\
>>>>>>> refs/remotes/origin/cm-10.0
do {									\
	print_hex_dump(KERN_ERR, "iwl data: ",				\
		       DUMP_PREFIX_OFFSET, 16, 1, p, len, 1);		\
} while (0)

<<<<<<< HEAD
#ifdef CONFIG_IWLWIFI_DEBUG
#define IWL_DEBUG(__priv, level, fmt, args...)				\
do {									\
	if (iwl_get_debug_level(__priv) & (level))					\
		dev_printk(KERN_ERR, &(__priv->hw->wiphy->dev),		\
			 "%c %s " fmt, in_interrupt() ? 'I' : 'U',	\
			__func__ , ## args);				\
} while (0)

#define IWL_DEBUG_LIMIT(__priv, level, fmt, args...)			\
do {									\
	if ((iwl_get_debug_level(__priv) & (level)) && net_ratelimit())		\
		dev_printk(KERN_ERR, &(__priv->hw->wiphy->dev),		\
			"%c %s " fmt, in_interrupt() ? 'I' : 'U',	\
			 __func__ , ## args);				\
} while (0)

#define iwl_print_hex_dump(priv, level, p, len) 			\
do {                                            			\
	if (iwl_get_debug_level(priv) & level) 				\
		print_hex_dump(KERN_DEBUG, "iwl data: ",		\
			       DUMP_PREFIX_OFFSET, 16, 1, p, len, 1);	\
} while (0)

#else
#define IWL_DEBUG(__priv, level, fmt, args...)
#define IWL_DEBUG_LIMIT(__priv, level, fmt, args...)
static inline void iwl_print_hex_dump(struct iwl_priv *priv, int level,
				      const void *p, u32 len)
{}
=======
#define IWL_DEBUG(m, level, fmt, args...)				\
	__iwl_dbg(trans(m)->dev, level, false, __func__, fmt, ##args)
#define IWL_DEBUG_LIMIT(m, level, fmt, args...)				\
	__iwl_dbg(trans(m)->dev, level, true, __func__, fmt, ##args)

#ifdef CONFIG_IWLWIFI_DEBUG
#define iwl_print_hex_dump(m, level, p, len)				\
do {                                            			\
	if (iwl_have_debug_level(level))				\
		print_hex_dump(KERN_DEBUG, "iwl data: ",		\
			       DUMP_PREFIX_OFFSET, 16, 1, p, len, 1);	\
} while (0)
#else
#define iwl_print_hex_dump(m, level, p, len)
>>>>>>> refs/remotes/origin/cm-10.0
#endif				/* CONFIG_IWLWIFI_DEBUG */

#ifdef CONFIG_IWLWIFI_DEBUGFS
int iwl_dbgfs_register(struct iwl_priv *priv, const char *name);
void iwl_dbgfs_unregister(struct iwl_priv *priv);
<<<<<<< HEAD
extern int iwl_dbgfs_statistics_flag(struct iwl_priv *priv, char *buf,
				     int bufsz);
=======
>>>>>>> refs/remotes/origin/cm-10.0
#else
static inline int iwl_dbgfs_register(struct iwl_priv *priv, const char *name)
{
	return 0;
}
static inline void iwl_dbgfs_unregister(struct iwl_priv *priv)
{
}
#endif				/* CONFIG_IWLWIFI_DEBUGFS */

/*
 * To use the debug system:
 *
 * If you are defining a new debug classification, simply add it to the #define
 * list here in the form of
 *
 * #define IWL_DL_xxxx VALUE
 *
 * where xxxx should be the name of the classification (for example, WEP).
 *
 * You then need to either add a IWL_xxxx_DEBUG() macro definition for your
 * classification, or use IWL_DEBUG(IWL_DL_xxxx, ...) whenever you want
 * to send output to that classification.
 *
 * The active debug levels can be accessed via files
 *
<<<<<<< HEAD
 * 	/sys/module/iwlagn/parameters/debug{50}
 * 	/sys/class/net/wlan0/device/debug_level
 *
 * when CONFIG_IWLWIFI_DEBUG=y.
 */

/* 0x0000000F - 0x00000001 */
#define IWL_DL_INFO		(1 << 0)
#define IWL_DL_MAC80211		(1 << 1)
#define IWL_DL_HCMD		(1 << 2)
#define IWL_DL_STATE		(1 << 3)
/* 0x000000F0 - 0x00000010 */
#define IWL_DL_MACDUMP		(1 << 4)
#define IWL_DL_HCMD_DUMP	(1 << 5)
#define IWL_DL_EEPROM		(1 << 6)
#define IWL_DL_RADIO		(1 << 7)
/* 0x00000F00 - 0x00000100 */
#define IWL_DL_POWER		(1 << 8)
#define IWL_DL_TEMP		(1 << 9)
#define IWL_DL_NOTIF		(1 << 10)
#define IWL_DL_SCAN		(1 << 11)
/* 0x0000F000 - 0x00001000 */
#define IWL_DL_ASSOC		(1 << 12)
#define IWL_DL_DROP		(1 << 13)
#define IWL_DL_TXPOWER		(1 << 14)
#define IWL_DL_AP		(1 << 15)
/* 0x000F0000 - 0x00010000 */
#define IWL_DL_FW		(1 << 16)
#define IWL_DL_RF_KILL		(1 << 17)
#define IWL_DL_FW_ERRORS	(1 << 18)
#define IWL_DL_LED		(1 << 19)
/* 0x00F00000 - 0x00100000 */
#define IWL_DL_RATE		(1 << 20)
#define IWL_DL_CALIB		(1 << 21)
#define IWL_DL_WEP		(1 << 22)
#define IWL_DL_TX		(1 << 23)
/* 0x0F000000 - 0x01000000 */
#define IWL_DL_RX		(1 << 24)
#define IWL_DL_ISR		(1 << 25)
#define IWL_DL_HT		(1 << 26)
/* 0xF0000000 - 0x10000000 */
#define IWL_DL_11H		(1 << 28)
#define IWL_DL_STATS		(1 << 29)
#define IWL_DL_TX_REPLY		(1 << 30)
#define IWL_DL_QOS		(1 << 31)

#define IWL_DEBUG_INFO(p, f, a...)	IWL_DEBUG(p, IWL_DL_INFO, f, ## a)
#define IWL_DEBUG_MAC80211(p, f, a...)	IWL_DEBUG(p, IWL_DL_MAC80211, f, ## a)
#define IWL_DEBUG_MACDUMP(p, f, a...)	IWL_DEBUG(p, IWL_DL_MACDUMP, f, ## a)
=======
 *	/sys/module/iwlwifi/parameters/debug
 * when CONFIG_IWLWIFI_DEBUG=y.
 *
 *	/sys/kernel/debug/phy0/iwlwifi/debug/debug_level
 * when CONFIG_IWLWIFI_DEBUGFS=y.
 *
 */

/* 0x0000000F - 0x00000001 */
#define IWL_DL_INFO		0x00000001
#define IWL_DL_MAC80211		0x00000002
#define IWL_DL_HCMD		0x00000004
#define IWL_DL_STATE		0x00000008
/* 0x000000F0 - 0x00000010 */
#define IWL_DL_EEPROM		0x00000040
#define IWL_DL_RADIO		0x00000080
/* 0x00000F00 - 0x00000100 */
#define IWL_DL_POWER		0x00000100
#define IWL_DL_TEMP		0x00000200
#define IWL_DL_SCAN		0x00000800
/* 0x0000F000 - 0x00001000 */
#define IWL_DL_ASSOC		0x00001000
#define IWL_DL_DROP		0x00002000
#define IWL_DL_COEX		0x00008000
/* 0x000F0000 - 0x00010000 */
#define IWL_DL_FW		0x00010000
#define IWL_DL_RF_KILL		0x00020000
#define IWL_DL_FW_ERRORS	0x00040000
#define IWL_DL_LED		0x00080000
/* 0x00F00000 - 0x00100000 */
#define IWL_DL_RATE		0x00100000
#define IWL_DL_CALIB		0x00200000
#define IWL_DL_WEP		0x00400000
#define IWL_DL_TX		0x00800000
/* 0x0F000000 - 0x01000000 */
#define IWL_DL_RX		0x01000000
#define IWL_DL_ISR		0x02000000
#define IWL_DL_HT		0x04000000
/* 0xF0000000 - 0x10000000 */
#define IWL_DL_11H		0x10000000
#define IWL_DL_STATS		0x20000000
#define IWL_DL_TX_REPLY		0x40000000
#define IWL_DL_TX_QUEUES	0x80000000

#define IWL_DEBUG_INFO(p, f, a...)	IWL_DEBUG(p, IWL_DL_INFO, f, ## a)
#define IWL_DEBUG_MAC80211(p, f, a...)	IWL_DEBUG(p, IWL_DL_MAC80211, f, ## a)
>>>>>>> refs/remotes/origin/cm-10.0
#define IWL_DEBUG_TEMP(p, f, a...)	IWL_DEBUG(p, IWL_DL_TEMP, f, ## a)
#define IWL_DEBUG_SCAN(p, f, a...)	IWL_DEBUG(p, IWL_DL_SCAN, f, ## a)
#define IWL_DEBUG_RX(p, f, a...)	IWL_DEBUG(p, IWL_DL_RX, f, ## a)
#define IWL_DEBUG_TX(p, f, a...)	IWL_DEBUG(p, IWL_DL_TX, f, ## a)
#define IWL_DEBUG_ISR(p, f, a...)	IWL_DEBUG(p, IWL_DL_ISR, f, ## a)
#define IWL_DEBUG_LED(p, f, a...)	IWL_DEBUG(p, IWL_DL_LED, f, ## a)
#define IWL_DEBUG_WEP(p, f, a...)	IWL_DEBUG(p, IWL_DL_WEP, f, ## a)
#define IWL_DEBUG_HC(p, f, a...)	IWL_DEBUG(p, IWL_DL_HCMD, f, ## a)
<<<<<<< HEAD
#define IWL_DEBUG_HC_DUMP(p, f, a...)	IWL_DEBUG(p, IWL_DL_HCMD_DUMP, f, ## a)
=======
>>>>>>> refs/remotes/origin/cm-10.0
#define IWL_DEBUG_EEPROM(p, f, a...)	IWL_DEBUG(p, IWL_DL_EEPROM, f, ## a)
#define IWL_DEBUG_CALIB(p, f, a...)	IWL_DEBUG(p, IWL_DL_CALIB, f, ## a)
#define IWL_DEBUG_FW(p, f, a...)	IWL_DEBUG(p, IWL_DL_FW, f, ## a)
#define IWL_DEBUG_RF_KILL(p, f, a...)	IWL_DEBUG(p, IWL_DL_RF_KILL, f, ## a)
<<<<<<< HEAD
#define IWL_DEBUG_DROP(p, f, a...)	IWL_DEBUG(p, IWL_DL_DROP, f, ## a)
#define IWL_DEBUG_DROP_LIMIT(p, f, a...)	\
		IWL_DEBUG_LIMIT(p, IWL_DL_DROP, f, ## a)
#define IWL_DEBUG_AP(p, f, a...)	IWL_DEBUG(p, IWL_DL_AP, f, ## a)
#define IWL_DEBUG_TXPOWER(p, f, a...)	IWL_DEBUG(p, IWL_DL_TXPOWER, f, ## a)
#define IWL_DEBUG_RATE(p, f, a...)	IWL_DEBUG(p, IWL_DL_RATE, f, ## a)
#define IWL_DEBUG_RATE_LIMIT(p, f, a...)	\
		IWL_DEBUG_LIMIT(p, IWL_DL_RATE, f, ## a)
#define IWL_DEBUG_NOTIF(p, f, a...)	IWL_DEBUG(p, IWL_DL_NOTIF, f, ## a)
=======
#define IWL_DEBUG_FW_ERRORS(p, f, a...)	IWL_DEBUG(p, IWL_DL_FW_ERRORS, f, ## a)
#define IWL_DEBUG_DROP(p, f, a...)	IWL_DEBUG(p, IWL_DL_DROP, f, ## a)
#define IWL_DEBUG_DROP_LIMIT(p, f, a...)	\
		IWL_DEBUG_LIMIT(p, IWL_DL_DROP, f, ## a)
#define IWL_DEBUG_COEX(p, f, a...)	IWL_DEBUG(p, IWL_DL_COEX, f, ## a)
#define IWL_DEBUG_RATE(p, f, a...)	IWL_DEBUG(p, IWL_DL_RATE, f, ## a)
#define IWL_DEBUG_RATE_LIMIT(p, f, a...)	\
		IWL_DEBUG_LIMIT(p, IWL_DL_RATE, f, ## a)
>>>>>>> refs/remotes/origin/cm-10.0
#define IWL_DEBUG_ASSOC(p, f, a...)	\
		IWL_DEBUG(p, IWL_DL_ASSOC | IWL_DL_INFO, f, ## a)
#define IWL_DEBUG_ASSOC_LIMIT(p, f, a...)	\
		IWL_DEBUG_LIMIT(p, IWL_DL_ASSOC | IWL_DL_INFO, f, ## a)
#define IWL_DEBUG_HT(p, f, a...)	IWL_DEBUG(p, IWL_DL_HT, f, ## a)
#define IWL_DEBUG_STATS(p, f, a...)	IWL_DEBUG(p, IWL_DL_STATS, f, ## a)
#define IWL_DEBUG_STATS_LIMIT(p, f, a...)	\
		IWL_DEBUG_LIMIT(p, IWL_DL_STATS, f, ## a)
#define IWL_DEBUG_TX_REPLY(p, f, a...)	IWL_DEBUG(p, IWL_DL_TX_REPLY, f, ## a)
<<<<<<< HEAD
#define IWL_DEBUG_TX_REPLY_LIMIT(p, f, a...) \
		IWL_DEBUG_LIMIT(p, IWL_DL_TX_REPLY, f, ## a)
#define IWL_DEBUG_QOS(p, f, a...)	IWL_DEBUG(p, IWL_DL_QOS, f, ## a)
=======
#define IWL_DEBUG_TX_QUEUES(p, f, a...)	IWL_DEBUG(p, IWL_DL_TX_QUEUES, f, ## a)
>>>>>>> refs/remotes/origin/cm-10.0
#define IWL_DEBUG_RADIO(p, f, a...)	IWL_DEBUG(p, IWL_DL_RADIO, f, ## a)
#define IWL_DEBUG_POWER(p, f, a...)	IWL_DEBUG(p, IWL_DL_POWER, f, ## a)
#define IWL_DEBUG_11H(p, f, a...)	IWL_DEBUG(p, IWL_DL_11H, f, ## a)

#endif
