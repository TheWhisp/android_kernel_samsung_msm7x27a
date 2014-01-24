#ifndef CEPH_CRUSH_MAPPER_H
#define CEPH_CRUSH_MAPPER_H

/*
 * CRUSH functions for find rules and then mapping an input to an
 * output set.
 *
 * LGPL2
 */

<<<<<<< HEAD
#include "crush.h"

<<<<<<< HEAD
<<<<<<< HEAD
extern int crush_find_rule(struct crush_map *map, int pool, int type, int size);
extern int crush_do_rule(struct crush_map *map,
			 int ruleno,
			 int x, int *result, int result_max,
			 int forcefeed,    /* -1 for none */
			 __u32 *weights);
=======
=======
#include <linux/crush/crush.h>

>>>>>>> refs/remotes/origin/master
extern int crush_find_rule(const struct crush_map *map, int ruleset, int type, int size);
extern int crush_do_rule(const struct crush_map *map,
			 int ruleno,
			 int x, int *result, int result_max,
<<<<<<< HEAD
			 int forcefeed,    /* -1 for none */
			 const __u32 *weights);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			 const __u32 *weights);
>>>>>>> refs/remotes/origin/master
=======
extern int crush_find_rule(const struct crush_map *map, int ruleset, int type, int size);
extern int crush_do_rule(const struct crush_map *map,
			 int ruleno,
			 int x, int *result, int result_max,
			 int forcefeed,    /* -1 for none */
			 const __u32 *weights);
>>>>>>> refs/remotes/origin/cm-11.0

#endif
