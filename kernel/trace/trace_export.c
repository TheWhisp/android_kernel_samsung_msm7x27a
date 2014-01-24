/*
 * trace_export.c - export basic ftrace utilities to user space
 *
 * Copyright (C) 2009 Steven Rostedt <srostedt@redhat.com>
 */
#include <linux/stringify.h>
#include <linux/kallsyms.h>
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/ftrace.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#include "trace_output.h"

#undef TRACE_SYSTEM
#define TRACE_SYSTEM	ftrace

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * The FTRACE_ENTRY_REG macro allows ftrace entry to define register
 * function and thus become accesible via perf.
 */
#undef FTRACE_ENTRY_REG
#define FTRACE_ENTRY_REG(name, struct_name, id, tstruct, print, \
			 filter, regfn) \
	FTRACE_ENTRY(name, struct_name, id, PARAMS(tstruct), PARAMS(print), \
		     filter)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* not needed for this file */
#undef __field_struct
#define __field_struct(type, item)

#undef __field
#define __field(type, item)				type item;

#undef __field_desc
#define __field_desc(type, container, item)		type item;

#undef __array
#define __array(type, item, size)			type item[size];

#undef __array_desc
#define __array_desc(type, container, item, size)	type item[size];

#undef __dynamic_array
#define __dynamic_array(type, item)			type item[];

#undef F_STRUCT
#define F_STRUCT(args...)				args

#undef F_printk
#define F_printk(fmt, args...) fmt, args

#undef FTRACE_ENTRY
<<<<<<< HEAD
<<<<<<< HEAD
#define FTRACE_ENTRY(name, struct_name, id, tstruct, print)	\
struct ____ftrace_##name {					\
	tstruct							\
};								\
static void __always_unused ____ftrace_check_##name(void)	\
{								\
	struct ____ftrace_##name *__entry = NULL;		\
								\
	/* force compile-time check on F_printk() */		\
	printk(print);						\
}

#undef FTRACE_ENTRY_DUP
#define FTRACE_ENTRY_DUP(name, struct_name, id, tstruct, print)	\
	FTRACE_ENTRY(name, struct_name, id, PARAMS(tstruct), PARAMS(print))
=======
=======
>>>>>>> refs/remotes/origin/master
#define FTRACE_ENTRY(name, struct_name, id, tstruct, print, filter)	\
struct ____ftrace_##name {						\
	tstruct								\
};									\
static void __always_unused ____ftrace_check_##name(void)		\
{									\
	struct ____ftrace_##name *__entry = NULL;			\
									\
	/* force compile-time check on F_printk() */			\
	printk(print);							\
}

#undef FTRACE_ENTRY_DUP
#define FTRACE_ENTRY_DUP(name, struct_name, id, tstruct, print, filter)	\
	FTRACE_ENTRY(name, struct_name, id, PARAMS(tstruct), PARAMS(print), \
		     filter)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#include "trace_entries.h"

#undef __field
#define __field(type, item)						\
	ret = trace_define_field(event_call, #type, #item,		\
				 offsetof(typeof(field), item),		\
				 sizeof(field.item),			\
<<<<<<< HEAD
<<<<<<< HEAD
				 is_signed_type(type), FILTER_OTHER);	\
=======
				 is_signed_type(type), filter_type);	\
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 is_signed_type(type), filter_type);	\
>>>>>>> refs/remotes/origin/master
	if (ret)							\
		return ret;

#undef __field_desc
#define __field_desc(type, container, item)	\
	ret = trace_define_field(event_call, #type, #item,		\
				 offsetof(typeof(field),		\
					  container.item),		\
				 sizeof(field.container.item),		\
<<<<<<< HEAD
<<<<<<< HEAD
				 is_signed_type(type), FILTER_OTHER);	\
=======
				 is_signed_type(type), filter_type);	\
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 is_signed_type(type), filter_type);	\
>>>>>>> refs/remotes/origin/master
	if (ret)							\
		return ret;

#undef __array
#define __array(type, item, len)					\
	do {								\
		BUILD_BUG_ON(len > MAX_FILTER_STR_VAL);			\
		mutex_lock(&event_storage_mutex);			\
		snprintf(event_storage, sizeof(event_storage),		\
			 "%s[%d]", #type, len);				\
		ret = trace_define_field(event_call, event_storage, #item, \
				 offsetof(typeof(field), item),		\
				 sizeof(field.item),			\
<<<<<<< HEAD
<<<<<<< HEAD
				 is_signed_type(type), FILTER_OTHER);	\
=======
				 is_signed_type(type), filter_type);	\
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 is_signed_type(type), filter_type);	\
>>>>>>> refs/remotes/origin/master
		mutex_unlock(&event_storage_mutex);			\
		if (ret)						\
			return ret;					\
	} while (0);

