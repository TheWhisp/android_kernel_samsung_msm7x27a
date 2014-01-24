	.text
	.globl __kernel_vsyscall
	.type __kernel_vsyscall,@function
__kernel_vsyscall:
.LSTART_vsyscall:
<<<<<<< HEAD
<<<<<<< HEAD
	/* XXX: We'll have to do something here once we opt to use the vDSO
	 * page for something other than the signal trampoline.. as well as
	 * fill out .eh_frame -- PFM. */
.LEND_vsyscall:
	.size __kernel_vsyscall,.-.LSTART_vsyscall

	.section .eh_frame,"a",@progbits
	.previous
=======
=======
>>>>>>> refs/remotes/origin/master
	trapa	#0x10
	 nop
.LEND_vsyscall:
	.size __kernel_vsyscall,.-.LSTART_vsyscall
	.previous

	.section .eh_frame,"a",@progbits
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
.LCIE:
	.ualong	.LCIE_end - .LCIE_start
.LCIE_start:
	.ualong	0		/* CIE ID */
	.byte	0x1		/* Version number */
<<<<<<< HEAD
<<<<<<< HEAD
	.string	"zRS"		/* NUL-terminated augmentation string */
	.uleb128 0x1		/* Code alignment factor */
	.sleb128 -4		/* Data alignment factor */
	.byte	0x11		/* Return address register column */
				/* Augmentation length and data (none) */
	.byte	0xc		/* DW_CFA_def_cfa */
	.uleb128 0xf		/* r15 */
	.uleb128 0x0		/* offset 0 */

=======
=======
>>>>>>> refs/remotes/origin/master
	.string	"zR"		/* NUL-terminated augmentation string */
	.uleb128 0x1		/* Code alignment factor */
	.sleb128 -4		/* Data alignment factor */
	.byte	0x11		/* Return address register column */
	.uleb128 0x1		/* Augmentation length and data */
	.byte 0x1b              /* DW_EH_PE_pcrel | DW_EH_PE_sdata4. */
	.byte	0xc,0xf,0x0	/* DW_CFA_def_cfa: r15 ofs 0 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.align 2
.LCIE_end:

	.ualong	.LFDE_end-.LFDE_start	/* Length FDE */
.LFDE_start:
<<<<<<< HEAD
<<<<<<< HEAD
	.ualong	.LCIE			/* CIE pointer */
	.ualong	.LSTART_vsyscall-.	/* start address */
	.ualong	.LEND_vsyscall-.LSTART_vsyscall
	.uleb128 0
=======
=======
>>>>>>> refs/remotes/origin/master
	.ualong	.LFDE_start-.LCIE	/* CIE pointer */
	.ualong	.LSTART_vsyscall-.	/* PC-relative start address */
	.ualong	.LEND_vsyscall-.LSTART_vsyscall
	.uleb128 0			/* Augmentation */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.align 2
.LFDE_end:
	.previous

/* Get the common code for the sigreturn entry points */
#include "vsyscall-sigreturn.S"
