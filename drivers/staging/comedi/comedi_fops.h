
#ifndef _COMEDI_FOPS_H
#define _COMEDI_FOPS_H
<<<<<<< HEAD

extern struct class *comedi_class;
extern const struct file_operations comedi_fops;
extern int comedi_autoconfig;
=======
#include <linux/types.h>

extern struct class *comedi_class;
extern const struct file_operations comedi_fops;
extern bool comedi_autoconfig;
>>>>>>> refs/remotes/origin/cm-10.0
extern struct comedi_driver *comedi_drivers;

#endif /* _COMEDI_FOPS_H */
