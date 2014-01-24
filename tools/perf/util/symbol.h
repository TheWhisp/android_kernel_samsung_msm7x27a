#ifndef __PERF_SYMBOL
#define __PERF_SYMBOL 1

#include <linux/types.h>
#include <stdbool.h>
#include <stdint.h>
#include "map.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "../perf.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/list.h>
#include <linux/rbtree.h>
#include <stdio.h>

#ifdef HAVE_CPLUS_DEMANGLE
extern char *cplus_demangle(const char *, int);

static inline char *bfd_demangle(void __used *v, const char *c, int i)
=======
#include "../perf.h"
#include <linux/list.h>
#include <linux/rbtree.h>
#include <stdio.h>
#include <byteswap.h>
#include <libgen.h>
#include "build-id.h"

#ifdef HAVE_LIBELF_SUPPORT
#include <libelf.h>
#include <gelf.h>
#endif
#include <elf.h>

#include "dso.h"

#ifdef HAVE_CPLUS_DEMANGLE_SUPPORT
extern char *cplus_demangle(const char *, int);

static inline char *bfd_demangle(void __maybe_unused *v, const char *c, int i)
>>>>>>> refs/remotes/origin/master
{
	return cplus_demangle(c, i);
}
#else
#ifdef NO_DEMANGLE
<<<<<<< HEAD
static inline char *bfd_demangle(void __used *v, const char __used *c,
				 int __used i)
=======
static inline char *bfd_demangle(void __maybe_unused *v,
				 const char __maybe_unused *c,
				 int __maybe_unused i)
>>>>>>> refs/remotes/origin/master
{
	return NULL;
}
#else
<<<<<<< HEAD
=======
#define PACKAGE 'perf'
>>>>>>> refs/remotes/origin/master
#include <bfd.h>
#endif
#endif

<<<<<<< HEAD
int hex2u64(const char *ptr, u64 *val);
char *strxfrchar(char *s, char from, char to);

=======
>>>>>>> refs/remotes/origin/master
/*
 * libelf 0.8.x and earlier do not support ELF_C_READ_MMAP;
 * for newer versions we can use mmap to reduce memory usage:
 */
<<<<<<< HEAD
#ifdef LIBELF_NO_MMAP
# define PERF_ELF_C_READ_MMAP ELF_C_READ
#else
# define PERF_ELF_C_READ_MMAP ELF_C_READ_MMAP
=======
#ifdef HAVE_LIBELF_MMAP_SUPPORT
# define PERF_ELF_C_READ_MMAP ELF_C_READ_MMAP
#else
# define PERF_ELF_C_READ_MMAP ELF_C_READ
#endif

#ifdef HAVE_LIBELF_SUPPORT
extern Elf_Scn *elf_section_by_name(Elf *elf, GElf_Ehdr *ep,
				GElf_Shdr *shp, const char *name, size_t *idx);
>>>>>>> refs/remotes/origin/master
#endif

#ifndef DMGL_PARAMS
#define DMGL_PARAMS      (1 << 0)       /* Include function args */
#define DMGL_ANSI        (1 << 1)       /* Include const, volatile, etc */
#endif

<<<<<<< HEAD
#define BUILD_ID_SIZE 20

=======
>>>>>>> refs/remotes/origin/master
/** struct symbol - symtab entry
 *
 * @ignore - resolvable but tools ignore it (e.g. idle routines)
 */
struct symbol {
	struct rb_node	rb_node;
	u64		start;
	u64		end;
	u16		namelen;
	u8		binding;
	bool		ignore;
	char		name[0];
};

void symbol__delete(struct symbol *sym);
<<<<<<< HEAD
=======
void symbols__delete(struct rb_root *symbols);

static inline size_t symbol__size(const struct symbol *sym)
{
	return sym->end - sym->start + 1;
}
>>>>>>> refs/remotes/origin/master

struct strlist;

