/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

<<<<<<< HEAD
#include <mach/msm_subsystem_map.h>
#include <linux/memory_alloc.h>
#include <linux/iommu.h>
#include <linux/vmalloc.h>
#include <asm/sizes.h>
#include <asm/page.h>
#include <linux/init.h>
#include <mach/iommu.h>
#include <mach/iommu_domains.h>
#include <mach/socinfo.h>
=======
#include <linux/module.h>
#include <linux/init.h>
#include <linux/iommu.h>
#include <linux/memory_alloc.h>
#include <linux/platform_device.h>
#include <linux/vmalloc.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <asm/sizes.h>
#include <asm/page.h>
#include <mach/iommu.h>
#include <mach/iommu_domains.h>
#include <mach/socinfo.h>
#include <mach/msm_subsystem_map.h>
>>>>>>> refs/remotes/origin/cm-10.0

/* dummy 64K for overmapping */
char iommu_dummy[2*SZ_64K-4];

<<<<<<< HEAD
struct msm_iommu_domain {
	/* iommu domain to map in */
	struct iommu_domain *domain;
	/* total number of allocations from this domain */
	atomic_t allocation_cnt;
	/* number of iova pools */
	int npools;
	/*
	 * array of gen_pools for allocating iovas.
	 * behavior is undefined if these overlap
	 */
	struct mem_pool *iova_pools;

};


struct {
	char *name;
	int  domain;
} msm_iommu_ctx_names[] = {
	/* Camera */
	{
		.name = "vpe_src",
		.domain = CAMERA_DOMAIN,
	},
	/* Camera */
	{
		.name = "vpe_dst",
		.domain = CAMERA_DOMAIN,
	},
	/* Camera */
	{
		.name = "vfe_imgwr",
		.domain = CAMERA_DOMAIN,
	},
	/* Camera */
	{
		.name = "vfe_misc",
		.domain = CAMERA_DOMAIN,
	},
	/* Camera */
	{
		.name = "ijpeg_src",
		.domain = CAMERA_DOMAIN,
	},
	/* Camera */
	{
		.name = "ijpeg_dst",
		.domain = CAMERA_DOMAIN,
	},
	/* Camera */
	{
		.name = "jpegd_src",
		.domain = CAMERA_DOMAIN,
	},
	/* Camera */
	{
		.name = "jpegd_dst",
		.domain = CAMERA_DOMAIN,
	},
	/* Rotator */
	{
		.name = "rot_src",
		.domain = ROTATOR_DOMAIN,
	},
	/* Rotator */
	{
		.name = "rot_dst",
		.domain = ROTATOR_DOMAIN,
	},
	/* Video */
	{
		.name = "vcodec_a_mm1",
		.domain = VIDEO_DOMAIN,
	},
	/* Video */
	{
		.name = "vcodec_b_mm2",
		.domain = VIDEO_DOMAIN,
	},
	/* Video */
	{
		.name = "vcodec_a_stream",
		.domain = VIDEO_DOMAIN,
	},
};

static struct mem_pool video_pools[] =  {
	/*
	 * Video hardware has the following requirements:
	 * 1. All video addresses used by the video hardware must be at a higher
	 *    address than video firmware address.
	 * 2. Video hardware can only access a range of 256MB from the base of
	 *    the video firmware.
	*/
	[VIDEO_FIRMWARE_POOL] =
	/* Low addresses, intended for video firmware */
		{
			.paddr	= SZ_128K,
			.size	= SZ_16M - SZ_128K,
		},
	[VIDEO_MAIN_POOL] =
	/* Main video pool */
		{
			.paddr	= SZ_16M,
			.size	= SZ_256M - SZ_16M,
		},
	[GEN_POOL] =
	/* Remaining address space up to 2G */
		{
			.paddr	= SZ_256M,
			.size	= SZ_2G - SZ_256M,
		},
};

static struct mem_pool camera_pools[] =  {
	[GEN_POOL] =
	/* One address space for camera */
		{
			.paddr	= SZ_128K,
			.size	= SZ_2G - SZ_128K,
		},
};

static struct mem_pool display_pools[] =  {
	[GEN_POOL] =
	/* One address space for display */
		{
			.paddr	= SZ_128K,
			.size	= SZ_2G - SZ_128K,
		},
};

static struct mem_pool rotator_pools[] =  {
	[GEN_POOL] =
	/* One address space for rotator */
		{
			.paddr	= SZ_128K,
			.size	= SZ_2G - SZ_128K,
		},
};

