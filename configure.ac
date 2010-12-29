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

dnl Set compiler & linker flags
case "$host" in
     *-*-linux*)
       LFP_CPPFLAGS="-D_GNU_SOURCE=1"
       LFP_LDFLAGS="-lrt"
       ;;
     *-*-freebsd*)
       LFP_CPPFLAGS="-D__BSD_VISIBLE"
       ;;
     *-apple-darwin*)
       LFP_CPPFLAGS="-D_DARWIN_C_SOURCE"
       ;;
esac
AX_PTHREAD
LFP_SYS_LARGEFILE

LFP_CPPFLAGS+=" -D_XOPEN_SOURCE=700 $LFS_CFLAGS"
LFP_CFLAGS+=" $PTHREAD_CFLAGS"
LFP_LDFLAGS+=" $PTHREAD_LIBS $LFS_LDFLAGS $LFS_LIBS"

dnl Checks for C99 & POSIX headers
AC_HEADER_ASSERT
LFP_REQUIRE_HEADER([stdlib.h])
LFP_REQUIRE_HEADER([stdio.h])
LFP_REQUIRE_HEADER([string.h])
LFP_REQUIRE_HEADER([errno.h])
LFP_REQUIRE_HEADER([time.h])
LFP_REQUIRE_HEADER([sys/types.h])
LFP_REQUIRE_HEADER([unistd.h])
LFP_REQUIRE_HEADER([fcntl.h])
LFP_REQUIRE_HEADER([signal.h])
AC_CHECK_HEADER([sys/signalfd.h])

dnl Checks for types
LFP_REQUIRE_TYPE([clockid_t], [time.h])
LFP_CHECK_SIZEOF_CLOCKID_T

dnl Checks for functions
AC_CHECK_FUNCS([accept4])
AC_CHECK_FUNCS([pipe2])
AC_CHECK_FUNCS([__xpg_strerror_r])
AC_CHECK_FUNCS([sendfile])
LFP_REQUIRE_FUNCS([pselect])
LFP_REQUIRE_FUNCS([vsyslog])

dnl Checks for declarations
LFP_REQUIRE_DECLS([NSIG], [signal.h])

AC_ARG_ENABLE([emulated-signalfd],
  [AS_HELP_STRING([--enable-emulated-signalfd={yes|no|check}],
    [Use emulated signalfd @<:@default=check@:>@])],
  [],
  [enable_emulated_signalfd=check])

AS_CASE($enable_emulated_signalfd,
  [yes], [AC_DEFINE(HAVE_EMULATED_SIGNALFD, 1, [Whether to use signalfd emulation])
          AC_SUBST(HAVE_EMULATED_SIGNALFD, 1)
         ],
  [check], [if test x"$ac_cv_header_sys_signalfd_h" = xyes ; then
              AC_DEFINE(HAVE_EMULATED_SIGNALFD, 0, [Whether to use signalfd emulation])
              AC_SUBST(HAVE_EMULATED_SIGNALFD, 0)
            else
              AC_DEFINE(HAVE_EMULATED_SIGNALFD, 1, [Whether to use signalfd emulation])
              AC_SUBST(HAVE_EMULATED_SIGNALFD, 1)
            fi],
  [no], [if test x"$ac_cv_header_sys_signalfd_h" = xyes ; then
           AC_DEFINE(HAVE_EMULATED_SIGNALFD, 0, [Whether to use signalfd emulation])
           AC_SUBST(HAVE_EMULATED_SIGNALFD, 0)
         else
           AC_MSG_FAILURE([signalfd emulation was disabled although <sys/signalfd.h> is absent])
         fi],
  [AC_MSG_FAILURE(
    [--enable-emulated-signalfd was given an illegal value: "$enable_emulated_signalfd"])])

AC_SUBST(LFP_CPPFLAGS)
AC_SUBST(LFP_CFLAGS)
AC_SUBST(LFP_LDFLAGS)

AC_CONFIG_FILES([Makefile libfixposix.pc])
AC_OUTPUT
