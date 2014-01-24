/*
 * Copyright 2003-2004, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 *
<<<<<<< HEAD
=======
 * Rewrite: Copyright (C) 2013 Linaro Ltd <ard.biesheuvel@linaro.org>
 *
>>>>>>> refs/remotes/origin/master
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/crypto.h>
#include <linux/err.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <crypto/aes.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <crypto/aes.h>
>>>>>>> refs/remotes/origin/master

#include <net/mac80211.h>
#include "key.h"
#include "aes_ccm.h"

<<<<<<< HEAD
static void aes_ccm_prepare(struct crypto_cipher *tfm, u8 *scratch, u8 *a)
{
	int i;
	u8 *b_0, *aad, *b, *s_0;

<<<<<<< HEAD
	b_0 = scratch + 3 * AES_BLOCK_LEN;
	aad = scratch + 4 * AES_BLOCK_LEN;
	b = scratch;
	s_0 = scratch + AES_BLOCK_LEN;
=======
	b_0 = scratch + 3 * AES_BLOCK_SIZE;
	aad = scratch + 4 * AES_BLOCK_SIZE;
	b = scratch;
	s_0 = scratch + AES_BLOCK_SIZE;
>>>>>>> refs/remotes/origin/cm-10.0

	crypto_cipher_encrypt_one(tfm, b, b_0);

	/* Extra Authenticate-only data (always two AES blocks) */
<<<<<<< HEAD
	for (i = 0; i < AES_BLOCK_LEN; i++)
		aad[i] ^= b[i];
	crypto_cipher_encrypt_one(tfm, b, aad);

	aad += AES_BLOCK_LEN;

	for (i = 0; i < AES_BLOCK_LEN; i++)
=======
	for (i = 0; i < AES_BLOCK_SIZE; i++)
		aad[i] ^= b[i];
	crypto_cipher_encrypt_one(tfm, b, aad);

	aad += AES_BLOCK_SIZE;

	for (i = 0; i < AES_BLOCK_SIZE; i++)
>>>>>>> refs/remotes/origin/cm-10.0
		aad[i] ^= b[i];
	crypto_cipher_encrypt_one(tfm, a, aad);

	/* Mask out bits from auth-only-b_0 */
	b_0[0] &= 0x07;

	/* S_0 is used to encrypt T (= MIC) */
	b_0[14] = 0;
	b_0[15] = 0;
	crypto_cipher_encrypt_one(tfm, s_0, b_0);
}


void ieee80211_aes_ccm_encrypt(struct crypto_cipher *tfm, u8 *scratch,
			       u8 *data, size_t data_len,
			       u8 *cdata, u8 *mic)
{
	int i, j, last_len, num_blocks;
	u8 *pos, *cpos, *b, *s_0, *e, *b_0;

	b = scratch;
<<<<<<< HEAD
	s_0 = scratch + AES_BLOCK_LEN;
	e = scratch + 2 * AES_BLOCK_LEN;
	b_0 = scratch + 3 * AES_BLOCK_LEN;

	num_blocks = DIV_ROUND_UP(data_len, AES_BLOCK_LEN);
	last_len = data_len % AES_BLOCK_LEN;
=======
	s_0 = scratch + AES_BLOCK_SIZE;
	e = scratch + 2 * AES_BLOCK_SIZE;
	b_0 = scratch + 3 * AES_BLOCK_SIZE;

	num_blocks = DIV_ROUND_UP(data_len, AES_BLOCK_SIZE);
	last_len = data_len % AES_BLOCK_SIZE;
>>>>>>> refs/remotes/origin/cm-10.0
	aes_ccm_prepare(tfm, scratch, b);

	/* Process payload blocks */
	pos = data;
	cpos = cdata;
	for (j = 1; j <= num_blocks; j++) {
		int blen = (j == num_blocks && last_len) ?
<<<<<<< HEAD
			last_len : AES_BLOCK_LEN;
=======
			last_len : AES_BLOCK_SIZE;
>>>>>>> refs/remotes/origin/cm-10.0

		/* Authentication followed by encryption */
		for (i = 0; i < blen; i++)
			b[i] ^= pos[i];
		crypto_cipher_encrypt_one(tfm, b, b);

		b_0[14] = (j >> 8) & 0xff;
		b_0[15] = j & 0xff;
		crypto_cipher_encrypt_one(tfm, e, b_0);
		for (i = 0; i < blen; i++)
			*cpos++ = *pos++ ^ e[i];
	}

	for (i = 0; i < CCMP_MIC_LEN; i++)
		mic[i] = b[i] ^ s_0[i];
}


