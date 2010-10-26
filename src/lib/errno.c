#include <errno.h>

#include <libfixposix.h>

lfp_errno_t lfp_errno ()
{
  return errno;
}

lfp_errno_t lfp_set_errno (lfp_errno_t value)
{
  errno = value;
  return errno;
}
