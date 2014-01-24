
#ifndef REGD_H
#define REGD_H

#include "core.h"

#define AR6003_BOARD_DATA_ADDR		0x00540654
#define AR6003_BOARD_DATA_INIT_ADDR	0x00540658
#define AR6003_BOARD_DATA_OFFSET	4
#define AR6003_RD_OFFSET		12

int ath6kl_set_reg_dmn(struct ath6kl *ar);

#endif
