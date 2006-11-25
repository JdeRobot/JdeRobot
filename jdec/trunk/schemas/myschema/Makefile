JDEDIR = /usr/local
INC-DIR = -I../../core -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

myschema: myschema.o myschemagui.o
	$(GCC) -shared -Wl,-soname,myschema.so.1 $(LIB-DIR) -o myschema.so.1.0 myschema.o  myschemagui.o $(LIBS)

myschemagui.o: myschemagui.c myschemagui.h
	$(GCC) $(CFLAGS) -fPIC -c myschemagui.c

myschema.o: myschema.c myschema.h
	$(GCC) $(CFLAGS) -fPIC -c myschema.c

clean:
	rm -f *.o myschema.so.1.0 

install:
	cp myschema.so* $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

