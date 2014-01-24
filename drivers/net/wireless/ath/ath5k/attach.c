/*
 * Copyright (c) 2004-2008 Reyk Floeter <reyk@openbsd.org>
 * Copyright (c) 2006-2008 Nick Kossifidis <mickflemm@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
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
 *
 */

/*************************************\
* Attach/Detach Functions and helpers *
\*************************************/

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/pci.h>
#include <linux/slab.h>
#include "ath5k.h"
#include "reg.h"
#include "debug.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "base.h"

/**
 * ath5k_hw_post - Power On Self Test helper function
 *
=======

/**
 * ath5k_hw_post() - Power On Self Test helper function
>>>>>>> refs/remotes/origin/cm-10.0
=======

/**
 * ath5k_hw_post() - Power On Self Test helper function
>>>>>>> refs/remotes/origin/master
 * @ah: The &struct ath5k_hw
 */
static int ath5k_hw_post(struct ath5k_hw *ah)
{

	static const u32 static_pattern[4] = {
		0x55555555,	0xaaaaaaaa,
		0x66666666,	0x99999999
	};
	static const u16 regs[2] = { AR5K_STA_ID0, AR5K_PHY(8) };
	int i, c;
	u16 cur_reg;
	u32 var_pattern;
	u32 init_val;
	u32 cur_val;

	for (c = 0; c < 2; c++) {

		cur_reg = regs[c];

		/* Save previous value */
		init_val = ath5k_hw_reg_read(ah, cur_reg);

		for (i = 0; i < 256; i++) {
			var_pattern = i << 16 | i;
			ath5k_hw_reg_write(ah, var_pattern, cur_reg);
			cur_val = ath5k_hw_reg_read(ah, cur_reg);

			if (cur_val != var_pattern) {
<<<<<<< HEAD
<<<<<<< HEAD
				ATH5K_ERR(ah->ah_sc, "POST Failed !!!\n");
=======
				ATH5K_ERR(ah, "POST Failed !!!\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
				ATH5K_ERR(ah, "POST Failed !!!\n");
>>>>>>> refs/remotes/origin/master
				return -EAGAIN;
			}

			/* Found on ndiswrapper dumps */
			var_pattern = 0x0039080f;
			ath5k_hw_reg_write(ah, var_pattern, cur_reg);
		}

		for (i = 0; i < 4; i++) {
			var_pattern = static_pattern[i];
			ath5k_hw_reg_write(ah, var_pattern, cur_reg);
			cur_val = ath5k_hw_reg_read(ah, cur_reg);

			if (cur_val != var_pattern) {
<<<<<<< HEAD
<<<<<<< HEAD
				ATH5K_ERR(ah->ah_sc, "POST Failed !!!\n");
=======
				ATH5K_ERR(ah, "POST Failed !!!\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
				ATH5K_ERR(ah, "POST Failed !!!\n");
>>>>>>> refs/remotes/origin/master
				return -EAGAIN;
			}

			/* Found on ndiswrapper dumps */
			var_pattern = 0x003b080f;
			ath5k_hw_reg_write(ah, var_pattern, cur_reg);
		}

		/* Restore previous value */
		ath5k_hw_reg_write(ah, init_val, cur_reg);

	}

	return 0;

}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * ath5k_hw_init - Check if hw is supported and init the needed structs
 *
 * @sc: The &struct ath5k_softc we got from the driver's init_softc function
=======
 * ath5k_hw_init() - Check if hw is supported and init the needed structs
 * @ah: The &struct ath5k_hw associated with the device
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * ath5k_hw_init() - Check if hw is supported and init the needed structs
 * @ah: The &struct ath5k_hw associated with the device
>>>>>>> refs/remotes/origin/master
 *
 * Check if the device is supported, perform a POST and initialize the needed
 * structs. Returns -ENOMEM if we don't have memory for the needed structs,
 * -ENODEV if the device is not supported or prints an error msg if something
 * else went wrong.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int ath5k_hw_init(struct ath5k_softc *sc)
{
	struct ath5k_hw *ah = sc->ah;
	struct ath_common *common = ath5k_hw_common(ah);
	struct pci_dev *pdev = sc->pdev;
=======
=======
>>>>>>> refs/remotes/origin/master
int ath5k_hw_init(struct ath5k_hw *ah)
{
	static const u8 zero_mac[ETH_ALEN] = { };
	struct ath_common *common = ath5k_hw_common(ah);
	struct pci_dev *pdev = ah->pdev;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct ath5k_eeprom_info *ee;
	int ret;
	u32 srev;

	/*
	 * HW information
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	ah->ah_radar.r_enabled = AR5K_TUNE_RADAR_ALERT;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ah->ah_bwmode = AR5K_BWMODE_DEFAULT;
	ah->ah_txpower.txp_tpc = AR5K_TUNE_TPC_TXPOWER;
	ah->ah_imr = 0;
	ah->ah_retry_short = AR5K_INIT_RETRY_SHORT;
	ah->ah_retry_long = AR5K_INIT_RETRY_LONG;
	ah->ah_ant_mode = AR5K_ANTMODE_DEFAULT;
	ah->ah_noise_floor = -95;	/* until first NF calibration is run */
<<<<<<< HEAD
<<<<<<< HEAD
	sc->ani_state.ani_mode = ATH5K_ANI_MODE_AUTO;
	ah->ah_current_channel = &sc->channels[0];
=======
	ah->ani_state.ani_mode = ATH5K_ANI_MODE_AUTO;
	ah->ah_current_channel = &ah->channels[0];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ah->ani_state.ani_mode = ATH5K_ANI_MODE_AUTO;
	ah->ah_current_channel = &ah->channels[0];
>>>>>>> refs/remotes/origin/master

	/*
	 * Find the mac version
	 */
	ath5k_hw_read_srev(ah);
	srev = ah->ah_mac_srev;
	if (srev < AR5K_SREV_AR5311)
		ah->ah_version = AR5K_AR5210;
	else if (srev < AR5K_SREV_AR5212)
		ah->ah_version = AR5K_AR5211;
	else
		ah->ah_version = AR5K_AR5212;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Get the MAC revision */
	ah->ah_mac_version = AR5K_REG_MS(srev, AR5K_SREV_VER);
	ah->ah_mac_revision = AR5K_REG_MS(srev, AR5K_SREV_REV);
=======
	/* Get the MAC version */
	ah->ah_mac_version = AR5K_REG_MS(srev, AR5K_SREV_VER);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Get the MAC version */
	ah->ah_mac_version = AR5K_REG_MS(srev, AR5K_SREV_VER);
>>>>>>> refs/remotes/origin/master

	/* Fill the ath5k_hw struct with the needed functions */
	ret = ath5k_hw_init_desc_functions(ah);
	if (ret)
		goto err;

	/* Bring device out of sleep and reset its units */
<<<<<<< HEAD
<<<<<<< HEAD
	ret = ath5k_hw_nic_wakeup(ah, 0, true);
=======
	ret = ath5k_hw_nic_wakeup(ah, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = ath5k_hw_nic_wakeup(ah, NULL);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto err;

	/* Get PHY and RADIO revisions */
	ah->ah_phy_revision = ath5k_hw_reg_read(ah, AR5K_PHY_CHIP_ID) &
			0xffffffff;
	ah->ah_radio_5ghz_revision = ath5k_hw_radio_revision(ah,
<<<<<<< HEAD
<<<<<<< HEAD
			CHANNEL_5GHZ);
	ah->ah_phy = AR5K_PHY(0);
=======
			IEEE80211_BAND_5GHZ);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			IEEE80211_BAND_5GHZ);
>>>>>>> refs/remotes/origin/master

	/* Try to identify radio chip based on its srev */
	switch (ah->ah_radio_5ghz_revision & 0xf0) {
	case AR5K_SREV_RAD_5111:
		ah->ah_radio = AR5K_RF5111;
		ah->ah_single_chip = false;
		ah->ah_radio_2ghz_revision = ath5k_hw_radio_revision(ah,
<<<<<<< HEAD
<<<<<<< HEAD
							CHANNEL_2GHZ);
=======
							IEEE80211_BAND_2GHZ);
>>>>>>> refs/remotes/origin/cm-10.0
=======
							IEEE80211_BAND_2GHZ);
>>>>>>> refs/remotes/origin/master
		break;
	case AR5K_SREV_RAD_5112:
	case AR5K_SREV_RAD_2112:
		ah->ah_radio = AR5K_RF5112;
		ah->ah_single_chip = false;
		ah->ah_radio_2ghz_revision = ath5k_hw_radio_revision(ah,
<<<<<<< HEAD
<<<<<<< HEAD
							CHANNEL_2GHZ);
=======
							IEEE80211_BAND_2GHZ);
>>>>>>> refs/remotes/origin/cm-10.0
=======
							IEEE80211_BAND_2GHZ);
