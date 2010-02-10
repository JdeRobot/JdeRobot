JDEDIR = /usr/local
INC-DIRGTK = `pkg-config --cflags gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0 libglade-2.0`
LIB-DIRGTK = `pkg-config --libs gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0 libglade-2.0`
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../base $(INC-DIRGTK)
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib $(LIB-DIRGTK)
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm

graphics_gtk: graphics_gtk.o
	$(GCC) -shared -Bsymbolic -Wl,-soname,graphics_gtk.so $(LIB-DIR) -o graphics_gtk.so graphics_gtk.o  $(LIBS)

graphics_gtk.o: graphics_gtk.c
	$(GCC) $(CFLAGS) -fPIC -c graphics_gtk.c

clean:
	rm -f *.o graphics_gtk.so

install:
	cp graphics_gtk.so $(JDEDIR)/share/jde-robot/

dist:
	tar -zcvf graphics_gtk`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile

release:
	tar -zcvf newrelease.tgz *c makefile

