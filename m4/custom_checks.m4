# -*- Autoconf -*-

# Check for a function and print an error if not found
AC_DEFUN([LFP_CHECK_SIZEOF_CLOCKID_T],
 [AC_CHECK_SIZEOF([clockid_t], [], [[#include <time.h>]])
if test $ac_cv_sizeof_clockid_t -gt 4
then
  AC_MSG_FAILURE([clockid_t is larger than 32 bits!])
fi])

AC_DEFUN([LFP_SYS_LARGEFILE],
 [AC_CHECK_PROGS(GETCONF, [getconf])
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
