# -*- Autoconf -*-

AC_DEFUN([LFP_SAVE_FLAGS], [
 _lfp_saved_cppflags_$1=$CPPFLAGS
 _lfp_saved_cflags_$1=$CFLAGS
 _lfp_saved_ldflags_$1=$LDFLAGS
 _lfp_saved_libs_$1=$LIBS
 CPPFLAGS="$LFP_CPPFLAGS"
 CFLAGS="$LFP_CFLAGS"
 LDFLAGS="$LFP_LDFLAGS"
 LIBS="$LFP_LIBS"
])

AC_DEFUN([LFP_RESTORE_FLAGS], [
 CPPFLAGS="$_lfp_saved_cppflags_$1"
 CFLAGS="$_lfp_saved_cflags_$1"
 LDFLAGS="$_lfp_saved_ldflags_$1"
 LIBS="$_lfp_saved_libs_$1"
])

# Check for some headers and print an error if not found
AC_DEFUN([LFP_REQUIRE_HEADERS], [
 LFP_SAVE_FLAGS(AS_TR_CPP($1))
 AC_CHECK_HEADERS($1,,AC_MSG_FAILURE([Cannot find header(s) $1]))
 LFP_RESTORE_FLAGS(AS_TR_CPP($1))
])

# Check for a function and print an error if not found
AC_DEFUN([LFP_REQUIRE_FUNCS], [
 LFP_SAVE_FLAGS(AS_TR_CPP($1))
 AC_CHECK_FUNCS($1,,AC_MSG_FAILURE([Cannot find function(s) $1]))
 LFP_RESTORE_FLAGS(AS_TR_CPP($1))
])

# Check for a type and print an error if not found
AC_DEFUN([LFP_REQUIRE_TYPE], [
 LFP_SAVE_FLAGS(AS_TR_CPP($1))
 AC_CHECK_TYPES($1,,AC_MSG_FAILURE([Cannot find type $1 in $2]),
  [m4_foreach_w([header], [$2], [[#]include [<]header[>]
 ])])
 LFP_RESTORE_FLAGS(AS_TR_CPP($1))
])

# Check for a declaration and print an error if not found
AC_DEFUN([LFP_REQUIRE_DECL], [
 LFP_SAVE_FLAGS(AS_TR_CPP($1))
 AC_CHECK_DECL($1,,AC_MSG_FAILURE([Cannot find $1 in <$2>]),
  [m4_foreach_w([header], [$2], [[#]include [<]header[>]
 ])])
 LFP_RESTORE_FLAGS(AS_TR_CPP($1))
])

# Check for a library and print an error if not found
AC_DEFUN([LFP_SEARCH_LIBS], [
 LFP_SAVE_FLAGS(AS_TR_CPP($1))
 AC_SEARCH_LIBS($1,$2,$3
  [if test -n "$LIBS"; then LFP_LIBS="$LFP_LIBS $LIBS"; fi],
  m4_default_nblank_quoted([$4],AC_MSG_FAILURE([Cannot find $1 in <$2>])))
 LFP_RESTORE_FLAGS(AS_TR_CPP($1))
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
