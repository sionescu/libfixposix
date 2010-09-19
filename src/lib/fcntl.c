#include <fcntl.h>
#include <stdarg.h>

#include <libfixposix.h>

int lfp_open (const char *pathname, lfp_open_flags_t flags, ...)
{
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode_t mode = va_arg(args, mode_t);
        va_end(args);
        return open(pathname, flags & 0xFFFFFFFF, mode);
    } else {
        return open(pathname, flags & 0xFFFFFFFF);
    }
}

int lfp_creat (const char *pathname, mode_t mode)
{
    return creat(pathname, mode);
}



int lfp_set_fd_cloexec (int fd)
{
    int current_flags = fcntl(fd, F_GETFD);
    if (current_flags < 0) {
        return -1;
    } else {
        return fcntl(fd, F_SETFD, current_flags | FD_CLOEXEC);
    }
}

int lfp_set_fd_nonblock (int fd)
{
    int current_flags = fcntl(fd, F_GETFL);
    if (current_flags < 0) {
        return -1;
    } else {
        return fcntl(fd, F_SETFL, current_flags | O_NONBLOCK);
    }
}
