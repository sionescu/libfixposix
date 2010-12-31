dnl Process this file with autoconf to produce a configure script.
dnl
dnl This file is free software; as a special exception the author gives
dnl unlimited permission to copy and/or distribute it, with or without
dnl modifications, as long as this notice is preserved.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
dnl implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

dnl Minimum Autoconf version required.
AC_PREREQ(2.60)
AC_INIT([LibFixPOSIX], [0.1], [iolib-devel@common-lisp.net],
        [libfixposix], [http://common-lisp.net/project/iolib/])
AC_CONFIG_HEADERS([config.h])

dnl Must come before AM_INIT_AUTOMAKE.
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.10 foreign silent-rules subdir-objects
                  color-tests parallel-tests])

dnl Have Kbuild-like build output
AM_SILENT_RULES([yes])

dnl Where to generate output; srcdir location.
AC_CONFIG_SRCDIR([src/lib/])

dnl Auxiliary M4 macros
AC_CONFIG_MACRO_DIR([m4])

dnl Contains replacement functions
# AC_CONFIG_LIBOBJ_DIR([lib])

dnl Checks for programs.
# We need a C compiler.
AC_PROG_CC_C99
# And we need libtool for building the shared library
AC_PROG_LIBTOOL

LFP_FLAGS_INIT

dnl Set compiler & linker flags
LFP_SYS_PLATFORM_SPECIFIC_DEFINITIONS
AX_PTHREAD
LFP_SYS_LARGEFILE

LFP_CPPFLAGS+=" $LFS_CFLAGS"
LFP_CFLAGS+=" $PTHREAD_CFLAGS"
LFP_LDFLAGS+=" $PTHREAD_LIBS $LFS_LDFLAGS $LFS_LIBS"

dnl Checks for C99 & POSIX headers
AC_HEADER_ASSERT
LFP_REQUIRE_HEADERS([stdlib.h stdio.h string.h errno.h time.h])
LFP_REQUIRE_HEADERS([sys/types.h unistd.h fcntl.h signal.h])
AC_CHECK_HEADER([sys/signalfd.h])

dnl Checks for types
LFP_REQUIRE_TYPE([clockid_t], [time.h])
LFP_CHECK_SIZEOF_CLOCKID_T

dnl Checks for functions
AC_CHECK_FUNCS([accept4 pipe2 __xpg_strerror_r sendfile])
LFP_REQUIRE_FUNCS([pselect vsyslog])

dnl Checks for declarations
LFP_REQUIRE_DECLS([NSIG], [signal.h])

dnl Configuration switches
# LFP_ARG_ENABLE_EMULATED_SIGNALFD

AC_CONFIG_FILES([Makefile libfixposix.pc])
AC_OUTPUT
