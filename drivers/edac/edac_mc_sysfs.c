/*
 * edac_mc kernel module
 * (C) 2005-2007 Linux Networx (http://lnxi.com)
 *
 * This file may be distributed under the terms of the
 * GNU General Public License.
 *
 * Written Doug Thompson <norsk5@xmission.com> www.softwarebitmaker.com
 *
<<<<<<< HEAD
=======
 * (c) 2012-2013 - Mauro Carvalho Chehab <mchehab@redhat.com>
 *	The entire API were re-written, and ported to use struct device
 *
>>>>>>> refs/remotes/origin/master
 */

#include <linux/ctype.h>
#include <linux/slab.h>
#include <linux/edac.h>
#include <linux/bug.h>
<<<<<<< HEAD
=======
#include <linux/pm_runtime.h>
#include <linux/uaccess.h>
>>>>>>> refs/remotes/origin/master

#include "edac_core.h"
#include "edac_module.h"

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/* MC EDAC Controls, setable by module parameter, and sysfs */
static int edac_mc_log_ue = 1;
static int edac_mc_log_ce = 1;
static int edac_mc_panic_on_ue;
static int edac_mc_poll_msec = 1000;

/* Getter functions for above */
int edac_mc_get_log_ue(void)
{
	return edac_mc_log_ue;
}

int edac_mc_get_log_ce(void)
{
	return edac_mc_log_ce;
}

int edac_mc_get_panic_on_ue(void)
{
	return edac_mc_panic_on_ue;
}

/* this is temporary */
int edac_mc_get_poll_msec(void)
{
	return edac_mc_poll_msec;
}

static int edac_set_poll_msec(const char *val, struct kernel_param *kp)
{
	long l;
	int ret;

	if (!val)
		return -EINVAL;

<<<<<<< HEAD
	ret = strict_strtol(val, 0, &l);
	if (ret == -EINVAL || ((int)l != l))
=======
	ret = kstrtol(val, 0, &l);
	if (ret)
		return ret;
	if ((int)l != l)
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	*((int *)kp->arg) = l;

	/* notify edac_mc engine to reset the poll period */
	edac_mc_reset_delay_period(l);

	return 0;
}

/* Parameter declarations for above */
module_param(edac_mc_panic_on_ue, int, 0644);
MODULE_PARM_DESC(edac_mc_panic_on_ue, "Panic on uncorrected error: 0=off 1=on");
module_param(edac_mc_log_ue, int, 0644);
MODULE_PARM_DESC(edac_mc_log_ue,
		 "Log uncorrectable error to console: 0=off 1=on");
module_param(edac_mc_log_ce, int, 0644);
MODULE_PARM_DESC(edac_mc_log_ce,
		 "Log correctable error to console: 0=off 1=on");
module_param_call(edac_mc_poll_msec, edac_set_poll_msec, param_get_int,
		  &edac_mc_poll_msec, 0644);
MODULE_PARM_DESC(edac_mc_poll_msec, "Polling period in milliseconds");

<<<<<<< HEAD
/*
 * various constants for Memory Controllers
 */
