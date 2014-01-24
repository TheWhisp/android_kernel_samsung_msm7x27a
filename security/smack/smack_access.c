/*
 * Copyright (C) 2007 Casey Schaufler <casey@schaufler-ca.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, version 2.
 *
 * Author:
 *      Casey Schaufler <casey@schaufler-ca.com>
 *
 */

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include "smack.h"

struct smack_known smack_known_huh = {
	.smk_known	= "?",
	.smk_secid	= 2,
<<<<<<< HEAD
	.smk_cipso	= NULL,
=======
>>>>>>> refs/remotes/origin/master
};

struct smack_known smack_known_hat = {
	.smk_known	= "^",
	.smk_secid	= 3,
<<<<<<< HEAD
	.smk_cipso	= NULL,
=======
>>>>>>> refs/remotes/origin/master
};

struct smack_known smack_known_star = {
	.smk_known	= "*",
	.smk_secid	= 4,
<<<<<<< HEAD
	.smk_cipso	= NULL,
=======
>>>>>>> refs/remotes/origin/master
};

struct smack_known smack_known_floor = {
	.smk_known	= "_",
	.smk_secid	= 5,
<<<<<<< HEAD
	.smk_cipso	= NULL,
=======
>>>>>>> refs/remotes/origin/master
};

struct smack_known smack_known_invalid = {
	.smk_known	= "",
	.smk_secid	= 6,
<<<<<<< HEAD
	.smk_cipso	= NULL,
=======
>>>>>>> refs/remotes/origin/master
};

struct smack_known smack_known_web = {
	.smk_known	= "@",
	.smk_secid	= 7,
<<<<<<< HEAD
	.smk_cipso	= NULL,
=======
>>>>>>> refs/remotes/origin/master
};

LIST_HEAD(smack_known_list);

/*
 * The initial value needs to be bigger than any of the
 * known values above.
 */
static u32 smack_next_secid = 10;

/*
 * what events do we log
 * can be overwritten at run-time by /smack/logging
 */
int log_policy = SMACK_AUDIT_DENIED;

/**
 * smk_access_entry - look up matching access rule
 * @subject_label: a pointer to the subject's Smack label
 * @object_label: a pointer to the object's Smack label
 * @rule_list: the list of rules to search
 *
 * This function looks up the subject/object pair in the
 * access rule list and returns the access mode. If no
 * entry is found returns -ENOENT.
 *
 * NOTE:
<<<<<<< HEAD
<<<<<<< HEAD
 * Even though Smack labels are usually shared on smack_list
 * labels that come in off the network can't be imported
 * and added to the list for locking reasons.
 *
 * Therefore, it is necessary to check the contents of the labels,
 * not just the pointer values. Of course, in most cases the labels
 * will be on the list, so checking the pointers may be a worthwhile
 * optimization.
=======
=======
>>>>>>> refs/remotes/origin/master
 *
 * Earlier versions of this function allowed for labels that
 * were not on the label list. This was done to allow for
 * labels to come over the network that had never been seen
 * before on this host. Unless the receiving socket has the
 * star label this will always result in a failure check. The
 * star labeled socket case is now handled in the networking
 * hooks so there is no case where the label is not on the
 * label list. Checking to see if the address of two labels
 * is the same is now a reliable test.
 *
 * Do the object check first because that is more
 * likely to differ.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *
 * Allowing write access implies allowing locking.
>>>>>>> refs/remotes/origin/master
 */
int smk_access_entry(char *subject_label, char *object_label,
			struct list_head *rule_list)
{
	int may = -ENOENT;
	struct smack_rule *srp;

	list_for_each_entry_rcu(srp, rule_list, list) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (srp->smk_subject == subject_label ||
		    strcmp(srp->smk_subject, subject_label) == 0) {
			if (srp->smk_object == object_label ||
			    strcmp(srp->smk_object, object_label) == 0) {
				may = srp->smk_access;
				break;
			}
=======
		if (srp->smk_object == object_label &&
		    srp->smk_subject == subject_label) {
			may = srp->smk_access;
			break;
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}

=======
		if (srp->smk_object == object_label &&
		    srp->smk_subject->smk_known == subject_label) {
			may = srp->smk_access;
			break;
		}
	}

