# -*- Autoconf -*-

# Check for a header and print an error if not found
AC_DEFUN([LFP_REQUIRE_HEADER],
 [AC_CHECK_HEADER($1,,AC_MSG_FAILURE([Cannot find header $1]))])

# Check for a function and print an error if not found
AC_DEFUN([LFP_REQUIRE_FUNCS],
 [AC_CHECK_FUNCS($1,,AC_MSG_FAILURE([Cannot find function(s) $1]))])

# Check for a type and print an error if not found
AC_DEFUN([LFP_REQUIRE_TYPE],
 [AC_CHECK_TYPE($1,,AC_MSG_FAILURE([Cannot find type $1 in $2]),
                [[#include <$2>]])])

# Check for a declaration and print an error if not found
AC_DEFUN([LFP_REQUIRE_DECLS],
 [AC_CHECK_DECLS($1,,AC_MSG_FAILURE([Cannot find $1 in $2]),
                [[#include <$2>]])])
