/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-17
 * SPDX-License-Identifier: MIT
 */

#ifndef PORT_SYSMACROS_H
#define PORT_SYSMACROS_H

#ifdef __belka__
#include <stdint.h>
#define major(device) ((unsigned int)((uint64_t)(device) >> 32))
#define minor(device) ((unsigned int)(uint64_t)(device))
#define makedev(major_number, minor_number) \
        (((uint64_t)(major_number) << 32) | (uint32_t)(minor_number))
#else
#include_next <sys/sysmacros.h>
#endif

#endif
