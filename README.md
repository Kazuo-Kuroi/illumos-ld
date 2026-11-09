# illumos-ld

The illumos link-editor (`ld` and `libld`, with `libelf`, `liblddbg`,
`libconv`, and the `sgsmsg` message tool), taken from illumos-gate and
made to build on hosts other than illumos, for
[Belka](https://github.com/Kazuo-Kuroi/belka). Belka uses it as its
link-editor: it links Belka's kernel and its statically linked programs.

## Contents and licences

| Path | What | Licence |
|---|---|---|
| `usr/src/` | illumos-gate sources, at the same paths as there | CDDL 1.0 ([`usr/src/OPENSOLARIS.LICENSE`](usr/src/OPENSOLARIS.LICENSE)) |
| `port/` | the build for other hosts: a GNU makefile and compatibility headers | MIT, except `port/compat/mmapobj.h` (CDDL) |

Every file keeps its own licence notice. Files under `usr/src/` that
Belka changed carry a "Portions Copyright Setsuna Software L.C." line
after their CDDL header, and each change is a separate commit.

## Origin

`usr/src/` was imported unchanged from illumos-gate commit
`d493fab2c163b4634af6f1857b7821b9f6627f4e` (2026-11-09): only the files
the link-editor needs. The Belka changes since are:

1. `libld: do not initialize the debug stream with stderr` (portability:
   `stderr` need not be a constant)
2. `libld: allow TLS in static executables`
3. `libld: drop the __tls_get_addr call when relaxing TLS in a static
   link`
4. `libld: map the ELF and program headers in static executables`
5. `libld: keep a segment's mapfile alignment when it has a fixed
   address`
6. `libld: give executables' segments physical addresses`

illumos has no static executables; Belka's are all static for now, which
changes 2 to 4 make possible. Changes 5 and 6 let a boot loader (Limine)
load a kernel linked with a mapfile.

To move to a later illumos-gate, import the same paths again in one
commit and reapply the changes above.

## Building

Needs GNU make, a C compiler (GCC or Clang), and GNU m4.

```sh
make -C port O=$PWD/obj          # builds obj/ld
make -C port O=$PWD/obj HOSTCC=clang
```

`ld` is the illumos command, with its options and mapfiles (`ld -dn -e
_start -M mapfile ...`); see the illumos `ld(1)` manual and the *Linker
and Libraries Guide*. It links for any target it knows (amd64, i386,
sparc), whatever the host.

## What the port leaves out

* Demangling of C++ names in messages (`compat/demangle-sys.h`).
* `sysinfo(2)` (the ISA list is not reported).
* The parts of libconv and libelf only other tools use (core file notes,
  `nlist`).
* The runtime linker, `ld.so.1`.
