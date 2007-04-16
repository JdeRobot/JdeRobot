JDEDIR = /usr/local
INC-DIR = -I../../core -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

speedy: speedy.o 
	$(GCC) -shared -Wl,-soname,speedy.so $(LIB-DIR) -o speedy.so speedy.o $(LIBS)

speedy.o: speedy.c speedy.h 
	$(GCC) $(CFLAGS) -fPIC -c speedy.c

clean:
	rm -f *.o speedy.so

install:
	cp speedy.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h Makefile

