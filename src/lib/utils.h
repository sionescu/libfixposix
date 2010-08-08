#if !defined(_LFP_UTILS_H_)
# define _LFP_UTILS_H_

#define SYSERR(errcode) { lfp_set_errno(errcode); return -1; }

#define SYSCHECK(errcode,expr) if(expr) SYSERR(errcode)

#endif // _LFP_UTILS_H_
