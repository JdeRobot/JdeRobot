JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../base
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm 

imagefile: imagefile.o 
	$(GCC) -shared -Wl,-soname,imagefile.so $(LIB-DIR) -o imagefile.so imagefile.o  $(LIBS)

imagefile.o: imagefile.c 
	$(GCC) $(CFLAGS) -fPIC -c imagefile.c

clean:
	rm -f *.o imagefile.so 

install:
	cp imagefile.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *ppm *pnm makefile

