/* Copyright (c) 2011-2013, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio_event.h>
#include <linux/i2c-gpio.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <mach/board.h>
#include <mach/msm_iomap.h>
#include <mach/msm_hsusb.h>
#include <mach/rpc_hsusb.h>
#include <mach/rpc_pmapp.h>
#include <mach/usbdiag.h>
#include <mach/usb_gadget_fserial.h>
#include <mach/msm_memtypes.h>
#include <mach/msm_serial_hs.h>
#include <linux/usb/android.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <mach/gpio_jena.h>
#include <mach/pmic.h>
#include <mach/socinfo.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <asm/mach/mmc.h>
#include <linux/i2c.h>
#include <linux/i2c/sx150x.h>
#include <linux/gpio.h>
#include <linux/android_pmem.h>
#include <linux/bootmem.h>
#include <linux/mfd/marimba.h>
#include <mach/vreg.h>
#include <linux/power_supply.h>
#include <linux/regulator/consumer.h>
#include <mach/rpc_pmapp.h>
#include <mach/msm_battery_jena.h>
#include <linux/smsc911x.h>
#include "devices.h"
#include "timer.h"
#include "board-msm7x27a-regulator.h"
#include "devices-msm7x2xa.h"
#include "pm.h"
#ifdef CONFIG_SAMSUNG_JACK
#include <linux/sec_jack.h>
#endif
#include <mach/rpc_server_handset.h>
#include <mach/socinfo.h>
#include <linux/fsaxxxx_usbsw.h>
#include "proc_comm.h"
#include "pm-boot.h"
#ifdef CONFIG_PROXIMITY_SENSOR
#include <linux/gp2a.h>
#endif

#define _CONFIG_MACH_JENA // Temporary flag
#define _CONFIG_MACH_TREBON // Temporary flag

#define PMEM_KERNEL_EBI1_SIZE	0x3A000
#define MSM_PMEM_AUDIO_SIZE	0x5B000
#define BAHAMA_SLAVE_ID_FM_ADDR         0x2A
#define BAHAMA_SLAVE_ID_QMEMBIST_ADDR   0x7B
#define BAHAMA_SLAVE_ID_FM_REG 0x02
#define FM_GPIO	83

#if defined(CONFIG_PN544)
#include <linux/pn544.h>
#endif
#if (CONFIG_MACH_TREBON_HWREV == 0x0)
#define GPIO_BLUETOOTH_LDO 82
#endif

#ifdef CONFIG_BQ27425_FUEL_GAUGE
#ifdef CONFIG_FUEL_GPIO
#define FUEL_I2C_SCL 79
#define FUEL_I2C_SDA 78
#else
#define FUEL_I2C_SCL 78
#define FUEL_I2C_SDA 79
#endif
#endif

int charging_boot;
EXPORT_SYMBOL(charging_boot);
int	fota_boot;
EXPORT_SYMBOL(fota_boot);

#define WLAN_33V_CONTROL_FOR_BT_ANTENNA

#define WLAN_OK (0)
#define WLAN_ERROR (-1)

#ifdef WLAN_33V_CONTROL_FOR_BT_ANTENNA
#define WLAN_33V_WIFI_FLAG (0x01)
#define WLAN_33V_BT_FLAG (0x02)

int wlan_33v_flag;

static int wlan_setup_ldo_33v(int input_flag, int on);
#endif

#ifdef CONFIG_SAMSUNG_JACK

#define GPIO_JACK_S_35	48
#define GPIO_SEND_END	92

static struct sec_jack_zone jack_zones[] = {
	[0] = {
		.adc_high	= 3,
		.delay_ms	= 10,
		.check_count	= 5,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[1] = {
		.adc_high	= 99,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[2] = {
		.adc_high	= 9999,
		.delay_ms	= 10,
		.check_count	= 5,
		.jack_type	= SEC_HEADSET_4POLE,
	},
};

int get_msm7x27a_det_jack_state(void)
{
	/* Active Low */
	return(gpio_get_value(GPIO_JACK_S_35)) ^ 1;
}
EXPORT_SYMBOL(get_msm7x27a_det_jack_state);

static int get_msm7x27a_send_key_state(void)
{
	return 0;
}

#define SMEM_PROC_COMM_MICBIAS_ONOFF		PCOM_OEM_MICBIAS_ONOFF
#define SMEM_PROC_COMM_MICBIAS_ONOFF_REG5	PCOM_OEM_MICBIAS_ONOFF_REG5
#define SMEM_PROC_COMM_GET_ADC				PCOM_OEM_SAMSUNG_GET_ADC

enum {
	SMEM_PROC_COMM_GET_ADC_BATTERY = 0x0,
	SMEM_PROC_COMM_GET_ADC_TEMP,
	SMEM_PROC_COMM_GET_ADC_VF,
	SMEM_PROC_COMM_GET_ADC_ALL, // data1 : VF(MSB 2 bytes) vbatt_adc(LSB 2bytes), data2 : temp_adc
	SMEM_PROC_COMM_GET_ADC_EAR_ADC,		// 3PI_ADC
	SMEM_PROC_COMM_GET_ADC_MAX,
};

enum {
	SMEM_PROC_COMM_MICBIAS_CONTROL_OFF = 0x0,
	SMEM_PROC_COMM_MICBIAS_CONTROL_ON,
	SMEM_PROC_COMM_MICBIAS_CONTROL_MAX
};

static void set_msm7x27a_micbias_state_reg5(bool state) {}
static bool cur_state = false;

static void set_msm7x27a_micbias_state(bool state)
{
	if(cur_state == state)
	{
		pr_info("sec_jack : earmic_bias same as cur_state\n");
		return;
	}
	if(state)
	{
		pmic_hsed_enable(PM_HSED_CONTROLLER_0, PM_HSED_ENABLE_ALWAYS);
		msleep(130);
		cur_state = true;
	}
	else
	{
		pmic_hsed_enable(PM_HSED_CONTROLLER_0, PM_HSED_ENABLE_OFF);
		cur_state = false;
	}

	report_headset_status(state);
	pr_info("sec_jack : earmic_bias %s\n", state?"on":"off");
}

static int sec_jack_get_adc_value(void)
{
	return current_jack_type;
}

