/*
 * Copyright (C) 2005,2006,2007,2008 IBM Corporation
 *
 * Authors:
 * Reiner Sailer      <sailer@watson.ibm.com>
 * Leendert van Doorn <leendert@watson.ibm.com>
 * Mimi Zohar         <zohar@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 * File: ima_init.c
 *             initialization and cleanup functions
 */
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <crypto/hash_info.h>
>>>>>>> refs/remotes/origin/master
#include "ima.h"

/* name for boot aggregate entry */
static const char *boot_aggregate_name = "boot_aggregate";
int ima_used_chip;

/* Add the boot aggregate to the IMA measurement list and extend
 * the PCR register.
 *
 * Calculate the boot aggregate, a SHA1 over tpm registers 0-7,
 * assuming a TPM chip exists, and zeroes if the TPM chip does not
 * exist.  Add the boot aggregate measurement to the measurement
 * list and extend the PCR register.
 *
 * If a tpm chip does not exist, indicate the core root of trust is
 * not hardware based by invalidating the aggregate PCR value.
 * (The aggregate PCR value is invalidated by adding one value to
 * the measurement list and extending the aggregate PCR value with
 * a different value.) Violations add a zero entry to the measurement
 * list and extend the aggregate PCR value with ff...ff's.
 */
static void __init ima_add_boot_aggregate(void)
{
	struct ima_template_entry *entry;
<<<<<<< HEAD
	const char *op = "add_boot_aggregate";
	const char *audit_cause = "ENOMEM";
	int result = -ENOMEM;
	int violation = 1;

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		goto err_out;

	memset(&entry->template, 0, sizeof(entry->template));
	strncpy(entry->template.file_name, boot_aggregate_name,
		IMA_EVENT_NAME_LEN_MAX);
	if (ima_used_chip) {
		violation = 0;
		result = ima_calc_boot_aggregate(entry->template.digest);
		if (result < 0) {
			audit_cause = "hashing_error";
			kfree(entry);
			goto err_out;
		}
	}
	result = ima_store_template(entry, violation, NULL);
	if (result < 0)
		kfree(entry);
=======
	struct integrity_iint_cache tmp_iint, *iint = &tmp_iint;
	const char *op = "add_boot_aggregate";
	const char *audit_cause = "ENOMEM";
	int result = -ENOMEM;
	int violation = 0;
	struct {
		struct ima_digest_data hdr;
		char digest[TPM_DIGEST_SIZE];
	} hash;

	memset(iint, 0, sizeof(*iint));
	memset(&hash, 0, sizeof(hash));
	iint->ima_hash = &hash.hdr;
	iint->ima_hash->algo = HASH_ALGO_SHA1;
	iint->ima_hash->length = SHA1_DIGEST_SIZE;

	if (ima_used_chip) {
		result = ima_calc_boot_aggregate(&hash.hdr);
		if (result < 0) {
			audit_cause = "hashing_error";
			goto err_out;
		}
	}

	result = ima_alloc_init_template(iint, NULL, boot_aggregate_name,
					 NULL, 0, &entry);
	if (result < 0)
		return;

	result = ima_store_template(entry, violation, NULL,
				    boot_aggregate_name);
	if (result < 0)
		ima_free_template_entry(entry);
>>>>>>> refs/remotes/origin/master
	return;
err_out:
	integrity_audit_msg(AUDIT_INTEGRITY_PCR, NULL, boot_aggregate_name, op,
			    audit_cause, result, 0);
}

int __init ima_init(void)
{
<<<<<<< HEAD
	u8 pcr_i[IMA_DIGEST_SIZE];
=======
	u8 pcr_i[TPM_DIGEST_SIZE];
>>>>>>> refs/remotes/origin/master
	int rc;

	ima_used_chip = 0;
	rc = tpm_pcr_read(TPM_ANY_NUM, 0, pcr_i);
	if (rc == 0)
		ima_used_chip = 1;

	if (!ima_used_chip)
		pr_info("IMA: No TPM chip found, activating TPM-bypass!\n");

<<<<<<< HEAD
=======
	rc = ima_init_crypto();
	if (rc)
		return rc;
	rc = ima_init_template();
	if (rc != 0)
		return rc;

>>>>>>> refs/remotes/origin/master
	ima_add_boot_aggregate();	/* boot aggregate must be first entry */
	ima_init_policy();

	return ima_fs_init();
}
<<<<<<< HEAD

void __exit ima_cleanup(void)
{
	ima_fs_cleanup();
}
=======
>>>>>>> refs/remotes/origin/master
