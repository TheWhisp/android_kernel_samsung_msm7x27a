# Makefile for vm selftests

CC = $(CROSS_COMPILE)gcc
<<<<<<< HEAD
CFLAGS = -Wall -Wextra

all: hugepage-mmap hugepage-shm  map_hugetlb
=======
CFLAGS = -Wall
BINARIES = hugepage-mmap hugepage-shm map_hugetlb thuge-gen hugetlbfstest

all: $(BINARIES)
>>>>>>> refs/remotes/origin/master
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

run_tests: all
<<<<<<< HEAD
	/bin/sh ./run_vmtests

clean:
	$(RM) hugepage-mmap hugepage-shm  map_hugetlb
=======
	@/bin/sh ./run_vmtests || (echo "vmtests: [FAIL]"; exit 1)

clean:
	$(RM) $(BINARIES)
>>>>>>> refs/remotes/origin/master
