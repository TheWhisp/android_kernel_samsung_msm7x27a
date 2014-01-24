/* This handles the memory map.. */

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_MMU
#ifndef CONFIG_SUN3
#define PAGE_OFFSET_RAW		0x00000000
#else
#define PAGE_OFFSET_RAW		0x0E000000
#endif
#else
#define	PAGE_OFFSET_RAW		CONFIG_RAMBASE
=======
=======
>>>>>>> refs/remotes/origin/master
#if defined(CONFIG_RAMBASE)
#define PAGE_OFFSET_RAW		CONFIG_RAMBASE
#elif defined(CONFIG_SUN3)
#define PAGE_OFFSET_RAW		0x0E000000
#else
#define PAGE_OFFSET_RAW		0x00000000
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif
