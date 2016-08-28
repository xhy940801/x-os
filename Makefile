TBUILDDIR=.build
TSRCDIR=.
NAME=x-os

ASM=nasm
BUILDDIR=$(shell cd $(TBUILDDIR) && pwd)
SRCDIR=$(shell cd $(TSRCDIR) && pwd)
INCDIR=$(SRCDIR)/include
BOOTMODULE=boot
SUBMODULES=common systemparams vfs tty0 mem task
CFLAGS=-Wall -Werror -m32 -std=c++11 -nostartfiles -nostdlib -nodefaultlibs -static -fno-common -fno-use-cxa-atexit -fno-exceptions -fno-non-call-exceptions -fno-weak -fno-rtti -ffreestanding -iquote$(INCDIR)

ALLSRCFILES=$(foreach MODULE,$(SUBMODULES),$(wildcard $(MODULE)/*.cpp) $(wildcard $(MODULE)/*.s))
CPPSRCFILES=$(foreach MODULE,$(SUBMODULES),$(wildcard $(MODULE)/*.cpp))

export CFLAGS
export ASM

.PHONY: all clean install

all:$(BUILDDIR)/boot.bin $(BUILDDIR)/system.bin

clean:
	$(RM) $(BUILDDIR)/$(NAME)
	$(RM) $(BUILDDIR)/boot.bin
	$(RM) $(BUILDDIR)/system.bin
	$(RM) $(BUILDDIR)/*.o
	$(RM) $(BUILDDIR)/*.d
	$(RM) $(BUILDDIR)/$(BOOTMODULE)/bootsect.bin
	$(RM) $(BUILDDIR)/$(BOOTMODULE)/setup.bin
	$(RM) $(patsubst %,$(BUILDDIR)/%.o,$(ALLSRCFILES))
	$(RM) $(patsubst %,$(BUILDDIR)/%.d,$(CPPSRCFILES))

install:$(BUILDDIR)/$(NAME)

$(BUILDDIR)/$(NAME):$(BUILDDIR)/boot.bin $(BUILDDIR)/system.bin
	dd if=/dev/zero of=$@ bs=1024 count=10240 conv=notrunc
	dd if=$(BUILDDIR)/boot.bin of=$@ bs=512 count=5 conv=notrunc
	dd if=$(BUILDDIR)/system.bin of=$@ bs=512 count=384 conv=notrunc seek=5

$(BUILDDIR)/boot.bin:$(BOOTMODULE)/bootsect.s $(BOOTMODULE)/setup.s
	mkdir -p $(BUILDDIR)/$(BOOTMODULE)
	nasm -o $(BUILDDIR)/$(BOOTMODULE)/bootsect.bin $(BOOTMODULE)/bootsect.s
	nasm -o $(BUILDDIR)/$(BOOTMODULE)/setup.bin $(BOOTMODULE)/setup.s
	dd if=$(BUILDDIR)/$(BOOTMODULE)/bootsect.bin of=$@ bs=512 count=1 conv=notrunc
	dd if=$(BUILDDIR)/$(BOOTMODULE)/setup.bin of=$@ bs=512 count=4 conv=notrunc seek=1

$(BUILDDIR)/system.bin:$(patsubst %,$(BUILDDIR)/%.o,$(ALLSRCFILES)) $(BUILDDIR)/head.o $(BUILDDIR)/main.o
	$(CC) $(CFLAGS) -Wl,--oformat=binary,-m,elf_i386,-M,-x,-e,startup_32,--section-start,.text=0xc0100000 -static \
	-o $@ $(BUILDDIR)/head.o $(BUILDDIR)/main.o $(patsubst %,$(BUILDDIR)/%.o,$(ALLSRCFILES)) -lgcc |\
	tee $(BUILDDIR)/os.symbols.origin |\
	grep -E "(^\s+0x[0-9a-f]+\s+[a-zA-Z_]\S+$$)|(^\s*\.text\s+0x[0-9a-f]+\s+0x[0-9a-f]+\s+[a-zA-Z_]\S+$$)" |\
	sed -r "s/^\s*\.text\s+(0x[0-9a-f]+)\s+0x[0-9a-f]+\s+([a-zA-Z_]\S+)$$/\1 \2/" |\
	sed -r "s/obj\/([a-zA-Z_]\S+)\.so/_______________\1.s/" |\
	sed -r "s/obj\/([a-zA-Z_]\S+)\.o/_______________\1.c/" |\
	sed -r "s/^[\t ]+//" | sed -r "s/[\t ]+/ /" |\
	sed -r "N;s/(0x[0-9a-f]+)\s+([a-zA-Z_]\S+)\s*\n\1\s+([a-zA-Z_]\S+)/\1 \2[\3]/;" > $(BUILDDIR)/os.symbols

$(BUILDDIR)/%.s.o:$(SRCDIR)/%.s
	@mkdir -p $(dir $@)
	$(ASM) -f elf -o $@ $^

$(BUILDDIR)/%.cpp.o:$(SRCDIR)/%.cpp $(BUILDDIR)/%.cpp.d
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.cpp.d:$(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(RM) $@
	@$(CC) -MM $(CFLAGS) $< | sed 's/\($(patsubst %.c,%,$(notdir $<))\)\.o *: */obj\/\1.o dep\/\1.d:/' > $@

$(BUILDDIR)/head.o:$(SRCDIR)/head.s
	$(ASM) -f elf -o $@ $^

$(BUILDDIR)/main.o:$(SRCDIR)/main.cpp $(BUILDDIR)/main.cpp.d
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/main.cpp.d:$(SRCDIR)/main.cpp
	@$(RM) $@
	@$(CC) -MM $(CFLAGS) $< | sed 's/\($(patsubst %.c,%,$(notdir $<))\)\.o *: */obj\/\1.o dep\/\1.d:/' > $@

ifneq ($(MAKECMDGOALS),clean)
sinclude $(BUILDDIR)/main.cpp.d
sinclude $(patsubst %,$(BUILDDIR)/%.d,$(CPPSRCFILES))
endif
