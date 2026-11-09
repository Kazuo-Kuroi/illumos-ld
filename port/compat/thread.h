/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-09
 * SPDX-License-Identifier: MIT
 */

/* Solaris threads and synchronisation, over POSIX threads. */

#ifndef _COMPAT_THREAD_H
#define _COMPAT_THREAD_H

#include <pthread.h>

typedef pthread_t thread_t;
typedef pthread_mutex_t mutex_t;
typedef pthread_cond_t cond_t;
typedef pthread_rwlock_t rwlock_t;
typedef pthread_key_t thread_key_t;

#define USYNC_THREAD    0
#define DEFAULTMUTEX    PTHREAD_MUTEX_INITIALIZER
#define DEFAULTRWLOCK   PTHREAD_RWLOCK_INITIALIZER
#define THR_ONCE_KEY    ((pthread_key_t)-1)

#define thr_self()              pthread_self()
#define mutex_init(m, t, a)     pthread_mutex_init((m), NULL)
#define mutex_lock(m)           pthread_mutex_lock(m)
#define mutex_unlock(m)         pthread_mutex_unlock(m)
#define rwlock_init(l, t, a)    pthread_rwlock_init((l), NULL)
#define rw_rdlock(l)            pthread_rwlock_rdlock(l)
#define rw_wrlock(l)            pthread_rwlock_wrlock(l)
#define rw_unlock(l)            pthread_rwlock_unlock(l)
#define thr_keycreate(k, d)     pthread_key_create((k), (d))
#define thr_setspecific(k, v)   pthread_setspecific((k), (v))

static inline int
thr_getspecific(thread_key_t k, void **v)
{
        *v = pthread_getspecific(k);
        return 0;
}

/* Create *k the first time; THR_ONCE_KEY marks one not yet created. */
static inline int
thr_keycreate_once(thread_key_t *k, void (*d)(void *))
{
        static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
        int r = 0;

        pthread_mutex_lock(&m);
        if (*k == THR_ONCE_KEY)
                r = pthread_key_create(k, d);
        pthread_mutex_unlock(&m);
        return r;
}

/* Whether this is the process's first thread: the linker does not care. */
static inline int
thr_main(void)
{
        return 1;
}

#endif /* _COMPAT_THREAD_H */
