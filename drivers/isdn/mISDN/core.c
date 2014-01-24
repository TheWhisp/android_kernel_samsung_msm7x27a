/*
 * Copyright 2008  by Karsten Keil <kkeil@novell.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/slab.h>
#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/mISDNif.h>
#include "core.h"

static u_int debug;

MODULE_AUTHOR("Karsten Keil");
MODULE_LICENSE("GPL");
module_param(debug, uint, S_IRUGO | S_IWUSR);

static u64		device_ids;
#define MAX_DEVICE_ID	63

static LIST_HEAD(Bprotocols);
static DEFINE_RWLOCK(bp_lock);

static void mISDN_dev_release(struct device *dev)
{
	/* nothing to do: the device is part of its parent's data structure */
}

<<<<<<< HEAD
static ssize_t _show_id(struct device *dev,
<<<<<<< HEAD
				struct device_attribute *attr, char *buf)
=======
			struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t id_show(struct device *dev,
		       struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct mISDNdevice *mdev = dev_to_mISDN(dev);

	if (!mdev)
		return -ENODEV;
	return sprintf(buf, "%d\n", mdev->id);
}
<<<<<<< HEAD

static ssize_t _show_nrbchan(struct device *dev,
<<<<<<< HEAD
				struct device_attribute *attr, char *buf)
=======
			     struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static DEVICE_ATTR_RO(id);

static ssize_t nrbchan_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct mISDNdevice *mdev = dev_to_mISDN(dev);

	if (!mdev)
		return -ENODEV;
	return sprintf(buf, "%d\n", mdev->nrbchan);
}
<<<<<<< HEAD

static ssize_t _show_d_protocols(struct device *dev,
<<<<<<< HEAD
				struct device_attribute *attr, char *buf)
=======
				 struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static DEVICE_ATTR_RO(nrbchan);

static ssize_t d_protocols_show(struct device *dev,
				struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct mISDNdevice *mdev = dev_to_mISDN(dev);

	if (!mdev)
		return -ENODEV;
	return sprintf(buf, "%d\n", mdev->Dprotocols);
}
<<<<<<< HEAD

static ssize_t _show_b_protocols(struct device *dev,
<<<<<<< HEAD
				struct device_attribute *attr, char *buf)
=======
				 struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static DEVICE_ATTR_RO(d_protocols);

static ssize_t b_protocols_show(struct device *dev,
				struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct mISDNdevice *mdev = dev_to_mISDN(dev);

	if (!mdev)
		return -ENODEV;
	return sprintf(buf, "%d\n", mdev->Bprotocols | get_all_Bprotocols());
}
<<<<<<< HEAD

static ssize_t _show_protocol(struct device *dev,
<<<<<<< HEAD
				struct device_attribute *attr, char *buf)
=======
			      struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static DEVICE_ATTR_RO(b_protocols);

static ssize_t protocol_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct mISDNdevice *mdev = dev_to_mISDN(dev);

	if (!mdev)
		return -ENODEV;
	return sprintf(buf, "%d\n", mdev->D.protocol);
}
<<<<<<< HEAD

static ssize_t _show_name(struct device *dev,
<<<<<<< HEAD
				struct device_attribute *attr, char *buf)
=======
			  struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static DEVICE_ATTR_RO(protocol);

static ssize_t name_show(struct device *dev,
			 struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	strcpy(buf, dev_name(dev));
	return strlen(buf);
}
<<<<<<< HEAD

#if 0 /* hangs */
static ssize_t _set_name(struct device *dev, struct device_attribute *attr,
<<<<<<< HEAD
				const char *buf, size_t count)
=======
			 const char *buf, size_t count)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static DEVICE_ATTR_RO(name);

#if 0 /* hangs */
static ssize_t name_set(struct device *dev, struct device_attribute *attr,
			const char *buf, size_t count)
