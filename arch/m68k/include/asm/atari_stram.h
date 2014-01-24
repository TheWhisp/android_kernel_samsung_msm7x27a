#ifndef _M68K_ATARI_STRAM_H
#define _M68K_ATARI_STRAM_H

/*
 * Functions for Atari ST-RAM management
 */

/* public interface */
<<<<<<< HEAD
void *atari_stram_alloc(long size, const char *owner);
=======
void *atari_stram_alloc(unsigned long size, const char *owner);
>>>>>>> refs/remotes/origin/cm-10.0
void atari_stram_free(void *);

/* functions called internally by other parts of the kernel */
void atari_stram_init(void);
void atari_stram_reserve_pages(void *start_mem);
<<<<<<< HEAD
void atari_stram_mem_init_hook (void);
=======
>>>>>>> refs/remotes/origin/cm-10.0

#endif /*_M68K_ATARI_STRAM_H */
