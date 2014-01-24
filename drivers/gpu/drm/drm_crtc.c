/*
 * Copyright (c) 2006-2008 Intel Corporation
 * Copyright (c) 2007 Dave Airlie <airlied@linux.ie>
 * Copyright (c) 2008 Red Hat Inc.
 *
 * DRM core CRTC related functions
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 * Authors:
 *      Keith Packard
 *	Eric Anholt <eric@anholt.net>
 *      Dave Airlie <airlied@linux.ie>
 *      Jesse Barnes <jesse.barnes@intel.com>
 */
<<<<<<< HEAD
#include <linux/list.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "drm.h"
#include "drmP.h"
#include "drm_crtc.h"
#include "drm_edid.h"
<<<<<<< HEAD

struct drm_prop_enum_list {
	int type;
	char *name;
};
=======
#include "drm_fourcc.h"
>>>>>>> refs/remotes/origin/cm-10.0

/* Avoid boilerplate.  I'm tired of typing. */
#define DRM_ENUM_NAME_FN(fnname, list)				\
	char *fnname(int val)					\
=======
#include <linux/ctype.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/drm_edid.h>
#include <drm/drm_fourcc.h>

/**
 * drm_modeset_lock_all - take all modeset locks
 * @dev: drm device
 *
 * This function takes all modeset locks, suitable where a more fine-grained
 * scheme isn't (yet) implemented.
 */
void drm_modeset_lock_all(struct drm_device *dev)
{
	struct drm_crtc *crtc;

	mutex_lock(&dev->mode_config.mutex);

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head)
		mutex_lock_nest_lock(&crtc->mutex, &dev->mode_config.mutex);
}
EXPORT_SYMBOL(drm_modeset_lock_all);

/**
 * drm_modeset_unlock_all - drop all modeset locks
 * @dev: device
 */
void drm_modeset_unlock_all(struct drm_device *dev)
{
	struct drm_crtc *crtc;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head)
		mutex_unlock(&crtc->mutex);

	mutex_unlock(&dev->mode_config.mutex);
}
EXPORT_SYMBOL(drm_modeset_unlock_all);

/**
 * drm_warn_on_modeset_not_all_locked - check that all modeset locks are locked
 * @dev: device
 */
void drm_warn_on_modeset_not_all_locked(struct drm_device *dev)
{
	struct drm_crtc *crtc;

	/* Locking is currently fubar in the panic handler. */
	if (oops_in_progress)
		return;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head)
		WARN_ON(!mutex_is_locked(&crtc->mutex));

	WARN_ON(!mutex_is_locked(&dev->mode_config.mutex));
}
EXPORT_SYMBOL(drm_warn_on_modeset_not_all_locked);

/* Avoid boilerplate.  I'm tired of typing. */
#define DRM_ENUM_NAME_FN(fnname, list)				\
	const char *fnname(int val)				\
>>>>>>> refs/remotes/origin/master
	{							\
		int i;						\
		for (i = 0; i < ARRAY_SIZE(list); i++) {	\
			if (list[i].type == val)		\
				return list[i].name;		\
		}						\
		return "(unknown)";				\
	}

/*
 * Global properties
 */
<<<<<<< HEAD
static struct drm_prop_enum_list drm_dpms_enum_list[] =
=======
static const struct drm_prop_enum_list drm_dpms_enum_list[] =
>>>>>>> refs/remotes/origin/master
{	{ DRM_MODE_DPMS_ON, "On" },
	{ DRM_MODE_DPMS_STANDBY, "Standby" },
	{ DRM_MODE_DPMS_SUSPEND, "Suspend" },
	{ DRM_MODE_DPMS_OFF, "Off" }
};

DRM_ENUM_NAME_FN(drm_get_dpms_name, drm_dpms_enum_list)

/*
 * Optional properties
 */
<<<<<<< HEAD
static struct drm_prop_enum_list drm_scaling_mode_enum_list[] =
=======
static const struct drm_prop_enum_list drm_scaling_mode_enum_list[] =
>>>>>>> refs/remotes/origin/master
{
	{ DRM_MODE_SCALE_NONE, "None" },
	{ DRM_MODE_SCALE_FULLSCREEN, "Full" },
	{ DRM_MODE_SCALE_CENTER, "Center" },
	{ DRM_MODE_SCALE_ASPECT, "Full aspect" },
};

<<<<<<< HEAD
static struct drm_prop_enum_list drm_dithering_mode_enum_list[] =
{
	{ DRM_MODE_DITHERING_OFF, "Off" },
	{ DRM_MODE_DITHERING_ON, "On" },
	{ DRM_MODE_DITHERING_AUTO, "Automatic" },
};

/*
 * Non-global properties, but "required" for certain connectors.
 */
static struct drm_prop_enum_list drm_dvi_i_select_enum_list[] =
=======
/*
 * Non-global properties, but "required" for certain connectors.
 */
static const struct drm_prop_enum_list drm_dvi_i_select_enum_list[] =
>>>>>>> refs/remotes/origin/master
{
	{ DRM_MODE_SUBCONNECTOR_Automatic, "Automatic" }, /* DVI-I and TV-out */
	{ DRM_MODE_SUBCONNECTOR_DVID,      "DVI-D"     }, /* DVI-I  */
	{ DRM_MODE_SUBCONNECTOR_DVIA,      "DVI-A"     }, /* DVI-I  */
};

DRM_ENUM_NAME_FN(drm_get_dvi_i_select_name, drm_dvi_i_select_enum_list)

<<<<<<< HEAD
static struct drm_prop_enum_list drm_dvi_i_subconnector_enum_list[] =
=======
static const struct drm_prop_enum_list drm_dvi_i_subconnector_enum_list[] =
>>>>>>> refs/remotes/origin/master
{
	{ DRM_MODE_SUBCONNECTOR_Unknown,   "Unknown"   }, /* DVI-I and TV-out */
	{ DRM_MODE_SUBCONNECTOR_DVID,      "DVI-D"     }, /* DVI-I  */
	{ DRM_MODE_SUBCONNECTOR_DVIA,      "DVI-A"     }, /* DVI-I  */
};

DRM_ENUM_NAME_FN(drm_get_dvi_i_subconnector_name,
		 drm_dvi_i_subconnector_enum_list)

<<<<<<< HEAD
static struct drm_prop_enum_list drm_tv_select_enum_list[] =
=======
static const struct drm_prop_enum_list drm_tv_select_enum_list[] =
>>>>>>> refs/remotes/origin/master
{
	{ DRM_MODE_SUBCONNECTOR_Automatic, "Automatic" }, /* DVI-I and TV-out */
	{ DRM_MODE_SUBCONNECTOR_Composite, "Composite" }, /* TV-out */
	{ DRM_MODE_SUBCONNECTOR_SVIDEO,    "SVIDEO"    }, /* TV-out */
	{ DRM_MODE_SUBCONNECTOR_Component, "Component" }, /* TV-out */
	{ DRM_MODE_SUBCONNECTOR_SCART,     "SCART"     }, /* TV-out */
};

DRM_ENUM_NAME_FN(drm_get_tv_select_name, drm_tv_select_enum_list)

<<<<<<< HEAD
static struct drm_prop_enum_list drm_tv_subconnector_enum_list[] =
=======
static const struct drm_prop_enum_list drm_tv_subconnector_enum_list[] =
>>>>>>> refs/remotes/origin/master
{
	{ DRM_MODE_SUBCONNECTOR_Unknown,   "Unknown"   }, /* DVI-I and TV-out */
	{ DRM_MODE_SUBCONNECTOR_Composite, "Composite" }, /* TV-out */
	{ DRM_MODE_SUBCONNECTOR_SVIDEO,    "SVIDEO"    }, /* TV-out */
	{ DRM_MODE_SUBCONNECTOR_Component, "Component" }, /* TV-out */
	{ DRM_MODE_SUBCONNECTOR_SCART,     "SCART"     }, /* TV-out */
};

DRM_ENUM_NAME_FN(drm_get_tv_subconnector_name,
		 drm_tv_subconnector_enum_list)

<<<<<<< HEAD
static struct drm_prop_enum_list drm_dirty_info_enum_list[] = {
=======
static const struct drm_prop_enum_list drm_dirty_info_enum_list[] = {
>>>>>>> refs/remotes/origin/master
	{ DRM_MODE_DIRTY_OFF,      "Off"      },
	{ DRM_MODE_DIRTY_ON,       "On"       },
	{ DRM_MODE_DIRTY_ANNOTATE, "Annotate" },
};

<<<<<<< HEAD
DRM_ENUM_NAME_FN(drm_get_dirty_info_name,
		 drm_dirty_info_enum_list)

struct drm_conn_prop_enum_list {
	int type;
	char *name;
	int count;
=======
struct drm_conn_prop_enum_list {
	int type;
	const char *name;
	struct ida ida;
>>>>>>> refs/remotes/origin/master
};

/*
 * Connector and encoder types.
 */
static struct drm_conn_prop_enum_list drm_connector_enum_list[] =
<<<<<<< HEAD
{	{ DRM_MODE_CONNECTOR_Unknown, "Unknown", 0 },
	{ DRM_MODE_CONNECTOR_VGA, "VGA", 0 },
	{ DRM_MODE_CONNECTOR_DVII, "DVI-I", 0 },
	{ DRM_MODE_CONNECTOR_DVID, "DVI-D", 0 },
	{ DRM_MODE_CONNECTOR_DVIA, "DVI-A", 0 },
	{ DRM_MODE_CONNECTOR_Composite, "Composite", 0 },
	{ DRM_MODE_CONNECTOR_SVIDEO, "SVIDEO", 0 },
	{ DRM_MODE_CONNECTOR_LVDS, "LVDS", 0 },
	{ DRM_MODE_CONNECTOR_Component, "Component", 0 },
	{ DRM_MODE_CONNECTOR_9PinDIN, "DIN", 0 },
	{ DRM_MODE_CONNECTOR_DisplayPort, "DP", 0 },
	{ DRM_MODE_CONNECTOR_HDMIA, "HDMI-A", 0 },
	{ DRM_MODE_CONNECTOR_HDMIB, "HDMI-B", 0 },
	{ DRM_MODE_CONNECTOR_TV, "TV", 0 },
	{ DRM_MODE_CONNECTOR_eDP, "eDP", 0 },
<<<<<<< HEAD
=======
	{ DRM_MODE_CONNECTOR_VIRTUAL, "Virtual", 0},
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct drm_prop_enum_list drm_encoder_enum_list[] =
=======
{	{ DRM_MODE_CONNECTOR_Unknown, "Unknown" },
	{ DRM_MODE_CONNECTOR_VGA, "VGA" },
	{ DRM_MODE_CONNECTOR_DVII, "DVI-I" },
	{ DRM_MODE_CONNECTOR_DVID, "DVI-D" },
	{ DRM_MODE_CONNECTOR_DVIA, "DVI-A" },
	{ DRM_MODE_CONNECTOR_Composite, "Composite" },
	{ DRM_MODE_CONNECTOR_SVIDEO, "SVIDEO" },
	{ DRM_MODE_CONNECTOR_LVDS, "LVDS" },
	{ DRM_MODE_CONNECTOR_Component, "Component" },
	{ DRM_MODE_CONNECTOR_9PinDIN, "DIN" },
	{ DRM_MODE_CONNECTOR_DisplayPort, "DP" },
	{ DRM_MODE_CONNECTOR_HDMIA, "HDMI-A" },
	{ DRM_MODE_CONNECTOR_HDMIB, "HDMI-B" },
	{ DRM_MODE_CONNECTOR_TV, "TV" },
	{ DRM_MODE_CONNECTOR_eDP, "eDP" },
	{ DRM_MODE_CONNECTOR_VIRTUAL, "Virtual" },
	{ DRM_MODE_CONNECTOR_DSI, "DSI" },
};

static const struct drm_prop_enum_list drm_encoder_enum_list[] =
>>>>>>> refs/remotes/origin/master
{	{ DRM_MODE_ENCODER_NONE, "None" },
	{ DRM_MODE_ENCODER_DAC, "DAC" },
	{ DRM_MODE_ENCODER_TMDS, "TMDS" },
	{ DRM_MODE_ENCODER_LVDS, "LVDS" },
	{ DRM_MODE_ENCODER_TVDAC, "TV" },
<<<<<<< HEAD
<<<<<<< HEAD
=======
	{ DRM_MODE_ENCODER_VIRTUAL, "Virtual" },
>>>>>>> refs/remotes/origin/cm-10.0
};

char *drm_get_encoder_name(struct drm_encoder *encoder)
=======
	{ DRM_MODE_ENCODER_VIRTUAL, "Virtual" },
	{ DRM_MODE_ENCODER_DSI, "DSI" },
};

void drm_connector_ida_init(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(drm_connector_enum_list); i++)
		ida_init(&drm_connector_enum_list[i].ida);
}

void drm_connector_ida_destroy(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(drm_connector_enum_list); i++)
		ida_destroy(&drm_connector_enum_list[i].ida);
}

const char *drm_get_encoder_name(const struct drm_encoder *encoder)
>>>>>>> refs/remotes/origin/master
{
	static char buf[32];

	snprintf(buf, 32, "%s-%d",
		 drm_encoder_enum_list[encoder->encoder_type].name,
		 encoder->base.id);
	return buf;
}
EXPORT_SYMBOL(drm_get_encoder_name);

<<<<<<< HEAD
char *drm_get_connector_name(struct drm_connector *connector)
=======
const char *drm_get_connector_name(const struct drm_connector *connector)
>>>>>>> refs/remotes/origin/master
{
	static char buf[32];

	snprintf(buf, 32, "%s-%d",
		 drm_connector_enum_list[connector->connector_type].name,
		 connector->connector_type_id);
	return buf;
}
EXPORT_SYMBOL(drm_get_connector_name);

<<<<<<< HEAD
char *drm_get_connector_status_name(enum drm_connector_status status)
=======
const char *drm_get_connector_status_name(enum drm_connector_status status)
>>>>>>> refs/remotes/origin/master
{
	if (status == connector_status_connected)
		return "connected";
	else if (status == connector_status_disconnected)
		return "disconnected";
	else
		return "unknown";
}
<<<<<<< HEAD

/**
 * drm_mode_object_get - allocate a new identifier
 * @dev: DRM device
 * @ptr: object pointer, used to generate unique ID
 * @type: object type
 *
 * LOCKING:
=======
EXPORT_SYMBOL(drm_get_connector_status_name);

static char printable_char(int c)
{
	return isascii(c) && isprint(c) ? c : '?';
}

const char *drm_get_format_name(uint32_t format)
{
	static char buf[32];

	snprintf(buf, sizeof(buf),
		 "%c%c%c%c %s-endian (0x%08x)",
		 printable_char(format & 0xff),
		 printable_char((format >> 8) & 0xff),
		 printable_char((format >> 16) & 0xff),
		 printable_char((format >> 24) & 0x7f),
		 format & DRM_FORMAT_BIG_ENDIAN ? "big" : "little",
		 format);

	return buf;
}
EXPORT_SYMBOL(drm_get_format_name);

/**
 * drm_mode_object_get - allocate a new modeset identifier
 * @dev: DRM device
 * @obj: object pointer, used to generate unique ID
 * @obj_type: object type
>>>>>>> refs/remotes/origin/master
 *
 * Create a unique identifier based on @ptr in @dev's identifier space.  Used
 * for tracking modes, CRTCs and connectors.
 *
 * RETURNS:
 * New unique (relative to other objects in @dev) integer identifier for the
 * object.
 */
static int drm_mode_object_get(struct drm_device *dev,
			       struct drm_mode_object *obj, uint32_t obj_type)
{
<<<<<<< HEAD
	int new_id = 0;
	int ret;

again:
	if (idr_pre_get(&dev->mode_config.crtc_idr, GFP_KERNEL) == 0) {
		DRM_ERROR("Ran out memory getting a mode number\n");
		return -EINVAL;
	}

	mutex_lock(&dev->mode_config.idr_mutex);
	ret = idr_get_new_above(&dev->mode_config.crtc_idr, obj, 1, &new_id);
	mutex_unlock(&dev->mode_config.idr_mutex);
	if (ret == -EAGAIN)
		goto again;

	obj->id = new_id;
	obj->type = obj_type;
	return 0;
}

/**
 * drm_mode_object_put - free an identifer
 * @dev: DRM device
 * @id: ID to free
 *
 * LOCKING:
 * Caller must hold DRM mode_config lock.
=======
	int ret;

	mutex_lock(&dev->mode_config.idr_mutex);
	ret = idr_alloc(&dev->mode_config.crtc_idr, obj, 1, 0, GFP_KERNEL);
	if (ret >= 0) {
		/*
		 * Set up the object linking under the protection of the idr
		 * lock so that other users can't see inconsistent state.
		 */
		obj->id = ret;
		obj->type = obj_type;
	}
	mutex_unlock(&dev->mode_config.idr_mutex);

	return ret < 0 ? ret : 0;
}

/**
 * drm_mode_object_put - free a modeset identifer
 * @dev: DRM device
 * @object: object to free
>>>>>>> refs/remotes/origin/master
 *
 * Free @id from @dev's unique identifier pool.
 */
static void drm_mode_object_put(struct drm_device *dev,
				struct drm_mode_object *object)
{
	mutex_lock(&dev->mode_config.idr_mutex);
	idr_remove(&dev->mode_config.crtc_idr, object->id);
	mutex_unlock(&dev->mode_config.idr_mutex);
}

<<<<<<< HEAD
=======
/**
 * drm_mode_object_find - look up a drm object with static lifetime
 * @dev: drm device
 * @id: id of the mode object
 * @type: type of the mode object
 *
 * Note that framebuffers cannot be looked up with this functions - since those
 * are reference counted, they need special treatment.
 */
>>>>>>> refs/remotes/origin/master
struct drm_mode_object *drm_mode_object_find(struct drm_device *dev,
		uint32_t id, uint32_t type)
{
	struct drm_mode_object *obj = NULL;

<<<<<<< HEAD
=======
	/* Framebuffers are reference counted and need their own lookup
	 * function.*/
	WARN_ON(type == DRM_MODE_OBJECT_FB);

>>>>>>> refs/remotes/origin/master
	mutex_lock(&dev->mode_config.idr_mutex);
	obj = idr_find(&dev->mode_config.crtc_idr, id);
	if (!obj || (obj->type != type) || (obj->id != id))
		obj = NULL;
	mutex_unlock(&dev->mode_config.idr_mutex);

	return obj;
}
EXPORT_SYMBOL(drm_mode_object_find);

/**
 * drm_framebuffer_init - initialize a framebuffer
 * @dev: DRM device
<<<<<<< HEAD
 *
 * LOCKING:
 * Caller must hold mode config lock.
=======
 * @fb: framebuffer to be initialized
 * @funcs: ... with these functions
>>>>>>> refs/remotes/origin/master
 *
 * Allocates an ID for the framebuffer's parent mode object, sets its mode
 * functions & device file and adds it to the master fd list.
 *
<<<<<<< HEAD
=======
 * IMPORTANT:
 * This functions publishes the fb and makes it available for concurrent access
 * by other users. Which means by this point the fb _must_ be fully set up -
 * since all the fb attributes are invariant over its lifetime, no further
 * locking but only correct reference counting is required.
 *
>>>>>>> refs/remotes/origin/master
 * RETURNS:
 * Zero on success, error code on failure.
 */
int drm_framebuffer_init(struct drm_device *dev, struct drm_framebuffer *fb,
			 const struct drm_framebuffer_funcs *funcs)
{
	int ret;

<<<<<<< HEAD
	ret = drm_mode_object_get(dev, &fb->base, DRM_MODE_OBJECT_FB);
<<<<<<< HEAD
	if (ret) {
		return ret;
	}
=======
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/cm-10.0

	fb->dev = dev;
	fb->funcs = funcs;
	dev->mode_config.num_fb++;
	list_add(&fb->head, &dev->mode_config.fb_list);
=======
	mutex_lock(&dev->mode_config.fb_lock);
	kref_init(&fb->refcount);
	INIT_LIST_HEAD(&fb->filp_head);
	fb->dev = dev;
	fb->funcs = funcs;

	ret = drm_mode_object_get(dev, &fb->base, DRM_MODE_OBJECT_FB);
	if (ret)
		goto out;

	/* Grab the idr reference. */
	drm_framebuffer_reference(fb);

	dev->mode_config.num_fb++;
	list_add(&fb->head, &dev->mode_config.fb_list);
out:
	mutex_unlock(&dev->mode_config.fb_lock);
>>>>>>> refs/remotes/origin/master

	return 0;
}
EXPORT_SYMBOL(drm_framebuffer_init);

<<<<<<< HEAD
=======
static void drm_framebuffer_free(struct kref *kref)
{
	struct drm_framebuffer *fb =
			container_of(kref, struct drm_framebuffer, refcount);
	fb->funcs->destroy(fb);
}

static struct drm_framebuffer *__drm_framebuffer_lookup(struct drm_device *dev,
							uint32_t id)
{
	struct drm_mode_object *obj = NULL;
	struct drm_framebuffer *fb;

	mutex_lock(&dev->mode_config.idr_mutex);
	obj = idr_find(&dev->mode_config.crtc_idr, id);
	if (!obj || (obj->type != DRM_MODE_OBJECT_FB) || (obj->id != id))
		fb = NULL;
	else
		fb = obj_to_fb(obj);
	mutex_unlock(&dev->mode_config.idr_mutex);

	return fb;
}

/**
 * drm_framebuffer_lookup - look up a drm framebuffer and grab a reference
 * @dev: drm device
 * @id: id of the fb object
 *
 * If successful, this grabs an additional reference to the framebuffer -
 * callers need to make sure to eventually unreference the returned framebuffer
 * again.
 */
struct drm_framebuffer *drm_framebuffer_lookup(struct drm_device *dev,
					       uint32_t id)
{
	struct drm_framebuffer *fb;

	mutex_lock(&dev->mode_config.fb_lock);
	fb = __drm_framebuffer_lookup(dev, id);
	if (fb)
		drm_framebuffer_reference(fb);
	mutex_unlock(&dev->mode_config.fb_lock);

	return fb;
}
EXPORT_SYMBOL(drm_framebuffer_lookup);

/**
 * drm_framebuffer_unreference - unref a framebuffer
 * @fb: framebuffer to unref
 *
 * This functions decrements the fb's refcount and frees it if it drops to zero.
 */
void drm_framebuffer_unreference(struct drm_framebuffer *fb)
{
	DRM_DEBUG("FB ID: %d\n", fb->base.id);
	kref_put(&fb->refcount, drm_framebuffer_free);
}
EXPORT_SYMBOL(drm_framebuffer_unreference);

/**
 * drm_framebuffer_reference - incr the fb refcnt
 * @fb: framebuffer
 */
void drm_framebuffer_reference(struct drm_framebuffer *fb)
{
	DRM_DEBUG("FB ID: %d\n", fb->base.id);
	kref_get(&fb->refcount);
}
EXPORT_SYMBOL(drm_framebuffer_reference);

static void drm_framebuffer_free_bug(struct kref *kref)
{
	BUG();
}

static void __drm_framebuffer_unreference(struct drm_framebuffer *fb)
{
	DRM_DEBUG("FB ID: %d\n", fb->base.id);
	kref_put(&fb->refcount, drm_framebuffer_free_bug);
}

/* dev->mode_config.fb_lock must be held! */
static void __drm_framebuffer_unregister(struct drm_device *dev,
					 struct drm_framebuffer *fb)
{
	mutex_lock(&dev->mode_config.idr_mutex);
	idr_remove(&dev->mode_config.crtc_idr, fb->base.id);
	mutex_unlock(&dev->mode_config.idr_mutex);

	fb->base.id = 0;

	__drm_framebuffer_unreference(fb);
}