>>>>>>> refs/remotes/origin/master
		break;
	case AR5K_SREV_RAD_2413:
		ah->ah_radio = AR5K_RF2413;
		ah->ah_single_chip = true;
		break;
	case AR5K_SREV_RAD_5413:
		ah->ah_radio = AR5K_RF5413;
		ah->ah_single_chip = true;
		break;
	case AR5K_SREV_RAD_2316:
		ah->ah_radio = AR5K_RF2316;
		ah->ah_single_chip = true;
		break;
	case AR5K_SREV_RAD_2317:
		ah->ah_radio = AR5K_RF2317;
		ah->ah_single_chip = true;
		break;
	case AR5K_SREV_RAD_5424:
		if (ah->ah_mac_version == AR5K_SREV_AR2425 ||
<<<<<<< HEAD
<<<<<<< HEAD
		ah->ah_mac_version == AR5K_SREV_AR2417){
=======
		    ah->ah_mac_version == AR5K_SREV_AR2417) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		    ah->ah_mac_version == AR5K_SREV_AR2417) {
>>>>>>> refs/remotes/origin/master
			ah->ah_radio = AR5K_RF2425;
			ah->ah_single_chip = true;
		} else {
			ah->ah_radio = AR5K_RF5413;
			ah->ah_single_chip = true;
		}
		break;
	default:
		/* Identify radio based on mac/phy srev */
		if (ah->ah_version == AR5K_AR5210) {
			ah->ah_radio = AR5K_RF5110;
			ah->ah_single_chip = false;
		} else if (ah->ah_version == AR5K_AR5211) {
			ah->ah_radio = AR5K_RF5111;
			ah->ah_single_chip = false;
			ah->ah_radio_2ghz_revision = ath5k_hw_radio_revision(ah,
<<<<<<< HEAD
<<<<<<< HEAD
								CHANNEL_2GHZ);
		} else if (ah->ah_mac_version == (AR5K_SREV_AR2425 >> 4) ||
		ah->ah_mac_version == (AR5K_SREV_AR2417 >> 4) ||
		ah->ah_phy_revision == AR5K_SREV_PHY_2425) {
=======
=======
>>>>>>> refs/remotes/origin/master
							IEEE80211_BAND_2GHZ);
		} else if (ah->ah_mac_version == (AR5K_SREV_AR2425 >> 4) ||
			   ah->ah_mac_version == (AR5K_SREV_AR2417 >> 4) ||
			   ah->ah_phy_revision == AR5K_SREV_PHY_2425) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			ah->ah_radio = AR5K_RF2425;
			ah->ah_single_chip = true;
			ah->ah_radio_5ghz_revision = AR5K_SREV_RAD_2425;
		} else if (srev == AR5K_SREV_AR5213A &&
<<<<<<< HEAD
<<<<<<< HEAD
		ah->ah_phy_revision == AR5K_SREV_PHY_5212B) {
=======
			   ah->ah_phy_revision == AR5K_SREV_PHY_5212B) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			   ah->ah_phy_revision == AR5K_SREV_PHY_5212B) {
>>>>>>> refs/remotes/origin/master
			ah->ah_radio = AR5K_RF5112;
			ah->ah_single_chip = false;
			ah->ah_radio_5ghz_revision = AR5K_SREV_RAD_5112B;
		} else if (ah->ah_mac_version == (AR5K_SREV_AR2415 >> 4) ||
<<<<<<< HEAD
<<<<<<< HEAD
			ah->ah_mac_version == (AR5K_SREV_AR2315_R6 >> 4)) {
=======
			   ah->ah_mac_version == (AR5K_SREV_AR2315_R6 >> 4)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			   ah->ah_mac_version == (AR5K_SREV_AR2315_R6 >> 4)) {
>>>>>>> refs/remotes/origin/master
			ah->ah_radio = AR5K_RF2316;
			ah->ah_single_chip = true;
			ah->ah_radio_5ghz_revision = AR5K_SREV_RAD_2316;
		} else if (ah->ah_mac_version == (AR5K_SREV_AR5414 >> 4) ||
<<<<<<< HEAD
<<<<<<< HEAD
		ah->ah_phy_revision == AR5K_SREV_PHY_5413) {
=======
			   ah->ah_phy_revision == AR5K_SREV_PHY_5413) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			   ah->ah_phy_revision == AR5K_SREV_PHY_5413) {
>>>>>>> refs/remotes/origin/master
			ah->ah_radio = AR5K_RF5413;
			ah->ah_single_chip = true;
			ah->ah_radio_5ghz_revision = AR5K_SREV_RAD_5413;
		} else if (ah->ah_mac_version == (AR5K_SREV_AR2414 >> 4) ||
<<<<<<< HEAD
<<<<<<< HEAD
		ah->ah_phy_revision == AR5K_SREV_PHY_2413) {
=======
			   ah->ah_phy_revision == AR5K_SREV_PHY_2413) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			   ah->ah_phy_revision == AR5K_SREV_PHY_2413) {
>>>>>>> refs/remotes/origin/master
			ah->ah_radio = AR5K_RF2413;
			ah->ah_single_chip = true;
			ah->ah_radio_5ghz_revision = AR5K_SREV_RAD_2413;
		} else {
<<<<<<< HEAD
<<<<<<< HEAD
			ATH5K_ERR(sc, "Couldn't identify radio revision.\n");
=======
			ATH5K_ERR(ah, "Couldn't identify radio revision.\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
			ATH5K_ERR(ah, "Couldn't identify radio revision.\n");
>>>>>>> refs/remotes/origin/master
			ret = -ENODEV;
			goto err;
		}
	}


<<<<<<< HEAD
<<<<<<< HEAD
	/* Return on unsuported chips (unsupported eeprom etc) */
	if ((srev >= AR5K_SREV_AR5416) &&
	(srev < AR5K_SREV_AR2425)) {
		ATH5K_ERR(sc, "Device not yet supported.\n");
=======
	/* Return on unsupported chips (unsupported eeprom etc) */
	if ((srev >= AR5K_SREV_AR5416) && (srev < AR5K_SREV_AR2425)) {
		ATH5K_ERR(ah, "Device not yet supported.\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Return on unsupported chips (unsupported eeprom etc) */
	if ((srev >= AR5K_SREV_AR5416) && (srev < AR5K_SREV_AR2425)) {
		ATH5K_ERR(ah, "Device not yet supported.\n");
>>>>>>> refs/remotes/origin/master
		ret = -ENODEV;
		goto err;
	}

	/*
	 * POST
	 */
	ret = ath5k_hw_post(ah);
	if (ret)
		goto err;

	/* Enable pci core retry fix on Hainan (5213A) and later chips */
	if (srev >= AR5K_SREV_AR5213A)
		AR5K_REG_ENABLE_BITS(ah, AR5K_PCICFG, AR5K_PCICFG_RETRY_FIX);

	/*
	 * Get card capabilities, calibration values etc
	 * TODO: EEPROM work
	 */
	ret = ath5k_eeprom_init(ah);
	if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
		ATH5K_ERR(sc, "unable to init EEPROM\n");
=======
		ATH5K_ERR(ah, "unable to init EEPROM\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ATH5K_ERR(ah, "unable to init EEPROM\n");
>>>>>>> refs/remotes/origin/master
		goto err;
	}

	ee = &ah->ah_capabilities.cap_eeprom;

	/*
	 * Write PCI-E power save settings
	 */
	if ((ah->ah_version == AR5K_AR5212) && pdev && (pci_is_pcie(pdev))) {
		ath5k_hw_reg_write(ah, 0x9248fc00, AR5K_PCIE_SERDES);
		ath5k_hw_reg_write(ah, 0x24924924, AR5K_PCIE_SERDES);

		/* Shut off RX when elecidle is asserted */
		ath5k_hw_reg_write(ah, 0x28000039, AR5K_PCIE_SERDES);
		ath5k_hw_reg_write(ah, 0x53160824, AR5K_PCIE_SERDES);

<<<<<<< HEAD
<<<<<<< HEAD
		/* If serdes programing is enabled, increase PCI-E
=======
		/* If serdes programming is enabled, increase PCI-E
>>>>>>> refs/remotes/origin/cm-10.0
=======
		/* If serdes programming is enabled, increase PCI-E
>>>>>>> refs/remotes/origin/master
		 * tx power for systems with long trace from host
		 * to minicard connector. */
		if (ee->ee_serdes)
			ath5k_hw_reg_write(ah, 0xe5980579, AR5K_PCIE_SERDES);
		else
			ath5k_hw_reg_write(ah, 0xf6800579, AR5K_PCIE_SERDES);

		/* Shut off PLL and CLKREQ active in L1 */
		ath5k_hw_reg_write(ah, 0x001defff, AR5K_PCIE_SERDES);

		/* Preserve other settings */
		ath5k_hw_reg_write(ah, 0x1aaabe40, AR5K_PCIE_SERDES);
		ath5k_hw_reg_write(ah, 0xbe105554, AR5K_PCIE_SERDES);
		ath5k_hw_reg_write(ah, 0x000e3007, AR5K_PCIE_SERDES);

		/* Reset SERDES to load new settings */
		ath5k_hw_reg_write(ah, 0x00000000, AR5K_PCIE_SERDES_RESET);
<<<<<<< HEAD
<<<<<<< HEAD
		mdelay(1);
=======
		usleep_range(1000, 1500);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		usleep_range(1000, 1500);
>>>>>>> refs/remotes/origin/master
	}

	/* Get misc capabilities */
	ret = ath5k_hw_set_capabilities(ah);
	if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
		ATH5K_ERR(sc, "unable to get device capabilities\n");
		goto err;
	}

	if (test_bit(ATH_STAT_2G_DISABLED, sc->status)) {
		__clear_bit(AR5K_MODE_11B, ah->ah_capabilities.cap_mode);
		__clear_bit(AR5K_MODE_11G, ah->ah_capabilities.cap_mode);
	}

	/* Crypto settings */
	common->keymax = (sc->ah->ah_version == AR5K_AR5210 ?
=======
=======
>>>>>>> refs/remotes/origin/master
		ATH5K_ERR(ah, "unable to get device capabilities\n");
		goto err;
	}

	/* Crypto settings */
	common->keymax = (ah->ah_version == AR5K_AR5210 ?
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			  AR5K_KEYTABLE_SIZE_5210 : AR5K_KEYTABLE_SIZE_5211);

	if (srev >= AR5K_SREV_AR5212_V4 &&
	    (ee->ee_version < AR5K_EEPROM_VERSION_5_0 ||
	    !AR5K_EEPROM_AES_DIS(ee->ee_misc5)))
		common->crypt_caps |= ATH_CRYPT_CAP_CIPHER_AESCCM;

	if (srev >= AR5K_SREV_AR2414) {
		common->crypt_caps |= ATH_CRYPT_CAP_MIC_COMBINED;
		AR5K_REG_ENABLE_BITS(ah, AR5K_MISC_MODE,
			AR5K_MISC_MODE_COMBINED_MIC);
	}

	/* MAC address is cleared until add_interface */
<<<<<<< HEAD
<<<<<<< HEAD
	ath5k_hw_set_lladdr(ah, (u8[ETH_ALEN]){});
=======
	ath5k_hw_set_lladdr(ah, zero_mac);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ath5k_hw_set_lladdr(ah, zero_mac);
>>>>>>> refs/remotes/origin/master

	/* Set BSSID to bcast address: ff:ff:ff:ff:ff:ff for now */
	memcpy(common->curbssid, ath_bcast_mac, ETH_ALEN);
	ath5k_hw_set_bssid(ah);
<<<<<<< HEAD
<<<<<<< HEAD
	ath5k_hw_set_opmode(ah, sc->opmode);
=======
	ath5k_hw_set_opmode(ah, ah->opmode);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ath5k_hw_set_opmode(ah, ah->opmode);
>>>>>>> refs/remotes/origin/master

	ath5k_hw_rfgain_opt_init(ah);

	ath5k_hw_init_nfcal_hist(ah);

	/* turn on HW LEDs */
	ath5k_hw_set_ledstate(ah, AR5K_LED_INIT);

	return 0;
err:
	return ret;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * ath5k_hw_deinit - Free the ath5k_hw struct
 *
=======
 * ath5k_hw_deinit() - Free the &struct ath5k_hw
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * ath5k_hw_deinit() - Free the &struct ath5k_hw
>>>>>>> refs/remotes/origin/master
 * @ah: The &struct ath5k_hw
 */
void ath5k_hw_deinit(struct ath5k_hw *ah)
{
<<<<<<< HEAD
<<<<<<< HEAD
	__set_bit(ATH_STAT_INVALID, ah->ah_sc->status);
=======
	__set_bit(ATH_STAT_INVALID, ah->status);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	__set_bit(ATH_STAT_INVALID, ah->status);
>>>>>>> refs/remotes/origin/master

	if (ah->ah_rf_banks != NULL)
		kfree(ah->ah_rf_banks);

	ath5k_eeprom_detach(ah);

	/* assume interrupts are down */
}
