/*
 * security/tomoyo/domain.c
 *
<<<<<<< HEAD
 * Domain transition functions for TOMOYO.
 *
 * Copyright (C) 2005-2010  NTT DATA CORPORATION
=======
 * Copyright (C) 2005-2011  NTT DATA CORPORATION
>>>>>>> refs/remotes/origin/cm-10.0
 */

#include "common.h"
#include <linux/binfmts.h>
#include <linux/slab.h>

/* Variables definitions.*/

/* The initial domain. */
struct tomoyo_domain_info tomoyo_kernel_domain;

/**
 * tomoyo_update_policy - Update an entry for exception policy.
 *
 * @new_entry:       Pointer to "struct tomoyo_acl_info".
 * @size:            Size of @new_entry in bytes.
<<<<<<< HEAD
 * @is_delete:       True if it is a delete request.
 * @list:            Pointer to "struct list_head".
=======
 * @param:           Pointer to "struct tomoyo_acl_param".
>>>>>>> refs/remotes/origin/cm-10.0
 * @check_duplicate: Callback function to find duplicated entry.
 *
 * Returns 0 on success, negative value otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
int tomoyo_update_policy(struct tomoyo_acl_head *new_entry, const int size,
<<<<<<< HEAD
			 bool is_delete, struct list_head *list,
=======
			 struct tomoyo_acl_param *param,
>>>>>>> refs/remotes/origin/cm-10.0
			 bool (*check_duplicate) (const struct tomoyo_acl_head
						  *,
						  const struct tomoyo_acl_head
						  *))
{
<<<<<<< HEAD
	int error = is_delete ? -ENOENT : -ENOMEM;
	struct tomoyo_acl_head *entry;
=======
	int error = param->is_delete ? -ENOENT : -ENOMEM;
	struct tomoyo_acl_head *entry;
	struct list_head *list = param->list;
>>>>>>> refs/remotes/origin/cm-10.0

	if (mutex_lock_interruptible(&tomoyo_policy_lock))
		return -ENOMEM;
	list_for_each_entry_rcu(entry, list, list) {
<<<<<<< HEAD
		if (!check_duplicate(entry, new_entry))
			continue;
		entry->is_deleted = is_delete;
		error = 0;
		break;
	}
	if (error && !is_delete) {
=======
		if (entry->is_deleted == TOMOYO_GC_IN_PROGRESS)
			continue;
		if (!check_duplicate(entry, new_entry))
			continue;
		entry->is_deleted = param->is_delete;
		error = 0;
		break;
	}
	if (error && !param->is_delete) {
>>>>>>> refs/remotes/origin/cm-10.0
		entry = tomoyo_commit_ok(new_entry, size);
		if (entry) {
			list_add_tail_rcu(&entry->list, list);
			error = 0;
		}
	}
	mutex_unlock(&tomoyo_policy_lock);
	return error;
}

/**
<<<<<<< HEAD
=======
 * tomoyo_same_acl_head - Check for duplicated "struct tomoyo_acl_info" entry.
 *
 * @a: Pointer to "struct tomoyo_acl_info".
 * @b: Pointer to "struct tomoyo_acl_info".
 *
 * Returns true if @a == @b, false otherwise.
 */
static inline bool tomoyo_same_acl_head(const struct tomoyo_acl_info *a,
					const struct tomoyo_acl_info *b)
{
	return a->type == b->type && a->cond == b->cond;
}

/**
>>>>>>> refs/remotes/origin/cm-10.0
 * tomoyo_update_domain - Update an entry for domain policy.
 *
 * @new_entry:       Pointer to "struct tomoyo_acl_info".
 * @size:            Size of @new_entry in bytes.
<<<<<<< HEAD
 * @is_delete:       True if it is a delete request.
 * @domain:          Pointer to "struct tomoyo_domain_info".
=======
 * @param:           Pointer to "struct tomoyo_acl_param".
>>>>>>> refs/remotes/origin/cm-10.0
 * @check_duplicate: Callback function to find duplicated entry.
 * @merge_duplicate: Callback function to merge duplicated entry.
 *
 * Returns 0 on success, negative value otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
int tomoyo_update_domain(struct tomoyo_acl_info *new_entry, const int size,
<<<<<<< HEAD
			 bool is_delete, struct tomoyo_domain_info *domain,
=======
			 struct tomoyo_acl_param *param,
>>>>>>> refs/remotes/origin/cm-10.0
			 bool (*check_duplicate) (const struct tomoyo_acl_info
						  *,
						  const struct tomoyo_acl_info
						  *),
			 bool (*merge_duplicate) (struct tomoyo_acl_info *,
						  struct tomoyo_acl_info *,
						  const bool))
{
<<<<<<< HEAD
	int error = is_delete ? -ENOENT : -ENOMEM;
	struct tomoyo_acl_info *entry;

	if (mutex_lock_interruptible(&tomoyo_policy_lock))
		return error;
	list_for_each_entry_rcu(entry, &domain->acl_info_list, list) {
		if (!check_duplicate(entry, new_entry))
=======
	const bool is_delete = param->is_delete;
	int error = is_delete ? -ENOENT : -ENOMEM;
	struct tomoyo_acl_info *entry;
	struct list_head * const list = param->list;

	if (param->data[0]) {
		new_entry->cond = tomoyo_get_condition(param);
		if (!new_entry->cond)
			return -EINVAL;
		/*
		 * Domain transition preference is allowed for only
		 * "file execute" entries.
		 */
		if (new_entry->cond->transit &&
		    !(new_entry->type == TOMOYO_TYPE_PATH_ACL &&
		      container_of(new_entry, struct tomoyo_path_acl, head)
		      ->perm == 1 << TOMOYO_TYPE_EXECUTE))
			goto out;
	}
	if (mutex_lock_interruptible(&tomoyo_policy_lock))
		goto out;
	list_for_each_entry_rcu(entry, list, list) {
		if (entry->is_deleted == TOMOYO_GC_IN_PROGRESS)
			continue;
		if (!tomoyo_same_acl_head(entry, new_entry) ||
		    !check_duplicate(entry, new_entry))
>>>>>>> refs/remotes/origin/cm-10.0
			continue;
		if (merge_duplicate)
			entry->is_deleted = merge_duplicate(entry, new_entry,
							    is_delete);
		else
			entry->is_deleted = is_delete;
		error = 0;
		break;
	}
	if (error && !is_delete) {
		entry = tomoyo_commit_ok(new_entry, size);
		if (entry) {
<<<<<<< HEAD
			list_add_tail_rcu(&entry->list, &domain->acl_info_list);
=======
			list_add_tail_rcu(&entry->list, list);
>>>>>>> refs/remotes/origin/cm-10.0
			error = 0;
		}
	}
	mutex_unlock(&tomoyo_policy_lock);
