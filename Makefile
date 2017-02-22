DESTDIR=/usr/local

CFLAGS += -O2 -Wall -Werror -pedantic -std=gnu99 $(shell pkg-config --cflags libxml-2.0)

LDLIBS=$(shell pkg-config --libs libxml-2.0)
sigma_loader: i2c.c 

install:
	install -d /usr/sbin
	install sigma_loader /usr/sbin

clean:
	rm -rf sigma_loader *.o