static struct msm_iommu_domain msm_iommu_domains[] = {
		[VIDEO_DOMAIN] = {
			.iova_pools = video_pools,
			.npools = ARRAY_SIZE(video_pools),
		},
		[CAMERA_DOMAIN] = {
			.iova_pools = camera_pools,
			.npools = ARRAY_SIZE(camera_pools),
		},
		[DISPLAY_DOMAIN] = {
			.iova_pools = display_pools,
			.npools = ARRAY_SIZE(display_pools),
		},
		[ROTATOR_DOMAIN] = {
			.iova_pools = rotator_pools,
			.npools = ARRAY_SIZE(rotator_pools),
		},
};
=======
struct msm_iova_data {
	struct rb_node node;
	struct mem_pool *pools;
	int npools;
	struct iommu_domain *domain;
	int domain_num;
};

static struct rb_root domain_root;
DEFINE_MUTEX(domain_mutex);
static atomic_t domain_nums = ATOMIC_INIT(-1);

int msm_use_iommu()
{
	return iommu_present(&platform_bus_type);
}
>>>>>>> refs/remotes/origin/cm-10.0

int msm_iommu_map_extra(struct iommu_domain *domain,
				unsigned long start_iova,
				unsigned long size,
				unsigned long page_size,
				int cached)
{
<<<<<<< HEAD
	int i, ret_value = 0;
	unsigned long order = get_order(page_size);
	unsigned long aligned_size = ALIGN(size, page_size);
	unsigned long nrpages = aligned_size >> (PAGE_SHIFT + order);
	unsigned long phy_addr = ALIGN(virt_to_phys(iommu_dummy), page_size);
	unsigned long temp_iova = start_iova;

	for (i = 0; i < nrpages; i++) {
		int ret = iommu_map(domain, temp_iova, phy_addr, order, cached);
		if (ret) {
			pr_err("%s: could not map %lx in domain %p, error: %d\n",
				__func__, start_iova, domain, ret);
			ret_value = -EAGAIN;
			goto out;
		}
		temp_iova += page_size;
	}
	return ret_value;
out:
	for (; i > 0; --i) {
		temp_iova -= page_size;
		iommu_unmap(domain, start_iova, order);
	}
	return ret_value;
=======
	int ret = 0;
	int i = 0;
	unsigned long phy_addr = ALIGN(virt_to_phys(iommu_dummy), page_size);
	unsigned long temp_iova = start_iova;
	if (page_size == SZ_4K) {
		struct scatterlist *sglist;
		unsigned int nrpages = PFN_ALIGN(size) >> PAGE_SHIFT;
		struct page *dummy_page = phys_to_page(phy_addr);

		sglist = vmalloc(sizeof(*sglist) * nrpages);
		if (!sglist) {
			ret = -ENOMEM;
			goto out;
		}

		sg_init_table(sglist, nrpages);

		for (i = 0; i < nrpages; i++)
			sg_set_page(&sglist[i], dummy_page, PAGE_SIZE, 0);

		ret = iommu_map_range(domain, temp_iova, sglist, size, cached);
		if (ret) {
			pr_err("%s: could not map extra %lx in domain %p\n",
				__func__, start_iova, domain);
		}

		vfree(sglist);
	} else {
		unsigned long order = get_order(page_size);
		unsigned long aligned_size = ALIGN(size, page_size);
		unsigned long nrpages = aligned_size >> (PAGE_SHIFT + order);

		for (i = 0; i < nrpages; i++) {
			ret = iommu_map(domain, temp_iova, phy_addr, page_size,
						cached);
			if (ret) {
				pr_err("%s: could not map %lx in domain %p, error: %d\n",
					__func__, start_iova, domain, ret);
				ret = -EAGAIN;
				goto out;
			}
			temp_iova += page_size;
		}
	}
	return ret;
out:
	for (; i > 0; --i) {
		temp_iova -= page_size;
		iommu_unmap(domain, start_iova, page_size);
	}
	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}

void msm_iommu_unmap_extra(struct iommu_domain *domain,
				unsigned long start_iova,
				unsigned long size,
				unsigned long page_size)
{
	int i;
	unsigned long order = get_order(page_size);
	unsigned long aligned_size = ALIGN(size, page_size);
	unsigned long nrpages =  aligned_size >> (PAGE_SHIFT + order);
	unsigned long temp_iova = start_iova;

	for (i = 0; i < nrpages; ++i) {
<<<<<<< HEAD
		iommu_unmap(domain, temp_iova, order);
=======
		iommu_unmap(domain, temp_iova, page_size);
>>>>>>> refs/remotes/origin/cm-10.0
		temp_iova += page_size;
	}
}

<<<<<<< HEAD

