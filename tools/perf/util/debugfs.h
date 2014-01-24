#ifndef __DEBUGFS_H__
#define __DEBUGFS_H__

<<<<<<< HEAD
#include <sys/mount.h>

#ifndef MAX_PATH
# define MAX_PATH 256
#endif

#ifndef STR
# define _STR(x) #x
# define STR(x) _STR(x)
#endif

extern const char *debugfs_find_mountpoint(void);
extern int debugfs_valid_mountpoint(const char *debugfs);
extern int debugfs_valid_entry(const char *path);
extern char *debugfs_mount(const char *mountpoint);
extern int debugfs_umount(void);
extern int debugfs_write(const char *entry, const char *value);
extern int debugfs_read(const char *entry, char *buffer, size_t size);
extern void debugfs_force_cleanup(void);
extern int debugfs_make_path(const char *element, char *buffer, int size);
=======
const char *debugfs_find_mountpoint(void);
int debugfs_valid_mountpoint(const char *debugfs);
char *debugfs_mount(const char *mountpoint);
void debugfs_set_path(const char *mountpoint);

extern char debugfs_mountpoint[];
extern char tracing_events_path[];
>>>>>>> refs/remotes/origin/cm-10.0

#endif /* __DEBUGFS_H__ */
