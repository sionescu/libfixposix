#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#include <lfp.h>

#define CHILDREN 128
#define NTEMPFILES (TMP_MAX/CHILDREN)

int main(void) {
    int parent=1;
    char top_tempfile[]="/tmp/aaaXXXXXX";
    static char tempfiles[NTEMPFILES][15];
    static pid_t children[CHILDREN];

    close(lfp_mkstemp(top_tempfile));

    for (int i = 0; i < CHILDREN;++i) {
        pid_t child = fork();
        if(child) {
            children[i]=child;
        }
        else {
            parent=0;
            break;
        }
    }

    for(int i=0;i<NTEMPFILES;++i) {
        int fd;
        strcpy(tempfiles[i],"/tmp/aaaXXXXXX");
        fd=lfp_mkstemp(tempfiles[i]);
        if(fd < 0) {
            fprintf(stderr,"%s\n",tempfiles[i]);
            perror("Can't create fd");
            assert(0);
        }
        close(fd);
    }
    sleep(1);
    for(int i=0;i<NTEMPFILES;++i) {
        unlink(tempfiles[i]);
    }
    if(parent) {
        for(int i=0;i<CHILDREN;++i) {
            waitpid(children[i],NULL,0);
        }
        unlink(top_tempfile);
    }
    return 0;
}
