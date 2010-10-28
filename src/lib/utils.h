#if !defined(_LFP_UTILS_H_)
# define _LFP_UTILS_H_

#include <sys/types.h>
#include <sys/time.h>

void _lfp_timespec_to_timeval(struct timespec *ts, struct timeval *tv);

#define SYSERR(errcode) do { lfp_set_errno(errcode); return -1; } while(0)

#define SYSCHECK(errcode,expr) do { if(expr) SYSERR(errcode); } while(0)

/* not checking for OPEN_MAX, which might not be valid, on Linux */
#define VALID_FD(fd) (fd>=0)

#endif // _LFP_UTILS_H_
