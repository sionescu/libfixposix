# -*- Autoconf -*-

# Check for a header and print an error if not found
AC_DEFUN([AC_ENSURE_HEADER],
[AC_CHECK_HEADER($1,,AC_MSG_FAILURE([Cannot find header $1]))])

# Check for a function and print an error if not found
AC_DEFUN([AC_ENSURE_FUNCS],
[AC_CHECK_FUNCS($1,,AC_MSG_FAILURE([Cannot find function(s) $1]))])
