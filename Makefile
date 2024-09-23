CFLAGS=-Wall -Wextra -Werror -O2 -std=c99 -pedantic
PREFIX?=/usr/local
TARGET=rrename

all: $(TARGET)

dev: CFLAGS += -g
dev: all

.PHONY: tests
tests: all
	sh ./test.sh

install: all
	install $(TARGET) $(DESTDIR)/$(PREFIX)/bin/

uninstall:
	rm -f $(DESTDIR)/$(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(TARGET)
