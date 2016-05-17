TBUILDDIR=.build
TSRCDIR=.
NAME=x-os

ASM=nasm
BUILDDIR=$(shell cd $(TBUILDDIR) && pwd)
SRCDIR=$(shell cd $(TSRCDIR) && pwd)
INCDIR=$(SRCDIR)/include
BOOTMODULE=boot
SUBMODULES=systemparams vfs tty0
CFLAGS=-Wall -m32 -std=c++11 -nostartfiles -nostdlib -nodefaultlibs -static -fno-common -fno-use-cxa-atexit -fno-exceptions -fno-non-call-exceptions -fno-weak -fno-rtti -ffreestanding -I$(INCDIR)

export CFLAGS
export ASM

.PHONY: all clean install

all:$(BUILDDIR)/boot.bin $(BUILDDIR)/system.bin

clean:
	$(RM) $(BUILDDIR)/$(NAME)
	$(RM) $(BUILDDIR)/boot.bin
	$(RM) $(BUILDDIR)/system.bin
	$(RM) $(BUILDDIR)/*.a
	$(RM) $(BUILDDIR)/*.o
	for name in `echo $(BOOTMODULE) $(SUBMODULES)`;do\
		cd $(SRCDIR)/$${name} && $(MAKE) clean \
			BUILDDIR=$(BUILDDIR)/$${name}; \
	done

install:$(BUILDDIR)/$(NAME)

$(BUILDDIR)/$(NAME):$(BUILDDIR)/boot.bin $(BUILDDIR)/system.bin
	dd if=/dev/zero of=$@ bs=1024 count=10240 conv=notrunc
	dd if=$(BUILDDIR)/boot.bin of=$@ bs=512 count=5 conv=notrunc
	dd if=$(BUILDDIR)/system.bin of=$@ bs=512 count=384 conv=notrunc seek=5

$(BUILDDIR)/boot.bin:$(BUILDDIR)/$(BOOTMODULE)/out.a
	cp $^ $@

$(BUILDDIR)/system.bin:$(patsubst %, $(BUILDDIR)/lib%.a, $(SUBMODULES)) $(BUILDDIR)/main.o $(BUILDDIR)/head.o
	$(CC) $(CFLAGS) -Wl,--oformat=binary,-m,elf_i386,-M,-x,-e,startup_32,--section-start,.text=0xc0100000 -static \
	-o $@ $(BUILDDIR)/head.o $(BUILDDIR)/main.o $(patsubst %,$(BUILDDIR)/%/*.o,$(SUBMODULES)) -lgcc |\
	tee $(BUILDDIR)/os.symbols.origin |\
	grep -E "(^\s+0x[0-9a-f]+\s+[a-zA-Z_]\S+$$)|(^\s*\.text\s+0x[0-9a-f]+\s+0x[0-9a-f]+\s+[a-zA-Z_]\S+$$)" |\
	sed -r "s/^\s*\.text\s+(0x[0-9a-f]+)\s+0x[0-9a-f]+\s+([a-zA-Z_]\S+)$$/\1 \2/" |\
	sed -r "s/obj\/([a-zA-Z_]\S+)\.so/_______________\1.s/" |\
	sed -r "s/obj\/([a-zA-Z_]\S+)\.o/_______________\1.c/" |\
	sed -r "s/^[\t ]+//" | sed -r "s/[\t ]+/ /" |\
	sed -r "N;s/(0x[0-9a-f]+)\s+([a-zA-Z_]\S+)\s*\n\1\s+([a-zA-Z_]\S+)/\1 \2[\3]/;" > $(BUILDDIR)/os.symbols

$(BUILDDIR)/lib%.a:$(BUILDDIR)/%/out.a
	cp $^ $@

$(BUILDDIR)/%/out.a:$(SRCDIR)/%/* $(INCDIR)/*
	mkdir -p $(BUILDDIR)/$(patsubst $(BUILDDIR)/%/out.a,%,$@)
	cd $(patsubst $(BUILDDIR)/%/out.a,%,$@) && \
	$(MAKE) \
	BUILDDIR=$(BUILDDIR)/$(patsubst $(BUILDDIR)/%/out.a,%,$@) \
	SRCDIR=$(SRCDIR)/$(patsubst $(BUILDDIR)/%/out.a,%,$@) \
	INCDIR=$(INCDIR)

$(BUILDDIR)/head.o:$(SRCDIR)/head.s
	$(ASM) -f elf -o $@ $^

$(BUILDDIR)/main.o:$(SRCDIR)/main.cpp $(BUILDDIR)/main.d
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/main.d:$(SRCDIR)/main.cpp
	@$(RM) $@
	@$(CC) -MM $(CFLAGS) $< | sed 's/\($(patsubst %.c,%,$(notdir $<))\)\.o *: */obj\/\1.o dep\/\1.d:/' > $@

ifneq ($(MAKECMDGOALS),clean)
sinclude $(BUILDDIR)/main.d
endif
