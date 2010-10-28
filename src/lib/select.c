#include <stdlib.h>
#include <stdbool.h>
#include <sys/select.h>
#include <signal.h>

#include <libfixposix.h>
#include "utils.h"

int lfp_select(int nfds, fd_set *readfds, fd_set *writefds,
               fd_set *exceptfds, const struct timespec *timeout,
               const sigset_t *sigmask)
{
#if defined(HAVE_PSELECT)
    return pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
#else
    sigset_t oldmask;
    struct timeval tv;
    _lfp_timespec_to_timeval(timeout, &tv);
    pthread_sigmask(SIG_SETMASK, sigmask, &oldmask);
    int ret = select(nfds, readfds, writefds, exceptfds, timeout);
    pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
    return ret;
#endif
}

void lfp_fd_clr(int fd, fd_set *set)
{
    FD_CLR(fd, set);
}

bool lfp_fd_isset(int fd, fd_set *set)
{
    return (bool) FD_ISSET(fd, set);
}

void lfp_fd_set(int fd, fd_set *set)
{
    FD_SET(fd, set);
}

void lfp_fd_zero(fd_set *set)
{
    FD_ZERO(set);
}
