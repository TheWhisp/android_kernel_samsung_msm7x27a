#ifndef __PLAT_AUDIO_H
#define __PLAT_AUDIO_H

<<<<<<< HEAD
#include <linux/mbus.h>

struct kirkwood_asoc_platform_data {
	u32 tclk;
	struct mbus_dram_target_info *dram;
=======
struct kirkwood_asoc_platform_data {
>>>>>>> refs/remotes/origin/cm-10.0
	int burst;
};
#endif
