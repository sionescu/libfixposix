#include <sys/socket.h>

#include <libfixposix.h>

extern
struct cmsghdr* lfp_cmsg_firsthdr (struct msghdr* msgh)
{
  return CMSG_FIRSTHDR(msgh);
}

extern
struct cmsghdr* lfp_cmsg_nxthdr (struct msghdr* msgh, struct cmsghdr* cmsg)
{
  return CMSG_NXTHDR(msgh, cmsg);
}

extern
size_t lfp_cmsg_align (size_t length)
{
  return CMSG_ALIGN(length);
}

extern
size_t lfp_cmsg_space (size_t length)
{
  return CMSG_SPACE(length);
}

extern
size_t lfp_cmsg_len (size_t length)
{
  return CMSG_LEN(length);
}

extern
void* lfp_cmsg_data (struct cmsghdr* cmsg)
{
  return CMSG_DATA(cmsg);
}
