#ifndef __PERF_THREAD_H
#define __PERF_THREAD_H

#include <linux/rbtree.h>
<<<<<<< HEAD
#include <unistd.h>
#include "symbol.h"
=======
#include <linux/list.h>
#include <unistd.h>
#include <sys/types.h>
#include "symbol.h"
#include <strlist.h>
>>>>>>> refs/remotes/origin/master

struct thread {
	union {
		struct rb_node	 rb_node;
		struct list_head node;
	};
	struct map_groups	mg;
<<<<<<< HEAD
	pid_t			pid;
	char			shortname[3];
	bool			comm_set;
	char			*comm;
	int			comm_len;
};

<<<<<<< HEAD
struct perf_session;
=======
struct machine;
>>>>>>> refs/remotes/origin/cm-10.0

void thread__delete(struct thread *self);

int thread__set_comm(struct thread *self, const char *comm);
int thread__comm_len(struct thread *self);
<<<<<<< HEAD
struct thread *perf_session__findnew(struct perf_session *self, pid_t pid);
void thread__insert_map(struct thread *self, struct map *map);
int thread__fork(struct thread *self, struct thread *parent);
size_t perf_session__fprintf(struct perf_session *self, FILE *fp);
=======
void thread__insert_map(struct thread *self, struct map *map);
int thread__fork(struct thread *self, struct thread *parent);
>>>>>>> refs/remotes/origin/cm-10.0

static inline struct map *thread__find_map(struct thread *self,
					   enum map_type type, u64 addr)
{
	return self ? map_groups__find(&self->mg, type, addr) : NULL;
}

<<<<<<< HEAD
void thread__find_addr_map(struct thread *self,
			   struct perf_session *session, u8 cpumode,
			   enum map_type type, pid_t pid, u64 addr,
			   struct addr_location *al);

void thread__find_addr_location(struct thread *self,
				struct perf_session *session, u8 cpumode,
				enum map_type type, pid_t pid, u64 addr,
=======
=======
	pid_t			pid_; /* Not all tools update this */
	pid_t			tid;
	pid_t			ppid;
	char			shortname[3];
	bool			comm_set;
	bool			dead; /* if set thread has exited */
	struct list_head	comm_list;
	int			comm_len;

	void			*priv;
};

struct machine;
struct comm;

struct thread *thread__new(pid_t pid, pid_t tid);
void thread__delete(struct thread *thread);
static inline void thread__exited(struct thread *thread)
{
	thread->dead = true;
}

int thread__set_comm(struct thread *thread, const char *comm, u64 timestamp);
int thread__comm_len(struct thread *thread);
struct comm *thread__comm(const struct thread *thread);
const char *thread__comm_str(const struct thread *thread);
void thread__insert_map(struct thread *thread, struct map *map);
int thread__fork(struct thread *thread, struct thread *parent, u64 timestamp);
size_t thread__fprintf(struct thread *thread, FILE *fp);

static inline struct map *thread__find_map(struct thread *thread,
					   enum map_type type, u64 addr)
{
	return thread ? map_groups__find(&thread->mg, type, addr) : NULL;
}

>>>>>>> refs/remotes/origin/master
void thread__find_addr_map(struct thread *thread, struct machine *machine,
			   u8 cpumode, enum map_type type, u64 addr,
			   struct addr_location *al);

void thread__find_addr_location(struct thread *thread, struct machine *machine,
				u8 cpumode, enum map_type type, u64 addr,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
				struct addr_location *al,
				symbol_filter_t filter);
=======
				struct addr_location *al);

static inline void *thread__priv(struct thread *thread)
{
	return thread->priv;
}

static inline void thread__set_priv(struct thread *thread, void *p)
{
	thread->priv = p;
}

static inline bool thread__is_filtered(struct thread *thread)
{
	if (symbol_conf.comm_list &&
	    !strlist__has_entry(symbol_conf.comm_list, thread__comm_str(thread))) {
		return true;
	}

	return false;
}

>>>>>>> refs/remotes/origin/master
#endif	/* __PERF_THREAD_H */
