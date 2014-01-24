/*
 * edac_mc kernel module
 * (C) 2005, 2006 Linux Networx (http://lnxi.com)
 * This file may be distributed under the terms of the
 * GNU General Public License.
 *
 * Written by Thayne Harbaugh
 * Based on work by Dan Hollis <goemon at anime dot net> and others.
 *	http://www.anime.net/~goemon/linux-ecc/
 *
 * Modified by Dave Peterson and Doug Thompson
 *
 */

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/smp.h>
#include <linux/init.h>
#include <linux/sysctl.h>
#include <linux/highmem.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>
#include <linux/list.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/sysdev.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/ctype.h>
#include <linux/edac.h>
=======
#include <linux/ctype.h>
#include <linux/edac.h>
#include <linux/bitops.h>
>>>>>>> refs/remotes/origin/master
#include <asm/uaccess.h>
#include <asm/page.h>
#include <asm/edac.h>
#include "edac_core.h"
#include "edac_module.h"

<<<<<<< HEAD
=======
#define CREATE_TRACE_POINTS
#define TRACE_INCLUDE_PATH ../../include/ras
#include <ras/ras_event.h>

>>>>>>> refs/remotes/origin/master
/* lock to memory controller's control array */
static DEFINE_MUTEX(mem_ctls_mutex);
static LIST_HEAD(mc_devices);

<<<<<<< HEAD
#ifdef CONFIG_EDAC_DEBUG

