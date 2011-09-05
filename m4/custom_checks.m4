# -*- Autoconf -*-

AC_DEFUN([LFP_FLAGS_INIT], [
 LFP_CFLAGS="-D_XOPEN_SOURCE=600"
 LFP_LDFLAGS="-Wl,--no-undefined"
 LFP_LIBS=""
 AC_SUBST(LFP_CFLAGS)
 AC_SUBST(LFP_LDFLAGS)
 AC_SUBST(LFP_LIBS)
])

AC_DEFUN([LFP_SYS_PLATFORM_SPECIFIC_DEFINITIONS], [
 case "$host" in
      *-*-linux*)
        LFP_CFLAGS+=" -D_GNU_SOURCE=1"
        ;;
      *-apple-darwin*)
        LFP_CFLAGS+=" -D_DARWIN_C_SOURCE"
        ;;
      *-sunos* | *-solaris*)
        LFP_CFLAGS+=" -D__EXTENSIONS__"
        ;;
 esac
])

AC_DEFUN([LFP_SYS_LARGEFILE], [
 LFP_GETCONF_UNDEF([LFS_CFLAGS],  [LFS_CFLAGS])
 LFP_GETCONF_UNDEF([LFS_LDFLAGS], [LFS_LDFLAGS])
 LFP_GETCONF_UNDEF([LFS_LIBS],    [LFS_LIBS])
])

AC_DEFUN([LFP_CHECK_CLOCKID_T], [
AC_CHECK_TYPE([clockid_t],
 [AC_CHECK_SIZEOF([clockid_t], [], [[#include <sys/time.h>]
                                    [#include <time.h>]])
  if test "$ac_cv_sizeof_clockid_t" -gt 4 ; then
    AC_MSG_FAILURE([Type clockid_t is larger than 32 bits])
  fi],
  AC_MSG_FAILURE([Type clockid_t not found])
 [[#include <sys/time.h>]
  [#include <time.h>]])
])

AC_DEFUN([LFP_CHECK_POSIX_REALTIME_CLOCK_TYPES], [
 LFP_CHECK_CLOCKID_T
 LFP_REQUIRE_DECL([CLOCK_REALTIME], [sys/time.h time.h])
 LFP_REQUIRE_DECL([CLOCK_MONOTONIC], [sys/time.h time.h])
])

AC_DEFUN([LFP_CHECK_POSIX_REALTIME_CLOCK_ALTERNATIVES], [
 case "$host" in
  *-apple-darwin*)
    # We could check for Mach functions here
    ;;
  *)
    AC_MSG_ERROR([clock_gettime not found and no workaround available])
    ;;
 esac
])

AC_DEFUN([LFP_REQUIRE_MONOTONIC_CLOCK], [
 AC_SEARCH_LIBS([clock_gettime], [rt],
                [LFP_CHECK_POSIX_REALTIME_CLOCK_TYPES
                 LFP_LIBS+=" $ac_cv_search_clock_gettime"
                 LIBS=""],
                [LFP_CHECK_POSIX_REALTIME_CLOCK_ALTERNATIVES])
])

AC_DEFUN([LFP_ARG_ENABLE_EMULATED_SIGNALFD], [
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
])
