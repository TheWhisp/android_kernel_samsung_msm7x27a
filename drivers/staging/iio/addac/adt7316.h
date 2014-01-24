/*
 * ADT7316 digital temperature sensor driver supporting ADT7316/7/8 ADT7516/7/9
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _ADT7316_H_
#define _ADT7316_H_

#include <linux/types.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/pm.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/pm.h>
>>>>>>> refs/remotes/origin/master

#define ADT7316_REG_MAX_ADDR		0x3F

struct adt7316_bus {
	void *client;
	int irq;
	int irq_flags;
	int (*read) (void *client, u8 reg, u8 *data);
	int (*write) (void *client, u8 reg, u8 val);
	int (*multi_read) (void *client, u8 first_reg, u8 count, u8 *data);
	int (*multi_write) (void *client, u8 first_reg, u8 count, u8 *data);
};

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_PM
int adt7316_disable(struct device *dev);
int adt7316_enable(struct device *dev);
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PM_SLEEP
extern const struct dev_pm_ops adt7316_pm_ops;
#define ADT7316_PM_OPS (&adt7316_pm_ops)
#else
#define ADT7316_PM_OPS NULL
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#endif
int adt7316_probe(struct device *dev, struct adt7316_bus *bus, const char *name);
int adt7316_remove(struct device *dev);
=======
#endif
int adt7316_probe(struct device *dev, struct adt7316_bus *bus, const char *name);
>>>>>>> refs/remotes/origin/master

#endif