<<<<<<< HEAD
	return error;
}

=======
out:
	tomoyo_put_condition(new_entry->cond);
	return error;
}

/**
 * tomoyo_check_acl - Do permission check.
 *
 * @r:           Pointer to "struct tomoyo_request_info".
 * @check_entry: Callback function to check type specific parameters.
 *
 * Returns 0 on success, negative value otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
>>>>>>> refs/remotes/origin/cm-10.0
void tomoyo_check_acl(struct tomoyo_request_info *r,
		      bool (*check_entry) (struct tomoyo_request_info *,
					   const struct tomoyo_acl_info *))
{
	const struct tomoyo_domain_info *domain = r->domain;
	struct tomoyo_acl_info *ptr;
<<<<<<< HEAD

	list_for_each_entry_rcu(ptr, &domain->acl_info_list, list) {
		if (ptr->is_deleted || ptr->type != r->param_type)
			continue;
		if (check_entry(r, ptr)) {
			r->granted = true;
			return;
		}
=======
	bool retried = false;
	const struct list_head *list = &domain->acl_info_list;

retry:
	list_for_each_entry_rcu(ptr, list, list) {
		if (ptr->is_deleted || ptr->type != r->param_type)
			continue;
		if (!check_entry(r, ptr))
			continue;
		if (!tomoyo_condition(r, ptr->cond))
			continue;
		r->matched_acl = ptr;
		r->granted = true;
		return;
	}
	if (!retried) {
		retried = true;
		list = &domain->ns->acl_group[domain->group];
		goto retry;
>>>>>>> refs/remotes/origin/cm-10.0
	}
	r->granted = false;
}

/* The list for "struct tomoyo_domain_info". */
LIST_HEAD(tomoyo_domain_list);

<<<<<<< HEAD
struct list_head tomoyo_policy_list[TOMOYO_MAX_POLICY];
struct list_head tomoyo_group_list[TOMOYO_MAX_GROUP];

/**
 * tomoyo_last_word - Get last component of a domainname.
 *
 * @domainname: Domainname to check.
=======
/**
 * tomoyo_last_word - Get last component of a domainname.
 *
 * @name: Domainname to check.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Returns the last word of @domainname.
 */
static const char *tomoyo_last_word(const char *name)
{
<<<<<<< HEAD
        const char *cp = strrchr(name, ' ');
        if (cp)
                return cp + 1;
        return name;
}

=======
	const char *cp = strrchr(name, ' ');
	if (cp)
		return cp + 1;
	return name;
}

/**
 * tomoyo_same_transition_control - Check for duplicated "struct tomoyo_transition_control" entry.
 *
 * @a: Pointer to "struct tomoyo_acl_head".
 * @b: Pointer to "struct tomoyo_acl_head".
 *
 * Returns true if @a == @b, false otherwise.
 */
>>>>>>> refs/remotes/origin/cm-10.0
static bool tomoyo_same_transition_control(const struct tomoyo_acl_head *a,
					   const struct tomoyo_acl_head *b)
{
	const struct tomoyo_transition_control *p1 = container_of(a,
								  typeof(*p1),
								  head);
	const struct tomoyo_transition_control *p2 = container_of(b,
								  typeof(*p2),
								  head);
	return p1->type == p2->type && p1->is_last_name == p2->is_last_name
		&& p1->domainname == p2->domainname
		&& p1->program == p2->program;
}

/**
<<<<<<< HEAD
 * tomoyo_update_transition_control_entry - Update "struct tomoyo_transition_control" list.
 *
 * @domainname: The name of domain. Maybe NULL.
 * @program:    The name of program. Maybe NULL.
 * @type:       Type of transition.
 * @is_delete:  True if it is a delete request.
 *
 * Returns 0 on success, negative value otherwise.
 */
