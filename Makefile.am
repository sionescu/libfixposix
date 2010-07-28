ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -std=c99 -Wall -Wstrict-prototypes
INCLUDES = $(LFP_DEP_CFLAGS) -I$(top_builddir)/src/lib



########################################################################
#
#                             libfixposix.so
#
########################################################################

lib_LTLIBRARIES = src/lib/libfixposix.la

src_lib_libfixposix_la_SOURCES = \
	src/lib/pipe.c src/lib/install_signalfd.c

src_lib_libfixposix_la_LIBADD = $(LFP_DEP_LIBS)

include_HEADERS = \
	src/lib/libfixposix.h



########################################################################
#
#                             signalfd_test
#
########################################################################

check_PROGRAMS = src/tests/signalfd_test

src_tests_signalfd_test_SOURCES = src/tests/signalfd_test.c

src_tests_signalfd_test_LDADD = src/lib/libfixposix.la
