#ifndef __ASM_SH_ETH_H__
#define __ASM_SH_ETH_H__

#include <linux/phy.h>
<<<<<<< HEAD

enum {EDMAC_LITTLE_ENDIAN, EDMAC_BIG_ENDIAN};
enum {
	SH_ETH_REG_GIGABIT,
	SH_ETH_REG_FAST_SH4,
	SH_ETH_REG_FAST_SH3_SH2
};
=======
#include <linux/if_ether.h>

enum {EDMAC_LITTLE_ENDIAN, EDMAC_BIG_ENDIAN};
>>>>>>> refs/remotes/origin/master

struct sh_eth_plat_data {
	int phy;
	int edmac_endian;
<<<<<<< HEAD
	int register_type;
	phy_interface_t phy_interface;
	void (*set_mdio_gate)(void *addr);

	unsigned char mac_addr[6];
=======
	phy_interface_t phy_interface;
	void (*set_mdio_gate)(void *addr);

	unsigned char mac_addr[ETH_ALEN];
>>>>>>> refs/remotes/origin/master
	unsigned no_ether_link:1;
	unsigned ether_link_active_low:1;
	unsigned needs_init:1;
};

#endif
