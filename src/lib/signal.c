#include <libfixposix.h>

#include <signal.h>

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
  return SIGRTMIN;
}

int lfp_sigrtmax (void)
{
  return SIGRTMAX;
}
