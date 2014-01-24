#ifndef __ARCH_ORION5X_COMMON_H
#define __ARCH_ORION5X_COMMON_H

<<<<<<< HEAD
=======
#include <linux/reboot.h>

>>>>>>> refs/remotes/origin/master
struct dsa_platform_data;
struct mv643xx_eth_platform_data;
struct mv_sata_platform_data;

<<<<<<< HEAD
=======
#define ORION_MBUS_PCIE_MEM_TARGET    0x04
#define ORION_MBUS_PCIE_MEM_ATTR      0x59
#define ORION_MBUS_PCIE_IO_TARGET     0x04
#define ORION_MBUS_PCIE_IO_ATTR       0x51
#define ORION_MBUS_PCIE_WA_TARGET     0x04
#define ORION_MBUS_PCIE_WA_ATTR       0x79
#define ORION_MBUS_PCI_MEM_TARGET     0x03
#define ORION_MBUS_PCI_MEM_ATTR       0x59
#define ORION_MBUS_PCI_IO_TARGET      0x03
#define ORION_MBUS_PCI_IO_ATTR        0x51
#define ORION_MBUS_DEVBUS_BOOT_TARGET 0x01
#define ORION_MBUS_DEVBUS_BOOT_ATTR   0x0f
#define ORION_MBUS_DEVBUS_TARGET(cs)  0x01
#define ORION_MBUS_DEVBUS_ATTR(cs)    (~(1 << cs))
#define ORION_MBUS_SRAM_TARGET        0x00
#define ORION_MBUS_SRAM_ATTR          0x00

>>>>>>> refs/remotes/origin/master
/*
 * Basic Orion init functions used early by machine-setup.
 */
void orion5x_map_io(void);
void orion5x_init_early(void);
void orion5x_init_irq(void);
void orion5x_init(void);
<<<<<<< HEAD
extern int orion5x_tclk;
extern struct sys_timer orion5x_timer;

/*
 * Enumerations and functions for Orion windows mapping. Used by Orion core
 * functions to map its interfaces and by the machine-setup to map its on-
 * board devices. Details in /mach-orion/addr-map.c
 */
<<<<<<< HEAD
extern struct mbus_dram_target_info orion5x_mbus_dram_info;
=======
>>>>>>> refs/remotes/origin/cm-10.0
void orion5x_setup_cpu_mbus_bridge(void);
void orion5x_setup_dev_boot_win(u32 base, u32 size);
void orion5x_setup_dev0_win(u32 base, u32 size);
void orion5x_setup_dev1_win(u32 base, u32 size);
void orion5x_setup_dev2_win(u32 base, u32 size);
void orion5x_setup_pcie_wa_win(u32 base, u32 size);
<<<<<<< HEAD
int orion5x_setup_sram_win(void);
=======
void orion5x_setup_sram_win(void);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void orion5x_id(u32 *dev, u32 *rev, char **dev_name);
void clk_init(void);
extern int orion5x_tclk;
extern void orion5x_timer_init(void);

void orion5x_setup_wins(void);
>>>>>>> refs/remotes/origin/master

void orion5x_ehci0_init(void);
void orion5x_ehci1_init(void);
void orion5x_eth_init(struct mv643xx_eth_platform_data *eth_data);
void orion5x_eth_switch_init(struct dsa_platform_data *d, int irq);
void orion5x_i2c_init(void);
void orion5x_sata_init(struct mv_sata_platform_data *sata_data);
void orion5x_spi_init(void);
void orion5x_uart0_init(void);
void orion5x_uart1_init(void);
void orion5x_xor_init(void);
<<<<<<< HEAD
<<<<<<< HEAD
=======
void orion5x_restart(char, const char *);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void orion5x_restart(enum reboot_mode, const char *);
>>>>>>> refs/remotes/origin/master

/*
 * PCIe/PCI functions.
 */
struct pci_bus;
struct pci_sys_data;
<<<<<<< HEAD
=======
struct pci_dev;
>>>>>>> refs/remotes/origin/master

void orion5x_pcie_id(u32 *dev, u32 *rev);
void orion5x_pci_disable(void);
void orion5x_pci_set_cardbus_mode(void);
int orion5x_pci_sys_setup(int nr, struct pci_sys_data *sys);
struct pci_bus *orion5x_pci_sys_scan_bus(int nr, struct pci_sys_data *sys);
<<<<<<< HEAD
<<<<<<< HEAD
int orion5x_pci_map_irq(struct pci_dev *dev, u8 slot, u8 pin);

struct machine_desc;
struct meminfo;
struct tag;
extern void __init tag_fixup_mem32(struct machine_desc *, struct tag *,
				   char **, struct meminfo *);

=======
int orion5x_pci_map_irq(const struct pci_dev *dev, u8 slot, u8 pin);

=======
int orion5x_pci_map_irq(const struct pci_dev *dev, u8 slot, u8 pin);

/* board init functions for boards not fully converted to fdt */
#ifdef CONFIG_MACH_EDMINI_V2_DT
void edmini_v2_init(void);
#else
static inline void edmini_v2_init(void) {};
#endif

>>>>>>> refs/remotes/origin/master
struct meminfo;
struct tag;
extern void __init tag_fixup_mem32(struct tag *, char **, struct meminfo *);

/*****************************************************************************
 * Helpers to access Orion registers
 ****************************************************************************/
/*
 * These are not preempt-safe.  Locks, if needed, must be taken
 * care of by the caller.
 */
#define orion5x_setbits(r, mask)	writel(readl(r) | (mask), (r))
#define orion5x_clrbits(r, mask)	writel(readl(r) & ~(mask), (r))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif
