#!/bin/sh
<<<<<<< HEAD
<<<<<<< HEAD
# Test for gcc 'asm goto' suport
# Copyright (C) 2010, Jason Baron <jbaron@redhat.com>

echo "int main(void) { entry: asm goto (\"\"::::entry); return 0; }" | $@ -x c - -c -o /dev/null >/dev/null 2>&1 && echo "y"
=======
=======
>>>>>>> refs/remotes/origin/master
# Test for gcc 'asm goto' support
# Copyright (C) 2010, Jason Baron <jbaron@redhat.com>

cat << "END" | $@ -x c - -c -o /dev/null >/dev/null 2>&1 && echo "y"
int main(void)
{
<<<<<<< HEAD
#ifdef __arm__
=======
#if defined(__arm__) || defined(__aarch64__)
>>>>>>> refs/remotes/origin/master
	/*
	 * Not related to asm goto, but used by jump label
	 * and broken on some ARM GCC versions (see GCC Bug 48637).
	 */
	static struct { int dummy; int state; } tp;
	asm (".long %c0" :: "i" (&tp.state));
#endif

entry:
	asm goto ("" :::: entry);
	return 0;
}
END
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
