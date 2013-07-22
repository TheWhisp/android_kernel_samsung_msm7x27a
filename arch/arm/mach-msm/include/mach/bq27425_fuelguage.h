#ifndef __BQ27425_FUELGUAGE_H__
#define __BQ27425_FUELGUAGE_H__

extern int is_attached;
unsigned int bq27425_get_vcell(void);
unsigned int bq27425_get_soc(void);
int bq27425_get_temperature(void);
unsigned int bq27425_get_current(void);
int bq27425_reset_soc(void);
unsigned int bq27425_get_flag(void);

#endif
