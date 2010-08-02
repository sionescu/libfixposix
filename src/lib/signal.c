#include <libfixposix.h>

#include <signal.h>

sighandler_t lfp_sig_ign (void) {
  return SIG_IGN;
}

sighandler_t lfp_sig_dfl (void) {
  return SIG_DFL;
}

sighandler_t lfp_sig_err (void) {
  return SIG_ERR;
}

int lfp_sigrtmin (void) {
  return SIGRTMIN;
}

int lfp_sigrtmax (void) {
  return SIGRTMAX;
}

