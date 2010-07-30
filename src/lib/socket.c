#include <stdbool.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <libfixposix.h>

#if !defined(SOCK_CLOEXEC)
# define SOCK_CLOEXEC 0
#endif
#if !defined(SOCK_NONBLOCK)
# define SOCK_NONBLOCK 0
#endif

extern
int lfp_socket(lfp_socket_domain_t domain,
               lfp_socket_type_t   type,
               int                 protocol,
               lfp_flags_t         flags)
{
    int _flags = 0;

    if (SOCK_CLOEXEC && (flags & O_CLOEXEC)) {
        _flags |= SOCK_CLOEXEC;
    }
    if (SOCK_NONBLOCK && (flags & O_NONBLOCK)) {
        _flags |= SOCK_NONBLOCK;
    }

    int fd = socket(domain, type | _flags, protocol);
    if (fd < 0) { goto error_return; };

    if (SOCK_CLOEXEC && (flags & O_CLOEXEC) &&
        lfp_set_fd_cloexec(fd) < 0) {
        goto error_close;
    }
    if (SOCK_NONBLOCK && (flags & O_NONBLOCK) &&
        lfp_set_fd_nonblock(fd) < 0) {
        goto error_close;
    }
    return fd;

  error_close:
    close(fd);
  error_return:
    return -1;
}

extern
int lfp_accept(int             sockfd,
               struct sockaddr *addr,
               socklen_t       *addrlen,
               lfp_flags_t     flags)
{
    if (HAVE_ACCEPT4) {
        int _flags = 0;

        if (flags & O_CLOEXEC) {
            _flags |= SOCK_CLOEXEC;
        }
        if (flags & O_NONBLOCK) {
            _flags |= SOCK_NONBLOCK;
        }

        return accept4(sockfd, addr, addrlen, _flags);
    } else {
        int fd = accept(sockfd, addr, addrlen);
        if (fd < 0) { goto error_return; }

        if ((flags & O_CLOEXEC) && lfp_set_fd_cloexec(fd) < 0) {
            goto error_close;
        }
        if ((flags & O_NONBLOCK) && lfp_set_fd_nonblock(fd) < 0) {
            goto error_close;
        }

        return fd;

      error_close:
        close(fd);
      error_return:
        return -1;
    }
}
