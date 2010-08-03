#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#include <libfixposix.h>

int lfp_pipe (int pipefd[2], lfp_open_flags_t flags)
{
    if (HAVE_PIPE2) {
        // We assume that if pipe2() is defined then O_CLOEXEC too
        // exists, which means that it's in the lower part of "flags"
        return pipe2(pipefd, flags & 0xFFFFFFFF);
    } else {
        if (pipe(pipefd) < 0) {
            goto error_return;
        }

        if ((flags & O_CLOEXEC) &&
            (lfp_set_fd_cloexec(pipefd[0]) < 0 ||
             lfp_set_fd_cloexec(pipefd[1]) < 0)) {
            goto error_close;
        }
        if ((flags & O_NONBLOCK) &&
            (lfp_set_fd_nonblock(pipefd[0]) < 0 ||
             lfp_set_fd_nonblock(pipefd[1]) < 0)) {
            goto error_close;
        }

        return 0;

      error_close:
        close(pipefd[0]);
        close(pipefd[1]);
      error_return:
        return -1;
    }
}
