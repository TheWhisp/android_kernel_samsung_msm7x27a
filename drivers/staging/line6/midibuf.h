/*
 * Line6 Linux USB driver - 0.9.1beta
 *
 * Copyright (C) 2004-2010 Markus Grabner (grabner@icg.tugraz.at)
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */

#ifndef MIDIBUF_H
#define MIDIBUF_H

<<<<<<< HEAD
struct MidiBuffer {
=======
struct midi_buffer {
>>>>>>> refs/remotes/origin/master
	unsigned char *buf;
	int size;
	int split;
	int pos_read, pos_write;
	int full;
	int command_prev;
};

<<<<<<< HEAD
extern int line6_midibuf_bytes_used(struct MidiBuffer *mb);
extern int line6_midibuf_bytes_free(struct MidiBuffer *mb);
extern void line6_midibuf_destroy(struct MidiBuffer *mb);
extern int line6_midibuf_ignore(struct MidiBuffer *mb, int length);
extern int line6_midibuf_init(struct MidiBuffer *mb, int size, int split);
extern int line6_midibuf_read(struct MidiBuffer *mb, unsigned char *data,
			      int length);
extern void line6_midibuf_reset(struct MidiBuffer *mb);
extern int line6_midibuf_skip_message(struct MidiBuffer *mb,
				      unsigned short mask);
extern void line6_midibuf_status(struct MidiBuffer *mb);
extern int line6_midibuf_write(struct MidiBuffer *mb, unsigned char *data,
=======
extern int line6_midibuf_bytes_used(struct midi_buffer *mb);
extern int line6_midibuf_bytes_free(struct midi_buffer *mb);
extern void line6_midibuf_destroy(struct midi_buffer *mb);
extern int line6_midibuf_ignore(struct midi_buffer *mb, int length);
extern int line6_midibuf_init(struct midi_buffer *mb, int size, int split);
extern int line6_midibuf_read(struct midi_buffer *mb, unsigned char *data,
			      int length);
extern void line6_midibuf_reset(struct midi_buffer *mb);
extern int line6_midibuf_skip_message(struct midi_buffer *mb,
				      unsigned short mask);
extern void line6_midibuf_status(struct midi_buffer *mb);
extern int line6_midibuf_write(struct midi_buffer *mb, unsigned char *data,
>>>>>>> refs/remotes/origin/master
			       int length);

#endif
