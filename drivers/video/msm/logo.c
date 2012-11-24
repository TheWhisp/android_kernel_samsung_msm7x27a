/* drivers/video/msm/logo.c
 *
 * Show Logo in RLE 565 format
 *
 * Copyright (C) 2008 Google Incorporated
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fb.h>
#include <linux/vt_kern.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>

#include <linux/irq.h>
#include <asm/system.h>
#include <asm/cacheflush.h>

#define fb_width(fb)	((fb)->var.xres)
#define fb_height(fb)	((fb)->var.yres)
#define fb_size(fb)	((fb)->var.xres * (fb)->var.yres * 2)

static void memset16(void *_ptr, unsigned short val, unsigned count)
{
	unsigned short *ptr = _ptr;
	count >>= 1;
	while (count--)
		*ptr++ = val;
}

/* convert RGB565 to RBG8888 */
static void memset16_rgb8888(void *_ptr, unsigned short val, unsigned count)
{
	unsigned short *ptr = _ptr;
	unsigned short red;
	unsigned short green;
	unsigned short blue;

	red = ( val & 0xF800) >> 8;
	green = (val & 0x7E0) >> 3;
	blue = (val & 0x1F) << 3;

	count >>= 1;
	while (count--) {
		*ptr++ = (red<<8) | green;
		*ptr++ = blue;
	}
}

/* 565RLE image format: [count(2 bytes), rle(2 bytes)] */
int load_565rle_image(char *filename)
{
	struct fb_info *info;
	int fd, count, err = 0;
	unsigned max;
	unsigned short *data, *bits, *ptr;
#ifndef CONFIG_FRAMEBUFFER_CONSOLE
	struct module *owner;
#endif

	info = registered_fb[0];
	if (!info) {
		printk(KERN_WARNING "%s: Can not access framebuffer\n",
			__func__);
		return -ENODEV;
	}

#ifndef CONFIG_FRAMEBUFFER_CONSOLE
	owner = info->fbops->owner;
	if (!try_module_get(owner))
		return -ENODEV;
	if (info->fbops->fb_open && info->fbops->fb_open(info, 0)) {
		module_put(owner);
		return -ENODEV;
	}
#endif

	fd = sys_open(filename, O_RDONLY, 0);
	if (fd < 0) {
		printk(KERN_WARNING "%s: Can not open %s\n",
			__func__, filename);
		return -ENOENT;
	}
	count = sys_lseek(fd, (off_t)0, 2);
	if (count <= 0) {
		err = -EIO;
		goto err_logo_close_file;
	}
	sys_lseek(fd, (off_t)0, 0);
	data = kmalloc(count, GFP_KERNEL);
	if (!data) {
		printk(KERN_WARNING "%s: Can not alloc data\n", __func__);
		err = -ENOMEM;
		goto err_logo_close_file;
	}
	if (sys_read(fd, (char *)data, count) != count) {
		err = -EIO;
		goto err_logo_free_data;
	}

	max = fb_width(info) * fb_height(info);
	ptr = data;
	bits = (unsigned short *)(info->screen_base);
	while (count > 3) {
		unsigned n = ptr[0];
		if (n > max)
			break;
		if (info->var.bits_per_pixel >= 24) { /* rgb888 */
			memset16_rgb8888(bits, ptr[1], n << 1);
			bits += n * 2;
		} else {
			memset16(bits, ptr[1], n << 1);
			bits += n;
		}
		max -= n;
		ptr += 2;
		count -= 4;
	}

	flush_cache_all();
	outer_flush_all();

err_logo_free_data:
	kfree(data);
err_logo_close_file:
	sys_close(fd);

	return err;
}
EXPORT_SYMBOL(load_565rle_image);

int draw_rgb888_screen(void)
{
	struct fb_info *fb = registered_fb[0];
	u32 height = fb->var.yres / 5;
	u32 line = fb->fix.line_length;
	u32 i, j;

	for (i = 0; i < height; i++) {
		for (j = 0; j < fb->var.xres; j++) {
			memset(fb->screen_base + i * line + j * 4 + 0, 0xff, 1);
			memset(fb->screen_base + i * line + j * 4 + 1, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 2, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 3, 0x00, 1);
		}
	}

	for (i = height; i < height * 2; i++) {
		for (j = 0; j < fb->var.xres; j++) {
			memset(fb->screen_base + i * line + j * 4 + 0, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 1, 0xff, 1);
			memset(fb->screen_base + i * line + j * 4 + 2, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 3, 0x00, 1);
		}
	}

	for (i = height * 2; i < height * 3; i++) {
		for (j = 0; j < fb->var.xres; j++) {
			memset(fb->screen_base + i * line + j * 4 + 0, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 1, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 2, 0xff, 1);
			memset(fb->screen_base + i * line + j * 4 + 3, 0x00, 1);
		}
	}

	for (i = height * 3; i < height * 4; i++) {
		for (j = 0; j < fb->var.xres; j++) {
			memset(fb->screen_base + i * line + j * 4 + 0, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 1, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 2, 0x00, 1);
			memset(fb->screen_base + i * line + j * 4 + 3, 0xff, 1);
		}
	}

	for (i = height * 4; i < height * 5; i++) {
		for (j = 0; j < fb->var.xres; j++) {
			memset(fb->screen_base + i * line + j * 4 + 0, 0xff, 1);
			memset(fb->screen_base + i * line + j * 4 + 1, 0xff, 1);
			memset(fb->screen_base + i * line + j * 4 + 2, 0xff, 1);
			memset(fb->screen_base + i * line + j * 4 + 3, 0x00, 1);
		}
	}

	return 0;
}
EXPORT_SYMBOL(draw_rgb888_screen);

int draw_rgb888_black_screen(void)
{
	struct fb_info *fb = registered_fb[0];
	u32 height = fb->var.yres;
	u32 line = fb->fix.line_length;
	u32 i, j;
#ifndef CONFIG_FRAMEBUFFER_CONSOLE
	struct module *owner;
#endif

	if (!fb) {
		printk(KERN_WARNING "%s: Can not access framebuffer\n",
			__func__);
		return -ENODEV;
	}

#ifndef CONFIG_FRAMEBUFFER_CONSOLE
	owner = fb->fbops->owner;
	if (!try_module_get(owner))
		return -ENODEV;
	if (fb->fbops->fb_open && fb->fbops->fb_open(fb, 0)) {
		module_put(owner);
		return -ENODEV;
	}
#endif

	for (i = 0; i < height ; i++) {
		for (j = 0; j < fb->var.xres; j++)
			memset(fb->screen_base + i * line + j * 4 + 0,
				0xff000000, 4);
	}
	flush_cache_all();
	outer_flush_all();

	return 0;
}
EXPORT_SYMBOL(draw_rgb888_black_screen);
