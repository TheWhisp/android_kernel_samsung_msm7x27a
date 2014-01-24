/*
 * Copyright (c) International Business Machines Corp., 2006
 * Copyright (c) Nokia Corporation, 2006, 2007
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Author: Artem Bityutskiy (Битюцкий Артём)
 */

/*
 * This file includes volume table manipulation code. The volume table is an
 * on-flash table containing volume meta-data like name, number of reserved
 * physical eraseblocks, type, etc. The volume table is stored in the so-called
 * "layout volume".
 *
 * The layout volume is an internal volume which is organized as follows. It
 * consists of two logical eraseblocks - LEB 0 and LEB 1. Each logical
 * eraseblock stores one volume table copy, i.e. LEB 0 and LEB 1 duplicate each
 * other. This redundancy guarantees robustness to unclean reboots. The volume
 * table is basically an array of volume table records. Each record contains
 * full information about the volume and protected by a CRC checksum.
 *
 * The volume table is changed, it is first changed in RAM. Then LEB 0 is
 * erased, and the updated volume table is written back to LEB 0. Then same for
 * LEB 1. This scheme guarantees recoverability from unclean reboots.
 *
 * In this UBI implementation the on-flash volume table does not contain any
<<<<<<< HEAD
 * information about how many data static volumes contain. This information may
 * be found from the scanning data.
=======
 * information about how much data static volumes contain.
>>>>>>> refs/remotes/origin/master
 *
 * But it would still be beneficial to store this information in the volume
 * table. For example, suppose we have a static volume X, and all its physical
 * eraseblocks became bad for some reasons. Suppose we are attaching the
<<<<<<< HEAD
 * corresponding MTD device, the scanning has found no logical eraseblocks
 * corresponding to the volume X. According to the volume table volume X does
 * exist. So we don't know whether it is just empty or all its physical
 * eraseblocks went bad. So we cannot alarm the user about this corruption.
=======
 * corresponding MTD device, for some reason we find no logical eraseblocks
 * corresponding to the volume X. According to the volume table volume X does
 * exist. So we don't know whether it is just empty or all its physical
 * eraseblocks went bad. So we cannot alarm the user properly.
>>>>>>> refs/remotes/origin/master
 *
 * The volume table also stores so-called "update marker", which is used for
 * volume updates. Before updating the volume, the update marker is set, and
 * after the update operation is finished, the update marker is cleared. So if
 * the update operation was interrupted (e.g. by an unclean reboot) - the
 * update marker is still there and we know that the volume's contents is
 * damaged.
 */

#include <linux/crc32.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <asm/div64.h>
#include "ubi.h"

<<<<<<< HEAD
#ifdef CONFIG_MTD_UBI_DEBUG
static void paranoid_vtbl_check(const struct ubi_device *ubi);
#else
#define paranoid_vtbl_check(ubi)
#endif
=======
static void self_vtbl_check(const struct ubi_device *ubi);
>>>>>>> refs/remotes/origin/master

/* Empty volume table record */
static struct ubi_vtbl_record empty_vtbl_record;

/**
 * ubi_change_vtbl_record - change volume table record.
 * @ubi: UBI device description object
 * @idx: table index to change
 * @vtbl_rec: new volume table record
 *
 * This function changes volume table record @idx. If @vtbl_rec is %NULL, empty
 * volume table record is written. The caller does not have to calculate CRC of
 * the record as it is done by this function. Returns zero in case of success
 * and a negative error code in case of failure.
 */
int ubi_change_vtbl_record(struct ubi_device *ubi, int idx,
			   struct ubi_vtbl_record *vtbl_rec)
{
	int i, err;
	uint32_t crc;
	struct ubi_volume *layout_vol;

	ubi_assert(idx >= 0 && idx < ubi->vtbl_slots);
	layout_vol = ubi->volumes[vol_id2idx(ubi, UBI_LAYOUT_VOLUME_ID)];

	if (!vtbl_rec)
		vtbl_rec = &empty_vtbl_record;
	else {
		crc = crc32(UBI_CRC32_INIT, vtbl_rec, UBI_VTBL_RECORD_SIZE_CRC);
		vtbl_rec->crc = cpu_to_be32(crc);
	}

	memcpy(&ubi->vtbl[idx], vtbl_rec, sizeof(struct ubi_vtbl_record));
	for (i = 0; i < UBI_LAYOUT_VOLUME_EBS; i++) {
		err = ubi_eba_unmap_leb(ubi, layout_vol, i);
		if (err)
			return err;

		err = ubi_eba_write_leb(ubi, layout_vol, i, ubi->vtbl, 0,
<<<<<<< HEAD
					ubi->vtbl_size, UBI_LONGTERM);
=======
					ubi->vtbl_size);
>>>>>>> refs/remotes/origin/master
		if (err)
			return err;
	}

<<<<<<< HEAD
	paranoid_vtbl_check(ubi);
=======
	self_vtbl_check(ubi);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/**
 * ubi_vtbl_rename_volumes - rename UBI volumes in the volume table.
 * @ubi: UBI device description object
 * @rename_list: list of &struct ubi_rename_entry objects
 *
 * This function re-names multiple volumes specified in @req in the volume
 * table. Returns zero in case of success and a negative error code in case of
 * failure.
 */
int ubi_vtbl_rename_volumes(struct ubi_device *ubi,
			    struct list_head *rename_list)
{
	int i, err;
	struct ubi_rename_entry *re;
	struct ubi_volume *layout_vol;

