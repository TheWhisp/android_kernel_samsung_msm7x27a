/*
 * (c) 2009 Arnaldo Carvalho de Melo <acme@redhat.com>
 *
 * Licensed under the GPLv2.
 */

#include "strlist.h"
<<<<<<< HEAD
=======
#include "util.h"
>>>>>>> refs/remotes/origin/master
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
static struct str_node *str_node__new(const char *s, bool dupstr)
{
	struct str_node *self = malloc(sizeof(*self));

	if (self != NULL) {
		if (dupstr) {
=======
static
struct rb_node *strlist__node_new(struct rblist *rblist, const void *entry)
{
	const char *s = entry;
	struct rb_node *rc = NULL;
	struct strlist *strlist = container_of(rblist, struct strlist, rblist);
	struct str_node *snode = malloc(sizeof(*snode));

	if (snode != NULL) {
		if (strlist->dupstr) {
>>>>>>> refs/remotes/origin/master
			s = strdup(s);
			if (s == NULL)
				goto out_delete;
		}
<<<<<<< HEAD
		self->s = s;
	}

	return self;

out_delete:
	free(self);
	return NULL;
}

static void str_node__delete(struct str_node *self, bool dupstr)
{
	if (dupstr)
		free((void *)self->s);
	free(self);
}

int strlist__add(struct strlist *self, const char *new_entry)
{
	struct rb_node **p = &self->entries.rb_node;
	struct rb_node *parent = NULL;
	struct str_node *sn;

	while (*p != NULL) {
		int rc;

		parent = *p;
		sn = rb_entry(parent, struct str_node, rb_node);
		rc = strcmp(sn->s, new_entry);

		if (rc > 0)
			p = &(*p)->rb_left;
		else if (rc < 0)
			p = &(*p)->rb_right;
		else
			return -EEXIST;
	}

	sn = str_node__new(new_entry, self->dupstr);
	if (sn == NULL)
		return -ENOMEM;

	rb_link_node(&sn->rb_node, parent, p);
	rb_insert_color(&sn->rb_node, &self->entries);
	++self->nr_entries;

	return 0;
}

int strlist__load(struct strlist *self, const char *filename)
=======
		snode->s = s;
		rc = &snode->rb_node;
	}

	return rc;

out_delete:
	free(snode);
	return NULL;
}

static void str_node__delete(struct str_node *snode, bool dupstr)
{
	if (dupstr)
		zfree((char **)&snode->s);
	free(snode);
}

static
void strlist__node_delete(struct rblist *rblist, struct rb_node *rb_node)
{
	struct strlist *slist = container_of(rblist, struct strlist, rblist);
	struct str_node *snode = container_of(rb_node, struct str_node, rb_node);

	str_node__delete(snode, slist->dupstr);
}

static int strlist__node_cmp(struct rb_node *rb_node, const void *entry)
{
	const char *str = entry;
	struct str_node *snode = container_of(rb_node, struct str_node, rb_node);

	return strcmp(snode->s, str);
}

int strlist__add(struct strlist *slist, const char *new_entry)
{
	return rblist__add_node(&slist->rblist, new_entry);
}

int strlist__load(struct strlist *slist, const char *filename)
>>>>>>> refs/remotes/origin/master
{
	char entry[1024];
	int err;
	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
		return errno;

	while (fgets(entry, sizeof(entry), fp) != NULL) {
		const size_t len = strlen(entry);

		if (len == 0)
			continue;
		entry[len - 1] = '\0';

<<<<<<< HEAD
		err = strlist__add(self, entry);
=======
		err = strlist__add(slist, entry);
>>>>>>> refs/remotes/origin/master
		if (err != 0)
			goto out;
	}

	err = 0;
out:
	fclose(fp);
	return err;
}

<<<<<<< HEAD
void strlist__remove(struct strlist *self, struct str_node *sn)
{
	rb_erase(&sn->rb_node, &self->entries);
	str_node__delete(sn, self->dupstr);
}

struct str_node *strlist__find(struct strlist *self, const char *entry)
{
	struct rb_node **p = &self->entries.rb_node;
	struct rb_node *parent = NULL;

