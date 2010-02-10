JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../base 
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib -lcwiid
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm

wiimote: wiimote.o
	$(GCC) -shared -Bsymbolic -Wl,-soname,wiimote.so $(LIB-DIR) -o wiimote.so wiimote.o  $(LIBS)

wiimote.o: wiimote.c
	$(GCC) $(CFLAGS) -fPIC -c wiimote.c

clean:
	rm -f *.o wiimote.so

install:
	cp wiimote.so $(JDEDIR)/share/jde-robot/

dist:
	tar -zcvf wiimote`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile

release:
	tar -zcvf newrelease.tgz *c *h *conf Makefile

