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

INCLUDES = -I$(top_builddir)/src/include -I$(top_srcdir)/src/include



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

# Private headers, used only for internal definitions
noinst_HEADERS = src/lib/utils.h src/lib/spawn.h

include_HEADERS = src/include/libfixposix.h

include_libfixposixdir = $(includedir)/libfixposix
include_libfixposix_HEADERS = \
	src/include/libfixposix/aux.h \
	src/include/libfixposix/stdlib.h \
	src/include/libfixposix/string.h \
	src/include/libfixposix/errno.h \
	src/include/libfixposix/fcntl.h \
	src/include/libfixposix/unistd.h \
	src/include/libfixposix/mman.h \
	src/include/libfixposix/select.h \
	src/include/libfixposix/socket.h \
	src/include/libfixposix/wait.h \
	src/include/libfixposix/signal.h \
	src/include/libfixposix/spawn.h \
	src/include/libfixposix/dirent.h \
	src/include/libfixposix/time.h \
	src/include/libfixposix/resource.h \
	src/include/libfixposix/sendfile.h \
	src/include/libfixposix/syslog.h
# src/include/libfixposix/signalfd.h

src_lib_libfixposix_la_SOURCES = \
	src/lib/utils.c \
	src/lib/stdlib.c \
	src/lib/string.c \
	src/lib/errno.c \
	src/lib/fcntl.c \
	src/lib/unistd.c \
	src/lib/mman.c \
	src/lib/select.c \
	src/lib/socket.c \
	src/lib/wait.c \
	src/lib/signal.c \
	src/lib/spawnattr.c \
	src/lib/spawn_file_actions.c \
	src/lib/spawn.c \
	src/lib/dirent.c \
	src/lib/time.c \
	src/lib/resource.c \
	src/lib/sendfile.c \
	src/lib/syslog.c
# src/lib/install_signalfd.c

src_lib_libfixposix_la_LIBADD = $(LFP_LDFLAGS)



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
