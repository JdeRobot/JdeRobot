JDEDIR = /usr/local
INC-DIR = -I../../core -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

myperceptive: myperceptive.o myperceptivegui.o
	$(GCC) -shared -Wl,-soname,myperceptive.so $(LIB-DIR) -o myperceptive.so myperceptive.o  myperceptivegui.o $(LIBS)

myperceptivegui.o: myperceptivegui.c myperceptivegui.h
	$(GCC) $(CFLAGS) -fPIC -c myperceptivegui.c

myperceptivegui.c: myperceptivegui.fd
	fdesign -convert myperceptivegui.fd

myperceptive.o: myperceptive.c myperceptive.h myperceptivegui.o
	$(GCC) $(CFLAGS) -fPIC -c myperceptive.c

clean:
	rm -f *.o myperceptive.so

install:
	cp myperceptive.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

