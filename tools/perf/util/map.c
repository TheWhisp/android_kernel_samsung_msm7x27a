#include "symbol.h"
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "map.h"
<<<<<<< HEAD
=======
#include "thread.h"
#include "strlist.h"
#include "vdso.h"
#include "build-id.h"
#include "util.h"
#include <linux/string.h>
>>>>>>> refs/remotes/origin/master

const char *map_type__name[MAP__NR_TYPES] = {
	[MAP__FUNCTION] = "Functions",
	[MAP__VARIABLE] = "Variables",
};

static inline int is_anon_memory(const char *filename)
{
	return !strcmp(filename, "//anon") ||
	       !strcmp(filename, "/dev/zero (deleted)") ||
	       !strcmp(filename, "/anon_hugepage (deleted)");
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static inline int is_no_dso_memory(const char *filename)
{
	return !strcmp(filename, "[stack]") ||
	       !strcmp(filename, "[vdso]")  ||
	       !strcmp(filename, "[heap]");
}

>>>>>>> refs/remotes/origin/cm-10.0
void map__init(struct map *self, enum map_type type,
	       u64 start, u64 end, u64 pgoff, struct dso *dso)
{
	self->type     = type;
	self->start    = start;
	self->end      = end;
	self->pgoff    = pgoff;
	self->dso      = dso;
	self->map_ip   = map__map_ip;
	self->unmap_ip = map__unmap_ip;
	RB_CLEAR_NODE(&self->rb_node);
	self->groups   = NULL;
	self->referenced = false;
<<<<<<< HEAD
=======
	self->erange_warned = false;
>>>>>>> refs/remotes/origin/cm-10.0
}

struct map *map__new(struct list_head *dsos__list, u64 start, u64 len,
		     u64 pgoff, u32 pid, char *filename,
		     enum map_type type)
{
	struct map *self = malloc(sizeof(*self));

	if (self != NULL) {
		char newfilename[PATH_MAX];
		struct dso *dso;
<<<<<<< HEAD
		int anon;

		anon = is_anon_memory(filename);
=======
		int anon, no_dso;

		anon = is_anon_memory(filename);
		no_dso = is_no_dso_memory(filename);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int is_no_dso_memory(const char *filename)
{
	return !strncmp(filename, "[stack", 6) ||
	       !strcmp(filename, "[heap]");
}

void map__init(struct map *map, enum map_type type,
	       u64 start, u64 end, u64 pgoff, struct dso *dso)
{
	map->type     = type;
	map->start    = start;
	map->end      = end;
	map->pgoff    = pgoff;
	map->dso      = dso;
	map->map_ip   = map__map_ip;
	map->unmap_ip = map__unmap_ip;
	RB_CLEAR_NODE(&map->rb_node);
	map->groups   = NULL;
	map->referenced = false;
	map->erange_warned = false;
}

struct map *map__new(struct list_head *dsos__list, u64 start, u64 len,
		     u64 pgoff, u32 pid, u32 d_maj, u32 d_min, u64 ino,
		     u64 ino_gen, char *filename,
		     enum map_type type)
{
	struct map *map = malloc(sizeof(*map));

	if (map != NULL) {
		char newfilename[PATH_MAX];
		struct dso *dso;
		int anon, no_dso, vdso;

		anon = is_anon_memory(filename);
		vdso = is_vdso_map(filename);
		no_dso = is_no_dso_memory(filename);

		map->maj = d_maj;
		map->min = d_min;
		map->ino = ino;
		map->ino_generation = ino_gen;
>>>>>>> refs/remotes/origin/master

		if (anon) {
			snprintf(newfilename, sizeof(newfilename), "/tmp/perf-%d.map", pid);
			filename = newfilename;
		}

<<<<<<< HEAD
		dso = __dsos__findnew(dsos__list, filename);
		if (dso == NULL)
			goto out_delete;

		map__init(self, type, start, start + len, pgoff, dso);

<<<<<<< HEAD
		if (anon) {
set_identity:
			self->map_ip = self->unmap_ip = identity__map_ip;
		} else if (strcmp(filename, "[vdso]") == 0) {
			dso__set_loaded(dso, self->type);
			goto set_identity;
=======
		if (anon || no_dso) {
			self->map_ip = self->unmap_ip = identity__map_ip;
=======
		if (vdso) {
			pgoff = 0;
			dso = vdso__dso_findnew(dsos__list);
		} else
			dso = __dsos__findnew(dsos__list, filename);

		if (dso == NULL)
			goto out_delete;

		map__init(map, type, start, start + len, pgoff, dso);

		if (anon || no_dso) {
			map->map_ip = map->unmap_ip = identity__map_ip;
>>>>>>> refs/remotes/origin/master

			/*
			 * Set memory without DSO as loaded. All map__find_*
			 * functions still return NULL, and we avoid the
			 * unnecessary map__load warning.
			 */
			if (no_dso)
<<<<<<< HEAD
				dso__set_loaded(dso, self->type);
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}
	return self;
out_delete:
	free(self);
	return NULL;
}

void map__delete(struct map *self)
{
	free(self);
}

void map__fixup_start(struct map *self)
{
	struct rb_root *symbols = &self->dso->symbols[self->type];
	struct rb_node *nd = rb_first(symbols);
	if (nd != NULL) {
		struct symbol *sym = rb_entry(nd, struct symbol, rb_node);
		self->start = sym->start;
	}
}

void map__fixup_end(struct map *self)
{
	struct rb_root *symbols = &self->dso->symbols[self->type];
	struct rb_node *nd = rb_last(symbols);
	if (nd != NULL) {
		struct symbol *sym = rb_entry(nd, struct symbol, rb_node);
		self->end = sym->end;
=======
				dso__set_loaded(dso, map->type);
		}
	}
	return map;
out_delete:
	free(map);
	return NULL;
}

/*
 * Constructor variant for modules (where we know from /proc/modules where
 * they are loaded) and for vmlinux, where only after we load all the
 * symbols we'll know where it starts and ends.
 */
struct map *map__new2(u64 start, struct dso *dso, enum map_type type)
{
	struct map *map = calloc(1, (sizeof(*map) +
				     (dso->kernel ? sizeof(struct kmap) : 0)));
	if (map != NULL) {
		/*
		 * ->end will be filled after we load all the symbols
		 */
		map__init(map, type, start, 0, 0, dso);
	}

	return map;
}

void map__delete(struct map *map)
{
	free(map);
}

void map__fixup_start(struct map *map)
{
	struct rb_root *symbols = &map->dso->symbols[map->type];
	struct rb_node *nd = rb_first(symbols);
	if (nd != NULL) {
		struct symbol *sym = rb_entry(nd, struct symbol, rb_node);
		map->start = sym->start;
	}
}

void map__fixup_end(struct map *map)
{
	struct rb_root *symbols = &map->dso->symbols[map->type];
	struct rb_node *nd = rb_last(symbols);
	if (nd != NULL) {
		struct symbol *sym = rb_entry(nd, struct symbol, rb_node);
		map->end = sym->end;
>>>>>>> refs/remotes/origin/master
	}
}

#define DSO__DELETED "(deleted)"

<<<<<<< HEAD
int map__load(struct map *self, symbol_filter_t filter)
{
	const char *name = self->dso->long_name;
	int nr;

	if (dso__loaded(self->dso, self->type))
		return 0;

	nr = dso__load(self->dso, self, filter);
	if (nr < 0) {
		if (self->dso->has_build_id) {
			char sbuild_id[BUILD_ID_SIZE * 2 + 1];

			build_id__sprintf(self->dso->build_id,
					  sizeof(self->dso->build_id),
=======
int map__load(struct map *map, symbol_filter_t filter)
{
	const char *name = map->dso->long_name;
	int nr;

	if (dso__loaded(map->dso, map->type))
		return 0;

	nr = dso__load(map->dso, map, filter);
	if (nr < 0) {
		if (map->dso->has_build_id) {
			char sbuild_id[BUILD_ID_SIZE * 2 + 1];

			build_id__sprintf(map->dso->build_id,
					  sizeof(map->dso->build_id),
>>>>>>> refs/remotes/origin/master
					  sbuild_id);
			pr_warning("%s with build id %s not found",
				   name, sbuild_id);
		} else
			pr_warning("Failed to open %s", name);

		pr_warning(", continuing without symbols\n");
		return -1;
	} else if (nr == 0) {
<<<<<<< HEAD
=======
#ifdef HAVE_LIBELF_SUPPORT
>>>>>>> refs/remotes/origin/master
		const size_t len = strlen(name);
		const size_t real_len = len - sizeof(DSO__DELETED);

		if (len > sizeof(DSO__DELETED) &&
		    strcmp(name + real_len + 1, DSO__DELETED) == 0) {
<<<<<<< HEAD
<<<<<<< HEAD
			pr_warning("%.*s was updated, restart the long "
				   "running apps that use it!\n",
=======
			pr_warning("%.*s was updated (is prelink enabled?). "
				"Restart the long running apps that use it!\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_warning("%.*s was updated (is prelink enabled?). "
				"Restart the long running apps that use it!\n",
>>>>>>> refs/remotes/origin/master
				   (int)real_len, name);
		} else {
			pr_warning("no symbols found in %s, maybe install "
				   "a debug package?\n", name);
		}
<<<<<<< HEAD

		return -1;
	}
	/*
	 * Only applies to the kernel, as its symtabs aren't relative like the
	 * module ones.
	 */
	if (self->dso->kernel)
		map__reloc_vmlinux(self);
=======
#endif
		return -1;
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
struct symbol *map__find_symbol(struct map *self, u64 addr,
				symbol_filter_t filter)
{
	if (map__load(self, filter) < 0)
		return NULL;

	return dso__find_symbol(self->dso, self->type, addr);
}

struct symbol *map__find_symbol_by_name(struct map *self, const char *name,
					symbol_filter_t filter)
{
	if (map__load(self, filter) < 0)
		return NULL;

	if (!dso__sorted_by_name(self->dso, self->type))
		dso__sort_by_name(self->dso, self->type);

	return dso__find_symbol_by_name(self->dso, self->type, name);
}

struct map *map__clone(struct map *self)
{
	struct map *map = malloc(sizeof(*self));

	if (!map)
		return NULL;

	memcpy(map, self, sizeof(*self));

	return map;
=======
struct symbol *map__find_symbol(struct map *map, u64 addr,
				symbol_filter_t filter)
{
	if (map__load(map, filter) < 0)
		return NULL;

	return dso__find_symbol(map->dso, map->type, addr);
}

struct symbol *map__find_symbol_by_name(struct map *map, const char *name,
					symbol_filter_t filter)
{
	if (map__load(map, filter) < 0)
		return NULL;

	if (!dso__sorted_by_name(map->dso, map->type))
		dso__sort_by_name(map->dso, map->type);

	return dso__find_symbol_by_name(map->dso, map->type, name);
}

struct map *map__clone(struct map *map)
{
	return memdup(map, sizeof(*map));
>>>>>>> refs/remotes/origin/master
}

int map__overlap(struct map *l, struct map *r)
{
	if (l->start > r->start) {
		struct map *t = l;
		l = r;
		r = t;
	}

	if (l->end > r->start)
		return 1;

	return 0;
}

<<<<<<< HEAD
size_t map__fprintf(struct map *self, FILE *fp)
{
	return fprintf(fp, " %" PRIx64 "-%" PRIx64 " %" PRIx64 " %s\n",
		       self->start, self->end, self->pgoff, self->dso->name);
}

<<<<<<< HEAD
=======
size_t map__fprintf_dsoname(struct map *map, FILE *fp)
{
	const char *dsoname;
=======
size_t map__fprintf(struct map *map, FILE *fp)
{
	return fprintf(fp, " %" PRIx64 "-%" PRIx64 " %" PRIx64 " %s\n",
		       map->start, map->end, map->pgoff, map->dso->name);
}

size_t map__fprintf_dsoname(struct map *map, FILE *fp)
{
	const char *dsoname = "[unknown]";
>>>>>>> refs/remotes/origin/master

	if (map && map->dso && (map->dso->name || map->dso->long_name)) {
		if (symbol_conf.show_kernel_path && map->dso->long_name)
			dsoname = map->dso->long_name;
		else if (map->dso->name)
			dsoname = map->dso->name;
<<<<<<< HEAD
	} else
		dsoname = "[unknown]";
=======
	}
>>>>>>> refs/remotes/origin/master

	return fprintf(fp, "%s", dsoname);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * objdump wants/reports absolute IPs for ET_EXEC, and RIPs for ET_DYN.
 * map->dso->adjust_symbols==1 for ET_EXEC-like cases.
 */
u64 map__rip_2objdump(struct map *map, u64 rip)
{
	u64 addr = map->dso->adjust_symbols ?
			map->unmap_ip(map, rip) :	/* RIP -> IP */
			rip;
	return addr;
}

u64 map__objdump_2ip(struct map *map, u64 addr)
{
	u64 ip = map->dso->adjust_symbols ?
			addr :
			map->unmap_ip(map, addr);	/* RIP -> IP */
	return ip;
}

<<<<<<< HEAD
void map_groups__init(struct map_groups *self)
{
	int i;
	for (i = 0; i < MAP__NR_TYPES; ++i) {
		self->maps[i] = RB_ROOT;
		INIT_LIST_HEAD(&self->removed_maps[i]);
	}
	self->machine = NULL;
}

static void maps__delete(struct rb_root *self)
{
	struct rb_node *next = rb_first(self);
=======
=======
int map__fprintf_srcline(struct map *map, u64 addr, const char *prefix,
			 FILE *fp)
{
	char *srcline;
	int ret = 0;

	if (map && map->dso) {
		srcline = get_srcline(map->dso,
				      map__rip_2objdump(map, addr));
		if (srcline != SRCLINE_UNKNOWN)
			ret = fprintf(fp, "%s%s", prefix, srcline);
		free_srcline(srcline);
	}
	return ret;
}

/**
 * map__rip_2objdump - convert symbol start address to objdump address.
 * @map: memory map
 * @rip: symbol start address
 *
 * objdump wants/reports absolute IPs for ET_EXEC, and RIPs for ET_DYN.
 * map->dso->adjust_symbols==1 for ET_EXEC-like cases except ET_REL which is
 * relative to section start.
 *
 * Return: Address suitable for passing to "objdump --start-address="
 */
u64 map__rip_2objdump(struct map *map, u64 rip)
{
	if (!map->dso->adjust_symbols)
		return rip;

	if (map->dso->rel)
		return rip - map->pgoff;

	return map->unmap_ip(map, rip);
}

/**
 * map__objdump_2mem - convert objdump address to a memory address.
 * @map: memory map
 * @ip: objdump address
 *
 * Closely related to map__rip_2objdump(), this function takes an address from
 * objdump and converts it to a memory address.  Note this assumes that @map
 * contains the address.  To be sure the result is valid, check it forwards
 * e.g. map__rip_2objdump(map->map_ip(map, map__objdump_2mem(map, ip))) == ip
 *
 * Return: Memory address.
 */
u64 map__objdump_2mem(struct map *map, u64 ip)
{
	if (!map->dso->adjust_symbols)
		return map->unmap_ip(map, ip);

	if (map->dso->rel)
		return map->unmap_ip(map, ip + map->pgoff);

	return ip;
}

>>>>>>> refs/remotes/origin/master
void map_groups__init(struct map_groups *mg)
{
	int i;
	for (i = 0; i < MAP__NR_TYPES; ++i) {
		mg->maps[i] = RB_ROOT;
		INIT_LIST_HEAD(&mg->removed_maps[i]);
	}
	mg->machine = NULL;
}

static void maps__delete(struct rb_root *maps)
{
	struct rb_node *next = rb_first(maps);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	while (next) {
		struct map *pos = rb_entry(next, struct map, rb_node);

		next = rb_next(&pos->rb_node);
<<<<<<< HEAD
<<<<<<< HEAD
		rb_erase(&pos->rb_node, self);
=======
		rb_erase(&pos->rb_node, maps);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		rb_erase(&pos->rb_node, maps);
>>>>>>> refs/remotes/origin/master
		map__delete(pos);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static void maps__delete_removed(struct list_head *self)
{
	struct map *pos, *n;

	list_for_each_entry_safe(pos, n, self, node) {
=======
=======
>>>>>>> refs/remotes/origin/master
static void maps__delete_removed(struct list_head *maps)
{
	struct map *pos, *n;

	list_for_each_entry_safe(pos, n, maps, node) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		list_del(&pos->node);
		map__delete(pos);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
void map_groups__exit(struct map_groups *self)
=======
void map_groups__exit(struct map_groups *mg)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void map_groups__exit(struct map_groups *mg)
>>>>>>> refs/remotes/origin/master
{
	int i;

	for (i = 0; i < MAP__NR_TYPES; ++i) {
<<<<<<< HEAD
<<<<<<< HEAD
		maps__delete(&self->maps[i]);
		maps__delete_removed(&self->removed_maps[i]);
	}
}

void map_groups__flush(struct map_groups *self)
=======
=======
>>>>>>> refs/remotes/origin/master
		maps__delete(&mg->maps[i]);
		maps__delete_removed(&mg->removed_maps[i]);
	}
}

void map_groups__flush(struct map_groups *mg)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	int type;

	for (type = 0; type < MAP__NR_TYPES; type++) {
<<<<<<< HEAD
<<<<<<< HEAD
		struct rb_root *root = &self->maps[type];
=======
		struct rb_root *root = &mg->maps[type];
>>>>>>> refs/remotes/origin/cm-10.0
=======
		struct rb_root *root = &mg->maps[type];
>>>>>>> refs/remotes/origin/master
		struct rb_node *next = rb_first(root);

		while (next) {
			struct map *pos = rb_entry(next, struct map, rb_node);
			next = rb_next(&pos->rb_node);
			rb_erase(&pos->rb_node, root);
			/*
			 * We may have references to this map, for
			 * instance in some hist_entry instances, so
			 * just move them to a separate list.
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			list_add_tail(&pos->node, &self->removed_maps[pos->type]);
=======
			list_add_tail(&pos->node, &mg->removed_maps[pos->type]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			list_add_tail(&pos->node, &mg->removed_maps[pos->type]);
>>>>>>> refs/remotes/origin/master
		}
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
struct symbol *map_groups__find_symbol(struct map_groups *self,
=======
struct symbol *map_groups__find_symbol(struct map_groups *mg,
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct symbol *map_groups__find_symbol(struct map_groups *mg,
>>>>>>> refs/remotes/origin/master
				       enum map_type type, u64 addr,
				       struct map **mapp,
				       symbol_filter_t filter)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct map *map = map_groups__find(self, type, addr);
=======
	struct map *map = map_groups__find(mg, type, addr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct map *map = map_groups__find(mg, type, addr);
>>>>>>> refs/remotes/origin/master

	if (map != NULL) {
		if (mapp != NULL)
			*mapp = map;
		return map__find_symbol(map, map->map_ip(map, addr), filter);
	}

	return NULL;
}

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
					       symbol_filter_t filter)
{
	struct rb_node *nd;

<<<<<<< HEAD
<<<<<<< HEAD
	for (nd = rb_first(&self->maps[type]); nd; nd = rb_next(nd)) {
=======
	for (nd = rb_first(&mg->maps[type]); nd; nd = rb_next(nd)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (nd = rb_first(&mg->maps[type]); nd; nd = rb_next(nd)) {
>>>>>>> refs/remotes/origin/master
		struct map *pos = rb_entry(nd, struct map, rb_node);
		struct symbol *sym = map__find_symbol_by_name(pos, name, filter);

		if (sym == NULL)
			continue;
		if (mapp != NULL)
			*mapp = pos;
		return sym;
	}

	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
size_t __map_groups__fprintf_maps(struct map_groups *self,
=======
size_t __map_groups__fprintf_maps(struct map_groups *mg,
>>>>>>> refs/remotes/origin/cm-10.0
=======
int map_groups__find_ams(struct addr_map_symbol *ams, symbol_filter_t filter)
{
	if (ams->addr < ams->map->start || ams->addr > ams->map->end) {
		if (ams->map->groups == NULL)
			return -1;
		ams->map = map_groups__find(ams->map->groups, ams->map->type,
					    ams->addr);
		if (ams->map == NULL)
			return -1;
	}

	ams->al_addr = ams->map->map_ip(ams->map, ams->addr);
	ams->sym = map__find_symbol(ams->map, ams->al_addr, filter);

	return ams->sym ? 0 : -1;
}

size_t __map_groups__fprintf_maps(struct map_groups *mg,
>>>>>>> refs/remotes/origin/master
				  enum map_type type, int verbose, FILE *fp)
{
	size_t printed = fprintf(fp, "%s:\n", map_type__name[type]);
	struct rb_node *nd;

<<<<<<< HEAD
<<<<<<< HEAD
	for (nd = rb_first(&self->maps[type]); nd; nd = rb_next(nd)) {
=======
	for (nd = rb_first(&mg->maps[type]); nd; nd = rb_next(nd)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (nd = rb_first(&mg->maps[type]); nd; nd = rb_next(nd)) {
>>>>>>> refs/remotes/origin/master
		struct map *pos = rb_entry(nd, struct map, rb_node);
		printed += fprintf(fp, "Map:");
		printed += map__fprintf(pos, fp);
		if (verbose > 2) {
			printed += dso__fprintf(pos->dso, type, fp);
			printed += fprintf(fp, "--\n");
		}
	}

	return printed;
}

<<<<<<< HEAD
<<<<<<< HEAD
size_t map_groups__fprintf_maps(struct map_groups *self, int verbose, FILE *fp)
{
	size_t printed = 0, i;
	for (i = 0; i < MAP__NR_TYPES; ++i)
		printed += __map_groups__fprintf_maps(self, i, verbose, fp);
	return printed;
}

static size_t __map_groups__fprintf_removed_maps(struct map_groups *self,
=======
=======
>>>>>>> refs/remotes/origin/master
size_t map_groups__fprintf_maps(struct map_groups *mg, int verbose, FILE *fp)
{
	size_t printed = 0, i;
	for (i = 0; i < MAP__NR_TYPES; ++i)
		printed += __map_groups__fprintf_maps(mg, i, verbose, fp);
	return printed;
}

static size_t __map_groups__fprintf_removed_maps(struct map_groups *mg,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
						 enum map_type type,
						 int verbose, FILE *fp)
{
	struct map *pos;
	size_t printed = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(pos, &self->removed_maps[type], node) {
=======
	list_for_each_entry(pos, &mg->removed_maps[type], node) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	list_for_each_entry(pos, &mg->removed_maps[type], node) {
>>>>>>> refs/remotes/origin/master
		printed += fprintf(fp, "Map:");
		printed += map__fprintf(pos, fp);
		if (verbose > 1) {
			printed += dso__fprintf(pos->dso, type, fp);
			printed += fprintf(fp, "--\n");
		}
	}
	return printed;
}

<<<<<<< HEAD
<<<<<<< HEAD
static size_t map_groups__fprintf_removed_maps(struct map_groups *self,
=======
static size_t map_groups__fprintf_removed_maps(struct map_groups *mg,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static size_t map_groups__fprintf_removed_maps(struct map_groups *mg,
>>>>>>> refs/remotes/origin/master
					       int verbose, FILE *fp)
{
	size_t printed = 0, i;
	for (i = 0; i < MAP__NR_TYPES; ++i)
<<<<<<< HEAD
<<<<<<< HEAD
		printed += __map_groups__fprintf_removed_maps(self, i, verbose, fp);
	return printed;
}

size_t map_groups__fprintf(struct map_groups *self, int verbose, FILE *fp)
{
	size_t printed = map_groups__fprintf_maps(self, verbose, fp);
	printed += fprintf(fp, "Removed maps:\n");
	return printed + map_groups__fprintf_removed_maps(self, verbose, fp);
}

int map_groups__fixup_overlappings(struct map_groups *self, struct map *map,
				   int verbose, FILE *fp)
{
	struct rb_root *root = &self->maps[map->type];
=======
=======
>>>>>>> refs/remotes/origin/master
		printed += __map_groups__fprintf_removed_maps(mg, i, verbose, fp);
	return printed;
}

size_t map_groups__fprintf(struct map_groups *mg, int verbose, FILE *fp)
{
	size_t printed = map_groups__fprintf_maps(mg, verbose, fp);
	printed += fprintf(fp, "Removed maps:\n");
	return printed + map_groups__fprintf_removed_maps(mg, verbose, fp);
}

int map_groups__fixup_overlappings(struct map_groups *mg, struct map *map,
				   int verbose, FILE *fp)
{
	struct rb_root *root = &mg->maps[map->type];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct rb_node *next = rb_first(root);
	int err = 0;

	while (next) {
		struct map *pos = rb_entry(next, struct map, rb_node);
		next = rb_next(&pos->rb_node);

		if (!map__overlap(pos, map))
			continue;

		if (verbose >= 2) {
			fputs("overlapping maps:\n", fp);
			map__fprintf(map, fp);
			map__fprintf(pos, fp);
		}

		rb_erase(&pos->rb_node, root);
		/*
		 * Now check if we need to create new maps for areas not
		 * overlapped by the new map:
		 */
		if (map->start > pos->start) {
			struct map *before = map__clone(pos);

			if (before == NULL) {
				err = -ENOMEM;
				goto move_map;
			}

			before->end = map->start - 1;
<<<<<<< HEAD
<<<<<<< HEAD
			map_groups__insert(self, before);
=======
			map_groups__insert(mg, before);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			map_groups__insert(mg, before);
>>>>>>> refs/remotes/origin/master
			if (verbose >= 2)
				map__fprintf(before, fp);
		}

		if (map->end < pos->end) {
			struct map *after = map__clone(pos);

			if (after == NULL) {
				err = -ENOMEM;
				goto move_map;
			}

			after->start = map->end + 1;
<<<<<<< HEAD
<<<<<<< HEAD
			map_groups__insert(self, after);
=======
			map_groups__insert(mg, after);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			map_groups__insert(mg, after);
>>>>>>> refs/remotes/origin/master
			if (verbose >= 2)
				map__fprintf(after, fp);
		}
move_map:
		/*
		 * If we have references, just move them to a separate list.
		 */
		if (pos->referenced)
<<<<<<< HEAD
<<<<<<< HEAD
			list_add_tail(&pos->node, &self->removed_maps[map->type]);
=======
			list_add_tail(&pos->node, &mg->removed_maps[map->type]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			list_add_tail(&pos->node, &mg->removed_maps[map->type]);
>>>>>>> refs/remotes/origin/master
		else
			map__delete(pos);

		if (err)
			return err;
	}

	return 0;
}

/*
 * XXX This should not really _copy_ te maps, but refcount them.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int map_groups__clone(struct map_groups *self,
=======
int map_groups__clone(struct map_groups *mg,
>>>>>>> refs/remotes/origin/cm-10.0
=======
int map_groups__clone(struct map_groups *mg,
>>>>>>> refs/remotes/origin/master
		      struct map_groups *parent, enum map_type type)
{
	struct rb_node *nd;
	for (nd = rb_first(&parent->maps[type]); nd; nd = rb_next(nd)) {
		struct map *map = rb_entry(nd, struct map, rb_node);
		struct map *new = map__clone(map);
		if (new == NULL)
			return -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
		map_groups__insert(self, new);
=======
		map_groups__insert(mg, new);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		map_groups__insert(mg, new);
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

<<<<<<< HEAD
static u64 map__reloc_map_ip(struct map *map, u64 ip)
{
	return ip + (s64)map->pgoff;
}

static u64 map__reloc_unmap_ip(struct map *map, u64 ip)
{
	return ip - (s64)map->pgoff;
}

void map__reloc_vmlinux(struct map *self)
{
	struct kmap *kmap = map__kmap(self);
	s64 reloc;

	if (!kmap->ref_reloc_sym || !kmap->ref_reloc_sym->unrelocated_addr)
		return;

	reloc = (kmap->ref_reloc_sym->unrelocated_addr -
		 kmap->ref_reloc_sym->addr);

	if (!reloc)
		return;

	self->map_ip   = map__reloc_map_ip;
	self->unmap_ip = map__reloc_unmap_ip;
	self->pgoff    = reloc;
}

=======
>>>>>>> refs/remotes/origin/master
void maps__insert(struct rb_root *maps, struct map *map)
{
	struct rb_node **p = &maps->rb_node;
	struct rb_node *parent = NULL;
	const u64 ip = map->start;
	struct map *m;

	while (*p != NULL) {
		parent = *p;
		m = rb_entry(parent, struct map, rb_node);
		if (ip < m->start)
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

	rb_link_node(&map->rb_node, parent, p);
	rb_insert_color(&map->rb_node, maps);
}

<<<<<<< HEAD
void maps__remove(struct rb_root *self, struct map *map)
{
	rb_erase(&map->rb_node, self);
=======
void maps__remove(struct rb_root *maps, struct map *map)
{
	rb_erase(&map->rb_node, maps);
>>>>>>> refs/remotes/origin/master
}

struct map *maps__find(struct rb_root *maps, u64 ip)
{
	struct rb_node **p = &maps->rb_node;
	struct rb_node *parent = NULL;
	struct map *m;

	while (*p != NULL) {
		parent = *p;
		m = rb_entry(parent, struct map, rb_node);
		if (ip < m->start)
			p = &(*p)->rb_left;
		else if (ip > m->end)
			p = &(*p)->rb_right;
		else
			return m;
	}

	return NULL;
}

<<<<<<< HEAD
int machine__init(struct machine *self, const char *root_dir, pid_t pid)
{
	map_groups__init(&self->kmaps);
	RB_CLEAR_NODE(&self->rb_node);
	INIT_LIST_HEAD(&self->user_dsos);
	INIT_LIST_HEAD(&self->kernel_dsos);

<<<<<<< HEAD
=======
	self->threads = RB_ROOT;
	INIT_LIST_HEAD(&self->dead_threads);
	self->last_match = NULL;

>>>>>>> refs/remotes/origin/cm-10.0
	self->kmaps.machine = self;
	self->pid	    = pid;
	self->root_dir      = strdup(root_dir);
	return self->root_dir == NULL ? -ENOMEM : 0;
}

static void dsos__delete(struct list_head *self)
{
	struct dso *pos, *n;

	list_for_each_entry_safe(pos, n, self, node) {
		list_del(&pos->node);
		dso__delete(pos);
	}
}

void machine__exit(struct machine *self)
{
	map_groups__exit(&self->kmaps);
	dsos__delete(&self->user_dsos);
	dsos__delete(&self->kernel_dsos);
	free(self->root_dir);
	self->root_dir = NULL;
}

void machine__delete(struct machine *self)
{
	machine__exit(self);
	free(self);
}

struct machine *machines__add(struct rb_root *self, pid_t pid,
			      const char *root_dir)
{
	struct rb_node **p = &self->rb_node;
	struct rb_node *parent = NULL;
	struct machine *pos, *machine = malloc(sizeof(*machine));

	if (!machine)
		return NULL;

	if (machine__init(machine, root_dir, pid) != 0) {
		free(machine);
		return NULL;
	}

	while (*p != NULL) {
		parent = *p;
		pos = rb_entry(parent, struct machine, rb_node);
		if (pid < pos->pid)
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

	rb_link_node(&machine->rb_node, parent, p);
	rb_insert_color(&machine->rb_node, self);

	return machine;
}

struct machine *machines__find(struct rb_root *self, pid_t pid)
{
	struct rb_node **p = &self->rb_node;
	struct rb_node *parent = NULL;
	struct machine *machine;
	struct machine *default_machine = NULL;

	while (*p != NULL) {
		parent = *p;
		machine = rb_entry(parent, struct machine, rb_node);
		if (pid < machine->pid)
			p = &(*p)->rb_left;
		else if (pid > machine->pid)
			p = &(*p)->rb_right;
		else
			return machine;
		if (!machine->pid)
			default_machine = machine;
	}

	return default_machine;
}

struct machine *machines__findnew(struct rb_root *self, pid_t pid)
{
	char path[PATH_MAX];
	const char *root_dir;
	struct machine *machine = machines__find(self, pid);

	if (!machine || machine->pid != pid) {
		if (pid == HOST_KERNEL_ID || pid == DEFAULT_GUEST_KERNEL_ID)
			root_dir = "";
		else {
			if (!symbol_conf.guestmount)
				goto out;
			sprintf(path, "%s/%d", symbol_conf.guestmount, pid);
			if (access(path, R_OK)) {
				pr_err("Can't access file %s\n", path);
				goto out;
			}
			root_dir = path;
		}
		machine = machines__add(self, pid, root_dir);
	}

out:
	return machine;
}

void machines__process(struct rb_root *self, machine__process_t process, void *data)
{
	struct rb_node *nd;

	for (nd = rb_first(self); nd; nd = rb_next(nd)) {
		struct machine *pos = rb_entry(nd, struct machine, rb_node);
		process(pos, data);
	}
}

char *machine__mmap_name(struct machine *self, char *bf, size_t size)
{
	if (machine__is_host(self))
		snprintf(bf, size, "[%s]", "kernel.kallsyms");
	else if (machine__is_default_guest(self))
		snprintf(bf, size, "[%s]", "guest.kernel.kallsyms");
	else
		snprintf(bf, size, "[%s.%d]", "guest.kernel.kallsyms", self->pid);

	return bf;
=======
struct map *maps__first(struct rb_root *maps)
{
	struct rb_node *first = rb_first(maps);

	if (first)
		return rb_entry(first, struct map, rb_node);
	return NULL;
}

struct map *maps__next(struct map *map)
{
	struct rb_node *next = rb_next(&map->rb_node);

	if (next)
		return rb_entry(next, struct map, rb_node);
	return NULL;
>>>>>>> refs/remotes/origin/master
}