>>>>>>> refs/remotes/origin/master
{
	int err = 0;
	char *out = kmalloc(count + 1, GFP_KERNEL);

	if (!out)
		return -ENOMEM;

	memcpy(out, buf, count);
	if (count && out[count - 1] == '\n')
		out[--count] = 0;
	if (count)
		err = device_rename(dev, out);
	kfree(out);

	return (err < 0) ? err : count;
}
<<<<<<< HEAD
#endif

static ssize_t _show_channelmap(struct device *dev,
				struct device_attribute *attr, char *buf)
=======
static DEVICE_ATTR_RW(name);
#endif

static ssize_t channelmap_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct mISDNdevice *mdev = dev_to_mISDN(dev);
	char *bp = buf;
	int i;

	for (i = 0; i <= mdev->nrbchan; i++)
		*bp++ = test_channelmap(i, mdev->channelmap) ? '1' : '0';

	return bp - buf;
}
<<<<<<< HEAD

static struct device_attribute mISDN_dev_attrs[] = {
	__ATTR(id,          S_IRUGO,         _show_id,          NULL),
	__ATTR(d_protocols, S_IRUGO,         _show_d_protocols, NULL),
	__ATTR(b_protocols, S_IRUGO,         _show_b_protocols, NULL),
	__ATTR(protocol,    S_IRUGO,         _show_protocol,    NULL),
	__ATTR(channelmap,  S_IRUGO,         _show_channelmap,  NULL),
	__ATTR(nrbchan,     S_IRUGO,         _show_nrbchan,     NULL),
	__ATTR(name,        S_IRUGO,         _show_name,        NULL),
<<<<<<< HEAD
/*	__ATTR(name,        S_IRUGO|S_IWUSR, _show_name,       _set_name), */
=======
/*	__ATTR(name,        S_IRUGO | S_IWUSR, _show_name,      _set_name), */
>>>>>>> refs/remotes/origin/cm-10.0
	{}
};

#ifdef CONFIG_HOTPLUG
=======
static DEVICE_ATTR_RO(channelmap);

static struct attribute *mISDN_attrs[] = {
	&dev_attr_id.attr,
	&dev_attr_d_protocols.attr,
	&dev_attr_b_protocols.attr,
	&dev_attr_protocol.attr,
	&dev_attr_channelmap.attr,
	&dev_attr_nrbchan.attr,
	&dev_attr_name.attr,
	NULL,
};
ATTRIBUTE_GROUPS(mISDN);

>>>>>>> refs/remotes/origin/master
static int mISDN_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct mISDNdevice *mdev = dev_to_mISDN(dev);

	if (!mdev)
		return 0;

	if (add_uevent_var(env, "nchans=%d", mdev->nrbchan))
		return -ENOMEM;

	return 0;
}
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/master

static void mISDN_class_release(struct class *cls)
{
	/* do nothing, it's static */
}

static struct class mISDN_class = {
	.name = "mISDN",
	.owner = THIS_MODULE,
<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG
	.dev_uevent = mISDN_uevent,
#endif
	.dev_attrs = mISDN_dev_attrs,
=======
	.dev_uevent = mISDN_uevent,
	.dev_groups = mISDN_groups,
>>>>>>> refs/remotes/origin/master
	.dev_release = mISDN_dev_release,
	.class_release = mISDN_class_release,
};

static int
<<<<<<< HEAD
_get_mdevice(struct device *dev, void *id)
=======
_get_mdevice(struct device *dev, const void *id)
>>>>>>> refs/remotes/origin/master
{
	struct mISDNdevice *mdev = dev_to_mISDN(dev);

	if (!mdev)
		return 0;
<<<<<<< HEAD
	if (mdev->id != *(u_int *)id)
=======
	if (mdev->id != *(const u_int *)id)
>>>>>>> refs/remotes/origin/master
		return 0;
	return 1;
}

struct mISDNdevice
*get_mdevice(u_int id)
{
	return dev_to_mISDN(class_find_device(&mISDN_class, NULL, &id,
<<<<<<< HEAD
<<<<<<< HEAD
		_get_mdevice));
=======
					      _get_mdevice));
>>>>>>> refs/remotes/origin/cm-10.0
=======
					      _get_mdevice));
