#ifndef __PERF_MAP_H
#define __PERF_MAP_H

#include <linux/compiler.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

enum map_type {
	MAP__FUNCTION = 0,
	MAP__VARIABLE,
};

#define MAP__NR_TYPES (MAP__VARIABLE + 1)

extern const char *map_type__name[MAP__NR_TYPES];

struct dso;
<<<<<<< HEAD
<<<<<<< HEAD
struct ref_reloc_sym;
struct map_groups;
struct machine;
=======
=======
>>>>>>> refs/remotes/origin/master
struct ip_callchain;
struct ref_reloc_sym;
struct map_groups;
struct machine;
struct perf_evsel;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

struct map {
	union {
		struct rb_node	rb_node;
		struct list_head node;
	};
	u64			start;
	u64			end;
	u8 /* enum map_type */	type;
	bool			referenced;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool			erange_warned;
>>>>>>> refs/remotes/origin/cm-10.0
	u32			priv;
	u64			pgoff;
=======
	bool			erange_warned;
	u32			priv;
	u64			pgoff;
	u32			maj, min; /* only valid for MMAP2 record */
	u64			ino;      /* only valid for MMAP2 record */
	u64			ino_generation;/* only valid for MMAP2 record */
>>>>>>> refs/remotes/origin/master

	/* ip -> dso rip */
	u64			(*map_ip)(struct map *, u64);
	/* dso rip -> ip */
	u64			(*unmap_ip)(struct map *, u64);

	struct dso		*dso;
	struct map_groups	*groups;
};

struct kmap {
	struct ref_reloc_sym	*ref_reloc_sym;
	struct map_groups	*kmaps;
};

struct map_groups {
	struct rb_root	 maps[MAP__NR_TYPES];
	struct list_head removed_maps[MAP__NR_TYPES];
	struct machine	 *machine;
};

<<<<<<< HEAD
/* Native host kernel uses -1 as pid index in machine */
#define	HOST_KERNEL_ID			(-1)
#define	DEFAULT_GUEST_KERNEL_ID		(0)

struct machine {
	struct rb_node	  rb_node;
	pid_t		  pid;
<<<<<<< HEAD
	char		  *root_dir;
=======
	u16		  id_hdr_size;
	char		  *root_dir;
	struct rb_root	  threads;
	struct list_head  dead_threads;
	struct thread	  *last_match;
>>>>>>> refs/remotes/origin/cm-10.0
	struct list_head  user_dsos;
	struct list_head  kernel_dsos;
	struct map_groups kmaps;
	struct map	  *vmlinux_maps[MAP__NR_TYPES];
};

static inline
struct map *machine__kernel_map(struct machine *self, enum map_type type)
{
	return self->vmlinux_maps[type];
}

