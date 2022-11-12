# -*- makefile-automake -*-

########################################################################
#                                                                      #
#                        Global Automake variables                     #
#                                                                      #
########################################################################

ACLOCAL_AMFLAGS = -I config/m4

AM_CFLAGS = $(LFP_CFLAGS) -fvisibility=hidden

AM_CPPFLAGS = -include $(top_builddir)/config.h \
	      -include $(top_srcdir)/src/lib/aux/macros.h \
	      -I$(top_srcdir)/src/include \
	      -I$(top_builddir)/src/include

EXTRA_DIST = \
	LICENCE \
	README.md \
	VERSION \
	distro/debian \
	distro/libfixposix.spec \
	distro/libfixposix.rpmlintrc \
	distro/PKGBUILD \
	src/tests/lib \
	$(wildcard $(top_srcdir)/src/tests/*.in)



########################################################################
#                                                                      #
#                              PKG-CONFIG                              #
#                                                                      #
########################################################################

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = src/libfixposix.pc



########################################################################
#                                                                      #
#                             libfixposix.so                           #
#                                                                      #
########################################################################

lib_LTLIBRARIES = src/lib/libfixposix.la

# Private headers, used only for internal definitions
# The only purpose of this definition it to ensure that
#  «make dist» knows about them
noinst_HEADERS = \
	src/lib/aux/inlines.h \
	src/lib/aux/macros.h \
	src/lib/bitset.h \
	src/lib/spawn.h

include_HEADERS = src/include/lfp.h

include_lfpdir = $(includedir)/lfp
include_lfp_HEADERS = \
	src/include/lfp/aux.h \
	src/include/lfp/stdlib.h \
	src/include/lfp/string.h \
	src/include/lfp/strerror.h \
	src/include/lfp/errno.h \
	src/include/lfp/fcntl.h \
	src/include/lfp/stat.h \
	src/include/lfp/unistd.h \
	src/include/lfp/ioctl.h \
	src/include/lfp/mman.h \
	src/include/lfp/select.h \
	src/include/lfp/socket.h \
	src/include/lfp/wait.h \
	src/include/lfp/signal.h \
	src/include/lfp/spawn.h \
	src/include/lfp/dirent.h \
	$(top_builddir)/src/include/lfp/time.h \
	src/include/lfp/resource.h \
	src/include/lfp/sendfile.h \
	src/include/lfp/syslog.h \
	src/include/lfp/buildinfo.h
# src/include/lfp/signalfd.h

src_lib_libfixposix_la_SOURCES = \
	src/libfixposix.libtool \
	src/lib/stdlib.c \
	src/lib/string.c \
	src/lib/strerror.c \
	src/lib/errno.c \
	src/lib/fcntl.c \
	src/lib/stat.c \
	src/lib/unistd.c \
	src/lib/ioctl.c \
	src/lib/mman.c \
	src/lib/select.c \
	src/lib/socket.c \
	src/lib/wait.c \
	src/lib/signal.c \
	src/lib/spawnattr.c \
	src/lib/bitset.c \
	src/lib/spawn_file_actions.c \
	src/lib/spawn.c \
	src/lib/dirent.c \
	src/lib/time.c \
	src/lib/resource.c \
	src/lib/sendfile.c \
	src/lib/syslog.c \
	src/lib/buildinfo.c
# src/lib/install_signalfd.c

src_lib_libfixposix_la_DEPENDENCIES = \
	src/libfixposix.libtool

src_lib_libfixposix_la_LIBADD = $(LFP_LIBS)

src_lib_libfixposix_la_LDFLAGS = $(LFP_LDFLAGS) -no-undefined \
	-version-info `@AWK@ '/^Version:/ { print $$2 }' $(top_srcdir)/src/libfixposix.libtool`



########################################################################
#                                                                      #
#                                TESTS                                 #
#                                                             	       #
########################################################################

if TESTS

LOG_DRIVER = env AM_TAP_AWK='$(AWK)' \
	TOP_SRCDIR=$(top_srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	$(SHELL) $(top_srcdir)/config/aux/tap-driver.sh

compile_tests = \
	src/tests/buildinfo

run_tests = \
	src/tests/spawn \
	src/tests/select \
	src/tests/mkstemp

check_PROGRAMS = $(compile_tests) $(run_tests)

check_SCRIPTS	= \
	src/tests/buildinfo.bats

# Bats tests must be created by /.configure to get the correct path of
# the Bats interpreter, but the configure script fails to preserve the
# execute bit, so we restore it here.
%.bats: %.bats.in $(top_builddir)/Makefile
	sed 's:\@BATSPATH\@:$(BATSPATH):' < $< > $@
	chmod +x $@

TESTS		= ${run_tests} ${check_SCRIPTS}
XFAIL_TESTS	=

src_tests_buildinfo_SOURCES = src/tests/buildinfo.c
src_tests_buildinfo_CFLAGS  = @CHECK_CFLAGS@ $(LFP_CFLAGS)
src_tests_buildinfo_LDADD   = @CHECK_LIBS@ $(top_builddir)/src/lib/libfixposix.la

src_tests_spawn_SOURCES = src/tests/spawn.c
src_tests_spawn_CFLAGS  = @CHECK_CFLAGS@ $(LFP_CFLAGS)
src_tests_spawn_LDADD   = @CHECK_LIBS@ $(top_builddir)/src/lib/libfixposix.la

src_tests_select_SOURCES = src/tests/select.c
src_tests_select_CFLAGS  = @CHECK_CFLAGS@ $(LFP_CFLAGS)
src_tests_select_LDADD   = @CHECK_LIBS@ $(top_builddir)/src/lib/libfixposix.la

src_tests_mkstemp_SOURCES = src/tests/mkstemp.c
src_tests_mkstemp_CFLAGS  = @CHECK_CFLAGS@ $(LFP_CFLAGS)
src_tests_mkstemp_LDADD   = @CHECK_LIBS@ $(top_builddir)/src/lib/libfixposix.la

endif
