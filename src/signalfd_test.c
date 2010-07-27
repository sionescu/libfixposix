#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

#include "install_signalfd.h"

static inline void error_abort (const char* msg, int perrorp) {
        if (perrorp) {
                perror(msg);
        } else {
                fprintf(stderr,"%s\n",msg);
        }
        abort();
}

static inline ssize_t do_read(int fd, void *buf, size_t count) {
        ssize_t s;
        fd_set readfds;
        FD_ZERO(&readfds);

do_try_read:
        FD_SET(fd,&readfds);
        select(fd+1, &readfds, NULL, NULL, NULL);
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

extern int main (int argc, char *argv[]) {
        int sfd = install_signalfd(SIGINT,0,NULL);
        struct signalfd_siginfo fdsi;
        ssize_t s = argc+**argv;

        s = do_read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo)) {
                error_abort("read",1);
        }
        if (fdsi.ssi_signo == SIGINT) {
                printf("\nGot SIGINT\n");
        } else {
                error_abort("unexpected signal",0);
        }
        uninstall_signalfd(SIGINT,0);
        install_signalfd(SIGINT,0,NULL);
        uninstall_signalfd(SIGINT,0);
        return 0;
}
