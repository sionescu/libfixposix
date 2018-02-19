#include <stdio.h>

#include <lfp/buildinfo.h>

int main()
{
    struct lfp_buildinfo info;
    int ret = lfp_buildinfo(&info);
    if (ret != 0) {
        perror("lfp_buildinfo()");
        return 1;
    }

    unsigned major = (info.release >> 16) & 0xff;
    unsigned minor = (info.release >>  8) & 0xff;
    unsigned micro =  info.release        & 0xff;
    printf("%u.%u.%u;%s\n", major, minor, micro, info.vcsid);
    return 0;
}
