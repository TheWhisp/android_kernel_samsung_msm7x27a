/* $Id divert_init.c,v 1.5.6.2 2001/01/24 22:18:17 kai Exp $
 *
 * Module init for DSS1 diversion services for i4l.
 *
 * Copyright 1999       by Werner Cornelius (werner@isdn4linux.de)
<<<<<<< HEAD
 * 
=======
 *
>>>>>>> refs/remotes/origin/cm-10.0
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include "isdn_divert.h"

MODULE_DESCRIPTION("ISDN4Linux: Call diversion support");
MODULE_AUTHOR("Werner Cornelius");
MODULE_LICENSE("GPL");

/****************************************/
/* structure containing interface to hl */
/****************************************/
isdn_divert_if divert_if =
<<<<<<< HEAD
  { DIVERT_IF_MAGIC,  /* magic value */
    DIVERT_CMD_REG,   /* register cmd */
    ll_callback,      /* callback routine from ll */
    NULL,             /* command still not specified */
    NULL,             /* drv_to_name */
    NULL,             /* name_to_drv */
  };
=======
{ DIVERT_IF_MAGIC,  /* magic value */
  DIVERT_CMD_REG,   /* register cmd */
  ll_callback,      /* callback routine from ll */
  NULL,             /* command still not specified */
  NULL,             /* drv_to_name */
  NULL,             /* name_to_drv */
};
>>>>>>> refs/remotes/origin/cm-10.0

/*************************/
/* Module interface code */
/* no cmd line parms     */
/*************************/
static int __init divert_init(void)
{ int i;

<<<<<<< HEAD
  if (divert_dev_init())
   { printk(KERN_WARNING "dss1_divert: cannot install device, not loaded\n");
     return(-EIO);
   }
  if ((i = DIVERT_REG_NAME(&divert_if)) != DIVERT_NO_ERR)
   { divert_dev_deinit();
     printk(KERN_WARNING "dss1_divert: error %d registering module, not loaded\n",i);
     return(-EIO);
   } 
  printk(KERN_INFO "dss1_divert module successfully installed\n");
  return(0);
=======
	if (divert_dev_init())
	{ printk(KERN_WARNING "dss1_divert: cannot install device, not loaded\n");
		return (-EIO);
	}
	if ((i = DIVERT_REG_NAME(&divert_if)) != DIVERT_NO_ERR)
	{ divert_dev_deinit();
		printk(KERN_WARNING "dss1_divert: error %d registering module, not loaded\n", i);
		return (-EIO);
	}
	printk(KERN_INFO "dss1_divert module successfully installed\n");
	return (0);
>>>>>>> refs/remotes/origin/cm-10.0
}

/**********************/
/* Module deinit code */
/**********************/
static void __exit divert_exit(void)
{
<<<<<<< HEAD
  unsigned long flags;
  int i;

  spin_lock_irqsave(&divert_lock, flags);
  divert_if.cmd = DIVERT_CMD_REL; /* release */
  if ((i = DIVERT_REG_NAME(&divert_if)) != DIVERT_NO_ERR)
   { printk(KERN_WARNING "dss1_divert: error %d releasing module\n",i);
     spin_unlock_irqrestore(&divert_lock, flags);
     return;
   } 
  if (divert_dev_deinit()) 
   { printk(KERN_WARNING "dss1_divert: device busy, remove cancelled\n");
     spin_unlock_irqrestore(&divert_lock, flags);
     return;
   }
  spin_unlock_irqrestore(&divert_lock, flags);
  deleterule(-1); /* delete all rules and free mem */
  deleteprocs();
  printk(KERN_INFO "dss1_divert module successfully removed \n");
=======
	unsigned long flags;
	int i;

	spin_lock_irqsave(&divert_lock, flags);
	divert_if.cmd = DIVERT_CMD_REL; /* release */
	if ((i = DIVERT_REG_NAME(&divert_if)) != DIVERT_NO_ERR)
	{ printk(KERN_WARNING "dss1_divert: error %d releasing module\n", i);
		spin_unlock_irqrestore(&divert_lock, flags);
		return;
	}
	if (divert_dev_deinit())
	{ printk(KERN_WARNING "dss1_divert: device busy, remove cancelled\n");
		spin_unlock_irqrestore(&divert_lock, flags);
		return;
	}
	spin_unlock_irqrestore(&divert_lock, flags);
	deleterule(-1); /* delete all rules and free mem */
	deleteprocs();
	printk(KERN_INFO "dss1_divert module successfully removed \n");
>>>>>>> refs/remotes/origin/cm-10.0
}

module_init(divert_init);
module_exit(divert_exit);
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
