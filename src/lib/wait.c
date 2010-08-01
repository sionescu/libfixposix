#include <sys/wait.h>

#include <libfixposix.h>

int lfp_wifexited (int status) {
  return WIFEXITED(status);
}

int lfp_wexitstatus (int status) {
  return WEXITSTATUS(status);
}

int lfp_wifsignaled (int status) {
  return WIFSIGNALED(status);
}

int lfp_wtermsig (int status) {
  return WTERMSIG(status);
}

int lfp_wcoredump (int status) {
  #ifdef WCOREDUMP
  return WCOREDUMP(status);
  #else
  return 0;
  #endif
}

int lfp_wifstopped (int status) {
  return WIFSTOPPED(status);
}

int lfp_wstopsig (int status) {
  return WSTOPSIG(status);
}

int lfp_wifcontinued (int status) {
  return WIFCONTINUED(status);
}