struct iommu_domain *msm_get_iommu_domain(int domain_num)
{
	if (domain_num >= 0 && domain_num < MAX_DOMAINS)
		return msm_iommu_domains[domain_num].domain;
	else
		return NULL;
}

static unsigned long subsystem_to_domain_tbl[] = {
	VIDEO_DOMAIN,
	VIDEO_DOMAIN,
	CAMERA_DOMAIN,
	DISPLAY_DOMAIN,
	ROTATOR_DOMAIN,
	0xFFFFFFFF
};

unsigned long msm_subsystem_get_domain_no(int subsys_id)
{
	if (subsys_id > INVALID_SUBSYS_ID && subsys_id <= MAX_SUBSYSTEM_ID &&
	    subsys_id < ARRAY_SIZE(subsystem_to_domain_tbl))
		return subsystem_to_domain_tbl[subsys_id];
	else
		return subsystem_to_domain_tbl[MAX_SUBSYSTEM_ID];
}

unsigned long msm_subsystem_get_partition_no(int subsys_id)
{
	switch (subsys_id) {
	case MSM_SUBSYSTEM_VIDEO_FWARE:
		return VIDEO_FIRMWARE_POOL;
	case MSM_SUBSYSTEM_VIDEO:
		return VIDEO_MAIN_POOL;
	case MSM_SUBSYSTEM_CAMERA:
	case MSM_SUBSYSTEM_DISPLAY:
	case MSM_SUBSYSTEM_ROTATOR:
		return GEN_POOL;
	default:
		return 0xFFFFFFFF;
	}
}

unsigned long msm_allocate_iova_address(unsigned int iommu_domain,
					unsigned int partition_no,
					unsigned long size,
					unsigned long align)
{
	struct mem_pool *pool;
	unsigned long iova;

	if (iommu_domain >= MAX_DOMAINS)
		return 0;

	if (partition_no >= msm_iommu_domains[iommu_domain].npools)
		return 0;

	pool = &msm_iommu_domains[iommu_domain].iova_pools[partition_no];

	if (!pool->gpool)
		return 0;

	iova = gen_pool_alloc_aligned(pool->gpool, size, ilog2(align));
	if (iova)
		pool->free -= size;

	return iova;
}

