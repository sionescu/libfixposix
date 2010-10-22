#if !defined(_LFP_SPAWN_H_)
# define _LFP_SPAWN_H_

#include <libfixposix.h>

int lfp_spawnattr_apply(const lfp_spawnattr_t *attr);

int lfp_spawn_apply_file_actions(const lfp_spawn_file_actions_t *file_actions);

#endif // _LFP_SPAWN_H_
