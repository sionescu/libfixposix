#if !defined(_LFP_UTILS_H_)
# define _LFP_UTILS_H_

#define SYSERR(errcode) do { lfp_set_errno(errcode); return -1; } while(0)

#define SYSCHECK(errcode,expr) if(expr) SYSERR(errcode)

#endif // _LFP_UTILS_H_