>>>>>>> refs/remotes/origin/master
}

static int
_get_mdevice_count(struct device *dev, void *cnt)
{
	*(int *)cnt += 1;
	return 0;
}

int
get_mdevice_count(void)
{
	int cnt = 0;

	class_for_each_device(&mISDN_class, NULL, &cnt, _get_mdevice_count);
	return cnt;
}

static int
get_free_devid(void)
{
	u_int	i;

	for (i = 0; i <= MAX_DEVICE_ID; i++)
		if (!test_and_set_bit(i, (u_long *)&device_ids))
			break;
	if (i > MAX_DEVICE_ID)
		return -EBUSY;
	return i;
}

int
mISDN_register_device(struct mISDNdevice *dev,
<<<<<<< HEAD
<<<<<<< HEAD
			struct device *parent, char *name)
=======
		      struct device *parent, char *name)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		      struct device *parent, char *name)
>>>>>>> refs/remotes/origin/master
{
	int	err;

	err = get_free_devid();
	if (err < 0)
		goto error1;
	dev->id = err;

	device_initialize(&dev->dev);
	if (name && name[0])
		dev_set_name(&dev->dev, "%s", name);
	else
		dev_set_name(&dev->dev, "mISDN%d", dev->id);
	if (debug & DEBUG_CORE)
		printk(KERN_DEBUG "mISDN_register %s %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
			dev_name(&dev->dev), dev->id);
=======
		       dev_name(&dev->dev), dev->id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       dev_name(&dev->dev), dev->id);
>>>>>>> refs/remotes/origin/master
	err = create_stack(dev);
	if (err)
		goto error1;

	dev->dev.class = &mISDN_class;
	dev->dev.platform_data = dev;
	dev->dev.parent = parent;
	dev_set_drvdata(&dev->dev, dev);

	err = device_add(&dev->dev);
	if (err)
		goto error3;
	return 0;

error3:
	delete_stack(dev);
	return err;
error1:
	return err;

}
EXPORT_SYMBOL(mISDN_register_device);

void
mISDN_unregister_device(struct mISDNdevice *dev) {
	if (debug & DEBUG_CORE)
		printk(KERN_DEBUG "mISDN_unregister %s %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
			dev_name(&dev->dev), dev->id);
=======
		       dev_name(&dev->dev), dev->id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       dev_name(&dev->dev), dev->id);
>>>>>>> refs/remotes/origin/master
	/* sysfs_remove_link(&dev->dev.kobj, "device"); */
	device_del(&dev->dev);
	dev_set_drvdata(&dev->dev, NULL);

	test_and_clear_bit(dev->id, (u_long *)&device_ids);
	delete_stack(dev);
	put_device(&dev->dev);
}
EXPORT_SYMBOL(mISDN_unregister_device);

u_int
get_all_Bprotocols(void)
{
	struct Bprotocol	*bp;
	u_int	m = 0;

	read_lock(&bp_lock);
	list_for_each_entry(bp, &Bprotocols, list)
		m |= bp->Bprotocols;
	read_unlock(&bp_lock);
	return m;
}

struct Bprotocol *
get_Bprotocol4mask(u_int m)
{
	struct Bprotocol	*bp;

	read_lock(&bp_lock);
	list_for_each_entry(bp, &Bprotocols, list)
		if (bp->Bprotocols & m) {
			read_unlock(&bp_lock);
			return bp;
		}
	read_unlock(&bp_lock);
	return NULL;
}

struct Bprotocol *
get_Bprotocol4id(u_int id)
{
	u_int	m;

	if (id < ISDN_P_B_START || id > 63) {
		printk(KERN_WARNING "%s id not in range  %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
		    __func__, id);
=======
		       __func__, id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       __func__, id);
>>>>>>> refs/remotes/origin/master
		return NULL;
	}
	m = 1 << (id & ISDN_P_B_MASK);
	return get_Bprotocol4mask(m);
}