#undef __array_desc
#define __array_desc(type, container, item, len)			\
	BUILD_BUG_ON(len > MAX_FILTER_STR_VAL);				\
	ret = trace_define_field(event_call, #type "[" #len "]", #item,	\
				 offsetof(typeof(field),		\
					  container.item),		\
				 sizeof(field.container.item),		\
<<<<<<< HEAD
<<<<<<< HEAD
				 is_signed_type(type), FILTER_OTHER);	\
=======
				 is_signed_type(type), filter_type);	\
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 is_signed_type(type), filter_type);	\
>>>>>>> refs/remotes/origin/master
	if (ret)							\
		return ret;

#undef __dynamic_array
#define __dynamic_array(type, item)					\
	ret = trace_define_field(event_call, #type, #item,		\
				 offsetof(typeof(field), item),		\
<<<<<<< HEAD
<<<<<<< HEAD
				 0, is_signed_type(type), FILTER_OTHER);\
=======
				 0, is_signed_type(type), filter_type);\
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 0, is_signed_type(type), filter_type);\
>>>>>>> refs/remotes/origin/master
	if (ret)							\
		return ret;

#undef FTRACE_ENTRY
<<<<<<< HEAD
<<<<<<< HEAD
#define FTRACE_ENTRY(name, struct_name, id, tstruct, print)		\
=======
#define FTRACE_ENTRY(name, struct_name, id, tstruct, print, filter)	\
>>>>>>> refs/remotes/origin/cm-10.0
int									\
=======
#define FTRACE_ENTRY(name, struct_name, id, tstruct, print, filter)	\
static int __init							\
>>>>>>> refs/remotes/origin/master
ftrace_define_fields_##name(struct ftrace_event_call *event_call)	\
{									\
	struct struct_name field;					\
	int ret;							\
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int filter_type = filter;					\
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int filter_type = filter;					\
>>>>>>> refs/remotes/origin/master
									\
	tstruct;							\
									\
	return ret;							\
}

#include "trace_entries.h"

#undef __entry
#define __entry REC

#undef __field
#define __field(type, item)

#undef __field_desc
#define __field_desc(type, container, item)

#undef __array
#define __array(type, item, len)

#undef __array_desc
#define __array_desc(type, container, item, len)

#undef __dynamic_array
#define __dynamic_array(type, item)

#undef F_printk
#define F_printk(fmt, args...) __stringify(fmt) ", "  __stringify(args)

<<<<<<< HEAD
<<<<<<< HEAD
#undef FTRACE_ENTRY
#define FTRACE_ENTRY(call, struct_name, etype, tstruct, print)		\
=======
#undef FTRACE_ENTRY_REG
#define FTRACE_ENTRY_REG(call, struct_name, etype, tstruct, print, filter,\
			 regfn)						\
>>>>>>> refs/remotes/origin/cm-10.0
									\
struct ftrace_event_class event_class_ftrace_##call = {			\
	.system			= __stringify(TRACE_SYSTEM),		\
	.define_fields		= ftrace_define_fields_##call,		\
	.fields			= LIST_HEAD_INIT(event_class_ftrace_##call.fields),\
<<<<<<< HEAD
=======
	.reg			= regfn,				\
>>>>>>> refs/remotes/origin/cm-10.0
=======
#undef FTRACE_ENTRY_REG
#define FTRACE_ENTRY_REG(call, struct_name, etype, tstruct, print, filter,\
			 regfn)						\
									\
struct ftrace_event_class __refdata event_class_ftrace_##call = {	\
	.system			= __stringify(TRACE_SYSTEM),		\
	.define_fields		= ftrace_define_fields_##call,		\
	.fields			= LIST_HEAD_INIT(event_class_ftrace_##call.fields),\
	.reg			= regfn,				\
>>>>>>> refs/remotes/origin/master
};									\
									\
struct ftrace_event_call __used event_##call = {			\
	.name			= #call,				\
	.event.type		= etype,				\
	.class			= &event_class_ftrace_##call,		\
	.print_fmt		= print,				\
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.flags			= TRACE_EVENT_FL_IGNORE_ENABLE,		\
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.flags			= TRACE_EVENT_FL_IGNORE_ENABLE | TRACE_EVENT_FL_USE_CALL_FILTER, \
>>>>>>> refs/remotes/origin/master
};									\
struct ftrace_event_call __used						\
__attribute__((section("_ftrace_events"))) *__event_##call = &event_##call;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#undef FTRACE_ENTRY
#define FTRACE_ENTRY(call, struct_name, etype, tstruct, print, filter)	\
	FTRACE_ENTRY_REG(call, struct_name, etype,			\
			 PARAMS(tstruct), PARAMS(print), filter, NULL)

int ftrace_event_is_function(struct ftrace_event_call *call)
{
	return call == &event_function;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include "trace_entries.h"
