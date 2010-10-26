#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <libfixposix.h>
#include "utils.h"
#include "spawn.h"

static
void child_exit(int pipe, int child_errno)
{
    int noctets = write(pipe, &child_errno, sizeof(int));
    if (noctets == sizeof(int))
        _exit(255);
    else
        _exit(254);
}

static
void handle_child(int (*execfun)(const char*, char *const[], char *const[]),
                  const char *path,
                  char *const argv[],
                  char *const envp[],
                  const lfp_spawn_file_actions_t *file_actions,
                  const lfp_spawnattr_t *attr,
                  int pipes[2])
{
    close(pipes[0]);
    int child_errno = lfp_spawnattr_apply(attr);
    if (child_errno != 0) {
        child_exit(pipes[1], child_errno);
    }
    child_errno = lfp_spawn_apply_file_actions(file_actions);
    if (child_errno != 0) {
        child_exit(pipes[1], child_errno);
    }
    execfun(path, argv, envp);
    child_exit(pipes[1], lfp_errno());
}

static
int handle_parent(pid_t *pid, pid_t child_pid, int pipes[2])
{
    close(pipes[1]);
    int status;
    lfp_errno_t child_errno;
    int noctets = read(pipes[0], &child_errno, sizeof(int));
    lfp_errno_t read_errno = lfp_errno();
    close(pipes[0]);
    switch (noctets) {
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
        SYSERR(LFP_EBUG);
    }
}

static
int _lfp_spawn(int (*execfun)(const char*, char *const[], char *const[]),
               pid_t *pid,
               const char *path,
               char *const argv[],
               char *const envp[],
               const lfp_spawn_file_actions_t *file_actions,
               const lfp_spawnattr_t *attr)
{
    int pipes[2];

    // Used for passing the error code from child to parent in case
    // some of the syscalls executed in the child fail
    if (lfp_pipe(pipes, O_CLOEXEC) < 0)
        return -1;

    pid_t child_pid = fork();

    switch (child_pid) {
    case -1:
        return -1;
    case 0:
        handle_child(execfun, path, argv, envp, file_actions, attr, pipes);
        // Flow reaches this point only if child_exit() mysteriously fails
        SYSERR(LFP_EBUG);
    default:
        return handle_parent(pid, child_pid, pipes);
    }
}

int lfp_spawn(pid_t *pid,
              const char *path,
              char *const argv[],
              char *const envp[],
              const lfp_spawn_file_actions_t *file_actions,
              const lfp_spawnattr_t *attr)
{
    SYSCHECK(LFP_EINVAL, pid == NULL);

    return _lfp_spawn(&lfp_execve, pid, path, argv, envp, file_actions, attr);
}

int lfp_spawnp(pid_t *pid,
               const char *file,
               char *const argv[],
               char *const envp[],
               const lfp_spawn_file_actions_t *file_actions,
               const lfp_spawnattr_t *attr)
{
    SYSCHECK(LFP_EINVAL, pid == NULL);

    return _lfp_spawn(&lfp_execvpe, pid, file, argv, envp, file_actions, attr);
}
