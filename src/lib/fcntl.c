#include <fcntl.h>

#include <libfixposix.h>

extern
int lfp_set_fd_cloexec(int fd)
{
    return fcntl(fd, F_SETFD, FD_CLOEXEC);
}

extern
int lfp_set_fd_nonblock(int fd)
{
    return fcntl(fd, F_SETFL, O_NONBLOCK);
}
