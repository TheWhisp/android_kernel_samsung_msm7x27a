# Makefile for vm tools
<<<<<<< HEAD

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra

all: page-types slabinfo
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) page-types slabinfo
=======
#
TARGETS=page-types slabinfo

LIB_DIR = ../lib/api
LIBS = $(LIB_DIR)/libapikfs.a

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -I../lib/
LDFLAGS = $(LIBS)

$(TARGETS): $(LIBS)

$(LIBS):
	make -C $(LIB_DIR)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	$(RM) page-types slabinfo
	make -C $(LIB_DIR) clean
>>>>>>> refs/remotes/origin/master
