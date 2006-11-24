INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../core
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm 

imagefile: imagefile.o 
	$(GCC) -shared -Wl,-soname,imagefile.so.1 $(LIB-DIR) -o imagefile.so.1.0 imagefile.o  $(LIBS)

imagefile.o: imagefile.c 
	$(GCC) $(CFLAGS) -fPIC -c imagefile.c

clean:
	rm -f *.o imagefile.so.1.0 

release:
	tar -zcvf newrelease.tgz *c *ppm *pnm makefile

