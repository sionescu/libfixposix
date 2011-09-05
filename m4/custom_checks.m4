# -*- Autoconf -*-

AC_DEFUN([LFP_FLAGS_INIT], [
 LFP_CFLAGS="-D_XOPEN_SOURCE=600"
 LFP_LDFLAGS=""
 LFP_LIBS=""
 AC_SUBST(LFP_CFLAGS)
 AC_SUBST(LFP_LDFLAGS)
 AC_SUBST(LFP_LIBS)
])

AC_DEFUN([LFP_INIT], [
 LFP_PROLOGUE
 LFP_FLAGS_INIT
])

AC_DEFUN([LFP_SYS_PLATFORM_SPECIFIC_DEFINITIONS], [
 case "$host" in
  *-*-linux*)
    LFP_CFLAGS="$LFP_CFLAGS -D_GNU_SOURCE=1"
    ;;
  *-apple-darwin*)
    LFP_CFLAGS="$LFP_CFLAGS -D_DARWIN_C_SOURCE"
    ;;
  *-sunos* | *-solaris*)
    LFP_CFLAGS="$LFP_CFLAGS -D__EXTENSIONS__"
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
 AC_SEARCH_LIBS([clock_gettime], [rt], [
                 AC_SUBST([HAVE_CLOCK_GETTIME], [1])
                 LFP_CHECK_POSIX_REALTIME_CLOCK_TYPES
                 AC_DEFINE([HAVE_CLOCK_GETTIME], [1],
                           [We have the function clock_gettime()])
                 LFP_LIBS="$LFP_LIBS $ac_cv_search_clock_gettime"
                 LIBS=""],
                [AC_SUBST([HAVE_CLOCK_GETTIME], [0])
                 AC_SUBST([HAVE_CLOCKID_T], [0])
                 LFP_CHECK_POSIX_REALTIME_CLOCK_ALTERNATIVES])
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
