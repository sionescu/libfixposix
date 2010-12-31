# -*- Autoconf -*-

AC_DEFUN([LFP_FLAGS_INIT], [
LFP_CPPFLAGS="-D_XOPEN_SOURCE"
LFP_CFLAGS=""
LFP_LDFLAGS=""
AC_SUBST(LFP_CPPFLAGS)
AC_SUBST(LFP_CFLAGS)
AC_SUBST(LFP_LDFLAGS)
])

AC_DEFUN([LFP_SYS_PLATFORM_SPECIFIC_DEFINITIONS], [
case "$host" in
     *-*-linux*)
       LFP_CPPFLAGS+=" -D_GNU_SOURCE=1"
       LFP_LDFLAGS+=" -lrt"
       ;;
     *-*-freebsd*)
       LFP_CPPFLAGS+=" -D__BSD_VISIBLE"
       ;;
     *-apple-darwin*)
       LFP_CPPFLAGS+=" -D_DARWIN_C_SOURCE"
       ;;
esac
])

AC_DEFUN([LFP_SYS_LARGEFILE], [
AC_CHECK_PROGS(GETCONF, [getconf])
if test x$GETCONF = x
then
  AC_MSG_FAILURE([Program getconf not found])
fi
LFS_CFLAGS=`(getconf LFS_CFLAGS) 2>/dev/null`
LFS_LDFLAGS=`(getconf LFS_LDFLAGS) 2>/dev/null`
LFS_LIBS=`(getconf LFS_LIBS) 2>/dev/null`
AC_SUBST(LFS_CFLAGS)
AC_SUBST(LFS_LDFLAGS)
AC_SUBST(LFS_LIBS)
])

AC_DEFUN([LFP_CHECK_SIZEOF_CLOCKID_T], [
AC_CHECK_SIZEOF([clockid_t], [], [[#include <time.h>]])
if test $ac_cv_sizeof_clockid_t -gt 4
then
  AC_MSG_FAILURE([clockid_t is larger than 32 bits!])
fi
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
