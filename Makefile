LIB = serial.a
SRCS = src/serial.c src/version.c

SRCDIR = src
OBJDIR = obj

TEST_PROGRAMS = $(basename $(wildcard tests/test_serial.c))

###########################################################################

OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

GPIO_CDEV_SUPPORT := $(shell ! echo "\#include <linux/gpio.h>" | $(CC) -E - >/dev/null 2>&1; echo $$?)
COMMIT_ID := $(shell git describe --abbrev --always --tags --dirty 2>/dev/null || echo "")

CFLAGS += -std=gnu99 -pedantic
CFLAGS += -Wall -Wextra -Wno-unused-parameter $(DEBUG) -fPIC
CFLAGS += -DPERIPHERY_VERSION_COMMIT=\"$(COMMIT_ID)\" -DPERIPHERY_GPIO_CDEV_SUPPORT=$(GPIO_CDEV_SUPPORT)
LDFLAGS +=

ifdef CROSS_COMPILE
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
endif

###########################################################################

.PHONY: all
all: $(LIB)

.PHONY: tests
tests: $(TEST_PROGRAMS)

.PHONY: clean
clean:
	rm -rf $(LIB) $(OBJDIR) $(TEST_PROGRAMS)

###########################################################################

tests/%: tests/%.c $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(LIB) -o $@ -lpthread

###########################################################################

$(OBJECTS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(LIB): $(OBJECTS)
	$(AR) rcs $(LIB) $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

