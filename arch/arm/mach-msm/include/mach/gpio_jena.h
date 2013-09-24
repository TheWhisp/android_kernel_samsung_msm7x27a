/* JENA GPIOS */

#ifndef __GPIO_JENA_H_
#define __GPIO_JENA_H_

#define GPIO_LCD_MCLK			4
#define GPIO_LCD_R_7			5
#define GPIO_LCD_R_6			6
#define GPIO_LCD_R_5			7
#define GPIO_LCDR_4				8
#define GPIO_LCD_R_3			9
#define GPIO_LCD_R_2			10
#define GPIO_LCD_R_1			11
#define GPIO_LCD_R_0			12
							/* 13 */
							/* 14 */
#define GPIO_CAM_MCLK			15
							/* 16 */
							/* 17 */
#define GPIO_SENSOR_SCL			16
#define GPIO_SENSOR_SDA			17
#ifdef	CONFIG_JENA_REV00
#define GPIO_ON_SW				18
#endif
							/* 19 */
#ifdef	CONFIG_JENA_REV00
#define GPIO_SENSOR_RDY			20
#endif

#define GPIO_PHONE_RXD			21
#define GPIO_LCD_RESET_N		22
#define GPIO_LCD_SDO			23
#define GPIO_PM_INT_N			24
#define GPIO_PS_HOLD			25
#define GPIO_LCD_CS				26
#define GPIO_TOUCH_IRQ			27
#define GPIO_MUSB_INT			28
#define GPIO_PROXI_INT			29
#define GPIO_LCD_SCL			30
#define GPIO_KBC_0				31
							/* 32 */
#define GPIO_WLAN_RESET_N		33
#define GPIO_BT_PWR				34
#define GPIO_TSP_SCL			35
#define GPIO_KBR_1				36
#define GPIO_KBR_2				37

#ifdef	CONFIG_JENA_REV00
#define GPIO_KBR_3				38
#define GPIO_KBR_0				39
#else
#define GPIO_KBR_4				39
#endif

#define GPIO_TSP_SDA			40
#define GPIO_FM_SDA				41
#define GPIO_WLAN_HOST_WAKE		42
#define GPIO_BT_UART_RTS		43
#define GPIO_BT_UART_CTS		44
#define GPIO_BT_UART_RXD		45
#define GPIO_BT_UART_TXD		46
#define GPIO_SIM1_CLK_MSM		47
#define GPIO_JACK_INT_N			48
							/* 49 */
#define GPIO_SIM1_DATA_MSM		50
#define GPIO_MICROSD_DATA_3		51
#define GPIO_MICROSD_DATA_2		52
#define GPIO_MICROSD_DATA_1		53
#define GPIO_MICROSD_DATA_0		54
#define GPIO_MICROSD_CMD		55
#define GPIO_MICROSD_CLK		56
#define GPIO_LCD_SDA			57
							/* 58 */
#define GPIO_SBDT_RTR6285A		59
#define GPIO_CAM_I2C_SCL		60
#define GPIO_CAM_I2C_SDA		61
#define GPIO_WLAN_SD_CLK		62
#define GPIO_WLAN_SD_CMD		63
#define GPIO_WLAN_SD_DATA_3		64
#define GPIO_WLAN_SD_DATA_2		65
#define GPIO_WLAN_SD_DATA_1		66
#define GPIO_WLAN_SD_DATA_0		67
#define GPIO_BT_PCM_DOUT		68
#define GPIO_BT_PCM_DIN			69
#define GPIO_BT_PCM_SYNC		70
#define GPIO_BT_PCM_CLK			71
#define GPIO_ANT_SEL_3			72
#define GPIO_ANT_SEL_2			73
#define GPIO_ANT_SEL_1			74

#ifdef	CONFIG_JENA_REV00
#define GPIO_ANT_SEL_0			75
#define GPIO_LCD_BL_CTRL		76
							/* 77 */
							/* 78 */
							/* 79 */
							/* 80 */
#else
#define GPIO_KEY_LED_EN			76
#define GPIO_ANT_SEL_0			80
#endif

#define GPIO_GPS_LNA_ON			81
							/* 82 */
#define GPIO_FM_RDS_INT			83

#ifdef	CONFIG_JENA_REV00
#define GPIO_SIM2_CLK_MSM		84
#endif

#define GPIO_3M_CAM_RESET		85
							/* 86 */

#ifdef	CONFIG_JENA_REV00
#define GPIO_SIM2_DATA_MSM		87
#define GPIO_EAR_SEL			88
#else
#define GPIO_CAMIO_EN			88
#endif

							/* 89 */
#define GPIO_MUS_SCL			123
#define GPIO_MUS_SDA			122

#ifdef	CONFIG_JENA_REV00
#define GPIO_EAR_SWITCH			92
							/* 93 */
#endif

#define GPIO_nTF_DETECT			94

#ifdef	CONFIG_JENA_REV00
#define GPIO_REG_MSMP_1_8V		95
#endif

#define GPIO_3M_CAM_STBY		96
#define GPIO_UART_BOOT_ON		97
							/* 98 */

#ifdef	CONFIG_JENA_REV00
							/* 107 */
#else
#define GPIO_WLAN_18V_EN		107
#endif

#define GPIO_PHONE_TXD			108

#ifdef	CONFIG_JENA_REV00
							/* 109 */
#else
#define GPIO_WLAN_33V_EN		109
#endif

#define GPIO_PA_ON_W900			110
#define GPIO_LCD_G_1			111
#define GPIO_LCD_G_0			112
#define GPIO_LCD_B_7			113
							/* 114 */
#define GPIO_LCD_B_5			115
#define GPIO_LCD_B_4			116
#define GPIO_LCD_B_3			117
#define GPIO_LCD_B_2			118
#define GPIO_LCD_G_4			119
#define GPIO_LCD_G_3			120
#define GPIO_LCDE_G_2			121
							/* 122 */

#ifdef	CONFIG_JENA_REV00
#define GPIO_CAM_AVDD_EN		123
#define GPIO_CAN_VDDIO_EN		124
#endif

#define GPIO_LCD_B_1			125
#define GPIO_LCD_B_0			126
#define GPIO_LCD_VSYNC			127
#define GPIO_LCD_HSYNC			128
#define GPIO_LCD_EN				129
#define GPIO_LCD_G_5			130
#define GPIO_I2C_SCL			131
#define GPIO_I2C_SDA			132

#if defined(CONFIG_PN544)
#define GPIO_NFC_SDA			13
#define GPIO_NFC_SCL			14
#define GPIO_NFC_FIRM			49
#define GPIO_NFC_EN				58
#define GPIO_NFC_IRQ			114
#endif

#endif	/* __GPIO_JENA_H_ */