int ieee80211_aes_ccm_decrypt(struct crypto_cipher *tfm, u8 *scratch,
			      u8 *cdata, size_t data_len, u8 *mic, u8 *data)
{
	int i, j, last_len, num_blocks;
	u8 *pos, *cpos, *b, *s_0, *a, *b_0;

	b = scratch;
<<<<<<< HEAD
	s_0 = scratch + AES_BLOCK_LEN;
	a = scratch + 2 * AES_BLOCK_LEN;
	b_0 = scratch + 3 * AES_BLOCK_LEN;

	num_blocks = DIV_ROUND_UP(data_len, AES_BLOCK_LEN);
	last_len = data_len % AES_BLOCK_LEN;
=======
	s_0 = scratch + AES_BLOCK_SIZE;
	a = scratch + 2 * AES_BLOCK_SIZE;
	b_0 = scratch + 3 * AES_BLOCK_SIZE;

	num_blocks = DIV_ROUND_UP(data_len, AES_BLOCK_SIZE);
	last_len = data_len % AES_BLOCK_SIZE;
>>>>>>> refs/remotes/origin/cm-10.0
	aes_ccm_prepare(tfm, scratch, a);

	/* Process payload blocks */
	cpos = cdata;
	pos = data;
	for (j = 1; j <= num_blocks; j++) {
		int blen = (j == num_blocks && last_len) ?
<<<<<<< HEAD
			last_len : AES_BLOCK_LEN;
=======
			last_len : AES_BLOCK_SIZE;
>>>>>>> refs/remotes/origin/cm-10.0

		/* Decryption followed by authentication */
		b_0[14] = (j >> 8) & 0xff;
		b_0[15] = j & 0xff;
		crypto_cipher_encrypt_one(tfm, b, b_0);
		for (i = 0; i < blen; i++) {
			*pos = *cpos++ ^ b[i];
			a[i] ^= *pos++;
		}
		crypto_cipher_encrypt_one(tfm, a, a);
	}

	for (i = 0; i < CCMP_MIC_LEN; i++) {
		if ((mic[i] ^ s_0[i]) != a[i])
			return -1;
	}

	return 0;
}


struct crypto_cipher *ieee80211_aes_key_setup_encrypt(const u8 key[])
{
	struct crypto_cipher *tfm;

	tfm = crypto_alloc_cipher("aes", 0, CRYPTO_ALG_ASYNC);
	if (!IS_ERR(tfm))
		crypto_cipher_setkey(tfm, key, ALG_CCMP_KEY_LEN);

	return tfm;
}


void ieee80211_aes_key_free(struct crypto_cipher *tfm)
{
	crypto_free_cipher(tfm);
=======
void ieee80211_aes_ccm_encrypt(struct crypto_aead *tfm, u8 *b_0, u8 *aad,
			       u8 *data, size_t data_len, u8 *mic)
{
	struct scatterlist assoc, pt, ct[2];
	struct {
		struct aead_request	req;
		u8			priv[crypto_aead_reqsize(tfm)];
	} aead_req;

	memset(&aead_req, 0, sizeof(aead_req));

	sg_init_one(&pt, data, data_len);
	sg_init_one(&assoc, &aad[2], be16_to_cpup((__be16 *)aad));
	sg_init_table(ct, 2);
	sg_set_buf(&ct[0], data, data_len);
	sg_set_buf(&ct[1], mic, IEEE80211_CCMP_MIC_LEN);

	aead_request_set_tfm(&aead_req.req, tfm);
	aead_request_set_assoc(&aead_req.req, &assoc, assoc.length);
	aead_request_set_crypt(&aead_req.req, &pt, ct, data_len, b_0);

	crypto_aead_encrypt(&aead_req.req);
}

int ieee80211_aes_ccm_decrypt(struct crypto_aead *tfm, u8 *b_0, u8 *aad,
			      u8 *data, size_t data_len, u8 *mic)
{
	struct scatterlist assoc, pt, ct[2];
	struct {
		struct aead_request	req;
		u8			priv[crypto_aead_reqsize(tfm)];
	} aead_req;

	memset(&aead_req, 0, sizeof(aead_req));

	sg_init_one(&pt, data, data_len);
	sg_init_one(&assoc, &aad[2], be16_to_cpup((__be16 *)aad));
	sg_init_table(ct, 2);
	sg_set_buf(&ct[0], data, data_len);
	sg_set_buf(&ct[1], mic, IEEE80211_CCMP_MIC_LEN);

	aead_request_set_tfm(&aead_req.req, tfm);
	aead_request_set_assoc(&aead_req.req, &assoc, assoc.length);
	aead_request_set_crypt(&aead_req.req, ct, &pt,
			       data_len + IEEE80211_CCMP_MIC_LEN, b_0);

	return crypto_aead_decrypt(&aead_req.req);
}

struct crypto_aead *ieee80211_aes_key_setup_encrypt(const u8 key[])
{
	struct crypto_aead *tfm;
	int err;

	tfm = crypto_alloc_aead("ccm(aes)", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(tfm))
		return tfm;

	err = crypto_aead_setkey(tfm, key, WLAN_KEY_LEN_CCMP);
	if (!err)
		err = crypto_aead_setauthsize(tfm, IEEE80211_CCMP_MIC_LEN);
	if (!err)
		return tfm;

	crypto_free_aead(tfm);
	return ERR_PTR(err);
}

void ieee80211_aes_key_free(struct crypto_aead *tfm)
{
	crypto_free_aead(tfm);
>>>>>>> refs/remotes/origin/master
}
