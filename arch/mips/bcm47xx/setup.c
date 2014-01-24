/*
 *  Copyright (C) 2004 Florian Schirmer <jolt@tuxbox.org>
 *  Copyright (C) 2006 Felix Fietkau <nbd@openwrt.org>
<<<<<<< HEAD
<<<<<<< HEAD
 *  Copyright (C) 2006 Michael Buesch <mb@bu3sch.de>
 *  Copyright (C) 2010 Waldemar Brodkorb <wbx@openadk.org>
 *  Copyright (C) 2010-2011 Hauke Mehrtens <hauke@hauke-m.de>
=======
 *  Copyright (C) 2006 Michael Buesch <m@bues.ch>
 *  Copyright (C) 2010 Waldemar Brodkorb <wbx@openadk.org>
 *  Copyright (C) 2010-2012 Hauke Mehrtens <hauke@hauke-m.de>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *  Copyright (C) 2006 Michael Buesch <m@bues.ch>
 *  Copyright (C) 2010 Waldemar Brodkorb <wbx@openadk.org>
 *  Copyright (C) 2010-2012 Hauke Mehrtens <hauke@hauke-m.de>
>>>>>>> refs/remotes/origin/master
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/types.h>
#include <linux/ssb/ssb.h>
#include <linux/ssb/ssb_embedded.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/export.h>
#include <linux/types.h>
#include <linux/ssb/ssb.h>
#include <linux/ssb/ssb_embedded.h>
#include <linux/bcma/bcma_soc.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <asm/bootinfo.h>
#include <asm/reboot.h>
#include <asm/time.h>
#include <bcm47xx.h>
<<<<<<< HEAD
#include <asm/mach-bcm47xx/nvram.h>

<<<<<<< HEAD
struct ssb_bus ssb_bcm47xx;
EXPORT_SYMBOL(ssb_bcm47xx);
=======
=======
#include <bcm47xx_nvram.h>
#include <bcm47xx_board.h>

>>>>>>> refs/remotes/origin/master
union bcm47xx_bus bcm47xx_bus;
EXPORT_SYMBOL(bcm47xx_bus);

enum bcm47xx_bus_type bcm47xx_bus_type;
EXPORT_SYMBOL(bcm47xx_bus_type);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static void bcm47xx_machine_restart(char *command)
{
	printk(KERN_ALERT "Please stand by while rebooting the system...\n");
	local_irq_disable();
	/* Set the watchdog timer to reset immediately */
<<<<<<< HEAD
<<<<<<< HEAD
	ssb_watchdog_timer_set(&ssb_bcm47xx, 1);
=======
=======
>>>>>>> refs/remotes/origin/master
	switch (bcm47xx_bus_type) {
#ifdef CONFIG_BCM47XX_SSB
	case BCM47XX_BUS_TYPE_SSB:
		ssb_watchdog_timer_set(&bcm47xx_bus.ssb, 1);
		break;
#endif
#ifdef CONFIG_BCM47XX_BCMA
	case BCM47XX_BUS_TYPE_BCMA:
		bcma_chipco_watchdog_timer_set(&bcm47xx_bus.bcma.bus.drv_cc, 1);
		break;
#endif
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	while (1)
		cpu_relax();
}

