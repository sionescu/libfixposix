#include <libfixposix.h>

#include <signal.h>

extern sighandler_t lfp_sig_ign (void) {
  return SIG_IGN;
}

extern sighandler_t lfp_sig_dfl (void) {
  return SIG_DFL;
}

extern sighandler_t lfp_sig_err (void) {
  return SIG_ERR;
}

extern int lfp_sigrtmin (void) {
  return SIGRTMIN;
}

extern int lfp_sigrtmax (void) {
  return SIGRTMAX;
}

