#include <sys/resource.h>

#include <libfixposix.h>


int lfp_getrlimit(int resource, struct rlimit *rlim)
{
    return getrlimit(resource, rlim);
}

int lfp_setrlimit(int resource, const struct rlimit *rlim)
{
    return setrlimit(resource, rlim);
}
