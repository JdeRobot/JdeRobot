JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../core

LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

hsituner: hsituner.o hsitunergui.o
	$(GCC) -shared -Wl,-soname,hsituner.so.1 $(LIB-DIR) -o hsituner.so.1.0 hsituner.o  hsitunergui.o $(LIBS)

hsitunergui.o: hsitunergui.c hsitunergui.h
	$(GCC) $(CFLAGS) -fPIC -c hsitunergui.c

hsituner.o: hsituner.c hsituner.h
	$(GCC) $(CFLAGS) -fPIC -c hsituner.c

clean:
	rm -f *.o hsituner.so.1.0
 
install:
	cp hsituner.so* $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

