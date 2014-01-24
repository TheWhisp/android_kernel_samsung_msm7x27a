#ifndef _PERF_UI_PROGRESS_H_
#define _PERF_UI_PROGRESS_H_ 1

<<<<<<< HEAD
struct ui_progress;

struct ui_progress *ui_progress__new(const char *title, u64 total);
void ui_progress__delete(struct ui_progress *self);

void ui_progress__update(struct ui_progress *self, u64 curr);
=======
#include <../types.h>

void ui_progress__update(u64 curr, u64 total, const char *title);
>>>>>>> refs/remotes/origin/cm-10.0

#endif