	list_for_each_entry(re, rename_list, list) {
		uint32_t crc;
		struct ubi_volume *vol = re->desc->vol;
		struct ubi_vtbl_record *vtbl_rec = &ubi->vtbl[vol->vol_id];

		if (re->remove) {
			memcpy(vtbl_rec, &empty_vtbl_record,
			       sizeof(struct ubi_vtbl_record));
			continue;
		}

		vtbl_rec->name_len = cpu_to_be16(re->new_name_len);
		memcpy(vtbl_rec->name, re->new_name, re->new_name_len);
		memset(vtbl_rec->name + re->new_name_len, 0,
		       UBI_VOL_NAME_MAX + 1 - re->new_name_len);
		crc = crc32(UBI_CRC32_INIT, vtbl_rec,
			    UBI_VTBL_RECORD_SIZE_CRC);
		vtbl_rec->crc = cpu_to_be32(crc);
	}

	layout_vol = ubi->volumes[vol_id2idx(ubi, UBI_LAYOUT_VOLUME_ID)];
	for (i = 0; i < UBI_LAYOUT_VOLUME_EBS; i++) {
		err = ubi_eba_unmap_leb(ubi, layout_vol, i);
		if (err)
			return err;

		err = ubi_eba_write_leb(ubi, layout_vol, i, ubi->vtbl, 0,
<<<<<<< HEAD
					ubi->vtbl_size, UBI_LONGTERM);
=======
					ubi->vtbl_size);
>>>>>>> refs/remotes/origin/master
		if (err)
			return err;
	}

	return 0;
}

/**
 * vtbl_check - check if volume table is not corrupted and sensible.
 * @ubi: UBI device description object
 * @vtbl: volume table
 *
 * This function returns zero if @vtbl is all right, %1 if CRC is incorrect,
 * and %-EINVAL if it contains inconsistent data.
 */
static int vtbl_check(const struct ubi_device *ubi,
		      const struct ubi_vtbl_record *vtbl)
{
	int i, n, reserved_pebs, alignment, data_pad, vol_type, name_len;
	int upd_marker, err;
	uint32_t crc;
	const char *name;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		cond_resched();

		reserved_pebs = be32_to_cpu(vtbl[i].reserved_pebs);
		alignment = be32_to_cpu(vtbl[i].alignment);
		data_pad = be32_to_cpu(vtbl[i].data_pad);
		upd_marker = vtbl[i].upd_marker;
		vol_type = vtbl[i].vol_type;
		name_len = be16_to_cpu(vtbl[i].name_len);
		name = &vtbl[i].name[0];

		crc = crc32(UBI_CRC32_INIT, &vtbl[i], UBI_VTBL_RECORD_SIZE_CRC);
		if (be32_to_cpu(vtbl[i].crc) != crc) {
			ubi_err("bad CRC at record %u: %#08x, not %#08x",
				 i, crc, be32_to_cpu(vtbl[i].crc));
<<<<<<< HEAD
			ubi_dbg_dump_vtbl_record(&vtbl[i], i);
=======
			ubi_dump_vtbl_record(&vtbl[i], i);
>>>>>>> refs/remotes/origin/master
			return 1;
		}

		if (reserved_pebs == 0) {
			if (memcmp(&vtbl[i], &empty_vtbl_record,
						UBI_VTBL_RECORD_SIZE)) {
				err = 2;
				goto bad;
			}
			continue;
		}

		if (reserved_pebs < 0 || alignment < 0 || data_pad < 0 ||
		    name_len < 0) {
			err = 3;
			goto bad;
		}

		if (alignment > ubi->leb_size || alignment == 0) {
			err = 4;
			goto bad;
		}

		n = alignment & (ubi->min_io_size - 1);
		if (alignment != 1 && n) {
			err = 5;
			goto bad;
		}

		n = ubi->leb_size % alignment;
		if (data_pad != n) {
<<<<<<< HEAD
			dbg_err("bad data_pad, has to be %d", n);
=======
			ubi_err("bad data_pad, has to be %d", n);
>>>>>>> refs/remotes/origin/master
			err = 6;
			goto bad;
		}

		if (vol_type != UBI_VID_DYNAMIC && vol_type != UBI_VID_STATIC) {
			err = 7;
			goto bad;
		}

		if (upd_marker != 0 && upd_marker != 1) {
			err = 8;
			goto bad;
		}

		if (reserved_pebs > ubi->good_peb_count) {
<<<<<<< HEAD
			dbg_err("too large reserved_pebs %d, good PEBs %d",
=======
			ubi_err("too large reserved_pebs %d, good PEBs %d",
>>>>>>> refs/remotes/origin/master
				reserved_pebs, ubi->good_peb_count);
			err = 9;
			goto bad;
		}

		if (name_len > UBI_VOL_NAME_MAX) {
			err = 10;
			goto bad;
		}

		if (name[0] == '\0') {
			err = 11;
			goto bad;
		}

		if (name_len != strnlen(name, name_len + 1)) {
			err = 12;
			goto bad;
		}
	}

	/* Checks that all names are unique */
	for (i = 0; i < ubi->vtbl_slots - 1; i++) {
		for (n = i + 1; n < ubi->vtbl_slots; n++) {
			int len1 = be16_to_cpu(vtbl[i].name_len);
			int len2 = be16_to_cpu(vtbl[n].name_len);

			if (len1 > 0 && len1 == len2 &&
			    !strncmp(vtbl[i].name, vtbl[n].name, len1)) {
<<<<<<< HEAD
				ubi_err("volumes %d and %d have the same name"
					" \"%s\"", i, n, vtbl[i].name);
				ubi_dbg_dump_vtbl_record(&vtbl[i], i);
				ubi_dbg_dump_vtbl_record(&vtbl[n], n);
=======
				ubi_err("volumes %d and %d have the same name \"%s\"",
					i, n, vtbl[i].name);
				ubi_dump_vtbl_record(&vtbl[i], i);
				ubi_dump_vtbl_record(&vtbl[n], n);
>>>>>>> refs/remotes/origin/master
				return -EINVAL;
			}
		}
	}

	return 0;

bad:
	ubi_err("volume table check failed: record %d, error %d", i, err);
<<<<<<< HEAD
	ubi_dbg_dump_vtbl_record(&vtbl[i], i);
=======
	ubi_dump_vtbl_record(&vtbl[i], i);
>>>>>>> refs/remotes/origin/master
	return -EINVAL;
}

