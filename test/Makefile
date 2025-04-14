
CFLAGS :=  -I../include
SRCDIR := .
BUILDDIR := build
SHARED_LIB := ../build/libomena.so

SOURCES = $(wildcard $(SRCDIR)/*.c)
EXECUTABLES = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%)

all: $(EXECUTABLES)
	@for exe in $(EXECUTABLES); do echo "Running $$exe..."; ./$$exe; done

$(BUILDDIR)/%: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	@$(CC) $(CFLAGS) $< -o $@ $(SHARED_LIB)

clean:
	@rm -rf $(BUILDDIR)

.PHONY: all clean
