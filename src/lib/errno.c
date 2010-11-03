#include <errno.h>

#include <libfixposix.h>

int lfp_errno ()
{
  return errno;
}

int lfp_set_errno (int value)
{
  errno = value;
  return errno;
}
