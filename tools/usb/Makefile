# Makefile for USB tools

CC = $(CROSS_COMPILE)gcc
PTHREAD_LIBS = -lpthread
WARNINGS = -Wall -Wextra
<<<<<<< HEAD
<<<<<<< HEAD
CFLAGS = $(WARNINGS) -g $(PTHREAD_LIBS)
=======
CFLAGS = $(WARNINGS) -g $(PTHREAD_LIBS) -I../include
>>>>>>> refs/remotes/origin/cm-10.0

all: testusb ffs-test
%: %.c
	$(CC) $(CFLAGS) -o $@ $^
=======
CFLAGS = $(WARNINGS) -g -I../include
LDFLAGS = $(PTHREAD_LIBS)

all: testusb ffs-test
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
>>>>>>> refs/remotes/origin/master

clean:
	$(RM) testusb ffs-test
