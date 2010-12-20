# -*- Autoconf -*-

# Check for a function and print an error if not found
AC_DEFUN([AC_LFP_ENSURE_SUITABLE_CLOCKID_T],
 [AC_MSG_CHECKING([for suitable clockid_t])
  AC_TRY_RUN([
#include <assert.h>
#include <time.h>
int main()
{
#if defined(_POSIX_TIMERS) && !defined(_POSIX_MONOTONIC_CLOCK)
    assert(sizeof(clockid_t) <= 4);
#else
    return 0;
#endif
}
  ],
  AC_MSG_RESULT([yes]),
  AC_MSG_FAILURE([clockid_t is larger than 32 bits!]))])