static void bcm47xx_machine_halt(void)
{
	/* Disable interrupts and watchdog and spin forever */
	local_irq_disable();
<<<<<<< HEAD
<<<<<<< HEAD
	ssb_watchdog_timer_set(&ssb_bcm47xx, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
	switch (bcm47xx_bus_type) {
#ifdef CONFIG_BCM47XX_SSB
	case BCM47XX_BUS_TYPE_SSB:
		ssb_watchdog_timer_set(&bcm47xx_bus.ssb, 0);
		break;
#endif
#ifdef CONFIG_BCM47XX_BCMA
	case BCM47XX_BUS_TYPE_BCMA:
		bcma_chipco_watchdog_timer_set(&bcm47xx_bus.bcma.bus.drv_cc, 0);
		break;
#endif
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	while (1)
		cpu_relax();
}

<<<<<<< HEAD
<<<<<<< HEAD
#define READ_FROM_NVRAM(_outvar, name, buf) \
	if (nvram_getprefix(prefix, name, buf, sizeof(buf)) >= 0)\
		sprom->_outvar = simple_strtoul(buf, NULL, 0);

#define READ_FROM_NVRAM2(_outvar, name1, name2, buf) \
	if (nvram_getprefix(prefix, name1, buf, sizeof(buf)) >= 0 || \
	    nvram_getprefix(prefix, name2, buf, sizeof(buf)) >= 0)\
		sprom->_outvar = simple_strtoul(buf, NULL, 0);

static inline int nvram_getprefix(const char *prefix, char *name,
				  char *buf, int len)
{
	if (prefix) {
		char key[100];

		snprintf(key, sizeof(key), "%s%s", prefix, name);
		return nvram_getenv(key, buf, len);
	}

	return nvram_getenv(name, buf, len);
}

static u32 nvram_getu32(const char *name, char *buf, int len)
{
	int rv;
	char key[100];
	u16 var0, var1;

	snprintf(key, sizeof(key), "%s0", name);
	rv = nvram_getenv(key, buf, len);
	/* return 0 here so this looks like unset */
	if (rv < 0)
		return 0;
	var0 = simple_strtoul(buf, NULL, 0);

	snprintf(key, sizeof(key), "%s1", name);
	rv = nvram_getenv(key, buf, len);
	if (rv < 0)
		return 0;
	var1 = simple_strtoul(buf, NULL, 0);
	return var1 << 16 | var0;
}

static void bcm47xx_fill_sprom(struct ssb_sprom *sprom, const char *prefix)
{
	char buf[100];
	u32 boardflags;

	memset(sprom, 0, sizeof(struct ssb_sprom));

	sprom->revision = 1; /* Fallback: Old hardware does not define this. */
	READ_FROM_NVRAM(revision, "sromrev", buf);
	if (nvram_getprefix(prefix, "il0macaddr", buf, sizeof(buf)) >= 0 ||
	    nvram_getprefix(prefix, "macaddr", buf, sizeof(buf)) >= 0)
		nvram_parse_macaddr(buf, sprom->il0mac);
	if (nvram_getprefix(prefix, "et0macaddr", buf, sizeof(buf)) >= 0)
		nvram_parse_macaddr(buf, sprom->et0mac);
	if (nvram_getprefix(prefix, "et1macaddr", buf, sizeof(buf)) >= 0)
		nvram_parse_macaddr(buf, sprom->et1mac);
	READ_FROM_NVRAM(et0phyaddr, "et0phyaddr", buf);
	READ_FROM_NVRAM(et1phyaddr, "et1phyaddr", buf);
	READ_FROM_NVRAM(et0mdcport, "et0mdcport", buf);
	READ_FROM_NVRAM(et1mdcport, "et1mdcport", buf);
	READ_FROM_NVRAM(board_rev, "boardrev", buf);
	READ_FROM_NVRAM(country_code, "ccode", buf);
	READ_FROM_NVRAM(ant_available_a, "aa5g", buf);
	READ_FROM_NVRAM(ant_available_bg, "aa2g", buf);
	READ_FROM_NVRAM(pa0b0, "pa0b0", buf);
	READ_FROM_NVRAM(pa0b1, "pa0b1", buf);
	READ_FROM_NVRAM(pa0b2, "pa0b2", buf);
	READ_FROM_NVRAM(pa1b0, "pa1b0", buf);
	READ_FROM_NVRAM(pa1b1, "pa1b1", buf);
	READ_FROM_NVRAM(pa1b2, "pa1b2", buf);
	READ_FROM_NVRAM(pa1lob0, "pa1lob0", buf);
	READ_FROM_NVRAM(pa1lob2, "pa1lob1", buf);
	READ_FROM_NVRAM(pa1lob1, "pa1lob2", buf);
	READ_FROM_NVRAM(pa1hib0, "pa1hib0", buf);
	READ_FROM_NVRAM(pa1hib2, "pa1hib1", buf);
	READ_FROM_NVRAM(pa1hib1, "pa1hib2", buf);
	READ_FROM_NVRAM2(gpio0, "ledbh0", "wl0gpio0", buf);
	READ_FROM_NVRAM2(gpio1, "ledbh1", "wl0gpio1", buf);
	READ_FROM_NVRAM2(gpio2, "ledbh2", "wl0gpio2", buf);
	READ_FROM_NVRAM2(gpio3, "ledbh3", "wl0gpio3", buf);
	READ_FROM_NVRAM2(maxpwr_bg, "maxp2ga0", "pa0maxpwr", buf);
	READ_FROM_NVRAM2(maxpwr_al, "maxp5gla0", "pa1lomaxpwr", buf);
	READ_FROM_NVRAM2(maxpwr_a, "maxp5ga0", "pa1maxpwr", buf);
	READ_FROM_NVRAM2(maxpwr_ah, "maxp5gha0", "pa1himaxpwr", buf);
	READ_FROM_NVRAM2(itssi_bg, "itt5ga0", "pa0itssit", buf);
	READ_FROM_NVRAM2(itssi_a, "itt2ga0", "pa1itssit", buf);
	READ_FROM_NVRAM(tri2g, "tri2g", buf);
	READ_FROM_NVRAM(tri5gl, "tri5gl", buf);
	READ_FROM_NVRAM(tri5g, "tri5g", buf);
	READ_FROM_NVRAM(tri5gh, "tri5gh", buf);
	READ_FROM_NVRAM(txpid2g[0], "txpid2ga0", buf);
	READ_FROM_NVRAM(txpid2g[1], "txpid2ga1", buf);
	READ_FROM_NVRAM(txpid2g[2], "txpid2ga2", buf);
	READ_FROM_NVRAM(txpid2g[3], "txpid2ga3", buf);
	READ_FROM_NVRAM(txpid5g[0], "txpid5ga0", buf);
	READ_FROM_NVRAM(txpid5g[1], "txpid5ga1", buf);
	READ_FROM_NVRAM(txpid5g[2], "txpid5ga2", buf);
	READ_FROM_NVRAM(txpid5g[3], "txpid5ga3", buf);
	READ_FROM_NVRAM(txpid5gl[0], "txpid5gla0", buf);
	READ_FROM_NVRAM(txpid5gl[1], "txpid5gla1", buf);
	READ_FROM_NVRAM(txpid5gl[2], "txpid5gla2", buf);
	READ_FROM_NVRAM(txpid5gl[3], "txpid5gla3", buf);
	READ_FROM_NVRAM(txpid5gh[0], "txpid5gha0", buf);
	READ_FROM_NVRAM(txpid5gh[1], "txpid5gha1", buf);
	READ_FROM_NVRAM(txpid5gh[2], "txpid5gha2", buf);
	READ_FROM_NVRAM(txpid5gh[3], "txpid5gha3", buf);
	READ_FROM_NVRAM(rxpo2g, "rxpo2g", buf);
	READ_FROM_NVRAM(rxpo5g, "rxpo5g", buf);
	READ_FROM_NVRAM(rssisav2g, "rssisav2g", buf);
	READ_FROM_NVRAM(rssismc2g, "rssismc2g", buf);
	READ_FROM_NVRAM(rssismf2g, "rssismf2g", buf);
	READ_FROM_NVRAM(bxa2g, "bxa2g", buf);
	READ_FROM_NVRAM(rssisav5g, "rssisav5g", buf);
	READ_FROM_NVRAM(rssismc5g, "rssismc5g", buf);
	READ_FROM_NVRAM(rssismf5g, "rssismf5g", buf);
	READ_FROM_NVRAM(bxa5g, "bxa5g", buf);
	READ_FROM_NVRAM(cck2gpo, "cck2gpo", buf);

	sprom->ofdm2gpo = nvram_getu32("ofdm2gpo", buf, sizeof(buf));
	sprom->ofdm5glpo = nvram_getu32("ofdm5glpo", buf, sizeof(buf));
	sprom->ofdm5gpo = nvram_getu32("ofdm5gpo", buf, sizeof(buf));
	sprom->ofdm5ghpo = nvram_getu32("ofdm5ghpo", buf, sizeof(buf));

	READ_FROM_NVRAM(antenna_gain.ghz24.a0, "ag0", buf);
	READ_FROM_NVRAM(antenna_gain.ghz24.a1, "ag1", buf);
	READ_FROM_NVRAM(antenna_gain.ghz24.a2, "ag2", buf);
	READ_FROM_NVRAM(antenna_gain.ghz24.a3, "ag3", buf);
	memcpy(&sprom->antenna_gain.ghz5, &sprom->antenna_gain.ghz24,
	       sizeof(sprom->antenna_gain.ghz5));

	if (nvram_getprefix(prefix, "boardflags", buf, sizeof(buf)) >= 0) {
		boardflags = simple_strtoul(buf, NULL, 0);
		if (boardflags) {
			sprom->boardflags_lo = (boardflags & 0x0000FFFFU);
			sprom->boardflags_hi = (boardflags & 0xFFFF0000U) >> 16;
		}
	}
	if (nvram_getprefix(prefix, "boardflags2", buf, sizeof(buf)) >= 0) {
		boardflags = simple_strtoul(buf, NULL, 0);
		if (boardflags) {
			sprom->boardflags2_lo = (boardflags & 0x0000FFFFU);
			sprom->boardflags2_hi = (boardflags & 0xFFFF0000U) >> 16;
		}
	}
}

int bcm47xx_get_sprom(struct ssb_bus *bus, struct ssb_sprom *out)
=======
#ifdef CONFIG_BCM47XX_SSB
static int bcm47xx_get_sprom_ssb(struct ssb_bus *bus, struct ssb_sprom *out)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_BCM47XX_SSB
static int bcm47xx_get_sprom_ssb(struct ssb_bus *bus, struct ssb_sprom *out)
>>>>>>> refs/remotes/origin/master
{
	char prefix[10];

	if (bus->bustype == SSB_BUSTYPE_PCI) {
<<<<<<< HEAD
		snprintf(prefix, sizeof(prefix), "pci/%u/%u/",
			 bus->host_pci->bus->number + 1,
			 PCI_SLOT(bus->host_pci->devfn));
		bcm47xx_fill_sprom(out, prefix);
=======
		memset(out, 0, sizeof(struct ssb_sprom));
		snprintf(prefix, sizeof(prefix), "pci/%u/%u/",
			 bus->host_pci->bus->number + 1,
			 PCI_SLOT(bus->host_pci->devfn));
		bcm47xx_fill_sprom(out, prefix, false);
>>>>>>> refs/remotes/origin/master
		return 0;
	} else {
		printk(KERN_WARNING "bcm47xx: unable to fill SPROM for given bustype.\n");
		return -EINVAL;
	}
}

static int bcm47xx_get_invariants(struct ssb_bus *bus,
<<<<<<< HEAD
<<<<<<< HEAD
				   struct ssb_init_invariants *iv)
=======
				  struct ssb_init_invariants *iv)
>>>>>>> refs/remotes/origin/cm-10.0
=======
				  struct ssb_init_invariants *iv)
