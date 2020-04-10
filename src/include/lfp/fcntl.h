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

#if !defined(_LFP_FCNTL_H_)
# define _LFP_FCNTL_H_

#include <lfp/aux.h>

LFP_BEGIN_DECLS

#include <fcntl.h>

#include <inttypes.h>
#include <stdbool.h>

#if !defined(O_CLOEXEC)
// Syscalls use "int" for passing flags, and since
// *nix systems use the LP64 data model, "int" is 32 bits
// which means that we can allocate unsupported flags in the
// upper part of an uint64_t value
# define O_CLOEXEC ( 1ULL << 32 )
#endif

int lfp_open(const char *pathname, uint64_t flags, ...);

int lfp_open_k(int *newfd, const char *pathname, uint64_t flags, mode_t mode);

int lfp_openpt(uint64_t flags);

int lfp_creat(const char *pathname, mode_t mode);

int lfp_is_fd_cloexec(int fd);

int lfp_set_fd_cloexec(int fd, bool enabled);

int lfp_is_fd_nonblock(int fd);

int lfp_set_fd_nonblock(int fd, bool enabled);

LFP_END_DECLS

#endif /* _LFP_FCNTL_H_ */
