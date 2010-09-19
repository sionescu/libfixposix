#include <string.h>

#include <libfixposix.h>
#include "utils.h"

#if HAVE___XPG_STRERROR_R
int __xpg_strerror_r (int errnum, char *buf, size_t buflen);
#endif

int lfp_strerror (int errnum, char *buf, size_t buflen)
{
    SYSCHECK(EINVAL, buf == NULL);
#if HAVE___XPG_STRERROR_R
    return __xpg_strerror_r(errnum, buf, buflen);
#else
    return strerror_r(errnum, buf, buflen);
#endif
}
