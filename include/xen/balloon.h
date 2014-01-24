/******************************************************************************
 * Xen balloon functionality
 */

#define RETRY_UNLIMITED	0

struct balloon_stats {
	/* We aim for 'current allocation' == 'target allocation'. */
	unsigned long current_pages;
	unsigned long target_pages;
	/* Number of pages in high- and low-memory balloons. */
	unsigned long balloon_low;
	unsigned long balloon_high;
	unsigned long schedule_delay;
	unsigned long max_schedule_delay;
	unsigned long retry_count;
	unsigned long max_retry_count;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
	unsigned long hotplug_pages;
	unsigned long balloon_hotplug;
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

extern struct balloon_stats balloon_stats;

void balloon_set_new_target(unsigned long target);

<<<<<<< HEAD
<<<<<<< HEAD
int alloc_xenballooned_pages(int nr_pages, struct page** pages);
void free_xenballooned_pages(int nr_pages, struct page** pages);
=======
=======
>>>>>>> refs/remotes/origin/master
int alloc_xenballooned_pages(int nr_pages, struct page **pages,
		bool highmem);
void free_xenballooned_pages(int nr_pages, struct page **pages);

<<<<<<< HEAD
=======
struct page *get_balloon_scratch_page(void);
void put_balloon_scratch_page(void);

>>>>>>> refs/remotes/origin/master
struct device;
#ifdef CONFIG_XEN_SELFBALLOONING
extern int register_xen_selfballooning(struct device *dev);
#else
static inline int register_xen_selfballooning(struct device *dev)
{
	return -ENOSYS;
}
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
