/*
 * Jump label s390 support
 *
 * Copyright IBM Corp. 2011
 * Author(s): Jan Glauber <jang@linux.vnet.ibm.com>
 */
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/stop_machine.h>
#include <linux/jump_label.h>
#include <asm/ipl.h>

#ifdef HAVE_JUMP_LABEL

struct insn {
	u16 opcode;
	s32 offset;
} __packed;

struct insn_args {
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long *target;
	struct insn *insn;
	ssize_t size;
};

static int __arch_jump_label_transform(void *data)
{
	struct insn_args *args = data;
	int rc;

	rc = probe_kernel_write(args->target, args->insn, args->size);
	WARN_ON_ONCE(rc < 0);
	return 0;
}

void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	struct insn_args args;
	struct insn insn;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct jump_entry *entry;
	enum jump_label_type type;
};

static void __jump_label_transform(struct jump_entry *entry,
				   enum jump_label_type type)
{
	struct insn insn;
	int rc;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (type == JUMP_LABEL_ENABLE) {
		/* brcl 15,offset */
		insn.opcode = 0xc0f4;
		insn.offset = (entry->target - entry->code) >> 1;
	} else {
		/* brcl 0,0 */
		insn.opcode = 0xc004;
		insn.offset = 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	args.target = (void *) entry->code;
	args.insn = &insn;
	args.size = JUMP_LABEL_NOP_SIZE;

	stop_machine(__arch_jump_label_transform, &args, NULL);
=======
=======
>>>>>>> refs/remotes/origin/master
	rc = probe_kernel_write((void *)entry->code, &insn, JUMP_LABEL_NOP_SIZE);
	WARN_ON_ONCE(rc < 0);
}

static int __sm_arch_jump_label_transform(void *data)
{
	struct insn_args *args = data;

	__jump_label_transform(args->entry, args->type);
	return 0;
}

void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	struct insn_args args;

	args.entry = entry;
	args.type = type;

	stop_machine(__sm_arch_jump_label_transform, &args, NULL);
}

void arch_jump_label_transform_static(struct jump_entry *entry,
				      enum jump_label_type type)
{
	__jump_label_transform(entry, type);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

#endif