static inline struct kmap *map__kmap(struct map *self)
{
	return (struct kmap *)(self + 1);
=======
static inline struct kmap *map__kmap(struct map *map)
{
	return (struct kmap *)(map + 1);
>>>>>>> refs/remotes/origin/master
}

static inline u64 map__map_ip(struct map *map, u64 ip)
{
	return ip - map->start + map->pgoff;
}

static inline u64 map__unmap_ip(struct map *map, u64 ip)
{
	return ip + map->start - map->pgoff;
}

<<<<<<< HEAD
static inline u64 identity__map_ip(struct map *map __used, u64 ip)
=======
static inline u64 identity__map_ip(struct map *map __maybe_unused, u64 ip)
>>>>>>> refs/remotes/origin/master
{
	return ip;
}


/* rip/ip <-> addr suitable for passing to `objdump --start-address=` */
u64 map__rip_2objdump(struct map *map, u64 rip);
<<<<<<< HEAD
u64 map__objdump_2ip(struct map *map, u64 addr);
=======

/* objdump address -> memory address */
u64 map__objdump_2mem(struct map *map, u64 ip);
>>>>>>> refs/remotes/origin/master

struct symbol;

typedef int (*symbol_filter_t)(struct map *map, struct symbol *sym);

<<<<<<< HEAD
void map__init(struct map *self, enum map_type type,
	       u64 start, u64 end, u64 pgoff, struct dso *dso);
struct map *map__new(struct list_head *dsos__list, u64 start, u64 len,
		     u64 pgoff, u32 pid, char *filename,
		     enum map_type type);
void map__delete(struct map *self);
struct map *map__clone(struct map *self);
int map__overlap(struct map *l, struct map *r);
size_t map__fprintf(struct map *self, FILE *fp);
<<<<<<< HEAD
=======
size_t map__fprintf_dsoname(struct map *map, FILE *fp);
>>>>>>> refs/remotes/origin/cm-10.0

int map__load(struct map *self, symbol_filter_t filter);
struct symbol *map__find_symbol(struct map *self,
				u64 addr, symbol_filter_t filter);
struct symbol *map__find_symbol_by_name(struct map *self, const char *name,
					symbol_filter_t filter);
void map__fixup_start(struct map *self);
void map__fixup_end(struct map *self);

void map__reloc_vmlinux(struct map *self);

<<<<<<< HEAD
size_t __map_groups__fprintf_maps(struct map_groups *self,
				  enum map_type type, int verbose, FILE *fp);
void maps__insert(struct rb_root *maps, struct map *map);
void maps__remove(struct rb_root *self, struct map *map);
struct map *maps__find(struct rb_root *maps, u64 addr);
void map_groups__init(struct map_groups *self);
void map_groups__exit(struct map_groups *self);
int map_groups__clone(struct map_groups *self,
		      struct map_groups *parent, enum map_type type);
size_t map_groups__fprintf(struct map_groups *self, int verbose, FILE *fp);
size_t map_groups__fprintf_maps(struct map_groups *self, int verbose, FILE *fp);
=======
=======
void map__init(struct map *map, enum map_type type,
	       u64 start, u64 end, u64 pgoff, struct dso *dso);
struct map *map__new(struct list_head *dsos__list, u64 start, u64 len,
		     u64 pgoff, u32 pid, u32 d_maj, u32 d_min, u64 ino,
		     u64 ino_gen,
		     char *filename, enum map_type type);
struct map *map__new2(u64 start, struct dso *dso, enum map_type type);
void map__delete(struct map *map);
struct map *map__clone(struct map *map);
int map__overlap(struct map *l, struct map *r);
size_t map__fprintf(struct map *map, FILE *fp);
size_t map__fprintf_dsoname(struct map *map, FILE *fp);
int map__fprintf_srcline(struct map *map, u64 addr, const char *prefix,
			 FILE *fp);

int map__load(struct map *map, symbol_filter_t filter);
struct symbol *map__find_symbol(struct map *map,
				u64 addr, symbol_filter_t filter);
struct symbol *map__find_symbol_by_name(struct map *map, const char *name,
					symbol_filter_t filter);
void map__fixup_start(struct map *map);
void map__fixup_end(struct map *map);

void map__reloc_vmlinux(struct map *map);

>>>>>>> refs/remotes/origin/master
size_t __map_groups__fprintf_maps(struct map_groups *mg,
				  enum map_type type, int verbose, FILE *fp);
void maps__insert(struct rb_root *maps, struct map *map);
void maps__remove(struct rb_root *maps, struct map *map);
struct map *maps__find(struct rb_root *maps, u64 addr);
<<<<<<< HEAD
=======
struct map *maps__first(struct rb_root *maps);
struct map *maps__next(struct map *map);
>>>>>>> refs/remotes/origin/master
void map_groups__init(struct map_groups *mg);
void map_groups__exit(struct map_groups *mg);
int map_groups__clone(struct map_groups *mg,
		      struct map_groups *parent, enum map_type type);
size_t map_groups__fprintf(struct map_groups *mg, int verbose, FILE *fp);
size_t map_groups__fprintf_maps(struct map_groups *mg, int verbose, FILE *fp);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

typedef void (*machine__process_t)(struct machine *self, void *data);

void machines__process(struct rb_root *self, machine__process_t process, void *data);
struct machine *machines__add(struct rb_root *self, pid_t pid,
			      const char *root_dir);
struct machine *machines__find_host(struct rb_root *self);
struct machine *machines__find(struct rb_root *self, pid_t pid);
struct machine *machines__findnew(struct rb_root *self, pid_t pid);
char *machine__mmap_name(struct machine *self, char *bf, size_t size);
int machine__init(struct machine *self, const char *root_dir, pid_t pid);
void machine__exit(struct machine *self);
void machine__delete(struct machine *self);

<<<<<<< HEAD
=======
int machine__resolve_callchain(struct machine *machine,
			       struct perf_evsel *evsel, struct thread *thread,
			       struct ip_callchain *chain,
			       struct symbol **parent);
int maps__set_kallsyms_ref_reloc_sym(struct map **maps, const char *symbol_name,
				     u64 addr);

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Default guest kernel is defined by parameter --guestkallsyms
 * and --guestmodules
 */
static inline bool machine__is_default_guest(struct machine *self)
{
	return self ? self->pid == DEFAULT_GUEST_KERNEL_ID : false;
}

static inline bool machine__is_host(struct machine *self)
{
	return self ? self->pid == HOST_KERNEL_ID : false;
}

<<<<<<< HEAD
static inline void map_groups__insert(struct map_groups *self, struct map *map)
{
	maps__insert(&self->maps[map->type], map);
	map->groups = self;
}

static inline void map_groups__remove(struct map_groups *self, struct map *map)
{
	maps__remove(&self->maps[map->type], map);
}

static inline struct map *map_groups__find(struct map_groups *self,
					   enum map_type type, u64 addr)
{
	return maps__find(&self->maps[type], addr);
}

struct symbol *map_groups__find_symbol(struct map_groups *self,
=======
=======

int maps__set_kallsyms_ref_reloc_sym(struct map **maps, const char *symbol_name,
				     u64 addr);

>>>>>>> refs/remotes/origin/master
static inline void map_groups__insert(struct map_groups *mg, struct map *map)
{
	maps__insert(&mg->maps[map->type], map);
	map->groups = mg;
}

static inline void map_groups__remove(struct map_groups *mg, struct map *map)
{
	maps__remove(&mg->maps[map->type], map);
}

static inline struct map *map_groups__find(struct map_groups *mg,
					   enum map_type type, u64 addr)
{
	return maps__find(&mg->maps[type], addr);
}

<<<<<<< HEAD
struct symbol *map_groups__find_symbol(struct map_groups *mg,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline struct map *map_groups__first(struct map_groups *mg,
					    enum map_type type)
{
	return maps__first(&mg->maps[type]);
}

static inline struct map *map_groups__next(struct map *map)
{
	return maps__next(map);
}

struct symbol *map_groups__find_symbol(struct map_groups *mg,
>>>>>>> refs/remotes/origin/master
				       enum map_type type, u64 addr,
				       struct map **mapp,
				       symbol_filter_t filter);

<<<<<<< HEAD
<<<<<<< HEAD
struct symbol *map_groups__find_symbol_by_name(struct map_groups *self,
=======
struct symbol *map_groups__find_symbol_by_name(struct map_groups *mg,
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct symbol *map_groups__find_symbol_by_name(struct map_groups *mg,
>>>>>>> refs/remotes/origin/master
					       enum map_type type,
					       const char *name,
					       struct map **mapp,
					       symbol_filter_t filter);

<<<<<<< HEAD
<<<<<<< HEAD
=======

struct thread *machine__findnew_thread(struct machine *machine, pid_t pid);
void machine__remove_thread(struct machine *machine, struct thread *th);

size_t machine__fprintf(struct machine *machine, FILE *fp);

>>>>>>> refs/remotes/origin/cm-10.0
static inline
struct symbol *machine__find_kernel_symbol(struct machine *self,
					   enum map_type type, u64 addr,
					   struct map **mapp,
					   symbol_filter_t filter)
{
	return map_groups__find_symbol(&self->kmaps, type, addr, mapp, filter);
}

static inline
struct symbol *machine__find_kernel_function(struct machine *self, u64 addr,
					     struct map **mapp,
					     symbol_filter_t filter)
{
	return machine__find_kernel_symbol(self, MAP__FUNCTION, addr, mapp, filter);
}

static inline
<<<<<<< HEAD
struct symbol *map_groups__find_function_by_name(struct map_groups *self,
						 const char *name, struct map **mapp,
						 symbol_filter_t filter)
{
	return map_groups__find_symbol_by_name(self, MAP__FUNCTION, name, mapp, filter);
=======
=======
struct addr_map_symbol;

int map_groups__find_ams(struct addr_map_symbol *ams, symbol_filter_t filter);

static inline
>>>>>>> refs/remotes/origin/master
struct symbol *map_groups__find_function_by_name(struct map_groups *mg,
						 const char *name, struct map **mapp,
						 symbol_filter_t filter)
{
	return map_groups__find_symbol_by_name(mg, MAP__FUNCTION, name, mapp, filter);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline
struct symbol *machine__find_kernel_function_by_name(struct machine *self,
						     const char *name,
						     struct map **mapp,
						     symbol_filter_t filter)
{
	return map_groups__find_function_by_name(&self->kmaps, name, mapp,
						 filter);
}

<<<<<<< HEAD
int map_groups__fixup_overlappings(struct map_groups *self, struct map *map,
				   int verbose, FILE *fp);

struct map *map_groups__find_by_name(struct map_groups *self,
				     enum map_type type, const char *name);
struct map *machine__new_module(struct machine *self, u64 start, const char *filename);

void map_groups__flush(struct map_groups *self);
=======
=======
}

>>>>>>> refs/remotes/origin/master
int map_groups__fixup_overlappings(struct map_groups *mg, struct map *map,
				   int verbose, FILE *fp);

struct map *map_groups__find_by_name(struct map_groups *mg,
				     enum map_type type, const char *name);
<<<<<<< HEAD
struct map *machine__new_module(struct machine *self, u64 start, const char *filename);

void map_groups__flush(struct map_groups *mg);
>>>>>>> refs/remotes/origin/cm-10.0
=======

void map_groups__flush(struct map_groups *mg);
>>>>>>> refs/remotes/origin/master

#endif /* __PERF_MAP_H */