static const char *mem_types[] = {
=======
static struct device *mci_pdev;

/*
 * various constants for Memory Controllers
 */
static const char * const mem_types[] = {
>>>>>>> refs/remotes/origin/master
	[MEM_EMPTY] = "Empty",
	[MEM_RESERVED] = "Reserved",
	[MEM_UNKNOWN] = "Unknown",
	[MEM_FPM] = "FPM",
	[MEM_EDO] = "EDO",
	[MEM_BEDO] = "BEDO",
	[MEM_SDR] = "Unbuffered-SDR",
	[MEM_RDR] = "Registered-SDR",
	[MEM_DDR] = "Unbuffered-DDR",
	[MEM_RDDR] = "Registered-DDR",
	[MEM_RMBS] = "RMBS",
	[MEM_DDR2] = "Unbuffered-DDR2",
	[MEM_FB_DDR2] = "FullyBuffered-DDR2",
	[MEM_RDDR2] = "Registered-DDR2",
	[MEM_XDR] = "XDR",
	[MEM_DDR3] = "Unbuffered-DDR3",
	[MEM_RDDR3] = "Registered-DDR3"
};

<<<<<<< HEAD
static const char *dev_types[] = {
=======
static const char * const dev_types[] = {
>>>>>>> refs/remotes/origin/master
	[DEV_UNKNOWN] = "Unknown",
	[DEV_X1] = "x1",
	[DEV_X2] = "x2",
	[DEV_X4] = "x4",
	[DEV_X8] = "x8",
	[DEV_X16] = "x16",
	[DEV_X32] = "x32",
	[DEV_X64] = "x64"
};

<<<<<<< HEAD
static const char *edac_caps[] = {
=======
static const char * const edac_caps[] = {
>>>>>>> refs/remotes/origin/master
	[EDAC_UNKNOWN] = "Unknown",
	[EDAC_NONE] = "None",
	[EDAC_RESERVED] = "Reserved",
	[EDAC_PARITY] = "PARITY",
	[EDAC_EC] = "EC",
	[EDAC_SECDED] = "SECDED",
	[EDAC_S2ECD2ED] = "S2ECD2ED",
	[EDAC_S4ECD4ED] = "S4ECD4ED",
	[EDAC_S8ECD8ED] = "S8ECD8ED",
	[EDAC_S16ECD16ED] = "S16ECD16ED"
};

<<<<<<< HEAD
/* EDAC sysfs CSROW data structures and methods
 */

/* Set of more default csrow<id> attribute show/store functions */
static ssize_t csrow_ue_count_show(struct csrow_info *csrow, char *data,
				int private)
{
	return sprintf(data, "%u\n", csrow->ue_count);
}

static ssize_t csrow_ce_count_show(struct csrow_info *csrow, char *data,
				int private)
{
	return sprintf(data, "%u\n", csrow->ce_count);
}

static ssize_t csrow_size_show(struct csrow_info *csrow, char *data,
				int private)
{
	return sprintf(data, "%u\n", PAGES_TO_MiB(csrow->nr_pages));
}

static ssize_t csrow_mem_type_show(struct csrow_info *csrow, char *data,
				int private)
{
	return sprintf(data, "%s\n", mem_types[csrow->mtype]);
}

static ssize_t csrow_dev_type_show(struct csrow_info *csrow, char *data,
				int private)
{
	return sprintf(data, "%s\n", dev_types[csrow->dtype]);
}

static ssize_t csrow_edac_mode_show(struct csrow_info *csrow, char *data,
				int private)
{
	return sprintf(data, "%s\n", edac_caps[csrow->edac_mode]);
}

/* show/store functions for DIMM Label attributes */
static ssize_t channel_dimm_label_show(struct csrow_info *csrow,
				char *data, int channel)
{
	/* if field has not been initialized, there is nothing to send */
	if (!csrow->channels[channel].label[0])
		return 0;

	return snprintf(data, EDAC_MC_LABEL_LEN, "%s\n",
			csrow->channels[channel].label);
}

static ssize_t channel_dimm_label_store(struct csrow_info *csrow,
					const char *data,
					size_t count, int channel)
{
	ssize_t max_size = 0;

	max_size = min((ssize_t) count, (ssize_t) EDAC_MC_LABEL_LEN - 1);
	strncpy(csrow->channels[channel].label, data, max_size);
	csrow->channels[channel].label[max_size] = '\0';
=======
#ifdef CONFIG_EDAC_LEGACY_SYSFS
/*
 * EDAC sysfs CSROW data structures and methods
 */

#define to_csrow(k) container_of(k, struct csrow_info, dev)

/*
 * We need it to avoid namespace conflicts between the legacy API
 * and the per-dimm/per-rank one
 */
#define DEVICE_ATTR_LEGACY(_name, _mode, _show, _store) \
	static struct device_attribute dev_attr_legacy_##_name = __ATTR(_name, _mode, _show, _store)

struct dev_ch_attribute {
	struct device_attribute attr;
	int channel;
};

#define DEVICE_CHANNEL(_name, _mode, _show, _store, _var) \
	struct dev_ch_attribute dev_attr_legacy_##_name = \
		{ __ATTR(_name, _mode, _show, _store), (_var) }

#define to_channel(k) (container_of(k, struct dev_ch_attribute, attr)->channel)

/* Set of more default csrow<id> attribute show/store functions */
static ssize_t csrow_ue_count_show(struct device *dev,
				   struct device_attribute *mattr, char *data)
{
	struct csrow_info *csrow = to_csrow(dev);

	return sprintf(data, "%u\n", csrow->ue_count);
}

static ssize_t csrow_ce_count_show(struct device *dev,
				   struct device_attribute *mattr, char *data)
{
	struct csrow_info *csrow = to_csrow(dev);

	return sprintf(data, "%u\n", csrow->ce_count);
}

static ssize_t csrow_size_show(struct device *dev,
			       struct device_attribute *mattr, char *data)
{
	struct csrow_info *csrow = to_csrow(dev);
	int i;
	u32 nr_pages = 0;

	for (i = 0; i < csrow->nr_channels; i++)
		nr_pages += csrow->channels[i]->dimm->nr_pages;
	return sprintf(data, "%u\n", PAGES_TO_MiB(nr_pages));
}

static ssize_t csrow_mem_type_show(struct device *dev,
				   struct device_attribute *mattr, char *data)
{
	struct csrow_info *csrow = to_csrow(dev);

	return sprintf(data, "%s\n", mem_types[csrow->channels[0]->dimm->mtype]);
}

static ssize_t csrow_dev_type_show(struct device *dev,
				   struct device_attribute *mattr, char *data)
{
	struct csrow_info *csrow = to_csrow(dev);

	return sprintf(data, "%s\n", dev_types[csrow->channels[0]->dimm->dtype]);
}

static ssize_t csrow_edac_mode_show(struct device *dev,
				    struct device_attribute *mattr,
				    char *data)
{
	struct csrow_info *csrow = to_csrow(dev);

	return sprintf(data, "%s\n", edac_caps[csrow->channels[0]->dimm->edac_mode]);
}

/* show/store functions for DIMM Label attributes */
static ssize_t channel_dimm_label_show(struct device *dev,
				       struct device_attribute *mattr,
				       char *data)
{
	struct csrow_info *csrow = to_csrow(dev);
	unsigned chan = to_channel(mattr);
	struct rank_info *rank = csrow->channels[chan];

	/* if field has not been initialized, there is nothing to send */
	if (!rank->dimm->label[0])
		return 0;

	return snprintf(data, EDAC_MC_LABEL_LEN, "%s\n",
			rank->dimm->label);
}

static ssize_t channel_dimm_label_store(struct device *dev,
					struct device_attribute *mattr,
					const char *data, size_t count)
{
	struct csrow_info *csrow = to_csrow(dev);
	unsigned chan = to_channel(mattr);
	struct rank_info *rank = csrow->channels[chan];

	ssize_t max_size = 0;

	max_size = min((ssize_t) count, (ssize_t) EDAC_MC_LABEL_LEN - 1);
	strncpy(rank->dimm->label, data, max_size);
	rank->dimm->label[max_size] = '\0';
>>>>>>> refs/remotes/origin/master

	return max_size;
}

/* show function for dynamic chX_ce_count attribute */
<<<<<<< HEAD
static ssize_t channel_ce_count_show(struct csrow_info *csrow,
				char *data, int channel)
{
	return sprintf(data, "%u\n", csrow->channels[channel].ce_count);
}

/* csrow specific attribute structure */
struct csrowdev_attribute {
	struct attribute attr;
	 ssize_t(*show) (struct csrow_info *, char *, int);
	 ssize_t(*store) (struct csrow_info *, const char *, size_t, int);
	int private;
};

#define to_csrow(k) container_of(k, struct csrow_info, kobj)
#define to_csrowdev_attr(a) container_of(a, struct csrowdev_attribute, attr)

/* Set of show/store higher level functions for default csrow attributes */
static ssize_t csrowdev_show(struct kobject *kobj,
			struct attribute *attr, char *buffer)
{
	struct csrow_info *csrow = to_csrow(kobj);
	struct csrowdev_attribute *csrowdev_attr = to_csrowdev_attr(attr);

	if (csrowdev_attr->show)
		return csrowdev_attr->show(csrow,
					buffer, csrowdev_attr->private);
	return -EIO;
}

static ssize_t csrowdev_store(struct kobject *kobj, struct attribute *attr,
			const char *buffer, size_t count)
{
	struct csrow_info *csrow = to_csrow(kobj);
	struct csrowdev_attribute *csrowdev_attr = to_csrowdev_attr(attr);

	if (csrowdev_attr->store)
		return csrowdev_attr->store(csrow,
					buffer,
					count, csrowdev_attr->private);
	return -EIO;
}

static const struct sysfs_ops csrowfs_ops = {
	.show = csrowdev_show,
	.store = csrowdev_store
};

#define CSROWDEV_ATTR(_name,_mode,_show,_store,_private)	\
static struct csrowdev_attribute attr_##_name = {			\
	.attr = {.name = __stringify(_name), .mode = _mode },	\
	.show   = _show,					\
	.store  = _store,					\
	.private = _private,					\
};

/* default cwrow<id>/attribute files */
CSROWDEV_ATTR(size_mb, S_IRUGO, csrow_size_show, NULL, 0);
CSROWDEV_ATTR(dev_type, S_IRUGO, csrow_dev_type_show, NULL, 0);
CSROWDEV_ATTR(mem_type, S_IRUGO, csrow_mem_type_show, NULL, 0);
CSROWDEV_ATTR(edac_mode, S_IRUGO, csrow_edac_mode_show, NULL, 0);
CSROWDEV_ATTR(ue_count, S_IRUGO, csrow_ue_count_show, NULL, 0);
CSROWDEV_ATTR(ce_count, S_IRUGO, csrow_ce_count_show, NULL, 0);

/* default attributes of the CSROW<id> object */
static struct csrowdev_attribute *default_csrow_attr[] = {
	&attr_dev_type,
	&attr_mem_type,
	&attr_edac_mode,
	&attr_size_mb,
	&attr_ue_count,
	&attr_ce_count,
	NULL,
};

/* possible dynamic channel DIMM Label attribute files */
CSROWDEV_ATTR(ch0_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 0);
CSROWDEV_ATTR(ch1_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 1);
CSROWDEV_ATTR(ch2_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 2);
CSROWDEV_ATTR(ch3_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 3);
CSROWDEV_ATTR(ch4_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 4);
CSROWDEV_ATTR(ch5_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 5);

/* Total possible dynamic DIMM Label attribute file table */
static struct csrowdev_attribute *dynamic_csrow_dimm_attr[] = {
	&attr_ch0_dimm_label,
	&attr_ch1_dimm_label,
	&attr_ch2_dimm_label,
	&attr_ch3_dimm_label,
	&attr_ch4_dimm_label,
	&attr_ch5_dimm_label
};

/* possible dynamic channel ce_count attribute files */
CSROWDEV_ATTR(ch0_ce_count, S_IRUGO | S_IWUSR, channel_ce_count_show, NULL, 0);
CSROWDEV_ATTR(ch1_ce_count, S_IRUGO | S_IWUSR, channel_ce_count_show, NULL, 1);
CSROWDEV_ATTR(ch2_ce_count, S_IRUGO | S_IWUSR, channel_ce_count_show, NULL, 2);
CSROWDEV_ATTR(ch3_ce_count, S_IRUGO | S_IWUSR, channel_ce_count_show, NULL, 3);
CSROWDEV_ATTR(ch4_ce_count, S_IRUGO | S_IWUSR, channel_ce_count_show, NULL, 4);
CSROWDEV_ATTR(ch5_ce_count, S_IRUGO | S_IWUSR, channel_ce_count_show, NULL, 5);

/* Total possible dynamic ce_count attribute file table */
static struct csrowdev_attribute *dynamic_csrow_ce_count_attr[] = {
	&attr_ch0_ce_count,
	&attr_ch1_ce_count,
	&attr_ch2_ce_count,
	&attr_ch3_ce_count,
	&attr_ch4_ce_count,
	&attr_ch5_ce_count
};

#define EDAC_NR_CHANNELS	6

/* Create dynamic CHANNEL files, indexed by 'chan',  under specifed CSROW */
static int edac_create_channel_files(struct kobject *kobj, int chan)
{
	int err = -ENODEV;

	if (chan >= EDAC_NR_CHANNELS)
		return err;

	/* create the DIMM label attribute file */
	err = sysfs_create_file(kobj,
				(struct attribute *)
				dynamic_csrow_dimm_attr[chan]);

	if (!err) {
		/* create the CE Count attribute file */
		err = sysfs_create_file(kobj,
					(struct attribute *)
					dynamic_csrow_ce_count_attr[chan]);
	} else {
		debugf1("%s()  dimm labels and ce_count files created",
			__func__);
=======
static ssize_t channel_ce_count_show(struct device *dev,
				     struct device_attribute *mattr, char *data)
{
	struct csrow_info *csrow = to_csrow(dev);
	unsigned chan = to_channel(mattr);
	struct rank_info *rank = csrow->channels[chan];

	return sprintf(data, "%u\n", rank->ce_count);
}

/* cwrow<id>/attribute files */
DEVICE_ATTR_LEGACY(size_mb, S_IRUGO, csrow_size_show, NULL);
DEVICE_ATTR_LEGACY(dev_type, S_IRUGO, csrow_dev_type_show, NULL);
DEVICE_ATTR_LEGACY(mem_type, S_IRUGO, csrow_mem_type_show, NULL);
DEVICE_ATTR_LEGACY(edac_mode, S_IRUGO, csrow_edac_mode_show, NULL);
DEVICE_ATTR_LEGACY(ue_count, S_IRUGO, csrow_ue_count_show, NULL);
DEVICE_ATTR_LEGACY(ce_count, S_IRUGO, csrow_ce_count_show, NULL);

/* default attributes of the CSROW<id> object */
static struct attribute *csrow_attrs[] = {
	&dev_attr_legacy_dev_type.attr,
	&dev_attr_legacy_mem_type.attr,
	&dev_attr_legacy_edac_mode.attr,
	&dev_attr_legacy_size_mb.attr,
	&dev_attr_legacy_ue_count.attr,
	&dev_attr_legacy_ce_count.attr,
	NULL,
};

static struct attribute_group csrow_attr_grp = {
	.attrs	= csrow_attrs,
};

static const struct attribute_group *csrow_attr_groups[] = {
	&csrow_attr_grp,
	NULL
};

static void csrow_attr_release(struct device *dev)
{
	struct csrow_info *csrow = container_of(dev, struct csrow_info, dev);

	edac_dbg(1, "Releasing csrow device %s\n", dev_name(dev));
	kfree(csrow);
}

static struct device_type csrow_attr_type = {
	.groups		= csrow_attr_groups,
	.release	= csrow_attr_release,
};

/*
 * possible dynamic channel DIMM Label attribute files
 *
 */

#define EDAC_NR_CHANNELS	6

DEVICE_CHANNEL(ch0_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 0);
DEVICE_CHANNEL(ch1_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 1);
DEVICE_CHANNEL(ch2_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 2);
DEVICE_CHANNEL(ch3_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 3);
DEVICE_CHANNEL(ch4_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 4);
DEVICE_CHANNEL(ch5_dimm_label, S_IRUGO | S_IWUSR,
	channel_dimm_label_show, channel_dimm_label_store, 5);

/* Total possible dynamic DIMM Label attribute file table */
static struct device_attribute *dynamic_csrow_dimm_attr[] = {
	&dev_attr_legacy_ch0_dimm_label.attr,
	&dev_attr_legacy_ch1_dimm_label.attr,
	&dev_attr_legacy_ch2_dimm_label.attr,
	&dev_attr_legacy_ch3_dimm_label.attr,
	&dev_attr_legacy_ch4_dimm_label.attr,
	&dev_attr_legacy_ch5_dimm_label.attr
};

/* possible dynamic channel ce_count attribute files */
DEVICE_CHANNEL(ch0_ce_count, S_IRUGO,
		   channel_ce_count_show, NULL, 0);
DEVICE_CHANNEL(ch1_ce_count, S_IRUGO,
		   channel_ce_count_show, NULL, 1);
DEVICE_CHANNEL(ch2_ce_count, S_IRUGO,
		   channel_ce_count_show, NULL, 2);
DEVICE_CHANNEL(ch3_ce_count, S_IRUGO,
		   channel_ce_count_show, NULL, 3);
DEVICE_CHANNEL(ch4_ce_count, S_IRUGO,
		   channel_ce_count_show, NULL, 4);
DEVICE_CHANNEL(ch5_ce_count, S_IRUGO,
		   channel_ce_count_show, NULL, 5);

/* Total possible dynamic ce_count attribute file table */
static struct device_attribute *dynamic_csrow_ce_count_attr[] = {
	&dev_attr_legacy_ch0_ce_count.attr,
	&dev_attr_legacy_ch1_ce_count.attr,
	&dev_attr_legacy_ch2_ce_count.attr,
	&dev_attr_legacy_ch3_ce_count.attr,
	&dev_attr_legacy_ch4_ce_count.attr,
	&dev_attr_legacy_ch5_ce_count.attr
};

static inline int nr_pages_per_csrow(struct csrow_info *csrow)
{
	int chan, nr_pages = 0;

	for (chan = 0; chan < csrow->nr_channels; chan++)
		nr_pages += csrow->channels[chan]->dimm->nr_pages;

	return nr_pages;
}

/* Create a CSROW object under specifed edac_mc_device */
static int edac_create_csrow_object(struct mem_ctl_info *mci,
				    struct csrow_info *csrow, int index)
{
	int err, chan;

	if (csrow->nr_channels >= EDAC_NR_CHANNELS)
		return -ENODEV;

	csrow->dev.type = &csrow_attr_type;
	csrow->dev.bus = mci->bus;
	device_initialize(&csrow->dev);
	csrow->dev.parent = &mci->dev;
	csrow->mci = mci;
	dev_set_name(&csrow->dev, "csrow%d", index);
	dev_set_drvdata(&csrow->dev, csrow);

	edac_dbg(0, "creating (virtual) csrow node %s\n",
		 dev_name(&csrow->dev));

	err = device_add(&csrow->dev);
	if (err < 0)
		return err;

	for (chan = 0; chan < csrow->nr_channels; chan++) {
		/* Only expose populated DIMMs */
		if (!csrow->channels[chan]->dimm->nr_pages)
			continue;
		err = device_create_file(&csrow->dev,
					 dynamic_csrow_dimm_attr[chan]);
		if (err < 0)
			goto error;
		err = device_create_file(&csrow->dev,
					 dynamic_csrow_ce_count_attr[chan]);
		if (err < 0) {
			device_remove_file(&csrow->dev,
					   dynamic_csrow_dimm_attr[chan]);
			goto error;
		}
	}

	return 0;

error:
	for (--chan; chan >= 0; chan--) {
		device_remove_file(&csrow->dev,
					dynamic_csrow_dimm_attr[chan]);
		device_remove_file(&csrow->dev,
					   dynamic_csrow_ce_count_attr[chan]);
	}
	put_device(&csrow->dev);

	return err;
}

/* Create a CSROW object under specifed edac_mc_device */
static int edac_create_csrow_objects(struct mem_ctl_info *mci)
{
	int err, i, chan;
	struct csrow_info *csrow;

	for (i = 0; i < mci->nr_csrows; i++) {
		csrow = mci->csrows[i];
		if (!nr_pages_per_csrow(csrow))
			continue;
		err = edac_create_csrow_object(mci, mci->csrows[i], i);
		if (err < 0) {
			edac_dbg(1,
				 "failure: create csrow objects for csrow %d\n",
				 i);
			goto error;
		}
	}
	return 0;

error:
	for (--i; i >= 0; i--) {
		csrow = mci->csrows[i];
		if (!nr_pages_per_csrow(csrow))
			continue;
		for (chan = csrow->nr_channels - 1; chan >= 0; chan--) {
			if (!csrow->channels[chan]->dimm->nr_pages)
				continue;
			device_remove_file(&csrow->dev,
						dynamic_csrow_dimm_attr[chan]);
			device_remove_file(&csrow->dev,
						dynamic_csrow_ce_count_attr[chan]);
		}
		put_device(&mci->csrows[i]->dev);
>>>>>>> refs/remotes/origin/master
	}

	return err;
}

<<<<<<< HEAD
/* No memory to release for this kobj */
static void edac_csrow_instance_release(struct kobject *kobj)
{
	struct mem_ctl_info *mci;
	struct csrow_info *cs;

	debugf1("%s()\n", __func__);

	cs = container_of(kobj, struct csrow_info, kobj);
	mci = cs->mci;

	kobject_put(&mci->edac_mci_kobj);
}

/* the kobj_type instance for a CSROW */
static struct kobj_type ktype_csrow = {
	.release = edac_csrow_instance_release,
	.sysfs_ops = &csrowfs_ops,
	.default_attrs = (struct attribute **)default_csrow_attr,
};

/* Create a CSROW object under specifed edac_mc_device */
static int edac_create_csrow_object(struct mem_ctl_info *mci,
					struct csrow_info *csrow, int index)
{
	struct kobject *kobj_mci = &mci->edac_mci_kobj;
	struct kobject *kobj;
	int chan;
	int err;

	/* generate ..../edac/mc/mc<id>/csrow<index>   */
	memset(&csrow->kobj, 0, sizeof(csrow->kobj));
	csrow->mci = mci;	/* include container up link */

	/* bump the mci instance's kobject's ref count */
	kobj = kobject_get(&mci->edac_mci_kobj);
	if (!kobj) {
		err = -ENODEV;
		goto err_out;
	}

	/* Instanstiate the csrow object */
	err = kobject_init_and_add(&csrow->kobj, &ktype_csrow, kobj_mci,
				   "csrow%d", index);
	if (err)
		goto err_release_top_kobj;

	/* At this point, to release a csrow kobj, one must
	 * call the kobject_put and allow that tear down
	 * to work the releasing
	 */

	/* Create the dyanmic attribute files on this csrow,
	 * namely, the DIMM labels and the channel ce_count
	 */
	for (chan = 0; chan < csrow->nr_channels; chan++) {
		err = edac_create_channel_files(&csrow->kobj, chan);
		if (err) {
			/* special case the unregister here */
			kobject_put(&csrow->kobj);
			goto err_out;
		}
	}
	kobject_uevent(&csrow->kobj, KOBJ_ADD);
	return 0;

	/* error unwind stack */
err_release_top_kobj:
	kobject_put(&mci->edac_mci_kobj);

err_out:
	return err;
}

/* default sysfs methods and data structures for the main MCI kobject */

static ssize_t mci_reset_counters_store(struct mem_ctl_info *mci,
					const char *data, size_t count)
{
	int row, chan;

	mci->ue_noinfo_count = 0;
	mci->ce_noinfo_count = 0;
	mci->ue_count = 0;
	mci->ce_count = 0;

	for (row = 0; row < mci->nr_csrows; row++) {
		struct csrow_info *ri = &mci->csrows[row];
=======
static void edac_delete_csrow_objects(struct mem_ctl_info *mci)
{
	int i, chan;
	struct csrow_info *csrow;

	for (i = mci->nr_csrows - 1; i >= 0; i--) {
		csrow = mci->csrows[i];
		if (!nr_pages_per_csrow(csrow))
			continue;
		for (chan = csrow->nr_channels - 1; chan >= 0; chan--) {
			if (!csrow->channels[chan]->dimm->nr_pages)
				continue;
			edac_dbg(1, "Removing csrow %d channel %d sysfs nodes\n",
				 i, chan);
			device_remove_file(&csrow->dev,
						dynamic_csrow_dimm_attr[chan]);
			device_remove_file(&csrow->dev,
						dynamic_csrow_ce_count_attr[chan]);
		}
		device_unregister(&mci->csrows[i]->dev);
	}
}
#endif

/*
 * Per-dimm (or per-rank) devices
 */

#define to_dimm(k) container_of(k, struct dimm_info, dev)

/* show/store functions for DIMM Label attributes */
static ssize_t dimmdev_location_show(struct device *dev,
				     struct device_attribute *mattr, char *data)
{
	struct dimm_info *dimm = to_dimm(dev);

	return edac_dimm_info_location(dimm, data, PAGE_SIZE);
}

static ssize_t dimmdev_label_show(struct device *dev,
				  struct device_attribute *mattr, char *data)
{
	struct dimm_info *dimm = to_dimm(dev);

	/* if field has not been initialized, there is nothing to send */
	if (!dimm->label[0])
		return 0;

	return snprintf(data, EDAC_MC_LABEL_LEN, "%s\n", dimm->label);
}

static ssize_t dimmdev_label_store(struct device *dev,
				   struct device_attribute *mattr,
				   const char *data,
				   size_t count)
{
	struct dimm_info *dimm = to_dimm(dev);

	ssize_t max_size = 0;

	max_size = min((ssize_t) count, (ssize_t) EDAC_MC_LABEL_LEN - 1);
	strncpy(dimm->label, data, max_size);
	dimm->label[max_size] = '\0';

	return max_size;
}

static ssize_t dimmdev_size_show(struct device *dev,
				 struct device_attribute *mattr, char *data)
{
	struct dimm_info *dimm = to_dimm(dev);

	return sprintf(data, "%u\n", PAGES_TO_MiB(dimm->nr_pages));
}

static ssize_t dimmdev_mem_type_show(struct device *dev,
				     struct device_attribute *mattr, char *data)
{
	struct dimm_info *dimm = to_dimm(dev);

	return sprintf(data, "%s\n", mem_types[dimm->mtype]);
}

static ssize_t dimmdev_dev_type_show(struct device *dev,
				     struct device_attribute *mattr, char *data)
{
	struct dimm_info *dimm = to_dimm(dev);

	return sprintf(data, "%s\n", dev_types[dimm->dtype]);
}

static ssize_t dimmdev_edac_mode_show(struct device *dev,
				      struct device_attribute *mattr,
				      char *data)
{
	struct dimm_info *dimm = to_dimm(dev);

	return sprintf(data, "%s\n", edac_caps[dimm->edac_mode]);
}

/* dimm/rank attribute files */
static DEVICE_ATTR(dimm_label, S_IRUGO | S_IWUSR,
		   dimmdev_label_show, dimmdev_label_store);
static DEVICE_ATTR(dimm_location, S_IRUGO, dimmdev_location_show, NULL);
static DEVICE_ATTR(size, S_IRUGO, dimmdev_size_show, NULL);
static DEVICE_ATTR(dimm_mem_type, S_IRUGO, dimmdev_mem_type_show, NULL);
static DEVICE_ATTR(dimm_dev_type, S_IRUGO, dimmdev_dev_type_show, NULL);
static DEVICE_ATTR(dimm_edac_mode, S_IRUGO, dimmdev_edac_mode_show, NULL);

/* attributes of the dimm<id>/rank<id> object */
static struct attribute *dimm_attrs[] = {
	&dev_attr_dimm_label.attr,
	&dev_attr_dimm_location.attr,
	&dev_attr_size.attr,
	&dev_attr_dimm_mem_type.attr,
	&dev_attr_dimm_dev_type.attr,
	&dev_attr_dimm_edac_mode.attr,
	NULL,
};

static struct attribute_group dimm_attr_grp = {
	.attrs	= dimm_attrs,
};

static const struct attribute_group *dimm_attr_groups[] = {
	&dimm_attr_grp,
	NULL
};

static void dimm_attr_release(struct device *dev)
{
	struct dimm_info *dimm = container_of(dev, struct dimm_info, dev);

	edac_dbg(1, "Releasing dimm device %s\n", dev_name(dev));
	kfree(dimm);
}

static struct device_type dimm_attr_type = {
	.groups		= dimm_attr_groups,
	.release	= dimm_attr_release,
};

/* Create a DIMM object under specifed memory controller device */
static int edac_create_dimm_object(struct mem_ctl_info *mci,
				   struct dimm_info *dimm,
				   int index)
{
	int err;
	dimm->mci = mci;

	dimm->dev.type = &dimm_attr_type;
	dimm->dev.bus = mci->bus;
	device_initialize(&dimm->dev);

	dimm->dev.parent = &mci->dev;
	if (mci->csbased)
		dev_set_name(&dimm->dev, "rank%d", index);
	else
		dev_set_name(&dimm->dev, "dimm%d", index);
	dev_set_drvdata(&dimm->dev, dimm);
	pm_runtime_forbid(&mci->dev);

	err =  device_add(&dimm->dev);

	edac_dbg(0, "creating rank/dimm device %s\n", dev_name(&dimm->dev));

	return err;
}

/*
 * Memory controller device
 */

#define to_mci(k) container_of(k, struct mem_ctl_info, dev)

static ssize_t mci_reset_counters_store(struct device *dev,
					struct device_attribute *mattr,
					const char *data, size_t count)
{
	struct mem_ctl_info *mci = to_mci(dev);
	int cnt, row, chan, i;
	mci->ue_mc = 0;
	mci->ce_mc = 0;
	mci->ue_noinfo_count = 0;
	mci->ce_noinfo_count = 0;

	for (row = 0; row < mci->nr_csrows; row++) {
		struct csrow_info *ri = mci->csrows[row];
>>>>>>> refs/remotes/origin/master

		ri->ue_count = 0;
		ri->ce_count = 0;

		for (chan = 0; chan < ri->nr_channels; chan++)
<<<<<<< HEAD
			ri->channels[chan].ce_count = 0;
=======
			ri->channels[chan]->ce_count = 0;
	}

	cnt = 1;
	for (i = 0; i < mci->n_layers; i++) {
		cnt *= mci->layers[i].size;
		memset(mci->ce_per_layer[i], 0, cnt * sizeof(u32));
		memset(mci->ue_per_layer[i], 0, cnt * sizeof(u32));
>>>>>>> refs/remotes/origin/master
	}

	mci->start_time = jiffies;
	return count;
}

/* Memory scrubbing interface:
 *
 * A MC driver can limit the scrubbing bandwidth based on the CPU type.
 * Therefore, ->set_sdram_scrub_rate should be made to return the actual
 * bandwidth that is accepted or 0 when scrubbing is to be disabled.
 *
 * Negative value still means that an error has occurred while setting
 * the scrub rate.
 */
<<<<<<< HEAD
static ssize_t mci_sdram_scrub_rate_store(struct mem_ctl_info *mci,
					  const char *data, size_t count)
{
	unsigned long bandwidth = 0;
	int new_bw = 0;

	if (!mci->set_sdram_scrub_rate)
<<<<<<< HEAD
		return -EINVAL;
=======
		return -ENODEV;
>>>>>>> refs/remotes/origin/cm-10.0

	if (strict_strtoul(data, 10, &bandwidth) < 0)
=======
static ssize_t mci_sdram_scrub_rate_store(struct device *dev,
					  struct device_attribute *mattr,
					  const char *data, size_t count)
{
	struct mem_ctl_info *mci = to_mci(dev);
	unsigned long bandwidth = 0;
	int new_bw = 0;

	if (kstrtoul(data, 10, &bandwidth) < 0)
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	new_bw = mci->set_sdram_scrub_rate(mci, bandwidth);
	if (new_bw < 0) {
		edac_printk(KERN_WARNING, EDAC_MC,
			    "Error setting scrub rate to: %lu\n", bandwidth);
		return -EINVAL;
	}

	return count;
}

/*
 * ->get_sdram_scrub_rate() return value semantics same as above.
 */
<<<<<<< HEAD
static ssize_t mci_sdram_scrub_rate_show(struct mem_ctl_info *mci, char *data)
{
	int bandwidth = 0;

	if (!mci->get_sdram_scrub_rate)
<<<<<<< HEAD
		return -EINVAL;
=======
		return -ENODEV;
>>>>>>> refs/remotes/origin/cm-10.0

=======
static ssize_t mci_sdram_scrub_rate_show(struct device *dev,
					 struct device_attribute *mattr,
					 char *data)
{
	struct mem_ctl_info *mci = to_mci(dev);
	int bandwidth = 0;

>>>>>>> refs/remotes/origin/master
	bandwidth = mci->get_sdram_scrub_rate(mci);
	if (bandwidth < 0) {
		edac_printk(KERN_DEBUG, EDAC_MC, "Error reading scrub rate\n");
		return bandwidth;
	}

	return sprintf(data, "%d\n", bandwidth);
}

/* default attribute files for the MCI object */
<<<<<<< HEAD
static ssize_t mci_ue_count_show(struct mem_ctl_info *mci, char *data)
{
	return sprintf(data, "%d\n", mci->ue_count);
}

static ssize_t mci_ce_count_show(struct mem_ctl_info *mci, char *data)
{
	return sprintf(data, "%d\n", mci->ce_count);
}

static ssize_t mci_ce_noinfo_show(struct mem_ctl_info *mci, char *data)
{
	return sprintf(data, "%d\n", mci->ce_noinfo_count);
}

static ssize_t mci_ue_noinfo_show(struct mem_ctl_info *mci, char *data)
{
	return sprintf(data, "%d\n", mci->ue_noinfo_count);
}

static ssize_t mci_seconds_show(struct mem_ctl_info *mci, char *data)
{
	return sprintf(data, "%ld\n", (jiffies - mci->start_time) / HZ);
}

static ssize_t mci_ctl_name_show(struct mem_ctl_info *mci, char *data)
{
	return sprintf(data, "%s\n", mci->ctl_name);
}

static ssize_t mci_size_mb_show(struct mem_ctl_info *mci, char *data)
{
	int total_pages, csrow_idx;

	for (total_pages = csrow_idx = 0; csrow_idx < mci->nr_csrows;
		csrow_idx++) {
		struct csrow_info *csrow = &mci->csrows[csrow_idx];

		if (!csrow->nr_pages)
			continue;

		total_pages += csrow->nr_pages;
=======
static ssize_t mci_ue_count_show(struct device *dev,
				 struct device_attribute *mattr,
				 char *data)
{
	struct mem_ctl_info *mci = to_mci(dev);

	return sprintf(data, "%d\n", mci->ue_mc);
}

static ssize_t mci_ce_count_show(struct device *dev,
				 struct device_attribute *mattr,
				 char *data)
{
	struct mem_ctl_info *mci = to_mci(dev);

	return sprintf(data, "%d\n", mci->ce_mc);
}

static ssize_t mci_ce_noinfo_show(struct device *dev,
				  struct device_attribute *mattr,
				  char *data)
{
	struct mem_ctl_info *mci = to_mci(dev);

	return sprintf(data, "%d\n", mci->ce_noinfo_count);
}

static ssize_t mci_ue_noinfo_show(struct device *dev,
				  struct device_attribute *mattr,
				  char *data)
{
	struct mem_ctl_info *mci = to_mci(dev);

	return sprintf(data, "%d\n", mci->ue_noinfo_count);
}

static ssize_t mci_seconds_show(struct device *dev,
				struct device_attribute *mattr,
				char *data)
{
	struct mem_ctl_info *mci = to_mci(dev);

	return sprintf(data, "%ld\n", (jiffies - mci->start_time) / HZ);
}

static ssize_t mci_ctl_name_show(struct device *dev,
				 struct device_attribute *mattr,
				 char *data)
{
	struct mem_ctl_info *mci = to_mci(dev);

	return sprintf(data, "%s\n", mci->ctl_name);
}

static ssize_t mci_size_mb_show(struct device *dev,
				struct device_attribute *mattr,
				char *data)
{
	struct mem_ctl_info *mci = to_mci(dev);
	int total_pages = 0, csrow_idx, j;

	for (csrow_idx = 0; csrow_idx < mci->nr_csrows; csrow_idx++) {
		struct csrow_info *csrow = mci->csrows[csrow_idx];

		for (j = 0; j < csrow->nr_channels; j++) {
			struct dimm_info *dimm = csrow->channels[j]->dimm;

			total_pages += dimm->nr_pages;
		}
>>>>>>> refs/remotes/origin/master
	}

	return sprintf(data, "%u\n", PAGES_TO_MiB(total_pages));
}

<<<<<<< HEAD
#define to_mci(k) container_of(k, struct mem_ctl_info, edac_mci_kobj)
#define to_mcidev_attr(a) container_of(a,struct mcidev_sysfs_attribute,attr)

/* MCI show/store functions for top most object */
static ssize_t mcidev_show(struct kobject *kobj, struct attribute *attr,
			char *buffer)
{
	struct mem_ctl_info *mem_ctl_info = to_mci(kobj);
	struct mcidev_sysfs_attribute *mcidev_attr = to_mcidev_attr(attr);

	debugf1("%s() mem_ctl_info %p\n", __func__, mem_ctl_info);

	if (mcidev_attr->show)
		return mcidev_attr->show(mem_ctl_info, buffer);

	return -EIO;
}

static ssize_t mcidev_store(struct kobject *kobj, struct attribute *attr,
			const char *buffer, size_t count)
{
	struct mem_ctl_info *mem_ctl_info = to_mci(kobj);
	struct mcidev_sysfs_attribute *mcidev_attr = to_mcidev_attr(attr);

	debugf1("%s() mem_ctl_info %p\n", __func__, mem_ctl_info);

	if (mcidev_attr->store)
		return mcidev_attr->store(mem_ctl_info, buffer, count);

	return -EIO;
}

/* Intermediate show/store table */
static const struct sysfs_ops mci_ops = {
	.show = mcidev_show,
	.store = mcidev_store
};

#define MCIDEV_ATTR(_name,_mode,_show,_store)			\
static struct mcidev_sysfs_attribute mci_attr_##_name = {			\
	.attr = {.name = __stringify(_name), .mode = _mode },	\
	.show   = _show,					\
	.store  = _store,					\
};

/* default Control file */
MCIDEV_ATTR(reset_counters, S_IWUSR, NULL, mci_reset_counters_store);

/* default Attribute files */
MCIDEV_ATTR(mc_name, S_IRUGO, mci_ctl_name_show, NULL);
MCIDEV_ATTR(size_mb, S_IRUGO, mci_size_mb_show, NULL);
MCIDEV_ATTR(seconds_since_reset, S_IRUGO, mci_seconds_show, NULL);
MCIDEV_ATTR(ue_noinfo_count, S_IRUGO, mci_ue_noinfo_show, NULL);
MCIDEV_ATTR(ce_noinfo_count, S_IRUGO, mci_ce_noinfo_show, NULL);
MCIDEV_ATTR(ue_count, S_IRUGO, mci_ue_count_show, NULL);
MCIDEV_ATTR(ce_count, S_IRUGO, mci_ce_count_show, NULL);

/* memory scrubber attribute file */
MCIDEV_ATTR(sdram_scrub_rate, S_IRUGO | S_IWUSR, mci_sdram_scrub_rate_show,
	mci_sdram_scrub_rate_store);

static struct mcidev_sysfs_attribute *mci_attr[] = {
	&mci_attr_reset_counters,
	&mci_attr_mc_name,
	&mci_attr_size_mb,
	&mci_attr_seconds_since_reset,
	&mci_attr_ue_noinfo_count,
	&mci_attr_ce_noinfo_count,
	&mci_attr_ue_count,
	&mci_attr_ce_count,
	&mci_attr_sdram_scrub_rate,
	NULL
};


/*
 * Release of a MC controlling instance
 *
 *	each MC control instance has the following resources upon entry:
 *		a) a ref count on the top memctl kobj
 *		b) a ref count on this module
 *
 *	this function must decrement those ref counts and then
 *	issue a free on the instance's memory
 */
static void edac_mci_control_release(struct kobject *kobj)
{
	struct mem_ctl_info *mci;

	mci = to_mci(kobj);

	debugf0("%s() mci instance idx=%d releasing\n", __func__, mci->mc_idx);

	/* decrement the module ref count */
	module_put(mci->owner);
}

static struct kobj_type ktype_mci = {
	.release = edac_mci_control_release,
	.sysfs_ops = &mci_ops,
	.default_attrs = (struct attribute **)mci_attr,
};

/* EDAC memory controller sysfs kset:
 *	/sys/devices/system/edac/mc
 */
static struct kset *mc_kset;

/*
 * edac_mc_register_sysfs_main_kobj
 *
 *	setups and registers the main kobject for each mci
 */
int edac_mc_register_sysfs_main_kobj(struct mem_ctl_info *mci)
{
	struct kobject *kobj_mci;
	int err;

	debugf1("%s()\n", __func__);

	kobj_mci = &mci->edac_mci_kobj;

	/* Init the mci's kobject */
	memset(kobj_mci, 0, sizeof(*kobj_mci));

	/* Record which module 'owns' this control structure
	 * and bump the ref count of the module
	 */
	mci->owner = THIS_MODULE;

	/* bump ref count on this module */
	if (!try_module_get(mci->owner)) {
		err = -ENODEV;
		goto fail_out;
	}

	/* this instance become part of the mc_kset */
	kobj_mci->kset = mc_kset;

	/* register the mc<id> kobject to the mc_kset */
	err = kobject_init_and_add(kobj_mci, &ktype_mci, NULL,
				   "mc%d", mci->mc_idx);
	if (err) {
		debugf1("%s()Failed to register '.../edac/mc%d'\n",
			__func__, mci->mc_idx);
		goto kobj_reg_fail;
	}
	kobject_uevent(kobj_mci, KOBJ_ADD);

	/* At this point, to 'free' the control struct,
	 * edac_mc_unregister_sysfs_main_kobj() must be used
	 */

	debugf1("%s() Registered '.../edac/mc%d' kobject\n",
		__func__, mci->mc_idx);

	return 0;

	/* Error exit stack */

kobj_reg_fail:
	module_put(mci->owner);

fail_out:
	return err;
}

/*
 * edac_mc_register_sysfs_main_kobj
 *
 *	tears down and the main mci kobject from the mc_kset
 */
void edac_mc_unregister_sysfs_main_kobj(struct mem_ctl_info *mci)
{
	debugf1("%s()\n", __func__);

	/* delete the kobj from the mc_kset */
	kobject_put(&mci->edac_mci_kobj);
}

#define EDAC_DEVICE_SYMLINK	"device"

#define grp_to_mci(k) (container_of(k, struct mcidev_sysfs_group_kobj, kobj)->mci)

/* MCI show/store functions for top most object */
static ssize_t inst_grp_show(struct kobject *kobj, struct attribute *attr,
			char *buffer)
{
	struct mem_ctl_info *mem_ctl_info = grp_to_mci(kobj);
	struct mcidev_sysfs_attribute *mcidev_attr = to_mcidev_attr(attr);

	debugf1("%s() mem_ctl_info %p\n", __func__, mem_ctl_info);

	if (mcidev_attr->show)
		return mcidev_attr->show(mem_ctl_info, buffer);

	return -EIO;
}

static ssize_t inst_grp_store(struct kobject *kobj, struct attribute *attr,
			const char *buffer, size_t count)
{
	struct mem_ctl_info *mem_ctl_info = grp_to_mci(kobj);
	struct mcidev_sysfs_attribute *mcidev_attr = to_mcidev_attr(attr);

	debugf1("%s() mem_ctl_info %p\n", __func__, mem_ctl_info);

	if (mcidev_attr->store)
		return mcidev_attr->store(mem_ctl_info, buffer, count);

	return -EIO;
}

/* No memory to release for this kobj */
static void edac_inst_grp_release(struct kobject *kobj)
{
	struct mcidev_sysfs_group_kobj *grp;
	struct mem_ctl_info *mci;

	debugf1("%s()\n", __func__);

	grp = container_of(kobj, struct mcidev_sysfs_group_kobj, kobj);
	mci = grp->mci;
}

/* Intermediate show/store table */
static struct sysfs_ops inst_grp_ops = {
	.show = inst_grp_show,
	.store = inst_grp_store
};

/* the kobj_type instance for a instance group */
static struct kobj_type ktype_inst_grp = {
	.release = edac_inst_grp_release,
	.sysfs_ops = &inst_grp_ops,
};


/*
 * edac_create_mci_instance_attributes
 *	create MC driver specific attributes bellow an specified kobj
 * This routine calls itself recursively, in order to create an entire
 * object tree.
 */
static int edac_create_mci_instance_attributes(struct mem_ctl_info *mci,
				const struct mcidev_sysfs_attribute *sysfs_attrib,
				struct kobject *kobj)
{
	int err;

	debugf4("%s()\n", __func__);

	while (sysfs_attrib) {
		debugf4("%s() sysfs_attrib = %p\n",__func__, sysfs_attrib);
		if (sysfs_attrib->grp) {
			struct mcidev_sysfs_group_kobj *grp_kobj;

			grp_kobj = kzalloc(sizeof(*grp_kobj), GFP_KERNEL);
			if (!grp_kobj)
				return -ENOMEM;

			grp_kobj->grp = sysfs_attrib->grp;
			grp_kobj->mci = mci;
			list_add_tail(&grp_kobj->list, &mci->grp_kobj_list);

			debugf0("%s() grp %s, mci %p\n", __func__,
				sysfs_attrib->grp->name, mci);

			err = kobject_init_and_add(&grp_kobj->kobj,
						&ktype_inst_grp,
						&mci->edac_mci_kobj,
						sysfs_attrib->grp->name);
			if (err < 0) {
				printk(KERN_ERR "kobject_init_and_add failed: %d\n", err);
				return err;
			}
			err = edac_create_mci_instance_attributes(mci,
					grp_kobj->grp->mcidev_attr,
					&grp_kobj->kobj);

			if (err < 0)
				return err;
		} else if (sysfs_attrib->attr.name) {
			debugf4("%s() file %s\n", __func__,
				sysfs_attrib->attr.name);

			err = sysfs_create_file(kobj, &sysfs_attrib->attr);
			if (err < 0) {
				printk(KERN_ERR "sysfs_create_file failed: %d\n", err);
				return err;
			}
		} else
			break;

		sysfs_attrib++;
	}

	return 0;
}

/*
 * edac_remove_mci_instance_attributes
 *	remove MC driver specific attributes at the topmost level
 *	directory of this mci instance.
 */
static void edac_remove_mci_instance_attributes(struct mem_ctl_info *mci,
				const struct mcidev_sysfs_attribute *sysfs_attrib,
				struct kobject *kobj, int count)
{
	struct mcidev_sysfs_group_kobj *grp_kobj, *tmp;

	debugf1("%s()\n", __func__);

	/*
	 * loop if there are attributes and until we hit a NULL entry
	 * Remove first all the attributes
	 */
	while (sysfs_attrib) {
		debugf4("%s() sysfs_attrib = %p\n",__func__, sysfs_attrib);
		if (sysfs_attrib->grp) {
			debugf4("%s() seeking for group %s\n",
				__func__, sysfs_attrib->grp->name);
			list_for_each_entry(grp_kobj,
					    &mci->grp_kobj_list, list) {
				debugf4("%s() grp_kobj->grp = %p\n",__func__, grp_kobj->grp);
				if (grp_kobj->grp == sysfs_attrib->grp) {
					edac_remove_mci_instance_attributes(mci,
						    grp_kobj->grp->mcidev_attr,
						    &grp_kobj->kobj, count + 1);
					debugf4("%s() group %s\n", __func__,
						sysfs_attrib->grp->name);
					kobject_put(&grp_kobj->kobj);
				}
			}
			debugf4("%s() end of seeking for group %s\n",
				__func__, sysfs_attrib->grp->name);
		} else if (sysfs_attrib->attr.name) {
			debugf4("%s() file %s\n", __func__,
				sysfs_attrib->attr.name);
			sysfs_remove_file(kobj, &sysfs_attrib->attr);
		} else
			break;
		sysfs_attrib++;
	}

	/* Remove the group objects */
	if (count)
		return;
	list_for_each_entry_safe(grp_kobj, tmp,
				 &mci->grp_kobj_list, list) {
		list_del(&grp_kobj->list);
		kfree(grp_kobj);
	}
}

=======
static ssize_t mci_max_location_show(struct device *dev,
				     struct device_attribute *mattr,
				     char *data)
{
	struct mem_ctl_info *mci = to_mci(dev);
	int i;
	char *p = data;

	for (i = 0; i < mci->n_layers; i++) {
		p += sprintf(p, "%s %d ",
			     edac_layer_name[mci->layers[i].type],
			     mci->layers[i].size - 1);
	}

	return p - data;
}

#ifdef CONFIG_EDAC_DEBUG
static ssize_t edac_fake_inject_write(struct file *file,
				      const char __user *data,
				      size_t count, loff_t *ppos)
{
	struct device *dev = file->private_data;
	struct mem_ctl_info *mci = to_mci(dev);
	static enum hw_event_mc_err_type type;
	u16 errcount = mci->fake_inject_count;

	if (!errcount)
		errcount = 1;

	type = mci->fake_inject_ue ? HW_EVENT_ERR_UNCORRECTED
				   : HW_EVENT_ERR_CORRECTED;

	printk(KERN_DEBUG
	       "Generating %d %s fake error%s to %d.%d.%d to test core handling. NOTE: this won't test the driver-specific decoding logic.\n",
		errcount,
		(type == HW_EVENT_ERR_UNCORRECTED) ? "UE" : "CE",
		errcount > 1 ? "s" : "",
		mci->fake_inject_layer[0],
		mci->fake_inject_layer[1],
		mci->fake_inject_layer[2]
	       );
	edac_mc_handle_error(type, mci, errcount, 0, 0, 0,
			     mci->fake_inject_layer[0],
			     mci->fake_inject_layer[1],
			     mci->fake_inject_layer[2],
			     "FAKE ERROR", "for EDAC testing only");

	return count;
}

static const struct file_operations debug_fake_inject_fops = {
	.open = simple_open,
	.write = edac_fake_inject_write,
	.llseek = generic_file_llseek,
};
#endif

/* default Control file */
DEVICE_ATTR(reset_counters, S_IWUSR, NULL, mci_reset_counters_store);

/* default Attribute files */
DEVICE_ATTR(mc_name, S_IRUGO, mci_ctl_name_show, NULL);
DEVICE_ATTR(size_mb, S_IRUGO, mci_size_mb_show, NULL);
DEVICE_ATTR(seconds_since_reset, S_IRUGO, mci_seconds_show, NULL);
DEVICE_ATTR(ue_noinfo_count, S_IRUGO, mci_ue_noinfo_show, NULL);
DEVICE_ATTR(ce_noinfo_count, S_IRUGO, mci_ce_noinfo_show, NULL);
DEVICE_ATTR(ue_count, S_IRUGO, mci_ue_count_show, NULL);
DEVICE_ATTR(ce_count, S_IRUGO, mci_ce_count_show, NULL);
DEVICE_ATTR(max_location, S_IRUGO, mci_max_location_show, NULL);

/* memory scrubber attribute file */
DEVICE_ATTR(sdram_scrub_rate, 0, NULL, NULL);

static struct attribute *mci_attrs[] = {
	&dev_attr_reset_counters.attr,
	&dev_attr_mc_name.attr,
	&dev_attr_size_mb.attr,
	&dev_attr_seconds_since_reset.attr,
	&dev_attr_ue_noinfo_count.attr,
	&dev_attr_ce_noinfo_count.attr,
	&dev_attr_ue_count.attr,
	&dev_attr_ce_count.attr,
	&dev_attr_max_location.attr,
	NULL
};

static struct attribute_group mci_attr_grp = {
	.attrs	= mci_attrs,
};

static const struct attribute_group *mci_attr_groups[] = {
	&mci_attr_grp,
	NULL
};

static void mci_attr_release(struct device *dev)
{
	struct mem_ctl_info *mci = container_of(dev, struct mem_ctl_info, dev);

	edac_dbg(1, "Releasing csrow device %s\n", dev_name(dev));
	kfree(mci);
}

static struct device_type mci_attr_type = {
	.groups		= mci_attr_groups,
	.release	= mci_attr_release,
};

#ifdef CONFIG_EDAC_DEBUG
static struct dentry *edac_debugfs;

int __init edac_debugfs_init(void)
{
	edac_debugfs = debugfs_create_dir("edac", NULL);
	if (IS_ERR(edac_debugfs)) {
		edac_debugfs = NULL;
		return -ENOMEM;
	}
	return 0;
}

void __exit edac_debugfs_exit(void)
{
	debugfs_remove(edac_debugfs);
}

static int edac_create_debug_nodes(struct mem_ctl_info *mci)
{
	struct dentry *d, *parent;
	char name[80];
	int i;

	if (!edac_debugfs)
		return -ENODEV;

	d = debugfs_create_dir(mci->dev.kobj.name, edac_debugfs);
	if (!d)
		return -ENOMEM;
	parent = d;

	for (i = 0; i < mci->n_layers; i++) {
		sprintf(name, "fake_inject_%s",
			     edac_layer_name[mci->layers[i].type]);
		d = debugfs_create_u8(name, S_IRUGO | S_IWUSR, parent,
				      &mci->fake_inject_layer[i]);
		if (!d)
			goto nomem;
	}

	d = debugfs_create_bool("fake_inject_ue", S_IRUGO | S_IWUSR, parent,
				&mci->fake_inject_ue);
	if (!d)
		goto nomem;

	d = debugfs_create_u16("fake_inject_count", S_IRUGO | S_IWUSR, parent,
				&mci->fake_inject_count);
	if (!d)
		goto nomem;

	d = debugfs_create_file("fake_inject", S_IWUSR, parent,
				&mci->dev,
				&debug_fake_inject_fops);
	if (!d)
		goto nomem;

	mci->debugfs = parent;
	return 0;
nomem:
	debugfs_remove(mci->debugfs);
	return -ENOMEM;
}
#endif
>>>>>>> refs/remotes/origin/master

/*
 * Create a new Memory Controller kobject instance,
 *	mc<id> under the 'mc' directory
 *
 * Return:
 *	0	Success
 *	!0	Failure
 */
int edac_create_sysfs_mci_device(struct mem_ctl_info *mci)
{
<<<<<<< HEAD
	int i;
	int err;
	struct csrow_info *csrow;
	struct kobject *kobj_mci = &mci->edac_mci_kobj;

	debugf0("%s() idx=%d\n", __func__, mci->mc_idx);

	INIT_LIST_HEAD(&mci->grp_kobj_list);

	/* create a symlink for the device */
	err = sysfs_create_link(kobj_mci, &mci->dev->kobj,
				EDAC_DEVICE_SYMLINK);
	if (err) {
		debugf1("%s() failure to create symlink\n", __func__);
		goto fail0;
	}

	/* If the low level driver desires some attributes,
	 * then create them now for the driver.
	 */
	if (mci->mc_driver_sysfs_attributes) {
		err = edac_create_mci_instance_attributes(mci,
					mci->mc_driver_sysfs_attributes,
					&mci->edac_mci_kobj);
		if (err) {
			debugf1("%s() failure to create mci attributes\n",
				__func__);
			goto fail0;
		}
	}

	/* Make directories for each CSROW object under the mc<id> kobject
	 */
	for (i = 0; i < mci->nr_csrows; i++) {
		csrow = &mci->csrows[i];

		/* Only expose populated CSROWs */
		if (csrow->nr_pages > 0) {
			err = edac_create_csrow_object(mci, csrow, i);
			if (err) {
				debugf1("%s() failure: create csrow %d obj\n",
					__func__, i);
				goto fail1;
			}
		}
	}

	return 0;

	/* CSROW error: backout what has already been registered,  */
fail1:
	for (i--; i >= 0; i--) {
		if (csrow->nr_pages > 0) {
			kobject_put(&mci->csrows[i].kobj);
		}
	}

	/* remove the mci instance's attributes, if any */
	edac_remove_mci_instance_attributes(mci,
		mci->mc_driver_sysfs_attributes, &mci->edac_mci_kobj, 0);

	/* remove the symlink */
	sysfs_remove_link(kobj_mci, EDAC_DEVICE_SYMLINK);

fail0:
=======
	int i, err;

	/*
	 * The memory controller needs its own bus, in order to avoid
	 * namespace conflicts at /sys/bus/edac.
	 */
	mci->bus->name = kasprintf(GFP_KERNEL, "mc%d", mci->mc_idx);
	if (!mci->bus->name)
		return -ENOMEM;

	edac_dbg(0, "creating bus %s\n", mci->bus->name);

	err = bus_register(mci->bus);
	if (err < 0)
		return err;

	/* get the /sys/devices/system/edac subsys reference */
	mci->dev.type = &mci_attr_type;
	device_initialize(&mci->dev);

	mci->dev.parent = mci_pdev;
	mci->dev.bus = mci->bus;
	dev_set_name(&mci->dev, "mc%d", mci->mc_idx);
	dev_set_drvdata(&mci->dev, mci);
	pm_runtime_forbid(&mci->dev);

	edac_dbg(0, "creating device %s\n", dev_name(&mci->dev));
	err = device_add(&mci->dev);
	if (err < 0) {
		edac_dbg(1, "failure: create device %s\n", dev_name(&mci->dev));
		bus_unregister(mci->bus);
		kfree(mci->bus->name);
		return err;
	}

	if (mci->set_sdram_scrub_rate || mci->get_sdram_scrub_rate) {
		if (mci->get_sdram_scrub_rate) {
			dev_attr_sdram_scrub_rate.attr.mode |= S_IRUGO;
			dev_attr_sdram_scrub_rate.show = &mci_sdram_scrub_rate_show;
		}
		if (mci->set_sdram_scrub_rate) {
			dev_attr_sdram_scrub_rate.attr.mode |= S_IWUSR;
			dev_attr_sdram_scrub_rate.store = &mci_sdram_scrub_rate_store;
		}
		err = device_create_file(&mci->dev,
					 &dev_attr_sdram_scrub_rate);
		if (err) {
			edac_dbg(1, "failure: create sdram_scrub_rate\n");
			goto fail2;
		}
	}
	/*
	 * Create the dimm/rank devices
	 */
	for (i = 0; i < mci->tot_dimms; i++) {
		struct dimm_info *dimm = mci->dimms[i];
		/* Only expose populated DIMMs */
		if (dimm->nr_pages == 0)
			continue;
#ifdef CONFIG_EDAC_DEBUG
		edac_dbg(1, "creating dimm%d, located at ", i);
		if (edac_debug_level >= 1) {
			int lay;
			for (lay = 0; lay < mci->n_layers; lay++)
				printk(KERN_CONT "%s %d ",
					edac_layer_name[mci->layers[lay].type],
					dimm->location[lay]);
			printk(KERN_CONT "\n");
		}
#endif
		err = edac_create_dimm_object(mci, dimm, i);
		if (err) {
			edac_dbg(1, "failure: create dimm %d obj\n", i);
			goto fail;
		}
	}

#ifdef CONFIG_EDAC_LEGACY_SYSFS
	err = edac_create_csrow_objects(mci);
	if (err < 0)
		goto fail;
#endif

#ifdef CONFIG_EDAC_DEBUG
	edac_create_debug_nodes(mci);
#endif
	return 0;

fail:
	for (i--; i >= 0; i--) {
		struct dimm_info *dimm = mci->dimms[i];
		if (dimm->nr_pages == 0)
			continue;
		device_unregister(&dimm->dev);
	}
fail2:
	device_unregister(&mci->dev);
	bus_unregister(mci->bus);
	kfree(mci->bus->name);
>>>>>>> refs/remotes/origin/master
	return err;
}

/*
 * remove a Memory Controller instance
 */
void edac_remove_sysfs_mci_device(struct mem_ctl_info *mci)
{
	int i;

<<<<<<< HEAD
	debugf0("%s()\n", __func__);

	/* remove all csrow kobjects */
	debugf4("%s()  unregister this mci kobj\n", __func__);
	for (i = 0; i < mci->nr_csrows; i++) {
		if (mci->csrows[i].nr_pages > 0) {
			debugf0("%s()  unreg csrow-%d\n", __func__, i);
			kobject_put(&mci->csrows[i].kobj);
		}
	}

	/* remove this mci instance's attribtes */
	if (mci->mc_driver_sysfs_attributes) {
		debugf4("%s()  unregister mci private attributes\n", __func__);
		edac_remove_mci_instance_attributes(mci,
						mci->mc_driver_sysfs_attributes,
						&mci->edac_mci_kobj, 0);
	}

	/* remove the symlink */
	debugf4("%s()  remove_link\n", __func__);
	sysfs_remove_link(&mci->edac_mci_kobj, EDAC_DEVICE_SYMLINK);

	/* unregister this instance's kobject */
	debugf4("%s()  remove_mci_instance\n", __func__);
	kobject_put(&mci->edac_mci_kobj);
}




/*
 * edac_setup_sysfs_mc_kset(void)
 *
 * Initialize the mc_kset for the 'mc' entry
 *	This requires creating the top 'mc' directory with a kset
 *	and its controls/attributes.
 *
 *	To this 'mc' kset, instance 'mci' will be grouped as children.
 *
 * Return:  0 SUCCESS
 *         !0 FAILURE error code
 */
int edac_sysfs_setup_mc_kset(void)
{
	int err = -EINVAL;
<<<<<<< HEAD
	struct sysdev_class *edac_class;

	debugf1("%s()\n", __func__);

	/* get the /sys/devices/system/edac class reference */
	edac_class = edac_get_sysfs_class();
	if (edac_class == NULL) {
		debugf1("%s() no edac_class error=%d\n", __func__, err);
=======
	struct bus_type *edac_subsys;

	debugf1("%s()\n", __func__);
=======
	edac_dbg(0, "\n");

#ifdef CONFIG_EDAC_DEBUG
	debugfs_remove(mci->debugfs);
#endif
#ifdef CONFIG_EDAC_LEGACY_SYSFS
	edac_delete_csrow_objects(mci);
#endif

	for (i = 0; i < mci->tot_dimms; i++) {
		struct dimm_info *dimm = mci->dimms[i];
		if (dimm->nr_pages == 0)
			continue;
		edac_dbg(0, "removing device %s\n", dev_name(&dimm->dev));
		device_unregister(&dimm->dev);
	}
}

void edac_unregister_sysfs(struct mem_ctl_info *mci)
{
	edac_dbg(1, "Unregistering device %s\n", dev_name(&mci->dev));
	device_unregister(&mci->dev);
	bus_unregister(mci->bus);
	kfree(mci->bus->name);
}

static void mc_attr_release(struct device *dev)
{
	/*
	 * There's no container structure here, as this is just the mci
	 * parent device, used to create the /sys/devices/mc sysfs node.
	 * So, there are no attributes on it.
	 */
	edac_dbg(1, "Releasing device %s\n", dev_name(dev));
	kfree(dev);
}

static struct device_type mc_attr_type = {
	.release	= mc_attr_release,
};
/*
 * Init/exit code for the module. Basically, creates/removes /sys/class/rc
 */
int __init edac_mc_sysfs_init(void)
{
	struct bus_type *edac_subsys;
	int err;
>>>>>>> refs/remotes/origin/master

	/* get the /sys/devices/system/edac subsys reference */
	edac_subsys = edac_get_sysfs_subsys();
	if (edac_subsys == NULL) {
<<<<<<< HEAD
		debugf1("%s() no edac_subsys error=%d\n", __func__, err);
>>>>>>> refs/remotes/origin/cm-10.0
		goto fail_out;
	}

	/* Init the MC's kobject */
<<<<<<< HEAD
	mc_kset = kset_create_and_add("mc", NULL, &edac_class->kset.kobj);
=======
	mc_kset = kset_create_and_add("mc", NULL, &edac_subsys->dev_root->kobj);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!mc_kset) {
		err = -ENOMEM;
		debugf1("%s() Failed to register '.../edac/mc'\n", __func__);
		goto fail_kset;
	}

	debugf1("%s() Registered '.../edac/mc' kobject\n", __func__);

	return 0;

fail_kset:
<<<<<<< HEAD
	edac_put_sysfs_class();
=======
	edac_put_sysfs_subsys();
>>>>>>> refs/remotes/origin/cm-10.0

fail_out:
	return err;
}

/*
 * edac_sysfs_teardown_mc_kset
 *
 *	deconstruct the mc_ket for memory controllers
 */
void edac_sysfs_teardown_mc_kset(void)
{
	kset_unregister(mc_kset);
<<<<<<< HEAD
	edac_put_sysfs_class();
=======
	edac_put_sysfs_subsys();
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
		edac_dbg(1, "no edac_subsys\n");
		err = -EINVAL;
		goto out;
	}

	mci_pdev = kzalloc(sizeof(*mci_pdev), GFP_KERNEL);
	if (!mci_pdev) {
		err = -ENOMEM;
		goto out_put_sysfs;
	}

	mci_pdev->bus = edac_subsys;
	mci_pdev->type = &mc_attr_type;
	device_initialize(mci_pdev);
	dev_set_name(mci_pdev, "mc");

	err = device_add(mci_pdev);
	if (err < 0)
		goto out_dev_free;

	edac_dbg(0, "device %s created\n", dev_name(mci_pdev));

	return 0;

 out_dev_free:
	kfree(mci_pdev);
 out_put_sysfs:
	edac_put_sysfs_subsys();
 out:
	return err;
}

void __exit edac_mc_sysfs_exit(void)
{
	device_unregister(mci_pdev);
	edac_put_sysfs_subsys();
}
>>>>>>> refs/remotes/origin/master
