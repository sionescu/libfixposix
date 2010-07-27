#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lfp_unistd.h"
#include "install_signalfd.h"

static struct signalfd_params {
    int read_fd;
    int write_fd;
} *(signalfd_params[NSIG]);

static inline
int lfp_signalfd (int fd, const sigset_t *mask, int flags)
{
    if (HAVE_EMULATED_SIGNALFD) {
        return -1;
    } else {
        return signalfd(fd, mask, flags);
    }
}

static
void signalfd_emulator (int signum)
{
    struct signalfd_siginfo ssi;
    struct signalfd_params *params = signalfd_params[signum];
    int ret;

    assert (params != NULL);
    assert (params->write_fd != -1);
    memset(&ssi, 0, sizeof(ssi));
    ssi.ssi_signo = signum;
  notify_listener:
    ret = write(params->write_fd, &ssi, sizeof(ssi));
    if (ret == sizeof(ssi)) {
        return;
    }
    if (ret == -1) {
        switch (errno) {
        case EAGAIN:
            /* The pipe buffer is full, therefore there's already
               a notification in the pipe pending reception.
               We can drop the redundant signal notification.
               Redundant signals are NEVER guaranteed not to be
               dropped anyway, and signals may have been dropped
               on our way already if the system was busy. */
            return;
        case EINTR:
            /* Some other signal trumped us; try again! */
            goto notify_listener;
        }
    }
    /* Something went deeply wrong. Abort. */
    abort();
}

static
void error_abort (const char* msg, int perrorp)
{
    if (perrorp) {
        perror(msg);
    } else {
        fprintf(stderr, "%s\n", msg);
    }
    abort();
}

static
void warning_signal_handler (int signum)
{
    char msg[128];
    int i = snprintf(msg, sizeof(msg),
                     "\nCaught signalfd-monitored signal %d, which should have been blocked.\n", signum);
    write(2, msg, i);
}

extern
int install_signalfd(int signum, int sa_flags, int* blockp)
{
    int pipefd[2];
    int ret;
    int block;
    sigset_t sigmask;
    int emulate_signalfd;
    struct signalfd_params *params;
    struct sigaction sa;

    if ( (signum < 0) || (signum >= NSIG) ) {
        error_abort("install_signalfd called with bad signal number.",0);
    }

    /* Setup sigaction */
    memset(&sa,0,sizeof(sa));
    sa.sa_flags = (sa_flags & (SA_NOCLDSTOP | SA_NOCLDWAIT)) | SA_ONSTACK;

    /* Create mask with one signal */
    sigemptyset(&sigmask);
    sigaddset(&sigmask, signum);

    /* Allocate parameters */
    if (signalfd_params[signum]) {
        error_abort("install_signalfd already installed.",0);
    }
    params = malloc(sizeof(signalfd_params));
    if (params == NULL) {
        error_abort("install_signalfd malloc failed.",0);
    }

    /* Before we touch the handler, block the signal */
    ret = sigprocmask(SIG_BLOCK, &sigmask, NULL);
    if (ret != 0) { error_abort("install_signalfd signal blocking failed.",0); }

    /* First, try signalfd */
    ret = lfp_signalfd(-1, &sigmask, SFD_CLOEXEC | SFD_NONBLOCK);
    if (ret != -1) { /* success with signalfd, instead default handler! */
        emulate_signalfd = 0;
        sa.sa_handler = &warning_signal_handler; /* was SIG_DFL, but we want to catch bugs */
        params->read_fd = ret;
        params->write_fd = -1;
        block = 1;
        goto signalfd_sigaction;
    }

    /* no success with signalfd (probably EINVAL), emulate! */
    emulate_signalfd = 1;
    sa.sa_handler = &signalfd_emulator;
    ret = lfp_pipe2(pipefd, O_CLOEXEC | O_NONBLOCK);
    if (ret != 0) { error_abort("install_signalfd pipe2 failed: ",1); }
    params->read_fd = pipefd[0];
    params->write_fd = pipefd[1];
    block = 0;

  signalfd_sigaction:
    signalfd_params[signum] = params;
    ret = sigaction(signum,&sa,NULL);
    if (ret != 0) { error_abort("install_signalfd failed sigaction: ",1); }
    if (emulate_signalfd) {
        ret = sigprocmask(SIG_UNBLOCK, &sigmask, NULL);
        if (ret != 0) { error_abort("install_signalfd signal unblocking failed.",0); }
    }
    if (blockp) { *blockp = block; }
    return params->read_fd;
}

extern
void uninstall_signalfd(int signum, int block)
{
    int ret;
    sigset_t sigmask;
    struct signalfd_params *params;
    struct sigaction sa;

    if ( (signum < 0) || (signum >= NSIG) ) {
        error_abort("install_signalfd called with bad signal number.",0);
    }

    /* Setup sigaction */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_DFL;

    /* Create mask with one signal */
    sigemptyset(&sigmask);
    sigaddset(&sigmask, signum);

    /* Before we touch the handler, block the signal */
    ret = sigprocmask(SIG_BLOCK, &sigmask, NULL);
    if (ret != 0) { error_abort("uninstall_signalfd signal blocking failed.",0); }

    /* Release parameters */
    params = signalfd_params[signum];
    if ( params == NULL) {
        error_abort("uninstall_signalfd not installed.",0);
    }
    close(params->read_fd);
    if (params->write_fd != -1) {
        close(params->write_fd);
    }
    free(params);
    signalfd_params[signum] = NULL;

    ret = sigaction(signum, &sa, NULL);
    if (ret != 0) { error_abort("uninstall_signalfd failed sigaction: ",1); }
    if (!block) {
        ret = sigprocmask(SIG_UNBLOCK, &sigmask, NULL);
        if (ret != 0) { error_abort("uninstall_signalfd signal unblocking failed.",0); }
    }
}
