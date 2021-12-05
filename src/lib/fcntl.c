/*******************************************************************************/
/* Permission is hereby granted, free of charge, to any person or organization */
/* obtaining a copy of the software and accompanying documentation covered by  */
/* this license (the "Software") to use, reproduce, display, distribute,       */
/* execute, and transmit the Software, and to prepare derivative works of the  */
/* Software, and to permit third-parties to whom the Software is furnished to  */
/* do so, all subject to the following:                                        */
/*                                                                             */
/* The copyright notices in the Software and this entire statement, including  */
/* the above license grant, this restriction and the following disclaimer,     */
/* must be included in all copies of the Software, in whole or in part, and    */
/* all derivative works of the Software, unless such copies or derivative      */
/* works are solely in the form of machine-executable object code generated by */
/* a source language processor.                                                */
/*                                                                             */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    */
/* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT   */
/* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE   */
/* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, */
/* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER */
/* DEALINGS IN THE SOFTWARE.                                                   */
/*******************************************************************************/

#include <lfp/fcntl.h>
#include <lfp/stdlib.h>
#include <lfp/unistd.h>

#include <stdbool.h>
#include <stdarg.h>

DSO_PUBLIC int
lfp_open (const char *pathname, uint64_t flags, ...)
{
    mode_t mode = 0;
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode = va_arg(args, mode_t);
        va_end(args);
    }

    int fd = lfp_open_k(pathname, flags, mode);
    if (fd < 0) {
        errno = -fd;
        return -1;
    }
    return fd;
}

DSO_PUBLIC int
lfp_open_k (const char *pathname, uint64_t flags, mode_t mode)
{
    int fd = 0;
    if (flags & O_CREAT) {
        fd = open(pathname, (int)flags, mode);
    } else {
        fd = open(pathname, (int)flags);
    }

    if (fd < 0) { return -errno; }
    return fd;
}

DSO_PUBLIC int
lfp_openpt (uint64_t flags)
{
    int fd = lfp_openpt_k(flags);
    if (fd < 0) {
        errno = -fd;
        return -1;
    }
    return fd;
}

DSO_PUBLIC int
lfp_openpt_k (uint64_t flags)
{
    bool cloexec = flags & O_CLOEXEC;
    flags &= ~O_CLOEXEC;

    int fd = posix_openpt((int)flags);
    if (fd < 0) { return -errno; }

    if (cloexec) {
        int err = lfp_set_fd_cloexec(fd, true);
        if (err < 0) {
            close(fd);
            return err;
        }
    }
    return fd;
}

DSO_PUBLIC int
lfp_creat (const char *pathname, mode_t mode)
{
    return creat(pathname, mode);
}

DSO_PUBLIC int
lfp_create_k (const char *pathname, mode_t mode)
{
    int fd = creat(pathname, mode);
    if (fd < 0) { return -errno; }
    return fd;
}


DSO_PUBLIC int
lfp_is_fd_cloexec (int fd)
{
    int yesno = lfp_is_fd_cloexec_k(fd);
    if (yesno < 0) {
        errno = -yesno;
        return -1;
    }
    return yesno;
}

DSO_PUBLIC int
lfp_is_fd_cloexec_k (int fd)
{
    int current_flags = fcntl(fd, F_GETFD);
    if (current_flags < 0) {
        return -errno;
    } else {
        return (current_flags & FD_CLOEXEC) ? true : false;
    }
}

DSO_PUBLIC int
lfp_set_fd_cloexec (int fd, bool enabled)
{
    int ret = lfp_set_fd_cloexec_k(fd, enabled);
    if (ret < 0) {
        errno = -ret;
        return -1;
    }
    return 0;
}

DSO_PUBLIC int
lfp_set_fd_cloexec_k (int fd, bool enabled)
{
    int current_flags = fcntl(fd, F_GETFD);
    if (current_flags < 0) { return -errno; }
    int new_flags = enabled ? current_flags | FD_CLOEXEC \
                            : current_flags & ~FD_CLOEXEC;
    if (new_flags != current_flags) {
        if(fcntl(fd, F_SETFD, new_flags) < 0) {
            return -errno;
        }
    }
    return 0;
}

DSO_PUBLIC int
lfp_is_fd_nonblock (int fd)
{
    int yesno = lfp_is_fd_nonblock_k(fd);
    if (yesno < 0) {
        errno = -yesno;
        return -1;
    }
    return yesno;
}

DSO_PUBLIC int
lfp_is_fd_nonblock_k (int fd)
{
    int current_flags = fcntl(fd, F_GETFL);
    if (current_flags < 0) {
        return -errno;
    } else {
        return (current_flags & O_NONBLOCK) ? true : false;
    }
}

DSO_PUBLIC int
lfp_set_fd_nonblock (int fd, bool enabled)
{
    int ret = lfp_set_fd_nonblock_k(fd, enabled);
    if (ret < 0) {
        errno = -ret;
        return -1;
    }
    return 0;
}

DSO_PUBLIC int
lfp_set_fd_nonblock_k (int fd, bool enabled)
{
    int current_flags = fcntl(fd, F_GETFL);
    if (current_flags < 0) { return -errno; }
    int new_flags = enabled ? current_flags | O_NONBLOCK \
                            : current_flags & ~O_NONBLOCK;
    if (new_flags != current_flags) {
        if(fcntl(fd, F_SETFL, new_flags) < 0) {
            return -errno;
        }
    }
    return 0;
}