void sec_jack_gpio_init(void)
{
	gpio_tlmm_config(GPIO_CFG(GPIO_JACK_S_35, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
	if(gpio_request(GPIO_JACK_S_35, "h2w_detect")<0)
		pr_err("sec_jack:gpio_request fail\n");
	if(gpio_direction_input(GPIO_JACK_S_35)<0)
		pr_err("sec_jack:gpio_direction fail\n");
}

static struct sec_jack_platform_data sec_jack_data = {
	.get_det_jack_state	= get_msm7x27a_det_jack_state,
	.get_send_key_state	= get_msm7x27a_send_key_state,
	.set_micbias_state	= set_msm7x27a_micbias_state,
	.set_micbias_state_reg5	= set_msm7x27a_micbias_state_reg5,
	.get_adc_value	= sec_jack_get_adc_value,
	.zones		= jack_zones,
	.num_zones	= ARRAY_SIZE(jack_zones),
	.det_int	= MSM_GPIO_TO_INT(GPIO_JACK_S_35),
	.send_int	= MSM_GPIO_TO_INT(GPIO_SEND_END),
};

static struct platform_device sec_device_jack = {
	.name           = "sec_jack",
	.id             = -1,
	.dev            = {
		.platform_data  = &sec_jack_data,
	},
};
#endif

enum {
	GPIO_EXPANDER_IRQ_BASE	= NR_MSM_IRQS + NR_GPIO_IRQS,
	GPIO_EXPANDER_GPIO_BASE	= NR_MSM_GPIOS,
	/* SURF expander */
	GPIO_CORE_EXPANDER_BASE	= GPIO_EXPANDER_GPIO_BASE,
	GPIO_BT_SYS_REST_EN	= GPIO_CORE_EXPANDER_BASE,
	GPIO_WLAN_EXT_POR_N,
	GPIO_DISPLAY_PWR_EN,
	GPIO_BACKLIGHT_EN,
	GPIO_PRESSURE_XCLR,
	GPIO_VREG_S3_EXP,
	GPIO_UBM2M_PWRDWN,
	GPIO_ETM_MODE_CS_N,
	GPIO_HOST_VBUS_EN,
	xGPIO_SPI_MOSI,
	xGPIO_SPI_MISO,
	xGPIO_SPI_CLK,
	xGPIO_SPI_CS0_N,
	GPIO_CORE_EXPANDER_IO13,
	GPIO_CORE_EXPANDER_IO14,
	GPIO_CORE_EXPANDER_IO15,
	/* Camera expander */
	GPIO_CAM_EXPANDER_BASE	= GPIO_CORE_EXPANDER_BASE + 16,
	GPIO_CAM_GP_STROBE_READY	= GPIO_CAM_EXPANDER_BASE,
	GPIO_CAM_GP_AFBUSY,
	GPIO_CAM_GP_CAM_PWDN,
	GPIO_CAM_GP_CAM1MP_XCLR,
	GPIO_CAM_GP_CAMIF_RESET_N,
	GPIO_CAM_GP_STROBE_CE,
	GPIO_CAM_GP_LED_EN1,
	GPIO_CAM_GP_LED_EN2,
};

#if defined(CONFIG_GPIO_SX150X)
enum {
	SX150X_CORE,
	SX150X_CAM,
};

static struct sx150x_platform_data sx150x_data[] __initdata = {
	[SX150X_CORE]	= {
		.gpio_base		= GPIO_CORE_EXPANDER_BASE,
		.oscio_is_gpo		= false,
		.io_pullup_ena		= 0,
		.io_pulldn_ena		= 0,
		.io_open_drain_ena	= 0,
		.irq_summary		= -1,
	},
	[SX150X_CAM]	= {
		.gpio_base		= GPIO_CAM_EXPANDER_BASE,
		.oscio_is_gpo		= false,
		.io_pullup_ena		= 0,
		.io_pulldn_ena		= 0,
		.io_open_drain_ena	= 0,
		.irq_summary		= -1,
	},
};
#endif

extern unsigned int board_hw_revision;
extern unsigned int kernel_uart_flag;

/* FM Platform power and shutdown routines */
#define FPGA_MSM_CNTRL_REG2 0x90008010

static void config_pcm_i2s_mode(int mode)
{
	void __iomem *cfg_ptr;
	u8 reg2;

	cfg_ptr = ioremap_nocache(FPGA_MSM_CNTRL_REG2, sizeof(char));

	if (!cfg_ptr)
		return;
	if (mode) {
		/*enable the pcm mode in FPGA*/
		reg2 = readb_relaxed(cfg_ptr);
		if (reg2 == 0) {
			reg2 = 1;
			writeb_relaxed(reg2, cfg_ptr);
		}
	} else {
		/*enable i2s mode in FPGA*/
		reg2 = readb_relaxed(cfg_ptr);
		if (reg2 == 1) {
			reg2 = 0;
			writeb_relaxed(reg2, cfg_ptr);
		}
	}
	iounmap(cfg_ptr);
}

static unsigned fm_i2s_config_power_on[] = {
	/*FM_I2S_SD*/
	GPIO_CFG(68, 1, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	/*FM_I2S_WS*/
	GPIO_CFG(70, 1, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	/*FM_I2S_SCK*/
	GPIO_CFG(71, 1, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};

static unsigned fm_i2s_config_power_off[] = {
	/*FM_I2S_SD*/
	GPIO_CFG(68, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/*FM_I2S_WS*/
	GPIO_CFG(70, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/*FM_I2S_SCK*/
	GPIO_CFG(71, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
};

static unsigned bt_config_power_on[] = {
	/*RFR*/
	GPIO_CFG(43, 2, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	/*CTS*/
	GPIO_CFG(44, 2, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	/*RX*/
	GPIO_CFG(45, 2, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	/*TX*/
	GPIO_CFG(46, 2, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};
static unsigned bt_config_pcm_on[] = {
	/*PCM_DOUT*/
	GPIO_CFG(68, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	/*PCM_DIN*/
	GPIO_CFG(69, 1, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	/*PCM_SYNC*/
	GPIO_CFG(70, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	/*PCM_CLK*/
	GPIO_CFG(71, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};
static unsigned bt_config_power_off[] = {
	/*RFR*/
	GPIO_CFG(43, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
	/*CTS*/
	GPIO_CFG(44, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
	/*RX*/
	GPIO_CFG(45, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
	/*TX*/
	GPIO_CFG(46, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
};
static unsigned bt_config_pcm_off[] = {
	/*PCM_DOUT*/
	GPIO_CFG(68, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/*PCM_DIN*/
	GPIO_CFG(69, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/*PCM_SYNC*/
	GPIO_CFG(70, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	/*PCM_CLK*/
	GPIO_CFG(71, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
};


static int config_i2s(int mode)
{
	int pin, rc = 0;

	if (mode == FM_I2S_ON) {
		if (machine_is_msm7x27a_surf())
			config_pcm_i2s_mode(0);
		pr_err("%s mode = FM_I2S_ON", __func__);
		for (pin = 0; pin < ARRAY_SIZE(fm_i2s_config_power_on);
			pin++) {
				rc = gpio_tlmm_config(
					fm_i2s_config_power_on[pin],
					GPIO_CFG_ENABLE
					);
				if (rc < 0)
					return rc;
			}
	} else if (mode == FM_I2S_OFF) {
		pr_err("%s mode = FM_I2S_OFF", __func__);
		for (pin = 0; pin < ARRAY_SIZE(fm_i2s_config_power_off);
			pin++) {
				rc = gpio_tlmm_config(
					fm_i2s_config_power_off[pin],
					GPIO_CFG_ENABLE
					);
				if (rc < 0)
					return rc;
			}
	}
	return rc;
}
static int config_pcm(int mode)
{
	int pin, rc = 0;

	if (mode == BT_PCM_ON) {
		if (machine_is_msm7x27a_surf())
			config_pcm_i2s_mode(1);
		pr_err("%s mode =BT_PCM_ON", __func__);
		for (pin = 0; pin < ARRAY_SIZE(bt_config_pcm_on);
			pin++) {
				rc = gpio_tlmm_config(bt_config_pcm_on[pin],
					GPIO_CFG_ENABLE);
				if (rc < 0)
					return rc;
			}
	} else if (mode == BT_PCM_OFF) {
		pr_err("%s mode =BT_PCM_OFF", __func__);
		for (pin = 0; pin < ARRAY_SIZE(bt_config_pcm_off);
			pin++) {
				rc = gpio_tlmm_config(bt_config_pcm_off[pin],
					GPIO_CFG_ENABLE);
				if (rc < 0)
					return rc;
			}

	}

	return rc;
}

static int msm_bahama_setup_pcm_i2s(int mode)
{
	int fm_state = 0, bt_state = 0;
	int rc = 0;
	struct marimba config = { .mod_id =  SLAVE_ID_BAHAMA};

	fm_state = marimba_get_fm_status(&config);
	bt_state = marimba_get_bt_status(&config);

	switch (mode) {
	case BT_PCM_ON:
	case BT_PCM_OFF:
		if (!fm_state)
			rc = config_pcm(mode);
		break;
	case FM_I2S_ON:
		rc = config_i2s(mode);
		break;
	case FM_I2S_OFF:
		if (bt_state)
			rc = config_pcm(BT_PCM_ON);
		else
			rc = config_i2s(mode);
		break;
	default:
		rc = -EIO;
		pr_err("%s:Unsupported mode", __func__);
	}
	return rc;
}

static int bt_set_gpio(int on)
{
	int rc = 0;
	struct marimba config = { .mod_id =  SLAVE_ID_BAHAMA};

	if (on) {
		rc = gpio_direction_output(GPIO_BT_PWR, 1);
		msleep(100);
	} else {
		if (!marimba_get_fm_status(&config) &&
				!marimba_get_bt_status(&config)) {
			gpio_set_value_cansleep(GPIO_BT_PWR, 0);
			rc = gpio_direction_input(GPIO_BT_PWR);
			msleep(100);
		}
	}
	if (rc)
		pr_err("%s: BT sys_reset_en GPIO : Error", __func__);

	return rc;
}
static struct regulator *fm_regulator;
static int fm_radio_setup(struct marimba_fm_platform_data *pdata)
{
	int rc = 0;
	const char *id = "FMPW";
	uint32_t irqcfg;
	struct marimba config = { .mod_id =  SLAVE_ID_BAHAMA};
	u8 value;

	/* Voting for 1.8V Regulator */
	fm_regulator = regulator_get(NULL , "vreg_msme");
	if (IS_ERR(fm_regulator)) {
		pr_err("%s: regulator get failed with : (%ld)\n",
			__func__, PTR_ERR(fm_regulator));
		return -EINVAL;
	}

	/* Set the voltage level to 1.8V */
	rc = regulator_set_voltage(fm_regulator, 1800000, 1800000);
	if (rc < 0) {
		pr_err("%s: set regulator level failed with :(%d)\n",
			__func__, rc);
		goto fm_regulator_fail;
	}

	/* Enabling the 1.8V regulator */
	rc = regulator_enable(fm_regulator);
	if (rc) {
		pr_err("%s: enable regulator failed with :(%d)\n",
			__func__, rc);
		goto fm_regulator_fail;
	}

	/* Voting for 19.2MHz clock */
	rc = pmapp_clock_vote(id, PMAPP_CLOCK_ID_D1,
			PMAPP_CLOCK_VOTE_ON);
	if (rc < 0) {
		pr_err("%s: clock vote failed with :(%d)\n",
			 __func__, rc);
		goto fm_clock_vote_fail;
	}

	rc = bt_set_gpio(1);
	if (rc) {
		pr_err("%s: bt_set_gpio = %d", __func__, rc);
		goto fm_gpio_config_fail;
	}
	/*re-write FM Slave Id, after reset*/
	value = BAHAMA_SLAVE_ID_FM_ADDR;
	rc = marimba_write_bit_mask(&config,
			BAHAMA_SLAVE_ID_FM_REG, &value, 1, 0xFF);
	if (rc < 0) {
		pr_err("%s: FM Slave ID rewrite Failed = %d", __func__, rc);
		goto fm_gpio_config_fail;
	}
	/* Configuring the FM GPIO */
	irqcfg = GPIO_CFG(FM_GPIO, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL,
			GPIO_CFG_2MA);

	rc = gpio_tlmm_config(irqcfg, GPIO_CFG_ENABLE);
	if (rc) {
		pr_err("%s: gpio_tlmm_config(%#x)=%d\n",
			 __func__, irqcfg, rc);
		goto fm_gpio_config_fail;
	}
	return 0;

fm_gpio_config_fail:
	pmapp_clock_vote(id, PMAPP_CLOCK_ID_D1,
		PMAPP_CLOCK_VOTE_OFF);
	bt_set_gpio(0);
fm_clock_vote_fail:
	regulator_disable(fm_regulator);

fm_regulator_fail:
	regulator_put(fm_regulator);

	return rc;
};

static void fm_radio_shutdown(struct marimba_fm_platform_data *pdata)
{
	int rc;
	const char *id = "FMPW";

	/* Releasing the GPIO line used by FM */
	uint32_t irqcfg = GPIO_CFG(FM_GPIO, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP,
		GPIO_CFG_2MA);

	rc = gpio_tlmm_config(irqcfg, GPIO_CFG_ENABLE);
	if (rc)
		pr_err("%s: gpio_tlmm_config(%#x)=%d\n",
			 __func__, irqcfg, rc);

	/* Releasing the 1.8V Regulator */
	if (fm_regulator != NULL) {
		rc = regulator_disable(fm_regulator);

		if (rc)
			pr_err("%s: disable regulator failed:(%d)\n",
				__func__, rc);
		fm_regulator = NULL;
	}

	/* Voting off the clock */
	rc = pmapp_clock_vote(id, PMAPP_CLOCK_ID_D1,
		PMAPP_CLOCK_VOTE_OFF);
	if (rc < 0)
		pr_err("%s: voting off failed with :(%d)\n",
			__func__, rc);
	rc = bt_set_gpio(0);
	if (rc)
		pr_err("%s: bt_set_gpio = %d", __func__, rc);
}


#ifndef ATH_POLLING
static void (*wlan_status_notify_cb)(int card_present, void *dev_id);
void *wlan_devid;

static int register_wlan_status_notify(void (*callback)(int card_present,
					void *dev_id), void *dev_id)
{
	printk(KERN_INFO "%s --enter\n", __func__);

	wlan_status_notify_cb = callback;
	wlan_devid = dev_id;
	return 0;
}

static unsigned int wlan_status(struct device *dev)
{
	int rc;

	printk(KERN_INFO "%s entered\n", __func__);

	rc = gpio_get_value(GPIO_WLAN_RESET_N/*gpio_wlan_reset_n*/);

	return rc;
}
#endif /* ATH_POLLING */

static struct marimba_fm_platform_data marimba_fm_pdata = {
	.fm_setup = fm_radio_setup,
	.fm_shutdown = fm_radio_shutdown,
	.irq = MSM_GPIO_TO_INT(FM_GPIO),
	.vreg_s2 = NULL,
	.vreg_xo_out = NULL,
	/* Configuring the FM SoC as I2S Master */
	.is_fm_soc_i2s_master = true,
	.config_i2s_gpio = msm_bahama_setup_pcm_i2s,
};

static struct platform_device msm_wlan_ar6000_pm_device = {
	.name           = "wlan_ar6000_pm_dev",
	.id             = -1,
};

#if defined(CONFIG_BT) && defined(CONFIG_MARIMBA_CORE)
static struct platform_device msm_bt_power_device = {
	.name = "bt_power",
};

struct bahama_config_register {
	u8 reg;
	u8 value;
	u8 mask;
};

static const char * const regulators_bahama_name[] = {
	"vreg_msme",
#if (CONFIG_MACH_TREBON_HWREV != 0x0)
	"vbt",
#endif
};

static struct regulator *regulators_bahama[ARRAY_SIZE(regulators_bahama_name)];

static int bahama_bt(int on)
{

	int rc = 0;
	int i;

	struct marimba config = { .mod_id =  SLAVE_ID_BAHAMA};

	struct bahama_variant_register {
		const size_t size;
		const struct bahama_config_register *set;
	};

	const struct bahama_config_register *p;

	u8 version;

	const struct bahama_config_register v10_bt_on[] = {
		{ 0xE9, 0x00, 0xFF },
		{ 0xF4, 0x80, 0xFF },
		{ 0xE4, 0x00, 0xFF },
		{ 0xE5, 0x00, 0x0F },
#ifdef CONFIG_WLAN
		{ 0xE6, 0x38, 0x7F },
		{ 0xE7, 0x06, 0xFF },
#endif
		{ 0xE9, 0x21, 0xFF },
		{ 0x01, 0x0C, 0x1F },
		{ 0x01, 0x08, 0x1F },
	};

	const struct bahama_config_register v20_bt_on_fm_off[] = {
		{ 0x11, 0x0C, 0xFF },
		{ 0x13, 0x01, 0xFF },
		{ 0xF4, 0x80, 0xFF },
		{ 0xF0, 0x00, 0xFF },
		{ 0xE9, 0x00, 0xFF },
#ifdef CONFIG_WLAN
		{ 0x81, 0x00, 0x7F },
		{ 0x82, 0x00, 0xFF },
		{ 0xE6, 0x38, 0x7F },
		{ 0xE7, 0x06, 0xFF },
#endif
		{ 0x8E, 0x15, 0xFF },
		{ 0x8F, 0x15, 0xFF },
		{ 0x90, 0x15, 0xFF },

		{ 0xE9, 0x21, 0xFF },
	};

	const struct bahama_config_register v20_bt_on_fm_on[] = {
		{ 0x11, 0x0C, 0xFF },
		{ 0x13, 0x01, 0xFF },
		{ 0xF4, 0x86, 0xFF },
		{ 0xF0, 0x06, 0xFF },
		{ 0xE9, 0x00, 0xFF },
#ifdef CONFIG_WLAN
		{ 0x81, 0x00, 0x7F },
		{ 0x82, 0x00, 0xFF },
		{ 0xE6, 0x38, 0x7F },
		{ 0xE7, 0x06, 0xFF },
#endif
		{ 0xE9, 0x21, 0xFF },
	};

	const struct bahama_config_register v10_bt_off[] = {
		{ 0xE9, 0x00, 0xFF },
	};

	const struct bahama_config_register v20_bt_off_fm_off[] = {
		{ 0xF4, 0x84, 0xFF },
		{ 0xF0, 0x04, 0xFF },
		{ 0xE9, 0x00, 0xFF }
	};

	const struct bahama_config_register v20_bt_off_fm_on[] = {
		{ 0xF4, 0x86, 0xFF },
		{ 0xF0, 0x06, 0xFF },
		{ 0xE9, 0x00, 0xFF }
	};
	const struct bahama_variant_register bt_bahama[2][3] = {
	{
		{ ARRAY_SIZE(v10_bt_off), v10_bt_off },
		{ ARRAY_SIZE(v20_bt_off_fm_off), v20_bt_off_fm_off },
		{ ARRAY_SIZE(v20_bt_off_fm_on), v20_bt_off_fm_on }
	},
	{
		{ ARRAY_SIZE(v10_bt_on), v10_bt_on },
		{ ARRAY_SIZE(v20_bt_on_fm_off), v20_bt_on_fm_off },
		{ ARRAY_SIZE(v20_bt_on_fm_on), v20_bt_on_fm_on }
	}
	};

	u8 offset = 0; /* index into bahama configs */
	on = on ? 1 : 0;
	version = marimba_read_bahama_ver(&config);
	if ((int)version < 0 || version == BAHAMA_VER_UNSUPPORTED) {
		dev_err(&msm_bt_power_device.dev, "%s: Bahama \
				version read Error, version = %d \n",
				__func__, version);
		return -EIO;
	}

	if (version == BAHAMA_VER_2_0) {
		if (marimba_get_fm_status(&config))
			offset = 0x01;
	}

	p = bt_bahama[on][version + offset].set;

	dev_info(&msm_bt_power_device.dev,
		"%s: found version %d\n", __func__, version);

	for (i = 0; i < bt_bahama[on][version + offset].size; i++) {
		u8 value = (p+i)->value;
		rc = marimba_write_bit_mask(&config,
			(p+i)->reg,
			&value,
			sizeof((p+i)->value),
			(p+i)->mask);
		if (rc < 0) {
			dev_err(&msm_bt_power_device.dev,
				"%s: reg %x write failed: %d\n",
				__func__, (p+i)->reg, rc);
			return rc;
		}
		dev_dbg(&msm_bt_power_device.dev,
			"%s: reg 0x%02x write value 0x%02x mask 0x%02x\n",
				__func__, (p+i)->reg,
				value, (p+i)->mask);
		value = 0;
		rc = marimba_read_bit_mask(&config,
				(p+i)->reg, &value,
				sizeof((p+i)->value), (p+i)->mask);
		if (rc < 0)
			dev_err(&msm_bt_power_device.dev, "%s marimba_read_bit_mask- error",
					__func__);
		dev_dbg(&msm_bt_power_device.dev,
			"%s: reg 0x%02x read value 0x%02x mask 0x%02x\n",
				__func__, (p+i)->reg,
				value, (p+i)->mask);
	}
	/* Update BT Status */
	if (on)
		marimba_set_bt_status(&config, true);
	else
		marimba_set_bt_status(&config, false);
	return rc;
}

static int bluetooth_switch_regulators(int on)
{
	int i, rc = 0;

	for (i = 0; i < ARRAY_SIZE(regulators_bahama_name); i++) {
		if (!regulators_bahama[i]) {
			pr_err("%s: regulator_get %s failed(%d)\n",
			__func__, regulators_bahama_name[i], rc);
			goto regulator_fail;
		}

		rc = on ? regulator_set_voltage(regulators_bahama[i],
					i ? 2900000 : 1800000,
						i ? 2900000 : 1800000) : 0;

		if (rc < 0) {
			pr_err("%s: regulator set level failed (%d)\n",
					__func__, rc);
			goto regulator_set_voltage_fail;
		}

		rc = on ? regulator_enable(regulators_bahama[i]) :
			  regulator_disable(regulators_bahama[i]);

		if (rc < 0) {
			pr_err("%s: regulator %s %s failed(%d)\n",
				__func__, regulators_bahama_name[i],
			       on ? "enable" : "disable", rc);
			goto regulator_fail;
			}
	}
	return rc;

regulator_fail:
	while (i) {
		if (on)
			regulator_disable(regulators_bahama[--i]);
		}
regulator_set_voltage_fail:
	regulator_put(regulators_bahama[0]);
	regulator_put(regulators_bahama[1]);
	return rc;
}


#if (CONFIG_MACH_TREBON_HWREV == 0x0)
static int bluetooth_setup_ldo(unsigned gpio, int on)
{
	int rc = 0;

	printk("%s - %d : %s\n", __func__, gpio, on ? "on" : "off");

	// Request
	if (gpio_request(gpio, "bt_en_gpio")) {
		printk(KERN_ERR "%s: gpio_request for %d failed\n",
				__func__, gpio);
		return -1;
	}
	int temp = gpio_get_value(gpio);
       printk( "%s:gpio_direction_output before(%d):: gpio_get_value=%d\n", __func__, on, temp);

	rc = gpio_direction_output(gpio, on);

        temp = gpio_get_value(gpio);
       printk( "%s:gpio_direction_output after(%d):: gpio_get_value=%d\n", __func__, on, temp);

	gpio_free(gpio);

	if (rc) {
		printk(KERN_ERR "%s: gpio_direction_output for %d failed\n",
				__func__, gpio);
		return -1;
	}
	return 0;
}
#endif

static unsigned int msm_bahama_setup_power(void)
{
	printk("%s -msm_bahama_setup_power\n", __func__);

	int rc = 0;
	struct regulator *regulator_s3 = NULL;

	regulator_s3 = regulator_get(NULL, "vreg_msme");
	if (IS_ERR(regulator_s3)) {
		pr_err("%s: regulator get failed (%ld)\n",
			__func__, PTR_ERR(regulator_s3));
		return PTR_ERR(regulator_s3);
	}
	rc = regulator_set_voltage(regulator_s3, 1800000, 1800000);
	if (rc < 0) {
		pr_err("%s: regulator set level failed (%d)\n",
				__func__, rc);
		goto regulator_fail;
	}
	rc = regulator_enable(regulator_s3);
	if (rc < 0) {
		pr_err("%s: regulator enable failed (%d)\n",
		       __func__, rc);
		goto regulator_fail;
	}

	/* Setup Bahama_sys_reset_n */
	rc = gpio_request(GPIO_BT_PWR, "bahama sys_rst_n");
	if (rc < 0) {
		pr_err("%s: gpio_request %d = %d\n", __func__,
			GPIO_BT_PWR, rc);
		goto regulator_fail;
	}

	rc = bt_set_gpio(1);
	if (rc < 0) {
		pr_err("%s: bt_set_gpio %d = %d\n", __func__,
			GPIO_BT_PWR, rc);
		goto gpio_fail;
	}
	//2011.07.06 qcomm - bt on failed
	msleep(100);

	return rc;

gpio_fail:
	gpio_free(GPIO_BT_PWR);
regulator_fail:
	regulator_put(regulator_s3);
	return rc;
}

static unsigned int msm_bahama_shutdown_power(int value)
{
       printk("%s -msm_bahama_shutdown_power\n", __func__);

	int rc = 0;
	struct regulator *regulator_s3 = NULL;

	regulator_s3 = regulator_get(NULL, "vreg_msme");
	if (IS_ERR(regulator_s3)) {
		pr_err("%s: regulator get failed (%ld)\n",
			__func__, PTR_ERR(regulator_s3));
		return PTR_ERR(regulator_s3);
	}
	rc = regulator_disable(regulator_s3);
	if (rc) {
		pr_err("%s: regulator disable failed (%d)\n",
		       __func__, rc);
		regulator_put(regulator_s3);
		return rc;
	}

	if (value == BAHAMA_ID) {
		rc = bt_set_gpio(0);
		if (rc) {
			pr_err("%s: bt_set_gpio = %d\n",
					__func__, rc);
		}
	}
	return rc;
}

static unsigned int msm_bahama_core_config(int type)
{
	int rc = 0;

	if (type == BAHAMA_ID) {
		int i;
		struct marimba config = { .mod_id = SLAVE_ID_BAHAMA };
		const struct bahama_config_register v20_init[] = {
			/* reg, value, mask */
			{ 0xF4, 0x84, 0xFF }, /* AREG */
			{ 0xF0, 0x04, 0xFF } /* DREG */
		};
		if (marimba_read_bahama_ver(&config) == BAHAMA_VER_2_0) {
			for (i = 0; i < ARRAY_SIZE(v20_init); i++) {
				u8 value = v20_init[i].value;
				rc = marimba_write_bit_mask(&config,
					v20_init[i].reg,
					&value,
					sizeof(v20_init[i].value),
					v20_init[i].mask);
				if (rc < 0) {
					pr_err("%s: reg %d write failed: %d\n",
						__func__, v20_init[i].reg, rc);
					return rc;
				}
				pr_debug("%s: reg 0x%02x value 0x%02x"
					" mask 0x%02x\n",
					__func__, v20_init[i].reg,
					v20_init[i].value, v20_init[i].mask);
			}
		}
	}
	rc = bt_set_gpio(0);
	if (rc) {
		pr_err("%s: bt_set_gpio = %d\n",
		       __func__, rc);
	}
	pr_debug("core type: %d\n", type);
	return rc;
}

static int bluetooth_power(int on)
{
	printk("%s -bluetooth_power\n", __func__);

	int pin, rc = 0;
	const char *id = "BTPW";
	int cid = 0;

	cid = adie_get_detected_connectivity_type();
	if (cid != BAHAMA_ID) {
		pr_err("%s: unexpected adie connectivity type: %d\n",
					__func__, cid);
		return -ENODEV;
	}
	if (on) {
#ifdef WLAN_33V_CONTROL_FOR_BT_ANTENNA
		wlan_setup_ldo_33v(WLAN_33V_BT_FLAG, 1);
#endif
		/* Setup power for BT SOC */
		rc = bt_set_gpio(on);
		if (rc) {
			pr_err("%s: bt_set_gpio = %d\n",
					__func__, rc);
			goto exit;
		}
		rc = bluetooth_switch_regulators(on);
		if (rc < 0) {
			pr_err("%s: bluetooth_switch_regulators rc = %d",
					__func__, rc);
			goto exit;
		}

		#if (CONFIG_MACH_TREBON_HWREV == 0x0)
		 if (bluetooth_setup_ldo(GPIO_BLUETOOTH_LDO,1))
		 {
					pr_err("%s: GPIO_BLUETOOTH_LDO fail = %d\n",
							__func__, GPIO_BLUETOOTH_LDO);
		   return -ENODEV;
		  }
			msleep(100);
		#endif
		/* Setup BT GPIO lines*/
		for (pin = 0; pin < ARRAY_SIZE(bt_config_power_on);
			pin++) {
			rc = gpio_tlmm_config(bt_config_power_on[pin],
					GPIO_CFG_ENABLE);
			if (rc < 0) {
				pr_err("%s: gpio_tlmm_config(%#x)=%d\n",
						__func__,
						bt_config_power_on[pin],
						rc);
				goto fail_power;
			}
		}

		/* Setup BT clocks */
		rc = pmapp_clock_vote(id, PMAPP_CLOCK_ID_D1,
			PMAPP_CLOCK_VOTE_ON);
		if (rc < 0) {
			pr_err("Failed to vote for TCXO_D1 ON\n");
			goto fail_clock;
		}
		msleep(20);

		/* I2C config for Bahama */
		rc = bahama_bt(1);
		if (rc < 0) {
			pr_err("%s: bahama_bt rc = %d", __func__, rc);
			goto fail_i2c;
		}
		msleep(20);

		/* Setup BT PCM lines*/
		rc = msm_bahama_setup_pcm_i2s(BT_PCM_ON);
		if (rc < 0) {
			pr_err("%s: msm_bahama_setup_pcm_i2s , rc =%d\n",
				__func__, rc);
				goto fail_power;
			}
		rc = pmapp_clock_vote(id, PMAPP_CLOCK_ID_D1,
				  PMAPP_CLOCK_VOTE_PIN_CTRL);
		if (rc < 0)
			pr_err("%s:Pin Control Failed, rc = %d",
					__func__, rc);
	} else {
		rc = bahama_bt(0);
		if (rc < 0)
			pr_err("%s: bahama_bt rc = %d", __func__, rc);

		#if (CONFIG_MACH_TREBON_HWREV == 0x0)
		 if (bluetooth_setup_ldo(GPIO_BLUETOOTH_LDO,0))
		 {
					pr_err("%s: GPIO_BLUETOOTH_LDO fail = %d\n",
							__func__, GPIO_BLUETOOTH_LDO);
		   return -ENODEV;
		  }
			msleep(100);
		#endif

		rc = bt_set_gpio(on);
		if (rc) {
			pr_err("%s: bt_set_gpio = %d\n",
					__func__, rc);
		}
#ifdef WLAN_33V_CONTROL_FOR_BT_ANTENNA
		wlan_setup_ldo_33v(WLAN_33V_BT_FLAG, 0);
#endif
fail_i2c:
		rc = pmapp_clock_vote(id, PMAPP_CLOCK_ID_D1,
				  PMAPP_CLOCK_VOTE_OFF);
		if (rc < 0)
			pr_err("%s: Failed to vote Off D1\n", __func__);
fail_clock:
		for (pin = 0; pin < ARRAY_SIZE(bt_config_power_off);
			pin++) {
				rc = gpio_tlmm_config(bt_config_power_off[pin],
					GPIO_CFG_ENABLE);
				if (rc < 0) {
					pr_err("%s: gpio_tlmm_config(%#x)=%d\n",
					__func__, bt_config_power_off[pin], rc);
				}
			}
		rc = msm_bahama_setup_pcm_i2s(BT_PCM_OFF);
		if (rc < 0) {
			pr_err("%s: msm_bahama_setup_pcm_i2s, rc =%d\n",
					__func__, rc);
				}
fail_power:
		rc = bluetooth_switch_regulators(0);
		if (rc < 0) {
			pr_err("%s: switch_regulators : rc = %d",\
					__func__, rc);
			goto exit;
		}
	}
	return rc;
exit:
	pr_err("%s: failed with rc = %d", __func__, rc);
	return rc;
}

static int __init bt_power_init(void)
{
	int i, rc = 0;
	for (i = 0; i < ARRAY_SIZE(regulators_bahama_name); i++) {
			regulators_bahama[i] = regulator_get(NULL,
						regulators_bahama_name[i]);
			if (IS_ERR(regulators_bahama[i])) {
				pr_err("%s: regulator get %s failed (%ld)\n",
				       __func__, regulators_bahama_name[i],
				       PTR_ERR(regulators_bahama[i]));
				rc = PTR_ERR(regulators_bahama[i]);
				goto regulator_get_fail;
			}
		}

	msm_bt_power_device.dev.platform_data = &bluetooth_power;

	return rc;

regulator_get_fail:
	while (i)
		regulator_put(regulators_bahama[--i]);
	return rc;
}

static struct marimba_platform_data marimba_pdata = {
	.slave_id[SLAVE_ID_BAHAMA_FM]        = BAHAMA_SLAVE_ID_FM_ADDR,
	.slave_id[SLAVE_ID_BAHAMA_QMEMBIST]  = BAHAMA_SLAVE_ID_QMEMBIST_ADDR,
	.bahama_setup                        = msm_bahama_setup_power,
	.bahama_shutdown                     = msm_bahama_shutdown_power,
	.bahama_core_config                  = msm_bahama_core_config,
	.fm				     			     = &marimba_fm_pdata,
};

#endif

#if defined(CONFIG_I2C) && defined(CONFIG_GPIO_SX150X)
static struct i2c_board_info core_exp_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO("sx1509q", 0x3e),
		.platform_data =  &sx150x_data[SX150X_CORE],
	},
};
static struct i2c_board_info cam_exp_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO("sx1508q", 0x22),
		.platform_data	= &sx150x_data[SX150X_CAM],
	},
};
#endif

#if defined(CONFIG_PN544)
static struct i2c_gpio_platform_data pn544_i2c_gpio_data = {
	.sda_pin    = GPIO_NFC_SDA,
	.scl_pin    = GPIO_NFC_SCL,
};

static struct platform_device pn544_i2c_gpio_device = {
	.name       = "i2c-gpio",
	.id         = 5,
	.dev        = {
	.platform_data  = &pn544_i2c_gpio_data,
	},
};

static struct pn544_i2c_platform_data pn544_pdata __initdata = {
	.irq_gpio = GPIO_NFC_IRQ,
	.ven_gpio = GPIO_NFC_EN,
	.firm_gpio = GPIO_NFC_FIRM,
};

static struct i2c_board_info pn544_i2c_devices[] __initdata = {
	{
		I2C_BOARD_INFO("pn544", 0x2b),
		.platform_data = &pn544_pdata,
		.irq = MSM_GPIO_TO_INT(GPIO_NFC_IRQ),
	},
};

static struct msm_gpio pn544_init_gpio_table[] = {
	{ GPIO_CFG(GPIO_NFC_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), "nfc_ven" },
	{ GPIO_CFG(GPIO_NFC_FIRM, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), "nfc_firm" },
	{ GPIO_CFG(GPIO_NFC_IRQ, 0, GPIO_CFG_INPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), "nfc_int" },
};

static void config_gpio_table_for_nfc(void)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(pn544_init_gpio_table); i++)
		gpio_tlmm_config(pn544_init_gpio_table[i].gpio_cfg,
			GPIO_CFG_ENABLE);
	return;
}
#endif

#if defined(CONFIG_BT) && defined(CONFIG_MARIMBA_CORE)
#ifdef CONFIG_PROXIMITY_SENSOR
static int gp2a_power(bool on)
{
	gpio_tlmm_config(GPIO_CFG( 29, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
	return 0;
}

static struct gp2a_platform_data gp2a_pdata = {
	.p_out = 29,
	.power = gp2a_power,
};
#endif


#if (CONFIG_MACH_TREBON_HWREV == 0x0)
static struct i2c_board_info bahama_devices[] = {
{
	I2C_BOARD_INFO("marimba", 0xc),
	.platform_data = &marimba_pdata,
},
};

static struct i2c_board_info sensor_devices[] = {
	#ifdef CONFIG_SENSORS_HSCD
{
	I2C_BOARD_INFO("bma222", 0x08),
},
{
	I2C_BOARD_INFO("bma222e", 0x18),
},
{
	I2C_BOARD_INFO("hscd_i2c", 0x0d),
},
	#endif
	#ifdef CONFIG_PROXIMITY_SENSOR
{
	I2C_BOARD_INFO("gp2a", 0x44 ),
	.platform_data = &gp2a_pdata,
},
	#endif
};

	#if defined(CONFIG_SENSORS_HSCD) || defined(CONFIG_PROXIMITY_SENSOR)
static struct i2c_gpio_platform_data sensor_i2c_gpio_data = {
	.sda_pin =GPIO_SENSOR_SDA,
	.scl_pin =GPIO_SENSOR_SCL,
	.udelay =1,
};

static struct platform_device sensor_i2c_gpio_device = {
	.name ="i2c-gpio",
	.id= 4,
	.dev = {
		.platform_data =&sensor_i2c_gpio_data,
	},
};
	#endif
#else
static struct i2c_board_info bahama_devices[] = {
{
	I2C_BOARD_INFO("marimba", 0xc),
	.platform_data = &marimba_pdata,
},
	#ifdef CONFIG_SENSORS_HSCD
{
	I2C_BOARD_INFO("accsns_i2c", 0x08),
},
{
	I2C_BOARD_INFO("hscd_i2c", 0x0d),
},
	#endif
	#ifdef CONFIG_PROXIMITY_SENSOR
{
	I2C_BOARD_INFO("gp2a", 0x44 ),
	.platform_data = &gp2a_pdata,
},
	#endif
};
#endif
#endif

static struct platform_device msm_device_pmic_leds = {
	.name	= "pmic-leds",
	.id		= -1,
};

static struct platform_device msm_vibrator_device = {
	.name	= "msm_vibrator",
	.id		= -1,
};

static struct i2c_gpio_platform_data touch_i2c_gpio_data = {
	.sda_pin    = GPIO_TSP_SDA,
	.scl_pin    = GPIO_TSP_SCL,
	.udelay	= 1,
};

static struct platform_device touch_i2c_gpio_device = {
	.name       = "i2c-gpio",
	.id     =  2,
	.dev        = {
		.platform_data  = &touch_i2c_gpio_data,
	},
};

/* I2C 2 */
static struct i2c_board_info touch_i2c_devices[] = {
	{
		I2C_BOARD_INFO("sec_touch", 0x48),
	        .irq = MSM_GPIO_TO_INT( GPIO_TOUCH_IRQ ),
	},
};

struct msm_battery_callback *charger_callbacks;
static enum cable_type_t set_cable_status;
static enum acc_type_t set_acc_status;
static enum ovp_type_t set_ovp_status;

static void msm_battery_register_callback(
		struct msm_battery_callback *ptr)
{
	charger_callbacks = ptr;
	pr_info("[BATT] msm_battery_register_callback start\n");
	if ((set_acc_status != 0) && charger_callbacks
		&& charger_callbacks->set_acc_type)
		charger_callbacks->set_acc_type(charger_callbacks,
		set_acc_status);

	if ((set_cable_status != 0) && charger_callbacks
		&& charger_callbacks->set_cable)
		charger_callbacks->set_cable(charger_callbacks,
		set_cable_status);

	if ((set_ovp_status != 0) && charger_callbacks
		&& charger_callbacks->set_ovp_type)
		charger_callbacks->set_ovp_type(charger_callbacks,
		set_ovp_status);
}

static u32 msm_calculate_batt_capacity(u32 current_voltage);

static struct msm_charger_data aries_charger = {
	.register_callbacks	= msm_battery_register_callback,
};

static struct msm_psy_batt_pdata msm_psy_batt_data = {
	.charger			= &aries_charger,
	.voltage_min_design		= 2800,
	.voltage_max_design	= 4300,
	.avail_chg_sources		= AC_CHG | USB_CHG ,
	.batt_technology		= POWER_SUPPLY_TECHNOLOGY_LION,
};

static struct platform_device msm_batt_device = {
	.name		= "msm-battery",
	.id		= -1,
	.dev.platform_data	= &msm_psy_batt_data,
};

int fsa_cable_type = CABLE_TYPE_UNKNOWN;
int fsa880_get_charger_status(void);
int fsa880_get_charger_status(void)
{
	return fsa_cable_type;
}

static void jena_usb_power(int onoff, char *path) {}

void trebon_chg_connected(enum chg_type chgtype)
{
	char *chg_types[] = {"STD DOWNSTREAM PORT",
			"CARKIT",
			"DEDICATED CHARGER",
			"INVALID"};
	unsigned *data1 = NULL;
	unsigned *data2 = NULL;
	int ret = 0;

	switch (chgtype) {
	case USB_CHG_TYPE__SDP:
		ret = msm_proc_comm(PCOM_CHG_USB_IS_PC_CONNECTED,
				data1, data2);
		break;
	case USB_CHG_TYPE__WALLCHARGER:
		ret = msm_proc_comm(PCOM_CHG_USB_IS_CHARGER_CONNECTED,
				data1, data2);
		break;
	case USB_CHG_TYPE__INVALID:
		ret = msm_proc_comm(PCOM_CHG_USB_IS_DISCONNECTED,
				data1, data2);
		break;
	default:
		break;
	}

	if (ret < 0)
		pr_err("%s: connection err, ret=%d\n", __func__, ret);

	pr_info("\nCharger Type: %s\n", chg_types[chgtype]);
}

static void jena_usb_cb(u8 attached, struct fsausb_ops *ops)
{
	pr_info("[BATT] [%s] Board file [FSA880]: USB Callback\n", __func__);

	set_acc_status = attached ? ACC_TYPE_USB : ACC_TYPE_NONE;
	if (charger_callbacks && charger_callbacks->set_acc_type)
		charger_callbacks->set_acc_type(charger_callbacks,
		set_acc_status);

	set_cable_status = attached ? CABLE_TYPE_USB : CABLE_TYPE_UNKNOWN;
	if (charger_callbacks && charger_callbacks->set_cable)
		charger_callbacks->set_cable(charger_callbacks,
		set_cable_status);
}

static void jena_charger_cb(u8 attached, struct fsausb_ops *ops)
{
	pr_info("[BATT] Board file [FSA880]: Charger Callback\n");

	set_acc_status = attached ? ACC_TYPE_CHARGER : ACC_TYPE_NONE;
	if (charger_callbacks && charger_callbacks->set_acc_type)
		charger_callbacks->set_acc_type(charger_callbacks,
		set_acc_status);

	set_cable_status = attached ? CABLE_TYPE_TA : CABLE_TYPE_UNKNOWN;
	if (charger_callbacks && charger_callbacks->set_cable)
		charger_callbacks->set_cable(charger_callbacks,
		set_cable_status);
}

static void jena_jig_cb(u8 attached, struct fsausb_ops *ops)
{
	pr_info("[BATT] Board file [FSA880]: Jig Callback\n");

	set_acc_status = attached ? ACC_TYPE_JIG : ACC_TYPE_NONE;
	if (charger_callbacks && charger_callbacks->set_acc_type)
		charger_callbacks->set_acc_type(charger_callbacks,
		set_acc_status);
}

static void jena_ovp_cb(u8 attached, struct fsausb_ops *ops)
{
	pr_info("[BATT] Board file [FSA880]: OVP Callback\n");

	set_ovp_status = attached ? OVP_TYPE_OVP : OVP_TYPE_NONE;
	if (charger_callbacks && charger_callbacks->set_ovp_type)
		charger_callbacks->set_ovp_type(charger_callbacks,
		set_ovp_status);
}

/* check charger cable type for USB phy off */
static int checkChargerType()
{
	return set_cable_status;
}

static void jena_fsa880_reset_cb(void)
{
	pr_info(" [BATT] Board file [FSA880]: Reset Callback\n");
}

/* For uUSB Switch */
static struct fsausb_platform_data jena_fsa880_pdata = {
       .intb_gpio      = MSM_GPIO_TO_INT(GPIO_MUSB_INT),
       .usb_cb         = jena_usb_cb,
       .uart_cb        = NULL,
       .charger_cb     = jena_charger_cb,
       .jig_cb         = jena_jig_cb,
	.ovp_cb		= jena_ovp_cb,
       .reset_cb       = jena_fsa880_reset_cb,
};

/* I2C 3 */
static struct i2c_gpio_platform_data fsa880_i2c_gpio_data = {
	.sda_pin    = GPIO_MUS_SDA,
	.scl_pin    = GPIO_MUS_SCL,
};

static struct platform_device fsa880_i2c_gpio_device = {
	.name       = "i2c-gpio",
	.id     =  3,
	.dev        = {
		.platform_data  = &fsa880_i2c_gpio_data,
	},
};

static struct i2c_board_info fsa880_i2c_devices[] = {
	{
		I2C_BOARD_INFO("FSA9280", 0x4A >> 1),
		.platform_data =  &jena_fsa880_pdata,
		.irq = MSM_GPIO_TO_INT(GPIO_MUSB_INT),
	},
};

#ifdef CONFIG_BQ27425_FUEL_GAUGE
/* Fuel_gauge */
static struct i2c_gpio_platform_data fuelgauge_i2c_gpio_data = {
	.sda_pin = FUEL_I2C_SDA,
	.scl_pin = FUEL_I2C_SCL,
};

static struct platform_device fuelgauge_i2c_gpio_device = {
	.name	= "i2c-gpio",
	.id	= 6,
	.dev	= {
	.platform_data	= &fuelgauge_i2c_gpio_data,
	},
};
static struct i2c_board_info fg_i2c_devices[] = {
	{
		I2C_BOARD_INFO( "bq27425", 0xAA>>1 ),
	},
};
#endif

#if defined(CONFIG_I2C) && defined(CONFIG_GPIO_SX150X)
static void __init register_i2c_devices(void)
{

	i2c_register_board_info(MSM_GSBI0_QUP_I2C_BUS_ID,
				cam_exp_i2c_info,
				ARRAY_SIZE(cam_exp_i2c_info));

	if (machine_is_msm7x27a_surf())
		sx150x_data[SX150X_CORE].io_open_drain_ena = 0xe0f0;

	core_exp_i2c_info[0].platform_data =
			&sx150x_data[SX150X_CORE];

	i2c_register_board_info(MSM_GSBI1_QUP_I2C_BUS_ID,
				core_exp_i2c_info,
				ARRAY_SIZE(core_exp_i2c_info));
#if defined(CONFIG_BT) && defined(CONFIG_MARIMBA_CORE)
	i2c_register_board_info(MSM_GSBI1_QUP_I2C_BUS_ID,
				bahama_devices,
				ARRAY_SIZE(bahama_devices));
#if (CONFIG_MACH_TREBON_HWREV == 0x0)
	#if defined(CONFIG_SENSORS_HSCD) || defined(CONFIG_PROXIMITY_SENSOR)
	i2c_register_board_info(4, sensor_devices, ARRAY_SIZE(sensor_devices));
	#endif
#endif
#endif
}
#endif

static struct msm_gpio qup_i2c_gpios_io[] = {
	{ GPIO_CFG(60, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
		"qup_scl" },
	{ GPIO_CFG(61, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
		"qup_sda" },
	{ GPIO_CFG(131, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
		"qup_scl" },
	{ GPIO_CFG(132, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
		"qup_sda" },
};

static struct msm_gpio qup_i2c_gpios_hw[] = {
	{ GPIO_CFG(60, 1, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
		"qup_scl" },
	{ GPIO_CFG(61, 1, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
		"qup_sda" },
	{ GPIO_CFG(131, 2, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		"qup_scl" },
	{ GPIO_CFG(132, 2, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		"qup_sda" },
};

static void gsbi_qup_i2c_gpio_config(int adap_id, int config_type)
{
	int rc;

	if (adap_id < 0 || adap_id > 1)
		return;

	/* Each adapter gets 2 lines from the table */
	if (config_type)
		rc = msm_gpios_request_enable(&qup_i2c_gpios_hw[adap_id*2], 2);
	else
		rc = msm_gpios_request_enable(&qup_i2c_gpios_io[adap_id*2], 2);
	if (rc < 0)
		pr_err("QUP GPIO request/enable failed: %d\n", rc);
}

static struct msm_i2c_platform_data msm_gsbi0_qup_i2c_pdata = {
	.clk_freq		= 400000,
	.msm_i2c_config_gpio	= gsbi_qup_i2c_gpio_config,
};

static struct msm_i2c_platform_data msm_gsbi1_qup_i2c_pdata = {
	.clk_freq		= 100000,
	.msm_i2c_config_gpio	= gsbi_qup_i2c_gpio_config,
};

#ifdef CONFIG_ARCH_MSM7X27A
#define MSM_PMEM_MDP_SIZE       0x1DD1000
#define MSM_PMEM_ADSP_SIZE      0x1000000
#ifdef CONFIG_FB_MSM_TRIPLE_BUFFER
/* prim = 320 x 480 x 4(bpp) x 3(pages) */
#define MSM_FB_SIZE             320 * 480 * 4 * 3
#else
/* prim = 320 x 480 x 4(bpp) x 2(pages) */
#define MSM_FB_SIZE             320 * 480 * 4 * 2
#endif /* CONFIG_FB_MSM_TRIPLE_BUFFER */
#else
#define MSM_PMEM_MDP_SIZE       0x1DD1000
#define MSM_PMEM_ADSP_SIZE      0x1000000
#define MSM_FB_SIZE             0x195000
#endif /* CONFIG_ARCH_MSM7X27A */

static struct android_usb_platform_data android_usb_pdata = {
	.update_pid_and_serial_num = usb_diag_update_pid_and_serial_num,
};

static struct platform_device android_usb_device = {
	.name	= "android_usb",
	.id	= -1,
	.dev	= {
		.platform_data = &android_usb_pdata,
	},
};

static int __init boot_mode_boot(char *onoff)
{
	if (strncmp(onoff, "true", 4) == 0) {
		charging_boot = 1;
		fota_boot = 0;
		pr_info("%s[BATT] charging_boot: %d\n",
			__func__, charging_boot);
	} else if (strncmp(onoff, "fota", 5) == 0) {
		fota_boot = 1;
		charging_boot = 0;
	} else {
		charging_boot = 0;
		fota_boot = 0;
	}
	return 1;
}
__setup("androidboot.battchg_pause", boot_mode_boot);


#ifdef CONFIG_USB_EHCI_MSM_72K
static void msm_hsusb_vbus_power(unsigned phy_info, int on)
{
	int rc = 0;
	unsigned gpio;

	gpio = GPIO_HOST_VBUS_EN;

	rc = gpio_request(gpio, "i2c_host_vbus_en");
	if (rc < 0) {
		pr_err("failed to request %d GPIO\n", gpio);
		return;
	}
	gpio_direction_output(gpio, !!on);
	gpio_set_value_cansleep(gpio, !!on);
	gpio_free(gpio);
}

static struct msm_usb_host_platform_data msm_usb_host_pdata = {
	.phy_info       = (USB_PHY_INTEGRATED | USB_PHY_MODEL_45NM),
};

static void __init msm7x2x_init_host(void)
{
	msm_add_host(0, &msm_usb_host_pdata);
}
#endif

#ifdef CONFIG_USB_MSM_OTG_72K
static int hsusb_rpc_connect(int connect)
{
	if (connect)
		return msm_hsusb_rpc_connect();
	else
		return msm_hsusb_rpc_close();
}

static struct regulator *reg_hsusb;
static int msm_hsusb_ldo_init(int init)
{
	int rc = 0;

	if (init) {
		reg_hsusb = regulator_get(NULL, "usb");
		if (IS_ERR(reg_hsusb)) {
			rc = PTR_ERR(reg_hsusb);
			pr_err("%s: sandeep could not get regulator: %d\n",
					__func__, rc);
			goto out;
		}

		rc = regulator_set_voltage(reg_hsusb, 3300000, 3300000);
		if (rc) {
			pr_err("%s:sandeep could not set voltage: %d\n",
					__func__, rc);
			goto reg_free;
		}

		return 0;
	}
	/* else fall through */
reg_free:
	regulator_put(reg_hsusb);
out:
	reg_hsusb = NULL;
	return rc;
}

static int msm_hsusb_ldo_enable(int enable)
{
	static int ldo_status;

	if (IS_ERR_OR_NULL(reg_hsusb))
		return reg_hsusb ? PTR_ERR(reg_hsusb) : -ENODEV;

	if (ldo_status == enable)
		return 0;

	ldo_status = enable;

	return enable ?
		regulator_enable(reg_hsusb) :
		regulator_disable(reg_hsusb);
}

#ifndef CONFIG_USB_EHCI_MSM_72K
static int msm_hsusb_pmic_notif_init(void (*callback)(int online), int init)
{
	int ret = 0;

	if (init)
		ret = msm_pm_app_rpc_init(callback);
	else
		msm_pm_app_rpc_deinit(callback);

	return ret;
}
#endif

static struct msm_otg_platform_data msm_otg_pdata = {
#ifndef CONFIG_USB_EHCI_MSM_72K
	.pmic_vbus_notif_init	 = msm_hsusb_pmic_notif_init,
#else
	.vbus_power		 = msm_hsusb_vbus_power,
#endif
	.rpc_connect		 = hsusb_rpc_connect,
	.pemp_level		 = PRE_EMPHASIS_WITH_20_PERCENT,
	.cdr_autoreset		 = CDR_AUTO_RESET_DISABLE,
	.drv_ampl		 = HS_DRV_AMPLITUDE_75_PERCENT,
	.se1_gating		 = SE1_GATING_DISABLE,
	.ldo_init		 = msm_hsusb_ldo_init,
	.ldo_enable		 = msm_hsusb_ldo_enable,
	.chg_init		 = hsusb_chg_init,
	/* check charger cable type for USB phy off */
//	.chg_connect_type = checkChargerType,
	/* XXX: block charger current setting */
	.chg_connected		 = hsusb_chg_connected,
	.chg_vbus_draw		 = hsusb_chg_vbus_draw,
};
#endif

static struct msm_hsusb_gadget_platform_data msm_gadget_pdata = {
	.is_phy_status_timer_on = 1,
};

static struct resource smc91x_resources[] = {
	[0] = {
		.start = 0x90000300,
		.end   = 0x900003ff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MSM_GPIO_TO_INT(4),
		.end   = MSM_GPIO_TO_INT(4),
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device smc91x_device = {
	.name           = "smc91x",
	.id             = 0,
	.num_resources  = ARRAY_SIZE(smc91x_resources),
	.resource       = smc91x_resources,
};

#define WLAN_HOST_WAKE


#ifdef WLAN_HOST_WAKE
struct wlansleep_info {
	unsigned host_wake;
	unsigned host_wake_irq;
	struct wake_lock wake_lock;
};


static struct wlansleep_info *wsi;
static struct tasklet_struct hostwake_task;


static void wlan_hostwake_task(unsigned long data)
{
	printk(KERN_INFO "WLAN: wake lock timeout 0.5 sec...\n");
	wake_lock_timeout(&wsi->wake_lock, HZ / 2);
}


static irqreturn_t wlan_hostwake_isr(int irq, void *dev_id)
{
	/* schedule a tasklet to handle the change in the host wake line */
	tasklet_schedule(&hostwake_task);
	return IRQ_HANDLED;
}


static int wlan_host_wake_init(void)
{
	int ret;

	wsi = kzalloc(sizeof(struct wlansleep_info), GFP_KERNEL);
	if (!wsi)
		return -ENOMEM;

	wake_lock_init(&wsi->wake_lock, WAKE_LOCK_SUSPEND, "bluesleep");
	tasklet_init(&hostwake_task, wlan_hostwake_task, 0);

	wsi->host_wake = GPIO_WLAN_HOST_WAKE;
	wsi->host_wake_irq = MSM_GPIO_TO_INT(wsi->host_wake);

	ret = request_irq(wsi->host_wake_irq, wlan_hostwake_isr,
						IRQF_DISABLED | IRQF_TRIGGER_RISING,
						"wlan hostwake", NULL);
	if (ret < 0) {
		printk(KERN_ERR "WLAN: Couldn't acquire WLAN_HOST_WAKE IRQ");
		return -1;
	}

	ret = enable_irq_wake(wsi->host_wake_irq);
	if (ret < 0) {
		printk(KERN_ERR "WLAN: Couldn't enable WLAN_HOST_WAKE as wakeup interrupt");
		free_irq(wsi->host_wake_irq, NULL);
		return -1;
	}

	return 0;
}


static void wlan_host_wake_exit(void)
{
	if (disable_irq_wake(wsi->host_wake_irq))
		printk(KERN_ERR "WLAN: Couldn't disable hostwake IRQ wakeup mode \n");

	free_irq(wsi->host_wake_irq, NULL);

	wake_lock_destroy(&wsi->wake_lock);
	kfree(wsi);
}
#endif /* WLAN_HOST_WAKE */


static int wlan_set_gpio(unsigned gpio, int on)
{
	int rc = 0;
	int gpio_value = 0;

	printk("%s - %d : %s\n", __func__, gpio, on ? "on" : "off");

	// Request
	if (gpio_request(gpio, "wlan_ar6000_pm")) {
		printk(KERN_ERR "%s: gpio_request for %d failed\n",
				__func__, gpio);
		return -1;
	}

	gpio_value = gpio_get_value(gpio);
	printk(KERN_INFO "%s: before (%d) :: gpio_get_value = %d",
			__func__, on, gpio_value);

	// Direction Output On/Off
	rc = gpio_direction_output(gpio, on);
	gpio_free(gpio);

	gpio_value = gpio_get_value(gpio);
	printk(KERN_INFO "%s: after (%d) :: gpio_get_value = %d",
			__func__, on, gpio_value);

	if (rc) {
		printk(KERN_ERR "%s: gpio_direction_output for %d failed\n",
				__func__, gpio);
		return -1;
	}

	return 0;
}


#ifdef WLAN_33V_CONTROL_FOR_BT_ANTENNA
int wlan_setup_ldo_33v(int input_flag, int on)
{
	int skip = 0;
	int temp_flag = wlan_33v_flag;

	printk(KERN_INFO "%s - set by %s : %s\n",
			__func__,
			(input_flag == WLAN_33V_WIFI_FLAG) ? "Wifi" : "BT",
			on ? "on" : "off");
	printk(KERN_INFO "%s - old wlan_33v_flag : %d\n",
			__func__, temp_flag);

	if (on) {
		if (temp_flag)  /* Already On */
			skip = 1;

		temp_flag |= input_flag;
	} else {
		temp_flag &= (~input_flag);

		/* Keep GPIO_WLAN_33V_EN on if either BT or Wifi is turned on*/
		if (temp_flag)
			skip = 1;
	}

	printk(KERN_INFO "%s - new wlan_33v_flag : %d\n",
			__func__, temp_flag);

	if (skip) {
		printk(KERN_INFO "%s - Skip GPIO_WLAN_33V_EN %s\n",
				__func__, on ? "on" : "off");
	} else {
		/* GPIO_WLAN_33V_EN - On / Off */
		if (wlan_set_gpio(GPIO_WLAN_33V_EN, on))
			return WLAN_ERROR;
	}

	wlan_33v_flag = temp_flag;

	return WLAN_OK;
}
#endif

void wlan_setup_power(int on, int detect)
{
	printk("%s %s --enter\n", __func__, on ? "on" : "down");

	if (on) {
#ifdef WLAN_33V_CONTROL_FOR_BT_ANTENNA
		/* GPIO_WLAN_33V_EN - On */
		if (wlan_setup_ldo_33v(WLAN_33V_WIFI_FLAG, 1))
			return;
#endif
		udelay(120);

		// GPIO_WLAN_RESET_N - On
		if (wlan_set_gpio(GPIO_WLAN_RESET_N, 1))
			return;

#ifdef WLAN_HOST_WAKE
		wlan_host_wake_init();
#endif /* WLAN_HOST_WAKE */
	}
	else {
#ifdef WLAN_HOST_WAKE
		wlan_host_wake_exit();
#endif /* WLAN_HOST_WAKE */

		// GPIO_WLAN_RESET_N - Off
		if (wlan_set_gpio(GPIO_WLAN_RESET_N, 0))
			return;

		udelay(100);

#ifdef WLAN_33V_CONTROL_FOR_BT_ANTENNA
		/* GPIO_WLAN_33V_EN - Off */
		if (wlan_setup_ldo_33v(WLAN_33V_WIFI_FLAG, 0))
			return;
#endif
	}

#ifndef ATH_POLLING
	mdelay(100);

	if (detect) {
		/* Detect card */
		if (wlan_status_notify_cb)
			wlan_status_notify_cb(on, wlan_devid);
		else
			printk(KERN_ERR "WLAN: No notify available\n");
	}
#endif /* ATH_POLLING */
}
EXPORT_SYMBOL(wlan_setup_power);
EXPORT_SYMBOL(board_hw_revision);


static int wlan_power_init(void)
{
#ifdef WLAN_33V_CONTROL_FOR_BT_ANTENNA
	wlan_33v_flag = 0;
#endif

	/* Set config - GPIO_WLAN_33V_EN */
	if (gpio_tlmm_config(GPIO_CFG(GPIO_WLAN_33V_EN, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP,
			GPIO_CFG_2MA), GPIO_CFG_ENABLE)) {
		printk(KERN_ERR "%s: gpio_tlmm_config for %d failed\n",
				__func__, GPIO_WLAN_33V_EN);
		return WLAN_ERROR;
	}

	return WLAN_OK;
}

#if (defined(CONFIG_MMC_MSM_SDC1_SUPPORT)\
	|| defined(CONFIG_MMC_MSM_SDC2_SUPPORT)\
	|| defined(CONFIG_MMC_MSM_SDC3_SUPPORT)\
	|| defined(CONFIG_MMC_MSM_SDC4_SUPPORT))

static unsigned long vreg_sts, gpio_sts;
static struct vreg *vreg_mmc;
static struct vreg *vreg_emmc;

struct sdcc_vreg {
	struct vreg *vreg_data;
	unsigned level;
};

static struct sdcc_vreg sdcc_vreg_data[4];

struct sdcc_gpio {
	struct msm_gpio *cfg_data;
	uint32_t size;
	struct msm_gpio *sleep_cfg_data;
};

/**
 * Due to insufficient drive strengths for SDC GPIO lines some old versioned
 * SD/MMC cards may cause data CRC errors. Hence, set optimal values
 * for SDC slots based on timing closure and marginality. SDC1 slot
 * require higher value since it should handle bad signal quality due
 * to size of T-flash adapters.
 */
static struct msm_gpio sdc1_cfg_data[] = {
	{GPIO_CFG(51, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_14MA),
								"sdc1_dat_3"},
	{GPIO_CFG(52, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_14MA),
								"sdc1_dat_2"},
	{GPIO_CFG(53, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_14MA),
								"sdc1_dat_1"},
	{GPIO_CFG(54, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_14MA),
								"sdc1_dat_0"},
	{GPIO_CFG(55, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_14MA),
								"sdc1_cmd"},
	{GPIO_CFG(56, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_14MA),
								"sdc1_clk"},
};

static struct msm_gpio sdc2_cfg_data[] = {
	{GPIO_CFG(62, 2, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc2_clk"},
	{GPIO_CFG(63, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc2_cmd"},
	{GPIO_CFG(64, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc2_dat_3"},
	{GPIO_CFG(65, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc2_dat_2"},
	{GPIO_CFG(66, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc2_dat_1"},
	{GPIO_CFG(67, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc2_dat_0"},
};

static struct msm_gpio sdc2_sleep_cfg_data[] = {
	{GPIO_CFG(62, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN , GPIO_CFG_2MA),
								"sdc2_clk"},
	{GPIO_CFG(63, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc2_cmd"},
	{GPIO_CFG(64, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc2_dat_3"},
	{GPIO_CFG(65, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc2_dat_2"},
	{GPIO_CFG(66, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc2_dat_1"},
	{GPIO_CFG(67, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),
								"sdc2_dat_0"},
};
static struct msm_gpio sdc3_cfg_data[] = {
	{GPIO_CFG(88, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc3_clk"},
	{GPIO_CFG(89, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc3_cmd"},
	{GPIO_CFG(90, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc3_dat_3"},
	{GPIO_CFG(91, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc3_dat_2"},
	{GPIO_CFG(92, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc3_dat_1"},
	{GPIO_CFG(93, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc3_dat_0"},
#ifdef CONFIG_MMC_MSM_SDC3_8_BIT_SUPPORT
	{GPIO_CFG(19, 3, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc3_dat_7"},
	{GPIO_CFG(20, 3, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc3_dat_6"},
	{GPIO_CFG(21, 3, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc3_dat_5"},
	{GPIO_CFG(108, 3, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc3_dat_4"},
#endif
};

static struct msm_gpio sdc4_cfg_data[] = {
	{GPIO_CFG(19, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc4_dat_3"},
	{GPIO_CFG(20, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc4_dat_2"},
	{GPIO_CFG(21, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc4_dat_1"},
	{GPIO_CFG(106, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc4_cmd"},
	{GPIO_CFG(108, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_10MA),
								"sdc4_dat_0"},
	{GPIO_CFG(109, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
								"sdc4_clk"},
};

static struct sdcc_gpio sdcc_cfg_data[] = {
	{
		.cfg_data = sdc1_cfg_data,
		.size = ARRAY_SIZE(sdc1_cfg_data),
	},
	{
		.cfg_data = sdc2_cfg_data,
		.size = ARRAY_SIZE(sdc2_cfg_data),
		.sleep_cfg_data = sdc2_sleep_cfg_data,
	},
	{
		.cfg_data = sdc3_cfg_data,
		.size = ARRAY_SIZE(sdc3_cfg_data),
	},
	{
		.cfg_data = sdc4_cfg_data,
		.size = ARRAY_SIZE(sdc4_cfg_data),
	},
};

static int msm_sdcc_setup_gpio(int dev_id, unsigned int enable)
{
	int rc = 0;
	struct sdcc_gpio *curr;

	curr = &sdcc_cfg_data[dev_id - 1];
	if (!(test_bit(dev_id, &gpio_sts)^enable))
		return rc;

	if (enable) {
		set_bit(dev_id, &gpio_sts);
		rc = msm_gpios_request_enable(curr->cfg_data, curr->size);
		if (rc)
			pr_err("%s: Failed to turn on GPIOs for slot %d\n",
					__func__,  dev_id);
	} else {
		clear_bit(dev_id, &gpio_sts);
		if (curr->sleep_cfg_data) {
			rc = msm_gpios_enable(curr->sleep_cfg_data, curr->size);
			msm_gpios_free(curr->sleep_cfg_data, curr->size);
			return rc;
		}
		msm_gpios_disable_free(curr->cfg_data, curr->size);
	}
	return rc;
}

static int msm_sdcc_setup_vreg(int dev_id, unsigned int enable)
{
	int rc = 0;
	struct sdcc_vreg *curr;

	curr = &sdcc_vreg_data[dev_id - 1];
	printk("%s : %d : %d : level : %d\n", __func__, dev_id, enable, curr->level);

	if (!(test_bit(dev_id, &vreg_sts)^enable))
		return rc;

	if (enable) {
		set_bit(dev_id, &vreg_sts);
		rc = vreg_set_level(curr->vreg_data, curr->level);
		if (rc)
			pr_err("%s: vreg_set_level() = %d\n", __func__, rc);

		rc = vreg_enable(curr->vreg_data);
		if (rc)
			pr_err("%s: vreg_enable() = %d\n", __func__, rc);
	} else {
		clear_bit(dev_id, &vreg_sts);
		rc = vreg_disable(curr->vreg_data);
		if (rc)
			pr_err("%s: vreg_disable() = %d\n", __func__, rc);
	}
	return rc;
}

static uint32_t msm_sdcc_setup_power(struct device *dv, unsigned int vdd)
{
	int rc = 0;
	struct platform_device *pdev;

	pdev = container_of(dv, struct platform_device, dev);

	rc = msm_sdcc_setup_gpio(pdev->id, !!vdd);
	if (rc)
		goto out;

	rc = msm_sdcc_setup_vreg(pdev->id, !!vdd);
out:
	return rc;
}

#define GPIO_SDC1_HW_DET 94

#if defined(CONFIG_MMC_MSM_SDC1_SUPPORT) \
	&& defined(CONFIG_MMC_MSM_CARD_HW_DETECTION)
static unsigned int msm7x2xa_sdcc_slot_status(struct device *dev)
{
	int status;

	printk("%s entered\n", __func__);

	status = gpio_tlmm_config(GPIO_CFG(GPIO_SDC1_HW_DET, 2, GPIO_CFG_INPUT,
			GPIO_CFG_PULL_UP, GPIO_CFG_8MA), GPIO_CFG_ENABLE);
	if (status)
		pr_err("%s:Failed to configure tlmm for GPIO %d\n", __func__,
				GPIO_SDC1_HW_DET);

	status = gpio_request(GPIO_SDC1_HW_DET, "SD_HW_Detect");
	if (status) {
		pr_err("%s:Failed to request GPIO %d\n", __func__,
				GPIO_SDC1_HW_DET);
	} else {
		status = gpio_direction_input(GPIO_SDC1_HW_DET);
		if (!status)
			status = gpio_get_value(GPIO_SDC1_HW_DET);
		gpio_free(GPIO_SDC1_HW_DET);
	}

	status = status?0:1 ; //PMMC
	printk("<=PMMC=> %s : status : %d \n", __func__, status);
	return status;
}
#endif

#ifdef CONFIG_MMC_MSM_SDC1_SUPPORT
static struct mmc_platform_data sdc1_plat_data = {
	.ocr_mask	= MMC_VDD_28_29,
	.translate_vdd  = msm_sdcc_setup_power,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
#ifdef CONFIG_MMC_MSM_CARD_HW_DETECTION
	.status      = msm7x2xa_sdcc_slot_status,
	.status_irq  = MSM_GPIO_TO_INT(GPIO_SDC1_HW_DET),
	.irq_flags   = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
#endif
};
#endif

#ifdef CONFIG_MMC_MSM_SDC2_SUPPORT
static struct mmc_platform_data sdc2_plat_data = {
	/*
	 * SDC2 supports only 1.8V, claim for 2.85V range is just
	 * for allowing buggy cards who advertise 2.8V even though
	 * they can operate at 1.8V supply.
	 */
	.ocr_mask	= MMC_VDD_28_29 | MMC_VDD_165_195,
	.translate_vdd  = msm_sdcc_setup_power,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
#if 0  /* def CONFIG_MMC_MSM_SDIO_SUPPORT */
	.sdiowakeup_irq = MSM_GPIO_TO_INT(66),
#endif
#ifndef ATH_POLLING
	.status = wlan_status,
	.register_status_notify = register_wlan_status_notify,
#endif /* ATH_POLLING */
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000, //24576000, ///*144000,//*/
#ifdef CONFIG_MMC_MSM_SDC2_DUMMY52_REQUIRED
	.dummy52_required = 1,
#endif
};
#endif

#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
static struct mmc_platform_data sdc3_plat_data = {
	.ocr_mask	= MMC_VDD_28_29,
	.translate_vdd  = msm_sdcc_setup_power,
#ifdef CONFIG_MMC_MSM_SDC3_8_BIT_SUPPORT
	.mmc_bus_width  = MMC_CAP_8_BIT_DATA,
#else
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
#endif
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
	.nonremovable	= 1,
};
#endif

#if (defined(CONFIG_MMC_MSM_SDC4_SUPPORT)\
		&& !defined(CONFIG_MMC_MSM_SDC3_8_BIT_SUPPORT))
static struct mmc_platform_data sdc4_plat_data = {
	.ocr_mask	= MMC_VDD_28_29,
	.translate_vdd  = msm_sdcc_setup_power,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
};
#endif
#endif

#ifdef CONFIG_SERIAL_MSM_HS
static struct msm_serial_hs_platform_data msm_uart_dm1_pdata = {
	.inject_rx_on_wakeup	= 1,
	.rx_to_inject		= 0xFD,
};
#endif
static struct msm_pm_platform_data msm7x27a_pm_data[MSM_PM_SLEEP_MODE_NR] = {
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] = {
					.idle_supported = 1,
					.suspend_supported = 1,
					.idle_enabled = 1,
					.suspend_enabled = 1,
					.latency = 16000,
					.residency = 20000,
	},
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN] = {
					.idle_supported = 1,
					.suspend_supported = 1,
					.idle_enabled = 1,
					.suspend_enabled = 1,
					.latency = 12000,
					.residency = 20000,
	},
	[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT] = {
					.idle_supported = 1,
					.suspend_supported = 1,
					.idle_enabled = 0,
					.suspend_enabled = 1,
					.latency = 2000,
					.residency = 0,
	},
	[MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT] = {
					.idle_supported = 1,
					.suspend_supported = 1,
					.idle_enabled = 1,
					.suspend_enabled = 1,
					.latency = 2,
					.residency = 0,
	},
};

u32 msm7627a_power_collapse_latency(enum msm_pm_sleep_mode mode)
{
	switch (mode) {
	case MSM_PM_SLEEP_MODE_POWER_COLLAPSE:
		return msm7x27a_pm_data
		[MSM_PM_SLEEP_MODE_POWER_COLLAPSE].latency;
	case MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN:
		return msm7x27a_pm_data
		[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN].latency;
	case MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT:
		return msm7x27a_pm_data
		[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].latency;
	case MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT:
		return msm7x27a_pm_data
		[MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT].latency;
	default:
		return 0;
	}
}

static struct msm_pm_boot_platform_data msm_pm_boot_pdata __initdata = {
	.mode = MSM_PM_BOOT_CONFIG_RESET_VECTOR_PHYS,
	.p_addr = 0,
};

static struct android_pmem_platform_data android_pmem_adsp_pdata = {
	.name = "pmem_adsp",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 1,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device android_pmem_adsp_device = {
	.name = "android_pmem",
	.id = 1,
	.dev = { .platform_data = &android_pmem_adsp_pdata },
};

static unsigned pmem_mdp_size = MSM_PMEM_MDP_SIZE;
static int __init pmem_mdp_size_setup(char *p)
{
	pmem_mdp_size = memparse(p, NULL);
	return 0;
}

early_param("pmem_mdp_size", pmem_mdp_size_setup);

static unsigned pmem_adsp_size = MSM_PMEM_ADSP_SIZE;
static int __init pmem_adsp_size_setup(char *p)
{
	pmem_adsp_size = memparse(p, NULL);
	return 0;
}

early_param("pmem_adsp_size", pmem_adsp_size_setup);

static unsigned fb_size = MSM_FB_SIZE;
static int __init fb_size_setup(char *p)
{
	fb_size = memparse(p, NULL);
	return 0;
}

early_param("fb_size", fb_size_setup);

static const char * const msm_fb_lcdc_vreg[] = {
		"gp2",
		"msme1",
};

static const int msm_fb_lcdc_vreg_mV[] = {
	2850,
	1800,
};

#define LCDC_CONFIG_PROC          21
#define LCDC_UN_CONFIG_PROC       22
#define LCDC_API_PROG             0x30000066
#define LCDC_API_VERS             0x00010001

#define	GPIO_SPI_CLK		30
#define	GPIO_SPI_CS		26
#define	GPIO_SPI_SDI		57
#define	GPIO_LCD_RESET_N	22
#define	GPIO_LCD_DETECT		38

struct vreg *lcdc_vreg[ARRAY_SIZE(msm_fb_lcdc_vreg)];

static int lcdc_gpio_num[] = {
	GPIO_SPI_CLK,
	GPIO_SPI_CS,
	GPIO_SPI_SDI,
	GPIO_LCD_RESET_N,
	GPIO_LCD_DETECT,
};

static void lcdc_trebon_gpio_init(void)
{
	int rc;

	if (gpio_request(GPIO_SPI_CLK, "spi_clk")) {
		pr_err("failed to request gpio spi_clk\n");
	}
	if (gpio_request(GPIO_SPI_CS, "spi_cs")) {
		pr_err("failed to request gpio spi_cs\n");
	}
	if (gpio_request(GPIO_SPI_SDI, "spi_mosi")) {
		pr_err("failed to request gpio spi_sdi\n");
	}
	if (gpio_request(GPIO_LCD_RESET_N, "gpio_lcd_reset_n")) {
		pr_err("failed to request gpio lcd_reset_n\n");
	}

	/* LCD Detect Irq */
	rc = gpio_tlmm_config(GPIO_CFG(GPIO_LCD_DETECT, 0,
				GPIO_CFG_INPUT, GPIO_CFG_NO_PULL,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc) {
		pr_err("%s: gpio_tlmm_config for %d failed\n",
			__func__, GPIO_LCD_DETECT);
		//goto err;
	}

	rc = gpio_request(GPIO_LCD_DETECT, "gpio_lcd_detect");
	if (rc) {
		pr_err("%s: unable to request gpio %d\n",
			__func__, GPIO_LCD_DETECT);
		//goto err;
	}

	rc = gpio_direction_input(GPIO_LCD_DETECT);
	if (rc < 0) {
		pr_err("%s: unable to set the direction of gpio %d\n",
			__func__, GPIO_LCD_DETECT);
		//goto err;
	}

	return;
}

static uint32_t lcdc_gpio_table[] = {
	GPIO_CFG(GPIO_SPI_CLK, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_SPI_CS, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_SPI_SDI, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_LCD_RESET_N,  0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};

static void config_lcdc_gpio_table(uint32_t *table, int len, unsigned enable)
{
	int n, rc;

	for (n = 0; n < len; n++) {
		rc = gpio_tlmm_config(table[n],
			enable ? GPIO_CFG_ENABLE : GPIO_CFG_DISABLE);
		if (rc) {
			printk(KERN_ERR "%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, table[n], rc);
			break;
		}
	}
}
static int msm_fb_lcdc_power_save(int on)
{
    return 0;
}

static struct lcdc_platform_data lcdc_pdata = {
	.lcdc_gpio_config = NULL,
	.lcdc_power_save   = msm_fb_lcdc_power_save,
};

static struct resource lcdc_trebon_resources[] = {
	{
		.name = "lcd_breakdown_det",
		.start = MSM_GPIO_TO_INT(GPIO_LCD_DETECT),
		.end = MSM_GPIO_TO_INT(GPIO_LCD_DETECT),
		.flags  = IORESOURCE_IRQ,
	}
};

static void lcdc_trebon_config_gpios(int enable)
{
	config_lcdc_gpio_table(lcdc_gpio_table,
		ARRAY_SIZE(lcdc_gpio_table), enable);
}

static struct msm_panel_common_pdata lcdc_trebon_panel_data = {
	.panel_config_gpio = lcdc_trebon_config_gpios,
	.gpio_num	  = lcdc_gpio_num,

};

static struct platform_device lcdc_trebon_panel_device = {
#if defined(CONFIG_FB_MSM_LCDC_TREBON_HVGA)
	.name   = "lcdc_trebon_hvga",
	.num_resources  = ARRAY_SIZE(lcdc_trebon_resources),
	.resource       = lcdc_trebon_resources,
#elif defined(CONFIG_FB_MSM_LCDC_JENA_HVGA)
	.name   = "lcdc_trebon_hvga",
	.num_resources  = ARRAY_SIZE(lcdc_trebon_resources),
	.resource       = lcdc_trebon_resources,
#else
	.name   = "lcdc_s6d16a0x_hvga",
#endif
	.id     = 0,
	.dev    = {
		.platform_data = &lcdc_trebon_panel_data,
	}
};

#ifdef CONFIG_TOUCHSCREEN_ZINITIX_A
static void tsp_power_on(void)
{
	int rc = 0;
	printk("[TSP] %s start \n", __func__);

#if (CONFIG_MACH_TREBON_HWREV == 0x0)
	rc = gpio_request(41, "touch_en");
#else
	rc = gpio_request(78, "touch_en");

#endif
	if (rc < 0) {
		pr_err("failed to request touch_en\n");
	}

#if (CONFIG_MACH_TREBON_HWREV == 0x0)
	gpio_tlmm_config(GPIO_CFG(41, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_direction_output(41, 1);
#else
	gpio_tlmm_config(GPIO_CFG(78, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_direction_output(78, 1);
#endif
}
#endif

#ifdef CONFIG_TOUCHSCREEN_MELFAS
static void tsp_power_on(void)
{
	int rc = 0;
	printk("[TSP] %s start \n", __func__);

	rc = gpio_request(41, "touch_en");
	if (rc < 0) {
		pr_err("failed to request touch_en\n");
	}

	gpio_tlmm_config(GPIO_CFG(41, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_direction_output(41, 1);
}
#endif

struct class *sec_class;
EXPORT_SYMBOL(sec_class);

static void samsung_sys_class_init(void)
{
	pr_info("samsung sys class init.\n");

	sec_class = class_create(THIS_MODULE, "sec");
	if (IS_ERR(sec_class))
		pr_err("Failed to create class(sec) !\n");

}

static struct resource msm_fb_resources[] = {
	{
		.flags  = IORESOURCE_DMA,
	}
};

static int msm_fb_detect_panel(const char *name)
{
	int ret = -EPERM;

#if defined(CONFIG_FB_MSM_LCDC_S6D16A0X_HVGA)
		if (!strcmp(name, "lcdc_s6d16a0x_hvga"))
			ret = 0;
		else
			ret = -ENODEV;
#elif defined(CONFIG_FB_MSM_LCDC_TREBON_HVGA)
		if (!strcmp(name, "lcdc_trebon_hvga"))
			ret = 0;
		else
			ret = -ENODEV;
#elif defined(CONFIG_FB_MSM_LCDC_JENA_HVGA)
		if (!strcmp(name, "lcdc_trebon_hvga"))
			ret = 0;
		else
			ret = -ENODEV;
#endif
    return ret;
}

static struct msm_fb_platform_data msm_fb_pdata = {
	.detect_client = msm_fb_detect_panel,
};

static struct platform_device msm_fb_device = {
	.name   = "msm_fb",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_fb_resources),
	.resource       = msm_fb_resources,
	.dev    = {
		.platform_data = &msm_fb_pdata,
	}
};

#ifdef CONFIG_FB_MSM_MIPI_DSI
static int mipi_renesas_set_bl(int level)
{
	int ret;

	ret = pmapp_disp_backlight_set_brightness(level);

	if (ret)
		pr_err("%s: can't set lcd backlight!\n", __func__);

	return ret;
}

static struct msm_panel_common_pdata mipi_renesas_pdata = {
	.pmic_backlight = mipi_renesas_set_bl,
};


static struct platform_device mipi_dsi_renesas_panel_device = {
	.name = "mipi_renesas",
	.id = 0,
	.dev    = {
		.platform_data = &mipi_renesas_pdata,
	}
};
#endif


static void __init msm7x27a_init_mmc(void)
{
        vreg_emmc = vreg_get(NULL,"msme1");
        if (IS_ERR(vreg_emmc)) {
                pr_err("%s: vreg get failed (%ld)\n",
                                __func__, PTR_ERR(vreg_emmc));
                return;
        }

        vreg_mmc = vreg_get(NULL,"mmc");
        if (IS_ERR(vreg_mmc)) {
                pr_err("%s: vreg get failed (%ld)\n",
                                __func__, PTR_ERR(vreg_mmc));
                return;
        }

	/* eMMC slot */
#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
	sdcc_vreg_data[2].vreg_data = vreg_emmc;
	sdcc_vreg_data[2].level = 1800;
	msm_add_sdcc(3, &sdc3_plat_data);
#endif
	/* Micro-SD slot */
#ifdef CONFIG_MMC_MSM_SDC1_SUPPORT
	sdcc_vreg_data[0].vreg_data = vreg_mmc;
	sdcc_vreg_data[0].level = 2850;
	msm_add_sdcc(1, &sdc1_plat_data);
#endif
	/* SDIO WLAN slot */
#ifdef CONFIG_MMC_MSM_SDC2_SUPPORT
	sdcc_vreg_data[1].vreg_data = vreg_emmc;
	sdcc_vreg_data[1].level = 1800; /* 2850 */
	msm_add_sdcc(2, &sdc2_plat_data);
#endif
	/* Not Used */
#if (defined(CONFIG_MMC_MSM_SDC4_SUPPORT)\
		&& !defined(CONFIG_MMC_MSM_SDC3_8_BIT_SUPPORT))
	sdcc_vreg_data[3].vreg_data = vreg_mmc;
	sdcc_vreg_data[3].level = 2850;
	msm_add_sdcc(4, &sdc4_plat_data);
#endif
}

#define SND(desc, num) { .name = #desc, .id = num }
static struct snd_endpoint snd_endpoints_list[] = {
	SND(HANDSET, 0),
	SND(MONO_HEADSET, 2),
	SND(HEADSET, 3),
	SND(SPEAKER, 6),
	SND(TTY_HEADSET, 8),
	SND(TTY_VCO, 9),
	SND(TTY_HCO, 10),
	SND(BT, 12),
	SND(IN_S_SADC_OUT_HANDSET, 16),
	SND(VOICE_RECOGNITION, 24),
	SND(FM_DIGITAL_STEREO_HEADSET, 26),
	SND(FM_DIGITAL_SPEAKER_PHONE, 27),
	SND(FM_DIGITAL_BT_A2DP_HEADSET, 28),
	SND(FM_STEREO_HEADSET, 29),
	SND(FM_SPEAKER_PHONE, 30),
	SND(STEREO_HEADSET_AND_SPEAKER, 31),
	SND(HEADSET_AND_SPEAKER, 32),
	SND(STEREO_HEADSET_3POLE, 34),
	SND(MP3_SPEAKER_PHONE, 35),
	SND(MP3_STEREO_HEADSET, 36),
	SND(BT_NSEC_OFF, 37),
	SND(HANDSET_VOIP, 38),
	SND(STEREO_HEADSET_VOIP, 39),
	SND(SPEAKER_VOIP, 40),
	SND(BT_VOIP, 41),
	SND(HANDSET_VOIP2, 42),
	SND(STEREO_HEADSET_VOIP2, 43),
	SND(SPEAKER_VOIP2, 44),
	SND(BT_VOIP2, 45),
	SND(VOICE_RECORDER_HPH, 46),
	SND(VOICE_RECORDER_SPK, 47),
	SND(FM_ANALOG_STEREO_HEADSET, 50),
	SND(FM_ANALOG_STEREO_HEADSET_CODEC, 51),
	SND(CURRENT, 0x7FFFFFFE),
};
#undef SND

static struct msm_snd_endpoints msm_device_snd_endpoints = {
	.endpoints = snd_endpoints_list,
	.num = sizeof(snd_endpoints_list) / sizeof(struct snd_endpoint)
};

static struct platform_device msm_device_snd = {
	.name = "msm_snd",
	.id = -1,
	.dev    = {
		.platform_data = &msm_device_snd_endpoints
	},
};

#define DEC0_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#define DEC1_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#define DEC2_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#define DEC3_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#define DEC4_FORMAT (1<<MSM_ADSP_CODEC_MIDI)

static unsigned int dec_concurrency_table[] = {
	/* Audio LP */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DMA)), 0,
	0, 0, 0,

	/* Concurrency 1 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	 /* Concurrency 2 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	/* Concurrency 3 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	/* Concurrency 4 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	/* Concurrency 5 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	/* Concurrency 6 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	0, 0, 0, 0,

	/* Concurrency 7 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),
};

#define DEC_INFO(name, queueid, decid, nr_codec) { .module_name = name, \
	.module_queueid = queueid, .module_decid = decid, \
	.nr_codec_support = nr_codec}

static struct msm_adspdec_info dec_info_list[] = {
	DEC_INFO("AUDPLAY0TASK", 13, 0, 11), /* AudPlay0BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY1TASK", 14, 1, 11),  /* AudPlay1BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY2TASK", 15, 2, 11),  /* AudPlay2BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY3TASK", 16, 3, 11),  /* AudPlay3BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY4TASK", 17, 4, 1),  /* AudPlay4BitStreamCtrlQueue */
};

static struct msm_adspdec_database msm_device_adspdec_database = {
	.num_dec = ARRAY_SIZE(dec_info_list),
	.num_concurrency_support = (ARRAY_SIZE(dec_concurrency_table) / \
					ARRAY_SIZE(dec_info_list)),
	.dec_concurrency_table = dec_concurrency_table,
	.dec_info_list = dec_info_list,
};

static struct platform_device msm_device_adspdec = {
	.name = "msm_adspdec",
	.id = -1,
	.dev    = {
		.platform_data = &msm_device_adspdec_database
	},
};

static struct android_pmem_platform_data android_pmem_audio_pdata = {
	.name = "pmem_audio",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device android_pmem_audio_device = {
	.name = "android_pmem",
	.id = 2,
	.dev = { .platform_data = &android_pmem_audio_pdata },
};

static struct android_pmem_platform_data android_pmem_pdata = {
	.name = "pmem",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 1,
	.memory_type = MEMTYPE_EBI1,
};
static struct platform_device android_pmem_device = {
	.name = "android_pmem",
	.id = 0,
	.dev = { .platform_data = &android_pmem_pdata },
};

static struct smsc911x_platform_config smsc911x_config = {
	.irq_polarity	= SMSC911X_IRQ_POLARITY_ACTIVE_HIGH,
	.irq_type	= SMSC911X_IRQ_TYPE_PUSH_PULL,
	.flags		= SMSC911X_USE_16BIT,
};

static struct resource smsc911x_resources[] = {
	[0] = {
		.start	= 0x90000000,
		.end	= 0x90007fff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= MSM_GPIO_TO_INT(48),
		.end	= MSM_GPIO_TO_INT(48),
		.flags	= IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHLEVEL,
	},
};

static struct platform_device smsc911x_device = {
	.name		= "smsc911x",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(smsc911x_resources),
	.resource	= smsc911x_resources,
	.dev		= {
		.platform_data	= &smsc911x_config,
	},
};

static struct msm_gpio smsc911x_gpios[] = {
	{ GPIO_CFG(48, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_6MA),
							 "smsc911x_irq"  },
	{ GPIO_CFG(49, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_6MA),
							 "eth_fifo_sel" },
};

#define ETH_FIFO_SEL_GPIO	49
static void msm7x27a_cfg_smsc911x(void)
{
	int res;

	res = msm_gpios_request_enable(smsc911x_gpios,
				 ARRAY_SIZE(smsc911x_gpios));
	if (res) {
		pr_err("%s: unable to enable gpios for SMSC911x\n", __func__);
		return;
	}

	/* ETH_FIFO_SEL */
	res = gpio_direction_output(ETH_FIFO_SEL_GPIO, 0);
	if (res) {
		pr_err("%s: unable to get direction for gpio %d\n", __func__,
							 ETH_FIFO_SEL_GPIO);
		msm_gpios_disable_free(smsc911x_gpios,
						 ARRAY_SIZE(smsc911x_gpios));
		return;
	}
	gpio_set_value(ETH_FIFO_SEL_GPIO, 0);
}

#ifdef CONFIG_MSM_CAMERA
static uint32_t camera_off_gpio_table[] = {
	GPIO_CFG(15, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
};

static uint32_t camera_on_gpio_table[] = {

#ifdef CONFIG_MACH_JENA
	GPIO_CFG(15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
	GPIO_CFG(96, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(18, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),  //cam_stby
	GPIO_CFG(98, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//vt_cam_reset
	GPIO_CFG(85, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),  //3//3m_cam_rst
#endif

};

#ifdef CONFIG_MSM_CAMERA_FLASH
static struct msm_camera_sensor_flash_src msm_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_CURRENT_DRIVER,
};
#endif

static struct vreg *vreg_gp1;
static struct vreg *vreg_gp2;
static struct vreg *vreg_gp3;
static void msm_camera_vreg_config(int vreg_en)
{
}

static int config_gpio_table(uint32_t *table, int len)
{
	int rc = 0, i = 0;

	for (i = 0; i < len; i++) {
		rc = gpio_tlmm_config(table[i], GPIO_CFG_ENABLE);
		if (rc) {
			pr_err("%s not able to get gpio\n", __func__);
			for (i--; i >= 0; i--)
				gpio_tlmm_config(camera_off_gpio_table[i],
							GPIO_CFG_ENABLE);
			break;
		}
	}
	return rc;
}

static struct msm_camera_sensor_info msm_camera_sensor_s5k4e1_data;
static struct msm_camera_sensor_info msm_camera_sensor_ov9726_data;
static int config_camera_on_gpios_rear(void)
{
	int rc = 0;

	if (machine_is_msm7x27a_ffa())
		msm_camera_vreg_config(1);

	rc = config_gpio_table(camera_on_gpio_table,
			ARRAY_SIZE(camera_on_gpio_table));
	if (rc < 0) {
		pr_err("%s: CAMSENSOR gpio table request"
		"failed\n", __func__);
		return rc;
	}

	return rc;
}

static void config_camera_off_gpios_rear(void)
{
	if (machine_is_msm7x27a_ffa())
		msm_camera_vreg_config(0);

	config_gpio_table(camera_off_gpio_table,
			ARRAY_SIZE(camera_off_gpio_table));
}

static int config_camera_on_gpios_front(void)
{
	int rc = 0;

	if (machine_is_msm7x27a_ffa())
		msm_camera_vreg_config(1);

	rc = config_gpio_table(camera_on_gpio_table,
			ARRAY_SIZE(camera_on_gpio_table));
	if (rc < 0) {
		pr_err("%s: CAMSENSOR gpio table request"
			"failed\n", __func__);
		return rc;
	}

	return rc;
}

static void config_camera_off_gpios_front(void)
{
	if (machine_is_msm7x27a_ffa())
		msm_camera_vreg_config(0);

	config_gpio_table(camera_off_gpio_table,
			ARRAY_SIZE(camera_off_gpio_table));
}

struct msm_camera_device_platform_data msm_camera_device_data_rear = {
	.camera_gpio_on  = config_camera_on_gpios_rear,
	.camera_gpio_off = config_camera_off_gpios_rear,
	.ioext.csiphy = 0xA1000000,
	.ioext.csisz  = 0x00100000,
	.ioext.csiirq = INT_CSI_IRQ_1,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 192000000,
	.ioext.appphy = MSM_CLK_CTL_PHYS,
	.ioext.appsz  = MSM_CLK_CTL_SIZE,
};

struct msm_camera_device_platform_data msm_camera_device_data_front = {
	.camera_gpio_on  = config_camera_on_gpios_front,
	.camera_gpio_off = config_camera_off_gpios_front,
	.ioext.csiphy = 0xA0F00000,
	.ioext.csisz  = 0x00100000,
	.ioext.csiirq = INT_CSI_IRQ_0,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 192000000,
	.ioext.appphy = MSM_CLK_CTL_PHYS,
	.ioext.appsz  = MSM_CLK_CTL_SIZE,
};

#ifdef CONFIG_S5K4E1
static struct msm_camera_sensor_platform_info s5k4e1_sensor_7627a_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_s5k4e1 = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
	.flash_src              = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k4e1_data = {
	.sensor_name    = "s5k4e1",
	.sensor_reset_enable = 1,
	.sensor_reset   = GPIO_CAM_GP_CAMIF_RESET_N,
	.sensor_pwd             = 85,
	.vcm_pwd                = GPIO_CAM_GP_CAM_PWDN,
	.vcm_enable             = 1,
	.pdata                  = &msm_camera_device_data_rear,
	.flash_data             = &flash_s5k4e1,
	.sensor_platform_info   = &s5k4e1_sensor_7627a_info,
	.csi_if                 = 1
};

static struct platform_device msm_camera_sensor_s5k4e1 = {
	.name   = "msm_camera_s5k4e1",
	.dev    = {
		.platform_data = &msm_camera_sensor_s5k4e1_data,
	},
};
#endif

#ifdef CONFIG_IMX072
static struct msm_camera_sensor_platform_info imx072_sensor_7627a_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_imx072 = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
	.flash_src              = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_imx072_data = {
	.sensor_name    = "imx072",
	.sensor_reset_enable = 1,
	.sensor_reset   = 85, /* TODO 106,*/
	.sensor_pwd             = 123,
	.vcm_pwd                = GPIO_CAM_GP_CAM_PWDN,
	.vcm_enable             = 1,
	.pdata                  = &msm_camera_device_data_rear,
	.flash_data             = &flash_imx072,
	.sensor_platform_info = &imx072_sensor_7627a_info,
	.csi_if                 = 1
};

static struct platform_device msm_camera_sensor_imx072 = {
	.name   = "msm_camera_imx072",
	.dev    = {
		.platform_data = &msm_camera_sensor_imx072_data,
	},
};
#endif
#ifdef CONFIG_S5K5CCAF
static struct msm_camera_sensor_platform_info s5k5ccaf_sensor_7627a_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_s5k5ccaf = {
	.flash_type             = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k5ccaf_data = {
	.sensor_name    = "s5k5ccaf",
	.sensor_reset_enable = 0,
	.vcm_enable             = 0,
	.pdata                  = &msm_camera_device_data_rear,
	.flash_data             = &flash_s5k5ccaf,
	.sensor_platform_info   = &s5k5ccaf_sensor_7627a_info,
	.csi_if                 = 1 // 0: Parallel interface , 1: MIPI interface
};

static struct platform_device msm_camera_sensor_s5k5ccaf = {
	.name   = "msm_camera_s5k5ccaf",
	.dev    = {
		.platform_data = &msm_camera_sensor_s5k5ccaf_data,
	},
};
#endif

#ifdef CONFIG_SR300PC20
static struct msm_camera_sensor_platform_info s5k5ccaf_sensor_7627a_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_s5k5ccaf = {
	.flash_type             = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k5ccaf_data = {
	.sensor_name    = "s5k5ccaf",
	.sensor_reset_enable = 0,
	.vcm_enable             = 0,
	.pdata                  = &msm_camera_device_data_rear,
	.flash_data             = &flash_s5k5ccaf,
	.sensor_platform_info   = &s5k5ccaf_sensor_7627a_info,
	.csi_if                 = 1 // 0: Parallel interface , 1: MIPI interface
};

static struct platform_device msm_camera_sensor_s5k5ccaf = {
	.name   = "msm_camera_s5k5ccaf",
	.dev    = {
	.platform_data = &msm_camera_sensor_s5k5ccaf_data,
	},
};
#endif


#ifdef CONFIG_WEBCAM_OV9726
static struct msm_camera_sensor_platform_info ov9726_sensor_7627a_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_ov9726 = {
	.flash_type             = MSM_CAMERA_FLASH_NONE,
	.flash_src              = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_ov9726_data = {
	.sensor_name    = "ov9726",
	.sensor_reset_enable = 0,
	.sensor_reset   = GPIO_CAM_GP_CAM1MP_XCLR,
	.sensor_pwd             = 85,
	.vcm_pwd                = 1,
	.vcm_enable             = 0,
	.pdata                  = &msm_camera_device_data_front,
	.flash_data             = &flash_ov9726,
	.sensor_platform_info   = &ov9726_sensor_7627a_info,
	.csi_if                 = 1
};

static struct platform_device msm_camera_sensor_ov9726 = {
	.name   = "msm_camera_ov9726",
	.dev    = {
		.platform_data = &msm_camera_sensor_ov9726_data,
	},
};
#endif
#ifdef CONFIG_SR300PC20
static struct msm_camera_sensor_platform_info sr300pc20_sensor_7627a_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_sr300pc20 = {
	.flash_type             = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_info msm_camera_sensor_sr300pc20_data = {
	.sensor_name    = "sr300pc20",
	.sensor_reset_enable = 0,
	.vcm_enable             = 0,
	.pdata                  = &msm_camera_device_data_rear,
	.flash_data             = &flash_sr300pc20,
	.sensor_platform_info = &sr300pc20_sensor_7627a_info,
	.csi_if                 = 1

};

static struct platform_device msm_camera_sensor_sr300pc20 = {
	.name   = "msm_camera_sr300pc20",
	.dev    = {
		.platform_data = &msm_camera_sensor_sr300pc20_data,
	},
};
#endif

#ifdef CONFIG_S5K4ECGX
static struct msm_camera_sensor_platform_info s5k4ecgx_sensor_7627a_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_s5k4ecgx = {
	.flash_type             = MSM_CAMERA_FLASH_LED,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k4ecgx_data = {
	.sensor_name    = "s5k4ecgx",
	.sensor_reset_enable = 0,
	.vcm_enable             = 0,
	.pdata                  = &msm_camera_device_data_rear,
	.flash_data             = &flash_s5k4ecgx,
	.sensor_platform_info   = &s5k4ecgx_sensor_7627a_info,
	.csi_if                 = 1
};

static struct platform_device msm_camera_sensor_s5k4ecgx = {
	.name   = "msm_camera_s5k4ecgx",
	.dev    = {
		.platform_data = &msm_camera_sensor_s5k4ecgx_data,
	},
};
#endif

#ifdef CONFIG_MT9V113
static struct msm_camera_sensor_platform_info mt9v113_sensor_7627a_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_mt9v113 = {
	.flash_type             = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9v113_data = {
	.sensor_name    = "mt9v113",
	.sensor_reset_enable = 0,
	.vcm_enable             = 0,
	.pdata                  = &msm_camera_device_data_front,
	.flash_data             = &flash_mt9v113,
	.sensor_platform_info   = &mt9v113_sensor_7627a_info,
	.csi_if                 = 1 // 0: Parallel interface , 1: MIPI interface
};

static struct platform_device msm_camera_sensor_mt9v113 = {
	.name   = "msm_camera_mt9v113",
	.dev    = {
		.platform_data = &msm_camera_sensor_mt9v113_data,
	},
};
#endif
#ifdef CONFIG_MT9E013
static struct msm_camera_sensor_platform_info mt9e013_sensor_7627a_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_mt9e013 = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9e013_data = {
	.sensor_name    = "mt9e013",
	.sensor_reset   = 0,
	.sensor_reset_enable = 1,
	.sensor_pwd     = 85,
	.vcm_pwd        = 1,
	.vcm_enable     = 0,
	.pdata          = &msm_camera_device_data_rear,
	.flash_data     = &flash_mt9e013,
	.sensor_platform_info   = &mt9e013_sensor_7627a_info,
	.csi_if         = 1
};

static struct platform_device msm_camera_sensor_mt9e013 = {
	.name      = "msm_camera_mt9e013",
	.dev       = {
		.platform_data = &msm_camera_sensor_mt9e013_data,
	},
};
#endif

static struct i2c_board_info i2c_camera_devices[] = {
	#ifdef CONFIG_S5K4E1
	{
		I2C_BOARD_INFO("s5k4e1", 0x36),
	},
	{
		I2C_BOARD_INFO("s5k4e1_af", 0x8c >> 1),
	},
	#endif
	#ifdef CONFIG_WEBCAM_OV9726
	{
		I2C_BOARD_INFO("ov9726", 0x10),
	},
	#endif
	#ifdef CONFIG_MT9V113
	{
		I2C_BOARD_INFO("mt9v113", 0x7A >> 1),
	},
	#endif
	#ifdef CONFIG_S5K5CCAF // yjh
	{
		I2C_BOARD_INFO("s5k5ccaf", 0x40>>1),
	},
	#endif // yjh_end
	#ifdef CONFIG_SR300PC20
	{
		I2C_BOARD_INFO("sr300pc20", 0x40>>1),
	},
	#endif
    #ifdef CONFIG_S5K4ECGX
	{
		I2C_BOARD_INFO("s5k4ecgx", 0xAC >> 1),
	},
    #endif
	#ifdef CONFIG_IMX072
	{
		I2C_BOARD_INFO("imx072", 0x34),
	},
	#endif
	#ifdef CONFIG_MT9E013
	{
		I2C_BOARD_INFO("mt9e013", 0x6C >> 2),
	},
	#endif
	{
		I2C_BOARD_INFO("sc628a", 0x37),
	},
};
#endif

#if defined(CONFIG_SERIAL_MSM_HSL_CONSOLE) \
		&& defined(CONFIG_MSM_SHARED_GPIO_FOR_UART2DM)
static struct msm_gpio uart2dm_gpios[] = {
	{GPIO_CFG(19, 2, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
							"uart2dm_rfr_n" },
	{GPIO_CFG(20, 2, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
							"uart2dm_cts_n" },
	{GPIO_CFG(21, 2, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
							"uart2dm_rx"    },
	{GPIO_CFG(108, 2, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
							"uart2dm_tx"    },
};

static void msm7x27a_cfg_uart2dm_serial(void)
{
	int ret;
	ret = msm_gpios_request_enable(uart2dm_gpios,
					ARRAY_SIZE(uart2dm_gpios));
	if (ret)
		pr_err("%s: unable to enable gpios for uart2dm\n", __func__);
}
#else
static void msm7x27a_cfg_uart2dm_serial(void) { }
#endif

static struct platform_device *rumi_sim_devices[] __initdata = {
	&msm_device_dmov,
	&msm_device_smd,
	&smc91x_device,
	&msm_device_uart1,
	&msm_device_nand,
	&msm_device_uart_dm1,
	&msm_gsbi0_qup_i2c_device,
	&msm_gsbi1_qup_i2c_device
};

static struct platform_device *surf_ffa_devices[] __initdata = {
	&msm_device_dmov,
	&msm_device_smd,
	&msm_device_uart1,
	&msm_device_uart_dm1,
	&msm_device_nand,
	&msm_gsbi0_qup_i2c_device,
	&msm_gsbi1_qup_i2c_device,
	&msm_device_otg,
	&msm_device_gadget_peripheral,
	&android_usb_device,
	&android_pmem_device,
	&android_pmem_adsp_device,
	&android_pmem_audio_device,
	&msm_device_snd,
	&msm_device_adspdec,
	&msm_fb_device,
	&lcdc_trebon_panel_device,
	&msm_batt_device,
	&msm_kgsl_3d0,
#ifdef CONFIG_IMX072
	&msm_camera_sensor_imx072,
#endif
#ifdef CONFIG_WEBCAM_OV9726
	&msm_camera_sensor_ov9726,
#endif
#ifdef CONFIG_S5K5CCAF
	&msm_camera_sensor_s5k5ccaf,
#endif
#ifdef CONFIG_SR300PC20
	&msm_camera_sensor_sr300pc20,
#endif
#ifdef CONFIG_S5K4ECGX
	&msm_camera_sensor_s5k4ecgx,
#endif

#ifdef CONFIG_MT9V113
	&msm_camera_sensor_mt9v113,
#endif
#ifdef CONFIG_MT9E013
	&msm_camera_sensor_mt9e013,
#endif
#ifdef CONFIG_FB_MSM_MIPI_DSI
	&mipi_dsi_renesas_panel_device,
#endif
#ifdef CONFIG_BT
	&msm_bt_power_device,
#endif
	&touch_i2c_gpio_device,
#if (CONFIG_MACH_TREBON_HWREV == 0x0)
	&sensor_i2c_gpio_device,
#endif
#ifdef CONFIG_BQ27425_FUEL_GAUGE
	&fuelgauge_i2c_gpio_device,
#endif
	&fsa880_i2c_gpio_device,
	&msm_device_pmic_leds,
	&msm_vibrator_device,
#ifdef CONFIG_SAMSUNG_JACK
	&sec_device_jack,
#endif
	&asoc_msm_pcm,
	&asoc_msm_dai0,
	&asoc_msm_dai1,
};

static unsigned pmem_kernel_ebi1_size = PMEM_KERNEL_EBI1_SIZE;
static int __init pmem_kernel_ebi1_size_setup(char *p)
{
	pmem_kernel_ebi1_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_kernel_ebi1_size", pmem_kernel_ebi1_size_setup);

static unsigned pmem_audio_size = MSM_PMEM_AUDIO_SIZE;
static int __init pmem_audio_size_setup(char *p)
{
	pmem_audio_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_audio_size", pmem_audio_size_setup);

static void __init msm_msm7x2x_allocate_memory_regions(void)
{
	void *addr;
	unsigned long size;

	size = fb_size ? : MSM_FB_SIZE;
	addr = alloc_bootmem_align(size, 0x1000);
	msm_fb_resources[0].start = __pa(addr);
	msm_fb_resources[0].end = msm_fb_resources[0].start + size - 1;
	pr_info("allocating %lu bytes at %p (%lx physical) for fb\n",
		size, addr, __pa(addr));
}

static struct memtype_reserve msm7x27a_reserve_table[] __initdata = {
	[MEMTYPE_SMI] = {
	},
	[MEMTYPE_EBI0] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
	[MEMTYPE_EBI1] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
};

static void __init size_pmem_devices(void)
{
#ifdef CONFIG_ANDROID_PMEM
	android_pmem_adsp_pdata.size = pmem_adsp_size;
	android_pmem_pdata.size = pmem_mdp_size;
	android_pmem_audio_pdata.size = pmem_audio_size;
#endif
}

static void __init reserve_memory_for(struct android_pmem_platform_data *p)
{
	msm7x27a_reserve_table[p->memory_type].size += p->size;
}

static void __init reserve_pmem_memory(void)
{
#ifdef CONFIG_ANDROID_PMEM
	reserve_memory_for(&android_pmem_adsp_pdata);
	reserve_memory_for(&android_pmem_pdata);
	reserve_memory_for(&android_pmem_audio_pdata);
	msm7x27a_reserve_table[MEMTYPE_EBI1].size += pmem_kernel_ebi1_size;
#endif
}

static void __init msm7x27a_calculate_reserve_sizes(void)
{
	size_pmem_devices();
	reserve_pmem_memory();
}

static int msm7x27a_paddr_to_memtype(unsigned int paddr)
{
	return MEMTYPE_EBI1;
}

static struct reserve_info msm7x27a_reserve_info __initdata = {
	.memtype_reserve_table = msm7x27a_reserve_table,
	.calculate_reserve_sizes = msm7x27a_calculate_reserve_sizes,
	.paddr_to_memtype = msm7x27a_paddr_to_memtype,
};

static void __init msm7x27a_reserve(void)
{
	reserve_info = &msm7x27a_reserve_info;
	msm_reserve();
}

static void __init msm_device_i2c_init(void)
{
	msm_gsbi0_qup_i2c_device.dev.platform_data = &msm_gsbi0_qup_i2c_pdata;
	msm_gsbi1_qup_i2c_device.dev.platform_data = &msm_gsbi1_qup_i2c_pdata;

	gpio_tlmm_config(GPIO_CFG(GPIO_TSP_SDA, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_TSP_SCL, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_MUS_SDA, 0, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_MUS_SCL, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
#if (CONFIG_MACH_TREBON_HWREV == 0x0)
	#if defined(CONFIG_SENSORS_HSCD) || defined(CONFIG_PROXIMITY_SENSOR)
	gpio_tlmm_config(GPIO_CFG(GPIO_SENSOR_SCL, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_SENSOR_SDA, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
	#endif
#endif
#if defined(CONFIG_PN544)
	gpio_tlmm_config(GPIO_CFG(GPIO_NFC_SDA, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_NFC_SCL, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
#endif
	printk("[TSP] %s =======gpio_request==test======ln=%d\n",
			__func__, __LINE__);
}

static struct msm_panel_common_pdata mdp_pdata = {
	.gpio = 97,
	.mdp_rev = MDP_REV_303,
};

#define GPIO_LCDC_BRDG_PD	128
#define GPIO_LCDC_BRDG_RESET_N	129

#define LCDC_RESET_PHYS		0x90008014

static	void __iomem *lcdc_reset_ptr;

static unsigned mipi_dsi_gpio[] = {
	GPIO_CFG(GPIO_LCDC_BRDG_RESET_N, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL,
		GPIO_CFG_2MA),       /* LCDC_BRDG_RESET_N */
	GPIO_CFG(GPIO_LCDC_BRDG_PD, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL,
		GPIO_CFG_2MA),       /* LCDC_BRDG_RESET_N */
};

enum {
	DSI_SINGLE_LANE = 1,
	DSI_TWO_LANES,
};

static int msm_fb_get_lane_config(void)
{
	int rc = DSI_TWO_LANES;

	if (machine_is_msm7625a_surf() || machine_is_msm7625a_ffa()) {
		rc = DSI_SINGLE_LANE;
		pr_info("DSI Single Lane\n");
	} else {
		pr_info("DSI Two Lanes\n");
	}
	return rc;
}

static int msm_fb_dsi_client_reset(void)
{
	int rc = 0;

	rc = gpio_request(GPIO_LCDC_BRDG_RESET_N, "lcdc_brdg_reset_n");
	if (rc < 0) {
		pr_err("failed to request lcd brdg reset_n\n");
		return rc;
	}

	rc = gpio_request(GPIO_LCDC_BRDG_PD, "lcdc_brdg_pd");
	if (rc < 0) {
		pr_err("failed to request lcd brdg pd\n");
		return rc;
	}

	rc = gpio_tlmm_config(mipi_dsi_gpio[0], GPIO_CFG_ENABLE);
	if (rc) {
		pr_err("Failed to enable LCDC Bridge reset enable\n");
		goto gpio_error;
	}

	rc = gpio_tlmm_config(mipi_dsi_gpio[1], GPIO_CFG_ENABLE);
	if (rc) {
		pr_err("Failed to enable LCDC Bridge pd enable\n");
		goto gpio_error2;
	}

	rc = gpio_direction_output(GPIO_LCDC_BRDG_RESET_N, 1);
	rc |= gpio_direction_output(GPIO_LCDC_BRDG_PD, 1);
	gpio_set_value_cansleep(GPIO_LCDC_BRDG_PD, 0);

	if (!rc) {
		if (machine_is_msm7x27a_surf()) {
			lcdc_reset_ptr = ioremap_nocache(LCDC_RESET_PHYS,
				sizeof(uint32_t));

			if (!lcdc_reset_ptr)
				return 0;
		}
		return rc;
	} else {
		goto gpio_error;
	}

gpio_error2:
	pr_err("Failed GPIO bridge pd\n");
	gpio_free(GPIO_LCDC_BRDG_PD);

gpio_error:
	pr_err("Failed GPIO bridge reset\n");
	gpio_free(GPIO_LCDC_BRDG_RESET_N);
	return rc;
}

static const char * const msm_fb_dsi_vreg[] = {
	"gp2",
	"msme1",
	"mddi"
};

static const int msm_fb_dsi_vreg_mV[] = {
	2850,
	1800,
	1200
};

static struct vreg *dsi_vreg[ARRAY_SIZE(msm_fb_dsi_vreg)];
static int dsi_gpio_initialized;

static int mipi_dsi_panel_power(int on)
{
	int i, rc = 0;
	uint32_t lcdc_reset_cfg;

	printk(KERN_INFO "%s: %d (on = 1, off = 0)\n", __func__, on);

	/* I2C-controlled GPIO Expander -init of the GPIOs very late */
	if (!dsi_gpio_initialized) {
		pmapp_disp_backlight_init();

		rc = gpio_request(GPIO_DISPLAY_PWR_EN, "gpio_disp_pwr");
		if (rc < 0) {
			pr_err("failed to request gpio_disp_pwr\n");
			return rc;
		}

		rc = gpio_direction_output(GPIO_DISPLAY_PWR_EN, 1);
		if (rc < 0) {
			pr_err("failed to enable display pwr\n");
			goto fail_gpio1;
		}

		if (machine_is_msm7x27a_surf()) {
			rc = gpio_request(GPIO_BACKLIGHT_EN, "gpio_bkl_en");
			if (rc < 0) {
				pr_err("failed to request gpio_bkl_en\n");
				goto fail_gpio1;
			}

			rc = gpio_direction_output(GPIO_BACKLIGHT_EN, 1);
			if (rc < 0) {
				pr_err("failed to enable backlight\n");
				goto fail_gpio2;
			}
		}

		for (i = 0; i < ARRAY_SIZE(msm_fb_dsi_vreg); i++) {
			dsi_vreg[i] = vreg_get(0, msm_fb_dsi_vreg[i]);

			if (IS_ERR(dsi_vreg[i])) {
				pr_err("%s: vreg get failed with : (%ld)\n",
					__func__, PTR_ERR(dsi_vreg[i]));
				goto fail_gpio2;
			}

			rc = vreg_set_level(dsi_vreg[i],
				msm_fb_dsi_vreg_mV[i]);

			if (rc < 0) {
				pr_err("%s: set regulator level failed "
					"with :(%d)\n",	__func__, rc);
				goto vreg_fail1;
			}
		}
		dsi_gpio_initialized = 1;
	}

		gpio_set_value_cansleep(GPIO_DISPLAY_PWR_EN, on);
		if (machine_is_msm7x27a_surf()) {
			gpio_set_value_cansleep(GPIO_BACKLIGHT_EN, on);
		}

		if (on) {
			gpio_set_value_cansleep(GPIO_LCDC_BRDG_PD, 0);

			if (machine_is_msm7x27a_surf()) {
				lcdc_reset_cfg = readl_relaxed(lcdc_reset_ptr);
				rmb();
				lcdc_reset_cfg &= ~1;

				writel_relaxed(lcdc_reset_cfg, lcdc_reset_ptr);
				msleep(20);
				wmb();
				lcdc_reset_cfg |= 1;
				writel_relaxed(lcdc_reset_cfg, lcdc_reset_ptr);
			} else {
				gpio_set_value_cansleep(GPIO_LCDC_BRDG_RESET_N,
					0);
				msleep(20);
				gpio_set_value_cansleep(GPIO_LCDC_BRDG_RESET_N,
					1);
			}

			if (pmapp_disp_backlight_set_brightness(100))
				pr_err("backlight set brightness failed\n");
		} else {
			gpio_set_value_cansleep(GPIO_LCDC_BRDG_PD, 1);

			if (pmapp_disp_backlight_set_brightness(0))
				pr_err("backlight set brightness failed\n");
		}

		/* Configure vreg lines */
		for (i = 0; i < ARRAY_SIZE(msm_fb_dsi_vreg); i++) {
			if (on) {
				rc = vreg_enable(dsi_vreg[i]);

				if (rc) {
					printk(KERN_ERR "vreg_enable: %s vreg"
						"operation failed\n",
						msm_fb_dsi_vreg[i]);

					goto vreg_fail2;
				}
			} else {
				rc = vreg_disable(dsi_vreg[i]);

				if (rc) {
					printk(KERN_ERR "vreg_disable: %s vreg "
						"operation failed\n",
						msm_fb_dsi_vreg[i]);
					goto vreg_fail2;
				}
			}
		}

	return rc;

vreg_fail2:
	if (on) {
		for (; i > 0; i--)
			vreg_disable(dsi_vreg[i - 1]);
	} else {
		for (; i > 0; i--)
			vreg_enable(dsi_vreg[i - 1]);
	}

	return rc;

vreg_fail1:
	for (; i > 0; i--)
		vreg_put(dsi_vreg[i - 1]);

fail_gpio2:
	gpio_free(GPIO_BACKLIGHT_EN);
fail_gpio1:
	gpio_free(GPIO_DISPLAY_PWR_EN);
	dsi_gpio_initialized = 0;
	return rc;
}

#define MDP_303_VSYNC_GPIO 97

#ifdef CONFIG_FB_MSM_MDP303
static struct mipi_dsi_platform_data mipi_dsi_pdata = {
	.vsync_gpio = MDP_303_VSYNC_GPIO,
	.dsi_power_save   = mipi_dsi_panel_power,
	.dsi_client_reset = msm_fb_dsi_client_reset,
	.get_lane_config = msm_fb_get_lane_config,
};
#endif

static void __init msm_fb_add_devices(void)
{
	msm_fb_register_device("mdp", &mdp_pdata);
	msm_fb_register_device("lcdc", &lcdc_pdata);
#ifdef CONFIG_FB_MSM_MDP303
	msm_fb_register_device("mipi_dsi", &mipi_dsi_pdata);
#endif
}

#define MSM_EBI2_PHYS			0xa0d00000
#define MSM_EBI2_XMEM_CS2_CFG1		0xa0d10030

static void __init msm7x27a_init_ebi2(void)
{
	uint32_t ebi2_cfg;
	void __iomem *ebi2_cfg_ptr;

	ebi2_cfg_ptr = ioremap_nocache(MSM_EBI2_PHYS, sizeof(uint32_t));
	if (!ebi2_cfg_ptr)
		return;

	ebi2_cfg = readl(ebi2_cfg_ptr);
	if (machine_is_msm7x27a_rumi3() || machine_is_msm7x27a_surf())
		ebi2_cfg |= (1 << 4); /* CS2 */

	writel(ebi2_cfg, ebi2_cfg_ptr);
	iounmap(ebi2_cfg_ptr);

	/* Enable A/D MUX[bit 31] from EBI2_XMEM_CS2_CFG1 */
	ebi2_cfg_ptr = ioremap_nocache(MSM_EBI2_XMEM_CS2_CFG1,
							 sizeof(uint32_t));
	if (!ebi2_cfg_ptr)
		return;

	ebi2_cfg = readl(ebi2_cfg_ptr);
	if (machine_is_msm7x27a_surf())
		ebi2_cfg |= (1 << 31);

	writel(ebi2_cfg, ebi2_cfg_ptr);
	iounmap(ebi2_cfg_ptr);
}

static void keypad_gpio_init(void)
{
	int rc = 0;
	printk(KERN_INFO "[KEY] %s start\n", __func__);

	gpio_tlmm_config(GPIO_CFG(36, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, \
						GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(37, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, \
						GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(39, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, \
						GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(31, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, \
						GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_set_value_cansleep(31, 0);
}

#define KP_INDEX(row, col) ((row)*ARRAY_SIZE(kp_col_gpios) + (col))
	static unsigned int kp_row_gpios[] = {36, 37, 39};
	static unsigned int kp_col_gpios[] = {31};
	static unsigned int kp_wakeup_gpios[] = {37};
	static const unsigned short keymap[ARRAY_SIZE(kp_col_gpios) *
					  ARRAY_SIZE(kp_row_gpios)] = {
		[KP_INDEX(0, 0)] = KEY_VOLUMEDOWN,

		[KP_INDEX(1, 0)] = KEY_HOME,

		[KP_INDEX(2, 0)] = KEY_VOLUMEUP,
	};

/* SURF keypad platform device information */
static struct gpio_event_matrix_info kp_matrix_info = {
	.info.func	= gpio_event_matrix_func,
	.keymap		= keymap,
	.output_gpios	= kp_col_gpios,
	.input_gpios	= kp_row_gpios,
	.wakeup_gpios	= kp_wakeup_gpios,
	.nwakeups	= ARRAY_SIZE(kp_wakeup_gpios),
	.noutputs	= ARRAY_SIZE(kp_col_gpios),
	.ninputs	= ARRAY_SIZE(kp_row_gpios),
	.settle_time.tv_nsec = 40 * NSEC_PER_USEC,
	.poll_time.tv_nsec = 20 * NSEC_PER_MSEC,
	.debounce_delay.tv_nsec = 20 * NSEC_PER_MSEC,
	.flags		= GPIOKPF_LEVEL_TRIGGERED_IRQ | GPIOKPF_DRIVE_INACTIVE |
			  GPIOKPF_PRINT_UNMAPPED_KEYS | GPIOKPF_DEBOUNCE,
};

static struct gpio_event_info *kp_info[] = {
	&kp_matrix_info.info
};

static struct gpio_event_platform_data kp_pdata = {
	.name		= "7x27a_kp",
	.info		= kp_info,
	.info_count	= ARRAY_SIZE(kp_info)
};

static struct platform_device kp_pdev = {
	.name	= GPIO_EVENT_DEV_NAME,
	.id	= -1,
	.dev	= {
		.platform_data	= &kp_pdata,
	},
};

static struct msm_handset_platform_data hs_platform_data = {
	.hs_name = "sec_jack",
	.pwr_key_delay_ms = 500, /* 0 will disable end key */
};

static struct platform_device hs_pdev = {
	.name   = "msm-handset",
	.id     = -1,
	.dev    = {
		.platform_data = &hs_platform_data,
	},
};

static struct platform_device msm_proccomm_regulator_dev = {
	.name   = PROCCOMM_REGULATOR_DEV_NAME,
	.id     = -1,
	.dev    = {
		.platform_data = &msm7x27a_proccomm_regulator_data
	}
};

static void __init msm7627a_rumi3_init(void)
{
	msm7x27a_init_ebi2();
	platform_add_devices(rumi_sim_devices,
			ARRAY_SIZE(rumi_sim_devices));
}

#define LED_GPIO_PDM		96
#define UART1DM_RX_GPIO		45

static int __init msm7x27a_init_ar6000pm(void)
{
	return platform_device_register(&msm_wlan_ar6000_pm_device);
}

static void __init msm7x27a_init_regulators(void)
{
	int rc = platform_device_register(&msm_proccomm_regulator_dev);
	if (rc)
		pr_err("%s: could not register regulator device: %d\n",
				__func__, rc);
}

static struct msm7x27a_regulators {
  const char *id;
  unsigned   voltage; // in mv
};

static struct msm7x27a_regulators msm7x27a_reg[] = {
[0] = {
        .id = "smps3",
        .voltage = 1800,
      },
};

/* function to Enable the regulators */
static void msm7x27a_enable_regulators(void)
{
   struct vreg *vreg = NULL;
   struct msm7x27a_regulators *regulators = msm7x27a_reg;
   static int i = 0, ret =0, VRG_SIZE =0;

   /* Get the count of regulators */
   VRG_SIZE= ARRAY_SIZE(msm7x27a_reg);

   for(i=0; i<VRG_SIZE; i++) {

       /* get the regulator descriptor */
       vreg = vreg_get( NULL, regulators[i].id);

       if(IS_ERR(vreg)) {
         pr_err("failed to get the regulator %s\n",regulators[i].id);
         return ;
       }

       /* set the regulator voltage(optimum) */
       ret = vreg_set_level(vreg,regulators[i].voltage);

       if(ret)
       {
         pr_err("failed to set the voltage level for regulator %s\n",regulators[i].id);
         return ;
       }

       /* enable the regulator or ldo */
       if(vreg_enable(vreg))
       {
         pr_err("failed to enable the regulator %s\n",regulators[i].id);
         return ;
       }
    }
   pr_debug("Successfully enabled all regulators\n");
   return;
}

static void __init msm7x2x_init(void)
{
    msm7x2x_misc_init();

	/* Initialize the regulators */
	msm7x27a_init_regulators();

	/* Enable the Required regulators */
    msm7x27a_enable_regulators();

	/* Common functions for SURF/FFA/RUMI3 */
	msm_device_i2c_init();
	msm7x27a_init_ebi2();

#ifdef CONFIG_SERIAL_MSM_HS
	msm_uart_dm1_pdata.wakeup_irq = gpio_to_irq(UART1DM_RX_GPIO);
	msm_device_uart_dm1.dev.platform_data = &msm_uart_dm1_pdata;
#endif

#ifdef CONFIG_USB_MSM_OTG_72K
	msm_otg_pdata.swfi_latency =
		msm7x27a_pm_data
		[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].latency;
	msm_device_otg.dev.platform_data = &msm_otg_pdata;
#endif
	msm_device_gadget_peripheral.dev.platform_data =
		&msm_gadget_pdata;
	msm7x27a_cfg_smsc911x();
#ifdef CONFIG_SAMSUNG_JACK
		sec_jack_gpio_init();
#endif
	platform_add_devices(msm_footswitch_devices,
			msm_num_footswitch_devices);
	platform_add_devices(surf_ffa_devices,
			ARRAY_SIZE(surf_ffa_devices));
		if (!kernel_uart_flag)
		{
			platform_device_register(&msm_device_uart3);
		}

	/* Ensure ar6000pm device is registered before MMC/SDC */
	msm7x27a_init_ar6000pm();
	msm7x27a_init_mmc();

	lcdc_trebon_gpio_init();
	msm_fb_add_devices();


#ifdef CONFIG_USB_EHCI_MSM_72K
	msm7x2x_init_host();
#endif

	msm_pm_set_platform_data(msm7x27a_pm_data,
				ARRAY_SIZE(msm7x27a_pm_data));

	BUG_ON(msm_pm_boot_init(&msm_pm_boot_pdata));

#if defined(CONFIG_I2C) && defined(CONFIG_GPIO_SX150X)
	register_i2c_devices();
#endif
#ifdef _CONFIG_MACH_TREBON
    wlan_power_init();
#endif
#if defined(CONFIG_BT) && defined(CONFIG_MARIMBA_CORE)
	bt_power_init();
#endif

#ifdef CONFIG_TOUCHSCREEN_ZINITIX_A
	tsp_power_on();
#endif

	samsung_sys_class_init();
	i2c_register_board_info( 2, touch_i2c_devices, ARRAY_SIZE(touch_i2c_devices));
	i2c_register_board_info( 3, fsa880_i2c_devices, ARRAY_SIZE(fsa880_i2c_devices));
#ifdef CONFIG_BQ27425_FUEL_GAUGE
	i2c_register_board_info(6, fg_i2c_devices, ARRAY_SIZE(fg_i2c_devices));
#endif

	i2c_register_board_info(MSM_GSBI0_QUP_I2C_BUS_ID,
			i2c_camera_devices,
			ARRAY_SIZE(i2c_camera_devices));
	keypad_gpio_init();
	platform_device_register(&kp_pdev);
	platform_device_register(&hs_pdev);

	/* Configure it as a pdm function*/
	if (gpio_tlmm_config(GPIO_CFG(LED_GPIO_PDM, 3,
				GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL,
				GPIO_CFG_8MA), GPIO_CFG_ENABLE))
		pr_err("%s: gpio_tlmm_config for %d failed\n",
			__func__, LED_GPIO_PDM);
	else
		platform_device_register(&led_pdev);

#ifdef CONFIG_MSM_RPC_VIBRATOR
	if (machine_is_msm7x27a_ffa() || machine_is_msm7625a_ffa())
		msm_init_pmic_vibrator();
#endif
       /*7x25a kgsl initializations*/
       msm7x25a_kgsl_3d0_init();

	   //ar6000_prealloc_init();

#if defined(CONFIG_PN544)
	config_gpio_table_for_nfc();
	platform_device_register(&pn544_i2c_gpio_device);
	i2c_register_board_info(5, pn544_i2c_devices,
		ARRAY_SIZE(pn544_i2c_devices));
#endif
}

static void __init msm7x2x_init_early(void)
{
	msm_msm7x2x_allocate_memory_regions();
}

MACHINE_START(MSM7X27A_RUMI3, "QCT MSM7x27a RUMI3")
	.boot_params	= PHYS_OFFSET + 0x100,
	.map_io		= msm_common_io_init,
	.reserve	= msm7x27a_reserve,
	.init_irq	= msm_init_irq,
	.init_machine	= msm7627a_rumi3_init,
	.timer		= &msm_timer,
	.init_early     = msm7x2x_init_early,
	.handle_irq	= vic_handle_irq,
MACHINE_END
MACHINE_START(MSM7X27A_SURF, "QCT MSM7x27a SURF")
	.boot_params	= PHYS_OFFSET + 0x100,
	.map_io		= msm_common_io_init,
	.reserve	= msm7x27a_reserve,
	.init_irq	= msm_init_irq,
	.init_machine	= msm7x2x_init,
	.timer		= &msm_timer,
	.init_early     = msm7x2x_init_early,
	.handle_irq	= vic_handle_irq,
MACHINE_END
MACHINE_START(MSM7X27A_FFA, "QCT MSM7x27a FFA")
	.boot_params	= PHYS_OFFSET + 0x100,
	.map_io		= msm_common_io_init,
	.reserve	= msm7x27a_reserve,
	.init_irq	= msm_init_irq,
	.init_machine	= msm7x2x_init,
	.timer		= &msm_timer,
	.init_early     = msm7x2x_init_early,
	.handle_irq	= vic_handle_irq,
MACHINE_END
MACHINE_START(MSM7625A_SURF, "QCT MSM7625a SURF")
	.boot_params    = PHYS_OFFSET + 0x100,
	.map_io         = msm_common_io_init,
	.reserve        = msm7x27a_reserve,
	.init_irq       = msm_init_irq,
	.init_machine   = msm7x2x_init,
	.timer          = &msm_timer,
	.init_early     = msm7x2x_init_early,
	.handle_irq	= vic_handle_irq,
MACHINE_END
MACHINE_START(MSM7625A_FFA, "QCT MSM7625a FFA")
	.boot_params    = PHYS_OFFSET + 0x100,
	.map_io         = msm_common_io_init,
	.reserve        = msm7x27a_reserve,
	.init_irq       = msm_init_irq,
	.init_machine   = msm7x2x_init,
	.timer          = &msm_timer,
	.init_early     = msm7x2x_init_early,
	.handle_irq	= vic_handle_irq,
MACHINE_END
