CFLAGS += -Wall -Wextra -pedantic -Wno-long-long -Wfatal-errors
CPPFLAGS += -DLIBCMP
CRAM_OPTIONS =

ifneq (,$(findstring BSD,$(shell uname -s)))
CPPFLAGS +=       -DOPEN_EMLINK
endif

.PHONY: all
all: cmp

.PHONY: check
check: all
	cram $(CRAM_OPTIONS) tests

cmp: cmp.o link.o misc.o regular.o special.o
