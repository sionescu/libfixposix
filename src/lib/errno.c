#include <errno.h>

#include <libfixposix.h>

extern
int lfp_errno ()
{
  return errno;
}

extern
int lfp_set_errno (int value)
{
  errno = value;
  return errno;
}
