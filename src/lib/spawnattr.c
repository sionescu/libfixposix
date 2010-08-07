#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <libfixposix.h>

#define LFP_SPAWN_ALLFLAGS ( LFP_SPAWN_SETSIGMASK    | \
                             LFP_SPAWN_SETSIGDEFAULT | \
                             LFP_SPAWN_SETPGROUP     | \
                             LFP_SPAWN_RESETIDS      | \
                             LFP_SPAWN_SETUGID         )

int lfp_spawnattr_init(lfp_spawnattr_t *attr)
{
    if (attr == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    memset(attr, 0, sizeof(lfp_spawnattr_t));
    sigemptyset(&attr->sigdefault);
    return 0;
}

int lfp_spawnattr_destroy(lfp_spawnattr_t *attr)
{
    if (attr == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    return 0;
}

int lfp_spawnattr_getflags(lfp_spawnattr_t *attr, uint32_t *flags)
{
    if (attr == NULL || flags == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    *flags = attr->flags;
    return 0;
}

int lfp_spawnattr_setflags(lfp_spawnattr_t *attr, const uint32_t flags)
{
    if (attr == NULL || (flags & ~LFP_SPAWN_ALLFLAGS) != 0) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    attr->flags = flags;
    return 0;
}

int lfp_spawnattr_getpgroup(lfp_spawnattr_t *attr, pid_t *pgroup)
{
    if (attr == NULL || pgroup == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    *pgroup = attr->pgroup;
    return 0;
}

int lfp_spawnattr_setpgroup(lfp_spawnattr_t *attr, const pid_t pgroup)
{
    if (attr == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    attr->flags |= LFP_SPAWN_SETPGROUP;
    attr->pgroup = pgroup;
    return 0;
}

int lfp_spawnattr_getsigmask(lfp_spawnattr_t *attr, sigset_t *sigmask)
{
    if (attr == NULL || sigmask == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    *sigmask = attr->sigmask;
    return 0;
}

int lfp_spawnattr_setsigmask(lfp_spawnattr_t *attr, const sigset_t *sigmask)
{
    if (attr == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    attr->flags |= LFP_SPAWN_SETSIGMASK;
    attr->sigmask = *sigmask;
    return 0;
}

int lfp_spawnattr_getsigdefault(lfp_spawnattr_t *attr, sigset_t *sigdefault)
{
    if (attr == NULL || sigdefault == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    *sigdefault = attr->sigdefault;
    return 0;
}

int lfp_spawnattr_setsigdefault(lfp_spawnattr_t *attr, const sigset_t *sigdefault)
{
    if (attr == NULL || sigdefault == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    attr->flags |= LFP_SPAWN_SETSIGDEFAULT;
    attr->sigdefault = *sigdefault;
    return 0;
}

int lfp_spawnattr_getugid(lfp_spawnattr_t *attr, uid_t *uid, gid_t *gid)
{
    if (attr == NULL || uid == NULL || gid == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    *uid = attr->uid;
    *gid = attr->gid;
    return 0;
}

int lfp_spawnattr_setugid(lfp_spawnattr_t *attr, const uid_t uid, const gid_t gid)
{
    if (attr == NULL) {
        lfp_set_errno(EINVAL);
        return -1;
    }
    attr->flags |= LFP_SPAWN_SETUGID;
    attr->uid = uid;
    attr->gid = gid;
    return 0;
}



int lfp_apply_spawnattr(const lfp_spawnattr_t *attr)
{
    if(attr == NULL)
        return 0;

    if (attr->flags & LFP_SPAWN_SETPGROUP)
        if (setpgid(0, attr->pgroup) < 0)
            return lfp_errno();

    if (attr->flags & LFP_SPAWN_RESETIDS) {
        if (seteuid(getuid()) < 0) 
            return lfp_errno();
        if (setegid(getgid()) < 0)
            return lfp_errno();
    }

    if (attr->flags & LFP_SPAWN_SETUGID) {
        if (seteuid(attr->uid) < 0) 
            return lfp_errno();
        if (setegid(attr->gid) < 0)
            return lfp_errno();
    }

    if (attr->flags & LFP_SPAWN_SETSIGMASK)
        if (sigprocmask(SIG_SETMASK, &attr->sigmask, NULL) < 0)
            return lfp_errno();

    if (attr->flags & LFP_SPAWN_SETSIGDEFAULT) {
        struct sigaction sa = { .sa_flags   = 0, .sa_handler = SIG_DFL };
        for (int i = 1; i <= LFP_NSIG; i++)
            if (sigismember(&attr->sigdefault, i))
                if (sigaction(i, &sa, NULL) < 0)
                    return lfp_errno();
    }

    return 0;
}
