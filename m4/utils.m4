# -*- Autoconf -*-

# Check for some headers and print an error if not found
AC_DEFUN([LFP_REQUIRE_HEADERS],
 [AC_CHECK_HEADERS($1,,AC_MSG_FAILURE([Cannot find header(s) $1]))])

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

# Check for a declaration and print an error if not found
# Syntax: LFP_GETCONF(VARIABLE, SYSTEM_VAR, ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
AC_DEFUN([LFP_GETCONF], [
$1=`(getconf $2) 2>/dev/null`
AC_SUBST($1)
if test $? -eq 0
then
  : ; $3
else
  : ; $4
fi
])
