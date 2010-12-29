/*******************************************************************************/
/* Permission is hereby granted, free of charge, to any person or organization */
/* obtaining a copy of the software and accompanying documentation covered by  */
/* this license (the "Software") to use, reproduce, display, distribute,       */
/* execute, and transmit the Software, and to prepare derivative works of the  */
/* Software, and to permit third-parties to whom the Software is furnished to  */
/* do so, all subject to the following:                                        */
/*                                                                             */
/* The copyright notices in the Software and this entire statement, including  */
/* the above license grant, this restriction and the following disclaimer,     */
/* must be included in all copies of the Software, in whole or in part, and    */
/* all derivative works of the Software, unless such copies or derivative      */
/* works are solely in the form of machine-executable object code generated by */
/* a source language processor.                                                */
/*                                                                             */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    */
/* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT   */
/* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE   */
/* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, */
/* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER */
/* DEALINGS IN THE SOFTWARE.                                                   */
/*******************************************************************************/

#include <config.h>

#include <lfp/stat.h>
#include <lfp/errno.h>

int lfp_stat(const char *path, struct stat *buf)
{
    return stat(path, buf);
}

int lfp_fstat(int fd, struct stat *buf)
{
    return fstat(fd, buf);
}

int lfp_lstat(const char *path, struct stat *buf)
{
    return lstat(path, buf);
}

int lfp_fd_is_open(int fd)
{
    struct stat buf;
    int ret = fstat(fd, &buf);
    if ( ret < 0 ) {
        if ( lfp_errno() == EBADF ) {
            return false;
        } else {
            return -1;
        }
    } else {
        return true;
    }
}

bool lfp_isreg(mode_t mode)
{
    return (bool) S_ISREG(mode);
}

bool lfp_isdir(mode_t mode)
{
    return (bool) S_ISDIR(mode);
}

bool lfp_ischr(mode_t mode)
{
    return (bool) S_ISCHR(mode);
}

bool lfp_isblk(mode_t mode)
{
    return (bool) S_ISBLK(mode);
}

bool lfp_isfifo(mode_t mode)
{
    return (bool) S_ISFIFO(mode);
}

bool lfp_islnk(mode_t mode)
{
    return (bool) S_ISLNK(mode);
}

bool lfp_issock(mode_t mode)
{
    return (bool) S_ISSOCK(mode);
}
