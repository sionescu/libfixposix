#include <string.h>

#include <libfixposix.h>
#include "utils.h"

#if defined(HAVE___XPG_STRERROR_R)
int __xpg_strerror_r (int errnum, char *buf, size_t buflen);
#endif

int lfp_strerror (int errnum, char *buf, size_t buflen)
{
    SYSCHECK(EINVAL, buf == NULL);
#if defined(HAVE___XPG_STRERROR_R)
    return __xpg_strerror_r(errnum, buf, buflen);
#else
    return strerror_r(errnum, buf, buflen);
#endif // HAVE___XPG_STRERROR_R
}

size_t lfp_strnlen(const char *s, size_t maxlen)
{
    for (size_t i = 0; i < maxlen; i++) {
        if (s[i] == '\0') {
            return i;
        }
    }
    return maxlen;
}
