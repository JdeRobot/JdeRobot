JDEDIR = /usr/local
INC-DIR = -I../../core -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

introrob: introrob.o introrobgui.o
	$(GCC) -shared -Wl,-soname,introrob.so $(LIB-DIR) -o introrob.so introrob.o  introrobgui.o $(LIBS)

introrobgui.o: introrobgui.c introrobgui.h
	$(GCC) $(CFLAGS) -fPIC -c introrobgui.c

introrob.o: introrob.c introrob.h
	$(GCC) $(CFLAGS) -fPIC -c introrob.c

clean:
	rm -f *.o introrob.so 

install:
	cp introrob.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

