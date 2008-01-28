JDEDIR = /usr/local
INC-DIR = -I../../base -I../../drivers/graphics_xforms  -I../../utils/pioneer/ -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L.  -L../../utils/pioneer/ -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms -lpioneer

introrob: introrobgui.o navegacion.o introrob.o 
	$(GCC) -shared -Wl,-soname,introrob.so $(LIB-DIR) -o introrob.so introrob.o introrobgui.o navegacion.o $(LIBS)

introrobgui.c: introrobgui.fd
	fdesign -code introrobgui.fd

introrobgui.o: introrobgui.c introrobgui.h
	$(GCC) $(CFLAGS) -fPIC -c introrobgui.c

introrob.o: introrob.c introrob.h navegacion.h introrobgui.o
	$(GCC) $(CFLAGS) -fPIC -c introrob.c

navegacion.o: navegacion.c
	$(GCC) $(CFLAGS) -fPIC -c navegacion.c

clean:
	rm -f *.o introrob.so introrobgui.h introrobgui.c 

install:
	cp introrob.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