struct symbol_conf {
	unsigned short	priv_size;
<<<<<<< HEAD
<<<<<<< HEAD
	bool		try_vmlinux_path,
			use_modules,
			sort_by_name,
			show_nr_samples,
=======
	unsigned short	nr_events;
	bool		try_vmlinux_path,
=======
	unsigned short	nr_events;
	bool		try_vmlinux_path,
			ignore_vmlinux,
>>>>>>> refs/remotes/origin/master
			show_kernel_path,
			use_modules,
			sort_by_name,
			show_nr_samples,
			show_total_period,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			use_callchain,
			exclude_other,
			show_cpu_utilization,
			initialized,
<<<<<<< HEAD
<<<<<<< HEAD
			kptr_restrict;
=======
			kptr_restrict,
			annotate_asm_raw,
			annotate_src;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			kptr_restrict,
			annotate_asm_raw,
			annotate_src,
			event_group,
			demangle;
>>>>>>> refs/remotes/origin/master
	const char	*vmlinux_name,
			*kallsyms_name,
			*source_prefix,
			*field_sep;
	const char	*default_guest_vmlinux_name,
			*default_guest_kallsyms,
			*default_guest_modules;
	const char	*guestmount;
	const char	*dso_list_str,
			*comm_list_str,
			*sym_list_str,
			*col_width_list_str;
       struct strlist	*dso_list,
			*comm_list,
<<<<<<< HEAD
<<<<<<< HEAD
			*sym_list;
=======
=======
>>>>>>> refs/remotes/origin/master
			*sym_list,
			*dso_from_list,
			*dso_to_list,
			*sym_from_list,
			*sym_to_list;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	const char	*symfs;
};

extern struct symbol_conf symbol_conf;
<<<<<<< HEAD
=======
extern int vmlinux_path__nr_entries;
extern char **vmlinux_path;
>>>>>>> refs/remotes/origin/master

static inline void *symbol__priv(struct symbol *sym)
{
	return ((void *)sym) - symbol_conf.priv_size;
}

struct ref_reloc_sym {
	const char	*name;
	u64		addr;
	u64		unrelocated_addr;
};

struct map_symbol {
	struct map    *map;
	struct symbol *sym;
	bool	      unfolded;
	bool	      has_children;
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
struct addr_map_symbol {
	struct map    *map;
	struct symbol *sym;
	u64	      addr;
	u64	      al_addr;
};

struct branch_info {
	struct addr_map_symbol from;
	struct addr_map_symbol to;
	struct branch_flags flags;
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
struct addr_location {
=======
struct mem_info {
	struct addr_map_symbol iaddr;
	struct addr_map_symbol daddr;
	union perf_mem_data_src data_src;
};

struct addr_location {
	struct machine *machine;
>>>>>>> refs/remotes/origin/master
	struct thread *thread;
	struct map    *map;
	struct symbol *sym;
	u64	      addr;
	char	      level;
	bool	      filtered;
	u8	      cpumode;
	s32	      cpu;
};

<<<<<<< HEAD
enum dso_kernel_type {
	DSO_TYPE_USER = 0,
	DSO_TYPE_KERNEL,
	DSO_TYPE_GUEST_KERNEL
};

struct dso {
	struct list_head node;
	struct rb_root	 symbols[MAP__NR_TYPES];
	struct rb_root	 symbol_names[MAP__NR_TYPES];
	enum dso_kernel_type	kernel;
	u8		 adjust_symbols:1;
	u8		 has_build_id:1;
	u8		 hit:1;
	u8		 annotate_warned:1;
	u8		 sname_alloc:1;
	u8		 lname_alloc:1;
	unsigned char	 symtab_type;
	u8		 sorted_by_name;
	u8		 loaded;
	u8		 build_id[BUILD_ID_SIZE];
	const char	 *short_name;
	char	 	 *long_name;
	u16		 long_name_len;
	u16		 short_name_len;
	char		 name[0];
};

struct dso *dso__new(const char *name);
<<<<<<< HEAD
struct dso *dso__new_kernel(const char *name);
=======
>>>>>>> refs/remotes/origin/cm-10.0
void dso__delete(struct dso *dso);

int dso__name_len(const struct dso *dso);

bool dso__loaded(const struct dso *dso, enum map_type type);
bool dso__sorted_by_name(const struct dso *dso, enum map_type type);

static inline void dso__set_loaded(struct dso *dso, enum map_type type)
{
	dso->loaded |= (1 << type);
}

void dso__sort_by_name(struct dso *dso, enum map_type type);

struct dso *__dsos__findnew(struct list_head *head, const char *name);

int dso__load(struct dso *dso, struct map *map, symbol_filter_t filter);
int dso__load_vmlinux(struct dso *dso, struct map *map,
		      const char *vmlinux, symbol_filter_t filter);
=======
struct symsrc {
	char *name;
	int fd;
	enum dso_binary_type type;

#ifdef HAVE_LIBELF_SUPPORT
	Elf *elf;
	GElf_Ehdr ehdr;