/**
 * create_vtbl - create a copy of volume table.
 * @ubi: UBI device description object
<<<<<<< HEAD
 * @si: scanning information
=======
 * @ai: attaching information
>>>>>>> refs/remotes/origin/master
 * @copy: number of the volume table copy
 * @vtbl: contents of the volume table
 *
 * This function returns zero in case of success and a negative error code in
 * case of failure.
 */
<<<<<<< HEAD
static int create_vtbl(struct ubi_device *ubi, struct ubi_scan_info *si,
		       int copy, void *vtbl)
{
	int err, tries = 0;
<<<<<<< HEAD
	static struct ubi_vid_hdr *vid_hdr;
	struct ubi_scan_volume *sv;
	struct ubi_scan_leb *new_seb, *old_seb = NULL;
=======
	struct ubi_vid_hdr *vid_hdr;
	struct ubi_scan_leb *new_seb;
>>>>>>> refs/remotes/origin/cm-10.0

	ubi_msg("create volume table (copy #%d)", copy + 1);
=======
static int create_vtbl(struct ubi_device *ubi, struct ubi_attach_info *ai,
		       int copy, void *vtbl)
{
	int err, tries = 0;
	struct ubi_vid_hdr *vid_hdr;
	struct ubi_ainf_peb *new_aeb;

	dbg_gen("create volume table (copy #%d)", copy + 1);
>>>>>>> refs/remotes/origin/master

	vid_hdr = ubi_zalloc_vid_hdr(ubi, GFP_KERNEL);
	if (!vid_hdr)
		return -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * Check if there is a logical eraseblock which would have to contain
	 * this volume table copy was found during scanning. It has to be wiped
	 * out.
	 */
	sv = ubi_scan_find_sv(si, UBI_LAYOUT_VOLUME_ID);
	if (sv)
		old_seb = ubi_scan_find_seb(sv, copy);

=======
>>>>>>> refs/remotes/origin/cm-10.0
retry:
	new_seb = ubi_scan_get_free_peb(ubi, si);
	if (IS_ERR(new_seb)) {
		err = PTR_ERR(new_seb);
		goto out_free;
	}

<<<<<<< HEAD
	vid_hdr->vol_type = UBI_VID_DYNAMIC;
=======
	vid_hdr->vol_type = UBI_LAYOUT_VOLUME_TYPE;
>>>>>>> refs/remotes/origin/cm-10.0
=======
retry:
	new_aeb = ubi_early_get_peb(ubi, ai);
	if (IS_ERR(new_aeb)) {
		err = PTR_ERR(new_aeb);
		goto out_free;
	}

	vid_hdr->vol_type = UBI_LAYOUT_VOLUME_TYPE;
>>>>>>> refs/remotes/origin/master
	vid_hdr->vol_id = cpu_to_be32(UBI_LAYOUT_VOLUME_ID);
	vid_hdr->compat = UBI_LAYOUT_VOLUME_COMPAT;
	vid_hdr->data_size = vid_hdr->used_ebs =
			     vid_hdr->data_pad = cpu_to_be32(0);
	vid_hdr->lnum = cpu_to_be32(copy);
<<<<<<< HEAD
	vid_hdr->sqnum = cpu_to_be64(++si->max_sqnum);

	/* The EC header is already there, write the VID header */
	err = ubi_io_write_vid_hdr(ubi, new_seb->pnum, vid_hdr);
=======
	vid_hdr->sqnum = cpu_to_be64(++ai->max_sqnum);

	/* The EC header is already there, write the VID header */
	err = ubi_io_write_vid_hdr(ubi, new_aeb->pnum, vid_hdr);
>>>>>>> refs/remotes/origin/master
	if (err)
		goto write_error;

	/* Write the layout volume contents */
<<<<<<< HEAD
	err = ubi_io_write_data(ubi, vtbl, new_seb->pnum, 0, ubi->vtbl_size);
=======
	err = ubi_io_write_data(ubi, vtbl, new_aeb->pnum, 0, ubi->vtbl_size);
>>>>>>> refs/remotes/origin/master
	if (err)
		goto write_error;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	 * And add it to the scanning information. Don't delete the old
	 * @old_seb as it will be deleted and freed in 'ubi_scan_add_used()'.
=======
	 * And add it to the scanning information. Don't delete the old version
	 * of this LEB as it will be deleted and freed in 'ubi_scan_add_used()'.
>>>>>>> refs/remotes/origin/cm-10.0
	 */
	err = ubi_scan_add_used(ubi, si, new_seb->pnum, new_seb->ec,
				vid_hdr, 0);
	kmem_cache_free(si->scan_leb_slab, new_seb);
<<<<<<< HEAD
=======
	 * And add it to the attaching information. Don't delete the old version
	 * of this LEB as it will be deleted and freed in 'ubi_add_to_av()'.
	 */
	err = ubi_add_to_av(ubi, ai, new_aeb->pnum, new_aeb->ec, vid_hdr, 0);
	kmem_cache_free(ai->aeb_slab_cache, new_aeb);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ubi_free_vid_hdr(ubi, vid_hdr);
	return err;

write_error:
	if (err == -EIO && ++tries <= 5) {
		/*
		 * Probably this physical eraseblock went bad, try to pick
		 * another one.
		 */
<<<<<<< HEAD
		list_add(&new_seb->u.list, &si->erase);
		goto retry;
	}
	kmem_cache_free(si->scan_leb_slab, new_seb);
<<<<<<< HEAD
=======
		list_add(&new_aeb->u.list, &ai->erase);
		goto retry;
	}
	kmem_cache_free(ai->aeb_slab_cache, new_aeb);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
out_free:
	ubi_free_vid_hdr(ubi, vid_hdr);
	return err;

}

