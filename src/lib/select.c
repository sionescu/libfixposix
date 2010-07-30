#include <stdlib.h>
#include <stdbool.h>
#include <sys/select.h>

#include <libfixposix.h>

extern
int lfp_select(int nfds, fd_set *readfds, fd_set *writefds,
               fd_set *exceptfds, const struct timespec *timeout,
               const sigset_t *sigmask)
{
    return pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

extern
void lfp_fd_clr(int fd, fd_set *set)
{
    FD_CLR(fd, set);
}

extern
bool lfp_fd_isset(int fd, fd_set *set)
{
    return (bool) FD_ISSET(fd, set);
}

extern
void lfp_fd_set(int fd, fd_set *set)
{
    FD_SET(fd, set);
}

extern
void lfp_fd_zero(fd_set *set)
{
    FD_ZERO(set);
}
