# -*- Autoconf -*-

# Check for a function and print an error if not found
AC_DEFUN([LFP_CHECK_SIZEOF_CLOCKID_T],
 [AC_CHECK_SIZEOF([clockid_t], [], [[#include <time.h>]])
if test $ac_cv_sizeof_clockid_t -gt 4
then
  AC_MSG_FAILURE([clockid_t is larger than 32 bits!])
fi])
