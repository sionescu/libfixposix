#include <sys/mman.h>

#include <libfixposix.h>

void *lfp_mmap(void *addr, size_t length, int prot,
               int flags, int fd, off_t offset)
{
    return mmap(addr, length, prot, flags, fd, offset);
}

int lfp_munmap(void *addr, size_t length)
{
    return munmap(addr, length);
}
