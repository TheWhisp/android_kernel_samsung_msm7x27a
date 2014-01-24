<<<<<<< HEAD
#include <stdlib.h>
#include <newt.h>
#include "../cache.h"
#include "progress.h"

struct ui_progress {
	newtComponent form, scale;
};

struct ui_progress *ui_progress__new(const char *title, u64 total)
{
	struct ui_progress *self = malloc(sizeof(*self));

	if (self != NULL) {
		int cols;

		if (use_browser <= 0)
			return self;
		newtGetScreenSize(&cols, NULL);
		cols -= 4;
		newtCenteredWindow(cols, 1, title);
		self->form  = newtForm(NULL, NULL, 0);
		if (self->form == NULL)
			goto out_free_self;
		self->scale = newtScale(0, 0, cols, total);
		if (self->scale == NULL)
			goto out_free_form;
		newtFormAddComponent(self->form, self->scale);
		newtRefresh();
	}

	return self;

out_free_form:
	newtFormDestroy(self->form);
out_free_self:
	free(self);
	return NULL;
}

void ui_progress__update(struct ui_progress *self, u64 curr)
{
=======
#include "../cache.h"
#include "progress.h"
#include "libslang.h"
#include "ui.h"
#include "browser.h"

void ui_progress__update(u64 curr, u64 total, const char *title)
{
	int bar, y;
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * FIXME: We should have a per UI backend way of showing progress,
	 * stdio will just show a percentage as NN%, etc.
	 */
	if (use_browser <= 0)
		return;
<<<<<<< HEAD
	newtScaleSet(self->scale, curr);
	newtRefresh();
}

void ui_progress__delete(struct ui_progress *self)
{
	if (use_browser > 0) {
		newtFormDestroy(self->form);
		newtPopWindow();
	}
	free(self);
=======

	if (total == 0)
		return;

	ui__refresh_dimensions(true);
	pthread_mutex_lock(&ui__lock);
	y = SLtt_Screen_Rows / 2 - 2;
	SLsmg_set_color(0);
	SLsmg_draw_box(y, 0, 3, SLtt_Screen_Cols);
	SLsmg_gotorc(y++, 1);
	SLsmg_write_string((char *)title);
	SLsmg_set_color(HE_COLORSET_SELECTED);
	bar = ((SLtt_Screen_Cols - 2) * curr) / total;
	SLsmg_fill_region(y, 1, 1, bar, ' ');
	SLsmg_refresh();
	pthread_mutex_unlock(&ui__lock);
>>>>>>> refs/remotes/origin/cm-10.0
}
