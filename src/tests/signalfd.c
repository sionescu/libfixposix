#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

#include <lfp.h>

static
void error_abort (const char* msg, int perrorp) {
    if (perrorp) {
        perror(msg);
    } else {
        fprintf(stderr,"%s\n",msg);
    }
    abort();
}

static
ssize_t do_read(int fd, void *buf, size_t count) {
    ssize_t s;
    fd_set readfds;
    lfp_fd_zero(&readfds);

  do_try_read:
    lfp_fd_set(fd,&readfds);
    lfp_select(fd+1, &readfds, NULL, NULL, NULL, NULL);
    s = read(fd, buf, count);
    if (s != -1) {
        return s;
    }
    switch (errno) {
    case EAGAIN:
    case ERESTART:
    case EINTR:
        goto do_try_read;
    }
    return -1;
}

int main ()
{
    struct signalfd_siginfo fdsi;

    int sfd = lfp_install_signalfd(SIGINT, 0, NULL);

    size_t s = do_read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
    if (s != sizeof(struct signalfd_siginfo)) {
        error_abort("read", 1);
    }
    if (fdsi.ssi_signo == SIGINT) {
        printf("\nGot SIGINT\n");
    } else {
        error_abort("unexpected signal", 0);
    }

    lfp_uninstall_signalfd(SIGINT, 0);
    lfp_install_signalfd(SIGINT, 0, NULL);
    lfp_uninstall_signalfd(SIGINT, 0);

    return 0;
}
