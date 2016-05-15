.PHONY: all clean

all:$(BUILDDIR)/out.a

clean:
	$(RM) $(BUILDDIR)/out.a
	$(RM) $(BUILDDIR)/bootsect.bin
	$(RM) $(BUILDDIR)/setup.bin

$(BUILDDIR)/out.a:$(BUILDDIR)/bootsect.bin $(BUILDDIR)/setup.bin
	dd if=$(BUILDDIR)/bootsect.bin of=$@ bs=512 count=1 conv=notrunc
	dd if=$(BUILDDIR)/setup.bin of=$@ bs=512 count=4 conv=notrunc seek=1

$(BUILDDIR)/%.bin:%.s
	nasm -o $@ $^
