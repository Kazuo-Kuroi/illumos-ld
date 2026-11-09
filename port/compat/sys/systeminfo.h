/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-09
 * SPDX-License-Identifier: MIT
 */

/* sysinfo(2): nothing to report on this host. */

#ifndef _COMPAT_SYS_SYSTEMINFO_H
#define _COMPAT_SYS_SYSTEMINFO_H

#include <errno.h>

#define SI_MACHINE      5
#define SI_ARCHITECTURE 6
#define SI_PLATFORM     513
#define SI_ISALIST      514
#define SYS_NMLN        257

static inline long
sysinfo(int cmd, char *buf, long n)
{
        (void)cmd;
        (void)buf;
        (void)n;
        errno = EINVAL;
        return -1;
}

#endif /* _COMPAT_SYS_SYSTEMINFO_H */
