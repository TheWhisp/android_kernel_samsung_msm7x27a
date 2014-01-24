#include <linux/fsnotify_backend.h>
#include <linux/inotify.h>
#include <linux/slab.h> /* struct kmem_cache */

<<<<<<< HEAD
extern struct kmem_cache *event_priv_cachep;

struct inotify_event_private_data {
	struct fsnotify_event_private_data fsnotify_event_priv_data;
	int wd;
=======
struct inotify_event_info {
	struct fsnotify_event fse;
	int wd;
	u32 sync_cookie;
	int name_len;
	char name[];
>>>>>>> refs/remotes/origin/master
};

struct inotify_inode_mark {
	struct fsnotify_mark fsn_mark;
	int wd;
};

<<<<<<< HEAD
extern void inotify_ignored_and_remove_idr(struct fsnotify_mark *fsn_mark,
					   struct fsnotify_group *group);
extern void inotify_free_event_priv(struct fsnotify_event_private_data *event_priv);
=======
static inline struct inotify_event_info *INOTIFY_E(struct fsnotify_event *fse)
{
	return container_of(fse, struct inotify_event_info, fse);
}

extern void inotify_ignored_and_remove_idr(struct fsnotify_mark *fsn_mark,
					   struct fsnotify_group *group);
extern int inotify_handle_event(struct fsnotify_group *group,
				struct inode *inode,
				struct fsnotify_mark *inode_mark,
				struct fsnotify_mark *vfsmount_mark,
				u32 mask, void *data, int data_type,
				const unsigned char *file_name);
>>>>>>> refs/remotes/origin/master

extern const struct fsnotify_ops inotify_fsnotify_ops;
