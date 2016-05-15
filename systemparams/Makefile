CPPSOURCE=$(wildcard $(SRCDIR)/*.cpp)
ASMSOURCE=$(wildcard $(SRCDIR)/*.s)

.PHONY: all clean

all:$(BUILDDIR)/out.a

clean:

$(BUILDDIR)/out.a:$(patsubst $(SRCDIR)/%,$(BUILDDIR)/%.o,$(CPPSOURCE) $(ASMSOURCE))
	$(AR) rc $@ $^

$(BUILDDIR)/%.cpp.o:$(SRCDIR)/%.cpp $(BUILDDIR)/%.cpp.d
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.cpp.d::$(SRCDIR)/%.cpp
	@$(RM) $@
	@$(CC) -MM $(CFLAGS) $< | sed 's/\($(patsubst %.c,%,$(notdir $<))\)\.o *: */obj\/\1.o dep\/\1.d:/' > $@

$(BUILDDIR)/%.s.o:$(SRCDIR)/%.s
	$(ASM) -f elf -o $@ $^

ifneq ($(MAKECMDGOALS),clean)
sinclude $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%.d,$(CPPSOURCE))
endif
