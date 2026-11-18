/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-17
 * SPDX-License-Identifier: MIT
 */

#ifndef PORT_LIBINTL_H
#define PORT_LIBINTL_H

#ifdef __belka__
/* The native port uses the compiled-in English message catalogue. */
static inline char *
gettext(const char *message)
{
        return (char *)message;
}

static inline char *
dgettext(const char *domain, const char *message)
{
        (void)domain;
        return (char *)message;
}

static inline char *
textdomain(const char *domain)
{
        return (char *)domain;
}
#else
#include_next <libintl.h>
#endif

#endif
