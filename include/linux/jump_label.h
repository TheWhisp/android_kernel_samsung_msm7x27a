#ifndef _LINUX_JUMP_LABEL_H
#define _LINUX_JUMP_LABEL_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/types.h>
#include <linux/compiler.h>

#if defined(CC_HAVE_ASM_GOTO) && defined(CONFIG_JUMP_LABEL)

struct jump_label_key {
	atomic_t enabled;
	struct jump_entry *entries;
#ifdef CONFIG_MODULES
	struct jump_label_mod *next;
#endif
};

# include <asm/jump_label.h>
# define HAVE_JUMP_LABEL
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Jump label support
 *
 * Copyright (C) 2009-2012 Jason Baron <jbaron@redhat.com>
 * Copyright (C) 2011-2012 Peter Zijlstra <pzijlstr@redhat.com>
 *
 * Jump labels provide an interface to generate dynamic branches using
 * self-modifying code. Assuming toolchain and architecture support the result
 * of a "if (static_key_false(&key))" statement is a unconditional branch (which
 * defaults to false - and the true block is placed out of line).
 *
 * However at runtime we can change the branch target using
 * static_key_slow_{inc,dec}(). These function as a 'reference' count on the key
 * object and for as long as there are references all branches referring to
 * that particular key will point to the (out of line) true block.
 *
 * Since this relies on modifying code the static_key_slow_{inc,dec}() functions
 * must be considered absolute slow paths (machine wide synchronization etc.).
 * OTOH, since the affected branches are unconditional their runtime overhead
 * will be absolutely minimal, esp. in the default (off) case where the total
 * effect is a single NOP of appropriate size. The on case will patch in a jump
 * to the out-of-line block.
 *
 * When the control is directly exposed to userspace it is prudent to delay the
 * decrement to avoid high frequency code modifications which can (and do)
 * cause significant performance degradation. Struct static_key_deferred and
 * static_key_slow_dec_deferred() provide for this.
 *
 * Lacking toolchain and or architecture support, it falls back to a simple
 * conditional branch.
 *
 * struct static_key my_key = STATIC_KEY_INIT_TRUE;
 *
 *   if (static_key_true(&my_key)) {
 *   }
 *
 * will result in the true case being in-line and starts the key with a single
 * reference. Mixing static_key_true() and static_key_false() on the same key is not
 * allowed.
 *
 * Not initializing the key (static data is initialized to 0s anyway) is the
<<<<<<< HEAD
 * same as using STATIC_KEY_INIT_FALSE and static_key_false() is
 * equivalent with static_branch().
=======
 * same as using STATIC_KEY_INIT_FALSE.
>>>>>>> refs/remotes/origin/master
 *
*/

#include <linux/types.h>
#include <linux/compiler.h>
<<<<<<< HEAD
#include <linux/workqueue.h>
=======
#include <linux/bug.h>

extern bool static_key_initialized;

#define STATIC_KEY_CHECK_USE() WARN(!static_key_initialized,		      \
				    "%s used before call to jump_label_init", \
				    __func__)
>>>>>>> refs/remotes/origin/master

#if defined(CC_HAVE_ASM_GOTO) && defined(CONFIG_JUMP_LABEL)

struct static_key {
	atomic_t enabled;
/* Set lsb bit to 1 if branch is default true, 0 ot */
	struct jump_entry *entries;
#ifdef CONFIG_MODULES
	struct static_key_mod *next;
#endif
};

<<<<<<< HEAD
struct static_key_deferred {
	struct static_key key;
	unsigned long timeout;
	struct delayed_work work;
};

# include <asm/jump_label.h>
# define HAVE_JUMP_LABEL
#endif	/* CC_HAVE_ASM_GOTO && CONFIG_JUMP_LABEL */
>>>>>>> refs/remotes/origin/cm-10.0
=======
# include <asm/jump_label.h>
# define HAVE_JUMP_LABEL
#endif	/* CC_HAVE_ASM_GOTO && CONFIG_JUMP_LABEL */
>>>>>>> refs/remotes/origin/master

enum jump_label_type {
	JUMP_LABEL_DISABLE = 0,
	JUMP_LABEL_ENABLE,
};

