JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../include -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm  -lX11 -lforms

papito: papito.o papitogui.o
	$(GCC) -shared -Wl,-soname,papito.so.1 $(LIB-DIR) -o papito.so.1.0 papito.o  papitogui.o $(LIBS)

papito.o: papito.c papito.h
	$(GCC) $(CFLAGS) -fPIC -c papito.c

papitogui.o: papitogui.c papitogui.h
	$(GCC) $(CFLAGS) -fPIC -c papitogui.c

clean:
	rm -f *.o papito.so.1.0 

install:
	cp papito.so* $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h makefile

