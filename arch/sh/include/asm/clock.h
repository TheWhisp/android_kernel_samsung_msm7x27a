#ifndef __ASM_SH_CLOCK_H
#define __ASM_SH_CLOCK_H

#include <linux/sh_clk.h>

/* Should be defined by processor-specific code */
<<<<<<< HEAD
<<<<<<< HEAD
void __deprecated arch_init_clk_ops(struct clk_ops **, int type);
=======
void __deprecated arch_init_clk_ops(struct sh_clk_ops **, int type);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void __deprecated arch_init_clk_ops(struct sh_clk_ops **, int type);
>>>>>>> refs/remotes/origin/master
int __init arch_clk_init(void);

/* arch/sh/kernel/cpu/clock-cpg.c */
int __init __deprecated cpg_clk_init(void);

/* arch/sh/kernel/cpu/clock.c */
int clk_init(void);

#endif /* __ASM_SH_CLOCK_H */
