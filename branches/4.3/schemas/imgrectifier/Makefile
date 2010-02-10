JDEDIR = /usr/local
INC-DIRA = `pkg-config --cflags libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
LIB-DIRA = `pkg-config --libs libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
INC-DIR = -I../../base -I../../drivers/graphics_gtk -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot $(INC-DIRA)
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib $(LIB-DIRA)
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms -lgsl -lgslcblas -lm

all:imgrectifier.so

imgrectifier.so: imgrectifier.o
	$(GCC) -shared -Wl,-soname,imgrectifier.so $(LIB-DIR) -o imgrectifier.so imgrectifier.o $(LIBS)

imgrectifier.o: imgrectifier.c imgrectifier.h
	$(GCC) $(CFLAGS) -fPIC -c imgrectifier.c

clean:
	rm -f *.o *~ imgrectifier.so *.bak

install:
	cp imgrectifier.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf imgrectifier.tgz *c *h *glade Makefile

