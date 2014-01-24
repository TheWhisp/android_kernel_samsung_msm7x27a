#ifndef LINUX_MM_DEBUG_H
#define LINUX_MM_DEBUG_H 1

#ifdef CONFIG_DEBUG_VM
#define VM_BUG_ON(cond) BUG_ON(cond)
#else
<<<<<<< HEAD
#define VM_BUG_ON(cond) do { (void)(cond); } while (0)
=======
#define VM_BUG_ON(cond) BUILD_BUG_ON_INVALID(cond)
>>>>>>> refs/remotes/origin/master
#endif

#ifdef CONFIG_DEBUG_VIRTUAL
#define VIRTUAL_BUG_ON(cond) BUG_ON(cond)
#else
#define VIRTUAL_BUG_ON(cond) do { } while (0)
#endif

#endif
