/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-09
 * SPDX-License-Identifier: MIT
 */

/*
 * Included before every source file: the Solaris types and macros the
 * illumos link-editor expects, on a host whose C library is not
 * illumos's.
 */

#ifndef _SOLCOMPAT_H
#define _SOLCOMPAT_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <libintl.h>

typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef unsigned long ulong_t;
typedef long long longlong_t;
typedef unsigned long long u_longlong_t;
typedef enum { B_FALSE = 0, B_TRUE = 1 } boolean_t;
typedef int32_t lwpid_t;
typedef uint64_t hrtime_t;
#ifdef __belka__
typedef long Lmid_t;
#define UID_NOBODY 32767
#define GID_NOBODY 32767
#define O_LARGEFILE 0
#endif
typedef char *caddr_t;

#ifndef _LP64
#define _LP64 1
#endif
#define _LONGLONG_TYPE 1
#define _INT64_TYPE 1

#include <sys/isa_defs.h>
#include <sys/auxv.h>

#ifndef __maybe_unused
#define __maybe_unused __attribute__((__unused__))
#endif
#ifndef __unused
#define __unused __attribute__((__unused__))
#endif
#define __NORETURN __attribute__((__noreturn__))
#define __PRINTFLIKE(n) __attribute__((__format__(__printf__, n, (n) + 1)))
#define __VPRINTFLIKE(n) __attribute__((__format__(__printf__, n, 0)))

#define PAGESIZE        4096
#define MILLISEC        1000
#define MICROSEC        1000000
#define NANOSEC         1000000000

#ifndef MAX
#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))
#endif
#define ISDIGIT(c)      ((c) >= '0' && (c) <= '9')

/* Lock assertions, which compile to nothing here. */
#define MUTEX_HELD(m)   1
#define RW_LOCK_HELD(l) 1
#define RW_WRITE_HELD(l) 1

#include "mmapobj.h"

/*
 * resolvepath(2): realpath() that returns the length it stored, without
 * a terminating NUL.
 */
static inline int
resolvepath(const char *path, char *buf, size_t bufsiz)
{
        char full[PATH_MAX];
        size_t n;

        if (realpath(path, full) == NULL)
                return -1;
        n = strlen(full);
        if (n > bufsiz)
                n = bufsiz;
        memcpy(buf, full, n);
        return (int)n;
}

#endif /* _SOLCOMPAT_H */
