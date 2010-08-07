#include <unistd.h>

#include <libfixposix.h>

typedef enum {
    LFP_SPAWN_FILE_ACTION_OPEN,
    LFP_SPAWN_FILE_ACTION_CLOSE,
    LFP_SPAWN_FILE_ACTION_DUP2
} lfp_spawn_action_type;

typedef struct lfp_spawn_action {
    lfp_spawn_action_type type;
    int fd;
    int newfd;
    char *path;
    lfp_open_flags_t flags;
    mode_t mode;
} lfp_spawn_action;

int lfp_spawn_file_actions_init(lfp_spawn_file_actions_t *file_actions)
{
    if (file_actions == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    file_actions->actions = NULL;
    return 0;
}

int lfp_spawn_file_actions_destroy(lfp_spawn_file_actions_t *file_actions)
{
    if (file_actions == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    // TODO: free file_actions->actions
    return 0;
}
