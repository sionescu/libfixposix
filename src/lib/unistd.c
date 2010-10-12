#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#include <libfixposix.h>

off_t lfp_lseek(int fd, off_t offset, int whence)
{
    return lseek(fd, offset, whence);
}



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
            (lfp_set_fd_cloexec(pipefd[0], true) < 0 ||
             lfp_set_fd_cloexec(pipefd[1], true) < 0)) {
            goto error_close;
        }
        if ((flags & O_NONBLOCK) &&
            (lfp_set_fd_nonblock(pipefd[0], true) < 0 ||
             lfp_set_fd_nonblock(pipefd[1], true) < 0)) {
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



ssize_t lfp_pread(int fd, void *buf, size_t count, off_t offset)
{
    return pread(fd, buf, count, offset);
}

ssize_t lfp_pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    return pwrite(fd, buf, count, offset);
}



int lfp_truncate(const char *path, off_t length)
{
    return truncate(path, length);
}

int lfp_ftruncate(int fd, off_t length)
{
    return ftruncate(fd, length);
}



int lfp_stat(const char *path, struct stat *buf)
{
    return stat(path, buf);
}

int lfp_fstat(int fd, struct stat *buf)
{
    return fstat(fd, buf);
}

int lfp_lstat(const char *path, struct stat *buf)
{
    return lstat(path, buf);
}

int lfp_fd_is_open(int fd)
{
    struct stat buf;
    int ret = fstat(fd, &buf);
    if ( ret < 0 ) {
        if ( lfp_errno() == LFP_EBADF ) {
            return false;
        } else {
            return -1;
        }
    } else {
        return true;
    }
}

bool lfp_isreg(mode_t mode)
{
    return (bool) S_ISREG(mode);
}

bool lfp_isdir(mode_t mode)
{
    return (bool) S_ISDIR(mode);
}

bool lfp_ischr(mode_t mode)
{
    return (bool) S_ISCHR(mode);
}

bool lfp_isblk(mode_t mode)
{
    return (bool) S_ISBLK(mode);
}

bool lfp_isfifo(mode_t mode)
{
    return (bool) S_ISFIFO(mode);
}

bool lfp_islnk(mode_t mode)
{
    return (bool) S_ISLNK(mode);
}

bool lfp_issock(mode_t mode)
{
    return (bool) S_ISSOCK(mode);
}