	Elf_Scn *opdsec;
	size_t opdidx;
	GElf_Shdr opdshdr;

	Elf_Scn *symtab;
	GElf_Shdr symshdr;

	Elf_Scn *dynsym;
	size_t dynsym_idx;
	GElf_Shdr dynshdr;

	bool adjust_symbols;
#endif
};

void symsrc__destroy(struct symsrc *ss);
int symsrc__init(struct symsrc *ss, struct dso *dso, const char *name,
		 enum dso_binary_type type);
bool symsrc__has_symtab(struct symsrc *ss);
bool symsrc__possibly_runtime(struct symsrc *ss);

int dso__load(struct dso *dso, struct map *map, symbol_filter_t filter);
int dso__load_vmlinux(struct dso *dso, struct map *map,
		      const char *vmlinux, bool vmlinux_allocated,
		      symbol_filter_t filter);
>>>>>>> refs/remotes/origin/master
int dso__load_vmlinux_path(struct dso *dso, struct map *map,
			   symbol_filter_t filter);
int dso__load_kallsyms(struct dso *dso, const char *filename, struct map *map,
		       symbol_filter_t filter);
<<<<<<< HEAD
int machine__load_kallsyms(struct machine *machine, const char *filename,
			   enum map_type type, symbol_filter_t filter);
int machine__load_vmlinux_path(struct machine *machine, enum map_type type,
			       symbol_filter_t filter);

size_t __dsos__fprintf(struct list_head *head, FILE *fp);

size_t machine__fprintf_dsos_buildid(struct machine *machine,
				     FILE *fp, bool with_hits);
size_t machines__fprintf_dsos(struct rb_root *machines, FILE *fp);
size_t machines__fprintf_dsos_buildid(struct rb_root *machines,
				      FILE *fp, bool with_hits);
size_t dso__fprintf_buildid(struct dso *dso, FILE *fp);
size_t dso__fprintf_symbols_by_name(struct dso *dso,
				    enum map_type type, FILE *fp);
size_t dso__fprintf(struct dso *dso, enum map_type type, FILE *fp);

enum symtab_type {
	SYMTAB__KALLSYMS = 0,
	SYMTAB__GUEST_KALLSYMS,
	SYMTAB__JAVA_JIT,
	SYMTAB__BUILD_ID_CACHE,
	SYMTAB__FEDORA_DEBUGINFO,
	SYMTAB__UBUNTU_DEBUGINFO,
	SYMTAB__BUILDID_DEBUGINFO,
	SYMTAB__SYSTEM_PATH_DSO,
	SYMTAB__GUEST_KMODULE,
	SYMTAB__SYSTEM_PATH_KMODULE,
	SYMTAB__NOT_FOUND,
};

char dso__symtab_origin(const struct dso *dso);
void dso__set_long_name(struct dso *dso, char *name);
void dso__set_build_id(struct dso *dso, void *build_id);
void dso__read_running_kernel_build_id(struct dso *dso,
				       struct machine *machine);
=======

>>>>>>> refs/remotes/origin/master
struct symbol *dso__find_symbol(struct dso *dso, enum map_type type,
				u64 addr);
struct symbol *dso__find_symbol_by_name(struct dso *dso, enum map_type type,
					const char *name);
<<<<<<< HEAD

int filename__read_build_id(const char *filename, void *bf, size_t size);
int sysfs__read_build_id(const char *filename, void *bf, size_t size);
bool __dsos__read_build_ids(struct list_head *head, bool with_hits);
int build_id__sprintf(const u8 *build_id, int len, char *bf);
int kallsyms__parse(const char *filename, void *arg,
		    int (*process_symbol)(void *arg, const char *name,
					  char type, u64 start, u64 end));

void machine__destroy_kernel_maps(struct machine *machine);
int __machine__create_kernel_maps(struct machine *machine, struct dso *kernel);
int machine__create_kernel_maps(struct machine *machine);

int machines__create_kernel_maps(struct rb_root *machines, pid_t pid);
int machines__create_guest_kernel_maps(struct rb_root *machines);
void machines__destroy_guest_kernel_maps(struct rb_root *machines);

int symbol__init(void);
void symbol__exit(void);
<<<<<<< HEAD
=======
size_t symbol__fprintf_symname_offs(const struct symbol *sym,
				    const struct addr_location *al, FILE *fp);
size_t symbol__fprintf_symname(const struct symbol *sym, FILE *fp);
>>>>>>> refs/remotes/origin/cm-10.0
bool symbol_type__is_a(char symbol_type, enum map_type map_type);

size_t machine__fprintf_vmlinux_path(struct machine *machine, FILE *fp);
=======
struct symbol *dso__first_symbol(struct dso *dso, enum map_type type);

int filename__read_build_id(const char *filename, void *bf, size_t size);
int sysfs__read_build_id(const char *filename, void *bf, size_t size);
int modules__parse(const char *filename, void *arg,
		   int (*process_module)(void *arg, const char *name,
					 u64 start));
int filename__read_debuglink(const char *filename, char *debuglink,
			     size_t size);

int symbol__init(void);
void symbol__exit(void);
void symbol__elf_init(void);
struct symbol *symbol__new(u64 start, u64 len, u8 binding, const char *name);
size_t symbol__fprintf_symname_offs(const struct symbol *sym,
				    const struct addr_location *al, FILE *fp);
size_t symbol__fprintf_symname(const struct symbol *sym, FILE *fp);
size_t symbol__fprintf(struct symbol *sym, FILE *fp);
bool symbol_type__is_a(char symbol_type, enum map_type map_type);
bool symbol__restricted_filename(const char *filename,
				 const char *restricted_filename);
bool symbol__is_idle(struct symbol *sym);

int dso__load_sym(struct dso *dso, struct map *map, struct symsrc *syms_ss,
		  struct symsrc *runtime_ss, symbol_filter_t filter,
		  int kmodule);
int dso__synthesize_plt_symbols(struct dso *dso, struct symsrc *ss,
				struct map *map, symbol_filter_t filter);

void symbols__insert(struct rb_root *symbols, struct symbol *sym);
void symbols__fixup_duplicate(struct rb_root *symbols);
void symbols__fixup_end(struct rb_root *symbols);
void __map_groups__fixup_end(struct map_groups *mg, enum map_type type);

typedef int (*mapfn_t)(u64 start, u64 len, u64 pgoff, void *data);
int file__read_maps(int fd, bool exe, mapfn_t mapfn, void *data,
		    bool *is_64_bit);

#define PERF_KCORE_EXTRACT "/tmp/perf-kcore-XXXXXX"

struct kcore_extract {
	char *kcore_filename;
	u64 addr;
	u64 offs;
	u64 len;
	char extract_filename[sizeof(PERF_KCORE_EXTRACT)];
	int fd;
};

int kcore_extract__create(struct kcore_extract *kce);
void kcore_extract__delete(struct kcore_extract *kce);

int kcore_copy(const char *from_dir, const char *to_dir);
int compare_proc_modules(const char *from, const char *to);

int setup_list(struct strlist **list, const char *list_str,
	       const char *list_name);
>>>>>>> refs/remotes/origin/master

#endif /* __PERF_SYMBOL */
