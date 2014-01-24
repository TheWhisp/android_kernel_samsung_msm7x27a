#ifndef _PERF_UI_H_
#define _PERF_UI_H_ 1

#include <pthread.h>
<<<<<<< HEAD

extern pthread_mutex_t ui__lock;

=======
#include <stdbool.h>

extern pthread_mutex_t ui__lock;

void ui__refresh_dimensions(bool force);

>>>>>>> refs/remotes/origin/cm-10.0
#endif /* _PERF_UI_H_ */
