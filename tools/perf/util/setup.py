#!/usr/bin/python2

from distutils.core import setup, Extension
from os import getenv

<<<<<<< HEAD
<<<<<<< HEAD
cflags = ['-fno-strict-aliasing', '-Wno-write-strings']
cflags += getenv('CFLAGS', '').split()

perf = Extension('perf',
		  sources = ['util/python.c', 'util/ctype.c', 'util/evlist.c',
			     'util/evsel.c', 'util/cpumap.c', 'util/thread_map.c',
			     'util/util.c', 'util/xyarray.c', 'util/cgroup.c'],
=======
=======
>>>>>>> refs/remotes/origin/master
from distutils.command.build_ext   import build_ext   as _build_ext
from distutils.command.install_lib import install_lib as _install_lib

class build_ext(_build_ext):
    def finalize_options(self):
        _build_ext.finalize_options(self)
        self.build_lib  = build_lib
        self.build_temp = build_tmp

class install_lib(_install_lib):
    def finalize_options(self):
        _install_lib.finalize_options(self)
        self.build_dir = build_lib


<<<<<<< HEAD
cflags = ['-fno-strict-aliasing', '-Wno-write-strings']
cflags += getenv('CFLAGS', '').split()

build_lib = getenv('PYTHON_EXTBUILD_LIB')
build_tmp = getenv('PYTHON_EXTBUILD_TMP')
=======
cflags = getenv('CFLAGS', '').split()
# switch off several checks (need to be at the end of cflags list)
cflags += ['-fno-strict-aliasing', '-Wno-write-strings', '-Wno-unused-parameter' ]

build_lib = getenv('PYTHON_EXTBUILD_LIB')
build_tmp = getenv('PYTHON_EXTBUILD_TMP')
libtraceevent = getenv('LIBTRACEEVENT')
libapikfs = getenv('LIBAPIKFS')
>>>>>>> refs/remotes/origin/master

ext_sources = [f.strip() for f in file('util/python-ext-sources')
				if len(f.strip()) > 0 and f[0] != '#']

perf = Extension('perf',
		  sources = ext_sources,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		  include_dirs = ['util/include'],
		  extra_compile_args = cflags,
=======
		  include_dirs = ['util/include'],
		  extra_compile_args = cflags,
		  extra_objects = [libtraceevent, libapikfs],
>>>>>>> refs/remotes/origin/master
                 )

setup(name='perf',
      version='0.1',
      description='Interface with the Linux profiling infrastructure',
      author='Arnaldo Carvalho de Melo',
      author_email='acme@redhat.com',
      license='GPLv2',
      url='http://perf.wiki.kernel.org',
<<<<<<< HEAD
<<<<<<< HEAD
      ext_modules=[perf])
=======
      ext_modules=[perf],
      cmdclass={'build_ext': build_ext, 'install_lib': install_lib})
>>>>>>> refs/remotes/origin/cm-10.0
=======
      ext_modules=[perf],
      cmdclass={'build_ext': build_ext, 'install_lib': install_lib})
>>>>>>> refs/remotes/origin/master
