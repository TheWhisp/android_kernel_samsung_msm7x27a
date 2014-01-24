/*
 * AppArmor security module
 *
 * This file contains AppArmor ipc mediation
 *
 * Copyright (C) 1998-2008 Novell/SUSE
 * Copyright 2009-2010 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 */

#include <linux/gfp.h>
#include <linux/ptrace.h>

#include "include/audit.h"
#include "include/capability.h"
#include "include/context.h"
#include "include/policy.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "include/ipc.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "include/ipc.h"
>>>>>>> refs/remotes/origin/master

/* call back to audit ptrace fields */
static void audit_cb(struct audit_buffer *ab, void *va)
{
	struct common_audit_data *sa = va;
	audit_log_format(ab, " target=");
<<<<<<< HEAD
<<<<<<< HEAD
	audit_log_untrustedstring(ab, sa->aad.target);
=======
	audit_log_untrustedstring(ab, sa->aad->target);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	audit_log_untrustedstring(ab, sa->aad->target);
>>>>>>> refs/remotes/origin/master
}

/**
 * aa_audit_ptrace - do auditing for ptrace
 * @profile: profile being enforced  (NOT NULL)
 * @target: profile being traced (NOT NULL)
 * @error: error condition
 *
 * Returns: %0 or error code
 */
static int aa_audit_ptrace(struct aa_profile *profile,
			   struct aa_profile *target, int error)
{
	struct common_audit_data sa;
<<<<<<< HEAD
<<<<<<< HEAD
	COMMON_AUDIT_DATA_INIT(&sa, NONE);
	sa.aad.op = OP_PTRACE;
	sa.aad.target = target;
	sa.aad.error = error;
=======
	struct apparmor_audit_data aad = {0,};
	COMMON_AUDIT_DATA_INIT(&sa, NONE);
=======
	struct apparmor_audit_data aad = {0,};
	sa.type = LSM_AUDIT_DATA_NONE;
>>>>>>> refs/remotes/origin/master
	sa.aad = &aad;
	aad.op = OP_PTRACE;
	aad.target = target;
	aad.error = error;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return aa_audit(AUDIT_APPARMOR_AUTO, profile, GFP_ATOMIC, &sa,
			audit_cb);
}

/**
 * aa_may_ptrace - test if tracer task can trace the tracee
<<<<<<< HEAD
 * @tracer_task: task who will do the tracing  (NOT NULL)
=======
>>>>>>> refs/remotes/origin/master
 * @tracer: profile of the task doing the tracing  (NOT NULL)
 * @tracee: task to be traced
 * @mode: whether PTRACE_MODE_READ || PTRACE_MODE_ATTACH
 *
 * Returns: %0 else error code if permission denied or error
 */
<<<<<<< HEAD
int aa_may_ptrace(struct task_struct *tracer_task, struct aa_profile *tracer,
		  struct aa_profile *tracee, unsigned int mode)
=======
int aa_may_ptrace(struct aa_profile *tracer, struct aa_profile *tracee,
		  unsigned int mode)
>>>>>>> refs/remotes/origin/master
{
	/* TODO: currently only based on capability, not extended ptrace
	 *       rules,
	 *       Test mode for PTRACE_MODE_READ || PTRACE_MODE_ATTACH
	 */

	if (unconfined(tracer) || tracer == tracee)
		return 0;
	/* log this capability request */
<<<<<<< HEAD
	return aa_capable(tracer_task, tracer, CAP_SYS_PTRACE, 1);
=======
	return aa_capable(tracer, CAP_SYS_PTRACE, 1);
>>>>>>> refs/remotes/origin/master
}

/**
 * aa_ptrace - do ptrace permission check and auditing
 * @tracer: task doing the tracing (NOT NULL)
 * @tracee: task being traced (NOT NULL)
 * @mode: ptrace mode either PTRACE_MODE_READ || PTRACE_MODE_ATTACH
 *
 * Returns: %0 else error code if permission denied or error
 */
int aa_ptrace(struct task_struct *tracer, struct task_struct *tracee,
	      unsigned int mode)
{
	/*
	 * tracer can ptrace tracee when
	 * - tracer is unconfined ||
	 *   - tracer is in complain mode
	 *   - tracer has rules allowing it to trace tracee currently this is:
	 *       - confined by the same profile ||
	 *       - tracer profile has CAP_SYS_PTRACE
	 */

<<<<<<< HEAD
	struct aa_profile *tracer_p;
	/* cred released below */
	const struct cred *cred = get_task_cred(tracer);
	int error = 0;
	tracer_p = aa_cred_profile(cred);

	if (!unconfined(tracer_p)) {
		/* lcred released below */
		const struct cred *lcred = get_task_cred(tracee);
		struct aa_profile *tracee_p = aa_cred_profile(lcred);

		error = aa_may_ptrace(tracer, tracer_p, tracee_p, mode);
		error = aa_audit_ptrace(tracer_p, tracee_p, error);

		put_cred(lcred);
	}
	put_cred(cred);
=======
	struct aa_profile *tracer_p = aa_get_task_profile(tracer);
	int error = 0;

	if (!unconfined(tracer_p)) {
		struct aa_profile *tracee_p = aa_get_task_profile(tracee);

		error = aa_may_ptrace(tracer_p, tracee_p, mode);
		error = aa_audit_ptrace(tracer_p, tracee_p, error);

		aa_put_profile(tracee_p);
	}
	aa_put_profile(tracer_p);
>>>>>>> refs/remotes/origin/master

	return error;
}
