JDEDIR = /usr/local
INC-DIR = -I../../core -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms -lformsGL -lGL -lGLU

opengldemo: opengldemo.o opengldemogui.o
	$(GCC) -shared -Wl,-soname,opengldemo.so.1 $(LIB-DIR) -o opengldemo.so.1.0 opengldemo.o  opengldemogui.o $(LIBS)

opengldemogui.o: opengldemogui.c opengldemogui.h
	$(GCC) $(CFLAGS) -fPIC -c opengldemogui.c

opengldemo.o: opengldemo.c opengldemo.h
	$(GCC) $(CFLAGS) -fPIC -c opengldemo.c

clean:
	rm -f *.o opengldemo.so.1.0 

install:
	cp opengldemo.so* $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

