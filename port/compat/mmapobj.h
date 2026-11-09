/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Portions Copyright 2026 Setsuna Software L.C.
 */

/*
 * The mmapobj(2) result, which the runtime linker's header names, as
 * illumos's <sys/mman.h> defines it.
 */

#ifndef _COMPAT_MMAPOBJ_H
#define _COMPAT_MMAPOBJ_H

#define	MR_PADDING	0x1
#define	MR_HDR_ELF	0x2
#define	MR_TYPE_MASK	0x0000ffff
#define	MR_GET_TYPE(val)	((val) & MR_TYPE_MASK)

typedef struct mmapobj_result {
	caddr_t		mr_addr;	/* mapping address */
	size_t		mr_msize;	/* mapping size */
	size_t		mr_fsize;	/* file size */
	size_t		mr_offset;	/* offset into file */
	uint_t		mr_prot;	/* the protections provided */
	uint_t		mr_flags;	/* info on the mapping */
} mmapobj_result_t;

#endif /* _COMPAT_MMAPOBJ_H */
