CC ?= cc
CFLAGS := -Wall -Werror -std=c99 -I. -std=c99 $(CFLAGS)

ifeq ($(shell uname),Darwin)
MACOSX_VERSION_MIN ?= 10.5
CFLAGS := -mmacosx-version-min=$(MACOSX_VERSION_MIN)
endif

ifdef $(DEBUG)
CFLAGS := -g $(CFLAGS) -DDEBUG=1
endif

AR ?= ar

OUT = libtfbase.a
TARGETS = list.o misc.o stringbuilder.o

all: $(OUT)

$(OUT): $(TARGETS)
	$(AR) crs $(OUT) $(TARGETS)

$(TARGETS):
	$(CC) -c -o $@ $(CFLAGS) $(shell basename "$@" .o).c

clean: distclean

distclean:
	-rm -f $(TARGETS) $(OUT)
	-rm -rf *.dSYM
