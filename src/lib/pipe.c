#include <unistd.h>
#include <fcntl.h>

#include <libfixposix.h>

extern
int lfp_pipe (int pipefd[2], int flags)
{
    if (HAVE_PIPE2) {
        return pipe2(pipefd, flags);
    }
    else {
        int fd = pipe(pipefd);
        if (fd < 0) { goto error_return; }
        if (flags & O_CLOEXEC) {
            int ret = fcntl(pipefd[0], F_SETFD, FD_CLOEXEC);
            if (ret < 0) { goto error_close; }
        }
        if (flags & O_NONBLOCK) {
            int ret = fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
            if (ret < 0) { goto error_close; }
        }
        return fd;
      error_close:
        close(fd);
      error_return:
        return -1;
    }
}