	/*
	 * MAY_WRITE implies MAY_LOCK.
	 */
	if ((may & MAY_WRITE) == MAY_WRITE)
		may |= MAY_LOCK;
>>>>>>> refs/remotes/origin/master
	return may;
}

/**
 * smk_access - determine if a subject has a specific access to an object
<<<<<<< HEAD
 * @subject_label: a pointer to the subject's Smack label
=======
 * @subject_known: a pointer to the subject's Smack label entry
>>>>>>> refs/remotes/origin/master
 * @object_label: a pointer to the object's Smack label
 * @request: the access requested, in "MAY" format
 * @a : a pointer to the audit data
 *
 * This function looks up the subject/object pair in the
 * access rule list and returns 0 if the access is permitted,
 * non zero otherwise.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Even though Smack labels are usually shared on smack_list
 * labels that come in off the network can't be imported
 * and added to the list for locking reasons.
 *
 * Therefore, it is necessary to check the contents of the labels,
 * not just the pointer values. Of course, in most cases the labels
 * will be on the list, so checking the pointers may be a worthwhile
 * optimization.
=======
 * Smack labels are shared on smack_list
>>>>>>> refs/remotes/origin/cm-10.0
 */
int smk_access(char *subject_label, char *object_label, int request,
	       struct smk_audit_info *a)
{
<<<<<<< HEAD
=======
	struct smack_known *skp;
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Smack labels are shared on smack_list
 */
int smk_access(struct smack_known *subject_known, char *object_label,
		int request, struct smk_audit_info *a)
{
>>>>>>> refs/remotes/origin/master
	int may = MAY_NOT;
	int rc = 0;

	/*
	 * Hardcoded comparisons.
	 *
	 * A star subject can't access any object.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (subject_label == smack_known_star.smk_known ||
	    strcmp(subject_label, smack_known_star.smk_known) == 0) {
=======
	if (subject_label == smack_known_star.smk_known) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (subject_known == &smack_known_star) {
>>>>>>> refs/remotes/origin/master
		rc = -EACCES;
		goto out_audit;
	}
	/*
	 * An internet object can be accessed by any subject.
	 * Tasks cannot be assigned the internet label.
	 * An internet subject can access any object.
	 */
	if (object_label == smack_known_web.smk_known ||
<<<<<<< HEAD
<<<<<<< HEAD
	    subject_label == smack_known_web.smk_known ||
	    strcmp(object_label, smack_known_web.smk_known) == 0 ||
	    strcmp(subject_label, smack_known_web.smk_known) == 0)
=======
	    subject_label == smack_known_web.smk_known)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    subject_known == &smack_known_web)
>>>>>>> refs/remotes/origin/master
		goto out_audit;
	/*
	 * A star object can be accessed by any subject.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (object_label == smack_known_star.smk_known ||
	    strcmp(object_label, smack_known_star.smk_known) == 0)
=======
	if (object_label == smack_known_star.smk_known)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (object_label == smack_known_star.smk_known)
>>>>>>> refs/remotes/origin/master
		goto out_audit;
	/*
	 * An object can be accessed in any way by a subject
	 * with the same label.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (subject_label == object_label ||
	    strcmp(subject_label, object_label) == 0)
=======
	if (subject_label == object_label)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (subject_known->smk_known == object_label)
>>>>>>> refs/remotes/origin/master
		goto out_audit;
	/*
	 * A hat subject can read any object.
	 * A floor object can be read by any subject.
	 */
	if ((request & MAY_ANYREAD) == request) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (object_label == smack_known_floor.smk_known ||
		    strcmp(object_label, smack_known_floor.smk_known) == 0)
			goto out_audit;
		if (subject_label == smack_known_hat.smk_known ||
		    strcmp(subject_label, smack_known_hat.smk_known) == 0)
=======
		if (object_label == smack_known_floor.smk_known)
			goto out_audit;
		if (subject_label == smack_known_hat.smk_known)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (object_label == smack_known_floor.smk_known)
			goto out_audit;
		if (subject_known == &smack_known_hat)
