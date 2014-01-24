/*
 * Helper function for splitting a string into an argv-like array.
 */

#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0

static const char *skip_arg(const char *cp)
{
	while (*cp && !isspace(*cp))
		cp++;

	return cp;
}
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master

static int count_argc(const char *str)
{
	int count = 0;
<<<<<<< HEAD

	while (*str) {
		str = skip_spaces(str);
		if (*str) {
			count++;
			str = skip_arg(str);
=======
	bool was_space;

	for (was_space = true; *str; str++) {
		if (isspace(*str)) {
			was_space = true;
		} else if (was_space) {
			was_space = false;
			count++;
>>>>>>> refs/remotes/origin/master
		}
	}

	return count;
}

/**
 * argv_free - free an argv
 * @argv - the argument vector to be freed
 *
 * Frees an argv and the strings it points to.
 */
void argv_free(char **argv)
{
<<<<<<< HEAD
	char **p;
	for (p = argv; *p; p++)
		kfree(*p);

=======
	argv--;
	kfree(argv[0]);
>>>>>>> refs/remotes/origin/master
	kfree(argv);
}
EXPORT_SYMBOL(argv_free);

/**
 * argv_split - split a string at whitespace, returning an argv
 * @gfp: the GFP mask used to allocate memory
 * @str: the string to be split
 * @argcp: returned argument count
 *
 * Returns an array of pointers to strings which are split out from
 * @str.  This is performed by strictly splitting on white-space; no
 * quote processing is performed.  Multiple whitespace characters are
 * considered to be a single argument separator.  The returned array
 * is always NULL-terminated.  Returns NULL on memory allocation
 * failure.
<<<<<<< HEAD
 */
char **argv_split(gfp_t gfp, const char *str, int *argcp)
{
	int argc = count_argc(str);
	char **argv = kzalloc(sizeof(*argv) * (argc+1), gfp);
	char **argvp;

	if (argv == NULL)
		goto out;

	if (argcp)
		*argcp = argc;

	argvp = argv;

	while (*str) {
		str = skip_spaces(str);

		if (*str) {
			const char *p = str;
			char *t;

			str = skip_arg(str);

			t = kstrndup(p, str-p, gfp);
			if (t == NULL)
				goto fail;
			*argvp++ = t;
		}
	}
	*argvp = NULL;

  out:
	return argv;

  fail:
	argv_free(argv);
	return NULL;
=======
 *
 * The source string at `str' may be undergoing concurrent alteration via
 * userspace sysctl activity (at least).  The argv_split() implementation
 * attempts to handle this gracefully by taking a local copy to work on.
 */
char **argv_split(gfp_t gfp, const char *str, int *argcp)
{
	char *argv_str;
	bool was_space;
	char **argv, **argv_ret;
	int argc;

	argv_str = kstrndup(str, KMALLOC_MAX_SIZE - 1, gfp);
	if (!argv_str)
		return NULL;

	argc = count_argc(argv_str);
	argv = kmalloc(sizeof(*argv) * (argc + 2), gfp);
	if (!argv) {
		kfree(argv_str);
		return NULL;
	}

	*argv = argv_str;
	argv_ret = ++argv;
	for (was_space = true; *argv_str; argv_str++) {
		if (isspace(*argv_str)) {
			was_space = true;
			*argv_str = 0;
		} else if (was_space) {
			was_space = false;
			*argv++ = argv_str;
		}
	}
	*argv = NULL;

	if (argcp)
		*argcp = argc;
	return argv_ret;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(argv_split);
