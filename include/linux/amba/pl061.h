#include <linux/types.h>

/* platform data for the PL061 GPIO driver */

struct pl061_platform_data {
	/* number of the first GPIO */
	unsigned	gpio_base;

	/* number of the first IRQ.
<<<<<<< HEAD
<<<<<<< HEAD
	 * If the IRQ functionality in not desired this must be set to
	 * (unsigned) -1.
=======
	 * If the IRQ functionality in not desired this must be set to 0.
>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * If the IRQ functionality in not desired this must be set to 0.
>>>>>>> refs/remotes/origin/master
	 */
	unsigned	irq_base;

	u8		directions;	/* startup directions, 1: out, 0: in */
	u8		values;		/* startup values */
};
