<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/init.h>

__INITDATA

	.globl vdso_start, vdso_end
vdso_start:
	.incbin "arch/x86/vdso/vdso.so"
vdso_end:

__FINIT
=======
#include <asm/page_types.h>
#include <linux/linkage.h>
#include <linux/init.h>
=======
#include <asm/page_types.h>
#include <linux/linkage.h>
>>>>>>> refs/remotes/origin/master

__PAGE_ALIGNED_DATA

	.globl vdso_start, vdso_end
	.align PAGE_SIZE
vdso_start:
	.incbin "arch/x86/vdso/vdso.so"
vdso_end:
	.align PAGE_SIZE /* extra data here leaks to userspace. */

.previous

	.globl vdso_pages
	.bss
	.align 8
	.type vdso_pages, @object
vdso_pages:
	.zero (vdso_end - vdso_start + PAGE_SIZE - 1) / PAGE_SIZE * 8
	.size vdso_pages, .-vdso_pages
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
