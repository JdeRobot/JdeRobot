JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../base 
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm

x10: x10.o
	$(GCC) -shared -Bsymbolic -Wl,-soname,x10.so $(LIB-DIR) -o x10.so x10.o  $(LIBS)

x10.o: x10.c
	$(GCC) $(CFLAGS) -fPIC -c x10.c

clean:
	rm -f *.o x10.so

install:
	cp x10.so $(JDEDIR)/share/jde-robot/

dist:
	tar -zcvf x10`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile

release:
	tar -zcvf newrelease.tgz *c *h *conf Makefile

