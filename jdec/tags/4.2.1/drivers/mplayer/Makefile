JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../base 
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm

mplayer: mplayer.o
	$(GCC) -shared -Bsymbolic -Wl,-soname,mplayer.so $(LIB-DIR) -o mplayer.so mplayer.o  $(LIBS)

mplayer.o: mplayer.c
	$(GCC) $(CFLAGS) -fPIC -c mplayer.c

clean:
	rm -f *.o mplayer.so

install:
	cp mplayer.so $(JDEDIR)/share/jde-robot/

dist:
	tar -zcvf mplayer`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile

release:
	tar -zcvf newrelease.tgz *c makefile

