#ifdef __ASSEMBLY__

<<<<<<< HEAD
=======
#include <asm/asm.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/dwarf2.h>

/* The annotation hides the frame from the unwinder and makes it look
   like a ordinary ebp save/restore. This avoids some special cases for
   frame pointer later */
#ifdef CONFIG_FRAME_POINTER
	.macro FRAME
<<<<<<< HEAD
	pushl_cfi %ebp
	CFI_REL_OFFSET ebp,0
	movl %esp,%ebp
	.endm
	.macro ENDFRAME
	popl_cfi %ebp
	CFI_RESTORE ebp
=======
	__ASM_SIZE(push,_cfi)	%__ASM_REG(bp)
	CFI_REL_OFFSET		__ASM_REG(bp), 0
	__ASM_SIZE(mov)		%__ASM_REG(sp), %__ASM_REG(bp)
	.endm
	.macro ENDFRAME
	__ASM_SIZE(pop,_cfi)	%__ASM_REG(bp)
	CFI_RESTORE		__ASM_REG(bp)
>>>>>>> refs/remotes/origin/cm-10.0
	.endm
#else
	.macro FRAME
	.endm
	.macro ENDFRAME
	.endm
#endif

#endif  /*  __ASSEMBLY__  */
