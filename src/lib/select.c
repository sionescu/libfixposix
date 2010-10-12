#include <stdlib.h>
#include <stdbool.h>
#include <sys/select.h>

#include <libfixposix.h>

int lfp_select(int nfds, fd_set *readfds, fd_set *writefds,
               fd_set *exceptfds, const struct timespec *timeout,
               const sigset_t *sigmask)
{
    // FIXME: use select & sigsetmask if 1 HAVE_PSELECT
    return pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
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
