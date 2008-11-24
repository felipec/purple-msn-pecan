CC := gcc

PLATFORM := $(shell uname -s)

LIBMSN_PECAN_CFLAGS := $(shell pkg-config --cflags msn-pecan)
LIBMSN_PECAN_LIBS := $(shell pkg-config --libs msn-pecan)

PURPLE_CFLAGS := $(shell pkg-config --cflags purple)
PURPLE_LIBS := $(shell pkg-config --libs purple)
PURPLE_PREFIX := $(shell pkg-config --variable=prefix purple)

GOBJECT_CFLAGS := $(shell pkg-config --cflags gobject-2.0)
GOBJECT_LIBS := $(shell pkg-config --libs gobject-2.0)

ifdef DEBUG
CFLAGS += -ggdb
else
CFLAGS += -O2
endif

SIMPLE_WARNINGS := -Wextra -ansi -std=c99 -Wno-unused-parameter

CFLAGS += -Wall $(SIMPLE_WARNINGS)

override CFLAGS += -I. -D PLUGIN_NAME='msn-pecan'

LDFLAGS := -Wl,--no-undefined

prefix := $(DESTDIR)/$(PURPLE_PREFIX)
plugin_dir := $(prefix)/lib/purple-2
data_dir := $(prefix)/share

objects := plugin.o

deps := $(objects:.o=.d)

SHLIBEXT=so

plugin := libmsn-pecan.$(SHLIBEXT)
override CFLAGS += -fPIC

.PHONY: clean

all: $(plugin)

version := 0.1

# from Lauri Leukkunen's build system
ifdef V
Q = 
P = @printf "" # <- space before hash is important!!!
else
P = @printf "[%s] $@\n" # <- space before hash is important!!!
Q = @
endif

$(plugin): $(objects)
$(plugin): CFLAGS := $(CFLAGS) $(PURPLE_CFLAGS) $(GOBJECT_CFLAGS) $(LIBMSN_PECAN_CFLAGS) -D VERSION='"$(version)"'
$(plugin): LIBS := $(PURPLE_LIBS) $(GOBJECT_LIBS) $(LIBMSN_PECAN_LIBS)

%.so::
	$(P)SHLIB
	$(Q)$(CC) $(LDFLAGS) -shared -o $@ $^ $(LIBS)

%.o:: %.c
	$(P)CC
	$(Q)$(CC) $(CFLAGS) -MMD -o $@ -c $<

clean:
	rm -f $(plugin) $(objects) $(deps)

install: $(plugin)
	mkdir -p $(plugin_dir)
	install $(plugin) $(plugin_dir)
	# chcon -t textrel_shlib_t $(plugin_dir)/$(plugin)

-include $(deps)
