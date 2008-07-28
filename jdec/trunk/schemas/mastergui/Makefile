JDEDIR = /usr/local
INC-DIRA = `pkg-config --cflags libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
LIB-DIRA = `pkg-config --libs libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
INC-DIR = -I../../base -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot -I../../drivers/graphics_gtk $(INC-DIRA)
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib $(LIB-DIRA)
GCC = gcc
CFLAGS = -g -O -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

mastergui: mastergui.o
	$(GCC) -shared -Wl,-soname,mastergui.so $(LIB-DIR) -o mastergui.so mastergui.o $(LIBS)

mastergui.o: mastergui.c mastergui.h
	$(GCC) $(CFLAGS) -fPIC -c mastergui.c

clean:
	rm -f *.o mastergui.so

install:
	cp mastergui.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h Makefile

dist:
	tar -zcvf mastergui`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile
