JDEDIR = /usr/local
INC_GLIB =  -I/usr/include -I/usr/include/glib-2.0/ -I/usr/include/glib-2.0/glib/ -I/usr/lib/glib-2.0/include/ 
INC-DIR = -I../../base -I$(INC_GLIB) -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I../../utils/colorspaces/ -I../../utils/progeo/ -I$(JDEDIR)/include/jde-robot  -I../../drivers/graphics_xforms
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib -L../../utils/colorspaces -L../../utils/progeo
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms  -lformsGL -lgsl -lgslcblas -lGL -lGLU -lglut -lglib-2.0 -lcolorspaces -lprogeo

all:calibrador.so

calibrador.so: calibrador.o calibradorgui.o clasificator.o segment.o
	$(GCC) -shared -Wl,-soname,calibrador.so $(LIB-DIR) -o calibrador.so calibrador.o  calibradorgui.o clasificator.o segment.o $(LIBS)

calibradorgui.o: calibradorgui.c calibradorgui.h
	$(GCC) $(CFLAGS) -fPIC -c calibradorgui.c

calibradorgui.c:
	fdesign -convert calibradorgui.fd

calibrador.o: calibrador.c calibrador.h calibradorgui.o
	$(GCC) $(CFLAGS) -fPIC -c calibrador.c

clasificator.o: clasificator.c clasificator.h
	$(GCC) $(CFLAGS) -fPIC -c clasificator.c 

segment.o: segment.c segment.h
	$(GCC) $(CFLAGS) -fPIC -c segment.c	

clean:
	rm -f *.o *~ calibrador.so *.bak calibradorgui.c calibradorgui.h

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

