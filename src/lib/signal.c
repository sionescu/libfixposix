#include <signal.h>

#include <libfixposix.h>
#include "utils.h"

lfp_sighandler_t lfp_sig_dfl (void)
{
  return SIG_DFL;
}

lfp_sighandler_t lfp_sig_err (void)
{
  return SIG_ERR;
}

lfp_sighandler_t lfp_sig_hold (void)
{
  return SIG_HOLD;
}

lfp_sighandler_t lfp_sig_ign (void)
{
  return SIG_IGN;
}

int lfp_sigrtmin (void)
{
#if defined(SIGRTMIN)
  return SIGRTMIN;
#else
  SYSERR(ENOSYS);
#endif // SIGRTMIN
}

int lfp_sigrtmax (void)
{
#if defined(SIGRTMAX)
  return SIGRTMAX;
#else
  SYSERR(ENOSYS);
#endif // SIGRTMAX
}
