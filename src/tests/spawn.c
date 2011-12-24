#include <stdio.h>

#include <lfp.h>

static
void error_abort (const char* msg, int perrorp)
{
    if (perrorp) {
        perror(msg);
    } else {
        fprintf(stderr,"%s\n",msg);
    }
    abort();
}

int main (void)
{
    // Not implemented yet
    pid_t pid;
    char *path = "/bin/sh";
    char *myargv[] = {"/bin/sh","-c","echo foo ; echo bar >&2 ; cat",NULL};
    lfp_spawn_file_actions_t file_actions;
    lfp_spawnattr_t attr;
    int err, status;
    char tempnam1[] = {"/tmp/lfp_spawn_test_1_XXXXXX"};
    char tempnam2[] = {"/tmp/lfp_spawn_test_2_XXXXXX"};
    int tempfd1, tempfd2;
    char buf[256];
    char expected[] = {"foo\nbar\nquux\n"};

    if ( (tempfd1 = mkstemp(tempnam1)) == -1 ) {
        error_abort("failed to create temp file 1", true);
    }
    if ( (tempfd2 = mkstemp(tempnam2)) == -1 ) {
        error_abort("failed to create temp file 2", true);
    }
    if (write(tempfd1,"quux\n",5)!=5) {
        error_abort("failed to initialize temp file 1", true);
    }
    if (close(tempfd1) == -1) {
        error_abort("failed to close temp file 1", true);
    }
    if (close(tempfd2) == -1) {
        error_abort("failed to close temp file 2", true);
    }
    err = lfp_spawn_file_actions_init(&file_actions);
    if (err) { error_abort("failed to init file actions", true); }
    err = lfp_spawnattr_init(&attr);
    if (err) { error_abort("failed to init attr", true); }
    err = lfp_spawn_file_actions_addopen(&file_actions,10,tempnam1,O_EXCL|O_RDONLY,0);
    err = lfp_spawn_file_actions_adddup2(&file_actions,10,0);
    err = lfp_spawn_file_actions_addclose(&file_actions,10);
    err = lfp_spawn_file_actions_addopen(&file_actions,1,tempnam2,O_WRONLY|O_APPEND,0);
    err = lfp_spawn_file_actions_adddup2(&file_actions,1,2);
    err = lfp_spawn(&pid, path, myargv, lfp_get_environ(), &file_actions, &attr);
    if (err) { error_abort("failed to spawn", true); }
    fprintf(stderr,"spawned child %d\n",pid);
    err = lfp_spawn_file_actions_destroy(&file_actions);
    if (err) { error_abort("failed to init file actions", true); }
    err = lfp_spawnattr_destroy(&attr);
    if (err) { error_abort("failed to init attr", true); }
    err = waitpid(pid,&status,0);
    if (err == -1) { error_abort("failed to wait for child", true); }
    if (!WIFEXITED(status)) { error_abort("child didn't exit properly", false); }
    if (WEXITSTATUS(status)) { error_abort("child didn't exited with status 0", false); }
    tempfd2 = open(tempnam2, O_RDONLY);
    if (tempfd2==-1) { error_abort("failed to open the output file", true); }
    err = read(tempfd2,buf,sizeof(buf));
    if (err == -1) { error_abort("failed to read the output file", true); }
    if (err != (sizeof(expected)-1)) {
        fprintf(stderr,"expected %zu bytes, got %d\n", sizeof(expected)-1, err);
        error_abort("found the wrong number of bytes", false);
    }
    err = strncmp(expected,buf,err);
    if (err) {
        error_abort("output doesn't match expectation", false);
    }
    err = unlink(tempnam1);
    if (err == -1) { error_abort("failed to remove the input file", true); }
    unlink(tempnam2);
    if (err == -1) { error_abort("failed to remove the output file", true); }
    fprintf(stderr,"spawn test successful\n");
    exit(0);
}
