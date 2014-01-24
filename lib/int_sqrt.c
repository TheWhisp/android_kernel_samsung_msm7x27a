<<<<<<< HEAD

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * Copyright (C) 2013 Davidlohr Bueso <davidlohr.bueso@hp.com>
 *
 *  Based on the shift-and-subtract algorithm for computing integer
 *  square root from Guy L. Steele.
 */

#include <linux/kernel.h>
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master

/**
 * int_sqrt - rough approximation to sqrt
 * @x: integer of which to calculate the sqrt
 *
 * A very rough approximation to the sqrt() function.
 */
unsigned long int_sqrt(unsigned long x)
{
<<<<<<< HEAD
	unsigned long op, res, one;

	op = x;
	res = 0;

	one = 1UL << (BITS_PER_LONG - 2);
	while (one > op)
		one >>= 2;

	while (one != 0) {
		if (op >= res + one) {
			op = op - (res + one);
			res = res +  2 * one;
		}
		res /= 2;
		one /= 4;
	}
	return res;
=======
	unsigned long b, m, y = 0;

	if (x <= 1)
		return x;

	m = 1UL << (BITS_PER_LONG - 2);
	while (m != 0) {
		b = y + m;
		y >>= 1;

		if (x >= b) {
			x -= b;
			y += m;
		}
		m >>= 2;
	}

	return y;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(int_sqrt);
