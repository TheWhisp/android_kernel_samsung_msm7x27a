#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/cpu.h>

#include <linux/jump_label.h>
#include <linux/memory.h>

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/cacheflush.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/cacheflush.h>

>>>>>>> refs/remotes/origin/master
#ifdef HAVE_JUMP_LABEL

void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	u32 val;
	u32 *insn = (u32 *) (unsigned long) entry->code;

	if (type == JUMP_LABEL_ENABLE) {
		s32 off = (s32)entry->target - (s32)entry->code;

#ifdef CONFIG_SPARC64
		/* ba,pt %xcc, . + (off << 2) */
		val = 0x10680000 | ((u32) off >> 2);
#else
		/* ba . + (off << 2) */
		val = 0x10800000 | ((u32) off >> 2);
#endif
	} else {
		val = 0x01000000;
	}

	get_online_cpus();
	mutex_lock(&text_mutex);
	*insn = val;
	flushi(insn);
	mutex_unlock(&text_mutex);
	put_online_cpus();
}

<<<<<<< HEAD
<<<<<<< HEAD
void arch_jump_label_text_poke_early(jump_label_t addr)
{
	u32 *insn_p = (u32 *) (unsigned long) addr;

	*insn_p = 0x01000000;
	flushi(insn_p);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif
