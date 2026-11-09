#
# Copyright (c) 2026 Setsuna Software L.C.
# Created: 2026-11-09
# SPDX-License-Identifier: MIT
#
# Build the illumos link-editor (ld) on a host other than illumos, with
# GNU make and a C compiler: the sgsmsg message tool, then libconv,
# libelf, liblddbg, and libld as static libraries, then ld.
#
#   make -C port O=objdir [HOSTCC=cc]     builds objdir/ld
#
# The sources are under ../usr/src as illumos-gate has them; compat/
# supplies the Solaris types, headers, and interfaces they expect on a
# host with another C library. Needs GNU m4 (libelf's translators).
#

PORT    := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
SRC     := $(abspath $(PORT)/../usr/src)
SGS     := $(SRC)/cmd/sgs
O       ?= $(PORT)/obj
O       := $(abspath $(O))

HOSTCC  ?= cc
M4      ?= m4
AR      ?= ar
LDLIBS  ?= -ldl

# The illumos sources are not written to the host compiler's warnings.
CFLAGS  ?= -O2 -g
ALLCFLAGS := -std=gnu99 -w -fcommon $(CFLAGS)
CPPFLAGS := -DNATIVE_BUILD -include $(PORT)/compat/solcompat.h \
        -I$(O)/gen -I$(PORT)/compat -I$(SGS)/include -I$(SGS)/include/i386 \
        -I$(SRC)/common/elfcap -I$(SRC)/common/sgsrtcid -I$(SRC)/lib/libc/inc

COMPILE  = $(HOSTCC) $(ALLCFLAGS) $(CPPFLAGS)
SGSMSG  := $(O)/sgsmsg
MSGFLAGS := -i $(SGS)/messages/sgs.ident
SGSCOMMON := $(SGS)/common

all: $(O)/ld

# --- Generated headers ---

# illumos's dlopen() flags, added to the host's <dlfcn.h>.
$(O)/gen/dlfcn.h: $(SRC)/head/dlfcn.h
	@mkdir -p $(O)/gen
	{ echo '/* Generated from illumos <dlfcn.h>: its flags, over the host'"'"'s. */'; \
	  echo '#include_next <dlfcn.h>'; \
	  awk '$$1 == "#define" && $$2 ~ /^RTLD_[A-Z_]+$$/ { \
	        n = $$2; $$1 = ""; $$2 = ""; \
	        print "#ifndef " n; print "#define " n " " $$0; print "#endif" }' $<; \
	} > $@

GEN     := $(O)/gen/dlfcn.h

# --- sgsmsg ---

OS      := $(O)/sgsmsg.d
SGSMSG_OBJS := $(OS)/assfail.o $(OS)/avl.o $(OS)/findprime.o \
        $(OS)/sgsmsg.o $(OS)/string_table.o

$(OS)/%.o: $(SGSCOMMON)/%.c $(GEN)
	@mkdir -p $(OS)
	$(COMPILE) -c -o $@ $<
$(OS)/avl.o: $(SRC)/common/avl/avl.c $(GEN)
	@mkdir -p $(OS)
	$(COMPILE) -c -o $@ $<
$(OS)/sgsmsg.o: $(SRC)/tools/sgs/sgsmsg/sgsmsg.c $(GEN)
	@mkdir -p $(OS)
	$(COMPILE) -c -o $@ $<
$(SGSMSG): $(SGSMSG_OBJS)
	$(HOSTCC) -o $@ $^

# --- libconv ---

CONV    := $(SGS)/libconv/common
OC      := $(O)/libconv
CONV_COM := arch audit c_literal cap config data deftag demangle dl dwarf \
        dwarf_ehe dynamic elf entry globals group lddstub map phdr relocate \
        relocate_i386 relocate_amd64 relocate_sparc sections segments strproc \
        symbols syminfo tokens time version
CONV_ELF := cap_machelf dynamic_machelf globals_machelf sections_machelf \
        symbols_machelf symbols_sparc_machelf
CONV_MSG := arch audit c_literal cap config data deftag demangle dl dwarf \
        dwarf_ehe dynamic elf entry globals group map lddstub phdr \
        relocate_amd64 relocate_i386 relocate_sparc sections segments symbols \
        symbols_sparc syminfo time version
CONV_OBJS := $(CONV_COM:%=$(OC)/%.o) $(CONV_ELF:%=$(OC)/%32.o) \
        $(CONV_ELF:%=$(OC)/%64.o) $(CONV_MSG:%=$(OC)/%_msg.o) \
        $(OC)/elfcap.o $(OC)/vernote.o
CONV_HDRS := $(CONV_MSG:%=$(OC)/%_msg.h) $(GEN)
CONVCPP := -I$(CONV) -I$(OC)

$(OC)/%_msg.h $(OC)/%_msg.c: $(CONV)/%.msg $(SGSMSG)
	@mkdir -p $(OC)
	cd $(OC) && $(SGSMSG) $(MSGFLAGS) -h $*_msg.h -d $*_msg.c \
	    -n sgs_msg_libconv_$* $<
