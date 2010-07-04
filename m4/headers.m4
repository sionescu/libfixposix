# -*- Autoconf -*-
# headers.m4 - Better location of system headers.

AC_DEFUN([AC_CHECK_HEADER_STRICT],
[AC_CHECK_HEADER($1,,AC_MSG_FAILURE([Cannot find $1]))])
