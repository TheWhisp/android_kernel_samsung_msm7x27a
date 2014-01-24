<<<<<<< HEAD
<<<<<<< HEAD
#!/bin/sh -x

=======
#!/bin/sh
>>>>>>> refs/remotes/origin/cm-10.0
=======
#!/bin/sh
>>>>>>> refs/remotes/origin/master

if [ -r Makefile ]; then
	make distclean
fi

<<<<<<< HEAD
<<<<<<< HEAD
FILES="configure cscope.out Makefile.in depcomp compile config.guess config.sub config.h.in~ config.log config.status ltmain.sh libtool config.h.in autom4te.cache missing aclocal.m4 install-sh cmd/Makefile.in lib/Makefile.in Makefile lib/Makefile cmd/Makefile"

rm -Rf $FILES
=======
=======
>>>>>>> refs/remotes/origin/master
FILES="aclocal.m4 autom4te.cache compile config.guess config.h.in config.log \
       config.status config.sub configure cscope.out depcomp install-sh      \
       libsrc/Makefile libsrc/Makefile.in libtool ltmain.sh Makefile         \
       Makefile.in missing src/Makefile src/Makefile.in"

rm -vRf $FILES
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
