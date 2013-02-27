# -*- Autoconf -*-

AC_DEFUN([LFP_TESTS_CHECK], [
AC_ARG_ENABLE([tests],
  [AS_HELP_STRING([--enable-tests],
    [enable test suite using CHECK @<:@default=no@:>@])],
  [],
  [enable_tests=no])
AS_CASE($enable_tests,
  [yes], [PKG_CHECK_MODULES([CHECK], [check >= 0.9.4])
          AC_SUBST([CHECK_CFLAGS])
          AC_SUBST([CHECK_LIBS])],
  [no], [],
  AC_MSG_ERROR(
    [--enable-tests was given an illegal value: "$enable_tests"])
)
AM_CONDITIONAL(TESTS, test x$enable_tests = xyes)
])

AC_DEFUN([LFP_INIT], [
 LFP_CFLAGS=""
 LFP_LDFLAGS=""
 LFP_LIBS=""
 AC_SUBST(LFP_CFLAGS)
 AC_SUBST(LFP_LDFLAGS)
 AC_SUBST(LFP_LIBS)
])

AC_DEFUN([LFP_SYS_PLATFORM_SPECIFIC_DEFINITIONS], [
 case "$host_os" in
  linux*)
    LFP_CFLAGS="$LFP_CFLAGS -D_XOPEN_SOURCE=600 -D_GNU_SOURCE=1"
    ;;
  freebsd*)
    LFP_CFLAGS="$LFP_CFLAGS -D_XOPEN_SOURCE=600"
    ;;
  openbsd*)
    LFP_CFLAGS="$LFP_CFLAGS -D_XOPEN_SOURCE=600 -D__BSD_VISIBLE"
    ;;
  netbsd*)
    LFP_CFLAGS="$LFP_CFLAGS -D_XOPEN_SOURCE=600 -D_NETBSD_SOURCE"
    ;;
  darwin*)
    LFP_CFLAGS="$LFP_CFLAGS -D_DARWIN_C_SOURCE"
    ;;
  sunos* | solaris*)
    LFP_CFLAGS="$LFP_CFLAGS -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -D_POSIX_PTHREAD_SEMANTICS"
    ;;
 esac
])

AC_DEFUN([LFP_SYS_LARGEFILE], [
 LFP_GETCONF_UNDEF([LFS_CFLAGS],  [LFS_CFLAGS])
 LFP_GETCONF_UNDEF([LFS_LDFLAGS], [LFS_LDFLAGS])
 LFP_GETCONF_UNDEF([LFS_LIBS],    [LFS_LIBS])
])

AC_DEFUN([LFP_CHECK_POSIX_REALTIME_CLOCK_TYPES], [
 LFP_REQUIRE_TYPE([clockid_t], [sys/time.h time.h])
 AC_SUBST([HAVE_CLOCKID_T], [1])
 LFP_REQUIRE_DECL([CLOCK_REALTIME], [sys/time.h time.h])
 LFP_REQUIRE_DECL([CLOCK_MONOTONIC], [sys/time.h time.h])
])

AC_DEFUN([LFP_CHECK_POSIX_REALTIME_CLOCK_ALTERNATIVES], [
 case "$host_os" in
  darwin*)
    LFP_REQUIRE_HEADERS([mach/mach.h mach/clock.h])
    LFP_REQUIRE_FUNCS([host_get_clock_service clock_get_attributes])
    dnl LFP_REQUIRE_DECL([CLOCK_GET_TIME_RES], [mach/mach.h mach/clock.h])
    dnl LFP_REQUIRE_DECL([CALENDAR_CLOCK], [mach/mach.h mach/clock.h])
    dnl LFP_REQUIRE_DECL([REALTIME_CLOCK], [mach/mach.h mach/clock.h])
    ;;
  *)
    AC_MSG_ERROR([clock_gettime not found and no workaround available])
    ;;
 esac
])

AC_DEFUN([LFP_REQUIRE_MONOTONIC_CLOCK], [
 LFP_SEARCH_LIBS([clock_gettime], [rt], [
  AC_SUBST([HAVE_CLOCK_GETTIME], [1])
  LFP_CHECK_POSIX_REALTIME_CLOCK_TYPES
  AC_DEFINE([HAVE_CLOCK_GETTIME], [1],
            [We have the function clock_gettime()])],
 [AC_SUBST([HAVE_CLOCK_GETTIME], [0])
  AC_SUBST([HAVE_CLOCKID_T], [0])
  LFP_CHECK_POSIX_REALTIME_CLOCK_ALTERNATIVES])
])

AC_DEFUN([LFP_REQUIRE_GETPEEREID], [
 AC_CHECK_FUNCS([getpeereid],
  [AC_DEFINE([HAVE_GETPEEREID], [1], [We have the function getpeereid()])],
  AC_CHECK_FUNCS([getpeerucred],
   [LFP_REQUIRE_HEADERS([ucred.h])
    AC_DEFINE([HAVE_GETPEERUCRED], [1], [We have the function getpeerucred()])],
   [LFP_REQUIRE_DECL([SO_PEERCRED], [sys/types.h sys/socket.h])]))
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
