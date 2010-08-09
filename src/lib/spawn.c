#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <libfixposix.h>
#include "utils.h"

int lfp_apply_spawnattr(const lfp_spawnattr_t *attr);
int lfp_apply_spawn_file_actions(const lfp_spawn_file_actions_t *file_actions);

void child_exit(int pipe, int child_errno)
{
    int noctets = write(pipe, &child_errno, sizeof(int));
    if (noctets == sizeof(int))
        _exit(255);
    else
        _exit(254);
}

int lfp_spawn(pid_t *pid,
              const char *path,
              char *const argv[],
              char *const envp[],
              const lfp_spawn_file_actions_t *file_actions,
              const lfp_spawnattr_t *attr,
              bool search)
{
    SYSCHECK(LFP_EINVAL, pid == NULL);

    int ret, pipes[2];

    // Used for passing the error code from child to parent in case
    // some of the syscalls executed in the child fail
    if (lfp_pipe(pipes, O_CLOEXEC | O_NONBLOCK) < 0)
        return -1;

    pid_t child_pid = fork();

    if (child_pid < 0) {
        return -1;
    } else if (child_pid == 0) { // child
        close(pipes[0]);
        int child_errno = lfp_apply_spawnattr(attr);
        if (child_errno != 0)
            child_exit(pipes[1], child_errno);;
        execve(path, argv, envp);
        child_exit(pipes[1], lfp_errno());
    } else {                 // parent
        int child_errno, read_errno, status;
        close(pipes[1]);
        ret = read(pipes[0], &child_errno, sizeof(int));
        read_errno = lfp_errno();
        close(pipes[0]);
        switch (ret) {
        case -1:
            SYSERR(read_errno);
        case 0:
            *pid = child_pid;
            return 0;
        case 4:
            waitpid(child_pid, &status, WNOHANG);
            SYSERR(child_errno);
        default:
            // This is not suppose to happen because all 4 octets
            // of the child's errno should get here with one write
            SYSERR(EDOM);
        }
    }
    SYSERR(LFP_EBUG);
}
