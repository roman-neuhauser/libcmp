CPPFLAGS += -DLIBCMP
CRAM_OPTIONS =

.PHONY: all
all: cmp

.PHONY: check
check: all
	cram $(CRAM_OPTIONS) tests

cmp: cmp.o link.o misc.o regular.o special.o