/**
 * drm_framebuffer_unregister_private - unregister a private fb from the lookup idr
 * @fb: fb to unregister
 *
 * Drivers need to call this when cleaning up driver-private framebuffers, e.g.
 * those used for fbdev. Note that the caller must hold a reference of it's own,
 * i.e. the object may not be destroyed through this call (since it'll lead to a
 * locking inversion).
 */
void drm_framebuffer_unregister_private(struct drm_framebuffer *fb)
{
	struct drm_device *dev = fb->dev;

	mutex_lock(&dev->mode_config.fb_lock);
	/* Mark fb as reaped and drop idr ref. */
	__drm_framebuffer_unregister(dev, fb);
	mutex_unlock(&dev->mode_config.fb_lock);
}
EXPORT_SYMBOL(drm_framebuffer_unregister_private);

>>>>>>> refs/remotes/origin/master
/**
 * drm_framebuffer_cleanup - remove a framebuffer object
 * @fb: framebuffer to remove
 *
<<<<<<< HEAD
 * LOCKING:
 * Caller must hold mode config lock.
 *
 * Scans all the CRTCs in @dev's mode_config.  If they're using @fb, removes
 * it, setting it to NULL.
=======
 * Cleanup references to a user-created framebuffer. This function is intended
 * to be used from the drivers ->destroy callback.
 *
 * Note that this function does not remove the fb from active usuage - if it is
 * still used anywhere, hilarity can ensue since userspace could call getfb on
 * the id and get back -EINVAL. Obviously no concern at driver unload time.
 *
 * Also, the framebuffer will not be removed from the lookup idr - for
 * user-created framebuffers this will happen in in the rmfb ioctl. For
 * driver-private objects (e.g. for fbdev) drivers need to explicitly call
 * drm_framebuffer_unregister_private.
>>>>>>> refs/remotes/origin/master
 */
void drm_framebuffer_cleanup(struct drm_framebuffer *fb)
{
	struct drm_device *dev = fb->dev;
<<<<<<< HEAD
	struct drm_crtc *crtc;
<<<<<<< HEAD
=======
	struct drm_plane *plane;
>>>>>>> refs/remotes/origin/cm-10.0
	struct drm_mode_set set;
	int ret;

	/* remove from any CRTC */
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		if (crtc->fb == fb) {
			/* should turn off the crtc */
			memset(&set, 0, sizeof(struct drm_mode_set));
			set.crtc = crtc;
			set.fb = NULL;
			ret = crtc->funcs->set_config(&set);
			if (ret)
				DRM_ERROR("failed to reset crtc %p when fb was deleted\n", crtc);
		}
	}

<<<<<<< HEAD
=======
	list_for_each_entry(plane, &dev->mode_config.plane_list, head) {
		if (plane->fb == fb) {
			/* should turn off the crtc */
			ret = plane->funcs->disable_plane(plane);
			if (ret)
				DRM_ERROR("failed to disable plane with busy fb\n");
			/* disconnect the plane from the fb and crtc: */
			plane->fb = NULL;
			plane->crtc = NULL;
		}
	}

>>>>>>> refs/remotes/origin/cm-10.0
	drm_mode_object_put(dev, &fb->base);
	list_del(&fb->head);
	dev->mode_config.num_fb--;
}
EXPORT_SYMBOL(drm_framebuffer_cleanup);
=======

	mutex_lock(&dev->mode_config.fb_lock);
	list_del(&fb->head);
	dev->mode_config.num_fb--;
	mutex_unlock(&dev->mode_config.fb_lock);
}
EXPORT_SYMBOL(drm_framebuffer_cleanup);

/**
 * drm_framebuffer_remove - remove and unreference a framebuffer object
 * @fb: framebuffer to remove
 *
 * Scans all the CRTCs and planes in @dev's mode_config.  If they're
 * using @fb, removes it, setting it to NULL. Then drops the reference to the
 * passed-in framebuffer. Might take the modeset locks.
 *
 * Note that this function optimizes the cleanup away if the caller holds the
 * last reference to the framebuffer. It is also guaranteed to not take the
 * modeset locks in this case.
 */
void drm_framebuffer_remove(struct drm_framebuffer *fb)
{
	struct drm_device *dev = fb->dev;
	struct drm_crtc *crtc;
	struct drm_plane *plane;
	struct drm_mode_set set;
	int ret;

	WARN_ON(!list_empty(&fb->filp_head));

	/*
	 * drm ABI mandates that we remove any deleted framebuffers from active
	 * useage. But since most sane clients only remove framebuffers they no
	 * longer need, try to optimize this away.
	 *
	 * Since we're holding a reference ourselves, observing a refcount of 1
	 * means that we're the last holder and can skip it. Also, the refcount
	 * can never increase from 1 again, so we don't need any barriers or
	 * locks.
	 *
	 * Note that userspace could try to race with use and instate a new
	 * usage _after_ we've cleared all current ones. End result will be an
	 * in-use fb with fb-id == 0. Userspace is allowed to shoot its own foot
	 * in this manner.
	 */
	if (atomic_read(&fb->refcount.refcount) > 1) {
		drm_modeset_lock_all(dev);
		/* remove from any CRTC */
		list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
			if (crtc->fb == fb) {
				/* should turn off the crtc */
				memset(&set, 0, sizeof(struct drm_mode_set));
				set.crtc = crtc;
				set.fb = NULL;
				ret = drm_mode_set_config_internal(&set);
				if (ret)
					DRM_ERROR("failed to reset crtc %p when fb was deleted\n", crtc);
			}
		}

		list_for_each_entry(plane, &dev->mode_config.plane_list, head) {
			if (plane->fb == fb)
				drm_plane_force_disable(plane);
		}
		drm_modeset_unlock_all(dev);
	}

	drm_framebuffer_unreference(fb);
}
EXPORT_SYMBOL(drm_framebuffer_remove);
>>>>>>> refs/remotes/origin/master

/**
 * drm_crtc_init - Initialise a new CRTC object
 * @dev: DRM device
 * @crtc: CRTC object to init
 * @funcs: callbacks for the new CRTC
 *
<<<<<<< HEAD
 * LOCKING:
 * Caller must hold mode config lock.
 *
 * Inits a new object created as base part of an driver crtc object.
<<<<<<< HEAD
 */
void drm_crtc_init(struct drm_device *dev, struct drm_crtc *crtc,
		   const struct drm_crtc_funcs *funcs)
{
=======
=======
 * Inits a new object created as base part of a driver crtc object.
>>>>>>> refs/remotes/origin/master
 *
 * RETURNS:
 * Zero on success, error code on failure.
 */
int drm_crtc_init(struct drm_device *dev, struct drm_crtc *crtc,
		   const struct drm_crtc_funcs *funcs)
{
	int ret;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	crtc->dev = dev;
	crtc->funcs = funcs;

	mutex_lock(&dev->mode_config.mutex);
<<<<<<< HEAD
	drm_mode_object_get(dev, &crtc->base, DRM_MODE_OBJECT_CRTC);

	list_add_tail(&crtc->head, &dev->mode_config.crtc_list);
	dev->mode_config.num_crtc++;
	mutex_unlock(&dev->mode_config.mutex);
=======
=======
	crtc->dev = dev;
	crtc->funcs = funcs;
	crtc->invert_dimensions = false;

	drm_modeset_lock_all(dev);
	mutex_init(&crtc->mutex);
	mutex_lock_nest_lock(&crtc->mutex, &dev->mode_config.mutex);
>>>>>>> refs/remotes/origin/master

	ret = drm_mode_object_get(dev, &crtc->base, DRM_MODE_OBJECT_CRTC);
	if (ret)
		goto out;

<<<<<<< HEAD
=======
	crtc->base.properties = &crtc->properties;

>>>>>>> refs/remotes/origin/master
	list_add_tail(&crtc->head, &dev->mode_config.crtc_list);
	dev->mode_config.num_crtc++;

 out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);

	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	drm_modeset_unlock_all(dev);

	return ret;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(drm_crtc_init);

/**
<<<<<<< HEAD
 * drm_crtc_cleanup - Cleans up the core crtc usage.
 * @crtc: CRTC to cleanup
 *
 * LOCKING:
 * Caller must hold mode config lock.
 *
 * Cleanup @crtc. Removes from drm modesetting space
 * does NOT free object, caller does that.
=======
 * drm_crtc_cleanup - Clean up the core crtc usage
 * @crtc: CRTC to cleanup
 *
 * This function cleans up @crtc and removes it from the DRM mode setting
 * core. Note that the function does *not* free the crtc structure itself,
 * this is the responsibility of the caller.
>>>>>>> refs/remotes/origin/master
 */
void drm_crtc_cleanup(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;

<<<<<<< HEAD
	if (crtc->gamma_store) {
		kfree(crtc->gamma_store);
		crtc->gamma_store = NULL;
	}
=======
	kfree(crtc->gamma_store);
	crtc->gamma_store = NULL;
>>>>>>> refs/remotes/origin/master

	drm_mode_object_put(dev, &crtc->base);
	list_del(&crtc->head);
	dev->mode_config.num_crtc--;
}
EXPORT_SYMBOL(drm_crtc_cleanup);

/**
 * drm_mode_probed_add - add a mode to a connector's probed mode list
 * @connector: connector the new mode
 * @mode: mode data
 *
<<<<<<< HEAD
 * LOCKING:
 * Caller must hold mode config lock.
 *
=======
>>>>>>> refs/remotes/origin/master
 * Add @mode to @connector's mode list for later use.
 */
void drm_mode_probed_add(struct drm_connector *connector,
			 struct drm_display_mode *mode)
{
<<<<<<< HEAD
	list_add(&mode->head, &connector->probed_modes);
}
EXPORT_SYMBOL(drm_mode_probed_add);

/**
=======
	list_add_tail(&mode->head, &connector->probed_modes);
}
EXPORT_SYMBOL(drm_mode_probed_add);

/*
>>>>>>> refs/remotes/origin/master
 * drm_mode_remove - remove and free a mode
 * @connector: connector list to modify
 * @mode: mode to remove
 *
<<<<<<< HEAD
 * LOCKING:
 * Caller must hold mode config lock.
 *
 * Remove @mode from @connector's mode list, then free it.
 */
void drm_mode_remove(struct drm_connector *connector,
		     struct drm_display_mode *mode)
{
	list_del(&mode->head);
<<<<<<< HEAD
	kfree(mode);
=======
	drm_mode_destroy(connector->dev, mode);
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(drm_mode_remove);
=======
 * Remove @mode from @connector's mode list, then free it.
 */
static void drm_mode_remove(struct drm_connector *connector,
			    struct drm_display_mode *mode)
{
	list_del(&mode->head);
	drm_mode_destroy(connector->dev, mode);
}
>>>>>>> refs/remotes/origin/master

/**
 * drm_connector_init - Init a preallocated connector
 * @dev: DRM device
 * @connector: the connector to init
 * @funcs: callbacks for this connector
<<<<<<< HEAD
 * @name: user visible name of the connector
 *
 * LOCKING:
<<<<<<< HEAD
 * Caller must hold @dev's mode_config lock.
 *
 * Initialises a preallocated connector. Connectors should be
 * subclassed as part of driver connector objects.
 */
void drm_connector_init(struct drm_device *dev,
		     struct drm_connector *connector,
		     const struct drm_connector_funcs *funcs,
		     int connector_type)
{
	mutex_lock(&dev->mode_config.mutex);

	connector->dev = dev;
	connector->funcs = funcs;
	drm_mode_object_get(dev, &connector->base, DRM_MODE_OBJECT_CONNECTOR);
=======
 * Takes mode config lock.
=======
 * @connector_type: user visible type of the connector
>>>>>>> refs/remotes/origin/master
 *
 * Initialises a preallocated connector. Connectors should be
 * subclassed as part of driver connector objects.
 *
 * RETURNS:
 * Zero on success, error code on failure.
 */
int drm_connector_init(struct drm_device *dev,
		       struct drm_connector *connector,
		       const struct drm_connector_funcs *funcs,
		       int connector_type)
{
	int ret;
<<<<<<< HEAD

	mutex_lock(&dev->mode_config.mutex);
=======
	struct ida *connector_ida =
		&drm_connector_enum_list[connector_type].ida;

	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master

	ret = drm_mode_object_get(dev, &connector->base, DRM_MODE_OBJECT_CONNECTOR);
	if (ret)
		goto out;

<<<<<<< HEAD
	connector->dev = dev;
	connector->funcs = funcs;
>>>>>>> refs/remotes/origin/cm-10.0
	connector->connector_type = connector_type;
	connector->connector_type_id =
		++drm_connector_enum_list[connector_type].count; /* TODO */
	INIT_LIST_HEAD(&connector->user_modes);
	INIT_LIST_HEAD(&connector->probed_modes);
	INIT_LIST_HEAD(&connector->modes);
	connector->edid_blob_ptr = NULL;
=======
	connector->base.properties = &connector->properties;
	connector->dev = dev;
	connector->funcs = funcs;
	connector->connector_type = connector_type;
	connector->connector_type_id =
		ida_simple_get(connector_ida, 1, 0, GFP_KERNEL);
	if (connector->connector_type_id < 0) {
		ret = connector->connector_type_id;
		drm_mode_object_put(dev, &connector->base);
		goto out;
	}
	INIT_LIST_HEAD(&connector->probed_modes);
	INIT_LIST_HEAD(&connector->modes);
	connector->edid_blob_ptr = NULL;
	connector->status = connector_status_unknown;
>>>>>>> refs/remotes/origin/master

	list_add_tail(&connector->head, &dev->mode_config.connector_list);
	dev->mode_config.num_connector++;

<<<<<<< HEAD
<<<<<<< HEAD
	drm_connector_attach_property(connector,
				      dev->mode_config.edid_property, 0);
=======
	if (connector_type != DRM_MODE_CONNECTOR_VIRTUAL)
		drm_connector_attach_property(connector,
					      dev->mode_config.edid_property,
					      0);
>>>>>>> refs/remotes/origin/cm-10.0

	drm_connector_attach_property(connector,
				      dev->mode_config.dpms_property, 0);

<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
 out:
	mutex_unlock(&dev->mode_config.mutex);

	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (connector_type != DRM_MODE_CONNECTOR_VIRTUAL)
		drm_object_attach_property(&connector->base,
					      dev->mode_config.edid_property,
					      0);

	drm_object_attach_property(&connector->base,
				      dev->mode_config.dpms_property, 0);

 out:
	drm_modeset_unlock_all(dev);

	return ret;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(drm_connector_init);

/**
 * drm_connector_cleanup - cleans up an initialised connector
 * @connector: connector to cleanup
 *
<<<<<<< HEAD
 * LOCKING:
<<<<<<< HEAD
 * Caller must hold @dev's mode_config lock.
=======
 * Takes mode config lock.
>>>>>>> refs/remotes/origin/cm-10.0
 *
=======
>>>>>>> refs/remotes/origin/master
 * Cleans up the connector but doesn't free the object.
 */
void drm_connector_cleanup(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	struct drm_display_mode *mode, *t;

	list_for_each_entry_safe(mode, t, &connector->probed_modes, head)
		drm_mode_remove(connector, mode);

	list_for_each_entry_safe(mode, t, &connector->modes, head)
		drm_mode_remove(connector, mode);

<<<<<<< HEAD
	list_for_each_entry_safe(mode, t, &connector->user_modes, head)
		drm_mode_remove(connector, mode);

	mutex_lock(&dev->mode_config.mutex);
	drm_mode_object_put(dev, &connector->base);
	list_del(&connector->head);
<<<<<<< HEAD
=======
	dev->mode_config.num_connector--;
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&dev->mode_config.mutex);
}
EXPORT_SYMBOL(drm_connector_cleanup);

<<<<<<< HEAD
void drm_encoder_init(struct drm_device *dev,
=======
=======
	ida_remove(&drm_connector_enum_list[connector->connector_type].ida,
		   connector->connector_type_id);

	drm_mode_object_put(dev, &connector->base);
	list_del(&connector->head);
	dev->mode_config.num_connector--;
}
EXPORT_SYMBOL(drm_connector_cleanup);

>>>>>>> refs/remotes/origin/master
void drm_connector_unplug_all(struct drm_device *dev)
{
	struct drm_connector *connector;

	/* taking the mode config mutex ends up in a clash with sysfs */
	list_for_each_entry(connector, &dev->mode_config.connector_list, head)
		drm_sysfs_connector_remove(connector);

}
EXPORT_SYMBOL(drm_connector_unplug_all);

<<<<<<< HEAD
int drm_encoder_init(struct drm_device *dev,
>>>>>>> refs/remotes/origin/cm-10.0
=======
int drm_bridge_init(struct drm_device *dev, struct drm_bridge *bridge,
		const struct drm_bridge_funcs *funcs)
{
	int ret;

	drm_modeset_lock_all(dev);

	ret = drm_mode_object_get(dev, &bridge->base, DRM_MODE_OBJECT_BRIDGE);
	if (ret)
		goto out;

	bridge->dev = dev;
	bridge->funcs = funcs;

	list_add_tail(&bridge->head, &dev->mode_config.bridge_list);
	dev->mode_config.num_bridge++;

 out:
	drm_modeset_unlock_all(dev);
	return ret;
}
EXPORT_SYMBOL(drm_bridge_init);

void drm_bridge_cleanup(struct drm_bridge *bridge)
{
	struct drm_device *dev = bridge->dev;

	drm_modeset_lock_all(dev);
	drm_mode_object_put(dev, &bridge->base);
	list_del(&bridge->head);
	dev->mode_config.num_bridge--;
	drm_modeset_unlock_all(dev);
}
EXPORT_SYMBOL(drm_bridge_cleanup);

int drm_encoder_init(struct drm_device *dev,
>>>>>>> refs/remotes/origin/master
		      struct drm_encoder *encoder,
		      const struct drm_encoder_funcs *funcs,
		      int encoder_type)
{
<<<<<<< HEAD
<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);

	encoder->dev = dev;

	drm_mode_object_get(dev, &encoder->base, DRM_MODE_OBJECT_ENCODER);
=======
	int ret;

	mutex_lock(&dev->mode_config.mutex);
=======
	int ret;

	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master

	ret = drm_mode_object_get(dev, &encoder->base, DRM_MODE_OBJECT_ENCODER);
	if (ret)
		goto out;

	encoder->dev = dev;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	encoder->encoder_type = encoder_type;
	encoder->funcs = funcs;

	list_add_tail(&encoder->head, &dev->mode_config.encoder_list);
	dev->mode_config.num_encoder++;

<<<<<<< HEAD
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
 out:
	mutex_unlock(&dev->mode_config.mutex);

	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
 out:
	drm_modeset_unlock_all(dev);

	return ret;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(drm_encoder_init);

void drm_encoder_cleanup(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	drm_mode_object_put(dev, &encoder->base);
	list_del(&encoder->head);
<<<<<<< HEAD
=======
	dev->mode_config.num_encoder--;
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&dev->mode_config.mutex);
}
EXPORT_SYMBOL(drm_encoder_cleanup);

<<<<<<< HEAD
=======
=======
	drm_modeset_lock_all(dev);
	drm_mode_object_put(dev, &encoder->base);
	list_del(&encoder->head);
	dev->mode_config.num_encoder--;
	drm_modeset_unlock_all(dev);
}
EXPORT_SYMBOL(drm_encoder_cleanup);

/**
 * drm_plane_init - Initialise a new plane object
 * @dev: DRM device
 * @plane: plane object to init
 * @possible_crtcs: bitmask of possible CRTCs
 * @funcs: callbacks for the new plane
 * @formats: array of supported formats (%DRM_FORMAT_*)
 * @format_count: number of elements in @formats
 * @priv: plane is private (hidden from userspace)?
 *
 * Inits a new object created as base part of a driver plane object.
 *
 * RETURNS:
 * Zero on success, error code on failure.
 */
>>>>>>> refs/remotes/origin/master
int drm_plane_init(struct drm_device *dev, struct drm_plane *plane,
		   unsigned long possible_crtcs,
		   const struct drm_plane_funcs *funcs,
		   const uint32_t *formats, uint32_t format_count,
		   bool priv)
{
	int ret;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
=======
	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master

	ret = drm_mode_object_get(dev, &plane->base, DRM_MODE_OBJECT_PLANE);
	if (ret)
		goto out;

<<<<<<< HEAD
=======
	plane->base.properties = &plane->properties;
>>>>>>> refs/remotes/origin/master
	plane->dev = dev;
	plane->funcs = funcs;
	plane->format_types = kmalloc(sizeof(uint32_t) * format_count,
				      GFP_KERNEL);
	if (!plane->format_types) {
		DRM_DEBUG_KMS("out of memory when allocating plane\n");
		drm_mode_object_put(dev, &plane->base);
		ret = -ENOMEM;
		goto out;
	}

	memcpy(plane->format_types, formats, format_count * sizeof(uint32_t));
	plane->format_count = format_count;
	plane->possible_crtcs = possible_crtcs;

	/* private planes are not exposed to userspace, but depending on
	 * display hardware, might be convenient to allow sharing programming
	 * for the scanout engine with the crtc implementation.
	 */
	if (!priv) {
		list_add_tail(&plane->head, &dev->mode_config.plane_list);
		dev->mode_config.num_plane++;
	} else {
		INIT_LIST_HEAD(&plane->head);
	}

 out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master

	return ret;
}
EXPORT_SYMBOL(drm_plane_init);

<<<<<<< HEAD
=======
/**
 * drm_plane_cleanup - Clean up the core plane usage
 * @plane: plane to cleanup
 *
 * This function cleans up @plane and removes it from the DRM mode setting
 * core. Note that the function does *not* free the plane structure itself,
 * this is the responsibility of the caller.
 */
>>>>>>> refs/remotes/origin/master
void drm_plane_cleanup(struct drm_plane *plane)
{
	struct drm_device *dev = plane->dev;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
=======
	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master
	kfree(plane->format_types);
	drm_mode_object_put(dev, &plane->base);
	/* if not added to a list, it must be a private plane */
	if (!list_empty(&plane->head)) {
		list_del(&plane->head);
		dev->mode_config.num_plane--;
	}
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
}
EXPORT_SYMBOL(drm_plane_cleanup);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	drm_modeset_unlock_all(dev);
}
EXPORT_SYMBOL(drm_plane_cleanup);

/**
 * drm_plane_force_disable - Forcibly disable a plane
 * @plane: plane to disable
 *
 * Forces the plane to be disabled.
 *
 * Used when the plane's current framebuffer is destroyed,
 * and when restoring fbdev mode.
 */
void drm_plane_force_disable(struct drm_plane *plane)
{
	int ret;

	if (!plane->fb)
		return;

	ret = plane->funcs->disable_plane(plane);
	if (ret)
		DRM_ERROR("failed to disable plane with busy fb\n");
	/* disconnect the plane from the fb and crtc: */
	__drm_framebuffer_unreference(plane->fb);
	plane->fb = NULL;
	plane->crtc = NULL;
}
EXPORT_SYMBOL(drm_plane_force_disable);

>>>>>>> refs/remotes/origin/master
/**
 * drm_mode_create - create a new display mode
 * @dev: DRM device
 *
<<<<<<< HEAD
 * LOCKING:
 * Caller must hold DRM mode_config lock.
 *
=======
>>>>>>> refs/remotes/origin/master
 * Create a new drm_display_mode, give it an ID, and return it.
 *
 * RETURNS:
 * Pointer to new mode on success, NULL on error.
 */
struct drm_display_mode *drm_mode_create(struct drm_device *dev)
{
	struct drm_display_mode *nmode;

	nmode = kzalloc(sizeof(struct drm_display_mode), GFP_KERNEL);
	if (!nmode)
		return NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	drm_mode_object_get(dev, &nmode->base, DRM_MODE_OBJECT_MODE);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (drm_mode_object_get(dev, &nmode->base, DRM_MODE_OBJECT_MODE)) {
		kfree(nmode);
		return NULL;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return nmode;
}
EXPORT_SYMBOL(drm_mode_create);

/**
 * drm_mode_destroy - remove a mode
 * @dev: DRM device
 * @mode: mode to remove
 *
<<<<<<< HEAD
 * LOCKING:
 * Caller must hold mode config lock.
 *
=======
>>>>>>> refs/remotes/origin/master
 * Free @mode's unique identifier, then free it.
 */
void drm_mode_destroy(struct drm_device *dev, struct drm_display_mode *mode)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (!mode)
		return;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!mode)
		return;

>>>>>>> refs/remotes/origin/master
	drm_mode_object_put(dev, &mode->base);

	kfree(mode);
}
EXPORT_SYMBOL(drm_mode_destroy);

