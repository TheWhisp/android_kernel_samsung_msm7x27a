#include <asm/div64.h>
#include <linux/reciprocal_div.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master

u32 reciprocal_value(u32 k)
{
	u64 val = (1LL << 32) + (k - 1);
	do_div(val, k);
	return (u32)val;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
EXPORT_SYMBOL(reciprocal_value);
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL(reciprocal_value);
>>>>>>> refs/remotes/origin/master
