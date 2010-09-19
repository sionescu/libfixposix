#include <dirent.h>

#include <libfixposix.h>

int lfp_readdir(DIR *dirp, struct dirent *entry, struct dirent **result)
{
    return readdir_r(dirp, entry, result);
}