struct module;

<<<<<<< HEAD
#ifdef HAVE_JUMP_LABEL

<<<<<<< HEAD
#ifdef CONFIG_MODULES
#define JUMP_LABEL_INIT {{ 0 }, NULL, NULL}
#else
#define JUMP_LABEL_INIT {{ 0 }, NULL}
#endif

static __always_inline bool static_branch(struct jump_label_key *key)
=======
#define JUMP_LABEL_TRUE_BRANCH 1UL
=======
#include <linux/atomic.h>
#ifdef HAVE_JUMP_LABEL

#define JUMP_LABEL_TYPE_FALSE_BRANCH	0UL
#define JUMP_LABEL_TYPE_TRUE_BRANCH	1UL
#define JUMP_LABEL_TYPE_MASK		1UL
>>>>>>> refs/remotes/origin/master

static
inline struct jump_entry *jump_label_get_entries(struct static_key *key)
{
	return (struct jump_entry *)((unsigned long)key->entries
<<<<<<< HEAD
						& ~JUMP_LABEL_TRUE_BRANCH);
=======
						& ~JUMP_LABEL_TYPE_MASK);
>>>>>>> refs/remotes/origin/master
}

static inline bool jump_label_get_branch_default(struct static_key *key)
{
<<<<<<< HEAD
	if ((unsigned long)key->entries & JUMP_LABEL_TRUE_BRANCH)
=======
	if (((unsigned long)key->entries & JUMP_LABEL_TYPE_MASK) ==
	    JUMP_LABEL_TYPE_TRUE_BRANCH)
>>>>>>> refs/remotes/origin/master
		return true;
	return false;
}

static __always_inline bool static_key_false(struct static_key *key)
{
	return arch_static_branch(key);
}

static __always_inline bool static_key_true(struct static_key *key)
{
	return !static_key_false(key);
}

<<<<<<< HEAD
/* Deprecated. Please use 'static_key_false() instead. */
static __always_inline bool static_branch(struct static_key *key)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return arch_static_branch(key);
}

extern struct jump_entry __start___jump_table[];
extern struct jump_entry __stop___jump_table[];

<<<<<<< HEAD
extern void jump_label_lock(void);
extern void jump_label_unlock(void);
extern void arch_jump_label_transform(struct jump_entry *entry,
				 enum jump_label_type type);
extern void arch_jump_label_text_poke_early(jump_label_t addr);
extern int jump_label_text_reserved(void *start, void *end);
extern void jump_label_inc(struct jump_label_key *key);
extern void jump_label_dec(struct jump_label_key *key);
extern bool jump_label_enabled(struct jump_label_key *key);
extern void jump_label_apply_nops(struct module *mod);

#else

#include <asm/atomic.h>

#define JUMP_LABEL_INIT {ATOMIC_INIT(0)}

struct jump_label_key {
	atomic_t enabled;
};

static __always_inline bool static_branch(struct jump_label_key *key)
{
	if (unlikely(atomic_read(&key->enabled)))
=======
=======
extern struct jump_entry __start___jump_table[];
extern struct jump_entry __stop___jump_table[];

>>>>>>> refs/remotes/origin/master
extern void jump_label_init(void);
extern void jump_label_lock(void);
extern void jump_label_unlock(void);
extern void arch_jump_label_transform(struct jump_entry *entry,
				      enum jump_label_type type);
extern void arch_jump_label_transform_static(struct jump_entry *entry,
					     enum jump_label_type type);
extern int jump_label_text_reserved(void *start, void *end);
extern void static_key_slow_inc(struct static_key *key);
extern void static_key_slow_dec(struct static_key *key);
<<<<<<< HEAD
extern void static_key_slow_dec_deferred(struct static_key_deferred *key);
extern void jump_label_apply_nops(struct module *mod);
extern void
jump_label_rate_limit(struct static_key_deferred *key, unsigned long rl);

#define STATIC_KEY_INIT_TRUE ((struct static_key) \
	{ .enabled = ATOMIC_INIT(1), .entries = (void *)1 })
#define STATIC_KEY_INIT_FALSE ((struct static_key) \
	{ .enabled = ATOMIC_INIT(0), .entries = (void *)0 })

