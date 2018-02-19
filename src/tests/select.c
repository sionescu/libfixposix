#include <stdio.h>

#include <lfp.h>

int main(void)
{
    fd_set rset, wset, eset;

    printf("1..1\n");

    lfp_fd_zero(&rset);
    lfp_fd_zero(&wset);
    lfp_fd_zero(&eset);

    for(unsigned i = 0; i < FD_SETSIZE; i++) {
        if(lfp_fd_isset(i, &rset)) {
            printf("%d ", i);
        }
    }

    printf("ok 1\n");
    return 0;
}
