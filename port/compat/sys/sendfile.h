/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-17
 * SPDX-License-Identifier: MIT
 */

#ifndef PORT_SENDFILE_H
#define PORT_SENDFILE_H
#ifdef __belka__
#include <errno.h>
#include <unistd.h>

/* Archive copying over Belka's existing descriptor I/O. */
static inline ssize_t
sendfile(int output, int input, off_t *offset, size_t length)
{
        char buffer[16384];
        size_t copied = 0;

        while (copied < length) {
                size_t wanted = length - copied;
                size_t written = 0;
                ssize_t count;

                if (wanted > sizeof(buffer))
                        wanted = sizeof(buffer);
                do {
                        count = offset != NULL ?
                            pread(input, buffer, wanted, *offset) :
                            read(input, buffer, wanted);
                } while (count < 0 && errno == EINTR);
                if (count <= 0)
                        return count < 0 && copied == 0 ? -1 : (ssize_t)copied;
                while (written < (size_t)count) {
                        ssize_t result = write(output, buffer + written,
                            (size_t)count - written);

                        if (result < 0 && errno == EINTR)
                                continue;
                        if (result <= 0)
                                return copied == 0 ? -1 : (ssize_t)copied;
                        copied += (size_t)result;
                        written += (size_t)result;
                        if (offset != NULL)
                                *offset += result;
                }
        }
        return (ssize_t)copied;
}
#else
#include_next <sys/sendfile.h>
#endif
#endif