>>>>>>> refs/remotes/origin/master
{
	char buf[20];

	/* Fill boardinfo structure */
	memset(&(iv->boardinfo), 0 , sizeof(struct ssb_boardinfo));

<<<<<<< HEAD
	if (nvram_getenv("boardvendor", buf, sizeof(buf)) >= 0)
		iv->boardinfo.vendor = (u16)simple_strtoul(buf, NULL, 0);
	else
		iv->boardinfo.vendor = SSB_BOARDVENDOR_BCM;
	if (nvram_getenv("boardtype", buf, sizeof(buf)) >= 0)
		iv->boardinfo.type = (u16)simple_strtoul(buf, NULL, 0);
	if (nvram_getenv("boardrev", buf, sizeof(buf)) >= 0)
		iv->boardinfo.rev = (u16)simple_strtoul(buf, NULL, 0);

	bcm47xx_fill_sprom(&iv->sprom, NULL);

	if (nvram_getenv("cardbus", buf, sizeof(buf)) >= 0)
=======
	bcm47xx_fill_ssb_boardinfo(&iv->boardinfo, NULL);

	memset(&iv->sprom, 0, sizeof(struct ssb_sprom));
	bcm47xx_fill_sprom(&iv->sprom, NULL, false);

	if (bcm47xx_nvram_getenv("cardbus", buf, sizeof(buf)) >= 0)
>>>>>>> refs/remotes/origin/master
		iv->has_cardbus_slot = !!simple_strtoul(buf, NULL, 10);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
void __init plat_mem_setup(void)
=======
static void __init bcm47xx_register_ssb(void)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void __init bcm47xx_register_ssb(void)
>>>>>>> refs/remotes/origin/master
{
	int err;
	char buf[100];
	struct ssb_mipscore *mcore;

<<<<<<< HEAD
<<<<<<< HEAD
	err = ssb_arch_register_fallback_sprom(&bcm47xx_get_sprom);
=======
	err = ssb_arch_register_fallback_sprom(&bcm47xx_get_sprom_ssb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = ssb_arch_register_fallback_sprom(&bcm47xx_get_sprom_ssb);
>>>>>>> refs/remotes/origin/master
	if (err)
		printk(KERN_WARNING "bcm47xx: someone else already registered"
			" a ssb SPROM callback handler (err %d)\n", err);

<<<<<<< HEAD
<<<<<<< HEAD
	err = ssb_bus_ssbbus_register(&ssb_bcm47xx, SSB_ENUM_BASE,
				      bcm47xx_get_invariants);
	if (err)
		panic("Failed to initialize SSB bus (err %d)\n", err);

	mcore = &ssb_bcm47xx.mipscore;
=======
=======
>>>>>>> refs/remotes/origin/master
	err = ssb_bus_ssbbus_register(&(bcm47xx_bus.ssb), SSB_ENUM_BASE,
				      bcm47xx_get_invariants);
	if (err)
		panic("Failed to initialize SSB bus (err %d)", err);

	mcore = &bcm47xx_bus.ssb.mipscore;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (nvram_getenv("kernel_args", buf, sizeof(buf)) >= 0) {
=======
	if (bcm47xx_nvram_getenv("kernel_args", buf, sizeof(buf)) >= 0) {
>>>>>>> refs/remotes/origin/master
		if (strstr(buf, "console=ttyS1")) {
			struct ssb_serial_port port;

			printk(KERN_DEBUG "Swapping serial ports!\n");
			/* swap serial ports */
			memcpy(&port, &mcore->serial_ports[0], sizeof(port));
			memcpy(&mcore->serial_ports[0], &mcore->serial_ports[1],
			       sizeof(port));
			memcpy(&mcore->serial_ports[1], &port, sizeof(port));
		}
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
}
#endif

#ifdef CONFIG_BCM47XX_BCMA
static int bcm47xx_get_sprom_bcma(struct bcma_bus *bus, struct ssb_sprom *out)
{
	char prefix[10];
	struct bcma_device *core;

	switch (bus->hosttype) {
	case BCMA_HOSTTYPE_PCI:
<<<<<<< HEAD
		snprintf(prefix, sizeof(prefix), "pci/%u/%u/",
			 bus->host_pci->bus->number + 1,
			 PCI_SLOT(bus->host_pci->devfn));
		bcm47xx_fill_sprom(out, prefix);
		return 0;
	case BCMA_HOSTTYPE_SOC:
		bcm47xx_fill_sprom_ethernet(out, NULL);
=======
		memset(out, 0, sizeof(struct ssb_sprom));
		snprintf(prefix, sizeof(prefix), "pci/%u/%u/",
			 bus->host_pci->bus->number + 1,
			 PCI_SLOT(bus->host_pci->devfn));
		bcm47xx_fill_sprom(out, prefix, false);
		return 0;
	case BCMA_HOSTTYPE_SOC:
		memset(out, 0, sizeof(struct ssb_sprom));
>>>>>>> refs/remotes/origin/master
		core = bcma_find_core(bus, BCMA_CORE_80211);
		if (core) {
			snprintf(prefix, sizeof(prefix), "sb/%u/",
				 core->core_index);
<<<<<<< HEAD
			bcm47xx_fill_sprom(out, prefix);
=======
			bcm47xx_fill_sprom(out, prefix, true);
		} else {
			bcm47xx_fill_sprom(out, NULL, false);
>>>>>>> refs/remotes/origin/master
		}
		return 0;
	default:
		pr_warn("bcm47xx: unable to fill SPROM for given bustype.\n");
		return -EINVAL;
	}
}

static void __init bcm47xx_register_bcma(void)
{
	int err;

	err = bcma_arch_register_fallback_sprom(&bcm47xx_get_sprom_bcma);
	if (err)
		pr_warn("bcm47xx: someone else already registered a bcma SPROM callback handler (err %d)\n", err);

	err = bcma_host_soc_register(&bcm47xx_bus.bcma);
	if (err)
		panic("Failed to initialize BCMA bus (err %d)", err);
<<<<<<< HEAD
=======

	bcm47xx_fill_bcma_boardinfo(&bcm47xx_bus.bcma.bus.boardinfo, NULL);
>>>>>>> refs/remotes/origin/master
}
#endif

void __init plat_mem_setup(void)
{
	struct cpuinfo_mips *c = &current_cpu_data;

	if (c->cputype == CPU_74K) {
		printk(KERN_INFO "bcm47xx: using bcma bus\n");
#ifdef CONFIG_BCM47XX_BCMA
		bcm47xx_bus_type = BCM47XX_BUS_TYPE_BCMA;
		bcm47xx_register_bcma();
#endif
	} else {
		printk(KERN_INFO "bcm47xx: using ssb bus\n");
#ifdef CONFIG_BCM47XX_SSB
		bcm47xx_bus_type = BCM47XX_BUS_TYPE_SSB;
		bcm47xx_register_ssb();
#endif
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	_machine_restart = bcm47xx_machine_restart;
	_machine_halt = bcm47xx_machine_halt;
	pm_power_off = bcm47xx_machine_halt;
<<<<<<< HEAD
}
<<<<<<< HEAD
=======
=======
	bcm47xx_board_detect();
}
>>>>>>> refs/remotes/origin/master

static int __init bcm47xx_register_bus_complete(void)
{
	switch (bcm47xx_bus_type) {
#ifdef CONFIG_BCM47XX_SSB
	case BCM47XX_BUS_TYPE_SSB:
		/* Nothing to do */
		break;
#endif
#ifdef CONFIG_BCM47XX_BCMA
	case BCM47XX_BUS_TYPE_BCMA:
		bcma_bus_register(&bcm47xx_bus.bcma.bus);
		break;
#endif
	}
	return 0;
}
device_initcall(bcm47xx_register_bus_complete);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
