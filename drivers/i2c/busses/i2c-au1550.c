/*
 * i2c-au1550.c: SMBus (i2c) adapter for Alchemy PSC interface
 * Copyright (C) 2004 Embedded Edge, LLC <dan@embeddededge.com>
 *
 * 2.6 port by Matt Porter <mporter@kernel.crashing.org>
 *
 * The documentation describes this as an SMBus controller, but it doesn't
 * understand any of the SMBus protocol in hardware.  It's really an I2C
 * controller that could emulate most of the SMBus in software.
 *
 * This is just a skeleton adapter to use with the Au1550 PSC
 * algorithm.  It was developed for the Pb1550, but will work with
 * any Au1550 board that has a similar PSC configuration.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/slab.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/mach-au1x00/au1xxx.h>
#include <asm/mach-au1x00/au1xxx_psc.h>

struct i2c_au1550_data {
	u32	psc_base;
	int	xfer_timeout;
	int	ack_timeout;
=======
=======
>>>>>>> refs/remotes/origin/master
#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-au1x00/au1xxx_psc.h>

#define PSC_SEL		0x00
#define PSC_CTRL	0x04
#define PSC_SMBCFG	0x08
#define PSC_SMBMSK	0x0C
#define PSC_SMBPCR	0x10
#define PSC_SMBSTAT	0x14
#define PSC_SMBEVNT	0x18
#define PSC_SMBTXRX	0x1C
#define PSC_SMBTMR	0x20

struct i2c_au1550_data {
	void __iomem *psc_base;
	int	xfer_timeout;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct i2c_adapter adap;
	struct resource *ioarea;
};

<<<<<<< HEAD
<<<<<<< HEAD
static int
wait_xfer_done(struct i2c_au1550_data *adap)
{
	u32	stat;
	int	i;
	volatile psc_smb_t	*sp;

	sp = (volatile psc_smb_t *)(adap->psc_base);

	/* Wait for Tx Buffer Empty
	*/
	for (i = 0; i < adap->xfer_timeout; i++) {
		stat = sp->psc_smbstat;
		au_sync();
		if ((stat & PSC_SMBSTAT_TE) != 0)
=======
=======
>>>>>>> refs/remotes/origin/master
static inline void WR(struct i2c_au1550_data *a, int r, unsigned long v)
{
	__raw_writel(v, a->psc_base + r);
	wmb();
}

static inline unsigned long RD(struct i2c_au1550_data *a, int r)
{
	return __raw_readl(a->psc_base + r);
}

static int wait_xfer_done(struct i2c_au1550_data *adap)
{
	int i;

	/* Wait for Tx Buffer Empty */
	for (i = 0; i < adap->xfer_timeout; i++) {
		if (RD(adap, PSC_SMBSTAT) & PSC_SMBSTAT_TE)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			return 0;

		udelay(1);
	}

	return -ETIMEDOUT;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int
wait_ack(struct i2c_au1550_data *adap)
{
	u32	stat;
	volatile psc_smb_t	*sp;
=======
static int wait_ack(struct i2c_au1550_data *adap)
{
	unsigned long stat;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int wait_ack(struct i2c_au1550_data *adap)
{
	unsigned long stat;
>>>>>>> refs/remotes/origin/master

	if (wait_xfer_done(adap))
		return -ETIMEDOUT;

<<<<<<< HEAD
<<<<<<< HEAD
	sp = (volatile psc_smb_t *)(adap->psc_base);

	stat = sp->psc_smbevnt;
	au_sync();

=======
	stat = RD(adap, PSC_SMBEVNT);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	stat = RD(adap, PSC_SMBEVNT);
>>>>>>> refs/remotes/origin/master
	if ((stat & (PSC_SMBEVNT_DN | PSC_SMBEVNT_AN | PSC_SMBEVNT_AL)) != 0)
		return -ETIMEDOUT;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int
wait_master_done(struct i2c_au1550_data *adap)
{
	u32	stat;
	int	i;
	volatile psc_smb_t	*sp;

	sp = (volatile psc_smb_t *)(adap->psc_base);

	/* Wait for Master Done.
	*/
	for (i = 0; i < adap->xfer_timeout; i++) {
		stat = sp->psc_smbevnt;
		au_sync();
		if ((stat & PSC_SMBEVNT_MD) != 0)
=======
=======
>>>>>>> refs/remotes/origin/master
static int wait_master_done(struct i2c_au1550_data *adap)
{
	int i;

	/* Wait for Master Done. */
	for (i = 0; i < 2 * adap->xfer_timeout; i++) {
		if ((RD(adap, PSC_SMBEVNT) & PSC_SMBEVNT_MD) != 0)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			return 0;
		udelay(1);
	}

	return -ETIMEDOUT;
}

static int
do_address(struct i2c_au1550_data *adap, unsigned int addr, int rd, int q)
{
<<<<<<< HEAD
<<<<<<< HEAD
	volatile psc_smb_t	*sp;
	u32			stat;

	sp = (volatile psc_smb_t *)(adap->psc_base);

	/* Reset the FIFOs, clear events.
	*/
	stat = sp->psc_smbstat;
	sp->psc_smbevnt = PSC_SMBEVNT_ALLCLR;
	au_sync();

	if (!(stat & PSC_SMBSTAT_TE) || !(stat & PSC_SMBSTAT_RE)) {
		sp->psc_smbpcr = PSC_SMBPCR_DC;
		au_sync();
		do {
			stat = sp->psc_smbpcr;
			au_sync();
		} while ((stat & PSC_SMBPCR_DC) != 0);
		udelay(50);
	}

	/* Write out the i2c chip address and specify operation
	*/
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long stat;

	/* Reset the FIFOs, clear events. */
	stat = RD(adap, PSC_SMBSTAT);
	WR(adap, PSC_SMBEVNT, PSC_SMBEVNT_ALLCLR);

	if (!(stat & PSC_SMBSTAT_TE) || !(stat & PSC_SMBSTAT_RE)) {
		WR(adap, PSC_SMBPCR, PSC_SMBPCR_DC);
		while ((RD(adap, PSC_SMBPCR) & PSC_SMBPCR_DC) != 0)
			cpu_relax();
		udelay(50);
	}

	/* Write out the i2c chip address and specify operation */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	addr <<= 1;
	if (rd)
		addr |= 1;

	/* zero-byte xfers stop immediately */
	if (q)
		addr |= PSC_SMBTXRX_STP;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Put byte into fifo, start up master.
	*/
	sp->psc_smbtxrx = addr;
	au_sync();
	sp->psc_smbpcr = PSC_SMBPCR_MS;
	au_sync();
=======
	/* Put byte into fifo, start up master. */
	WR(adap, PSC_SMBTXRX, addr);
	WR(adap, PSC_SMBPCR, PSC_SMBPCR_MS);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Put byte into fifo, start up master. */
	WR(adap, PSC_SMBTXRX, addr);
	WR(adap, PSC_SMBPCR, PSC_SMBPCR_MS);
>>>>>>> refs/remotes/origin/master
	if (wait_ack(adap))
		return -EIO;
	return (q) ? wait_master_done(adap) : 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32
wait_for_rx_byte(struct i2c_au1550_data *adap, u32 *ret_data)
{
	int	j;
	u32	data, stat;
	volatile psc_smb_t	*sp;
=======
static int wait_for_rx_byte(struct i2c_au1550_data *adap, unsigned char *out)
{
	int j;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int wait_for_rx_byte(struct i2c_au1550_data *adap, unsigned char *out)
{
	int j;
>>>>>>> refs/remotes/origin/master

	if (wait_xfer_done(adap))
		return -EIO;

<<<<<<< HEAD
<<<<<<< HEAD
	sp = (volatile psc_smb_t *)(adap->psc_base);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	j =  adap->xfer_timeout * 100;
	do {
		j--;
		if (j <= 0)
			return -EIO;

<<<<<<< HEAD
<<<<<<< HEAD
		stat = sp->psc_smbstat;
		au_sync();
		if ((stat & PSC_SMBSTAT_RE) == 0)
=======
		if ((RD(adap, PSC_SMBSTAT) & PSC_SMBSTAT_RE) == 0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if ((RD(adap, PSC_SMBSTAT) & PSC_SMBSTAT_RE) == 0)
>>>>>>> refs/remotes/origin/master
			j = 0;
		else
			udelay(1);
	} while (j > 0);
<<<<<<< HEAD
<<<<<<< HEAD
	data = sp->psc_smbtxrx;
	au_sync();
	*ret_data = data;
=======

	*out = RD(adap, PSC_SMBTXRX);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	*out = RD(adap, PSC_SMBTXRX);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int
i2c_read(struct i2c_au1550_data *adap, unsigned char *buf,
		    unsigned int len)
{
	int	i;
	u32	data;
	volatile psc_smb_t	*sp;
=======
=======
>>>>>>> refs/remotes/origin/master
static int i2c_read(struct i2c_au1550_data *adap, unsigned char *buf,
		    unsigned int len)
{
	int i;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (len == 0)
		return 0;

	/* A read is performed by stuffing the transmit fifo with
	 * zero bytes for timing, waiting for bytes to appear in the
	 * receive fifo, then reading the bytes.
	 */
<<<<<<< HEAD
<<<<<<< HEAD

	sp = (volatile psc_smb_t *)(adap->psc_base);

	i = 0;
	while (i < (len-1)) {
		sp->psc_smbtxrx = 0;
		au_sync();
		if (wait_for_rx_byte(adap, &data))
			return -EIO;

		buf[i] = data;
		i++;
	}

	/* The last byte has to indicate transfer done.
	*/
	sp->psc_smbtxrx = PSC_SMBTXRX_STP;
	au_sync();
	if (wait_master_done(adap))
		return -EIO;

	data = sp->psc_smbtxrx;
	au_sync();
	buf[i] = data;
	return 0;
}

static int
i2c_write(struct i2c_au1550_data *adap, unsigned char *buf,
		     unsigned int len)
{
	int	i;
	u32	data;
	volatile psc_smb_t	*sp;
=======
=======
>>>>>>> refs/remotes/origin/master
	i = 0;
	while (i < (len - 1)) {
		WR(adap, PSC_SMBTXRX, 0);
		if (wait_for_rx_byte(adap, &buf[i]))
			return -EIO;

		i++;
	}

	/* The last byte has to indicate transfer done. */
	WR(adap, PSC_SMBTXRX, PSC_SMBTXRX_STP);
	if (wait_master_done(adap))
		return -EIO;

	buf[i] = (unsigned char)(RD(adap, PSC_SMBTXRX) & 0xff);
	return 0;
}

static int i2c_write(struct i2c_au1550_data *adap, unsigned char *buf,
		     unsigned int len)
{
	int i;
	unsigned long data;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (len == 0)
		return 0;

<<<<<<< HEAD
<<<<<<< HEAD
	sp = (volatile psc_smb_t *)(adap->psc_base);

	i = 0;
	while (i < (len-1)) {
		data = buf[i];
		sp->psc_smbtxrx = data;
		au_sync();
=======
=======
>>>>>>> refs/remotes/origin/master
	i = 0;
	while (i < (len-1)) {
		data = buf[i];
		WR(adap, PSC_SMBTXRX, data);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (wait_ack(adap))
			return -EIO;
		i++;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* The last byte has to indicate transfer done.
	*/
	data = buf[i];
	data |= PSC_SMBTXRX_STP;
	sp->psc_smbtxrx = data;
	au_sync();
=======
=======
>>>>>>> refs/remotes/origin/master
	/* The last byte has to indicate transfer done. */
	data = buf[i];
	data |= PSC_SMBTXRX_STP;
	WR(adap, PSC_SMBTXRX, data);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (wait_master_done(adap))
		return -EIO;
	return 0;
}

static int
au1550_xfer(struct i2c_adapter *i2c_adap, struct i2c_msg *msgs, int num)
{
	struct i2c_au1550_data *adap = i2c_adap->algo_data;
<<<<<<< HEAD
<<<<<<< HEAD
	volatile psc_smb_t *sp = (volatile psc_smb_t *)adap->psc_base;
	struct i2c_msg *p;
	int i, err = 0;

	sp->psc_ctrl = PSC_CTRL_ENABLE;
	au_sync();
=======
=======
>>>>>>> refs/remotes/origin/master
	struct i2c_msg *p;
	int i, err = 0;

	WR(adap, PSC_CTRL, PSC_CTRL_ENABLE);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	for (i = 0; !err && i < num; i++) {
		p = &msgs[i];
		err = do_address(adap, p->addr, p->flags & I2C_M_RD,
				 (p->len == 0));
		if (err || !p->len)
			continue;
		if (p->flags & I2C_M_RD)
			err = i2c_read(adap, p->buf, p->len);
		else
			err = i2c_write(adap, p->buf, p->len);
	}

	/* Return the number of messages processed, or the error code.
	*/
	if (err == 0)
		err = num;

<<<<<<< HEAD
<<<<<<< HEAD
	sp->psc_ctrl = PSC_CTRL_SUSPEND;
	au_sync();
=======
	WR(adap, PSC_CTRL, PSC_CTRL_SUSPEND);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	WR(adap, PSC_CTRL, PSC_CTRL_SUSPEND);
>>>>>>> refs/remotes/origin/master

	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32
au1550_func(struct i2c_adapter *adap)
=======
static u32 au1550_func(struct i2c_adapter *adap)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static u32 au1550_func(struct i2c_adapter *adap)
>>>>>>> refs/remotes/origin/master
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
}

static const struct i2c_algorithm au1550_algo = {
	.master_xfer	= au1550_xfer,
	.functionality	= au1550_func,
};

static void i2c_au1550_setup(struct i2c_au1550_data *priv)
{
<<<<<<< HEAD
<<<<<<< HEAD
	volatile psc_smb_t *sp = (volatile psc_smb_t *)priv->psc_base;
	u32 stat;

	sp->psc_ctrl = PSC_CTRL_DISABLE;
	au_sync();
	sp->psc_sel = PSC_SEL_PS_SMBUSMODE;
	sp->psc_smbcfg = 0;
	au_sync();
	sp->psc_ctrl = PSC_CTRL_ENABLE;
	au_sync();
	do {
		stat = sp->psc_smbstat;
		au_sync();
	} while ((stat & PSC_SMBSTAT_SR) == 0);

	sp->psc_smbcfg = (PSC_SMBCFG_RT_FIFO8 | PSC_SMBCFG_TT_FIFO8 |
				PSC_SMBCFG_DD_DISABLE);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long cfg;

	WR(priv, PSC_CTRL, PSC_CTRL_DISABLE);
	WR(priv, PSC_SEL, PSC_SEL_PS_SMBUSMODE);
	WR(priv, PSC_SMBCFG, 0);
	WR(priv, PSC_CTRL, PSC_CTRL_ENABLE);
	while ((RD(priv, PSC_SMBSTAT) & PSC_SMBSTAT_SR) == 0)
		cpu_relax();

	cfg = PSC_SMBCFG_RT_FIFO8 | PSC_SMBCFG_TT_FIFO8 | PSC_SMBCFG_DD_DISABLE;
	WR(priv, PSC_SMBCFG, cfg);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Divide by 8 to get a 6.25 MHz clock.  The later protocol
	 * timings are based on this clock.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	sp->psc_smbcfg |= PSC_SMBCFG_SET_DIV(PSC_SMBCFG_DIV8);
	sp->psc_smbmsk = PSC_SMBMSK_ALLMASK;
	au_sync();
=======
	cfg |= PSC_SMBCFG_SET_DIV(PSC_SMBCFG_DIV8);
	WR(priv, PSC_SMBCFG, cfg);
	WR(priv, PSC_SMBMSK, PSC_SMBMSK_ALLMASK);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cfg |= PSC_SMBCFG_SET_DIV(PSC_SMBCFG_DIV8);
	WR(priv, PSC_SMBCFG, cfg);
	WR(priv, PSC_SMBMSK, PSC_SMBMSK_ALLMASK);
>>>>>>> refs/remotes/origin/master

	/* Set the protocol timer values.  See Table 71 in the
	 * Au1550 Data Book for standard timing values.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	sp->psc_smbtmr = PSC_SMBTMR_SET_TH(0) | PSC_SMBTMR_SET_PS(15) | \
		PSC_SMBTMR_SET_PU(15) | PSC_SMBTMR_SET_SH(15) | \
		PSC_SMBTMR_SET_SU(15) | PSC_SMBTMR_SET_CL(15) | \
		PSC_SMBTMR_SET_CH(15);
	au_sync();

	sp->psc_smbcfg |= PSC_SMBCFG_DE_ENABLE;
	do {
		stat = sp->psc_smbstat;
		au_sync();
	} while ((stat & PSC_SMBSTAT_SR) == 0);

	sp->psc_ctrl = PSC_CTRL_SUSPEND;
	au_sync();
=======
=======
>>>>>>> refs/remotes/origin/master
	WR(priv, PSC_SMBTMR, PSC_SMBTMR_SET_TH(0) | PSC_SMBTMR_SET_PS(15) | \
		PSC_SMBTMR_SET_PU(15) | PSC_SMBTMR_SET_SH(15) | \
		PSC_SMBTMR_SET_SU(15) | PSC_SMBTMR_SET_CL(15) | \
		PSC_SMBTMR_SET_CH(15));

	cfg |= PSC_SMBCFG_DE_ENABLE;
	WR(priv, PSC_SMBCFG, cfg);
	while ((RD(priv, PSC_SMBSTAT) & PSC_SMBSTAT_SR) == 0)
		cpu_relax();

	WR(priv, PSC_CTRL, PSC_CTRL_SUSPEND);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void i2c_au1550_disable(struct i2c_au1550_data *priv)
{
<<<<<<< HEAD
<<<<<<< HEAD
	volatile psc_smb_t *sp = (volatile psc_smb_t *)priv->psc_base;

	sp->psc_smbcfg = 0;
	sp->psc_ctrl = PSC_CTRL_DISABLE;
	au_sync();
=======
	WR(priv, PSC_SMBCFG, 0);
	WR(priv, PSC_CTRL, PSC_CTRL_DISABLE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	WR(priv, PSC_SMBCFG, 0);
	WR(priv, PSC_CTRL, PSC_CTRL_DISABLE);
>>>>>>> refs/remotes/origin/master
}

/*
 * registering functions to load algorithms at runtime
 * Prior to calling us, the 50MHz clock frequency and routing
 * must have been set up for the PSC indicated by the adapter.
 */
<<<<<<< HEAD
static int __devinit
=======
static int
>>>>>>> refs/remotes/origin/master
i2c_au1550_probe(struct platform_device *pdev)
{
	struct i2c_au1550_data *priv;
	struct resource *r;
	int ret;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		ret = -ENODEV;
		goto out;
	}

	priv = kzalloc(sizeof(struct i2c_au1550_data), GFP_KERNEL);
	if (!priv) {
		ret = -ENOMEM;
		goto out;
	}

	priv->ioarea = request_mem_region(r->start, resource_size(r),
					  pdev->name);
	if (!priv->ioarea) {
		ret = -EBUSY;
		goto out_mem;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	priv->psc_base = CKSEG1ADDR(r->start);
	priv->xfer_timeout = 200;
	priv->ack_timeout = 200;
=======
=======
>>>>>>> refs/remotes/origin/master
	priv->psc_base = ioremap(r->start, resource_size(r));
	if (!priv->psc_base) {
		ret = -EIO;
		goto out_map;
	}
	priv->xfer_timeout = 200;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	priv->adap.nr = pdev->id;
	priv->adap.algo = &au1550_algo;
	priv->adap.algo_data = priv;
	priv->adap.dev.parent = &pdev->dev;
	strlcpy(priv->adap.name, "Au1xxx PSC I2C", sizeof(priv->adap.name));

<<<<<<< HEAD
<<<<<<< HEAD
	/* Now, set up the PSC for SMBus PIO mode.
	*/
=======
	/* Now, set up the PSC for SMBus PIO mode. */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Now, set up the PSC for SMBus PIO mode. */
>>>>>>> refs/remotes/origin/master
	i2c_au1550_setup(priv);

	ret = i2c_add_numbered_adapter(&priv->adap);
	if (ret == 0) {
		platform_set_drvdata(pdev, priv);
		return 0;
	}

	i2c_au1550_disable(priv);
<<<<<<< HEAD
<<<<<<< HEAD

=======
	iounmap(priv->psc_base);
out_map:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	iounmap(priv->psc_base);
out_map:
>>>>>>> refs/remotes/origin/master
	release_resource(priv->ioarea);
	kfree(priv->ioarea);
out_mem:
	kfree(priv);
out:
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __devexit
i2c_au1550_remove(struct platform_device *pdev)
=======
static int __devexit i2c_au1550_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct i2c_au1550_data *priv = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);
	i2c_del_adapter(&priv->adap);
	i2c_au1550_disable(priv);
<<<<<<< HEAD
=======
	iounmap(priv->psc_base);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int i2c_au1550_remove(struct platform_device *pdev)
{
	struct i2c_au1550_data *priv = platform_get_drvdata(pdev);

	i2c_del_adapter(&priv->adap);
	i2c_au1550_disable(priv);
	iounmap(priv->psc_base);
>>>>>>> refs/remotes/origin/master
	release_resource(priv->ioarea);
	kfree(priv->ioarea);
	kfree(priv);
	return 0;
}

#ifdef CONFIG_PM
<<<<<<< HEAD
<<<<<<< HEAD
static int
i2c_au1550_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct i2c_au1550_data *priv = platform_get_drvdata(pdev);
=======
static int i2c_au1550_suspend(struct device *dev)
{
	struct i2c_au1550_data *priv = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int i2c_au1550_suspend(struct device *dev)
{
	struct i2c_au1550_data *priv = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/master

	i2c_au1550_disable(priv);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int
i2c_au1550_resume(struct platform_device *pdev)
{
	struct i2c_au1550_data *priv = platform_get_drvdata(pdev);
=======
static int i2c_au1550_resume(struct device *dev)
{
	struct i2c_au1550_data *priv = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int i2c_au1550_resume(struct device *dev)
{
	struct i2c_au1550_data *priv = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/master

	i2c_au1550_setup(priv);

	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
#else
#define i2c_au1550_suspend	NULL
#define i2c_au1550_resume	NULL
=======
=======
>>>>>>> refs/remotes/origin/master

static const struct dev_pm_ops i2c_au1550_pmops = {
	.suspend	= i2c_au1550_suspend,
	.resume		= i2c_au1550_resume,
};

#define AU1XPSC_SMBUS_PMOPS (&i2c_au1550_pmops)

#else
#define AU1XPSC_SMBUS_PMOPS NULL
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif

static struct platform_driver au1xpsc_smbus_driver = {
	.driver = {
		.name	= "au1xpsc_smbus",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
	},
	.probe		= i2c_au1550_probe,
	.remove		= __devexit_p(i2c_au1550_remove),
	.suspend	= i2c_au1550_suspend,
	.resume		= i2c_au1550_resume,
};

static int __init
i2c_au1550_init(void)
{
	return platform_driver_register(&au1xpsc_smbus_driver);
}

static void __exit
i2c_au1550_exit(void)
{
	platform_driver_unregister(&au1xpsc_smbus_driver);
}
=======
		.pm	= AU1XPSC_SMBUS_PMOPS,
	},
	.probe		= i2c_au1550_probe,
	.remove		= __devexit_p(i2c_au1550_remove),
};

module_platform_driver(au1xpsc_smbus_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.pm	= AU1XPSC_SMBUS_PMOPS,
	},
	.probe		= i2c_au1550_probe,
	.remove		= i2c_au1550_remove,
};

module_platform_driver(au1xpsc_smbus_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Dan Malek, Embedded Edge, LLC.");
MODULE_DESCRIPTION("SMBus adapter Alchemy pb1550");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:au1xpsc_smbus");
<<<<<<< HEAD
<<<<<<< HEAD

module_init (i2c_au1550_init);
module_exit (i2c_au1550_exit);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
