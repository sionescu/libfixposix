#include <stdlib.h>
#include <sys/socket.h>

#include <libfixposix.h>

extern
int lfp_socket(lfp_socket_domain_t domain,
               lfp_socket_type_t   type,
               int protocol)
{
    return socket(domain, type, protocol);
}