static int drm_mode_create_standard_connector_properties(struct drm_device *dev)
{
	struct drm_property *edid;
	struct drm_property *dpms;
<<<<<<< HEAD
<<<<<<< HEAD
	int i;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Standard properties (apply to all connectors)
	 */
	edid = drm_property_create(dev, DRM_MODE_PROP_BLOB |
				   DRM_MODE_PROP_IMMUTABLE,
				   "EDID", 0);
	dev->mode_config.edid_property = edid;

<<<<<<< HEAD
<<<<<<< HEAD
	dpms = drm_property_create(dev, DRM_MODE_PROP_ENUM,
				   "DPMS", ARRAY_SIZE(drm_dpms_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dpms_enum_list); i++)
		drm_property_add_enum(dpms, i, drm_dpms_enum_list[i].type,
				      drm_dpms_enum_list[i].name);
=======
	dpms = drm_property_create_enum(dev, 0,
				   "DPMS", drm_dpms_enum_list,
				   ARRAY_SIZE(drm_dpms_enum_list));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dpms = drm_property_create_enum(dev, 0,
				   "DPMS", drm_dpms_enum_list,
				   ARRAY_SIZE(drm_dpms_enum_list));
>>>>>>> refs/remotes/origin/master
	dev->mode_config.dpms_property = dpms;

	return 0;
}

/**
 * drm_mode_create_dvi_i_properties - create DVI-I specific connector properties
 * @dev: DRM device
 *
 * Called by a driver the first time a DVI-I connector is made.
 */
int drm_mode_create_dvi_i_properties(struct drm_device *dev)
{
	struct drm_property *dvi_i_selector;
	struct drm_property *dvi_i_subconnector;
<<<<<<< HEAD
<<<<<<< HEAD
	int i;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (dev->mode_config.dvi_i_select_subconnector_property)
		return 0;

	dvi_i_selector =
<<<<<<< HEAD
<<<<<<< HEAD
		drm_property_create(dev, DRM_MODE_PROP_ENUM,
				    "select subconnector",
				    ARRAY_SIZE(drm_dvi_i_select_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dvi_i_select_enum_list); i++)
		drm_property_add_enum(dvi_i_selector, i,
				      drm_dvi_i_select_enum_list[i].type,
				      drm_dvi_i_select_enum_list[i].name);
	dev->mode_config.dvi_i_select_subconnector_property = dvi_i_selector;

	dvi_i_subconnector =
		drm_property_create(dev, DRM_MODE_PROP_ENUM |
				    DRM_MODE_PROP_IMMUTABLE,
				    "subconnector",
				    ARRAY_SIZE(drm_dvi_i_subconnector_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dvi_i_subconnector_enum_list); i++)
		drm_property_add_enum(dvi_i_subconnector, i,
				      drm_dvi_i_subconnector_enum_list[i].type,
				      drm_dvi_i_subconnector_enum_list[i].name);
=======
=======
>>>>>>> refs/remotes/origin/master
		drm_property_create_enum(dev, 0,
				    "select subconnector",
				    drm_dvi_i_select_enum_list,
				    ARRAY_SIZE(drm_dvi_i_select_enum_list));
	dev->mode_config.dvi_i_select_subconnector_property = dvi_i_selector;

	dvi_i_subconnector = drm_property_create_enum(dev, DRM_MODE_PROP_IMMUTABLE,
				    "subconnector",
				    drm_dvi_i_subconnector_enum_list,
				    ARRAY_SIZE(drm_dvi_i_subconnector_enum_list));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	dev->mode_config.dvi_i_subconnector_property = dvi_i_subconnector;

	return 0;
}
EXPORT_SYMBOL(drm_mode_create_dvi_i_properties);

/**
 * drm_create_tv_properties - create TV specific connector properties
 * @dev: DRM device
 * @num_modes: number of different TV formats (modes) supported
 * @modes: array of pointers to strings containing name of each format
 *
 * Called by a driver's TV initialization routine, this function creates
 * the TV specific connector properties for a given device.  Caller is
 * responsible for allocating a list of format names and passing them to
 * this routine.
 */
int drm_mode_create_tv_properties(struct drm_device *dev, int num_modes,
				  char *modes[])
{
	struct drm_property *tv_selector;
	struct drm_property *tv_subconnector;
	int i;

	if (dev->mode_config.tv_select_subconnector_property)
		return 0;

	/*
	 * Basic connector properties
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	tv_selector = drm_property_create(dev, DRM_MODE_PROP_ENUM,
					  "select subconnector",
					  ARRAY_SIZE(drm_tv_select_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_tv_select_enum_list); i++)
		drm_property_add_enum(tv_selector, i,
				      drm_tv_select_enum_list[i].type,
				      drm_tv_select_enum_list[i].name);
	dev->mode_config.tv_select_subconnector_property = tv_selector;

	tv_subconnector =
		drm_property_create(dev, DRM_MODE_PROP_ENUM |
				    DRM_MODE_PROP_IMMUTABLE, "subconnector",
				    ARRAY_SIZE(drm_tv_subconnector_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_tv_subconnector_enum_list); i++)
		drm_property_add_enum(tv_subconnector, i,
				      drm_tv_subconnector_enum_list[i].type,
				      drm_tv_subconnector_enum_list[i].name);
=======
=======
>>>>>>> refs/remotes/origin/master
	tv_selector = drm_property_create_enum(dev, 0,
					  "select subconnector",
					  drm_tv_select_enum_list,
					  ARRAY_SIZE(drm_tv_select_enum_list));
	dev->mode_config.tv_select_subconnector_property = tv_selector;

	tv_subconnector =
		drm_property_create_enum(dev, DRM_MODE_PROP_IMMUTABLE,
				    "subconnector",
				    drm_tv_subconnector_enum_list,
				    ARRAY_SIZE(drm_tv_subconnector_enum_list));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	dev->mode_config.tv_subconnector_property = tv_subconnector;

	/*
	 * Other, TV specific properties: margins & TV modes.
	 */
	dev->mode_config.tv_left_margin_property =
<<<<<<< HEAD
<<<<<<< HEAD
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "left margin", 2);
	dev->mode_config.tv_left_margin_property->values[0] = 0;
	dev->mode_config.tv_left_margin_property->values[1] = 100;

	dev->mode_config.tv_right_margin_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "right margin", 2);
	dev->mode_config.tv_right_margin_property->values[0] = 0;
	dev->mode_config.tv_right_margin_property->values[1] = 100;

	dev->mode_config.tv_top_margin_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "top margin", 2);
	dev->mode_config.tv_top_margin_property->values[0] = 0;
	dev->mode_config.tv_top_margin_property->values[1] = 100;

	dev->mode_config.tv_bottom_margin_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "bottom margin", 2);
	dev->mode_config.tv_bottom_margin_property->values[0] = 0;
	dev->mode_config.tv_bottom_margin_property->values[1] = 100;
=======
=======
>>>>>>> refs/remotes/origin/master
		drm_property_create_range(dev, 0, "left margin", 0, 100);

	dev->mode_config.tv_right_margin_property =
		drm_property_create_range(dev, 0, "right margin", 0, 100);

	dev->mode_config.tv_top_margin_property =
		drm_property_create_range(dev, 0, "top margin", 0, 100);

	dev->mode_config.tv_bottom_margin_property =
		drm_property_create_range(dev, 0, "bottom margin", 0, 100);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	dev->mode_config.tv_mode_property =
		drm_property_create(dev, DRM_MODE_PROP_ENUM,
				    "mode", num_modes);
	for (i = 0; i < num_modes; i++)
		drm_property_add_enum(dev->mode_config.tv_mode_property, i,
				      i, modes[i]);

	dev->mode_config.tv_brightness_property =
<<<<<<< HEAD
<<<<<<< HEAD
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "brightness", 2);
	dev->mode_config.tv_brightness_property->values[0] = 0;
	dev->mode_config.tv_brightness_property->values[1] = 100;

	dev->mode_config.tv_contrast_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "contrast", 2);
	dev->mode_config.tv_contrast_property->values[0] = 0;
	dev->mode_config.tv_contrast_property->values[1] = 100;

	dev->mode_config.tv_flicker_reduction_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "flicker reduction", 2);
	dev->mode_config.tv_flicker_reduction_property->values[0] = 0;
	dev->mode_config.tv_flicker_reduction_property->values[1] = 100;

	dev->mode_config.tv_overscan_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "overscan", 2);
	dev->mode_config.tv_overscan_property->values[0] = 0;
	dev->mode_config.tv_overscan_property->values[1] = 100;

	dev->mode_config.tv_saturation_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "saturation", 2);
	dev->mode_config.tv_saturation_property->values[0] = 0;
	dev->mode_config.tv_saturation_property->values[1] = 100;

	dev->mode_config.tv_hue_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "hue", 2);
	dev->mode_config.tv_hue_property->values[0] = 0;
	dev->mode_config.tv_hue_property->values[1] = 100;
=======
=======
>>>>>>> refs/remotes/origin/master
		drm_property_create_range(dev, 0, "brightness", 0, 100);

	dev->mode_config.tv_contrast_property =
		drm_property_create_range(dev, 0, "contrast", 0, 100);

	dev->mode_config.tv_flicker_reduction_property =
		drm_property_create_range(dev, 0, "flicker reduction", 0, 100);

	dev->mode_config.tv_overscan_property =
		drm_property_create_range(dev, 0, "overscan", 0, 100);

	dev->mode_config.tv_saturation_property =
		drm_property_create_range(dev, 0, "saturation", 0, 100);

	dev->mode_config.tv_hue_property =
		drm_property_create_range(dev, 0, "hue", 0, 100);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}
EXPORT_SYMBOL(drm_mode_create_tv_properties);

/**
 * drm_mode_create_scaling_mode_property - create scaling mode property
 * @dev: DRM device
 *
 * Called by a driver the first time it's needed, must be attached to desired
 * connectors.
 */
int drm_mode_create_scaling_mode_property(struct drm_device *dev)
{
	struct drm_property *scaling_mode;
<<<<<<< HEAD
<<<<<<< HEAD
	int i;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (dev->mode_config.scaling_mode_property)
		return 0;

	scaling_mode =
<<<<<<< HEAD
<<<<<<< HEAD
		drm_property_create(dev, DRM_MODE_PROP_ENUM, "scaling mode",
				    ARRAY_SIZE(drm_scaling_mode_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_scaling_mode_enum_list); i++)
		drm_property_add_enum(scaling_mode, i,
				      drm_scaling_mode_enum_list[i].type,
				      drm_scaling_mode_enum_list[i].name);
=======
		drm_property_create_enum(dev, 0, "scaling mode",
				drm_scaling_mode_enum_list,
				    ARRAY_SIZE(drm_scaling_mode_enum_list));
>>>>>>> refs/remotes/origin/cm-10.0
=======
		drm_property_create_enum(dev, 0, "scaling mode",
				drm_scaling_mode_enum_list,
				    ARRAY_SIZE(drm_scaling_mode_enum_list));
>>>>>>> refs/remotes/origin/master

	dev->mode_config.scaling_mode_property = scaling_mode;

	return 0;
}
EXPORT_SYMBOL(drm_mode_create_scaling_mode_property);

/**
<<<<<<< HEAD
 * drm_mode_create_dithering_property - create dithering property
 * @dev: DRM device
 *
 * Called by a driver the first time it's needed, must be attached to desired
 * connectors.
 */
int drm_mode_create_dithering_property(struct drm_device *dev)
{
	struct drm_property *dithering_mode;
<<<<<<< HEAD
	int i;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	if (dev->mode_config.dithering_mode_property)
		return 0;

	dithering_mode =
<<<<<<< HEAD
		drm_property_create(dev, DRM_MODE_PROP_ENUM, "dithering",
				    ARRAY_SIZE(drm_dithering_mode_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dithering_mode_enum_list); i++)
		drm_property_add_enum(dithering_mode, i,
				      drm_dithering_mode_enum_list[i].type,
				      drm_dithering_mode_enum_list[i].name);
=======
		drm_property_create_enum(dev, 0, "dithering",
				drm_dithering_mode_enum_list,
				    ARRAY_SIZE(drm_dithering_mode_enum_list));
>>>>>>> refs/remotes/origin/cm-10.0
	dev->mode_config.dithering_mode_property = dithering_mode;

	return 0;
}
EXPORT_SYMBOL(drm_mode_create_dithering_property);

/**
=======
>>>>>>> refs/remotes/origin/master
 * drm_mode_create_dirty_property - create dirty property
 * @dev: DRM device
 *
 * Called by a driver the first time it's needed, must be attached to desired
 * connectors.
 */
int drm_mode_create_dirty_info_property(struct drm_device *dev)
{
	struct drm_property *dirty_info;
<<<<<<< HEAD
<<<<<<< HEAD
	int i;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (dev->mode_config.dirty_info_property)
		return 0;

	dirty_info =
<<<<<<< HEAD
<<<<<<< HEAD
		drm_property_create(dev, DRM_MODE_PROP_ENUM |
				    DRM_MODE_PROP_IMMUTABLE,
				    "dirty",
				    ARRAY_SIZE(drm_dirty_info_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dirty_info_enum_list); i++)
		drm_property_add_enum(dirty_info, i,
				      drm_dirty_info_enum_list[i].type,
				      drm_dirty_info_enum_list[i].name);
=======
=======
>>>>>>> refs/remotes/origin/master
		drm_property_create_enum(dev, DRM_MODE_PROP_IMMUTABLE,
				    "dirty",
				    drm_dirty_info_enum_list,
				    ARRAY_SIZE(drm_dirty_info_enum_list));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	dev->mode_config.dirty_info_property = dirty_info;

	return 0;
}
EXPORT_SYMBOL(drm_mode_create_dirty_info_property);

<<<<<<< HEAD
/**
 * drm_mode_config_init - initialize DRM mode_configuration structure
 * @dev: DRM device
 *
 * LOCKING:
 * None, should happen single threaded at init time.
 *
 * Initialize @dev's mode_config structure, used for tracking the graphics
 * configuration of @dev.
 */
void drm_mode_config_init(struct drm_device *dev)
{
	mutex_init(&dev->mode_config.mutex);
	mutex_init(&dev->mode_config.idr_mutex);
	INIT_LIST_HEAD(&dev->mode_config.fb_list);
	INIT_LIST_HEAD(&dev->mode_config.crtc_list);
	INIT_LIST_HEAD(&dev->mode_config.connector_list);
	INIT_LIST_HEAD(&dev->mode_config.encoder_list);
	INIT_LIST_HEAD(&dev->mode_config.property_list);
	INIT_LIST_HEAD(&dev->mode_config.property_blob_list);
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&dev->mode_config.plane_list);
>>>>>>> refs/remotes/origin/cm-10.0
	idr_init(&dev->mode_config.crtc_idr);

	mutex_lock(&dev->mode_config.mutex);
	drm_mode_create_standard_connector_properties(dev);
	mutex_unlock(&dev->mode_config.mutex);

	/* Just to be sure */
	dev->mode_config.num_fb = 0;
	dev->mode_config.num_connector = 0;
	dev->mode_config.num_crtc = 0;
	dev->mode_config.num_encoder = 0;
}
EXPORT_SYMBOL(drm_mode_config_init);

int drm_mode_group_init(struct drm_device *dev, struct drm_mode_group *group)
=======
static int drm_mode_group_init(struct drm_device *dev, struct drm_mode_group *group)
>>>>>>> refs/remotes/origin/master
{
	uint32_t total_objects = 0;

	total_objects += dev->mode_config.num_crtc;
	total_objects += dev->mode_config.num_connector;
	total_objects += dev->mode_config.num_encoder;
<<<<<<< HEAD
=======
	total_objects += dev->mode_config.num_bridge;
>>>>>>> refs/remotes/origin/master

	group->id_list = kzalloc(total_objects * sizeof(uint32_t), GFP_KERNEL);
	if (!group->id_list)
		return -ENOMEM;

	group->num_crtcs = 0;
	group->num_connectors = 0;
	group->num_encoders = 0;
<<<<<<< HEAD
=======
	group->num_bridges = 0;
>>>>>>> refs/remotes/origin/master
	return 0;
}

int drm_mode_group_init_legacy_group(struct drm_device *dev,
				     struct drm_mode_group *group)
{
	struct drm_crtc *crtc;
	struct drm_encoder *encoder;
	struct drm_connector *connector;
<<<<<<< HEAD
=======
	struct drm_bridge *bridge;
>>>>>>> refs/remotes/origin/master
	int ret;

	if ((ret = drm_mode_group_init(dev, group)))
		return ret;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head)
		group->id_list[group->num_crtcs++] = crtc->base.id;

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head)
		group->id_list[group->num_crtcs + group->num_encoders++] =
		encoder->base.id;

	list_for_each_entry(connector, &dev->mode_config.connector_list, head)
		group->id_list[group->num_crtcs + group->num_encoders +
			       group->num_connectors++] = connector->base.id;

<<<<<<< HEAD
	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(drm_mode_group_init_legacy_group);
>>>>>>> refs/remotes/origin/cm-10.0

/**
 * drm_mode_config_cleanup - free up DRM mode_config info
 * @dev: DRM device
 *
 * LOCKING:
 * Caller must hold mode config lock.
 *
 * Free up all the connectors and CRTCs associated with this DRM device, then
 * free up the framebuffers and associated buffer objects.
 *
 * FIXME: cleanup any dangling user buffer objects too
 */