/**
 * process_lvol - process the layout volume.
 * @ubi: UBI device description object
<<<<<<< HEAD
 * @si: scanning information
 * @sv: layout volume scanning information
=======
 * @ai: attaching information
 * @av: layout volume attaching information
>>>>>>> refs/remotes/origin/master
 *
 * This function is responsible for reading the layout volume, ensuring it is
 * not corrupted, and recovering from corruptions if needed. Returns volume
 * table in case of success and a negative error code in case of failure.
 */
static struct ubi_vtbl_record *process_lvol(struct ubi_device *ubi,
<<<<<<< HEAD
					    struct ubi_scan_info *si,
					    struct ubi_scan_volume *sv)
{
	int err;
	struct rb_node *rb;
	struct ubi_scan_leb *seb;
=======
					    struct ubi_attach_info *ai,
					    struct ubi_ainf_volume *av)
{
	int err;
	struct rb_node *rb;
	struct ubi_ainf_peb *aeb;
>>>>>>> refs/remotes/origin/master
	struct ubi_vtbl_record *leb[UBI_LAYOUT_VOLUME_EBS] = { NULL, NULL };
	int leb_corrupted[UBI_LAYOUT_VOLUME_EBS] = {1, 1};

	/*
	 * UBI goes through the following steps when it changes the layout
	 * volume:
	 * a. erase LEB 0;
	 * b. write new data to LEB 0;
	 * c. erase LEB 1;
	 * d. write new data to LEB 1.
	 *
	 * Before the change, both LEBs contain the same data.
	 *
	 * Due to unclean reboots, the contents of LEB 0 may be lost, but there
	 * should LEB 1. So it is OK if LEB 0 is corrupted while LEB 1 is not.
	 * Similarly, LEB 1 may be lost, but there should be LEB 0. And
	 * finally, unclean reboots may result in a situation when neither LEB
	 * 0 nor LEB 1 are corrupted, but they are different. In this case, LEB
	 * 0 contains more recent information.
	 *
	 * So the plan is to first check LEB 0. Then
	 * a. if LEB 0 is OK, it must be containing the most recent data; then
	 *    we compare it with LEB 1, and if they are different, we copy LEB
	 *    0 to LEB 1;
	 * b. if LEB 0 is corrupted, but LEB 1 has to be OK, and we copy LEB 1
	 *    to LEB 0.
	 */

	dbg_gen("check layout volume");

