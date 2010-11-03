#include <fcntl.h>
#include <stdarg.h>

#include <libfixposix.h>

int lfp_open (const char *pathname, uint64_t flags, ...)
{
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode_t mode = va_arg(args, int);
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



int lfp_is_fd_cloexec (int fd)
{
    int current_flags = fcntl(fd, F_GETFD);
    if (current_flags < 0) {
        return -1;
    } else {
        return (current_flags & FD_CLOEXEC) ? true : false;
    }
}

int lfp_set_fd_cloexec (int fd, bool enabled)
{
    int current_flags = fcntl(fd, F_GETFD);
    if (current_flags < 0) {
        return -1;
    } else {
        int new_flags = enabled ? current_flags | FD_CLOEXEC \
                                : current_flags & ~FD_CLOEXEC;
        if ( new_flags != current_flags ) {
            return fcntl(fd, F_SETFD, new_flags);
        } else {
            return 0;
        }
    }
}

int lfp_is_fd_nonblock (int fd)
{
    int current_flags = fcntl(fd, F_GETFL);
    if (current_flags < 0) {
        return -1;
    } else {
        return (current_flags & O_NONBLOCK) ? true : false;
    }
}

int lfp_set_fd_nonblock (int fd, bool enabled)
{
    int current_flags = fcntl(fd, F_GETFL);
    if (current_flags < 0) {
        return -1;
    } else {
        int new_flags = enabled ? current_flags | O_NONBLOCK \
                                : current_flags & ~O_NONBLOCK;
        if ( new_flags != current_flags ) {
            return fcntl(fd, F_SETFL, new_flags);
        } else {
            return 0;
        }
    }
}
