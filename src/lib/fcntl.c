#include <fcntl.h>

#include <libfixposix.h>

int lfp_set_fd_cloexec(int fd)
{
    int current_flags = fcntl(fd, F_GETFD);
    if (current_flags < 0) {
        return -1;
    } else {
        return fcntl(fd, F_SETFD, current_flags | FD_CLOEXEC);
    }
}

int lfp_set_fd_nonblock(int fd)
{
    int current_flags = fcntl(fd, F_GETFL);
    if (current_flags < 0) {
        return -1;
    } else {
        return fcntl(fd, F_SETFL, current_flags | O_NONBLOCK);
    }
}
