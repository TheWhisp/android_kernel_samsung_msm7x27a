#ifndef _LINUX_XGIFB
#define _LINUX_XGIFB
<<<<<<< HEAD
#include <linux/ioctl.h>
#include <linux/types.h>

<<<<<<< HEAD
#define DISPTYPE_CRT1       0x00000008L
#define DISPTYPE_CRT2       0x00000004L
#define DISPTYPE_LCD        0x00000002L
#define DISPTYPE_TV         0x00000001L
#define DISPTYPE_DISP1      DISPTYPE_CRT1
#define DISPTYPE_DISP2      (DISPTYPE_CRT2 | DISPTYPE_LCD | DISPTYPE_TV)
#define DISPMODE_SINGLE	    0x00000020L
#define DISPMODE_MIRROR	    0x00000010L
#define DISPMODE_DUALVIEW   0x00000040L
=======
=======
>>>>>>> refs/remotes/origin/master
#include "vgatypes.h"
#include "vb_struct.h"

enum xgifb_display_type {
	XGIFB_DISP_NONE = 0,
	XGIFB_DISP_CRT,
	XGIFB_DISP_LCD,
	XGIFB_DISP_TV,
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#define HASVB_NONE	    0x00
#define HASVB_301	    0x01
#define HASVB_LVDS	    0x02
#define HASVB_TRUMPION	    0x04
#define HASVB_LVDS_CHRONTEL 0x10
#define HASVB_302	    0x20
<<<<<<< HEAD
<<<<<<< HEAD
#define HASVB_303	    0x40
#define HASVB_CHRONTEL	    0x80

#ifndef XGIFB_ID
#define XGIFB_ID	0x53495346 /* Identify myself with 'XGIF' */
#endif

=======
#define HASVB_CHRONTEL	    0x80

>>>>>>> refs/remotes/origin/cm-10.0
enum XGI_CHIP_TYPE {
	XG40 = 32,
	XG41,
	XG42,
	XG45,
=======
#define HASVB_CHRONTEL	    0x80

enum XGI_CHIP_TYPE {
	XG40 = 32,
	XG42,
>>>>>>> refs/remotes/origin/master
	XG20 = 48,
	XG21,
	XG27,
};

enum xgi_tvtype {
	TVMODE_NTSC = 0,
	TVMODE_PAL,
	TVMODE_HIVISION,
<<<<<<< HEAD
	TVTYPE_PALM,	/* vicki@030226 */
	TVTYPE_PALN,	/* vicki@030226 */
	TVTYPE_NTSCJ,	/* vicki@030226 */
	TVMODE_TOTAL
};

enum xgi_tv_plug { /* vicki@030226 */
<<<<<<< HEAD
/*	TVPLUG_Legacy = 0, */
/*	TVPLUG_COMPOSITE,  */
/*	TVPLUG_SVIDEO,	   */
/*	TVPLUG_SCART,	   */
/*	TVPLUG_TOTAL	   */
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	TVTYPE_PALM,
	TVTYPE_PALN,
	TVTYPE_NTSCJ,
	TVMODE_TOTAL
};

enum xgi_tv_plug {
>>>>>>> refs/remotes/origin/master
	TVPLUG_UNKNOWN = 0,
	TVPLUG_COMPOSITE = 1,
	TVPLUG_SVIDEO = 2,
	TVPLUG_COMPOSITE_AND_SVIDEO = 3,
	TVPLUG_SCART = 4,
	TVPLUG_YPBPR_525i = 5,
	TVPLUG_YPBPR_525P = 6,
	TVPLUG_YPBPR_750P = 7,
	TVPLUG_YPBPR_1080i = 8,
	TVPLUG_TOTAL
};

<<<<<<< HEAD
<<<<<<< HEAD
struct video_info {
	int           chip_id;
	unsigned int  video_size;
	unsigned long video_base;
	char	      *video_vbase;
	unsigned long mmio_base;
	unsigned long mmio_size;
	char	      *mmio_vbase;
=======
=======
>>>>>>> refs/remotes/origin/master
struct xgifb_video_info {
	struct fb_info *fb_info;
	struct xgi_hw_device_info hw_info;
	struct vb_device_info dev_info;

	int mode_idx;
	int rate_idx;

	u32 pseudo_palette[17];

	int           chip_id;
	unsigned int  video_size;
<<<<<<< HEAD
	unsigned long video_base;
	void __iomem *video_vbase;
	unsigned long mmio_base;
	unsigned long mmio_size;
	void __iomem *mmio_vbase;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long vga_base;
	unsigned long mtrr;
=======
	phys_addr_t   video_base;
	void __iomem *video_vbase;
	phys_addr_t   mmio_base;
	unsigned long mmio_size;
	void __iomem *mmio_vbase;
	unsigned long vga_base;
	int mtrr;
>>>>>>> refs/remotes/origin/master

	int    video_bpp;
	int    video_cmap_len;
	int    video_width;
	int    video_height;
	int    video_vwidth;
	int    video_vheight;
	int    org_x;
	int    org_y;
	int    video_linelength;
	unsigned int refresh_rate;

<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long disp_state;
=======
	enum xgifb_display_type display2; /* the second display output type */
	bool display2_force;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	enum xgifb_display_type display2; /* the second display output type */
	bool display2_force;
>>>>>>> refs/remotes/origin/master
	unsigned char hasVB;
	unsigned char TV_type;
	unsigned char TV_plug;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct XGI21_LVDSCapStruct lvds_data;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct XGI21_LVDSCapStruct lvds_data;

>>>>>>> refs/remotes/origin/master
	enum XGI_CHIP_TYPE chip;
	unsigned char revision_id;

	unsigned short DstColor;
	unsigned long  XGI310_AccelDepth;
	unsigned long  CommandReg;

	unsigned int   pcibus;
	unsigned int   pcislot;
	unsigned int   pcifunc;

	unsigned short subsysvendor;
	unsigned short subsysdevice;

	char reserved[236];
};

<<<<<<< HEAD
<<<<<<< HEAD

extern struct video_info xgi_video_info;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif
