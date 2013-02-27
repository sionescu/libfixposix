#include <lfp.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#define CHILDREN 128
#define NTEMPFILES (TMP_MAX/CHILDREN)

char **allocStrings(int nstrings, int length)
{
    char **out=malloc(sizeof(char *) *nstrings);
    char *buf=malloc(length*nstrings);
    char *p=buf;
    for(int i=0;i<nstrings;++i,p+=length)
    {
        out[i]=p;
    }
    return out;
}

void *stuff(void *arg)
{
    char **tempfiles=allocStrings(NTEMPFILES,15) ;
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
    return NULL;
}

int main(void) {
    char top_tempfile[]="/tmp/aaaXXXXXX";
    static pthread_t children[CHILDREN];

    close(lfp_mkstemp(top_tempfile));

    for (int i = 0; i < CHILDREN;++i) {
        pthread_create(&children[i],NULL,stuff,NULL);
    }
    stuff(NULL);
    for(int i=0;i<CHILDREN;++i) {
        pthread_join(children[i],NULL);
    }
    unlink(top_tempfile);
    return 0;
}