>>>>>>> refs/remotes/origin/master
			goto out_audit;
	}
	/*
	 * Beyond here an explicit relationship is required.
	 * If the requested access is contained in the available
	 * access (e.g. read is included in readwrite) it's
	 * good. A negative response from smk_access_entry()
	 * indicates there is no entry for this pair.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	rcu_read_lock();
	may = smk_access_entry(subject_label, object_label, &smack_rule_list);
=======
	skp = smk_find_entry(subject_label);
	rcu_read_lock();
	may = smk_access_entry(subject_label, object_label, &skp->smk_rules);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rcu_read_lock();
	may = smk_access_entry(subject_known->smk_known, object_label,
				&subject_known->smk_rules);
>>>>>>> refs/remotes/origin/master
	rcu_read_unlock();

	if (may > 0 && (request & may) == request)
		goto out_audit;

	rc = -EACCES;
out_audit:
#ifdef CONFIG_AUDIT
	if (a)
<<<<<<< HEAD
		smack_log(subject_label, object_label, request, rc, a);
=======
		smack_log(subject_known->smk_known, object_label, request,
				rc, a);
>>>>>>> refs/remotes/origin/master
#endif
	return rc;
}

/**
 * smk_curacc - determine if current has a specific access to an object
 * @obj_label: a pointer to the object's Smack label
 * @mode: the access requested, in "MAY" format
 * @a : common audit data
 *
 * This function checks the current subject label/object label pair
 * in the access rule list and returns 0 if the access is permitted,
 * non zero otherwise. It allows that current may have the capability
 * to override the rules.
 */
int smk_curacc(char *obj_label, u32 mode, struct smk_audit_info *a)
{
	struct task_smack *tsp = current_security();
<<<<<<< HEAD
	char *sp = smk_of_task(tsp);
=======
	struct smack_known *skp = smk_of_task(tsp);
>>>>>>> refs/remotes/origin/master
	int may;
	int rc;

	/*
	 * Check the global rule list
	 */
<<<<<<< HEAD
	rc = smk_access(sp, obj_label, mode, NULL);
=======
	rc = smk_access(skp, obj_label, mode, NULL);
>>>>>>> refs/remotes/origin/master
	if (rc == 0) {
		/*
		 * If there is an entry in the task's rule list
		 * it can further restrict access.
		 */
<<<<<<< HEAD
		may = smk_access_entry(sp, obj_label, &tsp->smk_rules);
=======
		may = smk_access_entry(skp->smk_known, obj_label,
					&tsp->smk_rules);
>>>>>>> refs/remotes/origin/master
		if (may < 0)
			goto out_audit;
		if ((mode & may) == mode)
			goto out_audit;
		rc = -EACCES;
	}

	/*
<<<<<<< HEAD
	 * Return if a specific label has been designated as the
	 * only one that gets privilege and current does not
	 * have that label.
	 */
	if (smack_onlycap != NULL && smack_onlycap != sp)
		goto out_audit;

	if (capable(CAP_MAC_OVERRIDE))
=======
	 * Allow for priviliged to override policy.
	 */
	if (rc != 0 && smack_privileged(CAP_MAC_OVERRIDE))
>>>>>>> refs/remotes/origin/master
		rc = 0;

out_audit:
#ifdef CONFIG_AUDIT
	if (a)
<<<<<<< HEAD
		smack_log(sp, obj_label, mode, rc, a);
=======
		smack_log(skp->smk_known, obj_label, mode, rc, a);
>>>>>>> refs/remotes/origin/master
#endif
	return rc;
}

#ifdef CONFIG_AUDIT
/**
 * smack_str_from_perm : helper to transalate an int to a
 * readable string
 * @string : the string to fill
 * @access : the int
 *
 */
static inline void smack_str_from_perm(char *string, int access)
{
	int i = 0;
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	if (access & MAY_READ)
		string[i++] = 'r';
	if (access & MAY_WRITE)
		string[i++] = 'w';
	if (access & MAY_EXEC)
		string[i++] = 'x';
	if (access & MAY_APPEND)
		string[i++] = 'a';
<<<<<<< HEAD
=======
	if (access & MAY_TRANSMUTE)
		string[i++] = 't';
	if (access & MAY_LOCK)
		string[i++] = 'l';
>>>>>>> refs/remotes/origin/master
	string[i] = '\0';
}
/**
 * smack_log_callback - SMACK specific information
 * will be called by generic audit code
 * @ab : the audit_buffer
 * @a  : audit_data
 *
 */
