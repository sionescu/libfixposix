#include <stdlib.h>

#include <libfixposix.h>

int lfp_mkstemp(char *template)
{
    return mkstemp(template);
}
