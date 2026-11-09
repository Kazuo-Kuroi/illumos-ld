/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-09
 * SPDX-License-Identifier: MIT
 */

/*
 * Included only when a conversion buffer size in conv.h does not match
 * the size libconv computes; illumos generates one that names the
 * right value. The #error after the include says which constant.
 */

#ifndef REPORT_BUFSIZE
#error "REPORT_BUFSIZE must be defined before including report_bufsize.h"
#else
#warning "A buffer size in conv.h does not match its computed size (REPORT_BUFSIZE)"
#endif
