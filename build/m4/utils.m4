# -*- Autoconf -*-

AC_DEFUN([LFP_PROLOGUE], [
_lfp_save_flags() {
 _lfp_saved_cppflags=$CPPFLAGS
 _lfp_saved_cflags=$CFLAGS
 _lfp_saved_ldflags=$LDFLAGS
 _lfp_saved_libs=$LIBS
 CPPFLAGS="$LFP_CPPFLAGS"
 CFLAGS="$LFP_CFLAGS"
 LDFLAGS="$LFP_LDFLAGS"
 LIBS="$LFP_LIBS"
}

_lfp_restore_flags() {
 CPPFLAGS="$_lfp_saved_cppflags"
 CFLAGS="$_lfp_saved_cflags"
 LDFLAGS="$_lfp_saved_ldflags"
 LIBS="$_lfp_saved_libs"
 unset _lfp_saved_cppflags
 unset _lfp_saved_cflags
 unset _lfp_saved_ldflags
 unset _lfp_saved_libs
}
])

# Check for some headers and print an error if not found
AC_DEFUN([LFP_REQUIRE_HEADERS], [
 _lfp_save_flags
 AC_CHECK_HEADERS($1,,AC_MSG_FAILURE([Cannot find header(s) $1]))
 _lfp_restore_flags
])

# Check for a function and print an error if not found
AC_DEFUN([LFP_REQUIRE_FUNCS], [
 _lfp_save_flags
 AC_CHECK_FUNCS_ONCE($1,,AC_MSG_FAILURE([Cannot find function(s) $1]))
 _lfp_restore_flags
])

# Check for a type and print an error if not found
AC_DEFUN([LFP_REQUIRE_TYPE], [
 _lfp_save_flags
 AC_CHECK_TYPES($1,,AC_MSG_FAILURE([Cannot find type $1 in $2]),
                [m4_foreach_w([header], [$2], [[#]include [<]header[>]
 ])])
 _lfp_restore_flags
])

# Check for a declaration and print an error if not found
AC_DEFUN([LFP_REQUIRE_DECL], [
 _lfp_save_flags
 AC_CHECK_DECL($1,,AC_MSG_FAILURE([Cannot find $1 in <$2>]),
               [m4_foreach_w([header], [$2], [[#]include [<]header[>]
 ])])
 _lfp_restore_flags
])

AC_DEFUN([LFP_REQUIRE_PROG], [
 AC_CHECK_PROG($1, $2, [yes])
 if test "x$ac_cv_prog_$1" != xyes
 then
   AC_MSG_FAILURE([Program $2 not found])
 fi
])

# Check for a declaration and print an error if not found
# Syntax: LFP_GETCONF(VARIABLE, SYSTEM_VAR,
#                     ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
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
# Syntax: LFP_GETCONF(VARIABLE, SYSTEM_VAR,
#                     ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
AC_DEFUN([LFP_GETCONF_UNDEF], [
 $1=`(getconf $2) 2>/dev/null`
 AC_SUBST($1)
 if test $? -eq 0 && test x"$$1" != xundefined ; then
   : ; $3
 else
   : ; $4
 fi
])
