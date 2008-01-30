JDEDIR = /usr/local
INC-DIR = -I../../base -I../../drivers/graphics_xforms -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

myschema: myschema.o myschemagui.o
	$(GCC) -shared -Wl,-soname,myschema.so $(LIB-DIR) -o myschema.so myschema.o  myschemagui.o $(LIBS)

myschemagui.o: myschemagui.c myschemagui.h
	$(GCC) $(CFLAGS) -fPIC -c myschemagui.c

myschemagui.c: myschemagui.fd
	fdesign -convert myschemagui.fd

myschema.o: myschema.c myschema.h myschemagui.o
	$(GCC) $(CFLAGS) -fPIC -c myschema.c

clean:
	rm -f *.o myschema.so myschemagui.c myschemagui.h

install:
	cp myschema.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

