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

#include <config.h>

#include <lfp/fcntl.h>

#include <stdarg.h>

int lfp_open (const char *pathname, uint64_t flags, ...)
{
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode_t mode = va_arg(args, int);
        va_end(args);
        return open(pathname, (int)flags & 0xFFFFFFFF, mode);
    } else {
        return open(pathname, (int)flags & 0xFFFFFFFF);
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
