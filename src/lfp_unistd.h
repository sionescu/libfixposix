#if !defined(_LFP_UNISTD_H_)
#define _LFP_UNISTD_H_

#include <unistd.h>
#include <errno.h>

#if !defined(O_CLOEXEC)
# define O_CLOEXEC 0
#endif

int lfp_pipe2 (int pipefd[2], int flags);

#endif // _LFP_UNISTD_H_
