#if !defined(_LFP_INSTALL_SIGNALFD_H_)
#define _LFP_INSTALL_SIGNALFD_H_

#include <signal.h>

#if defined(__linux__)
# if HAVE_EMULATED_SIGNALFD
struct signalfd_siginfo { unsigned int ssi_signo; };
# else
#  include <sys/signalfd.h>
# endif
#endif

int install_signalfd(int signum, int sa_flags, int* blockp);
void uninstall_signalfd(int signum, int block);

#endif // _LFP_INSTALL_SIGNALFD_H_
