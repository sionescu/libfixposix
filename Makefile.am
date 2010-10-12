ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -std=c99 -Wall -Wstrict-prototypes
INCLUDES = $(LFP_CPPFLAGS) $(LFP_CFLAGS) -I$(top_builddir)/src/lib



pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libfixposix.pc



########################################################################
#
#                             libfixposix.so
#
########################################################################

lib_LTLIBRARIES = src/lib/libfixposix.la

src_lib_libfixposix_la_SOURCES = \
	src/lib/stdlib.c src/lib/string.c src/lib/fcntl.c \
	src/lib/unistd.c src/lib/mman.c \
	src/lib/install_signalfd.c \
	src/lib/select.c src/lib/socket.c \
	src/lib/errno.c src/lib/wait.c \
	src/lib/signal.c \
	src/lib/spawnattr.c src/lib/spawn_file_actions.c src/lib/spawn.c \
	src/lib/dirent.c src/lib/resource.c

src_lib_libfixposix_la_LIBADD = $(LFP_LIBS)

include_HEADERS = \
	src/lib/libfixposix.h



########################################################################
#
#                             signalfd_test
#
########################################################################

check_PROGRAMS = src/tests/signalfd src/tests/spawn \
	src/tests/select

src_tests_signalfd_SOURCES = src/tests/signalfd.c

src_tests_signalfd_LDADD = src/lib/libfixposix.la

src_tests_spawn_SOURCES = src/tests/spawn.c

src_tests_spawn_LDADD = src/lib/libfixposix.la

src_tests_select_SOURCES = src/tests/select.c

src_tests_select_LDADD = src/lib/libfixposix.la