void msm_free_iova_address(unsigned long iova,
			   unsigned int iommu_domain,
			   unsigned int partition_no,
			   unsigned long size)
{
	struct mem_pool *pool;

	if (iommu_domain >= MAX_DOMAINS) {
=======
static int msm_iommu_map_iova_phys(struct iommu_domain *domain,
				unsigned long iova,
				unsigned long phys,
				unsigned long size,
				int cached)
{
	int ret;
	struct scatterlist *sglist;
	int prot = IOMMU_WRITE | IOMMU_READ;
	prot |= cached ? IOMMU_CACHE : 0;

	sglist = vmalloc(sizeof(*sglist));
	if (!sglist) {
		ret = -ENOMEM;
		goto err1;
	}

	sg_init_table(sglist, 1);
	sglist->length = size;
	sglist->offset = 0;
	sglist->dma_address = phys;

	ret = iommu_map_range(domain, iova, sglist, size, prot);
	if (ret) {
		pr_err("%s: could not map extra %lx in domain %p\n",
			__func__, iova, domain);
	}

	vfree(sglist);
err1:
	return ret;

}

int msm_iommu_map_contig_buffer(unsigned long phys,
				unsigned int domain_no,
				unsigned int partition_no,
				unsigned long size,
				unsigned long align,
				unsigned long cached,
				unsigned long *iova_val)
{
	unsigned long iova;
	int ret;

	if (size & (align - 1))
		return -EINVAL;

	if (!msm_use_iommu()) {
		*iova_val = phys;
		return 0;
	}

	ret = msm_allocate_iova_address(domain_no, partition_no, size, align,
						&iova);

	if (ret)
		return -ENOMEM;

	ret = msm_iommu_map_iova_phys(msm_get_iommu_domain(domain_no), iova,
					phys, size, cached);

	if (ret)
		msm_free_iova_address(iova, domain_no, partition_no, size);
	else
		*iova_val = iova;

	return ret;
}

void msm_iommu_unmap_contig_buffer(unsigned long iova,
					unsigned int domain_no,
					unsigned int partition_no,
					unsigned long size)
{
	if (!msm_use_iommu())
		return;

	iommu_unmap_range(msm_get_iommu_domain(domain_no), iova, size);
	msm_free_iova_address(iova, domain_no, partition_no, size);
}

static struct msm_iova_data *find_domain(int domain_num)
{
	struct rb_root *root = &domain_root;
	struct rb_node *p = root->rb_node;

	mutex_lock(&domain_mutex);

	while (p) {
		struct msm_iova_data *node;

		node = rb_entry(p, struct msm_iova_data, node);
		if (domain_num < node->domain_num)
			p = p->rb_left;
		else if (domain_num > node->domain_num)
			p = p->rb_right;
		else {
			mutex_unlock(&domain_mutex);
			return node;
		}
	}
	mutex_unlock(&domain_mutex);
	return NULL;
}

static int add_domain(struct msm_iova_data *node)
{
	struct rb_root *root = &domain_root;
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;

	mutex_lock(&domain_mutex);
	while (*p) {
		struct msm_iova_data *tmp;
		parent = *p;

		tmp = rb_entry(parent, struct msm_iova_data, node);

		if (node->domain_num < tmp->domain_num)
			p = &(*p)->rb_left;
		else if (node->domain_num > tmp->domain_num)
			p = &(*p)->rb_right;
		else
			BUG();
	}
	rb_link_node(&node->node, parent, p);
	rb_insert_color(&node->node, root);
	mutex_unlock(&domain_mutex);
	return 0;
}

struct iommu_domain *msm_get_iommu_domain(int domain_num)
{
	struct msm_iova_data *data;

	data = find_domain(domain_num);

	if (data)
		return data->domain;
	else
		return NULL;
}

int msm_allocate_iova_address(unsigned int iommu_domain,
					unsigned int partition_no,
					unsigned long size,
					unsigned long align,
					unsigned long *iova)
{
	struct msm_iova_data *data;
	struct mem_pool *pool;
	unsigned long va;

	data = find_domain(iommu_domain);

	if (!data)
		return -EINVAL;

	if (partition_no >= data->npools)
		return -EINVAL;

	pool = &data->pools[partition_no];

	if (!pool->gpool)
		return -EINVAL;

	va = gen_pool_alloc_aligned(pool->gpool, size, ilog2(align));
	if (va) {
		pool->free -= size;
		/* Offset because genpool can't handle 0 addresses */
		if (pool->paddr == 0)
			va -= SZ_4K;
		*iova = va;
		return 0;
	}

	return -ENOMEM;
}

void msm_free_iova_address(unsigned long iova,
				unsigned int iommu_domain,
				unsigned int partition_no,
				unsigned long size)
{
	struct msm_iova_data *data;
	struct mem_pool *pool;

	data = find_domain(iommu_domain);

	if (!data) {
>>>>>>> refs/remotes/origin/cm-10.0
		WARN(1, "Invalid domain %d\n", iommu_domain);
		return;
	}

<<<<<<< HEAD
	if (partition_no >= msm_iommu_domains[iommu_domain].npools) {
=======
	if (partition_no >= data->npools) {
>>>>>>> refs/remotes/origin/cm-10.0
		WARN(1, "Invalid partition %d for domain %d\n",
			partition_no, iommu_domain);
		return;
	}

<<<<<<< HEAD
	pool = &msm_iommu_domains[iommu_domain].iova_pools[partition_no];
=======
	pool = &data->pools[partition_no];
>>>>>>> refs/remotes/origin/cm-10.0

	if (!pool)
		return;

	pool->free += size;
<<<<<<< HEAD
	gen_pool_free(pool->gpool, iova, size);
}

int msm_use_iommu()
{
	return iommu_found();
}

static int __init msm_subsystem_iommu_init(void)
{
	int i, j;

	for (i = 0; i < ARRAY_SIZE(msm_iommu_domains); i++) {
		msm_iommu_domains[i].domain = iommu_domain_alloc(0);
		if (!msm_iommu_domains[i].domain)
			continue;

		for (j = 0; j < msm_iommu_domains[i].npools; j++) {
			struct mem_pool *pool = &msm_iommu_domains[i].
							iova_pools[j];
			mutex_init(&pool->pool_mutex);
			if (pool->size) {
				pool->gpool = gen_pool_create(PAGE_SHIFT, -1);

				if (!pool->gpool) {
					pr_err("%s: could not allocate pool\n",
						__func__);
					pr_err("%s: domain %d iova space %d\n",
						__func__, i, j);
					continue;
				}

				if (gen_pool_add(pool->gpool, pool->paddr,
						pool->size, -1)) {
					pr_err("%s: could not add memory\n",
						__func__);
					pr_err("%s: domain %d pool %d\n",
						__func__, i, j);
					gen_pool_destroy(pool->gpool);
					pool->gpool = NULL;
					continue;
				}
			} else {
				pool->gpool = NULL;
			}
		}
	}

	for (i = 0; i < ARRAY_SIZE(msm_iommu_ctx_names); i++) {
		int domain_idx;
		struct device *ctx = msm_iommu_get_ctx(
						msm_iommu_ctx_names[i].name);
=======

	/* Offset because genpool can't handle 0 addresses */
	if (pool->paddr == 0)
		iova += SZ_4K;

	gen_pool_free(pool->gpool, iova, size);
}

int msm_register_domain(struct msm_iova_layout *layout)
{
	int i;
	struct msm_iova_data *data;
	struct mem_pool *pools;

	if (!layout)
		return -EINVAL;

	data = kmalloc(sizeof(*data), GFP_KERNEL);

	if (!data)
		return -ENOMEM;

	pools = kmalloc(sizeof(struct mem_pool) * layout->npartitions,
			GFP_KERNEL);

	if (!pools)
		goto out;

	for (i = 0; i < layout->npartitions; i++) {
		if (layout->partitions[i].size == 0)
			continue;

		pools[i].gpool = gen_pool_create(PAGE_SHIFT, -1);

		if (!pools[i].gpool)
			continue;

		pools[i].paddr = layout->partitions[i].start;
		pools[i].size = layout->partitions[i].size;

		/*
		 * genalloc can't handle a pool starting at address 0.
		 * For now, solve this problem by offsetting the value
		 * put in by 4k.
		 * gen pool address = actual address + 4k
		 */
		if (pools[i].paddr == 0)
			layout->partitions[i].start += SZ_4K;

		if (gen_pool_add(pools[i].gpool,
			layout->partitions[i].start,
			layout->partitions[i].size, -1)) {
			gen_pool_destroy(pools[i].gpool);
			pools[i].gpool = NULL;
			continue;
		}
	}

	data->pools = pools;
	data->npools = layout->npartitions;
	data->domain_num = atomic_inc_return(&domain_nums);
	data->domain = iommu_domain_alloc(&platform_bus_type,
					  layout->domain_flags);

	add_domain(data);

	return data->domain_num;

out:
	kfree(data);

	return -EINVAL;
}

static int __init iommu_domain_probe(struct platform_device *pdev)
{
	struct iommu_domains_pdata *p  = pdev->dev.platform_data;
	int i, j;

	if (!msm_use_iommu())
		return -ENODEV;

	if (!p)
		return -ENODEV;

	for (i = 0; i < p->ndomains; i++) {
		struct msm_iova_layout l;
		struct msm_iova_partition *part;
		struct msm_iommu_domain *domains;

		domains = p->domains;
		l.npartitions = domains[i].npools;
		part = kmalloc(
			sizeof(struct msm_iova_partition) * l.npartitions,
				GFP_KERNEL);

		if (!part) {
			pr_info("%s: could not allocate space for domain %d",
				__func__, i);
			continue;
		}

		for (j = 0; j < l.npartitions; j++) {
			part[j].start = p->domains[i].iova_pools[j].paddr;
			part[j].size = p->domains[i].iova_pools[j].size;
		}

		l.partitions = part;

		msm_register_domain(&l);

		kfree(part);
	}

	for (i = 0; i < p->nnames; i++) {
		struct device *ctx = msm_iommu_get_ctx(
						p->domain_names[i].name);
		struct iommu_domain *domain;
>>>>>>> refs/remotes/origin/cm-10.0

		if (!ctx)
			continue;

<<<<<<< HEAD
		domain_idx = msm_iommu_ctx_names[i].domain;

		if (!msm_iommu_domains[domain_idx].domain)
			continue;

		if (iommu_attach_device(msm_iommu_domains[domain_idx].domain,
					ctx)) {
			WARN(1, "%s: could not attach domain %d to context %s."
				" iommu programming will not occur.\n",
				__func__, domain_idx,
				msm_iommu_ctx_names[i].name);
=======
		domain = msm_get_iommu_domain(p->domain_names[i].domain);

		if (!domain)
			continue;

		if (iommu_attach_device(domain, ctx)) {
			WARN(1, "%s: could not attach domain %p to context %s."
				" iommu programming will not occur.\n",
				__func__, domain,
				p->domain_names[i].name);
>>>>>>> refs/remotes/origin/cm-10.0
			continue;
		}
	}

	return 0;
}
<<<<<<< HEAD
=======

static struct platform_driver iommu_domain_driver = {
	.driver         = {
		.name = "iommu_domains",
		.owner = THIS_MODULE
	},
};

static int __init msm_subsystem_iommu_init(void)
{
	return platform_driver_probe(&iommu_domain_driver, iommu_domain_probe);
}
>>>>>>> refs/remotes/origin/cm-10.0
device_initcall(msm_subsystem_iommu_init);
