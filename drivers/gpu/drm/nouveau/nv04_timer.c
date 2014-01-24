#include "drmP.h"
#include "drm.h"
#include "nouveau_drv.h"
#include "nouveau_drm.h"
<<<<<<< HEAD
=======
#include "nouveau_hw.h"
>>>>>>> refs/remotes/origin/cm-10.0

int
nv04_timer_init(struct drm_device *dev)
{
<<<<<<< HEAD
	nv_wr32(dev, NV04_PTIMER_INTR_EN_0, 0x00000000);
	nv_wr32(dev, NV04_PTIMER_INTR_0, 0xFFFFFFFF);

	/* Just use the pre-existing values when possible for now; these regs
	 * are not written in nv (driver writer missed a /4 on the address), and
	 * writing 8 and 3 to the correct regs breaks the timings on the LVDS
	 * hardware sequencing microcode.
	 * A correct solution (involving calculations with the GPU PLL) can
	 * be done when kernel modesetting lands
	 */
	if (!nv_rd32(dev, NV04_PTIMER_NUMERATOR) ||
				!nv_rd32(dev, NV04_PTIMER_DENOMINATOR)) {
		nv_wr32(dev, NV04_PTIMER_NUMERATOR, 0x00000008);
		nv_wr32(dev, NV04_PTIMER_DENOMINATOR, 0x00000003);
	}

	return 0;
}

uint64_t
nv04_timer_read(struct drm_device *dev)
{
	uint32_t low;
	/* From kmmio dumps on nv28 this looks like how the blob does this.
	 * It reads the high dword twice, before and after.
	 * The only explanation seems to be that the 64-bit timer counter
	 * advances between high and low dword reads and may corrupt the
	 * result. Not confirmed.
	 */
	uint32_t high2 = nv_rd32(dev, NV04_PTIMER_TIME_1);
	uint32_t high1;
	do {
		high1 = high2;
		low = nv_rd32(dev, NV04_PTIMER_TIME_0);
		high2 = nv_rd32(dev, NV04_PTIMER_TIME_1);
	} while (high1 != high2);
	return (((uint64_t)high2) << 32) | (uint64_t)low;
=======
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	u32 m, n, d;

	nv_wr32(dev, NV04_PTIMER_INTR_EN_0, 0x00000000);
	nv_wr32(dev, NV04_PTIMER_INTR_0, 0xFFFFFFFF);

	/* aim for 31.25MHz, which gives us nanosecond timestamps */
	d = 1000000 / 32;

	/* determine base clock for timer source */
	if (dev_priv->chipset < 0x40) {
		n = nouveau_hw_get_clock(dev, PLL_CORE);
	} else
	if (dev_priv->chipset == 0x40) {
		/*XXX: figure this out */
		n = 0;
	} else {
		n = dev_priv->crystal;
		m = 1;
		while (n < (d * 2)) {
			n += (n / m);
			m++;
		}

		nv_wr32(dev, 0x009220, m - 1);
	}

	if (!n) {
		NV_WARN(dev, "PTIMER: unknown input clock freq\n");
		if (!nv_rd32(dev, NV04_PTIMER_NUMERATOR) ||
		    !nv_rd32(dev, NV04_PTIMER_DENOMINATOR)) {
			nv_wr32(dev, NV04_PTIMER_NUMERATOR, 1);
			nv_wr32(dev, NV04_PTIMER_DENOMINATOR, 1);
		}
		return 0;
	}

	/* reduce ratio to acceptable values */
	while (((n % 5) == 0) && ((d % 5) == 0)) {
		n /= 5;
		d /= 5;
	}

	while (((n % 2) == 0) && ((d % 2) == 0)) {
		n /= 2;
		d /= 2;
	}

	while (n > 0xffff || d > 0xffff) {
		n >>= 1;
		d >>= 1;
	}

	nv_wr32(dev, NV04_PTIMER_NUMERATOR, n);
	nv_wr32(dev, NV04_PTIMER_DENOMINATOR, d);
	return 0;
}

u64
nv04_timer_read(struct drm_device *dev)
{
	u32 hi, lo;

	do {
		hi = nv_rd32(dev, NV04_PTIMER_TIME_1);
		lo = nv_rd32(dev, NV04_PTIMER_TIME_0);
	} while (hi != nv_rd32(dev, NV04_PTIMER_TIME_1));

	return ((u64)hi << 32 | lo);
>>>>>>> refs/remotes/origin/cm-10.0
}

void
nv04_timer_takedown(struct drm_device *dev)
{
}
