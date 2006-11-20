JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

perceptive1: perceptive1.o perceptive1gui.o
	$(GCC) -shared -Wl,-soname,perceptive1.so.1 $(LIB-DIR) -o perceptive1.so.1.0 perceptive1.o  perceptive1gui.o $(LIBS)

perceptive1gui.o: perceptive1gui.c perceptive1gui.h
	$(GCC) $(CFLAGS) -fPIC -c perceptive1gui.c

perceptive1.o: perceptive1.c perceptive1.h
	$(GCC) $(CFLAGS) -fPIC -c perceptive1.c

clean:
	rm -f *.o perceptive1.so.1.0 

install:
	cp perceptive1.so* $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

