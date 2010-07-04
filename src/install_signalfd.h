#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#if defined(__linux__)
# if HAVE_EMULATED_SIGNALFD
struct signalfd_siginfo { unsigned int ssi_signo; };
# else
#  include <sys/signalfd.h>
# endif
#endif

#if !defined(O_CLOEXEC)
# define O_CLOEXEC 0
#endif

int install_signalfd(int signum, int sa_flags);
void uninstall_signalfd(int signum, int block);