	/* Read both LEB 0 and LEB 1 into memory */
<<<<<<< HEAD
	ubi_rb_for_each_entry(rb, seb, &sv->root, u.rb) {
		leb[seb->lnum] = vzalloc(ubi->vtbl_size);
		if (!leb[seb->lnum]) {
=======
	ubi_rb_for_each_entry(rb, aeb, &av->root, u.rb) {
		leb[aeb->lnum] = vzalloc(ubi->vtbl_size);
		if (!leb[aeb->lnum]) {
>>>>>>> refs/remotes/origin/master
			err = -ENOMEM;
			goto out_free;
		}

<<<<<<< HEAD
		err = ubi_io_read_data(ubi, leb[seb->lnum], seb->pnum, 0,
				       ubi->vtbl_size);
<<<<<<< HEAD
		if (err == UBI_IO_BITFLIPS || err == -EBADMSG)
=======
		if (err == UBI_IO_BITFLIPS || mtd_is_eccerr(err))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		err = ubi_io_read_data(ubi, leb[aeb->lnum], aeb->pnum, 0,
				       ubi->vtbl_size);
		if (err == UBI_IO_BITFLIPS || mtd_is_eccerr(err))
>>>>>>> refs/remotes/origin/master
			/*
			 * Scrub the PEB later. Note, -EBADMSG indicates an
			 * uncorrectable ECC error, but we have our own CRC and
			 * the data will be checked later. If the data is OK,
			 * the PEB will be scrubbed (because we set
<<<<<<< HEAD
			 * seb->scrub). If the data is not OK, the contents of
			 * the PEB will be recovered from the second copy, and
			 * seb->scrub will be cleared in
			 * 'ubi_scan_add_used()'.
			 */
			seb->scrub = 1;
=======
			 * aeb->scrub). If the data is not OK, the contents of
			 * the PEB will be recovered from the second copy, and
			 * aeb->scrub will be cleared in
			 * 'ubi_add_to_av()'.
			 */
			aeb->scrub = 1;
>>>>>>> refs/remotes/origin/master
		else if (err)
			goto out_free;
	}

	err = -EINVAL;
	if (leb[0]) {
		leb_corrupted[0] = vtbl_check(ubi, leb[0]);
		if (leb_corrupted[0] < 0)
			goto out_free;
	}

	if (!leb_corrupted[0]) {
		/* LEB 0 is OK */
		if (leb[1])
			leb_corrupted[1] = memcmp(leb[0], leb[1],
						  ubi->vtbl_size);
		if (leb_corrupted[1]) {
			ubi_warn("volume table copy #2 is corrupted");
<<<<<<< HEAD
			err = create_vtbl(ubi, si, 1, leb[0]);
=======
			err = create_vtbl(ubi, ai, 1, leb[0]);
>>>>>>> refs/remotes/origin/master
			if (err)
				goto out_free;
			ubi_msg("volume table was restored");
		}

		/* Both LEB 1 and LEB 2 are OK and consistent */
		vfree(leb[1]);
		return leb[0];
	} else {
		/* LEB 0 is corrupted or does not exist */
		if (leb[1]) {
			leb_corrupted[1] = vtbl_check(ubi, leb[1]);
			if (leb_corrupted[1] < 0)
				goto out_free;
		}
		if (leb_corrupted[1]) {
			/* Both LEB 0 and LEB 1 are corrupted */
			ubi_err("both volume tables are corrupted");
			goto out_free;
		}

		ubi_warn("volume table copy #1 is corrupted");
<<<<<<< HEAD
		err = create_vtbl(ubi, si, 0, leb[1]);
=======
		err = create_vtbl(ubi, ai, 0, leb[1]);
>>>>>>> refs/remotes/origin/master
		if (err)
			goto out_free;
		ubi_msg("volume table was restored");

		vfree(leb[0]);
		return leb[1];
	}

out_free:
	vfree(leb[0]);
	vfree(leb[1]);
	return ERR_PTR(err);
}

/**
 * create_empty_lvol - create empty layout volume.
 * @ubi: UBI device description object
<<<<<<< HEAD
 * @si: scanning information
=======
 * @ai: attaching information
>>>>>>> refs/remotes/origin/master
 *
 * This function returns volume table contents in case of success and a
 * negative error code in case of failure.
 */
static struct ubi_vtbl_record *create_empty_lvol(struct ubi_device *ubi,
<<<<<<< HEAD
						 struct ubi_scan_info *si)
=======
						 struct ubi_attach_info *ai)
>>>>>>> refs/remotes/origin/master
{
	int i;
	struct ubi_vtbl_record *vtbl;

	vtbl = vzalloc(ubi->vtbl_size);
	if (!vtbl)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < ubi->vtbl_slots; i++)
		memcpy(&vtbl[i], &empty_vtbl_record, UBI_VTBL_RECORD_SIZE);

	for (i = 0; i < UBI_LAYOUT_VOLUME_EBS; i++) {
		int err;

<<<<<<< HEAD
		err = create_vtbl(ubi, si, i, vtbl);
=======
		err = create_vtbl(ubi, ai, i, vtbl);
>>>>>>> refs/remotes/origin/master
		if (err) {
			vfree(vtbl);
			return ERR_PTR(err);
		}
	}

	return vtbl;
}

/**
 * init_volumes - initialize volume information for existing volumes.
 * @ubi: UBI device description object
<<<<<<< HEAD
 * @si: scanning information
=======
 * @ai: scanning information
>>>>>>> refs/remotes/origin/master
 * @vtbl: volume table
 *
 * This function allocates volume description objects for existing volumes.
 * Returns zero in case of success and a negative error code in case of
 * failure.
 */