void drm_mode_config_cleanup(struct drm_device *dev)
{
	struct drm_connector *connector, *ot;
	struct drm_crtc *crtc, *ct;
	struct drm_encoder *encoder, *enct;
	struct drm_framebuffer *fb, *fbt;
	struct drm_property *property, *pt;
<<<<<<< HEAD
=======
	struct drm_plane *plane, *plt;
>>>>>>> refs/remotes/origin/cm-10.0

	list_for_each_entry_safe(encoder, enct, &dev->mode_config.encoder_list,
				 head) {
		encoder->funcs->destroy(encoder);
	}

	list_for_each_entry_safe(connector, ot,
				 &dev->mode_config.connector_list, head) {
		connector->funcs->destroy(connector);
	}

	list_for_each_entry_safe(property, pt, &dev->mode_config.property_list,
				 head) {
		drm_property_destroy(dev, property);
	}

	list_for_each_entry_safe(fb, fbt, &dev->mode_config.fb_list, head) {
		fb->funcs->destroy(fb);
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	list_for_each_entry_safe(plane, plt, &dev->mode_config.plane_list,
				 head) {
		plane->funcs->destroy(plane);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	list_for_each_entry_safe(crtc, ct, &dev->mode_config.crtc_list, head) {
		crtc->funcs->destroy(crtc);
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	idr_remove_all(&dev->mode_config.crtc_idr);
	idr_destroy(&dev->mode_config.crtc_idr);
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(drm_mode_config_cleanup);
=======
	list_for_each_entry(bridge, &dev->mode_config.bridge_list, head)
		group->id_list[group->num_crtcs + group->num_encoders +
			       group->num_connectors + group->num_bridges++] =
					bridge->base.id;

	return 0;
}
EXPORT_SYMBOL(drm_mode_group_init_legacy_group);
>>>>>>> refs/remotes/origin/master

/**
 * drm_crtc_convert_to_umode - convert a drm_display_mode into a modeinfo
 * @out: drm_mode_modeinfo struct to return to the user
 * @in: drm_display_mode to use
 *
<<<<<<< HEAD
 * LOCKING:
 * None.
 *
 * Convert a drm_display_mode into a drm_mode_modeinfo structure to return to
 * the user.
 */
<<<<<<< HEAD
void drm_crtc_convert_to_umode(struct drm_mode_modeinfo *out,
			       struct drm_display_mode *in)
{
=======
=======
 * Convert a drm_display_mode into a drm_mode_modeinfo structure to return to
 * the user.
 */
>>>>>>> refs/remotes/origin/master
static void drm_crtc_convert_to_umode(struct drm_mode_modeinfo *out,
				      const struct drm_display_mode *in)
{
	WARN(in->hdisplay > USHRT_MAX || in->hsync_start > USHRT_MAX ||
	     in->hsync_end > USHRT_MAX || in->htotal > USHRT_MAX ||
	     in->hskew > USHRT_MAX || in->vdisplay > USHRT_MAX ||
	     in->vsync_start > USHRT_MAX || in->vsync_end > USHRT_MAX ||
	     in->vtotal > USHRT_MAX || in->vscan > USHRT_MAX,
	     "timing values too large for mode info\n");

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	out->clock = in->clock;
	out->hdisplay = in->hdisplay;
	out->hsync_start = in->hsync_start;
	out->hsync_end = in->hsync_end;
	out->htotal = in->htotal;
	out->hskew = in->hskew;
	out->vdisplay = in->vdisplay;
	out->vsync_start = in->vsync_start;
	out->vsync_end = in->vsync_end;
	out->vtotal = in->vtotal;
	out->vscan = in->vscan;
	out->vrefresh = in->vrefresh;
	out->flags = in->flags;
	out->type = in->type;
	strncpy(out->name, in->name, DRM_DISPLAY_MODE_LEN);
	out->name[DRM_DISPLAY_MODE_LEN-1] = 0;
}

/**
<<<<<<< HEAD
 * drm_crtc_convert_to_umode - convert a modeinfo into a drm_display_mode
 * @out: drm_display_mode to return to the user
 * @in: drm_mode_modeinfo to use
 *
 * LOCKING:
 * None.
 *
 * Convert a drm_mode_modeinfo into a drm_display_mode structure to return to
 * the caller.
<<<<<<< HEAD
 */
void drm_crtc_convert_umode(struct drm_display_mode *out,
			    struct drm_mode_modeinfo *in)
{
=======
=======
 * drm_crtc_convert_umode - convert a modeinfo into a drm_display_mode
 * @out: drm_display_mode to return to the user
 * @in: drm_mode_modeinfo to use
 *
 * Convert a drm_mode_modeinfo into a drm_display_mode structure to return to
 * the caller.
>>>>>>> refs/remotes/origin/master
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
static int drm_crtc_convert_umode(struct drm_display_mode *out,
				  const struct drm_mode_modeinfo *in)
{
	if (in->clock > INT_MAX || in->vrefresh > INT_MAX)
		return -ERANGE;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((in->flags & DRM_MODE_FLAG_3D_MASK) > DRM_MODE_FLAG_3D_MAX)
		return -EINVAL;

>>>>>>> refs/remotes/origin/master
	out->clock = in->clock;
	out->hdisplay = in->hdisplay;
	out->hsync_start = in->hsync_start;
	out->hsync_end = in->hsync_end;
	out->htotal = in->htotal;
	out->hskew = in->hskew;
	out->vdisplay = in->vdisplay;
	out->vsync_start = in->vsync_start;
	out->vsync_end = in->vsync_end;
	out->vtotal = in->vtotal;
	out->vscan = in->vscan;
	out->vrefresh = in->vrefresh;
	out->flags = in->flags;
	out->type = in->type;
	strncpy(out->name, in->name, DRM_DISPLAY_MODE_LEN);
	out->name[DRM_DISPLAY_MODE_LEN-1] = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	return 0;
>>>>>>> refs/remotes/origin/master
}

/**
 * drm_mode_getresources - get graphics configuration
<<<<<<< HEAD
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * LOCKING:
 * Takes mode config lock.
=======
 * @dev: drm device for the ioctl
 * @data: data pointer for the ioctl
 * @file_priv: drm file for the ioctl call
>>>>>>> refs/remotes/origin/master
 *
 * Construct a set of configuration description structures and return
 * them to the user, including CRTC, connector and framebuffer configuration.
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_getresources(struct drm_device *dev, void *data,
			  struct drm_file *file_priv)
{
	struct drm_mode_card_res *card_res = data;
	struct list_head *lh;
	struct drm_framebuffer *fb;
	struct drm_connector *connector;
	struct drm_crtc *crtc;
	struct drm_encoder *encoder;
	int ret = 0;
	int connector_count = 0;
	int crtc_count = 0;
	int fb_count = 0;
	int encoder_count = 0;
	int copied = 0, i;
	uint32_t __user *fb_id;
	uint32_t __user *crtc_id;
	uint32_t __user *connector_id;
	uint32_t __user *encoder_id;
	struct drm_mode_group *mode_group;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);

=======

	mutex_lock(&file_priv->fbs_lock);
>>>>>>> refs/remotes/origin/master
	/*
	 * For the non-control nodes we need to limit the list of resources
	 * by IDs in the group list for this node
	 */
	list_for_each(lh, &file_priv->fbs)
		fb_count++;

<<<<<<< HEAD
=======
	/* handle this in 4 parts */
	/* FBs */
	if (card_res->count_fbs >= fb_count) {
		copied = 0;
		fb_id = (uint32_t __user *)(unsigned long)card_res->fb_id_ptr;
		list_for_each_entry(fb, &file_priv->fbs, filp_head) {
			if (put_user(fb->base.id, fb_id + copied)) {
				mutex_unlock(&file_priv->fbs_lock);
				return -EFAULT;
			}
			copied++;
		}
	}
	card_res->count_fbs = fb_count;
	mutex_unlock(&file_priv->fbs_lock);

	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master
	mode_group = &file_priv->master->minor->mode_group;
	if (file_priv->master->minor->type == DRM_MINOR_CONTROL) {

		list_for_each(lh, &dev->mode_config.crtc_list)
			crtc_count++;

		list_for_each(lh, &dev->mode_config.connector_list)
			connector_count++;

		list_for_each(lh, &dev->mode_config.encoder_list)
			encoder_count++;
	} else {

		crtc_count = mode_group->num_crtcs;
		connector_count = mode_group->num_connectors;
		encoder_count = mode_group->num_encoders;
	}

	card_res->max_height = dev->mode_config.max_height;
	card_res->min_height = dev->mode_config.min_height;
	card_res->max_width = dev->mode_config.max_width;
	card_res->min_width = dev->mode_config.min_width;

<<<<<<< HEAD
	/* handle this in 4 parts */
	/* FBs */
	if (card_res->count_fbs >= fb_count) {
		copied = 0;
		fb_id = (uint32_t __user *)(unsigned long)card_res->fb_id_ptr;
		list_for_each_entry(fb, &file_priv->fbs, filp_head) {
			if (put_user(fb->base.id, fb_id + copied)) {
				ret = -EFAULT;
				goto out;
			}
			copied++;
		}
	}
	card_res->count_fbs = fb_count;

=======
>>>>>>> refs/remotes/origin/master
	/* CRTCs */
	if (card_res->count_crtcs >= crtc_count) {
		copied = 0;
		crtc_id = (uint32_t __user *)(unsigned long)card_res->crtc_id_ptr;
		if (file_priv->master->minor->type == DRM_MINOR_CONTROL) {
			list_for_each_entry(crtc, &dev->mode_config.crtc_list,
					    head) {
				DRM_DEBUG_KMS("[CRTC:%d]\n", crtc->base.id);
				if (put_user(crtc->base.id, crtc_id + copied)) {
					ret = -EFAULT;
					goto out;
				}
				copied++;
			}
		} else {
			for (i = 0; i < mode_group->num_crtcs; i++) {
				if (put_user(mode_group->id_list[i],
					     crtc_id + copied)) {
					ret = -EFAULT;
					goto out;
				}
				copied++;
			}
		}
	}
	card_res->count_crtcs = crtc_count;

	/* Encoders */
	if (card_res->count_encoders >= encoder_count) {
		copied = 0;
		encoder_id = (uint32_t __user *)(unsigned long)card_res->encoder_id_ptr;
		if (file_priv->master->minor->type == DRM_MINOR_CONTROL) {
			list_for_each_entry(encoder,
					    &dev->mode_config.encoder_list,
					    head) {
				DRM_DEBUG_KMS("[ENCODER:%d:%s]\n", encoder->base.id,
						drm_get_encoder_name(encoder));
				if (put_user(encoder->base.id, encoder_id +
					     copied)) {
					ret = -EFAULT;
					goto out;
				}
				copied++;
			}
		} else {
			for (i = mode_group->num_crtcs; i < mode_group->num_crtcs + mode_group->num_encoders; i++) {
				if (put_user(mode_group->id_list[i],
					     encoder_id + copied)) {
					ret = -EFAULT;
					goto out;
				}
				copied++;
			}

		}
	}
	card_res->count_encoders = encoder_count;

	/* Connectors */
	if (card_res->count_connectors >= connector_count) {
		copied = 0;
		connector_id = (uint32_t __user *)(unsigned long)card_res->connector_id_ptr;
		if (file_priv->master->minor->type == DRM_MINOR_CONTROL) {
			list_for_each_entry(connector,
					    &dev->mode_config.connector_list,
					    head) {
				DRM_DEBUG_KMS("[CONNECTOR:%d:%s]\n",
					connector->base.id,
					drm_get_connector_name(connector));
				if (put_user(connector->base.id,
					     connector_id + copied)) {
					ret = -EFAULT;
					goto out;
				}
				copied++;
			}
		} else {
			int start = mode_group->num_crtcs +
				mode_group->num_encoders;
			for (i = start; i < start + mode_group->num_connectors; i++) {
				if (put_user(mode_group->id_list[i],
					     connector_id + copied)) {
					ret = -EFAULT;
					goto out;
				}
				copied++;
			}
		}
	}
	card_res->count_connectors = connector_count;

	DRM_DEBUG_KMS("CRTC[%d] CONNECTORS[%d] ENCODERS[%d]\n", card_res->count_crtcs,
		  card_res->count_connectors, card_res->count_encoders);

out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

/**
 * drm_mode_getcrtc - get CRTC configuration
<<<<<<< HEAD
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * LOCKING:
<<<<<<< HEAD
 * Caller? (FIXME)
=======
 * Takes mode config lock.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @dev: drm device for the ioctl
 * @data: data pointer for the ioctl
 * @file_priv: drm file for the ioctl call
>>>>>>> refs/remotes/origin/master
 *
 * Construct a CRTC configuration structure to return to the user.
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_getcrtc(struct drm_device *dev,
		     void *data, struct drm_file *file_priv)
{
	struct drm_mode_crtc *crtc_resp = data;
	struct drm_crtc *crtc;
	struct drm_mode_object *obj;
	int ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
=======
	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master

	obj = drm_mode_object_find(dev, crtc_resp->crtc_id,
				   DRM_MODE_OBJECT_CRTC);
	if (!obj) {
<<<<<<< HEAD
		ret = -EINVAL;
=======
		ret = -ENOENT;
>>>>>>> refs/remotes/origin/master
		goto out;
	}
	crtc = obj_to_crtc(obj);

	crtc_resp->x = crtc->x;
	crtc_resp->y = crtc->y;
	crtc_resp->gamma_size = crtc->gamma_size;
	if (crtc->fb)
		crtc_resp->fb_id = crtc->fb->base.id;
	else
		crtc_resp->fb_id = 0;

	if (crtc->enabled) {

		drm_crtc_convert_to_umode(&crtc_resp->mode, &crtc->mode);
		crtc_resp->mode_valid = 1;

	} else {
		crtc_resp->mode_valid = 0;
	}

out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
}

/**
 * drm_mode_getconnector - get connector configuration
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * LOCKING:
<<<<<<< HEAD
 * Caller? (FIXME)
=======
 * Takes mode config lock.
>>>>>>> refs/remotes/origin/cm-10.0
=======
	drm_modeset_unlock_all(dev);
	return ret;
}

static bool drm_mode_expose_to_userspace(const struct drm_display_mode *mode,
					 const struct drm_file *file_priv)
{
	/*
	 * If user-space hasn't configured the driver to expose the stereo 3D
	 * modes, don't expose them.
	 */
	if (!file_priv->stereo_allowed && drm_mode_is_stereo(mode))
		return false;

	return true;
}

/**
 * drm_mode_getconnector - get connector configuration
 * @dev: drm device for the ioctl
 * @data: data pointer for the ioctl
 * @file_priv: drm file for the ioctl call
>>>>>>> refs/remotes/origin/master
 *
 * Construct a connector configuration structure to return to the user.
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_getconnector(struct drm_device *dev, void *data,
			  struct drm_file *file_priv)
{
	struct drm_mode_get_connector *out_resp = data;
	struct drm_mode_object *obj;
	struct drm_connector *connector;
	struct drm_display_mode *mode;
	int mode_count = 0;
	int props_count = 0;
	int encoders_count = 0;
	int ret = 0;
	int copied = 0;
	int i;
	struct drm_mode_modeinfo u_mode;
	struct drm_mode_modeinfo __user *mode_ptr;
	uint32_t __user *prop_ptr;
	uint64_t __user *prop_values;
	uint32_t __user *encoder_ptr;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

	memset(&u_mode, 0, sizeof(struct drm_mode_modeinfo));

	DRM_DEBUG_KMS("[CONNECTOR:%d:?]\n", out_resp->connector_id);

	mutex_lock(&dev->mode_config.mutex);

	obj = drm_mode_object_find(dev, out_resp->connector_id,
				   DRM_MODE_OBJECT_CONNECTOR);
	if (!obj) {
<<<<<<< HEAD
		ret = -EINVAL;
=======
		ret = -ENOENT;
>>>>>>> refs/remotes/origin/master
		goto out;
	}
	connector = obj_to_connector(obj);

<<<<<<< HEAD
	for (i = 0; i < DRM_CONNECTOR_MAX_PROPERTY; i++) {
		if (connector->property_ids[i] != 0) {
			props_count++;
		}
	}
=======
	props_count = connector->properties.count;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < DRM_CONNECTOR_MAX_ENCODER; i++) {
		if (connector->encoder_ids[i] != 0) {
			encoders_count++;
		}
	}

	if (out_resp->count_modes == 0) {
		connector->funcs->fill_modes(connector,
					     dev->mode_config.max_width,
					     dev->mode_config.max_height);
	}

	/* delayed so we get modes regardless of pre-fill_modes state */
	list_for_each_entry(mode, &connector->modes, head)
<<<<<<< HEAD
		mode_count++;
=======
		if (drm_mode_expose_to_userspace(mode, file_priv))
			mode_count++;
>>>>>>> refs/remotes/origin/master

	out_resp->connector_id = connector->base.id;
	out_resp->connector_type = connector->connector_type;
	out_resp->connector_type_id = connector->connector_type_id;
	out_resp->mm_width = connector->display_info.width_mm;
	out_resp->mm_height = connector->display_info.height_mm;
	out_resp->subpixel = connector->display_info.subpixel_order;
	out_resp->connection = connector->status;
	if (connector->encoder)
		out_resp->encoder_id = connector->encoder->base.id;
	else
		out_resp->encoder_id = 0;

	/*
	 * This ioctl is called twice, once to determine how much space is
	 * needed, and the 2nd time to fill it.
	 */
	if ((out_resp->count_modes >= mode_count) && mode_count) {
		copied = 0;
<<<<<<< HEAD
<<<<<<< HEAD
		mode_ptr = (struct drm_mode_modeinfo *)(unsigned long)out_resp->modes_ptr;
=======
		mode_ptr = (struct drm_mode_modeinfo __user *)(unsigned long)out_resp->modes_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
		list_for_each_entry(mode, &connector->modes, head) {
=======
		mode_ptr = (struct drm_mode_modeinfo __user *)(unsigned long)out_resp->modes_ptr;
		list_for_each_entry(mode, &connector->modes, head) {
			if (!drm_mode_expose_to_userspace(mode, file_priv))
				continue;

>>>>>>> refs/remotes/origin/master
			drm_crtc_convert_to_umode(&u_mode, mode);
			if (copy_to_user(mode_ptr + copied,
					 &u_mode, sizeof(u_mode))) {
				ret = -EFAULT;
				goto out;
			}
			copied++;
		}
	}
	out_resp->count_modes = mode_count;

	if ((out_resp->count_props >= props_count) && props_count) {
		copied = 0;
<<<<<<< HEAD
<<<<<<< HEAD
		prop_ptr = (uint32_t *)(unsigned long)(out_resp->props_ptr);
		prop_values = (uint64_t *)(unsigned long)(out_resp->prop_values_ptr);
=======
		prop_ptr = (uint32_t __user *)(unsigned long)(out_resp->props_ptr);
		prop_values = (uint64_t __user *)(unsigned long)(out_resp->prop_values_ptr);
>>>>>>> refs/remotes/origin/cm-10.0
		for (i = 0; i < DRM_CONNECTOR_MAX_PROPERTY; i++) {
			if (connector->property_ids[i] != 0) {
				if (put_user(connector->property_ids[i],
					     prop_ptr + copied)) {
					ret = -EFAULT;
					goto out;
				}

				if (put_user(connector->property_values[i],
					     prop_values + copied)) {
					ret = -EFAULT;
					goto out;
				}
				copied++;
			}
=======
		prop_ptr = (uint32_t __user *)(unsigned long)(out_resp->props_ptr);
		prop_values = (uint64_t __user *)(unsigned long)(out_resp->prop_values_ptr);
		for (i = 0; i < connector->properties.count; i++) {
			if (put_user(connector->properties.ids[i],
				     prop_ptr + copied)) {
				ret = -EFAULT;
				goto out;
			}

			if (put_user(connector->properties.values[i],
				     prop_values + copied)) {
				ret = -EFAULT;
				goto out;
			}
			copied++;
>>>>>>> refs/remotes/origin/master
		}
	}
	out_resp->count_props = props_count;

	if ((out_resp->count_encoders >= encoders_count) && encoders_count) {
		copied = 0;
<<<<<<< HEAD
<<<<<<< HEAD
		encoder_ptr = (uint32_t *)(unsigned long)(out_resp->encoders_ptr);
=======
		encoder_ptr = (uint32_t __user *)(unsigned long)(out_resp->encoders_ptr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		encoder_ptr = (uint32_t __user *)(unsigned long)(out_resp->encoders_ptr);
>>>>>>> refs/remotes/origin/master
		for (i = 0; i < DRM_CONNECTOR_MAX_ENCODER; i++) {
			if (connector->encoder_ids[i] != 0) {
				if (put_user(connector->encoder_ids[i],
					     encoder_ptr + copied)) {
					ret = -EFAULT;
					goto out;
				}
				copied++;
			}
		}
	}
	out_resp->count_encoders = encoders_count;

out:
	mutex_unlock(&dev->mode_config.mutex);
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	return ret;
}

int drm_mode_getencoder(struct drm_device *dev, void *data,
			struct drm_file *file_priv)
{
	struct drm_mode_get_encoder *enc_resp = data;
	struct drm_mode_object *obj;
	struct drm_encoder *encoder;
	int ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, enc_resp->encoder_id,
				   DRM_MODE_OBJECT_ENCODER);
	if (!obj) {
		ret = -EINVAL;
=======
	drm_modeset_lock_all(dev);
	obj = drm_mode_object_find(dev, enc_resp->encoder_id,
				   DRM_MODE_OBJECT_ENCODER);
	if (!obj) {
		ret = -ENOENT;
>>>>>>> refs/remotes/origin/master
		goto out;
	}
	encoder = obj_to_encoder(obj);

	if (encoder->crtc)
		enc_resp->crtc_id = encoder->crtc->base.id;
	else
		enc_resp->crtc_id = 0;
	enc_resp->encoder_type = encoder->encoder_type;
	enc_resp->encoder_id = encoder->base.id;
	enc_resp->possible_crtcs = encoder->possible_crtcs;
	enc_resp->possible_clones = encoder->possible_clones;

out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
 * drm_mode_getplane_res - get plane info
 * @dev: DRM device
 * @data: ioctl data
 * @file_priv: DRM file info
 *
<<<<<<< HEAD
 * LOCKING:
 * Takes mode config lock.
 *
=======
>>>>>>> refs/remotes/origin/master
 * Return an plane count and set of IDs.
 */
int drm_mode_getplane_res(struct drm_device *dev, void *data,
			    struct drm_file *file_priv)
{
	struct drm_mode_get_plane_res *plane_resp = data;
	struct drm_mode_config *config;
	struct drm_plane *plane;
	uint32_t __user *plane_ptr;
	int copied = 0, ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
=======
	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master
	config = &dev->mode_config;

	/*
	 * This ioctl is called twice, once to determine how much space is
	 * needed, and the 2nd time to fill it.
	 */
	if (config->num_plane &&
	    (plane_resp->count_planes >= config->num_plane)) {
		plane_ptr = (uint32_t __user *)(unsigned long)plane_resp->plane_id_ptr;

		list_for_each_entry(plane, &config->plane_list, head) {
			if (put_user(plane->base.id, plane_ptr + copied)) {
				ret = -EFAULT;
				goto out;
			}
			copied++;
		}
	}
	plane_resp->count_planes = config->num_plane;

out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

/**
 * drm_mode_getplane - get plane info
 * @dev: DRM device
 * @data: ioctl data
 * @file_priv: DRM file info
 *
<<<<<<< HEAD
 * LOCKING:
 * Takes mode config lock.
 *
=======
>>>>>>> refs/remotes/origin/master
 * Return plane info, including formats supported, gamma size, any
 * current fb, etc.
 */
int drm_mode_getplane(struct drm_device *dev, void *data,
			struct drm_file *file_priv)
{
	struct drm_mode_get_plane *plane_resp = data;
	struct drm_mode_object *obj;
	struct drm_plane *plane;
	uint32_t __user *format_ptr;
	int ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
=======
	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master
	obj = drm_mode_object_find(dev, plane_resp->plane_id,
				   DRM_MODE_OBJECT_PLANE);
	if (!obj) {
		ret = -ENOENT;
		goto out;
	}
	plane = obj_to_plane(obj);

	if (plane->crtc)
		plane_resp->crtc_id = plane->crtc->base.id;
	else
		plane_resp->crtc_id = 0;

	if (plane->fb)
		plane_resp->fb_id = plane->fb->base.id;
	else
		plane_resp->fb_id = 0;

	plane_resp->plane_id = plane->base.id;
	plane_resp->possible_crtcs = plane->possible_crtcs;
<<<<<<< HEAD
	plane_resp->gamma_size = plane->gamma_size;
=======
	plane_resp->gamma_size = 0;
>>>>>>> refs/remotes/origin/master

	/*
	 * This ioctl is called twice, once to determine how much space is
	 * needed, and the 2nd time to fill it.
	 */
	if (plane->format_count &&
	    (plane_resp->count_format_types >= plane->format_count)) {
		format_ptr = (uint32_t __user *)(unsigned long)plane_resp->format_type_ptr;
		if (copy_to_user(format_ptr,
				 plane->format_types,
				 sizeof(uint32_t) * plane->format_count)) {
			ret = -EFAULT;
			goto out;
		}
	}
	plane_resp->count_format_types = plane->format_count;

out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

/**
 * drm_mode_setplane - set up or tear down an plane
 * @dev: DRM device
 * @data: ioctl data*
<<<<<<< HEAD
 * @file_prive: DRM file info
 *
 * LOCKING:
 * Takes mode config lock.
=======
 * @file_priv: DRM file info
>>>>>>> refs/remotes/origin/master
 *
 * Set plane info, including placement, fb, scaling, and other factors.
 * Or pass a NULL fb to disable.
 */
int drm_mode_setplane(struct drm_device *dev, void *data,
			struct drm_file *file_priv)
{
	struct drm_mode_set_plane *plane_req = data;
	struct drm_mode_object *obj;
	struct drm_plane *plane;
	struct drm_crtc *crtc;
<<<<<<< HEAD
	struct drm_framebuffer *fb;
=======
	struct drm_framebuffer *fb = NULL, *old_fb = NULL;
>>>>>>> refs/remotes/origin/master
	int ret = 0;
	unsigned int fb_width, fb_height;
	int i;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);

=======
>>>>>>> refs/remotes/origin/master
	/*
	 * First, find the plane, crtc, and fb objects.  If not available,
	 * we don't bother to call the driver.
	 */
	obj = drm_mode_object_find(dev, plane_req->plane_id,
				   DRM_MODE_OBJECT_PLANE);
	if (!obj) {
		DRM_DEBUG_KMS("Unknown plane ID %d\n",
			      plane_req->plane_id);
<<<<<<< HEAD
		ret = -ENOENT;
		goto out;
=======
		return -ENOENT;
>>>>>>> refs/remotes/origin/master
	}
	plane = obj_to_plane(obj);

	/* No fb means shut it down */
	if (!plane_req->fb_id) {
<<<<<<< HEAD
		plane->funcs->disable_plane(plane);
		plane->crtc = NULL;
		plane->fb = NULL;
=======
		drm_modeset_lock_all(dev);
		old_fb = plane->fb;
		plane->funcs->disable_plane(plane);
		plane->crtc = NULL;
		plane->fb = NULL;
		drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	obj = drm_mode_object_find(dev, plane_req->crtc_id,
				   DRM_MODE_OBJECT_CRTC);
	if (!obj) {
		DRM_DEBUG_KMS("Unknown crtc ID %d\n",
			      plane_req->crtc_id);
		ret = -ENOENT;
		goto out;
	}
	crtc = obj_to_crtc(obj);

<<<<<<< HEAD
	obj = drm_mode_object_find(dev, plane_req->fb_id,
				   DRM_MODE_OBJECT_FB);
	if (!obj) {
=======
	fb = drm_framebuffer_lookup(dev, plane_req->fb_id);
	if (!fb) {
>>>>>>> refs/remotes/origin/master
		DRM_DEBUG_KMS("Unknown framebuffer ID %d\n",
			      plane_req->fb_id);
		ret = -ENOENT;
		goto out;
	}
<<<<<<< HEAD
	fb = obj_to_fb(obj);
=======
>>>>>>> refs/remotes/origin/master

	/* Check whether this plane supports the fb pixel format. */
	for (i = 0; i < plane->format_count; i++)
		if (fb->pixel_format == plane->format_types[i])
			break;
	if (i == plane->format_count) {
<<<<<<< HEAD
		DRM_DEBUG_KMS("Invalid pixel format 0x%08x\n", fb->pixel_format);
=======
		DRM_DEBUG_KMS("Invalid pixel format %s\n",
			      drm_get_format_name(fb->pixel_format));
>>>>>>> refs/remotes/origin/master
		ret = -EINVAL;
		goto out;
	}

	fb_width = fb->width << 16;
	fb_height = fb->height << 16;

	/* Make sure source coordinates are inside the fb. */
	if (plane_req->src_w > fb_width ||
	    plane_req->src_x > fb_width - plane_req->src_w ||
	    plane_req->src_h > fb_height ||
	    plane_req->src_y > fb_height - plane_req->src_h) {
		DRM_DEBUG_KMS("Invalid source coordinates "
			      "%u.%06ux%u.%06u+%u.%06u+%u.%06u\n",
			      plane_req->src_w >> 16,
			      ((plane_req->src_w & 0xffff) * 15625) >> 10,
			      plane_req->src_h >> 16,
			      ((plane_req->src_h & 0xffff) * 15625) >> 10,
			      plane_req->src_x >> 16,
			      ((plane_req->src_x & 0xffff) * 15625) >> 10,
			      plane_req->src_y >> 16,
			      ((plane_req->src_y & 0xffff) * 15625) >> 10);
		ret = -ENOSPC;
		goto out;
	}

	/* Give drivers some help against integer overflows */
	if (plane_req->crtc_w > INT_MAX ||
	    plane_req->crtc_x > INT_MAX - (int32_t) plane_req->crtc_w ||
	    plane_req->crtc_h > INT_MAX ||
	    plane_req->crtc_y > INT_MAX - (int32_t) plane_req->crtc_h) {
		DRM_DEBUG_KMS("Invalid CRTC coordinates %ux%u+%d+%d\n",
			      plane_req->crtc_w, plane_req->crtc_h,
			      plane_req->crtc_x, plane_req->crtc_y);
		ret = -ERANGE;
		goto out;
	}

<<<<<<< HEAD
=======
	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master
	ret = plane->funcs->update_plane(plane, crtc, fb,
					 plane_req->crtc_x, plane_req->crtc_y,
					 plane_req->crtc_w, plane_req->crtc_h,
					 plane_req->src_x, plane_req->src_y,
					 plane_req->src_w, plane_req->src_h);
	if (!ret) {
<<<<<<< HEAD
		plane->crtc = crtc;
		plane->fb = fb;
	}

out:
	mutex_unlock(&dev->mode_config.mutex);

	return ret;
}

/**
>>>>>>> refs/remotes/origin/cm-10.0
 * drm_mode_setcrtc - set CRTC configuration
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * LOCKING:
<<<<<<< HEAD
 * Caller? (FIXME)
=======
 * Takes mode config lock.
>>>>>>> refs/remotes/origin/cm-10.0
=======
		old_fb = plane->fb;
		plane->crtc = crtc;
		plane->fb = fb;
		fb = NULL;
	}
	drm_modeset_unlock_all(dev);

out:
	if (fb)
		drm_framebuffer_unreference(fb);
	if (old_fb)
		drm_framebuffer_unreference(old_fb);

	return ret;
}

/**
 * drm_mode_set_config_internal - helper to call ->set_config
 * @set: modeset config to set
 *
 * This is a little helper to wrap internal calls to the ->set_config driver
 * interface. The only thing it adds is correct refcounting dance.
 */
int drm_mode_set_config_internal(struct drm_mode_set *set)
{
	struct drm_crtc *crtc = set->crtc;
	struct drm_framebuffer *fb;
	struct drm_crtc *tmp;
	int ret;

	/*
	 * NOTE: ->set_config can also disable other crtcs (if we steal all
	 * connectors from it), hence we need to refcount the fbs across all
	 * crtcs. Atomic modeset will have saner semantics ...
	 */
	list_for_each_entry(tmp, &crtc->dev->mode_config.crtc_list, head)
		tmp->old_fb = tmp->fb;

	fb = set->fb;

	ret = crtc->funcs->set_config(set);
	if (ret == 0) {
		/* crtc->fb must be updated by ->set_config, enforces this. */
		WARN_ON(fb != crtc->fb);
	}

	list_for_each_entry(tmp, &crtc->dev->mode_config.crtc_list, head) {
		if (tmp->fb)
			drm_framebuffer_reference(tmp->fb);
		if (tmp->old_fb)
			drm_framebuffer_unreference(tmp->old_fb);
	}

	return ret;
}
EXPORT_SYMBOL(drm_mode_set_config_internal);

/*
 * Checks that the framebuffer is big enough for the CRTC viewport
 * (x, y, hdisplay, vdisplay)
 */
static int drm_crtc_check_viewport(const struct drm_crtc *crtc,
				   int x, int y,
				   const struct drm_display_mode *mode,
				   const struct drm_framebuffer *fb)

{
	int hdisplay, vdisplay;

	hdisplay = mode->hdisplay;
	vdisplay = mode->vdisplay;

	if (drm_mode_is_stereo(mode)) {
		struct drm_display_mode adjusted = *mode;

		drm_mode_set_crtcinfo(&adjusted, CRTC_STEREO_DOUBLE);
		hdisplay = adjusted.crtc_hdisplay;
		vdisplay = adjusted.crtc_vdisplay;
	}

	if (crtc->invert_dimensions)
		swap(hdisplay, vdisplay);

	if (hdisplay > fb->width ||
	    vdisplay > fb->height ||
	    x > fb->width - hdisplay ||
	    y > fb->height - vdisplay) {
		DRM_DEBUG_KMS("Invalid fb size %ux%u for CRTC viewport %ux%u+%d+%d%s.\n",
			      fb->width, fb->height, hdisplay, vdisplay, x, y,
			      crtc->invert_dimensions ? " (inverted)" : "");
		return -ENOSPC;
	}

	return 0;
}

/**
 * drm_mode_setcrtc - set CRTC configuration
 * @dev: drm device for the ioctl
 * @data: data pointer for the ioctl
 * @file_priv: drm file for the ioctl call
>>>>>>> refs/remotes/origin/master
 *
 * Build a new CRTC configuration based on user request.
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_setcrtc(struct drm_device *dev, void *data,
		     struct drm_file *file_priv)
{
	struct drm_mode_config *config = &dev->mode_config;
	struct drm_mode_crtc *crtc_req = data;
	struct drm_mode_object *obj;
<<<<<<< HEAD
<<<<<<< HEAD
	struct drm_crtc *crtc, *crtcfb;
=======
	struct drm_crtc *crtc;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct drm_crtc *crtc;
>>>>>>> refs/remotes/origin/master
	struct drm_connector **connector_set = NULL, *connector;
	struct drm_framebuffer *fb = NULL;
	struct drm_display_mode *mode = NULL;
	struct drm_mode_set set;
	uint32_t __user *set_connectors_ptr;
<<<<<<< HEAD
	int ret = 0;
=======
	int ret;
>>>>>>> refs/remotes/origin/master
	int i;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/* For some reason crtc x/y offsets are signed internally. */
	if (crtc_req->x > INT_MAX || crtc_req->y > INT_MAX)
		return -ERANGE;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_lock(&dev->mode_config.mutex);
=======
	drm_modeset_lock_all(dev);
>>>>>>> refs/remotes/origin/master
	obj = drm_mode_object_find(dev, crtc_req->crtc_id,
				   DRM_MODE_OBJECT_CRTC);
	if (!obj) {
		DRM_DEBUG_KMS("Unknown CRTC ID %d\n", crtc_req->crtc_id);
<<<<<<< HEAD
		ret = -EINVAL;
=======
		ret = -ENOENT;
>>>>>>> refs/remotes/origin/master
		goto out;
	}
	crtc = obj_to_crtc(obj);
	DRM_DEBUG_KMS("[CRTC:%d]\n", crtc->base.id);

	if (crtc_req->mode_valid) {
		/* If we have a mode we need a framebuffer. */
		/* If we pass -1, set the mode with the currently bound fb */
		if (crtc_req->fb_id == -1) {
<<<<<<< HEAD
<<<<<<< HEAD
			list_for_each_entry(crtcfb,
					    &dev->mode_config.crtc_list, head) {
				if (crtcfb == crtc) {
					DRM_DEBUG_KMS("Using current fb for "
							"setmode\n");
					fb = crtc->fb;
				}
			}
=======
=======
>>>>>>> refs/remotes/origin/master
			if (!crtc->fb) {
				DRM_DEBUG_KMS("CRTC doesn't have current FB\n");
				ret = -EINVAL;
				goto out;
			}
			fb = crtc->fb;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		} else {
			obj = drm_mode_object_find(dev, crtc_req->fb_id,
						   DRM_MODE_OBJECT_FB);
			if (!obj) {
				DRM_DEBUG_KMS("Unknown FB ID%d\n",
						crtc_req->fb_id);
				ret = -EINVAL;
				goto out;
			}
			fb = obj_to_fb(obj);
		}

		mode = drm_mode_create(dev);
<<<<<<< HEAD
		drm_crtc_convert_umode(mode, &crtc_req->mode);
		drm_mode_set_crtcinfo(mode, CRTC_INTERLACE_HALVE_V);
=======
=======
			/* Make refcounting symmetric with the lookup path. */
			drm_framebuffer_reference(fb);
		} else {
			fb = drm_framebuffer_lookup(dev, crtc_req->fb_id);
			if (!fb) {
				DRM_DEBUG_KMS("Unknown FB ID%d\n",
						crtc_req->fb_id);
				ret = -ENOENT;
				goto out;
			}
		}

		mode = drm_mode_create(dev);
>>>>>>> refs/remotes/origin/master
		if (!mode) {
			ret = -ENOMEM;
			goto out;
		}

		ret = drm_crtc_convert_umode(mode, &crtc_req->mode);
		if (ret) {
			DRM_DEBUG_KMS("Invalid mode\n");
			goto out;
		}

		drm_mode_set_crtcinfo(mode, CRTC_INTERLACE_HALVE_V);

<<<<<<< HEAD
		if (mode->hdisplay > fb->width ||
		    mode->vdisplay > fb->height ||
		    crtc_req->x > fb->width - mode->hdisplay ||
		    crtc_req->y > fb->height - mode->vdisplay) {
			DRM_DEBUG_KMS("Invalid CRTC viewport %ux%u+%u+%u for fb size %ux%u.\n",
				      mode->hdisplay, mode->vdisplay,
				      crtc_req->x, crtc_req->y,
				      fb->width, fb->height);
			ret = -ENOSPC;
			goto out;
		}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = drm_crtc_check_viewport(crtc, crtc_req->x, crtc_req->y,
					      mode, fb);
		if (ret)
			goto out;

>>>>>>> refs/remotes/origin/master
	}

	if (crtc_req->count_connectors == 0 && mode) {
		DRM_DEBUG_KMS("Count connectors is 0 but mode set\n");
		ret = -EINVAL;
		goto out;
	}

	if (crtc_req->count_connectors > 0 && (!mode || !fb)) {
		DRM_DEBUG_KMS("Count connectors is %d but no mode or fb set\n",
			  crtc_req->count_connectors);
		ret = -EINVAL;
		goto out;
	}

	if (crtc_req->count_connectors > 0) {
		u32 out_id;

		/* Avoid unbounded kernel memory allocation */
		if (crtc_req->count_connectors > config->num_connector) {
			ret = -EINVAL;
			goto out;
		}

		connector_set = kmalloc(crtc_req->count_connectors *
					sizeof(struct drm_connector *),
					GFP_KERNEL);
		if (!connector_set) {
			ret = -ENOMEM;
			goto out;
		}

		for (i = 0; i < crtc_req->count_connectors; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
			set_connectors_ptr = (uint32_t *)(unsigned long)crtc_req->set_connectors_ptr;
=======
			set_connectors_ptr = (uint32_t __user *)(unsigned long)crtc_req->set_connectors_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			set_connectors_ptr = (uint32_t __user *)(unsigned long)crtc_req->set_connectors_ptr;
>>>>>>> refs/remotes/origin/master
			if (get_user(out_id, &set_connectors_ptr[i])) {
				ret = -EFAULT;
				goto out;
			}

			obj = drm_mode_object_find(dev, out_id,
						   DRM_MODE_OBJECT_CONNECTOR);
			if (!obj) {
				DRM_DEBUG_KMS("Connector id %d unknown\n",
						out_id);
<<<<<<< HEAD
				ret = -EINVAL;
=======
				ret = -ENOENT;
>>>>>>> refs/remotes/origin/master
				goto out;
			}
			connector = obj_to_connector(obj);
			DRM_DEBUG_KMS("[CONNECTOR:%d:%s]\n",
					connector->base.id,
					drm_get_connector_name(connector));

			connector_set[i] = connector;
		}
	}

	set.crtc = crtc;
	set.x = crtc_req->x;
	set.y = crtc_req->y;
	set.mode = mode;
	set.connectors = connector_set;
	set.num_connectors = crtc_req->count_connectors;
	set.fb = fb;
<<<<<<< HEAD
	ret = crtc->funcs->set_config(&set);

out:
	kfree(connector_set);
<<<<<<< HEAD
=======
	drm_mode_destroy(dev, mode);
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
}

int drm_mode_cursor_ioctl(struct drm_device *dev,
			void *data, struct drm_file *file_priv)
{
	struct drm_mode_cursor *req = data;
=======
	ret = drm_mode_set_config_internal(&set);

out:
	if (fb)
		drm_framebuffer_unreference(fb);

	kfree(connector_set);
	drm_mode_destroy(dev, mode);
	drm_modeset_unlock_all(dev);
	return ret;
}

static int drm_mode_cursor_common(struct drm_device *dev,
				  struct drm_mode_cursor2 *req,
				  struct drm_file *file_priv)
{
>>>>>>> refs/remotes/origin/master
	struct drm_mode_object *obj;
	struct drm_crtc *crtc;
	int ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (!req->flags) {
		DRM_ERROR("no operation set\n");
=======
	if (!req->flags || (~DRM_MODE_CURSOR_FLAGS & req->flags))
>>>>>>> refs/remotes/origin/cm-11.0
		return -EINVAL;
	}
=======
	if (!req->flags || (~DRM_MODE_CURSOR_FLAGS & req->flags))
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, req->crtc_id, DRM_MODE_OBJECT_CRTC);
	if (!obj) {
		DRM_DEBUG_KMS("Unknown CRTC ID %d\n", req->crtc_id);
		ret = -EINVAL;
		goto out;
	}
	crtc = obj_to_crtc(obj);

	if (req->flags & DRM_MODE_CURSOR_BO) {
		if (!crtc->funcs->cursor_set) {
<<<<<<< HEAD
			DRM_ERROR("crtc does not support cursor\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!req->flags || (~DRM_MODE_CURSOR_FLAGS & req->flags))
		return -EINVAL;

	obj = drm_mode_object_find(dev, req->crtc_id, DRM_MODE_OBJECT_CRTC);
	if (!obj) {
		DRM_DEBUG_KMS("Unknown CRTC ID %d\n", req->crtc_id);
		return -ENOENT;
	}
	crtc = obj_to_crtc(obj);

	mutex_lock(&crtc->mutex);
	if (req->flags & DRM_MODE_CURSOR_BO) {
		if (!crtc->funcs->cursor_set && !crtc->funcs->cursor_set2) {
>>>>>>> refs/remotes/origin/master
			ret = -ENXIO;
			goto out;
		}
		/* Turns off the cursor if handle is 0 */
<<<<<<< HEAD
		ret = crtc->funcs->cursor_set(crtc, file_priv, req->handle,
					      req->width, req->height);
=======
		if (crtc->funcs->cursor_set2)
			ret = crtc->funcs->cursor_set2(crtc, file_priv, req->handle,
						      req->width, req->height, req->hot_x, req->hot_y);
		else
			ret = crtc->funcs->cursor_set(crtc, file_priv, req->handle,
						      req->width, req->height);
>>>>>>> refs/remotes/origin/master
	}

	if (req->flags & DRM_MODE_CURSOR_MOVE) {
		if (crtc->funcs->cursor_move) {
			ret = crtc->funcs->cursor_move(crtc, req->x, req->y);
		} else {
<<<<<<< HEAD
<<<<<<< HEAD
			DRM_ERROR("crtc does not support cursor\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			ret = -EFAULT;
			goto out;
		}
	}
out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
}

<<<<<<< HEAD
=======
=======
	mutex_unlock(&crtc->mutex);

	return ret;

}
int drm_mode_cursor_ioctl(struct drm_device *dev,
			void *data, struct drm_file *file_priv)
{
	struct drm_mode_cursor *req = data;
	struct drm_mode_cursor2 new_req;

	memcpy(&new_req, req, sizeof(struct drm_mode_cursor));
	new_req.hot_x = new_req.hot_y = 0;

	return drm_mode_cursor_common(dev, &new_req, file_priv);
}

int drm_mode_cursor2_ioctl(struct drm_device *dev,
			   void *data, struct drm_file *file_priv)
{
	struct drm_mode_cursor2 *req = data;
	return drm_mode_cursor_common(dev, req, file_priv);
}

>>>>>>> refs/remotes/origin/master
/* Original addfb only supported RGB formats, so figure out which one */
uint32_t drm_mode_legacy_fb_format(uint32_t bpp, uint32_t depth)
{
	uint32_t fmt;

	switch (bpp) {
	case 8:
		fmt = DRM_FORMAT_C8;
		break;
	case 16:
		if (depth == 15)
			fmt = DRM_FORMAT_XRGB1555;
		else
			fmt = DRM_FORMAT_RGB565;
		break;
	case 24:
		fmt = DRM_FORMAT_RGB888;
		break;
	case 32:
		if (depth == 24)
			fmt = DRM_FORMAT_XRGB8888;
		else if (depth == 30)
			fmt = DRM_FORMAT_XRGB2101010;
		else
			fmt = DRM_FORMAT_ARGB8888;
		break;
	default:
		DRM_ERROR("bad bpp, assuming x8r8g8b8 pixel format\n");
		fmt = DRM_FORMAT_XRGB8888;
		break;
	}

	return fmt;
}
EXPORT_SYMBOL(drm_mode_legacy_fb_format);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/**
 * drm_mode_addfb - add an FB to the graphics configuration
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * LOCKING:
 * Takes mode config lock.
=======
/**
 * drm_mode_addfb - add an FB to the graphics configuration
 * @dev: drm device for the ioctl
 * @data: data pointer for the ioctl
 * @file_priv: drm file for the ioctl call
>>>>>>> refs/remotes/origin/master
 *
 * Add a new FB to the specified CRTC, given a user request.
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_addfb(struct drm_device *dev,
		   void *data, struct drm_file *file_priv)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct drm_mode_fb_cmd *r = data;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct drm_mode_fb_cmd *or = data;
	struct drm_mode_fb_cmd2 r = {};
	struct drm_mode_config *config = &dev->mode_config;
	struct drm_framebuffer *fb;
	int ret = 0;

	/* Use new struct with format internally */
	r.fb_id = or->fb_id;
	r.width = or->width;
	r.height = or->height;
	r.pitches[0] = or->pitch;
	r.pixel_format = drm_mode_legacy_fb_format(or->bpp, or->depth);
	r.handles[0] = or->handle;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

	if ((config->min_width > r.width) || (r.width > config->max_width))
		return -EINVAL;

	if ((config->min_height > r.height) || (r.height > config->max_height))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);

	/* TODO check buffer is sufficiently large */
	/* TODO setup destructor callback */

	fb = dev->mode_config.funcs->fb_create(dev, file_priv, &r);
	if (IS_ERR(fb)) {
		DRM_ERROR("could not create framebuffer\n");
		ret = PTR_ERR(fb);
		goto out;
	}

	or->fb_id = fb->base.id;
	list_add(&fb->filp_head, &file_priv->fbs);
	DRM_DEBUG_KMS("[FB:%d]\n", fb->base.id);

out:
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
}

static int format_check(struct drm_mode_fb_cmd2 *r)
=======
	fb = dev->mode_config.funcs->fb_create(dev, file_priv, &r);
	if (IS_ERR(fb)) {
		DRM_DEBUG_KMS("could not create framebuffer\n");
		return PTR_ERR(fb);
	}

	mutex_lock(&file_priv->fbs_lock);
	or->fb_id = fb->base.id;
	list_add(&fb->filp_head, &file_priv->fbs);
	DRM_DEBUG_KMS("[FB:%d]\n", fb->base.id);
	mutex_unlock(&file_priv->fbs_lock);

	return ret;
}

static int format_check(const struct drm_mode_fb_cmd2 *r)
>>>>>>> refs/remotes/origin/master
{
	uint32_t format = r->pixel_format & ~DRM_FORMAT_BIG_ENDIAN;

	switch (format) {
	case DRM_FORMAT_C8:
	case DRM_FORMAT_RGB332:
	case DRM_FORMAT_BGR233:
	case DRM_FORMAT_XRGB4444:
	case DRM_FORMAT_XBGR4444:
	case DRM_FORMAT_RGBX4444:
	case DRM_FORMAT_BGRX4444:
	case DRM_FORMAT_ARGB4444:
	case DRM_FORMAT_ABGR4444:
	case DRM_FORMAT_RGBA4444:
	case DRM_FORMAT_BGRA4444:
	case DRM_FORMAT_XRGB1555:
	case DRM_FORMAT_XBGR1555:
	case DRM_FORMAT_RGBX5551:
	case DRM_FORMAT_BGRX5551:
	case DRM_FORMAT_ARGB1555:
	case DRM_FORMAT_ABGR1555:
	case DRM_FORMAT_RGBA5551:
	case DRM_FORMAT_BGRA5551:
	case DRM_FORMAT_RGB565:
	case DRM_FORMAT_BGR565:
	case DRM_FORMAT_RGB888:
	case DRM_FORMAT_BGR888:
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_RGBX8888:
	case DRM_FORMAT_BGRX8888:
	case DRM_FORMAT_ARGB8888:
	case DRM_FORMAT_ABGR8888:
	case DRM_FORMAT_RGBA8888:
	case DRM_FORMAT_BGRA8888:
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_XBGR2101010:
	case DRM_FORMAT_RGBX1010102:
	case DRM_FORMAT_BGRX1010102:
	case DRM_FORMAT_ARGB2101010:
	case DRM_FORMAT_ABGR2101010:
	case DRM_FORMAT_RGBA1010102:
	case DRM_FORMAT_BGRA1010102:
	case DRM_FORMAT_YUYV:
	case DRM_FORMAT_YVYU:
	case DRM_FORMAT_UYVY:
	case DRM_FORMAT_VYUY:
	case DRM_FORMAT_AYUV:
	case DRM_FORMAT_NV12:
	case DRM_FORMAT_NV21:
	case DRM_FORMAT_NV16:
	case DRM_FORMAT_NV61:
<<<<<<< HEAD
=======
	case DRM_FORMAT_NV24:
	case DRM_FORMAT_NV42:
>>>>>>> refs/remotes/origin/master
	case DRM_FORMAT_YUV410:
	case DRM_FORMAT_YVU410:
	case DRM_FORMAT_YUV411:
	case DRM_FORMAT_YVU411:
	case DRM_FORMAT_YUV420:
	case DRM_FORMAT_YVU420:
	case DRM_FORMAT_YUV422:
	case DRM_FORMAT_YVU422:
	case DRM_FORMAT_YUV444:
	case DRM_FORMAT_YVU444:
		return 0;
	default:
<<<<<<< HEAD
		return -EINVAL;
	}
=======
		DRM_DEBUG_KMS("invalid pixel format %s\n",
			      drm_get_format_name(r->pixel_format));
		return -EINVAL;
	}
}

static int framebuffer_check(const struct drm_mode_fb_cmd2 *r)
{
	int ret, hsub, vsub, num_planes, i;

	ret = format_check(r);
	if (ret) {
		DRM_DEBUG_KMS("bad framebuffer format %s\n",
			      drm_get_format_name(r->pixel_format));
		return ret;
	}

	hsub = drm_format_horz_chroma_subsampling(r->pixel_format);
	vsub = drm_format_vert_chroma_subsampling(r->pixel_format);
	num_planes = drm_format_num_planes(r->pixel_format);

	if (r->width == 0 || r->width % hsub) {
		DRM_DEBUG_KMS("bad framebuffer width %u\n", r->height);
		return -EINVAL;
	}

	if (r->height == 0 || r->height % vsub) {
		DRM_DEBUG_KMS("bad framebuffer height %u\n", r->height);
		return -EINVAL;
	}

	for (i = 0; i < num_planes; i++) {
		unsigned int width = r->width / (i != 0 ? hsub : 1);
		unsigned int height = r->height / (i != 0 ? vsub : 1);
		unsigned int cpp = drm_format_plane_cpp(r->pixel_format, i);

		if (!r->handles[i]) {
			DRM_DEBUG_KMS("no buffer object handle for plane %d\n", i);
			return -EINVAL;
		}

		if ((uint64_t) width * cpp > UINT_MAX)
			return -ERANGE;

		if ((uint64_t) height * r->pitches[i] + r->offsets[i] > UINT_MAX)
			return -ERANGE;

		if (r->pitches[i] < width * cpp) {
			DRM_DEBUG_KMS("bad pitch %u for plane %d\n", r->pitches[i], i);
			return -EINVAL;
		}
	}

	return 0;
>>>>>>> refs/remotes/origin/master
}

/**
 * drm_mode_addfb2 - add an FB to the graphics configuration
<<<<<<< HEAD
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * LOCKING:
 * Takes mode config lock.
=======
 * @dev: drm device for the ioctl
 * @data: data pointer for the ioctl
 * @file_priv: drm file for the ioctl call
>>>>>>> refs/remotes/origin/master
 *
 * Add a new FB to the specified CRTC, given a user request with format.
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_addfb2(struct drm_device *dev,
		    void *data, struct drm_file *file_priv)
{
	struct drm_mode_fb_cmd2 *r = data;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	struct drm_mode_config *config = &dev->mode_config;
	struct drm_framebuffer *fb;
	int ret = 0;
=======
	struct drm_mode_config *config = &dev->mode_config;
	struct drm_framebuffer *fb;
	int ret;
>>>>>>> refs/remotes/origin/master

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	if ((config->min_width > r->width) || (r->width > config->max_width)) {
<<<<<<< HEAD
		DRM_ERROR("mode new framebuffer width not within limits\n");
		return -EINVAL;
	}
	if ((config->min_height > r->height) || (r->height > config->max_height)) {
		DRM_ERROR("mode new framebuffer height not within limits\n");
		return -EINVAL;
	}

	mutex_lock(&dev->mode_config.mutex);

	/* TODO check buffer is sufficiently large */
	/* TODO setup destructor callback */
=======
		DRM_ERROR("bad framebuffer width %d, should be >= %d && <= %d\n",
=======
	if (r->flags & ~DRM_MODE_FB_INTERLACED) {
		DRM_DEBUG_KMS("bad framebuffer flags 0x%08x\n", r->flags);
		return -EINVAL;
	}

	if ((config->min_width > r->width) || (r->width > config->max_width)) {
		DRM_DEBUG_KMS("bad framebuffer width %d, should be >= %d && <= %d\n",
>>>>>>> refs/remotes/origin/master
			  r->width, config->min_width, config->max_width);
		return -EINVAL;
	}
	if ((config->min_height > r->height) || (r->height > config->max_height)) {
<<<<<<< HEAD
		DRM_ERROR("bad framebuffer height %d, should be >= %d && <= %d\n",
=======
		DRM_DEBUG_KMS("bad framebuffer height %d, should be >= %d && <= %d\n",
>>>>>>> refs/remotes/origin/master
			  r->height, config->min_height, config->max_height);
		return -EINVAL;
	}

<<<<<<< HEAD
	ret = format_check(r);
	if (ret) {
		DRM_ERROR("bad framebuffer format 0x%08x\n", r->pixel_format);
		return ret;
	}

	mutex_lock(&dev->mode_config.mutex);
>>>>>>> refs/remotes/origin/cm-10.0

	fb = dev->mode_config.funcs->fb_create(dev, file_priv, r);
	if (IS_ERR(fb)) {
		DRM_ERROR("could not create framebuffer\n");
		ret = PTR_ERR(fb);
		goto out;
	}

	r->fb_id = fb->base.id;
	list_add(&fb->filp_head, &file_priv->fbs);
	DRM_DEBUG_KMS("[FB:%d]\n", fb->base.id);

out:
	mutex_unlock(&dev->mode_config.mutex);
=======
	ret = framebuffer_check(r);
	if (ret)
		return ret;

	fb = dev->mode_config.funcs->fb_create(dev, file_priv, r);
	if (IS_ERR(fb)) {
		DRM_DEBUG_KMS("could not create framebuffer\n");
		return PTR_ERR(fb);
	}

	mutex_lock(&file_priv->fbs_lock);
	r->fb_id = fb->base.id;
	list_add(&fb->filp_head, &file_priv->fbs);
	DRM_DEBUG_KMS("[FB:%d]\n", fb->base.id);
	mutex_unlock(&file_priv->fbs_lock);


>>>>>>> refs/remotes/origin/master
	return ret;
}

/**
 * drm_mode_rmfb - remove an FB from the configuration
<<<<<<< HEAD
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * LOCKING:
 * Takes mode config lock.
=======
 * @dev: drm device for the ioctl
 * @data: data pointer for the ioctl
 * @file_priv: drm file for the ioctl call
>>>>>>> refs/remotes/origin/master
 *
 * Remove the FB specified by the user.
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_rmfb(struct drm_device *dev,
		   void *data, struct drm_file *file_priv)
{
<<<<<<< HEAD
	struct drm_mode_object *obj;
	struct drm_framebuffer *fb = NULL;
	struct drm_framebuffer *fbl = NULL;
	uint32_t *id = data;
	int ret = 0;
=======
	struct drm_framebuffer *fb = NULL;
	struct drm_framebuffer *fbl = NULL;
	uint32_t *id = data;
>>>>>>> refs/remotes/origin/master
	int found = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, *id, DRM_MODE_OBJECT_FB);
	/* TODO check that we really get a framebuffer back. */
	if (!obj) {
<<<<<<< HEAD
		DRM_ERROR("mode invalid framebuffer id\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0
		ret = -EINVAL;
		goto out;
	}
	fb = obj_to_fb(obj);
=======
	mutex_lock(&file_priv->fbs_lock);
	mutex_lock(&dev->mode_config.fb_lock);
	fb = __drm_framebuffer_lookup(dev, *id);
	if (!fb)
		goto fail_lookup;
>>>>>>> refs/remotes/origin/master

	list_for_each_entry(fbl, &file_priv->fbs, filp_head)
		if (fb == fbl)
			found = 1;
<<<<<<< HEAD

	if (!found) {
<<<<<<< HEAD
		DRM_ERROR("tried to remove a fb that we didn't own\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0
		ret = -EINVAL;
		goto out;
	}

	/* TODO release all crtc connected to the framebuffer */
	/* TODO unhock the destructor from the buffer object */

	list_del(&fb->filp_head);
	fb->funcs->destroy(fb);

out:
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
=======
	if (!found)
		goto fail_lookup;

	/* Mark fb as reaped, we still have a ref from fpriv->fbs. */
	__drm_framebuffer_unregister(dev, fb);

	list_del_init(&fb->filp_head);
	mutex_unlock(&dev->mode_config.fb_lock);
	mutex_unlock(&file_priv->fbs_lock);

	drm_framebuffer_remove(fb);

	return 0;

fail_lookup:
	mutex_unlock(&dev->mode_config.fb_lock);
	mutex_unlock(&file_priv->fbs_lock);

	return -ENOENT;
>>>>>>> refs/remotes/origin/master
}

/**
 * drm_mode_getfb - get FB info
<<<<<<< HEAD
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * LOCKING:
<<<<<<< HEAD
 * Caller? (FIXME)
=======
 * Takes mode config lock.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @dev: drm device for the ioctl
 * @data: data pointer for the ioctl
 * @file_priv: drm file for the ioctl call
>>>>>>> refs/remotes/origin/master
 *
 * Lookup the FB given its ID and return info about it.
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_getfb(struct drm_device *dev,
		   void *data, struct drm_file *file_priv)
{
	struct drm_mode_fb_cmd *r = data;
<<<<<<< HEAD
	struct drm_mode_object *obj;
	struct drm_framebuffer *fb;
	int ret = 0;
=======
	struct drm_framebuffer *fb;
	int ret;
>>>>>>> refs/remotes/origin/master

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, r->fb_id, DRM_MODE_OBJECT_FB);
	if (!obj) {
<<<<<<< HEAD
		DRM_ERROR("invalid framebuffer id\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0
		ret = -EINVAL;
		goto out;
	}
	fb = obj_to_fb(obj);
=======
	fb = drm_framebuffer_lookup(dev, r->fb_id);
	if (!fb)
		return -ENOENT;
>>>>>>> refs/remotes/origin/master

	r->height = fb->height;
	r->width = fb->width;
	r->depth = fb->depth;
	r->bpp = fb->bits_per_pixel;
<<<<<<< HEAD
<<<<<<< HEAD
	r->pitch = fb->pitch;
=======
	r->pitch = fb->pitches[0];
>>>>>>> refs/remotes/origin/cm-10.0
	fb->funcs->create_handle(fb, file_priv, &r->handle);

out:
	mutex_unlock(&dev->mode_config.mutex);
=======
	r->pitch = fb->pitches[0];
	if (fb->funcs->create_handle) {
		if (file_priv->is_master || capable(CAP_SYS_ADMIN)) {
			ret = fb->funcs->create_handle(fb, file_priv,
						       &r->handle);
		} else {
			/* GET_FB() is an unprivileged ioctl so we must not
			 * return a buffer-handle to non-master processes! For
			 * backwards-compatibility reasons, we cannot make
			 * GET_FB() privileged, so just return an invalid handle
			 * for non-masters. */
			r->handle = 0;
			ret = 0;
		}
	} else {
		ret = -ENODEV;
	}

	drm_framebuffer_unreference(fb);

>>>>>>> refs/remotes/origin/master
	return ret;
}

int drm_mode_dirtyfb_ioctl(struct drm_device *dev,
			   void *data, struct drm_file *file_priv)
{
	struct drm_clip_rect __user *clips_ptr;
	struct drm_clip_rect *clips = NULL;
	struct drm_mode_fb_dirty_cmd *r = data;
<<<<<<< HEAD
	struct drm_mode_object *obj;
	struct drm_framebuffer *fb;
	unsigned flags;
	int num_clips;
	int ret = 0;
=======
	struct drm_framebuffer *fb;
	unsigned flags;
	int num_clips;
	int ret;
>>>>>>> refs/remotes/origin/master

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, r->fb_id, DRM_MODE_OBJECT_FB);
	if (!obj) {
<<<<<<< HEAD
		DRM_ERROR("invalid framebuffer id\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0
		ret = -EINVAL;
		goto out_err1;
	}
	fb = obj_to_fb(obj);

	num_clips = r->num_clips;
<<<<<<< HEAD
	clips_ptr = (struct drm_clip_rect *)(unsigned long)r->clips_ptr;
=======
	clips_ptr = (struct drm_clip_rect __user *)(unsigned long)r->clips_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	fb = drm_framebuffer_lookup(dev, r->fb_id);
	if (!fb)
		return -ENOENT;

	num_clips = r->num_clips;
	clips_ptr = (struct drm_clip_rect __user *)(unsigned long)r->clips_ptr;
>>>>>>> refs/remotes/origin/master

	if (!num_clips != !clips_ptr) {
		ret = -EINVAL;
		goto out_err1;
	}

	flags = DRM_MODE_FB_DIRTY_FLAGS & r->flags;

	/* If userspace annotates copy, clips must come in pairs */
	if (flags & DRM_MODE_FB_DIRTY_ANNOTATE_COPY && (num_clips % 2)) {
		ret = -EINVAL;
		goto out_err1;
	}

	if (num_clips && clips_ptr) {
		if (num_clips < 0 || num_clips > DRM_MODE_FB_DIRTY_MAX_CLIPS) {
			ret = -EINVAL;
			goto out_err1;
		}
		clips = kzalloc(num_clips * sizeof(*clips), GFP_KERNEL);
		if (!clips) {
			ret = -ENOMEM;
			goto out_err1;
		}

		ret = copy_from_user(clips, clips_ptr,
				     num_clips * sizeof(*clips));
		if (ret) {
			ret = -EFAULT;
			goto out_err2;
		}
	}

	if (fb->funcs->dirty) {
<<<<<<< HEAD
		ret = fb->funcs->dirty(fb, file_priv, flags, r->color,
				       clips, num_clips);
	} else {
		ret = -ENOSYS;
		goto out_err2;
=======
		drm_modeset_lock_all(dev);
		ret = fb->funcs->dirty(fb, file_priv, flags, r->color,
				       clips, num_clips);
		drm_modeset_unlock_all(dev);
	} else {
		ret = -ENOSYS;
>>>>>>> refs/remotes/origin/master
	}

out_err2:
	kfree(clips);
out_err1:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_framebuffer_unreference(fb);

>>>>>>> refs/remotes/origin/master
	return ret;
}


/**
 * drm_fb_release - remove and free the FBs on this file
<<<<<<< HEAD
 * @filp: file * from the ioctl
 *
 * LOCKING:
 * Takes mode config lock.
=======
 * @priv: drm file for the ioctl
>>>>>>> refs/remotes/origin/master
 *
 * Destroy all the FBs associated with @filp.
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
void drm_fb_release(struct drm_file *priv)
{
	struct drm_device *dev = priv->minor->dev;
	struct drm_framebuffer *fb, *tfb;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	list_for_each_entry_safe(fb, tfb, &priv->fbs, filp_head) {
		list_del(&fb->filp_head);
		fb->funcs->destroy(fb);
	}
	mutex_unlock(&dev->mode_config.mutex);
}

/**
 * drm_mode_attachmode - add a mode to the user mode list
 * @dev: DRM device
 * @connector: connector to add the mode to
 * @mode: mode to add
 *
 * Add @mode to @connector's user mode list.
 */
<<<<<<< HEAD
static int drm_mode_attachmode(struct drm_device *dev,
			       struct drm_connector *connector,
			       struct drm_display_mode *mode)
{
	int ret = 0;

	list_add_tail(&mode->head, &connector->user_modes);
	return ret;
}

int drm_mode_attachmode_crtc(struct drm_device *dev, struct drm_crtc *crtc,
			     struct drm_display_mode *mode)
{
	struct drm_connector *connector;
	int ret = 0;
	struct drm_display_mode *dup_mode;
	int need_dup = 0;
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		if (!connector->encoder)
			break;
		if (connector->encoder->crtc == crtc) {
			if (need_dup)
				dup_mode = drm_mode_duplicate(dev, mode);
			else
				dup_mode = mode;
			ret = drm_mode_attachmode(dev, connector, dup_mode);
			if (ret)
				return ret;
			need_dup = 1;
		}
	}
	return 0;
=======
static void drm_mode_attachmode(struct drm_device *dev,
				struct drm_connector *connector,
				struct drm_display_mode *mode)
{
	list_add_tail(&mode->head, &connector->user_modes);
}

int drm_mode_attachmode_crtc(struct drm_device *dev, struct drm_crtc *crtc,
			     const struct drm_display_mode *mode)
{
	struct drm_connector *connector;
	int ret = 0;
	struct drm_display_mode *dup_mode, *next;
	LIST_HEAD(list);

	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		if (!connector->encoder)
			continue;
		if (connector->encoder->crtc == crtc) {
			dup_mode = drm_mode_duplicate(dev, mode);
			if (!dup_mode) {
				ret = -ENOMEM;
				goto out;
			}
			list_add_tail(&dup_mode->head, &list);
		}
	}

	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		if (!connector->encoder)
			continue;
		if (connector->encoder->crtc == crtc)
			list_move_tail(list.next, &connector->user_modes);
	}

	WARN_ON(!list_empty(&list));

 out:
	list_for_each_entry_safe(dup_mode, next, &list, head)
		drm_mode_destroy(dev, dup_mode);

	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(drm_mode_attachmode_crtc);

static int drm_mode_detachmode(struct drm_device *dev,
			       struct drm_connector *connector,
			       struct drm_display_mode *mode)
{
	int found = 0;
	int ret = 0;
	struct drm_display_mode *match_mode, *t;

	list_for_each_entry_safe(match_mode, t, &connector->user_modes, head) {
		if (drm_mode_equal(match_mode, mode)) {
			list_del(&match_mode->head);
			drm_mode_destroy(dev, match_mode);
			found = 1;
			break;
		}
	}

	if (!found)
		ret = -EINVAL;

	return ret;
}

int drm_mode_detachmode_crtc(struct drm_device *dev, struct drm_display_mode *mode)
{
	struct drm_connector *connector;

	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		drm_mode_detachmode(dev, connector, mode);
	}
	return 0;
}
EXPORT_SYMBOL(drm_mode_detachmode_crtc);

/**
 * drm_fb_attachmode - Attach a user mode to an connector
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * This attaches a user specified mode to an connector.
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_attachmode_ioctl(struct drm_device *dev,
			      void *data, struct drm_file *file_priv)
{
	struct drm_mode_mode_cmd *mode_cmd = data;
	struct drm_connector *connector;
	struct drm_display_mode *mode;
	struct drm_mode_object *obj;
	struct drm_mode_modeinfo *umode = &mode_cmd->mode;
	int ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

	mutex_lock(&dev->mode_config.mutex);

	obj = drm_mode_object_find(dev, mode_cmd->connector_id, DRM_MODE_OBJECT_CONNECTOR);
	if (!obj) {
		ret = -EINVAL;
		goto out;
	}
	connector = obj_to_connector(obj);

	mode = drm_mode_create(dev);
	if (!mode) {
		ret = -ENOMEM;
		goto out;
	}

<<<<<<< HEAD
	drm_crtc_convert_umode(mode, umode);

	ret = drm_mode_attachmode(dev, connector, mode);
=======
	ret = drm_crtc_convert_umode(mode, umode);
	if (ret) {
		DRM_DEBUG_KMS("Invalid mode\n");
		drm_mode_destroy(dev, mode);
		goto out;
	}

	drm_mode_attachmode(dev, connector, mode);
>>>>>>> refs/remotes/origin/cm-10.0
out:
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
}


/**
 * drm_fb_detachmode - Detach a user specified mode from an connector
 * @inode: inode from the ioctl
 * @filp: file * from the ioctl
 * @cmd: cmd from ioctl
 * @arg: arg from ioctl
 *
 * Called by the user via ioctl.
 *
 * RETURNS:
 * Zero on success, errno on failure.
 */
int drm_mode_detachmode_ioctl(struct drm_device *dev,
			      void *data, struct drm_file *file_priv)
{
	struct drm_mode_object *obj;
	struct drm_mode_mode_cmd *mode_cmd = data;
	struct drm_connector *connector;
	struct drm_display_mode mode;
	struct drm_mode_modeinfo *umode = &mode_cmd->mode;
	int ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

	mutex_lock(&dev->mode_config.mutex);

	obj = drm_mode_object_find(dev, mode_cmd->connector_id, DRM_MODE_OBJECT_CONNECTOR);
	if (!obj) {
		ret = -EINVAL;
		goto out;
	}
	connector = obj_to_connector(obj);

<<<<<<< HEAD
	drm_crtc_convert_umode(&mode, umode);
=======
	ret = drm_crtc_convert_umode(&mode, umode);
	if (ret) {
		DRM_DEBUG_KMS("Invalid mode\n");
		goto out;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	ret = drm_mode_detachmode(dev, connector, &mode);
out:
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
}

struct drm_property *drm_property_create(struct drm_device *dev, int flags,
					 const char *name, int num_values)
{
	struct drm_property *property = NULL;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0

	property = kzalloc(sizeof(struct drm_property), GFP_KERNEL);
	if (!property)
		return NULL;

	if (num_values) {
		property->values = kzalloc(sizeof(uint64_t)*num_values, GFP_KERNEL);
		if (!property->values)
			goto fail;
	}

<<<<<<< HEAD
	drm_mode_object_get(dev, &property->base, DRM_MODE_OBJECT_PROPERTY);
=======
	ret = drm_mode_object_get(dev, &property->base, DRM_MODE_OBJECT_PROPERTY);
	if (ret)
		goto fail;

>>>>>>> refs/remotes/origin/cm-10.0
	property->flags = flags;
	property->num_values = num_values;
	INIT_LIST_HEAD(&property->enum_blob_list);

<<<<<<< HEAD
	if (name)
		strncpy(property->name, name, DRM_PROP_NAME_LEN);
=======
	if (name) {
		strncpy(property->name, name, DRM_PROP_NAME_LEN);
		property->name[DRM_PROP_NAME_LEN-1] = '\0';
	}
>>>>>>> refs/remotes/origin/cm-10.0

	list_add_tail(&property->head, &dev->mode_config.property_list);
	return property;
fail:
<<<<<<< HEAD
=======
	kfree(property->values);
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(property);
	return NULL;
}
EXPORT_SYMBOL(drm_property_create);

<<<<<<< HEAD
=======
struct drm_property *drm_property_create_enum(struct drm_device *dev, int flags,
					 const char *name,
					 const struct drm_prop_enum_list *props,
					 int num_values)
{
	struct drm_property *property;
	int i, ret;

	flags |= DRM_MODE_PROP_ENUM;

	property = drm_property_create(dev, flags, name, num_values);
	if (!property)
		return NULL;

	for (i = 0; i < num_values; i++) {
		ret = drm_property_add_enum(property, i,
				      props[i].type,
				      props[i].name);
		if (ret) {
			drm_property_destroy(dev, property);
			return NULL;
		}
	}

	return property;
}
EXPORT_SYMBOL(drm_property_create_enum);

struct drm_property *drm_property_create_range(struct drm_device *dev, int flags,
					 const char *name,
					 uint64_t min, uint64_t max)
=======
	mutex_lock(&priv->fbs_lock);
	list_for_each_entry_safe(fb, tfb, &priv->fbs, filp_head) {

		mutex_lock(&dev->mode_config.fb_lock);
		/* Mark fb as reaped, we still have a ref from fpriv->fbs. */
		__drm_framebuffer_unregister(dev, fb);
		mutex_unlock(&dev->mode_config.fb_lock);

		list_del_init(&fb->filp_head);

		/* This will also drop the fpriv->fbs reference. */
		drm_framebuffer_remove(fb);
	}
	mutex_unlock(&priv->fbs_lock);
}

struct drm_property *drm_property_create(struct drm_device *dev, int flags,
					 const char *name, int num_values)
{
	struct drm_property *property = NULL;
	int ret;

	property = kzalloc(sizeof(struct drm_property), GFP_KERNEL);
	if (!property)
		return NULL;

	if (num_values) {
		property->values = kzalloc(sizeof(uint64_t)*num_values, GFP_KERNEL);
		if (!property->values)
			goto fail;
	}

	ret = drm_mode_object_get(dev, &property->base, DRM_MODE_OBJECT_PROPERTY);
	if (ret)
		goto fail;

	property->flags = flags;
	property->num_values = num_values;
	INIT_LIST_HEAD(&property->enum_blob_list);

	if (name) {
		strncpy(property->name, name, DRM_PROP_NAME_LEN);
		property->name[DRM_PROP_NAME_LEN-1] = '\0';
	}

	list_add_tail(&property->head, &dev->mode_config.property_list);
	return property;
fail:
	kfree(property->values);
	kfree(property);
	return NULL;
}
EXPORT_SYMBOL(drm_property_create);

struct drm_property *drm_property_create_enum(struct drm_device *dev, int flags,
					 const char *name,
					 const struct drm_prop_enum_list *props,
					 int num_values)
{
	struct drm_property *property;
	int i, ret;

	flags |= DRM_MODE_PROP_ENUM;

	property = drm_property_create(dev, flags, name, num_values);
	if (!property)
		return NULL;

	for (i = 0; i < num_values; i++) {
		ret = drm_property_add_enum(property, i,
				      props[i].type,
				      props[i].name);
		if (ret) {
			drm_property_destroy(dev, property);
			return NULL;
		}
	}

	return property;
}
EXPORT_SYMBOL(drm_property_create_enum);

struct drm_property *drm_property_create_bitmask(struct drm_device *dev,
					 int flags, const char *name,
					 const struct drm_prop_enum_list *props,
					 int num_values)
{
	struct drm_property *property;
	int i, ret;

	flags |= DRM_MODE_PROP_BITMASK;

	property = drm_property_create(dev, flags, name, num_values);
	if (!property)
		return NULL;

	for (i = 0; i < num_values; i++) {
		ret = drm_property_add_enum(property, i,
				      props[i].type,
				      props[i].name);
		if (ret) {
			drm_property_destroy(dev, property);
			return NULL;
		}
	}

	return property;
}
EXPORT_SYMBOL(drm_property_create_bitmask);

struct drm_property *drm_property_create_range(struct drm_device *dev, int flags,
					 const char *name,
					 uint64_t min, uint64_t max)
>>>>>>> refs/remotes/origin/master
{
	struct drm_property *property;

	flags |= DRM_MODE_PROP_RANGE;

	property = drm_property_create(dev, flags, name, 2);
	if (!property)
		return NULL;

	property->values[0] = min;
	property->values[1] = max;

	return property;
}
EXPORT_SYMBOL(drm_property_create_range);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int drm_property_add_enum(struct drm_property *property, int index,
			  uint64_t value, const char *name)
{
	struct drm_property_enum *prop_enum;

<<<<<<< HEAD
	if (!(property->flags & DRM_MODE_PROP_ENUM))
=======
	if (!(property->flags & (DRM_MODE_PROP_ENUM | DRM_MODE_PROP_BITMASK)))
		return -EINVAL;

	/*
	 * Bitmask enum properties have the additional constraint of values
	 * from 0 to 63
	 */
	if ((property->flags & DRM_MODE_PROP_BITMASK) && (value > 63))
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	if (!list_empty(&property->enum_blob_list)) {
		list_for_each_entry(prop_enum, &property->enum_blob_list, head) {
			if (prop_enum->value == value) {
				strncpy(prop_enum->name, name, DRM_PROP_NAME_LEN);
				prop_enum->name[DRM_PROP_NAME_LEN-1] = '\0';
				return 0;
			}
		}
	}

	prop_enum = kzalloc(sizeof(struct drm_property_enum), GFP_KERNEL);
	if (!prop_enum)
		return -ENOMEM;

	strncpy(prop_enum->name, name, DRM_PROP_NAME_LEN);
	prop_enum->name[DRM_PROP_NAME_LEN-1] = '\0';
	prop_enum->value = value;

	property->values[index] = value;
	list_add_tail(&prop_enum->head, &property->enum_blob_list);
	return 0;
}
EXPORT_SYMBOL(drm_property_add_enum);

void drm_property_destroy(struct drm_device *dev, struct drm_property *property)
{
	struct drm_property_enum *prop_enum, *pt;

	list_for_each_entry_safe(prop_enum, pt, &property->enum_blob_list, head) {
		list_del(&prop_enum->head);
		kfree(prop_enum);
	}

	if (property->num_values)
		kfree(property->values);
	drm_mode_object_put(dev, &property->base);
	list_del(&property->head);
	kfree(property);
}
EXPORT_SYMBOL(drm_property_destroy);

<<<<<<< HEAD
int drm_connector_attach_property(struct drm_connector *connector,
			       struct drm_property *property, uint64_t init_val)
{
	int i;

	for (i = 0; i < DRM_CONNECTOR_MAX_PROPERTY; i++) {
		if (connector->property_ids[i] == 0) {
			connector->property_ids[i] = property->base.id;
			connector->property_values[i] = init_val;
			break;
		}
	}

	if (i == DRM_CONNECTOR_MAX_PROPERTY)
		return -EINVAL;
	return 0;
}
EXPORT_SYMBOL(drm_connector_attach_property);

int drm_connector_property_set_value(struct drm_connector *connector,
				  struct drm_property *property, uint64_t value)
{
	int i;

	for (i = 0; i < DRM_CONNECTOR_MAX_PROPERTY; i++) {
		if (connector->property_ids[i] == property->base.id) {
			connector->property_values[i] = value;
			break;
		}
	}

	if (i == DRM_CONNECTOR_MAX_PROPERTY)
		return -EINVAL;
	return 0;
}
EXPORT_SYMBOL(drm_connector_property_set_value);

int drm_connector_property_get_value(struct drm_connector *connector,
=======
void drm_object_attach_property(struct drm_mode_object *obj,
				struct drm_property *property,
				uint64_t init_val)
{
	int count = obj->properties->count;

	if (count == DRM_OBJECT_MAX_PROPERTY) {
		WARN(1, "Failed to attach object property (type: 0x%x). Please "
			"increase DRM_OBJECT_MAX_PROPERTY by 1 for each time "
			"you see this message on the same object type.\n",
			obj->type);
		return;
	}

	obj->properties->ids[count] = property->base.id;
	obj->properties->values[count] = init_val;
	obj->properties->count++;
}
EXPORT_SYMBOL(drm_object_attach_property);

int drm_object_property_set_value(struct drm_mode_object *obj,
				  struct drm_property *property, uint64_t val)
{
	int i;

	for (i = 0; i < obj->properties->count; i++) {
		if (obj->properties->ids[i] == property->base.id) {
			obj->properties->values[i] = val;
			return 0;
		}
	}

	return -EINVAL;
}
EXPORT_SYMBOL(drm_object_property_set_value);

int drm_object_property_get_value(struct drm_mode_object *obj,
>>>>>>> refs/remotes/origin/master
				  struct drm_property *property, uint64_t *val)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < DRM_CONNECTOR_MAX_PROPERTY; i++) {
		if (connector->property_ids[i] == property->base.id) {
			*val = connector->property_values[i];
			break;
		}
	}

	if (i == DRM_CONNECTOR_MAX_PROPERTY)
		return -EINVAL;
	return 0;
}
EXPORT_SYMBOL(drm_connector_property_get_value);
=======
	for (i = 0; i < obj->properties->count; i++) {
		if (obj->properties->ids[i] == property->base.id) {
			*val = obj->properties->values[i];
			return 0;
		}
	}

	return -EINVAL;
}
EXPORT_SYMBOL(drm_object_property_get_value);
>>>>>>> refs/remotes/origin/master

int drm_mode_getproperty_ioctl(struct drm_device *dev,
			       void *data, struct drm_file *file_priv)
{
	struct drm_mode_object *obj;
	struct drm_mode_get_property *out_resp = data;
	struct drm_property *property;
	int enum_count = 0;
	int blob_count = 0;
	int value_count = 0;
	int ret = 0, i;
	int copied;
	struct drm_property_enum *prop_enum;
	struct drm_mode_property_enum __user *enum_ptr;
	struct drm_property_blob *prop_blob;
<<<<<<< HEAD
<<<<<<< HEAD
	uint32_t *blob_id_ptr;
=======
	uint32_t __user *blob_id_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	uint32_t __user *blob_id_ptr;
>>>>>>> refs/remotes/origin/master
	uint64_t __user *values_ptr;
	uint32_t __user *blob_length_ptr;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, out_resp->prop_id, DRM_MODE_OBJECT_PROPERTY);
	if (!obj) {
		ret = -EINVAL;
=======
	drm_modeset_lock_all(dev);
	obj = drm_mode_object_find(dev, out_resp->prop_id, DRM_MODE_OBJECT_PROPERTY);
	if (!obj) {
		ret = -ENOENT;
>>>>>>> refs/remotes/origin/master
		goto done;
	}
	property = obj_to_property(obj);

<<<<<<< HEAD
	if (property->flags & DRM_MODE_PROP_ENUM) {
=======
	if (property->flags & (DRM_MODE_PROP_ENUM | DRM_MODE_PROP_BITMASK)) {
>>>>>>> refs/remotes/origin/master
		list_for_each_entry(prop_enum, &property->enum_blob_list, head)
			enum_count++;
	} else if (property->flags & DRM_MODE_PROP_BLOB) {
		list_for_each_entry(prop_blob, &property->enum_blob_list, head)
			blob_count++;
	}

	value_count = property->num_values;

	strncpy(out_resp->name, property->name, DRM_PROP_NAME_LEN);
	out_resp->name[DRM_PROP_NAME_LEN-1] = 0;
	out_resp->flags = property->flags;

	if ((out_resp->count_values >= value_count) && value_count) {
<<<<<<< HEAD
<<<<<<< HEAD
		values_ptr = (uint64_t *)(unsigned long)out_resp->values_ptr;
=======
		values_ptr = (uint64_t __user *)(unsigned long)out_resp->values_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		values_ptr = (uint64_t __user *)(unsigned long)out_resp->values_ptr;
>>>>>>> refs/remotes/origin/master
		for (i = 0; i < value_count; i++) {
			if (copy_to_user(values_ptr + i, &property->values[i], sizeof(uint64_t))) {
				ret = -EFAULT;
				goto done;
			}
		}
	}
	out_resp->count_values = value_count;

<<<<<<< HEAD
	if (property->flags & DRM_MODE_PROP_ENUM) {
		if ((out_resp->count_enum_blobs >= enum_count) && enum_count) {
			copied = 0;
<<<<<<< HEAD
			enum_ptr = (struct drm_mode_property_enum *)(unsigned long)out_resp->enum_blob_ptr;
=======
			enum_ptr = (struct drm_mode_property_enum __user *)(unsigned long)out_resp->enum_blob_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (property->flags & (DRM_MODE_PROP_ENUM | DRM_MODE_PROP_BITMASK)) {
		if ((out_resp->count_enum_blobs >= enum_count) && enum_count) {
			copied = 0;
			enum_ptr = (struct drm_mode_property_enum __user *)(unsigned long)out_resp->enum_blob_ptr;
>>>>>>> refs/remotes/origin/master
			list_for_each_entry(prop_enum, &property->enum_blob_list, head) {

				if (copy_to_user(&enum_ptr[copied].value, &prop_enum->value, sizeof(uint64_t))) {
					ret = -EFAULT;
					goto done;
				}

				if (copy_to_user(&enum_ptr[copied].name,
						 &prop_enum->name, DRM_PROP_NAME_LEN)) {
					ret = -EFAULT;
					goto done;
				}
				copied++;
			}
		}
		out_resp->count_enum_blobs = enum_count;
	}

	if (property->flags & DRM_MODE_PROP_BLOB) {
		if ((out_resp->count_enum_blobs >= blob_count) && blob_count) {
			copied = 0;
<<<<<<< HEAD
<<<<<<< HEAD
			blob_id_ptr = (uint32_t *)(unsigned long)out_resp->enum_blob_ptr;
			blob_length_ptr = (uint32_t *)(unsigned long)out_resp->values_ptr;
=======
			blob_id_ptr = (uint32_t __user *)(unsigned long)out_resp->enum_blob_ptr;
			blob_length_ptr = (uint32_t __user *)(unsigned long)out_resp->values_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			blob_id_ptr = (uint32_t __user *)(unsigned long)out_resp->enum_blob_ptr;
			blob_length_ptr = (uint32_t __user *)(unsigned long)out_resp->values_ptr;
>>>>>>> refs/remotes/origin/master

			list_for_each_entry(prop_blob, &property->enum_blob_list, head) {
				if (put_user(prop_blob->base.id, blob_id_ptr + copied)) {
					ret = -EFAULT;
					goto done;
				}

				if (put_user(prop_blob->length, blob_length_ptr + copied)) {
					ret = -EFAULT;
					goto done;
				}

				copied++;
			}
		}
		out_resp->count_enum_blobs = blob_count;
	}
done:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

static struct drm_property_blob *drm_property_create_blob(struct drm_device *dev, int length,
							  void *data)
{
	struct drm_property_blob *blob;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int ret;
>>>>>>> refs/remotes/origin/master

	if (!length || !data)
		return NULL;

	blob = kzalloc(sizeof(struct drm_property_blob)+length, GFP_KERNEL);
	if (!blob)
		return NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	blob->data = (void *)((char *)blob + sizeof(struct drm_property_blob));
=======
=======
>>>>>>> refs/remotes/origin/master
	ret = drm_mode_object_get(dev, &blob->base, DRM_MODE_OBJECT_BLOB);
	if (ret) {
		kfree(blob);
		return NULL;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	blob->length = length;

	memcpy(blob->data, data, length);

<<<<<<< HEAD
<<<<<<< HEAD
	drm_mode_object_get(dev, &blob->base, DRM_MODE_OBJECT_BLOB);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	list_add_tail(&blob->head, &dev->mode_config.property_blob_list);
	return blob;
}

static void drm_property_destroy_blob(struct drm_device *dev,
			       struct drm_property_blob *blob)
{
	drm_mode_object_put(dev, &blob->base);
	list_del(&blob->head);
	kfree(blob);
}

int drm_mode_getblob_ioctl(struct drm_device *dev,
			   void *data, struct drm_file *file_priv)
{
	struct drm_mode_object *obj;
	struct drm_mode_get_blob *out_resp = data;
	struct drm_property_blob *blob;
	int ret = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	void *blob_ptr;
=======
	void __user *blob_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	void __user *blob_ptr;
>>>>>>> refs/remotes/origin/master

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, out_resp->blob_id, DRM_MODE_OBJECT_BLOB);
	if (!obj) {
		ret = -EINVAL;
=======
	drm_modeset_lock_all(dev);
	obj = drm_mode_object_find(dev, out_resp->blob_id, DRM_MODE_OBJECT_BLOB);
	if (!obj) {
		ret = -ENOENT;
>>>>>>> refs/remotes/origin/master
		goto done;
	}
	blob = obj_to_blob(obj);

	if (out_resp->length == blob->length) {
<<<<<<< HEAD
<<<<<<< HEAD
		blob_ptr = (void *)(unsigned long)out_resp->data;
=======
		blob_ptr = (void __user *)(unsigned long)out_resp->data;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		blob_ptr = (void __user *)(unsigned long)out_resp->data;
>>>>>>> refs/remotes/origin/master
		if (copy_to_user(blob_ptr, blob->data, blob->length)){
			ret = -EFAULT;
			goto done;
		}
	}
	out_resp->length = blob->length;

done:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

int drm_mode_connector_update_edid_property(struct drm_connector *connector,
					    struct edid *edid)
{
	struct drm_device *dev = connector->dev;
<<<<<<< HEAD
	int ret = 0, size;
=======
	int ret, size;
>>>>>>> refs/remotes/origin/master

	if (connector->edid_blob_ptr)
		drm_property_destroy_blob(dev, connector->edid_blob_ptr);

	/* Delete edid, when there is none. */
	if (!edid) {
		connector->edid_blob_ptr = NULL;
<<<<<<< HEAD
		ret = drm_connector_property_set_value(connector, dev->mode_config.edid_property, 0);
=======
		ret = drm_object_property_set_value(&connector->base, dev->mode_config.edid_property, 0);
>>>>>>> refs/remotes/origin/master
		return ret;
	}

	size = EDID_LENGTH * (1 + edid->extensions);
	connector->edid_blob_ptr = drm_property_create_blob(connector->dev,
							    size, edid);
<<<<<<< HEAD

	ret = drm_connector_property_set_value(connector,
=======
	if (!connector->edid_blob_ptr)
		return -EINVAL;

	ret = drm_object_property_set_value(&connector->base,
>>>>>>> refs/remotes/origin/master
					       dev->mode_config.edid_property,
					       connector->edid_blob_ptr->base.id);

	return ret;
}
EXPORT_SYMBOL(drm_mode_connector_update_edid_property);

<<<<<<< HEAD
int drm_mode_connector_property_set_ioctl(struct drm_device *dev,
				       void *data, struct drm_file *file_priv)
{
	struct drm_mode_connector_set_property *out_resp = data;
	struct drm_mode_object *obj;
	struct drm_property *property;
	struct drm_connector *connector;
	int ret = -EINVAL;
	int i;
=======
static bool drm_property_change_is_valid(struct drm_property *property,
					 uint64_t value)
{
	if (property->flags & DRM_MODE_PROP_IMMUTABLE)
		return false;
	if (property->flags & DRM_MODE_PROP_RANGE) {
		if (value < property->values[0] || value > property->values[1])
			return false;
		return true;
	} else if (property->flags & DRM_MODE_PROP_BITMASK) {
		int i;
		uint64_t valid_mask = 0;
		for (i = 0; i < property->num_values; i++)
			valid_mask |= (1ULL << property->values[i]);
		return !(value & ~valid_mask);
	} else if (property->flags & DRM_MODE_PROP_BLOB) {
		/* Only the driver knows */
		return true;
	} else {
		int i;
		for (i = 0; i < property->num_values; i++)
			if (property->values[i] == value)
				return true;
		return false;
	}
}

int drm_mode_connector_property_set_ioctl(struct drm_device *dev,
				       void *data, struct drm_file *file_priv)
{
	struct drm_mode_connector_set_property *conn_set_prop = data;
	struct drm_mode_obj_set_property obj_set_prop = {
		.value = conn_set_prop->value,
		.prop_id = conn_set_prop->prop_id,
		.obj_id = conn_set_prop->connector_id,
		.obj_type = DRM_MODE_OBJECT_CONNECTOR
	};

	/* It does all the locking and checking we need */
	return drm_mode_obj_set_property_ioctl(dev, &obj_set_prop, file_priv);
}

static int drm_mode_connector_set_obj_prop(struct drm_mode_object *obj,
					   struct drm_property *property,
					   uint64_t value)
{
	int ret = -EINVAL;
	struct drm_connector *connector = obj_to_connector(obj);

	/* Do DPMS ourselves */
	if (property == connector->dev->mode_config.dpms_property) {
		if (connector->funcs->dpms)
			(*connector->funcs->dpms)(connector, (int)value);
		ret = 0;
	} else if (connector->funcs->set_property)
		ret = connector->funcs->set_property(connector, property, value);

	/* store the property value if successful */
	if (!ret)
		drm_object_property_set_value(&connector->base, property, value);
	return ret;
}

static int drm_mode_crtc_set_obj_prop(struct drm_mode_object *obj,
				      struct drm_property *property,
				      uint64_t value)
{
	int ret = -EINVAL;
	struct drm_crtc *crtc = obj_to_crtc(obj);

	if (crtc->funcs->set_property)
		ret = crtc->funcs->set_property(crtc, property, value);
	if (!ret)
		drm_object_property_set_value(obj, property, value);

	return ret;
}

static int drm_mode_plane_set_obj_prop(struct drm_mode_object *obj,
				      struct drm_property *property,
				      uint64_t value)
{
	int ret = -EINVAL;
	struct drm_plane *plane = obj_to_plane(obj);

	if (plane->funcs->set_property)
		ret = plane->funcs->set_property(plane, property, value);
	if (!ret)
		drm_object_property_set_value(obj, property, value);

	return ret;
}

int drm_mode_obj_get_properties_ioctl(struct drm_device *dev, void *data,
				      struct drm_file *file_priv)
{
	struct drm_mode_obj_get_properties *arg = data;
	struct drm_mode_object *obj;
	int ret = 0;
	int i;
	int copied = 0;
	int props_count = 0;
	uint32_t __user *props_ptr;
	uint64_t __user *prop_values_ptr;
>>>>>>> refs/remotes/origin/master

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);

	obj = drm_mode_object_find(dev, out_resp->connector_id, DRM_MODE_OBJECT_CONNECTOR);
	if (!obj) {
		goto out;
	}
	connector = obj_to_connector(obj);

	for (i = 0; i < DRM_CONNECTOR_MAX_PROPERTY; i++) {
		if (connector->property_ids[i] == out_resp->prop_id)
			break;
	}

	if (i == DRM_CONNECTOR_MAX_PROPERTY) {
		goto out;
	}

	obj = drm_mode_object_find(dev, out_resp->prop_id, DRM_MODE_OBJECT_PROPERTY);
	if (!obj) {
		goto out;
	}
	property = obj_to_property(obj);

	if (property->flags & DRM_MODE_PROP_IMMUTABLE)
		goto out;

	if (property->flags & DRM_MODE_PROP_RANGE) {
		if (out_resp->value < property->values[0])
			goto out;

		if (out_resp->value > property->values[1])
			goto out;
	} else {
		int found = 0;
		for (i = 0; i < property->num_values; i++) {
			if (property->values[i] == out_resp->value) {
				found = 1;
				break;
			}
		}
		if (!found) {
			goto out;
		}
	}

	/* Do DPMS ourselves */
	if (property == connector->dev->mode_config.dpms_property) {
		if (connector->funcs->dpms)
			(*connector->funcs->dpms)(connector, (int) out_resp->value);
		ret = 0;
	} else if (connector->funcs->set_property)
		ret = connector->funcs->set_property(connector, property, out_resp->value);

	/* store the property value if successful */
	if (!ret)
		drm_connector_property_set_value(connector, property, out_resp->value);
out:
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_lock_all(dev);

	obj = drm_mode_object_find(dev, arg->obj_id, arg->obj_type);
	if (!obj) {
		ret = -ENOENT;
		goto out;
	}
	if (!obj->properties) {
		ret = -EINVAL;
		goto out;
	}

	props_count = obj->properties->count;

	/* This ioctl is called twice, once to determine how much space is
	 * needed, and the 2nd time to fill it. */
	if ((arg->count_props >= props_count) && props_count) {
		copied = 0;
		props_ptr = (uint32_t __user *)(unsigned long)(arg->props_ptr);
		prop_values_ptr = (uint64_t __user *)(unsigned long)
				  (arg->prop_values_ptr);
		for (i = 0; i < props_count; i++) {
			if (put_user(obj->properties->ids[i],
				     props_ptr + copied)) {
				ret = -EFAULT;
				goto out;
			}
			if (put_user(obj->properties->values[i],
				     prop_values_ptr + copied)) {
				ret = -EFAULT;
				goto out;
			}
			copied++;
		}
	}
	arg->count_props = props_count;
out:
	drm_modeset_unlock_all(dev);
	return ret;
}

int drm_mode_obj_set_property_ioctl(struct drm_device *dev, void *data,
				    struct drm_file *file_priv)
{
	struct drm_mode_obj_set_property *arg = data;
	struct drm_mode_object *arg_obj;
	struct drm_mode_object *prop_obj;
	struct drm_property *property;
	int ret = -EINVAL;
	int i;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

	drm_modeset_lock_all(dev);

	arg_obj = drm_mode_object_find(dev, arg->obj_id, arg->obj_type);
	if (!arg_obj) {
		ret = -ENOENT;
		goto out;
	}
	if (!arg_obj->properties)
		goto out;

	for (i = 0; i < arg_obj->properties->count; i++)
		if (arg_obj->properties->ids[i] == arg->prop_id)
			break;

	if (i == arg_obj->properties->count)
		goto out;

	prop_obj = drm_mode_object_find(dev, arg->prop_id,
					DRM_MODE_OBJECT_PROPERTY);
	if (!prop_obj) {
		ret = -ENOENT;
		goto out;
	}
	property = obj_to_property(prop_obj);

	if (!drm_property_change_is_valid(property, arg->value))
		goto out;

	switch (arg_obj->type) {
	case DRM_MODE_OBJECT_CONNECTOR:
		ret = drm_mode_connector_set_obj_prop(arg_obj, property,
						      arg->value);
		break;
	case DRM_MODE_OBJECT_CRTC:
		ret = drm_mode_crtc_set_obj_prop(arg_obj, property, arg->value);
		break;
	case DRM_MODE_OBJECT_PLANE:
		ret = drm_mode_plane_set_obj_prop(arg_obj, property, arg->value);
		break;
	}

out:
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

int drm_mode_connector_attach_encoder(struct drm_connector *connector,
				      struct drm_encoder *encoder)
{
	int i;

	for (i = 0; i < DRM_CONNECTOR_MAX_ENCODER; i++) {
		if (connector->encoder_ids[i] == 0) {
			connector->encoder_ids[i] = encoder->base.id;
			return 0;
		}
	}
	return -ENOMEM;
}
EXPORT_SYMBOL(drm_mode_connector_attach_encoder);

void drm_mode_connector_detach_encoder(struct drm_connector *connector,
				    struct drm_encoder *encoder)
{
	int i;
	for (i = 0; i < DRM_CONNECTOR_MAX_ENCODER; i++) {
		if (connector->encoder_ids[i] == encoder->base.id) {
			connector->encoder_ids[i] = 0;
			if (connector->encoder == encoder)
				connector->encoder = NULL;
			break;
		}
	}
}
EXPORT_SYMBOL(drm_mode_connector_detach_encoder);

<<<<<<< HEAD
<<<<<<< HEAD
bool drm_mode_crtc_set_gamma_size(struct drm_crtc *crtc,
=======
int drm_mode_crtc_set_gamma_size(struct drm_crtc *crtc,
>>>>>>> refs/remotes/origin/cm-10.0
=======
int drm_mode_crtc_set_gamma_size(struct drm_crtc *crtc,
>>>>>>> refs/remotes/origin/master
				  int gamma_size)
{
	crtc->gamma_size = gamma_size;

	crtc->gamma_store = kzalloc(gamma_size * sizeof(uint16_t) * 3, GFP_KERNEL);
	if (!crtc->gamma_store) {
		crtc->gamma_size = 0;
<<<<<<< HEAD
<<<<<<< HEAD
		return false;
	}

	return true;
=======
=======
>>>>>>> refs/remotes/origin/master
		return -ENOMEM;
	}

	return 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(drm_mode_crtc_set_gamma_size);

int drm_mode_gamma_set_ioctl(struct drm_device *dev,
			     void *data, struct drm_file *file_priv)
{
	struct drm_mode_crtc_lut *crtc_lut = data;
	struct drm_mode_object *obj;
	struct drm_crtc *crtc;
	void *r_base, *g_base, *b_base;
	int size;
	int ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, crtc_lut->crtc_id, DRM_MODE_OBJECT_CRTC);
	if (!obj) {
		ret = -EINVAL;
=======
	drm_modeset_lock_all(dev);
	obj = drm_mode_object_find(dev, crtc_lut->crtc_id, DRM_MODE_OBJECT_CRTC);
	if (!obj) {
		ret = -ENOENT;
>>>>>>> refs/remotes/origin/master
		goto out;
	}
	crtc = obj_to_crtc(obj);

<<<<<<< HEAD
=======
	if (crtc->funcs->gamma_set == NULL) {
		ret = -ENOSYS;
		goto out;
	}

>>>>>>> refs/remotes/origin/master
	/* memcpy into gamma store */
	if (crtc_lut->gamma_size != crtc->gamma_size) {
		ret = -EINVAL;
		goto out;
	}

	size = crtc_lut->gamma_size * (sizeof(uint16_t));
	r_base = crtc->gamma_store;
	if (copy_from_user(r_base, (void __user *)(unsigned long)crtc_lut->red, size)) {
		ret = -EFAULT;
		goto out;
	}

	g_base = r_base + size;
	if (copy_from_user(g_base, (void __user *)(unsigned long)crtc_lut->green, size)) {
		ret = -EFAULT;
		goto out;
	}

	b_base = g_base + size;
	if (copy_from_user(b_base, (void __user *)(unsigned long)crtc_lut->blue, size)) {
		ret = -EFAULT;
		goto out;
	}

	crtc->funcs->gamma_set(crtc, r_base, g_base, b_base, 0, crtc->gamma_size);

out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
	return ret;

}

int drm_mode_gamma_get_ioctl(struct drm_device *dev,
			     void *data, struct drm_file *file_priv)
{
	struct drm_mode_crtc_lut *crtc_lut = data;
	struct drm_mode_object *obj;
	struct drm_crtc *crtc;
	void *r_base, *g_base, *b_base;
	int size;
	int ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, crtc_lut->crtc_id, DRM_MODE_OBJECT_CRTC);
	if (!obj) {
		ret = -EINVAL;
=======
	drm_modeset_lock_all(dev);
	obj = drm_mode_object_find(dev, crtc_lut->crtc_id, DRM_MODE_OBJECT_CRTC);
	if (!obj) {
		ret = -ENOENT;
>>>>>>> refs/remotes/origin/master
		goto out;
	}
	crtc = obj_to_crtc(obj);

	/* memcpy into gamma store */
	if (crtc_lut->gamma_size != crtc->gamma_size) {
		ret = -EINVAL;
		goto out;
	}

	size = crtc_lut->gamma_size * (sizeof(uint16_t));
	r_base = crtc->gamma_store;
	if (copy_to_user((void __user *)(unsigned long)crtc_lut->red, r_base, size)) {
		ret = -EFAULT;
		goto out;
	}

	g_base = r_base + size;
	if (copy_to_user((void __user *)(unsigned long)crtc_lut->green, g_base, size)) {
		ret = -EFAULT;
		goto out;
	}

	b_base = g_base + size;
	if (copy_to_user((void __user *)(unsigned long)crtc_lut->blue, b_base, size)) {
		ret = -EFAULT;
		goto out;
	}
out:
<<<<<<< HEAD
	mutex_unlock(&dev->mode_config.mutex);
=======
	drm_modeset_unlock_all(dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

int drm_mode_page_flip_ioctl(struct drm_device *dev,
			     void *data, struct drm_file *file_priv)
{
	struct drm_mode_crtc_page_flip *page_flip = data;
	struct drm_mode_object *obj;
	struct drm_crtc *crtc;
<<<<<<< HEAD
	struct drm_framebuffer *fb;
=======
	struct drm_framebuffer *fb = NULL, *old_fb = NULL;
>>>>>>> refs/remotes/origin/master
	struct drm_pending_vblank_event *e = NULL;
	unsigned long flags;
	int ret = -EINVAL;

	if (page_flip->flags & ~DRM_MODE_PAGE_FLIP_FLAGS ||
	    page_flip->reserved != 0)
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, page_flip->crtc_id, DRM_MODE_OBJECT_CRTC);
	if (!obj)
		goto out;
	crtc = obj_to_crtc(obj);

=======
	if ((page_flip->flags & DRM_MODE_PAGE_FLIP_ASYNC) && !dev->mode_config.async_page_flip)
		return -EINVAL;

	obj = drm_mode_object_find(dev, page_flip->crtc_id, DRM_MODE_OBJECT_CRTC);
	if (!obj)
		return -ENOENT;
	crtc = obj_to_crtc(obj);

	mutex_lock(&crtc->mutex);
>>>>>>> refs/remotes/origin/master
	if (crtc->fb == NULL) {
		/* The framebuffer is currently unbound, presumably
		 * due to a hotplug event, that userspace has not
		 * yet discovered.
		 */
		ret = -EBUSY;
		goto out;
	}

	if (crtc->funcs->page_flip == NULL)
		goto out;

<<<<<<< HEAD
	obj = drm_mode_object_find(dev, page_flip->fb_id, DRM_MODE_OBJECT_FB);
	if (!obj)
		goto out;
	fb = obj_to_fb(obj);

<<<<<<< HEAD
=======
	if (crtc->mode.hdisplay > fb->width ||
	    crtc->mode.vdisplay > fb->height ||
	    crtc->x > fb->width - crtc->mode.hdisplay ||
	    crtc->y > fb->height - crtc->mode.vdisplay) {
		DRM_DEBUG_KMS("Invalid fb size %ux%u for CRTC viewport %ux%u+%d+%d.\n",
			      fb->width, fb->height,
			      crtc->mode.hdisplay, crtc->mode.vdisplay,
			      crtc->x, crtc->y);
		ret = -ENOSPC;
		goto out;
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	fb = drm_framebuffer_lookup(dev, page_flip->fb_id);
	if (!fb) {
		ret = -ENOENT;
		goto out;
	}

	ret = drm_crtc_check_viewport(crtc, crtc->x, crtc->y, &crtc->mode, fb);
	if (ret)
		goto out;

	if (crtc->fb->pixel_format != fb->pixel_format) {
		DRM_DEBUG_KMS("Page flip is not allowed to change frame buffer format.\n");
		ret = -EINVAL;
		goto out;
	}

>>>>>>> refs/remotes/origin/master
	if (page_flip->flags & DRM_MODE_PAGE_FLIP_EVENT) {
		ret = -ENOMEM;
		spin_lock_irqsave(&dev->event_lock, flags);
		if (file_priv->event_space < sizeof e->event) {
			spin_unlock_irqrestore(&dev->event_lock, flags);
			goto out;
		}
		file_priv->event_space -= sizeof e->event;
		spin_unlock_irqrestore(&dev->event_lock, flags);

		e = kzalloc(sizeof *e, GFP_KERNEL);
		if (e == NULL) {
			spin_lock_irqsave(&dev->event_lock, flags);
			file_priv->event_space += sizeof e->event;
			spin_unlock_irqrestore(&dev->event_lock, flags);
			goto out;
		}

		e->event.base.type = DRM_EVENT_FLIP_COMPLETE;
		e->event.base.length = sizeof e->event;
		e->event.user_data = page_flip->user_data;
		e->base.event = &e->event.base;
		e->base.file_priv = file_priv;
		e->base.destroy =
			(void (*) (struct drm_pending_event *)) kfree;
	}

<<<<<<< HEAD
	ret = crtc->funcs->page_flip(crtc, fb, e);
	if (ret) {
<<<<<<< HEAD
		spin_lock_irqsave(&dev->event_lock, flags);
		file_priv->event_space += sizeof e->event;
		spin_unlock_irqrestore(&dev->event_lock, flags);
		kfree(e);
=======
=======
	old_fb = crtc->fb;
	ret = crtc->funcs->page_flip(crtc, fb, e, page_flip->flags);
	if (ret) {
>>>>>>> refs/remotes/origin/master
		if (page_flip->flags & DRM_MODE_PAGE_FLIP_EVENT) {
			spin_lock_irqsave(&dev->event_lock, flags);
			file_priv->event_space += sizeof e->event;
			spin_unlock_irqrestore(&dev->event_lock, flags);
			kfree(e);
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}

out:
	mutex_unlock(&dev->mode_config.mutex);
=======
		/* Keep the old fb, don't unref it. */
		old_fb = NULL;
	} else {
		/*
		 * Warn if the driver hasn't properly updated the crtc->fb
		 * field to reflect that the new framebuffer is now used.
		 * Failing to do so will screw with the reference counting
		 * on framebuffers.
		 */
		WARN_ON(crtc->fb != fb);
		/* Unref only the old framebuffer. */
		fb = NULL;
	}

out:
	if (fb)
		drm_framebuffer_unreference(fb);
	if (old_fb)
		drm_framebuffer_unreference(old_fb);
	mutex_unlock(&crtc->mutex);

>>>>>>> refs/remotes/origin/master
	return ret;
}

void drm_mode_config_reset(struct drm_device *dev)
{
	struct drm_crtc *crtc;
	struct drm_encoder *encoder;
	struct drm_connector *connector;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head)
		if (crtc->funcs->reset)
			crtc->funcs->reset(crtc);

	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head)
		if (encoder->funcs->reset)
			encoder->funcs->reset(encoder);

<<<<<<< HEAD
	list_for_each_entry(connector, &dev->mode_config.connector_list, head)
		if (connector->funcs->reset)
			connector->funcs->reset(connector);
=======
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		connector->status = connector_status_unknown;

		if (connector->funcs->reset)
			connector->funcs->reset(connector);
	}
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(drm_mode_config_reset);

int drm_mode_create_dumb_ioctl(struct drm_device *dev,
			       void *data, struct drm_file *file_priv)
{
	struct drm_mode_create_dumb *args = data;

	if (!dev->driver->dumb_create)
		return -ENOSYS;
	return dev->driver->dumb_create(file_priv, dev, args);
}

int drm_mode_mmap_dumb_ioctl(struct drm_device *dev,
			     void *data, struct drm_file *file_priv)
{
	struct drm_mode_map_dumb *args = data;

	/* call driver ioctl to get mmap offset */
	if (!dev->driver->dumb_map_offset)
		return -ENOSYS;

	return dev->driver->dumb_map_offset(file_priv, dev, args->handle, &args->offset);
}

int drm_mode_destroy_dumb_ioctl(struct drm_device *dev,
				void *data, struct drm_file *file_priv)
{
	struct drm_mode_destroy_dumb *args = data;

	if (!dev->driver->dumb_destroy)
		return -ENOSYS;

	return dev->driver->dumb_destroy(file_priv, dev, args->handle);
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

/*
 * Just need to support RGB formats here for compat with code that doesn't
 * use pixel formats directly yet.
 */
void drm_fb_get_bpp_depth(uint32_t format, unsigned int *depth,
			  int *bpp)
{
	switch (format) {
	case DRM_FORMAT_C8:
	case DRM_FORMAT_RGB332:
	case DRM_FORMAT_BGR233:
		*depth = 8;
		*bpp = 8;
		break;
	case DRM_FORMAT_XRGB1555:
	case DRM_FORMAT_XBGR1555:
	case DRM_FORMAT_RGBX5551:
	case DRM_FORMAT_BGRX5551:
	case DRM_FORMAT_ARGB1555:
	case DRM_FORMAT_ABGR1555:
	case DRM_FORMAT_RGBA5551:
	case DRM_FORMAT_BGRA5551:
		*depth = 15;
		*bpp = 16;
		break;
	case DRM_FORMAT_RGB565:
	case DRM_FORMAT_BGR565:
		*depth = 16;
		*bpp = 16;
		break;
	case DRM_FORMAT_RGB888:
	case DRM_FORMAT_BGR888:
		*depth = 24;
		*bpp = 24;
		break;
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_RGBX8888:
	case DRM_FORMAT_BGRX8888:
		*depth = 24;
		*bpp = 32;
		break;
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_XBGR2101010:
	case DRM_FORMAT_RGBX1010102:
	case DRM_FORMAT_BGRX1010102:
	case DRM_FORMAT_ARGB2101010:
	case DRM_FORMAT_ABGR2101010:
	case DRM_FORMAT_RGBA1010102:
	case DRM_FORMAT_BGRA1010102:
		*depth = 30;
		*bpp = 32;
		break;
	case DRM_FORMAT_ARGB8888:
	case DRM_FORMAT_ABGR8888:
	case DRM_FORMAT_RGBA8888:
	case DRM_FORMAT_BGRA8888:
		*depth = 32;
		*bpp = 32;
		break;
	default:
<<<<<<< HEAD
		DRM_DEBUG_KMS("unsupported pixel format\n");
=======
		DRM_DEBUG_KMS("unsupported pixel format %s\n",
			      drm_get_format_name(format));
>>>>>>> refs/remotes/origin/master
		*depth = 0;
		*bpp = 0;
		break;
	}
}
EXPORT_SYMBOL(drm_fb_get_bpp_depth);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

/**
 * drm_format_num_planes - get the number of planes for format
 * @format: pixel format (DRM_FORMAT_*)
 *
 * RETURNS:
 * The number of planes used by the specified pixel format.
 */
int drm_format_num_planes(uint32_t format)
{
	switch (format) {
	case DRM_FORMAT_YUV410:
	case DRM_FORMAT_YVU410:
	case DRM_FORMAT_YUV411:
	case DRM_FORMAT_YVU411:
	case DRM_FORMAT_YUV420:
	case DRM_FORMAT_YVU420:
	case DRM_FORMAT_YUV422:
	case DRM_FORMAT_YVU422:
	case DRM_FORMAT_YUV444:
	case DRM_FORMAT_YVU444:
		return 3;
	case DRM_FORMAT_NV12:
	case DRM_FORMAT_NV21:
	case DRM_FORMAT_NV16:
	case DRM_FORMAT_NV61:
	case DRM_FORMAT_NV24:
	case DRM_FORMAT_NV42:
		return 2;
	default:
		return 1;
	}
}
EXPORT_SYMBOL(drm_format_num_planes);

/**
 * drm_format_plane_cpp - determine the bytes per pixel value
 * @format: pixel format (DRM_FORMAT_*)
 * @plane: plane index
 *
 * RETURNS:
 * The bytes per pixel value for the specified plane.
 */
int drm_format_plane_cpp(uint32_t format, int plane)
{
	unsigned int depth;
	int bpp;

	if (plane >= drm_format_num_planes(format))
		return 0;

	switch (format) {
	case DRM_FORMAT_YUYV:
	case DRM_FORMAT_YVYU:
	case DRM_FORMAT_UYVY:
	case DRM_FORMAT_VYUY:
		return 2;
	case DRM_FORMAT_NV12:
	case DRM_FORMAT_NV21:
	case DRM_FORMAT_NV16:
	case DRM_FORMAT_NV61:
	case DRM_FORMAT_NV24:
	case DRM_FORMAT_NV42:
		return plane ? 2 : 1;
	case DRM_FORMAT_YUV410:
	case DRM_FORMAT_YVU410:
	case DRM_FORMAT_YUV411:
	case DRM_FORMAT_YVU411:
	case DRM_FORMAT_YUV420:
	case DRM_FORMAT_YVU420:
	case DRM_FORMAT_YUV422:
	case DRM_FORMAT_YVU422:
	case DRM_FORMAT_YUV444:
	case DRM_FORMAT_YVU444:
		return 1;
	default:
		drm_fb_get_bpp_depth(format, &depth, &bpp);
		return bpp >> 3;
	}
}
EXPORT_SYMBOL(drm_format_plane_cpp);

/**
 * drm_format_horz_chroma_subsampling - get the horizontal chroma subsampling factor
 * @format: pixel format (DRM_FORMAT_*)
 *
 * RETURNS:
 * The horizontal chroma subsampling factor for the
 * specified pixel format.
 */
int drm_format_horz_chroma_subsampling(uint32_t format)
{
	switch (format) {
	case DRM_FORMAT_YUV411:
	case DRM_FORMAT_YVU411:
	case DRM_FORMAT_YUV410:
	case DRM_FORMAT_YVU410:
		return 4;
	case DRM_FORMAT_YUYV:
	case DRM_FORMAT_YVYU:
	case DRM_FORMAT_UYVY:
	case DRM_FORMAT_VYUY:
	case DRM_FORMAT_NV12:
	case DRM_FORMAT_NV21:
	case DRM_FORMAT_NV16:
	case DRM_FORMAT_NV61:
	case DRM_FORMAT_YUV422:
	case DRM_FORMAT_YVU422:
	case DRM_FORMAT_YUV420:
	case DRM_FORMAT_YVU420:
		return 2;
	default:
		return 1;
	}
}
EXPORT_SYMBOL(drm_format_horz_chroma_subsampling);

/**
 * drm_format_vert_chroma_subsampling - get the vertical chroma subsampling factor
 * @format: pixel format (DRM_FORMAT_*)
 *
 * RETURNS:
 * The vertical chroma subsampling factor for the
 * specified pixel format.
 */
int drm_format_vert_chroma_subsampling(uint32_t format)
{
	switch (format) {
	case DRM_FORMAT_YUV410:
	case DRM_FORMAT_YVU410:
		return 4;
	case DRM_FORMAT_YUV420:
	case DRM_FORMAT_YVU420:
	case DRM_FORMAT_NV12:
	case DRM_FORMAT_NV21:
		return 2;
	default:
		return 1;
	}
}
EXPORT_SYMBOL(drm_format_vert_chroma_subsampling);

/**
 * drm_mode_config_init - initialize DRM mode_configuration structure
 * @dev: DRM device
 *
 * Initialize @dev's mode_config structure, used for tracking the graphics
 * configuration of @dev.
 *
 * Since this initializes the modeset locks, no locking is possible. Which is no
 * problem, since this should happen single threaded at init time. It is the
 * driver's problem to ensure this guarantee.
 *
 */
void drm_mode_config_init(struct drm_device *dev)
{
	mutex_init(&dev->mode_config.mutex);
	mutex_init(&dev->mode_config.idr_mutex);
	mutex_init(&dev->mode_config.fb_lock);
	INIT_LIST_HEAD(&dev->mode_config.fb_list);
	INIT_LIST_HEAD(&dev->mode_config.crtc_list);
	INIT_LIST_HEAD(&dev->mode_config.connector_list);
	INIT_LIST_HEAD(&dev->mode_config.bridge_list);
	INIT_LIST_HEAD(&dev->mode_config.encoder_list);
	INIT_LIST_HEAD(&dev->mode_config.property_list);
	INIT_LIST_HEAD(&dev->mode_config.property_blob_list);
	INIT_LIST_HEAD(&dev->mode_config.plane_list);
	idr_init(&dev->mode_config.crtc_idr);

	drm_modeset_lock_all(dev);
	drm_mode_create_standard_connector_properties(dev);
	drm_modeset_unlock_all(dev);

	/* Just to be sure */
	dev->mode_config.num_fb = 0;
	dev->mode_config.num_connector = 0;
	dev->mode_config.num_crtc = 0;
	dev->mode_config.num_encoder = 0;
}
EXPORT_SYMBOL(drm_mode_config_init);

/**
 * drm_mode_config_cleanup - free up DRM mode_config info
 * @dev: DRM device
 *
 * Free up all the connectors and CRTCs associated with this DRM device, then
 * free up the framebuffers and associated buffer objects.
 *
 * Note that since this /should/ happen single-threaded at driver/device
 * teardown time, no locking is required. It's the driver's job to ensure that
 * this guarantee actually holds true.
 *
 * FIXME: cleanup any dangling user buffer objects too
 */
void drm_mode_config_cleanup(struct drm_device *dev)
{
	struct drm_connector *connector, *ot;
	struct drm_crtc *crtc, *ct;
	struct drm_encoder *encoder, *enct;
	struct drm_bridge *bridge, *brt;
	struct drm_framebuffer *fb, *fbt;
	struct drm_property *property, *pt;
	struct drm_property_blob *blob, *bt;
	struct drm_plane *plane, *plt;

	list_for_each_entry_safe(encoder, enct, &dev->mode_config.encoder_list,
				 head) {
		encoder->funcs->destroy(encoder);
	}

	list_for_each_entry_safe(bridge, brt,
				 &dev->mode_config.bridge_list, head) {
		bridge->funcs->destroy(bridge);
	}

	list_for_each_entry_safe(connector, ot,
				 &dev->mode_config.connector_list, head) {
		connector->funcs->destroy(connector);
	}

	list_for_each_entry_safe(property, pt, &dev->mode_config.property_list,
				 head) {
		drm_property_destroy(dev, property);
	}

	list_for_each_entry_safe(blob, bt, &dev->mode_config.property_blob_list,
				 head) {
		drm_property_destroy_blob(dev, blob);
	}

	/*
	 * Single-threaded teardown context, so it's not required to grab the
	 * fb_lock to protect against concurrent fb_list access. Contrary, it
	 * would actually deadlock with the drm_framebuffer_cleanup function.
	 *
	 * Also, if there are any framebuffers left, that's a driver leak now,
	 * so politely WARN about this.
	 */
	WARN_ON(!list_empty(&dev->mode_config.fb_list));
	list_for_each_entry_safe(fb, fbt, &dev->mode_config.fb_list, head) {
		drm_framebuffer_remove(fb);
	}

	list_for_each_entry_safe(plane, plt, &dev->mode_config.plane_list,
				 head) {
		plane->funcs->destroy(plane);
	}

	list_for_each_entry_safe(crtc, ct, &dev->mode_config.crtc_list, head) {
		crtc->funcs->destroy(crtc);
	}

	idr_destroy(&dev->mode_config.crtc_idr);
}
EXPORT_SYMBOL(drm_mode_config_cleanup);
>>>>>>> refs/remotes/origin/master
