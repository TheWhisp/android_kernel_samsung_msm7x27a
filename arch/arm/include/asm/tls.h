#ifndef __ASMARM_TLS_H
#define __ASMARM_TLS_H

#ifdef __ASSEMBLY__
<<<<<<< HEAD
	.macro set_tls_none, tp, tmp1, tmp2
	.endm

	.macro set_tls_v6k, tp, tmp1, tmp2
	mcr	p15, 0, \tp, c13, c0, 3		@ set TLS register
	mov	\tmp1, #0
	mcr	p15, 0, \tmp1, c13, c0, 2	@ clear user r/w TLS register
	.endm

	.macro set_tls_v6, tp, tmp1, tmp2
=======
#include <asm/asm-offsets.h>
	.macro switch_tls_none, base, tp, tpuser, tmp1, tmp2
	.endm

	.macro switch_tls_v6k, base, tp, tpuser, tmp1, tmp2
	mrc	p15, 0, \tmp2, c13, c0, 2	@ get the user r/w register
	mcr	p15, 0, \tp, c13, c0, 3		@ set TLS register
	mcr	p15, 0, \tpuser, c13, c0, 2	@ and the user r/w register
	str	\tmp2, [\base, #TI_TP_VALUE + 4] @ save it
	.endm

	.macro switch_tls_v6, base, tp, tpuser, tmp1, tmp2
>>>>>>> refs/remotes/origin/master
	ldr	\tmp1, =elf_hwcap
	ldr	\tmp1, [\tmp1, #0]
	mov	\tmp2, #0xffff0fff
	tst	\tmp1, #HWCAP_TLS		@ hardware TLS available?
<<<<<<< HEAD
	mcrne	p15, 0, \tp, c13, c0, 3		@ yes, set TLS register
	movne	\tmp1, #0
	mcrne	p15, 0, \tmp1, c13, c0, 2	@ clear user r/w TLS register
	streq	\tp, [\tmp2, #-15]		@ set TLS value at 0xffff0ff0
	.endm

	.macro set_tls_software, tp, tmp1, tmp2
=======
	streq	\tp, [\tmp2, #-15]		@ set TLS value at 0xffff0ff0
	mrcne	p15, 0, \tmp2, c13, c0, 2	@ get the user r/w register
	mcrne	p15, 0, \tp, c13, c0, 3		@ yes, set TLS register
	mcrne	p15, 0, \tpuser, c13, c0, 2	@ set user r/w register
	strne	\tmp2, [\base, #TI_TP_VALUE + 4] @ save it
	.endm

	.macro switch_tls_software, base, tp, tpuser, tmp1, tmp2
>>>>>>> refs/remotes/origin/master
	mov	\tmp1, #0xffff0fff
	str	\tp, [\tmp1, #-15]		@ set TLS value at 0xffff0ff0
	.endm
#endif

#ifdef CONFIG_TLS_REG_EMUL
#define tls_emu		1
#define has_tls_reg		1
<<<<<<< HEAD
#define set_tls		set_tls_none
#elif defined(CONFIG_CPU_V6)
#define tls_emu		0
#define has_tls_reg		(elf_hwcap & HWCAP_TLS)
#define set_tls		set_tls_v6
#elif defined(CONFIG_CPU_32v6K)
#define tls_emu		0
#define has_tls_reg		1
#define set_tls		set_tls_v6k
#else
#define tls_emu		0
#define has_tls_reg		0
#define set_tls		set_tls_software
#endif

=======
#define switch_tls	switch_tls_none
#elif defined(CONFIG_CPU_V6)
#define tls_emu		0
#define has_tls_reg		(elf_hwcap & HWCAP_TLS)
#define switch_tls	switch_tls_v6
#elif defined(CONFIG_CPU_32v6K)
#define tls_emu		0
#define has_tls_reg		1
#define switch_tls	switch_tls_v6k
#else
#define tls_emu		0
#define has_tls_reg		0
#define switch_tls	switch_tls_software
#endif

#ifndef __ASSEMBLY__
static inline unsigned long get_tpuser(void)
{
	unsigned long reg = 0;

	if (has_tls_reg && !tls_emu)
		__asm__("mrc p15, 0, %0, c13, c0, 2" : "=r" (reg));

	return reg;
}
#endif
>>>>>>> refs/remotes/origin/master
#endif	/* __ASMARM_TLS_H */
