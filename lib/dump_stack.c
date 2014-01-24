/*
 * Provide a default dump_stack() function for architectures
 * which don't implement their own.
 */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0

void dump_stack(void)
{
	printk(KERN_NOTICE
		"This architecture does not implement dump_stack()\n");
}

EXPORT_SYMBOL(dump_stack);
