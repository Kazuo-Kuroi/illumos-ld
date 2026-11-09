/*
 * Copyright (c) 2026 Setsuna Software L.C.
 * Created: 2026-11-09
 * SPDX-License-Identifier: MIT
 *
 * The version string ld -V prints, which illumos generates from its
 * release notes.
 */

	.section .rodata
	.globl	link_ver_string
link_ver_string:
	.string	"illumos-gate d493fab2c163 (Belka port)"
	.section .note.GNU-stack,"",@progbits
