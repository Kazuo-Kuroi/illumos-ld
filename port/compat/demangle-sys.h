/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-09
 * SPDX-License-Identifier: MIT
 */

/* No demangler: symbol names are shown as they are. */

#ifndef _COMPAT_DEMANGLE_SYS_H
#define _COMPAT_DEMANGLE_SYS_H

#include <stddef.h>

typedef enum sysdem_lang_e {
        SYSDEM_LANG_AUTO,
        SYSDEM_LANG_CPP,
        SYSDEM_LANG_RUST
} sysdem_lang_t;
typedef struct sysdem_ops sysdem_ops_t;

static inline char *
sysdemangle(const char *s, sysdem_lang_t l, sysdem_ops_t *o)
{
        (void)s;
        (void)l;
        (void)o;
        return NULL;
}

#endif /* _COMPAT_DEMANGLE_SYS_H */