int
mISDN_register_Bprotocol(struct Bprotocol *bp)
{
	u_long			flags;
	struct Bprotocol	*old;

	if (debug & DEBUG_CORE)
		printk(KERN_DEBUG "%s: %s/%x\n", __func__,
<<<<<<< HEAD
<<<<<<< HEAD
		    bp->name, bp->Bprotocols);
	old = get_Bprotocol4mask(bp->Bprotocols);
	if (old) {
		printk(KERN_WARNING
		    "register duplicate protocol old %s/%x new %s/%x\n",
		    old->name, old->Bprotocols, bp->name, bp->Bprotocols);
=======
=======
>>>>>>> refs/remotes/origin/master
		       bp->name, bp->Bprotocols);
	old = get_Bprotocol4mask(bp->Bprotocols);
	if (old) {
		printk(KERN_WARNING
		       "register duplicate protocol old %s/%x new %s/%x\n",
		       old->name, old->Bprotocols, bp->name, bp->Bprotocols);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return -EBUSY;
	}
	write_lock_irqsave(&bp_lock, flags);
	list_add_tail(&bp->list, &Bprotocols);
	write_unlock_irqrestore(&bp_lock, flags);
	return 0;
}
EXPORT_SYMBOL(mISDN_register_Bprotocol);

void
mISDN_unregister_Bprotocol(struct Bprotocol *bp)
{
	u_long	flags;

	if (debug & DEBUG_CORE)
		printk(KERN_DEBUG "%s: %s/%x\n", __func__, bp->name,
<<<<<<< HEAD
<<<<<<< HEAD
			bp->Bprotocols);
=======
		       bp->Bprotocols);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		       bp->Bprotocols);
>>>>>>> refs/remotes/origin/master
	write_lock_irqsave(&bp_lock, flags);
	list_del(&bp->list);
	write_unlock_irqrestore(&bp_lock, flags);
}
EXPORT_SYMBOL(mISDN_unregister_Bprotocol);

<<<<<<< HEAD
=======
static const char *msg_no_channel = "<no channel>";
static const char *msg_no_stack = "<no stack>";
static const char *msg_no_stackdev = "<no stack device>";

const char *mISDNDevName4ch(struct mISDNchannel *ch)
{
	if (!ch)
		return msg_no_channel;
	if (!ch->st)
		return msg_no_stack;
	if (!ch->st->dev)
		return msg_no_stackdev;
	return dev_name(&ch->st->dev->dev);
};
EXPORT_SYMBOL(mISDNDevName4ch);

>>>>>>> refs/remotes/origin/master
static int
mISDNInit(void)
{
	int	err;

	printk(KERN_INFO "Modular ISDN core version %d.%d.%d\n",
<<<<<<< HEAD
<<<<<<< HEAD
		MISDN_MAJOR_VERSION, MISDN_MINOR_VERSION, MISDN_RELEASE);
=======
	       MISDN_MAJOR_VERSION, MISDN_MINOR_VERSION, MISDN_RELEASE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	       MISDN_MAJOR_VERSION, MISDN_MINOR_VERSION, MISDN_RELEASE);
>>>>>>> refs/remotes/origin/master
	mISDN_init_clock(&debug);
	mISDN_initstack(&debug);
	err = class_register(&mISDN_class);
	if (err)
		goto error1;
	err = mISDN_inittimer(&debug);
	if (err)
		goto error2;
	err = l1_init(&debug);
	if (err)
		goto error3;
	err = Isdnl2_Init(&debug);
	if (err)
		goto error4;
	err = misdn_sock_init(&debug);
	if (err)
		goto error5;
	return 0;

error5:
	Isdnl2_cleanup();
error4:
	l1_cleanup();
error3:
	mISDN_timer_cleanup();
error2:
	class_unregister(&mISDN_class);
error1:
	return err;
}

static void mISDN_cleanup(void)
{
	misdn_sock_cleanup();
	Isdnl2_cleanup();
	l1_cleanup();
	mISDN_timer_cleanup();
	class_unregister(&mISDN_class);

	printk(KERN_DEBUG "mISDNcore unloaded\n");
}

module_init(mISDNInit);
module_exit(mISDN_cleanup);
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
