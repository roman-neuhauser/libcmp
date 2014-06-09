CFLAGS +=         -Wall -Wextra -pedantic -Wno-long-long -Wfatal-errors
CPPFLAGS +=       -DLIBCMP
CRAM_OPTIONS =

GZIPCMD ?=        gzip
RST2HTML ?=       $(call first_in_path,rst2html.py rst2html)

name =            cmp

installed =       $(name).1.gz $(name)
artifacts =       $(installed) README.html $(objects)

sources =         cmp.c link.c misc.c regular.c special.c
objects =         $(sources:%.c=%.o)

.DEFAULT_GOAL := most

.PHONY: all
all: $(artifacts)

.PHONY: most
most: $(installed)

.PHONY: clean
clean:
	$(RM) $(artifacts)

.PHONY: check
check: all
	cram $(CRAM_OPTIONS) tests

.PHONY: html
html: README.html

%.gz: %
	$(GZIPCMD) -n < $< > $@

%.html: %.rest
	$(RST2HTML) $< $@

cmp: cmp.o link.o misc.o regular.o special.o
$(objects): extern.h

define first_in_path
  $(or \
    $(firstword $(wildcard \
      $(foreach p,$(1),$(addsuffix /$(p),$(subst :, ,$(PATH)))) \
    )) \
  , $(error Need one of: $(1)) \
  )
endef

