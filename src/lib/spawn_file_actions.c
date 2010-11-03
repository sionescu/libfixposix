#include <unistd.h>
#include <string.h>

#include <libfixposix.h>
#include "utils.h"
#include "spawn.h"

typedef enum {
    LFP_SPAWN_FILE_ACTION_OPEN,
    LFP_SPAWN_FILE_ACTION_CLOSE,
    LFP_SPAWN_FILE_ACTION_DUP2
} lfp_spawn_action_type;

typedef struct lfp_spawn_action {
    lfp_spawn_action_type type;
    int fd;
    int newfd;
    const char *path;
    uint64_t flags;
    mode_t mode;
} lfp_spawn_action;

int lfp_spawn_file_actions_init(lfp_spawn_file_actions_t *file_actions)
{
    SYSCHECK(EINVAL, file_actions == NULL);
    file_actions->initialized = 0;
    file_actions->allocated = 0;
    file_actions->actions = NULL;
    return 0;
}

int lfp_spawn_file_actions_destroy(lfp_spawn_file_actions_t *file_actions)
{
    SYSCHECK(EINVAL, file_actions == NULL);
    if (file_actions->actions) {
            free(file_actions->actions);
    }
    // lfp_spawn_file_actions_init(file_actions);
    return 0;
}

static
lfp_spawn_action* lfp_spawn_file_actions_allocate(lfp_spawn_file_actions_t *file_actions)
{
    int index = file_actions->initialized++;
    int allocated = file_actions->allocated;
    lfp_spawn_action *actions = file_actions->actions;
    int new_allocated;
    lfp_spawn_action *new_actions;

    if (index >= allocated) {
        /* Note: this code assumes we run out of memory before we overflow. */
        new_allocated = 4 + (allocated*3)/2;
        new_actions = calloc(new_allocated,sizeof(lfp_spawn_action));
        if (!new_actions) {
            return NULL;
        }
        if (actions) {
            memcpy(new_actions, actions, allocated*sizeof(lfp_spawn_action));
            free(actions);
        }
        actions = new_actions;
        file_actions->actions = actions;
        file_actions->allocated = new_allocated;
        memset(actions+index, 0, (new_allocated-index)*sizeof(lfp_spawn_action));
    }
    return actions+index;
}

int lfp_spawn_file_actions_addopen(lfp_spawn_file_actions_t *file_actions,
                                   int fd, const char *path,
                                   uint64_t oflags, mode_t mode)
{
    lfp_spawn_action *action;

    SYSCHECK(EINVAL, file_actions == NULL);
    SYSCHECK(EBADF, !VALID_FD(fd));
    action = lfp_spawn_file_actions_allocate(file_actions);
    SYSCHECK(ENOMEM, !action);
    action->type = LFP_SPAWN_FILE_ACTION_OPEN;
    action->fd = fd;
    action->path = path;
    action->flags = oflags;
    action->mode = mode;
    return 0;
}

int lfp_spawn_file_actions_addclose(lfp_spawn_file_actions_t *file_actions,
                                    int fd)
{
    lfp_spawn_action *action;

    SYSCHECK(EINVAL, file_actions == NULL);
    SYSCHECK(EBADF, !VALID_FD(fd));
    action = lfp_spawn_file_actions_allocate(file_actions);
    SYSCHECK(ENOMEM, !action);
    action->type = LFP_SPAWN_FILE_ACTION_CLOSE;
    action->fd = fd;
    return 0;
}

int lfp_spawn_file_actions_adddup2(lfp_spawn_file_actions_t *file_actions,
                                   int fd, int newfd)
{
    lfp_spawn_action *action;

    SYSCHECK(EINVAL, file_actions == NULL);
    SYSCHECK(EBADF, !VALID_FD(fd));
    SYSCHECK(EBADF, !VALID_FD(newfd));
    action = lfp_spawn_file_actions_allocate(file_actions);
    SYSCHECK(ENOMEM, !action);
    action->type = LFP_SPAWN_FILE_ACTION_DUP2;
    action->fd = fd;
    action->newfd = newfd;
    return 0;
}

static
int lfp_spawn_apply_one_file_action(const lfp_spawn_action *action)
{
    int err;
    int fd;

    switch (action->type) {
    case LFP_SPAWN_FILE_ACTION_OPEN:
        fd = open(action->path, action->flags, action->mode);
        if (fd == -1) { return errno; }
        if (fd != action->fd) {
            err = dup2(fd, action->fd);
            if (err == -1) { return errno; }
            err = close(fd);
            if (err == -1) { return errno; }
        }
        return 0;
    case LFP_SPAWN_FILE_ACTION_CLOSE:
        err = close(action->fd);
        if (err == -1) { return errno; }
        return 0;
    case LFP_SPAWN_FILE_ACTION_DUP2:
        err = dup2(action->fd, action->newfd);
        if (err == -1) { return errno; }
        return 0;
    default:
        return EINVAL;
    }
}

int lfp_spawn_apply_file_actions(const lfp_spawn_file_actions_t *file_actions)
{
    lfp_spawn_action *action = file_actions->actions;
    int err;

    for ( int count = file_actions->initialized; count > 0; count-- ) {
        err = lfp_spawn_apply_one_file_action (action++);
        if (err) { return err; }
    }
    return 0;
}