#else  /* !HAVE_JUMP_LABEL */

#include <linux/atomic.h>

=======
extern void jump_label_apply_nops(struct module *mod);

#define STATIC_KEY_INIT_TRUE ((struct static_key)		\
	{ .enabled = ATOMIC_INIT(1),				\
	  .entries = (void *)JUMP_LABEL_TYPE_TRUE_BRANCH })
#define STATIC_KEY_INIT_FALSE ((struct static_key)		\
	{ .enabled = ATOMIC_INIT(0),				\
	  .entries = (void *)JUMP_LABEL_TYPE_FALSE_BRANCH })

#else  /* !HAVE_JUMP_LABEL */

>>>>>>> refs/remotes/origin/master
struct static_key {
	atomic_t enabled;
};

static __always_inline void jump_label_init(void)
{
<<<<<<< HEAD
}

struct static_key_deferred {
	struct static_key  key;
};

static __always_inline bool static_key_false(struct static_key *key)
{
	if (unlikely(atomic_read(&key->enabled)) > 0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	static_key_initialized = true;
}

static __always_inline bool static_key_false(struct static_key *key)
{
	if (unlikely(atomic_read(&key->enabled) > 0))
>>>>>>> refs/remotes/origin/master
		return true;
	return false;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void jump_label_inc(struct jump_label_key *key)
=======
static __always_inline bool static_key_true(struct static_key *key)
{
	if (likely(atomic_read(&key->enabled)) > 0)
		return true;
	return false;
}

/* Deprecated. Please use 'static_key_false() instead. */
static __always_inline bool static_branch(struct static_key *key)
{
	if (unlikely(atomic_read(&key->enabled)) > 0)
=======
static __always_inline bool static_key_true(struct static_key *key)
{
	if (likely(atomic_read(&key->enabled) > 0))
>>>>>>> refs/remotes/origin/master
		return true;
	return false;
}

static inline void static_key_slow_inc(struct static_key *key)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
{
	atomic_inc(&key->enabled);
}

<<<<<<< HEAD
static inline void jump_label_dec(struct jump_label_key *key)
=======
static inline void static_key_slow_dec(struct static_key *key)
>>>>>>> refs/remotes/origin/cm-10.0
{
	atomic_dec(&key->enabled);
}

<<<<<<< HEAD
=======
static inline void static_key_slow_dec_deferred(struct static_key_deferred *key)
{
	static_key_slow_dec(&key->key);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
{
	STATIC_KEY_CHECK_USE();
	atomic_inc(&key->enabled);
}

static inline void static_key_slow_dec(struct static_key *key)
{
	STATIC_KEY_CHECK_USE();
	atomic_dec(&key->enabled);
}

>>>>>>> refs/remotes/origin/master
static inline int jump_label_text_reserved(void *start, void *end)
{
	return 0;
}

static inline void jump_label_lock(void) {}
static inline void jump_label_unlock(void) {}

<<<<<<< HEAD
<<<<<<< HEAD
static inline bool jump_label_enabled(struct jump_label_key *key)
{
	return !!atomic_read(&key->enabled);
}

static inline int jump_label_apply_nops(struct module *mod)
{
	return 0;
}

#endif

#endif
=======
=======
>>>>>>> refs/remotes/origin/master
static inline int jump_label_apply_nops(struct module *mod)
{
	return 0;
}

<<<<<<< HEAD
static inline void
jump_label_rate_limit(struct static_key_deferred *key,
		unsigned long rl)
{
}

=======
>>>>>>> refs/remotes/origin/master
#define STATIC_KEY_INIT_TRUE ((struct static_key) \
		{ .enabled = ATOMIC_INIT(1) })
#define STATIC_KEY_INIT_FALSE ((struct static_key) \
		{ .enabled = ATOMIC_INIT(0) })

#endif	/* HAVE_JUMP_LABEL */

#define STATIC_KEY_INIT STATIC_KEY_INIT_FALSE
#define jump_label_enabled static_key_enabled

static inline bool static_key_enabled(struct static_key *key)
{
	return (atomic_read(&key->enabled) > 0);
}

#endif	/* _LINUX_JUMP_LABEL_H */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
