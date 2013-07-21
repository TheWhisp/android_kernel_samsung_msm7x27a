#ifndef ___SENSORS_HEAD_H_INCLUDED
#define ___SENSORS_HEAD_H_INCLUDED

#define PROBE_SUCCESS 1
#define PROBE_FAIL	  0
extern unsigned int board_hw_revision;


int hscd_get_magnetic_field_data(int *xyz);
void hscd_activate(int flgatm, int flg, int dtime);
int hscd_open();

void bma222_activate(int flgatm, int flg, int dtime);
int bma222_get_acceleration_data(int *xyz);
int bma222_open();

void bma222e_activate(int flgatm, int flg, int dtime);
int bma222e_get_acceleration_data(int *xyz);
int bma222e_open();

int hscd_self_test_A(void);
int hscd_self_test_B(void);

extern int sensors_register(struct device *dev,
	void *drvdata, struct device_attribute *attributes[],
	char *name);

#endif
