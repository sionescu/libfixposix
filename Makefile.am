ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -std=c99 -Wall -Wstrict-prototypes
INCLUDES = $(LFP_DEP_CFLAGS)

lib_LTLIBRARIES = src/libfixposix.la

src_libfixposix_la_SOURCES = \
	src/install_signalfd.c

src_libfixposix_la_LIBADD = $(LFP_DEP_LIBS)

bin_PROGRAMS = src/signalfd_test

src_signalfd_test_SOURCES= src/signalfd_test.c

src_signalfd_test_LDADD = src/libfixposix.la
