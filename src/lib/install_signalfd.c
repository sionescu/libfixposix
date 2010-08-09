#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <libfixposix.h>
#include "utils.h"

static struct signalfd_params {
    int read_fd;
    int write_fd;
} *(signalfd_params[NSIG]);

static inline
int lfp_signalfd (int fd, const sigset_t *mask, int flags)
{
    if (HAVE_EMULATED_SIGNALFD)
        SYSERR(LFP_ENOSYS);
    else
        return signalfd(fd, mask, flags);
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
        switch (lfp_errno()) {
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
void warning_signal_handler (int signum)
{
    char msg[128];
    int i = snprintf(msg, sizeof(msg),
                     "\nCaught signalfd-monitored signal %d, which should have been blocked.\n", signum);
    write(STDERR_FILENO, msg, i);
}

int lfp_install_signalfd(int signum, int sa_flags, bool* blockp)
{
    int pipefd[2];
    bool block;
    sigset_t sigmask;
    int emulate_signalfd;
    struct signalfd_params *params;
    struct sigaction sa;

    SYSCHECK(EINVAL, signum < 0 || signum >= NSIG);

    /* Setup sigaction */
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = (sa_flags & (SA_NOCLDSTOP | SA_NOCLDWAIT)) | SA_ONSTACK;

    /* Create mask with one signal */
    sigemptyset(&sigmask);
    sigaddset(&sigmask, signum);

    /* Allocate parameters */
    SYSCHECK(EALREADY, signalfd_params[signum]);
    params = malloc(sizeof(signalfd_params));
    SYSCHECK(ENOMEM, params == NULL);

    /* Before we touch the handler, block the signal */
    if (sigprocmask(SIG_BLOCK, &sigmask, NULL) < 0)
        return -1;

    /* First, try signalfd */
    int sigfd = lfp_signalfd(-1, &sigmask, SFD_CLOEXEC | SFD_NONBLOCK);
    if (sigfd != -1) {
        /* success with signalfd, block signal and install warning handler */
        emulate_signalfd = 0;
        sa.sa_handler = &warning_signal_handler; /* SIG_DFL would work but we want to catch bugs */
        params->read_fd = sigfd;
        params->write_fd = -1;
        block = true;
        goto signalfd_sigaction;
    }

    /* no success with signalfd (probably ENOSYS), emulate! */
    emulate_signalfd = 1;
    sa.sa_handler = &signalfd_emulator;
    if (lfp_pipe(pipefd, O_NONBLOCK | O_CLOEXEC) < 0)
        return -1;
    params->read_fd = pipefd[0];
    params->write_fd = pipefd[1];
    block = false;

  signalfd_sigaction:
    signalfd_params[signum] = params;
    if (sigaction(signum,&sa,NULL) < 0) 
        return -1;
    if (emulate_signalfd && sigprocmask(SIG_UNBLOCK, &sigmask, NULL) < 0)
        return -1;
    if (blockp) { *blockp = block; }
    return params->read_fd;
}

int lfp_uninstall_signalfd(int signum, bool block)
{
    sigset_t sigmask;
    struct signalfd_params *params;
    struct sigaction sa;

    SYSCHECK(EINVAL, signum < 0 || signum >= NSIG);

    /* Setup sigaction */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_DFL;

    /* Create mask with one signal */
    sigemptyset(&sigmask);
    sigaddset(&sigmask, signum);

    /* Before we touch the handler, block the signal */
    if (sigprocmask(SIG_BLOCK, &sigmask, NULL) != 0)
        return -1;

    /* Release parameters */
    params = signalfd_params[signum];
    if ( params == NULL)
        return 0;
    close(params->read_fd);
    if (params->write_fd != -1)
        close(params->write_fd);
    free(params);
    signalfd_params[signum] = NULL;

    if (sigaction(signum, &sa, NULL) < 0)
        return -1;
    if (!block && sigprocmask(SIG_UNBLOCK, &sigmask, NULL) != 0)
        return -1;
    return 0;
}