<<<<<<< HEAD
static void edac_mc_dump_channel(struct channel_info *chan)
=======
static void edac_mc_dump_channel(struct rank_info *chan)
>>>>>>> refs/remotes/origin/cm-10.0
{
	debugf4("\tchannel = %p\n", chan);
	debugf4("\tchannel->chan_idx = %d\n", chan->chan_idx);
	debugf4("\tchannel->ce_count = %d\n", chan->ce_count);
	debugf4("\tchannel->label = '%s'\n", chan->label);
	debugf4("\tchannel->csrow = %p\n\n", chan->csrow);
=======
/*
 * Used to lock EDAC MC to just one module, avoiding two drivers e. g.
 *	apei/ghes and i7core_edac to be used at the same time.
 */
static void const *edac_mc_owner;

static struct bus_type mc_bus[EDAC_MAX_MCS];

unsigned edac_dimm_info_location(struct dimm_info *dimm, char *buf,
			         unsigned len)
{
	struct mem_ctl_info *mci = dimm->mci;
	int i, n, count = 0;
	char *p = buf;

	for (i = 0; i < mci->n_layers; i++) {
		n = snprintf(p, len, "%s %d ",
			      edac_layer_name[mci->layers[i].type],
			      dimm->location[i]);
		p += n;
		len -= n;
		count += n;
		if (!len)
			break;
	}

	return count;
}

#ifdef CONFIG_EDAC_DEBUG

static void edac_mc_dump_channel(struct rank_info *chan)
{
	edac_dbg(4, "  channel->chan_idx = %d\n", chan->chan_idx);
	edac_dbg(4, "    channel = %p\n", chan);
	edac_dbg(4, "    channel->csrow = %p\n", chan->csrow);
	edac_dbg(4, "    channel->dimm = %p\n", chan->dimm);
}

static void edac_mc_dump_dimm(struct dimm_info *dimm, int number)
{
	char location[80];

	edac_dimm_info_location(dimm, location, sizeof(location));

	edac_dbg(4, "%s%i: %smapped as virtual row %d, chan %d\n",
		 dimm->mci->csbased ? "rank" : "dimm",
		 number, location, dimm->csrow, dimm->cschannel);
	edac_dbg(4, "  dimm = %p\n", dimm);
	edac_dbg(4, "  dimm->label = '%s'\n", dimm->label);
	edac_dbg(4, "  dimm->nr_pages = 0x%x\n", dimm->nr_pages);
	edac_dbg(4, "  dimm->grain = %d\n", dimm->grain);
	edac_dbg(4, "  dimm->nr_pages = 0x%x\n", dimm->nr_pages);
>>>>>>> refs/remotes/origin/master
}

static void edac_mc_dump_csrow(struct csrow_info *csrow)
{
<<<<<<< HEAD
	debugf4("\tcsrow = %p\n", csrow);
	debugf4("\tcsrow->csrow_idx = %d\n", csrow->csrow_idx);
	debugf4("\tcsrow->first_page = 0x%lx\n", csrow->first_page);
	debugf4("\tcsrow->last_page = 0x%lx\n", csrow->last_page);
	debugf4("\tcsrow->page_mask = 0x%lx\n", csrow->page_mask);
	debugf4("\tcsrow->nr_pages = 0x%x\n", csrow->nr_pages);
	debugf4("\tcsrow->nr_channels = %d\n", csrow->nr_channels);
	debugf4("\tcsrow->channels = %p\n", csrow->channels);
	debugf4("\tcsrow->mci = %p\n\n", csrow->mci);
=======
	edac_dbg(4, "csrow->csrow_idx = %d\n", csrow->csrow_idx);
	edac_dbg(4, "  csrow = %p\n", csrow);
	edac_dbg(4, "  csrow->first_page = 0x%lx\n", csrow->first_page);
	edac_dbg(4, "  csrow->last_page = 0x%lx\n", csrow->last_page);
	edac_dbg(4, "  csrow->page_mask = 0x%lx\n", csrow->page_mask);
	edac_dbg(4, "  csrow->nr_channels = %d\n", csrow->nr_channels);
	edac_dbg(4, "  csrow->channels = %p\n", csrow->channels);
	edac_dbg(4, "  csrow->mci = %p\n", csrow->mci);
>>>>>>> refs/remotes/origin/master
}

static void edac_mc_dump_mci(struct mem_ctl_info *mci)
{
<<<<<<< HEAD
	debugf3("\tmci = %p\n", mci);
	debugf3("\tmci->mtype_cap = %lx\n", mci->mtype_cap);
	debugf3("\tmci->edac_ctl_cap = %lx\n", mci->edac_ctl_cap);
	debugf3("\tmci->edac_cap = %lx\n", mci->edac_cap);
	debugf4("\tmci->edac_check = %p\n", mci->edac_check);
	debugf3("\tmci->nr_csrows = %d, csrows = %p\n",
		mci->nr_csrows, mci->csrows);
	debugf3("\tdev = %p\n", mci->dev);
	debugf3("\tmod_name:ctl_name = %s:%s\n", mci->mod_name, mci->ctl_name);
	debugf3("\tpvt_info = %p\n\n", mci->pvt_info);
=======
	edac_dbg(3, "\tmci = %p\n", mci);
	edac_dbg(3, "\tmci->mtype_cap = %lx\n", mci->mtype_cap);
	edac_dbg(3, "\tmci->edac_ctl_cap = %lx\n", mci->edac_ctl_cap);
	edac_dbg(3, "\tmci->edac_cap = %lx\n", mci->edac_cap);
	edac_dbg(4, "\tmci->edac_check = %p\n", mci->edac_check);
	edac_dbg(3, "\tmci->nr_csrows = %d, csrows = %p\n",
		 mci->nr_csrows, mci->csrows);
	edac_dbg(3, "\tmci->nr_dimms = %d, dimms = %p\n",
		 mci->tot_dimms, mci->dimms);
	edac_dbg(3, "\tdev = %p\n", mci->pdev);
	edac_dbg(3, "\tmod_name:ctl_name = %s:%s\n",
		 mci->mod_name, mci->ctl_name);
	edac_dbg(3, "\tpvt_info = %p\n\n", mci->pvt_info);
>>>>>>> refs/remotes/origin/master
}

#endif				/* CONFIG_EDAC_DEBUG */

/*
 * keep those in sync with the enum mem_type
 */
const char *edac_mem_types[] = {
	"Empty csrow",
	"Reserved csrow type",
	"Unknown csrow type",
	"Fast page mode RAM",
	"Extended data out RAM",
	"Burst Extended data out RAM",
	"Single data rate SDRAM",
	"Registered single data rate SDRAM",
	"Double data rate SDRAM",
	"Registered Double data rate SDRAM",
	"Rambus DRAM",
	"Unbuffered DDR2 RAM",
	"Fully buffered DDR2",
	"Registered DDR2 RAM",
	"Rambus XDR",
	"Unbuffered DDR3 RAM",
	"Registered DDR3 RAM",
};
EXPORT_SYMBOL_GPL(edac_mem_types);

<<<<<<< HEAD
/* 'ptr' points to a possibly unaligned item X such that sizeof(X) is 'size'.
 * Adjust 'ptr' so that its alignment is at least as stringent as what the
 * compiler would provide for X and return the aligned result.
 *
 * If 'size' is a constant, the compiler will optimize this whole function
 * down to either a no-op or the addition of a constant to the value of 'ptr'.
 */
void *edac_align_ptr(void *ptr, unsigned size)
{
	unsigned align, r;

	/* Here we assume that the alignment of a "long long" is the most
=======
/**
 * edac_align_ptr - Prepares the pointer offsets for a single-shot allocation
 * @p:		pointer to a pointer with the memory offset to be used. At
 *		return, this will be incremented to point to the next offset
 * @size:	Size of the data structure to be reserved
 * @n_elems:	Number of elements that should be reserved
 *
 * If 'size' is a constant, the compiler will optimize this whole function
 * down to either a no-op or the addition of a constant to the value of '*p'.
 *
 * The 'p' pointer is absolutely needed to keep the proper advancing
 * further in memory to the proper offsets when allocating the struct along
 * with its embedded structs, as edac_device_alloc_ctl_info() does it
 * above, for example.
 *
 * At return, the pointer 'p' will be incremented to be used on a next call
 * to this function.
 */
void *edac_align_ptr(void **p, unsigned size, int n_elems)
{
	unsigned align, r;
	void *ptr = *p;

	*p += size * n_elems;

	/*
	 * 'p' can possibly be an unaligned item X such that sizeof(X) is
	 * 'size'.  Adjust 'p' so that its alignment is at least as
	 * stringent as what the compiler would provide for X and return
	 * the aligned result.
	 * Here we assume that the alignment of a "long long" is the most
>>>>>>> refs/remotes/origin/master
	 * stringent alignment that the compiler will ever provide by default.
	 * As far as I know, this is a reasonable assumption.
	 */
	if (size > sizeof(long))
		align = sizeof(long long);
	else if (size > sizeof(int))
		align = sizeof(long);
	else if (size > sizeof(short))
		align = sizeof(int);
	else if (size > sizeof(char))
		align = sizeof(short);
	else
		return (char *)ptr;

<<<<<<< HEAD
	r = size % align;
=======
	r = (unsigned long)p % align;
>>>>>>> refs/remotes/origin/master

	if (r == 0)
		return (char *)ptr;

<<<<<<< HEAD
	return (void *)(((unsigned long)ptr) + align - r);
}

/**
 * edac_mc_alloc: Allocate a struct mem_ctl_info structure
 * @size_pvt:	size of private storage needed
 * @nr_csrows:	Number of CWROWS needed for this MC
 * @nr_chans:	Number of channels for the MC
=======
	*p += align - r;

	return (void *)(((unsigned long)ptr) + align - r);
}

static void _edac_mc_free(struct mem_ctl_info *mci)
{
	int i, chn, row;
	struct csrow_info *csr;
	const unsigned int tot_dimms = mci->tot_dimms;
	const unsigned int tot_channels = mci->num_cschannel;
	const unsigned int tot_csrows = mci->nr_csrows;

	if (mci->dimms) {
		for (i = 0; i < tot_dimms; i++)
			kfree(mci->dimms[i]);
		kfree(mci->dimms);
	}
	if (mci->csrows) {
		for (row = 0; row < tot_csrows; row++) {
			csr = mci->csrows[row];
			if (csr) {
				if (csr->channels) {
					for (chn = 0; chn < tot_channels; chn++)
						kfree(csr->channels[chn]);
					kfree(csr->channels);
				}
				kfree(csr);
			}
		}
		kfree(mci->csrows);
	}
	kfree(mci);
}

/**
 * edac_mc_alloc: Allocate and partially fill a struct mem_ctl_info structure
 * @mc_num:		Memory controller number
 * @n_layers:		Number of MC hierarchy layers
 * layers:		Describes each layer as seen by the Memory Controller
 * @size_pvt:		size of private storage needed
 *
>>>>>>> refs/remotes/origin/master
 *
 * Everything is kmalloc'ed as one big chunk - more efficient.
 * Only can be used if all structures have the same lifetime - otherwise
 * you have to allocate and initialize your own structures.
 *
 * Use edac_mc_free() to free mc structures allocated by this function.
 *
<<<<<<< HEAD
 * Returns:
 *	NULL allocation failed
 *	struct mem_ctl_info pointer
 */
struct mem_ctl_info *edac_mc_alloc(unsigned sz_pvt, unsigned nr_csrows,
				unsigned nr_chans, int edac_index)
{
	struct mem_ctl_info *mci;
	struct csrow_info *csi, *csrow;
<<<<<<< HEAD
	struct channel_info *chi, *chp, *chan;
=======
	struct rank_info *chi, *chp, *chan;
>>>>>>> refs/remotes/origin/cm-10.0
	void *pvt;
	unsigned size;
	int row, chn;
	int err;
=======
 * NOTE: drivers handle multi-rank memories in different ways: in some
 * drivers, one multi-rank memory stick is mapped as one entry, while, in
 * others, a single multi-rank memory stick would be mapped into several
 * entries. Currently, this function will allocate multiple struct dimm_info
 * on such scenarios, as grouping the multiple ranks require drivers change.
 *
 * Returns:
 *	On failure: NULL
 *	On success: struct mem_ctl_info pointer
 */
struct mem_ctl_info *edac_mc_alloc(unsigned mc_num,
				   unsigned n_layers,
				   struct edac_mc_layer *layers,
				   unsigned sz_pvt)
{
	struct mem_ctl_info *mci;
	struct edac_mc_layer *layer;
	struct csrow_info *csr;
	struct rank_info *chan;
	struct dimm_info *dimm;
	u32 *ce_per_layer[EDAC_MAX_LAYERS], *ue_per_layer[EDAC_MAX_LAYERS];
	unsigned pos[EDAC_MAX_LAYERS];
	unsigned size, tot_dimms = 1, count = 1;
	unsigned tot_csrows = 1, tot_channels = 1, tot_errcount = 0;
	void *pvt, *p, *ptr = NULL;
	int i, j, row, chn, n, len, off;
	bool per_rank = false;

	BUG_ON(n_layers > EDAC_MAX_LAYERS || n_layers == 0);
	/*
	 * Calculate the total amount of dimms and csrows/cschannels while
	 * in the old API emulation mode
	 */
	for (i = 0; i < n_layers; i++) {
		tot_dimms *= layers[i].size;
		if (layers[i].is_virt_csrow)
			tot_csrows *= layers[i].size;
		else
			tot_channels *= layers[i].size;

		if (layers[i].type == EDAC_MC_LAYER_CHIP_SELECT)
			per_rank = true;
	}
>>>>>>> refs/remotes/origin/master

	/* Figure out the offsets of the various items from the start of an mc
	 * structure.  We want the alignment of each item to be at least as
	 * stringent as what the compiler would provide if we could simply
	 * hardcode everything into a single struct.
	 */
<<<<<<< HEAD
	mci = (struct mem_ctl_info *)0;
	csi = edac_align_ptr(&mci[1], sizeof(*csi));
	chi = edac_align_ptr(&csi[nr_csrows], sizeof(*chi));
	pvt = edac_align_ptr(&chi[nr_chans * nr_csrows], sz_pvt);
	size = ((unsigned long)pvt) + sz_pvt;

=======
	mci = edac_align_ptr(&ptr, sizeof(*mci), 1);
	layer = edac_align_ptr(&ptr, sizeof(*layer), n_layers);
	for (i = 0; i < n_layers; i++) {
		count *= layers[i].size;
		edac_dbg(4, "errcount layer %d size %d\n", i, count);
		ce_per_layer[i] = edac_align_ptr(&ptr, sizeof(u32), count);
		ue_per_layer[i] = edac_align_ptr(&ptr, sizeof(u32), count);
		tot_errcount += 2 * count;
	}

	edac_dbg(4, "allocating %d error counters\n", tot_errcount);
	pvt = edac_align_ptr(&ptr, sz_pvt, 1);
	size = ((unsigned long)pvt) + sz_pvt;

	edac_dbg(1, "allocating %u bytes for mci data (%d %s, %d csrows/channels)\n",
		 size,
		 tot_dimms,
		 per_rank ? "ranks" : "dimms",
		 tot_csrows * tot_channels);

>>>>>>> refs/remotes/origin/master
	mci = kzalloc(size, GFP_KERNEL);
	if (mci == NULL)
		return NULL;

	/* Adjust pointers so they point within the memory we just allocated
	 * rather than an imaginary chunk of memory located at address 0.
	 */
<<<<<<< HEAD
	csi = (struct csrow_info *)(((char *)mci) + ((unsigned long)csi));
<<<<<<< HEAD
	chi = (struct channel_info *)(((char *)mci) + ((unsigned long)chi));
=======
	chi = (struct rank_info *)(((char *)mci) + ((unsigned long)chi));
>>>>>>> refs/remotes/origin/cm-10.0
	pvt = sz_pvt ? (((char *)mci) + ((unsigned long)pvt)) : NULL;

	/* setup index and various internal pointers */
	mci->mc_idx = edac_index;
	mci->csrows = csi;
	mci->pvt_info = pvt;
	mci->nr_csrows = nr_csrows;

	for (row = 0; row < nr_csrows; row++) {
		csrow = &csi[row];
		csrow->csrow_idx = row;
		csrow->mci = mci;
		csrow->nr_channels = nr_chans;
		chp = &chi[row * nr_chans];
		csrow->channels = chp;

		for (chn = 0; chn < nr_chans; chn++) {
			chan = &chp[chn];
			chan->chan_idx = chn;
			chan->csrow = csrow;
		}
	}

	mci->op_state = OP_ALLOC;
	INIT_LIST_HEAD(&mci->grp_kobj_list);

	/*
	 * Initialize the 'root' kobj for the edac_mc controller
	 */
	err = edac_mc_register_sysfs_main_kobj(mci);
	if (err) {
		kfree(mci);
		return NULL;
	}

	/* at this point, the root kobj is valid, and in order to
	 * 'free' the object, then the function:
	 *      edac_mc_unregister_sysfs_main_kobj() must be called
	 * which will perform kobj unregistration and the actual free
	 * will occur during the kobject callback operation
	 */
	return mci;
=======
	layer = (struct edac_mc_layer *)(((char *)mci) + ((unsigned long)layer));
	for (i = 0; i < n_layers; i++) {
		mci->ce_per_layer[i] = (u32 *)((char *)mci + ((unsigned long)ce_per_layer[i]));
		mci->ue_per_layer[i] = (u32 *)((char *)mci + ((unsigned long)ue_per_layer[i]));
	}
	pvt = sz_pvt ? (((char *)mci) + ((unsigned long)pvt)) : NULL;

	/* setup index and various internal pointers */
	mci->mc_idx = mc_num;
	mci->tot_dimms = tot_dimms;
	mci->pvt_info = pvt;
	mci->n_layers = n_layers;
	mci->layers = layer;
	memcpy(mci->layers, layers, sizeof(*layer) * n_layers);
	mci->nr_csrows = tot_csrows;
	mci->num_cschannel = tot_channels;
	mci->csbased = per_rank;

	/*
	 * Alocate and fill the csrow/channels structs
	 */
	mci->csrows = kcalloc(tot_csrows, sizeof(*mci->csrows), GFP_KERNEL);
	if (!mci->csrows)
		goto error;
	for (row = 0; row < tot_csrows; row++) {
		csr = kzalloc(sizeof(**mci->csrows), GFP_KERNEL);
		if (!csr)
			goto error;
		mci->csrows[row] = csr;
		csr->csrow_idx = row;
		csr->mci = mci;
		csr->nr_channels = tot_channels;
		csr->channels = kcalloc(tot_channels, sizeof(*csr->channels),
					GFP_KERNEL);
		if (!csr->channels)
			goto error;

		for (chn = 0; chn < tot_channels; chn++) {
			chan = kzalloc(sizeof(**csr->channels), GFP_KERNEL);
			if (!chan)
				goto error;
			csr->channels[chn] = chan;
			chan->chan_idx = chn;
			chan->csrow = csr;
		}
	}

	/*
	 * Allocate and fill the dimm structs
	 */
	mci->dimms  = kcalloc(tot_dimms, sizeof(*mci->dimms), GFP_KERNEL);
	if (!mci->dimms)
		goto error;

	memset(&pos, 0, sizeof(pos));
	row = 0;
	chn = 0;
	for (i = 0; i < tot_dimms; i++) {
		chan = mci->csrows[row]->channels[chn];
		off = EDAC_DIMM_OFF(layer, n_layers, pos[0], pos[1], pos[2]);
		if (off < 0 || off >= tot_dimms) {
			edac_mc_printk(mci, KERN_ERR, "EDAC core bug: EDAC_DIMM_OFF is trying to do an illegal data access\n");
			goto error;
		}

		dimm = kzalloc(sizeof(**mci->dimms), GFP_KERNEL);
		if (!dimm)
			goto error;
		mci->dimms[off] = dimm;
		dimm->mci = mci;

		/*
		 * Copy DIMM location and initialize it.
		 */
		len = sizeof(dimm->label);
		p = dimm->label;
		n = snprintf(p, len, "mc#%u", mc_num);
		p += n;
		len -= n;
		for (j = 0; j < n_layers; j++) {
			n = snprintf(p, len, "%s#%u",
				     edac_layer_name[layers[j].type],
				     pos[j]);
			p += n;
			len -= n;
			dimm->location[j] = pos[j];

			if (len <= 0)
				break;
		}

		/* Link it to the csrows old API data */
		chan->dimm = dimm;
		dimm->csrow = row;
		dimm->cschannel = chn;

		/* Increment csrow location */
		if (layers[0].is_virt_csrow) {
			chn++;
			if (chn == tot_channels) {
				chn = 0;
				row++;
			}
		} else {
			row++;
			if (row == tot_csrows) {
				row = 0;
				chn++;
			}
		}

		/* Increment dimm location */
		for (j = n_layers - 1; j >= 0; j--) {
			pos[j]++;
			if (pos[j] < layers[j].size)
				break;
			pos[j] = 0;
		}
	}

	mci->op_state = OP_ALLOC;

	return mci;

error:
	_edac_mc_free(mci);

	return NULL;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(edac_mc_alloc);

/**
 * edac_mc_free
 *	'Free' a previously allocated 'mci' structure
 * @mci: pointer to a struct mem_ctl_info structure
 */
void edac_mc_free(struct mem_ctl_info *mci)
{
<<<<<<< HEAD
	debugf1("%s()\n", __func__);

	edac_mc_unregister_sysfs_main_kobj(mci);

	/* free the mci instance memory here */
	kfree(mci);
=======
	edac_dbg(1, "\n");

	/* If we're not yet registered with sysfs free only what was allocated
	 * in edac_mc_alloc().
	 */
	if (!device_is_registered(&mci->dev)) {
		_edac_mc_free(mci);
		return;
	}

	/* the mci instance is freed here, when the sysfs object is dropped */
	edac_unregister_sysfs(mci);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(edac_mc_free);


/**
 * find_mci_by_dev
 *
 *	scan list of controllers looking for the one that manages
 *	the 'dev' device
 * @dev: pointer to a struct device related with the MCI
 */
struct mem_ctl_info *find_mci_by_dev(struct device *dev)
{
	struct mem_ctl_info *mci;
	struct list_head *item;

<<<<<<< HEAD
	debugf3("%s()\n", __func__);
=======
	edac_dbg(3, "\n");
>>>>>>> refs/remotes/origin/master

	list_for_each(item, &mc_devices) {
		mci = list_entry(item, struct mem_ctl_info, link);

<<<<<<< HEAD
		if (mci->dev == dev)
=======
		if (mci->pdev == dev)
>>>>>>> refs/remotes/origin/master
			return mci;
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(find_mci_by_dev);

/*
 * handler for EDAC to check if NMI type handler has asserted interrupt
 */
static int edac_mc_assert_error_check_and_clear(void)
{
	int old_state;

	if (edac_op_state == EDAC_OPSTATE_POLL)
		return 1;

	old_state = edac_err_assert;
	edac_err_assert = 0;

	return old_state;
}

/*
 * edac_mc_workq_function
 *	performs the operation scheduled by a workq request
 */
static void edac_mc_workq_function(struct work_struct *work_req)
{
	struct delayed_work *d_work = to_delayed_work(work_req);
	struct mem_ctl_info *mci = to_edac_mem_ctl_work(d_work);

	mutex_lock(&mem_ctls_mutex);

	/* if this control struct has movd to offline state, we are done */
	if (mci->op_state == OP_OFFLINE) {
		mutex_unlock(&mem_ctls_mutex);
		return;
	}

	/* Only poll controllers that are running polled and have a check */
	if (edac_mc_assert_error_check_and_clear() && (mci->edac_check != NULL))
		mci->edac_check(mci);

	mutex_unlock(&mem_ctls_mutex);

	/* Reschedule */
	queue_delayed_work(edac_workqueue, &mci->work,
			msecs_to_jiffies(edac_mc_get_poll_msec()));
}

/*
 * edac_mc_workq_setup
 *	initialize a workq item for this mci
 *	passing in the new delay period in msec
 *
 *	locking model:
 *
 *		called with the mem_ctls_mutex held
 */
static void edac_mc_workq_setup(struct mem_ctl_info *mci, unsigned msec)
{
<<<<<<< HEAD
	debugf0("%s()\n", __func__);
=======
	edac_dbg(0, "\n");
>>>>>>> refs/remotes/origin/master

	/* if this instance is not in the POLL state, then simply return */
	if (mci->op_state != OP_RUNNING_POLL)
		return;

	INIT_DELAYED_WORK(&mci->work, edac_mc_workq_function);
<<<<<<< HEAD
	queue_delayed_work(edac_workqueue, &mci->work, msecs_to_jiffies(msec));
=======
	mod_delayed_work(edac_workqueue, &mci->work, msecs_to_jiffies(msec));
>>>>>>> refs/remotes/origin/master
}

/*
 * edac_mc_workq_teardown
 *	stop the workq processing on this mci
 *
 *	locking model:
 *
 *		called WITHOUT lock held
 */
static void edac_mc_workq_teardown(struct mem_ctl_info *mci)
{
	int status;

	if (mci->op_state != OP_RUNNING_POLL)
		return;

	status = cancel_delayed_work(&mci->work);
	if (status == 0) {
<<<<<<< HEAD
		debugf0("%s() not canceled, flush the queue\n",
			__func__);
=======
		edac_dbg(0, "not canceled, flush the queue\n");
>>>>>>> refs/remotes/origin/master

		/* workq instance might be running, wait for it */
		flush_workqueue(edac_workqueue);
	}
}

/*
 * edac_mc_reset_delay_period(unsigned long value)
 *
 *	user space has updated our poll period value, need to
 *	reset our workq delays
 */
void edac_mc_reset_delay_period(int value)
{
	struct mem_ctl_info *mci;
	struct list_head *item;

	mutex_lock(&mem_ctls_mutex);

<<<<<<< HEAD
	/* scan the list and turn off all workq timers, doing so under lock
	 */
	list_for_each(item, &mc_devices) {
		mci = list_entry(item, struct mem_ctl_info, link);

		if (mci->op_state == OP_RUNNING_POLL)
			cancel_delayed_work(&mci->work);
	}

	mutex_unlock(&mem_ctls_mutex);


	/* re-walk the list, and reset the poll delay */
	mutex_lock(&mem_ctls_mutex);

=======
>>>>>>> refs/remotes/origin/master
	list_for_each(item, &mc_devices) {
		mci = list_entry(item, struct mem_ctl_info, link);

		edac_mc_workq_setup(mci, (unsigned long) value);
	}

	mutex_unlock(&mem_ctls_mutex);
}



/* Return 0 on success, 1 on failure.
 * Before calling this function, caller must
 * assign a unique value to mci->mc_idx.
 *
 *	locking model:
 *
 *		called with the mem_ctls_mutex lock held
 */
static int add_mc_to_global_list(struct mem_ctl_info *mci)
{
	struct list_head *item, *insert_before;
	struct mem_ctl_info *p;

	insert_before = &mc_devices;

<<<<<<< HEAD
	p = find_mci_by_dev(mci->dev);
=======
	p = find_mci_by_dev(mci->pdev);
>>>>>>> refs/remotes/origin/master
	if (unlikely(p != NULL))
		goto fail0;

	list_for_each(item, &mc_devices) {
		p = list_entry(item, struct mem_ctl_info, link);

		if (p->mc_idx >= mci->mc_idx) {
			if (unlikely(p->mc_idx == mci->mc_idx))
				goto fail1;

			insert_before = item;
			break;
		}
	}

	list_add_tail_rcu(&mci->link, insert_before);
	atomic_inc(&edac_handlers);
	return 0;

fail0:
	edac_printk(KERN_WARNING, EDAC_MC,
<<<<<<< HEAD
		"%s (%s) %s %s already assigned %d\n", dev_name(p->dev),
=======
		"%s (%s) %s %s already assigned %d\n", dev_name(p->pdev),
>>>>>>> refs/remotes/origin/master
		edac_dev_name(mci), p->mod_name, p->ctl_name, p->mc_idx);
	return 1;

fail1:
	edac_printk(KERN_WARNING, EDAC_MC,
		"bug in low-level driver: attempt to assign\n"
		"    duplicate mc_idx %d in %s()\n", p->mc_idx, __func__);
	return 1;
}

<<<<<<< HEAD
static void del_mc_from_global_list(struct mem_ctl_info *mci)
{
	atomic_dec(&edac_handlers);
=======
static int del_mc_from_global_list(struct mem_ctl_info *mci)
{
	int handlers = atomic_dec_return(&edac_handlers);
>>>>>>> refs/remotes/origin/master
	list_del_rcu(&mci->link);

	/* these are for safe removal of devices from global list while
	 * NMI handlers may be traversing list
	 */
	synchronize_rcu();
	INIT_LIST_HEAD(&mci->link);
<<<<<<< HEAD
=======

	return handlers;
>>>>>>> refs/remotes/origin/master
}

/**
 * edac_mc_find: Search for a mem_ctl_info structure whose index is 'idx'.
 *
 * If found, return a pointer to the structure.
 * Else return NULL.
 *
 * Caller must hold mem_ctls_mutex.
 */
struct mem_ctl_info *edac_mc_find(int idx)
{
	struct list_head *item;
	struct mem_ctl_info *mci;

	list_for_each(item, &mc_devices) {
		mci = list_entry(item, struct mem_ctl_info, link);

		if (mci->mc_idx >= idx) {
			if (mci->mc_idx == idx)
				return mci;

			break;
		}
	}

	return NULL;
}
EXPORT_SYMBOL(edac_mc_find);

/**
 * edac_mc_add_mc: Insert the 'mci' structure into the mci global list and
 *                 create sysfs entries associated with mci structure
 * @mci: pointer to the mci structure to be added to the list
<<<<<<< HEAD
 * @mc_idx: A unique numeric identifier to be assigned to the 'mci' structure.
=======
>>>>>>> refs/remotes/origin/master
 *
 * Return:
 *	0	Success
 *	!0	Failure
 */

/* FIXME - should a warning be printed if no error detection? correction? */
int edac_mc_add_mc(struct mem_ctl_info *mci)
{
<<<<<<< HEAD
	debugf0("%s()\n", __func__);
=======
	int ret = -EINVAL;
	edac_dbg(0, "\n");

	if (mci->mc_idx >= EDAC_MAX_MCS) {
		pr_warn_once("Too many memory controllers: %d\n", mci->mc_idx);
		return -ENODEV;
	}
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_EDAC_DEBUG
	if (edac_debug_level >= 3)
		edac_mc_dump_mci(mci);

	if (edac_debug_level >= 4) {
		int i;

		for (i = 0; i < mci->nr_csrows; i++) {
<<<<<<< HEAD
			int j;

			edac_mc_dump_csrow(&mci->csrows[i]);
			for (j = 0; j < mci->csrows[i].nr_channels; j++)
				edac_mc_dump_channel(&mci->csrows[i].
						channels[j]);
		}
=======
			struct csrow_info *csrow = mci->csrows[i];
			u32 nr_pages = 0;
			int j;

			for (j = 0; j < csrow->nr_channels; j++)
				nr_pages += csrow->channels[j]->dimm->nr_pages;
			if (!nr_pages)
				continue;
			edac_mc_dump_csrow(csrow);
			for (j = 0; j < csrow->nr_channels; j++)
				if (csrow->channels[j]->dimm->nr_pages)
					edac_mc_dump_channel(csrow->channels[j]);
		}
		for (i = 0; i < mci->tot_dimms; i++)
			if (mci->dimms[i]->nr_pages)
				edac_mc_dump_dimm(mci->dimms[i], i);
>>>>>>> refs/remotes/origin/master
	}
#endif
	mutex_lock(&mem_ctls_mutex);

<<<<<<< HEAD
=======
	if (edac_mc_owner && edac_mc_owner != mci->mod_name) {
		ret = -EPERM;
		goto fail0;
	}

>>>>>>> refs/remotes/origin/master
	if (add_mc_to_global_list(mci))
		goto fail0;

	/* set load time so that error rate can be tracked */
	mci->start_time = jiffies;

<<<<<<< HEAD
=======
	mci->bus = &mc_bus[mci->mc_idx];

>>>>>>> refs/remotes/origin/master
	if (edac_create_sysfs_mci_device(mci)) {
		edac_mc_printk(mci, KERN_WARNING,
			"failed to create sysfs device\n");
		goto fail1;
	}

	/* If there IS a check routine, then we are running POLLED */
	if (mci->edac_check != NULL) {
		/* This instance is NOW RUNNING */
		mci->op_state = OP_RUNNING_POLL;

		edac_mc_workq_setup(mci, edac_mc_get_poll_msec());
	} else {
		mci->op_state = OP_RUNNING_INTERRUPT;
	}

	/* Report action taken */
<<<<<<< HEAD
	edac_mc_printk(mci, KERN_INFO, "Giving out device to '%s' '%s':"
		" DEV %s\n", mci->mod_name, mci->ctl_name, edac_dev_name(mci));
=======
	edac_mc_printk(mci, KERN_INFO,
		"Giving out device to module %s controller %s: DEV %s (%s)\n",
		mci->mod_name, mci->ctl_name, mci->dev_name,
		edac_op_state_to_string(mci->op_state));

	edac_mc_owner = mci->mod_name;
>>>>>>> refs/remotes/origin/master

	mutex_unlock(&mem_ctls_mutex);
	return 0;

fail1:
	del_mc_from_global_list(mci);

fail0:
	mutex_unlock(&mem_ctls_mutex);
<<<<<<< HEAD
	return 1;
=======
	return ret;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(edac_mc_add_mc);

/**
 * edac_mc_del_mc: Remove sysfs entries for specified mci structure and
 *                 remove mci structure from global list
 * @pdev: Pointer to 'struct device' representing mci structure to remove.
 *
 * Return pointer to removed mci structure, or NULL if device not found.
 */
struct mem_ctl_info *edac_mc_del_mc(struct device *dev)
{
	struct mem_ctl_info *mci;

<<<<<<< HEAD
	debugf0("%s()\n", __func__);
=======
	edac_dbg(0, "\n");
>>>>>>> refs/remotes/origin/master

	mutex_lock(&mem_ctls_mutex);

	/* find the requested mci struct in the global list */
	mci = find_mci_by_dev(dev);
	if (mci == NULL) {
		mutex_unlock(&mem_ctls_mutex);
		return NULL;
	}

<<<<<<< HEAD
	del_mc_from_global_list(mci);
=======
	if (!del_mc_from_global_list(mci))
		edac_mc_owner = NULL;
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&mem_ctls_mutex);

	/* flush workq processes */
	edac_mc_workq_teardown(mci);

	/* marking MCI offline */
	mci->op_state = OP_OFFLINE;

	/* remove from sysfs */
	edac_remove_sysfs_mci_device(mci);

	edac_printk(KERN_INFO, EDAC_MC,
		"Removed device %d for %s %s: DEV %s\n", mci->mc_idx,
		mci->mod_name, mci->ctl_name, edac_dev_name(mci));

	return mci;
}
EXPORT_SYMBOL_GPL(edac_mc_del_mc);

static void edac_mc_scrub_block(unsigned long page, unsigned long offset,
				u32 size)
{
	struct page *pg;
	void *virt_addr;
	unsigned long flags = 0;

<<<<<<< HEAD
	debugf3("%s()\n", __func__);
=======
	edac_dbg(3, "\n");
>>>>>>> refs/remotes/origin/master

	/* ECC error page was not in our memory. Ignore it. */
	if (!pfn_valid(page))
		return;

	/* Find the actual page structure then map it and fix */
	pg = pfn_to_page(page);

	if (PageHighMem(pg))
		local_irq_save(flags);

<<<<<<< HEAD
<<<<<<< HEAD
	virt_addr = kmap_atomic(pg, KM_BOUNCE_READ);
=======
	virt_addr = kmap_atomic(pg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	virt_addr = kmap_atomic(pg);
>>>>>>> refs/remotes/origin/master

	/* Perform architecture specific atomic scrub operation */
	atomic_scrub(virt_addr + offset, size);

	/* Unmap and complete */
<<<<<<< HEAD
<<<<<<< HEAD
	kunmap_atomic(virt_addr, KM_BOUNCE_READ);
=======
	kunmap_atomic(virt_addr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kunmap_atomic(virt_addr);
>>>>>>> refs/remotes/origin/master

	if (PageHighMem(pg))
		local_irq_restore(flags);
}

/* FIXME - should return -1 */
int edac_mc_find_csrow_by_page(struct mem_ctl_info *mci, unsigned long page)
{
<<<<<<< HEAD
	struct csrow_info *csrows = mci->csrows;
	int row, i;

	debugf1("MC%d: %s(): 0x%lx\n", mci->mc_idx, __func__, page);
	row = -1;

	for (i = 0; i < mci->nr_csrows; i++) {
		struct csrow_info *csrow = &csrows[i];

		if (csrow->nr_pages == 0)
			continue;

		debugf3("MC%d: %s(): first(0x%lx) page(0x%lx) last(0x%lx) "
			"mask(0x%lx)\n", mci->mc_idx, __func__,
			csrow->first_page, page, csrow->last_page,
			csrow->page_mask);
=======
	struct csrow_info **csrows = mci->csrows;
	int row, i, j, n;

	edac_dbg(1, "MC%d: 0x%lx\n", mci->mc_idx, page);
	row = -1;

	for (i = 0; i < mci->nr_csrows; i++) {
		struct csrow_info *csrow = csrows[i];
		n = 0;
		for (j = 0; j < csrow->nr_channels; j++) {
			struct dimm_info *dimm = csrow->channels[j]->dimm;
			n += dimm->nr_pages;
		}
		if (n == 0)
			continue;

		edac_dbg(3, "MC%d: first(0x%lx) page(0x%lx) last(0x%lx) mask(0x%lx)\n",
			 mci->mc_idx,
			 csrow->first_page, page, csrow->last_page,
			 csrow->page_mask);
>>>>>>> refs/remotes/origin/master

		if ((page >= csrow->first_page) &&
		    (page <= csrow->last_page) &&
		    ((page & csrow->page_mask) ==
		     (csrow->first_page & csrow->page_mask))) {
			row = i;
			break;
		}
	}

	if (row == -1)
		edac_mc_printk(mci, KERN_ERR,
			"could not look up page error address %lx\n",
			(unsigned long)page);

	return row;
}
EXPORT_SYMBOL_GPL(edac_mc_find_csrow_by_page);

<<<<<<< HEAD
/* FIXME - setable log (warning/emerg) levels */
/* FIXME - integrate with evlog: http://evlog.sourceforge.net/ */
void edac_mc_handle_ce(struct mem_ctl_info *mci,
		unsigned long page_frame_number,
		unsigned long offset_in_page, unsigned long syndrome,
		int row, int channel, const char *msg)
{
	unsigned long remapped_page;

	debugf3("MC%d: %s()\n", mci->mc_idx, __func__);

	/* FIXME - maybe make panic on INTERNAL ERROR an option */
	if (row >= mci->nr_csrows || row < 0) {
		/* something is wrong */
		edac_mc_printk(mci, KERN_ERR,
			"INTERNAL ERROR: row out of range "
			"(%d >= %d)\n", row, mci->nr_csrows);
		edac_mc_handle_ce_no_info(mci, "INTERNAL ERROR");
		return;
	}

	if (channel >= mci->csrows[row].nr_channels || channel < 0) {
		/* something is wrong */
		edac_mc_printk(mci, KERN_ERR,
			"INTERNAL ERROR: channel out of range "
			"(%d >= %d)\n", channel,
			mci->csrows[row].nr_channels);
		edac_mc_handle_ce_no_info(mci, "INTERNAL ERROR");
		return;
	}

	if (edac_mc_get_log_ce())
		/* FIXME - put in DIMM location */
		edac_mc_printk(mci, KERN_WARNING,
			"CE page 0x%lx, offset 0x%lx, grain %d, syndrome "
			"0x%lx, row %d, channel %d, label \"%s\": %s\n",
			page_frame_number, offset_in_page,
			mci->csrows[row].grain, syndrome, row, channel,
			mci->csrows[row].channels[channel].label, msg);

	mci->ce_count++;
	mci->csrows[row].ce_count++;
	mci->csrows[row].channels[channel].ce_count++;

	if (mci->scrub_mode & SCRUB_SW_SRC) {
		/*
		 * Some MC's can remap memory so that it is still available
		 * at a different address when PCI devices map into memory.
		 * MC's that can't do this lose the memory where PCI devices
		 * are mapped.  This mapping is MC dependent and so we call
		 * back into the MC driver for it to map the MC page to
		 * a physical (CPU) page which can then be mapped to a virtual
		 * page - which can then be scrubbed.
		 */
=======
const char *edac_layer_name[] = {
	[EDAC_MC_LAYER_BRANCH] = "branch",
	[EDAC_MC_LAYER_CHANNEL] = "channel",
	[EDAC_MC_LAYER_SLOT] = "slot",
	[EDAC_MC_LAYER_CHIP_SELECT] = "csrow",
	[EDAC_MC_LAYER_ALL_MEM] = "memory",
};
EXPORT_SYMBOL_GPL(edac_layer_name);

static void edac_inc_ce_error(struct mem_ctl_info *mci,
			      bool enable_per_layer_report,
			      const int pos[EDAC_MAX_LAYERS],
			      const u16 count)
{
	int i, index = 0;

	mci->ce_mc += count;

	if (!enable_per_layer_report) {
		mci->ce_noinfo_count += count;
		return;
	}

	for (i = 0; i < mci->n_layers; i++) {
		if (pos[i] < 0)
			break;
		index += pos[i];
		mci->ce_per_layer[i][index] += count;

		if (i < mci->n_layers - 1)
			index *= mci->layers[i + 1].size;
	}
}

static void edac_inc_ue_error(struct mem_ctl_info *mci,
				    bool enable_per_layer_report,
				    const int pos[EDAC_MAX_LAYERS],
				    const u16 count)
{
	int i, index = 0;

	mci->ue_mc += count;

	if (!enable_per_layer_report) {
		mci->ce_noinfo_count += count;
		return;
	}

	for (i = 0; i < mci->n_layers; i++) {
		if (pos[i] < 0)
			break;
		index += pos[i];
		mci->ue_per_layer[i][index] += count;

		if (i < mci->n_layers - 1)
			index *= mci->layers[i + 1].size;
	}
}

static void edac_ce_error(struct mem_ctl_info *mci,
			  const u16 error_count,
			  const int pos[EDAC_MAX_LAYERS],
			  const char *msg,
			  const char *location,
			  const char *label,
			  const char *detail,
			  const char *other_detail,
			  const bool enable_per_layer_report,
			  const unsigned long page_frame_number,
			  const unsigned long offset_in_page,
			  long grain)
{
	unsigned long remapped_page;
	char *msg_aux = "";

	if (*msg)
		msg_aux = " ";

	if (edac_mc_get_log_ce()) {
		if (other_detail && *other_detail)
			edac_mc_printk(mci, KERN_WARNING,
				       "%d CE %s%son %s (%s %s - %s)\n",
				       error_count, msg, msg_aux, label,
				       location, detail, other_detail);
		else
			edac_mc_printk(mci, KERN_WARNING,
				       "%d CE %s%son %s (%s %s)\n",
				       error_count, msg, msg_aux, label,
				       location, detail);
	}
	edac_inc_ce_error(mci, enable_per_layer_report, pos, error_count);

	if (mci->scrub_mode & SCRUB_SW_SRC) {
		/*
			* Some memory controllers (called MCs below) can remap
			* memory so that it is still available at a different
			* address when PCI devices map into memory.
			* MC's that can't do this, lose the memory where PCI
			* devices are mapped. This mapping is MC-dependent
			* and so we call back into the MC driver for it to
			* map the MC page to a physical (CPU) page which can
			* then be mapped to a virtual page - which can then
			* be scrubbed.
			*/
>>>>>>> refs/remotes/origin/master
		remapped_page = mci->ctl_page_to_phys ?
			mci->ctl_page_to_phys(mci, page_frame_number) :
			page_frame_number;

<<<<<<< HEAD
		edac_mc_scrub_block(remapped_page, offset_in_page,
				mci->csrows[row].grain);
	}
}
EXPORT_SYMBOL_GPL(edac_mc_handle_ce);

void edac_mc_handle_ce_no_info(struct mem_ctl_info *mci, const char *msg)
{
	if (edac_mc_get_log_ce())
		edac_mc_printk(mci, KERN_WARNING,
			"CE - no information available: %s\n", msg);

	mci->ce_noinfo_count++;
	mci->ce_count++;
}
EXPORT_SYMBOL_GPL(edac_mc_handle_ce_no_info);

void edac_mc_handle_ue(struct mem_ctl_info *mci,
		unsigned long page_frame_number,
		unsigned long offset_in_page, int row, const char *msg)
{
	int len = EDAC_MC_LABEL_LEN * 4;
	char labels[len + 1];
	char *pos = labels;
	int chan;
	int chars;

	debugf3("MC%d: %s()\n", mci->mc_idx, __func__);

	/* FIXME - maybe make panic on INTERNAL ERROR an option */
	if (row >= mci->nr_csrows || row < 0) {
		/* something is wrong */
		edac_mc_printk(mci, KERN_ERR,
			"INTERNAL ERROR: row out of range "
			"(%d >= %d)\n", row, mci->nr_csrows);
		edac_mc_handle_ue_no_info(mci, "INTERNAL ERROR");
		return;
	}

	chars = snprintf(pos, len + 1, "%s",
			 mci->csrows[row].channels[0].label);
	len -= chars;
	pos += chars;

	for (chan = 1; (chan < mci->csrows[row].nr_channels) && (len > 0);
		chan++) {
		chars = snprintf(pos, len + 1, ":%s",
				 mci->csrows[row].channels[chan].label);
		len -= chars;
		pos += chars;
	}

	if (edac_mc_get_log_ue())
		edac_mc_printk(mci, KERN_EMERG,
			"UE page 0x%lx, offset 0x%lx, grain %d, row %d, "
			"labels \"%s\": %s\n", page_frame_number,
			offset_in_page, mci->csrows[row].grain, row,
			labels, msg);

	if (edac_mc_get_panic_on_ue())
		panic("EDAC MC%d: UE page 0x%lx, offset 0x%lx, grain %d, "
			"row %d, labels \"%s\": %s\n", mci->mc_idx,
			page_frame_number, offset_in_page,
			mci->csrows[row].grain, row, labels, msg);

	mci->ue_count++;
	mci->csrows[row].ue_count++;
}
EXPORT_SYMBOL_GPL(edac_mc_handle_ue);

void edac_mc_handle_ue_no_info(struct mem_ctl_info *mci, const char *msg)
{
	if (edac_mc_get_panic_on_ue())
		panic("EDAC MC%d: Uncorrected Error", mci->mc_idx);

	if (edac_mc_get_log_ue())
		edac_mc_printk(mci, KERN_WARNING,
			"UE - no information available: %s\n", msg);
	mci->ue_noinfo_count++;
	mci->ue_count++;
}
EXPORT_SYMBOL_GPL(edac_mc_handle_ue_no_info);

/*************************************************************
 * On Fully Buffered DIMM modules, this help function is
 * called to process UE events
 */
void edac_mc_handle_fbd_ue(struct mem_ctl_info *mci,
			unsigned int csrow,
			unsigned int channela,
			unsigned int channelb, char *msg)
{
	int len = EDAC_MC_LABEL_LEN * 4;
	char labels[len + 1];
	char *pos = labels;
	int chars;

	if (csrow >= mci->nr_csrows) {
		/* something is wrong */
		edac_mc_printk(mci, KERN_ERR,
			"INTERNAL ERROR: row out of range (%d >= %d)\n",
			csrow, mci->nr_csrows);
		edac_mc_handle_ue_no_info(mci, "INTERNAL ERROR");
		return;
	}

	if (channela >= mci->csrows[csrow].nr_channels) {
		/* something is wrong */
		edac_mc_printk(mci, KERN_ERR,
			"INTERNAL ERROR: channel-a out of range "
			"(%d >= %d)\n",
			channela, mci->csrows[csrow].nr_channels);
		edac_mc_handle_ue_no_info(mci, "INTERNAL ERROR");
		return;
	}

	if (channelb >= mci->csrows[csrow].nr_channels) {
		/* something is wrong */
		edac_mc_printk(mci, KERN_ERR,
			"INTERNAL ERROR: channel-b out of range "
			"(%d >= %d)\n",
			channelb, mci->csrows[csrow].nr_channels);
		edac_mc_handle_ue_no_info(mci, "INTERNAL ERROR");
		return;
	}

	mci->ue_count++;
	mci->csrows[csrow].ue_count++;

	/* Generate the DIMM labels from the specified channels */
	chars = snprintf(pos, len + 1, "%s",
			 mci->csrows[csrow].channels[channela].label);
	len -= chars;
	pos += chars;
	chars = snprintf(pos, len + 1, "-%s",
			 mci->csrows[csrow].channels[channelb].label);

	if (edac_mc_get_log_ue())
		edac_mc_printk(mci, KERN_EMERG,
			"UE row %d, channel-a= %d channel-b= %d "
			"labels \"%s\": %s\n", csrow, channela, channelb,
			labels, msg);

	if (edac_mc_get_panic_on_ue())
		panic("UE row %d, channel-a= %d channel-b= %d "
			"labels \"%s\": %s\n", csrow, channela,
			channelb, labels, msg);
}
EXPORT_SYMBOL(edac_mc_handle_fbd_ue);

/*************************************************************
 * On Fully Buffered DIMM modules, this help function is
 * called to process CE events
 */
void edac_mc_handle_fbd_ce(struct mem_ctl_info *mci,
			unsigned int csrow, unsigned int channel, char *msg)
{

	/* Ensure boundary values */
	if (csrow >= mci->nr_csrows) {
		/* something is wrong */
		edac_mc_printk(mci, KERN_ERR,
			"INTERNAL ERROR: row out of range (%d >= %d)\n",
			csrow, mci->nr_csrows);
		edac_mc_handle_ce_no_info(mci, "INTERNAL ERROR");
		return;
	}
	if (channel >= mci->csrows[csrow].nr_channels) {
		/* something is wrong */
		edac_mc_printk(mci, KERN_ERR,
			"INTERNAL ERROR: channel out of range (%d >= %d)\n",
			channel, mci->csrows[csrow].nr_channels);
		edac_mc_handle_ce_no_info(mci, "INTERNAL ERROR");
		return;
	}

	if (edac_mc_get_log_ce())
		/* FIXME - put in DIMM location */
		edac_mc_printk(mci, KERN_WARNING,
			"CE row %d, channel %d, label \"%s\": %s\n",
			csrow, channel,
			mci->csrows[csrow].channels[channel].label, msg);

	mci->ce_count++;
	mci->csrows[csrow].ce_count++;
	mci->csrows[csrow].channels[channel].ce_count++;
}
EXPORT_SYMBOL(edac_mc_handle_fbd_ce);
=======
		edac_mc_scrub_block(remapped_page,
					offset_in_page, grain);
	}
}

static void edac_ue_error(struct mem_ctl_info *mci,
			  const u16 error_count,
			  const int pos[EDAC_MAX_LAYERS],
			  const char *msg,
			  const char *location,
			  const char *label,
			  const char *detail,
			  const char *other_detail,
			  const bool enable_per_layer_report)
{
	char *msg_aux = "";

	if (*msg)
		msg_aux = " ";

	if (edac_mc_get_log_ue()) {
		if (other_detail && *other_detail)
			edac_mc_printk(mci, KERN_WARNING,
				       "%d UE %s%son %s (%s %s - %s)\n",
				       error_count, msg, msg_aux, label,
				       location, detail, other_detail);
		else
			edac_mc_printk(mci, KERN_WARNING,
				       "%d UE %s%son %s (%s %s)\n",
				       error_count, msg, msg_aux, label,
				       location, detail);
	}

	if (edac_mc_get_panic_on_ue()) {
		if (other_detail && *other_detail)
			panic("UE %s%son %s (%s%s - %s)\n",
			      msg, msg_aux, label, location, detail, other_detail);
		else
			panic("UE %s%son %s (%s%s)\n",
			      msg, msg_aux, label, location, detail);
	}

	edac_inc_ue_error(mci, enable_per_layer_report, pos, error_count);
}

/**
 * edac_raw_mc_handle_error - reports a memory event to userspace without doing
 *			      anything to discover the error location
 *
 * @type:		severity of the error (CE/UE/Fatal)
 * @mci:		a struct mem_ctl_info pointer
 * @e:			error description
 *
 * This raw function is used internally by edac_mc_handle_error(). It should
 * only be called directly when the hardware error come directly from BIOS,
 * like in the case of APEI GHES driver.
 */
void edac_raw_mc_handle_error(const enum hw_event_mc_err_type type,
			      struct mem_ctl_info *mci,
			      struct edac_raw_error_desc *e)
{
	char detail[80];
	int pos[EDAC_MAX_LAYERS] = { e->top_layer, e->mid_layer, e->low_layer };

	/* Memory type dependent details about the error */
	if (type == HW_EVENT_ERR_CORRECTED) {
		snprintf(detail, sizeof(detail),
			"page:0x%lx offset:0x%lx grain:%ld syndrome:0x%lx",
			e->page_frame_number, e->offset_in_page,
			e->grain, e->syndrome);
		edac_ce_error(mci, e->error_count, pos, e->msg, e->location, e->label,
			      detail, e->other_detail, e->enable_per_layer_report,
			      e->page_frame_number, e->offset_in_page, e->grain);
	} else {
		snprintf(detail, sizeof(detail),
			"page:0x%lx offset:0x%lx grain:%ld",
			e->page_frame_number, e->offset_in_page, e->grain);

		edac_ue_error(mci, e->error_count, pos, e->msg, e->location, e->label,
			      detail, e->other_detail, e->enable_per_layer_report);
	}


}
EXPORT_SYMBOL_GPL(edac_raw_mc_handle_error);

/**
 * edac_mc_handle_error - reports a memory event to userspace
 *
 * @type:		severity of the error (CE/UE/Fatal)
 * @mci:		a struct mem_ctl_info pointer
 * @error_count:	Number of errors of the same type
 * @page_frame_number:	mem page where the error occurred
 * @offset_in_page:	offset of the error inside the page
 * @syndrome:		ECC syndrome
 * @top_layer:		Memory layer[0] position
 * @mid_layer:		Memory layer[1] position
 * @low_layer:		Memory layer[2] position
 * @msg:		Message meaningful to the end users that
 *			explains the event
 * @other_detail:	Technical details about the event that
 *			may help hardware manufacturers and
 *			EDAC developers to analyse the event
 */
void edac_mc_handle_error(const enum hw_event_mc_err_type type,
			  struct mem_ctl_info *mci,
			  const u16 error_count,
			  const unsigned long page_frame_number,
			  const unsigned long offset_in_page,
			  const unsigned long syndrome,
			  const int top_layer,
			  const int mid_layer,
			  const int low_layer,
			  const char *msg,
			  const char *other_detail)
{
	char *p;
	int row = -1, chan = -1;
	int pos[EDAC_MAX_LAYERS] = { top_layer, mid_layer, low_layer };
	int i, n_labels = 0;
	u8 grain_bits;
	struct edac_raw_error_desc *e = &mci->error_desc;

	edac_dbg(3, "MC%d\n", mci->mc_idx);

	/* Fills the error report buffer */
	memset(e, 0, sizeof (*e));
	e->error_count = error_count;
	e->top_layer = top_layer;
	e->mid_layer = mid_layer;
	e->low_layer = low_layer;
	e->page_frame_number = page_frame_number;
	e->offset_in_page = offset_in_page;
	e->syndrome = syndrome;
	e->msg = msg;
	e->other_detail = other_detail;

	/*
	 * Check if the event report is consistent and if the memory
	 * location is known. If it is known, enable_per_layer_report will be
	 * true, the DIMM(s) label info will be filled and the per-layer
	 * error counters will be incremented.
	 */
	for (i = 0; i < mci->n_layers; i++) {
		if (pos[i] >= (int)mci->layers[i].size) {

			edac_mc_printk(mci, KERN_ERR,
				       "INTERNAL ERROR: %s value is out of range (%d >= %d)\n",
				       edac_layer_name[mci->layers[i].type],
				       pos[i], mci->layers[i].size);
			/*
			 * Instead of just returning it, let's use what's
			 * known about the error. The increment routines and
			 * the DIMM filter logic will do the right thing by
			 * pointing the likely damaged DIMMs.
			 */
			pos[i] = -1;
		}
		if (pos[i] >= 0)
			e->enable_per_layer_report = true;
	}

	/*
	 * Get the dimm label/grain that applies to the match criteria.
	 * As the error algorithm may not be able to point to just one memory
	 * stick, the logic here will get all possible labels that could
	 * pottentially be affected by the error.
	 * On FB-DIMM memory controllers, for uncorrected errors, it is common
	 * to have only the MC channel and the MC dimm (also called "branch")
	 * but the channel is not known, as the memory is arranged in pairs,
	 * where each memory belongs to a separate channel within the same
	 * branch.
	 */
	p = e->label;
	*p = '\0';

	for (i = 0; i < mci->tot_dimms; i++) {
		struct dimm_info *dimm = mci->dimms[i];

		if (top_layer >= 0 && top_layer != dimm->location[0])
			continue;
		if (mid_layer >= 0 && mid_layer != dimm->location[1])
			continue;
		if (low_layer >= 0 && low_layer != dimm->location[2])
			continue;

		/* get the max grain, over the error match range */
		if (dimm->grain > e->grain)
			e->grain = dimm->grain;

		/*
		 * If the error is memory-controller wide, there's no need to
		 * seek for the affected DIMMs because the whole
		 * channel/memory controller/...  may be affected.
		 * Also, don't show errors for empty DIMM slots.
		 */
		if (e->enable_per_layer_report && dimm->nr_pages) {
			if (n_labels >= EDAC_MAX_LABELS) {
				e->enable_per_layer_report = false;
				break;
			}
			n_labels++;
			if (p != e->label) {
				strcpy(p, OTHER_LABEL);
				p += strlen(OTHER_LABEL);
			}
			strcpy(p, dimm->label);
			p += strlen(p);
			*p = '\0';

			/*
			 * get csrow/channel of the DIMM, in order to allow
			 * incrementing the compat API counters
			 */
			edac_dbg(4, "%s csrows map: (%d,%d)\n",
				 mci->csbased ? "rank" : "dimm",
				 dimm->csrow, dimm->cschannel);
			if (row == -1)
				row = dimm->csrow;
			else if (row >= 0 && row != dimm->csrow)
				row = -2;

			if (chan == -1)
				chan = dimm->cschannel;
			else if (chan >= 0 && chan != dimm->cschannel)
				chan = -2;
		}
	}

	if (!e->enable_per_layer_report) {
		strcpy(e->label, "any memory");
	} else {
		edac_dbg(4, "csrow/channel to increment: (%d,%d)\n", row, chan);
		if (p == e->label)
			strcpy(e->label, "unknown memory");
		if (type == HW_EVENT_ERR_CORRECTED) {
			if (row >= 0) {
				mci->csrows[row]->ce_count += error_count;
				if (chan >= 0)
					mci->csrows[row]->channels[chan]->ce_count += error_count;
			}
		} else
			if (row >= 0)
				mci->csrows[row]->ue_count += error_count;
	}

	/* Fill the RAM location data */
	p = e->location;

	for (i = 0; i < mci->n_layers; i++) {
		if (pos[i] < 0)
			continue;

		p += sprintf(p, "%s:%d ",
			     edac_layer_name[mci->layers[i].type],
			     pos[i]);
	}
	if (p > e->location)
		*(p - 1) = '\0';

	/* Report the error via the trace interface */
	grain_bits = fls_long(e->grain) + 1;
	trace_mc_event(type, e->msg, e->label, e->error_count,
		       mci->mc_idx, e->top_layer, e->mid_layer, e->low_layer,
		       PAGES_TO_MiB(e->page_frame_number) | e->offset_in_page,
		       grain_bits, e->syndrome, e->other_detail);

	edac_raw_mc_handle_error(type, mci, e);
}
EXPORT_SYMBOL_GPL(edac_mc_handle_error);
>>>>>>> refs/remotes/origin/master