static void smack_log_callback(struct audit_buffer *ab, void *a)
{
	struct common_audit_data *ad = a;
<<<<<<< HEAD
<<<<<<< HEAD
	struct smack_audit_data *sad = &ad->smack_audit_data;
	audit_log_format(ab, "lsm=SMACK fn=%s action=%s",
			 ad->smack_audit_data.function,
=======
	struct smack_audit_data *sad = ad->smack_audit_data;
	audit_log_format(ab, "lsm=SMACK fn=%s action=%s",
			 ad->smack_audit_data->function,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct smack_audit_data *sad = ad->smack_audit_data;
	audit_log_format(ab, "lsm=SMACK fn=%s action=%s",
			 ad->smack_audit_data->function,
>>>>>>> refs/remotes/origin/master
			 sad->result ? "denied" : "granted");
	audit_log_format(ab, " subject=");
	audit_log_untrustedstring(ab, sad->subject);
	audit_log_format(ab, " object=");
	audit_log_untrustedstring(ab, sad->object);
	audit_log_format(ab, " requested=%s", sad->request);
}

/**
 *  smack_log - Audit the granting or denial of permissions.
 *  @subject_label : smack label of the requester
 *  @object_label  : smack label of the object being accessed
 *  @request: requested permissions
 *  @result: result from smk_access
 *  @a:  auxiliary audit data
 *
 * Audit the granting or denial of permissions in accordance
 * with the policy.
 */
void smack_log(char *subject_label, char *object_label, int request,
	       int result, struct smk_audit_info *ad)
{
	char request_buffer[SMK_NUM_ACCESS_TYPE + 1];
	struct smack_audit_data *sad;
	struct common_audit_data *a = &ad->a;

	/* check if we have to log the current event */
	if (result != 0 && (log_policy & SMACK_AUDIT_DENIED) == 0)
		return;
	if (result == 0 && (log_policy & SMACK_AUDIT_ACCEPT) == 0)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	if (a->smack_audit_data.function == NULL)
		a->smack_audit_data.function = "unknown";

	/* end preparing the audit data */
	sad = &a->smack_audit_data;
=======
=======
>>>>>>> refs/remotes/origin/master
	sad = a->smack_audit_data;

	if (sad->function == NULL)
		sad->function = "unknown";

	/* end preparing the audit data */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	smack_str_from_perm(request_buffer, request);
	sad->subject = subject_label;
	sad->object  = object_label;
	sad->request = request_buffer;
	sad->result  = result;
<<<<<<< HEAD
<<<<<<< HEAD
	a->lsm_pre_audit = smack_log_callback;

	common_lsm_audit(a);
=======

	common_lsm_audit(a, smack_log_callback, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	common_lsm_audit(a, smack_log_callback, NULL);
>>>>>>> refs/remotes/origin/master
}
#else /* #ifdef CONFIG_AUDIT */
void smack_log(char *subject_label, char *object_label, int request,
               int result, struct smk_audit_info *ad)
{
}
#endif

<<<<<<< HEAD
static DEFINE_MUTEX(smack_known_lock);

/**
<<<<<<< HEAD
 * smk_import_entry - import a label, return the list entry
 * @string: a text string that might be a Smack label
 * @len: the maximum size, or zero if it is NULL terminated.
 *
 * Returns a pointer to the entry in the label list that
 * matches the passed string, adding it if necessary.
 */
struct smack_known *smk_import_entry(const char *string, int len)
{
	struct smack_known *skp;
	char smack[SMK_LABELLEN];
=======
=======
DEFINE_MUTEX(smack_known_lock);

struct hlist_head smack_known_hash[SMACK_HASH_SLOTS];

/**
 * smk_insert_entry - insert a smack label into a hash map,
 *
 * this function must be called under smack_known_lock
 */
void smk_insert_entry(struct smack_known *skp)
{
	unsigned int hash;
	struct hlist_head *head;

	hash = full_name_hash(skp->smk_known, strlen(skp->smk_known));
	head = &smack_known_hash[hash & (SMACK_HASH_SLOTS - 1)];

	hlist_add_head_rcu(&skp->smk_hashed, head);
	list_add_rcu(&skp->list, &smack_known_list);
}

/**
>>>>>>> refs/remotes/origin/master
 * smk_find_entry - find a label on the list, return the list entry
 * @string: a text string that might be a Smack label
 *
 * Returns a pointer to the entry in the label list that
 * matches the passed string.
 */
struct smack_known *smk_find_entry(const char *string)
{
<<<<<<< HEAD
	struct smack_known *skp;

	list_for_each_entry_rcu(skp, &smack_known_list, list) {
		if (strncmp(skp->smk_known, string, SMK_MAXLEN) == 0)
			return skp;
	}
=======
	unsigned int hash;
	struct hlist_head *head;
	struct smack_known *skp;

	hash = full_name_hash(string, strlen(string));
	head = &smack_known_hash[hash & (SMACK_HASH_SLOTS - 1)];

	hlist_for_each_entry_rcu(skp, head, smk_hashed)
		if (strcmp(skp->smk_known, string) == 0)
			return skp;
>>>>>>> refs/remotes/origin/master

	return NULL;
}

/**
 * smk_parse_smack - parse smack label from a text string
 * @string: a text string that might contain a Smack label
 * @len: the maximum size, or zero if it is NULL terminated.
<<<<<<< HEAD
 * @smack: parsed smack label, or NULL if parse error
 */
void smk_parse_smack(const char *string, int len, char *smack)
{
>>>>>>> refs/remotes/origin/cm-10.0
	int found;
	int i;

	if (len <= 0 || len > SMK_MAXLEN)
		len = SMK_MAXLEN;

	for (i = 0, found = 0; i < SMK_LABELLEN; i++) {
		if (found)
			smack[i] = '\0';
		else if (i >= len || string[i] > '~' || string[i] <= ' ' ||
			 string[i] == '/' || string[i] == '"' ||
			 string[i] == '\\' || string[i] == '\'') {
			smack[i] = '\0';
			found = 1;
		} else
			smack[i] = string[i];
	}
<<<<<<< HEAD

=======
=======
 *
 * Returns a pointer to the clean label, or NULL
 */
char *smk_parse_smack(const char *string, int len)
{
	char *smack;
	int i;

	if (len <= 0)
		len = strlen(string) + 1;

	/*
	 * Reserve a leading '-' as an indicator that
	 * this isn't a label, but an option to interfaces
	 * including /smack/cipso and /smack/cipso2
	 */
	if (string[0] == '-')
		return NULL;

	for (i = 0; i < len; i++)
		if (string[i] > '~' || string[i] <= ' ' || string[i] == '/' ||
		    string[i] == '"' || string[i] == '\\' || string[i] == '\'')
			break;

	if (i == 0 || i >= SMK_LONGLABEL)
		return NULL;

	smack = kzalloc(i + 1, GFP_KERNEL);
	if (smack != NULL) {
		strncpy(smack, string, i + 1);
		smack[i] = '\0';
	}
	return smack;
}

/**
 * smk_netlbl_mls - convert a catset to netlabel mls categories
 * @catset: the Smack categories
 * @sap: where to put the netlabel categories
 *
 * Allocates and fills attr.mls
 * Returns 0 on success, error code on failure.
 */
int smk_netlbl_mls(int level, char *catset, struct netlbl_lsm_secattr *sap,
			int len)
{
	unsigned char *cp;
	unsigned char m;
	int cat;
	int rc;
	int byte;

	sap->flags |= NETLBL_SECATTR_MLS_CAT;
	sap->attr.mls.lvl = level;
	sap->attr.mls.cat = netlbl_secattr_catmap_alloc(GFP_ATOMIC);
	if (!sap->attr.mls.cat)
		return -ENOMEM;
	sap->attr.mls.cat->startbit = 0;

	for (cat = 1, cp = catset, byte = 0; byte < len; cp++, byte++)
		for (m = 0x80; m != 0; m >>= 1, cat++) {
			if ((m & *cp) == 0)
				continue;
			rc = netlbl_secattr_catmap_setbit(sap->attr.mls.cat,
							  cat, GFP_ATOMIC);
			if (rc < 0) {
				netlbl_secattr_catmap_free(sap->attr.mls.cat);
				return rc;
			}
		}

	return 0;
>>>>>>> refs/remotes/origin/master
}

/**
 * smk_import_entry - import a label, return the list entry
 * @string: a text string that might be a Smack label
 * @len: the maximum size, or zero if it is NULL terminated.
 *
 * Returns a pointer to the entry in the label list that
 * matches the passed string, adding it if necessary.
 */
struct smack_known *smk_import_entry(const char *string, int len)
{
	struct smack_known *skp;
<<<<<<< HEAD
	char smack[SMK_LABELLEN];

	smk_parse_smack(string, len, smack);
>>>>>>> refs/remotes/origin/cm-10.0
	if (smack[0] == '\0')
=======
	char *smack;
	int slen;
	int rc;

	smack = smk_parse_smack(string, len);
	if (smack == NULL)
>>>>>>> refs/remotes/origin/master
		return NULL;

	mutex_lock(&smack_known_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	found = 0;
	list_for_each_entry_rcu(skp, &smack_known_list, list) {
		if (strncmp(skp->smk_known, smack, SMK_MAXLEN) == 0) {
			found = 1;
			break;
		}
	}

	if (found == 0) {
=======
	skp = smk_find_entry(smack);

	if (skp == NULL) {
>>>>>>> refs/remotes/origin/cm-10.0
		skp = kzalloc(sizeof(struct smack_known), GFP_KERNEL);
		if (skp != NULL) {
			strncpy(skp->smk_known, smack, SMK_MAXLEN);
			skp->smk_secid = smack_next_secid++;
			skp->smk_cipso = NULL;
<<<<<<< HEAD
			spin_lock_init(&skp->smk_cipsolock);
=======
			INIT_LIST_HEAD(&skp->smk_rules);
			spin_lock_init(&skp->smk_cipsolock);
			mutex_init(&skp->smk_rules_lock);
>>>>>>> refs/remotes/origin/cm-10.0
			/*
			 * Make sure that the entry is actually
			 * filled before putting it on the list.
			 */
			list_add_rcu(&skp->list, &smack_known_list);
		}
	}

=======
	skp = smk_find_entry(smack);
	if (skp != NULL)
		goto freeout;

	skp = kzalloc(sizeof(*skp), GFP_KERNEL);
	if (skp == NULL)
		goto freeout;

	skp->smk_known = smack;
	skp->smk_secid = smack_next_secid++;
	skp->smk_netlabel.domain = skp->smk_known;
	skp->smk_netlabel.flags =
		NETLBL_SECATTR_DOMAIN | NETLBL_SECATTR_MLS_LVL;
	/*
	 * If direct labeling works use it.
	 * Otherwise use mapped labeling.
	 */
	slen = strlen(smack);
	if (slen < SMK_CIPSOLEN)
		rc = smk_netlbl_mls(smack_cipso_direct, skp->smk_known,
			       &skp->smk_netlabel, slen);
	else
		rc = smk_netlbl_mls(smack_cipso_mapped, (char *)&skp->smk_secid,
			       &skp->smk_netlabel, sizeof(skp->smk_secid));

	if (rc >= 0) {
		INIT_LIST_HEAD(&skp->smk_rules);
		mutex_init(&skp->smk_rules_lock);
		/*
		 * Make sure that the entry is actually
		 * filled before putting it on the list.
		 */
		smk_insert_entry(skp);
		goto unlockout;
	}
	/*
	 * smk_netlbl_mls failed.
	 */
	kfree(skp);
	skp = NULL;
freeout:
	kfree(smack);
unlockout:
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&smack_known_lock);

	return skp;
}

/**
 * smk_import - import a smack label
 * @string: a text string that might be a Smack label
 * @len: the maximum size, or zero if it is NULL terminated.
 *
 * Returns a pointer to the label in the label list that
 * matches the passed string, adding it if necessary.
 */
char *smk_import(const char *string, int len)
{
	struct smack_known *skp;

	/* labels cannot begin with a '-' */
	if (string[0] == '-')
		return NULL;
	skp = smk_import_entry(string, len);
	if (skp == NULL)
		return NULL;
	return skp->smk_known;
}

/**
 * smack_from_secid - find the Smack label associated with a secid
 * @secid: an integer that might be associated with a Smack label
 *
<<<<<<< HEAD
 * Returns a pointer to the appropriate Smack label if there is one,
 * otherwise a pointer to the invalid Smack label.
 */
char *smack_from_secid(const u32 secid)
=======
 * Returns a pointer to the appropriate Smack label entry if there is one,
 * otherwise a pointer to the invalid Smack label.
 */
struct smack_known *smack_from_secid(const u32 secid)
>>>>>>> refs/remotes/origin/master
{
	struct smack_known *skp;

	rcu_read_lock();
	list_for_each_entry_rcu(skp, &smack_known_list, list) {
		if (skp->smk_secid == secid) {
			rcu_read_unlock();
<<<<<<< HEAD
			return skp->smk_known;
=======
			return skp;
>>>>>>> refs/remotes/origin/master
		}
	}

	/*
	 * If we got this far someone asked for the translation
	 * of a secid that is not on the list.
	 */
	rcu_read_unlock();
<<<<<<< HEAD
	return smack_known_invalid.smk_known;
=======
	return &smack_known_invalid;
>>>>>>> refs/remotes/origin/master
}

/**
 * smack_to_secid - find the secid associated with a Smack label
 * @smack: the Smack label
 *
 * Returns the appropriate secid if there is one,
 * otherwise 0
 */
u32 smack_to_secid(const char *smack)
{
<<<<<<< HEAD
	struct smack_known *skp;

	rcu_read_lock();
	list_for_each_entry_rcu(skp, &smack_known_list, list) {
		if (strncmp(skp->smk_known, smack, SMK_MAXLEN) == 0) {
			rcu_read_unlock();
			return skp->smk_secid;
		}
	}
	rcu_read_unlock();
	return 0;
}

/**
 * smack_from_cipso - find the Smack label associated with a CIPSO option
 * @level: Bell & LaPadula level from the network
 * @cp: Bell & LaPadula categories from the network
<<<<<<< HEAD
 * @result: where to put the Smack value
 *
 * This is a simple lookup in the label table.
 *
 * This is an odd duck as far as smack handling goes in that
 * it sends back a copy of the smack label rather than a pointer
 * to the master list. This is done because it is possible for
 * a foreign host to send a smack label that is new to this
 * machine and hence not on the list. That would not be an
 * issue except that adding an entry to the master list can't
 * be done at that point.
 */
void smack_from_cipso(u32 level, char *cp, char *result)
=======
 *
 * This is a simple lookup in the label table.
 *
 * Return the matching label from the label list or NULL.
 */
char *smack_from_cipso(u32 level, char *cp)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct smack_known *kp;
	char *final = NULL;

	rcu_read_lock();
	list_for_each_entry(kp, &smack_known_list, list) {
		if (kp->smk_cipso == NULL)
			continue;

		spin_lock_bh(&kp->smk_cipsolock);

		if (kp->smk_cipso->smk_level == level &&
		    memcmp(kp->smk_cipso->smk_catset, cp, SMK_LABELLEN) == 0)
			final = kp->smk_known;

		spin_unlock_bh(&kp->smk_cipsolock);
<<<<<<< HEAD
	}
	rcu_read_unlock();
	if (final == NULL)
		final = smack_known_huh.smk_known;
	strncpy(result, final, SMK_MAXLEN);
	return;
=======

		if (final != NULL)
			break;
	}
	rcu_read_unlock();

	return final;
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * smack_to_cipso - find the CIPSO option to go with a Smack label
 * @smack: a pointer to the smack label in question
 * @cp: where to put the result
 *
 * Returns zero if a value is available, non-zero otherwise.
 */
int smack_to_cipso(const char *smack, struct smack_cipso *cp)
{
	struct smack_known *kp;
	int found = 0;

	rcu_read_lock();
	list_for_each_entry_rcu(kp, &smack_known_list, list) {
		if (kp->smk_known == smack ||
		    strcmp(kp->smk_known, smack) == 0) {
			found = 1;
			break;
		}
	}
	rcu_read_unlock();

	if (found == 0 || kp->smk_cipso == NULL)
		return -ENOENT;

	memcpy(cp, kp->smk_cipso, sizeof(struct smack_cipso));
	return 0;
=======
	struct smack_known *skp = smk_find_entry(smack);

	if (skp == NULL)
		return 0;
	return skp->smk_secid;
>>>>>>> refs/remotes/origin/master
}
