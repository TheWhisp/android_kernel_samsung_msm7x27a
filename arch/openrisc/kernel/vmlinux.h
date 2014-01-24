#ifndef __OPENRISC_VMLINUX_H_
#define __OPENRISC_VMLINUX_H_

<<<<<<< HEAD
extern char _stext, _etext, _edata, _end;
#ifdef CONFIG_BLK_DEV_INITRD
extern char __initrd_start, __initrd_end;
extern char __initramfs_start;
=======
#ifdef CONFIG_BLK_DEV_INITRD
extern char __initrd_start, __initrd_end;
>>>>>>> refs/remotes/origin/master
#endif

extern u32 __dtb_start[];

#endif
