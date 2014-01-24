#ifndef __MAC80211_DEBUGFS_H
#define __MAC80211_DEBUGFS_H

#ifdef CONFIG_MAC80211_DEBUGFS
<<<<<<< HEAD
extern void debugfs_hw_add(struct ieee80211_local *local);
<<<<<<< HEAD
extern int mac80211_open_file_generic(struct inode *inode, struct file *file);
=======
>>>>>>> refs/remotes/origin/cm-10.0
extern int mac80211_format_buffer(char __user *userbuf, size_t count,
				  loff_t *ppos, char *fmt, ...);
=======
void debugfs_hw_add(struct ieee80211_local *local);
int __printf(4, 5) mac80211_format_buffer(char __user *userbuf, size_t count,
					  loff_t *ppos, char *fmt, ...);
>>>>>>> refs/remotes/origin/master
#else
static inline void debugfs_hw_add(struct ieee80211_local *local)
{
}
#endif

#endif /* __MAC80211_DEBUGFS_H */