<<<<<<< HEAD
static int init_volumes(struct ubi_device *ubi, const struct ubi_scan_info *si,
			const struct ubi_vtbl_record *vtbl)
{
	int i, reserved_pebs = 0;
	struct ubi_scan_volume *sv;
=======
static int init_volumes(struct ubi_device *ubi,
			const struct ubi_attach_info *ai,
			const struct ubi_vtbl_record *vtbl)
{
	int i, reserved_pebs = 0;
	struct ubi_ainf_volume *av;
>>>>>>> refs/remotes/origin/master
	struct ubi_volume *vol;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		cond_resched();

		if (be32_to_cpu(vtbl[i].reserved_pebs) == 0)
			continue; /* Empty record */

		vol = kzalloc(sizeof(struct ubi_volume), GFP_KERNEL);
		if (!vol)
			return -ENOMEM;

		vol->reserved_pebs = be32_to_cpu(vtbl[i].reserved_pebs);
		vol->alignment = be32_to_cpu(vtbl[i].alignment);
		vol->data_pad = be32_to_cpu(vtbl[i].data_pad);
		vol->upd_marker = vtbl[i].upd_marker;
		vol->vol_type = vtbl[i].vol_type == UBI_VID_DYNAMIC ?
					UBI_DYNAMIC_VOLUME : UBI_STATIC_VOLUME;
		vol->name_len = be16_to_cpu(vtbl[i].name_len);
		vol->usable_leb_size = ubi->leb_size - vol->data_pad;
		memcpy(vol->name, vtbl[i].name, vol->name_len);
		vol->name[vol->name_len] = '\0';
		vol->vol_id = i;

		if (vtbl[i].flags & UBI_VTBL_AUTORESIZE_FLG) {
			/* Auto re-size flag may be set only for one volume */
			if (ubi->autoresize_vol_id != -1) {
<<<<<<< HEAD
				ubi_err("more than one auto-resize volume (%d "
					"and %d)", ubi->autoresize_vol_id, i);
=======
				ubi_err("more than one auto-resize volume (%d and %d)",
					ubi->autoresize_vol_id, i);
>>>>>>> refs/remotes/origin/master
				kfree(vol);
				return -EINVAL;
			}

			ubi->autoresize_vol_id = i;
		}

		ubi_assert(!ubi->volumes[i]);
		ubi->volumes[i] = vol;
		ubi->vol_count += 1;
		vol->ubi = ubi;
		reserved_pebs += vol->reserved_pebs;

		/*
		 * In case of dynamic volume UBI knows nothing about how many
		 * data is stored there. So assume the whole volume is used.
		 */
		if (vol->vol_type == UBI_DYNAMIC_VOLUME) {
			vol->used_ebs = vol->reserved_pebs;
			vol->last_eb_bytes = vol->usable_leb_size;
			vol->used_bytes =
				(long long)vol->used_ebs * vol->usable_leb_size;
			continue;
		}

		/* Static volumes only */
<<<<<<< HEAD
		sv = ubi_scan_find_sv(si, i);
		if (!sv) {
=======
		av = ubi_find_av(ai, i);
		if (!av) {
>>>>>>> refs/remotes/origin/master
			/*
			 * No eraseblocks belonging to this volume found. We
			 * don't actually know whether this static volume is
			 * completely corrupted or just contains no data. And
			 * we cannot know this as long as data size is not
			 * stored on flash. So we just assume the volume is
			 * empty. FIXME: this should be handled.
			 */
			continue;
		}

<<<<<<< HEAD
		if (sv->leb_count != sv->used_ebs) {
=======
		if (av->leb_count != av->used_ebs) {
>>>>>>> refs/remotes/origin/master
			/*
			 * We found a static volume which misses several
			 * eraseblocks. Treat it as corrupted.
			 */
			ubi_warn("static volume %d misses %d LEBs - corrupted",
<<<<<<< HEAD
				 sv->vol_id, sv->used_ebs - sv->leb_count);
=======
				 av->vol_id, av->used_ebs - av->leb_count);
>>>>>>> refs/remotes/origin/master
			vol->corrupted = 1;
			continue;
		}

<<<<<<< HEAD
		vol->used_ebs = sv->used_ebs;
		vol->used_bytes =
			(long long)(vol->used_ebs - 1) * vol->usable_leb_size;
		vol->used_bytes += sv->last_data_size;
		vol->last_eb_bytes = sv->last_data_size;
=======
		vol->used_ebs = av->used_ebs;
		vol->used_bytes =
			(long long)(vol->used_ebs - 1) * vol->usable_leb_size;
		vol->used_bytes += av->last_data_size;
		vol->last_eb_bytes = av->last_data_size;
>>>>>>> refs/remotes/origin/master
	}

	/* And add the layout volume */
	vol = kzalloc(sizeof(struct ubi_volume), GFP_KERNEL);
	if (!vol)
		return -ENOMEM;

	vol->reserved_pebs = UBI_LAYOUT_VOLUME_EBS;
<<<<<<< HEAD
<<<<<<< HEAD
	vol->alignment = 1;
=======
	vol->alignment = UBI_LAYOUT_VOLUME_ALIGN;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	vol->alignment = UBI_LAYOUT_VOLUME_ALIGN;
>>>>>>> refs/remotes/origin/master
	vol->vol_type = UBI_DYNAMIC_VOLUME;
	vol->name_len = sizeof(UBI_LAYOUT_VOLUME_NAME) - 1;
	memcpy(vol->name, UBI_LAYOUT_VOLUME_NAME, vol->name_len + 1);
	vol->usable_leb_size = ubi->leb_size;
	vol->used_ebs = vol->reserved_pebs;
	vol->last_eb_bytes = vol->reserved_pebs;
	vol->used_bytes =
		(long long)vol->used_ebs * (ubi->leb_size - vol->data_pad);
	vol->vol_id = UBI_LAYOUT_VOLUME_ID;
	vol->ref_count = 1;

	ubi_assert(!ubi->volumes[i]);
	ubi->volumes[vol_id2idx(ubi, vol->vol_id)] = vol;
	reserved_pebs += vol->reserved_pebs;
	ubi->vol_count += 1;
	vol->ubi = ubi;

	if (reserved_pebs > ubi->avail_pebs) {
		ubi_err("not enough PEBs, required %d, available %d",
			reserved_pebs, ubi->avail_pebs);
		if (ubi->corr_peb_count)
			ubi_err("%d PEBs are corrupted and not used",
				ubi->corr_peb_count);
	}
	ubi->rsvd_pebs += reserved_pebs;
	ubi->avail_pebs -= reserved_pebs;

	return 0;
}

/**
<<<<<<< HEAD
 * check_sv - check volume scanning information.
 * @vol: UBI volume description object
 * @sv: volume scanning information
 *
 * This function returns zero if the volume scanning information is consistent
 * to the data read from the volume tabla, and %-EINVAL if not.
 */
static int check_sv(const struct ubi_volume *vol,
		    const struct ubi_scan_volume *sv)
{
	int err;

	if (sv->highest_lnum >= vol->reserved_pebs) {
		err = 1;
		goto bad;
	}
	if (sv->leb_count > vol->reserved_pebs) {
		err = 2;
		goto bad;
	}
	if (sv->vol_type != vol->vol_type) {
		err = 3;
		goto bad;
	}
	if (sv->used_ebs > vol->reserved_pebs) {
		err = 4;
		goto bad;
	}
	if (sv->data_pad != vol->data_pad) {
=======
 * check_av - check volume attaching information.
 * @vol: UBI volume description object
 * @av: volume attaching information
 *
 * This function returns zero if the volume attaching information is consistent
 * to the data read from the volume tabla, and %-EINVAL if not.
 */
static int check_av(const struct ubi_volume *vol,
		    const struct ubi_ainf_volume *av)
{
	int err;

	if (av->highest_lnum >= vol->reserved_pebs) {
		err = 1;
		goto bad;
	}
	if (av->leb_count > vol->reserved_pebs) {
		err = 2;
		goto bad;
	}
	if (av->vol_type != vol->vol_type) {
		err = 3;
		goto bad;
	}
	if (av->used_ebs > vol->reserved_pebs) {
		err = 4;
		goto bad;
	}
	if (av->data_pad != vol->data_pad) {
>>>>>>> refs/remotes/origin/master
		err = 5;
		goto bad;
	}
	return 0;

bad:
<<<<<<< HEAD
	ubi_err("bad scanning information, error %d", err);
	ubi_dbg_dump_sv(sv);
	ubi_dbg_dump_vol_info(vol);
=======
	ubi_err("bad attaching information, error %d", err);
	ubi_dump_av(av);
	ubi_dump_vol_info(vol);
>>>>>>> refs/remotes/origin/master
	return -EINVAL;
}

/**
<<<<<<< HEAD
 * check_scanning_info - check that scanning information.
 * @ubi: UBI device description object
 * @si: scanning information
 *
 * Even though we protect on-flash data by CRC checksums, we still don't trust
 * the media. This function ensures that scanning information is consistent to
 * the information read from the volume table. Returns zero if the scanning
 * information is OK and %-EINVAL if it is not.
 */
static int check_scanning_info(const struct ubi_device *ubi,
			       struct ubi_scan_info *si)
{
	int err, i;
	struct ubi_scan_volume *sv;
	struct ubi_volume *vol;

	if (si->vols_found > UBI_INT_VOL_COUNT + ubi->vtbl_slots) {
		ubi_err("scanning found %d volumes, maximum is %d + %d",
			si->vols_found, UBI_INT_VOL_COUNT, ubi->vtbl_slots);
		return -EINVAL;
	}

	if (si->highest_vol_id >= ubi->vtbl_slots + UBI_INT_VOL_COUNT &&
	    si->highest_vol_id < UBI_INTERNAL_VOL_START) {
		ubi_err("too large volume ID %d found by scanning",
			si->highest_vol_id);
=======
 * check_attaching_info - check that attaching information.
 * @ubi: UBI device description object
 * @ai: attaching information
 *
 * Even though we protect on-flash data by CRC checksums, we still don't trust
 * the media. This function ensures that attaching information is consistent to
 * the information read from the volume table. Returns zero if the attaching
 * information is OK and %-EINVAL if it is not.
 */
static int check_attaching_info(const struct ubi_device *ubi,
			       struct ubi_attach_info *ai)
{
	int err, i;
	struct ubi_ainf_volume *av;
	struct ubi_volume *vol;

	if (ai->vols_found > UBI_INT_VOL_COUNT + ubi->vtbl_slots) {
		ubi_err("found %d volumes while attaching, maximum is %d + %d",
			ai->vols_found, UBI_INT_VOL_COUNT, ubi->vtbl_slots);
		return -EINVAL;
	}

	if (ai->highest_vol_id >= ubi->vtbl_slots + UBI_INT_VOL_COUNT &&
	    ai->highest_vol_id < UBI_INTERNAL_VOL_START) {
		ubi_err("too large volume ID %d found", ai->highest_vol_id);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	for (i = 0; i < ubi->vtbl_slots + UBI_INT_VOL_COUNT; i++) {
		cond_resched();

<<<<<<< HEAD
		sv = ubi_scan_find_sv(si, i);
		vol = ubi->volumes[i];
		if (!vol) {
			if (sv)
				ubi_scan_rm_volume(si, sv);
=======
		av = ubi_find_av(ai, i);
		vol = ubi->volumes[i];
		if (!vol) {
			if (av)
				ubi_remove_av(ai, av);
>>>>>>> refs/remotes/origin/master
			continue;
		}

		if (vol->reserved_pebs == 0) {
			ubi_assert(i < ubi->vtbl_slots);

<<<<<<< HEAD
			if (!sv)
				continue;

			/*
			 * During scanning we found a volume which does not
=======
			if (!av)
				continue;

			/*
			 * During attaching we found a volume which does not
>>>>>>> refs/remotes/origin/master
			 * exist according to the information in the volume
			 * table. This must have happened due to an unclean
			 * reboot while the volume was being removed. Discard
			 * these eraseblocks.
			 */
<<<<<<< HEAD
			ubi_msg("finish volume %d removal", sv->vol_id);
			ubi_scan_rm_volume(si, sv);
		} else if (sv) {
			err = check_sv(vol, sv);
=======
			ubi_msg("finish volume %d removal", av->vol_id);
			ubi_remove_av(ai, av);
		} else if (av) {
			err = check_av(vol, av);
>>>>>>> refs/remotes/origin/master
			if (err)
				return err;
		}
	}

	return 0;
}

/**
 * ubi_read_volume_table - read the volume table.
 * @ubi: UBI device description object
<<<<<<< HEAD
 * @si: scanning information
=======
 * @ai: attaching information
>>>>>>> refs/remotes/origin/master
 *
 * This function reads volume table, checks it, recover from errors if needed,
 * or creates it if needed. Returns zero in case of success and a negative
 * error code in case of failure.
 */
<<<<<<< HEAD
int ubi_read_volume_table(struct ubi_device *ubi, struct ubi_scan_info *si)
{
	int i, err;
	struct ubi_scan_volume *sv;
=======
int ubi_read_volume_table(struct ubi_device *ubi, struct ubi_attach_info *ai)
{
	int i, err;
	struct ubi_ainf_volume *av;
>>>>>>> refs/remotes/origin/master

	empty_vtbl_record.crc = cpu_to_be32(0xf116c36b);

	/*
	 * The number of supported volumes is limited by the eraseblock size
	 * and by the UBI_MAX_VOLUMES constant.
	 */
	ubi->vtbl_slots = ubi->leb_size / UBI_VTBL_RECORD_SIZE;
	if (ubi->vtbl_slots > UBI_MAX_VOLUMES)
		ubi->vtbl_slots = UBI_MAX_VOLUMES;

	ubi->vtbl_size = ubi->vtbl_slots * UBI_VTBL_RECORD_SIZE;
	ubi->vtbl_size = ALIGN(ubi->vtbl_size, ubi->min_io_size);

<<<<<<< HEAD
	sv = ubi_scan_find_sv(si, UBI_LAYOUT_VOLUME_ID);
	if (!sv) {
=======
	av = ubi_find_av(ai, UBI_LAYOUT_VOLUME_ID);
	if (!av) {
>>>>>>> refs/remotes/origin/master
		/*
		 * No logical eraseblocks belonging to the layout volume were
		 * found. This could mean that the flash is just empty. In
		 * this case we create empty layout volume.
		 *
		 * But if flash is not empty this must be a corruption or the
		 * MTD device just contains garbage.
		 */
<<<<<<< HEAD
		if (si->is_empty) {
			ubi->vtbl = create_empty_lvol(ubi, si);
=======
		if (ai->is_empty) {
			ubi->vtbl = create_empty_lvol(ubi, ai);
>>>>>>> refs/remotes/origin/master
			if (IS_ERR(ubi->vtbl))
				return PTR_ERR(ubi->vtbl);
		} else {
			ubi_err("the layout volume was not found");
			return -EINVAL;
		}
	} else {
<<<<<<< HEAD
		if (sv->leb_count > UBI_LAYOUT_VOLUME_EBS) {
			/* This must not happen with proper UBI images */
			dbg_err("too many LEBs (%d) in layout volume",
				sv->leb_count);
			return -EINVAL;
		}

		ubi->vtbl = process_lvol(ubi, si, sv);
=======
		if (av->leb_count > UBI_LAYOUT_VOLUME_EBS) {
			/* This must not happen with proper UBI images */
			ubi_err("too many LEBs (%d) in layout volume",
				av->leb_count);
			return -EINVAL;
		}

		ubi->vtbl = process_lvol(ubi, ai, av);
>>>>>>> refs/remotes/origin/master
		if (IS_ERR(ubi->vtbl))
			return PTR_ERR(ubi->vtbl);
	}

	ubi->avail_pebs = ubi->good_peb_count - ubi->corr_peb_count;

	/*
	 * The layout volume is OK, initialize the corresponding in-RAM data
	 * structures.
	 */
<<<<<<< HEAD
	err = init_volumes(ubi, si, ubi->vtbl);
=======
	err = init_volumes(ubi, ai, ubi->vtbl);
>>>>>>> refs/remotes/origin/master
	if (err)
		goto out_free;

	/*
<<<<<<< HEAD
	 * Make sure that the scanning information is consistent to the
	 * information stored in the volume table.
	 */
	err = check_scanning_info(ubi, si);
=======
	 * Make sure that the attaching information is consistent to the
	 * information stored in the volume table.
	 */
	err = check_attaching_info(ubi, ai);
>>>>>>> refs/remotes/origin/master
	if (err)
		goto out_free;

	return 0;

out_free:
	vfree(ubi->vtbl);
	for (i = 0; i < ubi->vtbl_slots + UBI_INT_VOL_COUNT; i++) {
		kfree(ubi->volumes[i]);
		ubi->volumes[i] = NULL;
	}
	return err;
}

<<<<<<< HEAD
#ifdef CONFIG_MTD_UBI_DEBUG

/**
 * paranoid_vtbl_check - check volume table.
 * @ubi: UBI device description object
 */
static void paranoid_vtbl_check(const struct ubi_device *ubi)
{
<<<<<<< HEAD
	if (!(ubi_chk_flags & UBI_CHK_GEN))
=======
	if (!ubi->dbg->chk_gen)
>>>>>>> refs/remotes/origin/cm-10.0
		return;

	if (vtbl_check(ubi, ubi->vtbl)) {
		ubi_err("paranoid check failed");
		BUG();
	}
}

#endif /* CONFIG_MTD_UBI_DEBUG */
=======
/**
 * self_vtbl_check - check volume table.
 * @ubi: UBI device description object
 */
static void self_vtbl_check(const struct ubi_device *ubi)
{
	if (!ubi_dbg_chk_gen(ubi))
		return;

	if (vtbl_check(ubi, ubi->vtbl)) {
		ubi_err("self-check failed");
		BUG();
	}
}
>>>>>>> refs/remotes/origin/master
