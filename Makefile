DESTDIR=/usr/local

CFLAGS += -O2 -Wall -Werror -pedantic -std=gnu99 $(shell pkg-config --cflags libxml-2.0)

LDLIBS=$(shell pkg-config --libs libxml-2.0)
sigma-loader: i2c.c 

install:
	install -d $(DESTDIR)/bin
	install sigma-loader $(DESTDIR)/bin

clean:
	rm -rf sigma-loader *.o