static int tomoyo_update_transition_control_entry(const char *domainname,
						  const char *program,
						  const u8 type,
						  const bool is_delete)
{
	struct tomoyo_transition_control e = { .type = type };
	int error = is_delete ? -ENOENT : -ENOMEM;
	if (program) {
=======
 * tomoyo_write_transition_control - Write "struct tomoyo_transition_control" list.
 *
 * @param: Pointer to "struct tomoyo_acl_param".
 * @type:  Type of this entry.
 *
 * Returns 0 on success, negative value otherwise.
 */
int tomoyo_write_transition_control(struct tomoyo_acl_param *param,
				    const u8 type)
{
	struct tomoyo_transition_control e = { .type = type };
	int error = param->is_delete ? -ENOENT : -ENOMEM;
	char *program = param->data;
	char *domainname = strstr(program, " from ");
	if (domainname) {
		*domainname = '\0';
		domainname += 6;
	} else if (type == TOMOYO_TRANSITION_CONTROL_NO_KEEP ||
		   type == TOMOYO_TRANSITION_CONTROL_KEEP) {
		domainname = program;
		program = NULL;
	}
	if (program && strcmp(program, "any")) {
>>>>>>> refs/remotes/origin/cm-10.0
		if (!tomoyo_correct_path(program))
			return -EINVAL;
		e.program = tomoyo_get_name(program);
		if (!e.program)
			goto out;
	}
<<<<<<< HEAD
	if (domainname) {
=======
	if (domainname && strcmp(domainname, "any")) {
>>>>>>> refs/remotes/origin/cm-10.0
		if (!tomoyo_correct_domain(domainname)) {
			if (!tomoyo_correct_path(domainname))
				goto out;
			e.is_last_name = true;
		}
		e.domainname = tomoyo_get_name(domainname);
		if (!e.domainname)
			goto out;
	}
<<<<<<< HEAD
	error = tomoyo_update_policy(&e.head, sizeof(e), is_delete,
				     &tomoyo_policy_list
				     [TOMOYO_ID_TRANSITION_CONTROL],
				     tomoyo_same_transition_control);
 out:
=======
	param->list = &param->ns->policy_list[TOMOYO_ID_TRANSITION_CONTROL];
	error = tomoyo_update_policy(&e.head, sizeof(e), param,
				     tomoyo_same_transition_control);
out:
>>>>>>> refs/remotes/origin/cm-10.0
	tomoyo_put_name(e.domainname);
	tomoyo_put_name(e.program);
	return error;
}

/**
<<<<<<< HEAD
 * tomoyo_write_transition_control - Write "struct tomoyo_transition_control" list.
 *
 * @data:      String to parse.
 * @is_delete: True if it is a delete request.
 * @type:      Type of this entry.
 *
 * Returns 0 on success, negative value otherwise.
 */
int tomoyo_write_transition_control(char *data, const bool is_delete,
				    const u8 type)
{
	char *domainname = strstr(data, " from ");
	if (domainname) {
		*domainname = '\0';
		domainname += 6;
	} else if (type == TOMOYO_TRANSITION_CONTROL_NO_KEEP ||
		   type == TOMOYO_TRANSITION_CONTROL_KEEP) {
		domainname = data;
		data = NULL;
	}
	return tomoyo_update_transition_control_entry(domainname, data, type,
						      is_delete);
=======
 * tomoyo_scan_transition - Try to find specific domain transition type.
 *
 * @list:       Pointer to "struct list_head".
 * @domainname: The name of current domain.
 * @program:    The name of requested program.
 * @last_name:  The last component of @domainname.
 * @type:       One of values in "enum tomoyo_transition_type".
 *
 * Returns true if found one, false otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
static inline bool tomoyo_scan_transition
(const struct list_head *list, const struct tomoyo_path_info *domainname,
 const struct tomoyo_path_info *program, const char *last_name,
 const enum tomoyo_transition_type type)
{
	const struct tomoyo_transition_control *ptr;
	list_for_each_entry_rcu(ptr, list, head.list) {
		if (ptr->head.is_deleted || ptr->type != type)
			continue;
		if (ptr->domainname) {
			if (!ptr->is_last_name) {
				if (ptr->domainname != domainname)
					continue;
			} else {
				/*
				 * Use direct strcmp() since this is
				 * unlikely used.
				 */
				if (strcmp(ptr->domainname->name, last_name))
					continue;
			}
		}
		if (ptr->program && tomoyo_pathcmp(ptr->program, program))
			continue;
		return true;
	}
	return false;
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * tomoyo_transition_type - Get domain transition type.
 *
<<<<<<< HEAD
 * @domainname: The name of domain.
 * @program:    The name of program.
 *
 * Returns TOMOYO_TRANSITION_CONTROL_INITIALIZE if executing @program
 * reinitializes domain transition, TOMOYO_TRANSITION_CONTROL_KEEP if executing
 * @program suppresses domain transition, others otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
static u8 tomoyo_transition_type(const struct tomoyo_path_info *domainname,
				 const struct tomoyo_path_info *program)
{
	const struct tomoyo_transition_control *ptr;
	const char *last_name = tomoyo_last_word(domainname->name);
	u8 type;
	for (type = 0; type < TOMOYO_MAX_TRANSITION_TYPE; type++) {
 next:
		list_for_each_entry_rcu(ptr, &tomoyo_policy_list
					[TOMOYO_ID_TRANSITION_CONTROL],
					head.list) {
			if (ptr->head.is_deleted || ptr->type != type)
				continue;
			if (ptr->domainname) {
				if (!ptr->is_last_name) {
					if (ptr->domainname != domainname)
						continue;
				} else {
					/*
					 * Use direct strcmp() since this is
					 * unlikely used.
					 */
					if (strcmp(ptr->domainname->name,
						   last_name))
						continue;
				}
			}
			if (ptr->program &&
			    tomoyo_pathcmp(ptr->program, program))
				continue;
			if (type == TOMOYO_TRANSITION_CONTROL_NO_INITIALIZE) {
				/*
				 * Do not check for initialize_domain if
				 * no_initialize_domain matched.
				 */
				type = TOMOYO_TRANSITION_CONTROL_NO_KEEP;
				goto next;
			}
			goto done;
		}
	}
 done:
	return type;
}

static bool tomoyo_same_aggregator(const struct tomoyo_acl_head *a,
				   const struct tomoyo_acl_head *b)
{
	const struct tomoyo_aggregator *p1 = container_of(a, typeof(*p1), head);
	const struct tomoyo_aggregator *p2 = container_of(b, typeof(*p2), head);
=======
 * @ns:         Pointer to "struct tomoyo_policy_namespace".
 * @domainname: The name of current domain.
 * @program:    The name of requested program.
 *
 * Returns TOMOYO_TRANSITION_CONTROL_TRANSIT if executing @program causes
 * domain transition across namespaces, TOMOYO_TRANSITION_CONTROL_INITIALIZE if
 * executing @program reinitializes domain transition within that namespace,
 * TOMOYO_TRANSITION_CONTROL_KEEP if executing @program stays at @domainname ,
 * others otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
static enum tomoyo_transition_type tomoyo_transition_type
(const struct tomoyo_policy_namespace *ns,
 const struct tomoyo_path_info *domainname,
 const struct tomoyo_path_info *program)
{
	const char *last_name = tomoyo_last_word(domainname->name);
	enum tomoyo_transition_type type = TOMOYO_TRANSITION_CONTROL_NO_RESET;
	while (type < TOMOYO_MAX_TRANSITION_TYPE) {
		const struct list_head * const list =
			&ns->policy_list[TOMOYO_ID_TRANSITION_CONTROL];
		if (!tomoyo_scan_transition(list, domainname, program,
					    last_name, type)) {
			type++;
			continue;
		}
		if (type != TOMOYO_TRANSITION_CONTROL_NO_RESET &&
		    type != TOMOYO_TRANSITION_CONTROL_NO_INITIALIZE)
			break;
		/*
		 * Do not check for reset_domain if no_reset_domain matched.
		 * Do not check for initialize_domain if no_initialize_domain
		 * matched.
		 */
		type++;
		type++;
	}
	return type;
}

/**
 * tomoyo_same_aggregator - Check for duplicated "struct tomoyo_aggregator" entry.
 *
 * @a: Pointer to "struct tomoyo_acl_head".
 * @b: Pointer to "struct tomoyo_acl_head".
 *
 * Returns true if @a == @b, false otherwise.
 */
static bool tomoyo_same_aggregator(const struct tomoyo_acl_head *a,
				   const struct tomoyo_acl_head *b)
{
	const struct tomoyo_aggregator *p1 = container_of(a, typeof(*p1),
							  head);
	const struct tomoyo_aggregator *p2 = container_of(b, typeof(*p2),
							  head);
>>>>>>> refs/remotes/origin/cm-10.0
	return p1->original_name == p2->original_name &&
		p1->aggregated_name == p2->aggregated_name;
}

/**
<<<<<<< HEAD
 * tomoyo_update_aggregator_entry - Update "struct tomoyo_aggregator" list.
 *
 * @original_name:   The original program's name.
 * @aggregated_name: The program name to use.
 * @is_delete:       True if it is a delete request.
=======
 * tomoyo_write_aggregator - Write "struct tomoyo_aggregator" list.
 *
 * @param: Pointer to "struct tomoyo_acl_param".
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Returns 0 on success, negative value otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
<<<<<<< HEAD
static int tomoyo_update_aggregator_entry(const char *original_name,
					  const char *aggregated_name,
					  const bool is_delete)
{
	struct tomoyo_aggregator e = { };
	int error = is_delete ? -ENOENT : -ENOMEM;

	if (!tomoyo_correct_path(original_name) ||
=======
int tomoyo_write_aggregator(struct tomoyo_acl_param *param)
{
	struct tomoyo_aggregator e = { };
	int error = param->is_delete ? -ENOENT : -ENOMEM;
	const char *original_name = tomoyo_read_token(param);
	const char *aggregated_name = tomoyo_read_token(param);
	if (!tomoyo_correct_word(original_name) ||
>>>>>>> refs/remotes/origin/cm-10.0
	    !tomoyo_correct_path(aggregated_name))
		return -EINVAL;
	e.original_name = tomoyo_get_name(original_name);
	e.aggregated_name = tomoyo_get_name(aggregated_name);
	if (!e.original_name || !e.aggregated_name ||
	    e.aggregated_name->is_patterned) /* No patterns allowed. */
		goto out;
<<<<<<< HEAD
	error = tomoyo_update_policy(&e.head, sizeof(e), is_delete,
				     &tomoyo_policy_list[TOMOYO_ID_AGGREGATOR],
				     tomoyo_same_aggregator);
 out:
=======
	param->list = &param->ns->policy_list[TOMOYO_ID_AGGREGATOR];
	error = tomoyo_update_policy(&e.head, sizeof(e), param,
				     tomoyo_same_aggregator);
out:
>>>>>>> refs/remotes/origin/cm-10.0
	tomoyo_put_name(e.original_name);
	tomoyo_put_name(e.aggregated_name);
	return error;
}

/**
<<<<<<< HEAD
 * tomoyo_write_aggregator - Write "struct tomoyo_aggregator" list.
 *
 * @data:      String to parse.
 * @is_delete: True if it is a delete request.
 *
 * Returns 0 on success, negative value otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
int tomoyo_write_aggregator(char *data, const bool is_delete)
{
	char *cp = strchr(data, ' ');

	if (!cp)
		return -EINVAL;
	*cp++ = '\0';
	return tomoyo_update_aggregator_entry(data, cp, is_delete);
}

/**
 * tomoyo_assign_domain - Create a domain.
 *
 * @domainname: The name of domain.
 * @profile:    Profile number to assign if the domain was newly created.
=======
 * tomoyo_find_namespace - Find specified namespace.
 *
 * @name: Name of namespace to find.
 * @len:  Length of @name.
 *
 * Returns pointer to "struct tomoyo_policy_namespace" if found,
 * NULL otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
static struct tomoyo_policy_namespace *tomoyo_find_namespace
(const char *name, const unsigned int len)
{
	struct tomoyo_policy_namespace *ns;
	list_for_each_entry(ns, &tomoyo_namespace_list, namespace_list) {
		if (strncmp(name, ns->name, len) ||
		    (name[len] && name[len] != ' '))
			continue;
		return ns;
	}
	return NULL;
}

/**
 * tomoyo_assign_namespace - Create a new namespace.
 *
 * @domainname: Name of namespace to create.
 *
 * Returns pointer to "struct tomoyo_policy_namespace" on success,
 * NULL otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
struct tomoyo_policy_namespace *tomoyo_assign_namespace(const char *domainname)
{
	struct tomoyo_policy_namespace *ptr;
	struct tomoyo_policy_namespace *entry;
	const char *cp = domainname;
	unsigned int len = 0;
	while (*cp && *cp++ != ' ')
		len++;
	ptr = tomoyo_find_namespace(domainname, len);
	if (ptr)
		return ptr;
	if (len >= TOMOYO_EXEC_TMPSIZE - 10 || !tomoyo_domain_def(domainname))
		return NULL;
	entry = kzalloc(sizeof(*entry) + len + 1, GFP_NOFS);
	if (!entry)
		return NULL;
	if (mutex_lock_interruptible(&tomoyo_policy_lock))
		goto out;
	ptr = tomoyo_find_namespace(domainname, len);
	if (!ptr && tomoyo_memory_ok(entry)) {
		char *name = (char *) (entry + 1);
		ptr = entry;
		memmove(name, domainname, len);
		name[len] = '\0';
		entry->name = name;
		tomoyo_init_policy_namespace(entry);
		entry = NULL;
	}
	mutex_unlock(&tomoyo_policy_lock);
out:
	kfree(entry);
	return ptr;
}

/**
 * tomoyo_namespace_jump - Check for namespace jump.
 *
 * @domainname: Name of domain.
 *
 * Returns true if namespace differs, false otherwise.
 */
static bool tomoyo_namespace_jump(const char *domainname)
{
	const char *namespace = tomoyo_current_namespace()->name;
	const int len = strlen(namespace);
	return strncmp(domainname, namespace, len) ||
		(domainname[len] && domainname[len] != ' ');
}

/**
 * tomoyo_assign_domain - Create a domain or a namespace.
 *
 * @domainname: The name of domain.
 * @transit:    True if transit to domain found or created.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Returns pointer to "struct tomoyo_domain_info" on success, NULL otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
struct tomoyo_domain_info *tomoyo_assign_domain(const char *domainname,
<<<<<<< HEAD
						const u8 profile)
{
	struct tomoyo_domain_info *entry;
	struct tomoyo_domain_info *domain = NULL;
	const struct tomoyo_path_info *saved_domainname;
	bool found = false;

	if (!tomoyo_correct_domain(domainname))
		return NULL;
	saved_domainname = tomoyo_get_name(domainname);
	if (!saved_domainname)
		return NULL;
	entry = kzalloc(sizeof(*entry), GFP_NOFS);
	if (mutex_lock_interruptible(&tomoyo_policy_lock))
		goto out;
	list_for_each_entry_rcu(domain, &tomoyo_domain_list, list) {
		if (domain->is_deleted ||
		    tomoyo_pathcmp(saved_domainname, domain->domainname))
			continue;
		found = true;
		break;
	}
	if (!found && tomoyo_memory_ok(entry)) {
		INIT_LIST_HEAD(&entry->acl_info_list);
		entry->domainname = saved_domainname;
		saved_domainname = NULL;
		entry->profile = profile;
		list_add_tail_rcu(&entry->list, &tomoyo_domain_list);
		domain = entry;
		entry = NULL;
		found = true;
	}
	mutex_unlock(&tomoyo_policy_lock);
 out:
	tomoyo_put_name(saved_domainname);
	kfree(entry);
	return found ? domain : NULL;
=======
						const bool transit)
{
	struct tomoyo_domain_info e = { };
	struct tomoyo_domain_info *entry = tomoyo_find_domain(domainname);
	bool created = false;
	if (entry) {
		if (transit) {
			/*
			 * Since namespace is created at runtime, profiles may
			 * not be created by the moment the process transits to
			 * that domain. Do not perform domain transition if
			 * profile for that domain is not yet created.
			 */
			if (tomoyo_policy_loaded &&
			    !entry->ns->profile_ptr[entry->profile])
				return NULL;
		}
		return entry;
	}
	/* Requested domain does not exist. */
	/* Don't create requested domain if domainname is invalid. */
	if (strlen(domainname) >= TOMOYO_EXEC_TMPSIZE - 10 ||
	    !tomoyo_correct_domain(domainname))
		return NULL;
	/*
	 * Since definition of profiles and acl_groups may differ across
	 * namespaces, do not inherit "use_profile" and "use_group" settings
	 * by automatically creating requested domain upon domain transition.
	 */
	if (transit && tomoyo_namespace_jump(domainname))
		return NULL;
	e.ns = tomoyo_assign_namespace(domainname);
	if (!e.ns)
		return NULL;
	/*
	 * "use_profile" and "use_group" settings for automatically created
	 * domains are inherited from current domain. These are 0 for manually
	 * created domains.
	 */
	if (transit) {
		const struct tomoyo_domain_info *domain = tomoyo_domain();
		e.profile = domain->profile;
		e.group = domain->group;
	}
	e.domainname = tomoyo_get_name(domainname);
	if (!e.domainname)
		return NULL;
	if (mutex_lock_interruptible(&tomoyo_policy_lock))
		goto out;
	entry = tomoyo_find_domain(domainname);
	if (!entry) {
		entry = tomoyo_commit_ok(&e, sizeof(e));
		if (entry) {
			INIT_LIST_HEAD(&entry->acl_info_list);
			list_add_tail_rcu(&entry->list, &tomoyo_domain_list);
			created = true;
		}
	}
	mutex_unlock(&tomoyo_policy_lock);
out:
	tomoyo_put_name(e.domainname);
	if (entry && transit) {
		if (created) {
			struct tomoyo_request_info r;
			tomoyo_init_request_info(&r, entry,
						 TOMOYO_MAC_FILE_EXECUTE);
			r.granted = false;
			tomoyo_write_log(&r, "use_profile %u\n",
					 entry->profile);
			tomoyo_write_log(&r, "use_group %u\n", entry->group);
			tomoyo_update_stat(TOMOYO_STAT_POLICY_UPDATES);
		}
	}
	return entry;
}

/**
 * tomoyo_environ - Check permission for environment variable names.
 *
 * @ee: Pointer to "struct tomoyo_execve".
 *
 * Returns 0 on success, negative value otherwise.
 */
static int tomoyo_environ(struct tomoyo_execve *ee)
{
	struct tomoyo_request_info *r = &ee->r;
	struct linux_binprm *bprm = ee->bprm;
	/* env_page.data is allocated by tomoyo_dump_page(). */
	struct tomoyo_page_dump env_page = { };
	char *arg_ptr; /* Size is TOMOYO_EXEC_TMPSIZE bytes */
	int arg_len = 0;
	unsigned long pos = bprm->p;
	int offset = pos % PAGE_SIZE;
	int argv_count = bprm->argc;
	int envp_count = bprm->envc;
	int error = -ENOMEM;

	ee->r.type = TOMOYO_MAC_ENVIRON;
	ee->r.profile = r->domain->profile;
	ee->r.mode = tomoyo_get_mode(r->domain->ns, ee->r.profile,
				     TOMOYO_MAC_ENVIRON);
	if (!r->mode || !envp_count)
		return 0;
	arg_ptr = kzalloc(TOMOYO_EXEC_TMPSIZE, GFP_NOFS);
	if (!arg_ptr)
		goto out;
	while (error == -ENOMEM) {
		if (!tomoyo_dump_page(bprm, pos, &env_page))
			goto out;
		pos += PAGE_SIZE - offset;
		/* Read. */
		while (argv_count && offset < PAGE_SIZE) {
			if (!env_page.data[offset++])
				argv_count--;
		}
		if (argv_count) {
			offset = 0;
			continue;
		}
		while (offset < PAGE_SIZE) {
			const unsigned char c = env_page.data[offset++];

			if (c && arg_len < TOMOYO_EXEC_TMPSIZE - 10) {
				if (c == '=') {
					arg_ptr[arg_len++] = '\0';
				} else if (c == '\\') {
					arg_ptr[arg_len++] = '\\';
					arg_ptr[arg_len++] = '\\';
				} else if (c > ' ' && c < 127) {
					arg_ptr[arg_len++] = c;
				} else {
					arg_ptr[arg_len++] = '\\';
					arg_ptr[arg_len++] = (c >> 6) + '0';
					arg_ptr[arg_len++]
						= ((c >> 3) & 7) + '0';
					arg_ptr[arg_len++] = (c & 7) + '0';
				}
			} else {
				arg_ptr[arg_len] = '\0';
			}
			if (c)
				continue;
			if (tomoyo_env_perm(r, arg_ptr)) {
				error = -EPERM;
				break;
			}
			if (!--envp_count) {
				error = 0;
				break;
			}
			arg_len = 0;
		}
		offset = 0;
	}
out:
	if (r->mode != TOMOYO_CONFIG_ENFORCING)
		error = 0;
	kfree(env_page.data);
	kfree(arg_ptr);
	return error;
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * tomoyo_find_next_domain - Find a domain.
 *
 * @bprm: Pointer to "struct linux_binprm".
 *
 * Returns 0 on success, negative value otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
int tomoyo_find_next_domain(struct linux_binprm *bprm)
{
<<<<<<< HEAD
	struct tomoyo_request_info r;
	char *tmp = kzalloc(TOMOYO_EXEC_TMPSIZE, GFP_NOFS);
	struct tomoyo_domain_info *old_domain = tomoyo_domain();
	struct tomoyo_domain_info *domain = NULL;
	const char *original_name = bprm->filename;
	u8 mode;
	bool is_enforce;
	int retval = -ENOMEM;
	bool need_kfree = false;
	struct tomoyo_path_info rn = { }; /* real name */

	mode = tomoyo_init_request_info(&r, NULL, TOMOYO_MAC_FILE_EXECUTE);
	is_enforce = (mode == TOMOYO_CONFIG_ENFORCING);
	if (!tmp)
		goto out;

 retry:
	if (need_kfree) {
		kfree(rn.name);
		need_kfree = false;
	}
	/* Get symlink's pathname of program. */
	retval = -ENOENT;
	rn.name = tomoyo_realpath_nofollow(original_name);
	if (!rn.name)
		goto out;
	tomoyo_fill_path_info(&rn);
	need_kfree = true;

	/* Check 'aggregator' directive. */
	{
		struct tomoyo_aggregator *ptr;
		list_for_each_entry_rcu(ptr, &tomoyo_policy_list
					[TOMOYO_ID_AGGREGATOR], head.list) {
			if (ptr->head.is_deleted ||
			    !tomoyo_path_matches_pattern(&rn,
							 ptr->original_name))
				continue;
			kfree(rn.name);
			need_kfree = false;
			/* This is OK because it is read only. */
			rn = *ptr->aggregated_name;
=======
	struct tomoyo_domain_info *old_domain = tomoyo_domain();
	struct tomoyo_domain_info *domain = NULL;
	const char *original_name = bprm->filename;
	int retval = -ENOMEM;
	bool reject_on_transition_failure = false;
	const struct tomoyo_path_info *candidate;
	struct tomoyo_path_info exename;
	struct tomoyo_execve *ee = kzalloc(sizeof(*ee), GFP_NOFS);

	if (!ee)
		return -ENOMEM;
	ee->tmp = kzalloc(TOMOYO_EXEC_TMPSIZE, GFP_NOFS);
	if (!ee->tmp) {
		kfree(ee);
		return -ENOMEM;
	}
	/* ee->dump->data is allocated by tomoyo_dump_page(). */
	tomoyo_init_request_info(&ee->r, NULL, TOMOYO_MAC_FILE_EXECUTE);
	ee->r.ee = ee;
	ee->bprm = bprm;
	ee->r.obj = &ee->obj;
	ee->obj.path1 = bprm->file->f_path;
	/* Get symlink's pathname of program. */
	retval = -ENOENT;
	exename.name = tomoyo_realpath_nofollow(original_name);
	if (!exename.name)
		goto out;
	tomoyo_fill_path_info(&exename);
retry:
	/* Check 'aggregator' directive. */
	{
		struct tomoyo_aggregator *ptr;
		struct list_head *list =
			&old_domain->ns->policy_list[TOMOYO_ID_AGGREGATOR];
		/* Check 'aggregator' directive. */
		candidate = &exename;
		list_for_each_entry_rcu(ptr, list, head.list) {
			if (ptr->head.is_deleted ||
			    !tomoyo_path_matches_pattern(&exename,
							 ptr->original_name))
				continue;
			candidate = ptr->aggregated_name;
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		}
	}

	/* Check execute permission. */
<<<<<<< HEAD
	retval = tomoyo_path_permission(&r, TOMOYO_TYPE_EXECUTE, &rn);
=======
	retval = tomoyo_execute_permission(&ee->r, candidate);
>>>>>>> refs/remotes/origin/cm-10.0
	if (retval == TOMOYO_RETRY_REQUEST)
		goto retry;
	if (retval < 0)
		goto out;
	/*
	 * To be able to specify domainnames with wildcards, use the
	 * pathname specified in the policy (which may contain
	 * wildcard) rather than the pathname passed to execve()
	 * (which never contains wildcard).
	 */
<<<<<<< HEAD
	if (r.param.path.matched_path) {
		if (need_kfree)
			kfree(rn.name);
		need_kfree = false;
		/* This is OK because it is read only. */
		rn = *r.param.path.matched_path;
	}

	/* Calculate domain to transit to. */
	switch (tomoyo_transition_type(old_domain->domainname, &rn)) {
	case TOMOYO_TRANSITION_CONTROL_INITIALIZE:
		/* Transit to the child of tomoyo_kernel_domain domain. */
		snprintf(tmp, TOMOYO_EXEC_TMPSIZE - 1, TOMOYO_ROOT_NAME " "
			 "%s", rn.name);
		break;
	case TOMOYO_TRANSITION_CONTROL_KEEP:
=======
	if (ee->r.param.path.matched_path)
		candidate = ee->r.param.path.matched_path;

	/*
	 * Check for domain transition preference if "file execute" matched.
	 * If preference is given, make do_execve() fail if domain transition
	 * has failed, for domain transition preference should be used with
	 * destination domain defined.
	 */
	if (ee->transition) {
		const char *domainname = ee->transition->name;
		reject_on_transition_failure = true;
		if (!strcmp(domainname, "keep"))
			goto force_keep_domain;
		if (!strcmp(domainname, "child"))
			goto force_child_domain;
		if (!strcmp(domainname, "reset"))
			goto force_reset_domain;
		if (!strcmp(domainname, "initialize"))
			goto force_initialize_domain;
		if (!strcmp(domainname, "parent")) {
			char *cp;
			strncpy(ee->tmp, old_domain->domainname->name,
				TOMOYO_EXEC_TMPSIZE - 1);
			cp = strrchr(ee->tmp, ' ');
			if (cp)
				*cp = '\0';
		} else if (*domainname == '<')
			strncpy(ee->tmp, domainname, TOMOYO_EXEC_TMPSIZE - 1);
		else
			snprintf(ee->tmp, TOMOYO_EXEC_TMPSIZE - 1, "%s %s",
				 old_domain->domainname->name, domainname);
		goto force_jump_domain;
	}
	/*
	 * No domain transition preference specified.
	 * Calculate domain to transit to.
	 */
	switch (tomoyo_transition_type(old_domain->ns, old_domain->domainname,
				       candidate)) {
	case TOMOYO_TRANSITION_CONTROL_RESET:
force_reset_domain:
		/* Transit to the root of specified namespace. */
		snprintf(ee->tmp, TOMOYO_EXEC_TMPSIZE - 1, "<%s>",
			 candidate->name);
		/*
		 * Make do_execve() fail if domain transition across namespaces
		 * has failed.
		 */
		reject_on_transition_failure = true;
		break;
	case TOMOYO_TRANSITION_CONTROL_INITIALIZE:
force_initialize_domain:
		/* Transit to the child of current namespace's root. */
		snprintf(ee->tmp, TOMOYO_EXEC_TMPSIZE - 1, "%s %s",
			 old_domain->ns->name, candidate->name);
		break;
	case TOMOYO_TRANSITION_CONTROL_KEEP:
force_keep_domain:
>>>>>>> refs/remotes/origin/cm-10.0
		/* Keep current domain. */
		domain = old_domain;
		break;
	default:
		if (old_domain == &tomoyo_kernel_domain &&
		    !tomoyo_policy_loaded) {
			/*
			 * Needn't to transit from kernel domain before
			 * starting /sbin/init. But transit from kernel domain
			 * if executing initializers because they might start
			 * before /sbin/init.
			 */
			domain = old_domain;
<<<<<<< HEAD
		} else {
			/* Normal domain transition. */
			snprintf(tmp, TOMOYO_EXEC_TMPSIZE - 1, "%s %s",
				 old_domain->domainname->name, rn.name);
		}
		break;
	}
	if (domain || strlen(tmp) >= TOMOYO_EXEC_TMPSIZE - 10)
		goto done;
	domain = tomoyo_find_domain(tmp);
	if (domain)
		goto done;
	if (is_enforce) {
		int error = tomoyo_supervisor(&r, "# wants to create domain\n"
					      "%s\n", tmp);
		if (error == TOMOYO_RETRY_REQUEST)
			goto retry;
		if (error < 0)
			goto done;
	}
	domain = tomoyo_assign_domain(tmp, old_domain->profile);
 done:
	if (domain)
		goto out;
	printk(KERN_WARNING "TOMOYO-ERROR: Domain '%s' not defined.\n", tmp);
	if (is_enforce)
		retval = -EPERM;
	else
		old_domain->transition_failed = true;
=======
			break;
		}
force_child_domain:
		/* Normal domain transition. */
		snprintf(ee->tmp, TOMOYO_EXEC_TMPSIZE - 1, "%s %s",
			 old_domain->domainname->name, candidate->name);
		break;
	}
force_jump_domain:
	if (!domain)
		domain = tomoyo_assign_domain(ee->tmp, true);
	if (domain)
		retval = 0;
	else if (reject_on_transition_failure) {
		printk(KERN_WARNING "ERROR: Domain '%s' not ready.\n",
		       ee->tmp);
		retval = -ENOMEM;
	} else if (ee->r.mode == TOMOYO_CONFIG_ENFORCING)
		retval = -ENOMEM;
	else {
		retval = 0;
		if (!old_domain->flags[TOMOYO_DIF_TRANSITION_FAILED]) {
			old_domain->flags[TOMOYO_DIF_TRANSITION_FAILED] = true;
			ee->r.granted = false;
			tomoyo_write_log(&ee->r, "%s", tomoyo_dif
					 [TOMOYO_DIF_TRANSITION_FAILED]);
			printk(KERN_WARNING
			       "ERROR: Domain '%s' not defined.\n", ee->tmp);
		}
	}
>>>>>>> refs/remotes/origin/cm-10.0
 out:
	if (!domain)
		domain = old_domain;
	/* Update reference count on "struct tomoyo_domain_info". */
	atomic_inc(&domain->users);
	bprm->cred->security = domain;
<<<<<<< HEAD
	if (need_kfree)
		kfree(rn.name);
	kfree(tmp);
	return retval;
}
=======
	kfree(exename.name);
	if (!retval) {
		ee->r.domain = domain;
		retval = tomoyo_environ(ee);
	}
	kfree(ee->tmp);
	kfree(ee->dump.data);
	kfree(ee);
	return retval;
}

/**
 * tomoyo_dump_page - Dump a page to buffer.
 *
 * @bprm: Pointer to "struct linux_binprm".
 * @pos:  Location to dump.
 * @dump: Poiner to "struct tomoyo_page_dump".
 *
 * Returns true on success, false otherwise.
 */
bool tomoyo_dump_page(struct linux_binprm *bprm, unsigned long pos,
		      struct tomoyo_page_dump *dump)
{
	struct page *page;

	/* dump->data is released by tomoyo_find_next_domain(). */
	if (!dump->data) {
		dump->data = kzalloc(PAGE_SIZE, GFP_NOFS);
		if (!dump->data)
			return false;
	}
	/* Same with get_arg_page(bprm, pos, 0) in fs/exec.c */
#ifdef CONFIG_MMU
	if (get_user_pages(current, bprm->mm, pos, 1, 0, 1, &page, NULL) <= 0)
		return false;
#else
	page = bprm->page[pos / PAGE_SIZE];
#endif
	if (page != dump->page) {
		const unsigned int offset = pos % PAGE_SIZE;
		/*
		 * Maybe kmap()/kunmap() should be used here.
		 * But remove_arg_zero() uses kmap_atomic()/kunmap_atomic().
		 * So do I.
		 */
		char *kaddr = kmap_atomic(page);

		dump->page = page;
		memcpy(dump->data + offset, kaddr + offset,
		       PAGE_SIZE - offset);
		kunmap_atomic(kaddr);
	}
	/* Same with put_arg_page(page) in fs/exec.c */
#ifdef CONFIG_MMU
	put_page(page);
#endif
	return true;
}
>>>>>>> refs/remotes/origin/cm-10.0
