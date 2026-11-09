/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-09
 * SPDX-License-Identifier: MIT
 */

/* Kernel-style assertions: ASSERT is off, VERIFY is assert(). */

#ifndef _COMPAT_SYS_DEBUG_H
#define _COMPAT_SYS_DEBUG_H

#include <assert.h>

/* Reports a failed assertion (usr/src/cmd/sgs/common/assfail.c). */
int assfail(const char *, const char *, int);

#define ASSERT(x)               ((void)0)
#define ASSERT0(x)              ((void)0)
#define ASSERT3U(a, op, b)      ((void)0)
#define ASSERT3S(a, op, b)      ((void)0)
#define ASSERT3P(a, op, b)      ((void)0)
#define IMPLY(a, b)             ((void)0)
#define EQUIV(a, b)             ((void)0)
#define VERIFY(x)               assert(x)
#define VERIFY0(x)              assert((x) == 0)
#define VERIFY3U(a, op, b)      assert((a) op (b))
#define VERIFY3S(a, op, b)      assert((a) op (b))
#define VERIFY3P(a, op, b)      assert((a) op (b))

/* A compile-time assertion, in C99. */
#define CTASSERT_CAT2(a, b)     a##b
#define CTASSERT_CAT(a, b)      CTASSERT_CAT2(a, b)
#define CTASSERT(x)     \
        typedef char CTASSERT_CAT(ctassert_, __LINE__)[(x) ? 1 : -1] \
            __attribute__((__unused__))

#endif /* _COMPAT_SYS_DEBUG_H */
