#ifndef __PERF_THREAD_MAP_H
#define __PERF_THREAD_MAP_H

#include <sys/types.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <stdio.h>
>>>>>>> refs/remotes/origin/cm-10.0

struct thread_map {
	int nr;
	int map[];
=======
#include <stdio.h>

struct thread_map {
	int nr;
	pid_t map[];
>>>>>>> refs/remotes/origin/master
};

struct thread_map *thread_map__new_by_pid(pid_t pid);
struct thread_map *thread_map__new_by_tid(pid_t tid);
<<<<<<< HEAD
<<<<<<< HEAD
struct thread_map *thread_map__new(pid_t pid, pid_t tid);
void thread_map__delete(struct thread_map *threads);
=======
=======
>>>>>>> refs/remotes/origin/master
struct thread_map *thread_map__new_by_uid(uid_t uid);
struct thread_map *thread_map__new(pid_t pid, pid_t tid, uid_t uid);

struct thread_map *thread_map__new_str(const char *pid,
		const char *tid, uid_t uid);

void thread_map__delete(struct thread_map *threads);

size_t thread_map__fprintf(struct thread_map *threads, FILE *fp);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int thread_map__nr(struct thread_map *threads)
{
	return threads ? threads->nr : 1;
}

>>>>>>> refs/remotes/origin/master
#endif	/* __PERF_THREAD_MAP_H */
