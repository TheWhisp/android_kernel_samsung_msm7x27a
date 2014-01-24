#include <linux/highmem.h>

static inline void *kmap_skb_frag(const skb_frag_t *frag)
{
#ifdef CONFIG_HIGHMEM
	BUG_ON(in_irq());

	local_bh_disable();
#endif
<<<<<<< HEAD
	return kmap_atomic(frag->page, KM_SKB_DATA_SOFTIRQ);
=======
	return kmap_atomic(skb_frag_page(frag));
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline void kunmap_skb_frag(void *vaddr)
{
<<<<<<< HEAD
	kunmap_atomic(vaddr, KM_SKB_DATA_SOFTIRQ);
=======
	kunmap_atomic(vaddr);
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_HIGHMEM
	local_bh_enable();
#endif
}