$(OC)/%_msg.o: $(OC)/%_msg.c $(CONV_HDRS)
	$(COMPILE) $(CONVCPP) -c -o $@ $<
$(OC)/%32.o: $(CONV)/%.c $(CONV_HDRS)
	$(COMPILE) $(CONVCPP) -c -o $@ $<
$(OC)/%64.o: $(CONV)/%.c $(CONV_HDRS)
	$(COMPILE) $(CONVCPP) -D_ELF64 -c -o $@ $<
$(OC)/%.o: $(CONV)/%.c $(CONV_HDRS)
	$(COMPILE) $(CONVCPP) -c -o $@ $<
$(OC)/elfcap.o: $(SRC)/common/elfcap/elfcap.c $(CONV_HDRS)
	$(COMPILE) $(CONVCPP) -c -o $@ $<
$(OC)/vernote.o: $(PORT)/vernote.s
	@mkdir -p $(OC)
	$(HOSTCC) -c -o $@ $<
$(O)/libconv.a: $(CONV_OBJS)
	rm -f $@
	$(AR) rcs $@ $^

# --- libelf ---

ELFD    := $(SGS)/libelf/common
OE      := $(O)/libelf
ELF_COM := ar begin cntl cook data end fill flag getarhdr getarsym getbase \
        getdata getehdr getident getphdr getscn getshdr getphnum getshnum \
        getshstrndx hash input kind ndxscn newdata newehdr newphdr newscn next \
        nextscn output rand rawdata rawfile rawput strptr update error gelf \
        clscook checksum
ELF_CLS := clscook newehdr newphdr update checksum
ELF_OBJS := $(ELF_COM:%=$(OE)/%.o) $(ELF_CLS:%=$(OE)/%64.o) $(OE)/msg.o \
        $(OE)/xlate.o $(OE)/xlate64.o
ELFCPP  := -I$(ELFD) -I$(OE)

$(OE)/msg.h $(OE)/msg.c: $(ELFD)/libelf.msg $(ELFD)/libelf.32.msg $(SGSMSG)
	@mkdir -p $(OE)
	cd $(OE) && $(SGSMSG) $(MSGFLAGS) -h msg.h -d msg.c -n libelf_msg \
	    $(ELFD)/libelf.msg $(ELFD)/libelf.32.msg
$(OE)/xlate.c: $(ELFD)/xlate.m4
	@mkdir -p $(OE)
	$(M4) < $< > $@
$(OE)/xlate64.c: $(ELFD)/xlate64.m4
	@mkdir -p $(OE)
	$(M4) < $< > $@
$(OE)/%64.o: $(ELFD)/%.c $(OE)/msg.h $(GEN)
	$(COMPILE) $(ELFCPP) -D_ELF64 -c -o $@ $<
$(OE)/%.o: $(ELFD)/%.c $(OE)/msg.h $(GEN)
	$(COMPILE) $(ELFCPP) -c -o $@ $<
$(OE)/%.o: $(OE)/%.c $(OE)/msg.h $(GEN)
	$(COMPILE) $(ELFCPP) -c -o $@ $<
$(O)/libelf.a: $(ELF_OBJS)
	rm -f $@
	$(AR) rcs $@ $^

# --- liblddbg ---
#
# libld has its own dbg_print(), which on illumos interposes on
# liblddbg's; linked statically, liblddbg's is renamed out of the way so
# that every call reaches libld's, as there.

DBGD    := $(SGS)/liblddbg/common
OD      := $(O)/liblddbg
DBG_COM := args audit basic debug syminfo tls
DBG_CLS := bindings cap dlfcns dynamic elf entry files got libs map move phdr \
        relocate sections segments shdr statistics support syms unused util \
        version
DBG_OBJS := $(DBG_COM:%=$(OD)/%.o) $(DBG_CLS:%=$(OD)/%32.o) \
        $(DBG_CLS:%=$(OD)/%64.o) $(OD)/msg.o $(OD)/alist.o
DBGCPP  := -I$(DBGD) -I$(OD)

$(OD)/msg.h $(OD)/msg.c: $(DBGD)/liblddbg.msg $(SGSMSG)
	@mkdir -p $(OD)
	cd $(OD) && $(SGSMSG) $(MSGFLAGS) -h msg.h -d msg.c -n liblddbg_msg $<
$(OD)/debug.o: $(DBGD)/debug.c $(OD)/msg.h $(GEN)
	$(COMPILE) $(DBGCPP) -Ddbg_print=liblddbg_dbg_print -c -o $@ $<
$(OD)/%32.o: $(DBGD)/%.c $(OD)/msg.h $(GEN)
	$(COMPILE) $(DBGCPP) -c -o $@ $<
$(OD)/%64.o: $(DBGD)/%.c $(OD)/msg.h $(GEN)
	$(COMPILE) $(DBGCPP) -D_ELF64 -c -o $@ $<
$(OD)/%.o: $(DBGD)/%.c $(OD)/msg.h $(GEN)
	$(COMPILE) $(DBGCPP) -c -o $@ $<
