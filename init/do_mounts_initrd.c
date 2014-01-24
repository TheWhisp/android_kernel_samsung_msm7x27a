<<<<<<< HEAD
=======
/*
 * Many of the syscalls used in this file expect some of the arguments
 * to be __user pointers not __kernel pointers.  To limit the sparse
 * noise, turn off sparse checking for this file.
 */
#ifdef __CHECKER__
#undef __CHECKER__
#warning "Sparse checking disabled for this file"
#endif

>>>>>>> refs/remotes/origin/master
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/minix_fs.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/ext2_fs.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/romfs_fs.h>
#include <linux/initrd.h>
#include <linux/sched.h>
#include <linux/freezer.h>
<<<<<<< HEAD
=======
#include <linux/kmod.h>
>>>>>>> refs/remotes/origin/master

#include "do_mounts.h"

unsigned long initrd_start, initrd_end;
int initrd_below_start_ok;
unsigned int real_root_dev;	/* do_proc_dointvec cannot handle kdev_t */
<<<<<<< HEAD
static int __initdata old_fd, root_fd;
=======
>>>>>>> refs/remotes/origin/master
static int __initdata mount_initrd = 1;

static int __init no_initrd(char *str)
{
	mount_initrd = 0;
	return 1;
}

__setup("noinitrd", no_initrd);

<<<<<<< HEAD
static int __init do_linuxrc(void *_shell)
{
	static const char *argv[] = { "linuxrc", NULL, };
	extern const char *envp_init[];
	const char *shell = _shell;

	sys_close(old_fd);sys_close(root_fd);
	sys_setsid();
	return kernel_execve(shell, argv, envp_init);
=======
static int init_linuxrc(struct subprocess_info *info, struct cred *new)
{
	sys_unshare(CLONE_FS | CLONE_FILES);
	/* stdin/stdout/stderr for /linuxrc */
	sys_open("/dev/console", O_RDWR, 0);
	sys_dup(0);
	sys_dup(0);
	/* move initrd over / and chdir/chroot in initrd root */
	sys_chdir("/root");
	sys_mount(".", "/", NULL, MS_MOVE, NULL);
	sys_chroot(".");
	sys_setsid();
	return 0;
>>>>>>> refs/remotes/origin/master
}

static void __init handle_initrd(void)
{
<<<<<<< HEAD
	int error;
	int pid;
=======
	struct subprocess_info *info;
	static char *argv[] = { "linuxrc", NULL, };
	extern char *envp_init[];
	int error;
>>>>>>> refs/remotes/origin/master

	real_root_dev = new_encode_dev(ROOT_DEV);
	create_dev("/dev/root.old", Root_RAM0);
	/* mount initrd on rootfs' /root */
	mount_block_root("/dev/root.old", root_mountflags & ~MS_RDONLY);
	sys_mkdir("/old", 0700);
<<<<<<< HEAD
	root_fd = sys_open("/", 0, 0);
	old_fd = sys_open("/old", 0, 0);
	/* move initrd over / and chdir/chroot in initrd root */
	sys_chdir("/root");
	sys_mount(".", "/", NULL, MS_MOVE, NULL);
	sys_chroot(".");
=======
	sys_chdir("/old");

	/* try loading default modules from initrd */
	load_default_modules();
>>>>>>> refs/remotes/origin/master

	/*
	 * In case that a resume from disk is carried out by linuxrc or one of
	 * its children, we need to tell the freezer not to wait for us.
	 */
	current->flags |= PF_FREEZER_SKIP;

<<<<<<< HEAD
	pid = kernel_thread(do_linuxrc, "/linuxrc", SIGCHLD);
	if (pid > 0)
		while (pid != sys_wait4(-1, NULL, 0, NULL))
			yield();
=======
	info = call_usermodehelper_setup("/linuxrc", argv, envp_init,
					 GFP_KERNEL, init_linuxrc, NULL, NULL);
	if (!info)
		return;
	call_usermodehelper_exec(info, UMH_WAIT_PROC);
>>>>>>> refs/remotes/origin/master

	current->flags &= ~PF_FREEZER_SKIP;

	/* move initrd to rootfs' /old */
<<<<<<< HEAD
	sys_fchdir(old_fd);
	sys_mount("/", ".", NULL, MS_MOVE, NULL);
	/* switch root and cwd back to / of rootfs */
	sys_fchdir(root_fd);
	sys_chroot(".");
	sys_close(old_fd);
	sys_close(root_fd);
=======
	sys_mount("..", ".", NULL, MS_MOVE, NULL);
	/* switch root and cwd back to / of rootfs */
	sys_chroot("..");
>>>>>>> refs/remotes/origin/master

	if (new_decode_dev(real_root_dev) == Root_RAM0) {
		sys_chdir("/old");
		return;
	}

<<<<<<< HEAD
=======
	sys_chdir("/");
>>>>>>> refs/remotes/origin/master
	ROOT_DEV = new_decode_dev(real_root_dev);
	mount_root();

	printk(KERN_NOTICE "Trying to move old root to /initrd ... ");
	error = sys_mount("/old", "/root/initrd", NULL, MS_MOVE, NULL);
	if (!error)
		printk("okay\n");
	else {
		int fd = sys_open("/dev/root.old", O_RDWR, 0);
		if (error == -ENOENT)
			printk("/initrd does not exist. Ignored.\n");
		else
			printk("failed\n");
		printk(KERN_NOTICE "Unmounting old root\n");
		sys_umount("/old", MNT_DETACH);
		printk(KERN_NOTICE "Trying to free ramdisk memory ... ");
		if (fd < 0) {
			error = fd;
		} else {
			error = sys_ioctl(fd, BLKFLSBUF, 0);
			sys_close(fd);
		}
		printk(!error ? "okay\n" : "failed\n");
	}
}

int __init initrd_load(void)
{
	if (mount_initrd) {
		create_dev("/dev/ram", Root_RAM0);
		/*
		 * Load the initrd data into /dev/ram0. Execute it as initrd
		 * unless /dev/ram0 is supposed to be our actual root device,
		 * in that case the ram disk is just set up here, and gets
		 * mounted in the normal path.
		 */
		if (rd_load_image("/initrd.image") && ROOT_DEV != Root_RAM0) {
			sys_unlink("/initrd.image");
			handle_initrd();
			return 1;
		}
	}
	sys_unlink("/initrd.image");
	return 0;
}
