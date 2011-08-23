# -*- Autoconf -*-

# Check for some headers and print an error if not found
AC_DEFUN([LFP_REQUIRE_HEADERS],
 [AC_CHECK_HEADERS($1,,AC_MSG_FAILURE([Cannot find header(s) $1]))])

# Check for a function and print an error if not found
AC_DEFUN([LFP_REQUIRE_FUNCS],
 [AC_CHECK_FUNCS_ONCE($1,,AC_MSG_FAILURE([Cannot find function(s) $1]))])

# Check for a type and print an error if not found
AC_DEFUN([LFP_REQUIRE_TYPE],
 [AC_CHECK_TYPE($1,,AC_MSG_FAILURE([Cannot find type $1 in $2]),
                 [m4_foreach_w([header], [$2], [[#]include [<]header[>]
])])])

dnl # Check for a declaration and print an error if not found
dnl AC_DEFUN([LFP_REQUIRE_DECLS],
dnl  [AC_CHECK_DECLS($1,,AC_MSG_FAILURE([Cannot find $1 in $2]),
dnl                 [[#include <$2>]])])

# Check for a declaration and print an error if not found
AC_DEFUN([LFP_REQUIRE_DECLS],
 [AC_CHECK_DECLS($1,,AC_MSG_FAILURE([Cannot find $1 in <$2>]),
                 [m4_foreach_w([header], [$2], [[#]include [<]header[>]
])])])

# Check for a declaration and print an error if not found
# Syntax: LFP_GETCONF(VARIABLE, SYSTEM_VAR, ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
AC_DEFUN([LFP_GETCONF], [
$1=`(getconf $2) 2>/dev/null`
AC_SUBST($1)
if test $? -eq 0 ; then
  : ; $3
else
  : ; $4
fi
])

# Check for a declaration and print an error if not found or is undefined
# Syntax: LFP_GETCONF(VARIABLE, SYSTEM_VAR, ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
AC_DEFUN([LFP_GETCONF_UNDEF], [
$1=`(getconf $2) 2>/dev/null`
AC_SUBST($1)
if test $? -eq 0 && test x"$$1" != xundefined ; then
  : ; $3
else
  : ; $4
fi
])