$(OD)/msg.o: $(OD)/msg.c $(GEN)
	$(COMPILE) $(DBGCPP) -c -o $@ $<
$(OD)/alist.o: $(SGSCOMMON)/alist.c $(OD)/msg.h $(GEN)
	$(COMPILE) $(DBGCPP) -c -o $@ $<
$(O)/liblddbg.a: $(DBG_OBJS)
	rm -f $@
	$(AR) rcs $@ $^

# --- libld ---

LDD     := $(SGS)/libld/common
OL      := $(O)/libld
LD_COM  := debug globals util
LD_CLS  := args entry exit groups ldentry ldlibs ldmachdep ldmain libs files \
        map map_core map_support map_v2 order outfile place relocate resolve \
        sections sunwmove support syms update unwind version wrap
LD_OBJS := $(LD_COM:%=$(OL)/%.o) $(LD_CLS:%=$(OL)/%32.o) \
        $(LD_CLS:%=$(OL)/%64.o) \
        $(OL)/alist.o $(OL)/assfail.o $(OL)/findprime.o \
        $(OL)/string_table.o $(OL)/strhash.o $(OL)/leb128.o $(OL)/avl.o \
        $(OL)/elfcap.o $(OL)/msg.o \
        $(OL)/doreloc_x86_32.o $(OL)/doreloc_x86_64.o \
        $(OL)/doreloc_sparc_32.o $(OL)/doreloc_sparc_64.o \
        $(OL)/machrel.intel32.o $(OL)/machrel.amd64.o \
        $(OL)/machrel.sparc32.o $(OL)/machrel.sparc64.o \
        $(OL)/machsym.sparc32.o $(OL)/machsym.sparc64.o
LDCPP   := -DUSE_LIBLD_MALLOC -I$(LDD) -I$(OL) -I$(SRC)/uts/common/krtld
LDHDRS  := $(OL)/msg.h $(GEN)

$(OL)/msg.h $(OL)/msg.c: $(LDD)/libld.msg $(LDD)/libld.sparc.msg \
                $(LDD)/libld.intel.msg $(SGSMSG)
	@mkdir -p $(OL)
	cd $(OL) && $(SGSMSG) $(MSGFLAGS) -h msg.h -d msg.c -n libld_msg \
	    $(LDD)/libld.msg $(LDD)/libld.sparc.msg $(LDD)/libld.intel.msg
$(OL)/doreloc_x86_32.o: $(SRC)/uts/intel/ia32/krtld/doreloc.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -DDO_RELOC_LIBLD -c -o $@ $<
$(OL)/doreloc_x86_64.o: $(SRC)/uts/intel/amd64/krtld/doreloc.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -D_ELF64 -DDO_RELOC_LIBLD -c -o $@ $<
$(OL)/doreloc_sparc_32.o: $(SRC)/uts/sparc/krtld/doreloc.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -DDO_RELOC_LIBLD -c -o $@ $<
$(OL)/doreloc_sparc_64.o: $(SRC)/uts/sparc/krtld/doreloc.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -D_ELF64 -DDO_RELOC_LIBLD -c -o $@ $<
$(OL)/%32.o: $(LDD)/%.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -c -o $@ $<
$(OL)/%64.o: $(LDD)/%.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -D_ELF64 -c -o $@ $<
$(OL)/%.o: $(LDD)/%.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -c -o $@ $<
$(OL)/%.o: $(SGSCOMMON)/%.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -c -o $@ $<
$(OL)/avl.o: $(SRC)/common/avl/avl.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -c -o $@ $<
$(OL)/elfcap.o: $(SRC)/common/elfcap/elfcap.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -c -o $@ $<
$(OL)/msg.o: $(OL)/msg.c $(LDHDRS)
	$(COMPILE) $(LDCPP) -c -o $@ $<
$(O)/libld.a: $(LD_OBJS)
	rm -f $@
	$(AR) rcs $@ $^

# --- ld ---

OLD     := $(O)/ld.d
LIBS    := $(O)/libld.a $(O)/liblddbg.a $(O)/libelf.a $(O)/libconv.a

$(OLD)/msg.h $(OLD)/msg.c: $(SGS)/ld/common/ld.msg $(SGSMSG)
	@mkdir -p $(OLD)
	cd $(OLD) && $(SGSMSG) $(MSGFLAGS) -h msg.h -d msg.c -n ld_msg $<
$(OLD)/ld.o: $(SGS)/ld/common/ld.c $(OLD)/msg.h $(GEN)
	$(COMPILE) -I$(OLD) -c -o $@ $<
$(OLD)/msg.o: $(OLD)/msg.c $(GEN)
	$(COMPILE) -I$(OLD) -c -o $@ $<

# The libraries call each other, so they are searched twice.
$(O)/ld: $(OLD)/ld.o $(OLD)/msg.o $(LIBS)
	$(HOSTCC) -o $@ $(OLD)/ld.o $(OLD)/msg.o $(LIBS) $(LIBS) $(LDLIBS)

clean:
	rm -rf $(O)

.PHONY: all clean
.DELETE_ON_ERROR:
