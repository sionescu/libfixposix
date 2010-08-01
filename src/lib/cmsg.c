#include <sys/socket.h>

#include <libfixposix.h>

struct cmsghdr* lfp_cmsg_firsthdr (struct msghdr* msgh)
{
  return CMSG_FIRSTHDR(msgh);
}

struct cmsghdr* lfp_cmsg_nxthdr (struct msghdr* msgh, struct cmsghdr* cmsg)
{
  return CMSG_NXTHDR(msgh, cmsg);
}

size_t lfp_cmsg_align (size_t length)
{
  return CMSG_ALIGN(length);
}

size_t lfp_cmsg_space (size_t length)
{
  return CMSG_SPACE(length);
}

size_t lfp_cmsg_len (size_t length)
{
  return CMSG_LEN(length);
}

void* lfp_cmsg_data (struct cmsghdr* cmsg)
{
  return CMSG_DATA(cmsg);
}
