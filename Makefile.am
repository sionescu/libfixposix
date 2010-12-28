# -*- makefile-automake -*-

########################################################################
#                                                                      #
#                        Global Automake variables                     #
#                                                                      #
########################################################################

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = $(LFP_CPPFLAGS)

AM_CFLAGS = -Wall -Wextra -Wmissing-prototypes \
	    -Wmissing-declarations -Wmissing-field-initializers \
	    $(LFP_CFLAGS)

INCLUDES = -I$(top_builddir)/src/include



########################################################################
#                                                                      #
#                              PKG-CONFIG                              #
#                                                                      #
########################################################################

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libfixposix.pc



########################################################################
#                                                                      #
#                             libfixposix.so                           #
#                                                                      #
########################################################################

lib_LTLIBRARIES = src/lib/libfixposix.la

src_lib_libfixposix_la_SOURCES = \
	src/lib/utils.c \
	src/lib/stdlib.c src/lib/string.c src/lib/fcntl.c \
	src/lib/unistd.c src/lib/mman.c \
	src/lib/select.c src/lib/socket.c \
	src/lib/errno.c src/lib/wait.c \
	src/lib/signal.c \
	src/lib/spawnattr.c src/lib/spawn_file_actions.c src/lib/spawn.c \
	src/lib/dirent.c src/lib/time.c src/lib/resource.c \
	src/lib/sendfile.c src/lib/syslog.c
# src/lib/install_signalfd.c

noinst_HEADERS = src/lib/utils.h src/lib/spawn.h

src_lib_libfixposix_la_LIBADD = $(LFP_LDFLAGS)

include_HEADERS = \
	src/include/libfixposix.h



########################################################################
#                                                                      #
#                                TESTS                                 #
#                                                             	       #
########################################################################

check_PROGRAMS	= src/tests/spawn src/tests/select

check_SCRIPTS	=

TESTS		= ${check_PROGRAMS} ${check_SCRIPTS}
XFAIL_TESTS	=


# src_tests_signalfd_SOURCES = src/tests/signalfd.c

# src_tests_signalfd_LDADD = src/lib/libfixposix.la

src_tests_spawn_SOURCES = src/tests/spawn.c

src_tests_spawn_LDADD = src/lib/libfixposix.la

src_tests_select_SOURCES = src/tests/select.c

src_tests_select_LDADD = src/lib/libfixposix.la
