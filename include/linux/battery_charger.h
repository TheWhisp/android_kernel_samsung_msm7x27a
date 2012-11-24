
#ifndef _CHARGER_BATTERY_H_
#define _CHARGER_BATTERY_H_
/* update interval time. */
#define INTERVAL	30

#include <linux/power_supply.h>
#include <linux/irq.h>
/* C110 specific type definitions. */

enum {
	CABLE_TYPE_UNKNOWN = 0,
	CABLE_TYPE_USB,
	CABLE_TYPE_TA,
    CABLE_TYPE_USB_OTG,
};

enum {
	PF_EVENT_OFFLINE = 0,
	PF_EVENT_ONLINE,
	PF_EVENT_UNKNOWN,
	PF_EVENT_FULL,
	PF_EVENT_COUNT_FULL,
	PF_EVENT_ABNORMAL_TEMP,
	PF_EVENT_RECOVER_TEMP,
};

enum {
	BC_EVENT_OFFLINE = 0,
	BC_EVENT_ONLINE,
	BC_EVENT_UNKNOWN,
	BC_EVENT_FULL,
	BC_EVENT_REAL_FULL,
	BC_EVENT_FAULT,
	BC_EVENT_RECOVER_TEMP,
};

struct battery_charger_status {
	unsigned int voltage_uV;		/* units of uV */
	unsigned int voltage_mV;		/* units of mV */
	unsigned int voltage_adc;		/* adc data of voltage */
	unsigned int voltage_adc2;		/* debug adc data of voltage */
	unsigned int high_capacity;		/* percentage */
	unsigned int low_capacity;		/* additional resolution capacity */
	unsigned int temp_raw;			/* raw temperature data */
	int temp_C;			/* units of C */
	unsigned int vf;				/* battery connection status */
	unsigned int vf_adc;			/* battery connection adc */
	unsigned int ovp;				/* overvoltage protection */
	unsigned int timeover;			/* charging timeover */
	unsigned int full;				/* full_charging */
};

struct battery_charger_irq_handlers {
	void *dev_id;

	int ovp_irq;
	int timeover_irq;
	int full_irq;
	int real_full_irq;
	int fault_irq;
	int low_irq;
	int very_low_irq;

	irqreturn_t (*ovp_handler)(int irq, void *dev_id);
	irqreturn_t (*timeover_handler)(int irq, void *dev_id);
	irqreturn_t (*full_handler)(int irq, void *dev_id);
	irqreturn_t (*real_full_handler)(int irq, void *dev_id);
	irqreturn_t (*fault_handler)(int irq, void *dev_id);
	irqreturn_t (*low_handler)(int irq, void *dev_id);
	irqreturn_t (*very_low_handler)(int irq, void *dev_id);
};

struct battery_charger_ops {
	/* enable/disable charging
	 */
	void (*charging_onoff)(unsigned int);

	/* read low_capacity, high_capacity, voltage_uV,
	 * temp_C, temp_raw, vf
	 */
	int (*read_status)(struct battery_charger_status *);

	int (*initialize)(void);
	int (*release)(void);
	int (*enable_irq)(struct battery_charger_irq_handlers *);
	int (*free_irq)(struct battery_charger_irq_handlers *);
};

struct battery_charger_platform_data {
	/*---- parameters ----*/
    unsigned int skip_vf_check;
	unsigned int temp_relow;	
	unsigned int temp_low;
	unsigned int temp_rehigh;
	unsigned int temp_high;

	/* came from msm-battery
	 * check the necessity
	 */
	u32 voltage_max_design;
	u32 voltage_min_design;
	u32 batt_technology;

	int ovp_irq;
	int timeover_irq;
	int full_irq;
	int real_full_irq;
	int fault_irq;
	int low_irq;
	int very_low_irq;

	/*---- functions ----*/
	struct battery_charger_ops *ops;
	int (*get_charger_status)(void);
};

int battery_charger_is_charging(void);
int charger_battery_tick_handler(void);

extern unsigned int battery_debug;

#endif /* _CHARGER_BATTERY_H_ */
