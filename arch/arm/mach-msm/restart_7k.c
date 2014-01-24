/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/reboot.h>
#include <linux/pm.h>
#include <asm/system_misc.h>
#include <mach/proc_comm.h>
#include <linux/sec_param.h>

#include "devices-msm7x2xa.h"
#include "smd_rpcrouter.h"
#include "smem_vendor_type.h"

static uint32_t restart_reason = 0x776655AA;

static void msm_pm_power_off(void)
{
        unsigned size = 0;
        samsung_vendor1_id *smem_vendor1 = \
                (samsung_vendor1_id *)smem_get_entry(SMEM_ID_VENDOR1, &size);

        if (smem_vendor1) {
                smem_vendor1->AP_reserved[0] = 0;
        } else {
                printk(KERN_EMERG "smem_flag is NULL\n");
        }        
        
        printk("[msm_pm_power_off] START!!! \n");
        msm_proc_comm(PCOM_POWER_DOWN, 0, 0);
        for (;;)
                ;
}

static void msm_pm_restart(char str, const char *cmd)
{
        unsigned size;
        samsung_vendor1_id *smem_vendor1 = \
                (samsung_vendor1_id *)smem_get_entry(SMEM_ID_VENDOR1, &size);

        if (smem_vendor1) {
                smem_vendor1->silent_reset = 0xAEAEAEAE;
                smem_vendor1->reboot_reason = restart_reason;
        smem_vendor1->AP_reserved[0] = 0;
        } else {
                printk(KERN_EMERG "smem_flag is NULL\n");
        }

        pr_debug("The reset reason is %x\n", restart_reason);

        /* Disable interrupts */
        local_irq_disable();
        local_fiq_disable();

        /*
         * Take out a flat memory mapping  and will
         * insert a 1:1 mapping in place of
         * the user-mode pages to ensure predictable results
         * This function takes care of flushing the caches
         * and flushing the TLB.
         */
        setup_mm_for_reboot();

        msm_proc_comm(PCOM_RESET_CHIP, &restart_reason, 0);

        for (;;)
                ;
}

struct smem_apps_boot_mode {
        unsigned int restart_reason;
        unsigned int dummy;
};

static int msm_reboot_call
        (struct notifier_block *this, unsigned long code, void *_cmd)
{
        unsigned int debug_level;
      struct smem_apps_boot_mode * boot_mode;

        boot_mode = smem_alloc(SMEM_APPS_BOOT_MODE, sizeof(*boot_mode));
        if(boot_mode == NULL) {
            printk(KERN_ERR "Can't alloc smem for restart_reason!!\n");
            return NOTIFY_DONE;
        }

        if ((code == SYS_RESTART) && _cmd) {
                char *cmd = _cmd;
                if (!strncmp(cmd, "bootloader", 10)) {
                        restart_reason = 0x77665500;
                } else if (!strncmp(cmd, "recovery_done",13)) {
                        restart_reason = 0x77665503;                        
                }
                else if (!strncmp(cmd, "recovery", 8)) {
                        restart_reason = 0x77665502;
                } else if (!strncmp(cmd, "eraseflash", 10)) {
                        restart_reason = 0x776655EF;
                } else if (!strcmp(cmd, "download")) {
                        restart_reason = 0x776655FF;
                } else if (!strncmp(cmd, "sud", 3)) {
                        restart_reason = 0x776655FF;
                } else if (!strncmp(cmd, "oem-", 4)) {
                        unsigned long code;
                        int res;
                        res = kstrtoul(cmd + 4, 16, &code);
                        code &= 0xff;
                        restart_reason = 0x6f656d00 | code;
                } else {
                        restart_reason = 0x77665501;
                }
        }
        boot_mode->restart_reason = restart_reason;
        printk("[msm_reboot_call] restart_reason : 0x%x\n", restart_reason);
        return NOTIFY_DONE;
}

static struct notifier_block msm_reboot_notifier = {
        .notifier_call = msm_reboot_call,
};

static int __init msm_pm_restart_init(void)
{
        int ret;

        pm_power_off = msm_pm_power_off;
        arm_pm_restart = msm_pm_restart;

        ret = register_reboot_notifier(&msm_reboot_notifier);
        if (ret)
                pr_err("Failed to register reboot notifier\n");

        return ret;
}
late_initcall(msm_pm_restart_init);
