JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../base

LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm 

pantilt: pantilt.o 
	$(GCC) -shared -Bsymbolic -Wl,-soname,pantilt.so $(LIB-DIR) -o pantilt.so pantilt.o  $(LIBS)

pantilt.o: pantilt.c 
	$(GCC) $(CFLAGS) -fPIC -c pantilt.c

clean:
	rm -f *.o pantilt.so

install:
	cp pantilt.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c makefile

