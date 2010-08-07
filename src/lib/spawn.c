#include <unistd.h>

#include <libfixposix.h>

int lfp_spawn(pid_t *pid,
              const char *path,
              char *const argv[],
              char *const envp[],
              const lfp_spawn_file_actions_t *file_actions,
              const lfp_spawnattr_t *attr,
              bool search)
{
    if (pid == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }

    int ret, pipes[2];

    // Used for passing the error code from child to parent in case
    // some of the syscalls executed in the child fail
    ret = lfp_pipe(pipes, O_CLOEXEC | O_NONBLOCK);
    if (ret < 0) {
        return -1;
    }

    pid_t child_pid = fork();

    if (child_pid < 0) {
        return -1;
    } else if (child_pid == 0) { // child
        close(pipes[0]);
        execve(path, argv, envp);
        int child_errno = lfp_errno();
        write(pipes[1], &child_errno, sizeof(int));
        _exit(255);
    } else {                 // parent
        int child_errno, read_errno, status;
        close(pipes[1]);
        ret = read(pipes[0], &child_errno, sizeof(int));
        read_errno = lfp_errno();
        close(pipes[0]);
        switch (ret) {
        case -1:
            lfp_set_errno(read_errno);
            return -1;
        case 0:
            *pid = child_pid;
            return 0;
        case 4:
            waitpid(child_pid, &status, WNOHANG);
            lfp_set_errno(child_errno);
            return -1;
        default:
            // This is not suppose to happen because all 4 octets
            // of the child's errno should get here with one write
            lfp_set_errno(EDOM);
            return -1;
        }
    }
}
