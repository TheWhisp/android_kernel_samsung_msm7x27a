#include <linux/module.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/kvm_host.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/kvm_host.h>
>>>>>>> refs/remotes/origin/master
#include <asm/ftrace.h>

#ifdef CONFIG_FUNCTION_TRACER
EXPORT_SYMBOL(_mcount);
#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======
#if defined(CONFIG_KVM) || defined(CONFIG_KVM_MODULE)
EXPORT_SYMBOL(sie64a);
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if IS_ENABLED(CONFIG_KVM)
EXPORT_SYMBOL(sie64a);
EXPORT_SYMBOL(sie_exit);
#endif
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memset);
>>>>>>> refs/remotes/origin/master
