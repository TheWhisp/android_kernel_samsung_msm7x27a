#ifndef _MEDIA_MT9V032_H
#define _MEDIA_MT9V032_H

<<<<<<< HEAD
struct v4l2_subdev;

struct mt9v032_platform_data {
	unsigned int clk_pol:1;

	void (*set_clock)(struct v4l2_subdev *subdev, unsigned int rate);
=======
struct mt9v032_platform_data {
	unsigned int clk_pol:1;

	const s64 *link_freqs;
	s64 link_def_freq;
>>>>>>> refs/remotes/origin/master
};

#endif
