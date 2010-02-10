JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../base -I/usr/include/X11
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lpthread -lm -lforms -lX11 -ldl

graphics_xforms: graphics_xforms.o
	$(GCC) -shared -Bsymbolic -Wl,-soname,graphics_xforms.so $(LIB-DIR) -o graphics_xforms.so graphics_xforms.o  $(LIBS)

graphics_xforms.o: graphics_xforms.c
	$(GCC) $(CFLAGS) -fPIC -c graphics_xforms.c

clean:
	rm -f *.o graphics_xforms.so

install:
	cp graphics_xforms.so $(JDEDIR)/share/jde-robot/

dist:
	tar -zcvf graphics_xforms`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile

release:
	tar -zcvf newrelease.tgz *c makefile

