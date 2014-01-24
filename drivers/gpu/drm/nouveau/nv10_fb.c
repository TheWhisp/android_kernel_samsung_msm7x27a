#include "drmP.h"
#include "drm.h"
#include "nouveau_drv.h"
#include "nouveau_drm.h"

<<<<<<< HEAD
static struct drm_mm_node *
nv20_fb_alloc_tag(struct drm_device *dev, uint32_t size)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_fb_engine *pfb = &dev_priv->engine.fb;
	struct drm_mm_node *mem;
	int ret;

	ret = drm_mm_pre_get(&pfb->tag_heap);
	if (ret)
		return NULL;

	spin_lock(&dev_priv->tile.lock);
	mem = drm_mm_search_free(&pfb->tag_heap, size, 0, 0);
	if (mem)
		mem = drm_mm_get_block_atomic(mem, size, 0);
	spin_unlock(&dev_priv->tile.lock);

	return mem;
}

static void
nv20_fb_free_tag(struct drm_device *dev, struct drm_mm_node *mem)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;

	spin_lock(&dev_priv->tile.lock);
	drm_mm_put_block(mem);
	spin_unlock(&dev_priv->tile.lock);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
void
nv10_fb_init_tile_region(struct drm_device *dev, int i, uint32_t addr,
			 uint32_t size, uint32_t pitch, uint32_t flags)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_tile_reg *tile = &dev_priv->tile.reg[i];
<<<<<<< HEAD
	int bpp = (flags & NOUVEAU_GEM_TILE_32BPP ? 32 : 16);

	tile->addr = addr;
	tile->limit = max(1u, addr + size) - 1;
	tile->pitch = pitch;

	if (dev_priv->card_type == NV_20) {
		if (flags & NOUVEAU_GEM_TILE_ZETA) {
			/*
			 * Allocate some of the on-die tag memory,
			 * used to store Z compression meta-data (most
			 * likely just a bitmap determining if a given
			 * tile is compressed or not).
			 */
			tile->tag_mem = nv20_fb_alloc_tag(dev, size / 256);

			if (tile->tag_mem) {
				/* Enable Z compression */
				if (dev_priv->chipset >= 0x25)
					tile->zcomp = tile->tag_mem->start |
						(bpp == 16 ?
						 NV25_PFB_ZCOMP_MODE_16 :
						 NV25_PFB_ZCOMP_MODE_32);
				else
					tile->zcomp = tile->tag_mem->start |
						NV20_PFB_ZCOMP_EN |
						(bpp == 16 ? 0 :
						 NV20_PFB_ZCOMP_MODE_32);
			}

			tile->addr |= 3;
		} else {
			tile->addr |= 1;
		}

	} else {
		tile->addr |= 1 << 31;
	}
=======

	tile->addr  = 0x80000000 | addr;
	tile->limit = max(1u, addr + size) - 1;
	tile->pitch = pitch;
>>>>>>> refs/remotes/origin/cm-10.0
}

void
nv10_fb_free_tile_region(struct drm_device *dev, int i)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_tile_reg *tile = &dev_priv->tile.reg[i];

<<<<<<< HEAD
	if (tile->tag_mem) {
		nv20_fb_free_tag(dev, tile->tag_mem);
		tile->tag_mem = NULL;
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
	tile->addr = tile->limit = tile->pitch = tile->zcomp = 0;
}

void
nv10_fb_set_tile_region(struct drm_device *dev, int i)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_tile_reg *tile = &dev_priv->tile.reg[i];

	nv_wr32(dev, NV10_PFB_TLIMIT(i), tile->limit);
	nv_wr32(dev, NV10_PFB_TSIZE(i), tile->pitch);
	nv_wr32(dev, NV10_PFB_TILE(i), tile->addr);
<<<<<<< HEAD

	if (dev_priv->card_type == NV_20)
		nv_wr32(dev, NV20_PFB_ZCOMP(i), tile->zcomp);
=======
}

int
nv1a_fb_vram_init(struct drm_device *dev)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct pci_dev *bridge;
	uint32_t mem, mib;

	bridge = pci_get_bus_and_slot(0, PCI_DEVFN(0, 1));
	if (!bridge) {
		NV_ERROR(dev, "no bridge device\n");
		return 0;
	}

	if (dev_priv->chipset == 0x1a) {
		pci_read_config_dword(bridge, 0x7c, &mem);
		mib = ((mem >> 6) & 31) + 1;
	} else {
		pci_read_config_dword(bridge, 0x84, &mem);
		mib = ((mem >> 4) & 127) + 1;
	}

	dev_priv->vram_size = mib * 1024 * 1024;
	return 0;
}

int
nv10_fb_vram_init(struct drm_device *dev)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	u32 fifo_data = nv_rd32(dev, NV04_PFB_FIFO_DATA);
	u32 cfg0 = nv_rd32(dev, 0x100200);

	dev_priv->vram_size = fifo_data & NV10_PFB_FIFO_DATA_RAM_AMOUNT_MB_MASK;

	if (cfg0 & 0x00000001)
		dev_priv->vram_type = NV_MEM_TYPE_DDR1;
	else
		dev_priv->vram_type = NV_MEM_TYPE_SDRAM;

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

int
nv10_fb_init(struct drm_device *dev)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_fb_engine *pfb = &dev_priv->engine.fb;
	int i;

<<<<<<< HEAD
	pfb->num_tiles = NV10_PFB_TILE__SIZE;

	if (dev_priv->card_type == NV_20)
		drm_mm_init(&pfb->tag_heap, 0,
			    (dev_priv->chipset >= 0x25 ?
			     64 * 1024 : 32 * 1024));

	/* Turn all the tiling regions off. */
=======
	/* Turn all the tiling regions off. */
	pfb->num_tiles = NV10_PFB_TILE__SIZE;
>>>>>>> refs/remotes/origin/cm-10.0
	for (i = 0; i < pfb->num_tiles; i++)
		pfb->set_tile_region(dev, i);

	return 0;
}

void
nv10_fb_takedown(struct drm_device *dev)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_fb_engine *pfb = &dev_priv->engine.fb;
	int i;

	for (i = 0; i < pfb->num_tiles; i++)
		pfb->free_tile_region(dev, i);
<<<<<<< HEAD

	if (dev_priv->card_type == NV_20)
		drm_mm_takedown(&pfb->tag_heap);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}