	while (*p != NULL) {
		struct str_node *sn;
		int rc;

		parent = *p;
		sn = rb_entry(parent, struct str_node, rb_node);
		rc = strcmp(sn->s, entry);

		if (rc > 0)
			p = &(*p)->rb_left;
		else if (rc < 0)
			p = &(*p)->rb_right;
		else
			return sn;
	}

	return NULL;
}

static int strlist__parse_list_entry(struct strlist *self, const char *s)
{
	if (strncmp(s, "file://", 7) == 0)
		return strlist__load(self, s + 7);

	return strlist__add(self, s);
}

int strlist__parse_list(struct strlist *self, const char *s)
=======
void strlist__remove(struct strlist *slist, struct str_node *snode)
{
	rblist__remove_node(&slist->rblist, &snode->rb_node);
}

struct str_node *strlist__find(struct strlist *slist, const char *entry)
{
	struct str_node *snode = NULL;
	struct rb_node *rb_node = rblist__find(&slist->rblist, entry);

	if (rb_node)
		snode = container_of(rb_node, struct str_node, rb_node);

	return snode;
}

static int strlist__parse_list_entry(struct strlist *slist, const char *s)
{
	if (strncmp(s, "file://", 7) == 0)
		return strlist__load(slist, s + 7);

	return strlist__add(slist, s);
}

int strlist__parse_list(struct strlist *slist, const char *s)
>>>>>>> refs/remotes/origin/master
{
	char *sep;
	int err;

	while ((sep = strchr(s, ',')) != NULL) {
		*sep = '\0';
<<<<<<< HEAD
		err = strlist__parse_list_entry(self, s);
=======
		err = strlist__parse_list_entry(slist, s);
>>>>>>> refs/remotes/origin/master
		*sep = ',';
		if (err != 0)
			return err;
		s = sep + 1;
	}

<<<<<<< HEAD
	return *s ? strlist__parse_list_entry(self, s) : 0;
}

struct strlist *strlist__new(bool dupstr, const char *slist)
{
	struct strlist *self = malloc(sizeof(*self));

	if (self != NULL) {
		self->entries	 = RB_ROOT;
		self->dupstr	 = dupstr;
		self->nr_entries = 0;
		if (slist && strlist__parse_list(self, slist) != 0)
			goto out_error;
	}

	return self;
out_error:
	free(self);
	return NULL;
}

void strlist__delete(struct strlist *self)
{
	if (self != NULL) {
		struct str_node *pos;
		struct rb_node *next = rb_first(&self->entries);

		while (next) {
			pos = rb_entry(next, struct str_node, rb_node);
			next = rb_next(&pos->rb_node);
			strlist__remove(self, pos);
		}
		self->entries = RB_ROOT;
		free(self);
	}
}

struct str_node *strlist__entry(const struct strlist *self, unsigned int idx)
{
	struct rb_node *nd;

	for (nd = rb_first(&self->entries); nd; nd = rb_next(nd)) {
		struct str_node *pos = rb_entry(nd, struct str_node, rb_node);

		if (!idx--)
			return pos;
	}

	return NULL;
=======
	return *s ? strlist__parse_list_entry(slist, s) : 0;
}

struct strlist *strlist__new(bool dupstr, const char *list)
{
	struct strlist *slist = malloc(sizeof(*slist));

	if (slist != NULL) {
		rblist__init(&slist->rblist);
		slist->rblist.node_cmp    = strlist__node_cmp;
		slist->rblist.node_new    = strlist__node_new;
		slist->rblist.node_delete = strlist__node_delete;

		slist->dupstr	 = dupstr;
		if (list && strlist__parse_list(slist, list) != 0)
			goto out_error;
	}

	return slist;
out_error:
	free(slist);
	return NULL;
}

void strlist__delete(struct strlist *slist)
{
	if (slist != NULL)
		rblist__delete(&slist->rblist);
}

struct str_node *strlist__entry(const struct strlist *slist, unsigned int idx)
{
	struct str_node *snode = NULL;
	struct rb_node *rb_node;

	rb_node = rblist__entry(&slist->rblist, idx);
	if (rb_node)
		snode = container_of(rb_node, struct str_node, rb_node);

	return snode;
>>>>>>> refs/remotes/origin/master
}
